// SeasbaseVideoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SeasbaseVideoDlg.h"
#include "afxdialogex.h"
#include "ItemType.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "BASE64_API.h"
#include <gdiplus.h>
#include <stdlib.h>/*�õ���srand����������Ҫ�����ͷ�ļ�*/
#include<time.h>/*�õ���time����������Ҫ�����ͷ�ļ�*/
#include "DlgVideoLoad.h"

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

using namespace std;


int			maxItemCount		= 100;
extern   CItem*ItemList		= new CItem[maxItemCount]; 
//�Զ��庯��
int		GetItemCount(CItem	* pItem);
bool	HaveSubNode	(CItem	* pItem,	int pos); 
void	DeleteItem	(CItem	* pItem,	int pos);
// CSeasbaseVideoDlg �Ի���

IMPLEMENT_DYNAMIC(CSeasbaseVideoDlg, CDialogEx)

CSeasbaseVideoDlg::CSeasbaseVideoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSeasbaseVideoDlg::IDD, pParent)
{
	ColIndex = 0;
	nCnt = 0;
	framenum = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON);
	iIsTwice = 0;
	pImg=NULL;
	tagImg = NULL;
	bSelRect=false;
	subTarImg=NULL;
}

CSeasbaseVideoDlg::~CSeasbaseVideoDlg()
{
	if (pImg)
	{
		cvReleaseImage(&pImg);
	}
	if (tagImg)
	{
		cvReleaseImage(&tagImg);
	}
}

void CSeasbaseVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TREEVIEW, m_treeview);
	DDX_Control(pDX, IDC_TAB1, m_subtab);
	//DDX_Control(pDX, IDC_TAB2, m_subtab2);
}


BEGIN_MESSAGE_MAP(CSeasbaseVideoDlg, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREEVIEW, &CSeasbaseVideoDlg::OnTvnSelchangedTreeview)
	ON_BN_CLICKED(IDC_BINQUIRE, &CSeasbaseVideoDlg::OnBnClickedBinquire)
	ON_BN_CLICKED(IDC_BLOAD, &CSeasbaseVideoDlg::OnBnClickedBload)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CSeasbaseVideoDlg::OnTcnSelchangeTab1)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BLOADVIDEO, &CSeasbaseVideoDlg::OnBnClickedBloadvideo)
	ON_BN_CLICKED(IDC_LoadImage, &CSeasbaseVideoDlg::OnBnClickedLoadimage)
	ON_STN_CLICKED(IDC_IMAGE, &CSeasbaseVideoDlg::OnStnClickedImage)
	ON_COMMAND(ID_LOAD_LOADVIDEO, &CSeasbaseVideoDlg::OnLoadLoadvideo)
	ON_COMMAND(ID_LOAD_LOADTABLES, &CSeasbaseVideoDlg::OnLoadLoadtables)
	ON_COMMAND(ID_LOAD_LOADIMAGE, &CSeasbaseVideoDlg::OnLoadLoadimage)
	ON_COMMAND(ID_HIVE_CMD, &CSeasbaseVideoDlg::OnHiveCmd)
	ON_COMMAND(ID_HELP_HELP, &CSeasbaseVideoDlg::OnHelpHelp)
	ON_BN_CLICKED(IDC_BROWERDATA, &CSeasbaseVideoDlg::OnBnClickedBrowerdata)
	ON_COMMAND(ID_LOAD_EXECUTE, &CSeasbaseVideoDlg::OnLoadExecute)
END_MESSAGE_MAP()

// CSeasbaseVideoDlg ��Ϣ�������

BOOL CSeasbaseVideoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ��GID+
	GdiplusStartupInput gdiplusStartupInput;       
	GdiplusStartup( &m_gdiplusToken,&gdiplusStartupInput,NULL );
	m_subtab.InsertItem(0,_T("Image"));
	m_subtab.InsertItem(1,_T("Result"));
    m_subtab.InsertItem(2,_T("Message"));
	//�����Ի���,���ҽ�IDC_TAB�ؼ���Ϊ������
	m_listviewinfo.Create(IDD_LISTVIEW,GetDlgItem(IDC_TAB1));
	m_listdescinfo.Create(IDD_DESCINFO,GetDlgItem(IDC_TAB1));
	m_listimages.Create(IDD_IMAGELIST,GetDlgItem(IDC_TAB1));
	//���IDC_tabMain�ͻ�����С
	CRect rs;
	m_subtab.GetClientRect(&rs);

	//�����ӶԻ����ڸ������е�λ�ã����ԸĶ���ֵ��ʹ�Ӵ���Ĵ�С���ʣ�
	rs.top+=20;
	rs.bottom-=3;
	rs.left+=2;
	rs.right-=2;

	//�����ӶԻ���ߴ粢�ƶ���ָ��λ��
	m_listviewinfo.MoveWindow(&rs);
	m_listdescinfo.MoveWindow(&rs);
	m_listimages.MoveWindow(&rs);

	//�ֱ��������غ���ʾ
	m_listimages.ShowWindow(true);
	m_listviewinfo.ShowWindow(false);
	m_listdescinfo.ShowWindow(false);
	
	//����Ĭ�ϵ�ѡ�
	m_subtab.SetCurSel(0);
   
	hRoot =m_treeview.InsertItem("Default",0,0);
	m_treeview.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	m_treeview.SelectItem(hRoot);
	//-----------------------------��ʼItem�ڵ�ֵ-----------------------------------//

	for(int i=0 ;i<maxItemCount; i++)
	{
		ItemList[i].ItemFlag	= 0;				//0������ڵ�,����
		ItemList[i].ItemName	= "";
		ItemList[i].Value		= "";
		ItemList[i].ItemParent	= 0;
	}
	m_treeview.Expand(hRoot,TVE_EXPAND);

	//LONG lStyle; 
	//lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);// ��ȡ��ǰ����style 
	//lStyle &= ~LVS_TYPEMASK; // �����ʾ��ʽλ 
	//lStyle |= LVS_REPORT; // ����style 
	//SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle);// ����style
//	INIT_EASYSIZE;
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	///*����ͼ���б�imagesѡ��*/
	//img.Create(32,32,ILC_COLOR32,1,1);
	//CBitmap* m_bitmap;
	//for (int i=0;i<3;++i)
	//{
	//	m_bitmap = new CBitmap;
	//	bool sucess  = m_bitmap->LoadBitmapA(IDB_BITMAP2);
	//	img.Add(m_bitmap,RGB(0,255,255));
	//	delete m_bitmap;
	//	m_bitmap = NULL;
	//}
	//m_listimages.m_image.InsertColumn(0,"����ͼ",LVCFMT_LEFT,100); 
	//m_listimages.m_image.InsertColumn(1,"��Ϣ",LVCFMT_LEFT,100); 

	//m_listimages.m_image.SetImageList(&img,LVSIL_SMALL); 

	//m_listimages.m_image.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_SUBITEMIMAGES);
	//for (int i= 0;i<3;++i)
	//{
	//	m_listimages.m_image.InsertItem(i,"name",i);
	//}

   //m_listimages.m_image.SetItem(3, 0, LVIF_IMAGE, NULL, 2, 0, 0, 0);
	
	return TRUE;  
}
//-----------��ȡ���Ϳؼ��������Ŀ------------------------//
int GetItemCount(CItem * pItem)
{
	int		itemCount	= 0;
	for(int i=1; i<maxItemCount; i++ )
	{
		if (pItem[i].ItemFlag != 0)
			itemCount++;
	}
	return itemCount;
}

void CSeasbaseVideoDlg::OnTvnSelchangedTreeview(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strText; // ���ڵ�ı�ǩ�ı��ַ���    

	// ��ȡ��ǰѡ�нڵ�ľ��    
	HTREEITEM hItem = m_treeview.GetSelectedItem();    
	// ��ȡѡ�нڵ�ı�ǩ�ı��ַ���    
	strText = m_treeview.GetItemText(hItem); 
	tablename = strText;
	CString eText;
	if (strText.Find("Default")!=-1)
	{
		eText ="select frameid,targetnum,width,height,yh,eh,sh,fh,wh,lh,cnt,json,frame from "+strText+" limit 1000";
	}
	else
	{ 
		GetDlgItemTextA(IDC_ETEXT,eText);
		CString eCopyText = eText;
		int iPos = eText.Find("from ");
		eCopyText.Delete(0,iPos+5);
		int iPo = eCopyText.Find(" ");
		eText.Delete(iPos+5,iPo);
		eText.Insert(iPos+5,strText);
	}
	// ���ַ�����ʾ���༭����    
	SetDlgItemTextA(IDC_ETEXT,eText);//��ӵ�DlgInquireText����ı༭����ʾ
	sEditText = eText;
	sText = eText;
	*pResult = 0;
}
/*�ָ��ַ�������*/
void split(const string& src, const string& separator, vector<string>& dest)
{
	string str = src;
	string substring;
	string::size_type start = 0, index;

	do
	{
		index = str.find_first_of(separator,start);
		if (index != string::npos)
		{    
			substring = str.substr(start,index-start);
			dest.push_back(substring);
			start = str.find_first_not_of(separator,index);
			if (start == string::npos) return;
		}
	}while(index != string::npos);

	//the last token
	substring = str.substr(start);
	dest.push_back(substring);//��β������һ������
}
//����һ������ֱ�������н���
BOOL SeasbaseRunExec(const TCHAR *cmd, const TCHAR *para, DWORD dwMilliseconds,bool wait=false)
{    

	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = cmd;
	ShExecInfo.lpParameters = para;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow =SW_NORMAL; 
	ShExecInfo.hInstApp = NULL;
	BOOL suc = ShellExecuteEx(&ShExecInfo);

	if (wait) {  
		HANDLE hProcess = ShExecInfo.hProcess;  
		if (hProcess != 0) {  
			WaitForSingleObject(hProcess, INFINITE);  
			CloseHandle(hProcess);  
		}  
	}  

	return suc;
}

