#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Gestores/administrador.h"

#include "GUI/registroclientes.h"
#include "GUI/registrostock.h"
#include "GUI/registroventas.h"
#include "qprogressdialog.h"

#include <QWebEngineView>
#include <QPrinter>
#include <QTextDocument>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Administrador *administrador,QWidget *parent = nullptr);

    ~MainWindow();

    void on_action_Fullscreen_triggered();

    friend class GestorVentas;

    void keyPressEvent( QKeyEvent * event );

private slots:

    //void print(QPrinter *printer);

    //inicializadores

    void inicializarTabla();

    void inicializarPrecios();

    void inicializarComboBoxClientes();

    void inicializarComboBoxProductos();

    void setValidaciones();

    void setFecha();

    void rellenarTableVentasToday();

    //

    void actualizarTotal();

    //eventos clicked

    void on_btn_RegistroStock_clicked();

    void on_btn_RegistroClientes_clicked();

    void on_btn_AgregarVenta_clicked();

    void on_btn_Salir_clicked();

    void on_btn_EliminarVentaDeTabla_clicked();

    void on_btn_GenerarPDF_TablaVentas_clicked();

    void on_btn_MostrarPrecios_clicked();

    void on_btn_RegistroVentas_clicked();

    void on_checkBox_isMayorista_2_stateChanged(int arg1);

    void on_checkBox_isMayorista_stateChanged(int arg1);

private:

    //QPrinter *printer;
    //QTextDocument documento;

    /*Los uso par la imprecion de PDF*/
    QWebEngineView *modeloWeb;

    Ui::MainWindow *uiVentanaPrincipal;

    Administrador *administrador;

    RegistroStock*  VentanaStock;

    RegistroClientes* VentanaCliente;

    RegistroVentas* VentanaVentas;

    QProgressDialog mensajeEspera;

    unsigned ultimaFilaConDatos;

    enum Columna{
       HORA,NOMBRE_CLIENTE,NOMBRE_PRODUCTO,CANTIDAD,PRECIO_UNIDAD,TOTAL,ID
    };

    const QString PARTICULAR = "Particular";
    const QString SIN_DIRECCION= "Sin Direccion";
    const QString SELECCIONAR_PRODUCTO= "Seleccionar Producto";

};
#endif // MAINWINDOW_H
