//@@ DEBERIA CENTRAR LOS ITEMS DE LAS COLUMNAS PERO NO ENCONTRE MANERA
//los new QTableWidget ... deberia aplicarles delete???

//@@ EL ANCHO DE LAS COLUMNAS , cambiarlo para q entre bien el nombre del productisimo

//@@las verificaciones de los inputs hay que revisarlas mejor --> el tema de las , y los .

#include "GUI/registrostock.h"
#include "ui_registrostock.h"

#include <QMessageBox>

RegistroStock::RegistroStock(Administrador *administrador,QWidget *parent) :QMainWindow(parent),uiRegistroStock( new Ui::RegistroStock){
    uiRegistroStock->setupUi(this);

    setWindowTitle("Registro de Stock");

    this->administrador=administrador; // le asigno el administrador

    this->setValidaciones();

    this->inicializarTabla();

}


RegistroStock::~RegistroStock(){
    this->administrador=NULL;
    delete uiRegistroStock;
}

void RegistroStock::keyPressEvent( QKeyEvent * event ){
    if( event->key() == Qt::Key_Escape ){
        this->isFullScreen()? this->setWindowState(Qt::WindowMaximized):this->setWindowState(Qt::WindowFullScreen);
    }
}


void RegistroStock::setValidaciones(){

    //##Inicializacion Validaciones Sobre los Inputs

    //validator(minimo,maximo,decimales,padre de la validacion)
    QDoubleValidator *validatorDOUBLE = new QDoubleValidator(0.00,5000.00,2,this);
    validatorDOUBLE->setNotation(QDoubleValidator::StandardNotation); // esto porq daba problemas con las notaciones con comas


    QIntValidator *validatorINT = new QIntValidator(-5000,5000,this);

    uiRegistroStock->input_Precio->setValidator( validatorDOUBLE );

    uiRegistroStock->input_Cantidad->setValidator( validatorINT );

    // delete validatorINT;
    // delete validatorDOUBLE;

    //## Fin de la inicializacion Validaciones Sobre los Inputs

}

void RegistroStock::inicializarTabla(){

    //### Inicializacion sobre la tabla
    this->ultimaFilaConDatos=-1;

    //agrego a la tabla los datos de los productos registrados en el gestor
    this->rellenarTableProduct();

    //Para que las Columans de las tablas sea de vista dinamica
    uiRegistroStock->table_Stock->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    uiRegistroStock->table_Stock->horizontalHeader()->setSectionResizeMode(ID,QHeaderView::Fixed);
    uiRegistroStock->table_Stock->resizeColumnToContents(ID);

    uiRegistroStock->table_Stock->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //para que la seleccion sea de la fila entera
    uiRegistroStock->table_Stock->setSelectionBehavior(QAbstractItemView::SelectRows);

    //para que no aparezca el numero de la fila como una columna mas
    uiRegistroStock->table_Stock->verticalHeader()->setVisible(false);

    //altura de cada fila
    QHeaderView *verticalHeader = uiRegistroStock->table_Stock->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(40);


    //### Fin de la inicializacion sobre la tabla
}

/*void RegistroStock::inicializarComboBox(){


    this->uiRegistroStock->comboBox_seleccionarProducto->clear();
    //inicializar combo box

    this->uiRegistroStock->comboBox_seleccionarProducto->addItem(Lista_PrimerOpcion);

    QStringList nombresProductos;

    foreach(Producto *p, this->administrador->getGestorProductos()->getAll_Productos() ){
        nombresProductos.push_back(p->getNombre());
    }

    foreach(QString nombre, nombresProductos ){
        this->uiRegistroStock->comboBox_seleccionarProducto->addItem(nombre);
    }

}*/

