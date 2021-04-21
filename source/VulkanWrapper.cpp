#include "VulkanWrapper.hpp"

AllocatedBuffer AllocatedBuffer::create(VmaAllocator &a, size_t size,
                                        VkBufferUsageFlags usage, VmaMemoryUsage memUsage)
{
    AllocatedBuffer buffer{};
    VkBufferCreateInfo bufferInfo{};
    VmaAllocationCreateInfo allocInfo{};

    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.pNext = nullptr;
    bufferInfo.size = size;
    bufferInfo.usage = usage;

    allocInfo.usage = memUsage;

    VK_TRY(vmaCreateBuffer(a, &bufferInfo, &allocInfo, &buffer.buffer, &buffer.allocation,
                           nullptr));
    return buffer;
}

VulkanWrapper::VulkanWrapper(Vector<unsigned> s): uSize(s) {}

VulkanWrapper::~VulkanWrapper() { this->cleanup(); }

bool VulkanWrapper::init(GLFWwindow *win)
{
    window = win;

    init_vulkan();
    init_swapchain();
    init_default_renderpass();
    init_sync();
    init_descriptor();

    isInit = true;
    return isInit;
}

bool VulkanWrapper::draw(const uint8_t *) { return true; }

void VulkanWrapper::cleanup()
{
    if (!isInit) return;

    vkDeviceWaitIdle(device);
    mainDeletionQueue.flush();
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyDevice(device, nullptr);
    vkb::destroy_debug_utils_messenger(instance, debug_msg);
    vkDestroyInstance(instance, nullptr);

    isInit = false;
}

void VulkanWrapper::init_vulkan()
{
    vkb::InstanceBuilder builder;
    auto inst_ret = builder.set_app_name("doon")
                        .request_validation_layers(true)
                        .require_api_version(1, 1, 0)
                        .use_default_debug_messenger()
                        .build()
                        .value();

    instance = inst_ret.instance;
    debug_msg = inst_ret.debug_messenger;
    VK_TRY(glfwCreateWindowSurface(instance, window, nullptr, &surface));

    vkb::PhysicalDeviceSelector selector{inst_ret};
    vkb::PhysicalDevice physicalDevice =
        selector.set_minimum_version(1, 1).set_surface(surface).select().value();
    physical_device = physicalDevice.physical_device;

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
    mainDeletionQueue.push([this]() { vmaDestroyAllocator(allocator); });
}

void VulkanWrapper::init_swapchain()
{
    vkb::SwapchainBuilder builder{physical_device, device, surface};
    vkb::Swapchain vkbSwapchain = builder.use_default_format_selection()
                                      .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
                                      .set_desired_extent(uSize.x, uSize.y)
                                      .build()
                                      .value();

    swapchain = vkbSwapchain.swapchain;
    swapchainImages = vkbSwapchain.get_images().value();
    swapchainImagesViews = vkbSwapchain.get_image_views().value();
    swapchainImageFormat = vkbSwapchain.image_format;
    mainDeletionQueue.push([this]() {
        vkDestroySwapchainKHR(device, swapchain, nullptr);
        for (auto &i: swapchainImagesViews) { vkDestroyImageView(device, i, nullptr); }
    });
}

void VulkanWrapper::init_command()
{
    VkCommandPoolCreateInfo commandPoolInfo{};
    commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolInfo.pNext = nullptr;
    commandPoolInfo.queueFamilyIndex = graphicsQueueFamily;
    commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    for (auto &i: frames) {
        VK_TRY(vkCreateCommandPool(device, &commandPoolInfo, nullptr, &i.commandPool));

        VkCommandBufferAllocateInfo commandBufferInfo{};
        commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferInfo.pNext = nullptr;
        commandBufferInfo.commandPool = i.commandPool;
        commandBufferInfo.commandBufferCount = 1;
        commandBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        VK_TRY(
            vkAllocateCommandBuffers(device, &commandBufferInfo, &i.mainCommandBuffer));
    }
    mainDeletionQueue.push([this]() {
        for (auto &i: frames) vkDestroyCommandPool(device, i.commandPool, nullptr);
    });
}

