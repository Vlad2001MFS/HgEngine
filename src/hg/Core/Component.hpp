#pragma once
#include "Object.hpp"
#include "hd/Core/JSON.hpp"
#include "../Core/WindowEvent.hpp"

namespace hg {

class Node;

class Component : public Object {
    HG_OBJECT(Component, Object);

    friend class Node;
public:
    Component();

    virtual void onSaveLoad(hd::JSON &data, bool isLoad);
    virtual void onEvent(const WindowEvent &event);
    virtual void onFixedUpdate();
    virtual void onUpdate(float dt);

    Node *getOwner() const;

private:
    Node *mOwner;
};

}
