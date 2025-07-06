#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "seleccion.h"
#include "entrenamiento.h"
#include "combate.h"
#include <QMediaPlayer>
#include <QUrl>
#include <QAudioOutput>
#include <QDebug>
#include <QSoundEffect>

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
    void iniciarEntrenamiento(QString personaje, int vidas, int nivel, int derrotas);
    void iniciarCombate(QString personaje, int nivel, int derrotas);
    void mostrarSeleccion();
    void mostrarMenu();
    void reiniciarSeleccion();

private:
    Ui::Widget *ui;
    Seleccion *paginaSeleccion;
    Entrenamiento *entrenamiento;
    Combate *combate;
    QMediaPlayer *musicaFondo;
    QAudioOutput *volumen;

    QSoundEffect sonidoJugar;

};
#endif // WIDGET_H
