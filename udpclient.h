#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QtNetwork>
#include <QTimer>
#include "sysvar.h"
#include <QCoreApplication>
#include <QSettings>
#include <QFile>



class UdpClient : public QObject
{
    Q_OBJECT
    struct s_rmtp client;
    void loadDeviceSetting();
    void saveDeviceSetting();
    void initTcpClientparams();
public:
    explicit UdpClient(QObject *parent = nullptr);
    QUdpSocket *sender;
    void ClientDataWrite(char *value, int len);
public slots:
    void ClientDataWrite(QByteArray data);
};

#endif // UDPCLIENT_H
