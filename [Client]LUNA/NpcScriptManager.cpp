#include "stdafx.h"																		// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.

#include "WindowIDEnum.h"																// ������ ���̵� ������ ���� ��� ������ �ҷ��´�.

#include "MHMap.h"																		// ���� �� Ŭ���� ��� ������ �ҷ��´�.

#include "GameIn.h"																		// ���� �� Ŭ���� ��� ������ �ҷ��´�.

#include "cDialogueList.h"																// ���̾�α� ����Ʈ Ŭ���� ��� ������ �ҷ��´�.
#include "cHyperTextList.h"																// ������ �ؽ�Ʈ ����Ʈ Ŭ���� ��� ������ �ҷ��´�.
#include "cMsgBox.h"																	// �޽��� �ڽ� Ŭ���� ��� ������ �ҷ��´�.
#include "cPage.h"																		// ������ Ŭ���� ��� ������ �ҷ��´�.
#include "GuildUnion.h"																	// ��� ���Ͽ� Ŭ���� ��� ������ �ҷ��´�.
#include "Npc.h"																		// NPC Ŭ���� ��� ������ �ҷ��´�.

#include "BailDialog.h"																	// ��꼭 ���̾�α� Ŭ���� ����� �ҷ��´�.
#include "cSkillTrainingDlg.h"															// ��ų Ʈ���̴� ���̾�α� Ŭ���� ����� �ҷ��´�.
#include "../DateMatchingDlg.h"															// ����Ʈ ��Ī ���̾�α� Ŭ���� ��� ������ �ҷ��´�.
#include "FamilyCreateDialog.h"															// �йи� ���� ���̾�α� Ŭ���� ��� ������ �ҷ��´�.
#include "GTBattleListDialog.h"															// ��� ��Ʋ ����Ʈ ���̾�α� Ŭ���� ��� ������ �ҷ��´�.
#include "GuildCreateDialog.h"															// ��� ���� ���̾�α� Ŭ���� ��� ������ �ҷ��´�.
#include "GuildLevelUpDialog.h"															// ��� ������ ���̾�α� Ŭ���� ��� ������ �ҷ��´�.
#include "GuildMarkDialog.h"															// ��� ��ũ ���̾�α� Ŭ���� ��� ������ �ҷ��´�.
#include "GuildWarehouseDialog.h"														// ���� �Ͽ콺 ���̾�α� Ŭ���� ��� ������ �ҷ��´�.
#include "GTRegistDialog.h"																// ��� ��ʸ�Ʈ ��� ���̾�α� Ŭ���� ��� ������ �ҷ��´�.
#include "GTRegistcancelDialog.h"														// ��� ��ʸ�Ʈ �뵵 ���̾�α� Ŭ���� ��� ������ �ҷ��´�.
#include "InventoryExDialog.h"															// �κ��丮 ���̾�α� Ŭ���� ��� ������ �ҷ��´�.
#include "ItemShopDialog.h"																// �����ۼ� ���̾�α� Ŭ���� ��� ������ �ҷ��´�.
#include "NpcImageDlg.h"																// NPC �̹��� ���̾�α� Ŭ���� ��� ������ �ҷ��´�.
#include "NpcScriptDialog.h"															// NPC ��ũ��Ʈ ���̾�α� Ŭ���� ��� ������ �ҷ��´�.
#include "SeigeWarDialog.h"																// ������ ���̾�α� Ŭ���� ��� ������ �ҷ��´�.
#include "StorageDialog.h"																// ���丮�� ���̾�α� Ŭ���� ��� ������ �ҷ��´�.
#include "WantedDialog.h"																// ������� ���̾�α� Ŭ���� ��� ������ �ҷ��´�.

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

#include "ChatManager.h"																// ä�� �Ŵ��� ��� ������ �ҷ��´�.
//#include "FamilyManager.h"																// �йи� �Ŵ��� ��� ������ �ҷ��´�.
#include "GuildManager.h"																// ��� �Ŵ��� ��� ������ �ҷ��´�.
#include "MoveManager.h"																// �̵� �Ŵ��� ��� ������ �ҷ��´�.
#include "NpcScriptManager.h"															// NPC ��ũ��Ʈ �Ŵ��� ��� ������ �ҷ��´�.
#include "ObjectManager.h"																// ������Ʈ �Ŵ��� ��� ������ �ҷ��´�.
#include "objectstatemanager.h"															// ������Ʈ ���� �Ŵ��� ��� ������ �ҷ��´�.
#include "SiegeWarMgr.h"																// ������ �Ŵ��� ��� ������ �ҷ��´�.
#include "WantedManager.h"																// ������� �Ŵ��� ��� ������ �ҷ��´�.

// desc_hseos_����ý���_01
// S ����ý��� �߰� added by hseos 2007.09.13
#include "../hseos/Farm/SHFarmManager.h"
// E ����ý��� �߰� added by hseos 2007.09.13

// desc_hseos_�йи��ý���_01
// S �йи��ý��� �߰� added by hseos 2007.10.25
#include "../hseos/Family/SHFamilyManager.h"
// E �йи��ý��� �߰� added by hseos 2007.10.25

GLOBALTON(cNpcScriptManager)															// �۷ι� �� NPC ��ũ��Ʈ �Ŵ���.
#include "GameResourceStruct.h"

cNpcScriptManager::cNpcScriptManager()													// ������ �Լ�.
{
	m_nRegistNpcCount = 0;																// ��� NPC ī��Ʈ�� 0���� �����Ѵ�.

	m_pDialogue = NULL;																	// ���̾�α� �����͸� NULL ó�� �Ѵ�.

	m_pHyperText = NULL;																// ������ �ؽ��� �����͸� NULL ó�� �Ѵ�.

	SetSelectedNpc(NULL);																// ���� �� NPC�� ���� �Ѵ�.
}

cNpcScriptManager::~cNpcScriptManager()													// �Ҹ��� �Լ�.
{
	DeleteAllScriptInfo();																// ��� ��ũ��Ʈ ������ �����Ѵ�.

	SAFE_DELETE( m_pDialogue );															// ���̾�α� ������ �����ϰ� �����.
	SAFE_DELETE( m_pHyperText );														// ������ �ؽ�Ʈ ������ �����ϰ� �����.
}

void cNpcScriptManager::Init()															// �ʱ�ȭ �Լ�.
{
	LoadNpcScriptInfo();																// NPC ��ũ��Ʈ ������ �ε��Ѵ�.

	m_pDialogue = new cDialogueList;													// ���̾�α� ����Ʈ �޸𸮸� �Ҵ��Ͽ� �����ͷ� �޴´�.

	m_pDialogue->LoadDialogueListFile(NPC_DIALOGUE_PATH, "rb");							// ���Ϸ� ���� ���̾�α� ����Ʈ ������ �ε��Ѵ�.

	m_pHyperText = new cHyperTextList;													// ������ �ؽ�Ʈ �޸𸮸� �Ҵ��Ͽ� �����ͷ� �޴´�.

	m_pHyperText->LoadHyperTextFormFile(NPC_HYPERTEXT_PATH, "rb");						// ���Ϸ� ���� ������ �ؽ�Ʈ ������ �ε��Ѵ�.
}

void cNpcScriptManager::AddNpcScriptInfo(DWORD dwNpcId, cPage* pPage)					// NPC ��ũ��Ʈ ������ �߰��ϴ� �Լ�.
{
	m_NpcScriptList[dwNpcId].AddTail(pPage);											// NPC ��ũ��Ʈ ����Ʈ�� ���� ������ ������ �߰��Ѵ�.
}

void cNpcScriptManager::DeleteAllScriptInfo()											// ��� ��ũ��Ʈ ������ �����ϴ� �Լ�.
{
	for(int i=0;i<MAX_NPC_COUNT;++i)													// �ִ� NPC ī��Ʈ ��ŭ for���� ������.
	{
		PTRLISTSEARCHSTART(m_NpcScriptList[i],cPage*,pPage)								// NPC ��ũ��Ʈ ����Ʈ���� ������ ������ ã�´�.
			delete pPage;																// ������ ������ �����Ѵ�.
		PTRLISTSEARCHEND																// ������ ���� ã�� ����.
			
		m_NpcScriptList[i].RemoveAll();													// ī��Ʈ�� �ش��ϴ� NPC ��ũ��Ʈ�� ��� ����.
	}
}

