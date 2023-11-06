#include "stdafx.h"																		// 표준 시스템과, 프로젝트가 지정한, 자주쓰는 해더들을 모은 해더파일을 불러온다.

#include "WindowIDEnum.h"																// 윈도우 아이디를 정의해 놓은 헤더 파일을 불러온다.

#include "MHMap.h"																		// 묵향 맵 클래스 헤더 파일을 불러온다.

#include "GameIn.h"																		// 게임 인 클래스 헤더 파일을 불러온다.

#include "cDialogueList.h"																// 다이얼로그 리스트 클래스 헤더 파일을 불러온다.
#include "cHyperTextList.h"																// 하이퍼 텍스트 리스트 클래스 헤더 파일을 불러온다.
#include "cMsgBox.h"																	// 메시지 박스 클래스 헤더 파일을 불러온다.
#include "cPage.h"																		// 페이지 클래스 헤더 파일을 불러온다.
#include "GuildUnion.h"																	// 길드 유니온 클래스 헤더 파일을 불러온다.
#include "Npc.h"																		// NPC 클래스 헤더 파일을 불러온다.

#include "BailDialog.h"																	// 계산서 다이얼로그 클래스 헤더를 불러온다.
#include "cSkillTrainingDlg.h"															// 스킬 트레이닝 다이얼로그 클래스 헤더를 불러온다.
#include "../DateMatchingDlg.h"															// 데이트 매칭 다이얼로그 클래스 헤더 파일을 불러온다.
#include "FamilyCreateDialog.h"															// 패밀리 생성 다이얼로그 클래스 헤더 파일을 불러온다.
#include "GTBattleListDialog.h"															// 길드 배틀 리스트 다이얼로그 클래스 헤더 파일을 불러온다.
#include "GuildCreateDialog.h"															// 길드 생성 다이얼로그 클래스 헤더 파일을 불러온다.
#include "GuildLevelUpDialog.h"															// 길드 레벨업 다이얼로그 클래스 헤더 파일을 불러온다.
#include "GuildMarkDialog.h"															// 길드 마크 다이얼로그 클래스 헤더 파일을 불러온다.
#include "GuildWarehouseDialog.h"														// 길드워 하우스 다이얼로그 클래스 헤더 파일을 불러온다.
#include "GTRegistDialog.h"																// 길드 토너먼트 등록 다이얼로그 클래스 헤더 파일을 불러온다.
#include "GTRegistcancelDialog.h"														// 길드 토너먼트 취도 다이얼로그 클래스 헤더 파일을 불러온다.
#include "InventoryExDialog.h"															// 인벤토리 다이얼로그 클래스 헤더 파일을 불러온다.
#include "ItemShopDialog.h"																// 아이템샵 다이얼로그 클래스 헤더 파일을 불러온다.
#include "NpcImageDlg.h"																// NPC 이미지 다이얼로그 클래스 헤더 파일을 불러온다.
#include "NpcScriptDialog.h"															// NPC 스크립트 다이얼로그 클래스 헤더 파일을 불러온다.
#include "SeigeWarDialog.h"																// 공성전 다이얼로그 클래스 헤더 파일을 불러온다.
#include "StorageDialog.h"																// 스토리지 다이얼로그 클래스 헤더 파일을 불러온다.
#include "WantedDialog.h"																// 현상수배 다이얼로그 클래스 헤더 파일을 불러온다.

#include "NpcScriptDialog.h"
#include "InventoryExDialog.h"
#include "GuildLevelUpDialog.h"
#include "GuildMarkDialog.h"
#include "GuildWarehouseDialog.h"
#include "GuildCreateDialog.h"
#include "objectstatemanager.h"
#include "WantedManager.h"
#include "GuildManager.h"
#include "BailDialog.h"
#include "WantedDialog.h"
#include "./Interface/cWindowManager.h"
#include "cMsgBox.h"
#include "ItemShopDialog.h"
#include "MHMap.h"
#include "MoveManager.h"
// Guild Tournament
#include "GTRegistDialog.h"
#include "GTRegistcancelDialog.h"
#include "GTBattleListDialog.h"
#include "GuildDialog.h"

// guildunion
#include "GuildUnion.h"

#include "ChatManager.h"																// 채팅 매니져 헤더 파일을 불러온다.
//#include "FamilyManager.h"																// 패밀리 매니져 헤더 파일을 불러온다.
#include "GuildManager.h"																// 길드 매니져 헤더 파일을 불러온다.
#include "MoveManager.h"																// 이동 매니져 헤더 파일을 불러온다.
#include "NpcScriptManager.h"															// NPC 스크립트 매니져 헤더 파일을 불러온다.
#include "ObjectManager.h"																// 오브젝트 매니져 헤더 파일을 불러온다.
#include "objectstatemanager.h"															// 오브젝트 상태 매니져 헤더 파일을 불러온다.
#include "SiegeWarMgr.h"																// 공성전 매니져 헤더 파일을 불러온다.
#include "WantedManager.h"																// 현상수배 매니져 헤더 파일을 불러온다.

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.09.13
#include "../hseos/Farm/SHFarmManager.h"
// E 농장시스템 추가 added by hseos 2007.09.13

// desc_hseos_패밀리시스템_01
// S 패밀리시스템 추가 added by hseos 2007.10.25
#include "../hseos/Family/SHFamilyManager.h"
// E 패밀리시스템 추가 added by hseos 2007.10.25

GLOBALTON(cNpcScriptManager)															// 글로벌 톤 NPC 스크립트 매니져.
#include "GameResourceStruct.h"

cNpcScriptManager::cNpcScriptManager()													// 생성자 함수.
{
	m_nRegistNpcCount = 0;																// 등록 NPC 카운트를 0으로 세팅한다.

	m_pDialogue = NULL;																	// 다이얼로그 포인터를 NULL 처리 한다.

	m_pHyperText = NULL;																// 하이퍼 텍스르 포인터를 NULL 처리 한다.

	SetSelectedNpc(NULL);																// 선택 된 NPC를 해제 한다.
}

cNpcScriptManager::~cNpcScriptManager()													// 소멸자 함수.
{
	DeleteAllScriptInfo();																// 모든 스크립트 정보를 삭제한다.

	SAFE_DELETE( m_pDialogue );															// 다이얼로그 정보를 안전하게 지운다.
	SAFE_DELETE( m_pHyperText );														// 하이퍼 텍스트 정보를 안전하게 지운다.
}

void cNpcScriptManager::Init()															// 초기화 함수.
{
	LoadNpcScriptInfo();																// NPC 스크립트 정보를 로드한다.

	m_pDialogue = new cDialogueList;													// 다이얼로그 리스트 메모리를 할당하여 포인터로 받는다.

	m_pDialogue->LoadDialogueListFile(NPC_DIALOGUE_PATH, "rb");							// 파일로 부터 다이얼로그 리스트 정보를 로딩한다.

	m_pHyperText = new cHyperTextList;													// 하이퍼 텍스트 메모리를 할당하여 포인터로 받는다.

	m_pHyperText->LoadHyperTextFormFile(NPC_HYPERTEXT_PATH, "rb");						// 파일로 부터 하이퍼 텍스트 정보를 로딩한다.
}

void cNpcScriptManager::AddNpcScriptInfo(DWORD dwNpcId, cPage* pPage)					// NPC 스크립트 정보를 추가하는 함수.
{
	m_NpcScriptList[dwNpcId].AddTail(pPage);											// NPC 스크립트 리스트에 들어온 페이지 정보를 추가한다.
}

void cNpcScriptManager::DeleteAllScriptInfo()											// 모든 스크립트 정보를 삭제하는 함수.
{
	for(int i=0;i<MAX_NPC_COUNT;++i)													// 최대 NPC 카운트 만큼 for문을 돌린다.
	{
		PTRLISTSEARCHSTART(m_NpcScriptList[i],cPage*,pPage)								// NPC 스크립트 리스트에서 페이지 정보를 찾는다.
			delete pPage;																// 페이지 정보를 삭제한다.
		PTRLISTSEARCHEND																// 페이지 정보 찾기 종료.
			
		m_NpcScriptList[i].RemoveAll();													// 카운트에 해당하는 NPC 스크립트를 모두 비운다.
	}
}

cPage* cNpcScriptManager::GetMainPage( DWORD dwNpcId )									// 메인 페이지 정보를 반환하는 함수.
{
	if( m_NpcScriptList[dwNpcId].IsEmpty() ) return NULL;								// NPC 스크립트 리스트에 NPC와 관련된 정보가 없으면 NULL 리턴 처리를 한다.

	return (cPage*)m_NpcScriptList[dwNpcId].GetHead();									// NPC 스크립트 리스트에서 NPC 관련 메인 페이지를 리턴한다.
}

cPage* cNpcScriptManager::GetPage( DWORD dwNpcId, DWORD dwPageId )						// 원하는 페이지 정보를 반환하는 함수.
{
	PTRLISTSEARCHSTART(m_NpcScriptList[dwNpcId],cPage*,pPage)							// 들어온 NPC와 페이지 정보로, NPC 스크립트 리스트에서 정보를 찾는다.
		if( pPage->GetPageId() == dwPageId )											// 들어온 페이지 아이디와, 리스트 내의 페이지 아이디가 같으면,
		{
			return pPage;																// 페이지 정보를 리턴한다.
		}
	PTRLISTSEARCHEND																	// 페이지 정보 찾기 종료.
			
	return NULL;																		// NULL 리턴 처리를 한다.
}

void cNpcScriptManager::LoadNpcScriptInfo()												// NPC정보를 로딩하는 함수.
{
	CMHFile fp;																			// 묵향 파일을 선언한다.

	if(!fp.Init(NPC_SCRIPT_PATH, "rb"))													// 들어온 파일을 읽기 모드로 여는 것이 실패하면,
	{
		return;																			// 리턴 처리를 한다.
	}

	char buff[256]={0,};																// 임시 버퍼를 선언한다.

	while(1)																			// while문을 돌린다.
	{
		fp.GetString(buff);																// 임시 버퍼로 스트링을 읽어들인다.

		if( fp.IsEOF() )																// 파일 포인터가 파일의 끝을 가리키면,
		{
			break;																		// while문을 탈출한다.
		}

		if(buff[0] == '@')																// 버퍼의 첫 글자가 @와 같으면,
		{
			fp.GetLineX(buff, 256);														// 임시 버퍼로 라인을 읽어들인다.
			continue;																	// while문을 계속 돌린다.
		}

		CMD_ST(buff)																	// 임시 버퍼를 확인한다.
		CMD_CS("$NPC")																	// $NPC와 같으면,
			if((fp.GetString())[0] == '{')
			{
				LoadPageInfo(NULL, &fp);												// 페이지 정보를 로딩한다.
				++m_nRegistNpcCount;													// 등록 된 npc 수를 증가한다.
			}
			else
				__asm int 3;
		CMD_EN
	}
}

