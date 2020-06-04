#include "tcpserver.h"

const ushort CrcTab[] = {
    0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
    0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
    0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
    0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
    0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
    0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
    0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
    0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
    0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
    0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
    0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
    0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
    0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
    0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
    0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
    0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};

/*
 * 计算CRC
 */
unsigned short TcpServer::countCRC(QByteArray pBuf)
{
    uint8_t temp;
    unsigned short CRC_Code = 0;
    int dataLen = 0;

    while (dataLen < pBuf.length())
    {
        temp = (uint8_t)(CRC_Code >> 8);
        CRC_Code <<= 8;
        //CRC_Code ^= CrcTab[temp ^ (pBuf[dataLen])];
        CRC_Code ^= CrcTab[temp ^ ( ((uint8_t)pBuf.at(dataLen)) & 0XFF)];
        dataLen++;
    }
    return (CRC_Code);
}

unsigned short TcpServer::countCRC(char pBuf[], int length)
{
    uint8_t temp;
    unsigned short CRC_Code = 0;
    int dataLen = 0;

    while (dataLen < length)
    {
        temp = (uint8_t)(CRC_Code >> 8);
        CRC_Code <<= 8;
        //CRC_Code ^= CrcTab[temp ^ (pBuf[dataLen])];
        CRC_Code ^= CrcTab[temp ^ ( ((uint8_t)pBuf[dataLen]) & 0XFF)];
        dataLen++;
    }
    return (CRC_Code);
}
TcpServer::TcpServer(QObject *parent) : QObject(parent)
{
    serverSocket    = NULL;

    initTcpServerParams();
    startServerSocket();
}
void TcpServer::initTcpServerParams(void)
{
    sysData.lan.port = SERVERPORT;
    for (int i = 0; i < MAXTCPCLIENTS; i++)
    {
        clientSockets[i].socket = NULL;
        clientSockets[i].buffer.clear();
        clientSockets[i].readBuf.clear();
        clientSockets[i].writeBuf.clear();
    }
}
void TcpServer::startServerSocket(void)
{
    stopServerSocket();
    if (serverSocket == NULL)
    {
        serverSocket = new QTcpServer;
        serverSocket->setMaxPendingConnections(1);
        serverSocket->listen(QHostAddress::Any, sysData.lan.port);
        connect(serverSocket, SIGNAL(newConnection()), this, SLOT(acceptClientSocketConnect()));
    }

}
void TcpServer::stopServerSocket(void)
{
    if (serverSocket != NULL)
    {
        disconnect(serverSocket, SIGNAL(newConnection()), this, SLOT(acceptClientSocketConnect()));
        serverSocket->close();
    }

    for (int i = 0; i < MAXTCPCLIENTS; i++)
    {
        if (clientSockets[i].socket != NULL && clientSockets[i].socket->state() == QAbstractSocket::ConnectedState)
        {
            disconnect(clientSockets[i].socket, SIGNAL(disconnected()), this, SLOT(clientSocketDisConnect()));
            disconnect(clientSockets[i].socket, SIGNAL(readyRead()), this, SLOT(clientSocketReadyRead()));
            disconnect(clientSockets[i].socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(clientSocketError(QAbstractSocket::SocketError)));
            clientSockets[i].socket->close();
            clientSockets[i].socket->deleteLater();
            clientSockets[i].socket = NULL;
            clientSockets[i].buffer.clear();
            clientSockets[i].readBuf.clear();
            clientSockets[i].writeBuf.clear();
        }
    }
}

/*
 * 接受客服端的连接
 */
void TcpServer::acceptClientSocketConnect(void)
{
    QTcpSocket* client = NULL;
    int index = -1;

    while (serverSocket->hasPendingConnections())
    {
        index = getIdleClientSocketIndex();
        client = serverSocket->nextPendingConnection();

        clientSockets[index].socket = client;
        clientSockets[index].commTime = QDateTime::currentDateTime();
        clientSockets[index].socket->readAll();
        clientSockets[index].buffer.clear();
        clientSockets[index].readBuf.clear();
        clientSockets[index].writeBuf.clear();

        if (clientSockets[index].socket->readBufferSize() != MAXTCPBUFSIZE)
        {
            clientSockets[index].socket->setReadBufferSize(MAXTCPBUFSIZE);
        }

        connect(clientSockets[index].socket, SIGNAL(disconnected()), this, SLOT(clientSocketDisConnect()));
        connect(clientSockets[index].socket, SIGNAL(readyRead()), this, SLOT(clientSocketReadyRead()));
        connect(clientSockets[index].socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(clientSocketError(QAbstractSocket::SocketError)));
    }
}

