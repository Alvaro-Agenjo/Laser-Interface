#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

#include "laserscene.h"
#include "proceso.h"
#include "gcodegenerator.h"
#include "PerfilMaterial.h"




QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    // Input proceso
    void on_txt_pasadas_valueChanged(int arg1);
    void on_txt_power_valueChanged(int arg1);
    void on_txt_vel_valueChanged(int arg1);


    // Input forma
    void on_txt_posX_valueChanged(double arg1);
    void on_txt_posY_valueChanged(double arg1);
    void on_txt_sizeX_valueChanged(double arg1);
    void on_txt_sizeY_valueChanged(double arg1);

    void on_txt_rot_valueChanged(double arg1);


    // Input operacion
    void on_sel_modo_currentIndexChanged(int index);
    void on_chck_original_stateChanged(int arg1);
    void on_chck_invertir_stateChanged(int arg1);
    void on_sel_simetria_currentIndexChanged(int index);

    void on_sld_bn_umbral_valueChanged(int value);
    void on_txt_bn_densidad_valueChanged(int arg1);

    void on_sld_gray_contraste_valueChanged(int value);
    void on_sld_gray_brillo_valueChanged(int value);
    void on_sld_gray_lvl_valueChanged(int value);
    void on_txt_gray_densidad_valueChanged(int arg1);

    void on_sld_vec_umbral_valueChanged(int value);


    // Input texto
    void on_check_bold_stateChanged(int arg1);
    void on_check_italic_stateChanged(int arg1);
    void on_check_underline_stateChanged(int arg1);

    void on_txt_tamao_valueChanged(int arg1);
    void on_txt_texto_editingFinished();
    void on_txt_fuente_currentFontChanged(const QFont &f);


    // Lista de procesos
    void CargarProceso(QListWidgetItem *item);
    void RenombrarProceso(QListWidgetItem *item);
    void AjustarPrioridad();
    void MostrarMenu(const QPoint &pos);


    // Botones
    void on_btn_fijar_toggled(bool checked);
    void on_btn_borrar_clicked();
    void on_btn_gcode_clicked();


    // Grafica
    void MovimientoDetectado(QGraphicsItem *item, QPointF pos);


    //Adicion de objetos
    void on_btn_addIMG_clicked();
    void on_btn_addRectangle_clicked();
    void on_btn_addText_clicked();


    // Tests
    void on_action_Speed_vs_Power_triggered();
    void on_action_Pasadas_triggered();


    // Desplegable
    void on_actionBorrar_objetos_triggered();
    void on_actionDuplicar_objeto_triggered();

    void on_actionMostrar_perfiles_triggered();

    void on_actionAcuracy_Text_triggered();

    void on_btn_addElipse_clicked();

private:
    Ui::MainWindow *ui;
    QMenuBar *_menubar;
    LaserScene *scene;

    std::vector<Proceso *> _procesos;
    Proceso *_proceso_actual;

    GCodeGenerator _gcode;
    void loadProcess(Proceso *proceso);
    void loadProcessData(Proceso *proceso);
    void loadProcessOperation(Proceso *proceso, bool texto);

    void setLock(bool close);
    void enableOperacion(bool setTo);
    void aplicarPerfil(const PerfilMaterial &m);
    void clearProcess();
};
#endif // MAINWINDOW_H
