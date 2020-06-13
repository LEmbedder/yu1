#include "savedatathread.h"

SaveDataThread::SaveDataThread(QThread *parent) : QThread(parent)
{
    QDir tempDir;
    //临时保存程序当前路径
    currentDir = tempDir.currentPath()+"/data/";
    //如果filePath路径不存在，创建它
    if(!tempDir.exists(currentDir))
    {
        tempDir.mkpath(currentDir);
    }
}

void SaveDataThread::run()
{
    while(true)
    {
        /* 写入文件 */
        if (!udpServer->queueData.isEmpty())
        {
            QByteArray array = udpServer->queueData.dequeue();
            QString current_date = udpServer->queueTime.dequeue();

            current_date = currentDir + current_date + ".dat";
            QFile file(current_date);
            file.open(QIODevice::WriteOnly);
            QDataStream out(&file);
            out.setVersion(QDataStream::Qt_4_0);
            for (int i = 0; i < 1200; i++)
                out.writeRawData(array.data(),array.size());/* 不会有多余的头部字节 */
            file.close();
        }
    }

}
