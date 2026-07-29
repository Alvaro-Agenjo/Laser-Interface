#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "SP_test.h"
#include "cut_test.h"
#include "text_test.h"
#include "perfilesdlg.h"
#include "laserview.h"

#include "CONSTANTS.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QToolButton>


static int counter = 0;
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);


    ui->btn_addRectangle->setIcon(QIcon(":/icons/data/rectangulo.png"));
    ui->btn_addRectangle->setIconSize(QSize(30, 30));
    ui->btn_addElipse->setIcon(QIcon(":/icons/data/elipse.png"));
    ui->btn_addElipse->setIconSize(QSize(30, 30));
    ui->btn_addIMG->setIcon(QIcon(":/icons/data/IMG.png"));
    ui->btn_addIMG->setIconSize(QSize(30, 30));
    // Escena
    scene = new LaserScene(-TableX*3, -TableY-3, TableX*3, TableY*3, this);
    ui->escena->setScene(scene);
    ui->escena->centerOn(TableX/2, TableY/2);

    connect(scene, &LaserScene::itemMoved, this, &MainWindow::MovimientoDetectado);

    // Menu de parametros
    ui->toolBox->setMinimumWidth(320);

    connect(ui->btn_perfil,
            &QPushButton::clicked,
            this,
            &MainWindow::on_actionMostrar_perfiles_triggered);


    // Lista de procesos
    ui->listWidget->setEditTriggers(QAbstractItemView::DoubleClicked
                                    | QAbstractItemView::SelectedClicked);
    ui->listWidget->setIconSize(QSize(25, 25));
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);


    QFont fuente;
    fuente.setPointSize(15);
    ui->listWidget->setFont(fuente);

    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::CargarProceso);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::RenombrarProceso);
    connect(ui->listWidget->model(), &QAbstractItemModel::rowsMoved, this, &MainWindow::AjustarPrioridad);
    connect(ui->listWidget, &QListWidget::customContextMenuRequested, this, &MainWindow::MostrarMenu);


    //Desplegables
    connect(ui->actionAbrir_imagen, &QAction::triggered, this, &MainWindow::on_btn_addIMG_clicked);
    connect(ui->actionBorrar_selecci_n, &QAction::triggered, this, &MainWindow::on_btn_borrar_clicked);

    // Menus desplegables
    //(Objeto)
    QMenu *menu = new QMenu(this);
    // Añades tus acciones existentes al menú desplegable
    menu->addAction(ui->actionAbrir_imagen);
    menu->addAction(ui->actionBorrar_objetos);
    menu->addAction(ui->actionBorrar_selecci_n);
    menu->addAction(ui->actionDuplicar_objeto);

    // Asignas el menú al botón que ya tienes creado
    ui->actionObjetos->setMenu(menu);
    ui->actionObjetos->setProperty("toolButtonPopupMode", QVariant(QToolButton::InstantPopup));


     //(Tests)
    menu = new QMenu(this);

    menu->addAction(ui->action_Speed_vs_Power);
    menu->addAction(ui->action_Pasadas);

    ui->actionTests->setMenu(menu);
    ui->actionTests->setProperty("toolButtonPopupMode", QVariant(QToolButton::InstantPopup));

     //(Materiales)
    menu = new QMenu(this);
    menu->addAction(ui->actionMostrar_perfiles);
    ui->actionMateriales->setMenu(menu);
    ui->actionMateriales->setProperty("toolButtonPopupMode", QVariant(QToolButton::InstantPopup));
}
MainWindow::~MainWindow() {
    for (Proceso *p : _procesos) {
        delete p;
    }
    delete ui;
}


// Input proceso
void MainWindow::on_txt_pasadas_valueChanged(int arg1) {
    _proceso_actual->setPasadas(arg1);
}
void MainWindow::on_txt_power_valueChanged(int arg1) {
    _proceso_actual->setPotencia(arg1);
}
void MainWindow::on_txt_vel_valueChanged(int arg1) {
    _proceso_actual->setVelocidad(arg1);
}


