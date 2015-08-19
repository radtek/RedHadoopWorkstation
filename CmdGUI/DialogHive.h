#pragma once
#include "afxwin.h"


// CDialogHive 对话框

class CDialogHive : public CDialog
{
	DECLARE_DYNAMIC(CDialogHive)

public:
	CDialogHive(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogHive();

// 对话框数据
	enum { IDD = IDD_DIALOGHIVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBstarth();
	afx_msg void OnBnClickedBstoph();
	CButton m_bStartHive;
	CButton m_bStopHive;
	CButton m_bStopHiveServer;
	CButton m_bStartHiveServer;
	bool IsStartHive;
	bool IsStartHiveServer;
	afx_msg void OnBnClickedBstarthive();
	afx_msg void OnBnClickedBstophive();
	afx_msg void OnBnClickedBstarthiveserver();
	afx_msg void OnBnClickedBstophiveserver();
	//HWND wind;

};
