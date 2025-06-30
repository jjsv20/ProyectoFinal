#ifndef OBJETOS_H
#define OBJETOS_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

class Objetos : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Objetos(QObject *parent = nullptr);

signals:
    void eliminarRoca(Objetos *rocas);

public slots:
    void moverRoca();

private:
    QTimer *timer;
};

#endif // OBJETOS_H
