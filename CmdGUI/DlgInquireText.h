#pragma once

#include "resource.h"
// CDlgInquireText 对话框

class CDlgInquireText : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInquireText)

public:
	CDlgInquireText(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgInquireText();

// 对话框数据
	enum { IDD = IDD_INQUIRETEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
