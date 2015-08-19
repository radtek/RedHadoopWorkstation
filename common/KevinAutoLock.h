//CKevinCritSec.h
//
#ifndef __KEVIN_CRITSEC_H__
#define __KEVIN_CRITSEC_H__

class CKevinCritSec
{
public:
    CKevinCritSec() {
        InitializeCriticalSection(&m_CritSec);
    };

    ~CKevinCritSec() {
        DeleteCriticalSection(&m_CritSec);
    };

    void Lock() {
        EnterCriticalSection(&m_CritSec);
    };

    void Unlock() {
        LeaveCriticalSection(&m_CritSec);
    };
protected:
    CRITICAL_SECTION m_CritSec;
};



class CKevinAutoLock
{
public:
    CKevinAutoLock(CKevinCritSec * plock)
    {
        m_pLock = plock;
        m_pLock->Lock();
    };

    ~CKevinAutoLock() {
        m_pLock->Unlock();
    };
protected:
    CKevinCritSec * m_pLock;
};


class CKevinEvent
{
public:
    CKevinEvent(bool bManualReset = false)
	{
	    m_hEvent = CreateEvent(NULL, bManualReset, false, NULL);
	};

    virtual ~CKevinEvent()
	{
		if(m_hEvent) 
		{
			CloseHandle(m_hEvent);
			m_hEvent=NULL;
		}		
	};
public:
    void Set(void) 
	{
		SetEvent(m_hEvent);
	};

    bool Wait(unsigned long ulTimeout)
	{
		if (ulTimeout==0)
			ulTimeout=-1;

		return (WaitForSingleObject(m_hEvent, ulTimeout) == WAIT_OBJECT_0);
    };

    void Reset(void) 
	{ 
		ResetEvent(m_hEvent); 
	};
protected:
	HANDLE m_hEvent;

};

#endif