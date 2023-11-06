#include "stdafx.h"																	// 표준 시스템과, 프로젝트가 지정한, 자주쓰는 해더들을 모은 해더파일을 불러온다.

#include ".\questquickviewdialog.h"													// 퀘스트 알림이 클래스 헤더를 불러온다.

#include "WindowIDEnum.h"															// 윈도우 아이디 이넘 헤더를 불러온다.

#include "./Interface/cWindowManager.h"												// 윈도우 매니져 클래스 헤더를 불러온다.
#include "./Interface/cListDialog.h"												// 리스트 다이얼로그 클래스 헤더를 불러온다.
#include "QuestManager.h"															// 퀘스트 매니져 클래스 헤더를 불러온다.

#include "Quest.h"																	// 퀘스트 클래스 헤더를 불러온다.

#include "..\[CC]Quest\QuestInfo.h"
#include "..\[CC]Quest\QuestString.h"												// 퀘스트 스트링 클래스 헤더를 불러온다.

#include "cMsgBox.h"																// 알림 메시지 박스 클래스 헤더를 불러온다.
#include "QuestDialog.h"															// 퀘스트 다이얼로그 클래스 헤더를 불러온다.

#include "ChatManager.h"

CQuestQuickViewDialog::CQuestQuickViewDialog(void)									// 생성자 함수.
{
	m_type = WT_QUESTQUICKVIEWDIALOG ;												// 윈도우 타입을 알림이 다이얼로그로 세팅한다.

	m_QuestTreeList.RemoveAll() ;													// 퀘스트 트리 리스트를 모두 비운다.

	m_pViewList = NULL ;															// 트리 리스트 다이얼로그 포인터를 null로 세팅한다.
}

CQuestQuickViewDialog::~CQuestQuickViewDialog(void)									// 소멸자 함수.
{
	Release() ;																		// 해제 함수를 호출한다.
}

void CQuestQuickViewDialog::Release()												// 사용한 리소스를 해제하는 함수.
{
	DWORD* pDeleteIdx ;																// 삭제할 퀘스트 인덱스를 받을 포인터를 선언한다.

	PTRLISTPOS pos = NULL ;															// 위치 정보를 받을 포인터를 선언하고 null 처리 한다.
	pos = m_QuestTreeList.GetHeadPosition() ;										// 퀘스트 리스트를 헤드로 세팅하고 위치 정보를 받는다.

	while(pos)																		// 위치 정보가 유효할 동안 while문을 돌린다.
	{
		pDeleteIdx = NULL ;															// 삭제할 인덱스를 담을 포인터를 null 처리한다.
		pDeleteIdx = (DWORD*)m_QuestTreeList.GetNext(pos) ;							// 위치에 해당하는 인덱스를 받고, 위치를 다음으로 세팅한다.

		if( !pDeleteIdx ) continue ;												// 퀘스트 인덱스가 유효하지 않으면, continue 한다.

		m_QuestTreeList.Remove( pDeleteIdx ) ;										// 퀘스트 리스트에서 현재 인덱스를 꺼낸다.
	
		delete pDeleteIdx ;															// 현재 퀘스트 인덱스를 삭제한다.
	}

	m_QuestTreeList.RemoveAll() ;													// 퀘스트 리스트를 모두 비운다.
}

void CQuestQuickViewDialog::Linking()												// 링크 함수.
{
	m_pViewList = (cListDialog*)GetWindowForID(QUE_QUICKVIEWLIST) ;					// 트리 리스트 다이얼로그를 링크한다.
	m_pViewList->SetStyle( LDS_FONTSHADOW ) ;										// 리스트 다이얼로그를 그림자 출력 스타일로 세팅한다.
	// 071018 LYW --- QuestQuickViewDialog : Setting shadow distance of shadow font to use render shadowfont.
	m_pViewList->Set_ShadowDistance(1) ;
}

BOOL CQuestQuickViewDialog::CheckSameQuest(DWORD dwQuestIdx)						// 퀘스트 리스트 내에 같은 퀘스트가 있는지 여부를 반환하는 함수.
{
	BOOL b_Result = FALSE ;															// 결과 값을 담을 변수를 선언하고 값을 false로 세팅한다.

	DWORD* pCheckIdx ;																// 퀘스트 인덱스 정보를 받을 포인터를 선언한다.

	PTRLISTPOS pos = NULL ;															// 위치 정보를 받을 포인터를 선언하고 null 처리 한다.
	pos = m_QuestTreeList.GetHeadPosition() ;										// 퀘스트 리스트를 헤드로 세팅하고 위치 정보를 받는다.

	while(pos)																		// 위치 정보가 유효할 동안 while문을 돌린다.
	{
		pCheckIdx = NULL ;															// 퀘스트 인덱스 포인터를 null 처리 한다.
		pCheckIdx = (DWORD*)m_QuestTreeList.GetNext(pos) ;							// 위치에 해당하는 퀘스트 인덱스 정보를 받고, 다음 위치로 세팅한다.

		if( !pCheckIdx ) continue ;													// 퀘스트 인덱스 정보가 유효하지 않으면, continue 한다.

		if( *pCheckIdx != dwQuestIdx ) continue ;									// 인자로 넘어온 인덱스와, 현재 퀘스트 인덱스가 다르면, continue 한다.

		b_Result = TRUE ;															// 결과 값을 true로 세팅한다.

		break ;																		// while문을 빠져나간다.
	}

	return b_Result ;																// 결과 값을 return 처리 한다.
}

void CQuestQuickViewDialog::RegistQuest(DWORD dwQuestIdx)							// 알림이 창에 퀘스트를 등록 시키는 함수.
{
	CQuest* pQuest = QUESTMGR->GetQuest(dwQuestIdx) ;								// 인자로 넘어온 퀘스트 인덱스로 퀘스트 정보를 받는다.

	if( !pQuest ) return ;															// 퀘스트 정보가 유효하지 않으면, return 처리한다.

	if( pQuest->GetSubQuestFlag() == 0 )
	{
		DeleteQuest(dwQuestIdx) ;

		return ;
	}

	if( CheckSameQuest(dwQuestIdx) )												// 알림이 창에 인자로 넘어온 퀘스트와 같은 퀘스트가 있으면,
	{
		DeleteQuest(dwQuestIdx) ;													// 퀘스트를 삭제한다.
		return ;																	// return 처리한다.
	}

	int nRegistedCount = 0 ;														// 현재 등록 된 카운트 값을 담을 변수를 선언하고 0으로 세팅한다.
	nRegistedCount = m_QuestTreeList.GetCount() ;									// 퀘스트 트리 리스트의 카운트 값을 받는다.

	if( nRegistedCount >= MAX_QUESTTREECOUNT )										// 등록 된 퀘스트 수가 최대 등록 가능수 이상이면,
	{
		char tempBuf[128] = {0} ;													// 퀘스트 증록에 사용할 임시 버퍼를 선언한다.

		//sprintf(tempBuf, "알림이 등록은 %d개 까지만 할 수 있습니다.", 
		sprintf(tempBuf, CHATMGR->GetChatMsg(1465), 
			MAX_QUESTTREECOUNT) ;													// 등록 오류 메시지를 생성한다.

		WINDOWMGR->MsgBox( MBI_QUESTQUICKVIEW_FULL, MBT_OK, tempBuf) ;				// 오류 메시지를 출력한다.

		return ;																	// return 처리한다.
	}

	DWORD* pQuestIdx = new DWORD ;													// DWORD 형 데이터를 생성하고 포인터에 담는다.

	*pQuestIdx = dwQuestIdx ;														// 등록하려는 퀘스트 인덱스를 세팅한다.

	m_QuestTreeList.AddTail(pQuestIdx) ;											// 퀘스트 리스트에 추가한다.

	ResetQuickViewDialog() ;														// 알림이 창을 새로고침 한다.
}

void CQuestQuickViewDialog::DeleteQuest(DWORD dwQuestIdx) 							// 알림이 창에 있는 퀘스트를 삭제 하는 함수.
{
	DWORD* pDeleteIdx ;																// 삭제할 퀘스트 인덱스를 받을 포인터를 선언한다.

	PTRLISTPOS pos = NULL ;															// 위치 정보를 받을 포인터를 선언하고 null 처리 한다.
	pos = m_QuestTreeList.GetHeadPosition() ;										// 퀘스트 리스트를 헤드로 세팅하고 위치 정보를 받는다.

	while(pos)																		// 위치 정보가 유효할 동안 while문을 돌린다.
	{
		pDeleteIdx = NULL ;															// 삭제할 인덱스를 담을 포인터를 null 처리한다.
		pDeleteIdx = (DWORD*)m_QuestTreeList.GetNext(pos) ;							// 위치에 해당하는 인덱스를 받고, 위치를 다음으로 세팅한다.

		if( !pDeleteIdx ) continue ;												// 퀘스트 인덱스가 유효하지 않으면, continue 한다.

		if( *pDeleteIdx != dwQuestIdx ) continue ;									// 인자로 넘어온 인덱스와 현재 인덱스가 다르면, continue 한다.

		m_QuestTreeList.Remove( pDeleteIdx ) ;										// 퀘스트 리스트에서 현재 인덱스를 꺼낸다.
	
		delete pDeleteIdx ;															// 현재 퀘스트 인덱스를 삭제한다.

		break ;																		// while문을 탈출한다.
	}

	ResetQuickViewDialog() ;														// 알림이 창을 새로고침 한다.
}

void CQuestQuickViewDialog::ResetQuickViewDialog()									// 퀘스트 리스트를 새로고침하는 함수.
{
	int nItemCount = 0 ;															// 알림이 리스트의 아이템 카운트를 담을 변수를 선언하고 0으로 세팅한다.
	nItemCount = m_pViewList->GetItemCount() ;										// 알림이 리스트의 아이템 카운트를 받는다.

	m_pViewList->RemoveAll() ;														// 퀘스트 리스트 다이얼로그를 모두 비운다.

	for( int count = 0 ; count < nItemCount ; ++count )								// 알림이 리스트의 아이템 카운트 만큼 for문을 돌린다.
	{
		m_pViewList->SetExtendReduction(LD_REDUCTION) ;								// 리스트 다이얼로그를 축소한다.
	}

	DWORD* pCheckIdx ;																// 인덱스 정보를 받을 포인터를 선언한다.

	CQuest* pQuest ;																// 퀘스트 정보를 받을 포인터를 선언한다.
	CSubQuestInfo* pSubQuestInfo ;													// 서브 퀘스트 정보를 받을 포인터를 선언한다.

	SUBQUEST* pSubQuest ;															// 서브 퀘스트를 받을 포인터를 선언한다.

	PTRLISTPOS pos = NULL ;															// 위치 정보를 담을 포인터를 선언하고 null 처리한다.
	pos = m_QuestTreeList.GetHeadPosition() ;										// 퀘스트 리스트를 헤드로 세팅하고, 위치값을 세팅한다.

	while(pos)																		// 위치 정보가 유효할 동안 while문을 돌린다.
	{
		pCheckIdx = NULL ;															// 인덱스를 받을 포인터를 null 처리한다.
		pCheckIdx = (DWORD*)m_QuestTreeList.GetNext(pos) ;							// 위치에 해당하는 인덱스를 받고, 위치를 다음으로 세팅한다.

		if( !pCheckIdx ) continue ;													// 인덱스 정보가 유효하지 않으면, continue 한다.

		pQuest = NULL ;																// 퀘스트 정보를 받을 포인터를 null 처리한다.
		pQuest = QUESTMGR->GetQuest(*pCheckIdx) ;									// 현재 인덱스로 퀘스트 매니져에서 퀘스트 정보를 받는다.

		if( !pQuest ) continue ;													// 퀘스트 정보가 유효하지 않으면, continue 한다.

		if( pQuest->IsQuestComplete() || !pQuest->GetSubQuest(0) )					// 만약 완료했거나, 서브 퀘스트 0번 정보가 없으면,
		{
			DeleteQuest(*pCheckIdx) ;												// 퀘스트를 트리에서 삭제한다.
		}

		AddMainString( *pCheckIdx, 0 ) ;											// 퀘스트 제목을 추가한다.

		for( int count = 0 ; count < MAX_SUBQUEST ; ++count )						// 한 퀘스트가 가질 수 있는 최대 수 만큼 for문을 돌린다.
		{
			pSubQuestInfo = NULL ;													// 서브 퀘스트 정보를 받을 포인터를 null 처리한다.
			pSubQuestInfo = pQuest->GetSubQuestInfoArray(count) ;					// 카운트에 해당하는 서브 퀘스트 정보를 받는다.

			if( !pSubQuestInfo ) continue ;											// 서브 퀘스트 정보가 유효하지 않으면, continue 한다.

			pSubQuest = NULL ;														// 서브 퀘스트 정보를 받을 포인터를 null처리 한다.
			pSubQuest = pQuest->GetSubQuest(count) ;								// 카운트에 해당하는 서브 퀘스트 정보를 받는다.

			if(!pSubQuest) continue ;												// 서브 퀘스트 정보가 유효하지 않으면, continue 한다.

			int nSubQuestType = e_SQT_NORMAL ;										// 퀘스트 타입을 담을 변수를 선언하고, 기본으로 세팅한다.

			if( pSubQuest->dwMaxCount > 0 )											// 목표 카운트가 있으면,
			{
				nSubQuestType = e_SQT_HUNT ;										// 사냥 퀘스트이다.
			}
			else																	// 목표 카운트가 없으면,
			{
				if( pSubQuest->dwData > 0 )											// 현재 카운트가 있으면,
				{
					nSubQuestType = e_SQT_COLLECTION ;								// 수집 퀘스트이다.
				}
				else																// 현재 카운트가 없으면,
				{
					nSubQuestType = e_SQT_COMPLETED ;								// 완료 퀘스트 이다.
				}
			}

			AddSubString( *pCheckIdx, count, nSubQuestType ) ;						// 서브 퀘스트 스트링을 추가한다.
		}
	}
}

void CQuestQuickViewDialog::AddMainString(DWORD dwQuestIdx, DWORD dwSubQuestIdx)	// 퀘스트 제목을 추가하는 함수.
{
	CQuestString* pString = NULL ;													// 퀘스트 스트링 정보를 받을 포인터를 선언하고 null 처리를 한다.
	pString = QUESTMGR->GetQuestString(dwQuestIdx, dwSubQuestIdx) ;					// 퀘스트 인덱스로, 퀘스트 스트링 정보를 받아온다.

	if( !pString ) return ;															// 퀘스트 스트링 정보가 유효하지 않으면, return 처리 한다.	

	char tempBuf[256] = {0,} ;														// 임시 버퍼를 선언한다.
	GetTitleString( tempBuf, pString ) ;											// 퀘스트 스트링의 제목을 받는다.

	if( strlen(tempBuf) <= 4 ) return ;												// 제목이 유효하지 않으면 return 처리한다.

	m_pViewList->AddItem( tempBuf, RGB(0, 255, 255) ) ;								// 퀘스트 제목을 추가한다.
	m_pViewList->SetExtendReduction(LD_EXTEND) ;									// 리스트 다이얼로그를 확장한다.
}

void CQuestQuickViewDialog::AddSubString(DWORD dwQuestIdx, DWORD dwSubQuestIdx, int nType)		// 서브 퀘스트 제목을 추가하는 함수.
{
	CQuestString* pString = NULL ;													// 퀘스트 스트링 정보를 받을 포인터를 선언하고 null 처리를 한다.
	pString = QUESTMGR->GetQuestString(dwQuestIdx, dwSubQuestIdx) ;					// 퀘스트 인덱스로, 퀘스트 스트링 정보를 받아온다.

	if( !pString ) return ;															// 퀘스트 스트링 정보가 유효하지 않으면, return 처리 한다.	

	CQuest* pQuest = NULL ;															// 퀘스트 정보를 받을 포인터를 선언하고 null 처리한다.
	pQuest = QUESTMGR->GetQuest(dwQuestIdx) ;										// 인덱스에 해당하는 퀘스트 정보를 받는다.

	if( !pQuest ) return ;															// 퀘스트 정보가 유효하지 않으면, return 처리 한다.

	SUBQUEST* pSubQuest ;															// 서브 퀘스트 정보를 받을 포인터를 선언한다.
	pSubQuest = pQuest->GetSubQuest(dwSubQuestIdx) ;								// 서브 퀘스트 정보를 받는다.

	if( !pSubQuest ) return ;														// 서브 퀘스트 정보가 유효하지 않으면, return 처리 한다.

	CQuestString* pQuestString ;														// 퀘스트 스트링 정보를 받을 포인터를 선언한다.

	pQuestString = NULL ;														// 퀘스트 스트링 정보를 받을 포인터를 null 처리를 한다.
	pQuestString = QUESTMGR->GetQuestString(dwQuestIdx, 0) ;					// 현재 퀘스트의 제목 스트링을 받아온다.

	if( !pQuestString ) return ;

	CQuestInfo* pQuestInfo ;															// 퀘스트 정보를 받을 포인터를 선언한다.

	int nSubQuestCount = 0 ;
	pQuestInfo = QUESTMGR->GetQuestInfo(pQuestString->GetQuestID()) ;				// 퀘스트 번호로 퀘스트 정보를 받는다.

	if( !pQuestInfo ) return ;

	char tempBuf[256] = {0,} ;														// 임시 버퍼를 선언한다.
	strcat(tempBuf, "	- ") ;														// 하이픈을 추가한다.
	GetTitleString( tempBuf, pString ) ;											// 퀘스트 스트링의 제목을 받는다.

	if( strlen(tempBuf) <= 4 ) return ;												// 제목이 유효하지 않으면 return 처리한다.

	char typeBuf[32] = {0, } ;														// 타입에 따른 처리를 위해 타입 버퍼를 선언한다.

	switch(nType)																	// 서브 퀘스트 타입을 확인한다.
	{
	case e_SQT_NORMAL : break ;														// 기본 타입인경우,
	case e_SQT_HUNT :																// 사냥 타입인 경우,
		{
			sprintf(typeBuf, "(%d/%d)", pSubQuest->dwData, pSubQuest->dwMaxCount) ; // 현재/목표 카운트를 추가해 준다.
		}
		break ;

	case e_SQT_COLLECTION :															// 수집 타입인 경우,
		{
			sprintf(typeBuf, "(%d)", pSubQuest->dwData) ;							// 핸재 카운트를 추가해 준다.
		}
		break ;

	case e_SQT_COMPLETED :															// 완료 타입인 경우,
		{
			//sprintf(typeBuf, "(완료)") ;											// 완료를 추가해 준다.
		}
		break ;
	}

	strcat(tempBuf, typeBuf) ;

	nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// 서브 퀘스트 수를 받는다.

	if( nSubQuestCount-1 == pQuest->GetCurSubQuestIdx() )
	{
		strcat(tempBuf, CHATMGR->GetChatMsg(1463)) ;
	}

	m_pViewList->AddItem( tempBuf, RGB(255, 255, 255) ) ;							// 퀘스트 제목을 추가한다.
	m_pViewList->SetExtendReduction(LD_EXTEND) ;									// 리스트 다이얼로그를 확장한다.
}

void CQuestQuickViewDialog::GetTitleString( char* pBuf, CQuestString* pQuestString )// 퀘스트 스트링의 제목 스트링을 얻는 함수.
{
	if( !pQuestString ) return ;													// 퀘스트 스트링 정보가 유효하지 않으면, null을 return 한다.

	if( !pBuf ) return ;

	cPtrList* pList = NULL ;
	pList = pQuestString->GetTitle() ;

	if( !pList ) return ;

	ITEM* pItem ;

	PTRLISTPOS pos = NULL ;
	pos = pList->GetHeadPosition() ;

	while( pos )
	{
		pItem = NULL ;
		pItem = (ITEM*)pList->GetNext(pos) ;

		if( !pItem ) continue ;

		strcat(pBuf, pItem->string) ;
	}
}














































































