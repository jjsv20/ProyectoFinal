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

private slots:
    void on_pausa_clicked();

public slots:
    void moverFondo(int dx);
    void crearPiedras();
    void crearRocas();
    void crearAves();
    void pantallaDerrota();
    void pantallaVictoria();
    void detenerTimersGlobales();
    void reanudarTimersGlobales();
    void limpiaObjetos();
    void iniciarNivel1(QString personajeSeleccionado, int vidasIniciales, int nivel, int derrotas);
    void iniciarNivelTuto(QString personajeSeleccionado, int nivel);
    void mostrarTutorial();
    void cuentaRegresiva();

protected:
    void keyPressEvent(QKeyEvent *event);

signals:
    void volverSeleccionar();
    void entrenamientoTerminado(QString personajeSeleccionado, int vidasIniciales, int nivel, int derrotas);

private:
    Ui::Entrenamiento *ui;
    QGraphicsScene *escenaEntrenamiento;

    QGraphicsPixmapItem *fondoE, *fondoEE, *avatar, *piedraItem;
    QGraphicsTextItem *texto, *vidasT;

    QTimer *timerPiedras;
    int nPiedras;
    QList<Objetos *> piedras;
    QTimer *timerRocas;
    int nRocas;
    QList<Objetos *> rocas;
    int nAves;
    QList<Objetos *> aves;
    QTimer *timerAves;

    int cantidadPiedras;
    int widthFondo;
    int fondoMinX;

    Personaje *personajeActual;
    QString personaje;
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

    bool tutorialMostrado = false;

};

#endif // ENTRENAMIENTO_H
