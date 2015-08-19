// MsgUpdateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MsgUpdateDlg.h"
#include "afxdialogex.h"
#include "CmdGUI.h"
#include ".\logreport\HttpClient.h"
// CMsgUpdateDlg 对话框

IMPLEMENT_DYNAMIC(CMsgUpdateDlg, CDialogEx)

CMsgUpdateDlg::CMsgUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMsgUpdateDlg::IDD, pParent)
{

}

CMsgUpdateDlg::~CMsgUpdateDlg()
{
}

void CMsgUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMsgUpdateDlg, CDialogEx)
	ON_BN_CLICKED(ID_Update, &CMsgUpdateDlg::OnBnClickedUpdate)
END_MESSAGE_MAP()


// CMsgUpdateDlg 消息处理程序

static int quitFlg=0;
void CMsgUpdateDlg::OnBnClickedUpdate()
{
	// TODO: 在此添加控件通知处理程序代码
	quitFlg=1;
	WSACleanup();
	EndDialog(0);
	int iValue =MessageBox("即将进入升级程序，主程序将退出，是否继续？","升级确认",MB_YESNO|MB_ICONINFORMATION );
	//int yes =AfxMessageBox("即将进入升级程序，主程序将退出，是否继续？",MB_YESNO,NULL);
	if (iValue==IDYES)
	{
		CString info;
		info.Format("%s&type=active&ver=0.2&aid=%d&&cid=0",theApp.href,theApp.CRHAid::CRH_UPDATE);
		CHttpClient::getinstance()->startdoget(info);

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

}
