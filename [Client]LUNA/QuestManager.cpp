// QuestManager.cpp: implementation of the CQuestManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"																		// stdafx ����� �ҷ��´�.
#include "QuestManager.h"																// ����Ʈ �Ŵ��� ����� �ҷ��´�.
#include "./Interface/GameNotifyManager.h"												// ���� ���� �Ŵ��� Ŭ���� ����� �ҷ��´�.

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include "MHFile.h"																		// ���� ���� ����� �ҷ��´�.

#include "..\[CC]Quest\QuestScriptLoader.h"												// ����Ʈ ��ũ��Ʈ �δ� ����� �ҷ��´�.
#include "..\[CC]Quest\QuestString.h"													// ����Ʈ ��Ʈ�� ����� �ҷ��´�.
#include "..\[CC]Quest\QuestInfo.h"														// ����Ʈ ���� ����� �ҷ��´�.
#include "..\[CC]Quest\SubQuestInfo.h"													// ���� ����Ʈ ���� ����� �ҷ��´�.
#include "..\[CC]Quest\QuestExecute.h"													// ����Ʈ ���� Ŭ���� ����� �ҷ��´�.
#include "..\[CC]Quest\QuestNpc.h"														// ����Ʈ npc ����� �ҷ��´�.
#include "..\[CC]Quest\QuestNpcData.h"													// ����Ʈ npc ������ ����� �ҷ��´�.
#include "..\[CC]Quest\QuestNpcScript.h"												// ����Ʈ npc ��ũ��Ʈ Ŭ���� ����� �ҷ��´�.
#include "..\[CC]Quest\QuestNpcScriptData.h"											// ����Ʈ npc ��ũ��Ʈ ������ Ŭ���� ����� �ҷ��´�.

#include "Quest.h"																		// ����Ʈ ����� �ҷ��´�.
#include "QuestUpdater.h"																// ����Ʈ �������� ����� �ҷ��´�.
#include "ObjectManager.h"																// ������Ʈ �Ŵ��� ����� �ҷ��´�.
#include "ChatManager.h"																// ä�� �Ŵ��� ����� �ҷ��´�.
#include "QuestItemManager.h"															// ����Ʈ ������ �Ŵ��� ����� �ҷ��´�.
#include "UserInfoManager.h"

//
#include "GameIn.h"																		// ���� �� ����� �ҷ��´�.
#include "ItemManager.h"																// ������ �Ŵ��� ����� �ҷ��´�.
#include "InventoryExDialog.h"															// �κ��丮 ���̾�α� ����� �ҷ��´�.
#include "WindowIdEnum.h"																// ������ ���̵� �̳� ����� �ҷ��´�.
#include "./Interface/cWindowManager.h"													// ������ �Ŵ��� ����� �ҷ��´�.
#include "cDivideBox.h"																	// ������ �ڽ� ����� �ҷ��´�.
#include "cMsgBox.h"																	// �޽��� �ڽ� ����� �ҷ��´�.
#include "QuickManager.h"																// �� �Ŵ��� ����� �ҷ��´�.
#include "Npc.h"																		// npc ����� �ҷ��´�.
#include "MHMap.h"																		// ���� �� ����� �ҷ��´�.

#include "MiniMapDlg.h"																	// �̴ϸ� ���̾�α� ����� �ҷ��´�.

// 061220 LYW --- Delete this code.
//#include "MainBarDialog.h"
#include "QuestDialog.h"																// ����Ʈ ���̾�α� ����� �ҷ��´�.
#include "QuestQuickViewDialog.h"														// ����Ʈ �˸��� ���̾�α� ũ���� ����� �ҷ��´�.
#include "StatusIconDlg.h"																// ���� ������ ���̾�α� ����� �ҷ��´�.
#include "NpcScriptDialog.h"															// npc ��ũ��Ʈ ���̾�α� ����� �ҷ��´�.

// 070329 LYW --- ItemManager : Include NpcImageDlg.
#include "NpcImageDlg.h"																// npc �̹��� ���̾�α� ����� �ҷ��´�.

#include "QuestTotalDialog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CQuestManager);																// ����Ʈ �Ŵ����� �۷ι� ������ ��.

CQuestManager::CQuestManager()															// ������ �Լ�.
{
	m_QuestInfoTable.Initialize(MAX_QUEST);												// �ִ� ����Ʈ ����ŭ(200) ����Ʈ ���� ���̺��� �ʱ�ȭ �Ѵ�.
	m_QuestStringTable.Initialize(200);													// �ִ� ����Ʈ ����ŭ(200) ����Ʈ ��Ʈ�� ���̺��� �ʱ�ȭ �Ѵ�.
	m_QuestTable.Initialize(MAX_QUEST);													// �ִ� ����Ʈ ����ŭ(200) ����Ʈ ���̺��� �ʱ�ȭ �Ѵ�.
	m_QuestNpcTable.Initialize(100);													// 100�� ��ŭ ����Ʈ npc ���̺��� �ʱ�ȭ �Ѵ�.

	m_QuestUpdater = new CQuestUpdater;													// ����Ʈ �������͸� ���� �Ҵ��Ѵ�.

	memset( m_pNpcArray, 0, sizeof(CNpc*)*32 );											// npc ������ 32��ŭ �ʱ�ȭ �Ѵ�.
	m_dwMaxNpc = 0;																		// �ִ� npc�� ���� ������ 0���� �ʱ�ȭ �Ѵ�.
	
	m_dwLastMsg = 0;																	// ������ �޽����� ���� ������ 0���� �ʱ�ȭ �Ѵ�.

	for( int count = 0 ; count < MAX_QUICKVIEW_QUEST ; ++count )					// ����Ʈ �˸��� ���̾�α׿� ��� ������ �� ��ŭ for���� ������.
	{
		m_QuickViewQuestID[count] = 0 ;											// ����Ʈ ���̵� �ʱ�ȭ �Ѵ�.
	}
}

CQuestManager::~CQuestManager()															// �Ҹ��� �Լ�.
{
	Release();																			// ���� �Լ��� ȣ���Ѵ�.
}

void CQuestManager::Release()															// ���� �Լ�.
{
	// quest info																		// ����Ʈ ���� ��Ʈ.
	CQuestInfo* pQuestInfo = NULL;														// ����Ʈ ������ ���� �����͸� �����ϰ�, null ó���� �Ѵ�.

	m_QuestInfoTable.SetPositionHead();													// ����Ʈ ���� ���̺��� ���� �����Ѵ�.
	while( pQuestInfo = m_QuestInfoTable.GetData() )									// ����Ʈ ������ �ִµ��� while���� ������.
	{
		delete pQuestInfo;																// ����Ʈ ������ �����Ѵ�.
	}
	m_QuestInfoTable.RemoveAll();														// ����Ʈ ���� ���̺��� ��� ����.

	//// quest string																		// ����Ʈ ��Ʈ�� ��Ʈ.
	//CQuestString* pQuestString = NULL;													// ����Ʈ ��Ʈ���� ���� �����͸� �����ϰ�, null ó���� �Ѵ�.
	//m_QuestStringTable.SetPositionHead();												// ����Ʈ ��Ʈ�� ���̺��� ���� �����Ѵ�.
	//while( pQuestString = m_QuestStringTable.GetData() )								// ����Ʈ ��Ʈ�� ������ �ִµ��� while���� ������.
	//{
	//	//delete pQuestString;
	//	SAFE_DELETE( pQuestString );													// �����ϰ� ����Ʈ ��Ʈ���� ���� �Ѵ�.
	//}
	//m_QuestStringTable.RemoveAll();														// ����Ʈ ��Ʈ�� ���̺��� ��� ����.

	// quest string																		// ����Ʈ ��Ʈ�� ��Ʈ.

	CQuestString* pQuestString = NULL;													// ����Ʈ ��Ʈ���� ���� �����͸� �����ϰ�, null ó���� �Ѵ�.
	//int nDeleteCount = 0 ;
	m_QuestStringTable.SetPositionHead();												// ����Ʈ ��Ʈ�� ���̺��� ���� �����Ѵ�.
	while( pQuestString = m_QuestStringTable.GetData() )								// ����Ʈ ��Ʈ�� ������ �ִµ��� while���� ������.
	{
		//m_QuestStringTable.Remove(nDeleteCount) ;

		delete pQuestString ;
		pQuestString = NULL ;
		//++nDeleteCount ;
	}
	m_QuestStringTable.RemoveAll();														// ����Ʈ ��Ʈ�� ���̺��� ��� ����.


	// quest																			// ����Ʈ ��Ʈ.
	CQuest* pQuest = NULL;																// ����Ʈ�� ���� �����͸� �����ϰ�, nulló���� �Ѵ�.
	m_QuestTable.SetPositionHead();														// ����Ʈ ���̺��� ���� �����Ѵ�.
	while( pQuest = m_QuestTable.GetData() )											// ����Ʈ�� �ִµ��� while���� ������.
	{
		delete pQuest;																	// ����Ʈ�� �����Ѵ�.
	}
	m_QuestTable.RemoveAll();															// ����Ʈ ���̺��� ��� ����.

	// quest npcinfo																	// ����Ʈ npc���� ��Ʈ.
	QUESTNPCINFO* pData = NULL;															// ����Ʈ npc������ ���� �����͸� �����ϰ�, null ó���� �Ѵ�.
	m_QuestNpcTable.SetPositionHead();													// ����Ʈ npc���̺��� ���� �����Ѵ�.
	while( pData = m_QuestNpcTable.GetData() )											// ����Ʈ npc ������ ���� ���� while���� ������.
	{
		delete pData;																	// ����Ʈ npc������ �����Ѵ�.
	}
	m_QuestNpcTable.RemoveAll();														// ����Ʈ npc ���̺��� ��� ����.

	// updater																			// �������� ��Ʈ.
	if( m_QuestUpdater )																// ����Ʈ �������� ������ ��ȿ�ϸ�,
	{
		delete m_QuestUpdater;															// ����Ʈ �������� �޸𸮸� �����Ѵ�.
		m_QuestUpdater = NULL;															// ����Ʈ �������� �����͸� nulló���� �Ѵ�.
	}

	m_dwLastMsg = 0;																	// ������ �޽��� ��ȣ�� 0���� �����Ѵ�.
}

BOOL CQuestManager::LoadQuestScript( char* strFileName )								// ����Ʈ ��ũ��Ʈ�� �ε��ϴ� �Լ�.
{
	CMHFile file;																		// ���� ������ �����Ѵ�.

	if( !file.Init( strFileName, "rb") )												// ���� �����̸���, ���� ��忡 ���� ������ ����, ����� �޴´�.
	{																					// �����ߴٸ�,
		return FALSE;																	// FALSE�� �����Ѵ�.
	}

	char Token[256];																	// �ӽ� ���۸� �����Ѵ�.

	DWORD dwQuestNum = 0;																// ����Ʈ ��ȣ�� ���� ������ ����, 0���� �ʱ�ȭ �Ѵ�.

	int nMapNum = 0 ;

	DWORD dwNpcIdx = 0 ;

	while( !file.IsEOF() )																// ������ ���� �ƴ� ���� while���� ������.
	{
		file.GetString( Token );														// ��Ʈ���� �о� �ӽ� ���ۿ� ��´�.

		if( strcmp(Token,"$QUEST") == 0 )												// �о���� ��Ʈ���� $QUEST�� ���ٸ�,
		{
			dwQuestNum = file.GetDword();												// ����Ʈ ��ȣ�� �о���δ�.

			nMapNum = file.GetInt() ;													// �� ��ȣ�� �о���δ�.

			dwNpcIdx = file.GetDword() ;												// npc �ε����� �о���δ�.

			CQuestInfo* pQuestInfo = CQuestScriptLoader::LoadQuestInfo( &file, dwQuestNum ) ;

			if( pQuestInfo )
			{
				pQuestInfo->SetMapNum(nMapNum) ;
				pQuestInfo->SetNpcIdx(dwNpcIdx) ;
			}

			m_QuestInfoTable.Add( pQuestInfo, dwQuestNum );	// ����Ʈ ���� ���̺� ����Ʈ�� ��´�.
		}
	}

	CreateAllQuestFromScript();															// ��ũ��Ʈ�� ���� ��� ����Ʈ�� �����Ѵ�.

	return TRUE;																		// TRUE�� �����Ѵ�.
}

//BOOL CQuestManager::LoadQuestString(char* strFileName)									// ����Ʈ ��Ʈ���� �ε��ϴ� �Լ�.
//{
//	CMHFile file;																		// ���� ������ �����Ѵ�.
//
//	if( !file.Init( strFileName, "rb") )												// ���� �����̸���, ���� ��忡 ���� ������ ����, ����� �޴´�.
//	{																					// �����ߴٸ�,
//		return FALSE;																	// FALSE�� �����Ѵ�.
//	}
//
//	char Token[256];																	// �ӽ� ���۸� �����Ѵ�.
//	
//	while( !file.IsEOF() )																// ������ ���� �ƴѵ��� while���� ������.
//	{
//		file.GetString(Token);															// ��Ʈ���� �о� �ӽ� ���ۿ� ��´�.
//
//		if(strcmp(Token,"$SUBQUESTSTR") == 0)											// �о���� ��Ʈ���� $SUBQUESTSTR�� ���ٸ�,
//		{
//			DWORD Key_1 = file.GetDword();												// Ű1�� �о���δ�.
//			DWORD Key_2 = file.GetDword();												// Ű2�� �о���δ�.
//			DWORD Key;																	// Ű ������ �����Ѵ�.
//
//			COMBINEKEY(Key_1, Key_2, Key);												// Ű ������ Ű1, Ű2�� �����Ͽ� �޴´�.
//
//			CQuestString* pQuestString = new CQuestString ;
//
//			CQuestScriptLoader::LoadQuestString(&file, pQuestString);	// ����Ʈ ��Ʈ���� �ε��Ͽ� ����Ʈ ��Ʈ�� �����ͷ� �޴´�.
//
//			ASSERT( pQuestString && "Why is it null?" );								// ����Ʈ ��Ʈ�� ������ ��ȿ�ؾ� �Ѵ�. assertó��.
//			ASSERT( ! m_QuestStringTable.GetData( Key ) && "This memory'll be overwrite. It's reason of memory leak" );	// ����Ʈ ��Ʈ�� ���̺��� �˻��Ѵ�.
//			
//			m_QuestStringTable.Add(pQuestString, Key);									// ����Ʈ ��Ʈ�� ���̺� ����Ʈ ��Ʈ�� ������ ��´�.
//			pQuestString->SetIndex(Key_1, Key_2);										// ����Ʈ ��Ʈ���� Ű1, Ű2�� �ε����� �����Ѵ�.
//		}
//	}
//
//	//
//	m_QuestItemMGR.QuestItemInfoLoad();													// ����Ʈ ������ �Ŵ����� ����Ʈ ������ ������ �ε��Ѵ�.
//
//	return TRUE;																		// TRUE�� �����Ѵ�.
//}

BOOL CQuestManager::LoadQuestString(char* strFileName)									// ����Ʈ ��Ʈ���� �ε��ϴ� �Լ�.
{
	CMHFile file;																		// ���� ������ �����Ѵ�.

	if( !file.Init( strFileName, "rb") )												// ���� �����̸���, ���� ��忡 ���� ������ ����, ����� �޴´�.
	{																					// �����ߴٸ�,
		return FALSE;																	// FALSE�� �����Ѵ�.
	}

	char Token[256];																	// �ӽ� ���۸� �����Ѵ�.
	
	while( !file.IsEOF() )																// ������ ���� �ƴѵ��� while���� ������.
	{
		file.GetString(Token);															// ��Ʈ���� �о� �ӽ� ���ۿ� ��´�.

		if(strcmp(Token,"$SUBQUESTSTR") == 0)											// �о���� ��Ʈ���� $SUBQUESTSTR�� ���ٸ�,
		{
			DWORD Key_1 = file.GetDword();												// Ű1�� �о���δ�.
			DWORD Key_2 = file.GetDword();												// Ű2�� �о���δ�.
			DWORD Key;																	// Ű ������ �����Ѵ�.

			COMBINEKEY(Key_1, Key_2, Key);												// Ű ������ Ű1, Ű2�� �����Ͽ� �޴´�.
			CQuestString* pQuestString = CQuestScriptLoader::LoadQuestString(&file);	// ����Ʈ ��Ʈ���� �ε��Ͽ� ����Ʈ ��Ʈ�� �����ͷ� �޴´�.

			ASSERT( pQuestString && "Why is it null?" );								// ����Ʈ ��Ʈ�� ������ ��ȿ�ؾ� �Ѵ�. assertó��.
			ASSERT( ! m_QuestStringTable.GetData( Key ) && "This memory'll be overwrite. It's reason of memory leak" );	// ����Ʈ ��Ʈ�� ���̺��� �˻��Ѵ�.
			
			m_QuestStringTable.Add(pQuestString, Key);									// ����Ʈ ��Ʈ�� ���̺� ����Ʈ ��Ʈ�� ������ ��´�.
			pQuestString->SetIndex(Key_1, Key_2);										// ����Ʈ ��Ʈ���� Ű1, Ű2�� �ε����� �����Ѵ�.
		}
	}

	//
	m_QuestItemMGR.QuestItemInfoLoad();													// ����Ʈ ������ �Ŵ����� ����Ʈ ������ ������ �ε��Ѵ�.

	return TRUE;																		// TRUE�� �����Ѵ�.
}

//void CQuestManager::LoadNewbieGuide( char* strFileName )								// �ʺ��� ���̵带 �ε��ϴ� �Լ�.
//{
//	m_NewbieGuide.LoadNewbieGuide( strFileName );										// ���� ���� �̸�����, �ʺ��� ���̵带 �ε��Ѵ�.
//}

void CQuestManager::LoadQuestNpc( char* strFileName )									// ����Ʈ npc�� �ε��ϴ� �Լ�.
{
	CMHFile file;																		// ���� ������ �����Ѵ�.

	if( !file.Init( strFileName, "rb") )												// ���� �����̸���, ���� ��忡 ���� ������ ����, ����� �޴´�.
	{																					// �����ߴٸ�,
		return ;																		// ���� ó���� �Ѵ�.
	}
	
	while( !file.IsEOF() )																// ������ ���� �ƴѵ��� while���� ������.
	{
		QUESTNPCINFO* pData = new QUESTNPCINFO;											// ����Ʈ npc������ ���� �޸𸮸� �Ҵ��ϰ�, �����ͷ� �޴´�.
		pData->dwObjIdx = 0;															// ������Ʈ �ε����� 0���� �����Ѵ�.
		pData->wMapNum = file.GetWord();												// �� ��ȣ�� �޴´�.
		pData->wNpcKind = file.GetWord();												// npc ������ �޴´�.
//		file.GetString( pData->sName );
		SafeStrCpy( pData->sName, file.GetString(), MAX_NAME_LENGTH+1 );				// npc �̸��� �޴´�.
		pData->wNpcIdx = file.GetWord();												// npc �ε����� �޴´�.
		pData->vPos.x = file.GetFloat();												// npc ��ġ�� �޴´�.
		pData->vPos.y = 0;
		pData->vPos.z = file.GetFloat();
		pData->fDir = file.GetFloat();													// npc ����?�� �޴´�.

		m_QuestNpcTable.Add( pData, pData->wNpcIdx );									// npc ���̺� npc������ ��´�.
	}
}

void CQuestManager::CreateAllQuestFromScript()											// ��ũ��Ʈ�� ���� ��� ����Ʈ�� �����ϴ� �Լ�.
{
	CQuestInfo* pQuestInfo = NULL;														// ����Ʈ ������ ���� �����͸� �����Ѱ� nulló���� �Ѵ�.

	m_QuestInfoTable.SetPositionHead();													// ����Ʈ ���� ���̺��� ���� �����Ѵ�.

	while( pQuestInfo = m_QuestInfoTable.GetData() )									// ����Ʈ ������ �ִ� ���� while���� ������.
	{
		CQuest* pQuest = new CQuest( pQuestInfo );										// ����Ʈ ������ ���� ����Ʈ �޸𸮸� �Ҵ��ϰ�, ����Ʈ �����ͷ� �޴´�.
		m_QuestTable.Add( pQuest, pQuestInfo->GetQuestIdx() );							// ����Ʈ ���̺� �߰��� �Ѵ�.
	}
}

void CQuestManager::InitializeQuest()													// ����Ʈ�� �ʱ�ȭ �ϴ� �Լ�.
{
	CQuest* pQuest = NULL;																// ����Ʈ ������ ���� �����͸� ���� ��, NULL ó���� �Ѵ�.

	m_QuestTable.SetPositionHead();														// ����Ʈ ���̺��� ���� �����Ѵ�.

	while( pQuest = m_QuestTable.GetData() )											// ����Ʈ ������ �ִµ��� while���� ������.
	{
		pQuest->Initialize();															// ����Ʈ�� �ʱ�ȭ �Ѵ�.
	}
	m_dwMaxNpc = 0;																		// �ִ� npc ������ 0���� �����Ѵ�.

	m_dwLastMsg = 0;																	// ������ �޽����� 0���� �����Ѵ�.
}

