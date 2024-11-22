#include "worldmodel.h"

WorldModel::WorldModel(QObject *parent)
    : QObject{parent}
    , gravity{0.0f, -5.0f}
    , world{gravity}
    , bodies{}
{
    createBoundary(0.0f, -10.0f, 100.0f, 20.0f);
    createBoundary(0.0f, 14.0f, 100.0f, 20.0f);
    createBoundary(-10.0f, 0.0f, 20.0f, 100.0f);
    createBoundary(14.0f, 0.0f, 20.0f, 100.0f);
}

void WorldModel::updateWorld() {
    // Instruct the world to perform a single step of simulation.
    // It is generally best to keep the time step and iterations fixed.
    world.Step(timeStep, velocityIterations, positionIterations);

    std::vector<ObjectData> bodyData{};
    for (auto body : bodies) {
        auto pos = body->GetPosition();
        bodyData.push_back(ObjectData{pos.x, pos.y, body->GetAngle()});
    }
    emit newObjectData(bodyData);
}

void WorldModel::addBody(float32 x, float32 y, float32 width, float32 height) {
    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    b2Body* body = world.CreateBody(&bodyDef);

    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(width / 2, height / 2);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;

    // Override the default friction.
    fixtureDef.friction = 0.9f;

    // Override the default restitution.
    fixtureDef.restitution = 0.9f;

    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);

    bodies.push_back(body);
}

void WorldModel::removeBody(int index) {
    world.DestroyBody(bodies[index]);
}

void WorldModel::createBoundary(float32 x, float32 y, float32 width, float32 height) {
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(x, y);

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2PolygonShape groundBox;

    // The extents are the half-widths of the box.
    groundBox.SetAsBox(width / 2, height / 2);

    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 0.0f);
}
