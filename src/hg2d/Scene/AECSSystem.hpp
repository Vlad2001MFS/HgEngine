#pragma once
#include "hd/System/hdWindowEvent.hpp"
#include <vector>

namespace hg2d {

class Engine;
class GameObject;

class AECSSystem {
public:
    explicit AECSSystem(Engine &engine);
    virtual ~AECSSystem();

    virtual void onInitialize();
    virtual void onShutdown();
    virtual void onEvent(const std::vector<GameObject*> &objects, const hd::WindowEvent &event);
    virtual void onFixedUpdate(const std::vector<GameObject*> &objects);
    virtual void onUpdate(const std::vector<GameObject*> &objects);
    virtual void onDraw(const std::vector<GameObject*> &objects);

protected:
    Engine &mEngine;
};

}
