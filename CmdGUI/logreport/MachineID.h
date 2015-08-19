#pragma once
#include <string>

class CMachineID
{
public:
	CMachineID(void);
	~CMachineID(void);

	bool Init();
	std::string GetUserID();
private:
	void GetCPUID();
	void GetDiskID();
private:
	std::string cpuid;
	std::string diskid;
};

