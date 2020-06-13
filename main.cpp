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
    udpServer->tcpClient = tcpClient;

    SaveDataThread saveDataThread;
    saveDataThread.udpServer = udpServer;
    saveDataThread.setStackSize(1024 * 1024 * 4);
    saveDataThread.start();
//    TcpServer tcpserver;
//    tcpserver.tcpClient = tcpClient;

    return a.exec();
}
