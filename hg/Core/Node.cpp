#include "Node.hpp"

namespace hg {

HG_REGISTER_OBJECT(Node);

Node::Node() {
    mParent = nullptr;
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
        setActive(data["isActive"]);
        
        JSONObject &children = data["children"];
        for (auto &it : children) {
            Node *child = createChild(it["name"].get<std::string>(), it["typeInfo"]["hash"].get<hd::StringHash>());
            child->onSaveLoad(it, isLoad);
        }
    }
    else {
        data["name"] = getName();
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

bool Node::isActive() const {
    return mIsActive;
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