int Parser(CString text,vector<string> &vText,CString &table,CString &message)
{
	message ="Process Successfully!";
	CString colname;
	vector<string> vStr;
	vector<string> vAsStr;
	bool close = false;
	std::string newText;
	std::string tablename;
	int idx = 0;
	std::string orignal=text.MakeLower();
	std::size_t found = orignal.find("select");
	std::size_t found1 = orignal.find("from");
	if(string::npos==found 
		||string::npos == found1)
	{
		message ="Sql synax error!";
		return -1;
	}
	newText = orignal.substr(found+6,found1-found-6);
	tablename = orignal.substr(found1+4);
	
	split(tablename,"; ",vStr);
	if(vStr.size()>0)//����������ʵ�����ݵĸ�����
	{
		while(1)
		{
			if(vStr[idx].size()>1)
			{
				tablename = vStr[idx];
				break;
			}
			idx++;		
		}
	}else{
		message ="Sql synax error!";
		return -1;
	}
	table = tablename.c_str();
	found = newText.find_first_of("(),");
	while (found!=std::string::npos)
	{
		if(newText[found]=='(')
		{
			if(true == close)
			{
				message = "Sql synax error!";
				return -1;
			}
			close = true;
		}
	
		if(newText[found]==')')
		{
			if(false == close)
			{
				message = "Sql synax error!";
				return -1;
			}
			close = false;
		}
		if(newText[found]==','&&close)
			newText[found]=':';
		found = newText.find_first_of("(),",found+1);
	}

	vStr.clear();
	split(newText,", ",vStr);
	int ColIndex = 0;
	for(int i =0; i<vStr.size();i++)
	{
		if(vStr[i].size()>0)
		{
			found = vStr[i].find("as");
			if(found != string::npos)
			{
				found = vStr[i].find_first_of("(",found+2);
				found1 = vStr[i].find_first_of(")",found+1);
				if(string::npos==found 
				||string::npos == found1)
				{
					message ="Sql synax error!";
					return -1;
				}
				string asText =vStr[i].substr(found+1,found1-found-1);
				
				split(asText,": ",vAsStr);
				for(int j =0; j<vAsStr.size();j++)
				{
					vText.push_back(vAsStr[j]);			
				}
			}else
			{
				vText.push_back(vStr[i]);  
			}
		}
	}

	return 0;
}

int CSeasbaseVideoDlg::SaveData(char *buff,int len,int* fnoo)
{
	FILE *fp;
	CString fname;
	int fno =*fnoo;
	CHAR OutPath[MAX_PATH];  
	CString des="";
	//char tmp[4096];
	::GetTempPath(256,des.GetBuffer(256));
	des.ReleaseBuffer();
	//des+="\\";
	memcpy(OutPath,des,des.GetLength()+1); 
	//fname.Format("e:\\hadoop\\image\\frame%d.bmp",framenum++);
	//fname.Format("%simage\\frame%d.bmp",OutPath,framenum++);
	/*srand((unsigned)time(NULL));
	int i = rand();
	sprintf(tmp,"image%d",i);
	strcat(OutPath,tmp);*/
	//fname.Format("e:\\hadoop\\image\\frame%d.bmp",framenum++);
	CString strFrameID = m_listviewinfo.m_listinfo.GetItemText(fno,frameID);
	CString strTagID = m_listviewinfo.m_listinfo.GetItemText(fno,tagID);
	fname.Format("%sframe%s-%s.bmp",OutPath,strFrameID,strTagID);
	//fname.Format("%sframe%s-%s.bmp","E:\\bmp\\",strFrameID,strTagID);
	fp=fopen(fname,"wb");
	if(fp==NULL)
	{return 0;}
	fwrite(buff,len,1,fp);
	fclose(fp);
	
	return 1;
}

