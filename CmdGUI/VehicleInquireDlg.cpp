// VehicleInquireDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VehicleInquireDlg.h"
#include "afxdialogex.h"
#include "ItemType.h"
#include <fstream>
#include <iostream>
#include "DlgVehVideoLoad.h"
#include <string>
#include <vector>
#include "BASE64_API.h"
#include <gdiplus.h>
using namespace std;
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

int			maxItemCountV		= 100;
extern   CItem*ItemListV		= new CItem[maxItemCountV]; 
//自定义函数
int		GetItemCountV(CItem	* pItem);
bool	HaveSubNode	(CItem	* pItem,	int pos); 
void	DeleteItem	(CItem	* pItem,	int pos);
// CVehicleInquireDlg 对话框

IMPLEMENT_DYNAMIC(CVehicleInquireDlg, CDialogEx)

CVehicleInquireDlg::CVehicleInquireDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVehicleInquireDlg::IDD, pParent)
{
	ColIndex = 0;
	nCnt = 0;
	framenum = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON);
	pImg =NULL;
	tagImg = NULL;
	memset(vehinfo,0,sizeof(VehicleInfo));
	
}

CVehicleInquireDlg::~CVehicleInquireDlg()
{
	if (pImg)
	{
		cvReleaseImage(&pImg);
	}
	if (tagImg)
	{
		cvReleaseImage(&tagImg);
	}
	//m_listimages.m_image.RemoveImageList(LVSIL_NORMAL);
	VehicleRecRelease vehicleRecRelease;
	vehicleRecRelease = (VehicleRecRelease)GetProcAddress(dlh, "ITS_VehicleRecRelease");
	(*vehicleRecRelease)(pInstance);
}

void CVehicleInquireDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_subtab);
	DDX_Control(pDX, IDC_TREEVIEW, m_treeview);
	//DDX_Control(pDX, IDC_TAB2, m_subtab2);
}


BEGIN_MESSAGE_MAP(CVehicleInquireDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BLOADVIDEO, &CVehicleInquireDlg::OnBnClickedBloadvideo)
	ON_BN_CLICKED(IDC_BLOAD, &CVehicleInquireDlg::OnBnClickedBload)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREEVIEW, &CVehicleInquireDlg::OnTvnSelchangedTreeview)
	ON_BN_CLICKED(IDC_BINQUIRE, &CVehicleInquireDlg::OnBnClickedBinquire)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CVehicleInquireDlg::OnTcnSelchangeTab1)
	ON_COMMAND(ID_LOAD_LOADVIDEO, &CVehicleInquireDlg::OnLoadvideos)
	ON_COMMAND(ID_LOAD_LOADTABLES, &CVehicleInquireDlg::OnLoadLoadtables)
	ON_COMMAND(ID_LOAD_LOADIMAGE, &CVehicleInquireDlg::OnLoadLoadimage)
	ON_COMMAND(ID_HELP_HELP, &CVehicleInquireDlg::OnHelp)
	ON_COMMAND(ID_HIVE_CMD, &CVehicleInquireDlg::OnHiveCmd)
	ON_BN_CLICKED(IDC_LoadImage, &CVehicleInquireDlg::OnBnClickedLoadimage)
	ON_BN_CLICKED(IDC_BVehicle, &CVehicleInquireDlg::OnBnClickedBvehicle)
END_MESSAGE_MAP()


// CVehicleInquireDlg 消息处理程序


