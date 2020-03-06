#pragma once
#include "hd/Core/Common.hpp"
#include "../../imgui/imgui.h"
#include "SDL2/SDL_events.h"
#include <map>

namespace hg {

class GUISystem final : public hd::Singleton<GUISystem> {
public:
    void initialize();
    void shutdown();

    void onEvent(const SDL_Event &event);
    void onUpdate(float dt);
    void onDraw();
};

inline GUISystem &getGUISystem() {
    return GUISystem::get();
}
    
}
