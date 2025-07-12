
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
    ui->graphicsView->setFocusPolicy(Qt::NoFocus);
    ui->graphicsView->viewport()->setAttribute(Qt::WA_TransparentForMouseEvents, true);


    texto = nullptr;
    vidasT = nullptr;
    fondoE = nullptr;
    fondoEE = nullptr;
    textoTiempo = nullptr;
    timerTiempo = nullptr;
    personajeActual = nullptr;
    timerPiedras = nullptr;
    timerRocas = nullptr;
    timerAves = nullptr;
    derrotaPantalla = nullptr;
    derrotaTexto = nullptr;
    botonReiniciar = nullptr;
    botonMenu = nullptr;
    victoriaPantalla = nullptr;
    victoriaTexto = nullptr;
    botonContinuar = nullptr;
    botonMenu1 = nullptr;
}

Entrenamiento::~Entrenamiento()
{
    detenerTimersGlobales();
    limpiaObjetos();
    escenaEntrenamiento->clear();
    delete escenaEntrenamiento;
    delete ui;
    qDebug() << "Entrenamiento destruido correctamente";
}

void Entrenamiento::iniciarNivel1(QString personajeSeleccionado, int vidasIniciales, int nivel, int derrotas)
{
    limpiaObjetos();

    personaje = personajeSeleccionado;
    nivelActual = nivel;
    contadorDerrotas = derrotas;

    qDebug() << "Personaje recibido en entrenamiento:" << personajeSeleccionado;
    qDebug() << "Iniciando nivel" << nivelActual << "con personaje" << personaje;

    QPixmap fondo;
    fondo.load(":/imagenes/entreno.png");
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
        personajeActual->setScale(1.1);
        qDebug() << "Krilin creado correctamente";
    }

    if (!personajeActual) {
        qDebug() << "No se pudo crear personaje";
        return;
    }

    escenaEntrenamiento->addItem(personajeActual);
    personajeActual->setFlag(QGraphicsItem::ItemIsFocusable);
    personajeActual->setFocus(Qt::ActiveWindowFocusReason);
    //setFocus(Qt::ActiveWindowFocusReason);


    QPixmap piedraPixmap(":/imagenes/piedra.png");
    piedraItem = escenaEntrenamiento->addPixmap(piedraPixmap.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    piedraItem->setPos(350, 20);

    texto = escenaEntrenamiento->addText("x 0/10", QFont("Arial", 20));
    texto->setDefaultTextColor(Qt::black);
    texto->setPos(390, 15);
    personajeActual->setPuntos(texto);

    QPixmap avatarPixmap;

    if(personaje == "Goku"){
        avatarPixmap.load(":/imagenes/avatarGoku.png");
    }else if(personaje == "Krilin"){
        avatarPixmap.load(":/imagenes/avatarKrilin.png");
    }

    avatar = escenaEntrenamiento->addPixmap(avatarPixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    avatar->setPos(20, 10);
    avatar->setZValue(10);

    personajeActual->setPos(100, 492);
    personajeActual->setVidasMaximas(10);
    personajeActual->setContadorVidas(10);
    personajeActual->inciarBarraVida(escenaEntrenamiento, 70, 15, false);
    personajeActual->setContadorPiedras(0);
    personajeActual->iniciarAnimacion();
    personajeActual->setEstaMuerto(false);

    connect(personajeActual, &Personaje::moverFondoSignal, this, &Entrenamiento::moverFondo);
    connect(personajeActual, &Personaje::finalPartida, this, &Entrenamiento::pantallaDerrota);
    connect(personajeActual, &Personaje::partidaCompletada, this, &Entrenamiento::pantallaVictoria);

    timerPiedras = new QTimer(this);
    connect(timerPiedras, &QTimer::timeout, this, &Entrenamiento::crearPiedras);
    timerPiedras->start(5000);

    if(nivel == 1){
        timerRocas = new QTimer(this);
        connect(timerRocas, &QTimer::timeout, this, &Entrenamiento::crearRocas);
        timerRocas->start(2000);
    }else if(nivel == 2){
        timerAves = new QTimer(this);
        connect(timerAves, &QTimer::timeout, this, &Entrenamiento::crearAves);
        timerAves->start(1000);
    }

    tiempo = 60;
    textoTiempo = escenaEntrenamiento->addText("Tiempo: 60", QFont("Arial", 24));
    textoTiempo->setDefaultTextColor(Qt::red);
    textoTiempo->setPos(900, 20);

    timerTiempo = new QTimer(this);
    connect(timerTiempo, &QTimer::timeout, this, &Entrenamiento::cuentaRegresiva);
    timerTiempo->start(1000);

    ui->pausa->show();
    ui->pausa->setEnabled(true);
}

void Entrenamiento::iniciarNivelTuto(QString personajeSeleccionado, int nivel)
{
    limpiaObjetos();

    personaje = personajeSeleccionado;
    nivelActual = nivel;
    contadorDerrotas = 0;

    if(!tutorialMostrado){
        mostrarTutorial();
    }else{
        iniciarNivel1(personaje, 10, nivelActual, contadorDerrotas);
    }
}

void Entrenamiento::mostrarTutorial()
{
    detenerTimersGlobales();
    if(personajeActual){
        personajeActual->clearFocus();
    }

    QGraphicsRectItem* fondoTuto = new QGraphicsRectItem(0, 0, 1080, 720);
    fondoTuto->setBrush(QColor(0, 0, 0, 150));
    fondoTuto->setZValue(20);
    escenaEntrenamiento->addItem(fondoTuto);

    QGraphicsTextItem* texto = new QGraphicsTextItem(
        "🕹️ CONTROLES\n"
        "→ Flechas izquierda/derecha para moverte\n"
        "↑ Flecha arriba para saltar\n"
        "\n"
        "🎯 OBJETIVO\n"
        "✅ Recolecta 10 piedras\n"
        "⏱️ Tienes 60 segundos\n"
        "\n"
        "⚠️ CUIDADO\n"
        "🪨 Esquiva las rocas que caen\n"
        "🦅 Esquiva las aves que vuelan\n"
        "\n");
    texto->setFont(QFont("Arial", 20));
    texto->setDefaultTextColor(Qt::white);
    QRectF rect = texto->boundingRect();
    texto->setPos((1080 - rect.width()) / 2, 130);
    texto->setZValue(101);
    escenaEntrenamiento->addItem(texto);

    QTimer::singleShot(8000, this, [=]() {
        if (fondoTuto && fondoTuto->scene())
            escenaEntrenamiento->removeItem(fondoTuto);
        delete fondoTuto;

        if (texto && texto->scene())
            escenaEntrenamiento->removeItem(texto);
        delete texto;

         tutorialMostrado = true;

        iniciarNivel1(personaje, 10, nivelActual, contadorDerrotas);

        QTimer::singleShot(100, this, [=]() {
            if (personajeActual) {
                ui->graphicsView->setFocus();
                qDebug() << "Focus después del tutorial: " << personajeActual->hasFocus();
            }
        });
    });
}

void Entrenamiento::cuentaRegresiva()
{
    if (!personajeActual) {
        return;
    }

    tiempo--;
    if (textoTiempo)
        textoTiempo->setPlainText("Tiempo: " + QString::number(tiempo));
    if (tiempo <= 0) {
        timerTiempo->stop();
        if (nivelActual == 1 && personajeActual->getContadorPiedras() < 4) {
            qDebug() << "Tiempo agotado. No se recolectaron suficientes piedras.";
            pantallaDerrota();
        } else {
            //qDebug() << "Tiempo agotado, pero recolectó suficientes piedras.";
            pantallaDerrota();
        }
    }
}

void Entrenamiento::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        on_pausa_clicked();
    }
    QWidget::keyPressEvent(event);
}