BOOL CVehicleInquireDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//初始化GID+
	GdiplusStartupInput gdiplusStartupInput;       
	GdiplusStartup( &m_gdiplusToken,&gdiplusStartupInput,NULL );
	m_subtab.InsertItem(0,_T("Image"));
	m_subtab.InsertItem(1,_T("Result"));
	m_subtab.InsertItem(2,_T("Message"));
	//关联对话框,并且将IDC_TAB控件设为父窗口
	m_listviewinfo.Create(IDD_LISTVIEW,GetDlgItem(IDC_TAB1));
	m_listdescinfo.Create(IDD_DESCINFO,GetDlgItem(IDC_TAB1));
	m_listimages.Create(IDD_IMAGELIST,GetDlgItem(IDC_TAB1));
	//获得IDC_tabMain客户区大小
	CRect rs;
	m_subtab.GetClientRect(&rs);

	//调整子对话框在父窗口中的位置，可以改动数值，使子窗体的大小合适；
	rs.top+=20;
	rs.bottom-=3;
	rs.left+=2;
	rs.right-=2;

	//设置子对话框尺寸并移动到指定位置
	m_listviewinfo.MoveWindow(&rs);
	m_listdescinfo.MoveWindow(&rs);
	m_listimages.MoveWindow(&rs);

	//分别设置隐藏和显示
	m_listimages.ShowWindow(true);
	m_listviewinfo.ShowWindow(false);
	m_listdescinfo.ShowWindow(false);

	//设置默认的选项卡
	m_subtab.SetCurSel(0);

	//m_subtab2.InsertItem(0,_T("VehicleUI"));
	////关联对话框,并且将IDC_TAB控件设为父窗口
	//m_vehinquireUI.Create(IDD_VEHINQUIREUI,GetDlgItem(IDC_TAB2));
	////获得IDC_tabMain客户区大小
	//CRect rect;
	//m_subtab2.GetClientRect(&rect);
	////调整子对话框在父窗口中的位置，可以改动数值，使子窗体的大小合适；
	//rect.top+=20;
	//rect.bottom-=3;
	//rect.left+=2;
	//rect.right-=2;

	////设置子对话框尺寸并移动到指定位置
	//m_vehinquireUI.MoveWindow(&rect);
	////设置显示
	//m_vehinquireUI.ShowWindow(true);
	////设置默认的选项卡
	//m_subtab2.SetCurSel(0);

	hRoot =m_treeview.InsertItem("Default",0,0);
	m_treeview.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	//-----------------------------初始Item节点值-----------------------------------//

	for(int i=0 ;i<maxItemCountV; i++)
	{
		ItemListV[i].ItemFlag	= 0;				//0代表根节点,保留
		ItemListV[i].ItemName	= "";
		ItemListV[i].Value		= "";
		ItemListV[i].ItemParent	= 0;
	}
	m_treeview.Expand(hRoot,TVE_EXPAND);

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	dlh = NULL;
	if (!(dlh=LoadLibrary("VehicleTypeRecognition_D.dll")))      
	{
		printf("LoadLibrary() failed: %d\n", GetLastError()); 
	}
	// ITS_ThreadInit(); //进程启动时调用
	ThreadInit threadInit;
	threadInit = (ThreadInit)GetProcAddress(dlh, "ITS_ThreadInit");//ITS_ThreadInit(); 进程启动时调用
	(*threadInit)();


	int iInitFlag;
	char* modePath = "..\\model";//
	VehicleRecInit vehicleRecInit;
	vehicleRecInit = (VehicleRecInit)GetProcAddress(dlh, "ITS_VehicleRecInit");
	pInstance  = (*vehicleRecInit)(modePath, iInitFlag);//初始化操作
	//pInstance = ITS_VehicleRecInit(modePath, iInitFlag);//初始化操作
    
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
//-----------获取树型控件中项的数目------------------------//
int GetItemCountV(CItem * pItem)
{
	int		itemCount	= 0;
	for(int i=1; i<maxItemCountV; i++ )
	{
		if (pItem[i].ItemFlag != 0)
			itemCount++;
	}
	return itemCount;
}

void CVehicleInquireDlg::OnBnClickedBloadvideo()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgVehVideoLoad video =new CDlgVehVideoLoad();
	int ibt =video.DoModal();
}

BOOL VehicleRunExec(const TCHAR *cmd, const TCHAR *para, DWORD dwMilliseconds,bool wait=false)
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
/*分割字符串函数*/
void splitV(const string& src, const string& separator, vector<string>& dest)
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
	dest.push_back(substring);//在尾部加入一个数据
}
int ParserV(CString text,vector<string> &vText,CString &table,CString &message)
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

	splitV(tablename,"; ",vStr);
	if(vStr.size()>0)//返回容器中实际数据的个数。
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
	splitV(newText,", ",vStr);
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

				splitV(asText,": ",vAsStr);
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

int CVehicleInquireDlg::SaveData(char *buff,int len,int* fnoo)
{
	FILE *fp;
	CString fname;
	int fno =*fnoo;
	CHAR OutPath[MAX_PATH];  
	CString des="";
	char tmp[4096];
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
	fp=fopen(fname,"wb");
	if(fp==NULL)
	{return 0;}
	fwrite(buff,len,1,fp);
	fclose(fp);
	return 1;
}