void CQuestManager::SetMainQuestData( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime, BYTE bCheckType, DWORD dwCheckTime )
{
	if( dwQuestIdx == 0 )																// ����Ʈ �ε����� 0�� ���ٸ�, // Newbieguide
	{
		//m_NewbieGuide.SetFlagData( dwSubQuestFlag, dwData, dwTime );					// �ʺ��� ���̵��� �÷��� �����͸� �����Ѵ�.
		return;																			// ���� ó���� �Ѵ�.
	}
	else																				// ����Ʈ �ε����� 0�� �ƴ϶��,
	{
		CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );							// ����Ʈ ���̺��� ����Ʈ �ε����� ����Ʈ ������ �޴´�.

		if( !pQuest ) return ;															// ����Ʈ ������ ��ȿ�ϴٸ�,

		pQuest->SetMainQuestData( dwSubQuestFlag, dwData, dwTime, bCheckType, dwCheckTime );	// ���� ����Ʈ �����͸� �����Ѵ�.

		if( pQuest->IsQuestComplete() ) return ;
		m_QuestDialog->StartQuest(dwQuestIdx) ;
	}
}

//// ���� ����Ʈ �����͸� �����ϴ� �Լ�.
//void CQuestManager::SetMainQuestData( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime, BYTE bCheckType, DWORD dwCheckTime )
//{
//	if( dwQuestIdx == 0 )																// ����Ʈ �ε����� 0�� ���ٸ�, // Newbieguide
//	{
//		m_NewbieGuide.SetFlagData( dwSubQuestFlag, dwData, dwTime );					// �ʺ��� ���̵��� �÷��� �����͸� �����Ѵ�.
//		return;																			// ���� ó���� �Ѵ�.
//	}
//	else																				// ����Ʈ �ε����� 0�� �ƴ϶��,
//	{
//		CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );							// ����Ʈ ���̺��� ����Ʈ �ε����� ����Ʈ ������ �޴´�.
//
//		if( pQuest )																	// ����Ʈ ������ ��ȿ�ϴٸ�,
//		{
//			pQuest->SetMainQuestData( dwSubQuestFlag, dwData, dwTime, bCheckType, dwCheckTime );	// ���� ����Ʈ �����͸� �����Ѵ�.
//
//			if( dwData == 0 )
//			{
//				CQuestInfo* pQuestInfo = GetQuestInfo(dwQuestIdx) ;						// ����Ʈ �ε����� ����Ʈ ������ �޴´�.
//
//				int nMapNum = pQuestInfo->GetMapNum() ;									// ����Ʈ ������ �ִ� �� ��ȣ�� �޴´�.
//
//				AddMapTree(nMapNum) ;													// ����Ʈ�� ������ �� ��ȣ�� ���� Ʈ���� �߰��Ѵ�.
//
//				AddQuestTitle(nMapNum, dwQuestIdx) ;									// ����Ʈ ������ ����Ʈ ����Ʈ�� �߰��Ѵ�.
//
//				CQuestDialog* pDlg = NULL ;																		// ����Ʈ ���̾�α� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//				pDlg = GAMEIN->GetQuestTotalDialog()->GetQuestDlg() ;											// ����Ʈ ���̾�α� ������ �޴´�.
//
//				if( pDlg )																						// ����Ʈ ���̾�α� ������ ��ȿ�ϴٸ�,
//				{
//					pDlg->SetQuestCount(pDlg->GetQuestCount()+1) ;														// ����Ʈ ���̾�α��� ����Ʈ ������ �����Ѵ�.
//
//					pDlg->QuestListReset() ;											// ����Ʈ ����Ʈ�� ���ΰ�ħ�Ѵ�.
//				}
//			}
//			
//			//if(pQuest->GetEndParam() == 0)												// ����Ʈ ���� �Ķ���Ͱ� 0�� ���ٸ�,
//			//{
//			//	DWORD Key = 0;															// Ű ������ �����ϰ� 0���� �����Ѵ�.
//			//	COMBINEKEY(dwQuestIdx, 0, Key);											// ����Ʈ�ε����� 0�� �����Ͽ� Ű���� �޴´�.
//			//	m_QuestUpdater->ProcessQuestAdd(m_QuestStringTable.GetData(Key));		// ����Ʈ �������Ϳ� �����͸� �߰��Ѵ�.
//			//}
//		}
//	}
//}

//// ���� ����Ʈ �����͸� �����ϴ� �Լ�.
//void CQuestManager::SetMainQuestData( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime, BYTE bCheckType, DWORD dwCheckTime )
//{
//	if( dwQuestIdx == 0 )																// ����Ʈ �ε����� 0�� ���ٸ�, // Newbieguide
//	{
//		m_NewbieGuide.SetFlagData( dwSubQuestFlag, dwData, dwTime );					// �ʺ��� ���̵��� �÷��� �����͸� �����Ѵ�.
//		return;																			// ���� ó���� �Ѵ�.
//	}
//	else																				// ����Ʈ �ε����� 0�� �ƴ϶��,
//	{
//		CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );							// ����Ʈ ���̺��� ����Ʈ �ε����� ����Ʈ ������ �޴´�.
//
//		if( pQuest )																	// ����Ʈ ������ ��ȿ�ϴٸ�,
//		{
//			pQuest->SetMainQuestData( dwSubQuestFlag, dwData, dwTime, bCheckType, dwCheckTime );	// ���� ����Ʈ �����͸� �����Ѵ�.
//
//			if( dwData == 0 )
//			{
//				CQuestInfo* pQuestInfo = GetQuestInfo(dwQuestIdx) ;						// ����Ʈ �ε����� ����Ʈ ������ �޴´�.
//
//				int nMapNum = pQuestInfo->GetMapNum() ;									// ����Ʈ ������ �ִ� �� ��ȣ�� �޴´�.
//
//				AddMapTree(nMapNum) ;													// ����Ʈ�� ������ �� ��ȣ�� ���� Ʈ���� �߰��Ѵ�.
//
//				AddQuestTitle(nMapNum, dwQuestIdx) ;									// ����Ʈ ������ ����Ʈ ����Ʈ�� �߰��Ѵ�.
//
//				CQuestDialog* pDlg = NULL ;																		// ����Ʈ ���̾�α� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//				pDlg = GAMEIN->GetQuestTotalDialog()->GetQuestDlg() ;											// ����Ʈ ���̾�α� ������ �޴´�.
//
//				if( pDlg )																						// ����Ʈ ���̾�α� ������ ��ȿ�ϴٸ�,
//				{
//					pDlg->SetQuestCount(pDlg->GetQuestCount()+1) ;														// ����Ʈ ���̾�α��� ����Ʈ ������ �����Ѵ�.
//
//					pDlg->QuestListReset() ;											// ����Ʈ ����Ʈ�� ���ΰ�ħ�Ѵ�.
//				}
//			}
//			
//			//if(pQuest->GetEndParam() == 0)												// ����Ʈ ���� �Ķ���Ͱ� 0�� ���ٸ�,
//			//{
//			//	DWORD Key = 0;															// Ű ������ �����ϰ� 0���� �����Ѵ�.
//			//	COMBINEKEY(dwQuestIdx, 0, Key);											// ����Ʈ�ε����� 0�� �����Ͽ� Ű���� �޴´�.
//			//	m_QuestUpdater->ProcessQuestAdd(m_QuestStringTable.GetData(Key));		// ����Ʈ �������Ϳ� �����͸� �߰��Ѵ�.
//			//}
//		}
//	}
//}

// ���� ����Ʈ �����͸� �����ϴ� �Լ�.
void CQuestManager::SetSubQuestData( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwData, DWORD dwTime )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// ����Ʈ ���̺��� ���� ����Ʈ �ε����� ����Ʈ ������ �޴´�.

	if( !pQuest ) return ;																// ����Ʈ ������ ��ȿ���� üũ�Ѵ�.

	pQuest->SetSubQuestData( dwSubQuestIdx, dwData, dwTime );						// ����Ʈ�� ���� ����Ʈ�� �����Ѵ�.

	AddQuestNpc( pQuest->GetSubQuestInfo( dwSubQuestIdx ) );						// ����Ʈ npc�� �߰��Ѵ�.

	m_QuestDialog->RefreshQuestList() ;
}

//// ���� ����Ʈ �����͸� �����ϴ� �Լ�.
//void CQuestManager::SetSubQuestData( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwData, DWORD dwTime )
//{
//	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// ����Ʈ ���̺��� ���� ����Ʈ �ε����� ����Ʈ ������ �޴´�.
//
//	if( pQuest )																		// ����Ʈ ������ ��ȿ���� üũ�Ѵ�.
//	{
//		pQuest->SetSubQuestData( dwSubQuestIdx, dwData, dwTime );						// ����Ʈ�� ���� ����Ʈ�� �����Ѵ�.
//
//		AddQuestNpc( pQuest->GetSubQuestInfo( dwSubQuestIdx ) );						// ����Ʈ npc�� �߰��Ѵ�.
//
//		//
//		int Key = 0;																	// Ű ������ �����ϰ� 0���μ����Ѵ�.
//		COMBINEKEY( dwQuestIdx, dwSubQuestIdx, Key );									// ����Ʈ �ε����� ��������Ʈ �ε����� �����Ͽ�, Ű���� �޴´�.
//		m_QuestUpdater->ProcessQuestAdd( m_QuestStringTable.GetData(Key) );				// ����Ʈ �������Ϳ� �����͸� �߰��Ѵ�.
//	}
//}
void CQuestManager::AddMapTree(int nMapNum)												// �� Ʈ���� �߰��ϴ� �Լ�.
{	
	cPtrList* pTreeList = NULL ;														// ����Ʈ Ʈ�� ����Ʈ�� ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.

	CQuestTotalDialog * pTotalDialog = GAMEIN->GetQuestTotalDialog() ;					// ����Ʈ ��Ż ���̾�α� ������ �޴´�.

	if( pTotalDialog )																	// ��Ż ���̾�α� ������ ��ȿ�ϸ�,
	{
		CQuestDialog* pQuestDlg = pTotalDialog->GetQuestDlg() ;							// ����Ʈ ���̾�α� ������ �޴´�.

		if( pQuestDlg )																	// ����Ʈ ���̾�α� ������ ��ȿ�ϸ�,
		{
			pTreeList = pQuestDlg->GetTreeList() ;										// Ʈ�� ����Ʈ�� �޴´�.			

			if( pTreeList )																// Ʈ�� ����Ʈ ������ ��ȿ�ϴٸ�,
			{
				BOOL bResult = FALSE ;													// �˻� ����� ���� ������ �����ϰ� FALSE ó���� �Ѵ�.

				int nTreeCount = pTreeList->GetCount() ;								// Ʈ�� ī��Ʈ�� �޴´�.

				QuestTree* pTree = NULL ;												// Ʈ�� ������ ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.

				PTRLISTPOS pTreePos = NULL ;											// Ʈ���� ��ġ ������ ���� �����͸� �����ϰ� NULLó���� �Ѵ�.

				for( int count = 0 ; count < nTreeCount ; ++count )						// Ʈ�� ī��Ʈ ��ŭ FOR���� ������.
				{
					pTreePos = pTreeList->FindIndex(count) ;							// ī��Ʈ�� �ش��ϴ� ��ġ ������ �޴´�.

					if( pTreePos )														// Ʈ�� ��ġ ������ ��ȿ�ϴٸ�,
					{
						pTree = (QuestTree*)pTreeList->GetAt(pTreePos) ;				// ��ġ�� �ش��ϴ� Ʈ�� ������ �޴´�.

						if( pTree )														// Ʈ�� ������ ��ȿ�ϴٸ�,
						{
							if( pTree->nMapNum == nMapNum )								// ���� ����Ʈ�� �� ��ȣ�� Ʈ���� �� ��ȣ�� ���ؼ� ������,
							{
								bResult = TRUE ;										// ����� TRUE�� �����Ѵ�.

								break ;													// for���� ����������.
							}
						}
					}
				}

				if( bResult == FALSE )													// ����� FALSE�� ������,
				{
					//QuestTree* pData = new QuestTree;									// Ʈ���� �����Ѵ�.

					//CQuestString* pQuestString = new CQuestString;						// Ʈ���� ���� ����Ʈ ��Ʈ���� �����Ѵ�.

					//int nTitleLine = 0 ;												// Ÿ��Ʋ ������ ���� ������ �����ϰ� 0���� �����Ѵ�.
					//pQuestString->AddLine(GetMapName(nMapNum), nTitleLine, TRUE) ;		// ����Ʈ ��Ʈ���� �� �̸��� Ÿ��Ʋ�� �߰��Ѵ�.
					//pQuestString->SetIndex(nMapNum, 0);									// Ʈ�� �ε����� �� ��ȣ�� 0���� �����Ѵ�.
					//pQuestString->SetTreeTitle(TRUE) ;									// Ʈ���� �������� �����Ѵ�.
					//pData->list.AddHead(pQuestString);									// ���� ������ Ʈ���� �� Ÿ��Ʋ�� �߰��Ѵ�.
					//++pData->Count;														// Ʈ���� ��Ʈ�� ī��Ʈ�� �����Ѵ�.
					//pData->State = eQTree_Open ;										// Ʈ���� ���¸� ���� ���·� �����Ѵ�.
					//pData->nMapNum = nMapNum ;											// Ʈ���� �� ��ȣ�� �����Ѵ�.

					////pTreeList->AddTail(pData);											// Ʈ���� ����Ʈ Ʈ�� ����Ʈ�� �߰��Ѵ�.
					//pQuestDlg->AddQuestTree(pData) ;
					//pQuestDlg->SetTreeCount( pQuestDlg->GetTreeCount() + 1 ) ;			// Ʈ�� ī��Ʈ�� �����Ѵ�.

					pQuestDlg->AddQuestTree(nMapNum) ;
				}
			}
		}
	}
}

void CQuestManager::DeleteMapTree(int nMapNum)
{
	cPtrList* pTreeList = NULL ;														// ����Ʈ Ʈ�� ����Ʈ�� ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.

	CQuestTotalDialog * pTotalDialog = GAMEIN->GetQuestTotalDialog() ;					// ����Ʈ ��Ż ���̾�α� ������ �޴´�.

	if( pTotalDialog )																	// ��Ż ���̾�α� ������ ��ȿ�ϸ�,
	{
		CQuestDialog* pQuestDlg = pTotalDialog->GetQuestDlg() ;							// ����Ʈ ���̾�α� ������ �޴´�.

		if( pQuestDlg )																	// ����Ʈ ���̾�α� ������ ��ȿ�ϸ�,
		{
			pTreeList = pQuestDlg->GetTreeList() ;										// Ʈ�� ����Ʈ�� �޴´�.			

			if( pTreeList )																// Ʈ�� ����Ʈ ������ ��ȿ�ϴٸ�,
			{
				int nTreeCount = pTreeList->GetCount() ;								// Ʈ�� ī��Ʈ�� �޴´�.

				QuestTree* pTree = NULL ;												// Ʈ�� ������ ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.

				PTRLISTPOS pTreePos = NULL ;											// Ʈ���� ��ġ ������ ���� �����͸� �����ϰ� NULLó���� �Ѵ�.

				for( int count = 0 ; count < nTreeCount ; ++count )						// Ʈ�� ī��Ʈ ��ŭ FOR���� ������.
				{
					pTreePos = pTreeList->FindIndex(count) ;							// ī��Ʈ�� �ش��ϴ� ��ġ ������ �޴´�.

					if( pTreePos )														// Ʈ�� ��ġ ������ ��ȿ�ϴٸ�,
					{
						pTree = (QuestTree*)pTreeList->GetAt(pTreePos) ;				// ��ġ�� �ش��ϴ� Ʈ�� ������ �޴´�.

						if( pTree )														// Ʈ�� ������ ��ȿ�ϴٸ�,
						{
							if( pTree->nMapNum == nMapNum )								// ���� ����Ʈ�� �� ��ȣ�� Ʈ���� �� ��ȣ�� ���ؼ� ������,
							{
								if( pTree->list.GetCount() == 0 )						// Ʈ���� ī��Ʈ�� 0���̸�,
								{
									pTreeList->RemoveAt(pTreePos) ;						// ���� Ʈ���� �����Ѵ�.
								}
							}
						}
					}
				}
			}
		}
	}
}

void CQuestManager::AddQuestTitle(int nMapNum, DWORD dwQuestIdx)						// ����Ʈ ���̾�α׿� ����Ʈ ������ �߰��ϴ� �Լ�.
{
	if( dwQuestIdx < 1 )																// ����Ʈ �ε����� 1���� ������,
	{
		return ;																		// ���� ó���� �Ѵ�.
	}

	int nKey = 0 ;																		// ��Ʈ�� Ű ���� ���� ������ �����ϰ� 0���� �����Ѵ�.

	COMBINEKEY(dwQuestIdx, 0, nKey) ;													// ����Ʈ �ε����� 0���� Ű ���� �����Ѵ�.
	
	CQuestString* pQuestString = NULL ;													// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pQuestString = m_QuestStringTable.GetData(nKey) ;									// �Ĺ����� Ű ������ ����Ʈ ��Ʈ���� �޴´�.

	if( !pQuestString )																	// ����Ʈ ��Ʈ�� ������ ��ȿ���� �ʴٸ�,
	{
		return ;																		// ����ó���� �Ѵ�.
	}

	CQuestDialog* pQuestDlg = NULL ;													// ����Ʈ ���̾�α� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
	pQuestDlg = GAMEIN->GetQuestDialog() ;											// ����Ʈ ���̾�α� ������ �޴´�.

	if( pQuestDlg )
	{
		pQuestDlg->AddQuestTitle(pQuestString, nMapNum) ;
	}

	//if( !pQuestDlg )
	//{	
	//	return ;																		// ���� ó���� �Ѵ�.
	//}

	//cPtrList* pTreeList = NULL ;														// ����Ʈ Ʈ�� ����Ʈ�� ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.
	//pTreeList = pQuestDlg->GetTreeList() ;												// Ʈ�� ����Ʈ�� �޴´�.			

	//if( !pTreeList )
	//{	
	//	return ;																		// ���� ó���� �Ѵ�.
	//}

	//BOOL bResult = FALSE ;																// �˻� ����� ���� ������ �����ϰ� FALSE ó���� �Ѵ�.

	//int nTreeCount = pTreeList->GetCount() ;											// Ʈ�� ī��Ʈ�� �޴´�.

	//QuestTree* pTree = NULL ;															// Ʈ�� ������ ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.
	//PTRLISTPOS pTreePos = NULL ;														// Ʈ���� ��ġ ������ ���� �����͸� �����ϰ� NULLó���� �Ѵ�.

	//for( int count = 0 ; count < nTreeCount ; ++count )									// Ʈ�� ī��Ʈ ��ŭ FOR���� ������.
	//{
	//	pTreePos = pTreeList->FindIndex(count) ;										// ī��Ʈ�� �ش��ϴ� ��ġ ������ �޴´�.

	//	if( pTreePos )																	// Ʈ�� ��ġ ������ ��ȿ�ϴٸ�,
	//	{
	//		pTree = (QuestTree*)pTreeList->GetAt(pTreePos) ;							// ��ġ�� �ش��ϴ� Ʈ�� ������ �޴´�.

	//		if( pTree )																	// Ʈ�� ������ ��ȿ�ϴٸ�,
	//		{
	//			if( pTree->nMapNum == nMapNum )											// ���� ����Ʈ�� �� ��ȣ�� Ʈ���� �� ��ȣ�� ���ؼ� ������,
	//			{
	//				pQuestString->SetTreeTitle(FALSE) ;									// Ʈ���� Ÿ��Ʋ�� �ƴ� ������ �����Ѵ�.
	//				pTree->list.AddTail(pQuestString) ;									// ���� ���� Ʈ���� ����Ʈ ������ �߰��Ѵ�.		

	//				break ;																// for���� ����������.
	//			}
	//		}
	//	}
	//}

	//if( bResult == FALSE )																// ����� FALSE�� ������,
	//{
	//	return ;																		// ���� ó���� �Ѵ�.
	//}
}

