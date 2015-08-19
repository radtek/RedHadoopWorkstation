#pragma once

#include "Resource.h"
// CMsgUpdateDlg 对话框

class CMsgUpdateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMsgUpdateDlg)

public:
	CMsgUpdateDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMsgUpdateDlg();

// 对话框数据
	enum { IDD = IDD_MSG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedUpdate();
};
