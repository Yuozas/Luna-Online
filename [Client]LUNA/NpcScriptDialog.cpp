

#include "stdafx.h"																		// 표준 시스템과, 프로젝트가 지정한, 자주쓰는 해더들을 모은 해더파일을 불러온다.

#include "WindowIDEnum.h"																// 윈도우 아이디 이넘 헤더를 불러온다.

#include "GameIn.h"																		// 게임 인 클래스 헤더를 불러온다.
#include "Quest.h"																		// 퀘스트 클래스 헤더를 불러온다.

#include "ObjectManager.h"																// 오브젝트 매니져 헤더를 불러온다.
#include "ObjectStateManager.h"															// 오브젝트 상태 매니져 헤더를 불러온다.
#include "ChatManager.h"																// 채팅 매니져 헤더를 불러온다.
#include "QuestManager.h"																// 퀘스트 매니져 헤더를 불러온다.
#include "NpcScriptManager.h"															// NPC 스크립트 매니져 헤더를 불러온다.

#include "./Interface/cWindowManager.h"													// 윈도우 매니져 헤더를 불러온다.

#include "./Interface/cStatic.h"														// 스태틱 클래스 헤더를 불러온다.
#include "./Input/UserInput.h"															// 유저 인풋 클래스 헤더를 불러온다.

#include "cMsgBox.h"																// 메시지 박스 클래스 헤더를 불러온다.
#include "cDialogueList.h"																// 다이얼로그 리스트 클래스 헤더를 불러온다.
#include "cHyperTextList.h"																// 하이퍼 텍스트 리스트 클래스 헤더를 불러온다.

#include "NpcImageDlg.h"																// NPC 이미지 다이얼로그 클래스 헤더를 불러온다.
#include "NpcScriptDialog.h"															// NPC 스크립트 다이얼로그 클래스 헤더를 불러온다.

#include "QuestDialog.h"
#include "./Interface/GameNotifyManager.h"
#include "FishingPointDialog.h"

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.09.13
#include "../hseos/Farm/SHFarmManager.h"
// E 농장시스템 추가 added by hseos 2007.09.13

cNpcScriptDialog::cNpcScriptDialog()													// 생성자 함수.
{
	m_type = WT_NPCSCRIPDIALOG;															// 윈도우 타입을 NPC 스크립트 다이얼로그로 세팅한다.

	m_nHyperCount = 0;																	// 하이퍼 카운트를 0으로 세팅한다.

	m_nConversationHyperCount = 0 ;														// 대화창의 하이퍼 카운트를 0으로 세팅한다.

	m_dwCurNpc = 0;																		// 현재 NPC 아이디를 0으로 세팅한다.
	m_wNpcChxNum = 0 ;																	// 현재 NPC CHX 번호를 0으로 세팅한다.

	m_pListDlg = NULL;																	// 대사를 보여주는 리스트 포인터를 NULL 처리한다.

	m_pLinkListDlg = NULL ;																// 링크를 보여주는 리스트 포인터를 NULL 처리한다.

	m_pTopBtn = NULL ;																	// [ 처음화면 ] 버튼을 NULL 처리한다.
	m_pEndBtn = NULL ;																	// [ 대화종료 ] 버튼을 NULL 처리한다.

	m_pTitleText = NULL;																// 타이틀 텍스트 포인터도 NULL 처리 한다.

	for(int i=0 ; i< MAX_REGIST_HYPERLINK;++i)											// 최대 등록 하이퍼 링크 수 만큼 for문을 돌린다.
	{
		m_sHyper[i].Init();																// 카운트에 따른 하이퍼 링크 정보를 초기화 한다.
	}

	for(int i=0 ; i< MAX_REGIST_HYPERLINK;++i)											// 최대 등록 하이퍼 링크 수 만큼 for문을 돌린다.
	{
		m_sConversationHyper[i].Init() ;												// 카운트에 따른 하이퍼 링크 정보를 초기화 한다.
	}

	m_dwQuestIdx = 0;																	// 퀘스트 인덱스를 0으로 세팅한다.

	m_dwMainLinkPageId = 0 ;
}

cNpcScriptDialog::~cNpcScriptDialog()													// 소멸자 함수.
{
	m_pListDlg->RemoveAll() ;

	/*LINKITEM* pDeleteItem ;

	PTRLISTPOS pos = NULL ;
	pos = m_pConversationList.GetHeadPosition() ;

	while(pos)
	{
		pDeleteItem = NULL ;
		pDeleteItem = (LINKITEM*)m_pConversationList.GetNext(pos) ;

		if(pDeleteItem)
		{
			m_pConversationList.Remove(pDeleteItem) ;
		}
	}

	m_pConversationList.RemoveAll() ;*/

	m_pLinkListDlg->RemoveAll() ;

	/*LINKITEM* pLinkedDeleteItem ;

	PTRLISTPOS pDeletePos = NULL ;
	pDeletePos = m_pLinkedList.GetHeadPosition() ;

	while(pDeletePos)
	{
		pLinkedDeleteItem = NULL ;
		pLinkedDeleteItem = (LINKITEM*)m_pLinkedList.GetNext(pDeletePos) ;

		if( pLinkedDeleteItem )
		{
			m_pLinkedList.Remove(pLinkedDeleteItem) ;
		}
	}

	m_pLinkedList.RemoveAll() ;*/

	///*m_pListDlg->RemoveAll() ;

	//m_pLinkListDlg->RemoveAll() ;*/

	//LINKITEM* pLinkItem ;

	//PTRLISTPOS pos = NULL ;
	//pos = m_pConversationList.GetHeadPosition() ;

	//while(pos)
	//{
	//	pLinkItem = NULL ;
	//	pLinkItem = (LINKITEM*)m_pConversationList.GetNext(pos) ;

	//	if( pLinkItem )
	//	{
	//		m_pConversationList.Remove(pLinkItem) ;

	//		delete pLinkItem ;
	//		pLinkItem = NULL ;
	//	}
	//}

	//m_pConversationList.RemoveAll() ;

	/*cPtrList* pList = m_pListDlg->GetListItem() ;

	if( pList )
	{
		ITEM* pItem ;

		PTRLISTPOS pos = NULL ;
		pos = pList->GetHeadPosition() ;

		while(pos)
		{
			pItem = NULL ;
			pItem = (ITEM*)pList->GetNext(pos) ;

			if( pItem )
			{
				pList->Remove(pItem) ;

				delete pItem ;
			}
		}

		pList->RemoveAll() ;
	}*/
}

