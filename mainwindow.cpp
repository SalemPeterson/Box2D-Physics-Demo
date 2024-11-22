#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "worldmodel.h"
#include <QTimer>
#include <Box2D/Box2D.h>
#include <stdio.h>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <vector>

MainWindow::MainWindow(WorldModel* model, QWidget *parent)
    : QMainWindow{parent}
    , ui{new Ui::MainWindow}
    , timer{this}
    , physicsWidgets{}
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    connect(&timer, &QTimer::timeout, model, &WorldModel::updateWorld);
    connect(model, &WorldModel::newObjectData, this, &MainWindow::updateObjects);
    connect(this, &MainWindow::addObject, model, &WorldModel::addBody);
    timer.setTimerType(Qt::PreciseTimer);
    timer.start(1000.0f / 60.0f);
    addPhysicsWidget(0, 400, 200, 200);
    addPhysicsWidget(100, 500, 100, 100);
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
        physicsWidgets[i]->setPos(objData[i].x * 100, ui->graphicsView->height() - objData[i].y * 100);
        physicsWidgets[i]->setRotation(objData[i].angle * 57.2957795131);
    }
}

void MainWindow::addPhysicsWidget(int x, int y, int width, int height) {
    QWidget *w = new QWidget();
    w->setStyleSheet(QString("Background-color: rgb(255,0,0)"));
    w->resize(width,height);
    physicsWidgets.push_back(scene->addWidget(w));
    emit addObject(x / 100.0f, y / 100.0f, width / 100.0f, height / 100.0f);
}