void cNpcScriptManager::LoadPageInfo(cPage* pPage, CMHFile* fp)							// 페이지 정보를 로딩하는 함수.
{
	char buff[256]={0,};																// 임시 버퍼를 선언한다.

	static DWORD dwNpcId;																// Npc에대한 Page정보가 완료 될때까지 유지 가능해야 한다.

	DWORD dwPageId;																		// 페이지 아이디를 담는 변수를 선언한다.

	int nDialogueCount = 0;																// 다이얼로그 카운트를 0으로 세팅한다.
	int nHyperLinkCount = 0;															// 하이퍼 링크 카운트를 0으로 세팅한다.

	BYTE emotion = 0;																	// 이모션 값을 0으로 세팅한다.

	DWORD dwDialogueId;																	// 다이얼로그 아이디를 담을 변수를 선언한다.

	HYPERLINK sHyper;																	// 하이퍼 링크 정보를 담을 구조체를 선언한다.

	while(1)																			// while문을 돌린다.
	{
		memset(&sHyper, 0, sizeof(HYPERLINK)) ;											// 하이퍼링크 구조체를 메모리 셋 한다.

		fp->GetString(buff);															// 임시 버퍼로 스트링을 읽어들인다.

		if(buff[0] == '}'|| fp->IsEOF())												// 임시 버퍼의 첫 글자가 }이거나 파일의 끝이면,
		{
			break;																		// while문을 탈출한다.
		}

		if(buff[0] == '@')																// 임시 버퍼의 첫 글자가 @와 같으면,
		{
			fp->GetLineX(buff, 256);													// 임시 버퍼로 라인을 읽어들인다.
			continue;																	// while문을 계속 돌린다.
		}

		CMD_ST(buff)																	// 임시 버퍼를 확인한다.

		CMD_CS("$PAGE")																	// 버퍼가 $PAGE와 같으면,

			if((fp->GetString())[0] == '{')												// 스트링을 읽고, 이 것이 {와 같으면,
			{
				cPage* pNewPage = new cPage;											// 페이지 메모리를 할당한다.
				AddNpcScriptInfo( dwNpcId, pNewPage );									//스크립트 정보를 추가한다.
				LoadPageInfo(pNewPage, fp);												// 페이지 정보를 로드한다.
			}

		CMD_CS("#NPCID")																// 버퍼가 #NPCID와 같으면,

			dwNpcId = fp->GetDword();													// npc 아이디를 받는다.

		CMD_CS("#PAGEINFO")																// 버퍼가 #PAGEINFO와 같으면,

			dwPageId = fp->GetDword();													// 페이지 아이디를 받는다.
			nDialogueCount = fp->GetInt();												// 다이얼로그 카운트를 받는다.
			nHyperLinkCount = fp->GetInt();												// 하이퍼 링크 카운트를 받는다.
			emotion = fp->GetInt();														// 모션을 받는다.
			pPage->Init( dwPageId, emotion );											// 페이지 정보를 초기화 한다.

		CMD_CS("#DIALOGUE")																// 버퍼가 #DIALOGUE와 같다면,

			for(int i =0; i<nDialogueCount;++i)											// 다이얼로그 카운트 만큼 for문을 돌린다.
			{
				dwDialogueId = fp->GetDword();											// 다이얼로그 아이디를 받는다.
				pPage->AddDialogue( dwDialogueId );										// 페이지에 다이얼로그를 추가한다.
			}

		CMD_CS("#HYPERLINK")															// 버퍼가 #HYPERLINK와 같다면,

			sHyper.wLinkId		= fp->GetWord();										// 링크 아이디를 받는다.
			sHyper.wLinkType	= fp->GetWord()+1;										// 링크 타입을 받는다.
			sHyper.dwData		= fp->GetDword();										// 데이터를 받는다.
			if( sHyper.wLinkType == emLink_MapChange )
			{
				sHyper.fXpos		= fp->GetFloat() ;									// 이동 할 맵의 x 좌표를 받는다.
				sHyper.fZpos		= fp->GetFloat() ;									// 이동 할 맵의 y 좌표를 받는다.
			}
			pPage->AddHyperLink( &sHyper );												// 링크 정보를 추가한다.

		CMD_CS("#MAPMOVELINK")
			sHyper.wLinkId		= fp->GetWord();										// 링크 아이디를 받는다.
			sHyper.wLinkType	= fp->GetWord()+1;										// 링크 타입을 받는다.
			sHyper.dwData		= fp->GetDword();										// 이동 할 맵 번호를 받는다.
			sHyper.fXpos		= fp->GetFloat() ;										// 이동 할 맵의 x 좌표를 받는다.
			sHyper.fZpos		= fp->GetFloat() ;										// 이동 할 맵의 y 좌표를 받는다.
			pPage->AddHyperLink( &sHyper );												// 링크 정보를 추가한다.

		CMD_EN
	}
}

void cNpcScriptManager::StartNpcScript( CNpc* pNpc )									// NPC 스크립트를 시작하는 함수.
{
	if( m_pCurSelectedNpc )																// 선택 된 NPC 가 있으면, // 기존에 존재하면 기존것을 삭제하고 새로 연다.
	{
		GAMEIN->GetNpcScriptDialog()->EndDialog();										// 다이얼로그를 닫는다.
		SetSelectedNpc(NULL);															// 선택 된 NPC 를 해제한다.
	}

	VECTOR3 pos;																		// 위치 정보를 담을 벡터를 선언한다.

	HERO->GetPosition( &pos );															// HERO의 위치 정보를 받는다.

	if( pNpc->GetNpcKind() != 37 )														// NPC 종류가 37과 같지 않으면,
	{
		MOVEMGR->SetLookatPos(pNpc,&pos,0,gCurTime);									// HERO를 보도록 한다.
	}

	SetSelectedNpc(pNpc);																// 선택 된 NPC를 세팅한다.


	// 080402 NYJ --- 다이얼로그를 여는데 실패했을경우 처리
	//GAMEIN->GetNpcScriptDialog()->OpenDialog( pNpc->GetNpcUniqueIdx(), pNpc->GetNpcListInfo()->ModelNum );	// 다이얼로그를 연다.
	if(!GAMEIN->GetNpcScriptDialog()->OpenDialog( pNpc->GetNpcUniqueIdx(), pNpc->GetNpcListInfo()->ModelNum ))
	{
		if(GAMEIN->GetNpcScriptDialog()->IsActive())
		{
			NPCSCRIPTMGR->SetSelectedNpc(NULL);
			GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
		}

		if( HERO->GetState() == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}
}

void cNpcScriptManager::StartNpcBusiness(DWORD dwData, DWORD dwParam)					// NPC 비지니스를 시작하는 함수.
{
	if( m_pCurSelectedNpc == NULL && dwParam == 0 )										// 현재 선택 된 NPC 정보가 없거나, 파라메터 값이 0과 같으면,
	{
		ASSERT(0);																		// ASSERT 체크를 한다.

		if( HERO->GetState() == eObjectState_Deal )										// HERO의 상태가 거래 중이면,
		{
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);					// 거래 상태를 종료한다.
		}
		
		return;																			// 리턴 처리를 한다.
	}

	if( m_pCurSelectedNpc )																// 현재 선택 된 NPC 정보가 있다면,
	{
		WORD wJobKind = m_pCurSelectedNpc->GetNpcJob() ;								// NPC 직업을 받는다.

		switch( wJobKind )																// NPC 직업을 확인한다.
		{
		case DEALER_ROLE :			Business_Dealer() ;					break ;			// 딜러라면, 딜을 시작한다.
		case CASTLE_ROLE :			Business_Castle(dwData) ;			break ;			// 성지기라면, 성과 관련된 일을 시작하낟.
		case CHANGGO_ROLE :			Business_Changgo(dwData, dwParam) ;	break ;			// 창고지기라면, 창고 비지니스를 시작한다.
		case MUNPA_ROLE :			Business_Guild(dwData) ;			break ;			// 길드 관리인 이라면, 길드 비지니스를 시작한다.
		case WANTED_ROLE :			Business_Wanted(dwData) ;			break ;			// 현상금 관리인 이라면, 현상금 비지니스를 시작한다.
		case SURYUN_ROLE :			Business_Suryun(dwData) ;			break ;			// 수련 관리인 이라면, 수련 비지니스를 시작한다.
		case MAPCHANGE_ROLE :		Business_MapChange(dwData) ;		break ;			// 맵 체인지 NPC 라면, 맵 체인지 비지니스를 시작한다.
		//case SYMBOL_ROLE :			Business_Symbol() ;					break ;			// 심볼 관리인 이라면, 심볼 관련 비지니스를 시작한다.
		case FAMILY_ROLE :			Business_Family(dwData) ;			break ;			// 패밀리 관리인 이라면, 패밀리 관련 비지니스를 시작한다.
		case IDENTIFICATION_ROLE :	Business_Identifycation() ;			break ;			// 주민등록 관리 인이라면, 주민등록 비지니스를 시작한다.
		default :																		// 그 외의 경우,
			{
				ASSERT(0);																// ASSERT 체크를 한다.

				if( HERO->GetState() == eObjectState_Deal )								// HERO의 상태가 거래중이라면,
				{
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);			// 거래 상태를 끝낸다.
				}

				return;																	// 리턴 처리를 한다.
			}
			break ;
		}
	}

	if( dwParam == eNpcParam_ShowpStorage )												// 파라메터 값이 eNpcParam_ShowpStorage와 같으면,
	{
		Business_Changgo(dwData, dwParam) ;												// 창고 비지니스를 시작한다.
	}

	GAMEIN->GetNpcScriptDialog()->DisActiveEx();										// NPC 스크립트 다이얼로그의 DisActiveEx()를 호출한다.
}

void cNpcScriptManager::Business_Dealer()  												// 딜러 비지니스.
{
	MSG_WORD msg;																		// 메시지 구조체를 선언한다.
	msg.Category = MP_ITEM;																// 카테고리를 아이템으로 세팅한다.
	msg.Protocol = MP_ITEM_DEALER_SYN;													// 프로토콜을 아이템 딜러 요청으로 세팅한다.
	msg.dwObjectID = HEROID;															// HERO의 아이디를 세팅한다.
	msg.wData = m_pCurSelectedNpc->GetNpcUniqueIdx();									// NPC 유니크 아이디를 세팅한다.

	NETWORK->Send(&msg, sizeof(msg));													// 메시지를 전송한다.

	GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);										// 스크립트 다이얼로그를 비활성화 한다.
}

void cNpcScriptManager::Business_Castle(DWORD dwData)  									// (공성)성 비지니스.
{
	switch( dwData )																	// 데이터를 확인한다.
	{
	case 0:																				// 세율관리 라면,
		{
			if( HERO->GetGuildIdx() == 0 ||												// HERO가 길드가 없거나,
				HERO->GetGuildMemberRank() != GUILD_MASTER )							// 길드 마스터가 아니라면,
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(323) );				// 권한이 없다는 메시지를 출력한다.
				return;																	// 리턴 처리를 한다.
			}

			SWPROFIT->SendProfitInfoSyn();												// 공성전 이득의 이득정보 싱크를 호출한다.
		}
		break;

	case 1:
	case 2:
	case 3:																				// 외성문
	case 4:
	case 5:
	case 6:																				// 내성문
		{	
			if( HERO->GetGuildIdx() == 0 ||												// HERO가 길드가 없거나,
				HERO->GetGuildMemberRank() != GUILD_MASTER )							// 길드 마스터가 아니라면,
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(323) );				// 권한이 없다는 메시지를 출력한다.
				return;																	// 리턴 처리를 한다.
			}

			DWORD dwMoney = 0;															// 머니 정보를 담을 변수를 선언한다.

			switch( dwData )															// 데이터를 확인한다.
			{
			case 1:		dwMoney = 1000000;		break;									// 데이터에 따른 머니를 확인한다.
			case 2:		dwMoney = 2000000;		break;
			case 3:		dwMoney = 3000000;		break;
			case 4:		dwMoney = 1500000;		break;
			case 5:		dwMoney = 2500000;		break;				
			case 6:		dwMoney = 4000000;		break;
			}

			if( HERO->GetMoney() < dwMoney )											// HERO의 머니가 세율 머니 보다 작으면,
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(324) );				// 돈이 부족하다는 메시지를 출력한다.
				return;																	// 리턴 처리를 한다.
			}

			MSG_DWORD Msg;																// 메시지 구조체를 선언한다.
			Msg.Category = MP_SIEGEWAR;													// 카테고리를 공성전로 세팅한다.
			Msg.Protocol = MP_SIEGEWAR_UPGRADE_GATE_SYN;								// 프로토콜을 공성전 업그레이드 게이트 싱크로 세팅한다.
			Msg.dwObjectID = HEROID;													// HERO 아이디를 세팅한다.
			Msg.dwData = dwData;														// 데이터를 세팅한다.

			NETWORK->Send( &Msg, sizeof(Msg) );											// 메시지를 전송한다.

			return;																		// 리턴 처리를 한다.
		}
		break;
	}	
}

