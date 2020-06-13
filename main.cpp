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

    UdpServer *udpServer = new UdpServer;
    udpServer->tcpClient = tcpClient;
    udpServer->saveDataThread = saveDataThread;

//    TcpServer tcpserver;
//    tcpserver.tcpClient = tcpClient;

    return a.exec();
}
