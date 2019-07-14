#pragma once
#include "AECSComponent.hpp"
#include "AECSSystem.hpp"
#include "hd/Core/hdClock.hpp"
#include "3rd/include/glm/glm.hpp"

namespace hg2d {

struct Texture;

struct SpriteComponent : public AECSComponent {
    void onSaveLoad(hd::SerializerRW &stream);

    std::string texturePath;
    Texture *_texture = nullptr;
};

struct AnimationComponent : public AECSComponent {
    void onSaveLoad(hd::SerializerRW &stream);

    hd::Time frameTime;
    glm::ivec2 frameSize;
    glm::ivec2 _uv;
    hd::Time _timer;
};

class SpriteSystem : public AECSSystem {
public:
    SpriteSystem(Engine &engine);
    ~SpriteSystem();

    void onInitialize();
    void onUpdate(const std::vector<GameObject*> &objects);
    void onDraw(const std::vector<GameObject*> &objects);
};

}
