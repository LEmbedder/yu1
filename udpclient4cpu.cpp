#include "udpclient4cpu.h"

UdpClient4Cpu::UdpClient4Cpu(QObject *parent) : QObject(parent)
{
    initTcpClientparams();
    loadDeviceSetting();
    sender = new QUdpSocket;
    connect(sender,&QUdpSocket::readyRead,this ,&UdpClient4Cpu::udpReadData);
    ClientDataWrite("123456",6);
}

void UdpClient4Cpu::initTcpClientparams(void)
{
    client.HostIp = HOSTIP4CPU;
    client.HostPort = HOSTPORT4CPU;
    client.interval = 10;
}
void UdpClient4Cpu::udpReadData()
{
    while(sender->hasPendingDatagrams())
    {
        QByteArray temp;
        temp.resize(sender->pendingDatagramSize());
        sender->readDatagram(temp.data(),temp.size());
//        data += temp;
//        qDebug("udpserver:%x",QThread::currentThreadId());
//        if (sysData.connect_type == TCP) {
//            emit emitWriteData(temp);
//        } else {
//            udpClient->ClientDataWrite(temp.data(),temp.length());
//        }
        qDebug("temp len:%d",temp.length());
    }
    /* 接收到的数据 */
//    analysisData(&data);

}
void UdpClient4Cpu::ClientDataWrite(char *value, int len)
{
    if (value != nullptr && len > 0 && sender != nullptr)
    {
        sender->writeDatagram(value, len, QHostAddress(client.HostIp) ,client.HostPort);
        if (!sender->waitForBytesWritten(30))
        {
            //connecToServerSocket();
        }
        return;
    }
}

/*
 * load rmtp device setting
*/
void UdpClient4Cpu::loadDeviceSetting()
{
    QString fileName = QCoreApplication::applicationDirPath() + "/device";
    QStringList tagList;
    if (QFile(fileName).exists())
    {
        QSettings setting(fileName, QSettings::IniFormat);
        setting.beginGroup("UdpClient4Cpu");
        tagList = setting.childKeys();

        if(tagList.indexOf("HostIP") != -1)
        {
            client.HostIp = setting.value("HostIP").toString();
        }
        else{
            saveDeviceSetting();
            return;
        }
        if(tagList.indexOf("HostPort") != -1)
        {
            client.HostPort  = (quint16)setting.value("HostPort").toInt();
        }
        if(tagList.indexOf("Interval") != -1)
        {
            client.interval = setting.value("Interval").toInt();
            if(client.interval < 10)
            {
                client.interval = 10;
            }
        }
        setting.endGroup();

    }
    else
    {
        saveDeviceSetting();
    }
}
/*
 * save rmtp device setting
*/
void UdpClient4Cpu::saveDeviceSetting(void)
{
    QString fileName = QCoreApplication::applicationDirPath() + "/device";
    QSettings setting(fileName, QSettings::IniFormat);
    //TcpClient
    setting.beginGroup("UdpClient4Cpu");
    setting.setValue("HostIP",QString(client.HostIp));
    setting.setValue("HostPort",client.HostPort);
    setting.setValue("Interval",client.interval);
    setting.endGroup();
}

