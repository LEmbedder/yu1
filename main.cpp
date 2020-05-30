#include <QCoreApplication>
#include "tcpserver.h"
#include "tcpclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TcpServer tcpserver;
    TcpClient tcpClient;

    return a.exec();
}
