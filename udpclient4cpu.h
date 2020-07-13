#ifndef UDPCLIENT4CPU_H
#define UDPCLIENT4CPU_H

#include <QObject>
#include <QtNetwork>
#include <QTimer>
#include "sysvar.h"
#include <QCoreApplication>
#include <QSettings>
#include <QFile>

class UdpClient4Cpu : public QObject
{
    Q_OBJECT
    struct s_rmtp client;
    void loadDeviceSetting();
    void saveDeviceSetting();
    void initTcpClientparams();
    void udpReadData();
public:
    explicit UdpClient4Cpu(QObject *parent = 0);
    QUdpSocket *sender;
    void ClientDataWrite(char *value, int len);
signals:

public slots:
};

#endif // UDPCLIENT4CPU_H
