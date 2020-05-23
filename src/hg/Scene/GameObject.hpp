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
    template<typename T> T *findComponent();

    GameObject *createChild(const std::string &name);
    GameObject *createChildFromFile(const std::string &name, const std::string &path);
    void saveToFile(const std::string &path);
    void destroyChild(const std::string &name);

    Component *createComponent(const hd::StringHash &typeHash);
    void destroyComponent(const hd::StringHash &typeHash);

    void destroyAllChildren();
    void destroyAllComponents();

    void move(float x, float y);
    void move(const glm::vec2 &offset);
    void translate(float x, float y);
    void translate(const glm::vec2 &offset);
    void scale(float x, float y);
    void scale(const glm::vec2 &size);
    void rotate(float angle);

    void setActive(bool active);
    void setPosition(float x, float y);
    void setPosition(const glm::vec2 &pos);
    void setWorldPosition(const glm::vec2 &pos);
    void setSize(float x, float y);
    void setSize(const glm::vec2 &size);
    void setAngle(float angle);
    void setWorldAngle(float angle);

    GameObject *findChildByName(const std::string &name) const;
    Component *findComponent(const hd::StringHash &typeHash) const;

    GameObject *getParent() const;
    const std::vector<GameObject*> &getChildren() const;
    const std::unordered_map<hd::StringHash, GameObject*> &getChildrenByNames() const;
    const std::string &getName() const;
    bool isActive() const;
    const glm::vec2 &getPosition() const;
    const glm::vec2 &getWorldPosition() const;
    const glm::vec2 &getSize() const;
    float getAngle() const;
    float getWorldAngle() const;

protected:
    void mOnSaveLoad(hd::JSON &data, bool isLoad);
    void mOnEvent(const WindowEvent &event);
    void mOnFixedUpdate();
    void mOnUpdate(float dt);

private:
    static std::string mGetFullPath(const std::string &path);

    bool mAddComponent(Component *component);
    void mUpdateTransform(bool isPosUpdate, bool isSizeUpdate, bool isAngleUpdate);

    GameObject *mParent = nullptr;
    std::vector<GameObject*> mChildren;
    std::unordered_map<hd::StringHash, GameObject*> mChildrenByNames;
    std::vector<Component*> mComponents;
    std::string mName = "";
    bool mIsActive = true;
    glm::vec2 mPos = glm::vec2(0, 0);
    glm::vec2 mSize = glm::vec2(0, 0);
    float mAngle = 0.0f;
    glm::vec2 mWorldPos = glm::vec2(0, 0);
    float mWorldAngle = 0.0f;
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
    destroyComponent(T::getTypeHashStatic());
}

template<typename T>
T *GameObject::findComponent() {
    Component *component = findComponent(T::getTypeHashStatic());
    return component ? component->as<T>() : nullptr;
}

}
