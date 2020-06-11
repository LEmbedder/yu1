#include "udpserver.h"

UdpServer::UdpServer(QObject *parent) : QObject(parent)
{
    receiver = new QUdpSocket(this);
    receiver->bind(SERVERPORT, QUdpSocket::ShareAddress);
    connect(receiver,&QUdpSocket::readyRead,this ,&UdpServer::readData);

    data.resize(MAXTCPBUFSIZE);
    data.clear();
}

void UdpServer::readData()
{
    while(receiver->hasPendingDatagrams())
    {
        QByteArray temp;
        temp.resize(receiver->pendingDatagramSize());
        receiver->readDatagram(temp.data(),temp.size());
        data += temp;
    }
    //
    tcpClient->ClientDataWrite(data.data(),data.length());
    /* 接收到的数据 */
    analysisData(&data);

}
void UdpServer::outputToSocket(char* value, int len)
{
    if (value != NULL && len > 0)
    {
        receiver->writeDatagram(value, len,QHostAddress::Broadcast,SERVERPORT+1);
        if (!receiver->waitForBytesWritten(3000))
        {
        }
        return;
    }
}
static const ushort CrcTab[] = {
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
unsigned short UdpServer::countCRC(QByteArray pBuf)
{
    uint8_t temp;
    unsigned short CRC_Code = 0;
    int dataLen = 0;

    while (dataLen < pBuf.length())
    {
//        printf("0x%x ",(uint8_t)pBuf.at(dataLen));
        temp = (uint8_t)(CRC_Code >> 8);
        CRC_Code <<= 8;
        //CRC_Code ^= CrcTab[temp ^ (pBuf[dataLen])];
        CRC_Code ^= CrcTab[temp ^ ( ((uint8_t)pBuf.at(dataLen)) & 0XFF)];
        dataLen++;
    }
//    printf("\n");
    return (CRC_Code);
}

unsigned short UdpServer::countCRC(char pBuf[], int length)
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

bool UdpServer::calculateCrc(QByteArray data,int len)
{
    unsigned short high,low;
    unsigned short crcsum = 0;
    unsigned short crc;//低位在前高位在后

#if 0
    for (int i = 0; i < len; i++)
    {
        printf("0x%x ",(unsigned char)data.at(i));
    }
    printf("\n");fflush(stdout);
#endif

    high = ((uint8_t)data.at(len-1))&0xFFFF;
    low = ((uint8_t)data.at(len-2))&0xFFFF;
//    printf("high = 0x%x;low = 0x%x\n",high,low);
    crc = (high<<8) + low;
    crcsum = countCRC(data.mid(1,data.length()-3));
//    printf("crcsum = 0x%x;crc = 0x%x",crcsum,crc);
//    printf("\n");fflush(stdout);
    if (crcsum != crc)
    {
        qDebug("crcsum = 0x%x != crc = 0x%x ,calculateCrc data error",crcsum,crc);
        return false;
    }

    return true;
}

#define HANDSHANPLEN 71
#define DATALEN      16528
#define ONEBAGLEN    1033
void UdpServer::analysisData(QByteArray *thisData)
{
    QByteArray dataTemp = *thisData;
    int len = dataTemp.length();
#if 0
    for (int i = 0; i < len; i++)
    {
        printf("%02x ",(unsigned char)dataTemp.at(i));
    }
    printf("\n");fflush(stdout);
#endif
    /* 握手信号 */
    if ((int(dataTemp.at(0)) == 0x3A) && (len >= HANDSHANPLEN))
    {
        if (!calculateCrc(dataTemp, HANDSHANPLEN))
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
        *thisData = thisData->mid(HANDSHANPLEN);//

    }/* 数据包 */
    else if (uint8_t(dataTemp.at(0)) == 0x3B)
    {
        /* 需要的数据1024B */
        if (dataTemp.length() >= DATALEN)
        {
            unsigned short crc = 0;
            unsigned short *tmp = 0;
            char ret[72];
            memset(ret, 0 ,72);
            ret[0] = 0x30;
            ret[1] = 0x3B;

            data_all = thisData->mid(0, DATALEN);
            /* 对64K拆包校验,如果无误并写入文件 */
            for (int i = 0; i < 16; i++)
            {
                QByteArray tmp = data_all.mid(0,ONEBAGLEN);
//                qDebug("tmp = %d",tmp.length());
                if (!calculateCrc(tmp,ONEBAGLEN)){
                    ret[5] = CRCERR;
                    goto end;
                }
                ret[5] = SUCCESS;
                data2write += tmp.mid(7, 1024);
            }

//            qDebug("data2write = %d",data2write.length());
//            printf("0x%x ",(uint8_t)data2write.at(0));
//            printf("0x%x ",(uint8_t)data2write.at(1));
//            printf("0x%x ",(uint8_t)data2write.at(2));
//            printf("0x%x ",(uint8_t)data2write.at(16383));
//            printf("\n");fflush(stdout);
            /* 写入文件 */
            if (1)
            {
                QDateTime current_date_time = QDateTime::currentDateTime();
                QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm::ss.zzz");
                current_date = currentDir + current_date + ".dat";
                QFile file(current_date);
                file.open(QIODevice::WriteOnly);
                QDataStream out(&file);
                out.setVersion(QDataStream::Qt_4_0);
                out.writeRawData(data2write.data(),data2write.size());/* 不会有多余的头部字节 */
                file.close();
            }

end:
            /* 返回握手确认包 */
            crc = countCRC(&ret[1],70);
            tmp = (unsigned short *)&ret[70];
            *tmp = crc;
            outputToSocket(ret, 72);

            data2write.clear();
            data_all.clear();
            *thisData = thisData->mid(DATALEN);
//            qDebug("\n");
        }else{
            return;
        }
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