#ifndef FORMA_IMAGEN_H
#define FORMA_IMAGEN_H

#include "forma_compleja.h"

class Forma_imagen : public Forma_compleja {
public:
    Forma_imagen();
    Forma_imagen(const QString &filePath);
    virtual ~Forma_imagen() = default;
    Forma *clone() const override;

    void setX(float x) override;
    void setY(float y) override;
    void setLargo(float largo) override;
    void setAlto(float alto) override;
    void setSize(float scale) override;
    void setLock(bool close) override;

    void setGraphics(Operacion operacion) override;
    void setOrientation(Operacion operacion) override;

    QGraphicsItem* getRender() const override { return _render; }
    Camino getGcode(bool engrave, int densidad) const override;

protected:
    QGraphicsPixmapItem *_render;
    QPixmap _raw_pixmap, _original_pixmap, _pixmap;
};

#endif // FORMA_IMAGEN_H
