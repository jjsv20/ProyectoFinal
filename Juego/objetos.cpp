#include "objetos.h"
#include <QTimer>
#include <QRandomGenerator>

Objetos::Objetos(QString tipoObjeto, QObject *parent)
    : QObject{parent}, tipo(tipoObjeto)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Objetos::moverObjeto);
    timer->start(20);

    if (tipo == "roca") {
        setPixmap(QPixmap(":/imagenes/roca.png"));
        setPos(800, 200);
    } else if (tipo == "piedra") {
        setPixmap(QPixmap(":/imagenes/piedra.png"));
    }else if(tipo == "ave"){
        pixmap = new QPixmap(":/imagenes/ave.png");
        anchoFrame = 55;
        altoFrame = 55;
        setPixmap(pixmap->copy(0, 0, anchoFrame, altoFrame));
        setPos(500, 200);
    }

    setPos(1080, QRandomGenerator::global()->bounded(360, 540));
}

void Objetos::detener()
{
    if (timer && timer->isActive()) {
        timer->stop();
    }
}

void Objetos::reanudar()
{
    if (timer && !timer->isActive()) {
        timer->start(20);
    }
}

QString Objetos::getTipo() const
{
    return tipo;
}

void Objetos::moverObjeto()
{
    if (tipo == "roca") {
        moveBy(-10, 0);
    } else if (tipo == "piedra") {
        moveBy(-8, 0);
    }else if(tipo == "ave"){
        qreal dx = -6;
        qreal dy = 5 * std::sin(contadorMovimiento * 0.1);
        moveBy(dx, dy);
        contadorMovimiento++;
        contadorAnimacion++;
        if (contadorAnimacion % 5 == 0) {
            frameActual = (frameActual + 1) % 4;
            setPixmap(pixmap->copy(frameActual * anchoFrame, 0, anchoFrame, altoFrame));
        }
    }

    if (x() < -50) {
        if (tipo == "roca")
            emit eliminarRoca(this);
        else if (tipo == "piedra")
            emit eliminarPiedra(this);
        else if(tipo == "ave"){
            emit eliminarAve(this);
        }
        deleteLater();
    }
}
