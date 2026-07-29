#include "forma_texto.h"
#include "operacion.h"
#include <QGraphicsScene>
#include <QPainter>
#include <QTextDocument>
void exportPixelsToCSV(const QImage &image, const QString &filename);

Forma_texto::Forma_texto(QString texto) : Forma_texto(texto, 0, 0) {}

Forma_texto::Forma_texto(QString texto, float x, float y) : Forma_compleja(), _texto(texto) {
    _x = x, _y = y;
    _render = new QGraphicsTextItem(_texto);
    _render->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

    // Inicializar dimensiones base
    //_original = _render->boundingRect();
    // _marco = new QGraphicsRectItem(_original, _render);
    // _marco->setBrush(Qt::NoBrush);
    // _marco->setPen(QPen(Qt::black, 0.4));

    _largo = _render->boundingRect().width();
    _alto = _render->boundingRect().height();
    _render->setTransform(QTransform().scale(1, -1));
    _render->setPos(_x, _y + _alto);
}

Forma_texto::~Forma_texto() {
    delete _render;
    // delete  _marco;
}

Forma *Forma_texto::clone() const {
    Forma_texto *f = new Forma_texto(_texto);

    f->_x = _x;
    f->_y = _y;
    f->_largo = _largo;
    f->_alto = _alto;

    f->_angulo = _angulo;
    f->_fuente = _fuente;

    return f;
}

Camino Forma_texto::getGcode(bool engrave, int densidad) const {
    densidad = 10;
    QRectF textRect(_x * densidad,
                    _y * densidad,
                    _render->boundingRect().width() * densidad,
                    _render->boundingRect().height() * densidad);
    // = _render->boundingRect();
    // textRect.setHeight(textRect.height() * densidad);
    // textRect.setWidth(textRect.width() * densidad);

    // 2. Crear la imagen (Formato A8 es ideal para mapas de profundidad o máscaras)
    QImage img(textRect.size().toSize(), QImage::Format_Grayscale8);
    img.fill(255);

    // 3. Configurar el Painter
    QPainter painter(&img);
    painter.setPen(QPen(Qt::black, 2));
    painter.setRenderHint(QPainter::Antialiasing, false); // Fundamental para suavizar curvas
    painter.setRenderHint(QPainter::TextAntialiasing, false);

    QFont f = _fuente;
    f.setPixelSize(densidad * f.pointSizeF());
    painter.setFont(f);
    painter.drawText(0, 0, textRect.width(), textRect.height(), Qt::AlignCenter , _texto);
    painter.end();

    QTransform transform;
    transform.scale(1, -1);
    img = img.transformed(transform, Qt::FastTransformation);

    // debugMostrarImagen(img);
    for (int y = 0; y < img.height(); ++y) {
        uchar *line = img.scanLine(y);
        for (int x = 0; x < img.width(); ++x) {
            float pixel = line[x];
            line[x] = (pixel < 127) ? 0 : 255;
        }
    }
    // debugMostrarImagen(img);
    exportPixelsToCSV(img, "img 1");
    if (engrave) return Forma_compleja::raster(&img, densidad);
    return Forma_compleja::vectorCV(&img, densidad);
}
void Forma_texto::setX(float x) {
    if (_fixed) return;
    _x = x;
    _render->setX(_x);
}

void Forma_texto::setY(float y) {
    if (_fixed) return;
    _y = y;
    _render->setY(_y + _alto);
}

void Forma_texto::setLargo(float largo) {
    // if (_fixed) return;
    // _largo = largo;


    // _render->setY(_y + _alto);
}

// void Forma_texto::setAlto(float alto) {
//     // if (_fixed) return;
//     // _alto = alto;
//     // _render.setT
// }

// void Forma_texto::setSize(float scale) {
//     if (_fixed) return;
//     _largo *= scale;
//     _alto *= scale;
//     _marco->setRect(_render->boundingRect());
//     _render->setTextWidth(_largo);
// }

void Forma_texto::setLock(bool close) {
    _fixed = close;
    _render->setFlag(QGraphicsItem::ItemIsMovable, !close);
}

void Forma_texto::setGraphics(Operacion operacion) {
    _render->setFont(_fuente);
    _render->document()->adjustSize();
    // _marco->setRect(_render->boundingRect());
    _alto = _render->boundingRect().height();
    _largo = _render->boundingRect().width();

    setY(_y);
}

void Forma_texto::setOrientation(Operacion operacion) {}

// void Forma_texto::setOrientation(Operacion operacion) {
//     // switch (operacion.flip()) {
//     // case flip::NO_FLIP:
//     //     _render->setTransform(
//     //         QTransform().scale(_largo / _original.width(), -_alto / _original.height()));
//     //     _render->setPos(_x,  _y + _alto);
//     //     break;

//     // case flip::VERTICAL:
//     //     _render->setTransform(
//     //         QTransform().scale(_largo / _original.width(), _alto / _original.height()));
//     //     _render->setPos(_x,  _y);
//     //     break;

//     // case flip::HORIZONTAL:
//     //     _render->setTransform(
//     //         QTransform().scale(-_largo / _original.width(), -_alto / _original.height()));
//     //     _render->setPos(_x + _largo, _y + _alto);
//     //     break;

//     // case flip::BOTH:
//     //     _render->setTransform(
//     //         QTransform().scale(-_largo / _original.width(), _alto / _original.height()));
//     //     _render->setPos(_x + _largo,  _y);
//     //     break;
//     // }
// }

void Forma_texto::setTexto(const QString &texto) {
    _texto = texto;
    _render->setPlainText(texto);
}

void Forma_texto::setAngulo(int angulo) {
    _angulo = angulo;
    _render->setTransformOriginPoint(_largo / 2.0, _alto / 2.0);
    _render->setRotation(angulo);
}




#include <QLabel>
#include <QPixmap>
#include <QDialog>
#include <QVBoxLayout>

void Forma_texto::debugMostrarImagen(const QImage &img) const {
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle("Debug Raster");
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    QLabel *label = new QLabel();

    // Escalamos la imagen para verla mejor (pixel art style)
    label->setPixmap(QPixmap::fromImage(img).scaled(img.size() * 5, Qt::KeepAspectRatio, Qt::FastTransformation));

    layout->addWidget(label);
    dialog->exec();
    delete dialog;
}

#include <QFile>
void exportPixelsToCSV(const QImage &image, const QString &filename) {
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (int y = 0; y < image.height(); ++y) {
            for (int x = 0; x < image.width(); ++x) {
                // Obtenemos la intensidad (por ejemplo, valor de gris)
                int pixelValue = qGray(image.pixel(x, y));
                out << pixelValue;
                if (x < image.width() - 1) out << ",";
            }
            out << "\n";
        }
        file.close();
    }
}
