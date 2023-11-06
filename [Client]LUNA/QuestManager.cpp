// QuestManager.cpp: implementation of the CQuestManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"																		// stdafx 헤더를 불러온다.
#include "QuestManager.h"																// 퀘스트 매니져 헤더를 불러온다.
#include "./Interface/GameNotifyManager.h"												// 게임 공지 매니져 클래스 헤더를 불러온다.

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include "MHFile.h"																		// 묵향 파일 헤더를 불러온다.

#include "..\[CC]Quest\QuestScriptLoader.h"												// 퀘스트 스크립트 로더 헤더를 불러온다.
#include "..\[CC]Quest\QuestString.h"													// 퀘스트 스트링 헤더를 불러온다.
#include "..\[CC]Quest\QuestInfo.h"														// 퀘스트 정보 헤더를 불러온다.
#include "..\[CC]Quest\SubQuestInfo.h"													// 서브 퀘스트 정보 헤더를 불러온다.
#include "..\[CC]Quest\QuestExecute.h"													// 퀘스트 실행 클래스 헤더를 불러온다.
#include "..\[CC]Quest\QuestNpc.h"														// 퀘스트 npc 헤더를 불러온다.
#include "..\[CC]Quest\QuestNpcData.h"													// 퀘스트 npc 데이터 헤더를 불러온다.
#include "..\[CC]Quest\QuestNpcScript.h"												// 퀘스트 npc 스크립트 클래스 헤더를 불러온다.
#include "..\[CC]Quest\QuestNpcScriptData.h"											// 퀘스트 npc 스크립트 데이터 클래스 헤더를 불러온다.

#include "Quest.h"																		// 퀘스트 헤더를 불러온다.
#include "QuestUpdater.h"																// 퀘스트 업데이터 헤더를 불러온다.
#include "ObjectManager.h"																// 오브젝트 매니져 헤더를 불러온다.
#include "ChatManager.h"																// 채팅 매니져 헤더를 불러온다.
#include "QuestItemManager.h"															// 퀘스트 아이템 매니져 헤더를 불러온다.
#include "UserInfoManager.h"

//
#include "GameIn.h"																		// 게임 인 헤더를 불러온다.
#include "ItemManager.h"																// 아이템 매니져 헤더를 불러온다.
#include "InventoryExDialog.h"															// 인벤토리 다이얼로그 헤더를 불러온다.
#include "WindowIdEnum.h"																// 윈도우 아이디 이넘 헤더를 불러온다.
#include "./Interface/cWindowManager.h"													// 윈도우 매니져 헤더를 불러온다.
#include "cDivideBox.h"																	// 나누기 박스 헤더를 불러온다.
#include "cMsgBox.h"																	// 메시지 박스 헤더를 불러온다.
#include "QuickManager.h"																// 퀵 매니져 헤더를 불러온다.
#include "Npc.h"																		// npc 헤더를 불러온다.
#include "MHMap.h"																		// 묵향 맵 헤더를 불러온다.

#include "MiniMapDlg.h"																	// 미니맵 다이얼로그 헤더를 불러온다.

// 061220 LYW --- Delete this code.
//#include "MainBarDialog.h"
#include "QuestDialog.h"																// 퀘스트 다이얼로그 헤더를 불러온다.
#include "QuestQuickViewDialog.h"														// 퀘스트 알림이 다이얼로그 크래스 헤더를 불러온다.
#include "StatusIconDlg.h"																// 상태 아이콘 다이얼로그 헤더를 불러온다.
#include "NpcScriptDialog.h"															// npc 스크립트 다이얼로그 헤더를 불러온다.

// 070329 LYW --- ItemManager : Include NpcImageDlg.
#include "NpcImageDlg.h"																// npc 이미지 다이얼로그 헤더를 불러온다.

#include "QuestTotalDialog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CQuestManager);																// 퀘스트 매니져를 글로벌 톤으로 함.

CQuestManager::CQuestManager()															// 생성자 함수.
{
	m_QuestInfoTable.Initialize(MAX_QUEST);												// 최대 퀘스트 수만큼(200) 퀘스트 정보 테이블을 초기화 한다.
	m_QuestStringTable.Initialize(200);													// 최대 퀘스트 수만큼(200) 퀘스트 스트링 테이블을 초기화 한다.
	m_QuestTable.Initialize(MAX_QUEST);													// 최대 퀘스트 수만큼(200) 퀘스트 테이블을 초기화 한다.
	m_QuestNpcTable.Initialize(100);													// 100개 만큼 퀘스트 npc 테이블을 초기화 한다.

	m_QuestUpdater = new CQuestUpdater;													// 퀘스트 업데이터를 새로 할당한다.

	memset( m_pNpcArray, 0, sizeof(CNpc*)*32 );											// npc 정보를 32만큼 초기화 한다.
	m_dwMaxNpc = 0;																		// 최대 npc를 담을 변수를 0으로 초기화 한다.
	
	m_dwLastMsg = 0;																	// 마지막 메시지를 담을 변수를 0으로 초기화 한다.

	for( int count = 0 ; count < MAX_QUICKVIEW_QUEST ; ++count )					// 퀘스트 알림이 다이얼로그에 등록 가능한 수 만큼 for문을 돌린다.
	{
		m_QuickViewQuestID[count] = 0 ;											// 퀘스트 아이디를 초기화 한다.
	}
}

CQuestManager::~CQuestManager()															// 소멸자 함수.
{
	Release();																			// 해제 함수를 호출한다.
}

void CQuestManager::Release()															// 해제 함수.
{
	// quest info																		// 퀘스트 정보 파트.
	CQuestInfo* pQuestInfo = NULL;														// 퀘스트 정보를 받을 포인터를 선언하고, null 처리를 한다.

	m_QuestInfoTable.SetPositionHead();													// 퀘스트 정보 테이블을 헤드로 세팅한다.
	while( pQuestInfo = m_QuestInfoTable.GetData() )									// 퀘스트 정보가 있는동안 while문을 돌린다.
	{
		delete pQuestInfo;																// 퀘스트 정보를 삭제한다.
	}
	m_QuestInfoTable.RemoveAll();														// 퀘스트 정보 테이블을 모두 비운다.

	//// quest string																		// 퀘스트 스트링 파트.
	//CQuestString* pQuestString = NULL;													// 퀘스트 스트링을 받을 포인터를 선언하고, null 처리를 한다.
	//m_QuestStringTable.SetPositionHead();												// 퀘스트 스트링 테이블을 헤드로 세팅한다.
	//while( pQuestString = m_QuestStringTable.GetData() )								// 퀘스트 스트링 정보가 있는동안 while문을 돌린다.
	//{
	//	//delete pQuestString;
	//	SAFE_DELETE( pQuestString );													// 안전하게 퀘스트 스트링을 삭제 한다.
	//}
	//m_QuestStringTable.RemoveAll();														// 퀘스트 스트링 테이블을 모두 비운다.

	// quest string																		// 퀘스트 스트링 파트.

	CQuestString* pQuestString = NULL;													// 퀘스트 스트링을 받을 포인터를 선언하고, null 처리를 한다.
	//int nDeleteCount = 0 ;
	m_QuestStringTable.SetPositionHead();												// 퀘스트 스트링 테이블을 헤드로 세팅한다.
	while( pQuestString = m_QuestStringTable.GetData() )								// 퀘스트 스트링 정보가 있는동안 while문을 돌린다.
	{
		//m_QuestStringTable.Remove(nDeleteCount) ;

		delete pQuestString ;
		pQuestString = NULL ;
		//++nDeleteCount ;
	}
	m_QuestStringTable.RemoveAll();														// 퀘스트 스트링 테이블을 모두 비운다.


	// quest																			// 퀘스트 파트.
	CQuest* pQuest = NULL;																// 퀘스트를 받을 포인터를 선언하고, null처리를 한다.
	m_QuestTable.SetPositionHead();														// 퀘스트 테이블을 헤드로 세팅한다.
	while( pQuest = m_QuestTable.GetData() )											// 퀘스트가 있는동안 while문을 돌린다.
	{
		delete pQuest;																	// 퀘스트를 삭제한다.
	}
	m_QuestTable.RemoveAll();															// 퀘스트 테이블을 모두 비운다.

	// quest npcinfo																	// 퀘스트 npc정보 파트.
	QUESTNPCINFO* pData = NULL;															// 퀘스트 npc정보를 받을 포인터를 선언하고, null 처리를 한다.
	m_QuestNpcTable.SetPositionHead();													// 퀘스트 npc테이블을 헤드로 세팅한다.
	while( pData = m_QuestNpcTable.GetData() )											// 퀘스트 npc 정보가 있을 동안 while문을 돌린다.
	{
		delete pData;																	// 퀘스트 npc정보를 삭제한다.
	}
	m_QuestNpcTable.RemoveAll();														// 퀘스트 npc 테이블을 모두 비운다.

	// updater																			// 업데이터 파트.
	if( m_QuestUpdater )																// 퀘스트 업데이터 정보가 유효하면,
	{
		delete m_QuestUpdater;															// 퀘스트 업데이터 메모리를 삭제한다.
		m_QuestUpdater = NULL;															// 퀘스트 업데이터 포인터를 null처리를 한다.
	}

	m_dwLastMsg = 0;																	// 마지막 메시지 번호를 0으로 세팅한다.
}

BOOL CQuestManager::LoadQuestScript( char* strFileName )								// 퀘스트 스크립트를 로딩하는 함수.
{
	CMHFile file;																		// 묵향 파일을 선언한다.

	if( !file.Init( strFileName, "rb") )												// 들어온 파일이름과, 열기 모드에 따라 파일을 열고, 결과를 받는다.
	{																					// 실패했다면,
		return FALSE;																	// FALSE를 리턴한다.
	}

	char Token[256];																	// 임시 버퍼를 선언한다.

	DWORD dwQuestNum = 0;																// 퀘스트 번호를 받을 변수를 선언, 0으로 초기화 한다.

	int nMapNum = 0 ;

	DWORD dwNpcIdx = 0 ;

	while( !file.IsEOF() )																// 파일의 끝이 아닌 동안 while문을 돌린다.
	{
		file.GetString( Token );														// 스트링을 읽어 임시 버퍼에 담는다.

		if( strcmp(Token,"$QUEST") == 0 )												// 읽어들인 스트링이 $QUEST와 같다면,
		{
			dwQuestNum = file.GetDword();												// 퀘스트 번호를 읽어들인다.

			nMapNum = file.GetInt() ;													// 맵 번호를 읽어들인다.

			dwNpcIdx = file.GetDword() ;												// npc 인덱스를 읽어들인다.

			CQuestInfo* pQuestInfo = CQuestScriptLoader::LoadQuestInfo( &file, dwQuestNum ) ;

			if( pQuestInfo )
			{
				pQuestInfo->SetMapNum(nMapNum) ;
				pQuestInfo->SetNpcIdx(dwNpcIdx) ;
			}

			m_QuestInfoTable.Add( pQuestInfo, dwQuestNum );	// 퀘스트 정보 테이블에 퀘스트를 담는다.
		}
	}

	CreateAllQuestFromScript();															// 스크립트로 부터 모든 퀘스트를 생성한다.

	return TRUE;																		// TRUE를 리턴한다.
}

//BOOL CQuestManager::LoadQuestString(char* strFileName)									// 퀘스트 스트링을 로딩하는 함수.
//{
//	CMHFile file;																		// 묵향 파일을 선언한다.
//
//	if( !file.Init( strFileName, "rb") )												// 들어온 파일이름과, 열기 모드에 따라 파일을 열고, 결과를 받는다.
//	{																					// 실패했다면,
//		return FALSE;																	// FALSE를 리턴한다.
//	}
//
//	char Token[256];																	// 임시 버퍼를 선언한다.
//	
//	while( !file.IsEOF() )																// 파일의 끝이 아닌동안 while문을 돌린다.
//	{
//		file.GetString(Token);															// 스트링을 읽어 임시 버퍼에 담는다.
//
//		if(strcmp(Token,"$SUBQUESTSTR") == 0)											// 읽어들인 스트링이 $SUBQUESTSTR과 같다면,
//		{
//			DWORD Key_1 = file.GetDword();												// 키1을 읽어들인다.
//			DWORD Key_2 = file.GetDword();												// 키2를 읽어들인다.
//			DWORD Key;																	// 키 변수를 선언한다.
//
//			COMBINEKEY(Key_1, Key_2, Key);												// 키 변수에 키1, 키2를 결합하여 받는다.
//
//			CQuestString* pQuestString = new CQuestString ;
//
//			CQuestScriptLoader::LoadQuestString(&file, pQuestString);	// 퀘스트 스트링을 로딩하여 퀘스트 스트링 포인터로 받는다.
//
//			ASSERT( pQuestString && "Why is it null?" );								// 퀘스트 스트링 정보가 유효해야 한다. assert처리.
//			ASSERT( ! m_QuestStringTable.GetData( Key ) && "This memory'll be overwrite. It's reason of memory leak" );	// 퀘스트 스트링 테이블을 검사한다.
//			
//			m_QuestStringTable.Add(pQuestString, Key);									// 퀘스트 스트링 테이블에 퀘스트 스트링 정보를 담는다.
//			pQuestString->SetIndex(Key_1, Key_2);										// 퀘스트 스트링에 키1, 키2를 인덱스로 세팅한다.
//		}
//	}
//
//	//
//	m_QuestItemMGR.QuestItemInfoLoad();													// 퀘스트 아이템 매니져로 퀘스트 아이템 정보를 로딩한다.
//
//	return TRUE;																		// TRUE를 리턴한다.
//}

BOOL CQuestManager::LoadQuestString(char* strFileName)									// 퀘스트 스트링을 로딩하는 함수.
{
	CMHFile file;																		// 묵향 파일을 선언한다.

	if( !file.Init( strFileName, "rb") )												// 들어온 파일이름과, 열기 모드에 따라 파일을 열고, 결과를 받는다.
	{																					// 실패했다면,
		return FALSE;																	// FALSE를 리턴한다.
	}

	char Token[256];																	// 임시 버퍼를 선언한다.
	
	while( !file.IsEOF() )																// 파일의 끝이 아닌동안 while문을 돌린다.
	{
		file.GetString(Token);															// 스트링을 읽어 임시 버퍼에 담는다.

		if(strcmp(Token,"$SUBQUESTSTR") == 0)											// 읽어들인 스트링이 $SUBQUESTSTR과 같다면,
		{
			DWORD Key_1 = file.GetDword();												// 키1을 읽어들인다.
			DWORD Key_2 = file.GetDword();												// 키2를 읽어들인다.
			DWORD Key;																	// 키 변수를 선언한다.

			COMBINEKEY(Key_1, Key_2, Key);												// 키 변수에 키1, 키2를 결합하여 받는다.
			CQuestString* pQuestString = CQuestScriptLoader::LoadQuestString(&file);	// 퀘스트 스트링을 로딩하여 퀘스트 스트링 포인터로 받는다.

			ASSERT( pQuestString && "Why is it null?" );								// 퀘스트 스트링 정보가 유효해야 한다. assert처리.
			ASSERT( ! m_QuestStringTable.GetData( Key ) && "This memory'll be overwrite. It's reason of memory leak" );	// 퀘스트 스트링 테이블을 검사한다.
			
			m_QuestStringTable.Add(pQuestString, Key);									// 퀘스트 스트링 테이블에 퀘스트 스트링 정보를 담는다.
			pQuestString->SetIndex(Key_1, Key_2);										// 퀘스트 스트링에 키1, 키2를 인덱스로 세팅한다.
		}
	}

	//
	m_QuestItemMGR.QuestItemInfoLoad();													// 퀘스트 아이템 매니져로 퀘스트 아이템 정보를 로딩한다.

	return TRUE;																		// TRUE를 리턴한다.
}

//void CQuestManager::LoadNewbieGuide( char* strFileName )								// 초보자 가이드를 로딩하는 함수.
//{
//	m_NewbieGuide.LoadNewbieGuide( strFileName );										// 들어온 파일 이름으로, 초보자 가이드를 로딩한다.
//}

void CQuestManager::LoadQuestNpc( char* strFileName )									// 퀘스트 npc를 로딩하는 함수.
{
	CMHFile file;																		// 묵향 파일을 선언한다.

	if( !file.Init( strFileName, "rb") )												// 들어온 파일이름과, 열기 모드에 따라 파일을 열고, 결과를 받는다.
	{																					// 실패했다면,
		return ;																		// 리턴 처리를 한다.
	}
	
	while( !file.IsEOF() )																// 파일의 끝이 아닌동안 while문을 돌린다.
	{
		QUESTNPCINFO* pData = new QUESTNPCINFO;											// 퀘스트 npc정보를 담을 메모리를 할당하고, 포인터로 받는다.
		pData->dwObjIdx = 0;															// 오브젝트 인덱스를 0으로 세팅한다.
		pData->wMapNum = file.GetWord();												// 맵 번호를 받는다.
		pData->wNpcKind = file.GetWord();												// npc 종류를 받는다.
//		file.GetString( pData->sName );
		SafeStrCpy( pData->sName, file.GetString(), MAX_NAME_LENGTH+1 );				// npc 이름을 받는다.
		pData->wNpcIdx = file.GetWord();												// npc 인덱스를 받는다.
		pData->vPos.x = file.GetFloat();												// npc 위치를 받는다.
		pData->vPos.y = 0;
		pData->vPos.z = file.GetFloat();
		pData->fDir = file.GetFloat();													// npc 방향?을 받는다.

		m_QuestNpcTable.Add( pData, pData->wNpcIdx );									// npc 테이블에 npc정보를 담는다.
	}
}

void CQuestManager::CreateAllQuestFromScript()											// 스크립트로 부터 모든 퀘스트를 생성하는 함수.
{
	CQuestInfo* pQuestInfo = NULL;														// 퀘스트 정보를 담을 포인터를 선언한고 null처리를 한다.

	m_QuestInfoTable.SetPositionHead();													// 퀘스트 정보 테이블을 헤드로 세팅한다.

	while( pQuestInfo = m_QuestInfoTable.GetData() )									// 퀘스트 정보가 있는 동안 while문을 돌린다.
	{
		CQuest* pQuest = new CQuest( pQuestInfo );										// 퀘스트 정보에 따라 퀘스트 메모리를 할당하고, 퀘스트 포인터로 받는다.
		m_QuestTable.Add( pQuest, pQuestInfo->GetQuestIdx() );							// 퀘스트 테이블에 추가를 한다.
	}
}

void CQuestManager::InitializeQuest()													// 퀘스트를 초기화 하는 함수.
{
	CQuest* pQuest = NULL;																// 퀘스트 정보를 받을 포인터를 선언 및, NULL 처리를 한다.

	m_QuestTable.SetPositionHead();														// 퀘스트 테이블을 헤드로 세팅한다.

	while( pQuest = m_QuestTable.GetData() )											// 퀘스트 정보가 있는동안 while문을 돌린다.
	{
		pQuest->Initialize();															// 퀘스트를 초기화 한다.
	}
	m_dwMaxNpc = 0;																		// 최대 npc 변수를 0으로 세팅한다.

	m_dwLastMsg = 0;																	// 마지막 메시지를 0으로 세팅한다.
}

void CQuestManager::SetMainQuestData( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime, BYTE bCheckType, DWORD dwCheckTime )
{
	if( dwQuestIdx == 0 )																// 퀘스트 인덱스가 0과 같다면, // Newbieguide
	{
		//m_NewbieGuide.SetFlagData( dwSubQuestFlag, dwData, dwTime );					// 초보자 가이드의 플래그 데이터를 세팅한다.
		return;																			// 리턴 처리를 한다.
	}
	else																				// 퀘스트 인덱스가 0이 아니라면,
	{
		CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );							// 퀘스트 테이블에서 퀘스트 인덱스의 퀘스트 정보를 받는다.

		if( !pQuest ) return ;															// 퀘스트 정보가 유효하다면,

		pQuest->SetMainQuestData( dwSubQuestFlag, dwData, dwTime, bCheckType, dwCheckTime );	// 메인 퀘스트 데이터를 세팅한다.

		if( pQuest->IsQuestComplete() ) return ;
		m_QuestDialog->StartQuest(dwQuestIdx) ;
	}
}

