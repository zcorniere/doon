#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Frame.hpp"
#include "Map.hpp"
#include "Vector.hpp"
#include "VulkanWrapper.hpp"
#include <tuple>
#include <vector>

class GameInstance
{
public:
    GameInstance(const Vector<unsigned> &);
    ~GameInstance();
    void init();
    void run();
    const Vector<unsigned> &getSize() const;

private:
    void populateMap(const Map &);
    void handleInput(const float &);

private:
    Vector<unsigned> uSize;
    VulkanWrapper vulkan;
    // std::vector<std::tuple<sf::Texture, sf::Sprite>> extraSprites;
    GLFWwindow *window = nullptr;
};
