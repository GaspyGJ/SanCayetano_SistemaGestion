#include "ui_registroventas.h"
#include "registroventas.h"
#include "qevent.h"

#include "qsqlquery.h"
#include "qsqlquerymodel.h"
#include "Gestores/gestorbasedatos.h"

#include <QMessageBox>


RegistroVentas::RegistroVentas(Administrador *administrador, QWidget *parent) :QMainWindow(parent), ui(new Ui::RegistroVentas){
    ui->setupUi(this);

    setWindowTitle("Registro de Ventas");

    this->administrador=administrador;


    ui->cb_Desde->setStyleSheet("QComboBox QAbstractItemView {min-height: 420px; background-color:#505152;}\
                                                          QComboBox::down-arrow {image:  url(./SanCayetanoIMG/down.ico); }");
    ui->cb_Hasta->setStyleSheet("QComboBox QAbstractItemView {min-height: 420px; background-color:#505152;}\
                                QComboBox::down-arrow {image:  url(./SanCayetanoIMG/down.ico); }");


    //Indico los meses
    this->nroMes.insert("Enero", "01");
    this->nroMes.insert("Febrero", "02");
    this->nroMes.insert("Marzo", "03");
    this->nroMes.insert("Abril", "04");
    this->nroMes.insert("Mayo", "05");
    this->nroMes.insert("Junio", "06");
    this->nroMes.insert("Julio", "07");
    this->nroMes.insert("Agosto", "08");
    this->nroMes.insert("Septiembre", "09");
    this->nroMes.insert("Octubre", "10");
    this->nroMes.insert("Noviembre", "11");
    this->nroMes.insert("Diciembre", "12");

    //altura de cada fila
    QHeaderView *verticalHeader = ui->tableView->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(50);

    //para que la seleccion sea de la fila entera
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //para que no aparezca el numero de la fila como una columna mas
    ui->tableView->verticalHeader()->setVisible(false);


    //Disposicion de las columnas
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);


    //para q no se vean los 3 puntitos cuando un elemento es muy grande
    ui->tableView->setTextElideMode(Qt::ElideNone);


}
RegistroVentas::~RegistroVentas(){
    delete ui;
}


void RegistroVentas::keyPressEvent( QKeyEvent * event ){
    if( event->key() == Qt::Key_Escape ){
        this->isFullScreen()? this->setWindowState(Qt::WindowMaximized):this->setWindowState(Qt::WindowFullScreen);
    }
}



void RegistroVentas::on_btn_ventas_por_producto_clicked(){
    QSqlQueryModel * model = new QSqlQueryModel();

    GestorBaseDatos::ConectarDB();
    GestorBaseDatos::abrirDB();
    QSqlQuery* qry = new QSqlQuery(*GestorBaseDatos::getBBDD());


    //SELECCIONAMOS CUAL SERA LA CONSULTA EN BASE A LOS seleccionados

    QString mesInicio=ui->cb_Desde->currentText();
    QString mesFin=ui->cb_Hasta->currentText();


    QString fecha_desde = QString::number(QDate::currentDate().year())+this->nroMes[mesInicio]+"01";

    QString fecha_hasta = QString::number(QDate::currentDate().year())+this->nroMes[mesFin]+"31";


    //where fecha_Venta between '12-09-2022' and '13-09-2022'

    QString consulta = "select nombreP as Producto, count(nombreP) as Cantidad_Ventas ,sum(cantidad_Producto) as Cantidad_Comprados, sum(total_Venta) || '  $' as Total_Ingresado\
            from Ventas\
            where substr(fecha_Venta,7) || substr(fecha_Venta,4,2) || substr (fecha_Venta,1,2)\
            between '"+fecha_desde+"' and '"+fecha_hasta+"'\
            group by nombreP;";


    qDebug()<<"Se ejecuto la consulta : "<<consulta;

    qry->prepare(consulta);

    qry->exec();
    model->setQuery(std::move(*qry));
    delete qry;
    ui->tableView->setModel(model);


    setTotal(V_PRODUCTOS,fecha_desde,fecha_hasta);
}


