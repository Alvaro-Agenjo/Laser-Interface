#ifndef OPERACION_H
#define OPERACION_H

enum class modo { BLANCO_NEGRO, GRIS, VECTOR };
enum class flip { NO_FLIP, VERTICAL, HORIZONTAL, BOTH };

class Operacion {
public:
    Operacion();

    // General
    inline enum modo modo() const { return _modo; }
    inline bool show_original() const { return _show_original; }
    inline bool invert() const { return _invert; }
    inline enum flip flip() const { return _flip; }

    void setModo(int modo);
    inline void setShow_original(bool show_original) { _show_original = show_original; }
    inline void setInvert(bool invert) { _invert = invert; }
    void setFlip(int flip);


    // Blanco y negro & Vector
    inline int umbral() const { return _modo == modo::GRIS ? -1 : _umbral; }
    inline int densidad() const { return _modo == modo::VECTOR ? -1 : _densidad; }

    void setUmbral(int umbral);
    void setDensidad(int densidad);

    // Gris
    inline int brillo() const { return _modo == modo::GRIS ? _brillo : -1; }
    inline int contraste() const { return _modo == modo::GRIS ? _contraste : -1; }
    inline int lvlgris() const { return _modo == modo::GRIS ? _lvlgris : -1; }

    void setBrillo(int brillo);
    void setContraste(int contraste);
    void setLvlgris(int lvlgris);


private:
    enum modo _modo = modo::BLANCO_NEGRO;
    bool _show_original = false;
    bool _invert = false;

    enum flip _flip = flip::NO_FLIP;
    int _umbral = 127;

    int _brillo = 50;
    int _contraste = 50;
    int _lvlgris = 255;

    int _densidad = 9;
};

#endif // OPERACION_H
