// SMTPSET.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyMail.h"
#include "SMTPSET.h"
#include "afxdialogex.h"
extern CString Temp1;
extern CString Temp2;
// SMTPSET �Ի���

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


// SMTPSET ��Ϣ�������

//�ӶԻ���ȷ����ť
void SMTPSET::OnBnClickedBtok()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_EDIT7)->GetWindowText(Temp1);
	GetDlgItem(IDC_EDIT8)->GetWindowText(Temp2);
	if (Temp1.IsEmpty())
		MessageBox("����д��������ַ��", "��ʾ", MB_OK);
	if (Temp2.IsEmpty())
		MessageBox("����д�˿ڣ�", "��ʾ", MB_OK);
	if (!Temp1.IsEmpty() && !Temp2.IsEmpty())
	{
		OnCancel();
	}
}

//�ӶԻ������ť
void SMTPSET::OnBnClickedBtclear()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetDlgItemText(IDC_EDIT7, "");
	SetDlgItemText(IDC_EDIT8, "");
}


