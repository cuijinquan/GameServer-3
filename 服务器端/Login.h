#include "Winsock2.h"  
#ifndef Login_H
#define Login_H

int  Login(char *UserName,char *PassWord);       //��¼����
int Select(char *UserName, char *Result);        //���ݿ��ѯ����
int Analy_Str(char *Recv, char *ReData1, char *ReData2);                //�����յ������ݰ�,�ֱ��ǵ�¼��Ϣ������Ϸ���������ǽ�����ĳɼ���
#endif

/*
���ݲ������&��Ϊ����Ԫ��ָ����ţ�UserName����PassWord��Ϊ��ǩ����ǩ������֮����#�ֿ�
���ݹ淶Ϊ��UserName Ϊ��¼����passwordΪ���룻OperateΪ��������EndGameΪ��Ϸ������ǩ��StartGameΪ��ʼ��ǩ
Ϊ����ʶ�𣬱�ǩ���������ȡΪ7�ַ�����������ض��ַ���8�ַ���
��װЭ��Ϊ��UserName��PassWord��һ�����ݰ��ڡ��������⣬���������������ݰ���Ϊ�������ݡ�
*/