#pragma once
#include "Component.hpp"
#include "glm/glm.hpp"
#include "Box2D/Box2D.h"

namespace hg {

class RigidBody;

class PhysicsWorld : public Component {
    HG_OBJECT(PhysicsWorld, Component);
    friend class RigidBody;
public:
    PhysicsWorld();

    void onSaveLoad(hd::JSON &data, bool isLoad) override;
    void onUpdate(float dt) override;

    void setGravity(const glm::vec2 &gravity);

    b2World &getWorld();
    glm::vec2 getGravity() const;
    bool isApplyingTransformsToOwners() const;

private:
    void mAddRigidBody(RigidBody *body);
    void mRemoveRigidBody(RigidBody *body);

    b2World mWorld;
    std::vector<std::pair<int, RigidBody*>> mRigidBodies;
    bool mIsRigidBodiesDirty = false;
    bool mIsApplyingTransformsToOwners = false;
};

}
