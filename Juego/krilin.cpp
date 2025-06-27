#include "krilin.h"

Krilin::Krilin(QObject *parent)
    : QObject{parent}
{
    cuadroActual = 0;
    totalCuadros = 8;
    ancho = 32;
    alto = 40;
    krilinSeleccion = new QPixmap(":/imagenes/krilinSeleccion.png");
    setPixmap(krilinSeleccion->copy(0, 0, ancho, alto));
    setPos(750, 380);

    seleccion = new QTimer(this);
    connect(seleccion, &QTimer::timeout, this, &Krilin::animar);
}

void Krilin::iniciarAnimacion()
{
    cuadroActual = 0;
    seleccion->start(80);
}

void Krilin::detenerAnimacion()
{
    seleccion->stop();
}

void Krilin::animar()
{
    int x = cuadroActual * ancho;
    setPixmap(krilinSeleccion->copy(x, 0, ancho, alto));
    cuadroActual = (cuadroActual + 1) % totalCuadros;
}


