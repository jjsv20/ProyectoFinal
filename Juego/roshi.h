#ifndef ROSHI_H
#define ROSHI_H

#include "personaje.h"
#include <QObject>
#include <QTimer>
#include <QPixmap>
#include <QKeyEvent>
#include <QDebug>
#include <QPainter>
#include <QRandomGenerator>
#include "goku.h"
#include <QSoundEffect>

class Roshi : public Personaje
{
    Q_OBJECT
public:
    explicit Roshi(QObject *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

    void iniciarRoshi();
    void reaccionGolpe() override;

    //void setRival(Personaje *rival);

public slots:
    void moverRoshi();
    void atacarRoshi();
    void actualizar();

private:
    QTimer *timerMovimientoRoshi;
    QTimer *timerAtaqueRoshi;
    QPixmap *pixmap;
    int frameAtaque, ancho, alto, coordenadaX, coordenadaY;
    bool atacando;
    bool golpeRecibido;
    //Personaje *rival;


signals:
};

#endif // ROSHI_H
