#if !defined _ItemType_h
#define _ItemType_h


class CItem
{
public:

	//ItemFlag����ڵ�ļ���,1����1���ڵ�,2����2���ڵ�
	UINT8		ItemFlag;

	CString		ItemName;

	CString		Value;

	int			ItemParent;				//������ʶ�ýڵ�ĸ��ڵ�
};

#endif