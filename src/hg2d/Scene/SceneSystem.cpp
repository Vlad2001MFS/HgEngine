#include "SceneSystem.hpp"
#include "../Core/Engine.hpp"
#include "hd/IO/hdSerializerRW.hpp"

namespace hg2d {

SceneSystem::SceneSystem(Engine &engine) : mEngine(engine) {
}

SceneSystem::~SceneSystem() {
}

void SceneSystem::onInitialize() {
    mRootObject = new GameObject(mEngine);
}

void SceneSystem::onShutdown() {
    HD_DELETE(mRootObject);
    for (auto &system : mSystems) {
        system.second->onShutdown();
        HD_DELETE(system.second);
    }
}

void SceneSystem::onEvent(const hd::WindowEvent &event) {	
    for (auto &system : mSystems) {
        system.second->onEvent(mObjects, event);
    }
}

void SceneSystem::onFixedUpdate() {
    for (auto &system : mSystems) {
        system.second->onFixedUpdate(mObjects);
    }
}

void SceneSystem::onUpdate() {
    for (auto &system : mSystems) {
        system.second->onUpdate(mObjects);
    }
}

void SceneSystem::onDraw() {
    for (auto &system : mSystems) {
        system.second->onDraw(mObjects);
    }
}

AECSComponent *SceneSystem::createComponent(uint64_t typeNameHash) {
    auto it = mComponentTypes.find(typeNameHash);
    if (it != mComponentTypes.end()) {
        return it->second();
    }
    else {
        HD_LOG_ERROR("Failed to create component with typename hash '%s'. The component type not registered at SceneSystem", std::to_string(typeNameHash).data());
        return nullptr;
    }
}

void SceneSystem::saveScene(const std::string &filename) {
    hd::SerializerRW stream("data/levels/" + filename, true);
    mRootObject->_saveLoad(stream);
}

void SceneSystem::loadScene(const std::string &filename) {
    HD_DELETE(mRootObject);
    mRootObject = new GameObject(mEngine);
    hd::SerializerRW stream("data/levels/" + filename, false);
    mRootObject->_saveLoad(stream);
}

GameObject *SceneSystem::createObject(GameObject *parent) {
    GameObject *object = new GameObject(mEngine);
    object->setParent(parent ? parent : mRootObject);
    auto it = std::find(mObjects.begin(), mObjects.end(), nullptr);
    if (it != mObjects.end()) {
        *it = object;
    }
    else {
        mObjects.push_back(object);
    }
    return object;
}

const std::vector<GameObject*>& SceneSystem::getObjects() const {
    return mObjects;
}

GameObject* SceneSystem::getRootObject() {
    return mRootObject;
}

void SceneSystem::destroyObject(GameObject *&object) {
    if (!object) {
        HD_LOG_WARNING("object is nullptr");
    }
    else if (object == mRootObject) {
        HD_LOG_WARNING("The trying to destroy the root GameObject. The destroying the root GameObject is not impossible");
    }
    else {
        auto it = std::find(mObjects.begin(), mObjects.end(), object);
        if (it != mObjects.end()) {
            HD_DELETE(object);
            *it = nullptr;
        }
        else {
            HD_LOG_WARNING("Failed to destroy object. The Object wasn't created by SceneSystem");
        }
    }
}

}
