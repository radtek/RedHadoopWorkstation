// ListImagesDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ListImagesDlg.h"
#include "afxdialogex.h"
#include "afx.h"

// CListImagesDlg 对话框

IMPLEMENT_DYNAMIC(CListImagesDlg, CDialogEx)

CListImagesDlg::CListImagesDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CListImagesDlg::IDD, pParent)
{

}

CListImagesDlg::~CListImagesDlg()
{
}

void CListImagesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIMAGES, m_image);
}


BEGIN_MESSAGE_MAP(CListImagesDlg, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIMAGES, &CListImagesDlg::OnNMDblclkLimages)
	ON_NOTIFY(LVN_HOTTRACK, IDC_LIMAGES, &CListImagesDlg::OnLvnHotTrackLimages)
	ON_NOTIFY(NM_HOVER, IDC_LIMAGES, &CListImagesDlg::OnNMHoverLimages)
END_MESSAGE_MAP()


// CListImagesDlg 消息处理程序


BOOL CListImagesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	LONG lStyle; 
	lStyle = GetWindowLong(m_image.m_hWnd, GWL_STYLE);// 获取当前窗口style  
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}





void CListImagesDlg::OnNMDblclkLimages(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	int index = m_image.GetSelectionMark();
	CString str = m_image.GetItemText(index,0);
	int i = str.Find("frame");
	int j =str.Find("-");
	CString sframeID = str.Mid(i+5,j-i-5);//图像的帧号
	int iframeID =_ttoi(sframeID);
	//::ShellExecuteA(NULL,"open",sPath,NULL,NULL,SW_SHOW);
    char* path ="E:\\\\videos\\\\2.avi";
	CvCapture* capture = cvCaptureFromFile(path);
	IplImage* frame = 0;
	int dex =0;
	char buf[128];
	while(cvWaitKey(40))
	{
        frame = cvQueryFrame( capture );
		if( !frame )
			break;
		if (dex==iframeID)
		{
			sprintf(buf,"a.bmp");
			cvSaveImage(buf,frame);
			::ShellExecuteA(NULL,"open",NULL,"a.bmp",NULL,SW_SHOW);
			break;
		}
		dex++;
	}
	
	*pResult = 0;
}


void CListImagesDlg::OnLvnHotTrackLimages(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	AfxMessageBox("hello");
	*pResult = 0;
}


void CListImagesDlg::OnNMHoverLimages(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	//AfxMessageBox("jjj");
	SYSTEMTIME st;   
	CString strDate,strTime;   
	GetLocalTime(&st);   
	strDate.Format("%d-%d-%d,",st.wYear,st.wMonth,st.wDay);   
	strTime.Format("%d:%d:%d",st.wHour,st.wMinute,st.wSecond);
	strDate=strDate+strTime;
	//printf("%s\n",strDate);
	//printf("%s\n",strTime);
	LVSETINFOTIP tip;
	USES_CONVERSION;  
	tip.pszText = A2W(strDate);
	//m_image.SetInfoTip(&tip);
	AfxMessageBox(strDate);
	*pResult = 0;
}


