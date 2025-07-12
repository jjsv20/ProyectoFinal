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
    ui->graphicsView->setFocusPolicy(Qt::NoFocus);
    ui->graphicsView->viewport()->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    personajeJugador = nullptr;
    roshi = nullptr;
    timerTiempo = nullptr;
    textoTiempo = nullptr;
    avatar = nullptr;

}



void Combate::iniciarCombate(QString personajeSeleccionado)
{
    limpiaObjetos();
    qDebug() << "¡Combate iniciado!";

    personaje = personajeSeleccionado;

    if (!tutorialMostrado) {
        iniciarCombateTuto();
    }else{
        iniciarNivel2(personajeSeleccionado);
    }
}

void Combate::iniciarNivel2(QString personajeSeleccionado)
{
    limpiaObjetos();

    personaje = personajeSeleccionado;

    if (rondaActual == 1) {
        rondasGanadasJugador = 0;
        rondasGanadasRoshi = 0;
    }
    if (rondaActual > 3) {
        rondaActual = 1;
    }

    QGraphicsPixmapItem* fondo = new QGraphicsPixmapItem(QPixmap(":/imagenes/combate-2.png"));
    escenaCombate->addItem(fondo);

    QGraphicsTextItem* textoRonda = escenaCombate->addText("ROUND " + QString::number(rondaActual), QFont("Arial", 28));
    textoRonda->setDefaultTextColor(Qt::black);
    textoRonda->setPos(470, 80);
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

    avatar = escenaCombate->addPixmap(avatarPixmap.scaled(50, 50));
    avatar->setPos(20, 10);
    avatar->setZValue(10);

    QPixmap avatarRoshiPixmap(":/imagenes/avatarJackie.png");
    avatarRoshi = escenaCombate->addPixmap(avatarRoshiPixmap.scaled(50, 50));
    avatarRoshi->setPos(1000, 10);
    avatarRoshi->setZValue(10);

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

    escenaCombate->addItem(personajeJugador);
    personajeJugador->setFlag(QGraphicsItem::ItemIsFocusable);
    personajeJugador->setFocus(Qt::ActiveWindowFocusReason);
    personajeJugador->setPos(100, 492);

    roshi = new Roshi();
    roshi->setPos(800, 492);
    roshi->setScale(1.5);

    roshi->reiniciar();
    personajeJugador->reiniciar();

    personajeJugador->setVidasMaximas(20);
    personajeJugador->setContadorVidas(20);
    roshi->setVidasMaximas(20);
    roshi->setContadorVidas(20);

    escenaCombate->addItem(roshi);

    roshi->inciarBarraVida(escenaCombate, 790, 15, true);
    personajeJugador->inciarBarraVida(escenaCombate, 70, 15, false);

    personajeJugador->setZValue(1);
    roshi->setZValue(1);

    personajeJugador->setRival(roshi);
    roshi->setRival(personajeJugador);

    connect(personajeJugador, &Personaje::finalPartida, this, &Combate::pantallaDerrota);
    connect(roshi, &Personaje::finalPartida, this, &Combate::pantallaVictoria);

    personajeJugador->setEnCombate(true);
    personajeJugador->iniciarAnimacion();
    personajeJugador->setEstaMuerto(false);
    roshi->iniciarRoshi();

    tiempo = 30;
    textoTiempo = escenaCombate->addText("Tiempo: 30", QFont("Arial", 24));
    textoTiempo->setDefaultTextColor(Qt::red);
    textoTiempo->setPos(460, 30);

    timerTiempo = new QTimer(this);
    connect(timerTiempo, &QTimer::timeout, this, &Combate::cuentaRegresiva);
    timerTiempo->start(1000);

    ui->pausa->show();
    ui->pausa->setEnabled(true);
}

void Combate::iniciarCombateTuto()
{
    ui->pausa->hide();
    detenerTimersGlobales();
    if (personajeJugador) {
        personajeJugador->clearFocus();
    }

    QGraphicsRectItem* fondoTuto = new QGraphicsRectItem(0, 0, 1080, 720);
    fondoTuto->setBrush(QColor(0, 0, 0, 150));
    fondoTuto->setZValue(20);
    escenaCombate->addItem(fondoTuto);

    QGraphicsTextItem* texto = new QGraphicsTextItem(
        "🥊 CONTROLES DE COMBATE\n"
        "➡️ Z: Patada\n"
        "➡️ X: Puño\n"
        "➡️ Barra espaciadora: Saltar\n"
        "\n"
        "🎯 OBJETIVO\n"
        "✅ Derrota al oponente\n"
        "❌ Evita quedarte sin vidas\n"
        );
    texto->setFont(QFont("Arial", 20));
    texto->setDefaultTextColor(Qt::white);
    QRectF rect = texto->boundingRect();
    texto->setPos((1080 - rect.width()) / 2, 130);
    texto->setZValue(101);
    escenaCombate->addItem(texto);

    QTimer::singleShot(7000, this, [=]() {
        if (fondoTuto && fondoTuto->scene())
            escenaCombate->removeItem(fondoTuto);
        delete fondoTuto;

        if (texto && texto->scene())
            escenaCombate->removeItem(texto);
        delete texto;

        tutorialMostrado = true;
        iniciarNivel2(personaje);

        QTimer::singleShot(100, this, [=]() {
            if (personajeJugador) {
                ui->graphicsView->setFocus();
                qDebug() << "Focus después del tutorial de combate: " << personajeJugador->hasFocus();
            }
        });
    });
}

