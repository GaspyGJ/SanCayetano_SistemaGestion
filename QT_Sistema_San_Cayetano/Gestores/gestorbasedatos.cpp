#include "gestorbasedatos.h"
#include "qdebug.h"
#include "qsqlerror.h"
#include <QMessageBox>


QSqlDatabase *GestorBaseDatos::db = new QSqlDatabase();

GestorBaseDatos::GestorBaseDatos()
{
    try {
        ConectarDB();
    } catch (...) {
        qDebug()<<"No se pudo conectar a la base de datos";
        QMessageBox::critical(0,"Error de base de datos","no se puedo conectar la base de datos");
    }
}

QSqlDatabase *GestorBaseDatos::getBBDD(){
    return GestorBaseDatos::db;
}


short GestorBaseDatos::ConectarDB(){

    short retorno=1;

    if( ! QSqlDatabase::isDriverAvailable("QSQLITE") ){

        qDebug()<<"Error : QSQLITE drivers no disponibles";

        retorno = -1;
    }

    else { // no hay problemas de drivers

        *db=QSqlDatabase::addDatabase("QSQLITE");

        db->setDatabaseName("SanCayetano.db");
    }

    return retorno;
}

short GestorBaseDatos::abrirDB(){

    short retorno=1;

    if( ! db->open() ){// si no se pudo abrir

        qDebug()<< db->lastError();

        qDebug()<<"Error : QSQLITE no se pudo abrir";

        retorno = -1;
    }


    return retorno;
}

void GestorBaseDatos::cerrarDB(){

    db->close();
    db->removeDatabase("QSQLITE");
}
