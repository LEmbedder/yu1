#include <QCoreApplication>
#include "tcpserver.h"
#include "tcpclient.h"
#include "udpserver.h"
#include "savedatathread.h"
#include "udpclient.h"

#define STACKSIZE 1024*1024*4

void Usage(char *argv[])
{
    qDebug("%s <tcpserver/udpserver> <tcpclient/udpclient> [savenumber]",argv[0]);
    qDebug("return");
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TcpClient *tcpClient = nullptr;
    UdpClient *udpClient = nullptr;

    if (argc < 3)
    {
        Usage(argv);
        return -1;
    }

    DEBUGPRINTF("main:%x",QThread::currentThreadId());

    QThread   *client    = new QThread;
    client->setStackSize(STACKSIZE);

    SaveDataThread *saveDataThread = new SaveDataThread;
    saveDataThread->setStackSize(STACKSIZE);

    if (QString(argv[2]).toUpper() == "TCPCLIENT")
    {
        sysData.connect_type = TCPCLIENT;
        tcpClient = new TcpClient;
        tcpClient->moveToThread(client);
        qDebug()<<"tcpclient";
    }else{
        sysData.connect_type = UDPCLIENT;
        udpClient = new UdpClient;
        udpClient->moveToThread(client);
        qDebug()<<"udpclient";
    }

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
        else if (udpClient != nullptr)
            QObject::connect(udpServer,SIGNAL(emitWriteData(QByteArray)),udpClient,SLOT(ClientDataWrite(QByteArray)));
        qDebug()<<"udpserver";
    }

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
