#ifndef GESTORVENTAS_H
#define GESTORVENTAS_H

#include "Modelos/venta.h"

class GestorVentas
{
public:

    GestorVentas();

    void inicializarTodayConBaseDeDatos(); // ejecuta consulta sql , crea las Ventas DE HOY  y los guarda en el vector

    void agregarVentaToVec(unsigned int ID,short cantidad_Producto,double precio_Producto,QString fecha_Venta,QString hora_Venta,QString nombre_Producto,QString nombre_Cliente);

    short crearVentaToday(short cantidad_Producto,double precio_Producto,QString fecha_Venta,QString hora_Venta,QString nombre_Producto,QString nombre_Cliente);

    short buscarVentaToday(unsigned int ID); // devuelve la posicion de vecVentas donde esta el de ID, si no esta retorna -1

    Venta* getVentaToday(unsigned int ID); //devuelve un puntero a la venta

    short eliminarVenta_Today(unsigned int ID);

    QVector <Venta *> getAll_Ventas_Today(); //retorna todos los Ids de las ventas del dia de hoy

private:

    QVector <Venta *> vecVentasToday; // registro de las ventas de HOY
};

#endif // GESTORVENTAS_H