void cNpcScriptDialog::Linking()														// 링크 함수.
{
	m_pListDlg = (cListDialogEx*)this->GetWindowForID( NSI_LISTDLG );					// 대사 리스트 정보를 받아온다.

	m_pLinkListDlg = (cListDialogEx*)this->GetWindowForID(NSI_LISTDLG2) ;				// NPC와 관련된 링크를 보여주는 리스트 정보를 받아온다.

	m_pTopBtn = (cButton*)this->GetWindowForID(NSI_TOPBTN) ;							// [ 처음화면 ] 버튼 정보를 받아온다.
	m_pEndBtn = (cButton*)this->GetWindowForID(NSI_ENDBTN) ;							// [ 대화종료 ] 버튼 정보를 받아온다.

	m_pTitleText = new cStatic;															// 타이틀 텍스트만큼 메모리를 할당한다.

	if( m_pTitleText )																	// 메모리 할당 정보가 유효하다면,
	{
		m_pTitleText->Init( 13, 10, 0, 0, NULL, -1 );									// 타이틀 텍스트를 초기화 한다.

		m_pTitleText->SetShadow( TRUE );												// 그림자 여부를 TRUE로 세팅한다.

		m_pTitleText->SetFontIdx( 2 );													// 2번 폰트로 세팅한다.

		m_pTitleText->SetAlign( TXT_LEFT );												// 텍스트 정렬을 왼쪽 정렬로 세팅한다.
		
		m_pTitleText->SetStaticText( "" );												// 텍스트를 ""로 세팅한다.
		
		Add( m_pTitleText );															// 다이얼로그에 타이틀을 추가한다.
	}
}

void cNpcScriptDialog::OnActionEvent(LONG lId, void* p, DWORD we)
{
	switch( lId )
	{
	case NSI_TOPBTN :
		{
			OpenDialog(m_dwCurNpc, m_wNpcChxNum) ;
		}
		break ;

	case NSI_ENDBTN :
		{
			EndDialog() ;
			SetActive(FALSE) ;
		}
		break ;
	}
}

void cNpcScriptDialog::ResetConversationList(cDialogueList* pList, DWORD dwMsg)			// 대화창을 다시 세팅 하는 함수.
{
	if( !pList )
	{
		return ;
	}

	m_pListDlg->RemoveAll();															// 대사 리스트를 모두 비운다.

	//LINKITEM* pDeleteItem ;

	//PTRLISTPOS pos = NULL ;
	//pos = m_pConversationList.GetHeadPosition() ;

	//while(pos)
	//{
	//	pDeleteItem = NULL ;
	//	pDeleteItem = (LINKITEM*)m_pConversationList.GetNext(pos) ;

	//	if(pDeleteItem)
	//	{
	//		m_pConversationList.Remove(pDeleteItem) ;
	//		//delete pDeleteItem ;
	//	}
	//}

	//m_pConversationList.RemoveAll() ;

	WORD wIdx = 0;																		// 인덱스를 담을 변수를 선언하고 0으로 세팅한다.

	WORD LineInfo = 0;																	// 라인 정보를 받을 변수를 선언하고 0으로 세팅한다.

	DIALOGUE* temp = NULL;																// 다이얼로그 정보를 담을 포인터 변수를 선언하고 NULL처리를 한다.

	LINKITEM* pItem = NULL;																// 링크 아이템 정보를 담을 포인터 변수를 선언하고 NULL 처리를 한다.
	LINKITEM* Prev = NULL;																// 이전 링크 아이템 정보를 담을 포인터 변수를 선언하고 NULL 처리를 한다.

	while(1)																			// while문을 돌린다.
	{
		temp = pList->GetDialogue( dwMsg, wIdx );										// 랜덤하게 뽑은 페이지로 다이얼로그 정보를 받는다.

		if( temp == NULL ) break;														// 정보가 유효하지 않으면, while문을 빠져나간다.

		pItem = new LINKITEM;															// 아이템 메모리를 할당하여 포인터로 넘긴다.

		strcpy(pItem->string, temp->str);												// 임시 정보의 스트링을 아이템 스트링에 복사를 한다.

		pItem->rgb = temp->dwColor;														// 임시 정보의 색상을 아이템 색상으로 세팅한다.

		//m_pConversationList.AddTail(pItem) ;

		if( Prev == NULL )																// 이전 아이템 정보가 없다면,
		{
			m_pListDlg->cListItem::AddItem(pItem);										// 아이템 정보를 대사 리스트에 추가한다.

			LineInfo = temp->wLine;														// 라인정보는 임시 정보의 라인으로 세팅한다.
		}
		else																			// 이전 아이템 정보가 있다면,
		{
			if( LineInfo == temp->wLine )												// 라인 정보가 임시 라인과 같으면,
			{
				Prev->NextItem = pItem;													// 이전 아이템의 다음 아이템으로 현재 아이템을 세팅한다.
			}
			else																		// 라인 정보가 임시 라인과 같지 않으면,
			{
				m_pListDlg->cListItem::AddItem(pItem);									// 대사 리스트 다이얼로그에 아이템을 추가한다.

				LineInfo = temp->wLine;													// 라인 정보는 임시 라인으로 세팅한다.
			}
		}
		
		Prev = pItem;																	// 이전 아이템 정보에 현재 아이템 정보를 세팅한다.

		++wIdx;																			// 인덱스 정보를 증가한다.
	}

	m_pListDlg->ResetGuageBarPos() ;													// 대화 리스트의 게이지바 위치를 다시 잡는다.
}

void cNpcScriptDialog::ResetMainLinkList(cPage* pMainPage, cHyperTextList* pHyper, cListDialogEx* pListDlg)	// 링크 페이지를 다시 세팅하는 함수.
{
	for(int i=0 ; i< MAX_REGIST_HYPERLINK;++i)											// 최대 등록 가능 하이퍼 링크 수 만큼 for문을 돌린다.
	{
		m_sHyper[i].Init();																// 카운트에 해당하는 하이퍼 링크 정보를 초기화 한다.
	}

	m_pLinkListDlg->RemoveAll() ;														// 링크 리스트를 모두 비운다.

	/*LINKITEM* pDeleteItem ;

	PTRLISTPOS pDeletePos = NULL ;
	pDeletePos = m_pLinkedList.GetHeadPosition() ;

	while(pDeletePos)
	{
		pDeleteItem = NULL ;
		pDeleteItem = (LINKITEM*)m_pLinkedList.GetNext(pDeletePos) ;

		if( pDeleteItem )
		{
			m_pLinkedList.Remove(pDeleteItem) ;
		}
	}

	m_pLinkedList.RemoveAll() ;*/

	m_nHyperCount = 0;																	// 하이퍼 링크 카운트를 0으로 세팅한다.

	DIALOGUE* temp = NULL;																// 다이얼로그 정보를 담을 포인터 변수를 선언하고 NULL처리를 한다.

	LINKITEM* pItem = NULL;																// 링크 아이템 정보를 담을 포인터 변수를 선언하고 NULL 처리를 한다.

	HYPERLINK* pLink;																	// 링크 정보를 담을 포인터를 선언하고 NULL처리를 한다.

	int nLinkCount = pMainPage->GetHyperLinkCount();									// 페이지의 링크 카운트를 받는다.

	// 협행 보일지 말지 결정
	BOOL bQuest = QUESTMGR->IsNpcRelationQuest( m_dwCurNpc );							// 퀘스트 여부를 받는다.

	for( int j = 0 ; j < nLinkCount ; ++j )												// 링크 카운트 만큼 for문을 돌린다.
	{
		pLink = pMainPage->GetHyperText(j);												// 카운트에 해당하는 링크 정보를 받는다.

		if( pLink == NULL )																// 링크 정보가 유효하지 않으면 
		{
			break;																		// for문을 탈출한다.
		}

		if( pLink->wLinkType == emLink_Quest && !bQuest )								// 링크 타입이 퀘스트와 같고, bQuest가 FALSE와 같으면,
		{
			continue;																	// 계속한다.
		}

		temp = pHyper->GetHyperText( pLink->wLinkId );									// 링크 아이디로 다이얼로그 정보를 받는다.

		if( temp )																		// 다이얼로그 정보가 유효하면,
		{
			LINKITEM* pItem = new LINKITEM;												// 링크 아이템 메모리를 할당한다.

			strcpy(pItem->string, temp->str);											// 아이템 스트링을 다이얼로그 스트링으로 세팅한다.

			pItem->dwType = pLink->wLinkType;											// 아이템 링크 타입을 세팅한다.
			
			pListDlg->cListItem::AddItem( pItem );										// 링크 리스트 다이얼로그에 아이템을 추가한다.

			m_sHyper[m_nHyperCount].bUse = TRUE;										// 하이퍼 링크 정보의 사용여부를 TRUE로 세팅한다.
			m_sHyper[m_nHyperCount].dwListItemIdx = pListDlg->GetItemCount()-1;			// 하이퍼 링크 정보의 리스트 아이템 인덱스를 세팅한다.
			m_sHyper[m_nHyperCount].sHyper = *pLink;									// 하이퍼 링크 정보의 링크 정보를 세팅한다.

			++m_nHyperCount;															// 하이퍼 카운트를 증가한다.
		}
	}

	pListDlg->ResetGuageBarPos() ;														// 리스트의 게이지바 위치를 다시 잡는다.
}

