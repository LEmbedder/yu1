#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{
    cSocket = new QTcpSocket(this);

    initTcpClientparams();
    connecToServerSocket();
    connect(cSocket,SIGNAL(readyRead()),this,SLOT(ClientDataReceived()));
    connect(cSocket,SIGNAL(disconnected()),this,SLOT(disConnectFromServerSocket()));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(connecToServerSocket()));
//    timer->start(1000*10);
}
void TcpClient::initTcpClientparams(void)
{
    client.HostIp = HOSTIP;
    client.HostPort = HOSTPORT;
    client.interval = 10;
}
bool TcpClient::connecToServerSocket(void)
{
    disConnectFromServerSocket();
    cSocket->connectToHost(client.HostIp, client.HostPort, QTcpSocket::ReadWrite);
    if (cSocket->waitForConnected(1000))
    {
        qDebug("connect ok\n");
        return true;
    } else
    {
        return false;
    }
}
//断开与服务器的连接
bool TcpClient::disConnectFromServerSocket(void)
{
    cSocket->disconnectFromHost();
    return true;
}

/*Tcp Client connect*/
void TcpClient::ClientDataReceived(void)
{
//    timer->start(1000*client.interval);
    while (cSocket->bytesAvailable() > 0)
    {
        ClientreadBuf = cSocket->readAll();
//        QByteArray datagram;
//        datagram.resize(cSocket->bytesAvailable());
//        cSocket->read(datagram.data(),datagram.size());
//        QString msg = datagram.data();
//        qDebug()<<msg;
    }
}
