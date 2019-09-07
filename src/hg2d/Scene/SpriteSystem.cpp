#include "SpriteSystem.hpp"
#include "TransformComponent.hpp"
#include "../Renderer/RenderSystem.hpp"
#include "../Cache/CacheSystem.hpp"
#include "hd/Core/hdLog.hpp"

namespace hg2d {

SpriteComponent::SpriteComponent(Engine &engine) : AECSComponent(engine) {
    mTexture = nullptr;
}

void SpriteComponent::onSaveLoad(JSONObject& json, bool isLoad) {
    JSONObject &jsonPath = json["texturePath"];
    if (isLoad) {
        if (!jsonPath.is_null()) {
            setTexture(mCacheSystem.loadTexture(jsonPath.get<std::string>()));
        }
    }
    else {
        if (getTexture()) {
            jsonPath = mRenderSystem.getTexturePath(getTexture());
        }
    }
}

void SpriteComponent::setTexture(const Texture* texture) {
    mTexture = texture;
}

const Texture* SpriteComponent::getTexture() const {
    return mTexture;
}

CameraComponent::CameraComponent(Engine& engine) : AECSComponent(engine) {
    mZoom = 0.0f;
}

void CameraComponent::onSaveLoad(JSONObject& json, bool isLoad) {
    JSONObject &jsonZoom = json["zoom"];
    if (isLoad) {
        jsonZoom.get_to(mZoom);
    }
    else {
        jsonZoom = mZoom;
    }
}

void CameraComponent::setZoom(float zoom) {
    mZoom = zoom;
}

float CameraComponent::getZoom() const {
    return mZoom;
}

SpriteSystem::SpriteSystem(Engine &engine) : AECSSystem(engine) {
    mCameraComponent = nullptr;
}

void SpriteSystem::onInitialize() {
    mSceneSystem.registerComponentType<SpriteComponent>();
    mSceneSystem.registerComponentType<CameraComponent>();
}

void SpriteSystem::onSaveLoad(JSONObject& json, bool isLoad) {
    JSONObject &jsonCameraEntity = json["cameraEntity"];
    if (isLoad) {
        setCameraEntity(HEntity(jsonCameraEntity.get<uint64_t>()));
    }
    else {
        jsonCameraEntity = getCameraEntity().value;
    }
}

void SpriteSystem::onClear() {
    mCameraEntity.invalidate();
    mTransformComponent = nullptr;
    mCameraComponent = nullptr;
}

void SpriteSystem::onDraw() {
    if (!mTransformComponent) {
        mTransformComponent = mSceneSystem.getComponent<TransformComponent>(mCameraEntity);
    }
    if (!mCameraComponent) {
        mCameraComponent = mSceneSystem.getComponent<CameraComponent>(mCameraEntity);
    }

    const std::vector<TransformComponent*> &transforms = mSceneSystem.getComponents<TransformComponent>();
    const std::vector<SpriteComponent*> &sprites = mSceneSystem.getComponents<SpriteComponent>();
    for (size_t i = 0; i < transforms.size(); i++) {
        TransformComponent *transform = transforms[i];
        SpriteComponent *sprite = sprites[i];
        if (transform && sprite && sprite->getTexture()) {
            RenderOp rop;
            if (mTransformComponent && mCameraComponent) {
                rop.camPos = glm::vec3(mTransformComponent->getPosition(), mCameraComponent->getZoom());
                rop.camAngle = mTransformComponent->getAngle();
            }
            rop.texture = sprite->getTexture();
            rop.pos = transform->getPosition();
            rop.size = transform->getSize();
            rop.angle = transform->getAngle();
            mRenderSystem.addRenderOp(rop);
        }
    }
}

void SpriteSystem::setCameraEntity(const HEntity& handle) {
    if (handle) {
        mCameraEntity = handle;
        mTransformComponent = mSceneSystem.getComponent<TransformComponent>(handle);
        mCameraComponent = mSceneSystem.getComponent<CameraComponent>(handle);
    }
    else {
        HD_LOG_WARNING("Failed to set invalid entity as camera");
    }
}

const HEntity& SpriteSystem::getCameraEntity() const {
    return mCameraEntity;
}

}
