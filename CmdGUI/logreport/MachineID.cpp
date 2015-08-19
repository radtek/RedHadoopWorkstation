#include "StdAfx.h"
#include "MachineID.h"


CMachineID::CMachineID(void)
{
}


CMachineID::~CMachineID(void)
{
}


bool CMachineID::Init()
{
	GetCPUID();
	GetDiskID();
	return true;
}

std::string CMachineID::GetUserID()
{
	return cpuid+"-"+diskid;
}

void CMachineID::GetCPUID()
{
	/*char h1[260]; 
	unsigned long s1,s2,s3,s4; 
	__asm 
	{ 
	mov eax,00h 
	xor edx,edx 
	cpuid 
	mov s1, edx 
	mov s2, eax 
	} 
	__asm 
	{ 
	mov eax,01h 
	xor ecx,ecx 
	xor edx,edx 
	cpuid 
	mov s3, edx 
	mov s4, ecx 
	} 

	sprintf(h1,"%08X%08X%08X%08X",s1,s2,s3,s4);

	cpuid = h1;
	*/
}
void CMachineID::GetDiskID()
{
	/*CString id;  
	char Name[MAX_PATH];  
	DWORD serno;  
	DWORD length;  
	DWORD FileFlag;  
	char FileName[MAX_PATH];  
	BOOL Ret;  
	Ret = GetVolumeInformation("c:\\", Name, MAX_PATH, &serno, &length, &FileFlag, FileName, MAX_PATH);  
	if (Ret)  
	id.Format( "%x", serno);  
	diskid = id;*/
	//diskid = id;
}

