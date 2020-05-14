#include "GUIWidget.hpp"
#include "GUISystem.hpp"
#include "../../magic_enum/magic_enum.hpp"

namespace hg {

void GUIWidget::onSaveLoad(hd::JSON &data, bool isLoad) {
    BaseClassName::onSaveLoad(data, isLoad);

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
}

void GUIWidget::onEvent(const WindowEvent &event) {
    BaseClassName::onEvent(event);

    glm::ivec2 leftUp = getAbsolutePosition();
    glm::ivec2 rightDown = leftUp + glm::ivec2(getSize());
    if (event.type == WindowEventType::MouseButton) {
        if (event.mouseButton.x >= leftUp.x && event.mouseButton.x < rightDown.x &&
            event.mouseButton.y >= leftUp.y && event.mouseButton.y < rightDown.y) {
            if (event.mouseButton.state == KeyState::Pressed) {
                onMouseButtonPressed();
            }
            else if (event.mouseButton.state == KeyState::Released) {
                onMouseButtonReleased();
            }
        }
    }
    else if (event.type == WindowEventType::MouseMove) {
        if (event.mouseMove.x >= leftUp.x && event.mouseMove.x < rightDown.x &&
            event.mouseMove.y >= leftUp.y && event.mouseMove.y < rightDown.y) {
            if (!mIsMouseHovered) {
                mIsMouseHovered = true;
                onMouseEnter();
            }
        }
        else {
            if (mIsMouseHovered) {
                mIsMouseHovered = false;
                onMouseLeave();
            }
        }
    }
}

void GUIWidget::onFixedUpdate() {
    BaseClassName::onFixedUpdate();

    mApplyHAlign();
    mApplyVAlign();
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

GUIHAlign GUIWidget::getHAlign() const {
    return mHAlign;
}

GUIVAlign GUIWidget::getVAlign() const {
    return mVAlign;
}

bool GUIWidget::isMouseHovered() const {
    return mIsMouseHovered;
}

void GUIWidget::mApplyHAlign() {
    const size_t childrenCount = getChildren().size();
    for (size_t i = 0; i < childrenCount; i++) {
        GUIWidget *child = getChildren()[i]->as<GUIWidget>();
        GUIWidget *invChild = getChildren()[childrenCount - i - 1]->as<GUIWidget>();
        if (child->isActive() && child->getHAlign() == GUIHAlign::Left) {
            child->setPosition(glm::vec2(getGUISystem().getSkin().alignSpaceX, getGUISystem().getSkin().alignSpaceY));
        }
        if (invChild->isActive() && invChild->getHAlign() == GUIHAlign::Right) {
            int x = getSize().x - invChild->getSize().x - getGUISystem().getSkin().alignSpaceX;
            invChild->setPosition(glm::vec2(x, invChild->getPosition().y));
        }
        if (child->isActive() && child->getHAlign() == GUIHAlign::Center) {
            int x = getSize().x / 2 - child->getSize().x / 2;
            child->setPosition(glm::vec2(x, child->getPosition().y));
        }
    }
}

void GUIWidget::mApplyVAlign() {
    int topY[4] = { // 0 - none, 1 - left, 2 - center, 3 - right
        getGUISystem().getSkin().alignSpaceY,
        getGUISystem().getSkin().alignSpaceY,
        getGUISystem().getSkin().alignSpaceY,
        getGUISystem().getSkin().alignSpaceY,
    };
    int bottomY[4] = { // 0 - none, 1 - left, 2 - center, 3 - right
        static_cast<int>(getSize().y) - getGUISystem().getSkin().alignSpaceY,
        static_cast<int>(getSize().y) - getGUISystem().getSkin().alignSpaceY,
        static_cast<int>(getSize().y) - getGUISystem().getSkin().alignSpaceY,
        static_cast<int>(getSize().y) - getGUISystem().getSkin().alignSpaceY,
    };
    int centerHeight = 0;
    const size_t childrenCount = getChildren().size();
    for (size_t i = 0; i < childrenCount; i++) {
        GUIWidget *child = getChildren()[i]->as<GUIWidget>();
        GUIWidget *invChild = getChildren()[childrenCount - i - 1]->as<GUIWidget>();
        if (child->isActive() && child->getVAlign() == GUIVAlign::Top) {
            child->setPosition(glm::vec2(child->getPosition().x, topY[static_cast<size_t>(child->getHAlign())]));
            topY[static_cast<size_t>(child->getHAlign())] += child->getSize().y + getGUISystem().getSkin().alignSpaceY;
        }
        if (invChild->isActive() && invChild->getVAlign() == GUIVAlign::Bottom) {
            invChild->setPosition(glm::vec2(invChild->getPosition().x, bottomY[static_cast<size_t>(invChild->getHAlign())] - invChild->getSize().y));
            bottomY[static_cast<size_t>(invChild->getHAlign())] -= invChild->getSize().y + getGUISystem().getSkin().alignSpaceY;
        }
        if (child->isActive() && child->getVAlign() == GUIVAlign::Center) {
            centerHeight += child->getSize().y + getGUISystem().getSkin().alignSpaceY;
        }
    }

    centerHeight -= getGUISystem().getSkin().alignSpaceY;
    int centerY = getSize().y / 2 - centerHeight / 2;
    for (size_t i = 0; i < childrenCount; i++) {
        GUIWidget *child = getChildren()[i]->as<GUIWidget>();
        if (child->isActive() && child->getVAlign() == GUIVAlign::Center) {
            child->setPosition(glm::vec2(child->getPosition().x, centerY));
            centerY += child->getSize().y + getGUISystem().getSkin().alignSpaceY;
        }
    }
}

}
