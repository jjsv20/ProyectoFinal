#include "entrenamiento.h"
#include "ui_entrenamiento.h"
#include "krilin.h"

Entrenamiento::Entrenamiento(QString personajeSeleccionado, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Entrenamiento)
    , personaje(personajeSeleccionado)
{
    ui->setupUi(this);

    escenaEntrenamiento = new QGraphicsScene(0, 0, 1080, 720);
    ui->graphicsView->setScene(escenaEntrenamiento);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QPixmap fondo(":/imagenes/isla.png");
    //QPixmap fondo(":/imagenes/fondoN1.png");
    widthFondo = fondo.width();
    fondoE = new QGraphicsPixmapItem(fondo);
    fondoE->setZValue(-1);
    escenaEntrenamiento->addItem(fondoE);

    fondoEE = new QGraphicsPixmapItem(fondo);
    fondoEE->setPos(widthFondo, 0);
    fondoEE->setZValue(-1);
    escenaEntrenamiento->addItem(fondoEE);

    if(personaje == "Goku"){
        goku = new Goku();
        goku->setPos(100, 500);
        goku->setScale(1.0);
        goku->setFlag(QGraphicsItem::ItemIsFocusable);
        goku->setFocus();
        escenaEntrenamiento->addItem(goku);
        goku->iniciarAnimacion();
        connect(goku, &Goku::moverFondoSignal, this, &Entrenamiento::moverFondo);
    }else if(personaje == "Krilin"){
        krilin = new Krilin();
        krilin->setPos(100, 500);
        krilin->setScale(1.0);
        krilin->setFlag(QGraphicsItem::ItemIsFocusable);
        krilin->setFocus();
        escenaEntrenamiento->addItem(krilin);
        krilin->iniciarAnimacion();
        connect(krilin, &Krilin::moverFondoSignal, this, &Entrenamiento::moverFondo);
    }
}

Entrenamiento::~Entrenamiento()
{
    delete ui;
}

void Entrenamiento::on_pausa_clicked()
{

}

void Entrenamiento::on_reanudar_clicked()
{

}

void Entrenamiento::on_salir_clicked()
{
    close();
}

void Entrenamiento::moverFondo(int dx)
{
    fondoE->moveBy(-dx, 0);
    fondoEE->moveBy(-dx, 0);

    if (fondoE->x() + widthFondo <= 0) {
        fondoE->setX(fondoEE->x() + widthFondo);
    }
    if (fondoEE->x() + widthFondo <= 0) {
        fondoEE->setX(fondoE->x() + widthFondo);
    }
}
