#pragma once

#include "Resource.h"
// CReminderDlg 对话框

class CReminderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CReminderDlg)

public:
	CReminderDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReminderDlg();

// 对话框数据
	enum { IDD = IDD_REMINDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
