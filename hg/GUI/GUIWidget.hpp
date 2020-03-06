#pragma once
#include "../Core/Node.hpp"
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

class GUIWidget : public Node {
    HG_OBJECT(GUIWidget, Node);
public:
    GUIWidget();

    virtual void onSaveLoad(JSONObject &data, bool isLoad) override;
    virtual void onEvent(const hd::WindowEvent &event) override;
    virtual void onFixedUpdate() override;

    void setHAlign(GUIHAlign align);
    void setVAlign(GUIVAlign align);
    void setAlign(GUIHAlign hAlign, GUIVAlign vAlign);
    void setAbsoluteSize(const glm::vec2 &size);

    GUIHAlign getHAlign() const;
    GUIVAlign getVAlign() const;
    bool isMouseHovered() const;
    glm::vec2 getAbsoluteSize() const;

    std::function<void()> onMouseButtonPressed, onMouseButtonReleased;
    std::function<void()> onMouseEnter, onMouseLeave;

private:
    void mApplyHAlign();
    void mApplyVAlign();

    bool mIsMouseHovered;
    GUIHAlign mHAlign;
    GUIVAlign mVAlign;
};

}
