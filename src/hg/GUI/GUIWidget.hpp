#pragma once
#include "../Core/WindowEvent.hpp"
#include "hd/Core/Delegate.hpp"
#include <glm/glm.hpp>

namespace hg {

enum class GUIHAlign {
    None,
    Left,
    Center,
    Right
};

enum class GUIVAlign {
    None,
    Top,
    Center,
    Bottom
};

class GUIWidget {
    friend class GUISystem;
public:
    virtual ~GUIWidget();

    virtual void onEvent(const WindowEvent &event);
    virtual void onFixedUpdate();
    virtual void onUpdate(float dt);

    template<typename T>
    T *createChild() {
        T *widget = new T();
        mChildren.push_back(widget);
        widget->mParent = this;
        return widget;
    }

    void setActive(bool active);
    void setHAlign(GUIHAlign align);
    void setVAlign(GUIVAlign align);
    void setAlign(GUIHAlign hAlign, GUIVAlign vAlign);
    void setPosition(int x, int y);
    void setPosition(const glm::ivec2 &pos);
    void setSize(int w, int h);
    void setSize(const glm::ivec2 &size);
    void setExpand(bool expand);

    bool isActive() const;
    GUIHAlign getHAlign() const;
    GUIVAlign getVAlign() const;
    bool isMouseHovered() const;
    glm::ivec2 getAbsolutePosition() const;
    const glm::ivec2 &getPosition() const;
    const glm::ivec2 &getSize() const;
    bool isExpand() const;

    hd::Delegate<> onMouseButtonPressed, onMouseButtonReleased;
    hd::Delegate<> onMouseEnter, onMouseLeave;

private:
    void mOnEvent(const WindowEvent &event);
    void mOnFixedUpdate();
    void mOnUpdate(float dt);
    void mApplyHAlign();
    void mApplyVAlign();

    GUIWidget *mParent = nullptr;
    std::vector<GUIWidget*> mChildren;
    bool mIsActive = true;

    bool mIsMouseHovered = false;
    GUIHAlign mHAlign = GUIHAlign::None;
    GUIVAlign mVAlign = GUIVAlign::None;
    glm::ivec2 mPos = glm::ivec2(0, 0);
    glm::ivec2 mSize = glm::ivec2(0, 0);
    bool mIsExpand = false;
};

}