void CVehicleInquireDlg::UTF8toANSI(CString &strUTF8)  
{  
	//获取转换为多字节后需要的缓冲区大小，创建多字节缓冲区  
	UINT nLen = MultiByteToWideChar(CP_UTF8,NULL,strUTF8,-1,NULL,NULL);  
	WCHAR *wszBuffer = new WCHAR[nLen+1];  
	nLen = MultiByteToWideChar(CP_UTF8,NULL,strUTF8,-1,wszBuffer,nLen);  
	wszBuffer[nLen] = 0;  

	nLen = WideCharToMultiByte(936,NULL,wszBuffer,-1,NULL,NULL,NULL,NULL);  
	CHAR *szBuffer = new CHAR[nLen+1];  
	nLen = WideCharToMultiByte(936,NULL,wszBuffer,-1,szBuffer,nLen,NULL,NULL);  
	szBuffer[nLen] = 0;  

	strUTF8 = szBuffer;  
	//清理内存  
	delete []szBuffer;  
	delete []wszBuffer;  
}  
void CVehicleInquireDlg::OnBnClickedBload()
{
	// TODO: 在此添加控件通知处理程序代码
	m_treeview.DeleteAllItems();
	hRoot =m_treeview.InsertItem("Default",0,0);
	m_treeview.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	//-----------------------------初始Item节点值-----------------------------------//
	for(int i=0 ;i<maxItemCountV; i++)
	{
		ItemListV[i].ItemFlag	= 0;				//0代表根节点,保留
		ItemListV[i].ItemName	= "";
		ItemListV[i].Value		= "";
		ItemListV[i].ItemParent	= 0;
	}
	m_treeview.Expand(hRoot,TVE_EXPAND);

	VehicleRunExec(_T("cmd"),_T("/c hive -e \"show tables\" >tables.txt"), INFINITE,true);
	fstream fin("tables.txt");  //打开文件    
	string ReadLine; 
	int NodeIndex=1;
	while(getline(fin,ReadLine))  //逐行读取，直到结束 
	{
		ItemListV[NodeIndex].ItemFlag		= 1;				//1代表1级节点
		ItemListV[NodeIndex].ItemName		= ReadLine.c_str();
		ItemListV[NodeIndex++].ItemParent		= 0;				//父节点为数组的第0项	
	}
	HTREEITEM  newItem;
	int itemCount	= GetItemCountV(ItemListV);				//树型节点数
	int addedItemC	= 0;									//统计已经加入了多少节点,如果等于链表中的节点数,则跳出
	for(int i=1; i < itemCount+1; i++)
	{
		if(ItemListV[i].ItemParent == 0 )					//判断是否为根节点的子节点,是则加入
		{			
			//添加1级子节点
			newItem		= m_treeview.InsertItem(ItemListV[i].ItemName ,1 , 1, hRoot);
			m_treeview.EnsureVisible(newItem);	
			addedItemC++;
		}
		if(addedItemC == itemCount)break;

	}
}


