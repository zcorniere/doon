#include "vulak/Window.hpp"
#include <stdexcept>
namespace vulak
{

Window::Window(std::string n, unsigned w, unsigned h): width(w), height(h), windowName(n)
{
    initWindow();
}

Window::~Window()
{
    if (window != nullptr) glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::shouldClose() { return glfwWindowShouldClose(window); }

void Window::createSurface(VkInstance &instance, VkSurfaceKHR *surface)
{
    if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create surface");
    }
}

void Window::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}

}    // namespace vulak
