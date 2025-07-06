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
    } else if (tipo == "piedra") {
        setPixmap(QPixmap(":/imagenes/piedra.png"));
    }

    setPos(1080, QRandomGenerator::global()->bounded(600));
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
        moveBy(-5, 0);
    } else if (tipo == "piedra") {
        moveBy(-8, 0);
    }

    if (x() < -50) {
        if (tipo == "roca")
            emit eliminarRoca(this);
        else if (tipo == "piedra")
            emit eliminarPiedra(this);

        deleteLater();
    }
}
