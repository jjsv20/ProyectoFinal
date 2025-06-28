#ifndef ENTRENAMIENTO_H
#define ENTRENAMIENTO_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include "goku.h"
#include "krilin.h"

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

private:
    Ui::Entrenamiento *ui;
    QGraphicsScene *escenaEntrenamiento;
    QGraphicsPixmapItem *fondoE;
    Goku *goku;
    Krilin *krilin;
    QString personaje;
};

#endif // ENTRENAMIENTO_H
