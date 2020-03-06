#include "GUIWidget.hpp"
#include "GUISystem.hpp"
#include "../Core/Engine.hpp"
#include "magic_enum/magic_enum.hpp"

namespace hg {

HG_REGISTER_OBJECT(GUIWidget);

GUIWidget::GUIWidget() {
    mIsMouseHovered = false;
    mHAlign = GUIHAlign::None;
    mVAlign = GUIVAlign::None;
    setSize(1, 1);
}

void GUIWidget::onSaveLoad(JSONObject &data, bool isLoad) {
    if (isLoad) {
        auto hAlign = magic_enum::enum_cast<GUIHAlign>(data["hAlign"].get<std::string>());
        mHAlign = hAlign.value_or(GUIHAlign::None);
        if (!hAlign) {
            HD_LOG_WARNING("Invalid value '{}' of GUIHAlign enum", data["hAlign"].get<std::string>());
        }

        auto vAlign = magic_enum::enum_cast<GUIVAlign>(data["vAlign"].get<std::string>());
        mVAlign = vAlign.value_or(GUIVAlign::None);
        if (!vAlign) {
            HD_LOG_WARNING("Invalid value '{}' of GUIVAlign enum", data["vAlign"].get<std::string>());
        }
    }
    else {
        data["hAlign"] = magic_enum::enum_name(mHAlign);
        data["vAlign"] = magic_enum::enum_name(mVAlign);
    }

    BaseClassName::onSaveLoad(data, isLoad);
}

void GUIWidget::onEvent(const hd::WindowEvent &event) {
    if (event.type == hd::WindowEventType::MouseButton) {
        glm::vec2 leftUp = getAbsolutePosition();
        glm::vec2 rightDown = leftUp + getAbsoluteSize();
        if (event.mouseButton.x >= leftUp.x && event.mouseButton.x < rightDown.x &&
            event.mouseButton.y >= leftUp.y && event.mouseButton.y < rightDown.y) {
            if (event.mouseButton.state == hd::KeyState::Pressed) {
                if (onMouseButtonPressed) {
                    onMouseButtonPressed();
                }
            }
            else if (event.mouseButton.state == hd::KeyState::Released) {
                if (onMouseButtonReleased) {
                    onMouseButtonReleased();
                }
            }
        }
    }
    else if (event.type == hd::WindowEventType::MouseMove) {
        glm::vec2 leftUp = getAbsolutePosition();
        glm::vec2 rightDown = leftUp + getAbsoluteSize();
        if (event.mouseMove.x >= leftUp.x && event.mouseMove.x < rightDown.x &&
            event.mouseMove.y >= leftUp.y && event.mouseMove.y < rightDown.y) {
            if (!mIsMouseHovered) {
                mIsMouseHovered = true;
                if (onMouseEnter) {
                    onMouseEnter();
                }
            }
        }
        else {
            if (mIsMouseHovered) {
                mIsMouseHovered = false;
                if (onMouseLeave) {
                    onMouseLeave();
                }
            }
        }
    }

    BaseClassName::onEvent(event);
}

void GUIWidget::onFixedUpdate() {
    mApplyHAlign();
    mApplyVAlign();

    BaseClassName::onFixedUpdate();
}

void GUIWidget::setHAlign(GUIHAlign align) {
    mHAlign = align;
}

void GUIWidget::setVAlign(GUIVAlign align) {
    mVAlign = align;
}

void GUIWidget::setAlign(GUIHAlign hAlign, GUIVAlign vAlign) {
    setHAlign(hAlign);
    setVAlign(vAlign);
}

void GUIWidget::setAbsoluteSize(const glm::vec2 &size) {
    if (getParent() && getParent()->isInstanceOf<GUIWidget>()) {
        glm::vec2 absSize = getParent()->as<GUIWidget>()->getAbsoluteSize();
        if (absSize.x != 0 && absSize.y != 0) {
            setSize(size / getParent()->as<GUIWidget>()->getAbsoluteSize());
        }
        else {
            setSize(0, 0);
        }
    }
    else {
        setSize(size / glm::vec2(getEngine().getWindow().getSize()));
    }
}

GUIHAlign GUIWidget::getHAlign() const {
    return mHAlign;
}

GUIVAlign GUIWidget::getVAlign() const {
    return mVAlign;
}

bool GUIWidget::isMouseHovered() const {
    return mIsMouseHovered;
}

glm::vec2 GUIWidget::getAbsoluteSize() const {
    if (getParent() && getParent()->isInstanceOf<GUIWidget>()) {
        return getSize()*getParent()->as<GUIWidget>()->getAbsoluteSize();
    }
    else {
        return getSize()*glm::vec2(getEngine().getWindow().getSize());
    }
}

void GUIWidget::mApplyHAlign() {
    const size_t childrenCount = getChildren().size();
    for (size_t i = 0; i < childrenCount; i++) {
        GUIWidget *child = static_cast<GUIWidget*>(getChildren()[i]);
        GUIWidget *invChild = static_cast<GUIWidget*>(getChildren()[childrenCount - i - 1]);
        if (child->isActive() && child->getHAlign() == GUIHAlign::Left) {
            child->setPosition(glm::vec2(getGUISystem().getSkin().alignSpaceX, getGUISystem().getSkin().alignSpaceY));
        }
        if (invChild->isActive() && invChild->getHAlign() == GUIHAlign::Right) {
            float x = getAbsoluteSize().x - invChild->getAbsoluteSize().x - getGUISystem().getSkin().alignSpaceX;
            invChild->setPosition(glm::vec2(x, invChild->getPosition().y));
        }
        if (child->isActive() && child->getHAlign() == GUIHAlign::Center) {
            float x = getAbsoluteSize().x / 2 - child->getAbsoluteSize().x / 2;
            child->setPosition(glm::vec2(x, child->getPosition().y));
        }
    }
}

void GUIWidget::mApplyVAlign() {
    float topY[4] = { // 0 - none, 1 - left, 2 - center, 3 - right
        getGUISystem().getSkin().alignSpaceY,
        getGUISystem().getSkin().alignSpaceY,
        getGUISystem().getSkin().alignSpaceY,
        getGUISystem().getSkin().alignSpaceY,
    };
    float bottomY[4] = { // 0 - none, 1 - left, 2 - center, 3 - right
        getAbsoluteSize().y - getGUISystem().getSkin().alignSpaceY,
        getAbsoluteSize().y - getGUISystem().getSkin().alignSpaceY,
        getAbsoluteSize().y - getGUISystem().getSkin().alignSpaceY,
        getAbsoluteSize().y - getGUISystem().getSkin().alignSpaceY,
    };
    float centerHeight = 0;
    const size_t childrenCount = getChildren().size();
    for (size_t i = 0; i < childrenCount; i++) {
        GUIWidget *child = static_cast<GUIWidget*>(getChildren()[i]);
        GUIWidget *invChild = static_cast<GUIWidget*>(getChildren()[childrenCount - i - 1]);
        if (child->isActive() && child->getVAlign() == GUIVAlign::Top) {
            child->setPosition(glm::vec2(child->getPosition().x, topY[static_cast<size_t>(child->getHAlign())]));
            topY[static_cast<size_t>(child->getHAlign())] += child->getAbsoluteSize().y + getGUISystem().getSkin().alignSpaceY;
        }
        if (invChild->isActive() && invChild->getVAlign() == GUIVAlign::Bottom) {
            invChild->setPosition(glm::vec2(invChild->getPosition().x, bottomY[static_cast<size_t>(invChild->getHAlign())] - invChild->getAbsoluteSize().y));
            bottomY[static_cast<size_t>(invChild->getHAlign())] -= invChild->getAbsoluteSize().y + getGUISystem().getSkin().alignSpaceY;
        }
        if (child->isActive() && child->getVAlign() == GUIVAlign::Center) {
            centerHeight += child->getAbsoluteSize().y + getGUISystem().getSkin().alignSpaceY;
        }
    }

    centerHeight -= getGUISystem().getSkin().alignSpaceY;
    float centerY = getAbsoluteSize().y / 2 - centerHeight / 2;
    for (size_t i = 0; i < childrenCount; i++) {
        GUIWidget *child = static_cast<GUIWidget*>(getChildren()[i]);
        if (child->isActive() && child->getVAlign() == GUIVAlign::Center) {
            child->setPosition(glm::vec2(child->getPosition().x, centerY));
            centerY += child->getAbsoluteSize().y + getGUISystem().getSkin().alignSpaceY;
        }
    }
}

}
