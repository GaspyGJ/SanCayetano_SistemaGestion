#include "cliente.h"

Cliente::Cliente(QString nombre,Establecimiento *establecimiento){
    this->setNombre(nombre);
    this->vecEstablecimientos.push_back(establecimiento);
}

//setters
void Cliente::setID(unsigned int id){
    this->ID=id;
}
void Cliente::setNombre(QString nombre){
    this->nombre=nombre;
}
void Cliente::setEstablecimiento(QString direccion,QString telefono){
    Establecimiento *e = new Establecimiento(direccion,telefono);
    this->vecEstablecimientos.push_back(e);
}

//getters
QString Cliente::getNombre(){
    return this->nombre;
}

Establecimiento* Cliente::getEstablecimiento(QString direccion){
    foreach(Establecimiento *e , this->getAll_Establecimientos() ){
        if(e->getDireccion()==direccion){
            return e;
        }
    }
    return NULL;
}

QVector <Establecimiento *> Cliente::getAll_Establecimientos(){
    return this->vecEstablecimientos;
}


