// Pictureproc.cpp : 实现文件
//

#include "stdafx.h"
#include "Pictureproc.h"


// CPictureproc

IMPLEMENT_DYNAMIC(CPictureproc, CStatic)

CPictureproc::CPictureproc()
{
	 m_bLBDown = FALSE;//在构造函数初始化为FALSE 
}

CPictureproc::~CPictureproc()
{
}


BEGIN_MESSAGE_MAP(CPictureproc, CStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CPictureproc 消息处理程序




void CPictureproc::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bLBDown = true;
	if (m_bLBDown)
	{
		m_StartPoint = point;  
		m_EndPoint = point;  

		SetCapture();//设置鼠标捕获 
	}

	CStatic::OnLButtonDown(nFlags, point);
}


void CPictureproc::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (GetCapture() != this)
		 return; 
	CDC *pDC=GetDC();    
	pDC->MoveTo(m_StartPoint);    
	pDC->LineTo(point);
	ReleaseDC(pDC);
	ReleaseCapture();
	m_bLBDown = FALSE;    
	CStatic::OnLButtonUp(nFlags, point);
}


void CPictureproc::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//if (m_bLBDown)  
	//{  
	//	m_EndPoint = point;  
	//	Invalidate(FALSE);//更新界面  
	//} 
	if (GetCapture() != this)
		 return; 
	int oldMode;
	CDC *pDC=GetDC();
	oldMode = pDC->GetROP2();
	pDC->SetROP2(R2_NOT);    
	pDC->MoveTo(m_StartPoint);
	pDC->LineTo(m_EndPoint);
	pDC->MoveTo(m_StartPoint);
	pDC->LineTo(point);
	pDC->SetROP2(oldMode);   
	m_EndPoint = point;
	ReleaseDC(pDC);
	CStatic::OnMouseMove(nFlags, point);
}


void CPictureproc::OnPaint()
{
	

	// 不为绘图消息调用 CStatic::OnPaint()
}
