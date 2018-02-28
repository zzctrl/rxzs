#pragma once



// CNetMonitor

#define NETMSG_START_QUERYLINE			WM_USER + 0x100
#define NETMSG_STOP_QUERYLINE			WM_USER + 0x101
#define NETMSG_NOTIFY_LINEDATA			WM_USER + 0x102



class CNetMonitor : public CWinThread
{
	DECLARE_DYNCREATE(CNetMonitor)

protected:
	CNetMonitor();           // 动态创建所使用的受保护的构造函数
	virtual ~CNetMonitor();

	void ParseRecvData(const char* a_buf, int a_len);
public:
	void SetNotifyWnd(HWND a_hWnd);

	virtual BOOL InitInstance();
	virtual int ExitInstance();

private:
	HWND m_hNotifyWnd;

	static const int s_bufLen = 2048;
	char m_recvBuf[s_bufLen];
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMsgStartQueryLine(WPARAM a_wpram, LPARAM a_lpram);
	afx_msg void OnMsgStopQueryLine(WPARAM a_wpram, LPARAM a_lpram);
};


