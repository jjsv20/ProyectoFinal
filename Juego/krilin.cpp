#include "krilin.h"

Krilin::Krilin(QObject *parent)
    : Personaje{parent}
{
    coordenadaX = 0;
    coordenadaY = 0;
    ancho = 100;
    alto = 100;
    pixmap = new QPixmap(":/imagenes/2-Krilin.png");
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
    setPos(750, 380);
    frameActual = 0;
    totalFramesDerecha = 8;

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    seleccion = new QTimer(this);
    connect(seleccion, &QTimer::timeout, this, &Krilin::actualizar);

    caminar = new QTimer(this);
    connect(caminar, &QTimer::timeout, this, &Krilin::animar);

    saltar = new QTimer(this);
    connect(saltar, &QTimer::timeout, this, &Krilin::animarSalto);

    timerColision = new QTimer(this);
    connect(timerColision, &QTimer::timeout, this, &Krilin::colisionPiedras);
    connect(timerColision, &QTimer::timeout, this, &Krilin::colisionRocas);
    timerColision->start(50);

    timerPuno = new QTimer(this);
    connect(timerPuno, &QTimer::timeout, this, &Krilin::animarPuno);

    timerPatada = new QTimer(this);
    connect(timerPatada, &QTimer::timeout, this, &Krilin::animarPatada);

    moviendoDerecha = false;
    moviendoIzquierda = false;
    saltando = false;

    gravedad = 5;
    velocidadY = 0;
    velocidadX = 0;
}

QRectF Krilin::boundingRect() const {
    return QRectF(0, 0, ancho, alto);
}

void Krilin::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->drawPixmap(0, 0, *pixmap, coordenadaX, coordenadaY, ancho, alto);
}

void Krilin::animar()
{
    if(saltando) return;

    if (moviendoDerecha) {
        coordenadaY = 100;
        frameActual = (frameActual + 1) % totalFramesDerecha;
        coordenadaX = frameActual * ancho;
        setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
        //setPos(x() + 10, y());
        emit moverFondoSignal(20);
    } else if (moviendoIzquierda) {
        coordenadaY = 300;
        frameActual = (frameActual + 1) % 8;
        coordenadaX = frameActual * ancho;
        setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
        if (x() > 10)
            setPos(x() - 10, y());
        //emit moverFondoSignal(-10);
    }
}

void Krilin::animarSalto()
{
    coordenadaY = 200;
    frameActual = (frameActual + 1) % 9;
    coordenadaX = frameActual * ancho;
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
    setX(x() + velocidadX);
    setY(y() + velocidadY);
    velocidadY += gravedad;
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

void Krilin::actualizar()
{
    coordenadaX += ancho;
    if(coordenadaX >= 300){
        coordenadaX = 0;
    }
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
}

void Krilin::colisionPiedras()
{
    const auto items = scene()->items();
    for(auto i : items){
        Objetos *r = dynamic_cast<Objetos*>(i);
        if(r && r->getTipo() == "piedra"){
            if(this->collidesWithItem(r)){
                sonidoPremio.play();
                qDebug() << "Colision piedra";
                scene()->removeItem(r);
                contadorPiedras += 1;
                qDebug() << "Piedras recolectadas:" << contadorPiedras;
                if(puntos) {
                    puntos->setPlainText("x " + QString::number(contadorPiedras) + "/10");
                }
            }
            if (contadorPiedras == 10 && !getNivelCompletado()) {
                QTimer::singleShot(1500, this, [=]() {
                    desactivarTimers();
                    frameActual = 7;
                    QTimer* animacion = new QTimer(this);
                    connect(animacion, &QTimer::timeout, this, [=]() mutable {
                        spriteVictoria();
                        frameActual++;
                        if (frameActual >= 9) {
                            victoriaKrilin.play();
                            animacion->stop();
                            animacion->deleteLater();
                            nivelCompletado = true;
                            emit partidaCompletada();
                            qDebug() << "Emit partidaCompletada por recolectar piedras";
                        }
                    });
                    animacion->start(100);
                    qDebug() << "Emit partidaCompletada por recolectar piedras";
                    qDebug() << "Nivel completado:" << this->nivelCompletado;
                });
            }
        }
    }
}

void Krilin::colisionRocas()
{
    if (getEstaMuerto()) return;
    const auto items = scene()->items();
    for(auto i : items){
        Objetos *r = dynamic_cast<Objetos*>(i);
        if(r && r->getTipo() == "roca"){
            if(this->collidesWithItem(r)){
                sonidoGolpeRecibidoKrilin.play();
                qDebug() << "Colision roca";
                perderVida();
                break;
            }
        }
    }
    for(auto i : items){
        Objetos *a = dynamic_cast<Objetos*>(i);
        if(a && a->getTipo() == "ave"){
            if(this->collidesWithItem(a)){
                sonidoGolpeRecibidoKrilin.play();
                qDebug() << "Colision ave";
                perderVida();
                break;
            }
        }
    }
}

void Krilin::animarPuno()
{
    if (!pixmap || !timerPuno) return;

    coordenadaY = 500;
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

void Krilin::reaccionGolpe()
{
    if (getEstaMuerto() || golpeRecibido) return;

    qDebug() << "Krilin recibio golep";
    golpeRecibido = true;
    sonidoGolpeRecibidoKrilin.play();
    caminar->stop();

    coordenadaY = 600;
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
        qDebug() << "Krilin ha sido derrotado";
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

void Krilin::animarPatada()
{
    if (!pixmap || !timerPatada) return;

    coordenadaY = 400;
    coordenadaX = frameActual * ancho;
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));

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

void Krilin::perderVida()
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
        muerteKrilin.play();
        detenerAnimacion();
        emit finalPartida();
    }

    QTimer::singleShot(1500, this, [=]() {
        setEstaMuerto(false);
        qDebug() << "Ya puede volver a recibir daño.";
    });
}

void Krilin::desactivarTimers()
{
    caminar->stop();
    saltar->stop();
    seleccion->stop();
    timerColision->stop();
    timerPuno->stop();
    timerPatada->stop();
}

void Krilin::reanudarAnimacion()
{
    iniciarAnimacion();
    caminar->start(40);
    saltar->start(40);
    timerColision->start(50);
}

void Krilin::spriteVictoria()
{
    coordenadaY = 600;
    coordenadaX = frameActual * ancho;
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
    update();
}

void Krilin::reiniciar()
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

void Krilin::volverASeleccion()
{
    coordenadaX = 0;
    coordenadaY = 0;
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
}

void Krilin::iniciarAnimacion()
{
    seleccion->start(120);
    timerColision->start(50);
    caminar->start(40);
}

void Krilin::detenerAnimacion()
{
    seleccion->stop();
    timerColision->stop();
    caminar->stop();
}

void Krilin::keyReleaseEvent(QKeyEvent *event)
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

void Krilin::keyPressEvent(QKeyEvent *event)
{
    if(getEnPausa()) return;

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
            saltoKrilin.play();
            caminar->stop();
            velocidadY = -40;
            saltando = true;
            saltar->start(40);
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