void cNpcScriptDialog::ResetConversationLinkList(cPage* pMainPage, cHyperTextList* pHyper)
{
	for(int i=0 ; i< MAX_REGIST_HYPERLINK;++i)											// 최대 등록 가능 하이퍼 링크 수 만큼 for문을 돌린다.
	{
		m_sConversationHyper[i].Init();													// 카운트에 해당하는 하이퍼 링크 정보를 초기화 한다.
	}

	m_nConversationHyperCount = 0 ;														// 대화 리스트의 링크 카운트를 0으로 세팅한다.

	DIALOGUE* temp = NULL;																// 다이얼로그 정보를 담을 포인터 변수를 선언하고 NULL처리를 한다.

	LINKITEM* pItem = NULL;																// 링크 아이템 정보를 담을 포인터 변수를 선언하고 NULL 처리를 한다.

	HYPERLINK* pLink;																	// 링크 정보를 담을 포인터를 선언하고 NULL처리를 한다.

	int nLinkCount = pMainPage->GetHyperLinkCount();									// 페이지의 링크 카운트를 받는다.

	// 협행 보일지 말지 결정
	BOOL bQuest = QUESTMGR->IsNpcRelationQuest( m_dwCurNpc );							// 퀘스트 여부를 받는다.

	pItem = new LINKITEM;																// 링크 아이템 메모리를 할당한다.
	strcpy( pItem->string, "" );														// 아이템 스트링을 ""로 세팅한다.
	m_pListDlg->cListItem::AddItem( pItem );											// 현재 아이템을 추가한다.

	for( int j = 0 ; j < nLinkCount ; ++j )												// 링크 카운트 만큼 for문을 돌린다.
	{
		pLink = pMainPage->GetHyperText(j);												// 카운트에 해당하는 링크 정보를 받는다.

		if( pLink == NULL )																// 링크 정보가 유효하지 않으면 
		{
			break;																		// for문을 탈출한다.
		}

		if( pLink->wLinkType == emLink_Quest && !bQuest )								// 링크 타입이 퀘스트와 같고, bQuest가 FALSE와 같으면,
		{
			continue;																	// 계속한다.
		}

		temp = pHyper->GetHyperText( pLink->wLinkId );									// 링크 아이디로 다이얼로그 정보를 받는다.

		if( temp )																		// 다이얼로그 정보가 유효하면,
		{
			LINKITEM* pItem = new LINKITEM;												// 링크 아이템 메모리를 할당한다.

			strcpy(pItem->string, temp->str);											// 아이템 스트링을 다이얼로그 스트링으로 세팅한다.

			pItem->dwType = pLink->wLinkType;											// 아이템 링크 타입을 세팅한다.
			
			m_pListDlg->cListItem::AddItem( pItem );									// 링크 리스트 다이얼로그에 아이템을 추가한다.

			m_sConversationHyper[m_nConversationHyperCount].bUse = TRUE;										// 하이퍼 링크 정보의 사용여부를 TRUE로 세팅한다.
			m_sConversationHyper[m_nConversationHyperCount].dwListItemIdx = m_pListDlg->GetItemCount()-1;		// 하이퍼 링크 정보의 리스트 아이템 인덱스를 세팅한다.
			m_sConversationHyper[m_nConversationHyperCount].sHyper = *pLink;									// 하이퍼 링크 정보의 링크 정보를 세팅한다.

			++m_nConversationHyperCount;												// 하이퍼 카운트를 증가한다.
		}
	}

	m_pListDlg->ResetGuageBarPos() ;													//대화 리스트의 게이지바 위치를 다시 잡는다.
}

void cNpcScriptDialog::SettingNpcImage(cPage* pMainPage, WORD npcChxNum)				// NPC 이미지와 배경을 세팅하는 함수.											
{
   	GAMEIN->GetNpcImageDlg()->SetNpc( npcChxNum );										// NPC 이미지 다이얼로그에 현재 NPC를 세팅한다.
 	GAMEIN->GetNpcImageDlg()->SetEmotion( pMainPage->GetEmotion() );					// NPC의 이모션을 세팅한다.
	GAMEIN->GetNpcImageDlg()->SetActive( TRUE );										// NPC 이미지 다이얼로그를 활성화 한다.
	GAMEIN->GetNpcImageDlg()->SetDisable( TRUE );										// NPC 이미지 다이얼로그를 디스에이블 시킨다.

	CNpc* pObj = NPCSCRIPTMGR->GetSelectedNpc();										// 선택 된 NPC 정보를 받는다.

	if( pObj )																			// NPC 정보가 유효하면,
	{
		SetDlgTitleText( pObj->GetObjectName() );										// 다이얼로그 제목을 NPC 이름으로 세팅한다.
	}
	else																				// NPC 정보가 유효하지 않으면,
	{	
 		SetDlgTitleText( CHATMGR->GetChatMsg(42) );										// 다이얼로그 제목을 NPC 대화창으로 한다.
		// desc_hseos_농장시스템_01
		// S 농장시스템 추가 added by hseos 2007.09.13
		// ..농장 집사NPC의 경우..
  		CSHFarmRenderObj* pFarmObj = g_csFarmManager.CLI_GetTargetObj();
		if (pFarmObj &&
			pFarmObj->GetInfo()->nKind == CSHFarmManager::RENDER_STEWARD_KIND)
		{
			// 이름
   			SetDlgTitleText( pFarmObj->GetObjectName() );									
			// 집사의 경우는 채팅창도 뜨도록 한다.. 시스템 메세지 때문에.
 			GAMEIN->GetChattingDlg()->SetActive( TRUE );
 			SetFocus(TRUE);
		}
		// E 농장시스템 추가 added by hseos 2007.09.13
		// 080414 KTH -- NPC
		//else if( pFarmObj &&
		//	pFarmObj->GetInfo()->nKind == CSHFarmManager::RENDER_ANIMAL_CAGE_SLOT_KIND )
		//{
		//	SetDlgTitleText( pFarmObj->GetObjectName() );
		//	GAMEIN->GetChattingDlg()->SetActive( TRUE );
		//	SetFocus(TRUE);
		//}
	}

	SetActive( TRUE );																	// NPC 스크립트 다이얼로그를 활성화 한다.
}

