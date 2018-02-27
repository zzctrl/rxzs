
// CheckListBoxDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CheckListBoxDemo.h"
#include "CheckListBoxDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CCheckListBoxDemoDlg �Ի���



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


// CCheckListBoxDemoDlg ��Ϣ�������

BOOL CCheckListBoxDemoDlg::OnInitDialog()
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
	m_listGames.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_listGames.InsertColumn(0, "����ID", LVCFMT_LEFT, 80);
	m_listGames.InsertColumn(1, "��Ϸ����", LVCFMT_LEFT, 200);

	OnRefreshGameList();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCheckListBoxDemoDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCheckListBoxDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void ModifyOpCode(HANDLE hProcess, DWORD a_addr, BYTE* a_code, DWORD a_len)
{
	DWORD oldProtect = 0;
	// �޸�ԭ���Ĵ���
	::VirtualProtect((LPVOID)a_addr, a_len, PAGE_EXECUTE_READWRITE, &oldProtect);
	//memcpy((LPVOID)a_addr, a_code, a_len);
	::WriteProcessMemory(hProcess, (LPVOID)a_addr, a_code, a_len, NULL);
	::VirtualProtect((LPVOID)a_addr, a_len, oldProtect, &oldProtect);
}

#define INCODE_LEN		9
void CCheckListBoxDemoDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	int nSel = m_listGames.GetSelectionMark();
	if (nSel < 0)
	{
		AfxMessageBox("����ѡ����Ϸ�����޸ģ�");
	}
	else
	{
		DWORD dwProcessId = m_listGames.GetItemData(nSel);
		if (dwProcessId > 0)
		{
			HANDLE hGmae = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
			if (NULL == hGmae)
			{
				AfxMessageBox("����Ϸ����ʧ�ܣ�");
				return;
			}

			DWORD dwCodeAddr = 0x00506509;
			BYTE oldCode[INCODE_LEN] = { 0x75, 0x07, 0xC7, 0x45, 0x0C, 0x05, 0x00, 0x00, 0x00 };
			BYTE newCode[INCODE_LEN] = { 0x75, 0x00, 0xC7, 0x45, 0x0C, m_nDistance, 0x00, 0x00, 0x00 };
			//BYTE* pCode = a_bModify ? newCode : oldCode;
			// �޸�ԭ���Ĵ���
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_listGames.DeleteAllItems();

	::EnumWindows(EnumWindowsProc, (LPARAM)this);
}

void CCheckListBoxDemoDlg::AddGame(HWND hGameWnd)
{
	char szTitle[256] = { 0 };
	::GetWindowTextA(hGameWnd, szTitle, 256);
	// ���û�к�������޸ı��⣬��ֱ�Ӷ�ȡ��Ϸ�ڴ�����
	DWORD dwProcessId = 0;
	DWORD dwThreadId = ::GetWindowThreadProcessId(hGameWnd, &dwProcessId);
	if (strcmp(szTitle, "YB_OnlineClient") == 0)
	{
		HANDLE hGmae = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
		if (NULL == hGmae)
		{
			return;
		}
		// �������Ի�ַ
		DWORD dwBaseAddr = 0x02D36514;
		DWORD dwNation = 0;
		BOOL bRet = ::ReadProcessMemory(hGmae, (LPVOID)dwBaseAddr, &dwNation, 4, NULL);
		if (dwNation > 0)
		{
			// �������Ƶ�ַ
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