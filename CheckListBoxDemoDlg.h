
// CheckListBoxDemoDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CCheckListBoxDemoDlg �Ի���
class CCheckListBoxDemoDlg : public CDialogEx
{
// ����
public:
	CCheckListBoxDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

	void AddGame(HWND hGameWnd);
// �Ի�������
	enum { IDD = IDD_CHECKLISTBOXDEMO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	DWORD m_nDistance;
	afx_msg void OnRefreshGameList();
	CListCtrl m_listGames;
};
