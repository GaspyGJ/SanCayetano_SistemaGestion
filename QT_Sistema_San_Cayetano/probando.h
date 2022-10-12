#ifndef PROBANDO_H
#define PROBANDO_H

#include <QMainWindow>

namespace Ui {
class Probando;
}

class Probando : public QMainWindow
{
    Q_OBJECT

public:
    explicit Probando(QWidget *parent = nullptr);
    ~Probando();

private:
    Ui::Probando *ui;
};

#endif // PROBANDO_H
