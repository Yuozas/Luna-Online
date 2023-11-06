// ShoutDialog.h: interface for the CShoutDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOUTDIALOG_H__2851FDE7_5AC8_4835_8F46_900FC49814D7__INCLUDED_)
#define AFX_SHOUTDIALOG_H__2851FDE7_5AC8_4835_8F46_900FC49814D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"

class cStatic;
class cTextArea;
class cButton;
//class cEditBox;

class CItem ;


class CShoutDialog : public cDialog
{
	DWORD			m_dwItemIdx;
	DWORD			m_dwItemPos;

	cStatic *		m_pTitleStc;
	//cEditBox*		m_pMsgBox;
	cTextArea*		m_pMsgText;
	cButton *		m_pOkBtn;
	cButton *		m_pCancelBtn;

	CItem*			m_pItem ;													// 아이템 포인터.
	

public:
	CShoutDialog();
	virtual ~CShoutDialog();

	void Linking();
	BOOL SendShoutMsgSyn();
	virtual void SetActive(BOOL val);

	void SetItem(CItem* pItem) {m_pItem = pItem;}
	void SetItemInfo( DWORD ItemIdx, DWORD ItemPos )			{	m_dwItemIdx = ItemIdx;	m_dwItemPos = ItemPos;	}
};

#endif // !defined(AFX_SHOUTDIALOG_H__2851FDE7_5AC8_4835_8F46_900FC49814D7__INCLUDED_)
