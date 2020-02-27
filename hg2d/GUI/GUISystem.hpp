#pragma once
#include "GUIWidget.hpp"
#include "Font.hpp"
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

class GUISystem final : public hd::Singleton<GUISystem> {
public:
    void initialize();
    void shutdown();

    Font *createFontFromFile(const std::string &filename, uint32_t size);
    void destroyFont(Font *&font);
    const GUISkin &getSkin() const { return mSkin; }

private:
    GUISkin mSkin;
    std::vector<Font*> mCreatedFonts;
};

inline GUISystem &getGUISystem() {
    return GUISystem::get();
}
    
}
