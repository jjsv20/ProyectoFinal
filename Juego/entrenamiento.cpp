#include "entrenamiento.h"
#include "ui_entrenamiento.h"

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
    fondoE = new QGraphicsPixmapItem(fondo);
    fondoE->setZValue(-1);
    escenaEntrenamiento->addItem(fondoE);


    if(personaje == "Goku"){

    }else if(personaje == "Krilin"){
        krilin = new Krilin();
        krilin->setPos(100, 500);
        krilin->setScale(1.0);
        krilin->setFlag(QGraphicsItem::ItemIsFocusable);
        krilin->setFocus();
        escenaEntrenamiento->addItem(krilin);
        krilin->iniciarAnimacion();
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
