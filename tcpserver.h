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
#include <QFile>
#include <QDataStream>
#include <QDateTime>
#include <QDir>
#include "tcpclient.h"
#include "savedatathread.h"
#include "udpclient.h"

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
    QByteArray data_all;
    int getIdleClientSocketIndex();
    struct clientSocketDef clientSockets[MAXTCPCLIENTS];
    void stopServerSocket();
    void initTcpServerParams();
    bool calculateCrc(QByteArray data, int len);
    void outputToSocket(char *value, int len);
    unsigned short countCRC(QByteArray pBuf);
    unsigned short countCRC(char pBuf[], int length);
    void analysisData(clientSocketDef *clientSocket);
    void test();
    unsigned short countCRC2(char *pBuf, unsigned short DataLen);
    QString currentDir;
public:
    explicit TcpServer(QObject *parent = 0);
    QTcpServer* serverSocket;
    TcpClient *tcpClient;
    UdpClient *udpClient;
    SaveDataThread *saveDataThread;

    void startServerSocket();
signals:
    void emitWriteData(QByteArray data);
public slots:
private slots:
    void acceptClientSocketConnect();
    void clientSocketDisConnect();
    void clientSocketReadyRead();
    void clientSocketError(QAbstractSocket::SocketError);
};

#endif // TCPSERVER_H
