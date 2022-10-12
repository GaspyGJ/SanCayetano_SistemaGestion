#ifndef GESTORBASEDATOS_H
#define GESTORBASEDATOS_H


#include "qsqldatabase.h"
class GestorBaseDatos
{
public:

    GestorBaseDatos();

    static QSqlDatabase *getBBDD();

    static short ConectarDB();

    static short abrirDB();

    static void cerrarDB();

 private:

    static QSqlDatabase *db;
};

#endif // GESTORBASEDATOS_H
