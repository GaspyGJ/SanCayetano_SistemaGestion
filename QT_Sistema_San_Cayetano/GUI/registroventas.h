#ifndef REGISTROVENTAS_H
#define REGISTROVENTAS_H

#include "Gestores/administrador.h"
#include <QMainWindow>

namespace Ui {
class RegistroVentas;
}

class RegistroVentas : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegistroVentas(Administrador *administrador,QWidget *parent = nullptr);
    ~RegistroVentas();

    void keyPressEvent( QKeyEvent * event );


private slots:
    void on_btn_ventas_por_producto_clicked();

    void on_btn_ventas_por_cliente_clicked();

    void on_btn_ventas_clicked();

    void on_btn_volver_clicked();

    void on_btn_ventas_bidones_propios_clicked();

    void setTotal(int nombre_tabla, QString fecha_desde, QString fecha_hasta);

    void on_pushButton_clicked();

private:

    Ui::RegistroVentas *ui;

    QHash<QString, QString> nroMes;

    Administrador *administrador;

    enum nombre_tabla{
        V_PRODUCTOS,V_CLIENTE,V_GENERAL,V_BIDONES_PROPIOS
    };

};

#endif // REGISTROVENTAS_H
