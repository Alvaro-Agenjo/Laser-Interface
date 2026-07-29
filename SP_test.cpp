#include "SP_test.h"
#include "ui_SP_test.h"
#include <QMessageBox>
#include <QPushButton>
#include "factoria.h"


Tests::Tests(QWidget *parent) : QDialog(parent),  ui(new Ui::Tests) {
    ui->setupUi(this);

    connect(ui->btn_test->button(QDialogButtonBox::Apply), &QPushButton::clicked , this, &Tests::genetareTest);
    connect(ui->btn_test, &QDialogButtonBox::rejected, this, &Tests::reject);
}

Tests::~Tests() {
    delete ui;
}

void Tests::genetareTest() {
    QString output;

    if (!check(&output)) {
        QMessageBox::critical(this, "Error", output);
        return;
    }

    int columnas = (ui->txt_maxP->value() - ui->txt_minP->value()) / ui->txt_stepP->value();
    int filas = (ui->txt_maxV->value() - ui->txt_minV->value()) / ui->txt_stepV->value();
    float desfase = ((ui->txt_lado->value() + _margen) < 9)? 9 : ui->txt_lado->value() + _margen;
    float origenX = ui->txt_X->value(), origenY = ui->txt_Y->value();
    // Generar cuadrados
    int counter = 0;

    for (int row = filas; row >= 0; row--) {
        for (int col = 0; col <= columnas; col++) {
            Proceso *p = new Proceso(counter,
                                     Factoria::Plano::RECTANGULO,
                                     col * desfase + 11 + origenX,
                                     row * desfase + 9.5 + origenY,
                                     ui->txt_lado->value());
            p->setPotencia(ui->txt_minP->value() + ui->txt_stepP->value() * col);
            p->setVelocidad(ui->txt_minV->value() + ui->txt_stepV->value() * row);

            p->getOperation()->setDensidad(ui->txt_densidad->value());
            _p.push_back(p);
            counter++;
        }
    }

    //añadir texto
    int p = 40;
    int v = 1000;
    for (int col = 0; col <= columnas; col++) {
        QString text("S");
        text.append(QString().number(ui->txt_minP->value() + ui->txt_stepP->value() * col));

        Proceso *p = new Proceso(counter, Factoria::Plano::TEXTO, text);
        auto *f = dynamic_cast<Forma_texto *>(p->getObjeto());
        f->setStyle("Agency FB");
        f->setSize(3);
        f->setBold(true);
        f->setX(col * desfase + 7 + origenX);
        f->setY(origenY);
        f->setGraphics(*p->getOperation());
        f->setAngulo(0);

        _p.push_back(p);
        counter++;
    }

    for (int row = 0; row <= filas; row++) {
        QString text("F");
        text.append(QString().number(ui->txt_minV->value() + ui->txt_stepV->value() * row));

        Proceso *p = new Proceso(counter, Factoria::Plano::TEXTO, text);
        auto *f = dynamic_cast<Forma_texto *>(p->getObjeto());
        f->setStyle("Agency FB");
        f->setSize(3);
        f->setBold(true);
        f->setX(origenX);
        f->setY(row * desfase + 5 + origenY);
        f->setGraphics(*p->getOperation());
        f->setAngulo(-90);

        _p.push_back(p);
        counter++;
    }

    this->accept();
}

bool Tests::check(QString *response) {
    if (ui->txt_minP->value() >= ui->txt_minV->value()) {
        response->append("El número máximo de pasadas debe ser superior al mínimo");
        return false;
    }
    if (ui->txt_minV->value() >= ui->txt_maxV->value()) {
        response->append("La velocidad mínima debe ser inferior a la máxima");
        return false;
    }


    int filas = (ui->txt_maxP->value() - ui->txt_minP->value()) / ui->txt_stepP->value();
    if (filas * (ui->txt_lado->value() + _margen) > 210- ui->txt_Y->value()) {
        response->append("No se puede superar el tamaño máximo de la placa (220mmx220mm). Alto: ");
        response->append(QString().number(filas * (ui->txt_lado->value() + _margen) + ui->txt_Y->value()+ 10));
        return false;
    }
    int columnas = (ui->txt_maxV->value() - ui->txt_minV->value()) / ui->txt_stepV->value();
    if (columnas * (ui->txt_lado->value() + _margen) > 210 - ui->txt_X->value()) {
        response->append("No se puede superar el tamaño máximo de la placa (220mmx220mm). Largo: ");
        response->append(QString().number(columnas * (ui->txt_lado->value() + _margen) + ui->txt_X->value() + 10));
        return false;
    }
    return true;
}