void cNpcScriptManager::Business_Changgo(DWORD dwData, DWORD dwParam)					// 창고 비지니스.
{
	// 080513 KTH -- NPC 스크립트가 열려 있어야만 열수 있도록 변경 (defense hack)
	if( !GAMEIN->GetNpcScriptDialog()->IsActive() )
		return;

	if(dwData == 0)																		// 데이터를 확인한다.
	{	
		if(GAMEIN->GetStorageDialog()->IsItemInit())									// 창고 아이템의 아이템 초기화가 되었으면,
		{
			if( !GAMEIN->GetInventoryDialog()->IsActive() )								// 인벤토리 다이얼로그가 활성화 중이면,
			{
				GAMEIN->GetStorageDialog()->ShowStorageDlg(TRUE) ;							// 창고 다이얼로그를 활성화 한다.
			}
			else
			{
				GAMEIN->GetStorageDialog()->SetActive(TRUE) ;							// 창고 다이얼로그를 활성화 한다.
			}

			if(HERO->GetStorageNum() == 0)												// 개설한 창고가 없다면,
			{

				GAMEIN->GetStorageDialog()->ShowStorageMode(eStorageMode_NoWare);		// 창고가 없다는 모드로 세팅한다.

				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);							// 창고 다이얼로그를 비활성화 한다.
			}
			else																		// 개설한 창고가 있다면,
			{
				GAMEIN->GetStorageDialog()->ShowStorageMode(eStorageMode_StorageWare1);	// 창고 1을 활성화 한 모드로 한다.
				GAMEIN->GetStorageDialog()->AddStorageMode(eStorageMode_StorageWare1);	// 창고 모드를 추가한다.

				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);							// 스크립트 다이얼로그를 비활성화 한다.
			}
		}
		else																			// 창고의 아이템의 아이템 초기화가 되지 않았다면,
		{
			if(HERO->GetStorageNum() == 0)												// 개설한 창고가 없으면,
			{
				if( !GAMEIN->GetInventoryDialog()->IsActive() )							// 인벤토리 다이얼로그가 활성화 되지 않았으면,
				{
					GAMEIN->GetStorageDialog()->ShowStorageDlg(TRUE) ;					// 창고 다이얼로그를 활성화 한다.
				}

				GAMEIN->GetStorageDialog()->ShowStorageMode(eStorageMode_NoWare);		// 창고가 없다는 모드로 세팅한다.

				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);							// 스트립트 다이얼로그를 비활성화 한다.
			}
			else																		// 개설한 창고가 있다면,
			{
				MAP->SetVillage(TRUE) ;													// 마을로 세팅한다.

				if( dwParam == eNpcParam_ShowpStorage || MAP->IsVillage() == TRUE )		// 창고 npc 이고, 마을이 맞으면,
				{						
					MSG_WORD2 msg;														// 메시지 구조체를 선언한다.
					msg.Category = MP_ITEM;												// 카테고리를 아이템으로 세팅한다.
					msg.Protocol = MP_ITEM_STORAGE_ITEM_INFO_SYN;						// 프로토콜을 아이템 창고, 아이템 정보 싱크로 세팅한다.
					msg.dwObjectID = HEROID;											// HERO 아이디를 세팅한다.
					
					if( dwParam==eNpcParam_ShowpStorage )								// NPC가 창고 NPC 이면,
					{
						msg.wData1=79 ;													// 데이터를 79로 세팅한다.
					}
					else																// NPC가 창고 NPC가 아니면,
					{
						msg.wData1=m_pCurSelectedNpc->GetNpcUniqueIdx();				// 데이터를 NPC 유니크 인덱스로 세팅한다.
					}
					//dwParam==eNpcParam_ShowpStorage ? msg.wData1=79 : msg.wData1=m_pCurSelectedNpc->GetNpcUniqueIdx();

					msg.wData2 = eNpcParam_ShowpStorage;								// 데이터 2를 NPC 데이터로 세팅한다.

					NETWORK->Send(&msg, sizeof(msg));									// 메시지를 전송한다.
				}
				else																	// NPC가 창고 NPC가 아니거나, 마을이 아니면,
					return;																// 리턴 처리를 한다.
			}
		}

		OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);						// HERO의 상태를 거래 상태로 세팅한다.
	}	
	else if(dwData == 1)																// 가격표/구입
	{
		GAMEIN->GetStorageDialog()->SetStorageListInfo();								// 창고 다이얼로그의 리스트 정보를 세팅한다.
		GAMEIN->GetStorageDialog()->ShowStorageDlg(TRUE) ;								// 창고 다이얼로그의 창고 다이얼로그를 보여준다.

		GAMEIN->GetStorageDialog()->ShowStorageMode(eStorageMode_StorageListInfo);		// 창고 리스트 정보로 창고 모드를 세팅한다.
		GAMEIN->GetStorageDialog()->AddStorageMode(eStorageMode_StorageListInfo);		// 창고 다이얼로그에 창고 모드를 추가한다.

		GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);									// 스크립트 다이얼로그를 비활성화 한다.
	}
	else if(dwData == 2)																// 길드 창고
	{
		GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);

		if( ! HERO->GetGuildIdx() )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 325 ) );
			return;
		}

		const CGuildManager::LevelSetting* setting = GUILDMGR->GetLevelSetting( GUILDMGR->GetLevel() );
		ASSERT( setting );

		if( !	setting ||
			!	setting->mWarehouseSize )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 613 ) );
			return;
		}
		
		MSGBASE message;

		message.Category	= MP_GUILD;
		message.Protocol	= MP_GUILD_OPEN_WAREHOUSE_SYN;
		message.dwObjectID	= HEROID;

		NETWORK->Send( &message, sizeof( message ) );
	}
	else if(dwData == 3)																// 아이템몰창고
	{
		MSGBASE msg;																	// 메시지 구조체를 선언한다.
		msg.Category = MP_ITEM;															// 카테고리를 아이템으로 세팅한다.
		msg.Protocol = MP_ITEM_SHOPITEM_INFO_SYN;										// 프로토콜을 아이템 샵 정보 싱크로 세팅한다.
		msg.dwObjectID = HERO->GetID();													// HERO의 아이디를 세팅한다.

		NETWORK->Send(&msg, sizeof(msg));												// 메시지를 전송한다.

		GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);									// 스크립트 다이얼로그를 비활성화 한다.
		OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);						// HERO의 상태를 거래 상태로 세팅한다.
	}
}