void CQuestManager::DeleteQuestTitle(int nMapNum, DWORD dwQuestIdx)						// ����Ʈ ����Ʈ���� ����Ʈ ������ �����ϴ� �Լ�.
{
	if( dwQuestIdx < 1 )																// ����Ʈ �ε����� 1���� ������,
	{
		ASSERTMSG(0, "CQuestManager::AddQuestTitle\n����Ʈ ������ �����ϴ� ��, ����Ʈ �ε����� 1���� ���� ���� �Ѿ�Դ�.!!!") ;	// ASSERT ���� ó��.

		return ;																		// ���� ó���� �Ѵ�.
	}

	int nKey = 0 ;																		// ��Ʈ�� Ű ���� ���� ������ �����ϰ� 0���� �����Ѵ�.

	COMBINEKEY(dwQuestIdx, 0, nKey) ;													// ����Ʈ �ε����� 0���� Ű ���� �����Ѵ�.
	
	CQuestString* pQuestString = NULL ;													// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pQuestString = m_QuestStringTable.GetData(nKey) ;									// �Ĺ����� Ű ������ ����Ʈ ��Ʈ���� �޴´�.

	if( !pQuestString )																	// ����Ʈ ��Ʈ�� ������ ��ȿ���� �ʴٸ�,
	{
		ASSERTMSG(0, "CQuestManager::AddQuestTitle\n����Ʈ ������ �����ϴ� �� ����Ʈ ��Ʈ�� ������ ��ȿ���� �ʴ�.") ;	// ASSERT ���� ó��.

		return ;																		// ����ó���� �Ѵ�.
	}

	CQuestTotalDialog * pTotalDialog = NULL ;											// ����Ʈ ��Ż ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pTotalDialog = GAMEIN->GetQuestTotalDialog() ;										// ����Ʈ ��Ż ���̾�α� ������ �޴´�.

	if( !pTotalDialog )
	{
		ASSERTMSG(0, "CQuestManager::AddQuestTitle\n����Ʈ ������ �����ϴ� �� ����Ʈ ��Ż ������ ��ȿ���� �ʴ�.") ;	// ASSERT ���� ó��.
		
		return ;																		// ���� ó���� �Ѵ�.
	}

	CQuestDialog* pQuestDlg = NULL ;													// ����Ʈ ���̾�α� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
	pQuestDlg = pTotalDialog->GetQuestDlg() ;											// ����Ʈ ���̾�α� ������ �޴´�.

	if( !pQuestDlg )
	{
		ASSERTMSG(0, "CQuestManager::AddQuestTitle\n����Ʈ ������ �����ϴ� �� ����Ʈ ���̾�α� ������ ���� ���ߴ�.") ;	// ASSERT ���� ó��.
		
		return ;																		// ���� ó���� �Ѵ�.
	}

	cPtrList* pTreeList = NULL ;														// ����Ʈ Ʈ�� ����Ʈ�� ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.
	pTreeList = pQuestDlg->GetTreeList() ;												// Ʈ�� ����Ʈ�� �޴´�.			

	if( !pTreeList )
	{
		ASSERTMSG(0, "CQuestManager::AddQuestTitle\n����Ʈ ������ �����ϴ� �� �� Ʈ�� ����Ʈ�� ���� ���ߴ�.") ;	// ASSERT ���� ó��.
		
		return ;																		// ���� ó���� �Ѵ�.
	}

	BOOL bResult = FALSE ;																// �˻� ����� ���� ������ �����ϰ� FALSE ó���� �Ѵ�.

	int nTreeCount = pTreeList->GetCount() ;											// Ʈ�� ī��Ʈ�� �޴´�.

	QuestTree* pTree = NULL ;															// Ʈ�� ������ ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.
	PTRLISTPOS pTreePos = NULL ;														// Ʈ���� ��ġ ������ ���� �����͸� �����ϰ� NULLó���� �Ѵ�.
	PTRLISTPOS pDeletePos = NULL ;														// ������ ����Ʈ ��Ʈ�� ��ġ�� ��Ÿ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.

	for( int count = 0 ; count < nTreeCount ; ++count )									// Ʈ�� ī��Ʈ ��ŭ FOR���� ������.
	{
		pTreePos = pTreeList->FindIndex(count) ;										// ī��Ʈ�� �ش��ϴ� ��ġ ������ �޴´�.

		if( pTreePos )																	// Ʈ�� ��ġ ������ ��ȿ�ϴٸ�,
		{
			pTree = (QuestTree*)pTreeList->GetAt(pTreePos) ;							// ��ġ�� �ش��ϴ� Ʈ�� ������ �޴´�.

			if( pTree )																	// Ʈ�� ������ ��ȿ�ϴٸ�,
			{
				if( pTree->nMapNum == nMapNum )											// ���� ����Ʈ�� �� ��ȣ�� Ʈ���� �� ��ȣ�� ���ؼ� ������,
				{
					int nQuestCount = pTree->list.GetCount() ;							// ����Ʈ ���� ī��Ʈ�� �޴´�.

					CQuestString* pDelete = NULL ;										// ���� �� ����Ʈ ��Ʈ���� ���� �����͸� �����ϰ� null ó���� �Ѵ�.

					for( int count = 0 ; count < nQuestCount ; ++count )				// ����Ʈ ���� �� ��ŭ for���� ������.
					{
						pDeletePos = pTree->list.FindIndex(count) ;						// ī��Ʈ�� �ش��ϴ� ���� ��ġ�� �޴´�.

						if( pDeletePos )												// ���� ��ġ ������ ��ȿ�ϸ�,
						{
							pDelete = (CQuestString*)pTree->list.GetAt(pDeletePos) ;	// ���� �� ��Ʈ�� ������ �޴´�.

							if( pDelete )												// ��Ʈ�� ������ ��ȿ�ϸ�,
							{
								if( pDelete->GetKey() == pQuestString->GetKey() )		// Ʈ���� ��Ʈ����, ������ ��Ʈ���� Ű ���� ������,
								{
									bResult = TRUE ;									// ��� ���� TRUE�� �����Ѵ�.

									//pTree->list.RemoveAt(pDeletePos) ;					// Ʈ������ ��Ʈ���� �����Ѵ�.
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

					break ;																// for���� ����������.
				}
			}
		}
	}

	if( bResult == FALSE )														// ����� FALSE�� ������,
	{
		ASSERT(0 && "CQuestManager::AddQuestTitle\n���� �ʿ� ����Ʈ�� �������� ����!!") ;

		return ;
	}
}


void CQuestManager::StartQuest( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// ����Ʈ ���̺��� ���� ����Ʈ �ε����� ����Ʈ ������ �޴´�.

	pQuest->StartQuest( dwSubQuestFlag, dwData, dwTime );								// ����Ʈ�� �����Ѵ�.

	m_QuestDialog->StartQuest(dwQuestIdx) ;

	int Key = 0;																	// Ű���� ���� ������ �����Ѱ�, 0���� �����Ѵ�.
	COMBINEKEY( dwQuestIdx, 0, Key );												// ����Ʈ �ε����� 0�� �����Ͽ� Ű ���� �޴´�.

	// title
	CQuestString* pQString = m_QuestStringTable.GetData(Key);						// Ű ������ ����Ʈ ��Ʈ�� ������ �޴´�.

	if( pQString == NULL )															// ����Ʈ ��Ʈ�� ������ ��ȿ���� �ʴٸ�,
	{
		return;																		// ���� ó���� �Ѵ�.
	}
	cPtrList* p = pQString->GetTitle();												// ����Ʈ �����͸� �����Ͽ�, ������ �޴´�.
	ITEM* string = (ITEM*)p->GetHead();										// ����Ʈ�� ��带 ����Ʈ ��Ʈ�� �����Ϳ� �޴´�.

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(221), string->string );			// ����Ʈ�� �����Ѵٴ� �޽����� ����Ѵ�.
}

void CQuestManager::EndQuest( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// ����Ʈ ���̺��� ���� ����Ʈ �ε����� ����Ʈ ������ �޴´�.

	if( !pQuest ) return ;

	if( pQuest->IsTimeCheck() )															// �ð� üũ�� �ʿ��� ����Ʈ���,
	{
		STATUSICONDLG->RemoveQuestTimeIcon( HERO, eStatusIcon_QuestTime );				// ����Ʈ �ð� �������� �����Ѵ�.
	}

	pQuest->EndQuest( dwSubQuestFlag, dwData, dwTime );									// ����Ʈ�� �����Ѵ�.

	m_QuestDialog->EndQuest(dwQuestIdx) ;

	m_QuickView->DeleteQuest(dwQuestIdx) ;

	DWORD Key = 0;																	// Ű ���� ���� ������ �����Ѵ�.
	COMBINEKEY(dwQuestIdx, 0, Key);													// ����Ʈ �ε����� 0�� �����Ͽ� Ű���� �޴´�.

	// title
	CQuestString* pQString = m_QuestStringTable.GetData(Key);						// Ű ������ ����Ʈ ��Ʈ�� ���̺��� ��Ʈ�� ������ �޴´�.

	if( pQString == NULL )															// ����Ʈ ��Ʈ�� ������ ��ȿ���� üũ�Ѵ�.
	{
		return;																		// ������ ��ȿ���� ������ ���� ó���� �Ѵ�.
	}

	cPtrList* p = pQString->GetTitle();												// ����Ʈ �����ͷ� ������ �޴´�.

	ITEM* string = (ITEM*)p->GetHead();										// ����Ʈ�� ��带 �޴´�.

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(222), string->string );			// ����Ʈ �޽����� ä��â�� ����Ѵ�.
}

// ����Ʈ�� �����ϴ� �Լ�.
//void CQuestManager::StartQuest( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime )
//{
//	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// ����Ʈ ���̺��� ���� ����Ʈ �ε����� ����Ʈ ������ �޴´�.
//
//	CQuestInfo* pQuestInfo =NULL ;														// ����Ʈ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
//	pQuestInfo = GetQuestInfo(dwQuestIdx) ;												// ����Ʈ �ε����� ����Ʈ ������ �޴´�.
//
//	if(!pQuestInfo)																		// ����Ʈ ������ ��ȿ���� ������,
//	{
//		return  ;																		// ���� ó���� �Ѵ�.
//	}
//
//	int nMapNum = pQuestInfo->GetMapNum() ;												// ����Ʈ ������ �ִ� �� ��ȣ�� �޴´�.
//
//	if( pQuest )																		// ����Ʈ ������ ��ȿ���� üũ�Ѵ�.
//	{
//		pQuest->StartQuest( dwSubQuestFlag, dwData, dwTime );							// ����Ʈ�� �����Ѵ�.
//
//		AddMapTree(nMapNum) ;															// ����Ʈ�� ������ �� ��ȣ�� ���� Ʈ���� �߰��Ѵ�.
//
//		AddQuestTitle(nMapNum, dwQuestIdx) ;											// ����Ʈ ������ ����Ʈ ����Ʈ�� �߰��Ѵ�.
//
//		int Key = 0;																	// Ű���� ���� ������ �����Ѱ�, 0���� �����Ѵ�.
//		COMBINEKEY( dwQuestIdx, 0, Key );												// ����Ʈ �ε����� 0�� �����Ͽ� Ű ���� �޴´�.
//		m_QuestUpdater->ProcessQuestAdd( m_QuestStringTable.GetData(Key) );				// ����Ʈ �������Ϳ� ����Ʈ�� �߰��Ѵ�.
//
//		// title
//		CQuestString* pQString = m_QuestStringTable.GetData(Key);						// Ű ������ ����Ʈ ��Ʈ�� ������ �޴´�.
//
//		if( pQString == NULL )															// ����Ʈ ��Ʈ�� ������ ��ȿ���� �ʴٸ�,
//		{
//			return;																		// ���� ó���� �Ѵ�.
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
//		cPtrList* p = pQString->GetTitle();												// ����Ʈ �����͸� �����Ͽ�, ������ �޴´�.
//		ITEM* string = (ITEM*)p->GetHead();										// ����Ʈ�� ��带 ����Ʈ ��Ʈ�� �����Ϳ� �޴´�.
//
//		CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(221), string->string );			// ����Ʈ�� �����Ѵٴ� �޽����� ����Ѵ�.
//
//		//GAMENOTIFYMGR->StartQuest(string->Str) ;										// ����Ʈ ���� ������ ����.
//	}	
//}
//void CQuestManager::EndQuest( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime )
//{
//	CQuestInfo* pQuestInfo =NULL ;														// ����Ʈ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
//	pQuestInfo = GetQuestInfo(dwQuestIdx) ;												// ����Ʈ �ε����� ����Ʈ ������ �޴´�.
//
//	if(!pQuestInfo)																		// ����Ʈ ������ ��ȿ���� ������,
//	{
//		ASSERTMSG(0, "����Ʈ ������ �������� ����Ʈ ������ �ε����� ���Ͽ����ϴ�.!") ;	// ASSERT ���� üũ.
//		return  ;																		// ���� ó���� �Ѵ�.
//	}
//
//	int nMapNum = pQuestInfo->GetMapNum() ;												// ����Ʈ ������ �ִ� �� ��ȣ�� �޴´�.
//
//	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// ����Ʈ ���̺��� ���� ����Ʈ �ε����� ����Ʈ ������ �޴´�.
//
//	if( pQuest )
//	{
//		if( pQuest->IsTimeCheck() )														// �ð� üũ�� �ʿ��� ����Ʈ���,
//		{
//			STATUSICONDLG->RemoveQuestTimeIcon( HERO, eStatusIcon_QuestTime );			// ����Ʈ �ð� �������� �����Ѵ�.
//		}
//
//		pQuest->EndQuest( dwSubQuestFlag, dwData, dwTime );								// ����Ʈ�� �����Ѵ�.
//
//		CQuestQuickViewDialog* pQuickView = NULL ;														// ����Ʈ �˸��� ���̾�α� ������ ���� �����͸� �����ϰ� nulló�� �Ѵ�.
//		pQuickView = GAMEIN->GetQuestQuickViewDialog() ;												// ����Ʈ �˸��� ���̾�α� ������ �޴´�.
//
//		if( pQuickView )																				// ����Ʈ �˸��� ���̾�α� ������ ��ȿ�ϴٸ�,
//		{
//			pQuickView->DeleteQuest(dwQuestIdx) ;
//			//pQuickView->ResetQuickViewDialog() ;																// ����Ʈ �˸����� ���� ����Ʈ �����͸� �ε��Ѵ�.
//		}
//
//		DeleteQuestTitle(nMapNum, dwQuestIdx) ;											// ����Ʈ ����Ʈ���� ����Ʈ�� �����Ѵ�.
//
//		DeleteMapTree(nMapNum) ;
//
//		CQuestDialog* pDlg = GAMEIN->GetQuestTotalDialog()->GetQuestDlg() ;				// ����Ʈ ���̾�α׸� �޴´�.
//
//		if( pDlg )																		// ����Ʈ ���̾��� ������ ��ȿ�ϸ�,
//		{
//			pDlg->QuestListReset() ;													// ����Ʈ����Ʈ�� ���ΰ�ħ�Ѵ�.
//
//			int nQuestCount = 0 ;																		// ����Ʈ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
//			nQuestCount = pDlg->GetQuestCount() ;														// ����Ʈ ī��Ʈ�� �޴´�.
//
//			if( nQuestCount == 0 )																		// ����Ʈ ī��Ʈ�� 0�� ������,
//			{
//				pDlg->SetQuestCount( 0 ) ;																// ����Ʈ ī��Ʈ�� 0���� �����Ѵ�.
//			}
//			else																						// ����Ʈ ī��Ʈ�� 0�� ���� ������,
//			{
//				pDlg->SetQuestCount( nQuestCount-1 ) ;													// ����Ʈ ī��Ʈ�� �����Ѵ�.
//			}
//		}
//	}
//	else
//	{
//		ASSERTMSG(0, "����Ʈ ������ �������� ����Ʈ�� �ε����� ���Ͽ����ϴ�.!") ;		// ASSERT ���� üũ.
//		return ;
//	}
//
//
//	DWORD Key = 0;																	// Ű ���� ���� ������ �����Ѵ�.
//	COMBINEKEY(dwQuestIdx, 0, Key);													// ����Ʈ �ε����� 0�� �����Ͽ� Ű���� �޴´�.
//
//	// title
//	CQuestString* pQString = m_QuestStringTable.GetData(Key);						// Ű ������ ����Ʈ ��Ʈ�� ���̺��� ��Ʈ�� ������ �޴´�.
//
//	if( pQString == NULL )															// ����Ʈ ��Ʈ�� ������ ��ȿ���� üũ�Ѵ�.
//	{
//		return;																		// ������ ��ȿ���� ������ ���� ó���� �Ѵ�.
//	}
//
//	cPtrList* p = pQString->GetTitle();												// ����Ʈ �����ͷ� ������ �޴´�.
//
//	ITEM* string = (ITEM*)p->GetHead();										// ����Ʈ�� ��带 �޴´�.
//
//	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(222), string->string );			// ����Ʈ �޽����� ä��â�� ����Ѵ�.
//
//	//GAMENOTIFYMGR->EndQuest(string->Str) ;											// ����Ʈ ���� ������ ����Ѵ�.
//}

// ����Ʈ�� �����ϴ� �Լ�.
//void CQuestManager::EndQuest( DWORD dwQuestIdx, DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime )
//{
//	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// ���� ����Ʈ �ε����� ����Ʈ ���̺��� ����Ʈ ������ �޴´�.
//
//	if( pQuest )																		// ����Ʈ ������ ��ȿ���� üũ�Ѵ�.
//	{
//		// remove icon
//		if( pQuest->IsTimeCheck() )														// �ð� üũ�� �ʿ��� ����Ʈ���,
//		{
//			STATUSICONDLG->RemoveQuestTimeIcon( HERO, eStatusIcon_QuestTime );			// ����Ʈ �ð� �������� �����Ѵ�.
//		}
//
//		pQuest->EndQuest( dwSubQuestFlag, dwData, dwTime );								// ����Ʈ�� �����Ѵ�.
//
//		//
//		DWORD Key = 0;																	// Ű ���� ���� ������ �����Ѵ�.
//		COMBINEKEY(dwQuestIdx, 0, Key);													// ����Ʈ �ε����� 0�� �����Ͽ� Ű���� �޴´�.
//
//		if(dwSubQuestFlag == 0)															// ���� ����Ʈ �÷��װ� 0�� ������,
//		{
//			m_QuestUpdater->CompleteQuestDelete(m_QuestStringTable.GetData(Key));		// ����Ʈ �������ͷ� �Ϸ� ����Ʈ�� �����Ѵ�.
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
//		//stTIME stime;																	// �ð� ����ü�� �����Ѵ�.
//		//stime.value = dwTime;															// �ð��� ����Ѵ�.
//		//
//		//char buf[16] = { 0, };															// �ӽ� ���۸� �����Ѵ�.
//		//sprintf(buf, "20%02d.%02d.%02d", stime.GetYear(), stime.GetMonth(), stime.GetDay());	// �ð��� ���ۿ� �����Ѵ�.
//		//
//		//JOURNALINFO pItem;																// ���� ���� ����ü�� �����Ѵ�.
//		//pItem.Kind = eJournal_Quest;													// ������ eJournal_Quest�� �����Ѵ�.
//		//pItem.Param = dwQuestIdx;														// ����Ʈ �ε����� �����Ѵ�.
//		//pItem.Param_2 = 0;																// SubQuestIdx�� 0�� ���(��������Ʈ�� ���)
//		//pItem.Param_3 = 1;																// bCompleted
//		//strncpy(pItem.RegDate, buf, 11);												// �ð��� �����Ѵ�.
//		//m_QuestUpdater->JournalItemAdd(&pItem);											// ����Ʈ �������Ϳ� ���� �������� �߰��Ѵ�.
//		
//		
//		// title
//		CQuestString* pQString = m_QuestStringTable.GetData(Key);						// Ű ������ ����Ʈ ��Ʈ�� ���̺��� ��Ʈ�� ������ �޴´�.
//
//		if( pQString == NULL )															// ����Ʈ ��Ʈ�� ������ ��ȿ���� üũ�Ѵ�.
//		{
//			return;																		// ������ ��ȿ���� ������ ���� ó���� �Ѵ�.
//		}
//
//		cPtrList* p = pQString->GetTitle();												// ����Ʈ �����ͷ� ������ �޴´�.
//
//		QString* string = (QString*)p->GetHead();										// ����Ʈ�� ��带 �޴´�.
//
//		CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(222), string->Str );			// ����Ʈ �޽����� ä��â�� ����Ѵ�.
//	}
//}

// ���� ����Ʈ�� �����Ѵ�.

void CQuestManager::StartSubQuest( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwSubQuestFlag, DWORD dwSubData, DWORD dwSubTime )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// ����Ʈ ���̺��� �ε����� �ش��ϴ� ����Ʈ ������ �޴´�.

	if( !pQuest ) return ;																// ����Ʈ ������ ��ȿ���� üũ�Ѵ�.

	pQuest->StartSubQuest( dwSubQuestIdx, dwSubQuestFlag, dwSubData, dwSubTime );	// ���� ����Ʈ�� �����Ѵ�.
	AddQuestNpc( pQuest->GetSubQuestInfo( dwSubQuestIdx ) );						// ����Ʈ NPC�� �߰��Ѵ�.

	m_QuestDialog->RefreshQuestList() ;
}

//void CQuestManager::StartSubQuest( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwSubQuestFlag, DWORD dwSubData, DWORD dwSubTime )
//{
//	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// ����Ʈ ���̺��� �ε����� �ش��ϴ� ����Ʈ ������ �޴´�.
//
//	if( pQuest )																		// ����Ʈ ������ ��ȿ���� üũ�Ѵ�.
//	{
//		pQuest->StartSubQuest( dwSubQuestIdx, dwSubQuestFlag, dwSubData, dwSubTime );	// ���� ����Ʈ�� �����Ѵ�.
//		AddQuestNpc( pQuest->GetSubQuestInfo( dwSubQuestIdx ) );						// ����Ʈ NPC�� �߰��Ѵ�.
//
//		//CQuestQuickViewDialog* pQuickView = NULL ;														// ����Ʈ �˸��� ���̾�α� ������ ���� �����͸� �����ϰ� nulló�� �Ѵ�.
//		//pQuickView = GAMEIN->GetQuestQuickViewDialog() ;												// ����Ʈ �˸��� ���̾�α� ������ �޴´�.
//
//		//if( pQuickView )																				// ����Ʈ �˸��� ���̾�α� ������ ��ȿ�ϴٸ�,
//		//{
//		//	pQuickView->ResetQuickViewDialog() ;																// ����Ʈ �˸����� ���� ����Ʈ �����͸� �ε��Ѵ�.
//		//}
//
//		////
//		//int Key = 0;																	// Ű ���� ���� ������ �����Ѵ�.
//		//COMBINEKEY( dwQuestIdx, dwSubQuestIdx, Key );									// ����Ʈ �ε����� ���� ����Ʈ �ε����� �����Ͽ� Ű���� �޴´�.
//		//m_QuestUpdater->ProcessQuestAdd( m_QuestStringTable.GetData(Key) );				// ����Ʈ �������Ϳ� ����Ʈ�� �߰��Ѵ�.
//
//		//if( gCurTime - m_dwLastMsg > 10000 )											// 10�ʰ� �����ٸ�,
//		//{
//		//	m_dwLastMsg = gCurTime;														// ������ �ð��� ���� �ð��� ����ϰ�,
//		//	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(226));						// ����Ʈ �޽����� ä��â�� ����Ѵ�.
//		//}
//	}
//}

// ���� ����Ʈ�� �����Ѵ�.
void CQuestManager::EndSubQuest( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwSubQuestFlag, DWORD dwSubData, DWORD dwSubTime )
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// ����Ʈ �ε����� ����Ʈ ���̺��� ����Ʈ ������ �޴´�.

	if( !pQuest ) return ;																// ����Ʈ ������ ��ȿ���� üũ�Ѵ�.

	pQuest->EndSubQuest( dwSubQuestIdx, dwSubQuestFlag, dwSubData, dwSubTime );			// ���� ����Ʈ�� ���� �Ѵ�.

	m_QuestDialog->RefreshQuestList() ;
}

