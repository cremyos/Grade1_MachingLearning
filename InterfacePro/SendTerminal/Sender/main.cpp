#include <QCoreApplication>
#include "cudpsender.h"
#include <string>
#include <iostream>
#include <fstream>
#include <QTime>

using namespace std;

ifstream infile;

void sleep(unsigned int msec)

{

    QTime dieTime = QTime::currentTime().addMSecs(msec);

    while( QTime::currentTime() < dieTime ) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

typedef unsigned short WORD;
typedef unsigned char BYTE;

typedef union {
    WORD wData;
    BYTE bData[2];
}UN_BYTE_WORD;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CUdpSender *udpSender;

    udpSender = new CUdpSender();

#if 1

    string input = "input_Data.txt";
    double x = 0.0;


    infile.open(input.c_str());
    if (!infile) {
        cout << "Unable to open input."<< endl;
    }

    char inputData[1024] = {0x0};

    inputData[0] = 0xAA;
    inputData[1] = 0xBB;

    int typeCode = 0;

    while(!infile.eof()) {

        infile >> typeCode >>x;

        UN_BYTE_WORD unByteWord;

        unByteWord.wData = typeCode;

        memcpy(inputData + 2, unByteWord.bData, sizeof(WORD));

        memcpy(inputData + 4, &x, sizeof(double));

        inputData[12] = 0xBB;
        inputData[13] = 0xAA;

        udpSender->SenderData(inputData, 14);

        sleep(100);


    }


#endif

    qDebug()<<"Send Success!";

    infile.close();

    return a.exec();
}