//// 메인 퀘스트 데이터를 세팅하는 함수.
//void CQuestManager::SetMainQuestData( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime, BYTE bCheckType, DWORD dwCheckTime )
//{
//	if( dwQuestIdx == 0 )																// 퀘스트 인덱스가 0과 같다면, // Newbieguide
//	{
//		m_NewbieGuide.SetFlagData( dwSubQuestFlag, dwData, dwTime );					// 초보자 가이드의 플래그 데이터를 세팅한다.
//		return;																			// 리턴 처리를 한다.
//	}
//	else																				// 퀘스트 인덱스가 0이 아니라면,
//	{
//		CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );							// 퀘스트 테이블에서 퀘스트 인덱스의 퀘스트 정보를 받는다.
//
//		if( pQuest )																	// 퀘스트 정보가 유효하다면,
//		{
//			pQuest->SetMainQuestData( dwSubQuestFlag, dwData, dwTime, bCheckType, dwCheckTime );	// 메인 퀘스트 데이터를 세팅한다.
//
//			if( dwData == 0 )
//			{
//				CQuestInfo* pQuestInfo = GetQuestInfo(dwQuestIdx) ;						// 퀘스트 인덱스로 퀘스트 정보를 받는다.
//
//				int nMapNum = pQuestInfo->GetMapNum() ;									// 퀘스트 정보에 있는 맵 번호를 받는다.
//
//				AddMapTree(nMapNum) ;													// 퀘스트를 수락한 맵 번호에 따른 트리를 추가한다.
//
//				AddQuestTitle(nMapNum, dwQuestIdx) ;									// 퀘스트 제목을 퀘스트 리스트에 추가한다.
//
//				CQuestDialog* pDlg = NULL ;																		// 퀘스트 다이얼로그 정보를 받을 포인터를 선언하고 null처리를 한다.
//				pDlg = GAMEIN->GetQuestTotalDialog()->GetQuestDlg() ;											// 퀘스트 다이얼로그 정보를 받는다.
//
//				if( pDlg )																						// 퀘스트 다이얼로그 정보가 유효하다면,
//				{
//					pDlg->SetQuestCount(pDlg->GetQuestCount()+1) ;														// 퀘스트 다이얼로그의 퀘스트 개수를 세팅한다.
//
//					pDlg->QuestListReset() ;											// 퀘스트 리스트를 새로고침한다.
//				}
//			}
//			
//			//if(pQuest->GetEndParam() == 0)												// 퀘스트 종료 파라메터가 0과 같다면,
//			//{
//			//	DWORD Key = 0;															// 키 변수를 선언하고 0으로 세팅한다.
//			//	COMBINEKEY(dwQuestIdx, 0, Key);											// 퀘스트인덱스와 0을 결합하여 키값을 받는다.
//			//	m_QuestUpdater->ProcessQuestAdd(m_QuestStringTable.GetData(Key));		// 퀘스트 업데이터에 데이터를 추가한다.
//			//}
//		}
//	}
//}

//// 메인 퀘스트 데이터를 세팅하는 함수.
//void CQuestManager::SetMainQuestData( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime, BYTE bCheckType, DWORD dwCheckTime )
//{
//	if( dwQuestIdx == 0 )																// 퀘스트 인덱스가 0과 같다면, // Newbieguide
//	{
//		m_NewbieGuide.SetFlagData( dwSubQuestFlag, dwData, dwTime );					// 초보자 가이드의 플래그 데이터를 세팅한다.
//		return;																			// 리턴 처리를 한다.
//	}
//	else																				// 퀘스트 인덱스가 0이 아니라면,
//	{
//		CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );							// 퀘스트 테이블에서 퀘스트 인덱스의 퀘스트 정보를 받는다.
//
//		if( pQuest )																	// 퀘스트 정보가 유효하다면,
//		{
//			pQuest->SetMainQuestData( dwSubQuestFlag, dwData, dwTime, bCheckType, dwCheckTime );	// 메인 퀘스트 데이터를 세팅한다.
//
//			if( dwData == 0 )
//			{
//				CQuestInfo* pQuestInfo = GetQuestInfo(dwQuestIdx) ;						// 퀘스트 인덱스로 퀘스트 정보를 받는다.
//
//				int nMapNum = pQuestInfo->GetMapNum() ;									// 퀘스트 정보에 있는 맵 번호를 받는다.
//
//				AddMapTree(nMapNum) ;													// 퀘스트를 수락한 맵 번호에 따른 트리를 추가한다.
//
//				AddQuestTitle(nMapNum, dwQuestIdx) ;									// 퀘스트 제목을 퀘스트 리스트에 추가한다.
//
//				CQuestDialog* pDlg = NULL ;																		// 퀘스트 다이얼로그 정보를 받을 포인터를 선언하고 null처리를 한다.
//				pDlg = GAMEIN->GetQuestTotalDialog()->GetQuestDlg() ;											// 퀘스트 다이얼로그 정보를 받는다.
//
//				if( pDlg )																						// 퀘스트 다이얼로그 정보가 유효하다면,
//				{
//					pDlg->SetQuestCount(pDlg->GetQuestCount()+1) ;														// 퀘스트 다이얼로그의 퀘스트 개수를 세팅한다.
//
//					pDlg->QuestListReset() ;											// 퀘스트 리스트를 새로고침한다.
//				}
//			}
//			
//			//if(pQuest->GetEndParam() == 0)												// 퀘스트 종료 파라메터가 0과 같다면,
//			//{
//			//	DWORD Key = 0;															// 키 변수를 선언하고 0으로 세팅한다.
//			//	COMBINEKEY(dwQuestIdx, 0, Key);											// 퀘스트인덱스와 0을 결합하여 키값을 받는다.
//			//	m_QuestUpdater->ProcessQuestAdd(m_QuestStringTable.GetData(Key));		// 퀘스트 업데이터에 데이터를 추가한다.
//			//}
//		}
//	}
//}

// 서브 퀘스트 데이터를 세팅하는 함수.
void CQuestManager::SetSubQuestData( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwData, DWORD dwTime )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// 퀘스트 테이블에서 들어온 퀘스트 인덱스로 퀘스트 정보를 받는다.

	if( !pQuest ) return ;																// 퀘스트 정보가 유효한지 체크한다.

	pQuest->SetSubQuestData( dwSubQuestIdx, dwData, dwTime );						// 퀘스트의 서브 퀘스트를 세팅한다.

	AddQuestNpc( pQuest->GetSubQuestInfo( dwSubQuestIdx ) );						// 퀘스트 npc를 추가한다.

	m_QuestDialog->RefreshQuestList() ;
}

//// 서브 퀘스트 데이터를 세팅하는 함수.
//void CQuestManager::SetSubQuestData( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwData, DWORD dwTime )
//{
//	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// 퀘스트 테이블에서 들어온 퀘스트 인덱스로 퀘스트 정보를 받는다.
//
//	if( pQuest )																		// 퀘스트 정보가 유효한지 체크한다.
//	{
//		pQuest->SetSubQuestData( dwSubQuestIdx, dwData, dwTime );						// 퀘스트의 서브 퀘스트를 세팅한다.
//
//		AddQuestNpc( pQuest->GetSubQuestInfo( dwSubQuestIdx ) );						// 퀘스트 npc를 추가한다.
//
//		//
//		int Key = 0;																	// 키 변수를 선언하고 0으로세팅한다.
//		COMBINEKEY( dwQuestIdx, dwSubQuestIdx, Key );									// 퀘스트 인덱스와 서브퀘스트 인덱스를 결합하여, 키값을 받는다.
//		m_QuestUpdater->ProcessQuestAdd( m_QuestStringTable.GetData(Key) );				// 퀘스트 업데이터에 데이터를 추가한다.
//	}
//}
void CQuestManager::AddMapTree(int nMapNum)												// 맵 트리를 추가하는 함수.
{	
	cPtrList* pTreeList = NULL ;														// 퀘스트 트리 리스트를 받을 포인터를 선언하고 NULL 처리를 한다.

	CQuestTotalDialog * pTotalDialog = GAMEIN->GetQuestTotalDialog() ;					// 퀘스트 토탈 다이얼로그 정보를 받는다.

	if( pTotalDialog )																	// 토탈 다이얼로그 정보가 유효하면,
	{
		CQuestDialog* pQuestDlg = pTotalDialog->GetQuestDlg() ;							// 퀘스트 다이얼로그 정보를 받는다.

		if( pQuestDlg )																	// 퀘스트 다이얼로그 정보가 유효하면,
		{
			pTreeList = pQuestDlg->GetTreeList() ;										// 트리 리스트를 받는다.			

			if( pTreeList )																// 트리 리스트 정보가 유효하다면,
			{
				BOOL bResult = FALSE ;													// 검색 결과를 담을 변수를 선언하고 FALSE 처리를 한다.

				int nTreeCount = pTreeList->GetCount() ;								// 트리 카운트를 받는다.

				QuestTree* pTree = NULL ;												// 트리 정보를 받을 포인터를 선언하고 NULL 처리를 한다.

				PTRLISTPOS pTreePos = NULL ;											// 트리의 위치 정보를 받을 포인터를 선언하고 NULL처리를 한다.

				for( int count = 0 ; count < nTreeCount ; ++count )						// 트리 카운트 만큼 FOR문을 돌린다.
				{
					pTreePos = pTreeList->FindIndex(count) ;							// 카운트에 해당하는 위치 정보를 받는다.

					if( pTreePos )														// 트리 위치 정보가 유효하다면,
					{
						pTree = (QuestTree*)pTreeList->GetAt(pTreePos) ;				// 위치에 해당하는 트리 정보를 받는다.

						if( pTree )														// 트리 정보가 유효하다면,
						{
							if( pTree->nMapNum == nMapNum )								// 현재 퀘스트의 맵 번호와 트리의 맵 번호를 비교해서 같으면,
							{
								bResult = TRUE ;										// 결과를 TRUE로 세팅한다.

								break ;													// for문을 빠져나간다.
							}
						}
					}
				}

				if( bResult == FALSE )													// 결과가 FALSE와 같으면,
				{
					//QuestTree* pData = new QuestTree;									// 트리를 생성한다.

					//CQuestString* pQuestString = new CQuestString;						// 트리에 담을 퀘스트 스트링을 생성한다.

					//int nTitleLine = 0 ;												// 타이틀 라인을 담을 변수를 선언하고 0으로 세팅한다.
					//pQuestString->AddLine(GetMapName(nMapNum), nTitleLine, TRUE) ;		// 퀘스트 스트링에 맵 이름을 타이틀로 추가한다.
					//pQuestString->SetIndex(nMapNum, 0);									// 트리 인덱스를 맵 번호의 0으로 세팅한다.
					//pQuestString->SetTreeTitle(TRUE) ;									// 트리의 제목으로 세팅한다.
					//pData->list.AddHead(pQuestString);									// 새로 생성한 트리에 맵 타이틀을 추가한다.
					//++pData->Count;														// 트리의 스트링 카운트를 증가한다.
					//pData->State = eQTree_Open ;										// 트리의 상태를 닫힌 상태로 세팅한다.
					//pData->nMapNum = nMapNum ;											// 트리의 맵 번호를 세팅한다.

					////pTreeList->AddTail(pData);											// 트리를 퀘스트 트리 리스트에 추가한다.
					//pQuestDlg->AddQuestTree(pData) ;
					//pQuestDlg->SetTreeCount( pQuestDlg->GetTreeCount() + 1 ) ;			// 트리 카운트를 증가한다.

					pQuestDlg->AddQuestTree(nMapNum) ;
				}
			}
		}
	}
}

void CQuestManager::DeleteMapTree(int nMapNum)
{
	cPtrList* pTreeList = NULL ;														// 퀘스트 트리 리스트를 받을 포인터를 선언하고 NULL 처리를 한다.

	CQuestTotalDialog * pTotalDialog = GAMEIN->GetQuestTotalDialog() ;					// 퀘스트 토탈 다이얼로그 정보를 받는다.

	if( pTotalDialog )																	// 토탈 다이얼로그 정보가 유효하면,
	{
		CQuestDialog* pQuestDlg = pTotalDialog->GetQuestDlg() ;							// 퀘스트 다이얼로그 정보를 받는다.

		if( pQuestDlg )																	// 퀘스트 다이얼로그 정보가 유효하면,
		{
			pTreeList = pQuestDlg->GetTreeList() ;										// 트리 리스트를 받는다.			

			if( pTreeList )																// 트리 리스트 정보가 유효하다면,
			{
				int nTreeCount = pTreeList->GetCount() ;								// 트리 카운트를 받는다.

				QuestTree* pTree = NULL ;												// 트리 정보를 받을 포인터를 선언하고 NULL 처리를 한다.

				PTRLISTPOS pTreePos = NULL ;											// 트리의 위치 정보를 받을 포인터를 선언하고 NULL처리를 한다.

				for( int count = 0 ; count < nTreeCount ; ++count )						// 트리 카운트 만큼 FOR문을 돌린다.
				{
					pTreePos = pTreeList->FindIndex(count) ;							// 카운트에 해당하는 위치 정보를 받는다.

					if( pTreePos )														// 트리 위치 정보가 유효하다면,
					{
						pTree = (QuestTree*)pTreeList->GetAt(pTreePos) ;				// 위치에 해당하는 트리 정보를 받는다.

						if( pTree )														// 트리 정보가 유효하다면,
						{
							if( pTree->nMapNum == nMapNum )								// 현재 퀘스트의 맵 번호와 트리의 맵 번호를 비교해서 같으면,
							{
								if( pTree->list.GetCount() == 0 )						// 트리의 카운트가 0개이면,
								{
									pTreeList->RemoveAt(pTreePos) ;						// 현재 트리를 삭제한다.
								}
							}
						}
					}
				}
			}
		}
	}
}

void CQuestManager::AddQuestTitle(int nMapNum, DWORD dwQuestIdx)						// 퀘스트 다이얼로그에 퀘스트 제목을 추가하는 함수.
{
	if( dwQuestIdx < 1 )																// 퀘스트 인덱스가 1보다 작으면,
	{
		return ;																		// 리턴 처리를 한다.
	}

	int nKey = 0 ;																		// 스트링 키 값을 담을 변수를 선언하고 0으로 세팅한다.

	COMBINEKEY(dwQuestIdx, 0, nKey) ;													// 퀘스트 인덱스와 0으로 키 값을 세팅한다.
	
	CQuestString* pQuestString = NULL ;													// 퀘스트 스트링 정보를 받을 포인터를 선언하고 null 처리를 한다.
	pQuestString = m_QuestStringTable.GetData(nKey) ;									// 컴바인한 키 값으로 퀘스트 스트링을 받는다.

	if( !pQuestString )																	// 퀘스트 스트링 정보가 유효하지 않다면,
	{
		return ;																		// 리턴처리를 한다.
	}

	CQuestDialog* pQuestDlg = NULL ;													// 퀘스트 다이얼로그 정보를 받을 포인터를 선언하고 null처리를 한다.
	pQuestDlg = GAMEIN->GetQuestDialog() ;											// 퀘스트 다이얼로그 정보를 받는다.

	if( pQuestDlg )
	{
		pQuestDlg->AddQuestTitle(pQuestString, nMapNum) ;
	}

	//if( !pQuestDlg )
	//{	
	//	return ;																		// 리턴 처리를 한다.
	//}

	//cPtrList* pTreeList = NULL ;														// 퀘스트 트리 리스트를 받을 포인터를 선언하고 NULL 처리를 한다.
	//pTreeList = pQuestDlg->GetTreeList() ;												// 트리 리스트를 받는다.			

	//if( !pTreeList )
	//{	
	//	return ;																		// 리턴 처리를 한다.
	//}

	//BOOL bResult = FALSE ;																// 검색 결과를 담을 변수를 선언하고 FALSE 처리를 한다.

	//int nTreeCount = pTreeList->GetCount() ;											// 트리 카운트를 받는다.

	//QuestTree* pTree = NULL ;															// 트리 정보를 받을 포인터를 선언하고 NULL 처리를 한다.
	//PTRLISTPOS pTreePos = NULL ;														// 트리의 위치 정보를 받을 포인터를 선언하고 NULL처리를 한다.

	//for( int count = 0 ; count < nTreeCount ; ++count )									// 트리 카운트 만큼 FOR문을 돌린다.
	//{
	//	pTreePos = pTreeList->FindIndex(count) ;										// 카운트에 해당하는 위치 정보를 받는다.

	//	if( pTreePos )																	// 트리 위치 정보가 유효하다면,
	//	{
	//		pTree = (QuestTree*)pTreeList->GetAt(pTreePos) ;							// 위치에 해당하는 트리 정보를 받는다.

	//		if( pTree )																	// 트리 정보가 유효하다면,
	//		{
	//			if( pTree->nMapNum == nMapNum )											// 현재 퀘스트의 맵 번호와 트리의 맵 번호를 비교해서 같으면,
	//			{
	//				pQuestString->SetTreeTitle(FALSE) ;									// 트리의 타이틀이 아닌 것으로 세팅한다.
	//				pTree->list.AddTail(pQuestString) ;									// 현재 맵의 트리에 퀘스트 제목을 추가한다.		

	//				break ;																// for문을 빠져나간다.
	//			}
	//		}
	//	}
	//}

	//if( bResult == FALSE )																// 결과가 FALSE와 같으면,
	//{
	//	return ;																		// 리턴 처리를 한다.
	//}
}

void CQuestManager::DeleteQuestTitle(int nMapNum, DWORD dwQuestIdx)						// 퀘스트 리스트에서 퀘스트 제목을 삭제하는 함수.
{
	if( dwQuestIdx < 1 )																// 퀘스트 인덱스가 1보다 작으면,
	{
		ASSERTMSG(0, "CQuestManager::AddQuestTitle\n퀘스트 제목을 삭제하던 중, 퀘스트 인덱스가 1보다 작은 값이 넘어왔다.!!!") ;	// ASSERT 에러 처리.

		return ;																		// 리턴 처리를 한다.
	}

	int nKey = 0 ;																		// 스트링 키 값을 담을 변수를 선언하고 0으로 세팅한다.

	COMBINEKEY(dwQuestIdx, 0, nKey) ;													// 퀘스트 인덱스와 0으로 키 값을 세팅한다.
	
	CQuestString* pQuestString = NULL ;													// 퀘스트 스트링 정보를 받을 포인터를 선언하고 null 처리를 한다.
	pQuestString = m_QuestStringTable.GetData(nKey) ;									// 컴바인한 키 값으로 퀘스트 스트링을 받는다.

	if( !pQuestString )																	// 퀘스트 스트링 정보가 유효하지 않다면,
	{
		ASSERTMSG(0, "CQuestManager::AddQuestTitle\n퀘스트 제목을 삭제하던 중 퀘스트 스트링 정보가 유효하지 않다.") ;	// ASSERT 에러 처리.

		return ;																		// 리턴처리를 한다.
	}

	CQuestTotalDialog * pTotalDialog = NULL ;											// 퀘스트 토탈 정보를 받을 포인터를 선언하고 null 처리를 한다.
	pTotalDialog = GAMEIN->GetQuestTotalDialog() ;										// 퀘스트 토탈 다이얼로그 정보를 받는다.

	if( !pTotalDialog )
	{
		ASSERTMSG(0, "CQuestManager::AddQuestTitle\n퀘스트 제목을 삭제하던 중 퀘스트 토탈 정보가 유효하지 않다.") ;	// ASSERT 에러 처리.
		
		return ;																		// 리턴 처리를 한다.
	}

	CQuestDialog* pQuestDlg = NULL ;													// 퀘스트 다이얼로그 정보를 받을 포인터를 선언하고 null처리를 한다.
	pQuestDlg = pTotalDialog->GetQuestDlg() ;											// 퀘스트 다이얼로그 정보를 받는다.

	if( !pQuestDlg )
	{
		ASSERTMSG(0, "CQuestManager::AddQuestTitle\n퀘스트 제목을 삭제하던 중 퀘스트 다이얼로그 정보를 받지 못했다.") ;	// ASSERT 에러 처리.
		
		return ;																		// 리턴 처리를 한다.
	}

	cPtrList* pTreeList = NULL ;														// 퀘스트 트리 리스트를 받을 포인터를 선언하고 NULL 처리를 한다.
	pTreeList = pQuestDlg->GetTreeList() ;												// 트리 리스트를 받는다.			

	if( !pTreeList )
	{
		ASSERTMSG(0, "CQuestManager::AddQuestTitle\n퀘스트 제목을 삭제하던 중 맵 트리 리스트를 받지 못했다.") ;	// ASSERT 에러 처리.
		
		return ;																		// 리턴 처리를 한다.
	}

	BOOL bResult = FALSE ;																// 검색 결과를 담을 변수를 선언하고 FALSE 처리를 한다.

	int nTreeCount = pTreeList->GetCount() ;											// 트리 카운트를 받는다.

	QuestTree* pTree = NULL ;															// 트리 정보를 받을 포인터를 선언하고 NULL 처리를 한다.
	PTRLISTPOS pTreePos = NULL ;														// 트리의 위치 정보를 받을 포인터를 선언하고 NULL처리를 한다.
	PTRLISTPOS pDeletePos = NULL ;														// 삭제할 퀘스트 스트링 위치를 나타내는 포인터를 선언하고 NULL 처리를 한다.

	for( int count = 0 ; count < nTreeCount ; ++count )									// 트리 카운트 만큼 FOR문을 돌린다.
	{
		pTreePos = pTreeList->FindIndex(count) ;										// 카운트에 해당하는 위치 정보를 받는다.

		if( pTreePos )																	// 트리 위치 정보가 유효하다면,
		{
			pTree = (QuestTree*)pTreeList->GetAt(pTreePos) ;							// 위치에 해당하는 트리 정보를 받는다.

			if( pTree )																	// 트리 정보가 유효하다면,
			{
				if( pTree->nMapNum == nMapNum )											// 현재 퀘스트의 맵 번호와 트리의 맵 번호를 비교해서 같으면,
				{
					int nQuestCount = pTree->list.GetCount() ;							// 퀘스트 제목 카운트를 받는다.

					CQuestString* pDelete = NULL ;										// 삭제 할 퀘스트 스트링을 받을 포인터를 선언하고 null 처리를 한다.

					for( int count = 0 ; count < nQuestCount ; ++count )				// 퀘스트 제목 수 만큼 for문을 돌린다.
					{
						pDeletePos = pTree->list.FindIndex(count) ;						// 카운트에 해당하는 삭제 위치를 받는다.

						if( pDeletePos )												// 삭제 위치 정보가 유효하면,
						{
							pDelete = (CQuestString*)pTree->list.GetAt(pDeletePos) ;	// 삭제 할 스트링 정보를 받는다.

							if( pDelete )												// 스트링 정보가 유효하면,
							{
								if( pDelete->GetKey() == pQuestString->GetKey() )		// 트리의 스트링과, 삭제할 스트링의 키 값이 같으면,
								{
									bResult = TRUE ;									// 결과 값을 TRUE로 세팅한다.

									//pTree->list.RemoveAt(pDeletePos) ;					// 트리에서 스트링을 삭제한다.
									CQuestString* pDeleteString = (CQuestString*)pTree->list.GetAt(pDeletePos) ;

									if( pDeleteString )
									{
										pTree->list.Remove(pDeleteString) ;
										delete pDeleteString ;
									}
								}
							}
						}
					}

					break ;																// for문을 빠져나간다.
				}
			}
		}
	}

	if( bResult == FALSE )														// 결과가 FALSE와 같으면,
	{
		ASSERT(0 && "CQuestManager::AddQuestTitle\n같은 맵에 퀘스트를 삭제하지 못함!!") ;

		return ;
	}
}


