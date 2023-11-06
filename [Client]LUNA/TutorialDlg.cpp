//=================================================================================================
//	FILE		: TutorialDlg.cpp
//	PURPOSE		: Implemention part of tutorial dialog.
//	PROGRAMMER	: Yongs Lee
//	DATE		: October 16, 2007
//	ADDITION	:
//	ATTENTION	:
//=================================================================================================





//=========================================================================
//		INCLUDE HEADER FILES
//=========================================================================
#include "stdafx.h"															// 표준 헤더파일을 포함한다.
#include "../WindowIDEnum.h"												// 윈도우 아이디 정의 헤더를 포함한다.

#include "./Interface/cButton.h"											// 버튼 헤더를 포함한다.
#include "./interface/cFont.h"												// 폰트 헤더를 포함한다.
#include "./Interface\cListDialog.h"										// 리스트 다이얼로그 헤더를 포함한다.
#include "./Interface\cStatic.h"											// 스태틱 헤더를 포함한다.

#include "../[CC]Header/GameResourceManager.h"								// 게임 리소스 매니져 헤더를 포함한다.
#include "../ObjectManager.h"												// 오브젝트 매니져 헤더를 포함한다.
#include "./interface/cWindowManager.h"										// 윈도우 매니져 헤더를 포함한다.
#include "./Interface/cScriptManager.h"										// 스트립트 매니져 헤더를 포함한다.
#include "./Audio/MHAudioManager.h"											// 묵향 오디오 매니져 헤더를 포함한다.
#include "TutorialManager.h"												// 튜토리얼 매니져 헤더를 포함한다.

#include "Tutorial.h"														// 튜토리얼 헤더를 포함한다.
#include "Tutorial_Msg.h"													// 튜토리얼 메시지 헤더를 포함한다.

#include "Tutorialdlg.h"													// 튜토리얼 다이얼로그 헤더를 포함한다.





//=========================================================================
//		튜토리얼 다이얼로그 생성자 함수.
//=========================================================================
cTutorialDlg::cTutorialDlg(void)											// 생성자 함수.
{
	DISPLAY_INFO	dispInfo ;												// 디스플레이 정보를 세팅할 구조체를 선언한다.

	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;								// 디스플레이 정보를 얻어온다.

	m_nDispWidth  = dispInfo.dwWidth ;										// 디스플레이 가로 사이즈를 세팅한다.
	m_nDispHeight = dispInfo.dwHeight ;										// 디스플레이 세로 사이즈를 세팅한다.

	m_nBtnStartXpos = 0 ;													// 버튼 시작 x좌표를 0으로 세팅한다.
	m_nBtnStartYpos = 0 ;													// 버튼 시작 y좌표를 0으로 세팅한다.

	m_vImagePos.x = 0 ;														// 출력 위치 x 를 0으로 세팅한다.
	m_vImagePos.y = 0 ;														// 출력 위치 y 를 0으로 세팅한다.

	m_nImageAlpha = 0 ;														// NPC 이미지의 알파 값을 0으로 세팅한다.

	m_pSpeechList	= NULL ;												// 튜토리얼 대사 리스트 다이얼로그를 null 처리한다.

	m_pOkBtn		= NULL ;												// 확인 버튼 포인터를 null 처리한다.
	m_pPrevBtn		= NULL ;												// 이전 버튼 포인터를 null 처리한다.
	m_pNextBtn		= NULL ;												// 다음 버튼 포인터를 null 처리한다.
	m_pPassBtn		= NULL ;												// 패스 버튼 포인터를 null 처리한다.

	m_pSpeechBack	= NULL ;												// 대사 배경 스태틱 포인터를 null 처리한다.

	m_pTutorial = NULL ;													// 멤버 튜토리얼 포인터를 null 처리를 한다.
	m_pCurSubTutorial = NULL ;												// 멤버 현재 서브 튜토리얼 포인터를 null 처리를 한다.

	m_bShowTutorial = FALSE ;												// 튜토리얼 보여주기를 false로 세팅한다.

	m_nCurSubIndex = 0 ;													// 현재 서브 튜토리얼 인덱스를 0으로 세팅한다.
	m_nTotalSubCount = 0 ;													// 총 서브 튜토리얼 카운트를 0으로 세팅한다.
}





//=========================================================================
//		튜토리얼 다이얼로그 소멸자 함수.
//=========================================================================
cTutorialDlg::~cTutorialDlg(void)											// 소멸자 함수.
{
}





