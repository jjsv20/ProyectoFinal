#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , entrenamiento(nullptr)
{
    ui->setupUi(this);
    paginaSeleccion = qobject_cast<Seleccion*>(ui->page);
    entrenamiento = qobject_cast<Entrenamiento*>(ui->paginaEntrenamiento);
    combate = qobject_cast<Combate*>(ui->paginaCombate);
    ui->stackedWidget->setCurrentWidget(ui->paginaMenu);

    musicaFondo = new QMediaPlayer(this);
    musicaFondo->setSource(QUrl("qrc:/musica/music.mp3"));
    volumen = new QAudioOutput(this);
    volumen->setVolume(0.0);
    musicaFondo->setAudioOutput(volumen);
    musicaFondo->setLoops(QMediaPlayer::Infinite);
    musicaFondo->play();

    connect(paginaSeleccion, &Seleccion::regresarMenu, this, &Widget::mostrarMenu);
    connect(paginaSeleccion, &Seleccion::personajeSeleccionado, this, &Widget::iniciarEntrenamiento);
    connect(entrenamiento, &Entrenamiento::volverSeleccionar, this, &Widget::mostrarSeleccion);
    connect(entrenamiento, &Entrenamiento::entrenamientoTerminado, this, &Widget::iniciarCombate);
    connect(combate, &Combate::combateTerminado, this, &Widget::mostrarMenu);

    sonidoJugar.setSource(QUrl("qrc:/sonido/jugar.wav"));

}


Widget::~Widget()
{
    delete ui;
}

void Widget::mostrarSeleccion()
{
    volumen->setVolume(0.3);
    entrenamiento->limpiaObjetos();
    combate->limpiaObjetos();
    ui->stackedWidget->setCurrentWidget(ui->page); // Volver a pantalla de selección
}

void Widget::mostrarMenu()
{
    entrenamiento->limpiaObjetos();
    combate->limpiaObjetos();
    ui->stackedWidget->setCurrentWidget(ui->paginaMenu);
}

void Widget::reiniciarSeleccion()
{
    if (paginaSeleccion) {
        paginaSeleccion->reiniciar();
        ui->stackedWidget->setCurrentWidget(paginaSeleccion);
    }
}

void Widget::iniciarEntrenamiento(QString personaje, int vidas, int nivel, int derrotas)
{
    //entrenamiento = qobject_cast<Entrenamiento*>(ui->paginaEntrenamiento);
    volumen->setVolume(0.0);
    qDebug() << "Iniciando entrenamiento con" << personaje;
    entrenamiento->limpiaObjetos();
    entrenamiento->iniciarNivel1(personaje, 5, 1, 0);
    ui->stackedWidget->setCurrentWidget(ui->paginaEntrenamiento);
}

void Widget::iniciarCombate(QString personaje, int nivel, int derrotas)
{
    volumen->setVolume(0.0);
    qDebug() << "¡Entrando a la escena de combate!";
    combate->iniciarCombate(personaje, nivel, derrotas);
    ui->stackedWidget->setCurrentWidget(ui->paginaCombate);
}

void Widget::on_botonJugar_clicked()
{
    sonidoJugar.play();
    mostrarSeleccion();
}


void Widget::on_botonSalir_clicked()
{
    close();
}