void VulkanWrapper::init_default_renderpass()
{
    VkAttachmentDescription color_attachment{};
    color_attachment.format = swapchainImageFormat;
    color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment_ref{};
    color_attachment_ref.attachment = 0;
    color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_ref;

    VkRenderPassCreateInfo render_pass_info{};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.attachmentCount = 1;
    render_pass_info.pAttachments = &color_attachment;
    render_pass_info.subpassCount = 1;
    render_pass_info.pSubpasses = &subpass;

    VK_TRY(vkCreateRenderPass(device, &render_pass_info, nullptr, &renderPass));
    mainDeletionQueue.push(
        [this]() { vkDestroyRenderPass(device, renderPass, nullptr); });
}

void VulkanWrapper::init_sync()
{
    VkFenceCreateInfo fenceCreateInfo{};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.pNext = nullptr;
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VkSemaphoreCreateInfo semaphoreCreateInfo{};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.pNext = nullptr;
    semaphoreCreateInfo.flags = 0;

    VkFenceCreateInfo uploadFenceCreateInfo{};
    uploadFenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    uploadFenceCreateInfo.pNext = nullptr;
    uploadFenceCreateInfo.flags = 0;

    for (auto &i: frames) {
        VK_TRY(vkCreateFence(device, &fenceCreateInfo, nullptr, &i.renderFence));
        VK_TRY(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr,
                                 &i.presentSemaphore));
        VK_TRY(
            vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &i.renderSemaphore));
    }
    mainDeletionQueue.push([this]() {
        for (auto &i: frames) {
            vkDestroyFence(device, i.renderFence, nullptr);
            vkDestroySemaphore(device, i.presentSemaphore, nullptr);
            vkDestroySemaphore(device, i.renderSemaphore, nullptr);
        }
    });
}

void VulkanWrapper::init_descriptor()
{
    std::vector<VkDescriptorPoolSize> sizes = {
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10},
    };
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.pNext = nullptr;
    poolInfo.maxSets = 10;
    poolInfo.poolSizeCount = (uint32_t)sizes.size();
    poolInfo.pPoolSizes = sizes.data();
    VK_TRY(vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool));

    VkDescriptorSetLayoutBinding setbind{};
    setbind.binding = 0;
    setbind.descriptorCount = 1;
    setbind.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    setbind.pImmutableSamplers = nullptr;
    setbind.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo setInfo{};
    setInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    setInfo.pNext = nullptr;
    setInfo.flags = 0;
    setInfo.bindingCount = 1;
    setInfo.pBindings = &setbind;

    VK_TRY(vkCreateDescriptorSetLayout(device, &setInfo, nullptr, &descriptorSetLayout));
    for (auto &i: frames) {
        i.pictureBuffer = AllocatedBuffer::create(
            allocator, sizeof(Pixel) * uSize.x * uSize.y,
            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.pNext = nullptr;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &descriptorSetLayout;
        VK_TRY(vkAllocateDescriptorSets(device, &allocInfo, &i.pictureDescriptor));

        VkDescriptorBufferInfo info;
        info.buffer = i.pictureBuffer.buffer;
        info.offset = 0;
        info.range = sizeof(Pixel) * uSize.x * uSize.y;

        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.pNext = nullptr;
        write.dstBinding = 0;
        write.dstSet = i.pictureDescriptor;
        write.descriptorCount = 1;
        write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        write.pBufferInfo = &info;

        vkUpdateDescriptorSets(device, 1, &write, 0, nullptr);
    }

    mainDeletionQueue.push([this]() {
        vkDestroyDescriptorPool(device, descriptorPool, nullptr);
        vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
        for (auto &i: frames) {
            vmaDestroyBuffer(allocator, i.pictureBuffer.buffer,
                             i.pictureBuffer.allocation);
        }
    });
}
