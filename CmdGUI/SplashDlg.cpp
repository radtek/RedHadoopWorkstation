// SplashDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CmdGUI.h"
#include "SplashDlg.h"


// CSplashDlg 对话框

IMPLEMENT_DYNAMIC(CSplashDlg, CDialog)

CSplashDlg::CSplashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSplashDlg::IDD, pParent)
{
 
}

CSplashDlg::~CSplashDlg()
{
}

void CSplashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_SInfo, m_sinfo);
	//DDX_Control(pDX, IDC_PICTURE, m_pinfo);
}


BEGIN_MESSAGE_MAP(CSplashDlg, CDialog)
END_MESSAGE_MAP()


// CSplashDlg 消息处理程序
