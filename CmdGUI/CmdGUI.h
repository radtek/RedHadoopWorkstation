// CmdGUI.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCmdGUIApp:
// �йش����ʵ�֣������ CmdGUI.cpp
//

class CCmdGUIApp : public CWinApp
{
public:
	CCmdGUIApp();
	enum CRHAid
	{
		CRH_INSTALL=0,
		CRH_START,
		CRH_USE,
		CRH_UPDATE,
		CRH_UNINSTALL
	};
	enum CRHServiceType
	{
		CRH_HDFS=0,
		CRH_YARN,
		CRH_HIVE,
		CRH_HBASE,
		CRH_SPARK
	};

// ��д
	public:
	virtual BOOL InitInstance();
	
	CString href;
	
// ʵ��

	DECLARE_MESSAGE_MAP()
	
	virtual int ExitInstance();
	
};


extern CCmdGUIApp theApp;
