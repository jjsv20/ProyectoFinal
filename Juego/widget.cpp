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
}


Widget::~Widget()
{
    delete ui;
}

void Widget::mostrarSeleccion()
{
    ui->stackedWidget->setCurrentWidget(ui->page); // Volver a pantalla de selección
}

void Widget::mostrarMenu()
{
     ui->stackedWidget->setCurrentWidget(ui->paginaMenu);
}

void Widget::iniciarEntrenamiento(QString personaje, int vidas, int nivel)
{
    qDebug() << "Iniciando entrenamiento con" << personaje;
    entrenamiento->limpiaObjetos();
    entrenamiento->iniciarNivel1(personaje, vidas, nivel);
    ui->stackedWidget->setCurrentWidget(ui->paginaEntrenamiento);
}

void Widget::on_botonJugar_clicked()
{
    mostrarSeleccion();
}


void Widget::on_botonSalir_clicked()
{
    close();
}

