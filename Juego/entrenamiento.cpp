#include "entrenamiento.h"
#include "ui_entrenamiento.h"
#include "personaje.h"
#include "objetos.h"
#include "seleccion.h"
#include <QRandomGenerator>
#include <QMessageBox>

Entrenamiento::Entrenamiento(QString personajeSeleccionado, int vidasIniciales, int piedrasIniciales, int nivel, int derrotas, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Entrenamiento)
    , personaje(personajeSeleccionado)
    , nivelActual(nivel)
    , derrotasNivel(derrotas)
{
    ui->setupUi(this);

    escenaEntrenamiento = new QGraphicsScene(0, 0, 1080, 720);
    ui->graphicsView->setScene(escenaEntrenamiento);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //QPixmap fondo(":/imagenes/isla.png");
    //QPixmap fondo(":/imagenes/fondoN1.png");

    QPixmap fondo;
    if (nivelActual == 1) {
        fondo.load(":/imagenes/isla.png");
    } else if (nivelActual >= 2) {
        fondo.load(":/imagenes/fondoN1.png");
    }
    widthFondo = fondo.width();
    fondoE = new QGraphicsPixmapItem(fondo);
    fondoE->setZValue(-1);
    escenaEntrenamiento->addItem(fondoE);

    fondoEE = new QGraphicsPixmapItem(fondo);
    fondoEE->setPos(widthFondo, 0);
    fondoEE->setZValue(-1);
    escenaEntrenamiento->addItem(fondoEE);

    if(personaje == "Goku"){
        personajeActual = new Goku();
        personajeActual->setScale(1.5);
    } else if(personaje == "Krilin"){
        personajeActual = new Krilin();
        personajeActual->setScale(1.0);
    }

    connect(personajeActual, &Personaje::finalPartida, this, &Entrenamiento::pantallaDerrota);
    connect(personajeActual, &Personaje::partidaCompletada, this, &Entrenamiento::pantallaVictoria);
    personajeActual->setPos(100, 492);
    personajeActual->setFlag(QGraphicsItem::ItemIsFocusable);
    personajeActual->setFocus();
    personajeActual->setContadorVidas(vidasIniciales);
    personajeActual->setContadorPiedras(0);
    escenaEntrenamiento->addItem(personajeActual);
    personajeActual->iniciarAnimacion();

    personajeActual->setNivelCompletado(false);
    personajeActual->setEstaMuerto(false);

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
}

Entrenamiento::~Entrenamiento()
{
    if (timerPiedras) timerPiedras->stop();
    if (timerRocas) timerRocas->stop();

    for (Piedras* p : piedras) {
        if (p) escenaEntrenamiento->removeItem(p);
    }

    for (Objetos* r : rocas) {
        if (r) escenaEntrenamiento->removeItem(r);
    }
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
    //personajeActual->setEstaMuerto(true);
    detenerTimersGlobales();

    QGraphicsRectItem* fondo = new QGraphicsRectItem(0, 0, 1080, 720);
    fondo->setBrush(Qt::black);
    fondo->setZValue(10);
    escenaEntrenamiento->addItem(fondo);

    QGraphicsTextItem* mensaje = new QGraphicsTextItem("GAME OVER");
    mensaje->setDefaultTextColor(Qt::white);
    mensaje->setFont(QFont("Arial", 28, QFont::Bold));
    mensaje->setPos(400, 250);
    mensaje->setZValue(11);
    escenaEntrenamiento->addItem(mensaje);
    ui->pausa->hide();
    QTimer::singleShot(2000, this, [=]() {
        escenaEntrenamiento->removeItem(fondo);
        escenaEntrenamiento->removeItem(mensaje);
        delete fondo;
        delete mensaje;
        derrotasNivel++;
        qDebug() << "Derrotas en nivel" << nivelActual << ":" << derrotasNivel;
        if (derrotasNivel < 3 && (nivelActual == 1 || nivelActual == 2)) {
            int vidasRestantes = 5;
            int piedrasRestantes = 0;
            reiniciarPartida(vidasRestantes, piedrasRestantes);
        } else {
            emit volverSeleccionar();
            this->close();
        }
    });
}

