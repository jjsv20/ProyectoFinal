#include "entrenamiento.h"
#include "ui_entrenamiento.h"
#include "personaje.h"
#include "objetos.h"
#include "seleccion.h"
#include <QRandomGenerator>
#include <QMessageBox>

Entrenamiento::Entrenamiento(QString personajeSeleccionado, int vidasIniciales, int piedrasIniciales, QWidget *parent)
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
    //QPixmap fondo(":/imagenes/fondoN1.png");
    QPixmap fondo;
    if (piedrasIniciales >= 4) {
        fondo.load(":/imagenes/fondoN1.png");
    } else {
        fondo.load(":/imagenes/isla.png");
    }
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
        personajeActual = new Goku();
        personajeActual->setScale(1.5);
    } else if(personaje == "Krilin"){
        qDebug() << "jfhf";
        //personajeActual = new Krilin();
        //personajeActual->setScale(1.0);
    }

    personajeActual->setPos(100, 500);
    personajeActual->setFlag(QGraphicsItem::ItemIsFocusable);
    personajeActual->setFocus();
    personajeActual->setContadorVidas(vidasIniciales);
    personajeActual->setContadorPiedras(piedrasIniciales);
    escenaEntrenamiento->addItem(personajeActual);
    personajeActual->iniciarAnimacion();

    connect(personajeActual, &Personaje::moverFondoSignal, this, &Entrenamiento::moverFondo);

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
    personajeActual->setPuntos(texto);

    vidasT = new QGraphicsTextItem("Vidas: 5");
    vidasT->setDefaultTextColor(Qt::yellow);
    vidasT->setFont(QFont("Arial", 24));
    vidasT->setPos(50, 50);
    escenaEntrenamiento->addItem(vidasT);
    personajeActual->setVidas(vidasT);

    connect(personajeActual, &Personaje::finalPartida, this, &Entrenamiento::pantallaDerrota);
    connect(personajeActual, &Personaje::partidaCompletada, this, &Entrenamiento::pantallaVictoria);

}

Entrenamiento::~Entrenamiento()
{
    delete ui;
}

