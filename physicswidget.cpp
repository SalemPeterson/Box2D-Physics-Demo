#include "physicswidget.h"

PhysicsWidget::PhysicsWidget(QWidget *parent)
    : QWidget{parent}
{}

void PhysicsWidget::mousePressEvent(QMouseEvent *event) {
    emit mousePressed(this);
}
void PhysicsWidget::mouseReleaseEvent(QMouseEvent *event) {
    auto pos = event->position();
    emit mouseReleased(this, pos.x(), pos.y());
}
