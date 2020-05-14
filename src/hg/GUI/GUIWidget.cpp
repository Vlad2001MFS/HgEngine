#include "GUIWidget.hpp"
#include "GUISystem.hpp"

namespace hg {

GUIWidget::~GUIWidget() {
    for (auto &child : mChildren) {
        HD_DELETE(child);
    }
}

void GUIWidget::onEvent(const WindowEvent &event) {
}

void GUIWidget::onFixedUpdate() {
}

void GUIWidget::onUpdate(float dt) {
}

bool GUIWidget::isActive() const {
    return mIsActive;
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

void GUIWidget::setPosition(int x, int y) {
    mPos.x = x;
    mPos.y = y;
}

void GUIWidget::setPosition(const glm::ivec2 &pos) {
    setPosition(pos.x, pos.y);
}

void GUIWidget::setSize(int w, int h) {
    mSize.x = w;
    mSize.y = h;
}

void GUIWidget::setSize(const glm::ivec2 &size) {
    setSize(size.x, size.y);
}

void GUIWidget::setExpand(bool expand) {
    mIsExpand = expand;
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

glm::ivec2 GUIWidget::getAbsolutePosition() const {
    glm::ivec2 pos = glm::ivec2(0, 0);

    const GUIWidget *widget = this;
    while (widget) {
        pos += widget->getPosition();
        widget = widget->mParent;
    }

    return pos;
}

const glm::ivec2 &GUIWidget::getPosition() const {
    return mPos;
}

const glm::ivec2 &GUIWidget::getSize() const {
    return mSize;
}

bool GUIWidget::isExpand() const {
    return mIsExpand;
}

void GUIWidget::mOnEvent(const WindowEvent &event) {
    if (!isActive()) {
        return;
    }

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

    onEvent(event);
    for (auto &child : mChildren) {
        child->mOnEvent(event);
    }
}

void GUIWidget::mOnFixedUpdate() {
    if (!isActive()) {
        return;
    }

    if (mIsExpand && mParent) {
        setSize(mParent->getSize());
    }
    else {
        mApplyHAlign();
        mApplyVAlign();
    }

    onFixedUpdate();
    for (auto &child : mChildren) {
        child->mOnFixedUpdate();
    }
}

void GUIWidget::mOnUpdate(float dt) {
    if (!isActive()) {
        return;
    }

    onUpdate(dt);
    for (auto &child : mChildren) {
        child->mOnUpdate(dt);
    }
}

void GUIWidget::mApplyHAlign() {
    const size_t childrenCount = mChildren.size();
    for (size_t i = 0; i < childrenCount; i++) {
        GUIWidget *child = mChildren[i];
        GUIWidget *invChild = mChildren[childrenCount - i - 1];
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
    const size_t childrenCount = mChildren.size();
    for (size_t i = 0; i < childrenCount; i++) {
        GUIWidget *child = mChildren[i];
        GUIWidget *invChild = mChildren[childrenCount - i - 1];
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
        GUIWidget *child = mChildren[i];
        if (child->isActive() && child->getVAlign() == GUIVAlign::Center) {
            child->setPosition(glm::vec2(child->getPosition().x, centerY));
            centerY += child->getSize().y + getGUISystem().getSkin().alignSpaceY;
        }
    }
}

}
