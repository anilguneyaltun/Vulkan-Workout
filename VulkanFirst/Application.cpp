//
//  Application.cpp
//  VulkanFirst
//
//  Created by anilguneyaltun on 30.03.2022.
//
#include "Application.hpp"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


#include "InputController.hpp"
#include "simple_render_system.hpp"
#include "Ph_Camera.hpp"

#include <chrono>
#include <stdexcept>
#include <array>
#include <iostream>

namespace Photonic{

Application::Application(){
    loadGameObjects();
}

// temporary helper function, creates a 1x1x1 cube centered at offset
std::unique_ptr<PhModel> createCubeModel(PhDevice& device, glm::vec3 offset) {
  std::vector<PhModel::Vertex> vertices{

      // left face (white)
      {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

      // right face (yellow)
      {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

      // top face (orange, remember y axis points down)
      {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

      // bottom face (red)
      {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

      // nose face (blue)
      {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

      // tail face (green)
      {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

  };
  for (auto& v : vertices) {
    v.position += offset;
  }
  return std::make_unique<PhModel>(device, vertices);
}

void Application::loadGameObjects(){
    std::shared_ptr<PhModel> phModel = createCubeModel(phDevice, {.0f, .0f, .0f});
    
    auto cube = PhGameObject::createGameObject();
    cube.model = phModel;
    cube.transform.translation = {.0f, .0f, 2.5f};
    cube.transform.scale = {.5f, .5f, 0.5f};

    gameObjects.push_back(std::move(cube));
}

void Application::run(){
    SimpleRenderSystem simpleRenderSystem{phDevice, phRenderer.getSwapChainRenderPass()};
    PhCamera camera{};
    
    auto currentTime = std::chrono::high_resolution_clock::now();
    
    auto camObject = PhGameObject::createGameObject();
    InputController cameraController{};
    
    
    while(!phWindow.shouldClose()){
        glfwPollEvents();
        
        auto newTime = std::chrono::high_resolution_clock::now();
        float frame = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;
            
        cameraController.moveInPlaneXZ(phWindow.getGLFWWindow(), frame, camObject);
        camera.setViewYXZ(camObject.transform.translation, camObject.transform.rotation);
        
        float aspect = phRenderer.getAspectRatio();
        camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 100.f);
        
        if(auto commandBuffer = phRenderer.beginFrame()){
            phRenderer.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
            phRenderer.endSwapChainRenderPass(commandBuffer);
            phRenderer.endFrame();
        }
    }
    vkDeviceWaitIdle(phDevice.device());
}

Application::~Application(){}

}
