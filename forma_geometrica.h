#ifndef FORMA_GEOMETRICA_H
#define FORMA_GEOMETRICA_H

#include "forma.h"

class Forma_Geometrica : public Forma {
public:
    Forma_Geometrica(float largo, float alto);
    virtual ~Forma_Geometrica() = default;
    virtual Forma *clone() const = 0;

    virtual void setX(float x) = 0;
    virtual void setY(float y) = 0;
    virtual void setLargo(float largo) = 0;
    virtual void setAlto(float alto) = 0;
    virtual void setSize(float scale) = 0;
    virtual void setLock(bool close) = 0;
    virtual void setGraphics(Operacion operacion) = 0;

    void setOrientation(Operacion operacion) override;

    virtual QGraphicsItem *getRender() const = 0;
    virtual Camino getGcode(bool engrave, int densidad) const = 0;

};

#endif // FORMA_GEOMETRICA_H
