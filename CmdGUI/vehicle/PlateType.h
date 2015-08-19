#ifndef PLATE_TYPE_H
#define PLATE_TYPE_H




/***************������ɫ******************/
typedef enum
{
	E_PLATE_COLOR_NOT,  //δ֪
	E_PLATE_COLOR_BLUE,
	E_PLATE_COLOR_BLACK,
	E_PLATE_COLOR_YELLOW,
	E_PLATE_COLOR_WHITE,
}E_Plate_Color; 



/***************��������******************/
typedef enum
{
	E_PLATE_TYPE_NOT,          //δ֪
	E_PLATE_TYPE_NORMAL_BLUE,  //��ͨ����
	E_PLATE_TYPE_NORMAL_BLACK, //��ͨ����
	E_PLATE_TYPE_NORMAL_YELLOW,//��ͨ�������
	E_PLATE_TYPE_DOUBLE_YELLOW,//˫�����
	E_PLATE_TYPE_POLICE,       //��ɫ����
	E_PLATE_TYPE_WJ,           //��ɫ�侯
	E_PLATE_TYPE_ARMY,         //��ɫ����
	E_PLATE_TYPE_GANG,         //����
	E_PLATE_TYPE_NONG,         //ũ�ó���
}E_Plate_Type; 



typedef struct
{
	E_Plate_Color ePlateColor;
	E_Plate_Type  ePlateType;
	char platenumber[20];         //���ƺ���
}PlateInfor;




#endif