void UTF8toANSI(CString &strUTF8)  
{  
	//��ȡת��Ϊ���ֽں���Ҫ�Ļ�������С���������ֽڻ�����  
	UINT nLen = MultiByteToWideChar(CP_UTF8,NULL,strUTF8,-1,NULL,NULL);  
	WCHAR *wszBuffer = new WCHAR[nLen+1];  
	nLen = MultiByteToWideChar(CP_UTF8,NULL,strUTF8,-1,wszBuffer,nLen);  
	wszBuffer[nLen] = 0;  

	nLen = WideCharToMultiByte(936,NULL,wszBuffer,-1,NULL,NULL,NULL,NULL);  
	CHAR *szBuffer = new CHAR[nLen+1];  
	nLen = WideCharToMultiByte(936,NULL,wszBuffer,-1,szBuffer,nLen,NULL,NULL);  
	szBuffer[nLen] = 0;  

	strUTF8 = szBuffer;  
	//�����ڴ�  
	delete []szBuffer;  
	delete []wszBuffer;  
}  
double colorMomentHSV(IplImage* obj,IplImage* lib)
{
	IplImage* obj_hsv=cvCreateImage(cvGetSize(obj),8,3);	//ӳ�䵽hsv�ռ�
	IplImage* lib_hsv=cvCreateImage(cvGetSize(lib),8,3);
	cvCvtColor(obj,obj_hsv,CV_BGR2HSV);
	cvCvtColor(lib,lib_hsv,CV_BGR2HSV);

	IplImage* obj_h_plane=cvCreateImage(cvGetSize(obj),8,1);	//��ȡhsv����
	IplImage* obj_s_plane=cvCreateImage(cvGetSize(obj),8,1);
	IplImage* obj_v_plane=cvCreateImage(cvGetSize(obj),8,1);
	IplImage* obj_planes[]={obj_h_plane,obj_s_plane};
	cvCvtPixToPlane(obj_hsv,obj_h_plane,obj_s_plane,obj_v_plane,0);
	IplImage* lib_h_plane=cvCreateImage(cvGetSize(lib),8,1);	
	IplImage* lib_s_plane=cvCreateImage(cvGetSize(lib),8,1);
	IplImage* lib_v_plane=cvCreateImage(cvGetSize(lib),8,1);
	IplImage* lib_planes[]={lib_h_plane,lib_s_plane};
	cvCvtPixToPlane(lib_hsv,lib_h_plane,lib_s_plane,lib_v_plane,0);

	int h_bins=16;		//��ʼ��hist
	int s_bins=8;
	int hist_size[]={h_bins,s_bins};
	float h_ranges[]={0,180};
	float s_ranges[]={0,255};
	float* ranges[]={h_ranges,s_ranges};
	CvHistogram* obj_hist=cvCreateHist(2,hist_size,CV_HIST_ARRAY,ranges,1);	
	CvHistogram* lib_hist=cvCreateHist(2,hist_size,CV_HIST_ARRAY,ranges,1);	

	cvCalcHist(obj_planes,obj_hist,0,0);		//����ֱ��ͼ
	cvCalcHist(lib_planes,lib_hist,0,0);

	double* h_obj=new double[h_bins];
	double* s_obj=new double[s_bins];
	double* h_lib=new double[h_bins];
	double* s_lib=new double[s_bins];

	memset(h_obj,0,h_bins*sizeof(double));
	memset(s_obj,0,s_bins*sizeof(double));
	memset(h_lib,0,h_bins*sizeof(double));
	memset(s_lib,0,s_bins*sizeof(double));

	for(int i=0;i<h_bins;i++)
		for(int j=0;j<s_bins;j++)
		{
			double temp_obj=cvQueryHistValue_2D(obj_hist,i,j);
			double temp_lib=cvQueryHistValue_2D(lib_hist,i,j);
			h_obj[i]+=temp_obj;
			s_obj[j]+=temp_obj;
			h_lib[i]+=temp_lib;
			s_lib[j]+=temp_lib;
		}

		double sum_h_obj=0;					//���ֵ
		double sum_h_lib=0;
		double sum_s_obj=0;
		double sum_s_lib=0;
		for(int i=0;i<h_bins;i++)
		{
			sum_h_obj+=h_obj[i];
			sum_h_lib+=h_lib[i];
		}
		for(int i=0;i<s_bins;i++)
		{
			sum_s_obj+=s_obj[i];
			sum_s_lib+=s_lib[i];
		}
		double ave_h_obj=sum_h_obj/h_bins;
		double ave_h_lib=sum_h_lib/h_bins;
		double ave_s_obj=sum_s_obj/s_bins;
		double ave_s_lib=sum_s_lib/s_bins;

		double men2_h_obj=0;				//����׾�
		double men2_h_lib=0;
		double men2_s_obj=0;
		double men2_s_lib=0;
		for(int i=0;i<h_bins;i++)
		{
			men2_h_obj+=(h_obj[i]-ave_h_obj)*(h_obj[i]-ave_h_obj);
			men2_h_lib+=(h_lib[i]-ave_h_lib)*(h_lib[i]-ave_h_lib);
		}
		for(int i=0;i<s_bins;i++)
		{
			men2_s_obj+=(s_obj[i]-ave_s_obj)*(s_obj[i]-ave_s_obj);
			men2_s_lib+=(s_lib[i]-ave_s_lib)*(s_lib[i]-ave_s_lib);
		}
		men2_h_obj=sqrt(men2_h_obj/h_bins);
		men2_h_lib=sqrt(men2_h_lib/h_bins);
		men2_s_obj=sqrt(men2_s_obj/s_bins);
		men2_s_lib=sqrt(men2_s_lib/s_bins);

		double men3_h_obj=0;				//�����׾�
		double men3_h_lib=0;
		double men3_s_obj=0;
		double men3_s_lib=0;
		for(int i=0;i<h_bins;i++)
		{
			men3_h_obj+=(h_obj[i]-ave_h_obj)*(h_obj[i]-ave_h_obj)*(h_obj[i]-ave_h_obj);
			men3_h_lib+=(h_lib[i]-ave_h_lib)*(h_lib[i]-ave_h_lib)*(h_lib[i]-ave_h_lib);
		}
		for(int i=0;i<s_bins;i++)
		{
			men3_s_obj+=(s_obj[i]-ave_s_obj)*(s_obj[i]-ave_s_obj)*(s_obj[i]-ave_s_obj);
			men3_s_lib+=(s_lib[i]-ave_s_lib)*(s_lib[i]-ave_s_lib)*(s_lib[i]-ave_s_lib);
		}
		men3_h_obj=pow(men3_h_obj,1.0/3);
		men3_h_lib=pow(men3_h_lib,1.0/3);
		men3_s_obj=pow(men3_s_obj,1.0/3);
		men3_s_lib=pow(men3_s_lib,1.0/3);

		double s=0.2*abs(ave_h_lib-ave_h_obj)+0.2*abs(ave_s_lib-ave_s_obj)+
			0.4*abs(men2_h_lib-men2_h_obj)+0.4*abs(men2_s_lib-men2_s_obj)+
			0.4*abs(men3_h_lib-men3_h_obj)+0.4*abs(men3_s_lib-men3_s_obj);

		delete [] h_obj;
		delete [] s_obj;
		delete [] h_lib;
		delete [] s_lib;

		return s;
}
double colorMomentRGB(IplImage* obj,IplImage* lib)
{
	int r_bins=256;		
	int g_bins=256;
	int b_bins=256;
	int hist_r_size[]={r_bins};
	int hist_g_size[]={g_bins};
	int hist_b_size[]={b_bins};
	float r_ranges[]={0,255};
	float g_ranges[]={0,255};
	float b_ranges[]={0,255};
	float* ranges_r[]={r_ranges};
	float* ranges_g[]={g_ranges};
	float* ranges_b[]={b_ranges};
	CvHistogram* obj_hist_r=cvCreateHist(1,hist_r_size,CV_HIST_ARRAY,ranges_r,1);
	CvHistogram* obj_hist_g=cvCreateHist(1,hist_g_size,CV_HIST_ARRAY,ranges_g,1);
	CvHistogram* obj_hist_b=cvCreateHist(1,hist_b_size,CV_HIST_ARRAY,ranges_b,1);
	CvHistogram* lib_hist_r=cvCreateHist(1,hist_r_size,CV_HIST_ARRAY,ranges_r,1);
	CvHistogram* lib_hist_g=cvCreateHist(1,hist_g_size,CV_HIST_ARRAY,ranges_g,1);
	CvHistogram* lib_hist_b=cvCreateHist(1,hist_b_size,CV_HIST_ARRAY,ranges_b,1);

	IplImage* obj_r_plane=cvCreateImage(cvGetSize(obj),8,1);
	IplImage* obj_g_plane=cvCreateImage(cvGetSize(obj),8,1);
	IplImage* obj_b_plane=cvCreateImage(cvGetSize(obj),8,1);
	IplImage* lib_r_plane=cvCreateImage(cvGetSize(lib),8,1);
	IplImage* lib_g_plane=cvCreateImage(cvGetSize(lib),8,1);
	IplImage* lib_b_plane=cvCreateImage(cvGetSize(lib),8,1);
	cvCvtPixToPlane(obj,obj_r_plane,obj_g_plane,obj_b_plane,0);
	cvCvtPixToPlane(lib,lib_r_plane,lib_g_plane,lib_b_plane,0);
	IplImage* obj_r_planes[]={obj_r_plane};
	IplImage* obj_g_planes[]={obj_g_plane};
	IplImage* obj_b_planes[]={obj_b_plane};
	IplImage* lib_r_planes[]={lib_r_plane};
	IplImage* lib_g_planes[]={lib_g_plane};
	IplImage* lib_b_planes[]={lib_b_plane};
	cvCalcHist(obj_r_planes,obj_hist_r,0,0);
	cvCalcHist(obj_g_planes,obj_hist_g,0,0);
	cvCalcHist(obj_b_planes,obj_hist_b,0,0);
	cvCalcHist(lib_r_planes,lib_hist_r,0,0);
	cvCalcHist(lib_g_planes,lib_hist_g,0,0);
	cvCalcHist(lib_b_planes,lib_hist_b,0,0);

	double* obj_r=new double[r_bins];
	double* obj_g=new double[g_bins];
	double* obj_b=new double[b_bins];
	double* lib_r=new double[r_bins];
	double* lib_g=new double[g_bins];
	double* lib_b=new double[b_bins];
	memset(obj_r,0,r_bins*sizeof(double));
	memset(obj_g,0,g_bins*sizeof(double));
	memset(obj_b,0,b_bins*sizeof(double));
	memset(lib_r,0,r_bins*sizeof(double));
	memset(lib_g,0,g_bins*sizeof(double));
	memset(lib_b,0,b_bins*sizeof(double));

	int i=0;
	double obj_r_ave=0;			//rͨ����ֵ
	double lib_r_ave=0;
	for(i=0;i<r_bins;i++)
	{
		obj_r[i]=cvQueryHistValue_1D(obj_hist_r,i);
		obj_r_ave+=obj_r[i];
		lib_r[i]=cvQueryHistValue_1D(lib_hist_r,i);
		lib_r_ave+=lib_r[i];
	}
	obj_r_ave=obj_r_ave/r_bins;
	lib_r_ave=lib_r_ave/r_bins;

	double obj_g_ave=0;			//gͨ����ֵ
	double lib_g_ave=0;
	for(i=0;i<g_bins;i++)
	{
		obj_g[i]=cvQueryHistValue_1D(obj_hist_g,i);
		obj_g_ave+=obj_g[i];
		lib_g[i]=cvQueryHistValue_1D(lib_hist_g,i);
		lib_g_ave+=lib_g[i];
	}
	obj_g_ave=obj_g_ave/g_bins;
	lib_g_ave=lib_g_ave/g_bins;

	double obj_b_ave=0;			//bͨ����ֵ
	double lib_b_ave=0;
	for(i=0;i<b_bins;i++)
	{
		obj_b[i]=cvQueryHistValue_1D(obj_hist_b,i);
		obj_b_ave+=obj_b[i];
		lib_b[i]=cvQueryHistValue_1D(lib_hist_b,i);
		lib_b_ave+=lib_b[i];
	}
	obj_b_ave=obj_b_ave/b_bins;
	lib_b_ave=lib_b_ave/b_bins;

	double obj_r_moment2=0;		//rͨ�����׾�
	double lib_r_moment2=0;
	for(i=0;i<r_bins;i++)
	{
		obj_r_moment2+=(obj_r[i]-obj_r_ave)*(obj_r[i]-obj_r_ave);
		lib_r_moment2+=(lib_r[i]-lib_r_ave)*(lib_r[i]-lib_r_ave);
	}
	obj_r_moment2=sqrt(obj_r_moment2/r_bins);
	lib_r_moment2=sqrt(lib_r_moment2/r_bins);

	double obj_g_moment2=0;		//gͨ�����׾�
	double lib_g_moment2=0;
	for(i=0;i<g_bins;i++)
	{
		obj_g_moment2+=(obj_g[i]-obj_g_ave)*(obj_g[i]-obj_g_ave);
		lib_g_moment2+=(lib_g[i]-lib_g_ave)*(lib_g[i]-lib_g_ave);
	}
	obj_g_moment2=sqrt(obj_g_moment2/g_bins);
	lib_g_moment2=sqrt(lib_g_moment2/g_bins);

	double obj_b_moment2=0;		//bͨ�����׾�
	double lib_b_moment2=0;
	for(i=0;i<b_bins;i++)
	{
		obj_b_moment2+=(obj_b[i]-obj_b_ave)*(obj_b[i]-obj_b_ave);
		lib_b_moment2+=(lib_b[i]-lib_b_ave)*(lib_b[i]-lib_b_ave);
	}
	obj_b_moment2=sqrt(obj_b_moment2/b_bins);
	lib_b_moment2=sqrt(lib_b_moment2/b_bins);

	double obj_r_moment3=0;		//rͨ�����׾�
	double lib_r_moment3=0;
	for(i=0;i<r_bins;i++)
	{
		obj_r_moment3+=(obj_r[i]-obj_r_ave)*(obj_r[i]-obj_r_ave)*(obj_r[i]-obj_r_ave);
		lib_r_moment3+=(lib_r[i]-lib_r_ave)*(lib_r[i]-lib_r_ave)*(lib_r[i]-lib_r_ave);
	}
	obj_r_moment3=pow(obj_r_moment3/r_bins,1.0/3);
	lib_r_moment3=pow(lib_r_moment3/r_bins,1.0/3);

	double obj_b_moment3=0;		//bͨ�����׾�
	double lib_b_moment3=0;
	for(i=0;i<b_bins;i++)
	{
		obj_b_moment3+=(obj_b[i]-obj_b_ave)*(obj_b[i]-obj_b_ave)*(obj_b[i]-obj_b_ave);
		lib_b_moment3+=(lib_b[i]-lib_b_ave)*(lib_b[i]-lib_b_ave)*(lib_b[i]-lib_b_ave);
	}
	obj_b_moment3=pow(obj_b_moment3/b_bins,1.0/3);
	lib_b_moment3=pow(lib_r_moment3/b_bins,1.0/3);

	double obj_g_moment3=0;		//gͨ�����׾�
	double lib_g_moment3=0;
	for(i=0;i<g_bins;i++)
	{
		obj_g_moment3+=(obj_g[i]-obj_g_ave)*(obj_g[i]-obj_g_ave)*(obj_g[i]-obj_g_ave);
		lib_g_moment3+=(lib_g[i]-lib_g_ave)*(lib_g[i]-lib_g_ave)*(lib_g[i]-lib_g_ave);
	}
	obj_g_moment3=pow(obj_g_moment3/g_bins,1.0/3);
	lib_g_moment3=pow(lib_g_moment3/g_bins,1.0/3);

	double s=abs(lib_r_ave-obj_r_ave)+abs(lib_g_ave-obj_g_ave)+abs(lib_b_ave-obj_b_ave)		//һ�׾�
		+abs(lib_r_moment2-obj_r_moment2)+abs(lib_g_moment2-obj_g_moment2)+abs(lib_b_moment2-obj_b_moment2)		//���׾�
		+abs(lib_r_moment3-obj_r_moment3)+abs(lib_g_moment3-obj_g_moment3)+abs(lib_b_moment3-obj_b_moment3);

	cvReleaseImage(&obj);		//�ͷ��ڴ�
	cvReleaseImage(&lib);
	cvReleaseHist(&obj_hist_r);
	cvReleaseHist(&obj_hist_g);
	cvReleaseHist(&obj_hist_b);
	cvReleaseHist(&lib_hist_r);
	cvReleaseHist(&lib_hist_g);
	cvReleaseHist(&lib_hist_b);
	cvReleaseImage(&obj_r_plane);
	cvReleaseImage(&obj_b_plane);
	cvReleaseImage(&obj_g_plane);
	cvReleaseImage(&lib_r_plane);
	cvReleaseImage(&lib_g_plane);
	cvReleaseImage(&lib_b_plane);
	delete [] obj_r;
	delete [] obj_g;
	delete [] obj_b;
	delete [] lib_r;
	delete [] lib_g;
	delete [] lib_b;

	return s;
}
int SaveDatas(char *buff)
{
	FILE *fp;
	fp=fopen("infom.txt","at+");
	if(fp==NULL)
	{return 0;}
	//fwrite(buff,sizeof(char),100,fp);
	fputs(buff,fp);
	fclose(fp);
	return 1;
}
void CSeasbaseVideoDlg::OnBnClickedBinquire()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	 //m_listimages.sPath =strVideoPath;
	 CString EditText;
		/*CTime l_date;
		CString l_csDate;
		UpdateData(TRUE);
		m_inquireUI.m_ctlBeginDate.GetTime(l_date);
		l_csDate = l_date.Format( "%Y-%m-%d " );

		CTime l_time;
		CString l_csTime;
		UpdateData(TRUE);
		m_inquireUI.m_ctlBeginTime.GetTime(l_time);
		l_csTime = l_time.Format( "%H:%M:%S " );*/

        /*CString l_csDateTime;
		l_csDateTime = "��ʼ���ڣ�"+l_csDate+" ��ʼʱ�䣺"+l_csTime;*/
