#ifndef ENTRENAMIENTO_H
#define ENTRENAMIENTO_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include "goku.h"
#include "krilin.h"
#include "piedras.h"
#include "objetos.h"

namespace Ui {
class Entrenamiento;
}

class Entrenamiento : public QWidget
{
    Q_OBJECT

public:
    explicit Entrenamiento(QString personajeSeleccionado, QWidget *parent = nullptr);
    ~Entrenamiento();

private slots:
    void on_pausa_clicked();
    void on_reanudar_clicked();
    void on_salir_clicked();
public slots:
    void moverFondo(int dx);
    void crearPiedras();
    void crearRocas();
private:
    Ui::Entrenamiento *ui;
    QGraphicsScene *escenaEntrenamiento;

    QGraphicsPixmapItem *fondoE, *fondoEE;
    QGraphicsRectItem *suelo;
    QGraphicsTextItem *texto;

    QTimer *timerPiedras;
    int nPiedras;
    QList<Piedras *> piedras;
    QTimer *timerRocas;
    int nRocas;
    QList<Objetos *> rocas;

    int cantidadPiedras;
    int widthFondo;
    int fondoMinX;

    Goku *goku;
    Krilin *krilin;
    QString personaje;
    //Objetos *piedra;
};

#endif // ENTRENAMIENTO_H
