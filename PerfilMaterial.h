#ifndef PERFILMATERIAL_H
#define PERFILMATERIAL_H

#include <QString>
#include <QJsonObject>

enum class Material {Tilo, Abedul, Nogal, Bambu, Sapeli, Corcho, DES};

static QString Mat2Str(Material m) {
    switch (m) {
    case Material::Tilo:
        return "Tilo";
    case Material::Abedul:
        return "Abedul";
    case Material::Nogal:
        return "Nogal";
    case Material::Bambu:
        return "Bambu";
    case Material::Sapeli:
        return "Sapeli";
    case Material::Corcho:
        return "Corcho";
    default:
        return "Desconocido";
    }
}
static Material Str2Mat(const QJsonValue &m) {
    if (m == "Tilo") return Material::Tilo;
    if (m == "Abedul") return Material::Abedul;
    if (m == "Nogal") return Material::Nogal;
    if (m == "Bambu") return Material::Bambu;
    if (m == "Sapeli") return Material::Sapeli;
    if (m == "Corcho") return Material::Corcho;
    return Material::DES;
}
static Material Str2Mat(const QString &m) {
    if (m == "Tilo") return Material::Tilo;
    if (m == "Abedul") return Material::Abedul;
    if (m == "Nogal") return Material::Nogal;
    if (m == "Bambu") return Material::Bambu;
    if (m == "Sapeli") return Material::Sapeli;
    if (m == "Corcho") return Material::Corcho;
    return Material::DES;
}



struct PerfilMaterial {
    QString alias;
    bool grabado;
    enum Material material;
    double profundidad;
    int pasadas;
    int potencia;
    int velocidad;
    static const int num_mat = 6;
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["alias"] = alias;
        obj["operacion"] = grabado ? "Grabado" : "Corte";
        obj["material"] = Mat2Str(material);
        obj["profundidad"] = profundidad;
        obj["pasadas"] = pasadas;
        obj["potencia"] = potencia;
        obj["velocidad"] = velocidad;
        return obj;
    }

    static PerfilMaterial formJson(const QJsonObject &obj) {
        return {obj["alias"].toString(),
                obj["operacion"].toString() == "Grabado",
                Str2Mat(obj["material"]),
                obj["profundidad"].toDouble(),
                obj["pasadas"].toInt(),
                obj["potencia"].toInt(),
                obj["velocidad"].toInt()};
    }
};



#endif // PERFILMATERIAL_H
