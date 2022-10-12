#ifndef GESTORCLIENTES_H
#define GESTORCLIENTES_H

#include "Modelos/cliente.h"
#include <QVector>

class GestorClientes{

public:

    GestorClientes();
    ~GestorClientes();

    //getters

    Cliente *getCliente(QString nombre);

    QVector<Cliente *> getAll_Clientes();

    //--> si retorna short es que retorna -1 si hubo error o 1 si se ejecuto correctamento <--//

    //buscador
    short find_by_Nombre(QString nombre); // retorna la posicion del cliente en el vector , si no lo encuentra retorna -1

    //BBDD
    void actualizarGestor(); // limpia el vector de clientes y lo vuelve a cargar

    void inicializarConBaseDeDatos(); // ejecuta consulta sql , crea los Clientes y los guarda en el vector de clientes

    //Creacion/Eliminacion de clientes
    short crearNuevoCliente(QString nombre ,QString direccion , QString telefono);

    void agregarClienteToList(QString nombre,Establecimiento *establecimiento);

    short eliminarCliente(QString nombre );

    void eliminarClienteToVec(QString nombre );

    // modificacion de clientes
    short actualizarCliente(short columna,QString nombre,QString datoViejo,QString datoActualizado,QString direccion=""); //direccion valor por defecto = "" ,(si no se le pasa por parametro otra toma ese)

    short modificarTelefono(QString nombre,QString direccion ,QString telefonoViejo, QString telefonoNuevo);

    short modificarNombre(QString nombre ,QString nombreViejo,QString nombreNuevo);

    short modificarDireccion(QString nombre,  QString direccionVieja , QString direccionNueva);

    //
    short agregarDireccionToCliente(QString nombreClienteSeleccionado, QString valorDeDireccion);

    short eliminarDireccionToCliente(QString nombreClienteSeleccionado, QString valorDeDireccion);

private:
    QVector<Cliente *> list_Clientes;
};

#endif // GESTORCLIENTES_H
