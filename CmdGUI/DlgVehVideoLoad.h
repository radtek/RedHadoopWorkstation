#pragma once
#include "resource.h"

// CDlgVehVideoLoad �Ի���

class CDlgVehVideoLoad : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgVehVideoLoad)

public:
	CDlgVehVideoLoad(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgVehVideoLoad();

// �Ի�������
	enum { IDD = IDD_DLGVEHVIDEOLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
