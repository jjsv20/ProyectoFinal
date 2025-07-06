#ifndef ENTRENAMIENTO_H
#define ENTRENAMIENTO_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QPushButton>
#include "personaje.h"
#include "objetos.h"

namespace Ui {
class Entrenamiento;
}

class Entrenamiento : public QWidget
{
    Q_OBJECT

public:
    explicit Entrenamiento(QWidget *parent = nullptr);
    ~Entrenamiento();

    //void iniciarNivel1(QString personajeSeleccionado, int vidasIniciales, int nivel);

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
    void limpiaObjetos();
    void iniciarNivel1(QString personajeSeleccionado, int vidasIniciales, int nivel, int derrotas);
    void cuentaRegresiva();

signals:
    void volverSeleccionar();
    void entrenamientoTerminado(QString personajeSeleccionado, int vidasIniciales, int nivel, int derrotas);

private:
    Ui::Entrenamiento *ui;
    QGraphicsScene *escenaEntrenamiento;

    QGraphicsPixmapItem *fondoE, *fondoEE;
    QGraphicsTextItem *texto, *vidasT;

    QTimer *timerPiedras;
    int nPiedras;
    QList<Objetos *> piedras;
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

    QGraphicsRectItem* derrotaPantalla;
    QGraphicsTextItem* derrotaTexto;
    QPushButton* botonReiniciar;
    QPushButton* botonMenu;

    QGraphicsRectItem* victoriaPantalla;
    QGraphicsTextItem* victoriaTexto;
    QPushButton* botonContinuar;
    QPushButton* botonMenu1;

    QTimer *timerTiempo;
    int tiempo = 0;
    QGraphicsTextItem *textoTiempo;

    int contadorDerrotas;
};

#endif // ENTRENAMIENTO_H
