 // ListViewInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ListViewInfoDlg.h"
#include "afxdialogex.h"


// CListViewInfoDlg �Ի���

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


// CListViewInfoDlg ��Ϣ�������


BOOL CListViewInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	DWORD dwStyle = m_listinfo.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;// ѡ��ĳ��ʹ���и�����ֻ������report ����listctrl �� 
	dwStyle |= LVS_EX_GRIDLINES;// �����ߣ�ֻ������report ����listctrl �� 
	//dwStyle |= LVS_EX_CHECKBOXES;//item ǰ����checkbox �ؼ� 
	m_listinfo.SetExtendedStyle(dwStyle);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
