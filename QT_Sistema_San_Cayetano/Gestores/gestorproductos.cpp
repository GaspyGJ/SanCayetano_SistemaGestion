//@@ FALTA QUE LA BASE DE DATOS SE "Desconecte" CREO

#include "Gestores/gestorproductos.h"
#include "Gestores/gestorbasedatos.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include <QMessageBox>


GestorProductos::GestorProductos(){

    this->inicializarConBaseDeDatos();
}

GestorProductos::~GestorProductos(){

    for(int i=0; i<this->list_Productos.length(); i++ ){
        delete this->list_Productos[i];
    }
}

void GestorProductos::inicializarConBaseDeDatos(){

    this->list_Productos.clear();

    GestorBaseDatos::abrirDB();

    QSqlQuery *consulta= new QSqlQuery(*GestorBaseDatos::getBBDD());

    consulta->prepare("select * from Productos order by nombre");

    if( ! consulta->exec() ){
        qDebug()<<"Error al ejecutar la consulta SQL GESTOR PRODUCTO";
    }

    while( consulta->next() ){ // mientras la consulta tenga datos sigo leyendo (fila por fila)

        QString nombre =consulta->value("nombre").toString();
        double precio = consulta->value("precio").toDouble();
        double precio_mayorista = consulta->value("precio_mayorista").toDouble();
        QString cantidad_stock = consulta->value("cant_Stock").toString();
        QString ultima_actualizacion =consulta->value("ultima_actualizacion").toString();

        agregarProducto(nombre,precio,precio_mayorista,cantidad_stock,ultima_actualizacion);

    }

    delete consulta;
    GestorBaseDatos::cerrarDB();

}


void GestorProductos::agregarProducto(QString nombre,double precio,double precio_mayorista, QString cantidad_stock,QString ultima_actualizacion){ // agrega un producto al vector de productos

    Producto *p = new Producto(nombre,precio,precio_mayorista,cantidad_stock,ultima_actualizacion);
    this->list_Productos.push_back(p);
}


QVector<Producto *> GestorProductos::getAll_Productos(){
    return  this->list_Productos;
}

short GestorProductos::find_by_Nombre(QString nombre){

    for(int i=0; i<this->list_Productos.length(); i++ ){

        if(this->list_Productos[i]->getNombre()==nombre){
            return i;
        }
    }

    return -1;
}


Producto* GestorProductos::getProducto(QString nombre){

    short pos = this->find_by_Nombre(nombre);
    if(pos!=-1){
        return this->list_Productos[pos];
    }

    return nullptr;
}


short GestorProductos::modificarPrecio(QString nombre,QString precio){

    short posProducto = this->find_by_Nombre(nombre);

    if ( posProducto != -1){


        GestorBaseDatos::ConectarDB();

        GestorBaseDatos::abrirDB();

        // Actualizo el precio en la BBDD
        QSqlQuery *consulta= new QSqlQuery(*GestorBaseDatos::getBBDD());

        QString query = "update Productos set precio="+precio+" where nombre='"+nombre+"';";
        consulta->prepare(query);


        GestorBaseDatos::getBBDD()->transaction(); //empiezo la transaccion

        if( ! consulta->exec() ){

            qDebug()<<"Error al ejecutar la consulta SQL";
            QMessageBox::critical(0,"Error de Base de Datos","Error al ejecutar la consulta SQL Update Precio Productos");

            delete consulta;

            GestorBaseDatos::cerrarDB();

            return -1;

        }
        else{

            GestorBaseDatos::getBBDD()->commit(); // confirmo la transaccion

            delete consulta;
            GestorBaseDatos::cerrarDB();

            //Fin de la actualizacion con la BBBDD

            //reinicializo y asi actualizo la informacion
            this->inicializarConBaseDeDatos();

            return 1;
        }
    }
    else{
        qDebug("El Producto no se encontro en el vector de Productos 1");
        QMessageBox::critical(0,"Error" , "El Producto no fue enconontrado");

        return -1;
    }

}

