#pragma once
#include "GUIWidget.hpp"

namespace hg {

class GUIImage;
class GUILabel;

class GUIButton : public GUIWidget {
public:
    GUIButton();

    void setText(const std::string &text);
    const std::string &getText() const;

private:
    void mOnMouseButtonPressed();
    void mOnMouseButtonReleased();
    void mOnMouseEnter();
    void mOnMouseLeave();

    GUIImage *mBgImage;
    GUILabel *mTextLabel;
};

}
