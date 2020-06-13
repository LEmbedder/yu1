#ifndef SAVEDATATHREAD_H
#define SAVEDATATHREAD_H

#include <QObject>
#include <QtCore/QThread>
#include "udpserver.h"

class SaveDataThread : public QThread
{
    Q_OBJECT
    QString currentDir;
public:
    explicit SaveDataThread(QThread *parent = 0);
    virtual void run();
    UdpServer *udpServer;
signals:

public slots:
};

#endif // SAVEDATATHREAD_H
