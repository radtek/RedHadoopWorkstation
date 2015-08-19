// ReminderDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ReminderDlg.h"
#include "afxdialogex.h"


// CReminderDlg �Ի���

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


// CReminderDlg ��Ϣ�������


void CReminderDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
