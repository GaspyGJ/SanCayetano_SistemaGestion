#include "Gestores/administrador.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include <QMessageBox>

Administrador::Administrador(){

   this->gestorClientes = new GestorClientes();

   this->gestorProductos  = new GestorProductos();

   this->gestorVentas  = new GestorVentas();
}

GestorClientes  *Administrador::getGestorClientes(){
    return this->gestorClientes;
}

GestorProductos  *Administrador::getGestorProductos(){
    return this->gestorProductos;
}

GestorVentas  *Administrador::getGestorVentas(){
    return this->gestorVentas;
}