void CQuestManager::StartQuest( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// 퀘스트 테이블에서 들어온 퀘스트 인덱스로 퀘스트 정보를 받는다.

	pQuest->StartQuest( dwSubQuestFlag, dwData, dwTime );								// 퀘스트를 시작한다.

	m_QuestDialog->StartQuest(dwQuestIdx) ;

	int Key = 0;																	// 키값을 받을 변수를 선언한고, 0으로 세팅한다.
	COMBINEKEY( dwQuestIdx, 0, Key );												// 퀘스트 인덱스와 0을 결합하여 키 값을 받는다.

	// title
	CQuestString* pQString = m_QuestStringTable.GetData(Key);						// 키 값으로 퀘스트 스트링 정보를 받는다.

	if( pQString == NULL )															// 퀘스트 스트링 정보가 유효하지 않다면,
	{
		return;																		// 리턴 처리를 한다.
	}
	cPtrList* p = pQString->GetTitle();												// 리스트 포인터를 선언하여, 제목을 받는다.
	ITEM* string = (ITEM*)p->GetHead();										// 리스트의 헤드를 퀘스트 스트링 포인터에 받는다.

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(221), string->string );			// 퀘스트를 시작한다는 메시지를 출력한다.
}

void CQuestManager::EndQuest( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// 퀘스트 테이블에서 들어온 퀘스트 인덱스로 퀘스트 정보를 받는다.

	if( !pQuest ) return ;

	if( pQuest->IsTimeCheck() )															// 시간 체크가 필요한 퀘스트라면,
	{
		STATUSICONDLG->RemoveQuestTimeIcon( HERO, eStatusIcon_QuestTime );				// 퀘스트 시간 아이콘을 삭제한다.
	}

	pQuest->EndQuest( dwSubQuestFlag, dwData, dwTime );									// 퀘스트를 종료한다.

	m_QuestDialog->EndQuest(dwQuestIdx) ;

	m_QuickView->DeleteQuest(dwQuestIdx) ;

	DWORD Key = 0;																	// 키 값을 담을 변수를 선언한다.
	COMBINEKEY(dwQuestIdx, 0, Key);													// 퀘스트 인덱스와 0을 결합하여 키값을 받는다.

	// title
	CQuestString* pQString = m_QuestStringTable.GetData(Key);						// 키 값으로 퀘스트 스트링 테이블에서 스트링 정보를 받는다.

	if( pQString == NULL )															// 퀘스트 스트링 정보가 유효한지 체크한다.
	{
		return;																		// 정보가 유효하지 않으면 리턴 처리를 한다.
	}

	cPtrList* p = pQString->GetTitle();												// 리스트 포인터로 제목을 받는다.

	ITEM* string = (ITEM*)p->GetHead();										// 리스트의 헤드를 받는다.

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(222), string->string );			// 퀘스트 메시지를 채팅창에 출력한다.
}

// 퀘스트를 시작하는 함수.
//void CQuestManager::StartQuest( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime )
//{
//	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// 퀘스트 테이블에서 들어온 퀘스트 인덱스로 퀘스트 정보를 받는다.
//
//	CQuestInfo* pQuestInfo =NULL ;														// 퀘스트 정보를 받을 포인터를 선언하고 null 처리를 한다.
//	pQuestInfo = GetQuestInfo(dwQuestIdx) ;												// 퀘스트 인덱스로 퀘스트 정보를 받는다.
//
//	if(!pQuestInfo)																		// 퀘스트 정보가 유효하지 않으면,
//	{
//		return  ;																		// 리턴 처리를 한다.
//	}
//
//	int nMapNum = pQuestInfo->GetMapNum() ;												// 퀘스트 정보에 있는 맵 번호를 받는다.
//
//	if( pQuest )																		// 퀘스트 정보가 유효한지 체크한다.
//	{
//		pQuest->StartQuest( dwSubQuestFlag, dwData, dwTime );							// 퀘스트를 시작한다.
//
//		AddMapTree(nMapNum) ;															// 퀘스트를 수락한 맵 번호에 따른 트리를 추가한다.
//
//		AddQuestTitle(nMapNum, dwQuestIdx) ;											// 퀘스트 제목을 퀘스트 리스트에 추가한다.
//
//		int Key = 0;																	// 키값을 받을 변수를 선언한고, 0으로 세팅한다.
//		COMBINEKEY( dwQuestIdx, 0, Key );												// 퀘스트 인덱스와 0을 결합하여 키 값을 받는다.
//		m_QuestUpdater->ProcessQuestAdd( m_QuestStringTable.GetData(Key) );				// 퀘스트 업데이터에 퀘스트를 추가한다.
//
//		// title
//		CQuestString* pQString = m_QuestStringTable.GetData(Key);						// 키 값으로 퀘스트 스트링 정보를 받는다.
//
//		if( pQString == NULL )															// 퀘스트 스트링 정보가 유효하지 않다면,
//		{
//			return;																		// 리턴 처리를 한다.
//		}
//
//		CQuestDialog* pDlg = GAMEIN->GetQuestTotalDialog()->GetQuestDlg() ;
//
//		if( pDlg )
//		{
//			pDlg->QuestListReset() ;
//			pDlg->QuestDescView(pQString) ;
//			pDlg->GetQuestDescList()->ResetGuageBarPos() ;
//
//			pDlg->SetQuestCount( pDlg->GetQuestCount()+1 ) ;
//		}
//
//		cPtrList* p = pQString->GetTitle();												// 리스트 포인터를 선언하여, 제목을 받는다.
//		ITEM* string = (ITEM*)p->GetHead();										// 리스트의 헤드를 퀘스트 스트링 포인터에 받는다.
//
//		CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(221), string->string );			// 퀘스트를 시작한다는 메시지를 출력한다.
//
//		//GAMENOTIFYMGR->StartQuest(string->Str) ;										// 퀘스트 시작 공지를 띄운다.
//	}	
//}
//void CQuestManager::EndQuest( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime )
//{
//	CQuestInfo* pQuestInfo =NULL ;														// 퀘스트 정보를 받을 포인터를 선언하고 null 처리를 한다.
//	pQuestInfo = GetQuestInfo(dwQuestIdx) ;												// 퀘스트 인덱스로 퀘스트 정보를 받는다.
//
//	if(!pQuestInfo)																		// 퀘스트 정보가 유효하지 않으면,
//	{
//		ASSERTMSG(0, "퀘스트 끝내는 시점에서 퀘스트 정보를 로딩하지 못하였습니다.!") ;	// ASSERT 에러 체크.
//		return  ;																		// 리턴 처리를 한다.
//	}
//
//	int nMapNum = pQuestInfo->GetMapNum() ;												// 퀘스트 정보에 있는 맵 번호를 받는다.
//
//	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// 퀘스트 테이블에서 들어온 퀘스트 인덱스로 퀘스트 정보를 받는다.
//
//	if( pQuest )
//	{
//		if( pQuest->IsTimeCheck() )														// 시간 체크가 필요한 퀘스트라면,
//		{
//			STATUSICONDLG->RemoveQuestTimeIcon( HERO, eStatusIcon_QuestTime );			// 퀘스트 시간 아이콘을 삭제한다.
//		}
//
//		pQuest->EndQuest( dwSubQuestFlag, dwData, dwTime );								// 퀘스트를 종료한다.
//
//		CQuestQuickViewDialog* pQuickView = NULL ;														// 퀘스트 알림이 다이얼로그 정보를 받을 포인터를 선언하고 null처리 한다.
//		pQuickView = GAMEIN->GetQuestQuickViewDialog() ;												// 퀘스트 알림이 다이얼로그 정보를 받는다.
//
//		if( pQuickView )																				// 퀘스트 알림이 다이얼로그 정보가 유효하다면,
//		{
//			pQuickView->DeleteQuest(dwQuestIdx) ;
//			//pQuickView->ResetQuickViewDialog() ;																// 퀘스트 알림이의 메인 퀘스트 데이터를 로딩한다.
//		}
//
//		DeleteQuestTitle(nMapNum, dwQuestIdx) ;											// 퀘스트 리스트에서 퀘스트를 삭제한다.
//
//		DeleteMapTree(nMapNum) ;
//
//		CQuestDialog* pDlg = GAMEIN->GetQuestTotalDialog()->GetQuestDlg() ;				// 퀘스트 다이얼로그를 받는다.
//
//		if( pDlg )																		// 퀘스트 다이얼의 정보가 유효하면,
//		{
//			pDlg->QuestListReset() ;													// 퀘스트리스트를 새로고침한다.
//
//			int nQuestCount = 0 ;																		// 퀘스트 카운트를 담을 변수를 선언하고 0으로 세팅한다.
//			nQuestCount = pDlg->GetQuestCount() ;														// 퀘스트 카운트를 받는다.
//
//			if( nQuestCount == 0 )																		// 퀘스트 카운트가 0과 같으면,
//			{
//				pDlg->SetQuestCount( 0 ) ;																// 퀘스트 카운트를 0으로 세팅한다.
//			}
//			else																						// 퀘스트 카운트가 0과 같지 않으면,
//			{
//				pDlg->SetQuestCount( nQuestCount-1 ) ;													// 퀘스트 카운트를 세팅한다.
//			}
//		}
//	}
//	else
//	{
//		ASSERTMSG(0, "퀘스트 끝내는 시점에서 퀘스트를 로딩하지 못하였습니다.!") ;		// ASSERT 에러 체크.
//		return ;
//	}
//
//
//	DWORD Key = 0;																	// 키 값을 담을 변수를 선언한다.
//	COMBINEKEY(dwQuestIdx, 0, Key);													// 퀘스트 인덱스와 0을 결합하여 키값을 받는다.
//
//	// title
//	CQuestString* pQString = m_QuestStringTable.GetData(Key);						// 키 값으로 퀘스트 스트링 테이블에서 스트링 정보를 받는다.
//
//	if( pQString == NULL )															// 퀘스트 스트링 정보가 유효한지 체크한다.
//	{
//		return;																		// 정보가 유효하지 않으면 리턴 처리를 한다.
//	}
//
//	cPtrList* p = pQString->GetTitle();												// 리스트 포인터로 제목을 받는다.
//
//	ITEM* string = (ITEM*)p->GetHead();										// 리스트의 헤드를 받는다.
//
//	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(222), string->string );			// 퀘스트 메시지를 채팅창에 출력한다.
//
//	//GAMENOTIFYMGR->EndQuest(string->Str) ;											// 퀘스트 종료 공지를 출력한다.
//}

// 퀘스트를 종료하는 함수.
//void CQuestManager::EndQuest( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime )
//{
//	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// 들어온 퀘스트 인덱스로 퀘스트 테이블에서 퀘스트 정보를 받는다.
//
//	if( pQuest )																		// 퀘스트 정보가 유효한지 체크한다.
//	{
//		// remove icon
//		if( pQuest->IsTimeCheck() )														// 시간 체크가 필요한 퀘스트라면,
//		{
//			STATUSICONDLG->RemoveQuestTimeIcon( HERO, eStatusIcon_QuestTime );			// 퀘스트 시간 아이콘을 삭제한다.
//		}
//
//		pQuest->EndQuest( dwSubQuestFlag, dwData, dwTime );								// 퀘스트를 종료한다.
//
//		//
//		DWORD Key = 0;																	// 키 값을 담을 변수를 선언한다.
//		COMBINEKEY(dwQuestIdx, 0, Key);													// 퀘스트 인덱스와 0을 결합하여 키값을 받는다.
//
//		if(dwSubQuestFlag == 0)															// 서브 퀘스트 플래그가 0과 같으면,
//		{
//			m_QuestUpdater->CompleteQuestDelete(m_QuestStringTable.GetData(Key));		// 퀘스트 업데이터로 완료 퀘스트를 삭제한다.
//		}
//
//		CQuestTotalDialog* pDlg = GAMEIN->GetQuestTotalDialog() ;
//		if(pDlg)
//		{
//			CQuestDialog* pQuestDlg = pDlg->GetQuestDlg() ;
//
//			if( pQuestDlg )
//			{
//				pQuestDlg->CompleteQuestDelete(QUESTMGR->GetQuestString(Key));
//			}
//		}
//
//		//stTIME stime;																	// 시간 구조체를 선언한다.
//		//stime.value = dwTime;															// 시간을 기록한다.
//		//
//		//char buf[16] = { 0, };															// 임시 버퍼를 선언한다.
//		//sprintf(buf, "20%02d.%02d.%02d", stime.GetYear(), stime.GetMonth(), stime.GetDay());	// 시간을 버퍼에 복사한다.
//		//
//		//JOURNALINFO pItem;																// 저널 정보 구조체를 선언한다.
//		//pItem.Kind = eJournal_Quest;													// 종류를 eJournal_Quest로 세팅한다.
//		//pItem.Param = dwQuestIdx;														// 퀘스트 인덱스를 세팅한다.
//		//pItem.Param_2 = 0;																// SubQuestIdx가 0만 기록(메인퀘스트만 기록)
//		//pItem.Param_3 = 1;																// bCompleted
//		//strncpy(pItem.RegDate, buf, 11);												// 시간을 복사한다.
//		//m_QuestUpdater->JournalItemAdd(&pItem);											// 퀘스트 업데이터에 저널 아이템을 추가한다.
//		
//		
//		// title
//		CQuestString* pQString = m_QuestStringTable.GetData(Key);						// 키 값으로 퀘스트 스트링 테이블에서 스트링 정보를 받는다.
//
//		if( pQString == NULL )															// 퀘스트 스트링 정보가 유효한지 체크한다.
//		{
//			return;																		// 정보가 유효하지 않으면 리턴 처리를 한다.
//		}
//
//		cPtrList* p = pQString->GetTitle();												// 리스트 포인터로 제목을 받는다.
//
//		QString* string = (QString*)p->GetHead();										// 리스트의 헤드를 받는다.
//
//		CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(222), string->Str );			// 퀘스트 메시지를 채팅창에 출력한다.
//	}
//}

// 서브 퀘스트를 시작한다.

void CQuestManager::StartSubQuest( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwSubQuestFlag, DWORD dwSubData, DWORD dwSubTime )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// 퀘스트 테이블에서 인덱스에 해당하는 퀘스트 정보를 받는다.

	if( !pQuest ) return ;																// 퀘스트 정보가 유효한지 체크한다.

	pQuest->StartSubQuest( dwSubQuestIdx, dwSubQuestFlag, dwSubData, dwSubTime );	// 서브 퀘스트를 시작한다.
	AddQuestNpc( pQuest->GetSubQuestInfo( dwSubQuestIdx ) );						// 퀘스트 NPC를 추가한다.

	m_QuestDialog->RefreshQuestList() ;
}

//void CQuestManager::StartSubQuest( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwSubQuestFlag, DWORD dwSubData, DWORD dwSubTime )
//{
//	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// 퀘스트 테이블에서 인덱스에 해당하는 퀘스트 정보를 받는다.
//
//	if( pQuest )																		// 퀘스트 정보가 유효한지 체크한다.
//	{
//		pQuest->StartSubQuest( dwSubQuestIdx, dwSubQuestFlag, dwSubData, dwSubTime );	// 서브 퀘스트를 시작한다.
//		AddQuestNpc( pQuest->GetSubQuestInfo( dwSubQuestIdx ) );						// 퀘스트 NPC를 추가한다.
//
//		//CQuestQuickViewDialog* pQuickView = NULL ;														// 퀘스트 알림이 다이얼로그 정보를 받을 포인터를 선언하고 null처리 한다.
//		//pQuickView = GAMEIN->GetQuestQuickViewDialog() ;												// 퀘스트 알림이 다이얼로그 정보를 받는다.
//
//		//if( pQuickView )																				// 퀘스트 알림이 다이얼로그 정보가 유효하다면,
//		//{
//		//	pQuickView->ResetQuickViewDialog() ;																// 퀘스트 알림이의 메인 퀘스트 데이터를 로딩한다.
//		//}
//
//		////
//		//int Key = 0;																	// 키 값을 받을 변수를 선언한다.
//		//COMBINEKEY( dwQuestIdx, dwSubQuestIdx, Key );									// 퀘스트 인덱스와 서브 퀘스트 인덱스를 결합하여 키값을 받는다.
//		//m_QuestUpdater->ProcessQuestAdd( m_QuestStringTable.GetData(Key) );				// 퀘스트 업데이터에 퀘스트를 추가한다.
//
//		//if( gCurTime - m_dwLastMsg > 10000 )											// 10초가 지났다면,
//		//{
//		//	m_dwLastMsg = gCurTime;														// 마지막 시간에 현재 시간을 기록하고,
//		//	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(226));						// 퀘스트 메시지를 채팅창에 출력한다.
//		//}
//	}
//}

// 서브 퀘스트를 종료한다.
void CQuestManager::EndSubQuest( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwSubQuestFlag, DWORD dwSubData, DWORD dwSubTime )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// 퀘스트 인덱스로 퀘스트 테이블에서 퀘스트 정보를 받는다.

	if( !pQuest ) return ;																// 퀘스트 정보가 유효한지 체크한다.

	pQuest->EndSubQuest( dwSubQuestIdx, dwSubQuestFlag, dwSubData, dwSubTime );			// 서브 퀘스트를 종료 한다.

	m_QuestDialog->RefreshQuestList() ;
}

//// 서브 퀘스트를 종료한다.
//void CQuestManager::EndSubQuest( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwSubQuestFlag, DWORD dwSubData, DWORD dwSubTime )
//{
//	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// 퀘스트 인덱스로 퀘스트 테이블에서 퀘스트 정보를 받는다.
//
//	if( pQuest )																		// 퀘스트 정보가 유효한지 체크한다.
//	{
//		pQuest->EndSubQuest( dwSubQuestIdx, dwSubQuestFlag, dwSubData, dwSubTime );		// 서브 퀘스트를 종료 한다.
//
//		////
//		//int Key = 0;																	// 키 값을 받을 변수를 선언한다.
//		//COMBINEKEY( dwQuestIdx, dwSubQuestIdx, Key );									// 퀘스트 인덱스와 서브 퀘스트 인덱스로, 키값을 받는다.
//		//m_QuestUpdater->ProcessQuestDelete(m_QuestStringTable.GetData(Key));			// 퀘스트 업데이터에 퀘스트를 추가한다.
//	}
//
//	int QuestStringKey = 0 ;
//	COMBINEKEY(dwQuestIdx, dwSubQuestIdx, QuestStringKey) ;							// 퀘스트, 서브 퀘스트 인덱스로 퀘스트 스트링을 구한다.
//	CQuestString* pQString = GetQuestString(QuestStringKey);						// 서브 퀘스트 스트링을 받는다.
//
//	if( pQString )
//	{
//		pQString->SetComplete(TRUE) ;
//
//		CQuestDialog* pDlg = GAMEIN->GetQuestDialog() ;
//
//		if( pDlg )
//		{
//			//pDlg->QuestDescView(pQString) ;
//
//			pDlg->QuestListReset() ;
//			pDlg->QuestDescView(pDlg->QuestTitleString(1)) ;
//			pDlg->GetQuestDescList()->ResetGuageBarPos() ;
//		}
//	}
//
//	/*CQuestDialog* pQuestDialog = GAMEIN->GetQuestDialog() ;
//
//	if( pQuestDialog )
//	{
//		pQuestDialog->QuestListReset() ;
//		pQuestDialog->QuestDescView(pQuestDialog->QuestTitleString(1)) ;
//		pQuestDialog->GetQuestDescList()->ResetGuageBarPos() ;
//	}*/
//}

