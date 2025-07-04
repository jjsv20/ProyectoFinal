#include "piedras.h"
#include <QTimer>

Piedras::Piedras(QObject *parent)
    : QObject{parent}
{
    setPixmap(QPixmap(":/imagenes/piedra.png"));
    setPos(1080, 400);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Piedras::moverPiedra);
    timer->start(20);
}

void Piedras::detener()
{
    if (timer && timer->isActive()) {
        timer->stop();
    }
}

void Piedras::reanudar()
{
    if (timer && !timer->isActive()) {
        timer->start(20);
    }
}

void Piedras::moverPiedra()
{
    int newX = x() - 5;
    setPos(newX, y());
    if(newX + pixmap().width() <= 0){
        emit eliminarLista(this);
        if (timer) {
            timer->stop();
            timer->deleteLater();
            timer = nullptr;
        }
        emit eliminarLista(this);

        if (scene()) {
            scene()->removeItem(this);
        }

        this->deleteLater();
    }
}
