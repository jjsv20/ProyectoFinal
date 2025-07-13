#ifndef COMBATE_H
#define COMBATE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QDebug>
#include "goku.h"
#include "krilin.h"
#include "roshi.h"
#include "entrenamiento.h"

namespace Ui {
class Combate;
}

class Combate : public QWidget
{
    Q_OBJECT

public:
    explicit Combate(QWidget *parent = nullptr);
    ~Combate();

    void pantallaDerrota();
    void pantallaVictoria();
    void iniciarCombate(QString personajeSeleccionado);
    void iniciarNivel2(QString personajeSeleccionado);
    void iniciarCombateTuto();

    void mensajeFinal(QString mensaje);

    void detenerTimersGlobales();
    void reanudarTimersGlobales();

public slots:
    void cuentaRegresiva();
    void limpiaObjetos();

signals:
    void combateTerminado();
    void volverSeleccionar();

private slots:
    void on_pausa_clicked();

private:
    Ui::Combate *ui;
    QGraphicsScene* escenaCombate;
    Goku* goku;
    Roshi* roshi;
    Krilin* krilin;

    Personaje *personajeJugador;
    int nivelActual = 2;
    int contadorDerrotas;
    QString personaje;
    QGraphicsTextItem* vidasTexto;

    int tiempo;
    QTimer *timerTiempo;
    QGraphicsTextItem *textoTiempo;


    QGraphicsRectItem* derrotaPantalla;
    QGraphicsTextItem* derrotaTexto;
    QGraphicsRectItem* victoriaPantalla;
    QGraphicsTextItem* victoriaTexto;

    QGraphicsPixmapItem *avatar, *avatarRoshi;

    int rondasGanadasJugador = 0;
    int rondasGanadasRoshi = 0;
    int rondaActual = 1;

     bool tutorialMostrado = false;

};

#endif // COMBATE_H
