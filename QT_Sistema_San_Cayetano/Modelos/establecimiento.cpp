#include "establecimiento.h"
#include "qdebug.h"

Establecimiento::Establecimiento(QString direccion ,QString telefono){
    setDireccion(direccion);
    this->list_telefonos.push_back(telefono);
}

QString Establecimiento::getDireccion(){
        return this->direccion;
}
QStringList Establecimiento::get_All_Telefonos(){
       return this->list_telefonos;
}
void Establecimiento::setDireccion(QString direccion){
     this->direccion=direccion;
}
void Establecimiento::setTelefono(QString telefono){
     this->list_telefonos.push_back(telefono);
}

/*

void Establecimiento::actualizarTelefono(QString telefonoViejo, QString telefonoNuevo){
    if(telefonoViejo==SIN_TEL){ // SIN_TEL = "Sin Telefono"
            this->list_telefonos.push_back(telefonoNuevo);
    }
    else{
           this->list_telefonos[this->vecTelefonos.indexOf(telefonoViejo)]= telefonoNuevo;
    }
}
*/
