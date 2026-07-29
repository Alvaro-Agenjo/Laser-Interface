#include "perfilesdlg.h"
#include "ui_perfilesdlg.h"


#include <QSettings>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QMenu>
#include <QMessageBox>

PerfilesDlg::PerfilesDlg(QWidget *parent) : QDialog(parent), ui(new Ui::PerfilesDlg) {
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setHorizontalHeaderLabels(
        {"Alias", "Operacion", "Tipo", "Profundidad", "Pasadas", "Potencia", "Velocidad"});
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int n = 0; n < static_cast<int>(Material::DES); n++) {
        ui->combo_mat->addItem(Mat2Str(Material(n)));
        ui->txt_material->addItem(Mat2Str(Material(n)));
    }
    ui->combo_prof->addItem("2");
    ui->combo_prof->addItem("3");


    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidget,
            &QTableWidget::customContextMenuRequested,
            this,
            &PerfilesDlg::showMenu);

    loadProfiles();
    // _perfiles.push_back({"MAT1", Material::Corcho, 2, 1, 60, 1200});
    updateTable();
}

PerfilesDlg::~PerfilesDlg() {
    saveProfiles();
    delete ui;
}





void PerfilesDlg::loadProfiles() {
    QSettings settings("Ag Industries", "Ender3-S1 5W");
    QString jsonString = settings.value("material_profiles").toString();

    //_perfiles.clear();
    if (!jsonString.isEmpty()) {
        QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
        QJsonArray array = doc.array();
        for (const auto &val : array) {
            _perfiles.append(PerfilMaterial::formJson(val.toObject()));
        }
    }
}

void PerfilesDlg::saveProfiles() {
    QJsonArray array;
    for (const auto &p : _perfiles) {
        array.append(p.toJson());
    }
    QSettings settings("Ag Industries", "Ender3-S1 5W");
    settings.setValue("material_profiles", QString::fromUtf8(QJsonDocument(array).toJson()));
}
void PerfilesDlg::updateTable() {
    ui->tableWidget->setRowCount(0); // Limpiamos tabla

    for (const auto &p : _perfiles) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        // Alias
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(p.alias));

        // Operacion
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(p.grabado ? "Grabado" : "Corte"));
        // Tipo (usamos nuestro helper del enum)
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(Mat2Str(p.material)));

        // Valores numéricos (convertidos a string)
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(p.profundidad)));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(p.pasadas)));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(p.potencia)));
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(QString::number(p.velocidad)));
    }
}

void PerfilesDlg::on_btn_add_clicked() {
    _perfiles.push_back({ui->txt_alias->text(),
                         ui->txt_operacion->currentIndex() == 0,
                         Str2Mat(ui->txt_material->currentText()),
                         ui->txt_profundidad->text().toDouble(),
                         ui->txt_pasadas->text().toInt(),
                         ui->txt_potencia->text().toInt(),
                         ui->txt_velocidad->text().toInt()});

    updateTable();


    ui->txt_alias->clear();
    ui->txt_pasadas->clear();
    ui->txt_potencia->clear();
    ui->txt_profundidad->clear();
    ui->txt_velocidad->clear();
}

PerfilMaterial PerfilesDlg::perfil() const {
    return _perfil;
}

void PerfilesDlg::showMenu(const QPoint &pos) {
    // Convertir la posición del click a coordenadas de la tabla
    // QTableWidgetItem *item = m_table->itemAt(pos);
    QTableWidgetItem *item = ui->tableWidget->currentItem();
    if (!item) return; // Si no hay nada seleccionado, no mostrar menú

    QMenu menu(this);
    QAction *actEliminar = menu.addAction("Eliminar Perfil");
    QAction *actSeleccionar = menu.addAction("Seleccionar Perfil");

    // Ejecutar el menú y guardar la acción elegida
    QAction *selectedAction = menu.exec(ui->tableWidget->viewport()->mapToGlobal(pos));

    // Lógica según la acción elegida
    if (selectedAction == actEliminar) {
        if (QMessageBox::question(this,
                                  "Confirmar",
                                  "¿Desea eliminar el perfil " + ui->tableWidget->item(item->row(), 0)->text() + "?")
            != QMessageBox::Yes)
            return;
        for (int i = 0; i < _perfiles.size(); i++) {
            if (ui->tableWidget->item(item->row(), 0)->text() != _perfiles[i].alias) continue;
            _perfiles.erase(_perfiles.begin() + i);
            break;
        }
        ui->tableWidget->removeRow(item->row());
    } else if (selectedAction == actSeleccionar) {
        if (QMessageBox::question(this,
                                  "Confirmar",
                                  "¿Desea aplicar el perfil " + ui->tableWidget->item(item->row(), 0)->text() + "?")
            != QMessageBox::Yes)
            return;
        for (int i = 0; i < _perfiles.size(); i++) {
            if (ui->tableWidget->item(item->row(), 0)->text() != _perfiles[i].alias) continue;
            _perfil = _perfiles[i];
            break;
        }
        this->accept();
    }
}

void PerfilesDlg::on_btn_filtro_clicked() {
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        bool mostrar = true;
        for (int n = 0; n < 3; n++) {
            QTableWidgetItem *item = ui->tableWidget->item(i, 1 + n);
            QString f;
            switch (n) {
            case 0:
                f = ui->combo_op->currentText();
                break;
            case 1:
                f = ui->combo_mat->currentText();
                break;
            case 2:
                f = ui->combo_prof->currentText();
                break;
            }
            mostrar = mostrar && (item->text() == f || f == "");
            if (!mostrar) break;
        }


        ui->tableWidget->setRowHidden(i, !mostrar);
    }
}

void PerfilesDlg::on_tableWidget_cellChanged(int row, int column) {
    switch (column) {
    case 0:
        _perfiles[row].alias = ui->tableWidget->item(row, column)->text();
        break;
    case 3:
        _perfiles[row].profundidad = ui->tableWidget->item(row, column)->text().toDouble();
        break;
    case 4:
        _perfiles[row].pasadas = ui->tableWidget->item(row, column)->text().toInt();
        break;
    case 5:
        _perfiles[row].potencia = ui->tableWidget->item(row, column)->text().toInt();
        break;
    case 6:
        _perfiles[row].velocidad = ui->tableWidget->item(row, column)->text().toInt();
        break;
    }
}
