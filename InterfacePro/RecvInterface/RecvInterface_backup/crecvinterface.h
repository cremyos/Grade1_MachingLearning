#ifndef CRECVINTERFACE_H
#define CRECVINTERFACE_H

#include <QWidget>
#include <QtNetwork>
#include <iostream>
#include "ck_means.h"
#include <fstream>

using namespace std;

#define RECVPORT 8090

typedef unsigned short WORD;
typedef unsigned char BYTE;

typedef union {
    WORD wData;
    BYTE bData[2];
}UN_BYTE_WORD;

typedef union  {
    BYTE bData[8];
    double dData;
}UN_BYTE_DOUBLE;



namespace Ui {
class CRecvInterface;
}

class CRecvInterface : public QWidget
{
    Q_OBJECT

public:
    explicit CRecvInterface(QWidget *parent = 0);
    ~CRecvInterface();
    void ReadFileData();

private:
    Ui::CRecvInterface *ui;
    QUdpSocket *UdpRecv;
    CK_means k_means;
    ifstream infile;
    int i32RecvIndex;

private slots:
    void RecvData();
};

#endif // CRECVINTERFACE_H
