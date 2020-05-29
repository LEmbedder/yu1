#include <QCoreApplication>
#include "tcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    TcpServer *tcpserver = new TcpServer;

    return a.exec();
}
