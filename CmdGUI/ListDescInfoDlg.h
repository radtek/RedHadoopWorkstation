#pragma once

#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"
// CListDescInfoDlg 对话框

class CListDescInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CListDescInfoDlg)

public:
	CListDescInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CListDescInfoDlg();

// 对话框数据
	enum { IDD = IDD_DESCINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	//CListCtrl m_listdescinfo;
	CListBox m_listmessage;
	
};
