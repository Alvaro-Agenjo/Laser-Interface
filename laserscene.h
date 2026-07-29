#ifndef LASERSCENE_H
#define LASERSCENE_H

#include <QGraphicsScene>
#include <QPainter>

class LaserScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit LaserScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = nullptr)
        : QGraphicsScene(x, y, width, height, parent) {}

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void itemMoved(QGraphicsItem *item, QPointF pos);
};

#endif // LASERSCENE_H
