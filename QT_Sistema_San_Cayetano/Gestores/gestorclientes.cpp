//@@ falta que la base de "desconecte" creo
#include "Gestores/gestorclientes.h"
#include "Gestores/gestorbasedatos.h"
#include "GUI/registroclientes.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include <QMessageBox>
#include <QVector>



GestorClientes::GestorClientes(){
    this->inicializarConBaseDeDatos();
}

GestorClientes::~GestorClientes(){
    //@@ esto es necesario ??? igual no estoy eliminando el vector del todo, nose
    for(int i=0; i<this->list_Clientes.length(); i++ ){
        delete this->list_Clientes[i];
    }

}

void GestorClientes::inicializarConBaseDeDatos(){

    this->list_Clientes.clear();

    GestorBaseDatos::ConectarDB();
    GestorBaseDatos::abrirDB();

    QSqlQuery *consulta= new QSqlQuery(*GestorBaseDatos::getBBDD());

    consulta->prepare("select C.nombre,D.direccion,T.telefono from (Clientes C left join DireccionC D on C.nombre=D.nombreC) left join TelefonosD T on D.direccion=T.direccion and D.nombreC=T.nombreC;");

    if( ! consulta->exec() ){
        qDebug()<<"El Gestor Cliente dice :\n Error al ejecutar la consulta SQL GESTOR CLIENTE";
        qDebug()<< GestorBaseDatos::getBBDD()->lastError();
    }

    while( consulta->next() ){ // mientras la consulta tenga datos sigo leyendo (fila por fila)

        QString nombre =consulta->value("nombre").toString();
        QString direccion =consulta->value("direccion").toString();
        QString telefono = consulta->value("telefono").toString();

        if( this->find_by_Nombre(nombre)==1){//ya existe el cliente


            if(this->getCliente(nombre)->getEstablecimiento(direccion)!=NULL){ // si existe establecimiento con esa direccion
                this->getCliente(nombre)->getEstablecimiento(direccion)->setTelefono(telefono);
                //qDebug()<<"El Gestor Cliente dice :\nSe agrego un telefono al Establecimiento de direccion:"<<direccion<<"\n del cliente : "<<nombre;
            }
            else{
                this->getCliente(nombre)->setEstablecimiento(direccion,telefono);
                //qDebug()<<"El Gestor Cliente dice :\nSe agrego un Establecimiento con direccion y telefono:"<<direccion+","+telefono<<"\n al cliente : "<<nombre;
            }
        }
        else{

            Establecimiento *establecimiento = new Establecimiento(direccion,telefono);

            agregarClienteToList(nombre,establecimiento);

            //qDebug()<<"El Gestor Cliente dice :\nSe agrego a la lista cliente :"<<nombre<<"\n con ID:"<<ID<<"\n Establecimiento con la direccion de : "<<direccion<<"\n con el telefono: "<<telefono<<Qt::endl;
        }
    }

    delete consulta;


    GestorBaseDatos::cerrarDB();

}

void GestorClientes::actualizarGestor(){

    this->list_Clientes.clear();

    this->inicializarConBaseDeDatos();

}

void GestorClientes::agregarClienteToList(QString nombre,Establecimiento *establecimiento){

    // el mismo cliente (nombre de cliente) puede tener varias direcciones, el programa los toma como clientes dirferentes  --> diferentes objetos

    Cliente *c= new Cliente(nombre,establecimiento);

    this->list_Clientes.push_back(c);
}


short GestorClientes::find_by_Nombre(QString nombre){

    for(int i=0; i<this->list_Clientes.length(); i++ ){

        if(this->list_Clientes[i]->getNombre()==nombre){
            return i;
        }
    }
    return -1;
}

Cliente* GestorClientes::getCliente(QString nombre){

    int pos = this->find_by_Nombre(nombre);
    if(pos!=-1){
        return this->list_Clientes[pos];
    }

    return nullptr;
}

QVector<Cliente *> GestorClientes::getAll_Clientes(){
    return this->list_Clientes;
}


