#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QObject>
#include <QUdpSocket>
#include "sysvar.h"
#include <QDateTime>
#include <QByteArray>
#include <string.h>
#include <QFile>
#include <QDataStream>
#include <QDateTime>
#include <QDir>
#include "tcpclient.h"

class UdpServer : public QObject
{
    Q_OBJECT
    QByteArray data;
    QByteArray data_all;
    QByteArray data2write;
    QString currentDir;
    void analysisData(QByteArray *thisData);
    unsigned short countCRC(QByteArray pBuf);
    unsigned short countCRC(char pBuf[], int length);
    bool calculateCrc(QByteArray data, int len);
    void outputToSocket(char *value, int len);
public:
    explicit UdpServer(QObject *parent = 0);
    QUdpSocket *receiver;
    TcpClient *tcpClient;

signals:

public slots:
private slots:
    void readData();
};

#endif // UDPSERVER_H
