#pragma once
#include "../Scene/Node2D.hpp"
#include "hd/Core/Delegate.hpp"
#include <functional>

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

class GUIWidget : public Node2D {
    HG_OBJECT(GUIWidget, Node2D);
public:
    void onSaveLoad(hd::JSON &data, bool isLoad) override;
    void onEvent(const WindowEvent &event) override;
    void onFixedUpdate() override;

    void setHAlign(GUIHAlign align);
    void setVAlign(GUIVAlign align);
    void setAlign(GUIHAlign hAlign, GUIVAlign vAlign);

    GUIHAlign getHAlign() const;
    GUIVAlign getVAlign() const;
    bool isMouseHovered() const;

    hd::Delegate<> onMouseButtonPressed, onMouseButtonReleased;
    hd::Delegate<> onMouseEnter, onMouseLeave;

private:
    void mApplyHAlign();
    void mApplyVAlign();

    bool mIsMouseHovered = false;
    GUIHAlign mHAlign = GUIHAlign::None;
    GUIVAlign mVAlign = GUIVAlign::None;
};

}
