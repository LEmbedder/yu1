#ifndef SYSVAR_H
#define SYSVAR_H

#include <QString>

/*
 * 服务端数据
 */
#define MAXTCPCLIENTS 1
#define MAXTCPBUFSIZE (1024 * 1024)
#define SOCKETTIMEOUT 3000   //3000ms
#define SERVERPORT 5555
/*
 * 客户端数据
 */
#define HOSTIP   "192.168.1.4"
#define HOSTPORT 15000

/*
 * 应答包返回结果
 */
#define SUCCESS 0x00 //数据包接收成功；
#define CRCERR  0x01 //校验错误；
#define LENERR  0x02 //长度错误；

/*
 * LAN定义
*/
struct lanDef
{
  char ip[32];                   //IP地址
  char hostIp[64];
  char mask[32];                 //子网掩码
  char gate[32];                 //网关
  char mac[32];                  //MAC地址
  int  hostPort;
  unsigned int port;             //端口
};

//系统数据结构
struct SysData
{
    lanDef lan;
};

extern SysData sysData;
extern int saveDataTimes;
#endif // SYSVAR_H
