#include "stdafx.h"
#include ".\quickslot.h"
#include "MapDBMsgParser.h"

cQuickSlot::cQuickSlot(void)
{
	memset( mSlot, 0, sizeof(SLOT_INFO) * 10 );
}

cQuickSlot::~cQuickSlot(void)
{
}

void cQuickSlot::Init( DWORD characterIdx, BYTE num ) 
{ 
	mCharacterIdx = characterIdx; 
	mTabNum = num; 
}

BOOL cQuickSlot::Update( WORD pos, SLOT_INFO* pSlot )
{
	if( pos > 10 )
	{
		return FALSE;
	}

	mSlot[ pos ].kind = pSlot->kind;
	mSlot[ pos ].dbIdx = pSlot->dbIdx;
	mSlot[ pos ].idx = pSlot->idx;
	mSlot[ pos ].data = pSlot->data;

	return TRUE;
}

void cQuickSlot::DBUpdate()
{
	QuickUpdate( mCharacterIdx, mTabNum, mSlot );
} 

SLOT_INFO* cQuickSlot::GetQuick( WORD pos )
{
	return &( mSlot[ pos ] );
}
