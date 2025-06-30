#include "entrenamiento.h"
#include "ui_entrenamiento.h"
#include "krilin.h"
#include "objetos.h"
#include <QRandomGenerator>
#include <QMessageBox>

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

    //QPixmap fondo(":/imagenes/isla.png");
    QPixmap fondo(":/imagenes/fondoN1.png");
    widthFondo = fondo.width();
    fondoE = new QGraphicsPixmapItem(fondo);
    fondoE->setZValue(-1);
    escenaEntrenamiento->addItem(fondoE);

    fondoEE = new QGraphicsPixmapItem(fondo);
    fondoEE->setPos(widthFondo, 0);
    fondoEE->setZValue(-1);
    escenaEntrenamiento->addItem(fondoEE);

    suelo = new QGraphicsRectItem(0, 592, 1080, 40);
    suelo->setBrush(Qt::NoBrush);
    suelo->setPen(Qt::NoPen);
    escenaEntrenamiento->addItem(suelo);

    if(personaje == "Goku"){
        goku = new Goku();
        goku->setPos(100, 500);
        goku->setScale(1.5);
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

    nPiedras = 0;
    timerPiedras = new QTimer(this);
    connect(timerPiedras, &QTimer::timeout, this, &Entrenamiento::crearPiedras);
    timerPiedras->start(5000);

    nRocas = 0;
    timerRocas = new QTimer(this);
    connect(timerRocas, &QTimer::timeout, this, &Entrenamiento::crearRocas);
    timerRocas->start(2000);

    texto = new QGraphicsTextItem("x 0");
    texto->setDefaultTextColor(Qt::yellow);
    texto->setFont(QFont("Arial", 24));
    texto->setPos(30, 20);
    escenaEntrenamiento->addItem(texto);
    goku->setPuntos(texto);
}

Entrenamiento::~Entrenamiento()
{
    delete ui;
}

void Entrenamiento::on_pausa_clicked()
{
    goku->detenerAnimacion();
    krilin->detenerAnimacion();
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

void Entrenamiento::crearPiedras()
{
    if(piedras.count() < 10){
        Piedras *p = new Piedras();
        escenaEntrenamiento->addItem(p);
        piedras.append(p);

        connect(p, &Piedras::eliminarLista, this, [=](Piedras *piedra){
            piedras.removeOne(piedra);
        });
    }
}

void Entrenamiento::crearRocas()
{
    if(rocas.count() < 4){
        Objetos *r = new Objetos(this);
        escenaEntrenamiento->addItem(r);
        rocas.append(r);

        connect(r, &Objetos::eliminarRoca, this, [=](Objetos *roca){
            rocas.removeOne(roca);
        });
    }
}


