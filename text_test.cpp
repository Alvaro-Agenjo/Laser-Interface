#include "text_test.h"
#include "ui_text_test.h"
#include <QMessageBox>
#include <QPushButton>

Text_test::Text_test(QWidget *parent) : QDialog(parent), ui(new Ui::Text_test) {
    ui->setupUi(this);


    connect(ui->btn_test->button(QDialogButtonBox::Apply),
            &QPushButton::clicked,
            this,
            &Text_test::genetareTest);
    connect(ui->btn_test, &QDialogButtonBox::rejected, this, &Text_test::reject);
}

Text_test::~Text_test() {
    delete ui;
}

void Text_test::genetareTest() {
    QString output;

    if (!check(&output)) {
        QMessageBox::critical(this, "Error", output);
        return;
    }

    int filas = (ui->txt_maxS->value() - ui->txt_minS->value()) / ui->txt_stepS->value();
    int columnas = (ui->txt_maxV->value() - ui->txt_minV->value()) / ui->txt_stepV->value();
    float desfaseX = _maxW + _margen;
    float desfaseY = _maxH + _margen;

    float origenX = ui->txt_X->value(), origenY = ui->txt_Y->value();
    // Generar cuadrados
    int counter = 0;

    for (int col = columnas; col >= 0; col--) {
        for (int row = 0; row <= filas; row++) {
            Proceso *p = new Proceso(counter,
                                     Factoria::Plano::TEXTO,
                                     "WOL&7",
                                     col * desfaseX + origenX + 4,
                                     row * desfaseY + origenY + 6);
            p->setPotencia(ui->txt_potencia->value());
            p->setVelocidad(ui->txt_minV->value() + ui->txt_stepV->value() * col);
            p->setPasadas(1);

            p->getOperation()->setModo(0);

            auto *f = dynamic_cast<Forma_texto *>(p->getObjeto());
            f->setStyle(ui->combo_estilo->currentFont().family());
            f->setSize(ui->txt_minS->value() + ui->txt_stepS->value() * row);

            _p.push_back(p);
            counter++;
        }
    }

    //añadir texto
    int p = 40;
    int v = 1000;
    for (int col = 0; col <= columnas; col++) {
        QString text("F");
        text.append(QString().number(ui->txt_minV->value() + ui->txt_stepV->value() * col));

        Proceso *p = new Proceso(counter, Factoria::Plano::TEXTO, text);
        auto *f = dynamic_cast<Forma_texto *>(p->getObjeto());
        f->setStyle("Agency FB");
        f->setSize(3);
        f->setBold(true);
        f->setX(col * desfaseX + origenX + 6);
        f->setY(origenY);
        f->setGraphics(*p->getOperation());
        f->setAngulo(0);

        _p.push_back(p);
        counter++;
    }

    for (int row = 0; row <= filas; row++) {
        QString text(QString().number(ui->txt_minS->value() + row));

        Proceso *p = new Proceso(counter, Factoria::Plano::TEXTO, text);
        auto *f = dynamic_cast<Forma_texto *>(p->getObjeto());
        f->setStyle("Agency FB");
        f->setSize(3);
        f->setBold(true);
        f->setX(origenX);
        f->setY(row * desfaseY + origenY + 7);
        f->setGraphics(*p->getOperation());
        f->setAngulo(-90);

        _p.push_back(p);
        counter++;
    }

    this->accept();
}

bool Text_test::check(QString *response) {
    if (ui->txt_minS->value() >= ui->txt_maxS->value()) {
        response->append("El tamaño de letra mínimo debe ser superior al máximo");
        return false;
    }
    if (ui->txt_minV->value() >= ui->txt_maxV->value()) {
        response->append("La velocidad mínima debe ser inferior a la máxima");
        return false;
    }


    int filas = (ui->txt_maxS->value() - ui->txt_minS->value()) / ui->txt_stepS->value();
    Proceso *p = new Proceso(-1, Factoria::Plano::TEXTO, "WOL&7");
    // Aplicamos estilo y actualizamos para obtener las dimensiones correctas
    auto *f = dynamic_cast<Forma_texto *>(p->getObjeto());
    f->setStyle(ui->combo_estilo->currentFont().family());
    f->setSize(ui->txt_maxS->value());
    p->updateGraphics();

    _maxH = p->getObjeto()->getAlto()*0.2;
    _maxW = p->getObjeto()->getLargo()*0.75;

    if (filas * (_maxH + _margen) > 210- ui->txt_Y->value()) {
        response->append("No se puede superar el tamaño máximo de la placa (220mmx220mm). Alto: ");
        response->append(QString().number(filas * (_maxH + _margen) + ui->txt_Y->value()+ 10));
        return false;
    }
    int columnas = (ui->txt_maxV->value() - ui->txt_minV->value()) / ui->txt_stepV->value();
    if (columnas * (_maxW + _margen) > 210 - ui->txt_X->value()) {
        response->append("No se puede superar el tamaño máximo de la placa (220mmx220mm). Largo: ");
        response->append(QString().number(columnas * (_maxW + _margen) + ui->txt_X->value() + 10));
        return false;
    }
    return true;
}


