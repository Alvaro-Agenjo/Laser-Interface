#ifndef PROCESO_H
#define PROCESO_H

#include "forma.h"
#include "operacion.h"
#include "factoria.h"


class Proceso {
public:
    Proceso(int num, bool texto);
    Proceso(int num, Factoria::Plano tipo, float x, float y, float l);
    Proceso(int num, Factoria::Plano tipo, float largo = 10, float _alto = 10);
    Proceso(int num, Factoria::Plano tipo, QString name);
    Proceso(int num, Factoria::Plano tipo, QString name, float x, float y);
    ~Proceso();

    inline void setProcessName(QString nombre) { _nombre = nombre; }
    inline void setPasadas(int pasadas){ _pasadas = pasadas; }
    inline void setPotencia(int potencia) { _potencia = potencia; }
    inline void setVelocidad(int velocidad) { _velocidad = velocidad; }

    inline void setObjeto(Forma* objeto)  { _objeto = objeto; }
    inline void setOperacion(Operacion operacion) {_modo = &operacion;}

    inline QString getProcessName() const { return _nombre; };
    inline int getPasadas() const  { return _pasadas; }
    inline int getPotencia() const { return _potencia; }
    inline int getVelocidad() const{ return _velocidad; }


    inline Forma *getObjeto() const { return _objeto; }
    inline Operacion *getOperation() const { return _modo; }
    inline QGraphicsItem* getRender() { return _objeto->getRender(); }


    void updateGraphics();
    void updateOrientation();
    Camino getGcode() const;

    inline bool EsTexto() { return _texto; }

private:
    int _potencia = 80;
    int _pasadas = 1;
    int _velocidad = 1500;

    bool _texto;

    Operacion *_modo;
    Forma *_objeto;
    QString _nombre;
};

#endif // PROCESO_H
