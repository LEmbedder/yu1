#ifndef SAVEDATATHREAD_H
#define SAVEDATATHREAD_H

#include <QObject>
#include <QtCore/QThread>
#include <sysvar.h>
#include <QFile>
#include <QDataStream>
#include <QDateTime>
#include <QDir>

class SaveDataThread : public QThread
{
    Q_OBJECT
    QString currentDir;
    bool isRuning;
public:
    explicit SaveDataThread(QThread *parent = 0);
    virtual void run();
signals:

public slots:
};

#endif // SAVEDATATHREAD_H
