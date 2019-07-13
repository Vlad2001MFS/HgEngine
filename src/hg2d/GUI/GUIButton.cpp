#include "GUIButton.hpp"
#include "../Core/Engine.hpp"

namespace hg2d {

GUIButton::GUIButton(Engine &engine) : GUIImage(engine) {
    setTexture(getEngine().getGUISystem().getSkin().buttonTexture);
    mLabel = createChild<GUILabel>();
    mLabel->setAlign(GUIHAlign::Center, GUIVAlign::Center);
    onMouseButtonPressed.connect(std::bind(&GUIButton::mOnMouseButtonPressed, this));
    onMouseButtonReleased.connect(std::bind(&GUIButton::mOnMouseButtonReleased, this));
    onMouseEnter.connect(std::bind(&GUIButton::mOnMouseEnter, this));
    onMouseLeave.connect(std::bind(&GUIButton::mOnMouseLeave, this));
}

GUIButton::~GUIButton() {
}

void GUIButton::setText(const std::string &text) {
    mLabel->setText(text);
}

const std::string& GUIButton::getText() const {
    return mLabel->getText();
}

void GUIButton::mOnMouseButtonPressed() {
    setTexture(getEngine().getGUISystem().getSkin().clickedButtonTexture);
}

void GUIButton::mOnMouseButtonReleased() {
    if (isMouseHovered()) {
        setTexture(getEngine().getGUISystem().getSkin().hoveredButtonTexture);
    }
    else {
        setTexture(getEngine().getGUISystem().getSkin().buttonTexture);
    }
}

void GUIButton::mOnMouseEnter() {
    setTexture(getEngine().getGUISystem().getSkin().hoveredButtonTexture);
}

void GUIButton::mOnMouseLeave() {
    setTexture(getEngine().getGUISystem().getSkin().buttonTexture);
}

}