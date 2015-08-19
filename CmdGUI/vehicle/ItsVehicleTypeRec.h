#ifndef _ITS_VEHICLE_TYPE_REC_H
#define _ITS_VEHICLE_TYPE_REC_H
#include "PlateType.h"

#define  MAX_VEHICLE_NUM  (10)
#define  MAX_FACE_NUM     (10)

#define  OK           (0)     //��ʼ����ȷ
#define  DATA_OUT     (-1)    //SDK����


/***************������ɫ******************/
typedef enum
{
	E_VEHICLE_COLOR_NOT,//δ֪
	E_VEHICLE_RED,      //��ɫ
	E_VEHICLE_GREEN,    //��ɫ
	E_VEHICLE_BLUE,     //��ɫ
	E_VEHICLE_PINK,     //��ɫ
	E_VEHICLE_BROWN,    //��ɫ
	E_VEHICLE_YELLOW,   //��ɫ
	E_VEHICLE_WHITE,    //��ɫ
	E_VEHICLE_BLACK,    //��ɫ
	E_VEHICLE_GRAY,     //��ɫ
	E_VEHICLE_CYAN,     //��ɫ
}E_Vehicle_Color;


typedef enum 
{
	E_RGB,
	E_BGR,
	E_GRAY,
}V_ImageType;  //ͼƬ��ʽ

typedef struct
{
	int       iImageHeight;
	int       iImageWidth;
	V_ImageType eType;
	unsigned char *pImageData;
}V_Image; //ͼ���ʽ


typedef struct  
{
	int iLeft;
	int iTop;
	int iRight;
	int iBottom;
}S_Rect;


typedef struct
{
	char tempVehicleType[1024];    //��������
	float fConfdence;              //�����������Ŷ�
	PlateInfor plateInfor;         //������Ϣ
	S_Rect plateRect;              //����λ��
	S_Rect pVehicleRect;        //����λ��
	int iVehicleSubModel;          //����
	E_Vehicle_Color eVehicleColor1;//��������ɫ
	E_Vehicle_Color eVehicleColor2;//������ɫ
	
}ITS_Vehicle_Result_Single;


typedef struct
{
	ITS_Vehicle_Result_Single tempResult[MAX_VEHICLE_NUM];
	int iNum;
	float fLprTime;
	float fVtrTime;
}ITS_Vehicle_Result;

//extern "C" __declspec (dllexport) void  ITS_ThreadInit();
//
//extern "C" __declspec (dllexport) void * ITS_VehicleRecInit(char* modePath, int& iInitFlag);//modePathΪmodel�ļ��е�·����
//
//extern "C" __declspec (dllexport) int ITS_VehicleTypeRec(void* pInstance,V_Image* pVImage,ITS_Vehicle_Result* pResult);
//
//extern "C" __declspec (dllexport) void ITS_VehicleRecRelease(void* pInstance);





#endif