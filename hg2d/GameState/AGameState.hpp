#pragma once
#include "hd/Window/WindowEvent.hpp"

namespace hg2d {

class AGameState {
public:
    virtual void onEvent(const hd::WindowEvent &event);
	virtual void onChangeCurrentState(AGameState *lastState);
    virtual void onFixedUpdate();
    virtual void onUpdate();
    virtual void onDraw();
};

}
