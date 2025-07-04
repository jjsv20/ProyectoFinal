#include "krilin.h"

Krilin::Krilin(QObject *parent)
    : Personaje{parent}
{
    coordenadaX = 0;
    coordenadaY = 0;
    ancho = 66;
    alto = 100;
    pixmap = new QPixmap(":/imagenes/movimientosKrilin.png");
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
        if(r && r->getTipo() == "roca"){
            if(this->collidesWithItem(r)){
                qDebug() << "Colision piedra";
                scene()->removeItem(r);
                contadorPiedras += 1;
                qDebug() << "Piedras recolectadas:" << contadorPiedras;
                if(puntos) {
                    puntos->setPlainText("x " + QString::number(contadorPiedras));
                }
            }
            if (contadorPiedras >= 4 && !getNivelCompletado()) {
                QTimer::singleShot(1500, this, [=]() {
                    emit partidaCompletada();
                    qDebug() << "Emit partidaCompletada por recolectar piedras";
                    qDebug() << "Nivel completado:" << this->nivelCompletado;
                    //emit partidaCompletada();
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
                qDebug() << "Colision roca";
                perderVida();
                break;
            }
        }
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

void Krilin::desactivarTimers()
{
    caminar->stop();
    saltar->stop();
    seleccion->stop();
    timerColision->stop();
}

void Krilin::reanudarAnimacion()
{
    iniciarAnimacion();
    timerColision->start(50);
}

void Krilin::reiniciar()
{
    setEstaMuerto(false);
    iniciarAnimacion();
    actualizar();
    volverASeleccion();
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
}

void Krilin::detenerAnimacion()
{
    seleccion->stop();
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
            caminar->stop();
            velocidadY = -40;
            saltando = true;
            saltar->start(30);
        }
    }
}