void cNpcScriptManager::Business_Guild(DWORD dwData)  									// 길드 비지니스.
{
	switch( dwData )																	// 데이터를 확인한다.
	{
	case 0:																				// 길드를 생성하려고 한다면,
		{
			if(HERO->GetGuildIdx())														// HERO의 길드 인덱스가 유효하면,
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(327));					// 이미 길드가 있다는 메시지를 출력한다.
				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);							// NPC 스크립트 다이얼로그를 비활성화 한다.

				return;																	// 리턴 처리를 한다.
			}

			char* pEntryDate = HERO->GetGuildEntryDate() ;								// HERO의 GuildEntryDate를 받느다.

			if(GUILDMGR->CanEntryGuild(pEntryDate) == FALSE)							// 길드매니져의 CanEntryGuild()를 체크하여 FALSE와 같으면,
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(826), pEntryDate);		// 주어진 날짜부터 창설 가능하다는 메시지를 출력한다.
				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);							// NPC 스크립트 다이얼로그를 닫는다.

				return;																	// 리턴 처리를 한다.
			}

			cDialog* pDlg = (cDialog*)GAMEIN->GetGuildCreateDlg();						// 길드 생성 다이얼로그 정보를 받아온다.

			if( pDlg )																	// 길드 생성 다이얼로그 정보가 유효한지 체크한다.
			{
				pDlg->SetActive(TRUE);													// 길드 생성 다이얼로그를 활성화 한다.
			}

			GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);								// NPC 스크립트 다이얼로그를 닫는다.
		}
		break;

	case 1:																				// 길드를 해제하려고 한다면,
		{
			if(HERO->GetGuildIdx() == 0)												// HERO의 길드 인덱스가 유효하지 않으면,
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));					// 길드에 가입되어 있지 않다는 메시지를 출력한다.
				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);							// NPC 스크립트 다이얼로그를 비활성화 한다.

				return;																	// 리턴 처리를 한다.
			}
			else if(HERO->GetGuildMemberRank() == GUILD_MASTER)							// HERO의 직위가 길드 마스터와 같다면,
			{
				WINDOWMGR->MsgBox( MBI_GUILD_BREAKUP, MBT_YESNO, CHATMGR->GetChatMsg( 329 ) );	// 패널티를 받게 된다는 선택창을 띄운다.
				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);							// NPC 스크립트 다이얼로그를 닫는다.

				return ;																// 리턴 처리를 한다.
			}
			else																		// HERO의 직위기 길드 마스터가 아니라면,
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(323));					// 권한이 없다는 메시지를 출력한다.
				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);							// NPC 스크립트 다이얼로그를 닫는다.

				return;																	// 리턴 처리를 한다.
			}
		}
		break;

	case 2:																				// 길드 레벨업 관련.
		{
			if(HERO->GetGuildIdx() == 0)												// HERO의 길드 인덱스를 체크한다.
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));					// 길드에 가입되어 있지 않다는 메시지를 출력한다.
			}
			else if(HERO->GetGuildMemberRank() != GUILD_MASTER)							// HERO의 직위가 길드 마스터와 같지 않다면,
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(323));					// 권한이 없다는 메시지를 출력한다.
			}
			else
			{
				GAMEIN->GetGuildLevelUpDlg()->SetActive(TRUE);							// 길드 레벨업 다이얼로그를 활성화 한다.
			}	

			GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
		}
		break;

	case 3:																				// 길드 마크 등록.
		{
			if( HERO->GetGuildIdx() )
			{
				GAMEIN->GetGuildMarkDlg()->ShowGuildMark();
				//GAMEIN->GetGuildMarkDlg()->SetActive( TRUE );
			}
			else
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));
			}

			GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);

			//if(GUILDMGR->GetGuildLevel() < GUILD_2LEVEL)								// 길드 레벨이 2레벨보다 작으면,
			//{
			//	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(326));					// 길드 레벨이 낮다는 메시지를 출력한다.

			//	return;																	// 리턴 처리를 한다.
			//}

			//if(HERO->GetGuildIdx() && (HERO->GetGuildMemberRank() == GUILD_MASTER))		// HERO의 길드 인덱스가 유효하고, 마스터라면,
			//{
			//	GAMEIN->GetGuildMarkDlg()->ShowGuildMark();								// 길드 마크 다이얼로그의 길드 마크를 보여준다.
			//}
			//else																		// 인덱스가 없거나, 마스터가 아니라면,
			//{
			//	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(330));					// 문장은 길드에 속한 마스터만 가능하다는 메시지를 출력한다.

			//	return;																	// 리턴 처리를 한다.
			//}	
		}
		break;

	// 070820 웅주, 동맹 생성/해제가 길드창으로 옮겨짐
	//case 4:																				// 길드 동맹을 생성하려 한다면,	
	//	{
	//		if( GUILDUNION->CheckGuildUnionCondition( 0 ) )								// 길드 동맹 상태를 체크하여 TRUE를 반환하면,
	//		{
	//			GAMEIN->GetGuildUnionCreateDlg()->SetActive( TRUE );					// 길드 동맹 생성 다이얼로그를 활성화 한다.
	//		}
	//		else																		// 길드 동맹 상태를 체크하여 FALSE를 리턴하면,
	//			return;																	// 리턴 처리를 한다.
	//	}
	//	break;

	//case 5:																				// 길드 동맹을 삭제하려고 하면,
	//	{
	//		if( GUILDUNION->CheckGuildUnionCondition( 1 ) )								// 길드 동맹 상태를 체크하여 TRUE를 반환하면,
	//		{
	//			WINDOWMGR->MsgBox( MBI_GUILD_UNION_DESTROY, MBT_YESNO, CHATMGR->GetChatMsg( 539 ) );	// 패널티 메시지를 출력한다.
	//		}
	//		else																		// 길드 동맹 상태를 체크하여 FALSE를 리턴하면,
	//			return;																	// 리턴 처리를 한다.
	//	}
	//	break;

	// 동맹 마크 등록
	case 4:
		{
			GAMEIN->GetGuildMarkDlg()->ShowGuildUnionMark();

			GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
		}
		break;
	
	//case 7:																				// 공성전 시간 등록을 하려고 하면,
	//	{
	//		if(HERO->GetGuildIdx() == 0)												// HERO의 길드 인덱스가 유효하지 않으면,
	//		{
	//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));					// 길드에 가입되어 있지 않다는 메시지를 출력한다.

	//			return;																	// 리턴 처리를 한다.
	//		}
	//		else if(HERO->GetGuildMemberRank() != GUILD_MASTER)							// HERO의 계급이 길드 마스터와 같지 않으면,
	//		{
	//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(323));					// 권한이 없다는 메시지를 출력한다.

	//			return;																	// 리턴 처리를 한다.
	//		}

	//		GAMEIN->GetSWTimeRegDlg()->SetActive( TRUE );								// 공성전 시간 등록 다이얼로그를 활성화 한다.
	//	}
	//	break;

	//case 8:																				// 공성전 신청 리스트를 보려고 하면,
	//	{
	//		if(HERO->GetGuildIdx() == 0)												// HERO의 길드 인덱스가 유효하지 않으면,
	//		{
	//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));					// 길드에 가입되어 있지 않다는 메시지를 출력한다.

	//			return;																	// 리턴 처리를 한다.
	//		}
	//		else if(HERO->GetGuildMemberRank() != GUILD_MASTER)							// HERO의 계급이 길드 마스터와 같지 않으면,
	//		{
	//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(323));					// 권한이 없다는 메시지를 출력한다.

	//			return;																	// 리턴 처리를 한다.
	//		}

	//		MSGBASE Msg;																// 메시지 구조체를 선언한다.
	//		Msg.Category = MP_SIEGEWAR;													// 카테고리를 공성전로 세팅한다.
	//		Msg.Protocol = MP_SIEGEWAR_PROPOSALLIST_SYN;								// 프로토콜을 공성전 신청 리스트 싱크로 세팅한다.
	//		Msg.dwObjectID = HEROID;													// HERO 아이디를 세팅한다.

	//		NETWORK->Send( &Msg, sizeof(Msg) );											// 메시지를 전송한다.
	//	}
	//	break;

	//case 9:																				// 공성전을 관람하려고 하면,
	//	{	
	//		WINDOWMGR->MsgBox( MBI_SW_OBSERVER_SYN, MBT_YESNO, CHATMGR->GetChatMsg(332) );	// 각인을 하겠냐는 알림창을 띄운다.

	//		return;																		// 리턴 처리를 한다.
	//	}
	//	break;

	//case 10:																			// 공성전 정보.
	//	{
	//		MSGBASE Msg;																// 메시지 구조체를 선언한다.
	//		Msg.Category = MP_SIEGEWAR;													// 카테고리를 공성전로 세팅한다.
	//		Msg.Protocol = MP_SIEGEWAR_GUILDLIST_SYN;									// 프로토콜을 공성전 길드 리스트 싱크로 세팅한다.
	//		Msg.dwObjectID = HEROID;													// HERO 아이디를 세팅한다.

	//		NETWORK->Send( &Msg, sizeof(Msg) );											// 메시지를 전송한다.
	//	}
	//	break;

	//case 11:																			// 수비팀으로 들어가기를 한다면,
	//	{
	//		if(HERO->GetGuildIdx() == 0)												// HERO의 길드 인덱스가 유효하지 않으면,
	//		{
	//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));					// 길드에 가입되어 있지 않다는 메시지를 출력한다.

	//			return;																	// 리턴 처리를 한다.
	//		}
	//		
	//		MSG_DWORD2 Msg;																// 메시지 구조체를 선언한다.
	//		Msg.Category = MP_SIEGEWAR;													// 카테고리를 공성전으로 세팅한다.
	//		Msg.Protocol =  MP_SIEGEWAR_MOVEIN_SYN;										// 프로토콜을 공성전 무브인 싱크로 세팅한다.
	//		Msg.dwObjectID = HEROID;													// HERO 아이디를 체크한다.
	//		Msg.dwData1 = HERO->GetGuildIdx();											// HERO의 길드 인덱스를 세팅한다.
	//		Msg.dwData2 = 0;															// 데이터 2를 0으로 세팅한다.

	//		NETWORK->Send( &Msg, sizeof(Msg) );											// 메시지를 전송한다.
	//		
	//		return;																		// 리턴 처리를 한다.
	//	}
	//	break;

	//case 12:																			// 공격팀으로 들어가기를 한다면,
	//	{
	//		if(HERO->GetGuildIdx() == 0)												// HERO의 길드 인덱스가 유효하지 않으면,
	//		{
	//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));					// 길드에 가입되어 있지 않다는 메시지를 출력한다.

	//			return;																	// 리턴 처리를 한다.
	//		}

	//		MSG_DWORD2 Msg;																// 메시지 구조체를 선언한다.
	//		Msg.Category = MP_SIEGEWAR;													// 카테고리를 공성전으로 세팅한다.
	//		Msg.Protocol =  MP_SIEGEWAR_MOVEIN_SYN;										// 프로토콜을 공성전 입장 싱크로 한다.
	//		Msg.dwObjectID = HEROID;													// HERO 아이디를 세팅한다.
	//		Msg.dwData1 = HERO->GetGuildIdx();											// 길드 인덱스를 세팅한다.
	//		Msg.dwData2 = 1;															// 데이터2를 1로 세팅한다.

	//		NETWORK->Send( &Msg, sizeof(Msg) );											// 메시지를 전송한다.
	//		
	//		return;																		// 리턴 처리를 한다.
	//	}
	//	break;

	//case 13:																			// 수성 참여신청
	//	{
	//		if(HERO->GetGuildIdx() == 0)												// HERO의 길드 인덱스가 유효하지 않으면,
	//		{
	//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));					// 길드에 가입되어 있지 않다는 메시지를 출력한다.

	//			return;																	// 리턴 처리를 한다.
	//		}	
	//		else if(HERO->GetGuildMemberRank() != GUILD_MASTER)							// HERO의 계급이 길드 마스터와 같지 않으면,
	//		{	
	//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(323));					// 권한이 없다는 메시지를 출력한다.

	//			return;																	// 리턴 처리를 한다.									
	//		}
	//		
	//		WINDOWMGR->MsgBox( MBI_SW_PROPOSAL_SYN, MBT_YESNO, CHATMGR->GetChatMsg(333) );	// 수성측 길드장에게 수성참여 신청을 하시겠냐는 알림창을 띄운다.

	//		return;																		// 리턴 처리를 한다.
	//	}
	//	break;

	//case 14:																			// 공성 참여신청
	//	{
	//		if(HERO->GetGuildIdx() == 0)												// HERO의 길드 인덱스가 유효하지 않으면,
	//		{																			
	//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));					// 길드에 가입되어 있지 않다는 메시지를 출력한다.
	//																					
	//			return;																	// 리턴 처리를 한다.
	//		}																			
	//		else if(HERO->GetGuildMemberRank() != GUILD_MASTER)							// HERO의 계급이 길드 마스터와 같지 않으면,
	//		{																			
	//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(323));					// 권한이 없다는 메시지를 출력한다.
	//																					
	//			return;																	// 리턴 처리를 한다.									
	//		}

	//		WINDOWMGR->MsgBox( MBI_SW_ATTACKREGIST_SYN, MBT_YESNO, CHATMGR->GetChatMsg(334) );	// 성을 차지하기 위한 공성측에 신청을 하시겠습니까?	

	//		return;																		// 리턴 처리를 한다.		
	//	}
	//	break;

	//case 15:
	//case 16:
	//case 17:
	//case 18:
	//case 19:
	//	{
	//		if(HERO->GetGuildIdx() == 0)												// HERO의 길드 인덱스가 유효하지 않으면,
	//		{																			
	//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));					// 길드에 가입되어 있지 않다는 메시지를 출력한다.
	//																					
	//			return;																	// 리턴 처리를 한다.
	//		}																			
	//		if(HERO->GetGuildMemberRank() != GUILD_MASTER)								// HERO의 계급이 길드 마스터와 같지 않으면,
	//		{																			
	//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(326));					// 길드 레벨이 낮다는 메시지를 출력한다.
	//																					
	//			return;																	// 리턴 처리를 한다.									
	//		}

	//		if( GUILDMGR->CheckChangeLocation( dwData ) )								// 지역을 확인하여 TRUE 이면,
	//		{
	//			// 본거지를 이동하겠냐는 메시지를 띄운다.
	//			WINDOWMGR->MsgBox( MBI_GUILD_CHANGE_LOCATION, MBT_YESNO, CHATMGR->GetChatMsg(335), GetMapName( GUILDMGR->GetTempLocation() ) );
	//		}
	//		else																		// FALSE이면,
	//			return;																	// 리턴 처리를 한다.
	//	}
	//	break;

	//case 20:																			// 공성전 시간 정보.
	//	{
	//		MSGBASE Msg;																// 메시지 구조체를 선언한다.
	//		Msg.Category = MP_SIEGEWAR;													// 카테고리를 공성전으로 세팅한다.
	//		Msg.Protocol = MP_SIEGEWAR_TIMEINFO_SYN;									// 프로토콜을 공성전 시간정보 싱크로 세팅한다.
	//		Msg.dwObjectID = HEROID;													// HERO 아이디를 세팅한다.

	//		NETWORK->Send( &Msg, sizeof(Msg) );											// 메시지를 전송한다.
	//	}
	//	break;
	}
}

void cNpcScriptManager::Business_Wanted(DWORD dwData)  									// 현상수배 비지니스.
{
	//전에 클릭한게 늦게올 수도 있네
	if(GAMEIN->GetWantedDialog()->IsActive() == TRUE)									// 현상수배 다이얼로그가 활성화 중이면,
	{
		return;																			// 리턴 처리를 한다.
	}

	if(dwData == 0) //창고 보기
	{
		WANTEDMGR->SortWantedListSyn(1);												// 현상수배 리스트를 정렬하는 싱크를 보낸다.
	}
	else if(dwData == 1)
	{
		GAMEIN->GetBailDialog()->Open();												// 보석금 다이얼로그를 연다.
	}
}

