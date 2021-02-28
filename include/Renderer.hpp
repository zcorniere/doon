#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include "Coords.hpp"
#include "DepthBuffer.hpp"
#include "Frame.hpp"
#include "Map.hpp"
#include "ObjectManager.hpp"
#include "Player.hpp"
#include "Storage.hpp"
#include "ThreadPool.hpp"
#include "abstract/AObject.hpp"
#include <atomic>
#include <execution>
#include <memory>
#include <unordered_map>

class Renderer
{
public:
    Renderer(ThreadPool &, const Storage &, const Map &, const Coords<unsigned>);
    ~Renderer();
    const uint8_t *update(const ObjectManager &, const unsigned);
    void resize(Coords<unsigned>);

private:
    const Pixel sampleTexture(const Coords<float> &, const std::string &) const;
    const Coords<unsigned> sampleCoords(const Coords<float> &fSample,
                                        const Coords<float> &fSize) const
    {
        return {std::min(unsigned(fSample.x * fSize.x), unsigned(fSize.x) - 1),
                std::min(unsigned(fSample.y * fSize.y), unsigned(fSize.y) - 1)};
    }

    float computeColumn(const unsigned &, const float, const Coords<float> &,
                        Coords<float> &) const;
    void drawColumn(const float &, const unsigned x, Coords<float> &);

    void drawObject(const std::unique_ptr<AObject> &, const Coords<float> &,
                    const float &);

private:
    Frame img;
    Coords<unsigned> size;

    DepthBuffer qDepthBuffer;

    ThreadPool &pool;
    const Storage &storage;
    const Map &map;
};

#endif    //_RENDERER_HPP_