//=========================================================================
//		튜토리얼 다이얼로그 하위 컨트롤 링크 함수.
//=========================================================================
void cTutorialDlg::Linking() 												// 다이얼로그 하위 컨트롤들을 링크하는 함수.
{
	m_pSpeechList = (cListDialog*)GetWindowForID(TTR_SPEECH_LIST) ;			// 대사 리스트 다이얼로그를 링크한다.
	m_pSpeechList->SetStyle(LDS_FONTSHADOW) ;								// 그림자 출력되는 리스트 스타일로 세팅한다.
	m_pSpeechList->Set_ShadowFontIndex(TUTORIAL_FONTIDX) ;					// 출력 폰트를 세팅한다.
	m_pSpeechList->Set_ShadowDistance(2) ;									// 그림자 거리를 2로 세팅한다.

	m_pOkBtn		= (cButton*)GetWindowForID(TTR_OKBTN) ;					// 확인 버튼을 링크한다.
	m_pOkBtn->SetActive(FALSE) ;											// 확인 버튼을 비활성화 한다.

	m_pPrevBtn		= (cButton*)GetWindowForID(TTR_PREVBTN) ;				// 이전 버튼을 링크한다.
	m_pPrevBtn->SetActive(FALSE) ;											// 이전 버튼을 비활성화 한다.

	m_pNextBtn		= (cButton*)GetWindowForID(TTR_NEXTBTN) ;				// 다음 버튼을 링크한다.
	m_pNextBtn->SetActive(FALSE) ;											// 다음 버튼을 비활성화 한다.

	m_pPassBtn		= (cButton*)GetWindowForID(TTR_PASSBTN) ;				// 통과 버튼을 링크한다.
	m_pPassBtn->SetActive(FALSE) ;											// 통과 버튼을 비활성화 한다.

	m_pSpeechBack	= (cStatic*)GetWindowForID(TTR_SP_BACK) ;				// 대사 배경 스태틱을 링크한다.
	m_pSpeechBack->SetActive(FALSE) ;										// 대사 배경 스태틱을 비활성화 한다.
}





//=========================================================================
//		튜토리얼 다이얼로그 렌더 함수.
//=========================================================================
void cTutorialDlg::Render()													// 렌더 함수.
{
	if( !IsActive() ) return ;												// 활성화 상태가 아니면, return 처리를 한다.

	if( !m_pTutorial ) return ;												// 튜토리얼 정보가 유효하지 않으면 return 처리를 한다.

	//m_nImageAlpha += 2 ;													// 알파 값을 2씩 증가한다.

	//if ( m_nImageAlpha >= 255 ) m_nImageAlpha = 255 ;						// 알파 값이 255 이상이면, 255로 고정한다.

	//SetAlpha((BYTE)m_nImageAlpha) ;

	if( m_nImageAlpha < 250 )
	{
		DWORD dwCurTick = GetTickCount() ;

		DWORD dwResultTime = dwCurTick - m_dwStartTime ;

		m_nImageAlpha = (int)(2.5f * (dwResultTime/10)) ;
	}
	else
	{
		m_nImageAlpha = 255 ;
	}

	m_NpcImage.RenderSprite(NULL, NULL, 0, &m_vImagePos, 
		RGBA_MAKE(255,255,255,m_nImageAlpha)) ;								// npc 이미지를 출력한다.

	//int itemCount = m_pSpeechList->GetItemCount() ;

	//ITEM* pItem ;

	//for( int count = 0 ; count < itemCount ; ++count )
	//{
	//	pItem = NULL ;
	//	pItem = m_pSpeechList->GetItem(count) ;

	//	if( !pItem )
	//	{
	//		// 에러 처리 한다.
	//		continue ;
	//	}


	//}

	cDialog::RenderWindow();												// 윈도우를 렌더한다.
	cDialog::RenderComponent();												// 하위 객체들을 렌더한다.
}





//=========================================================================
//		튜토리얼 버튼 이벤트를 처리하는 함수.
//=========================================================================
void cTutorialDlg::OnActionEvent(LONG lId, void* p, DWORD we) 				// 지정 된 컨트롤 이벤트를 처리 할 함수.
{
	//int nCondition1 = m_pCurSubTutorial->nComplete_Condition1 ;				// 첫번째 완료 조건을 받는다.
	//int nCondition2 = m_pCurSubTutorial->nComplete_Condition2 ;				// 두번째 완료 조건을 받는다.

	switch(lId)																// 컨트롤 아이디를 확인한다.
	{
	case TTR_OKBTN :														// 확인 버튼을 눌렀다면,
		{
			m_pTutorial->Set_Complete(TRUE) ;
			UpdateTutorial() ;												// 튜토리얼을 종료하고, 업데이트 한다.
		}
		break ;

	case TTR_PREVBTN :														// 이전 버튼을 눌렀다면,
		{
			if( m_nCurSubIndex <= 0 )
			{
				// 에러처리 한다~~~!!!
				// 0번 서브 퀘에서 이전 버튼이 떠??!!!
				return ;
			}

			SUB_TUTORIAL* pPrevSubTutorial = NULL ;
			pPrevSubTutorial = m_pTutorial->Get_SubTutorial(m_nCurSubIndex-1) ;		// 이전 서브 튜토리얼 정보를 받는다.

			if( pPrevSubTutorial )											// 이전 서브 튜토리얼이 있을경우,
			{
				--m_nCurSubIndex ;											// 현재 튜토리얼 인덱스를 감소한다.
				Prepare_Tutorial() ;										// 튜토리얼 진행을 준비한다.
			}
			else															// 이전 서브 튜토리얼이 없을경우,
			{
				// 에러 처리 해야 한다.
				int a = 0 ;
				return ;
			}
		}
		break ;

	case TTR_NEXTBTN :														// 다음 버튼을 눌렀다면,
		{
			SUB_TUTORIAL* pNextSubTutorial = NULL ;
			pNextSubTutorial = m_pTutorial->Get_SubTutorial(m_nCurSubIndex+1) ;		// 현재 서브 튜토리얼 정보를 받는다.

			if( pNextSubTutorial )											// 다음 서브 튜토리얼이 있을경우,
			{
				++m_nCurSubIndex ;											// 현재 튜토리얼 인덱스를 증가한다.
				Prepare_Tutorial() ;										// 튜토리얼 진행을 준비한다.
			}
			else															// 다음 서브 튜토리얼이 없을경우,
			{
				// 에러 처리 해야 한다.
				int a = 0 ;
				return ;
			}
		}
		break ;

	case TTR_PASSBTN :														// 통과 버튼을 눌렀다면,
		{
			m_pTutorial->Set_Complete(TRUE) ;
			UpdateTutorial() ;												// 튜토리얼을 종료하고, 업데이트 한다.
		}
		break ;
	}
}





