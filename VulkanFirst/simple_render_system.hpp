#ifndef simple_render_system_hpp
#define simple_render_system_hpp

#include "Ph_Device.hpp"
#include "Ph_Pipeline.hpp"
#include "Ph_gameObject.hpp"

#include <memory>
#include <vector>

namespace Photonic {
class SimpleRenderSystem{
public:

    SimpleRenderSystem(PhDevice &phDevice, VkRenderPass renderPass);
    ~SimpleRenderSystem();
    
    SimpleRenderSystem(const SimpleRenderSystem &) = delete;
    SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;
    
    void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<PhGameObject> &gameObjects);
    
private:

    void createPipelineLayout();
    void createPipeline(VkRenderPass renderPass);
    
    PhDevice &phDevice;
    std::unique_ptr<PhPipeline> phPipeline;
    VkPipelineLayout pipelineLayout;
    
};
}

#endif /* simple_render_system_hpp */
