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

void Seleccion::reiniciar()
{
    personajeElegido.clear();
    if (krilin) {
        krilin->iniciarAnimacion();
    }
    if (goku) {
        goku->iniciarAnimacion();
    }

}

void Seleccion::on_btnKrilin_clicked()
{
    krilin->detenerAnimacion();
     goku->detenerAnimacion();
    personajeElegido = "Krilin";
}


void Seleccion::on_bthGoku_clicked()
{
    goku->detenerAnimacion();
    krilin->detenerAnimacion();
    personajeElegido = "Goku";
}


void Seleccion::on_regresar_clicked()
{
    emit regresarMenu();
}


void Seleccion::on_btnComenzar_clicked()
{
    if (personajeElegido.isEmpty()) {
        return;
    }
     qDebug() << "Personaje elegido:" << personajeElegido;
    emit personajeSeleccionado(personajeElegido, 5, 1, 0);
}

