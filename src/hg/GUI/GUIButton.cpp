#include "GUIButton.hpp"
#include "../GUI/GUISystem.hpp"

namespace hg {

GUIButton::GUIButton() {
    setTexture(getGUISystem().getSkin().buttonTexture);
    mTextLabel = createChild<GUILabel>("");
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
    setTexture(getGUISystem().getSkin().clickedButtonTexture);
}

void GUIButton::mOnMouseButtonReleased() {
    if (isMouseHovered()) {
        setTexture(getGUISystem().getSkin().hoveredButtonTexture);
    }
    else {
        setTexture(getGUISystem().getSkin().buttonTexture);
    }
}

void GUIButton::mOnMouseEnter() {
    setTexture(getGUISystem().getSkin().hoveredButtonTexture);
}

void GUIButton::mOnMouseLeave() {
    setTexture(getGUISystem().getSkin().buttonTexture);
}

}