void Entrenamiento::on_pausa_clicked()
{
    detenerTimersGlobales();
    QGraphicsRectItem* panelPausa = new QGraphicsRectItem(0, 0, 1080, 720);
    panelPausa->setBrush(QColor(0, 0, 0, 150));
    panelPausa->setZValue(20);
    escenaEntrenamiento->addItem(panelPausa);
    QGraphicsTextItem* titulo = new QGraphicsTextItem("PAUSA");
    titulo->setDefaultTextColor(Qt::white);
    titulo->setFont(QFont("Arial", 32, QFont::Bold));
    titulo->setZValue(21);
    titulo->setPos(450, 200);
    escenaEntrenamiento->addItem(titulo);
    QPushButton* botonReanudar = new QPushButton("Reanudar", this);
    botonReanudar->setGeometry(450, 300, 180, 40);
    botonReanudar->raise();
    botonReanudar->show();

    QPushButton* botonMenu = new QPushButton("Volver al Menú", this);
    botonMenu->setGeometry(450, 360, 180, 40);
    botonMenu->raise();
    botonMenu->show();

    QPushButton* botonSalir = new QPushButton("Salir", this);
    botonSalir->setGeometry(450, 420, 180, 40);
    botonSalir->raise();
    botonSalir->show();
    connect(botonReanudar, &QPushButton::clicked, this, [=]() {
        escenaEntrenamiento->removeItem(panelPausa);
        delete titulo;
        botonReanudar->hide();
        botonMenu->hide();
        botonSalir->hide();
        reanudarTimersGlobales();
    });
    connect(botonMenu, &QPushButton::clicked, this, [=]() {
        //volver a la selección de personajes
    });
    connect(botonSalir, &QPushButton::clicked, this, []() {
        //
    });
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

void Entrenamiento::pantallaDerrota() {
    detenerTimersGlobales();

    QGraphicsRectItem* fondo = new QGraphicsRectItem(0, 0, 1080, 720);
    fondo->setBrush(QColor(0, 0, 0, 180));
    fondo->setZValue(10);
    escenaEntrenamiento->addItem(fondo);

    QGraphicsTextItem* mensaje = new QGraphicsTextItem("¡Game Over!");
    mensaje->setDefaultTextColor(Qt::white);
    mensaje->setFont(QFont("Arial", 28, QFont::Bold));
    mensaje->setPos(400, 250);
    mensaje->setZValue(11);
    escenaEntrenamiento->addItem(mensaje);
    ui->pausa->hide();

    QPushButton* botonReiniciar = new QPushButton("Volver a jugar", this);
    botonReiniciar->setGeometry(450, 300, 180, 40);
    botonReiniciar->raise();
    botonReiniciar->show();

    QPushButton* botonMenu = new QPushButton("Volver al Menú", this);
    botonMenu->setGeometry(450, 360, 180, 40);
    botonMenu->raise();
    botonMenu->show();

    connect(botonReiniciar, &QPushButton::clicked, this, [=]() {
        emit volverASeleccionar();
        this->close();
    });
    connect(botonMenu, &QPushButton::clicked, this, [=]() {
        this->close();
        //emit volverAMenu();
    });
}

void Entrenamiento::pantallaVictoria() {
    detenerTimersGlobales();

    QGraphicsRectItem* fondo = new QGraphicsRectItem(0, 0, 1080, 720);
    fondo->setBrush(Qt::black);
    fondo->setZValue(10);
    escenaEntrenamiento->addItem(fondo);

    QGraphicsTextItem* mensaje = new QGraphicsTextItem("¡Nivel completado!");
    mensaje->setDefaultTextColor(Qt::white);
    mensaje->setFont(QFont("Arial", 28, QFont::Bold));
    mensaje->setPos(350, 250);
    mensaje->setZValue(11);
    escenaEntrenamiento->addItem(mensaje);

    QPushButton* botonReiniciar = new QPushButton("Volver a jugar", this);
    botonReiniciar->setGeometry(450, 300, 180, 40);
    botonReiniciar->raise();
    botonReiniciar->show();

    QPushButton* botonMenu = new QPushButton("Volver al Menú", this);
    botonMenu->setGeometry(450, 360, 180, 40);
    botonMenu->raise();
    botonMenu->show();

    QPushButton* botonNext = new QPushButton("Siguiente nivel", this);
    botonNext->setGeometry(450, 420, 180, 40);
    botonNext->raise();
    botonNext->show();

    connect(botonReiniciar, &QPushButton::clicked, this, [=]() {
        emit volverASeleccionar();
        this->close();
    });
    connect(botonMenu, &QPushButton::clicked, this, [=]() {
        this->close();
        //emit volverAMenu();
    });
    connect(botonNext, &QPushButton::clicked, this, [=]() {
        int vidasRestantes = personajeActual->getContadorVidas();
        int piedrasAcumuladas = personajeActual->getContadorPiedras();
        Entrenamiento* siguienteNivel = new Entrenamiento(personaje, vidasRestantes, piedrasAcumuladas);
        siguienteNivel->show();
        this->close();
    });

    ui->pausa->hide();
}

void Entrenamiento::detenerTimersGlobales()
{
    if (personajeActual) {
        personajeActual->desactivarTimers();
        personajeActual->setEnPausa(true);
    }

    if (timerPiedras) timerPiedras->stop();
    for (Piedras* p : piedras) {
        if (p) p->detener();
    }

    if (timerRocas)timerRocas->stop();
    for (Objetos* r : rocas) {
        if(r) r->detener();
    }
}

void Entrenamiento::reanudarTimersGlobales()
{
    if (personajeActual) {
        personajeActual->reanudarAnimacion();
        personajeActual->setEnPausa(false);
    }

    if (timerPiedras) timerPiedras->start(5000);
    for (Piedras* p : piedras) {
        if (p) p->reanudar();
    }

    if (timerRocas)timerRocas->start(2000);
    for (Objetos* r : rocas) {
        if(r) r->reanudar();
    }
}