void cNpcScriptManager::Business_Suryun(DWORD dwData)  									// 수련 비지니스.
{
	if( dwData == eSuryunNpc_Suryun )													// 수련하기		
	{
	}
	else if( dwData == eSuryunNpc_About_GuildTournament )								// 문파토너먼트 알아보기
	{
		if( HERO->GetState() == eObjectState_Deal )										// HERO 상태가 거래중이면,
		{
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);					// 거래 상태를 해제한다.
		}
	}
	else if( dwData == eSuryunNpc_Regist_GuildTournament )								// 문파토너먼트 참가신청하기
	{
		MSGBASE msg;																	// 메시지 구조체를 선언한다.
		msg.Category = MP_GTOURNAMENT;													// 카테고리를 길드 토너먼트로 세팅한다.
		msg.Protocol = MP_GTOURNAMENT_REGISTCOUNT;										// 프로토콜을 길드 토너먼트 등록 카운트로 세팅한다.
		msg.dwObjectID = HEROID;														// HERO 아이디를 세팅한다.
			
		NETWORK->Send( &msg, sizeof(msg) );												// 메시지를 전송한다.
	}
	else if( dwData == eSuryunNpc_Cancel_GuildTournament )								// 문파토너먼트 신청 취소하기
	{
		if( GAMEIN->GetGTRegistcancelDlg() )											// 길드 토너먼트 취소 다이얼로그 정보가 유효하면,
		{
			GAMEIN->GetGTRegistcancelDlg()->SetActive( TRUE );							// 길드 토너먼트 취소 다이얼로그 다이얼로그를 활성화 한다.
		}
	}
	else if( dwData == eSuryunNpc_Show_GuildStand )										// 문파토너먼트 대진표 보기
	{
		MSG_WORD msg;																	// 메시지 구조체를 선언한다.
		msg.Category = MP_GTOURNAMENT;													// 카테고리를 길드 토너먼트로 세팅한다.
		msg.Protocol = MP_GTOURNAMENT_STANDINGINFO_SYN;									// 프로토콜을 길드 토너먼트 스탠딩 정보 싱크로 세팅한다.
		msg.wData = 1;																	// 데이터를 1로 세팅한다.
		msg.dwObjectID = HEROID;														// HERO 아이디를 세팅한다.

		NETWORK->Send( &msg, sizeof(msg) );												// 메시지를 전송한다.
	}
	else if( dwData == eSuryunNpc_Enter_GuildTournament )								// 문파토너먼트 입장하기
	{
		MSG_DWORD msg;																	// 메시지 구조체를 선언한다.
		msg.Category = MP_GTOURNAMENT;													// 카테고리를 길드 토너먼트로 세팅한다.
		msg.Protocol = MP_GTOURNAMENT_MOVETOBATTLEMAP_SYN;								// 프로토콜을 길드 토너먼트 배틀맵으로 이동 싱크로 세팅한다.
		msg.dwObjectID = HEROID;														// HERO 아이디를 세팅한다.
		msg.dwData = 0;																	// 데이터를 0으로 세팅한다.

		NETWORK->Send( &msg, sizeof(msg) );												// 메시지를 전송한다.
	}
	else if( dwData == eSuryunNpc_EnterObserver_GuildTournament )						// 관람모드로 문파토너먼트 입장하기
	{
		GAMEIN->SetGuildIdx( HERO->GetGuildIdx() );										// 길드 인덱스를 HERO의 길드 인덱스로 세팅한다.

		MSGBASE msg;																	// 메시지 구조체를 선언한다.
		msg.Category = MP_GTOURNAMENT;													// 카테고리를 길드 토너먼트로 세팅한다.
		msg.Protocol = MP_GTOURNAMENT_BATTLEINFO_SYN;									// 프로토콜을 길드 토너먼트 배틀 정보 요청으로 세팅한다.
		msg.dwObjectID = HEROID;														// HERO 아이디를 세팅한다.

		NETWORK->Send( &msg, sizeof(msg) );												// 메시지를 전송한다.
	}
	else if( dwData == eSuryunNpc_SkillTraining )										// 스킬 수련이라면,
	{
		if(GAMEIN->GetNpcScriptDialog()->IsActive())									// NPC 스크립트 다이얼로그가 열려 있다면,
		{
			NPCSCRIPTMGR->SetSelectedNpc(NULL);											// 선택 된 NPC를 해제한다.
			GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);								// NPC 스크립트 다이얼로그를 닫는다.
		}
	
		GAMEIN->GetSkillTrainingDlg()->OpenDialog();									// 스킬 수련 다이얼로그를 연다.

		return;																			// 리턴 처리를 한다.
	}
}

void cNpcScriptManager::Business_MapChange(DWORD dwData)  								// 맵 체인지 비지니스.
{
	if(dwData == 0)																		// 데이터가 0과 같으면,
	{
		MSG_DWORD msg;																	// 메시지 구조체를 선언한다.
		msg.Category = MP_NPC;															// 카테고리를 NPC 로 세팅한다.
		msg.Protocol = MP_NPC_DOJOB_SYN;												// 프로토콜을 NPC 직업 시작 요청으로 세팅한다.
		msg.dwObjectID = HEROID;														// HERO 아이디를 세팅한다.
		msg.dwData = m_pCurSelectedNpc->GetID();										// 현재 선택 된 NPC의 아이디를 세팅한다.

		NETWORK->Send(&msg, sizeof(msg));												// 메시지를 전송한다.
	}
	else																				// 데이터가 0이 아니면,
	{
		if(GAMEIN->GetNpcScriptDialog()->IsActive())									// NPC 스크립트 다이얼로그가 열려있으면,
		{
			NPCSCRIPTMGR->SetSelectedNpc(NULL);											// 현재 선택 된 NPC를 해제한다.
			GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);								// NPC 스크립트 다이얼로그를 닫는다.
		}
		
		if( HERO->GetState() == eObjectState_Deal )										// HERO 상태가 거래중이면,
		{
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);					// 거래 상태를 끝낸다.
		}
	}
}

void cNpcScriptManager::Business_Symbol()  												// 심볼 비지니스.
{
	// 각인시작		
	if( HERO->GetGuildMemberRank() == GUILD_MASTER )									// HERO의 계급이 길드 마스터라면,
	{
		CGuildDialog* dialog = GAMEIN->GetGuildDlg();

		if( dialog->GetLevel() == 5 )											// 길드 정보가 유효하고, 길드 레벨이 5와 같으면,
		{
			MSG_DWORD msg;																// 메시지 구조체를 선언한다.
			SetProtocol( &msg, MP_SIEGEWAR, MP_SIEGEWAR_ENGRAVE_SYN );					// 카테고리를 공성전, 프로토콜을 공성전 새기기 요청으로 세팅한다.
			msg.dwObjectID = HEROID;													// HERO 아이디를 세팅한다.
			msg.dwData = HERO->GetGuildIdx();											// HERO의 길드 인덱스를 세팅한다.

			NETWORK->Send( &msg, sizeof(msg) );											// 메시지를 전송한다.
		}
		else																			// HERO의 길드 정보가 유효하지 않거나, 길드 레벨이 5와 같지 않으면,
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(336) );					// 완성된 길드만이 각인을 할 수 있다는 메시지를 출력한다.
		}
	}																					// HERO의 계급이 마스터가 아니면,
	else
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(337) );						// 길드 마스터만이 각인을 할 수 있다는 메시지를 출력한다.
	}
}

void cNpcScriptManager::Business_Family(DWORD dwData)  									// 패밀리 비지니스.
{
	if( dwData == 0 )																	// 패밀리를 생성하려고 하면,
	{
		if(GAMEIN->GetNpcScriptDialog()->IsActive())									// NPC스크립트 다이얼로가 열려있으면,
		{
			NPCSCRIPTMGR->SetSelectedNpc(NULL);											// 선택 된 NPC를 해제한다.
			GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);								// NPC 스크립트 다이얼로그를 닫는다.
		}

		GAMEIN->GetFamilyCreateDlg()->SetActive(TRUE);									// 패밀리 생성 다이얼로그를 활성화 한다.
	}
	else if( dwData == 1 )																// 패밀리를 해체하려고 하면,
	{
		if(GAMEIN->GetNpcScriptDialog()->IsActive())									// NPC 스크립트가 열려 있다면,
		{
			NPCSCRIPTMGR->SetSelectedNpc(NULL);											// 선택 된 NPC를 해제 한다.
			GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);								// NPC 스크립트를 닫는다.
		}				

   		WINDOWMGR->MsgBox( MBI_FAMILY_BREAKUP, MBT_YESNO, CHATMGR->GetChatMsg( 1144 ), int((CSHFamilyManager::LEAVE_PENALTY_EXP_DOWN_RATE[CSHFamilyManager::FLK_BREAKUP]+0.001)*100), CSHFamilyManager::LEAVE_PENALTY_REJOIN_TIME[CSHFamilyManager::FLK_BREAKUP] );
	}
	else if(dwData == 2 )																// 주민등록을 발급 받으려고 하면,
	{
		const DATE_MATCHING_INFO& dateInfo = HERO->GetDateMatchingInfo() ;				// 데이트 매칭 정보를 받는다.

		if( &dateInfo )																	// 정보가 유효하면,
		{
			if( dateInfo.bIsValid )														// 이미 발급 받았다면,
			{
				//WINDOWMGR->MsgBox( MBI_IDENTIFICATION_ALREADY_HAVE, MBT_OK, "이미 주민등록증을 발급 받았습니다.") ;
				WINDOWMGR->MsgBox( MBI_IDENTIFICATION_ALREADY_HAVE, MBT_OK, CHATMGR->GetChatMsg( 830 ) );

				// 080122 LYW --- NpcScriptManager : npc 다이얼로그 닫기.
				cNpcScriptDialog* pNpcDlg = NULL ;
				pNpcDlg = GAMEIN->GetNpcScriptDialog() ;
				
				if( pNpcDlg )
				{
					pNpcDlg->SetActive(FALSE) ;
				}
			}
			else																		// 발급 받지 않았다면,
			{
				CIdentification* pIdentificationDlg = GAMEIN->GetIdentificationDlg() ;	// 주민등록 발급창 정보를 받는다.

				if(pIdentificationDlg)													// 발급창 정보가 유효하면,
				{
					pIdentificationDlg->SettingControls() ;								// 컨트롤 정보들을 세팅한다.

					if( !pIdentificationDlg->IsActive() )								// 발급창이 열려있지 않으면,
					{
						pIdentificationDlg->SetActive(TRUE) ;							// 발급창을 연다.
					}
				}

				CFavorIcon* pFavorIconDlg = GAMEIN->GetFavorIconDlg() ;					// 성향 창 정보를 받느다.

				if( pFavorIconDlg )														// 성향 창 정보가 유효하면,
				{
					if( !pFavorIconDlg->IsActive() )									// 성향창이 열려있지 않으면,
					{
						pFavorIconDlg->SetActiveRecursive(TRUE) ;						// 성향창을 활성화 한다.
					}
				}

			}
		}
	}
}

void cNpcScriptManager::Business_Identifycation()  										// 주민등록 비지니스.
{
	const DATE_MATCHING_INFO& dateInfo = HERO->GetDateMatchingInfo() ;					// 데이트 매칭 정보를 받는다.

	if( &dateInfo )																		// 정보가 유효하면,
	{
		if( dateInfo.bIsValid )															// 이미 발급 받았다면,
		{
			//WINDOWMGR->MsgBox( MBI_IDENTIFICATION_ALREADY_HAVE, MBT_OK, "이미 주민등록증을 발급 받았습니다.") ;
			WINDOWMGR->MsgBox( MBI_IDENTIFICATION_ALREADY_HAVE, MBT_OK, CHATMGR->GetChatMsg( 830 ) );

			// 080122 LYW --- NpcScriptManager : npc 다이얼로그 닫기.
			cNpcScriptDialog* pNpcDlg = NULL ;
			pNpcDlg = GAMEIN->GetNpcScriptDialog() ;
			
			if( pNpcDlg )
			{
				pNpcDlg->SetActive(FALSE) ;
			}
		}
		else																			// 발급 받지 않았다면,
		{
			CIdentification* pIdentificationDlg = GAMEIN->GetIdentificationDlg() ;		// 주민등록 발급창 정보를 받는다.

			if(pIdentificationDlg)														// 발급창 정보가 유효하면,
			{
				pIdentificationDlg->SettingControls() ;									// 컨트롤 정보들을 세팅한다.

				if( !pIdentificationDlg->IsActive() )									// 발급창이 열려있지 않으면,
				{
					pIdentificationDlg->SetActive(TRUE) ;								// 발급창을 연다.
				}
			}

			CFavorIcon* pFavorIconDlg = GAMEIN->GetFavorIconDlg() ;						// 성향 창 정보를 받느다.

			if( pFavorIconDlg )															// 성향 창 정보가 유효하면,
			{
				if( !pFavorIconDlg->IsActive() )										// 성향창이 열려있지 않으면,
				{
					pFavorIconDlg->SetActiveRecursive(TRUE) ;							// 성향창을 활성화 한다.
				}
			}

		}
	}	
}





