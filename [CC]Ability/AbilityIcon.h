/**************本资源来自互联网 开发者中心提示禁商业 仅学习*******************/


/*************站长 星期八 感谢广大用户对开发者中心游戏资源社区的大力支持*****************/
// AbilityIcon.h: interface for the CAbilityIcon class.

//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_ABILITYICON_H__704928D4_D243_4636_B070_C8ED0B758C66__INCLUDED_)

#define AFX_ABILITYICON_H__704928D4_D243_4636_B070_C8ED0B758C66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "BaseItem.h"
class CAbilityInfo;
class CAbility;

class CAbilityIcon : public CBaseItem  
{
	int m_State;
	int m_Level;
	// 06. 01. 绢呼府萍芒 牢磐其捞胶 荐沥 - 捞康霖
	// 弥辆饭骇俊 档崔窍搁 枨磊甫 嘛绢林绢具 茄促
	// 坊歹且锭付促 拌魂窍绰芭焊促 檬扁拳矫俊 茄锅父 拌魂窍磊
	int m_MaxLevel;
	BOOL m_bAbilityActive;

	CAbilityInfo* m_pAbilityInfo;
	
	cImage m_ScrollImg[3];
	int m_CurScrollImage;
public:
	CAbilityIcon();
	virtual ~CAbilityIcon();
	

	CAbilityInfo* GetAbilityInfo()	{	return m_pAbilityInfo;	}

	void InitAbilityIcon(CAbilityInfo* pInfo);
	void UpdateState(int State,int Level);
	virtual void Render();
	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);


	virtual inline POSTYPE	GetQuickPosition();

	virtual inline POSTYPE	GetPosition();
	virtual inline void		SetQuickPosition(POSTYPE pos);

	virtual inline WORD		GetItemIdx();

	int GetState() { return m_State; }
	int GetLevel() { return m_Level; }

	void RenderInfo(VECTOR2* pPos);


};

#endif // !defined(AFX_ABILITYICON_H__704928D4_D243_4636_B070_C8ED0B758C66__INCLUDED_)


