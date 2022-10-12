#ifndef REGISTROSTOCK_H
#define REGISTROSTOCK_H

#include <QMainWindow>
#include "Gestores/administrador.h"
#include "qevent.h"

namespace Ui {
class RegistroStock;
}

class RegistroStock : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegistroStock(Administrador *administrador,QWidget *parent = nullptr);
    ~RegistroStock();

    //setters
    void setValidaciones();

    void inicializarTabla();

    void rellenarTableProduct();

    //void inicializarComboBox();

    void keyPressEvent( QKeyEvent * event );

signals:

    void precioActualizado();


private slots:

    //eventos clicked

    void on_btn_ActualizarStock_clicked();

    void on_btn_ActualizarPrecio_clicked();

    void on_btn_Volver_clicked();

    void on_btn_GenerarPDF_Stock_clicked();

    void on_btn_ActualizarPrecio_mayorista_clicked();

private:

    Ui::RegistroStock *uiRegistroStock; // puntero a la interfaz grafica

    Administrador *administrador;

    int ultimaFilaConDatos; //contiene la ultima fila de la tabla con datos cargados , sirve para ir cargando los datos en secuencia.

    const QString Lista_PrimerOpcion ="Seleccionar producto";

    enum Columna{
        ID,NOMBRE,PRECIO,PRECIO_MAYORISTA,CANTIDAD ,FECHA_INCORPORACION //enum para mejor legibilidad del ingreso de datos en la tabla
    };
};

#endif // REGISTROSTOCK_H
