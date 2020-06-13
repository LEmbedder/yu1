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
        if (!udpServer->pupDataFormDataList().time.isEmpty())
        {
            QFile file(udpServer->pupDataFormDataList().time);
            QDataStream out(&file);
            out.setVersion(QDataStream::Qt_4_0);
            for (int i = 0; i < 1200; i++)
                out.writeRawData(udpServer->data2write.data(),udpServer->data2write.size());/* 不会有多余的头部字节 */
            file.close();
        }
//        if (udpServer->data2write.length() == 16*1024)
//        {
//            QDateTime current_date_time = QDateTime::currentDateTime();
//            QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm::ss.zzz");
//            current_date = currentDir + current_date + ".dat";
//            QFile file(current_date);
//            file.open(QIODevice::WriteOnly);
//            QDataStream out(&file);
//            out.setVersion(QDataStream::Qt_4_0);
//            for (int i = 0; i < 1200; i++)
//                out.writeRawData(udpServer->data2write.data(),udpServer->data2write.size());/* 不会有多余的头部字节 */
//            file.close();
//            udpServer->data2write.clear();
//        }
    }

}
