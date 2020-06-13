#ifndef SAVEDATATHREAD_H
#define SAVEDATATHREAD_H

#include <QObject>
#include <QtCore/QThread>
#include <QFile>
#include <QDataStream>
#include <QDateTime>
#include <QDir>
#include <QByteArray>
#include <QQueue>
#include <QDebug>

class SaveDataThread : public QThread
{
    Q_OBJECT
    QString currentDir;
    bool isRuning = false;
public:
    explicit SaveDataThread(QThread *parent = 0);
    virtual void run();
    QQueue<QString> queueTime;
    QQueue<QByteArray> queueData;
signals:

public slots:
};

#endif // SAVEDATATHREAD_H
