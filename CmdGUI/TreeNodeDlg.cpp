// TreeNodeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CmdGUI.h"
#include "TreeNodeDlg.h"
#include "afxdialogex.h"


// CTreeNodeDlg 对话框

IMPLEMENT_DYNAMIC(CTreeNodeDlg, CDialogEx)

CTreeNodeDlg::CTreeNodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTreeNodeDlg::IDD, pParent)
	/*, node_Name(_T(""))
	, node_IP(_T(""))*/
{

}

CTreeNodeDlg::~CTreeNodeDlg()
{
}

void CTreeNodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_IPADDRESS, m_ipaddress);
}


BEGIN_MESSAGE_MAP(CTreeNodeDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTreeNodeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTreeNodeDlg 消息处理程序


void CTreeNodeDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_NODENAME,node_Name);
	 DWORD    dwIP;
	 IN_ADDR ia;
	 m_ipaddress.GetAddress(dwIP);
	 ia.S_un.S_addr = htonl(dwIP);
	 node_IP = inet_ntoa(ia);
	CDialogEx::OnOK();
}
