#include "Ray.hpp"
#include "RenderManager.hpp"

void Ray::shoot(const Map &map)
{
    Vector<float> fRayDelta(std::abs(1 / this->fDirection.x),
                            std::abs(1 / this->fDirection.y));
    Vector<float> fRayLength;
    Vector<int> iStep;

    if (this->fDirection.x < 0) {
        iStep.x = -1;
        fRayLength.x = (this->fOrigin.x - float(this->uMap.x)) * fRayDelta.x;
    } else {
        iStep.x = 1;
        fRayLength.x = (float(this->uMap.x) + 1.0f - this->fOrigin.x) * fRayDelta.x;
    }
    if (this->fDirection.y < 0) {
        iStep.y = -1;
        fRayLength.y = (this->fOrigin.y - float(this->uMap.y)) * fRayDelta.y;
    } else {
        iStep.y = 1;
        fRayLength.y = (float(this->uMap.y) + 1.0f - this->fOrigin.y) * fRayDelta.y;
    }

    while (!map.isLocationSolid(this->uMap) && this->fDistance < fDepth) {
        if (fRayLength.x < fRayLength.y) {
            this->uMap.x += iStep.x;
            this->fDistance = fRayLength.x;
            fRayLength.x += fRayDelta.x;
        } else {
            this->uMap.y += iStep.y;
            this->fDistance = fRayLength.y;
            fRayLength.y += fRayDelta.y;
        }
    }
    this->cHit = map.at(this->uMap);
}

void Ray::sample() noexcept
{
    Vector<float> fIntersection(this->fOrigin + this->fDirection * this->fDistance);
    Vector<float> fBlockMid(this->uMap);
    fBlockMid += 0.5f;
    float fTestAngle =
        std::atan2((fIntersection.y - fBlockMid.y), (fIntersection.x - fBlockMid.x));
    if (fTestAngle >= -M_PI * 0.25f && fTestAngle < M_PI * 0.25f)
        this->fSample = fIntersection.y - this->uMap.y;
    else if (fTestAngle >= M_PI * 0.25f && fTestAngle < M_PI * 0.75f)
        this->fSample = fIntersection.x - this->uMap.x;
    else if (fTestAngle < -M_PI * 0.25f && fTestAngle >= -M_PI * 0.75f)
        this->fSample = fIntersection.x - this->uMap.x;
    else if (fTestAngle >= M_PI * 0.75f || fTestAngle < -M_PI * 0.75f)
        this->fSample = fIntersection.y - this->uMap.y;
}
