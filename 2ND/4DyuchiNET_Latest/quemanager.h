// ��Ƽ�������� ������ �츮�� ���ؼ��� ���� io�� ó���ϴ� ���ȿ��� ������ �޽����� ó���ϰ� �־�� �Ѵ�.
// ���� writable�� readableť�� ���� worker������� writableť���� �����ϰ� readableť�� 
// ���� �����常 �����ϵ��� �Ѵ�.���� �����尡 readableť�� �޽����� �� ó���ϸ� ť�� �ٲٵ��� �����Ѵ�.
// ��� ü��� ���� �����忡 �켱���� �ִ�.�����߰� ���� �����尡 �޽����� ó���ϰ� �ִ� ���� ť�� �ٲ�� ������
// �� ���� (���writableť�� �� ����������) worker������� �޽����� �����ؼ� writableť���� �׾Ƴ��� �ȴ�.
// �� �ҽ��� �� �ΰ��� ť�� �����ϴ� ť �Ŵ��� Ŭ������ ������ ���̴�.

#ifndef QUE_MANAGER_H
#define QUE_MANAGER_H

#include "que.h"

class CQueManager
{
public:
	CMsgQue*			m_pWritableQue;		// ���Ⱑ�� �޽��� ť�� ������.��Ȯ���� worker�����忡 ���ؼ� ���⸸ ��������.
	CMsgQue*			m_pReadableQue;		// �бⰡ�� �޽��� ť�� ������.��Ȯ���� ���� �����忡 ���ؼ� �б⸸ ��������.
public:
	CMsgQue*			GetWritableQue() {return m_pWritableQue;}	// write������ ť�������͸� ��´�.
	CMsgQue*			GetReadableQue() {return m_pReadableQue;}	// read������ ť�� �����͸� ��´�.
	
	void				Initialize(DWORD dwQueSize,HANDLE hMsgEvent);	// �ʱ�ȭ.
	void				Release();		// �����Ҷ� �޸� ������.
	CQueManager();
	~CQueManager();
};

extern CQueManager* g_pQueManager;

#endif

