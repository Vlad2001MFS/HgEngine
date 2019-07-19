#include "TransformComponent.hpp"

namespace hg2d {

TransformComponent::TransformComponent() : mPos(0, 0), mSize(0, 0) {
    mAngle = 0.0f;
}

void TransformComponent::translate(const glm::vec2 &offset) {
    mPos += offset;
}

void TransformComponent::scale(const glm::vec2 &scale) {
    mSize += scale;
}

void TransformComponent::rotate(float angle) {
    mAngle += angle;
}

void TransformComponent::setPosition(const glm::vec2 &pos) {
    mPos = pos;
}

void TransformComponent::setSize(const glm::vec2 &size) {
    mSize = size;
}

void TransformComponent::setAngle(float angle) {
    mAngle = angle;
}

const glm::vec2 &TransformComponent::getPosition() const {
    return mPos;
}

const glm::vec2 &TransformComponent::getSize() const {
    return mSize;
}

float TransformComponent::getAngle() const {
    return mAngle;
}

}
