// DlgInquireText.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgInquireText.h"
#include "afxdialogex.h"


// CDlgInquireText 对话框

IMPLEMENT_DYNAMIC(CDlgInquireText, CDialogEx)

CDlgInquireText::CDlgInquireText(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgInquireText::IDD, pParent)
{

}

CDlgInquireText::~CDlgInquireText()
{
}

void CDlgInquireText::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgInquireText, CDialogEx)
END_MESSAGE_MAP()


// CDlgInquireText 消息处理程序


BOOL CDlgInquireText::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
    // CString strText;
	//strText ="select frameid,R,G,B,HM,HS,HT,frame from table limit 20";
	// 将字符串显示到编辑框中    
	//SetDlgItemText(IDC_EINQUIRETEXT, strText); 
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
