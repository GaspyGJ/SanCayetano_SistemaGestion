#ifndef VENTA_H
#define VENTA_H


#include "cliente.h"
#include "producto.h"

class Venta
{
public:

    Venta(unsigned int ID,QString Producto,QString Cliente,double precio_Producto,short cantidad_Producto,QString fecha_Venta,QString hora_Venta);

    //getters
    unsigned int getID();
    QString  getCliente();
    QString  getProducto();
    short getCantidad();
    double getPrecio_producto();
    double getPrecio_total();
    QString getFecha_venta();
    QString getHora_venta();

    //setters
    void setID(unsigned int ID);
    void setCliente(QString nombreC);
    void setProducto(QString nombreP);
    void setCantidad(short cantidad);
    void setPrecio_prodcuto(double precio_producto);
    void setPrecio_total(double precio_total);
    void setFecha_venta(QString fecha_venta);
    void setHora_venta(QString hora_venta);



private:
    unsigned int ID;

    QString nombreC,nombreP;

    short cantidad_producto;

    double precio_producto,precio_total;

    QString fecha_venta,hora_venta;

};

#endif // VENTA_H
