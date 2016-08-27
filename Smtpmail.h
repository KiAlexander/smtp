#pragma once
#ifndef __AFXWIN_H__
  #error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <list>
using namespace std;
static const char base64Char[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
extern bool isAttached;//附件有无判定量
struct FILEINFO /*用来记录文件的一些信息*/
{
	char fileName[128]; /*文件名称*/
	char filePath[256]; /*文件绝对路径*/
};
class SmtpMail
{
private:
	char SmtpSrvName[32];//服务器地址
	char Port[7];		 //端口
	char UserName[32];	 //用户名
	char Password[16];	 //密码
	char From[32];		 //发件人地址
	char To[32];		 //收件人地址
	char Subject[32];	 //主题
	char Msg[64];		 //正文
	list <FILEINFO *> listFile;
	char* Base64(char * const origSigned, unsigned origLength);
	int Talk(SOCKET sockid, const char *OkCode, char *pSend);
public:
	SmtpMail(const char* s, const char* p, const char* u, const char* w,
		const char* f, const char* t, const char* j, const char* m)
	{
		strcpy(SmtpSrvName, s);
		strcpy(Port, p);
		strcpy(UserName, u);
		strcpy(Password, w);
		strcpy(From, f);
		strcpy(To, t);
		strcpy(Subject, j);
		strcpy(Msg, m);
	}
	int SendMail(bool isAttach);
	void AddAttachment(string &filePath); //添加附件
};
