
// rxzsDlg.h : ͷ�ļ�
//

#pragma once


// CrxzsDlg �Ի���
class CrxzsDlg : public CDialogEx
{
// ����
public:
	CrxzsDlg(CWnd* pParent = NULL);	// ��׼���캯��

	void AddGame(HWND hGameWnd);
// �Ի�������
	enum { IDD = IDD_RXZS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	DWORD m_nDistance;
	afx_msg void OnRefreshGameList();
	CListCtrl m_listGames;
};
