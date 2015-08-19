// AVThread.h: interface for the CAVThreadclass.
//
//////////////////////////////////////////////////////////////////////

#ifndef __AV_THREAD_H__
#define __AV_THREAD_H__

class CKevinThread
{
public:
	CKevinThread();
	virtual ~CKevinThread();
public:
	bool StartThread(void);
	void WaitForStop(void);

	static DWORD WINAPI InitThreadProc(PVOID pObj){
		return	((CKevinThread*)pObj)->ThreadProc();
	}
	
	unsigned long ThreadProc(void);

protected:
	virtual void ThreadProcMain(void)=0;
protected:
	DWORD	m_dwThreadID;		// �̱߳�ʶ
	HANDLE	m_hThread;			// �߳̾��
	HANDLE	m_evStop;
};

#endif
