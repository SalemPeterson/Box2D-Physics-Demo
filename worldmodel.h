#ifndef WORLDMODEL_H
#define WORLDMODEL_H

#include <QObject>
#include <Box2D/Box2D.h>
#include <stdio.h>
#include <vector>

class WorldModel : public QObject
{
    Q_OBJECT
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;
    float32 scale = 100.0f;
    b2Vec2 gravity;
    b2World world;
    struct BodyData {
        b2Body* body;
        int width, height;
    };

    std::vector<BodyData> bodies;

    void createBoundary(float32 x, float32 y, float32 width, float32 height);

public:
    explicit WorldModel(QObject *parent = nullptr);
    struct ObjectData {
        int x, y;
        float32 angle;
    };

public slots:
    void updateWorld();
    void addBody(int x, int y, int width, int height);
    void removeBody(int index);

signals:
    void newObjectData(std::vector<ObjectData> data);
};

#endif // WORLDMODEL_H