void CQuestManager::UpdateSubQuestData( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwMaxCount, DWORD dwSubData, DWORD dwSubTime )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// 퀘스트 인덱스로, 퀘스트테이블에서 퀘스트 정보를 받는다.

	if( !pQuest ) return ;																// 퀘스트 정보가 유효한지 체크한다.

	pQuest->UpdateSubQuestData( dwSubQuestIdx, dwMaxCount, dwSubData, dwSubTime );		// 서브 퀘스트 정보를 업데이트 한다.

	m_QuestDialog->RefreshQuestList() ;
}

//// 서브 퀘스트 데이터를 업데이트 한다.
//void CQuestManager::UpdateSubQuestData( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwMaxCount, DWORD dwSubData, DWORD dwSubTime )
//{
//	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// 퀘스트 인덱스로, 퀘스트테이블에서 퀘스트 정보를 받는다.
//
//	if( pQuest )																		// 퀘스트 정보가 유효한지 체크한다.
//	{
//		pQuest->UpdateSubQuestData( dwSubQuestIdx, dwMaxCount, dwSubData, dwSubTime );	// 서브 퀘스트 정보를 업데이트 한다.
//
//		CQuestDialog* pDlg = GAMEIN->GetQuestTotalDialog()->GetQuestDlg() ;
//
//		if( pDlg )
//		{
//			pDlg->UpdateQuestDescView(dwQuestIdx, dwSubQuestIdx) ;
//		}
//
//		////
//		//m_QuestUpdater->UpdateSubQuestData();											// 퀘스트 업데이터에 서브 퀘스트 데이터를 업데이트 한다.
//
//		//CQuestQuickViewDialog* pQuickView = NULL ;														// 퀘스트 알림이 다이얼로그 정보를 받을 포인터를 선언하고 null처리 한다.
//		//pQuickView = GAMEIN->GetQuestQuickViewDialog() ;												// 퀘스트 알림이 다이얼로그 정보를 받는다.
//
//		//if( pQuickView )																				// 퀘스트 알림이 다이얼로그 정보가 유효하다면,
//		//{
//		//	pQuickView->ResetQuickViewDialog() ;																// 퀘스트 알림이의 메인 퀘스트 데이터를 로딩한다.
//		//}
//	}
//}

void CQuestManager::QuestItemAdd( DWORD dwItemIdx, DWORD dwCount )						// 퀘스트 아이템을 추가하는 함수.
{
	QUEST_ITEM_INFO* pData = m_QuestItemMGR.GetQuestItem( dwItemIdx );					// 퀘스트 아이템 매니져에서 아이템 인덱스로, 퀘스트 아이템 정보를 받는다.

	if( !pData ) return;																// 아이템 정보가 유효하지 않으면, 리턴 처리를 한다.

	m_QuestUpdater->QuestItemAdd( pData, dwCount );										// 퀘스트 업데이터에 퀘스트 아이템을 추가한다.

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(224), pData->ItemName );			// 채팅창에, 아이템 명을 출력한다.
}

void CQuestManager::QuestItemDelete( DWORD dwItemIdx )									// 퀘스트 아이템 삭제 함수.
{
	// itemname
	QUEST_ITEM_INFO* pData = m_QuestItemMGR.GetQuestItem( dwItemIdx );					// 아이템 인덱스로, 퀘스트 아이템 매니져에서 퀘스트 아이템 정보를 받는다.

	if( !pData ) return;																// 아이템 정보가 유효하지 않으면, 리턴 처리를 한다.

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(225), pData->ItemName );			// 채팅창에 아이템명과 메시지를 결합하여 추가한다.

	m_QuestUpdater->QuestItemDelete( dwItemIdx );										// 퀘스트 업데이터에서 퀘스트 아이템을 삭제한다.
}

void CQuestManager::QuestItemUpdate( DWORD dwType, DWORD dwItemIdx, DWORD dwCount )		// 퀘스트 아이템을 업데이트 하는 함수.
{
	m_QuestUpdater->QuestItemUpdate( dwType, dwItemIdx, dwCount);						// 퀘스트 업데이터에서 퀘스트 아이템을 업데이트 한다.

	// itemname
	QUEST_ITEM_INFO* pData = m_QuestItemMGR.GetQuestItem( dwItemIdx );					// 받은 아이템 인덱스로 퀘스트 아이템 매니져에서 퀘스트 아이템 정보를 받는다.

	if( !pData ) return;																// 아이템 정보가 유효하지 않으면, 리턴처리를 한다.

	m_QuestUpdater->QuestItemAdd( pData, dwCount );										// 퀘스트 업데이터에 아이템을 추가한다.

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(224), pData->ItemName );			// 채팅창에 아이템 이름과 메시지를 결합하여 출력한다.
}

BOOL CQuestManager::IsQuestComplete( DWORD dwQuestIdx )									// 퀘스트 완료 여부를 반환하는 함수.
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// 들어온 인덱스로 퀘스트 테이블에서 퀘스트 정보를 받는다.

	if( pQuest )																		// 퀘스트 정보가 유효한지 체크한다.
	{
		if( pQuest->IsQuestComplete() )													// 퀘스트가 완료 되었다면,
		{
			return TRUE;																// TRUE를 리턴한다.
		}
		else																			// 퀘스트가 완료되지 않았다면,
		{
			return FALSE;																// FALSE를 리턴한다.
		}
	}

	return FALSE;																		// FALSE를 리턴한다.
}

void CQuestManager::DeleteQuest( DWORD dwQuestIdx, BOOL bCheckTime )					// 퀘스트를 삭제하는 함수.
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// 들어온 인덱스로 퀘스트 테이블에서 퀘스트 정보를 받는다.

	if( !pQuest ) return ;																// 퀘스트 정보가 유효한지 체크한다.

	if( pQuest->IsQuestComplete() )	return ;											// 퀘스트를 완료하였다면,

	pQuest->Initialize();																// 퀘스트를 초기화 한다.
	m_QuestUpdater->DeleteQuest( dwQuestIdx );											// 퀘스트 업데이터에서 퀘스트를 삭제한다.

	m_QuestDialog->EndQuest(dwQuestIdx) ;
	m_QuickView->DeleteQuest(dwQuestIdx) ;

	int Key = 0;																	// 키 값을 받을 변수를 선언하고, 0으로 세팅한다.
	COMBINEKEY( dwQuestIdx, 0, Key );												// 퀘스트 인덱스와 0으로 키값을 받는다.

	CQuestString* pQString = m_QuestStringTable.GetData(Key);						// 키 값으로 퀘스트 스트링 테이블에서 퀘스트 스트링 정보를 받는다.

	if( pQString == NULL )	return;													// 퀘스트 정보가 유효하지 않으면, 리턴 처리를 한다.

	cPtrList* p = pQString->GetTitle();												// 리스트 포인터를 선언하고, 스트링의 제목을 받는다.
	ITEM* string = (ITEM*)p->GetHead();										// 리스트 포인터를 헤드로 세팅하여 스트링 포인터 변수로 받는다.

	if( pQuest->IsTimeCheck() && bCheckTime )										// 퀘스트가 시간을 체크한다면,
	{
		CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(295), string->string );		// 진행시간 초과로 종료되었다는 메시지를 출력한다.

		char msg[256] = {0, };														// 임시 버퍼를 선언한다.
		CompositString( msg, CHATMGR->GetChatMsg(295), string->string );				// 공지 메시지를 만든다.

		STATUSICONDLG->RemoveQuestTimeIcon( HERO, eStatusIcon_QuestTime );			// 시간체크 아이콘을 지운다.
	}
	else																			// 시간체크를 하지 않는다면,
	{
		CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(223), string->string );		// 퀘스트를 포기했다는 메시지를 채팅창에 출력한다.

		char msg[256] = {0, };														// 임시 버퍼를 선언한다.
		CompositString( msg, CHATMGR->GetChatMsg(223), string->string );				// 공지 메시지를 만든다.

		STATUSICONDLG->RemoveQuestTimeIcon( HERO, eStatusIcon_QuestTime );			// 퀘스트 시간 아이콘을 삭제한다.
	}
}

//void CQuestManager::DeleteQuest( DWORD dwQuestIdx, BOOL bCheckTime )					// 퀘스트를 삭제하는 함수.
//{
//	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// 들어온 인덱스로 퀘스트 테이블에서 퀘스트 정보를 받는다.
//
//	if( pQuest )																		// 퀘스트 정보가 유효한지 체크한다.
//	{
//		if( pQuest->IsQuestComplete() )													// 퀘스트를 완료하였다면,
//		{
//			return;																		// 리턴 처리를 한다.
//		}
//		//
//		int Key = 0;																	// 키 값을 받을 변수를 선언하고, 0으로 세팅한다.
//		COMBINEKEY( dwQuestIdx, 0, Key );												// 퀘스트 인덱스와 0으로 키값을 받는다.
//
//		CQuestString* pQString = m_QuestStringTable.GetData(Key);						// 키 값으로 퀘스트 스트링 테이블에서 퀘스트 스트링 정보를 받는다.
//
//		if( pQString == NULL )	return;													// 퀘스트 정보가 유효하지 않으면, 리턴 처리를 한다.
//
//		cPtrList* p = pQString->GetTitle();												// 리스트 포인터를 선언하고, 스트링의 제목을 받는다.
//		ITEM* string = (ITEM*)p->GetHead();										// 리스트 포인터를 헤드로 세팅하여 스트링 포인터 변수로 받는다.
//
//		if( pQuest->IsTimeCheck() && bCheckTime )										// 퀘스트가 시간을 체크한다면,
//		{
//			CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(295), string->string );		// 진행시간 초과로 종료되었다는 메시지를 출력한다.
//
//			char msg[256] = {0, };														// 임시 버퍼를 선언한다.
//			CompositString( msg, CHATMGR->GetChatMsg(295), string->string );				// 공지 메시지를 만든다.
//
//			STATUSICONDLG->RemoveQuestTimeIcon( HERO, eStatusIcon_QuestTime );			// 시간체크 아이콘을 지운다.
//		}
//		else																			// 시간체크를 하지 않는다면,
//		{
//			CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(223), string->string );		// 퀘스트를 포기했다는 메시지를 채팅창에 출력한다.
//
//			char msg[256] = {0, };														// 임시 버퍼를 선언한다.
//			CompositString( msg, CHATMGR->GetChatMsg(223), string->string );				// 공지 메시지를 만든다.
//
//			STATUSICONDLG->RemoveQuestTimeIcon( HERO, eStatusIcon_QuestTime );			// 퀘스트 시간 아이콘을 삭제한다.
//		}
//
//		pQuest->Initialize();															// 퀘스트를 초기화 한다.
//		m_QuestUpdater->DeleteQuest( dwQuestIdx );										// 퀘스트 업데이터에서 퀘스트를 삭제한다.
//	}
//}

BOOL CQuestManager::IsNpcRelationQuest( DWORD dwNpcIdx )								// NPC와 관계된 퀘스트인지 반환하는 함수.
{
	CQuest* pQuest = NULL;																// 퀘스트 포인터를 선언하고, NULL처리를 한다.

	m_QuestTable.SetPositionHead();														// 퀘스트 테이블을 헤드로 세팅한다.

	while( pQuest = m_QuestTable.GetData() )											// 퀘스트 테이블에 퀘스트 정보가 있는동안 while문을 돌린다.
	{
		if( pQuest->IsNpcRelationQuest( dwNpcIdx ) )									// npc 인덱스로 퀘스트가 npc와 관계된 퀘스트인지 체크한다.
		{
			return TRUE;																// npc와 관련이 있으면, TRUE를 리턴한다.
		}
	}

	return FALSE;																		// FALSE를 리턴한다.
}

DWORD CQuestManager::GetNpcScriptPage( DWORD dwNpcIdx )									// NPC 스크립트 페이지를 리턴하는 함수.
{
	DWORD dwPage = 0;																	// 페이지를 담을 변수를 선언하고 0으로 세팅한다.

	CQuest* pQuest = NULL;																// 퀘스트 정보를 받을 포인터 변수를 선언한다.

	m_QuestTable.SetPositionHead();														// 퀘스트 테이블을 헤드로 세팅한다.

	while( pQuest = m_QuestTable.GetData() )											// 퀘스트 테이블에 퀘스트 정보가 있을 동안 while문을 돌린다.
	{
		dwPage = pQuest->GetNpcScriptPage( dwNpcIdx );									// npc 인덱스로, npc 스크립트 페이지를 받는다.

		if( dwPage )																	// 페이지 정보가 유효하다면,
		{
			return dwPage;																// 페이지를 리턴한다.
		}
	}

	return 0;																			// 0을 리턴한다.
}

void CQuestManager::SendNpcTalkEvent( DWORD dwNpcIdx, DWORD dwNpcScriptIdx )			// npc 대화 이벤트를 보내는 함수.
{
	SEND_QUEST_IDX Msg;																	// 퀘스트 정보를 담을 메시지 구조체를 선언한다.

	Msg.Category = MP_QUEST;															// 카테고리를 퀘스트로 세팅한다.
	Msg.Protocol = MP_QUESTEVENT_NPCTALK;												// 프로토콜을 퀘스트 이벤트 중 npc 대화로 세팅한다.
	Msg.dwObjectID = HERO->GetID();														// HERO의 아이디를 세팅한다.
	Msg.MainQuestIdx = (WORD)dwNpcIdx;													// 메인 퀘스트 인덱스를 세팅한다.
	Msg.SubQuestIdx = (WORD)dwNpcScriptIdx;												// 서브 퀘스트 인덱스를 세팅한다.

	/*CQuestDialog* pQuestDialog = GAMEIN->GetQuestDialog() ;

	if( pQuestDialog ) 
	{
		Msg.dwData = pQuestDialog->GetQuestCount() ;
	}*/

	NETWORK->Send( &Msg, sizeof(Msg) );													// 세팅 된 정보로 메시지를 전송한다.
}

void CQuestManager::SendNpcTalkEventForEvent( DWORD dwNpcIdx, DWORD dwNpcScriptIdx )	// 이벤트를 위한 npc대화 이벤트를 보내는 함수.
{
	SEND_QUEST_IDX Msg;																	// 퀘스트 정보를 담을 메시지 구조체를 선언한다.

	Msg.Category = MP_QUEST;															// 카테고리를 퀘스트로 세팅한다.
	Msg.Protocol = MP_QUESTEVENT_NPCTALK_SYN;											// 프로토콜을 퀘스트 이벤트 중 npc 대화 요청으로 세팅한다.
	Msg.dwObjectID = HERO->GetID();														// HERO 아이디를 세팅한다.
	Msg.MainQuestIdx = (WORD)dwNpcIdx;													// npc 인덱스를 세팅한다.
	Msg.SubQuestIdx = (WORD)dwNpcScriptIdx;												// npc 스크립트 인덱스를 세팅한다.

	NETWORK->Send( &Msg, sizeof(Msg) );													// 세팅 된 정보로 메시지를 전송한다.
}

void CQuestManager::SetNpcData( CNpc* pNpc )											// npc 데이터를 세팅하는 함수.
{
	if( m_dwMaxNpc > MAX_NPCARRAY-1 )													// 최대 npc 인덱스가 31보다 큰지 체크한다.
	{
		return;																			// 크다면, 리턴처리를 한다.
	}

	m_pNpcArray[m_dwMaxNpc] = pNpc;														// npc 정보를 담는 배열 인덱스에 npc 정보를 담는다.

	++m_dwMaxNpc;																		// 최대 npc 번호를 증가시킨다.
}

void CQuestManager::SetNpcIDName(WORD wIdx, stNPCIDNAME* pInfo)							// npc 이름과 아이디를 세팅하는 함수.
{
	if( wIdx > MAX_NPCIDNAME-1 )
	{
		ASSERTMSG(0, "스태틱 NPC 정보가 100개를 넘었음, MAX_NPCIDNAME 수치를 늘려주시오.") ;
		return ;
	}

	m_stNPCIDNAME[wIdx].wId = pInfo->wId ;
	strcpy(m_stNPCIDNAME[wIdx].name, pInfo->name) ;
}

stNPCIDNAME* CQuestManager::GetNpcIdName(DWORD npcIdx)
{
	if( npcIdx < 0 || npcIdx > MAX_NPCIDNAME-1 )
	{
		return NULL ;
	}

	for( int count = 0 ; count < MAX_NPCIDNAME ; ++count )
	{
		if( m_stNPCIDNAME[count].wId == (WORD)npcIdx )
		{
			return &m_stNPCIDNAME[count] ;
		}
	}

	return NULL ;
}





void CQuestManager::ProcessNpcMark()													// npc 마크 프로세스 함수.
{
	if( ! HERO )																		// HERO 정보가 유효하지 않으면,
	{
		return;																			// 리턴 처리를 한다.
	}

	CMiniMapDlg* miniMapDialog = GAMEIN->GetMiniMapDialog();							// 미니맵 다이얼로그 정보를 받아온다.
	ASSERT( miniMapDialog );															// 미니맵 다이얼로그 정보를 ASSERT 체크 한다.

	for( DWORD i = 0; i < m_dwMaxNpc; ++i )												// npc 수 만큼 for문을 돌린다.
	{
		CNpc* object = m_pNpcArray[ i ];												// NPC 정보를 받는다.

		if( !object ) continue ;														// NPC 정보가 유효하지 않으면 continue 처리를 한다.

		object->RemoveAllObjectEffect();												// NPC의 모든 이펙트를 삭제한다.

		miniMapDialog->ShowQuestMarkIcon( object, eQM_NONE );							// 미니맵 다이얼로그의 퀘스트 마크를 보통으로 세팅한다.
	}

	m_QuestTable.SetPositionHead();														// 퀘스트 테이블을 헤드 포지션으로 세팅한다.

	CQuest* pQuest ;

	while(TRUE)
	{
		pQuest = NULL ;
		pQuest = m_QuestTable.GetData() ;

		if( !pQuest ) break ;

		for( DWORD i = 0; i < m_dwMaxNpc; ++i )											// npc 수만큼 for문을 돌린다.
		{
			CNpc* object = m_pNpcArray[ i ];											// NPC 정보를 받는다.

			if( !object ) continue ;

			switch( pQuest->GetNpcMarkType( object->GetNpcUniqueIdx() ) )				// NPC의 마크 타입을 확인한다.
			{
				case 1:																	// 마크 타입이 !라면,
				{
					NPC_LIST* pNpcList = GAMERESRCMNGR->GetNpcInfo( object->GetNpcKind() );	// NPC 리스트 정보를 받는다.

					VECTOR3 Pos;														// 위치 정보를 담을 벡터를 선언한다.
					Pos.x = Pos.z = 0.f;												// 위치 정보를 세팅한다.
					Pos.y = float( pNpcList->Tall ) + 50.0f;

					if( object->IsNpcMark( NPCMARK_2_ID ) )								// NPC 마크가 ?라면,
					{
						object->RemoveObjectEffect( NPCMARK_2_ID );						// NPC마크 ?를 삭제한다.

						OBJECTEFFECTDESC desc( 17 );									// 오브젝트 이펙트 디스크립션을 선언한다.
						desc.Position = Pos;											// 위치 정보를 세팅한다.

						object->AddObjectEffect( NPCMARK_1_ID, &desc, 1, HERO );		// 오브젝트 이펙트를 추가한다.

						miniMapDialog->ShowQuestMarkIcon( object, eQM_EXCLAMATION );	// 미니맵에 퀘스트 마크를 eQM_EXCLAMATION로 세팅한다.
					}
					else																// NPC 마크가 !도?도 아니면,
					{
						OBJECTEFFECTDESC desc( 17 );									// 오브젝트 이펙트 디스크립션을 선언한다.
						desc.Position = Pos;											// 위치 정보를 세팅한다.

						object->AddObjectEffect( NPCMARK_1_ID, &desc, 1, HERO );		// 오브젝트에 이펙트를 추가한다.


						miniMapDialog->ShowQuestMarkIcon( object, eQM_EXCLAMATION );	// 미니맵에 퀘스트 마크를 보여준다.
					}

					break;
				}
				case 2:																	// 마크 타입이 ?라면,
				{
					NPC_LIST* pNpcList = GAMERESRCMNGR->GetNpcInfo( object->GetNpcKind() );	// NPC 리스트 정보를 받는다.

					VECTOR3 Pos;														// 위치 정보를 담을 벡터를 선언한다.
					Pos.x = Pos.z = 0.f;												// 위치를 세팅한다.
					Pos.y = float( pNpcList->Tall ) + 50.0f;

					if( object->IsNpcMark( NPCMARK_1_ID ) )								// NPC 마크가 !와 같다면,
					{
						//object->RemoveObjectEffect( NPCMARK_1_ID );						// NPC의 !마크를 삭제한다.
					}
					else if( object->IsNpcMark( NPCMARK_2_ID ) )						// NPC 마크가 ?와 같다면,
					{

					}
					else																// NPC 마크가 !도?도 아니면,
					{
						OBJECTEFFECTDESC desc( 18 );									// 이펙트 디스크립트 구조체를 선언한다.
						desc.Position = Pos;                                            // 위치를 세팅한다.                                   
						object->AddObjectEffect( NPCMARK_2_ID, &desc, 1, HERO );		// 이펙트를 추가한다.

						miniMapDialog->ShowQuestMarkIcon( object, eQM_QUESTION );		// 미니맵에 퀘스트 아이콘을 보여준다.
					}

					break;
				}
			}
		}
	}
}





