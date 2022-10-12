#ifndef GESTORPRODUCTOS_H
#define GESTORPRODUCTOS_H

#include "Modelos/producto.h"
#include <QVector>

class GestorProductos
{
public:

    GestorProductos();

    ~GestorProductos();

    //getters

    Producto* getProducto(QString nombre);

    QVector<Producto *> getAll_Productos();

    //finders

    short find_by_Nombre(QString nombre); // devuelve la posicion de vecProductos donde esta, si no esta retorna -1


    //Creacion / Eliminacion de Productos
    void agregarProducto(QString nombre,double precio,double precio_mayorista,QString cantidad_stock,QString ultima_actualizacion);


    //modificacion Productos

    short modificarCantidad(QString nombre,QString cantidad);

    short modificarPrecio(QString nombre,QString precio);

    short modificarPrecioMayorista(QString nombre,QString precio);

    void modificarUltima_actualizacion(QString nombre,QString ultima_actualizacion);



    //BBDD
    void inicializarConBaseDeDatos(); // ejecuta consulta sql , crea los Productos y los guarda en el vector de productos


private:

    QVector <Producto *> list_Productos;
};

#endif // GESTORPRODUCTOS_H
