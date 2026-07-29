#include "forma_geometrica.h"
#include "operacion.h"

Forma_Geometrica::Forma_Geometrica(float largo, float alto)
    : Forma(largo, alto) {}

void Forma_Geometrica::setOrientation(Operacion operacion) {
    QGraphicsItem *_render = getRender();
    QRectF rect = _render->boundingRect();
    QPointF center = rect.center();

    _render->setTransform(QTransform());

    QTransform transform;
    transform.translate(center.x(), center.y());

    switch (operacion.flip()) {
    case flip::VERTICAL:
        transform.scale(1, -1);
        break;
    case flip::HORIZONTAL:
        transform.scale(-1, 1);
        break;
    case flip::BOTH:
        transform.scale(-1, -1);
        break;
    default:
        break;
    }
    transform.translate(-center.x(), -center.y());
    _render->setTransform(transform);
}