//=========================================================================
//		튜토리얼을 시작하는 함수.
//=========================================================================
void cTutorialDlg::Set_TutorialInfo(cTutorial* pTutorial) 					// 튜토리얼 정보를 세팅하는 함수.
{
	if( !pTutorial )
	{
		#ifdef _GMTOOL_														// GM툴일 경우,
		MessageBox(NULL, TDERR_MSG1, "cTutorialDlg::Start_Tutorial", MB_OK);// 인자로 넘어온 튜토리얼 정보 무효~!!
		#endif

		return ;															// return 처리를 한다.
	}

	m_pTutorial = pTutorial ;												// 멤버 튜토리얼 포인터로 튜토리얼 정보를 받는다.
}





//=========================================================================
//		튜토리얼을 시작하는 함수.
//=========================================================================
void cTutorialDlg::Start_Tutorial() 										// 튜토리얼 진행함수.
{
	m_nCurSubIndex = 0 ;													// 현재 서브 튜토리얼 인덱스를 0으로 세팅한다.
	m_nTotalSubCount = m_pTutorial->Get_SubTutorial_Count() ;				// 현재 튜토리얼의 서브 튜토리얼 카운트를 받는다.

	Prepare_Tutorial() ;													// 튜토리얼 진행을 준비한다.

	SetActive(TRUE) ;														// 튜토리얼 다이얼로그를 활성화 한다.

	m_pSpeechBack->SetActive(TRUE) ;										// 대사 배경을 활성화 한다.

	m_nImageAlpha = 0 ;														// npc 이미지의 알파 값을 0으로 세팅한다.

	m_dwStartTime = GetTickCount() ;

	cDialog* npcScriptDialog	= WINDOWMGR->GetWindowForID( NSI_SCRIPTDLG );// npc 대화창의 정보를 받는다.

	if( !npcScriptDialog ) return ;											// npc 대화창 정보가 유효하지 않으면 return 처리를 한다.

	if( npcScriptDialog->IsActive() ) npcScriptDialog->SetActive(FALSE) ;	// npc 대화창이 열려진 상태면, 닫는다.
}





//=========================================================================
//		튜토리얼을 시작하는 함수.
//=========================================================================
//void cTutorialDlg::Start_Tutorial(cTutorial* pTutorial) 					// 튜토리얼 진행함수.
//{
//	if( !pTutorial )
//	{
//		#ifdef _GMTOOL_														// GM툴일 경우,
//		MessageBox(NULL, TDERR_MSG1, "cTutorialDlg::Start_Tutorial", MB_OK);// 인자로 넘어온 튜토리얼 정보 무효~!!
//		#endif
//
//		return ;															// return 처리를 한다.
//	}
//
//	m_pTutorial = pTutorial ;												// 멤버 튜토리얼 포인터로 튜토리얼 정보를 받는다.
//
//	m_nCurSubIndex = 0 ;													// 현재 서브 튜토리얼 인덱스를 0으로 세팅한다.
//	m_nTotalSubCount = pTutorial->Get_SubTutorial_Count() ;					// 현재 튜토리얼의 서브 튜토리얼 카운트를 받는다.
//
//	Prepare_Tutorial() ;													// 튜토리얼 진행을 준비한다.
//
//	SetActive(TRUE) ;														// 튜토리얼 다이얼로그를 활성화 한다.
//
//	m_nImageAlpha = 0 ;														// npc 이미지의 알파 값을 0으로 세팅한다.
//
//	cDialog* npcScriptDialog	= WINDOWMGR->GetWindowForID( NSI_SCRIPTDLG );// npc 대화창의 정보를 받는다.
//
//	if( !npcScriptDialog ) return ;											// npc 대화창 정보가 유효하지 않으면 return 처리를 한다.
//
//	if( npcScriptDialog->IsActive() ) npcScriptDialog->SetActive(FALSE) ;	// npc 대화창이 열려진 상태면, 닫는다.
//}





