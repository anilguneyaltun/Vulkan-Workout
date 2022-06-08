//
//  Ph_Renderer.cpp
//  VulkanFirst
//
//  Created by anilguneyaltun on 4.04.2022.
//


#include "Ph_Renderer.hpp"
#include <stdexcept>
#include <array>
#include <iostream>

namespace Photonic{

PhRenderer::PhRenderer(PhWindow &window, PhDevice &device) : phWindow(window), phDevice(device){
    recreateSwapChain();
    createCommandBuffers();
}

PhRenderer::~PhRenderer(){
    freeCommandBuffers();
}

void PhRenderer::recreateSwapChain(){
    auto extent = phWindow.getExtent();
    while (extent.width == 0 || extent.height == 0) {
        extent = phWindow.getExtent();
        glfwWaitEvents();
    }
    vkDeviceWaitIdle(phDevice.device());
    
    if(phSwapChain == nullptr){
        phSwapChain = std::make_unique<PhSwapChain>(phDevice, extent);
    }else{
        std::shared_ptr<PhSwapChain> oldSwapChain = std::move(phSwapChain);
        phSwapChain = std::make_unique<PhSwapChain>(phDevice, extent, oldSwapChain);
        
        if (!oldSwapChain->compareSwapFormats(*phSwapChain.get())) {
            throw std::runtime_error("Swap Chain image format has changed!");
        }
    }
    
    
}

void PhRenderer::createCommandBuffers(){
    commandBuffers.resize(PhSwapChain::MAX_FRAMES_IN_FLIGHT);
    
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = phDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
    
    if (vkAllocateCommandBuffers(phDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers");
    }

}

void PhRenderer::freeCommandBuffers(){
    vkFreeCommandBuffers(phDevice.device(), phDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
    commandBuffers.clear();
}

VkCommandBuffer PhRenderer::beginFrame(){
    assert(!isFrameStarted && "Can't call beginFrame while already in progress");
    
    auto result = phSwapChain->acquireNextImage(&currentImageIndex);
    
    if(result == VK_ERROR_OUT_OF_DATE_KHR){
        recreateSwapChain();
        return nullptr;
    }
    
    if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
        throw std::runtime_error("failed to acquire swap chain image");
    }
    
    isFrameStarted = true;
    
    auto commandBuffer = getCurrentCommandBuffer();
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
    
    return commandBuffer;
}
void PhRenderer::endFrame(){
    assert(isFrameStarted && "Can't call endframe while frame is not in progress ");
    auto commandBuffer = getCurrentCommandBuffer();
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer");
    }
    
    auto result = phSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
    if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || phWindow.wasWindowResized()){
        phWindow.resetWindowResizedFlag();
        recreateSwapChain();
    }
    else if(result != VK_SUCCESS){
        throw std::runtime_error("failed to present swap chain image");
    }
    isFrameStarted = false;
    currentFrameIndex = (currentFrameIndex + 1) % PhSwapChain::MAX_FRAMES_IN_FLIGHT;
    
}
void PhRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer){
    assert(isFrameStarted && "Can't call beginSwapChainRenderPass while frame is not in progress ");
    assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame");
    
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType =  VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = phSwapChain->getRenderPass();
    renderPassInfo.framebuffer = phSwapChain->getFrameBuffer(currentImageIndex);
    
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = phSwapChain->getSwapChainExtent();
    
    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
    clearValues[1].depthStencil = {1, 0};
    
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();
    
    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(phSwapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(phSwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, phSwapChain->getSwapChainExtent()};
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}
void PhRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer){
    assert(isFrameStarted && "Can't call endSwapChainRenderPass while frame is not in progress ");
    assert(commandBuffer == getCurrentCommandBuffer() && "Can't end render pass on command buffer from a different frame");
    vkCmdEndRenderPass(commandBuffer);
}

}
