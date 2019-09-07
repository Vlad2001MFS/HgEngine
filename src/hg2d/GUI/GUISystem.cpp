#include "GUISystem.hpp"
#include "../Core/Engine.hpp"
#include "../Renderer/RenderSystem.hpp"
#include "hd/Core/hdLog.hpp"
#include <algorithm>

namespace hg2d {

GUISystem::GUISystem(Engine &engine) : AEngineObject(engine) {
    mCurrentFrame = nullptr;
}

void GUISystem::onInitialize() {
    mSkin.font = createFontFromFile(mEngine.getCreateInfo().gui.fontPath, mEngine.getCreateInfo().gui.fontSize);
    mSkin.font->setHinting(FontHinting::Mono);
    mSkin.fontColor = mEngine.getCreateInfo().gui.fontColor;
    mSkin.buttonTexture = mRenderSystem.createTextureFromFile(mEngine.getCreateInfo().gui.buttonTexturePath);
    mSkin.hoveredButtonTexture = mRenderSystem.createTextureFromFile(mEngine.getCreateInfo().gui.hoveredButtonTexturePath);
    mSkin.clickedButtonTexture = mRenderSystem.createTextureFromFile(mEngine.getCreateInfo().gui.clickedButtonTexturePath);
    mSkin.alignSpaceX = mEngine.getCreateInfo().gui.alignSpaceX;
    mSkin.alignSpaceY = mEngine.getCreateInfo().gui.alignSpaceY;
}

void GUISystem::onShutdown() {
    mRenderSystem.destroyTexture(mSkin.clickedButtonTexture);
    mRenderSystem.destroyTexture(mSkin.hoveredButtonTexture);
    mRenderSystem.destroyTexture(mSkin.buttonTexture);
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
        Font *font = new Font(mEngine, path, size);
        mCreatedFonts.push_back(font);
        return font;
    }
    else {
        HD_LOG_ERROR("Failed to create font from file '%s' with size '%d'", filename.data(), size);
        return nullptr;
    }
}

void GUISystem::destroyFont(Font *&font) {
    if (!font) {
        HD_LOG_WARNING("font is nullptr");
    }
    else {
        auto it = std::find(mCreatedFonts.begin(), mCreatedFonts.end(), font);
        if (it != mCreatedFonts.end()) {
            HD_DELETE(font);
            mCreatedFonts.erase(std::remove(mCreatedFonts.begin(), mCreatedFonts.end(), font), mCreatedFonts.end());
        }
        else {
            HD_LOG_WARNING("Failed to destroy Font. The Font wasn't created by GUISystem");
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
        HD_LOG_WARNING("Failed to destroy GUIFrame. The GUIFrame '%s' not registered at GUISystem", name.data());
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
                HD_LOG_WARNING("Failed to set GUIFrame. The GUIFrame '%s' already current frame", name.data());
            }
        }
        else {
            HD_LOG_WARNING("Failed to set GUIFrame. The GUIFrame '%s' not registered at GUISystem", name.data());
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
            HD_LOG_WARNING("Failed to add GUIFrame. The GUIFrame '%s' already registered at GUISystem", name.data());
        }
    }
    else {
        HD_LOG_WARNING("Failed to add GUIFrame without name");
    }
}

}
