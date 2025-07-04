#include "entrenamiento.h"
#include "ui_entrenamiento.h"
#include "personaje.h"
#include "objetos.h"
#include "seleccion.h"

Entrenamiento::Entrenamiento(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Entrenamiento)
{
    ui->setupUi(this);
    escenaEntrenamiento = new QGraphicsScene(0, 0, 1080, 720, this);
    ui->graphicsView->setScene(escenaEntrenamiento);
    ui->graphicsView->setGeometry(this->rect());
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

Entrenamiento::~Entrenamiento()
{
    detenerTimersGlobales();
    limpiaObjetos();
    delete ui;
    qDebug() << "Entrenamiento destruido correctamente";
}

void Entrenamiento::iniciarNivel1(QString personajeSeleccionado, int vidasIniciales, int nivel)
{
    personaje = personajeSeleccionado;
    nivelActual = nivel;

    qDebug() << "Iniciando nivel" << nivelActual << "con personaje" << personaje;

    QPixmap fondo;
    if (nivelActual == 1) {
        fondo.load(":/imagenes/isla.png");
    } else {
        fondo.load(":/imagenes/fondoN1.png");
    }
    widthFondo = fondo.width();
    fondoE = escenaEntrenamiento->addPixmap(fondo);
    fondoE->setZValue(-1);
    fondoEE = escenaEntrenamiento->addPixmap(fondo);
    fondoEE->setPos(widthFondo, 0);
    fondoEE->setZValue(-1);

    if(personaje == "Goku"){
        personajeActual = new Goku();
        personajeActual->setScale(1.4);
        qDebug() << "Goku creado correctamente";
    } else if(personaje == "Krilin"){
        personajeActual = new Krilin();
        personajeActual->setScale(1.5);
        qDebug() << "Krilin creado correctamente";
    }

    personajeActual->setPos(100, 492);
    personajeActual->setContadorVidas(vidasIniciales);
    personajeActual->setContadorPiedras(0);
    personajeActual->setFlag(QGraphicsItem::ItemIsFocusable);
    personajeActual->setFocus();
    escenaEntrenamiento->addItem(personajeActual);
    personajeActual->iniciarAnimacion();
    personajeActual->setEstaMuerto(false);

    connect(personajeActual, &Personaje::moverFondoSignal, this, &Entrenamiento::moverFondo);
    connect(personajeActual, &Personaje::finalPartida, this, &Entrenamiento::pantallaDerrota);
    connect(personajeActual, &Personaje::partidaCompletada, this, &Entrenamiento::pantallaVictoria);

    texto = escenaEntrenamiento->addText("x 0", QFont("Arial", 24));
    texto->setDefaultTextColor(Qt::yellow);
    texto->setPos(30, 20);
    personajeActual->setPuntos(texto);

    vidasT = escenaEntrenamiento->addText("Vidas: 5", QFont("Arial", 24));
    vidasT->setDefaultTextColor(Qt::yellow);
    vidasT->setPos(50, 50);
    personajeActual->setVidas(vidasT);

    timerPiedras = new QTimer(this);
    connect(timerPiedras, &QTimer::timeout, this, &Entrenamiento::crearPiedras);
    timerPiedras->start(5000);

    timerRocas = new QTimer(this);
    connect(timerRocas, &QTimer::timeout, this, &Entrenamiento::crearRocas);
    timerRocas->start(2000);
}

void Entrenamiento::on_pausa_clicked()
{
    ui->pausa->setEnabled(false);
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
        botonReanudar->deleteLater();
        botonMenu->deleteLater();
        botonSalir->deleteLater();
        reanudarTimersGlobales();
    });
    connect(botonMenu, &QPushButton::clicked, this, [=]() {
        emit volverSeleccionar();
        QTimer::singleShot(0, this, &Entrenamiento::close);
    });
    connect(botonSalir, &QPushButton::clicked, this, []() {
        qApp->quit();
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
        Objetos *p = new Objetos("piedra", this);
        escenaEntrenamiento->addItem(p);
        piedras.append(p);

        connect(p, &Objetos::eliminarPiedra, this, [=](Objetos *piedra){
            piedras.removeOne(piedra);
        });
    }
}

