// SplashWindow.cpp : 实现文件
//

#include "stdafx.h"
#include "CmdGUI.h"
#include "SplashWindow.h"
#include<afxwin.h>
#include<mmsystem.h>

// CSplashWindow

IMPLEMENT_DYNAMIC(CSplashWindow, CWnd)

CSplashWindow::CSplashWindow()
{
    m_Bitmap.LoadBitmap(MAKEINTRESOURCE(IDB_SPLASHWINDOW)); //Load Bitmap
	m_Bitmap.GetBitmap(&bmBitmap);         //Get Bitmap Info
}

CSplashWindow::~CSplashWindow()
{
}


BEGIN_MESSAGE_MAP(CSplashWindow, CWnd)
 ON_WM_PAINT()
END_MESSAGE_MAP()



// CSplashWindow 消息处理程序
void CSplashWindow::CreateSplash()
{
	//Create Splash Window
	CreateEx(0,
		     AfxRegisterWndClass(
			 0,
			 AfxGetApp()->LoadStandardCursor(IDC_UPARROW)),
			 "RedHadoop Enterprise 0.2",
			 WS_POPUP,
			 0,
			 0,
			 bmBitmap.bmWidth,  //Bitmap Width = Splash Window Width
			 bmBitmap.bmHeight, //Bitmap Height = Splash Window Height
			 NULL,
			 NULL,
			 NULL);
}

void CSplashWindow::OnPaint()
{
	CPaintDC dc(this);
	MemDC.CreateCompatibleDC(NULL); //Create Memory DC
	Old_Bitmap = MemDC.SelectObject(&m_Bitmap); //Select DC
	dc.StretchBlt(0,
				  0,
				  bmBitmap.bmWidth,
				  bmBitmap.bmHeight,   
				  &MemDC,   
				  0,
				  0,
				  bmBitmap.bmWidth,    
				  bmBitmap.bmHeight,
				  SRCCOPY);
	
	MemDC.SelectObject(Old_Bitmap); //Select Bitmap
}
void CSplashWindow::ShowText(LPCTSTR lpStr)
{
	//if(fileIsValid)
	{
		Invalidate();
        CWnd   *pWnd   =   GetDlgItem(IDB_SPLASHWINDOW); // 取得控件的指针
       HWND hwnd = pWnd->GetSafeHwnd();  // 取得控件的句柄
      /* CreateDC(
	   hScreenDC	= CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
hMemDC	 = CreateCompatibleDC(hScreenDC);
hMemBitmap	= CreateCompatibleBitmap(hScreenDC, iWidth, iHeight);	
hOldBitmap	= (HBITMAP)SelectObject(hMemDC, hMemBitmap);*/
		CPaintDC dc(pWnd);
		dc.SetTextColor(RGB(255,0,0));//设置字体颜色
		CFont font;
        VERIFY(font.CreatePointFont(120,_T(""), &dc));
		dc.SelectObject(&font);//设置字体
		//dc.SetBkMode(TRANSPARENT);//设置和背景颜色模式相同
		SIZE sz;
		sz = (SIZE)dc.GetTextExtent(lpStr,strlen(lpStr));
		dc.TextOut(( bmBitmap.bmWidth-sz.cx)/2,bmBitmap.bmHeight/2,lpStr);
	}
}
