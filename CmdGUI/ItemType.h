#if !defined _ItemType_h
#define _ItemType_h


class CItem
{
public:

	//ItemFlag定义节点的级别,1代表1级节点,2代表2级节点
	UINT8		ItemFlag;

	CString		ItemName;

	CString		Value;

	int			ItemParent;				//用来标识该节点的父节点
};

#endif