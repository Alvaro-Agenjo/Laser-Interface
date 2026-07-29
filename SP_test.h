#ifndef SP_TEST_H
#define SP_TEST_H

#include <QDialog>
#include "proceso.h"

namespace Ui {
class Tests;
}

class Tests : public QDialog {
    Q_OBJECT

public:
    explicit Tests(QWidget *parent = nullptr);
    ~Tests();

    const std::vector<Proceso*>& getProcesosGenerados() const { return _p; } // Nuevo método

private:
    Ui::Tests *ui;
    std::vector<Proceso*> _p;
    float _margen = 2; //mm

    bool check( QString* respponse);
    void genetareTest();
};

#endif // SP_TEST_H
