#ifndef FORMA_RECTANGULO_H
#define FORMA_RECTANGULO_H

#include "forma_geometrica.h"

class Forma_rectangulo : public Forma_Geometrica {
public:
    Forma_rectangulo();
    Forma_rectangulo(float l);
    Forma_rectangulo(float largo, float alto);
    Forma* clone() const override;

    void setX(float x) override;
    void setY(float y) override;
    void setLargo(float largo) override;
    void setAlto(float alto) override;
    void setSize(float scale) override;
    void setLock(bool close) override;
    void setGraphics(Operacion operacion) override;


    inline QGraphicsItem *getRender() const override {return _render;}
    Camino getGcode(bool engrave, int densidad) const override;
private:
    QGraphicsRectItem *_render;
};

#endif // FORMA_RECTANGULO_H