cPage* cNpcScriptManager::GetMainPage( DWORD dwNpcId )									// ���� ������ ������ ��ȯ�ϴ� �Լ�.
{
	if( m_NpcScriptList[dwNpcId].IsEmpty() ) return NULL;								// NPC ��ũ��Ʈ ����Ʈ�� NPC�� ���õ� ������ ������ NULL ���� ó���� �Ѵ�.

	return (cPage*)m_NpcScriptList[dwNpcId].GetHead();									// NPC ��ũ��Ʈ ����Ʈ���� NPC ���� ���� �������� �����Ѵ�.
}

cPage* cNpcScriptManager::GetPage( DWORD dwNpcId, DWORD dwPageId )						// ���ϴ� ������ ������ ��ȯ�ϴ� �Լ�.
{
	PTRLISTSEARCHSTART(m_NpcScriptList[dwNpcId],cPage*,pPage)							// ���� NPC�� ������ ������, NPC ��ũ��Ʈ ����Ʈ���� ������ ã�´�.
		if( pPage->GetPageId() == dwPageId )											// ���� ������ ���̵��, ����Ʈ ���� ������ ���̵� ������,
		{
			return pPage;																// ������ ������ �����Ѵ�.
		}
	PTRLISTSEARCHEND																	// ������ ���� ã�� ����.
			
	return NULL;																		// NULL ���� ó���� �Ѵ�.
}

void cNpcScriptManager::LoadNpcScriptInfo()												// NPC������ �ε��ϴ� �Լ�.
{
	CMHFile fp;																			// ���� ������ �����Ѵ�.

	if(!fp.Init(NPC_SCRIPT_PATH, "rb"))													// ���� ������ �б� ���� ���� ���� �����ϸ�,
	{
		return;																			// ���� ó���� �Ѵ�.
	}

	char buff[256]={0,};																// �ӽ� ���۸� �����Ѵ�.

	while(1)																			// while���� ������.
	{
		fp.GetString(buff);																// �ӽ� ���۷� ��Ʈ���� �о���δ�.

		if( fp.IsEOF() )																// ���� �����Ͱ� ������ ���� ����Ű��,
		{
			break;																		// while���� Ż���Ѵ�.
		}

		if(buff[0] == '@')																// ������ ù ���ڰ� @�� ������,
		{
			fp.GetLineX(buff, 256);														// �ӽ� ���۷� ������ �о���δ�.
			continue;																	// while���� ��� ������.
		}

		CMD_ST(buff)																	// �ӽ� ���۸� Ȯ���Ѵ�.
		CMD_CS("$NPC")																	// $NPC�� ������,
			if((fp.GetString())[0] == '{')
			{
				LoadPageInfo(NULL, &fp);												// ������ ������ �ε��Ѵ�.
				++m_nRegistNpcCount;													// ��� �� npc ���� �����Ѵ�.
			}
			else
				__asm int 3;
		CMD_EN
	}
}

void cNpcScriptManager::LoadPageInfo(cPage* pPage, CMHFile* fp)							// ������ ������ �ε��ϴ� �Լ�.
{
	char buff[256]={0,};																// �ӽ� ���۸� �����Ѵ�.

	static DWORD dwNpcId;																// Npc������ Page������ �Ϸ� �ɶ����� ���� �����ؾ� �Ѵ�.

	DWORD dwPageId;																		// ������ ���̵� ��� ������ �����Ѵ�.

	int nDialogueCount = 0;																// ���̾�α� ī��Ʈ�� 0���� �����Ѵ�.
	int nHyperLinkCount = 0;															// ������ ��ũ ī��Ʈ�� 0���� �����Ѵ�.

	BYTE emotion = 0;																	// �̸�� ���� 0���� �����Ѵ�.

	DWORD dwDialogueId;																	// ���̾�α� ���̵� ���� ������ �����Ѵ�.

	HYPERLINK sHyper;																	// ������ ��ũ ������ ���� ����ü�� �����Ѵ�.

	while(1)																			// while���� ������.
	{
		memset(&sHyper, 0, sizeof(HYPERLINK)) ;											// �����۸�ũ ����ü�� �޸� �� �Ѵ�.

		fp->GetString(buff);															// �ӽ� ���۷� ��Ʈ���� �о���δ�.

		if(buff[0] == '}'|| fp->IsEOF())												// �ӽ� ������ ù ���ڰ� }�̰ų� ������ ���̸�,
		{
			break;																		// while���� Ż���Ѵ�.
		}

		if(buff[0] == '@')																// �ӽ� ������ ù ���ڰ� @�� ������,
		{
			fp->GetLineX(buff, 256);													// �ӽ� ���۷� ������ �о���δ�.
			continue;																	// while���� ��� ������.
		}

		CMD_ST(buff)																	// �ӽ� ���۸� Ȯ���Ѵ�.

		CMD_CS("$PAGE")																	// ���۰� $PAGE�� ������,

			if((fp->GetString())[0] == '{')												// ��Ʈ���� �а�, �� ���� {�� ������,
			{
				cPage* pNewPage = new cPage;											// ������ �޸𸮸� �Ҵ��Ѵ�.
				AddNpcScriptInfo( dwNpcId, pNewPage );									//��ũ��Ʈ ������ �߰��Ѵ�.
				LoadPageInfo(pNewPage, fp);												// ������ ������ �ε��Ѵ�.
			}

		CMD_CS("#NPCID")																// ���۰� #NPCID�� ������,

			dwNpcId = fp->GetDword();													// npc ���̵� �޴´�.

		CMD_CS("#PAGEINFO")																// ���۰� #PAGEINFO�� ������,

			dwPageId = fp->GetDword();													// ������ ���̵� �޴´�.
			nDialogueCount = fp->GetInt();												// ���̾�α� ī��Ʈ�� �޴´�.
			nHyperLinkCount = fp->GetInt();												// ������ ��ũ ī��Ʈ�� �޴´�.
			emotion = fp->GetInt();														// ����� �޴´�.
			pPage->Init( dwPageId, emotion );											// ������ ������ �ʱ�ȭ �Ѵ�.

		CMD_CS("#DIALOGUE")																// ���۰� #DIALOGUE�� ���ٸ�,

			for(int i =0; i<nDialogueCount;++i)											// ���̾�α� ī��Ʈ ��ŭ for���� ������.
			{
				dwDialogueId = fp->GetDword();											// ���̾�α� ���̵� �޴´�.
				pPage->AddDialogue( dwDialogueId );										// �������� ���̾�α׸� �߰��Ѵ�.
			}

		CMD_CS("#HYPERLINK")															// ���۰� #HYPERLINK�� ���ٸ�,

			sHyper.wLinkId		= fp->GetWord();										// ��ũ ���̵� �޴´�.
			sHyper.wLinkType	= fp->GetWord()+1;										// ��ũ Ÿ���� �޴´�.
			sHyper.dwData		= fp->GetDword();										// �����͸� �޴´�.
			if( sHyper.wLinkType == emLink_MapChange )
			{
				sHyper.fXpos		= fp->GetFloat() ;									// �̵� �� ���� x ��ǥ�� �޴´�.
				sHyper.fZpos		= fp->GetFloat() ;									// �̵� �� ���� y ��ǥ�� �޴´�.
			}
			pPage->AddHyperLink( &sHyper );												// ��ũ ������ �߰��Ѵ�.

		CMD_CS("#MAPMOVELINK")
			sHyper.wLinkId		= fp->GetWord();										// ��ũ ���̵� �޴´�.
			sHyper.wLinkType	= fp->GetWord()+1;										// ��ũ Ÿ���� �޴´�.
			sHyper.dwData		= fp->GetDword();										// �̵� �� �� ��ȣ�� �޴´�.
			sHyper.fXpos		= fp->GetFloat() ;										// �̵� �� ���� x ��ǥ�� �޴´�.
			sHyper.fZpos		= fp->GetFloat() ;										// �̵� �� ���� y ��ǥ�� �޴´�.
			pPage->AddHyperLink( &sHyper );												// ��ũ ������ �߰��Ѵ�.

		CMD_EN
	}
}

