#include <QCoreApplication>
#include "tcpserver.h"
#include "tcpclient.h"
#include "udpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TcpClient *tcpClient = new TcpClient;
    UdpServer udpServer;
    udpServer.tcpClient = tcpClient;
//    TcpServer tcpserver;
//    tcpserver.tcpClient = tcpClient;

    return a.exec();
}
