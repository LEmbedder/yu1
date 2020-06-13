#include <QCoreApplication>
#include "tcpserver.h"
#include "tcpclient.h"
#include "udpserver.h"
#include "savedatathread.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TcpClient *tcpClient = new TcpClient;
    UdpServer *udpServer = new UdpServer;

    SaveDataThread *saveDataThread = new SaveDataThread;
    saveDataThread->setStackSize(1024 * 1024 * 4);
    udpServer->tcpClient = tcpClient;
    udpServer->saveDataThread = saveDataThread;

//    TcpServer tcpserver;
//    tcpserver.tcpClient = tcpClient;

    return a.exec();
}
