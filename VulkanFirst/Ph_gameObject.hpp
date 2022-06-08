//
//  Ph_gameObject.hpp
//  VulkanFirst
//
//  Created by anilguneyaltun on 2.04.2022.
//

#ifndef Ph_gameObject_hpp
#define Ph_gameObject_hpp

#include "Ph_Model.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace Photonic{
struct TransformComponent{
    glm::vec3 translation{};
    glm::vec3 scale{1.f, 1.f, 1.f};
    glm::vec3 rotation;
    
    glm::mat4 mat4(){
        auto transform = glm::translate(glm::mat4(1.f), translation);
        transform = glm::rotate(transform, rotation.y, {0.f, 1.f, 0.f});
        transform = glm::rotate(transform, rotation.x, {1.f, 0.f, 0.f});
        transform = glm::rotate(transform, rotation.z, {0.f, 0.f, 1.f});
        transform = glm::scale(transform, scale);
        return transform; //update the rotation mat
    }
};

class PhGameObject{
    
public:
    using id_t = unsigned int;
    
    static PhGameObject createGameObject(){
        static id_t currentId = 0;
        return PhGameObject{currentId++};
    }
    
    PhGameObject(const PhGameObject &) = delete;
    PhGameObject &operator=(const PhGameObject) = delete;
    PhGameObject (PhGameObject &&) = default;
    PhGameObject &operator=(PhGameObject&&) = default;
    
    id_t getId(){return id;}
    
    std::shared_ptr<PhModel> model{};
    glm::vec3 color{};
    TransformComponent transform{};
private:
    PhGameObject(id_t objId) : id{objId}{}
    
    id_t id;
};


}


#endif /* Ph_gameObject_hpp */
