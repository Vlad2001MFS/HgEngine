#include "GUILabel.hpp"
#include "../Renderer/RenderSystem.hpp"
#include "../GUI/GUISystem.hpp"

namespace hg2d {

GUILabel::GUILabel() : mIsUserSize(false, false) {
    mTexture = nullptr;
    mColor = getGUISystem().getSkin().fontColor;
}

GUILabel::~GUILabel() {
    getRenderSystem().destroyTexture(mTexture);
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
        getRenderSystem().addRenderOp(rop, true);
    }
}

void GUILabel::setSize(int x, int y) {
    mIsUserSize.x = x != 0;
    mIsUserSize.y = y != 0;
    AGUIWidget::setSize(x, y);
}

void GUILabel::setSize(const glm::ivec2 &size) {
    mIsUserSize.x = size.x != 0;
    mIsUserSize.y = size.y != 0;
    AGUIWidget::setSize(size);
}

void GUILabel::mUpdateTexture() {
    if (!mText.empty()) {
        if (mTexture) {
            getRenderSystem().destroyTexture(mTexture);
        }
        
        mTexture = getGUISystem().getSkin().font->renderText(mText, mColor);
        if (!mIsUserSize.x) {
            AGUIWidget::setSize(getRenderSystem().getTextureSize(mTexture).x, getSize().y);
        }
        if (!mIsUserSize.y) {
            AGUIWidget::setSize(getSize().x, getRenderSystem().getTextureSize(mTexture).y);
        }
    }
}
    
}
