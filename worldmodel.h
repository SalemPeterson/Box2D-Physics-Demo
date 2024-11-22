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
    b2Vec2 gravity;
    b2World world;
    std::vector<b2Body*> bodies;

public:
    explicit WorldModel(QObject *parent = nullptr);
    struct ObjectData {
        float32 x, y, angle;
    };

public slots:
    void updateWorld();
    void addBody(float32 x, float32 y, float32 width, float32 height);
    void removeBody(int index);

signals:
    void newObjectData(std::vector<ObjectData> data);
};

#endif // WORLDMODEL_H
