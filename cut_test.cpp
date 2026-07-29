#include "cut_test.h"
#include "ui_cut_test.h"

#include <QMessageBox>
#include <QPushButton>

Cut_test::Cut_test(QWidget *parent) : QDialog(parent), ui(new Ui::Cut_test) {
    ui->setupUi(this);

    connect(ui->btn_test->button(QDialogButtonBox::Apply),
            &QPushButton::clicked,
            this,
            &Cut_test::genetareTest);
    connect(ui->btn_test, &QDialogButtonBox::rejected, this, &Cut_test::reject);
}

Cut_test::~Cut_test() {
    delete ui;
}

void Cut_test::genetareTest() {
    QString output;

    if (!check(&output)) {
        QMessageBox::critical(this, "Error", output);
        return;
    }

    int filas = (ui->txt_maxP->value() - ui->txt_minP->value());
    int columnas = (ui->txt_maxV->value() - ui->txt_minV->value()) / ui->txt_stepV->value();
    float desfase = ((ui->txt_lado->value() + _margen) < 9) ? 9 : ui->txt_lado->value() + _margen;

    float origenX = ui->txt_X->value(), origenY = ui->txt_Y->value();
    // Generar cuadrados
    int counter = 0;

    for (int col = columnas; col >= 0; col--) {
        for (int row = 0; row <= filas; row++) {

            Proceso *p = new Proceso(counter,
                                     Factoria::Plano::RECTANGULO,
                                     col * desfase + origenX + 9,
                                     row * desfase + origenY + 10,
                                     ui->txt_lado->value());
            p->setPotencia(ui->txt_potencia->value());
            p->setVelocidad(ui->txt_minV->value() + ui->txt_stepV->value() * col);
            p->setPasadas(ui->txt_minP->value() + row);

            p->getOperation()->setModo(2);
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
        f->setX(col * desfase + origenX + 4);
        f->setY(origenY);
        f->setGraphics(*p->getOperation());
        f->setAngulo(0);

        _p.push_back(p);
        counter++;
    }

    for (int row = 0; row <= filas; row++) {
        QString text(QString().number(ui->txt_minP->value()+row));

        Proceso *p = new Proceso(counter, Factoria::Plano::TEXTO, text);
        auto *f = dynamic_cast<Forma_texto *>(p->getObjeto());
        f->setStyle("Agency FB");
        f->setSize(3);
        f->setBold(true);
        f->setX(origenX);
        f->setY(row * desfase + origenY + 6);
        f->setGraphics(*p->getOperation());
        f->setAngulo(-90);

        _p.push_back(p);
        counter++;
    }

    // Potencia
    QString text("Potencia: " + QString().number(ui->txt_potencia->value()));

    Proceso *pr = new Proceso(counter, Factoria::Plano::TEXTO, text);
    auto *f = dynamic_cast<Forma_texto *>(pr->getObjeto());
    f->setStyle("Agency FB");
    f->setSize(3);
    f->setBold(true);
    f->setX(floor(columnas/2) * desfase + origenX + 4);
    f->setY((filas+1) * desfase + origenY + 6);
    f->setGraphics(*pr->getOperation());
    f->setAngulo(0);

    _p.push_back(pr);
    counter++;
    this->accept();
}

bool Cut_test::check(QString *response) {
    if (ui->txt_minP->value() >= ui->txt_minV->value()) {
        response->append("El número máximo de pasadas debe ser superior al mínimo");
        return false;
    }
    if (ui->txt_minV->value() >= ui->txt_maxV->value()) {
        response->append("La velocidad mínima debe ser inferior a la máxima");
        return false;
    }


    int filas = (ui->txt_maxP->value() - ui->txt_minP->value()+1);
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


