#include "proceso.h"

Proceso::Proceso(int num, bool texto)  : _modo(new Operacion()), _texto(texto) {
    _nombre = "Proceso " + QString().number(num);
}


Proceso::Proceso(int num, Factoria::Plano tipo, float x, float y, float l)
    : _modo(new Operacion()), _texto(false) {
    _objeto = Factoria::crearForma(tipo, x, y, l);
    _nombre = "Proceso " + QString().number(num);
}

Proceso::Proceso(int num, Factoria::Plano tipo, float largo, float alto)
    : _modo(new Operacion()), _texto(false) {
    _objeto = Factoria::crearForma(tipo, largo, alto);
    _nombre = "Proceso " + QString().number(num);
}

Proceso::Proceso(int num, Factoria::Plano tipo, QString name) : _modo(new Operacion()){
    _objeto = Factoria::crearForma(tipo, name);
    _nombre = "Proceso " + QString().number(num);
    _texto = (tipo == Factoria::Plano::TEXTO);
}

Proceso::Proceso(int num, Factoria::Plano tipo, QString name, float x, float y)
    : _modo(new Operacion()) {
    _objeto = Factoria::crearForma(tipo, name, x, y);
    _nombre = "Proceso " + QString().number(num);
    _texto = (tipo == Factoria::Plano::TEXTO);
}

Proceso::~Proceso() { delete _objeto; }

void Proceso::updateGraphics() {
    if (_objeto)
        _objeto->setGraphics(*_modo);
}

void Proceso::updateOrientation() {
    if (_objeto)
        _objeto->setOrientation(*_modo);
}

Camino Proceso::getGcode() const {
    return _objeto->getGcode((_modo->modo() == modo::VECTOR? false : true), _modo->densidad());
}