//=========================================================================
//		튜토리얼 진행을 위한 준비 처리 함수.	
//=========================================================================
void cTutorialDlg::LoadNpcImage() 											// ncp 이미지를 로딩하는 함수.
{
	if( !m_pCurSubTutorial )												// 현재 서브 튜토리얼이 유효하지 않으면,
	{
		#ifdef _GMTOOL_														// GM툴일 경우,
		MessageBox(NULL, TDERR_MSG6, "cTutorialDlg::LoadNpcImage", MB_OK) ;	// 현재 서브 튜토리얼 정보 무효~!!
		#endif
	}

	CMHFile file;															// 묵향 파일을 선언한다.

	file.Init("Data/Interface/Windows/NpcImageList.bin","rb");				// npc 이미지 리스트를 읽기모드로 연다.

	if(file.IsInited() == FALSE)											// 열기에 실패하면,
	{
		#ifdef _GMTOOL_														// GM툴일 경우,
		MessageBox(NULL, TERR_MSG2, "cTutorialDlg::LoadNpcImage", MB_OK) ;	// 에러 메시지를 메시지 박스로 띄운다.
		#endif

		return ;															// return 처리를 한다.
	}

	char buf[256];															// 임시 버퍼를 선언한다.

	WORD wIndex;															// 인덱스를 담을 변수를 선언한다.

	BYTE emotion;															// 모션을 담을 변수를 선언한다.

	memset(&m_NpcImage, 0, sizeof(cImageSelf)) ;							// 현재 이미지를 메모리 셋 한다.

	while(!file.IsEOF())													// while문을 돌린다.
	{
		wIndex  = file.GetWord();											// 인덱스를 읽어들인다.
		emotion = file.GetByte();											// 모션을 읽어들인다.
		char*	szFileName	=	file.GetString();							// 파일명을 세팅한다.

		if( strlen(szFileName) == 0 )										// 파일명이 0과 같다면, while문을 탈출한다.
		{
			#ifdef _GMTOOL_													// GM툴일 경우,
			char tempBuf[128] = {0, } ;										// 에러 출력용 임시 버퍼를 선언한다.
			sprintf(tempBuf, TDERR_MSG7, wIndex, emotion) ;					// npc 이미지 파일명 받기 실패~!!
			MessageBox(NULL, tempBuf, "cTutorialDlg::LoadNpcImage", MB_OK) ;// 에러 메시지를 메시지 박스로 띄운다.
			#endif

			return ;														// return 처리를 한다.
		}

		if( wIndex != m_pCurSubTutorial->nNpc_Idx ) continue ;				// npc 인덱스가 같지 않으면 continue 처리를 한다.

		if( emotion != m_pCurSubTutorial->nFace_Idx ) continue ;			// emotion 인덱스가 같지 않으면, continue 처리를 한다.

		sprintf( buf, "%s%s", NPC_IMAGE_PATH, szFileName);					// 임시 버퍼에 경로및, 파일명을 세팅한다.
			
		if( !m_NpcImage.LoadSprite(buf) )									// npc 이미지 로드에 실패 했다면,
		{
			#ifdef _GMTOOL_													// GM툴일 경우,
			char tempBuf[128] = {0, } ;										// 에러 출력용 임시 버퍼를 선언한다.
			sprintf(tempBuf, TDERR_MSG8, wIndex, emotion) ;					// npc 스프라이트 이미지 로드 실패~!!
			MessageBox(NULL, tempBuf, "cTutorialDlg::LoadNpcImage", MB_OK) ;// 에러 메시지를 메시지 박스로 띄운다.
			#endif
		}

		int nWidth = m_NpcImage.GetImageRect()->right ;						// 이미지의 가로 길이를 받는다.
		int nHeight = m_NpcImage.GetImageRect()->bottom ;					// 이미지의 세로 길이를 받는다.

		m_vImagePos.x = (float)(m_nDispWidth - nWidth) ;					// 출력 위치 X좌표를 세팅한다.
		m_vImagePos.y = (float)(m_nDispHeight - nHeight) ;					// 출력 위치 Y좌표를 세팅한다.

		return ;															// return 처리를 한다.
	}
}





