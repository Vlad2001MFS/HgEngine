#include "AEngineObject.hpp"
#include "Engine.hpp"

namespace hg2d {

AEngineObject::AEngineObject(Engine& engine) : 
    mEngine(engine),
    mGameStateSystem(engine.getGameStateSystem()),
    mRenderSystem(engine.getRenderSystem()),
    mSoundSystem(engine.getSoundSystem()),
    mGUISystem(engine.getGUISystem()),
    mCacheSystem(engine.getCacheSystem()),
    mSceneSystem(engine.getSceneSystem()) {
}

}