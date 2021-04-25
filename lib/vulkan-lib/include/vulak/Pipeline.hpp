#pragma once

#include <string>
#include <vector>

#include "vulak/Device.hpp"

namespace vulak
{

struct PipelineConfigInfo {
    VkViewport viewport;
    VkRect2D scissor;
    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

class Pipeline
{
public:
    static VkShaderModule load_shader(VkDevice &, const std::string &);
    static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t, uint32_t);

public:
    Pipeline(Device &, const std::string &, const std::string &,
             const PipelineConfigInfo &);
    Pipeline(Pipeline &) = delete;
    Pipeline(const Pipeline &) = delete;
    void operator=(const Pipeline &) = delete;
    ~Pipeline();

private:
    Device &device;
    VkPipeline pipeline;
};
}    // namespace vulak