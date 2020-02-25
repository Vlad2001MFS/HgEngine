#pragma once
#include "GUIImage.hpp"
#include "GUILabel.hpp"

namespace hg2d {

class GUIButton : public GUIImage {
public:
    explicit GUIButton();
    virtual ~GUIButton() = default;

    void setText(const std::string &text);
    const std::string &getText() const;

private:
    void mOnMouseButtonPressed();
    void mOnMouseButtonReleased();
    void mOnMouseEnter();
    void mOnMouseLeave();

    GUILabel *mLabel;
};

}