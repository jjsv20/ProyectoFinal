#ifndef PIEDRAS_H
#define PIEDRAS_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

class Piedras : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Piedras(QObject *parent = nullptr);

    void detener();
    void reanudar();

signals:
    void eliminarLista(Piedras *piedras);

public slots:
    void moverPiedra();


private:
    QTimer *timer;
};

#endif // PIEDRAS_H
