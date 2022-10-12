#include "venta.h"



Venta::Venta(unsigned int ID,QString nombreP,QString nombreC,double precio_producto,short cantidad_producto,QString fecha_venta,QString hora_venta){
    this->setID(ID);
    this->setCliente(nombreC);
    this->setProducto(nombreP);
    this->setPrecio_prodcuto(precio_producto);
    this->setCantidad(cantidad_producto);
    this->setPrecio_total(this->precio_producto * this->cantidad_producto);
    this->setFecha_venta(fecha_venta);
    this->setHora_venta(hora_venta);
}

//Getters
unsigned int Venta::getID(){
    return this->ID;
}
QString Venta::getCliente(){
    return this->nombreC;
}
QString Venta::getProducto(){
    return this->nombreP;
}
double Venta::getPrecio_producto(){
    return this->precio_producto;
}
short Venta::getCantidad(){
    return this->cantidad_producto;
}
double Venta::getPrecio_total(){
    return this->precio_total;
}
QString Venta::getFecha_venta(){
    return this->fecha_venta;
}
QString Venta::getHora_venta(){
    return this->hora_venta;
}


//Setters
void Venta::setID(unsigned int ID){
    this->ID=ID;
}
void Venta::setCliente(QString cliente){
    this->nombreC=cliente;
}
void Venta::setProducto(QString producto){
    this->nombreP=producto;
}
void  Venta::setPrecio_prodcuto(double precio_producto){
    this->precio_producto=precio_producto;
}
void Venta::setCantidad(short cantidad){
    this->cantidad_producto=cantidad;
}
void Venta::setPrecio_total(double precio_total){
    this->precio_total=precio_total;
}
void  Venta::setFecha_venta(QString fecha_venta){
    this->fecha_venta=fecha_venta;
}
void  Venta::setHora_venta(QString hora_venta){
    this->hora_venta=hora_venta;
}
