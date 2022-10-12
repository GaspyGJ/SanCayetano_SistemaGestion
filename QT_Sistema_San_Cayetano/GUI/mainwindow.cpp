
//#include "Extra/tableprinter.h"
//#include <QPrinter>
//#include <QPainter>
//#include <QPdfWriter>
//#include <QPrintPreviewDialog>

#include "ui_mainwindow.h"
#include "GUI/mainwindow.h"
#include "Extra/styleitem.h"
#include "Extra/styleitem.h"
#include "Extra/containerexpandible.h"

#include <QSqlQueryModel>

#include <QTextTableCell>
#include <QMessageBox>

#include<QWebEngineView>
#include <QProgressDialog>

#include<QStandardPaths>
#include<QDir>


MainWindow::MainWindow(Administrador *administrador,QWidget *parent):QMainWindow(parent),uiVentanaPrincipal(new Ui::MainWindow){

    uiVentanaPrincipal->setupUi(this);

    setWindowTitle("Sistema San Cayetano");

    /*ESTILOS QUE NO FUNCIONAN EN QSS NOSE PORQUE*/

    uiVentanaPrincipal->comboBox_Productos->setStyleSheet("QComboBox QAbstractItemView {min-width: 300px; min-height: 600px; background-color:#505152;}\
                                                          QComboBox::down-arrow {image:  url(./SanCayetanoIMG/down.ico); }");
    uiVentanaPrincipal->comboBox_Clientes->setStyleSheet("QComboBox QAbstractItemView {min-width: 300px;background-color:#505152;}\
                                                          QComboBox::down-arrow {image:  url(./SanCayetanoIMG/down.ico); }");

    uiVentanaPrincipal->btn_MostrarPrecios->setIcon(QIcon("./SanCayetanoIMG/right.ico"));
    uiVentanaPrincipal->btn_MostrarPrecios->setIconSize(QSize(15,15));
    /**Fin Estilos**/

    this->administrador= administrador;

    inicializarTabla();

    inicializarComboBoxClientes();

    inicializarComboBoxProductos();

    inicializarPrecios();

    setValidaciones();

    setFecha();


    uiVentanaPrincipal->input_PrecioMayoristaEspecial->setDisabled(true);


    QPushButton *btn_notShow = new QPushButton;
    btn_notShow->setDisabled(true); // PARA QUE NO SE MUESTRE EL BOTON
    btn_notShow->setStyleSheet("background-color:#121212; border:none");
    mensajeEspera.setCancelButton(btn_notShow);
    mensajeEspera.close();
}

void MainWindow::on_btn_Salir_clicked(){
    QCoreApplication::quit();
}

void MainWindow::keyPressEvent( QKeyEvent * event ){ //Salir de fullscrean

    if( event->key() == Qt::Key_Escape ){
        this->isFullScreen()? this->setWindowState(Qt::WindowMaximized):this->setWindowState(Qt::WindowFullScreen);
    }
}

MainWindow::~MainWindow(){
    delete uiVentanaPrincipal;
}

void MainWindow::setValidaciones(){

    QIntValidator *validatorINT = new QIntValidator(0,5000,this);

    uiVentanaPrincipal->input_Cantidad->setValidator( validatorINT );


    QDoubleValidator *validatorDOUBLE = new QDoubleValidator(0.00,5000.00,2,this);
    validatorDOUBLE->setNotation(QDoubleValidator::StandardNotation); // esto porq daba problemas con las notaciones con comas
    uiVentanaPrincipal->input_PrecioMayoristaEspecial->setValidator( validatorDOUBLE );


    //delete validatorINT; --> no hago esta sentencia , porq sino se elimina la validacion justamente.
}

void MainWindow::setFecha(){

    QDate *fecha = new QDate();

    uiVentanaPrincipal->label_fechaDeHoy->setText( fecha->currentDate().toString("dd-MM-yyyy") );

    delete fecha;
}

