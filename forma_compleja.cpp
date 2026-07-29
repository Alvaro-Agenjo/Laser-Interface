#include "forma_compleja.h"
#include "operacion.h"
#include "opencv2/opencv.hpp"


Camino Forma_compleja::raster(const QImage *img, int densidad) const {
    Camino camino;
    int offset = img->width()-1;
    int counter = 0;

    for (int y = 0; y < img->height(); y++) {
        const uchar *line = img->scanLine(y);
        for (int x = 0; x < img->width(); x++) {
            if (y % 2) {
                if (x != offset && x != 0 && line[offset - x] == line[offset - x - 1])
                    continue; //solo añadi si es distinto o el ultimo
                if (x == 0){
                    camino.append({{(float) (offset - x) / densidad + _x, (float) y / densidad + _y}, 0});
                    counter++;
                } else if (x == offset) {
                    camino.append(
                        {{(float) (offset - x) / densidad + _x, (float) y / densidad + _y}, 255 - line[offset-x]});
                    counter++;
                    continue;
                }


                // camino.append({{(float) (offset - x) / densidad + _x, (float) y / densidad + _y},
                               // 255 - line[offset - x]});
                camino.append({{(float) (offset - x - 0.5) / densidad + _x, (float) y / densidad + _y},
                               255 - line[offset - x]});
                counter++;
            } else { //izda a dcha
                if (x != offset && x != 0
                    && line[x] == line[x + 1]) //si no es borde ni hay cambio paso
                    continue;
                if (x == 0) {
                    camino.append({{(float) x / densidad + _x, (float) y / densidad + _y}, 0});
                    counter++;
                } else if (x == offset) {
                    camino.append(
                        {{(float) x / densidad + _x, (float) y / densidad + _y}, 255 - line[x]});
                    counter++;
                    continue;
                }
                // camino.append({{(float) x / densidad + _x, (float) y / densidad + _y}, 255 - line[x]});
                camino.append({{(float) (x+0.5) / densidad + _x, (float) y / densidad + _y}, 255 - line[x]});
                counter++;
            }
        }

        if (counter == 3) {
            for (int n = 0; n < counter; n++)
                camino.pop_back();
        }

        counter = 0;
    }
    return camino;
};
Camino Forma_compleja::vectorCV(const QImage *img, int densidad) const {
    Camino camino;
    cv::Mat mat(img->height(), img->width(), CV_8UC1, (void *) img->constBits(), img->bytesPerLine());

    cv::GaussianBlur(mat, mat, cv::Size(3, 3), 0);
    std::vector<std::vector<cv::Point>> contours;
    // cv::findContours(mat, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    cv::findContours(mat, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    // 3. (Opcional) Simplificación: Reduce puntos innecesarios para un G-code más limpio
    std::vector<std::vector<cv::Point>> simplifiedContours;
    for (const auto &c : contours) {
        std::vector<cv::Point> approx;
        cv::approxPolyDP(c, approx, 1.5, true); // El '1.5' es la precisión
        simplifiedContours.push_back(approx);
    }

    for (const auto &contour : contours) {
        // Al inicio de cada contorno, subimos la herramienta (Z=0)
        camino.append(
            {{(float) contour[0].x / densidad + _x, (float) contour[0].y / densidad + _y}, 0});

        for (const auto &p : contour) {
            // Añadimos el punto de corte (Z=255)
            camino.append({{(float) p.x / densidad + _x, (float) p.y / densidad + _y}, 255});
        }

        camino.append(
            {{(float) contour[0].x / densidad + _x, (float) contour[0].y / densidad + _y}, 255});
    }
    return camino;
}