// Input forma
void MainWindow::on_txt_posX_valueChanged(double arg1) {
    if (_proceso_actual->getObjeto()->getLock()) {
        ui->txt_posX->blockSignals(true);
        ui->txt_posX->setValue(_proceso_actual->getObjeto()->getX());
        ui->txt_posX->blockSignals(false);

        return;
    }
    float limit = 220 - ui->txt_sizeX->value();
    if (arg1 > limit) _proceso_actual->getObjeto()->setX(limit);
    else
        _proceso_actual->getObjeto()->setX(arg1);
}
void MainWindow::on_txt_posY_valueChanged(double arg1) {
    if (_proceso_actual->getObjeto()->getLock()) {
        ui->txt_posY->blockSignals(true);
        ui->txt_posY->setValue(_proceso_actual->getObjeto()->getY());
        ui->txt_posY->blockSignals(false);
        return;
    }
    float limit = 220 - ui->txt_sizeY->value();
    if (arg1 > limit) _proceso_actual->getObjeto()->setY(limit);
    else
        _proceso_actual->getObjeto()->setY(arg1);
}
void MainWindow::on_txt_sizeX_valueChanged(double arg1) {
    if (_proceso_actual->getObjeto()->getLock()) {
        ui->txt_sizeX->blockSignals(true);
        ui->txt_sizeX->setValue(_proceso_actual->getObjeto()->getLargo());
        ui->txt_sizeX->blockSignals(false);
        return;
    }

    if (ui->checkBox->isChecked()) {
        float _lastSize = _proceso_actual->getObjeto()->getLargo();
        float scale = arg1 / _lastSize;
        _proceso_actual->getObjeto()->setSize(scale);

        ui->txt_sizeY->blockSignals(true);
        ui->txt_sizeY->setValue(ui->txt_sizeY->value() * scale);
        ui->txt_sizeY->blockSignals(false);
    } else {
        _proceso_actual->getObjeto()->setLargo(arg1);
        ui->txt_sizeY->setValue(_proceso_actual->getObjeto()->getAlto());
    }
}
void MainWindow::on_txt_sizeY_valueChanged(double arg1) {
    if (_proceso_actual->getObjeto()->getLock()) {
        ui->txt_sizeY->blockSignals(true);
        ui->txt_sizeY->setValue(_proceso_actual->getObjeto()->getAlto());
        ui->txt_sizeY->blockSignals(false);
        return;
    }
    if (ui->checkBox->isChecked()) {
        float _lastSize = _proceso_actual->getObjeto()->getAlto();
        float scale = arg1 / _lastSize;
        _proceso_actual->getObjeto()->setSize(scale);

        ui->txt_sizeX->blockSignals(true);
        ui->txt_sizeX->setValue(ui->txt_sizeX->value() * scale);
        ui->txt_sizeX->blockSignals(false);
    } else {
        _proceso_actual->getObjeto()->setAlto(arg1);
    }
}

void MainWindow::on_txt_rot_valueChanged(double arg1) {
    Forma *forma = _proceso_actual->getObjeto();
    auto f = dynamic_cast<Forma_texto *>(forma);

    if (f) {
        f->setAngulo(arg1);
        _proceso_actual->updateGraphics(); // Esto redibuja la escena
    }
}


// Input operacion
void MainWindow::on_sel_modo_currentIndexChanged(int index) {
    ui->operacion->setCurrentIndex(index);
    _proceso_actual->getOperation()->setModo(index);
    _proceso_actual->updateGraphics();
}
void MainWindow::on_chck_original_stateChanged(int arg1) {
    _proceso_actual->getOperation()->setShow_original(arg1);
    _proceso_actual->updateGraphics();
}
void MainWindow::on_chck_invertir_stateChanged(int arg1) {
    _proceso_actual->getOperation()->setInvert(arg1);
    _proceso_actual->updateGraphics();
}
void MainWindow::on_sel_simetria_currentIndexChanged(int index) {
    _proceso_actual->getOperation()->setFlip(index);
    _proceso_actual->updateOrientation();
}

void MainWindow::on_sld_bn_umbral_valueChanged(int value) {
    ui->num_bn_umbral->setValue(value);
    _proceso_actual->getOperation()->setUmbral(value);
    _proceso_actual->updateGraphics();
}
void MainWindow::on_txt_bn_densidad_valueChanged(int arg1) {
    _proceso_actual->getOperation()->setDensidad(arg1);
}

