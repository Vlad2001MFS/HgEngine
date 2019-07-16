#pragma once
#include "SceneSystem.hpp"
#include "3rd/include/glm/glm.hpp"

namespace hg2d {

class TransformComponent : public AECSComponent {
public:
    TransformComponent();

    void updateAbsolutePosition();

    void translate(const glm::vec2 &offset);
    void scale(const glm::vec2 &scale);
    void rotate(float angle);

    void setParent(TransformComponent *parent);
    void setPosition(const glm::vec2 &pos);
    void setSize(const glm::vec2 &size);
    void setAngle(float angle);

    TransformComponent *getParent() const;
    const std::vector<TransformComponent*> &getChildren() const;
    const glm::vec2 &getAbsolutePosition() const;
    const glm::vec2 &getPosition() const;
    const glm::vec2 &getSize() const;
    float getAngle() const;

private:
    std::vector<TransformComponent*> mChildren;
    TransformComponent *mParent;

    glm::vec2 mPos, mAbsolutePos;
    glm::vec2 mSize;
    float mAngle;
    bool mIsDirty;
};

class TransformSystem : public AECSSystem {
public:
    TransformSystem(Engine &engine);

    virtual void onFixedUpdate() override;
};

}
