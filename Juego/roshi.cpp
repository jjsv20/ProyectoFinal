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
    timerAtaqueRoshi->start(1000 + QRandomGenerator::global()->bounded(900));
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
        muerteRoshi.play();
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


void Roshi::reiniciar()
{
    setEstaMuerto(false);
    golpeRecibido = false;
    atacando = false;
    frameAtaque = 0;
    coordenadaX = 0;
    coordenadaY = 0;
    actualizar();
    detenerAnimacion();
}

void Roshi::moverRoshi()
{
    if (!objetivo) return;

    qreal distancia = objetivo->x() - x();

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
    if (atacando || !getRival() || getEstaMuerto()) return;

    qreal distancia = qAbs(x() - objetivo->x());
    if (distancia > 80) return;

    atacando = true;
    frameAtaque = 0;
    bool yaGolpeo = false;

    int tipoAtaque = QRandomGenerator::global()->bounded(2); // 0 o 1
    int coordY_ataque = (tipoAtaque == 0) ? 100 : 200;

    QTimer *animacion = new QTimer(this);
    connect(animacion, &QTimer::timeout, this, [=]() mutable {
        coordenadaY = coordY_ataque;
        coordenadaX = frameAtaque * ancho;
        setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
        frameAtaque++;

        if (!yaGolpeo && objetivo && collidesWithItem(objetivo)) {
            objetivo->reaccionGolpe();
            qDebug() << "Roshi golpeó";
            yaGolpeo = true;
        }

        if (frameAtaque >= 6) {
            animacion->stop();
            animacion->deleteLater();
            atacando = false;
            yaGolpeo = false;
            coordenadaX = 0;
            coordenadaY = 0;
            setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
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

void Roshi::desactivarTimers()
{
    timerAtaqueRoshi->stop();
    timerMovimientoRoshi->stop();
}

void Roshi::reanudarAnimacion()
{
    timerAtaqueRoshi->start(1500);
    timerMovimientoRoshi->start(50);
}
