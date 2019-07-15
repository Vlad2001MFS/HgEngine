#include "TransformSystem.hpp"
#include "../Core/Engine.hpp"

namespace hg2d {

TransformComponent::TransformComponent() : mPos(0, 0), mAbsolutePos(0, 0), mSize(0, 0) {
    mParent = nullptr;
    mAngle = 0.0f;
    mIsDirty = true;
}

void TransformComponent::updateAbsolutePosition() {
    if (mIsDirty) {
        mIsDirty = false;
        for (auto &child : mChildren) {
            child->mIsDirty = true;
        }
        if (getParent()) {
            float parentAngle = getParent()->getAngle();
            glm::vec2 pos = glm::vec2(
                getPosition().x*std::cos(parentAngle) - getPosition().y*std::sin(parentAngle),
                getPosition().x*std::sin(parentAngle) + getPosition().y*std::cos(parentAngle)
            );
            mAbsolutePos = getParent()->getAbsolutePosition() + pos;
        }
        else {
            mAbsolutePos = mPos;
        }
    }
}

void TransformComponent::translate(const glm::vec2 &offset) {
    mPos += offset;
    mIsDirty = true;
}

void TransformComponent::scale(const glm::vec2 &scale) {
    mSize += scale;
    mIsDirty = true;
}

void TransformComponent::rotate(float angle) {
    mAngle += angle;
    mIsDirty = true;
}

void TransformComponent::setParent(TransformComponent *parent) {
    if (parent != mParent) {
        if (parent) {
            if (mParent) {
                auto it = std::find(mParent->mChildren.begin(), mParent->mChildren.end(), this);
                if (it != mParent->mChildren.end()) {
                    *it = nullptr;
                }
            }
            mParent = parent;
            auto it = std::find(mParent->mChildren.begin(), mParent->mChildren.end(), nullptr);
            if (it != mParent->mChildren.end()) {
                *it = this;
            }
            else {
                parent->mChildren.push_back(this);
            }
        }
        else if (mParent) {
            auto it = std::find(mParent->mChildren.begin(), mParent->mChildren.end(), this);
            if (it != mParent->mChildren.end()) {
                *it = nullptr;
            }
            mParent = nullptr;
        }
        mIsDirty = true;
    }
}

void TransformComponent::setPosition(const glm::vec2 &pos) {
    mPos = pos;
    mIsDirty = true;
}

void TransformComponent::setSize(const glm::vec2 &size) {
    mSize = size;
    mIsDirty = true;
}

void TransformComponent::setAngle(float angle) {
    mAngle = angle;
    mIsDirty = true;
}

TransformComponent *TransformComponent::getParent() const {
    return mParent;
}

const std::vector<TransformComponent*> &TransformComponent::getChildren() const {
    return mChildren;
}

const glm::vec2 &TransformComponent::getAbsolutePosition() const {
    return mAbsolutePos;
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

TransformSystem::TransformSystem(Engine& engine) : AECSSystem(engine) {
}

void TransformSystem::onFixedUpdate() {
    const std::vector<TransformComponent*> &transforms = mEngine.getSceneSystem().getComponents<TransformComponent>();
    for (auto &transform : transforms) {
        if (transform) {
            transform->updateAbsolutePosition();
        }
    }
}

}
