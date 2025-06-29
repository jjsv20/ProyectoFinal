#ifndef GOKU_H
#define GOKU_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QTimer>
#include <QPixmap>
#include <QKeyEvent>
#include <QDebug>
#include <QBitmap>
#include <QPainter>

class Goku : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Goku(QObject *parent = nullptr);
    void iniciarAnimacion();
    void detenerAnimacion();
    void volverASeleccion();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

public slots:
    void animar();
    void animarSalto();
    void actualizar();

signals:
    void moverFondoSignal(int dx);

protected:
    void keyReleaseEvent(QKeyEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

signals:
private:
    void moverDerecha();
    void moverIzquierda();
    void moverArriba();
    QTimer *seleccion, *saltar;
    QPixmap *pixmap;
    QGraphicsPixmapItem *gokuSeleccion;
    QTimer* caminar;

    bool moviendoDerecha, moviendoIzquierda, saltando;
    int coordenadaX, coordenadaY;
    int ancho, alto;

    int sueloY, velocidadY, velocidadX, gravedad;

    int frameActual;
    int totalFramesDerecha;
};

#endif // GOKU_H