void MainWindow::on_sld_gray_contraste_valueChanged(int value) {
    ui->num_gray_contraste->setValue(value);
    _proceso_actual->getOperation()->setContraste(value);
    _proceso_actual->updateGraphics();
}
void MainWindow::on_sld_gray_brillo_valueChanged(int value) {
    ui->num_gray_brillo->setValue(value);
    _proceso_actual->getOperation()->setBrillo(value);
    _proceso_actual->updateGraphics();
}
void MainWindow::on_sld_gray_lvl_valueChanged(int value) {
    ui->num_gray_lvl->setValue(value);
    _proceso_actual->getOperation()->setLvlgris(value);
    _proceso_actual->updateGraphics();
}
void MainWindow::on_txt_gray_densidad_valueChanged(int arg1) {
    _proceso_actual->getOperation()->setDensidad(arg1);
}

void MainWindow::on_sld_vec_umbral_valueChanged(int value) {
    ui->num_vec_umbral->setValue(value);
    _proceso_actual->getOperation()->setUmbral(value);
    _proceso_actual->updateGraphics();
}


//Input texto
void MainWindow::on_check_bold_stateChanged(int arg1) {
    Forma *forma = _proceso_actual->getObjeto();
    auto f = dynamic_cast<Forma_texto *>(forma);
    f->setBold(arg1);
    _proceso_actual->updateGraphics();
}
void MainWindow::on_check_italic_stateChanged(int arg1) {
    Forma *forma = _proceso_actual->getObjeto();
    auto f = dynamic_cast<Forma_texto *>(forma);
    f->setItalic(arg1);
    _proceso_actual->updateGraphics();
}
void MainWindow::on_check_underline_stateChanged(int arg1) {
    Forma *forma = _proceso_actual->getObjeto();
    auto f = dynamic_cast<Forma_texto *>(forma);
    f->setUnderline(arg1);
    _proceso_actual->updateGraphics();
}
void MainWindow::on_txt_tamao_valueChanged(int arg1) {
    Forma *forma = _proceso_actual->getObjeto();
    auto f = dynamic_cast<Forma_texto *>(forma);
    f->setSize(arg1);
    _proceso_actual->updateGraphics();

    ui->txt_sizeX->setValue(f->getLargo());
    ui->txt_sizeY->setValue(f->getAlto());
}
void MainWindow::on_txt_texto_editingFinished() {
    Forma *forma = _proceso_actual->getObjeto();
    auto f = dynamic_cast<Forma_texto *>(forma);
    f->setTexto(ui->txt_texto->text());
    _proceso_actual->updateGraphics();
}
void MainWindow::on_txt_fuente_currentFontChanged(const QFont &f) {
    Forma *forma = _proceso_actual->getObjeto();
    auto tex = dynamic_cast<Forma_texto *>(forma);
    // tex->setStyle(f.styleName());
    tex->setStyle(f.family());
    _proceso_actual->updateGraphics();
}