void RegistroStock::rellenarTableProduct(){//agarra todos los productos del gestor y los coloca en la tabla

    uiRegistroStock->table_Stock->setRowCount(0);

    //@@debo eliminar estos punteros???
    GestorProductos *GP=this->administrador->getGestorProductos();

    auto productos = GP->getAll_Productos();

    unsigned nroFila = this->ultimaFilaConDatos;

    short contador = 1;

    foreach(auto p ,productos){

        QString nombreProducto =  p->getNombre();
        QString precioProducto = QString::number(p->getPrecio()) +" $";
        QString precioMayoristaProducto = QString::number(p->getPrecio_mayorista()) +" $";
        QString fechaProducto = p->getUltima_actualizacion();

        QString cantidadProducto;
        if(nombreProducto.contains("Granel")){
            cantidadProducto="-";
        }
        else{
           cantidadProducto = p->getCantidad_stock();
        }

        nroFila+=1;

        uiRegistroStock->table_Stock->insertRow(nroFila );


        uiRegistroStock->table_Stock->setItem(nroFila ,
                                              ID,
                                              new QTableWidgetItem(QString::number(contador)));

        uiRegistroStock->table_Stock->setItem(nroFila ,
                                              NOMBRE,
                                              new QTableWidgetItem(nombreProducto));

        uiRegistroStock->table_Stock->setItem(nroFila ,
                                              PRECIO,
                                              new QTableWidgetItem(precioProducto));
        uiRegistroStock->table_Stock->setItem(nroFila ,
                                              PRECIO_MAYORISTA,
                                              new QTableWidgetItem(precioMayoristaProducto));
        uiRegistroStock->table_Stock->setItem(nroFila ,
                                              CANTIDAD,
                                              new QTableWidgetItem(cantidadProducto));
        uiRegistroStock->table_Stock->setItem(nroFila,
                                              FECHA_INCORPORACION,
                                              new QTableWidgetItem(fechaProducto));

        contador++;
    }

}

void RegistroStock::on_btn_ActualizarStock_clicked(){

    int filaSeleccionada = uiRegistroStock->table_Stock->currentRow();
    QString stockAnterior = uiRegistroStock->table_Stock->model()->index(filaSeleccionada,CANTIDAD).data().toString();
    QString nombreProducto = uiRegistroStock->table_Stock->model()->index(filaSeleccionada,NOMBRE).data().toString();

    if(stockAnterior=="-"){
        QMessageBox::information( this,"Operacion no realizada", "No es posible cambiar el stock de un producto a Granel");
        return;
    }

    if(filaSeleccionada !=-1){

        QString nuevaCantidad = uiRegistroStock->input_Cantidad->text();

        if(nuevaCantidad!="" and !nuevaCantidad.contains(',') and !nuevaCantidad.contains('.')){



            QMessageBox::StandardButton vConfirmacion = QMessageBox::question( this,"Actualizar Stock",
                                                                               "Se modificara el stock de "+nombreProducto+"\n Sumando "+nuevaCantidad+" al stock total",
                                                                               QMessageBox::Cancel|QMessageBox::Yes);

            if (vConfirmacion == QMessageBox::Yes){


                short stockActualizado = nuevaCantidad.toShort()+stockAnterior.toShort();

                if( this->administrador->getGestorProductos()->modificarCantidad(nombreProducto,nuevaCantidad) == 1 ){
                    uiRegistroStock->table_Stock->setItem(filaSeleccionada ,
                                                          CANTIDAD,
                                                          new QTableWidgetItem( QString::number(stockActualizado) ));

                    QDate *fecha = new QDate();
                    uiRegistroStock->table_Stock->setItem(filaSeleccionada ,
                                                          FECHA_INCORPORACION,
                                                          new QTableWidgetItem( fecha->currentDate().toString("dd-MM-yyyy") ));
                    delete fecha;

                    //por si se habia puesto rojo antes
                    uiRegistroStock->label_Cantidad->setStyleSheet("#label_Cantidad{color: rgb(250, 250, 250);}");
                }


            }

        }
        else{
            uiRegistroStock->label_Cantidad->setStyleSheet("#label_Cantidad{color: rgb(255, 0, 0);}");
            QMessageBox::warning( this,"Error al actualizar Stock", "Coloque un valor correcto en la casilla de 'Cantidad' ");
        }


        uiRegistroStock->input_Cantidad->setText("");
    }

    else{
        QMessageBox::warning( this,"Error al actualizar Stock", "Seleccione una fila a actualizar y rellene la casilla 'Cantidad'");
    }

}