BOOL cNpcScriptDialog::OpenDialog( DWORD dwNpcId, WORD npcChxNum )						// 다이얼로그를 여는 함수.
{
	m_dwCurNpc = dwNpcId;																// 현재 NPC 아이디를 세팅한다.
	m_wNpcChxNum = npcChxNum ;															// 현재 NPC CHX 번호를 세팅한다.

	cPage* pMainPage = NPCSCRIPTMGR->GetMainPage( m_dwCurNpc );							// NPC 스크립트 매니져로 부터 현재 NPC의 메인 페이지 정보를 받는다.

	if( pMainPage == NULL ) return FALSE;												// 페이지 정보가 유효하지 않으면, FALSE 리턴 처리를 한다.

	cDialogueList* pList = NPCSCRIPTMGR->GetDialogueList();								// 스트립트 매니져로 부터 다이얼로그 정보를 받는다.

	if( pList == NULL ) return FALSE;													// 다이얼로그 정보가 유효하지 않으면 FALSE 리턴 처리를 한다.

	cHyperTextList* pHyper = NPCSCRIPTMGR->GetHyperTextList();							// 스크립트 매니져로 부터 하이퍼 텍스트 정보를 받는다.

	if( pHyper == NULL ) return FALSE;													// 하이퍼 텍스트 정보가 유효하지 않으면 FALSE 리턴 처리를 한다.

	DWORD dwMsg = pMainPage->GetRandomDialogue(); // 메세지의 번호						// 메시지를 랜덤하게 받는다.

	ResetConversationList(pList, dwMsg) ;												// 대화 리스트를 다시 세팅한다.

	ResetMainLinkList(pMainPage, pHyper, m_pLinkListDlg) ;								// 메인 링크 리스트를 다시 세팅한다.

	SettingNpcImage(pMainPage, npcChxNum) ;												// NPC 이미지와 배경이미지를 세팅한다.

	return TRUE;																		// TRUE를 리턴한다.
}

BOOL cNpcScriptDialog::OpenLinkPage( DWORD dwPageId )									// 링크 페이지를 여는 함수.
{
	m_dwCurPageId = dwPageId;															// 임시로 현재 표시 페이지를 가진다.	

	cPage* pPage = NPCSCRIPTMGR->GetPage( m_dwCurNpc, dwPageId );						// 현재 NPC와 현재 페이지 아이디로 페이지 정보를 받는다.

	if( pPage == NULL ) return FALSE;													// 페이지 정보가 유효하지 못하면 FALSE 리턴 처리를 한다.

	cDialogueList* pList = NPCSCRIPTMGR->GetDialogueList();								// 메세지 번호를 이용하여 사용할 메세지를 가져온다.

	if( pList == NULL ) return FALSE;													// 다이얼로그 정보가 유효하지 않으면, FALSE 리턴 처리를 한다.

	cHyperTextList* pHyper = NPCSCRIPTMGR->GetHyperTextList();							// 하이퍼 텍스트 리스트 정보를 받는다.

	if( pHyper == NULL ) return FALSE;													// 하이퍼 텍스트 리스트 정보가 유효하지 않으면 FALSE 리턴 처리를 한다.

	DWORD dwMsg = pPage->GetRandomDialogue();											// 페이지 정보를 이용하여 페이지를 구성한다. // 메세지의 번호

	ResetConversationList(pList, dwMsg) ;												// 대화 리스트를 다시 세팅한다.
	
	ResetConversationLinkList(pPage, pHyper) ;											// 대화 리스트에 링크를 세팅한다.
	
	GAMEIN->GetNpcImageDlg()->SetEmotion( pPage->GetEmotion() );						// NPC 이미지 다이얼로그의 이모션을 설정한다.

	return TRUE;																		// TRUE를 리턴한다.
}

void cNpcScriptDialog::EndDialog()														// 다이얼로그를 종료하는 함수.
{
	for(int i=0 ; i< MAX_REGIST_HYPERLINK;++i)											// 등록 가능한 하이퍼 링크 수만큼 for문을 돌린다.
	{
		m_sHyper[i].Init();																// 하이퍼 링크 정보를 초기화 한다.
	}

	m_pListDlg->RemoveAll();															// 리스트 다이얼로그를 모두 비운다.

	m_nHyperCount = 0;																	// 하이퍼 카운트 수를 0으로 세팅한다.

	m_dwCurNpc = 0;																		// 현재 npc를 0으로 세팅한다.
	m_wNpcChxNum = 0 ;																	// 현재 npc chx 번호를 0으로 세팅한다.

	GAMEIN->GetNpcImageDlg()->SetDisable( FALSE );										// NPC 이미지 다이얼로그의 Disable을 해제한다.
	GAMEIN->GetNpcImageDlg()->SetActive( FALSE );										// NPC 이미지 다이얼로그를 비활성화 한다.

	NPCSCRIPTMGR->SetSelectedNpc( NULL );												// 선택한 NPC를 해제한다.
	
	//죽은후에 들어올 수도 있다.
	if( HERO->GetState() != eObjectState_Die )											// HERO가 죽은 상태가 아니면,
	{
		BOOL chk = FALSE;

		chk = ( chk || ( ( cDialog* )GAMEIN->GetStorageDialog() )->IsActive() );
		chk = ( chk || ( ( cDialog* )GAMEIN->GetDealDialog() )->IsActive() );
		chk = ( chk || ( ( cDialog* )GAMEIN->GetGuildWarehouseDlg() )->IsActive() );

		if( !chk )
		{
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);						// 오브젝트 상태를 종료한다.
		}
	}

	SetDlgTitleText("");																// 타이틀을 ""로 세팅한다.
}

HYPER* cNpcScriptDialog::GetHyperInfo( DWORD dwIdx )									// 하이퍼 정보를 리턴하는 함수.
{
	for(int i=0;i<m_nHyperCount;++i)													// 하이퍼 카운트 만큼 for문을 돌린다.
	{
		if( m_sHyper[i].bUse && m_sHyper[i].dwListItemIdx == dwIdx)						// 하이버 정보의 사용여부가 TRUE이고, 인덱스가 같으면,
		{
			return &m_sHyper[i];														// 하이퍼 정보를 리턴한다.
		}
	}

	return NULL;																		// NULL을 리턴한다.
}

