#pragma once
#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"

// CDlgVidwoLoad �Ի���

class CDlgVideoLoad : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgVideoLoad)

public:
	CDlgVideoLoad(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgVideoLoad();

// �Ի�������
	enum { IDD = IDD_DLGVIDEOLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnvideobrowser();
	afx_msg void OnBnClickedOk();
	CProgressCtrl m_progress;
	virtual BOOL OnInitDialog();
	CString strStore;
	CString  strPath;
	int m_radiobtngroup1;
	afx_msg void OnRadioBtnGroup1Clicked();
};
