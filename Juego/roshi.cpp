#include "roshi.h"

Roshi::Roshi(QObject *parent) : Personaje{parent}
{
    setPos(800, 380);
    ancho = 60;
    alto = 100;
    coordenadaX = 0;
    coordenadaY = 0;
    frameAtaque = 0;
    atacando = false;

    pixmap = new QPixmap(":/imagenes/jackieChun.png");
    setPixmap(pixmap->copy(0, 0, ancho, alto));

    timerMovimientoRoshi = new QTimer(this);
    connect(timerMovimientoRoshi, &QTimer::timeout, this, &Roshi::moverRoshi);

    timerAtaqueRoshi = new QTimer(this);
    connect(timerAtaqueRoshi, &QTimer::timeout, this, &Roshi::atacarRoshi);

    iniciarRoshi();
}

QRectF Roshi::boundingRect() const
{
    return QRectF(0, 0, ancho, alto);
}

void Roshi::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
     painter->drawPixmap(0, 0, *pixmap, coordenadaX, coordenadaY, ancho, alto);
}

void Roshi::iniciarRoshi()
{
    timerMovimientoRoshi->start(50);
    timerAtaqueRoshi->start(1500);
}

void Roshi::setRival(Personaje *rivl)
{
    rival = rivl;
}

void Roshi::moverRoshi()
{
    if (!rival) return;

    qreal distancia = rival->x() - x();

    if (qAbs(distancia) < 60) return;

    if (qAbs(distancia) < 10) return;

    if (distancia > 0 && x() < 1000) {
        setX(x() + 10);
    }
    else if(distancia < 0 && (x() > 100)){
        setX(x() - 10);
    }

    actualizar();
}

void Roshi::atacarRoshi()
{
    if (atacando || !rival) return;

    qreal distancia = qAbs(x() - rival->x());
    if (distancia > 80) return;

    atacando = true;
    frameAtaque = 0;

    QTimer *animacion = new QTimer(this);
    connect(animacion, &QTimer::timeout, this, [=]() mutable {
        coordenadaY = 200;
        coordenadaX = frameAtaque * ancho;
        setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
        frameAtaque++;

        if (frameAtaque >= 5) {
            animacion->stop();
            animacion->deleteLater();
            atacando = false;
            coordenadaX = 0;
            coordenadaY = 0;
            setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
            if (rival) {
                Goku *goku = dynamic_cast<Goku*>(rival);
                if (goku) {
                    //goku->reacionGolpe();
                }
            }
        }
    });

    animacion->start(60);
}

void Roshi::actualizar()
{
    coordenadaX = 7;
    coordenadaX += ancho;
    if(coordenadaX >= 300){
        coordenadaX = 0;
    }
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
}
