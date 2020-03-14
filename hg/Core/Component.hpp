#pragma once
#include "Object.hpp"
#include "JSONObject.hpp"
#include "SDL2/SDL_events.h"

namespace hg {

class Node;

class Component : public Object {
    HG_OBJECT(Component, Object);

    friend class Node;
public:
    Component();

    virtual void onSaveLoad(JSONObject &data, bool isLoad);
    virtual void onEvent(const SDL_Event &event);
    virtual void onFixedUpdate();
    virtual void onUpdate(float dt);

    Node *getOwner() const;

private:
    Node *mOwner;
};

}