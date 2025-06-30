#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>

class Personaje : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Personaje(QObject *parent = nullptr);
    virtual void iniciarAnimacion();
    virtual void detenerAnimacion();
    virtual void reiniciar();
    virtual void perderVida();

    virtual void desactivarTimers();
    virtual void reanudarAnimacion();

    virtual void setPuntos(QGraphicsTextItem* texto);
    virtual void setVidas(QGraphicsTextItem* texto);
    virtual void setContadorVidas(int vidas);
    virtual void setContadorPiedras(int piedras);
    virtual int getContadorVidas() const;
    virtual int getContadorPiedras() const;

    virtual void setEnPausa(bool estado);
    virtual bool getEnPausa() const;

    virtual void setNivelCompletado(bool estado);
    virtual bool getNivelCompletado() const;

    virtual void setEstaMuerto(bool estado);
    virtual bool getEstaMuerto() const;

signals:
    void partidaCompletada();
    void finalPartida();
    void moverFondoSignal(int dx);
    void reiniciarPartida();

protected:
    int contadorVidas;
    int contadorPiedras;
    QGraphicsTextItem* puntos;
    QGraphicsTextItem* vidasT;

    bool enPausa = false;
    bool nivelCompletado = false;
    bool estaMuerto = false;
};

#endif // PERSONAJE_H
