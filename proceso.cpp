#include "proceso.h"
#include "QJsonObject"
Proceso::Proceso(int num, bool texto)  : _modo(new Operacion()), _texto(texto) {
    _nombre = "Proceso " + QString().number(num);
}

Proceso::Proceso(QString nombre, int potencia, int pasadas, int velocidad, bool visible)
    : _nombre(nombre), _potencia(potencia), _pasadas(pasadas), _velocidad(velocidad),
      _visible(visible), _modo(new Operacion()), _texto(false) {}

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

QJsonObject Proceso::save() {
    QJsonObject proceso;
    proceso["nombre"] = _nombre;
    proceso["velocidad"] = _velocidad;
    proceso["pasadas"] = _pasadas;
    proceso["potencia"] = _potencia;
    proceso["visible"] = _visible;

    proceso["forma"] = _objeto->save();
    proceso["operacion"] = _modo->save();

    return proceso;
}

void Proceso::load(QJsonObject proceso) {
    // Forma
    QJsonObject forma = proceso["forma"].toObject();
    if (forma["tipo"].toString() == "rectangulo" || forma["tipo"].toString() == "elipse") {
        _objeto = Factoria::crearForma((forma["tipo"].toString() == "elipse")
                                           ? Factoria::Plano::ELIPSE
                                           : Factoria::Plano::RECTANGULO,
                                       forma["largo"].toDouble(),
                                       forma["alto"].toDouble());
        _objeto->setX(forma["x"].toDouble());
        _objeto->setY(forma["y"].toDouble());

        _objeto->setLock(forma["fijo"].toBool());
    }

    // Operacion
    QJsonObject operacion = proceso["operacion"].toObject();

    _modo->setInvert(operacion["invertir"].toBool());
    _modo->setShow_original(operacion["original"].toBool());

    if (operacion["simetria"].toString() == "none") _modo->setFlip(0);
    else if (operacion["simetria"].toString() == "vertical") _modo->setFlip(1);
    else if (operacion["simetria"].toString() == "horizontal") _modo->setFlip(2);
    else if(operacion["simetria"].toString() == "ambos") _modo->setFlip(3);


    if (operacion["modo"].toString() == "blanco y negro") {
        _modo->setModo(0);
        _modo->setUmbral(operacion["umbral"].toInt());
        _modo->setDensidad(operacion["densidad"].toInt());
        return;
    }
    if (operacion["modo"].toString() == "vector") {
        _modo->setModo(2);
        _modo->setUmbral(operacion["umbral"].toInt());
        return;
    }

    _modo->setModo(1);
    _modo->setBrillo(operacion["brillo"].toInt());
    _modo->setContraste(operacion["contraste"].toInt());
    _modo->setLvlgris(operacion["lvl"].toInt());
    _modo->setDensidad(operacion["densidad"].toInt());

}
