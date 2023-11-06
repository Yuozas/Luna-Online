#include "stdafx.h"
#include "cDialog.h"
#include "cWindowManager.h"
#include "cbutton.h"
#include "../WindowIDEnum.h"
#include "../Input/Mouse.h"


cDialog::cDialog()
{
	m_type = WT_DIALOG;
	m_bMovable = TRUE;
	///m_pGuageBar = NULL;
	m_pCloseBtn = NULL;
	m_absPos.x = 0;
	m_absPos.y = 0;
	m_pCloseBtn = 0;
	m_bAutoClose = FALSE;
	m_bCaptionHit = FALSE;
}

cDialog::~cDialog()
{
	DestroyCtrl();
}

void cDialog::DestroyCtrl()
{
	PTRLISTPOS pos = m_pComponentList.GetHeadPosition();
	while(pos)
	{
		cWindow * window = (cWindow *)m_pComponentList.GetNext(pos);
		SAFE_DELETE(window);
	}	
	m_pComponentList.RemoveAll();
}

void cDialog::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cWindow::Init(x,y,wid,hei,basicImage,ID);
	//aziz
	POINT standardPosition = {0};
	SIZE standardResolution = {0};
	SIZE currentResolution = {0};

	GetVariablePostion(
		standardResolution,
		standardPosition,
		currentResolution);

	const POINT centerPoint = {standardResolution.cx / 2, standardResolution.cy / 2};
	const RECT dialogArea = {standardPosition.x, standardPosition.y, standardPosition.x + GetWidth(), standardPosition.y + GetHeight()};

	// 091221 LUJ, 원점을 기준으로 대화상자를 네 부분으로 나누고, 분할된 길이/높이를 구한다
	const LONG widthLeftByCenter = (dialogArea.right < centerPoint.x ? GetWidth() : max(0, centerPoint.x - dialogArea.left));
	const LONG widthRightByCenter = (dialogArea.left > centerPoint.x ? GetWidth() : max(0, dialogArea.right - centerPoint.x));
	const LONG heightTopByCenter = (dialogArea.bottom < centerPoint.y ? GetHeight() : max(0, centerPoint.y - dialogArea.top));
	const LONG heightBottomByCenter = (dialogArea.top > centerPoint.y ? GetHeight() : max(0, dialogArea.bottom - centerPoint.y));

	// 091221 LUJ, 1~4분면에서 점유하는 비율을 구한다
	const float sizeDialog = float(GetWidth() * GetHeight());
	const float ratioFirstArea = float(widthLeftByCenter * heightTopByCenter) / sizeDialog;	
	const float ratioSecondArea = float(widthRightByCenter * heightTopByCenter) / sizeDialog;
	const float ratioThirdArea = float(widthLeftByCenter * heightBottomByCenter) / sizeDialog;
	const float ratioFourthArea = float(widthRightByCenter * heightBottomByCenter) / sizeDialog;

	// 091221 LUJ, 특정 분면의 중앙에 위치할 경우를 가려낸다
	const BOOL isAlignFirstBetweenSecondArea = (0.22f < ratioFirstArea && 0.22f < ratioSecondArea);
	const BOOL isAlignFirstBetweenThirdArea = (0.22f < ratioFirstArea && 0.22f < ratioThirdArea);
	const BOOL isAlignSecondBetweenFourthArea = (0.22f < ratioSecondArea && 0.22f < ratioFourthArea);
	const BOOL isAlignThirdBetweenFourthArea = (0.22f < ratioThirdArea && 0.22f < ratioFourthArea);

	// 091221 LUJ, 1~2분면을 점유할 경우, 상단 패딩을 맞추고 X축 중앙 정렬한다
	if(isAlignFirstBetweenSecondArea)
	{
		SetAbsXY(
			max(0, currentResolution.cx - GetWidth()) / 2,
			LONG(GetAbsY()));
	}
	// 091221 LUJ, 1~3분면을 점유할 경우, 좌측 패딩을 맞추고 Y축 중앙 정렬한다
	else if(isAlignFirstBetweenThirdArea)
	{
		SetAbsXY(
			LONG(GetAbsX()),
			max(0, currentResolution.cy - GetHeight()) / 2);
	}
	// 091221 LUJ, 2~4분면을 점유할 경우, 우측 패딩 맞추고 Y축 중앙 정렬한다
	else if(isAlignSecondBetweenFourthArea)
	{
		const LONG paddingRight = max(0, standardResolution.cx - dialogArea.right);
		const LONG positionX = max(0, currentResolution.cx - GetWidth() - paddingRight);

		SetAbsXY(
			positionX,
			max(0, currentResolution.cy - GetHeight()) / 2);
	}
	// 091221 LUJ, 3~4분면을 점유할 경우, 하단 패딩을 맞추고 X축 중앙 정렬한다
	else if(isAlignThirdBetweenFourthArea)
	{
		const LONG paddingBottom = max(0, standardResolution.cy - dialogArea.bottom);
		const LONG positionY = max(0, currentResolution.cy - GetHeight() - paddingBottom);

		SetAbsXY(
			max(0, currentResolution.cx - GetWidth()) / 2,
			positionY);
	}
	// 점유율을 계산해서, 해당 분면을 기준으로 정렬시킨다
	else
	{
		if(0.25f < ratioFirstArea)
		{
			// 091221 LUJ, 수작업으로 설정한 좌표가 좌측 상단을 원점으로
			//			삼았기 때문에 처리하지 않아도 된다
		}
		else if(0.25f < ratioSecondArea)
		{
			const LONG paddingX = max(0, standardResolution.cx - dialogArea.right);
			const LONG positionX = max(0, currentResolution.cx - GetWidth() - paddingX);

			SetAbsXY(
				positionX,
				dialogArea.top);
		}
		else if(0.25f < ratioThirdArea)
		{
			const LONG paddingY = max(0, standardResolution.cy - dialogArea.bottom);
			const LONG positionY = max(0, currentResolution.cy - GetHeight() - paddingY);

			SetAbsXY(
				dialogArea.left,
				positionY);
		}
		else if(0.25f < ratioFourthArea)
		{
			const LONG paddingX = max(0, standardResolution.cx - dialogArea.right);
			const LONG paddingY = max(0, standardResolution.cy - dialogArea.bottom);
			const LONG positionX = max(0, currentResolution.cx - GetWidth() - paddingX);
			const LONG positionY = max(0, currentResolution.cy - GetHeight() - paddingY);

			SetAbsXY(
				positionX,
				positionY);
		}
		else
		{
			// 091221 LUJ, 영역을 벗어나지 않도록 보정한다
			const LONG positionY = (standardResolution.cy < dialogArea.bottom ? currentResolution.cy - GetHeight() : LONG(GetAbsY()));

			SetAbsXY(
				(currentResolution.cx - GetWidth()) / 2,
				positionY);
		}
	}
}

