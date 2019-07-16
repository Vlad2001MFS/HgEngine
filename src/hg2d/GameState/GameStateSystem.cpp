#include "GameStateSystem.hpp"
#include "hd/Core/hdCommon.hpp"

namespace hg2d {

GameStateSystem::GameStateSystem(Engine &engine) : AEngineObject(engine) {
}

void GameStateSystem::onInitialize() {
}

void GameStateSystem::onShutdown() {
    for (auto &it : mStates) {
        mDestroyState(it.second);
    }
}

void GameStateSystem::destroyState(const std::string &name) {
    auto it = mStates.find(name);
    if (it != mStates.end()) {
        mDestroyState(it->second);
        mStates.erase(it);
    }
    else {
        HD_LOG_WARNING("Failed to destroy GameState. The GameState '%s' not registered at GameStateSystem", name.data());
    }
}

void GameStateSystem::pushState(const std::string& name) {
    auto it = mStates.find(name);
    if (it != mStates.end()) {
        if (mStateStack.empty() || mStateStack.top() != it->second) {
            mStateStack.push(it->second);
        }
        else {
            HD_LOG_WARNING("Failed to push GameState. The GameState '%s' already on top of the stack of states", name.data());
        }
    }
    else {
        HD_LOG_WARNING("Failed to push GameState. The GameState '%s' not registered at GameStateSystem", name.data());
    }
}

void GameStateSystem::popState() {
    if (mStateStack.size() > 1) {
        mStateStack.pop();
    }
    else {
        HD_LOG_WARNING("Failed to pop GameState. Impossible to pop the last GameState from stack of states");
    }
}

void GameStateSystem::onEvent(const hd::WindowEvent &event) {
    if (!mStateStack.empty()) {
        mStateStack.top()->onEvent(event);
    }
    else {
        HD_LOG_WARNING("The stack of GameStates is empty!");
    }
}

void GameStateSystem::onFixedUpdate() {
    if (!mStateStack.empty()) {
        mStateStack.top()->onFixedUpdate();
    }
    else {
        HD_LOG_WARNING("The stack of GameStates is empty!");
    }
}

void GameStateSystem::onUpdate() {
    if (!mStateStack.empty()) {
        mStateStack.top()->onUpdate();
    }
    else {
        HD_LOG_WARNING("The stack of GameStates is empty!");
    }
}

void GameStateSystem::onDraw() {
    if (!mStateStack.empty()) {
        mStateStack.top()->onDraw();
    }
    else {
        HD_LOG_WARNING("The stack of GameStates is empty!");
    }
}

void GameStateSystem::mAddState(AGameState *state, const std::string &name) {
    if (!name.empty()) {
        auto it = mStates.find(name);
        if (it == mStates.end()) {
            mStates.insert(std::make_pair(name, state));
            state->onInitialize();
        }
        else {
            HD_DELETE(state);
            HD_LOG_WARNING("Failed to register GameState. The GameState '%s' already registered at GameStateSystem", name.data());
        }
    }
    else {
        HD_DELETE(state);
        HD_LOG_WARNING("Failed to register GameState without name");
    }
}

void GameStateSystem::mDestroyState(AGameState *&state) {
    state->onShutdown();
    HD_DELETE(state);
}

}
