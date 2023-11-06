#pragma once
#include "baseitem.h"

enum QuickKind
{
	QuickKind_None,
	QuickKind_SingleItem,
	QuickKind_MultiItem,
	QuickKind_Skill,
	// 070216 LYW --- cQuickItem : Add enum for conduct to QuickKind.
	QuickKind_Conduct,
	QuickKind_PetItem,
	QuickKind_PetSkill,
};

class cImage;

class cQuickItem :
	public CBaseItem
{
	cImage m_HighLayerImage;
	cImage m_LinkImage;
	cImage m_DelayImage;
	CBaseItem* m_pItemLink;

	SLOT_INFO mInfo;

	BOOL	m_bRemainTime;
	DWORD	m_dwLastSecond;

	WORD	mTab;
	WORD	mPos;

	char	mData[ 17 ];
public:
	cQuickItem( WORD tab, WORD pos );
	virtual ~cQuickItem(void);

	SLOT_INFO* GetSlotInfo() { return &mInfo; }
	void SetSlotData( WORD dur );
	void SetLinkItem( CBaseItem * pItem );
	CBaseItem* GetItem() { return m_pItemLink; }
	void GetPosition( WORD& tab, WORD& pos ) { tab = mTab; pos = mPos; }
	void ClearLink();

	void SetImage2(cImage * low, cImage * high);

	virtual void Render();
	void SetSkillIconToolTip( DWORD dwRemainTime );
	// 070221 LYW --- cQuickItem : Add function to clean data.
	void ClearData() { memset( mData, 0, sizeof(mData) ) ; }
};