//		EditText.Format("select frameid,R,G,B,HM,HS,HT,frame from %s limit 20    start datetime %s,%s",tablename,l_csDate,l_csTime);
	 GetDlgItemTextA(IDC_ETEXT,EditText);
	vector<string> vStr;
	CString table;
	CString message;
	char buff[204800];

	m_listviewinfo.m_listinfo.DeleteAllItems();
	for(int c = 0; c<ColIndex;c++)
		m_listviewinfo.m_listinfo.DeleteColumn(0);
	m_listimages.m_image.DeleteAllItems();

	img.DeleteImageList();

	CString str = EditText;
	string editText = EditText.MakeLower();
	frameID =0;
	tagID=0;
	imgWidthID =0;
	imgHeightID =0;
	ColIndex =0;
	/*if( std::string::npos!=editText.find("insert")
		||(std::string::npos!=editText.find("create")
		&&std::string::npos==editText.find("select"))
		||std::string::npos!=editText.find("show")
		||std::string::npos!=editText.find("desc"))//���û�ҵ��򷵻�string::npos
	{
		//sprintf(buff,"/c hive -e \"%s\" ",editText);
		sprintf(buff,"/c hive -e \"%s\"  ",str);
		SeasbaseRunExec(_T("cmd"),buff, INFINITE,true);
		//message = "error!";
		m_listdescinfo.m_listmessage.InsertString(nCnt++,message);
		m_subtab.SetCurFocus(2);
		int nCount = m_listdescinfo.m_listmessage.GetCount();
		if (nCount > 0)
			m_listdescinfo.m_listmessage.SetCurSel(nCount - 1); 
		m_listdescinfo.m_listmessage.SetTopIndex(nCount - 1);
		return;
	}
	int ret = Parser( EditText,vStr,table,message);
	if(ret == -1)
	{
		//��message��ʾ��message tab page ��
		m_listdescinfo.m_listmessage.InsertString(nCnt++,message);
		m_subtab.SetCurFocus(2);
		int nCount = m_listdescinfo.m_listmessage.GetCount();
		if (nCount > 0)
			m_listdescinfo.m_listmessage.SetCurSel(nCount - 1); 
		m_listdescinfo.m_listmessage.SetTopIndex(nCount - 1);
		return;
	}*/

//	if (vStr.size()==1
//		&& std::string::npos!=vStr[0].find("*"))
//	{
//		sprintf(buff,"/c hive -e \"desc %s\" >type.txt",table);
//		SeasbaseRunExec(_T("cmd"),buff, INFINITE,true);
//		//InsertColumn
//		fstream fin("type.txt");  //���ļ�
//		string ReadLine; 
//		bool bFirstSubItem = true;
//		while(getline(fin,ReadLine))  //���ж�ȡ��ֱ������    
//		{     
//			vector<string> d;
//			split(ReadLine," \t\n",d);
//
//			int SubItemIndex=1;
//			int nIndex = m_listviewinfo.m_listinfo.GetItemCount();
//
//			CString colname=d[0].c_str();
//			m_listviewinfo.m_listinfo.InsertColumn(ColIndex,colname,LVCFMT_LEFT,100,ColIndex++);//������ 
//
//		}
//}else 
	{
		sprintf(buff,"/c hive -e \"explain %s\" >type.txt ",editText.c_str());
		SeasbaseRunExec(_T("cmd"),buff, INFINITE,true);
		//InsertColumn
		fstream fin("type.txt");  //���ļ�
		string ReadLine; 
		while(getline(fin,ReadLine))  //���ж�ȡ��ֱ������    
		{     
			if (std::string::npos!=ReadLine.find("expressions"))
			{
              CString strLine=ReadLine.c_str();
			  int exinx = strLine.Find("expressions");
			  strLine.Delete(0,exinx+12);
			  do 
			  {
				  int linx = strLine.Find("(");
			      int rinx = strLine.Find(")");
				  if (linx!=-1
					  &&rinx!=-1)
				  {
					  if(linx<rinx)
					      strLine.Delete(linx,rinx-linx+1);
				      else
                          strLine.Delete(rinx,rinx);
				  }
				  
			  } while (strLine.Find(")")!=-1);
			  string editText =strLine.MakeLower();
			  vector<string> vStr;
			  split(editText,",",vStr);
			  for(int i =0; i<vStr.size();i++)
			  {
				  if(vStr[i].size()>1)
				  {
					  CString colname=vStr[i].c_str();
					  if (strcmp(colname.MakeLower()," frameid ")==0)
					  {
						  frameID =ColIndex;
					  }
					  if (strcmp(colname.MakeLower()," targetnum ")==0)
					  {
						  tagID =ColIndex;
					  }
					  if (strcmp(colname.MakeLower()," width ")==0)
					  {
						  imgWidthID =ColIndex;
					  }
					  if (strcmp(colname.MakeLower()," height ")==0)
					  {
						  imgHeightID =ColIndex;
					  }
					  m_listviewinfo.m_listinfo.InsertColumn(ColIndex,colname,LVCFMT_LEFT,100,ColIndex++);//������ 
				  }
			  }
			  break;
			}
		}
	/*		for(int i = 0; i< vStr.size();i++)
		{
			CString colname=vStr[i].c_str();
			if (strcmp(colname.MakeLower(),"frameid")==0)
			{
				frameID =ColIndex;
			}
			if (strcmp(colname.MakeLower(),"targetnum")==0)
			{
				tagID =ColIndex;
			}
			m_listviewinfo.m_listinfo.InsertColumn(ColIndex,colname,LVCFMT_LEFT,100,ColIndex++);//������ 
			
		}*/
	}
	string ReadLine;
	int len =0;
	int fno=0;
	BYTE  output[2048*120];
	//BYTE*  output=NULL;
	CString path="";
	int iSubTypeIndex = 0;
	char buf[128];
		sprintf(buff,"/c hive -e \"%s\" 1>data.txt",str); //2>err.txt
		SeasbaseRunExec(_T("cmd"),buff, INFINITE,true);
		path = "data.txt"; 

	    fstream fsin(path);  //���ļ�   
		while(getline(fsin,ReadLine))  //���ж�ȡ��ֱ������    
		{      
			vector<string> d;
			vector<string>::iterator p;

			split(ReadLine,"\t\n",d);
			bool bFirstSubItem = true;
			int SubItemIndex=1;
			int icnt =0;
			int nIndex = m_listviewinfo.m_listinfo.GetItemCount();
			CString strWidth,strHeight;
			
			for(p=d.begin();p!=d.end();++p,icnt++)
			{
				if (icnt==imgWidthID)
				{
					strWidth = p->c_str();
				}
				if (icnt==imgHeightID)
				{
					strHeight = p->c_str();
				}

			}
			for(p=d.begin();p!=d.end();++p)
			{
				if (p->length()>10000)//std::string::npos!=p->find("\\")
				{
					len = BASE64_Decode(p->c_str(),p->length(),output);
					
					/*cvSetData(subTarImg,output,atoi(strWidth)*3);*/					
					//double hsvMoment = colorMomentHSV(pImg,subTarImg);
					//double rgbMoment = colorMomentRGB(tagImg,subTarImg);

					SaveData((char *)output,len,&fno);
					//cvReleaseImageHeader(&subTarImg);
					fno++;
				}
				else
				{
					//�²�������������� 
					LV_ITEM lvItem;
					if(bFirstSubItem)
					{
						lvItem.mask = LVIF_TEXT ; 
						lvItem.iItem = nIndex;     //����
						lvItem.iSubItem = 0;
						lvItem.pszText = (LPSTR)(&(*p->c_str()));  //��һ��
						//�����һ�в����¼ֵm_listviewinfo.m_listinfo..
						m_listviewinfo.m_listinfo.InsertItem(&lvItem);
						bFirstSubItem = false;

					}
					else
						//����������
					{
						const char* s = &(*p->c_str());
						CString ss(s);
						UTF8toANSI(ss);
						m_listviewinfo.m_listinfo.SetItemText(nIndex,SubItemIndex++,ss);//(LPCTSTR)(&(*p->c_str()))
						 
					}
				}

			}
		}
		fsin.close();

	m_listimages.m_image.SetIconSpacing(180,155);//140 130
	//*����ͼ���б�imagesѡ��*/
	img.Create(160,130,ILC_COLORDDB|ILC_COLOR32,1,1);//120 90

	//report���
	//CBitmap* m_bitmap;
	//for (int i = 0;i<framenum;i++)
	//{
	//	m_bitmap = new CBitmap;
	//	CString bitmapname;
	//	bitmapname.Format("E:\\RedHadoop Project\\RedHadoop Workstation\\RedHadoopWorkstation0.1\\RedHadoopWorkstation0.1\\CmdGUI\\frames%d.bmp",i);
	//	//BOOL bSucess =m_bitmap->LoadBitmapA(IDB_BITMAP2);
	//	HBITMAP bitmap;
	//	bitmap=(HBITMAP)LoadImage(AfxGetInstanceHandle(),bitmapname,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

	//	m_bitmap->DeleteObject();
	//	BOOL bSucess = m_bitmap->Attach(bitmap);
	//	//BOOL bSucess = m_bitmap->LoadBitmapA(bitmapname);
	//	img.Add(m_bitmap,RGB(0,255,255));
	//	
	//	delete m_bitmap;
	//	m_bitmap = NULL;
	//}

	m_listimages.m_image.SetImageList(&img,LVSIL_NORMAL); //LVSIL_SMALL

	m_listimages.m_image.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//LVS_EX_SUBITEMIMAGES report���
	int nIndex = m_listviewinfo.m_listinfo.GetItemCount();
	cvSetImageROI(pImg,roi_rect_src);
	IplImage* srcImg = NULL;
	//for (int i = 0;i<fno;i++)
	for (int i=0;i<nIndex;i++)
	{
		CHAR OutPath[MAX_PATH];  
		CString des="";
	//	char tmp[4096];
		::GetTempPath(256,des.GetBuffer(256));
		des.ReleaseBuffer();
		des+="\\";
		memcpy(OutPath,des,des.GetLength()+1); 
		CString strFrameID = m_listviewinfo.m_listinfo.GetItemText(i,frameID);
		CString strTagID = m_listviewinfo.m_listinfo.GetItemText(i,tagID);
		CString bitmapname;
    //	bitmapname.Format("e:\\hadoop\\image\\frame%s.bmp",strFrameID);
	   //Format("%simage\\frame%d.bmp",OutPath,i);
       //Format("e:\\hadoop\\image\\frame%d.bmp",i);	

		bitmapname.Format("%sframe%s-%s.bmp",OutPath,strFrameID,strTagID);
		subTarImg = cvLoadImage(bitmapname,1);
		srcImg = cvCreateImage(cvSize(roi_rect_src.width,roi_rect_src.height),IPL_DEPTH_8U,3);
		cvCopyImage(pImg,srcImg);
		double hsvMoment=0,rgbMoment=0;
		if(  subTarImg ) 
		{
			
			sprintf(buf,"E:\\bmp\\%s-%s.bmp",strFrameID,strTagID);
			cvSaveImage(buf,subTarImg);
			hsvMoment = colorMomentHSV(srcImg,subTarImg);
			rgbMoment = colorMomentRGB(srcImg,subTarImg);
		}
		else
			continue;
		USES_CONVERSION;
		//WCHAR* pBuf = A2W((LPCTSTR)vImageFiles[i].c_str()); 
		WCHAR* pBuf = T2W((LPCTSTR)bitmapname); 
		Bitmap bmp(pBuf);

		int sourceWidth = 160;                              //���ͼƬ���,���120�ʹ�����120������ͬ��
		int sourceHeight = bmp.GetHeight();                 //���ͼƬ���  
		
		if(sourceHeight>160)             //��m_ImageList.Create(120��)�кܴ��ϵ�����С����
			//���Ŀ�ȣ�ͼƬ���޷���ʾ��
		{
			sourceHeight=160;
		}
	
		Bitmap* pThumbnail = (Bitmap*)bmp.GetThumbnailImage(sourceWidth , sourceHeight , NULL, NULL); //�趨����ͼ�Ĵ�С
		HBITMAP hBmp;
		pThumbnail->GetHBITMAP(Color(255,255,255),&hBmp);
		CBitmap *pImage = CBitmap::FromHandle(hBmp);         //ת����CBitmap��ʽλͼ
		int a=img.Add(pImage,RGB(255,255, 255));
		if (hsvMoment>40000.000//25000.000       1000.000
			&&rgbMoment>2000.000)
		{
             bitmapname.Format("frame%s-%s.bmp",strFrameID,strTagID);
		     m_listimages.m_image.InsertItem(i ,bitmapname,i);
			 sprintf(buff,"frame%s-%s.bmp,hsv:%.3f,rgb:%.3f\n",strFrameID,strTagID,hsvMoment,rgbMoment);
			 SaveDatas(buff);
		}
		//cvReleaseImage(&srcImg);
		//m_listimages.m_image.InsertItem(i ,vImageFiles[i].c_str(),i);	
		//m_listimages.m_image.InsertItem(LVIF_TEXT |LVIF_STATE, i, NULL, 
		//	(i % 2) == 0 ? LVIS_SELECTED : 0, LVIS_SELECTED, i, 0);
	}
	framenum=0;
	cvResetImageROI(pImg);
	
}

