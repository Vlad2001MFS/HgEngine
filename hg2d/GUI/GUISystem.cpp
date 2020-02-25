#include "GUISystem.hpp"
#include "../Core/Engine.hpp"
#include "../Renderer/RenderSystem.hpp"
#include "hd/Core/Log.hpp"
#include <algorithm>

namespace hg2d {

GUISystem::GUISystem() {
    mCurrentFrame = nullptr;
}

void GUISystem::initialize() {
    mSkin.font = createFontFromFile(getEngine().getCreateInfo().gui.fontPath, getEngine().getCreateInfo().gui.fontSize);
    mSkin.font->setHinting(FontHinting::Mono);
    mSkin.fontColor = getEngine().getCreateInfo().gui.fontColor;
    mSkin.buttonTexture = getRenderSystem().createTextureFromFile(getEngine().getCreateInfo().gui.buttonTexturePath);
    mSkin.hoveredButtonTexture = getRenderSystem().createTextureFromFile(getEngine().getCreateInfo().gui.hoveredButtonTexturePath);
    mSkin.clickedButtonTexture = getRenderSystem().createTextureFromFile(getEngine().getCreateInfo().gui.clickedButtonTexturePath);
    mSkin.alignSpaceX = getEngine().getCreateInfo().gui.alignSpaceX;
    mSkin.alignSpaceY = getEngine().getCreateInfo().gui.alignSpaceY;
}

void GUISystem::shutdown() {
    getRenderSystem().destroyTexture(mSkin.clickedButtonTexture);
    getRenderSystem().destroyTexture(mSkin.hoveredButtonTexture);
    getRenderSystem().destroyTexture(mSkin.buttonTexture);
    for (auto &it : mCreatedFonts) {
        HD_DELETE(it);
    }
    for (auto &it : mFrames) {
        mDestroyFrame(it.second);
    }
}

Font *GUISystem::createFontFromFile(const std::string &filename, uint32_t size) {
    if (!filename.empty() && size > 0) {
        std::string path = "data/configs/" + filename;
        Font *font = new Font(path, size);
        mCreatedFonts.push_back(font);
        return font;
    }
    else {
        LOG_F(FATAL, "Failed to create font from file '{}' with size '{}'", filename.data(), size);
        return nullptr;
    }
}

void GUISystem::destroyFont(Font *&font) {
    if (!font) {
        LOG_F(WARNING, "font is nullptr");
    }
    else {
        auto it = std::find(mCreatedFonts.begin(), mCreatedFonts.end(), font);
        if (it != mCreatedFonts.end()) {
            HD_DELETE(font);
            mCreatedFonts.erase(std::remove(mCreatedFonts.begin(), mCreatedFonts.end(), font), mCreatedFonts.end());
        }
        else {
            LOG_F(WARNING, "Failed to destroy Font. The Font wasn't created by GUISystem");
        }
    }
}

void GUISystem::destroyFrame(const std::string &name) {
    auto it = mFrames.find(name);
    if (it != mFrames.end()) {
        mDestroyFrame(it->second);
        mFrames.erase(name);
    }
    else {
        LOG_F(WARNING, "Failed to destroy GUIFrame. The GUIFrame '{}' not registered at GUISystem", name.data());
    }
}

void GUISystem::setFrame(const std::string& name) {
    if (name.empty()) {
        mCurrentFrame = nullptr;
    }
    else {
        auto it = mFrames.find(name);
        if (it != mFrames.end()) {
            if (mCurrentFrame != it->second) {
                mCurrentFrame = it->second;
            }
            else {
                LOG_F(WARNING, "Failed to set GUIFrame. The GUIFrame '{}' already current frame", name.data());
            }
        }
        else {
            LOG_F(WARNING, "Failed to set GUIFrame. The GUIFrame '{}' not registered at GUISystem", name.data());
        }
    }
}

void GUISystem::onEvent(const hd::WindowEvent &event) {
    if (event.type == hd::WindowEventType::Resize) {
        for (auto &frame : mFrames) {
            frame.second->setSize(event.resize.width, event.resize.height);
        }
    }
    if (mCurrentFrame) {
        mCurrentFrame->_onEvent(event);
    }
}

void GUISystem::onFixedUpdate() {
    if (mCurrentFrame) {
        mCurrentFrame->_onFixedUpdate();
    }
}

void GUISystem::onUpdate() {
    if (mCurrentFrame) {
        mCurrentFrame->_onUpdate();
    }
}

void GUISystem::onDraw() {
    if (mCurrentFrame) {
        mCurrentFrame->_onDraw();
    }
}

void GUISystem::mDestroyFrame(AGUIWidget *&frame) {
    frame->_onShutdown();
    HD_DELETE(frame);
}

void GUISystem::mAddFrame(AGUIWidget *frame, const std::string &name) {
    if (!name.empty()) {
        auto it = mFrames.find(name);
        if (it == mFrames.end()) {
            mFrames.insert(std::make_pair(name, frame));
            frame->_onInitialize();
        }
        else {
            LOG_F(WARNING, "Failed to add GUIFrame. The GUIFrame '{}' already registered at GUISystem", name.data());
        }
    }
    else {
        LOG_F(WARNING, "Failed to add GUIFrame without name");
    }
}

}