void cNpcScriptManager::StartNpcScript( CNpc* pNpc )									// NPC ��ũ��Ʈ�� �����ϴ� �Լ�.
{
	if( m_pCurSelectedNpc )																// ���� �� NPC �� ������, // ������ �����ϸ� �������� �����ϰ� ���� ����.
	{
		GAMEIN->GetNpcScriptDialog()->EndDialog();										// ���̾�α׸� �ݴ´�.
		SetSelectedNpc(NULL);															// ���� �� NPC �� �����Ѵ�.
	}

	VECTOR3 pos;																		// ��ġ ������ ���� ���͸� �����Ѵ�.

	HERO->GetPosition( &pos );															// HERO�� ��ġ ������ �޴´�.

	if( pNpc->GetNpcKind() != 37 )														// NPC ������ 37�� ���� ������,
	{
		MOVEMGR->SetLookatPos(pNpc,&pos,0,gCurTime);									// HERO�� ������ �Ѵ�.
	}

	SetSelectedNpc(pNpc);																// ���� �� NPC�� �����Ѵ�.


	// 080402 NYJ --- ���̾�α׸� ���µ� ����������� ó��
	//GAMEIN->GetNpcScriptDialog()->OpenDialog( pNpc->GetNpcUniqueIdx(), pNpc->GetNpcListInfo()->ModelNum );	// ���̾�α׸� ����.
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

void cNpcScriptManager::StartNpcBusiness(DWORD dwData, DWORD dwParam)					// NPC �����Ͻ��� �����ϴ� �Լ�.
{
	if( m_pCurSelectedNpc == NULL && dwParam == 0 )										// ���� ���� �� NPC ������ ���ų�, �Ķ���� ���� 0�� ������,
	{
		ASSERT(0);																		// ASSERT üũ�� �Ѵ�.

		if( HERO->GetState() == eObjectState_Deal )										// HERO�� ���°� �ŷ� ���̸�,
		{
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);					// �ŷ� ���¸� �����Ѵ�.
		}
		
		return;																			// ���� ó���� �Ѵ�.
	}

	if( m_pCurSelectedNpc )																// ���� ���� �� NPC ������ �ִٸ�,
	{
		WORD wJobKind = m_pCurSelectedNpc->GetNpcJob() ;								// NPC ������ �޴´�.

		switch( wJobKind )																// NPC ������ Ȯ���Ѵ�.
		{
		case DEALER_ROLE :			Business_Dealer() ;					break ;			// �������, ���� �����Ѵ�.
		case CASTLE_ROLE :			Business_Castle(dwData) ;			break ;			// ��������, ���� ���õ� ���� �����ϳ�.
		case CHANGGO_ROLE :			Business_Changgo(dwData, dwParam) ;	break ;			// â��������, â�� �����Ͻ��� �����Ѵ�.
		case MUNPA_ROLE :			Business_Guild(dwData) ;			break ;			// ��� ������ �̶��, ��� �����Ͻ��� �����Ѵ�.
		case WANTED_ROLE :			Business_Wanted(dwData) ;			break ;			// ����� ������ �̶��, ����� �����Ͻ��� �����Ѵ�.
		case SURYUN_ROLE :			Business_Suryun(dwData) ;			break ;			// ���� ������ �̶��, ���� �����Ͻ��� �����Ѵ�.
		case MAPCHANGE_ROLE :		Business_MapChange(dwData) ;		break ;			// �� ü���� NPC ���, �� ü���� �����Ͻ��� �����Ѵ�.
		//case SYMBOL_ROLE :			Business_Symbol() ;					break ;			// �ɺ� ������ �̶��, �ɺ� ���� �����Ͻ��� �����Ѵ�.
		case FAMILY_ROLE :			Business_Family(dwData) ;			break ;			// �йи� ������ �̶��, �йи� ���� �����Ͻ��� �����Ѵ�.
		case IDENTIFICATION_ROLE :	Business_Identifycation() ;			break ;			// �ֹε�� ���� ���̶��, �ֹε�� �����Ͻ��� �����Ѵ�.
		default :																		// �� ���� ���,
			{
				ASSERT(0);																// ASSERT üũ�� �Ѵ�.

				if( HERO->GetState() == eObjectState_Deal )								// HERO�� ���°� �ŷ����̶��,
				{
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);			// �ŷ� ���¸� ������.
				}

				return;																	// ���� ó���� �Ѵ�.
			}
			break ;
		}
	}

	if( dwParam == eNpcParam_ShowpStorage )												// �Ķ���� ���� eNpcParam_ShowpStorage�� ������,
	{
		Business_Changgo(dwData, dwParam) ;												// â�� �����Ͻ��� �����Ѵ�.
	}

	GAMEIN->GetNpcScriptDialog()->DisActiveEx();										// NPC ��ũ��Ʈ ���̾�α��� DisActiveEx()�� ȣ���Ѵ�.
}

void cNpcScriptManager::Business_Dealer()  												// ���� �����Ͻ�.
{
	MSG_WORD msg;																		// �޽��� ����ü�� �����Ѵ�.
	msg.Category = MP_ITEM;																// ī�װ��� ���������� �����Ѵ�.
	msg.Protocol = MP_ITEM_DEALER_SYN;													// ���������� ������ ���� ��û���� �����Ѵ�.
	msg.dwObjectID = HEROID;															// HERO�� ���̵� �����Ѵ�.
	msg.wData = m_pCurSelectedNpc->GetNpcUniqueIdx();									// NPC ����ũ ���̵� �����Ѵ�.

	NETWORK->Send(&msg, sizeof(msg));													// �޽����� �����Ѵ�.

	GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);										// ��ũ��Ʈ ���̾�α׸� ��Ȱ��ȭ �Ѵ�.
}

void cNpcScriptManager::Business_Castle(DWORD dwData)  									// (����)�� �����Ͻ�.
{
	switch( dwData )																	// �����͸� Ȯ���Ѵ�.
	{
	case 0:																				// �������� ���,
		{
			if( HERO->GetGuildIdx() == 0 ||												// HERO�� ��尡 ���ų�,
				HERO->GetGuildMemberRank() != GUILD_MASTER )							// ��� �����Ͱ� �ƴ϶��,
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(323) );				// ������ ���ٴ� �޽����� ����Ѵ�.
				return;																	// ���� ó���� �Ѵ�.
			}

			SWPROFIT->SendProfitInfoSyn();												// ������ �̵��� �̵����� ��ũ�� ȣ���Ѵ�.
		}
		break;

	case 1:
	case 2:
	case 3:																				// �ܼ���
	case 4:
	case 5:
	case 6:																				// ������
		{	
			if( HERO->GetGuildIdx() == 0 ||												// HERO�� ��尡 ���ų�,
				HERO->GetGuildMemberRank() != GUILD_MASTER )							// ��� �����Ͱ� �ƴ϶��,
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(323) );				// ������ ���ٴ� �޽����� ����Ѵ�.
				return;																	// ���� ó���� �Ѵ�.
			}

			DWORD dwMoney = 0;															// �Ӵ� ������ ���� ������ �����Ѵ�.

			switch( dwData )															// �����͸� Ȯ���Ѵ�.
			{
			case 1:		dwMoney = 1000000;		break;									// �����Ϳ� ���� �Ӵϸ� Ȯ���Ѵ�.
			case 2:		dwMoney = 2000000;		break;
			case 3:		dwMoney = 3000000;		break;
			case 4:		dwMoney = 1500000;		break;
			case 5:		dwMoney = 2500000;		break;				
			case 6:		dwMoney = 4000000;		break;
			}

			if( HERO->GetMoney() < dwMoney )											// HERO�� �Ӵϰ� ���� �Ӵ� ���� ������,
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(324) );				// ���� �����ϴٴ� �޽����� ����Ѵ�.
				return;																	// ���� ó���� �Ѵ�.
			}

			MSG_DWORD Msg;																// �޽��� ����ü�� �����Ѵ�.
			Msg.Category = MP_SIEGEWAR;													// ī�װ��� �������� �����Ѵ�.
			Msg.Protocol = MP_SIEGEWAR_UPGRADE_GATE_SYN;								// ���������� ������ ���׷��̵� ����Ʈ ��ũ�� �����Ѵ�.
			Msg.dwObjectID = HEROID;													// HERO ���̵� �����Ѵ�.
			Msg.dwData = dwData;														// �����͸� �����Ѵ�.

			NETWORK->Send( &Msg, sizeof(Msg) );											// �޽����� �����Ѵ�.

			return;																		// ���� ó���� �Ѵ�.
		}
		break;
	}	
}

