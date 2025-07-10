#include <QApplication>
#include "widget.h"
#include "combate.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();/*/
    Combate *pantallaCombate = new Combate();
    pantallaCombate->show();
    pantallaCombate->iniciarCombate("Krilin");/*/

    return a.exec();
}
