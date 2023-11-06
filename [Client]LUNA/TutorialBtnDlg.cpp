//=================================================================================================
//	FILE		: TutorialDlg.cpp
//	PURPOSE		: Implemention part of tutorial button dialog.
//	PROGRAMMER	: Yongs Lee
//	DATE		: October 23, 2007
//	ADDITION	:
//	ATTENTION	:
//=================================================================================================





//=========================================================================
//		INCLUDE HEADER FILES
//=========================================================================
#include "stdafx.h"															// 표준 헤더파일을 포함한다.
#include ".\tutorialbtndlg.h"												// 튜토리얼 알림 버튼 다이얼로그 헤더를 포함한다.

#include "../WindowIDEnum.h"												// 윈도우 아이디 정의 헤더를 포함한다.

#include "./Interface/cButton.h"											// 버튼 헤더를 포함한다.

#include "TutorialDlg.h"													// 튜토리얼 다이얼로그 헤더를 포함한다.

#include "GameIn.h"															// gamein 헤더를 포함한다.

#include "./Audio/MHAudioManager.h"											// 오디오 매니져를 불러 온다.
#include "TutorialManager.h"												// 튜토리얼 매니져를 불러온다.
#include "ObjectManager.h"													// 오브젝트 매니져를 포함한다.
#include "OptionManager.h"													// 옵션 매니져를 포함한다.





//=========================================================================
//		튜토리얼 알림 버튼 다이얼로그 생성자 함수.
//=========================================================================
cTutorialBtnDlg::cTutorialBtnDlg(void)										// 생성자 함수.
{
	m_pButton = NULL ;														// 버튼 포인터를 null 처리를 한다.

	m_nAlpha = 255 ;														// 버튼 알파 값을 255로 세팅한다.
}





//=========================================================================
//		튜토리얼 알림 버튼 다이얼로그 소멸자 함수.
//=========================================================================
cTutorialBtnDlg::~cTutorialBtnDlg(void)										// 소멸자 함수.
{
}





//=========================================================================
//		하위 컨트롤들을 링크하는 함수.
//=========================================================================
void cTutorialBtnDlg::Linking() 											// 하위 컨트롤들을 링크하는 함수.
{
	m_pButton = (cButton*)GetWindowForID(TTR_NOTICEBTN) ;					// 공지 버튼을 링크한다.

	//if( OPTIONMGR->GetGameOption()->bShowTutorial )
	//{
	//	m_pButton->SetActive(TRUE) ;										// 비활성화 한다.
	//}
	//else
	//{
	//	m_pButton->SetActive(FALSE) ;										// 비활성화 한다.
	//}
}




//=========================================================================
//		이벤트 처리를 위한 함수.
//=========================================================================
void cTutorialBtnDlg::OnActionEvent(LONG lId, void* p, DWORD we ) 			// 이벤트 처리를 위한 함수.
{
	if( lId != TTR_NOTICEBTN ) return ;										// 공지 버튼이 아니면 return 처리를 한다.

	cTutorialDlg* pDlg = GAMEIN->GetTutorialDlg() ;							// 튜토리얼 다이얼로그를 얻는다.

	if( !pDlg ) return ;													// 튜토리얼 다이얼로그가 유효하지 않으면, 

	pDlg->Start_Tutorial() ;												// 튜토리얼을 시작한다.

	SetActive(FALSE) ;														// 공지 버튼 다이얼로그를 비활성화 한다.
	m_pButton->SetActive(FALSE) ;											// 버튼을 비활성화 한다.
}





//=========================================================================
//		렌더 함수.
//=========================================================================
void cTutorialBtnDlg::Render() 												// 렌더 함수.
{
	if( !IsActive() ) return ;												// 활성화 상태가 아니면, return 처리를 한다.

	DWORD dwCurTick = GetTickCount() ;

	DWORD dwResultTime = dwCurTick - m_dwStartTime ;

	if( dwResultTime >= 1000 )
	{
		m_dwStartTime = GetTickCount() ;
	}

	m_nAlpha = (int)(2.5f * (dwResultTime/10)) ;

	m_pButton->SetAlpha((BYTE)m_nAlpha) ;									// 버튼에 알파 값을 적용한다.

	cDialog::RenderWindow();												// 윈도우를 렌더한다.
	cDialog::RenderComponent();												// 하위 객체들을 렌더한다.

	//m_pButton->Render() ;													// 버튼을 렌더한다.
}





//=========================================================================
//		튜토리얼 공지 버튼을 활성화 시키는 함수.
//=========================================================================
void cTutorialBtnDlg::Active_NoticeBtn() 									// 튜토리얼 공지 버튼을 활성화 시키는 함수.
{
	SetActive(TRUE) ;														// 다이얼로그를 활성화 시킨다.

	m_nAlpha = 255 ;														// 버튼 알파 값을 255로 세팅한다.

	m_dwStartTime = GetTickCount() ;										// 시작 시간을 받는다.

	m_pButton->SetActive(TRUE) ;											// 버튼을 활성화 한다.

	AUDIOMGR->Play(TUTORIALMGR->Get_ShowBtnSndIdx(), HERO);					// 버튼 출연 사운드를 플레이 한다.
}