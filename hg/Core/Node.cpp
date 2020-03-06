#include "Node.hpp"

namespace hg {

HG_REGISTER_OBJECT(Node);

Node::Node() : mPos(0, 0), mSize(0, 0) {
    mParent = nullptr;
    mAngle = 0.0f;
    mIsActive = true;
}

Node::~Node() {
    for (auto &it : mChildren) {
        HD_DELETE(it);
    }
}

void Node::onSaveLoad(JSONObject &data, bool isLoad) {
    if (isLoad) {
        mName = data["name"];
        setPosition(data["position"]);
        setSize(data["size"]);
        setAngle(data["angle"]);
        setActive(data["isActive"]);
        
        JSONObject &children = data["children"];
        for (auto &it : children) {
            Node *child = createChild(it["name"].get<std::string>(), it["typeInfo"]["hash"].get<hd::StringHash>());
            child->onSaveLoad(it, isLoad);
        }
    }
    else {
        data["name"] = getName();
        data["position"] = getPosition();
        data["size"] = getSize();
        data["angle"] = getAngle();
        data["isActive"] = isActive();
        data["typeInfo"] = {
            { "name", getTypeName() },
            { "hash", getTypeHash() }
        };

        JSONObject &children = data["children"];
        for (const auto &it : mChildren) {
            JSONObject child;
            it->onSaveLoad(child, isLoad);
            children.push_back(child);
        }
    }
}

void Node::onEvent(const SDL_Event &event) {
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
        HD_LOG_FATAL("Invalid name '{}'", name);
    }

    auto it = mChildrenByNames.find(hd::StringHash(name));
    if (it == mChildrenByNames.end()) {
        HD_LOG_FATAL("Node '{}' not found", name);
    }
    return it->second;
}

void Node::destroyChild(const std::string &name) {
    Node *ptr = findByName(name);
    if (ptr) {
        mChildren.erase(std::remove(mChildren.begin(), mChildren.end(), ptr));
        mChildrenByNames.erase(hd::StringHash(name));
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

const std::unordered_map<hd::StringHash, Node*> &Node::getChildrenByNames() const {
    return mChildrenByNames;
}

const std::string &Node::getName() const {
    return mName;
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

void Node::mAddChild(Node *node, const std::string &name) {
    hd::StringHash nameHash = hd::StringHash(name);
    if (!name.empty() && mChildrenByNames.count(nameHash) != 0) {
        HD_LOG_FATAL("Failed to create child '{}'", name);
    }
    mChildren.push_back(node);
    node->mParent = this;
    node->mName = name;
    if (!name.empty()) {
        mChildrenByNames.insert(std::make_pair(nameHash, node));
    }
}

}