void RegistroVentas::on_btn_ventas_por_cliente_clicked(){
    QSqlQueryModel * model = new QSqlQueryModel();

    GestorBaseDatos::ConectarDB();
    GestorBaseDatos::abrirDB();
    QSqlQuery* qry = new QSqlQuery(*GestorBaseDatos::getBBDD());


    QString mesInicio=ui->cb_Desde->currentText();
    QString mesFin=ui->cb_Hasta->currentText();


    QString fecha_desde = QString::number(QDate::currentDate().year())+this->nroMes[mesInicio]+"01";

    QString fecha_hasta = QString::number(QDate::currentDate().year())+this->nroMes[mesFin]+"31";

    //where fecha_Venta between '12-09-2022' and '13-09-2022'

    QString consulta = "select nombreC as Cliente, count(nombreC) as Cantidad_Ventas ,sum(cantidad_Producto) as Cantidad_Prod_Comprados, sum(total_Venta) || '  $' as Total_Ingresado\
            from Ventas\
            where substr(fecha_Venta,7) || substr(fecha_Venta,4,2) || substr (fecha_Venta,1,2)\
            between '"+fecha_desde+"' and '"+fecha_hasta+"'\
            group by nombreC;";

    qDebug()<<"Se ejecuto la consulta : "<<consulta;

    qry->prepare(consulta);


    qry->exec();
    model->setQuery(std::move(*qry));
    delete qry;
    ui->tableView->setModel(model);


    setTotal(V_CLIENTE,fecha_desde,fecha_hasta);

}


void RegistroVentas::on_btn_ventas_clicked(){

    QSqlQueryModel * model = new QSqlQueryModel();

    GestorBaseDatos::ConectarDB();
    GestorBaseDatos::abrirDB();
    QSqlQuery* qry = new QSqlQuery(*GestorBaseDatos::getBBDD());


    QString mesInicio=ui->cb_Desde->currentText();
    QString mesFin=ui->cb_Hasta->currentText();


    QString fecha_desde = QString::number(QDate::currentDate().year())+this->nroMes[mesInicio]+"01";

    qDebug()<<"El mes es : "<<this->nroMes[mesInicio];
     qDebug()<<"El mesInicio : "<<mesInicio;

    QString fecha_hasta = QString::number(QDate::currentDate().year())+this->nroMes[mesFin]+"31";
    //where fecha_Venta between '12-09-2022' and '13-09-2022'

    QString consulta = "select *\
            from Ventas\
            where substr(fecha_Venta,7) || substr(fecha_Venta,4,2) || substr (fecha_Venta,1,2)\
            between '"+fecha_desde+"' and '"+fecha_hasta+"';";

    qDebug()<<"Se ejecuto la consulta : "<<consulta;

    qry->prepare(consulta);

    qry->exec();
    model->setQuery(std::move(*qry));
    delete qry;
    ui->tableView->setModel(model);

    setTotal(V_GENERAL,fecha_desde,fecha_hasta);
}

void RegistroVentas::on_btn_ventas_bidones_propios_clicked()
{

    QSqlQueryModel * model = new QSqlQueryModel();

    GestorBaseDatos::ConectarDB();
    GestorBaseDatos::abrirDB();
    QSqlQuery* qry = new QSqlQuery(*GestorBaseDatos::getBBDD());


    QString mesInicio=ui->cb_Desde->currentText();
    QString mesFin=ui->cb_Hasta->currentText();


    QString fecha_desde = QString::number(QDate::currentDate().year())+this->nroMes[mesInicio]+"01";

    QString fecha_hasta = QString::number(QDate::currentDate().year())+this->nroMes[mesFin]+"31";
    //where fecha_Venta between '12-09-2022' and '13-09-2022'


    QString consulta = "select Fecha_Venta as Fecha , nombreC as Cliente , nombreP as Producto,sum(cantidad_Producto) as Cantidad_Bidones\
            from Ventas\
            where substr(fecha_Venta,7) || substr(fecha_Venta,4,2) || substr (fecha_Venta,1,2)\
            between '"+fecha_desde+"' and '"+fecha_hasta+"'\
            and nombreP='Bidon Agua de Mesa 20 [L]'\
            group by nombreC,Fecha_Venta;";


    qDebug()<<"Se ejecuto la consulta : "<<consulta;

    qry->prepare(consulta);

    qry->exec();
    model->setQuery(std::move(*qry));
    delete qry;
    ui->tableView->setModel(model);

    setTotal(V_BIDONES_PROPIOS,fecha_desde,fecha_hasta);

}


