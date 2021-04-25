#pragma once

#include "DeletionQueue.hpp"
#include "Window.hpp"

#include <VkBootstrap.h>
#include <vk_mem_alloc.h>

namespace vulak
{
class Device
{
public:
#ifdef NDEBUG
    static const bool enableValidationLayers = false;
#else
    static const bool enableValidationLayers = true;
#endif

public:
    Device(Window &);
    ~Device();

    Device(Device &) = delete;
    Device(const Device &) = delete;
    Device(Device &&) = delete;
    Device operator=(Device &&) = delete;

    void immediate_submit(std::function<void(VkCommandBuffer cmd)> &&);
    VkDevice &getDevice() { return device; }

private:
    void init_vulkan();
    void init_command_pool();
    void init_instant_upload_context();

private:
    Window &window;
    VmaAllocator allocator;
    VkDevice device;
    VkInstance instance;
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT debug_msg;
    VkQueue graphicsQueue;
    uint32_t graphicsQueueFamily;

    VkCommandPool commandPool;

    VkSurfaceKHR surface;

    DeleteionQueue mainDeletionQueue;

    VkFence instantUploadFence;
    VkCommandPool instantCommandPool;
};
}    // namespace vulak

#define VK_TRY(x)                                                                \
    {                                                                            \
        VkResult err = x;                                                        \
        if (err) throw std::runtime_error("VULKAN_ERROR" + std::to_string(err)); \
    }