void Entrenamiento::crearRocas()
{
    if(rocas.count() < 4){
        Objetos *r = new Objetos("roca", this);
        escenaEntrenamiento->addItem(r);
        rocas.append(r);

        connect(r, &Objetos::eliminarRoca, this, [=](Objetos *roca){
            rocas.removeOne(roca);
        });
    }
}

void Entrenamiento::pantallaDerrota() {
    ui->pausa->setEnabled(false);
    detenerTimersGlobales();
    derrotaPantalla = new QGraphicsRectItem(0, 0, 1080, 720);
    derrotaPantalla->setBrush(QColor(0, 0, 0, 150));
    derrotaPantalla->setZValue(20);
    escenaEntrenamiento->addItem(derrotaPantalla);

    derrotaTexto = new QGraphicsTextItem("PERDISTE");
    derrotaTexto->setDefaultTextColor(Qt::white);
    derrotaTexto->setFont(QFont("Arial", 32, QFont::Bold));
    derrotaTexto->setZValue(21);
    derrotaTexto->setPos(450, 200);
    escenaEntrenamiento->addItem(derrotaTexto);

    botonReiniciar = new QPushButton("Reiniciar", this);
    botonReiniciar->setGeometry(450, 300, 180, 40);
    botonReiniciar->raise();
    botonReiniciar->show();

    botonMenu = new QPushButton("Volver al Menú", this);
    botonMenu->setGeometry(450, 360, 180, 40);
    botonMenu->raise();
    botonMenu->show();

    connect(botonReiniciar, &QPushButton::clicked, this, [=]() {
        if (derrotaPantalla && derrotaPantalla->scene())
            escenaEntrenamiento->removeItem(derrotaPantalla);
        delete derrotaPantalla;
        derrotaPantalla = nullptr;

        if (derrotaTexto && derrotaTexto->scene())
            escenaEntrenamiento->removeItem(derrotaTexto);
        delete derrotaTexto;
        derrotaTexto = nullptr;

        if (botonReiniciar) botonReiniciar->deleteLater();
        if (botonMenu) botonMenu->deleteLater();
        botonReiniciar = nullptr;
        botonMenu = nullptr;
        detenerTimersGlobales();
        limpiaObjetos();
        emit volverSeleccionar();
    });

    connect(botonMenu, &QPushButton::clicked, this, [=]() {
        if (derrotaPantalla && derrotaPantalla->scene())
            escenaEntrenamiento->removeItem(derrotaPantalla);
        delete derrotaPantalla;
        derrotaPantalla = nullptr;

        if (derrotaTexto && derrotaTexto->scene())
            escenaEntrenamiento->removeItem(derrotaTexto);
        delete derrotaTexto;
        derrotaTexto = nullptr;

        if (botonReiniciar) botonReiniciar->deleteLater();
        if (botonMenu) botonMenu->deleteLater();
        botonReiniciar = nullptr;
        botonMenu = nullptr;
        detenerTimersGlobales();
        limpiaObjetos();
        emit volverSeleccionar();
    });
}

