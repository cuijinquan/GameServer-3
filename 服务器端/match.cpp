//��cpp�ļ���дƥ����� ��Ȼ����һ����Ϸ���䣬�������߳�Ϊ�������Ȼ��ص���ƥ����еȴ��������
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
#define MATCH_HOST_PORT 8081    //ƥ�������׽��ֺ���Ϸ�õ��׽��ֲ���ͬһ����ƥ��ϵͳ�õ�8081����Ϸϵͳ�õ�8080
#define OK_STR "Conn_Ok"
#define THANKS "���Ѿ��ɹ����ӷ�����"
#define MATCH_STR "NEED_MATCH"  //�ȴ�ƥ���ַ�������Ϊ��ʶ
#define ROOM_MAX 100
int room_num = 0;
Room * room_thread;            //��Ϊһ�����ã���ת����ʵ��
Room GameRoom(room_thread);           //��Ϸ���䣬ȫ�ֱ����������ã�ʵ�ʲ�����ȫ�ֱ���
void * TransMessage(void *hi);             //Ϊ��Ϸ�����ת����Ϣ����
void MatchClient()           //ƥ����ң��ﵽ4����ң��ʹ���һ�����䣬Ȼ��ʼ��Ϸ
{
	int JudgeData;            //�ж����ݰ��������͵ı�ʶ��
	int LoginMark; 
	char Data1[8];
	char Data2[8];
	SOCKADDR_IN addr_Srv;
	SOCKET Match_socServer;
	char recvBuf[100];  //�յ���Ϣ�Ļ�����
	int client_num;         //��Ϸ����
	SOCKADDR_IN the_addr_Clt[player];  //ÿ����ҵĵ�ַ
	SOCKADDR_IN in_addr;        //�����ַ
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
	while (true)        //���ú��׽��ֿ�ʼ��ѭ��������Ϣ
	{
		while (client_num < player)         //����4�ˣ���ʼ��Ϸ
		{
			recvfrom(Match_socServer, recvBuf, 100, 0, (SOCKADDR*)&in_addr, &fromlen);        //��ȡ��Ϣ��������
			JudgeData = Analy_Str(recvBuf,Data1,Data2);                    //�յ���Ϣ���������ȡ��������
			
		                                    //�յ����ӳɹ�����Ϣ,д����ҵ�ַ
			if (JudgeData == 1)                //��ʾ�յ������û���������
			{
				LoginMark = Login(Data1, Data2);
				if (LoginMark == 1)         //��ʶ��¼�ɹ�
				{
					the_addr_Clt[client_num] = in_addr;             //�Ǽ�IP
					client_num++;

					sendto(Match_socServer, THANKS, strlen(THANKS) + 1, 0, (SOCKADDR*)&in_addr, sizeof(SOCKADDR));
				}
			}
			
			printf("buf: %s\n", recvBuf);
			//	this->addr_Clt[client_num] = in_addr;
			/*
			if (strcmp(recvBuf, OK_STR) == 0)
			{
				the_addr_Clt[client_num] = in_addr;             //�Ǽ�IP
				client_num++;
			}
			sendto(Match_socServer, THANKS, strlen(THANKS) + 1, 0, (SOCKADDR*)&in_addr, sizeof(SOCKADDR));
			*/
		}
		
		room_num++;
		GameRoom.SetClient(the_addr_Clt);                //���÷�����4���IP
		pthread_t TransThread;                //�����߳�Ϊ����Ϸ�������
		pthread_create(&TransThread, NULL, TransMessage,0 );
	//	GameRoom.Rece_SendMe();
		//GameRoom.Rece_SendMe();          //�߳���ֹ��ʽΪ�첽
		client_num = 0;
	}
}
void * TransMessage(void *hi)
{
	GameRoom.Rece_SendMe();
	return 0;
}