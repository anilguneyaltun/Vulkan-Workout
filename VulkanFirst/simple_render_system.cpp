//
//  simple_render_system.cpp
//  VulkanFirst
//
//  Created by anilguneyaltun on 4.06.2022.
//

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "simple_render_system.hpp"
#include <stdexcept>
#include <array>
#include <iostream>

namespace Photonic{

struct SimplePushConstantData{
    glm::mat4 transform{1.f}; //identity matrix
    alignas(16) glm::vec3 color;
};

SimpleRenderSystem::SimpleRenderSystem(PhDevice &device, VkRenderPass renderPass) : phDevice(device){
    createPipelineLayout();
    createPipeline(renderPass);
}


SimpleRenderSystem::~SimpleRenderSystem(){
    vkDestroyPipelineLayout(phDevice.device(), pipelineLayout, nullptr);
}

void SimpleRenderSystem::createPipelineLayout(){
    
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);
    
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if (vkCreatePipelineLayout(phDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout");
    }
}


void SimpleRenderSystem::createPipeline(VkRenderPass renderPass){
    assert(pipelineLayout != nullptr && "cannot create pipeline before pipeline layout");
    
    PipelineConfigInfo pipelineConfig{};
    PhPipeline::defaultPiplineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout =  pipelineLayout;
    phPipeline = std::make_unique<PhPipeline>(phDevice, "../shaders/vert.spv", "../shaders/frag.spv", pipelineConfig);
}


void SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<PhGameObject> &gameObjects, const PhCamera &camera){
    phPipeline->bind(commandBuffer);
    
    for (auto &obj : gameObjects) {
        
        obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + 0.01f, glm::two_pi<float>());
        obj.transform.rotation.x = glm::mod(obj.transform.rotation.x + 0.005f, glm::two_pi<float>());
        SimplePushConstantData push{};
        push.color = obj.color;
        push.transform = camera.getProjection() * obj.transform.mat4();
        
        vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
        obj.model->bind(commandBuffer);
        obj.model->draw(commandBuffer);
       
    }
   }
}
