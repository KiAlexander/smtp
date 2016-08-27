
// MyMailDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyMail.h"
#include "MyMailDlg.h"
#include "afxdialogex.h"
#include "Smtpmail.h"
#include "SMTPSET.h"
#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString Temp1;//��������ַ������
CString Temp2;//�˿ڻ�����
CString strFilePath;//����·��
bool isAttached;//�Ƿ��и���
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyMailDlg �Ի���

void   CMyMailDlg::SetControlInfo(WORD   CtrlId)//��ӿؼ�ID�����ڵ����Ի����С
{
	m_control_info.Add(CtrlId);

}

CMyMailDlg::CMyMailDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MYMAIL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);//����ͼ��
	m_old_cx = m_old_cy = 0;
}

void CMyMailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyMailDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMyMailDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyMailDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyMailDlg::OnBnClickedButton2)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_ATTACH, &CMyMailDlg::OnBnClickedAttach)
END_MESSAGE_MAP()


// CMyMailDlg ��Ϣ�������

BOOL CMyMailDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);//δ����SMTP֮ǰ���������ʼ�


	//��ӿؼ�����
	SetControlInfo(IDC_SET);
	SetControlInfo(IDC_NAME);
	SetControlInfo(IDC_EDIT1);
	SetControlInfo(IDC_PASSWORD);
	SetControlInfo(IDC_EDIT2);
	SetControlInfo(IDC_SENDADDR);
	SetControlInfo(IDC_EDIT3);
	SetControlInfo(IDC_ACCEPTADDR);
	SetControlInfo(IDC_EDIT4);
	SetControlInfo(IDC_SUBJECT);
	SetControlInfo(IDC_EDIT5);
	SetControlInfo(IDC_BUTTON1);
	SetControlInfo(IDC_BUTTON2);
	SetControlInfo(IDC_BUTTON3);
	SetControlInfo(IDC_ATTACH);
	SetControlInfo(IDC_CONTENT);
	SetControlInfo(IDC_EDIT6);
	SetControlInfo(IDC_STATIC0);


	//��������
	GetDlgItem(IDC_EDIT1)->SetWindowText("a_mail_test@163.com");//�û���
	GetDlgItem(IDC_EDIT2)->SetWindowText("bupt123");//��Ȩ��
	GetDlgItem(IDC_EDIT3)->SetWindowText("a_mail_test@163.com");//���͵�ַ
	GetDlgItem(IDC_EDIT4)->SetWindowText("a_mail_test@163.com");//j���յ�ַ
	GetDlgItem(IDC_EDIT5)->SetWindowText("hello");//����
	GetDlgItem(IDC_EDIT6)->SetWindowText("�����ɹ���");//����

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMyMailDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMyMailDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//��ӶԻ��򱳾�ͼƬ
		CPaintDC dc(this);
		CFile m_file("MyMail.jpg", CFile::modeRead);
		//��ȡ�ļ�����
		DWORD m_filelen = m_file.GetLength();
		//�ڶ��Ϸ���ռ�
		HGLOBAL m_hglobal = GlobalAlloc(GMEM_MOVEABLE, m_filelen);

		LPVOID pvdata = NULL;
		//�����ѿռ�,��ȡָ��ѿռ��ָ��
		pvdata = GlobalLock(m_hglobal);

		//���ļ����ݶ���������
		m_file.Read(pvdata, m_filelen);

		IStream*  m_stream;

		GlobalUnlock(m_hglobal);

		//�ڶ��д���������
		CreateStreamOnHGlobal(m_hglobal, TRUE, &m_stream);

		//����������ͼ��
		OleLoadPicture(m_stream, m_filelen, TRUE, IID_IPicture, (LPVOID*)&m_picture);
		m_stream->Release();

		m_picture->get_Width(&m_width);// ��ߣ�MM_HIMETRIC ģʽ����λ��0.01����
		m_picture->get_Height(&m_height);
		m_IsShow = TRUE;
		m_file.Close();

		if (m_IsShow == TRUE) {
			CRect rect;
			GetClientRect(rect);
			int nW, nH;
			nW = (int)(rect.Width());
			nH = (int)(rect.Height());
			m_picture->Render(dc, 0, 0, nW, nH, 0, m_height, m_width, -m_height, &rect);
		}
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMyMailDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//ʹ����֪
void CMyMailDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	//��ȡԤ���趨ʹ����֪�ı�����
	char buffer[1023] = { 0 };
	ifstream  Guidance("Guidance.txt");
	if(!Guidance.fail()) //����򿪳ɹ�
	{ 
		int i = 0;
		while (!Guidance.eof())//����ļ�����û�н���
		{
			Guidance.get(buffer[i]);//��ȡ�ı�����
			i++;
		}
		Guidance.close();//�ر��ı��ļ�
	}
	//������ʾʹ����֪���ݵĶԻ���
	MessageBox(buffer, "ʹ��˵��",MB_OK);
}

