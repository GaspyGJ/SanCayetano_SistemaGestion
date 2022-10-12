#ifndef ADMINISTRADOR_H
#define ADMINISTRADOR_H

#include "gestorclientes.h"
#include "gestorproductos.h"
#include "gestorventas.h"

class Administrador
{

public:
    Administrador();


    GestorClientes  *getGestorClientes();

    GestorProductos  *getGestorProductos();

    GestorVentas  *getGestorVentas();


private:

    GestorClientes  *gestorClientes;

    GestorProductos  *gestorProductos;

    GestorVentas  *gestorVentas;

};

#endif // ADMINISTRADOR_H
