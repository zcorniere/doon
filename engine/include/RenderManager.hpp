#pragma once

#include "DepthBuffer.hpp"
#include "Frame.hpp"
#include "Ray.hpp"
#include "Vector.hpp"
#include "abstract/AObject.hpp"
#include <atomic>
#include <memory>
#include <unordered_map>

constexpr const float fDepth = 100;

class RenderManager
{
public:
    RenderManager();
    RenderManager(const RenderManager &) = delete;
    RenderManager(RenderManager &) = delete;
    ~RenderManager();

    const Frame &update(const unsigned);
    void resize(Vector<unsigned>);

private:
    template <typename S, typename Si>
    constexpr static Vector<Si> sampleVector(const Vector<S> &fSample,
                                             const Vector<Si> &fSize)
    {
        return {std::min(Si(fSample.x * fSize.x), Si(fSize.x) - 1),
                std::min(Si(fSample.y * fSize.y), Si(fSize.y) - 1)};
    }

    static float sampleColumn(const Ray &) noexcept;
    void drawColumn(const Map &, const unsigned, const Ray &, const float);
    void drawObject(const std::unique_ptr<AObject> &, const Vector<float> &,
                    const float &);

private:
    bool bDrawDebug = false;
    Frame img;
    Vector<unsigned> size;

    DepthBuffer qDepthBuffer;
};

extern RenderManager *render_manager;
