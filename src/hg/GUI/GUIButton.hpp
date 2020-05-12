#pragma once
#include "GUIImage.hpp"
#include "GUILabel.hpp"

namespace hg {

class GUIButton : public GUIImage {
    HG_OBJECT(GUIButton, GUIImage);
public:
    GUIButton();

    void setText(const std::string &text);
    const std::string &getText() const;

private:
    void mOnMouseButtonPressed();
    void mOnMouseButtonReleased();
    void mOnMouseEnter();
    void mOnMouseLeave();

    GUILabel *mTextLabel;
};

}
