#include "goku.h"

Goku::Goku(QObject *parent)
    : QObject{parent}
{
    coordenadaX = 0;
    coordenadaY = 0;
    ancho = 66;
    alto = 100;
    pixmap = new QPixmap(":/imagenes/movimientosGoku.png");
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
    setPos(150, 380);

    frameActual = 0;
    totalFramesDerecha = 6;

    seleccion = new QTimer(this);
    connect(seleccion, &QTimer::timeout, this, &Goku::actualizar);

    caminar = new QTimer(this);
    connect(caminar, &QTimer::timeout, this, &Goku::animar);

    saltar = new QTimer(this);
    connect(saltar, &QTimer::timeout, this, &Goku::animarSalto);

    moviendoDerecha = false;
    moviendoIzquierda = false;
    saltando = false;

    gravedad = 9;
    velocidadY = 0;
    velocidadX = 0;
    sueloY = 380;

}

QRectF Goku::boundingRect() const {
    return QRectF(0, 0, ancho, alto);
}

void Goku::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->drawPixmap(0, 0, *pixmap, coordenadaX, coordenadaY, ancho, alto);
}

void Goku::animar()
{
    if(saltando) return;
    if (moviendoDerecha) {
        coordenadaY = 100;
        frameActual = (frameActual + 1) % totalFramesDerecha;
        coordenadaX = frameActual * ancho;
        setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
        setPos(x() + 10, y());
        emit moverFondoSignal(10);
    } else if (moviendoIzquierda) {
        coordenadaY = 200;
        frameActual = (frameActual + 1) % 8;
        coordenadaX = frameActual * ancho;
        setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
        if (x() > 10)
            setPos(x() - 10, y());
        //emit moverFondoSignal(-10);
    }
}

void Goku::animarSalto()
{
    coordenadaY = 300;
    frameActual = (frameActual + 1) % 9;
    coordenadaX = frameActual * ancho;
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
    setX(x() + velocidadX);
    setY(y() + velocidadY);
    velocidadY += gravedad;
    if (y() >= sueloY) {
        setY(sueloY);
        velocidadY = 0;
        saltando = false;
        saltar->stop();
        velocidadX = 0;
        volverASeleccion();
    }
}

void Goku::actualizar()
{
    coordenadaX += ancho;
    if(coordenadaX >= 300){
        coordenadaX = 0;
    }
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
}

void Goku::keyReleaseEvent(QKeyEvent *event)
{
    if ((event->key() == Qt::Key_Right && !event->isAutoRepeat() && moviendoDerecha) ||
        (event->key() == Qt::Key_Left && !event->isAutoRepeat() && moviendoIzquierda)){
        caminar->stop();
        moviendoDerecha = false;
        moviendoIzquierda = false;
        saltando =false;
        volverASeleccion();
    }
}

void Goku::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right && !event->isAutoRepeat()) {
        moviendoDerecha = true;
        moviendoIzquierda = false;
        if (saltando) {
            velocidadX = 10;
        }
        caminar->start(40);
    }else if(event->key() == Qt::Key_Left && !event->isAutoRepeat()){
        moviendoIzquierda = true;
        moviendoDerecha = false;
        if (saltando) {
            velocidadX = -10;
        }
        caminar->start(40);
    }else if (event->key() == Qt::Key_Space  && !event->isAutoRepeat()) {
        if(!saltando){
            if (moviendoDerecha){
                velocidadX = 10;
            }else if (moviendoIzquierda){
                velocidadX = -10;
            }else velocidadX = 0;{
                caminar->stop();
                velocidadY = -40;
                saltar->start(100);
            }
        }
    }
}

void Goku::volverASeleccion()
{
    coordenadaX = 0;
    coordenadaY = 0;
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
}

void Goku::iniciarAnimacion()
{
    seleccion->start(120);
}

void Goku::detenerAnimacion()
{
    seleccion->stop();
}
