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
#import "C:\Program Files\Common Files\System\ADO\msado15.dll"  no_namespace rename("EOF", "EndOfFile")  //�����ļ�
#define NULL 0
int Select(char *UserName, char *Result)
{
	char temp[100];       //��ʱ���������ڴ洢��ѯ�Ĳ��ֽ��
	_RecordsetPtr pRst;
	char sqlcommand[1024];
	strcpy_s(sqlcommand, "select * from table1 where UserName='");
	strcat_s(sqlcommand, UserName);
	strcat_s(sqlcommand, "';");
	//_RecordsetPtr pRst;
	_ConnectionPtr  sqlSp;
	_bstr_t strConnect = "Provider = SQLOLEDB.1; Integrated Security = SSPI; Persist Security Info = False; Initial Catalog = MyGame; Data Source = .";//�������ݿ��ַ���
																																					 //��ʼ��COM��  
	::CoInitialize(NULL);
	//���һ��ָ��Connection�����ָ��m_pConnection  
	_ConnectionPtr m_pConnection(__uuidof(Connection));
	//����Connection����  
	if (!(m_pConnection.CreateInstance(__uuidof(Connection))))
	{
		cout << "����Connection����ʱ����\n";
	}

	try
	{
		//�������ݿ�  
		m_pConnection->Open(strConnect, "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		cout << "�������ݿ�ʱ����\n ";
		cout << e.Description();
		cin.get();
		return 1;
	}
	try                                   //����ѯ���Ľ���洢�ڼ�¼��
	{
		pRst = m_pConnection->Execute(sqlcommand, NULL, adCmdText);
		//ִ��SQL�� select * from Table_BB 
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

		//�洢���������������ʾ�������
		for (int i = 0; i< pRst->Fields->GetCount(); i++)
		{
			cout << pRst->Fields->GetItem(_variant_t((long)i))->Name << " ";
			column_name.push_back(pRst->Fields->GetItem(_variant_t((long)i))->Name);
		}
		cout << endl;

		//�Ա���б�������,��ʾ����ÿһ�е�����
		while (!pRst->EndOfFile)
		{
			vector<_bstr_t>::iterator iter = column_name.begin();
			for (iter; iter != column_name.end(); iter++)
			{
				if (pRst->GetCollect(*iter).vt != VT_NULL)
				{
					cout << (_bstr_t)pRst->GetCollect(*iter) << " ";
					try {
						strcpy_s(temp, (_bstr_t)pRst->GetCollect(*iter));        //�ȰѲ鵽��һ������洢��temp
																				 //strcat_s(result, sizeof((char *)((_bstr_t)pRst->GetCollect(*iter))), (char *)((_bstr_t)pRst->GetCollect(*iter)));
						strcat_s(Result, sizeof(temp), temp);       //��temp�Ľ�����ӵ�result
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
	printf("�ɹ�\n");

	//�ͷų���ռ�õ�COM ��Դ  
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
		return 0;                           //����0����������֤�ɹ�
	}
	else
		return 1;                             //����1������֤ʧ��
}

int Analy_Str(char *Recv,char *ReData1,char *ReData2) //recv��ʾ���յ������������ݰ���Redata1��2�ֱ��ʾ�������ص����ݡ�
{
	char temp[4][8];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			temp[i][j] = Recv[i * 8 + j];           //�����ݰ���ȡ��4�Σ�д����������
		}
	}
	if (strcmp(temp[0], "&UserNam") == 0)
	{  
		strcpy_s(ReData1,8,temp[1]);
		strcpy_s(ReData2, 8, temp[3]);   //���˺������ȡ����
		return 1;                       //1��ʾ�յ������û���

	}
	if (strcmp(temp[0], "&Operate") == 0)
	{
		strcpy_s(ReData1, 8, temp[1]);
		ReData2 = "\0";
		return 2;                       //2��ʾ�յ����ǲ�����
	}
	if (strcmp(temp[0], "&StartGa") == 0)
	{
		strcpy_s(ReData1, 8, temp[1]);
		ReData2 = "\0";
		return 3;                       //3��ʾ�յ����ǿ�ʼ��Ϸ�ı�־
	}
	if (strcmp(temp[0], "&EndGam") == 0)
	{
		strcpy_s(ReData1, 8, temp[1]);
		ReData2 = "\0";
		return 4;                       //4��ʾ�յ����ǽ�����Ϸ��־
	}
}