DWORD cDialog::ActionEvent(CMouse * mouseInfo)
{
	if( !m_bActive ) return WE_NULL;

	DWORD we = WE_NULL;
	we |= cWindow::ActionEvent(mouseInfo);

	if( !m_bDisable )
		we |= ActionEventWindow(mouseInfo);

	we |= ActionEventComponent(mouseInfo);
	return we;
}

DWORD cDialog::ActionEventWindow(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
//	if( !m_bActive ) return we;

//	long x = mouseInfo->GetMouseX();
//	long y = mouseInfo->GetMouseY();
	
//KES TEST 031004
	long x2 = mouseInfo->GetMouseEventX();
	long y2 = mouseInfo->GetMouseEventY();
//

	BOOL LeftUp = mouseInfo->LButtonUp();
	BOOL LeftPress = mouseInfo->LButtonPressed();
	BOOL click = mouseInfo->LButtonDown();

	if( m_pCloseBtn )
	{
		we = m_pCloseBtn->ActionEvent(mouseInfo);

		if(we & WE_BTNCLICK)
		{
			cbWindowFunc(m_ID, m_pParent, WE_CLOSEWINDOW);	//PPP
			cWindowManager::OnWindowEvent( m_ID, this, WE_CLOSEWINDOW );
			return (we |= WE_CLOSEWINDOW);
		}

		if( m_pCloseBtn->IsClickInside() )
			return we;
	}

/*
	if(m_pGuageBar)
	{
		m_pGuageBar->ActionEvent(mouseInfo, keyInfo);
		SetAlpha(m_pGuageBar->GetCurValue());
	}
*/
	

//for drag
//	if( !m_bCaptionHit )
	if( mouseInfo->LButtonDown() && !WINDOWMGR->IsMouseDownUsed() )
	{
		if( PtInWindow( x2, y2 ) )
		{
			if( m_bMovable && PtInCaption( x2, y2 ) )
				m_bCaptionHit = TRUE;
			
			we |= WE_TOPWINDOW;
		}
	}

	if( mouseInfo->LButtonUp() )
		m_bCaptionHit = FALSE;

	if( m_bCaptionHit )
	if( mouseInfo->LButtonDrag() )
	if(WINDOWMGR->m_OldDragFlag == FALSE)
	{					
		SetDepend(FALSE);
		
		//first enter
		float X = (float)x2;
		float Y = (float)y2;
		LONG OldX = (LONG)(X>m_absPos.x?X-m_absPos.x:m_absPos.x-X);
		LONG OldY = (LONG)(Y>m_absPos.y?Y-m_absPos.y:m_absPos.y-Y);
		WINDOWMGR->SetDragStart((LONG)m_absPos.x, (LONG)m_absPos.y, OldX, OldY, m_ID);
		
		m_bCaptionHit = FALSE;
	}

	return we;
}

DWORD cDialog::ActionEventComponent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if(!m_bActive) return we;
	PTRLISTPOS pos = m_pComponentList.GetTailPosition();
	cWindow * window;

	while(pos)
	{
		window = (cWindow *)m_pComponentList.GetPrev(pos);

		if( window->IsActive() )
			we |= window->ActionEvent(mouseInfo);
	}
	return we;
}