//// ���� ����Ʈ�� �����Ѵ�.
//void CQuestManager::EndSubQuest( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwSubQuestFlag, DWORD dwSubData, DWORD dwSubTime )
//{
//	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// ����Ʈ �ε����� ����Ʈ ���̺��� ����Ʈ ������ �޴´�.
//
//	if( pQuest )																		// ����Ʈ ������ ��ȿ���� üũ�Ѵ�.
//	{
//		pQuest->EndSubQuest( dwSubQuestIdx, dwSubQuestFlag, dwSubData, dwSubTime );		// ���� ����Ʈ�� ���� �Ѵ�.
//
//		////
//		//int Key = 0;																	// Ű ���� ���� ������ �����Ѵ�.
//		//COMBINEKEY( dwQuestIdx, dwSubQuestIdx, Key );									// ����Ʈ �ε����� ���� ����Ʈ �ε�����, Ű���� �޴´�.
//		//m_QuestUpdater->ProcessQuestDelete(m_QuestStringTable.GetData(Key));			// ����Ʈ �������Ϳ� ����Ʈ�� �߰��Ѵ�.
//	}
//
//	int QuestStringKey = 0 ;
//	COMBINEKEY(dwQuestIdx, dwSubQuestIdx, QuestStringKey) ;							// ����Ʈ, ���� ����Ʈ �ε����� ����Ʈ ��Ʈ���� ���Ѵ�.
//	CQuestString* pQString = GetQuestString(QuestStringKey);						// ���� ����Ʈ ��Ʈ���� �޴´�.
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
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// ����Ʈ �ε�����, ����Ʈ���̺��� ����Ʈ ������ �޴´�.

	if( !pQuest ) return ;																// ����Ʈ ������ ��ȿ���� üũ�Ѵ�.

	pQuest->UpdateSubQuestData( dwSubQuestIdx, dwMaxCount, dwSubData, dwSubTime );		// ���� ����Ʈ ������ ������Ʈ �Ѵ�.

	m_QuestDialog->RefreshQuestList() ;
}

//// ���� ����Ʈ �����͸� ������Ʈ �Ѵ�.
//void CQuestManager::UpdateSubQuestData( DWORD dwQuestIdx, DWORD dwSubQuestIdx, DWORD dwMaxCount, DWORD dwSubData, DWORD dwSubTime )
//{
//	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// ����Ʈ �ε�����, ����Ʈ���̺��� ����Ʈ ������ �޴´�.
//
//	if( pQuest )																		// ����Ʈ ������ ��ȿ���� üũ�Ѵ�.
//	{
//		pQuest->UpdateSubQuestData( dwSubQuestIdx, dwMaxCount, dwSubData, dwSubTime );	// ���� ����Ʈ ������ ������Ʈ �Ѵ�.
//
//		CQuestDialog* pDlg = GAMEIN->GetQuestTotalDialog()->GetQuestDlg() ;
//
//		if( pDlg )
//		{
//			pDlg->UpdateQuestDescView(dwQuestIdx, dwSubQuestIdx) ;
//		}
//
//		////
//		//m_QuestUpdater->UpdateSubQuestData();											// ����Ʈ �������Ϳ� ���� ����Ʈ �����͸� ������Ʈ �Ѵ�.
//
//		//CQuestQuickViewDialog* pQuickView = NULL ;														// ����Ʈ �˸��� ���̾�α� ������ ���� �����͸� �����ϰ� nulló�� �Ѵ�.
//		//pQuickView = GAMEIN->GetQuestQuickViewDialog() ;												// ����Ʈ �˸��� ���̾�α� ������ �޴´�.
//
//		//if( pQuickView )																				// ����Ʈ �˸��� ���̾�α� ������ ��ȿ�ϴٸ�,
//		//{
//		//	pQuickView->ResetQuickViewDialog() ;																// ����Ʈ �˸����� ���� ����Ʈ �����͸� �ε��Ѵ�.
//		//}
//	}
//}

void CQuestManager::QuestItemAdd( DWORD dwItemIdx, DWORD dwCount )						// ����Ʈ �������� �߰��ϴ� �Լ�.
{
	QUEST_ITEM_INFO* pData = m_QuestItemMGR.GetQuestItem( dwItemIdx );					// ����Ʈ ������ �Ŵ������� ������ �ε�����, ����Ʈ ������ ������ �޴´�.

	if( !pData ) return;																// ������ ������ ��ȿ���� ������, ���� ó���� �Ѵ�.

	m_QuestUpdater->QuestItemAdd( pData, dwCount );										// ����Ʈ �������Ϳ� ����Ʈ �������� �߰��Ѵ�.

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(224), pData->ItemName );			// ä��â��, ������ ���� ����Ѵ�.
}

void CQuestManager::QuestItemDelete( DWORD dwItemIdx )									// ����Ʈ ������ ���� �Լ�.
{
	// itemname
	QUEST_ITEM_INFO* pData = m_QuestItemMGR.GetQuestItem( dwItemIdx );					// ������ �ε�����, ����Ʈ ������ �Ŵ������� ����Ʈ ������ ������ �޴´�.

	if( !pData ) return;																// ������ ������ ��ȿ���� ������, ���� ó���� �Ѵ�.

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(225), pData->ItemName );			// ä��â�� �����۸�� �޽����� �����Ͽ� �߰��Ѵ�.

	m_QuestUpdater->QuestItemDelete( dwItemIdx );										// ����Ʈ �������Ϳ��� ����Ʈ �������� �����Ѵ�.
}

void CQuestManager::QuestItemUpdate( DWORD dwType, DWORD dwItemIdx, DWORD dwCount )		// ����Ʈ �������� ������Ʈ �ϴ� �Լ�.
{
	m_QuestUpdater->QuestItemUpdate( dwType, dwItemIdx, dwCount);						// ����Ʈ �������Ϳ��� ����Ʈ �������� ������Ʈ �Ѵ�.

	// itemname
	QUEST_ITEM_INFO* pData = m_QuestItemMGR.GetQuestItem( dwItemIdx );					// ���� ������ �ε����� ����Ʈ ������ �Ŵ������� ����Ʈ ������ ������ �޴´�.

	if( !pData ) return;																// ������ ������ ��ȿ���� ������, ����ó���� �Ѵ�.

	m_QuestUpdater->QuestItemAdd( pData, dwCount );										// ����Ʈ �������Ϳ� �������� �߰��Ѵ�.

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(224), pData->ItemName );			// ä��â�� ������ �̸��� �޽����� �����Ͽ� ����Ѵ�.
}

BOOL CQuestManager::IsQuestComplete( DWORD dwQuestIdx )									// ����Ʈ �Ϸ� ���θ� ��ȯ�ϴ� �Լ�.
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// ���� �ε����� ����Ʈ ���̺��� ����Ʈ ������ �޴´�.

	if( pQuest )																		// ����Ʈ ������ ��ȿ���� üũ�Ѵ�.
	{
		if( pQuest->IsQuestComplete() )													// ����Ʈ�� �Ϸ� �Ǿ��ٸ�,
		{
			return TRUE;																// TRUE�� �����Ѵ�.
		}
		else																			// ����Ʈ�� �Ϸ���� �ʾҴٸ�,
		{
			return FALSE;																// FALSE�� �����Ѵ�.
		}
	}

	return FALSE;																		// FALSE�� �����Ѵ�.
}

void CQuestManager::DeleteQuest( DWORD dwQuestIdx, BOOL bCheckTime )					// ����Ʈ�� �����ϴ� �Լ�.
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// ���� �ε����� ����Ʈ ���̺��� ����Ʈ ������ �޴´�.

	if( !pQuest ) return ;																// ����Ʈ ������ ��ȿ���� üũ�Ѵ�.

	if( pQuest->IsQuestComplete() )	return ;											// ����Ʈ�� �Ϸ��Ͽ��ٸ�,

	pQuest->Initialize();																// ����Ʈ�� �ʱ�ȭ �Ѵ�.
	m_QuestUpdater->DeleteQuest( dwQuestIdx );											// ����Ʈ �������Ϳ��� ����Ʈ�� �����Ѵ�.

	m_QuestDialog->EndQuest(dwQuestIdx) ;
	m_QuickView->DeleteQuest(dwQuestIdx) ;

	int Key = 0;																	// Ű ���� ���� ������ �����ϰ�, 0���� �����Ѵ�.
	COMBINEKEY( dwQuestIdx, 0, Key );												// ����Ʈ �ε����� 0���� Ű���� �޴´�.

	CQuestString* pQString = m_QuestStringTable.GetData(Key);						// Ű ������ ����Ʈ ��Ʈ�� ���̺��� ����Ʈ ��Ʈ�� ������ �޴´�.

	if( pQString == NULL )	return;													// ����Ʈ ������ ��ȿ���� ������, ���� ó���� �Ѵ�.

	cPtrList* p = pQString->GetTitle();												// ����Ʈ �����͸� �����ϰ�, ��Ʈ���� ������ �޴´�.
	ITEM* string = (ITEM*)p->GetHead();										// ����Ʈ �����͸� ���� �����Ͽ� ��Ʈ�� ������ ������ �޴´�.

	if( pQuest->IsTimeCheck() && bCheckTime )										// ����Ʈ�� �ð��� üũ�Ѵٸ�,
	{
		CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(295), string->string );		// ����ð� �ʰ��� ����Ǿ��ٴ� �޽����� ����Ѵ�.

		char msg[256] = {0, };														// �ӽ� ���۸� �����Ѵ�.
		CompositString( msg, CHATMGR->GetChatMsg(295), string->string );				// ���� �޽����� �����.

		STATUSICONDLG->RemoveQuestTimeIcon( HERO, eStatusIcon_QuestTime );			// �ð�üũ �������� �����.
	}
	else																			// �ð�üũ�� ���� �ʴ´ٸ�,
	{
		CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(223), string->string );		// ����Ʈ�� �����ߴٴ� �޽����� ä��â�� ����Ѵ�.

		char msg[256] = {0, };														// �ӽ� ���۸� �����Ѵ�.
		CompositString( msg, CHATMGR->GetChatMsg(223), string->string );				// ���� �޽����� �����.

		STATUSICONDLG->RemoveQuestTimeIcon( HERO, eStatusIcon_QuestTime );			// ����Ʈ �ð� �������� �����Ѵ�.
	}
}

//void CQuestManager::DeleteQuest( DWORD dwQuestIdx, BOOL bCheckTime )					// ����Ʈ�� �����ϴ� �Լ�.
//{
//	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );								// ���� �ε����� ����Ʈ ���̺��� ����Ʈ ������ �޴´�.
//
//	if( pQuest )																		// ����Ʈ ������ ��ȿ���� üũ�Ѵ�.
//	{
//		if( pQuest->IsQuestComplete() )													// ����Ʈ�� �Ϸ��Ͽ��ٸ�,
//		{
//			return;																		// ���� ó���� �Ѵ�.
//		}
//		//
//		int Key = 0;																	// Ű ���� ���� ������ �����ϰ�, 0���� �����Ѵ�.
//		COMBINEKEY( dwQuestIdx, 0, Key );												// ����Ʈ �ε����� 0���� Ű���� �޴´�.
//
//		CQuestString* pQString = m_QuestStringTable.GetData(Key);						// Ű ������ ����Ʈ ��Ʈ�� ���̺��� ����Ʈ ��Ʈ�� ������ �޴´�.
//
//		if( pQString == NULL )	return;													// ����Ʈ ������ ��ȿ���� ������, ���� ó���� �Ѵ�.
//
//		cPtrList* p = pQString->GetTitle();												// ����Ʈ �����͸� �����ϰ�, ��Ʈ���� ������ �޴´�.
//		ITEM* string = (ITEM*)p->GetHead();										// ����Ʈ �����͸� ���� �����Ͽ� ��Ʈ�� ������ ������ �޴´�.
//
//		if( pQuest->IsTimeCheck() && bCheckTime )										// ����Ʈ�� �ð��� üũ�Ѵٸ�,
//		{
//			CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(295), string->string );		// ����ð� �ʰ��� ����Ǿ��ٴ� �޽����� ����Ѵ�.
//
//			char msg[256] = {0, };														// �ӽ� ���۸� �����Ѵ�.
//			CompositString( msg, CHATMGR->GetChatMsg(295), string->string );				// ���� �޽����� �����.
//
//			STATUSICONDLG->RemoveQuestTimeIcon( HERO, eStatusIcon_QuestTime );			// �ð�üũ �������� �����.
//		}
//		else																			// �ð�üũ�� ���� �ʴ´ٸ�,
//		{
//			CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(223), string->string );		// ����Ʈ�� �����ߴٴ� �޽����� ä��â�� ����Ѵ�.
//
//			char msg[256] = {0, };														// �ӽ� ���۸� �����Ѵ�.
//			CompositString( msg, CHATMGR->GetChatMsg(223), string->string );				// ���� �޽����� �����.
//
//			STATUSICONDLG->RemoveQuestTimeIcon( HERO, eStatusIcon_QuestTime );			// ����Ʈ �ð� �������� �����Ѵ�.
//		}
//
//		pQuest->Initialize();															// ����Ʈ�� �ʱ�ȭ �Ѵ�.
//		m_QuestUpdater->DeleteQuest( dwQuestIdx );										// ����Ʈ �������Ϳ��� ����Ʈ�� �����Ѵ�.
//	}
//}

BOOL CQuestManager::IsNpcRelationQuest( DWORD dwNpcIdx )								// NPC�� ����� ����Ʈ���� ��ȯ�ϴ� �Լ�.
{
	CQuest* pQuest = NULL;																// ����Ʈ �����͸� �����ϰ�, NULLó���� �Ѵ�.

	m_QuestTable.SetPositionHead();														// ����Ʈ ���̺��� ���� �����Ѵ�.

	while( pQuest = m_QuestTable.GetData() )											// ����Ʈ ���̺� ����Ʈ ������ �ִµ��� while���� ������.
	{
		if( pQuest->IsNpcRelationQuest( dwNpcIdx ) )									// npc �ε����� ����Ʈ�� npc�� ����� ����Ʈ���� üũ�Ѵ�.
		{
			return TRUE;																// npc�� ������ ������, TRUE�� �����Ѵ�.
		}
	}

	return FALSE;																		// FALSE�� �����Ѵ�.
}

DWORD CQuestManager::GetNpcScriptPage( DWORD dwNpcIdx )									// NPC ��ũ��Ʈ �������� �����ϴ� �Լ�.
{
	DWORD dwPage = 0;																	// �������� ���� ������ �����ϰ� 0���� �����Ѵ�.

	CQuest* pQuest = NULL;																// ����Ʈ ������ ���� ������ ������ �����Ѵ�.

	m_QuestTable.SetPositionHead();														// ����Ʈ ���̺��� ���� �����Ѵ�.

	while( pQuest = m_QuestTable.GetData() )											// ����Ʈ ���̺� ����Ʈ ������ ���� ���� while���� ������.
	{
		dwPage = pQuest->GetNpcScriptPage( dwNpcIdx );									// npc �ε�����, npc ��ũ��Ʈ �������� �޴´�.

		if( dwPage )																	// ������ ������ ��ȿ�ϴٸ�,
		{
			return dwPage;																// �������� �����Ѵ�.
		}
	}

	return 0;																			// 0�� �����Ѵ�.
}

void CQuestManager::SendNpcTalkEvent( DWORD dwNpcIdx, DWORD dwNpcScriptIdx )			// npc ��ȭ �̺�Ʈ�� ������ �Լ�.
{
	SEND_QUEST_IDX Msg;																	// ����Ʈ ������ ���� �޽��� ����ü�� �����Ѵ�.

	Msg.Category = MP_QUEST;															// ī�װ��� ����Ʈ�� �����Ѵ�.
	Msg.Protocol = MP_QUESTEVENT_NPCTALK;												// ���������� ����Ʈ �̺�Ʈ �� npc ��ȭ�� �����Ѵ�.
	Msg.dwObjectID = HERO->GetID();														// HERO�� ���̵� �����Ѵ�.
	Msg.MainQuestIdx = (WORD)dwNpcIdx;													// ���� ����Ʈ �ε����� �����Ѵ�.
	Msg.SubQuestIdx = (WORD)dwNpcScriptIdx;												// ���� ����Ʈ �ε����� �����Ѵ�.

	/*CQuestDialog* pQuestDialog = GAMEIN->GetQuestDialog() ;

	if( pQuestDialog ) 
	{
		Msg.dwData = pQuestDialog->GetQuestCount() ;
	}*/

	NETWORK->Send( &Msg, sizeof(Msg) );													// ���� �� ������ �޽����� �����Ѵ�.
}

void CQuestManager::SendNpcTalkEventForEvent( DWORD dwNpcIdx, DWORD dwNpcScriptIdx )	// �̺�Ʈ�� ���� npc��ȭ �̺�Ʈ�� ������ �Լ�.
{
	SEND_QUEST_IDX Msg;																	// ����Ʈ ������ ���� �޽��� ����ü�� �����Ѵ�.

	Msg.Category = MP_QUEST;															// ī�װ��� ����Ʈ�� �����Ѵ�.
	Msg.Protocol = MP_QUESTEVENT_NPCTALK_SYN;											// ���������� ����Ʈ �̺�Ʈ �� npc ��ȭ ��û���� �����Ѵ�.
	Msg.dwObjectID = HERO->GetID();														// HERO ���̵� �����Ѵ�.
	Msg.MainQuestIdx = (WORD)dwNpcIdx;													// npc �ε����� �����Ѵ�.
	Msg.SubQuestIdx = (WORD)dwNpcScriptIdx;												// npc ��ũ��Ʈ �ε����� �����Ѵ�.

	NETWORK->Send( &Msg, sizeof(Msg) );													// ���� �� ������ �޽����� �����Ѵ�.
}

void CQuestManager::SetNpcData( CNpc* pNpc )											// npc �����͸� �����ϴ� �Լ�.
{
	if( m_dwMaxNpc > MAX_NPCARRAY-1 )													// �ִ� npc �ε����� 31���� ū�� üũ�Ѵ�.
	{
		return;																			// ũ�ٸ�, ����ó���� �Ѵ�.
	}

	m_pNpcArray[m_dwMaxNpc] = pNpc;														// npc ������ ��� �迭 �ε����� npc ������ ��´�.

	++m_dwMaxNpc;																		// �ִ� npc ��ȣ�� ������Ų��.
}

