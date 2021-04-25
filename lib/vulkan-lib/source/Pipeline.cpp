#include "vulak/Pipeline.hpp"
#include <fstream>
namespace vulak
{
VkShaderModule Pipeline::load_shader(VkDevice &device, const std::string &filepath)
{
    std::ifstream file(filepath, std::ios::ate | std::ios::binary);

    if (!file.is_open()) { throw std::runtime_error("failed to open file " + filepath); }
    auto fileSize = file.tellg();
    std::vector<uint32_t> buffer(fileSize);

    file.seekg(0);
    file.read((char *)buffer.data(), fileSize);
    file.close();

    VkShaderModule shader;
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.codeSize = buffer.size() * sizeof(uint32_t);
    createInfo.pCode = buffer.data();
    VK_TRY(vkCreateShaderModule(device, &createInfo, nullptr, &shader));
    return shader;
}

PipelineConfigInfo Pipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height)
{
    PipelineConfigInfo configInfo{};

    configInfo.inputAssemblyInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    configInfo.inputAssemblyInfo.pNext = nullptr;
    configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    configInfo.viewport.x = 0.0f;
    configInfo.viewport.y = 0.0f;
    configInfo.viewport.width = width;
    configInfo.viewport.height = height;
    configInfo.viewport.minDepth = 0.0f;
    configInfo.viewport.maxDepth = 1.0f;

    configInfo.scissor.offset = {0, 0};
    configInfo.scissor.extent = {width, height};
    return configInfo;
}

Pipeline::Pipeline(Device &d, const std::string &vertFilepath,
                   const std::string &fragFilepath, const PipelineConfigInfo &config)
    : device(d)
{
    auto vertShader = load_shader(device.getDevice(), vertFilepath);
    auto fragShader = load_shader(device.getDevice(), fragFilepath);
}

Pipeline::~Pipeline() {}
}    // namespace vulak