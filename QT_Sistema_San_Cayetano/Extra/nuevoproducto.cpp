#include "nuevoproducto.h"
#include "ui_nuevoproducto.h"

NuevoProducto::NuevoProducto(Administrador *administrador,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NuevoProducto)
{
    ui->setupUi(this);

    this->administrador=administrador;

}

NuevoProducto::~NuevoProducto(){
    delete ui;
}

void NuevoProducto::on_btn_cancelar_clicked(){
    this->~NuevoProducto();
}


void NuevoProducto::on_btn_Aceptar_clicked(){

    //REALIZAR MEJOR LAS VERIFICACIONES
    if(ui->input_Nombre->text()!="" and ui->input_Precio->text()!="" ){


        //creo el producto mediante el gestor que lo crea y guarda en su lista de productos.
       // this->administrador->getGestorProductos()->agregarProducto(ui->input_Nombre->text(),
        //                                                           ui->input_Precio->text().toDouble(),1,(new QDate)->currentDate().toString());


        qDebug()<<"Se creo el Producto :\n Nombre = "<<ui->input_Nombre->text()<<Qt::endl;
        qDebug()<<"Precio = "<<ui->input_Precio->text()<<Qt::endl;

        emit ProductoCreadoCorrectamente();

        this->~NuevoProducto();

    }

}

