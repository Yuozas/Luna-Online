#pragma once


#include "cWindow.h"

class cIcon;
class cButton;


class cDialog : public cWindow  
{
	friend class cWindowManager;

public:
	cDialog();
	virtual ~cDialog();

	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);
	virtual void Render();
	virtual void RenderWindow();
	virtual void RenderComponent();
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual DWORD ActionKeyboardEvent( CKeyboard* keyInfo );

	virtual DWORD ActionEventWindow(CMouse * mouseInfo);
	virtual DWORD ActionEventComponent(CMouse * mouseInfo);
	virtual DWORD ActionKeyboardEventComponent(CKeyboard * keyInfo);

	virtual void SetAutoClose(BOOL val){ m_bAutoClose = val; }
	virtual BOOL IsAutoClose(){ return m_bAutoClose; }
	BOOL PtInCaption(LONG x, LONG y);
	virtual void Add(cWindow * window);
	void DestroyCtrl();
	virtual void SetAbsXY(LONG x, LONG y);
	virtual void SetActive( BOOL val );
	virtual void SetActiveRecursive(BOOL val);
	virtual void SetAlpha(BYTE al);
	virtual void SetOptionAlpha( DWORD dwAlpha );

	// id: 이벤트가 발생한 컨트롤 아이디, p: 이벤트가 발생한 컨트롤 포인터, event: 처리할 이벤트 종류
	virtual void OnActionEvent( LONG id, void* p, DWORD event ) {};

	void SetCaptionRect(RECT * pRect){ CopyRect(&m_captionRect, pRect); }
	RECT* GetCaptionRect() { return &m_captionRect; }

	virtual void SetDisable( BOOL val );
	
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon){return FALSE;}
	virtual void FakeDeleteIcon(WORD pos, cIcon * icon){}
	
	virtual cWindow * GetWindowForID(LONG id);
	

protected:	
	BOOL	m_bCaptionHit;

	BOOL m_bAutoClose;
	RECT m_captionRect;

	//cGuageBar * m_pGuageBar;
	cButton * m_pCloseBtn;
	
	cPtrList m_pComponentList;
};