void Entrenamiento::pantallaVictoria() {
    if (personajeActual->getNivelCompletado()) return;
    personajeActual->setNivelCompletado(true);

    detenerTimersGlobales();

    QGraphicsRectItem* fondo = new QGraphicsRectItem(0, 0, 1080, 720);
    fondo->setBrush(Qt::black);
    fondo->setZValue(10);
    escenaEntrenamiento->addItem(fondo);

    QGraphicsTextItem* mensaje = new QGraphicsTextItem("Nivel: " + QString::number(nivelActual) + " completado");
    mensaje->setDefaultTextColor(Qt::white);
    mensaje->setFont(QFont("Arial", 28, QFont::Bold));
    mensaje->setPos(350, 250);
    mensaje->setZValue(11);
    escenaEntrenamiento->addItem(mensaje);

    ui->pausa->hide();

    QTimer::singleShot(2000, this, [=]() {
        escenaEntrenamiento->removeItem(fondo);
        escenaEntrenamiento->removeItem(mensaje);
        delete fondo;
        delete mensaje;
        int vidasRestantes = personajeActual->getContadorVidas();
        //int piedrasRestantes = 0;
        if (nivelActual < 2) {
            qDebug() << "Nivel actual cargado:" << nivelActual + 1;
            disconnect(personajeActual, nullptr, this, nullptr);
            Entrenamiento* siguienteNivel = new Entrenamiento(personaje, vidasRestantes, 0, nivelActual + 1, 0);
            siguienteNivel->show();
            this->close();
        } else {
            QGraphicsRectItem* fondo = new QGraphicsRectItem(0, 0, 1080, 720);
            fondo->setBrush(Qt::black);
            fondo->setZValue(10);
            escenaEntrenamiento->addItem(fondo);
            QGraphicsTextItem* mensajeFinal = new QGraphicsTextItem("ENTRENAMIENTO COMPLETADO");
            mensajeFinal->setDefaultTextColor(Qt::white);
            mensajeFinal->setFont(QFont("Arial", 30, QFont::Bold));
            mensajeFinal->setPos(250, 300);
            mensajeFinal->setZValue(12);
            escenaEntrenamiento->addItem(mensajeFinal);
            qDebug() << "Pasas a combate";
            QTimer::singleShot(3000, this, [=]() {
                escenaEntrenamiento->removeItem(fondo);
                escenaEntrenamiento->removeItem(mensajeFinal);
                delete fondo;
                delete mensajeFinal;
                this->close();
            });
        }
    });
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

void Entrenamiento::reiniciarPartida(int vidasr, int piedrasr)
{
    for (Piedras* p : piedras) {
        disconnect(p, nullptr, this, nullptr);
    }
    for (Piedras* p : piedras) {
        escenaEntrenamiento->removeItem(p);
        p->deleteLater();
    }
    piedras.clear();

    for (Objetos* r : rocas) {
        disconnect(r, nullptr, this, nullptr);
    }
    for (Objetos* r : rocas) {
        escenaEntrenamiento->removeItem(r);
        r->deleteLater();
    }
    rocas.clear();

    if (personajeActual) {
        personajeActual->desactivarTimers();
        disconnect(personajeActual, nullptr, this, nullptr);
        escenaEntrenamiento->removeItem(personajeActual);
        delete personajeActual;
        personajeActual = nullptr;
    }
    if(personaje == "Goku"){
        personajeActual = new Goku();
        personajeActual->setScale(1.5);
    } else if(personaje == "Krilin"){
        personajeActual = new Krilin();
        personajeActual->setScale(1.0);
    }

    personajeActual->setPos(100, 500);
    personajeActual->setFlag(QGraphicsItem::ItemIsFocusable);
    personajeActual->setFocus();
    personajeActual->setContadorVidas(vidasr);       // Reinicia vidas
    personajeActual->setContadorPiedras(piedrasr);     // Reinicia piedras
    personajeActual->setNivelCompletado(false);
    personajeActual->setEstaMuerto(false);
    escenaEntrenamiento->addItem(personajeActual);

    connect(personajeActual, &Personaje::moverFondoSignal, this, &Entrenamiento::moverFondo);
    connect(personajeActual, &Personaje::finalPartida, this, &Entrenamiento::pantallaDerrota);
    connect(personajeActual, &Personaje::partidaCompletada, this, &Entrenamiento::pantallaVictoria);

    QPixmap fondo;
    if (nivelActual == 1) {
        fondo.load(":/imagenes/isla.png");
    } else if (nivelActual == 2) {
        fondo.load(":/imagenes/fondoN1.png");
    }
    widthFondo = fondo.width();
    fondoE->setPixmap(fondo);
    fondoEE->setPixmap(fondo);
    fondoE->setX(0);
    fondoEE->setX(widthFondo);
    fondoE->setZValue(-1);
    fondoEE->setZValue(-1);

    if (vidasT) vidasT->setPlainText("Vidas: " + QString::number(vidasr));
    if (texto)  texto->setPlainText("x 0");
    personajeActual->setVidas(vidasT);
    personajeActual->setPuntos(texto);

    reanudarTimersGlobales();
}
