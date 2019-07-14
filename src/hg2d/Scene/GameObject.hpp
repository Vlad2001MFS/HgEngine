#pragma once
#include "AECSComponent.hpp"
#include "3rd/include/glm/glm.hpp"
#include <map>
#include <typeinfo>

namespace hg2d {

class Engine;

class GameObject {
public:
    GameObject(Engine &engine);
    ~GameObject();

    template<typename T>
    T *createComponent() {
        return static_cast<T*>(createComponent(typeid(T).hash_code()));
    }
    AECSComponent *createComponent(uint64_t typeNameHash);

    template<typename T>
    void destroyComponent() {
        std::type_info typeInfo = typeid(T);
        if (mComponents.count(typeInfo.hash_code()) != 0) {
            HD_DELETE(mComponents.at(typeInfo.hash_code()));
            mComponents.erase(mComponents.find(typeInfo.hash_code()));
        }
        else {
            HD_LOG_WARNING("Failed to destroy component '%s'. The component not registered at GameObject", typeInfo.name());
        }
    }

    template<typename T>
    T *getComponent() const {
        auto it = mComponents.find(typeid(T).hash_code());
        if (it != mComponents.end()) {
            return reinterpret_cast<T*>(it->second);
        }
        else {
            return nullptr;
        }
    }

    void setParent(GameObject *parent);

    void setPosition(float x, float y);
    void setPosition(const glm::vec2 &pos);
    void setSize(float x, float y);
    void setSize(const glm::vec2 &size);
    void setAngle(float angle);
    void translate(float x, float y);
    void translate(const glm::vec2 &pos);
    void scale(float x, float y);
    void scale(const glm::vec2 &size);
    void rotate(float angle);

    glm::vec2 getAbsolutePosition() const;
    const glm::vec2 &getPosition() const;
    const glm::vec2 &getSize() const;
    float getAngle() const;
    const std::vector<GameObject*> &getChildren() const;
    GameObject *getParent() const;

    void _saveLoad(hd::SerializerRW &stream);

private:
    struct TransformComponent : public hg2d::AECSComponent {
        TransformComponent();

        void onSaveLoad(hd::SerializerRW &stream);

        glm::vec2 pos, size;
        float angle;
    };

    Engine &mEngine;
    TransformComponent mTransform;
    std::map<uint64_t, AECSComponent*> mComponents;
    std::vector<GameObject*> mChildren;
    GameObject *mParent;
};

}
