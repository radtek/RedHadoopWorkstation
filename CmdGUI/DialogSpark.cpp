// DialogSpark.cpp : 实现文件
//

#include "stdafx.h"
#include "DialogSpark.h"
#include "afxdialogex.h"
#include "CmdGUI.h"
#include ".\logreport\HttpClient.h"


// CDialogSpark 对话框

IMPLEMENT_DYNAMIC(CDialogSpark, CDialog)

CDialogSpark::CDialogSpark(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSpark::IDD, pParent)
{

}

CDialogSpark::~CDialogSpark()
{
}

void CDialogSpark::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogSpark, CDialog)
	ON_BN_CLICKED(IDC_BSTARTMASTER, &CDialogSpark::OnBnClickedBstartmaster)
	ON_BN_CLICKED(IDC_BStartHiveServer, &CDialogSpark::OnBnClickedBstarthiveserver)
	ON_BN_CLICKED(IDC_BStTARTSHELL, &CDialogSpark::OnBnClickedBsttartshell)
END_MESSAGE_MAP()


BOOL SparkRunExec(const TCHAR *cmd, const TCHAR *para, DWORD dwMilliseconds,bool wait=false)
{    //  DWORD Sk=GetTickCount();

	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = cmd;
	ShExecInfo.lpParameters = para;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow =SW_SHOWMINIMIZED; // SW_HIDE
	ShExecInfo.hInstApp = NULL;
	BOOL suc = ShellExecuteEx(&ShExecInfo);

	if (wait) {  
		HANDLE hProcess = ShExecInfo.hProcess;  
		if (hProcess != 0) {  
			WaitForSingleObject(hProcess, INFINITE);  
			CloseHandle(hProcess);  
		}  
	}  
	return suc;
}
void CDialogSpark::OnBnClickedBstartmaster()
{
	CString info;
	info.Format("%s&type=active&ver=0.2&aid=%d&service=%d&cid=0",theApp.href,theApp.CRH_USE,theApp.CRH_SPARK);
	CHttpClient::getinstance()->startdoget(info);
	 BOOL IsStartHive = SparkRunExec(_T("cmd"),_T("/c start-master.cmd"), INFINITE);
	if(!IsStartHive) 
	{
		AfxMessageBox("spark master start failed! ");
	}
}


void CDialogSpark::OnBnClickedBstarthiveserver()
{
	CString info;
	info.Format("%s&type=active&ver=0.2&aid=%d&service=%d&cid=0",theApp.href,theApp.CRH_USE,theApp.CRH_SPARK);
	CHttpClient::getinstance()->startdoget(info);
	 BOOL IsStartHive = SparkRunExec(_T("cmd"),_T("/c start-slave.cmd"), INFINITE);
	if(!IsStartHive) 
	{
		AfxMessageBox("spark worker start failed! ");
	}
}


void CDialogSpark::OnBnClickedBsttartshell()
{
	CString info;
	info.Format("%s&type=active&ver=0.2&aid=%d&service=%d&cid=0",theApp.href,theApp.CRH_USE,theApp.CRH_SPARK);
	CHttpClient::getinstance()->startdoget(info);
	 BOOL IsStartHive = SparkRunExec(_T("cmd"),_T("/c spark-shell.cmd"), INFINITE);
	if(!IsStartHive) 
	{
		AfxMessageBox("spark shell start failed! ");
	}
}
