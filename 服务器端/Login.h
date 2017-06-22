#include "Winsock2.h"  
#ifndef Login_H
#define Login_H

int  Login(char *UserName,char *PassWord);       //登录函数
int Select(char *UserName, char *Result);        //数据库查询函数
int Analy_Str(char *Recv, char *ReData1, char *ReData2);                //分析收到的数据包,分辨是登录信息还是游戏操作，还是结束后的成绩。
#endif

/*
数据拆包：以&作为数据元组分隔符号，UserName或者PassWord作为标签，标签与数据之间用#分开
数据规范为：UserName 为登录名，password为密码；Operate为操作符，EndGame为游戏结束标签，StartGame为开始标签
为方便识别，标签和数据项都截取为7字符，加上特殊截断字符共8字符。
封装协议为：UserName和PassWord在一个数据包内。除此以外，所有其他类型数据包都为单个数据。
*/