void MainWindow::inicializarComboBoxClientes(){

    uiVentanaPrincipal->comboBox_Clientes->clear();

    auto listaClientes = this->administrador->getGestorClientes()->getAll_Clientes();

    foreach(Cliente *c, listaClientes){

        if( c->getNombre() != PARTICULAR){

            foreach(Establecimiento *e , c->getAll_Establecimientos() ){
                if( !(e->getDireccion().isEmpty()) or e->getDireccion()!= SIN_DIRECCION){
                     uiVentanaPrincipal->comboBox_Clientes->addItem( c->getNombre() +"\n"+ e->getDireccion());
                }
                else{
                    uiVentanaPrincipal->comboBox_Clientes->addItem( c->getNombre() );
                }
            }
        }
        else{
            uiVentanaPrincipal->comboBox_Clientes->addItem( c->getNombre() );
        }
    }

}

void MainWindow::inicializarComboBoxProductos(){

    uiVentanaPrincipal->comboBox_Productos->clear();
    uiVentanaPrincipal->comboBox_Productos->addItem(SELECCIONAR_PRODUCTO);
    auto listaProductos = this->administrador->getGestorProductos()->getAll_Productos();

    foreach(Producto *p, listaProductos){
        uiVentanaPrincipal->comboBox_Productos->addItem(p->getNombre());
    }

}

void MainWindow::inicializarTabla(){

    //### Inicializacion sobre la tabla
    this->ultimaFilaConDatos=-1;

    // tuve q hacer esa clase nueva para sobreescribir un metodo de otra clase para poder quitar los "..." de la tabla.
    uiVentanaPrincipal->table_Ventas->setItemDelegate(new StyleItem());

    //para q no se vean los 3 puntitos cuando un elemento es muy grande
    uiVentanaPrincipal->table_Ventas->setTextElideMode(Qt::ElideNone);

    //altura de cada fila
    QHeaderView *verticalHeader = uiVentanaPrincipal->table_Ventas->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(50);

    //agrego a la tabla los datos de los productos registrados en el gestor
    this->rellenarTableVentasToday();

    //Oculto la columna ID  --> la coloco porq la venta si tiene un ID en la BBDD pero no la muestro aqui
    uiVentanaPrincipal->table_Ventas->setColumnHidden(ID,true);

    //para que la seleccion sea de la fila entera
    uiVentanaPrincipal->table_Ventas->setSelectionBehavior(QAbstractItemView::SelectRows);

    //para que no aparezca el numero de la fila como una columna mas
    uiVentanaPrincipal->table_Ventas->verticalHeader()->setVisible(false);

    //Disposicion de las columnas
    uiVentanaPrincipal->table_Ventas->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    uiVentanaPrincipal->table_Ventas->setEditTriggers(QAbstractItemView::NoEditTriggers);

    uiVentanaPrincipal->table_Ventas->horizontalHeader()->setSectionResizeMode(CANTIDAD,QHeaderView::Fixed);
    uiVentanaPrincipal->table_Ventas->setColumnWidth(CANTIDAD,130);

    uiVentanaPrincipal->table_Ventas->horizontalHeader()->setSectionResizeMode(PRECIO_UNIDAD,QHeaderView::Fixed);
    uiVentanaPrincipal->table_Ventas->setColumnWidth(PRECIO_UNIDAD,130);

    uiVentanaPrincipal->table_Ventas->horizontalHeader()->setSectionResizeMode(TOTAL,QHeaderView::Fixed);
    uiVentanaPrincipal->table_Ventas->setColumnWidth(TOTAL,130);

    uiVentanaPrincipal->table_Ventas->horizontalHeader()->setSectionResizeMode(HORA,QHeaderView::Fixed);
    uiVentanaPrincipal->table_Ventas->resizeColumnToContents(HORA);

    //### Fin de la inicializacion sobre la tabla
}

