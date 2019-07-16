#pragma once

namespace hg2d {

class Engine;
class GameStateSystem;
class RenderSystem;
class SoundSystem;
class GUISystem;
class CacheSystem;
class SceneSystem;

class AEngineObject {
public:
    AEngineObject(Engine &engine);
    virtual ~AEngineObject() = default;

protected:
    Engine &mEngine;
    GameStateSystem &mGameStateSystem;
    RenderSystem &mRenderSystem;
    SoundSystem &mSoundSystem;
    GUISystem &mGUISystem;
    CacheSystem &mCacheSystem;
    SceneSystem &mSceneSystem;
};

}