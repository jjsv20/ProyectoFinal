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
}

void Combate::iniciarCombate(QString personajeSeleccionado)
{
    limpiaObjetos();
    qDebug() << "¡Combate iniciado!";

    personaje = personajeSeleccionado;
    //nivelActual = nivel;
    //contadorDerrotas = derrotas;

    if (rondaActual == 1) {
        rondasGanadasJugador = 0;
        rondasGanadasRoshi = 0;
    }
    if (rondaActual > 3) {
        rondaActual = 1;
    }

    QGraphicsPixmapItem* fondo = new QGraphicsPixmapItem(QPixmap(":/imagenes/combate.png"));
    escenaCombate->addItem(fondo);

    QGraphicsTextItem* textoRonda = escenaCombate->addText("Ronda " + QString::number(rondaActual), QFont("Arial", 28));
    textoRonda->setDefaultTextColor(Qt::white);
    textoRonda->setPos(480, 10);
    textoRonda->setZValue(15);
    QTimer::singleShot(2000, this, [=]() {
        escenaCombate->removeItem(textoRonda);
        delete textoRonda;
    });

    QPixmap avatarPixmap;
    if(personaje == "Goku"){
        avatarPixmap.load(":/imagenes/avatarGoku.png");
    }else if(personaje == "Krilin"){
        avatarPixmap.load(":/imagenes/avatarKrilin.png");
    }

    avatar = escenaCombate->addPixmap(avatarPixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    avatar->setPos(20, 10);
    avatar->setZValue(10);

    if (personaje == "Goku") {
        personajeJugador = new Goku();
        personajeJugador->setScale(1.4);
        qDebug() << "Goku creado para combate";
    } else if (personaje == "Krilin") {
        personajeJugador = new Krilin();
        personajeJugador->setScale(1.2);
        qDebug() << "Krilin creado para combate";
    }

    if (!personajeJugador) {
        qDebug() << "ERRo";
        return;
    }

    roshi = new Roshi();
    roshi->setPos(800, 592);
    roshi->setScale(1.5);
    roshi->setVidasMaximas(20);
    roshi->setContadorVidas(20);
    roshi->inciarBarraVida(escenaCombate, 750, 15);
    escenaCombate->addItem(roshi);

    personajeJugador->setPos(100, 592);
    personajeJugador->setFlag(QGraphicsItem::ItemIsFocusable);
    personajeJugador->setFocus();
    personajeJugador->iniciarAnimacion();
    personajeJugador->setEstaMuerto(false);
    personajeJugador->setVidasMaximas(20);
    personajeJugador->setContadorVidas(20);
    personajeJugador->inciarBarraVida(escenaCombate, 70, 15);

    escenaCombate->addItem(personajeJugador);
    personajeJugador->setRival(roshi);
    roshi->setRival(personajeJugador);

    personajeJugador->reanudarAnimacion();
    roshi->reanudarAnimacion();

    tiempo = 50;
    textoTiempo = escenaCombate->addText("Tiempo: 50", QFont("Arial", 24));
    textoTiempo->setDefaultTextColor(Qt::red);
    textoTiempo->setPos(900, 20);

    timerTiempo = new QTimer(this);
    connect(timerTiempo, &QTimer::timeout, this, &Combate::cuentaRegresiva);
    timerTiempo->start(1000);

    connect(personajeJugador, &Personaje::finalPartida, this, &Combate::pantallaDerrota);
    connect(roshi, &Personaje::finalPartida, this, &Combate::pantallaVictoria);
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
        if(personajeJugador->getContadorVidas() > roshi->getContadorVidas()){
            pantallaVictoria();
        }else if(roshi->getContadorVidas() > personajeJugador->getContadorVidas()){
            pantallaDerrota();
        }else{
            QGraphicsTextItem* empate = escenaCombate->addText("EMPATE", QFont("Arial", 28));
            empate->setDefaultTextColor(Qt::yellow);
            empate->setPos(480, 300);
            empate->setZValue(15);

            QTimer::singleShot(3000, this, [=]() {
                escenaCombate->removeItem(empate);
                delete empate;
                rondaActual++;
                iniciarCombate(personaje);
            });
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
        personajeJugador->eliminarBarraVida();
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
    //detenerTimersGlobales();
    rondasGanadasRoshi++;

    derrotaPantalla = new QGraphicsRectItem(0, 0, 1080, 720);
    derrotaPantalla->setBrush(QColor(0, 0, 0, 150));
    derrotaPantalla->setZValue(20);
    escenaCombate->addItem(derrotaPantalla);

    derrotaTexto = new QGraphicsTextItem("PERDISTE EL ROUND");
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

        rondaActual++;

        if(rondaActual <= 3){
            iniciarCombate(personaje);
        }else{
            if(rondasGanadasJugador > rondasGanadasRoshi){
                mensajeFinal("GANASTE");
            }else if(rondasGanadasJugador < rondasGanadasRoshi){
                mensajeFinal("PERDISTE");
            }else{
                mensajeFinal("EMPATE");
            }
        }
    });
}

void Combate::pantallaVictoria()
{
    //detenerTimersGlobales();
    rondasGanadasJugador++;

    victoriaPantalla = new QGraphicsRectItem(0, 0, 1080, 720);
    victoriaPantalla->setBrush(QColor(0, 0, 0, 150));
    victoriaPantalla->setZValue(20);
    escenaCombate->addItem(victoriaPantalla);

    victoriaTexto = new QGraphicsTextItem("GANASTE EL ROUND");
    victoriaTexto->setDefaultTextColor(Qt::white);
    victoriaTexto->setFont(QFont("Arial", 32, QFont::Bold));
    victoriaTexto->setZValue(21);
    victoriaTexto->setPos(450, 200);
    escenaCombate->addItem(victoriaTexto);

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

        rondaActual++;

        if(rondaActual <= 3){
            iniciarCombate(personaje);
        }else{
            if(rondasGanadasJugador > rondasGanadasRoshi){
                mensajeFinal("GANASTE");
            }else if(rondasGanadasJugador < rondasGanadasRoshi){
                mensajeFinal("PERDISTE");
            }else{
                mensajeFinal("EMPATE");
            }
        }

    });

}

void Combate::mensajeFinal(QString mensaje)
{
    QGraphicsRectItem* fondo = new QGraphicsRectItem(0, 0, 1080, 720);
    fondo->setBrush(QColor(0, 0, 0, 150));
    fondo->setZValue(20);
    escenaCombate->addItem(fondo);

    QGraphicsTextItem* texto = escenaCombate->addText(mensaje, QFont("Arial", 32, QFont::Bold));
    texto->setDefaultTextColor(Qt::white);
    texto->setZValue(21);
    texto->setPos(350, 300);
    escenaCombate->addItem(texto);

    QTimer::singleShot(3000, this, [=]() {
        escenaCombate->removeItem(fondo);
        escenaCombate->removeItem(texto);
        delete fondo;
        delete texto;
        limpiaObjetos();
        emit combateTerminado(); // vuelve al menú principal
    });
}
