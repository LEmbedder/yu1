#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{
    cSocket = new QTcpSocket(this);

    isConnect = false;
    initTcpClientparams();
    loadDeviceSetting();
    connecToServerSocket();
    connect(cSocket,SIGNAL(readyRead()),this,SLOT(ClientDataReceived()));
    connect(cSocket,SIGNAL(disconnected()),this,SLOT(disConnectFromServerSocket()));
    connect(cSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(disConnectFromServerSocket()));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(connecToServerSocket()));
    timer->start(1000*10);
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
        qDebug("connect %s ,%d  ok\n",client.HostIp.toStdString().c_str(),client.HostPort);
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
    timer->start(1000*client.interval);
    while (cSocket->bytesAvailable() > 0)
    {
        ClientreadBuf = cSocket->readAll();
//        QByteArray datagram;
//        datagram.resize(cSocket->bytesAvailable());
//        cSocket->read(datagram.data(),datagram.size());
//        QString msg = datagram.data();
//        qDebug()<<msg;
//        ClientDataWrite(ClientreadBuf.data(),ClientreadBuf.length());
    }
}
void TcpClient::ClientDataWrite(QByteArray data)
{
    ClientDataWrite(data.data(), data.length());
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
    if (value != NULL && len > 0 && cSocket != NULL && (isConnect == true))
    {
        cSocket->write(value, len);
        if (!cSocket->waitForBytesWritten(30))
        {
            //connecToServerSocket();
        }
        return;
    }
}
/*
 * load rmtp device setting
*/
void TcpClient::loadDeviceSetting()
{
    QString fileName = QCoreApplication::applicationDirPath() + "/device";
    QStringList tagList;
    if (QFile(fileName).exists())
    {
        QSettings setting(fileName, QSettings::IniFormat);
        setting.beginGroup("TcpClient");
        tagList = setting.childKeys();

        if(tagList.indexOf("HostIP") != -1)
        {
            client.HostIp = setting.value("HostIP").toString();
        }
        if(tagList.indexOf("HostPort") != -1)
        {
            client.HostPort  = setting.value("HostPort").toInt();
        }
        if(tagList.indexOf("Interval") != -1)
        {
            client.interval = setting.value("Interval").toInt();
            if(client.interval < 10)
            {
                client.interval = 10;
            }
        }
        setting.endGroup();

    }else
    {
        saveDeviceSetting();
    }
}
/*
 * save rmtp device setting
*/
void TcpClient::saveDeviceSetting(void)
{
    QString fileName = QCoreApplication::applicationDirPath() + "/device";
    QSettings setting(fileName, QSettings::IniFormat);
    //TcpClient
    setting.beginGroup("TcpClient");
    setting.setValue("HostIP",QString(client.HostIp));
    setting.setValue("HostPort",client.HostPort);
    setting.setValue("Interval",client.interval);
    setting.endGroup();
}
