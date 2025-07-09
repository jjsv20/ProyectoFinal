#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QSoundEffect>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsRectItem>

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

    virtual void animarPuno();
    virtual void animarPatada();

    virtual void setVidasMaximas(int max);
    virtual int getVidasMaximas() const;

    virtual void actualizarBarraVida();
    virtual void inciarBarraVida(QGraphicsScene *escena, int x, int y);
    virtual void eliminarBarraVida();

    virtual void setRival(Personaje *r);
    Personaje *getRival()const;

    virtual void reaccionGolpe();

signals:
    void partidaCompletada();
    void finalPartida();
    void moverFondoSignal(int dx);
    void reiniciarPartida();
    void vida(int vidasRestantes, int vidasMaximas);

protected:
    int contadorVidas;
    int vidasMaximas;
    int contadorPiedras;
    QGraphicsTextItem* puntos;
    QGraphicsTextItem* vidasT = nullptr;

    bool enPausa = false;
    bool nivelCompletado = false;
    bool estaMuerto = false;

    QTimer *timerPuno;
    QTimer *timerPatada;
    bool puno = false;
    bool patada = false;

    QSoundEffect sonidoSalto;
    QSoundEffect sonidoAterrizaje;
    QSoundEffect sonidoGolpeRecibidoGoku, muerteGoku;
    QSoundEffect sonidoGolpeRecibidoKrilin;
    QSoundEffect sonidoPremio;
    QSoundEffect sonidoGolpeRecibidoRoshi;

    QGraphicsRectItem* barraVida = nullptr;
    QGraphicsScene* escenaActual = nullptr;
    Personaje *objetivo = nullptr;

    int posicionBarraX, posicionBarraY;
};

#endif // PERSONAJE_H
