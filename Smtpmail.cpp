#include "stdafx.h"
#include "Smtpmail.h"
#include "MyMailDlg.h"

#ifdef _DEBUG
  #define new DEBUG_NEW
#endif

CString strMsg;//提示或者错误对话框的内容暂存量
const int buflen = 1024;//缓冲数组长度
const int MAX_FILE_LEN = 6000;//最大文件长度
//---------------------------------------------------------------------
int SmtpMail::SendMail(bool isAttach)
{
	char buf[buflen];//缓冲量
	int userlen, passlen;//用户名、密码长度
	//(1)初始化网络环境
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
	{
		strMsg.Format("WSAStartup() error : %d\n", GetLastError());
		MessageBox(NULL,strMsg, "Error", MB_OK);
		return 1;
	}
	//(2)创建套接字
	SOCKET sockid;
	if ((sockid = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		strMsg.Format("socket() error : %d\n", GetLastError());
		MessageBox(NULL, strMsg, "Error", MB_OK);
		WSACleanup();
		return 1;
	}
	//(3)得到SMTP服务器IP
	struct hostent *phostent = gethostbyname(SmtpSrvName);
	struct sockaddr_in addr;
	CopyMemory(&addr.sin_addr.S_un.S_addr,
		phostent->h_addr_list[0],
		sizeof(addr.sin_addr.S_un.S_addr));
	struct in_addr srvaddr;
	CopyMemory(&srvaddr, &addr.sin_addr.S_un.S_addr, sizeof(struct in_addr));
	strMsg.Format("Smtp server name is %s\n", SmtpSrvName);
	MessageBox(NULL, strMsg, "Tip", MB_OK);
	strMsg.Format("Smtp server ip is %s\n", inet_ntoa(srvaddr));
	MessageBox(NULL, strMsg, "Tip", MB_OK);
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(Port));
	ZeroMemory(&addr.sin_zero, 8);
	//(4)连接服务器
	if (connect(sockid, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
	{
		strMsg.Format("connect() error : please check your Internet\n");
		MessageBox(NULL, strMsg, "Error", MB_OK);
		goto STOP;
	}
	//(5)按照SMTP收发信息
	if (Talk(sockid, "220", "HELO asdf"))//发送HELO命令，告知服务器
	{
		goto STOP;
	}
	if (Talk(sockid, "250", "AUTH LOGIN"))//启动认证程序
	{
		goto STOP;
	}
	ZeroMemory(buf, buflen);
	userlen = lstrlen(UserName);//用户名长度
	passlen = lstrlen(Password);//密码长度

	//对用户名进行Base64的编码
	char* buffer0 = Base64(UserName, userlen);

	if (Talk(sockid, "334", buffer0))
	{
		goto STOP;
	}
	
	//对密码进行Base64的编码
	char* buffer1 = Base64(Password, passlen);

	if (Talk(sockid, "334", buffer1))
	{
		goto STOP;
	}

	//告知服务器发送人地址
	wsprintf(buf, "MAIL FROM:<%s>", From);
	if (Talk(sockid, "235", buf))
	{
		goto STOP;
	}
	ZeroMemory(buf, buflen);
	//告知服务器接收人地址
	wsprintf(buf, "RCPT TO:<%s>", To);
	if (Talk(sockid, "250", buf))
	{
		goto STOP;
	}
	//协商结束，正式开始接收数据
	if (Talk(sockid, "250", "DATA"))
	{
		goto STOP;
	}
	ZeroMemory(buf, buflen);

	//发送邮件头信息
	wsprintf(buf, "TO: %s\r\nFROM: %s\r\nSUBJECT: %s\r\nMIME - Version: 1.0\r\nContent-Type: multipart/mixed;boundary=qwertyuiop\r\n\r\n",
		To, From, Subject);
	send(sockid, buf, strlen(buf), 0);
	ZeroMemory(buf, buflen);

	//发送正文信息
	wsprintf(buf, "--qwertyuiop\r\nContent-Type: text/plain;charset=\"gb2312\"\r\nContent-Transfer-Encoding: quoted-printable\r\n\r\n%s\r\n\r\n", Msg);
	send(sockid, buf, strlen(buf), 0);
	ZeroMemory(buf, buflen);

	//发送附件信息
	if(isAttach)
	{
		list<FILEINFO *>::iterator pIter = listFile.begin();//方便下次将代码拓展成发多个附件
		strMsg.Format("Attachment is sending ~~~~~\r\nPlease be patient!\n");
		MessageBox(NULL, strMsg, "Tip", MB_OK);

		ifstream ifs((*pIter)->filePath, ios::in | ios::binary);
		if (false == ifs.is_open())//文件打开失败则返回
		{
			return 1;
		}
		char fileBuff[MAX_FILE_LEN];//存放文件信息
		char chSendBuff[MAX_FILE_LEN];//存放编码后的文件信息
		memset(fileBuff, 0, sizeof(fileBuff));

		/*文件使用base64加密传送*/
		ifs.read(fileBuff, MAX_FILE_LEN);//读取文件
		memcpy(chSendBuff, Base64(fileBuff, strlen(fileBuff)), sizeof(fileBuff));

		//发送附件信息
		if ((strlen(chSendBuff) + 162) < buflen)
		{
			wsprintf(buf, "--qwertyuiop\r\nContent-Type: application/octet-stream;\r\n name=\" %s\"\r\nContent-Transfer-Encoding: base64\r\nContent-Disposition: attachment;\r\n filename=\" %s\"\r\n\r\n%s\r\n\r\n",
				(*pIter)->fileName, (*pIter)->fileName, chSendBuff);
			send(sockid, buf, strlen(buf), 0);
			ZeroMemory(buf, buflen);
		}
		else
		{
			strMsg.Format("Attachment is too large to be sent\n");
			MessageBox(NULL, strMsg, "Error", MB_OK);
			return 1;
		}
	
	}

	//发送结束符
	wsprintf(buf, "--qwertyuiop--\r\n.\r\n");
	if (Talk(sockid, "354", buf))
	{
		goto STOP;
	}

	//结束此次发送，发送QUIT命令断开连接
	if (Talk(sockid, "250", "QUIT"))
	{
		goto STOP;
	}
	if (Talk(sockid, "221", ""))
	{
		goto STOP;
	}
	else
	{
		closesocket(sockid);
		WSACleanup();
		return 0;
	}
STOP://(6)关闭socket，释放网络资源
	closesocket(sockid);
	WSACleanup();
	return 1;
}
//---------------------------------------------------------------------
int SmtpMail::Talk(SOCKET sockid, const char *OkCode, char *pSend)
{
	const int buflen = 256;
	char buf[buflen];
	ZeroMemory(buf, buflen);
	//接收返回信息
	if (recv(sockid, buf, buflen, 0) == SOCKET_ERROR)
	{
		strMsg.Format("recv() error : %d\n", GetLastError());
		MessageBox(NULL, strMsg, "Error", MB_OK);
	    return 1;
	}

	if (strstr(buf, OkCode) == NULL)
	{

		if ("235" == OkCode)//用户名或密码错误
		{
			strMsg.Format("Error: username error or password error!");
			MessageBox(NULL, strMsg, "Error", MB_OK);
			return 1;
		}
		else if("250"== OkCode)//收发地址错误
		{
			strMsg.Format("Error: sendaddress error or receiveaddress error!");
			MessageBox(NULL, strMsg, "Error", MB_OK);
			return 1;
		}
		else if ("354" == OkCode)//正文发送错误
		{
			strMsg.Format("Error: sended unsuccessfully");
			MessageBox(NULL, strMsg, "Error", MB_OK);
			return 1;
		}

	}
	//发送命令
	if (lstrlen(pSend))
	{
		ZeroMemory(buf, buflen);
		wsprintf(buf, "%s\r\n", pSend);
		if (send(sockid, buf, lstrlen(buf), 0) == SOCKET_ERROR)
		{
			strMsg.Format("send() error : %d\n", GetLastError());
			MessageBox(NULL, strMsg, "Error", MB_OK);
			return 1;
		}
	}
	return 0;
}
//Base64编码，origSigned：未编码的二进制代码，origLength：未编码的二进制代码的长度

char* SmtpMail::Base64(char* const origSigned, unsigned origLength)
{
	unsigned char const* orig = (unsigned char const*)origSigned; // in case any input bytes have the MSB set  
	if (orig == NULL) return NULL;

	unsigned const numOrig24BitValues = origLength / 3;
	bool havePadding = origLength > numOrig24BitValues * 3;
	bool havePadding2 = origLength == numOrig24BitValues * 3 + 2;
	unsigned const numResultBytes = 4 * (numOrig24BitValues + havePadding);
	char* result = new char[numResultBytes + 3]; // allow for trailing '/0'  

	// Map each full group of 3 input bytes into 4 output base-64 characters:  
	unsigned i;
	for (i = 0; i < numOrig24BitValues; ++i)
	{
		result[4 * i + 0] = base64Char[(orig[3 * i] >> 2) & 0x3F];
		result[4 * i + 1] = base64Char[(((orig[3 * i] & 0x3) << 4) | (orig[3 * i + 1] >> 4)) & 0x3F];
		result[4 * i + 2] = base64Char[((orig[3 * i + 1] << 2) | (orig[3 * i + 2] >> 6)) & 0x3F];
		result[4 * i + 3] = base64Char[orig[3 * i + 2] & 0x3F];
	}

	// Now, take padding into account.  (Note: i == numOrig24BitValues)  
	if (havePadding)
	{
		result[4 * i + 0] = base64Char[(orig[3 * i] >> 2) & 0x3F];
		if (havePadding2)
		{
			result[4 * i + 1] = base64Char[(((orig[3 * i] & 0x3) << 4) | (orig[3 * i + 1] >> 4)) & 0x3F];
			result[4 * i + 2] = base64Char[(orig[3 * i + 1] << 2) & 0x3F];
		}
		else
		{
			result[4 * i + 1] = base64Char[((orig[3 * i] & 0x3) << 4) & 0x3F];
			result[4 * i + 2] = '=';
		}
		result[4 * i + 3] = '=';
	}

	result[numResultBytes] = '\0';
	return result;
}

//添加附件  
void SmtpMail::AddAttachment(string &filePath) 
{
	FILEINFO *pFile = new FILEINFO;
	strcpy_s(pFile->filePath, filePath.c_str());
	const char *p = filePath.c_str();
	strcpy_s(pFile->fileName, p + filePath.find_last_of("\\") + 1);
	listFile.push_back(pFile);
}