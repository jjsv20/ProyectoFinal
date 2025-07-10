#ifndef OBJETOS_H
#define OBJETOS_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include "goku.h"

class Objetos : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Objetos(QString tiopObejto, QObject *parent = nullptr);
    //virtual void mover();
    void detener();
    void reanudar();
    QString getTipo() const;
signals:
    void eliminarRoca(Objetos *rocas);
    void eliminarPiedra(Objetos *piedra);
    void eliminarAve(Objetos *ave);

public slots:
    void moverObjeto();

private:
    QTimer *timer;
    QString tipo;

    QPixmap *pixmap;
    int frameActual = 0;
    int anchoFrame = 60;
    int altoFrame = 60;
    int contadorMovimiento = 0;
    int contadorAnimacion = 0;
};

#endif // OBJETOS_H
