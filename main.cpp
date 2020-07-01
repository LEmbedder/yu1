#include <QCoreApplication>
#include "tcpserver.h"
#include "tcpclient.h"
#include "udpserver.h"
#include "savedatathread.h"
#include "udpclient.h"

void Usage(char *argv[])
{
    qDebug("%s <tcpserver/udpserver> <tcpclient/udpclient> [savenumber]",argv[0]);
    qDebug("return");
}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug()<<VERSION;

    TcpClient *tcpClient = nullptr;
    UdpClient *udpClient = nullptr;
    QThread   *client    = nullptr;

    DEBUGPRINTF("main:%x",QThread::currentThreadId());

    if (argc < 3)
    {
        Usage(argv);
        return -1;
    }
    if (QString(argv[2]).toUpper() == "TCPCLIENT")
    {
        client = new QThread;
        client->setStackSize(1024 * 1024 * 4);
        sysData.connect_type = TCP;
        tcpClient = new TcpClient;
        tcpClient->moveToThread(client);
        qDebug()<<"tcpclient";
    }else{
        sysData.connect_type = UDP;
        udpClient = new UdpClient;
        qDebug()<<"udpclient";
    }

    SaveDataThread *saveDataThread = new SaveDataThread;
    saveDataThread->setStackSize(1024 * 1024 * 4);

    if (QString(argv[1]).toUpper() == "TCPSERVER")
    {
        TcpServer *tcpserver = new TcpServer;
        tcpserver->udpClient = udpClient;
        tcpserver->tcpClient = tcpClient;
        tcpserver->saveDataThread = saveDataThread;
        if (tcpClient != nullptr)
            QObject::connect(tcpserver,SIGNAL(emitWriteData(QByteArray)),tcpClient,SLOT(ClientDataWrite(QByteArray)));
        qDebug()<<"tcpserver";

    } else {
        UdpServer *udpServer = new UdpServer;
        udpServer->udpClient = udpClient;
        udpServer->tcpClient = tcpClient;
        udpServer->saveDataThread = saveDataThread;
        if (tcpClient != nullptr)
            QObject::connect(udpServer,SIGNAL(emitWriteData(QByteArray)),tcpClient,SLOT(ClientDataWrite(QByteArray)));
        qDebug()<<"udpserver";
    }
    if (client != nullptr)
        client->start();
    if (QString(argv[3]).toInt())
    {
        saveDataTimes = QString(argv[2]).toInt();
        if (saveDataTimes <=0 || saveDataTimes > 1200)
        {
            saveDataTimes = 1;
        }
        qDebug()<<saveDataTimes;
    }
    return a.exec();
}
