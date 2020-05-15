#pragma once
#include "Component.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>

namespace hg {

class GameObject {
public:
    virtual ~GameObject();

    template<typename T> T *createComponent();
    template<typename T> void destroyComponent();
    template<typename T> T *getComponent();

    GameObject *createChild(const std::string &name);
    void destroyChild(const std::string &name);

    Component *createComponent(const hd::StringHash &typeHash);
    void destroyComponent(const hd::StringHash &typeHash);

    void destroyAllChildren();
    void destroyAllComponents();

    void translate(float x, float y, float z);
    void translate(const glm::vec3 &offset);
    void scale(float x, float y);
    void scale(const glm::vec2 &size);
    void rotate(float angle);

    void setActive(bool active);
    void setPosition(float x, float y, float z);
    void setPosition(const glm::vec3 &pos);
    void setSize(float x, float y);
    void setSize(const glm::vec2 &size);
    void setAngle(float angle);

    GameObject *findChildByName(const std::string &name) const;

    Component *getComponent(const hd::StringHash &typeHash) const;
    GameObject *getParent() const;
    const std::vector<GameObject*> &getChildren() const;
    const std::unordered_map<hd::StringHash, GameObject*> &getChildrenByNames() const;
    const std::string &getName() const;
    bool isActive() const;
    const glm::vec3 &getPosition() const;
    const glm::vec2 &getSize() const;
    float getAngle() const;
    glm::vec3 getAbsolutePosition() const;

protected:
    void mOnSaveLoad(hd::JSON &data, bool isLoad);
    void mOnEvent(const WindowEvent &event);
    void mOnFixedUpdate();
    void mOnUpdate(float dt);

private:
    bool mAddComponent(Component *component);

    GameObject *mParent = nullptr;
    std::vector<GameObject*> mChildren;
    std::unordered_map<hd::StringHash, GameObject*> mChildrenByNames;
    std::vector<Component*> mComponents;
    std::string mName = "";
    bool mIsActive = true;
    glm::vec3 mPos = glm::vec3(0, 0, 0);
    glm::vec2 mSize = glm::vec2(0, 0);
    float mAngle = 0.0f;
};

template<typename T>
T *GameObject::createComponent() {
    T *component = new T();
    if (mAddComponent(component)) {
        return component;
    }
    else {
        return nullptr;
    }
}

template<typename T>
void GameObject::destroyComponent() {
    destroyComponent(T::getStaticTypeHash());
}

template<typename T>
T *GameObject::getComponent() {
    Component *c = getComponent(T::getStaticTypeHash());
    return c->as<T>();
}

}
