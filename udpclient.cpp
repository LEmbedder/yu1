#include "udpclient.h"

UdpClient::UdpClient(QObject *parent) : QObject(parent)
{
    initTcpClientparams();
    loadDeviceSetting();
    sender = new QUdpSocket;
}

void UdpClient::initTcpClientparams(void)
{
    client.HostIp = HOSTIP;
    client.HostPort = HOSTPORT;
    client.interval = 10;
}
void UdpClient::ClientDataWrite(QByteArray data)
{
    ClientDataWrite(data.data(), data.length());
}
void UdpClient::ClientDataWrite(char *value, int len)
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
void UdpClient::loadDeviceSetting()
{
    QString fileName = QCoreApplication::applicationDirPath() + "/device";
    QStringList tagList;
    if (QFile(fileName).exists())
    {
        QSettings setting(fileName, QSettings::IniFormat);
        setting.beginGroup("TcpClient");
        tagList = setting.childKeys();

        if(tagList.indexOf("HostIP") != -1)
        {
            client.HostIp = setting.value("HostIP").toString();
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

    }else
    {
        saveDeviceSetting();
    }
}
/*
 * save rmtp device setting
*/
void UdpClient::saveDeviceSetting(void)
{
    QString fileName = QCoreApplication::applicationDirPath() + "/device";
    QSettings setting(fileName, QSettings::IniFormat);
    //TcpClient
    setting.beginGroup("TcpClient");
    setting.setValue("HostIP",QString(client.HostIp));
    setting.setValue("HostPort",client.HostPort);
    setting.setValue("Interval",client.interval);
    setting.endGroup();
}

