// MsgUpdateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MsgUpdateDlg.h"
#include "afxdialogex.h"
#include "CmdGUI.h"
#include ".\logreport\HttpClient.h"
// CMsgUpdateDlg �Ի���

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


// CMsgUpdateDlg ��Ϣ�������

static int quitFlg=0;
void CMsgUpdateDlg::OnBnClickedUpdate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	quitFlg=1;
	WSACleanup();
	EndDialog(0);
	int iValue =MessageBox("�����������������������˳����Ƿ������","����ȷ��",MB_YESNO|MB_ICONINFORMATION );
	//int yes =AfxMessageBox("�����������������������˳����Ƿ������",MB_YESNO,NULL);
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
