#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    musicaFondo = new QMediaPlayer(this);
    musicaFondo->setSource(QUrl("qrc:/musica/music.mp3"));
    volumen = new QAudioOutput(this);
    volumen->setVolume(0.4);
    musicaFondo->setAudioOutput(volumen);
    musicaFondo->setLoops(QMediaPlayer::Infinite);
    musicaFondo->play();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_salir_clicked()
{
    close();
}


void Widget::on_jugar_clicked()
{
    //qDebug() << "Botón Jugar presionado";
    ventanaSeleccion = new Seleccion();
    connect(ventanaSeleccion, &Seleccion::regresarMenu, this, &Widget::show);
    connect(ventanaSeleccion, &Seleccion::personajeSeleccionado, this, &Widget::iniciarEntrenamiento);
    ventanaSeleccion->setAttribute(Qt::WA_DeleteOnClose);
    ventanaSeleccion->show();
    this->hide(); // Oculta el menú
}


void Widget::on_creditos_clicked()
{
    close();
}

void Widget::iniciarEntrenamiento(QString personaje)
{
    entrenamiento = new Entrenamiento(personaje);
    entrenamiento->show();
}

