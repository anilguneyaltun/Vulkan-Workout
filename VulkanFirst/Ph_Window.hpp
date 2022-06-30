//
//  Ph_Window.hpp
//  VulkanFirst
//
//  Created by anilguneyaltun on 30.03.2022.
//

#ifndef Ph_Window_hpp
#define Ph_Window_hpp
#define GLFW_INCLUDE_VULKAN

#include <stdio.h>
#include <GLFW/glfw3.h>

#include <string>
namespace Photonic{

class PhWindow{
    
public:
    PhWindow(int w, int h, std::string name);
    
    bool shouldClose(){return glfwWindowShouldClose(window);}
    
    VkExtent2D getExtent(){return {static_cast<uint32_t>(width), static_cast<uint32_t>(heigth)};}
    
    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
    bool wasWindowResized(){return framebufferResized;}
    void resetWindowResizedFlag(){framebufferResized = false;}
    
    GLFWwindow *getGLFWWindow() const {return window;}
    
    
    ~PhWindow();
    
    PhWindow(const PhWindow &) = delete;
    PhWindow &operator=(const PhWindow &) = delete;

    
private:
    
    static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
    void initWindow();
    
    bool framebufferResized = false;
    int width;
    int heigth;
    
    std::string windowName;
    GLFWwindow *window;
    
};

}
#endif /* Ph_Window_hpp */
