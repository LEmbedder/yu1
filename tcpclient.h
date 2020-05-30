#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QTimer>
#include "sysvar.h"


struct s_rmtp{
  QString isEnable;
  QString HostIp;
  int HostPort;
  int interval;
};

class TcpClient : public QObject
{
    Q_OBJECT
    QTcpSocket* cSocket;
    struct s_rmtp client;
    QByteArray ClientreadBuf;
    QTimer *timer;

    void initTcpClientparams();
public:
    explicit TcpClient(QObject *parent = 0);

signals:

public slots:
private slots:
    void ClientDataReceived();
    bool disConnectFromServerSocket();
    bool connecToServerSocket();
};

#endif // TCPCLIENT_H
