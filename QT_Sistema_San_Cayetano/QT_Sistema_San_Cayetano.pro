QT       += core gui sql printsupport webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Extra/containerexpandible.cpp \
    GUI/registroventas.cpp \
    Gestores/administrador.cpp \
    Gestores/gestorbasedatos.cpp \
    Modelos/cliente.cpp \
    Modelos/establecimiento.cpp \
    Gestores/gestorclientes.cpp \
    Gestores/gestorproductos.cpp \
    Gestores/gestorventas.cpp \
    Main/main.cpp \
    GUI/mainwindow.cpp \
    Modelos/producto.cpp \
    GUI/registroclientes.cpp \
    GUI/registrostock.cpp \
    Extra/styleitem.cpp \
    Extra/tableprinter.cpp \
    Modelos/venta.cpp \
    probando.cpp

HEADERS += \
    Extra/containerexpandible.h \
    GUI/registroventas.h \
    Gestores/administrador.h \
    Gestores/gestorbasedatos.h \
    Modelos/cliente.h \
    Modelos/establecimiento.h \
    Gestores/gestorclientes.h \
    Gestores/gestorproductos.h \
    Gestores/gestorventas.h \
    GUI/mainwindow.h \
    Modelos/producto.h \
    GUI/registroclientes.h \
    GUI/registrostock.h \
    Extra/styleitem.h \
    Extra/tableprinter.h \
    Modelos/venta.h \
    probando.h

FORMS += \
    GUI/mainwindow.ui \
    GUI/registroclientes.ui \
    GUI/registrostock.ui \
    GUI/registroventas.ui \
    probando.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