short GestorProductos::modificarPrecioMayorista(QString nombre,QString precio){

    short posProducto = this->find_by_Nombre(nombre);

    if ( posProducto != -1){


        GestorBaseDatos::ConectarDB();

        GestorBaseDatos::abrirDB();

        // Actualizo el precio en la BBDD
        QSqlQuery *consulta= new QSqlQuery(*GestorBaseDatos::getBBDD());

        QString query = "update Productos set precio_mayorista ="+precio+" where nombre='"+nombre+"';";
        consulta->prepare(query);
        qDebug()<<"Consulta ejecutada : "+query;

        GestorBaseDatos::getBBDD()->transaction(); //empiezo la transaccion

        if( ! consulta->exec() ){

            qDebug()<<"Error al ejecutar la consulta SQL";
            QMessageBox::critical(0,"Error de Base de Datos","Error al ejecutar la consulta SQL Update Precio mayorista Productos");

            delete consulta;

            GestorBaseDatos::cerrarDB();

            return -1;

        }
        else{

            GestorBaseDatos::getBBDD()->commit(); // confirmo la transaccion

            delete consulta;

            GestorBaseDatos::cerrarDB();

            //Fin de la actualizacion con la BBBDD

            //reinicializo y asi actualizo la informacion
            this->inicializarConBaseDeDatos();

            return 1;
        }
    }
    else{
        qDebug("El Producto no se encontro en el vector de Productos 1");
        QMessageBox::critical(0,"Error" , "El Producto no fue enconontrado");

        return -1;
    }

}

short GestorProductos::modificarCantidad(QString nombre,QString cantidad){


     if (nombre.contains("Granel")){
         return 1;
     }

    short posProducto = this->find_by_Nombre(nombre);

    if ( posProducto != -1){

       GestorBaseDatos::ConectarDB();

        GestorBaseDatos::abrirDB();

        // Actualizo el stock en la BBDD

        QSqlQuery *consulta= new QSqlQuery(*GestorBaseDatos::getBBDD());

        QString query = "update Productos set cant_Stock=cant_Stock+"+cantidad+" where nombre='"+nombre+"';";

        qDebug()<<"La consulta ejecutada es: "<< query;

        consulta->prepare(query);

         GestorBaseDatos::getBBDD()->transaction(); //empiezo la transaccion

        if( ! consulta->exec() ){

            qDebug()<<"Error al ejecutar la consulta SQL";
            QMessageBox::critical(0,"Error de Base de Datos","Error al ejecutar la consulta SQL Update Cantidad Productos");

             delete consulta;

            GestorBaseDatos::cerrarDB();

            return -1;
        }
        else{

            QDate *fecha = new QDate();

            QString query2 = "update Productos set ultima_actualizacion='"+fecha->currentDate().toString("dd-MM-yyyy")+"' where nombre='"+nombre+"';";

            consulta->prepare(query2);

            if( ! consulta->exec() ){

                qDebug()<<"Error al ejecutar la consulta SQL";
                qDebug()<<"El error de la  consulta 2 es = "<< GestorBaseDatos::getBBDD()->lastError();
                QMessageBox::critical(0,"Error de Base de Datos","Error al ejecutar la consulta SQL Update Fecha incorporacion");

                delete fecha;

                delete consulta;

                GestorBaseDatos::cerrarDB();

                return -1;
            }
            else{

                GestorBaseDatos::getBBDD()->commit(); // confirmo la transaccion

                delete fecha;

                delete consulta;

                GestorBaseDatos::cerrarDB();

                //Fin de la actualizacion con la BBBDD

                //reinicializo y asi actualizo la informacion
                this->inicializarConBaseDeDatos();

                return 1;
            }

        }
    }
    else{
        qDebug("El ID del Producto no se encontro en el vector de Productos");
        QMessageBox::critical(0,"Error" , "El ID del Producto no fue enconontrado");

        return -1;
    }

}
