#pragma once
#include "Component.hpp"
#include <vector>
#include <unordered_map>

namespace hg {

class Node : public Object {
    HG_OBJECT(Node, Object);
public:
    Node();
    ~Node();

    virtual void onSaveLoad(JSONObject &data, bool isLoad);
    virtual void onEvent(const SDL_Event &event);
    virtual void onFixedUpdate();
    virtual void onUpdate(float dt);
    virtual void onDraw();

    Node *findByName(const std::string &name);

    template<typename T>
    T *createChild(const std::string &name) {
        T *node = new T();
        mAddChild(node, name);
        return node;
    }

    template<typename T = Node>
    T *createChild(const std::string &name, const std::string &typeName) {
        T *node = Factory::get().createObject(typeName)->as<T>();
        mAddChild(node, name);
        return node;
    }

    template<typename T = Node>
    T *createChild(const std::string &name, const hd::StringHash &typeHash) {
        T *node = Factory::get().createObject(typeHash)->as<T>();
        mAddChild(node, name);
        return node;
    }

    template<typename T>
    T *createComponent() {
        T *component = new T();
        mAddComponent(component);
        return component;
    }

    template<typename T = Component>
    T *createComponent(const std::string &typeName) {
        T *component = Factory::get().createObject(typeName)->as<T>();
        mAddComponent(component);
        return component;
    }

    template<typename T = Component>
    T *createComponent(const hd::StringHash &typeHash) {
        T *component = Factory::get().createObject(typeHash)->as<T>();
        mAddComponent(component);
        return component;
    }

    template<typename T>
    T *getComponent() {
        return getComponent<T>(T::getStaticTypeInfo()->getTypeHash());
    }

    template<typename T = Component>
    T *getComponent(const std::string &typeName) {
        return getComponent<T>(hd::StringHash(typeName));
    }

    template<typename T = Component>
    T *getComponent(const hd::StringHash &typeHash) {
        auto it = mComponentsByType.find(typeHash);
        if (it != mComponentsByType.end()) {
            return it->second->as<T>();
        }
        else {
            HD_LOG_WARNING("Failed to get component '{}'", typeHash.getString());
            return nullptr;
        }
    }

    template<typename T>
    void destroyComponent() {
        auto it = std::find_if(mComponents.begin(), mComponents.end(), [](Component *comp) { return comp->isInstanceOf<T>(); });
        if (it != mComponents.end()) {
            delete *it;
            mComponents.erase(it);
            mComponentsByType.erase(T::getStaticTypeInfo()->getTypeHash());
        }
        else {
            HD_LOG_WARNING("Failed to destroy component '{}'", T::getTypeInfoStatic()->getTypeName());
        }
    }

    void destroyChild(const std::string &name);

    void setActive(bool active);

    Node *getParent() const;
    const std::vector<Node*> &getChildren() const;
    const std::unordered_map<hd::StringHash, Node*> &getChildrenByNames() const;
    const std::string &getName() const;
    bool isActive() const;

private:
    void mAddChild(Node *node, const std::string &name);
    void mAddComponent(Component *component);

    Node *mParent;
    std::vector<Node*> mChildren;
    std::unordered_map<hd::StringHash, Node*> mChildrenByNames;
    std::vector<Component*> mComponents;
    std::unordered_map<hd::StringHash, Component*> mComponentsByType;
    std::string mName;
    bool mIsActive;
};

}
