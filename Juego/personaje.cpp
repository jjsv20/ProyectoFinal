#include "personaje.h"

Personaje::Personaje(QObject *parent)
    : QObject{parent}, QGraphicsPixmapItem()
{
    contadorVidas = 5;
    vidasMaximas = 5;

    sonidoSalto.setSource(QUrl("qrc:/sonido/saltoGoku.wav"));
    sonidoAterrizaje.setSource(QUrl("qrc:/sonido/aterrizar.wav"));
    sonidoGolpeRecibidoGoku.setSource(QUrl("qrc:/sonido/golpeRgoku.wav"));
    sonidoPremio.setSource(QUrl("qrc:/sonido/premio.wav"));
    sonidoGolpeRecibidoRoshi.setSource(QUrl("qrc:/sonido/golpeRoshi.wav"));
    sonidoGolpeRecibidoKrilin.setSource(QUrl("qrc:/sonido/golpeRecibidoKrilin.wav"));
    muerteGoku.setSource(QUrl("qrc:/sonido/muertegoku.wav"));
    muerteRoshi.setSource(QUrl("qrc:/sonido/muerteRoshi.wav"));
    muerteKrilin.setSource(QUrl("qrc:/sonido/muerteKrilin.wav"));
    saltoKrilin.setSource(QUrl("qrc:/sonido/salyoKrilin.wav"));
    victoriaGoku.setSource(QUrl("qrc:/sonido/victoriaGoku.wav"));
    victoriaKrilin.setSource(QUrl("qrc:/sonido/victoriaKrilin.wav"));
    victoriaRoshi.setSource(QUrl("qrc:/sonido/victoriaRoshi.wav"));

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

void Personaje::inciarBarraVida(QGraphicsScene *escena, int x, int y, bool invertida)
{
    escenaActual = escena;
    posicionBarraX = x;
    posicionBarraY = y;
    barraInvertida = invertida;

    if (barraVida) {
        escena->removeItem(barraVida);
        delete barraVida;
        barraVida = nullptr;
    }

    barraVida = escena->addRect(x, y, 200, 20, QPen(Qt::black), QBrush(Qt::green));
    barraVida->setZValue(1);

    actualizarBarraVida();
}

void Personaje::actualizarBarraVida()
{
    if (!barraVida || vidasMaximas <= 0) return;

    qreal porcentaje = static_cast<qreal>(contadorVidas) / vidasMaximas;
    qreal nuevaAncho = 200 * porcentaje;

    if (barraInvertida) {
        qreal nuevoX = posicionBarraX + (200 - nuevaAncho);
        barraVida->setRect(nuevoX, posicionBarraY, nuevaAncho, 20);
    } else {
        barraVida->setRect(posicionBarraX, posicionBarraY, nuevaAncho, 20);
    }

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

void Personaje::setRival(Personaje *r)
{
    objetivo = r;
}

Personaje *Personaje::getRival() const
{
    return objetivo;
}

void Personaje::reaccionGolpe()
{

}

void Personaje::spriteVictoria()
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
