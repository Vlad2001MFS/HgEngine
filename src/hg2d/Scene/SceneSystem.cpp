#include "SceneSystem.hpp"
#include <string>

namespace hg2d {

AECSSystem::AECSSystem(Engine &engine) : AEngineObject(engine) {
}

void AECSSystem::onInitialize() {
}

void AECSSystem::onShutdown() {
}

void AECSSystem::onEvent(const hd::WindowEvent &event) {
}

void AECSSystem::onCreateComponent(AECSComponent *component, uint64_t typeHash, const HEntity &entity) {
}

void AECSSystem::onDestroyComponent(AECSComponent *component, uint64_t typeHash, const HEntity &entity) {
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
        for (size_t i = 0; i < mEntities.size(); i++) {
            mDestroyComponent(components.second.at(i), components.first, mEntities.at(i));
        }
    }
    for (auto &it : mSystems) {
        mDestroySystem(it.second.second);
    }
}

void SceneSystem::onEvent(const hd::WindowEvent &event) {	
    for (auto &system : mSystems) {
        system.second.second->onEvent(event);
    }
}

void SceneSystem::onFixedUpdate() {
    for (auto &system : mSystems) {
        system.second.second->onFixedUpdate();
    }
}

void SceneSystem::onUpdate() {
    for (auto &system : mSystems) {
        system.second.second->onUpdate();
    }
}

void SceneSystem::onDraw() {
    for (auto &system : mSystems) {
        system.second.second->onDraw();
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
        mDestroyComponent(component, components.first, handle);
    }
    handle.invalidate();
}

AECSComponent* SceneSystem::createComponent(HEntity& handle, uint64_t typeHash) {
    return mCreateComponent(handle, typeHash, std::to_string(typeHash));
}

const std::vector<HEntity> &SceneSystem::getEntities() const {
    return mEntities;
}

const std::map<uint64_t, std::vector<AECSComponent*>> &SceneSystem::getComponentsMap() const {
    return mComponentsMap;
}

const std::map<uint64_t, std::pair<std::string, AECSSystem*>> &SceneSystem::getSystems() const {
   return mSystems;
}

AECSComponent* SceneSystem::mCreateComponent(HEntity& handle, uint64_t typeHash, const std::string& typeName) {
    auto factory = mComponentTypes.find(typeHash);
    if (factory == mComponentTypes.end()) {
        HD_LOG_ERROR("Failed to create component of not registered type '%s'", typeName.data());
        return nullptr;
    }
    if (handle && handle.value < mEntities.size()) {
        if (mComponentsMap.count(typeHash) == 0) {
            mComponentsMap.insert(std::make_pair(typeHash, std::vector<AECSComponent*>(mEntities.size(), nullptr)));
        }
        AECSComponent *&component = mComponentsMap.at(typeHash).at(handle.value);
        if (!component) {
            component = factory->second.second();
            for (auto &system : mSystems) {
                system.second.second->onCreateComponent(component, typeHash, handle);
            }
        }
        return component;
    }
    else {
        HD_LOG_ERROR("Failed to create component '%s' for entity '%ll'", factory->second.first.data(), handle.value);
        return nullptr;
    }
}

void SceneSystem::mDestroyComponent(AECSComponent *&component, uint64_t typeHash, const HEntity &entity) {
    if (component) {
        for (auto &system : mSystems) {
            system.second.second->onDestroyComponent(component, typeHash, entity);
        }
        HD_DELETE(component);
    }
}

void SceneSystem::mDestroySystem(AECSSystem *&system) {
    system->onShutdown();
    HD_DELETE(system);
}

}