void cNpcScriptManager::Business_Changgo(DWORD dwData, DWORD dwParam)					// â�� �����Ͻ�.
{
	// 080513 KTH -- NPC ��ũ��Ʈ�� ���� �־�߸� ���� �ֵ��� ���� (defense hack)
	if( !GAMEIN->GetNpcScriptDialog()->IsActive() )
		return;

	if(dwData == 0)																		// �����͸� Ȯ���Ѵ�.
	{	
		if(GAMEIN->GetStorageDialog()->IsItemInit())									// â�� �������� ������ �ʱ�ȭ�� �Ǿ�����,
		{
			if( !GAMEIN->GetInventoryDialog()->IsActive() )								// �κ��丮 ���̾�αװ� Ȱ��ȭ ���̸�,
			{
				GAMEIN->GetStorageDialog()->ShowStorageDlg(TRUE) ;							// â�� ���̾�α׸� Ȱ��ȭ �Ѵ�.
			}
			else
			{
				GAMEIN->GetStorageDialog()->SetActive(TRUE) ;							// â�� ���̾�α׸� Ȱ��ȭ �Ѵ�.
			}

			if(HERO->GetStorageNum() == 0)												// ������ â�� ���ٸ�,
			{

				GAMEIN->GetStorageDialog()->ShowStorageMode(eStorageMode_NoWare);		// â�� ���ٴ� ���� �����Ѵ�.

				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);							// â�� ���̾�α׸� ��Ȱ��ȭ �Ѵ�.
			}
			else																		// ������ â�� �ִٸ�,
			{
				GAMEIN->GetStorageDialog()->ShowStorageMode(eStorageMode_StorageWare1);	// â�� 1�� Ȱ��ȭ �� ���� �Ѵ�.
				GAMEIN->GetStorageDialog()->AddStorageMode(eStorageMode_StorageWare1);	// â�� ��带 �߰��Ѵ�.

				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);							// ��ũ��Ʈ ���̾�α׸� ��Ȱ��ȭ �Ѵ�.
			}
		}
		else																			// â���� �������� ������ �ʱ�ȭ�� ���� �ʾҴٸ�,
		{
			if(HERO->GetStorageNum() == 0)												// ������ â�� ������,
			{
				if( !GAMEIN->GetInventoryDialog()->IsActive() )							// �κ��丮 ���̾�αװ� Ȱ��ȭ ���� �ʾ�����,
				{
					GAMEIN->GetStorageDialog()->ShowStorageDlg(TRUE) ;					// â�� ���̾�α׸� Ȱ��ȭ �Ѵ�.
				}

				GAMEIN->GetStorageDialog()->ShowStorageMode(eStorageMode_NoWare);		// â�� ���ٴ� ���� �����Ѵ�.

				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);							// ��Ʈ��Ʈ ���̾�α׸� ��Ȱ��ȭ �Ѵ�.
			}
			else																		// ������ â�� �ִٸ�,
			{
				MAP->SetVillage(TRUE) ;													// ������ �����Ѵ�.

				if( dwParam == eNpcParam_ShowpStorage || MAP->IsVillage() == TRUE )		// â�� npc �̰�, ������ ������,
				{						
					MSG_WORD2 msg;														// �޽��� ����ü�� �����Ѵ�.
					msg.Category = MP_ITEM;												// ī�װ��� ���������� �����Ѵ�.
					msg.Protocol = MP_ITEM_STORAGE_ITEM_INFO_SYN;						// ���������� ������ â��, ������ ���� ��ũ�� �����Ѵ�.
					msg.dwObjectID = HEROID;											// HERO ���̵� �����Ѵ�.
					
					if( dwParam==eNpcParam_ShowpStorage )								// NPC�� â�� NPC �̸�,
					{
						msg.wData1=79 ;													// �����͸� 79�� �����Ѵ�.
					}
					else																// NPC�� â�� NPC�� �ƴϸ�,
					{
						msg.wData1=m_pCurSelectedNpc->GetNpcUniqueIdx();				// �����͸� NPC ����ũ �ε����� �����Ѵ�.
					}
					//dwParam==eNpcParam_ShowpStorage ? msg.wData1=79 : msg.wData1=m_pCurSelectedNpc->GetNpcUniqueIdx();

					msg.wData2 = eNpcParam_ShowpStorage;								// ������ 2�� NPC �����ͷ� �����Ѵ�.

					NETWORK->Send(&msg, sizeof(msg));									// �޽����� �����Ѵ�.
				}
				else																	// NPC�� â�� NPC�� �ƴϰų�, ������ �ƴϸ�,
					return;																// ���� ó���� �Ѵ�.
			}
		}

		OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);						// HERO�� ���¸� �ŷ� ���·� �����Ѵ�.
	}	
	else if(dwData == 1)																// ����ǥ/����
	{
		GAMEIN->GetStorageDialog()->SetStorageListInfo();								// â�� ���̾�α��� ����Ʈ ������ �����Ѵ�.
		GAMEIN->GetStorageDialog()->ShowStorageDlg(TRUE) ;								// â�� ���̾�α��� â�� ���̾�α׸� �����ش�.

		GAMEIN->GetStorageDialog()->ShowStorageMode(eStorageMode_StorageListInfo);		// â�� ����Ʈ ������ â�� ��带 �����Ѵ�.
		GAMEIN->GetStorageDialog()->AddStorageMode(eStorageMode_StorageListInfo);		// â�� ���̾�α׿� â�� ��带 �߰��Ѵ�.

		GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);									// ��ũ��Ʈ ���̾�α׸� ��Ȱ��ȭ �Ѵ�.
	}
	else if(dwData == 2)																// ��� â��
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
	else if(dwData == 3)																// �����۸�â��
	{
		MSGBASE msg;																	// �޽��� ����ü�� �����Ѵ�.
		msg.Category = MP_ITEM;															// ī�װ��� ���������� �����Ѵ�.
		msg.Protocol = MP_ITEM_SHOPITEM_INFO_SYN;										// ���������� ������ �� ���� ��ũ�� �����Ѵ�.
		msg.dwObjectID = HERO->GetID();													// HERO�� ���̵� �����Ѵ�.

		NETWORK->Send(&msg, sizeof(msg));												// �޽����� �����Ѵ�.

		GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);									// ��ũ��Ʈ ���̾�α׸� ��Ȱ��ȭ �Ѵ�.
		OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);						// HERO�� ���¸� �ŷ� ���·� �����Ѵ�.
	}
}

