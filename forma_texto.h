#ifndef FORMA_TEXTO_H
#define FORMA_TEXTO_H

#include "forma_compleja.h"
#include <QFont>

class Forma_texto : public Forma_compleja {
public:
    Forma_texto(QString texto);
    Forma_texto(QString texto, float x, float y);
    virtual ~Forma_texto();
    Forma *clone() const override;

    // Solo texto
    inline QString texto() const { return _texto; }
    inline bool bold() const { return _fuente.bold(); }
    inline bool italic() const { return _fuente.italic(); }
    inline bool underline() const { return _fuente.underline(); }
    inline float texSize() const { return _fuente.pointSizeF(); }
    inline float angulo() const { return _angulo; }

    void setTexto(const QString &texto);
    inline void setBold(bool bold) { _fuente.setBold(bold); }
    inline void setItalic(bool italic) { _fuente.setItalic(italic); }
    inline void setUnderline(bool underline) { _fuente.setUnderline(underline); }
    inline void setSize(int size) { _fuente.setPointSize(size); }
    inline void setStyle(const QString &fuente) { _fuente.setFamily(fuente); }

    void setAngulo(int newAngulo);


    // Forma
    void setX(float x) override;
    void setY(float y) override;
    void setLargo(float largo) override;
    void setLock(bool close) override;
    void setGraphics(Operacion operacion) override;
    void setAlto(float alto) override {};   // Sin uso
    void setSize(float size) override {};   // Sin uso
    void setOrientation(Operacion operacion) override; // Sin uso

    QGraphicsItem *getRender() const override { return _render; }
    Camino getGcode(bool engrave, int densidad) const override;

private:
    QGraphicsTextItem *_render;
    QString _texto = "Ag WINS";

    int _angulo = 0;
    QFont _fuente;

    //Helper
    void debugMostrarImagen(const QImage &img) const;
};

#endif // FORMA_TEXTO_H