void MainWindow::inicializarPrecios(){

    //@@ESTO SE PODRIA MEJORAR. MUCHO TEXTO

    auto productos = this->administrador->getGestorProductos()->getAll_Productos();

    QString precio , stock;

    short contador = 1;
    foreach(auto p, productos){

        precio = QString::number(p->getPrecio());
        //precio_mayorista = QString::number(p->getPrecio_mayorista())+ " $";
        stock = p->getCantidad_stock();

        switch( contador ){

        /*Aclaracion --> nose q paso que los nombres no se podian poner sin los _numero , no me dejab*/

        case 1: uiVentanaPrincipal->label_Precio_AguaDeMesaGranel1L->setText(precio);
            uiVentanaPrincipal->label_Precio_AguaDeMesaGranel1L->setStyleSheet("color:#2dbd5a");
            uiVentanaPrincipal->Mlabel_Precio_AguaDeMesaGranel1L->setText(stock);
            uiVentanaPrincipal->Mlabel_Precio_AguaDeMesaGranel1L->setStyleSheet("color:rgb(237, 208, 111)");break;

        case 2: uiVentanaPrincipal->label_Precio_AguaDestiladaGranel->setText(precio);
            uiVentanaPrincipal->label_Precio_AguaDestiladaGranel->setStyleSheet("color:#2dbd5a");
            uiVentanaPrincipal->Mlabel_Precio_AguaDestiladaGranel->setText(stock);
            uiVentanaPrincipal->Mlabel_Precio_AguaDestiladaGranel->setStyleSheet("color:rgb(237, 208, 111)");break;

        case 3: uiVentanaPrincipal->label_Precio_AguaDestilada1L->setText(precio);
            uiVentanaPrincipal->label_Precio_AguaDestilada1L->setStyleSheet("color:#2dbd5a");
            uiVentanaPrincipal->Mlabel_Precio_AguaDestilada1L->setText(stock);
            uiVentanaPrincipal->Mlabel_Precio_AguaDestilada1L->setStyleSheet("color:rgb(237, 208, 111)");break;

        case 4: uiVentanaPrincipal->label_Precio_AguaDestilada5L->setText(precio);
            uiVentanaPrincipal->label_Precio_AguaDestilada5L->setStyleSheet("color:#2dbd5a");
            uiVentanaPrincipal->Mlabel_Precio_AguaDestilada5L->setText(stock);
            uiVentanaPrincipal->Mlabel_Precio_AguaDestilada5L->setStyleSheet("color:rgb(237, 208, 111)");break;

        case 5: uiVentanaPrincipal->label_Precio_LiquidoRefrigerante1L->setText(precio);
            uiVentanaPrincipal->label_Precio_LiquidoRefrigerante1L->setStyleSheet("color:#2dbd5a");
            uiVentanaPrincipal->Mlabel_Precio_LiquidoRefrigerante1L->setText(stock);
            uiVentanaPrincipal->Mlabel_Precio_LiquidoRefrigerante1L->setStyleSheet("color:rgb(237, 208, 111)");break;

        case 6: uiVentanaPrincipal->label_Precio_LiquidoRefrigerante5L->setText(precio);
            uiVentanaPrincipal->label_Precio_LiquidoRefrigerante5L->setStyleSheet("color:#2dbd5a");
            uiVentanaPrincipal->Mlabel_Precio_LiquidoRefrigerante5L->setText(stock);
            uiVentanaPrincipal->Mlabel_Precio_LiquidoRefrigerante5L->setStyleSheet("color:rgb(237, 208, 111)");break;

        case 7: uiVentanaPrincipal->label_Precio_BolsaHieloMolido10KG->setText(precio);
            uiVentanaPrincipal->label_Precio_BolsaHieloMolido10KG->setStyleSheet("color:#2dbd5a");
            uiVentanaPrincipal->Mlabel_Precio_BolsaHieloMolido10KG->setText(stock);
            uiVentanaPrincipal->Mlabel_Precio_BolsaHieloMolido10KG->setStyleSheet("color:rgb(237, 208, 111)");break;

        case 8: uiVentanaPrincipal->label_Precio_BolsaHieloRolito10KG->setText(precio);
            uiVentanaPrincipal->label_Precio_BolsaHieloRolito10KG->setStyleSheet("color:#2dbd5a");
            uiVentanaPrincipal->Mlabel_Precio_BolsaHieloRolito10KG->setText(stock);
            uiVentanaPrincipal->Mlabel_Precio_BolsaHieloRolito10KG->setStyleSheet("color:rgb(237, 208, 111)");break;

        case 9: uiVentanaPrincipal->label_Precio_BolsaHieloRolito14KG->setText(precio);
            uiVentanaPrincipal->label_Precio_BolsaHieloRolito14KG->setStyleSheet("color:#2dbd5a");
            uiVentanaPrincipal->Mlabel_Precio_BolsaHieloRolito14KG->setText(stock);
            uiVentanaPrincipal->Mlabel_Precio_BolsaHieloRolito14KG->setStyleSheet("color:rgb(237, 208, 111)");break;

        case 10: uiVentanaPrincipal->label_Precio_BolsaHieloRolito4KG->setText(precio);
            uiVentanaPrincipal->label_Precio_BolsaHieloRolito4KG->setStyleSheet("color:#2dbd5a");
            uiVentanaPrincipal->Mlabel_Precio_BolsaHieloRolito4KG->setText(stock);
            uiVentanaPrincipal->Mlabel_Precio_BolsaHieloRolito4KG->setStyleSheet("color:rgb(237, 208, 111)");break;

        case 11: uiVentanaPrincipal->label_Precio_LiquidoRefrigeranteGranel->setText(precio);
            uiVentanaPrincipal->label_Precio_LiquidoRefrigeranteGranel->setStyleSheet("color:#2dbd5a");
            uiVentanaPrincipal->Mlabel_Precio_LiquidoRefrigeranteGranel->setText(stock);
            uiVentanaPrincipal->Mlabel_Precio_LiquidoRefrigeranteGranel->setStyleSheet("color:rgb(237, 208, 111)");break;

        case 12: uiVentanaPrincipal->label_Precio_Pack3AguaDestilada->setText(precio);
            uiVentanaPrincipal->label_Precio_Pack3AguaDestilada->setStyleSheet("color:#2dbd5a");
            uiVentanaPrincipal->Mlabel_Precio_Pack3AguaDestilada->setText(stock);
            uiVentanaPrincipal->Mlabel_Precio_Pack3AguaDestilada->setStyleSheet("color:rgb(237, 208, 111)");break;

        case 13: uiVentanaPrincipal->label_Precio_Pack3LiquidoRefrigerante->setText(precio);
            uiVentanaPrincipal->label_Precio_Pack3LiquidoRefrigerante->setStyleSheet("color:#2dbd5a");
            uiVentanaPrincipal->Mlabel_Precio_Pack3LiquidoRefrigerante->setText(stock);
            uiVentanaPrincipal->Mlabel_Precio_Pack3LiquidoRefrigerante->setStyleSheet("color:rgb(237, 208, 111)");break;

        case 14: uiVentanaPrincipal->label_Precio_Pack10AguaDestilada->setText(precio);
            uiVentanaPrincipal->label_Precio_Pack10AguaDestilada->setStyleSheet("color:#2dbd5a");
            uiVentanaPrincipal->Mlabel_Precio_Pack10AguaDestilada->setText(stock);
            uiVentanaPrincipal->Mlabel_Precio_Pack10AguaDestilada->setStyleSheet("color:rgb(237, 208, 111)");break;

        case 15: uiVentanaPrincipal->label_Precio_Pack10LiquidoRefrigerante->setText(precio);
            uiVentanaPrincipal->label_Precio_Pack10LiquidoRefrigerante->setStyleSheet("color:#2dbd5a");
            uiVentanaPrincipal->Mlabel_Precio_Pack10LiquidoRefrigerante->setText(stock);
            uiVentanaPrincipal->Mlabel_Precio_Pack10LiquidoRefrigerante->setStyleSheet("color:rgb(237, 208, 111)");break;

        case 16: uiVentanaPrincipal->label_Precio_AguaDeMesa20L->setText(precio);
            uiVentanaPrincipal->label_Precio_AguaDeMesa20L->setStyleSheet("color:#2dbd5a");
            uiVentanaPrincipal->Mlabel_Precio_AguaDeMesa20L->setText(stock);
            uiVentanaPrincipal->Mlabel_Precio_AguaDeMesa20L->setStyleSheet("color:rgb(237, 208, 111)");break;

        }

        contador++;
    }

}