//�����ʼ�
void CMyMailDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString buffer;//�ݴ�༭�����ݵ�ֵ

	if (Temp1.IsEmpty())//����������ַΪ���򵯳���ʾ�Ի���
	{
		MessageBox("����д��������ַ��", "��ʾ", MB_OK);
		return;
	}

	if (Temp2.IsEmpty())//���˿�Ϊ���򵯳���ʾ�Ի���
	{
		MessageBox("����д�˿ڣ�", "��ʾ", MB_OK);
		return;
	}

	//��ȡ��������ַ�Ͷ˿ں�
	char* SmtpSrvName = Temp1.GetBuffer(Temp1.GetLength());
	char* Port = Temp2.GetBuffer(Temp2.GetLength());


	GetDlgItem(IDC_EDIT1)->GetWindowText(buffer);
	//���û���Ϊ���򵯳���ʾ�Ի���
	if (buffer.IsEmpty())
	{
		MessageBox("����д�û�����", "��ʾ", MB_OK);
		return;
	}
	char UserName[32];//��ȡ�û���
	strcpy(UserName, buffer.GetBuffer(buffer.GetLength()));
	buffer.ReleaseBuffer();

	GetDlgItem(IDC_EDIT2)->GetWindowText(buffer);
	//������Ϊ���򵯳���ʾ�Ի���
	if (buffer.IsEmpty())
	{
		MessageBox("����д���룡", "��ʾ", MB_OK);
		return;
	}
	char Password[16];//��ȡ����
	strcpy(Password, buffer.GetBuffer(buffer.GetLength()));
	buffer.ReleaseBuffer();

	GetDlgItem(IDC_EDIT3)->GetWindowText(buffer);
	//�������˵�ַΪ���򵯳���ʾ�Ի���
	if (buffer.IsEmpty())
	{
		MessageBox("����д�����˵�ַ��", "��ʾ", MB_OK);
		return;
	}
	char From[32];//��ȡ�����˵�ַ
	strcpy(From, buffer.GetBuffer(buffer.GetLength()));
	buffer.ReleaseBuffer();

	GetDlgItem(IDC_EDIT4)->GetWindowText(buffer);
	//���ռ��˵�ַΪ���򵯳���ʾ�Ի���
	if (buffer.IsEmpty())
	{
		MessageBox("����д�ռ��˵�ַ��", "��ʾ", MB_OK);
		return;
	}
	char To[32];//��ȡ�ռ��˵�ַ
	strcpy(To, buffer.GetBuffer(buffer.GetLength()));
	buffer.ReleaseBuffer();

	GetDlgItem(IDC_EDIT5)->GetWindowText(buffer);
	//������Ϊ���򵯳���ʾ�Ի���
	if (buffer.IsEmpty())
	{
		MessageBox("����д���⣡", "��ʾ", MB_OK);
		return;
	}
	char Subject[32];//��ȡ����
	strcpy(Subject, buffer.GetBuffer(buffer.GetLength()));
	buffer.ReleaseBuffer();

	GetDlgItem(IDC_EDIT6)->GetWindowText(buffer);
	//���ʼ�����Ϊ���򵯳���ʾ�Ի���
	if (buffer.IsEmpty())
	{
		MessageBox("����д�ʼ����ݣ�", "��ʾ", MB_OK);
		return;
	}
	char Msg[64];//��ȡ�ʼ�����
	strcpy(Msg, buffer.GetBuffer(buffer.GetLength()));
	buffer.ReleaseBuffer();

	SmtpMail mymail(SmtpSrvName, Port, UserName, Password,
		From, To, Subject, Msg);

	

	if (isAttached)//����и��������÷�����Ϣ
	{
		string filePath(strFilePath.GetBuffer(0));
		mymail.AddAttachment(filePath);//��Ӹ���
		
    }

	if (NULL == mymail.SendMail(isAttached))//�ʼ����ͳɹ��򵯳���ʾ
	{
		MessageBox("Mailed successfully!", "��ʾ", MB_OK);
		isAttached = false;//��ʼ�������ж������������ٴη���
	}
	else//ʧ����ʾ
		MessageBox("Mailed unsuccessfully!", "����", MB_OK);

}

//smtp����
void CMyMailDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SMTPSET Dlg;
	Dlg.DoModal();//�����ӶԻ���
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);//ʹ�����ʼ���ť����
}

//�Ի����С�ı���Ӧ����
void CMyMailDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������


	if (cx == 0 || cy == 0)
	{
		cx = 800;
		cy = 600;
	}
	float   dx_percent = (m_old_cx == 0) ? 1 : (float)((float)cx / (float)m_old_cx);
	float   dy_percent = (m_old_cy == 0) ? 1 : (float)((float)cy / (float)m_old_cy);

	if (m_old_cx)
	{
		CRect   WndRect;
		CWnd   *pWnd;
		for (int i = 0; i < m_control_info.GetSize(); i++)
		{
			pWnd = GetDlgItem(m_control_info[i]);
			if (!pWnd)
			{
				TRACE("Control   ID   -  %d   NOT   FOUND!!\n ", m_control_info[i]);


				continue;
			}

			pWnd->GetWindowRect(&WndRect);
			ScreenToClient(&WndRect);

			WndRect.left = (int)(WndRect.left*dx_percent);
			WndRect.right = (int)(WndRect.right*   dx_percent);
			WndRect.bottom = (int)(WndRect.bottom*dy_percent);
			WndRect.top = (int)(WndRect.top*dy_percent);

			pWnd->MoveWindow(&WndRect);
		}

	}
	m_old_cx = cx;
	m_old_cy = cy;
}

//��Ӹ���
void CMyMailDlg::OnBnClickedAttach()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ���ù�����   
	strFilePath.Empty();
	TCHAR szFilter[] = _T("�ı��ļ�(*.txt)|*.txt|�����ļ�(*.*)|*.*||");
    // ������ļ��Ի���   
    CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);
  	 
    // ��ʾ���ļ��Ի���   
    if (IDOK == fileDlg.DoModal())
    {
     // ���������ļ��Ի����ϵġ��򿪡���ť����ѡ����ļ�·���洢��strFilePath
        strFilePath = fileDlg.GetPathName();
		isAttached = true; //ȷ���и���
		MessageBox("������ӳɹ�", "��ʾ", MB_OK);
     }

}
