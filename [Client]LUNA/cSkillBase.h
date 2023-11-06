#pragma once
#include "baseitem.h"

class cSkillInfo;

class cSkillBase :
	public CBaseItem
{
	SKILL_BASE m_SkillBaseInfo;
	cImage	m_pHighLayerImage;

	DWORD	m_dwFontColor;

	cImage  m_DelayImage;
	BOOL	m_bRemainTime;
	DWORD	m_dwLastSecond;

	// 070209 LYW --- cSkillBase : Add Rect.
	RECT m_rect ;

public:
	cSkillInfo* m_pSkillInfo;

public:
	cSkillBase(void);
	virtual ~cSkillBase(void);

	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * lowImage, cImage * highImage, LONG ID=0);
	virtual void Render();
	
	virtual inline DWORD	GetLastSecond()					{	return m_dwLastSecond; }
	virtual inline DWORD	GetDBIdx()					{	return m_SkillBaseInfo.dwDBIdx;	}
	virtual inline DWORD	GetSkillIdx()				{	return m_SkillBaseInfo.wSkillIdx;	}
	virtual inline DWORD	GetItemIdx()				{	return m_SkillBaseInfo.wSkillIdx; }
	virtual inline void		SetLevel(BYTE level)				{	m_SkillBaseInfo.Level = level;		}
	virtual inline BYTE		GetLevel()						{	return m_SkillBaseInfo.Level;		}
	
	void SetSkillBase( SKILL_BASE* pSkill );
	SKILL_BASE* GetSkillBase() { return &m_SkillBaseInfo; }

	void SetFontColor( DWORD dwFontColor ) { m_dwFontColor = dwFontColor; }
};