void CVehicleInquireDlg::OnTvnSelchangedTreeview(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	
	CString strText; // 树节点的标签文本字符串    
	// 获取当前选中节点的句柄    
	HTREEITEM hItem = m_treeview.GetSelectedItem();    
	// 获取选中节点的标签文本字符串    
	strText = m_treeview.GetItemText(hItem); 
	 CString eText;
	if (strText.Find("Default")!=-1)
	{
		eText ="select frameid,targetnum,number,color,type,frame from "+strText+" limit 100";
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
	// 将字符串显示到编辑框中    
	SetDlgItemTextA(IDC_ETEXT,eText);//添加到DlgInquireText界面的编辑框显示
	sText = eText;
	*pResult = 0;
}


void CVehicleInquireDlg::OnBnClickedBinquire()
{
	// TODO: 在此添加控件通知处理程序代码
	m_listimages.sPath =strVideoPath;
	CString EditText;
	GetDlgItemTextA(IDC_ETEXT,EditText);
	vector<string> vStr;
	CString table;
	CString message;
	char buff[1024];

	m_listviewinfo.m_listinfo.DeleteAllItems();
	for(int c = 0; c<ColIndex;c++)
		m_listviewinfo.m_listinfo.DeleteColumn(0);
	m_listimages.m_image.DeleteAllItems();

	img.DeleteImageList();

	CString str = EditText;
	string editText = EditText.MakeLower();
	frameID =0;
	tagID=0;
	numID=0;
	colorID =0;
	typeID =0;
	ColIndex =0;
	if( std::string::npos!=editText.find("insert")
		||(std::string::npos!=editText.find("create")
		&&std::string::npos==editText.find("select"))
		||std::string::npos!=editText.find("show")
		||std::string::npos!=editText.find("desc"))
	{
		//sprintf(buff,"/c hive -e \"%s\" ",editText);
		sprintf(buff,"/c hive -e \"%s\"  ",str);
		VehicleRunExec(_T("cmd"),buff, INFINITE,true);
		message = "error!";
		m_listdescinfo.m_listmessage.InsertString(nCnt++,message);
		m_subtab.SetCurFocus(2);
		int nCount = m_listdescinfo.m_listmessage.GetCount();
		if (nCount > 0)
			m_listdescinfo.m_listmessage.SetCurSel(nCount - 1); 
		m_listdescinfo.m_listmessage.SetTopIndex(nCount - 1);
		return;
	}
	int ret = ParserV( EditText,vStr,table,message);
	if(ret == -1)
	{
		//把message显示到message tab page 中
		m_listdescinfo.m_listmessage.InsertString(nCnt++,message);
		m_subtab.SetCurFocus(2);
		int nCount = m_listdescinfo.m_listmessage.GetCount();
		if (nCount > 0)
			m_listdescinfo.m_listmessage.SetCurSel(nCount - 1); 
		m_listdescinfo.m_listmessage.SetTopIndex(nCount - 1);
		return;
	}
	if (vStr.size()==1
		&& std::string::npos!=vStr[0].find("*"))
	{
		sprintf(buff,"/c hive -e \"desc %s\" >type.txt",table);
		VehicleRunExec(_T("cmd"),buff, INFINITE,true);
		//InsertColumn
		fstream fin("type.txt");  //打开文件
		string ReadLine; 
		bool bFirstSubItem = true;
		while(getline(fin,ReadLine))  //逐行读取，直到结束    
		{      
			vector<string> d;

			splitV(ReadLine," \t\n",d);

			int SubItemIndex=1;
			int nIndex = m_listviewinfo.m_listinfo.GetItemCount();

			CString colname=d[0].c_str();
			m_listviewinfo.m_listinfo.InsertColumn(ColIndex,colname,LVCFMT_LEFT,100,ColIndex++);//设置列 

		}
	}else 
	{
		for(int i = 0; i< vStr.size();i++)
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
			if (strcmp(colname.MakeLower(),"number")==0)
			{
				numID =ColIndex;
			}
			if (strcmp(colname.MakeLower(),"color")==0)
			{
				colorID =ColIndex;
			}
			if (strcmp(colname.MakeLower(),"type")==0)
			{
				typeID =ColIndex;
			}
			m_listviewinfo.m_listinfo.InsertColumn(ColIndex,colname,LVCFMT_LEFT,100,ColIndex++);//设置列 

		}
	}
	string ReadLine;
	int len =0;
	int fno=0;
	BYTE  output[2048*120];
	CString path="";
	int iSubTypeIndex = 0;

	sprintf(buff,"/c hive -e \"%s\" 1>data.txt",str); //2>err.txt
	VehicleRunExec(_T("cmd"),buff, INFINITE,true);
	path = "data.txt"; 
	fstream fin(path);  //打开文件   
	while(getline(fin,ReadLine))  //逐行读取，直到结束    
	{      
		vector<string> d;
		vector<string>::iterator p;

		splitV(ReadLine,",\t\n",d);
		bool bFirstSubItem = true;
		int SubItemIndex=1;
		int nIndex = m_listviewinfo.m_listinfo.GetItemCount();
		for(p=d.begin();p!=d.end();++p)
		{
			if (p->length()>100)
			{
				len = BASE64_Decode(p->c_str(),p->length(),output);
				SaveData((char *)output,len,&fno);
				fno++;
			}
			else
			{
				//新插入的数据在下面 
				LV_ITEM lvItem;
				if(bFirstSubItem)
				{
					lvItem.mask = LVIF_TEXT ; 
					lvItem.iItem = nIndex;     //行数
					lvItem.iSubItem = 0;
					lvItem.pszText = (LPSTR)(&(*p->c_str()));  //第一列
					//在最后一行插入记录值
					m_listviewinfo.m_listinfo.InsertItem(&lvItem);
					bFirstSubItem = false;

				}
				else
					//插入其它列
				{
					const char* s = &(*p->c_str());
					CString ss(s);
					UTF8toANSI(ss);
					m_listviewinfo.m_listinfo.SetItemText(nIndex,SubItemIndex++,ss);//(LPCTSTR)(&(*p->c_str()))

				}
			}

		}
	}
	fin.close();
	
	m_listimages.m_image.SetIconSpacing(420,440);//140 130
	//*加载图像列表到images选卡*/
	img.Create(400,400,ILC_COLORDDB|ILC_COLOR32,1,1);//120 90
	m_listimages.m_image.SetImageList(&img,LVSIL_NORMAL); //LVSIL_SMAL
	m_listimages.m_image.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//LVS_EX_SUBITEMIMAGES report风格
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
		CString strNumID = m_listviewinfo.m_listinfo.GetItemText(i,numID);
		CString strColorID = m_listviewinfo.m_listinfo.GetItemText(i,colorID);
		CString strTypeID = m_listviewinfo.m_listinfo.GetItemText(i,typeID);
		CString bitmapname;
		bitmapname.Format("%sframe%s-%s.bmp",OutPath,strFrameID,strTagID);

		USES_CONVERSION;
		WCHAR* pBuf = T2W((LPCTSTR)bitmapname); 
		Bitmap bmp(pBuf);

		int sourceWidth = 400;                              //获得图片宽度,这个400和创建的400保持相同。
		int sourceHeight = bmp.GetHeight();                 //获得图片宽度  

		if(sourceHeight>400)             //和m_ImageList.Create(120…)有很大关系，如果小于设定的宽度，图片将无法显示。	
		{
			sourceHeight=400;
		}

		Bitmap* pThumbnail = (Bitmap*)bmp.GetThumbnailImage(sourceWidth , sourceHeight , NULL, NULL); //设定缩略图的大小
		HBITMAP hBmp;
		pThumbnail->GetHBITMAP(Color(255,255,255),&hBmp);
		CBitmap *pImage = CBitmap::FromHandle(hBmp);         //转换成CBitmap格式位图
		int a=img.Add(pImage,RGB(255,255, 255));

		bitmapname.Format("frame%s %s %s %s %s",strFrameID,strTagID,strNumID,strColorID,strTypeID);
		m_listimages.m_image.InsertItem(i ,bitmapname,i);
	}
	framenum=0;
	//m_listimages.m_image.RemoveImageList(LVSIL_NORMAL);
}


void CVehicleInquireDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
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


void CVehicleInquireDlg::OnLoadvideos()
{
	// TODO: 在此添加命令处理程序代码
	CDlgVehVideoLoad video =new CDlgVehVideoLoad();
	int ibt =video.DoModal();
}


void CVehicleInquireDlg::OnLoadLoadtables()
{
	// TODO: 在此添加命令处理程序代码
	m_treeview.DeleteAllItems();
	hRoot =m_treeview.InsertItem("Default",0,0);
	m_treeview.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	//-----------------------------初始Item节点值-----------------------------------//
	for(int i=0 ;i<maxItemCountV; i++)
	{
		ItemListV[i].ItemFlag	= 0;				//0代表根节点,保留
		ItemListV[i].ItemName	= "";
		ItemListV[i].Value		= "";
		ItemListV[i].ItemParent	= 0;
	}
	m_treeview.Expand(hRoot,TVE_EXPAND);

	VehicleRunExec(_T("cmd"),_T("/c hive -e \"show tables\" >tables.txt"), INFINITE,true);
	fstream fin("tables.txt");  //打开文件    
	string ReadLine; 
	int NodeIndex=1;
	while(getline(fin,ReadLine))  //逐行读取，直到结束 
	{
		ItemListV[NodeIndex].ItemFlag		= 1;				//1代表1级节点
		ItemListV[NodeIndex].ItemName		= ReadLine.c_str();
		ItemListV[NodeIndex++].ItemParent		= 0;				//父节点为数组的第0项	
	}
	HTREEITEM  newItem;
	int itemCount	= GetItemCountV(ItemListV);				//树型节点数
	int addedItemC	= 0;									//统计已经加入了多少节点,如果等于链表中的节点数,则跳出
	for(int i=1; i < itemCount+1; i++)
	{
		if(ItemListV[i].ItemParent == 0 )					//判断是否为根节点的子节点,是则加入
		{			
			//添加1级子节点
			newItem		= m_treeview.InsertItem(ItemListV[i].ItemName ,1 , 1, hRoot);
			m_treeview.EnsureVisible(newItem);	
			addedItemC++;
		}
		if(addedItemC == itemCount)break;

	}
}


