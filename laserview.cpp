#include "laserview.h"
#include <QScrollBar>

LaserView::LaserView(QWidget *parent) : QGraphicsView(parent) {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scale(1, -1);
}

void LaserView::wheelEvent(QWheelEvent *event) {

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    double scaleFactor = (event->angleDelta().y() > 0) ? 1.15 : 0.85;
    scale(scaleFactor, scaleFactor);
    event->accept();
}

void LaserView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        _desplazar = true;
        _lastPos = event->pos(); // Guardamos la posición inicial
        setCursor(Qt::ClosedHandCursor); // Opcional: cambio de cursor
        event->accept();
        return;
    }
    QGraphicsView::mousePressEvent(event);
}

void LaserView::mouseMoveEvent(QMouseEvent *event) {
    if (_desplazar) {
        // Calculamos la diferencia de movimiento
        QPoint delta = event->pos() - _lastPos;
        _lastPos = event->pos();

        // Aplicamos el desplazamiento a la vista
        // Nota: Si el eje Y está invertido, el movimiento vertical
        // se comportará de forma natural con esta lógica sencilla
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());

        event->accept();
        return;
    }
    QGraphicsView::mouseMoveEvent(event);
}

void LaserView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        _desplazar = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    QGraphicsView::mouseReleaseEvent(event);
}
