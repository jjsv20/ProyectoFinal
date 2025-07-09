#include "goku.h"

Goku::Goku(QObject *parent)
    : Personaje{parent}
{
    coordenadaX = 0;
    coordenadaY = 0;
    ancho = 70;
    alto = 100;

    pixmap = new QPixmap(":/imagenes/1-Goku.png");
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
    setPos(100, 380);

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    frameActual = 0;
    totalFramesDerecha = 6;

    seleccion = new QTimer(this);
    connect(seleccion, &QTimer::timeout, this, &Goku::actualizar);

    caminar = new QTimer(this);
    connect(caminar, &QTimer::timeout, this, &Goku::animar);

    saltar = new QTimer(this);
    connect(saltar, &QTimer::timeout, this, &Goku::animarSalto);

    timerColision = new QTimer(this);
    connect(timerColision, &QTimer::timeout, this, &Goku::colisionPiedras);
    connect(timerColision, &QTimer::timeout, this, &Goku::colisionRocas);
    timerColision->start(50);

    timerPuno = new QTimer(this);
    connect(timerPuno, &QTimer::timeout, this, &Goku::animarPuno);

    timerPatada = new QTimer(this);
    connect(timerPatada, &QTimer::timeout, this, &Goku::animarPatada);

    moviendoDerecha = false;
    moviendoIzquierda = false;
    saltando = false;

    gravedad = 5;
    velocidadY = 0;
    velocidadX = 0;

}

QRectF Goku::boundingRect() const {
    return QRectF(0, 0, ancho, alto);
}

void Goku::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->drawPixmap(0, 0, *pixmap, coordenadaX, coordenadaY, ancho, alto);
}

void Goku::animar()
{
    if(saltando) return;

    if (moviendoDerecha) {
        coordenadaY = 100;
        frameActual = (frameActual + 1) % totalFramesDerecha;
        coordenadaX = frameActual * ancho;
        setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
        if(x() > 10)
            setPos(x() + 10, y());
        emit moverFondoSignal(20);
    } else if (moviendoIzquierda) {
        coordenadaY = 200;
        frameActual = (frameActual + 1) % 8;
        coordenadaX = frameActual * ancho;
        setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
        if (x() > 10)
            setPos(x() - 10, y());
        //emit moverFondoSignal(-10);
    }
}

void Goku::animarSalto()
{
    coordenadaY = 300;
    frameActual = (frameActual + 1) % 9;
    coordenadaX = frameActual * 70;
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, 70, alto));
    setX(x() + velocidadX);
    setY(y() + velocidadY);
    velocidadY += gravedad;
    //sonidoSalto.play();
    const qreal alturaSuelo = 592 - boundingRect().height();  // donde aterriza

    if (y() >= alturaSuelo) {
        sonidoAterrizaje.play();
        setY(alturaSuelo);
        velocidadY = 0;
        velocidadX = 0;
        saltando = false;
        saltar->stop();
        volverASeleccion();
        qDebug() << "Aterrizó en el suelo";
        return;
    }

}

void Goku::actualizar()
{
    coordenadaX += ancho;
    if(coordenadaX >= 300){
        coordenadaX = 0;
    }
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
}

void Goku::colisionPiedras()
{
    const auto items = scene()->items();
    for(auto i : items){
        Objetos *p = dynamic_cast<Objetos*>(i);
        if(p && p->getTipo() == "piedra"){
            if(this->collidesWithItem(p)){
                sonidoPremio.play();
                qDebug() << "Colision piedra";
                scene()->removeItem(p);
                contadorPiedras += 1;
                qDebug() << "Piedras recolectadas:" << contadorPiedras;
                if(puntos) {
                    puntos->setPlainText("x " + QString::number(contadorPiedras) + "/10");
                }
            }
            if (contadorPiedras == 10 && !getNivelCompletado()) {
                QTimer::singleShot(1500, this, [=]() {
                    desactivarTimers();
                    frameActual = 0;
                    QTimer* animacion = new QTimer(this);
                    connect(animacion, &QTimer::timeout, this, [=]() mutable {
                        spriteVictoria();
                        if (frameActual >= 7) {
                            animacion->stop();
                            animacion->deleteLater();
                            emit partidaCompletada();
                            qDebug() << "Emit partidaCompletada por recolectar piedras";
                        }
                    });
                    animacion->start(100);
                    emit partidaCompletada();
                    qDebug() << "Emit partidaCompletada por recolectar piedras";
                    qDebug() << "Nivel completado:" << this->nivelCompletado;
                });
            }
        }
    }
}

void Goku::colisionRocas()
{
    if (getEstaMuerto()) return;
    const auto items = scene()->items();
    for(auto i : items){
        Objetos *r = dynamic_cast<Objetos*>(i);
        if(r && r->getTipo() == "roca"){
            if(this->collidesWithItem(r)){
                sonidoGolpeRecibidoGoku.play();
                qDebug() << "Colision roca";
                perderVida();
                break;
            }
        }
    }
}

void Goku::animarPuno()
{
    if (!pixmap || !timerPuno) return;

    coordenadaY = 400;
    coordenadaX = frameActual * ancho;
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));

    if (!yaGolpeo) {
        const auto items = scene()->items();
        for (auto item : items) {
            Roshi* r = dynamic_cast<Roshi*>(item);
            if (r && this->collidesWithItem(r)) {
                r->reaccionGolpe();
                yaGolpeo = true;
                break;
            }
        }
    }

    frameActual++;

    if (frameActual >= 6) {
        timerPuno->stop();
        puno = false;
        frameActual = 0;
        volverASeleccion();
        yaGolpeo = false;
    }
}

