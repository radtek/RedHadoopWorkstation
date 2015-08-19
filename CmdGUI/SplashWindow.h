#pragma once


// CSplashWindow

class CSplashWindow : public CWnd
{
	DECLARE_DYNAMIC(CSplashWindow)

public:
	CSplashWindow();
	virtual ~CSplashWindow();
	void CreateSplash();
	void ShowText(LPCTSTR lpStr);
	


protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
private:
	CDC MemDC;
	BITMAP bmBitmap;
	CBitmap m_Bitmap;
	CBitmap *Old_Bitmap;
	//BOOL fileIsValid;
};