void Entrenamiento::pantallaVictoria() {
    ui->pausa->setEnabled(false);
    detenerTimersGlobales();

    victoriaPantalla = new QGraphicsRectItem(0, 0, 1080, 720);
    victoriaPantalla->setBrush(QColor(0, 0, 0, 150));
    victoriaPantalla->setZValue(20);
    escenaEntrenamiento->addItem(victoriaPantalla);

    victoriaTexto = new QGraphicsTextItem("GANASTE");
    victoriaTexto->setDefaultTextColor(Qt::white);
    victoriaTexto->setFont(QFont("Arial", 32, QFont::Bold));
    victoriaTexto->setZValue(21);
    victoriaTexto->setPos(450, 200);
    escenaEntrenamiento->addItem(victoriaTexto);

    botonContinuar = new QPushButton("Continuar", this);
    botonContinuar->setGeometry(450, 300, 180, 40);
    botonContinuar->raise();
    botonContinuar->show();

    botonMenu1 = new QPushButton("Volver al Menú", this);
    botonMenu1->setGeometry(450, 360, 180, 40);
    botonMenu1->raise();
    botonMenu1->show();

    connect(botonContinuar, &QPushButton::clicked, this, [=]() {
        qDebug() << "Siguiente nivel";

        if (nivelActual < 2) {
            if (victoriaPantalla && victoriaPantalla->scene())
                escenaEntrenamiento->removeItem(victoriaPantalla);
            delete victoriaPantalla;
            victoriaPantalla = nullptr;

            if (victoriaTexto && victoriaTexto->scene())
                escenaEntrenamiento->removeItem(victoriaTexto);
            delete victoriaTexto;
            victoriaTexto = nullptr;

            if (botonContinuar) botonContinuar->deleteLater();
            if (botonMenu1) botonMenu1->deleteLater();
            limpiaObjetos();
            //emit volverSeleccionar();
            iniciarNivel1(personaje, 5, nivelActual + 1);
        } else {
            if (victoriaPantalla && victoriaPantalla->scene())
                escenaEntrenamiento->removeItem(victoriaPantalla);
            delete victoriaPantalla;
            victoriaPantalla = nullptr;

            if (victoriaTexto && victoriaTexto->scene())
                escenaEntrenamiento->removeItem(victoriaTexto);
            delete victoriaTexto;
            victoriaTexto = nullptr;

            if (botonContinuar) { botonContinuar->deleteLater(); botonContinuar = nullptr; }
            if (botonMenu1) { botonMenu1->deleteLater(); botonMenu1 = nullptr; }
            limpiaObjetos();
            emit entrenamientoTerminado();
        }
    });

    connect(botonMenu1, &QPushButton::clicked, this, [=]() {
        if (victoriaPantalla && victoriaPantalla->scene())
            escenaEntrenamiento->removeItem(victoriaPantalla);
        delete victoriaPantalla;
        victoriaPantalla = nullptr;

        if (victoriaTexto && victoriaTexto->scene())
            escenaEntrenamiento->removeItem(victoriaTexto);
        delete victoriaTexto;
        victoriaTexto = nullptr;

        if (botonContinuar) botonContinuar->deleteLater();
        if (botonMenu1) botonMenu1->deleteLater();
        detenerTimersGlobales();
        limpiaObjetos();
        emit volverSeleccionar();
    });
}

void Entrenamiento::detenerTimersGlobales()
{
    if (personajeActual) {
        personajeActual->desactivarTimers();
        personajeActual->setEnPausa(true);
    }

    if (timerPiedras) timerPiedras->stop();
    for (Objetos* p : piedras) {
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
        personajeActual->setFocus(Qt::OtherFocusReason);
    }

    if (timerPiedras) timerPiedras->start(5000);
    for (Objetos* p : piedras) {
        if (p) p->reanudar();
    }

    if (timerRocas)timerRocas->start(2000);
    for (Objetos* r : rocas) {
        if(r) r->reanudar();
    }
}

void Entrenamiento::limpiaObjetos()
{
    for (Objetos* p : piedras) {
        if (p && p->scene()) {
            escenaEntrenamiento->removeItem(p);
            p->deleteLater();
        }
    }
    piedras.clear();

    for (Objetos* r : rocas) {
        if (r && r->scene()) {
            escenaEntrenamiento->removeItem(r);
            r->deleteLater();
        }
    }
    rocas.clear();

    if (personajeActual) {
        escenaEntrenamiento->removeItem(personajeActual);
        personajeActual->deleteLater();
        personajeActual = nullptr;
    }

    if (texto) {
        escenaEntrenamiento->removeItem(texto);
        delete texto;
        texto = nullptr;
    }
    if (vidasT) {
        escenaEntrenamiento->removeItem(vidasT);
        delete vidasT;
        vidasT = nullptr;
    }

    if (fondoE) {
        escenaEntrenamiento->removeItem(fondoE);
        delete fondoE;
        fondoE = nullptr;
    }
    if (fondoEE) {
        escenaEntrenamiento->removeItem(fondoEE);
        delete fondoEE;
        fondoEE = nullptr;
    }
}
