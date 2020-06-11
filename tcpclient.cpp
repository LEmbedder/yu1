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
    isConnect = false;
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
        isConnect = true;
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
    isConnect = false;
    return true;
}

/*Tcp Client 读取数据*/
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
/* 写出数据 */
void TcpClient::ClientDataWrite(char *value,int len)
{
#if 0
    printf("222222222222222222222222222222\n");
    for (int j = 0; j < 10; j++)
    {
        printf("0x%x ",(uint8_t)value[j]);
    }
    printf("\n");fflush(stdout);
#endif
    if (value != NULL && len > 0 && cSocket != NULL && isConnect == true)
    {
        cSocket->write(value, len);
        if (!cSocket->waitForBytesWritten(3000))
        {
            //connecToServerSocket();
        }
        return;
    }
}
