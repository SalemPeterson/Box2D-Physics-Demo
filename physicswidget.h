#ifndef PHYSICSWIDGET_H
#define PHYSICSWIDGET_H

#include <QWidget>
#include <QMouseEvent>

class PhysicsWidget : public QWidget
{
    Q_OBJECT
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
public:
    explicit PhysicsWidget(QWidget *parent = nullptr);

signals:
    void mousePressed(PhysicsWidget* me);
    void mouseReleased(PhysicsWidget* me, int x, int y);
};

#endif // PHYSICSWIDGET_H
