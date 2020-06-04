#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QHostAddress>
#include <QDateTime>
#include <QByteArray>
#include "sysvar.h"
#include <string.h>

struct clientSocketDef
{
  QTcpSocket* socket;
  QDateTime commTime;
  QByteArray buffer;
  QByteArray readBuf;
  QByteArray writeBuf;
};

class TcpServer : public QObject
{
    Q_OBJECT
    int getIdleClientSocketIndex();
    struct clientSocketDef clientSockets[MAXTCPCLIENTS];
    void stopServerSocket();
    void initTcpServerParams();
    void analysisData(clientSocketDef clientSocket);
    bool calculateCrc(QByteArray data, int len);
    void outputToSocket(char *value, int len);
    unsigned short countCRC(QByteArray pBuf);
    unsigned short countCRC(char pBuf[], int length);
public:
    explicit TcpServer(QObject *parent = 0);
    QTcpServer* serverSocket;

    void startServerSocket();
signals:

public slots:
private slots:
    void acceptClientSocketConnect();
    void clientSocketDisConnect();
    void clientSocketReadyRead();
    void clientSocketError(QAbstractSocket::SocketError);
};

#endif // TCPSERVER_H