//void CQuestManager::ProcessNpcMark()													// npc 마크 프로세스 함수.
//{
//	if( ! HERO )																		// HERO 정보가 유효하지 않으면,
//	{
//		return;																			// 리턴 처리를 한다.
//	}
//
//	CMiniMapDlg* miniMapDialog = GAMEIN->GetMiniMapDialog();							// 미니맵 다이얼로그 정보를 받아온다.
//	ASSERT( miniMapDialog );															// 미니맵 다이얼로그 정보를 ASSERT 체크 한다.
//
//	for( DWORD i = 0; i < m_dwMaxNpc; ++i )																		// npc 수 만큼 for문을 돌린다.
//	{
//		CNpc* object = m_pNpcArray[ i ];												// NPC 정보를 받는다.
//
//		if( object )																	// NPC 정보가 유효하다면,
//		{
//			object->RemoveAllObjectEffect();											// NPC의 모든 이펙트를 삭제한다.
//
//			miniMapDialog->ShowQuestMarkIcon( object, eQM_NONE );						// 미니맵 다이얼로그의 퀘스트 마크를 보통으로 세팅한다.
//		}
//	}
//
//	m_QuestTable.SetPositionHead();														// 퀘스트 테이블을 헤드 포지션으로 세팅한다.
//
//	for( CQuest* pQuest; pQuest = m_QuestTable.GetData(); )								// 퀘스트 테이블에서 퀘스트 정보를 받고, 퀘스트 정보가 유효할 동안 FOR문을 돌린다.
//	{
//		for( DWORD i = 0; i < m_dwMaxNpc; ++i )											// npc 수만큼 for문을 돌린다.
//		{
//			CNpc* object = m_pNpcArray[ i ];											// NPC 정보를 받는다.
//
//			if( ! object )																// NPC 정보가 유효하지 않으면,
//			{
//				continue;																// continue를 한다.
//			}
//
//			switch( pQuest->GetNpcMarkType( object->GetNpcUniqueIdx() ) )				// NPC의 마크 타입을 확인한다.
//			{
//				case 1:																	// 마크 타입이 !라면,
//				{
//					NPC_LIST* pNpcList = GAMERESRCMNGR->GetNpcInfo( object->GetNpcKind() );	// NPC 리스트 정보를 받는다.
//
//					VECTOR3 Pos;														// 위치 정보를 담을 벡터를 선언한다.
//					Pos.x = Pos.z = 0.f;												// 위치 정보를 세팅한다.
//					Pos.y = float( pNpcList->Tall ) + 50.0f;
//
//					if( object->IsNpcMark( NPCMARK_1_ID ) )								// NPC 마크가 !라면,
//					{
//						//	object->RemoveObjectEffect( NPCMARK_1_ID );					// 아무것도 하지 않는다.
//					}
//					else if( object->IsNpcMark( NPCMARK_2_ID ) )						// NPC 마크가 ?라면,
//					{
//						object->RemoveObjectEffect( NPCMARK_2_ID );						// NPC마크 ?를 삭제한다.
//
//						OBJECTEFFECTDESC desc( 17 );									// 오브젝트 이펙트 디스크립션을 선언한다.
//						desc.Position = Pos;											// 위치 정보를 세팅한다.
//
//						object->AddObjectEffect( NPCMARK_1_ID, &desc, 1, HERO );		// 오브젝트 이펙트를 추가한다.
//
//						miniMapDialog->ShowQuestMarkIcon( object, eQM_EXCLAMATION );	// 미니맵에 퀘스트 마크를 eQM_EXCLAMATION로 세팅한다.
//					}
//					else																// NPC 마크가 !도?도 아니면,
//					{
//						OBJECTEFFECTDESC desc( 17 );									// 오브젝트 이펙트 디스크립션을 선언한다.
//						desc.Position = Pos;											// 위치 정보를 세팅한다.
//
//						object->AddObjectEffect( NPCMARK_1_ID, &desc, 1, HERO );		// 오브젝트에 이펙트를 추가한다.
//
//
//						miniMapDialog->ShowQuestMarkIcon( object, eQM_EXCLAMATION );	// 미니맵에 퀘스트 마크를 보여준다.
//					}
//
//					break;
//				}
//				case 2:																	// 마크 타입이 ?라면,
//				{
//					NPC_LIST* pNpcList = GAMERESRCMNGR->GetNpcInfo( object->GetNpcKind() );	// NPC 리스트 정보를 받는다.
//
//					VECTOR3 Pos;														// 위치 정보를 담을 벡터를 선언한다.
//					Pos.x = Pos.z = 0.f;												// 위치를 세팅한다.
//					Pos.y = float( pNpcList->Tall ) + 50.0f;
//
//					if( object->IsNpcMark( NPCMARK_1_ID ) )								// NPC 마크가 !와 같다면,
//					{
//						object->RemoveObjectEffect( NPCMARK_1_ID );						// NPC의 !마크를 삭제한다.
//					}
//					else if( object->IsNpcMark( NPCMARK_2_ID ) )						// NPC 마크가 ?와 같다면,
//					{
//
//					}
//					else																// NPC 마크가 !도?도 아니면,
//					{
//						OBJECTEFFECTDESC desc( 18 );									// 이펙트 디스크립트 구조체를 선언한다.
//						desc.Position = Pos;                                            // 위치를 세팅한다.                                   
//						object->AddObjectEffect( NPCMARK_2_ID, &desc, 1, HERO );		// 이펙트를 추가한다.
//
//						miniMapDialog->ShowQuestMarkIcon( object, eQM_QUESTION );		// 미니맵에 퀘스트 아이콘을 보여준다.
//					}
//
//					break;
//				}
//			}
//		}
//	}
//}





void CQuestManager::NetworkMsgParse( BYTE Protocol, void* pMsg )									// 퀘스트 네트워크 메시지 파싱함수.
{
	if(!pMsg) return ;																				// 인자로 넘어온 메시지 정보가 유효하지 않으면, return 처리를 한다.

	switch(Protocol)																				// 프로토콜 정보를 확인한다.
	{
	case MP_QUEST_MAINDATA_LOAD:				Quest_Maindata_Load( pMsg ) ;			break;		// 메인 데이터 로드 처리를 한다.
	case MP_QUEST_SUBDATA_LOAD:					Quest_Subdata_Load( pMsg ) ;			break;		// 서브 데이터 로드 처리를 한다.
	case MP_QUEST_ITEM_LOAD:					Quest_Item_Load( pMsg ) ;				break;		// 아이템 로드 처리를 한다.
	case MP_QUEST_START_ACK:					Quest_Start_Ack( pMsg ) ;				break;		// 퀘스트 시작 처리를 한다.
	case MP_SUBQUEST_START_ACK:					SubQuest_Start_Ack( pMsg ) ;			break;		// 서브 퀘스트 시작 처리를 한다.
	case MP_QUEST_END_ACK:						Quest_End_Ack( pMsg )  ;				break;		// 퀘스트 종료 처리를 한다.
	case MP_SUBQUEST_END_ACK:					SubQuest_End_Ack( pMsg ) ;				break;		// 서브 퀘스트 종료 처리를 한다.
	case MP_SUBQUEST_UPDATE:					SubQuest_Update( pMsg ) ;				break;		// 서브 퀘스트 업데이트 처리를 한다.
	case MP_QUEST_DELETE_ACK:					Quest_Delete_Ack( pMsg ) ;				break;		// 퀘스트 삭제를 처리한다.
	case MP_QUEST_DELETE_CONFIRM_ACK:			Quest_Delete_Confirm_Ack( pMsg ) ;		break;		// 퀘스트 삭제 컨펌 처리를 한다.
	case MP_QUESTITEM_GIVE:						QuestItem_Give( pMsg ) ;				break;		// 퀘스트 아이템을 주는 처리를 한다.
	case MP_QUESTITEM_TAKE:						QuestItem_Take( pMsg ) ;				break;		// 퀘스트 아이템을 수거하는 처리를 한다.
	case MP_QUESTITEM_UPDATE:					QuestItem_Update( pMsg ) ;				break;		// 퀘스트 아이템 업데이트 처리를 한다.
	case MP_QUEST_TAKEITEM_ACK:					Quest_TakeItem_Ack( pMsg ) ;			break;		// 퀘스트 아이템 수거 처리를 한다.
	case MP_QUEST_TAKEMONEY_ACK:				Quest_Takemoney_Ack( pMsg ) ;			break;		// 퀘스트 머니 획득 처리를 한다.
	case MP_QUEST_TAKEEXP_ACK:					Quest_TakeExp_Ack( pMsg ) ;				break;		// 퀘스트 경험치 획득 처리를 한다.
	case MP_QUEST_TAKESEXP_ACK:					Quest_TakesExp_Ack( pMsg ) ;			break;		// 퀘스트 경험치 획득 처리를 한다.
	case MP_QUEST_GIVEITEM_ACK:					Quest_GiveItem_Ack( pMsg ) ;			break;		// 퀘스트 아이템 분배 처리를 한다.
	// 071026 LYW --- QuestManager : Add process part of give stack item.
	case MP_QUEST_GIVEITEMS_ACK :				Quest_GiveItems_Ack( pMsg ) ;			break ;		// 퀘스트 스택 아이템 분배 처리를 한다.
	case MP_QUEST_GIVEMONEY_ACK:				Quest_GiveMoney_Ack( pMsg ) ;			break;		// 퀘스트 머니 분배 처리를 한다.
	case MP_QUEST_TIME_LIMIT:					Quest_Time_Limit( pMsg ) ;				break;		// 퀘스트 시간 제한 처리를 한다.
	case MP_QUEST_REGIST_CHECKTIME:				Quest_Regist_Checktime( pMsg ) ;		break;		// 퀘스트 시간 체크 등록 처리를 한다.
	case MP_QUEST_UNREGIST_CHECKTIME:			Quest_Unregist_Checktime( pMsg ) ;		break;		// 퀘스트 시간 체크 탈퇴 처리를 한다.
	case MP_QUESTEVENT_NPCTALK_ACK:				QuestEvent_NpcTalk_Ack( pMsg ) ;		break;		// 퀘스트 이벤트 npc 대화 수락 처리를 한다.
	case MP_QUESTEVENT_NPCTALK_NACK:			QuestEvent_NpcTalk_Nack( pMsg ) ;		break;		// 퀘스트 이벤트 npc 대화 실패 처리를 한다.
	case MP_QUEST_EXECUTE_ERROR:				Quest_Execute_Error( pMsg ) ;			break;		// 퀘스트 에러 처리를 한다.
	case MP_QUEST_ERROR_EXT :					Quest_Error_Ext( pMsg ) ;				break;		// 퀘스트 기타 에러 처리를 한다.
	case MP_QUEST_ERROR_FULLQUEST :				Quest_Error_FullQuest( pMsg ) ;			break ;		// 퀘스트 목록이 다 찼다는 에러 메시지를 처리한다.

	// 071028 LYW --- QuestManager : Add process part of error when failed give item.
	case MP_QUEST_GIVEITEM_ERR :				Quest_GiveItem_Error( pMsg ) ;			break ;		// 퀘스트 아이템 반납 에러 처리를 한다.

	default:																			break;		// 디폴트 처리.
	}

	GAMEIN->GetNpcScriptDialog()->RefreshQuestLinkPage() ;

	CQuestQuickViewDialog* pQuickView = NULL ;														// 퀘스트 알림이 다이얼로그 정보를 받을 포인터를 선언하고 null처리 한다.
	pQuickView = GAMEIN->GetQuestQuickViewDialog() ;												// 퀘스트 알림이 다이얼로그 정보를 받는다.

	if( pQuickView )																				// 퀘스트 알림이 다이얼로그 정보가 유효하다면,
	{
		pQuickView->ResetQuickViewDialog() ;																// 퀘스트 알림이의 메인 퀘스트 데이터를 로딩한다.
	}
}

//
#include "GameIn.h"																					// 게임인 헤더파일을 불러온다.
#include "QuestTotalDialog.h"																		// 퀘스트 토탈 다이얼로그 헤더를 불러온다.

void CQuestManager::AddStringTest()																	// 테스트 함수.
{
	CQuestString* pQuestString = NULL;																// 호출하는 곳이 없다.
}

void CQuestManager::ViewToggle()																	// 토글 뷰 함수.
{
	GAMEIN->GetQuestTotalDialog()->QuestListView();													// 호출하는 곳이 없다.
}

void CQuestManager::AddJournalTest()																// 저널 추가 테스트 함수.(호출하는 곳이 없다.)
{
	CQuestString* pQuestString = NULL;																// 퀘스트 스트링 포인터를 선언하고, NULL 처리를 한다.

	char buf[16] = { 0, };																			// 임시 버퍼를 선언한다.
	sprintf(buf, "2004.08.06");																		// 임시 버퍼에 날짜를 복사한다.
	
	JOURNALINFO item;																				// 저널 정보 구조체를 선언한다.

	item.Kind = eJournal_Quest;																		// 종류를 eJournal_Quest로 세팅한다.

	item.bSaved = FALSE;																			// 세이브 여부를 FALSE로 세팅한다.
	item.Param = 1;																					// 파라메터를 1로 세팅한다.

	strcpy(item.RegDate, buf);																		// 아이템 등록 데이터를 버퍼에 복사한다.

	m_QuestUpdater->JournalItemAdd(&item);															// 퀘스트 업데이터에 저널 아이템을 추가한다.

	item.Param = 2;																					// 파라메터를 2로 세팅한다.
	item.Param_2 = 0;																				// 파라메터2는 0으로 세팅한다.

	m_QuestUpdater->JournalItemAdd(&item);															// 퀘스트 업데이터에 저널 아이템을 추가한다.

	item.Param = 3;																					// 파라메터를 3으로 세팅한다.

	m_QuestUpdater->JournalItemAdd(&item);															// 퀘스트 업데이터에 저널 아이템을 추가한다.

}

void CQuestManager::QuestAbandon()																	// 퀘스트 포기 처리 함수.
{
	DWORD QuestIdx = 0 ;																			// 퀘스트 인덱스를 담을 변수를 선언하고 0으로 세팅한다.

	CQuestDialog* pDlg = NULL ;																		// 퀘스트 다이얼로그 정보를 받을 포인터를 선언하고 null처리를 한다.
	pDlg = GAMEIN->GetQuestTotalDialog()->GetQuestDlg() ;											// 퀘스트 다이얼로그 정보를 받는다.

	if( pDlg )																						// 퀘스트 다이얼로그 정보가 유효하다면,
	{
		QuestIdx = pDlg->GetSelectedQuestID() ;														// 선택 된 퀘스트 인덱스를 받는다.

		pDlg->SetQuestCount() ;

		//int nQuestCount = 0 ;																		// 퀘스트 카운트를 담을 변수를 선언하고 0으로 세팅한다.
		//nQuestCount = pDlg->GetQuestCount() ;														// 퀘스트 카운트를 받는다.

		//if( nQuestCount == 0 )																		// 퀘스트 카운트가 0과 같으면,
		//{
		//	pDlg->SetQuestCount( 0 ) ;																// 퀘스트 카운트를 0으로 세팅한다.
		//}
		//else																						// 퀘스트 카운트가 0과 같지 않으면,
		//{
		//	pDlg->SetQuestCount( nQuestCount-1 ) ;													// 퀘스트 카운트를 세팅한다.
		//}
	}

	m_QuestUpdater->CloseMsgBox();																	// 퀘스트 업데이터의 클로즈 메시지 박스를 호출한다.

	if( IsQuestComplete(QuestIdx) )																	// 퀘스트 인덱스로 완료한 퀘스트인지 체크한다.
	{
		return;																						// 완료한 퀘스트라면, 리턴처리를 한다.
	}

	SEND_QUEST_IDX msg;																				// 퀘스트 정보를 담을 구조체를 선언한다.

	msg.Category = MP_QUEST;																		// 카테고리를 퀘스트로 세팅한다.
	msg.Protocol = MP_QUEST_DELETE_SYN;																// 프로토콜을 퀘스트 삭세 싱크로 세팅한다.
	msg.dwObjectID = HERO->GetID();																	// HERO 아이디를 세팅한다.
	msg.MainQuestIdx = (WORD)QuestIdx;																// 퀘스트 인덱스를 세팅한다.

	NETWORK->Send(&msg, sizeof(msg));																// 세팅된 정보를 전송한다.

	CQuestInfo* pQuestInfo =NULL ;																	// 퀘스트 정보를 받을 포인터를 선언하고 null 처리를 한다.
	pQuestInfo = GetQuestInfo(QuestIdx) ;															// 퀘스트 인덱스로 퀘스트 정보를 받는다.

	if(!pQuestInfo)																					// 퀘스트 정보가 유효하지 않으면,
	{
		ASSERTMSG(0, "퀘스트 포기하는 시점에서 퀘스트 정보를 로딩하지 못하였습니다.!") ;			// ASSERT 에러 체크.
		return  ;																					// 리턴 처리를 한다.
	}

	int nMapNum = pQuestInfo->GetMapNum() ;															// 퀘스트 정보에 있는 맵 번호를 받는다.

	DeleteQuestTitle(nMapNum, QuestIdx) ;															// 퀘스트 리스트에서 퀘스트를 삭제한다.

	DeleteMapTree(nMapNum) ;																		// 맵 트리를 체크후 삭제한다.

	// 설명 리스트 리셋
}

//void CQuestManager::QuestAbandon()																	// 퀘스트 포기 처리 함수.
//{
//	DWORD QuestIdx = m_QuestUpdater->GetSelectedQuestID();											// 퀘스트 업데이터에서 선택 된 퀘스트 아이디를 받는다.
//
//	m_QuestUpdater->CloseMsgBox();																	// 퀘스트 업데이터의 클로즈 메시지 박스를 호출한다.
//
//	if( IsQuestComplete(QuestIdx) )																	// 퀘스트 인덱스로 완료한 퀘스트인지 체크한다.
//	{
//		return;																						// 완료한 퀘스트라면, 리턴처리를 한다.
//	}
//
//	SEND_QUEST_IDX msg;																				// 퀘스트 정보를 담을 구조체를 선언한다.
//
//	msg.Category = MP_QUEST;																		// 카테고리를 퀘스트로 세팅한다.
//	msg.Protocol = MP_QUEST_DELETE_SYN;																// 프로토콜을 퀘스트 삭세 싱크로 세팅한다.
//	msg.dwObjectID = HERO->GetID();																	// HERO 아이디를 세팅한다.
//	msg.MainQuestIdx = (WORD)QuestIdx;																// 퀘스트 인덱스를 세팅한다.
//
//	NETWORK->Send(&msg, sizeof(msg));																// 세팅된 정보를 전송한다.
//
//	//test
//	m_QuestUpdater->DeleteQuest(QuestIdx);															// 퀘스트 업데이터의 퀘스트 삭제함수를 호출한다.
//}

void CQuestManager::QuestAbandon( DWORD dwQuestIdx )												// 퀘스트 포기함수( 퀘스트 인덱스 인자 )
{
	if( IsQuestComplete(dwQuestIdx) )																// 들어온 퀘스트 인덱스의 퀘스트를 완료 했는지 체크한다.
	{
		return;																						// 완료한 퀘스트라면, 리턴 처리를 한다.
	}

	SEND_QUEST_IDX msg;																				// 퀘스트 정보를 담을 구조체를 선언한다.

	msg.Category = MP_QUEST;																		// 카테고리를 퀘스트로 세팅한다.
	msg.Protocol = MP_QUEST_DELETE_SYN;																// 프로토콜을 퀘스트 삭제 싱크로 세팅한다.
	msg.dwObjectID = HERO->GetID();																	// HERO 아이디를 세팅한다.
	msg.MainQuestIdx = (WORD)dwQuestIdx;															// 퀘스트 인덱스를 세팅한다.

	NETWORK->Send(&msg, sizeof(msg));																// 세팅된 정보를 전송한다.

	//test
	m_QuestUpdater->DeleteQuest( dwQuestIdx );														// 퀘스트 업데이터의 퀘스트 삭제 함수를 호출한다.
}

void CQuestManager::QuestAbandonClose()																// 퀘스트 포기 닫기 함수.
{
	m_QuestUpdater->CloseMsgBox();																	// 퀘스트 업데이터의 클로즈 메시지 박스 함수를 호출한다.
}

