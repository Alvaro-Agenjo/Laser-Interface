#ifndef FORMA_H
#define FORMA_H

#include <QGraphicsItem>
#include "PuntosPaso.h"

class Operacion;
class Forma {

public:
    Forma();
    Forma(float largo, float alto);
    virtual ~Forma() = default;
    virtual Forma* clone() const = 0;


    virtual void setX(float x) = 0;
    virtual void setY(float y) = 0;
    virtual void setLargo(float largo) = 0;
    virtual void setAlto(float alto) = 0;
    virtual void setSize(float scale) = 0;
    virtual void setLock(bool close) = 0;

    virtual void setGraphics(Operacion operacion) = 0;
    virtual void setOrientation(Operacion operacion) = 0;

    inline float getX() const { return _x; }
    inline float getY() const { return _y; }
    inline float getLargo() const { return _largo; }
    inline float getAlto() const { return _alto; }
    inline bool getLock() const {return _fixed; }

    virtual QGraphicsItem *getRender() const = 0;
    virtual Camino getGcode(bool engrave, int densidad) const = 0;

    virtual QJsonObject save() const = 0;

protected:
    float _x = 0, _y = 0;
    float _largo, _alto;

    bool _fixed = false;
};

#endif // FORMA_H
