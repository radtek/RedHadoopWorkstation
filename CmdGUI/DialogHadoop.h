#pragma once
#include "afxwin.h"


// CDialogHadoop �Ի���

class CDialogHadoop : public CDialog
{
	DECLARE_DYNAMIC(CDialogHadoop)

public:
	CDialogHadoop(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogHadoop();

// �Ի�������
	enum { IDD = IDD_DIALOGHADOOP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBstarth();
	afx_msg void OnBnClickedBstoph();
	afx_msg void OnBnClickedBstarty();
	afx_msg void OnBnClickedBstopy();

	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

protected:
	CButton m_bStartH;
	CButton m_bStopH;
	CButton m_pStartY;
	CButton m_pStopY;
private:
	BOOL IsStartH;
	BOOL IsStopH;
	BOOL IsStartY;
	BOOL IsStopY;
public:

	 void DestroyWin();
};
