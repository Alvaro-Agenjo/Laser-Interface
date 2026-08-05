#include "forma_rectangulo.h"
#include "operacion.h"
#include <QGraphicsScene>
#include "QJsonObject"


Forma_rectangulo::Forma_rectangulo(float largo, float alto)  : Forma_Geometrica(largo, alto){
    _render = new QGraphicsRectItem(_x, _y, _largo, _alto);
    _render->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable
                      | QGraphicsItem::ItemSendsGeometryChanges);
}

Forma *Forma_rectangulo::clone() const{
    Forma_rectangulo *r = new Forma_rectangulo(_largo, _alto);
    r->setX(_x);
    r->setY(_y);
    return r;
}

Forma_rectangulo::Forma_rectangulo() : Forma_rectangulo(10, 20) {}

Forma_rectangulo::Forma_rectangulo(float l) : Forma_rectangulo(l, l) {}

void Forma_rectangulo::setX(float x) {
    if (_fixed) return;
    _x = x;
    _render->setX(_x);
}

void Forma_rectangulo::setY(float y) {
    if (_fixed) return;
    _y = y;
    _render->setY(_y);
}

void Forma_rectangulo::setLargo(float largo) {
    if (_fixed) return;
    _largo = largo;
    _render->setRect(0, 0, _largo, _alto);
}

void Forma_rectangulo::setAlto(float alto) {
    if (_fixed) return;
    _alto = alto;
    _render->setRect(0, 0, _largo, _alto);
}

void Forma_rectangulo::setSize(float scale) {
    if (_fixed) return;
    _largo *= scale;
    _alto *= scale;
    _render->setRect(0, 0, _largo, _alto);
}

void Forma_rectangulo::setLock(bool close) {
    _fixed = close;
    _render->setFlag(QGraphicsItem::ItemIsMovable, !close);
}

void Forma_rectangulo::setGraphics(Operacion operacion) {
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

Camino Forma_rectangulo::getGcode(bool engrave, int densidad) const {
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

QJsonObject Forma_rectangulo::save() const {
    QJsonObject forma;
    forma["tipo"] = "rectangulo";
    return Forma_Geometrica::save(forma);
}
