//
//  Ph_Model.hpp
//  VulkanFirst
//
//  Created by anilguneyaltun on 31.03.2022.
//

#ifndef Ph_Model_hpp
#define Ph_Model_hpp

#include <stdio.h>
#include <vector>
#include "Ph_Device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace Photonic{
    class PhModel{
    public:
        struct Vertex{
            glm::vec3 position;
            glm::vec3 color;
            
            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };
        
        PhModel(PhDevice &device, const std::vector<Vertex> &vertices);
        ~PhModel();
        
        PhModel(const PhModel &) = delete;
        PhModel &operator=(const PhModel &) = delete;
    
        void bind(VkCommandBuffer commandbuffer);
        void draw(VkCommandBuffer commandbuffer);
        
    private:
        void createVertexBuffers(const std::vector<Vertex> &vertices);
        
        PhDevice &phDevice;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;
    };

}

#endif /* Ph_Model_hpp */