void MainWindow::rellenarTableVentasToday(){
    //DESACTIVAR SIGNALS -- //@@Esto para q era? me olvide XD , capaz no es para nada o capas si sirve
     bool oldState = uiVentanaPrincipal->table_Ventas->blockSignals(true);

    uiVentanaPrincipal->table_Ventas->setRowCount(0); // elimino las posibles filas (esto es mas que nada para cuando elimino un Cliente)

    GestorVentas *GV=this->administrador->getGestorVentas();

    unsigned nroFila = this->ultimaFilaConDatos;

    foreach(Venta *v ,GV->getAll_Ventas_Today()){

        QString id_Venta = QString::number(v->getID());
        QString nombre_cliente  = v->getCliente();
        QString nombre_Producto =  v->getProducto();

        short cantidad_Producto = v->getCantidad();

        double precio_Producto = v->getPrecio_producto();

        double total = v->getPrecio_total();

        QString hora_Venta = v->getHora_venta();

        nroFila+=1;

        uiVentanaPrincipal->table_Ventas->insertRow(nroFila );

        uiVentanaPrincipal->table_Ventas->setItem(nroFila ,
                                                  NOMBRE_CLIENTE,
                                                  new QTableWidgetItem( nombre_cliente ));

        uiVentanaPrincipal->table_Ventas->setItem(nroFila ,
                                                  NOMBRE_PRODUCTO,
                                                  new QTableWidgetItem( nombre_Producto ));

        uiVentanaPrincipal->table_Ventas->setItem(nroFila ,
                                                  CANTIDAD,
                                                  new QTableWidgetItem( QString::number(cantidad_Producto) ));

        uiVentanaPrincipal->table_Ventas->setItem(nroFila,
                                                  PRECIO_UNIDAD,
                                                  new QTableWidgetItem( QString::number(precio_Producto) ) );

        uiVentanaPrincipal->table_Ventas->setItem(nroFila,
                                                  TOTAL,
                                                  new QTableWidgetItem( QString::number(total) ) );

        uiVentanaPrincipal->table_Ventas->setItem(nroFila,
                                                  HORA,
                                                  new QTableWidgetItem( hora_Venta ) );

        uiVentanaPrincipal->table_Ventas->setItem(nroFila ,
                                                  ID,
                                                  new QTableWidgetItem( id_Venta ));

    }

    //HABILITO LAS SENIALES
    uiVentanaPrincipal->table_Ventas->blockSignals(oldState);

    actualizarTotal();
}

