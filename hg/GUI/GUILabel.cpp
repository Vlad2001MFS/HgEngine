#include "GUILabel.hpp"
#include "../Renderer/RenderSystem.hpp"
#include "../GUI/GUISystem.hpp"
#include "../Cache/CacheSystem.hpp"

namespace hg {

HG_REGISTER_OBJECT(GUILabel);

GUILabel::GUILabel() : mColor(getGUISystem().getSkin().fontColor) {
    mTexture = nullptr;
}

GUILabel::~GUILabel() {
    getRenderSystem().destroyTexture(mTexture);
}

void GUILabel::onSaveLoad(JSONObject &data, bool isLoad) {
    if (isLoad) {
        mTexture = getCacheSystem().loadTexture(data["texture"].get<std::string>());
        mText = data["text"].get<std::string>();
        mColor = data["color"].get<hd::Color4>();
    }
    else {
        data["texture"] = getRenderSystem().getTexturePath(mTexture);
        data["text"] = mText;
        data["color"] = mColor;
    }

    BaseClassName::onSaveLoad(data, isLoad);
}

void GUILabel::onEvent(const hd::WindowEvent &event) {
    if (event.type == hd::WindowEventType::Resize) {
        mUpdateTexture();
    }

    BaseClassName::onEvent(event);
}

void GUILabel::onDraw() {
    if (!mText.empty() && mTexture) {
        RenderOp rop;
        rop.texture = mTexture;
        rop.pos = getAbsolutePosition();
        rop.size = getAbsoluteSize();
        getRenderSystem().addRenderOp(rop, true);
    }

    BaseClassName::onDraw();
}

void GUILabel::setText(const std::string &text, bool updateSize) {
    if (mText != text) {
        mText = text;
        mUpdateTexture();
        if (updateSize) {
            setAbsoluteSize(getRenderSystem().getTextureSize(mTexture));
        }
    }
}

const std::string& GUILabel::getText() const {
    return mText;
}

void GUILabel::setColor(const hd::Color4 &color) {
    if (mColor != color) {
        mColor = color;
        mUpdateTexture();
    }
}

const hd::Color4& GUILabel::getColor() const {
    return mColor;
}

void GUILabel::mUpdateTexture() {
    if (!mText.empty()) {
        if (mTexture) {
            getRenderSystem().destroyTexture(mTexture);
        }
        
        mTexture = getGUISystem().getSkin().font->renderText(mText, mColor);
    }
}
    
}
