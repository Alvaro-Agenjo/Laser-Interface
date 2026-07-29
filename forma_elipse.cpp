#include "Forma_elipse.h"
#include "operacion.h"
#include <QGraphicsScene>


Forma_elipse::Forma_elipse(float largo, float alto)  : Forma_Geometrica(largo, alto){
    _render = new QGraphicsEllipseItem(_x, _y, _largo, _alto);
    _render->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable
                      | QGraphicsItem::ItemSendsGeometryChanges);
}

Forma *Forma_elipse::clone() const{
    Forma_elipse *r = new Forma_elipse(_largo, _alto);
    r->setX(_x);
    r->setY(_y);
    return r;
}

Forma_elipse::Forma_elipse() : Forma_elipse(10, 20) {}

Forma_elipse::Forma_elipse(float l) : Forma_elipse(l, l) {}

void Forma_elipse::setX(float x) {
    if (_fixed) return;
    _x = x;
    _render->setX(_x);
}

void Forma_elipse::setY(float y) {
    if (_fixed) return;
    _y = y;
    _render->setY(_y);
}

void Forma_elipse::setLargo(float largo) {
    if (_fixed) return;
    _largo = largo;
    _render->setRect(0, 0, _largo, _alto);
}

void Forma_elipse::setAlto(float alto) {
    if (_fixed) return;
    _alto = alto;
    _render->setRect(0, 0, _largo, _alto);
}

void Forma_elipse::setSize(float scale) {
    if (_fixed) return;
    _largo *= scale;
    _alto *= scale;
    _render->setRect(0, 0, _largo, _alto);
}

void Forma_elipse::setLock(bool close) {
    _fixed = close;
    _render->setFlag(QGraphicsItem::ItemIsMovable, !close);
}

void Forma_elipse::setGraphics(Operacion operacion) {
    if (operacion.show_original()) {
        _render->setPen(QPen(Qt::black, 2));
        _render->setBrush(QBrush(Qt::lightGray));

        return;
    }

    if (operacion.modo() != modo::VECTOR) {
        _render->setPen(Qt::NoPen);
        operacion.invert() ? _render->setBrush(Qt::white) : _render->setBrush(Qt::black);
    }
    else {
        _render->setBrush(Qt::NoBrush);
        _render->setPen(QPen(Qt::black, 0.4));
    }
}

Camino Forma_elipse::getGcode(bool engrave, int densidad) const {
    Camino camino;
    if (engrave) {
        float paso = 1.0 / (float) densidad;
        int max_linea = std::round(_alto * densidad);
        for (int n = 0; n < max_linea + 1; n++) {
            float y = _y + (n * paso);

            if (n % 2) {
                camino.append({{_x + _largo, y}, 0});
                camino.append({{_x, y}, 255});
            } else {
                camino.append({{_x, y}, 0});
                camino.append({{_x + _largo, y}, 255});
            }
        }
    } else {
        camino.append({{_x, _y}, 1});
        camino.append({{_x + _largo, _y}, 255});
        camino.append({{_x + _largo, _y + _alto}, 255});
        camino.append({{_x, _y + _alto}, 255});
        camino.append({{_x, _y}, 255});
    }

    return camino;
}