short GestorClientes::crearNuevoCliente(QString nombre ,QString direccion , QString telefono){ // retorna 1 si se creo en la BBDD correctamente

    //PRIMERO LO AGREGAMOS A LA BBDD

    if( !  GestorBaseDatos::ConectarDB() ){
        qDebug()<<"El Gestor Cliente dice :\nUltimo Error de BBDD = "<< GestorBaseDatos::getBBDD()->lastError().text();
        QMessageBox::critical(0,"Error de Base de Datos","Error al conectarse con la base de datos");
        return -1;
    }
    if( !GestorBaseDatos::abrirDB() ){
        qDebug()<<"El Gestor Cliente dice :\nUltimo Error de BBDD = "<< GestorBaseDatos::getBBDD()->lastError().text();
        QMessageBox::critical(0,"Error de Base de Datos","Error al abrir la base de datos");
        return -1;
    }

    //si se pudo conectar y abrir correctamente creo las consultas

    QSqlQuery *consulta0= new QSqlQuery( *GestorBaseDatos::getBBDD()); // foreing keys prevencion
    QSqlQuery *consulta= new QSqlQuery( *GestorBaseDatos::getBBDD()); // para agregar cliente  a la bbdd
    QSqlQuery *consulta2= new QSqlQuery( *GestorBaseDatos::getBBDD()); // para agregar direccion  a la bbdd
    QSqlQuery *consulta3= new QSqlQuery( *GestorBaseDatos::getBBDD()); // para agregar telefono a la bbdd

    //@@ HACER LA CONSULTA CORRESPONDIENTE


    QString insertPragma = "PRAGMA foreign_keys = ON;"; // por las dudas ( SQLITE me daba problemas con las foreing keys)

    QString insertCliente = "INSERT INTO Clientes (nombre) VALUES ('"+nombre+"');";

    QString insertDireccionC = "INSERT INTO DireccionC (nombreC,direccion) VALUES ( '"+nombre+"','"+direccion+"');";

    QString insertTelefonoD = "INSERT INTO TelefonosD (nombreC,direccion,telefono) VALUES ( '"+nombre+"','"+direccion+"','"+telefono+"');";


    consulta0->prepare(insertPragma);
    consulta0->exec();

    GestorBaseDatos::getBBDD()->transaction(); // ABRO TRANSACCION

    consulta->prepare(insertCliente);
    consulta2->prepare(insertDireccionC);
    consulta3->prepare(insertTelefonoD);

    qDebug()<<"Se ejecuto : "+ insertCliente;
    qDebug()<<"Se ejecuto : "+ insertDireccionC;
    qDebug()<<"Se ejecuto : "+ insertTelefonoD;

    if( ! consulta->exec() ){
        qDebug()<<"El Gestor Cliente dice :\nUltimo Error de BBDD = "<< GestorBaseDatos::getBBDD()->lastError().text();
        QMessageBox::critical(0,"Error de Base de Datos","Error al ejecutar la consulta SQL Crear Nuevo Cliente");

        return -1;
    }

    else if( ! consulta2->exec() ){
        qDebug()<<"El Gestor Cliente dice :\nUltimo Error de BBDD = "<< GestorBaseDatos::getBBDD()->lastError();
        QMessageBox::critical(0,"Error de Base de Datos","Error al ejecutar la consulta SQL Crear Nuevo Cliente");

        return -1;
    }
    else if ( ! consulta3->exec()){
        qDebug()<<"El Gestor Cliente dice :\nUltimo Error de BBDD = "<< GestorBaseDatos::getBBDD()->lastError();
        QMessageBox::critical(0,"Error de Base de Datos","Error al ejecutar la consulta SQL Crear Nuevo Cliente");

        return -1;
    }
    else{

        GestorBaseDatos::getBBDD()->commit(); //CIERRO LA TRANSACCION

        delete consulta0;
        delete consulta;
        delete consulta2;
        delete consulta3;

        GestorBaseDatos::cerrarDB();

        //reinicializamos para agregar los cambios
        this->inicializarConBaseDeDatos();

        return 1;
    }

}

