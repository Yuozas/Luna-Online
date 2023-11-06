#include "stdafx.h"																	// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.

#include ".\questquickviewdialog.h"													// ����Ʈ �˸��� Ŭ���� ����� �ҷ��´�.

#include "WindowIDEnum.h"															// ������ ���̵� �̳� ����� �ҷ��´�.

#include "./Interface/cWindowManager.h"												// ������ �Ŵ��� Ŭ���� ����� �ҷ��´�.
#include "./Interface/cListDialog.h"												// ����Ʈ ���̾�α� Ŭ���� ����� �ҷ��´�.
#include "QuestManager.h"															// ����Ʈ �Ŵ��� Ŭ���� ����� �ҷ��´�.

#include "Quest.h"																	// ����Ʈ Ŭ���� ����� �ҷ��´�.

#include "..\[CC]Quest\QuestInfo.h"
#include "..\[CC]Quest\QuestString.h"												// ����Ʈ ��Ʈ�� Ŭ���� ����� �ҷ��´�.

#include "cMsgBox.h"																// �˸� �޽��� �ڽ� Ŭ���� ����� �ҷ��´�.
#include "QuestDialog.h"															// ����Ʈ ���̾�α� Ŭ���� ����� �ҷ��´�.

#include "ChatManager.h"

CQuestQuickViewDialog::CQuestQuickViewDialog(void)									// ������ �Լ�.
{
	m_type = WT_QUESTQUICKVIEWDIALOG ;												// ������ Ÿ���� �˸��� ���̾�α׷� �����Ѵ�.

	m_QuestTreeList.RemoveAll() ;													// ����Ʈ Ʈ�� ����Ʈ�� ��� ����.

	m_pViewList = NULL ;															// Ʈ�� ����Ʈ ���̾�α� �����͸� null�� �����Ѵ�.
}

CQuestQuickViewDialog::~CQuestQuickViewDialog(void)									// �Ҹ��� �Լ�.
{
	Release() ;																		// ���� �Լ��� ȣ���Ѵ�.
}

void CQuestQuickViewDialog::Release()												// ����� ���ҽ��� �����ϴ� �Լ�.
{
	DWORD* pDeleteIdx ;																// ������ ����Ʈ �ε����� ���� �����͸� �����Ѵ�.

	PTRLISTPOS pos = NULL ;															// ��ġ ������ ���� �����͸� �����ϰ� null ó�� �Ѵ�.
	pos = m_QuestTreeList.GetHeadPosition() ;										// ����Ʈ ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	while(pos)																		// ��ġ ������ ��ȿ�� ���� while���� ������.
	{
		pDeleteIdx = NULL ;															// ������ �ε����� ���� �����͸� null ó���Ѵ�.
		pDeleteIdx = (DWORD*)m_QuestTreeList.GetNext(pos) ;							// ��ġ�� �ش��ϴ� �ε����� �ް�, ��ġ�� �������� �����Ѵ�.

		if( !pDeleteIdx ) continue ;												// ����Ʈ �ε����� ��ȿ���� ������, continue �Ѵ�.

		m_QuestTreeList.Remove( pDeleteIdx ) ;										// ����Ʈ ����Ʈ���� ���� �ε����� ������.
	
		delete pDeleteIdx ;															// ���� ����Ʈ �ε����� �����Ѵ�.
	}

	m_QuestTreeList.RemoveAll() ;													// ����Ʈ ����Ʈ�� ��� ����.
}

void CQuestQuickViewDialog::Linking()												// ��ũ �Լ�.
{
	m_pViewList = (cListDialog*)GetWindowForID(QUE_QUICKVIEWLIST) ;					// Ʈ�� ����Ʈ ���̾�α׸� ��ũ�Ѵ�.
	m_pViewList->SetStyle( LDS_FONTSHADOW ) ;										// ����Ʈ ���̾�α׸� �׸��� ��� ��Ÿ�Ϸ� �����Ѵ�.
	// 071018 LYW --- QuestQuickViewDialog : Setting shadow distance of shadow font to use render shadowfont.
	m_pViewList->Set_ShadowDistance(1) ;
}

BOOL CQuestQuickViewDialog::CheckSameQuest(DWORD dwQuestIdx)						// ����Ʈ ����Ʈ ���� ���� ����Ʈ�� �ִ��� ���θ� ��ȯ�ϴ� �Լ�.
{
	BOOL b_Result = FALSE ;															// ��� ���� ���� ������ �����ϰ� ���� false�� �����Ѵ�.

	DWORD* pCheckIdx ;																// ����Ʈ �ε��� ������ ���� �����͸� �����Ѵ�.

	PTRLISTPOS pos = NULL ;															// ��ġ ������ ���� �����͸� �����ϰ� null ó�� �Ѵ�.
	pos = m_QuestTreeList.GetHeadPosition() ;										// ����Ʈ ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	while(pos)																		// ��ġ ������ ��ȿ�� ���� while���� ������.
	{
		pCheckIdx = NULL ;															// ����Ʈ �ε��� �����͸� null ó�� �Ѵ�.
		pCheckIdx = (DWORD*)m_QuestTreeList.GetNext(pos) ;							// ��ġ�� �ش��ϴ� ����Ʈ �ε��� ������ �ް�, ���� ��ġ�� �����Ѵ�.

		if( !pCheckIdx ) continue ;													// ����Ʈ �ε��� ������ ��ȿ���� ������, continue �Ѵ�.

		if( *pCheckIdx != dwQuestIdx ) continue ;									// ���ڷ� �Ѿ�� �ε�����, ���� ����Ʈ �ε����� �ٸ���, continue �Ѵ�.

		b_Result = TRUE ;															// ��� ���� true�� �����Ѵ�.

		break ;																		// while���� ����������.
	}

	return b_Result ;																// ��� ���� return ó�� �Ѵ�.
}

void CQuestQuickViewDialog::RegistQuest(DWORD dwQuestIdx)							// �˸��� â�� ����Ʈ�� ��� ��Ű�� �Լ�.
{
	CQuest* pQuest = QUESTMGR->GetQuest(dwQuestIdx) ;								// ���ڷ� �Ѿ�� ����Ʈ �ε����� ����Ʈ ������ �޴´�.

	if( !pQuest ) return ;															// ����Ʈ ������ ��ȿ���� ������, return ó���Ѵ�.

	if( pQuest->GetSubQuestFlag() == 0 )
	{
		DeleteQuest(dwQuestIdx) ;

		return ;
	}

	if( CheckSameQuest(dwQuestIdx) )												// �˸��� â�� ���ڷ� �Ѿ�� ����Ʈ�� ���� ����Ʈ�� ������,
	{
		DeleteQuest(dwQuestIdx) ;													// ����Ʈ�� �����Ѵ�.
		return ;																	// return ó���Ѵ�.
	}

	int nRegistedCount = 0 ;														// ���� ��� �� ī��Ʈ ���� ���� ������ �����ϰ� 0���� �����Ѵ�.
	nRegistedCount = m_QuestTreeList.GetCount() ;									// ����Ʈ Ʈ�� ����Ʈ�� ī��Ʈ ���� �޴´�.

	if( nRegistedCount >= MAX_QUESTTREECOUNT )										// ��� �� ����Ʈ ���� �ִ� ��� ���ɼ� �̻��̸�,
	{
		char tempBuf[128] = {0} ;													// ����Ʈ ���Ͽ� ����� �ӽ� ���۸� �����Ѵ�.

		//sprintf(tempBuf, "�˸��� ����� %d�� ������ �� �� �ֽ��ϴ�.", 
		sprintf(tempBuf, CHATMGR->GetChatMsg(1465), 
			MAX_QUESTTREECOUNT) ;													// ��� ���� �޽����� �����Ѵ�.

		WINDOWMGR->MsgBox( MBI_QUESTQUICKVIEW_FULL, MBT_OK, tempBuf) ;				// ���� �޽����� ����Ѵ�.

		return ;																	// return ó���Ѵ�.
	}

	DWORD* pQuestIdx = new DWORD ;													// DWORD �� �����͸� �����ϰ� �����Ϳ� ��´�.

	*pQuestIdx = dwQuestIdx ;														// ����Ϸ��� ����Ʈ �ε����� �����Ѵ�.

	m_QuestTreeList.AddTail(pQuestIdx) ;											// ����Ʈ ����Ʈ�� �߰��Ѵ�.

	ResetQuickViewDialog() ;														// �˸��� â�� ���ΰ�ħ �Ѵ�.
}

void CQuestQuickViewDialog::DeleteQuest(DWORD dwQuestIdx) 							// �˸��� â�� �ִ� ����Ʈ�� ���� �ϴ� �Լ�.
{
	DWORD* pDeleteIdx ;																// ������ ����Ʈ �ε����� ���� �����͸� �����Ѵ�.

	PTRLISTPOS pos = NULL ;															// ��ġ ������ ���� �����͸� �����ϰ� null ó�� �Ѵ�.
	pos = m_QuestTreeList.GetHeadPosition() ;										// ����Ʈ ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	while(pos)																		// ��ġ ������ ��ȿ�� ���� while���� ������.
	{
		pDeleteIdx = NULL ;															// ������ �ε����� ���� �����͸� null ó���Ѵ�.
		pDeleteIdx = (DWORD*)m_QuestTreeList.GetNext(pos) ;							// ��ġ�� �ش��ϴ� �ε����� �ް�, ��ġ�� �������� �����Ѵ�.

		if( !pDeleteIdx ) continue ;												// ����Ʈ �ε����� ��ȿ���� ������, continue �Ѵ�.

		if( *pDeleteIdx != dwQuestIdx ) continue ;									// ���ڷ� �Ѿ�� �ε����� ���� �ε����� �ٸ���, continue �Ѵ�.

		m_QuestTreeList.Remove( pDeleteIdx ) ;										// ����Ʈ ����Ʈ���� ���� �ε����� ������.
	
		delete pDeleteIdx ;															// ���� ����Ʈ �ε����� �����Ѵ�.

		break ;																		// while���� Ż���Ѵ�.
	}

	ResetQuickViewDialog() ;														// �˸��� â�� ���ΰ�ħ �Ѵ�.
}

void CQuestQuickViewDialog::ResetQuickViewDialog()									// ����Ʈ ����Ʈ�� ���ΰ�ħ�ϴ� �Լ�.
{
	int nItemCount = 0 ;															// �˸��� ����Ʈ�� ������ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
	nItemCount = m_pViewList->GetItemCount() ;										// �˸��� ����Ʈ�� ������ ī��Ʈ�� �޴´�.

	m_pViewList->RemoveAll() ;														// ����Ʈ ����Ʈ ���̾�α׸� ��� ����.

	for( int count = 0 ; count < nItemCount ; ++count )								// �˸��� ����Ʈ�� ������ ī��Ʈ ��ŭ for���� ������.
	{
		m_pViewList->SetExtendReduction(LD_REDUCTION) ;								// ����Ʈ ���̾�α׸� ����Ѵ�.
	}

	DWORD* pCheckIdx ;																// �ε��� ������ ���� �����͸� �����Ѵ�.

	CQuest* pQuest ;																// ����Ʈ ������ ���� �����͸� �����Ѵ�.
	CSubQuestInfo* pSubQuestInfo ;													// ���� ����Ʈ ������ ���� �����͸� �����Ѵ�.

	SUBQUEST* pSubQuest ;															// ���� ����Ʈ�� ���� �����͸� �����Ѵ�.

	PTRLISTPOS pos = NULL ;															// ��ġ ������ ���� �����͸� �����ϰ� null ó���Ѵ�.
	pos = m_QuestTreeList.GetHeadPosition() ;										// ����Ʈ ����Ʈ�� ���� �����ϰ�, ��ġ���� �����Ѵ�.

	while(pos)																		// ��ġ ������ ��ȿ�� ���� while���� ������.
	{
		pCheckIdx = NULL ;															// �ε����� ���� �����͸� null ó���Ѵ�.
		pCheckIdx = (DWORD*)m_QuestTreeList.GetNext(pos) ;							// ��ġ�� �ش��ϴ� �ε����� �ް�, ��ġ�� �������� �����Ѵ�.

		if( !pCheckIdx ) continue ;													// �ε��� ������ ��ȿ���� ������, continue �Ѵ�.

		pQuest = NULL ;																// ����Ʈ ������ ���� �����͸� null ó���Ѵ�.
		pQuest = QUESTMGR->GetQuest(*pCheckIdx) ;									// ���� �ε����� ����Ʈ �Ŵ������� ����Ʈ ������ �޴´�.

		if( !pQuest ) continue ;													// ����Ʈ ������ ��ȿ���� ������, continue �Ѵ�.

		if( pQuest->IsQuestComplete() || !pQuest->GetSubQuest(0) )					// ���� �Ϸ��߰ų�, ���� ����Ʈ 0�� ������ ������,
		{
			DeleteQuest(*pCheckIdx) ;												// ����Ʈ�� Ʈ������ �����Ѵ�.
		}

		AddMainString( *pCheckIdx, 0 ) ;											// ����Ʈ ������ �߰��Ѵ�.

		for( int count = 0 ; count < MAX_SUBQUEST ; ++count )						// �� ����Ʈ�� ���� �� �ִ� �ִ� �� ��ŭ for���� ������.
		{
			pSubQuestInfo = NULL ;													// ���� ����Ʈ ������ ���� �����͸� null ó���Ѵ�.
			pSubQuestInfo = pQuest->GetSubQuestInfoArray(count) ;					// ī��Ʈ�� �ش��ϴ� ���� ����Ʈ ������ �޴´�.

			if( !pSubQuestInfo ) continue ;											// ���� ����Ʈ ������ ��ȿ���� ������, continue �Ѵ�.

			pSubQuest = NULL ;														// ���� ����Ʈ ������ ���� �����͸� nulló�� �Ѵ�.
			pSubQuest = pQuest->GetSubQuest(count) ;								// ī��Ʈ�� �ش��ϴ� ���� ����Ʈ ������ �޴´�.

			if(!pSubQuest) continue ;												// ���� ����Ʈ ������ ��ȿ���� ������, continue �Ѵ�.

			int nSubQuestType = e_SQT_NORMAL ;										// ����Ʈ Ÿ���� ���� ������ �����ϰ�, �⺻���� �����Ѵ�.

			if( pSubQuest->dwMaxCount > 0 )											// ��ǥ ī��Ʈ�� ������,
			{
				nSubQuestType = e_SQT_HUNT ;										// ��� ����Ʈ�̴�.
			}
			else																	// ��ǥ ī��Ʈ�� ������,
			{
				if( pSubQuest->dwData > 0 )											// ���� ī��Ʈ�� ������,
				{
					nSubQuestType = e_SQT_COLLECTION ;								// ���� ����Ʈ�̴�.
				}
				else																// ���� ī��Ʈ�� ������,
				{
					nSubQuestType = e_SQT_COMPLETED ;								// �Ϸ� ����Ʈ �̴�.
				}
			}

			AddSubString( *pCheckIdx, count, nSubQuestType ) ;						// ���� ����Ʈ ��Ʈ���� �߰��Ѵ�.
		}
	}
}

void CQuestQuickViewDialog::AddMainString(DWORD dwQuestIdx, DWORD dwSubQuestIdx)	// ����Ʈ ������ �߰��ϴ� �Լ�.
{
	CQuestString* pString = NULL ;													// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pString = QUESTMGR->GetQuestString(dwQuestIdx, dwSubQuestIdx) ;					// ����Ʈ �ε�����, ����Ʈ ��Ʈ�� ������ �޾ƿ´�.

	if( !pString ) return ;															// ����Ʈ ��Ʈ�� ������ ��ȿ���� ������, return ó�� �Ѵ�.	

	char tempBuf[256] = {0,} ;														// �ӽ� ���۸� �����Ѵ�.
	GetTitleString( tempBuf, pString ) ;											// ����Ʈ ��Ʈ���� ������ �޴´�.

	if( strlen(tempBuf) <= 4 ) return ;												// ������ ��ȿ���� ������ return ó���Ѵ�.

	m_pViewList->AddItem( tempBuf, RGB(0, 255, 255) ) ;								// ����Ʈ ������ �߰��Ѵ�.
	m_pViewList->SetExtendReduction(LD_EXTEND) ;									// ����Ʈ ���̾�α׸� Ȯ���Ѵ�.
}

void CQuestQuickViewDialog::AddSubString(DWORD dwQuestIdx, DWORD dwSubQuestIdx, int nType)		// ���� ����Ʈ ������ �߰��ϴ� �Լ�.
{
	CQuestString* pString = NULL ;													// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pString = QUESTMGR->GetQuestString(dwQuestIdx, dwSubQuestIdx) ;					// ����Ʈ �ε�����, ����Ʈ ��Ʈ�� ������ �޾ƿ´�.

	if( !pString ) return ;															// ����Ʈ ��Ʈ�� ������ ��ȿ���� ������, return ó�� �Ѵ�.	

	CQuest* pQuest = NULL ;															// ����Ʈ ������ ���� �����͸� �����ϰ� null ó���Ѵ�.
	pQuest = QUESTMGR->GetQuest(dwQuestIdx) ;										// �ε����� �ش��ϴ� ����Ʈ ������ �޴´�.

	if( !pQuest ) return ;															// ����Ʈ ������ ��ȿ���� ������, return ó�� �Ѵ�.

	SUBQUEST* pSubQuest ;															// ���� ����Ʈ ������ ���� �����͸� �����Ѵ�.
	pSubQuest = pQuest->GetSubQuest(dwSubQuestIdx) ;								// ���� ����Ʈ ������ �޴´�.

	if( !pSubQuest ) return ;														// ���� ����Ʈ ������ ��ȿ���� ������, return ó�� �Ѵ�.

	CQuestString* pQuestString ;														// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����Ѵ�.

	pQuestString = NULL ;														// ����Ʈ ��Ʈ�� ������ ���� �����͸� null ó���� �Ѵ�.
	pQuestString = QUESTMGR->GetQuestString(dwQuestIdx, 0) ;					// ���� ����Ʈ�� ���� ��Ʈ���� �޾ƿ´�.

	if( !pQuestString ) return ;

	CQuestInfo* pQuestInfo ;															// ����Ʈ ������ ���� �����͸� �����Ѵ�.

	int nSubQuestCount = 0 ;
	pQuestInfo = QUESTMGR->GetQuestInfo(pQuestString->GetQuestID()) ;				// ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.

	if( !pQuestInfo ) return ;

	char tempBuf[256] = {0,} ;														// �ӽ� ���۸� �����Ѵ�.
	strcat(tempBuf, "	- ") ;														// �������� �߰��Ѵ�.
	GetTitleString( tempBuf, pString ) ;											// ����Ʈ ��Ʈ���� ������ �޴´�.

	if( strlen(tempBuf) <= 4 ) return ;												// ������ ��ȿ���� ������ return ó���Ѵ�.

	char typeBuf[32] = {0, } ;														// Ÿ�Կ� ���� ó���� ���� Ÿ�� ���۸� �����Ѵ�.

	switch(nType)																	// ���� ����Ʈ Ÿ���� Ȯ���Ѵ�.
	{
	case e_SQT_NORMAL : break ;														// �⺻ Ÿ���ΰ��,
	case e_SQT_HUNT :																// ��� Ÿ���� ���,
		{
			sprintf(typeBuf, "(%d/%d)", pSubQuest->dwData, pSubQuest->dwMaxCount) ; // ����/��ǥ ī��Ʈ�� �߰��� �ش�.
		}
		break ;

	case e_SQT_COLLECTION :															// ���� Ÿ���� ���,
		{
			sprintf(typeBuf, "(%d)", pSubQuest->dwData) ;							// ���� ī��Ʈ�� �߰��� �ش�.
		}
		break ;

	case e_SQT_COMPLETED :															// �Ϸ� Ÿ���� ���,
		{
			//sprintf(typeBuf, "(�Ϸ�)") ;											// �ϷḦ �߰��� �ش�.
		}
		break ;
	}

	strcat(tempBuf, typeBuf) ;

	nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// ���� ����Ʈ ���� �޴´�.

	if( nSubQuestCount-1 == pQuest->GetCurSubQuestIdx() )
	{
		strcat(tempBuf, CHATMGR->GetChatMsg(1463)) ;
	}

	m_pViewList->AddItem( tempBuf, RGB(255, 255, 255) ) ;							// ����Ʈ ������ �߰��Ѵ�.
	m_pViewList->SetExtendReduction(LD_EXTEND) ;									// ����Ʈ ���̾�α׸� Ȯ���Ѵ�.
}

