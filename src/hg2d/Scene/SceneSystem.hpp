#pragma once
#include "hd/Core/hdHandle.hpp"
#include "hd/Core/hdCommon.hpp"
#include "hd/System/hdWindowEvent.hpp"
#include <map>
#include <vector>
#include <typeinfo>

namespace hg2d {

class Engine;

using HEntity = hd::Handle<uint64_t, struct TAG_HEntity, UINT64_MAX>;

class AECSComponent {
public:
    AECSComponent() = default;
    virtual ~AECSComponent() = default;
};

class AECSSystem {
public:
    AECSSystem(Engine &engine);
    virtual ~AECSSystem();

    virtual void onInitialize();
    virtual void onShutdown();
    virtual void onEvent(const hd::WindowEvent &event);
    virtual void onFixedUpdate();
    virtual void onUpdate();
    virtual void onDraw();

protected:
    Engine &mEngine;
};

class SceneSystem {
public:
    explicit SceneSystem(Engine &engine);
    ~SceneSystem() = default;

    void onInitialize();
    void onShutdown();
    void onEvent(const hd::WindowEvent &event);
    void onFixedUpdate();
    void onUpdate();
    void onDraw();

    HEntity createEntity();
    void destroyEntity(HEntity &handle);

    template<typename T>
    T *createComponent(HEntity &handle) {
        const std::type_info &typeInfo = typeid(T);
        const char *typeName = typeInfo.name();
        uint64_t typeHash = typeInfo.hash_code();
        if (handle && handle.value < mEntities.size()) {
            if (mComponentsMap.count(typeHash) == 0) {
                mComponentsMap.insert(std::make_pair(typeHash, std::vector<AECSComponent*>(mEntities.size(), nullptr)));
            }
            AECSComponent *&comp = mComponentsMap.at(typeHash).at(handle.value);
            if (!comp) {
                comp = new T();
            }
            return static_cast<T*>(comp);
        }
        else {
            HD_LOG_ERROR("Failed to create component '%s' for entity '%ll'", typeName, handle.value);
            return nullptr;
        }
    }

    template<typename T>
    void destroyComponent(HEntity &handle) {
        const std::type_info &typeInfo = typeid(T);
        const char *typeName = typeInfo.name();
        uint64_t typeHash = typeInfo.hash_code();
        if (handle && handle.value < mEntities.size() && mComponentsMap.count(typeHash) != 0) {
            AECSComponent *&component = mComponentsMap.at(typeHash).at(handle.value);
            HD_DELETE(component);
        }
    }

    template<typename T>
    T *getComponent(const HEntity &handle) {
        const std::type_info &typeInfo = typeid(T);
        const char *typeName = typeInfo.name();
        uint64_t typeHash = typeInfo.hash_code();
        if (handle && handle.value < mEntities.size() && mComponentsMap.count(typeHash) != 0) {
            return static_cast<T*>(mComponentsMap.at(typeHash).at(handle.value));
        }
        else {
            return nullptr;
        }
    }

    template<typename T>
    const std::vector<T*> &getComponents() {
        const std::type_info &typeInfo = typeid(T);
        const char *typeName = typeInfo.name();
        uint64_t typeHash = typeInfo.hash_code();
        if (mComponentsMap.count(typeHash) == 0) {
            mComponentsMap.insert(std::make_pair(typeHash, std::vector<AECSComponent*>(mEntities.size(), nullptr)));
        }
        return reinterpret_cast<const std::vector<T*>&>(mComponentsMap.at(typeHash));
    }

    template<typename T>
    T *createSystem() {
        const std::type_info &typeInfo = typeid(T);
        const char *typeName = typeInfo.name();
        uint64_t typeHash = typeInfo.hash_code();
        if (mSystems.count(typeHash) == 0) {
            T *system = new T(mEngine);
            mSystems.insert(std::make_pair(typeHash, system));
            system->onInitialize();
            return system;
        }
        else {
            return static_cast<T*>(mSystems.at(typeHash));
        }
    }

    template<typename T>
    void destroySystem() {
        const std::type_info &typeInfo = typeid(T);
        const char *typeName = typeInfo.name();
        uint64_t typeHash = typeInfo.hash_code();
        if (mSystems.count(typeHash) != 0) {
            mDestroySystem(mSystems.at(typeHash));
            mSystems.erase(typeHash);
        }
    }

    template<typename T>
    T *getSystem() {
        const std::type_info &typeInfo = typeid(T);
        const char *typeName = typeInfo.name();
        uint64_t typeHash = typeInfo.hash_code();
        if (mSystems.count(typeHash) != 0) {
            return static_cast<T*>(mSystems.at(typeHash));
        }
        else {
            return nullptr;
        }
    }

    const std::vector<HEntity> &getEntities() const;
    const std::map<uint64_t, std::vector<AECSComponent*>> &getComponentsMap() const;
    const std::map<uint64_t, AECSSystem*> &getSystem() const;


private:
    void mDestroySystem(AECSSystem *&system);

    Engine &mEngine;
    std::vector<HEntity> mEntities;
    std::map<uint64_t, std::vector<AECSComponent*>> mComponentsMap;
    std::map<uint64_t, AECSSystem*> mSystems;
};

}
