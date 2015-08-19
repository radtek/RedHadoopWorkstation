 // ListViewInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ListViewInfoDlg.h"
#include "afxdialogex.h"


// CListViewInfoDlg 对话框

IMPLEMENT_DYNAMIC(CListViewInfoDlg, CDialogEx)

CListViewInfoDlg::CListViewInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CListViewInfoDlg::IDD, pParent)
{

}

CListViewInfoDlg::~CListViewInfoDlg()
{
}

void CListViewInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTINFOS, m_listinfo);
}


BEGIN_MESSAGE_MAP(CListViewInfoDlg, CDialogEx)
END_MESSAGE_MAP()


// CListViewInfoDlg 消息处理程序


BOOL CListViewInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DWORD dwStyle = m_listinfo.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;// 选中某行使整行高亮（只适用与report 风格的listctrl ） 
	dwStyle |= LVS_EX_GRIDLINES;// 网格线（只适用与report 风格的listctrl ） 
	//dwStyle |= LVS_EX_CHECKBOXES;//item 前生成checkbox 控件 
	m_listinfo.SetExtendedStyle(dwStyle);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
