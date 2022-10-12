#include "probando.h"
#include "ui_probando.h"

Probando::Probando(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Probando)
{
    ui->setupUi(this);
}

Probando::~Probando()
{
    delete ui;
}