void RegistroStock::on_btn_ActualizarPrecio_clicked(){

    int filaSeleccionada = uiRegistroStock->table_Stock->currentRow();
    QString precioAnterior = uiRegistroStock->table_Stock->model()->index(filaSeleccionada,PRECIO).data().toString();
    QString nombreProducto = uiRegistroStock->table_Stock->model()->index(filaSeleccionada,NOMBRE).data().toString();



    if(filaSeleccionada ==-1  ){ // SI selecciono una fila
        QMessageBox::warning( this,"Error al actualizar Stock", "Seleccione una fila a actualizar y rellene la casilla 'Precio'");
        return;
    }


    QString nuevoPrecio = uiRegistroStock->input_Precio->text().replace(',','.'); //la bbdd funciona con . por eso

    if(nuevoPrecio!=""){

        QMessageBox::StandardButton vConfirmacion = QMessageBox::question( this,"Actualizar Precio",
                                                                           "Se modificara el precio de "+nombreProducto+"\n Precio Anterior : "
                                                                           + precioAnterior + "\n Precio Actualizado : "+ nuevoPrecio + " $",
                                                                           QMessageBox::Cancel|QMessageBox::Yes);

        if (vConfirmacion == QMessageBox::Yes){

            if( this->administrador->getGestorProductos()->modificarPrecio(nombreProducto,nuevoPrecio) == 1 ){


                this->rellenarTableProduct();

                //por si se habia puesto rojo antes
                uiRegistroStock->label_Precio->setStyleSheet("#label_Precio{color: rgb(250, 250, 250);}");

                emit this->precioActualizado();

            }

        }
    }
    else{

        uiRegistroStock->label_Precio->setStyleSheet("#label_Precio{color: rgb(255, 0, 0);}");
        QMessageBox::warning( this,"Error al actualizar Precio  ", "Coloque un valor en la casilla de 'Precio' ");

    }

    uiRegistroStock->input_Precio->setText("");
}

