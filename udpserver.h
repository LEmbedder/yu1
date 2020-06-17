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
#include "udpclient.h"

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
    explicit UdpServer(QObject *parent = nullptr);
    QUdpSocket *receiver;
    TcpClient *tcpClient;
    UdpClient *udpClient;
    SaveDataThread *saveDataThread;

signals:

public slots:
private slots:
    void readData();
};

#endif // UDPSERVER_H
