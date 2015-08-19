#pragma once

#include "resource.h"
#include "afxcmn.h"
// CListImagesDlg �Ի���

class CListImagesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CListImagesDlg)

public:
	CListImagesDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CListImagesDlg();

// �Ի�������
	enum { IDD = IDD_IMAGELIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_image;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkLimages(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnLvnHotTrackLimages(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMHoverLimages(NMHDR *pNMHDR, LRESULT *pResult);
	CString sPath;//��Ƶ·��
};
