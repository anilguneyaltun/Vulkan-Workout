//
//  InputKeyboardController.hpp
//  VulkanFirst
//
//  Created by anilguneyaltun on 26.06.2022.
//

#ifndef InputController_hpp
#define InputController_hpp

#include "Ph_gameObject.hpp"
#include "Ph_Window.hpp"



namespace Photonic{
    class InputController{
    
    public:
        struct KeyMappings{
            int moveLeft = GLFW_KEY_A;
            int moveRight = GLFW_KEY_D;
            int moveForward = GLFW_KEY_W;
            int moveBackward = GLFW_KEY_S;
            int moveUp = GLFW_KEY_Q;
            int moveDown = GLFW_KEY_E;
            int lookLeft = GLFW_KEY_LEFT;
            int lookRight = GLFW_KEY_RIGHT;
            int lookUp = GLFW_KEY_UP;
            int lookDown = GLFW_KEY_DOWN;
        };
        
        void moveInPlaneXZ(GLFWwindow *window, float dt, PhGameObject &gameObject);
        
        KeyMappings keys{};
        float moveSpeed{2.5f};
        float lookSpeed{2.0f};
        GLfloat xChange, yChange;
    };

}

#endif /* InputController_hpp */