void CQuestManager::SetNpcIDName(WORD wIdx, stNPCIDNAME* pInfo)							// npc �̸��� ���̵� �����ϴ� �Լ�.
{
	if( wIdx > MAX_NPCIDNAME-1 )
	{
		ASSERTMSG(0, "����ƽ NPC ������ 100���� �Ѿ���, MAX_NPCIDNAME ��ġ�� �÷��ֽÿ�.") ;
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





void CQuestManager::ProcessNpcMark()													// npc ��ũ ���μ��� �Լ�.
{
	if( ! HERO )																		// HERO ������ ��ȿ���� ������,
	{
		return;																			// ���� ó���� �Ѵ�.
	}

	CMiniMapDlg* miniMapDialog = GAMEIN->GetMiniMapDialog();							// �̴ϸ� ���̾�α� ������ �޾ƿ´�.
	ASSERT( miniMapDialog );															// �̴ϸ� ���̾�α� ������ ASSERT üũ �Ѵ�.

	for( DWORD i = 0; i < m_dwMaxNpc; ++i )												// npc �� ��ŭ for���� ������.
	{
		CNpc* object = m_pNpcArray[ i ];												// NPC ������ �޴´�.

		if( !object ) continue ;														// NPC ������ ��ȿ���� ������ continue ó���� �Ѵ�.

		object->RemoveAllObjectEffect();												// NPC�� ��� ����Ʈ�� �����Ѵ�.

		miniMapDialog->ShowQuestMarkIcon( object, eQM_NONE );							// �̴ϸ� ���̾�α��� ����Ʈ ��ũ�� �������� �����Ѵ�.
	}

	m_QuestTable.SetPositionHead();														// ����Ʈ ���̺��� ��� ���������� �����Ѵ�.

	CQuest* pQuest ;

	while(TRUE)
	{
		pQuest = NULL ;
		pQuest = m_QuestTable.GetData() ;

		if( !pQuest ) break ;

		for( DWORD i = 0; i < m_dwMaxNpc; ++i )											// npc ����ŭ for���� ������.
		{
			CNpc* object = m_pNpcArray[ i ];											// NPC ������ �޴´�.

			if( !object ) continue ;

			switch( pQuest->GetNpcMarkType( object->GetNpcUniqueIdx() ) )				// NPC�� ��ũ Ÿ���� Ȯ���Ѵ�.
			{
				case 1:																	// ��ũ Ÿ���� !���,
				{
					NPC_LIST* pNpcList = GAMERESRCMNGR->GetNpcInfo( object->GetNpcKind() );	// NPC ����Ʈ ������ �޴´�.

					VECTOR3 Pos;														// ��ġ ������ ���� ���͸� �����Ѵ�.
					Pos.x = Pos.z = 0.f;												// ��ġ ������ �����Ѵ�.
					Pos.y = float( pNpcList->Tall ) + 50.0f;

					if( object->IsNpcMark( NPCMARK_2_ID ) )								// NPC ��ũ�� ?���,
					{
						object->RemoveObjectEffect( NPCMARK_2_ID );						// NPC��ũ ?�� �����Ѵ�.

						OBJECTEFFECTDESC desc( 17 );									// ������Ʈ ����Ʈ ��ũ������ �����Ѵ�.
						desc.Position = Pos;											// ��ġ ������ �����Ѵ�.

						object->AddObjectEffect( NPCMARK_1_ID, &desc, 1, HERO );		// ������Ʈ ����Ʈ�� �߰��Ѵ�.

						miniMapDialog->ShowQuestMarkIcon( object, eQM_EXCLAMATION );	// �̴ϸʿ� ����Ʈ ��ũ�� eQM_EXCLAMATION�� �����Ѵ�.
					}
					else																// NPC ��ũ�� !��?�� �ƴϸ�,
					{
						OBJECTEFFECTDESC desc( 17 );									// ������Ʈ ����Ʈ ��ũ������ �����Ѵ�.
						desc.Position = Pos;											// ��ġ ������ �����Ѵ�.

						object->AddObjectEffect( NPCMARK_1_ID, &desc, 1, HERO );		// ������Ʈ�� ����Ʈ�� �߰��Ѵ�.


						miniMapDialog->ShowQuestMarkIcon( object, eQM_EXCLAMATION );	// �̴ϸʿ� ����Ʈ ��ũ�� �����ش�.
					}

					break;
				}
				case 2:																	// ��ũ Ÿ���� ?���,
				{
					NPC_LIST* pNpcList = GAMERESRCMNGR->GetNpcInfo( object->GetNpcKind() );	// NPC ����Ʈ ������ �޴´�.

					VECTOR3 Pos;														// ��ġ ������ ���� ���͸� �����Ѵ�.
					Pos.x = Pos.z = 0.f;												// ��ġ�� �����Ѵ�.
					Pos.y = float( pNpcList->Tall ) + 50.0f;

					if( object->IsNpcMark( NPCMARK_1_ID ) )								// NPC ��ũ�� !�� ���ٸ�,
					{
						//object->RemoveObjectEffect( NPCMARK_1_ID );						// NPC�� !��ũ�� �����Ѵ�.
					}
					else if( object->IsNpcMark( NPCMARK_2_ID ) )						// NPC ��ũ�� ?�� ���ٸ�,
					{

					}
					else																// NPC ��ũ�� !��?�� �ƴϸ�,
					{
						OBJECTEFFECTDESC desc( 18 );									// ����Ʈ ��ũ��Ʈ ����ü�� �����Ѵ�.
						desc.Position = Pos;                                            // ��ġ�� �����Ѵ�.                                   
						object->AddObjectEffect( NPCMARK_2_ID, &desc, 1, HERO );		// ����Ʈ�� �߰��Ѵ�.

						miniMapDialog->ShowQuestMarkIcon( object, eQM_QUESTION );		// �̴ϸʿ� ����Ʈ �������� �����ش�.
					}

					break;
				}
			}
		}
	}
}





//void CQuestManager::ProcessNpcMark()													// npc ��ũ ���μ��� �Լ�.
//{
//	if( ! HERO )																		// HERO ������ ��ȿ���� ������,
//	{
//		return;																			// ���� ó���� �Ѵ�.
//	}
//
//	CMiniMapDlg* miniMapDialog = GAMEIN->GetMiniMapDialog();							// �̴ϸ� ���̾�α� ������ �޾ƿ´�.
//	ASSERT( miniMapDialog );															// �̴ϸ� ���̾�α� ������ ASSERT üũ �Ѵ�.
//
//	for( DWORD i = 0; i < m_dwMaxNpc; ++i )																		// npc �� ��ŭ for���� ������.
//	{
//		CNpc* object = m_pNpcArray[ i ];												// NPC ������ �޴´�.
//
//		if( object )																	// NPC ������ ��ȿ�ϴٸ�,
//		{
//			object->RemoveAllObjectEffect();											// NPC�� ��� ����Ʈ�� �����Ѵ�.
//
//			miniMapDialog->ShowQuestMarkIcon( object, eQM_NONE );						// �̴ϸ� ���̾�α��� ����Ʈ ��ũ�� �������� �����Ѵ�.
//		}
//	}
//
//	m_QuestTable.SetPositionHead();														// ����Ʈ ���̺��� ��� ���������� �����Ѵ�.
//
//	for( CQuest* pQuest; pQuest = m_QuestTable.GetData(); )								// ����Ʈ ���̺��� ����Ʈ ������ �ް�, ����Ʈ ������ ��ȿ�� ���� FOR���� ������.
//	{
//		for( DWORD i = 0; i < m_dwMaxNpc; ++i )											// npc ����ŭ for���� ������.
//		{
//			CNpc* object = m_pNpcArray[ i ];											// NPC ������ �޴´�.
//
//			if( ! object )																// NPC ������ ��ȿ���� ������,
//			{
//				continue;																// continue�� �Ѵ�.
//			}
//
//			switch( pQuest->GetNpcMarkType( object->GetNpcUniqueIdx() ) )				// NPC�� ��ũ Ÿ���� Ȯ���Ѵ�.
//			{
//				case 1:																	// ��ũ Ÿ���� !���,
//				{
//					NPC_LIST* pNpcList = GAMERESRCMNGR->GetNpcInfo( object->GetNpcKind() );	// NPC ����Ʈ ������ �޴´�.
//
//					VECTOR3 Pos;														// ��ġ ������ ���� ���͸� �����Ѵ�.
//					Pos.x = Pos.z = 0.f;												// ��ġ ������ �����Ѵ�.
//					Pos.y = float( pNpcList->Tall ) + 50.0f;
//
//					if( object->IsNpcMark( NPCMARK_1_ID ) )								// NPC ��ũ�� !���,
//					{
//						//	object->RemoveObjectEffect( NPCMARK_1_ID );					// �ƹ��͵� ���� �ʴ´�.
//					}
//					else if( object->IsNpcMark( NPCMARK_2_ID ) )						// NPC ��ũ�� ?���,
//					{
//						object->RemoveObjectEffect( NPCMARK_2_ID );						// NPC��ũ ?�� �����Ѵ�.
//
//						OBJECTEFFECTDESC desc( 17 );									// ������Ʈ ����Ʈ ��ũ������ �����Ѵ�.
//						desc.Position = Pos;											// ��ġ ������ �����Ѵ�.
//
//						object->AddObjectEffect( NPCMARK_1_ID, &desc, 1, HERO );		// ������Ʈ ����Ʈ�� �߰��Ѵ�.
//
//						miniMapDialog->ShowQuestMarkIcon( object, eQM_EXCLAMATION );	// �̴ϸʿ� ����Ʈ ��ũ�� eQM_EXCLAMATION�� �����Ѵ�.
//					}
//					else																// NPC ��ũ�� !��?�� �ƴϸ�,
//					{
//						OBJECTEFFECTDESC desc( 17 );									// ������Ʈ ����Ʈ ��ũ������ �����Ѵ�.
//						desc.Position = Pos;											// ��ġ ������ �����Ѵ�.
//
//						object->AddObjectEffect( NPCMARK_1_ID, &desc, 1, HERO );		// ������Ʈ�� ����Ʈ�� �߰��Ѵ�.
//
//
//						miniMapDialog->ShowQuestMarkIcon( object, eQM_EXCLAMATION );	// �̴ϸʿ� ����Ʈ ��ũ�� �����ش�.
//					}
//
//					break;
//				}
//				case 2:																	// ��ũ Ÿ���� ?���,
//				{
//					NPC_LIST* pNpcList = GAMERESRCMNGR->GetNpcInfo( object->GetNpcKind() );	// NPC ����Ʈ ������ �޴´�.
//
//					VECTOR3 Pos;														// ��ġ ������ ���� ���͸� �����Ѵ�.
//					Pos.x = Pos.z = 0.f;												// ��ġ�� �����Ѵ�.
//					Pos.y = float( pNpcList->Tall ) + 50.0f;
//
//					if( object->IsNpcMark( NPCMARK_1_ID ) )								// NPC ��ũ�� !�� ���ٸ�,
//					{
//						object->RemoveObjectEffect( NPCMARK_1_ID );						// NPC�� !��ũ�� �����Ѵ�.
//					}
//					else if( object->IsNpcMark( NPCMARK_2_ID ) )						// NPC ��ũ�� ?�� ���ٸ�,
//					{
//
//					}
//					else																// NPC ��ũ�� !��?�� �ƴϸ�,
//					{
//						OBJECTEFFECTDESC desc( 18 );									// ����Ʈ ��ũ��Ʈ ����ü�� �����Ѵ�.
//						desc.Position = Pos;                                            // ��ġ�� �����Ѵ�.                                   
//						object->AddObjectEffect( NPCMARK_2_ID, &desc, 1, HERO );		// ����Ʈ�� �߰��Ѵ�.
//
//						miniMapDialog->ShowQuestMarkIcon( object, eQM_QUESTION );		// �̴ϸʿ� ����Ʈ �������� �����ش�.
//					}
//
//					break;
//				}
//			}
//		}
//	}
//}





void CQuestManager::NetworkMsgParse( BYTE Protocol, void* pMsg )									// ����Ʈ ��Ʈ��ũ �޽��� �Ľ��Լ�.
{
	if(!pMsg) return ;																				// ���ڷ� �Ѿ�� �޽��� ������ ��ȿ���� ������, return ó���� �Ѵ�.

	switch(Protocol)																				// �������� ������ Ȯ���Ѵ�.
	{
	case MP_QUEST_MAINDATA_LOAD:				Quest_Maindata_Load( pMsg ) ;			break;		// ���� ������ �ε� ó���� �Ѵ�.
	case MP_QUEST_SUBDATA_LOAD:					Quest_Subdata_Load( pMsg ) ;			break;		// ���� ������ �ε� ó���� �Ѵ�.
	case MP_QUEST_ITEM_LOAD:					Quest_Item_Load( pMsg ) ;				break;		// ������ �ε� ó���� �Ѵ�.
	case MP_QUEST_START_ACK:					Quest_Start_Ack( pMsg ) ;				break;		// ����Ʈ ���� ó���� �Ѵ�.
	case MP_SUBQUEST_START_ACK:					SubQuest_Start_Ack( pMsg ) ;			break;		// ���� ����Ʈ ���� ó���� �Ѵ�.
	case MP_QUEST_END_ACK:						Quest_End_Ack( pMsg )  ;				break;		// ����Ʈ ���� ó���� �Ѵ�.
	case MP_SUBQUEST_END_ACK:					SubQuest_End_Ack( pMsg ) ;				break;		// ���� ����Ʈ ���� ó���� �Ѵ�.
	case MP_SUBQUEST_UPDATE:					SubQuest_Update( pMsg ) ;				break;		// ���� ����Ʈ ������Ʈ ó���� �Ѵ�.
	case MP_QUEST_DELETE_ACK:					Quest_Delete_Ack( pMsg ) ;				break;		// ����Ʈ ������ ó���Ѵ�.
	case MP_QUEST_DELETE_CONFIRM_ACK:			Quest_Delete_Confirm_Ack( pMsg ) ;		break;		// ����Ʈ ���� ���� ó���� �Ѵ�.
	case MP_QUESTITEM_GIVE:						QuestItem_Give( pMsg ) ;				break;		// ����Ʈ �������� �ִ� ó���� �Ѵ�.
	case MP_QUESTITEM_TAKE:						QuestItem_Take( pMsg ) ;				break;		// ����Ʈ �������� �����ϴ� ó���� �Ѵ�.
	case MP_QUESTITEM_UPDATE:					QuestItem_Update( pMsg ) ;				break;		// ����Ʈ ������ ������Ʈ ó���� �Ѵ�.
	case MP_QUEST_TAKEITEM_ACK:					Quest_TakeItem_Ack( pMsg ) ;			break;		// ����Ʈ ������ ���� ó���� �Ѵ�.
	case MP_QUEST_TAKEMONEY_ACK:				Quest_Takemoney_Ack( pMsg ) ;			break;		// ����Ʈ �Ӵ� ȹ�� ó���� �Ѵ�.
	case MP_QUEST_TAKEEXP_ACK:					Quest_TakeExp_Ack( pMsg ) ;				break;		// ����Ʈ ����ġ ȹ�� ó���� �Ѵ�.
	case MP_QUEST_TAKESEXP_ACK:					Quest_TakesExp_Ack( pMsg ) ;			break;		// ����Ʈ ����ġ ȹ�� ó���� �Ѵ�.
	case MP_QUEST_GIVEITEM_ACK:					Quest_GiveItem_Ack( pMsg ) ;			break;		// ����Ʈ ������ �й� ó���� �Ѵ�.
	// 071026 LYW --- QuestManager : Add process part of give stack item.
	case MP_QUEST_GIVEITEMS_ACK :				Quest_GiveItems_Ack( pMsg ) ;			break ;		// ����Ʈ ���� ������ �й� ó���� �Ѵ�.
	case MP_QUEST_GIVEMONEY_ACK:				Quest_GiveMoney_Ack( pMsg ) ;			break;		// ����Ʈ �Ӵ� �й� ó���� �Ѵ�.
	case MP_QUEST_TIME_LIMIT:					Quest_Time_Limit( pMsg ) ;				break;		// ����Ʈ �ð� ���� ó���� �Ѵ�.
	case MP_QUEST_REGIST_CHECKTIME:				Quest_Regist_Checktime( pMsg ) ;		break;		// ����Ʈ �ð� üũ ��� ó���� �Ѵ�.
	case MP_QUEST_UNREGIST_CHECKTIME:			Quest_Unregist_Checktime( pMsg ) ;		break;		// ����Ʈ �ð� üũ Ż�� ó���� �Ѵ�.
	case MP_QUESTEVENT_NPCTALK_ACK:				QuestEvent_NpcTalk_Ack( pMsg ) ;		break;		// ����Ʈ �̺�Ʈ npc ��ȭ ���� ó���� �Ѵ�.
	case MP_QUESTEVENT_NPCTALK_NACK:			QuestEvent_NpcTalk_Nack( pMsg ) ;		break;		// ����Ʈ �̺�Ʈ npc ��ȭ ���� ó���� �Ѵ�.
	case MP_QUEST_EXECUTE_ERROR:				Quest_Execute_Error( pMsg ) ;			break;		// ����Ʈ ���� ó���� �Ѵ�.
	case MP_QUEST_ERROR_EXT :					Quest_Error_Ext( pMsg ) ;				break;		// ����Ʈ ��Ÿ ���� ó���� �Ѵ�.
	case MP_QUEST_ERROR_FULLQUEST :				Quest_Error_FullQuest( pMsg ) ;			break ;		// ����Ʈ ����� �� á�ٴ� ���� �޽����� ó���Ѵ�.

	// 071028 LYW --- QuestManager : Add process part of error when failed give item.
	case MP_QUEST_GIVEITEM_ERR :				Quest_GiveItem_Error( pMsg ) ;			break ;		// ����Ʈ ������ �ݳ� ���� ó���� �Ѵ�.

	default:																			break;		// ����Ʈ ó��.
	}

	GAMEIN->GetNpcScriptDialog()->RefreshQuestLinkPage() ;

	CQuestQuickViewDialog* pQuickView = NULL ;														// ����Ʈ �˸��� ���̾�α� ������ ���� �����͸� �����ϰ� nulló�� �Ѵ�.
	pQuickView = GAMEIN->GetQuestQuickViewDialog() ;												// ����Ʈ �˸��� ���̾�α� ������ �޴´�.

	if( pQuickView )																				// ����Ʈ �˸��� ���̾�α� ������ ��ȿ�ϴٸ�,
	{
		pQuickView->ResetQuickViewDialog() ;																// ����Ʈ �˸����� ���� ����Ʈ �����͸� �ε��Ѵ�.
	}
}

//
#include "GameIn.h"																					// ������ ��������� �ҷ��´�.
#include "QuestTotalDialog.h"																		// ����Ʈ ��Ż ���̾�α� ����� �ҷ��´�.

void CQuestManager::AddStringTest()																	// �׽�Ʈ �Լ�.
{
	CQuestString* pQuestString = NULL;																// ȣ���ϴ� ���� ����.
}

void CQuestManager::ViewToggle()																	// ��� �� �Լ�.
{
	GAMEIN->GetQuestTotalDialog()->QuestListView();													// ȣ���ϴ� ���� ����.
}

void CQuestManager::AddJournalTest()																// ���� �߰� �׽�Ʈ �Լ�.(ȣ���ϴ� ���� ����.)
{
	CQuestString* pQuestString = NULL;																// ����Ʈ ��Ʈ�� �����͸� �����ϰ�, NULL ó���� �Ѵ�.

	char buf[16] = { 0, };																			// �ӽ� ���۸� �����Ѵ�.
	sprintf(buf, "2004.08.06");																		// �ӽ� ���ۿ� ��¥�� �����Ѵ�.
	
	JOURNALINFO item;																				// ���� ���� ����ü�� �����Ѵ�.

	item.Kind = eJournal_Quest;																		// ������ eJournal_Quest�� �����Ѵ�.

	item.bSaved = FALSE;																			// ���̺� ���θ� FALSE�� �����Ѵ�.
	item.Param = 1;																					// �Ķ���͸� 1�� �����Ѵ�.

	strcpy(item.RegDate, buf);																		// ������ ��� �����͸� ���ۿ� �����Ѵ�.

	m_QuestUpdater->JournalItemAdd(&item);															// ����Ʈ �������Ϳ� ���� �������� �߰��Ѵ�.

	item.Param = 2;																					// �Ķ���͸� 2�� �����Ѵ�.
	item.Param_2 = 0;																				// �Ķ����2�� 0���� �����Ѵ�.

	m_QuestUpdater->JournalItemAdd(&item);															// ����Ʈ �������Ϳ� ���� �������� �߰��Ѵ�.

	item.Param = 3;																					// �Ķ���͸� 3���� �����Ѵ�.

	m_QuestUpdater->JournalItemAdd(&item);															// ����Ʈ �������Ϳ� ���� �������� �߰��Ѵ�.

}

void CQuestManager::QuestAbandon()																	// ����Ʈ ���� ó�� �Լ�.
{
	DWORD QuestIdx = 0 ;																			// ����Ʈ �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.

	CQuestDialog* pDlg = NULL ;																		// ����Ʈ ���̾�α� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
	pDlg = GAMEIN->GetQuestTotalDialog()->GetQuestDlg() ;											// ����Ʈ ���̾�α� ������ �޴´�.

	if( pDlg )																						// ����Ʈ ���̾�α� ������ ��ȿ�ϴٸ�,
	{
		QuestIdx = pDlg->GetSelectedQuestID() ;														// ���� �� ����Ʈ �ε����� �޴´�.

		pDlg->SetQuestCount() ;

		//int nQuestCount = 0 ;																		// ����Ʈ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
		//nQuestCount = pDlg->GetQuestCount() ;														// ����Ʈ ī��Ʈ�� �޴´�.

		//if( nQuestCount == 0 )																		// ����Ʈ ī��Ʈ�� 0�� ������,
		//{
		//	pDlg->SetQuestCount( 0 ) ;																// ����Ʈ ī��Ʈ�� 0���� �����Ѵ�.
		//}
		//else																						// ����Ʈ ī��Ʈ�� 0�� ���� ������,
		//{
		//	pDlg->SetQuestCount( nQuestCount-1 ) ;													// ����Ʈ ī��Ʈ�� �����Ѵ�.
		//}
	}

	m_QuestUpdater->CloseMsgBox();																	// ����Ʈ ���������� Ŭ���� �޽��� �ڽ��� ȣ���Ѵ�.

	if( IsQuestComplete(QuestIdx) )																	// ����Ʈ �ε����� �Ϸ��� ����Ʈ���� üũ�Ѵ�.
	{
		return;																						// �Ϸ��� ����Ʈ���, ����ó���� �Ѵ�.
	}

	SEND_QUEST_IDX msg;																				// ����Ʈ ������ ���� ����ü�� �����Ѵ�.

	msg.Category = MP_QUEST;																		// ī�װ��� ����Ʈ�� �����Ѵ�.
	msg.Protocol = MP_QUEST_DELETE_SYN;																// ���������� ����Ʈ �輼 ��ũ�� �����Ѵ�.
	msg.dwObjectID = HERO->GetID();																	// HERO ���̵� �����Ѵ�.
	msg.MainQuestIdx = (WORD)QuestIdx;																// ����Ʈ �ε����� �����Ѵ�.

	NETWORK->Send(&msg, sizeof(msg));																// ���õ� ������ �����Ѵ�.

	CQuestInfo* pQuestInfo =NULL ;																	// ����Ʈ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pQuestInfo = GetQuestInfo(QuestIdx) ;															// ����Ʈ �ε����� ����Ʈ ������ �޴´�.

	if(!pQuestInfo)																					// ����Ʈ ������ ��ȿ���� ������,
	{
		ASSERTMSG(0, "����Ʈ �����ϴ� �������� ����Ʈ ������ �ε����� ���Ͽ����ϴ�.!") ;			// ASSERT ���� üũ.
		return  ;																					// ���� ó���� �Ѵ�.
	}

	int nMapNum = pQuestInfo->GetMapNum() ;															// ����Ʈ ������ �ִ� �� ��ȣ�� �޴´�.

	DeleteQuestTitle(nMapNum, QuestIdx) ;															// ����Ʈ ����Ʈ���� ����Ʈ�� �����Ѵ�.

	DeleteMapTree(nMapNum) ;																		// �� Ʈ���� üũ�� �����Ѵ�.

	// ���� ����Ʈ ����
}

//void CQuestManager::QuestAbandon()																	// ����Ʈ ���� ó�� �Լ�.
//{
//	DWORD QuestIdx = m_QuestUpdater->GetSelectedQuestID();											// ����Ʈ �������Ϳ��� ���� �� ����Ʈ ���̵� �޴´�.
//
//	m_QuestUpdater->CloseMsgBox();																	// ����Ʈ ���������� Ŭ���� �޽��� �ڽ��� ȣ���Ѵ�.
//
//	if( IsQuestComplete(QuestIdx) )																	// ����Ʈ �ε����� �Ϸ��� ����Ʈ���� üũ�Ѵ�.
//	{
//		return;																						// �Ϸ��� ����Ʈ���, ����ó���� �Ѵ�.
//	}
//
//	SEND_QUEST_IDX msg;																				// ����Ʈ ������ ���� ����ü�� �����Ѵ�.
//
//	msg.Category = MP_QUEST;																		// ī�װ��� ����Ʈ�� �����Ѵ�.
//	msg.Protocol = MP_QUEST_DELETE_SYN;																// ���������� ����Ʈ �輼 ��ũ�� �����Ѵ�.
//	msg.dwObjectID = HERO->GetID();																	// HERO ���̵� �����Ѵ�.
//	msg.MainQuestIdx = (WORD)QuestIdx;																// ����Ʈ �ε����� �����Ѵ�.
//
//	NETWORK->Send(&msg, sizeof(msg));																// ���õ� ������ �����Ѵ�.
//
//	//test
//	m_QuestUpdater->DeleteQuest(QuestIdx);															// ����Ʈ ���������� ����Ʈ �����Լ��� ȣ���Ѵ�.
//}

void CQuestManager::QuestAbandon( DWORD dwQuestIdx )												// ����Ʈ �����Լ�( ����Ʈ �ε��� ���� )
{
	if( IsQuestComplete(dwQuestIdx) )																// ���� ����Ʈ �ε����� ����Ʈ�� �Ϸ� �ߴ��� üũ�Ѵ�.
	{
		return;																						// �Ϸ��� ����Ʈ���, ���� ó���� �Ѵ�.
	}

	SEND_QUEST_IDX msg;																				// ����Ʈ ������ ���� ����ü�� �����Ѵ�.

	msg.Category = MP_QUEST;																		// ī�װ��� ����Ʈ�� �����Ѵ�.
	msg.Protocol = MP_QUEST_DELETE_SYN;																// ���������� ����Ʈ ���� ��ũ�� �����Ѵ�.
	msg.dwObjectID = HERO->GetID();																	// HERO ���̵� �����Ѵ�.
	msg.MainQuestIdx = (WORD)dwQuestIdx;															// ����Ʈ �ε����� �����Ѵ�.

	NETWORK->Send(&msg, sizeof(msg));																// ���õ� ������ �����Ѵ�.

	//test
	m_QuestUpdater->DeleteQuest( dwQuestIdx );														// ����Ʈ ���������� ����Ʈ ���� �Լ��� ȣ���Ѵ�.
}

void CQuestManager::QuestAbandonClose()																// ����Ʈ ���� �ݱ� �Լ�.
{
	m_QuestUpdater->CloseMsgBox();																	// ����Ʈ ���������� Ŭ���� �޽��� �ڽ� �Լ��� ȣ���Ѵ�.
}

void CQuestManager::AddQuestNpc( CSubQuestInfo* pSubQuestInfo )										// ����Ʈ npc�� �߰��ϴ� �Լ�.
{
	if( !pSubQuestInfo ) return;																	// ���� ����Ʈ ������ ������, ����ó���� �Ѵ�.

	cPtrList* pNpcList = pSubQuestInfo->GetQuestNpcList();											// ���� ����Ʈ ������ ����Ʈ npc����Ʈ�� �޴´�.

	if( !pNpcList ) return;																			// npc ����Ʈ ������ ��ȿ���� ������ ���� ó���� �Ѵ�.

	PTRLISTPOS npos = pNpcList->GetHeadPosition();													// npc ����Ʈ�� ���� �����ϰ� ��ġ������ �޴´�.

	while( npos )																					// npc ����Ʈ�� ��ġ ������ ��� �����Ͱ� ��ȿ�� ���� while���� ������.
	{
		CQuestNpc* pNpc = (CQuestNpc*)pNpcList->GetNext( npos );									// ��ġ������ npc ������ �޴´�.

		if( pNpc )																					// npc ������ ��ȿ�ϴٸ�,
		{
			cPtrList* pList = pNpc->GetNpcDataList();												// npc ������ ����Ʈ�� �޴´�.

			if( pList )																				// ����Ʈ ������ ��ȿ���� üũ�Ѵ�.
			{
				PTRLISTPOS pos = pList->GetHeadPosition();											// ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

				while( pos )																		// ��ġ ������ ��ȿ�� ���� while���� ������.
				{
					CQuestNpcData* pData = (CQuestNpcData*)pList->GetNext( pos );					// ��ġ�� �ش��ϴ� ����Ʈ npc ������ ������ �޴´�.

					if( pData )																		// ������ ������ ��ȿ���� üũ�Ѵ�.
					{
						QUESTNPCINFO* pInfo = m_QuestNpcTable.GetData( pData->GetNpcIdx() );		// ������ ������ npc �ε����� �����Ͽ� ����Ʈ npc ������ �޴´�.

						if( pInfo )																	// npc ������ ��ȿ���� üũ�Ѵ�.
						{
							MAP->AddQuestNpc( pInfo );												// �ʿ� ����Ʈ npc�� �߰��Ѵ�.
						}
					}
				}
			}
		}
	}
}

BOOL CQuestManager::IsQuestStarted( DWORD dwQuestIdx )												// ����Ʈ�� ���۵Ǿ����� ���θ� �����ϴ� �Լ�.
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );											// ���� ����Ʈ �ε�����, ����Ʈ ������ �޴´�.

	if( pQuest )																					// ����Ʈ ������ ��ȿ���� üũ�Ѵ�.
	{
		if( pQuest->IsSubQuestComplete( 0 ) )														// ����Ʈ�� ���� ����Ʈ�� �Ϸ� �Ǿ��ٸ�,
		{
			return FALSE;																			// FALSE�� �����Ѵ�.
		}
		else																						// �׷��� ������,
		{
			return pQuest->CanItemUse( dwQuestIdx );												// �������� ����� �� �ִ��� ���θ� �����Ѵ�.
		}
	}
	return FALSE;																					// FALSE�� �����Ѵ�.
}