void MainWindow::actualizarTotal(){

    double total = 0;

    for(int fila=0; fila<uiVentanaPrincipal->table_Ventas->rowCount() ;fila++){

        total+= uiVentanaPrincipal->table_Ventas->model()->index(fila,TOTAL).data().toDouble();
    }

    uiVentanaPrincipal->label_Total->setText( QString::number(total) );
}

void MainWindow::on_checkBox_isMayorista_stateChanged(int estado){

    if(estado==2){ // se activo
       uiVentanaPrincipal->checkBox_isMayorista_2->setCheckState(Qt::Unchecked); // DESCHECKEO EL OTRO CHECK
    }

}


void MainWindow::on_checkBox_isMayorista_2_stateChanged(int estado){

   if(estado==2){ // se activo
       uiVentanaPrincipal->input_PrecioMayoristaEspecial->setDisabled(false);
       uiVentanaPrincipal->input_PrecioMayoristaEspecial->setMinimumHeight(30);

       uiVentanaPrincipal->checkBox_isMayorista->setCheckState(Qt::Unchecked); // DESCHECKEO EL OTRO CHECK
   }
   else if(estado==0){//se desactivo
      uiVentanaPrincipal->input_PrecioMayoristaEspecial->setDisabled(true);
      uiVentanaPrincipal->input_PrecioMayoristaEspecial->setMinimumHeight(5);
   }


}



