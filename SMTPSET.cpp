// SMTPSET.cpp : 实现文件
//

#include "stdafx.h"
#include "MyMail.h"
#include "SMTPSET.h"
#include "afxdialogex.h"
extern CString Temp1;
extern CString Temp2;
// SMTPSET 对话框

IMPLEMENT_DYNAMIC(SMTPSET, CDialogEx)

SMTPSET::SMTPSET(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SMTPSET, pParent)
{
}

BOOL SMTPSET::OnInitDialog()
{
	if (Temp1.IsEmpty() && Temp2.IsEmpty())
	{
		GetDlgItem(IDC_EDIT7)->SetWindowText("smtp.163.com");
		GetDlgItem(IDC_EDIT8)->SetWindowText("25");
	}
	else 
	{
		GetDlgItem(IDC_EDIT7)->SetWindowText(Temp1);
		GetDlgItem(IDC_EDIT8)->SetWindowText(Temp2);
	}
	return true;
}


SMTPSET::~SMTPSET()
{
}

void SMTPSET::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SMTPSET, CDialogEx)
	ON_BN_CLICKED(ID_BTOK, &SMTPSET::OnBnClickedBtok)
	ON_BN_CLICKED(IDC_BTCLEAR, &SMTPSET::OnBnClickedBtclear)
END_MESSAGE_MAP()


// SMTPSET 消息处理程序

//子对话框确定按钮
void SMTPSET::OnBnClickedBtok()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT7)->GetWindowText(Temp1);
	GetDlgItem(IDC_EDIT8)->GetWindowText(Temp2);
	if (Temp1.IsEmpty())
		MessageBox("请填写服务器地址！", "提示", MB_OK);
	if (Temp2.IsEmpty())
		MessageBox("请填写端口！", "提示", MB_OK);
	if (!Temp1.IsEmpty() && !Temp2.IsEmpty())
	{
		OnCancel();
	}
}

//子对话框重填按钮
void SMTPSET::OnBnClickedBtclear()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT7, "");
	SetDlgItemText(IDC_EDIT8, "");
}