//=========================================================================
//		튜토리얼 진행을 위한 준비 처리 함수.	
//=========================================================================
void cTutorialDlg::Prepare_Tutorial() 										// 튜토리얼 진행을 미리 준비하는 함수.
{
	m_pCurSubTutorial = NULL ;												// 현재 서브 튜토리얼을 받을 포인터를 null 처리를 한다.
	m_pCurSubTutorial = m_pTutorial->Get_SubTutorial(m_nCurSubIndex) ;		// 현재 서브 튜토리얼 정보를 받는다.

	if( !m_pTutorial )														// 튜토리얼 정보가 유효하지 않으면,
	{
		#ifdef _GMTOOL_														// GM툴일 경우,
		char tempBuf[128] = {0, } ;											// 에러 출력용 임시 버퍼를 선언한다.
		MessageBox(NULL, TDERR_MSG1, "cTutorialDlg::Prepare_Tutorial", MB_OK);	// 에러 메시지를 메시지 박스로 띄운다.
		#endif

		return ;															// return 처리를 한다.
	}

	if( !m_pCurSubTutorial )												// 서브 튜토리얼 정보를 받는데 실패 했다면,
	{
		#ifdef _GMTOOL_														// GM툴일 경우,
		char tempBuf[128] = {0, } ;											// 에러 출력용 임시 버퍼를 선언한다.
		sprintf(tempBuf, TDERR_MSG2, m_pTutorial->Get_Index()) ;			// 서브 튜토리얼 받기 실패~!!
		MessageBox(NULL, tempBuf, "cTutorialDlg::Prepare_Tutorial", MB_OK);	// 에러 메시지를 메시지 박스로 띄운다.
		#endif

		return ;															// return 처리를 한다.
	}

	int nMsgIndex = m_pCurSubTutorial->nSpeech_Idx ;						// 현재 서브 튜토리얼의 메시지 번호를 받는다.
	cTutorial_Msg* pMsg = TUTORIALMGR->Get_TutorialMsg( nMsgIndex ) ;		// 현재 서브 튜토리얼의 메시지를 받는다.

	if( !pMsg )																// 메시지를 받는데 실패했다면,
	{
		#ifdef _GMTOOL_														// GM툴일 경우,
		char tempBuf[128] = {0, } ;											// 에러 출력용 임시 버퍼를 선언한다.
		sprintf(tempBuf, TDERR_MSG3, nMsgIndex) ;							// 튜토리얼 메시지 받기 실패~!!
		MessageBox(NULL, tempBuf, "cTutorialDlg::Prepare_Tutorial", MB_OK);	// 에러 메시지를 메시지 박스로 띄운다.
		#endif

		return ;															// return 처리를 한다.
	}

	m_pSpeechList->RemoveAll() ;											// 대사 출력 리스트를 모두 비운다.

	//ResetSpeechMsgList() ;													// 튜토리얼 대사 리스트를 초기화 한다.

	int nMsgCount = 0 ;														// 메시지 카운트를 받을 변수를 선언하고 0으로 초기화 한다.
	nMsgCount = pMsg->Get_LineCount() ;										// 메시기 카운트를 받는다.

	if( nMsgCount < 0 )
	{
		#ifdef _GMTOOL_														// GM툴일 경우,
		char tempBuf[128] = {0, } ;											// 에러 출력용 임시 버퍼를 선언한다.
		sprintf(tempBuf, TDERR_MSG4, nMsgIndex) ;							// 메시지의 내용이 없다~!!
		MessageBox(NULL, tempBuf, "cTutorialDlg::Prepare_Tutorial", MB_OK);	// 에러 메시지를 메시지 박스로 띄운다.
		#endif

		return ;															// return 처리를 한다.
	}

	MSG_LINE* pMsgLine ;													// 메시지 라인을 받을 포인터를 선언한다.
	for( int count = 0 ; count < nMsgCount ; ++count )						// 메시지 라인 수 만큼 for문을 돌린다.
	{
		pMsgLine = NULL ;													// 메시지 라인 포인터를 null 처리를 한다.
		pMsgLine = pMsg->Get_MsgLine(count) ;								// 카운트에 해당하는 메시지 라인을 받는다.

		if( !pMsgLine )
		{
			#ifdef _GMTOOL_													// GM툴일 경우,
			char tempBuf[128] = {0, } ;										// 에러 출력용 임시 버퍼를 선언한다.
			sprintf(tempBuf, TDERR_MSG5, nMsgIndex, count) ;				// 메시지 라인 받기 실패~!!
			MessageBox(NULL, tempBuf, "cTutorialDlg::Prepare_Tutorial", MB_OK);// 에러 메시지를 메시지 박스로 띄운다.
			#endif

			continue ;														// continue 처리를 한다.
		}

		m_pSpeechList->AddItem(pMsgLine->msg, RGB(255, 255, 255), count) ;	// 튜토리얼 대사를 추가한다.

        if( count != (nMsgCount - 1) ) continue ;							// 현재 카운트가, 마지막 카운트가 아니면 continue 처리를 한다.

		m_nBtnStartXpos = int(m_pSpeechList->GetAbsX() + 
						  CFONT_OBJ->GetTextWidth(TUTORIAL_FONTIDX) + 
					      CFONT_OBJ->GetTextExtentWidth( TUTORIAL_FONTIDX, 
						  pMsgLine->msg, strlen(pMsgLine->msg)) ) ;			// 버튼 출력의 시작 x좌표를 구한다.

		m_nBtnStartYpos = int(m_pSpeechList->GetAbsY() + 6 + 
						  (CFONT_OBJ->GetTextHeight(TUTORIAL_FONTIDX) *
						  (nMsgCount-1)));									// 버튼 출력의 시작 y좌표를 구한다.
	}

	DeActiveAllBtn() ;														// 모든 버튼들을 비활성화 한다.

	int nCompleteCondition = 0 ;											// 첫 번째 완료 조건을 받을 변수를 선언한다.
	nCompleteCondition = m_pCurSubTutorial->nComplete_Condition1 ;			// 첫 번재 완료 조건을 받는다.

	switch(nCompleteCondition)												// 완료 조건을 확인한다.
	{
	case e_CLICK_OKBTN :	ActiveBtns() ; break ;							// 버튼 클릭으로 완료하는 상황이면, 상황에 맞는 버튼을 활성화 시킨다.

	case e_NPC_TALK :break ;												// npc와 대화를 해서 완료해야 하는 상황이면,

	case e_OPEN_DIALOG :													// 창을 열어 완료해야 하는 상황이면,
		{
			cWindow* pWindow = WINDOWMGR->GetWindowForID(IDSEARCH(m_pCurSubTutorial->szCompleteCondition2)) ;

			if( pWindow )
			{
				pWindow->SetActive(FALSE) ;
			}
		}
		break ;											
	}


	LoadNpcImage() ;														// 튜토리얼 npc 이미지를 로딩한다.

	AUDIOMGR->StopAll() ;													// play 중이던 사운드를 끈다.

	int nSoundIdx = m_pCurSubTutorial->nSound_Idx ;							// 현재 서브 튜토리얼에 지정된 사운드 인덱스를 받는다.

	if( nSoundIdx < 0 )														// 사운드 인덱스가 0이하이면,
	{
		#ifdef _GMTOOL_														// GM툴일 경우,
		char tempBuf[128] = {0, } ;											// 에러 출력용 임시 버퍼를 선언한다.
		sprintf(tempBuf, TDERR_MSG10, m_pTutorial->Get_Index(), 
			m_pCurSubTutorial->nIndex, m_pCurSubTutorial->nSound_Idx) ;		// npc 이미지 파일명 받기 실패~!!
		MessageBox(NULL, tempBuf, "cTutorialDlg::Prepare_Tutorial", MB_OK) ;// 에러 메시지를 메시지 박스로 띄운다.
		#endif

		return ;															// return 처리를 한다.
	}

	AUDIOMGR->Play(nSoundIdx, HERO);										// 현재 서브 튜토리얼의 사운드를 play 한다.
}





