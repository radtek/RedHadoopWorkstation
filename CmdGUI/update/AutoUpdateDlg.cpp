// AutoUpdateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoUpdateDlg.h"
#include "afxdialogex.h"


// CAutoUpdateDlg 对话框

IMPLEMENT_DYNAMIC(CAutoUpdateDlg, CDialogEx)

CAutoUpdateDlg::CAutoUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutoUpdateDlg::IDD, pParent)
{
	m_prsCur.SetRange(0, 100);	
	m_prsTotal.SetRange(0, 100);
	//m_servername.SetWindowTextA("服务器：红象云腾 www.redhadoop.com");
	//m_servername.GetWindowTextA(serverName);
	SetTimer(PUBLIC_TIMER, 100, NULL);

	if ( ! m_bTray )
	{
		m_pTray.cbSize				= sizeof(m_pTray);
		m_pTray.hWnd				= GetSafeHwnd();
		m_pTray.uID					= 0;
		m_pTray.uFlags				= NIF_ICON | NIF_MESSAGE | NIF_TIP;
		m_pTray.uCallbackMessage	= WM_TRAY;
		//m_pTray.hIcon				= theApp.LoadIcon( IDI_ICON1);
		_tcscpy( m_pTray.szTip, _T("正在自动更新") );
		Shell_NotifyIcon( NIM_ADD, &m_pTray );
		ShowWindow( SW_HIDE );
		m_bTray = true;
	}
}

CAutoUpdateDlg::~CAutoUpdateDlg()
{
}

void CAutoUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRS_TOTAL, m_prsTotal);
	DDX_Control(pDX, IDC_PRS_CUR, m_prsCur);
	DDX_Control(pDX, IDC_LST_LOG, m_lstLog);
	DDX_Control(pDX, IDC_EDIT1, m_servername);
}


BEGIN_MESSAGE_MAP(CAutoUpdateDlg, CDialogEx)
END_MESSAGE_MAP()


// CAutoUpdateDlg 消息处理程序
