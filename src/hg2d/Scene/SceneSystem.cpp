#include "SceneSystem.hpp"
#include "TransformComponent.hpp"
#include "hd/Core/hdStringUtils.hpp"
#include "hd/IO/hdFileStream.hpp"
#include "hd/Core/hdLog.hpp"
#include <string>

namespace hg2d {

AECSComponent::AECSComponent(Engine &engine) : AEngineObject(engine) {
}

void AECSComponent::onSaveLoad(JSONObject &json, bool isLoad) {
}

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

void AECSSystem::onSaveLoad(JSONObject &json, bool isLoad) {
}

void AECSSystem::onClear() {
}

void AECSSystem::onFixedUpdate() {
}

void AECSSystem::onUpdate() {
}

void AECSSystem::onDraw() {
}

SceneSystem::SceneSystem(Engine &engine) : AEngineObject(engine) {
    mIsUpdateEnabled = true;
    mIsDrawEnabled = true;
}

void SceneSystem::onInitialize() { 
    registerComponentType<TransformComponent>();
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
    if (mIsUpdateEnabled) {
        for (auto &system : mSystems) {
            system.second.second->onEvent(event);
        }
    }
}

void SceneSystem::onFixedUpdate() {
    if (mIsUpdateEnabled) {
        for (auto &system : mSystems) {
            system.second.second->onFixedUpdate();
        }
    }
}

void SceneSystem::onUpdate() {
    if (mIsUpdateEnabled) {
        for (auto &system : mSystems) {
            system.second.second->onUpdate();
        }
    }
}

void SceneSystem::onDraw() {
    if (mIsDrawEnabled) {
        for (auto &system : mSystems) {
            system.second.second->onDraw();
        }
    }
}

void SceneSystem::setUpdateEnabled(bool enabled) {
    mIsUpdateEnabled = enabled;
}

void SceneSystem::setDrawEnabled(bool enabled) {
    mIsDrawEnabled = enabled;
}

void SceneSystem::clear() {
    for (auto entity : mEntities) {
        destroyEntity(entity);
    }
    mEntities.clear();
    mComponentsMap.clear();
    for (auto &system : mSystems) {
        system.second.second->onClear();
    }
}

void SceneSystem::save(const std::string& name) {
    std::string path = "data/levels/" + name;
    JSONObject json;

    JSONObject &jsonSystems = json["systems"];
    for (auto &system : mSystems) {
        JSONObject &jsonSystem = jsonSystems[std::to_string(system.first)];
        jsonSystem["name"] = system.second.first;
        system.second.second->onSaveLoad(jsonSystem["data"], false);
    }

    JSONObject &jsonEntities = json["entities"];
    for (size_t i = 0; i < mEntities.size(); i++) {
        HEntity entity = mEntities.at(i);
        if (entity) {
            JSONObject &jsonEntity = jsonEntities[std::to_string(entity.value)];
            JSONObject &jsonComponents = jsonEntity["components"];
            for (auto &components : mComponentsMap) {
                AECSComponent *component = components.second.at(entity.value);
                if (component) {
                    JSONObject &jsonComponent = jsonComponents[std::to_string(components.first)];
                    jsonComponent["name"] = mComponentTypes.at(components.first).first;
                    component->onSaveLoad(jsonComponent["data"], false);
                }
            }
        }
    }

    hd::FileStream(path, hd::FileMode::Write).writeLine(json.dump(4));
}

void SceneSystem::load(const std::string& name) {
    clear();

    std::string path = "data/levels/" + name;
    JSONObject json = JSONObject::parse(hd::FileStream(path, hd::FileMode::Read).readAllText());

    JSONObject &jsonSystems = json["systems"];
    for (auto &jsonSystem : jsonSystems.items()) {
        uint64_t typeHash = hd::StringUtils::toUint64(jsonSystem.key());
        if (mSystems.count(typeHash) != 0) {
            AECSSystem *system = mSystems.at(typeHash).second;
            JSONObject &jsonData = jsonSystem.value()["data"];
            if (!jsonData.is_null()) {
                system->onSaveLoad(jsonData, true);
            }
        }
    }

    JSONObject &jsonEntities = json["entities"];
    for (auto &jsonEntity : jsonEntities.items()) {
        HEntity entity = mCreateEntity(hd::StringUtils::toUint64(jsonEntity.key()));
        JSONObject &jsonComponents = jsonEntity.value()["components"];
        for (auto &jsonComponent : jsonComponents.items()) {
            AECSComponent *component = createComponent(entity, hd::StringUtils::toUint64(jsonComponent.key()));
            JSONObject &jsonData = jsonComponent.value()["data"];
            if (!jsonData.is_null()) {
                component->onSaveLoad(jsonData, true);
            }
        }
    }
}

HEntity SceneSystem::createEntity() {
    return mCreateEntity(mEntities.size());
}

void SceneSystem::destroyEntity(HEntity &handle) {
    if (handle) {
        mEntities.at(handle.value).invalidate();
        for (auto &components : mComponentsMap) {
            AECSComponent *&component = components.second.at(handle.value);
            mDestroyComponent(component, components.first, handle);
        }
        handle.invalidate();
    }
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

HEntity SceneSystem::mCreateEntity(uint64_t idx) {
    while (idx > mEntities.size()) {
        mEntities.push_back(HEntity(mEntities.size()));
        for (auto &components : mComponentsMap) {
            components.second.push_back(nullptr);
        }
    }
    HEntity handle(idx);
    mEntities.push_back(handle);
    for (auto &components : mComponentsMap) {
        components.second.push_back(nullptr);
    }
    return handle;
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
