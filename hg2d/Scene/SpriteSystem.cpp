#include "SpriteSystem.hpp"
#include "TransformComponent.hpp"
#include "../Renderer/RenderSystem.hpp"
#include "../Cache/CacheSystem.hpp"
#include "hd/Core/Log.hpp"

namespace hg2d {

SpriteComponent::SpriteComponent() {
    mTexture = nullptr;
}

void SpriteComponent::onSaveLoad(JSONObject& json, bool isLoad) {
    JSONObject &jsonPath = json["texturePath"];
    if (isLoad) {
        if (!jsonPath.is_null()) {
            setTexture(getCacheSystem().loadTexture(jsonPath.get<std::string>()));
        }
    }
    else {
        if (getTexture()) {
            jsonPath = getRenderSystem().getTexturePath(getTexture());
        }
    }
}

void SpriteComponent::setTexture(const Texture* texture) {
    mTexture = texture;
}

const Texture* SpriteComponent::getTexture() const {
    return mTexture;
}

CameraComponent::CameraComponent() {
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

SpriteSystem::SpriteSystem() {
    mCameraComponent = nullptr;
}

void SpriteSystem::onInitialize() {
    getSceneSystem().registerComponentType<SpriteComponent>();
    getSceneSystem().registerComponentType<CameraComponent>();
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
        mTransformComponent = getSceneSystem().getComponent<TransformComponent>(mCameraEntity);
    }
    if (!mCameraComponent) {
        mCameraComponent = getSceneSystem().getComponent<CameraComponent>(mCameraEntity);
    }

    const std::vector<TransformComponent*> &transforms = getSceneSystem().getComponents<TransformComponent>();
    const std::vector<SpriteComponent*> &sprites = getSceneSystem().getComponents<SpriteComponent>();
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
            getRenderSystem().addRenderOp(rop, false);
        }
    }
}

void SpriteSystem::setCameraEntity(const HEntity& handle) {
    if (handle) {
        mCameraEntity = handle;
        mTransformComponent = getSceneSystem().getComponent<TransformComponent>(handle);
        mCameraComponent = getSceneSystem().getComponent<CameraComponent>(handle);
    }
    else {
        LOG_F(WARNING, "Failed to set invalid entity as camera");
    }
}

const HEntity& SpriteSystem::getCameraEntity() const {
    return mCameraEntity;
}

}
