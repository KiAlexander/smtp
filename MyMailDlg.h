
// MyMailDlg.h : ͷ�ļ�
//

#pragma once

// CMyMailDlg �Ի���
class CMyMailDlg : public CDialogEx
{
// ����
public:
	CMyMailDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYMAIL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	IPicture *m_picture; 
	OLE_XSIZE_HIMETRIC m_width; 
	OLE_YSIZE_HIMETRIC m_height;
	BOOL m_IsShow;

	int   m_old_cx, m_old_cy;
	CDWordArray   m_control_info;

	void   SetControlInfo(WORD   CtrlId);

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();//ʹ����֪
	afx_msg void OnBnClickedButton3();//�����ʼ�
	afx_msg void OnBnClickedButton2();//SMTP����
	afx_msg void OnSize(UINT nType, int cx, int cy);//�����Ի����С
	afx_msg void OnBnClickedAttach();//��Ӹ���
};