Combate::~Combate()
{
    detenerTimersGlobales();
    limpiaObjetos();
    escenaCombate->clear();
    delete escenaCombate;
    delete ui;
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
            empate->setDefaultTextColor(Qt::black);
            empate->setPos(480, 300);
            empate->setZValue(15);

            QTimer::singleShot(3000, this, [=]() {
                escenaCombate->removeItem(empate);
                delete empate;
                rondaActual++;
                limpiaObjetos();
                iniciarCombate(personaje);
            });
        }
    }
}

void Combate::limpiaObjetos()
{
    detenerTimersGlobales();

    if (timerTiempo) {
        timerTiempo->stop();
        delete timerTiempo;
        timerTiempo = nullptr;
    }

    if (personajeJugador) {
        personajeJugador->desactivarTimers();
        escenaCombate->removeItem(personajeJugador);
        delete personajeJugador;
        personajeJugador = nullptr;
    }

    if (roshi) {
        roshi->desactivarTimers();
        escenaCombate->removeItem(roshi);
        delete roshi;
        roshi = nullptr;
    }

    escenaCombate->clear();

    textoTiempo = nullptr;
    avatar = nullptr;
    tiempo = 30;

    qDebug() << "Combate limpiado correctamente.";
}

void Combate::pantallaDerrota()
{
    detenerTimersGlobales();
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

        rondaActual++;

        if(rondaActual <= 3){
            limpiaObjetos();
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
    detenerTimersGlobales();
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

        rondaActual++;

        if(rondaActual <= 3){
            limpiaObjetos();
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

void Combate::detenerTimersGlobales()
{
    if (personajeJugador) {
        personajeJugador->desactivarTimers();
        personajeJugador->setEnPausa(true);
    }

    if (timerTiempo) {
        timerTiempo->stop();
    }

    if (roshi) {
        roshi->desactivarTimers();
    }
}

void Combate::reanudarTimersGlobales()
{
    if (personajeJugador) {
        personajeJugador->reanudarAnimacion();
        personajeJugador->setEnPausa(false);
        personajeJugador->setFocus(Qt::OtherFocusReason);
    }

    if (timerTiempo) {
        timerTiempo->start(1000);
    }

    if (roshi) {
        roshi->reanudarAnimacion();
    }
}

void Combate::on_pausa_clicked()
{
    ui->pausa->hide();
    detenerTimersGlobales();
    QGraphicsRectItem* panelPausa = new QGraphicsRectItem(0, 0, 1080, 720);
    panelPausa->setBrush(QColor(0, 0, 0, 150));
    panelPausa->setZValue(20);
    escenaCombate->addItem(panelPausa);

    QGraphicsTextItem* titulo = new QGraphicsTextItem("PAUSA");
    titulo->setDefaultTextColor(Qt::white);
    titulo->setFont(QFont("Arial", 32, QFont::Bold));
    titulo->setZValue(21);
    titulo->setPos(450, 200);
    escenaCombate->addItem(titulo);

    QPushButton* botonReanudar = new QPushButton("Reanudar", this);
    botonReanudar->setFocusPolicy(Qt::NoFocus);

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
        escenaCombate->removeItem(panelPausa);
        delete titulo;
        botonReanudar->deleteLater();
        botonMenu->deleteLater();
        botonSalir->deleteLater();
        reanudarTimersGlobales();
        ui->pausa->show();
        ui->pausa->setEnabled(true);
        QTimer::singleShot(100, this, [=]() {
            if (personajeJugador) {
                ui->graphicsView->setFocus();
                qDebug() << "Focus después de reanudar: " << personajeJugador->hasFocus();
            }
        });
    });
    connect(botonMenu, &QPushButton::clicked, this, [=]() {
        emit volverSeleccionar();
        QTimer::singleShot(0, this, &Entrenamiento::close);
    });
    connect(botonSalir, &QPushButton::clicked, this, []() {
        qApp->quit();
    });
}

