//
//  Ph_Camera.hpp
//  VulkanFirst
//
//  Created by anilguneyaltun on 14.06.2022.
//

#ifndef Ph_Camera_hpp
#define Ph_Camera_hpp

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>



namespace Photonic{

class PhCamera{
public:
    void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
    void setPerspectiveProjection(float fovy, float aspect, float near, float far);
    
    const glm::mat4 getProjection() const { return projectionMatrix; }
    
private:
    glm::mat4 projectionMatrix{1.0f};
    

    };
}
#endif /* Ph_Camera_hpp */