void MainWindow::on_btn_AgregarVenta_clicked(){

    if(uiVentanaPrincipal->comboBox_Productos->currentText()==SELECCIONAR_PRODUCTO){
        QMessageBox::critical(0,"Error" , "Por favor, seleccione un producto");
        return;
    }

    if( ! uiVentanaPrincipal->input_Cantidad->text().isEmpty() ){

        short cantidad_Producto = uiVentanaPrincipal->input_Cantidad->text().toShort();

        QString nombre_Cliente = uiVentanaPrincipal->comboBox_Clientes->currentText();

        QString nombre_Producto = uiVentanaPrincipal->comboBox_Productos->currentText();
        double precio_Producto;

        if(uiVentanaPrincipal->checkBox_isMayorista->isChecked()){
            precio_Producto = this->administrador->getGestorProductos()->getProducto(nombre_Producto)->getPrecio_mayorista();
            if(precio_Producto==0){
                QMessageBox::critical(0,"Error" , "El producto seleccionado no tiene asignado un precio mayorista");
                return;
            }
        }
        else if(uiVentanaPrincipal->checkBox_isMayorista_2->isChecked()){
            QString precioIngresado = uiVentanaPrincipal->input_PrecioMayoristaEspecial->text().replace(',','.');
            if( ! precioIngresado.isEmpty() ){
                precio_Producto = precioIngresado.toDouble(); // tomo el precio del input
            }
            else{
                QMessageBox::critical(0,"Error" , "Por favor ingrese el precio mayorista especial");
                return;
            }
        }
        else{//si no checkeamos ninguno de los do2 checks, el precio es el precio normal
            precio_Producto = this->administrador->getGestorProductos()->getProducto(nombre_Producto)->getPrecio();
        }

        QString hora_Venta= QDateTime::currentDateTime().toString("hh:mm") + " hrs";

        QString fecha_Venta = uiVentanaPrincipal->label_fechaDeHoy->text();

        if(this->administrador->getGestorVentas()->crearVentaToday(cantidad_Producto,precio_Producto,fecha_Venta,hora_Venta,nombre_Producto,nombre_Cliente) == 1){

            this->rellenarTableVentasToday();

            if( this->administrador->getGestorProductos()->getProducto(nombre_Producto)->getCantidad_stock()!="-" ){
                this->administrador->getGestorProductos()->modificarCantidad(nombre_Producto,"-"+QString::number(cantidad_Producto) );
            }

            QMessageBox::information(0,"OK" , "La Venta se creó correctamente");

            uiVentanaPrincipal->input_Cantidad->clear();
            uiVentanaPrincipal->checkBox_isMayorista->setCheckState(Qt::Unchecked);

            uiVentanaPrincipal->input_PrecioMayoristaEspecial->clear();
            uiVentanaPrincipal->checkBox_isMayorista_2->setCheckState(Qt::Unchecked);

            this->inicializarComboBoxProductos();
            this->inicializarComboBoxClientes();

        }
        else{
            QMessageBox::critical(0,"Error" , "No se creó la Venta");
        }

    }
    else{
        QMessageBox::critical(0,"Error" , "Rellene la casilla de cantidad antes de agregar una venta");
    }

}

void MainWindow::on_btn_EliminarVentaDeTabla_clicked(){

    int filaSeleccionada = uiVentanaPrincipal->table_Ventas->currentRow();

    unsigned int id= uiVentanaPrincipal->table_Ventas->model()->index(filaSeleccionada,ID).data().toUInt();

    QString nombre_Producto= uiVentanaPrincipal->table_Ventas->model()->index(filaSeleccionada,NOMBRE_PRODUCTO).data().toString();

    QString cantidad_Producto = uiVentanaPrincipal->table_Ventas->model()->index(filaSeleccionada,CANTIDAD).data().toString();

    if( this->administrador->getGestorVentas()->eliminarVenta_Today(id) == 1 ){

        this->rellenarTableVentasToday();

        this->administrador->getGestorProductos()->modificarCantidad(nombre_Producto,cantidad_Producto);

        QMessageBox::information(0,"OK" , "La Venta se elimino Correctamente");
    }
    else{
        QMessageBox::critical(0,"Error" , "La venta NO se elimino");
    }

}

void MainWindow::on_btn_MostrarPrecios_clicked(){

    if( uiVentanaPrincipal->gBox_PreciosProducto->isVisible() ){
        uiVentanaPrincipal->gBox_PreciosProducto->setVisible(false);//lo oculto

        uiVentanaPrincipal->btn_MostrarPrecios->setIcon(QIcon("./SanCayetanoIMG/left.ico"));
        uiVentanaPrincipal->btn_MostrarPrecios->setIconSize(QSize(15,15));

    }
    else{
        uiVentanaPrincipal->gBox_PreciosProducto->setVisible(true);

        uiVentanaPrincipal->btn_MostrarPrecios->setIcon(QIcon("./SanCayetanoIMG/right.ico"));
        uiVentanaPrincipal->btn_MostrarPrecios->setIconSize(QSize(15,15));
    }
}

void MainWindow::on_btn_RegistroVentas_clicked(){

    VentanaVentas = new RegistroVentas(this->administrador,this);

    //connect( VentanaVentas , SIGNAL(  precioActualizado() )  ,  this  ,  SLOT( inicializarPrecios() ) );

    VentanaVentas->setWindowState(Qt::WindowFullScreen);
    VentanaVentas->show();
}