void CQuestManager::AddQuestNpc( CSubQuestInfo* pSubQuestInfo )										// 퀘스트 npc를 추가하는 함수.
{
	if( !pSubQuestInfo ) return;																	// 서브 퀘스트 정보가 없으면, 리턴처리를 한다.

	cPtrList* pNpcList = pSubQuestInfo->GetQuestNpcList();											// 서브 퀘스트 정보의 퀘스트 npc리스트를 받는다.

	if( !pNpcList ) return;																			// npc 리스트 정보가 유효하지 않으면 리턴 처리를 한다.

	PTRLISTPOS npos = pNpcList->GetHeadPosition();													// npc 리스트를 헤드로 세팅하고 위치정보를 받는다.

	while( npos )																					// npc 리스트의 위치 정보를 담는 포인터가 유효한 동안 while문을 돌린다.
	{
		CQuestNpc* pNpc = (CQuestNpc*)pNpcList->GetNext( npos );									// 위치정보의 npc 정보를 받는다.

		if( pNpc )																					// npc 정보가 유효하다면,
		{
			cPtrList* pList = pNpc->GetNpcDataList();												// npc 데이터 리스트를 받는다.

			if( pList )																				// 리스트 정보가 유효한지 체크한다.
			{
				PTRLISTPOS pos = pList->GetHeadPosition();											// 리스트를 헤드로 세팅하고 위치 정보를 받는다.

				while( pos )																		// 위치 정보가 유효한 동안 while문을 돌린다.
				{
					CQuestNpcData* pData = (CQuestNpcData*)pList->GetNext( pos );					// 위치에 해당하는 퀘스트 npc 데이터 정보를 받는다.

					if( pData )																		// 데이터 정보가 유효한지 체크한다.
					{
						QUESTNPCINFO* pInfo = m_QuestNpcTable.GetData( pData->GetNpcIdx() );		// 데이터 정보의 npc 인덱스를 참조하여 퀘스트 npc 정보를 받는다.

						if( pInfo )																	// npc 정보가 유효한지 체크한다.
						{
							MAP->AddQuestNpc( pInfo );												// 맵에 퀘스트 npc를 추가한다.
						}
					}
				}
			}
		}
	}
}

BOOL CQuestManager::IsQuestStarted( DWORD dwQuestIdx )												// 퀘스트가 시작되었는지 여부를 리턴하는 함수.
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );											// 들어온 퀘스트 인덱스로, 퀘스트 정보를 받는다.

	if( pQuest )																					// 퀘스트 정보가 유효한지 체크한다.
	{
		if( pQuest->IsSubQuestComplete( 0 ) )														// 퀘스트의 서브 퀘스트가 완료 되었다면,
		{
			return FALSE;																			// FALSE를 리턴한다.
		}
		else																						// 그렇지 않으면,
		{
			return pQuest->CanItemUse( dwQuestIdx );												// 아이템을 사용할 수 있는지 여부를 리턴한다.
		}
	}
	return FALSE;																					// FALSE를 리턴한다.
}

char* CQuestManager::GetQuestTitle( DWORD dwQuestIdx )												// 퀘스트 타이틀을 리턴하는 함수.
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );											// 들어온 퀘스트 인덱스로 퀘스트 정보를 받는다.

	if( !pQuest )	return NULL;																	// 퀘스트 정보가 유효하지 않으면, NULL을 리턴한다.

	//
	int Key = 0;																					// 키 값을 받을 변수를 선언하고 0으로 세팅한다.

	COMBINEKEY( dwQuestIdx, 0, Key );																// 퀘스트 인덱스와 0을 결합하여 키값을 받는다.

	CQuestString* pQString = m_QuestStringTable.GetData(Key);										// 키 값으로 퀘스트 스트링 정보를 받는다.

	if( pQString == NULL )	return NULL;															// 퀘스트 스트링 정보가 유효하지 않으면, NULL을 리턴한다.

	cPtrList* p = pQString->GetTitle();																// 퀘스트 스트링의 타이틀을 리스트 포인터로 받는다.

	QString* string = (QString*)p->GetHead();														// 리스트의 해드를 스트링 포인터로 받는다.

	return string->Str;																				// 스트링 포인터의 스트링을 리턴한다.
}

CQuestString* CQuestManager::GetQuestString(DWORD dwMainQuestIdx, DWORD dwSubQuestIdx)				// 퀘스트 번호와 서브 퀘 번호로 퀘스트 스트링을 반환하는 함수.
{
	int nCombineKey = 0 ;																			// 컴바인 키 값을 담을 변수를 선언하고 0으로 세팅한다.

	COMBINEKEY( dwMainQuestIdx, dwSubQuestIdx, nCombineKey) ;										// 메인퀘 번호와 서브퀘 번호로 컴바인 키를 만든다.

	CQuestString* pQuestString = m_QuestStringTable.GetData(nCombineKey) ;							// 컴바인 키로 퀘스트 스트링 테이블에서 퀘스트 스트링을 받는다.

	return pQuestString ;																			// 퀘스트 스트링을 반환한다.
}

void CQuestManager::QuestTimeLimit( DWORD dwQuestIdx, DWORD dwTime )								// 퀘스트의 시간을 제한하는 함수.
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );											// 들어온 인덱스로 퀘스트 정보를 받는다.

	if( !pQuest )	return;																			// 퀘스트 정보가 유효하지 않으면 리턴 처리를 한다.

	//
	stTIME time;																					// 시간 정보를 담을 구조체를 선언한다.
	time.value = dwTime;																			// 들어온 시간값을 세팅한다.

	int Key = 0;																					// 키 값을 받을 변수를 선언하고 0으로 세팅한다.

	COMBINEKEY( dwQuestIdx, 0, Key );																// 퀘스트 인덱스와 0을 결합하여 키 값을 받는다.

	CQuestString* pQString = m_QuestStringTable.GetData(Key);										// 받은 키 값을 참조해 퀘스트 스트링 정보를 받는다.

	if( pQString == NULL )	return;																	// 퀘스트 스트링 정보가 유효하지 않으면 리턴 처리를 한다.

	cPtrList* p = pQString->GetTitle();																// 스트링의 타이틀을 리스트 포인터로 받는다.

	QString* string = (QString*)p->GetHead();														// 리스트의 헤드를 스트링 포인터로 받는다.

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(299), string->Str,								// 시간 정보와 메시지를 결합하여, 채팅창으로 출력한다.
					 time.GetYear()+2000, time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute() );	

	char msg[256] = {0, };																			// 임시 버퍼를 선언한다.

	CompositString( msg, CHATMGR->GetChatMsg(299), string->Str,										// 시간 정보와 메시지를 결합하여, 출력할 메시지를 완성한다.
					 time.GetYear()+2000, time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute() );	

	STATUSICONDLG->RemoveQuestTimeIcon( HERO, eStatusIcon_QuestTime );								// 퀘스트의 시간제한 아이콘을 삭제한다.
}

void CQuestManager::RegistCheckTime( DWORD dwQuestIdx, BYTE bCheckType, DWORD dwCheckTime )			// 시간 체크 등록을 하는 함수.
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );											// 들어온 퀘스트 인덱스로 퀘스트 정보를 받는다.

	if( !pQuest )	return;																			// 퀘스트 정보가 유효하지 않으면 리턴 처리를 한다.

	pQuest->RegistCheckTime( bCheckType, dwCheckTime );												// 퀘스트 정보에 시간 체크 여부 정보를 세팅한다.

	STATUSICONDLG->AddQuestTimeIcon( HERO, eStatusIcon_QuestTime );									// 퀘스트 시간 아이콘을 추가한다.
}

void CQuestManager::UnRegistCheckTime( DWORD dwQuestIdx )											// 시간 체크 등록을 해제하는 함수.
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );											// 들어온 퀘스트 인덱스로 퀘스트 정보를 받는다.

	if( !pQuest )	return;																			// 퀘스트 정보가 유효하지 않으면 리턴 처리를 한다.

	pQuest->UnRegistCheckTime();																	// 퀘스트 정보에서 UnRegistCheckTime함수를 호출한다.

	STATUSICONDLG->RemoveQuestTimeIcon( HERO, eStatusIcon_QuestTime );								// 퀘스트 시간 아이콘을 삭제한다.
}

void CQuestManager::QuestEventAck( DWORD dwQuestIdx )												// 퀘스트 이벤트 요청 함수.
{
	WINDOWMGR->MsgBox( MBI_QUESTMSG, MBT_OK, "출석체크가 되었습니다." );							// 출석 체크가 되었다는 메시지를 출력한다.
}

void CQuestManager::QuestEventNack( DWORD dwQuestIdx, DWORD dwError )								// 퀘스트 이벤트 실패 함수.
{
	switch( dwError )																				// 에러를 확인한다.
	{
	case 1:																							// 에러가 1번과 같으면,
		{
			WINDOWMGR->MsgBox( MBI_QUESTMSG, MBT_OK, "이미 출석체크가 되어 협행을 진행할 수 없습니다." );	// 메시지를 출력한다.
		}
		break;
	case 2:																							// 에러가 2번과 같으면,
		{
			WINDOWMGR->MsgBox( MBI_QUESTMSG, MBT_OK, "홈페이지 출석체크가 되어 있지 않아 협행을 진행할 수 없습니다." );	// 메시지를 출력한다.
		}
		break;
	}
}
void CQuestManager::Quest_Maindata_Load( void* pMsg )												// 메인 데이터를 로딩하는 함수.
{
	SEND_MAINQUEST_DATA* pmsg = (SEND_MAINQUEST_DATA*)pMsg;											// 메인 퀘스트 데이터를 받는 포인터 변수를 선언하고 메시지를 받는다.

	if(pmsg->wCount==0)		return;																	// 카운트가 0과 같으면 리턴 처리를 한다.
	
	for(int i=0; i<pmsg->wCount; i++)																// 카운트 수 만큼 for문을 돌린다.
	{
		SetMainQuestData( pmsg->QuestList[i].QuestIdx, pmsg->QuestList[i].state.value,				// 메인 퀘스트 데이터를 세팅한다.
			pmsg->QuestList[i].EndParam, pmsg->QuestList[i].Time, pmsg->QuestList[i].CheckType, 
			pmsg->QuestList[i].CheckTime );
	}

	ProcessNpcMark();																				// npc 마크 프로세스를 실행한다.
}

//void CQuestManager::Quest_Maindata_Load( void* pMsg )												// 메인 데이터를 로딩하는 함수.
//{
//	SEND_MAINQUEST_DATA* pmsg = (SEND_MAINQUEST_DATA*)pMsg;											// 메인 퀘스트 데이터를 받는 포인터 변수를 선언하고 메시지를 받는다.
//
//	if(pmsg->wCount==0)		return;																	// 카운트가 0과 같으면 리턴 처리를 한다.
//	
//	for(int i=0; i<pmsg->wCount; i++)																// 카운트 수 만큼 for문을 돌린다.
//	{
//		SetMainQuestData( pmsg->QuestList[i].QuestIdx, pmsg->QuestList[i].state.value,				// 메인 퀘스트 데이터를 세팅한다.
//			pmsg->QuestList[i].EndParam, pmsg->QuestList[i].Time, pmsg->QuestList[i].CheckType, 
//			pmsg->QuestList[i].CheckTime );
//	}
//
//	//if( pmsg->wCount > 1 )
//	//{
//	//	CQuestQuickViewDialog* pQuickView = NULL ;														// 퀘스트 알림이 다이얼로그 정보를 받을 포인터를 선언하고 null처리 한다.
//	//	pQuickView = GAMEIN->GetQuestQuickViewDialog() ;												// 퀘스트 알림이 다이얼로그 정보를 받는다.
//
//	//	if( pQuickView )																				// 퀘스트 알림이 다이얼로그 정보가 유효하다면,
//	//	{
//	//		pQuickView->MainDataLoad() ;																// 퀘스트 알림이의 메인 퀘스트 데이터를 로딩한다.
//	//	}
//	//}
//	
//	ProcessNpcMark();																				// npc 마크 프로세스를 실행한다.
//}

void CQuestManager::Quest_Subdata_Load( void* pMsg )												// 서브 데이터를 로딩하는 함수.
{
	SEND_SUBQUEST_DATA* pmsg = (SEND_SUBQUEST_DATA*)pMsg;											// 서브 퀘스트 데이터를 받는 포인터 변수를 선언하고 메시지를 받는다.

	if(pmsg->wCount==0)		return;																	// 카운트가 0과 같으면 리턴 처리를 한다.
	
	for(int i=0; i<pmsg->wCount; i++)																// 카운트 수 만큼 for문을 돌린다.
	{
		SetSubQuestData( pmsg->QuestList[i].QuestIdx, pmsg->QuestList[i].SubQuestIdx,				// 서브 퀘스트 데이터를 세팅한다.
							pmsg->QuestList[i].state, pmsg->QuestList[i].time );
	}

	ProcessNpcMark();																				// npc 마크 프로세스를 실행한다.
}

void CQuestManager::Quest_Item_Load( void* pMsg )													// 퀘스트 아이템을 로딩하는 함수.
{
	SEND_QUESTITEM* pmsg = (SEND_QUESTITEM*)pMsg;													// 퀘스트 아이템 정보를 받는 포인터 변수를 선언하고 메시지를 받는다.

	if(pmsg->wCount==0)		return;																	// 카운트가 0과 같으면 리턴 처리를 한다.

	for(int i=0; i<pmsg->wCount; i++)																// 카운트 수만큼 for문을 돌린다.
	{			
		m_QuestUpdater->QuestItemAdd(m_QuestItemMGR.GetQuestItem(pmsg->ItemList[i].ItemIdx),		// 퀘스트 업데이터에 퀘스트 아이템을 추가한다.	
			pmsg->ItemList[i].Count);			
	}
}

void CQuestManager::Quest_Start_Ack( void* pMsg )													// 퀘스트 시작 수락 함수.
{
	SEND_QUEST_IDX* pmsg = (SEND_QUEST_IDX*)pMsg;													// 퀘스트 정보를 받을 포인터 변수를 선언하고 메시지를 받는다.

	StartQuest( pmsg->MainQuestIdx, pmsg->dwFlag, pmsg->dwData, pmsg->dwTime );						// 받은 정보를 참조로 퀘스트를 시작한다.
}

void CQuestManager::SubQuest_Start_Ack( void* pMsg )												// 서브 퀘스트 시작 수락 함수.
{
	SEND_QUEST_IDX* pmsg = (SEND_QUEST_IDX*)pMsg;													// 퀘스트 정보를 받을 포인터 변수를 선언하고 메시지를 받는다.

	StartSubQuest( pmsg->MainQuestIdx, pmsg->SubQuestIdx, pmsg->dwFlag, pmsg->dwData, pmsg->dwTime );// 서브 퀘스트를 시작한다.

	ProcessNpcMark();																				// npc 마크 프로세스를 시작한다.
}

//void CQuestManager::SubQuest_Start_Ack( void* pMsg )												// 서브 퀘스트 시작 수락 함수.
//{
//	SEND_QUEST_IDX* pmsg = (SEND_QUEST_IDX*)pMsg;													// 퀘스트 정보를 받을 포인터 변수를 선언하고 메시지를 받는다.
//
//	StartSubQuest( pmsg->MainQuestIdx, pmsg->SubQuestIdx, pmsg->dwFlag, pmsg->dwData, pmsg->dwTime );// 서브 퀘스트를 시작한다.
//
//	ProcessNpcMark();																				// npc 마크 프로세스를 시작한다.
//
//	//CQuestQuickViewDialog* pDlg = NULL ;															// 퀘스트 알림이 다이얼로그 정보를 받을 포인터를 선언하고 null처리 한다.
//	//pDlg = GAMEIN->GetQuestQuickViewDialog() ;														// 퀘스트 알림이 다이얼로그 정보를 받는다.
//
//	//if( pDlg )																						// 퀘스트 알림이 다이얼로그 정보가 유효하면,
//	//{
//	//	pDlg->StartSubQuest( pmsg ) ;																// 퀘스트 알림이 다이얼로그를 업데이트한다.
//	//}
//
//	CQuestDialog* pQuestDialog = GAMEIN->GetQuestDialog() ;
//
//	if( pQuestDialog )
//	{
//		CQuestInfo* pQuestInfo = NULL ;												// 퀘스트 정보 포인터를 선언하고 null처리를 한다.
//		pQuestInfo = QUESTMGR->GetQuestInfo(pmsg->MainQuestIdx) ;							// 퀘스트 매니져로 부터, 인자로 넘어온 퀘스트 번호로 퀘스트 정보를 받는다.
//
//		int nSubQuestCount = 0 ;													// 서브 퀘스트 카운트를 받을 변수를 선언하고 0으로 세팅한다.
//		nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// 퀘스트 정보로 부터 서브 퀘스트 카운트를 받는다.
//
//		if( pmsg->SubQuestIdx == nSubQuestCount-1 )
//		{
//			pQuestDialog->UpdateQuestList(pmsg->MainQuestIdx, pmsg->SubQuestIdx) ;
//		}
//	}
//
//}

void CQuestManager::Quest_End_Ack( void* pMsg )														// 퀘스트 종료 수락 함수.
{
	SEND_QUEST_IDX* msg = (SEND_QUEST_IDX*)pMsg;													// 퀘스트 정보를 받을 포인터를 선언하고, 메시지를 받는다.
		
	EndQuest( msg->MainQuestIdx, msg->dwFlag, msg->dwData, msg->dwTime );							// 퀘스트를 종료한다.

	QuestAbandonClose();																			// 퀘스트 포기 닫기 함수를 실행한다.

	ProcessNpcMark();	
}

//void CQuestManager::Quest_End_Ack( void* pMsg )														// 퀘스트 종료 수락 함수.
//{
//	SEND_QUEST_IDX* msg = (SEND_QUEST_IDX*)pMsg;													// 퀘스트 정보를 받을 포인터를 선언하고, 메시지를 받는다.
//		
//	EndQuest( msg->MainQuestIdx, msg->dwFlag, msg->dwData, msg->dwTime );							// 퀘스트를 종료한다.
//
//	QuestAbandonClose();																			// 퀘스트 포기 닫기 함수를 실행한다.
//
//	ProcessNpcMark();																				// npc 마크 프로세스를 실행한다.
//
//	CQuestInfo* pQuestInfo = NULL ;												// 퀘스트 정보 포인터를 선언하고 null처리를 한다.
//	pQuestInfo = QUESTMGR->GetQuestInfo(msg->MainQuestIdx) ;							// 퀘스트 매니져로 부터, 인자로 넘어온 퀘스트 번호로 퀘스트 정보를 받는다.
//
//	int nSubQuestCount = 0 ;													// 서브 퀘스트 카운트를 받을 변수를 선언하고 0으로 세팅한다.
//	nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// 퀘스트 정보로 부터 서브 퀘스트 카운트를 받는다.
//
//	for( int count = 0 ; count < nSubQuestCount ; ++count )
//	{
//		CQuestString* pString = QUESTMGR->GetQuestString(msg->MainQuestIdx, count) ;
//
//		if( pString )
//		{
//			pString->SetComplete(FALSE) ;
//
//			pString->SetCount(0) ;
//			pString->SetTotalCount(0) ;
//		}
//	}
//
//	CQuestQuickViewDialog* pDlg = NULL ;															// 퀘스트 알림이 다이얼로그 정보를 받을 포인터를 선언하고 null처리 한다.
//	pDlg = GAMEIN->GetQuestQuickViewDialog() ;														// 퀘스트 알림이 다이얼로그 정보를 받는다.
//
//	if( pDlg )																						// 퀘스트 알림이 다이얼로그 정보가 유효하면,
//	{
//		pDlg->DeleteQuest(msg->dwObjectID) ;														// 퀘스트 알림이 다이얼로그를 업데이트한다.
//	}
//
//	//CQuestQuickViewDialog* pDlg = NULL ;															// 퀘스트 알림이 다이얼로그 정보를 받을 포인터를 선언하고 null처리 한다.
//	//pDlg = GAMEIN->GetQuestQuickViewDialog() ;														// 퀘스트 알림이 다이얼로그 정보를 받는다.
//
//	//if( pDlg )																						// 퀘스트 알림이 다이얼로그 정보가 유효하면,
//	//{
//	//	pDlg->EndQuest( msg ) ;																		// 퀘스트 알림이 다이얼로그를 업데이트한다.
//	//}
//
//	//CQuestQuickViewDialog* pDlg = NULL ;															// 퀘스트 알림이 다이얼로그 정보를 받을 포인터를 선언하고 null처리 한다.
//	//pDlg = GAMEIN->GetQuestQuickViewDialog() ;														// 퀘스트 알림이 다이얼로그 정보를 받는다.
//
//	//if( pDlg )																						// 퀘스트 알림이 다이얼로그 정보가 유효하면,
//	//{
//	//	pDlg->EndQuest( msg ) ;																		// 퀘스트 알림이 다이얼로그를 업데이트한다.
//
//	//	for( int count = 0 ; count < MAX_QUICKVIEW_QUEST ; ++count )								// 퀘스트 알림이 다이얼로그에 등록 가능한 최대 수 만큼 for문을 돌린다.
//	//	{
//	//		if(m_QuickViewQuestID[count] != 0)														// 퀘스트 인덱스가 0이 아니라면,
//	//		{
//	//			if( m_QuickViewQuestID[count] == msg->MainQuestIdx )								// 종료하려는 퀘스트와 인덱스가 같으면,
//	//			{
//	//				DeleteQuestIDToQuickView(count) ;												// 저장하고 있던 퀘스트 인덱스를 삭제한다.
//	//			}
//	//		}
//	//	}
//	//}
//
//	CQuestDialog* pQuestDialog = GAMEIN->GetQuestDialog() ;
//
//	if( pQuestDialog )
//	{
//		pQuestDialog->QuestListReset() ;
//		pQuestDialog->QuestDescView(pQuestDialog->QuestTitleString(1)) ;
//		pQuestDialog->GetQuestDescList()->ResetGuageBarPos() ;
//	}
//}

