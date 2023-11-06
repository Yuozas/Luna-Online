#include "stdafx.h"																		// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.

#include ".\gamenotifymanager.h"														// ���� ���� �Ŵ��� Ŭ���� ����� �ҷ��´�.

#include "GameIn.h"																		// ���� �� Ŭ���� ����� �ҷ��´�.

#include "./Interface/cFont.h"

#include "QuestManager.h"
#include "..\[CC]Quest\QuestDefines.h"	
#include "..\[CC]Quest\QuestString.h"
#include "..\[CC]Quest\QuestInfo.h"
#include "../[CC]Header/GameResourceManager.h"

#include "Item.h"

#include "ObjectManager.h"

#include "ChatManager.h"

GLOBALTON(CGameNotifyManager)

CGameNotifyManager::CGameNotifyManager(void)											// ������ �Լ�.
{
	m_pNotifyDlg = NULL ;																// ���� ���� ��� �� ���̾�α� �����͸� nulló���� �Ѵ�.

	Init() ;																			// �ʱ�ȭ �Լ��� ȣ���Ѵ�.
}

CGameNotifyManager::~CGameNotifyManager(void)											// �Ҹ��� �Լ�.
{
	PTRLISTPOS pos = NULL ;
	pos = m_MsgList.GetHeadPosition() ;

	sGAMENOTIFY_MSG* pGameNotifyMsg ;

	while(pos)
	{
		pGameNotifyMsg = NULL ;
		pGameNotifyMsg = (sGAMENOTIFY_MSG*)m_MsgList.GetNext(pos) ;

		if( pGameNotifyMsg )
		{
			cPtrList* pList = NULL ;
			pList = &(pGameNotifyMsg->itemList) ;

			if( pList )
			{
				PTRLISTPOS itemPos = NULL ;
				itemPos = pList->GetHeadPosition() ;

				ITEM* pItem ;

				while(itemPos)
				{
					pItem = NULL ;
					pItem = (ITEM*)pList->GetNext(itemPos) ;

					if( pItem )
					{
						pList->Remove(pItem) ;

						delete pItem ;
						pItem = NULL ;
					}
				}

				pList->RemoveAll() ;

				m_MsgList.Remove(pGameNotifyMsg) ;

				delete pGameNotifyMsg ;
				pGameNotifyMsg = NULL ;
			}
		}
	}

	m_MsgList.RemoveAll() ;
}

void CGameNotifyManager::Init()
{}

void CGameNotifyManager::AddMsg(sGAMENOTIFY_MSG* pGameNotifyMsg) 						// ���� ���� �޽��� �߰� �Լ�.
{
	m_MsgList.AddTail(pGameNotifyMsg) ;
}

void CGameNotifyManager::Render()
{
	PTRLISTPOS pos = m_MsgList.GetHeadPosition() ;

	if( m_MsgList.GetCount() == 0 )
	{
		return ;
	}

	if( !pos )
	{
		return ;
	}

	PTRLISTPOS pos2 ;

	sGAMENOTIFY_MSG* pGameNotifyMsg ;

	cPtrList tempList ;

	cPtrList* pList ;

	LONG lTextWidth ;

	ITEM* pItem ;

	RECT rect ;

	while(pos)
	{
		pGameNotifyMsg = NULL ;
		pGameNotifyMsg = (sGAMENOTIFY_MSG*)m_MsgList.GetNext(pos) ;

		if( pGameNotifyMsg )
		{
			tempList.AddTail(pGameNotifyMsg) ;

			pList = &(pGameNotifyMsg->itemList) ;

			if( pList )
			{
				lTextWidth = 0 ;

				pos2 = pList->GetHeadPosition() ;

				while(pos2)
				{
					pItem = NULL ;
					pItem = (ITEM*)pList->GetNext(pos2) ;

					if( pItem )
					{
						lTextWidth += CFONT_OBJ->GetTextExtentWidth(6, pItem->string, strlen(pItem->string)) ;
					}
				}
			}
		}
	}

	DISPLAY_INFO	dispInfo ;

	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;

	int nStartXpos = (int)(dispInfo.dwWidth - lTextWidth)/2 ;

	pos = m_MsgList.GetHeadPosition() ;

	while(pos)
	{
		pGameNotifyMsg = NULL ;
		pGameNotifyMsg = (sGAMENOTIFY_MSG*)m_MsgList.GetNext(pos) ;

		if( pGameNotifyMsg )
		{
			pList = &(pGameNotifyMsg->itemList) ;

			if( pList )
			{
				if( pList->GetCount() == 0 )
				{
					m_MsgList.Remove(pGameNotifyMsg) ;

					delete pGameNotifyMsg ;
					pGameNotifyMsg = NULL ;

					continue ;
				}

				lTextWidth = 0 ;

				pos2 = pList->GetHeadPosition() ;

				while(pos2)
				{
					rect.left = nStartXpos + lTextWidth ;
					rect.top = pGameNotifyMsg->rect.top ;
					rect.right = 1 ;
					rect.bottom = 1 ;

					pItem = NULL ;
					pItem = (ITEM*)pList->GetNext(pos2) ;

					if( pItem )
					{
						if( pItem->nAlpha <= 10 )
						{
							pList->Remove(pItem) ;

							delete pItem ;
							pItem = NULL ;

							continue ;
						}

						DWORD dwFrontColor = RGBA_MERGE(pItem->rgb, pItem->nAlpha) ;
						DWORD dwBackColor  = RGBA_MERGE(RGB(10, 10, 10), pItem->nAlpha) ;
						CFONT_OBJ->RenderNoticeMsg( 6, pItem->string, lstrlen(pItem->string), &rect, dwFrontColor, dwBackColor);

						pItem->nAlpha -=2 ;

						lTextWidth += CFONT_OBJ->GetTextExtentWidth(6, pItem->string, strlen(pItem->string)) ;
					}
				}
			}

			pGameNotifyMsg->rect.top -= 1 ;
		}
	}
}

