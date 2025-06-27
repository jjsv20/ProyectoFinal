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

class Goku : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Goku(QObject *parent = nullptr);
    void iniciarAnimacion();
    void detenerAnimacion();

public slots:
    void animar();

signals:
private:
    QTimer *seleccion;
    QPixmap *gokuSeleccion;
    int cuadroActual;
    float ancho;
    int alto;
    int totalCuadros;
};

#endif // GOKU_H