void CSeasbaseVideoDlg::OnBnClickedBload()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_treeview.DeleteAllItems();
	hRoot =m_treeview.InsertItem("Default",0,0);
	m_treeview.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	//-----------------------------��ʼItem�ڵ�ֵ-----------------------------------//
	for(int i=0 ;i<maxItemCount; i++)
	{
		ItemList[i].ItemFlag	= 0;				//0������ڵ�,����
		ItemList[i].ItemName	= "";
		ItemList[i].Value		= "";
		ItemList[i].ItemParent	= 0;
	}
	m_treeview.Expand(hRoot,TVE_EXPAND);
	
	SeasbaseRunExec(_T("cmd"),_T("/c hive -e \"show tables\" >tables.txt"), INFINITE,true);
	fstream fin("tables.txt");  //���ļ�    
	string ReadLine; 
	int NodeIndex=1;
	while(getline(fin,ReadLine))  //���ж�ȡ��ֱ������ 
	{
		ItemList[NodeIndex].ItemFlag		= 1;				//1����1���ڵ�
		ItemList[NodeIndex].ItemName		= ReadLine.c_str();
		ItemList[NodeIndex++].ItemParent		= 0;				//���ڵ�Ϊ����ĵ�0��	
	}
	HTREEITEM  newItem;
	int itemCount	= GetItemCount(ItemList);				//���ͽڵ���
	int addedItemC	= 0;									//ͳ���Ѿ������˶��ٽڵ�,������������еĽڵ���,������
	for(int i=1; i < itemCount+1; i++)
	{
		if(ItemList[i].ItemParent == 0 )					//�ж��Ƿ�Ϊ���ڵ���ӽڵ�,�������
		{			
			//���1���ӽڵ�
			newItem		= m_treeview.InsertItem(ItemList[i].ItemName ,1 , 1, hRoot);
			m_treeview.EnsureVisible(newItem);	
			addedItemC++;
		}
		if(addedItemC == itemCount)break;

	}
}
void CSeasbaseVideoDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int CurSel = m_subtab.GetCurSel();
	switch(CurSel)
	{
	case 0:
		m_listimages.ShowWindow(true);
		m_listviewinfo.ShowWindow(false);
		m_listdescinfo.ShowWindow(false);
		break;
	case 1:
		m_listimages.ShowWindow(false);
		m_listviewinfo.ShowWindow(true);		
		m_listdescinfo.ShowWindow(false);
		break;
	case 2:
		m_listimages.ShowWindow(false);
		m_listviewinfo.ShowWindow(false);
		m_listdescinfo.ShowWindow(true);
	default:;
	} 
	*pResult = 0;
}

void CSeasbaseVideoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
//
//	// TODO: �ڴ˴������Ϣ����������
//	float   dx_percent   =   (m_old_cx  ==0)?   1   :   (float)((float)cx/(float)m_old_cx); 
//	float   dy_percent   =   (m_old_cy  ==0)?   1   :     (float)((float)cy/(float)m_old_cy);
//
//	if(m_old_cx)
//	{
//		CWnd *pWnd; 
//		for(int i=0;i<m_control_info.GetSize();i++)
//		{
//			pWnd =GetDlgItem(m_control_info[i]);
//		
//		   if(pWnd && nType!=1)//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ���� 
//		  {  
//			CRect rect;   //��ȡ�ؼ��仯ǰ��С  
//			pWnd->GetWindowRect(&rect);  
//			ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������  
//			// cx/m_rect.Width()Ϊ�Ի����ں���ı仯����  
//			rect.left=rect.left*cx/m_rect.Width();/////�����ؼ���С  
//			rect.right=rect.right*cx/m_rect.Width();  
//			rect.top=rect.top*cy/m_rect.Height();  
//			rect.bottom=rect.bottom*cy/m_rect.Height();  
//			pWnd->MoveWindow(rect);//���ÿؼ���С 
//		  }
//		}
//	}
//	m_old_cx   =   cx; 
//	m_old_cy   =   cy; 
//	//GetClientRect(&m_rect);//���仯��ĶԻ����С��Ϊ�ɴ�С
////	UPDATE_EASYSIZE;
//
}

void CSeasbaseVideoDlg::SetControlInfo(WORD CtrlId)
{
	m_control_info.Add(CtrlId); 

}

void CSeasbaseVideoDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	
	// TODO: �ڴ˴������Ϣ����������
	GdiplusShutdown( m_gdiplusToken );	
}

void CSeasbaseVideoDlg::OnBnClickedBloadvideo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgVideoLoad video =new CDlgVideoLoad();
	int ibt =video.DoModal();
}

