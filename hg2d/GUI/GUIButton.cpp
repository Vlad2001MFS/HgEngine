#include "GUIButton.hpp"
#include "../GUI/GUISystem.hpp"

namespace hg2d {

GUIButton::GUIButton(Engine &engine) : GUIImage(engine) {
    setTexture(mGUISystem.getSkin().buttonTexture);
    mLabel = createChild<GUILabel>();
    mLabel->setAlign(GUIHAlign::Center, GUIVAlign::Center);
    onMouseButtonPressed.connect(std::bind(&GUIButton::mOnMouseButtonPressed, this));
    onMouseButtonReleased.connect(std::bind(&GUIButton::mOnMouseButtonReleased, this));
    onMouseEnter.connect(std::bind(&GUIButton::mOnMouseEnter, this));
    onMouseLeave.connect(std::bind(&GUIButton::mOnMouseLeave, this));
}

void GUIButton::setText(const std::string &text) {
    mLabel->setText(text);
}

const std::string& GUIButton::getText() const {
    return mLabel->getText();
}

void GUIButton::mOnMouseButtonPressed() {
    setTexture(mGUISystem.getSkin().clickedButtonTexture);
}

void GUIButton::mOnMouseButtonReleased() {
    if (isMouseHovered()) {
        setTexture(mGUISystem.getSkin().hoveredButtonTexture);
    }
    else {
        setTexture(mGUISystem.getSkin().buttonTexture);
    }
}

void GUIButton::mOnMouseEnter() {
    setTexture(mGUISystem.getSkin().hoveredButtonTexture);
}

void GUIButton::mOnMouseLeave() {
    setTexture(mGUISystem.getSkin().buttonTexture);
}

}
