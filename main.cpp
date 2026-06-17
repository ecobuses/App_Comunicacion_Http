#include <QCoreApplication>
#include "hilo.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    hilo *h = new hilo();
    h->start();
    return QCoreApplication::exec();
}
