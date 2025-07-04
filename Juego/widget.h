#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "seleccion.h"
#include "entrenamiento.h"
#include <QMediaPlayer>
#include <QUrl>
#include <QAudioOutput>
#include <QDebug>

class Seleccion;
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_botonJugar_clicked();
    void on_botonSalir_clicked();
    void iniciarEntrenamiento(QString personaje, int vidas, int nivel);
    void mostrarSeleccion();
    void mostrarMenu();

private:
    Ui::Widget *ui;
    Seleccion *paginaSeleccion;
    Entrenamiento *entrenamiento;
    QMediaPlayer *musicaFondo;
    QAudioOutput *volumen;
};
#endif // WIDGET_H
