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
#include <QTransform>

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
    void animar();
    void actualizar();
    void animarSalto();

protected:
    void keyReleaseEvent(QKeyEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
signals:
    void moverFondoSignal(int dx);
private:
    void moverDerecha();
    void moverIzquierda();
    void moverArriba();
    //QTimer* controlTeclas;
    //QSet<int> teclasPresionadas;
    QTimer *seleccion;
    QPixmap *pixmap, *fondo, *frame, frameIzquierda;
    QGraphicsPixmapItem *krilinSeleccion;
    QTimer *caminar, *saltar;

    bool moviendoDerecha, moviendoIzquierda, saltando;
    int coordenadaX, coordenadaY;
    int ancho, alto;

    int sueloY, velocidadY, velocidadX, gravedad;

    int frameActual;
    int totalFramesDerecha;
};

#endif // KRILIN_H
