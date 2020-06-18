#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QtNetwork>
#include <QTimer>
#include "sysvar.h"
#include <QCoreApplication>
#include <QSettings>
#include <QFile>
#include <QThread>


class TcpClient : public QObject
{
    Q_OBJECT
    QTcpSocket* cSocket;
    struct s_rmtp client;
    QByteArray ClientreadBuf;
    QTimer *timer;
    bool isConnect;

    void initTcpClientparams();
    void loadDeviceSetting();
    void saveDeviceSetting();
public:
    explicit TcpClient(QObject *parent = nullptr);

    void ClientDataWrite(char *value, int len);
signals:

public slots:
    void ClientDataWrite(QByteArray data);
private slots:
    void ClientDataReceived();
    bool disConnectFromServerSocket();
    bool connecToServerSocket();
};

#endif // TCPCLIENT_H
