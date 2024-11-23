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
        int x_BL = objData[i].x * 100;
        int y_BL = objData[i].y * 100;
        int w_off = physicsWidgets[i]->preferredWidth() / 2;
        int h_off = physicsWidgets[i]->preferredHeight() / 2;
        int x = x_BL - w_off * (std::cos(objData[i].angle) + std::cos(objData[i].angle + 1.57079632679));
        int y = y_BL - h_off * (std::sin(objData[i].angle) + std::sin(objData[i].angle + 1.57079632679));

        physicsWidgets[i]->setPos(x, y);
        physicsWidgets[i]->setRotation(objData[i].angle * 57.2957795131);
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
    wp->setPreferredWidth(width);
    wp->setPreferredHeight(height);
    physicsWidgets.push_back(wp);
    emit addObject(x / 100.0f, y / 100.0f, width / 100.0f, height / 100.0f);
}

void MainWindow::createBoundary(int x, int y, int width, int height) {
    QWidget *w = new QWidget();
    w->setStyleSheet(QString("Background-color: rgb(0,255,0)"));
    w->resize(width,height);
    scene->addWidget(w)->setPos(x,y);
}