void cNpcScriptDialog::HyperLinkParser( DWORD dwIdx, BOOL bConversation )				// 하이퍼 링크 파서 함수.
{
	if( bConversation )
	{
		if( m_nConversationHyperCount == 0 ) return;													// 하이퍼 카운트가 0과 같으면 리턴 처리를 한다.
	}
	else
	{
		if( m_nHyperCount == 0 ) return;													// 하이퍼 카운트가 0과 같으면 리턴 처리를 한다.
	}

	int nType = -1;																		// 타입을 -1로 세팅한다.

	DWORD dwData = 0 ;																		// 데이터 변수를 선언한다.

	int	hypertext = 0;																	// 하이퍼 텍스트 변수를 0으로 세팅한다.

	float fXpos = 0.0f ;
	float fZpos = 0.0f ;

	if( bConversation )
	{
		for( int i=0 ; i<m_nConversationHyperCount ; ++i)									// 하이퍼 카운트 수 만큼 for문을 돌린다.
		{
			if( m_sConversationHyper[i].dwListItemIdx == dwIdx )							// 인덱스가 같으면,
			{
				nType		= m_sConversationHyper[i].sHyper.wLinkType;						// 링크 타입을 세팅한다.	
				dwData		= m_sConversationHyper[i].sHyper.dwData;						// 링크 데이터를 세팅한다.
				hypertext	= m_sConversationHyper[i].sHyper.wLinkId;						// 링크 아이디를 세팅한다.

				fXpos		= m_sConversationHyper[i].sHyper.fXpos ;						// 맵 이동시 사용할 x 좌표를 받는다.
				fZpos		= m_sConversationHyper[i].sHyper.fZpos ;						// 맵 이동시 사용할 y 좌표를 받는다.

				break;
			}
		}
	}
	else
	{
		for( int i=0 ; i<m_nHyperCount ; ++i)												// 하이퍼 카운트 수 만큼 for문을 돌린다.
		{
			if( m_sHyper[i].dwListItemIdx == dwIdx )										// 인덱스가 같으면,
			{
				nType = m_sHyper[i].sHyper.wLinkType;										// 링크 타입을 세팅한다.	
				dwData = m_sHyper[i].sHyper.dwData;											// 링크 데이터를 세팅한다.
				hypertext = m_sHyper[i].sHyper.wLinkId;										// 링크 아이디를 세팅한다.

				fXpos		= m_sHyper[i].sHyper.fXpos ;									// 맵 이동시 사용할 x 좌표를 받는다.
				fZpos		= m_sHyper[i].sHyper.fZpos ;									// 맵 이동시 사용할 y 좌표를 받는다.

				break;
			}
		}
	}

	switch( nType )																		// 링크 타입을 확인한다.
	{
	case emLink_Page:																	// 링크 페이지라면,
		{
			OpenLinkPage( dwData );														// 링크 페이지를 연다.
			m_pListDlg->ResetGuageBarPos();												// 게이지바 위치를 리세팅 한다.
		}
		break;

	case emLink_Open:																	// 링크 오픈이면,
		{
			// desc_hseos_농장시스템_01
			// S 농장시스템 추가 added by hseos 2008.01.23
			// ..농장 업그레이드 처리
			if (m_dwCurNpc == 999)
			{		
				if (m_dwCurPageId == 90		&&
					g_csFarmManager.CLI_ProcUpgradeDlg(dwData))
				{
					return;
				}
				// ..농장 관리비 납부 처리
				if (hypertext == 691	&&
					dwData == 6			&&
					g_csFarmManager.CLI_ProcPayTaxDlg())
				{
					return;
				}
			}
			// E 농장시스템 추가 added by hseos 2008.01.23

			// 080514 KTH -- 주석 처리
			//NPCSCRIPTMGR->StartNpcBusiness( dwData );									// 거래창을 연다.
			// 080514 KTH -- Hack Check
			CObject* pObject = OBJECTMGR->GetSelectedObject();

			MSG_DWORD3	msg;
			msg.Protocol = MP_NPC_CHECK_HACK_SYN;
			msg.Category = MP_NPC;
			msg.dwObjectID = HEROID;
			msg.dwData1 = m_dwCurNpc;
			msg.dwData2 = (DWORD)((CNpc*)pObject)->GetNpcUniqueIdx();
			msg.dwData3 = dwData;
			NETWORK->Send(&msg, sizeof(msg));
		}
		break;

	case emLink_End:																	// 링크 종료면,
		{
			EndDialog();																// 다이얼로그를 종료한다.
			DisActiveEx();																// Disable한다.
		}
		break;

	case emLink_Quest:																	// 링크 퀘스트면,
		{
			m_dwMainLinkPageId = dwData ;												// 메인 퀘스트 링크 페이지를 저장한다.
			OpenQuestLinkPage( dwData );												// 퀘스트 링크 페이지를 연다.
		}
		break;

	case emLink_QuestLink:																// 링크 퀘스트 링크면,
		{
			m_dwQuestIdx = 0;															// 퀘스트 인덱스를 0으로 세팅한다.

			DWORD dwPage = 0;															// 페이지를 0으로 세팅한다.

			CQuest* pQuest = QUESTMGR->GetQuest( dwData );								// 퀘스트 정보를 받는다.

			if( pQuest )																// 퀘스트 정보가 유효하면,
			{
				dwPage = pQuest->GetNpcScriptPage( m_dwCurNpc );						// 페이지를 받는다.

				if( dwPage == 0 )														// 페이지가 0과 같으면,
				{
					dwPage = 30;														// 페이지를 30으로 세팅한다.
				}

				OpenLinkPage( dwPage );													// 링크 페이지를 연다.

				m_dwQuestIdx = dwData;													// 퀘스트 인덱스를 세팅한다.
			}						
		}
		break;

	case emLink_QuestStart:																// 퀘스트를 수락했다면,
		{
			if( m_dwQuestIdx == 45 || m_dwQuestIdx == 46 || m_dwQuestIdx == 47 )		// 퀘스트 인덱스가 45, 46, 47이면,
			{
				CQuest* pQuest = QUESTMGR->GetQuest( m_dwQuestIdx );					// 퀘스트 정보를 받는다.

				if( pQuest )															// 퀘스트 정보가 유효하다면,
				{
					if( pQuest->IsTimeCheck() )											// 타임 체크 퀘스트라면,
					{
					}
					else																// 타임 체크 퀘스트가 아니라면,
					{
						if( hypertext == 347 || hypertext == 349 )						// 하이퍼 텍스트가 347 이나 349라면,
						{
							HERO->StartSocietyActForQuest( 259, -1, -1, TRUE );			// 퀘스트를 위한 액션을 시작한다.
						}

						OpenLinkPage( dwData );											// 링크 페이지를 연다.
					}
				}
			}
			else																		// 퀘스트 인덱스가 45, 46, 47이 아니면,
			{
				OpenLinkPage( dwData );													// 링크 페이지를 연다.
			}

			CQuestDialog* pQuestDialog = GAMEIN->GetQuestDialog() ;

			if( pQuestDialog ) 
			{
				// 080506 LYW --- NpcScriptDialog : 퀘스트 목록이 20개 모두 찼을 때, 퀘스트를 완료 할 수 없는 버그 수정.
				//if( pQuestDialog->GetQuestCount() >= MAX_QUEST_COUNT )
				if( pQuestDialog->GetQuestCount() > MAX_QUEST_COUNT )
				{
					//WINDOWMGR->MsgBox( NULL, MBT_OK, "퀘스트 목록이 가득 차서 더 이상 퀘스트를 진행 할 수 없습니다." );
					WINDOWMGR->MsgBox( NULL, MBT_OK, CHATMGR->GetChatMsg( 1315 ) );

					return ;
				}
			}

			if( m_dwQuestIdx )															// 퀘스트 인덱스정보가 유효하다면,
			{
				QUESTMGR->SendNpcTalkEvent( m_dwCurNpc, m_dwQuestIdx );					// NPC 대화 이벤트를 보낸다.
			}
			
			if(NPCSCRIPTMGR->GetSelectedNpc())											// 선택한 NPC 정보가 유효하다면,
			{
				if(NPCSCRIPTMGR->GetSelectedNpc()->GetNpcJob() == BOMUL_ROLE)			// NPC의 직업이 보물 롤이라면,
				{
					MSG_DWORD msg;														// 메시지 구조체를 선언한다.
					msg.Category = MP_NPC;												// 카테고리를 NPC로 세팅한다.
					msg.Protocol = MP_NPC_DOJOB_SYN;									// 프로토콜을 직업싱크로 세팅한다.
					msg.dwData = NPCSCRIPTMGR->GetSelectedNpc()->GetID();				// 선택한 NPC의 아이디를 세팅한다.
					msg.dwObjectID = HEROID;											// HERO의 아이 세팅한다.
					NETWORK->Send(&msg, sizeof(msg));									// 메시지를 전송한다.
				}
			}
		}
		break;

	case emLink_EventQuestStart:														// 퀘스트 시작 이벤트 링크라면,
		{
			if( dwData )																// 데이터 정보가 유효한지 체크한다.
			{
				QUESTMGR->SendNpcTalkEventForEvent( m_dwCurNpc, dwData );				// 이벤트를 위한 NPC 대화 이벤트를 보낸다.
			}

			EndDialog();																// 다이얼로그를 닫는다.
			DisActiveEx();																// 비활성화 시킨다.
		}
		break;

	case emLink_QuestContinue:															// 퀘스트를 계속하는 링크라면,
		{
			OpenLinkPage( dwData );														// 링크 페이지를 연다.
		}
		break;

	case emLink_QuestEnd:																// 퀘스트를 종료하는 링크라면,
		{
			QUESTMGR->QuestAbandon( m_dwQuestIdx );										// 퀘스트를 포기한다.

			OpenLinkPage( dwData );														// 링크 페이지를 연다.
		}
		break;

	case emLink_MapChange:																// 맵 체인지 링크라면,
		{
			//g_UserInput.GetHeroMove()->MapChange((WORD)dwData );						// 맵 체인지를 한다.
			g_UserInput.GetHeroMove()->MapChangeToNpc(dwData, fXpos, fZpos) ;
		}
		break;

	case emLink_ChangeMapPage :
		{
			cPage* pPage = NPCSCRIPTMGR->GetPage( m_dwCurNpc, dwData );					// 현재 NPC와 현재 페이지 아이디로 페이지 정보를 받는다.
			if( pPage == NULL ) return ;												// 페이지 정보가 유효하지 못하면 FALSE 리턴 처리를 한다.

			cDialogueList* pList = NPCSCRIPTMGR->GetDialogueList();						// 메세지 번호를 이용하여 사용할 메세지를 가져온다.
			if( pList == NULL ) return ;												// 다이얼로그 정보가 유효하지 않으면, FALSE 리턴 처리를 한다.

			cHyperTextList* pHyper = NPCSCRIPTMGR->GetHyperTextList();					// 하이퍼 텍스트 리스트 정보를 받는다.
			if( pHyper == NULL ) return ;												// 하이퍼 텍스트 리스트 정보가 유효하지 않으면 FALSE 리턴 처리를 한다.

			DWORD dwMsg = pPage->GetRandomDialogue();									// 페이지 정보를 이용하여 페이지를 구성한다. // 메세지의 번호
			ResetConversationList(pList, dwMsg) ;										// 대화 리스트를 다시 세팅한다.

			m_pListDlg->ResetGuageBarPos();												// 게이지바 위치를 리세팅 한다.

			m_dwMainLinkPageId = dwData ;												// 메인 퀘스트 링크 페이지를 저장한다.
			OpenChangeMapLinkPage( dwData ) ;											// 맵 변경 링크 페이지를 연다.
		}
		break ;

	case emLink_FishPointChange:
		{
			GAMEIN->GetNpcScriptDialog()->EndDialog();
			GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
			GAMEIN->GetFishingPointDlg()->ShowDealDialog(TRUE);
		}
		break;
	}	
}

