#include "roshi.h"

Roshi::Roshi(QObject *parent) : Personaje{parent}
{
    setPos(800, 380);
    ancho = 100;
    alto = 100;
    coordenadaX = 0;
    coordenadaY = 0;
    frameAtaque = 0;
    atacando = false;

    pixmap = new QPixmap(":/imagenes/3-JackieChun.png");
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

void Roshi::reaccionGolpe()
{
    if(getEstaMuerto() || golpeRecibido) return;

    golpeRecibido = true;
    sonidoGolpeRecibidoRoshi.play();
    coordenadaY = 400;
    coordenadaX = 0;
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
    setX(x() + 90);
    setOpacity(0.5);

    if (getContadorVidas() > 0) {
        setContadorVidas(getContadorVidas() - 1);
        actualizarBarraVida();
        qDebug() << "Roshi vidas restantes: " << getContadorVidas();
    }

    if (getContadorVidas() <= 0) {
        setEstaMuerto(true);
        detenerAnimacion();
        qDebug() << "¡Roshi ha sido derrotado!";
        emit finalPartida();
    }

    QTimer::singleShot(400, this, [=]() {
        setOpacity(1.0);
        golpeRecibido = false;
        if (!getEstaMuerto()) {
            iniciarRoshi();
        }
    });

}

void Roshi::moverRoshi()
{
    if (!rival) return;

    qreal distancia = rival->x() - x();

    if (qAbs(distancia) < 60) return;

    if (qAbs(distancia) < 10) return;

    if (distancia > 0 && x() < 1000) {
        setX(x() + 7);
    }
    else if(distancia < 0 && (x() > 100)){
        setX(x() - 7);
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
        coordenadaY = 100;
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
            if (getRival()) {
                getRival()->reaccionGolpe();
            }
        }
    });

    animacion->start(60);
}

void Roshi::actualizar()
{
    coordenadaY = 200;
    frameAtaque = 7;
    frameAtaque = (frameAtaque + 1) % 6;
    coordenadaX = frameAtaque * ancho;
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
}
