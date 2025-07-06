#include "personaje.h"

Personaje::Personaje(QObject *parent)
    : QObject{parent}, QGraphicsPixmapItem()
{
    contadorVidas = 5;
    vidasMaximas = 5;

    sonidoSalto.setSource(QUrl("qrc:/sonido/saltoGoku.wav"));
    sonidoAterrizaje.setSource(QUrl("qrc:/sonido/aterrizar.wav"));
    sonidoGolpeRecibido.setSource(QUrl("qrc:/sonido/golpeRgoku.wav"));
    sonidoPremio.setSource(QUrl("qrc:/sonido/premio.wav"));
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
    actualizarBarraVida();
}

void Personaje::setContadorPiedras(int piedras) {
    contadorPiedras = piedras;
    if (puntos) {
        puntos->setPlainText("x " + QString::number(contadorPiedras) + "/10");
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

void Personaje::setVidasMaximas(int max)
{
    vidasMaximas = max;
}

int Personaje::getVidasMaximas() const
{
    return vidasMaximas;
}

void Personaje::inciarBarraVida(QGraphicsScene *escena)
{
    escenaActual = escena;
    barraVida = escena->addRect(70, 15, 200, 20, QPen(Qt::black), QBrush(Qt::green));
    barraVida->setZValue(0);
    actualizarBarraVida();
}

void Personaje::actualizarBarraVida()
{
    if (!barraVida || vidasMaximas <= 0) return;

    qreal porcentaje = static_cast<qreal>(contadorVidas) / vidasMaximas;
    barraVida->setRect(70, 15, 200 * porcentaje, 20);

    if (porcentaje > 0.5)
        barraVida->setBrush(QBrush(Qt::green));
    else if (porcentaje > 0.25)
        barraVida->setBrush(QBrush(Qt::yellow));
    else
        barraVida->setBrush(QBrush(Qt::red));
}

void Personaje::eliminarBarraVida()
{
    if (escenaActual && barraVida) {
        escenaActual->removeItem(barraVida);
        delete barraVida;
        barraVida = nullptr;
    }
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
    if(getEstaMuerto()) return;
    int vidasActuales = getContadorVidas();
    setEstaMuerto(true);

    if (contadorVidas > 0) {
        --contadorVidas;
        setContadorVidas(vidasActuales - 1);
        emit vida(getContadorVidas(), vidasMaximas);
    }
    if (contadorVidas == 0) {
        estaMuerto = true;
        emit finalPartida();
    }

    QTimer::singleShot(1500, this, [=]() {
        setEstaMuerto(false);
    });
}

void Personaje::desactivarTimers()
{

}

void Personaje::reanudarAnimacion()
{

}