void CQuestManager::SubQuest_End_Ack( void* pMsg )													// 서브 퀘스트 종료 수락 함수.
{
	SEND_QUEST_IDX* msg = (SEND_QUEST_IDX*)pMsg;													// 퀘스트 정보를 받을 포인터를 선언하고, 메시지를 받는다.

	EndSubQuest( msg->MainQuestIdx, msg->SubQuestIdx, msg->dwFlag, msg->dwData, msg->dwTime );		// 서부 퀘스트를 종료한다.

	QuestAbandonClose();																			// 퀘스트 포기 닫기 함수를 실행한다.

	ProcessNpcMark();																				// npc 마크 프로세스를 실행한다.

	GAMENOTIFYMGR->EndSubQuest(msg) ;																// 서브 퀘스트 종료 공지를 추가한다.
}

//void CQuestManager::SubQuest_End_Ack( void* pMsg )													// 서브 퀘스트 종료 수락 함수.
//{
//	SEND_QUEST_IDX* msg = (SEND_QUEST_IDX*)pMsg;													// 퀘스트 정보를 받을 포인터를 선언하고, 메시지를 받는다.
//
//	EndSubQuest( msg->MainQuestIdx, msg->SubQuestIdx, msg->dwFlag, msg->dwData, msg->dwTime );		// 서부 퀘스트를 종료한다.
//
//	QuestAbandonClose();																			// 퀘스트 포기 닫기 함수를 실행한다.
//
//	ProcessNpcMark();																				// npc 마크 프로세스를 실행한다.
//
//	//CQuestQuickViewDialog* pDlg = NULL ;															// 퀘스트 알림이 다이얼로그 정보를 받을 포인터를 선언하고 null처리 한다.
//	//pDlg = GAMEIN->GetQuestQuickViewDialog() ;														// 퀘스트 알림이 다이얼로그 정보를 받는다.
//
//	//if( pDlg )																						// 퀘스트 알림이 다이얼로그 정보가 유효하면,
//	//{
//	//	pDlg->EndSubQuest( msg ) ;																	// 퀘스트 알림이 다이얼로그를 업데이트한다.
//	//}
//
//	CQuestDialog* pQuestDlg = GAMEIN->GetQuestDialog() ;
//
//	if( pQuestDlg )
//	{
//		pQuestDlg->QuestListReset() ;
//		pQuestDlg->UpdateQuestDescView(msg->MainQuestIdx, msg->SubQuestIdx) ;
//	}
//
//	GAMENOTIFYMGR->EndSubQuest(msg) ;																// 서브 퀘스트 종료 공지를 추가한다.
//}

void CQuestManager::SubQuest_Update( void* pMsg )													// 서브 퀘스트 업데이트 함수.
{
	SEND_SUBQUEST_UPDATE* msg = (SEND_SUBQUEST_UPDATE*)pMsg;										// 서브 퀘스트 업데이트 정보를 받을 포인터를 선언하고 메시지를 받는다.

	UpdateSubQuestData( msg->wQuestIdx, msg->wSubQuestIdx, msg->dwMaxCount, msg->dwData, msg->dwTime );	// 서브 퀘스트 데이터를 업데이트 한다.

	GAMENOTIFYMGR->UpdateSubQuest(msg) ;															// 퀘스트 업데이트 공지를 추가한다.
}	

//void CQuestManager::SubQuest_Update( void* pMsg )													// 서브 퀘스트 업데이트 함수.
//{
//	SEND_SUBQUEST_UPDATE* msg = (SEND_SUBQUEST_UPDATE*)pMsg;										// 서브 퀘스트 업데이트 정보를 받을 포인터를 선언하고 메시지를 받는다.
//
//	UpdateSubQuestData( msg->wQuestIdx, msg->wSubQuestIdx, msg->dwMaxCount, msg->dwData, msg->dwTime );	// 서브 퀘스트 데이터를 업데이트 한다.
//
//	//CQuestQuickViewDialog* pDlg = NULL ;															// 퀘스트 알림이 다이얼로그 정보를 받을 포인터를 선언하고 null처리 한다.
//	//pDlg = GAMEIN->GetQuestQuickViewDialog() ;														// 퀘스트 알림이 다이얼로그 정보를 받는다.
//
//	//if( pDlg )																						// 퀘스트 알림이 다이얼로그 정보가 유효하면,
//	//{
//	//	pDlg->UpdateQuest( msg ) ;																	// 퀘스트 알림이 다이얼로그를 업데이트한다.
//	//}
//
//	CQuestString* pUpdateString = NULL ;													// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
//	pUpdateString = QUESTMGR->GetQuestString(msg->wQuestIdx, msg->wSubQuestIdx) ;			// 퀘스트 인덱스와, 서브 퀘스트 인덱스로 퀘스트 스트링을 받는다.
//
//	if( pUpdateString )
//	{
//		pUpdateString->SetCount(msg->dwData) ;
//		pUpdateString->SetTotalCount(msg->dwMaxCount) ;
//	}
//
//
//	GAMENOTIFYMGR->UpdateSubQuest(msg) ;															// 퀘스트 업데이트 공지를 추가한다.
//}	

void CQuestManager::QuestItem_Give( void* pMsg )													// 퀘슽 아이템 반환 함수.
{
	SEND_QUESTITEM_IDX* pmsg = (SEND_QUESTITEM_IDX*)pMsg;											// 퀘스트 아이템 정보를 받을 포인터를 선언하고 메시지를 받는다.

	QuestItemDelete( pmsg->dwItemIdx );																// 퀘스트 아이템을 삭제한다.
}

void CQuestManager::QuestItem_Take( void* pMsg )													// 퀘스트 아이템 수락 함수.
{
	SEND_QUESTITEM_IDX* pmsg = (SEND_QUESTITEM_IDX*)pMsg;											// 퀘스트 아이템 정보를 받을 포인터를 선언하고 메시지를 받는다.

	QuestItemAdd( pmsg->dwItemIdx, pmsg->dwItemNum );												// 퀘스트 아이템을 추가한다.

	EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Quest);										// 아이템을 수락하는 이펙트를 시작한다.

//			ITEMMGR->ItemDropEffect( pmsg->dwItemIdx );
}

void CQuestManager::QuestItem_Update( void* pMsg )													// 퀘스트 아이템 업데이트 함수.
{
	SEND_QUESTITEM_IDX* pmsg = (SEND_QUESTITEM_IDX*)pMsg;											// 퀘스트 아이템 정보를 받을 포인터를 선언하고 메시지를 받는다.
		
	QuestItemUpdate( eQItem_SetCount, pmsg->dwItemIdx, pmsg->dwItemNum );							// 퀘스트 아이템을 업데이트 한다.

	EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Quest);										// 아이템을 수락하는 이펙트를 시작한다.
//			ITEMMGR->ItemDropEffect( pmsg->dwItemIdx );
}

void CQuestManager::Quest_Delete_Ack( void* pMsg )													// 퀘스트 삭제를 수락하는 함수.
{
	SEND_QUEST_IDX* pmsg = (SEND_QUEST_IDX*)pMsg;													// 퀘스트 정보를 담을 포인터를 선언하고 메시지를 받는다.

	DeleteQuest( pmsg->MainQuestIdx, (BOOL)pmsg->dwFlag );											// 퀘스트를 삭제한다.

	ProcessNpcMark();																				// npc 마크 프로세스를 실행한다.
}

//void CQuestManager::Quest_Delete_Ack( void* pMsg )													// 퀘스트 삭제를 수락하는 함수.
//{
//	SEND_QUEST_IDX* pmsg = (SEND_QUEST_IDX*)pMsg;													// 퀘스트 정보를 담을 포인터를 선언하고 메시지를 받는다.
//
//	DeleteQuest( pmsg->MainQuestIdx, (BOOL)pmsg->dwFlag );											// 퀘스트를 삭제한다.
//
//	ProcessNpcMark();																				// npc 마크 프로세스를 실행한다.
//
//	CQuestInfo* pQuestInfo = NULL ;												// 퀘스트 정보 포인터를 선언하고 null처리를 한다.
//	pQuestInfo = QUESTMGR->GetQuestInfo(pmsg->MainQuestIdx) ;							// 퀘스트 매니져로 부터, 인자로 넘어온 퀘스트 번호로 퀘스트 정보를 받는다.
//
//	int nSubQuestCount = 0 ;													// 서브 퀘스트 카운트를 받을 변수를 선언하고 0으로 세팅한다.
//	nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// 퀘스트 정보로 부터 서브 퀘스트 카운트를 받는다.
//
//	for( int count = 0 ; count < nSubQuestCount ; ++count )
//	{
//		CQuestString* pString = QUESTMGR->GetQuestString(pmsg->MainQuestIdx, count) ;
//
//		if( pString )
//		{
//			pString->SetComplete(FALSE) ;
//
//			pString->SetCount(0) ;
//			pString->SetTotalCount(0) ;
//		}
//	}
//
//	CQuestDialog* pQuestDialog = GAMEIN->GetQuestDialog() ;
//
//	if( pQuestDialog )
//	{
//		pQuestDialog->QuestListReset() ;
//		pQuestDialog->QuestDescView(pQuestDialog->QuestTitleString(1)) ;
//		pQuestDialog->GetQuestDescList()->ResetGuageBarPos() ;
//	}
//
//	CQuestQuickViewDialog* pQuickView = NULL ;														// 퀘스트 알림이 다이얼로그 정보를 받을 포인터를 선언하고 null처리 한다.
//	pQuickView = GAMEIN->GetQuestQuickViewDialog() ;												// 퀘스트 알림이 다이얼로그 정보를 받는다.
//
//	if( pQuickView )																				// 퀘스트 알림이 다이얼로그 정보가 유효하다면,
//	{
//		pQuickView->DeleteQuest(pmsg->MainQuestIdx) ;												// 퀘스트 알림이의 메인 퀘스트 데이터를 로딩한다.
//	}
//}

void CQuestManager::Quest_Delete_Confirm_Ack( void* pMsg )											// 퀘스트 삭제 컴펌 요청 함수.
{
	SEND_QUEST_IDX_SIMPLE* pmsg = (SEND_QUEST_IDX_SIMPLE*)pMsg;										// 퀘슽 정보를 받을 포인터를 선언하고, 메시지를 받는다.
			
	m_QuestUpdater->DeleteQuest(pmsg->MainQuestIdx);												// 퀘스트 업데이터의 퀘스트 삭제함수를 실행한다.
}

void CQuestManager::Quest_TakeItem_Ack( void* pMsg )												// 퀘스트 아이템 획득 수락 함수.
{
	ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;												// 아이템 획득 정보를 담을 포인터를 선언하고 메시지를 받는다.

	CItem* pItem = NULL;																			// 아이템 정보를 담을 포인터를 선언하고 null처리를 한다.

	for( int i = 0; i < pmsg->ItemNum; ++i )														// 아이템 수만큼 for문을 돌린다.
	{
		if( pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->GetItem(i)->Position ) )		// 아이템 포지션으로 아이템 정보를 받아 정보가 유효한지 체크한다.
		{
			pItem->SetItemBaseInfo( *( pmsg->GetItem(i) ) );												// 아이템 기본 정보를 세팅한다.
		}
		else																						// 아이템 정보가 유효하지 않으면,
		{
			pItem = ITEMMGR->MakeNewItem(pmsg->GetItem(i),"MP_QUEST_TAKEITEM_ACK");					// 새로 아이템을 생성해 포인터로 받는다.
				
			BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);									// 인벤토리에 아이템을 추가하고 결과를 받는다.

			if(!rt)																					// 실패 했다면,
			{
				ASSERT(0);																			// assert 처리.
			}
		}
	}
	// 
	cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_DIVIDE_INV );									// 디바이드 박스 정보를 받는다.

	if( pDlg )																						// 정보가 유효한지 체크한다.
	{
		((cDivideBox*)pDlg)->ExcuteDBFunc( 0 );														// 박스의 함수를 실행한다.
	}

	QUICKMGR->RefreshQickItem();																	// 퀵 매니져의 퀵 아이템 새로고침을 실행한다.

	if( !pItem ) return;																			// 아이템 정보가 유효하지 않으면 리턴 처리를 한다.

	{
		CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(235), pItem->GetItemInfo()->ItemName );		// 퀘스트 보상으로 아이템을 얻었다는 메시지를 출력한다.
	}

	EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Quest);										// 아이템 획득 이펙트를 실행한다.

	//GAMENOTIFYMGR->TakeItem(pItem) ;																// 퀘스트 아이템 획득 공지를 띄운다.
}

void CQuestManager::Quest_Takemoney_Ack( void* pMsg )												// 퀘스트 머니 획득 수락 함수.
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;																// 메시지를 받을 포인터를 선언하고 메시지를 받는다.

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(236), AddComma(pmsg->dwData) );					// 골드를 얻었다는 메시지를 출력한다.
	
	EFFECTMGR->StartHeroEffectProcess(eEffect_GetMoney);											// 아이템을 얻었다는 이펙트를 실행한다.

	//GAMENOTIFYMGR->TakeMoney(AddComma(pmsg->dwData)) ;												// 퀘스트 보상 골드 획득 공지를 띄운다.
}

void CQuestManager::Quest_TakeExp_Ack( void* pMsg )													// 퀘스트 경험치를 획득 수락 함수.
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;																// 메시지를 받을 포인터를 선언하고 메시지를 받는다.

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(237), pmsg->dwData );							// 경험치를 얻었다는 메시지를 출력한다.

	//GAMENOTIFYMGR->TakeExp(pmsg->dwData) ;															// 퀘스트 보상 경험치 획득 공지를 띄운다.
	
//	HERO->SetExpPoint(HERO->GetExpPoint() + pmsg->dwData) ;											// HERO의 경험치를 세팅한다.
}

void CQuestManager::Quest_TakesExp_Ack( void* pMsg )												// 퀘스트 sp 획득 수락 함수.
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;																// 메시지를 받을 포인터를 선언하고 메시지를 받는다.

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(238), pmsg->dwData );							// sp를 얻었다는 메시지를 출력한다.

	HERO->SetExpPoint(HERO->GetExpPoint() + pmsg->dwData) ;											// HERO의 경험치를 세팅한다.
}

void CQuestManager::Quest_GiveItem_Ack( void* pMsg )												// 퀘스트 아이템을 반환하는 함수.
{
	MSG_EXCHANGE_REMOVEITEM* pmsg = (MSG_EXCHANGE_REMOVEITEM*)pMsg;									// 교환 삭제 아이템 정보를 받는 포인터를 선언하고 메시지를 받는다.

	CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->wAbsPosition );				// 아이템 위치로 아이템 정보를 받는다.

	WORD wTableIdx = ITEMMGR->GetTableIdxForAbsPos( pmsg->wAbsPosition );							// 텝 인덱스와 위치 정보로 테이블 인덱스를 받는다.

	const ITEMBASE* pItemInfo = ITEMMGR->GetItemInfoAbsIn( HERO, pmsg->wAbsPosition );				// 아이템 기본 정보를 받는다.

	ITEMMGR->DeleteItemofTable( wTableIdx, pmsg->wAbsPosition, FALSE );								// 아이템 매니져로 테이블의 아이템을 삭제한다.

	QUICKMGR->RefreshQickItem();																	// 퀵 매니져의 퀵 아이템 새로고침을 실행한다.

	if( !pItem ) return;																			// 아이템 정보가 유효하지 않으면 리턴 처리를 한다.

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(297), pItem->GetItemInfo()->ItemName );			// 아이템이 사라졌다는 메시지를 출력한다.
}

// 071026 LYW --- QuestManager : Add function to decrease item.
void CQuestManager::Quest_GiveItems_Ack( void* pMsg )
{
	MSG_EXCHANGE_DECREASEITEM* pmsg = (MSG_EXCHANGE_DECREASEITEM*)pMsg;								// 교환 삭제 아이템 정보를 받는 포인터를 선언하고 메시지를 받는다.

	CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->wAbsPosition );				// 아이템 위치로 아이템 정보를 받는다.

	WORD wTableIdx = ITEMMGR->GetTableIdxForAbsPos( pmsg->wAbsPosition );							// 텝 인덱스와 위치 정보로 테이블 인덱스를 받는다.

	const ITEMBASE* pItemInfo = ITEMMGR->GetItemInfoAbsIn( HERO, pmsg->wAbsPosition );				// 아이템 기본 정보를 받는다.

	//pItemInfo->Durability = pmsg->nItemCount ;

	pItem->SetDurability(pmsg->nItemCount) ;

	QUICKMGR->RefreshQickItem();																	// 퀵 매니져의 퀵 아이템 새로고침을 실행한다.

	if( !pItem ) return;																			// 아이템 정보가 유효하지 않으면 리턴 처리를 한다.

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(297), pItem->GetItemInfo()->ItemName );			// 아이템이 사라졌다는 메시지를 출력한다.
}

// 071028 LYW --- CQuestManager : Add function to process give item error.
void CQuestManager::Quest_GiveItem_Error( void* pMsg )						// 퀘스트 아이템 반납에 실패한 에러 처리를 하는 함수.
{
	ASSERT(pMsg);

	//CHATMGR->AddMsg( CTC_QUEST, "퀘스트 아이템 반납에 실패 했습니다." );	// 퀘스트 아이템 반납에 실패 했다는 에러 메시지를 띄운다.
	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg( 1285 ) );

	MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg ;									// 에러 처리에 맞는 데이터 형번환을 한다.

	sUSERINFO_ERR_GIVEITEM EGI ;											// 에러 내용을 담을 구조체를 선언한다.
	memset(&EGI, 0, sizeof(sUSERINFO_ERR_GIVEITEM)) ;						// 구조체를 초기화 한다.

	if( HERO == NULL ) return ;												// 히어로 정보가 없으면 리턴 처리를 한다.

	strcpy(EGI.UserName, HERO->GetObjectName()) ;							// 유저 캐릭터의 이름을 복사한다.
	EGI.bCaption = (BYTE)pmsg->dwData1 ;									// 에러 발생 함수를 세팅한다.
	EGI.bErrNum	 = (BYTE)pmsg->dwData2 ;									// 에러 번호를 세팅한다.
	EGI.dwQuestIdx = pmsg->dwData3 ;										// 퀘스트 인덱스를 세팅한다.
	EGI.dwItemIdx  = pmsg->dwData4 ;										// 아이템 인덱스를 세팅한다.

	char strFilePath[MAX_PATH] = {0, } ;									// 파일을 기록할 경로를 담을 버퍼를 선언하고 초기화 한다.

	wsprintf( strFilePath, "%s\\data\\interface\\log\\%d.EGI", 
		DIRECTORYMGR->GetFullDirectoryName(eLM_Root),HERO->GetID()); // 경로와 파일명을 세팅한다.

	HANDLE hFile = CreateFile( strFilePath, GENERIC_WRITE, 0, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ) ;						// 파일을 생성한다.

	if( hFile == INVALID_HANDLE_VALUE )										// 결과가 실패 했으면,
	{
		return ;															// return.
	}

	DWORD dwSize ;															// 사이즈를 담을 변수를 선언한다.

	WriteFile( hFile, &EGI, sizeof(EGI), &dwSize, NULL ) ;					// 에러 내용을 기록한다.

	CloseHandle( hFile ) ;
}

void CQuestManager::Quest_GiveMoney_Ack( void* pMsg )												// 머니를 주는 함수.
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;																// 메시지를 받을 포인터를 선언하고 메시지를 받는다.

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(236), AddComma(pmsg->dwData) );					// 퀘스트 보상으로 골드를 얻었다는 메시지를 출력한다.
}

void CQuestManager::Quest_Time_Limit( void* pMsg )													// 퀘스트 시간 제한 함수.
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;															// 메시지를 받을 포인터를 선언하고 메시지를 받는다.

	QuestTimeLimit( pmsg->dwData1, pmsg->dwData2 );													// 들어온 정보로 퀘스트 시간 제한을 한다.
}

void CQuestManager::Quest_Regist_Checktime( void* pMsg )											// 시간 체크 등록을 하는 함수.
{
	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;															// 메시지를 받을 포인터를 선언하고 메시지를 받는다.

	RegistCheckTime( pmsg->dwData1, (BYTE)pmsg->dwData2, pmsg->dwData3 );							// 시간 체크를 등록한다.
}

void CQuestManager::Quest_Unregist_Checktime( void* pMsg )											// 시간 체크 등록을 취소하는 함수.
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;																// 메시지를 받을 포인터를 선언하고 메시지를 받는다.

	UnRegistCheckTime( pmsg->dwData );																// 시간 체크를 취소한다.
}

