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
    explicit Entrenamiento(QString personajeSeleccionado, int vidasIniciales, int piedrasIniciales, int nivel, int derrotas, QWidget *parent = nullptr);
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
    void reiniciarPartida(int vidasr, int piedrasr);

signals:
    void volverSeleccionar();

private:
    Ui::Entrenamiento *ui;
    QGraphicsScene *escenaEntrenamiento;

    QGraphicsPixmapItem *fondoE, *fondoEE;
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
    int nivelActual = 1;
    int derrotasNivel = 0;
};

#endif // ENTRENAMIENTO_H
