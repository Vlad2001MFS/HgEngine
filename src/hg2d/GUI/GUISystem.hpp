#pragma once
#include "AGUIWidget.hpp"
#include "Font.hpp"
#include "hd/System/hdWindowEvent.hpp"
#include <map>
#include <stack>

namespace hg2d {

struct GUISystemCreateInfo {
    std::string fontPath = "font.ttf";
    uint32_t fontSize = 16;
    bool monospacedFont = true;
    hd::Color4 fontColor = hd::Color4::White;
    std::string buttonTexturePath = "button.png";
    std::string hoveredButtonTexturePath = "hoveredButton.png";
    std::string clickedButtonTexturePath = "clickedButton.png";
    int alignSpaceX = 10;
    int alignSpaceY = 10;
};

struct GUISkin {
    Font *font = nullptr;
    hd::Color4 fontColor = hd::Color4::White;
    Texture *buttonTexture = nullptr, *hoveredButtonTexture = nullptr, *clickedButtonTexture = nullptr;
    int alignSpaceX = 10;
    int alignSpaceY = 10;
};

class GUISystem {
public:
    explicit GUISystem(Engine &engine);
    ~GUISystem();

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
    void pushFrame(const std::string &name);
    void popFrame();

private:
    void mAddFrame(AGUIWidget *frame, const std::string &name);
    void mDestroyFrame(AGUIWidget *&frame);

    Engine &mEngine;
    GUISkin mSkin;
    std::vector<Font*> mCreatedFonts;
    std::map<std::string, AGUIWidget*> mFrames;
    std::stack<AGUIWidget*> mFrameStack;
};
    
}
