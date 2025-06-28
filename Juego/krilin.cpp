#include "krilin.h"

Krilin::Krilin(QObject *parent)
    : QObject{parent}
{
    coordenadaX = 0;
    coordenadaY = 0;
    ancho = 66;
    alto = 100;
    pixmap = new QPixmap(":/imagenes/movimientosKrilin.png");
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
    setPos(750, 380);
    frameActual = 0;
    totalFramesDerecha = 8;

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    seleccion = new QTimer(this);
    connect(seleccion, &QTimer::timeout, this, &Krilin::actualizar);

    caminar = new QTimer(this);
    connect(caminar, &QTimer::timeout, this, &Krilin::animarDerecha);

    moviendoDerecha = false;
}

QRectF Krilin::boundingRect() const {
    return QRectF(0, 0, ancho, alto);
}

void Krilin::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->drawPixmap(0, 0, *pixmap, coordenadaX, coordenadaY, ancho, alto);
}

void Krilin::actualizar()
{
    coordenadaX += ancho;
    if(coordenadaX >= 200){
        coordenadaX = 0;
    }
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
}

void Krilin::volverASeleccion()
{
    coordenadaX = 0;
    coordenadaY = 0;
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
}

void Krilin::animarDerecha()
{
    coordenadaY = 100;

    frameActual = (frameActual + 1) % totalFramesDerecha;
    coordenadaX = frameActual * ancho;

    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));

    setPos(x() + 10, y());
}

void Krilin::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right && !event->isAutoRepeat()) {
        if (!moviendoDerecha) {
            moviendoDerecha = true;
            coordenadaY = 100;
            caminar->start(25);
        }
    }
}

void Krilin::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right && !event->isAutoRepeat()) {
        caminar->stop();
        moviendoDerecha = false;
        frameActual = 0;
        coordenadaX = 0;
        coordenadaY = 0;
        setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
    }
}

void Krilin::iniciarAnimacion()
{
    seleccion->start(120);
}

void Krilin::detenerAnimacion()
{
    seleccion->stop();
}