void RegistroVentas::setTotal(int nombre_tabla, QString fecha_desde, QString fecha_hasta){

    GestorBaseDatos::abrirDB();

    QSqlQuery *consulta= new QSqlQuery(*GestorBaseDatos::getBBDD());

    bool mostrar=false;

    switch(nombre_tabla){

    case V_PRODUCTOS: consulta->prepare( "select sum(Total_Ingresado) as total\
                from (select nombreP as Producto, count(nombreP) as Cantidad_Ventas ,sum(cantidad_Producto) as Cantidad_Comprados, sum(total_Venta) || '  $' as Total_Ingresado\
                      from Ventas\
                      where substr(fecha_Venta,7) || substr(fecha_Venta,4,2) || substr (fecha_Venta,1,2)\
                      between '"+fecha_desde+"' and '"+fecha_hasta+"'\
                      group by nombreP);");
                mostrar=true;



                 qDebug()<<"Se ejecuto : select sum(Total_Ingresado) as total\
                           from Ventas\
                           where substr(fecha_Venta,7) || substr(fecha_Venta,4,2) || substr (fecha_Venta,1,2)\
                           between '"+fecha_desde+"' and '"+fecha_hasta+"'\
                           group by nombreP;";


                break;

    case V_CLIENTE:    consulta->prepare( "select sum(Total_Ingresado) as total\
                from (select nombreC as Cliente, count(nombreC) as Cantidad_Ventas ,sum(cantidad_Producto) as Cantidad_Prod_Comprados, sum(total_Venta) || '  $' as Total_Ingresado\
                      from Ventas\
                      where substr(fecha_Venta,7) || substr(fecha_Venta,4,2) || substr (fecha_Venta,1,2)\
                      between '"+fecha_desde+"' and '"+fecha_hasta+"'\
                      group by nombreC);");
                mostrar=true;


                qDebug()<<"Se ejecuto : select sum(Total_Ingresado) as total\
                          from Ventas\
                          where substr(fecha_Venta,7) || substr(fecha_Venta,4,2) || substr (fecha_Venta,1,2)\
                          between '"+fecha_desde+"' and '"+fecha_hasta+"'\
                          group by nombreC;";

                break;

    case V_GENERAL:  consulta->prepare("select sum(total_venta) as total\
                from Ventas\
                where substr(fecha_Venta,7) || substr(fecha_Venta,4,2) || substr (fecha_Venta,1,2)\
                between '"+fecha_desde+"' and '"+fecha_hasta+"';");

                mostrar=true;

                qDebug()<<"Se ejecuto : select sum(total_venta) as total\
                          from Ventas\
                          where substr(fecha_Venta,7) || substr(fecha_Venta,4,2) || substr (fecha_Venta,1,2)\
                          between '"+fecha_desde+"' and '"+fecha_hasta+"';";


                break;

    case V_BIDONES_PROPIOS: /*No hago nada porq no hay precios q sumar*/  ui->label_total->setText("---- $"); break;

    }


    if(mostrar==true){

        if( ! consulta->exec() ){
            qDebug()<<"Error al ejecutar la consulta SQL GESTOR PRODUCTO";
        }

        while( consulta->next() ){

            QString total =consulta->value("total").toString();

            ui->label_total->setText(total+" $");

        }
    }

    delete consulta;
    GestorBaseDatos::cerrarDB();
}


void RegistroVentas::on_pushButton_clicked(){
        QMessageBox::information(0,"No implementado" , "Esta opcion no fue implementada todavia.");
}


void RegistroVentas::on_btn_volver_clicked(){
    this->~RegistroVentas();
}










