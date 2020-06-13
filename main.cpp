#include <QCoreApplication>
#include "tcpserver.h"
#include "tcpclient.h"
#include "udpserver.h"
#include "savedatathread.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TcpClient *tcpClient = new TcpClient;

    SaveDataThread *saveDataThread = new SaveDataThread;
    saveDataThread->setStackSize(1024 * 1024 * 4);


    if (QString(argv[1]) == "tcp")
    {
        TcpServer *tcpserver = new TcpServer;
        tcpserver->tcpClient = tcpClient;
        tcpserver->saveDataThread = saveDataThread;
        qDebug()<<"tcp";

    } else {
        UdpServer *udpServer = new UdpServer;
        udpServer->tcpClient = tcpClient;
        udpServer->saveDataThread = saveDataThread;
        qDebug()<<"udp";
    }
    if (QString(argv[2]).toInt())
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