void cNpcScriptManager::Business_Guild(DWORD dwData)  									// ��� �����Ͻ�.
{
	switch( dwData )																	// �����͸� Ȯ���Ѵ�.
	{
	case 0:																				// ��带 �����Ϸ��� �Ѵٸ�,
		{
			if(HERO->GetGuildIdx())														// HERO�� ��� �ε����� ��ȿ�ϸ�,
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(327));					// �̹� ��尡 �ִٴ� �޽����� ����Ѵ�.
				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);							// NPC ��ũ��Ʈ ���̾�α׸� ��Ȱ��ȭ �Ѵ�.

				return;																	// ���� ó���� �Ѵ�.
			}

			char* pEntryDate = HERO->GetGuildEntryDate() ;								// HERO�� GuildEntryDate�� �޴���.

			if(GUILDMGR->CanEntryGuild(pEntryDate) == FALSE)							// ���Ŵ����� CanEntryGuild()�� üũ�Ͽ� FALSE�� ������,
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(826), pEntryDate);		// �־��� ��¥���� â�� �����ϴٴ� �޽����� ����Ѵ�.
				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);							// NPC ��ũ��Ʈ ���̾�α׸� �ݴ´�.

				return;																	// ���� ó���� �Ѵ�.
			}

			cDialog* pDlg = (cDialog*)GAMEIN->GetGuildCreateDlg();						// ��� ���� ���̾�α� ������ �޾ƿ´�.

			if( pDlg )																	// ��� ���� ���̾�α� ������ ��ȿ���� üũ�Ѵ�.
			{
				pDlg->SetActive(TRUE);													// ��� ���� ���̾�α׸� Ȱ��ȭ �Ѵ�.
			}

			GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);								// NPC ��ũ��Ʈ ���̾�α׸� �ݴ´�.
		}
		break;

	case 1:																				// ��带 �����Ϸ��� �Ѵٸ�,
		{
			if(HERO->GetGuildIdx() == 0)												// HERO�� ��� �ε����� ��ȿ���� ������,
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));					// ��忡 ���ԵǾ� ���� �ʴٴ� �޽����� ����Ѵ�.
				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);							// NPC ��ũ��Ʈ ���̾�α׸� ��Ȱ��ȭ �Ѵ�.

				return;																	// ���� ó���� �Ѵ�.
			}
			else if(HERO->GetGuildMemberRank() == GUILD_MASTER)							// HERO�� ������ ��� �����Ϳ� ���ٸ�,
			{
				WINDOWMGR->MsgBox( MBI_GUILD_BREAKUP, MBT_YESNO, CHATMGR->GetChatMsg( 329 ) );	// �г�Ƽ�� �ް� �ȴٴ� ����â�� ����.
				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);							// NPC ��ũ��Ʈ ���̾�α׸� �ݴ´�.

				return ;																// ���� ó���� �Ѵ�.
			}
			else																		// HERO�� ������ ��� �����Ͱ� �ƴ϶��,
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(323));					// ������ ���ٴ� �޽����� ����Ѵ�.
				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);							// NPC ��ũ��Ʈ ���̾�α׸� �ݴ´�.

				return;																	// ���� ó���� �Ѵ�.
			}
		}
		break;

	case 2:																				// ��� ������ ����.
		{
			if(HERO->GetGuildIdx() == 0)												// HERO�� ��� �ε����� üũ�Ѵ�.
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));					// ��忡 ���ԵǾ� ���� �ʴٴ� �޽����� ����Ѵ�.
			}
			else if(HERO->GetGuildMemberRank() != GUILD_MASTER)							// HERO�� ������ ��� �����Ϳ� ���� �ʴٸ�,
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(323));					// ������ ���ٴ� �޽����� ����Ѵ�.
			}
			else
			{
				GAMEIN->GetGuildLevelUpDlg()->SetActive(TRUE);							// ��� ������ ���̾�α׸� Ȱ��ȭ �Ѵ�.
			}	

			GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
		}
		break;

	case 3:																				// ��� ��ũ ���.
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

			//if(GUILDMGR->GetGuildLevel() < GUILD_2LEVEL)								// ��� ������ 2�������� ������,
			//{
			//	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(326));					// ��� ������ ���ٴ� �޽����� ����Ѵ�.

			//	return;																	// ���� ó���� �Ѵ�.
			//}

			//if(HERO->GetGuildIdx() && (HERO->GetGuildMemberRank() == GUILD_MASTER))		// HERO�� ��� �ε����� ��ȿ�ϰ�, �����Ͷ��,
			//{
			//	GAMEIN->GetGuildMarkDlg()->ShowGuildMark();								// ��� ��ũ ���̾�α��� ��� ��ũ�� �����ش�.
			//}
			//else																		// �ε����� ���ų�, �����Ͱ� �ƴ϶��,
			//{
			//	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(330));					// ������ ��忡 ���� �����͸� �����ϴٴ� �޽����� ����Ѵ�.

			//	return;																	// ���� ó���� �Ѵ�.
			//}	
		}
		break;

	// 070820 ����, ���� ����/������ ���â���� �Ű���
	//case 4:																				// ��� ������ �����Ϸ� �Ѵٸ�,	
	//	{
	//		if( GUILDUNION->CheckGuildUnionCondition( 0 ) )								// ��� ���� ���¸� üũ�Ͽ� TRUE�� ��ȯ�ϸ�,
	//		{
	//			GAMEIN->GetGuildUnionCreateDlg()->SetActive( TRUE );					// ��� ���� ���� ���̾�α׸� Ȱ��ȭ �Ѵ�.
	//		}
	//		else																		// ��� ���� ���¸� üũ�Ͽ� FALSE�� �����ϸ�,
	//			return;																	// ���� ó���� �Ѵ�.
	//	}
	//	break;

	//case 5:																				// ��� ������ �����Ϸ��� �ϸ�,
	//	{
	//		if( GUILDUNION->CheckGuildUnionCondition( 1 ) )								// ��� ���� ���¸� üũ�Ͽ� TRUE�� ��ȯ�ϸ�,
	//		{
	//			WINDOWMGR->MsgBox( MBI_GUILD_UNION_DESTROY, MBT_YESNO, CHATMGR->GetChatMsg( 539 ) );	// �г�Ƽ �޽����� ����Ѵ�.
	//		}
	//		else																		// ��� ���� ���¸� üũ�Ͽ� FALSE�� �����ϸ�,
	//			return;																	// ���� ó���� �Ѵ�.
	//	}
	//	break;

	// ���� ��ũ ���
	case 4:
		{
			GAMEIN->GetGuildMarkDlg()->ShowGuildUnionMark();

			GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
		}
		break;
	
	//case 7:																				// ������ �ð� ����� �Ϸ��� �ϸ�,
	//	{
	//		if(HERO->GetGuildIdx() == 0)												// HERO�� ��� �ε����� ��ȿ���� ������,
	//		{
	//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));					// ��忡 ���ԵǾ� ���� �ʴٴ� �޽����� ����Ѵ�.

	//			return;																	// ���� ó���� �Ѵ�.
	//		}
	//		else if(HERO->GetGuildMemberRank() != GUILD_MASTER)							// HERO�� ����� ��� �����Ϳ� ���� ������,
	//		{
	//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(323));					// ������ ���ٴ� �޽����� ����Ѵ�.

	//			return;																	// ���� ó���� �Ѵ�.
	//		}

	//		GAMEIN->GetSWTimeRegDlg()->SetActive( TRUE );								// ������ �ð� ��� ���̾�α׸� Ȱ��ȭ �Ѵ�.
	//	}
	//	break;

	//case 8:																				// ������ ��û ����Ʈ�� ������ �ϸ�,
	//	{
	//		if(HERO->GetGuildIdx() == 0)												// HERO�� ��� �ε����� ��ȿ���� ������,
	//		{
	//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));					// ��忡 ���ԵǾ� ���� �ʴٴ� �޽����� ����Ѵ�.

	//			return;																	// ���� ó���� �Ѵ�.
	//		}
	//		else if(HERO->GetGuildMemberRank() != GUILD_MASTER)							// HERO�� ����� ��� �����Ϳ� ���� ������,
	//		{
	//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(323));					// ������ ���ٴ� �޽����� ����Ѵ�.

	//			return;																	// ���� ó���� �Ѵ�.
	//		}

	//		MSGBASE Msg;																// �޽��� ����ü�� �����Ѵ�.
	//		Msg.Category = MP_SIEGEWAR;													// ī�װ��� �������� �����Ѵ�.
	//		Msg.Protocol = MP_SIEGEWAR_PROPOSALLIST_SYN;								// ���������� ������ ��û ����Ʈ ��ũ�� �����Ѵ�.
	//		Msg.dwObjectID = HEROID;													// HERO ���̵� �����Ѵ�.

	//		NETWORK->Send( &Msg, sizeof(Msg) );											// �޽����� �����Ѵ�.
	//	}
	//	break;

	//case 9:																				// �������� �����Ϸ��� �ϸ�,
	//	{	
	//		WINDOWMGR->MsgBox( MBI_SW_OBSERVER_SYN, MBT_YESNO, CHATMGR->GetChatMsg(332) );	// ������ �ϰڳĴ� �˸�â�� ����.

	//		return;																		// ���� ó���� �Ѵ�.
	//	}
	//	break;

	//case 10:																			// ������ ����.
	//	{
	//		MSGBASE Msg;																// �޽��� ����ü�� �����Ѵ�.
	//		Msg.Category = MP_SIEGEWAR;													// ī�װ��� �������� �����Ѵ�.
	//		Msg.Protocol = MP_SIEGEWAR_GUILDLIST_SYN;									// ���������� ������ ��� ����Ʈ ��ũ�� �����Ѵ�.
	//		Msg.dwObjectID = HEROID;													// HERO ���̵� �����Ѵ�.

	//		NETWORK->Send( &Msg, sizeof(Msg) );											// �޽����� �����Ѵ�.
	//	}
	//	break;

	//case 11:																			// ���������� ���⸦ �Ѵٸ�,
	//	{
	//		if(HERO->GetGuildIdx() == 0)												// HERO�� ��� �ε����� ��ȿ���� ������,
	//		{
	//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));					// ��忡 ���ԵǾ� ���� �ʴٴ� �޽����� ����Ѵ�.

	//			return;																	// ���� ó���� �Ѵ�.
	//		}
	//		
	//		MSG_DWORD2 Msg;																// �޽��� ����ü�� �����Ѵ�.
	//		Msg.Category = MP_SIEGEWAR;													// ī�װ��� ���������� �����Ѵ�.
	//		Msg.Protocol =  MP_SIEGEWAR_MOVEIN_SYN;										// ���������� ������ ������ ��ũ�� �����Ѵ�.
	//		Msg.dwObjectID = HEROID;													// HERO ���̵� üũ�Ѵ�.
	//		Msg.dwData1 = HERO->GetGuildIdx();											// HERO�� ��� �ε����� �����Ѵ�.
	//		Msg.dwData2 = 0;															// ������ 2�� 0���� �����Ѵ�.

	//		NETWORK->Send( &Msg, sizeof(Msg) );											// �޽����� �����Ѵ�.
	//		
	//		return;																		// ���� ó���� �Ѵ�.
	//	}
	//	break;

	//case 12:																			// ���������� ���⸦ �Ѵٸ�,
	//	{
	//		if(HERO->GetGuildIdx() == 0)												// HERO�� ��� �ε����� ��ȿ���� ������,
	//		{
	//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));					// ��忡 ���ԵǾ� ���� �ʴٴ� �޽����� ����Ѵ�.

	//			return;																	// ���� ó���� �Ѵ�.
	//		}

	//		MSG_DWORD2 Msg;																// �޽��� ����ü�� �����Ѵ�.
	//		Msg.Category = MP_SIEGEWAR;													// ī�װ��� ���������� �����Ѵ�.
	//		Msg.Protocol =  MP_SIEGEWAR_MOVEIN_SYN;										// ���������� ������ ���� ��ũ�� �Ѵ�.
	//		Msg.dwObjectID = HEROID;													// HERO ���̵� �����Ѵ�.
	//		Msg.dwData1 = HERO->GetGuildIdx();											// ��� �ε����� �����Ѵ�.
	//		Msg.dwData2 = 1;															// ������2�� 1�� �����Ѵ�.

	//		NETWORK->Send( &Msg, sizeof(Msg) );											// �޽����� �����Ѵ�.
	//		
	//		return;																		// ���� ó���� �Ѵ�.
	//	}
	//	break;

	//case 13:																			// ���� ������û
	//	{
	//		if(HERO->GetGuildIdx() == 0)												// HERO�� ��� �ε����� ��ȿ���� ������,
	//		{
	//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));					// ��忡 ���ԵǾ� ���� �ʴٴ� �޽����� ����Ѵ�.

	//			return;																	// ���� ó���� �Ѵ�.
	//		}	
	//		else if(HERO->GetGuildMemberRank() != GUILD_MASTER)							// HERO�� ����� ��� �����Ϳ� ���� ������,
	//		{	
	//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(323));					// ������ ���ٴ� �޽����� ����Ѵ�.

	//			return;																	// ���� ó���� �Ѵ�.									
	//		}
	//		
	//		WINDOWMGR->MsgBox( MBI_SW_PROPOSAL_SYN, MBT_YESNO, CHATMGR->GetChatMsg(333) );	// ������ ����忡�� �������� ��û�� �ϽðڳĴ� �˸�â�� ����.

	//		return;																		// ���� ó���� �Ѵ�.
	//	}
	//	break;

	//case 14:																			// ���� ������û
	//	{
	//		if(HERO->GetGuildIdx() == 0)												// HERO�� ��� �ε����� ��ȿ���� ������,
	//		{																			
	//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));					// ��忡 ���ԵǾ� ���� �ʴٴ� �޽����� ����Ѵ�.
	//																					
	//			return;																	// ���� ó���� �Ѵ�.
	//		}																			
	//		else if(HERO->GetGuildMemberRank() != GUILD_MASTER)							// HERO�� ����� ��� �����Ϳ� ���� ������,
	//		{																			
	//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(323));					// ������ ���ٴ� �޽����� ����Ѵ�.
	//																					
	//			return;																	// ���� ó���� �Ѵ�.									
	//		}

	//		WINDOWMGR->MsgBox( MBI_SW_ATTACKREGIST_SYN, MBT_YESNO, CHATMGR->GetChatMsg(334) );	// ���� �����ϱ� ���� �������� ��û�� �Ͻðڽ��ϱ�?	

	//		return;																		// ���� ó���� �Ѵ�.		
	//	}
	//	break;

	//case 15:
	//case 16:
	//case 17:
	//case 18:
	//case 19:
	//	{
	//		if(HERO->GetGuildIdx() == 0)												// HERO�� ��� �ε����� ��ȿ���� ������,
	//		{																			
	//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(325));					// ��忡 ���ԵǾ� ���� �ʴٴ� �޽����� ����Ѵ�.
	//																					
	//			return;																	// ���� ó���� �Ѵ�.
	//		}																			
	//		if(HERO->GetGuildMemberRank() != GUILD_MASTER)								// HERO�� ����� ��� �����Ϳ� ���� ������,
	//		{																			
	//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(326));					// ��� ������ ���ٴ� �޽����� ����Ѵ�.
	//																					
	//			return;																	// ���� ó���� �Ѵ�.									
	//		}

	//		if( GUILDMGR->CheckChangeLocation( dwData ) )								// ������ Ȯ���Ͽ� TRUE �̸�,
	//		{
	//			// �������� �̵��ϰڳĴ� �޽����� ����.
	//			WINDOWMGR->MsgBox( MBI_GUILD_CHANGE_LOCATION, MBT_YESNO, CHATMGR->GetChatMsg(335), GetMapName( GUILDMGR->GetTempLocation() ) );
	//		}
	//		else																		// FALSE�̸�,
	//			return;																	// ���� ó���� �Ѵ�.
	//	}
	//	break;

	//case 20:																			// ������ �ð� ����.
	//	{
	//		MSGBASE Msg;																// �޽��� ����ü�� �����Ѵ�.
	//		Msg.Category = MP_SIEGEWAR;													// ī�װ��� ���������� �����Ѵ�.
	//		Msg.Protocol = MP_SIEGEWAR_TIMEINFO_SYN;									// ���������� ������ �ð����� ��ũ�� �����Ѵ�.
	//		Msg.dwObjectID = HEROID;													// HERO ���̵� �����Ѵ�.

	//		NETWORK->Send( &Msg, sizeof(Msg) );											// �޽����� �����Ѵ�.
	//	}
	//	break;
	}
}

