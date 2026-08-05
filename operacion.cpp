#include "operacion.h"
#include "QJsonObject"

Operacion::Operacion() {}

void Operacion::setModo(int modo) {
    switch (modo) {
    case 0:
        _modo = modo::BLANCO_NEGRO;
        break;
    case 1:
        _modo = modo::GRIS;
        break;
    case 2:
        _modo = modo::VECTOR;
        break;
    }
}

void Operacion::setFlip(int flip) {
    switch (flip) {
    case 0:
        _flip = flip::NO_FLIP;
        break;
    case 1:
        _flip = flip::VERTICAL;
        break;
    case 2:
        _flip = flip::HORIZONTAL;
        break;
    case 3:
        _flip = flip::BOTH;
        break;
    }
}

void Operacion::setUmbral(int umbral) {
    if (_modo == modo::GRIS) return;
    _umbral = umbral;
}

void Operacion::setDensidad(int densidad) {
    if (_modo == modo::VECTOR) return;
    _densidad = densidad;
}

void Operacion::setBrillo(int brillo) {
    if (_modo != modo::GRIS) return;
    _brillo = brillo;
}
void Operacion::setContraste(int contraste) {
    if (_modo != modo::GRIS) return;
    _contraste = contraste;
}
void Operacion::setLvlgris(int lvlgris) {
    if (_modo != modo::GRIS) return;
    _lvlgris = lvlgris;
}

QJsonObject Operacion::save() const {
    QJsonObject operacion;

    operacion["original"] = _show_original;
    operacion["invertir"] = _invert;
    switch (_flip) {
    case flip::NO_FLIP:
        operacion["simetria"] = "none";
    case flip::VERTICAL:
        operacion["simetria"] = "vertical";
    case flip::HORIZONTAL:
        operacion["simetria"] = "horizontal";
    case flip::BOTH:
        operacion["simetria"] = "ambos";
    }

    switch (_modo) {
    case modo::BLANCO_NEGRO:
        operacion["modo"] = "blanco y negro";
        operacion["umbral"] = _umbral;
        operacion["densidad"] = _densidad;
        return operacion;
    case modo::GRIS:
        operacion["modo"] = "gris";
        operacion["brillo"] = _brillo;
        operacion["contraste"] = _contraste;
        operacion["lvl"] = _lvlgris;
        operacion["densidad"] = _densidad;
        return operacion;
    case modo::VECTOR:
        operacion["modo"] = "vector";
        operacion["umbral"] = _umbral;
        return operacion;

    }
}
