// ListDescInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ListDescInfoDlg.h"
#include "afxdialogex.h"


// CListDescInfoDlg 对话框

IMPLEMENT_DYNAMIC(CListDescInfoDlg, CDialogEx)

CListDescInfoDlg::CListDescInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CListDescInfoDlg::IDD, pParent)
{
	
}

CListDescInfoDlg::~CListDescInfoDlg()
{
}

void CListDescInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LISTDESCINFO, m_listdescinfo);
	DDX_Control(pDX, IDC_LISTMESSAGE, m_listmessage);
}


BEGIN_MESSAGE_MAP(CListDescInfoDlg, CDialogEx)
END_MESSAGE_MAP()


// CListDescInfoDlg 消息处理程序


BOOL CListDescInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//DWORD dwStyle = m_listdescinfo.GetExtendedStyle(); 
	//dwStyle |= LVS_EX_FULLROWSELECT;// 选中某行使整行高亮（只适用与report 风格的listctrl ） 
	//dwStyle |= LVS_EX_GRIDLINES;// 网格线（只适用与report 风格的listctrl ） 
	//dwStyle |= LVS_EX_CHECKBOXES;//item 前生成checkbox 控件 
	//m_listdescinfo.SetExtendedStyle(dwStyle);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
