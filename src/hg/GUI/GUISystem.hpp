#pragma once
#include "GUIWidget.hpp"
#include "Font.hpp"
#include "../Graphics/Texture2D.hpp"
#include "../Core/WindowEvent.hpp"
#include "hd/Core/StringHash.hpp"
#include "hd/Core/Log.hpp"
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
    void onFixedUpdate();
    void onUpdate(float dt);

    template<typename T>
    T *createFrame(const std::string &name) {
        hd::StringHash nameHash = hd::StringHash(name);
        auto it = mFramesDB.find(nameHash);
        if (it == mFramesDB.end()) {
            T *frame = new T();
            mFramesDB.insert(it, std::make_pair(nameHash, frame));
            return frame;
        }
        else {
            HD_LOG_WARNING("GUI frame '{}' already exist!", name);
            return it->second;
        }
    }

    void setActiveFrame(const std::string &name);

    FontPtr loadFont(const std::string &path, uint32_t size);

    const GUISkin &getSkin() const;

private:
    GUISkin mSkin;
    std::unordered_map<hd::StringHash, GUIWidget*> mFramesDB;
    std::unordered_map<hd::StringHash, FontPtr> mFontsDB;
    GUIWidget *mActiveFrame = nullptr;
};

GUISystem &getGUISystem();

}
