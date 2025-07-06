QT       += core gui widgets multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    combate.cpp \
    entrenamiento.cpp \
    goku.cpp \
    krilin.cpp \
    main.cpp \
    objetos.cpp \
    personaje.cpp \
    roshi.cpp \
    seleccion.cpp \
    widget.cpp

HEADERS += \
    combate.h \
    entrenamiento.h \
    goku.h \
    krilin.h \
    objetos.h \
    personaje.h \
    roshi.h \
    seleccion.h \
    widget.h

FORMS += \
    combate.ui \
    entrenamiento.ui \
    seleccion.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    imagenes.qrc \
    musica.qrc

DISTFILES +=
