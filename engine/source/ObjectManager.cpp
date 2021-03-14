#include "ObjectManager.hpp"
#include "Logger.hpp"
#include <cmath>
#include <execution>
#include <functional>

ObjectManager::ObjectManager(ThreadPool &p): pool(p) {}

ObjectManager::~ObjectManager() {}

void ObjectManager::update(const IMap &map, const float fElapsedTime)
{
    std::deque<std::future<void>> fut;

    for (auto &i: qObjects) {
        fut.push_back(pool.push(
            [&i, &map, this](int, float fElapsedTime) {
                if (map.isLocationSolid(i->getPosition())) i->setRemove(true);
                if (i->needRemove()) return;
                Coords<float> fPotential(i->update(fElapsedTime));
                if (fPotential == i->getPosition()) return;
                this->computeCollision(i);
                Coords<float> fSolved(this->resolveWallCollision(map, i, fPotential));
                if (fSolved != fPotential) i->onSceneryCollision(fSolved, fPotential);
                i->setPosition(fPotential);
            },
            fElapsedTime));
    }

    std::for_each(std::execution::par, fut.begin(), fut.end(), [](auto &i) { i.wait(); });
    std::erase_if(qObjects, [](auto &i) { return i->needRemove(); });
}

Coords<float> ObjectManager::resolveWallCollision(const IMap &map,
                                                  std::unique_ptr<AObject> &obj,
                                                  const Coords<float> &fPotential) const
{
    Coords<float> fSolved(fPotential);
    Coords<unsigned> uCurrent(obj->getPosition().floor());
    Coords<unsigned> uTarget(fSolved);

    Coords<unsigned> uTL(std::min(uCurrent.x, uTarget.x) - 1,
                         std::min(uCurrent.y, uTarget.y) - 1);
    Coords<unsigned> uBR(std::max(uCurrent.x, uTarget.x) + 1,
                         std::max(uCurrent.y, uTarget.y) + 1);

    Coords<unsigned> uCell;
    for (uCell.y = uTL.y; uCell.y <= uBR.y; uCell.y++) {
        for (uCell.x = uTL.x; uCell.x <= uBR.x; uCell.x++) {
            if (map.isLocationSolid(uCell)) {
                Coords<float> fNearestPoint(
                    std::clamp<float>(fSolved.x, uCell.x, uCell.x + 1),
                    std::clamp<float>(fSolved.y, uCell.y, uCell.y + 1));

                Coords<float> fRayNear(fNearestPoint - fSolved);
                float fOverlap = obj->getSize() - fRayNear.mag();
                if (std::isnan(fOverlap)) fOverlap = 0;

                if (fOverlap > 0) {
                    fSolved -= fRayNear.norm() * fOverlap;
                    if (std::isnan(fSolved)) {
                        logger.err("COLLISION")
                            << "fSolved is nan, thus stopping collsion "
                               "detection; fSolved defaulted to object's "
                               "previous position";
                        logger.endl();
                        fSolved = obj->getPosition();
                    }
                }
            }
        }
    }
    return fSolved;
}

void ObjectManager::computeCollision(std::unique_ptr<AObject> &obj)
{
    for (auto &i: qObjects) {
        if (obj == i || i->needRemove()) continue;
        if ((i->getPosition() - obj->getPosition()).mag2() <=
            std::pow((i->getSize() * obj->getSize()), 2)) {
            i->onCollision(obj);
            obj->onCollision(i);
        }
    }
}
