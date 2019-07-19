#pragma once
#include "SceneSystem.hpp"
#include <string>

namespace hg2d {

struct Texture;

class SpriteComponent final : public AECSComponent {
public:
    SpriteComponent(Engine &engine);

    virtual void onSaveLoad(JSONObject &json, bool isLoad) override;

    void setTexture(const Texture *texture);

    const Texture *getTexture() const;

private:
    const Texture *mTexture;
};

class SpriteSystem final : public AECSSystem {
public:
    SpriteSystem(Engine &engine);

    virtual void onInitialize() override;
    virtual void onDraw() override;
};

}