char* CQuestManager::GetQuestTitle( DWORD dwQuestIdx )												// ����Ʈ Ÿ��Ʋ�� �����ϴ� �Լ�.
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );											// ���� ����Ʈ �ε����� ����Ʈ ������ �޴´�.

	if( !pQuest )	return NULL;																	// ����Ʈ ������ ��ȿ���� ������, NULL�� �����Ѵ�.

	//
	int Key = 0;																					// Ű ���� ���� ������ �����ϰ� 0���� �����Ѵ�.

	COMBINEKEY( dwQuestIdx, 0, Key );																// ����Ʈ �ε����� 0�� �����Ͽ� Ű���� �޴´�.

	CQuestString* pQString = m_QuestStringTable.GetData(Key);										// Ű ������ ����Ʈ ��Ʈ�� ������ �޴´�.

	if( pQString == NULL )	return NULL;															// ����Ʈ ��Ʈ�� ������ ��ȿ���� ������, NULL�� �����Ѵ�.

	cPtrList* p = pQString->GetTitle();																// ����Ʈ ��Ʈ���� Ÿ��Ʋ�� ����Ʈ �����ͷ� �޴´�.

	QString* string = (QString*)p->GetHead();														// ����Ʈ�� �ص带 ��Ʈ�� �����ͷ� �޴´�.

	return string->Str;																				// ��Ʈ�� �������� ��Ʈ���� �����Ѵ�.
}

CQuestString* CQuestManager::GetQuestString(DWORD dwMainQuestIdx, DWORD dwSubQuestIdx)				// ����Ʈ ��ȣ�� ���� �� ��ȣ�� ����Ʈ ��Ʈ���� ��ȯ�ϴ� �Լ�.
{
	int nCombineKey = 0 ;																			// �Ĺ��� Ű ���� ���� ������ �����ϰ� 0���� �����Ѵ�.

	COMBINEKEY( dwMainQuestIdx, dwSubQuestIdx, nCombineKey) ;										// ������ ��ȣ�� ������ ��ȣ�� �Ĺ��� Ű�� �����.

	CQuestString* pQuestString = m_QuestStringTable.GetData(nCombineKey) ;							// �Ĺ��� Ű�� ����Ʈ ��Ʈ�� ���̺��� ����Ʈ ��Ʈ���� �޴´�.

	return pQuestString ;																			// ����Ʈ ��Ʈ���� ��ȯ�Ѵ�.
}

void CQuestManager::QuestTimeLimit( DWORD dwQuestIdx, DWORD dwTime )								// ����Ʈ�� �ð��� �����ϴ� �Լ�.
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );											// ���� �ε����� ����Ʈ ������ �޴´�.

	if( !pQuest )	return;																			// ����Ʈ ������ ��ȿ���� ������ ���� ó���� �Ѵ�.

	//
	stTIME time;																					// �ð� ������ ���� ����ü�� �����Ѵ�.
	time.value = dwTime;																			// ���� �ð����� �����Ѵ�.

	int Key = 0;																					// Ű ���� ���� ������ �����ϰ� 0���� �����Ѵ�.

	COMBINEKEY( dwQuestIdx, 0, Key );																// ����Ʈ �ε����� 0�� �����Ͽ� Ű ���� �޴´�.

	CQuestString* pQString = m_QuestStringTable.GetData(Key);										// ���� Ű ���� ������ ����Ʈ ��Ʈ�� ������ �޴´�.

	if( pQString == NULL )	return;																	// ����Ʈ ��Ʈ�� ������ ��ȿ���� ������ ���� ó���� �Ѵ�.

	cPtrList* p = pQString->GetTitle();																// ��Ʈ���� Ÿ��Ʋ�� ����Ʈ �����ͷ� �޴´�.

	QString* string = (QString*)p->GetHead();														// ����Ʈ�� ��带 ��Ʈ�� �����ͷ� �޴´�.

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(299), string->Str,								// �ð� ������ �޽����� �����Ͽ�, ä��â���� ����Ѵ�.
					 time.GetYear()+2000, time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute() );	

	char msg[256] = {0, };																			// �ӽ� ���۸� �����Ѵ�.

	CompositString( msg, CHATMGR->GetChatMsg(299), string->Str,										// �ð� ������ �޽����� �����Ͽ�, ����� �޽����� �ϼ��Ѵ�.
					 time.GetYear()+2000, time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute() );	

	STATUSICONDLG->RemoveQuestTimeIcon( HERO, eStatusIcon_QuestTime );								// ����Ʈ�� �ð����� �������� �����Ѵ�.
}

void CQuestManager::RegistCheckTime( DWORD dwQuestIdx, BYTE bCheckType, DWORD dwCheckTime )			// �ð� üũ ����� �ϴ� �Լ�.
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );											// ���� ����Ʈ �ε����� ����Ʈ ������ �޴´�.

	if( !pQuest )	return;																			// ����Ʈ ������ ��ȿ���� ������ ���� ó���� �Ѵ�.

	pQuest->RegistCheckTime( bCheckType, dwCheckTime );												// ����Ʈ ������ �ð� üũ ���� ������ �����Ѵ�.

	STATUSICONDLG->AddQuestTimeIcon( HERO, eStatusIcon_QuestTime );									// ����Ʈ �ð� �������� �߰��Ѵ�.
}

void CQuestManager::UnRegistCheckTime( DWORD dwQuestIdx )											// �ð� üũ ����� �����ϴ� �Լ�.
{
	CQuest* pQuest = m_QuestTable.GetData( dwQuestIdx );											// ���� ����Ʈ �ε����� ����Ʈ ������ �޴´�.

	if( !pQuest )	return;																			// ����Ʈ ������ ��ȿ���� ������ ���� ó���� �Ѵ�.

	pQuest->UnRegistCheckTime();																	// ����Ʈ �������� UnRegistCheckTime�Լ��� ȣ���Ѵ�.

	STATUSICONDLG->RemoveQuestTimeIcon( HERO, eStatusIcon_QuestTime );								// ����Ʈ �ð� �������� �����Ѵ�.
}

void CQuestManager::QuestEventAck( DWORD dwQuestIdx )												// ����Ʈ �̺�Ʈ ��û �Լ�.
{
	WINDOWMGR->MsgBox( MBI_QUESTMSG, MBT_OK, "�⼮üũ�� �Ǿ����ϴ�." );							// �⼮ üũ�� �Ǿ��ٴ� �޽����� ����Ѵ�.
}

void CQuestManager::QuestEventNack( DWORD dwQuestIdx, DWORD dwError )								// ����Ʈ �̺�Ʈ ���� �Լ�.
{
	switch( dwError )																				// ������ Ȯ���Ѵ�.
	{
	case 1:																							// ������ 1���� ������,
		{
			WINDOWMGR->MsgBox( MBI_QUESTMSG, MBT_OK, "�̹� �⼮üũ�� �Ǿ� ������ ������ �� �����ϴ�." );	// �޽����� ����Ѵ�.
		}
		break;
	case 2:																							// ������ 2���� ������,
		{
			WINDOWMGR->MsgBox( MBI_QUESTMSG, MBT_OK, "Ȩ������ �⼮üũ�� �Ǿ� ���� �ʾ� ������ ������ �� �����ϴ�." );	// �޽����� ����Ѵ�.
		}
		break;
	}
}
void CQuestManager::Quest_Maindata_Load( void* pMsg )												// ���� �����͸� �ε��ϴ� �Լ�.
{
	SEND_MAINQUEST_DATA* pmsg = (SEND_MAINQUEST_DATA*)pMsg;											// ���� ����Ʈ �����͸� �޴� ������ ������ �����ϰ� �޽����� �޴´�.

	if(pmsg->wCount==0)		return;																	// ī��Ʈ�� 0�� ������ ���� ó���� �Ѵ�.
	
	for(int i=0; i<pmsg->wCount; i++)																// ī��Ʈ �� ��ŭ for���� ������.
	{
		SetMainQuestData( pmsg->QuestList[i].QuestIdx, pmsg->QuestList[i].state.value,				// ���� ����Ʈ �����͸� �����Ѵ�.
			pmsg->QuestList[i].EndParam, pmsg->QuestList[i].Time, pmsg->QuestList[i].CheckType, 
			pmsg->QuestList[i].CheckTime );
	}

	ProcessNpcMark();																				// npc ��ũ ���μ����� �����Ѵ�.
}

//void CQuestManager::Quest_Maindata_Load( void* pMsg )												// ���� �����͸� �ε��ϴ� �Լ�.
//{
//	SEND_MAINQUEST_DATA* pmsg = (SEND_MAINQUEST_DATA*)pMsg;											// ���� ����Ʈ �����͸� �޴� ������ ������ �����ϰ� �޽����� �޴´�.
//
//	if(pmsg->wCount==0)		return;																	// ī��Ʈ�� 0�� ������ ���� ó���� �Ѵ�.
//	
//	for(int i=0; i<pmsg->wCount; i++)																// ī��Ʈ �� ��ŭ for���� ������.
//	{
//		SetMainQuestData( pmsg->QuestList[i].QuestIdx, pmsg->QuestList[i].state.value,				// ���� ����Ʈ �����͸� �����Ѵ�.
//			pmsg->QuestList[i].EndParam, pmsg->QuestList[i].Time, pmsg->QuestList[i].CheckType, 
//			pmsg->QuestList[i].CheckTime );
//	}
//
//	//if( pmsg->wCount > 1 )
//	//{
//	//	CQuestQuickViewDialog* pQuickView = NULL ;														// ����Ʈ �˸��� ���̾�α� ������ ���� �����͸� �����ϰ� nulló�� �Ѵ�.
//	//	pQuickView = GAMEIN->GetQuestQuickViewDialog() ;												// ����Ʈ �˸��� ���̾�α� ������ �޴´�.
//
//	//	if( pQuickView )																				// ����Ʈ �˸��� ���̾�α� ������ ��ȿ�ϴٸ�,
//	//	{
//	//		pQuickView->MainDataLoad() ;																// ����Ʈ �˸����� ���� ����Ʈ �����͸� �ε��Ѵ�.
//	//	}
//	//}
//	
//	ProcessNpcMark();																				// npc ��ũ ���μ����� �����Ѵ�.
//}

void CQuestManager::Quest_Subdata_Load( void* pMsg )												// ���� �����͸� �ε��ϴ� �Լ�.
{
	SEND_SUBQUEST_DATA* pmsg = (SEND_SUBQUEST_DATA*)pMsg;											// ���� ����Ʈ �����͸� �޴� ������ ������ �����ϰ� �޽����� �޴´�.

	if(pmsg->wCount==0)		return;																	// ī��Ʈ�� 0�� ������ ���� ó���� �Ѵ�.
	
	for(int i=0; i<pmsg->wCount; i++)																// ī��Ʈ �� ��ŭ for���� ������.
	{
		SetSubQuestData( pmsg->QuestList[i].QuestIdx, pmsg->QuestList[i].SubQuestIdx,				// ���� ����Ʈ �����͸� �����Ѵ�.
							pmsg->QuestList[i].state, pmsg->QuestList[i].time );
	}

	ProcessNpcMark();																				// npc ��ũ ���μ����� �����Ѵ�.
}

void CQuestManager::Quest_Item_Load( void* pMsg )													// ����Ʈ �������� �ε��ϴ� �Լ�.
{
	SEND_QUESTITEM* pmsg = (SEND_QUESTITEM*)pMsg;													// ����Ʈ ������ ������ �޴� ������ ������ �����ϰ� �޽����� �޴´�.

	if(pmsg->wCount==0)		return;																	// ī��Ʈ�� 0�� ������ ���� ó���� �Ѵ�.

	for(int i=0; i<pmsg->wCount; i++)																// ī��Ʈ ����ŭ for���� ������.
	{			
		m_QuestUpdater->QuestItemAdd(m_QuestItemMGR.GetQuestItem(pmsg->ItemList[i].ItemIdx),		// ����Ʈ �������Ϳ� ����Ʈ �������� �߰��Ѵ�.	
			pmsg->ItemList[i].Count);			
	}
}

void CQuestManager::Quest_Start_Ack( void* pMsg )													// ����Ʈ ���� ���� �Լ�.
{
	SEND_QUEST_IDX* pmsg = (SEND_QUEST_IDX*)pMsg;													// ����Ʈ ������ ���� ������ ������ �����ϰ� �޽����� �޴´�.

	StartQuest( pmsg->MainQuestIdx, pmsg->dwFlag, pmsg->dwData, pmsg->dwTime );						// ���� ������ ������ ����Ʈ�� �����Ѵ�.
}

void CQuestManager::SubQuest_Start_Ack( void* pMsg )												// ���� ����Ʈ ���� ���� �Լ�.
{
	SEND_QUEST_IDX* pmsg = (SEND_QUEST_IDX*)pMsg;													// ����Ʈ ������ ���� ������ ������ �����ϰ� �޽����� �޴´�.

	StartSubQuest( pmsg->MainQuestIdx, pmsg->SubQuestIdx, pmsg->dwFlag, pmsg->dwData, pmsg->dwTime );// ���� ����Ʈ�� �����Ѵ�.

	ProcessNpcMark();																				// npc ��ũ ���μ����� �����Ѵ�.
}

