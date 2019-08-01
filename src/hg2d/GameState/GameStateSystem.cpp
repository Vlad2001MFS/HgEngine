#include "GameStateSystem.hpp"
#include "hd/Core/hdCommon.hpp"

namespace hg2d {

GameStateSystem::GameStateSystem(Engine &engine) : AEngineObject(engine) {
    mCurrentState = nullptr;
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

void GameStateSystem::setState(const std::string& name) {
    if (!name.empty()) {
		auto it = mStates.find(name);
        if (it != mStates.end()) {
            if (mCurrentState != it->second) {
                mSetState(it->second);
            }
            else {
                HD_LOG_WARNING("Failed to set GameState. The GameState '%s' already current state", name.data());
            }
        }
        else {
            HD_LOG_WARNING("Failed to set GameState. The GameState '%s' not registered at GameStateSystem", name.data());
        }
    }
	else {
		HD_LOG_ERROR("Failed to set GameState without name");
	}
}

AGameState *GameStateSystem::getState(const std::string &name) const {
	if (!name.empty()) {
		auto it = mStates.find(name);
		if (it != mStates.end()) {
			return it->second;
		}
		else {
			HD_LOG_WARNING("Failed to get GameState. The GameState '%s' not registered at GameStateSystem", name.data());
			return nullptr;
		}
	}
	else {
		HD_LOG_WARNING("Try to get GameState with empty name");
		return nullptr;
	}
}

void GameStateSystem::onEvent(const hd::WindowEvent &event) {
    if (mCurrentState) {
        mCurrentState->onEvent(event);
    }
    else {
        HD_LOG_WARNING("Current GameState is nullptr!");
    }
}

void GameStateSystem::onFixedUpdate() {
    if (mCurrentState) {
        mCurrentState->onFixedUpdate();
    }
    else {
        HD_LOG_WARNING("Current GameState is nullptr!");
    }
}

void GameStateSystem::onUpdate() {
    if (mCurrentState) {
        mCurrentState->onUpdate();
    }
    else {
        HD_LOG_WARNING("Current GameState is nullptr!");
    }
}

void GameStateSystem::onDraw() {
    if (mCurrentState) {
        mCurrentState->onDraw();
    }
    else {
        HD_LOG_WARNING("Current GameState is nullptr!");
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

void GameStateSystem::mSetState(AGameState *state) {
	state->onChangeCurrentState(mCurrentState);
	mCurrentState = state;
}

}
