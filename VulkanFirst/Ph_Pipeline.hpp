//
//  Ph_Pipeline.hpp
//  VulkanFirst
//
//  Created by anilguneyaltun on 30.03.2022.
//

#ifndef Ph_Pipeline_hpp
#define Ph_Pipeline_hpp

#include "Ph_Device.hpp"

#include <string>
#include <vector>

namespace Photonic {

struct PipelineConfigInfo {
    PipelineConfigInfo(const PipelineConfigInfo&) = delete;
    PipelineConfigInfo &operator=(const PipelineConfigInfo&) = delete;
    
    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

class PhPipeline{
public:
    PhPipeline(PhDevice &device, const std::string &vertFilepath, const std::string &fragFilepath, const PipelineConfigInfo &configInfo);
    ~PhPipeline();
    
    PhPipeline(const PhPipeline&) = delete;
    PhPipeline &operator=(const PhPipeline&) = delete;
    
    void bind(VkCommandBuffer commandBuffer);
    static void defaultPiplineConfigInfo(PipelineConfigInfo &configInfo);
    
private:
    static std::vector<char> readFile(const std::string &filePath);
    
    void createGraphicsPipeline(const std::string &vertFilepath, const std::string &fragFilpath, const PipelineConfigInfo &configInfo);
    void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);
    
    
    PhDevice &phDevice;
    VkPipeline graphicsPipeline;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
    
    
};
}

#endif /* Ph_Pipeline_hpp */