void cNpcScriptManager::Business_Wanted(DWORD dwData)  									// ������� �����Ͻ�.
{
	//���� Ŭ���Ѱ� �ʰԿ� ���� �ֳ�
	if(GAMEIN->GetWantedDialog()->IsActive() == TRUE)									// ������� ���̾�αװ� Ȱ��ȭ ���̸�,
	{
		return;																			// ���� ó���� �Ѵ�.
	}

	if(dwData == 0) //â�� ����
	{
		WANTEDMGR->SortWantedListSyn(1);												// ������� ����Ʈ�� �����ϴ� ��ũ�� ������.
	}
	else if(dwData == 1)
	{
		GAMEIN->GetBailDialog()->Open();												// ������ ���̾�α׸� ����.
	}
}

void cNpcScriptManager::Business_Suryun(DWORD dwData)  									// ���� �����Ͻ�.
{
	if( dwData == eSuryunNpc_Suryun )													// �����ϱ�		
	{
	}
	else if( dwData == eSuryunNpc_About_GuildTournament )								// ������ʸ�Ʈ �˾ƺ���
	{
		if( HERO->GetState() == eObjectState_Deal )										// HERO ���°� �ŷ����̸�,
		{
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);					// �ŷ� ���¸� �����Ѵ�.
		}
	}
	else if( dwData == eSuryunNpc_Regist_GuildTournament )								// ������ʸ�Ʈ ������û�ϱ�
	{
		MSGBASE msg;																	// �޽��� ����ü�� �����Ѵ�.
		msg.Category = MP_GTOURNAMENT;													// ī�װ��� ��� ��ʸ�Ʈ�� �����Ѵ�.
		msg.Protocol = MP_GTOURNAMENT_REGISTCOUNT;										// ���������� ��� ��ʸ�Ʈ ��� ī��Ʈ�� �����Ѵ�.
		msg.dwObjectID = HEROID;														// HERO ���̵� �����Ѵ�.
			
		NETWORK->Send( &msg, sizeof(msg) );												// �޽����� �����Ѵ�.
	}
	else if( dwData == eSuryunNpc_Cancel_GuildTournament )								// ������ʸ�Ʈ ��û ����ϱ�
	{
		if( GAMEIN->GetGTRegistcancelDlg() )											// ��� ��ʸ�Ʈ ��� ���̾�α� ������ ��ȿ�ϸ�,
		{
			GAMEIN->GetGTRegistcancelDlg()->SetActive( TRUE );							// ��� ��ʸ�Ʈ ��� ���̾�α� ���̾�α׸� Ȱ��ȭ �Ѵ�.
		}
	}
	else if( dwData == eSuryunNpc_Show_GuildStand )										// ������ʸ�Ʈ ����ǥ ����
	{
		MSG_WORD msg;																	// �޽��� ����ü�� �����Ѵ�.
		msg.Category = MP_GTOURNAMENT;													// ī�װ��� ��� ��ʸ�Ʈ�� �����Ѵ�.
		msg.Protocol = MP_GTOURNAMENT_STANDINGINFO_SYN;									// ���������� ��� ��ʸ�Ʈ ���ĵ� ���� ��ũ�� �����Ѵ�.
		msg.wData = 1;																	// �����͸� 1�� �����Ѵ�.
		msg.dwObjectID = HEROID;														// HERO ���̵� �����Ѵ�.

		NETWORK->Send( &msg, sizeof(msg) );												// �޽����� �����Ѵ�.
	}
	else if( dwData == eSuryunNpc_Enter_GuildTournament )								// ������ʸ�Ʈ �����ϱ�
	{
		MSG_DWORD msg;																	// �޽��� ����ü�� �����Ѵ�.
		msg.Category = MP_GTOURNAMENT;													// ī�װ��� ��� ��ʸ�Ʈ�� �����Ѵ�.
		msg.Protocol = MP_GTOURNAMENT_MOVETOBATTLEMAP_SYN;								// ���������� ��� ��ʸ�Ʈ ��Ʋ������ �̵� ��ũ�� �����Ѵ�.
		msg.dwObjectID = HEROID;														// HERO ���̵� �����Ѵ�.
		msg.dwData = 0;																	// �����͸� 0���� �����Ѵ�.

		NETWORK->Send( &msg, sizeof(msg) );												// �޽����� �����Ѵ�.
	}
	else if( dwData == eSuryunNpc_EnterObserver_GuildTournament )						// �������� ������ʸ�Ʈ �����ϱ�
	{
		GAMEIN->SetGuildIdx( HERO->GetGuildIdx() );										// ��� �ε����� HERO�� ��� �ε����� �����Ѵ�.

		MSGBASE msg;																	// �޽��� ����ü�� �����Ѵ�.
		msg.Category = MP_GTOURNAMENT;													// ī�װ��� ��� ��ʸ�Ʈ�� �����Ѵ�.
		msg.Protocol = MP_GTOURNAMENT_BATTLEINFO_SYN;									// ���������� ��� ��ʸ�Ʈ ��Ʋ ���� ��û���� �����Ѵ�.
		msg.dwObjectID = HEROID;														// HERO ���̵� �����Ѵ�.

		NETWORK->Send( &msg, sizeof(msg) );												// �޽����� �����Ѵ�.
	}
	else if( dwData == eSuryunNpc_SkillTraining )										// ��ų �����̶��,
	{
		if(GAMEIN->GetNpcScriptDialog()->IsActive())									// NPC ��ũ��Ʈ ���̾�αװ� ���� �ִٸ�,
		{
			NPCSCRIPTMGR->SetSelectedNpc(NULL);											// ���� �� NPC�� �����Ѵ�.
			GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);								// NPC ��ũ��Ʈ ���̾�α׸� �ݴ´�.
		}
	
		GAMEIN->GetSkillTrainingDlg()->OpenDialog();									// ��ų ���� ���̾�α׸� ����.

		return;																			// ���� ó���� �Ѵ�.
	}
}