// Lista de procesos
void MainWindow::CargarProceso(QListWidgetItem *item) {
    int row = ui->listWidget->row(item);
    if (row >= 0 && row < _procesos.size()) {
        _proceso_actual = _procesos[row];
        loadProcessData(_procesos[row]);
    }
}
void MainWindow::RenombrarProceso(QListWidgetItem *item) {
    int row = ui->listWidget->row(item);
    if (row >= 0 && row < _procesos.size()) _procesos[row]->setProcessName(item->text());
}
void MainWindow::AjustarPrioridad() {
    std::vector<Proceso *> nuevaLista;
    for (int n = 0; n < ui->listWidget->count(); n++) {
        QListWidgetItem *item = ui->listWidget->item(n);
        for (int i = 0; i < _procesos.size(); i++) {
            if (item->text() != _procesos[i]->getProcessName()) continue;
            nuevaLista.push_back(_procesos[i]);
            // _procesos[i]->getRender()->setZValue(_procesos.size() - i);
            break;
        }
    }

    _procesos.clear();
    _procesos.assign(nuevaLista.begin(), nuevaLista.end());
}
void MainWindow::MostrarMenu(const QPoint &pos) {
    QListWidgetItem *item = ui->listWidget->currentItem();
    if (!item) return; // Si no hay ítem, no hacemos nada

    QMenu menu;
    QAction *alFrente = menu.addAction("Enviar al frente");
    QAction *alFondo = menu.addAction("Enviar al fondo");

    QAction *seleccionada = menu.exec(ui->listWidget->mapToGlobal(pos));

    if (seleccionada == alFrente) {
        // int fila = ui->listWidget->row(item);
        for (int n = _procesos.size()-1; n >= 0; n--) {
            if (_procesos[n]->getProcessName() != item->text()) {
                auto z = _procesos[n]->getRender()->zValue();
                _procesos[n]->getRender()->setZValue(z - 1);
                continue;
            }
            _procesos[n]->getRender()->setZValue(_procesos.size());
            break;
        }
    } else if (seleccionada == alFondo) {
        for (int n = 0; n < _procesos.size(); n++) {
            if (_procesos[n]->getProcessName() != item->text()) {
                auto z = _procesos[n]->getRender()->zValue();
                _procesos[n]->getRender()->setZValue(z + 1);
                continue;
            }
            _procesos[n]->getRender()->setZValue(1);
            break;
        }
    }
}

// Botones
void MainWindow::on_btn_fijar_toggled(bool checked) {
    setLock(checked);
}
void MainWindow::on_btn_borrar_clicked() {
    clearProcess();
}
void MainWindow::on_btn_gcode_clicked() {
    // 1. Abrir el diálogo para que el usuario elija la ruta y nombre del archivo
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Guardar archivo GCODE"), // Título de la ventana
        "", // Directorio por defecto (vacío = actual)
        tr("GCode Files (*.gcode);;All Files (*)")); // Filtros de extensión

    // 2. Verificar si el usuario canceló la operación (fileName estará vacío)
    if (fileName.isEmpty()) return;

    // 3. Obtener el contenido del GCODE (suponiendo que ya lo generaste)
    QString contenidoGcode = *_gcode.GenerateGcode(_procesos);

    // 4. Escribir el archivo
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << contenidoGcode;
        file.close();

        // Feedback opcional al usuario
        QMessageBox::information(this, "Éxito", "Archivo GCODE guardado correctamente.");
    } else {
        QMessageBox::critical(this, "Error", "No se pudo guardar el archivo en la ubicación seleccionada.");
    }
}


// Grafica
void MainWindow::MovimientoDetectado(QGraphicsItem *item, QPointF pos) {
    for (Proceso *p : _procesos) {
        if (p->getRender() == item) {
            // _proceso_actual = p;
            QListWidgetItem *elem = ui->listWidget
                                        ->findItems(p->getProcessName(), Qt::MatchFlag::MatchExactly)
                                        .first();
            ui->listWidget->itemClicked(elem);
            ui->listWidget->setCurrentItem(elem);
            // Actualizas tus SpinBoxes
            ui->txt_posX->setValue(pos.x());
            (p->EsTexto() || dynamic_cast<QGraphicsPixmapItem *>(item))
                ? ui->txt_posY->setValue(pos.y() - p->getObjeto()->getAlto())
                : ui->txt_posY->setValue(pos.y());
            break;
        }
    }
}


// Adicion de objetos

void MainWindow::on_btn_addIMG_clicked() {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Abrir archivo"), // Título de la ventana
        "", // Directorio por defecto (vacío = actual)
        tr("Archivos de imagen (*.png *.jpg *.jpeg);;All Files (*)")); // Filtros de extensión

    // QString fileName = "C:\\Users\\Alvaro\\Desktop\\test.png";
    // 2. Verificar si el usuario canceló la operación (fileName estará vacío)
    if (fileName.isEmpty()) return;

    if(fileName.contains(".png"))
        loadProcess(new Proceso(counter, Factoria::Plano::PNG, fileName));
    else
        loadProcess(new Proceso(counter, Factoria::Plano::IMG, fileName));


    if (_proceso_actual->getObjeto()->getX() == -1){
        QMessageBox::information(this, "ERROR", "No se puede abrir el archivo seleccionado.");
        clearProcess();
    }

}
void MainWindow::on_btn_addRectangle_clicked() {
    loadProcess(new Proceso(counter, Factoria::Plano::RECTANGULO, 10, 10));
}
void MainWindow::on_btn_addElipse_clicked() {
    loadProcess(new Proceso(counter, Factoria::Plano::ELIPSE, 10, 10));
}

