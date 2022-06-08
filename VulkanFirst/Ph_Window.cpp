//
//  Ph_Window.cpp
//  VulkanFirst
//
//  Created by anilguneyaltun on 30.03.2022.
//

#include "Ph_Window.hpp"
#include <stdexcept>

namespace Photonic{

PhWindow::PhWindow(int w, int h, std::string name) : width{w}, heigth{h}, windowName{name}{
    initWindow();
}

PhWindow::~PhWindow(){
    glfwDestroyWindow(window);
    glfwTerminate();
}

void PhWindow::initWindow(){
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //to not directly implement opengl
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    
    window = glfwCreateWindow(width, heigth, windowName.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void PhWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
{
    if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("creating window surface failed");
    }
}

void PhWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height){
    auto phWindow = reinterpret_cast<PhWindow *>(glfwGetWindowUserPointer(window));
    phWindow->framebufferResized = true;
    phWindow->width = width;
    phWindow->heigth = height;
}

}