DWORD cDialog::ActionKeyboardEvent(CKeyboard * keyInfo)
{
	DWORD we = WE_NULL;

	if( !m_bActive ) return we;

	we |= cWindow::ActionKeyboardEvent(keyInfo);
	we |= ActionKeyboardEventComponent(keyInfo);

	return we;
}


DWORD cDialog::ActionKeyboardEventComponent(CKeyboard * keyInfo)
{
	DWORD we = WE_NULL;
	if(!m_bActive) return we;
	PTRLISTPOS pos = m_pComponentList.GetTailPosition();
	cWindow * window;

	while(pos)
	{
		window = (cWindow *)m_pComponentList.GetPrev(pos);
		if( window->IsActive() )
			we = window->ActionKeyboardEvent(keyInfo);
	}

	return we;
}


void cDialog::Add(cWindow * window)
{
/*
	if(!m_pGuageBar && window->GetID() == CMI_ALPHAGUAGE)
	{
		m_pGuageBar = (cGuageBar *)window;
	}*/

	if(!m_pCloseBtn && window->GetID()== CMI_CLOSEBTN)
	{
		m_pCloseBtn = (cButton *)window;
	}

	window->SetAbsXY((LONG)(m_absPos.x+window->GetRelX()), (LONG)(m_absPos.y+window->GetRelY()));

	window->SetParent((cObject *)this);
	m_pComponentList.AddTail((void *)window);
}

void cDialog::SetActive( BOOL val )
{
	if( val == TRUE && m_bSetTopOnActive )	//제일 위로.
		WINDOWMGR->SetWindowTop( this );

	if( m_bDisable ) return;

	cWindow::SetActive(val);
}

void cDialog::SetActiveRecursive(BOOL val)
{
	if( m_bDisable ) return;
	
	cWindow::SetActive(val);

	PTRLISTPOS pos = m_pComponentList.GetHeadPosition();
	while(pos)
	{
		cWindow * window = (cWindow *)m_pComponentList.GetNext(pos);
		window->SetActive(val);
	}
	
	if( val == TRUE )	//제일 위로.
		WINDOWMGR->SetWindowTop( this );
}

void cDialog::SetAlpha(BYTE al)
{ 
	cWindow::SetAlpha(al);
	
	PTRLISTPOS pos = m_pComponentList.GetHeadPosition();
	while(pos)
	{
		cWindow * window = (cWindow *)m_pComponentList.GetNext(pos);
		// desc_hseos_몬스터미터01
		// S 몬스터미터 추가 added by hseos 2007.05.30
		// ..부모의 알파값을 적용하지 않고 자신의 알파값이 설정된 컨트롤이면 건너뛴다.
		if (window->GetUseOwnAlpha()) continue;
		// E 몬스터미터 추가 added by hseos 2007.05.30
		window->SetAlpha(al);
	}
}

//KES 030825
void cDialog::SetOptionAlpha( DWORD dwAlpha )
{ 
	cWindow::SetOptionAlpha( dwAlpha );
	PTRLISTPOS pos = m_pComponentList.GetHeadPosition();
	while(pos)
	{
		cWindow * window = (cWindow *)m_pComponentList.GetNext(pos);
		window->SetOptionAlpha( dwAlpha );
	}
}


void cDialog::Render()
{
	if(!m_bActive) return;	
	cDialog::RenderWindow();
	cDialog::RenderComponent();
	//ToolTipRender(pGeometry);
}
void cDialog::RenderWindow()
{
	if(!m_bActive) return;	
	cWindow::Render();
}
void cDialog::RenderComponent()
{
	if(!m_bActive) return;	
	//window of componetList
	PTRLISTPOS pos = m_pComponentList.GetHeadPosition();
	while(pos)
	{
		cWindow * window = (cWindow *)m_pComponentList.GetNext(pos);
		if(window->IsActive())
		{

			window->Render();
		}
	}
}

BOOL cDialog::PtInCaption(LONG x, LONG y)
{
	if(m_absPos.x + m_captionRect.left > x || m_absPos.y + m_captionRect.top > y || m_absPos.x + m_captionRect.right < x || m_absPos.y + m_captionRect.bottom < y)
		return FALSE;
	else
		return TRUE;
}

cWindow * cDialog::GetWindowForID(LONG id)
{
	PTRLISTPOS pos = m_pComponentList.GetHeadPosition();
	while(pos)
	{
		cWindow * win = (cWindow *)m_pComponentList.GetNext(pos);
		if(win->GetID() == id)
			return win;
	}
	return NULL;
}

void cDialog::SetAbsXY(LONG x, LONG y)
{
	cWindow::SetAbsXY(x,y);
	PTRLISTPOS pos = m_pComponentList.GetHeadPosition();
	while(pos)
	{
		cWindow * win = (cWindow *)m_pComponentList.GetNext(pos);
		win->SetAbsXY(x+(LONG)win->GetRelX(), y+(LONG)win->GetRelY());
	}
}

void cDialog::SetDisable( BOOL val )
{
	m_bDisable = val;
	PTRLISTPOS pos = m_pComponentList.GetHeadPosition();
	while(pos)
	{
		cWindow * win = (cWindow *)m_pComponentList.GetNext(pos);
		win->SetDisable( val );
	}
}