void MainWindow::on_btn_addText_clicked() {
    loadProcess(new Proceso(counter, Factoria::Plano::TEXTO, "Text"));
}

//Tests
void MainWindow::on_action_Speed_vs_Power_triggered() {
    Tests testDialog(this);

    // Lo mostramos de forma modal (bloquea la ventana principal hasta cerrar)
    if (testDialog.exec() == QDialog::Accepted) {
        const std::vector<Proceso *> procesos
            = testDialog.getProcesosGenerados(); // Debes tener este método en Tests

        for (const auto &p : procesos) {
            loadProcess(p);
        }
    }
}
void MainWindow::on_action_Pasadas_triggered() {
    Cut_test testDialog(this);

    // Lo mostramos de forma modal (bloquea la ventana principal hasta cerrar)
    if (testDialog.exec() == QDialog::Accepted) {
        const std::vector<Proceso *> procesos
            = testDialog.getProcesosGenerados(); // Debes tener este método en Tests

        for (const auto &p : procesos) {
            loadProcess(p);
        }
    }
}
void MainWindow::on_actionAcuracy_Text_triggered() {
    Text_test testDialog(this);

    // Lo mostramos de forma modal (bloquea la ventana principal hasta cerrar)
    if (testDialog.exec() == QDialog::Accepted) {
        const std::vector<Proceso *> procesos
            = testDialog.getProcesosGenerados(); // Debes tener este método en Tests

        for (const auto &p : procesos) {
            loadProcess(p);
        }
    }
}


//Desplegable
void MainWindow::on_actionBorrar_objetos_triggered() {
    for (const auto &p : _procesos) {
        delete p;
    }
    _procesos.clear();
    _proceso_actual = nullptr;
    scene->clear();
    ui->listWidget->clear();
}
void MainWindow::on_actionDuplicar_objeto_triggered() {
    Proceso *p = new Proceso(counter, _proceso_actual->EsTexto());
    p->setObjeto(_proceso_actual->getObjeto()->clone());
    p->setOperacion(*_proceso_actual->getOperation());

    p->setPotencia(_proceso_actual->getPotencia());
    p->setPasadas(_proceso_actual->getPasadas());
    p->setVelocidad(_proceso_actual->getVelocidad());

    loadProcess(p);
}

