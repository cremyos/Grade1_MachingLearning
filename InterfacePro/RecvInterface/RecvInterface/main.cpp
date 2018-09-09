#include "crecvinterface.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CRecvInterface w;
    w.show();

    return a.exec();
}