short GestorClientes::eliminarCliente(QString nombre){

    //PRIMERO VERIFICO SI EXISTE ESE CLIENTE

    if( this->find_by_Nombre(nombre) == -1 ){
        qDebug("El Gestor Cliente dice :\nEl ID del Cliente no se encontro en el vector de Clientes");
        QMessageBox::critical(0,"Error" , "El ID del Cliente no fue enconontrado /n Nombre cliente: "+nombre);

        return -1;
    }

    //AHORA LO ELIMINAMOS DE LA BBDD

    short retorno = -1;

    GestorBaseDatos::ConectarDB();

    GestorBaseDatos::abrirDB();

    QSqlQuery *consulta0= new QSqlQuery( *GestorBaseDatos::getBBDD());
    QSqlQuery *consulta= new QSqlQuery( *GestorBaseDatos::getBBDD());
    QSqlQuery *consulta2= new QSqlQuery( *GestorBaseDatos::getBBDD());
    QSqlQuery *consulta3= new QSqlQuery( *GestorBaseDatos::getBBDD());

    QString insertPragma = "PRAGMA foreign_keys = ON;";

    QString eliminarTelefono = "DELETE FROM TelefonosD Where nombreC ='"+nombre+"';";

    QString eliminarDireccion = "DELETE FROM DireccionC Where nombreC ='"+nombre+"';";

    QString eliminarCliente = "DELETE FROM Clientes Where nombre ='"+nombre+"';";

    consulta0->prepare(insertPragma);
    consulta0->exec();

    if ( QMessageBox::question(0, "Pregunta", "Desea eliminar relamente el cliente "+nombre+" de la base de datos?") == QMessageBox::Yes) {

        qDebug()<<"La consulta a ejecutar fue = "<<eliminarTelefono<<" \n"<<eliminarDireccion<<"\n"<<eliminarCliente;

        GestorBaseDatos::getBBDD()->transaction();

        consulta->prepare(eliminarTelefono);

        consulta2->prepare(eliminarDireccion);

        consulta3->prepare(eliminarCliente);


        if( ! consulta->exec() ){

            qDebug()<<"El Gestor Cliente dice :\nError al ejecutar la consulta SQL GestorCliente (elimnarCliente) --> Eliminar TELEFONO";
            qDebug()<<"El Gestor Cliente dice :\nUltimo Error de BBDD = "<< GestorBaseDatos::getBBDD()->lastError();
            QMessageBox::critical(0,"Error de Base de Datos","Error al ejecutar la consulta SQL Eliminar TELEFONO");

            retorno = -1;
        }

        else if( ! consulta2->exec() ){

            qDebug()<<"El Gestor Cliente dice :\nError al ejecutar la consulta SQL GestorCliente (crearNuevoCliente) --> Eliminar DIRECCION";
            qDebug()<<"El Gestor Cliente dice :\nUltimo Error de BBDD = "<< GestorBaseDatos::getBBDD()->lastError();
            QMessageBox::critical(0,"Error de Base de Datos","Error al ejecutar la consulta SQL Eliminar DIRECCION");

            retorno = -1;
        }
        else if( ! consulta3->exec() ){

            qDebug()<<"El Gestor Cliente dice :\nError al ejecutar la consulta SQL GestorCliente (crearNuevoCliente) --> Eliminar CLIENTE";
            qDebug()<<"El Gestor Cliente dice :\nUltimo Error de BBDD = "<< GestorBaseDatos::getBBDD()->lastError();
            QMessageBox::critical(0,"Error de Base de Datos","Error al ejecutar la consulta SQL Eliminar CLIENTE");

            retorno = -1;
        }
        else{

            GestorBaseDatos::getBBDD()->commit();

            //reinicializamos la lista
            this->inicializarConBaseDeDatos();

            retorno = 1;
        }
    }

    delete consulta0;
    delete consulta;
    delete consulta2;
    delete consulta3;

    GestorBaseDatos::cerrarDB();


    return retorno;
}