void cNpcScriptManager::Business_MapChange(DWORD dwData)  								// �� ü���� �����Ͻ�.
{
	if(dwData == 0)																		// �����Ͱ� 0�� ������,
	{
		MSG_DWORD msg;																	// �޽��� ����ü�� �����Ѵ�.
		msg.Category = MP_NPC;															// ī�װ��� NPC �� �����Ѵ�.
		msg.Protocol = MP_NPC_DOJOB_SYN;												// ���������� NPC ���� ���� ��û���� �����Ѵ�.
		msg.dwObjectID = HEROID;														// HERO ���̵� �����Ѵ�.
		msg.dwData = m_pCurSelectedNpc->GetID();										// ���� ���� �� NPC�� ���̵� �����Ѵ�.

		NETWORK->Send(&msg, sizeof(msg));												// �޽����� �����Ѵ�.
	}
	else																				// �����Ͱ� 0�� �ƴϸ�,
	{
		if(GAMEIN->GetNpcScriptDialog()->IsActive())									// NPC ��ũ��Ʈ ���̾�αװ� ����������,
		{
			NPCSCRIPTMGR->SetSelectedNpc(NULL);											// ���� ���� �� NPC�� �����Ѵ�.
			GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);								// NPC ��ũ��Ʈ ���̾�α׸� �ݴ´�.
		}
		
		if( HERO->GetState() == eObjectState_Deal )										// HERO ���°� �ŷ����̸�,
		{
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);					// �ŷ� ���¸� ������.
		}
	}
}

void cNpcScriptManager::Business_Symbol()  												// �ɺ� �����Ͻ�.
{
	// ���ν���		
	if( HERO->GetGuildMemberRank() == GUILD_MASTER )									// HERO�� ����� ��� �����Ͷ��,
	{
		CGuildDialog* dialog = GAMEIN->GetGuildDlg();

		if( dialog->GetLevel() == 5 )											// ��� ������ ��ȿ�ϰ�, ��� ������ 5�� ������,
		{
			MSG_DWORD msg;																// �޽��� ����ü�� �����Ѵ�.
			SetProtocol( &msg, MP_SIEGEWAR, MP_SIEGEWAR_ENGRAVE_SYN );					// ī�װ��� ������, ���������� ������ ����� ��û���� �����Ѵ�.
			msg.dwObjectID = HEROID;													// HERO ���̵� �����Ѵ�.
			msg.dwData = HERO->GetGuildIdx();											// HERO�� ��� �ε����� �����Ѵ�.

			NETWORK->Send( &msg, sizeof(msg) );											// �޽����� �����Ѵ�.
		}
		else																			// HERO�� ��� ������ ��ȿ���� �ʰų�, ��� ������ 5�� ���� ������,
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(336) );					// �ϼ��� ��常�� ������ �� �� �ִٴ� �޽����� ����Ѵ�.
		}
	}																					// HERO�� ����� �����Ͱ� �ƴϸ�,
	else
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(337) );						// ��� �����͸��� ������ �� �� �ִٴ� �޽����� ����Ѵ�.
	}
}

void cNpcScriptManager::Business_Family(DWORD dwData)  									// �йи� �����Ͻ�.
{
	if( dwData == 0 )																	// �йи��� �����Ϸ��� �ϸ�,
	{
		if(GAMEIN->GetNpcScriptDialog()->IsActive())									// NPC��ũ��Ʈ ���̾�ΰ� ����������,
		{
			NPCSCRIPTMGR->SetSelectedNpc(NULL);											// ���� �� NPC�� �����Ѵ�.
			GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);								// NPC ��ũ��Ʈ ���̾�α׸� �ݴ´�.
		}

		GAMEIN->GetFamilyCreateDlg()->SetActive(TRUE);									// �йи� ���� ���̾�α׸� Ȱ��ȭ �Ѵ�.
	}
	else if( dwData == 1 )																// �йи��� ��ü�Ϸ��� �ϸ�,
	{
		if(GAMEIN->GetNpcScriptDialog()->IsActive())									// NPC ��ũ��Ʈ�� ���� �ִٸ�,
		{
			NPCSCRIPTMGR->SetSelectedNpc(NULL);											// ���� �� NPC�� ���� �Ѵ�.
			GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);								// NPC ��ũ��Ʈ�� �ݴ´�.
		}				

   		WINDOWMGR->MsgBox( MBI_FAMILY_BREAKUP, MBT_YESNO, CHATMGR->GetChatMsg( 1144 ), int((CSHFamilyManager::LEAVE_PENALTY_EXP_DOWN_RATE[CSHFamilyManager::FLK_BREAKUP]+0.001)*100), CSHFamilyManager::LEAVE_PENALTY_REJOIN_TIME[CSHFamilyManager::FLK_BREAKUP] );
	}
	else if(dwData == 2 )																// �ֹε���� �߱� �������� �ϸ�,
	{
		const DATE_MATCHING_INFO& dateInfo = HERO->GetDateMatchingInfo() ;				// ����Ʈ ��Ī ������ �޴´�.

		if( &dateInfo )																	// ������ ��ȿ�ϸ�,
		{
			if( dateInfo.bIsValid )														// �̹� �߱� �޾Ҵٸ�,
			{
				//WINDOWMGR->MsgBox( MBI_IDENTIFICATION_ALREADY_HAVE, MBT_OK, "�̹� �ֹε������ �߱� �޾ҽ��ϴ�.") ;
				WINDOWMGR->MsgBox( MBI_IDENTIFICATION_ALREADY_HAVE, MBT_OK, CHATMGR->GetChatMsg( 830 ) );

				// 080122 LYW --- NpcScriptManager : npc ���̾�α� �ݱ�.
				cNpcScriptDialog* pNpcDlg = NULL ;
				pNpcDlg = GAMEIN->GetNpcScriptDialog() ;
				
				if( pNpcDlg )
				{
					pNpcDlg->SetActive(FALSE) ;
				}
			}
			else																		// �߱� ���� �ʾҴٸ�,
			{
				CIdentification* pIdentificationDlg = GAMEIN->GetIdentificationDlg() ;	// �ֹε�� �߱�â ������ �޴´�.

				if(pIdentificationDlg)													// �߱�â ������ ��ȿ�ϸ�,
				{
					pIdentificationDlg->SettingControls() ;								// ��Ʈ�� �������� �����Ѵ�.

					if( !pIdentificationDlg->IsActive() )								// �߱�â�� �������� ������,
					{
						pIdentificationDlg->SetActive(TRUE) ;							// �߱�â�� ����.
					}
				}

				CFavorIcon* pFavorIconDlg = GAMEIN->GetFavorIconDlg() ;					// ���� â ������ �޴���.

				if( pFavorIconDlg )														// ���� â ������ ��ȿ�ϸ�,
				{
					if( !pFavorIconDlg->IsActive() )									// ����â�� �������� ������,
					{
						pFavorIconDlg->SetActiveRecursive(TRUE) ;						// ����â�� Ȱ��ȭ �Ѵ�.
					}
				}

			}
		}
	}
}