void CQuestManager::QuestEvent_NpcTalk_Ack( void* pMsg )											// npc 대화 요청 이벤트 함수.
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;															// 메시지를 받을 포인터를 선언하고 메시지를 받는다.

	QuestEventAck( pmsg->dwData1 );																	// 퀘스트 이벤트를 수락한다.
}

void CQuestManager::QuestEvent_NpcTalk_Nack( void* pMsg )											// npc 대화요청 실패 이벤트 함수.
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;															// 메시지를 받을 포인터를 선언하고 메시지를 받는다.

	QuestEventNack( pmsg->dwData1, pmsg->dwData2 );													// 퀘스트 이벤트 실패 처리를 한다.
}

void CQuestManager::Quest_Execute_Error( void* pMsg )												// 퀘스트 실행 에러 처리 함수.
{
	MSG_INT* pmsg = (MSG_INT*)pMsg;																	// 메시지를 받을 포인터를 선언하고 메시지를 받는다.

	int nMsgNum = 0 ;																				// 에러 메시지 번호를 세팅할 변수를 선언하고 0으로 세팅한다.

	switch(pmsg->nData)																				// 메시지로 넘어온 에러 코드를 확인한다.
	{
	case e_EXC_ERROR_NO_ERROR :					nMsgNum = 990 ;		break ;							// 에러 코드에 맞는 메시지 번호를 지정한다.
	case e_EXC_ERROR_NO_PLAYERINFO :			nMsgNum = 991 ;		break ;
	case e_EXC_ERROR_NO_QUESTGROUP :			nMsgNum = 992 ;		break ;
	case e_EXC_ERROR_NO_QUEST :					nMsgNum = 993 ;		break ;
	case e_EXC_ERROR_NO_INVENTORYINFO :			nMsgNum = 994 ;		break ;
	case e_EXC_ERROR_NO_EXTRASLOT :				nMsgNum = 995 ;		break ;
	case e_EXC_ERROR_NO_EXTRASLOTQUEST :		nMsgNum = 996 ;		break ;
	case e_EXC_ERROR_NO_QUESTITEM :				nMsgNum = 997 ;		break ;
	case e_EXC_ERROR_NOT_ENOUGH_QUESTITEM :		nMsgNum = 998 ;		break ;
	case e_EXC_ERROR_NO_GIVEITEM :				nMsgNum = 999 ;		break ;
	case e_EXC_ERROR_NOT_ENOUGH_GIVEITEM :		nMsgNum = 1000 ;	break ;
	case e_EXC_ERROR_NOT_ENOUGH_GIVEMONEY :		nMsgNum = 1001 ;	break ;
	//case e_EXC_ERROR_NOT_SAME_WEAPONKIND :		nMsgNum = 1002 ;	break ;
	// 071223 LYW --- QuestManager : 퀘스트 에러 메시지 출력을 알림창에서 채팅창 출력으로 바꾼다.
	case e_EXC_ERROR_NOT_SAME_WEAPONKIND :
		{
			CHATMGR->AddMsg( CTC_CHEAT_1, CHATMGR->GetChatMsg(1002) ) ;
			return ;
		}
		break ;
	case e_EXC_ERROR_NOT_SAME_WEAPONKIND_COUNT :nMsgNum = 1003 ;	break ;
	case e_EXC_ERROR_CANT_RECEIVE_MONEY :		nMsgNum = 1004 ;	break ;
	}

	GAMEIN->GetNpcScriptDialog()->EndDialog();														// npc 스크립트 다이얼로그로 end 다이얼로그를 호출한다.
	GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);;												// npc 스크립트 다이얼로그를 비활성화 한다.
	WINDOWMGR->MsgBox( MBI_QUESTMSG, MBT_OK, CHATMGR->GetChatMsg(nMsgNum) );						// 소지품에 빈 공간이 없어 진행 불가라는 메시지를 출력한다.
}

void CQuestManager::Quest_Error_Ext( void* pMsg )													// 퀘스트 기타 에러 처리 함수.
{
	if( !pMsg ) return ;																			// 메시지 정보가 유효하지 않으면, 리턴처리한다.

	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg ;															// 메시지를 받을 포인터를 선언하고 메시지를 받는다.

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pmsg->dwData);										// 메시지로 넘어온 아이템 인덱스로 아이템 정보를 받는다.

	if( !pItemInfo ) return ;																		// 아이템 정보가 유요하지 않으면, return 처리 한다.

	char tempBuf[128] = {0,} ;																		// 메시지 출력을 위한 임시 버퍼를 선언한다.
	//sprintf(tempBuf, "[%s] 아이템의 개수가 부족하여 퀘스트를 완료할 수 없습니다.", pItemInfo->ItemName) ;	// 에러 메시지를 조합한다.
	sprintf(tempBuf, CHATMGR->GetChatMsg(986), pItemInfo->ItemName) ;	// 에러 메시지를 조합한다.

	GAMEIN->GetNpcScriptDialog()->EndDialog();														// npc 스크립트 다이얼로그로 end 다이얼로그를 호출한다.
	GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);;												// npc 스크립트 다이얼로그를 비활성화 한다.
	WINDOWMGR->MsgBox( MBI_QUESTMSG, MBT_OK, tempBuf );												// 소지품에 빈 공간이 없어 진행 불가라는 메시지를 출력한다.
}

void CQuestManager::Quest_Error_FullQuest( void* pMsg )
{
	if( !pMsg ) return ;

	MSGBASE* pmsg = (MSGBASE*)pMsg ;

	char tempBuf[128] = {0,} ;																		// 메시지 출력을 위한 임시 버퍼를 선언한다.
	//sprintf(tempBuf, "[%s] 아이템의 개수가 부족하여 퀘스트를 완료할 수 없습니다.", pItemInfo->ItemName) ;	// 에러 메시지를 조합한다.
	//sprintf(tempBuf, "퀘스트 목록이 가득 차서 더이상 퀘스트를 진행할 수 없습니다.") ;	// 에러 메시지를 조합한다.
	sprintf(tempBuf, CHATMGR->GetChatMsg(1474)) ;

	GAMEIN->GetNpcScriptDialog()->EndDialog();														// npc 스크립트 다이얼로그로 end 다이얼로그를 호출한다.
	GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);;												// npc 스크립트 다이얼로그를 비활성화 한다.
	WINDOWMGR->MsgBox( MBI_QUESTMSG, MBT_OK, tempBuf );												// 소지품에 빈 공간이 없어 진행 불가라는 메시지를 출력한다.
}

// 070329 LYW --- QuestManager : Add function to make string.
void CQuestManager::CompositString( char* inputStr, char* str, ... )								// 출력용 메시지를 완성하는 함수.
{
	if( inputStr == NULL ) return ;																	// 들어온 스트링 정보가 유효하지 않으면 리턴 처리를 한다.
	if( str == NULL ) return;																		// 스트링 정보가 유효하지 않으면 리턴 처리를 한다.

	va_list argList;																				// 가변 인자 변수를 선언한다.

	va_start(argList, str);																			// 가변 인자 해석을 시작한다.
	vsprintf(inputStr,str,argList);																	// 메시지를 완성한다.
	va_end(argList);																				// 가변 인자 사용 완료.
}


//void CQuestManager::RegistQuestIDToQuickView(int idx, DWORD dwQuestId)								// 퀘스트 알림이에 등록 되는 퀘스트 아이디를 저장하는 함수.
//{
//	if( idx >= MAX_QUICKVIEW_QUEST )																// 인자로 넘어온 인덱스가 최대 등록 가능 퀘스트 수를 넘으면,
//	{
//		return  ;																					// 리턴 처리를 한다.
//	}
//
//	m_QuickViewQuestID[idx] = dwQuestId ;															// 인덱스에 해당하는 배열에 퀘스트 아이디를 세팅한다.
//}

//void CQuestManager::DeleteQuestIDToQuickView(int idx)												// 퀘스트 알림이에 등록 된 퀘스트 아이디를 삭제하는 함수.
//{
//	if( idx >= MAX_QUICKVIEW_QUEST )																// 인자로 넘어온 인덱스가 최대 등록 가능 퀘스트 수를 넘으면,
//	{
//		return  ;																					// 리턴 처리를 한다.
//	}
//
//	m_QuickViewQuestID[idx] = 0 ;																	// 인덱스에 해당하는 배열에 퀘스트 아이디를 지정 가능 최대 값으로 세팅한다.
//}

void CQuestManager::SaveQuestInfoFromQuickView(sQUEST_INFO* pInfo)									// 퀘스트 알림이에 등록 된 퀘스트 아이디를 반환하는 함수.
{
	//CQuestQuickViewDialog* pQuickViewDlg = NULL ;
	//pQuickViewDlg = GAMEIN->GetQuestQuickViewDialog() ;

	//if( !pQuickViewDlg )
	//{
	//	return ;
	//}

	//cPtrList* pList = NULL ;
	//pList = pQuickViewDlg->GetViewList() ;

	//if( !pList )
	//{
	//	return ;
	//}

	//QuestTree* pTree ;																// 트리 정보를 받을 포인터를 선언한다.
	//CQuestString* pQuestString ;													// 스트링 정보를 받을 포인터를 선언한다.

	//PTRLISTPOS treePos ;
	//PTRLISTPOS pos = NULL ;
	//pos = pList->GetHeadPosition() ;

	//int nCount = 0 ;
	//while(pos)
	//{
	//	pTree = NULL ;																// 트리 정보를 받을 포인터를 null 처리를 한다.
	//	pTree = (QuestTree*)pList->GetNext(pos) ;									// 위치 정보에 해당하는 트리를 받는다.

	//	if( !pTree )																// 트리 정보가 유효하다면,
	//	{
	//		continue ;
	//	}

	//	treePos = NULL ;															// 트리의 리스트에서 위치 정보를 받을 포인터를 선언하고 null 처리를 한다.	
	//	treePos = pTree->list.GetHeadPosition() ;									// 트리의 리스트를 헤드로 세팅한다.

	//	while(treePos)																// 위치 정보가 유효할 동안 while문을 돌린다.
	//	{
	//		pQuestString = NULL ;													// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
	//		pQuestString = (CQuestString*)pTree->list.GetNext(treePos) ;			// 0번째 위치에 해당하는 퀘스트 스트링을 받는다.

	//		if( !pQuestString )														// 퀘스트 스트링 정보가 유효하면,
	//		{
	//			continue ;
	//		}
	//		
	//		if( pQuestString->GetSubID() == 0 )
	//		{
	//			pInfo[nCount].dwQuestIdx = pQuestString->GetQuestID() ;
	//		}

	//		int nIndex = pQuestString->GetSubID() ;

	//		if( pTree->list.GetCount() == 2 )
	//		{
	//			if( nIndex != 0 )
	//			{
	//				pInfo[nCount].bComplete[nIndex] = TRUE ;
	//			}
	//			else
	//			{
	//				pInfo[nCount].bComplete[nIndex]	= pQuestString->IsComplete() ;
	//			}
	//		}
	//		else
	//		{
	//			pInfo[nCount].bComplete[nIndex] = pQuestString->IsComplete() ;
	//		}

	//		

	//		pInfo[nCount].nCount[nIndex]		= pQuestString->GetCount() ;
	//		pInfo[nCount].nTotalCount[nIndex]	= pQuestString->GetTotalCount() ;

	//		/*CQuestString* pDefaultString = QUESTMGR->GetQuestString(pQuestString->GetQuestID(), pQuestString->GetSubID()) ;

	//		if( pDefaultString )
	//		{
	//			pDefaultString->SetComplete(FALSE) ;

	//			pDefaultString->SetCount(0) ;
	//			pDefaultString->SetTotalCount(0) ;
	//		}*/
	//	}

	//	++nCount ;
	//}
}

void CQuestManager::LoadQuickViewInfoFromFile(DWORD* pIdx)
{
	if( !pIdx )
	{
		return ;
	}

	CQuestQuickViewDialog* pQuickViewDlg = NULL ;
	pQuickViewDlg = GAMEIN->GetQuestQuickViewDialog() ;

	if( pQuickViewDlg )
	{
		for( int index = 0 ; index < MAX_QUESTTREECOUNT ; ++index )
		{
			pQuickViewDlg->RegistQuest(pIdx[index]) ;
		}
	}

	/*CQuestQuickViewDialog* pQuickViewDlg = NULL ;
	pQuickViewDlg = GAMEIN->GetQuestQuickViewDialog() ;

	if( pQuickViewDlg )
	{
		pQuickViewDlg->LoadQuickViewInfoFromFile(pInfo) ;
	}*/

	//if( !pInfo )
	//{
	//	return ;
	//}

	//CQuestQuickViewDialog* pQuickViewDlg = NULL ;
	//pQuickViewDlg = GAMEIN->GetQuestQuickViewDialog() ;

	//if( !pQuickViewDlg )
	//{
	//	return ;
	//}

	//cPtrList* pList = NULL ;
	//pList = pQuickViewDlg->GetViewList() ;

	//if( !pList )
	//{
	//	return ;
	//}

	//for( int index = 0 ; index < MAX_QUESTTREECOUNT ; ++index )
	//{
	//	DWORD dwQuestIdx = pInfo[index].dwQuestIdx ;

	//	if( pQuickViewDlg->CheckSameQuest(dwQuestIdx) )									// 퀘스트 리스트에 인자로 넘어온 퀘스트와 같은 퀘스트가 있다면,
	//	{
	//		return ;																	// 리턴 처리를 한다.
	//	}

	//	int nTreeCount = 0 ;															// 퀘스트 트리 카운트를 담을 변수를 선언하고 0으로 세팅한다.

	//	nTreeCount = pList->GetCount() ;												// 퀘스트를 담는 리스트의 카운트를 받는다.	

	//	if( nTreeCount >= MAX_QUESTTREECOUNT )											// 트리 카운트가 최대 트리 수 이상이면,
	//	{
	//		return ;
	//	}
	//	else																			// 트리 개수가 5개 미만이면,
	//	{
	//		QuestTree* pTree = new QuestTree ;											// 트리를 생성한다.

	//		CQuestInfo* pQuestInfo = NULL ;												// 퀘스트 정보 포인터를 선언하고 null처리를 한다.
	//		pQuestInfo = QUESTMGR->GetQuestInfo(dwQuestIdx) ;							// 퀘스트 매니져로 부터, 인자로 넘어온 퀘스트 번호로 퀘스트 정보를 받는다.

	//		if( !pQuestInfo )															// 퀘스트 정보가 유효하지 않다면,
	//		{
	//			ASSERTMSG(0, "퀘스트 알림이 등록 중 퀘스트 정보를 받아오지 못했습니다.!") ;		// assert 에러 처리를 한다.
	//			return ;																// 리턴 처리를 한다.
	//		}

	//		int nSubQuestCount = 0 ;													// 서브 퀘스트 카운트를 받을 변수를 선언하고 0으로 세팅한다.
	//		nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// 퀘스트 정보로 부터 서브 퀘스트 카운트를 받는다.

	//		CQuestString* pQuestString ;												// 퀘스트 스트링 정보를 받을 포인터를 선언한다.
	//		
	//		for(int count = 0 ; count < nSubQuestCount ; ++count )						// 서브 퀘스트 카운트 만큼 for문을 돌린다.
	//		{
	//			pQuestString = NULL ;													// 퀘스트 스트링 정보를 받을 포인터를 null처리를 한다.
	//			pQuestString = QUESTMGR->GetQuestString(dwQuestIdx, count) ;			// 퀘스트 인덱스와, 서브 퀘스트 인덱스로 퀘스트 스트링을 받는다.
	//            
	//			if( pQuestString )														// 퀘스트 스트링 정보가 유효하다면,
	//			{
	//				CQuestString* pNewString = new CQuestString ;						// 퀘스트 스트링을 생성한다.
	//				memcpy(pNewString, pQuestString, sizeof(CQuestString)) ;			// 새로 생성한 퀘스트 스트링에, 현재의 퀘스트 스트링을 복사한다.

	//				cPtrList* pList = pQuestString->GetTitle() ;						// 퀘스트 스트링의 타이틀 리스트를 받는다.

	//				PTRLISTPOS titlePos = NULL ;										// 타이틀 리스트 상의 위치 정보를 받을 포인터를 선언하고 null처리를 한다.
	//				titlePos = pList->GetHeadPosition() ;								// 타이틀 리스트를 헤드로 세팅하고 위치 정보를 받는다.

	//				QString* pQString = NULL ;											// Q스트링을 받을 포인터를 선언하고 null처리를 한다.

	//				while(titlePos)														// 위치 정보가 유효할 동안 while문을 돌린다.
	//				{
	//					pQString = NULL ;												// Q스트링 포인터를 null처리를 한다.
	//					pQString = (QString*)pList->GetNext(titlePos) ;					// 위치에 따른 Q스트링을 받는다.

	//					if( pQString )													// Q스트링 정보가 유효하다면,
	//					{
	//						pNewString->SetTitleStr( pQString->Str ) ;					// Q스트링의 문자열을 받아서 새로운 퀘스트 스트링에 타이틀로 세팅한다.
	//					}
	//				}

	//				pNewString->SetComplete(pInfo[index].bComplete[count]) ;
	//				pNewString->SetCount(pInfo[index].nCount[count]) ;
	//				pNewString->SetTotalCount(pInfo[index].nTotalCount[count]) ;

	//				pQuestString->SetComplete(pInfo[index].bComplete[count]) ;
	//				pQuestString->SetCount(pInfo[index].nCount[count]) ;
	//				pQuestString->SetTotalCount(pInfo[index].nTotalCount[count]) ;

	//				pTree->list.AddTail(pNewString) ;									// 트리의 리스트에 퀘스트 스트링을 추가한다.

	//			}
	//		}

	//		pTree->State = eQTree_Open ;												// 트리 상태를 열린 상태로 세팅한다.
	//		pTree->QuestIdx = dwQuestIdx ;												// 트리에 퀘스트 인덱스를 세팅한다.
	//		pList->AddTail(pTree) ;														// 퀘스트 리스트에 트리를 추가한다.

	//		m_QuickViewQuestID[index] = dwQuestIdx ;									// 알림창에 등록 된 퀘스트 인덱스를 퀘스트 매니져가 관리 한다.
	//	}

	//	//pQuickViewDlg->ResetLoadedQuickViewDialog() ;									// 퀵 뷰를 새로고침한다.
	//	pQuickViewDlg->ResetQuickViewDialog() ;
	//}
}

void CQuestManager::SaveQuestInfoFromQuestDlg(sQUEST_INFO* pInfo)
{

}

void CQuestManager::LoadQuestInfoFromFile(sQUEST_INFO* pInfo)
{
}

//DWORD CQuestManager::GetQuestIDFromQuickView(int idx)												// 퀘스트 알림이에 등록 된 퀘스트 아이디를 반환하는 함수.
//{
//	DWORD dwQuestId = 0 ;																			// 퀘스트 아이디를 담을 변수를 선언하고 초기 값을 세팅한다.
//
//	if( idx >= MAX_QUICKVIEW_QUEST )																// 인자로 넘어온 인덱스가 최대 등록 가능 퀘스트 수를 넘으면,
//	{
//		return dwQuestId ;																			// 리턴 처리를 한다.
//	}
//
//	return m_QuickViewQuestID[idx] ;																// 인덱스에 해당하는 퀘스트 아이디를 반환한다.
//}


int CQuestManager::CheckQuestType(DWORD wQuestIdx, DWORD wSubQuestIdx)							
{
	int nQuestType = e_QT_ERROR_TYPE ;

	CQuest* pQuest = QUESTMGR->GetQuest( wQuestIdx );							// 퀘스트 정보를 받는다.
	CSubQuestInfo* pDescSubQuestInfo ;												// 서브 퀘스트 정보를 받을 포인터를 선언한다.

	pDescSubQuestInfo = NULL ;
	pDescSubQuestInfo = pQuest->GetSubQuestInfo(wSubQuestIdx) ;

	if( !pDescSubQuestInfo ) return e_QT_ERROR_TYPE ;

	cPtrList* pList = pDescSubQuestInfo->GetQuestNpcScriptList() ;

	if( !pList ) return e_QT_ERROR_TYPE ;

	CQuestNpcScript* pScript ;
	PTRLISTPOS scriptPos = pList->GetHeadPosition() ;

	BOOL bNpcResult = FALSE ;

	while(scriptPos)
	{
		pScript = NULL ;
		pScript = (CQuestNpcScript*)pList->GetNext(scriptPos) ;

		if(!pScript) continue ;

		cPtrList* pNpcScriptList = pScript->GetNpcScriptList() ;
		if( !pNpcScriptList ) continue ;

		CQuestNpcScriptData* pScriptData ;
		PTRLISTPOS dataPos = pNpcScriptList->GetHeadPosition() ;

		while(dataPos)
		{
			pScriptData = NULL ;
			pScriptData = (CQuestNpcScriptData*)pNpcScriptList->GetNext(dataPos) ;

			if(!pScriptData) continue ;

			return nQuestType = pScriptData->GetDialogType() ;
		}
	}

	return  e_QT_ERROR_TYPE ;
}





