//#include "stdafx.h"																	// 표준 시스템과, 프로젝트가 지정한, 자주쓰는 해더들을 모은 해더파일을 불러온다.
//
//#include ".\questquickviewdialog.h"
//
//#include "./Input/Mouse.h"
//
//#include "WindowIDEnum.h"															// 윈도우 아이디 이넘 헤더를 불러온다.
//
//#include "./Interface/cWindowManager.h"												// 윈도우 매니져 클래스 헤더를 불러온다.
//#include "./Interface/GameNotifyManager.h"											// 게임 공지 매니져 클래스 헤더를 불러온다.
//#include "QuestManager.h"															// 퀘스트 매니져 클래스 헤더를 불러온다.
//
//#include "./Interface/cListDialog.h"												// 리스트 다이얼로그 클래스 헤더를 불러온다.
//#include "./Interface/cStatic.h"
//
//#include "cMsgBox.h"																// 메시지 박스 클래스 헤더를 불러온다.
//#include "QuestDialog.h"															// 퀘스트 다이얼로그 클래스 헤더를 불러온다.
//
//#include "Quest.h"
//
//#include "..\[CC]Quest\QuestInfo.h"													// 퀘스트 정보 클래스 헤더를 불러온다.
//#include "..\[CC]Quest\QuestString.h"												// 퀘스트 스트링 클래스 헤더를 불러온다.
//
//#include "UserInfoManager.h"
//
//CQuestQuickViewDialog::CQuestQuickViewDialog(void)									// 퀘스트 알림이 다이얼로그 생성자 함수.
//{
//	m_type = WT_QUESTQUICKVIEWDIALOG ;												// 윈도우 타입을 알림이 다이얼로그로 세팅한다.
//
//	m_byAlpha = 0 ;																	// 윈도우 Alpha값을 0으로 세팅한다.
//
//	m_pViewList = NULL ;															// 트리 리스트 다이얼로그 포인터를 null로 세팅한다.
//}
//
//CQuestQuickViewDialog::~CQuestQuickViewDialog(void)									// 퀘스트 알림이 다이얼로그 소멸자 함수.
//{
//	Release() ;
//}
//
//void CQuestQuickViewDialog::Release()
//{
//	QuestTree* pTree ;																// 트리 정보를 받을 포인터를 선언한다.
//	CQuestString* pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 선언하고 null처리를 한다.
//
//	PTRLISTPOS pos = NULL ;															// 리스트 내의 위치를 담을 포인터를 선언하고 null처리를 한다.
//	PTRLISTPOS treePos = NULL ;														// 트리의 리스트 내의 위치를 담을 포인터를 선언하고 null처리를 한다.
//
//	pos = m_QuestTreeList.GetHeadPosition() ;										// 위치 포인터를 퀘스트의 헤드로 세팅한다.
//
//	while(pos)																		// 리스트의 다음 요소가 유효할 동안 while문을 돌린다.
//	{
//		pTree = NULL ;																// 트리 정보를 받을 포인터를 null 처리를 한다.
//		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// 위치 정보에 해당하는 트리를 받는다.
//
//		if( pTree )																	// 트리 정보가 유효하다면,
//		{
//			treePos = NULL ;														// 트리의 리스트에서 위치 정보를 받을 포인터를 선언하고 null 처리를 한다.	
//			treePos = pTree->list.GetHeadPosition() ;								// 트리의 리스트를 헤드로 세팅한다.
//
//			while(treePos)															// 위치 정보가 유효할 동안 while문을 돌린다.
//			{
//				pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
//				pQuestString = (CQuestString*)pTree->list.GetNext(treePos) ;		// 0번째 위치에 해당하는 퀘스트 스트링을 받는다.
//
//				if( pQuestString )													// 퀘스트 스트링 정보가 유효하면,
//				{
//					pTree->list.Remove(pQuestString) ;
//
//					delete pQuestString ;
//					pQuestString = NULL ;
//				}
//			}
//
//			pTree->list.RemoveAll() ;												// 리스트를 모두 비운다.
//
//			m_QuestTreeList.Remove(pTree) ;											// 퀘스트 리스트에서 트리를 삭제한다.
//
//			delete pTree ;
//			pTree = NULL ;
//		}
//	}
//
//	m_QuestTreeList.RemoveAll() ;													// 퀘스트 리스트를 모두 비운다.
//}
//
////CQuestQuickViewDialog::~CQuestQuickViewDialog(void)									// 퀘스트 알림이 다이얼로그 소멸자 함수.
////{
////	QuestTree* pTree ;																// 트리 정보를 받을 포인터를 선언한다.
////	CQuestString* pQuestString ;													// 스트링 정보를 받을 포인터를 선언한다.
////
////	PTRLISTPOS pos = NULL ;															// 리스트 내의 위치를 담을 포인터를 선언하고 null처리를 한다.
////	PTRLISTPOS treePos = NULL ;														// 트리의 리스트 내의 위치를 담을 포인터를 선언하고 null처리를 한다.
////
////	pos = m_QuestTreeList.GetHeadPosition() ;										// 위치 포인터를 퀘스트의 헤드로 세팅한다.
////
////	while(pos)																		// 리스트의 다음 요소가 유효할 동안 while문을 돌린다.
////	{
////		pTree = NULL ;																// 트리 정보를 받을 포인터를 null 처리를 한다.
////		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// 위치 정보에 해당하는 트리를 받는다.
////
////		if( pTree )																	// 트리 정보가 유효하다면,
////		{
////			treePos = NULL ;														// 트리의 리스트에서 위치 정보를 받을 포인터를 선언하고 null 처리를 한다.	
////			treePos = pTree->list.GetHeadPosition() ;								// 트리의 리스트를 헤드로 세팅한다.
////
////			while(treePos)															// 위치 정보가 유효할 동안 while문을 돌린다.
////			{
////				pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
////				pQuestString = (CQuestString*)pTree->list.GetNext(treePos) ;		// 0번째 위치에 해당하는 퀘스트 스트링을 받는다.
////
////				if( pQuestString )													// 퀘스트 스트링 정보가 유효하면,
////				{
////					pTree->list.Remove(pQuestString) ;
////
////					if( pQuestString )
////					{
////						delete pQuestString ;
////					}
////				}
////			}
////
////			pTree->list.RemoveAll() ;												// 리스트를 모두 비운다.
////
////			m_QuestTreeList.Remove(pTree) ;											// 퀘스트 리스트에서 트리를 삭제한다.
////
////			if( pTree )
////			{
////				delete pTree ;
////			}
////		}
////	}
////
////	m_QuestTreeList.RemoveAll() ;													// 퀘스트 리스트를 모두 비운다.
////}
//
//void CQuestQuickViewDialog::Linking()												// 링크 함수.
//{
//	//m_pTitle = (cStatic*)GetWindowForID(QUE_QUICKVIEWTITLE) ;
//	m_pViewList = (cListDialog*)GetWindowForID(QUE_QUICKVIEWLIST) ;					// 트리 리스트 다이얼로그를 링크한다.
//	m_pViewList->SetStyle( LDS_FONTSHADOW ) ;										// 리스트 다이얼로그 스타일을 세팅한다.
//}
//
//DWORD CQuestQuickViewDialog::ActionEvent(CMouse * mouseInfo)						// 이벤트 처리 함수.
//{
//	DWORD we = WE_NULL ;															// 윈도우 이벤트를 받을 변수를 선언하고 null처리를 한다.
//
//	we = cDialog::ActionEvent(mouseInfo) ;											// 상위 윈도우의 이벤트를 받는다.
//
//	int nXpos = mouseInfo->GetMouseX() ;											// 마우스 X좌표를 받는다.
//	int nYpos = mouseInfo->GetMouseY() ;											// 마우스 Y좌표를 받는다.
//
//	//if( m_pTitle->PtInWindow(nXpos, nYpos) )													// 마우스가 윈도우 영역 내에 있다면,
//	//{
//	//	if( m_byAlpha < 230 )														// 알파 값이 230보다 작다면,
//	//	{
//	//		SetAlpha(m_byAlpha += 10) ;												// 알파 값을 10씩 증가한다.
//	//	}
//	//	else																		// 알파 값이 230 이상이면,
//	//	{
//	//		SetAlpha(255) ;															// 알파 값을 255로 세팅한다.
//	//	}
//	//}
//	//else																			// 마우스가 윈도우 영역 밖이면,
//	//{
//	//	if( m_byAlpha > 20 )														// 알파 값이 20보다 크면,
//	//	{
//	//		SetAlpha(m_byAlpha -= 10) ;												// 알파 값을 10씩 감소한다.
//	//	}
//	//	else																		// 알파 값이 20이하면,
//	//	{
//	//		SetAlpha(0) ;															// 알파 값을 0으로 세팅한다.
//	//	}
//	//}
//
//	//if( m_pViewList->PtInWindow( nXpos, nYpos )	)									// 알림이 리스트 상에 마우스가 있으면,
//	//{
//	//	int nSelectedLine = -1 ;													// 선택 된 라인을 받을 변수를 선언하고 -1로 세팅한다.
//
//	//	if( we & WE_LBTNCLICK )														// 그리고 마우스 왼쪽 버튼을 클릭했다면,
//	//	{
//	//		nSelectedLine = m_pViewList->GetSelectRowIdx() ;						// 알림이 리스트로 부터 선택 된 라인을 받는다.
//
//	//		if( nSelectedLine != -1 )												// 선택 된 라인의 인덱스가 -1이 아니라면,
//	//		{
//	//			CheckTreeState(nSelectedLine) ;										// 트리 상태를 체크하는 함수를 호출한다.
//	//		}
//	//	}
//	//}
//
//	return we ;																		// 윈도우 이벤트를 리턴한다.
//}
//
//BOOL CQuestQuickViewDialog::CheckSameQuest(DWORD dwQuestIdx)						// 퀘스트 리스트 내에 같은 퀘스트가 있는지 여부를 반환하는 함수.
//{
//	BOOL bResult = FALSE ;															// 결과 값을 담을 변수를 선언하고 false로 세팅한다.
//
//	QuestTree* pTree ;																// 트리 정보를 받을 포인터를 선언한다.
//
//	PTRLISTPOS pos = NULL ;															// 리스트 내의 위치를 담을 포인터를 선언하고 null처리를 한다.
//	PTRLISTPOS treePos = NULL ;														// 트리의 리스트 내의 위치를 담을 포인터를 선언하고 null처리를 한다.
//
//	pos = m_QuestTreeList.GetHeadPosition() ;										// 위치 포인터를 퀘스트의 헤드로 세팅한다.
//
//	int nCount = 0 ;																// 퀘스트 삭제시 필요한 카운트를 선언하고 0으로 세팅한다.
//
//	while(pos)																		// 리스트의 다음 요소가 유효할 동안 while문을 돌린다.
//	{
//		pTree = NULL ;																// 트리 정보를 받을 포인터를 null 처리를 한다.
//		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// 위치 정보에 해당하는 트리를 받는다.
//
//		if( pTree )																	// 트리 정보가 유효하다면,
//		{
//			if( pTree->QuestIdx == dwQuestIdx )										// 트리의 퀘스트 번호와, 인자로 넘어온 퀘스트 인덱스가 같으면,
//			{
//				bResult = TRUE ;													// 결과 값을 true로 세팅한다.
//
//				//QUESTMGR->DeleteQuestIDToQuickView(nCount) ;						// 퀘스트 매니져에 저장한 퀘스트 아이디를 삭제한다.
//
//				return bResult ;													// 결과 값을 리턴한다.
//			}
//
//			++nCount ;																// 카운트 값을 증가한다.
//		}
//	}
//
//	return bResult ;																// 결과 값을 리턴한다.
//}
//
//void CQuestQuickViewDialog::RegistQuest(DWORD dwQuestIdx)							// 퀘스트를 추가하는 함수.
//{
//	CQuestString* pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 선언하고 null처리를 한다.
//
//	CQuest* pQuest = QUESTMGR->GetQuest( dwQuestIdx );							// 퀘스트 정보를 받는다.
//
//	if( !pQuest ) return ;															// 퀘스트 정보가 유효하지 않으면, return 처리 한다.
//
//	CSubQuestInfo* pDescSubQuestInfo ;												// 서브 퀘스트 정보를 받을 포인터를 선언한다.
//
//    if( CheckSameQuest(dwQuestIdx) )												// 퀘스트 리스트에 인자로 넘어온 퀘스트와 같은 퀘스트가 있다면,
//	{
//		DeleteQuest(dwQuestIdx) ;													// 리스트에서 퀘스트를 삭제한다.
//
//		return ;																	// 리턴 처리를 한다.
//	}
//
//	int nTreeCount = 0 ;															// 퀘스트 트리 카운트를 담을 변수를 선언하고 0으로 세팅한다.
//
//	nTreeCount = m_QuestTreeList.GetCount() ;										// 퀘스트를 담는 리스트의 카운트를 받는다.	
//
//	if( nTreeCount >= MAX_QUESTTREECOUNT )											// 트리 카운트가 최대 트리 수 이상이면,
//	{
//		WINDOWMGR->MsgBox( MBI_QUESTQUICKVIEW_FULL, MBT_OK, "알림이 등록은 5개 까지만 할 수 있습니다.") ;	// 5개 까지만 등록이 가능하다고 메시지 창을 띄운다.
//	}
//	else																			// 트리 개수가 5개 미만이면,
//	{
//		QuestTree* pTree = new QuestTree ;											// 트리를 생성한다.
//
//		CQuestInfo* pQuestInfo = NULL ;												// 퀘스트 정보 포인터를 선언하고 null처리를 한다.
//		pQuestInfo = QUESTMGR->GetQuestInfo(dwQuestIdx) ;							// 퀘스트 매니져로 부터, 인자로 넘어온 퀘스트 번호로 퀘스트 정보를 받는다.
//
//		if( !pQuestInfo )															// 퀘스트 정보가 유효하지 않다면,
//		{
//			ASSERTMSG(0, "퀘스트 알림이 등록 중 퀘스트 정보를 받아오지 못했습니다.!") ;		// assert 에러 처리를 한다.
//			return ;																// 리턴 처리를 한다.
//		}
//
//		int nSubQuestCount = 0 ;													// 서브 퀘스트 카운트를 받을 변수를 선언하고 0으로 세팅한다.
//		nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// 퀘스트 정보로 부터 서브 퀘스트 카운트를 받는다.
//		
//		for(int count = 0 ; count < nSubQuestCount ; ++count )						// 서브 퀘스트 카운트 만큼 for문을 돌린다.
//		{
//			if( count != 0 )
//			{
//				pDescSubQuestInfo = NULL ;													// 서브 퀘스트 정보를 받을 포인터를 null처리 한다.
//				pDescSubQuestInfo = pQuest->GetSubQuestInfoArray(count) ;					// 카운트에 해당하는 서브 퀘스트를 받는다.
//
//				if( !pDescSubQuestInfo ) continue ;											// 서브 퀘스트 정보가 유효하지 않으면 continue한다.
//			}
//
//			pQuestString = NULL ;													// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
//			pQuestString = QUESTMGR->GetQuestString(dwQuestIdx, count) ;			// 퀘스트 인덱스와, 서브 퀘스트 인덱스로 퀘스트 스트링을 받는다.
//            
//			if( pQuestString )														// 퀘스트 스트링 정보가 유효하다면,
//			{
//				CQuestString* pNewQuestString = new CQuestString ;
//
//				DWORD dwMainQuestIdx = pQuestString->GetQuestID() ;
//				DWORD dwSubQuestIdx  = pQuestString->GetSubID() ;
//
//				pNewQuestString->SetQuestID( dwMainQuestIdx ) ;
//				pNewQuestString->SetSubID( dwSubQuestIdx ) ;
//				pNewQuestString->SetIndex( dwMainQuestIdx, dwSubQuestIdx ) ;
//
//				pNewQuestString->SetComplete( pQuestString->IsComplete() ) ;
//
//				pNewQuestString->SetCount( pQuestString->GetCount() ) ;
//				pNewQuestString->SetTotalCount( pQuestString->GetTotalCount() ) ;
//
//
//				ITEM* pItem = NULL ;
//
//				cPtrList* titleList = NULL ;
//				titleList = pQuestString->GetTitle() ;
//
//				if( titleList )
//				{
//					PTRLISTPOS titlePos = NULL ;
//					titlePos = titleList->GetHeadPosition() ;
//
//					while(titlePos)
//					{
//						pItem = NULL ;
//						pItem = (ITEM*)titleList->GetNext(titlePos) ;
//
//						if( pItem )
//						{
//							ITEM* pNewItem = new ITEM ;
//
//							//memcpy(pNewItem, pItem, sizeof(ITEM)) ;
//
//							memcpy(pNewItem->string, pItem->string, sizeof(pItem->string)) ;
//							pNewItem->line		= pItem->line ;
//							pNewItem->nAlpha	= pItem->nAlpha ;
//							pNewItem->nFontIdx	= pItem->nFontIdx ;
//							pNewItem->rgb		= pItem->rgb ;
//
//							pNewQuestString->GetTitle()->AddTail(pNewItem) ;
//						}
//					}
//				}
//
//				//cPtrList* descList = NULL ;
//				//descList = pQuestString->GetDesc() ;
//
//				//if( descList )
//				//{
//				//	PTRLISTPOS descPos = NULL ;
//				//	descPos = descList->GetHeadPosition() ;
//
//				//	while(descPos)
//				//	{
//				//		pItem = NULL ;
//				//		pItem = (ITEM*)descList->GetNext(descPos) ;
//
//				//		if( pItem )
//				//		{
//				//			ITEM* pNewItem = new ITEM ;
//
//				//			//memcpy(pNewItem, pItem, sizeof(ITEM)) ;
//
//				//			memcpy(pNewItem->string, pItem->string, sizeof(pItem->string)) ;
//				//			pNewItem->line		= pItem->line ;
//				//			pNewItem->nAlpha	= pItem->nAlpha ;
//				//			pNewItem->nFontIdx	= pItem->nFontIdx ;
//				//			pNewItem->rgb		= pItem->rgb ;
//
//				//			pNewQuestString->GetDesc()->AddTail(pNewItem) ;
//				//		}
//				//	}
//				//}
//
//				pNewQuestString->SetTreeTitle( pQuestString->IsTreeTitle() ) ;
//
//				cPtrList* pList = pNewQuestString->GetTitle() ;						// 퀘스트 스트링의 타이틀 리스트를 받는다.
//
//				PTRLISTPOS titlePos = NULL ;										// 타이틀 리스트 상의 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
//				titlePos = pList->GetHeadPosition() ;								// 타이틀 리스트를 헤드로 세팅하고 위치 정보를 받는다.
//
//				ITEM* pQString = NULL ;												// Q스트링을 받을 포인터를 선언하고 null처리를 한다.
//
//				while(titlePos)														// 위치 정보가 유효할 동안 while문을 돌린다.
//				{
//					pQString = NULL ;												// Q스트링 포인터를 null처리를 한다.
//					pQString = (ITEM*)pList->GetNext(titlePos) ;					// 위치에 따른 Q스트링을 받는다.
//
//					if( pQString )													// Q스트링 정보가 유효하다면,
//					{
//						pNewQuestString->SetTitleStr( pQString->string) ;			// Q스트링의 문자열을 받아서 새로운 퀘스트 스트링에 타이틀로 세팅한다.
//
//						break ;
//					}
//				}
//
//				pTree->list.AddTail(pNewQuestString) ;								// 트리의 리스트에 퀘스트 스트링을 추가한다.
//
//			}
//		}
//
//		pTree->State = eQTree_Open ;												// 트리 상태를 열린 상태로 세팅한다.
//		pTree->QuestIdx = dwQuestIdx ;												// 트리에 퀘스트 인덱스를 세팅한다.
//		m_QuestTreeList.AddTail(pTree) ;											// 퀘스트 리스트에 트리를 추가한다.
//
//		//QUESTMGR->RegistQuestIDToQuickView(m_QuestTreeList.GetCount()-1, dwQuestIdx) ;	// 퀘스트 매니져에, 알림이에 등록 된 퀘스트 아이디를 저장한다.
//	}
//
//	ResetQuickViewDialog() ;														// 퀵 뷰를 새로고침한다.
//}
//
////void CQuestQuickViewDialog::RegistQuest(DWORD dwQuestIdx)							// 퀘스트를 추가하는 함수.
////{
////    if( CheckSameQuest(dwQuestIdx) )												// 퀘스트 리스트에 인자로 넘어온 퀘스트와 같은 퀘스트가 있다면,
////	{
////		DeleteQuest(dwQuestIdx) ;													// 리스트에서 퀘스트를 삭제한다.
////
////		return ;																	// 리턴 처리를 한다.
////	}
////
////	int nTreeCount = 0 ;															// 퀘스트 트리 카운트를 담을 변수를 선언하고 0으로 세팅한다.
////
////	nTreeCount = m_QuestTreeList.GetCount() ;										// 퀘스트를 담는 리스트의 카운트를 받는다.	
////
////	if( nTreeCount >= MAX_QUESTTREECOUNT )											// 트리 카운트가 최대 트리 수 이상이면,
////	{
////		WINDOWMGR->MsgBox( MBI_QUESTQUICKVIEW_FULL, MBT_OK, "알림이 등록은 5개 까지만 할 수 있습니다.") ;	// 5개 까지만 등록이 가능하다고 메시지 창을 띄운다.
////	}
////	else																			// 트리 개수가 5개 미만이면,
////	{
////		QuestTree* pTree = new QuestTree ;											// 트리를 생성한다.
////
////		CQuestInfo* pQuestInfo = NULL ;												// 퀘스트 정보 포인터를 선언하고 null처리를 한다.
////		pQuestInfo = QUESTMGR->GetQuestInfo(dwQuestIdx) ;							// 퀘스트 매니져로 부터, 인자로 넘어온 퀘스트 번호로 퀘스트 정보를 받는다.
////
////		if( !pQuestInfo )															// 퀘스트 정보가 유효하지 않다면,
////		{
////			ASSERTMSG(0, "퀘스트 알림이 등록 중 퀘스트 정보를 받아오지 못했습니다.!") ;		// assert 에러 처리를 한다.
////			return ;																// 리턴 처리를 한다.
////		}
////
////		int nSubQuestCount = 0 ;													// 서브 퀘스트 카운트를 받을 변수를 선언하고 0으로 세팅한다.
////		nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// 퀘스트 정보로 부터 서브 퀘스트 카운트를 받는다.
////
////		CQuestString* pQuestString ;												// 퀘스트 스트링 정보를 받을 포인터를 선언한다.
////		
////		for(int count = 0 ; count < nSubQuestCount ; ++count )						// 서브 퀘스트 카운트 만큼 for문을 돌린다.
////		{
////			pQuestString = NULL ;													// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
////			pQuestString = QUESTMGR->GetQuestString(dwQuestIdx, count) ;			// 퀘스트 인덱스와, 서브 퀘스트 인덱스로 퀘스트 스트링을 받는다.
////            
////			if( pQuestString )														// 퀘스트 스트링 정보가 유효하다면,
////			{
////				CQuestString* pNewString = new CQuestString ;						// 퀘스트 스트링을 생성한다.
////				memcpy(pNewString, pQuestString, sizeof(CQuestString)) ;			// 새로 생성한 퀘스트 스트링에, 현재의 퀘스트 스트링을 복사한다.
////
////				cPtrList* pList = pQuestString->GetTitle() ;						// 퀘스트 스트링의 타이틀 리스트를 받는다.
////
////				PTRLISTPOS titlePos = NULL ;										// 타이틀 리스트 상의 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
////				titlePos = pList->GetHeadPosition() ;								// 타이틀 리스트를 헤드로 세팅하고 위치 정보를 받는다.
////
////				QString* pQString = NULL ;											// Q스트링을 받을 포인터를 선언하고 null처리를 한다.
////
////				while(titlePos)														// 위치 정보가 유효할 동안 while문을 돌린다.
////				{
////					pQString = NULL ;												// Q스트링 포인터를 null처리를 한다.
////					pQString = (QString*)pList->GetNext(titlePos) ;					// 위치에 따른 Q스트링을 받는다.
////
////					if( pQString )													// Q스트링 정보가 유효하다면,
////					{
////						pNewString->SetTitleStr( pQString->Str ) ;					// Q스트링의 문자열을 받아서 새로운 퀘스트 스트링에 타이틀로 세팅한다.
////					}
////				}
////
////				pTree->list.AddTail(pNewString) ;									// 트리의 리스트에 퀘스트 스트링을 추가한다.
////
////			}
////		}
////
////		pTree->State = eQTree_Open ;												// 트리 상태를 열린 상태로 세팅한다.
////		pTree->QuestIdx = dwQuestIdx ;												// 트리에 퀘스트 인덱스를 세팅한다.
////		m_QuestTreeList.AddTail(pTree) ;											// 퀘스트 리스트에 트리를 추가한다.
////
////		QUESTMGR->RegistQuestIDToQuickView(m_QuestTreeList.GetCount()-1, dwQuestIdx) ;	// 퀘스트 매니져에, 알림이에 등록 된 퀘스트 아이디를 저장한다.
////	}
////
////	ResetQuickViewDialog() ;														// 퀵 뷰를 새로고침한다.
////}
//
//void CQuestQuickViewDialog::DeleteQuest(DWORD dwQuestIdx)							// 인자로 넘어온 퀘스트의 트리를 삭제하는 함수.
//{
//	QuestTree* pTree ;																// 트리 정보를 받을 포인터를 선언한다.
//	CQuestString* pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 선언하고 null처리를 한다.
//
//	PTRLISTPOS pos = NULL ;															// 리스트 내의 위치를 담을 포인터를 선언하고 null처리를 한다.
//	PTRLISTPOS treePos = NULL ;														// 트리의 리스트 내의 위치를 담을 포인터를 선언하고 null처리를 한다.
//
//	pos = m_QuestTreeList.GetHeadPosition() ;										// 위치 포인터를 퀘스트의 헤드로 세팅한다.
//
//	while(pos)																		// 리스트의 다음 요소가 유효할 동안 while문을 돌린다.
//	{
//		pTree = NULL ;																// 트리 정보를 받을 포인터를 null 처리를 한다.
//		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// 위치 정보에 해당하는 트리를 받는다.
//
//		if( pTree )																	// 트리 정보가 유효하다면,
//		{
//			if( pTree->QuestIdx == dwQuestIdx )										// 트리의 퀘스트 인덱스와, 인자로 넘어온 퀘스트 인덱스가 같으면,
//			{
//				treePos = NULL ;													// 트리의 리스트에서 위치 정보를 받을 포인터를 선언하고 null 처리를 한다.	
//				treePos = pTree->list.GetHeadPosition() ;							// 트리의 리스트를 헤드로 세팅한다.
//
//				while(treePos)														// 위치 정보가 유효할 동안 while문을 돌린다.
//				{
//					pQuestString = NULL ;											// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
//					pQuestString = (CQuestString*)pTree->list.GetNext(treePos) ;	// 0번째 위치에 해당하는 퀘스트 스트링을 받는다.
//
//					if( pQuestString )												// 퀘스트 스트링 정보가 유효하면,
//					{
//						pTree->list.Remove(pQuestString) ;							// 트리의 리스트에서 퀘스트 스트링을 삭제한다.
//
//						delete pQuestString ;
//						pQuestString = NULL ;
//					}
//				}
//
//				pTree->list.RemoveAll() ;											// 리스트를 모두 비운다.
//
//				m_QuestTreeList.Remove(pTree) ;										// 퀘스트 리스트에서 트리를 삭제한다.
//
//				delete pTree ;
//				pTree = NULL ;
//			}
//		}
//	}
//
//	ResetQuickViewDialog() ;														// 퀵 뷰를 새로고침한다.
//}
//
////void CQuestQuickViewDialog::DeleteQuest(DWORD dwQuestIdx)							// 인자로 넘어온 퀘스트의 트리를 삭제하는 함수.
////{
////	QuestTree* pTree ;																// 트리 정보를 받을 포인터를 선언한다.
////	CQuestString* pQuestString ;													// 스트링 정보를 받을 포인터를 선언한다.
////
////	PTRLISTPOS pos = NULL ;															// 리스트 내의 위치를 담을 포인터를 선언하고 null처리를 한다.
////	PTRLISTPOS treePos = NULL ;														// 트리의 리스트 내의 위치를 담을 포인터를 선언하고 null처리를 한다.
////
////	pos = m_QuestTreeList.GetHeadPosition() ;										// 위치 포인터를 퀘스트의 헤드로 세팅한다.
////
////	while(pos)																		// 리스트의 다음 요소가 유효할 동안 while문을 돌린다.
////	{
////		pTree = NULL ;																// 트리 정보를 받을 포인터를 null 처리를 한다.
////		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// 위치 정보에 해당하는 트리를 받는다.
////
////		if( pTree )																	// 트리 정보가 유효하다면,
////		{
////			if( pTree->QuestIdx == dwQuestIdx )										// 트리의 퀘스트 인덱스와, 인자로 넘어온 퀘스트 인덱스가 같으면,
////			{
////				treePos = NULL ;													// 트리의 리스트에서 위치 정보를 받을 포인터를 선언하고 null 처리를 한다.	
////				treePos = pTree->list.GetHeadPosition() ;							// 트리의 리스트를 헤드로 세팅한다.
////
////				while(treePos)														// 위치 정보가 유효할 동안 while문을 돌린다.
////				{
////					pQuestString = NULL ;											// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
////					pQuestString = (CQuestString*)pTree->list.GetNext(treePos) ;	// 0번째 위치에 해당하는 퀘스트 스트링을 받는다.
////
////					if( pQuestString )												// 퀘스트 스트링 정보가 유효하면,
////					{
////						pTree->list.Remove(pQuestString) ;							// 트리의 리스트에서 퀘스트 스트링을 삭제한다.
////					}
////				}
////
////				pTree->list.RemoveAll() ;											// 리스트를 모두 비운다.
////
////				m_QuestTreeList.Remove(pTree) ;										// 퀘스트 리스트에서 트리를 삭제한다.
////			}
////
////			pTree = NULL ;															// 트리 포인터를 null처리를 한다.
////		}
////	}
////
////	ResetQuickViewDialog() ;														// 퀵 뷰를 새로고침한다.
////}
//
//void CQuestQuickViewDialog::EndQuest(SEND_QUEST_IDX* msg)							// 알림이 창에 있는 퀘스트를 종료하는 함수.
//{
//	WORD wQuestIdx = 0 ;															// 퀘스트 인덱스를 담을 변수를 선언하고 0으로 세팅한다.
//	wQuestIdx = msg->MainQuestIdx ;													// 인자로 넘어온 메시지로 부터 퀘스트 인덱스를 받는다.
//
//	QuestTree* pTree = NULL ;														// 트리 정보를 받을 포인터를 선언하고 null처리를 한다.
//	CQuestString* pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 선언하고 null처리를 한다.
//
//	PTRLISTPOS pos = NULL ;															// 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
//	pos = m_QuestTreeList.GetHeadPosition() ;										// 퀘스트 트리 리스트를 헤드로 세팅한다.
//
//	while(pos)																		// 위치 정보가 유효할 동안 while문을 돌린다.
//	{
//		pTree = NULL ;																// 트리 정보를 받을 포인터를 null처리를 한다.
//		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// 퀘스트 트리 리스트에서 퀘스트 트리 정보를 받는다.
//
//		if( pTree )																	// 트리 정보가 유효하다면,
//		{
//			if( (WORD)(pTree->QuestIdx) == wQuestIdx )								// 트리의 퀘스트 인덱스와, 업데이트 할 퀘스트 인덱스가 같다면,
//			{
//				cPtrList* pList = NULL ;											// 리스트 정보를 받을 포인터를 선언하고 null처리한다.
//				pList = &(pTree->list) ;											// 트리의 리스트 정보를 받는다.
//
//				if( pList )
//				{
//					PTRLISTPOS treePos = NULL ;										// 트리 리스트의 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
//					treePos = pList->GetHeadPosition() ;							// 트리 리스트를 헤드로 세팅한다.
//
//					while(treePos)													// 위치 정보가 유효할 동안 while문을 돌린다.
//					{	
//						pQuestString = NULL ;										// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
//						pQuestString = (CQuestString*)pList->GetNext(treePos) ;		// 위치에 해당하는 퀘스트 스트링을 받는다.
//
//						if( pQuestString )											// 퀘스트 스트링 정보가 유효하면,
//						{
//							pList->Remove(pQuestString) ;							// 현재 퀘스트 스트링을 삭제한다.
//
//							delete pQuestString ;
//							pQuestString = NULL ;
//						}
//					}
//
//					pList->RemoveAll() ;											// 리스트의 아이템을 모두 비운다.
//				}
//				else
//				{
//					ASSERTMSG(0, "퀘스트 알림창에서 퀘스트를 업데이트 하지 못했습니다.!") ;
//					return ;
//				}
//
//				m_QuestTreeList.Remove(pTree) ;										// 퀘스트 트리 리스트에서 현재 트리를 삭제한다.
//
//				delete pTree ;
//				pTree = NULL ;
//			}
//		}
//	}
//
//	ResetQuickViewDialog() ;														// 퀵 뷰를 새로고침한다.
//}
//
////void CQuestQuickViewDialog::EndQuest(SEND_QUEST_IDX* msg)							// 알림이 창에 있는 퀘스트를 종료하는 함수.
////{
////	WORD wQuestIdx = 0 ;															// 퀘스트 인덱스를 담을 변수를 선언하고 0으로 세팅한다.
////	wQuestIdx = msg->MainQuestIdx ;													// 인자로 넘어온 메시지로 부터 퀘스트 인덱스를 받는다.
////
////	QuestTree* pTree = NULL ;														// 트리 정보를 받을 포인터를 선언하고 null처리를 한다.
////	CQuestString* pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 선언하고 null처리를 한다.
////
////	PTRLISTPOS pos = NULL ;															// 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
////	pos = m_QuestTreeList.GetHeadPosition() ;										// 퀘스트 트리 리스트를 헤드로 세팅한다.
////
////	while(pos)																		// 위치 정보가 유효할 동안 while문을 돌린다.
////	{
////		pTree = NULL ;																// 트리 정보를 받을 포인터를 null처리를 한다.
////		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// 퀘스트 트리 리스트에서 퀘스트 트리 정보를 받는다.
////
////		if( pTree )																	// 트리 정보가 유효하다면,
////		{
////			if( (WORD)(pTree->QuestIdx) == wQuestIdx )								// 트리의 퀘스트 인덱스와, 업데이트 할 퀘스트 인덱스가 같다면,
////			{
////				cPtrList* pList = NULL ;											// 리스트 정보를 받을 포인터를 선언하고 null처리한다.
////				pList = &(pTree->list) ;											// 트리의 리스트 정보를 받는다.
////
////				if( pList )
////				{
////					PTRLISTPOS treePos = NULL ;										// 트리 리스트의 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
////					treePos = pList->GetHeadPosition() ;							// 트리 리스트를 헤드로 세팅한다.
////
////					while(treePos)													// 위치 정보가 유효할 동안 while문을 돌린다.
////					{	
////						pQuestString = NULL ;										// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
////						pQuestString = (CQuestString*)pList->GetNext(treePos) ;		// 위치에 해당하는 퀘스트 스트링을 받는다.
////
////						if( pQuestString )											// 퀘스트 스트링 정보가 유효하면,
////						{
////							pList->Remove(pQuestString) ;							// 현재 퀘스트 스트링을 삭제한다.
////						}
////					}
////
////					pList->RemoveAll() ;											// 리스트의 아이템을 모두 비운다.
////				}
////				else
////				{
////					ASSERTMSG(0, "퀘스트 알림창에서 퀘스트를 업데이트 하지 못했습니다.!") ;
////					return ;
////				}
////
////				m_QuestTreeList.Remove(pTree) ;										// 퀘스트 트리 리스트에서 현재 트리를 삭제한다.
////			}
////		}
////	}
////
////	ResetQuickViewDialog() ;														// 퀵 뷰를 새로고침한다.
////}
//
//void CQuestQuickViewDialog::StartSubQuest(SEND_QUEST_IDX* pmsg)						// 알림이 창에 등록 된 서브 퀘스트를 시작하는 함수.
//{
//	if( !pmsg ) return ;															// 인자로 넘어온 메시지가 유효하지 않으면, return 처리 한다.
//
//	CQuestString* pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 선언하고 null처리를 한다.
//
//	WORD wQuestIdx = 0 ;															// 퀘스트 인덱스를 담을 변수를 선언하고 0으로 세팅한다.
//	wQuestIdx = pmsg->MainQuestIdx ;												// 인자로 넘어온 메시지로 부터 퀘스트 인덱스를 받는다.
//
//	WORD wSubQuestIdx = 0 ;															// 서브 퀘스트 인덱스를 담을 변수를 선언하고 0으로 세팅한다.
//	wSubQuestIdx = pmsg->SubQuestIdx ;												// 인자로 넘어온 메시지로 부터 서브 퀘스트 인덱스를 받는다.
//
//	QuestTree* pTree = NULL ;														// 트리 정보를 받을 포인터를 선언하고 null처리를 한다.
//
//	PTRLISTPOS pos = NULL ;															// 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
//	pos = m_QuestTreeList.GetHeadPosition() ;										// 퀘스트 트리 리스트를 헤드로 세팅한다.
//
//	while(pos)																		// 위치 정보가 유효할 동안 while문을 돌린다.
//	{
//		pTree = NULL ;																// 트리 정보를 받을 포인터를 null처리를 한다.
//		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// 퀘스트 트리 리스트에서 퀘스트 트리 정보를 받는다.
//
//		if( pTree )																	// 트리 정보가 유효하다면,
//		{
//			if( (WORD)(pTree->QuestIdx) == wQuestIdx )								// 트리의 퀘스트 인덱스와, 업데이트 할 퀘스트 인덱스가 같다면,
//			{
//				cPtrList* pList = NULL ;											// 리스트 정보를 받을 포인터를 선언하고 null처리한다.
//				pList = &(pTree->list) ;											// 트리의 리스트 정보를 받는다.
//
//				if( pList )
//				{
//					CQuestInfo* pQuestInfo = NULL ;									// 퀘스트 정보 포인터를 선언하고 null처리를 한다.
//					pQuestInfo = QUESTMGR->GetQuestInfo((DWORD)wQuestIdx) ;			// 퀘스트 매니져로 부터, 인자로 넘어온 퀘스트 번호로 퀘스트 정보를 받는다.
//
//					if( !pQuestInfo )												// 퀘스트 정보가 유효하지 않다면,
//					{
//						ASSERTMSG(0, "퀘스트 알림이 등록 중 퀘스트 정보를 받아오지 못했습니다.!") ;		// assert 에러 처리를 한다.
//						return ;													// 리턴 처리를 한다.
//					}
//
//					int nSubQuestCount = 0 ;										// 서브 퀘스트 카운트를 받을 변수를 선언하고 0으로 세팅한다.
//					nSubQuestCount = pQuestInfo->GetSubQuestCount() ;				// 퀘스트 정보로 부터 서브 퀘스트 카운트를 받는다.
//
//					if( wSubQuestIdx == nSubQuestCount-1 )
//					{
//						PTRLISTPOS treePos = NULL ;										// 트리 리스트의 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
//						treePos = pList->GetHeadPosition() ;							// 트리 리스트를 헤드로 세팅한다.
//
//						//int count = 0 ;
//
//						pQuestString = NULL ;													// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
//						pQuestString = QUESTMGR->GetQuestString(wQuestIdx, wSubQuestIdx) ;			// 퀘스트 인덱스와, 서브 퀘스트 인덱스로 퀘스트 스트링을 받는다.
//			            
//						if( !pQuestString )	continue ;											// 퀘스트 스트링 정보가 유효하지 않으면 continue한다.
//
//						CQuestString* pNewQuestString = new CQuestString ;
//
//						DWORD dwMainQuestIdx = pQuestString->GetQuestID() ;
//						DWORD dwSubQuestIdx  = pQuestString->GetSubID() ;
//
//						pNewQuestString->SetQuestID( dwMainQuestIdx ) ;
//						pNewQuestString->SetSubID( dwSubQuestIdx ) ;
//						pNewQuestString->SetIndex( dwMainQuestIdx, dwSubQuestIdx ) ;
//
//						pNewQuestString->SetComplete( pQuestString->IsComplete() ) ;
//
//						pNewQuestString->SetCount( pQuestString->GetCount() ) ;
//						pNewQuestString->SetTotalCount( pQuestString->GetTotalCount() ) ;
//
//
//						ITEM* pItem = NULL ;
//
//						cPtrList* titleList = NULL ;
//						titleList = pQuestString->GetTitle() ;
//
//						if( titleList )
//						{
//							PTRLISTPOS titlePos = NULL ;
//							titlePos = titleList->GetHeadPosition() ;
//
//							while(titlePos)
//							{
//								pItem = NULL ;
//								pItem = (ITEM*)titleList->GetNext(titlePos) ;
//
//								if( pItem )
//								{
//									ITEM* pNewItem = new ITEM ;
//
//									//memcpy(pNewItem, pItem, sizeof(ITEM)) ;
//
//									memcpy(pNewItem->string, pItem->string, sizeof(pItem->string)) ;
//									pNewItem->line		= pItem->line ;
//									pNewItem->nAlpha	= pItem->nAlpha ;
//									pNewItem->nFontIdx	= pItem->nFontIdx ;
//									pNewItem->rgb		= pItem->rgb ;
//
//									pNewQuestString->GetTitle()->AddTail(pNewItem) ;
//								}
//							}
//						}								
//
//						pNewQuestString->SetTreeTitle( pQuestString->IsTreeTitle() ) ;
//
//						cPtrList* pNewTitleList = pNewQuestString->GetTitle() ;						// 퀘스트 스트링의 타이틀 리스트를 받는다.
//
//						PTRLISTPOS newTitlePos = NULL ;										// 타이틀 리스트 상의 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
//						newTitlePos = pNewTitleList->GetHeadPosition() ;								// 타이틀 리스트를 헤드로 세팅하고 위치 정보를 받는다.
//
//						ITEM* pNewQString = NULL ;												// Q스트링을 받을 포인터를 선언하고 null처리를 한다.
//
//						while(newTitlePos)														// 위치 정보가 유효할 동안 while문을 돌린다.
//						{
//							pNewQString = NULL ;												// Q스트링 포인터를 null처리를 한다.
//							pNewQString = (ITEM*)pNewTitleList->GetNext(newTitlePos) ;					// 위치에 따른 Q스트링을 받는다.
//
//							if( pNewQString )													// Q스트링 정보가 유효하다면,
//							{
//								pNewQuestString->SetTitleStr( pNewQString->string) ;			// Q스트링의 문자열을 받아서 새로운 퀘스트 스트링에 타이틀로 세팅한다.
//
//								break ;
//							}
//						}
//
//						pTree->list.AddTail(pNewQuestString) ;								// 트리의 리스트에 퀘스트 스트링을 추가한다.
//						
//						
//						
//						//while(treePos)													// 위치 정보가 유효할 동안 while문을 돌린다.
//						//{	
//						//	if( count != 0 )
//						//	{
//						//		pDescSubQuestInfo = NULL ;													// 서브 퀘스트 정보를 받을 포인터를 null처리 한다.
//						//		pDescSubQuestInfo = pQuest->GetSubQuestInfoArray(count) ;					// 카운트에 해당하는 서브 퀘스트를 받는다.
//
//						//		if( !pDescSubQuestInfo ) 
//						//		{
//						//			++count ;
//						//			continue ;											// 서브 퀘스트 정보가 유효하지 않으면 continue한다.
//						//		}
//						//	}
//
//						//	++count ;
//
//						//	pQuestString = NULL ;										// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
//						//	pQuestString = (CQuestString*)pList->GetNext(treePos) ;		// 위치에 해당하는 퀘스트 스트링을 받는다.
//
//						//	if( pQuestString )											// 퀘스트 스트링 정보가 유효하면,
//						//	{
//						//		WORD wSubQuestId = (WORD)(pQuestString->GetSubID()) ;
//
//						//		if( wSubQuestId != nSubQuestCount-1 && wSubQuestId != 0 )// 시작이과 마지막 스트링이 아니면,
//						//		{
//						//			pList->Remove(pQuestString) ;						// 리스트에서 퀘스트 스트링을 뺀다.
//
//						//			delete pQuestString ;
//						//			pQuestString  = NULL ;
//						//		}
//						//		else													// 시작이나 마지막 스트링이면,
//						//		{
//						//			char tempBuf[256] = {0, } ;
//
//						//			strcpy(tempBuf, pQuestString->GetTitleStr()) ;
//						//			if(pQuestString->GetSubID() != 0 )
//						//			{
//						//				//strcat(tempBuf, "(완료)") ;
//
//						//				pQuestString->SetComplete(TRUE) ;
//						//			}
//
//						//			pQuestString->SetTitleStr(tempBuf) ;
//						//		}
//						//	}
//						//	else
//						//	{
//						//		pQuestString = NULL ;													// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
//						//		pQuestString = QUESTMGR->GetQuestString(wQuestIdx, wSubQuestIdx) ;			// 퀘스트 인덱스와, 서브 퀘스트 인덱스로 퀘스트 스트링을 받는다.
//					 //           
//						//		if( pQuestString )														// 퀘스트 스트링 정보가 유효하다면,
//						//		{
//						//			CQuestString* pNewQuestString = new CQuestString ;
//
//						//			DWORD dwMainQuestIdx = pQuestString->GetQuestID() ;
//						//			DWORD dwSubQuestIdx  = pQuestString->GetSubID() ;
//
//						//			pNewQuestString->SetQuestID( dwMainQuestIdx ) ;
//						//			pNewQuestString->SetSubID( dwSubQuestIdx ) ;
//						//			pNewQuestString->SetIndex( dwMainQuestIdx, dwSubQuestIdx ) ;
//
//						//			pNewQuestString->SetComplete( pQuestString->IsComplete() ) ;
//
//						//			pNewQuestString->SetCount( pQuestString->GetCount() ) ;
//						//			pNewQuestString->SetTotalCount( pQuestString->GetTotalCount() ) ;
//
//
//						//			ITEM* pItem = NULL ;
//
//						//			cPtrList* titleList = NULL ;
//						//			titleList = pQuestString->GetTitle() ;
//
//						//			if( titleList )
//						//			{
//						//				PTRLISTPOS titlePos = NULL ;
//						//				titlePos = titleList->GetHeadPosition() ;
//
//						//				while(titlePos)
//						//				{
//						//					pItem = NULL ;
//						//					pItem = (ITEM*)titleList->GetNext(titlePos) ;
//
//						//					if( pItem )
//						//					{
//						//						ITEM* pNewItem = new ITEM ;
//
//						//						//memcpy(pNewItem, pItem, sizeof(ITEM)) ;
//
//						//						memcpy(pNewItem->string, pItem->string, sizeof(pItem->string)) ;
//						//						pNewItem->line		= pItem->line ;
//						//						pNewItem->nAlpha	= pItem->nAlpha ;
//						//						pNewItem->nFontIdx	= pItem->nFontIdx ;
//						//						pNewItem->rgb		= pItem->rgb ;
//
//						//						pNewQuestString->GetTitle()->AddTail(pNewItem) ;
//						//					}
//						//				}
//						//			}								
//
//						//			pNewQuestString->SetTreeTitle( pQuestString->IsTreeTitle() ) ;
//
//						//			cPtrList* pNewTitleList = pNewQuestString->GetTitle() ;						// 퀘스트 스트링의 타이틀 리스트를 받는다.
//
//						//			PTRLISTPOS newTitlePos = NULL ;										// 타이틀 리스트 상의 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
//						//			newTitlePos = pNewTitleList->GetHeadPosition() ;								// 타이틀 리스트를 헤드로 세팅하고 위치 정보를 받는다.
//
//						//			ITEM* pNewQString = NULL ;												// Q스트링을 받을 포인터를 선언하고 null처리를 한다.
//
//						//			while(newTitlePos)														// 위치 정보가 유효할 동안 while문을 돌린다.
//						//			{
//						//				pNewQString = NULL ;												// Q스트링 포인터를 null처리를 한다.
//						//				pNewQString = (ITEM*)pNewTitleList->GetNext(newTitlePos) ;					// 위치에 따른 Q스트링을 받는다.
//
//						//				if( pNewQString )													// Q스트링 정보가 유효하다면,
//						//				{
//						//					pNewQuestString->SetTitleStr( pNewQString->string) ;			// Q스트링의 문자열을 받아서 새로운 퀘스트 스트링에 타이틀로 세팅한다.
//
//						//					break ;
//						//				}
//						//			}
//
//						//			pTree->list.AddTail(pNewQuestString) ;								// 트리의 리스트에 퀘스트 스트링을 추가한다.
//						//		}
//						//	}
//						//}
//					}
//					else
//					{
//						if( QUESTMGR->CheckQuestType(wQuestIdx, wSubQuestIdx) == e_QT_MEET_OTHER )
//						{
//							int a = 0 ;
//						}
//						else
//						{
//							pQuestString = NULL ;													// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
//							pQuestString = QUESTMGR->GetQuestString(wQuestIdx, wSubQuestIdx) ;			// 퀘스트 인덱스와, 서브 퀘스트 인덱스로 퀘스트 스트링을 받는다.
//				            
//							if( !pQuestString )	continue ;											// 퀘스트 스트링 정보가 유효하지 않으면 continue한다.
//
//							CQuestString* pNewQuestString = new CQuestString ;
//
//							DWORD dwMainQuestIdx = pQuestString->GetQuestID() ;
//							DWORD dwSubQuestIdx  = pQuestString->GetSubID() ;
//
//							pNewQuestString->SetQuestID( dwMainQuestIdx ) ;
//							pNewQuestString->SetSubID( dwSubQuestIdx ) ;
//							pNewQuestString->SetIndex( dwMainQuestIdx, dwSubQuestIdx ) ;
//
//							pNewQuestString->SetComplete( pQuestString->IsComplete() ) ;
//
//							pNewQuestString->SetCount( pQuestString->GetCount() ) ;
//							pNewQuestString->SetTotalCount( pQuestString->GetTotalCount() ) ;
//
//
//							ITEM* pItem = NULL ;
//
//							cPtrList* titleList = NULL ;
//							titleList = pQuestString->GetTitle() ;
//
//							if( titleList )
//							{
//								PTRLISTPOS titlePos = NULL ;
//								titlePos = titleList->GetHeadPosition() ;
//
//								while(titlePos)
//								{
//									pItem = NULL ;
//									pItem = (ITEM*)titleList->GetNext(titlePos) ;
//
//									if( pItem )
//									{
//										ITEM* pNewItem = new ITEM ;
//
//										//memcpy(pNewItem, pItem, sizeof(ITEM)) ;
//
//										memcpy(pNewItem->string, pItem->string, sizeof(pItem->string)) ;
//										strcat(pNewItem->string, "(완료)") ;
//										pNewItem->line		= pItem->line ;
//										pNewItem->nAlpha	= pItem->nAlpha ;
//										pNewItem->nFontIdx	= pItem->nFontIdx ;
//										pNewItem->rgb		= pItem->rgb ;
//
//										pNewQuestString->GetTitle()->AddTail(pNewItem) ;
//									}
//								}
//							}								
//
//							pNewQuestString->SetTreeTitle( pQuestString->IsTreeTitle() ) ;
//
//							cPtrList* pNewTitleList = pNewQuestString->GetTitle() ;						// 퀘스트 스트링의 타이틀 리스트를 받는다.
//
//							PTRLISTPOS newTitlePos = NULL ;										// 타이틀 리스트 상의 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
//							newTitlePos = pNewTitleList->GetHeadPosition() ;								// 타이틀 리스트를 헤드로 세팅하고 위치 정보를 받는다.
//
//							ITEM* pNewQString = NULL ;												// Q스트링을 받을 포인터를 선언하고 null처리를 한다.
//
//							while(newTitlePos)														// 위치 정보가 유효할 동안 while문을 돌린다.
//							{
//								pNewQString = NULL ;												// Q스트링 포인터를 null처리를 한다.
//								pNewQString = (ITEM*)pNewTitleList->GetNext(newTitlePos) ;					// 위치에 따른 Q스트링을 받는다.
//
//								if( pNewQString )													// Q스트링 정보가 유효하다면,
//								{
//									pNewQuestString->SetTitleStr( pNewQString->string) ;			// Q스트링의 문자열을 받아서 새로운 퀘스트 스트링에 타이틀로 세팅한다.
//
//									break ;
//								}
//							}
//
//							pTree->list.AddTail(pNewQuestString) ;								// 트리의 리스트에 퀘스트 스트링을 추가한다.
//						}
//					}
//				}
//				else
//				{
//					ASSERTMSG(0, "퀘스트 알림창에서 퀘스트를 업데이트 하지 못했습니다.!") ;
//					return ;
//				}
//			}
//		}
//	}
//
//	ResetQuickViewDialog() ;														// 퀵 뷰를 새로고침한다.
//}
//
////void CQuestQuickViewDialog::StartSubQuest(SEND_QUEST_IDX* pmsg)						// 알림이 창에 등록 된 서브 퀘스트를 시작하는 함수.
////{
////	WORD wQuestIdx = 0 ;															// 퀘스트 인덱스를 담을 변수를 선언하고 0으로 세팅한다.
////	wQuestIdx = pmsg->MainQuestIdx ;												// 인자로 넘어온 메시지로 부터 퀘스트 인덱스를 받는다.
////
////	WORD wSubQuestIdx = 0 ;															// 서브 퀘스트 인덱스를 담을 변수를 선언하고 0으로 세팅한다.
////	wSubQuestIdx = pmsg->SubQuestIdx ;												// 인자로 넘어온 메시지로 부터 서브 퀘스트 인덱스를 받는다.
////
////	QuestTree* pTree = NULL ;														// 트리 정보를 받을 포인터를 선언하고 null처리를 한다.
////	CQuestString* pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 선언하고 null처리를 한다.
////
////	PTRLISTPOS pos = NULL ;															// 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
////	pos = m_QuestTreeList.GetHeadPosition() ;										// 퀘스트 트리 리스트를 헤드로 세팅한다.
////
////	while(pos)																		// 위치 정보가 유효할 동안 while문을 돌린다.
////	{
////		pTree = NULL ;																// 트리 정보를 받을 포인터를 null처리를 한다.
////		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// 퀘스트 트리 리스트에서 퀘스트 트리 정보를 받는다.
////
////		if( pTree )																	// 트리 정보가 유효하다면,
////		{
////			if( (WORD)(pTree->QuestIdx) == wQuestIdx )								// 트리의 퀘스트 인덱스와, 업데이트 할 퀘스트 인덱스가 같다면,
////			{
////				cPtrList* pList = NULL ;											// 리스트 정보를 받을 포인터를 선언하고 null처리한다.
////				pList = &(pTree->list) ;											// 트리의 리스트 정보를 받는다.
////
////				if( pList )
////				{
////					CQuestInfo* pQuestInfo = NULL ;									// 퀘스트 정보 포인터를 선언하고 null처리를 한다.
////					pQuestInfo = QUESTMGR->GetQuestInfo((DWORD)wQuestIdx) ;			// 퀘스트 매니져로 부터, 인자로 넘어온 퀘스트 번호로 퀘스트 정보를 받는다.
////
////					if( !pQuestInfo )												// 퀘스트 정보가 유효하지 않다면,
////					{
////						ASSERTMSG(0, "퀘스트 알림이 등록 중 퀘스트 정보를 받아오지 못했습니다.!") ;		// assert 에러 처리를 한다.
////						return ;													// 리턴 처리를 한다.
////					}
////
////					int nSubQuestCount = 0 ;										// 서브 퀘스트 카운트를 받을 변수를 선언하고 0으로 세팅한다.
////					nSubQuestCount = pQuestInfo->GetSubQuestCount() ;				// 퀘스트 정보로 부터 서브 퀘스트 카운트를 받는다.
////
////					if( wSubQuestIdx == nSubQuestCount-1 )
////					{
////						PTRLISTPOS treePos = NULL ;										// 트리 리스트의 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
////						treePos = pList->GetHeadPosition() ;							// 트리 리스트를 헤드로 세팅한다.
////
////						while(treePos)													// 위치 정보가 유효할 동안 while문을 돌린다.
////						{	
////							pQuestString = NULL ;										// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
////							pQuestString = (CQuestString*)pList->GetNext(treePos) ;		// 위치에 해당하는 퀘스트 스트링을 받는다.
////
////							if( pQuestString )											// 퀘스트 스트링 정보가 유효하면,
////							{
////								WORD wSubQuestId = (WORD)(pQuestString->GetSubID()) ;
////
////								if( wSubQuestId != nSubQuestCount-1 && wSubQuestId != 0 )// 시작이과 마지막 스트링이 아니면,
////								{
////									pList->Remove(pQuestString) ;						// 리스트에서 퀘스트 스트링을 뺀다.
////								}
////								else													// 시작이나 마지막 스트링이면,
////								{
////									//if( wSubQuestId != 0 )								// 첫 스트링이 아니면,
////									//{
////									//	pList->AddTail(pQuestString) ;					// 리스트에 추가한다.
////									//}
////
////									char tempBuf[256] = {0, } ;
////
////									strcpy(tempBuf, pQuestString->GetTitleStr()) ;
////									if(pQuestString->GetSubID() != 0 )
////									{
////										//strcat(tempBuf, "(완료)") ;
////
////										pQuestString->SetComplete(TRUE) ;
////									}
////
////									pQuestString->SetTitleStr(tempBuf) ;
////								}
////							}
////						}
////					}
////				}
////				else
////				{
////					ASSERTMSG(0, "퀘스트 알림창에서 퀘스트를 업데이트 하지 못했습니다.!") ;
////					return ;
////				}
////			}
////		}
////	}
////
////	ResetQuickViewDialog() ;														// 퀵 뷰를 새로고침한다.
////}
//
//void CQuestQuickViewDialog::EndSubQuest(SEND_QUEST_IDX* msg)						// 알림이 창에 등록 된 서브 퀘스트를 end하는 함수.
//{
//	if( !msg ) return ;																// 인자로 넘어온 msg가 유효하지 않으면 return 처리 한다.
//
//	CQuestString* pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 선언하고 null처리를 한다.
//
//	//CQuest* pQuest = QUESTMGR->GetQuest( msg->MainQuestIdx );							// 퀘스트 정보를 받는다.
//
//	//if( !pQuest ) return ;															// 퀘스트 정보가 유효하지 않으면, return 처리 한다.
//
//	//CSubQuestInfo* pDescSubQuestInfo ;												// 서브 퀘스트 정보를 받을 포인터를 선언한다.
//
//	WORD wQuestIdx = 0 ;															// 퀘스트 인덱스를 담을 변수를 선언하고 0으로 세팅한다.
//	wQuestIdx = msg->MainQuestIdx ;													// 인자로 넘어온 메시지로 부터 퀘스트 인덱스를 받는다.
//
//	WORD wSubQuestIdx = 0 ;															// 서브 퀘스트 인덱스를 담을 변수를 선언하고 0으로 세팅한다.
//	wSubQuestIdx = msg->SubQuestIdx ;												// 인자로 넘어온 메시지로 부터 서브 퀘스트 인덱스를 받는다.
//
//	QuestTree* pTree = NULL ;														// 트리 정보를 받을 포인터를 선언하고 null처리를 한다.
//
//	PTRLISTPOS pos = NULL ;															// 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
//	pos = m_QuestTreeList.GetHeadPosition() ;										// 퀘스트 트리 리스트를 헤드로 세팅한다.
//
//	while(pos)																		// 위치 정보가 유효할 동안 while문을 돌린다.
//	{
//		pTree = NULL ;																// 트리 정보를 받을 포인터를 null처리를 한다.
//		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// 퀘스트 트리 리스트에서 퀘스트 트리 정보를 받는다.
//
//		if( pTree )																	// 트리 정보가 유효하다면,
//		{
//			if( (WORD)(pTree->QuestIdx) == wQuestIdx )								// 트리의 퀘스트 인덱스와, 업데이트 할 퀘스트 인덱스가 같다면,
//			{
//				cPtrList* pList = NULL ;											// 리스트 정보를 받을 포인터를 선언하고 null처리한다.
//				pList = &(pTree->list) ;											// 트리의 리스트 정보를 받는다.
//
//				if( pList )
//				{
//					PTRLISTPOS treePos = NULL ;										// 트리 리스트의 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
//					treePos = pList->GetHeadPosition() ;							// 트리 리스트를 헤드로 세팅한다.
//
//					//int count = 0 ;
//
//					while(treePos)													// 위치 정보가 유효할 동안 while문을 돌린다.
//					{	
//						//if( count != 0 )
//						//{
//						//	pDescSubQuestInfo = NULL ;													// 서브 퀘스트 정보를 받을 포인터를 null처리 한다.
//						//	pDescSubQuestInfo = pQuest->GetSubQuestInfoArray(count) ;					// 카운트에 해당하는 서브 퀘스트를 받는다.
//
//						//	if( !pDescSubQuestInfo ) 
//						//	{
//						//		++count ;
//						//		continue ;											// 서브 퀘스트 정보가 유효하지 않으면 continue한다.
//						//	}
//						//}
//
//						//++count ;
//
//						pQuestString = NULL ;										// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
//						pQuestString = (CQuestString*)pList->GetNext(treePos) ;		// 위치에 해당하는 퀘스트 스트링을 받는다.
//
//						if( pQuestString )											// 퀘스트 스트링 정보가 유효하면,
//						{
//							if( (WORD)(pQuestString->GetSubID()) == wSubQuestIdx )	// 스트링의 서브 퀘 인덱스와, 인자로 넘어온 서브 퀘 인덱스가 같으면,
//							{
//								pTree->list.Remove(pQuestString) ;
//
//								delete pQuestString ;
//								pQuestString = NULL ;
//
//
//								//char tempBuf[1024] = {0, } ;						// 임시 버퍼를 선언한다.
//
//								//cPtrList* pTitleList = NULL ;						// 업데이트 스트링의 리스트를 받을 포인터를 선언하고 null처리를 한다.
//								//pTitleList = pQuestString->GetTitle() ;				// 업데이트 스트링의 리스트를 받는다.
//
//								//if( pTitleList )									// 리스트 정보가 유효하면,
//								//{
//								//	ITEM* pQString = NULL ;						// 퀘스트 스트링을 받을 포인터를 선언하고 null처리를 한다.
//
//								//	PTRLISTPOS titlePos = NULL ;					// 업데이트 할 스트링의 리스트 상의 위치를 받을 포인터를 선언하고 null처리 한다.
//								//	titlePos = pTitleList->GetHeadPosition() ;		// 업데이트 할 스트링의 리스트를 헤드로 세팅한다.
//	
//								//	while(titlePos)									// 위치 정보가 유효할 동안 while문을 돌린다.
//								//	{
//								//		pQString = NULL ;							// 포인터를 null처리를 한다.
//								//		pQString = (ITEM*)pTitleList->GetNext(titlePos) ; // 위치에 따른 스트링 정보를 받는다.
//
//								//		if( pQString )								// 스트링 정보가 유효하면,
//								//		{
//								//			strcat(tempBuf, pQString->string) ;		// 임시버퍼에 스트링을 담는다.
//
//								//			break ;
//								//		}
//								//	}
//
//								//	pQuestString->SetComplete(TRUE) ;
//
//								//	pQuestString->SetTitleStr(tempBuf) ;			// 스트링의 타이틀을 세팅한다.
//								//}
//							}
//						}
//					}
//				}
//				else
//				{
//					ASSERTMSG(0, "퀘스트 알림창에서 퀘스트를 업데이트 하지 못했습니다.!") ;
//					return ;
//				}
//			}
//		}
//	}
//
//	ResetQuickViewDialog() ;														// 퀵 뷰를 새로고침한다.
//}
//
////void CQuestQuickViewDialog::EndSubQuest(SEND_QUEST_IDX* msg)						// 알림이 창에 등록 된 서브 퀘스트를 end하는 함수.
////{
////	WORD wQuestIdx = 0 ;															// 퀘스트 인덱스를 담을 변수를 선언하고 0으로 세팅한다.
////	wQuestIdx = msg->MainQuestIdx ;													// 인자로 넘어온 메시지로 부터 퀘스트 인덱스를 받는다.
////
////	WORD wSubQuestIdx = 0 ;															// 서브 퀘스트 인덱스를 담을 변수를 선언하고 0으로 세팅한다.
////	wSubQuestIdx = msg->SubQuestIdx ;												// 인자로 넘어온 메시지로 부터 서브 퀘스트 인덱스를 받는다.
////
////	QuestTree* pTree = NULL ;														// 트리 정보를 받을 포인터를 선언하고 null처리를 한다.
////	CQuestString* pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 선언하고 null처리를 한다.
////
////	PTRLISTPOS pos = NULL ;															// 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
////	pos = m_QuestTreeList.GetHeadPosition() ;										// 퀘스트 트리 리스트를 헤드로 세팅한다.
////
////	while(pos)																		// 위치 정보가 유효할 동안 while문을 돌린다.
////	{
////		pTree = NULL ;																// 트리 정보를 받을 포인터를 null처리를 한다.
////		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// 퀘스트 트리 리스트에서 퀘스트 트리 정보를 받는다.
////
////		if( pTree )																	// 트리 정보가 유효하다면,
////		{
////			if( (WORD)(pTree->QuestIdx) == wQuestIdx )								// 트리의 퀘스트 인덱스와, 업데이트 할 퀘스트 인덱스가 같다면,
////			{
////				cPtrList* pList = NULL ;											// 리스트 정보를 받을 포인터를 선언하고 null처리한다.
////				pList = &(pTree->list) ;											// 트리의 리스트 정보를 받는다.
////
////				if( pList )
////				{
////					PTRLISTPOS treePos = NULL ;										// 트리 리스트의 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
////					treePos = pList->GetHeadPosition() ;							// 트리 리스트를 헤드로 세팅한다.
////
////					while(treePos)													// 위치 정보가 유효할 동안 while문을 돌린다.
////					{	
////						pQuestString = NULL ;										// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
////						pQuestString = (CQuestString*)pList->GetNext(treePos) ;		// 위치에 해당하는 퀘스트 스트링을 받는다.
////
////						if( pQuestString )											// 퀘스트 스트링 정보가 유효하면,
////						{
////							if( (WORD)(pQuestString->GetSubID()) == wSubQuestIdx )	// 스트링의 서브 퀘 인덱스와, 인자로 넘어온 서브 퀘 인덱스가 같으면,
////							{
////								char tempBuf[1024] = {0, } ;						// 임시 버퍼를 선언한다.
////
////								cPtrList* pTitleList = NULL ;						// 업데이트 스트링의 리스트를 받을 포인터를 선언하고 null처리를 한다.
////								pTitleList = pQuestString->GetTitle() ;				// 업데이트 스트링의 리스트를 받는다.
////
////								if( pTitleList )									// 리스트 정보가 유효하면,
////								{
////									QString* pQString = NULL ;						// 퀘스트 스트링을 받을 포인터를 선언하고 null처리를 한다.
////
////									PTRLISTPOS titlePos = NULL ;					// 업데이트 할 스트링의 리스트 상의 위치를 받을 포인터를 선언하고 null처리 한다.
////									titlePos = pTitleList->GetHeadPosition() ;		// 업데이트 할 스트링의 리스트를 헤드로 세팅한다.
////
////									//sGAMENOTIFY_MSG* pNoticeMsg ;						// 게임 공지 메시지 구조체를 선언한다.
////									//pNoticeMsg = new sGAMENOTIFY_MSG ;
////									//pNoticeMsg->nNotifyType = eSTYLE_QUEST ;			// 공지 타입을 퀘스트로 세팅한다.
////
////									//pNoticeMsg->rect.left = 500 ;
////									//pNoticeMsg->rect.top = 500 ;
////									//pNoticeMsg->rect.right = 1 ;
////									//pNoticeMsg->rect.bottom = 500 ;
////	
////									while(titlePos)									// 위치 정보가 유효할 동안 while문을 돌린다.
////									{
////										pQString = NULL ;							// 포인터를 null처리를 한다.
////										pQString = (QString*)pTitleList->GetNext(titlePos) ; // 위치에 따른 스트링 정보를 받는다.
////
////										if( pQString )								// 스트링 정보가 유효하면,
////										{
////											strcat(tempBuf, pQString->Str) ;		// 임시버퍼에 스트링을 담는다.
////
////											//ITEM* pNoticeItem = new ITEM ;			// 서브 퀘스트 아이템을 담을 아이템을 생성한다.
////
////											//pNoticeItem->rgb = dwQuestStringColor[eQC_SUBQUEST_NAME] ;	// 서브 퀘스트 스트링의 색상을 세팅한다.
////
////											///*pNoticeMsg->red = 0 ;
////											//pNoticeMsg->gree = 255 ;
////											//pNoticeMsg->blue =  0  ;
////											//pNoticeMsg->nAlpha = 255 ;*/
////
////											//strcpy(pNoticeItem->string, tempBuf) ;	// 서브 퀘스트 스트링을 복사한다.
////
////											//pNoticeMsg->itemList.AddTail(pNoticeItem);// 공지 메시지 구조체의 리스트에 서브 퀘스트 아이템을 추가한다.
////										}
////									}
////
////									//char tempBuf2[128] = {0, } ;					// 두번째 임시 버퍼를 선언한다.
////
////									//sprintf(tempBuf2, "(완료)") ;					// 임시 버퍼에 현재 진행을 표현한다.
////
////									//strcat(tempBuf, tempBuf2) ;						// 첫 번째 임시 버퍼에 두 번째 임시 버퍼를 추가한다.
////
////									pQuestString->SetComplete(TRUE) ;
////
////									pQuestString->SetTitleStr(tempBuf) ;			// 스트링의 타이틀을 세팅한다.
////
////									//ITEM* pCountItem = new ITEM ;					// 카운트를 담을 아이템을 생성한다.
////
////									//strcpy(pCountItem->string, tempBuf2) ;			// 카운트 아이템에 카운트 스트링을 복사한다.
////
////									//pCountItem->rgb = dwQuestStringColor[eQC_COUNT];// 카운트 스트링의 색상을 세팅한다.
////
////									///*pNoticeMsg->red = 0 ;
////									//pNoticeMsg->gree = 0 ;
////									//pNoticeMsg->blue = 255  ;
////									//pNoticeMsg->nAlpha = 255 ;*/
////
////									//pNoticeMsg->itemList.AddTail(pCountItem) ;		// 공지 메시지 구조체의 리스트에 카운트 아이템을 추가한다.
////									//
////
////									//GAMENOTIFYMGR->AddMsg(pNoticeMsg) ;				// 게임 공지 매니져에 공지 메시지를 추가한다.
////								}
////							}
////						}
////					}
////				}
////				else
////				{
////					ASSERTMSG(0, "퀘스트 알림창에서 퀘스트를 업데이트 하지 못했습니다.!") ;
////					return ;
////				}
////			}
////		}
////	}
////
////	ResetQuickViewDialog() ;														// 퀵 뷰를 새로고침한다.
////}
//
//void CQuestQuickViewDialog::UpdateQuest(SEND_SUBQUEST_UPDATE* msg)					// 알림이 창에 등록 된 퀘스트를 업데이트 하는 함수.
//{
//	CQuestString* pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 선언하고 null처리를 한다.
//
//	CQuest* pQuest = QUESTMGR->GetQuest( msg->wQuestIdx );							// 퀘스트 정보를 받는다.
//
//	if( !pQuest ) return ;															// 퀘스트 정보가 유효하지 않으면, return 처리 한다.
//
//	CSubQuestInfo* pDescSubQuestInfo ;												// 서브 퀘스트 정보를 받을 포인터를 선언한다.
//
//	WORD wQuestIdx = 0 ;															// 퀘스트 인덱스를 담을 변수를 선언하고 0으로 세팅한다.
//	wQuestIdx = msg->wQuestIdx ;													// 인자로 넘어온 메시지로 부터 퀘스트 인덱스를 받는다.
//
//	WORD wSubQuestIdx = 0 ;															// 서브 퀘스트 인덱스를 담을 변수를 선언하고 0으로 세팅한다.
//	wSubQuestIdx = msg->wSubQuestIdx ;												// 인자로 넘어온 메시지로 부터 서브 퀘스트 인덱스를 받는다.
//
//	QuestTree* pTree = NULL ;														// 트리 정보를 받을 포인터를 선언하고 null처리를 한다.
//
//	PTRLISTPOS pos = NULL ;															// 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
//	pos = m_QuestTreeList.GetHeadPosition() ;										// 퀘스트 트리 리스트를 헤드로 세팅한다.
//
//	while(pos)																		// 위치 정보가 유효할 동안 while문을 돌린다.
//	{
//		pTree = NULL ;																// 트리 정보를 받을 포인터를 null처리를 한다.
//		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// 퀘스트 트리 리스트에서 퀘스트 트리 정보를 받는다.
//
//		if( pTree )																	// 트리 정보가 유효하다면,
//		{
//			if( (WORD)(pTree->QuestIdx) == wQuestIdx )								// 트리의 퀘스트 인덱스와, 업데이트 할 퀘스트 인덱스가 같다면,
//			{
//				cPtrList* pList = NULL ;											// 리스트 정보를 받을 포인터를 선언하고 null처리한다.
//				pList = &(pTree->list) ;											// 트리의 리스트 정보를 받는다.
//
//				if( pList )
//				{
//					PTRLISTPOS treePos = NULL ;										// 트리 리스트의 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
//					treePos = pList->GetHeadPosition() ;							// 트리 리스트를 헤드로 세팅한다.
//
//					int count = 0 ;
//
//					while(treePos)													// 위치 정보가 유효할 동안 while문을 돌린다.
//					{	
//						if( count != 0 )
//						{
//							pDescSubQuestInfo = NULL ;													// 서브 퀘스트 정보를 받을 포인터를 null처리 한다.
//							pDescSubQuestInfo = pQuest->GetSubQuestInfoArray(count) ;					// 카운트에 해당하는 서브 퀘스트를 받는다.
//
//							if( !pDescSubQuestInfo ) 
//							{
//								++count ;
//								continue ;											// 서브 퀘스트 정보가 유효하지 않으면 continue한다.
//							}
//						}
//
//						++count ;
//
//						pQuestString = NULL ;										// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
//						pQuestString = (CQuestString*)pList->GetNext(treePos) ;		// 위치에 해당하는 퀘스트 스트링을 받는다.
//
//						if( pQuestString )											// 퀘스트 스트링 정보가 유효하면,
//						{
//							if( (WORD)(pQuestString->GetSubID()) == wSubQuestIdx )	// 스트링의 서브 퀘 인덱스와, 인자로 넘어온 서브 퀘 인덱스가 같으면,
//							{
//								char tempBuf[1024] = {0, } ;						// 임시 버퍼를 선언한다.
//
//								cPtrList* pTitleList = NULL ;						// 업데이트 스트링의 리스트를 받을 포인터를 선언하고 null처리를 한다.
//								pTitleList = pQuestString->GetTitle() ;				// 업데이트 스트링의 리스트를 받는다.
//
//								if( pTitleList )									// 리스트 정보가 유효하면,
//								{
//									ITEM* pQString = NULL ;						// 퀘스트 스트링을 받을 포인터를 선언하고 null처리를 한다.
//
//									PTRLISTPOS titlePos = NULL ;					// 업데이트 할 스트링의 리스트 상의 위치를 받을 포인터를 선언하고 null처리 한다.
//									titlePos = pTitleList->GetHeadPosition() ;		// 업데이트 할 스트링의 리스트를 헤드로 세팅한다.
//	
//									while(titlePos)									// 위치 정보가 유효할 동안 while문을 돌린다.
//									{
//										pQString = NULL ;							// 포인터를 null처리를 한다.
//										pQString = (ITEM*)pTitleList->GetNext(titlePos) ; // 위치에 따른 스트링 정보를 받는다.
//
//										if( pQString )								// 스트링 정보가 유효하면,
//										{
//											strcat(tempBuf, pQString->string) ;		// 임시버퍼에 스트링을 담는다.
//											break ;
//										}
//									}
//
//									pQuestString->SetCount(msg->dwData) ;
//									pQuestString->SetTotalCount(msg->dwMaxCount) ;
//
//									pQuestString->SetTitleStr(tempBuf) ;			// 스트링의 타이틀을 세팅한다.
//								}
//							}
//						}
//					}
//				}
//				else
//				{
//					ASSERTMSG(0, "퀘스트 알림창에서 퀘스트를 업데이트 하지 못했습니다.!") ;
//					return ;
//				}
//			}
//		}
//	}
//
//	ResetQuickViewDialog() ;														// 퀵 뷰를 새로고침한다.
//}
//
////void CQuestQuickViewDialog::UpdateQuest(SEND_SUBQUEST_UPDATE* msg)					// 알림이 창에 등록 된 퀘스트를 업데이트 하는 함수.
////{
////	WORD wQuestIdx = 0 ;															// 퀘스트 인덱스를 담을 변수를 선언하고 0으로 세팅한다.
////	wQuestIdx = msg->wQuestIdx ;													// 인자로 넘어온 메시지로 부터 퀘스트 인덱스를 받는다.
////
////	WORD wSubQuestIdx = 0 ;															// 서브 퀘스트 인덱스를 담을 변수를 선언하고 0으로 세팅한다.
////	wSubQuestIdx = msg->wSubQuestIdx ;												// 인자로 넘어온 메시지로 부터 서브 퀘스트 인덱스를 받는다.
////
////	QuestTree* pTree = NULL ;														// 트리 정보를 받을 포인터를 선언하고 null처리를 한다.
////	CQuestString* pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 선언하고 null처리를 한다.
////
////	PTRLISTPOS pos = NULL ;															// 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
////	pos = m_QuestTreeList.GetHeadPosition() ;										// 퀘스트 트리 리스트를 헤드로 세팅한다.
////
////	while(pos)																		// 위치 정보가 유효할 동안 while문을 돌린다.
////	{
////		pTree = NULL ;																// 트리 정보를 받을 포인터를 null처리를 한다.
////		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// 퀘스트 트리 리스트에서 퀘스트 트리 정보를 받는다.
////
////		if( pTree )																	// 트리 정보가 유효하다면,
////		{
////			if( (WORD)(pTree->QuestIdx) == wQuestIdx )								// 트리의 퀘스트 인덱스와, 업데이트 할 퀘스트 인덱스가 같다면,
////			{
////				cPtrList* pList = NULL ;											// 리스트 정보를 받을 포인터를 선언하고 null처리한다.
////				pList = &(pTree->list) ;											// 트리의 리스트 정보를 받는다.
////
////				if( pList )
////				{
////					PTRLISTPOS treePos = NULL ;										// 트리 리스트의 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
////					treePos = pList->GetHeadPosition() ;							// 트리 리스트를 헤드로 세팅한다.
////
////					while(treePos)													// 위치 정보가 유효할 동안 while문을 돌린다.
////					{	
////						pQuestString = NULL ;										// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
////						pQuestString = (CQuestString*)pList->GetNext(treePos) ;		// 위치에 해당하는 퀘스트 스트링을 받는다.
////
////						if( pQuestString )											// 퀘스트 스트링 정보가 유효하면,
////						{
////							if( (WORD)(pQuestString->GetSubID()) == wSubQuestIdx )	// 스트링의 서브 퀘 인덱스와, 인자로 넘어온 서브 퀘 인덱스가 같으면,
////							{
////								char tempBuf[1024] = {0, } ;						// 임시 버퍼를 선언한다.
////
////								cPtrList* pTitleList = NULL ;						// 업데이트 스트링의 리스트를 받을 포인터를 선언하고 null처리를 한다.
////								pTitleList = pQuestString->GetTitle() ;				// 업데이트 스트링의 리스트를 받는다.
////
////								if( pTitleList )									// 리스트 정보가 유효하면,
////								{
////									//sGAMENOTIFY_MSG* pNoticeMsg ;						// 게임 공지 메시지 구조체를 선언한다.
////									//pNoticeMsg = new sGAMENOTIFY_MSG ;
////									//pNoticeMsg->nNotifyType = eSTYLE_QUEST ;			// 공지 타입을 퀘스트로 세팅한다.
////
////									//pNoticeMsg->rect.left = 500 ;
////									//pNoticeMsg->rect.top = 500 ;
////									//pNoticeMsg->rect.right = 1 ;
////									//pNoticeMsg->rect.bottom = 500 ;
////
////
////									QString* pQString = NULL ;						// 퀘스트 스트링을 받을 포인터를 선언하고 null처리를 한다.
////
////									PTRLISTPOS titlePos = NULL ;					// 업데이트 할 스트링의 리스트 상의 위치를 받을 포인터를 선언하고 null처리 한다.
////									titlePos = pTitleList->GetHeadPosition() ;		// 업데이트 할 스트링의 리스트를 헤드로 세팅한다.
////	
////									while(titlePos)									// 위치 정보가 유효할 동안 while문을 돌린다.
////									{
////										pQString = NULL ;							// 포인터를 null처리를 한다.
////										pQString = (QString*)pTitleList->GetNext(titlePos) ; // 위치에 따른 스트링 정보를 받는다.
////
////										if( pQString )								// 스트링 정보가 유효하면,
////										{
////											strcat(tempBuf, pQString->Str) ;		// 임시버퍼에 스트링을 담는다.
////
////											//ITEM* pNoticeItem = new ITEM ;			// 서브 퀘스트 아이템을 담을 아이템을 생성한다.
////
////											//pNoticeItem->rgb = dwQuestStringColor[eQC_SUBQUEST_NAME] ;	// 서브 퀘스트 스트링의 색상을 세팅한다.
////
////											///*pNoticeMsg->red = 0 ;
////											//pNoticeMsg->gree = 255 ;
////											//pNoticeMsg->blue =  0  ;
////											//pNoticeMsg->nAlpha = 255 ;*/
////
////											//strcpy(pNoticeItem->string, tempBuf) ;	// 서브 퀘스트 스트링을 복사한다.
////
////											//pNoticeMsg->itemList.AddTail(pNoticeItem);// 공지 메시지 구조체의 리스트에 서브 퀘스트 아이템을 추가한다.
////										}
////									}
////
////									//char tempBuf2[128] = {0, } ;					// 두번째 임시 버퍼를 선언한다.
////
////									//if( msg->dwMaxCount == 0 )
////									//{
////									//	sprintf(tempBuf2, " (%d)", msg->dwData) ;// 임시 버퍼에 현재 진행을 표현한다.
////									//}
////									//else
////									//{
////									//	sprintf(tempBuf2, " (%d/%d)", msg->dwData, msg->dwMaxCount) ;// 임시 버퍼에 현재 진행을 표현한다.
////									//}
////
////									pQuestString->SetCount(msg->dwData) ;
////									pQuestString->SetTotalCount(msg->dwMaxCount) ;
////
////									//strcat(tempBuf, tempBuf2) ;						// 첫 번째 임시 버퍼에 두 번째 임시 버퍼를 추가한다.
////
////									pQuestString->SetTitleStr(tempBuf) ;			// 스트링의 타이틀을 세팅한다.
////
////
////									//ITEM* pCountItem = new ITEM ;					// 카운트를 담을 아이템을 생성한다.
////
////									//strcpy(pCountItem->string, tempBuf2) ;			// 카운트 아이템에 카운트 스트링을 복사한다.
////
////									//pCountItem->rgb = dwQuestStringColor[eQC_COUNT];// 카운트 스트링의 색상을 세팅한다.
////
////									///*pNoticeMsg->red = 0 ;
////									//pNoticeMsg->gree = 0 ;
////									//pNoticeMsg->blue = 255  ;
////									//pNoticeMsg->nAlpha = 255 ;*/
////
////									//pNoticeMsg->itemList.AddTail(pCountItem) ;		// 공지 메시지 구조체의 리스트에 카운트 아이템을 추가한다.
////									//
////
////									//GAMENOTIFYMGR->AddMsg(pNoticeMsg) ;				// 게임 공지 매니져에 공지 메시지를 추가한다.
////								}
////							}
////						}
////					}
////				}
////				else
////				{
////					ASSERTMSG(0, "퀘스트 알림창에서 퀘스트를 업데이트 하지 못했습니다.!") ;
////					return ;
////				}
////			}
////		}
////	}
////
////	ResetQuickViewDialog() ;														// 퀵 뷰를 새로고침한다.
////}
//
//void CQuestQuickViewDialog::ResetLoadedQuickViewDialog()							// 퀘스트 리스트를 새로고침하는 함수.
//{
//	DWORD Color = 0;																// 색상 값을 담을 변수를 선언하고 0으로 세팅한다.
//	char buf[256] = { 0, };															// 임시 버퍼를 선언한다.	
//
//	QuestTree* pTree ;																// 트리 정보를 받을 포인터를 선언한다.
//	CQuestString* pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 선언하고 null처리를 한다.
//	//QString* pLine ;
//
//	PTRLISTPOS pos = NULL ;															// 리스트 내의 위치를 담을 포인터를 선언하고 null처리를 한다.
//	PTRLISTPOS treePos = NULL ;														// 트리의 리스트 내의 위치를 담을 포인터를 선언하고 null처리를 한다.
//	PTRLISTPOS pLinePos = NULL ;													// 퀘스트 스트링의 라인을 받는 위치 포인터를 선언하고 null처리를 한다.
//
//	int nItemCount = 0 ;															// 알림이 리스트의 아이템 카운트를 담을 변수를 선언하고 0으로 세팅한다.
//	nItemCount = m_pViewList->GetItemCount() ;										// 알림이 리스트의 아이템 카운트를 받는다.
//
//	m_pViewList->RemoveAll() ;														// 퀘스트 리스트 다이얼로그를 모두 비운다.
//
//	for( int count = 0 ; count < nItemCount ; ++count )								// 알림이 리스트의 아이템 카운트 만큼 for문을 돌린다.
//	{
//		m_pViewList->SetExtendReduction(LD_REDUCTION) ;								// 리스트 다이얼로그를 축소한다.
//	}
//
//	pos = m_QuestTreeList.GetHeadPosition() ;										// 위치 포인터를 퀘스트의 헤드로 세팅한다.
//
//	while(pos)																		// 리스트의 다음 요소가 유효할 동안 while문을 돌린다.
//	{
//		pTree = NULL ;																// 트리 정보를 받을 포인터를 null 처리를 한다.
//		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// 위치 정보에 해당하는 트리를 받는다.
//
//		if( pTree )																	// 트리 정보가 유효하다면,
//		{
//			treePos = NULL ;														// 트리의 리스트에서 위치 정보를 받을 포인터를 선언하고 null 처리를 한다.	
//			treePos = pTree->list.FindIndex(0) ;									// 리스트의 첫 위치를 받는다.
//
//			int nSubQuestCount = 0 ;												// 서브 퀘스트 카운트를 받을 변수를 선언하고 0으로 세팅한다.
//
//			if( treePos )															// 트리의 위치 정보가 유효하면,
//			{
//				pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
//				pQuestString = (CQuestString*)pTree->list.GetAt(treePos) ;			// 트리 위치 포인터에 해당하는 퀘스트 스트링을 받는다.
//
//				if( pQuestString )													// 퀘스트 스트링 정보가 유효하면,
//				{
//					CQuestInfo* pQuestInfo = NULL ;									// 퀘스트 정보 포인터를 선언하고 null처리를 한다.
//					pQuestInfo = QUESTMGR->GetQuestInfo(pQuestString->GetQuestID()) ;// 퀘스트 매니져로 부터, 인자로 넘어온 퀘스트 번호로 퀘스트 정보를 받는다.
//
//					if( !pQuestInfo )												// 퀘스트 정보가 유효하지 않다면,
//					{
//						ASSERTMSG(0, "퀘스트 알림이 등록 중 리스트 다이얼로그에 스트링 추가 중 퀘 정보 못가져 왔습니다.!") ;		// assert 에러 처리를 한다.
//						return ;													// 리턴 처리를 한다.
//					}
//					
//					nSubQuestCount = pQuestInfo->GetSubQuestCount() ;				// 퀘스트 정보로 부터 서브 퀘스트 카운트를 받는다.
//				}
//				else																// 퀘스트 스트링 정보가 유효하지 않으면,
//				{
//					continue ;														// 컨티뉴 한다.
//				}
//			}
//
//			if( nSubQuestCount > 1 )												// 적어도 서브퀘 최소 두 개는 되야 한다.
//			{
//				int nCountOfTreeCount = 0 ;											// 트리의 리스트의 카운트를 담을 변수를 선언하고, 0으로 세팅한다.
//				nCountOfTreeCount = pTree->list.GetCount() ;						// 트리의 리스트의 카운트를 받는다.
//
//				treePos = NULL ;													// 트리의 리스트에서 위치 정보를 받을 포인터를 선언하고 null 처리를 한다.	
//				treePos = pTree->list.GetHeadPosition() ;							// 트리의 리스트를 헤드로 세팅한다.
//
//				while(treePos)														// 위치 정보가 유효할 동안 while문을 돌린다.
//				{
//					pQuestString = NULL ;											// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
//					pQuestString = (CQuestString*)pTree->list.GetNext(treePos) ;	// 0번째 위치에 해당하는 퀘스트 스트링을 받는다.
//
//					if( pQuestString )												// 퀘스트 스트링 정보가 유효하면,
//					{
//						DWORD dwSubQuestIdx = pQuestString->GetSubID() ;			// 서브 퀘스트 인덱스를 받는다.		
//
//						if(dwSubQuestIdx == 0)										// 서브 퀘스트 인덱스가 0과 같으면, 
//						{
//							Color=RGB(0, 255, 255) ;								// 색상을 메인 퀘스트 색상으로 세팅한다.
//						}
//						else														// 그 외의 경우.
//						{
//							Color=RGB(255, 255, 255) ;								// 색상을 서브 퀘스트 색상으로 세팅한다.
//						}
//
//						memset(buf, 0, 256) ;										// 임시 버퍼를 초기화 한다.
//
//						if( dwSubQuestIdx != 0 )									// 서브 퀘스트 인덱스가 0이 아니면
//						{
//							sprintf(buf, "	- ");									// 임시 버퍼를 -로 세팅한다.
//						}
//
//						strcat(buf, pQuestString->GetTitleStr()) ;					// 스트링을 복사한다.
//
//						if( pQuestString->IsComplete() )
//						{
//							strcat(buf, "(완료)") ;
//						}
//						else
//						{
//							char tempBuf2[64] = {0, } ;
//
//							if( pQuestString->GetTotalCount() == 0 )
//							{
//								if(pQuestString->GetCount() != 0 )
//								{
//									sprintf(tempBuf2, "(%d)", pQuestString->GetCount()) ;
//								}
//							}
//							else
//							{
//								if(pQuestString->GetCount() != 0 )
//								{
//									if( pQuestString->GetCount() == pQuestString->GetTotalCount() )
//									{
//										strcat(tempBuf2, "(완료)") ;
//									}
//									else
//									{
//										sprintf(tempBuf2, "(%d/%d)", pQuestString->GetCount(), pQuestString->GetTotalCount()) ;
//									}
//								}
//							}
//
//							strcat(buf, tempBuf2) ;
//						}
//						
//						/*if( nSubQuestCount <= 2 && pQuestString->GetSubID() != 0)
//						{
//							strcat(buf, "(완료)") ;
//						}*/
//
//						if( nCountOfTreeCount > 2 )									// 트리의 리스트의 카운트가 2개를 넘으면,
//						{
//							if( dwSubQuestIdx != nSubQuestCount-1 )					// 서브 인덱스가 마지막 인덱스가 아니면,
//							{
//								m_pViewList->AddItemWithFont(buf, Color, 0) ;		// 리스트 다이얼로그에 아이템을 추가한다.
//								m_pViewList->SetExtendReduction(LD_EXTEND) ;		// 리스트 다이얼로그를 확장한다.
//							}
//						}
//						else														// 트리의 리스트 카운트가 2개 이하면,
//						{
//							m_pViewList->AddItemWithFont(buf, Color, 0) ;			// 리스트 다이얼로그에 아이템을 추가한다.
//							m_pViewList->SetExtendReduction(LD_EXTEND) ;			// 리스트 다이얼로그를 확장한다.
//						}
//
//						if( dwSubQuestIdx == 0 && pTree->State == eQTree_Close )
//						{
//							break ;
//						}
//					}
//				}
//			}
//		}
//	}
//}
//
//void CQuestQuickViewDialog::ResetQuickViewDialog()									// 퀘스트 리스트를 새로고침하는 함수.
//{
// 	DWORD Color = 0;																// 색상 값을 담을 변수를 선언하고 0으로 세팅한다.
//	char buf[256] = { 0, };															// 임시 버퍼를 선언한다.	
//
//	QuestTree* pTree ;																// 트리 정보를 받을 포인터를 선언한다.
//	CQuestString* pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 선언하고 null처리를 한다.
//	//QString* pLine ;
//
//	PTRLISTPOS pos = NULL ;															// 리스트 내의 위치를 담을 포인터를 선언하고 null처리를 한다.
//	PTRLISTPOS treePos = NULL ;														// 트리의 리스트 내의 위치를 담을 포인터를 선언하고 null처리를 한다.
//	PTRLISTPOS pLinePos = NULL ;													// 퀘스트 스트링의 라인을 받는 위치 포인터를 선언하고 null처리를 한다.
//
//	int nItemCount = 0 ;															// 알림이 리스트의 아이템 카운트를 담을 변수를 선언하고 0으로 세팅한다.
//	nItemCount = m_pViewList->GetItemCount() ;										// 알림이 리스트의 아이템 카운트를 받는다.
//
//	m_pViewList->RemoveAll() ;														// 퀘스트 리스트 다이얼로그를 모두 비운다.
//
//	for( int count = 0 ; count < nItemCount ; ++count )								// 알림이 리스트의 아이템 카운트 만큼 for문을 돌린다.
//	{
//		m_pViewList->SetExtendReduction(LD_REDUCTION) ;								// 리스트 다이얼로그를 축소한다.
//	}
//
//	pos = m_QuestTreeList.GetHeadPosition() ;										// 위치 포인터를 퀘스트의 헤드로 세팅한다.
//
//	while(pos)																		// 리스트의 다음 요소가 유효할 동안 while문을 돌린다.
//	{
//		pTree = NULL ;																// 트리 정보를 받을 포인터를 null 처리를 한다.
//		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// 위치 정보에 해당하는 트리를 받는다.
//
//		if( pTree )																	// 트리 정보가 유효하다면,
//		{
//			CQuestString* pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 선언하고 null처리를 한다.
//
//			CQuest* pQuest = QUESTMGR->GetQuest( pTree->QuestIdx );							// 퀘스트 정보를 받는다.
//
//			if( !pQuest ) return ;															// 퀘스트 정보가 유효하지 않으면, return 처리 한다.
//
//			CSubQuestInfo* pDescSubQuestInfo ;												// 서브 퀘스트 정보를 받을 포인터를 선언한다.
//
//			treePos = NULL ;														// 트리의 리스트에서 위치 정보를 받을 포인터를 선언하고 null 처리를 한다.	
//			treePos = pTree->list.FindIndex(0) ;									// 리스트의 첫 위치를 받는다.
//
//			int nSubQuestCount = 0 ;												// 서브 퀘스트 카운트를 받을 변수를 선언하고 0으로 세팅한다.
//
//			if( treePos )															// 트리의 위치 정보가 유효하면,
//			{
//				pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
//				pQuestString = (CQuestString*)pTree->list.GetAt(treePos) ;			// 트리 위치 포인터에 해당하는 퀘스트 스트링을 받는다.
//
//				if( pQuestString )													// 퀘스트 스트링 정보가 유효하면,
//				{
//					CQuestInfo* pQuestInfo = NULL ;									// 퀘스트 정보 포인터를 선언하고 null처리를 한다.
//					pQuestInfo = QUESTMGR->GetQuestInfo(pQuestString->GetQuestID()) ;// 퀘스트 매니져로 부터, 인자로 넘어온 퀘스트 번호로 퀘스트 정보를 받는다.
//
//					if( !pQuestInfo )												// 퀘스트 정보가 유효하지 않다면,
//					{
//						ASSERTMSG(0, "퀘스트 알림이 등록 중 리스트 다이얼로그에 스트링 추가 중 퀘 정보 못가져 왔습니다.!") ;		// assert 에러 처리를 한다.
//						return ;													// 리턴 처리를 한다.
//					}
//					
//					nSubQuestCount = pQuestInfo->GetSubQuestCount() ;				// 퀘스트 정보로 부터 서브 퀘스트 카운트를 받는다.
//				}
//				else																// 퀘스트 스트링 정보가 유효하지 않으면,
//				{
//					continue ;														// 컨티뉴 한다.
//				}
//			}
//
//			if( nSubQuestCount > 1 )												// 적어도 서브퀘 최소 두 개는 되야 한다.
//			{
//				int nCountOfTreeCount = 0 ;											// 트리의 리스트의 카운트를 담을 변수를 선언하고, 0으로 세팅한다.
//				nCountOfTreeCount = pTree->list.GetCount() ;						// 트리의 리스트의 카운트를 받는다.
//
//				treePos = NULL ;													// 트리의 리스트에서 위치 정보를 받을 포인터를 선언하고 null 처리를 한다.	
//				treePos = pTree->list.GetHeadPosition() ;							// 트리의 리스트를 헤드로 세팅한다.
//
//				int count = 0 ;
//
//				while(treePos)														// 위치 정보가 유효할 동안 while문을 돌린다.
//				{
//					if( QUESTMGR->CheckQuestType(pTree->QuestIdx, count) == e_QT_MULTY_HUNT )
//					{
//						if( count != 0 )
//						{
//							pDescSubQuestInfo = NULL ;													// 서브 퀘스트 정보를 받을 포인터를 null처리 한다.
//							pDescSubQuestInfo = pQuest->GetSubQuestInfoArray(count) ;					// 카운트에 해당하는 서브 퀘스트를 받는다.
//
//							if( !pDescSubQuestInfo ) 
//							{
//								++count ;
//								continue ;											// 서브 퀘스트 정보가 유효하지 않으면 continue한다.
//							}
//						}
//
//						++count ;
//					}
//
//					pQuestString = NULL ;											// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
//					pQuestString = (CQuestString*)pTree->list.GetNext(treePos) ;	// 0번째 위치에 해당하는 퀘스트 스트링을 받는다.
//
//					if( pQuestString )												// 퀘스트 스트링 정보가 유효하면,
//					{
//						DWORD dwSubQuestIdx = pQuestString->GetSubID() ;			// 서브 퀘스트 인덱스를 받는다.		
//
//						if(dwSubQuestIdx == 0)										// 서브 퀘스트 인덱스가 0과 같으면, 
//						{
//							Color=RGB(0, 255, 255) ;								// 색상을 메인 퀘스트 색상으로 세팅한다.
//						}
//						else														// 그 외의 경우.
//						{
//							Color=RGB(255, 255, 255) ;								// 색상을 서브 퀘스트 색상으로 세팅한다.
//						}
//
//						memset(buf, 0, 256) ;										// 임시 버퍼를 초기화 한다.
//
//						if( dwSubQuestIdx != 0 )									// 서브 퀘스트 인덱스가 0이 아니면
//						{
//							sprintf(buf, "	- ");									// 임시 버퍼를 -로 세팅한다.
//						}
//
//						strcat(buf, pQuestString->GetTitleStr()) ;					// 스트링을 복사한다.
//
//						if( pQuestString->GetSubID() == nSubQuestCount-1 )
//						{
//							strcat(buf, "(완료)") ;
//						}
//						else
//						{
//							if( pQuestString->IsComplete() )
//							{
//								if( pQuestString->GetSubID() != 0 )
//								{
//									strcat(buf, "(완료)") ;
//								}
//							}
//							else
//							{
//								char tempBuf2[64] = {0, } ;
//
//								if( pQuestString->GetTotalCount() == 0 )
//								{
//									if( dwSubQuestIdx != 0 )
//									{
//										if(pQuestString->GetCount() != 0 )
//										{
//											sprintf(tempBuf2, "(%d)", pQuestString->GetCount()) ;
//										}
//									}
//								}
//								else
//								{
//									if( dwSubQuestIdx != 0 )
//									{
//										if(pQuestString->GetCount() != 0 )
//										{
//											if( pQuestString->GetCount() == pQuestString->GetTotalCount() )
//											{
//												strcat(tempBuf2, "(완료)") ;
//											}
//											else
//											{
//												sprintf(tempBuf2, "(%d/%d)", pQuestString->GetCount(), pQuestString->GetTotalCount()) ;
//											}
//										}
//									}
//								}
//
//								strcat(buf, tempBuf2) ;
//							}
//						}
//						
//						/*if( nSubQuestCount <= 2 && pQuestString->GetSubID() != 0)
//						{
//							strcat(buf, "(완료)") ;
//						}*/
//
//						if( nCountOfTreeCount > 2 )									// 트리의 리스트의 카운트가 2개를 넘으면,
//						{
//							if( dwSubQuestIdx != nSubQuestCount-1 )					// 서브 인덱스가 마지막 인덱스가 아니면,
//							{
//								m_pViewList->AddItemWithFont(buf, Color, 0) ;		// 리스트 다이얼로그에 아이템을 추가한다.
//								m_pViewList->SetExtendReduction(LD_EXTEND) ;		// 리스트 다이얼로그를 확장한다.
//							}
//						}
//						else														// 트리의 리스트 카운트가 2개 이하면,
//						{
//							m_pViewList->AddItemWithFont(buf, Color, 0) ;			// 리스트 다이얼로그에 아이템을 추가한다.
//							m_pViewList->SetExtendReduction(LD_EXTEND) ;			// 리스트 다이얼로그를 확장한다.
//						}
//
//						if( dwSubQuestIdx == 0 && pTree->State == eQTree_Close )
//						{
//							break ;
//						}
//					}
//				}
//			}
//		}
//	}
//}
//
////void CQuestQuickViewDialog::ResetQuickViewDialog()									// 퀘스트 리스트를 새로고침하는 함수.
////{
//// 	DWORD Color = 0;																// 색상 값을 담을 변수를 선언하고 0으로 세팅한다.
////	char buf[256] = { 0, };															// 임시 버퍼를 선언한다.	
////
////	QuestTree* pTree ;																// 트리 정보를 받을 포인터를 선언한다.
////	CQuestString* pQuestString ;													// 스트링 정보를 받을 포인터를 선언한다.
////	//QString* pLine ;
////
////	PTRLISTPOS pos = NULL ;															// 리스트 내의 위치를 담을 포인터를 선언하고 null처리를 한다.
////	PTRLISTPOS treePos = NULL ;														// 트리의 리스트 내의 위치를 담을 포인터를 선언하고 null처리를 한다.
////	PTRLISTPOS pLinePos = NULL ;													// 퀘스트 스트링의 라인을 받는 위치 포인터를 선언하고 null처리를 한다.
////
////	int nItemCount = 0 ;															// 알림이 리스트의 아이템 카운트를 담을 변수를 선언하고 0으로 세팅한다.
////	nItemCount = m_pViewList->GetItemCount() ;										// 알림이 리스트의 아이템 카운트를 받는다.
////
////	m_pViewList->RemoveAll() ;														// 퀘스트 리스트 다이얼로그를 모두 비운다.
////
////	for( int count = 0 ; count < nItemCount ; ++count )								// 알림이 리스트의 아이템 카운트 만큼 for문을 돌린다.
////	{
////		m_pViewList->SetExtendReduction(LD_REDUCTION) ;								// 리스트 다이얼로그를 축소한다.
////	}
////
////	pos = m_QuestTreeList.GetHeadPosition() ;										// 위치 포인터를 퀘스트의 헤드로 세팅한다.
////
////	while(pos)																		// 리스트의 다음 요소가 유효할 동안 while문을 돌린다.
////	{
////		pTree = NULL ;																// 트리 정보를 받을 포인터를 null 처리를 한다.
////		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// 위치 정보에 해당하는 트리를 받는다.
////
////		if( pTree )																	// 트리 정보가 유효하다면,
////		{
////			treePos = NULL ;														// 트리의 리스트에서 위치 정보를 받을 포인터를 선언하고 null 처리를 한다.	
////			treePos = pTree->list.FindIndex(0) ;									// 리스트의 첫 위치를 받는다.
////
////			int nSubQuestCount = 0 ;												// 서브 퀘스트 카운트를 받을 변수를 선언하고 0으로 세팅한다.
////
////			if( treePos )															// 트리의 위치 정보가 유효하면,
////			{
////				pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
////				pQuestString = (CQuestString*)pTree->list.GetAt(treePos) ;			// 트리 위치 포인터에 해당하는 퀘스트 스트링을 받는다.
////
////				if( pQuestString )													// 퀘스트 스트링 정보가 유효하면,
////				{
////					CQuestInfo* pQuestInfo = NULL ;									// 퀘스트 정보 포인터를 선언하고 null처리를 한다.
////					pQuestInfo = QUESTMGR->GetQuestInfo(pQuestString->GetQuestID()) ;// 퀘스트 매니져로 부터, 인자로 넘어온 퀘스트 번호로 퀘스트 정보를 받는다.
////
////					if( !pQuestInfo )												// 퀘스트 정보가 유효하지 않다면,
////					{
////						ASSERTMSG(0, "퀘스트 알림이 등록 중 리스트 다이얼로그에 스트링 추가 중 퀘 정보 못가져 왔습니다.!") ;		// assert 에러 처리를 한다.
////						return ;													// 리턴 처리를 한다.
////					}
////					
////					nSubQuestCount = pQuestInfo->GetSubQuestCount() ;				// 퀘스트 정보로 부터 서브 퀘스트 카운트를 받는다.
////				}
////				else																// 퀘스트 스트링 정보가 유효하지 않으면,
////				{
////					continue ;														// 컨티뉴 한다.
////				}
////			}
////
////			if( nSubQuestCount > 1 )												// 적어도 서브퀘 최소 두 개는 되야 한다.
////			{
////				int nCountOfTreeCount = 0 ;											// 트리의 리스트의 카운트를 담을 변수를 선언하고, 0으로 세팅한다.
////				nCountOfTreeCount = pTree->list.GetCount() ;						// 트리의 리스트의 카운트를 받는다.
////
////				treePos = NULL ;													// 트리의 리스트에서 위치 정보를 받을 포인터를 선언하고 null 처리를 한다.	
////				treePos = pTree->list.GetHeadPosition() ;							// 트리의 리스트를 헤드로 세팅한다.
////
////				while(treePos)														// 위치 정보가 유효할 동안 while문을 돌린다.
////				{
////					pQuestString = NULL ;											// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
////					pQuestString = (CQuestString*)pTree->list.GetNext(treePos) ;	// 0번째 위치에 해당하는 퀘스트 스트링을 받는다.
////
////					if( pQuestString )												// 퀘스트 스트링 정보가 유효하면,
////					{
////						DWORD dwSubQuestIdx = pQuestString->GetSubID() ;			// 서브 퀘스트 인덱스를 받는다.		
////
////						if(dwSubQuestIdx == 0)										// 서브 퀘스트 인덱스가 0과 같으면, 
////						{
////							Color=RGB(0, 255, 255) ;								// 색상을 메인 퀘스트 색상으로 세팅한다.
////						}
////						else														// 그 외의 경우.
////						{
////							Color=RGB(255, 255, 255) ;								// 색상을 서브 퀘스트 색상으로 세팅한다.
////						}
////
////						memset(buf, 0, 256) ;										// 임시 버퍼를 초기화 한다.
////
////						if( dwSubQuestIdx != 0 )									// 서브 퀘스트 인덱스가 0이 아니면
////						{
////							sprintf(buf, "	- ");									// 임시 버퍼를 -로 세팅한다.
////						}
////
////						strcat(buf, pQuestString->GetTitleStr()) ;					// 스트링을 복사한다.
////
////						if( pQuestString->GetSubID() == nSubQuestCount-1 )
////						{
////							strcat(buf, "(완료)") ;
////						}
////						else
////						{
////							if( pQuestString->IsComplete() )
////							{
////								if( pQuestString->GetSubID() != 0 )
////								{
////									strcat(buf, "(완료)") ;
////								}
////							}
////							else
////							{
////								char tempBuf2[64] = {0, } ;
////
////								if( pQuestString->GetTotalCount() == 0 )
////								{
////									if(pQuestString->GetCount() != 0 )
////									{
////										sprintf(tempBuf2, "(%d)", pQuestString->GetCount()) ;
////									}
////								}
////								else
////								{
////									if(pQuestString->GetCount() != 0 )
////									{
////										if( pQuestString->GetCount() == pQuestString->GetTotalCount() )
////										{
////											strcat(tempBuf2, "(완료)") ;
////										}
////										else
////										{
////											sprintf(tempBuf2, "(%d/%d)", pQuestString->GetCount(), pQuestString->GetTotalCount()) ;
////										}
////									}
////								}
////
////								strcat(buf, tempBuf2) ;
////							}
////						}
////						
////						/*if( nSubQuestCount <= 2 && pQuestString->GetSubID() != 0)
////						{
////							strcat(buf, "(완료)") ;
////						}*/
////
////						if( nCountOfTreeCount > 2 )									// 트리의 리스트의 카운트가 2개를 넘으면,
////						{
////							if( dwSubQuestIdx != nSubQuestCount-1 )					// 서브 인덱스가 마지막 인덱스가 아니면,
////							{
////								m_pViewList->AddItemWithFont(buf, Color, 0) ;		// 리스트 다이얼로그에 아이템을 추가한다.
////								m_pViewList->SetExtendReduction(LD_EXTEND) ;		// 리스트 다이얼로그를 확장한다.
////							}
////						}
////						else														// 트리의 리스트 카운트가 2개 이하면,
////						{
////							m_pViewList->AddItemWithFont(buf, Color, 0) ;			// 리스트 다이얼로그에 아이템을 추가한다.
////							m_pViewList->SetExtendReduction(LD_EXTEND) ;			// 리스트 다이얼로그를 확장한다.
////						}
////
////						if( dwSubQuestIdx == 0 && pTree->State == eQTree_Close )
////						{
////							break ;
////						}
////					}
////				}
////			}
////		}
////	}
////}
//
//void CQuestQuickViewDialog::CheckTreeState(int nSelectedLine)						// 트리가 열린 상태인지 닫힌 상태인지 체크해서 리스트를 갱신하는 함수.
//{
//	QuestTree* pTree = NULL ;														// 트리 정보를 받을 포인터를 선언하고 null처리를 한다.
//	CQuestString* pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 선언하고 null처리를 한다.
//
//	PTRLISTPOS pos = NULL ;															// 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
//	pos = m_QuestTreeList.GetHeadPosition() ;										// 퀘스트 트리 리스트를 헤드로 세팅한다.
//
//	PTRLISTPOS treePos ;
//
//	BOOL bResult = FALSE ;
//
//	int nCheckCount = -1 ;
//	while(pos)
//	{
//		pTree = NULL ;
//		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;
//
//		if( pTree )
//		{
//			++nCheckCount ;
//
//			treePos = NULL ;														// 트리의 리스트에서 위치 정보를 받을 포인터를 선언하고 null 처리를 한다.	
//			treePos = pTree->list.GetHeadPosition() ;
//
//			int nCheckCount2 = -1 ;
//
//			while(treePos)
//			{
//				pQuestString = NULL ;
//				pQuestString = (CQuestString*)pTree->list.GetNext(treePos) ;
//
//				if( pQuestString )
//				{
//					++nCheckCount2 ;
//
//					if(nCheckCount2 == 0 && nCheckCount == nSelectedLine)
//					{
//						bResult = TRUE ;
//
//						if( pTree->State == eQTree_Open )
//						{
//							pTree->State = eQTree_Close  ;
//						}
//						else
//						{
//							pTree->State = eQTree_Open ;
//						}
//
//						break ;
//					}
//				}
//			}
//		}
//	}
//
//	if( !bResult )
//	{
//		return ;
//	}
//
//	DWORD Color = 0;																// 색상 값을 담을 변수를 선언하고 0으로 세팅한다.
//	char buf[256] = { 0, };															// 임시 버퍼를 선언한다.	
//
//	int nItemCount = 0 ;															// 알림이 리스트의 아이템 카운트를 담을 변수를 선언하고 0으로 세팅한다.
//	nItemCount = m_pViewList->GetItemCount() ;										// 알림이 리스트의 아이템 카운트를 받는다.
//
//	for( int count = 0 ; count < nItemCount ; ++count )								// 알림이 리스트의 아이템 카운트 만큼 for문을 돌린다.
//	{
//		m_pViewList->SetExtendReduction(LD_REDUCTION) ;								// 리스트 다이얼로그를 축소한다.
//	}
//
//	m_pViewList->RemoveAll() ;														// 퀘스트 리스트 다이얼로그를 모두 비운다.
//
//	pos = NULL ;																	// 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
//	pos = m_QuestTreeList.GetHeadPosition() ;										// 퀘스트 트리 리스트를 헤드로 세팅한다.
//
//	while(pos)
//	{
//		pTree = NULL ;																// 트리 정보를 받을 포인터를 null처리를 한다.
//		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// 퀘스트 트리 리스트에서 퀘스트 트리 정보를 받는다.
//
//		if( pTree )
//		{
//			treePos = NULL ;														// 트리의 리스트에서 위치 정보를 받을 포인터를 선언하고 null 처리를 한다.	
//			treePos = pTree->list.FindIndex(0) ;									// 리스트의 첫 위치를 받는다.
//
//			int nSubQuestCount = 0 ;												// 서브 퀘스트 카운트를 받을 변수를 선언하고 0으로 세팅한다.
//
//			if( treePos )															// 트리의 위치 정보가 유효하면,
//			{
//				pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
//				pQuestString = (CQuestString*)pTree->list.GetAt(treePos) ;			// 트리 위치 포인터에 해당하는 퀘스트 스트링을 받는다.
//
//				if( pQuestString )													// 퀘스트 스트링 정보가 유효하면,
//				{
//					CQuestInfo* pQuestInfo = NULL ;									// 퀘스트 정보 포인터를 선언하고 null처리를 한다.
//					pQuestInfo = QUESTMGR->GetQuestInfo(pQuestString->GetQuestID()) ;// 퀘스트 매니져로 부터, 인자로 넘어온 퀘스트 번호로 퀘스트 정보를 받는다.
//
//					if( !pQuestInfo )												// 퀘스트 정보가 유효하지 않다면,
//					{
//						ASSERTMSG(0, "퀘스트 알림이 등록 중 리스트 다이얼로그에 스트링 추가 중 퀘 정보 못가져 왔습니다.!") ;		// assert 에러 처리를 한다.
//						return ;													// 리턴 처리를 한다.
//					}
//					
//					nSubQuestCount = pQuestInfo->GetSubQuestCount() ;				// 퀘스트 정보로 부터 서브 퀘스트 카운트를 받는다.
//				}
//				else																// 퀘스트 스트링 정보가 유효하지 않으면,
//				{
//					continue ;														// 컨티뉴 한다.
//				}
//			}
//			if( nSubQuestCount > 1 )												// 적어도 서브퀘 최소 두 개는 되야 한다.
//			{
//				int nCountOfTreeCount = 0 ;											// 트리의 리스트의 카운트를 담을 변수를 선언하고, 0으로 세팅한다.
//				nCountOfTreeCount = pTree->list.GetCount() ;						// 트리의 리스트의 카운트를 받는다.
//
//				cPtrList* pList = NULL ;												// 리스트 정보를 받을 포인터를 선언하고 null처리한다.
//				pList = &(pTree->list) ;												// 트리의 리스트 정보를 받는다.
//
//				treePos = NULL ;													// 트리 리스트의 위치 정보를 받을 포인터를 null처리를 한다.
//				treePos = pList->GetHeadPosition() ;								// 트리 리스트를 헤드로 세팅한다.
//
//				if( pTree->State == eQTree_Open )
//				{
//					while(treePos)
//					{
//						pQuestString = NULL ;										// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
//						pQuestString = (CQuestString*)pList->GetNext(treePos) ;		// 위치에 해당하는 퀘스트 스트링을 받는다.
//
//						if( pQuestString )
//						{
//							DWORD dwSubQuestIdx = pQuestString->GetSubID() ;		// 서브 퀘스트 인덱스를 받는다.		
//
//							if(dwSubQuestIdx == 0)									// 서브 퀘스트 인덱스가 0과 같으면, 
//							{
//								Color=RGB(255, 100, 0) ;							// 색상을 메인 퀘스트 색상으로 세팅한다.
//							}
//							else													// 그 외의 경우.
//							{
//								Color=RGB(255, 255, 255) ;							// 색상을 서브 퀘스트 색상으로 세팅한다.
//							}
//
//							memset(buf, 0, 256) ;									// 임시 버퍼를 초기화 한다.
//
//							if( dwSubQuestIdx != 0 )								// 서브 퀘스트 인덱스가 0이 아니면
//							{
//								sprintf(buf, "	- ");								// 임시 버퍼를 -로 세팅한다.
//							}
//
//							strcat(buf, pQuestString->GetTitleStr()) ;				// 스트링을 복사한다.
//
//							if( nCountOfTreeCount > 2 )								// 트리의 리스트의 카운트가 2개를 넘으면,
//							{
//								if( dwSubQuestIdx != nSubQuestCount-1 )				// 서브 인덱스가 마지막 인덱스가 아니면,
//								{
//									m_pViewList->SetExtendReduction(LD_EXTEND) ;	// 리스트 다이얼로그를 확장한다.
//									m_pViewList->AddItemWithFont(buf, Color, 0) ;	// 리스트 다이얼로그에 아이템을 추가한다.
//								}
//							}
//							else													// 트리의 리스트 카운트가 2개 이하면,
//							{
//								m_pViewList->SetExtendReduction(LD_EXTEND) ;		// 리스트 다이얼로그를 확장한다.
//								m_pViewList->AddItemWithFont(buf, Color, 0) ;		// 리스트 다이얼로그에 아이템을 추가한다.
//							}
//						}
//					}	
//				}
//				else
//				{
//					while(treePos)
//					{
//						pQuestString = NULL ;										// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
//						pQuestString = (CQuestString*)pList->GetNext(treePos) ;		// 위치에 해당하는 퀘스트 스트링을 받는다.
//
//						if( pQuestString )
//						{
//							DWORD dwSubQuestIdx = pQuestString->GetSubID() ;		// 서브 퀘스트 인덱스를 받는다.		
//
//							if(dwSubQuestIdx == 0)									// 서브 퀘스트 인덱스가 0과 같으면, 
//							{
//								Color=RGB(255, 100, 0) ;							// 색상을 메인 퀘스트 색상으로 세팅한다.
//
//								m_pViewList->SetExtendReduction(LD_EXTEND) ;		// 리스트 다이얼로그를 확장한다.
//								m_pViewList->AddItemWithFont(pQuestString->GetTitleStr(), Color, 0) ;	// 리스트 다이얼로그에 아이템을 추가한다.
//
//								break ;
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//
//
//
//	//int nCheckCount = -1 ;															// 선택 된 라인과 퀘스트 스트링을 체크하기 위한 변수를 선언하고 -1로 세팅한다.
//
//	//BOOL bSameTree = FALSE ;														// 알림이 리스트에서 선택 된 트리와 같은지 여부를 체크하기 위한 변수를 선언하고 FALSE세팅.
//
//	//int nItemCount = 0 ;															// 알림이 리스트의 아이템 카운트를 담을 변수를 선언하고 0으로 세팅한다.
//	//nItemCount = m_pViewList->GetItemCount() ;										// 알림이 리스트의 아이템 카운트를 받는다.
//
//	//for( int count = 0 ; count < nItemCount ; ++count )								// 알림이 리스트의 아이템 카운트 만큼 for문을 돌린다.
//	//{
//	//	m_pViewList->SetExtendReduction(LD_REDUCTION) ;								// 리스트 다이얼로그를 축소한다.
//	//}
//
//	//m_pViewList->RemoveAll() ;														// 퀘스트 리스트 다이얼로그를 모두 비운다.
//
//	//while(pos)																		// 위치 정보가 유효할 동안 while문을 돌린다.
//	//{
//	//	pTree = NULL ;																// 트리 정보를 받을 포인터를 null처리를 한다.
//	//	pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// 퀘스트 트리 리스트에서 퀘스트 트리 정보를 받는다.
//
//	//	if( pTree )
//	//	{
//	//		PTRLISTPOS treePos = NULL ;												// 트리의 리스트에서 위치 정보를 받을 포인터를 선언하고 null 처리를 한다.	
//	//		treePos = pTree->list.FindIndex(0) ;									// 리스트의 첫 위치를 받는다.
//
//	//		int nSubQuestCount = 0 ;												// 서브 퀘스트 카운트를 받을 변수를 선언하고 0으로 세팅한다.
//
//	//		if( treePos )															// 트리의 위치 정보가 유효하면,
//	//		{
//	//			pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
//	//			pQuestString = (CQuestString*)pTree->list.GetAt(treePos) ;			// 트리 위치 포인터에 해당하는 퀘스트 스트링을 받는다.
//
//	//			if( pQuestString )													// 퀘스트 스트링 정보가 유효하면,
//	//			{
//	//				CQuestInfo* pQuestInfo = NULL ;									// 퀘스트 정보 포인터를 선언하고 null처리를 한다.
//	//				pQuestInfo = QUESTMGR->GetQuestInfo(pQuestString->GetQuestID()) ;// 퀘스트 매니져로 부터, 인자로 넘어온 퀘스트 번호로 퀘스트 정보를 받는다.
//
//	//				if( !pQuestInfo )												// 퀘스트 정보가 유효하지 않다면,
//	//				{
//	//					ASSERTMSG(0, "퀘스트 알림이 등록 중 리스트 다이얼로그에 스트링 추가 중 퀘 정보 못가져 왔습니다.!") ;		// assert 에러 처리를 한다.
//	//					return ;													// 리턴 처리를 한다.
//	//				}
//	//				
//	//				nSubQuestCount = pQuestInfo->GetSubQuestCount() ;				// 퀘스트 정보로 부터 서브 퀘스트 카운트를 받는다.
//	//			}
//	//			else																// 퀘스트 스트링 정보가 유효하지 않으면,
//	//			{
//	//				continue ;														// 컨티뉴 한다.
//	//			}
//	//		}
//
//	//		cPtrList* pList = NULL ;												// 리스트 정보를 받을 포인터를 선언하고 null처리한다.
//	//		pList = &(pTree->list) ;												// 트리의 리스트 정보를 받는다.
//
//	//		int nCountOfTreeCount = 0 ;												// 트리의 리스트의 카운트를 담을 변수를 선언하고, 0으로 세팅한다.
//	//		nCountOfTreeCount = pTree->list.GetCount() ;							// 트리의 리스트의 카운트를 받는다.
//
//	//		treePos = NULL ;														// 트리 리스트의 위치 정보를 받을 포인터를 null처리를 한다.
//	//		treePos = pList->GetHeadPosition() ;									// 트리 리스트를 헤드로 세팅한다.
//
//	//		while(treePos)															// 위치 정보가 유효할 동안 while문을 돌린다.
//	//		{	
//	//			pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
//	//			pQuestString = (CQuestString*)pList->GetNext(treePos) ;				// 위치에 해당하는 퀘스트 스트링을 받는다.
//
//	//			if( pQuestString )													// 퀘스트 스트링 정보가 유효하면,
//	//			{
//	//				++nCheckCount ;													// 체크 카운트를 증가한다.
//
//	//				if( nSelectedLine == nCheckCount )								// 알림이 리스트에서 선택 된 라인과, 체크 카운트가 같다면,
//	//				{
//	//					bSameTree = TRUE ;											// 같은 트리라는 체크 변수를 TRUE로 세팅한다.
//
//	//					if( pQuestString->GetSubID() == 0 )							// 현재 퀘스트 스트링의 서브 아이디가 0이라면,(큰 제목 스트링이다)
//	//					{
//	//						if( pTree->State == eQTree_Open )						// 현재 트리가 열려있는 상태라면,
//	//						{
//	//							pTree->State = eQTree_Close ;						// 현재 트리를 닫힌 상태로 세팅한다.
//	//						}
//	//						else													// 현재 트리가 닫혀있는 상태라면,
//	//						{
//	//							pTree->State = eQTree_Open ;						// 현재 트리를 열린 상태로 세팅한다.
//	//						}
//	//					}
//
//	//					Color=RGB(255, 100, 0) ;									// 색상을 메인 퀘스트 색상으로 세팅한다.
//
//	//					memset(buf, 0, 256) ;										// 임시 버퍼를 초기화 한다.
//
//	//					strcat(buf, pQuestString->GetTitleStr()) ;					// 스트링을 복사한다.
//
//	//					m_pViewList->SetExtendReduction(LD_EXTEND) ;				// 리스트 다이얼로그를 확장한다.
//	//					m_pViewList->AddItemWithFont(buf, Color, 0) ;				// 리스트 다이얼로그에 아이템을 추가한다.
//	//				}
//	//				else															// 알림이 리스트에서 선택 된 라인과 다른 트리라면,
//	//				{
//	//					if( bSameTree )												// 알림이 리스트에서 선택 된 트리의 하위 제목이라면,
//	//					{
//	//						if( pTree->State == eQTree_Open )						// 현재 트리가 열려있는 상태라면,
//	//						{
//	//							DWORD dwSubQuestIdx = pQuestString->GetSubID() ;	// 서브 퀘스트 인덱스를 받는다.		
//
//	//							if(dwSubQuestIdx == 0)								// 서브 퀘스트 인덱스가 0과 같으면, 
//	//							{
//	//								Color=RGB(255, 100, 0) ;						// 색상을 메인 퀘스트 색상으로 세팅한다.
//	//							}
//	//							else												// 그 외의 경우.
//	//							{
//	//								Color=RGB(255, 255, 255) ;						// 색상을 서브 퀘스트 색상으로 세팅한다.
//	//							}
//
//	//							memset(buf, 0, 256) ;								// 임시 버퍼를 초기화 한다.
//
//	//							if( dwSubQuestIdx != 0 )							// 서브 퀘스트 인덱스가 0이 아니면
//	//							{
//	//								sprintf(buf, "	- ");							// 임시 버퍼를 -로 세팅한다.
//	//							}
//
//	//							strcat(buf, pQuestString->GetTitleStr()) ;			// 스트링을 복사한다.
//
//	//							if( nCountOfTreeCount > 2 )							// 트리의 리스트의 카운트가 2개를 넘으면,
//	//							{
//	//								if( dwSubQuestIdx != nSubQuestCount-1 )			// 서브 인덱스가 마지막 인덱스가 아니면,
//	//								{
//	//									m_pViewList->SetExtendReduction(LD_EXTEND) ;// 리스트 다이얼로그를 확장한다.
//	//									m_pViewList->AddItemWithFont(buf, Color, 0);// 리스트 다이얼로그에 아이템을 추가한다.
//	//								}
//	//							}
//	//							else												// 트리의 리스트 카운트가 2개 이하면,
//	//							{
//	//								m_pViewList->SetExtendReduction(LD_EXTEND) ;	// 리스트 다이얼로그를 확장한다.
//	//								m_pViewList->AddItemWithFont(buf, Color, 0) ;	// 리스트 다이얼로그에 아이템을 추가한다.
//	//							}
//	//						}
//	//					}
//	//					else
//	//					{
//	//						DWORD dwSubQuestIdx = pQuestString->GetSubID() ;		// 서브 퀘스트 인덱스를 받는다.		
//
//	//						if(dwSubQuestIdx == 0)									// 서브 퀘스트 인덱스가 0과 같으면, 
//	//						{
//	//							Color=RGB(255, 100, 0) ;							// 색상을 메인 퀘스트 색상으로 세팅한다.
//	//						}
//	//						else													// 그 외의 경우.
//	//						{
//	//							Color=RGB(255, 255, 255) ;							// 색상을 서브 퀘스트 색상으로 세팅한다.
//	//						}
//
//	//						memset(buf, 0, 256) ;									// 임시 버퍼를 초기화 한다.
//
//	//						if( dwSubQuestIdx != 0 )								// 서브 퀘스트 인덱스가 0이 아니면
//	//						{
//	//							sprintf(buf, "	- ");								// 임시 버퍼를 -로 세팅한다.
//	//						}
//
//	//						strcat(buf, pQuestString->GetTitleStr()) ;				// 스트링을 복사한다.
//
//	//						if( nCountOfTreeCount > 2 )								// 트리의 리스트의 카운트가 2개를 넘으면,
//	//						{
//	//							if( dwSubQuestIdx != nSubQuestCount-1 )				// 서브 인덱스가 마지막 인덱스가 아니면,
//	//							{
//	//								m_pViewList->SetExtendReduction(LD_EXTEND) ;	// 리스트 다이얼로그를 확장한다.
//	//								m_pViewList->AddItemWithFont(buf, Color, 0) ;	// 리스트 다이얼로그에 아이템을 추가한다.
//	//							}
//	//						}
//	//						else													// 트리의 리스트 카운트가 2개 이하면,
//	//						{
//	//							m_pViewList->SetExtendReduction(LD_EXTEND) ;		// 리스트 다이얼로그를 확장한다.
//	//							m_pViewList->AddItemWithFont(buf, Color, 0) ;		// 리스트 다이얼로그에 아이템을 추가한다.
//	//						}
//	//					}
//	//				}
//	//			}
//	//		}
//
//	//		bSameTree = FALSE ;														// 같은 트리라는 체크 변수를 TRUE로 세팅한다.
//	//	}
//	//}
//}
//
////void CQuestQuickViewDialog::MainDataLoad()											// 퀵 뷰에 등록 되어 있는 퀘스트 데이터를 로딩하는 함수.
////{
////	/*DWORD dwQuestIdx = 0 ;
////
////	for( int count = 0 ; count < MAX_QUESTTREECOUNT ; ++count )
////	{
////		dwQuestIdx = QUESTMGR->GetQuestIDFromQuickView(count) ;
////
////		if( dwQuestIdx != 0 )
////		{
////			RegistQuest(dwQuestIdx) ;
////		}
////	}*/
////}
//
//void CQuestQuickViewDialog::LoadQuickViewInfoFromFile(sQUEST_INFO* pInfo)
//{
//	if( !pInfo )
//	{
//		return ;
//	}
//
//	CQuestString* pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 선언하고 null처리를 한다.
//
//	CQuest* pQuest = QUESTMGR->GetQuest( pInfo->dwQuestIdx );							// 퀘스트 정보를 받는다.
//
//	if( !pQuest ) return ;															// 퀘스트 정보가 유효하지 않으면, return 처리 한다.
//
//	CSubQuestInfo* pDescSubQuestInfo ;												// 서브 퀘스트 정보를 받을 포인터를 선언한다.
//
//	if( m_QuestTreeList.GetCount() > 0 )
//	{
//		Release() ;
//	}
//
//	for( int index = 0 ; index < MAX_QUESTTREECOUNT ; ++index )
//	{
//		DWORD dwQuestIdx = pInfo[index].dwQuestIdx ;
//
//		if( CheckSameQuest(dwQuestIdx) )												// 퀘스트 리스트에 인자로 넘어온 퀘스트와 같은 퀘스트가 있다면,
//		{
//			return ;																	// 리턴 처리를 한다.
//		}
//
//		int nTreeCount = 0 ;															// 퀘스트 트리 카운트를 담을 변수를 선언하고 0으로 세팅한다.
//
//		nTreeCount = m_QuestTreeList.GetCount() ;												// 퀘스트를 담는 리스트의 카운트를 받는다.	
//
//		if( nTreeCount >= MAX_QUESTTREECOUNT )											// 트리 카운트가 최대 트리 수 이상이면,
//		{
//			return ;
//		}
//		else																			// 트리 개수가 5개 미만이면,
//		{
//			QuestTree* pTree = new QuestTree ;											// 트리를 생성한다.
//
//			CQuestInfo* pQuestInfo = NULL ;												// 퀘스트 정보 포인터를 선언하고 null처리를 한다.
//			pQuestInfo = QUESTMGR->GetQuestInfo(dwQuestIdx) ;							// 퀘스트 매니져로 부터, 인자로 넘어온 퀘스트 번호로 퀘스트 정보를 받는다.
//
//			if( !pQuestInfo )															// 퀘스트 정보가 유효하지 않다면,
//			{
//				ASSERTMSG(0, "퀘스트 알림이 등록 중 퀘스트 정보를 받아오지 못했습니다.!") ;		// assert 에러 처리를 한다.
//				return ;																// 리턴 처리를 한다.
//			}
//
//			int nSubQuestCount = 0 ;													// 서브 퀘스트 카운트를 받을 변수를 선언하고 0으로 세팅한다.
//			nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// 퀘스트 정보로 부터 서브 퀘스트 카운트를 받는다.
//
//			CQuestString* pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 선언하고 null처리를 한다.
//			
//			for(int count = 0 ; count < nSubQuestCount ; ++count )						// 서브 퀘스트 카운트 만큼 for문을 돌린다.
//			{
//				if( QUESTMGR->CheckQuestType(dwQuestIdx, count) == e_QT_MULTY_HUNT )
//				{
//					if( count != 0 )
//					{
//						pDescSubQuestInfo = NULL ;													// 서브 퀘스트 정보를 받을 포인터를 null처리 한다.
//						pDescSubQuestInfo = pQuest->GetSubQuestInfoArray(count) ;					// 카운트에 해당하는 서브 퀘스트를 받는다.
//
//						if( !pDescSubQuestInfo ) continue ;											// 서브 퀘스트 정보가 유효하지 않으면 continue한다.
//					}
//				}
//
//				pQuestString = NULL ;													// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
//				pQuestString = QUESTMGR->GetQuestString(dwQuestIdx, count) ;			// 퀘스트 인덱스와, 서브 퀘스트 인덱스로 퀘스트 스트링을 받는다.
//	            
//				if( pQuestString )														// 퀘스트 스트링 정보가 유효하다면,
//				{
//					CQuestString* pNewQuestString = new CQuestString ;						// 퀘스트 스트링을 생성한다.
//
//					DWORD dwMainQuestIdx = pQuestString->GetQuestID() ;
//					DWORD dwSubQuestIdx  = pQuestString->GetSubID() ;
//
//					pNewQuestString->SetQuestID( dwMainQuestIdx ) ;
//					pNewQuestString->SetSubID( dwSubQuestIdx ) ;
//					pNewQuestString->SetIndex( dwMainQuestIdx, dwSubQuestIdx ) ;
//
//					pNewQuestString->SetComplete( pQuestString->IsComplete() ) ;
//
//					pNewQuestString->SetCount( pQuestString->GetCount() ) ;
//					pNewQuestString->SetTotalCount( pQuestString->GetTotalCount() ) ;
//
//
//					ITEM* pItem = NULL ;
//
//					cPtrList* titleList = NULL ;
//					titleList = pQuestString->GetTitle() ;
//
//					if( titleList )
//					{
//						PTRLISTPOS titlePos = NULL ;
//						titlePos = titleList->GetHeadPosition() ;
//
//						while(titlePos)
//						{
//							pItem = NULL ;
//							pItem = (ITEM*)titleList->GetNext(titlePos) ;
//
//							if( pItem )
//							{
//								ITEM* pNewItem = new ITEM ;
//
//								//memcpy(pNewItem, pItem, sizeof(ITEM)) ;
//
//								memcpy(pNewItem->string, pItem->string, sizeof(pItem->string)) ;
//								pNewItem->line		= pItem->line ;
//								pNewItem->nAlpha	= pItem->nAlpha ;
//								pNewItem->nFontIdx	= pItem->nFontIdx ;
//								pNewItem->rgb		= pItem->rgb ;
//
//								pNewQuestString->GetTitle()->AddTail(pNewItem) ;
//							}
//						}
//					}
//
//					//cPtrList* descList = NULL ;
//					//descList = pQuestString->GetDesc() ;
//
//					//if( descList )
//					//{
//					//	PTRLISTPOS descPos = NULL ;
//					//	descPos = descList->GetHeadPosition() ;
//
//					//	while(descPos)
//					//	{
//					//		pItem = NULL ;
//					//		pItem = (ITEM*)descList->GetNext(descPos) ;
//
//					//		if( pItem )
//					//		{
//					//			ITEM* pNewItem = new ITEM ;
//
//					//			//memcpy(pNewItem, pItem, sizeof(ITEM)) ;
//
//					//			memcpy(pNewItem->string, pItem->string, sizeof(pItem->string)) ;
//					//			pNewItem->line		= pItem->line ;
//					//			pNewItem->nAlpha	= pItem->nAlpha ;
//					//			pNewItem->nFontIdx	= pItem->nFontIdx ;
//					//			pNewItem->rgb		= pItem->rgb ;
//
//					//			pNewQuestString->GetDesc()->AddTail(pNewItem) ;
//					//		}
//					//	}
//					//}
//
//					pNewQuestString->SetTreeTitle( FALSE ) ;
//
//					cPtrList* pList = pQuestString->GetTitle() ;						// 퀘스트 스트링의 타이틀 리스트를 받는다.
//
//					PTRLISTPOS titlePos = NULL ;										// 타이틀 리스트 상의 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
//					titlePos = pList->GetHeadPosition() ;								// 타이틀 리스트를 헤드로 세팅하고 위치 정보를 받는다.
//
//					ITEM* pQString = NULL ;											// Q스트링을 받을 포인터를 선언하고 null처리를 한다.
//
//					while(titlePos)														// 위치 정보가 유효할 동안 while문을 돌린다.
//					{
//						pQString = NULL ;												// Q스트링 포인터를 null처리를 한다.
//						pQString = (ITEM*)pList->GetNext(titlePos) ;					// 위치에 따른 Q스트링을 받는다.
//
//						if( pQString )													// Q스트링 정보가 유효하다면,
//						{
//							pNewQuestString->SetTitleStr( pQString->string ) ;					// Q스트링의 문자열을 받아서 새로운 퀘스트 스트링에 타이틀로 세팅한다.
//
//							break ;
//						}
//					}
//
//					pNewQuestString->SetComplete(pInfo[index].bComplete[count]) ;
//					pNewQuestString->SetCount(pInfo[index].nCount[count]) ;
//					pNewQuestString->SetTotalCount(pInfo[index].nTotalCount[count]) ;
//
//					pQuestString->SetComplete(pInfo[index].bComplete[count]) ;
//					pQuestString->SetCount(pInfo[index].nCount[count]) ;
//					pQuestString->SetTotalCount(pInfo[index].nTotalCount[count]) ;
//
//					pTree->list.AddTail(pNewQuestString) ;									// 트리의 리스트에 퀘스트 스트링을 추가한다.
//
//				}
//			}
//
//			pTree->State = eQTree_Open ;												// 트리 상태를 열린 상태로 세팅한다.
//			pTree->QuestIdx = dwQuestIdx ;												// 트리에 퀘스트 인덱스를 세팅한다.
//			m_QuestTreeList.AddTail(pTree) ;														// 퀘스트 리스트에 트리를 추가한다.
//
//			//QUESTMGR->RegistQuestIDToQuickView(index, dwQuestIdx) ;									// 알림창에 등록 된 퀘스트 인덱스를 퀘스트 매니져가 관리 한다.
//		}
//
//		//pQuickViewDlg->ResetLoadedQuickViewDialog() ;									// 퀵 뷰를 새로고침한다.
//		ResetQuickViewDialog() ;
//	}
//}
//
////void CQuestQuickViewDialog::LoadQuickViewInfoFromFile(sQUEST_INFO* pInfo)
////{
////	if( !pInfo )
////	{
////		return ;
////	}
////
////	for( int index = 0 ; index < MAX_QUESTTREECOUNT ; ++index )
////	{
////		DWORD dwQuestIdx = pInfo[index].dwQuestIdx ;
////
////		if( CheckSameQuest(dwQuestIdx) )												// 퀘스트 리스트에 인자로 넘어온 퀘스트와 같은 퀘스트가 있다면,
////		{
////			return ;																	// 리턴 처리를 한다.
////		}
////
////		int nTreeCount = 0 ;															// 퀘스트 트리 카운트를 담을 변수를 선언하고 0으로 세팅한다.
////
////		nTreeCount = m_QuestTreeList.GetCount() ;												// 퀘스트를 담는 리스트의 카운트를 받는다.	
////
////		if( nTreeCount >= MAX_QUESTTREECOUNT )											// 트리 카운트가 최대 트리 수 이상이면,
////		{
////			return ;
////		}
////		else																			// 트리 개수가 5개 미만이면,
////		{
////			QuestTree* pTree = new QuestTree ;											// 트리를 생성한다.
////
////			CQuestInfo* pQuestInfo = NULL ;												// 퀘스트 정보 포인터를 선언하고 null처리를 한다.
////			pQuestInfo = QUESTMGR->GetQuestInfo(dwQuestIdx) ;							// 퀘스트 매니져로 부터, 인자로 넘어온 퀘스트 번호로 퀘스트 정보를 받는다.
////
////			if( !pQuestInfo )															// 퀘스트 정보가 유효하지 않다면,
////			{
////				ASSERTMSG(0, "퀘스트 알림이 등록 중 퀘스트 정보를 받아오지 못했습니다.!") ;		// assert 에러 처리를 한다.
////				return ;																// 리턴 처리를 한다.
////			}
////
////			int nSubQuestCount = 0 ;													// 서브 퀘스트 카운트를 받을 변수를 선언하고 0으로 세팅한다.
////			nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// 퀘스트 정보로 부터 서브 퀘스트 카운트를 받는다.
////
////			CQuestString* pQuestString ;												// 퀘스트 스트링 정보를 받을 포인터를 선언한다.
////			
////			for(int count = 0 ; count < nSubQuestCount ; ++count )						// 서브 퀘스트 카운트 만큼 for문을 돌린다.
////			{
////				pQuestString = NULL ;													// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
////				pQuestString = QUESTMGR->GetQuestString(dwQuestIdx, count) ;			// 퀘스트 인덱스와, 서브 퀘스트 인덱스로 퀘스트 스트링을 받는다.
////	            
////				if( pQuestString )														// 퀘스트 스트링 정보가 유효하다면,
////				{
////					CQuestString* pNewString = new CQuestString ;						// 퀘스트 스트링을 생성한다.
////					memcpy(pNewString, pQuestString, sizeof(CQuestString)) ;			// 새로 생성한 퀘스트 스트링에, 현재의 퀘스트 스트링을 복사한다.
////
////					cPtrList* pList = pQuestString->GetTitle() ;						// 퀘스트 스트링의 타이틀 리스트를 받는다.
////
////					PTRLISTPOS titlePos = NULL ;										// 타이틀 리스트 상의 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
////					titlePos = pList->GetHeadPosition() ;								// 타이틀 리스트를 헤드로 세팅하고 위치 정보를 받는다.
////
////					ITEM* pQString = NULL ;											// Q스트링을 받을 포인터를 선언하고 null처리를 한다.
////
////					while(titlePos)														// 위치 정보가 유효할 동안 while문을 돌린다.
////					{
////						pQString = NULL ;												// Q스트링 포인터를 null처리를 한다.
////						pQString = (ITEM*)pList->GetNext(titlePos) ;					// 위치에 따른 Q스트링을 받는다.
////
////						if( pQString )													// Q스트링 정보가 유효하다면,
////						{
////							pNewString->SetTitleStr( pQString->string ) ;					// Q스트링의 문자열을 받아서 새로운 퀘스트 스트링에 타이틀로 세팅한다.
////						}
////					}
////
////					pNewString->SetComplete(pInfo[index].bComplete[count]) ;
////					pNewString->SetCount(pInfo[index].nCount[count]) ;
////					pNewString->SetTotalCount(pInfo[index].nTotalCount[count]) ;
////
////					pQuestString->SetComplete(pInfo[index].bComplete[count]) ;
////					pQuestString->SetCount(pInfo[index].nCount[count]) ;
////					pQuestString->SetTotalCount(pInfo[index].nTotalCount[count]) ;
////
////					pTree->list.AddTail(pNewString) ;									// 트리의 리스트에 퀘스트 스트링을 추가한다.
////
////				}
////			}
////
////			pTree->State = eQTree_Open ;												// 트리 상태를 열린 상태로 세팅한다.
////			pTree->QuestIdx = dwQuestIdx ;												// 트리에 퀘스트 인덱스를 세팅한다.
////			m_QuestTreeList.AddTail(pTree) ;														// 퀘스트 리스트에 트리를 추가한다.
////
////			QUESTMGR->RegistQuestIDToQuickView(index, dwQuestIdx) ;									// 알림창에 등록 된 퀘스트 인덱스를 퀘스트 매니져가 관리 한다.
////		}
////
////		//pQuickViewDlg->ResetLoadedQuickViewDialog() ;									// 퀵 뷰를 새로고침한다.
////		ResetQuickViewDialog() ;
////	}
////}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
