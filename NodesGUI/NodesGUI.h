
// NodesGUI.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CNodesGUIApp:
// �йش����ʵ�֣������ NodesGUI.cpp
//

class CNodesGUIApp : public CWinApp
{
public:
	CNodesGUIApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CNodesGUIApp theApp;