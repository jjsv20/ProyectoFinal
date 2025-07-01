#include "seleccion.h"
#include "widget.h"
#include "ui_seleccion.h"

Seleccion::Seleccion(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Seleccion)
{
    ui->setupUi(this);
    seleccion = new QGraphicsScene;
    seleccion->setSceneRect(0, 0, 1080, 720);
    ui->graphicsView->setScene(seleccion);

    QPixmap fondo(":/imagenes/seleccion.png");
    fondoItem = new QGraphicsPixmapItem(fondo);
    fondoItem->setZValue(-1);
    seleccion->addItem(fondoItem);

    ui->graphicsView->setScene(seleccion);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    krilin = new Krilin();
    seleccion->addItem(krilin);
    krilin->setScale(2.4);
    krilin->iniciarAnimacion();

    goku = new Goku();
    seleccion->addItem(goku);
    goku->setScale(2.5);
    goku->iniciarAnimacion();
}

Seleccion::~Seleccion()
{
    delete ui;
}

void Seleccion::on_regresar_clicked()
{
    emit regresarMenu();
    this->close();
}


void Seleccion::on_btnKrilin_clicked()
{
    krilin->detenerAnimacion();
    goku->detenerAnimacion();
    emit personajeSeleccionado("Krilin");
    this->close();
}


void Seleccion::on_bthGoku_clicked()
{
    goku->detenerAnimacion();
    krilin->detenerAnimacion();
    emit personajeSeleccionado("Goku");
    this->close();
}

