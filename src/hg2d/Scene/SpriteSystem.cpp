#include "SpriteSystem.hpp"
#include "TransformSystem.hpp"
#include "../Core/Engine.hpp"

namespace hg2d {

SpriteComponent::SpriteComponent() {
    mTexture = nullptr;
}

void SpriteComponent::setTexture(const Texture* texture) {
    mTexture = texture;
}

const Texture* SpriteComponent::getTexture() const {
    return mTexture;
}

SpriteSystem::SpriteSystem(Engine &engine) : AECSSystem(engine) {
}

void SpriteSystem::onDraw() {
    const std::vector<TransformComponent*> &transforms = mEngine.getSceneSystem().getComponents<TransformComponent>();
    const std::vector<SpriteComponent*> &sprites = mEngine.getSceneSystem().getComponents<SpriteComponent>();
    for (size_t i = 0; i < transforms.size(); i++) {
        TransformComponent *transform = transforms[i];
        SpriteComponent *sprite = sprites[i];
        if (transform && sprite && sprite->getTexture()) {
            RenderOp rop;
            rop.texture = sprite->getTexture();
            rop.pos = transform->getAbsolutePosition();
            rop.size = transform->getSize();
            rop.angle = transform->getAngle();
            mEngine.getRenderSystem().addRenderOp(rop);
        }
    }
}

}
