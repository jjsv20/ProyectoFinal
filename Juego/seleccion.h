#ifndef SELECCION_H
#define SELECCION_H

#include <QWidget>
#include <QPainter>
//#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QRectF>
#include <QDebug>
#include <QPushButton>
#include "krilin.h"
#include "goku.h"

namespace Ui {
class Seleccion;
}

class Seleccion : public QWidget
{
    Q_OBJECT

public:
    explicit Seleccion(QWidget *parent = nullptr);
    ~Seleccion();

signals:
    void regresarMenu();

private slots:
    void on_regresar_clicked();

    void on_btnKrilin_clicked();

    void on_bthGoku_clicked();

private:
    Ui::Seleccion *ui;
    QGraphicsScene *seleccion;
    Krilin *krilin;
    Goku *goku;
    QGraphicsPixmapItem *fondoItem;
};

#endif // SELECCION_H
