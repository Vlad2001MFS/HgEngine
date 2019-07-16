#pragma once
#include "AGameState.hpp"
#include "../Core/AEngineObject.hpp"
#include <string>
#include <map>
#include <stack>

namespace hg2d {

class GameStateSystem final : public AEngineObject {
public:
    explicit GameStateSystem(Engine &engine);

    void onInitialize();
    void onShutdown();
    void onEvent(const hd::WindowEvent &event);
    void onFixedUpdate();
    void onUpdate();
    void onDraw();

    template<typename T, typename ...Args>
    void createState(const std::string &name, Args &&...args) { mAddState(new T(mEngine, args...), name); }
    void destroyState(const std::string &name);
    void pushState(const std::string &name);
    void popState();

private:
    void mAddState(AGameState *state, const std::string &name);
    void mDestroyState(AGameState *&state);

    std::map<std::string, AGameState*> mStates;
    std::stack<AGameState*> mStateStack;
};

}
