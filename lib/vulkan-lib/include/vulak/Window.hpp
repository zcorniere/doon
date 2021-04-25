#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace vulak
{
class Window
{
public:
    Window(std::string, unsigned, unsigned);
    ~Window();
    Window(Window &) = delete;
    Window(const Window &) = delete;
    bool shouldClose();
    void createSurface(VkInstance &, VkSurfaceKHR *);

private:
    void initWindow();

private:
    unsigned width;
    unsigned height;
    std::string windowName;
    GLFWwindow *window = nullptr;
};
}    // namespace vulak
