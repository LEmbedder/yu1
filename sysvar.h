#ifndef SYSVAR_H
#define SYSVAR_H

/*
 * 数据的定义
 */
#define MAXTCPCLIENTS 1
#define MAXTCPBUFSIZE (1024 * 1024)
#define SOCKETTIMEOUT 3000   //3000ms

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

#endif // SYSVAR_H
