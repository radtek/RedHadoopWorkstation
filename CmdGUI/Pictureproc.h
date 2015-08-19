#pragma once
#include "resource.h"

// CPictureproc

class CPictureproc : public CStatic
{
	DECLARE_DYNAMIC(CPictureproc)

public:
	CPictureproc();
	virtual ~CPictureproc();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	CPoint m_StartPoint;//开始点   
	CPoint m_EndPoint;//结束点   
	BOOL m_bLBDown;//鼠标是否按下  
};