//=========================================================================
//		튜토리얼 대사 리스트를 초기화 하는 함수.
//=========================================================================
void cTutorialDlg::ResetSpeechMsgList() 									// 대사 리스트를 다시 세팅하는 함수.
{
	MSG_LINE* pMsgLine ;													// 메시지 라인을 받을 포인터를 선언한다.
	
	PTRLISTPOS pos = NULL ;													// 위치 정보를 받을 포인터를 선언하고 null 처리를 한다.
	pos = m_SpeechMsgList.GetHeadPosition() ;								// 튜토리얼 진행 대사 리스트를 헤드로 세팅하고 위치 정보를 받는다.

	while(pos)																// 위치 정보가 유효할 동안 while문을 돌린다.
	{
		pMsgLine = NULL ;													// 메시지 라인 포인터를 null 처리를 한다.
		pMsgLine = (MSG_LINE*)m_SpeechMsgList.GetNext(pos) ;				// 메시지 라인 정보를 받고, 위치를 다음으로 세팅한다.

		if( !pMsgLine ) continue ;											// 메시지 라인 정보가 없으면, continue 처리를 한다.

		delete pMsgLine ;													// 메시지 라인을 삭제한다.
	}

	m_SpeechMsgList.RemoveAll() ;											// 튜토리얼 진행 대사 리스트를 모두 비운다.
}





//=========================================================================
//		모든 버튼을 비활성화 하는 버튼.
//=========================================================================
void cTutorialDlg::DeActiveAllBtn() 										// 모든 버튼을 비활성화 하는 함수.
{
	m_pOkBtn->SetActive(FALSE) ;											// 확인 버튼을 비활성화 한다.
	m_pPrevBtn->SetActive(FALSE) ;											// 이전 버튼을 비활성화 한다.
	m_pNextBtn->SetActive(FALSE) ;											// 다음 버튼을 비활성화 한다.
	m_pPassBtn->SetActive(FALSE) ;											// 통과 버튼을 비활성화 한다.
}





