#pragma once
// SMTPSET �Ի���

class SMTPSET : public CDialogEx
{
	DECLARE_DYNAMIC(SMTPSET)

public:
	SMTPSET(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SMTPSET();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SMTPSET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtok();//ȷ��
	afx_msg void OnBnClickedBtclear();//����



};