void cNpcScriptDialog::SetDlgTitleText( char* strTitle )								// 다이얼로그 제목을 세팅하는 함수.
{
	if( !m_pTitleText ) return;															// 다이얼로그 제목 정보가 유효하지 않으면 리턴 처리를 한다.

	m_pTitleText->SetStaticText( strTitle );											// 들어온 제목으로 다이얼로그 제목을 세팅한다.
}

void cNpcScriptDialog::SetActive( BOOL val )											// 활성화 여부 세팅 함수.
{
//	if( m_bActive == val ) return;														// 현재 활성화 여부와 들어온 값이 같으면 리턴 처리를 한다.

	if(!val)																			// 비활성화와 같으면,
	{
		CNpc* pNpc = NPCSCRIPTMGR->GetSelectedNpc();									// 현재 선택한 NPC 정보를 받는다.

		if(pNpc)																		// NPC 정보가 유효하면,
		{			
			if( pNpc->GetNpcJob() == BOMUL_ROLE)										// NPC 직업을 받고, 직업이 보물 롤과 같으면,
			{
				MSG_DWORD msg;															// 메시지 구조체를 선언한다.
				msg.Category = MP_NPC;													// 카테고리를 NPC로 세팅한다.
				msg.Protocol = MP_NPC_CLOSEBOMUL_SYN;									// 프로토콜을 MP_NPC_CLOSEBOMUL_SYN로 세팅한다.
				msg.dwObjectID = HERO->GetID();											// HERO의 아이디를 세팅한다.
				msg.dwData = NPCSCRIPTMGR->GetSelectedNpc()->GetID();					// 현재 선택 된 NPC 아이디를 세팅한다.
				NETWORK->Send(&msg, sizeof(msg));										// 메시지를 전송한다.
			}
		}

		EndDialog();																	// 다이얼로그를 끝낸다.
	}



	cDialog::SetActiveRecursive( val );													// 								
}

// 다른곳에서 절대 호출하지 마시오!!
void cNpcScriptDialog::DisActiveEx()													// 비활성화 확장 함수.
{
	for(int i=0 ; i< MAX_REGIST_HYPERLINK;++i)											// 등록 가능한 하이퍼 링크 수 만큼 for문을 돌린다.
	{
		m_sHyper[i].Init();																// 하이퍼 링크 정보를 초기화 한다.
	}

	m_pListDlg->RemoveAll();															// 리스트 다이얼로그를 모두 비운다.

	m_nHyperCount = 0;																	// 하이퍼 카운트 수를 0으로 세팅한다.
	m_dwCurNpc = 0;																		// 현재 npc를 0으로 세팅한다.
	m_wNpcChxNum = 0 ;																	// 현재 npc chx 번호를 0으로 세팅한다.

	NPCSCRIPTMGR->SetSelectedNpc( NULL );												// 현재 선택한 npc 를 해제한다.

	SetDlgTitleText("");																// 다이얼로그 타이틀을 ""로 세팅한다.

	cDialog::SetActiveRecursive( FALSE );												// 다이얼로그를 비활성화 한다.

	WINDOWMGR->SetMouseInputProcessed();												// 윈도우 매니져의 마우스 입력 처리 프로세스를 호출한다.
}

