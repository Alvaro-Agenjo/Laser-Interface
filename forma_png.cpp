#include "forma_png.h"

Forma_png::Forma_png() : Forma_imagen() {}

Forma_png::Forma_png(const QString &filePath) : Forma_imagen(filePath) {
    QImage img = _raw_pixmap.toImage().convertToFormat(QImage::Format_ARGB32);

    // 2. Rellenar los píxeles transparentes (alfa == 0) con color blanco
    for (int y = 0; y < img.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(img.scanLine(y));
        for (int x = 0; x < img.width(); ++x) {
            if (qAlpha(line[x]) != 0) continue;

            line[x] = qRgba(255, 255, 255, 1); // Blanco opaco
        }
    }

    // 3. Volver a pasarlo a QPixmap para que _raw_pixmap ya guarde la imagen sin transparencias
    _raw_pixmap = QPixmap::fromImage(img);
}
