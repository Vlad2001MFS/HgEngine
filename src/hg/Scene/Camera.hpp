#pragma once
#include "Component.hpp"

namespace hg {

class Camera : public Component {
public:
    void onSaveLoad(hd::JSON &data, bool isLoad) override;

    void move(float offset);

    void setDistance(float distance);
    float getDistance() const;

private:
    float mDistance = 1.0f;
};

}
