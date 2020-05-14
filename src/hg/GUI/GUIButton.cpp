#include "GUIButton.hpp"
#include "GUIImage.hpp"
#include "GUILabel.hpp"
#include "../GUI/GUISystem.hpp"

namespace hg {

GUIButton::GUIButton() {
    mBgImage = createChild<GUIImage>();
    mBgImage->setExpand(true);
    mBgImage->setTexture(getGUISystem().getSkin().buttonTexture);

    mTextLabel = createChild<GUILabel>();
    mTextLabel->setAlign(GUIHAlign::Center, GUIVAlign::Center);
    onMouseButtonPressed += std::bind(&GUIButton::mOnMouseButtonPressed, this);
    onMouseButtonReleased += std::bind(&GUIButton::mOnMouseButtonReleased, this);
    onMouseEnter += std::bind(&GUIButton::mOnMouseEnter, this);
    onMouseLeave += std::bind(&GUIButton::mOnMouseLeave, this);
}

void GUIButton::setText(const std::string &text) {
    mTextLabel->setText(text, true);
}

const std::string& GUIButton::getText() const {
    return mTextLabel->getText();
}

void GUIButton::mOnMouseButtonPressed() {
    mBgImage->setTexture(getGUISystem().getSkin().clickedButtonTexture);
}

void GUIButton::mOnMouseButtonReleased() {
    if (isMouseHovered()) {
        mBgImage->setTexture(getGUISystem().getSkin().hoveredButtonTexture);
    }
    else {
        mBgImage->setTexture(getGUISystem().getSkin().buttonTexture);
    }
}

void GUIButton::mOnMouseEnter() {
    mBgImage->setTexture(getGUISystem().getSkin().hoveredButtonTexture);
}

void GUIButton::mOnMouseLeave() {
    mBgImage->setTexture(getGUISystem().getSkin().buttonTexture);
}

}
