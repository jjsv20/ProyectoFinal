#include "seleccion.h"
#include "ui_seleccion.h"

#include <QPixmap>

Seleccion::Seleccion(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Seleccion)
{
    ui->setupUi(this);

    QPixmap krilin(":/imagenes/Krillin.png");
    int ancho = 32;
    int alto = 32;
    int fila = 0;
    int columna = 0;

    QPixmap krilinSeleccion = krilin.copy(columna * ancho, fila * alto, ancho, alto);
    ui->labelKrilin->setPixmap(krilinSeleccion.scaled(64, 64, Qt::KeepAspectRatio));
}

Seleccion::~Seleccion()
{
    delete ui;
}

void Seleccion::on_regresar_clicked()
{
    emit regresarMenu();
    this->close();
}

