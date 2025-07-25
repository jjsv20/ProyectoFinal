#ifndef GOKU_H
#define GOKU_H

#include "personaje.h"
#include "roshi.h"
#include <QTimer>
#include <QPixmap>
#include <QKeyEvent>
#include <QDebug>
#include <QPainter>
#include <objetos.h>

class Goku : public Personaje
{
    Q_OBJECT
public:
    explicit Goku(QObject *parent = nullptr);
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
    void animarPuno();
    void animarPatada();
    void reaccionGolpe() override;
    void desactivarTimers() override;
    void reanudarAnimacion() override;
    void setPausa(bool estado) { enPausa = estado; }
    void spriteVictoria() override;

protected:
    void keyReleaseEvent(QKeyEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void moverDerecha();
    void moverIzquierda();
    void moverArriba();

    QTimer *seleccion, *saltar, *timerColision, *caminar;
    QPixmap *pixmap;
    QGraphicsPixmapItem *gokuSeleccion;

    bool moviendoDerecha;
    bool moviendoIzquierda;
    bool saltando;

    int coordenadaX, coordenadaY;
    int ancho, alto;
    bool nivelCompletado;
    int sueloY, velocidadY, velocidadX, gravedad;

    int frameActual = 0;
    int totalFramesDerecha;

    bool golpeRecibido;
    bool yaGolpeo = false;
};

#endif // GOKU_H
