#ifndef REGISTROCLIENTES_H
#define REGISTROCLIENTES_H

#include <QMainWindow>
#include "Gestores/administrador.h"
#include "qtablewidget.h"
#include "qevent.h"

namespace Ui {
class RegistroClientes;
}

class RegistroClientes : public QMainWindow{

    Q_OBJECT // palabra clave que ejecuta un codigo para el funcionamineto de las ventanas (creo)

public:

    explicit RegistroClientes(Administrador *administrador,QWidget *parent = nullptr); //Porq explicit???
    ~RegistroClientes();

    //setters
    void setValidaciones();

    void inicializarTabla();
    void rellenarTableClientes();

    void inicializarComboBox();

    enum Columna{
       ID,NOMBRE,TELEFONO,DIRECCION  //Publica porque lo uso en el Gestor De Clientes. sino no pordria acceder
    };

    void keyPressEvent( QKeyEvent * event );

signals:

    void actualizacionNombre_or_Direcciones();

private slots:

    //Eventos de Clicked

    void on_btn_Volver_clicked();

    void on_btn_CrearCliente_clicked();

    void on_btn_EliminarCliente_clicked();

    void on_btn_AgregarDireccion_clicked();

    void on_btn_EliminarDireccion_clicked();


    //Eventos de Cambios

    void on_table_Clientes_itemChanged(QTableWidgetItem *item); // cuando un item cambia de valor

    void on_table_Clientes_itemSelectionChanged(); // cuando cambia de item seleccionado --> lo uso para obtener la fila seleccionada en realidad

    void on_btn_GenerarPDF_Clientes_clicked();

private:

    Ui::RegistroClientes *uiRegistroClientes; //puntero a la interfaz de la ventana

    Administrador *administrador;

    int ultimaFilaConDatos; //contiene la ultima fila de la tabla con datos cargados , sirve para ir cargando los datos en secuencia.

    QStringList valoresAnterioresDelItem;//cuando se posiciona sobre una fila se guardan estos datos--> [nombre,telefono,direccion]
                                        //por si ocurre un cambio y no fue exitoso volver a los valores anteriores

    const QString Lista_PrimerOpcion ="Seleccionar cliente";
    const QString PARTICUTAR ="Particular";

};

#endif // REGISTROCLIENTES_H
