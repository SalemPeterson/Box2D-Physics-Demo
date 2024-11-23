#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "worldmodel.h"
#include <QTimer>
#include <Box2D/Box2D.h>
#include <stdio.h>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <vector>
#include <QDebug>
#include <cmath>

MainWindow::MainWindow(WorldModel* model, QWidget *parent)
    : QMainWindow{parent}
    , ui{new Ui::MainWindow}
    , timer{this}
    , physicsWidgets{}
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    createBoundary(0, 400, 400, 1);
    createBoundary(0, 0, 400, 1);
    createBoundary(400, 0, 1, 400);
    createBoundary(0, 0, 1, 400);

    connect(&timer, &QTimer::timeout, model, &WorldModel::updateWorld);
    connect(model, &WorldModel::newObjectData, this, &MainWindow::updateObjects);
    connect(this, &MainWindow::addObject, model, &WorldModel::addBody);
    timer.setTimerType(Qt::PreciseTimer);
    timer.start(1000.0f / 60.0f);
    addPhysicsWidget(100, 100, 50, 50);
    addPhysicsWidget(125, 200, 50, 50);
    addPhysicsWidget(150, 300, 50, 50);
}

MainWindow::~MainWindow()
{
    for (auto widget : physicsWidgets) {
        delete widget;
    }
    delete scene;
    delete ui;
}

void MainWindow::updateObjects(std::vector<WorldModel::ObjectData> objData) {
    for (size_t i = 0; i < objData.size(); i++) {
        physicsWidgets[i]->setPos(objData[i].x, objData[i].y);
        physicsWidgets[i]->setRotation(objData[i].angle);
    }
}

void MainWindow::addPhysicsWidget(int x, int y, int width, int height) {
    QWidget *w = new QWidget();
    w->setStyleSheet(QString("Background-color: rgb(255,0,0)"));
    w->resize(width,height);
    QWidget *w2 = new QWidget(w);
    w2->setStyleSheet(QString("Background-color: rgb(0,0,255)"));
    w2->resize(10, 10);
    w2->move(0, height - 10);
    auto wp = scene->addWidget(w);
    physicsWidgets.push_back(wp);
    emit addObject(x, y, width, height);
}

void MainWindow::createBoundary(int x, int y, int width, int height) {
    QWidget *w = new QWidget();
    w->setStyleSheet(QString("Background-color: rgb(0,255,0)"));
    w->resize(width,height);
    scene->addWidget(w)->setPos(x, y);
}
