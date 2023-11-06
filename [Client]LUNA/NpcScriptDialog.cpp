

#include "stdafx.h"																		// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.

#include "WindowIDEnum.h"																// ������ ���̵� �̳� ����� �ҷ��´�.

#include "GameIn.h"																		// ���� �� Ŭ���� ����� �ҷ��´�.
#include "Quest.h"																		// ����Ʈ Ŭ���� ����� �ҷ��´�.

#include "ObjectManager.h"																// ������Ʈ �Ŵ��� ����� �ҷ��´�.
#include "ObjectStateManager.h"															// ������Ʈ ���� �Ŵ��� ����� �ҷ��´�.
#include "ChatManager.h"																// ä�� �Ŵ��� ����� �ҷ��´�.
#include "QuestManager.h"																// ����Ʈ �Ŵ��� ����� �ҷ��´�.
#include "NpcScriptManager.h"															// NPC ��ũ��Ʈ �Ŵ��� ����� �ҷ��´�.

#include "./Interface/cWindowManager.h"													// ������ �Ŵ��� ����� �ҷ��´�.

#include "./Interface/cStatic.h"														// ����ƽ Ŭ���� ����� �ҷ��´�.
#include "./Input/UserInput.h"															// ���� ��ǲ Ŭ���� ����� �ҷ��´�.

#include "cMsgBox.h"																// �޽��� �ڽ� Ŭ���� ����� �ҷ��´�.
#include "cDialogueList.h"																// ���̾�α� ����Ʈ Ŭ���� ����� �ҷ��´�.
#include "cHyperTextList.h"																// ������ �ؽ�Ʈ ����Ʈ Ŭ���� ����� �ҷ��´�.

#include "NpcImageDlg.h"																// NPC �̹��� ���̾�α� Ŭ���� ����� �ҷ��´�.
#include "NpcScriptDialog.h"															// NPC ��ũ��Ʈ ���̾�α� Ŭ���� ����� �ҷ��´�.

#include "QuestDialog.h"
#include "./Interface/GameNotifyManager.h"
#include "FishingPointDialog.h"

// desc_hseos_����ý���_01
// S ����ý��� �߰� added by hseos 2007.09.13
#include "../hseos/Farm/SHFarmManager.h"
// E ����ý��� �߰� added by hseos 2007.09.13

cNpcScriptDialog::cNpcScriptDialog()													// ������ �Լ�.
{
	m_type = WT_NPCSCRIPDIALOG;															// ������ Ÿ���� NPC ��ũ��Ʈ ���̾�α׷� �����Ѵ�.

	m_nHyperCount = 0;																	// ������ ī��Ʈ�� 0���� �����Ѵ�.

	m_nConversationHyperCount = 0 ;														// ��ȭâ�� ������ ī��Ʈ�� 0���� �����Ѵ�.

	m_dwCurNpc = 0;																		// ���� NPC ���̵� 0���� �����Ѵ�.
	m_wNpcChxNum = 0 ;																	// ���� NPC CHX ��ȣ�� 0���� �����Ѵ�.

	m_pListDlg = NULL;																	// ��縦 �����ִ� ����Ʈ �����͸� NULL ó���Ѵ�.

	m_pLinkListDlg = NULL ;																// ��ũ�� �����ִ� ����Ʈ �����͸� NULL ó���Ѵ�.

	m_pTopBtn = NULL ;																	// [ ó��ȭ�� ] ��ư�� NULL ó���Ѵ�.
	m_pEndBtn = NULL ;																	// [ ��ȭ���� ] ��ư�� NULL ó���Ѵ�.

	m_pTitleText = NULL;																// Ÿ��Ʋ �ؽ�Ʈ �����͵� NULL ó�� �Ѵ�.

	for(int i=0 ; i< MAX_REGIST_HYPERLINK;++i)											// �ִ� ��� ������ ��ũ �� ��ŭ for���� ������.
	{
		m_sHyper[i].Init();																// ī��Ʈ�� ���� ������ ��ũ ������ �ʱ�ȭ �Ѵ�.
	}

	for(int i=0 ; i< MAX_REGIST_HYPERLINK;++i)											// �ִ� ��� ������ ��ũ �� ��ŭ for���� ������.
	{
		m_sConversationHyper[i].Init() ;												// ī��Ʈ�� ���� ������ ��ũ ������ �ʱ�ȭ �Ѵ�.
	}

	m_dwQuestIdx = 0;																	// ����Ʈ �ε����� 0���� �����Ѵ�.

	m_dwMainLinkPageId = 0 ;
}

cNpcScriptDialog::~cNpcScriptDialog()													// �Ҹ��� �Լ�.
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