void Entrenamiento::on_pausa_clicked()
{
    ui->pausa->hide();
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
        escenaEntrenamiento->removeItem(panelPausa);
        delete titulo;
        botonReanudar->deleteLater();
        botonMenu->deleteLater();
        botonSalir->deleteLater();
        reanudarTimersGlobales();
        ui->pausa->show();
        ui->pausa->setEnabled(true);
        QTimer::singleShot(100, this, [=]() {
            if (personajeActual) {
                ui->graphicsView->setFocus();
                qDebug() << "Focus después del tutorial: " << personajeActual->hasFocus();
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
    if(rocas.count() < 10){
        Objetos *r = new Objetos("roca", this);
        escenaEntrenamiento->addItem(r);
        rocas.append(r);

        connect(r, &Objetos::eliminarRoca, this, [=](Objetos *roca){
            rocas.removeOne(roca);
        });
    }
}

void Entrenamiento::crearAves()
{
    if(aves.count() < 3){
        Objetos *a = new Objetos("ave", this);
        escenaEntrenamiento->addItem(a);
        aves.append(a);

        connect(a, &Objetos::eliminarAve, this, [=](Objetos *ave){
            aves.removeOne(ave);
        });
    }
}

void Entrenamiento::pantallaDerrota() {
    ui->pausa->hide();
    detenerTimersGlobales();

    derrotaPantalla = new QGraphicsRectItem(0, 0, 1080, 720);
    derrotaPantalla->setBrush(QColor(0, 0, 0, 150));
    derrotaPantalla->setZValue(20);
    escenaEntrenamiento->addItem(derrotaPantalla);

    contadorDerrotas++;

    derrotaTexto = new QGraphicsTextItem("PERDISTE");
    derrotaTexto->setDefaultTextColor(Qt::white);
    derrotaTexto->setFont(QFont("Arial", 32, QFont::Bold));
    derrotaTexto->setZValue(21);
    derrotaTexto->setPos(450, 200);
    escenaEntrenamiento->addItem(derrotaTexto);

    QTimer::singleShot(3000, this, [=]() {
        if (derrotaPantalla && derrotaPantalla->scene())
            escenaEntrenamiento->removeItem(derrotaPantalla);
        delete derrotaPantalla;
        derrotaPantalla = nullptr;

        if (derrotaTexto && derrotaTexto->scene())
            escenaEntrenamiento->removeItem(derrotaTexto);
        delete derrotaTexto;
        derrotaTexto = nullptr;

        if (contadorDerrotas < 3) {
            limpiaObjetos();
            iniciarNivel1(personaje, 5, nivelActual, contadorDerrotas);
        } else {
            limpiaObjetos();
            emit volverSeleccionar(); // vuelve al menú
        }
    });   
}

void Entrenamiento::pantallaVictoria() {
    ui->pausa->hide();
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

    int vidasRestantes = personajeActual->getContadorVidas();
    int derrotas = contadorDerrotas;

    QTimer::singleShot(3000, this, [=]() {
        if (victoriaPantalla && victoriaPantalla->scene())
            escenaEntrenamiento->removeItem(victoriaPantalla);
        delete victoriaPantalla;
        victoriaPantalla = nullptr;

        if (victoriaTexto && victoriaTexto->scene())
            escenaEntrenamiento->removeItem(victoriaTexto);
        delete victoriaTexto;
        victoriaTexto = nullptr;
        limpiaObjetos();

        if(nivelActual == 1){
            iniciarNivel1(personaje, vidasRestantes, 2, derrotas);
        }else {
            emit entrenamientoTerminado(personaje, vidasRestantes, nivelActual, derrotas);
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
    for (Objetos* p : piedras) {
        if (p) p->detener();
    }

    if (timerRocas)timerRocas->stop();
    for (Objetos* r : rocas) {
        if(r) r->detener();
    }

    if (timerAves)timerAves->stop();
    for (Objetos* a : aves) {
        if(a) a->detener();
    }

    if (timerTiempo) {
        timerTiempo->stop();
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

    if (timerAves)timerAves->start(2000);
    for (Objetos* a : aves) {
        if(a) a->reanudar();
    }

    if (timerTiempo) timerTiempo->start(1000);
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

    for (Objetos* a : aves) {
        if (a && a->scene()) {
            escenaEntrenamiento->removeItem(a);
            a->deleteLater();
        }
    }
    aves.clear();

    if (personajeActual) {
        personajeActual->desactivarTimers();
        personajeActual->eliminarBarraVida();
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

    if (timerTiempo) {
        timerTiempo->stop();
        timerTiempo->deleteLater();
        timerTiempo = nullptr;
    }
    if (textoTiempo) {
        escenaEntrenamiento->removeItem(textoTiempo);
        delete textoTiempo;
        textoTiempo = nullptr;
    }
}
