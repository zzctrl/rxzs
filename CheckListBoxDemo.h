
// CheckListBoxDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCheckListBoxDemoApp: 
// �йش����ʵ�֣������ CheckListBoxDemo.cpp
//

class CCheckListBoxDemoApp : public CWinApp
{
public:
	CCheckListBoxDemoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCheckListBoxDemoApp theApp;