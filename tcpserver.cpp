#include "tcpserver.h"


TcpServer::TcpServer(QObject *parent) : QObject(parent)
{
    serverSocket    = NULL;

    initTcpServerParams();
    startServerSocket();
}
void TcpServer::initTcpServerParams(void)
{
    sysData.lan.port = 5555;
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
                qDebug()<<clientSockets[i].readBuf;

//                clientSockets[i].socket->write(clientSockets[i].readBuf);
                analysisData(clientSockets[i]);
            }
        }
    }
}


void TcpServer::clientSocketError(QAbstractSocket::SocketError)
{
    //no handler
    clientSocketDisConnect();
}

bool TcpServer::calculateCrc(QByteArray data,int len)
{
    unsigned short crcsum = 0;
    unsigned short crc = int(data.at(len-2)<<8) + int(data.at(len-1));//高位在前低位在后

    for (int i=0; i < len-2; i++)
    {
        crcsum = (unsigned short)data.at(i);
        qDebug("%d",(unsigned short)data.at(i));
    }
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

    if (len < 2)return;
#ifdef TEST
    for (int i = 0; i < len; i++)
    {
        qDebug("0x%x ",(int)data.at(i));
    }
#endif
    if (!calculateCrc(data, len))
        return;

    int index = int(data.at(1)<<8) +int(data.at(2));//包序:高位在前低位在后
    /* handshape data */
    if (data.at(0) == 0x3A)
    {
        /* 需要的数据64B */
        data_get = data.mid(3);
        qDebug()<<data_get;
    }
    else if (data.at(0) == 0x3B)/* 数据包 */
    {
        int freq_index = data.at(3);//频段记号 0-3
        int data_16K_index = data.at(4);//序号0-15

        /* 需要的数据1024B */
        data_get = data.mid(5);
        qDebug()<<data_get;

        if (data_16K_index == 15)
        {
            /* 发送应答包 */
            char ret[69];
            memset(ret, 0, 69);
            ret[0] = 0x30;
            ret[2] = 0;
            for (int i=0;i < 69;i++)
            {
                ret[68] += ret[i];
            }
            clientSocket.socket->write(QByteArray(ret));
        }

    }

}
