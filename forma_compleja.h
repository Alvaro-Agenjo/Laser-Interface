#ifndef FORMA_COMPLEJA_H
#define FORMA_COMPLEJA_H

#include "forma.h"

class Forma_compleja : public Forma {
public:
    Forma_compleja() : Forma(){};
    virtual ~Forma_compleja() = default;
    virtual Forma *clone() const = 0;

    virtual void setX(float x) = 0;
    virtual void setY(float y) = 0;
    virtual void setLargo(float largo) = 0;
    virtual void setAlto(float alto) = 0;
    virtual void setSize(float scale) = 0;
    virtual void setLock(bool close) = 0;

    virtual void setGraphics(Operacion operacion) = 0;
    virtual void setOrientation(Operacion operacion)  = 0;

    virtual QGraphicsItem *getRender() const = 0;
    virtual Camino getGcode(bool engrave, int densidad) const = 0;


protected:
    QImage aplicarSobel(const QImage *img) const; //Sin uso
    Camino raster(const QImage *img, int densidad) const;
    Camino vector(const QImage *img, int densidad) const; //Sin uso

    Camino vectorCV(const QImage *img, int densidad) const;
    QImage renderCV(QImage *img);
};

#endif // FORMA_COMPLEJA_H
