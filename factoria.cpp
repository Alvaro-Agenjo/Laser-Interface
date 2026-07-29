#include "factoria.h"

Factoria::Factoria() {}

Forma *Factoria::crearForma(Plano tipo, QString name) {
    switch (tipo) {
    case Plano::RECTANGULO:
        return new Forma_rectangulo();
    case Plano::ELIPSE:
        return new Forma_elipse();
    case Plano::IMG:
        return new Forma_imagen(name);
    case Plano::PNG:
        return new Forma_png(name);
    case Plano::TEXTO:
        return new Forma_texto(name);
    default:
        return nullptr;
    }
}

Forma *Factoria::crearForma(Plano tipo, QString name, float x, float y) {
    if(tipo == Plano::TEXTO)
        return new Forma_texto(name, x, y);
    else
        return nullptr;
}

Forma *Factoria::crearForma(Plano tipo, float sx, float sy) {
    switch (tipo) {
    case Plano::RECTANGULO:
        return new Forma_rectangulo(sx, sy);
    case Plano::ELIPSE:
        return new Forma_elipse(sx, sy);
    default:
        return nullptr;
    }
}

Forma *Factoria::crearForma(Plano tipo, float x, float y, float l) {
    switch (tipo) {
    case Plano::RECTANGULO: {
        Forma_rectangulo *r = new Forma_rectangulo(l);
        r->setX(x);
        r->setY(y);
        return r;
    }
    case Plano::ELIPSE: {
        Forma_elipse *r = new Forma_elipse(l);
        r->setX(x);
        r->setY(y);
        return r;
    }
    default:
        return nullptr;
    }
}
