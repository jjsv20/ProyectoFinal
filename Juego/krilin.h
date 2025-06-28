#ifndef KRILIN_H
#define KRILIN_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QTimer>
#include <QPixmap>
#include <QKeyEvent>
#include <QDebug>
#include <QBitmap>
#include <QPainter>
#include <QSet>

class Krilin : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Krilin(QObject *parent = nullptr);
    void iniciarAnimacion();
    void detenerAnimacion();
    void volverASeleccion();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

public slots:
    void animarDerecha();
    void actualizar();

protected:
    void keyReleaseEvent(QKeyEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

signals:
private:
    QTimer* controlTeclas;
    QSet<int> teclasPresionadas;
    QTimer *seleccion;
    QPixmap *pixmap, *fondo;
    QGraphicsPixmapItem *krilinSeleccion;
    QTimer* caminar;
    bool moviendoDerecha;
    int coordenadaX, coordenadaY;
    int ancho, alto;
    void moverDerecha();
    void moverIzquierda();
    void moverArriba();
    int frameActual;
    int totalFramesDerecha;
};

#endif // KRILIN_H
