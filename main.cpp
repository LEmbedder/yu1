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

    TcpClient *tcpClient = nullptr;
    UdpClient *udpClient = nullptr;

    if (argc < 3)
    {
        Usage(argv);
        return -1;
    }
    if (QString(argv[2]) == "tcpclient")
    {
        sysData.connect_type = TCP;
        tcpClient = new TcpClient;
        qDebug()<<"tcpclient";
    }else{
        sysData.connect_type = UDP;
        udpClient = new UdpClient;
        qDebug()<<"udpclient";
    }

    SaveDataThread *saveDataThread = new SaveDataThread;
    saveDataThread->setStackSize(1024 * 1024 * 4);

    if (QString(argv[1]) == "tcpserver")
    {
        TcpServer *tcpserver = new TcpServer;
        tcpserver->udpClient = udpClient;
        tcpserver->tcpClient = tcpClient;
        tcpserver->saveDataThread = saveDataThread;
        qDebug()<<"tcpserver";

    } else {
        UdpServer *udpServer = new UdpServer;
        udpServer->udpClient = udpClient;
        udpServer->tcpClient = tcpClient;
        udpServer->saveDataThread = saveDataThread;
        qDebug()<<"udpserver";
    }
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
