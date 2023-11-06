#pragma once
#include "stdafx.h"

struct EVENTINFO
{
	DWORD	Index;
	DWORD	Delay;
	DWORD	Next;
	DWORD	Item;
	DWORD	Count;
};

class cGiftEvent;

#define GIFTMGR	USINGTON( cGiftManager )

class cGiftManager
{
	// �̺�Ʈ ���� ���̺�
	CYHHashTable<EVENTINFO>			mEventInfoTable;
	
	// �̺�Ʈ ��ü ����
	CMemoryPoolTempl<cGiftEvent>*	mpGiftEventPool;		// �̺�Ʈ ��ü �޸�Ǯ
	CYHHashTable<cGiftEvent>		mGiftEventTable;		// �̺�Ʈ ��ü ���̺�
	CIndexGenerator					mIDCreater;				// �̺�Ʈ ��ü ID ������

	DWORD							mStartEventCount;		// ���� �̺�Ʈ ����
	DWORD*							mStartEventIndex;		// ���� �̺�Ʈ

public:
	cGiftManager(void);
	~cGiftManager(void);

	void	Init();											// �ʱ�ȭ
	void	Release();										// 

	void	AddEvent( DWORD CharacterIdx, DWORD EventIdx );	// �̺�Ʈ �߰�
	void	RemoveEvent( DWORD EventID );					// �̺�Ʈ ����
	void	ExcuteEvent( DWORD CharacterIdx, DWORD EventIdx );	// �̺�Ʈ ����

	void	Process();										// �̺�Ʈ ���μ���

	DWORD	GetStartEventCount() { return mStartEventCount; }
	DWORD*	GetStartEventIndex() { return mStartEventIndex; }
};

EXTERNGLOBALTON( cGiftManager )
