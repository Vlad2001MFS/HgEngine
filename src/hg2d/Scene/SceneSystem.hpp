#pragma once
#include "AECSSystem.hpp"
#include "GameObject.hpp"
#include "hd/System/hdWindowEvent.hpp"
#include <typeinfo>
#include <map>
#include <functional>

namespace hg2d {

class Engine;

class SceneSystem {
public:
    explicit SceneSystem(Engine &engine);
    ~SceneSystem();

    void onInitialize();
    void onShutdown();
    void onEvent(const hd::WindowEvent &event);
    void onFixedUpdate();
    void onUpdate();
    void onDraw();

    template<typename T>
    void registerSystem() {
        const std::type_info &typeInfo = typeid(T);
        if (mSystems.count(typeInfo.hash_code()) == 0) {
            AECSSystem *system = new T(mEngine);
            mSystems.insert(std::make_pair(typeInfo.hash_code(), system));
            system->onInitialize();
        }
        else {
            HD_LOG_WARNING("Failed to create system '%s'. The system with same type already registered at SceneSystem", typeInfo.name());
        }
    }

    template<typename T>
    void registerComponentType() {
        const std::type_info &typeInfo = typeid(T);
        auto it = mComponentTypes.find(typeInfo.hash_code());
        if (it == mComponentTypes.end()) {
            mComponentTypes.insert(std::make_pair(typeInfo.hash_code(), []() { return new T(); }));
        }
        else {
            HD_LOG_WARNING("Failed to register component type '%s'. The component type already registered at SceneSystem", typeInfo.name());
        }
    }
    AECSComponent *createComponent(uint64_t typeNameHash);

    void saveScene(const std::string &filename);
    void loadScene(const std::string &filename);

    GameObject *createObject(GameObject *parent);
    void destroyObject(GameObject *&object);

    const std::vector<GameObject*> &getObjects() const;
    GameObject *getRootObject();

private:
    Engine &mEngine;
    std::map<uint64_t, std::function<AECSComponent*()>> mComponentTypes;
    std::map<uint64_t, AECSSystem*> mSystems;
    std::vector<GameObject*> mObjects;
    GameObject *mRootObject;
};

}
