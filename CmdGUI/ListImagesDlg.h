#pragma once

#include "resource.h"
#include "afxcmn.h"
// CListImagesDlg 对话框

class CListImagesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CListImagesDlg)

public:
	CListImagesDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CListImagesDlg();

// 对话框数据
	enum { IDD = IDD_IMAGELIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_image;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkLimages(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnLvnHotTrackLimages(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMHoverLimages(NMHDR *pNMHDR, LRESULT *pResult);
	CString sPath;//视频路径
};
