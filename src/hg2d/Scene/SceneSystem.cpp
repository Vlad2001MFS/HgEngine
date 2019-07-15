#include "SceneSystem.hpp"
#include "../Core/Engine.hpp"
#include "hd/IO/hdSerializerRW.hpp"

namespace hg2d {

AECSSystem::AECSSystem(Engine &engine) : mEngine(engine) {
}

AECSSystem::~AECSSystem() {
}

void AECSSystem::onInitialize() {
}

void AECSSystem::onShutdown() {
}

void AECSSystem::onEvent(const hd::WindowEvent &event) {
}

void AECSSystem::onFixedUpdate() {
}

void AECSSystem::onUpdate() {
}

void AECSSystem::onDraw() {
}

SceneSystem::SceneSystem(Engine &engine) : mEngine(engine) {
}

void SceneSystem::onInitialize() { 
}

void SceneSystem::onShutdown() {
    for (auto &it : mSystems) {
        mDestroySystem(it.second);
    }
    for (auto &components : mComponentsMap) {
        for (auto &component : components.second) {
            HD_DELETE(component);
        }
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
        HD_DELETE(component);
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

void SceneSystem::mDestroySystem(AECSSystem *&system) {
    system->onShutdown();
    HD_DELETE(system);
}

}