int TcpServer::getIdleClientSocketIndex(void)
{
    int index = -1;
    QDateTime minCommTime;

    for (int i = 0; i < MAXTCPCLIENTS; i++)
    {
        if (clientSockets[i].socket == NULL)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        index = 0;
        minCommTime = clientSockets[index].commTime;

        for (int i = 1; i < MAXTCPCLIENTS; i++)
        {
            if (clientSockets[i].commTime < minCommTime)
            {
                index = i;
                minCommTime = clientSockets[index].commTime;
            }
        }

        if (index >= 0 && index < MAXTCPCLIENTS)
        {
            disconnect(clientSockets[index].socket, SIGNAL(disconnected()), this, SLOT(clientSocketDisConnect()));
            disconnect(clientSockets[index].socket, SIGNAL(readyRead()), this, SLOT(clientSocketReadyRead()));
            disconnect(clientSockets[index].socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(clientSocketError(QAbstractSocket::SocketError)));
            clientSockets[index].socket->close();
            clientSockets[index].socket->deleteLater();
            clientSockets[index].socket = NULL;
            clientSockets[index].commTime = QDateTime::currentDateTime();
            clientSockets[index].buffer.clear();
            clientSockets[index].readBuf.clear();
            clientSockets[index].writeBuf.clear();
        }
    }

    return index;
}

void TcpServer::clientSocketDisConnect(void)
{
    for (int i = 0; i < MAXTCPCLIENTS; i++)
    {
        if (clientSockets[i].socket != NULL && clientSockets[i].socket->state() == QAbstractSocket::UnconnectedState)
        {
            disconnect(clientSockets[i].socket, SIGNAL(disconnected()), this, SLOT(clientSocketDisConnect()));
            disconnect(clientSockets[i].socket, SIGNAL(readyRead()), this, SLOT(clientSocketReadyRead()));
            disconnect(clientSockets[i].socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(clientSocketError(QAbstractSocket::SocketError)));
            clientSockets[i].socket->close();
            clientSockets[i].socket->deleteLater();
            clientSockets[i].socket = NULL;
            clientSockets[i].commTime = QDateTime::currentDateTime();
            clientSockets[i].buffer.clear();
            clientSockets[i].readBuf.clear();
            clientSockets[i].writeBuf.clear();
        }
    }
}
void TcpServer::clientSocketReadyRead(void)
{
    qint64 len = 0;

    for (int i = 0; i < MAXTCPCLIENTS; i++)
    {
        if (clientSockets[i].socket != NULL)
        {
            len = clientSockets[i].socket->bytesAvailable();
            if (len > 0)
            {
                clientSockets[i].readBuf = clientSockets[i].socket->readAll();
                while (clientSockets[i].socket->bytesAvailable() > 0)
                {
                    clientSockets[i].readBuf += clientSockets[i].socket->readAll();
                }
                /* 接收到的数据 */
                analysisData(clientSockets[i]);
            }
        }
    }
}


void TcpServer::clientSocketError(QAbstractSocket::SocketError)
{
    clientSocketDisConnect();
}

bool TcpServer::calculateCrc(QByteArray data,int len)
{
    unsigned short crcsum = 0;
    unsigned short crc;//低位在前高位在后

    crc = int(data.at(len-2))<<8 + int(data.at(len-1));
    crcsum = countCRC(data.mid(1,-2));
    printf("crcsum = 0x%x",crcsum);
    printf("\n");fflush(stdout);
    if (crcsum != crc)
    {
        qDebug("crcsum = %d != crc = %d ,calculateCrc data error",crcsum,crc);
        return false;
    }

    return true;
}

void TcpServer::analysisData(struct clientSocketDef clientSocket)
{
    QByteArray data = clientSocket.readBuf;
    QByteArray data_get;
    int len = data.length();

#if 0
    for (int i = 0; i < len; i++)
    {
        printf("0x%x ",(unsigned char)data.at(i));
    }
    printf("\n");fflush(stdout);
#endif

    /* 握手信号 */
    if (int(data.at(0)) == 0x3A)
    {
        if (!calculateCrc(data, len))
            return;
        /* 发送应答包 */
        unsigned short crc = 0;
        unsigned short *tmp = 0;
        char ret[72];
        memset(ret, 0 ,72);
        ret[0] = 0x30;
        ret[1] = 0x3A;
        crc = countCRC(&ret[1],70);
        tmp = (unsigned short *)&ret[70];
        *tmp = crc;
        outputToSocket(ret, 72);

    }/* 数据包 */
    else if (int(data.at(0)) == 0x3B)
    {

        /* 需要的数据1024B */

//        if (data_16K_index == 15)
//        {
//            /* 发送应答包 */
//            char ret[69];
//            memset(ret, 0, 69);
//            ret[0] = 0x30;
//            ret[2] = 0;
//            for (int i=0;i < 69;i++)
//            {
//                ret[68] += ret[i];
//            }
//        }

    }

}
void TcpServer::outputToSocket(char* value, int len)
{
    if (value != NULL && len > 0 && clientSockets[0].socket != NULL && clientSockets[0].socket->isOpen() && clientSockets[0].socket->isWritable())
    {
        clientSockets[0].socket->write(value, len);
        if (!clientSockets[0].socket->waitForBytesWritten(3000))
        {
        }
        return;
    }
}
