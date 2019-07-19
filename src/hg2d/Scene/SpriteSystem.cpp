#include "SpriteSystem.hpp"
#include "TransformComponent.hpp"
#include "../Renderer/RenderSystem.hpp"
#include "../Cache/CacheSystem.hpp"

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
        std::string path = mRenderSystem.getTexturePath(getTexture());
        if (!path.empty()) {
            jsonPath = path;
        }
    }
}

void SpriteComponent::setTexture(const Texture* texture) {
    mTexture = texture;
}

const Texture* SpriteComponent::getTexture() const {
    return mTexture;
}

SpriteSystem::SpriteSystem(Engine &engine) : AECSSystem(engine) {
}

void SpriteSystem::onInitialize() {
    mSceneSystem.registerComponentType<SpriteComponent>();
}

void SpriteSystem::onDraw() {
    const std::vector<TransformComponent*> &transforms = mSceneSystem.getComponents<TransformComponent>();
    const std::vector<SpriteComponent*> &sprites = mSceneSystem.getComponents<SpriteComponent>();
    for (size_t i = 0; i < transforms.size(); i++) {
        TransformComponent *transform = transforms[i];
        SpriteComponent *sprite = sprites[i];
        if (transform && sprite && sprite->getTexture()) {
            RenderOp rop;
            rop.texture = sprite->getTexture();
            rop.pos = transform->getPosition();
            rop.size = transform->getSize();
            rop.angle = transform->getAngle();
            mRenderSystem.addRenderOp(rop);
        }
    }
}

}
