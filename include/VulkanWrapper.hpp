#pragma once
#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <VkBootstrap.h>
#include <vector>

#include "DeletionQueue.hpp"
#include "Pixel.hpp"
#include "Vector.hpp"

constexpr const unsigned FRAME_OVERLAP = 2;

struct AllocatedBuffer {
    VkBuffer buffer;
    VmaAllocation allocation;

    static AllocatedBuffer create(VmaAllocator &, size_t, VkBufferUsageFlags,
                                  VmaMemoryUsage);
};

struct FrameData {
    VkCommandPool commandPool;
    VkCommandBuffer mainCommandBuffer;

    VkSemaphore presentSemaphore;
    VkSemaphore renderSemaphore;
    VkFence renderFence;

    AllocatedBuffer pictureBuffer;
    VkDescriptorSet pictureDescriptor;
};

class VulkanWrapper
{
public:
    VulkanWrapper(Vector<unsigned>);
    ~VulkanWrapper();
    bool init(GLFWwindow *);
    bool draw(const uint8_t *);
    void cleanup();

private:
    void init_vulkan();
    void init_swapchain();
    void init_command();
    void init_default_renderpass();
    void init_sync();
    void init_descriptor();

private:
    Vector<unsigned> uSize;
    DeleteionQueue mainDeletionQueue;
    bool isInit = false;
    GLFWwindow *window = nullptr;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_msg;
    VkPhysicalDevice physical_device;
    VkDevice device;
    VkSurfaceKHR surface;
    VkQueue graphicsQueue;
    uint32_t graphicsQueueFamily;
    VmaAllocator allocator;

    VkSwapchainKHR swapchain;
    VkFormat swapchainImageFormat;
    std::vector<VkImage> swapchainImages;
    std::vector<VkImageView> swapchainImagesViews;
    std::array<FrameData, FRAME_OVERLAP> frames;

    VkRenderPass renderPass;
    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout descriptorSetLayout;
};

#define VK_TRY(x)                                                                \
    {                                                                            \
        VkResult err = x;                                                        \
        if (err) throw std::runtime_error("VULKAN_ERROR" + std::to_string(err)); \
    }
