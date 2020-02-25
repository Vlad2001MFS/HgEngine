#pragma once
#include "TransformComponent.hpp"
#include "SceneSystem.hpp"
#include <string>

namespace hg2d {

struct Texture;

class SpriteComponent final : public AECSComponent {
public:
    SpriteComponent();

    virtual void onSaveLoad(JSONObject &json, bool isLoad) override;

    void setTexture(const Texture *texture);
    const Texture *getTexture() const;

private:
    const Texture *mTexture;
};

class CameraComponent final : public AECSComponent {
public:
    CameraComponent();

    virtual void onSaveLoad(JSONObject &json, bool isLoad) override;

    void setZoom(float zoom);
    float getZoom() const;

private:
    float mZoom;
};

class SpriteSystem final : public AECSSystem {
public:
    SpriteSystem();

    virtual void onInitialize() override;
    virtual void onSaveLoad(JSONObject &json, bool isLoad) override;
    virtual void onClear() override;
    virtual void onDraw() override;

    void setCameraEntity(const HEntity &handle);
    const HEntity &getCameraEntity() const;

private:
    HEntity mCameraEntity;
    TransformComponent *mTransformComponent;
    CameraComponent *mCameraComponent;
};

}