void CQuestQuickViewDialog::GetTitleString( char* pBuf, CQuestString* pQuestString )// ����Ʈ ��Ʈ���� ���� ��Ʈ���� ��� �Լ�.
{
	if( !pQuestString ) return ;													// ����Ʈ ��Ʈ�� ������ ��ȿ���� ������, null�� return �Ѵ�.

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














































































//#include "stdafx.h"																	// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.
//
//#include ".\questquickviewdialog.h"
//
//#include "./Input/Mouse.h"
//
//#include "WindowIDEnum.h"															// ������ ���̵� �̳� ����� �ҷ��´�.
//
//#include "./Interface/cWindowManager.h"												// ������ �Ŵ��� Ŭ���� ����� �ҷ��´�.
//#include "./Interface/GameNotifyManager.h"											// ���� ���� �Ŵ��� Ŭ���� ����� �ҷ��´�.
//#include "QuestManager.h"															// ����Ʈ �Ŵ��� Ŭ���� ����� �ҷ��´�.
//
//#include "./Interface/cListDialog.h"												// ����Ʈ ���̾�α� Ŭ���� ����� �ҷ��´�.
//#include "./Interface/cStatic.h"
//
//#include "cMsgBox.h"																// �޽��� �ڽ� Ŭ���� ����� �ҷ��´�.
//#include "QuestDialog.h"															// ����Ʈ ���̾�α� Ŭ���� ����� �ҷ��´�.
//
//#include "Quest.h"
//
//#include "..\[CC]Quest\QuestInfo.h"													// ����Ʈ ���� Ŭ���� ����� �ҷ��´�.
//#include "..\[CC]Quest\QuestString.h"												// ����Ʈ ��Ʈ�� Ŭ���� ����� �ҷ��´�.
//
//#include "UserInfoManager.h"
//
//CQuestQuickViewDialog::CQuestQuickViewDialog(void)									// ����Ʈ �˸��� ���̾�α� ������ �Լ�.
//{
//	m_type = WT_QUESTQUICKVIEWDIALOG ;												// ������ Ÿ���� �˸��� ���̾�α׷� �����Ѵ�.
//
//	m_byAlpha = 0 ;																	// ������ Alpha���� 0���� �����Ѵ�.
//
//	m_pViewList = NULL ;															// Ʈ�� ����Ʈ ���̾�α� �����͸� null�� �����Ѵ�.
//}
//
//CQuestQuickViewDialog::~CQuestQuickViewDialog(void)									// ����Ʈ �˸��� ���̾�α� �Ҹ��� �Լ�.
//{
//	Release() ;
//}
//
//void CQuestQuickViewDialog::Release()
//{
//	QuestTree* pTree ;																// Ʈ�� ������ ���� �����͸� �����Ѵ�.
//	CQuestString* pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	PTRLISTPOS pos = NULL ;															// ����Ʈ ���� ��ġ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	PTRLISTPOS treePos = NULL ;														// Ʈ���� ����Ʈ ���� ��ġ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	pos = m_QuestTreeList.GetHeadPosition() ;										// ��ġ �����͸� ����Ʈ�� ���� �����Ѵ�.
//
//	while(pos)																		// ����Ʈ�� ���� ��Ұ� ��ȿ�� ���� while���� ������.
//	{
//		pTree = NULL ;																// Ʈ�� ������ ���� �����͸� null ó���� �Ѵ�.
//		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// ��ġ ������ �ش��ϴ� Ʈ���� �޴´�.
//
//		if( pTree )																	// Ʈ�� ������ ��ȿ�ϴٸ�,
//		{
//			treePos = NULL ;														// Ʈ���� ����Ʈ���� ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.	
//			treePos = pTree->list.GetHeadPosition() ;								// Ʈ���� ����Ʈ�� ���� �����Ѵ�.
//
//			while(treePos)															// ��ġ ������ ��ȿ�� ���� while���� ������.
//			{
//				pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//				pQuestString = (CQuestString*)pTree->list.GetNext(treePos) ;		// 0��° ��ġ�� �ش��ϴ� ����Ʈ ��Ʈ���� �޴´�.
//
//				if( pQuestString )													// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
//				{
//					pTree->list.Remove(pQuestString) ;
//
//					delete pQuestString ;
//					pQuestString = NULL ;
//				}
//			}
//
//			pTree->list.RemoveAll() ;												// ����Ʈ�� ��� ����.
//
//			m_QuestTreeList.Remove(pTree) ;											// ����Ʈ ����Ʈ���� Ʈ���� �����Ѵ�.
//
//			delete pTree ;
//			pTree = NULL ;
//		}
//	}
//
//	m_QuestTreeList.RemoveAll() ;													// ����Ʈ ����Ʈ�� ��� ����.
//}
//
////CQuestQuickViewDialog::~CQuestQuickViewDialog(void)									// ����Ʈ �˸��� ���̾�α� �Ҹ��� �Լ�.
////{
////	QuestTree* pTree ;																// Ʈ�� ������ ���� �����͸� �����Ѵ�.
////	CQuestString* pQuestString ;													// ��Ʈ�� ������ ���� �����͸� �����Ѵ�.
////
////	PTRLISTPOS pos = NULL ;															// ����Ʈ ���� ��ġ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////	PTRLISTPOS treePos = NULL ;														// Ʈ���� ����Ʈ ���� ��ġ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////
////	pos = m_QuestTreeList.GetHeadPosition() ;										// ��ġ �����͸� ����Ʈ�� ���� �����Ѵ�.
////
////	while(pos)																		// ����Ʈ�� ���� ��Ұ� ��ȿ�� ���� while���� ������.
////	{
////		pTree = NULL ;																// Ʈ�� ������ ���� �����͸� null ó���� �Ѵ�.
////		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// ��ġ ������ �ش��ϴ� Ʈ���� �޴´�.
////
////		if( pTree )																	// Ʈ�� ������ ��ȿ�ϴٸ�,
////		{
////			treePos = NULL ;														// Ʈ���� ����Ʈ���� ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.	
////			treePos = pTree->list.GetHeadPosition() ;								// Ʈ���� ����Ʈ�� ���� �����Ѵ�.
////
////			while(treePos)															// ��ġ ������ ��ȿ�� ���� while���� ������.
////			{
////				pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
////				pQuestString = (CQuestString*)pTree->list.GetNext(treePos) ;		// 0��° ��ġ�� �ش��ϴ� ����Ʈ ��Ʈ���� �޴´�.
////
////				if( pQuestString )													// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
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
////			pTree->list.RemoveAll() ;												// ����Ʈ�� ��� ����.
////
////			m_QuestTreeList.Remove(pTree) ;											// ����Ʈ ����Ʈ���� Ʈ���� �����Ѵ�.
////
////			if( pTree )
////			{
////				delete pTree ;
////			}
////		}
////	}
////
////	m_QuestTreeList.RemoveAll() ;													// ����Ʈ ����Ʈ�� ��� ����.
////}
//
//void CQuestQuickViewDialog::Linking()												// ��ũ �Լ�.
//{
//	//m_pTitle = (cStatic*)GetWindowForID(QUE_QUICKVIEWTITLE) ;
//	m_pViewList = (cListDialog*)GetWindowForID(QUE_QUICKVIEWLIST) ;					// Ʈ�� ����Ʈ ���̾�α׸� ��ũ�Ѵ�.
//	m_pViewList->SetStyle( LDS_FONTSHADOW ) ;										// ����Ʈ ���̾�α� ��Ÿ���� �����Ѵ�.
//}
//
//DWORD CQuestQuickViewDialog::ActionEvent(CMouse * mouseInfo)						// �̺�Ʈ ó�� �Լ�.
//{
//	DWORD we = WE_NULL ;															// ������ �̺�Ʈ�� ���� ������ �����ϰ� nulló���� �Ѵ�.
//
//	we = cDialog::ActionEvent(mouseInfo) ;											// ���� �������� �̺�Ʈ�� �޴´�.
//
//	int nXpos = mouseInfo->GetMouseX() ;											// ���콺 X��ǥ�� �޴´�.
//	int nYpos = mouseInfo->GetMouseY() ;											// ���콺 Y��ǥ�� �޴´�.
//
//	//if( m_pTitle->PtInWindow(nXpos, nYpos) )													// ���콺�� ������ ���� ���� �ִٸ�,
//	//{
//	//	if( m_byAlpha < 230 )														// ���� ���� 230���� �۴ٸ�,
//	//	{
//	//		SetAlpha(m_byAlpha += 10) ;												// ���� ���� 10�� �����Ѵ�.
//	//	}
//	//	else																		// ���� ���� 230 �̻��̸�,
//	//	{
//	//		SetAlpha(255) ;															// ���� ���� 255�� �����Ѵ�.
//	//	}
//	//}
//	//else																			// ���콺�� ������ ���� ���̸�,
//	//{
//	//	if( m_byAlpha > 20 )														// ���� ���� 20���� ũ��,
//	//	{
//	//		SetAlpha(m_byAlpha -= 10) ;												// ���� ���� 10�� �����Ѵ�.
//	//	}
//	//	else																		// ���� ���� 20���ϸ�,
//	//	{
//	//		SetAlpha(0) ;															// ���� ���� 0���� �����Ѵ�.
//	//	}
//	//}
//
//	//if( m_pViewList->PtInWindow( nXpos, nYpos )	)									// �˸��� ����Ʈ �� ���콺�� ������,
//	//{
//	//	int nSelectedLine = -1 ;													// ���� �� ������ ���� ������ �����ϰ� -1�� �����Ѵ�.
//
//	//	if( we & WE_LBTNCLICK )														// �׸��� ���콺 ���� ��ư�� Ŭ���ߴٸ�,
//	//	{
//	//		nSelectedLine = m_pViewList->GetSelectRowIdx() ;						// �˸��� ����Ʈ�� ���� ���� �� ������ �޴´�.
//
//	//		if( nSelectedLine != -1 )												// ���� �� ������ �ε����� -1�� �ƴ϶��,
//	//		{
//	//			CheckTreeState(nSelectedLine) ;										// Ʈ�� ���¸� üũ�ϴ� �Լ��� ȣ���Ѵ�.
//	//		}
//	//	}
//	//}
//
//	return we ;																		// ������ �̺�Ʈ�� �����Ѵ�.
//}
//
//BOOL CQuestQuickViewDialog::CheckSameQuest(DWORD dwQuestIdx)						// ����Ʈ ����Ʈ ���� ���� ����Ʈ�� �ִ��� ���θ� ��ȯ�ϴ� �Լ�.
//{
//	BOOL bResult = FALSE ;															// ��� ���� ���� ������ �����ϰ� false�� �����Ѵ�.
//
//	QuestTree* pTree ;																// Ʈ�� ������ ���� �����͸� �����Ѵ�.
//
//	PTRLISTPOS pos = NULL ;															// ����Ʈ ���� ��ġ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	PTRLISTPOS treePos = NULL ;														// Ʈ���� ����Ʈ ���� ��ġ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	pos = m_QuestTreeList.GetHeadPosition() ;										// ��ġ �����͸� ����Ʈ�� ���� �����Ѵ�.
//
//	int nCount = 0 ;																// ����Ʈ ������ �ʿ��� ī��Ʈ�� �����ϰ� 0���� �����Ѵ�.
//
//	while(pos)																		// ����Ʈ�� ���� ��Ұ� ��ȿ�� ���� while���� ������.
//	{
//		pTree = NULL ;																// Ʈ�� ������ ���� �����͸� null ó���� �Ѵ�.
//		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// ��ġ ������ �ش��ϴ� Ʈ���� �޴´�.
//
//		if( pTree )																	// Ʈ�� ������ ��ȿ�ϴٸ�,
//		{
//			if( pTree->QuestIdx == dwQuestIdx )										// Ʈ���� ����Ʈ ��ȣ��, ���ڷ� �Ѿ�� ����Ʈ �ε����� ������,
//			{
//				bResult = TRUE ;													// ��� ���� true�� �����Ѵ�.
//
//				//QUESTMGR->DeleteQuestIDToQuickView(nCount) ;						// ����Ʈ �Ŵ����� ������ ����Ʈ ���̵� �����Ѵ�.
//
//				return bResult ;													// ��� ���� �����Ѵ�.
//			}
//
//			++nCount ;																// ī��Ʈ ���� �����Ѵ�.
//		}
//	}
//
//	return bResult ;																// ��� ���� �����Ѵ�.
//}
//
//void CQuestQuickViewDialog::RegistQuest(DWORD dwQuestIdx)							// ����Ʈ�� �߰��ϴ� �Լ�.
//{
//	CQuestString* pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	CQuest* pQuest = QUESTMGR->GetQuest( dwQuestIdx );							// ����Ʈ ������ �޴´�.
//
//	if( !pQuest ) return ;															// ����Ʈ ������ ��ȿ���� ������, return ó�� �Ѵ�.
//
//	CSubQuestInfo* pDescSubQuestInfo ;												// ���� ����Ʈ ������ ���� �����͸� �����Ѵ�.
//
//    if( CheckSameQuest(dwQuestIdx) )												// ����Ʈ ����Ʈ�� ���ڷ� �Ѿ�� ����Ʈ�� ���� ����Ʈ�� �ִٸ�,
//	{
//		DeleteQuest(dwQuestIdx) ;													// ����Ʈ���� ����Ʈ�� �����Ѵ�.
//
//		return ;																	// ���� ó���� �Ѵ�.
//	}
//
//	int nTreeCount = 0 ;															// ����Ʈ Ʈ�� ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//	nTreeCount = m_QuestTreeList.GetCount() ;										// ����Ʈ�� ��� ����Ʈ�� ī��Ʈ�� �޴´�.	
//
//	if( nTreeCount >= MAX_QUESTTREECOUNT )											// Ʈ�� ī��Ʈ�� �ִ� Ʈ�� �� �̻��̸�,
//	{
//		WINDOWMGR->MsgBox( MBI_QUESTQUICKVIEW_FULL, MBT_OK, "�˸��� ����� 5�� ������ �� �� �ֽ��ϴ�.") ;	// 5�� ������ ����� �����ϴٰ� �޽��� â�� ����.
//	}
//	else																			// Ʈ�� ������ 5�� �̸��̸�,
//	{
//		QuestTree* pTree = new QuestTree ;											// Ʈ���� �����Ѵ�.
//
//		CQuestInfo* pQuestInfo = NULL ;												// ����Ʈ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//		pQuestInfo = QUESTMGR->GetQuestInfo(dwQuestIdx) ;							// ����Ʈ �Ŵ����� ����, ���ڷ� �Ѿ�� ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.
//
//		if( !pQuestInfo )															// ����Ʈ ������ ��ȿ���� �ʴٸ�,
//		{
//			ASSERTMSG(0, "����Ʈ �˸��� ��� �� ����Ʈ ������ �޾ƿ��� ���߽��ϴ�.!") ;		// assert ���� ó���� �Ѵ�.
//			return ;																// ���� ó���� �Ѵ�.
//		}
//
//		int nSubQuestCount = 0 ;													// ���� ����Ʈ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
//		nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// ����Ʈ ������ ���� ���� ����Ʈ ī��Ʈ�� �޴´�.
//		
//		for(int count = 0 ; count < nSubQuestCount ; ++count )						// ���� ����Ʈ ī��Ʈ ��ŭ for���� ������.
//		{
//			if( count != 0 )
//			{
//				pDescSubQuestInfo = NULL ;													// ���� ����Ʈ ������ ���� �����͸� nulló�� �Ѵ�.
//				pDescSubQuestInfo = pQuest->GetSubQuestInfoArray(count) ;					// ī��Ʈ�� �ش��ϴ� ���� ����Ʈ�� �޴´�.
//
//				if( !pDescSubQuestInfo ) continue ;											// ���� ����Ʈ ������ ��ȿ���� ������ continue�Ѵ�.
//			}
//
//			pQuestString = NULL ;													// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//			pQuestString = QUESTMGR->GetQuestString(dwQuestIdx, count) ;			// ����Ʈ �ε�����, ���� ����Ʈ �ε����� ����Ʈ ��Ʈ���� �޴´�.
//            
//			if( pQuestString )														// ����Ʈ ��Ʈ�� ������ ��ȿ�ϴٸ�,
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
//				cPtrList* pList = pNewQuestString->GetTitle() ;						// ����Ʈ ��Ʈ���� Ÿ��Ʋ ����Ʈ�� �޴´�.
//
//				PTRLISTPOS titlePos = NULL ;										// Ÿ��Ʋ ����Ʈ ���� ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//				titlePos = pList->GetHeadPosition() ;								// Ÿ��Ʋ ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.
//
//				ITEM* pQString = NULL ;												// Q��Ʈ���� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//				while(titlePos)														// ��ġ ������ ��ȿ�� ���� while���� ������.
//				{
//					pQString = NULL ;												// Q��Ʈ�� �����͸� nulló���� �Ѵ�.
//					pQString = (ITEM*)pList->GetNext(titlePos) ;					// ��ġ�� ���� Q��Ʈ���� �޴´�.
//
//					if( pQString )													// Q��Ʈ�� ������ ��ȿ�ϴٸ�,
//					{
//						pNewQuestString->SetTitleStr( pQString->string) ;			// Q��Ʈ���� ���ڿ��� �޾Ƽ� ���ο� ����Ʈ ��Ʈ���� Ÿ��Ʋ�� �����Ѵ�.
//
//						break ;
//					}
//				}
//
//				pTree->list.AddTail(pNewQuestString) ;								// Ʈ���� ����Ʈ�� ����Ʈ ��Ʈ���� �߰��Ѵ�.
//
//			}
//		}
//
//		pTree->State = eQTree_Open ;												// Ʈ�� ���¸� ���� ���·� �����Ѵ�.
//		pTree->QuestIdx = dwQuestIdx ;												// Ʈ���� ����Ʈ �ε����� �����Ѵ�.
//		m_QuestTreeList.AddTail(pTree) ;											// ����Ʈ ����Ʈ�� Ʈ���� �߰��Ѵ�.
//
//		//QUESTMGR->RegistQuestIDToQuickView(m_QuestTreeList.GetCount()-1, dwQuestIdx) ;	// ����Ʈ �Ŵ�����, �˸��̿� ��� �� ����Ʈ ���̵� �����Ѵ�.
//	}
//
//	ResetQuickViewDialog() ;														// �� �並 ���ΰ�ħ�Ѵ�.
//}
//
////void CQuestQuickViewDialog::RegistQuest(DWORD dwQuestIdx)							// ����Ʈ�� �߰��ϴ� �Լ�.
////{
////    if( CheckSameQuest(dwQuestIdx) )												// ����Ʈ ����Ʈ�� ���ڷ� �Ѿ�� ����Ʈ�� ���� ����Ʈ�� �ִٸ�,
////	{
////		DeleteQuest(dwQuestIdx) ;													// ����Ʈ���� ����Ʈ�� �����Ѵ�.
////
////		return ;																	// ���� ó���� �Ѵ�.
////	}
////
////	int nTreeCount = 0 ;															// ����Ʈ Ʈ�� ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
////
////	nTreeCount = m_QuestTreeList.GetCount() ;										// ����Ʈ�� ��� ����Ʈ�� ī��Ʈ�� �޴´�.	
////
////	if( nTreeCount >= MAX_QUESTTREECOUNT )											// Ʈ�� ī��Ʈ�� �ִ� Ʈ�� �� �̻��̸�,
////	{
////		WINDOWMGR->MsgBox( MBI_QUESTQUICKVIEW_FULL, MBT_OK, "�˸��� ����� 5�� ������ �� �� �ֽ��ϴ�.") ;	// 5�� ������ ����� �����ϴٰ� �޽��� â�� ����.
////	}
////	else																			// Ʈ�� ������ 5�� �̸��̸�,
////	{
////		QuestTree* pTree = new QuestTree ;											// Ʈ���� �����Ѵ�.
////
////		CQuestInfo* pQuestInfo = NULL ;												// ����Ʈ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////		pQuestInfo = QUESTMGR->GetQuestInfo(dwQuestIdx) ;							// ����Ʈ �Ŵ����� ����, ���ڷ� �Ѿ�� ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.
////
////		if( !pQuestInfo )															// ����Ʈ ������ ��ȿ���� �ʴٸ�,
////		{
////			ASSERTMSG(0, "����Ʈ �˸��� ��� �� ����Ʈ ������ �޾ƿ��� ���߽��ϴ�.!") ;		// assert ���� ó���� �Ѵ�.
////			return ;																// ���� ó���� �Ѵ�.
////		}
////
////		int nSubQuestCount = 0 ;													// ���� ����Ʈ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
////		nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// ����Ʈ ������ ���� ���� ����Ʈ ī��Ʈ�� �޴´�.
////
////		CQuestString* pQuestString ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����Ѵ�.
////		
////		for(int count = 0 ; count < nSubQuestCount ; ++count )						// ���� ����Ʈ ī��Ʈ ��ŭ for���� ������.
////		{
////			pQuestString = NULL ;													// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
////			pQuestString = QUESTMGR->GetQuestString(dwQuestIdx, count) ;			// ����Ʈ �ε�����, ���� ����Ʈ �ε����� ����Ʈ ��Ʈ���� �޴´�.
////            
////			if( pQuestString )														// ����Ʈ ��Ʈ�� ������ ��ȿ�ϴٸ�,
////			{
////				CQuestString* pNewString = new CQuestString ;						// ����Ʈ ��Ʈ���� �����Ѵ�.
////				memcpy(pNewString, pQuestString, sizeof(CQuestString)) ;			// ���� ������ ����Ʈ ��Ʈ����, ������ ����Ʈ ��Ʈ���� �����Ѵ�.
////
////				cPtrList* pList = pQuestString->GetTitle() ;						// ����Ʈ ��Ʈ���� Ÿ��Ʋ ����Ʈ�� �޴´�.
////
////				PTRLISTPOS titlePos = NULL ;										// Ÿ��Ʋ ����Ʈ ���� ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////				titlePos = pList->GetHeadPosition() ;								// Ÿ��Ʋ ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.
////
////				QString* pQString = NULL ;											// Q��Ʈ���� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////
////				while(titlePos)														// ��ġ ������ ��ȿ�� ���� while���� ������.
////				{
////					pQString = NULL ;												// Q��Ʈ�� �����͸� nulló���� �Ѵ�.
////					pQString = (QString*)pList->GetNext(titlePos) ;					// ��ġ�� ���� Q��Ʈ���� �޴´�.
////
////					if( pQString )													// Q��Ʈ�� ������ ��ȿ�ϴٸ�,
////					{
////						pNewString->SetTitleStr( pQString->Str ) ;					// Q��Ʈ���� ���ڿ��� �޾Ƽ� ���ο� ����Ʈ ��Ʈ���� Ÿ��Ʋ�� �����Ѵ�.
////					}
////				}
////
////				pTree->list.AddTail(pNewString) ;									// Ʈ���� ����Ʈ�� ����Ʈ ��Ʈ���� �߰��Ѵ�.
////
////			}
////		}
////
////		pTree->State = eQTree_Open ;												// Ʈ�� ���¸� ���� ���·� �����Ѵ�.
////		pTree->QuestIdx = dwQuestIdx ;												// Ʈ���� ����Ʈ �ε����� �����Ѵ�.
////		m_QuestTreeList.AddTail(pTree) ;											// ����Ʈ ����Ʈ�� Ʈ���� �߰��Ѵ�.
////
////		QUESTMGR->RegistQuestIDToQuickView(m_QuestTreeList.GetCount()-1, dwQuestIdx) ;	// ����Ʈ �Ŵ�����, �˸��̿� ��� �� ����Ʈ ���̵� �����Ѵ�.
////	}
////
////	ResetQuickViewDialog() ;														// �� �並 ���ΰ�ħ�Ѵ�.
////}
//
//void CQuestQuickViewDialog::DeleteQuest(DWORD dwQuestIdx)							// ���ڷ� �Ѿ�� ����Ʈ�� Ʈ���� �����ϴ� �Լ�.
//{
//	QuestTree* pTree ;																// Ʈ�� ������ ���� �����͸� �����Ѵ�.
//	CQuestString* pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	PTRLISTPOS pos = NULL ;															// ����Ʈ ���� ��ġ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	PTRLISTPOS treePos = NULL ;														// Ʈ���� ����Ʈ ���� ��ġ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	pos = m_QuestTreeList.GetHeadPosition() ;										// ��ġ �����͸� ����Ʈ�� ���� �����Ѵ�.
//
//	while(pos)																		// ����Ʈ�� ���� ��Ұ� ��ȿ�� ���� while���� ������.
//	{
//		pTree = NULL ;																// Ʈ�� ������ ���� �����͸� null ó���� �Ѵ�.
//		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// ��ġ ������ �ش��ϴ� Ʈ���� �޴´�.
//
//		if( pTree )																	// Ʈ�� ������ ��ȿ�ϴٸ�,
//		{
//			if( pTree->QuestIdx == dwQuestIdx )										// Ʈ���� ����Ʈ �ε�����, ���ڷ� �Ѿ�� ����Ʈ �ε����� ������,
//			{
//				treePos = NULL ;													// Ʈ���� ����Ʈ���� ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.	
//				treePos = pTree->list.GetHeadPosition() ;							// Ʈ���� ����Ʈ�� ���� �����Ѵ�.
//
//				while(treePos)														// ��ġ ������ ��ȿ�� ���� while���� ������.
//				{
//					pQuestString = NULL ;											// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//					pQuestString = (CQuestString*)pTree->list.GetNext(treePos) ;	// 0��° ��ġ�� �ش��ϴ� ����Ʈ ��Ʈ���� �޴´�.
//
//					if( pQuestString )												// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
//					{
//						pTree->list.Remove(pQuestString) ;							// Ʈ���� ����Ʈ���� ����Ʈ ��Ʈ���� �����Ѵ�.
//
//						delete pQuestString ;
//						pQuestString = NULL ;
//					}
//				}
//
//				pTree->list.RemoveAll() ;											// ����Ʈ�� ��� ����.
//
//				m_QuestTreeList.Remove(pTree) ;										// ����Ʈ ����Ʈ���� Ʈ���� �����Ѵ�.
//
//				delete pTree ;
//				pTree = NULL ;
//			}
//		}
//	}
//
//	ResetQuickViewDialog() ;														// �� �並 ���ΰ�ħ�Ѵ�.
//}
//
////void CQuestQuickViewDialog::DeleteQuest(DWORD dwQuestIdx)							// ���ڷ� �Ѿ�� ����Ʈ�� Ʈ���� �����ϴ� �Լ�.
////{
////	QuestTree* pTree ;																// Ʈ�� ������ ���� �����͸� �����Ѵ�.
////	CQuestString* pQuestString ;													// ��Ʈ�� ������ ���� �����͸� �����Ѵ�.
////
////	PTRLISTPOS pos = NULL ;															// ����Ʈ ���� ��ġ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////	PTRLISTPOS treePos = NULL ;														// Ʈ���� ����Ʈ ���� ��ġ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////
////	pos = m_QuestTreeList.GetHeadPosition() ;										// ��ġ �����͸� ����Ʈ�� ���� �����Ѵ�.
////
////	while(pos)																		// ����Ʈ�� ���� ��Ұ� ��ȿ�� ���� while���� ������.
////	{
////		pTree = NULL ;																// Ʈ�� ������ ���� �����͸� null ó���� �Ѵ�.
////		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// ��ġ ������ �ش��ϴ� Ʈ���� �޴´�.
////
////		if( pTree )																	// Ʈ�� ������ ��ȿ�ϴٸ�,
////		{
////			if( pTree->QuestIdx == dwQuestIdx )										// Ʈ���� ����Ʈ �ε�����, ���ڷ� �Ѿ�� ����Ʈ �ε����� ������,
////			{
////				treePos = NULL ;													// Ʈ���� ����Ʈ���� ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.	
////				treePos = pTree->list.GetHeadPosition() ;							// Ʈ���� ����Ʈ�� ���� �����Ѵ�.
////
////				while(treePos)														// ��ġ ������ ��ȿ�� ���� while���� ������.
////				{
////					pQuestString = NULL ;											// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
////					pQuestString = (CQuestString*)pTree->list.GetNext(treePos) ;	// 0��° ��ġ�� �ش��ϴ� ����Ʈ ��Ʈ���� �޴´�.
////
////					if( pQuestString )												// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
////					{
////						pTree->list.Remove(pQuestString) ;							// Ʈ���� ����Ʈ���� ����Ʈ ��Ʈ���� �����Ѵ�.
////					}
////				}
////
////				pTree->list.RemoveAll() ;											// ����Ʈ�� ��� ����.
////
////				m_QuestTreeList.Remove(pTree) ;										// ����Ʈ ����Ʈ���� Ʈ���� �����Ѵ�.
////			}
////
////			pTree = NULL ;															// Ʈ�� �����͸� nulló���� �Ѵ�.
////		}
////	}
////
////	ResetQuickViewDialog() ;														// �� �並 ���ΰ�ħ�Ѵ�.
////}
//
//void CQuestQuickViewDialog::EndQuest(SEND_QUEST_IDX* msg)							// �˸��� â�� �ִ� ����Ʈ�� �����ϴ� �Լ�.
//{
//	WORD wQuestIdx = 0 ;															// ����Ʈ �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
//	wQuestIdx = msg->MainQuestIdx ;													// ���ڷ� �Ѿ�� �޽����� ���� ����Ʈ �ε����� �޴´�.
//
//	QuestTree* pTree = NULL ;														// Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	CQuestString* pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	PTRLISTPOS pos = NULL ;															// ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	pos = m_QuestTreeList.GetHeadPosition() ;										// ����Ʈ Ʈ�� ����Ʈ�� ���� �����Ѵ�.
//
//	while(pos)																		// ��ġ ������ ��ȿ�� ���� while���� ������.
//	{
//		pTree = NULL ;																// Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// ����Ʈ Ʈ�� ����Ʈ���� ����Ʈ Ʈ�� ������ �޴´�.
//
//		if( pTree )																	// Ʈ�� ������ ��ȿ�ϴٸ�,
//		{
//			if( (WORD)(pTree->QuestIdx) == wQuestIdx )								// Ʈ���� ����Ʈ �ε�����, ������Ʈ �� ����Ʈ �ε����� ���ٸ�,
//			{
//				cPtrList* pList = NULL ;											// ����Ʈ ������ ���� �����͸� �����ϰ� nulló���Ѵ�.
//				pList = &(pTree->list) ;											// Ʈ���� ����Ʈ ������ �޴´�.
//
//				if( pList )
//				{
//					PTRLISTPOS treePos = NULL ;										// Ʈ�� ����Ʈ�� ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//					treePos = pList->GetHeadPosition() ;							// Ʈ�� ����Ʈ�� ���� �����Ѵ�.
//
//					while(treePos)													// ��ġ ������ ��ȿ�� ���� while���� ������.
//					{	
//						pQuestString = NULL ;										// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//						pQuestString = (CQuestString*)pList->GetNext(treePos) ;		// ��ġ�� �ش��ϴ� ����Ʈ ��Ʈ���� �޴´�.
//
//						if( pQuestString )											// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
//						{
//							pList->Remove(pQuestString) ;							// ���� ����Ʈ ��Ʈ���� �����Ѵ�.
//
//							delete pQuestString ;
//							pQuestString = NULL ;
//						}
//					}
//
//					pList->RemoveAll() ;											// ����Ʈ�� �������� ��� ����.
//				}
//				else
//				{
//					ASSERTMSG(0, "����Ʈ �˸�â���� ����Ʈ�� ������Ʈ ���� ���߽��ϴ�.!") ;
//					return ;
//				}
//
//				m_QuestTreeList.Remove(pTree) ;										// ����Ʈ Ʈ�� ����Ʈ���� ���� Ʈ���� �����Ѵ�.
//
//				delete pTree ;
//				pTree = NULL ;
//			}
//		}
//	}
//
//	ResetQuickViewDialog() ;														// �� �並 ���ΰ�ħ�Ѵ�.
//}
//
////void CQuestQuickViewDialog::EndQuest(SEND_QUEST_IDX* msg)							// �˸��� â�� �ִ� ����Ʈ�� �����ϴ� �Լ�.
////{
////	WORD wQuestIdx = 0 ;															// ����Ʈ �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
////	wQuestIdx = msg->MainQuestIdx ;													// ���ڷ� �Ѿ�� �޽����� ���� ����Ʈ �ε����� �޴´�.
////
////	QuestTree* pTree = NULL ;														// Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////	CQuestString* pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////
////	PTRLISTPOS pos = NULL ;															// ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////	pos = m_QuestTreeList.GetHeadPosition() ;										// ����Ʈ Ʈ�� ����Ʈ�� ���� �����Ѵ�.
////
////	while(pos)																		// ��ġ ������ ��ȿ�� ���� while���� ������.
////	{
////		pTree = NULL ;																// Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
////		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// ����Ʈ Ʈ�� ����Ʈ���� ����Ʈ Ʈ�� ������ �޴´�.
////
////		if( pTree )																	// Ʈ�� ������ ��ȿ�ϴٸ�,
////		{
////			if( (WORD)(pTree->QuestIdx) == wQuestIdx )								// Ʈ���� ����Ʈ �ε�����, ������Ʈ �� ����Ʈ �ε����� ���ٸ�,
////			{
////				cPtrList* pList = NULL ;											// ����Ʈ ������ ���� �����͸� �����ϰ� nulló���Ѵ�.
////				pList = &(pTree->list) ;											// Ʈ���� ����Ʈ ������ �޴´�.
////
////				if( pList )
////				{
////					PTRLISTPOS treePos = NULL ;										// Ʈ�� ����Ʈ�� ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////					treePos = pList->GetHeadPosition() ;							// Ʈ�� ����Ʈ�� ���� �����Ѵ�.
////
////					while(treePos)													// ��ġ ������ ��ȿ�� ���� while���� ������.
////					{	
////						pQuestString = NULL ;										// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
////						pQuestString = (CQuestString*)pList->GetNext(treePos) ;		// ��ġ�� �ش��ϴ� ����Ʈ ��Ʈ���� �޴´�.
////
////						if( pQuestString )											// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
////						{
////							pList->Remove(pQuestString) ;							// ���� ����Ʈ ��Ʈ���� �����Ѵ�.
////						}
////					}
////
////					pList->RemoveAll() ;											// ����Ʈ�� �������� ��� ����.
////				}
////				else
////				{
////					ASSERTMSG(0, "����Ʈ �˸�â���� ����Ʈ�� ������Ʈ ���� ���߽��ϴ�.!") ;
////					return ;
////				}
////
////				m_QuestTreeList.Remove(pTree) ;										// ����Ʈ Ʈ�� ����Ʈ���� ���� Ʈ���� �����Ѵ�.
////			}
////		}
////	}
////
////	ResetQuickViewDialog() ;														// �� �並 ���ΰ�ħ�Ѵ�.
////}
//
//void CQuestQuickViewDialog::StartSubQuest(SEND_QUEST_IDX* pmsg)						// �˸��� â�� ��� �� ���� ����Ʈ�� �����ϴ� �Լ�.
//{
//	if( !pmsg ) return ;															// ���ڷ� �Ѿ�� �޽����� ��ȿ���� ������, return ó�� �Ѵ�.
//
//	CQuestString* pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	WORD wQuestIdx = 0 ;															// ����Ʈ �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
//	wQuestIdx = pmsg->MainQuestIdx ;												// ���ڷ� �Ѿ�� �޽����� ���� ����Ʈ �ε����� �޴´�.
//
//	WORD wSubQuestIdx = 0 ;															// ���� ����Ʈ �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
//	wSubQuestIdx = pmsg->SubQuestIdx ;												// ���ڷ� �Ѿ�� �޽����� ���� ���� ����Ʈ �ε����� �޴´�.
//
//	QuestTree* pTree = NULL ;														// Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	PTRLISTPOS pos = NULL ;															// ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	pos = m_QuestTreeList.GetHeadPosition() ;										// ����Ʈ Ʈ�� ����Ʈ�� ���� �����Ѵ�.
//
//	while(pos)																		// ��ġ ������ ��ȿ�� ���� while���� ������.
//	{
//		pTree = NULL ;																// Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// ����Ʈ Ʈ�� ����Ʈ���� ����Ʈ Ʈ�� ������ �޴´�.
//
//		if( pTree )																	// Ʈ�� ������ ��ȿ�ϴٸ�,
//		{
//			if( (WORD)(pTree->QuestIdx) == wQuestIdx )								// Ʈ���� ����Ʈ �ε�����, ������Ʈ �� ����Ʈ �ε����� ���ٸ�,
//			{
//				cPtrList* pList = NULL ;											// ����Ʈ ������ ���� �����͸� �����ϰ� nulló���Ѵ�.
//				pList = &(pTree->list) ;											// Ʈ���� ����Ʈ ������ �޴´�.
//
//				if( pList )
//				{
//					CQuestInfo* pQuestInfo = NULL ;									// ����Ʈ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//					pQuestInfo = QUESTMGR->GetQuestInfo((DWORD)wQuestIdx) ;			// ����Ʈ �Ŵ����� ����, ���ڷ� �Ѿ�� ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.
//
//					if( !pQuestInfo )												// ����Ʈ ������ ��ȿ���� �ʴٸ�,
//					{
//						ASSERTMSG(0, "����Ʈ �˸��� ��� �� ����Ʈ ������ �޾ƿ��� ���߽��ϴ�.!") ;		// assert ���� ó���� �Ѵ�.
//						return ;													// ���� ó���� �Ѵ�.
//					}
//
//					int nSubQuestCount = 0 ;										// ���� ����Ʈ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
//					nSubQuestCount = pQuestInfo->GetSubQuestCount() ;				// ����Ʈ ������ ���� ���� ����Ʈ ī��Ʈ�� �޴´�.
//
//					if( wSubQuestIdx == nSubQuestCount-1 )
//					{
//						PTRLISTPOS treePos = NULL ;										// Ʈ�� ����Ʈ�� ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//						treePos = pList->GetHeadPosition() ;							// Ʈ�� ����Ʈ�� ���� �����Ѵ�.
//
//						//int count = 0 ;
//
//						pQuestString = NULL ;													// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//						pQuestString = QUESTMGR->GetQuestString(wQuestIdx, wSubQuestIdx) ;			// ����Ʈ �ε�����, ���� ����Ʈ �ε����� ����Ʈ ��Ʈ���� �޴´�.
//			            
//						if( !pQuestString )	continue ;											// ����Ʈ ��Ʈ�� ������ ��ȿ���� ������ continue�Ѵ�.
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
//						cPtrList* pNewTitleList = pNewQuestString->GetTitle() ;						// ����Ʈ ��Ʈ���� Ÿ��Ʋ ����Ʈ�� �޴´�.
//
//						PTRLISTPOS newTitlePos = NULL ;										// Ÿ��Ʋ ����Ʈ ���� ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//						newTitlePos = pNewTitleList->GetHeadPosition() ;								// Ÿ��Ʋ ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.
//
//						ITEM* pNewQString = NULL ;												// Q��Ʈ���� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//						while(newTitlePos)														// ��ġ ������ ��ȿ�� ���� while���� ������.
//						{
//							pNewQString = NULL ;												// Q��Ʈ�� �����͸� nulló���� �Ѵ�.
//							pNewQString = (ITEM*)pNewTitleList->GetNext(newTitlePos) ;					// ��ġ�� ���� Q��Ʈ���� �޴´�.
//
//							if( pNewQString )													// Q��Ʈ�� ������ ��ȿ�ϴٸ�,
//							{
//								pNewQuestString->SetTitleStr( pNewQString->string) ;			// Q��Ʈ���� ���ڿ��� �޾Ƽ� ���ο� ����Ʈ ��Ʈ���� Ÿ��Ʋ�� �����Ѵ�.
//
//								break ;
//							}
//						}
//
//						pTree->list.AddTail(pNewQuestString) ;								// Ʈ���� ����Ʈ�� ����Ʈ ��Ʈ���� �߰��Ѵ�.
//						
//						
//						
//						//while(treePos)													// ��ġ ������ ��ȿ�� ���� while���� ������.
//						//{	
//						//	if( count != 0 )
//						//	{
//						//		pDescSubQuestInfo = NULL ;													// ���� ����Ʈ ������ ���� �����͸� nulló�� �Ѵ�.
//						//		pDescSubQuestInfo = pQuest->GetSubQuestInfoArray(count) ;					// ī��Ʈ�� �ش��ϴ� ���� ����Ʈ�� �޴´�.
//
//						//		if( !pDescSubQuestInfo ) 
//						//		{
//						//			++count ;
//						//			continue ;											// ���� ����Ʈ ������ ��ȿ���� ������ continue�Ѵ�.
//						//		}
//						//	}
//
//						//	++count ;
//
//						//	pQuestString = NULL ;										// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//						//	pQuestString = (CQuestString*)pList->GetNext(treePos) ;		// ��ġ�� �ش��ϴ� ����Ʈ ��Ʈ���� �޴´�.
//
//						//	if( pQuestString )											// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
//						//	{
//						//		WORD wSubQuestId = (WORD)(pQuestString->GetSubID()) ;
//
//						//		if( wSubQuestId != nSubQuestCount-1 && wSubQuestId != 0 )// �����̰� ������ ��Ʈ���� �ƴϸ�,
//						//		{
//						//			pList->Remove(pQuestString) ;						// ����Ʈ���� ����Ʈ ��Ʈ���� ����.
//
//						//			delete pQuestString ;
//						//			pQuestString  = NULL ;
//						//		}
//						//		else													// �����̳� ������ ��Ʈ���̸�,
//						//		{
//						//			char tempBuf[256] = {0, } ;
//
//						//			strcpy(tempBuf, pQuestString->GetTitleStr()) ;
//						//			if(pQuestString->GetSubID() != 0 )
//						//			{
//						//				//strcat(tempBuf, "(�Ϸ�)") ;
//
//						//				pQuestString->SetComplete(TRUE) ;
//						//			}
//
//						//			pQuestString->SetTitleStr(tempBuf) ;
//						//		}
//						//	}
//						//	else
//						//	{
//						//		pQuestString = NULL ;													// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//						//		pQuestString = QUESTMGR->GetQuestString(wQuestIdx, wSubQuestIdx) ;			// ����Ʈ �ε�����, ���� ����Ʈ �ε����� ����Ʈ ��Ʈ���� �޴´�.
//					 //           
//						//		if( pQuestString )														// ����Ʈ ��Ʈ�� ������ ��ȿ�ϴٸ�,
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
//						//			cPtrList* pNewTitleList = pNewQuestString->GetTitle() ;						// ����Ʈ ��Ʈ���� Ÿ��Ʋ ����Ʈ�� �޴´�.
//
//						//			PTRLISTPOS newTitlePos = NULL ;										// Ÿ��Ʋ ����Ʈ ���� ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//						//			newTitlePos = pNewTitleList->GetHeadPosition() ;								// Ÿ��Ʋ ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.
//
//						//			ITEM* pNewQString = NULL ;												// Q��Ʈ���� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//						//			while(newTitlePos)														// ��ġ ������ ��ȿ�� ���� while���� ������.
//						//			{
//						//				pNewQString = NULL ;												// Q��Ʈ�� �����͸� nulló���� �Ѵ�.
//						//				pNewQString = (ITEM*)pNewTitleList->GetNext(newTitlePos) ;					// ��ġ�� ���� Q��Ʈ���� �޴´�.
//
//						//				if( pNewQString )													// Q��Ʈ�� ������ ��ȿ�ϴٸ�,
//						//				{
//						//					pNewQuestString->SetTitleStr( pNewQString->string) ;			// Q��Ʈ���� ���ڿ��� �޾Ƽ� ���ο� ����Ʈ ��Ʈ���� Ÿ��Ʋ�� �����Ѵ�.
//
//						//					break ;
//						//				}
//						//			}
//
//						//			pTree->list.AddTail(pNewQuestString) ;								// Ʈ���� ����Ʈ�� ����Ʈ ��Ʈ���� �߰��Ѵ�.
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
//							pQuestString = NULL ;													// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//							pQuestString = QUESTMGR->GetQuestString(wQuestIdx, wSubQuestIdx) ;			// ����Ʈ �ε�����, ���� ����Ʈ �ε����� ����Ʈ ��Ʈ���� �޴´�.
//				            
//							if( !pQuestString )	continue ;											// ����Ʈ ��Ʈ�� ������ ��ȿ���� ������ continue�Ѵ�.
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
//										strcat(pNewItem->string, "(�Ϸ�)") ;
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
//							cPtrList* pNewTitleList = pNewQuestString->GetTitle() ;						// ����Ʈ ��Ʈ���� Ÿ��Ʋ ����Ʈ�� �޴´�.
//
//							PTRLISTPOS newTitlePos = NULL ;										// Ÿ��Ʋ ����Ʈ ���� ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//							newTitlePos = pNewTitleList->GetHeadPosition() ;								// Ÿ��Ʋ ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.
//
//							ITEM* pNewQString = NULL ;												// Q��Ʈ���� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//							while(newTitlePos)														// ��ġ ������ ��ȿ�� ���� while���� ������.
//							{
//								pNewQString = NULL ;												// Q��Ʈ�� �����͸� nulló���� �Ѵ�.
//								pNewQString = (ITEM*)pNewTitleList->GetNext(newTitlePos) ;					// ��ġ�� ���� Q��Ʈ���� �޴´�.
//
//								if( pNewQString )													// Q��Ʈ�� ������ ��ȿ�ϴٸ�,
//								{
//									pNewQuestString->SetTitleStr( pNewQString->string) ;			// Q��Ʈ���� ���ڿ��� �޾Ƽ� ���ο� ����Ʈ ��Ʈ���� Ÿ��Ʋ�� �����Ѵ�.
//
//									break ;
//								}
//							}
//
//							pTree->list.AddTail(pNewQuestString) ;								// Ʈ���� ����Ʈ�� ����Ʈ ��Ʈ���� �߰��Ѵ�.
//						}
//					}
//				}
//				else
//				{
//					ASSERTMSG(0, "����Ʈ �˸�â���� ����Ʈ�� ������Ʈ ���� ���߽��ϴ�.!") ;
//					return ;
//				}
//			}
//		}
//	}
//
//	ResetQuickViewDialog() ;														// �� �並 ���ΰ�ħ�Ѵ�.
//}
//
////void CQuestQuickViewDialog::StartSubQuest(SEND_QUEST_IDX* pmsg)						// �˸��� â�� ��� �� ���� ����Ʈ�� �����ϴ� �Լ�.
////{
////	WORD wQuestIdx = 0 ;															// ����Ʈ �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
////	wQuestIdx = pmsg->MainQuestIdx ;												// ���ڷ� �Ѿ�� �޽����� ���� ����Ʈ �ε����� �޴´�.
////
////	WORD wSubQuestIdx = 0 ;															// ���� ����Ʈ �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
////	wSubQuestIdx = pmsg->SubQuestIdx ;												// ���ڷ� �Ѿ�� �޽����� ���� ���� ����Ʈ �ε����� �޴´�.
////
////	QuestTree* pTree = NULL ;														// Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////	CQuestString* pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////
////	PTRLISTPOS pos = NULL ;															// ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////	pos = m_QuestTreeList.GetHeadPosition() ;										// ����Ʈ Ʈ�� ����Ʈ�� ���� �����Ѵ�.
////
////	while(pos)																		// ��ġ ������ ��ȿ�� ���� while���� ������.
////	{
////		pTree = NULL ;																// Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
////		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// ����Ʈ Ʈ�� ����Ʈ���� ����Ʈ Ʈ�� ������ �޴´�.
////
////		if( pTree )																	// Ʈ�� ������ ��ȿ�ϴٸ�,
////		{
////			if( (WORD)(pTree->QuestIdx) == wQuestIdx )								// Ʈ���� ����Ʈ �ε�����, ������Ʈ �� ����Ʈ �ε����� ���ٸ�,
////			{
////				cPtrList* pList = NULL ;											// ����Ʈ ������ ���� �����͸� �����ϰ� nulló���Ѵ�.
////				pList = &(pTree->list) ;											// Ʈ���� ����Ʈ ������ �޴´�.
////
////				if( pList )
////				{
////					CQuestInfo* pQuestInfo = NULL ;									// ����Ʈ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////					pQuestInfo = QUESTMGR->GetQuestInfo((DWORD)wQuestIdx) ;			// ����Ʈ �Ŵ����� ����, ���ڷ� �Ѿ�� ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.
////
////					if( !pQuestInfo )												// ����Ʈ ������ ��ȿ���� �ʴٸ�,
////					{
////						ASSERTMSG(0, "����Ʈ �˸��� ��� �� ����Ʈ ������ �޾ƿ��� ���߽��ϴ�.!") ;		// assert ���� ó���� �Ѵ�.
////						return ;													// ���� ó���� �Ѵ�.
////					}
////
////					int nSubQuestCount = 0 ;										// ���� ����Ʈ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
////					nSubQuestCount = pQuestInfo->GetSubQuestCount() ;				// ����Ʈ ������ ���� ���� ����Ʈ ī��Ʈ�� �޴´�.
////
////					if( wSubQuestIdx == nSubQuestCount-1 )
////					{
////						PTRLISTPOS treePos = NULL ;										// Ʈ�� ����Ʈ�� ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////						treePos = pList->GetHeadPosition() ;							// Ʈ�� ����Ʈ�� ���� �����Ѵ�.
////
////						while(treePos)													// ��ġ ������ ��ȿ�� ���� while���� ������.
////						{	
////							pQuestString = NULL ;										// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
////							pQuestString = (CQuestString*)pList->GetNext(treePos) ;		// ��ġ�� �ش��ϴ� ����Ʈ ��Ʈ���� �޴´�.
////
////							if( pQuestString )											// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
////							{
////								WORD wSubQuestId = (WORD)(pQuestString->GetSubID()) ;
////
////								if( wSubQuestId != nSubQuestCount-1 && wSubQuestId != 0 )// �����̰� ������ ��Ʈ���� �ƴϸ�,
////								{
////									pList->Remove(pQuestString) ;						// ����Ʈ���� ����Ʈ ��Ʈ���� ����.
////								}
////								else													// �����̳� ������ ��Ʈ���̸�,
////								{
////									//if( wSubQuestId != 0 )								// ù ��Ʈ���� �ƴϸ�,
////									//{
////									//	pList->AddTail(pQuestString) ;					// ����Ʈ�� �߰��Ѵ�.
////									//}
////
////									char tempBuf[256] = {0, } ;
////
////									strcpy(tempBuf, pQuestString->GetTitleStr()) ;
////									if(pQuestString->GetSubID() != 0 )
////									{
////										//strcat(tempBuf, "(�Ϸ�)") ;
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
////					ASSERTMSG(0, "����Ʈ �˸�â���� ����Ʈ�� ������Ʈ ���� ���߽��ϴ�.!") ;
////					return ;
////				}
////			}
////		}
////	}
////
////	ResetQuickViewDialog() ;														// �� �並 ���ΰ�ħ�Ѵ�.
////}
//
//void CQuestQuickViewDialog::EndSubQuest(SEND_QUEST_IDX* msg)						// �˸��� â�� ��� �� ���� ����Ʈ�� end�ϴ� �Լ�.
//{
//	if( !msg ) return ;																// ���ڷ� �Ѿ�� msg�� ��ȿ���� ������ return ó�� �Ѵ�.
//
//	CQuestString* pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	//CQuest* pQuest = QUESTMGR->GetQuest( msg->MainQuestIdx );							// ����Ʈ ������ �޴´�.
//
//	//if( !pQuest ) return ;															// ����Ʈ ������ ��ȿ���� ������, return ó�� �Ѵ�.
//
//	//CSubQuestInfo* pDescSubQuestInfo ;												// ���� ����Ʈ ������ ���� �����͸� �����Ѵ�.
//
//	WORD wQuestIdx = 0 ;															// ����Ʈ �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
//	wQuestIdx = msg->MainQuestIdx ;													// ���ڷ� �Ѿ�� �޽����� ���� ����Ʈ �ε����� �޴´�.
//
//	WORD wSubQuestIdx = 0 ;															// ���� ����Ʈ �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
//	wSubQuestIdx = msg->SubQuestIdx ;												// ���ڷ� �Ѿ�� �޽����� ���� ���� ����Ʈ �ε����� �޴´�.
//
//	QuestTree* pTree = NULL ;														// Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	PTRLISTPOS pos = NULL ;															// ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	pos = m_QuestTreeList.GetHeadPosition() ;										// ����Ʈ Ʈ�� ����Ʈ�� ���� �����Ѵ�.
//
//	while(pos)																		// ��ġ ������ ��ȿ�� ���� while���� ������.
//	{
//		pTree = NULL ;																// Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// ����Ʈ Ʈ�� ����Ʈ���� ����Ʈ Ʈ�� ������ �޴´�.
//
//		if( pTree )																	// Ʈ�� ������ ��ȿ�ϴٸ�,
//		{
//			if( (WORD)(pTree->QuestIdx) == wQuestIdx )								// Ʈ���� ����Ʈ �ε�����, ������Ʈ �� ����Ʈ �ε����� ���ٸ�,
//			{
//				cPtrList* pList = NULL ;											// ����Ʈ ������ ���� �����͸� �����ϰ� nulló���Ѵ�.
//				pList = &(pTree->list) ;											// Ʈ���� ����Ʈ ������ �޴´�.
//
//				if( pList )
//				{
//					PTRLISTPOS treePos = NULL ;										// Ʈ�� ����Ʈ�� ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//					treePos = pList->GetHeadPosition() ;							// Ʈ�� ����Ʈ�� ���� �����Ѵ�.
//
//					//int count = 0 ;
//
//					while(treePos)													// ��ġ ������ ��ȿ�� ���� while���� ������.
//					{	
//						//if( count != 0 )
//						//{
//						//	pDescSubQuestInfo = NULL ;													// ���� ����Ʈ ������ ���� �����͸� nulló�� �Ѵ�.
//						//	pDescSubQuestInfo = pQuest->GetSubQuestInfoArray(count) ;					// ī��Ʈ�� �ش��ϴ� ���� ����Ʈ�� �޴´�.
//
//						//	if( !pDescSubQuestInfo ) 
//						//	{
//						//		++count ;
//						//		continue ;											// ���� ����Ʈ ������ ��ȿ���� ������ continue�Ѵ�.
//						//	}
//						//}
//
//						//++count ;
//
//						pQuestString = NULL ;										// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//						pQuestString = (CQuestString*)pList->GetNext(treePos) ;		// ��ġ�� �ش��ϴ� ����Ʈ ��Ʈ���� �޴´�.
//
//						if( pQuestString )											// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
//						{
//							if( (WORD)(pQuestString->GetSubID()) == wSubQuestIdx )	// ��Ʈ���� ���� �� �ε�����, ���ڷ� �Ѿ�� ���� �� �ε����� ������,
//							{
//								pTree->list.Remove(pQuestString) ;
//
//								delete pQuestString ;
//								pQuestString = NULL ;
//
//
//								//char tempBuf[1024] = {0, } ;						// �ӽ� ���۸� �����Ѵ�.
//
//								//cPtrList* pTitleList = NULL ;						// ������Ʈ ��Ʈ���� ����Ʈ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//								//pTitleList = pQuestString->GetTitle() ;				// ������Ʈ ��Ʈ���� ����Ʈ�� �޴´�.
//
//								//if( pTitleList )									// ����Ʈ ������ ��ȿ�ϸ�,
//								//{
//								//	ITEM* pQString = NULL ;						// ����Ʈ ��Ʈ���� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//								//	PTRLISTPOS titlePos = NULL ;					// ������Ʈ �� ��Ʈ���� ����Ʈ ���� ��ġ�� ���� �����͸� �����ϰ� nulló�� �Ѵ�.
//								//	titlePos = pTitleList->GetHeadPosition() ;		// ������Ʈ �� ��Ʈ���� ����Ʈ�� ���� �����Ѵ�.
//	
//								//	while(titlePos)									// ��ġ ������ ��ȿ�� ���� while���� ������.
//								//	{
//								//		pQString = NULL ;							// �����͸� nulló���� �Ѵ�.
//								//		pQString = (ITEM*)pTitleList->GetNext(titlePos) ; // ��ġ�� ���� ��Ʈ�� ������ �޴´�.
//
//								//		if( pQString )								// ��Ʈ�� ������ ��ȿ�ϸ�,
//								//		{
//								//			strcat(tempBuf, pQString->string) ;		// �ӽù��ۿ� ��Ʈ���� ��´�.
//
//								//			break ;
//								//		}
//								//	}
//
//								//	pQuestString->SetComplete(TRUE) ;
//
//								//	pQuestString->SetTitleStr(tempBuf) ;			// ��Ʈ���� Ÿ��Ʋ�� �����Ѵ�.
//								//}
//							}
//						}
//					}
//				}
//				else
//				{
//					ASSERTMSG(0, "����Ʈ �˸�â���� ����Ʈ�� ������Ʈ ���� ���߽��ϴ�.!") ;
//					return ;
//				}
//			}
//		}
//	}
//
//	ResetQuickViewDialog() ;														// �� �並 ���ΰ�ħ�Ѵ�.
//}
//
////void CQuestQuickViewDialog::EndSubQuest(SEND_QUEST_IDX* msg)						// �˸��� â�� ��� �� ���� ����Ʈ�� end�ϴ� �Լ�.
////{
////	WORD wQuestIdx = 0 ;															// ����Ʈ �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
////	wQuestIdx = msg->MainQuestIdx ;													// ���ڷ� �Ѿ�� �޽����� ���� ����Ʈ �ε����� �޴´�.
////
////	WORD wSubQuestIdx = 0 ;															// ���� ����Ʈ �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
////	wSubQuestIdx = msg->SubQuestIdx ;												// ���ڷ� �Ѿ�� �޽����� ���� ���� ����Ʈ �ε����� �޴´�.
////
////	QuestTree* pTree = NULL ;														// Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////	CQuestString* pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////
////	PTRLISTPOS pos = NULL ;															// ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////	pos = m_QuestTreeList.GetHeadPosition() ;										// ����Ʈ Ʈ�� ����Ʈ�� ���� �����Ѵ�.
////
////	while(pos)																		// ��ġ ������ ��ȿ�� ���� while���� ������.
////	{
////		pTree = NULL ;																// Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
////		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// ����Ʈ Ʈ�� ����Ʈ���� ����Ʈ Ʈ�� ������ �޴´�.
////
////		if( pTree )																	// Ʈ�� ������ ��ȿ�ϴٸ�,
////		{
////			if( (WORD)(pTree->QuestIdx) == wQuestIdx )								// Ʈ���� ����Ʈ �ε�����, ������Ʈ �� ����Ʈ �ε����� ���ٸ�,
////			{
////				cPtrList* pList = NULL ;											// ����Ʈ ������ ���� �����͸� �����ϰ� nulló���Ѵ�.
////				pList = &(pTree->list) ;											// Ʈ���� ����Ʈ ������ �޴´�.
////
////				if( pList )
////				{
////					PTRLISTPOS treePos = NULL ;										// Ʈ�� ����Ʈ�� ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////					treePos = pList->GetHeadPosition() ;							// Ʈ�� ����Ʈ�� ���� �����Ѵ�.
////
////					while(treePos)													// ��ġ ������ ��ȿ�� ���� while���� ������.
////					{	
////						pQuestString = NULL ;										// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
////						pQuestString = (CQuestString*)pList->GetNext(treePos) ;		// ��ġ�� �ش��ϴ� ����Ʈ ��Ʈ���� �޴´�.
////
////						if( pQuestString )											// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
////						{
////							if( (WORD)(pQuestString->GetSubID()) == wSubQuestIdx )	// ��Ʈ���� ���� �� �ε�����, ���ڷ� �Ѿ�� ���� �� �ε����� ������,
////							{
////								char tempBuf[1024] = {0, } ;						// �ӽ� ���۸� �����Ѵ�.
////
////								cPtrList* pTitleList = NULL ;						// ������Ʈ ��Ʈ���� ����Ʈ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////								pTitleList = pQuestString->GetTitle() ;				// ������Ʈ ��Ʈ���� ����Ʈ�� �޴´�.
////
////								if( pTitleList )									// ����Ʈ ������ ��ȿ�ϸ�,
////								{
////									QString* pQString = NULL ;						// ����Ʈ ��Ʈ���� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////
////									PTRLISTPOS titlePos = NULL ;					// ������Ʈ �� ��Ʈ���� ����Ʈ ���� ��ġ�� ���� �����͸� �����ϰ� nulló�� �Ѵ�.
////									titlePos = pTitleList->GetHeadPosition() ;		// ������Ʈ �� ��Ʈ���� ����Ʈ�� ���� �����Ѵ�.
////
////									//sGAMENOTIFY_MSG* pNoticeMsg ;						// ���� ���� �޽��� ����ü�� �����Ѵ�.
////									//pNoticeMsg = new sGAMENOTIFY_MSG ;
////									//pNoticeMsg->nNotifyType = eSTYLE_QUEST ;			// ���� Ÿ���� ����Ʈ�� �����Ѵ�.
////
////									//pNoticeMsg->rect.left = 500 ;
////									//pNoticeMsg->rect.top = 500 ;
////									//pNoticeMsg->rect.right = 1 ;
////									//pNoticeMsg->rect.bottom = 500 ;
////	
////									while(titlePos)									// ��ġ ������ ��ȿ�� ���� while���� ������.
////									{
////										pQString = NULL ;							// �����͸� nulló���� �Ѵ�.
////										pQString = (QString*)pTitleList->GetNext(titlePos) ; // ��ġ�� ���� ��Ʈ�� ������ �޴´�.
////
////										if( pQString )								// ��Ʈ�� ������ ��ȿ�ϸ�,
////										{
////											strcat(tempBuf, pQString->Str) ;		// �ӽù��ۿ� ��Ʈ���� ��´�.
////
////											//ITEM* pNoticeItem = new ITEM ;			// ���� ����Ʈ �������� ���� �������� �����Ѵ�.
////
////											//pNoticeItem->rgb = dwQuestStringColor[eQC_SUBQUEST_NAME] ;	// ���� ����Ʈ ��Ʈ���� ������ �����Ѵ�.
////
////											///*pNoticeMsg->red = 0 ;
////											//pNoticeMsg->gree = 255 ;
////											//pNoticeMsg->blue =  0  ;
////											//pNoticeMsg->nAlpha = 255 ;*/
////
////											//strcpy(pNoticeItem->string, tempBuf) ;	// ���� ����Ʈ ��Ʈ���� �����Ѵ�.
////
////											//pNoticeMsg->itemList.AddTail(pNoticeItem);// ���� �޽��� ����ü�� ����Ʈ�� ���� ����Ʈ �������� �߰��Ѵ�.
////										}
////									}
////
////									//char tempBuf2[128] = {0, } ;					// �ι�° �ӽ� ���۸� �����Ѵ�.
////
////									//sprintf(tempBuf2, "(�Ϸ�)") ;					// �ӽ� ���ۿ� ���� ������ ǥ���Ѵ�.
////
////									//strcat(tempBuf, tempBuf2) ;						// ù ��° �ӽ� ���ۿ� �� ��° �ӽ� ���۸� �߰��Ѵ�.
////
////									pQuestString->SetComplete(TRUE) ;
////
////									pQuestString->SetTitleStr(tempBuf) ;			// ��Ʈ���� Ÿ��Ʋ�� �����Ѵ�.
////
////									//ITEM* pCountItem = new ITEM ;					// ī��Ʈ�� ���� �������� �����Ѵ�.
////
////									//strcpy(pCountItem->string, tempBuf2) ;			// ī��Ʈ �����ۿ� ī��Ʈ ��Ʈ���� �����Ѵ�.
////
////									//pCountItem->rgb = dwQuestStringColor[eQC_COUNT];// ī��Ʈ ��Ʈ���� ������ �����Ѵ�.
////
////									///*pNoticeMsg->red = 0 ;
////									//pNoticeMsg->gree = 0 ;
////									//pNoticeMsg->blue = 255  ;
////									//pNoticeMsg->nAlpha = 255 ;*/
////
////									//pNoticeMsg->itemList.AddTail(pCountItem) ;		// ���� �޽��� ����ü�� ����Ʈ�� ī��Ʈ �������� �߰��Ѵ�.
////									//
////
////									//GAMENOTIFYMGR->AddMsg(pNoticeMsg) ;				// ���� ���� �Ŵ����� ���� �޽����� �߰��Ѵ�.
////								}
////							}
////						}
////					}
////				}
////				else
////				{
////					ASSERTMSG(0, "����Ʈ �˸�â���� ����Ʈ�� ������Ʈ ���� ���߽��ϴ�.!") ;
////					return ;
////				}
////			}
////		}
////	}
////
////	ResetQuickViewDialog() ;														// �� �並 ���ΰ�ħ�Ѵ�.
////}
//
//void CQuestQuickViewDialog::UpdateQuest(SEND_SUBQUEST_UPDATE* msg)					// �˸��� â�� ��� �� ����Ʈ�� ������Ʈ �ϴ� �Լ�.
//{
//	CQuestString* pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	CQuest* pQuest = QUESTMGR->GetQuest( msg->wQuestIdx );							// ����Ʈ ������ �޴´�.
//
//	if( !pQuest ) return ;															// ����Ʈ ������ ��ȿ���� ������, return ó�� �Ѵ�.
//
//	CSubQuestInfo* pDescSubQuestInfo ;												// ���� ����Ʈ ������ ���� �����͸� �����Ѵ�.
//
//	WORD wQuestIdx = 0 ;															// ����Ʈ �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
//	wQuestIdx = msg->wQuestIdx ;													// ���ڷ� �Ѿ�� �޽����� ���� ����Ʈ �ε����� �޴´�.
//
//	WORD wSubQuestIdx = 0 ;															// ���� ����Ʈ �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
//	wSubQuestIdx = msg->wSubQuestIdx ;												// ���ڷ� �Ѿ�� �޽����� ���� ���� ����Ʈ �ε����� �޴´�.
//
//	QuestTree* pTree = NULL ;														// Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	PTRLISTPOS pos = NULL ;															// ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	pos = m_QuestTreeList.GetHeadPosition() ;										// ����Ʈ Ʈ�� ����Ʈ�� ���� �����Ѵ�.
//
//	while(pos)																		// ��ġ ������ ��ȿ�� ���� while���� ������.
//	{
//		pTree = NULL ;																// Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// ����Ʈ Ʈ�� ����Ʈ���� ����Ʈ Ʈ�� ������ �޴´�.
//
//		if( pTree )																	// Ʈ�� ������ ��ȿ�ϴٸ�,
//		{
//			if( (WORD)(pTree->QuestIdx) == wQuestIdx )								// Ʈ���� ����Ʈ �ε�����, ������Ʈ �� ����Ʈ �ε����� ���ٸ�,
//			{
//				cPtrList* pList = NULL ;											// ����Ʈ ������ ���� �����͸� �����ϰ� nulló���Ѵ�.
//				pList = &(pTree->list) ;											// Ʈ���� ����Ʈ ������ �޴´�.
//
//				if( pList )
//				{
//					PTRLISTPOS treePos = NULL ;										// Ʈ�� ����Ʈ�� ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//					treePos = pList->GetHeadPosition() ;							// Ʈ�� ����Ʈ�� ���� �����Ѵ�.
//
//					int count = 0 ;
//
//					while(treePos)													// ��ġ ������ ��ȿ�� ���� while���� ������.
//					{	
//						if( count != 0 )
//						{
//							pDescSubQuestInfo = NULL ;													// ���� ����Ʈ ������ ���� �����͸� nulló�� �Ѵ�.
//							pDescSubQuestInfo = pQuest->GetSubQuestInfoArray(count) ;					// ī��Ʈ�� �ش��ϴ� ���� ����Ʈ�� �޴´�.
//
//							if( !pDescSubQuestInfo ) 
//							{
//								++count ;
//								continue ;											// ���� ����Ʈ ������ ��ȿ���� ������ continue�Ѵ�.
//							}
//						}
//
//						++count ;
//
//						pQuestString = NULL ;										// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//						pQuestString = (CQuestString*)pList->GetNext(treePos) ;		// ��ġ�� �ش��ϴ� ����Ʈ ��Ʈ���� �޴´�.
//
//						if( pQuestString )											// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
//						{
//							if( (WORD)(pQuestString->GetSubID()) == wSubQuestIdx )	// ��Ʈ���� ���� �� �ε�����, ���ڷ� �Ѿ�� ���� �� �ε����� ������,
//							{
//								char tempBuf[1024] = {0, } ;						// �ӽ� ���۸� �����Ѵ�.
//
//								cPtrList* pTitleList = NULL ;						// ������Ʈ ��Ʈ���� ����Ʈ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//								pTitleList = pQuestString->GetTitle() ;				// ������Ʈ ��Ʈ���� ����Ʈ�� �޴´�.
//
//								if( pTitleList )									// ����Ʈ ������ ��ȿ�ϸ�,
//								{
//									ITEM* pQString = NULL ;						// ����Ʈ ��Ʈ���� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//									PTRLISTPOS titlePos = NULL ;					// ������Ʈ �� ��Ʈ���� ����Ʈ ���� ��ġ�� ���� �����͸� �����ϰ� nulló�� �Ѵ�.
//									titlePos = pTitleList->GetHeadPosition() ;		// ������Ʈ �� ��Ʈ���� ����Ʈ�� ���� �����Ѵ�.
//	
//									while(titlePos)									// ��ġ ������ ��ȿ�� ���� while���� ������.
//									{
//										pQString = NULL ;							// �����͸� nulló���� �Ѵ�.
//										pQString = (ITEM*)pTitleList->GetNext(titlePos) ; // ��ġ�� ���� ��Ʈ�� ������ �޴´�.
//
//										if( pQString )								// ��Ʈ�� ������ ��ȿ�ϸ�,
//										{
//											strcat(tempBuf, pQString->string) ;		// �ӽù��ۿ� ��Ʈ���� ��´�.
//											break ;
//										}
//									}
//
//									pQuestString->SetCount(msg->dwData) ;
//									pQuestString->SetTotalCount(msg->dwMaxCount) ;
//
//									pQuestString->SetTitleStr(tempBuf) ;			// ��Ʈ���� Ÿ��Ʋ�� �����Ѵ�.
//								}
//							}
//						}
//					}
//				}
//				else
//				{
//					ASSERTMSG(0, "����Ʈ �˸�â���� ����Ʈ�� ������Ʈ ���� ���߽��ϴ�.!") ;
//					return ;
//				}
//			}
//		}
//	}
//
//	ResetQuickViewDialog() ;														// �� �並 ���ΰ�ħ�Ѵ�.
//}
//
////void CQuestQuickViewDialog::UpdateQuest(SEND_SUBQUEST_UPDATE* msg)					// �˸��� â�� ��� �� ����Ʈ�� ������Ʈ �ϴ� �Լ�.
////{
////	WORD wQuestIdx = 0 ;															// ����Ʈ �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
////	wQuestIdx = msg->wQuestIdx ;													// ���ڷ� �Ѿ�� �޽����� ���� ����Ʈ �ε����� �޴´�.
////
////	WORD wSubQuestIdx = 0 ;															// ���� ����Ʈ �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
////	wSubQuestIdx = msg->wSubQuestIdx ;												// ���ڷ� �Ѿ�� �޽����� ���� ���� ����Ʈ �ε����� �޴´�.
////
////	QuestTree* pTree = NULL ;														// Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////	CQuestString* pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////
////	PTRLISTPOS pos = NULL ;															// ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////	pos = m_QuestTreeList.GetHeadPosition() ;										// ����Ʈ Ʈ�� ����Ʈ�� ���� �����Ѵ�.
////
////	while(pos)																		// ��ġ ������ ��ȿ�� ���� while���� ������.
////	{
////		pTree = NULL ;																// Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
////		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// ����Ʈ Ʈ�� ����Ʈ���� ����Ʈ Ʈ�� ������ �޴´�.
////
////		if( pTree )																	// Ʈ�� ������ ��ȿ�ϴٸ�,
////		{
////			if( (WORD)(pTree->QuestIdx) == wQuestIdx )								// Ʈ���� ����Ʈ �ε�����, ������Ʈ �� ����Ʈ �ε����� ���ٸ�,
////			{
////				cPtrList* pList = NULL ;											// ����Ʈ ������ ���� �����͸� �����ϰ� nulló���Ѵ�.
////				pList = &(pTree->list) ;											// Ʈ���� ����Ʈ ������ �޴´�.
////
////				if( pList )
////				{
////					PTRLISTPOS treePos = NULL ;										// Ʈ�� ����Ʈ�� ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////					treePos = pList->GetHeadPosition() ;							// Ʈ�� ����Ʈ�� ���� �����Ѵ�.
////
////					while(treePos)													// ��ġ ������ ��ȿ�� ���� while���� ������.
////					{	
////						pQuestString = NULL ;										// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
////						pQuestString = (CQuestString*)pList->GetNext(treePos) ;		// ��ġ�� �ش��ϴ� ����Ʈ ��Ʈ���� �޴´�.
////
////						if( pQuestString )											// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
////						{
////							if( (WORD)(pQuestString->GetSubID()) == wSubQuestIdx )	// ��Ʈ���� ���� �� �ε�����, ���ڷ� �Ѿ�� ���� �� �ε����� ������,
////							{
////								char tempBuf[1024] = {0, } ;						// �ӽ� ���۸� �����Ѵ�.
////
////								cPtrList* pTitleList = NULL ;						// ������Ʈ ��Ʈ���� ����Ʈ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////								pTitleList = pQuestString->GetTitle() ;				// ������Ʈ ��Ʈ���� ����Ʈ�� �޴´�.
////
////								if( pTitleList )									// ����Ʈ ������ ��ȿ�ϸ�,
////								{
////									//sGAMENOTIFY_MSG* pNoticeMsg ;						// ���� ���� �޽��� ����ü�� �����Ѵ�.
////									//pNoticeMsg = new sGAMENOTIFY_MSG ;
////									//pNoticeMsg->nNotifyType = eSTYLE_QUEST ;			// ���� Ÿ���� ����Ʈ�� �����Ѵ�.
////
////									//pNoticeMsg->rect.left = 500 ;
////									//pNoticeMsg->rect.top = 500 ;
////									//pNoticeMsg->rect.right = 1 ;
////									//pNoticeMsg->rect.bottom = 500 ;
////
////
////									QString* pQString = NULL ;						// ����Ʈ ��Ʈ���� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////
////									PTRLISTPOS titlePos = NULL ;					// ������Ʈ �� ��Ʈ���� ����Ʈ ���� ��ġ�� ���� �����͸� �����ϰ� nulló�� �Ѵ�.
////									titlePos = pTitleList->GetHeadPosition() ;		// ������Ʈ �� ��Ʈ���� ����Ʈ�� ���� �����Ѵ�.
////	
////									while(titlePos)									// ��ġ ������ ��ȿ�� ���� while���� ������.
////									{
////										pQString = NULL ;							// �����͸� nulló���� �Ѵ�.
////										pQString = (QString*)pTitleList->GetNext(titlePos) ; // ��ġ�� ���� ��Ʈ�� ������ �޴´�.
////
////										if( pQString )								// ��Ʈ�� ������ ��ȿ�ϸ�,
////										{
////											strcat(tempBuf, pQString->Str) ;		// �ӽù��ۿ� ��Ʈ���� ��´�.
////
////											//ITEM* pNoticeItem = new ITEM ;			// ���� ����Ʈ �������� ���� �������� �����Ѵ�.
////
////											//pNoticeItem->rgb = dwQuestStringColor[eQC_SUBQUEST_NAME] ;	// ���� ����Ʈ ��Ʈ���� ������ �����Ѵ�.
////
////											///*pNoticeMsg->red = 0 ;
////											//pNoticeMsg->gree = 255 ;
////											//pNoticeMsg->blue =  0  ;
////											//pNoticeMsg->nAlpha = 255 ;*/
////
////											//strcpy(pNoticeItem->string, tempBuf) ;	// ���� ����Ʈ ��Ʈ���� �����Ѵ�.
////
////											//pNoticeMsg->itemList.AddTail(pNoticeItem);// ���� �޽��� ����ü�� ����Ʈ�� ���� ����Ʈ �������� �߰��Ѵ�.
////										}
////									}
////
////									//char tempBuf2[128] = {0, } ;					// �ι�° �ӽ� ���۸� �����Ѵ�.
////
////									//if( msg->dwMaxCount == 0 )
////									//{
////									//	sprintf(tempBuf2, " (%d)", msg->dwData) ;// �ӽ� ���ۿ� ���� ������ ǥ���Ѵ�.
////									//}
////									//else
////									//{
////									//	sprintf(tempBuf2, " (%d/%d)", msg->dwData, msg->dwMaxCount) ;// �ӽ� ���ۿ� ���� ������ ǥ���Ѵ�.
////									//}
////
////									pQuestString->SetCount(msg->dwData) ;
////									pQuestString->SetTotalCount(msg->dwMaxCount) ;
////
////									//strcat(tempBuf, tempBuf2) ;						// ù ��° �ӽ� ���ۿ� �� ��° �ӽ� ���۸� �߰��Ѵ�.
////
////									pQuestString->SetTitleStr(tempBuf) ;			// ��Ʈ���� Ÿ��Ʋ�� �����Ѵ�.
////
////
////									//ITEM* pCountItem = new ITEM ;					// ī��Ʈ�� ���� �������� �����Ѵ�.
////
////									//strcpy(pCountItem->string, tempBuf2) ;			// ī��Ʈ �����ۿ� ī��Ʈ ��Ʈ���� �����Ѵ�.
////
////									//pCountItem->rgb = dwQuestStringColor[eQC_COUNT];// ī��Ʈ ��Ʈ���� ������ �����Ѵ�.
////
////									///*pNoticeMsg->red = 0 ;
////									//pNoticeMsg->gree = 0 ;
////									//pNoticeMsg->blue = 255  ;
////									//pNoticeMsg->nAlpha = 255 ;*/
////
////									//pNoticeMsg->itemList.AddTail(pCountItem) ;		// ���� �޽��� ����ü�� ����Ʈ�� ī��Ʈ �������� �߰��Ѵ�.
////									//
////
////									//GAMENOTIFYMGR->AddMsg(pNoticeMsg) ;				// ���� ���� �Ŵ����� ���� �޽����� �߰��Ѵ�.
////								}
////							}
////						}
////					}
////				}
////				else
////				{
////					ASSERTMSG(0, "����Ʈ �˸�â���� ����Ʈ�� ������Ʈ ���� ���߽��ϴ�.!") ;
////					return ;
////				}
////			}
////		}
////	}
////
////	ResetQuickViewDialog() ;														// �� �並 ���ΰ�ħ�Ѵ�.
////}
//
//void CQuestQuickViewDialog::ResetLoadedQuickViewDialog()							// ����Ʈ ����Ʈ�� ���ΰ�ħ�ϴ� �Լ�.
//{
//	DWORD Color = 0;																// ���� ���� ���� ������ �����ϰ� 0���� �����Ѵ�.
//	char buf[256] = { 0, };															// �ӽ� ���۸� �����Ѵ�.	
//
//	QuestTree* pTree ;																// Ʈ�� ������ ���� �����͸� �����Ѵ�.
//	CQuestString* pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	//QString* pLine ;
//
//	PTRLISTPOS pos = NULL ;															// ����Ʈ ���� ��ġ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	PTRLISTPOS treePos = NULL ;														// Ʈ���� ����Ʈ ���� ��ġ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	PTRLISTPOS pLinePos = NULL ;													// ����Ʈ ��Ʈ���� ������ �޴� ��ġ �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	int nItemCount = 0 ;															// �˸��� ����Ʈ�� ������ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
//	nItemCount = m_pViewList->GetItemCount() ;										// �˸��� ����Ʈ�� ������ ī��Ʈ�� �޴´�.
//
//	m_pViewList->RemoveAll() ;														// ����Ʈ ����Ʈ ���̾�α׸� ��� ����.
//
//	for( int count = 0 ; count < nItemCount ; ++count )								// �˸��� ����Ʈ�� ������ ī��Ʈ ��ŭ for���� ������.
//	{
//		m_pViewList->SetExtendReduction(LD_REDUCTION) ;								// ����Ʈ ���̾�α׸� ����Ѵ�.
//	}
//
//	pos = m_QuestTreeList.GetHeadPosition() ;										// ��ġ �����͸� ����Ʈ�� ���� �����Ѵ�.
//
//	while(pos)																		// ����Ʈ�� ���� ��Ұ� ��ȿ�� ���� while���� ������.
//	{
//		pTree = NULL ;																// Ʈ�� ������ ���� �����͸� null ó���� �Ѵ�.
//		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// ��ġ ������ �ش��ϴ� Ʈ���� �޴´�.
//
//		if( pTree )																	// Ʈ�� ������ ��ȿ�ϴٸ�,
//		{
//			treePos = NULL ;														// Ʈ���� ����Ʈ���� ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.	
//			treePos = pTree->list.FindIndex(0) ;									// ����Ʈ�� ù ��ġ�� �޴´�.
//
//			int nSubQuestCount = 0 ;												// ���� ����Ʈ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//			if( treePos )															// Ʈ���� ��ġ ������ ��ȿ�ϸ�,
//			{
//				pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//				pQuestString = (CQuestString*)pTree->list.GetAt(treePos) ;			// Ʈ�� ��ġ �����Ϳ� �ش��ϴ� ����Ʈ ��Ʈ���� �޴´�.
//
//				if( pQuestString )													// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
//				{
//					CQuestInfo* pQuestInfo = NULL ;									// ����Ʈ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//					pQuestInfo = QUESTMGR->GetQuestInfo(pQuestString->GetQuestID()) ;// ����Ʈ �Ŵ����� ����, ���ڷ� �Ѿ�� ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.
//
//					if( !pQuestInfo )												// ����Ʈ ������ ��ȿ���� �ʴٸ�,
//					{
//						ASSERTMSG(0, "����Ʈ �˸��� ��� �� ����Ʈ ���̾�α׿� ��Ʈ�� �߰� �� �� ���� ������ �Խ��ϴ�.!") ;		// assert ���� ó���� �Ѵ�.
//						return ;													// ���� ó���� �Ѵ�.
//					}
//					
//					nSubQuestCount = pQuestInfo->GetSubQuestCount() ;				// ����Ʈ ������ ���� ���� ����Ʈ ī��Ʈ�� �޴´�.
//				}
//				else																// ����Ʈ ��Ʈ�� ������ ��ȿ���� ������,
//				{
//					continue ;														// ��Ƽ�� �Ѵ�.
//				}
//			}
//
//			if( nSubQuestCount > 1 )												// ��� ������ �ּ� �� ���� �Ǿ� �Ѵ�.
//			{
//				int nCountOfTreeCount = 0 ;											// Ʈ���� ����Ʈ�� ī��Ʈ�� ���� ������ �����ϰ�, 0���� �����Ѵ�.
//				nCountOfTreeCount = pTree->list.GetCount() ;						// Ʈ���� ����Ʈ�� ī��Ʈ�� �޴´�.
//
//				treePos = NULL ;													// Ʈ���� ����Ʈ���� ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.	
//				treePos = pTree->list.GetHeadPosition() ;							// Ʈ���� ����Ʈ�� ���� �����Ѵ�.
//
//				while(treePos)														// ��ġ ������ ��ȿ�� ���� while���� ������.
//				{
//					pQuestString = NULL ;											// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//					pQuestString = (CQuestString*)pTree->list.GetNext(treePos) ;	// 0��° ��ġ�� �ش��ϴ� ����Ʈ ��Ʈ���� �޴´�.
//
//					if( pQuestString )												// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
//					{
//						DWORD dwSubQuestIdx = pQuestString->GetSubID() ;			// ���� ����Ʈ �ε����� �޴´�.		
//
//						if(dwSubQuestIdx == 0)										// ���� ����Ʈ �ε����� 0�� ������, 
//						{
//							Color=RGB(0, 255, 255) ;								// ������ ���� ����Ʈ �������� �����Ѵ�.
//						}
//						else														// �� ���� ���.
//						{
//							Color=RGB(255, 255, 255) ;								// ������ ���� ����Ʈ �������� �����Ѵ�.
//						}
//
//						memset(buf, 0, 256) ;										// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
//
//						if( dwSubQuestIdx != 0 )									// ���� ����Ʈ �ε����� 0�� �ƴϸ�
//						{
//							sprintf(buf, "	- ");									// �ӽ� ���۸� -�� �����Ѵ�.
//						}
//
//						strcat(buf, pQuestString->GetTitleStr()) ;					// ��Ʈ���� �����Ѵ�.
//
//						if( pQuestString->IsComplete() )
//						{
//							strcat(buf, "(�Ϸ�)") ;
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
//										strcat(tempBuf2, "(�Ϸ�)") ;
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
//							strcat(buf, "(�Ϸ�)") ;
//						}*/
//
//						if( nCountOfTreeCount > 2 )									// Ʈ���� ����Ʈ�� ī��Ʈ�� 2���� ������,
//						{
//							if( dwSubQuestIdx != nSubQuestCount-1 )					// ���� �ε����� ������ �ε����� �ƴϸ�,
//							{
//								m_pViewList->AddItemWithFont(buf, Color, 0) ;		// ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.
//								m_pViewList->SetExtendReduction(LD_EXTEND) ;		// ����Ʈ ���̾�α׸� Ȯ���Ѵ�.
//							}
//						}
//						else														// Ʈ���� ����Ʈ ī��Ʈ�� 2�� ���ϸ�,
//						{
//							m_pViewList->AddItemWithFont(buf, Color, 0) ;			// ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.
//							m_pViewList->SetExtendReduction(LD_EXTEND) ;			// ����Ʈ ���̾�α׸� Ȯ���Ѵ�.
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
//void CQuestQuickViewDialog::ResetQuickViewDialog()									// ����Ʈ ����Ʈ�� ���ΰ�ħ�ϴ� �Լ�.
//{
// 	DWORD Color = 0;																// ���� ���� ���� ������ �����ϰ� 0���� �����Ѵ�.
//	char buf[256] = { 0, };															// �ӽ� ���۸� �����Ѵ�.	
//
//	QuestTree* pTree ;																// Ʈ�� ������ ���� �����͸� �����Ѵ�.
//	CQuestString* pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	//QString* pLine ;
//
//	PTRLISTPOS pos = NULL ;															// ����Ʈ ���� ��ġ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	PTRLISTPOS treePos = NULL ;														// Ʈ���� ����Ʈ ���� ��ġ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	PTRLISTPOS pLinePos = NULL ;													// ����Ʈ ��Ʈ���� ������ �޴� ��ġ �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	int nItemCount = 0 ;															// �˸��� ����Ʈ�� ������ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
//	nItemCount = m_pViewList->GetItemCount() ;										// �˸��� ����Ʈ�� ������ ī��Ʈ�� �޴´�.
//
//	m_pViewList->RemoveAll() ;														// ����Ʈ ����Ʈ ���̾�α׸� ��� ����.
//
//	for( int count = 0 ; count < nItemCount ; ++count )								// �˸��� ����Ʈ�� ������ ī��Ʈ ��ŭ for���� ������.
//	{
//		m_pViewList->SetExtendReduction(LD_REDUCTION) ;								// ����Ʈ ���̾�α׸� ����Ѵ�.
//	}
//
//	pos = m_QuestTreeList.GetHeadPosition() ;										// ��ġ �����͸� ����Ʈ�� ���� �����Ѵ�.
//
//	while(pos)																		// ����Ʈ�� ���� ��Ұ� ��ȿ�� ���� while���� ������.
//	{
//		pTree = NULL ;																// Ʈ�� ������ ���� �����͸� null ó���� �Ѵ�.
//		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// ��ġ ������ �ش��ϴ� Ʈ���� �޴´�.
//
//		if( pTree )																	// Ʈ�� ������ ��ȿ�ϴٸ�,
//		{
//			CQuestString* pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//			CQuest* pQuest = QUESTMGR->GetQuest( pTree->QuestIdx );							// ����Ʈ ������ �޴´�.
//
//			if( !pQuest ) return ;															// ����Ʈ ������ ��ȿ���� ������, return ó�� �Ѵ�.
//
//			CSubQuestInfo* pDescSubQuestInfo ;												// ���� ����Ʈ ������ ���� �����͸� �����Ѵ�.
//
//			treePos = NULL ;														// Ʈ���� ����Ʈ���� ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.	
//			treePos = pTree->list.FindIndex(0) ;									// ����Ʈ�� ù ��ġ�� �޴´�.
//
//			int nSubQuestCount = 0 ;												// ���� ����Ʈ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//			if( treePos )															// Ʈ���� ��ġ ������ ��ȿ�ϸ�,
//			{
//				pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//				pQuestString = (CQuestString*)pTree->list.GetAt(treePos) ;			// Ʈ�� ��ġ �����Ϳ� �ش��ϴ� ����Ʈ ��Ʈ���� �޴´�.
//
//				if( pQuestString )													// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
//				{
//					CQuestInfo* pQuestInfo = NULL ;									// ����Ʈ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//					pQuestInfo = QUESTMGR->GetQuestInfo(pQuestString->GetQuestID()) ;// ����Ʈ �Ŵ����� ����, ���ڷ� �Ѿ�� ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.
//
//					if( !pQuestInfo )												// ����Ʈ ������ ��ȿ���� �ʴٸ�,
//					{
//						ASSERTMSG(0, "����Ʈ �˸��� ��� �� ����Ʈ ���̾�α׿� ��Ʈ�� �߰� �� �� ���� ������ �Խ��ϴ�.!") ;		// assert ���� ó���� �Ѵ�.
//						return ;													// ���� ó���� �Ѵ�.
//					}
//					
//					nSubQuestCount = pQuestInfo->GetSubQuestCount() ;				// ����Ʈ ������ ���� ���� ����Ʈ ī��Ʈ�� �޴´�.
//				}
//				else																// ����Ʈ ��Ʈ�� ������ ��ȿ���� ������,
//				{
//					continue ;														// ��Ƽ�� �Ѵ�.
//				}
//			}
//
//			if( nSubQuestCount > 1 )												// ��� ������ �ּ� �� ���� �Ǿ� �Ѵ�.
//			{
//				int nCountOfTreeCount = 0 ;											// Ʈ���� ����Ʈ�� ī��Ʈ�� ���� ������ �����ϰ�, 0���� �����Ѵ�.
//				nCountOfTreeCount = pTree->list.GetCount() ;						// Ʈ���� ����Ʈ�� ī��Ʈ�� �޴´�.
//
//				treePos = NULL ;													// Ʈ���� ����Ʈ���� ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.	
//				treePos = pTree->list.GetHeadPosition() ;							// Ʈ���� ����Ʈ�� ���� �����Ѵ�.
//
//				int count = 0 ;
//
//				while(treePos)														// ��ġ ������ ��ȿ�� ���� while���� ������.
//				{
//					if( QUESTMGR->CheckQuestType(pTree->QuestIdx, count) == e_QT_MULTY_HUNT )
//					{
//						if( count != 0 )
//						{
//							pDescSubQuestInfo = NULL ;													// ���� ����Ʈ ������ ���� �����͸� nulló�� �Ѵ�.
//							pDescSubQuestInfo = pQuest->GetSubQuestInfoArray(count) ;					// ī��Ʈ�� �ش��ϴ� ���� ����Ʈ�� �޴´�.
//
//							if( !pDescSubQuestInfo ) 
//							{
//								++count ;
//								continue ;											// ���� ����Ʈ ������ ��ȿ���� ������ continue�Ѵ�.
//							}
//						}
//
//						++count ;
//					}
//
//					pQuestString = NULL ;											// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//					pQuestString = (CQuestString*)pTree->list.GetNext(treePos) ;	// 0��° ��ġ�� �ش��ϴ� ����Ʈ ��Ʈ���� �޴´�.
//
//					if( pQuestString )												// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
//					{
//						DWORD dwSubQuestIdx = pQuestString->GetSubID() ;			// ���� ����Ʈ �ε����� �޴´�.		
//
//						if(dwSubQuestIdx == 0)										// ���� ����Ʈ �ε����� 0�� ������, 
//						{
//							Color=RGB(0, 255, 255) ;								// ������ ���� ����Ʈ �������� �����Ѵ�.
//						}
//						else														// �� ���� ���.
//						{
//							Color=RGB(255, 255, 255) ;								// ������ ���� ����Ʈ �������� �����Ѵ�.
//						}
//
//						memset(buf, 0, 256) ;										// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
//
//						if( dwSubQuestIdx != 0 )									// ���� ����Ʈ �ε����� 0�� �ƴϸ�
//						{
//							sprintf(buf, "	- ");									// �ӽ� ���۸� -�� �����Ѵ�.
//						}
//
//						strcat(buf, pQuestString->GetTitleStr()) ;					// ��Ʈ���� �����Ѵ�.
//
//						if( pQuestString->GetSubID() == nSubQuestCount-1 )
//						{
//							strcat(buf, "(�Ϸ�)") ;
//						}
//						else
//						{
//							if( pQuestString->IsComplete() )
//							{
//								if( pQuestString->GetSubID() != 0 )
//								{
//									strcat(buf, "(�Ϸ�)") ;
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
//												strcat(tempBuf2, "(�Ϸ�)") ;
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
//							strcat(buf, "(�Ϸ�)") ;
//						}*/
//
//						if( nCountOfTreeCount > 2 )									// Ʈ���� ����Ʈ�� ī��Ʈ�� 2���� ������,
//						{
//							if( dwSubQuestIdx != nSubQuestCount-1 )					// ���� �ε����� ������ �ε����� �ƴϸ�,
//							{
//								m_pViewList->AddItemWithFont(buf, Color, 0) ;		// ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.
//								m_pViewList->SetExtendReduction(LD_EXTEND) ;		// ����Ʈ ���̾�α׸� Ȯ���Ѵ�.
//							}
//						}
//						else														// Ʈ���� ����Ʈ ī��Ʈ�� 2�� ���ϸ�,
//						{
//							m_pViewList->AddItemWithFont(buf, Color, 0) ;			// ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.
//							m_pViewList->SetExtendReduction(LD_EXTEND) ;			// ����Ʈ ���̾�α׸� Ȯ���Ѵ�.
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
////void CQuestQuickViewDialog::ResetQuickViewDialog()									// ����Ʈ ����Ʈ�� ���ΰ�ħ�ϴ� �Լ�.
////{
//// 	DWORD Color = 0;																// ���� ���� ���� ������ �����ϰ� 0���� �����Ѵ�.
////	char buf[256] = { 0, };															// �ӽ� ���۸� �����Ѵ�.	
////
////	QuestTree* pTree ;																// Ʈ�� ������ ���� �����͸� �����Ѵ�.
////	CQuestString* pQuestString ;													// ��Ʈ�� ������ ���� �����͸� �����Ѵ�.
////	//QString* pLine ;
////
////	PTRLISTPOS pos = NULL ;															// ����Ʈ ���� ��ġ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////	PTRLISTPOS treePos = NULL ;														// Ʈ���� ����Ʈ ���� ��ġ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////	PTRLISTPOS pLinePos = NULL ;													// ����Ʈ ��Ʈ���� ������ �޴� ��ġ �����͸� �����ϰ� nulló���� �Ѵ�.
////
////	int nItemCount = 0 ;															// �˸��� ����Ʈ�� ������ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
////	nItemCount = m_pViewList->GetItemCount() ;										// �˸��� ����Ʈ�� ������ ī��Ʈ�� �޴´�.
////
////	m_pViewList->RemoveAll() ;														// ����Ʈ ����Ʈ ���̾�α׸� ��� ����.
////
////	for( int count = 0 ; count < nItemCount ; ++count )								// �˸��� ����Ʈ�� ������ ī��Ʈ ��ŭ for���� ������.
////	{
////		m_pViewList->SetExtendReduction(LD_REDUCTION) ;								// ����Ʈ ���̾�α׸� ����Ѵ�.
////	}
////
////	pos = m_QuestTreeList.GetHeadPosition() ;										// ��ġ �����͸� ����Ʈ�� ���� �����Ѵ�.
////
////	while(pos)																		// ����Ʈ�� ���� ��Ұ� ��ȿ�� ���� while���� ������.
////	{
////		pTree = NULL ;																// Ʈ�� ������ ���� �����͸� null ó���� �Ѵ�.
////		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// ��ġ ������ �ش��ϴ� Ʈ���� �޴´�.
////
////		if( pTree )																	// Ʈ�� ������ ��ȿ�ϴٸ�,
////		{
////			treePos = NULL ;														// Ʈ���� ����Ʈ���� ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.	
////			treePos = pTree->list.FindIndex(0) ;									// ����Ʈ�� ù ��ġ�� �޴´�.
////
////			int nSubQuestCount = 0 ;												// ���� ����Ʈ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
////
////			if( treePos )															// Ʈ���� ��ġ ������ ��ȿ�ϸ�,
////			{
////				pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
////				pQuestString = (CQuestString*)pTree->list.GetAt(treePos) ;			// Ʈ�� ��ġ �����Ϳ� �ش��ϴ� ����Ʈ ��Ʈ���� �޴´�.
////
////				if( pQuestString )													// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
////				{
////					CQuestInfo* pQuestInfo = NULL ;									// ����Ʈ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////					pQuestInfo = QUESTMGR->GetQuestInfo(pQuestString->GetQuestID()) ;// ����Ʈ �Ŵ����� ����, ���ڷ� �Ѿ�� ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.
////
////					if( !pQuestInfo )												// ����Ʈ ������ ��ȿ���� �ʴٸ�,
////					{
////						ASSERTMSG(0, "����Ʈ �˸��� ��� �� ����Ʈ ���̾�α׿� ��Ʈ�� �߰� �� �� ���� ������ �Խ��ϴ�.!") ;		// assert ���� ó���� �Ѵ�.
////						return ;													// ���� ó���� �Ѵ�.
////					}
////					
////					nSubQuestCount = pQuestInfo->GetSubQuestCount() ;				// ����Ʈ ������ ���� ���� ����Ʈ ī��Ʈ�� �޴´�.
////				}
////				else																// ����Ʈ ��Ʈ�� ������ ��ȿ���� ������,
////				{
////					continue ;														// ��Ƽ�� �Ѵ�.
////				}
////			}
////
////			if( nSubQuestCount > 1 )												// ��� ������ �ּ� �� ���� �Ǿ� �Ѵ�.
////			{
////				int nCountOfTreeCount = 0 ;											// Ʈ���� ����Ʈ�� ī��Ʈ�� ���� ������ �����ϰ�, 0���� �����Ѵ�.
////				nCountOfTreeCount = pTree->list.GetCount() ;						// Ʈ���� ����Ʈ�� ī��Ʈ�� �޴´�.
////
////				treePos = NULL ;													// Ʈ���� ����Ʈ���� ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.	
////				treePos = pTree->list.GetHeadPosition() ;							// Ʈ���� ����Ʈ�� ���� �����Ѵ�.
////
////				while(treePos)														// ��ġ ������ ��ȿ�� ���� while���� ������.
////				{
////					pQuestString = NULL ;											// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
////					pQuestString = (CQuestString*)pTree->list.GetNext(treePos) ;	// 0��° ��ġ�� �ش��ϴ� ����Ʈ ��Ʈ���� �޴´�.
////
////					if( pQuestString )												// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
////					{
////						DWORD dwSubQuestIdx = pQuestString->GetSubID() ;			// ���� ����Ʈ �ε����� �޴´�.		
////
////						if(dwSubQuestIdx == 0)										// ���� ����Ʈ �ε����� 0�� ������, 
////						{
////							Color=RGB(0, 255, 255) ;								// ������ ���� ����Ʈ �������� �����Ѵ�.
////						}
////						else														// �� ���� ���.
////						{
////							Color=RGB(255, 255, 255) ;								// ������ ���� ����Ʈ �������� �����Ѵ�.
////						}
////
////						memset(buf, 0, 256) ;										// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
////
////						if( dwSubQuestIdx != 0 )									// ���� ����Ʈ �ε����� 0�� �ƴϸ�
////						{
////							sprintf(buf, "	- ");									// �ӽ� ���۸� -�� �����Ѵ�.
////						}
////
////						strcat(buf, pQuestString->GetTitleStr()) ;					// ��Ʈ���� �����Ѵ�.
////
////						if( pQuestString->GetSubID() == nSubQuestCount-1 )
////						{
////							strcat(buf, "(�Ϸ�)") ;
////						}
////						else
////						{
////							if( pQuestString->IsComplete() )
////							{
////								if( pQuestString->GetSubID() != 0 )
////								{
////									strcat(buf, "(�Ϸ�)") ;
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
////											strcat(tempBuf2, "(�Ϸ�)") ;
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
////							strcat(buf, "(�Ϸ�)") ;
////						}*/
////
////						if( nCountOfTreeCount > 2 )									// Ʈ���� ����Ʈ�� ī��Ʈ�� 2���� ������,
////						{
////							if( dwSubQuestIdx != nSubQuestCount-1 )					// ���� �ε����� ������ �ε����� �ƴϸ�,
////							{
////								m_pViewList->AddItemWithFont(buf, Color, 0) ;		// ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.
////								m_pViewList->SetExtendReduction(LD_EXTEND) ;		// ����Ʈ ���̾�α׸� Ȯ���Ѵ�.
////							}
////						}
////						else														// Ʈ���� ����Ʈ ī��Ʈ�� 2�� ���ϸ�,
////						{
////							m_pViewList->AddItemWithFont(buf, Color, 0) ;			// ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.
////							m_pViewList->SetExtendReduction(LD_EXTEND) ;			// ����Ʈ ���̾�α׸� Ȯ���Ѵ�.
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
//void CQuestQuickViewDialog::CheckTreeState(int nSelectedLine)						// Ʈ���� ���� �������� ���� �������� üũ�ؼ� ����Ʈ�� �����ϴ� �Լ�.
//{
//	QuestTree* pTree = NULL ;														// Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	CQuestString* pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	PTRLISTPOS pos = NULL ;															// ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	pos = m_QuestTreeList.GetHeadPosition() ;										// ����Ʈ Ʈ�� ����Ʈ�� ���� �����Ѵ�.
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
//			treePos = NULL ;														// Ʈ���� ����Ʈ���� ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.	
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
//	DWORD Color = 0;																// ���� ���� ���� ������ �����ϰ� 0���� �����Ѵ�.
//	char buf[256] = { 0, };															// �ӽ� ���۸� �����Ѵ�.	
//
//	int nItemCount = 0 ;															// �˸��� ����Ʈ�� ������ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
//	nItemCount = m_pViewList->GetItemCount() ;										// �˸��� ����Ʈ�� ������ ī��Ʈ�� �޴´�.
//
//	for( int count = 0 ; count < nItemCount ; ++count )								// �˸��� ����Ʈ�� ������ ī��Ʈ ��ŭ for���� ������.
//	{
//		m_pViewList->SetExtendReduction(LD_REDUCTION) ;								// ����Ʈ ���̾�α׸� ����Ѵ�.
//	}
//
//	m_pViewList->RemoveAll() ;														// ����Ʈ ����Ʈ ���̾�α׸� ��� ����.
//
//	pos = NULL ;																	// ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	pos = m_QuestTreeList.GetHeadPosition() ;										// ����Ʈ Ʈ�� ����Ʈ�� ���� �����Ѵ�.
//
//	while(pos)
//	{
//		pTree = NULL ;																// Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//		pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// ����Ʈ Ʈ�� ����Ʈ���� ����Ʈ Ʈ�� ������ �޴´�.
//
//		if( pTree )
//		{
//			treePos = NULL ;														// Ʈ���� ����Ʈ���� ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.	
//			treePos = pTree->list.FindIndex(0) ;									// ����Ʈ�� ù ��ġ�� �޴´�.
//
//			int nSubQuestCount = 0 ;												// ���� ����Ʈ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//			if( treePos )															// Ʈ���� ��ġ ������ ��ȿ�ϸ�,
//			{
//				pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//				pQuestString = (CQuestString*)pTree->list.GetAt(treePos) ;			// Ʈ�� ��ġ �����Ϳ� �ش��ϴ� ����Ʈ ��Ʈ���� �޴´�.
//
//				if( pQuestString )													// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
//				{
//					CQuestInfo* pQuestInfo = NULL ;									// ����Ʈ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//					pQuestInfo = QUESTMGR->GetQuestInfo(pQuestString->GetQuestID()) ;// ����Ʈ �Ŵ����� ����, ���ڷ� �Ѿ�� ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.
//
//					if( !pQuestInfo )												// ����Ʈ ������ ��ȿ���� �ʴٸ�,
//					{
//						ASSERTMSG(0, "����Ʈ �˸��� ��� �� ����Ʈ ���̾�α׿� ��Ʈ�� �߰� �� �� ���� ������ �Խ��ϴ�.!") ;		// assert ���� ó���� �Ѵ�.
//						return ;													// ���� ó���� �Ѵ�.
//					}
//					
//					nSubQuestCount = pQuestInfo->GetSubQuestCount() ;				// ����Ʈ ������ ���� ���� ����Ʈ ī��Ʈ�� �޴´�.
//				}
//				else																// ����Ʈ ��Ʈ�� ������ ��ȿ���� ������,
//				{
//					continue ;														// ��Ƽ�� �Ѵ�.
//				}
//			}
//			if( nSubQuestCount > 1 )												// ��� ������ �ּ� �� ���� �Ǿ� �Ѵ�.
//			{
//				int nCountOfTreeCount = 0 ;											// Ʈ���� ����Ʈ�� ī��Ʈ�� ���� ������ �����ϰ�, 0���� �����Ѵ�.
//				nCountOfTreeCount = pTree->list.GetCount() ;						// Ʈ���� ����Ʈ�� ī��Ʈ�� �޴´�.
//
//				cPtrList* pList = NULL ;												// ����Ʈ ������ ���� �����͸� �����ϰ� nulló���Ѵ�.
//				pList = &(pTree->list) ;												// Ʈ���� ����Ʈ ������ �޴´�.
//
//				treePos = NULL ;													// Ʈ�� ����Ʈ�� ��ġ ������ ���� �����͸� nulló���� �Ѵ�.
//				treePos = pList->GetHeadPosition() ;								// Ʈ�� ����Ʈ�� ���� �����Ѵ�.
//
//				if( pTree->State == eQTree_Open )
//				{
//					while(treePos)
//					{
//						pQuestString = NULL ;										// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//						pQuestString = (CQuestString*)pList->GetNext(treePos) ;		// ��ġ�� �ش��ϴ� ����Ʈ ��Ʈ���� �޴´�.
//
//						if( pQuestString )
//						{
//							DWORD dwSubQuestIdx = pQuestString->GetSubID() ;		// ���� ����Ʈ �ε����� �޴´�.		
//
//							if(dwSubQuestIdx == 0)									// ���� ����Ʈ �ε����� 0�� ������, 
//							{
//								Color=RGB(255, 100, 0) ;							// ������ ���� ����Ʈ �������� �����Ѵ�.
//							}
//							else													// �� ���� ���.
//							{
//								Color=RGB(255, 255, 255) ;							// ������ ���� ����Ʈ �������� �����Ѵ�.
//							}
//
//							memset(buf, 0, 256) ;									// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
//
//							if( dwSubQuestIdx != 0 )								// ���� ����Ʈ �ε����� 0�� �ƴϸ�
//							{
//								sprintf(buf, "	- ");								// �ӽ� ���۸� -�� �����Ѵ�.
//							}
//
//							strcat(buf, pQuestString->GetTitleStr()) ;				// ��Ʈ���� �����Ѵ�.
//
//							if( nCountOfTreeCount > 2 )								// Ʈ���� ����Ʈ�� ī��Ʈ�� 2���� ������,
//							{
//								if( dwSubQuestIdx != nSubQuestCount-1 )				// ���� �ε����� ������ �ε����� �ƴϸ�,
//								{
//									m_pViewList->SetExtendReduction(LD_EXTEND) ;	// ����Ʈ ���̾�α׸� Ȯ���Ѵ�.
//									m_pViewList->AddItemWithFont(buf, Color, 0) ;	// ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.
//								}
//							}
//							else													// Ʈ���� ����Ʈ ī��Ʈ�� 2�� ���ϸ�,
//							{
//								m_pViewList->SetExtendReduction(LD_EXTEND) ;		// ����Ʈ ���̾�α׸� Ȯ���Ѵ�.
//								m_pViewList->AddItemWithFont(buf, Color, 0) ;		// ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.
//							}
//						}
//					}	
//				}
//				else
//				{
//					while(treePos)
//					{
//						pQuestString = NULL ;										// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//						pQuestString = (CQuestString*)pList->GetNext(treePos) ;		// ��ġ�� �ش��ϴ� ����Ʈ ��Ʈ���� �޴´�.
//
//						if( pQuestString )
//						{
//							DWORD dwSubQuestIdx = pQuestString->GetSubID() ;		// ���� ����Ʈ �ε����� �޴´�.		
//
//							if(dwSubQuestIdx == 0)									// ���� ����Ʈ �ε����� 0�� ������, 
//							{
//								Color=RGB(255, 100, 0) ;							// ������ ���� ����Ʈ �������� �����Ѵ�.
//
//								m_pViewList->SetExtendReduction(LD_EXTEND) ;		// ����Ʈ ���̾�α׸� Ȯ���Ѵ�.
//								m_pViewList->AddItemWithFont(pQuestString->GetTitleStr(), Color, 0) ;	// ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.
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
//	//int nCheckCount = -1 ;															// ���� �� ���ΰ� ����Ʈ ��Ʈ���� üũ�ϱ� ���� ������ �����ϰ� -1�� �����Ѵ�.
//
//	//BOOL bSameTree = FALSE ;														// �˸��� ����Ʈ���� ���� �� Ʈ���� ������ ���θ� üũ�ϱ� ���� ������ �����ϰ� FALSE����.
//
//	//int nItemCount = 0 ;															// �˸��� ����Ʈ�� ������ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
//	//nItemCount = m_pViewList->GetItemCount() ;										// �˸��� ����Ʈ�� ������ ī��Ʈ�� �޴´�.
//
//	//for( int count = 0 ; count < nItemCount ; ++count )								// �˸��� ����Ʈ�� ������ ī��Ʈ ��ŭ for���� ������.
//	//{
//	//	m_pViewList->SetExtendReduction(LD_REDUCTION) ;								// ����Ʈ ���̾�α׸� ����Ѵ�.
//	//}
//
//	//m_pViewList->RemoveAll() ;														// ����Ʈ ����Ʈ ���̾�α׸� ��� ����.
//
//	//while(pos)																		// ��ġ ������ ��ȿ�� ���� while���� ������.
//	//{
//	//	pTree = NULL ;																// Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//	//	pTree = (QuestTree*)m_QuestTreeList.GetNext(pos) ;							// ����Ʈ Ʈ�� ����Ʈ���� ����Ʈ Ʈ�� ������ �޴´�.
//
//	//	if( pTree )
//	//	{
//	//		PTRLISTPOS treePos = NULL ;												// Ʈ���� ����Ʈ���� ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.	
//	//		treePos = pTree->list.FindIndex(0) ;									// ����Ʈ�� ù ��ġ�� �޴´�.
//
//	//		int nSubQuestCount = 0 ;												// ���� ����Ʈ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//	//		if( treePos )															// Ʈ���� ��ġ ������ ��ȿ�ϸ�,
//	//		{
//	//			pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//	//			pQuestString = (CQuestString*)pTree->list.GetAt(treePos) ;			// Ʈ�� ��ġ �����Ϳ� �ش��ϴ� ����Ʈ ��Ʈ���� �޴´�.
//
//	//			if( pQuestString )													// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
//	//			{
//	//				CQuestInfo* pQuestInfo = NULL ;									// ����Ʈ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	//				pQuestInfo = QUESTMGR->GetQuestInfo(pQuestString->GetQuestID()) ;// ����Ʈ �Ŵ����� ����, ���ڷ� �Ѿ�� ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.
//
//	//				if( !pQuestInfo )												// ����Ʈ ������ ��ȿ���� �ʴٸ�,
//	//				{
//	//					ASSERTMSG(0, "����Ʈ �˸��� ��� �� ����Ʈ ���̾�α׿� ��Ʈ�� �߰� �� �� ���� ������ �Խ��ϴ�.!") ;		// assert ���� ó���� �Ѵ�.
//	//					return ;													// ���� ó���� �Ѵ�.
//	//				}
//	//				
//	//				nSubQuestCount = pQuestInfo->GetSubQuestCount() ;				// ����Ʈ ������ ���� ���� ����Ʈ ī��Ʈ�� �޴´�.
//	//			}
//	//			else																// ����Ʈ ��Ʈ�� ������ ��ȿ���� ������,
//	//			{
//	//				continue ;														// ��Ƽ�� �Ѵ�.
//	//			}
//	//		}
//
//	//		cPtrList* pList = NULL ;												// ����Ʈ ������ ���� �����͸� �����ϰ� nulló���Ѵ�.
//	//		pList = &(pTree->list) ;												// Ʈ���� ����Ʈ ������ �޴´�.
//
//	//		int nCountOfTreeCount = 0 ;												// Ʈ���� ����Ʈ�� ī��Ʈ�� ���� ������ �����ϰ�, 0���� �����Ѵ�.
//	//		nCountOfTreeCount = pTree->list.GetCount() ;							// Ʈ���� ����Ʈ�� ī��Ʈ�� �޴´�.
//
//	//		treePos = NULL ;														// Ʈ�� ����Ʈ�� ��ġ ������ ���� �����͸� nulló���� �Ѵ�.
//	//		treePos = pList->GetHeadPosition() ;									// Ʈ�� ����Ʈ�� ���� �����Ѵ�.
//
//	//		while(treePos)															// ��ġ ������ ��ȿ�� ���� while���� ������.
//	//		{	
//	//			pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//	//			pQuestString = (CQuestString*)pList->GetNext(treePos) ;				// ��ġ�� �ش��ϴ� ����Ʈ ��Ʈ���� �޴´�.
//
//	//			if( pQuestString )													// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
//	//			{
//	//				++nCheckCount ;													// üũ ī��Ʈ�� �����Ѵ�.
//
//	//				if( nSelectedLine == nCheckCount )								// �˸��� ����Ʈ���� ���� �� ���ΰ�, üũ ī��Ʈ�� ���ٸ�,
//	//				{
//	//					bSameTree = TRUE ;											// ���� Ʈ����� üũ ������ TRUE�� �����Ѵ�.
//
//	//					if( pQuestString->GetSubID() == 0 )							// ���� ����Ʈ ��Ʈ���� ���� ���̵� 0�̶��,(ū ���� ��Ʈ���̴�)
//	//					{
//	//						if( pTree->State == eQTree_Open )						// ���� Ʈ���� �����ִ� ���¶��,
//	//						{
//	//							pTree->State = eQTree_Close ;						// ���� Ʈ���� ���� ���·� �����Ѵ�.
//	//						}
//	//						else													// ���� Ʈ���� �����ִ� ���¶��,
//	//						{
//	//							pTree->State = eQTree_Open ;						// ���� Ʈ���� ���� ���·� �����Ѵ�.
//	//						}
//	//					}
//
//	//					Color=RGB(255, 100, 0) ;									// ������ ���� ����Ʈ �������� �����Ѵ�.
//
//	//					memset(buf, 0, 256) ;										// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
//
//	//					strcat(buf, pQuestString->GetTitleStr()) ;					// ��Ʈ���� �����Ѵ�.
//
//	//					m_pViewList->SetExtendReduction(LD_EXTEND) ;				// ����Ʈ ���̾�α׸� Ȯ���Ѵ�.
//	//					m_pViewList->AddItemWithFont(buf, Color, 0) ;				// ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.
//	//				}
//	//				else															// �˸��� ����Ʈ���� ���� �� ���ΰ� �ٸ� Ʈ�����,
//	//				{
//	//					if( bSameTree )												// �˸��� ����Ʈ���� ���� �� Ʈ���� ���� �����̶��,
//	//					{
//	//						if( pTree->State == eQTree_Open )						// ���� Ʈ���� �����ִ� ���¶��,
//	//						{
//	//							DWORD dwSubQuestIdx = pQuestString->GetSubID() ;	// ���� ����Ʈ �ε����� �޴´�.		
//
//	//							if(dwSubQuestIdx == 0)								// ���� ����Ʈ �ε����� 0�� ������, 
//	//							{
//	//								Color=RGB(255, 100, 0) ;						// ������ ���� ����Ʈ �������� �����Ѵ�.
//	//							}
//	//							else												// �� ���� ���.
//	//							{
//	//								Color=RGB(255, 255, 255) ;						// ������ ���� ����Ʈ �������� �����Ѵ�.
//	//							}
//
//	//							memset(buf, 0, 256) ;								// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
//
//	//							if( dwSubQuestIdx != 0 )							// ���� ����Ʈ �ε����� 0�� �ƴϸ�
//	//							{
//	//								sprintf(buf, "	- ");							// �ӽ� ���۸� -�� �����Ѵ�.
//	//							}
//
//	//							strcat(buf, pQuestString->GetTitleStr()) ;			// ��Ʈ���� �����Ѵ�.
//
//	//							if( nCountOfTreeCount > 2 )							// Ʈ���� ����Ʈ�� ī��Ʈ�� 2���� ������,
//	//							{
//	//								if( dwSubQuestIdx != nSubQuestCount-1 )			// ���� �ε����� ������ �ε����� �ƴϸ�,
//	//								{
//	//									m_pViewList->SetExtendReduction(LD_EXTEND) ;// ����Ʈ ���̾�α׸� Ȯ���Ѵ�.
//	//									m_pViewList->AddItemWithFont(buf, Color, 0);// ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.
//	//								}
//	//							}
//	//							else												// Ʈ���� ����Ʈ ī��Ʈ�� 2�� ���ϸ�,
//	//							{
//	//								m_pViewList->SetExtendReduction(LD_EXTEND) ;	// ����Ʈ ���̾�α׸� Ȯ���Ѵ�.
//	//								m_pViewList->AddItemWithFont(buf, Color, 0) ;	// ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.
//	//							}
//	//						}
//	//					}
//	//					else
//	//					{
//	//						DWORD dwSubQuestIdx = pQuestString->GetSubID() ;		// ���� ����Ʈ �ε����� �޴´�.		
//
//	//						if(dwSubQuestIdx == 0)									// ���� ����Ʈ �ε����� 0�� ������, 
//	//						{
//	//							Color=RGB(255, 100, 0) ;							// ������ ���� ����Ʈ �������� �����Ѵ�.
//	//						}
//	//						else													// �� ���� ���.
//	//						{
//	//							Color=RGB(255, 255, 255) ;							// ������ ���� ����Ʈ �������� �����Ѵ�.
//	//						}
//
//	//						memset(buf, 0, 256) ;									// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
//
//	//						if( dwSubQuestIdx != 0 )								// ���� ����Ʈ �ε����� 0�� �ƴϸ�
//	//						{
//	//							sprintf(buf, "	- ");								// �ӽ� ���۸� -�� �����Ѵ�.
//	//						}
//
//	//						strcat(buf, pQuestString->GetTitleStr()) ;				// ��Ʈ���� �����Ѵ�.
//
//	//						if( nCountOfTreeCount > 2 )								// Ʈ���� ����Ʈ�� ī��Ʈ�� 2���� ������,
//	//						{
//	//							if( dwSubQuestIdx != nSubQuestCount-1 )				// ���� �ε����� ������ �ε����� �ƴϸ�,
//	//							{
//	//								m_pViewList->SetExtendReduction(LD_EXTEND) ;	// ����Ʈ ���̾�α׸� Ȯ���Ѵ�.
//	//								m_pViewList->AddItemWithFont(buf, Color, 0) ;	// ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.
//	//							}
//	//						}
//	//						else													// Ʈ���� ����Ʈ ī��Ʈ�� 2�� ���ϸ�,
//	//						{
//	//							m_pViewList->SetExtendReduction(LD_EXTEND) ;		// ����Ʈ ���̾�α׸� Ȯ���Ѵ�.
//	//							m_pViewList->AddItemWithFont(buf, Color, 0) ;		// ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.
//	//						}
//	//					}
//	//				}
//	//			}
//	//		}
//
//	//		bSameTree = FALSE ;														// ���� Ʈ����� üũ ������ TRUE�� �����Ѵ�.
//	//	}
//	//}
//}
//
////void CQuestQuickViewDialog::MainDataLoad()											// �� �信 ��� �Ǿ� �ִ� ����Ʈ �����͸� �ε��ϴ� �Լ�.
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
//	CQuestString* pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	CQuest* pQuest = QUESTMGR->GetQuest( pInfo->dwQuestIdx );							// ����Ʈ ������ �޴´�.
//
//	if( !pQuest ) return ;															// ����Ʈ ������ ��ȿ���� ������, return ó�� �Ѵ�.
//
//	CSubQuestInfo* pDescSubQuestInfo ;												// ���� ����Ʈ ������ ���� �����͸� �����Ѵ�.
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
//		if( CheckSameQuest(dwQuestIdx) )												// ����Ʈ ����Ʈ�� ���ڷ� �Ѿ�� ����Ʈ�� ���� ����Ʈ�� �ִٸ�,
//		{
//			return ;																	// ���� ó���� �Ѵ�.
//		}
//
//		int nTreeCount = 0 ;															// ����Ʈ Ʈ�� ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//		nTreeCount = m_QuestTreeList.GetCount() ;												// ����Ʈ�� ��� ����Ʈ�� ī��Ʈ�� �޴´�.	
//
//		if( nTreeCount >= MAX_QUESTTREECOUNT )											// Ʈ�� ī��Ʈ�� �ִ� Ʈ�� �� �̻��̸�,
//		{
//			return ;
//		}
//		else																			// Ʈ�� ������ 5�� �̸��̸�,
//		{
//			QuestTree* pTree = new QuestTree ;											// Ʈ���� �����Ѵ�.
//
//			CQuestInfo* pQuestInfo = NULL ;												// ����Ʈ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//			pQuestInfo = QUESTMGR->GetQuestInfo(dwQuestIdx) ;							// ����Ʈ �Ŵ����� ����, ���ڷ� �Ѿ�� ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.
//
//			if( !pQuestInfo )															// ����Ʈ ������ ��ȿ���� �ʴٸ�,
//			{
//				ASSERTMSG(0, "����Ʈ �˸��� ��� �� ����Ʈ ������ �޾ƿ��� ���߽��ϴ�.!") ;		// assert ���� ó���� �Ѵ�.
//				return ;																// ���� ó���� �Ѵ�.
//			}
//
//			int nSubQuestCount = 0 ;													// ���� ����Ʈ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
//			nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// ����Ʈ ������ ���� ���� ����Ʈ ī��Ʈ�� �޴´�.
//
//			CQuestString* pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//			
//			for(int count = 0 ; count < nSubQuestCount ; ++count )						// ���� ����Ʈ ī��Ʈ ��ŭ for���� ������.
//			{
//				if( QUESTMGR->CheckQuestType(dwQuestIdx, count) == e_QT_MULTY_HUNT )
//				{
//					if( count != 0 )
//					{
//						pDescSubQuestInfo = NULL ;													// ���� ����Ʈ ������ ���� �����͸� nulló�� �Ѵ�.
//						pDescSubQuestInfo = pQuest->GetSubQuestInfoArray(count) ;					// ī��Ʈ�� �ش��ϴ� ���� ����Ʈ�� �޴´�.
//
//						if( !pDescSubQuestInfo ) continue ;											// ���� ����Ʈ ������ ��ȿ���� ������ continue�Ѵ�.
//					}
//				}
//
//				pQuestString = NULL ;													// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
//				pQuestString = QUESTMGR->GetQuestString(dwQuestIdx, count) ;			// ����Ʈ �ε�����, ���� ����Ʈ �ε����� ����Ʈ ��Ʈ���� �޴´�.
//	            
//				if( pQuestString )														// ����Ʈ ��Ʈ�� ������ ��ȿ�ϴٸ�,
//				{
//					CQuestString* pNewQuestString = new CQuestString ;						// ����Ʈ ��Ʈ���� �����Ѵ�.
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
//					cPtrList* pList = pQuestString->GetTitle() ;						// ����Ʈ ��Ʈ���� Ÿ��Ʋ ����Ʈ�� �޴´�.
//
//					PTRLISTPOS titlePos = NULL ;										// Ÿ��Ʋ ����Ʈ ���� ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//					titlePos = pList->GetHeadPosition() ;								// Ÿ��Ʋ ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.
//
//					ITEM* pQString = NULL ;											// Q��Ʈ���� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//					while(titlePos)														// ��ġ ������ ��ȿ�� ���� while���� ������.
//					{
//						pQString = NULL ;												// Q��Ʈ�� �����͸� nulló���� �Ѵ�.
//						pQString = (ITEM*)pList->GetNext(titlePos) ;					// ��ġ�� ���� Q��Ʈ���� �޴´�.
//
//						if( pQString )													// Q��Ʈ�� ������ ��ȿ�ϴٸ�,
//						{
//							pNewQuestString->SetTitleStr( pQString->string ) ;					// Q��Ʈ���� ���ڿ��� �޾Ƽ� ���ο� ����Ʈ ��Ʈ���� Ÿ��Ʋ�� �����Ѵ�.
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
//					pTree->list.AddTail(pNewQuestString) ;									// Ʈ���� ����Ʈ�� ����Ʈ ��Ʈ���� �߰��Ѵ�.
//
//				}
//			}
//
//			pTree->State = eQTree_Open ;												// Ʈ�� ���¸� ���� ���·� �����Ѵ�.
//			pTree->QuestIdx = dwQuestIdx ;												// Ʈ���� ����Ʈ �ε����� �����Ѵ�.
//			m_QuestTreeList.AddTail(pTree) ;														// ����Ʈ ����Ʈ�� Ʈ���� �߰��Ѵ�.
//
//			//QUESTMGR->RegistQuestIDToQuickView(index, dwQuestIdx) ;									// �˸�â�� ��� �� ����Ʈ �ε����� ����Ʈ �Ŵ����� ���� �Ѵ�.
//		}
//
//		//pQuickViewDlg->ResetLoadedQuickViewDialog() ;									// �� �並 ���ΰ�ħ�Ѵ�.
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
////		if( CheckSameQuest(dwQuestIdx) )												// ����Ʈ ����Ʈ�� ���ڷ� �Ѿ�� ����Ʈ�� ���� ����Ʈ�� �ִٸ�,
////		{
////			return ;																	// ���� ó���� �Ѵ�.
////		}
////
////		int nTreeCount = 0 ;															// ����Ʈ Ʈ�� ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
////
////		nTreeCount = m_QuestTreeList.GetCount() ;												// ����Ʈ�� ��� ����Ʈ�� ī��Ʈ�� �޴´�.	
////
////		if( nTreeCount >= MAX_QUESTTREECOUNT )											// Ʈ�� ī��Ʈ�� �ִ� Ʈ�� �� �̻��̸�,
////		{
////			return ;
////		}
////		else																			// Ʈ�� ������ 5�� �̸��̸�,
////		{
////			QuestTree* pTree = new QuestTree ;											// Ʈ���� �����Ѵ�.
////
////			CQuestInfo* pQuestInfo = NULL ;												// ����Ʈ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////			pQuestInfo = QUESTMGR->GetQuestInfo(dwQuestIdx) ;							// ����Ʈ �Ŵ����� ����, ���ڷ� �Ѿ�� ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.
////
////			if( !pQuestInfo )															// ����Ʈ ������ ��ȿ���� �ʴٸ�,
////			{
////				ASSERTMSG(0, "����Ʈ �˸��� ��� �� ����Ʈ ������ �޾ƿ��� ���߽��ϴ�.!") ;		// assert ���� ó���� �Ѵ�.
////				return ;																// ���� ó���� �Ѵ�.
////			}
////
////			int nSubQuestCount = 0 ;													// ���� ����Ʈ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
////			nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// ����Ʈ ������ ���� ���� ����Ʈ ī��Ʈ�� �޴´�.
////
////			CQuestString* pQuestString ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����Ѵ�.
////			
////			for(int count = 0 ; count < nSubQuestCount ; ++count )						// ���� ����Ʈ ī��Ʈ ��ŭ for���� ������.
////			{
////				pQuestString = NULL ;													// ����Ʈ ��Ʈ�� ������ ���� �����͸� nulló���� �Ѵ�.
////				pQuestString = QUESTMGR->GetQuestString(dwQuestIdx, count) ;			// ����Ʈ �ε�����, ���� ����Ʈ �ε����� ����Ʈ ��Ʈ���� �޴´�.
////	            
////				if( pQuestString )														// ����Ʈ ��Ʈ�� ������ ��ȿ�ϴٸ�,
////				{
////					CQuestString* pNewString = new CQuestString ;						// ����Ʈ ��Ʈ���� �����Ѵ�.
////					memcpy(pNewString, pQuestString, sizeof(CQuestString)) ;			// ���� ������ ����Ʈ ��Ʈ����, ������ ����Ʈ ��Ʈ���� �����Ѵ�.
////
////					cPtrList* pList = pQuestString->GetTitle() ;						// ����Ʈ ��Ʈ���� Ÿ��Ʋ ����Ʈ�� �޴´�.
////
////					PTRLISTPOS titlePos = NULL ;										// Ÿ��Ʋ ����Ʈ ���� ��ġ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////					titlePos = pList->GetHeadPosition() ;								// Ÿ��Ʋ ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.
////
////					ITEM* pQString = NULL ;											// Q��Ʈ���� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
////
////					while(titlePos)														// ��ġ ������ ��ȿ�� ���� while���� ������.
////					{
////						pQString = NULL ;												// Q��Ʈ�� �����͸� nulló���� �Ѵ�.
////						pQString = (ITEM*)pList->GetNext(titlePos) ;					// ��ġ�� ���� Q��Ʈ���� �޴´�.
////
////						if( pQString )													// Q��Ʈ�� ������ ��ȿ�ϴٸ�,
////						{
////							pNewString->SetTitleStr( pQString->string ) ;					// Q��Ʈ���� ���ڿ��� �޾Ƽ� ���ο� ����Ʈ ��Ʈ���� Ÿ��Ʋ�� �����Ѵ�.
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
////					pTree->list.AddTail(pNewString) ;									// Ʈ���� ����Ʈ�� ����Ʈ ��Ʈ���� �߰��Ѵ�.
////
////				}
////			}
////
////			pTree->State = eQTree_Open ;												// Ʈ�� ���¸� ���� ���·� �����Ѵ�.
////			pTree->QuestIdx = dwQuestIdx ;												// Ʈ���� ����Ʈ �ε����� �����Ѵ�.
////			m_QuestTreeList.AddTail(pTree) ;														// ����Ʈ ����Ʈ�� Ʈ���� �߰��Ѵ�.
////
////			QUESTMGR->RegistQuestIDToQuickView(index, dwQuestIdx) ;									// �˸�â�� ��� �� ����Ʈ �ε����� ����Ʈ �Ŵ����� ���� �Ѵ�.
////		}
////
////		//pQuickViewDlg->ResetLoadedQuickViewDialog() ;									// �� �並 ���ΰ�ħ�Ѵ�.
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
