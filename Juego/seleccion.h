#ifndef SELECCION_H
#define SELECCION_H

#include <QWidget>
#include <QPainter>
#include <QGraphicsItem>
#include <QRectF>
#include <QDebug>
#include <QPushButton>
#include "personaje.h"
#include "widget.h"
#include "krilin.h"

namespace Ui {
class Seleccion;
}

class Seleccion : public QWidget
{
    Q_OBJECT

public:
    explicit Seleccion(QWidget *parent = nullptr);
    ~Seleccion();
    void actualizar();
    void reiniciar();

signals:
    void regresarMenu();
    void personajeSeleccionado(QString nombre, int vidas, int nivel, int derrotas);

private slots:
    void on_regresar_clicked();

    void on_btnKrilin_clicked();

    void on_bthGoku_clicked();

    void on_btnComenzar_clicked();

private:
    Ui::Seleccion *ui;
    QGraphicsScene *seleccion;
    Personaje *krilin, *goku;
    QGraphicsPixmapItem *fondoItem;
    QString personajeElegido;
};

#endif // SELECCION_H
