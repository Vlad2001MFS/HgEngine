#pragma once
#include "hd/Core/Common.hpp"
#include "hd/Core/Log.hpp"
#include "hd/Core/StringUtils.hpp"
#include "hd/Window/WindowEvent.hpp"
#include "glm/glm.hpp"
#include "nameof/nameof.hpp"
#include <vector>
#include <unordered_map>

namespace hg2d {

class Node : public hd::Noncopyable {
public:
    Node();
    virtual ~Node();

    virtual void onEvent(const hd::WindowEvent &event);
    virtual void onFixedUpdate();
    virtual void onUpdate(float dt);
    virtual void onDraw();

    Node *findByName(const std::string &name);

    template<typename T>
    T *as() {
        T *ptr = dynamic_cast<T*>(this);
        if (!ptr) {
            LOG_F(WARNING, "Failed to cast node to '{}'", NAMEOF_TYPE(T));
        }
        return ptr;
    }

    template<typename T, typename... Args>
    T *createChild(const std::string &name, Args &&...args) {
        uint64_t nameHash = hd::StringUtils::getHash(name);
        if (!name.empty() && mChildrenByNames.count(nameHash) != 0) {
            LOG_F(FATAL, "Failed to create child '{}'", name);
        }
        T *node = new T(std::forward(args)...);
        mChildren.push_back(node);
        node->mParent = this;
        if (!name.empty()) {
            mChildrenByNames.insert(std::make_pair(nameHash, node));
        }
        return node;
    }
    void destroyChild(const std::string &name);

    void translate(const glm::vec2 &offset);
    void scale(const glm::vec2 &size);
    void rotate(float angle);

    void setPosition(const glm::vec2 &pos);
    void setSize(const glm::vec2 &size);
    void setAngle(float angle);
    void setActive(bool active);

    Node *getParent() const;
    const std::vector<Node*> &getChildren() const;
    const std::unordered_map<uint64_t, Node*> &getChildrenByNames() const;
    const glm::vec2 &getPosition() const;
    const glm::vec2 &getSize() const;
    float getAngle() const;
    bool isActive() const;
    glm::vec2 getAbsolutePosition() const;

private:
    Node *mParent;
    std::vector<Node*> mChildren;
    std::unordered_map<uint64_t, Node*> mChildrenByNames;
    glm::vec2 mPos, mSize;
    float mAngle;
    bool mIsActive;
};

}