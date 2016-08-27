#pragma once
#ifndef __AFXWIN_H__
  #error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
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
extern bool isAttached;//���������ж���
struct FILEINFO /*������¼�ļ���һЩ��Ϣ*/
{
	char fileName[128]; /*�ļ�����*/
	char filePath[256]; /*�ļ�����·��*/
};
class SmtpMail
{
private:
	char SmtpSrvName[32];//��������ַ
	char Port[7];		 //�˿�
	char UserName[32];	 //�û���
	char Password[16];	 //����
	char From[32];		 //�����˵�ַ
	char To[32];		 //�ռ��˵�ַ
	char Subject[32];	 //����
	char Msg[64];		 //����
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
	void AddAttachment(string &filePath); //��Ӹ���
};
