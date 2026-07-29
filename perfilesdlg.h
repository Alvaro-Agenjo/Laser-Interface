#ifndef PERFILESDLG_H
#define PERFILESDLG_H

#include <QDialog>
#include "PerfilMaterial.h"


namespace Ui {
class PerfilesDlg;
}

class PerfilesDlg : public QDialog {
    Q_OBJECT

public:
    explicit PerfilesDlg(QWidget *parent = nullptr);
    ~PerfilesDlg();

    void loadProfiles();
    void saveProfiles();
    PerfilMaterial perfil() const;

private slots:
    void on_btn_add_clicked();
    void on_btn_filtro_clicked();
    void on_tableWidget_cellChanged(int row, int column);

private:
    Ui::PerfilesDlg *ui;
    QList<PerfilMaterial> _perfiles;
    PerfilMaterial _perfil;

    void updateTable();
    void showMenu(const QPoint &pos);
};

#endif // PERFILESDLG_H
