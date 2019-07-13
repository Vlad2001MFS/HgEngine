#include "GUILabel.hpp"
#include "../Core/Engine.hpp"

namespace hg2d {

GUILabel::GUILabel(Engine &engine) : AGUIWidget(engine) {
    mTexture = nullptr;
    mColor = getEngine().getGUISystem().getSkin().fontColor;
}

GUILabel::~GUILabel() {
    getEngine().getRenderSystem().destroyTexture(mTexture);
}

void GUILabel::setText(const std::string &text) {
    if (mText != text) {
        mText = text;
        mUpdateTexture();
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
        rop.isGUI = true;
        getEngine().getRenderSystem().addRenderOp(rop);
    }
}

void GUILabel::mUpdateTexture() {
    if (!mText.empty()) {
        if (mTexture) {
            getEngine().getRenderSystem().destroyTexture(mTexture);
        }

        mTexture = getEngine().getGUISystem().getSkin().font->renderText(mText, mColor);
        if (getSize().x == 0 && getSize().y == 0) {
            setSize(getEngine().getRenderSystem().getTextureSize(mTexture));
        }
    }
}
    
}
