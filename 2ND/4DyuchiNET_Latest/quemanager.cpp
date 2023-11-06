#include "quemanager.h"

CQueManager* g_pQueManager = NULL;

// constructor
CQueManager::CQueManager()
{
	// ���� �ʱ�ȭ
	m_pWritableQue = NULL;
	m_pReadableQue = NULL;
}
// �ʱ� �޸� �Ҵ�
void CQueManager::Initialize(DWORD dwQueSize,HANDLE hMsgHandle)
{
	// wrtiable,readableť�� �����Ѵ�.
	m_pReadableQue = new CMsgQue(dwQueSize);
	m_pWritableQue = new CMsgQue(dwQueSize);

	// ũ������ ���� �ʱ�ȭ.
	InitializeCriticalSectionAndSpinCount(&g_csWrite,1000);

}


// �����Ҷ� �޸𸮸� �����Ѵ�.
void CQueManager::Release()
{
	// �ΰ��� ť�� ��� �����ϰ� ����ȭ ��ü�鵵 �����Ѵ�.
	if (m_pWritableQue)
	{
		delete m_pWritableQue;
		m_pWritableQue = NULL;
	}
	if (m_pReadableQue)
	{
		delete m_pReadableQue;
		m_pReadableQue = NULL;
	}
	DeleteCriticalSection(&g_csWrite);
}

// destructor
CQueManager::~CQueManager()
{
	Release();
}