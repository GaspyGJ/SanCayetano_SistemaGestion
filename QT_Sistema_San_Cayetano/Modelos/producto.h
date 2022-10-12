#ifndef PRODUCTO_H
#define PRODUCTO_H

#include<QString>
#include<QDate>

class Producto
{
public:
    Producto(QString nombre,double precio,double precio_mayorista,QString cantidad_stock,QString ultima_actualizacion);

    //getters
    QString getNombre(); 
    double getPrecio();
    double getPrecio_mayorista();
    QString getCantidad_stock();
    QString getUltima_actualizacion();

    //setters
    void setNombre(QString nombre);
    void setPrecio(double precio);
    void setPrecio_mayorista(double precio);
    void setCantidad_stock(QString cantidad);
    void setUltima_actualizacion(QString fecha);

private:
    QString nombre;
    double precio;
    double precio_mayorista;
    QString cantidad_stock;
    QString ultima_actualizacion; // registra la fecha de la ultima vez que se actualizo el Stock
};

#endif // PRODUCTO_H
