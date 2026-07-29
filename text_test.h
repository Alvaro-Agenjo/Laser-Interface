#ifndef TEXT_TEST_H
#define TEXT_TEST_H

#include <QDialog>
#include "proceso.h"

namespace Ui {
class Text_test;
}

class Text_test : public QDialog {
    Q_OBJECT

public:
    explicit Text_test(QWidget *parent = nullptr);
    ~Text_test();

    const std::vector<Proceso*>& getProcesosGenerados() const { return _p; } // Nuevo método

private:
    Ui::Text_test *ui;

    std::vector<Proceso*> _p;
    float _margen = 1.5; //mm
    float _maxW = -1;
    float _maxH = -1;

    bool check( QString* respponse);
    void genetareTest();
};

#endif // TEXT_TEST_H
