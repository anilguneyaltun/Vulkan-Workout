//
//  Ph_Renderer.hpp
//  VulkanFirst
//
//  Created by anilguneyaltun on 4.04.2022.
//

#ifndef Ph_Renderer_hpp
#define Ph_Renderer_hpp

#include "Ph_Device.hpp"
#include "Ph_Window.hpp"
#include "Ph_swapChain.hpp"

#include <cassert>
#include <memory>
#include <vector>

namespace Photonic {
class PhRenderer{
public:
    

    PhRenderer(PhWindow &window, PhDevice &device);
    ~PhRenderer();
    
    PhRenderer(const PhRenderer &) = delete;
    PhRenderer &operator=(const PhRenderer &) = delete;

    VkRenderPass getSwapChainRenderPass() const {return phSwapChain->getRenderPass();}
    float getAspectRatio() const { return phSwapChain->extentAspectRatio(); }
    bool isFrameInProgress() const {return isFrameStarted;}
    
    VkCommandBuffer getCurrentCommandBuffer() const {
        assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
        return commandBuffers[currentFrameIndex];
    }
    
    int getFrameIndex() const{
        assert(isFrameStarted && "Cannot get frame index when frame not in progress");
        return currentImageIndex;
    }
    
    
    VkCommandBuffer beginFrame();
    void endFrame();
    void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
    
private:

    void createCommandBuffers();
    void freeCommandBuffers();
    void recreateSwapChain();


    PhWindow &phWindow;
    PhDevice &phDevice;
    std::unique_ptr<PhSwapChain> phSwapChain;
    std::vector<VkCommandBuffer> commandBuffers;
    
    uint32_t currentImageIndex;
    int currentFrameIndex;
    bool isFrameStarted;
    
};
}

#endif /* Ph_Renderer_hpp */
