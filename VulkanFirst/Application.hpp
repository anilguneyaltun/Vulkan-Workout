//
//  Application.hpp
//  VulkanFirst
//
//  Created by anilguneyaltun on 30.03.2022.
//

#ifndef Application_hpp
#define Application_hpp

#include "Ph_Device.hpp"
#include "Ph_Window.hpp"
#include "Ph_Renderer.hpp"
#include "Ph_gameObject.hpp"

#include <memory>
#include <vector>

namespace Photonic {
class Application{
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    Application();
    ~Application();
    
    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;
    void run();
    
private:
    void loadGameObjects();

    PhWindow phWindow{WIDTH, HEIGHT, "VULKANIZED"};
    PhDevice phDevice{phWindow};
    PhRenderer phRenderer{phWindow, phDevice};
    std::vector<PhGameObject> gameObjects;
    
    
};
}

#endif /* Application_hpp */
