#include "forma_imagen.h"
#include "QMessageBox"
#include "operacion.h"
#include "CONSTANTS.h"

Forma_imagen::Forma_imagen() : Forma_compleja() {}

Forma_imagen::Forma_imagen(const QString &filePath) : Forma_compleja() {
    if (!_raw_pixmap.load(filePath)) {
        _x = -1;
        _raw_pixmap = QPixmap(10, 10);
        _raw_pixmap.fill(Qt::red);
    } // Carga JPG, PNG, BMP, etc. automáticamente


    // Ajustar al maximo permitido para evitar calculos pesados (muchos pixeles)
    _raw_pixmap = _raw_pixmap.scaled(std::min(TableX * maxDensity, _raw_pixmap.width()),
                       std::min(TableY * maxDensity, _raw_pixmap.height()),
                       Qt::AspectRatioMode::KeepAspectRatio);
    // Inicializar dimensiones desde la imagen
    _largo = _raw_pixmap.width();
    _alto = _raw_pixmap.height();

    _raw_pixmap.transformed(QTransform().scale(1, -1), Qt::FastTransformation);
    _pixmap = _raw_pixmap;
    _original_pixmap = _raw_pixmap;
    _render = new QGraphicsPixmapItem(_pixmap);
    _render->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable
                      | QGraphicsItem::ItemSendsGeometryChanges);
}

Forma *Forma_imagen::clone() const {
    Forma_imagen *f = new Forma_imagen();
    f->_raw_pixmap = _raw_pixmap;
    f->_original_pixmap = _original_pixmap;
    f->_pixmap = _pixmap;
    f->_render = new QGraphicsPixmapItem(_pixmap);
    f->_render->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable
                         | QGraphicsItem::ItemSendsGeometryChanges);


    f->setX(_x);
    f->setY(_y);
    f->setLargo(_largo);
    f->setAlto(_alto);

    return f;
}

Camino Forma_imagen::getGcode(bool engrave, int densidad) const {

    QTransform transform;
    transform.scale(1, -1);
    QPixmap pixels = _pixmap.transformed(transform, Qt::FastTransformation);


    Camino camino;
    if (engrave) {
        pixels = pixels.scaledToWidth(_largo * densidad);
        QImage img = pixels.toImage().convertToFormat(QImage::Format_Grayscale8);

        return Forma_compleja::raster(&img, densidad);
    } else {
        densidad = 8;
        pixels = pixels.scaledToWidth(_largo * densidad);
        QImage img = pixels.toImage().convertToFormat(QImage::Format_Grayscale8);

        // return Forma_compleja::vector(&img, densidad);
        return Forma_compleja::vectorCV(&img, densidad);
    }
}
void Forma_imagen::setX(float x) {
    if (_fixed) return;
    _x = x;
    _render->setX(_x);
}

void Forma_imagen::setY(float y) {
    if (_fixed) return;
    _y = y;
    _render->setY(_y+_alto);
}

void Forma_imagen::setLargo(float largo) {
    if (_fixed) return;
    _largo = largo;
    _render->setTransform(QTransform::fromScale(_largo/_raw_pixmap.width(), -_alto/_raw_pixmap.height()));
}

void Forma_imagen::setAlto(float alto) {
    if (_fixed) return;
    _alto = alto;
    _render->setTransform(QTransform().scale(_largo/_raw_pixmap.width(), -_alto/_raw_pixmap.height()));
    _render->setY(_y + _alto);
}

void Forma_imagen::setSize(float scale) {
    if (_fixed) return;
    _largo *= scale;
    _alto *= scale;
    _render->setTransform(
        QTransform().scale(_largo / _raw_pixmap.width(), -_alto / _raw_pixmap.height()));
    _render->setY(_y + _alto);
}

void Forma_imagen::setLock(bool close) {
    _fixed = close;
    _render->setFlag(QGraphicsItem::ItemIsMovable, !close);
}

void Forma_imagen::setGraphics(Operacion operacion) {
    if (operacion.show_original()) {
        _render->setPixmap(_original_pixmap);
        return;
    }

    QImage img = _original_pixmap.toImage().convertToFormat(QImage::Format_Grayscale8);

    // 2. Aplicar Niveles de Gris y Contraste
    for (int y = 0; y < img.height(); ++y) {
        uchar *line = img.scanLine(y);
        for (int x = 0; x < img.width(); ++x) {
            float pixel = line[x];
            switch (operacion.modo()) {
            case modo::BLANCO_NEGRO:
                pixel = (pixel < operacion.umbral()) ? 0 : 255;
                break;
            case modo::GRIS:
                pixel = (pixel - 128) * operacion.contraste() / 50.0 + 128
                        + (operacion.brillo() - 50.0) * 2.55f;
                pixel = std::floorf(pixel / (256.0 / operacion.lvlgris()))
                        * (256.0 / operacion.lvlgris());
                break;
            case modo::VECTOR:
                pixel = (pixel < operacion.umbral()) ? 0 : 255;
                break;
            }
            if (operacion.invert()) pixel = 255.0 - pixel;
            line[x] = qBound(0, (int) pixel, 255);
        }
    }
    if (operacion.modo() == modo::VECTOR){
        img = renderCV(&img);
    }
    _pixmap = QPixmap::fromImage(img);
    _render->setPixmap(_pixmap);
}

void Forma_imagen::setOrientation(Operacion operacion) {
    QTransform transform;
    switch (operacion.flip()) {
    case flip::NO_FLIP:
        transform.scale(1, 1);
        break;

    case flip::VERTICAL:
        transform.scale(1, -1);
        break;

    case flip::HORIZONTAL:
        transform.scale(-1, 1);
        break;

    case flip::BOTH:
        transform.scale(-1, -1);
        break;
    }

    _original_pixmap = _raw_pixmap.transformed(transform, Qt::FastTransformation);
    _pixmap = _raw_pixmap.transformed(transform, Qt::FastTransformation);

    setGraphics(operacion);
    // switch (flip) {
    // case 0:
    //     _render->setTransform(
    //         QTransform().scale(_largo / _raw_pixmap.width(), -_alto / _raw_pixmap.height()));
    //     _render->setPos(_x,  _y + _alto);
    //     break;

    // case 1:
    //     _render->setTransform(
    //         QTransform().scale(_largo / _raw_pixmap.width(), _alto / _raw_pixmap.height()));
    //     _render->setPos(_x,  _y);
    //     break;

    // case 2:
    //     _render->setTransform(
    //         QTransform().scale(-_largo / _raw_pixmap.width(), -_alto / _raw_pixmap.height()));
    //     _render->setPos(_x + _largo, _y + _alto);
    //     break;

    // case 3:
    //     _render->setTransform(
    //         QTransform().scale(-_largo / _raw_pixmap.width(), _alto / _raw_pixmap.height()));
    //     _render->setPos(_x + _largo,  _y);
    //     break;
    // }
}