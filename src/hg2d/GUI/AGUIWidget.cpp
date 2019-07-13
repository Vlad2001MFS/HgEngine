#include "AGUIWidget.hpp"
#include "../Core/Engine.hpp"

namespace hg2d {

AGUIWidget::AGUIWidget(Engine &engine) : mEngine(engine), mPosition(0, 0), mSize(0, 0), mAbsolutePosition(0, 0) {
    mParent = nullptr;
    mIsMouseHovered = false;
    mHAlign = GUIHAlign::None;
    mVAlign = GUIVAlign::None;
}

AGUIWidget::~AGUIWidget() {
    for (auto &child : mChildren) {
        if (child) {
            mDestroyChild(child);
        }
    }
}

void AGUIWidget::destroyChild(AGUIWidget *&widget) {
    if (!widget) {
        HD_LOG_WARNING("widget is nullptr");
    }
    else {
        auto it = std::find(mChildren.begin(), mChildren.end(), widget);
        if (it != mChildren.end()) {
            mDestroyChild(*it);
            widget = nullptr;
        }
        else {
            HD_LOG_WARNING("Failed to destroy GUIWidget. The widget wasn't registered at parent as child");
        }
    }
}

void AGUIWidget::_onInitialize() {
    onInitialize();
    for (auto &child : mChildren) {
        if (child) {
            child->_onUpdate();
        }
    }
}

void AGUIWidget::_onShutdown() {
    onShutdown();
    for (auto &child : mChildren) {
        if (child) {
            child->_onUpdate();
        }
    }
}

void AGUIWidget::_onEvent(const hd::WindowEvent &event) {
    if (event.type == hd::WindowEventType::MouseButton) {
        glm::ivec2 leftUp = getAbsolutePosition();
        glm::ivec2 rightDown = leftUp + getSize();
        if (event.mouseButton.x >= leftUp.x && event.mouseButton.x < rightDown.x &&
            event.mouseButton.y >= leftUp.y && event.mouseButton.y < rightDown.y) {
            if (event.mouseButton.state == hd::KeyState::Pressed) {
                onMouseButtonPressed();
            }
            else if (event.mouseButton.state == hd::KeyState::Released) {
                onMouseButtonReleased();
            }
        }
    }
    else if (event.type == hd::WindowEventType::MouseMove) {
        glm::ivec2 leftUp = getAbsolutePosition();
        glm::ivec2 rightDown = leftUp + getSize();
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
        if (child) {
            child->_onEvent(event);
        }
    }
}

void AGUIWidget::_onFixedUpdate() {
    mApplyHAlign();
    mApplyVAlign();

    onFixedUpdate();
    for (auto &child : mChildren) {
        if (child) {
            child->_onFixedUpdate();
        }
    }
}

void AGUIWidget::_onUpdate() {
    onUpdate();
    for (auto &child : mChildren) {
        if (child) {
            child->_onUpdate();
        }
    }
}

void AGUIWidget::_onDraw() {
    onDraw();
    for (auto &child : mChildren) {
        if (child) {
            child->_onDraw();
        }
    }
}

void AGUIWidget::setPosition(int x, int y) {
    mPosition.x = x;
    mPosition.y = y;

}
void AGUIWidget::setPosition(const glm::ivec2 &position) {
    mPosition = position;
}

void AGUIWidget::setSize(int x, int y) {
    mSize.x = x;
    mSize.y = y;
}

void AGUIWidget::setSize(const glm::ivec2 &size) {
    mSize = size;
}

void AGUIWidget::setHAlign(GUIHAlign align) {
    mHAlign = align;
}

void AGUIWidget::setVAlign(GUIVAlign align) {
    mVAlign = align;
}

void AGUIWidget::setAlign(GUIHAlign hAlign, GUIVAlign vAlign) {
    setHAlign(hAlign);
    setVAlign(vAlign);
}

AGUIWidget *AGUIWidget::getParent() const {
    return mParent;
}

const std::vector<AGUIWidget*> &AGUIWidget::getChildren() const {
    return mChildren;
}

const glm::ivec2 &AGUIWidget::getPosition() const {
    return mPosition;
}

const glm::ivec2 &AGUIWidget::getAbsolutePosition() const {
    mAbsolutePosition = mPosition;
    if (mParent) {
        mAbsolutePosition = mAbsolutePosition + mParent->getAbsolutePosition();
    }
    return mAbsolutePosition;
}

const glm::ivec2 &AGUIWidget::getSize() const {
    return mSize;
}

GUIHAlign AGUIWidget::getHAlign() const {
    return mHAlign;
}

GUIVAlign AGUIWidget::getVAlign() const {
    return mVAlign;
}

bool AGUIWidget::isMouseHovered() const {
    return mIsMouseHovered;
}

void AGUIWidget::mDestroyChild(AGUIWidget*& widget) {
    widget->_onShutdown();
    HD_DELETE(widget);
}

Engine& AGUIWidget::getEngine() const {
    return mEngine;
}

void AGUIWidget::mAddChild(AGUIWidget *widget) {
    widget->mParent = this;
    auto empty = std::find(mChildren.begin(), mChildren.end(), nullptr);
    if (empty != mChildren.end()) {
        *empty = widget;
    }
    else {
        mChildren.push_back(widget);
    }
    widget->_onInitialize();
}

void AGUIWidget::mApplyHAlign() {
    const size_t childrenCount = getChildren().size();
    for (size_t i = 0; i < childrenCount; i++) {
        AGUIWidget *child = getChildren()[i];
        AGUIWidget *invChild = getChildren()[childrenCount - i - 1];
        if (child->getHAlign() == GUIHAlign::Left) {
            child->setPosition(getEngine().getCreateInfo().gui.alignSpaceX, child->getPosition().y);
        }
        if (invChild->getHAlign() == GUIHAlign::Right) {
            int x = getSize().x - invChild->getSize().x - getEngine().getCreateInfo().gui.alignSpaceX;
            invChild->setPosition(x, invChild->getPosition().y);
        }
        if (child->getHAlign() == GUIHAlign::Center) {
            int x = getSize().x / 2 - child->getSize().x / 2;
            child->setPosition(x, child->getPosition().y);
        }
    }
}

void AGUIWidget::mApplyVAlign() {
    int topY[4] = { // 0 - none, 1 - left, 2 - center, 3 - right
        getEngine().getCreateInfo().gui.alignSpaceY,
        getEngine().getCreateInfo().gui.alignSpaceY,
        getEngine().getCreateInfo().gui.alignSpaceY,
        getEngine().getCreateInfo().gui.alignSpaceY,
    };
    int bottomY[4] = { // 0 - none, 1 - left, 2 - center, 3 - right
        getSize().y - getEngine().getCreateInfo().gui.alignSpaceY,
        getSize().y - getEngine().getCreateInfo().gui.alignSpaceY,
        getSize().y - getEngine().getCreateInfo().gui.alignSpaceY,
        getSize().y - getEngine().getCreateInfo().gui.alignSpaceY,
    };
    int centerHeight = 0;
    const size_t childrenCount = getChildren().size();
    for (size_t i = 0; i < childrenCount; i++) {
        AGUIWidget *child = getChildren()[i];
        AGUIWidget *invChild = getChildren()[childrenCount - i - 1];
        if (child->getVAlign() == GUIVAlign::Top) {
            child->setPosition(child->getPosition().x, topY[static_cast<size_t>(child->getHAlign())]);
            topY[static_cast<size_t>(child->getHAlign())] += child->getSize().y + getEngine().getCreateInfo().gui.alignSpaceY;
        }
        if (invChild->getVAlign() == GUIVAlign::Bottom) {
            invChild->setPosition(invChild->getPosition().x, bottomY[static_cast<size_t>(invChild->getHAlign())] - invChild->getSize().y);
            bottomY[static_cast<size_t>(invChild->getHAlign())] -= invChild->getSize().y + getEngine().getCreateInfo().gui.alignSpaceY;
        }
        if (child->getVAlign() == GUIVAlign::Center) {
            centerHeight += child->getSize().y + getEngine().getCreateInfo().gui.alignSpaceY;
        }
    }

    centerHeight -= getEngine().getCreateInfo().gui.alignSpaceY;
    int centerY = getSize().y / 2 - centerHeight / 2;
    for (size_t i = 0; i < childrenCount; i++) {
        AGUIWidget *child = getChildren()[i];
        if (child->getVAlign() == GUIVAlign::Center) {
            child->setPosition(child->getPosition().x, centerY);
            centerY += child->getSize().y + getEngine().getCreateInfo().gui.alignSpaceY;
        }
    }
}

}
