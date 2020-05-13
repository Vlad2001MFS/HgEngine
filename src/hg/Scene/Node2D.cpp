#include "Node2D.hpp"

namespace hg {

Node2D::Node2D() : mPos(0, 0), mSize(0, 0) {
    mAngle = 0.0f;
}

void Node2D::onSaveLoad(hd::JSON &data, bool isLoad) {
    BaseClassName::onSaveLoad(data, isLoad);

    if (isLoad) {
        setPosition(data["position"]);
        setSize(data["size"]);
        setAngle(data["angle"]);
    }
    else {
        data["position"] = getPosition();
        data["size"] = getSize();
        data["angle"] = getAngle();
    }
}

void Node2D::translate(float x, float y) {
    setPosition(mPos.x + x, mPos.y + y);
}

void Node2D::translate(const glm::vec2 &offset) {
    translate(offset.x, offset.y);
}

void Node2D::scale(float x, float y) {
    setSize(mSize.x + x, mSize.y + y);
}

void Node2D::scale(const glm::vec2 &size) {
    scale(size.x, size.y);
}

void Node2D::rotate(float angle) {
    setAngle(mAngle + angle);
}

void Node2D::setPosition(float x, float y) {
    mPos.x = x;
    mPos.y = y;
}

void Node2D::setPosition(const glm::vec2 &pos) {
    setPosition(pos.x, pos.y);
}

void Node2D::setSize(float x, float y) {
    mSize.x = x;
    mSize.y = y;
}

void Node2D::setSize(const glm::vec2 &size) {
    setSize(size.x, size.y);
}

void Node2D::setAngle(float angle) {
    mAngle = angle;
}

const glm::vec2 &Node2D::getPosition() const {
    return mPos;
}

const glm::vec2 &Node2D::getSize() const {
    return mSize;
}

float Node2D::getAngle() const {
    return mAngle;
}

glm::vec2 Node2D::getAbsolutePosition() const {
    auto rotatedPos = [](const glm::vec2 &p, float a) {
        return glm::vec2(
            p.x*cosf(a) - p.y*sin(a),
            p.y*cosf(a) + p.x*sin(a)
        );
    };

    auto pos = glm::vec2(0, 0);
    const Node2D *node = this;
    while (node) {
        const Node2D *parent = node->getParent() && node->getParent()->isInstanceOf<Node2D>() ? node->getParent()->as<Node2D>() : nullptr;
        if (parent) {
            pos += rotatedPos(node->getPosition(), parent->getAngle());
        }
        else {
            pos += node->getPosition();
        }
        node = parent;
    }
    return pos;
}

}
