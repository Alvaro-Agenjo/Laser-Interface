#ifndef FORMA_PNG_H
#define FORMA_PNG_H

#include "forma_imagen.h"

class Forma_png : public Forma_imagen {
public:
    Forma_png();
    Forma_png(const QString &filePath);
};

#endif // FORMA_PNG_H
