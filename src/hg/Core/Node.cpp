#include "Node.hpp"

namespace hg {

Node::Node() {
    mParent = nullptr;
    mIsActive = true;
}

Node::~Node() {
    for (auto &it : mComponents) {
        HD_DELETE(it);
    }
    for (auto &it : mChildren) {
        HD_DELETE(it);
    }
}

void Node::onSaveLoad(hd::JSON &data, bool isLoad) {
    if (isLoad) {
        mName = data["name"];
        setActive(data["isActive"]);

        hd::JSON &components = data["components"];
        for (auto &it : components) {
            Component *comp = createComponent(it["typeInfo"]["hash"].get<hd::StringHash>());
            comp->onSaveLoad(it, isLoad);
        }

        hd::JSON &children = data["children"];
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

        hd::JSON &components = data["components"];
        for (const auto &it : mComponents) {
            hd::JSON comp;
            it->onSaveLoad(comp, isLoad);
            components.push_back(comp);
        }

        hd::JSON &children = data["children"];
        for (const auto &it : mChildren) {
            hd::JSON child;
            it->onSaveLoad(child, isLoad);
            children.push_back(child);
        }
    }
}

void Node::onEvent(const WindowEvent &event) {
    for (auto &it : mComponents) {
        it->onEvent(event);
    }

    for (const auto &it : mChildren) {
        if (it->isActive()) {
            it->onEvent(event);
        }
    }
}

void Node::onFixedUpdate() {
    for (auto &it : mComponents) {
        it->onFixedUpdate();
    }

    for (const auto &it : mChildren) {
        if (it->isActive()) {
            it->onFixedUpdate();
        }
    }
}

void Node::onUpdate(float dt) {
    for (auto &it : mComponents) {
        it->onUpdate(dt);
    }

    for (const auto &it : mChildren) {
        if (it->isActive()) {
            it->onUpdate(dt);
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

void Node::mAddComponent(Component *component) {
    if (!mComponentsByType.count(component->getTypeHash())) {
        component->mOwner = this;
        mComponents.push_back(component);
        mComponentsByType.insert(std::make_pair(component->getTypeHash(), component));
    }
    else {
        HD_LOG_ERROR("Failed to add component '{}'", component->getTypeName());
    }
}

}
