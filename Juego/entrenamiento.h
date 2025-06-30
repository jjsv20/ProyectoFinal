#ifndef ENTRENAMIENTO_H
#define ENTRENAMIENTO_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include "personaje.h"
#include "piedras.h"
#include "objetos.h"

namespace Ui {
class Entrenamiento;
}

class Entrenamiento : public QWidget
{
    Q_OBJECT

public:
    explicit Entrenamiento(QString personajeSeleccionado, int vidasIniciales, int piedrasIniciales, QWidget *parent = nullptr);
    ~Entrenamiento();

private slots:
    void on_pausa_clicked();
public slots:
    void moverFondo(int dx);
    void crearPiedras();
    void crearRocas();
    void pantallaDerrota();
    void pantallaVictoria();
    void detenerTimersGlobales();
    void reanudarTimersGlobales();
signals:
    void volverASeleccionar();

private:
    Ui::Entrenamiento *ui;
    QGraphicsScene *escenaEntrenamiento;

    QGraphicsPixmapItem *fondoE, *fondoEE;
    QGraphicsRectItem *suelo;
    QGraphicsTextItem *texto, *vidasT;

    QTimer *timerPiedras;
    int nPiedras;
    QList<Piedras *> piedras;
    QTimer *timerRocas;
    int nRocas;
    QList<Objetos *> rocas;

    int cantidadPiedras;
    int widthFondo;
    int fondoMinX;

    Personaje *personajeActual;
    QString personaje;
    //Entrenamiento *entrenamiento2;
    //Objetos *piedra;
};

#endif // ENTRENAMIENTO_H
