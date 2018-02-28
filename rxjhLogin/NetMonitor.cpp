// NetMonitor.cpp : 实现文件
//

#include "stdafx.h"
#include "rxjhLogin.h"
#include "NetMonitor.h"


// CNetMonitor

IMPLEMENT_DYNCREATE(CNetMonitor, CWinThread)

CNetMonitor::CNetMonitor()
{
	m_hNotifyWnd = NULL;
}

CNetMonitor::~CNetMonitor()
{
}

BOOL CNetMonitor::InitInstance()
{
	// TODO:    在此执行任意逐线程初始化
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	//构建保存版本号的WORD类型变量
	wVersionRequested = MAKEWORD(1, 1);
	//加载套接字库
	err = ::WSAStartup(wVersionRequested, &wsaData);
	hostent* phost = ::gethostbyname("sessionw1.rxjh.cdcgames.net");
	if (phost)
	{
		SOCKET s = ::socket(AF_INET, SOCK_STREAM, 0);
		if (INVALID_SOCKET != s)
		{
			unsigned short port = 13101;
			struct sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_port = ntohs(port);
			addr.sin_addr.S_un.S_addr = *((ULONG*)(phost->h_addr));
			int len = sizeof(addr);
			int ret = ::connect(s, (const sockaddr*)&addr, len);
			int nerr = ::WSAGetLastError();
			if (0 == ret)
			{
				DWORD dwBuf = 0x00008016;
				BYTE buf[4] = { 0x16, 0x80, 0, 0 };
				int nSend = ::send(s, (const char*)buf, 4, 0);
				memset(m_recvBuf, 0, s_bufLen);
				int nRecv = ::recv(s, m_recvBuf, s_bufLen, 0);
				if (nRecv > 0)
				{
					ParseRecvData(m_recvBuf, nRecv);
					::OutputDebugStringA("succ");
				}
				else
				{
					::OutputDebugStringA("err");
				}
			}
			::closesocket(s);
		}
	}

	return TRUE;
}

const char* memfind(const char* pMemory, int nMemLen, const char* pFind)
{
	int nFindLen = strlen(pFind);
	int nMaxPos = nMemLen - nFindLen;
	int nPos = 0;
	const char* pRet = (const char*)::memchr(pMemory + nPos, *pFind, nMemLen);
	while (pRet && nPos <= nMaxPos)
	{
		if (memcmp(pRet, pFind, nFindLen) == 0)
		{
			return pRet;
		}

		nPos++;
		nMemLen--;
		pRet = (const char*)::memchr(pMemory + nPos, *pFind, nMemLen);
	}
	return NULL;
}

void CNetMonitor::ParseRecvData(const char* a_buf, int a_len)
{
	if (NULL == a_buf)
	{
		return;
	}

	const char* pname = "雪原";
	const char* paddr = memfind(a_buf, a_len, pname);
	if (paddr)
	{
		BYTE nNameLen = *(paddr - 2);
		const char* pLineCount = paddr + nNameLen + 6;
		BYTE nLineCount = *pLineCount;
		BYTE nLineIndex = 1;
		const char* pLine = pLineCount + 2;
		while (nLineIndex <= nLineCount)
		{
			nNameLen = *(pLine + 2);
			pLine = pLine + 4 + nNameLen;
			BYTE percent = *pLine;
			CString szMsg;
			szMsg.Format(_T("line%d : %d"), nLineIndex, percent);
			::OutputDebugString(szMsg);

			nLineIndex++;
			pLine += 2;
		}
	}
}

int CNetMonitor::ExitInstance()
{
	// TODO:    在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

void CNetMonitor::SetNotifyWnd(HWND a_hWnd)
{
	m_hNotifyWnd = a_hWnd;
}

BEGIN_MESSAGE_MAP(CNetMonitor, CWinThread)
	ON_THREAD_MESSAGE(NETMSG_START_QUERYLINE, &CNetMonitor::OnMsgStartQueryLine)
	ON_THREAD_MESSAGE(NETMSG_STOP_QUERYLINE, &CNetMonitor::OnMsgStopQueryLine)
END_MESSAGE_MAP()


// CNetMonitor 消息处理程序
void CNetMonitor::OnMsgStartQueryLine(WPARAM a_wpram, LPARAM a_lpram)
{

}

void CNetMonitor::OnMsgStopQueryLine(WPARAM a_wpram, LPARAM a_lpram)
{

}