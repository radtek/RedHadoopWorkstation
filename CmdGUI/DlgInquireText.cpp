// DlgInquireText.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgInquireText.h"
#include "afxdialogex.h"


// CDlgInquireText �Ի���

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


// CDlgInquireText ��Ϣ�������


BOOL CDlgInquireText::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
    // CString strText;
	//strText ="select frameid,R,G,B,HM,HS,HT,frame from table limit 20";
	// ���ַ�����ʾ���༭����    
	//SetDlgItemText(IDC_EINQUIRETEXT, strText); 
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
