#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
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
    ventanaSeleccion = new Seleccion(this);
    connect(ventanaSeleccion, &Seleccion::regresarMenu, this, &Widget::show);
    ventanaSeleccion->setAttribute(Qt::WA_DeleteOnClose);
    ventanaSeleccion->show();
    this->hide(); // Oculta el menú
}


void Widget::on_creditos_clicked()
{
    close();
}

