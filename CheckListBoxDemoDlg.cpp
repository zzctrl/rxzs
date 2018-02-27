
// CheckListBoxDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CheckListBoxDemo.h"
#include "CheckListBoxDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CCheckListBoxDemoDlg 对话框



CCheckListBoxDemoDlg::CCheckListBoxDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CHECKLISTBOXDEMO_DIALOG, pParent)
	, m_nDistance(90)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCheckListBoxDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nDistance);
	DDX_Control(pDX, IDC_LIST3, m_listGames);
}

BEGIN_MESSAGE_MAP(CCheckListBoxDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CCheckListBoxDemoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CCheckListBoxDemoDlg::OnRefreshGameList)
END_MESSAGE_MAP()


// CCheckListBoxDemoDlg 消息处理程序

BOOL CCheckListBoxDemoDlg::OnInitDialog()
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
	m_listGames.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_listGames.InsertColumn(0, "进程ID", LVCFMT_LEFT, 80);
	m_listGames.InsertColumn(1, "游戏名称", LVCFMT_LEFT, 200);

	OnRefreshGameList();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCheckListBoxDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCheckListBoxDemoDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCheckListBoxDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void ModifyOpCode(HANDLE hProcess, DWORD a_addr, BYTE* a_code, DWORD a_len)
{
	DWORD oldProtect = 0;
	// 修改原来的代码
	::VirtualProtect((LPVOID)a_addr, a_len, PAGE_EXECUTE_READWRITE, &oldProtect);
	//memcpy((LPVOID)a_addr, a_code, a_len);
	::WriteProcessMemory(hProcess, (LPVOID)a_addr, a_code, a_len, NULL);
	::VirtualProtect((LPVOID)a_addr, a_len, oldProtect, &oldProtect);
}

#define INCODE_LEN		9
void CCheckListBoxDemoDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	int nSel = m_listGames.GetSelectionMark();
	if (nSel < 0)
	{
		AfxMessageBox("请先选中游戏后再修改！");
	}
	else
	{
		DWORD dwProcessId = m_listGames.GetItemData(nSel);
		if (dwProcessId > 0)
		{
			HANDLE hGmae = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
			if (NULL == hGmae)
			{
				AfxMessageBox("打开游戏进程失败！");
				return;
			}

			DWORD dwCodeAddr = 0x00506509;
			BYTE oldCode[INCODE_LEN] = { 0x75, 0x07, 0xC7, 0x45, 0x0C, 0x05, 0x00, 0x00, 0x00 };
			BYTE newCode[INCODE_LEN] = { 0x75, 0x00, 0xC7, 0x45, 0x0C, m_nDistance, 0x00, 0x00, 0x00 };
			//BYTE* pCode = a_bModify ? newCode : oldCode;
			// 修改原来的代码
			ModifyOpCode(hGmae, dwCodeAddr, newCode, INCODE_LEN);
			::CloseHandle(hGmae);
		}
	}
}


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam)
{
	char szClass[128] = { 0 };
	::GetClassNameA(hwnd, szClass, 128);
	if (strcmp(szClass, "D3D Window") == 0)
	{
		CCheckListBoxDemoDlg* pDlg = (CCheckListBoxDemoDlg*)lparam;
		if (pDlg)
		{
			pDlg->AddGame(hwnd);
		}
	}
	return TRUE;
}

void CCheckListBoxDemoDlg::OnRefreshGameList()
{
	// TODO: 在此添加控件通知处理程序代码
	m_listGames.DeleteAllItems();

	::EnumWindows(EnumWindowsProc, (LPARAM)this);
}

void CCheckListBoxDemoDlg::AddGame(HWND hGameWnd)
{
	char szTitle[256] = { 0 };
	::GetWindowTextA(hGameWnd, szTitle, 256);
	// 如果没有呼出外挂修改标题，则直接读取游戏内存数据
	DWORD dwProcessId = 0;
	DWORD dwThreadId = ::GetWindowThreadProcessId(hGameWnd, &dwProcessId);
	if (strcmp(szTitle, "YB_OnlineClient") == 0)
	{
		HANDLE hGmae = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
		if (NULL == hGmae)
		{
			return;
		}
		// 人物属性基址
		DWORD dwBaseAddr = 0x02D36514;
		DWORD dwNation = 0;
		BOOL bRet = ::ReadProcessMemory(hGmae, (LPVOID)dwBaseAddr, &dwNation, 4, NULL);
		if (dwNation > 0)
		{
			// 人物名称地址
			DWORD dwNameAddr = dwNation + 0x18;
			memset(szTitle, 0, 256);
			bRet = ::ReadProcessMemory(hGmae, (LPVOID)dwNameAddr, szTitle, 80, NULL);
		}
		
		::CloseHandle(hGmae);
	}

	CString szId;
	szId.Format("%d", dwProcessId);
	int nIndex = m_listGames.GetItemCount();
	m_listGames.InsertItem(nIndex, szId);
	m_listGames.SetItemText(nIndex, 1, szTitle);
	m_listGames.SetItemData(nIndex, dwProcessId);
}