#include "SpriteSystem.hpp"
#include "GameObject.hpp"
#include "../Core/Engine.hpp"

namespace hg2d {

void SpriteComponent::onSaveLoad(hd::SerializerRW &stream) {
    stream.processString(this->texturePath);
}

void AnimationComponent::onSaveLoad(hd::SerializerRW &stream) {
    stream.process<hd::Time>(this->frameTime);
    stream.process<glm::ivec2>(this->frameSize);
}

SpriteSystem::SpriteSystem(Engine &engine) : AECSSystem(engine) {
}

SpriteSystem::~SpriteSystem() {
}

void SpriteSystem::onInitialize() {
    mEngine.getSceneSystem().registerComponentType<SpriteComponent>();
    mEngine.getSceneSystem().registerComponentType<AnimationComponent>();
}

void SpriteSystem::onUpdate(const std::vector<GameObject*> &objects) {
    for (auto &obj : objects) {
        if (obj) {
            SpriteComponent *sprite = obj->getComponent<SpriteComponent>();
            AnimationComponent *anim = obj->getComponent<AnimationComponent>();
            if (sprite) {
                if (!sprite->_texture) {
                    sprite->_texture = mEngine.getCacheSystem().loadTexture(sprite->texturePath);
                }
                if (anim) {
                    if (hd::Clock::getElapsedTime(anim->_timer) >= anim->frameTime) {
                        anim->_uv.x += anim->frameSize.x;
                        if (anim->_uv.x >= mEngine.getRenderSystem().getTextureSize(sprite->_texture).x) {
                            anim->_uv.x = 0;
                        }
                        anim->_timer = hd::Clock::getTime();
                    }
                }
            }
        }
    }
}

void SpriteSystem::onDraw(const std::vector<GameObject*> &objects) {
    for (auto &obj : objects) {
        if (obj) {
            SpriteComponent *sprite = obj->getComponent<SpriteComponent>();
            if (sprite) {
                AnimationComponent *anim = obj->getComponent<AnimationComponent>();
                RenderOp rop;
                rop.texture = sprite->_texture;
                rop.pos = obj->getAbsolutePosition();
                rop.size = obj->getSize();
                rop.angle = obj->getAngle();
                if (anim) {
                    rop.uvOffset = anim->_uv;
                    rop.uvSize = anim->frameSize;
                }
                mEngine.getRenderSystem().addRenderOp(rop);
            }
        }
    }
}

}