void cNpcScriptDialog::RefreshQuestLinkPage()
{
	for( int i = 0; i < MAX_REGIST_HYPERLINK; ++i )										// 등록 가능한 하이퍼 링크 수 만큼 for문을 돌린다.
	{
		m_sHyper[i].Init();																// 하이퍼 링크 정보를 초기화 한다.
	}

	m_pLinkListDlg->RemoveAll() ;														// 링크 리스트를 모두 비운다.				

	m_nHyperCount = 0;																	// 하이퍼 카운트를 0으로 세팅한다.

	DWORD dwPageId = 0 ;

	if( m_dwMainLinkPageId != 0 )
	{
		dwPageId = m_dwMainLinkPageId ;
	}
	else
	{
		dwPageId = m_dwCurPageId ;
	}

	cPage* pPage = NPCSCRIPTMGR->GetPage( m_dwCurNpc, dwPageId );						// 현재 npc와 페이지 아이디로 페이지 정보를 얻는다.

	if( pPage == NULL ) return;															// 페이지 정보가 유효하지 않으면 리턴 처리를 한다.

	cHyperTextList* pHyper = NPCSCRIPTMGR->GetHyperTextList();							// 하이퍼 텍스트 정보를 받는다.

	if( pHyper == NULL ) return;														// 하이퍼 텍스트 정보가 유효하지 않으면 리턴 처리를 한다.

	DIALOGUE* temp = NULL;																// 임시 다이얼로그 정보를 받을 포인터 변수를 선언하고 NULL 처리를 한다.

	LINKITEM* pItem = NULL;																// 링크 아이템 정보를 받을 포인터 변수를 선언하고 NULL 처리를 한다.

	HYPERLINK* pLink;																	// 링크 정보를 받을 포인터를 선언한다. // 링크 정보를 가져와서 설정한다.

	int nLinkCount = pPage->GetHyperLinkCount();										// 링크 카운트를 세팅한다.

	pItem = new LINKITEM;																// 링크 아이템 메모리를 할당한다.
	strcpy( pItem->string, CHATMGR->GetChatMsg(239) );									// 아이템 스트링으로 [ 퀘스트 목록 ]을 세팅한다.
	pItem->rgb = RGBA_MAKE( 10, 10, 10, 255 );											// 아이템 색상을 세팅한다.
	m_pLinkListDlg->cListItem::AddItem( pItem );										// 링크 리스트 다이얼로그에 아이템을 추가한다.

	// 한줄 띄우기
	pItem = new LINKITEM;																// 링크 아이템 메모리를 할당한다.
	strcpy( pItem->string, "" );														// 아이템 스트링을 ""로 세팅한다.
	m_pLinkListDlg->cListItem::AddItem( pItem );										// 현재 아이템을 추가한다.

	BOOL bCheck = FALSE;																// 체크 여부를 FALSE로 세팅한다.

	// 수행가능한 협행
	for( int j = 0; j < nLinkCount; ++j )												// 링크 카운트 만큼 for문을 돌린다.
	{
		pLink = pPage->GetHyperText(j);													// 하이퍼 텍스트 정보를 받는다.

		if( pLink == NULL )																// 하이퍼 텍스트 정보가 유효하지 않으면,
		{
			break;																		// for문을 탈출한다.
		}

		CQuest* pQuest = QUESTMGR->GetQuest( pLink->dwData );							// 퀘스트 정보를 받는다.

		if( pQuest )																	// 퀘스트 정보가 유효하면,
		{
			if( pQuest->IsQuestState( m_dwCurNpc ) != 1 )								// 퀘스트 상태가를 체크한다.
			{
				continue;																// 계속한다.
			}
		}
		else																			// 퀘스트 정보가 유효하지 않으면,
			continue;																	// 계속한다.

		if( !bCheck )																	// 체크 여부가 FALSE와 같으면,
		{
			pItem = new LINKITEM;														// 링크 아이템 메모리를 세팅한다.
			strcpy( pItem->string, CHATMGR->GetChatMsg(243) );							// 수행 가능한 퀘스트를 세팅한다.
			pItem->rgb = RGBA_MAKE( 0, 0, 255, 255 );									// 아이템 색상을 세팅한다.
			m_pLinkListDlg->cListItem::AddItem( pItem );								// 리스트 다이얼로그에 아이템을 추가한다.
			bCheck = TRUE;																// 체크 여부를 TRUE로 세팅한다.
		}

		temp = pHyper->GetHyperText( pLink->wLinkId );									// 하이퍼 텍스트 정보를 받는다.

		if( temp )																		// 정보가 유효하면,
		{
			pItem = new LINKITEM;														// 링크 아이템 메모리를 할당한다.
			strcpy( pItem->string, temp->str );											// 링크 스트링을 아이템 스트링에 세팅한다.
			pItem->dwType = pLink->wLinkType;											// 아이템 타입을 링크 타입으로 세팅한다.
			
			m_pLinkListDlg->cListItem::AddItem( pItem );								// 리스트 다이얼로그에 아이템을 추가한다.

			// Data
			m_sHyper[m_nHyperCount].bUse = TRUE;										// 사용여부를 TRUE로 세팅한다.
			m_sHyper[m_nHyperCount].dwListItemIdx = m_pLinkListDlg->GetItemCount()-1;	// 아이템 인덱스를 세팅한다.
			m_sHyper[m_nHyperCount].sHyper = *pLink;									// 하이퍼 링크 정보를 세팅한다.

			++m_nHyperCount;															// 하이퍼 링크 카운트를 증가한다.
		}
	}

	if( bCheck )																		// 체크 여부가 TRUE와 같다면,
	{
		pItem = new LINKITEM;															// 링크 아이템 메모리를 할당한다.
		strcpy( pItem->string, "" );													// 스트링을 ""로 세팅한다.
		m_pLinkListDlg->cListItem::AddItem( pItem );									// 리스트 다이얼로그에 아이템을 추가한다.
	}

	bCheck = FALSE;																		// 체크 여부를 FALSE로 세팅한다.
	// 수행중인 협행

	for( j = 0; j < nLinkCount; ++j )													// 링크 카운트 수 만큼 for문을 돌린다.
	{
		pLink = pPage->GetHyperText(j);													// 하이퍼 링크 정보를 받는다.

		if( pLink == NULL ) break;														// 하이퍼 링크 정보가 유효하지 않으면 for문을 탈출한다.

		CQuest* pQuest = QUESTMGR->GetQuest( pLink->dwData );							// 퀘스트 정보를 받는다.

		temp = pHyper->GetHyperText( pLink->wLinkId );									// 하이퍼 텍스트 정보를 받는다.
		if( pQuest )																	// 퀘스트 정보가 유효하면,
		{
			if( pQuest->IsQuestState( m_dwCurNpc ) != 2 )								// 퀘스트 상태를 체크한다.
			{
				continue;																// 계속한다.
			}
		}
		else																			// 퀘스트 정보가 유효하지 않으면,
			continue;																	// 계속한다.

		if( !bCheck )																	// 체크여부가 FALSE와 같으면,
		{
			pItem = new LINKITEM;														// 링크 아이템 메모리를 할당한다.
			strcpy( pItem->string, CHATMGR->GetChatMsg(244) );							// 스트링을 수행중인 퀘스트로 세팅한다.
			pItem->rgb = RGBA_MAKE( 10, 10, 10, 255 );									// 아이템 색상을 세팅한다.
			m_pLinkListDlg->cListItem::AddItem( pItem );								// 리스트 다이얼로그에 아이템을 추가한다.
			bCheck = TRUE;																// 체크 여부를 TRUE로 세팅한다.
		}

		temp = pHyper->GetHyperText( pLink->wLinkId );									// 하이퍼 텍스트 정보를 받는다.

		if( temp )																		// 하이퍼 텍스트 정보가 유효하면,
		{
			pItem = new LINKITEM;														// 링크 아이템 메모리를 할당한다.
			strcpy( pItem->string, temp->str );											// 아이템 스트링을 링크 스트링으로 세팅한다.

			pItem->dwType = pLink->wLinkType;											// 아이템 타입을 링크 타입으로 세팅한다.
			
			m_pLinkListDlg->cListItem::AddItem( pItem );								// 리스트 다이얼로그에 아이템을 추가한다.

			// Data
			m_sHyper[m_nHyperCount].bUse = TRUE;										// 사용여부를 TRUE로 세팅한다.
			m_sHyper[m_nHyperCount].dwListItemIdx = m_pLinkListDlg->GetItemCount()-1;	// 아이템 인덱스를 세팅한다.
			m_sHyper[m_nHyperCount].sHyper = *pLink;									// 하이퍼 링크 정보를 세팅한다.

			++m_nHyperCount;															// 하이퍼 링크 카운트를 증가한다.
		}
	}

	if( m_pLinkListDlg->GetItemCount() <= 2 )
	{
		m_pLinkListDlg->RemoveAll() ;
	}

	m_pLinkListDlg->ResetGuageBarPos() ;												// 링크 리스트의 게이지바 위치를 다시 잡는다.
}

