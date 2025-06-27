#ifndef KRILIN_H
#define KRILIN_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QTimer>
#include <QPixmap>
#include <QKeyEvent>
#include <QDebug>

class Krilin : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Krilin(QObject *parent = nullptr);
    void iniciarAnimacion();
    void detenerAnimacion();

public slots:
    void animar();

signals:
private:
    QTimer *seleccion;
    QPixmap *krilinSeleccion, *fondo;
    int cuadroActual;
    int ancho;
    int alto;
    int totalCuadros;
};

#endif // KRILIN_H
