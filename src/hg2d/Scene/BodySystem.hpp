#pragma once
#include "SceneSystem.hpp"
#include "3rd/include/glm/glm.hpp"
#include "../../3rd/include/Box2D/Box2D.h"
#include <string>

namespace hg2d {

enum class BodyType {
    Static,
    Kinematic,
    Dynamic
};

struct CollisionInfo {
    HEntity entity;
    bool hasCollision;
};

class BodyComponent final : public AECSComponent {
    friend class BodySystem;
public:
    BodyComponent(Engine &engine);

    virtual void onSaveLoad(JSONObject &json, bool isLoad) override;
    
    void setPosition(const glm::vec2 &pos);
    void setAngle(float angle);
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

    void setBoxShapeSize(const glm::vec2 &size);
    void setShapeDensity(float density);
    void setShapeFriction(float friction);
    void setShapeRestitution(float restitution);
    void setShapeSensor(bool sensor);

    const glm::vec2 &getBoxShapeSize() const;
    glm::vec2 getPosition() const;
    float getAngle() const;
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

    CollisionInfo getCollisionInfo() const;

private:
    b2BodyDef mBodyDef;
    b2Body *mBody;
    b2Fixture *mFixture;
    glm::vec2 mBoxShapeSize;
};

class ContactListener final : public b2ContactListener {
public:
    virtual void BeginContact(b2Contact *contact) override;
    virtual void EndContact(b2Contact *contact) override;
};

class BodySystem final : public AECSSystem {
public:
    BodySystem(Engine &engine);

    virtual void onInitialize() override;
    virtual void onShutdown() override;
    virtual void onCreateComponent(AECSComponent *component, uint64_t typeHash, const HEntity &entity) override;
    virtual void onDestroyComponent(AECSComponent *component, uint64_t typeHash, const HEntity &entity) override;
    virtual void onSaveLoad(JSONObject &json, bool isLoad) override;
    virtual void onClear() override;
    virtual void onFixedUpdate() override;

    void setGravity(const glm::vec2 &gravity);

    glm::vec2 getGravity() const;

private:
    b2World *mWorld;
    ContactListener mContactListener;
};

}