void CGameNotifyManager::UpdateSubQuest(SEND_SUBQUEST_UPDATE* msg) 						// ����Ʈ ������Ʈ ������ �����ϴ� �Լ�.
{
	if( !msg )
	{
		return ;
	}

	WORD wQuestIdx = 0 ;																// ����Ʈ �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
	wQuestIdx = msg->wQuestIdx ;														// ���ڷ� �Ѿ�� �޽����� ���� ����Ʈ �ε����� �޴´�.

	WORD wSubQuestIdx = 0 ;																// ���� ����Ʈ �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
	wSubQuestIdx = msg->wSubQuestIdx ;													// ���ڷ� �Ѿ�� �޽����� ���� ���� ����Ʈ �ε����� �޴´�.

	int nCombineKey = 0 ;
	COMBINEKEY(wQuestIdx, wSubQuestIdx, nCombineKey) ;
	CQuestString* pQuestString = QUESTMGR->GetQuestString(nCombineKey);

	if( pQuestString )
	{
		char tempBuf[1024] = {0, } ;													// �ӽ� ���۸� �����Ѵ�.

		cPtrList* pTitleList = NULL ;													// ������Ʈ ��Ʈ���� ����Ʈ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
		pTitleList = pQuestString->GetTitle() ;											// ������Ʈ ��Ʈ���� ����Ʈ�� �޴´�.

		if( pTitleList )																// ����Ʈ ������ ��ȿ�ϸ�,
		{
			sGAMENOTIFY_MSG* pNoticeMsg ;												// ���� ���� �޽��� ����ü�� �����Ѵ�.
			pNoticeMsg = new sGAMENOTIFY_MSG ;
			pNoticeMsg->nNotifyType = eSTYLE_QUEST ;									// ���� Ÿ���� ����Ʈ�� �����Ѵ�.

			pNoticeMsg->rect.left = 500 ;
			//pNoticeMsg->rect.top = 500 ;
			//pNoticeMsg->rect.top = HERO->GetTall() ;
			VECTOR3 OutPos3, inPos3;
			HERO->GetPosition(&inPos3);

			inPos3.y += 300 ;
			GetScreenXYFromXYZ(g_pExecutive->GetGeometry(), 0, &inPos3, &OutPos3);
			pNoticeMsg->rect.top = (LONG)(GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight*OutPos3.y); 

			pNoticeMsg->rect.right = 1 ;
			pNoticeMsg->rect.bottom = 500 ;

			pNoticeMsg->rect.top += ((CFONT_OBJ->GetTextHeight(6) + 5)* m_MsgList.GetCount()) ;


			ITEM* pQString = NULL ;													// ����Ʈ ��Ʈ���� ���� �����͸� �����ϰ� nulló���� �Ѵ�.

			PTRLISTPOS titlePos = NULL ;												// ������Ʈ �� ��Ʈ���� ����Ʈ ���� ��ġ�� ���� �����͸� �����ϰ� nulló�� �Ѵ�.
			titlePos = pTitleList->GetHeadPosition() ;									// ������Ʈ �� ��Ʈ���� ����Ʈ�� ���� �����Ѵ�.

			while(titlePos)																// ��ġ ������ ��ȿ�� ���� while���� ������.
			{
				pQString = NULL ;														// �����͸� nulló���� �Ѵ�.
				pQString = (ITEM*)pTitleList->GetNext(titlePos) ;					// ��ġ�� ���� ��Ʈ�� ������ �޴´�.

				if( pQString )															// ��Ʈ�� ������ ��ȿ�ϸ�,
				{
					strcat(tempBuf, pQString->string) ;									// �ӽù��ۿ� ��Ʈ���� ��´�.

					ITEM* pNoticeItem = new ITEM ;										// ���� ����Ʈ �������� ���� �������� �����Ѵ�.

					pNoticeItem->rgb = dwQuestStringColor[eQC_SUBQUEST_NAME] ;			// ���� ����Ʈ ��Ʈ���� ������ �����Ѵ�.
					pNoticeItem->nAlpha = 255 ;

					/*pNoticeMsg->red = 0 ;
					pNoticeMsg->gree = 255 ;
					pNoticeMsg->blue =  0  ;
					pNoticeMsg->nAlpha = 255 ;*/

					strcpy(pNoticeItem->string, tempBuf) ;								// ���� ����Ʈ ��Ʈ���� �����Ѵ�.

					pNoticeMsg->itemList.AddTail(pNoticeItem);							// ���� �޽��� ����ü�� ����Ʈ�� ���� ����Ʈ �������� �߰��Ѵ�.
				}
			}

			char tempBuf2[128] = {0, } ;												// �ι�° �ӽ� ���۸� �����Ѵ�.

			if( msg->dwMaxCount == 0 )
			{
				sprintf(tempBuf2, " (%d)", msg->dwData) ;// �ӽ� ���ۿ� ���� ������ ǥ���Ѵ�.
			}
			else
			{
				sprintf(tempBuf2, " (%d/%d)", msg->dwData, msg->dwMaxCount) ;// �ӽ� ���ۿ� ���� ������ ǥ���Ѵ�.
			}

			strcat(tempBuf, tempBuf2) ;													// ù ��° �ӽ� ���ۿ� �� ��° �ӽ� ���۸� �߰��Ѵ�.

			pQuestString->SetTitleStr(tempBuf) ;										// ��Ʈ���� Ÿ��Ʋ�� �����Ѵ�.


			ITEM* pCountItem = new ITEM ;												// ī��Ʈ�� ���� �������� �����Ѵ�.

			strcpy(pCountItem->string, tempBuf2) ;										// ī��Ʈ �����ۿ� ī��Ʈ ��Ʈ���� �����Ѵ�.

			pCountItem->rgb = dwQuestStringColor[eQC_COUNT];							// ī��Ʈ ��Ʈ���� ������ �����Ѵ�.
			pCountItem->nAlpha = 255 ;

			/*pNoticeMsg->red = 0 ;
			pNoticeMsg->gree = 0 ;
			pNoticeMsg->blue = 255  ;
			pNoticeMsg->nAlpha = 255 ;*/

			pNoticeMsg->itemList.AddTail(pCountItem) ;									// ���� �޽��� ����ü�� ����Ʈ�� ī��Ʈ �������� �߰��Ѵ�.
			

			m_MsgList.AddTail(pNoticeMsg) ;												// ���� ���� �Ŵ����� ���� �޽����� �߰��Ѵ�.
		}
	}
}

