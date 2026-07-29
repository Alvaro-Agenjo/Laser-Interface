#include "laserscene.h"
#include <QLineF>
#include <QVarLengthArray>
#include <qgraphicsitem.h>
#include <qgraphicssceneevent.h>
#include "CONSTANTS.h"

void LaserScene::drawBackground(QPainter *painter, const QRectF &rect) {
    QGraphicsScene::drawBackground(painter, rect);

    const int gridSize = 10; // Graduación cada 10 unidades
    const int limit = 220;

    // Configuración del estilo del grid
    QPen pen(Qt::lightGray, 0.5, Qt::DashLine);
    painter->setPen(pen);

    // Dibujar el grid
    for (int i = 0; i <= TableX; i += gridSize) {
        painter->drawLine(i, 0, i, TableX);
    }
    for (int i = 0; i <= TableY; i += gridSize) {
            painter->drawLine(0, i, TableY, i);
    }


    // Dibujar números (Graduación)
    painter->setPen(Qt::red);
    QFont font("Arial", 3);
    painter->setFont(font);

    /*Inversion*/
    painter->save();
    painter->scale(1, -1);

    for (int i = 0; i <= TableX; i += gridSize) {
        // Etiquetas eje X (inferior o superior)
        painter->drawText(i, 8, QString::number(i));
    }
    painter->setPen(Qt::green);
    for (int i = 0; i <= TableY; i += gridSize) {
        // Etiquetas eje Y (lateral)
        painter->drawText(-10, 5 - i, QString::number(i));
    }

    painter->restore();
    // Opcional: Dibujar un borde grueso para delimitar el área de trabajo
    painter->setPen(QPen(Qt::green, 2));
    painter->drawLine(0, 0, 0, TableY);
    painter->setPen(QPen(Qt::red, 2));
    painter->drawLine(0, 0, TableX, 0);
}

void LaserScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    // Llamamos a la implementación base para que siga moviendo los objetos
    QGraphicsScene::mouseReleaseEvent(event);

    // Verificamos qué item se movió
    QGraphicsItem *item = itemAt(event->scenePos(), QTransform());

    if (item) {
        // Emitimos una señal desde la ESCENA hacia MainWindow
        emit itemMoved(item, item->pos());
    }
}