#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include "Coords.hpp"
#include "DepthBuffer.hpp"
#include "Frame.hpp"
#include "interface/IMap.hpp"
#include "interface/IGame.hpp"
#include "ObjectManager.hpp"
#include "ThreadPool.hpp"
#include "abstract/AObject.hpp"
#include <atomic>
#include <execution>
#include <memory>
#include <unordered_map>

class Renderer
{
public:
    struct Ray {
        float fFish = 0.0f;
        float fRayAngle = 0.0f;
        Coords<float> fOrigin;
        Coords<float> fDirection;
        float fDistance = 0.0f;
        Coords<float> fContact;
        Coords<float> fSample;
        char cHit;
    };

public:
    Renderer(ThreadPool &, const Coords<unsigned>);
    Renderer(const Renderer &) = delete;
    Renderer(Renderer &) = delete;

    ~Renderer();
    const uint8_t *update(const IGame &, const IMap &, const ObjectManager &, const unsigned);
    void resize(Coords<unsigned>);

private:
    const Coords<unsigned> sampleCoords(const Coords<float> &fSample,
                                        const Coords<float> &fSize) const
    {
        return {std::min(unsigned(fSample.x * fSize.x), unsigned(fSize.x) - 1),
                std::min(unsigned(fSample.y * fSize.y), unsigned(fSize.y) - 1)};
    }

    void computeColumn(const IMap &, Ray &) const;
    void drawColumn(const IMap &, const IGame &, const unsigned, Ray &);
    void drawObject(const IGame &, const std::unique_ptr<AObject> &, const Coords<float> &,
                    const float &);

private:
    Frame img;
    Coords<unsigned> size;

    DepthBuffer qDepthBuffer;

    ThreadPool &pool;
};

#endif    //_RENDERER_HPP_
