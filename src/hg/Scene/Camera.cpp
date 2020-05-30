#include "Camera.hpp"
#include "GameObject.hpp"
#include <glm/ext.hpp>

namespace hg {

void Camera::onSaveLoad(hd::JSON &data, bool isLoad) {
    hd::JSON &distance = data["distance"];
    if (isLoad) {
        setDistance(distance);
    }
    else {
        distance = getDistance();
    }
}

void Camera::move(float offset) {
    setDistance(getDistance() + offset);
}

void Camera::setDistance(float distance) {
    mDistance = glm::max(distance, 1.0f);
}

float Camera::getDistance() const {
    return mDistance;
}

}