//void CQuestManager::SubQuest_Start_Ack( void* pMsg )												// ���� ����Ʈ ���� ���� �Լ�.
//{
//	SEND_QUEST_IDX* pmsg = (SEND_QUEST_IDX*)pMsg;													// ����Ʈ ������ ���� ������ ������ �����ϰ� �޽����� �޴´�.
//
//	StartSubQuest( pmsg->MainQuestIdx, pmsg->SubQuestIdx, pmsg->dwFlag, pmsg->dwData, pmsg->dwTime );// ���� ����Ʈ�� �����Ѵ�.
//
//	ProcessNpcMark();																				// npc ��ũ ���μ����� �����Ѵ�.
//
//	//CQuestQuickViewDialog* pDlg = NULL ;															// ����Ʈ �˸��� ���̾�α� ������ ���� �����͸� �����ϰ� nulló�� �Ѵ�.
//	//pDlg = GAMEIN->GetQuestQuickViewDialog() ;														// ����Ʈ �˸��� ���̾�α� ������ �޴´�.
//
//	//if( pDlg )																						// ����Ʈ �˸��� ���̾�α� ������ ��ȿ�ϸ�,
//	//{
//	//	pDlg->StartSubQuest( pmsg ) ;																// ����Ʈ �˸��� ���̾�α׸� ������Ʈ�Ѵ�.
//	//}
//
//	CQuestDialog* pQuestDialog = GAMEIN->GetQuestDialog() ;
//
//	if( pQuestDialog )
//	{
//		CQuestInfo* pQuestInfo = NULL ;												// ����Ʈ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//		pQuestInfo = QUESTMGR->GetQuestInfo(pmsg->MainQuestIdx) ;							// ����Ʈ �Ŵ����� ����, ���ڷ� �Ѿ�� ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.
//
//		int nSubQuestCount = 0 ;													// ���� ����Ʈ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
//		nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// ����Ʈ ������ ���� ���� ����Ʈ ī��Ʈ�� �޴´�.
//
//		if( pmsg->SubQuestIdx == nSubQuestCount-1 )
//		{
//			pQuestDialog->UpdateQuestList(pmsg->MainQuestIdx, pmsg->SubQuestIdx) ;
//		}
//	}
//
//}

void CQuestManager::Quest_End_Ack( void* pMsg )														// ����Ʈ ���� ���� �Լ�.
{
	SEND_QUEST_IDX* msg = (SEND_QUEST_IDX*)pMsg;													// ����Ʈ ������ ���� �����͸� �����ϰ�, �޽����� �޴´�.
		
	EndQuest( msg->MainQuestIdx, msg->dwFlag, msg->dwData, msg->dwTime );							// ����Ʈ�� �����Ѵ�.

	QuestAbandonClose();																			// ����Ʈ ���� �ݱ� �Լ��� �����Ѵ�.

	ProcessNpcMark();	
}

//void CQuestManager::Quest_End_Ack( void* pMsg )														// ����Ʈ ���� ���� �Լ�.
//{
//	SEND_QUEST_IDX* msg = (SEND_QUEST_IDX*)pMsg;													// ����Ʈ ������ ���� �����͸� �����ϰ�, �޽����� �޴´�.
//		
//	EndQuest( msg->MainQuestIdx, msg->dwFlag, msg->dwData, msg->dwTime );							// ����Ʈ�� �����Ѵ�.
//
//	QuestAbandonClose();																			// ����Ʈ ���� �ݱ� �Լ��� �����Ѵ�.
//
//	ProcessNpcMark();																				// npc ��ũ ���μ����� �����Ѵ�.
//
//	CQuestInfo* pQuestInfo = NULL ;												// ����Ʈ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	pQuestInfo = QUESTMGR->GetQuestInfo(msg->MainQuestIdx) ;							// ����Ʈ �Ŵ����� ����, ���ڷ� �Ѿ�� ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.
//
//	int nSubQuestCount = 0 ;													// ���� ����Ʈ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
//	nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// ����Ʈ ������ ���� ���� ����Ʈ ī��Ʈ�� �޴´�.
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
//	CQuestQuickViewDialog* pDlg = NULL ;															// ����Ʈ �˸��� ���̾�α� ������ ���� �����͸� �����ϰ� nulló�� �Ѵ�.
//	pDlg = GAMEIN->GetQuestQuickViewDialog() ;														// ����Ʈ �˸��� ���̾�α� ������ �޴´�.
//
//	if( pDlg )																						// ����Ʈ �˸��� ���̾�α� ������ ��ȿ�ϸ�,
//	{
//		pDlg->DeleteQuest(msg->dwObjectID) ;														// ����Ʈ �˸��� ���̾�α׸� ������Ʈ�Ѵ�.
//	}
//
//	//CQuestQuickViewDialog* pDlg = NULL ;															// ����Ʈ �˸��� ���̾�α� ������ ���� �����͸� �����ϰ� nulló�� �Ѵ�.
//	//pDlg = GAMEIN->GetQuestQuickViewDialog() ;														// ����Ʈ �˸��� ���̾�α� ������ �޴´�.
//
//	//if( pDlg )																						// ����Ʈ �˸��� ���̾�α� ������ ��ȿ�ϸ�,
//	//{
//	//	pDlg->EndQuest( msg ) ;																		// ����Ʈ �˸��� ���̾�α׸� ������Ʈ�Ѵ�.
//	//}
//
//	//CQuestQuickViewDialog* pDlg = NULL ;															// ����Ʈ �˸��� ���̾�α� ������ ���� �����͸� �����ϰ� nulló�� �Ѵ�.
//	//pDlg = GAMEIN->GetQuestQuickViewDialog() ;														// ����Ʈ �˸��� ���̾�α� ������ �޴´�.
//
//	//if( pDlg )																						// ����Ʈ �˸��� ���̾�α� ������ ��ȿ�ϸ�,
//	//{
//	//	pDlg->EndQuest( msg ) ;																		// ����Ʈ �˸��� ���̾�α׸� ������Ʈ�Ѵ�.
//
//	//	for( int count = 0 ; count < MAX_QUICKVIEW_QUEST ; ++count )								// ����Ʈ �˸��� ���̾�α׿� ��� ������ �ִ� �� ��ŭ for���� ������.
//	//	{
//	//		if(m_QuickViewQuestID[count] != 0)														// ����Ʈ �ε����� 0�� �ƴ϶��,
//	//		{
//	//			if( m_QuickViewQuestID[count] == msg->MainQuestIdx )								// �����Ϸ��� ����Ʈ�� �ε����� ������,
//	//			{
//	//				DeleteQuestIDToQuickView(count) ;												// �����ϰ� �ִ� ����Ʈ �ε����� �����Ѵ�.
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

void CQuestManager::SubQuest_End_Ack( void* pMsg )													// ���� ����Ʈ ���� ���� �Լ�.
{
	SEND_QUEST_IDX* msg = (SEND_QUEST_IDX*)pMsg;													// ����Ʈ ������ ���� �����͸� �����ϰ�, �޽����� �޴´�.

	EndSubQuest( msg->MainQuestIdx, msg->SubQuestIdx, msg->dwFlag, msg->dwData, msg->dwTime );		// ���� ����Ʈ�� �����Ѵ�.

	QuestAbandonClose();																			// ����Ʈ ���� �ݱ� �Լ��� �����Ѵ�.

	ProcessNpcMark();																				// npc ��ũ ���μ����� �����Ѵ�.

	GAMENOTIFYMGR->EndSubQuest(msg) ;																// ���� ����Ʈ ���� ������ �߰��Ѵ�.
}

//void CQuestManager::SubQuest_End_Ack( void* pMsg )													// ���� ����Ʈ ���� ���� �Լ�.
//{
//	SEND_QUEST_IDX* msg = (SEND_QUEST_IDX*)pMsg;													// ����Ʈ ������ ���� �����͸� �����ϰ�, �޽����� �޴´�.
//
//	EndSubQuest( msg->MainQuestIdx, msg->SubQuestIdx, msg->dwFlag, msg->dwData, msg->dwTime );		// ���� ����Ʈ�� �����Ѵ�.
//
//	QuestAbandonClose();																			// ����Ʈ ���� �ݱ� �Լ��� �����Ѵ�.
//
//	ProcessNpcMark();																				// npc ��ũ ���μ����� �����Ѵ�.
//
//	//CQuestQuickViewDialog* pDlg = NULL ;															// ����Ʈ �˸��� ���̾�α� ������ ���� �����͸� �����ϰ� nulló�� �Ѵ�.
//	//pDlg = GAMEIN->GetQuestQuickViewDialog() ;														// ����Ʈ �˸��� ���̾�α� ������ �޴´�.
//
//	//if( pDlg )																						// ����Ʈ �˸��� ���̾�α� ������ ��ȿ�ϸ�,
//	//{
//	//	pDlg->EndSubQuest( msg ) ;																	// ����Ʈ �˸��� ���̾�α׸� ������Ʈ�Ѵ�.
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
//	GAMENOTIFYMGR->EndSubQuest(msg) ;																// ���� ����Ʈ ���� ������ �߰��Ѵ�.
//}

void CQuestManager::SubQuest_Update( void* pMsg )													// ���� ����Ʈ ������Ʈ �Լ�.
{
	SEND_SUBQUEST_UPDATE* msg = (SEND_SUBQUEST_UPDATE*)pMsg;										// ���� ����Ʈ ������Ʈ ������ ���� �����͸� �����ϰ� �޽����� �޴´�.

	UpdateSubQuestData( msg->wQuestIdx, msg->wSubQuestIdx, msg->dwMaxCount, msg->dwData, msg->dwTime );	// ���� ����Ʈ �����͸� ������Ʈ �Ѵ�.

	GAMENOTIFYMGR->UpdateSubQuest(msg) ;															// ����Ʈ ������Ʈ ������ �߰��Ѵ�.
}	

//void CQuestManager::SubQuest_Update( void* pMsg )													// ���� ����Ʈ ������Ʈ �Լ�.
//{
//	SEND_SUBQUEST_UPDATE* msg = (SEND_SUBQUEST_UPDATE*)pMsg;										// ���� ����Ʈ ������Ʈ ������ ���� �����͸� �����ϰ� �޽����� �޴´�.
//
//	UpdateSubQuestData( msg->wQuestIdx, msg->wSubQuestIdx, msg->dwMaxCount, msg->dwData, msg->dwTime );	// ���� ����Ʈ �����͸� ������Ʈ �Ѵ�.
//
//	//CQuestQuickViewDialog* pDlg = NULL ;															// ����Ʈ �˸��� ���̾�α� ������ ���� �����͸� �����ϰ� nulló�� �Ѵ�.
//	//pDlg = GAMEIN->GetQuestQuickViewDialog() ;														// ����Ʈ �˸��� ���̾�α� ������ �޴´�.
//
//	//if( pDlg )																						// ����Ʈ �˸��� ���̾�α� ������ ��ȿ�ϸ�,
//	//{
//	//	pDlg->UpdateQuest( msg ) ;																	// ����Ʈ �˸��� ���̾�α׸� ������Ʈ�Ѵ�.
//	//}
//
//	CQuestString* pUpdateString = NULL ;													// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//	pUpdateString = QUESTMGR->GetQuestString(msg->wQuestIdx, msg->wSubQuestIdx) ;			// ����Ʈ �ε�����, ���� ����Ʈ �ε����� ����Ʈ ��Ʈ���� �޴´�.
//
//	if( pUpdateString )
//	{
//		pUpdateString->SetCount(msg->dwData) ;
//		pUpdateString->SetTotalCount(msg->dwMaxCount) ;
//	}
//
//
//	GAMENOTIFYMGR->UpdateSubQuest(msg) ;															// ����Ʈ ������Ʈ ������ �߰��Ѵ�.
//}	

void CQuestManager::QuestItem_Give( void* pMsg )													// ���� ������ ��ȯ �Լ�.
{
	SEND_QUESTITEM_IDX* pmsg = (SEND_QUESTITEM_IDX*)pMsg;											// ����Ʈ ������ ������ ���� �����͸� �����ϰ� �޽����� �޴´�.

	QuestItemDelete( pmsg->dwItemIdx );																// ����Ʈ �������� �����Ѵ�.
}

void CQuestManager::QuestItem_Take( void* pMsg )													// ����Ʈ ������ ���� �Լ�.
{
	SEND_QUESTITEM_IDX* pmsg = (SEND_QUESTITEM_IDX*)pMsg;											// ����Ʈ ������ ������ ���� �����͸� �����ϰ� �޽����� �޴´�.

	QuestItemAdd( pmsg->dwItemIdx, pmsg->dwItemNum );												// ����Ʈ �������� �߰��Ѵ�.

	EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Quest);										// �������� �����ϴ� ����Ʈ�� �����Ѵ�.

//			ITEMMGR->ItemDropEffect( pmsg->dwItemIdx );
}

void CQuestManager::QuestItem_Update( void* pMsg )													// ����Ʈ ������ ������Ʈ �Լ�.
{
	SEND_QUESTITEM_IDX* pmsg = (SEND_QUESTITEM_IDX*)pMsg;											// ����Ʈ ������ ������ ���� �����͸� �����ϰ� �޽����� �޴´�.
		
	QuestItemUpdate( eQItem_SetCount, pmsg->dwItemIdx, pmsg->dwItemNum );							// ����Ʈ �������� ������Ʈ �Ѵ�.

	EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Quest);										// �������� �����ϴ� ����Ʈ�� �����Ѵ�.
//			ITEMMGR->ItemDropEffect( pmsg->dwItemIdx );
}

void CQuestManager::Quest_Delete_Ack( void* pMsg )													// ����Ʈ ������ �����ϴ� �Լ�.
{
	SEND_QUEST_IDX* pmsg = (SEND_QUEST_IDX*)pMsg;													// ����Ʈ ������ ���� �����͸� �����ϰ� �޽����� �޴´�.

	DeleteQuest( pmsg->MainQuestIdx, (BOOL)pmsg->dwFlag );											// ����Ʈ�� �����Ѵ�.

	ProcessNpcMark();																				// npc ��ũ ���μ����� �����Ѵ�.
}

//void CQuestManager::Quest_Delete_Ack( void* pMsg )													// ����Ʈ ������ �����ϴ� �Լ�.
//{
//	SEND_QUEST_IDX* pmsg = (SEND_QUEST_IDX*)pMsg;													// ����Ʈ ������ ���� �����͸� �����ϰ� �޽����� �޴´�.
//
//	DeleteQuest( pmsg->MainQuestIdx, (BOOL)pmsg->dwFlag );											// ����Ʈ�� �����Ѵ�.
//
//	ProcessNpcMark();																				// npc ��ũ ���μ����� �����Ѵ�.
//
//	CQuestInfo* pQuestInfo = NULL ;												// ����Ʈ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	pQuestInfo = QUESTMGR->GetQuestInfo(pmsg->MainQuestIdx) ;							// ����Ʈ �Ŵ����� ����, ���ڷ� �Ѿ�� ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.
//
//	int nSubQuestCount = 0 ;													// ���� ����Ʈ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
//	nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// ����Ʈ ������ ���� ���� ����Ʈ ī��Ʈ�� �޴´�.
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
//	CQuestQuickViewDialog* pQuickView = NULL ;														// ����Ʈ �˸��� ���̾�α� ������ ���� �����͸� �����ϰ� nulló�� �Ѵ�.
//	pQuickView = GAMEIN->GetQuestQuickViewDialog() ;												// ����Ʈ �˸��� ���̾�α� ������ �޴´�.
//
//	if( pQuickView )																				// ����Ʈ �˸��� ���̾�α� ������ ��ȿ�ϴٸ�,
//	{
//		pQuickView->DeleteQuest(pmsg->MainQuestIdx) ;												// ����Ʈ �˸����� ���� ����Ʈ �����͸� �ε��Ѵ�.
//	}
//}

void CQuestManager::Quest_Delete_Confirm_Ack( void* pMsg )											// ����Ʈ ���� ���� ��û �Լ�.
{
	SEND_QUEST_IDX_SIMPLE* pmsg = (SEND_QUEST_IDX_SIMPLE*)pMsg;										// ���� ������ ���� �����͸� �����ϰ�, �޽����� �޴´�.
			
	m_QuestUpdater->DeleteQuest(pmsg->MainQuestIdx);												// ����Ʈ ���������� ����Ʈ �����Լ��� �����Ѵ�.
}

void CQuestManager::Quest_TakeItem_Ack( void* pMsg )												// ����Ʈ ������ ȹ�� ���� �Լ�.
{
	ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;												// ������ ȹ�� ������ ���� �����͸� �����ϰ� �޽����� �޴´�.

	CItem* pItem = NULL;																			// ������ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.

	for( int i = 0; i < pmsg->ItemNum; ++i )														// ������ ����ŭ for���� ������.
	{
		if( pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->GetItem(i)->Position ) )		// ������ ���������� ������ ������ �޾� ������ ��ȿ���� üũ�Ѵ�.
		{
			pItem->SetItemBaseInfo( *( pmsg->GetItem(i) ) );												// ������ �⺻ ������ �����Ѵ�.
		}
		else																						// ������ ������ ��ȿ���� ������,
		{
			pItem = ITEMMGR->MakeNewItem(pmsg->GetItem(i),"MP_QUEST_TAKEITEM_ACK");					// ���� �������� ������ �����ͷ� �޴´�.
				
			BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);									// �κ��丮�� �������� �߰��ϰ� ����� �޴´�.

			if(!rt)																					// ���� �ߴٸ�,
			{
				ASSERT(0);																			// assert ó��.
			}
		}
	}
	// 
	cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_DIVIDE_INV );									// ����̵� �ڽ� ������ �޴´�.

	if( pDlg )																						// ������ ��ȿ���� üũ�Ѵ�.
	{
		((cDivideBox*)pDlg)->ExcuteDBFunc( 0 );														// �ڽ��� �Լ��� �����Ѵ�.
	}

	QUICKMGR->RefreshQickItem();																	// �� �Ŵ����� �� ������ ���ΰ�ħ�� �����Ѵ�.

	if( !pItem ) return;																			// ������ ������ ��ȿ���� ������ ���� ó���� �Ѵ�.

	{
		CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(235), pItem->GetItemInfo()->ItemName );		// ����Ʈ �������� �������� ����ٴ� �޽����� ����Ѵ�.
	}

	EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Quest);										// ������ ȹ�� ����Ʈ�� �����Ѵ�.

	//GAMENOTIFYMGR->TakeItem(pItem) ;																// ����Ʈ ������ ȹ�� ������ ����.
}

void CQuestManager::Quest_Takemoney_Ack( void* pMsg )												// ����Ʈ �Ӵ� ȹ�� ���� �Լ�.
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;																// �޽����� ���� �����͸� �����ϰ� �޽����� �޴´�.

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(236), AddComma(pmsg->dwData) );					// ��带 ����ٴ� �޽����� ����Ѵ�.
	
	EFFECTMGR->StartHeroEffectProcess(eEffect_GetMoney);											// �������� ����ٴ� ����Ʈ�� �����Ѵ�.

	//GAMENOTIFYMGR->TakeMoney(AddComma(pmsg->dwData)) ;												// ����Ʈ ���� ��� ȹ�� ������ ����.
}

void CQuestManager::Quest_TakeExp_Ack( void* pMsg )													// ����Ʈ ����ġ�� ȹ�� ���� �Լ�.
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;																// �޽����� ���� �����͸� �����ϰ� �޽����� �޴´�.

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(237), pmsg->dwData );							// ����ġ�� ����ٴ� �޽����� ����Ѵ�.

	//GAMENOTIFYMGR->TakeExp(pmsg->dwData) ;															// ����Ʈ ���� ����ġ ȹ�� ������ ����.
	
//	HERO->SetExpPoint(HERO->GetExpPoint() + pmsg->dwData) ;											// HERO�� ����ġ�� �����Ѵ�.
}

void CQuestManager::Quest_TakesExp_Ack( void* pMsg )												// ����Ʈ sp ȹ�� ���� �Լ�.
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;																// �޽����� ���� �����͸� �����ϰ� �޽����� �޴´�.

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(238), pmsg->dwData );							// sp�� ����ٴ� �޽����� ����Ѵ�.

	HERO->SetExpPoint(HERO->GetExpPoint() + pmsg->dwData) ;											// HERO�� ����ġ�� �����Ѵ�.
}

void CQuestManager::Quest_GiveItem_Ack( void* pMsg )												// ����Ʈ �������� ��ȯ�ϴ� �Լ�.
{
	MSG_EXCHANGE_REMOVEITEM* pmsg = (MSG_EXCHANGE_REMOVEITEM*)pMsg;									// ��ȯ ���� ������ ������ �޴� �����͸� �����ϰ� �޽����� �޴´�.

	CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->wAbsPosition );				// ������ ��ġ�� ������ ������ �޴´�.

	WORD wTableIdx = ITEMMGR->GetTableIdxForAbsPos( pmsg->wAbsPosition );							// �� �ε����� ��ġ ������ ���̺� �ε����� �޴´�.

	const ITEMBASE* pItemInfo = ITEMMGR->GetItemInfoAbsIn( HERO, pmsg->wAbsPosition );				// ������ �⺻ ������ �޴´�.

	ITEMMGR->DeleteItemofTable( wTableIdx, pmsg->wAbsPosition, FALSE );								// ������ �Ŵ����� ���̺��� �������� �����Ѵ�.

	QUICKMGR->RefreshQickItem();																	// �� �Ŵ����� �� ������ ���ΰ�ħ�� �����Ѵ�.

	if( !pItem ) return;																			// ������ ������ ��ȿ���� ������ ���� ó���� �Ѵ�.

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(297), pItem->GetItemInfo()->ItemName );			// �������� ������ٴ� �޽����� ����Ѵ�.
}

