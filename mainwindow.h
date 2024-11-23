#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "worldmodel.h"
#include <QMainWindow>
#include <QTimer>
#include <QGraphicsScene>
#include <Box2D/Box2D.h>
#include <stdio.h>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(WorldModel* model, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer timer;
    std::vector<QGraphicsProxyWidget*> physicsWidgets;
    QGraphicsScene* scene;
    void removePhysicsWidget(int index);
    void deleteWidgets();
    void spawnWidgets();

public slots:
    void updateObjects(std::vector<WorldModel::ObjectData>);
    void addPhysicsWidget(int x, int y, int width, int height);
    void createBoundary(int x, int y, int width, int height);

signals:
    void updateWorld();
    void addObject(float32 x, float32 y, float32 width, float32 height);
    void removeObject(int index);
};
#endif // MAINWINDOW_H
