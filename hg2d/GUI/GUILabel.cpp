#include "GUILabel.hpp"
#include "../Renderer/RenderSystem.hpp"
#include "../GUI/GUISystem.hpp"

namespace hg2d {

GUILabel::GUILabel() : mColor(getGUISystem().getSkin().fontColor) {
    mTexture = nullptr;
}

GUILabel::~GUILabel() {
    getRenderSystem().destroyTexture(mTexture);
}

void GUILabel::setText(const std::string &text, bool updateSize) {
    if (mText != text) {
        mText = text;
        mUpdateTexture();
        if (updateSize) {
            setSize(getRenderSystem().getTextureSize(mTexture));
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

void GUILabel::onDraw() {
    if (!mText.empty() && mTexture) {
        RenderOp rop;
        rop.texture = mTexture;
        rop.pos = getAbsolutePosition();
        rop.size = getSize();
        getRenderSystem().addRenderOp(rop, true);
    }

    GUIWidget::onDraw();
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
