#ifndef PUNTOSPASO_H
#define PUNTOSPASO_H

#include <QPointF>
#include <QList>

struct PuntodePaso {
    QPointF punto;
    int power;
};

typedef QList<PuntodePaso> Camino;
#endif // PUNTOSPASO_H