void MainWindow::on_btn_RegistroStock_clicked(){

    VentanaStock = new RegistroStock(this->administrador,this);

    connect( VentanaStock , SIGNAL(  precioActualizado() )  ,  this  ,  SLOT( inicializarPrecios() ) );

    VentanaStock->setWindowState(Qt::WindowFullScreen);
    VentanaStock->show();
}

void MainWindow::on_btn_RegistroClientes_clicked(){

    VentanaCliente = new RegistroClientes(this->administrador,this);

    connect( VentanaCliente , SIGNAL(  actualizacionNombre_or_Direcciones()  )  ,  this  ,  SLOT( inicializarComboBoxClientes() ) );

    VentanaCliente->setWindowState(Qt::WindowFullScreen);
    VentanaCliente->show();
}

void MainWindow::on_btn_GenerarPDF_TablaVentas_clicked(){

    QString htmlFilas;

    QString casilla;

    for(short f=0; f<uiVentanaPrincipal->table_Ventas->rowCount();f++){
        htmlFilas+= "<tr> <th>"+QString::number(f)+"</th>";
        for(short c=0; c<uiVentanaPrincipal->table_Ventas->columnCount()-1;c++){
            casilla = uiVentanaPrincipal->table_Ventas->model()->index(f,c).data().toString();
            htmlFilas+= "<td>"+casilla+"</td>";
        }
        htmlFilas+="</tr>";
    }

    QString hora= QDateTime::currentDateTime().toString("hh:mm") + " hrs";

    /* QFile tabla("../Extras/SanCayetanoHTML/tabla_Ventas.html");
    tabla.open(QFile::ReadOnly);
    QString htmlConCss1 = tabla.readAll();
    tabla.close();*/

    QString htmlConCss=
            "<!DOCTYPE html>\
            <head>\
            <title>Document</title>\
            <style>\
            img {\
            width: 100px;\
            height: 100px;\
}\
        #img-h2-img{\
            display: flex;\
            justify-content: space-between;\
            align-items: center;\
}\
            p{\
            text-align: center;\
}\
            th,\
            td {\
            border: 1px solid;\
}\
            table {\
            width: 100%;\
            border-collapse: collapse;\
}\
            table tr {\
            height: 35px;\
            border: 1px solid;\
}\
            table td {\
            border: 1px solid;\
            text-align: center;\
}\
            table #NroColumna {\
            width: 2%;\
}\
            table #HoraRegistro {\
            width: 5%;\
}\
            table #Cliente {\
            width: 15%;\
}\
            table #Producto {\
            width: 15%;\
}\
            table #Cantidad {\
            width: 10%;\
}\
            table #PrecioUnidad {\
            width: 10%;\
}\
            table #PrecioTotal {\
            width: 10%;\
}\
            tr td p{\
            margin: 0;\
            font-weight: bold;\
}\
            </style>\
            </head>\
            <body>\
            <div id=\"img-h2-img\">\
            <img src=\"iconoSinFondo.png\">\
            <div>\
            <h1>SAN CAYETANO</h1>\
            <p>Tabla Ventas ("+uiVentanaPrincipal->label_fechaDeHoy->text()+" | "+ hora +")</p>\
            </div>\
            <img src=\"iconoSinFondo-Invertido.png\">\
            </div>\
            <table>\
            <thead>\
            <tr>\
            <th id=\"NroColumna\" >#</th>\
            <th id=\"HoraRegistro\">Hora Registro</th>\
            <th id=\"Cliente\">Cliente</th>\
            <th id=\"Producto\">Producto</th>\
            <th id=\"Cantidad\">Cantidad</th>\
            <th id=\"PrecioUnidad\">P. Unidad</th>\
            <th id=\"PrecioTotal\">P. Total</th>\
            </tr>\
            </thead >\
            <tbody>\
            "+htmlFilas+"\
            <tr>\
            <th>T</th>\
            <td>-</td>\
            <td>-</td>\
            <td>-</td>\
            <td>-</td>\
            <td>-</td>\
            <td> <p>Total = "+uiVentanaPrincipal->label_Total->text()+" $</p></td>\
            </tr>\
            </tbody>\
            </table>\
            </body>\
            </html>";

            //******************************* METODOS DE IMPRECION QUE FUNCIONAN ***********************************.

            // 1)--------------------------------------------------


    modeloWeb = new QWebEngineView();

    mensajeEspera.open();

    mensajeEspera.setMinimum(0);
    mensajeEspera.setMaximum(100);
    mensajeEspera.setLabelText("Exportando PDF por favor aguarde...");



    mensajeEspera.setWindowModality(Qt::WindowModal);


    mensajeEspera.setValue(0);

    bool ok= connect(modeloWeb, &QWebEngineView::loadFinished, this, [this,htmlConCss](){

        mensajeEspera.setValue(75);
        modeloWeb->page()->printToPdf(QString("%1/"+uiVentanaPrincipal->label_fechaDeHoy->text()+"_Ventas.pdf").arg(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)));
        mensajeEspera.setValue(100);
        mensajeEspera.close();
        QMessageBox::information(0,"OK" , "Se creó el PDF correctamente en el escritorio");

    });
    if(ok){
        //coloco el HTML y la base q toma el HTML para cargar imagenes o otras cosas externas
        mensajeEspera.setValue(25);
        //modeloWeb->setHtml(htmlConCss,QUrl(QDir::currentPath()+"/../SanCayetanoIMG/"));
        modeloWeb->setHtml(htmlConCss,QUrl(QDir::currentPath()+"/SanCayetanoIMG/"));
        mensajeEspera.setValue(55);
    }

    // 2)--------------------------------------------------

    /*
printer = new QPrinter(QPrinter::PrinterResolution);
printer->setOutputFormat(QPrinter::PdfFormat);
printer->setPageSize(QPageSize::A4);
printer->setPageMargins(QMargins(0,0,0,0));
printer->setOutputFileName("C:/Users/CompuGaspy/Desktop/Ventas_"+uiVentanaPrincipal->label_fechaDeHoy->text()+".pdf");
documento.setDocumentMargin(0.0);
documento.setHtml(htmlConCss);
//documento.print(printer);// --> se puede imprimir antes sin el PrintPreviewDialog
QPrintPreviewDialog dialog(printer);
connect(&dialog, &QPrintPreviewDialog::paintRequested, this, [this](){
documento.print(printer);
});
dialog.exec();
*/
    // 3)-------------------------------------------------

    /*
QPrintPreviewDialog dialog;
connect(&dialog, SIGNAL(paintRequested( QPrinter*) ), this, SLOT( print(QPrinter*)) );
dialog.exec();

//Funcion para el Metodo 3 de imprecion --> utiliza archivos externos
void MainWindow::print(QPrinter *printer) {

    // ------------------ simplest example --------------------------
    QPainter painter;
    if(!painter.begin(printer)) {
        qWarning() << "can't start printer";
        return;
    }
    // print table
    TablePrinter tablePrinter(&painter, printer);
    QVector<int> columnStretch = QVector<int>() << 4 << 10 << 10 << 2 << 2 <<2 << 0;
    if(!tablePrinter.printTable(uiVentanaPrincipal->table_Ventas->model(), columnStretch)) {
        qDebug() << tablePrinter.lastError();
    }
    painter.end();
}
*/

