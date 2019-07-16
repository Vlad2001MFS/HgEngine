#pragma once
#include "SceneSystem.hpp"
#include <string>

namespace hg2d {

struct Texture;

class SpriteComponent : public AECSComponent {
public:
    SpriteComponent();

    void setTexture(const Texture *texture);

    const Texture *getTexture() const;

private:
    const Texture *mTexture;
};

class SpriteSystem : public AECSSystem {
public:
    SpriteSystem(Engine &engine);

    virtual void onDraw() override;
};

}
