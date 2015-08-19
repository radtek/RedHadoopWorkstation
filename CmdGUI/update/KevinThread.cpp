// AVThread.cpp: implementation of the CKevinThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KevinThread.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKevinThread::CKevinThread()
{
	m_dwThreadID=0;
	m_hThread=NULL;
	m_evStop = CreateEvent(NULL,true,true,NULL);
	SetEvent(m_evStop);
}

CKevinThread::~CKevinThread()
{
	CloseHandle(m_evStop);
}

bool CKevinThread::StartThread()
{
	//����߳��Ѿ�����������Ҫ�ٴ���
	if (!m_hThread){ 
		//�����������̺߳���
		m_hThread = CreateThread(
					NULL,			//the handle cannot be inherited. 
                    0,				//the default Thread Stack Size is set
                    InitThreadProc,	//�̺߳���
                    this,			//�̺߳����Ĳ���
                    0,				//ʹ�̺߳������������������
                    &m_dwThreadID	//receives the thread identifier
					);
                
        }//end if (!m_hThread...

	if (m_hThread)
		ResetEvent(m_evStop);

	return	m_hThread != NULL;
}

void CKevinThread::WaitForStop()
{
	WaitForSingleObject(m_evStop,INFINITE);

	// �����߳̾��
	HANDLE hThread = (HANDLE)InterlockedExchange((LONG *)&m_hThread, 0);
	if (hThread) {
		// �ȴ��߳���ֹ
		WaitForSingleObject(hThread, INFINITE);
		// �ر��߳̾��
		CloseHandle(hThread);
	}// end if (hThread...
}

unsigned long  CKevinThread::ThreadProc()
{
	ThreadProcMain();
	TRACE("m_evStopm_evStopm_evStop\n");
	SetEvent(m_evStop);

	return 0;
}

