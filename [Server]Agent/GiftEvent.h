#pragma once
#include "stdafx.h"

struct EVENTINFO;

class cGiftEvent
{
	DWORD		mID;			// �̺�Ʈ ID
	DWORD		mCharacterIdx;	// ��� ĳ����
		
	EVENTINFO*	mpInfo;			// �̺�Ʈ ����
	
	DWORD		mDelay;			// ���� �ð�
	DWORD		mCheckTime;		//

public:
	cGiftEvent(void);
	~cGiftEvent(void);

	BOOL	Init( DWORD CharacterIdx, DWORD ID, EVENTINFO* pInfo );
	BOOL	Excute();			// ���� �Լ�

	DWORD	GetID() { return mID; }

	void	Release();
};
