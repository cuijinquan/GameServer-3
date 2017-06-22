//本cpp文件主写匹配玩家 ，然后另开一个游戏房间，并创建线程为房间服务，然后回到主匹配队列等待其他玩家
#include <stdio.h>  
#include <tchar.h>  
#include<iostream>
#include <vector>
#include "stdio.h"  
#include "Winsock2.h"  
#include "iostream"  
#include "string"  
#include<pthread.h>
#include"Server.h"
#include"Login.h"
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "pthreadVC2.lib")
#pragma comment(lib,"Msimg32.lib")
#pragma comment(lib,"ws2_32.lib")
#define MAX 1024
//define host IP and usable port.  
#define HOST_IP 127.0.0.1  
#define MATCH_HOST_PORT 8081    //匹配所用套接字和游戏用的套接字不是同一个，匹配系统用的8081，游戏系统用的8080
#define OK_STR "Conn_Ok"
#define THANKS "你已经成功连接服务器"
#define MATCH_STR "NEED_MATCH"  //等待匹配字符串，作为标识
#define ROOM_MAX 100
int room_num = 0;
Room * room_thread;            //作为一个引用（中转）的实例
Room GameRoom(room_thread);           //游戏房间，全局变量，测试用，实际不能用全局变量
void * TransMessage(void *hi);             //为游戏服务的转发消息函数
void MatchClient()           //匹配玩家，达到4个玩家，就创建一个房间，然后开始游戏
{
	int JudgeData;            //判断数据包数据类型的标识符
	int LoginMark; 
	char Data1[8];
	char Data2[8];
	SOCKADDR_IN addr_Srv;
	SOCKET Match_socServer;
	char recvBuf[100];  //收到消息的缓冲区
	int client_num;         //游戏人数
	SOCKADDR_IN the_addr_Clt[player];  //每个玩家的地址
	SOCKADDR_IN in_addr;        //接入地址
	int fromlen = sizeof(SOCKADDR);
	client_num = 0;
	int version_a = 1;//low bit  
	int version_b = 1;//high bit  
	WORD versionRequest = MAKEWORD(version_a, version_b);
	WSAData wsaData;
	int err;
	//wsa startup  
	err = WSAStartup(versionRequest, &wsaData);

	if (err != 0) {
		printf("ERROR!");

	}
	//check whether the version is 1.1, if not print the error and cleanup wsa?  
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		printf("WRONG WINSOCK VERSION!");
		WSACleanup();

	}  Match_socServer = socket(AF_INET, SOCK_DGRAM, 0);
	//infomation of address, always NOT being operated directly.  

	//Struct sin_addr is  used to defind IP address,  it's a property of addr_in.  
	//It's nest three structs as S_un_b, S_un_w and S-un.(union)  
	addr_Srv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	//set protocol family  
	addr_Srv.sin_family = AF_INET;
	//set host port  
	addr_Srv.sin_port = htons(MATCH_HOST_PORT);

	//bind socket to  the host  
	bind(Match_socServer, (SOCKADDR*)&addr_Srv, sizeof(SOCKADDR));
	while (true)        //设置好套接字开始主循环接收消息
	{
		while (client_num < player)         //凑齐4人，开始游戏
		{
			recvfrom(Match_socServer, recvBuf, 100, 0, (SOCKADDR*)&in_addr, &fromlen);        //读取消息到缓冲区
			JudgeData = Analy_Str(recvBuf,Data1,Data2);                    //收到消息，解包，获取数据类型
			
		                                    //收到链接成功的消息,写入玩家地址
			if (JudgeData == 1)                //表示收到的是用户名和密码
			{
				LoginMark = Login(Data1, Data2);
				if (LoginMark == 1)         //标识登录成功
				{
					the_addr_Clt[client_num] = in_addr;             //登记IP
					client_num++;

					sendto(Match_socServer, THANKS, strlen(THANKS) + 1, 0, (SOCKADDR*)&in_addr, sizeof(SOCKADDR));
				}
			}
			
			printf("buf: %s\n", recvBuf);
			//	this->addr_Clt[client_num] = in_addr;
			/*
			if (strcmp(recvBuf, OK_STR) == 0)
			{
				the_addr_Clt[client_num] = in_addr;             //登记IP
				client_num++;
			}
			sendto(Match_socServer, THANKS, strlen(THANKS) + 1, 0, (SOCKADDR*)&in_addr, sizeof(SOCKADDR));
			*/
		}
		
		room_num++;
		GameRoom.SetClient(the_addr_Clt);                //设置房间内4玩家IP
		pthread_t TransThread;                //创建线程为此游戏房间服务
		pthread_create(&TransThread, NULL, TransMessage,0 );
	//	GameRoom.Rece_SendMe();
		//GameRoom.Rece_SendMe();          //线程终止方式为异步
		client_num = 0;
	}
}
void * TransMessage(void *hi)
{
	GameRoom.Rece_SendMe();
	return 0;
}