void cNpcScriptManager::Business_Identifycation()  										// �ֹε�� �����Ͻ�.
{
	const DATE_MATCHING_INFO& dateInfo = HERO->GetDateMatchingInfo() ;					// ����Ʈ ��Ī ������ �޴´�.

	if( &dateInfo )																		// ������ ��ȿ�ϸ�,
	{
		if( dateInfo.bIsValid )															// �̹� �߱� �޾Ҵٸ�,
		{
			//WINDOWMGR->MsgBox( MBI_IDENTIFICATION_ALREADY_HAVE, MBT_OK, "�̹� �ֹε������ �߱� �޾ҽ��ϴ�.") ;
			WINDOWMGR->MsgBox( MBI_IDENTIFICATION_ALREADY_HAVE, MBT_OK, CHATMGR->GetChatMsg( 830 ) );

			// 080122 LYW --- NpcScriptManager : npc ���̾�α� �ݱ�.
			cNpcScriptDialog* pNpcDlg = NULL ;
			pNpcDlg = GAMEIN->GetNpcScriptDialog() ;
			
			if( pNpcDlg )
			{
				pNpcDlg->SetActive(FALSE) ;
			}
		}
		else																			// �߱� ���� �ʾҴٸ�,
		{
			CIdentification* pIdentificationDlg = GAMEIN->GetIdentificationDlg() ;		// �ֹε�� �߱�â ������ �޴´�.

			if(pIdentificationDlg)														// �߱�â ������ ��ȿ�ϸ�,
			{
				pIdentificationDlg->SettingControls() ;									// ��Ʈ�� �������� �����Ѵ�.

				if( !pIdentificationDlg->IsActive() )									// �߱�â�� �������� ������,
				{
					pIdentificationDlg->SetActive(TRUE) ;								// �߱�â�� ����.
				}
			}

			CFavorIcon* pFavorIconDlg = GAMEIN->GetFavorIconDlg() ;						// ���� â ������ �޴���.

			if( pFavorIconDlg )															// ���� â ������ ��ȿ�ϸ�,
			{
				if( !pFavorIconDlg->IsActive() )										// ����â�� �������� ������,
				{
					pFavorIconDlg->SetActiveRecursive(TRUE) ;							// ����â�� Ȱ��ȭ �Ѵ�.
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
//	// �ٽ��ѹ� Ȯ�� �� �ּ� ó����!!
//	char buff[256]={0,};
//
//	static DWORD dwNpcId; // Npc������ Page������ �Ϸ� �ɶ����� ���� �����ؾ� �Ѵ�.
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
//				cPage* pNewPage = new cPage; // ���ο� �������� ���
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
//	// ������ �����ϸ� �������� �����ϰ� ���� ����.
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
//		case 0:		// ��������
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
//		case 3:		// �ܼ���
//		case 4:
//		case 5:
//		case 6:		// ������
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
//		if(dwData == 0) //â�� ����
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
//		else if(dwData == 1) //����ǥ/����
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
//		else if(dwData == 3)			// �����۸�â��
//		{
//			MSGBASE msg;
//			msg.Category = MP_ITEM;
//			msg.Protocol = MP_ITEM_SHOPITEM_INFO_SYN;
//			msg.dwObjectID = HERO->GetID();
//			NETWORK->Send(&msg, sizeof(msg));
//		}
//	}
//	//SW060920 ���Ĺ�Ʋ�� â���ȯ�ֹ��� ���Ұ��� ���� ��ġ ����.// NpcJob�� �ʱ�ȭ���� �ʱ⿡ ��ġ�������� ó��.
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
//		case 9:		// ������ �����ϱ�
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
//		case 13:	// ���� ������û
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
//		case 14:	// ���� ������û
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
//		//���� Ŭ���Ѱ� �ʰԿ� ���� �ֳ�
//		if(GAMEIN->GetWantedDialog()->IsActive() == TRUE)
//			return;
//		if(dwData == 0) //â�� ����
//			WANTEDMGR->SortWantedListSyn(1);
//		else if(dwData == 1)
//			GAMEIN->GetBailDialog()->Open();
//		//
//	}
//	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == SURYUN_ROLE)
//	{
//		if( dwData == eSuryunNpc_Suryun )		// �����ϱ�
//		{
//		}
//		else if( dwData == eSuryunNpc_About_GuildTournament )		// ������ʸ�Ʈ �˾ƺ���
//		{
//			if( HERO->GetState() == eObjectState_Deal )
//				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
//		}
//		else if( dwData == eSuryunNpc_Regist_GuildTournament )		// ������ʸ�Ʈ ������û�ϱ�
//		{
//			MSGBASE msg;
//			msg.Category = MP_GTOURNAMENT;
//			msg.Protocol = MP_GTOURNAMENT_REGISTCOUNT;
//			msg.dwObjectID = HEROID;
//			NETWORK->Send( &msg, sizeof(msg) );
//		}
//		else if( dwData == eSuryunNpc_Cancel_GuildTournament )		// ������ʸ�Ʈ ��û ����ϱ�
//		{
//			if( GAMEIN->GetGTRegistcancelDlg() )
//				GAMEIN->GetGTRegistcancelDlg()->SetActive( TRUE );
//		}
//		else if( dwData == eSuryunNpc_Show_GuildStand )				// ������ʸ�Ʈ ����ǥ ����
//		{
//			MSG_WORD msg;
//			msg.Category = MP_GTOURNAMENT;
//			msg.Protocol = MP_GTOURNAMENT_STANDINGINFO_SYN;
//			msg.wData = 1;
//			msg.dwObjectID = HEROID;
//			NETWORK->Send( &msg, sizeof(msg) );
//		}
//		else if( dwData == eSuryunNpc_Enter_GuildTournament )		// ������ʸ�Ʈ �����ϱ�
//		{
//			MSG_DWORD msg;
//			msg.Category = MP_GTOURNAMENT;
//			msg.Protocol = MP_GTOURNAMENT_MOVETOBATTLEMAP_SYN;
//			msg.dwObjectID = HEROID;
//			msg.dwData = 0;
//			NETWORK->Send( &msg, sizeof(msg) );
//		}
//		else if( dwData == eSuryunNpc_EnterObserver_GuildTournament )	// �������� ������ʸ�Ʈ �����ϱ�
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
//		// ���ν���		
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
//					WINDOWMGR->MsgBox( MBI_IDENTIFICATION_ALREADY_HAVE, MBT_OK, "�̹� �ֹε������ �߱� �޾ҽ��ϴ�.") ;
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
//				WINDOWMGR->MsgBox( MBI_IDENTIFICATION_ALREADY_HAVE, MBT_OK, "�̹� �ֹε������ �߱� �޾ҽ��ϴ�.") ;
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
//					//	pIdentificationDlg->GetGender()->SetStaticText("��") ;
//					//}
//					//else
//					//{
//					//	pIdentificationDlg->GetGender()->SetStaticText("��") ;
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
