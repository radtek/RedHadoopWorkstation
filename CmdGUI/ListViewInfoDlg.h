#pragma once

#include "resource.h"
#include "afxcmn.h"
// CListViewInfoDlg �Ի���

class CListViewInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CListViewInfoDlg)

public:
	CListViewInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CListViewInfoDlg();

// �Ի�������
	enum { IDD = IDD_LISTVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_listinfo;
};