//=========================================================================
//		상황에 맞는 버튼을 활성화 하는 함수.
//=========================================================================
void cTutorialDlg::ActiveBtns() 											// 상황에 맞는 버튼을 활성화 하는 함수.
{
	int nCurSubIndex = m_pCurSubTutorial->nIndex ;							// 현재 서브 튜토리얼의 인덱스를 구한다.
	int nLastSubIndex = m_nTotalSubCount-1 ;								// 마지막 서브 튜토리얼의 인덱스를 구한다.

	int nBtnDistance = 5 ;													// 버튼 사이 거리 값을 5로 세팅한다.

	if( nCurSubIndex == 0 )													// 현재 서브 튜토리얼 인덱스가 0과 같으면,
	{
		if( nCurSubIndex == nLastSubIndex )									// 처음이자 마지막이면,
		{
			m_pOkBtn->SetAbsXY(m_nBtnStartXpos, m_nBtnStartYpos) ;			// ok 버튼의 위치를 잡고,
			m_pOkBtn->SetActive(TRUE) ;										// 활성화 시킨다.
		}
		else																// 다수 페이지 중 처음이면,
		{
			m_pNextBtn->SetAbsXY(m_nBtnStartXpos, m_nBtnStartYpos) ;		// ok 버튼의 위치를 잡고,
			m_pNextBtn->SetActive(TRUE) ;									// 활성화 시킨다.

			int nSecontX = m_nBtnStartXpos + m_pNextBtn->GetWidth() + nBtnDistance ;	// 다음 버튼의 시작 좌표를 구한다.

			m_pPassBtn->SetAbsXY(nSecontX, m_nBtnStartYpos) ;				// 다음 버튼의 위치를 잡고,
			m_pPassBtn->SetActive(TRUE) ;									// 활성화 시킨다.
		}
	}
	else if( nCurSubIndex == nLastSubIndex )								// 현재 서브 튜토리얼이 마지막 튜토리얼이면,
	{
		if(m_nTotalSubCount > 1)											// 서브 튜토리얼 총 페이지가 1페이지 이상이면,
		{
			m_pPrevBtn->SetAbsXY(m_nBtnStartXpos, m_nBtnStartYpos) ;		// 이전 버튼의 위치를 잡고,
			m_pPrevBtn->SetActive(TRUE) ;									// 활성화 시킨다.

			int nSecontX = m_nBtnStartXpos + m_pPrevBtn->GetWidth() + nBtnDistance ;	// 확인 버튼의 좌표를 구한다.

			m_pOkBtn->SetAbsXY(nSecontX, m_nBtnStartYpos) ;					// 확인 버튼의 위치를 잡고,
			m_pOkBtn->SetActive(TRUE) ;										// 활성화 시킨다.
		}
		else																// 현재 서브 튜토리얼 페이지가 마지막 페이지라면,
		{
			m_pOkBtn->SetAbsXY(m_nBtnStartXpos, m_nBtnStartYpos) ;			// 확인 버튼의 위치를 잡고,
			m_pOkBtn->SetActive(TRUE) ;										// 활성화 시킨다.
		}
	}
	else																	// 첫페이지나 마지막 페이지가 아니면,
	{		
		m_pPrevBtn->SetAbsXY(m_nBtnStartXpos, m_nBtnStartYpos) ;			// 이전 버튼의 위치를 잡는다.
		m_pPrevBtn->SetActive(TRUE) ;										// 활성화 시킨다.
	
		int nSecontX = m_nBtnStartXpos + m_pPrevBtn->GetWidth() + nBtnDistance ;	// 다음 버튼의 시작 좌표를 구한다.

		m_pNextBtn->SetAbsXY(nSecontX, m_nBtnStartYpos) ;					// 다음 버튼의 위치를 잡고,
		m_pNextBtn->SetActive(TRUE) ;										// 활성화 시킨다.

		int nThirdX = nSecontX + m_pNextBtn->GetWidth() + nBtnDistance ;	// 통과 버튼의 시작 좌표를 구한다.

		m_pPassBtn->SetAbsXY(nThirdX, m_nBtnStartYpos) ;					// 통과 버튼의 위치를 잡고,
		m_pPassBtn->SetActive(TRUE) ;										// 활성화 시킨다.
	}
}





