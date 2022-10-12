#include "producto.h"

Producto::Producto(QString nombre,double precio,double precio_mayorista, QString cantidad_stock,QString ultima_actualizacion){
    this->setNombre(nombre);
    this->setPrecio(precio);
    this->setPrecio_mayorista(precio_mayorista);
    this->setCantidad_stock(cantidad_stock);
    this->setUltima_actualizacion(ultima_actualizacion);
}

//Getters
QString Producto::getNombre(){
    return this->nombre;
}

double Producto::getPrecio(){
    return this->precio;
}
double Producto::getPrecio_mayorista(){
    return this->precio_mayorista;
}

QString Producto::getCantidad_stock(){
    return this->cantidad_stock;
}

QString Producto::getUltima_actualizacion(){
    return this->ultima_actualizacion;
}


//Setters
void Producto::setNombre(QString nombre){
    this->nombre=nombre;
}

void Producto::setPrecio(double precio){
    this->precio=precio;
}

void Producto::setPrecio_mayorista(double precio){
    this->precio_mayorista=precio;
}

void Producto::setCantidad_stock(QString cantidad){
    this->cantidad_stock=cantidad;
}

void Producto::setUltima_actualizacion(QString fecha){
    this->ultima_actualizacion=fecha;
}