void Goku::animarPatada()
{
    if (!pixmap || !timerPatada) return;

    coordenadaY = 600;
    coordenadaX = frameActual * ancho;
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));

    /*/if (!yaGolpeo) {
        const auto items = scene()->items();
        for (auto item : items) {
            Roshi* r = dynamic_cast<Roshi*>(item);
            if (r && this->collidesWithItem(r)) {
                r->reaccionGolpe();
                yaGolpeo = true;
                break;
            }
        }
    }/*/

    if (!yaGolpeo && getRival() && collidesWithItem(getRival())) {
        getRival()->reaccionGolpe();
        yaGolpeo = true;
        qDebug() << "Goku golpeó";
    }


    frameActual++;

    if (frameActual >= 6) {
        timerPatada->stop();
        patada = false;
        frameActual = 0;
        volverASeleccion();
        yaGolpeo = false;
    }
}

void Goku::reaccionGolpe()
{
    if (getEstaMuerto() || golpeRecibido) return;

    qDebug() << "goku recibio golep";
    golpeRecibido = true;
    sonidoGolpeRecibidoGoku.play();
    caminar->stop();

    coordenadaY = 500;
    coordenadaX = 0;
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
    setX(x() - 50);
    setOpacity(0.5);

    if (getContadorVidas() > 0) {
        setContadorVidas(getContadorVidas() - 1);
        actualizarBarraVida();
        qDebug() << "Vidas restantes: " << getContadorVidas();
    }

    if (getContadorVidas() <= 0) {
        setEstaMuerto(true);
        golpeRecibido = false;
        muerteGoku.play();
        detenerAnimacion();
        qDebug() << "Goku ha sido derrotado";
        emit finalPartida();
        return;
    }

    QTimer::singleShot(400, this, [this]() {
        setOpacity(1.0);
        golpeRecibido = false;
        if (moviendoDerecha || moviendoIzquierda) {
            caminar->start(40);
        } else {
            volverASeleccion();
        }
    });
}

void Goku::perderVida()
{
    if (getEstaMuerto()) return;

    int vidasActuales = getContadorVidas();
    setEstaMuerto(true);

    if (vidasActuales > 0) {
        setContadorVidas(vidasActuales - 1);
        qDebug() << "Perdió una vida. Vidas restantes:" << getContadorVidas();
    }

    if (getContadorVidas() == 0) {
        qDebug() << "¡Game Over!";
        muerteGoku.play();
        detenerAnimacion();
        emit finalPartida();
    }

    QTimer::singleShot(1500, this, [=]() {
        setEstaMuerto(false);
        qDebug() << "Ya puede volver a recibir daño.";
    });
}

void Goku::desactivarTimers()
{
    caminar->stop();
    saltar->stop();
    seleccion->stop();
    timerColision->stop();
    timerPuno->stop();
     timerPatada->stop();
}

void Goku::reanudarAnimacion()
{
    iniciarAnimacion();
    caminar->start(40);
    saltar->start(40);
    timerColision->start(50);
}

void Goku::spriteVictoria()
{
    coordenadaY = 700;
    coordenadaX = frameActual * ancho;
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
}


void Goku::reiniciar()
{
    setEstaMuerto(false);
    iniciarAnimacion();
    actualizar();
    volverASeleccion();
    puno = false;
    patada = false;
    saltando = false;
    moviendoDerecha = false;
    moviendoIzquierda = false;
    golpeRecibido = false;
    frameActual = 0;
}

void Goku::volverASeleccion()
{
    coordenadaX = 0;
    coordenadaY = 0;
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
}

void Goku::iniciarAnimacion()
{
    seleccion->start(120);
    timerColision->start(50);
    caminar->start(40);
}

void Goku::detenerAnimacion()
{
    seleccion->stop();
    timerColision->stop();
    caminar->stop();
}

void Goku::keyReleaseEvent(QKeyEvent *event)
{
    if (getEnPausa()) return;

    if ((event->key() == Qt::Key_Right && !event->isAutoRepeat() && moviendoDerecha) ||
        (event->key() == Qt::Key_Left && !event->isAutoRepeat() && moviendoIzquierda)){
        caminar->stop();
        moviendoDerecha = false;
        moviendoIzquierda = false;
        saltando =false;
        volverASeleccion();
    }
}

void Goku::keyPressEvent(QKeyEvent *event)
{
    if(getEnPausa()) return;
    if (puno || patada) return;

    if (event->key() == Qt::Key_Right && !event->isAutoRepeat()) {
        moviendoDerecha = true;
        moviendoIzquierda = false;
        if (saltando) {
            velocidadX = 5;
        }
        caminar->start(40);
    }else if(event->key() == Qt::Key_Left && !event->isAutoRepeat()){
        moviendoIzquierda = true;
        moviendoDerecha = false;
        if (saltando) {
            velocidadX = -10;
        }
        caminar->start(40);
    }else if (event->key() == Qt::Key_Space && !event->isAutoRepeat()) {
        if(!saltando){
            if (moviendoDerecha){
                velocidadX = 10;
            }else if (moviendoIzquierda){
                velocidadX = -10;
            }else {
                velocidadX = 0;
            }
            sonidoSalto.play();
            caminar->stop();
            velocidadY = -40;
            saltando = true;
            saltar->start(30);
        }
    }else if (event->key() == Qt::Key_Z && !event->isAutoRepeat()) {
        if (!patada && !puno && !saltando) {
            patada = true;
            frameActual = 0;
            timerPatada->start(60);
        }
    } else if (event->key() == Qt::Key_X && !event->isAutoRepeat()) {
        if (!puno && !patada && !saltando) {
            puno = true;
            frameActual = 0;
            timerPuno->start(60);
        }
    }
}
