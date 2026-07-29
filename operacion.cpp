#include "operacion.h"

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