void CSeasbaseVideoDlg::OnBnClickedLoadimage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE, 
		"Image Files (*bmp)", 
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_ENABLESIZING,
		_T("Image Files (*.jpg;*.bmp;*.png;*.icon;*.gif)|*.jpg;*.bmp;*.png;*.icon;*.gif||"),
		NULL);
	dlg.m_ofn.lpstrTitle=_T("Please select an image");

	dlg.m_ofn.nMaxFile = MAX_PATH; 
	dlg.m_ofn.lpstrFile = new TCHAR[dlg.m_ofn.nMaxFile];  
	ZeroMemory(dlg.m_ofn.lpstrFile, sizeof(TCHAR) * dlg.m_ofn.nMaxFile); 
	int retval = dlg.DoModal();
	if(retval==IDCANCEL)
		return;
	POSITION pos_file;
	pos_file = dlg.GetStartPosition();
	CArray<CString, CString> ary_filename;
	while(pos_file != NULL)
		ary_filename.Add(dlg.GetNextPathName(pos_file));
	this->SetDlgItemTextA(IDC_ImagePath,ary_filename.GetAt(0));
	//sInfo= ary_filename.GetAt(0);
	CImage myImage;
	myImage.Load(ary_filename.GetAt(0));
	IplImage* copyImg = cvCreateImage(cvSize(myImage.GetWidth(),myImage.GetHeight()),IPL_DEPTH_8U,3);

	copyImg = cvLoadImage(ary_filename.GetAt(0),1);

	//ResizeImage( myImage );    // �Զ����ͼƬ��������

	GetClientRect(&windowRect);
	ClientToScreen(&windowRect);
	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_IMAGE); // (�����ڴ���Դ����������ڲ�, �������ⲿ, ����ͨ�����CMainFrame��ָ��, ��ͨ��pMianFrame->GetDlgItem(IDCk_MY_PIC)����ȡ)
	CDC *pDC = pWnd->GetDC();
	pWnd->GetClientRect(&rect);
	pDC->SetStretchBltMode(STRETCH_HALFTONE);
	myImage.Draw(pDC->m_hDC, rect);
	
	
	// ���ͼƬ�ؼ��Ŀ�͸� 
	int rw = rect.right - rect.left;  
	int rh = rect.bottom - rect.top; 
	pImg = cvCreateImage(cvSize(rw,rh),IPL_DEPTH_8U,3);
	cvResize(copyImg,pImg,CV_INTER_LINEAR);
    pWnd->ClientToScreen(&rect);
   //�����ؼ��ڴ����е�λ�ã����ԸĶ���ֵ��ʹ�Ӵ���Ĵ�С���ʣ�
    rect.top-=windowRect.top;
    rect.bottom-=windowRect.top;//45
    rect.left-=windowRect.left;
    rect.right-=windowRect.left;//6
	CDC *pDc; pDc = GetDC(); 
	CBrush cbr; 
	cbr.CreateStockObject(NULL_BRUSH);  //����һ���ջ�ˢ 
	CBrush *pOldBr = pDc->SelectObject(&cbr); 
	CPen cpen; 
	cpen.CreatePen(PS_SOLID, 1, RGB( 0, 0, 255)); //����һ����ɫ�Ļ���
	CPen *pOldPen = pDc->SelectObject(&cpen);
	//pDc->Rectangle(windowRect.left,windowRect.top,windowRect.right,windowRect.bottom);
	pDc->Rectangle(rect.left,rect.top,rect.right,rect.bottom);
	//myImage.Draw(pDC->m_hDC, rect);
	ReleaseDC(pDC);
	myImage.Destroy();
	cvReleaseImage(&copyImg);
}
void RGBtoHSV( BYTE r, BYTE g, BYTE b, float &h, float &s, float &v )
{
	BYTE min, max;
	float delta;

	min = MIN( r, g, b );
	max = MAX( r, g, b );
	v = max; // v
	delta = max - min;

	if( max != 0 )
	{
		s = delta / max; // s
	}
	else
	{
		// r = g = b = 0 // s = 0, v is undefined
		s = 0;
		h = -1;
		return;
	}

	if( r == max )
	{
		h = ( g - b ) / delta; // between yellow & magenta
	}
	else if( g == max )
	{
		h = 2 + ( b - r ) / delta; // between cyan & yellow
	}
	else
	{
		h = 4 + ( r - g ) / delta; // between magenta & cyan
	}

	h *= 60; // degrees

	if( h < 0 )
	{
		h += 360;
	}
} 
void CSeasbaseVideoDlg::HistogramProcess(IplImage* tagImage)
{
	IplImage* hsv = cvCreateImage( cvGetSize(tagImage), 8, 3 );//hsvɫ�ʿռ�(hueɫ�� saturation���Ͷ� value����)
	IplImage* h_plane = cvCreateImage( cvGetSize(tagImage), 8, 1 );
	IplImage* s_plane = cvCreateImage( cvGetSize(tagImage), 8, 1 );
	IplImage* v_plane = cvCreateImage( cvGetSize(tagImage), 8, 1 );
	IplImage* planes[] = { h_plane, s_plane, v_plane };//
	//** H ��������Ϊ16���ȼ���S��������Ϊ8���ȼ� */  
	int h_bins =32,s_bins =4;  //,v_bins = 4
	int hist_size[] = { h_bins,s_bins  };// ,v_bins 


	//** H �����ı仯��Χ */  
	float h_ranges[] = { 0, 180 };   
	//** S �����ı仯��Χ*/  
	float s_ranges[] = { 0, 255 };  
	float* ranges[] = { h_ranges, s_ranges };// ,v_ranges
	//** ����ͼ��ת����HSV��ɫ�ռ� */  
	cvCvtColor( tagImage, hsv, CV_BGR2HSV );  
	cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 ); //
	//** ����ֱ��ͼ����ά, ÿ��ά���Ͼ��� */  
	CvHistogram * hist = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1 ); //2 
	//** ����H,S����ƽ������ͳ��ֱ��ͼ */  
	cvCalcHist( planes, hist, 0, 0 );  
	//��һ������
	cvNormalizeHist(hist,1.0);

	//** ��ȡֱ��ͼͳ�Ƶ����ֵ�����ڶ�̬��ʾֱ��ͼ */  
	float max_value;
	cvGetMinMaxHistValue( hist,0, &max_value, 0, 0 );
	//** ����ֱ��ͼ��ʾͼ�� */  
	int height = 240;//3000
	int width = (h_bins*s_bins*6);//h_bins*6   
	IplImage* hist_img = cvCreateImage( cvSize(width,height), 8, 3 );
	cvZero( hist_img );
	IplImage* histImg = cvCreateImage( cvSize(width,height), 8, 3 );
	cvZero( histImg );
	//** ��������HSV��RGB��ɫת������ʱ��λͼ�� */  
	IplImage * hsv_color = cvCreateImage(cvSize(1,1),8,3);
	IplImage * rgb_color = cvCreateImage(cvSize(1,1),8,3);
	int bin_w = width/(h_bins*s_bins );//*v_bins 
	if(max_value<0.000001)
	{
		return;
	}

	memset(tColor.nH,0,sizeof(struct Hist));

	int i=0,j=0;
	bool IsFirst = true;
	memset(histinfo,0,sizeof(struct Hist)*128);
	for(int h = 0; h < h_bins; h++)
	{
		for(int s = 0; s < s_bins; s++)
		{
			    int i = h*s_bins+s;
				/** ���ֱ��ͼ�е�ͳ�ƴ�����������ʾ��ͼ���еĸ߶� */ 
				float bin_val = cvQueryHistValue_2D( hist, h, s );
				int intensity = cvRound(bin_val*height/max_value);

				/** ��õ�ǰֱ��ͼ�������ɫ��ת����RGB���ڻ��� */  
				cvSet2D(hsv_color,0,0,cvScalar(h*180.f / h_bins,s*255.f/s_bins,255,0));//s*255.f/s_bins        v*255.f/v_bins
				cvCvtColor(hsv_color,rgb_color,CV_HSV2BGR);
				CvScalar color = cvGet2D(rgb_color,0,0);
				cvRectangle( hist_img, cvPoint(i*bin_w,height),
					cvPoint((i+1)*bin_w,height - intensity),
					color, -1, 8, 0 );
				////��ȡ��������ɫ��Ϣ��ֱ����
				//if ((color.val[0]!=color.val[1]
				//||color.val[1]!=color.val[2]
				//||color.val[2]!=color.val[0])
				//	&&intensity!=0)
				histinfo[i].hsv.val[0]= h*180.f / h_bins;
				histinfo[i].hsv.val[1] = s*255.f/s_bins;
				histinfo[i].bgr = color;
				histinfo[i].value = intensity;				
		}

	}
	histGroup histgroup[32];
	memset (histgroup,0,sizeof(struct Group)*32);
	for (int i =0;i<31;i++)
	{
		histgroup[i+1].h=histgroup[i].h+180.f/h_bins;
	}

	for (int j=0;j<128;j++)
	{
		for (int i =0;i<32;i++)
		{
			if (histinfo[j].hsv.val[0]==histgroup[i].h)
			{
				histgroup[i].value=histinfo[j].value+histgroup[i].value;
			}
		}
	}
	//������
	int max,tmp;
	float temp;
	for(i=0;i<32;i++)
	{
		max =i;
		for (j=i+1;j<32;j++)
		{
			if(histgroup[max].value<histgroup[j].value)
				max=j;
		}
		if(i!=max)
		{
			tmp =histgroup[max].value;
			histgroup[max].value=histgroup[i].value;
			histgroup[i].value= tmp;
			temp =histgroup[max].h;
			histgroup[max].h=histgroup[i].h;
			histgroup[i].h= temp;
		}
	}
	//��ȡ�ۼӺ���������

	tColor.nH[0].hsv.val[0] = histgroup[0].h;
	tColor.nH[1].hsv.val[0] = histgroup[1].h;
	tColor.nH[2].hsv.val[0] = histgroup[2].h;
	tColor.nH[3].hsv.val[0] = histgroup[3].h;
	tColor.nH[4].hsv.val[0] = histgroup[4].h;
	tColor.nH[5].hsv.val[0] = histgroup[5].h;

	cvNamedWindow("hist",1);
	cvMoveWindow("hist",420,120);
	cvShowImage("hist",hist_img);
	
	cvReleaseImage(&histImg);
	cvReleaseImage(&hsv);
	cvReleaseImage(&h_plane);
	cvReleaseImage(&s_plane);
	cvReleaseImage(&v_plane);
	cvReleaseImage(&hsv_color);
	cvReleaseImage(&rgb_color);
}