//=========================================================================
//		튜토리얼 종료 조건과 같은 npc 인지 체크하는 함수.
//=========================================================================
BOOL cTutorialDlg::IsSameEndNpc(DWORD dwNpcIdx) 							// 튜토리얼 종료 조건과 같은 npc 인지 체크하는 함수.
{
	if( !m_pCurSubTutorial ) return FALSE ;

	int nEndCondition = 0 ;													// 첫 번째 완료 조건을 받을 변수를 선언한다.
	nEndCondition = m_pCurSubTutorial->nComplete_Condition1 ;				// 첫 번재 완료 조건을 받는다.

	if( nEndCondition != e_NPC_TALK )										// 완료 조건 첫번째가 npc와 대화가 아니면,
	{
		//#ifdef _GMTOOL_														// GM툴일 경우,
		//char tempBuf[64] = {0, } ;											// 에러 출력용 임시 버퍼를 선언한다.
		//sprintf(tempBuf, TDERR_MSG9, dwNpcIdx) ;							// 메시지 라인 받기 실패~!!
		//MessageBox(NULL, tempBuf, "cTutorialDlg::IsSameEndNpc", MB_OK);		// 에러 메시지를 메시지 박스로 띄운다.
		//#endif

		return FALSE ;														// false return 처리를 한다.
	}

	int nEndCondition2 = 0 ;												// 두 번째 완료 조건을 받을 변수를 선언한다.
	nEndCondition2 = m_pCurSubTutorial->nComplete_Condition2 ;				// 두 번재 완료 조건을 받는다.

	if( dwNpcIdx != nEndCondition2 )										// 완료 조건의 npc와 인자로 넘어온 npc 인덱스가 다르면,
	{
		return FALSE ;														// false return 처리를 한다.
	}

	return TRUE ;															// true return 처리를 한다.
}





//=========================================================================
//		이전 서브 튜토리얼이 있는지 여부를 체크하는 함수.
//=========================================================================
BOOL cTutorialDlg::IsHavePrevSubTutorial() 									// 이전 서브 튜토리얼이 있는지 여부를 체크하는 함수.
{
	if( m_nCurSubIndex == 0 ) return FALSE ;								// 현재 서브 튜토리얼 인덱스가 0과 같으면 false return 처리를 한다.

	SUB_TUTORIAL* pPrevSubTutorial = NULL ;									// 서브 튜토리얼 정보를 받을 포인터를 선언하고 null 처리를 한다.
	pPrevSubTutorial = m_pTutorial->Get_SubTutorial(m_nCurSubIndex-1) ;		// 이전 서브 튜토리얼 정보를 받는다.

	if( pPrevSubTutorial )													// 이전 서브 튜토리얼이 있을경우,
	{
		return TRUE ;														// true return 처리를 한다.
	}

	return FALSE ;															// false return 처리를 한다.
}





//=========================================================================
//		다음 서브 튜토리얼이 있는지 여부를 체크하는 함수.
//=========================================================================
BOOL cTutorialDlg::IsHaveNextSubTutorial()									// 다음 서브 튜토리얼이 있는지 여부를 체크하는 함수.
{
	SUB_TUTORIAL* pNextSubTutorial = NULL ;									// 다음 서브 튜토리얼 정보를 받을 포인터를 선언하고 null 처리를 한다.
	pNextSubTutorial = m_pTutorial->Get_SubTutorial(m_nCurSubIndex+1) ;		// 현재 서브 튜토리얼 정보를 받는다.

	if( pNextSubTutorial ) return TRUE ;									// 다음 서브 튜토리얼 정보가 있으면 ture return 처리를 한다.

	return FALSE ;															// false return 처리를 한다.
}





//=========================================================================
//		다음 서브 튜토리얼을 진행하는 함수.
//=========================================================================
void cTutorialDlg::StartNextSubTutorial() 									// 다음 서브 튜토리얼을 진행하는 함수.
{
	SUB_TUTORIAL* pNextSubTutorial = NULL ;									// 다음 서브 튜토리얼 정보를 받을 포인터를 선언하고 null 처리를 한다.
	pNextSubTutorial = m_pTutorial->Get_SubTutorial(m_nCurSubIndex+1) ;		// 현재 서브 튜토리얼 정보를 받는다.

	if( pNextSubTutorial )													// 다음 서브 튜토리얼이 있을경우,
	{
		++m_nCurSubIndex ;													// 현재 튜토리얼 인덱스를 증가한다.
		Prepare_Tutorial() ;												// 튜토리얼 진행을 준비한다.
	}
}





//=========================================================================
//		다음 서브 튜토리얼을 진행하는 함수.
//=========================================================================
void cTutorialDlg::UpdateTutorial() 										// 튜토리얼을 완료하고 업데이트 하는 함수.
{
	m_pSpeechBack->SetActive(FALSE) ;										// 대사 배경을 비활성화 한다.

	SetActive(FALSE) ;														// 튜토리얼 창을 비활성화 한다.

//	m_pTutorial->Set_Complete(TRUE) ;										// 튜토리얼의 완료를 true로 세팅한다.
	TUTORIALMGR->UpdateTutorial(m_pTutorial->Get_Index()) ;					// 현재 튜토리얼을 업데이트 한다.

	//m_pTutorial = NULL ;
	//m_pCurSubTutorial = NULL ;
}





//=========================================================================
//		현재 서브 튜토리얼을 반환하는 함수.
//=========================================================================
SUB_TUTORIAL* cTutorialDlg::GetCurSubTutorial()								// 현재 서브 튜토리얼을 반환하는 함수.
{
	if( !m_pCurSubTutorial ) return NULL ;									// 현재 서브 튜토리얼이 유효하지 않으면, null return 처리를 한다.

	return m_pCurSubTutorial ;												// 현재 서브 튜토리얼을 return 한다.
}





