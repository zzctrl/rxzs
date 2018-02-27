
// rxzsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "rxzs.h"
#include "rxzsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CrxzsDlg �Ի���



CrxzsDlg::CrxzsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CrxzsDlg::IDD, pParent)
	, m_nDistance(90)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CrxzsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nDistance);
	DDX_Control(pDX, IDC_LIST3, m_listGames);
}

BEGIN_MESSAGE_MAP(CrxzsDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CrxzsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CrxzsDlg::OnRefreshGameList)
END_MESSAGE_MAP()


// CrxzsDlg ��Ϣ�������

BOOL CrxzsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_listGames.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listGames.InsertColumn(0, L"����ID", LVCFMT_LEFT, 80);
	m_listGames.InsertColumn(1, L"��Ϸ����", LVCFMT_LEFT, 200);

	OnRefreshGameList();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CrxzsDlg::OnPaint()
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
HCURSOR CrxzsDlg::OnQueryDragIcon()
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
}


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam)
{
	char szClass[128] = { 0 };
	::GetClassNameA(hwnd, szClass, 128);
	if (strcmp(szClass, "D3D Window") == 0)
	{
		CrxzsDlg* pDlg = (CrxzsDlg*)lparam;
		if (pDlg)
		{
			pDlg->AddGame(hwnd);
		}
	}
	return TRUE;
}

void CrxzsDlg::OnRefreshGameList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_listGames.DeleteAllItems();

	::EnumWindows(EnumWindowsProc, (LPARAM)this);
}

void CrxzsDlg::AddGame(HWND hGameWnd)
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
	szId.Format(L"%d", dwProcessId);
	CString szName(szTitle);
	int nIndex = m_listGames.GetItemCount();
	m_listGames.InsertItem(nIndex, szId);
	m_listGames.SetItemText(nIndex, 1, szName);
	m_listGames.SetItemData(nIndex, dwProcessId);
}
