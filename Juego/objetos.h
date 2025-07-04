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

public slots:
    void moverObjeto();

private:
    QTimer *timer;
    QString tipo;
};

#endif // OBJETOS_H
