#include "Node.hpp"

namespace hg2d {

Node::Node() : mPos(0, 0), mSize(0, 0) {
    mParent = nullptr;
    mAngle = 0.0f;
    mIsActive = true;
}

Node::~Node() {
    for (const auto &it : mChildren) {
        delete it;
    }
}

void Node::onEvent(const hd::WindowEvent &event) {
    for (const auto &it : mChildren) {
        if (it->isActive()) {
            it->onEvent(event);
        }
    }
}

void Node::onFixedUpdate() {
    for (const auto &it : mChildren) {
        if (it->isActive()) {
            it->onFixedUpdate();
        }
    }
}

void Node::onUpdate(float dt) {
    for (const auto &it : mChildren) {
        if (it->isActive()) {
            it->onUpdate(dt);
        }
    }
}

void Node::onDraw() {
    for (const auto &it : mChildren) {
        if (it->isActive()) {
            it->onDraw();
        }
    }
}

Node *Node::findByName(const std::string &name) {
    if (name.empty()) {
        LOG_F(FATAL, "Invalid name '{}'", name);
    }

    uint64_t nameHash = hd::StringUtils::getHash(name);
    auto it = mChildrenByNames.find(nameHash);
    if (it == mChildrenByNames.end()) {
        LOG_F(FATAL, "Node '{}' not found", name);
    }
    return it->second;
}

void Node::destroyChild(const std::string &name) {
    Node *ptr = findByName(name);
    if (ptr) {
        mChildren.erase(std::remove(mChildren.begin(), mChildren.end(), ptr));
        mChildrenByNames.erase(hd::StringUtils::getHash(name));
        delete ptr;
    }
}

void Node::translate(float x, float y) {
    setPosition(mPos.x + x, mPos.y + y);
}

void Node::translate(const glm::vec2 &offset) {
    translate(offset.x, offset.y);
}

void Node::scale(float x, float y) {
    setSize(mSize.x + x, mSize.y + y);
}

void Node::scale(const glm::vec2 &size) {
    scale(size.x, size.y);
}

void Node::rotate(float angle) {
    setAngle(mAngle + angle);
}

void Node::setPosition(float x, float y) {
    mPos.x = x;
    mPos.y = y;
}

void Node::setPosition(const glm::vec2 &pos) {
    setPosition(pos.x, pos.y);
}

void Node::setSize(float x, float y) {
    mSize.x = x;
    mSize.y = y;
}

void Node::setSize(const glm::vec2 &size) {
    setSize(size.x, size.y);
}

void Node::setAngle(float angle) {
    mAngle = angle;
}

void Node::setActive(bool active) {
    mIsActive = active;
}

Node *Node::getParent() const {
    return mParent;
}

const std::vector<Node*> &Node::getChildren() const {
    return mChildren;
}

const std::unordered_map<uint64_t, Node*> &Node::getChildrenByNames() const {
    return mChildrenByNames;
}

const glm::vec2 &Node::getPosition() const {
    return mPos;
}

const glm::vec2 &Node::getSize() const {
    return mSize;
}

float Node::getAngle() const {
    return mAngle;
}

bool Node::isActive() const {
    return mIsActive;
}

glm::vec2 Node::getAbsolutePosition() const {
    auto rotatedPos = [](const glm::vec2 &p, float a) {
        return glm::vec2(
            p.x*cosf(a) - p.y*sin(a),
            p.y*cosf(a) + p.x*sin(a)
        );
    };

    auto pos = glm::vec2(0, 0);
    const Node *node = this;
    while (node) {
        if (node->getParent()) {
            pos += rotatedPos(node->getPosition(), node->getParent()->getAngle());
        }
        else {
            pos += node->getPosition();
        }
        node = node->getParent();
    }
    return pos;
}

}