#pragma once  
#define WIN32_LEAN_AND_MEAN 
#include <stdio.h>  
#include <tchar.h>  
#include<iostream>
#include <vector>
#include "stdio.h"  
#include "Winsock2.h"  
#include <iostream>  
#include "string"  
#pragma comment(lib,"ws2_32.lib")
using namespace std;
#define LENGTH 1024
#import "C:\Program Files\Common Files\System\ADO\msado15.dll"  no_namespace rename("EOF", "EndOfFile")  //置入文件
#define NULL 0
int Select(char *UserName, char *Result)
{
	char temp[100];       //临时变量，用于存储查询的部分结果
	_RecordsetPtr pRst;
	char sqlcommand[1024];
	strcpy_s(sqlcommand, "select * from table1 where UserName='");
	strcat_s(sqlcommand, UserName);
	strcat_s(sqlcommand, "';");
	//_RecordsetPtr pRst;
	_ConnectionPtr  sqlSp;
	_bstr_t strConnect = "Provider = SQLOLEDB.1; Integrated Security = SSPI; Persist Security Info = False; Initial Catalog = MyGame; Data Source = .";//连接数据库字符串
																																					 //初始化COM库  
	::CoInitialize(NULL);
	//添加一个指向Connection对象的指针m_pConnection  
	_ConnectionPtr m_pConnection(__uuidof(Connection));
	//创键Connection对象  
	if (!(m_pConnection.CreateInstance(__uuidof(Connection))))
	{
		cout << "创键Connection对象时出错\n";
	}

	try
	{
		//连接数据库  
		m_pConnection->Open(strConnect, "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		cout << "连接数据库时出错\n ";
		cout << e.Description();
		cin.get();
		return 1;
	}
	try                                   //将查询到的结果存储在记录集
	{
		pRst = m_pConnection->Execute(sqlcommand, NULL, adCmdText);
		//执行SQL： select * from Table_BB 
		if (!pRst->BOF)
		{
			pRst->MoveFirst();
		}
		else
		{
			cout << "Data is empty!" << endl;
			return 0;
		}
		vector<_bstr_t> column_name;

		//存储表的所有列名，显示表的列名
		for (int i = 0; i< pRst->Fields->GetCount(); i++)
		{
			cout << pRst->Fields->GetItem(_variant_t((long)i))->Name << " ";
			column_name.push_back(pRst->Fields->GetItem(_variant_t((long)i))->Name);
		}
		cout << endl;

		//对表进行遍历访问,显示表中每一行的内容
		while (!pRst->EndOfFile)
		{
			vector<_bstr_t>::iterator iter = column_name.begin();
			for (iter; iter != column_name.end(); iter++)
			{
				if (pRst->GetCollect(*iter).vt != VT_NULL)
				{
					cout << (_bstr_t)pRst->GetCollect(*iter) << " ";
					try {
						strcpy_s(temp, (_bstr_t)pRst->GetCollect(*iter));        //先把查到的一个结果存储在temp
																				 //strcat_s(result, sizeof((char *)((_bstr_t)pRst->GetCollect(*iter))), (char *)((_bstr_t)pRst->GetCollect(*iter)));
						strcat_s(Result, sizeof(temp), temp);       //把temp的结果连接到result
					}
					catch (_com_error me)
					{
						cout << endl << me.Description() << endl;
					}

				}
				else
				{
					cout << "NULL" << endl;
				}

			}

			pRst->MoveNext();
			cout << endl;
		}
	}
	catch (_com_error &e)

	{
		cout << e.Description() << endl;
		//cout<<e.HelpFile()<<endl;
		return 0;
	}
	m_pConnection->Close();
	printf("成功\n");

	//释放程序占用的COM 资源  
	::CoUninitialize();

	return pRst;
}
;
int  Login(char *UserName, char *PassWord)
{
	char Result[LENGTH];
	Select(UserName, Result);
	if (strcmp(PassWord, Result) == 0)
	{
		return 0;                           //返回0代表密码验证成功
	}
	else
		return 1;                             //返回1代表验证失败
}

int Analy_Str(char *Recv,char *ReData1,char *ReData2) //recv表示接收到待分析的数据包，Redata1和2分别表示分析返回的数据。
{
	char temp[4][8];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			temp[i][j] = Recv[i * 8 + j];           //把数据包截取成4段，写进分析数组
		}
	}
	if (strcmp(temp[0], "&UserNam") == 0)
	{  
		strcpy_s(ReData1,8,temp[1]);
		strcpy_s(ReData2, 8, temp[3]);   //把账号密码截取出来
		return 1;                       //1表示收到的是用户名

	}
	if (strcmp(temp[0], "&Operate") == 0)
	{
		strcpy_s(ReData1, 8, temp[1]);
		ReData2 = "\0";
		return 2;                       //2表示收到的是操作符
	}
	if (strcmp(temp[0], "&StartGa") == 0)
	{
		strcpy_s(ReData1, 8, temp[1]);
		ReData2 = "\0";
		return 3;                       //3表示收到的是开始游戏的标志
	}
	if (strcmp(temp[0], "&EndGam") == 0)
	{
		strcpy_s(ReData1, 8, temp[1]);
		ReData2 = "\0";
		return 4;                       //4表示收到的是结束游戏标志
	}
}