#include "cudpsender.h"

CUdpSender::CUdpSender()
{
    InitUdp();
}

void CUdpSender::InitUdp()
{
    udpSendsocket = new QUdpSocket();

    SenderAddress = QHostAddress("192.168.1.3");
}


void CUdpSender::SenderData(const char *SendData, qint64 len)
{
    udpSendsocket->writeDatagram(SendData, len, SenderAddress, SENDPORT);
}


CUdpSender::~CUdpSender()
{
    delete udpSendsocket;
}
