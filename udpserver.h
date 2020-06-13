#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QObject>
#include <QUdpSocket>
#include "sysvar.h"
#include <QDateTime>
#include <QByteArray>
#include <string.h>
#include "tcpclient.h"
#include "savedatathread.h"

class UdpServer : public QObject
{
    Q_OBJECT
    QByteArray data;
    QByteArray data_all;

    void analysisData(QByteArray *thisData);
    unsigned short countCRC(QByteArray pBuf);
    unsigned short countCRC(char pBuf[], int length);
    bool calculateCrc(QByteArray data, int len);
    void outputToSocket(char *value, int len);
public:
    QByteArray data2write;
    explicit UdpServer(QObject *parent = 0);
    QUdpSocket *receiver;
    TcpClient *tcpClient;
    SaveDataThread *saveDataThread;
signals:

public slots:
private slots:
    void readData();
};

#endif // UDPSERVER_H
