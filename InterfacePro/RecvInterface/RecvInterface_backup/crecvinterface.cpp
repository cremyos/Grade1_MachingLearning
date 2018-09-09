#include "crecvinterface.h"
#include "ui_crecvinterface.h"


CRecvInterface::CRecvInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CRecvInterface)
{
    ui->setupUi(this);

    i32RecvIndex = 0;

    UdpRecv = new QUdpSocket();
    UdpRecv->bind(QHostAddress("192.168.1.3"), RECVPORT);

    connect(UdpRecv, SIGNAL(readyRead()), this, SLOT(RecvData()));

}

void CRecvInterface::RecvData()
{
#if 1
    while(UdpRecv->hasPendingDatagrams()) {
        QByteArray datagram;

        datagram.resize(UdpRecv->pendingDatagramSize());

        UdpRecv->readDatagram(datagram.data(), datagram.size());

        BYTE tempType[2] = {0x0};

        UN_BYTE_WORD unByteWord;

        memcpy(unByteWord.bData, datagram.data() + 2, sizeof(BYTE) * 2);


        BYTE tempData[100] = {0x0};

        memcpy(tempData, datagram.data() + 4, sizeof(BYTE) * (datagram.size() - 6));

        printf("Data is : \n");
        for(int Index = 0; Index < datagram.size() - 6; Index++) {
            printf("0x%x ", tempData[Index]);
        }
        printf("\n");

        UN_BYTE_DOUBLE unByteDou;

        memcpy(unByteDou.bData, tempData, 8 * sizeof(BYTE));

        k_means.AddSamples(unByteWord.wData, unByteDou.dData);

        qDebug()<<"Data = " + QString::number(unByteDou.dData);

        k_means.assignCentroid(unByteDou.dData, k_means.i32SampleIndex);
        k_means.calculateNewCentroid();

        i32RecvIndex++;

    }
#endif
}

void CRecvInterface::ReadFileData()
{
#if 0
    string input = "input_Data.txt";
    double x = 0.0;


    infile.open(input.c_str());
    if (!infile) {
        cout << "Unable to open input."<< endl;
    }

    char inputData[100] = {0x0};


    while(!infile.eof()) {

        infile >> x;

        qDebug()<<"x = " + QString::number(x);

        k_means.AddSamples(x);

        k_means.assignCentroid(x, k_means.i32SampleIndex);
        k_means.calculateNewCentroid();

        i32RecvIndex++;
//        k_means.ShowData();

    }

#if 0
    for(int i32TempIndex = 0; i32TempIndex < i32RecvIndex; i32TempIndex++) {
        k_means.assignCentroid(k_means.Samples[i32TempIndex], i32TempIndex);
    }



    for( int i=0; i<2; i++)
    {
        k_means.calculateNewCentroid();
        for( int i32TempIndex=0; i32TempIndex < i32RecvIndex; i32TempIndex++)
        {
            k_means.assignCentroid(k_means.Samples[i32TempIndex], i32TempIndex);
        }
    }
#endif

    qDebug()<<"Final ";
//    k_means.ShowData();

    infile.close();
#endif
}

CRecvInterface::~CRecvInterface()
{
    qDebug()<<"i32RecvIndex = " + QString::number(i32RecvIndex) + " || k_means.i32SampleIndex = " + QString::number(k_means.i32SampleIndex);
    delete UdpRecv;
    delete ui;
}
