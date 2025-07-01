#ifndef KRILIN_H
#define KRILIN_H

#include "personaje.h"
#include <QTimer>
#include <QPixmap>
#include <QKeyEvent>
#include <QDebug>
#include <QPainter>
#include <piedras.h>
#include <objetos.h>

class Krilin : public Personaje
{
    Q_OBJECT
public:
    explicit Krilin(QObject *parent = nullptr);
    void iniciarAnimacion() override;
    void detenerAnimacion() override;
    void perderVida() override;
    void reiniciar() override;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

public slots:
    void animar();
    void animarSalto();
    void actualizar();
    void volverASeleccion();
    void colisionPiedras();
    void colisionRocas();
    void desactivarTimers() override;
    void reanudarAnimacion() override;
    void setPausa(bool estado) { enPausa = estado; }

protected:
    void keyReleaseEvent(QKeyEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void moverDerecha();
    void moverIzquierda();
    void moverArriba();

    QTimer *seleccion, *caminar, *saltar, *timerColision;
    QPixmap *pixmap;
    QGraphicsPixmapItem *krilinSeleccion;

    bool moviendoDerecha, moviendoIzquierda, saltando;
    int coordenadaX, coordenadaY;
    int ancho, alto;
    bool nivelCompletado;
    int sueloY, velocidadY, velocidadX, gravedad;

    int frameActual = 0;
    int totalFramesDerecha;
};

#endif // KRILIN_H