// 070917 LYW --- NpcScriptDialog : Add function to refresh page to change map.
void cNpcScriptDialog::RefreshChangeMapLinkPage()
{
	for( int i = 0; i < MAX_REGIST_HYPERLINK; ++i )										// 등록 가능한 하이퍼 링크 수 만큼 for문을 돌린다.
	{
		m_sHyper[i].Init();																// 하이퍼 링크 정보를 초기화 한다.
	}

	m_pLinkListDlg->RemoveAll() ;														// 링크 리스트를 모두 비운다.				

	m_nHyperCount = 0;																	// 하이퍼 카운트를 0으로 세팅한다.

	DWORD dwPageId = 0 ;

	if( m_dwMainLinkPageId != 0 )
	{
		dwPageId = m_dwMainLinkPageId ;
	}
	else
	{
		dwPageId = m_dwCurPageId ;
	}

	cPage* pPage = NPCSCRIPTMGR->GetPage( m_dwCurNpc, dwPageId );						// 현재 npc와 페이지 아이디로 페이지 정보를 얻는다.

	if( pPage == NULL ) return;															// 페이지 정보가 유효하지 않으면 리턴 처리를 한다.

	cHyperTextList* pHyper = NPCSCRIPTMGR->GetHyperTextList();							// 하이퍼 텍스트 정보를 받는다.

	if( pHyper == NULL ) return;														// 하이퍼 텍스트 정보가 유효하지 않으면 리턴 처리를 한다.

	DIALOGUE* temp = NULL;																// 임시 다이얼로그 정보를 받을 포인터 변수를 선언하고 NULL 처리를 한다.

	LINKITEM* pItem = NULL;																// 링크 아이템 정보를 받을 포인터 변수를 선언하고 NULL 처리를 한다.

	HYPERLINK* pLink;																	// 링크 정보를 받을 포인터를 선언한다. // 링크 정보를 가져와서 설정한다.

	int nLinkCount = pPage->GetHyperLinkCount();										// 링크 카운트를 세팅한다.

	pItem = new LINKITEM;																// 링크 아이템 메모리를 할당한다.
	//strcpy( pItem->string, "[ 이동가능한 지역 ]" );										// 아이템 스트링으로 [ 퀘스트 목록 ]을 세팅한다.
	strcpy( pItem->string, CHATMGR->GetChatMsg(1472) );
	pItem->rgb = RGBA_MAKE( 0, 0, 255, 255 );											// 아이템 색상을 세팅한다.
	m_pLinkListDlg->cListItem::AddItem( pItem );										// 링크 리스트 다이얼로그에 아이템을 추가한다.

	// 한줄 띄우기
	pItem = new LINKITEM;																// 링크 아이템 메모리를 할당한다.
	strcpy( pItem->string, "" );														// 아이템 스트링을 ""로 세팅한다.
	m_pLinkListDlg->cListItem::AddItem( pItem );										// 현재 아이템을 추가한다.

	// 수행가능한 협행
	for( int j = 0; j < nLinkCount; ++j )												// 링크 카운트 만큼 for문을 돌린다.
	{
		pLink = pPage->GetHyperText(j);													// 하이퍼 텍스트 정보를 받는다.

		if( pLink == NULL )																// 하이퍼 텍스트 정보가 유효하지 않으면,
		{
			break;																		// for문을 탈출한다.
		}

		temp = pHyper->GetHyperText( pLink->wLinkId );									// 하이퍼 텍스트 정보를 받는다.

		if( temp )																		// 정보가 유효하면,
		{
			pItem = new LINKITEM;														// 링크 아이템 메모리를 할당한다.
			strcpy( pItem->string, temp->str );											// 링크 스트링을 아이템 스트링에 세팅한다.
			pItem->dwType = pLink->wLinkType;											// 아이템 타입을 링크 타입으로 세팅한다.
			
			m_pLinkListDlg->cListItem::AddItem( pItem );								// 리스트 다이얼로그에 아이템을 추가한다.

			// Data
			m_sHyper[m_nHyperCount].bUse = TRUE;										// 사용여부를 TRUE로 세팅한다.
			m_sHyper[m_nHyperCount].dwListItemIdx = m_pLinkListDlg->GetItemCount()-1;	// 아이템 인덱스를 세팅한다.
			m_sHyper[m_nHyperCount].sHyper = *pLink;									// 하이퍼 링크 정보를 세팅한다.

			++m_nHyperCount;															// 하이퍼 링크 카운트를 증가한다.
		}
	}

	m_pLinkListDlg->ResetGuageBarPos() ;												// 링크 리스트의 게이지바 위치를 다시 잡는다.
}


void cNpcScriptDialog::OpenQuestLinkPage( DWORD dwPageId )								// 퀘스트 링크 페이지를 여는 함수.
{
	m_dwCurPageId = dwPageId;															// 페이지 정보를 세이브 한다.

	RefreshQuestLinkPage() ;															// 퀘스트 링크 리스트를 새로고침 한다.
}

// 070917 LYW --- NpcScriptDialog : Add function to open page to change map.
void cNpcScriptDialog::OpenChangeMapLinkPage( DWORD dwPageId )							// 맵 변경 링크 페이지를 여는 함수.
{
	m_dwCurPageId = dwPageId;															// 페이지 정보를 세이브 한다.

	//RefreshQuestLinkPage() ;															// 퀘스트 링크 리스트를 새로고침 한다.
	RefreshChangeMapLinkPage() ;
}