short GestorClientes::actualizarCliente(short columna,QString nombre,QString datoViejo,QString datoActualizado,QString direccion){

    short retorno;

    switch(columna){
    case RegistroClientes::NOMBRE : retorno  = modificarNombre(nombre,datoViejo,datoActualizado); break;
    case RegistroClientes::DIRECCION : retorno  = modificarDireccion(nombre,datoViejo,datoActualizado); break;
    case RegistroClientes::TELEFONO : retorno  = modificarTelefono(nombre,direccion,datoViejo,datoActualizado); break;
    default : retorno = -2;
    }

    return retorno;
}

short GestorClientes::modificarTelefono(QString nombre,QString direccion ,QString telefonoViejo, QString telefonoNuevo){

    short retorno=-1;

    if ( this->find_by_Nombre(nombre) == -1){
        qDebug("El Gestor Cliente dice :\nEl ID del Cliente no se encontro en el vector de Clientes 2");
        QMessageBox::critical(0,"Error" , "El ID del Cliente no fue enconontrado");

        return-1;
    }

    // Actualizo el telefono en la BBDD

    GestorBaseDatos::ConectarDB();

    GestorBaseDatos::abrirDB();

    QSqlQuery *consulta= new QSqlQuery( *GestorBaseDatos::getBBDD());

    QString query;

    if(telefonoViejo=="Sin Telefono"){

        if(direccion==""){
            //qDebug()<<"Se ejecuto LA DE SIN TELEFONO SIN DIRECCION";
            query = "update TelefonosD set telefono= '"+telefonoNuevo+"' where (telefono='' or telefono is Null) and ( direccion='' or direccion is Null ) and nombreC='"+nombre+"';";
        }
        else{
            //qDebug()<<"Se ejecuto LA DE SIN TELEFONO CON DIRECCION";
            query = "update TelefonosD set telefono= '"+telefonoNuevo+"' where (telefono='' or telefono is Null) and direccion='"+direccion+"' and nombreC='"+nombre+"';";
        }

    }
    else{

        if(direccion==""){
            //qDebug()<<"Se ejecuto LA DE CON TELEFONO SIN DIRECCION";
            query = "update TelefonosD set telefono= '"+telefonoNuevo+"' where telefono='"+telefonoViejo+"' and ( direccion='' or direccion is Null ) and nombreC='"+nombre+"';";
        }
        else{
            //qDebug()<<"Se ejecuto LA DE CON TELEFONO CON DIRECCION";
            query = "update TelefonosD set telefono= '"+telefonoNuevo+"' where telefono='"+telefonoViejo+"' and direccion='"+direccion+"' and nombreC='"+nombre+"';";
        }
    }
    consulta->prepare(query);

    GestorBaseDatos::getBBDD()->transaction(); //empiezo la transaccion

    qDebug()<<"consulta ejecutada = "<<query;
    if( ! consulta->exec() ){

        qDebug()<<"El Gestor Cliente dice :\nError al ejecutar la consulta SQL modificar Telefono";
        qDebug()<<"El Gestor Cliente dice :\nUltimo Error de BBDD = "<< GestorBaseDatos::getBBDD()->lastError();
        QMessageBox::critical(0,"Error de Base de Datos","Error al ejecutar la consulta SQL Update Telefono Cliente");

        delete consulta;

        GestorBaseDatos::cerrarDB();

        retorno = -1;
    }
    else{

        GestorBaseDatos::getBBDD()->commit(); // confirmo la transaccion

        //Fin de la actualizacion con la BBBDD
        delete consulta;

        GestorBaseDatos::cerrarDB();

        //reinicializo para actualizar los datos
        this->inicializarConBaseDeDatos();

        QMessageBox::information(0,"OK" , "Se actualizo correctamente el telefono del cliente");

        retorno = 1;
    }

    return retorno;
}

