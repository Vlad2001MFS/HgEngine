#pragma once
#include "../Core/Object.hpp"
#include "../Core/WindowEvent.hpp"
#include "hd/Core/JSON.hpp"

namespace hg {

class GameObject;

class Component : public Object {
    HG_OBJECT(Component, Object);
    friend class GameObject;
public:
    virtual void onSaveLoad(hd::JSON &data, bool isLoad);
    virtual void onCreate();
    virtual void onFirstUpdate();
    virtual void onTransformUpdate(bool isPosUpdate, bool isSizeUpdate, bool isAngleUpdate);
    virtual void onEvent(const WindowEvent &event);
    virtual void onFixedUpdate();
    virtual void onUpdate(float dt);

    GameObject *getOwner() const;

private:
    GameObject *mOwner = nullptr;
};

}
