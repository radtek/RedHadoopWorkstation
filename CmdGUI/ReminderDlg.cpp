// ReminderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ReminderDlg.h"
#include "afxdialogex.h"


// CReminderDlg 对话框

IMPLEMENT_DYNAMIC(CReminderDlg, CDialogEx)

CReminderDlg::CReminderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReminderDlg::IDD, pParent)
{

}

CReminderDlg::~CReminderDlg()
{
}

void CReminderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CReminderDlg, CDialogEx)
	ON_BN_CLICKED(ID_OK, &CReminderDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CReminderDlg 消息处理程序


void CReminderDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	CString    sPath;   
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);   
	sPath.ReleaseBuffer    ();   
	int    nPos;   
	nPos=sPath.ReverseFind('\\');   
	sPath=sPath.Left(nPos);   
	::ShellExecuteA(NULL,"open","AutoUpdateClient.exe",NULL,sPath,SW_SHOW);
	exit(0);
}
