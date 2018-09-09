#include "cshowdata.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CShowData w;
    w.show();

    return a.exec();
}
