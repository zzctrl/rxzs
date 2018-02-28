
// rxjhLoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "rxjhLogin.h"
#include "rxjhLoginDlg.h"
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


// CrxjhLoginDlg �Ի���



CrxjhLoginDlg::CrxjhLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RXJHLOGIN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pNetThread = NULL;
}

void CrxjhLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CrxjhLoginDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CrxjhLoginDlg ��Ϣ�������

BOOL CrxjhLoginDlg::OnInitDialog()
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
	if (NULL == m_pNetThread)
	{
		m_pNetThread = (CNetMonitor*)AfxBeginThread(RUNTIME_CLASS(CNetMonitor));
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CrxjhLoginDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CrxjhLoginDlg::OnPaint()
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
HCURSOR CrxjhLoginDlg::OnQueryDragIcon()
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
/*
#define INCODE_LEN		9
void CrxzsDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	int nSel = m_listGames.GetSelectionMark();
	if (nSel < 0)
	{
		AfxMessageBox(L"����ѡ����Ϸ�����޸ģ�");
	}
	else
	{
		DWORD dwProcessId = m_listGames.GetItemData(nSel);
		if (dwProcessId > 0)
		{
			HANDLE hGmae = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
			if (NULL == hGmae)
			{
				AfxMessageBox(L"����Ϸ����ʧ�ܣ�");
				return;
			}

			DWORD dwCodeAddr = 0x00506509;
			BYTE oldCode[INCODE_LEN] = { 0x75, 0x07, 0xC7, 0x45, 0x0C, 0x05, 0x00, 0x00, 0x00 };
			BYTE newCode[INCODE_LEN] = { 0x75, 0x00, 0xC7, 0x45, 0x0C, m_nDistance, 0x00, 0x00, 0x00 };
			memcpy(newCode + 5, &m_nDistance, 4);
			//BYTE* pCode = a_bModify ? newCode : oldCode;
			// �޸�ԭ���Ĵ���
			ModifyOpCode(hGmae, dwCodeAddr, newCode, INCODE_LEN);
			::CloseHandle(hGmae);
		}
	}
}*/
