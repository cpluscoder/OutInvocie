
// MgrCenter.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMgrCenterApp:
// �йش����ʵ�֣������ MgrCenter.cpp
//

class CMgrCenterApp : public CWinApp
{
public:
	CMgrCenterApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMgrCenterApp theApp;