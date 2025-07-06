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
    void iniciarCombate(QString personajeSeleccionado, int nivel, int derrotas);

public slots:
    void cuentaRegresiva();
    void limpiaObjetos();

signals:
    void combateTerminado();

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

};

#endif // COMBATE_H