// 071026 LYW --- QuestManager : Add function to decrease item.
void CQuestManager::Quest_GiveItems_Ack( void* pMsg )
{
	MSG_EXCHANGE_DECREASEITEM* pmsg = (MSG_EXCHANGE_DECREASEITEM*)pMsg;								// ��ȯ ���� ������ ������ �޴� �����͸� �����ϰ� �޽����� �޴´�.

	CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->wAbsPosition );				// ������ ��ġ�� ������ ������ �޴´�.

	WORD wTableIdx = ITEMMGR->GetTableIdxForAbsPos( pmsg->wAbsPosition );							// �� �ε����� ��ġ ������ ���̺� �ε����� �޴´�.

	const ITEMBASE* pItemInfo = ITEMMGR->GetItemInfoAbsIn( HERO, pmsg->wAbsPosition );				// ������ �⺻ ������ �޴´�.

	//pItemInfo->Durability = pmsg->nItemCount ;

	pItem->SetDurability(pmsg->nItemCount) ;

	QUICKMGR->RefreshQickItem();																	// �� �Ŵ����� �� ������ ���ΰ�ħ�� �����Ѵ�.

	if( !pItem ) return;																			// ������ ������ ��ȿ���� ������ ���� ó���� �Ѵ�.

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(297), pItem->GetItemInfo()->ItemName );			// �������� ������ٴ� �޽����� ����Ѵ�.
}

// 071028 LYW --- CQuestManager : Add function to process give item error.
void CQuestManager::Quest_GiveItem_Error( void* pMsg )						// ����Ʈ ������ �ݳ��� ������ ���� ó���� �ϴ� �Լ�.
{
	ASSERT(pMsg);

	//CHATMGR->AddMsg( CTC_QUEST, "����Ʈ ������ �ݳ��� ���� �߽��ϴ�." );	// ����Ʈ ������ �ݳ��� ���� �ߴٴ� ���� �޽����� ����.
	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg( 1285 ) );

	MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg ;									// ���� ó���� �´� ������ ����ȯ�� �Ѵ�.

	sUSERINFO_ERR_GIVEITEM EGI ;											// ���� ������ ���� ����ü�� �����Ѵ�.
	memset(&EGI, 0, sizeof(sUSERINFO_ERR_GIVEITEM)) ;						// ����ü�� �ʱ�ȭ �Ѵ�.

	if( HERO == NULL ) return ;												// ����� ������ ������ ���� ó���� �Ѵ�.

	strcpy(EGI.UserName, HERO->GetObjectName()) ;							// ���� ĳ������ �̸��� �����Ѵ�.
	EGI.bCaption = (BYTE)pmsg->dwData1 ;									// ���� �߻� �Լ��� �����Ѵ�.
	EGI.bErrNum	 = (BYTE)pmsg->dwData2 ;									// ���� ��ȣ�� �����Ѵ�.
	EGI.dwQuestIdx = pmsg->dwData3 ;										// ����Ʈ �ε����� �����Ѵ�.
	EGI.dwItemIdx  = pmsg->dwData4 ;										// ������ �ε����� �����Ѵ�.

	char strFilePath[MAX_PATH] = {0, } ;									// ������ ����� ��θ� ���� ���۸� �����ϰ� �ʱ�ȭ �Ѵ�.

	wsprintf( strFilePath, "%s\\data\\interface\\log\\%d.EGI", 
		DIRECTORYMGR->GetFullDirectoryName(eLM_Root),HERO->GetID()); // ��ο� ���ϸ��� �����Ѵ�.

	HANDLE hFile = CreateFile( strFilePath, GENERIC_WRITE, 0, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ) ;						// ������ �����Ѵ�.

	if( hFile == INVALID_HANDLE_VALUE )										// ����� ���� ������,
	{
		return ;															// return.
	}

	DWORD dwSize ;															// ����� ���� ������ �����Ѵ�.

	WriteFile( hFile, &EGI, sizeof(EGI), &dwSize, NULL ) ;					// ���� ������ ����Ѵ�.

	CloseHandle( hFile ) ;
}

void CQuestManager::Quest_GiveMoney_Ack( void* pMsg )												// �Ӵϸ� �ִ� �Լ�.
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;																// �޽����� ���� �����͸� �����ϰ� �޽����� �޴´�.

	CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(236), AddComma(pmsg->dwData) );					// ����Ʈ �������� ��带 ����ٴ� �޽����� ����Ѵ�.
}

void CQuestManager::Quest_Time_Limit( void* pMsg )													// ����Ʈ �ð� ���� �Լ�.
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;															// �޽����� ���� �����͸� �����ϰ� �޽����� �޴´�.

	QuestTimeLimit( pmsg->dwData1, pmsg->dwData2 );													// ���� ������ ����Ʈ �ð� ������ �Ѵ�.
}

void CQuestManager::Quest_Regist_Checktime( void* pMsg )											// �ð� üũ ����� �ϴ� �Լ�.
{
	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;															// �޽����� ���� �����͸� �����ϰ� �޽����� �޴´�.

	RegistCheckTime( pmsg->dwData1, (BYTE)pmsg->dwData2, pmsg->dwData3 );							// �ð� üũ�� ����Ѵ�.
}

void CQuestManager::Quest_Unregist_Checktime( void* pMsg )											// �ð� üũ ����� ����ϴ� �Լ�.
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;																// �޽����� ���� �����͸� �����ϰ� �޽����� �޴´�.

	UnRegistCheckTime( pmsg->dwData );																// �ð� üũ�� ����Ѵ�.
}

void CQuestManager::QuestEvent_NpcTalk_Ack( void* pMsg )											// npc ��ȭ ��û �̺�Ʈ �Լ�.
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;															// �޽����� ���� �����͸� �����ϰ� �޽����� �޴´�.

	QuestEventAck( pmsg->dwData1 );																	// ����Ʈ �̺�Ʈ�� �����Ѵ�.
}

void CQuestManager::QuestEvent_NpcTalk_Nack( void* pMsg )											// npc ��ȭ��û ���� �̺�Ʈ �Լ�.
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;															// �޽����� ���� �����͸� �����ϰ� �޽����� �޴´�.

	QuestEventNack( pmsg->dwData1, pmsg->dwData2 );													// ����Ʈ �̺�Ʈ ���� ó���� �Ѵ�.
}

void CQuestManager::Quest_Execute_Error( void* pMsg )												// ����Ʈ ���� ���� ó�� �Լ�.
{
	MSG_INT* pmsg = (MSG_INT*)pMsg;																	// �޽����� ���� �����͸� �����ϰ� �޽����� �޴´�.

	int nMsgNum = 0 ;																				// ���� �޽��� ��ȣ�� ������ ������ �����ϰ� 0���� �����Ѵ�.

	switch(pmsg->nData)																				// �޽����� �Ѿ�� ���� �ڵ带 Ȯ���Ѵ�.
	{
	case e_EXC_ERROR_NO_ERROR :					nMsgNum = 990 ;		break ;							// ���� �ڵ忡 �´� �޽��� ��ȣ�� �����Ѵ�.
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
	// 071223 LYW --- QuestManager : ����Ʈ ���� �޽��� ����� �˸�â���� ä��â ������� �ٲ۴�.
	case e_EXC_ERROR_NOT_SAME_WEAPONKIND :
		{
			CHATMGR->AddMsg( CTC_CHEAT_1, CHATMGR->GetChatMsg(1002) ) ;
			return ;
		}
		break ;
	case e_EXC_ERROR_NOT_SAME_WEAPONKIND_COUNT :nMsgNum = 1003 ;	break ;
	case e_EXC_ERROR_CANT_RECEIVE_MONEY :		nMsgNum = 1004 ;	break ;
	}

	GAMEIN->GetNpcScriptDialog()->EndDialog();														// npc ��ũ��Ʈ ���̾�α׷� end ���̾�α׸� ȣ���Ѵ�.
	GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);;												// npc ��ũ��Ʈ ���̾�α׸� ��Ȱ��ȭ �Ѵ�.
	WINDOWMGR->MsgBox( MBI_QUESTMSG, MBT_OK, CHATMGR->GetChatMsg(nMsgNum) );						// ����ǰ�� �� ������ ���� ���� �Ұ���� �޽����� ����Ѵ�.
}

void CQuestManager::Quest_Error_Ext( void* pMsg )													// ����Ʈ ��Ÿ ���� ó�� �Լ�.
{
	if( !pMsg ) return ;																			// �޽��� ������ ��ȿ���� ������, ����ó���Ѵ�.

	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg ;															// �޽����� ���� �����͸� �����ϰ� �޽����� �޴´�.

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pmsg->dwData);										// �޽����� �Ѿ�� ������ �ε����� ������ ������ �޴´�.

	if( !pItemInfo ) return ;																		// ������ ������ �������� ������, return ó�� �Ѵ�.

	char tempBuf[128] = {0,} ;																		// �޽��� ����� ���� �ӽ� ���۸� �����Ѵ�.
	//sprintf(tempBuf, "[%s] �������� ������ �����Ͽ� ����Ʈ�� �Ϸ��� �� �����ϴ�.", pItemInfo->ItemName) ;	// ���� �޽����� �����Ѵ�.
	sprintf(tempBuf, CHATMGR->GetChatMsg(986), pItemInfo->ItemName) ;	// ���� �޽����� �����Ѵ�.

	GAMEIN->GetNpcScriptDialog()->EndDialog();														// npc ��ũ��Ʈ ���̾�α׷� end ���̾�α׸� ȣ���Ѵ�.
	GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);;												// npc ��ũ��Ʈ ���̾�α׸� ��Ȱ��ȭ �Ѵ�.
	WINDOWMGR->MsgBox( MBI_QUESTMSG, MBT_OK, tempBuf );												// ����ǰ�� �� ������ ���� ���� �Ұ���� �޽����� ����Ѵ�.
}

void CQuestManager::Quest_Error_FullQuest( void* pMsg )
{
	if( !pMsg ) return ;

	MSGBASE* pmsg = (MSGBASE*)pMsg ;

	char tempBuf[128] = {0,} ;																		// �޽��� ����� ���� �ӽ� ���۸� �����Ѵ�.
	//sprintf(tempBuf, "[%s] �������� ������ �����Ͽ� ����Ʈ�� �Ϸ��� �� �����ϴ�.", pItemInfo->ItemName) ;	// ���� �޽����� �����Ѵ�.
	//sprintf(tempBuf, "����Ʈ ����� ���� ���� ���̻� ����Ʈ�� ������ �� �����ϴ�.") ;	// ���� �޽����� �����Ѵ�.
	sprintf(tempBuf, CHATMGR->GetChatMsg(1474)) ;

	GAMEIN->GetNpcScriptDialog()->EndDialog();														// npc ��ũ��Ʈ ���̾�α׷� end ���̾�α׸� ȣ���Ѵ�.
	GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);;												// npc ��ũ��Ʈ ���̾�α׸� ��Ȱ��ȭ �Ѵ�.
	WINDOWMGR->MsgBox( MBI_QUESTMSG, MBT_OK, tempBuf );												// ����ǰ�� �� ������ ���� ���� �Ұ���� �޽����� ����Ѵ�.
}

// 070329 LYW --- QuestManager : Add function to make string.
void CQuestManager::CompositString( char* inputStr, char* str, ... )								// ��¿� �޽����� �ϼ��ϴ� �Լ�.
{
	if( inputStr == NULL ) return ;																	// ���� ��Ʈ�� ������ ��ȿ���� ������ ���� ó���� �Ѵ�.
	if( str == NULL ) return;																		// ��Ʈ�� ������ ��ȿ���� ������ ���� ó���� �Ѵ�.

	va_list argList;																				// ���� ���� ������ �����Ѵ�.

	va_start(argList, str);																			// ���� ���� �ؼ��� �����Ѵ�.
	vsprintf(inputStr,str,argList);																	// �޽����� �ϼ��Ѵ�.
	va_end(argList);																				// ���� ���� ��� �Ϸ�.
}


//void CQuestManager::RegistQuestIDToQuickView(int idx, DWORD dwQuestId)								// ����Ʈ �˸��̿� ��� �Ǵ� ����Ʈ ���̵� �����ϴ� �Լ�.
//{
//	if( idx >= MAX_QUICKVIEW_QUEST )																// ���ڷ� �Ѿ�� �ε����� �ִ� ��� ���� ����Ʈ ���� ������,
//	{
//		return  ;																					// ���� ó���� �Ѵ�.
//	}
//
//	m_QuickViewQuestID[idx] = dwQuestId ;															// �ε����� �ش��ϴ� �迭�� ����Ʈ ���̵� �����Ѵ�.
//}

//void CQuestManager::DeleteQuestIDToQuickView(int idx)												// ����Ʈ �˸��̿� ��� �� ����Ʈ ���̵� �����ϴ� �Լ�.
//{
//	if( idx >= MAX_QUICKVIEW_QUEST )																// ���ڷ� �Ѿ�� �ε����� �ִ� ��� ���� ����Ʈ ���� ������,
//	{
//		return  ;																					// ���� ó���� �Ѵ�.
//	}
//
//	m_QuickViewQuestID[idx] = 0 ;																	// �ε����� �ش��ϴ� �迭�� ����Ʈ ���̵� ���� ���� �ִ� ������ �����Ѵ�.
//}

void CQuestManager::SaveQuestInfoFromQuickView(sQUEST_INFO* pInfo)									// ����Ʈ �˸��̿� ��� �� ����Ʈ ���̵� ��ȯ�ϴ� �Լ�.
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

	//QuestTree* pTree ;																// Ʈ�� ������ ���� �����͸� �����Ѵ�.
	//CQuestString* pQuestString ;													// ��Ʈ�� ������ ���� �����͸� �����Ѵ�.

	//PTRLISTPOS treePos ;
	//PTRLISTPOS pos = NULL ;
	//pos = pList->GetHeadPosition() ;

	//int nCount = 0 ;
	//while(pos)
	//{
	//	pTree = NULL ;																// Ʈ�� ������ ���� �����͸� null ó���� �Ѵ�.
	//	pTree = (QuestTree*)pList->GetNext(pos) ;									// ��ġ ������ �ش��ϴ� Ʈ���� �޴´�.

	//	if( !pTree )																// Ʈ�� ������ ��ȿ�ϴٸ�,
	//	{
	//		continue ;
	//	}

	//	treePos = NULL ;															// Ʈ���� ����Ʈ���� ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.	
	//	treePos = pTree->list.GetHeadPosition() ;									// Ʈ���� ����Ʈ�� ���� �����Ѵ�.

	//	while(treePos)																// ��ġ ������ ��ȿ�� ���� while���� ������.
	//	{
	//		pQuestString = NULL ;													// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
	//		pQuestString = (CQuestString*)pTree->list.GetNext(treePos) ;			// 0��° ��ġ�� �ش��ϴ� ����Ʈ ��Ʈ���� �޴´�.

	//		if( !pQuestString )														// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
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

	//	if( pQuickViewDlg->CheckSameQuest(dwQuestIdx) )									// ����Ʈ ����Ʈ�� ���ڷ� �Ѿ�� ����Ʈ�� ���� ����Ʈ�� �ִٸ�,
	//	{
	//		return ;																	// ���� ó���� �Ѵ�.
	//	}

	//	int nTreeCount = 0 ;															// ����Ʈ Ʈ�� ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.

	//	nTreeCount = pList->GetCount() ;												// ����Ʈ�� ��� ����Ʈ�� ī��Ʈ�� �޴´�.	

	//	if( nTreeCount >= MAX_QUESTTREECOUNT )											// Ʈ�� ī��Ʈ�� �ִ� Ʈ�� �� �̻��̸�,
	//	{
	//		return ;
	//	}
	//	else																			// Ʈ�� ������ 5�� �̸��̸�,
	//	{
	//		QuestTree* pTree = new QuestTree ;											// Ʈ���� �����Ѵ�.

	//		CQuestInfo* pQuestInfo = NULL ;												// ����Ʈ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
	//		pQuestInfo = QUESTMGR->GetQuestInfo(dwQuestIdx) ;							// ����Ʈ �Ŵ����� ����, ���ڷ� �Ѿ�� ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.

	//		if( !pQuestInfo )															// ����Ʈ ������ ��ȿ���� �ʴٸ�,
	//		{
	//			ASSERTMSG(0, "����Ʈ �˸��� ��� �� ����Ʈ ������ �޾ƿ��� ���߽��ϴ�.!") ;		// assert ���� ó���� �Ѵ�.
	//			return ;																// ���� ó���� �Ѵ�.
	//		}

	//		int nSubQuestCount = 0 ;													// ���� ����Ʈ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
	//		nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// ����Ʈ ������ ���� ���� ����Ʈ ī��Ʈ�� �޴´�.

	//		CQuestString* pQuestString ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����Ѵ�.
	//		
	//		for(int count = 0 ; count < nSubQuestCount ; ++count )						// ���� ����Ʈ ī��Ʈ ��ŭ for���� ������.
	//		{
	//			pQuestString = NULL ;													// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
	//			pQuestString = QUESTMGR->GetQuestString(dwQuestIdx, count) ;			// ����Ʈ �ε�����, ���� ����Ʈ �ε����� ����Ʈ ��Ʈ���� �޴´�.
	//            
	//			if( pQuestString )														// ����Ʈ ��Ʈ�� ������ ��ȿ�ϴٸ�,
	//			{
	//				CQuestString* pNewString = new CQuestString ;						// ����Ʈ ��Ʈ���� �����Ѵ�.
	//				memcpy(pNewString, pQuestString, sizeof(CQuestString)) ;			// ���� ������ ����Ʈ ��Ʈ����, ������ ����Ʈ ��Ʈ���� �����Ѵ�.

	//				cPtrList* pList = pQuestString->GetTitle() ;						// ����Ʈ ��Ʈ���� Ÿ��Ʋ ����Ʈ�� �޴´�.

	//				PTRLISTPOS titlePos = NULL ;										// Ÿ��Ʋ ����Ʈ ���� ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
	//				titlePos = pList->GetHeadPosition() ;								// Ÿ��Ʋ ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	//				QString* pQString = NULL ;											// Q��Ʈ���� ���� �����͸� �����ϰ� nulló���� �Ѵ�.

	//				while(titlePos)														// ��ġ ������ ��ȿ�� ���� while���� ������.
	//				{
	//					pQString = NULL ;												// Q��Ʈ�� �����͸� nulló���� �Ѵ�.
	//					pQString = (QString*)pList->GetNext(titlePos) ;					// ��ġ�� ���� Q��Ʈ���� �޴´�.

	//					if( pQString )													// Q��Ʈ�� ������ ��ȿ�ϴٸ�,
	//					{
	//						pNewString->SetTitleStr( pQString->Str ) ;					// Q��Ʈ���� ���ڿ��� �޾Ƽ� ���ο� ����Ʈ ��Ʈ���� Ÿ��Ʋ�� �����Ѵ�.
	//					}
	//				}

	//				pNewString->SetComplete(pInfo[index].bComplete[count]) ;
	//				pNewString->SetCount(pInfo[index].nCount[count]) ;
	//				pNewString->SetTotalCount(pInfo[index].nTotalCount[count]) ;

	//				pQuestString->SetComplete(pInfo[index].bComplete[count]) ;
	//				pQuestString->SetCount(pInfo[index].nCount[count]) ;
	//				pQuestString->SetTotalCount(pInfo[index].nTotalCount[count]) ;

	//				pTree->list.AddTail(pNewString) ;									// Ʈ���� ����Ʈ�� ����Ʈ ��Ʈ���� �߰��Ѵ�.

	//			}
	//		}

	//		pTree->State = eQTree_Open ;												// Ʈ�� ���¸� ���� ���·� �����Ѵ�.
	//		pTree->QuestIdx = dwQuestIdx ;												// Ʈ���� ����Ʈ �ε����� �����Ѵ�.
	//		pList->AddTail(pTree) ;														// ����Ʈ ����Ʈ�� Ʈ���� �߰��Ѵ�.

	//		m_QuickViewQuestID[index] = dwQuestIdx ;									// �˸�â�� ��� �� ����Ʈ �ε����� ����Ʈ �Ŵ����� ���� �Ѵ�.
	//	}

	//	//pQuickViewDlg->ResetLoadedQuickViewDialog() ;									// �� �並 ���ΰ�ħ�Ѵ�.
	//	pQuickViewDlg->ResetQuickViewDialog() ;
	//}
}

void CQuestManager::SaveQuestInfoFromQuestDlg(sQUEST_INFO* pInfo)
{

}

void CQuestManager::LoadQuestInfoFromFile(sQUEST_INFO* pInfo)
{
}

//DWORD CQuestManager::GetQuestIDFromQuickView(int idx)												// ����Ʈ �˸��̿� ��� �� ����Ʈ ���̵� ��ȯ�ϴ� �Լ�.
//{
//	DWORD dwQuestId = 0 ;																			// ����Ʈ ���̵� ���� ������ �����ϰ� �ʱ� ���� �����Ѵ�.
//
//	if( idx >= MAX_QUICKVIEW_QUEST )																// ���ڷ� �Ѿ�� �ε����� �ִ� ��� ���� ����Ʈ ���� ������,
//	{
//		return dwQuestId ;																			// ���� ó���� �Ѵ�.
//	}
//
//	return m_QuickViewQuestID[idx] ;																// �ε����� �ش��ϴ� ����Ʈ ���̵� ��ȯ�Ѵ�.
//}


int CQuestManager::CheckQuestType(DWORD wQuestIdx, DWORD wSubQuestIdx)							
{
	int nQuestType = e_QT_ERROR_TYPE ;

	CQuest* pQuest = QUESTMGR->GetQuest( wQuestIdx );							// ����Ʈ ������ �޴´�.
	CSubQuestInfo* pDescSubQuestInfo ;												// ���� ����Ʈ ������ ���� �����͸� �����Ѵ�.

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





























