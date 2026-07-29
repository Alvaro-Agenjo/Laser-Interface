#include "gcodegenerator.h"

GCodeGenerator::GCodeGenerator() {}

QString *GCodeGenerator::GenerateGcode(std::vector<Proceso *> lista) {
    _gcode.clear();
    QString cuerpo;
    for (const auto *p : lista) {
        int velocidad = p->getVelocidad();
        int pasadas = p->getPasadas();
        int potencia = p->getPotencia();

        Camino camino = p->getGcode();
        QString gcode_camino;

        _tiempo_s += processBody(&gcode_camino, camino, velocidad, potencia, pasadas);

        cuerpo.append(gcode_camino);

        float x, y, sx, sy;
        auto forma = p->getObjeto();
        x = forma->getX();
        y = forma->getY();
        sx = forma->getLargo();
        sy = forma->getAlto();

        if (x < _minX) _minX = x;
        if (y < _minY) _minY = y;
        if (x + sx > _maxX) _maxX = x + sx;
        if (y + sy > _maxY) _maxY = y + sy;
    }

    WriteGcode(&cuerpo);

    clearLimits();
    return &_gcode;
}

void GCodeGenerator::WriteGcode(QString *code) {
    addHeader();
    addBody(code);
    addEnd();
}

void GCodeGenerator::addHeader() {
    QString header;
    header.append(";Header Start\n");
    header.append(";estimated_time(s): " + QString().number(_tiempo_s, 'f', 3) + "\n");
    header.append(";MAXX: " + QString().number(_maxX, 'f', 3) + "\n");
    header.append(";MAXY: " + QString().number(_maxY, 'f', 3) + "\n");
    header.append(";MINX: " + QString().number(_minX, 'f', 3) + "\n");
    header.append(";MINY: " + QString().number(_minY, 'f', 3) + "\n");
    header.append(";Header End\n");

    header.append("\n");
    header.append("\n");

    header.append("G92 X0 Y0 Z0\n");
    header.append("G90\n");
    header.append("M3 I\n");

    _gcode.append(header);
}

void GCodeGenerator::addBody(QString *body) {
    _gcode.append(*body);
}

void GCodeGenerator::addEnd() {
    QString end;
    end.append("M5 I\n");
    end.append("G0 F3000 X0 Y0\n");

    _gcode.append(end);
}

float GCodeGenerator::processBody(QString *gcode, Camino camino, int vel, int pw, int pas) {
    QString gBody;
    gcode->append("G0 F3000\n");
    gcode->append("G1 F" + QString().number(vel) + "\n");
    gcode->append(("G0 X" + QString().number(camino.front().punto.x(), 'f', 3))
                  + (" Y" + QString().number(camino.front().punto.y(), 'f', 3)) + "\n");

    float tiempo_s = 0.0;
    QPointF last = camino.front().punto;
    for (const auto pt : camino) {
        float potencia = mapPower(pt.power, pw);
        if (potencia < 100){
            gBody.append(("G0 X" + QString().number(pt.punto.x(), 'f', 3))
                         + (" Y" + QString().number(pt.punto.y(), 'f', 3)) + "\n");
            tiempo_s += tiempoOperacion(last, pt.punto, 3000.0);
        }
        else{
            gBody.append(("G1 X" + QString().number(pt.punto.x(), 'f', 3))
                         + (" Y" + QString().number(pt.punto.y(), 'f', 3))
                         + (" S" + QString().number(potencia)) + "\n");
            tiempo_s += tiempoOperacion(last, pt.punto, vel);
        }
        last = pt.punto;
    }

    for (int n = 0; n < pas; n++) {
        gcode->append(gBody);
    }
    tiempo_s *= pas;
    return tiempo_s;
}

float GCodeGenerator::mapPower(float lvl, float power_max) {
    return ((power_max * 10.0 / 255.0) * lvl);
}

void GCodeGenerator::clearLimits() {
    _minX = 230, _minY = 230;
    _maxX = -1, _maxY = -1;

    _tiempo_s = 0.0;
}

float GCodeGenerator::tiempoOperacion(QPointF origen, QPointF destino, float v) {
    QPointF diff = destino - origen;
    float espacio = sqrt(pow(diff.x(), 2) + pow(diff.y(), 2));
    return espacio / v;
}
