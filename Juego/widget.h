#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include "seleccion.h"
#include "entrenamiento.h"

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
    void on_salir_clicked();

    void on_jugar_clicked();

    void on_creditos_clicked();

    void iniciarEntrenamiento(QString personaje);

private:
    Ui::Widget *ui;
    Seleccion *ventanaSeleccion;
    Entrenamiento *entrenamiento;
};
#endif // WIDGET_H
