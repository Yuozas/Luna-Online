// BuyItem.h: interface for the CBuyItem class.
// ���ſ� ���� ������
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUYITEM_H__3002E03D_BE93_4267_B015_37BB522D150A__INCLUDED_)
#define AFX_BUYITEM_H__3002E03D_BE93_4267_B015_37BB522D150A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cIcon.h"

class CItem;

class CBuyItem : public cIcon  
{
	DWORD		m_ItemIdx;
	WORD		m_nVolume;
	DWORD		m_nMoney;
	ITEMPARAM	m_dwParam;
	BOOL		m_bVolume;
	WORD		m_wSeal;

	// 070209 LYW --- BuyItem : Add Rect.
	RECT m_rect ;
		
public:
	CBuyItem();
	virtual ~CBuyItem();

	void InitItem(ITEMBASE ItemInfo, WORD Volume, DWORD Money);
	
	WORD GetVolume() { return m_nVolume; }
	void SetVolume(WORD Volume) { m_nVolume = Volume; }
	
	DWORD GetMoney() { return m_nMoney; }
	void SetMoney(DWORD Money) { m_nMoney = Money; }

	DWORD GetItemIdx() { return m_ItemIdx; }

	WORD GetSeal() { return m_wSeal; }

	//CItem* GetItem() { return m_pItem; }

	virtual void Render();
};

#endif // !defined(AFX_BUYITEM_H__3002E03D_BE93_4267_B015_37BB522D150A__INCLUDED_)
