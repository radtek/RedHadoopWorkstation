#pragma once
#include "afxwin.h"
#include "resource.h"

// CDialogSpark 对话框

class CDialogSpark : public CDialog
{
	DECLARE_DYNAMIC(CDialogSpark)

public:
	CDialogSpark(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogSpark();

// 对话框数据
	enum { IDD = IDD_DIALOGSPARK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBstartmaster();
	afx_msg void OnBnClickedBstarthiveserver();
	afx_msg void OnBnClickedBsttartshell();
};
