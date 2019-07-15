#pragma once
#include "hd/Core/hdDelegate.hpp"
#include "hd/System/hdWindowEvent.hpp"
#include "3rd/include/glm/glm.hpp"

namespace hg2d {

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

class Engine;

class AGUIWidget {
public:
    explicit AGUIWidget(Engine &engine);
    virtual ~AGUIWidget();

    template<typename T, typename ...Args>
    T *createChild(Args &&...args) { T *widget = new T(mEngine, args...); mAddChild(widget); return widget; }
    void destroyChild(AGUIWidget *&widget);
    
    virtual void onInitialize() {}
    virtual void onShutdown() {}
    virtual void onEvent(const hd::WindowEvent &event) {}
    virtual void onFixedUpdate() {}
    virtual void onUpdate() {}
    virtual void onDraw() {}

    void _onInitialize();
    void _onShutdown();
    void _onEvent(const hd::WindowEvent &event);
    void _onFixedUpdate();
    void _onUpdate();
    void _onDraw();

    void setPosition(int x, int y);
    void setPosition(const glm::ivec2 &position);
    void setSize(int x, int y);
    void setSize(const glm::ivec2 &size);
    void setHAlign(GUIHAlign align);
    void setVAlign(GUIVAlign align);
    void setAlign(GUIHAlign hAlign, GUIVAlign vAlign);

    AGUIWidget *getParent() const;
    const std::vector<AGUIWidget*> &getChildren() const;
    const glm::ivec2 &getPosition() const;
    const glm::ivec2 &getAbsolutePosition() const;
    const glm::ivec2 &getSize() const;
    GUIHAlign getHAlign() const;
    GUIVAlign getVAlign() const;
    bool isMouseHovered() const;

    hd::Delegate<> onMouseButtonPressed, onMouseButtonReleased;
    hd::Delegate<> onMouseEnter, onMouseLeave;

protected:
    Engine &mEngine;

private:
    void mAddChild(AGUIWidget *widget);
    void mDestroyChild(AGUIWidget *&widget);
    void mApplyHAlign();
    void mApplyVAlign();

    AGUIWidget *mParent;
    std::vector<AGUIWidget*> mChildren;
    glm::ivec2 mPosition, mSize;
    mutable glm::ivec2 mAbsolutePosition;
    bool mIsMouseHovered;
    GUIHAlign mHAlign;
    GUIVAlign mVAlign;
};

}
