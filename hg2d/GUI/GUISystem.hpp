#pragma once
#include "AGUIWidget.hpp"
#include "Font.hpp"
#include "../Core/AEngineObject.hpp"
#include "hd/Window/WindowEvent.hpp"
#include <map>

namespace hg2d {

struct GUISkin {
    Font *font = nullptr;
    hd::Color4 fontColor = hd::Color4::White;
    Texture *buttonTexture = nullptr, *hoveredButtonTexture = nullptr, *clickedButtonTexture = nullptr;
    int alignSpaceX = 10;
    int alignSpaceY = 10;
};

class GUISystem final : public AEngineObject {
public:
    explicit GUISystem(Engine &engine);

    void onInitialize();
    void onShutdown();
    void onEvent(const hd::WindowEvent &event);
    void onFixedUpdate();
    void onUpdate();
    void onDraw();

    Font *createFontFromFile(const std::string &filename, uint32_t size);
    void destroyFont(Font *&font);
    const GUISkin &getSkin() const { return mSkin; }

    template<typename T, typename ...Args>
    T *createFrame(const std::string &name, Args &&...args) { T *frame = new T(mEngine, args...); mAddFrame(frame, name); return frame; }
    void destroyFrame(const std::string &name);
    void setFrame(const std::string &name);

private:
    void mAddFrame(AGUIWidget *frame, const std::string &name);
    void mDestroyFrame(AGUIWidget *&frame);

    GUISkin mSkin;
    std::vector<Font*> mCreatedFonts;
    std::map<std::string, AGUIWidget*> mFrames;
    AGUIWidget *mCurrentFrame;
};
    
}
