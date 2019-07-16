#include "SceneSystem.hpp"

namespace hg2d {

AECSSystem::AECSSystem(Engine &engine) : AEngineObject(engine) {
}

void AECSSystem::onInitialize() {
}

void AECSSystem::onShutdown() {
}

void AECSSystem::onEvent(const hd::WindowEvent &event) {
}

void AECSSystem::onCreateComponent(AECSComponent *component, uint64_t typeHash) {
}

void AECSSystem::onDestroyComponent(AECSComponent *component, uint64_t typeHash) {
}

void AECSSystem::onFixedUpdate() {
}

void AECSSystem::onUpdate() {
}

void AECSSystem::onDraw() {
}

SceneSystem::SceneSystem(Engine &engine) : AEngineObject(engine) {
}

void SceneSystem::onInitialize() { 
}

void SceneSystem::onShutdown() {
    for (auto &components : mComponentsMap) {
        for (auto &component : components.second) {
            mDestroyComponent(component, components.first);
        }
    }
    for (auto &it : mSystems) {
        mDestroySystem(it.second);
    }
}

void SceneSystem::onEvent(const hd::WindowEvent &event) {	
    for (auto &system : mSystems) {
        system.second->onEvent(event);
    }
}

void SceneSystem::onFixedUpdate() {
    for (auto &system : mSystems) {
        system.second->onFixedUpdate();
    }
}

void SceneSystem::onUpdate() {
    for (auto &system : mSystems) {
        system.second->onUpdate();
    }
}

void SceneSystem::onDraw() {
    for (auto &system : mSystems) {
        system.second->onDraw();
    }
}

HEntity SceneSystem::createEntity() {
    HEntity handle(mEntities.size());
    mEntities.push_back(handle);
    for (auto &components : mComponentsMap) {
        components.second.push_back(nullptr);
    }
    return handle;
}

void SceneSystem::destroyEntity(HEntity &handle) {
    mEntities.at(handle.value).invalidate();
    for (auto &components : mComponentsMap) {
        AECSComponent *&component = components.second.at(handle.value);
        mDestroyComponent(component, components.first);
    }
    handle.invalidate();
}

const std::vector<HEntity> &SceneSystem::getEntities() const {
    return mEntities;
}

const std::map<uint64_t, std::vector<AECSComponent*>> &SceneSystem::getComponentsMap() const {
    return mComponentsMap;
}

const std::map<uint64_t, AECSSystem*> &SceneSystem::getSystem() const {
   return mSystems;
}

void SceneSystem::mDestroyComponent(AECSComponent *&component, uint64_t typeHash) {
    if (component) {
        for (auto &system : mSystems) {
            system.second->onDestroyComponent(component, typeHash);
        }
        HD_DELETE(component);
    }
}

void SceneSystem::mDestroySystem(AECSSystem *&system) {
    system->onShutdown();
    HD_DELETE(system);
}

}
