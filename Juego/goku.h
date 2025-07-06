#ifndef GOKU_H
#define GOKU_H

#include "personaje.h"
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
    void reacionGolpe();
    void desactivarTimers() override;
    void reanudarAnimacion() override;
    void setPausa(bool estado) { enPausa = estado; }

/*/signals:
    void moverFondoSignal(int dx);
    void reiniciarPartida();
    void finalPartida();
    void partidaCompletada();/*/

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
};

#endif // GOKU_H
