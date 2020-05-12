#pragma once
#include "Font.hpp"
#include "../Graphics/Texture2D.hpp"
#include "../Core/WindowEvent.hpp"
#include "hd/Core/StringHash.hpp"
#include <unordered_map>

namespace hg {

struct GUISkin {
    FontPtr font;
    glm::vec4 fontColor;
    Texture2DPtr buttonTexture, hoveredButtonTexture, clickedButtonTexture;
    int alignSpaceX, alignSpaceY;
};

class GUISystem {
public:
    GUISystem();
    ~GUISystem();

    void onEvent(const WindowEvent &event);
    void onUpdate(float dt);

    FontPtr loadFont(const std::string &path, uint32_t size);

    const GUISkin &getSkin() const;

private:
    GUISkin mSkin;
    std::unordered_map<hd::StringHash, FontPtr> mFontsDB;
};

GUISystem &getGUISystem();

}
