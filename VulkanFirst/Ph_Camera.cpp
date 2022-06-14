//
//  Ph_Camera.cpp
//  VulkanFirst
//
//  Created by anilguneyaltun on 14.06.2022.
//

#include "Ph_Camera.hpp"
#include <cassert>
#include <limits>

using namespace Photonic;

void PhCamera::setOrthographicProjection(float left, float right, float top, float bottom, float near, float far) { 
    projectionMatrix = glm::mat4{1.f};
    projectionMatrix[0][0] = 2.f/(right - left);
    projectionMatrix[1][1] = 2.f/(bottom - top);
    projectionMatrix[2][2] = 1.f/(far - near); //differs on vulkan!
    projectionMatrix[3][0] = -(right + left) / (right - left);
    projectionMatrix[3][1] = -(top + bottom) / (top - bottom);
    projectionMatrix[3][2] = -near / (far - near);
}

void PhCamera::setPerspectiveProjection(float fovy, float aspect, float near, float far) {
    
    const float tanHalfFovy = tan(fovy/2.f);
    
    projectionMatrix = glm::mat4{0.f};
    projectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
    projectionMatrix[1][1] = 1.f / tanHalfFovy;
    projectionMatrix[2][2] = far / (far - near);
    projectionMatrix[2][3] = 1.f;
    projectionMatrix[3][2] = -(far * near) / (far - near);
    
}

