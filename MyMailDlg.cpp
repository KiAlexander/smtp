
// MyMailDlg.cpp : 实现文件
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

CString Temp1;//服务器地址缓存量
CString Temp2;//端口缓存量
CString strFilePath;//附件路径
bool isAttached;//是否有附件
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMyMailDlg 对话框

void   CMyMailDlg::SetControlInfo(WORD   CtrlId)//添加控件ID，便于调整对话框大小
{
	m_control_info.Add(CtrlId);

}

CMyMailDlg::CMyMailDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MYMAIL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);//加载图标
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


// CMyMailDlg 消息处理程序

BOOL CMyMailDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);//未配置SMTP之前不允许发送邮件


	//添加控件控制
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


	//测试数据
	GetDlgItem(IDC_EDIT1)->SetWindowText("a_mail_test@163.com");//用户名
	GetDlgItem(IDC_EDIT2)->SetWindowText("bupt123");//授权码
	GetDlgItem(IDC_EDIT3)->SetWindowText("a_mail_test@163.com");//发送地址
	GetDlgItem(IDC_EDIT4)->SetWindowText("a_mail_test@163.com");//j接收地址
	GetDlgItem(IDC_EDIT5)->SetWindowText("hello");//主题
	GetDlgItem(IDC_EDIT6)->SetWindowText("发件成功！");//正文

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyMailDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//添加对话框背景图片
		CPaintDC dc(this);
		CFile m_file("MyMail.jpg", CFile::modeRead);
		//获取文件长度
		DWORD m_filelen = m_file.GetLength();
		//在堆上分配空间
		HGLOBAL m_hglobal = GlobalAlloc(GMEM_MOVEABLE, m_filelen);

		LPVOID pvdata = NULL;
		//锁定堆空间,获取指向堆空间的指针
		pvdata = GlobalLock(m_hglobal);

		//将文件数据读区到堆中
		m_file.Read(pvdata, m_filelen);

		IStream*  m_stream;

		GlobalUnlock(m_hglobal);

		//在堆中创建流对象
		CreateStreamOnHGlobal(m_hglobal, TRUE, &m_stream);

		//利用流加载图像
		OleLoadPicture(m_stream, m_filelen, TRUE, IID_IPicture, (LPVOID*)&m_picture);
		m_stream->Release();

		m_picture->get_Width(&m_width);// 宽高，MM_HIMETRIC 模式，单位是0.01毫米
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyMailDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//使用须知
void CMyMailDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//读取预先设定使用须知文本内容
	char buffer[1023] = { 0 };
	ifstream  Guidance("Guidance.txt");
	if(!Guidance.fail()) //如果打开成功
	{ 
		int i = 0;
		while (!Guidance.eof())//如果文件内容没有结束
		{
			Guidance.get(buffer[i]);//读取文本内容
			i++;
		}
		Guidance.close();//关闭文本文件
	}
	//弹出显示使用须知内容的对话框
	MessageBox(buffer, "使用说明",MB_OK);
}

//发送邮件
void CMyMailDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码

	CString buffer;//暂存编辑框内容的值

	if (Temp1.IsEmpty())//若服务器地址为空则弹出提示对话框
	{
		MessageBox("请填写服务器地址！", "提示", MB_OK);
		return;
	}

	if (Temp2.IsEmpty())//若端口为空则弹出提示对话框
	{
		MessageBox("请填写端口！", "提示", MB_OK);
		return;
	}

	//读取服务器地址和端口号
	char* SmtpSrvName = Temp1.GetBuffer(Temp1.GetLength());
	char* Port = Temp2.GetBuffer(Temp2.GetLength());


	GetDlgItem(IDC_EDIT1)->GetWindowText(buffer);
	//若用户名为空则弹出提示对话框
	if (buffer.IsEmpty())
	{
		MessageBox("请填写用户名！", "提示", MB_OK);
		return;
	}
	char UserName[32];//读取用户名
	strcpy(UserName, buffer.GetBuffer(buffer.GetLength()));
	buffer.ReleaseBuffer();

	GetDlgItem(IDC_EDIT2)->GetWindowText(buffer);
	//若密码为空则弹出提示对话框
	if (buffer.IsEmpty())
	{
		MessageBox("请填写密码！", "提示", MB_OK);
		return;
	}
	char Password[16];//读取密码
	strcpy(Password, buffer.GetBuffer(buffer.GetLength()));
	buffer.ReleaseBuffer();

	GetDlgItem(IDC_EDIT3)->GetWindowText(buffer);
	//若发送人地址为空则弹出提示对话框
	if (buffer.IsEmpty())
	{
		MessageBox("请填写发送人地址！", "提示", MB_OK);
		return;
	}
	char From[32];//读取发送人地址
	strcpy(From, buffer.GetBuffer(buffer.GetLength()));
	buffer.ReleaseBuffer();

	GetDlgItem(IDC_EDIT4)->GetWindowText(buffer);
	//若收件人地址为空则弹出提示对话框
	if (buffer.IsEmpty())
	{
		MessageBox("请填写收件人地址！", "提示", MB_OK);
		return;
	}
	char To[32];//读取收件人地址
	strcpy(To, buffer.GetBuffer(buffer.GetLength()));
	buffer.ReleaseBuffer();

	GetDlgItem(IDC_EDIT5)->GetWindowText(buffer);
	//若主题为空则弹出提示对话框
	if (buffer.IsEmpty())
	{
		MessageBox("请填写主题！", "提示", MB_OK);
		return;
	}
	char Subject[32];//读取主题
	strcpy(Subject, buffer.GetBuffer(buffer.GetLength()));
	buffer.ReleaseBuffer();

	GetDlgItem(IDC_EDIT6)->GetWindowText(buffer);
	//若邮件内容为空则弹出提示对话框
	if (buffer.IsEmpty())
	{
		MessageBox("请填写邮件内容！", "提示", MB_OK);
		return;
	}
	char Msg[64];//读取邮件内容
	strcpy(Msg, buffer.GetBuffer(buffer.GetLength()));
	buffer.ReleaseBuffer();

	SmtpMail mymail(SmtpSrvName, Port, UserName, Password,
		From, To, Subject, Msg);

	

	if (isAttached)//如果有附件则设置发送信息
	{
		string filePath(strFilePath.GetBuffer(0));
		mymail.AddAttachment(filePath);//添加附件
		
    }

	if (NULL == mymail.SendMail(isAttached))//邮件发送成功则弹出显示
	{
		MessageBox("Mailed successfully!", "提示", MB_OK);
		isAttached = false;//初始化附件判定变量，便于再次发送
	}
	else//失败提示
		MessageBox("Mailed unsuccessfully!", "错误", MB_OK);

}

//smtp设置
void CMyMailDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	SMTPSET Dlg;
	Dlg.DoModal();//生成子对话框
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);//使发送邮件按钮可用
}

//对话框大小改变响应函数
void CMyMailDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码


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

//添加附件
void CMyMailDlg::OnBnClickedAttach()
{
	// TODO: 在此添加控件通知处理程序代码
	// 设置过滤器   
	strFilePath.Empty();
	TCHAR szFilter[] = _T("文本文件(*.txt)|*.txt|所有文件(*.*)|*.*||");
    // 构造打开文件对话框   
    CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);
  	 
    // 显示打开文件对话框   
    if (IDOK == fileDlg.DoModal())
    {
     // 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径存储到strFilePath
        strFilePath = fileDlg.GetPathName();
		isAttached = true; //确认有附件
		MessageBox("附件添加成功", "提示", MB_OK);
     }

}
