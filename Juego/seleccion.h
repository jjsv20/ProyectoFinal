#ifndef SELECCION_H
#define SELECCION_H

#include <QDialog>

namespace Ui {
class Seleccion;
}

class Seleccion : public QDialog
{
    Q_OBJECT

public:
    explicit Seleccion(QWidget *parent = nullptr);
    ~Seleccion();

signals:
    void regresarMenu();

private slots:
    void on_regresar_clicked();

private:
    Ui::Seleccion *ui;
    //Seleccion *ventanaSeleccion;
};

#endif // SELECCION_H