short GestorClientes::modificarDireccion(QString nombre,QString direccionVieja,QString direccionNueva){

    short retorno;

    if ( this->find_by_Nombre(nombre) == -1){
        qDebug("El Gestor Cliente dice :\n El ID del Cliente no se encontro en el vector de Clientes");
        QMessageBox::critical(0,"Error" , "El ID del Cliente no fue enconontrado");

        return -1;
    }

    // Actualizo el telefono en la BBDD


    GestorBaseDatos::ConectarDB();

    GestorBaseDatos::abrirDB();

    QSqlQuery *consulta= new QSqlQuery( *GestorBaseDatos::getBBDD());
    QSqlQuery *consulta2= new QSqlQuery( *GestorBaseDatos::getBBDD());

    QString query1,query2;


    if(direccionVieja=="Sin Direccion"){
        //qDebug()<<"Se ejecuto LA DE SIN DIRECCION";
        query1 = "update DireccionC set direccion= '"+direccionNueva+"' where (direccion='' or direccion is Null) and nombreC='"+nombre+"';";

        query2 = "update TelefonosD set direccion= '"+direccionNueva+"' where (direccion='' or direccion is Null) and nombreC='"+nombre+"';";
    }
    else{
        //qDebug()<<"Se ejecuto LA DE CON DIRECCION";
        query1 = "update DireccionC set direccion= '"+direccionNueva+"' where direccion='"+direccionVieja+"' and nombreC='"+nombre+"';";

        query2 = "update TelefonosD set direccion= '"+direccionNueva+"' where  direccion='"+direccionVieja+"' and nombreC='"+nombre+"';";
    }

    consulta->prepare(query1);
    consulta2->prepare(query2);


    qDebug()<<"consulta ejecutada ="<<query1;
    GestorBaseDatos::getBBDD()->transaction(); //empiezo la transaccion

    if( ! consulta->exec() ){

        qDebug()<<"El Gestor Cliente dice :\nError al ejecutar la consulta SQL modificar direccion cambiar direccion";
        qDebug()<<"El Gestor Cliente dice :\nUltimo Error de BBDD = "<< GestorBaseDatos::getBBDD()->lastError();
        QMessageBox::critical(0,"Error de Base de Datos","Error al ejecutar la consulta SQL Update Direccion Cliente");

        retorno = -1;

    }
    else if(! consulta2->exec()){

        qDebug()<<"El Gestor Cliente dice :\nError al ejecutar la consulta SQL modificar direccion cambiar en telefonos";
        qDebug()<<"El Gestor Cliente dice :\nUltimo Error de BBDD = "<< GestorBaseDatos::getBBDD()->lastError();
        QMessageBox::critical(0,"Error de Base de Datos","Error al ejecutar la consulta SQL Update Direccion Cliente");

        delete consulta;
        delete consulta2;
        GestorBaseDatos::cerrarDB();

        retorno = -1;
    }
    else{
        //Cambio el valor en el vector actual de clientes

        if(direccionVieja=="Sin Direccion"){
            direccionVieja="";
        }

        GestorBaseDatos::getBBDD()->commit(); // confirmo la transaccion

        delete consulta;
        delete consulta2;
        GestorBaseDatos::cerrarDB();

        //reinicializo para actualizar los datos
        this->inicializarConBaseDeDatos();

        //Fin de la actualizacion con la BBBDD

        QMessageBox::information(0,"OK" , "Se actualizo correctamente la direccion del cliente");

        retorno = 1;
    }


    return retorno;

}

