#ifndef CUT_TEST_H
#define CUT_TEST_H

#include <QDialog>
#include "proceso.h"

namespace Ui {
class Cut_test;
}

class Cut_test : public QDialog {
    Q_OBJECT

public:
    explicit Cut_test(QWidget *parent = nullptr);
    ~Cut_test();
    const std::vector<Proceso*>& getProcesosGenerados() const { return _p; }

private:
    Ui::Cut_test *ui;
    std::vector<Proceso*> _p;
    float _margen = 1.5; //mm

    bool check( QString* respponse);
    void genetareTest();
};

#endif // CUT_TEST_H
