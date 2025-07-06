#include "personaje.h"

Personaje::Personaje(QObject *parent)
    : QObject{parent}, QGraphicsPixmapItem()
{
    contadorVidas = 5;
}

void Personaje::setPuntos(QGraphicsTextItem* texto) {
    puntos = texto;
}

void Personaje::setVidas(QGraphicsTextItem* texto) {
    vidasT = texto;
}

void Personaje::setContadorVidas(int vidas) {
    contadorVidas = vidas;
    if (vidasT) {
        vidasT->setPlainText("Vidas: " + QString::number(contadorVidas));
    }
}

void Personaje::setContadorPiedras(int piedras) {
    contadorPiedras = piedras;
    if (puntos) {
        puntos->setPlainText("x " + QString::number(contadorPiedras));
    }
}

int Personaje::getContadorVidas() const {
    return contadorVidas;
}

int Personaje::getContadorPiedras() const {
    return contadorPiedras;
}

bool Personaje::getEnPausa() const {
    return enPausa;
}

void Personaje::setEnPausa(bool estado) {
    enPausa = estado;
}

bool Personaje::getNivelCompletado() const {
    return nivelCompletado;
}

void Personaje::setNivelCompletado(bool completado) {
    nivelCompletado = completado;
}

bool Personaje::getEstaMuerto() const {
    return estaMuerto;
}

void Personaje::animarPuno()
{

}

void Personaje::animarPatada()
{

}

void Personaje::setEstaMuerto(bool muerto) {
    estaMuerto = muerto;
}

void Personaje::iniciarAnimacion() {
}

void Personaje::detenerAnimacion() {
}

void Personaje::reiniciar() {
}

void Personaje::perderVida() {
    if (contadorVidas > 0) {
        --contadorVidas;
        setContadorVidas(contadorVidas);
        if (contadorVidas == 0) {
            estaMuerto = true;
            emit finalPartida();
        }
    }
}

void Personaje::desactivarTimers()
{

}

void Personaje::reanudarAnimacion()
{

}
