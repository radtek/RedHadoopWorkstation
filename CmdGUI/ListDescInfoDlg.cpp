// ListDescInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ListDescInfoDlg.h"
#include "afxdialogex.h"


// CListDescInfoDlg �Ի���

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


// CListDescInfoDlg ��Ϣ�������


BOOL CListDescInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//DWORD dwStyle = m_listdescinfo.GetExtendedStyle(); 
	//dwStyle |= LVS_EX_FULLROWSELECT;// ѡ��ĳ��ʹ���и�����ֻ������report ����listctrl �� 
	//dwStyle |= LVS_EX_GRIDLINES;// �����ߣ�ֻ������report ����listctrl �� 
	//dwStyle |= LVS_EX_CHECKBOXES;//item ǰ����checkbox �ؼ� 
	//m_listdescinfo.SetExtendedStyle(dwStyle);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
