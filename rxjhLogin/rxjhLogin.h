
// rxjhLogin.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CrxjhLoginApp: 
// �йش����ʵ�֣������ rxjhLogin.cpp
//

class CrxjhLoginApp : public CWinApp
{
public:
	CrxjhLoginApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CrxjhLoginApp theApp;