//--------------------EXPORTA A EXEL MEDIANTE CSV  (lo puse aca asi lo puede comprimir en una linea nomas)-----------------------------------------
/*           // [Collect model data to QString]
                    QString textData;
                    int rows = uiVentanaPrincipal->table_Ventas->rowCount();
                    int columns = uiVentanaPrincipal->table_Ventas->columnCount()-1;

                    //add the name columns

                        textData += "Hora , Nombre-Cliente , Nombre-Producto , Cantidad ,PrecioUnidad ,Total-Venta\n";


                    for (int i = 0; i < rows; i++) {
                        for (int j = 0; j < columns; j++) {

                                QString datoTable = uiVentanaPrincipal->table_Ventas->model()->index(i,j).data().toString().remove("\n");
                                textData += datoTable;
                                textData += ", ";     // for .csv file format
                        }
                        textData += "\n";             // (optional: for new line segmentation)
                    }

                    textData += ",,,,, TOTAL = "+uiVentanaPrincipal->label_Total->text()+"\n";

                    // [Save to file] (header file <QFile> needed)
                    // .csv

                    QFile csvFile("../test.csv");
                    if(csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {

                        QTextStream out(&csvFile);
                        out << textData;

                        csvFile.close();
                    }

                    // .txt
                    QFile txtFile("../test.txt");
                    if(txtFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {

                        QTextStream out(&txtFile);
                        out << textData;

                        txtFile.close();}
        }*/

}