void CGameNotifyManager::EndSubQuest(SEND_QUEST_IDX* msg) 								// ���� ����Ʈ ����� ������ �����ϴ� �Լ�.
{
	if( !msg )
	{
		return ;
	}

	WORD wQuestIdx = 0 ;																// ����Ʈ �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
	wQuestIdx = msg->MainQuestIdx ;														// ���ڷ� �Ѿ�� �޽����� ���� ����Ʈ �ε����� �޴´�.

	WORD wSubQuestIdx = 0 ;																// ���� ����Ʈ �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
	wSubQuestIdx = msg->SubQuestIdx ;													// ���ڷ� �Ѿ�� �޽����� ���� ���� ����Ʈ �ε����� �޴´�.

	int nCombineKey = 0 ;
	COMBINEKEY(wQuestIdx, wSubQuestIdx, nCombineKey) ;
	CQuestString* pQuestString = QUESTMGR->GetQuestString(nCombineKey);

	if( pQuestString )																	// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
	{
		char tempBuf[1024] = {0, } ;													// �ӽ� ���۸� �����Ѵ�.

		cPtrList* pTitleList = NULL ;													// ������Ʈ ��Ʈ���� ����Ʈ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
		pTitleList = pQuestString->GetTitle() ;											// ������Ʈ ��Ʈ���� ����Ʈ�� �޴´�.

		if( pTitleList )																// ����Ʈ ������ ��ȿ�ϸ�,
		{
			ITEM* pQString = NULL ;													// ����Ʈ ��Ʈ���� ���� �����͸� �����ϰ� nulló���� �Ѵ�.

			PTRLISTPOS titlePos = NULL ;												// ������Ʈ �� ��Ʈ���� ����Ʈ ���� ��ġ�� ���� �����͸� �����ϰ� nulló�� �Ѵ�.
			titlePos = pTitleList->GetHeadPosition() ;									// ������Ʈ �� ��Ʈ���� ����Ʈ�� ���� �����Ѵ�.

			sGAMENOTIFY_MSG* pNoticeMsg ;												// ���� ���� �޽��� ����ü�� �����Ѵ�.
			pNoticeMsg = new sGAMENOTIFY_MSG ;
			pNoticeMsg->nNotifyType = eSTYLE_QUEST ;									// ���� Ÿ���� ����Ʈ�� �����Ѵ�.

			pNoticeMsg->rect.left = 500 ;
			VECTOR3 OutPos3, inPos3;
			HERO->GetPosition(&inPos3);

			inPos3.y += 300 ;
			GetScreenXYFromXYZ(g_pExecutive->GetGeometry(), 0, &inPos3, &OutPos3);
			pNoticeMsg->rect.top = (LONG)(GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight*OutPos3.y); 
			pNoticeMsg->rect.right = 1 ;
			pNoticeMsg->rect.bottom = 500 ;

			pNoticeMsg->rect.top += ((CFONT_OBJ->GetTextHeight(6) + 5)* m_MsgList.GetCount()) ;

			while(titlePos)																// ��ġ ������ ��ȿ�� ���� while���� ������.
			{
				pQString = NULL ;														// �����͸� nulló���� �Ѵ�.
				pQString = (ITEM*)pTitleList->GetNext(titlePos) ;					// ��ġ�� ���� ��Ʈ�� ������ �޴´�.

				if( pQString )															// ��Ʈ�� ������ ��ȿ�ϸ�,
				{
					strcat(tempBuf, pQString->string) ;									// �ӽù��ۿ� ��Ʈ���� ��´�.

					ITEM* pNoticeItem = new ITEM ;										// ���� ����Ʈ �������� ���� �������� �����Ѵ�.

					pNoticeItem->rgb = dwQuestStringColor[eQC_SUBQUEST_NAME] ;			// ���� ����Ʈ ��Ʈ���� ������ �����Ѵ�.
					pNoticeItem->nAlpha = 255 ;

					strcpy(pNoticeItem->string, tempBuf) ;								// ���� ����Ʈ ��Ʈ���� �����Ѵ�.

					pNoticeMsg->itemList.AddTail(pNoticeItem);							// ���� �޽��� ����ü�� ����Ʈ�� ���� ����Ʈ �������� �߰��Ѵ�.
				}				
			}

			char tempBuf2[128] = {0, } ;												// �ι�° �ӽ� ���۸� �����Ѵ�.

			//sprintf(tempBuf2, "(�Ϸ�)") ;												// �ӽ� ���ۿ� ���� ������ ǥ���Ѵ�.
			sprintf(tempBuf2, CHATMGR->GetChatMsg(1463)) ;								// �ӽ� ���ۿ� ���� ������ ǥ���Ѵ�.

			ITEM* pCountItem = new ITEM ;												// ī��Ʈ�� ���� �������� �����Ѵ�.

			strcpy(pCountItem->string, tempBuf2) ;										// ī��Ʈ �����ۿ� ī��Ʈ ��Ʈ���� �����Ѵ�.

			pCountItem->rgb = dwQuestStringColor[eQC_COUNT];							// ī��Ʈ ��Ʈ���� ������ �����Ѵ�.
			pCountItem->nAlpha = 255 ;

			pNoticeMsg->itemList.AddTail(pCountItem) ;									// ���� �޽��� ����ü�� ����Ʈ�� ī��Ʈ �������� �߰��Ѵ�.
			m_MsgList.AddTail(pNoticeMsg) ;												// ���� ���� �Ŵ����� ���� �޽����� �߰��Ѵ�.
		}
	}
}
