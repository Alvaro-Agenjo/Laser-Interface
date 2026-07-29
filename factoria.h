#ifndef FACTORIA_H
#define FACTORIA_H

#include "forma_rectangulo.h"
#include "forma_imagen.h"
#include "forma_texto.h"
#include "forma_png.h"
#include "forma_elipse.h"
class Factoria {
public:
    Factoria();

    enum class Plano { RECTANGULO, ELIPSE, IMG, PNG, TEXTO };

    static Forma* crearForma(Plano tipo, QString name = "");
    static Forma* crearForma(Plano tipo, QString name, float x, float y );
    static Forma* crearForma(Plano tipo, float sx, float sy);
    static Forma* crearForma(Plano tipo, float x, float y, float l);
};

#endif // FACTORIA_H
