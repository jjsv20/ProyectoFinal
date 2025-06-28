#include "goku.h"

Goku::Goku(QObject *parent)
    : QObject{parent}
{
    cuadroActual = 0;
    totalCuadros = 6;
    ancho = 36;
    alto = 46;
    gokuSeleccion = new QPixmap(":/imagenes/Goku.png");
    QBitmap mask = gokuSeleccion->createMaskFromColor(QColor(132, 0, 132), Qt::MaskInColor);
    gokuSeleccion->setMask(mask);
    setPixmap(gokuSeleccion->copy(0, 0, ancho, alto));
    setPos(150, 370);
    seleccion = new QTimer(this);
    connect(seleccion, &QTimer::timeout, this, &Goku::animar);
}

void Goku::iniciarAnimacion()
{
    cuadroActual = 0;
    seleccion->start(150);
}

void Goku::detenerAnimacion()
{
    seleccion->stop();
}

void Goku::animar()
{
    int x = cuadroActual * ancho;
    setPixmap(gokuSeleccion->copy(x, 0, ancho, alto));
    cuadroActual = (cuadroActual + 1) % totalCuadros;
}

