#include "objetos.h"
#include <QTimer>

Objetos::Objetos(QObject *parent)
    : QObject{parent}
{
    setPixmap(QPixmap(":/imagenes/roca.png"));
    setPos(1080, 600);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Objetos::moverRoca);
    timer->start(20);
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



void Objetos::moverRoca()
{
    int newX = x() - 5;
    setPos(newX, y());
    if(newX + pixmap().width() <= 5){
        emit eliminarRoca(this);
        if(scene()){
            scene()->removeItem(this);
            deleteLater();
            return;
        }
    }
}
