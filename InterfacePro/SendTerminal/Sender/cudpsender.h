#ifndef CUDPSENDER_H
#define CUDPSENDER_H

#include <QUdpSocket>

#define SENDPORT 8090

class CUdpSender
{
private:
    QUdpSocket *udpSendsocket;
    QHostAddress SenderAddress;

public:
    CUdpSender();
    ~CUdpSender();

    void InitUdp();
    void SenderData(const char *SendData, qint64 len);


};

#endif // CUDPSENDER_H