//@@DEBERIA MODIFICAR LOS TELEFONOS Y DIRECCINES ASOCIADOS ANTES
short GestorClientes::modificarNombre(QString nombre,QString nombreViejo,QString nombreNuevo){

    short retorno;

    if(this->find_by_Nombre(nombre) == -1){
        qDebug("El Gestor Cliente dice :\nEl ID del Cliente no se encontro en el vector de Clientes");
        QMessageBox::critical(0,"Error" , "El ID del Cliente no fue enconontrado");

        return -1;
    }

    // Actualizo el telefono en la BBDD

    GestorBaseDatos::ConectarDB();

    GestorBaseDatos::abrirDB();

    QSqlQuery *consulta= new QSqlQuery( *GestorBaseDatos::getBBDD());

    QString query = "update Clientes set nombre= '"+nombreNuevo+"' where nombre='"+nombreViejo+"';";
    consulta->prepare(query);

    GestorBaseDatos::getBBDD()->transaction(); //empiezo la transaccion

    if( ! consulta->exec() ){

        qDebug()<<"El Gestor Cliente dice :\nError al ejecutar la consulta SQL modificar nombre";
        qDebug()<<"El Gestor Cliente dice :\nUltimo Error de BBDD = "<< GestorBaseDatos::getBBDD()->lastError();

        QMessageBox::critical(0,"Error de Base de Datos","Error al ejecutar la consulta SQL Update Nombre Cliente");

        delete consulta;

        GestorBaseDatos::cerrarDB();

        retorno = -1;

    }
    else{

        GestorBaseDatos::getBBDD()->commit(); // confirmo la transaccion
        //Fin de la actualizacion con la BBBDD

        delete consulta;

        GestorBaseDatos::cerrarDB();

        this->inicializarConBaseDeDatos();

        QMessageBox::information(0,"OK" , "Se actualizo correctamente el nombre del cliente");

        retorno = 1;
    }

    return retorno;

}

short GestorClientes::agregarDireccionToCliente(QString nombreClienteSeleccionado,QString valorDeDireccion){

    short retorno;

    short posCliente = this->find_by_Nombre(nombreClienteSeleccionado);

    if ( posCliente != -1){

        if(valorDeDireccion==""){

            qDebug()<<"El Gestor Cliente dice :\nNo se puede agregar direccion ,el campo de direccion esta vacio";

            QMessageBox::critical(0,"Error de Base de Datos","Agrege un valor en el campo de direccion");

            return -1;

        }
        else{

            //@@ NO VERIFICO QUE LA DIRECCION QUE INGRESE NO ESTE REPETIDA CON ALGUNA YA EXISTENTE. DEBERIA HACERLO

            // Actualizo el telefono en la BBDD

            GestorBaseDatos::ConectarDB();

            GestorBaseDatos::abrirDB();

            QSqlQuery *consulta= new QSqlQuery( *GestorBaseDatos::getBBDD());
            //this->getAll_Clientes().contains(posCliente)->getID()
            QString query = "insert into DireccionC(nombreC,direccion) values('"+nombreClienteSeleccionado+"','"+valorDeDireccion+"');";
            QString query2 = "insert into TelefonosD(nombreC,direccion,telefono) values('"+nombreClienteSeleccionado+"','"+valorDeDireccion+"','');";


            consulta->prepare(query);

            GestorBaseDatos::getBBDD()->transaction(); //empiezo la transaccion
            qDebug()<<"consulta1 ejecutada = "<<query;

            if( ! consulta->exec() ){

                qDebug()<<"El Gestor Cliente dice :\nError al ejecutar la consulta SQL agregar Direccion";
                qDebug()<<"El Gestor Cliente dice :\nUltimo Error de BBDD = "<< consulta->lastError();
                QMessageBox::critical(0,"Error de Base de Datos","Error al ejecutar la consulta SQL Agregar Direccion a Cliente");

                delete consulta;

                GestorBaseDatos::cerrarDB();

                retorno = -1;

            }
            else{

                consulta->prepare(query2);
                qDebug()<<"consulta2 ejecutada = "<<query2;

                if( ! consulta->exec() ){

                    qDebug()<<"El Gestor Cliente dice :\nError al ejecutar la consulta SQL agregar Direccion 2";
                    qDebug()<<"El Gestor Cliente dice :\nUltimo Error de BBDD = "<< consulta->lastError();
                    QMessageBox::critical(0,"Error de Base de Datos","Error al ejecutar la consulta SQL Agregar Direccion a Cliente");

                    delete consulta;

                    GestorBaseDatos::cerrarDB();

                    retorno = -1;

                }
                else{
                    GestorBaseDatos::getBBDD()->commit(); // confirmo la transaccion

                    delete consulta;

                    GestorBaseDatos::cerrarDB();

                    //Fin de la actualizacion con la BBBDD

                    this->inicializarConBaseDeDatos();

                    QMessageBox::information(0,"OK" , "Se agrego correctamente la direccion del cliente");

                    qDebug()<<"PASO ACA";

                    retorno = 1;
                }

            }

        }
    }
    else{
        qDebug()<<"El Gestor Cliente dice :\nError no se encontro el cliente para agregar la direccion";

        retorno = -1 ;
    }

    return retorno;
}