//Privadas
void MainWindow::loadProcess(Proceso *proceso) {
    counter++;
    _proceso_actual = proceso;
    _procesos.push_back(proceso);

    // Add render
    scene->addItem(proceso->getRender());

    // Add to list
    QListWidgetItem *item = new QListWidgetItem(proceso->getProcessName());
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    ui->listWidget->addItem(item);

    loadProcessData(proceso);
}
void MainWindow::loadProcessData(Proceso *proceso) {
    // Get Process properties
    ui->txt_pasadas->setValue(proceso->getPasadas());
    ui->txt_power->setValue(proceso->getPotencia());
    ui->txt_vel->setValue(proceso->getVelocidad());

    // Get Form properties
    bool texto = proceso->EsTexto();
    auto forma = proceso->getObjeto();
    ui->txt_posX->setValue(forma->getX());
    ui->txt_posY->setValue(forma->getY());
    ui->txt_sizeX->setValue(forma->getLargo());
    ui->txt_sizeY->setValue(forma->getAlto());

    bool lock = forma->getLock();
    ui->btn_fijar->setChecked(lock);
    setLock(lock);

    loadProcessOperation(proceso, texto);

}
void MainWindow::loadProcessOperation(Proceso *proceso, bool texto) {
    // Get operation
    auto operacion = proceso->getOperation();

    if (!texto) {
        ui->Texto->setEnabled(false);
        enableOperacion(true);
        // Atributos comunes
        switch (operacion->flip()) {
        case flip::NO_FLIP: {
            ui->sel_simetria->setCurrentIndex(0);
            break;
        }
        case flip::VERTICAL: {
            ui->sel_simetria->setCurrentIndex(1);
            break;
        }
        case flip::HORIZONTAL: {
            ui->sel_simetria->setCurrentIndex(2);
            break;
        }
        case flip::BOTH: {
            ui->sel_simetria->setCurrentIndex(3);
            break;
        }
        }

        ui->chck_original->setCheckState(
            (operacion->show_original() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked));
        ui->chck_invertir->setCheckState(
            (operacion->invert() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked));
    } else {
        ui->Texto->setEnabled(true);
        enableOperacion(false);
        Forma_texto *forma = dynamic_cast<Forma_texto *>(proceso->getObjeto());

        ui->txt_rot->setValue(forma->angulo());
        ui->txt_texto->setText(forma->texto());
        ui->txt_tamao->setValue(forma->texSize());

        ui->check_bold->setChecked(forma->bold());
        ui->check_italic->setChecked(forma->italic());
        ui->check_underline->setChecked(forma->underline());

    }

    switch (operacion->modo()) {
    case modo::BLANCO_NEGRO: {
        ui->sel_modo->setCurrentIndex(0);
        ui->sld_bn_umbral->setValue(operacion->umbral());
        ui->txt_bn_densidad->setValue(operacion->densidad());
        break;
    }
    case modo::GRIS: {
        ui->sel_modo->setCurrentIndex(1);
        ui->sld_gray_brillo->setValue(operacion->brillo());
        ui->sld_gray_contraste->setValue(operacion->contraste());
        ui->sld_gray_lvl->setValue(operacion->lvlgris());
        ui->txt_gray_densidad->setValue(operacion->densidad());
        break;
    }
    case modo::VECTOR: {
        ui->sel_modo->setCurrentIndex(2);
        ui->sld_vec_umbral->setValue(operacion->umbral());
        break;
    }
    }

    proceso->updateGraphics();
    proceso->updateOrientation();
}

void MainWindow::setLock(bool close) {
    _proceso_actual->getObjeto()->setLock(close);

    QListWidgetItem *item
        = ui->listWidget->findItems(_proceso_actual->getProcessName(), Qt::MatchExactly).first();

    close ? item->setIcon(QIcon(":/icons/data/close_lock")) : item->setIcon(QIcon(":/icons/data/open_lock"));
}
void MainWindow::enableOperacion(bool setTo) {
    ui->chck_invertir->setEnabled(setTo);
    ui->chck_original->setEnabled(setTo);
    ui->sel_simetria->setEnabled(setTo);
    ui->operacion->setEnabled(setTo);

    ui->checkBox->setChecked(setTo);
    ui->checkBox->setEnabled(setTo);

    ui->txt_sizeY->blockSignals(!setTo);
    ui->txt_sizeY->setEnabled(setTo);

    ui->txt_sizeX->blockSignals(!setTo);
    ui->txt_sizeX->setEnabled(setTo);
}

void MainWindow::aplicarPerfil(const PerfilMaterial &m) {
    _proceso_actual->setPasadas(m.pasadas);
    _proceso_actual->setPotencia(m.potencia);
    _proceso_actual->setVelocidad(m.velocidad);

    loadProcessData(_proceso_actual);
}

void MainWindow::clearProcess() {
    for (int i = 0; i < _procesos.size(); i++) {
        if (_procesos[i] == _proceso_actual) {
            //eliminar reder de la escena
            scene->removeItem(_proceso_actual->getRender());

            //eliminar proceso de la lista
            auto *item = ui->listWidget->takeItem(i);
            ui->listWidget->removeItemWidget(item);

            // eliminar del historial
            _procesos.erase(_procesos.begin() + i);

            // Cargar siguiente proceso
            if (_procesos.size() > 0) {
                _proceso_actual = _procesos.front();
                loadProcessData(_proceso_actual);
            }

            break;
        }
    }
}
void MainWindow::on_actionMostrar_perfiles_triggered() {
    PerfilesDlg perfil(this);

    // Lo mostramos de forma modal (bloquea la ventana principal hasta cerrar)
    if (perfil.exec() == QDialog::Accepted) {
        aplicarPerfil(perfil.perfil());
    }
}
