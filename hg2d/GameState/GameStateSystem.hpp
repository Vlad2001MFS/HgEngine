#pragma once
#include "AGameState.hpp"
#include "hd/Core/Common.hpp"
#include "hd/Core/Log.hpp"
#include <string>
#include <map>

namespace hg2d {

class GameStateSystem final : public hd::Singleton<GameStateSystem> {
public:
    GameStateSystem();

    void initialize();
    void shutdown();

    void onEvent(const hd::WindowEvent &event);
    void onFixedUpdate();
    void onUpdate();
    void onDraw();

    template<typename T, typename ...Args>
    void createState(const std::string &name, Args &&...args) {
        if (!name.empty()) {
            auto it = mStates.find(name);
            if (it == mStates.end()) {
                mStates.insert(std::make_pair(name, new T(args...)));
            }
            else {
                LOG_F(WARNING, "Failed to register GameState. The GameState '{}' already registered at GameStateSystem", name.data());
            }
        }
        else {
            LOG_F(WARNING, "Failed to register GameState without name");
        }
    }
    void destroyState(const std::string &name);
    void setState(const std::string &name);
	AGameState* getState(const std::string &name) const;

private:
	void mSetState(AGameState *state);

    std::map<std::string, AGameState*> mStates;
    AGameState *mCurrentState;
};

inline GameStateSystem &getGameStateSystem() {
    return GameStateSystem::get();
}

}