void CVehicleInquireDlg::OnLoadLoadimage()
{
	// TODO: 在此添加命令处理程序代码
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

	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_IMAGE); // (这是在此资源创建的类的内部, 若是在外部, 可先通过获得CMainFrame的指针, 再通过pMianFrame->GetDlgItem(IDCk_MY_PIC)来获取)
	CDC *pDC = pWnd->GetDC();
	pWnd->GetClientRect(&rect);
	pDC->SetStretchBltMode(STRETCH_HALFTONE);
	myImage.Draw(pDC->m_hDC, rect);

	//调整控件在窗口中的位置，可以改动数值，使子窗体的大小合适；
	rect.top+=7;
	rect.bottom+=7;
	rect.left+=25;
	rect.right+=25;
	// 求出图片控件的宽和高 
	int rw = rect.right - rect.left;  
	int rh = rect.bottom - rect.top; 
	pImg = cvCreateImage(cvSize(rw,rh),IPL_DEPTH_8U,3);
	cvResize(copyImg,pImg,CV_INTER_LINEAR);
	ReleaseDC(pDC);
	myImage.Destroy();
	cvReleaseImage(&copyImg);
}


void CVehicleInquireDlg::OnHelp()
{
	// TODO: 在此添加命令处理程序代码
	::ShellExecuteA(NULL,"open","http://www.redhadoop.com/studio/",NULL,NULL,SW_SHOW);
}



