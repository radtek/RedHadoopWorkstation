#pragma once
#include "resource.h"

// CDlgVehVideoLoad 对话框

class CDlgVehVideoLoad : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgVehVideoLoad)

public:
	CDlgVehVideoLoad(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgVehVideoLoad();

// 对话框数据
	enum { IDD = IDD_DLGVEHVIDEOLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnvideobrowser();
	CProgressCtrl m_progress;
	CString strStore;
	CString  strPath;
	int m_radiobtngroup1;
	virtual BOOL OnInitDialog();
};
