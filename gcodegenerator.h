#ifndef GCODEGENERATOR_H
#define GCODEGENERATOR_H

#include "proceso.h"

class GCodeGenerator {
public:
    GCodeGenerator();
    QString* GenerateGcode(std::vector<Proceso*> lista);

private:
    float _minX= 230, _minY = 230;
    float _maxX = -1, _maxY = -1;

    float _tiempo_s = 0.0;
    QString _gcode;

    void WriteGcode(QString* code);
    void addHeader();
    void addBody(QString* body);
    void addEnd();

    float processBody(QString* gcode, Camino camino, int vel, int pw, int pas);
    float mapPower(float lvl, float power_max);
    float tiempoOperacion(QPointF origen, QPointF destino, float v);

    void clearLimits();
};

#endif // GCODEGENERATOR_H