//启动一个程序，直到它运行结束
BOOL CVehicleInquireDlg::HiveRunExec(const TCHAR *cmd, const TCHAR *para, DWORD dwMilliseconds)
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
void CVehicleInquireDlg::OnHiveCmd()
{
	// TODO: 在此添加命令处理程序代码
	HiveRunExec(_T("cmd"),_T("/c hive.cmd"), INFINITE);
}
void CVehicleInquireDlg::OnBnClickedLoadimage()
{
	// TODO: 在此添加控件通知处理程序代码
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

	//ResizeImage( myImage );    // 对读入的图片进行缩放

	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_IMAGE); // (这是在此资源创建的类的内部, 若是在外部, 可先通过获得CMainFrame的指针, 再通过pMianFrame->GetDlgItem(IDCk_MY_PIC)来获取)
	CDC *pDC = pWnd->GetDC();
	pWnd->GetClientRect(&rect);
	pDC->SetStretchBltMode(STRETCH_HALFTONE);
	myImage.Draw(pDC->m_hDC, rect);
	tagImg = cvCreateImage(cvSize(myImage.GetWidth(),myImage.GetHeight()),IPL_DEPTH_8U,3);
	cvCopyImage(copyImg,tagImg);

	//调整控件在窗口中的位置，可以改动数值，使子窗体的大小合适；
	rect.top+=7;
	rect.bottom+=7;
	rect.left+=25;
	rect.right+=25;
	// 求出图片控件的宽和高 
	int rw = rect.right - rect.left;  
	int rh = rect.bottom - rect.top; 
	pImg = cvCreateImage(cvSize(rw,rh),IPL_DEPTH_8U,3);
	cvResize(copyImg,pImg,CV_INTER_LINEAR);
	ReleaseDC(pDC);
	myImage.Destroy();
	cvReleaseImage(&copyImg);
}
void CVehicleInquireDlg::OnBnClickedBvehicle()
{
	// TODO: 在此添加控件通知处理程序代码
	//调用代码
	
	ITS_Vehicle_Result sResult;
	V_Image* pVImage = (V_Image*)malloc(sizeof(V_Image)*1);
	pVImage->eType =E_BGR ;
	pVImage->iImageHeight = tagImg->height;
	pVImage->iImageWidth = tagImg->width;

	pVImage->pImageData = (unsigned char*)malloc(sizeof(unsigned char)*pVImage->iImageHeight*pVImage->iImageWidth*3);
	int iIndex = 0;
	for (int i = 0; i < pVImage->iImageHeight; i ++)
	{
		unsigned char* p = (unsigned char*)(tagImg->imageData + tagImg->widthStep*i);
		for (int j = 0; j < pVImage->iImageWidth; j ++)
		{
			pVImage->pImageData[iIndex ++] = p[j*3 + 0];
			pVImage->pImageData[iIndex ++] = p[j*3 + 1];
			pVImage->pImageData[iIndex ++] = p[j*3 + 2];
		}
	}
	VehicleTypeRec vehicleTypeRec;
	vehicleTypeRec = (VehicleTypeRec)GetProcAddress(dlh, "ITS_VehicleTypeRec");
	(*vehicleTypeRec)(pInstance,pVImage,&sResult);
	//ITS_VehicleTypeRec(pInstance,pVImage,&sResult);
	CvPoint point1,point2;
	for (int j = 0; j < sResult.iNum; j++)
	{
		point1.x = sResult.tempResult[j].plateRect.iLeft;
		point1.y = sResult.tempResult[j].plateRect.iTop;
		point2.x = sResult.tempResult[j].plateRect.iRight;
		point2.y = sResult.tempResult[j].plateRect.iBottom;

		vehinfo[j].pointLT = point1;
		vehinfo[j].pointRB = point2; //车牌位置
		strcpy_s(vehinfo[j].platenumber ,sResult.tempResult[j].plateInfor.platenumber); //车牌号
		strcpy_s(vehinfo[j].tempVehicleType,sResult.tempResult[j].tempVehicleType);
		switch (sResult.tempResult[j].eVehicleColor1)                            //车身颜色信息
		{
		case E_VEHICLE_RED:
			//printf("车身主颜色：红色");
			vehinfo[j].eVehicleColor ="红色";
			break;
		case E_VEHICLE_GREEN:
			//printf("车身主颜色：绿色");
			vehinfo[j].eVehicleColor ="绿色";
			break;
		case E_VEHICLE_BLUE:
			//printf("车身主颜色：蓝色");
			vehinfo[j].eVehicleColor ="蓝色";
			break;
		case E_VEHICLE_PINK:
			//printf("车身主颜色：粉色");
			vehinfo[j].eVehicleColor ="粉色";
			break;
		case E_VEHICLE_BROWN:
			//printf("车身主颜色：棕色");
			vehinfo[j].eVehicleColor ="棕色";
			break;
		case E_VEHICLE_YELLOW:
			//printf("车身主颜色：黄色");
			vehinfo[j].eVehicleColor ="黄色";
			break;
		case E_VEHICLE_WHITE:
			//printf("车身主颜色：白色");
			vehinfo[j].eVehicleColor ="白色";
			break;
		case E_VEHICLE_BLACK:
			//printf("车身主颜色：黑色");
			vehinfo[j].eVehicleColor ="黑色";
			break;
		case E_VEHICLE_GRAY:
			//printf("车身主颜色：灰色");
			vehinfo[j].eVehicleColor ="灰色";
			break;
		case E_VEHICLE_CYAN:
			//printf("车身主颜色：青色");
			vehinfo[j].eVehicleColor ="青色";
			break;
		default:
			//printf("车身主颜色：未知");
			vehinfo[j].eVehicleColor ="未知";
			break;
		}

		CString sInfo;
		sInfo.Format("plate position:\t\nleft:%d,top:%d\t\nright:%d,bottom:%d\t\nnumber:%s\t\ncolor:%s\t\ntype:%s\t\n",vehinfo[j].pointLT.x,vehinfo[j].pointLT.y,vehinfo[j].pointRB.x,vehinfo[j].pointRB.y,vehinfo[j].platenumber,vehinfo[j].eVehicleColor,vehinfo[j].tempVehicleType);
		this->SetDlgItemTextA(IDC_EDIT2,sInfo);
		CString vehValue;
		CString  tText;
		tText =sText;
		int idex ;
		if ( tText.Find("where")!=-1)
		{
			int iw = tText.Find("where");
            idex = tText.Find(" limit");
			tText.Delete(iw,idex-iw);
		}
		
		idex = tText.Find("limit");
		vehValue.Format("where number='%s' and color='%s' and  type='%s' ",vehinfo[j].platenumber,vehinfo[j].eVehicleColor,vehinfo[j].tempVehicleType);
		tText.Insert(idex,vehValue);
		SetDlgItemTextA(IDC_ETEXT,tText); //添加到编辑框显示

	}


	
}
