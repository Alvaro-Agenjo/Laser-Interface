#ifndef FORMA_ELIPSE_H
#define FORMA_ELIPSE_H

#include "forma_geometrica.h"

class Forma_elipse : public Forma_Geometrica {
public:
    Forma_elipse();

    Forma_elipse(float r);
    Forma_elipse(float r1, float r2);
    Forma* clone() const override;

    void setX(float x) override;
    void setY(float y) override;
    void setLargo(float r1) override;   // r1 = largo
    void setAlto(float r2) override;    // r2 = alto
    void setSize(float scale) override;
    void setLock(bool close) override;
    void setGraphics(Operacion operacion) override;


    inline QGraphicsItem *getRender() const override {return _render;}
    Camino getGcode(bool engrave, int densidad) const override;

    QJsonObject save() const override;
private:
    QGraphicsEllipseItem *_render;
};

#endif // FORMA_ELIPSE_H