QImage Forma_compleja::renderCV(QImage *img) {
    cv::Mat mat(img->height(), img->width(), CV_8UC1, (void *) img->constBits(), img->bytesPerLine());

    std::vector<std::vector<cv::Point>> contours;
    // cv::findContours(mat, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    cv::findContours(mat, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    // 3. (Opcional) Simplificación: Reduce puntos innecesarios para un G-code más limpio
    // float min_area = 1000;
    std::vector<std::vector<cv::Point>> simplifiedContours;
    for (const auto &c : contours) {
        std::vector<cv::Point> approx;
        cv::approxPolyDP(c, approx, 1.5, true); // El '1.5' es la precisión

        // if (cv::contourArea(c) > min_area)
            simplifiedContours.push_back(approx);
    }

    cv::Mat lienzo = cv::Mat::zeros(img->height(), img->width(), CV_8UC4);
    lienzo = cv::Scalar(255, 255, 255, 10);
    cv::drawContours(lienzo, contours, -1, cv::Scalar(0, 0, 0, 255), 2);
    cv::Mat rgba;
    cv::cvtColor(lienzo, rgba, cv::COLOR_BGRA2RGBA);

    QImage imagen((const uchar*)rgba.data, rgba.cols, rgba.rows, rgba.step, QImage::Format_RGBA8888);
    return imagen.copy();

}

Camino Forma_compleja::vector(const QImage *img, int densidad) const{
    Camino camino;
    static int offset = img->width();
    std::vector<bool> visited(img->width() * img->height(), false);
    const int dx[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    const int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};

    for (int y = 0; y < img->height(); y++) {
        const auto *line = img->scanLine(y);
        for (int x = 0; x < img->width(); x++) {
            if (line[x] != 0 || visited[x + y * offset]) continue; //es blanco -> paso
            //is negro -> oh shit, here we go again
            bool search = true;
            camino.append({{(float) x / densidad + _x, (float) y / densidad + _y}, 0}); //añado G0 inicial
            int currX = x, currY = y;
            int initX = x, initY = y;
            int currDir = 0;
            while (search) {
                search = false;
                for (int i = 0; i < 8; i++) {
                    int n = (i + currDir) % 8;
                    int nextX = currX + dx[n];
                    int nextY = currY + dy[n];
                    const auto *line2 = img->scanLine(nextY);
                    if (nextX < 0 || nextX > offset) continue;
                    if (nextY < 0 || nextY > img->height()) continue;
                    if (visited[nextX + nextY * offset]) continue;
                    if (line2[nextX] != 0) continue;

                    camino.append({{(float) nextX / densidad + _x, (float) nextY / densidad + _y}, 255});
                    search = true;
                    currX = nextX;
                    currY = nextY;
                    currDir = n;
                    visited[currX + currY * offset]
                        = true; //punto visitado comienza busqueda de vecinos

                    //Si se cierra el bucle salta
                    if (currX == initX && currY == initY) search = false;
                    break;
                }
            }
        }
    }
    return camino;
}
QImage Forma_compleja::aplicarSobel(const QImage *img) const {
    float min = 255, max = -1;
    int width = img->width();
    int height = img->height();
    int sobelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int sobelY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    QImage dst(width, height, QImage::Format_ARGB32);
    dst.fill(qRgba(255, 255, 255, 10));

    for (int y = 1; y < height - 1; ++y) {
        const uchar *prev = img->scanLine(y - 1);
        const uchar *curr = img->scanLine(y);
        const uchar *next = img->scanLine(y + 1);
        QRgb *out = (QRgb *) dst.scanLine(y);

        for (int x = 1; x < width - 1; ++x) {
            int gX = 0;
            int gY = 0;

            for (int i = -1; i <= 1; ++i) {
                const uchar *line = (i == -1) ? prev : (i == 0) ? curr : next;
                for (int j = -1; j <= 1; ++j) {
                    int pixelVal = line[x + j];
                    gX += pixelVal * sobelX[i + 1][j + 1] / 8.0;
                    gY += pixelVal * sobelY[i + 1][j + 1] / 8.0;
                }
            }
            int magnitude = std::abs(gX) + std::abs(gY);

            if (magnitude < min) min = magnitude;
            if (magnitude > max) max = magnitude;

            if (magnitude > 50)
                out[x] = qRgba(0, 0, 0, 255);
        }
    }


    return dst;
}