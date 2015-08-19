// Pictureproc.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Pictureproc.h"


// CPictureproc

IMPLEMENT_DYNAMIC(CPictureproc, CStatic)

CPictureproc::CPictureproc()
{
	 m_bLBDown = FALSE;//�ڹ��캯����ʼ��ΪFALSE 
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



// CPictureproc ��Ϣ�������




void CPictureproc::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bLBDown = true;
	if (m_bLBDown)
	{
		m_StartPoint = point;  
		m_EndPoint = point;  

		SetCapture();//������겶�� 
	}

	CStatic::OnLButtonDown(nFlags, point);
}


void CPictureproc::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//if (m_bLBDown)  
	//{  
	//	m_EndPoint = point;  
	//	Invalidate(FALSE);//���½���  
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
	

	// ��Ϊ��ͼ��Ϣ���� CStatic::OnPaint()
}
