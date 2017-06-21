#include "Winsock2.h"  
#ifndef Server_H
#define Server_H
#define player 4
class Room        //游戏房间
{
public:
	SOCKADDR_IN addr_Srv;
	SOCKET socServer;
	char recvBuf[100];  //收到消息的缓冲区
	int client_num;         //游戏人数
	SOCKADDR_IN addr_Clt[player];  //每个玩家的地址
	Room(Room * room_thread);                  //构造函数
	//int CheckRe(SOCKADDR_IN addr);           //判定重复IP
	//int SendMe();            //发送消息
	void SetClient(SOCKADDR_IN Clt[player]);         //设置客户机IP，在匹配用户成功之后，设置IP；
	void Rece_SendMe();             //接收消息和转发
};

#endif