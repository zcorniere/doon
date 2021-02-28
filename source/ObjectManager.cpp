#include "ObjectManager.hpp"
#include "Logger.hpp"
#include <execution>
#include <functional>

ObjectManager::ObjectManager(ThreadPool &p, const Map &m): map(m), pool(p) {}

ObjectManager::~ObjectManager() {}

void ObjectManager::update(float fElapsedTime)
{
    std::deque<std::future<void>> fut;

    for (auto &i: qObjects) {
        fut.push_back(pool.push(
            [&i, this](int, float fElapsedTime) {
                Coords<float> fPotential(i->update(fElapsedTime));
                Coords<float> fSolved(fPotential);
                Coords<unsigned> uCurrent(i->getPosition().floor());
                Coords<unsigned> uTarget(fSolved);

                Coords<unsigned> uTL((uCurrent.min(uTarget) - 1).max({0, 0}));
                Coords<unsigned> uBR((uCurrent.max(uTarget) + 1).min(map.getSize()));

                Coords<unsigned> uCell;

                for (uCell.y = uTL.y; uCell.y <= uBR.y; uCell.y++) {
                    for (uCell.x = uTL.x; uCell.x <= uBR.x; uCell.x++) {
                        if (map.at(uCell) == '#') {
                            Coords<float> fNearestPoint(
                                std::clamp<float>(fSolved.x, uCell.x, uCell.x + 1),
                                std::clamp<float>(fSolved.y, uCell.y, uCell.y + 1));

                            Coords<float> fRayNear(fNearestPoint - fSolved);
                            float fOverlap = i->getSize() - fRayNear.mag();
                            if (std::isnan(fOverlap)) fOverlap = 0;

                            if (fOverlap > 0) {
                                fSolved -= fRayNear.norm() * fOverlap;
                                if (std::isnan(fSolved.x) || std::isnan(fSolved.y)) {
                                    logger.err("COLLISION")
                                        << "fSolved is nan, thus stopping collsion "
                                           "detection; fSolved defaulted to object's "
                                           "previous position";
                                    logger.endl();
                                    fSolved = i->getPosition();
                                    break;
                                }
                            }
                        }
                    }
                }
                if (fSolved != fPotential)
                    i->onSceneryCollision(map, fSolved, fPotential);
                i->setPosition(fPotential);
            },
            fElapsedTime));
    }

    std::for_each(std::execution::par, fut.begin(), fut.end(), [](auto &i) { i.wait(); });
    this->computeCollision();
    std::erase_if(qObjects, [](auto &i) { return i->needRemove(); });
}

void ObjectManager::computeCollision()
{
    for (auto &s: qObjects) {
        if (s->needRemove()) continue;
        for (auto &i: qObjects) {
            if (s == i || i->needRemove()) continue;
            Coords<float> fVec(s->getPosition<float>() - i->getPosition<float>());
            if (std::abs(fVec.mag()) <= i->getSize()) {
                i->onCollision(s);
                s->onCollision(i);
            }
        }
    }
}
