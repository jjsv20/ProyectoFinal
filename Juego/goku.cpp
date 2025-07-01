#include "goku.h"

Goku::Goku(QObject *parent)
    : Personaje{parent}
{
    coordenadaX = 0;
    coordenadaY = 0;
    ancho = 66;
    alto = 100;

    pixmap = new QPixmap(":/imagenes/movimientosGoku.png");
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
    setPos(100, 380);

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

void Goku::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
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
        //setPos(x() + 10, y());
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
    coordenadaX = frameActual * ancho;
    setPixmap(pixmap->copy(coordenadaX, coordenadaY, ancho, alto));
    setX(x() + velocidadX);
    setY(y() + velocidadY);
    velocidadY += gravedad;
    const qreal alturaSuelo = 592 - boundingRect().height();  // donde aterriza

    if (y() >= alturaSuelo) {
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
        if(Piedras *p = dynamic_cast<Piedras*>(i)){
            if(this->collidesWithItem(p)){
                qDebug() << "Colision piedra";
                scene()->removeItem(p);
                contadorPiedras += 1;
                qDebug() << "Piedras recolectadas:" << contadorPiedras;
                if(puntos) {
                    puntos->setPlainText("x " + QString::number(contadorPiedras));
                }
            }
            if (contadorPiedras >= 4 && !getNivelCompletado()) {
                //setNivelCompletado(true);
                QTimer::singleShot(1500, this, [=]() {
                    qDebug() << "Emit partidaCompletada por recolectar piedras";
                    qDebug() << "Nivel completado:" << getNivelCompletado();
                    emit partidaCompletada();
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
        if(Objetos *p = dynamic_cast<Objetos*>(i)){
            if(this->collidesWithItem(p)){
                qDebug() << "Colision roca";
                moveBy(-20, 0);
                perderVida();
                break;
            }
        }
    }
}

void Goku::perderVida()
{
    if (getEstaMuerto()) return;
    int vidasActuales = getContadorVidas();
    setEstaMuerto(true);
    if (vidasActuales > 0) {
        setContadorVidas(vidasActuales - 1);
        qDebug() << "Perdió una vida. Vidas restantes:" << getContadorVidas();

        if (vidasT) {
            vidasT->setPlainText("Vidas: " + QString::number(getContadorVidas()));
        }
    }
    if (getContadorVidas() == 0) {
        qDebug() << "¡Game Over!";
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
}

void Goku::reanudarAnimacion()
{
    iniciarAnimacion();
    timerColision->start(50);
}

void Goku::reiniciar()
{
    setEstaMuerto(false);
    iniciarAnimacion();
    actualizar();
    volverASeleccion();
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
}

void Goku::detenerAnimacion()
{
    seleccion->stop();
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
            caminar->stop();
            velocidadY = -40;
            saltando = true;
            saltar->start(30);
        }
    }
}
