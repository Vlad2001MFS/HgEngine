#include "PhysicsWorld.hpp"
#include "RigidBody.hpp"
#include "GameObject.hpp"

namespace hg {

static b2Vec2 toBox2D(const glm::vec2 &v) {
    return b2Vec2(v.x, v.y);
}

static glm::vec2 fromBox2D(const b2Vec2 &v) {
    return glm::vec2(v.x, v.y);
}

PhysicsWorld::PhysicsWorld() : mWorld(b2Vec2(0, 0)) {
}

void PhysicsWorld::onSaveLoad(hd::JSON& data, bool isLoad) {
    hd::JSON &gravity = data["gravity"];
    if (isLoad) {
        setGravity(gravity.get<glm::vec2>());
    }
    else {
        gravity = getGravity();
    }
}

void PhysicsWorld::onUpdate(float dt) {
    mWorld.Step(1.0f / 60.0f, 6, 2);

    if (mIsRigidBodiesDirty) {
        mIsRigidBodiesDirty = false;
        std::sort(mRigidBodies.begin(), mRigidBodies.end(), [](const std::pair<int, RigidBody*> &a, const std::pair<int, RigidBody*> &b) {
            return a.first < b.first;
        });
    }

    mIsApplyingTransformsToOwners = true;
    for (auto &[depth, body] : mRigidBodies) {
        body->mApplyTransformToOwner();
    }
    mIsApplyingTransformsToOwners = false;
}

void PhysicsWorld::setGravity(const glm::vec2 &gravity) {
    mWorld.SetGravity(toBox2D(gravity));
}

b2World &PhysicsWorld::getWorld() {
    return mWorld;
}

glm::vec2 PhysicsWorld::getGravity() const {
    return fromBox2D(mWorld.GetGravity());
}

bool PhysicsWorld::isApplyingTransformsToOwners() const {
    return mIsApplyingTransformsToOwners;
}

void PhysicsWorld::mAddRigidBody(RigidBody *body) {
    int goDepth = 0;
    GameObject *go = body->getOwner()->getParent();
    while (go) {
        goDepth++;
        go = go->getParent();
    }

    mRigidBodies.push_back(std::make_pair(goDepth, body));
    mIsRigidBodiesDirty = true;
}

void PhysicsWorld::mRemoveRigidBody(RigidBody *body) {
    auto it = std::find_if(mRigidBodies.begin(), mRigidBodies.end(), [&](const std::pair<int, RigidBody*> &p) {
        return p.second == body;
    });
    mRigidBodies.erase(it);
}

}