/* Lo dejo por las dudas , asi era cunando estaba el combo box
void RegistroStock::on_btn_ActualizarPrecio_clicked(){

    int filaSeleccionada = uiRegistroStock->table_Stock->currentRow();
    QString precioAnterior;
    QString nombreProducto;


    if(filaSeleccionada !=-1  && uiRegistroStock->comboBox_seleccionarProducto->currentText()==Lista_PrimerOpcion){ // SI selecciono una fila y NO selecciono combobbox

        precioAnterior = uiRegistroStock->table_Stock->model()->index(filaSeleccionada,PRECIO).data().toString();

        nombreProducto = uiRegistroStock->table_Stock->model()->index(filaSeleccionada,NOMBRE).data().toString();

    }
    else if(uiRegistroStock->comboBox_seleccionarProducto->currentText()!=Lista_PrimerOpcion){ //Selecciono el combo box  (con la fila puede pasar cualquier caso)

        auto producto = administrador->getGestorProductos()->getProducto(uiRegistroStock->comboBox_seleccionarProducto->currentText());


        // despues utilizo el filaSeleccionada para actualizar la tabla .. por eso le asgino el valor aca

        filaSeleccionada = administrador->getGestorProductos()->find_by_Nombre(uiRegistroStock->comboBox_seleccionarProducto->currentText())-1;

        uiRegistroStock->table_Stock->selectRow(filaSeleccionada);

        precioAnterior = QString::number(producto->getPrecio())+" $";

        nombreProducto = producto->getNombre();
    }
    else{ // no selecciono ninguno
        QMessageBox::warning( this,"Error al actualizar Stock", "Seleccione una fila a actualizar o seleccione el nombre del producto y rellene la casilla 'Precio'");
        return;
    }

    QString nuevoPrecio = uiRegistroStock->input_Precio->text().replace(',','.');

    if(nuevoPrecio!=""){

        QMessageBox::StandardButton vConfirmacion = QMessageBox::question( this,"Actualizar Precio",
                                                                           "Se modificara el precio de "+nombreProducto+"\n Precio Anterior : "
                                                                           + precioAnterior + "\n Precio Actualizado : "+ nuevoPrecio + " $",
                                                                           QMessageBox::Cancel|QMessageBox::Yes);

        if (vConfirmacion == QMessageBox::Yes){

            if( this->administrador->getGestorProductos()->modificarPrecio(nombreProducto,nuevoPrecio) == 1 ){


                this->rellenarTableProduct();

                //por si se habia puesto rojo antes
                uiRegistroStock->label_Precio->setStyleSheet("#label_Precio{color: rgb(250, 250, 250);}");

                emit this->precioActualizado();

            }

        }
    }
    else{

        uiRegistroStock->label_Precio->setStyleSheet("#label_Precio{color: rgb(255, 0, 0);}");
        QMessageBox::warning( this,"Error al actualizar Precio  ", "Coloque un valor en la casilla de 'Precio' ");

    }

    uiRegistroStock->input_Precio->setText("");
}

*/
void RegistroStock::on_btn_ActualizarPrecio_mayorista_clicked(){
    int filaSeleccionada = uiRegistroStock->table_Stock->currentRow();
    QString precioAnterior = uiRegistroStock->table_Stock->model()->index(filaSeleccionada,PRECIO_MAYORISTA).data().toString();
    QString nombreProducto = uiRegistroStock->table_Stock->model()->index(filaSeleccionada,NOMBRE).data().toString();


    if(filaSeleccionada ==-1 ){ // SI selecciono una fila y NO selecciono combobbox

        QMessageBox::warning( this,"Error al actualizar Stock", "Seleccione una fila a actualizar y rellene la casilla 'Precio'");
        return;
    }


    QString nuevoPrecio = uiRegistroStock->input_Precio_mayorista->text().replace(',','.');
    if(nuevoPrecio!=""){

        QMessageBox::StandardButton vConfirmacion = QMessageBox::question( this,"Actualizar Precio mayorista",
                                                                           "Se modificara el precio mayorista de "+nombreProducto+"\n Precio Anterior : "
                                                                           + precioAnterior + "\n Precio Actualizado : "+ nuevoPrecio + " $",
                                                                           QMessageBox::Cancel|QMessageBox::Yes);

        if (vConfirmacion == QMessageBox::Yes){

            if( this->administrador->getGestorProductos()->modificarPrecioMayorista(nombreProducto,nuevoPrecio) == 1 ){


                this->rellenarTableProduct();

                //por si se habia puesto rojo antes
                uiRegistroStock->label_Precio_3->setStyleSheet("#label_Precio{color: rgb(250, 250, 250);}");


                emit this->precioActualizado();
            }

        }
    }
    else{

        uiRegistroStock->label_Precio->setStyleSheet("#label_Precio{color: rgb(255, 0, 0);}");
        QMessageBox::warning( this,"Error al actualizar Precio  ", "Coloque un valor en la casilla de 'Precio' ");

    }

    uiRegistroStock->input_Precio_mayorista->setText("");
}



void RegistroStock::on_btn_GenerarPDF_Stock_clicked(){
    QMessageBox::information(0,"No implementado" , "Esta opcion no fue implementada todavia.");

}


void RegistroStock::on_btn_Volver_clicked(){

    this->~RegistroStock();
}






