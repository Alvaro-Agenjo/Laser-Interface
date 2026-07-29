#ifndef LASERVIEW_H
#define LASERVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QPoint>


class LaserView : public QGraphicsView {
    Q_OBJECT
public:
    explicit LaserView(QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    bool _desplazar = false;
    QPoint _lastPos;
};

#endif // LASERVIEW_H
