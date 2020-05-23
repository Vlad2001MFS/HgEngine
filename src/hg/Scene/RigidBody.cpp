#include "RigidBody.hpp"
#include "PhysicsWorld.hpp"
#include "Scene.hpp"

namespace hg {

static b2Vec2 toBox2D(const glm::vec2 &v) {
    return b2Vec2(v.x, v.y);
}

static glm::vec2 fromBox2D(const b2Vec2 &v) {
    return glm::vec2(v.x, v.y);
}

RigidBody::~RigidBody() {
    mWorld->mRemoveRigidBody(this);
    mWorld->getWorld().DestroyBody(mBody);
}

void RigidBody::onSaveLoad(hd::JSON& data, bool isLoad) {
    hd::JSON &position = data["position"];
    hd::JSON &angle = data["angle"];
    hd::JSON &linearVelocity = data["linearVelocity"];
    hd::JSON &angularVelocity = data["angularVelocity"];
    hd::JSON &linearDamping = data["linearDamping"];
    hd::JSON &angularDamping = data["angularDamping"];
    hd::JSON &sleepAllowed = data["isSleepingAllowed"];
    hd::JSON &awake = data["isAwake"];
    hd::JSON &fixedRotation = data["isFixedRotation"];
    hd::JSON &bullet = data["isBullet"];
    hd::JSON &type = data["type"];
    hd::JSON &active = data["isActive"];
    hd::JSON &gravityScale = data["gravityScale"];

    hd::JSON &boxShapeSize = data["boxShapeSize"];
    hd::JSON &shapeDensity = data["density"];
    hd::JSON &shapeFriction = data["friction"];
    hd::JSON &shapeRestitution = data["restitution"];
    hd::JSON &shapeSensor = data["isSensor"];

    hd::JSON &components = data["components"];
    hd::JSON &children = data["children"];

    if (isLoad) {
        mSetPosition(position.get<glm::vec2>());
        mSetAngle(angle.get<float>());
        setLinearVelocity(linearVelocity.get<glm::vec2>());
        setAngularVelocity(angularVelocity.get<float>());
        setLinearDamping(linearDamping.get<float>());
        setAngularDamping(angularDamping.get<float>());
        setSleepAllowed(sleepAllowed.get<bool>());
        setAwake(awake.get<bool>());
        setFixedRotation(fixedRotation.get<bool>());
        setBullet(bullet.get<bool>());
        setType(type.get<BodyType>());
        setActive(active.get<bool>());
        setGravityScale(gravityScale.get<float>());

        mSetBoxShapeSize(boxShapeSize.get<glm::vec2>());
        setShapeDensity(shapeDensity.get<float>());
        setShapeFriction(shapeFriction.get<float>());
        setShapeRestitution(shapeRestitution.get<float>());
        setShapeSensor(shapeSensor.get<bool>());
    }
    else {
        position = mGetPosition();
        angle = mGetAngle();
        linearVelocity = getLinearVelocity();
        angularVelocity = getAngularVelocity();
        linearDamping = getLinearDamping();
        angularDamping = getAngularDamping();
        sleepAllowed = isSleepAllowed();
        awake = isAwake();
        fixedRotation = isFixedRotation();
        bullet = isBullet();
        type = getType();
        active = isActive();
        gravityScale = getGravityScale();

        boxShapeSize = mGetBoxShapeSize();
        shapeDensity = getShapeDensity();
        shapeFriction = getShapeFriction();
        shapeRestitution = getShapeRestitution();
        shapeSensor = isShapeSensor();
    }
}

void RigidBody::onCreate() {
    mWorld = getScene().findComponent<PhysicsWorld>();
    if (!mWorld) {
        HD_LOG_FATAL("Failed to find 'PhysicsWorld' component at Scene");
    }
    mBody = mWorld->getWorld().CreateBody(&mBodyDef);
    mWorld->mAddRigidBody(this);
    onTransformUpdate();
}

void RigidBody::onTransformUpdate() {
    if (mWorld->isApplyingTransformsToOwners()) {
        return;
    }

    mSetPosition(getOwner()->getWorldPosition());
    mSetBoxShapeSize(getOwner()->getSize());
    mSetAngle(getOwner()->getWorldAngle());
}

void RigidBody::setLinearVelocity(const glm::vec2 &vel) {
    mBody->SetLinearVelocity(toBox2D(vel));
}

void RigidBody::setAngularVelocity(float vel) {
    mBody->SetAngularVelocity(vel);
}

void RigidBody::setLinearDamping(float damping) {
    mBody->SetLinearDamping(damping);
}

void RigidBody::setAngularDamping(float damping) {
    mBody->SetAngularDamping(damping);
}

void RigidBody::setSleepAllowed(bool allowed) {
    mBody->SetSleepingAllowed(allowed);
}

void RigidBody::setAwake(bool awake) {
    mBody->SetAwake(awake);
}

void RigidBody::setFixedRotation(bool fixedRotation) {
    mBody->SetFixedRotation(fixedRotation);
}

void RigidBody::setBullet(bool bullet) {
    mBody->SetBullet(bullet);
}

void RigidBody::setType(BodyType type) {
    mBody->SetType(static_cast<b2BodyType>(type));
}

void RigidBody::setActive(bool active) {
    mBody->SetActive(active);
}

void RigidBody::setGravityScale(float scale) {
    mBody->SetGravityScale(scale);
}

void RigidBody::setShapeDensity(float density) {
    if (mFixture) {
        mFixture->SetDensity(density);
    }
}

void RigidBody::setShapeFriction(float friction) {
    if (mFixture) {
        mFixture->SetFriction(friction);
    }
}

void RigidBody::setShapeRestitution(float restitution) {
    if (mFixture) {
        mFixture->SetRestitution(restitution);
    }
}

void RigidBody::setShapeSensor(bool sensor) {
    if (mFixture) {
        mFixture->SetSensor(sensor);
    }
}

glm::vec2 RigidBody::getLinearVelocity() const {
    return fromBox2D(mBody->GetLinearVelocity());
}

float RigidBody::getAngularVelocity() const {
    return mBody->GetAngularVelocity();
}

float RigidBody::getLinearDamping() const {
    return mBody->GetLinearDamping();
}

float RigidBody::getAngularDamping() const {
    return mBody->GetAngularDamping();
}

bool RigidBody::isSleepAllowed() const {
    return mBody->IsSleepingAllowed();
}

bool RigidBody::isAwake() const {
    return mBody->IsAwake();
}

bool RigidBody::isFixedRotation() const {
    return mBody->IsFixedRotation();
}

bool RigidBody::isBullet() const {
    return mBody->IsBullet();
}

BodyType RigidBody::getType() const {
    return static_cast<BodyType>(mBody->GetType());
}

bool RigidBody::isActive() const {
    return mBody->IsActive();
}

float RigidBody::getGravityScale() const {
    return mBody->GetGravityScale();
}

float RigidBody::getShapeDensity() const {
    if (mFixture) {
        return mFixture->GetDensity();
    }
    else {
        return 0.0f;
    }
}

float RigidBody::getShapeFriction() const {
    if (mFixture) {
        return mFixture->GetFriction();
    }
    else {
        return 0.0f;
    }
}

float RigidBody::getShapeRestitution() const {
    if (mFixture) {
        return mFixture->GetRestitution();
    }
    else {
        return 0.0f;
    }
}

bool RigidBody::isShapeSensor() const {
    if (mFixture) {
        return mFixture->IsSensor();
    }
    else {
        return false;
    }
}

void RigidBody::mSetPosition(const glm::vec2 &pos) {
    mBody->SetTransform(toBox2D(pos), mBody->GetAngle());
}

void RigidBody::mSetBoxShapeSize(const glm::vec2& size) {
    if (mFixture) {
        mBody->DestroyFixture(mFixture);
    }

    mBoxShapeSize = size;
    b2PolygonShape shape;
    shape.SetAsBox(size.x, size.y);
    b2FixtureDef fd;
    fd.shape = &shape;
    mFixture = mBody->CreateFixture(&fd);
}

void RigidBody::mSetAngle(float angle) {
    mBody->SetTransform(mBody->GetPosition(), angle);
}

glm::vec2 RigidBody::mGetPosition() const {
    return fromBox2D(mBody->GetPosition());
}

const glm::vec2& RigidBody::mGetBoxShapeSize() const {
    return mBoxShapeSize;
}

float RigidBody::mGetAngle() const {
    return mBody->GetAngle();
}

void RigidBody::mApplyTransformToOwner() {
    getOwner()->setWorldPosition(mGetPosition());
    getOwner()->setSize(mGetBoxShapeSize());
    getOwner()->setWorldAngle(mGetAngle());
}

}