void CSeasbaseVideoDlg::OnStnClickedImage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	iIsTwice++;
	GetCursorPos(&position);
	// (�����ڴ���Դ����������ڲ�, �������ⲿ, ����ͨ�����CMainFrame��ָ��, ��ͨ��pMianFrame->GetDlgItem(IDCk_MY_PIC)����ȡ)
	//pWnd->ScreenToClient(&position);
	//ScreenToClient(&position);
	position.x =position.x -windowRect.left;//6
	position.y =position.y -windowRect.top;//45
	CClientDC ClientDC(this); 
	CBrush brush,*oldbrush; 
	brush.CreateSolidBrush(RGB(0,0,0));    //��ɫ��
	oldbrush=ClientDC.SelectObject(&brush); 
	ClientDC.Ellipse(position.x-2,position.y-2,position.x+2,position.y+2);
	ClientDC.SelectObject(oldbrush);

	//ͨ���Խ����ϵ������㻭���ο�
	if (iIsTwice>0
		&&(iIsTwice%2==0))
	{
		bSelRect=false;
		CDC *pDc; pDc = GetDC(); 
		CBrush cbr; 
		cbr.CreateStockObject(NULL_BRUSH);  //����һ���ջ�ˢ 
		CBrush *pOldBr = pDc->SelectObject(&cbr); 
		CPen cpen; 
		cpen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); //����һ����ɫ�Ļ���
		CPen *pOldPen = pDc->SelectObject(&cpen);
		pDc->Rectangle(position.x,position.y,pos.x,pos.y);
		CPoint post;
		post.x=position.x>pos.x?pos.x:position.x;
		post.y=position.y>pos.y?pos.y:position.y;
		roi_rect_src.width=abs(position.x-pos.x);
		roi_rect_src.height=abs(position.y-pos.y);
		CWnd *pWnd = GetDlgItem(IDC_IMAGE);
		pWnd->ScreenToClient(&post);
		roi_rect_src.x= post.x+windowRect.left;//6
		roi_rect_src.y= post.y+windowRect.top;//45
		
		cvSetImageROI(pImg,roi_rect_src);
		tagImg = cvCreateImage(cvSize(roi_rect_src.width,roi_rect_src.height),IPL_DEPTH_8U,3);
		cvCopyImage(pImg,tagImg);
		meas.box= cvRect(roi_rect_src.x,roi_rect_src.y,roi_rect_src.width,roi_rect_src.height);
		HistogramProcess(tagImg);
		CString sInfo="";
		//histinfo[i].hsv.val[0]
		for (int i = 0;i<128;i++)
		{
			if (histinfo[i].value!=0)
			{
				CString subInfo ;
				subInfo.Format("{\\\"hv\\\":%.3f,\\\"sv\\\":%.3f,\\\"hi\\\":%d,\\\"RGB\\\":\\\"%d,%d,%d\\\"}",histinfo[i].hsv.val[0],histinfo[i].hsv.val[1],histinfo[i].value,(int)histinfo[i].bgr.val[2],(int)histinfo[i].bgr.val[1],(int)histinfo[i].bgr.val[0]);
				if (sInfo.IsEmpty())
				{
					sInfo=subInfo;
				}
				else
					sInfo=sInfo+","+subInfo;
			}
		}
		//sInfo.Format("YH:%f\nEH:%f\nSH:%f\nFH:%f\nWH:%f\nLH:%f\n",tColor.nH[0].hsv.val[0],tColor.nH[1].hsv.val[0],tColor.nH[2].hsv.val[0],tColor.nH[3].hsv.val[0],tColor.nH[4].hsv.val[0],tColor.nH[5].hsv.val[0]);
		sInfo="["+sInfo+"]";
		this->SetDlgItemTextA(IDC_EDIT2,sInfo);
		
		CString colorValue;
		CString  tText;
		tText =sText;
		int idex;
		if ( tText.Find("where")!=-1)
		{
			int iw = tText.Find("where");
			idex = tText.Find(" limit");
			tText.Delete(iw,idex-iw);
		}
		idex = tText.Find("limit");
		//colorValue.Format("where yh=%.3f and eh=%.3f and  sh=%.3f or fh=%.3f or wh=%.3f or  lh=%.3f ",tColor.nH[0].hsv.val[0],tColor.nH[1].hsv.val[0],tColor.nH[2].hsv.val[0],tColor.nH[3].hsv.val[0],tColor.nH[4].hsv.val[0],tColor.nH[5].hsv.val[0]);
		colorValue.Format("where yh=%.3f and eh=%.3f and  sh=%.3f and cnt>9 ",tColor.nH[0].hsv.val[0],tColor.nH[1].hsv.val[0],tColor.nH[2].hsv.val[0]);
		tText.Insert(idex,colorValue);
		SetDlgItemTextA(IDC_ETEXT,tText); //��ӵ��༭����ʾ
		bSelRect =true;
		pDc->SelectObject(pOldPen); 
		cbr.DeleteObject(); 
		pDc->SelectObject(pOldBr); 
		cbr.DeleteObject();
		cvResetImageROI(pImg);
	}
	else
	{
		pos.x = position.x;
		pos.y = position.y;
	}

}

void CSeasbaseVideoDlg::OnLoadLoadvideo()
{
	// TODO: �ڴ���������������
	CDlgVideoLoad video =new CDlgVideoLoad();
	int ibt =video.DoModal();
}

void CSeasbaseVideoDlg::OnLoadLoadtables()
{
	// TODO: �ڴ���������������
	m_treeview.DeleteAllItems();
	hRoot =m_treeview.InsertItem("Default",0,0);
	m_treeview.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	//-----------------------------��ʼItem�ڵ�ֵ-----------------------------------//
	for(int i=0 ;i<maxItemCount; i++)
	{
		ItemList[i].ItemFlag	= 0;				//0������ڵ�,����
		ItemList[i].ItemName	= "";
		ItemList[i].Value		= "";
		ItemList[i].ItemParent	= 0;
	}
	m_treeview.Expand(hRoot,TVE_EXPAND);

	SeasbaseRunExec(_T("cmd"),_T("/c hive -e \"show tables\" >tables.txt"), INFINITE,true);
	fstream fin("tables.txt");  //���ļ�    
	string ReadLine; 
	int NodeIndex=1;
	while(getline(fin,ReadLine))  //���ж�ȡ��ֱ������ 
	{
		ItemList[NodeIndex].ItemFlag		= 1;				//1����1���ڵ�
		ItemList[NodeIndex].ItemName		= ReadLine.c_str();
		ItemList[NodeIndex++].ItemParent		= 0;				//���ڵ�Ϊ����ĵ�0��	
	}
	HTREEITEM  newItem;
	int itemCount	= GetItemCount(ItemList);				//���ͽڵ���
	int addedItemC	= 0;									//ͳ���Ѿ������˶��ٽڵ�,������������еĽڵ���,������
	for(int i=1; i < itemCount+1; i++)
	{
		if(ItemList[i].ItemParent == 0 )					//�ж��Ƿ�Ϊ���ڵ���ӽڵ�,�������
		{			
			//���1���ӽڵ�
			newItem		= m_treeview.InsertItem(ItemList[i].ItemName ,1 , 1, hRoot);
			m_treeview.EnsureVisible(newItem);	
			addedItemC++;
		}
		if(addedItemC == itemCount)break;

	}
}

void CSeasbaseVideoDlg::OnLoadLoadimage()
{
	// TODO: �ڴ���������������
	CFileDialog dlg(TRUE, 
		"Image Files (*bmp)", 
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_ENABLESIZING,
		_T("Image Files (*.jpg;*.bmp;*.png;*.icon;*.gif)|*.jpg;*.bmp;*.png;*.icon;*.gif||"),
		NULL);
	dlg.m_ofn.lpstrTitle=_T("Please select an image");

	dlg.m_ofn.nMaxFile = MAX_PATH; 
	dlg.m_ofn.lpstrFile = new TCHAR[dlg.m_ofn.nMaxFile];  
	ZeroMemory(dlg.m_ofn.lpstrFile, sizeof(TCHAR) * dlg.m_ofn.nMaxFile); 
	int retval = dlg.DoModal();
	if(retval==IDCANCEL)
		return;
	POSITION pos_file;
	pos_file = dlg.GetStartPosition();
	CArray<CString, CString> ary_filename;
	while(pos_file != NULL)
		ary_filename.Add(dlg.GetNextPathName(pos_file));
	this->SetDlgItemTextA(IDC_ImagePath,ary_filename.GetAt(0));
	//sInfo= ary_filename.GetAt(0);
	CImage myImage;
	myImage.Load(ary_filename.GetAt(0));
	IplImage* copyImg = cvCreateImage(cvSize(myImage.GetWidth(),myImage.GetHeight()),IPL_DEPTH_8U,3);

	copyImg = cvLoadImage(ary_filename.GetAt(0),1);

	//ResizeImage( myImage );    // �Զ����ͼƬ��������

	GetClientRect(&windowRect);
	ClientToScreen(&windowRect);
	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_IMAGE); // (�����ڴ���Դ����������ڲ�, �������ⲿ, ����ͨ�����CMainFrame��ָ��, ��ͨ��pMianFrame->GetDlgItem(IDCk_MY_PIC)����ȡ)
	CDC *pDC = pWnd->GetDC();
	pWnd->GetClientRect(&rect);
	pDC->SetStretchBltMode(STRETCH_HALFTONE);
	myImage.Draw(pDC->m_hDC, rect);


	// ���ͼƬ�ؼ��Ŀ�͸� 
	int rw = rect.right - rect.left;  
	int rh = rect.bottom - rect.top; 
	pImg = cvCreateImage(cvSize(rw,rh),IPL_DEPTH_8U,3);
	cvResize(copyImg,pImg,CV_INTER_LINEAR);
	pWnd->ClientToScreen(&rect);
	//�����ؼ��ڴ����е�λ�ã����ԸĶ���ֵ��ʹ�Ӵ���Ĵ�С���ʣ�
	rect.top-=windowRect.top;
	rect.bottom-=windowRect.top;//45
	rect.left-=windowRect.left;
	rect.right-=windowRect.left;//6
	CDC *pDc; pDc = GetDC(); 
	CBrush cbr; 
	cbr.CreateStockObject(NULL_BRUSH);  //����һ���ջ�ˢ 
	CBrush *pOldBr = pDc->SelectObject(&cbr); 
	CPen cpen; 
	cpen.CreatePen(PS_SOLID, 1, RGB( 0, 0, 255)); //����һ����ɫ�Ļ���
	CPen *pOldPen = pDc->SelectObject(&cpen);
	//pDc->Rectangle(windowRect.left,windowRect.top,windowRect.right,windowRect.bottom);
	pDc->Rectangle(rect.left,rect.top,rect.right,rect.bottom);
	//myImage.Draw(pDC->m_hDC, rect);
	ReleaseDC(pDC);
	myImage.Destroy();
	cvReleaseImage(&copyImg);
}

