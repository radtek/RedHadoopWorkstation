#pragma once

#include "resource.h"
// CDlgInquireText �Ի���

class CDlgInquireText : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInquireText)

public:
	CDlgInquireText(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgInquireText();

// �Ի�������
	enum { IDD = IDD_INQUIRETEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