void cNpcScriptDialog::Linking()														// ��ũ �Լ�.
{
	m_pListDlg = (cListDialogEx*)this->GetWindowForID( NSI_LISTDLG );					// ��� ����Ʈ ������ �޾ƿ´�.

	m_pLinkListDlg = (cListDialogEx*)this->GetWindowForID(NSI_LISTDLG2) ;				// NPC�� ���õ� ��ũ�� �����ִ� ����Ʈ ������ �޾ƿ´�.

	m_pTopBtn = (cButton*)this->GetWindowForID(NSI_TOPBTN) ;							// [ ó��ȭ�� ] ��ư ������ �޾ƿ´�.
	m_pEndBtn = (cButton*)this->GetWindowForID(NSI_ENDBTN) ;							// [ ��ȭ���� ] ��ư ������ �޾ƿ´�.

	m_pTitleText = new cStatic;															// Ÿ��Ʋ �ؽ�Ʈ��ŭ �޸𸮸� �Ҵ��Ѵ�.

	if( m_pTitleText )																	// �޸� �Ҵ� ������ ��ȿ�ϴٸ�,
	{
		m_pTitleText->Init( 13, 10, 0, 0, NULL, -1 );									// Ÿ��Ʋ �ؽ�Ʈ�� �ʱ�ȭ �Ѵ�.

		m_pTitleText->SetShadow( TRUE );												// �׸��� ���θ� TRUE�� �����Ѵ�.

		m_pTitleText->SetFontIdx( 2 );													// 2�� ��Ʈ�� �����Ѵ�.

		m_pTitleText->SetAlign( TXT_LEFT );												// �ؽ�Ʈ ������ ���� ���ķ� �����Ѵ�.
		
		m_pTitleText->SetStaticText( "" );												// �ؽ�Ʈ�� ""�� �����Ѵ�.
		
		Add( m_pTitleText );															// ���̾�α׿� Ÿ��Ʋ�� �߰��Ѵ�.
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

void cNpcScriptDialog::ResetConversationList(cDialogueList* pList, DWORD dwMsg)			// ��ȭâ�� �ٽ� ���� �ϴ� �Լ�.
{
	if( !pList )
	{
		return ;
	}

	m_pListDlg->RemoveAll();															// ��� ����Ʈ�� ��� ����.

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

	WORD wIdx = 0;																		// �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.

	WORD LineInfo = 0;																	// ���� ������ ���� ������ �����ϰ� 0���� �����Ѵ�.

	DIALOGUE* temp = NULL;																// ���̾�α� ������ ���� ������ ������ �����ϰ� NULLó���� �Ѵ�.

	LINKITEM* pItem = NULL;																// ��ũ ������ ������ ���� ������ ������ �����ϰ� NULL ó���� �Ѵ�.
	LINKITEM* Prev = NULL;																// ���� ��ũ ������ ������ ���� ������ ������ �����ϰ� NULL ó���� �Ѵ�.

	while(1)																			// while���� ������.
	{
		temp = pList->GetDialogue( dwMsg, wIdx );										// �����ϰ� ���� �������� ���̾�α� ������ �޴´�.

		if( temp == NULL ) break;														// ������ ��ȿ���� ������, while���� ����������.

		pItem = new LINKITEM;															// ������ �޸𸮸� �Ҵ��Ͽ� �����ͷ� �ѱ��.

		strcpy(pItem->string, temp->str);												// �ӽ� ������ ��Ʈ���� ������ ��Ʈ���� ���縦 �Ѵ�.

		pItem->rgb = temp->dwColor;														// �ӽ� ������ ������ ������ �������� �����Ѵ�.

		//m_pConversationList.AddTail(pItem) ;

		if( Prev == NULL )																// ���� ������ ������ ���ٸ�,
		{
			m_pListDlg->cListItem::AddItem(pItem);										// ������ ������ ��� ����Ʈ�� �߰��Ѵ�.

			LineInfo = temp->wLine;														// ���������� �ӽ� ������ �������� �����Ѵ�.
		}
		else																			// ���� ������ ������ �ִٸ�,
		{
			if( LineInfo == temp->wLine )												// ���� ������ �ӽ� ���ΰ� ������,
			{
				Prev->NextItem = pItem;													// ���� �������� ���� ���������� ���� �������� �����Ѵ�.
			}
			else																		// ���� ������ �ӽ� ���ΰ� ���� ������,
			{
				m_pListDlg->cListItem::AddItem(pItem);									// ��� ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.

				LineInfo = temp->wLine;													// ���� ������ �ӽ� �������� �����Ѵ�.
			}
		}
		
		Prev = pItem;																	// ���� ������ ������ ���� ������ ������ �����Ѵ�.

		++wIdx;																			// �ε��� ������ �����Ѵ�.
	}

	m_pListDlg->ResetGuageBarPos() ;													// ��ȭ ����Ʈ�� �������� ��ġ�� �ٽ� ��´�.
}

void cNpcScriptDialog::ResetMainLinkList(cPage* pMainPage, cHyperTextList* pHyper, cListDialogEx* pListDlg)	// ��ũ �������� �ٽ� �����ϴ� �Լ�.
{
	for(int i=0 ; i< MAX_REGIST_HYPERLINK;++i)											// �ִ� ��� ���� ������ ��ũ �� ��ŭ for���� ������.
	{
		m_sHyper[i].Init();																// ī��Ʈ�� �ش��ϴ� ������ ��ũ ������ �ʱ�ȭ �Ѵ�.
	}

	m_pLinkListDlg->RemoveAll() ;														// ��ũ ����Ʈ�� ��� ����.

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

	m_nHyperCount = 0;																	// ������ ��ũ ī��Ʈ�� 0���� �����Ѵ�.

	DIALOGUE* temp = NULL;																// ���̾�α� ������ ���� ������ ������ �����ϰ� NULLó���� �Ѵ�.

	LINKITEM* pItem = NULL;																// ��ũ ������ ������ ���� ������ ������ �����ϰ� NULL ó���� �Ѵ�.

	HYPERLINK* pLink;																	// ��ũ ������ ���� �����͸� �����ϰ� NULLó���� �Ѵ�.

	int nLinkCount = pMainPage->GetHyperLinkCount();									// �������� ��ũ ī��Ʈ�� �޴´�.

	// ���� ������ ���� ����
	BOOL bQuest = QUESTMGR->IsNpcRelationQuest( m_dwCurNpc );							// ����Ʈ ���θ� �޴´�.

	for( int j = 0 ; j < nLinkCount ; ++j )												// ��ũ ī��Ʈ ��ŭ for���� ������.
	{
		pLink = pMainPage->GetHyperText(j);												// ī��Ʈ�� �ش��ϴ� ��ũ ������ �޴´�.

		if( pLink == NULL )																// ��ũ ������ ��ȿ���� ������ 
		{
			break;																		// for���� Ż���Ѵ�.
		}

		if( pLink->wLinkType == emLink_Quest && !bQuest )								// ��ũ Ÿ���� ����Ʈ�� ����, bQuest�� FALSE�� ������,
		{
			continue;																	// ����Ѵ�.
		}

		temp = pHyper->GetHyperText( pLink->wLinkId );									// ��ũ ���̵�� ���̾�α� ������ �޴´�.

		if( temp )																		// ���̾�α� ������ ��ȿ�ϸ�,
		{
			LINKITEM* pItem = new LINKITEM;												// ��ũ ������ �޸𸮸� �Ҵ��Ѵ�.

			strcpy(pItem->string, temp->str);											// ������ ��Ʈ���� ���̾�α� ��Ʈ������ �����Ѵ�.

			pItem->dwType = pLink->wLinkType;											// ������ ��ũ Ÿ���� �����Ѵ�.
			
			pListDlg->cListItem::AddItem( pItem );										// ��ũ ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.

			m_sHyper[m_nHyperCount].bUse = TRUE;										// ������ ��ũ ������ ��뿩�θ� TRUE�� �����Ѵ�.
			m_sHyper[m_nHyperCount].dwListItemIdx = pListDlg->GetItemCount()-1;			// ������ ��ũ ������ ����Ʈ ������ �ε����� �����Ѵ�.
			m_sHyper[m_nHyperCount].sHyper = *pLink;									// ������ ��ũ ������ ��ũ ������ �����Ѵ�.

			++m_nHyperCount;															// ������ ī��Ʈ�� �����Ѵ�.
		}
	}

	pListDlg->ResetGuageBarPos() ;														// ����Ʈ�� �������� ��ġ�� �ٽ� ��´�.
}

void cNpcScriptDialog::ResetConversationLinkList(cPage* pMainPage, cHyperTextList* pHyper)
{
	for(int i=0 ; i< MAX_REGIST_HYPERLINK;++i)											// �ִ� ��� ���� ������ ��ũ �� ��ŭ for���� ������.
	{
		m_sConversationHyper[i].Init();													// ī��Ʈ�� �ش��ϴ� ������ ��ũ ������ �ʱ�ȭ �Ѵ�.
	}

	m_nConversationHyperCount = 0 ;														// ��ȭ ����Ʈ�� ��ũ ī��Ʈ�� 0���� �����Ѵ�.

	DIALOGUE* temp = NULL;																// ���̾�α� ������ ���� ������ ������ �����ϰ� NULLó���� �Ѵ�.

	LINKITEM* pItem = NULL;																// ��ũ ������ ������ ���� ������ ������ �����ϰ� NULL ó���� �Ѵ�.

	HYPERLINK* pLink;																	// ��ũ ������ ���� �����͸� �����ϰ� NULLó���� �Ѵ�.

	int nLinkCount = pMainPage->GetHyperLinkCount();									// �������� ��ũ ī��Ʈ�� �޴´�.

	// ���� ������ ���� ����
	BOOL bQuest = QUESTMGR->IsNpcRelationQuest( m_dwCurNpc );							// ����Ʈ ���θ� �޴´�.

	pItem = new LINKITEM;																// ��ũ ������ �޸𸮸� �Ҵ��Ѵ�.
	strcpy( pItem->string, "" );														// ������ ��Ʈ���� ""�� �����Ѵ�.
	m_pListDlg->cListItem::AddItem( pItem );											// ���� �������� �߰��Ѵ�.

	for( int j = 0 ; j < nLinkCount ; ++j )												// ��ũ ī��Ʈ ��ŭ for���� ������.
	{
		pLink = pMainPage->GetHyperText(j);												// ī��Ʈ�� �ش��ϴ� ��ũ ������ �޴´�.

		if( pLink == NULL )																// ��ũ ������ ��ȿ���� ������ 
		{
			break;																		// for���� Ż���Ѵ�.
		}

		if( pLink->wLinkType == emLink_Quest && !bQuest )								// ��ũ Ÿ���� ����Ʈ�� ����, bQuest�� FALSE�� ������,
		{
			continue;																	// ����Ѵ�.
		}

		temp = pHyper->GetHyperText( pLink->wLinkId );									// ��ũ ���̵�� ���̾�α� ������ �޴´�.

		if( temp )																		// ���̾�α� ������ ��ȿ�ϸ�,
		{
			LINKITEM* pItem = new LINKITEM;												// ��ũ ������ �޸𸮸� �Ҵ��Ѵ�.

			strcpy(pItem->string, temp->str);											// ������ ��Ʈ���� ���̾�α� ��Ʈ������ �����Ѵ�.

			pItem->dwType = pLink->wLinkType;											// ������ ��ũ Ÿ���� �����Ѵ�.
			
			m_pListDlg->cListItem::AddItem( pItem );									// ��ũ ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.

			m_sConversationHyper[m_nConversationHyperCount].bUse = TRUE;										// ������ ��ũ ������ ��뿩�θ� TRUE�� �����Ѵ�.
			m_sConversationHyper[m_nConversationHyperCount].dwListItemIdx = m_pListDlg->GetItemCount()-1;		// ������ ��ũ ������ ����Ʈ ������ �ε����� �����Ѵ�.
			m_sConversationHyper[m_nConversationHyperCount].sHyper = *pLink;									// ������ ��ũ ������ ��ũ ������ �����Ѵ�.

			++m_nConversationHyperCount;												// ������ ī��Ʈ�� �����Ѵ�.
		}
	}

	m_pListDlg->ResetGuageBarPos() ;													//��ȭ ����Ʈ�� �������� ��ġ�� �ٽ� ��´�.
}

void cNpcScriptDialog::SettingNpcImage(cPage* pMainPage, WORD npcChxNum)				// NPC �̹����� ����� �����ϴ� �Լ�.											
{
   	GAMEIN->GetNpcImageDlg()->SetNpc( npcChxNum );										// NPC �̹��� ���̾�α׿� ���� NPC�� �����Ѵ�.
 	GAMEIN->GetNpcImageDlg()->SetEmotion( pMainPage->GetEmotion() );					// NPC�� �̸���� �����Ѵ�.
	GAMEIN->GetNpcImageDlg()->SetActive( TRUE );										// NPC �̹��� ���̾�α׸� Ȱ��ȭ �Ѵ�.
	GAMEIN->GetNpcImageDlg()->SetDisable( TRUE );										// NPC �̹��� ���̾�α׸� �𽺿��̺� ��Ų��.

	CNpc* pObj = NPCSCRIPTMGR->GetSelectedNpc();										// ���� �� NPC ������ �޴´�.

	if( pObj )																			// NPC ������ ��ȿ�ϸ�,
	{
		SetDlgTitleText( pObj->GetObjectName() );										// ���̾�α� ������ NPC �̸����� �����Ѵ�.
	}
	else																				// NPC ������ ��ȿ���� ������,
	{	
 		SetDlgTitleText( CHATMGR->GetChatMsg(42) );										// ���̾�α� ������ NPC ��ȭâ���� �Ѵ�.
		// desc_hseos_����ý���_01
		// S ����ý��� �߰� added by hseos 2007.09.13
		// ..���� ����NPC�� ���..
  		CSHFarmRenderObj* pFarmObj = g_csFarmManager.CLI_GetTargetObj();
		if (pFarmObj &&
			pFarmObj->GetInfo()->nKind == CSHFarmManager::RENDER_STEWARD_KIND)
		{
			// �̸�
   			SetDlgTitleText( pFarmObj->GetObjectName() );									
			// ������ ���� ä��â�� �ߵ��� �Ѵ�.. �ý��� �޼��� ������.
 			GAMEIN->GetChattingDlg()->SetActive( TRUE );
 			SetFocus(TRUE);
		}
		// E ����ý��� �߰� added by hseos 2007.09.13
		// 080414 KTH -- NPC
		//else if( pFarmObj &&
		//	pFarmObj->GetInfo()->nKind == CSHFarmManager::RENDER_ANIMAL_CAGE_SLOT_KIND )
		//{
		//	SetDlgTitleText( pFarmObj->GetObjectName() );
		//	GAMEIN->GetChattingDlg()->SetActive( TRUE );
		//	SetFocus(TRUE);
		//}
	}

	SetActive( TRUE );																	// NPC ��ũ��Ʈ ���̾�α׸� Ȱ��ȭ �Ѵ�.
}

BOOL cNpcScriptDialog::OpenDialog( DWORD dwNpcId, WORD npcChxNum )						// ���̾�α׸� ���� �Լ�.
{
	m_dwCurNpc = dwNpcId;																// ���� NPC ���̵� �����Ѵ�.
	m_wNpcChxNum = npcChxNum ;															// ���� NPC CHX ��ȣ�� �����Ѵ�.

	cPage* pMainPage = NPCSCRIPTMGR->GetMainPage( m_dwCurNpc );							// NPC ��ũ��Ʈ �Ŵ����� ���� ���� NPC�� ���� ������ ������ �޴´�.

	if( pMainPage == NULL ) return FALSE;												// ������ ������ ��ȿ���� ������, FALSE ���� ó���� �Ѵ�.

	cDialogueList* pList = NPCSCRIPTMGR->GetDialogueList();								// ��Ʈ��Ʈ �Ŵ����� ���� ���̾�α� ������ �޴´�.

	if( pList == NULL ) return FALSE;													// ���̾�α� ������ ��ȿ���� ������ FALSE ���� ó���� �Ѵ�.

	cHyperTextList* pHyper = NPCSCRIPTMGR->GetHyperTextList();							// ��ũ��Ʈ �Ŵ����� ���� ������ �ؽ�Ʈ ������ �޴´�.

	if( pHyper == NULL ) return FALSE;													// ������ �ؽ�Ʈ ������ ��ȿ���� ������ FALSE ���� ó���� �Ѵ�.

	DWORD dwMsg = pMainPage->GetRandomDialogue(); // �޼����� ��ȣ						// �޽����� �����ϰ� �޴´�.

	ResetConversationList(pList, dwMsg) ;												// ��ȭ ����Ʈ�� �ٽ� �����Ѵ�.

	ResetMainLinkList(pMainPage, pHyper, m_pLinkListDlg) ;								// ���� ��ũ ����Ʈ�� �ٽ� �����Ѵ�.

	SettingNpcImage(pMainPage, npcChxNum) ;												// NPC �̹����� ����̹����� �����Ѵ�.

	return TRUE;																		// TRUE�� �����Ѵ�.
}

BOOL cNpcScriptDialog::OpenLinkPage( DWORD dwPageId )									// ��ũ �������� ���� �Լ�.
{
	m_dwCurPageId = dwPageId;															// �ӽ÷� ���� ǥ�� �������� ������.	

	cPage* pPage = NPCSCRIPTMGR->GetPage( m_dwCurNpc, dwPageId );						// ���� NPC�� ���� ������ ���̵�� ������ ������ �޴´�.

	if( pPage == NULL ) return FALSE;													// ������ ������ ��ȿ���� ���ϸ� FALSE ���� ó���� �Ѵ�.

	cDialogueList* pList = NPCSCRIPTMGR->GetDialogueList();								// �޼��� ��ȣ�� �̿��Ͽ� ����� �޼����� �����´�.

	if( pList == NULL ) return FALSE;													// ���̾�α� ������ ��ȿ���� ������, FALSE ���� ó���� �Ѵ�.

	cHyperTextList* pHyper = NPCSCRIPTMGR->GetHyperTextList();							// ������ �ؽ�Ʈ ����Ʈ ������ �޴´�.

	if( pHyper == NULL ) return FALSE;													// ������ �ؽ�Ʈ ����Ʈ ������ ��ȿ���� ������ FALSE ���� ó���� �Ѵ�.

	DWORD dwMsg = pPage->GetRandomDialogue();											// ������ ������ �̿��Ͽ� �������� �����Ѵ�. // �޼����� ��ȣ

	ResetConversationList(pList, dwMsg) ;												// ��ȭ ����Ʈ�� �ٽ� �����Ѵ�.
	
	ResetConversationLinkList(pPage, pHyper) ;											// ��ȭ ����Ʈ�� ��ũ�� �����Ѵ�.
	
	GAMEIN->GetNpcImageDlg()->SetEmotion( pPage->GetEmotion() );						// NPC �̹��� ���̾�α��� �̸���� �����Ѵ�.

	return TRUE;																		// TRUE�� �����Ѵ�.
}

void cNpcScriptDialog::EndDialog()														// ���̾�α׸� �����ϴ� �Լ�.
{
	for(int i=0 ; i< MAX_REGIST_HYPERLINK;++i)											// ��� ������ ������ ��ũ ����ŭ for���� ������.
	{
		m_sHyper[i].Init();																// ������ ��ũ ������ �ʱ�ȭ �Ѵ�.
	}

	m_pListDlg->RemoveAll();															// ����Ʈ ���̾�α׸� ��� ����.

	m_nHyperCount = 0;																	// ������ ī��Ʈ ���� 0���� �����Ѵ�.

	m_dwCurNpc = 0;																		// ���� npc�� 0���� �����Ѵ�.
	m_wNpcChxNum = 0 ;																	// ���� npc chx ��ȣ�� 0���� �����Ѵ�.

	GAMEIN->GetNpcImageDlg()->SetDisable( FALSE );										// NPC �̹��� ���̾�α��� Disable�� �����Ѵ�.
	GAMEIN->GetNpcImageDlg()->SetActive( FALSE );										// NPC �̹��� ���̾�α׸� ��Ȱ��ȭ �Ѵ�.

	NPCSCRIPTMGR->SetSelectedNpc( NULL );												// ������ NPC�� �����Ѵ�.
	
	//�����Ŀ� ���� ���� �ִ�.
	if( HERO->GetState() != eObjectState_Die )											// HERO�� ���� ���°� �ƴϸ�,
	{
		BOOL chk = FALSE;

		chk = ( chk || ( ( cDialog* )GAMEIN->GetStorageDialog() )->IsActive() );
		chk = ( chk || ( ( cDialog* )GAMEIN->GetDealDialog() )->IsActive() );
		chk = ( chk || ( ( cDialog* )GAMEIN->GetGuildWarehouseDlg() )->IsActive() );

		if( !chk )
		{
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);						// ������Ʈ ���¸� �����Ѵ�.
		}
	}

	SetDlgTitleText("");																// Ÿ��Ʋ�� ""�� �����Ѵ�.
}

HYPER* cNpcScriptDialog::GetHyperInfo( DWORD dwIdx )									// ������ ������ �����ϴ� �Լ�.
{
	for(int i=0;i<m_nHyperCount;++i)													// ������ ī��Ʈ ��ŭ for���� ������.
	{
		if( m_sHyper[i].bUse && m_sHyper[i].dwListItemIdx == dwIdx)						// ���̹� ������ ��뿩�ΰ� TRUE�̰�, �ε����� ������,
		{
			return &m_sHyper[i];														// ������ ������ �����Ѵ�.
		}
	}

	return NULL;																		// NULL�� �����Ѵ�.
}

void cNpcScriptDialog::HyperLinkParser( DWORD dwIdx, BOOL bConversation )				// ������ ��ũ �ļ� �Լ�.
{
	if( bConversation )
	{
		if( m_nConversationHyperCount == 0 ) return;													// ������ ī��Ʈ�� 0�� ������ ���� ó���� �Ѵ�.
	}
	else
	{
		if( m_nHyperCount == 0 ) return;													// ������ ī��Ʈ�� 0�� ������ ���� ó���� �Ѵ�.
	}

	int nType = -1;																		// Ÿ���� -1�� �����Ѵ�.

	DWORD dwData = 0 ;																		// ������ ������ �����Ѵ�.

	int	hypertext = 0;																	// ������ �ؽ�Ʈ ������ 0���� �����Ѵ�.

	float fXpos = 0.0f ;
	float fZpos = 0.0f ;

	if( bConversation )
	{
		for( int i=0 ; i<m_nConversationHyperCount ; ++i)									// ������ ī��Ʈ �� ��ŭ for���� ������.
		{
			if( m_sConversationHyper[i].dwListItemIdx == dwIdx )							// �ε����� ������,
			{
				nType		= m_sConversationHyper[i].sHyper.wLinkType;						// ��ũ Ÿ���� �����Ѵ�.	
				dwData		= m_sConversationHyper[i].sHyper.dwData;						// ��ũ �����͸� �����Ѵ�.
				hypertext	= m_sConversationHyper[i].sHyper.wLinkId;						// ��ũ ���̵� �����Ѵ�.

				fXpos		= m_sConversationHyper[i].sHyper.fXpos ;						// �� �̵��� ����� x ��ǥ�� �޴´�.
				fZpos		= m_sConversationHyper[i].sHyper.fZpos ;						// �� �̵��� ����� y ��ǥ�� �޴´�.

				break;
			}
		}
	}
	else
	{
		for( int i=0 ; i<m_nHyperCount ; ++i)												// ������ ī��Ʈ �� ��ŭ for���� ������.
		{
			if( m_sHyper[i].dwListItemIdx == dwIdx )										// �ε����� ������,
			{
				nType = m_sHyper[i].sHyper.wLinkType;										// ��ũ Ÿ���� �����Ѵ�.	
				dwData = m_sHyper[i].sHyper.dwData;											// ��ũ �����͸� �����Ѵ�.
				hypertext = m_sHyper[i].sHyper.wLinkId;										// ��ũ ���̵� �����Ѵ�.

				fXpos		= m_sHyper[i].sHyper.fXpos ;									// �� �̵��� ����� x ��ǥ�� �޴´�.
				fZpos		= m_sHyper[i].sHyper.fZpos ;									// �� �̵��� ����� y ��ǥ�� �޴´�.

				break;
			}
		}
	}

	switch( nType )																		// ��ũ Ÿ���� Ȯ���Ѵ�.
	{
	case emLink_Page:																	// ��ũ ���������,
		{
			OpenLinkPage( dwData );														// ��ũ �������� ����.
			m_pListDlg->ResetGuageBarPos();												// �������� ��ġ�� ������ �Ѵ�.
		}
		break;

	case emLink_Open:																	// ��ũ �����̸�,
		{
			// desc_hseos_����ý���_01
			// S ����ý��� �߰� added by hseos 2008.01.23
			// ..���� ���׷��̵� ó��
			if (m_dwCurNpc == 999)
			{		
				if (m_dwCurPageId == 90		&&
					g_csFarmManager.CLI_ProcUpgradeDlg(dwData))
				{
					return;
				}
				// ..���� ������ ���� ó��
				if (hypertext == 691	&&
					dwData == 6			&&
					g_csFarmManager.CLI_ProcPayTaxDlg())
				{
					return;
				}
			}
			// E ����ý��� �߰� added by hseos 2008.01.23

			// 080514 KTH -- �ּ� ó��
			//NPCSCRIPTMGR->StartNpcBusiness( dwData );									// �ŷ�â�� ����.
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

	case emLink_End:																	// ��ũ �����,
		{
			EndDialog();																// ���̾�α׸� �����Ѵ�.
			DisActiveEx();																// Disable�Ѵ�.
		}
		break;

	case emLink_Quest:																	// ��ũ ����Ʈ��,
		{
			m_dwMainLinkPageId = dwData ;												// ���� ����Ʈ ��ũ �������� �����Ѵ�.
			OpenQuestLinkPage( dwData );												// ����Ʈ ��ũ �������� ����.
		}
		break;

	case emLink_QuestLink:																// ��ũ ����Ʈ ��ũ��,
		{
			m_dwQuestIdx = 0;															// ����Ʈ �ε����� 0���� �����Ѵ�.

			DWORD dwPage = 0;															// �������� 0���� �����Ѵ�.

			CQuest* pQuest = QUESTMGR->GetQuest( dwData );								// ����Ʈ ������ �޴´�.

			if( pQuest )																// ����Ʈ ������ ��ȿ�ϸ�,
			{
				dwPage = pQuest->GetNpcScriptPage( m_dwCurNpc );						// �������� �޴´�.

				if( dwPage == 0 )														// �������� 0�� ������,
				{
					dwPage = 30;														// �������� 30���� �����Ѵ�.
				}

				OpenLinkPage( dwPage );													// ��ũ �������� ����.

				m_dwQuestIdx = dwData;													// ����Ʈ �ε����� �����Ѵ�.
			}						
		}
		break;

	case emLink_QuestStart:																// ����Ʈ�� �����ߴٸ�,
		{
			if( m_dwQuestIdx == 45 || m_dwQuestIdx == 46 || m_dwQuestIdx == 47 )		// ����Ʈ �ε����� 45, 46, 47�̸�,
			{
				CQuest* pQuest = QUESTMGR->GetQuest( m_dwQuestIdx );					// ����Ʈ ������ �޴´�.

				if( pQuest )															// ����Ʈ ������ ��ȿ�ϴٸ�,
				{
					if( pQuest->IsTimeCheck() )											// Ÿ�� üũ ����Ʈ���,
					{
					}
					else																// Ÿ�� üũ ����Ʈ�� �ƴ϶��,
					{
						if( hypertext == 347 || hypertext == 349 )						// ������ �ؽ�Ʈ�� 347 �̳� 349���,
						{
							HERO->StartSocietyActForQuest( 259, -1, -1, TRUE );			// ����Ʈ�� ���� �׼��� �����Ѵ�.
						}

						OpenLinkPage( dwData );											// ��ũ �������� ����.
					}
				}
			}
			else																		// ����Ʈ �ε����� 45, 46, 47�� �ƴϸ�,
			{
				OpenLinkPage( dwData );													// ��ũ �������� ����.
			}

			CQuestDialog* pQuestDialog = GAMEIN->GetQuestDialog() ;

			if( pQuestDialog ) 
			{
				// 080506 LYW --- NpcScriptDialog : ����Ʈ ����� 20�� ��� á�� ��, ����Ʈ�� �Ϸ� �� �� ���� ���� ����.
				//if( pQuestDialog->GetQuestCount() >= MAX_QUEST_COUNT )
				if( pQuestDialog->GetQuestCount() > MAX_QUEST_COUNT )
				{
					//WINDOWMGR->MsgBox( NULL, MBT_OK, "����Ʈ ����� ���� ���� �� �̻� ����Ʈ�� ���� �� �� �����ϴ�." );
					WINDOWMGR->MsgBox( NULL, MBT_OK, CHATMGR->GetChatMsg( 1315 ) );

					return ;
				}
			}

			if( m_dwQuestIdx )															// ����Ʈ �ε��������� ��ȿ�ϴٸ�,
			{
				QUESTMGR->SendNpcTalkEvent( m_dwCurNpc, m_dwQuestIdx );					// NPC ��ȭ �̺�Ʈ�� ������.
			}
			
			if(NPCSCRIPTMGR->GetSelectedNpc())											// ������ NPC ������ ��ȿ�ϴٸ�,
			{
				if(NPCSCRIPTMGR->GetSelectedNpc()->GetNpcJob() == BOMUL_ROLE)			// NPC�� ������ ���� ���̶��,
				{
					MSG_DWORD msg;														// �޽��� ����ü�� �����Ѵ�.
					msg.Category = MP_NPC;												// ī�װ��� NPC�� �����Ѵ�.
					msg.Protocol = MP_NPC_DOJOB_SYN;									// ���������� ������ũ�� �����Ѵ�.
					msg.dwData = NPCSCRIPTMGR->GetSelectedNpc()->GetID();				// ������ NPC�� ���̵� �����Ѵ�.
					msg.dwObjectID = HEROID;											// HERO�� ���� �����Ѵ�.
					NETWORK->Send(&msg, sizeof(msg));									// �޽����� �����Ѵ�.
				}
			}
		}
		break;

	case emLink_EventQuestStart:														// ����Ʈ ���� �̺�Ʈ ��ũ���,
		{
			if( dwData )																// ������ ������ ��ȿ���� üũ�Ѵ�.
			{
				QUESTMGR->SendNpcTalkEventForEvent( m_dwCurNpc, dwData );				// �̺�Ʈ�� ���� NPC ��ȭ �̺�Ʈ�� ������.
			}

			EndDialog();																// ���̾�α׸� �ݴ´�.
			DisActiveEx();																// ��Ȱ��ȭ ��Ų��.
		}
		break;

	case emLink_QuestContinue:															// ����Ʈ�� ����ϴ� ��ũ���,
		{
			OpenLinkPage( dwData );														// ��ũ �������� ����.
		}
		break;

	case emLink_QuestEnd:																// ����Ʈ�� �����ϴ� ��ũ���,
		{
			QUESTMGR->QuestAbandon( m_dwQuestIdx );										// ����Ʈ�� �����Ѵ�.

			OpenLinkPage( dwData );														// ��ũ �������� ����.
		}
		break;

	case emLink_MapChange:																// �� ü���� ��ũ���,
		{
			//g_UserInput.GetHeroMove()->MapChange((WORD)dwData );						// �� ü������ �Ѵ�.
			g_UserInput.GetHeroMove()->MapChangeToNpc(dwData, fXpos, fZpos) ;
		}
		break;

	case emLink_ChangeMapPage :
		{
			cPage* pPage = NPCSCRIPTMGR->GetPage( m_dwCurNpc, dwData );					// ���� NPC�� ���� ������ ���̵�� ������ ������ �޴´�.
			if( pPage == NULL ) return ;												// ������ ������ ��ȿ���� ���ϸ� FALSE ���� ó���� �Ѵ�.

			cDialogueList* pList = NPCSCRIPTMGR->GetDialogueList();						// �޼��� ��ȣ�� �̿��Ͽ� ����� �޼����� �����´�.
			if( pList == NULL ) return ;												// ���̾�α� ������ ��ȿ���� ������, FALSE ���� ó���� �Ѵ�.

			cHyperTextList* pHyper = NPCSCRIPTMGR->GetHyperTextList();					// ������ �ؽ�Ʈ ����Ʈ ������ �޴´�.
			if( pHyper == NULL ) return ;												// ������ �ؽ�Ʈ ����Ʈ ������ ��ȿ���� ������ FALSE ���� ó���� �Ѵ�.

			DWORD dwMsg = pPage->GetRandomDialogue();									// ������ ������ �̿��Ͽ� �������� �����Ѵ�. // �޼����� ��ȣ
			ResetConversationList(pList, dwMsg) ;										// ��ȭ ����Ʈ�� �ٽ� �����Ѵ�.

			m_pListDlg->ResetGuageBarPos();												// �������� ��ġ�� ������ �Ѵ�.

			m_dwMainLinkPageId = dwData ;												// ���� ����Ʈ ��ũ �������� �����Ѵ�.
			OpenChangeMapLinkPage( dwData ) ;											// �� ���� ��ũ �������� ����.
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

void cNpcScriptDialog::SetDlgTitleText( char* strTitle )								// ���̾�α� ������ �����ϴ� �Լ�.
{
	if( !m_pTitleText ) return;															// ���̾�α� ���� ������ ��ȿ���� ������ ���� ó���� �Ѵ�.

	m_pTitleText->SetStaticText( strTitle );											// ���� �������� ���̾�α� ������ �����Ѵ�.
}

void cNpcScriptDialog::SetActive( BOOL val )											// Ȱ��ȭ ���� ���� �Լ�.
{
//	if( m_bActive == val ) return;														// ���� Ȱ��ȭ ���ο� ���� ���� ������ ���� ó���� �Ѵ�.

	if(!val)																			// ��Ȱ��ȭ�� ������,
	{
		CNpc* pNpc = NPCSCRIPTMGR->GetSelectedNpc();									// ���� ������ NPC ������ �޴´�.

		if(pNpc)																		// NPC ������ ��ȿ�ϸ�,
		{			
			if( pNpc->GetNpcJob() == BOMUL_ROLE)										// NPC ������ �ް�, ������ ���� �Ѱ� ������,
			{
				MSG_DWORD msg;															// �޽��� ����ü�� �����Ѵ�.
				msg.Category = MP_NPC;													// ī�װ��� NPC�� �����Ѵ�.
				msg.Protocol = MP_NPC_CLOSEBOMUL_SYN;									// ���������� MP_NPC_CLOSEBOMUL_SYN�� �����Ѵ�.
				msg.dwObjectID = HERO->GetID();											// HERO�� ���̵� �����Ѵ�.
				msg.dwData = NPCSCRIPTMGR->GetSelectedNpc()->GetID();					// ���� ���� �� NPC ���̵� �����Ѵ�.
				NETWORK->Send(&msg, sizeof(msg));										// �޽����� �����Ѵ�.
			}
		}

		EndDialog();																	// ���̾�α׸� ������.
	}



	cDialog::SetActiveRecursive( val );													// 								
}

// �ٸ������� ���� ȣ������ ���ÿ�!!
void cNpcScriptDialog::DisActiveEx()													// ��Ȱ��ȭ Ȯ�� �Լ�.
{
	for(int i=0 ; i< MAX_REGIST_HYPERLINK;++i)											// ��� ������ ������ ��ũ �� ��ŭ for���� ������.
	{
		m_sHyper[i].Init();																// ������ ��ũ ������ �ʱ�ȭ �Ѵ�.
	}

	m_pListDlg->RemoveAll();															// ����Ʈ ���̾�α׸� ��� ����.

	m_nHyperCount = 0;																	// ������ ī��Ʈ ���� 0���� �����Ѵ�.
	m_dwCurNpc = 0;																		// ���� npc�� 0���� �����Ѵ�.
	m_wNpcChxNum = 0 ;																	// ���� npc chx ��ȣ�� 0���� �����Ѵ�.

	NPCSCRIPTMGR->SetSelectedNpc( NULL );												// ���� ������ npc �� �����Ѵ�.

	SetDlgTitleText("");																// ���̾�α� Ÿ��Ʋ�� ""�� �����Ѵ�.

	cDialog::SetActiveRecursive( FALSE );												// ���̾�α׸� ��Ȱ��ȭ �Ѵ�.

	WINDOWMGR->SetMouseInputProcessed();												// ������ �Ŵ����� ���콺 �Է� ó�� ���μ����� ȣ���Ѵ�.
}

void cNpcScriptDialog::RefreshQuestLinkPage()
{
	for( int i = 0; i < MAX_REGIST_HYPERLINK; ++i )										// ��� ������ ������ ��ũ �� ��ŭ for���� ������.
	{
		m_sHyper[i].Init();																// ������ ��ũ ������ �ʱ�ȭ �Ѵ�.
	}

	m_pLinkListDlg->RemoveAll() ;														// ��ũ ����Ʈ�� ��� ����.				

	m_nHyperCount = 0;																	// ������ ī��Ʈ�� 0���� �����Ѵ�.

	DWORD dwPageId = 0 ;

	if( m_dwMainLinkPageId != 0 )
	{
		dwPageId = m_dwMainLinkPageId ;
	}
	else
	{
		dwPageId = m_dwCurPageId ;
	}

	cPage* pPage = NPCSCRIPTMGR->GetPage( m_dwCurNpc, dwPageId );						// ���� npc�� ������ ���̵�� ������ ������ ��´�.

	if( pPage == NULL ) return;															// ������ ������ ��ȿ���� ������ ���� ó���� �Ѵ�.

	cHyperTextList* pHyper = NPCSCRIPTMGR->GetHyperTextList();							// ������ �ؽ�Ʈ ������ �޴´�.

	if( pHyper == NULL ) return;														// ������ �ؽ�Ʈ ������ ��ȿ���� ������ ���� ó���� �Ѵ�.

	DIALOGUE* temp = NULL;																// �ӽ� ���̾�α� ������ ���� ������ ������ �����ϰ� NULL ó���� �Ѵ�.

	LINKITEM* pItem = NULL;																// ��ũ ������ ������ ���� ������ ������ �����ϰ� NULL ó���� �Ѵ�.

	HYPERLINK* pLink;																	// ��ũ ������ ���� �����͸� �����Ѵ�. // ��ũ ������ �����ͼ� �����Ѵ�.

	int nLinkCount = pPage->GetHyperLinkCount();										// ��ũ ī��Ʈ�� �����Ѵ�.

	pItem = new LINKITEM;																// ��ũ ������ �޸𸮸� �Ҵ��Ѵ�.
	strcpy( pItem->string, CHATMGR->GetChatMsg(239) );									// ������ ��Ʈ������ [ ����Ʈ ��� ]�� �����Ѵ�.
	pItem->rgb = RGBA_MAKE( 10, 10, 10, 255 );											// ������ ������ �����Ѵ�.
	m_pLinkListDlg->cListItem::AddItem( pItem );										// ��ũ ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.

	// ���� ����
	pItem = new LINKITEM;																// ��ũ ������ �޸𸮸� �Ҵ��Ѵ�.
	strcpy( pItem->string, "" );														// ������ ��Ʈ���� ""�� �����Ѵ�.
	m_pLinkListDlg->cListItem::AddItem( pItem );										// ���� �������� �߰��Ѵ�.

	BOOL bCheck = FALSE;																// üũ ���θ� FALSE�� �����Ѵ�.

	// ���డ���� ����
	for( int j = 0; j < nLinkCount; ++j )												// ��ũ ī��Ʈ ��ŭ for���� ������.
	{
		pLink = pPage->GetHyperText(j);													// ������ �ؽ�Ʈ ������ �޴´�.

		if( pLink == NULL )																// ������ �ؽ�Ʈ ������ ��ȿ���� ������,
		{
			break;																		// for���� Ż���Ѵ�.
		}

		CQuest* pQuest = QUESTMGR->GetQuest( pLink->dwData );							// ����Ʈ ������ �޴´�.

		if( pQuest )																	// ����Ʈ ������ ��ȿ�ϸ�,
		{
			if( pQuest->IsQuestState( m_dwCurNpc ) != 1 )								// ����Ʈ ���°��� üũ�Ѵ�.
			{
				continue;																// ����Ѵ�.
			}
		}
		else																			// ����Ʈ ������ ��ȿ���� ������,
			continue;																	// ����Ѵ�.

		if( !bCheck )																	// üũ ���ΰ� FALSE�� ������,
		{
			pItem = new LINKITEM;														// ��ũ ������ �޸𸮸� �����Ѵ�.
			strcpy( pItem->string, CHATMGR->GetChatMsg(243) );							// ���� ������ ����Ʈ�� �����Ѵ�.
			pItem->rgb = RGBA_MAKE( 0, 0, 255, 255 );									// ������ ������ �����Ѵ�.
			m_pLinkListDlg->cListItem::AddItem( pItem );								// ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.
			bCheck = TRUE;																// üũ ���θ� TRUE�� �����Ѵ�.
		}

		temp = pHyper->GetHyperText( pLink->wLinkId );									// ������ �ؽ�Ʈ ������ �޴´�.

		if( temp )																		// ������ ��ȿ�ϸ�,
		{
			pItem = new LINKITEM;														// ��ũ ������ �޸𸮸� �Ҵ��Ѵ�.
			strcpy( pItem->string, temp->str );											// ��ũ ��Ʈ���� ������ ��Ʈ���� �����Ѵ�.
			pItem->dwType = pLink->wLinkType;											// ������ Ÿ���� ��ũ Ÿ������ �����Ѵ�.
			
			m_pLinkListDlg->cListItem::AddItem( pItem );								// ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.

			// Data
			m_sHyper[m_nHyperCount].bUse = TRUE;										// ��뿩�θ� TRUE�� �����Ѵ�.
			m_sHyper[m_nHyperCount].dwListItemIdx = m_pLinkListDlg->GetItemCount()-1;	// ������ �ε����� �����Ѵ�.
			m_sHyper[m_nHyperCount].sHyper = *pLink;									// ������ ��ũ ������ �����Ѵ�.

			++m_nHyperCount;															// ������ ��ũ ī��Ʈ�� �����Ѵ�.
		}
	}

	if( bCheck )																		// üũ ���ΰ� TRUE�� ���ٸ�,
	{
		pItem = new LINKITEM;															// ��ũ ������ �޸𸮸� �Ҵ��Ѵ�.
		strcpy( pItem->string, "" );													// ��Ʈ���� ""�� �����Ѵ�.
		m_pLinkListDlg->cListItem::AddItem( pItem );									// ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.
	}

	bCheck = FALSE;																		// üũ ���θ� FALSE�� �����Ѵ�.
	// �������� ����

	for( j = 0; j < nLinkCount; ++j )													// ��ũ ī��Ʈ �� ��ŭ for���� ������.
	{
		pLink = pPage->GetHyperText(j);													// ������ ��ũ ������ �޴´�.

		if( pLink == NULL ) break;														// ������ ��ũ ������ ��ȿ���� ������ for���� Ż���Ѵ�.

		CQuest* pQuest = QUESTMGR->GetQuest( pLink->dwData );							// ����Ʈ ������ �޴´�.

		temp = pHyper->GetHyperText( pLink->wLinkId );									// ������ �ؽ�Ʈ ������ �޴´�.
		if( pQuest )																	// ����Ʈ ������ ��ȿ�ϸ�,
		{
			if( pQuest->IsQuestState( m_dwCurNpc ) != 2 )								// ����Ʈ ���¸� üũ�Ѵ�.
			{
				continue;																// ����Ѵ�.
			}
		}
		else																			// ����Ʈ ������ ��ȿ���� ������,
			continue;																	// ����Ѵ�.

		if( !bCheck )																	// üũ���ΰ� FALSE�� ������,
		{
			pItem = new LINKITEM;														// ��ũ ������ �޸𸮸� �Ҵ��Ѵ�.
			strcpy( pItem->string, CHATMGR->GetChatMsg(244) );							// ��Ʈ���� �������� ����Ʈ�� �����Ѵ�.
			pItem->rgb = RGBA_MAKE( 10, 10, 10, 255 );									// ������ ������ �����Ѵ�.
			m_pLinkListDlg->cListItem::AddItem( pItem );								// ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.
			bCheck = TRUE;																// üũ ���θ� TRUE�� �����Ѵ�.
		}

		temp = pHyper->GetHyperText( pLink->wLinkId );									// ������ �ؽ�Ʈ ������ �޴´�.

		if( temp )																		// ������ �ؽ�Ʈ ������ ��ȿ�ϸ�,
		{
			pItem = new LINKITEM;														// ��ũ ������ �޸𸮸� �Ҵ��Ѵ�.
			strcpy( pItem->string, temp->str );											// ������ ��Ʈ���� ��ũ ��Ʈ������ �����Ѵ�.

			pItem->dwType = pLink->wLinkType;											// ������ Ÿ���� ��ũ Ÿ������ �����Ѵ�.
			
			m_pLinkListDlg->cListItem::AddItem( pItem );								// ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.

			// Data
			m_sHyper[m_nHyperCount].bUse = TRUE;										// ��뿩�θ� TRUE�� �����Ѵ�.
			m_sHyper[m_nHyperCount].dwListItemIdx = m_pLinkListDlg->GetItemCount()-1;	// ������ �ε����� �����Ѵ�.
			m_sHyper[m_nHyperCount].sHyper = *pLink;									// ������ ��ũ ������ �����Ѵ�.

			++m_nHyperCount;															// ������ ��ũ ī��Ʈ�� �����Ѵ�.
		}
	}

	if( m_pLinkListDlg->GetItemCount() <= 2 )
	{
		m_pLinkListDlg->RemoveAll() ;
	}

	m_pLinkListDlg->ResetGuageBarPos() ;												// ��ũ ����Ʈ�� �������� ��ġ�� �ٽ� ��´�.
}

// 070917 LYW --- NpcScriptDialog : Add function to refresh page to change map.
void cNpcScriptDialog::RefreshChangeMapLinkPage()
{
	for( int i = 0; i < MAX_REGIST_HYPERLINK; ++i )										// ��� ������ ������ ��ũ �� ��ŭ for���� ������.
	{
		m_sHyper[i].Init();																// ������ ��ũ ������ �ʱ�ȭ �Ѵ�.
	}

	m_pLinkListDlg->RemoveAll() ;														// ��ũ ����Ʈ�� ��� ����.				

	m_nHyperCount = 0;																	// ������ ī��Ʈ�� 0���� �����Ѵ�.

	DWORD dwPageId = 0 ;

	if( m_dwMainLinkPageId != 0 )
	{
		dwPageId = m_dwMainLinkPageId ;
	}
	else
	{
		dwPageId = m_dwCurPageId ;
	}

	cPage* pPage = NPCSCRIPTMGR->GetPage( m_dwCurNpc, dwPageId );						// ���� npc�� ������ ���̵�� ������ ������ ��´�.

	if( pPage == NULL ) return;															// ������ ������ ��ȿ���� ������ ���� ó���� �Ѵ�.

	cHyperTextList* pHyper = NPCSCRIPTMGR->GetHyperTextList();							// ������ �ؽ�Ʈ ������ �޴´�.

	if( pHyper == NULL ) return;														// ������ �ؽ�Ʈ ������ ��ȿ���� ������ ���� ó���� �Ѵ�.

	DIALOGUE* temp = NULL;																// �ӽ� ���̾�α� ������ ���� ������ ������ �����ϰ� NULL ó���� �Ѵ�.

	LINKITEM* pItem = NULL;																// ��ũ ������ ������ ���� ������ ������ �����ϰ� NULL ó���� �Ѵ�.

	HYPERLINK* pLink;																	// ��ũ ������ ���� �����͸� �����Ѵ�. // ��ũ ������ �����ͼ� �����Ѵ�.

	int nLinkCount = pPage->GetHyperLinkCount();										// ��ũ ī��Ʈ�� �����Ѵ�.

	pItem = new LINKITEM;																// ��ũ ������ �޸𸮸� �Ҵ��Ѵ�.
	//strcpy( pItem->string, "[ �̵������� ���� ]" );										// ������ ��Ʈ������ [ ����Ʈ ��� ]�� �����Ѵ�.
	strcpy( pItem->string, CHATMGR->GetChatMsg(1472) );
	pItem->rgb = RGBA_MAKE( 0, 0, 255, 255 );											// ������ ������ �����Ѵ�.
	m_pLinkListDlg->cListItem::AddItem( pItem );										// ��ũ ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.

	// ���� ����
	pItem = new LINKITEM;																// ��ũ ������ �޸𸮸� �Ҵ��Ѵ�.
	strcpy( pItem->string, "" );														// ������ ��Ʈ���� ""�� �����Ѵ�.
	m_pLinkListDlg->cListItem::AddItem( pItem );										// ���� �������� �߰��Ѵ�.

	// ���డ���� ����
	for( int j = 0; j < nLinkCount; ++j )												// ��ũ ī��Ʈ ��ŭ for���� ������.
	{
		pLink = pPage->GetHyperText(j);													// ������ �ؽ�Ʈ ������ �޴´�.

		if( pLink == NULL )																// ������ �ؽ�Ʈ ������ ��ȿ���� ������,
		{
			break;																		// for���� Ż���Ѵ�.
		}

		temp = pHyper->GetHyperText( pLink->wLinkId );									// ������ �ؽ�Ʈ ������ �޴´�.

		if( temp )																		// ������ ��ȿ�ϸ�,
		{
			pItem = new LINKITEM;														// ��ũ ������ �޸𸮸� �Ҵ��Ѵ�.
			strcpy( pItem->string, temp->str );											// ��ũ ��Ʈ���� ������ ��Ʈ���� �����Ѵ�.
			pItem->dwType = pLink->wLinkType;											// ������ Ÿ���� ��ũ Ÿ������ �����Ѵ�.
			
			m_pLinkListDlg->cListItem::AddItem( pItem );								// ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.

			// Data
			m_sHyper[m_nHyperCount].bUse = TRUE;										// ��뿩�θ� TRUE�� �����Ѵ�.
			m_sHyper[m_nHyperCount].dwListItemIdx = m_pLinkListDlg->GetItemCount()-1;	// ������ �ε����� �����Ѵ�.
			m_sHyper[m_nHyperCount].sHyper = *pLink;									// ������ ��ũ ������ �����Ѵ�.

			++m_nHyperCount;															// ������ ��ũ ī��Ʈ�� �����Ѵ�.
		}
	}

	m_pLinkListDlg->ResetGuageBarPos() ;												// ��ũ ����Ʈ�� �������� ��ġ�� �ٽ� ��´�.
}


void cNpcScriptDialog::OpenQuestLinkPage( DWORD dwPageId )								// ����Ʈ ��ũ �������� ���� �Լ�.
{
	m_dwCurPageId = dwPageId;															// ������ ������ ���̺� �Ѵ�.

	RefreshQuestLinkPage() ;															// ����Ʈ ��ũ ����Ʈ�� ���ΰ�ħ �Ѵ�.
}

// 070917 LYW --- NpcScriptDialog : Add function to open page to change map.
void cNpcScriptDialog::OpenChangeMapLinkPage( DWORD dwPageId )							// �� ���� ��ũ �������� ���� �Լ�.
{
	m_dwCurPageId = dwPageId;															// ������ ������ ���̺� �Ѵ�.

	//RefreshQuestLinkPage() ;															// ����Ʈ ��ũ ����Ʈ�� ���ΰ�ħ �Ѵ�.
	RefreshChangeMapLinkPage() ;
}
