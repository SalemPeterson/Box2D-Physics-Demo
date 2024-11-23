#include "worldmodel.h"
#include <vector>
#include <cmath>

WorldModel::WorldModel(QObject *parent)
    : QObject{parent}
    , gravity{0.0f, 5.0f}
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
        auto pos = body.body->GetPosition();
        auto angle = body.body->GetAngle();
        int xBL = pos.x * 100.0f;
        int yBL = pos.y * 100.0f;
        int wOff = body.width / 2.0f;
        int hOff = body.height / 2.0f;
        int xMid = xBL - wOff * (std::cos(angle) + std::cos(angle + 1.57079632679f));
        int yMid = yBL - hOff * (std::sin(angle) + std::sin(angle + 1.57079632679f));
        bodyData.push_back(ObjectData{xMid, yMid, angle * 57.2957795131f});
    }
    emit newObjectData(bodyData);
}

void WorldModel::addBody(int x, int y, int width, int height) {
    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x / scale, y /scale);
    b2Body* body = world.CreateBody(&bodyDef);

    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(width / (2.0f * scale), height / (2.0f * scale));

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;

    // Override the default friction.
    fixtureDef.friction = 0.9f;

    // Override the default restitution.
    fixtureDef.restitution = 0.3f;

    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);

    bodies.push_back(BodyData{body, width, height});
}

void WorldModel::removeBody(int index) {
    world.DestroyBody(bodies[index].body);
    bodies.erase(bodies.begin() + index);
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
    groundBox.SetAsBox(width / 2.0f, height / 2.0f);

    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 0.0f);
}
