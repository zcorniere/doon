#include "vulak/Device.hpp"
#include <iostream>
namespace vulak
{
Device::Device(Window &w): window(w)
{
    init_vulkan();
    init_command_pool();
    init_instant_upload_context();
}

Device::~Device() { mainDeletionQueue.flush(); }

void Device::immediate_submit(std::function<void(VkCommandBuffer)> &&function)
{
    VkCommandBufferAllocateInfo cmdAllocInfo{};
    cmdAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdAllocInfo.pNext = nullptr;
    cmdAllocInfo.commandPool = instantCommandPool;
    cmdAllocInfo.commandBufferCount = 1;
    cmdAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    VkCommandBuffer cmd{};
    VK_TRY(vkAllocateCommandBuffers(device, &cmdAllocInfo, &cmd));

    VkCommandBufferBeginInfo cmdBeginInfo{};
    cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBeginInfo.pNext = nullptr;
    cmdBeginInfo.pInheritanceInfo = nullptr;
    cmdBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VK_TRY(vkBeginCommandBuffer(cmd, &cmdBeginInfo));
    function(cmd);
    VK_TRY(vkEndCommandBuffer(cmd));

    VkSubmitInfo submit{};

    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit.pNext = nullptr;
    submit.pWaitDstStageMask = nullptr;
    submit.waitSemaphoreCount = 0;
    submit.pWaitSemaphores = nullptr;
    submit.signalSemaphoreCount = 0;
    submit.pSignalSemaphores = nullptr;
    submit.commandBufferCount = 1;
    submit.pCommandBuffers = &cmd;

    VK_TRY(vkQueueSubmit(graphicsQueue, 1, &submit, instantUploadFence));

    vkWaitForFences(device, 1, &instantUploadFence, true, 999999999999999);
    vkResetFences(device, 1, &instantUploadFence);
    vkResetCommandPool(device, instantCommandPool, 0);
}

void Device::init_vulkan()
{
    vkb::InstanceBuilder builder;
    auto inst_ret = builder.request_validation_layers(enableValidationLayers)
                        .require_api_version(1, 1, 0)
                        .use_default_debug_messenger()
                        .build()
                        .value();
    instance = inst_ret.instance;
    debug_msg = inst_ret.debug_messenger;
    window.createSurface(instance, &surface);

    vkb::PhysicalDeviceSelector selector{inst_ret};
    vkb::PhysicalDevice physicalDevice =
        selector.set_minimum_version(1, 1).set_surface(surface).select().value();
    physical_device = physicalDevice.physical_device;

    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(physical_device, &properties);
    std::cout << "Using device " << properties.deviceName << std::endl;

    vkb::DeviceBuilder deviceBuilder{physicalDevice};
    vkb::Device vkbDevice = deviceBuilder.build().value();
    device = vkbDevice.device;
    graphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
    graphicsQueueFamily =
        vkbDevice.get_dedicated_queue_index(vkb::QueueType::graphics).value();

    VmaAllocatorCreateInfo allocatorInfo{};
    allocatorInfo.physicalDevice = physical_device;
    allocatorInfo.device = device;
    allocatorInfo.instance = instance;
    VK_TRY(vmaCreateAllocator(&allocatorInfo, &allocator));

    mainDeletionQueue.push([this]() {
        vmaDestroyAllocator(allocator);
        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyDevice(device, nullptr);
        vkb::destroy_debug_utils_messenger(instance, debug_msg);
        vkDestroyInstance(instance, nullptr);
    });
}

void Device::init_instant_upload_context()
{
    VkCommandPoolCreateInfo uploadCommandPoolInfo{};
    uploadCommandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    uploadCommandPoolInfo.pNext = nullptr;
    uploadCommandPoolInfo.queueFamilyIndex = graphicsQueueFamily;
    uploadCommandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    VkFenceCreateInfo uploadFenceCreateInfo{};
    uploadFenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    uploadFenceCreateInfo.pNext = nullptr;
    uploadFenceCreateInfo.flags = 0;

    VK_TRY(vkCreateCommandPool(device, &uploadCommandPoolInfo, nullptr,
                               &instantCommandPool));
    VK_TRY(vkCreateFence(device, &uploadFenceCreateInfo, nullptr, &instantUploadFence));

    mainDeletionQueue.push([this]() {
        vkDestroyFence(device, instantUploadFence, nullptr);
        vkDestroyCommandPool(device, instantCommandPool, nullptr);
    });
}
void Device::init_command_pool()
{
    VkCommandPoolCreateInfo commandPoolInfo{};
    commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolInfo.pNext = nullptr;
    commandPoolInfo.queueFamilyIndex = graphicsQueueFamily;
    commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    VK_TRY(vkCreateCommandPool(device, &commandPoolInfo, nullptr, &commandPool));
    mainDeletionQueue.push(
        [this]() { vkDestroyCommandPool(device, commandPool, nullptr); });
}
}    // namespace vulak