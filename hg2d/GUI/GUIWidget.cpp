#include "GUIWidget.hpp"
#include "GUISystem.hpp"

namespace hg2d {

HG2D_REGISTER_OBJECT(GUIWidget);

GUIWidget::GUIWidget() {
    mIsMouseHovered = false;
    mHAlign = GUIHAlign::None;
    mVAlign = GUIVAlign::None;
}

void GUIWidget::onEvent(const hd::WindowEvent &event) {
    if (event.type == hd::WindowEventType::MouseButton) {
        glm::vec2 leftUp = getAbsolutePosition();
        glm::vec2 rightDown = leftUp + getSize();
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
        glm::vec2 rightDown = leftUp + getSize();
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

    Node::onEvent(event);
}

void GUIWidget::onFixedUpdate() {
    mApplyHAlign();
    mApplyVAlign();

    Node::onFixedUpdate();
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
        GUIWidget *child = static_cast<GUIWidget*>(getChildren()[i]);
        GUIWidget *invChild = static_cast<GUIWidget*>(getChildren()[childrenCount - i - 1]);
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
        getSize().y - getGUISystem().getSkin().alignSpaceY,
        getSize().y - getGUISystem().getSkin().alignSpaceY,
        getSize().y - getGUISystem().getSkin().alignSpaceY,
        getSize().y - getGUISystem().getSkin().alignSpaceY,
    };
    int centerHeight = 0;
    const size_t childrenCount = getChildren().size();
    for (size_t i = 0; i < childrenCount; i++) {
        GUIWidget *child = static_cast<GUIWidget*>(getChildren()[i]);
        GUIWidget *invChild = static_cast<GUIWidget*>(getChildren()[childrenCount - i - 1]);
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
        GUIWidget *child = static_cast<GUIWidget*>(getChildren()[i]);
        if (child->isActive() && child->getVAlign() == GUIVAlign::Center) {
            child->setPosition(glm::vec2(child->getPosition().x, centerY));
            centerY += child->getSize().y + getGUISystem().getSkin().alignSpaceY;
        }
    }
}

}