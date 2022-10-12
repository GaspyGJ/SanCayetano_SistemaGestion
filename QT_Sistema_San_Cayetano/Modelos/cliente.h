#ifndef CLIENTE_H
#define CLIENTE_H

#include <QString>
#include <QVector>

#include "Modelos/establecimiento.h"

class Cliente
{
public:
    Cliente(QString nombre,Establecimiento *establecimiento);

    //getters
    QString getNombre();
    Establecimiento* getEstablecimiento(QString direccion);

    QVector <Establecimiento *> getAll_Establecimientos();

    //setters
    void setID(unsigned int id);
    void setNombre(QString nombre);
    void setEstablecimiento(QString direccion,QString telefono);

private:
    unsigned int  ID;
    QString nombre;
    QVector <Establecimiento *> vecEstablecimientos;
};

#endif // CLIENTE_H