short GestorClientes::eliminarDireccionToCliente(QString nombreClienteSeleccionado,QString valorDeDireccion){

    short retorno=-1;

    short posCliente = this->find_by_Nombre(nombreClienteSeleccionado);

    if ( posCliente != -1){
        // Actualizo el telefono en la BBDD


        GestorBaseDatos::ConectarDB();

        GestorBaseDatos::abrirDB();

        QSqlQuery *consulta= new QSqlQuery( *GestorBaseDatos::getBBDD());
        QSqlQuery *consulta2= new QSqlQuery( *GestorBaseDatos::getBBDD());

        QString eliminarDireccion = "DELETE FROM DireccionC Where direccion='"+valorDeDireccion+"' and nombreC ='"+nombreClienteSeleccionado+"';";

        QString eliminarTelefonoAsociadoADireccion = "DELETE FROM TelefonosD Where direccion='"+valorDeDireccion+"' and and nombreC ='"+nombreClienteSeleccionado+"';";

        consulta->prepare(eliminarTelefonoAsociadoADireccion);

        consulta2->prepare(eliminarDireccion);

        GestorBaseDatos::getBBDD()->transaction(); //empiezo la transaccion

        if( ! consulta->exec() ){

            qDebug()<<"El Gestor Cliente dice :\nError al ejecutar la consulta SQL eliminar telefono asociado a eliminar direccion";
            qDebug()<<"El Gestor Cliente dice :\nUltimo Error de BBDD = "<< GestorBaseDatos::getBBDD()->lastError();

            QMessageBox::critical(0,"Error de Base de Datos","Error al ejecutar la consulta SQL Eliminar telefono asociado a direccion");

            retorno = -1;
        }
        else if( ! consulta2->exec() ){

            qDebug()<<"El Gestor Cliente dice :\nError al ejecutar la consulta SQL eliminar direccion";
            qDebug()<<"El Gestor Cliente dice :\nUltimo Error de BBDD = "<< GestorBaseDatos::getBBDD()->lastError();

            QMessageBox::critical(0,"Error de Base de Datos","Error al ejecutar la consulta SQL Eliminar direccion");

            delete consulta;
            delete consulta2;

            GestorBaseDatos::cerrarDB();

            retorno = -1;
        }
        else{

            GestorBaseDatos::getBBDD()->commit(); // confirmo la transaccion

            delete consulta;
            delete consulta2;

            GestorBaseDatos::cerrarDB();

            //Fin de la actualizacion con la BBBDD

            this->inicializarConBaseDeDatos();

            QMessageBox::information(0,"OK" , "Se elimino correctamente el nombre la direccion");

            retorno = 1;
        }

        delete consulta;
        delete consulta2;

        GestorBaseDatos::cerrarDB();


    }
    else{
        qDebug("El Gestor Cliente dice :\nEl ID del Cliente no se encontro en el vector de Clientes 2");
        QMessageBox::critical(0,"Error" , "El ID del Cliente no fue enconontrado");

        retorno = -1;
    }

    return retorno;


}