//����һ������ֱ�������н���
BOOL CSeasbaseVideoDlg::HiveRunExec(const TCHAR *cmd, const TCHAR *para, DWORD dwMilliseconds)
{    //  DWORD Sk=GetTickCount();
	   
       SHELLEXECUTEINFO ShExecInfo = {0};
       ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
       ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
       ShExecInfo.hwnd = NULL;
       ShExecInfo.lpVerb = NULL;
       ShExecInfo.lpFile = cmd;
       ShExecInfo.lpParameters = para;
       ShExecInfo.lpDirectory = NULL;
       ShExecInfo.nShow =SW_SHOWMAXIMIZED; // SW_HIDE
       ShExecInfo.hInstApp = NULL;
       BOOL suc = ShellExecuteEx(&ShExecInfo);
       //WaitForSingleObject(ShExecInfo.hProcess, dwMilliseconds);
	  /* DWORD Ek=GetTickCount();
	   printf("################################k:%dms\n",Ek-Sk);*/
       return suc;
}
void CSeasbaseVideoDlg::OnHiveCmd()
{
	// TODO: �ڴ���������������
	HiveRunExec(_T("cmd"),_T("/c hive.cmd"), INFINITE);
}

void CSeasbaseVideoDlg::OnHelpHelp()
{
	// TODO: �ڴ���������������
	::ShellExecuteA(NULL,"open","http://www.redhadoop.com/studio/",NULL,NULL,SW_SHOW);
}

void CSeasbaseVideoDlg::OnBnClickedBrowerdata()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString path ="E:\\RedHadoop Project\\RedHadoop Workstation\\RedHadoopWorkstation0.1\\RedHadoopWorkstation0.3\\CmdGUI";//.//CmdGui
	::ShellExecuteA(NULL,"open","data.txt",NULL,path,SW_SHOW);
	
}

void CSeasbaseVideoDlg::OnLoadExecute()
{
	// TODO: �ڴ���������������
	 CString EditText;
	 GetDlgItemTextA(IDC_ETEXT,EditText);
	 vector<string> vStr;
	 CString table;
	 CString message;
	 char buff[204800];

	 m_listviewinfo.m_listinfo.DeleteAllItems();
	 for(int c = 0; c<ColIndex;c++)
		 m_listviewinfo.m_listinfo.DeleteColumn(0);
	 m_listimages.m_image.DeleteAllItems();

	 img.DeleteImageList();

	 CString str = EditText;
	 string editText = EditText.MakeLower();
	 frameID =0;
	 tagID=0;
	 ColIndex =0;

	 sprintf(buff,"/c hive -e \"explain %s\" >type.txt ",editText.c_str());
	 SeasbaseRunExec(_T("cmd"),buff, INFINITE,true);
	 //InsertColumn
	 fstream fin("type.txt");  //���ļ�
	 string ReadLine; 
	 while(getline(fin,ReadLine))  //���ж�ȡ��ֱ������    
	 {     
		 if (std::string::npos!=ReadLine.find("expressions"))
		 {
			 CString strLine=ReadLine.c_str();
			 int exinx = strLine.Find("expressions");
			 strLine.Delete(0,exinx+12);
			 do 
			 {
				 int linx = strLine.Find("(");
				 int rinx = strLine.Find(")");
				 if (linx!=-1
					 &&rinx!=-1)
				 {
					 if(linx<rinx)
						 strLine.Delete(linx,rinx-linx+1);
					 else
						 strLine.Delete(rinx,rinx);
				 }

			 } while (strLine.Find(")")!=-1);
			 string editText =strLine.MakeLower();
			 vector<string> vStr;
			 split(editText,",",vStr);
			 for(int i =0; i<vStr.size();i++)
			 {
				 if(vStr[i].size()>1)
				 {
					 CString colname=vStr[i].c_str();
					 if (strcmp(colname.MakeLower()," frameid ")==0)
					 {
						 frameID =ColIndex;
					 }
					 if (strcmp(colname.MakeLower()," targetnum ")==0)
					 {
						 tagID =ColIndex;
					 }
					 m_listviewinfo.m_listinfo.InsertColumn(ColIndex,colname,LVCFMT_LEFT,100,ColIndex++);//������ 
				 }
			 }
			 break;
		 }
	 }

	 int len =0;
	 int fno=0;
	 BYTE  output[2048*120];
	 CString path="";
	 int iSubTypeIndex = 0;

	 sprintf(buff,"/c hive -e \"%s\" 1>data.txt",str); //2>err.txt
	 SeasbaseRunExec(_T("cmd"),buff, INFINITE,true);
	 path = "data.txt"; 

	 fstream fsin(path);  //���ļ�   
	 while(getline(fsin,ReadLine))  //���ж�ȡ��ֱ������    
	 {      
		 vector<string> d;
		 vector<string>::iterator p;

		 split(ReadLine,"\t\n",d);
		 bool bFirstSubItem = true;
		 int SubItemIndex=1;
		 int nIndex = m_listviewinfo.m_listinfo.GetItemCount();
		 for(p=d.begin();p!=d.end();++p)
		 {
			 if (p->length()>10000)
			 {
				 len = BASE64_Decode(p->c_str(),p->length(),output);
				 SaveData((char *)output,len,&fno);
				 fno++;
			 }
			 else
			 {
				 //�²�������������� 
				 LV_ITEM lvItem;
				 if(bFirstSubItem)
				 {
					 lvItem.mask = LVIF_TEXT ; 
					 lvItem.iItem = nIndex;     //����
					 lvItem.iSubItem = 0;
					 lvItem.pszText = (LPSTR)(&(*p->c_str()));  //��һ��
					 //�����һ�в����¼ֵm_listviewinfo.m_listinfo
					 m_listviewinfo.m_listinfo.InsertItem(&lvItem);
					 bFirstSubItem = false;

				 }
				 else
					 //����������
				 {
					 const char* s = &(*p->c_str());
					 CString ss(s);
					 UTF8toANSI(ss);
					 m_listviewinfo.m_listinfo.SetItemText(nIndex,SubItemIndex++,ss);//(LPCTSTR)(&(*p->c_str()))

				 }
			 }

		 }
	 }
	 fsin.close();

	 m_listimages.m_image.SetIconSpacing(180,155);//140 130
	 //*����ͼ���б�imagesѡ��*/
	 img.Create(160,130,ILC_COLORDDB|ILC_COLOR32,1,1);//120 90
	 m_listimages.m_image.SetImageList(&img,LVSIL_NORMAL); //LVSIL_SMAL

	 m_listimages.m_image.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//LVS_EX_SUBITEMIMAGES report���
	 int nIndex = m_listviewinfo.m_listinfo.GetItemCount();
	 for (int i=0;i<nIndex;i++)
	 {
		 CHAR OutPath[MAX_PATH];  
		 CString des="";
		 ::GetTempPath(256,des.GetBuffer(256));
		 des.ReleaseBuffer();
		 des+="\\";
		 memcpy(OutPath,des,des.GetLength()+1); 
		 CString strFrameID = m_listviewinfo.m_listinfo.GetItemText(i,frameID);
		 CString strTagID = m_listviewinfo.m_listinfo.GetItemText(i,tagID);
		 CString bitmapname;
		 bitmapname.Format("%sframe%s-%s.bmp",OutPath,strFrameID,strTagID);
		 //bitmapname.Format("%sframe%s-%s.bmp","E:\\bmp\\",strFrameID,strTagID);	
		 USES_CONVERSION;
		 WCHAR* pBuf = T2W((LPCTSTR)bitmapname); 
		 Bitmap bmp(pBuf);

		 int sourceWidth = 160;                              //���ͼƬ���,���120�ʹ�����120������ͬ��
		 int sourceHeight = bmp.GetHeight();                 //���ͼƬ���  

		 if(sourceHeight>160)             //��m_ImageList.Create(120��)�кܴ��ϵ�����С����
			 //���Ŀ�ȣ�ͼƬ���޷���ʾ��
		 {
			 sourceHeight=160;
		 }

		 Bitmap* pThumbnail = (Bitmap*)bmp.GetThumbnailImage(sourceWidth , sourceHeight , NULL, NULL); //�趨����ͼ�Ĵ�С
		 HBITMAP hBmp;
		 pThumbnail->GetHBITMAP(Color(255,255,255),&hBmp);
		 CBitmap *pImage = CBitmap::FromHandle(hBmp);         //ת����CBitmap��ʽλͼ
		 int a=img.Add(pImage,RGB(255,255, 255));

		 bitmapname.Format("frame%s-%s.bmp",strFrameID,strTagID);
		 m_listimages.m_image.InsertItem(i ,bitmapname,i);
	 }
	 framenum=0;
}
