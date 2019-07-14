#include "GameObject.hpp"
#include "../Core/Engine.hpp"

namespace hg2d {

GameObject::GameObject(Engine &engine) : mEngine(engine) {
    mParent = nullptr;
}

GameObject::~GameObject() {
    setParent(nullptr);
    for (auto &component : mComponents) {
        HD_DELETE(component.second);
    }
    for (auto &child : mChildren) {
        mEngine.getSceneSystem().destroyObject(child);
    }
}

AECSComponent *GameObject::createComponent(uint64_t typeNameHash) {
    if (mComponents.count(typeNameHash) == 0) {
        AECSComponent *component = mEngine.getSceneSystem().createComponent(typeNameHash);
        mComponents.insert(std::make_pair(typeNameHash, component));
        return component;
    }
    else {
        HD_LOG_WARNING("Failed to create component with typename hash '%s'. The component with same type already registered at GameObject", std::to_string(typeNameHash).data());
        return nullptr;
    }
}

void GameObject::setParent(GameObject *parent) {
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
    }
}

void GameObject::setPosition(float x, float y) {
    mTransform.pos.x = x;
    mTransform.pos.y = y;
}

void GameObject::setPosition(const glm::vec2 &pos) {
    mTransform.pos = pos;
}

void GameObject::setSize(float x, float y) {
    mTransform.size.x = x;
    mTransform.size.y = y;
}

void GameObject::setSize(const glm::vec2 &size) {
    mTransform.size = size;
}

void GameObject::setAngle(float angle) {
    mTransform.angle = angle;
}

void GameObject::translate(float x, float y) {
    mTransform.pos.x += x;
    mTransform.pos.y += y;
}

void GameObject::translate(const glm::vec2 &pos) {
    mTransform.pos += pos;
}

void GameObject::scale(float x, float y) {
    mTransform.size.x += x;
    mTransform.size.y += y;
}

void GameObject::scale(const glm::vec2 &size) {
    mTransform.size += size;
}

void GameObject::rotate(float angle) {
    mTransform.angle += angle;
}

glm::vec2 GameObject::getAbsolutePosition() const {
    if (getParent()) {
        float parentAngle = glm::radians(getParent()->getAngle());
        glm::vec2 pos = glm::vec2(
            getPosition().x*std::cos(parentAngle) - getPosition().y*std::sin(parentAngle),
            getPosition().x*std::sin(parentAngle) + getPosition().y*std::cos(parentAngle)
        );
        return getParent()->getAbsolutePosition() + pos;
    }
    else {
        return getPosition();
    }
}

const glm::vec2 &GameObject::getPosition() const {
    return mTransform.pos;
}

const glm::vec2 &GameObject::getSize() const {
    return mTransform.size;
}

float GameObject::getAngle() const {
    return mTransform.angle;
}

const std::vector<GameObject*> &GameObject::getChildren() const {
    return mChildren;
}

GameObject *GameObject::getParent() const {
    return mParent;
}

void GameObject::_saveLoad(hd::SerializerRW &stream) {
    mTransform.onSaveLoad(stream);
    if (stream.isWrite()) {
        stream.process<uint64_t>(mComponents.size());
        for (auto &component : mComponents) {
            stream.process<uint64_t>(typeid(component.second).hash_code());
            component.second->onSaveLoad(stream);
        }
        stream.process<uint64_t>(mChildren.size());
        for (auto &child : mChildren) {
            child->_saveLoad(stream);
        }
    }
    else {
        uint64_t componentsCount = stream.process<uint64_t>();
        for (uint64_t i = 0; i < componentsCount; i++) {
            uint64_t typeNameHash = stream.process<uint64_t>();
            createComponent(typeNameHash)->onSaveLoad(stream);
        }
        uint64_t childrenCount = stream.process<uint64_t>();
        for (uint64_t i = 0; i < childrenCount; i++) {
            mEngine.getSceneSystem().createObject(this)->_saveLoad(stream);
        }
    }
}

GameObject::TransformComponent::TransformComponent() : pos(0, 0), size(0, 0) {
    this->angle = 0;
}

void GameObject::TransformComponent::onSaveLoad(hd::SerializerRW &stream) {
    stream.process<glm::vec2>(this->pos);
    stream.process<glm::vec2>(this->size);
    stream.process<float>(this->angle);
}

}
