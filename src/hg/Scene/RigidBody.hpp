#pragma once
#include "Component.hpp"
#include "glm/glm.hpp"
#include "Box2D/Box2D.h"

namespace hg {

class PhysicsWorld;

enum class BodyType {
    Static = b2_staticBody,
    Kinematic = b2_kinematicBody,
    Dynamic = b2_dynamicBody
};

class RigidBody : public Component {
    HG_OBJECT(RigidBody, Component);
    friend class PhysicsWorld;
public:
    ~RigidBody();

    void onSaveLoad(hd::JSON &data, bool isLoad) override;
    void onCreate() override;
    void onTransformUpdate() override;

    void setLinearVelocity(const glm::vec2 &vel);
    void setAngularVelocity(float vel);
    void setLinearDamping(float damping);
    void setAngularDamping(float damping);
    void setSleepAllowed(bool allowed);
    void setAwake(bool awake);
    void setFixedRotation(bool fixedRotation);
    void setBullet(bool bullet);
    void setType(BodyType type);
    void setActive(bool active);
    void setGravityScale(float scale);

    void setShapeDensity(float density);
    void setShapeFriction(float friction);
    void setShapeRestitution(float restitution);
    void setShapeSensor(bool sensor);

    glm::vec2 getLinearVelocity() const;
    float getAngularVelocity() const;
    float getLinearDamping() const;
    float getAngularDamping() const;
    bool isSleepAllowed() const;
    bool isAwake() const;
    bool isFixedRotation() const;
    bool isBullet() const;
    BodyType getType() const;
    bool isActive() const;
    float getGravityScale() const;

    float getShapeDensity() const;
    float getShapeFriction() const;
    float getShapeRestitution() const;
    bool isShapeSensor() const;

private:
    void mSetPosition(const glm::vec2 &pos);
    void mSetBoxShapeSize(const glm::vec2 &size);
    void mSetAngle(float angle);
    glm::vec2 mGetPosition() const;
    const glm::vec2 &mGetBoxShapeSize() const;
    float mGetAngle() const;
    void mApplyTransformToOwner();

    PhysicsWorld *mWorld;
    b2BodyDef mBodyDef;
    b2Body *mBody = nullptr;
    b2Fixture *mFixture = nullptr;
    glm::vec2 mBoxShapeSize = glm::vec2(0, 0);
};

}
