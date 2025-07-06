#include "combate.h"
#include "ui_combate.h"

Combate::Combate(QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::Combate)
{
    ui->setupUi(this);


    escenaCombate = new QGraphicsScene(this);
    escenaCombate->setSceneRect(0, 0, 1080, 720);
    ui->graphicsView->setScene(escenaCombate);
    ui->graphicsView->setGeometry(this->rect());
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    personajeJugador = nullptr;
    roshi = nullptr;
    timerTiempo = nullptr;
    textoTiempo = nullptr;
    barraFondo = nullptr;
    barraVida = nullptr;

}

void Combate::iniciarCombate(QString personajeSeleccionado, int nivel, int derrotas)
{
    limpiaObjetos();
    qDebug() << "¡Combate iniciado!";

    personaje = personajeSeleccionado;
    nivelActual = nivel;
    contadorDerrotas = derrotas;

    QGraphicsPixmapItem* fondo = new QGraphicsPixmapItem(QPixmap(":/imagenes/combate.png"));
    escenaCombate->addItem(fondo);

    roshi = new Roshi();
    roshi->setPos(800, 492);
    roshi->setScale(2.0);
    escenaCombate->addItem(roshi);

    if (personaje == "Goku") {
        personajeJugador = new Goku();
        personajeJugador->setScale(1.4);
        qDebug() << "Goku creado para combate";
    } else if (personaje == "Krilin") {
        personajeJugador = new Krilin();
        personajeJugador->setScale(1.5);
        qDebug() << "Krilin creado para combate";
    }

    if (!personajeJugador) {
        qDebug() << "ERRo";
        return;
    }

    personajeJugador->setPos(100, 492);
    personajeJugador->setFlag(QGraphicsItem::ItemIsFocusable);
    personajeJugador->setFocus();
    //personajeActual->iniciarAnimacion();
    if (personajeJugador) personajeJugador->iniciarAnimacion();
    personajeJugador->setEstaMuerto(false);

    escenaCombate->addItem(personajeJugador);

    roshi->setRival(personajeJugador);

    tiempo = 30;
    textoTiempo = escenaCombate->addText("Tiempo: 30", QFont("Arial", 24));
    textoTiempo->setDefaultTextColor(Qt::red);
    textoTiempo->setPos(900, 20);

    timerTiempo = new QTimer(this);
    connect(timerTiempo, &QTimer::timeout, this, &Combate::cuentaRegresiva);
    timerTiempo->start(1000);

    barraVida = escenaCombate->addRect(30, 20, 200, 20, QPen(Qt::black), QBrush(Qt::green));

}

void Combate::cuentaRegresiva()
{
    if (!personajeJugador) {
        return;
    }

    tiempo--;
    if (textoTiempo)
        textoTiempo->setPlainText("Tiempo: " + QString::number(tiempo));
    if (tiempo <= 0) {
        timerTiempo->stop();
        if (roshi->getContadorVidas() > 0) {
            pantallaDerrota();
        } else {
            pantallaVictoria();
        }
    }
}

void Combate::limpiaObjetos()
{
    if (timerTiempo) {
        timerTiempo->stop();
        delete timerTiempo;
        timerTiempo = nullptr;
    }

    if (textoTiempo && escenaCombate) {
        escenaCombate->removeItem(textoTiempo);
        delete textoTiempo;
        textoTiempo = nullptr;
    }

    if (personajeJugador && escenaCombate) {
        escenaCombate->removeItem(personajeJugador);
        delete personajeJugador;
        personajeJugador = nullptr;
    }

    if (roshi && escenaCombate) {
        escenaCombate->removeItem(roshi);
        delete roshi;
        roshi = nullptr;
    }

    if (escenaCombate) {
        escenaCombate->clear();
    }

    tiempo = 30;

    qDebug() << "Combate limpiado correctamente.";
}

Combate::~Combate()
{
    delete ui;
}

void Combate::pantallaDerrota()
{
    ui->pausa->hide();
    //detenerTimersGlobales();

    derrotaPantalla = new QGraphicsRectItem(0, 0, 1080, 720);
    derrotaPantalla->setBrush(QColor(0, 0, 0, 150));
    derrotaPantalla->setZValue(20);
    escenaCombate->addItem(derrotaPantalla);

    contadorDerrotas++;

    derrotaTexto = new QGraphicsTextItem("PERDISTE");
    derrotaTexto->setDefaultTextColor(Qt::white);
    derrotaTexto->setFont(QFont("Arial", 32, QFont::Bold));
    derrotaTexto->setZValue(21);
    derrotaTexto->setPos(450, 200);
    escenaCombate->addItem(derrotaTexto);

    QTimer::singleShot(3000, this, [=]() {
        if (derrotaPantalla && derrotaPantalla->scene())
            escenaCombate->removeItem(derrotaPantalla);
        delete derrotaPantalla;
        derrotaPantalla = nullptr;

        if (derrotaTexto && derrotaTexto->scene())
            escenaCombate->removeItem(derrotaTexto);
        delete derrotaTexto;
        derrotaTexto = nullptr;

        limpiaObjetos();
        emit combateTerminado(); // vuelve al menú
    });
}

void Combate::pantallaVictoria()
{
    ui->pausa->hide();
    //detenerTimersGlobales();

    victoriaPantalla = new QGraphicsRectItem(0, 0, 1080, 720);
    victoriaPantalla->setBrush(QColor(0, 0, 0, 150));
    victoriaPantalla->setZValue(20);
    escenaCombate->addItem(victoriaPantalla);

    victoriaTexto = new QGraphicsTextItem("GANASTE");
    victoriaTexto->setDefaultTextColor(Qt::white);
    victoriaTexto->setFont(QFont("Arial", 32, QFont::Bold));
    victoriaTexto->setZValue(21);
    victoriaTexto->setPos(450, 200);
    escenaCombate->addItem(victoriaTexto);

    //int vidasRestantes = personajeActual->getContadorVidas();
    //int derrotas = contadorDerrotas;

    QTimer::singleShot(3000, this, [=]() {
        if (victoriaPantalla && victoriaPantalla->scene())
            escenaCombate->removeItem(victoriaPantalla);
        delete victoriaPantalla;
        victoriaPantalla = nullptr;

        if (victoriaTexto && victoriaTexto->scene())
            escenaCombate->removeItem(victoriaTexto);
        delete victoriaTexto;
        victoriaTexto = nullptr;
        limpiaObjetos();
        emit combateTerminado();
    });

}

void Combate::on_pausa_clicked()
{

}

