#include "GUILabel.hpp"
#include "../Renderer2D/RenderSystem2D.hpp"
#include "../GUI/GUISystem.hpp"

namespace hg {

GUILabel::GUILabel() : mColor(getGUISystem().getSkin().fontColor) {
}

void GUILabel::onSaveLoad(hd::JSON &data, bool isLoad) {
    BaseClassName::onSaveLoad(data, isLoad);

    if (isLoad) {
        mText = data["text"].get<std::string>();
        mColor = data["color"].get<glm::vec4>();
    }
    else {
        data["text"] = mText;
        data["color"] = mColor;
    }
}

void GUILabel::onUpdate(float dt) {
    BaseClassName::onUpdate(dt);

    if (!mText.empty() && mTexture) {
        getRenderSystem2D().drawTextureGUI(mTexture, getAbsolutePosition(), getSize());
    }
}

void GUILabel::setText(const std::string &text, bool updateSize) {
    if (mText != text) {
        mText = text;
        mUpdateTexture();
        if (updateSize) {
            setSize(mTexture->getSize());
        }
    }
}

const std::string &GUILabel::getText() const {
    return mText;
}

void GUILabel::setColor(const glm::vec4 &color) {
    if (mColor != color) {
        mColor = color;
        mUpdateTexture();
    }
}

const glm::vec4 &GUILabel::getColor() const {
    return mColor;
}

void GUILabel::mUpdateTexture() {
    if (!mText.empty()) {
        hd::Image image = getGUISystem().getSkin().font->renderText(mText, mColor);
        mTexture = Texture2D::createFromImage(image);
    }
}

}