//#include "stdafx.h"
//#include "NpcScriptManager.h"
//#include "cPage.h"
//#include "cDialogueList.h"
//#include "cHyperTextList.h"
//#include "GameIn.h"
//#include "Npc.h"
//#include "ObjectManager.h"
//#include "StorageDialog.h"
//#include "ChatManager.h"
//#include "WindowIDEnum.h"
//
//#include "NpcScriptDialog.h"
//#include "InventoryExDialog.h"
//#include "GuildLevelUpDialog.h"
//#include "GuildMarkDialog.h"
//#include "GuildWarehouseDialog.h"
//#include "GuildCreateDialog.h"
//#include "objectstatemanager.h"
//#include "WantedManager.h"
//#include "GuildManager.h"
//#include "BailDialog.h"
//#include "WantedDialog.h"
//#include "./Interface/cWindowManager.h"
//#include "cMsgBox.h"
//#include "ItemShopDialog.h"
//#include "MHMap.h"
//#include "MoveManager.h"
//// Guild Tournament
//#include "GTRegistDialog.h"
//#include "GTRegistcancelDialog.h"
//#include "GTBattleListDialog.h"
//// guildunion
//#include "GuildUnion.h"
//
//// seigewar
//#include "SeigeWarDialog.h"
//#include "SiegeWarMgr.h"
//
//// 070329 LYW --- NpcScriptManager : Include NpcImageDlg.
//#include "NpcImageDlg.h"
//#include "cSkillTrainingDlg.h"
//#include "FamilyManager.h"
//#include "FamilyCreateDialog.h"
//
//#include "../DateMatchingDlg.h"
//
//GLOBALTON(cNpcScriptManager)
//cNpcScriptManager::cNpcScriptManager()
//{
//	m_nRegistNpcCount = 0;
//	m_pDialogue = NULL;
//	m_pHyperText = NULL;
//
//	SetSelectedNpc(NULL);
//}
//
//cNpcScriptManager::~cNpcScriptManager()
//{
//	DeleteAllScriptInfo();
//	SAFE_DELETE( m_pDialogue );
//	SAFE_DELETE( m_pHyperText );
//}
//
//void cNpcScriptManager::Init()
//{
//	LoadNpcScriptInfo();
//
//	m_pDialogue = new cDialogueList;
//	m_pDialogue->LoadDialogueListFile(NPC_DIALOGUE_PATH, "rb");
//
//	m_pHyperText = new cHyperTextList;
//	m_pHyperText->LoadHyperTextFormFile(NPC_HYPERTEXT_PATH, "rb");
//}
//
//
//void cNpcScriptManager::AddNpcScriptInfo(DWORD dwNpcId, cPage* pPage)
//{
//	m_NpcScriptList[dwNpcId].AddTail(pPage);
//}
//
//void cNpcScriptManager::DeleteAllScriptInfo()
//{
//	for(int i=0;i<MAX_NPC_COUNT;++i)
//	{
//		PTRLISTSEARCHSTART(m_NpcScriptList[i],cPage*,pPage)
//			delete pPage;
//		PTRLISTSEARCHEND
//			
//		m_NpcScriptList[i].RemoveAll();
//	}
//}
//
//cPage* cNpcScriptManager::GetMainPage( DWORD dwNpcId )
//{
//	if( m_NpcScriptList[dwNpcId].IsEmpty() ) return NULL;
//
//	return (cPage*)m_NpcScriptList[dwNpcId].GetHead();
//}
//
//cPage* cNpcScriptManager::GetPage( DWORD dwNpcId, DWORD dwPageId )
//{
//	PTRLISTSEARCHSTART(m_NpcScriptList[dwNpcId],cPage*,pPage)
//		if( pPage->GetPageId() == dwPageId )
//			return pPage;
//	PTRLISTSEARCHEND
//			
//	return NULL;
//}
//
//
//void cNpcScriptManager::LoadNpcScriptInfo()
//{
//	CMHFile fp;
//	if(!fp.Init(NPC_SCRIPT_PATH, "rb"))
//		return;
//
//	char buff[256]={0,};
//
//	while(1)
//	{
//		fp.GetString(buff);
//		if( fp.IsEOF() )
//			break;
//		if(buff[0] == '@')
//		{
//			fp.GetLineX(buff, 256);
//			continue;
//		}
//
//		CMD_ST(buff)
//		CMD_CS("$NPC")
//			if((fp.GetString())[0] == '{')
//			{
//				LoadPageInfo(NULL, &fp);
//				++m_nRegistNpcCount;
//			}
//			else
//				__asm int 3;
//		CMD_EN
//	}
//}
//
//void cNpcScriptManager::LoadPageInfo(cPage* pPage, CMHFile* fp)
//{
//	// 다시한번 확인 및 주석 처리요!!
//	char buff[256]={0,};
//
//	static DWORD dwNpcId; // Npc에대한 Page정보가 완료 될때까지 유지 가능해야 한다.
//	DWORD dwPageId;
//	int nDialogueCount = 0;
//	int nHyperLinkCount = 0;
//	BYTE emotion = 0;
//	DWORD dwDialogueId;
//	HYPERLINK sHyper;
//
//	while(1)
//	{
//		fp->GetString(buff);
//		if(buff[0] == '}'|| fp->IsEOF())
//			break;
//		if(buff[0] == '@')
//		{
//			fp->GetLineX(buff, 256);
//			continue;
//		}
//		CMD_ST(buff)
//		CMD_CS("$PAGE")
//			if((fp->GetString())[0] == '{')
//			{
//				cPage* pNewPage = new cPage; // 새로운 페이지의 등록
//				AddNpcScriptInfo( dwNpcId, pNewPage );
//				LoadPageInfo(pNewPage, fp);
//			}
//		CMD_CS("#NPCID")
//			dwNpcId = fp->GetDword();
//		CMD_CS("#PAGEINFO")
//			dwPageId = fp->GetDword();
//			nDialogueCount = fp->GetInt();
//			nHyperLinkCount = fp->GetInt();
//			emotion = fp->GetInt();
//			pPage->Init( dwPageId, emotion );
//		CMD_CS("#DIALOGUE")
//			for(int i =0; i<nDialogueCount;++i)
//			{
//				dwDialogueId = fp->GetDword();
//				pPage->AddDialogue( dwDialogueId );
//			}
//		CMD_CS("#HYPERLINK")
//			sHyper.wLinkId = fp->GetWord();
//			sHyper.wLinkType = fp->GetWord()+1;
////			if( sHyper.wLinkType == emLink_Page || sHyper.wLinkType == emLink_Open 
////				|| sHyper.wLinkType == emLink_Quest )
//				sHyper.dwData = fp->GetDword();
//			pPage->AddHyperLink( &sHyper );
//		CMD_EN
//	}
//}
//
//void cNpcScriptManager::StartNpcScript( CNpc* pNpc )
//{
//	// 기존에 존재하면 기존것을 삭제하고 새로 연다.
//	if( m_pCurSelectedNpc )
//	{
//		GAMEIN->GetNpcScriptDialog()->EndDialog();
//		SetSelectedNpc(NULL);
//	}
//
//	VECTOR3 pos;
//
//	HERO->GetPosition( &pos );
//
//	// 070612 LYWE --- NpcScriptManager : Check board.
//	if( pNpc->GetNpcKind() != 37 )
//	MOVEMGR->SetLookatPos(pNpc,&pos,0,gCurTime);
//
//	SetSelectedNpc(pNpc);
//	// 070502 LYW --- NpcScriptManager : Modified open dialog of npc script dialog.
//	//GAMEIN->GetNpcScriptDialog()->OpenDialog( pNpc->GetNpcUniqueIdx() );
//	GAMEIN->GetNpcScriptDialog()->OpenDialog( pNpc->GetNpcUniqueIdx(), pNpc->GetNpcListInfo()->ModelNum );
//}
//
//void cNpcScriptManager::StartNpcBusiness(DWORD dwData, DWORD dwParam)
//{
//	if( m_pCurSelectedNpc == NULL && dwParam == 0 ) 
//	{
//		ASSERT(0);
//
//		if( HERO->GetState() == eObjectState_Deal )
//			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
//		
//		return;
//	}
//
//	if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == DEALER_ROLE)
//	{
//		MSG_WORD msg;
//		msg.Category = MP_ITEM;
//		msg.Protocol = MP_ITEM_DEALER_SYN;
//		msg.dwObjectID = HEROID;
//		msg.wData = m_pCurSelectedNpc->GetNpcUniqueIdx();
//		NETWORK->Send(&msg, sizeof(msg));
//
//		GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
//	}
//	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == CASTLE_ROLE )
//	{
//		switch( dwData )
//		{
//		case 0:		// 세율관리
//			{
//				if( HERO->GetGuildIdx() == 0 || HERO->GetGuildMemberRank() != GUILD_MASTER )
//				{
//					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(323) );
//					return;
//				}
//				SWPROFIT->SendProfitInfoSyn();
//			}
//			break;
//		case 1:
//		case 2:
//		case 3:		// 외성문
//		case 4:
//		case 5:
//		case 6:		// 내성문
//			{	
//				if( HERO->GetGuildIdx() == 0 || HERO->GetGuildMemberRank() != GUILD_MASTER )
//				{
//					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(323) );
//					return;
//				}
//				DWORD dwMoney = 0;
//				switch( dwData )
//				{
//				case 1:		dwMoney = 1000000;		break;
//				case 2:		dwMoney = 2000000;		break;
//				case 3:		dwMoney = 3000000;		break;
//				case 4:		dwMoney = 1500000;		break;
//				case 5:		dwMoney = 2500000;		break;				
//				case 6:		dwMoney = 4000000;		break;
//				}
//				if( HERO->GetMoney() < dwMoney )
//				{
//					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(324) );
//					return;
//				}
//				MSG_DWORD Msg;
//				Msg.Category = MP_SIEGEWAR;
//				Msg.Protocol = MP_SIEGEWAR_UPGRADE_GATE_SYN;
//				Msg.dwObjectID = HEROID;
//				Msg.dwData = dwData;
//				NETWORK->Send( &Msg, sizeof(Msg) );
//				return;
//			}
//			break;
//		}		
//	}
//	else if( (m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == CHANGGO_ROLE) || dwParam == eNpcParam_ShowpStorage )
//	{
//		if(dwData == 0) //창고 보기
//		{	
//			if(GAMEIN->GetStorageDialog()->IsItemInit())
//			{
//				if( !GAMEIN->GetInventoryDialog()->IsActive() )
//				// 070326 LYW --- GlobalEventFunc : Modified active function.
//				//GAMEIN->GetStorageDialog()->SetActive(TRUE);
//				//GAMEIN->GetInventoryDialog()->SetActive(TRUE);
//				GAMEIN->GetStorageDialog()->ShowStorageDlg(TRUE) ;
//				if(HERO->GetStorageNum() == 0)
//				{
//
//					GAMEIN->GetStorageDialog()->ShowStorageMode(eStorageMode_NoWare);
//
//					GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
//				}
//				else
//				{
//					GAMEIN->GetStorageDialog()->ShowStorageMode(eStorageMode_StorageWare1);
//					GAMEIN->GetStorageDialog()->AddStorageMode(eStorageMode_StorageWare1);
//
//					GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
//				}
//			}
//			else
//			{
//				if(HERO->GetStorageNum() == 0)
//				{
//					if( !GAMEIN->GetInventoryDialog()->IsActive() )
//					// 070326 LYW --- GlobalEventFunc : Modified active function.
//					//GAMEIN->GetStorageDialog()->SetActive(TRUE);
//					//GAMEIN->GetInventoryDialog()->SetActive(TRUE);
//					GAMEIN->GetStorageDialog()->ShowStorageDlg(TRUE) ;
//					GAMEIN->GetStorageDialog()->ShowStorageMode(eStorageMode_NoWare);
//
//					GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
//				}
//				else
//				{
//					// 070325 LYW --- NpcScriptManager : Setting to true for test.
//					MAP->SetVillage(TRUE) ;
//					if( dwParam == eNpcParam_ShowpStorage || MAP->IsVillage() == TRUE )
//					{						
//						MSG_WORD2 msg;
//						msg.Category = MP_ITEM;
//						msg.Protocol = MP_ITEM_STORAGE_ITEM_INFO_SYN;
//						msg.dwObjectID = HEROID;						
//						dwParam==eNpcParam_ShowpStorage ? msg.wData1=79 : msg.wData1=m_pCurSelectedNpc->GetNpcUniqueIdx();
//						msg.wData2 = eNpcParam_ShowpStorage;
//						NETWORK->Send(&msg, sizeof(msg));
//					}
//					else
//						return;
//				}
//			}
//
//			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);
//		}	
//		else if(dwData == 1) //가격표/구입
//		{
//			GAMEIN->GetStorageDialog()->SetStorageListInfo();
//			// 070326 LYW --- GlobalEventFunc : Modified active function.
//			//GAMEIN->GetStorageDialog()->SetActive(TRUE);
//			GAMEIN->GetStorageDialog()->ShowStorageDlg(TRUE) ;		
//
//			GAMEIN->GetStorageDialog()->ShowStorageMode(eStorageMode_StorageListInfo);
//			GAMEIN->GetStorageDialog()->AddStorageMode(eStorageMode_StorageListInfo);
//
//			GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
//		}
//		else if(dwData == 2)
//		{
//			if(HERO->GetGuildIdx() == 0)
//			{
//				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));
//
//				// 070326 LYW --- NpcScriptManager : Add windowbox.
//				WINDOWMGR->MsgBox( MBI_NOTJOIN_GUILD, MBT_OK, CHATMGR->GetChatMsg(325)) ;
//				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
//				return;
//			}
//			if(GUILDMGR->GetGuildLevel() < GUILD_3LEVEL)
//			{
//				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(326));
//
//				// 070326 LYW --- NpcScriptManager : Add windowbox.
//				WINDOWMGR->MsgBox( MBI_BUY_COMPLETE, MBT_OK, CHATMGR->GetChatMsg(326)) ;
//				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
//				return;
//			}
//			if(GUILDMGR->CanUseWarehouse() == FALSE)
//				return;
//			GAMEIN->GetGuildWarehouseDlg()->SetActive(TRUE);	
//		}
//		else if(dwData == 3)			// 아이템몰창고
//		{
//			MSGBASE msg;
//			msg.Category = MP_ITEM;
//			msg.Protocol = MP_ITEM_SHOPITEM_INFO_SYN;
//			msg.dwObjectID = HERO->GetID();
//			NETWORK->Send(&msg, sizeof(msg));
//		}
//	}
//	//SW060920 문파배틀시 창고소환주문서 사용불가로 인한 위치 수정.// NpcJob을 초기화하지 않기에 위치변경으로 처리.
//	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == MUNPA_ROLE)
//	{
//		switch( dwData )
//		{
//		case 0:
//			{
//				if(HERO->GetGuildIdx())
//				{
//					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(327));
//					GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
//					return;
//				}
//				if(GUILDMGR->CanEntryGuild(HERO->GetGuildEntryDate()) == FALSE)
//				{
//					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(59), HERO->GetGuildEntryDate());
//					GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
//					return;
//				}
//				cDialog* pDlg = (cDialog*)GAMEIN->GetGuildCreateDlg();
//				pDlg->SetActive(TRUE);
//				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
//			}
//			break;
//		case 1:
//			{
//				if(HERO->GetGuildIdx() == 0)
//				{
//					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));
//					GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
//					return;
//				}
//				else if(HERO->GetGuildMemberRank() == GUILD_MASTER)
//				{
//					WINDOWMGR->MsgBox( MBI_GUILD_BREAKUP, MBT_YESNO, CHATMGR->GetChatMsg( 329 ) );
//					GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
//				}
//				else
//				{
//					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(323));
//					GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
//					return;
//				}
//			}
//			break;
//		case 2:
//			{
//				if(HERO->GetGuildIdx() == 0)
//				{
//					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));
//					GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
//					return;
//				}
//				else if(HERO->GetGuildMemberRank() != GUILD_MASTER)
//				{
//					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(323));
//					GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
//					return;
//				}
//				else
//				{
//					GAMEIN->GetGuildLevelUpDlg()->SetActive(TRUE);
//					GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
//				}	
//			}
//			break;
//		case 3:
//			{
//				if(GUILDMGR->GetGuildLevel() < GUILD_2LEVEL)
//				{
//					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(326));
//					return;
//				}
//				if(HERO->GetGuildIdx() && (HERO->GetGuildMemberRank() == GUILD_MASTER))
//				{
//					GAMEIN->GetGuildMarkDlg()->ShowGuildMark();
//				}
//				else
//				{
//					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(330));
//					return;
//				}	
//			}
//			break;
//		case 4:		// create guildunion
//			{
//				if( GUILDUNION->CheckGuildUnionCondition( 0 ) )
//					GAMEIN->GetGuildUnionCreateDlg()->SetActive( TRUE );
//				else
//					return;
//			}
//			break;
//		case 5:		// destroy guildunion
//			{
//				if( GUILDUNION->CheckGuildUnionCondition( 1 ) )
//					// 070125 LYW --- NPCChat : Modified message number.
//					//WINDOWMGR->MsgBox( MBI_GUILD_UNION_DESTROY, MBT_YESNO, CHATMGR->GetChatMsg( 1126 ) );
//					WINDOWMGR->MsgBox( MBI_GUILD_UNION_DESTROY, MBT_YESNO, CHATMGR->GetChatMsg( 539 ) );
//				else
//					return;
//			}
//			break;
//		case 6:		// regist guildunion mark
//			{
//				if( GUILDUNION->CheckGuildUnionCondition( 5 ) )
//					GAMEIN->GetGuildMarkDlg()->ShowGuildUnionMark();
//				else
//					return;
//			}
//			break;
//		case 7:		// seigewar time regist
//			{
//				if(HERO->GetGuildIdx() == 0)
//				{
//					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));
//					return;
//				}
//				else if(HERO->GetGuildMemberRank() != GUILD_MASTER)
//				{
//					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(323));
//					return;
//				}
//				GAMEIN->GetSWTimeRegDlg()->SetActive( TRUE );
//			}
//			break;
//		case 8:		// seigewar protectregist
//			{
//				if(HERO->GetGuildIdx() == 0)
//				{
//					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));
//					return;
//				}
//				else if(HERO->GetGuildMemberRank() != GUILD_MASTER)
//				{
//					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(323));
//					return;
//				}
//
//				MSGBASE Msg;
//				Msg.Category = MP_SIEGEWAR;
//				Msg.Protocol = MP_SIEGEWAR_PROPOSALLIST_SYN;
//				Msg.dwObjectID = HEROID;
//				NETWORK->Send( &Msg, sizeof(Msg) );
//			}
//			break;
//		case 9:		// 공성전 관전하기
//			{	
//				WINDOWMGR->MsgBox( MBI_SW_OBSERVER_SYN, MBT_YESNO, CHATMGR->GetChatMsg(332) );
//				return;
//			}
//			break;
//		case 10:	// seigewar info
//			{
//				MSGBASE Msg;
//				Msg.Category = MP_SIEGEWAR;
//				Msg.Protocol = MP_SIEGEWAR_GUILDLIST_SYN;
//				Msg.dwObjectID = HEROID;
//				NETWORK->Send( &Msg, sizeof(Msg) );
//			}
//			break;
//		case 11:	// enter deffense team
//			{
//				if(HERO->GetGuildIdx() == 0)
//				{
//					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));
//					return;
//				}
//				
//				MSG_DWORD2 Msg;
//				Msg.Category = MP_SIEGEWAR;
//				Msg.Protocol =  MP_SIEGEWAR_MOVEIN_SYN;
//				Msg.dwObjectID = HEROID;
//				Msg.dwData1 = HERO->GetGuildIdx();
//				Msg.dwData2 = 0;
//				NETWORK->Send( &Msg, sizeof(Msg) );		
//				
//				return;
//			}
//			break;
//		case 12:	// enter offense team
//			{
//				if(HERO->GetGuildIdx() == 0)
//				{
//					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));
//					return;
//				}
//
//				MSG_DWORD2 Msg;
//				Msg.Category = MP_SIEGEWAR;
//				Msg.Protocol =  MP_SIEGEWAR_MOVEIN_SYN;
//				Msg.dwObjectID = HEROID;
//				Msg.dwData1 = HERO->GetGuildIdx();
//				Msg.dwData2 = 1;
//				NETWORK->Send( &Msg, sizeof(Msg) );	
//				
//				return;
//			}
//			break;
//		case 13:	// 수성 참여신청
//			{
//				if(HERO->GetGuildIdx() == 0)
//				{
//					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));
//					return;
//				}
//				else if(HERO->GetGuildMemberRank() != GUILD_MASTER)
//				{
//					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(323));
//					return;
//				}
//				
//				WINDOWMGR->MsgBox( MBI_SW_PROPOSAL_SYN, MBT_YESNO, CHATMGR->GetChatMsg(333) );
//				return;
//			}
//			break;
//		case 14:	// 공성 참여신청
//			{
//				if(HERO->GetGuildIdx() == 0)
//				{
//					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));
//					return;
//				}
//				else if(HERO->GetGuildMemberRank() != GUILD_MASTER)
//				{
//					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(323));
//					return;
//				}
//
//				WINDOWMGR->MsgBox( MBI_SW_ATTACKREGIST_SYN, MBT_YESNO, CHATMGR->GetChatMsg(334) );		
//				return;
//			}
//			break;
//		case 15:
//		case 16:
//		case 17:
//		case 18:
//		case 19:
//			{
//				if(HERO->GetGuildIdx() == 0)
//				{
//					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));
//					return;
//				}
//				if(HERO->GetGuildMemberRank() != GUILD_MASTER)
//				{
//					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(326));
//					return;
//				}
//
//				if( GUILDMGR->CheckChangeLocation( dwData ) )
//				{
//					WINDOWMGR->MsgBox( MBI_GUILD_CHANGE_LOCATION, MBT_YESNO, CHATMGR->GetChatMsg(335), GetMapName( GUILDMGR->GetTempLocation() ) );
////					return;
//				}
//				else
//					return;
//			}
//			break;
//		case 20:
//			{
//				MSGBASE Msg;
//				Msg.Category = MP_SIEGEWAR;
//				Msg.Protocol = MP_SIEGEWAR_TIMEINFO_SYN;
//				Msg.dwObjectID = HEROID;
//				NETWORK->Send( &Msg, sizeof(Msg) );				
//			}
//			break;
//		}
//	}
//	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == WANTED_ROLE)
//	{
//		//전에 클릭한게 늦게올 수도 있네
//		if(GAMEIN->GetWantedDialog()->IsActive() == TRUE)
//			return;
//		if(dwData == 0) //창고 보기
//			WANTEDMGR->SortWantedListSyn(1);
//		else if(dwData == 1)
//			GAMEIN->GetBailDialog()->Open();
//		//
//	}
//	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == SURYUN_ROLE)
//	{
//		if( dwData == eSuryunNpc_Suryun )		// 수련하기
//		{
//		}
//		else if( dwData == eSuryunNpc_About_GuildTournament )		// 문파토너먼트 알아보기
//		{
//			if( HERO->GetState() == eObjectState_Deal )
//				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
//		}
//		else if( dwData == eSuryunNpc_Regist_GuildTournament )		// 문파토너먼트 참가신청하기
//		{
//			MSGBASE msg;
//			msg.Category = MP_GTOURNAMENT;
//			msg.Protocol = MP_GTOURNAMENT_REGISTCOUNT;
//			msg.dwObjectID = HEROID;
//			NETWORK->Send( &msg, sizeof(msg) );
//		}
//		else if( dwData == eSuryunNpc_Cancel_GuildTournament )		// 문파토너먼트 신청 취소하기
//		{
//			if( GAMEIN->GetGTRegistcancelDlg() )
//				GAMEIN->GetGTRegistcancelDlg()->SetActive( TRUE );
//		}
//		else if( dwData == eSuryunNpc_Show_GuildStand )				// 문파토너먼트 대진표 보기
//		{
//			MSG_WORD msg;
//			msg.Category = MP_GTOURNAMENT;
//			msg.Protocol = MP_GTOURNAMENT_STANDINGINFO_SYN;
//			msg.wData = 1;
//			msg.dwObjectID = HEROID;
//			NETWORK->Send( &msg, sizeof(msg) );
//		}
//		else if( dwData == eSuryunNpc_Enter_GuildTournament )		// 문파토너먼트 입장하기
//		{
//			MSG_DWORD msg;
//			msg.Category = MP_GTOURNAMENT;
//			msg.Protocol = MP_GTOURNAMENT_MOVETOBATTLEMAP_SYN;
//			msg.dwObjectID = HEROID;
//			msg.dwData = 0;
//			NETWORK->Send( &msg, sizeof(msg) );
//		}
//		else if( dwData == eSuryunNpc_EnterObserver_GuildTournament )	// 관람모드로 문파토너먼트 입장하기
//		{
//			GAMEIN->SetGuildIdx( HERO->GetGuildIdx() );
//
//			MSGBASE msg;
//			msg.Category = MP_GTOURNAMENT;
//			msg.Protocol = MP_GTOURNAMENT_BATTLEINFO_SYN;
//			msg.dwObjectID = HEROID;
//			NETWORK->Send( &msg, sizeof(msg) );
//			
//			//if( GAMEIN->GetGTBattleListDlg() )
//			//	GAMEIN->GetGTBattleListDlg()->SetActive( TRUE );
//		}
//		else if( dwData == eSuryunNpc_SkillTraining )
//		{
//			if(GAMEIN->GetNpcScriptDialog()->IsActive())
//			{
//				NPCSCRIPTMGR->SetSelectedNpc(NULL);
//				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);						
//			}
//
//			GAMEIN->GetSkillTrainingDlg()->OpenDialog();
//
//			return;
//		}
//	}
//	else if(m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == MAPCHANGE_ROLE )
//	{
//		if(dwData == 0)
//		{
//			MSG_DWORD msg;
//			msg.Category = MP_NPC;
//			msg.Protocol = MP_NPC_DOJOB_SYN;
//			msg.dwObjectID = HEROID;
//			msg.dwData = m_pCurSelectedNpc->GetID();
//			NETWORK->Send(&msg, sizeof(msg));
//		}
//		else
//		{
//			if(GAMEIN->GetNpcScriptDialog()->IsActive())
//			{
//				NPCSCRIPTMGR->SetSelectedNpc(NULL);
//				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);						
//			}
//			
//			if( HERO->GetState() == eObjectState_Deal )
//				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
//		}
//	}
//	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == SYMBOL_ROLE )
//	{
//		// 각인시작		
//		if( HERO->GetGuildMemberRank() == GUILD_MASTER )
//		{
//			CGuild* pGuild = GUILDMGR->GetGuild();
//			if( pGuild && pGuild->GetLevel() == 5 )
//			{
//				MSG_DWORD msg;
//				SetProtocol( &msg, MP_SIEGEWAR, MP_SIEGEWAR_ENGRAVE_SYN );
//				msg.dwObjectID = HEROID;
//				msg.dwData = HERO->GetGuildIdx();
//				NETWORK->Send( &msg, sizeof(msg) );
//			}
//			else
//			{
//				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(336) );
//			}
//		}
//		else
//		{
//			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(337) );
//		}
//	}
//	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == FAMILY_ROLE )
//	{
//		if( dwData == 0 )
//		{
//			if(HERO->GetFamilyIdx())
//			{
//				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(327));
//				return;
//			}
//			if(FAMILYMGR->CanEntryFamily(HERO->GetFamilyEntryDate()) == FALSE)
//			{
//				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(59), HERO->GetFamilyEntryDate());
//				return;
//			}
//			if(GAMEIN->GetNpcScriptDialog()->IsActive())
//			{
//				NPCSCRIPTMGR->SetSelectedNpc(NULL);
//				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);						
//			}
//
//			GAMEIN->GetFamilyCreateDlg()->SetActive(TRUE);
//		}
//		else if( dwData == 1 )
//		{
//			if(HERO->GetFamilyIdx() == 0)
//			{
//				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));
//				return;
//			}
//			else if(HERO->GetFamilyMemberRank() == FAMILY_MASTER)
//			{
//				if(GAMEIN->GetNpcScriptDialog()->IsActive())
//				{
//					NPCSCRIPTMGR->SetSelectedNpc(NULL);
//					GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);						
//				}				
//				WINDOWMGR->MsgBox( MBI_FAMILY_BREAKUP, MBT_YESNO, CHATMGR->GetChatMsg( 329 ) );
//			}
//			else
//			{
//				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(323));
//				return;
//			}
//		}
//		else if(dwData == 2 )
//		{
//			const DATE_MATCHING_INFO& dateInfo = HERO->GetDateMatchingInfo() ;
//
//			if( &dateInfo )
//			{
//				if( dateInfo.bIsValid )
//				{
//					// notice error.
//					WINDOWMGR->MsgBox( MBI_IDENTIFICATION_ALREADY_HAVE, MBT_OK, "이미 주민등록증을 발급 받았습니다.") ;
//				}
//				else
//				{
//					// open identification dialog.
//					CIdentification* pIdentificationDlg = GAMEIN->GetIdentificationDlg() ;
//
//					if(pIdentificationDlg)
//					{
//						//// setting some info. id, gender.
//						pIdentificationDlg->SettingControls() ;
//
//						// active dialog.
//						if( !pIdentificationDlg->IsActive() )
//						{
//							pIdentificationDlg->SetActive(TRUE) ;
//						}
//					}
//
//					// open favor icon dialog.
//					CFavorIcon* pFavorIconDlg = GAMEIN->GetFavorIconDlg() ;
//
//					if( pFavorIconDlg )
//					{
//						if( !pFavorIconDlg->IsActive() )
//						{
//							//pFavorIconDlg->SetActive(TRUE) ;
//							pFavorIconDlg->SetActiveRecursive(TRUE) ;
//						}
//					}
//
//				}
//			}
//		}
//	}
//	// 070501 LYW --- NpcScriptManager : Check GUILD_ROLE.
//	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == MUNPA_ROLE )
//	{
//		if( dwData == 0 )
//		{
//			if(HERO->GetGuildIdx())
//			{
//				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(327));
//				return;
//			}
//			if(GUILDMGR->CanEntryGuild(HERO->GetGuildEntryDate()) == FALSE)
//			{
//				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(59), HERO->GetGuildEntryDate());
//				return;
//			}
//			if(GAMEIN->GetNpcScriptDialog()->IsActive())
//			{
//				NPCSCRIPTMGR->SetSelectedNpc(NULL);
//				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);						
//			}
//
//			GAMEIN->GetGuildCreateDlg()->SetActive(TRUE);
//		}
//		else if( dwData == 1 )
//		{
//			if(HERO->GetGuildIdx() == 0)
//			{
//				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));
//				return;
//			}
//			else if(HERO->GetGuildMemberRank() == FAMILY_MASTER)
//			{
//				if(GAMEIN->GetNpcScriptDialog()->IsActive())
//				{
//					NPCSCRIPTMGR->SetSelectedNpc(NULL);
//					GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);						
//				}				
//				WINDOWMGR->MsgBox( MBI_FAMILY_BREAKUP, MBT_YESNO, CHATMGR->GetChatMsg( 329 ) );
//			}
//			else
//			{
//				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(323));
//				return;
//			}
//		}
//	}
//	// 070605 LYW --- NpcScriptManager : Add identification npc.
//	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == IDENTIFICATION_ROLE)
//	{
//		const DATE_MATCHING_INFO& dateInfo = HERO->GetDateMatchingInfo() ;
//
//		if( &dateInfo )
//		{
//			if( dateInfo.bIsValid )
//			{
//				// notice error.
//				WINDOWMGR->MsgBox( MBI_IDENTIFICATION_ALREADY_HAVE, MBT_OK, "이미 주민등록증을 발급 받았습니다.") ;
//			}
//			else
//			{
//				// open identification dialog.
//				CIdentification* pIdentificationDlg = GAMEIN->GetIdentificationDlg() ;
//
//				if(pIdentificationDlg)
//				{
//					//// setting some info. id, gender.
//					//pIdentificationDlg->GetNickName()->SetStaticText(HERO->GetObjectName()) ;
//
//					//// setting gender.
//					//if(dateInfo.byGender == 0)
//					//{
//					//	pIdentificationDlg->GetGender()->SetStaticText("남") ;
//					//}
//					//else
//					//{
//					//	pIdentificationDlg->GetGender()->SetStaticText("여") ;
//					//}
//
//					pIdentificationDlg->SettingControls() ;
//
//					// active dialog.
//					if( !pIdentificationDlg->IsActive() )
//					{
//						pIdentificationDlg->SetActive(TRUE) ;
//					}
//				}
//
//				// open favor icon dialog.
//				CFavorIcon* pFavorIconDlg = GAMEIN->GetFavorIconDlg() ;
//
//				if( pFavorIconDlg )
//				{
//					if( !pFavorIconDlg->IsActive() )
//					{
//						//pFavorIconDlg->SetActive(TRUE) ;
//						pFavorIconDlg->SetActiveRecursive(TRUE) ;
//					}
//				}
//
//			}
//		}		
//		
//		/*MSGBASE msg ;
//		msg.Category = MP_DATE ;
//		msg.Protocol = MP_DATEMATCHING_CREATE_IDENTIFICATION_SYN ;
//		msg.dwObjectID = HEROID ;
//
//		NETWORK->Send(&msg, sizeof(msg)) ;*/
//	}
//	else
//	{
//		ASSERT(0);
//		if( HERO->GetState() == eObjectState_Deal )
//			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
//		return;
//	}
//
//	GAMEIN->GetNpcScriptDialog()->DisActiveEx();
////	GAMEIN->GetNpcScriptDialog()->EndDialog();
//}
