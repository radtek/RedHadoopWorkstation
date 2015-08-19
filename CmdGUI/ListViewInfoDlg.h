#pragma once

#include "resource.h"
#include "afxcmn.h"
// CListViewInfoDlg 对话框

class CListViewInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CListViewInfoDlg)

public:
	CListViewInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CListViewInfoDlg();

// 对话框数据
	enum { IDD = IDD_LISTVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_listinfo;
};
