// QuestDialog.cpp: implementation of the CQuestDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"																	// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.

#include "WindowIDEnum.h"															// ������ ���̵� �̳� ����� �ҷ��´�.

#include "../Input/Mouse.h"															// ���콺 Ŭ���� ����� �ҷ��´�.

#include "GameIn.h"

#include "ChatManager.h"															// ä�� �Ŵ��� Ŭ���� ����� �ҷ��´�.
#include "ObjectManager.h"															// ������Ʈ �Ŵ��� Ŭ���� ����� �ҷ��´�.
#include "QuestManager.h"															// ����Ʈ �Ŵ��� Ŭ���� ����� �ҷ��´�.

#include "./Interface/cScriptManager.h"												// ��ũ��Ʈ �Ŵ��� Ŭ���� ����� �ҷ��´�.
#include "./Interface/cWindowManager.h"												// ������ �Ŵ��� Ŭ���� ����� �ҷ��´�.

#include "../ItemManager.h"

#include "NpcScriptManager.h"														// npc ��ũ��Ʈ �Ŵ��� ����� �ҷ��´�.

#include "./Interface/cFont.h"														// ��Ʈ Ŭ���� ����� �ҷ��´�.
#include "./Interface/cIcon.h"														// ������ Ŭ���� ����� �ҷ��´�.
#include "./Interface/cIconGridDialog.h"											// ������ �׸��� ���̾�α� Ŭ���� ����� �ҷ��´�.
#include "./Interface/cListDialog.h"												// ����Ʈ ���̾�α� Ŭ���� ����� �ҷ��´�.
#include "./Interface/cPushupButton.h"												// Ǫ�þ� ��ư Ŭ���� ����� �ҷ��´�.
#include "./Interface/cStatic.h"													// ����ƽ Ŭ���� ����� �ҷ��´�.

#include "cDialogueList.h"															// ���̾�α� ����Ʈ ����� �ҷ��´�.
#include "cMsgBox.h"																// �޽��� �ڽ� Ŭ���� ����� �ҷ��´�.
#include "cPage.h"																	// ������ Ŭ���� ����� �ҷ��´�.
#include "QuestDialog.h"															// ����Ʈ ���̾�α� Ŭ���� ����� �ҷ��´�.
#include "Quest.h"																	// ����Ʈ Ŭ���� ����� �ҷ��´�.

#include "..\[CC]Quest\QuestExecute.h"												// ����Ʈ ���� Ŭ���� ����� �ҷ��´�.
#include "..\[CC]Quest\QuestExecute_Item.h"											// ����Ʈ ���� ������ Ŭ���� ����� �ҷ��´�.
#include "..\[CC]Quest\QuestInfo.h"													// ����Ʈ ���� Ŭ���� ����� �ҷ��´�.
#include "..\[CC]Quest\QuestNpcScript.h"											// ����Ʈ npc ��ũ��Ʈ Ŭ���� ����� �ҷ��´�.
#include "..\[CC]Quest\QuestNpcScriptData.h"										// ����Ʈ npc ��ũ��Ʈ ������ Ŭ���� ����� �ҷ��´�.
#include "..\[CC]Quest\QuestString.h"												// ����Ʈ ��Ʈ�� Ŭ���� ����� �ҷ��´�.
#include "..\[CC]Quest\QuestTrigger.h"												// ����Ʈ Ʈ���� Ŭ���� ����� �ҷ��´�.
#include "..\[CC]Quest\SubQuestInfo.h"												// ���� ����Ʈ ���� Ŭ���� ����� �ҷ��´�.


#include "QuestQuickViewDialog.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestDialog::CQuestDialog()														// ������ �Լ�.
{
	m_type	= WT_QUESTDIALOG;														// ����Ʈ ���̾�α��� Ÿ���� WT_QUESTDIALOG�� �����Ѵ�.
	State	= eQView_QList;															// ���̾�α� ���¸� eQView_QList�� �����Ѵ�.
	
	m_SelectedQuestID	= 0;														// ���� �� ����Ʈ ���̵� -1�� �����Ѵ�.
	m_pMsgBox			= NULL;														// �޽��� �ڽ� ������ NULLó�� �Ѵ�.
	m_QTreeCount		= 0;														// ����Ʈ Ʈ�� ī��Ʈ�� 0���� �����Ѵ�.
	m_QListCount		= 0;														// ����Ʈ ����Ʈ ī��Ʈ�� 0���� �����Ѵ�.
	m_ChangedRow		= -1;														// ���� �� �� ��ȣ�� -1�� �����Ѵ�.
	m_PreSelectedIdx	= 0;														// ���� �� ���� �ε����� 0���� �����Ѵ�.
	m_ListLineIdx		= 0;														// ����Ʈ ���� �ε����� 0���� �����Ѵ�.
	m_QuestItemCount	= 0;														// ����Ʈ ������ ī��Ʈ�� 0���� �����Ѵ�.
	m_QuestItemPage		= 0;														// ����Ʈ ������ �������� 0���� �����Ѵ�.
	m_TooltipIdx		= -1;														// ���� �ε����� -1�� �����Ѵ�.
	m_RequitalItemTooltipIdx = -1 ;
	m_PreDescQuestString	= NULL;													// ���� ����Ʈ ��Ʈ�� ���� ������ NULLó�� �Ѵ�.

	memset(m_QuestList, 0, sizeof(CQuestString*)*MAX_PROCESSQUEST);					// ����Ʈ ����Ʈ�� �޸� ���Ѵ�.

	m_IndexGenerator.Init( 999, IG_QUESTITEM_STARTINDEX);							// ����Ʈ ������ ���� �ε����� �ʱ�ȭ �Ѵ�.

	m_pQuestCount		= NULL ;													// ����Ʈ �������� ����ϴ� ����ƽ �����͸� NULLó���Ѵ�.

	m_pNoticeBtn		= NULL ;													// ����Ʈ �˸��� ��ư �����͸� NULL ó�� �Ѵ�.
	m_pGiveupBtn		= NULL ;													// ����Ʈ ���� ��ư �����͸� NULLó�� �Ѵ�.

	m_pQuestDescList	= NULL ;													// ����Ʈ ���� ����Ʈ ���̾�α� �����͸� NULL ó�� �Ѵ�.

	m_pRequitalGrid		= NULL ;													// ����Ʈ ���� ������ ���̾�α� �����͸� NULL ó�� �Ѵ�.

	m_pRequitalGold		= NULL ;													// ��� ���� ����ƽ �����͸� NULL ó�� �Ѵ�.
	m_pExperience		= NULL ;													// ����ġ ���� ����ƽ �����͸� NULL ó�� �Ѵ�.

	m_nQuestCount = 0 ;

	m_dwNpcIdx = 0 ;
	m_dwScriptPage = 0 ;
}

CQuestDialog::~CQuestDialog()														// �Ҹ��� �Լ�.
{
	m_IndexGenerator.Release();														// �ε��� �����⸦ �����Ѵ�.

	{
		sQuestItem* pItem;

		PTRLISTPOS pos = NULL ;
		pos = m_QuestItemList.GetHeadPosition() ;

		while(pos)
		{
			pItem = NULL ;
			pItem = (sQuestItem*)m_QuestItemList.GetNext(pos) ;

			if( pItem )
			{
				m_QuestItemList.Remove(pItem) ;

				delete pItem ;
			}
		}

		m_QuestItemList.RemoveAll() ;
	}

	{
		cIcon* pDeleteIcon = NULL ;
		RequitalItem* pDeleteItem = NULL ;

		PTRLISTPOS deletePos = NULL ;
		deletePos = m_QuestRequitalList.GetHeadPosition() ;

		while(deletePos)
		{
			pDeleteItem = NULL ;
			pDeleteItem = (RequitalItem*)m_QuestRequitalList.GetNext(deletePos) ;

			if( pDeleteItem )
			{
				m_QuestRequitalList.Remove(pDeleteItem) ;

				delete pDeleteItem ;
			}
		}

		m_QuestRequitalList.RemoveAll() ;
	}

	{
		QuestTree* pTree ;
		DWORD* pQuestIdx ;

		PTRLISTPOS treePos ;
		PTRLISTPOS idPos ;

		treePos = NULL ;
		treePos = m_QuestTree.GetHeadPosition() ;

		while(treePos)
		{
			pTree = NULL ;
			pTree = (QuestTree*)m_QuestTree.GetNext(treePos) ;

			if( !pTree ) continue ;

			idPos = NULL ;
			idPos = pTree->list.GetHeadPosition() ;

			while(idPos)
			{
				pQuestIdx = NULL ;
				pQuestIdx = (DWORD*)pTree->list.GetNext(idPos) ;

				if( !pQuestIdx ) continue ;

				pTree->list.Remove(pQuestIdx) ;

				delete pQuestIdx ;
			}

			pTree->list.RemoveAll() ;

			m_QuestTree.Remove(pTree) ;

			delete pTree ;
			pTree = NULL ;
		}
		m_QuestTree.RemoveAll() ;
	}
}

//CQuestDialog::~CQuestDialog()														// �Ҹ��� �Լ�.
//{
//	m_IndexGenerator.Release();														// �ε��� �����⸦ �����Ѵ�.
//
//																					// ����Ʈ �������� �����Ѵ�
//	//{
//	//	PTRLISTPOS pos = m_QuestItemList.GetHeadPosition();							// ����Ʈ ������ ����Ʈ�� ���� �����Ѵ�.
//
//	//	sQuestItem* item;															// ����Ʈ ������ ������ ���� �����͸� �����Ѵ�.
//
//	//	while( item = ( sQuestItem* )m_QuestItemList.GetNext( pos ) )				// ����Ʈ ������ ������ ��ȿ�� ���� while���� ������.
//	//	{
//	//		SAFE_DELETE( item );													// ����Ʈ ������ ������ �����ϰ� �����Ѵ�.
//	//	}
//	//}
//
//	//{
//	//	/*PTRLISTPOS pos = m_QuestRequitalList.GetHeadPosition() ;
//
//	//	RequitalItem* item ;
//
//	//	while( item = ( RequitalItem* )m_QuestRequitalList.GetNext( pos ) )
//	//	{
//	//		SAFE_DELETE( item ) ;
//	//	}*/
//
//	//	PTRLISTPOS pos = m_QuestRequitalList.GetHeadPosition() ;
//
//	//	RequitalItem* item ;
//
//	//	while( pos )
//	//	{
//	//		item = NULL ;
//	//		item = ( RequitalItem* )m_QuestRequitalList.GetNext( pos ) ;
//
//	//		//WINDOWMGR->AddListDestroyWindow( item->pIcon );
//
//	//		m_QuestRequitalList.Remove(item) ;
//
//	//		//SAFE_DELETE( item ) ;
//	//	}
//
//	//	m_QuestRequitalList.RemoveAll() ;
//	//}
//
//	{
//		sQuestItem* pItem;
//
//		PTRLISTPOS pos = NULL ;
//		pos = m_QuestItemList.GetHeadPosition() ;
//
//		while(pos)
//		{
//			pItem = NULL ;
//			pItem = (sQuestItem*)m_QuestItemList.GetNext(pos) ;
//
//			if( pItem )
//			{
//				m_QuestItemList.Remove(pItem) ;
//
//				delete pItem ;
//			}
//		}
//
//		m_QuestItemList.RemoveAll() ;
//	}
//
//	{
//		cIcon* pDeleteIcon = NULL ;
//		RequitalItem* pDeleteItem = NULL ;
//
//		PTRLISTPOS deletePos = NULL ;
//		deletePos = m_QuestRequitalList.GetHeadPosition() ;
//
//		while(deletePos)
//		{
//			pDeleteItem = NULL ;
//			pDeleteItem = (RequitalItem*)m_QuestRequitalList.GetNext(deletePos) ;
//
//			if( pDeleteItem )
//			{
//				//m_pRequitalGrid->DeleteIcon( pDeleteItem->wPos, &pDeleteItem->pIcon ) ;
//
//				/*if( pDeleteItem->pIcon )
//				{
//					delete pDeleteItem->pIcon ;
//				}*/
//
//				/*pDeleteItem->pIcon->SetActive(FALSE) ;
//				WINDOWMGR->AddListDestroyWindow(pDeleteItem->pIcon) ;
//
//				m_IndexGenerator.ReleaseIndex( pDeleteItem->pIcon->GetID() ) ;*/
//
//				m_QuestRequitalList.Remove(pDeleteItem) ;
//
//				delete pDeleteItem ;
//			}
//		}
//
//		m_QuestRequitalList.RemoveAll() ;
//
//		//RefreshRequitalItem() ;
//	}
//
//																					// ����Ʈ Ʈ���� �����Ѵ�. �� Ʈ���� ���Ҹ��� Ʈ���� �� �޷��ִ�. �Բ� ����������
//	//{
//	//	PTRLISTPOS pos = m_QuestTree.GetHeadPosition();								// ����Ʈ Ʈ�� ����Ʈ�� ���� �����Ѵ�.
//
//	//	QuestTree* tree;															// ����Ʈ Ʈ�� ������ ���� �����͸� �����Ѵ�.
//
//	//	while( tree = ( QuestTree* )m_QuestTree.GetNext( pos ) )					// ����Ʈ Ʈ�� ������ ��ȿ�� ���� while���� ������.
//	//	{
//	//		PTRLISTPOS		innerPos = tree->list.GetHeadPosition();				// ����Ʈ Ʈ���� ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.
//
//	//		CQuestString*	text;													// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����Ѵ�.
//
//	//		while( text = ( CQuestString* )( tree->list.GetNext( innerPos ) ) )		// ����Ʈ ��Ʈ�� ������ ��ȿ�� ���� while���� ������.
//	//		{
//	//																				/// �� ���� �𸣰�����... GIVEUP_ID�� ���⼭ ����������, �ٸ� ID�� �ٸ� ������ �����ؼ� �ѱ��.
//	//			if( QUEST_GIVEUP_ID == text->GetSubID() )							// ���� ���̵� ����Ʈ ���� ���̵�� ������,
//	//			{
//	//				SAFE_DELETE( text );											// ��Ʈ�� ������ �����ϰ� �����Ѵ�.
//	//			}
//	//		}
//	//		
//	//		SAFE_DELETE( tree );													// �����ϰ� Ʈ�� ������ �����Ѵ�.
//	//	}
//	//}
//
//	/*{
//		QuestTree* pTree ;
//		CQuestString* pQuestString ;
//
//		PTRLISTPOS pos = NULL ;
//		PTRLISTPOS pInnerPos = NULL ;
//
//
//		pos = m_QuestTree.GetHeadPosition() ;
//		while( pos )
//		{
//			pTree = NULL ;
//			pTree = (QuestTree*)m_QuestTree.GetNext(pos) ;
//
//			if( pTree )
//			{
//				pInnerPos = NULL ;
//				pInnerPos = pTree->list.GetHeadPosition() ;
//
//				while(pInnerPos)
//				{
//					pQuestString = NULL ;
//					pQuestString = (CQuestString*)pTree->list.GetNext(pInnerPos) ;
//
//					if( pQuestString )
//					{
//						delete pQuestString ;
//					}
//				}
//
//				pTree->list.RemoveAll() ;
//
//				delete pTree ;
//			}
//		}
//
//		m_QuestTree.RemoveAll() ;
//	}*/
//
//	{
//		QuestTree* pTree ;
//		CQuestString* pQuestString ;
//
//		PTRLISTPOS treePos ;
//		PTRLISTPOS stringPos ;
//
//		treePos = NULL ;
//		treePos = m_QuestTree.GetHeadPosition() ;
//
//		while(treePos)
//		{
//			pTree = NULL ;
//			pTree = (QuestTree*)m_QuestTree.GetNext(treePos) ;
//
//			if( pTree )
//			{
//				stringPos = NULL ;
//				stringPos = pTree->list.GetHeadPosition() ;
//
//				while(stringPos)
//				{
//					pQuestString = NULL ;
//					pQuestString = (CQuestString*)pTree->list.GetNext(stringPos) ;
//
//					if( pQuestString )
//					{
//						pTree->list.Remove(pQuestString) ;
//
//						delete pQuestString ;
//						pQuestString = NULL ;
//					}
//				}
//
//				pTree->list.RemoveAll() ;
//
//				m_QuestTree.Remove(pTree) ;
//
//				delete pTree ;
//				pTree = NULL ;
//			}
//		}
//
//		m_QuestTree.RemoveAll() ;
//	}
//
//}

void CQuestDialog::DeleteTree(QuestTree* pTree)
{
	if(!pTree)
	{
		return ;
	}

	CQuestString* pDeleteString ;

	PTRLISTPOS pos = NULL ;
	pos = pTree->list.GetHeadPosition() ;

	while(pos)
	{
		pDeleteString = NULL ;
		pDeleteString = (CQuestString*)pTree->list.GetNext(pos) ;

		if( pDeleteString )
		{
			pTree->list.Remove(pDeleteString) ;

			delete pDeleteString ;
			pDeleteString = NULL ;
		}
	}

	pTree->list.RemoveAll() ;

	m_QuestTree.Remove(pTree) ;

	delete pTree ;

	pTree = NULL ;
}

void CQuestDialog::Linking()														// ���� ��Ʈ�ѵ��� ��ũ�ϴ� �Լ�.
{
	for(int i=0; i<MAX_QUEST_PAGEBTN; ++i)											// �ִ� ����Ʈ ������ �� ��ŭ for���� ������.
	{
		m_pPageBtn[i] = (cPushupButton*)GetWindowForID(QUE_PAGE1BTN+i);				// ����Ʈ ������ ������ ������ ��ũ�Ѵ�.
	}

	m_pPageBtn[0]->SetPush(TRUE);													// ù ������ ��ư�� ���� ���·� �����Ѵ�.
	
//	m_pTitleBtn = (cButton*)GetWindowForID(QUE_TITLEBTN);
	m_pQuestListDlg = (cListDialog*)GetWindowForID(QUE_QUESTLIST);					// ����Ʈ ����Ʈ ������ ��ũ�Ѵ�.
	m_pIconGridDlg = (cIconGridDialog*)GetWindowForID(QUE_ITEM_GRID);				// ����Ʈ �������� ������ �׸��� ������ ��ũ�Ѵ�.

	m_pQuestCount = (cStatic*)GetWindowForID(QUE_COUNTSTATIC) ;						// ����Ʈ �������� ����ϴ� ����ƽ ������ ��ũ�Ѵ�.

	m_pNoticeBtn = (cButton*)GetWindowForID(QUE_NOTICEBTN) ;						// ����Ʈ �˸��� ��ư ������ ��ũ�Ѵ�.
	m_pGiveupBtn = (cButton*)GetWindowForID(QUE_GIVEUPBTN) ;						// ����Ʈ ���� ��ư ������ ��ũ�Ѵ�.

	m_pQuestDescList	= (cListDialog*)GetWindowForID(QUE_DESCDLG) ;				// ����Ʈ ���� ����Ʈ ���̾�α� ������ ��ũ�Ѵ�.

	m_pRequitalGrid		= (cIconGridDialog*)GetWindowForID(QUE_REQUITAL_GRID) ;		// ����Ʈ ���� ������ ���̾�α� ������ ��ũ�Ѵ�.

	m_pRequitalGold		= (cStatic*)GetWindowForID(QUE_REQUITAL_GOLD) ;				// ��� ���� ����ƽ �����͸� ������ ��ũ�Ѵ�.
	m_pExperience		= (cStatic*)GetWindowForID(QUE_REQUITAL_EXP) ;				// ����ġ ���� ����ƽ �����͸� ������ ��ũ�Ѵ�.
}

DWORD CQuestDialog::ActionEvent(CMouse* mouseInfo)										// ����Ʈ ���̾�α��� �̺�Ʈ�� ó���ϴ� �Լ�.
{
	DWORD we = WE_NULL;																	// �̺�Ʈ�� ���� ������ �����ϰ� NULL ó���� �Ѵ�.

	if( !m_bActive )																	// ����Ʈ ���̾�αװ� �����ִ� ���°� �ƴ϶��, 
	{
		return we;																		// �̺�Ʈ�� �����Ѵ�.
	}

	we = cDialog::ActionEvent(mouseInfo);												// ���̾�α��� �̺�Ʈ�� �޴´�.

	if(m_pMsgBox)																		// �޽��� �ڽ��� ����������,
	{
		return we;																		// �̺�Ʈ�� �����Ѵ�.
	}

	int nMouseX = mouseInfo->GetMouseX() ;												// �̺�Ʈ�� �߻��ϴ� ���콺 X��ǥ�� �޴´�.
	int nMouseY = mouseInfo->GetMouseY() ;												// �̺�Ʈ�� �߻��ϴ� ���콺 Y��ǥ�� �޴´�.
    																					// Show QuestItem Tooltip
	if(m_pIconGridDlg->PtInWindow(nMouseX,nMouseY))										// ���콺�� ������ �׸��� �ȿ� ������,
	{
		WORD Pos = -1;																	// ��ġ ������ �����ϰ� -1�� �����Ѵ�.

		if(m_pIconGridDlg->GetPositionForXYRef(nMouseX,nMouseY,Pos))					// ������ �׸��� �󿡼� ��ġ�� �޴´�. ��ġ�� ��ȿ�ϸ�,
		{
			WORD RealPos = Pos + m_QuestItemPage*VIEW_QUESTITEM_PERPAGE;				// �������� �����Ͽ� ���� ��ġ�� �޴´�.

			if(m_TooltipIdx != RealPos)													// ���� �ε����� �������� ���� ������,
			{
				ShowItemToolTip(RealPos);												// ���� ��ġ�� ������ �����ش�.
			}

			return 0;																	// 0�� �����Ѵ�.
		}
	}

	if( m_pRequitalGrid->PtInWindow(nMouseX,nMouseY))
	{
		WORD Pos = -1;																	// ��ġ ������ �����ϰ� -1�� �����Ѵ�.

		if(m_pRequitalGrid->GetPositionForXYRef(nMouseX,nMouseY,Pos))					// ������ �׸��� �󿡼� ��ġ�� �޴´�. ��ġ�� ��ȿ�ϸ�,
		{
			WORD RealPos = Pos ;														// �������� �����Ͽ� ���� ��ġ�� �޴´�.

			ShowRequitalItemToolTip(RealPos);											// ���� ��ġ�� ������ �����ش�.
			return 0;																	// 0�� �����Ѵ�.
		}
	}

	if( m_pQuestListDlg->PtInWindow(nMouseX, nMouseY) )									// ���콺�� ����Ʈ ����Ʈ �ȿ� ������,
	{
		int nSelectedLine = -1 ;														// ����Ʈ���� ���� �� ������ ���� ������ �����ϰ� -1�� �����Ѵ�.

		nSelectedLine = m_pQuestListDlg->GetCurSelectedRowIdx() ;						// ����Ʈ ����Ʈ���� ���� �� ������ �޴´�.

		m_ChangedRow = nSelectedLine ;													// ������ �ٲ� ���ι�ȣ�� �����Ѵ�.

		ITEM* pItem = NULL ;
		pItem = m_pQuestListDlg->GetItem(nSelectedLine) ;

		if( !pItem ) return we ;

		//if( pItem->rgb == RGBA_MAKE(200, 128, 0, 255) )									// ���� �� ���� ������ �� Ʈ�� �����̸�,
		if(pItem->nFontIdx == cFont::FONT1)
		{
			if( we & WE_LBTNDBLCLICK )													// ���� Ŭ���� �ߴٸ�,
			{
				QuestTreeStateCheck(nSelectedLine) ;
			}
		}
		else
		{
			if( nSelectedLine != -1 )													// ���� �� ������ -1�� ���� ������,
			{
				DWORD QuestIdx = NULL ;
			
				QuestIdx = m_dwQuestList[nSelectedLine] ;								// ���� �� ������ ����Ʈ ��Ʈ���� �޴´�.

				if( QuestIdx != 0xFFFFFFFF )											// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
				{				
					if( we & WE_LBTNCLICK )												// Ŭ���� �ߴٸ�,
					{
						m_SelectedQuestID = QuestIdx ;									// ���� �� ����Ʈ �ε����� �����Ѵ�.

						RefreshQuestList() ;											// ����Ʈ ����Ʈ�� ���ΰ�ħ�Ѵ�.

						CQuestString* pQuestString = NULL ;
						pQuestString = QUESTMGR->GetQuestString(QuestIdx, 0) ;

						QuestDescView(pQuestString);									// ����Ʈ ������ �����ش�.
					}
				}
			}
		}
	}

	return we;
}

//DWORD CQuestDialog::ActionEvent(CMouse* mouseInfo)										// ����Ʈ ���̾�α��� �̺�Ʈ�� ó���ϴ� �Լ�.
//{
//	DWORD we = WE_NULL;																	// �̺�Ʈ�� ���� ������ �����ϰ� NULL ó���� �Ѵ�.
//
//	if( !m_bActive )																	// ����Ʈ ���̾�αװ� �����ִ� ���°� �ƴ϶��, 
//	{
//		return we;																		// �̺�Ʈ�� �����Ѵ�.
//	}
//
//	we = cDialog::ActionEvent(mouseInfo);												// ���̾�α��� �̺�Ʈ�� �޴´�.
//
//	if(m_pMsgBox)																		// �޽��� �ڽ��� ����������,
//	{
//		return we;																		// �̺�Ʈ�� �����Ѵ�.
//	}
//
//	int nMouseX = mouseInfo->GetMouseX() ;												// �̺�Ʈ�� �߻��ϴ� ���콺 X��ǥ�� �޴´�.
//	int nMouseY = mouseInfo->GetMouseY() ;												// �̺�Ʈ�� �߻��ϴ� ���콺 Y��ǥ�� �޴´�.
//    																					// Show QuestItem Tooltip
//	if(m_pIconGridDlg->PtInWindow(nMouseX,nMouseY))										// ���콺�� ������ �׸��� �ȿ� ������,
//	{
//		WORD Pos = -1;																	// ��ġ ������ �����ϰ� -1�� �����Ѵ�.
//
//		if(m_pIconGridDlg->GetPositionForXYRef(nMouseX,nMouseY,Pos))					// ������ �׸��� �󿡼� ��ġ�� �޴´�. ��ġ�� ��ȿ�ϸ�,
//		{
//			WORD RealPos = Pos + m_QuestItemPage*VIEW_QUESTITEM_PERPAGE;				// �������� �����Ͽ� ���� ��ġ�� �޴´�.
//
//			if(m_TooltipIdx != RealPos)													// ���� �ε����� �������� ���� ������,
//			{
//				ShowItemToolTip(RealPos);												// ���� ��ġ�� ������ �����ش�.
//			}
//
//			return 0;																	// 0�� �����Ѵ�.
//		}
//	}
//
//	if( m_pRequitalGrid->PtInWindow(nMouseX,nMouseY))
//	{
//		WORD Pos = -1;																	// ��ġ ������ �����ϰ� -1�� �����Ѵ�.
//
//		if(m_pRequitalGrid->GetPositionForXYRef(nMouseX,nMouseY,Pos))					// ������ �׸��� �󿡼� ��ġ�� �޴´�. ��ġ�� ��ȿ�ϸ�,
//		{
//			WORD RealPos = Pos ;														// �������� �����Ͽ� ���� ��ġ�� �޴´�.
//
//			ShowRequitalItemToolTip(RealPos);										// ���� ��ġ�� ������ �����ش�.
//
//			//if(m_RequitalItemTooltipIdx == RealPos)										// ���� �ε����� �������� ���� ������,
//			//{
//			//	ShowRequitalItemToolTip(RealPos);										// ���� ��ġ�� ������ �����ش�.
//			//}
//
//			return 0;																	// 0�� �����Ѵ�.
//		}
//	}
//
//	if( m_pQuestListDlg->PtInWindow(nMouseX, nMouseY) )									// ���콺�� ����Ʈ ����Ʈ �ȿ� ������,
//	{
//		int nSelectedLine = -1 ;														// ����Ʈ���� ���� �� ������ ���� ������ �����ϰ� -1�� �����Ѵ�.
//
//		nSelectedLine = m_pQuestListDlg->GetCurSelectedRowIdx() ;						// ����Ʈ ����Ʈ���� ���� �� ������ �޴´�.
//
//		m_ChangedRow = nSelectedLine ;													// ������ �ٲ� ���ι�ȣ�� �����Ѵ�.
//
//		if( nSelectedLine != -1 )														// ���� �� ������ -1�� ���� ������,
//		{
//			CQuestString* pQuestString = NULL ;											// ����Ʈ ��Ʈ���� ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.
//		
//			pQuestString = m_QuestList[nSelectedLine] ;									// ���� �� ������ ����Ʈ ��Ʈ���� �޴´�.
//
//			if( pQuestString )															// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
//			{
//				if( pQuestString->IsTreeTitle() )										// ����Ʈ �������� üũ�Ѵ�. �� Ÿ��Ʋ�̶��,
//				{
//					if( we & WE_LBTNDBLCLICK )											// ���� Ŭ���� �ߴٸ�,
//					{
//						QuestTreeStateCheck(nSelectedLine) ;
//					}
//				}
//				else																	// ����Ʈ Ÿ��Ʋ �̶��,
//				{
//					if( we & WE_LBTNCLICK )												// Ŭ���� �ߴٸ�,
//					{
//						m_SelectedQuestID = pQuestString->GetQuestID() ;				// ���� �� ����Ʈ �ε����� �����Ѵ�.
//
//						QuestListReset() ;												// ����Ʈ ����Ʈ�� ���ΰ�ħ�Ѵ�.
//
//						QuestDescView(pQuestString);									// ����Ʈ ������ �����ش�.
//
//						m_pQuestDescList->ResetGuageBarPos() ;
//					}
//				}
//			}
//		}
//	}
//
//	return we;
//}

void CQuestDialog::SetQuestCount()
{
	char tempBuf[128] = {0, } ;

	DWORD dwColor = RGBA_MAKE(255, 255, 255, 255) ;

	int nCount = 0 ;

	QuestTree* pTree ;
	DWORD* pQuestIdx ;

	PTRLISTPOS treePos ;
	PTRLISTPOS idPos ;

	treePos = NULL ;
	treePos = m_QuestTree.GetHeadPosition() ;

	while(treePos)
	{
		pTree = NULL ;
		pTree = (QuestTree*)m_QuestTree.GetNext(treePos) ;

		if( !pTree ) continue ;

		idPos = NULL ;
		idPos = pTree->list.GetHeadPosition() ;

		while(idPos)
		{
			pQuestIdx = NULL ;
			pQuestIdx = (DWORD*)pTree->list.GetNext(idPos) ;

			if( !pQuestIdx ) continue ;

			++nCount ;
		}
	}

	if( nCount > 20 )
	{
		dwColor = RGBA_MAKE(255, 0, 0, 255) ;
	}

	sprintf(tempBuf, "(%d/20)", nCount) ;

	m_pQuestCount->SetStaticText(tempBuf) ;
	m_pQuestCount->SetFGColor(dwColor) ;

	m_nQuestCount = nCount ;
}

void CQuestDialog::SetGold(DWORD dwGold) 
{
	char tempBuf[128] = {0, } ;

	sprintf(tempBuf, "%d", dwGold) ;

	m_pRequitalGold->SetStaticText(tempBuf) ;
}

void CQuestDialog::SetExp(DWORD dwExp)
{
	char tempBuf[128] = {0, } ;

	sprintf(tempBuf, "%d", dwExp) ;

	m_pExperience->SetStaticText(tempBuf) ;
}

//void CQuestDialog::QuestAbandon()
//{
//	// ��Ʈ������ ����Ʈ �ε����� �޴´�.
//	CQuestString* pQuestString = NULL ;													// ����Ʈ ��Ʈ���� ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.
//		
//	pQuestString = m_QuestList[m_dwSeletedQuestIdx] ;									// ���� �� ������ ����Ʈ ��Ʈ���� �޴´�.
//
//	DWORD QuestIdx = 0 ;
//
//	if( pQuestString )
//	{
//		QuestIdx = pQuestString->GetQuestID() ;
//	}
//
//	if( QUESTMGR->IsQuestComplete(QuestIdx) )											// ����Ʈ �ε����� �Ϸ��� ����Ʈ���� üũ�Ѵ�.
//	{
//		return;																			// �Ϸ��� ����Ʈ���, ����ó���� �Ѵ�.
//	}
//
//	SEND_QUEST_IDX msg;																	// ����Ʈ ������ ���� ����ü�� �����Ѵ�.
//
//	msg.Category = MP_QUEST;															// ī�װ��� ����Ʈ�� �����Ѵ�.
//	msg.Protocol = MP_QUEST_DELETE_SYN;													// ���������� ����Ʈ �輼 ��ũ�� �����Ѵ�.
//	msg.dwObjectID = HERO->GetID();														// HERO ���̵� �����Ѵ�.
//	msg.MainQuestIdx = (WORD)QuestIdx;													// ����Ʈ �ε����� �����Ѵ�.
//
//	NETWORK->Send(&msg, sizeof(msg));													// ���õ� ������ �����Ѵ�.
//
//	// ����Ʈ���� ����Ʈ ���� ó���� �Ѵ�.
//
//	//test
//	//m_QuestUpdater->DeleteQuest(QuestIdx);															// ����Ʈ ���������� ����Ʈ �����Լ��� ȣ���Ѵ�.
//}

//DWORD CQuestDialog::ActionEvent(CMouse* mouseInfo)									// �̺�Ʈ�� ó���ϴ� �Լ�.
//{
//	DWORD we = WE_NULL;																// ������ �̺�Ʈ ������ NULL�� �����Ѵ�.
//
//	if( !m_bActive )		return we;												// ����Ʈ ���̾�αװ� �������� ������, ���� ó���� �Ѵ�.
//
//	we = cDialog::ActionEvent(mouseInfo);											// ����Ʈ ���̾�α׿��� �߻��� �̺�Ʈ�� �޴´�.
//
//	if(m_pMsgBox)			return we;												// �޽��� �ڽ��� Ȱ��ȭ �Ǿ� �ִٸ�, ���� ó���� �Ѵ�.
//
//	int nMouseX = mouseInfo->GetMouseX() ;											// ���콺 X��ǥ�� �޴´�.
//	int nMouseY = mouseInfo->GetMouseY() ;											// ���콺 Y��ǥ�� �޴´�.
//																					// Show QuestItem Tooltip
//	if(m_pIconGridDlg->PtInWindow(nMouseX,nMouseY))									// ���콺�� ������ �׸��� ���� ���̶��,
//	{
//		WORD Pos = -1;																// ��ġ�� ��� ������ -1�� �����Ѵ�.(-���� �ȵ��µ� �Ѥ�;)
//
//		if(m_pIconGridDlg->GetPositionForXYRef(nMouseX,nMouseY,Pos))				// ������ �׸����� ���° ĭ�� �ִ��� üũ�Ͽ� �����ϸ�,
//		{
//			WORD RealPos = Pos + m_QuestItemPage*VIEW_QUESTITEM_PERPAGE;			// ���� ��ġ�� �������� �����Ͽ� �����Ѵ�.
//
//			if(m_TooltipIdx != RealPos)												// ���� �ε����� ���� ��ġ�� ���� ������,
//			{
//				ShowItemToolTip(RealPos);											// ������ ������ �����ش�.
//			}
//
//			return 0;																// ���� ó���� �Ѵ�.
//		}
//	}
//
//	if(m_pQuestListDlg->PtIdxInRow(nMouseX,nMouseY) == -1)							// ����Ʈ ����Ʈ���� ���� �� ���� ������,
//	{
//		m_ChangedRow = -1;															// ���� �� �� ��ȣ�� -1�� �����Ѵ�.
//
//		if(m_PreSelectedIdx > -1)													// ���õȰ� ������ ȭ�� ������ ������ ������ҽ����ش�.
//		{
//			if(State == eQView_QList)												// ����Ʈâ ���°� ����Ʈ ����Ʈ ���� ���� ���¶��,
//			{
//				QuestListReset();													// ����Ʈ ����Ʈ�� ���ΰ�ħ �Ѵ�.
//			}
//			else if(State == eQView_QDesc)											// ����Ʈâ ���°� ����Ʈ ������ ���� ���¶��,
//			{
//				QuestDescView(m_PreDescQuestString);								// ����Ʈ ������ �����ش�.
//			}
//
//			m_PreSelectedIdx = -1;													// ���� �� �ε����� -1�� �����Ѵ�.
//		}
//		return 0;																	// ���� ó���� �Ѵ�.
//	}
//	
//	int Idx = m_pQuestListDlg->GetSelectRowIdx() + m_pQuestListDlg->GetTopListItemIdx();	// ����Ʈ���� ���� �� �ٰ�, ž ����Ʈ ������ �ε����� �ε����� ��´�.
//
//	m_ChangedRow = Idx;																// ����� �� ��ȣ�� �ε����� �����Ѵ�.
//
//	if(State==eQView_QList)															// ����Ʈâ ���°� ����Ʈ�� ���� ���¶��,
//	{	
//		if(we & WE_LBTNCLICK)														// ���콺 ���� ��ư�� �����ٸ�,
//		{
//			QuestTreeStateCheck(Idx);												// �ε����� ����Ʈ Ʈ�� ���¸� üũ�Ѵ�.
//		}
//		else if(m_PreSelectedIdx != m_ChangedRow)									// ������ ���� �� �ε�����, ���� �� �� ��ȣ�� �ٸ���,
//		{
//			QuestListReset();														// ����Ʈ ����Ʈ�� ���ΰ�ħ�Ѵ�.
//			m_PreSelectedIdx = m_ChangedRow;										// ���� �� �� ��ȣ�� ������ ���� �� �ε����� �����Ѵ�.
//		}
//	}
//	else if(State==eQView_QDesc)													// ����Ʈâ ���°� ������ ���� �ִ� ���¶��,
//	{
//		if(we & WE_LBTNCLICK && m_toPreList == m_ChangedRow)						// ���� ��ư�� Ŭ�� �߰�, ���� ����Ʈ�� ���� �� �� ��ȣ�� ������,
//		{
//			QuestListReset();														// ����Ʈ ����Ʈ�� ���ΰ�ħ�Ѵ�.
//		}
//		else if(m_PreSelectedIdx != m_ChangedRow)									// ������ ���� �� �ε����� ���� �� �� ��ȣ�� ���� ������,
//		{
//			QuestDescView(m_PreDescQuestString);									// ����Ʈ ������ �����ش�.
//			m_PreSelectedIdx = m_ChangedRow;										// ���� ���õ� �ε����� ���� �� �� ��ȣ�� �����Ѵ�.
//		}
//	}	
//
//	return we;																		// �̺�Ʈ�� �����Ѵ�.
//}


void CQuestDialog::Render()															// ���� �Լ�.
{
	cDialog::Render();																// ���̾�α׸� �����Ѵ�.

	int Count = 0;																	// ī��Ʈ�� �����ϰ� 0���� �����Ѵ�.
	sQuestItem* pSQuest = NULL;														// ����Ʈ ������ �����͸� �����ϰ� NULLó���� �Ѵ�.
	int	nStart = m_QuestItemPage*VIEW_QUESTITEM_PERPAGE;							// ��ŸƮ �ε����� �����Ѵ�.
	
	PTRLISTSEARCHSTART(m_QuestItemList, sQuestItem*, pSQuest);						// ����Ʈ ������ ����Ʈ���� ����Ʈ ������ ������ ã�´�.
		if(!pSQuest)				break;											// ����Ʈ ������ ������ ��ȿ���� ������, ����������.

		if(Count >= nStart)															// ī��Ʈ�� ��ŸƮ �ε��� �̻��̸�,
		{
																					// ���� ��ġ ��Ƽ� ��´�.
			if(pSQuest->Count > 1)													// ����Ʈ �������� �ϳ� �̻��̸�,
			{
				static char nums[4];
				int absX=0;	int absY=0;												// ��� ��ġ ������ �����ϰ� 0���� �����Ѵ�.

				wsprintf(nums,"%3d", pSQuest->Count);								// ���ڸ� �� ���� ������ ������ �����Ѵ�.

				if(m_pIconGridDlg->GetCellAbsPos(pSQuest->Pos, absX, absY))			// ������ �׸��忡�� ��ġ ������ ��ȿ�ϸ�,
				{
					// 070209 LYW --- QuestDialog : Modified render part.
					m_renderRect.left	= (LONG)absX+16 ;							// ��� ��ġ�� �����Ѵ�.
					m_renderRect.top	= (LONG)absY+24 ;
					m_renderRect.right	= 1 ;
					m_renderRect.bottom	= 1 ;
																					// ����Ʈ ������ ������ ����Ѵ�.
					CFONT_OBJ->RenderFontShadow(0, 1, nums,strlen(nums),&m_renderRect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100 ));
				}
			}
		}
	
		++Count;																	// ī��Ʈ�� ������Ų��.

		if(Count >= nStart+VIEW_QUESTITEM_PERPAGE)		break;						// ī��Ʈ�� ������ �� �������� ������ ���� �Ѿ��, �������´�.

	PTRLISTSEARCHEND																// ����Ʈ �˻��� ��ģ��.


	int Count2 = 0;																	// ī��Ʈ�� �����ϰ� 0���� �����Ѵ�.
	RequitalItem* pSQuest2 = NULL;													// ����Ʈ ������ �����͸� �����ϰ� NULLó���� �Ѵ�.
	int	nStart2 = 0*MAX_REQUITAL_ICON_CELL;											// ��ŸƮ �ε����� �����Ѵ�.

	PTRLISTSEARCHSTART(m_QuestRequitalList, RequitalItem*, pSQuest2) ;
		if( !pSQuest2 )			break ;

		if(Count2 >= nStart2)
		{
			if( pSQuest2->nItemCount > 1 )
			{
				static char nums[4] ;
				int absX=0; int absY=0;

				wsprintf(nums, "%3d", pSQuest2->nItemCount) ;

				if( m_pRequitalGrid->GetCellAbsPos(pSQuest2->wPos, absX, absY))
				{
					// 070209 LYW --- QuestDialog : Modified render part.
					m_renderRect.left	= (LONG)absX+16 ;							// ��� ��ġ�� �����Ѵ�.
					m_renderRect.top	= (LONG)absY+24 ;
					m_renderRect.right	= 1 ;
					m_renderRect.bottom	= 1 ;
																					// ����Ʈ ������ ������ ����Ѵ�.
					CFONT_OBJ->RenderFontShadow(0, 1, nums,strlen(nums),&m_renderRect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100 ));
				}
			}
		}

		++Count2;																	// ī��Ʈ�� ������Ų��.

		if(Count2 >= nStart2 + MAX_REQUITAL_ICON_CELL)		break;					// ī��Ʈ�� ������ �� �������� ������ ���� �Ѿ��, �������´�.
	PTRLISTSEARCHEND
}

// ������������������������������������������������������������
// ��														 ��
// ��	QuestList											 ��
// ��														 ��
// ������������������������������������������������������������
//void CQuestDialog::MainQuestAdd(CQuestString* pQuestString, int nMapNum)			// ���� ����Ʈ �߰� �Լ�.
//{
//	//if(!pQuestString)		return;													// ����Ʈ ��Ʈ�� ������ ��ȿ���� ������, ���� ó���� �Ѵ�.
//
//	//int nLine = 0 ;
//
//	//QuestTree* pTree = NULL ;														// Ʈ�� ������ ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.
//
//	//PTRLISTPOS pos = NULL ;															// ����Ʈ Ʈ�� ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.
//
//	//int nTreeCount = m_QuestTree.GetCount() ;										// ����Ʈ Ʈ���� ������ ���� ���Ѵ�.
//
//	//BOOL bAdded = FALSE ;															// �߰� ���� ������ FALSE�� �����Ѵ�.
//
//	//for(int count =0 ; count < nTreeCount ; ++count )								// Ʈ�� ī��Ʈ �� ��ŭ for���� ������.
//	//{
//	//	pos = m_QuestTree.FindIndex(count) ;										// ī��Ʈ�� �ش��ϴ� ��ġ ������ �޴´�.
//
//	//	if( pos )																	// ��ġ ������ ��ȿ���� üũ�Ѵ�.
//	//	{
//	//		pTree = (QuestTree*)m_QuestTree.GetAt(pos) ;							// ��ġ�� �ش��ϴ� ����Ʈ Ʈ�� ������ �޴´�.
//
//	//		if( pTree )																// ����Ʈ Ʈ�� ������ ��ȿ���� üũ�Ѵ�.
//	//		{
//	//			if( pTree->nQuestMapNum == nMapNum )								// ����Ʈ Ʈ���� �� ��ȣ��, ���ڷ� �Ѿ�� �� ��ȣ�� ������ üũ�Ѵ�.
//	//			{
//	//				bAdded = TRUE ;													// �߰� ���θ� TRUE�� �����Ѵ�.
//	//				
//	//				pQuestString->SetKey(pTree->Count) ;							// ��Ʈ�� �ε����� �����Ѵ�.
//	//				pTree->list.AddTail(pQuestString) ;								// ����Ʈ ��Ʈ���� �߰��Ѵ�.
//	//				++pTree->Count;													// ����Ʈ Ʈ���� ī��Ʈ�� ������Ų��.
//
//	//				break ;
//	//			}
//	//		}
//	//	}
//	//}
//
//	//if( !bAdded )																	// ����Ʈ ��Ʈ���� �߰����� �ʾ�����,
//	//{
//	//	QuestTree* pData = new QuestTree;											// ����Ʈ Ʈ�� �޸𸮸� �Ҵ��Ѵ�.
//	//	pData->State = eQTree_Close;												// ����Ʈ Ʈ���� ������ ���·� �����Ѵ�.
//	//	pData->QuestIdx = nTreeCount ;												// ����Ʈ �ε����� �����Ѵ�.
//	//	pQuestString->SetKey(1);													// �� �̸� �ε����� �����Ѵ�.
//	//	pData->list.AddHead(pQuestString) ;											// ����Ʈ ��Ʈ���� �߰��Ѵ�.
//	//	++pData->Count;																// ����Ʈ Ʈ���� ī��Ʈ�� ������Ų��.
//
//	//	char* pMapName = GetMapName(nMapNum) ;										// �� ��ȣ�� ���� �� �̸��� �޴´�.	
//
//	//	CQuestString* pQSMapName = new CQuestString;								// ����Ʈ ���� �޸𸮸� �Ҵ��Ѵ�.
//	//	pQSMapName->SetIndex(0, 0);													// �� �̸� �ε����� �����Ѵ�.
//	//	pQSMapName->SetKey(0);														// �� �̸� �ε����� �����Ѵ�.
//	//	pQSMapName->AddLine(pMapName, nLine, TRUE) ;								// ��Ʈ���� �� �̸��� �����Ѵ�.
//
//	//	pData->list.AddHead(pQSMapName) ;											// �� �̸��� ����Ʈ�� �߰��Ѵ�.
//	//	++pData->Count;																// ����Ʈ Ʈ���� ī��Ʈ�� ������Ų��.
//
//	//	pData->nQuestMapNum = nMapNum ;												// �� ��ȣ�� �����Ѵ�.
//
//	//	m_QuestTree.AddTail(pData);													// ����Ʈ Ʈ�� ����Ʈ�� ����Ʈ Ʈ���� �߰��Ѵ�.
//	//	++m_QTreeCount;																// ����Ʈ Ʈ�� ī��Ʈ�� �����Ѵ�.
//	//}
//}

void CQuestDialog::SubQuestAdd(CQuestString* pQuestString)							// ���� ����Ʈ �߰� �Լ�.
{
	if(!pQuestString)		return;													// ����Ʈ ��Ʈ�� ������ ��ȿ���� ������, ����ó���� �Ѵ�.

	QuestTree* pData = new QuestTree;												// ����Ʈ Ʈ�� �޸𸮸� �Ҵ��Ѵ�.
	pData->QuestIdx = pQuestString->GetQuestID();									// ����Ʈ �ε����� �����Ѵ�.
	pData->State = eQTree_Close;													// ����Ʈ Ʈ�� ���¸� ������ ���·� �����Ѵ�.
	pData->list.AddHead(pQuestString);												// ����Ʈ ��Ʈ���� Ʈ�� ����Ʈ�� �߰��Ѵ�.
	++pData->Count;																	// ����Ʈ Ʈ���� ī��Ʈ�� �����Ѵ�.
	
	m_QuestTree.AddTail(pData);														// ����Ʈ Ʈ�� ����Ʈ�� Ʈ�� ������ �߰��Ѵ�.
}

/*
BOOL CQuestDialog::CheckMainQuestList(CQuestString* pQuestString)
{
	if(pQuestString->GetSubID() != 0)			return FALSE;
	QuestTree* pData = NULL;
	CQuestString* pQuestStringList = NULL;


	PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pData);
		if(pData->QuestIdx == pQuestString->GetQuestID())
		{
			PTRLISTSEARCHSTART(pData->list, CQuestString*, pQuestStringList);
				if(pQuestStringList->GetSubID() == 0)
					return FALSE;
			PTRLISTSEARCHEND
		}		
	PTRLISTSEARCHEND

	return TRUE;
}
*/

void CQuestDialog::ProcessQuestAdd(CQuestString* pQuestString, int nMapNum)			// ����Ʈ �߰� ���μ���.
{
	if(!pQuestString)		return;													// ����Ʈ ������ ��ȿ���� ������, ���� ó���� �Ѵ�.

	QuestTree* pData = NULL;														// ����Ʈ Ʈ�� ������ ���� �����͸� �����ϰ� NULLó�� �Ѵ�.
	CQuestString* pQuestGiveup = NULL;												// ����Ʈ ���� ��Ʈ�� ������ ���� �����͸� �����ϰ� NULLó�� �Ѵ�.
	CQuestString* pQuestStringList = NULL;											// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� NULLó�� �Ѵ�.

	BOOL bQuestAdd = FALSE;															// ����Ʈ �߰� ���θ� FALSE�� �����Ѵ�.

																					// MainQuest Add
	if(pQuestString->GetSubID() == 0)												// ���� ����Ʈ ���̵� 0�� ������,
	{
		PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pData);							// ����Ʈ Ʈ�� ����Ʈ �˻��� �����Ѵ�.
			if(!pData)		break;													// ����Ʈ Ʈ�� ������ ��ȿ���� ������, �˻��� ����������.
			
																					// ��������Ʈ�� ���߿� �ε�������
			if(pData->QuestIdx == pQuestString->GetQuestID())						// ����Ʈ �ε����� ��Ʈ���� �ε����� �������,
			{
				PTRLISTSEARCHSTART(pData->list, CQuestString*, pQuestStringList);	// ����Ʈ Ʈ���� �ε������� �˻��� �����Ѵ�.
					if(pQuestStringList->GetSubID() == 0)							// ����Ʈ ��Ʈ�� ����Ʈ�� ���� ����Ʈ ���̱Ⱑ 0�� ������,
						return;														// �˻��� ����������.
				PTRLISTSEARCHEND													// �˻��� �����Ѵ�.

				pData->State = eQTree_Close;										// ����Ʈ Ʈ�� ���¸� ������ ���·� �����Ѵ�.
				pData->list.AddHead(pQuestString);									// ����Ʈ Ʈ�� ������ ����Ʈ ��Ʈ�� ������ �߰��Ѵ�.
				++pData->Count;														// ����Ʈ Ʈ�� ������ ī��Ʈ�� �����Ѵ�.
				
				// Quest Giveup Add
				pQuestGiveup = new CQuestString;									// ����Ʈ ���� ���� ��Ʈ�� ���� �޸𸮸� �Ҵ��Ѵ�.
				pQuestGiveup->SetIndex(pQuestString->GetQuestID(), QUEST_GIVEUP_ID);// ����Ʈ ������ �ε����� �����Ѵ�.
				pData->list.AddTail(pQuestGiveup);									// ����Ʈ Ʈ�� ���� ����Ʈ�� ���� ��Ʈ���� �߰��Ѵ�.
				++pData->Count;														// ����Ʈ Ʈ�� ������ ī��Ʈ�� �����Ѵ�.

				++m_QTreeCount;														// ����Ʈ Ʈ�� ī��Ʈ�� �����Ѵ�.
				bQuestAdd = TRUE;													// ����Ʈ �߰� ���θ� TRUE�� �����Ѵ�.
				break;																// �˻��� ����������.	
			}

		PTRLISTSEARCHEND															// �˻��� ���� �Ѵ�.

		//if(!bQuestAdd)																// ����Ʈ �߰� ���ΰ� FALSE���,
		//{
		//	MainQuestAdd(pQuestString, nMapNum);									// ���� ����Ʈ�� ��Ʈ���� �߰��Ѵ�.
		//}
	}
	// SubQuest Add
	else																			// ����Ʈ ��Ʈ���� ���� ����Ʈ ���̵� ������,
	{
		PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pData);							// ����Ʈ Ʈ�� �˻��� �����Ѵ�.
			if(!pData)			break;												// ����Ʈ Ʈ�� ������ ��ȿ���� ������, �˻��� ����������.
			
			if(pData->QuestIdx == pQuestString->GetQuestID())						// ����Ʈ ��Ʈ���� ����Ʈ ���̵��, ����Ʈ Ʈ���� ����Ʈ �ε����� ������,
			{
				PTRLISTPOS pos = pData->list.GetTailPosition();						// ����Ʈ Ʈ���� ����Ʈ�� ������ ��ġ ������ �޴´�.
				if(pos)																// ��ġ ������ ��ȿ�ϸ�,
				{
					CQuestString* pQString = (CQuestString*)pData->list.GetTail();	// ����Ʈ Ʈ�� ����Ʈ���� ��Ʈ�� ������ �޴´�.
					if(pQString->GetSubID() == QUEST_GIVEUP_ID)						// ���� ���̵� ����� ������,
					{
						pData->list.InsertBefore(pos, pQuestString);				// ����Ʈ Ʈ�� ����Ʈ�� ����Ʈ ��Ʈ���� ������ ���� �ִ´�.
					}
					else															// ���� ���̵� ���Ⱑ �ƴ϶��,
					{
						pData->list.AddTail(pQuestString);							// ����Ʈ Ʈ�� ����Ʈ�� �ڷ� ��Ʈ���� �߰��Ѵ�.
					}

					++pData->Count;													// ����Ʈ Ʈ���� ī��Ʈ�� �����Ѵ�.
					bQuestAdd = TRUE;												// ����Ʈ �߰� ���θ� TRUE�� �����Ѵ�.
				}				
				break;																// �˻��� ���� ������.
			}

		PTRLISTSEARCHEND															// �˻��� �����Ѵ�.

		if(!bQuestAdd)																// ����Ʈ �߰� ���ΰ� FALSE�� ������,
		{
			SubQuestAdd(pQuestString);												// ���� ����Ʈ�� �߰��Ѵ�.
		}
	}

	//
	QuestListReset();																// ����Ʈ ����Ʈ�� ���ΰ�ħ �Ѵ�.
}


// ���� ����Ʈ�� DB�� ����� ������ �ȵǴ� ��찡 ����. Ȯ���� ó��. - RaMa 04.10.16
void CQuestDialog::CheckQuestDate()													// ����Ʈ ��¥�� üũ�ϴ� �Լ�.
{	
	QuestTree* pQTree = NULL;														// ����Ʈ Ʈ�� ������ ���� �����͸� �����ϰ� NULL ó�� �Ѵ�.

	PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pQTree);							// ����Ʈ Ʈ�� ����Ʈ���� �˻��� �Ѵ�.
		// 2���̶��.. MainQuest�� ���������̴�. ����...
		if(pQTree && pQTree->list.GetCount() == 2)									// ����Ʈ Ʈ�� ������ ��ȿ�ϰ�, ����Ʈ ������ 2���̸�,
		{	
																					// ������ ������.
			SEND_QUEST_IDX_SIMPLE msg;												// �޽��� ����ü�� �����Ѵ�.
			msg.Category = MP_QUEST;												// ī�װ��� ����Ʈ�� �����Ѵ�.
			msg.Protocol = MP_QUEST_DELETE_CONFIRM_SYN;								// ���������� ����Ʈ ���� ���� ��ũ�� �����Ѵ�.
			msg.dwObjectID = HERO->GetID();											// HERO ���̵� �����Ѵ�.
			msg.MainQuestIdx = pQTree->QuestIdx;									// ���� ����Ʈ �ε����� �����Ѵ�.

			NETWORK->Send(&msg, sizeof(msg));										// �޽����� �����Ѵ�.
		}
		// Ȥ�ö� MainQuest�� �����ߴµ� SubQuest�� ���� ���� ���. �����ش�.
		else if(pQTree)																// ����Ʈ Ʈ�� ������ ��ȿ�ϰ�, ����Ʈ ������ 2���� �ƴϸ�,
		{
			CQuestString* pQuestString = (CQuestString*)pQTree->list.GetHead();		// ����Ʈ Ʈ�������� ����Ʈ���� ��� ��Ʈ���� �޴´�.

			if(pQuestString && pQuestString->GetSubID() != 0)						// ��Ʈ�� ������ ��ȿ�ϰ�, ���� ���̵� 0�� �ƴϸ�,
			{
				// 
				m_QuestTree.Remove(pQTree);											// ����Ʈ Ʈ�� ����Ʈ���� Ʈ�� ������ �����.	
				delete pQTree;														// ����Ʈ Ʈ�� ������ �����Ѵ�.
				pQTree = NULL;														// ����Ʈ Ʈ�� ������ �޴� �����͸� NULL ó�� �Ѵ�.

				// �����۵� ������ �Ѵ�.
			}
		}
	PTRLISTSEARCHEND																// �˻��� �����Ѵ�.
		
	QuestListReset();																// ����Ʈ ����Ʈ�� ���ΰ�ħ�Ѵ�.
}


void CQuestDialog::ProcessQuestDelete(CQuestString* pQuestString)					// ����Ʈ ���� ���μ��� �Լ�.
{
	if(!pQuestString)		return;													// ��Ʈ�� ������ ��ȿ���� ������, ����ó���� �Ѵ�.

	QuestTree* pQTree = NULL;														// ����Ʈ Ʈ�� ������ ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.
	CQuestString* pData = NULL;														// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.

	PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pQTree);							// ����Ʈ Ʈ�� ����Ʈ���� �˻��� �����Ѵ�.

		if(pQTree && pQTree->QuestIdx==pQuestString->GetQuestID())					// ����Ʈ Ʈ�� ������ ��ȿ�ϰ�, ����Ʈ �ε����� ��Ʈ���� ����Ʈ ���̵�� ������,
		{
			PTRLISTSEARCHSTART(pQTree->list, CQuestString*, pData);					// Ʈ�� ������ ����Ʈ���� �˻��� �����Ѵ�.
				if(pData && pData==pQuestString)									// Ʈ�� ������ ��ȿ�ϰ�, ����Ʈ ��Ʈ�� ������ Ʈ�� ������ ������,
				{
					pQTree->list.Remove(pQuestString);								// Ʈ�� �������� ����Ʈ ��Ʈ�� ������ ����.
					--pQTree->Count;												// ����Ʈ Ʈ���� ī��Ʈ�� �����Ѵ�.

																					// ��� ����Ʈ�� ���� �Ǿ�����
					if(pQTree->Count<=0 || pQuestString->GetSubID()==0)				// ����Ʈ ī��Ʈ�� 0����, ����Ʈ ��Ʈ���� ���� ���̵� 0�� ���ٸ�,
					{
						CompleteQuestDelete(pQuestString);							// �Ϻ��ϰ� ����Ʈ ������ �Ѵ�.
					}
					
					break;															// �˻��� ���� ������.
				}
			PTRLISTSEARCHEND														// �˻��� �����Ѵ�.

			break;																	// �˻��� ����������.
		}
	PTRLISTSEARCHEND																// ����Ʈ �˻��� �����Ѵ�.

	//
	QuestListReset();																// ����Ʈ ����Ʈ�� ���� ��ħ�Ѵ�.
}

void CQuestDialog::CompleteQuestDelete(CQuestString* pQuestString)					// ������ ����Ʈ�� �����ϴ� �Լ�.
{
	if(!pQuestString)		return;													// ����Ʈ ������ ��ȿ���� ������, ���� ó���� �Ѵ�.
	
	QuestTree* pQTree = NULL;														// ����Ʈ Ʈ�� ������ ���� �����͸� �����ϰ� NULLó���� �Ѵ�.
	
	PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pQTree);							// ����Ʈ Ʈ�� ����Ʈ���� ����Ʈ Ʈ�� ������ �˻��Ѵ�.

		if(pQTree && pQTree->QuestIdx == pQuestString->GetQuestID())				// ����Ʈ Ʈ�� ������ ��ȿ�ϰ�, ����Ʈ ��Ʈ���� ����Ʈ ���̵��, Ʈ�� ������ ����Ʈ �ε����� ������,
		{
			pQTree->State = eQTree_NotUse;											// ����Ʈ ���¸� ������� ���� ���·� �����Ѵ�.
			pQTree->Count = 0;														// ����Ʈ Ʈ���� ī��Ʈ�� 0���� �����Ѵ�.
			pQTree->list.RemoveAll();												// ����Ʈ Ʈ�� ������ ����Ʈ�� ��� ����.
			m_QuestTree.Remove(pQTree);												// ����Ʈ Ʈ�� ����Ʈ���� ����Ʈ Ʈ�� ������ �����Ѵ�.
			delete pQTree;															// ����Ʈ Ʈ�� ������ �����Ѵ�.
			--m_QTreeCount;															// ����Ʈ Ʈ�� ī��Ʈ�� �����Ѵ�.
			break;																	// �˻��� ����������.
		}	
	PTRLISTSEARCHEND																// ����Ʈ Ʈ�� ����Ʈ �˻��� �����Ѵ�.
		
	QuestListReset();																// ����Ʈ Ʈ������ ���ΰ�ħ �Ѵ�.
}

//void CQuestDialog::CompleteQuestDelete(CQuestString* pQuestString)					// ������ ����Ʈ�� �����ϴ� �Լ�.
//{
//	if(!pQuestString)		return;													// ����Ʈ ������ ��ȿ���� ������, ���� ó���� �Ѵ�.
//	
//	QuestTree* pQTree = NULL;														// ����Ʈ Ʈ�� ������ ���� �����͸� �����ϰ� NULLó���� �Ѵ�.
//	
//	PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pQTree);							// ����Ʈ Ʈ�� ����Ʈ���� ����Ʈ Ʈ�� ������ �˻��Ѵ�.
//
//		if(pQTree && pQTree->QuestIdx == pQuestString->GetQuestID())				// ����Ʈ Ʈ�� ������ ��ȿ�ϰ�, ����Ʈ ��Ʈ���� ����Ʈ ���̵��, Ʈ�� ������ ����Ʈ �ε����� ������,
//		{
//			pQTree->State = eQTree_NotUse;											// ����Ʈ ���¸� ������� ���� ���·� �����Ѵ�.
//			pQTree->Count = 0;														// ����Ʈ Ʈ���� ī��Ʈ�� 0���� �����Ѵ�.
//			pQTree->list.RemoveAll();												// ����Ʈ Ʈ�� ������ ����Ʈ�� ��� ����.
//			m_QuestTree.Remove(pQTree);												// ����Ʈ Ʈ�� ����Ʈ���� ����Ʈ Ʈ�� ������ �����Ѵ�.
//			delete pQTree;															// ����Ʈ Ʈ�� ������ �����Ѵ�.
//			--m_QTreeCount;															// ����Ʈ Ʈ�� ī��Ʈ�� �����Ѵ�.
//			break;																	// �˻��� ����������.
//		}	
//	PTRLISTSEARCHEND																// ����Ʈ Ʈ�� ����Ʈ �˻��� �����Ѵ�.
//		
//	QuestListReset();																// ����Ʈ Ʈ������ ���ΰ�ħ �Ѵ�.
//}

//void CQuestDialog::QuestTreeStateCheck(int RowIdx)									// ����Ʈ Ʈ�� ���¸� üũ�ϴ� �Լ�.
//{
//	if(RowIdx<0)			return;													// �� �ε����� 0�̻��̸�, ���� ó���� �Ѵ�.
//
//	QuestTree* pQTree = NULL;														// ����Ʈ Ʈ�� ������ ���� �����͸� �����ϰ�, NULLó���� �Ѵ�.
//	CQuestString* pQuestString = m_QuestList[RowIdx];								// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ�, 
//
//	if(!pQuestString)			return;												// ��Ʈ�� ������ ��ȿ���� ������ ���� ó���� �Ѵ�.
//
//	// ����Ʈ ����
//	
//	DWORD dwKey = pQuestString->GetKey() ;											// Ű ���� �޴´�.
//
//	if(pQuestString->IsTreeTitle())													// ������ Ʈ���� �� �̸��̸�,
//	{
//		PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pQTree);						// ����Ʈ Ʈ�� ����Ʈ �˻��� �Ѵ�.
//
//			if(pQTree && pQTree->nMapNum == pQuestString->GetQuestID())				// Ʈ�� ������ ��ȿ�ϰ�, Ʈ���� �� ��ȣ��, ��Ʈ���� �� ��ȣ�� ��ġ�ϸ�,
//			{
//				if(pQTree->State == eQTree_Open)									// ����Ʈ Ʈ���� ���� ���¶��,
//				{
//					pQTree->State = eQTree_Close;									// Ʈ�� ���¸� �������� �����Ѵ�.
//				}
//				else if(pQTree->State == eQTree_Close)								// ����Ʈ Ʈ���� �����ִ� ���¶��,
//				{
//					pQTree->State = eQTree_Open;									// ����Ʈ Ʈ���� ���� ���·� �����Ѵ�.
//				}
//
//				QuestListReset();													// ����Ʈ ����Ʈ�� ���ΰ�ħ�Ѵ�.
//
//				break;																// Ʈ�� ����Ʈ �˻��� ����������.
//			}
//		PTRLISTSEARCHEND															// Ʈ�� ����Ʈ �˻��� �����Ѵ�.
//
//		QuestListReset() ;
//	}																					
//	else																			// ����Ʈ ��Ʈ���� ���� ���̵� 0�� ���� ������,
//		QuestDescView(pQuestString);												// ����Ʈ �� ������ ����.
//}

void CQuestDialog::QuestTreeStateCheck(int RowIdx)									// ����Ʈ Ʈ�� ���¸� üũ�ϴ� �Լ�.
{
	if(RowIdx<0)			return;													// �� �ε����� 0�̻��̸�, ���� ó���� �Ѵ�.

	DWORD mapNum = m_dwQuestList[RowIdx];											// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ�, 

	QuestTree* pTree ;

	PTRLISTPOS pos = NULL ;
	pos = m_QuestTree.GetHeadPosition() ;

	while(pos)
	{
		pTree = NULL ;
		pTree = (QuestTree*)m_QuestTree.GetNext(pos) ;

		if( !pTree ) continue ;

		if( pTree->nMapNum != mapNum ) continue ;

		if(pTree->State == eQTree_Open)									// ����Ʈ Ʈ���� ���� ���¶��,
		{
			pTree->State = eQTree_Close;									// Ʈ�� ���¸� �������� �����Ѵ�.
		}
		else if(pTree->State == eQTree_Close)								// ����Ʈ Ʈ���� �����ִ� ���¶��,
		{
			pTree->State = eQTree_Open;									// ����Ʈ Ʈ���� ���� ���·� �����Ѵ�.
		}

		break;																// Ʈ�� ����Ʈ �˻��� ����������.
	}

	RefreshQuestList();													// ����Ʈ ����Ʈ�� ���ΰ�ħ�Ѵ�.



	//if(!pQuestString)			return;												// ��Ʈ�� ������ ��ȿ���� ������ ���� ó���� �Ѵ�.

	//// ����Ʈ ����
	//
	//DWORD dwKey = pQuestString->GetKey() ;											// Ű ���� �޴´�.

	//if(pQuestString->IsTreeTitle())													// ������ Ʈ���� �� �̸��̸�,
	//{
	//	PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pQTree);						// ����Ʈ Ʈ�� ����Ʈ �˻��� �Ѵ�.

	//		if(pQTree && pQTree->nMapNum == pQuestString->GetQuestID())				// Ʈ�� ������ ��ȿ�ϰ�, Ʈ���� �� ��ȣ��, ��Ʈ���� �� ��ȣ�� ��ġ�ϸ�,
	//		{
	//			if(pQTree->State == eQTree_Open)									// ����Ʈ Ʈ���� ���� ���¶��,
	//			{
	//				pQTree->State = eQTree_Close;									// Ʈ�� ���¸� �������� �����Ѵ�.
	//			}
	//			else if(pQTree->State == eQTree_Close)								// ����Ʈ Ʈ���� �����ִ� ���¶��,
	//			{
	//				pQTree->State = eQTree_Open;									// ����Ʈ Ʈ���� ���� ���·� �����Ѵ�.
	//			}

	//			QuestListReset();													// ����Ʈ ����Ʈ�� ���ΰ�ħ�Ѵ�.

	//			break;																// Ʈ�� ����Ʈ �˻��� ����������.
	//		}
	//	PTRLISTSEARCHEND															// Ʈ�� ����Ʈ �˻��� �����Ѵ�.

	//	QuestListReset() ;
	//}																					
	//else																			// ����Ʈ ��Ʈ���� ���� ���̵� 0�� ���� ������,
	//	QuestDescView(pQuestString);												// ����Ʈ �� ������ ����.
}


void CQuestDialog::QuestListReset()													// ����Ʈ ����Ʈ ���ΰ�ħ �Լ�.
{
	//int BackupLine = m_pQuestListDlg->GetTopListItemIdx();							// ��� ������ �޴´�.
	////
	//m_pQuestListDlg->RemoveAll();													// ����Ʈ ����Ʈ ���̾�α׸� ��� ����.

	//memset(m_QuestList, 0, sizeof(CQuestString*)*MAX_PROCESSQUEST);					// ����Ʈ ����Ʈ�� �޸� �� �Ѵ�.

	//m_QListCount=0;																	// ����Ʈ ����Ʈ ī��Ʈ�� 0���� �����Ѵ�.
	//m_ListLineIdx = 0;																// ����Ʈ ���� �ε����� 0���� �����Ѵ�.

	////
	//QuestTree* pQTree ;																// ����Ʈ Ʈ�� ������ ���� �����͸� �����Ѵ�.
	//CQuestString* pQuestString = NULL;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� NULLó�� �Ѵ�.

	//PTRLISTPOS pos = NULL ;															// ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	//pos = m_QuestTree.GetHeadPosition() ;											// ����Ʈ Ʈ���� ��� ���������� �����ϰ� ��ġ ������ �޴´�.

	//while(pos)																		// ��ġ������ ��ȿ�� ���� while���� ������.
	//{
	//	pQTree = NULL ;																// Ʈ�� ������ ���� �����͸� null ó���� �Ѵ�.
	//	pQTree = (QuestTree*)m_QuestTree.GetNext(pos) ;								// Ʈ���� ������ �ް� ��ġ�� �������� �����Ѵ�.

	//	if( !pQTree ) continue ;													// Ʈ�� ������ ��ȿ���� ������ continue ó���� �Ѵ�.

	//	if( pQTree->list.GetCount() <= 1 )											// ����Ʈ Ʈ���� ������ ���� 1�� ���ϸ�,
	//	{
	//		DeleteTree(pQTree) ;													// ����Ʈ Ʈ���� �����Ѵ�.

	//		QuestDescView(QuestTitleString(1)) ;									// ����Ʈ ����â�� ���� ��ħ�Ѵ�.
	//		m_pQuestDescList->ResetGuageBarPos() ;									// ����Ʈ ����â�� ��ġ�� �ٽ� �����Ѵ�.
	//	}

	//	if(pQTree->State == eQTree_Close)											// Ʈ�� ���°� �����ִ� ���¿� ������,
	//	{
	//		pQuestString = NULL ;													// ����Ʈ ��Ʈ�� ������ ���� �����͸� null ó���� �Ѵ�.
	//		pQuestString = (CQuestString*)pQTree->list.GetHead();					// ����� ����Ʈ ��Ʈ�� ������ �޴´�.
	//		QuestListAdd(pQuestString);												// ����Ʈ ����Ʈ�� ��Ʈ���� �߰��Ѵ�.
	//	}
	//	else if(pQTree->State == eQTree_Open)										// ����Ʈ�� �����ִ� ���¶��,
	//	{
	//		PTRLISTPOS openPos = NULL ;												// Ʈ���� ����Ʈ ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	//		openPos = pQTree->list.GetHeadPosition() ;								// ����Ʈ Ʈ�� ����Ʈ�� ��� ��ġ�� �޴´�.

	//		while(openPos)															// ��ġ ������ ��ȿ�� ���� while���� ������.
	//		{
	//			pQuestString = NULL ;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� null ó���� �Ѵ�.
	//			pQuestString = (CQuestString*)pQTree->list.GetNext(openPos) ;		// ��ġ�� ���� ����Ʈ ��Ʈ�� ������ �ް�, ����Ʈ�� ���� ��ġ�� �޴´�.

	//			if(!pQuestString) continue ;										// ����Ʈ�� ��Ʈ�� ������ ��ȿ���� ������ continue ó���� �Ѵ�.

	//			QuestListAdd(pQuestString) ;										// ����Ʈ ����Ʈ�� ��Ʈ���� �߰��Ѵ�.
	//		}
	//	}	
	//}

	//m_ListLineIdx = -1;																// ����Ʈ ���� �ε����� -1�� �����Ѵ�.

	//m_pQuestListDlg->SetTopListItemIdx(BackupLine);									// ����Ʈ ����Ʈ ���̾�α��� ž����Ʈ ������ �ε����� ����������� �����Ѵ�.

	//State = eQView_QList;															// ����Ʈ ���̾�α��� ���¸� eQView_QList�� �����Ѵ�.
}

//void CQuestDialog::QuestListReset()													// ����Ʈ ����Ʈ ���ΰ�ħ �Լ�.
//{
//	//m_SelectedQuestID = 0 ;
//	int BackupLine = m_pQuestListDlg->GetTopListItemIdx();							// ��� ������ �޴´�.
//	//
//	m_pQuestListDlg->RemoveAll();													// ����Ʈ ����Ʈ ���̾�α׸� ��� ����.
//
//	memset(m_QuestList, 0, sizeof(CQuestString*)*MAX_PROCESSQUEST);					// ����Ʈ ����Ʈ�� �޸� �� �Ѵ�.
//
//	m_QListCount=0;																	// ����Ʈ ����Ʈ ī��Ʈ�� 0���� �����Ѵ�.
//	m_ListLineIdx = 0;																// ����Ʈ ���� �ε����� 0���� �����Ѵ�.
//
//	//
//	QuestTree* pQTree = NULL;														// ����Ʈ Ʈ�� ������ ���� �����͸� �����ϰ� NULLó�� �Ѵ�.
//	CQuestString* pQuestString = NULL;												// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� NULLó�� �Ѵ�.
//
//	PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pQTree);							// ����Ʈ Ʈ�� ����Ʈ�� �˻��Ѵ�.
//
//		if( pQTree )																// Ʈ�� ������ ��ȿ���� üũ�Ѵ�.
//		{
//			if( pQTree->list.GetCount() <= 1 )
//			{
//				DeleteTree(pQTree) ;
//
//				QuestDescView(QuestTitleString(1)) ;
//				m_pQuestDescList->ResetGuageBarPos() ;
//
//
//				/*pQTree->list.RemoveAll() ;
//				m_QuestTree.Remove(pQTree) ;
//
//				CQuestDialog* pQuestDialog = GAMEIN->GetQuestDialog() ;
//
//				if( pQuestDialog )
//				{
//					pQuestDialog->QuestDescView(pQuestDialog->QuestTitleString(1)) ;
//					pQuestDialog->GetQuestDescList()->ResetGuageBarPos() ;
//				}
//
//				return ;*/
//			}
//
//			if(pQTree->State == eQTree_Close)										// Ʈ�� ���°� �����ִ� ���¿� ������,
//			{
//				pQuestString = (CQuestString*)pQTree->list.GetHead();				// ����Ʈ ��Ʈ�� ������ �޴´�.
//				QuestListAdd(pQuestString);											// ����Ʈ ����Ʈ�� ��Ʈ���� �߰��Ѵ�.
//			}
//			else if(pQTree->State == eQTree_Open)									// ����Ʈ�� �����ִ� ���¶��,
//			{
//				PTRLISTSEARCHSTART(pQTree->list, CQuestString*, pQuestString);		// Ʈ�� ������ ����Ʈ �˻��� �Ѵ�.
//					QuestListAdd(pQuestString, pQuestString->GetSubID());			// ����Ʈ ��Ʈ���� ����Ʈ ����Ʈ�� �߰��Ѵ�.
//				PTRLISTSEARCHEND													// Ʈ�� ������ ����Ʈ �˻� ����.
//			}		
//		}
//	PTRLISTSEARCHEND																// Ʈ�� ����Ʈ �˻��� �����Ѵ�.
//
//	m_ListLineIdx = -1;																// ����Ʈ ���� �ε����� -1�� �����Ѵ�.
//
//	//if(!m_pMsgBox)		m_SelectedQuestID = 0;										// �˸�â�� �������� ������, ���� �� ����Ʈ ���̵� -1�� �����Ѵ�.
//
//	m_pQuestListDlg->SetTopListItemIdx(BackupLine);									// ����Ʈ ����Ʈ ���̾�α��� ž����Ʈ ������ �ε����� ����������� �����Ѵ�.
//
//	State = eQView_QList;															// ����Ʈ ���̾�α��� ���¸� eQView_QList�� �����Ѵ�.
//}
CQuestString* CQuestDialog::QuestTitleString(int index)
{
	if( index < 0 || index >= m_QListCount )
	{
		return NULL ;
	}

	return m_QuestList[index] ;
}

DWORD CQuestDialog::GetQuestIdx(int index)
{
	if( index < 0 || index >= m_QListCount )
	{
		return NULL ;
	}

	return m_dwQuestList[index] ;
}

void CQuestDialog::QuestListAdd(CQuestString* pQuestString, int SubID)				// ����Ʈ ����Ʈ�� ��Ʈ���� �߰��ϴ� �Լ�.
{	
	if(!pQuestString)			return;												// ����Ʈ ��Ʈ�� ������ ��ȿ���� ������, ���� ó���� �Ѵ�.

	DWORD dwKey = pQuestString->GetKey() ;

	DWORD Color = 0;																// ���� ���� ���� ������ �����ϰ� 0���� �����Ѵ�.

	ITEM* pQStr = NULL;															// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� null ó�� �Ѵ�.

	char buf[256] = { 0, };															// �ӽ� ���۸� �����Ѵ�.	

	PTRLISTPOS spos = pQuestString->GetTitle()->GetHeadPosition();					// ����Ʈ ��Ʈ���� ��� ��ġ�� �޴´�.

	while(spos)																		// ��ġ ������ ��ȿ�� ���� while���� ������.
	{
		pQStr = (ITEM*)pQuestString->GetTitle()->GetNext(spos);					// ��ġ ������ �ش��ϴ� ����Ʈ ��Ʈ�� ������ �޴´�.

		if(pQStr)																	// ��Ʈ�� ������ ��ȿ�ϸ�,
		{
			if( pQuestString->IsTreeTitle() )
			{
				pQStr->rgb = RGBA_MAKE(200, 128, 0, 255) ;
				pQStr->nFontIdx = cFont::FONT1 ;
			}
			else
			{
				if( m_SelectedQuestID == pQuestString->GetQuestID() )
				{
					pQStr->rgb = RGBA_MAKE(0, 0, 255, 255) ;
				}
				else
				{
					pQStr->rgb = RGBA_MAKE(10, 10, 10, 255) ;
				}
				pQStr->nFontIdx = cFont::FONT0 ;
			}

			if(pQuestString->IsTreeTitle())										// ��Ʈ���� Ʈ�� Ÿ��Ʋ�̶��,
			{
				sprintf(buf, "* ") ;												// �ӽ� ���۸� *�� �����Ѵ�.
			}
			else																	// ���� ���̵� ������,
			{
				BOOL bResult = FALSE ;

				/*for(int count = 0 ; count < 5 ; ++count )
				{
					if( QUESTMGR->GetQuestIDFromQuickView(count) == pQuestString->GetQuestID() )
					{
						bResult = TRUE ;
						break ;
					}
				}*/

				/*CQuestQuickViewDialog* pQuickView = GAMEIN->GetQuestQuickViewDialog() ;

				if( pQuickView )
				{
					cPtrList* pQuickList = pQuickView->GetViewList() ;

					if( pQuickList )
					{
						QuestTree* pQuestTree ;

						PTRLISTPOS quickPos = NULL ;
						quickPos = pQuickList->GetHeadPosition() ;

						while(quickPos)
						{
							pQuestTree = NULL ;
							pQuestTree = (QuestTree*)pQuickList->GetNext(quickPos) ;

							if( pQuestTree )
							{
								if( pQuestTree->QuestIdx == pQuestString->GetQuestID() )
								{
									bResult = TRUE ;

									break ;
								}
							}
						}
					}
				}*/

				CQuestQuickViewDialog* pQuickView = NULL ;
				pQuickView = GAMEIN->GetQuestQuickViewDialog() ;

				if( !pQuickView ) continue ;

				DWORD* pCheckIdx = NULL ;

				cPtrList* pCheckList = NULL ;
				pCheckList = pQuickView->GetViewList() ;

				if( !pCheckList ) continue ;
				
				PTRLISTPOS checkPos = NULL ;
				checkPos = pCheckList->GetHeadPosition() ;

				while(checkPos)
				{
					pCheckIdx = NULL ;
					pCheckIdx = (DWORD*)pCheckList->GetNext(checkPos) ;

					if( !pCheckIdx ) continue ;

					if( *pCheckIdx == pQuestString->GetQuestID() )
					{
						bResult = TRUE ;
						break ;
					}
				}



				if( bResult == FALSE )
				{
					sprintf(buf, "	- ");												// �ӽ� ���۸� -�� �����Ѵ�.
				}
				else
				{
					sprintf(buf, "	��");												// �ӽ� ���۸� -�� �����Ѵ�.
				}
			}

			strcat(buf, pQStr->string);

			int nSubQuestCount = 0 ;
			CQuestInfo* pQuestInfo = QUESTMGR->GetQuestInfo(pQuestString->GetQuestID()) ;				// ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.

			if( pQuestInfo )
			{
				nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// ���� ����Ʈ ���� �޴´�.
			}

			if( pQuestString->IsComplete() || nSubQuestCount == 2 )
			{
				strcat(buf, CHATMGR->GetChatMsg(1463)) ;
			}

			m_pQuestListDlg->AddItem(buf, pQStr->rgb) ;
			++m_ListLineIdx;														// ����Ʈ ���� �ε����� �����Ѵ�.
			memset(buf, 0, 256);													// �ӽ� ���۸� �޸� �� �Ѵ�.
			
			// ListDialog 
			m_QuestList[m_QListCount] = pQuestString;								// ����Ʈ ����Ʈ�� ����Ʈ ��Ʈ�� ������ �����Ѵ�.
			++m_QListCount;															// ����Ʈ ����Ʈ ī��Ʈ�� �����Ѵ�.
		}
	}
}

//void CQuestDialog::QuestListAdd(CQuestString* pQuestString, int SubID)				// ����Ʈ ����Ʈ�� ��Ʈ���� �߰��ϴ� �Լ�.
//{	
//	if(!pQuestString)			return;												// ����Ʈ ��Ʈ�� ������ ��ȿ���� ������, ���� ó���� �Ѵ�.
//
//	DWORD dwKey = pQuestString->GetKey() ;
//
//	DWORD Color = 0;																// ���� ���� ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//	QString* pQStr = NULL;															// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� null ó�� �Ѵ�.
//
//	char buf[256] = { 0, };															// �ӽ� ���۸� �����Ѵ�.	
//
//	PTRLISTPOS spos = pQuestString->GetTitle()->GetHeadPosition();					// ����Ʈ ��Ʈ���� ��� ��ġ�� �޴´�.
//
//	while(spos)																		// ��ġ ������ ��ȿ�� ���� while���� ������.
//	{
//		pQStr = (QString*)pQuestString->GetTitle()->GetNext(spos);					// ��ġ ������ �ش��ϴ� ����Ʈ ��Ʈ�� ������ �޴´�.
//
//		if(pQStr)																	// ��Ʈ�� ������ ��ȿ�ϸ�,
//		{
//			if( pQuestString->IsTreeTitle() )
//			{
//				Color = RGBA_MAKE(200, 128, 0, 255) ;
//			}
//			else
//			{
//				if( m_SelectedQuestID == pQuestString->GetQuestID() )
//				{
//					Color = RGBA_MAKE(0, 0, 255, 255) ;
//				}
//				else
//				{
//					Color = RGBA_MAKE(10, 10, 10, 255) ;
//				}
//			}
//
//			//if(m_ListLineIdx == m_ChangedRow)										// ����Ʈ ���� �ε�����, ����� �� ��ȣ�� ������,
//			//{
//			//	if(pQuestString->IsTreeTitle())										// ��Ʈ���� Ʈ�� Ÿ��Ʋ�̶��,
//			//	{
//			//		//Color=MAINQUEST_TITLE_COLOR_SEL ;								// ������ ���� ����Ʈ �������� �����Ѵ�.
//			//		Color= RGB(0, 64, 64) ;								// ������ ���� ����Ʈ �������� �����Ѵ�.
//			//	}
//			//	else																// ���� ���̵� 0�� �ƴϸ�,
//			//	{
//			//		//Color=SUBQUEST_TITLE_COLOR_SEL;									// ������ ���� ����Ʈ �������� �����Ѵ�.
//			//		Color= RGB(255, 100, 0) ;									// ������ ���� ����Ʈ �������� �����Ѵ�.
//			//	}
//			//}
//			//else																	// ����Ʈ ���� �ε�����, ���� �� �� ��ȣ�� ���� ������,
//			//{
//			//	if(pQuestString->IsTreeTitle())										// ��Ʈ���� Ʈ�� Ÿ��Ʋ�̶��,
//			//	{
//			//		//Color=MAINQUEST_TITLE_COLOR ;									// ������ ���� ����Ʈ �������� �����Ѵ�.
//			//		Color= RGB(100, 50, 0) ;									// ������ ���� ����Ʈ �������� �����Ѵ�.
//			//	}
//			//	else																// ���� ���̵� ������,
//			//	{
//			//		//Color=SUBQUEST_TITLE_COLOR;										// ������ ���� ����Ʈ �������� �����Ѵ�.
//			//		Color= RGB(0, 100, 50) ;										// ������ ���� ����Ʈ �������� �����Ѵ�.
//			//	}
//			//}
//
//			if(pQuestString->IsTreeTitle())										// ��Ʈ���� Ʈ�� Ÿ��Ʋ�̶��,
//			{
//				sprintf(buf, "* ") ;												// �ӽ� ���۸� *�� �����Ѵ�.
//			}
//			else																	// ���� ���̵� ������,
//			{
//				BOOL bResult = FALSE ;
//
//				for(int count = 0 ; count < 5 ; ++count )
//				{
//					if( QUESTMGR->GetQuestIDFromQuickView(count) == pQuestString->GetQuestID() )
//					{
//						bResult = TRUE ;
//						sprintf(buf, "	��");												// �ӽ� ���۸� -�� �����Ѵ�.
//						break ;
//					}
//				}
//
//				if( bResult == FALSE )
//				{
//					sprintf(buf, "	- ");												// �ӽ� ���۸� -�� �����Ѵ�.
//				}
//			}
//
//			strcat(buf, pQStr->Str);												// ����Ʈ ��Ʈ���� �ӽ� ���� ������ �����Ѵ�.
//			//strcat(buf, pQuestString->GetTitleStr());												// ����Ʈ ��Ʈ���� �ӽ� ���� ������ �����Ѵ�.
//
//			//DWORD dwSubID = pQuestString->GetSubID() ;
//			//DWORD dwSubID = 0 ;
//
//			//if(dwSubID)																// ����Ʈ ��Ʈ���� ���� ���̵� ��ȿ�ϸ�,
//			//{
//			//	CQuest* pQuest = QUESTMGR->GetQuest(dwSubID);						// ����Ʈ ���̵�� ����Ʈ ������ �޴´�.
//			//	SUBQUEST* pSubQuest = pQuest->GetSubQuest(dwSubID);					// ���� ����Ʈ ���̵�� ���� ����Ʈ ������ �޴´�.
//
//			//	if(pSubQuest->dwMaxCount)											// ���� ����Ʈ�� �ִ� ī��Ʈ�� �ϳ� �̻��̸�,
//			//	{
//			//		char tbuf[32] = { 0, };											// �ӽ� ���۸� �����Ѵ�.
//			//		sprintf(tbuf, "  (%d/%d)", pSubQuest->dwData, pSubQuest->dwMaxCount);	// ����Ʈ �����Ϳ� ī��Ʈ�� �ӽ� ���ۿ� �����Ѵ�.
//			//		strcat(buf, tbuf);												// �ӽ� ���� ���� �����Ѵ�.
//			//	}
//			//}
//
//			CQuestInfo* pQuestInfo = QUESTMGR->GetQuestInfo(pQuestString->GetQuestID()) ;				// ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.
//			int nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// ���� ����Ʈ ���� �޴´�.
//
//			if( pQuestString->IsComplete() || nSubQuestCount == 2 )
//			{
//				strcat(buf, "(�Ϸ�)") ;
//			}
//
//			m_pQuestListDlg->AddItem(buf, Color);									// ����Ʈ ����Ʈ�� �ӽ� ���۸� �߰��Ѵ�.
//			++m_ListLineIdx;														// ����Ʈ ���� �ε����� �����Ѵ�.
//			memset(buf, 0, 256);													// �ӽ� ���۸� �޸� �� �Ѵ�.
//			
//			// ListDialog 
//			m_QuestList[m_QListCount] = pQuestString;								// ����Ʈ ����Ʈ�� ����Ʈ ��Ʈ�� ������ �����Ѵ�.
//			++m_QListCount;															// ����Ʈ ����Ʈ ī��Ʈ�� �����Ѵ�.
//		}
//	}
//}


//void CQuestDialog::QuestListAdd(CQuestString* pQuestString, int SubID)				// ����Ʈ ����Ʈ�� ��Ʈ���� �߰��ϴ� �Լ�.
//{	
//	if(!pQuestString)			return;												// ����Ʈ ��Ʈ�� ������ ��ȿ���� ������, ���� ó���� �Ѵ�.
//
//	DWORD Color = 0;																// ���� ���� ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//	QString* pQStr = NULL;															// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� null ó�� �Ѵ�.
//
//	char buf[256] = { 0, };															// �ӽ� ���۸� �����Ѵ�.	
//
//																					// Quest Giveup Message
//	if(SubID == QUEST_GIVEUP_ID)													// ���� ���̵� ������̵�� ���ٸ�,
//	{
//		if(m_ListLineIdx==m_ChangedRow)												// ����Ʈ ���� �ε����� ����� �� ��ȣ�� ������,
//		{
//			Color=QUEST_GIVEUP_COLOR_SEL ;											// ������ ����Ʈ ���� ���� �������� �����Ѵ�.
//		}
//		else																		// ���� ������,
//		{
//			Color=QUEST_GIVEUP_COLOR;												// ����Ʈ ���� ����(�Ϲ�)���� �����Ѵ�.
//		}
//
//		m_pQuestListDlg->AddItem(CHATMGR->GetChatMsg(232), Color);					// ����Ʈ ����Ʈ ���̾�α׿�, < ����Ʈ ���� > ��Ʈ���� �߰��Ѵ�.
//		++m_ListLineIdx;															// ����Ʈ ���� �ε����� �����Ѵ�.
//		m_QuestList[m_QListCount] = pQuestString;									// ����Ʈ ����Ʈ�� ��Ʈ���� �߰��Ѵ�.
//		++m_QListCount;																// ����Ʈ ī��Ʈ�� �����Ѵ�.
//
//		return;																		// ���� ó���� �Ѵ�.
//	}
//
//	PTRLISTPOS spos = pQuestString->GetTitle()->GetHeadPosition();					// ����Ʈ ��Ʈ���� ��� ��ġ�� �޴´�.
//
//	while(spos)																		// ��ġ ������ ��ȿ�� ���� while���� ������.
//	{
//		pQStr = (QString*)pQuestString->GetTitle()->GetNext(spos);					// ��ġ ������ �ش��ϴ� ����Ʈ ��Ʈ�� ������ �޴´�.
//
//		if(pQStr)																	// ��Ʈ�� ������ ��ȿ�ϸ�,
//		{
//			if(m_ListLineIdx == m_ChangedRow)										// ����Ʈ ���� �ε�����, ����� �� ��ȣ�� ������,
//			{
//				if(SubID==0)														// ���� ���̵� 0�� ������,
//				{
//					Color=MAINQUEST_TITLE_COLOR_SEL ;								// ������ ���� ����Ʈ �������� �����Ѵ�.
//				}
//				else																// ���� ���̵� 0�� �ƴϸ�,
//				{
//					Color=SUBQUEST_TITLE_COLOR_SEL;									// ������ ���� ����Ʈ �������� �����Ѵ�.
//				}
//			}
//			else																	// ����Ʈ ���� �ε�����, ���� �� �� ��ȣ�� ���� ������,
//			{
//				if(SubID==0)														// ���� ���̵� ������,
//				{
//					Color=MAINQUEST_TITLE_COLOR ;									// ������ ���� ����Ʈ �������� �����Ѵ�.
//				}
//				else																// ���� ���̵� ������,
//				{
//					Color=SUBQUEST_TITLE_COLOR;										// ������ ���� ����Ʈ �������� �����Ѵ�.
//				}
//			}
//
//			if(SubID==0)															// ���� ���̵� ������, 
//			{
//				sprintf(buf, "* ") ;												// �ӽ� ���۸� *�� �����Ѵ�.
//			}
//			else																	// ���� ���̵� ������,
//			{
//				sprintf(buf, "	- ");												// �ӽ� ���۸� -�� �����Ѵ�.
//			}
//
//			strcat(buf, pQStr->Str);												// ����Ʈ ��Ʈ���� �ӽ� ���� ������ �����Ѵ�.
//
//			//DWORD dwSubID = pQuestString->GetSubID() ;
//			DWORD dwSubID = 0 ;
//
//			if(dwSubID)																// ����Ʈ ��Ʈ���� ���� ���̵� ��ȿ�ϸ�,
//			{
//				CQuest* pQuest = QUESTMGR->GetQuest(dwSubID);						// ����Ʈ ���̵�� ����Ʈ ������ �޴´�.
//				SUBQUEST* pSubQuest = pQuest->GetSubQuest(dwSubID);					// ���� ����Ʈ ���̵�� ���� ����Ʈ ������ �޴´�.
//
//				if(pSubQuest->dwMaxCount)											// ���� ����Ʈ�� �ִ� ī��Ʈ�� �ϳ� �̻��̸�,
//				{
//					char tbuf[32] = { 0, };											// �ӽ� ���۸� �����Ѵ�.
//					sprintf(tbuf, "  (%d/%d)", pSubQuest->dwData, pSubQuest->dwMaxCount);	// ����Ʈ �����Ϳ� ī��Ʈ�� �ӽ� ���ۿ� �����Ѵ�.
//					strcat(buf, tbuf);												// �ӽ� ���� ���� �����Ѵ�.
//				}
//			}
//
//			m_pQuestListDlg->AddItem(buf, Color);									// ����Ʈ ����Ʈ�� �ӽ� ���۸� �߰��Ѵ�.
//			++m_ListLineIdx;														// ����Ʈ ���� �ε����� �����Ѵ�.
//			memset(buf, 0, 256);													// �ӽ� ���۸� �޸� �� �Ѵ�.
//			
//			// ListDialog 
//			m_QuestList[m_QListCount] = pQuestString;								// ����Ʈ ����Ʈ�� ����Ʈ ��Ʈ�� ������ �����Ѵ�.
//			++m_QListCount;															// ����Ʈ ����Ʈ ī��Ʈ�� �����Ѵ�.
//		}
//	}
//}

//void CQuestDialog::QuestDescView(CQuestString* pQuestString)						// ����Ʈ �� ���� ���� �Լ�.
//{
//	if(!pQuestString)			return;												// ����Ʈ ��Ʈ�� ������ ��ȿ���� ������, ���� ó���� �Ѵ�.
//	
//	int BackupLine = m_pQuestListDlg->GetTopListItemIdx();							// ��� ������ �����Ѵ�.
//
//	m_pQuestDescList->RemoveAll();													// ����Ʈ ����Ʈ ���̾�α׸� ��� ����.
//
//	memset(m_QuestList, 0, sizeof(CQuestString*)*MAX_PROCESSQUEST);					// ����Ʈ ����Ʈ�� �޸� �� �Ѵ�.
//
//	m_QListCount=0;																	// ����Ʈ ����Ʈ ī��Ʈ�� 0���� �����Ѵ�.
//	m_toPreList= -1;																// ���� ����Ʈ�� -1�� �����Ѵ�.
//
//	QString* pQStr=NULL;															// ����Ʈ ��Ʈ�� ������ ���� ������ �����ϰ� nulló�� �Ѵ�.
//
//	PTRLISTPOS pos = NULL;															// ��ġ ������ ���� ������ ������ �����ϰ� nulló�� �Ѵ�.
//
//	// Title
//	pos = pQuestString->GetTitle()->GetHeadPosition();								// ����Ʈ ��Ʈ���� �ص� ��ġ�� �����Ѵ�.
//
//	while(pos)																		// ��ġ ������ ��ȿ�� ���� while���� ������.
//	{
//		pQStr = (QString*)pQuestString->GetTitle()->GetNext(pos);					// ��ġ ������ �ش��ϴ� ����Ʈ ��Ʈ�� ������ �޴´�.
//
//		if(pQStr)																	// ����Ʈ ��Ʈ�� ������ ��ȿ�ϴٸ�,
//		{
//			m_pQuestDescList->AddItem(pQStr->Str, SUBQUEST_TITLE_COLOR_SEL);		// ����Ʈ ����Ʈ ���̾�α׿� �߰��Ѵ�.
//			++m_toPreList;															// ���� ����Ʈ ���� �����Ѵ�.
//		}
//	}
//	m_pQuestDescList->AddItem(" ", 0);												// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.
//	++m_toPreList;																	// ���� ����Ʈ ���� �����Ѵ�.
//
//	// �Ƿ��ڸ� �߰��Ѵ�.
//	m_pQuestDescList->AddItem("�Ƿ��� - ����", SUBQUEST_TITLE_COLOR_SEL);	
//	// ��ġ�� �߰��Ѵ�.
//	m_pQuestDescList->AddItem("��ġ - ���ɸ� ����Ա�", SUBQUEST_TITLE_COLOR_SEL);	
//
//	m_pQuestDescList->AddItem(" ", 0);												// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.
//
//	// <�Ƿڳ���>�� �߰��Ѵ�.
//
//	// Desc	
//	int preline = -1;																// ���� ������ -1�� �����Ѵ�.
//	pos = pQuestString->GetDesc()->GetHeadPosition();								// ����Ʈ ��Ʈ���� ��� ���������� �����Ѵ�.
//
//	while(pos)																		// ��ġ ������ ��ȿ�� ���� while���� ������.
//	{
//		pQStr = (QString*)pQuestString->GetDesc()->GetNext(pos);					// ����Ʈ ��Ʈ�� ������ �޴´�.
//
//		if(pQStr)																	// ����Ʈ ��Ʈ�� ������ ��ȿ�ϸ�,
//		{
//			m_pQuestDescList->AddItem(pQStr->Str, pQStr->Color, pQStr->Line);		// ����Ʈ ����Ʈ ���̾�α׿� �������� �߰��Ѵ�.
//
//																					// ������ �ٲ𶧸� ����ȭ�� ������ ����.
//			if(preline != pQStr->Line)												// ���� ���ΰ� ����Ʈ ��Ʈ�� ������ ������, 
//			{
//				++m_toPreList;														// ���� ����Ʈ ���� ������Ų��.
//				preline = pQStr->Line;												// ���� ������ ����Ʈ ��Ʈ���� �������� �����Ѵ�.
//			}
//		}
//	}
//
//	DWORD Color = 0;																// ������ ���� ������ �����ϰ� 0���� �����Ѵ�.
//	m_toPreList+=2;																	// ���� ����Ʈ  2�⸦ �� �����Ѵ�.
//
//	if(m_ChangedRow==m_toPreList)													// ���� �� �� ��ȣ�� ���� ����Ʈ ���� ������,
//	{
//		Color=SELECT_COLOR_SEL ;													// ������ ���� �� ��������  �����Ѵ�.
//	}
//	else																			// �׷��� ������,
//	{
//		Color=SELECT_COLOR;															// ������ �Ϲ� �������� �����Ѵ�.
//	}
//
//	// Back
//	//m_pQuestListDlg->AddItem(" ", 0);												// ����Ʈ ����Ʈ ���̾�α׿� ��ĭ �������� �߰��Ѵ�.
//	//// 070125 LYW --- Quest : Modified this line.
//	////m_pQuestListDlg->AddItem(CHATMGR->GetChatMsg(601), Color);
//	//m_pQuestListDlg->AddItem(CHATMGR->GetChatMsg(229), Color);						// ����Ʈ ����Ʈ ���̾�α׿�  < ���� ȭ�� >�� ����Ѵ�.
//
//	// ���� �������(2/5) �߰��ؾ� �Ѵ�.
//
//	m_PreDescQuestString = pQuestString;											// ���� ���� ����Ʈ ��Ʈ���� �޴´�.
//	m_pQuestDescList->SetTopListItemIdx(BackupLine);								// �������� ���� �ε����� �����Ѵ�.
//	State = eQView_QDesc;															// ����Ʈ â ���¸� eQView_QDesc�� �����Ѵ�.
//}

void CQuestDialog::UpdateQuestDescView(DWORD dwQuestIdx, DWORD dwSubQuestIdx)		// ����Ʈ ����â�� ������Ʈ �ϴ� �Լ�.
{
	int nQuestStringKey = 0 ;														// ����Ʈ ��Ʈ�� Ű ���� ���� ������ �����ϰ� 0���� �����Ѵ�.

	COMBINEKEY(dwQuestIdx, dwSubQuestIdx, nQuestStringKey) ;						// ���� ����Ʈ, ���� ����Ʈ �ε�����, ��ũ�� Ű ���� �޴´�.

	CQuestString* pQuestString = NULL ;												// ����Ʈ ��Ʈ���� ���� �����͸� �����ϰ� NULL ó���� �Ѵ�.
		
	pQuestString = QUESTMGR->GetQuestString(dwQuestIdx, dwSubQuestIdx);				// ����Ʈ �Ŵ������� Ű ������ ����Ʈ ��Ʈ���� �޾ƿ´�.

	if( pQuestString )
	{
		QuestDescView(pQuestString) ;												// ����Ʈ ����â�� ������Ʈ �Ѵ�.
	}
}

void CQuestDialog::UpdateQuestList(DWORD dwQuestIdx, DWORD dwSubQuestIdx)
{
	PTRLISTPOS pos = NULL ;
	pos = m_QuestTree.GetHeadPosition() ;

	QuestTree* pQTree ;

	CQuestString* pQuestString ;

	int count = 0 ;

	while(pos)
	{
		pQTree = NULL ;
		pQTree = (QuestTree*)m_QuestTree.GetNext(pos) ;

		cPtrList* pList = NULL ;

		if( pQTree )
		{
			pList = &(pQTree->list) ;

			if( pList )
			{
				PTRLISTPOS pTreePos = NULL ;
				pTreePos = pList->GetHeadPosition() ;

				while( pTreePos )
				{
					pQuestString = NULL ;
					pQuestString = (CQuestString*)pList->GetNext(pTreePos) ;

					if( pQuestString )
					{
						if( pQuestString->GetQuestID() == dwQuestIdx )
						{
							PTRLISTPOS spos = pQuestString->GetTitle()->GetHeadPosition();					// ����Ʈ ��Ʈ���� ��� ��ġ�� �޴´�.

							m_pQuestListDlg->RemoveItem(count) ;

							char tempBuf[256] = {0, } ;

							strcat(tempBuf, " - ") ;

							while(spos)																		// ��ġ ������ ��ȿ�� ���� while���� ������.
							{
								ITEM* pQStr = NULL;															// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� null ó�� �Ѵ�.
								pQStr = (ITEM*)pQuestString->GetTitle()->GetNext(spos);					// ��ġ ������ �ش��ϴ� ����Ʈ ��Ʈ�� ������ �޴´�.

								if(pQStr)																	// ��Ʈ�� ������ ��ȿ�ϸ�,
								{
									strcat(tempBuf, pQStr->string) ;
								}
							}

							strcat(tempBuf, CHATMGR->GetChatMsg(1463)) ;

							m_pQuestListDlg->AddItem(tempBuf, SUBQUEST_TITLE_COLOR, count) ;

							pQuestString->SetComplete(TRUE) ;

							break ;
						}

						++count ;
					}
				}
			}
		}
	}
}

void CQuestDialog::SetQuestPrevInfo(CQuestString* pQuestString)
{
	char tempBuf[128] = {0, } ;

	if( !pQuestString )
	{
		/*sprintf(tempBuf, "[SetQuestPrevInfo] - ����Ʈ ��Ʈ�� ������ ��ȿ���� �ʾ�, ����Ʈ �⺻ ������ �������� ���߽��ϴ�.") ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;
	}

	DWORD dwQuestIdx = pQuestString->GetQuestID() ;

	// - ����Ʈ ���� ���� - 
	CQuestString* pTitleString = NULL ;												// ����Ʈ ���� ��Ʈ�� ������ ���� �����͸� �����ϰ� nulló�� �Ѵ�.
	pTitleString = QUESTMGR->GetQuestString(dwQuestIdx, 0) ;						// ����Ʈ ���� ��Ʈ���� �޴´�.

	if( !pTitleString )
	{
		/*sprintf(tempBuf, "[SetQuestPrevInfo] - (%d)�� ����Ʈ ��Ʈ���� ���� ���� ����Ʈ ���� ó���� ���� ���߽��ϴ�.", pQuestString->GetQuestID()) ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;
	}

	ITEM* pItem ;

	PTRLISTPOS pos = NULL ;
	pos = pTitleString->GetTitle()->GetHeadPosition();								// ����Ʈ ��Ʈ���� �ص� ��ġ�� �����Ѵ�.

	while(pos)																		// ��ġ ������ ��ȿ�� ���� while���� ������.
	{
		pItem = NULL ;
		pItem = (ITEM*)pTitleString->GetTitle()->GetNext(pos);						// ��ġ ������ �ش��ϴ� ����Ʈ ��Ʈ�� ������ �ް�, ��ġ�� �������� �����Ѵ�.

		if(!pItem) continue ;														// ����Ʈ ��Ʈ�� ������ ��ȿ���� ������ continue ó���� �Ѵ�.

		sprintf(tempBuf, "< %s >", pItem->string) ;									// �ӽ� ���ۿ� ����Ʈ ������ �����Ѵ�.
		m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(255, 150, 0, 255));					// ����Ʈ ����Ʈ ���̾�α׿� �߰��Ѵ�.
	}

	m_pQuestDescList->AddItem(" ", 0);												// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.

	CQuest* pQuest = NULL ;
	pQuest = QUESTMGR->GetQuest(dwQuestIdx) ;

	if( !pQuest )
	{
		/*sprintf(tempBuf, "[SetQuestPrevInfo] - (%d)�� ����Ʈ�� ���� ���ؼ� ����Ʈ �Ƿ��ڸ� �������� ���߽��ϴ�.", dwQuestIdx) ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;
	}

	DWORD dwCurSubQuestIdx = pQuest->GetCurSubQuestIdx() ;							// ���� �������� ���� ����Ʈ �ε����� �޴´�.

	if( dwCurSubQuestIdx != 0 )
	{
		--dwCurSubQuestIdx ;
	}

	CSubQuestInfo* pSubQuestInfo = NULL ;											// ���� ����Ʈ ������ ���� �����͸� �����Ѵ�.
	pSubQuestInfo = pQuest->GetSubQuestInfo(dwCurSubQuestIdx) ;

	if( !pSubQuestInfo ) 
	{
		/*sprintf(tempBuf, "[SetQuestPrevInfo] - (%d)�� ����Ʈ�� (%d)�� ���� ����Ʈ ������ ���� ���ؼ� �Ƿ��ڸ� �������� ���߽��ϴ�.", dwQuestIdx, dwCurSubQuestIdx-1) ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;
	}

	cPtrList* pList = pSubQuestInfo->GetQuestNpcScriptList() ;

	if( !pList ) 
	{
		/*sprintf(tempBuf, "[SetQuestPrevInfo] - (%d)�� ����Ʈ�� (%d)�� ���� ����Ʈ�� npc ��Ʈ��Ʈ ����Ʈ�� ���� ���� �Ƿ��ڸ� �������� ���߽��ϴ�.", dwQuestIdx, dwCurSubQuestIdx-1) ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;
	}

	BOOL bNpcResult = FALSE ;

	CQuestNpcScript* pScript ;

	PTRLISTPOS scriptPos = NULL ;
	scriptPos = pList->GetHeadPosition() ;

	if(!scriptPos)
	{
		m_dwNpcIdx = 0 ;
	}

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

			m_dwNpcIdx = pScriptData->GetNpcIdx() ;
			m_dwScriptPage = pScriptData->GetNpcScriptPage() ;
			m_dwDialogType = pScriptData->GetDialogType() ;

			stNPCIDNAME* pNpcIdName = QUESTMGR->GetNpcIdName(m_dwNpcIdx) ;			// ����Ʈ �Ŵ������� npc ���̵�, �̸��� �޴´�.

			if( !pNpcIdName ) continue ;

			memset(tempBuf, 0, sizeof(tempBuf)) ;									// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
			sprintf(tempBuf, CHATMGR->GetChatMsg( 1446 ), pNpcIdName->name) ;
			m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(255, 255, 0, 255));		// ����Ʈ ���� ����Ʈ�� �߰��Ѵ�.

			bNpcResult = TRUE ;

			break ;
		}

		if( bNpcResult ) break ;
	}

	CQuestInfo* pQuestInfo = QUESTMGR->GetQuestInfo(dwQuestIdx) ;					// ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.

	if( !pQuestInfo )																// ����Ʈ ������ ��ȿ���� �ʴٸ�,
	{
		/*sprintf(tempBuf, "[SetQuestPrevInfo] - (%d)�� ����Ʈ ������ �޾ƿ��� ���� ����Ʈ ���� ��ġ�� �������� ���߽��ϴ�.", dwQuestIdx) ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;																	// ���� ó���� �Ѵ�.
	}

	// - ��ġ�� �����Ѵ�. -
	int nMapNum = pQuestInfo->GetMapNum() ;											// ����Ʈ �������� �� ��ȣ�� �޴´�.
	memset(tempBuf, 0, sizeof(tempBuf)) ;											// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
	char* pMapName = GetMapName(nMapNum) ;											// �� ��ȣ�� ���� �� �̸��� �޴´�.	
	sprintf(tempBuf, CHATMGR->GetChatMsg( 1447 ), pMapName) ;
	m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(255, 255, 0, 255));				// ����Ʈ ���� �߰��Ѵ�.
}





void CQuestDialog::RequestInfo(CQuestString* pQuestString)
{
	char tempBuf[128] = {0, } ;

	if( !pQuestString )
	{
		/*sprintf(tempBuf, "[RequestInfo] - ����Ʈ ��Ʈ�� ������ ��ȿ���� �ʾ�, �Ƿڳ����� �������� ���߽��ϴ�.") ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;
	}

	DWORD dwQuestIdx = pQuestString->GetQuestID() ;

	CQuest* pQuest = NULL ;
	pQuest = QUESTMGR->GetQuest(dwQuestIdx) ;

	CQuestInfo* pQuestInfo = QUESTMGR->GetQuestInfo(dwQuestIdx) ;				// ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.

	if( !pQuestInfo )																// ����Ʈ ������ ��ȿ���� �ʴٸ�,
	{
		/*sprintf(tempBuf, "[QuestDescView] - (%d)�� ����Ʈ ������ �޾ƿ��� ���� ����Ʈ ����â�� �������� ���߽��ϴ�.", dwQuestIdx) ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;																	// ���� ó���� �Ѵ�.
	}

	CSubQuestInfo* pSubQuestInfo = NULL ;											// ���� ����Ʈ ������ ���� �����͸� �����Ѵ�.
	CQuestString* pQString ;

	PTRLISTPOS pos ;

	cPtrList* pTitleList ;
	cPtrList* pDescList ;

	int QuestStringKey = 0 ;

	int nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// ���� ����Ʈ ���� �޴´�.

    for( int count = 0 ; count < nSubQuestCount ; ++count )							// ���� ����Ʈ �� ��ŭ for���� ������.
	{
		pSubQuestInfo = NULL ;													// ���� ����Ʈ ������ ���� �����͸� nulló�� �Ѵ�.
		pSubQuestInfo = pQuest->GetSubQuestInfoArray(count) ;					// ī��Ʈ�� �ش��ϴ� ���� ����Ʈ�� �޴´�.

		if( !pSubQuestInfo ) continue ;											// ���� ����Ʈ ������ ��ȿ���� ������ continue�Ѵ�.
		
		COMBINEKEY(dwQuestIdx, count, QuestStringKey) ;							// ����Ʈ, ���� ����Ʈ �ε����� ����Ʈ ��Ʈ���� ���Ѵ�.

		SUBQUEST* pSubQuest = pQuest->GetSubQuest(count) ;							// ���� ����Ʈ ������ �޴´�.

		pQString = NULL ;
		pQString = QUESTMGR->GetQuestString(QuestStringKey);						// ���� ����Ʈ ��Ʈ���� �޴´�.

		if( !pQString )	continue ;													// ����Ʈ ��Ʈ�� ������ ��ȿ���� ������ continue�Ѵ�.

		// - ���� ����Ʈ Ÿ��Ʋ ���� -
		pTitleList = NULL ;
		pTitleList = pQString->GetTitle() ;											// ���� ����Ʈ�� Ÿ��Ʋ ����Ʈ�� �޴´�.

		if( !pTitleList ) continue ;												// Ÿ��Ʋ ����Ʈ�� ��ȿ���� ������ continue�Ѵ�.

		m_pQuestDescList->AddItem(" ", 0);											// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.

		int nTitleCount = 0 ;														// ���� ����Ʈ�� Ÿ��Ʋ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.

		nTitleCount = pTitleList->GetCount() ;										// ���� ����Ʈ�� Ÿ��Ʋ�� �����Ѵ�.

		for( int count2 = 0 ; count2 < nTitleCount ; ++count2 )						// ���� Ÿ��Ʋ ī��Ʈ ��ŭ for���� ������.
		{
			pos = NULL ;															// ��ġ ������ ���� �����͸� nulló���� �Ѵ�.
			pos = pTitleList->FindIndex(count2) ;									// Ÿ��Ʋ�� ��ġ�� �޴´�.

			if( !pos ) continue ;													// ��ġ ������ ��ȿ���� ������ continue�Ѵ�.

			ITEM* pTitleLine = (ITEM*)pTitleList->GetAt(pos) ;						// Ÿ��Ʋ ��Ʈ���� �޴´�.

			if( !pTitleLine ) continue ;											// Ÿ��Ʋ ��Ʈ���� ��ȿ���� ������, continue�Ѵ�.

			if( !pSubQuest ) continue ;												// ���� ����Ʈ ������ ��ȿ���� ������ continue�Ѵ�.

			DWORD dwCurCount = pSubQuest->dwData ;									// ���� ������ �޴´�.
			DWORD dwMaxCount = pSubQuest->dwMaxCount ;								// �ִ� ������ �޴´�.

			memset(tempBuf, 0, sizeof(tempBuf)) ;									// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.

			if( pQString->IsComplete() )											// ����Ʈ�� �Ϸ� �Ǿ�����,
			{
				//sprintf(tempBuf, "%s (�Ϸ�)", pTitleLine->string) ;					// ���� �ϷḦ �߰��Ѵ�.
				sprintf(tempBuf, "%s", pTitleLine->string) ;
				strcat(tempBuf, CHATMGR->GetChatMsg(1463)) ;

				m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(255, 255, 0, 255)) ;	// ����� �ϷḦ ����Ѵ�.
			}
			else																	// �Ϸ��� ����Ʈ�� �ƴϸ�,
			{	
				if( pSubQuest->dwMaxCount == 0 )									// ���� ����Ʈ�� ��Ż ī��Ʈ�� 0�� ������,
				{
					if( pSubQuest->dwData == 0 )									// ���� ����Ʈ�� ���� ī��Ʈ�� 0�� ������,
					{
						sprintf(tempBuf, "%s", pTitleLine->string) ;				// ����Ʈ ���� �ӽù��ۿ� �����Ѵ�.
					}
					else															// ���� ����Ʈ�� ���� ī��Ʈ�� 0�� �ƴϸ�,
					{
						sprintf(tempBuf, "%s (%d)", pTitleLine->string, pSubQuest->dwData) ; // ���� ����Ʈ�� �����, ������ �ӽ� ���ۿ� �����Ѵ�.
					}
				}
				else																// ���� ����Ʈ�� ��Ż ī��Ʈ�� 0�� �ƴϸ�,
				{
					sprintf(tempBuf, "%s (%d/%d)", pTitleLine->string, pSubQuest->dwData, pSubQuest->dwMaxCount) ; // ���� ����Ʈ�� ����� ���� ī��Ʈ�� �ӽ� ���ۿ� �����Ѵ�.
				}										

				m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(255, 255, 0, 255)) ;		// ���õ� ��Ʈ���� ����Ʈ ���� ����Ʈ ���̾�α׿� �߰��Ѵ�.
			}
		}

        // - ���� ����Ʈ ���� ����
		pDescList = pQString->GetDesc() ;										// ���� ����Ʈ ���� ����Ʈ�� �޴´�.

		if( !pDescList ) continue ;												// ���� ����Ʈ ������ ��ȿ���� ������, continue�Ѵ�.

		int nDescCount = 0 ;													// ���� ������ ���� ������ �����ϰ� 0���� �����Ѵ�.

		nDescCount = pDescList->GetCount() ;									// ���� ����Ʈ�� ������ ���� �޴´�.

		for( int count2 = 0 ; count2 < nDescCount ; ++count2 )					// ���� ���� ��ŭ for���� ������.
		{
			pos = NULL ;														// ��ġ ������ ���� �����͸� nulló���� �Ѵ�.
			pos = pDescList->FindIndex(count2) ;								// ī��Ʈ�� �ش��ϴ� ��ġ ������ �޴´�.

			if( !pos ) continue ;												// ��ġ ������ ��ȿ���� ������ continue�Ѵ�.

			ITEM* pDescLine = (ITEM*)pDescList->GetAt(pos) ;  					// ���� ��Ʈ���� �޴´�.

			if( pDescLine )														// ���� ��Ʈ�� ������ ��ȿ�ϴٸ�,						
			{
				m_pQuestDescList->AddItem(pDescLine->string, pDescLine->rgb) ;	// ����Ʈ ���� ����Ʈ�� �߰��� �Ѵ�.
			}
		}

		pTitleList = NULL ;														// ���� ����Ʈ �����͸� nulló���� �Ѵ�.
		pDescList  = NULL ;														// ���� ����Ʈ �����͸� nulló���� �Ѵ�.

		pQString = NULL ;														// ��Ʈ�� �����͸� nulló���� �Ѵ�.			
	}

	m_pQuestDescList->AddItem(" ", 0);												// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.
	m_pQuestDescList->AddItem(CHATMGR->GetChatMsg( 1448 ), RGBA_MAKE(255, 255, 0, 255));				// <�Ƿڳ���>�� �߰��Ѵ�.
	if( m_dwNpcIdx != 0 )
	{
		m_pQuestDescList->AddItem(" ", 0);												// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.
	}

	cDialogueList* pDialogueList = NULL ;											// ���̾�α� ����Ʈ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
	pDialogueList = NPCSCRIPTMGR->GetDialogueList();								// NPC ��ũ��Ʈ �Ŵ������� ���̾�α� ����Ʈ�� �޴´�.

	cPage* pPage = NPCSCRIPTMGR->GetPage( m_dwNpcIdx, m_dwScriptPage );					// ���� NPC�� ���� ������ ���̵�� ������ ������ �޴´�.

	WORD wIdx = 0;																	// �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
	WORD LineInfo = 0;																// ���� ������ ���� ������ �����ϰ� 0���� �����Ѵ�.

	DIALOGUE* temp = NULL;															// ���̾�α� ������ ���� ������ ������ �����ϰ� NULLó���� �Ѵ�.

	LINKITEM* pItem = NULL;															// ��ũ ������ ������ ���� ������ ������ �����ϰ� NULL ó���� �Ѵ�.
	LINKITEM* Prev = NULL;															// ���� ��ũ ������ ������ ���� ������ ������ �����ϰ� NULL ó���� �Ѵ�.

	char npcDialog[1024] = {0, } ;													// npc ��縦 ���� �ӽ� ���۸� �����Ѵ�.

	if( m_dwDialogType == 0 )
	{
		while(TRUE)
		{
			if( pPage )
			{
				DWORD dwDialogue = pPage->GetDialogue() ;										// �������� ���̾�α� ��ȣ�� �޴´�.

				memset(npcDialog, 0, 1024) ;

				wIdx = 0 ;

				while(TRUE)																// while���� ������.
				{
					temp = pDialogueList->GetDialogue( dwDialogue, wIdx );				// �����ϰ� ���� �������� ���̾�α� ������ �޴´�.

					if( temp == NULL ) break;											// ������ ��ȿ���� ������, while���� ����������.

					strcat(npcDialog, temp->str) ;										// �о���� ��縦 �ӽ� ���ۿ� �߰��Ѵ�.

					++wIdx ;															// �ε����� �����Ѵ�.
				}

				m_pQuestDescList->AddItemAutoLine(npcDialog, RGBA_MAKE(255, 255, 255, 255)) ;

				m_pQuestDescList->AddItem(" ", 0);										// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.

				HYPERLINK* pHyperLink = pPage->GetHyperText(0) ;

				if( pHyperLink )
				{
					m_dwScriptPage = pHyperLink->dwData ;

					pPage = NPCSCRIPTMGR->GetPage( m_dwNpcIdx, m_dwScriptPage );			// ���� NPC�� ���� ������ ���̵�� ������ ������ �޴´�.
				}
				else
				{
					break ;
				}
			}
			else
			{
				break ;
			}
		}
	}
	else
	{
		int nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// ���� ����Ʈ ���� �޴´�.

		cDialogueList* pDialogueList = NULL ;											// ���̾�α� ����Ʈ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
		pDialogueList = NPCSCRIPTMGR->GetDialogueList();								// NPC ��ũ��Ʈ �Ŵ������� ���̾�α� ����Ʈ�� �޴´�.
		if( pDialogueList == NULL ) return ;											// ���̾�α� ������ ��ȿ���� ������, FALSE ���� ó���� �Ѵ�.

		CSubQuestInfo* pSubQuestInfo = NULL ;											// ���� ����Ʈ ������ ���� �����͸� �����ϰ� NULLó���� �Ѵ�.
		pSubQuestInfo = pQuestInfo->GetSubQuestInfo(0) ;								// ī��Ʈ�� �ش��ϴ� ���� ����Ʈ ������ �޴´�.

		if( !pSubQuestInfo ) return ;

		DWORD dwPage = pSubQuestInfo->GetNpcScriptPage(m_dwNpcIdx) ;						// NPC �ε����� ������ �ε����� �޴´�.
		cPage* pPage = NPCSCRIPTMGR->GetPage( m_dwNpcIdx, dwPage );						// ���� NPC�� ���� ������ ���̵�� ������ ������ �޴´�.

		while(TRUE)
		{
			if( pPage )
			{
				DWORD dwDialogue = pPage->GetDialogue() ;										// �������� ���̾�α� ��ȣ�� �޴´�.

				memset(npcDialog, 0, 1024) ;

				wIdx = 0 ;

				while(TRUE)																// while���� ������.
				{
					temp = pDialogueList->GetDialogue( dwDialogue, wIdx );				// �����ϰ� ���� �������� ���̾�α� ������ �޴´�.

					if( temp == NULL ) break;											// ������ ��ȿ���� ������, while���� ����������.

					strcat(npcDialog, temp->str) ;										// �о���� ��縦 �ӽ� ���ۿ� �߰��Ѵ�.

					++wIdx ;															// �ε����� �����Ѵ�.
				}

				m_pQuestDescList->AddItemAutoLine(npcDialog, RGBA_MAKE(255, 255, 255, 255)) ;

				m_pQuestDescList->AddItem(" ", 0);										// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.

				HYPERLINK* pHyperLink = pPage->GetHyperText(0) ;

				if( pHyperLink )
				{
					dwPage = pHyperLink->dwData ;

					pPage = NPCSCRIPTMGR->GetPage( m_dwNpcIdx, dwPage );					// ���� NPC�� ���� ������ ���̵�� ������ ������ �޴´�.
				}
				else
				{
					break ;
				}
			}
			else
			{
				break ;
			}
		}
	}
}




void CQuestDialog::RequitalInfo(CQuestString* pQuestString)
{
	char tempBuf[128] = {0, } ;

	// 080103 LYW --- QuestDialog : ���� ����ġ�� ���� �Ӵ� �ʱ�ȭ ó�� �߰�.
	SetGold(0) ;
	SetExp(0) ;

	if( !pQuestString )
	{
		/*sprintf(tempBuf, "[RequitalInfo] - ����Ʈ ��Ʈ�� ������ ��ȿ���� �ʾ�, �Ƿڳ����� �������� ���߽��ϴ�.") ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;
	}

	DWORD dwQuestIdx = pQuestString->GetQuestID() ;

	CQuest* pQuest = NULL ;
	pQuest = QUESTMGR->GetQuest(dwQuestIdx) ;

	CQuestInfo* pQuestInfo = QUESTMGR->GetQuestInfo(dwQuestIdx) ;				// ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.

	if( !pQuestInfo )																// ����Ʈ ������ ��ȿ���� �ʴٸ�,
	{
		/*sprintf(tempBuf, "[RequitalInfo] - (%d)�� ����Ʈ ������ �޾ƿ��� ���� ����Ʈ ����â�� �������� ���߽��ϴ�.", dwQuestIdx) ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;																	// ���� ó���� �Ѵ�.
	}

	cIcon* pDeleteIcon = NULL ;
	RequitalItem* pDeleteItem = NULL ;

	PTRLISTPOS deletePos = NULL ;
	deletePos = m_QuestRequitalList.GetHeadPosition() ;

	while(deletePos)
	{
		pDeleteItem = NULL ;
		pDeleteItem = (RequitalItem*)m_QuestRequitalList.GetNext(deletePos) ;

		if( !pDeleteItem ) continue ;

		m_pRequitalGrid->DeleteIcon( pDeleteItem->wPos, &pDeleteItem->pIcon ) ;

		pDeleteItem->pIcon->SetActive(FALSE) ;
		WINDOWMGR->AddListDestroyWindow(pDeleteItem->pIcon) ;

		m_IndexGenerator.ReleaseIndex( pDeleteItem->pIcon->GetID() ) ;

		m_QuestRequitalList.Remove(pDeleteItem) ;

		delete pDeleteItem ;
	}

	m_QuestRequitalList.RemoveAll() ;

	RefreshRequitalItem() ;


	int nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// ���� ����Ʈ ���� �޴´�.

	if( nSubQuestCount < 1 )
	{
		/*sprintf(tempBuf, "[RequitalInfo] - (%d)�� ����Ʈ�� ���� ����Ʈ ������ ����, ���� ������ ó���� ���� ���߽��ϴ�.", dwQuestIdx) ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;																	// ���� ó���� �Ѵ�.
	}

	CSubQuestInfo* pSubQuestInfo = pQuestInfo->GetSubQuestInfo(nSubQuestCount-1) ;					// ������ ���� ����Ʈ ������ �޴´�.

	if( !pSubQuestInfo )																// ���� ����Ʈ ������ ��ȿ�ϴٸ�,
	{
		/*sprintf(tempBuf, "[RequitalInfo] - (%d)�� ����Ʈ�� ���� ����Ʈ ������ ����, ���� ������ ó���� ���� ���߽��ϴ�.", dwQuestIdx) ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;																	// ���� ó���� �Ѵ�.
	}

	cPtrList* pTriggerList = pSubQuestInfo->GetTriggerList() ;					// ������ ���� ����Ʈ�� Ʈ���� ������ �޴´�.

	if( !pTriggerList )															// Ʈ���� ������ ��ȿ�ϴٸ�,
	{
		/*sprintf(tempBuf, "[RequitalInfo] - (%d)�� ����Ʈ�� Ʈ���� ������ ���� ����, ���� ������ ó���� ���� ���߽��ϴ�.", dwQuestIdx) ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;																	// ���� ó���� �Ѵ�.
	}

	int nTriggerCount = 0 ;													// Ʈ���� ī��Ʈ�� ���� ������ �����ϰ�, 0���� �����Ѵ�.

	nTriggerCount = pTriggerList->GetCount() ;								// Ʈ���� ī��Ʈ�� �޴´�.

	CQuestTrigger* pTrigger = NULL ;										// Ʈ���� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.

	PTRLISTPOS pos ;

	PTRLISTPOS exePos = NULL ;												// ���� ����Ʈ ��ġ ������ ���� ��ġ �����͸� �����ϰ� null ó���� �Ѵ�.

	for( int count = 0 ; count < nTriggerCount ; ++count )					// Ʈ���� ī��Ʈ �� ��ŭ for���� ������.
	{
		pos = NULL ;														// ��ġ ������ ���� �����͸� null ó���� �Ѵ�.
		pos = pTriggerList->FindIndex(count) ;								// ī��Ʈ�� �ش��ϴ� ��ġ ������ �޴´�.

		if( !pos ) continue ;												// ��ġ ������ ��ȿ�ϴٸ�,

		pTrigger = NULL ;
		pTrigger = (CQuestTrigger*)pTriggerList->GetAt(pos) ;			// ��ġ�� �ش��ϴ� Ʈ���� ������ �޴´�.

		if( !pTrigger )	 continue ;										// Ʈ���� ������ ��ȿ�ϴٸ�,

		cPtrList* pExeList = NULL ;
		pExeList = pTrigger->GetExeList() ;								// Ʈ���� ���� ����Ʈ�� �޴´�.

		if( !pExeList )	continue ;										// Ʈ���� ���� ����Ʈ ������ ��ȿ�ϴٸ�,

		int nExeCount = pExeList->GetCount() ;					// ���� ����Ʈ ī��Ʈ�� �޴´�.

		CQuestExecute* pExecute = NULL ;						// ����Ʈ ���� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.

		for(int count2 = 0 ; count2 < nExeCount ; ++count2 )	// ���� ī��Ʈ ��ŭ for���� ������.
		{
			exePos = NULL ;										// ���� ��ġ �����͸� null ó���� �Ѵ�.
			exePos = pExeList->FindIndex(count2) ;				// ī��Ʈ�� �ش��ϴ� ���� ��ġ ������ �޴´�.									

			if( !exePos ) continue ;										// ����Ʈ ���� ��ġ�� ��ȿ�ϸ�,

			pExecute = (CQuestExecute*)pExeList->GetAt(exePos) ;	// ����Ʈ ���� ������ �޴´�.

			if( !pExecute ) continue ;								// ����Ʈ ���� ������ ��ȿ�ϸ�,

			CQuestExecute_Item* pExecute_Item = ((CQuestExecute_Item*)pExecute) ;
			DWORD dwExeKind = pExecute->GetQuestExecuteKind() ;	// ���� Ÿ���� �޴´�.

			switch(dwExeKind)							// ���� Ÿ���� Ȯ���Ѵ�.
			{
			case eQuestExecute_TakeItem :				// ���� Ÿ���� �������� �޴� Ÿ���̶��,
				{
					ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(pExecute_Item->GetItemKind());

					if( pItemInfo )
					{
						AddRequitalItem(pItemInfo, pExecute_Item->GetItemNum()) ;
					}
				}
				break ;

			case eQuestExecute_TakeMoney :				// ���� Ÿ���� ��带 �޴� Ÿ���̶��,
				{
					SetGold( pExecute_Item->GetItemNum() ) ;
				}
				break ;

			case eQuestExecute_TakeExp :				// ���� Ÿ���� ����ġ�� �޴� Ÿ���̶��,
				{
					SetExp( pExecute_Item->GetItemNum() ) ;
				}
				break ;

			}
		}
	}
}





void CQuestDialog::QuestDescView(CQuestString* pQuestString)						// ����Ʈ �� ������ ó�� �ϴ� �Լ�.
{
	if( !m_bActive ) return ;														// ����Ʈ â�� ���� ���°� �ƴ϶��, ���� ó���� �Ѵ�.

	if(!pQuestString)																// ���ڷ� �Ѿ�� ����Ʈ ��Ʈ�� ������ �������� ������ ����ó���Ѵ�.
	{
		m_pQuestDescList->RemoveAll() ;												// ���� ����Ʈ ���̾�α׸� ��� ����.

		int nCellCount = m_pRequitalGrid->GetCellNum() ;							// ������ ���� �׸����� �� ī��Ʈ�� �޴´�.

		cIcon* pIcon ;																// ������ ������ ���� �����͸� �����Ѵ�.

		for( int count = 0 ; count < nCellCount ; ++count )							// �׸��� �� ī��Ʈ  ��ŭ for���� ������.
		{
			pIcon = NULL ;															// ������ ������ ���� �����͸� nulló�� �Ѵ�.
			pIcon = m_pRequitalGrid->GetIconForIdx(count) ;							// ī��Ʈ�� �ش��ϴ� ������ ������ ������ �޴´�.

			if( pIcon )																// ������ ������ ��ȿ�ϸ�,
			{
				m_pRequitalGrid->DeleteIcon(pIcon) ;								// ���� ������ �׸��忡�� �������� �����Ѵ�.
			}
		}

		RequitalItem* item ;														// ���� �������� ������ ���� �����͸� �����Ѵ�.

		PTRLISTPOS pos = m_QuestRequitalList.GetHeadPosition() ;					// ����Ʈ ���� ������ ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

		while( pos )																// ��ġ ������ ��ȿ�� ���� while���� ������.
		{
			item = NULL ;															// ���� ������ ������ ���� �����͸� nulló�� �Ѵ�.
			item = ( RequitalItem* )m_QuestRequitalList.GetNext( pos ) ;			// ��ġ�� �ش��ϴ� ��������� ������ �ް�, ��ġ�� �������� �����Ѵ�.

			if( item )																// ������ ������ ��ȿ�ϸ�,
			{
				WINDOWMGR->AddListDestroyWindow( item->pIcon );						// ������ �Ŵ����� ������ ������� ���� �������� �������� ����Ѵ�.
				SAFE_DELETE( item ) ;												// ������ ������ �����ϰ� �����Ѵ�.
			}
		}

		m_QuestRequitalList.RemoveAll() ;											// ���� ������ ����Ʈ�� ��� ����.

		SetGold(0) ;																// ���� ��� ��ġ�� 0���� �����Ѵ�.
		SetExp(0) ;																	// ���� ����ġ ��ġ�� 0���� �����Ѵ�.

		return;																		// return ó�� �Ѵ�.
	}

	m_pQuestDescList->RemoveAll() ;													// ���� ����Ʈ ���̾�α׸� ��� ����.

	SetQuestPrevInfo(pQuestString) ;

	RequestInfo(pQuestString) ;

	RequitalInfo(pQuestString) ;

	m_pQuestDescList->ResetGuageBarPos() ;
}

//void CQuestDialog::QuestDescView(CQuestString* pQuestString)						// ����Ʈ �� ������ ó�� �ϴ� �Լ�.
//{
//	if( !m_bActive ) return ;														// ����Ʈ â�� ���� ���°� �ƴ϶��, ���� ó���� �Ѵ�.
//
//	if(!pQuestString)																// ���ڷ� �Ѿ�� ����Ʈ ��Ʈ�� ������ �������� ������ ����ó���Ѵ�.
//	{
//		m_pQuestDescList->RemoveAll() ;												// ���� ����Ʈ ���̾�α׸� ��� ����.
//
//		int nCellCount = m_pRequitalGrid->GetCellNum() ;							// ������ ���� �׸����� �� ī��Ʈ�� �޴´�.
//
//		cIcon* pIcon ;																// ������ ������ ���� �����͸� �����Ѵ�.
//
//		for( int count = 0 ; count < nCellCount ; ++count )							// �׸��� �� ī��Ʈ  ��ŭ for���� ������.
//		{
//			pIcon = NULL ;															// ������ ������ ���� �����͸� nulló�� �Ѵ�.
//			pIcon = m_pRequitalGrid->GetIconForIdx(count) ;							// ī��Ʈ�� �ش��ϴ� ������ ������ ������ �޴´�.
//
//			if( pIcon )																// ������ ������ ��ȿ�ϸ�,
//			{
//				m_pRequitalGrid->DeleteIcon(pIcon) ;								// ���� ������ �׸��忡�� �������� �����Ѵ�.
//			}
//		}
//
//		RequitalItem* item ;														// ���� �������� ������ ���� �����͸� �����Ѵ�.
//
//		PTRLISTPOS pos = m_QuestRequitalList.GetHeadPosition() ;					// ����Ʈ ���� ������ ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.
//
//		while( pos )																// ��ġ ������ ��ȿ�� ���� while���� ������.
//		{
//			item = NULL ;															// ���� ������ ������ ���� �����͸� nulló�� �Ѵ�.
//			item = ( RequitalItem* )m_QuestRequitalList.GetNext( pos ) ;			// ��ġ�� �ش��ϴ� ��������� ������ �ް�, ��ġ�� �������� �����Ѵ�.
//
//			if( item )																// ������ ������ ��ȿ�ϸ�,
//			{
//				WINDOWMGR->AddListDestroyWindow( item->pIcon );						// ������ �Ŵ����� ������ ������� ���� �������� �������� ����Ѵ�.
//				SAFE_DELETE( item ) ;												// ������ ������ �����ϰ� �����Ѵ�.
//			}
//		}
//
//		m_QuestRequitalList.RemoveAll() ;											// ���� ������ ����Ʈ�� ��� ����.
//
//		SetGold(0) ;																// ���� ��� ��ġ�� 0���� �����Ѵ�.
//		SetExp(0) ;																	// ���� ����ġ ��ġ�� 0���� �����Ѵ�.
//
//		return;																		// return ó�� �Ѵ�.
//	}
//
//	char tempBuf[128] = {0, } ;														// �ӽù��۸� �����ϰ� �ʱ�ȭ �Ѵ�.
//
//	PTRLISTPOS pos = NULL;															// ��ġ ������ ���� ������ ������ �����ϰ� nulló�� �Ѵ�.
//
//	DWORD dwMainQuestID = pQuestString->GetQuestID() ;								// ��Ʈ������ ����Ʈ ��ȣ�� �޴´�.
//
//	CQuestInfo* pQuestInfo = QUESTMGR->GetQuestInfo(dwMainQuestID) ;				// ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.
//
//	if( !pQuestInfo )																// ����Ʈ ������ ��ȿ���� �ʴٸ�,
//	{
//		return ;																	// ���� ó���� �Ѵ�.
//	}
//
//	int nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// ���� ����Ʈ ���� �޴´�.
//	
//	m_pQuestDescList->RemoveAll();													// ����Ʈ ����Ʈ ���̾�α׸� ��� ����.
//
//	ITEM* pQStr = NULL ;
//
//	int QuestStringKey = 0 ;														// ����Ʈ ��Ʈ�� Ű�� ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//	CQuestString* pQString = NULL ;													// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	cPtrList* pTitleList = NULL ;													// ���� ����Ʈ ���� ����Ʈ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	cPtrList* pDescList = NULL ;													// ���� ����Ʈ ���� ����Ʈ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	CQuest* pQuest = QUESTMGR->GetQuest( dwMainQuestID );							// ����Ʈ ������ �޴´�.
//
//	if( !pQuest ) return ;															// ����Ʈ ������ ��ȿ���� ������, return ó�� �Ѵ�.
//
//	CSubQuestInfo* pDescSubQuestInfo ;												// ���� ����Ʈ ������ ���� �����͸� �����Ѵ�.
////	CSubQuestInfo* pDescCurSubQuestInfo ;
//
//	////////////////////////////////////////
//	// - ����Ʈ �̸�, �Ƿ���, ��ġ ���� -
//	////////////////////////////////////////
//	// - ����Ʈ ���� ���� - 
//	CQuestString* pTitleString = NULL ;												// ����Ʈ ���� ��Ʈ�� ������ ���� �����͸� �����ϰ� nulló�� �Ѵ�.
//	pTitleString = QUESTMGR->GetQuestString(pQuestString->GetQuestID(), 0) ;		// ����Ʈ ���� ��Ʈ���� �޴´�.
//
//	pos = pTitleString->GetTitle()->GetHeadPosition();								// ����Ʈ ��Ʈ���� �ص� ��ġ�� �����Ѵ�.
//
//	while(pos)																		// ��ġ ������ ��ȿ�� ���� while���� ������.
//	{
//		pQStr = (ITEM*)pTitleString->GetTitle()->GetNext(pos);						// ��ġ ������ �ش��ϴ� ����Ʈ ��Ʈ�� ������ �ް�, ��ġ�� �������� �����Ѵ�.
//
//		if(pQStr)																	// ����Ʈ ��Ʈ�� ������ ��ȿ�ϴٸ�,
//		{
//			sprintf(tempBuf, "< %s >", pQStr->string) ;								// �ӽ� ���ۿ� ����Ʈ ������ �����Ѵ�.
//			m_pQuestDescList->AddItem(tempBuf, MAINQUEST_TITLE_COLOR);				// ����Ʈ ����Ʈ ���̾�α׿� �߰��Ѵ�.
//			++m_toPreList;															// ���� ����Ʈ ���� �����Ѵ�.
//		}
//	}
//
//	m_pQuestDescList->AddItem(" ", 0);												// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.
//	++m_toPreList;																	// ���� ����Ʈ ���� �����Ѵ�.
//
//	DWORD dwNpcIdx = 0 ;															// ����Ʈ �������� npc �ε����� �޴´�. �޴´�.
//	DWORD dwScriptPage = 0 ;
//	DWORD dwDialogType = 0 ;
//	DWORD dwCurSubQuestIdx = pQuest->GetCurSubQuestIdx() ;							// ���� �������� ���� ����Ʈ �ε����� �޴´�.
//
//	pDescSubQuestInfo = NULL ;
//	pDescSubQuestInfo = pQuest->GetSubQuestInfo(dwCurSubQuestIdx-1) ;
//
//	if( !pDescSubQuestInfo ) return ;
//
//	cPtrList* pList = pDescSubQuestInfo->GetQuestNpcScriptList() ;
//
//	if( !pList ) return ;
//
//	CQuestNpcScript* pScript ;
//	PTRLISTPOS scriptPos = pList->GetHeadPosition() ;
//
//	BOOL bNpcResult = FALSE ;
//
//	while(scriptPos)
//	{
//		pScript = NULL ;
//		pScript = (CQuestNpcScript*)pList->GetNext(scriptPos) ;
//
//		if(!pScript) continue ;
//
//		cPtrList* pNpcScriptList = pScript->GetNpcScriptList() ;
//		if( !pNpcScriptList ) continue ;
//
//		CQuestNpcScriptData* pScriptData ;
//		PTRLISTPOS dataPos = pNpcScriptList->GetHeadPosition() ;
//
//		while(dataPos)
//		{
//			pScriptData = NULL ;
//			pScriptData = (CQuestNpcScriptData*)pNpcScriptList->GetNext(dataPos) ;
//
//			if(!pScriptData) continue ;
//
//			dwNpcIdx = pScriptData->GetNpcIdx() ;
//			dwScriptPage = pScriptData->GetNpcScriptPage() ;
//			dwDialogType = pScriptData->GetDialogType() ;
//
//			stNPCIDNAME* pNpcIdName = QUESTMGR->GetNpcIdName(dwNpcIdx) ;					// ����Ʈ �Ŵ������� npc ���̵�, �̸��� �޴´�.
//
//			if( !pNpcIdName ) continue ;
//
//			memset(tempBuf, 0, sizeof(tempBuf)) ;										// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
//			sprintf(tempBuf, "�Ƿ��� - %s", pNpcIdName->name) ;					// �Ƿ��� �̸����� npc �̸��� �����Ѵ�.
//			m_pQuestDescList->AddItem(tempBuf, SUBQUEST_TITLE_COLOR_SEL);				// ����Ʈ ���� ����Ʈ�� �߰��Ѵ�.
//
//			bNpcResult = TRUE ;
//
//			break ;
//		}
//
//		if( bNpcResult ) break ;
//	}
//
//	//int nPrevCount = 0 ;
//
//	// for( int count = 0 ; count < nSubQuestCount ; ++count )							// ���� ����Ʈ �� ��ŭ for���� ������.
//	//{
//	//	pDescSubQuestInfo = NULL ;													// ���� ����Ʈ ������ ���� �����͸� nulló�� �Ѵ�.
//	//	pDescSubQuestInfo = pQuest->GetSubQuestInfoArray(count) ;					// ī��Ʈ�� �ش��ϴ� ���� ����Ʈ�� �޴´�.
//
//	//	if( !pDescSubQuestInfo ) continue ;											// ���� ����Ʈ ������ ��ȿ���� ������ continue�Ѵ�.
//
//	//	pDescCurSubQuestInfo = pQuest->GetSubQuestInfo(0) ;
//
//	//	if( !pDescCurSubQuestInfo ) continue ;
//
//	//	cPtrList* pList = pDescCurSubQuestInfo->GetQuestNpcScriptList() ;
//
//	//	if( !pList ) continue ;
//
//	//	CQuestNpcScript* pScript ;
//	//	PTRLISTPOS scriptPos = pList->GetHeadPosition() ;
//
//	//	while(scriptPos)
//	//	{
//	//		pScript = NULL ;
//	//		pScript = (CQuestNpcScript*)pList->GetNext(scriptPos) ;
//
//	//		if(!pScript) continue ;
//
//	//		cPtrList* pNpcScriptList = pScript->GetNpcScriptList() ;
//	//		if( !pNpcScriptList ) continue ;
//
//	//		CQuestNpcScriptData* pScriptData ;
//	//		PTRLISTPOS dataPos = pNpcScriptList->GetHeadPosition() ;
//
//	//		while(dataPos)
//	//		{
//	//			pScriptData = NULL ;
//	//			pScriptData = (CQuestNpcScriptData*)pNpcScriptList->GetNext(dataPos) ;
//
//	//			if(!pScriptData) continue ;
//
//	//			dwNpcIdx = pScriptData->GetNpcIdx() ;
//
//	//			NPC_LIST* pNpcInfo = GAMERESRCMNGR->GetNpcInfo((WORD)dwNpcIdx);
//
//	//			if( !pNpcInfo ) continue ;
//
//	//			memset(tempBuf, 0, sizeof(tempBuf)) ;										// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
//	//			sprintf(tempBuf, "�Ƿ��� - %s", pNpcInfo->Name) ;							// �Ƿ��� �̸����� npc �̸��� �����Ѵ�.
//	//			m_pQuestDescList->AddItem(tempBuf, SUBQUEST_TITLE_COLOR_SEL);				// ����Ʈ ���� ����Ʈ�� �߰��Ѵ�.
//	//		}
//	//	}
//	//}
//
//	// - ��ġ�� �����Ѵ�. -
//	int nMapNum = pQuestInfo->GetMapNum() ;											// ����Ʈ �������� �� ��ȣ�� �޴´�.
//	memset(tempBuf, 0, sizeof(tempBuf)) ;											// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
//	char* pMapName = GetMapName(nMapNum) ;											// �� ��ȣ�� ���� �� �̸��� �޴´�.	
//	sprintf(tempBuf, "��ġ - %s", pMapName) ;										// ��ġ�� �� �̸��� �߰��Ѵ�.
//	m_pQuestDescList->AddItem(tempBuf, SUBQUEST_TITLE_COLOR_SEL);					// ����Ʈ ���� �߰��Ѵ�.
//
//	//m_toPreList= -1;																// ���� ����Ʈ�� -1�� �����Ѵ�.
//
//	//ITEM* pQStr = NULL;																// ����Ʈ ��Ʈ�� ������ ���� ������ �����ϰ� nulló�� �Ѵ�.
//
//	//// - ����Ʈ ���� ���� - 
//	//CQuestString* pTitleString = NULL ;												// ����Ʈ ���� ��Ʈ�� ������ ���� �����͸� �����ϰ� nulló�� �Ѵ�.
//	//pTitleString = QUESTMGR->GetQuestString(pQuestString->GetQuestID(), 0) ;		// ����Ʈ ���� ��Ʈ���� �޴´�.
//
//	//pos = pTitleString->GetTitle()->GetHeadPosition();								// ����Ʈ ��Ʈ���� �ص� ��ġ�� �����Ѵ�.
//
//	//while(pos)																		// ��ġ ������ ��ȿ�� ���� while���� ������.
//	//{
//	//	pQStr = (ITEM*)pTitleString->GetTitle()->GetNext(pos);						// ��ġ ������ �ش��ϴ� ����Ʈ ��Ʈ�� ������ �ް�, ��ġ�� �������� �����Ѵ�.
//
//	//	if(pQStr)																	// ����Ʈ ��Ʈ�� ������ ��ȿ�ϴٸ�,
//	//	{
//	//		sprintf(tempBuf, "< %s >", pQStr->string) ;								// �ӽ� ���ۿ� ����Ʈ ������ �����Ѵ�.
//	//		m_pQuestDescList->AddItem(tempBuf, MAINQUEST_TITLE_COLOR);				// ����Ʈ ����Ʈ ���̾�α׿� �߰��Ѵ�.
//	//		++m_toPreList;															// ���� ����Ʈ ���� �����Ѵ�.
//	//	}
//	//}
//
//	//m_pQuestDescList->AddItem(" ", 0);												// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.
//	//++m_toPreList;																	// ���� ����Ʈ ���� �����Ѵ�.
//
//	//// - �Ƿ��� ���� - 
//	//if( pNpcIdName )																// npc ������ ��ȿ�ϸ�,
//	//{
//	//	memset(tempBuf, 0, sizeof(tempBuf)) ;										// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
//	//	sprintf(tempBuf, "�Ƿ��� - %s", pNpcIdName->name) ;							// �Ƿ��� �̸����� npc �̸��� �����Ѵ�.
//	//	m_pQuestDescList->AddItem(tempBuf, SUBQUEST_TITLE_COLOR_SEL);				// ����Ʈ ���� ����Ʈ�� �߰��Ѵ�.
//	//}
//	//else																			// npc ������ ��ȿ���� ������,
//	//{
//	//	ASSERTMSG(0, "����Ʈ ����â�� NPC ������ �ε����� ���߽��ϴ�.") ;			// npc ������ �и��� �־�� �Ѵ�.
//	//	m_pQuestDescList->AddItem("<*********>", SUBQUEST_TITLE_COLOR_SEL);			// �˾ƺ� �� �ְ� <*********> �� ǥ���Ѵ�.
//	//}
//
//	//// - ��ġ�� �����Ѵ�. -
//	//memset(tempBuf, 0, sizeof(tempBuf)) ;											// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
//	//char* pMapName = GetMapName(nMapNum) ;											// �� ��ȣ�� ���� �� �̸��� �޴´�.	
//	//sprintf(tempBuf, "��ġ - %s", pMapName) ;										// ��ġ�� �� �̸��� �߰��Ѵ�.
//	//m_pQuestDescList->AddItem(tempBuf, SUBQUEST_TITLE_COLOR_SEL);					// ����Ʈ ���� �߰��Ѵ�.
//
//
//	//////////////////////////////////////////
//	//// - �Ƿ� ���� ���� -
//	//////////////////////////////////////////
//
//	m_pQuestDescList->AddItem(" ", 0);												// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.
//	m_pQuestDescList->AddItem("< �Ƿ� ���� > ", MAINQUEST_TITLE_COLOR);				// <�Ƿڳ���>�� �߰��Ѵ�.
//	m_pQuestDescList->AddItem(" ", 0);												// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.
//
//	cDialogueList* pDialogueList = NULL ;											// ���̾�α� ����Ʈ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	pDialogueList = NPCSCRIPTMGR->GetDialogueList();								// NPC ��ũ��Ʈ �Ŵ������� ���̾�α� ����Ʈ�� �޴´�.
//
//	cPage* pPage = NPCSCRIPTMGR->GetPage( dwNpcIdx, dwScriptPage );					// ���� NPC�� ���� ������ ���̵�� ������ ������ �޴´�.
//
//	WORD wIdx = 0;																	// �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
//	WORD LineInfo = 0;																// ���� ������ ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//	DIALOGUE* temp = NULL;															// ���̾�α� ������ ���� ������ ������ �����ϰ� NULLó���� �Ѵ�.
//
//	LINKITEM* pItem = NULL;															// ��ũ ������ ������ ���� ������ ������ �����ϰ� NULL ó���� �Ѵ�.
//	LINKITEM* Prev = NULL;															// ���� ��ũ ������ ������ ���� ������ ������ �����ϰ� NULL ó���� �Ѵ�.
//
//	char npcDialog[1024] = {0, } ;													// npc ��縦 ���� �ӽ� ���۸� �����Ѵ�.
//
//	if( dwDialogType == 0 )
//	{
//		while(TRUE)
//		{
//			if( pPage )
//			{
//				DWORD dwDialogue = pPage->GetDialogue() ;										// �������� ���̾�α� ��ȣ�� �޴´�.
//
//				memset(npcDialog, 0, 1024) ;
//
//				wIdx = 0 ;
//
//				while(TRUE)																// while���� ������.
//				{
//					temp = pDialogueList->GetDialogue( dwDialogue, wIdx );				// �����ϰ� ���� �������� ���̾�α� ������ �޴´�.
//
//					if( temp == NULL ) break;											// ������ ��ȿ���� ������, while���� ����������.
//
//					strcat(npcDialog, temp->str) ;										// �о���� ��縦 �ӽ� ���ۿ� �߰��Ѵ�.
//
//					++wIdx ;															// �ε����� �����Ѵ�.
//				}
//
//				m_pQuestDescList->AddItemAutoLine(npcDialog, RGBA_MAKE(10, 10, 10, 255)) ;
//
//				m_pQuestDescList->AddItem(" ", 0);										// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.
//
//				HYPERLINK* pHyperLink = pPage->GetHyperText(0) ;
//
//				if( pHyperLink )
//				{
//					dwScriptPage = pHyperLink->dwData ;
//
//					pPage = NPCSCRIPTMGR->GetPage( dwNpcIdx, dwScriptPage );			// ���� NPC�� ���� ������ ���̵�� ������ ������ �޴´�.
//				}
//				else
//				{
//					break ;
//				}
//			}
//			else
//			{
//				break ;
//			}
//		}
//	}
//	else
//	{
//		int nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// ���� ����Ʈ ���� �޴´�.
//
//		cDialogueList* pDialogueList = NULL ;											// ���̾�α� ����Ʈ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//		pDialogueList = NPCSCRIPTMGR->GetDialogueList();								// NPC ��ũ��Ʈ �Ŵ������� ���̾�α� ����Ʈ�� �޴´�.
//		if( pDialogueList == NULL ) return ;											// ���̾�α� ������ ��ȿ���� ������, FALSE ���� ó���� �Ѵ�.
//
//		CSubQuestInfo* pSubQuestInfo = NULL ;											// ���� ����Ʈ ������ ���� �����͸� �����ϰ� NULLó���� �Ѵ�.
//		pSubQuestInfo = pQuestInfo->GetSubQuestInfo(0) ;								// ī��Ʈ�� �ش��ϴ� ���� ����Ʈ ������ �޴´�.
//
//		if( !pSubQuestInfo ) return ;
//
//		DWORD dwPage = pSubQuestInfo->GetNpcScriptPage(dwNpcIdx) ;						// NPC �ε����� ������ �ε����� �޴´�.
//		cPage* pPage = NPCSCRIPTMGR->GetPage( dwNpcIdx, dwPage );						// ���� NPC�� ���� ������ ���̵�� ������ ������ �޴´�.
//
//		while(TRUE)
//		{
//			if( pPage )
//			{
//				DWORD dwDialogue = pPage->GetDialogue() ;										// �������� ���̾�α� ��ȣ�� �޴´�.
//
//				memset(npcDialog, 0, 1024) ;
//
//				wIdx = 0 ;
//
//				while(TRUE)																// while���� ������.
//				{
//					temp = pDialogueList->GetDialogue( dwDialogue, wIdx );				// �����ϰ� ���� �������� ���̾�α� ������ �޴´�.
//
//					if( temp == NULL ) break;											// ������ ��ȿ���� ������, while���� ����������.
//
//					strcat(npcDialog, temp->str) ;										// �о���� ��縦 �ӽ� ���ۿ� �߰��Ѵ�.
//
//					++wIdx ;															// �ε����� �����Ѵ�.
//				}
//
//				m_pQuestDescList->AddItemAutoLine(npcDialog, RGBA_MAKE(10, 10, 10, 255)) ;
//
//				m_pQuestDescList->AddItem(" ", 0);										// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.
//
//				HYPERLINK* pHyperLink = pPage->GetHyperText(0) ;
//
//				if( pHyperLink )
//				{
//					dwPage = pHyperLink->dwData ;
//
//					pPage = NPCSCRIPTMGR->GetPage( dwNpcIdx, dwPage );					// ���� NPC�� ���� ������ ���̵�� ������ ������ �޴´�.
//				}
//				else
//				{
//					break ;
//				}
//			}
//			else
//			{
//				break ;
//			}
//		}
//	}
//
//	// for( int count = 0 ; count < nSubQuestCount ; ++count )							// ���� ����Ʈ �� ��ŭ for���� ������.
//	//{
//	//	pDescSubQuestInfo = NULL ;													// ���� ����Ʈ ������ ���� �����͸� nulló�� �Ѵ�.
//	//	pDescSubQuestInfo = pQuest->GetSubQuestInfoArray(count) ;					// ī��Ʈ�� �ش��ϴ� ���� ����Ʈ�� �޴´�.
//
//	//	if( !pDescSubQuestInfo ) continue ;											// ���� ����Ʈ ������ ��ȿ���� ������ continue�Ѵ�.
//
//	//	cPtrList* pList = pDescSubQuestInfo->GetQuestNpcScriptList() ;
//
//	//	if( !pList ) continue ;
//
//	//	CQuestNpcScript* pScript ;
//	//	PTRLISTPOS scriptPos = pList->GetHeadPosition() ;
//
//	//	while(scriptPos)
//	//	{
//	//		pScript = NULL ;
//	//		pScript = (CQuestNpcScript*)pList->GetNext(scriptPos) ;
//
//	//		if(!pScript) continue ;
//
//	//		cPtrList* pNpcScriptList = pScript->GetNpcScriptList() ;
//	//		if( !pNpcScriptList ) continue ;
//
//	//		CQuestNpcScriptData* pScriptData ;
//	//		PTRLISTPOS dataPos = pNpcScriptList->GetHeadPosition() ;
//
//	//		while(dataPos)
//	//		{
//	//			pScriptData = NULL ;
//	//			pScriptData = (CQuestNpcScriptData*)pNpcScriptList->GetNext(dataPos) ;
//
//	//			if(!pScriptData) continue ;
//
//	//			DWORD dwNpcIdx = pScriptData->GetNpcIdx() ;
//	//			DWORD dwScriptPage = pScriptData->GetNpcScriptPage() ;
//
//	//			cPage* pPage = NPCSCRIPTMGR->GetPage( dwNpcIdx, dwScriptPage );					// ���� NPC�� ���� ������ ���̵�� ������ ������ �޴´�.
//
//	//			WORD wIdx = 0;																	// �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
//	//			WORD LineInfo = 0;																// ���� ������ ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//	//			DIALOGUE* temp = NULL;															// ���̾�α� ������ ���� ������ ������ �����ϰ� NULLó���� �Ѵ�.
//
//	//			LINKITEM* pItem = NULL;															// ��ũ ������ ������ ���� ������ ������ �����ϰ� NULL ó���� �Ѵ�.
//	//			LINKITEM* Prev = NULL;															// ���� ��ũ ������ ������ ���� ������ ������ �����ϰ� NULL ó���� �Ѵ�.
//
//	//			char npcDialog[1024] = {0, } ;													// npc ��縦 ���� �ӽ� ���۸� �����Ѵ�.
//
//	//			while(TRUE)
//	//			{
//	//				if( pPage )
//	//				{
//	//					DWORD dwDialogue = pPage->GetDialogue() ;										// �������� ���̾�α� ��ȣ�� �޴´�.
//
//	//					memset(npcDialog, 0, 1024) ;
//
//	//					wIdx = 0 ;
//
//	//					while(TRUE)																// while���� ������.
//	//					{
//	//						temp = pDialogueList->GetDialogue( dwDialogue, wIdx );				// �����ϰ� ���� �������� ���̾�α� ������ �޴´�.
//
//	//						if( temp == NULL ) break;											// ������ ��ȿ���� ������, while���� ����������.
//
//	//						strcat(npcDialog, temp->str) ;										// �о���� ��縦 �ӽ� ���ۿ� �߰��Ѵ�.
//
//	//						++wIdx ;															// �ε����� �����Ѵ�.
//	//					}
//
//	//					m_pQuestDescList->AddItemAutoLine(npcDialog, RGBA_MAKE(10, 10, 10, 255)) ;
//
//	//					m_pQuestDescList->AddItem(" ", 0);										// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.
//
//	//					HYPERLINK* pHyperLink = pPage->GetHyperText(0) ;
//
//	//					if( pHyperLink )
//	//					{
//	//						dwPage = pHyperLink->dwData ;
//
//	//						pPage = NPCSCRIPTMGR->GetPage( dwNpcIdx, dwPage );					// ���� NPC�� ���� ������ ���̵�� ������ ������ �޴´�.
//	//					}
//	//					else
//	//					{
//	//						break ;
//	//					}
//	//				}
//	//				else
//	//				{
//	//					break ;
//	//				}
//	//			}
//	//		}
//	//	}
//	//}
//
//
//	////int nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// ���� ����Ʈ ���� �޴´�.
//
//	////cDialogueList* pDialogueList = NULL ;											// ���̾�α� ����Ʈ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	////pDialogueList = NPCSCRIPTMGR->GetDialogueList();								// NPC ��ũ��Ʈ �Ŵ������� ���̾�α� ����Ʈ�� �޴´�.
//	////if( pDialogueList == NULL ) return ;											// ���̾�α� ������ ��ȿ���� ������, FALSE ���� ó���� �Ѵ�.
//
//	////CSubQuestInfo* pSubQuestInfo = NULL ;											// ���� ����Ʈ ������ ���� �����͸� �����ϰ� NULLó���� �Ѵ�.
//	////pSubQuestInfo = pQuestInfo->GetSubQuestInfo(0) ;								// ī��Ʈ�� �ش��ϴ� ���� ����Ʈ ������ �޴´�.
//
//	////if( !pSubQuestInfo ) return ;
//
//	////DWORD dwPage = pSubQuestInfo->GetNpcScriptPage(dwNpcIdx) ;						// NPC �ε����� ������ �ε����� �޴´�.
//	////cPage* pPage = NPCSCRIPTMGR->GetPage( dwNpcIdx, dwPage );						// ���� NPC�� ���� ������ ���̵�� ������ ������ �޴´�.
//
//	////WORD wIdx = 0;																	// �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
//	////WORD LineInfo = 0;																// ���� ������ ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//	////DIALOGUE* temp = NULL;															// ���̾�α� ������ ���� ������ ������ �����ϰ� NULLó���� �Ѵ�.
//
//	////LINKITEM* pItem = NULL;															// ��ũ ������ ������ ���� ������ ������ �����ϰ� NULL ó���� �Ѵ�.
//	////LINKITEM* Prev = NULL;															// ���� ��ũ ������ ������ ���� ������ ������ �����ϰ� NULL ó���� �Ѵ�.
//
//	////char npcDialog[1024] = {0, } ;													// npc ��縦 ���� �ӽ� ���۸� �����Ѵ�.
//
//	////while(TRUE)
//	////{
//	////	if( pPage )
//	////	{
//	////		DWORD dwDialogue = pPage->GetDialogue() ;										// �������� ���̾�α� ��ȣ�� �޴´�.
//
//	////		memset(npcDialog, 0, 1024) ;
//
//	////		wIdx = 0 ;
//
//	////		while(TRUE)																// while���� ������.
//	////		{
//	////			temp = pDialogueList->GetDialogue( dwDialogue, wIdx );				// �����ϰ� ���� �������� ���̾�α� ������ �޴´�.
//
//	////			if( temp == NULL ) break;											// ������ ��ȿ���� ������, while���� ����������.
//
//	////			strcat(npcDialog, temp->str) ;										// �о���� ��縦 �ӽ� ���ۿ� �߰��Ѵ�.
//
//	////			++wIdx ;															// �ε����� �����Ѵ�.
//	////		}
//
//	////		m_pQuestDescList->AddItemAutoLine(npcDialog, RGBA_MAKE(10, 10, 10, 255)) ;
//
//	////		m_pQuestDescList->AddItem(" ", 0);										// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.
//
//	////		HYPERLINK* pHyperLink = pPage->GetHyperText(0) ;
//
//	////		if( pHyperLink )
//	////		{
//	////			dwPage = pHyperLink->dwData ;
//
//	////			pPage = NPCSCRIPTMGR->GetPage( dwNpcIdx, dwPage );					// ���� NPC�� ���� ������ ���̵�� ������ ������ �޴´�.
//	////		}
//	////		else
//	////		{
//	////			break ;
//	////		}
//	////	}
//	////	else
//	////	{
//	////		break ;
//	////	}
//	////}
//
//	//////////////////////////////////////////
//	//// - ���� ����Ʈ ���� ���� -
//	//////////////////////////////////////////
//
//    for( int count = 0 ; count < nSubQuestCount ; ++count )							// ���� ����Ʈ �� ��ŭ for���� ������.
//	{
//		pDescSubQuestInfo = NULL ;													// ���� ����Ʈ ������ ���� �����͸� nulló�� �Ѵ�.
//		pDescSubQuestInfo = pQuest->GetSubQuestInfoArray(count) ;					// ī��Ʈ�� �ش��ϴ� ���� ����Ʈ�� �޴´�.
//
//		if( !pDescSubQuestInfo ) continue ;											// ���� ����Ʈ ������ ��ȿ���� ������ continue�Ѵ�.
//		
//		COMBINEKEY(dwMainQuestID, count, QuestStringKey) ;							// ����Ʈ, ���� ����Ʈ �ε����� ����Ʈ ��Ʈ���� ���Ѵ�.
//
//		SUBQUEST* pSubQuest = pQuest->GetSubQuest(count) ;							// ���� ����Ʈ ������ �޴´�.
//
//		pQString = QUESTMGR->GetQuestString(QuestStringKey);						// ���� ����Ʈ ��Ʈ���� �޴´�.
//
//		if( !pQString )	continue ;													// ����Ʈ ��Ʈ�� ������ ��ȿ���� ������ continue�Ѵ�.
//
//		// - ���� ����Ʈ Ÿ��Ʋ ���� -
//		pTitleList = pQString->GetTitle() ;											// ���� ����Ʈ�� Ÿ��Ʋ ����Ʈ�� �޴´�.
//
//		if( !pTitleList ) continue ;												// Ÿ��Ʋ ����Ʈ�� ��ȿ���� ������ continue�Ѵ�.
//
//		m_pQuestDescList->AddItem(" ", 0);											// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.
//
//		int nTitleCount = 0 ;														// ���� ����Ʈ�� Ÿ��Ʋ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//		nTitleCount = pTitleList->GetCount() ;										// ���� ����Ʈ�� Ÿ��Ʋ�� �����Ѵ�.
//
//		for( int count2 = 0 ; count2 < nTitleCount ; ++count2 )						// ���� Ÿ��Ʋ ī��Ʈ ��ŭ for���� ������.
//		{
//			pos = NULL ;															// ��ġ ������ ���� �����͸� nulló���� �Ѵ�.
//			pos = pTitleList->FindIndex(count2) ;									// Ÿ��Ʋ�� ��ġ�� �޴´�.
//
//			if( !pos ) continue ;													// ��ġ ������ ��ȿ���� ������ continue�Ѵ�.
//
//			ITEM* pTitleLine = (ITEM*)pTitleList->GetAt(pos) ;						// Ÿ��Ʋ ��Ʈ���� �޴´�.
//
//			if( !pTitleLine ) continue ;											// Ÿ��Ʋ ��Ʈ���� ��ȿ���� ������, continue�Ѵ�.
//
//			if( !pSubQuest ) continue ;												// ���� ����Ʈ ������ ��ȿ���� ������ continue�Ѵ�.
//
//			DWORD dwCurCount = pSubQuest->dwData ;									// ���� ������ �޴´�.
//			DWORD dwMaxCount = pSubQuest->dwMaxCount ;								// �ִ� ������ �޴´�.
//
//			memset(tempBuf, 0, sizeof(tempBuf)) ;									// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
//
//			if( pQString->IsComplete() )											// ����Ʈ�� �Ϸ� �Ǿ�����,
//			{
//				sprintf(tempBuf, "%s (�Ϸ�)", pTitleLine->string) ;					// ���� �ϷḦ �߰��Ѵ�.
//
//				m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(0, 64, 128, 255)) ;	// ����� �ϷḦ ����Ѵ�.
//			}
//			else																	// �Ϸ��� ����Ʈ�� �ƴϸ�,
//			{	
//				if( pSubQuest->dwMaxCount == 0 )									// ���� ����Ʈ�� ��Ż ī��Ʈ�� 0�� ������,
//				{
//					if( pSubQuest->dwData == 0 )									// ���� ����Ʈ�� ���� ī��Ʈ�� 0�� ������,
//					{
//						sprintf(tempBuf, "%s", pTitleLine->string) ;				// ����Ʈ ���� �ӽù��ۿ� �����Ѵ�.
//					}
//					else															// ���� ����Ʈ�� ���� ī��Ʈ�� 0�� �ƴϸ�,
//					{
//						sprintf(tempBuf, "%s (%d)", pTitleLine->string, pSubQuest->dwData) ; // ���� ����Ʈ�� �����, ������ �ӽ� ���ۿ� �����Ѵ�.
//					}
//				}
//				else																// ���� ����Ʈ�� ��Ż ī��Ʈ�� 0�� �ƴϸ�,
//				{
//					sprintf(tempBuf, "%s (%d/%d)", pTitleLine->string, pSubQuest->dwData, pSubQuest->dwMaxCount) ; // ���� ����Ʈ�� ����� ���� ī��Ʈ�� �ӽ� ���ۿ� �����Ѵ�.
//				}										
//
//				m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(0, 0, 255, 255)) ;		// ���õ� ��Ʈ���� ����Ʈ ���� ����Ʈ ���̾�α׿� �߰��Ѵ�.
//			}
//		}
//
//        // - ���� ����Ʈ ���� ����
//		pDescList = pQString->GetDesc() ;										// ���� ����Ʈ ���� ����Ʈ�� �޴´�.
//
//		if( !pDescList ) continue ;												// ���� ����Ʈ ������ ��ȿ���� ������, continue�Ѵ�.
//
//		int nDescCount = 0 ;													// ���� ������ ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//		nDescCount = pDescList->GetCount() ;									// ���� ����Ʈ�� ������ ���� �޴´�.
//
//		for( int count2 = 0 ; count2 < nDescCount ; ++count2 )					// ���� ���� ��ŭ for���� ������.
//		{
//			pos = NULL ;														// ��ġ ������ ���� �����͸� nulló���� �Ѵ�.
//			pos = pDescList->FindIndex(count2) ;								// ī��Ʈ�� �ش��ϴ� ��ġ ������ �޴´�.
//
//			if( !pos ) continue ;												// ��ġ ������ ��ȿ���� ������ continue�Ѵ�.
//
//			ITEM* pDescLine = (ITEM*)pDescList->GetAt(pos) ;  					// ���� ��Ʈ���� �޴´�.
//
//			if( pDescLine )														// ���� ��Ʈ�� ������ ��ȿ�ϴٸ�,						
//			{
//				m_pQuestDescList->AddItem(pDescLine->string, pDescLine->rgb) ;	// ����Ʈ ���� ����Ʈ�� �߰��� �Ѵ�.
//			}
//		}
//
//		pTitleList = NULL ;														// ���� ����Ʈ �����͸� nulló���� �Ѵ�.
//		pDescList  = NULL ;														// ���� ����Ʈ �����͸� nulló���� �Ѵ�.
//
//		pQString = NULL ;														// ��Ʈ�� �����͸� nulló���� �Ѵ�.			
//	}
//
//	//////////////////////////////////////
//	// - ���� ���� ���� -
//	//////////////////////////////////////
//
//	cIcon* pDeleteIcon = NULL ;
//	RequitalItem* pDeleteItem = NULL ;
//
//	PTRLISTPOS deletePos = NULL ;
//	deletePos = m_QuestRequitalList.GetHeadPosition() ;
//
//	while(deletePos)
//	{
//		pDeleteItem = NULL ;
//		pDeleteItem = (RequitalItem*)m_QuestRequitalList.GetNext(deletePos) ;
//
//		if( pDeleteItem )
//		{
//			m_pRequitalGrid->DeleteIcon( pDeleteItem->wPos, &pDeleteItem->pIcon ) ;
//
//			pDeleteItem->pIcon->SetActive(FALSE) ;
//			WINDOWMGR->AddListDestroyWindow(pDeleteItem->pIcon) ;
//
//			m_IndexGenerator.ReleaseIndex( pDeleteItem->pIcon->GetID() ) ;
//
//			m_QuestRequitalList.Remove(pDeleteItem) ;
//
//			delete pDeleteItem ;
//		}
//	}
//
//	m_QuestRequitalList.RemoveAll() ;
//
//	RefreshRequitalItem() ;
//
//
//	CSubQuestInfo* pSubQuestInfo = pQuestInfo->GetSubQuestInfo(nSubQuestCount-1) ;					// ������ ���� ����Ʈ ������ �޴´�.
//
//	if( pSubQuestInfo )																// ���� ����Ʈ ������ ��ȿ�ϴٸ�,
//	{
//		cPtrList* pTriggerList = pSubQuestInfo->GetTriggerList() ;					// ������ ���� ����Ʈ�� Ʈ���� ������ �޴´�.
//
//		if( pTriggerList )															// Ʈ���� ������ ��ȿ�ϴٸ�,
//		{
//			int nTriggerCount = 0 ;													// Ʈ���� ī��Ʈ�� ���� ������ �����ϰ�, 0���� �����Ѵ�.
//
//			nTriggerCount = pTriggerList->GetCount() ;								// Ʈ���� ī��Ʈ�� �޴´�.
//
//			CQuestTrigger* pTrigger = NULL ;										// Ʈ���� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//			PTRLISTPOS exePos = NULL ;												// ���� ����Ʈ ��ġ ������ ���� ��ġ �����͸� �����ϰ� null ó���� �Ѵ�.
//
//			for( int count = 0 ; count < nTriggerCount ; ++count )					// Ʈ���� ī��Ʈ �� ��ŭ for���� ������.
//			{
//				pos = NULL ;														// ��ġ ������ ���� �����͸� null ó���� �Ѵ�.
//				pos = pTriggerList->FindIndex(count) ;								// ī��Ʈ�� �ش��ϴ� ��ġ ������ �޴´�.
//
//				if( pos )															// ��ġ ������ ��ȿ�ϴٸ�,
//				{
//					pTrigger = (CQuestTrigger*)pTriggerList->GetAt(pos) ;			// ��ġ�� �ش��ϴ� Ʈ���� ������ �޴´�.
//
//					if( pTrigger )													// Ʈ���� ������ ��ȿ�ϴٸ�,
//					{
//						cPtrList* pExeList = pTrigger->GetExeList() ;				// Ʈ���� ���� ����Ʈ�� �޴´�.
//
//						if( pExeList )												// Ʈ���� ���� ����Ʈ ������ ��ȿ�ϴٸ�,
//						{
//							int nExeCount = pExeList->GetCount() ;					// ���� ����Ʈ ī��Ʈ�� �޴´�.
//
//							CQuestExecute* pExecute = NULL ;						// ����Ʈ ���� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//							for(int count2 = 0 ; count2 < nExeCount ; ++count2 )	// ���� ī��Ʈ ��ŭ for���� ������.
//							{
//								exePos = NULL ;										// ���� ��ġ �����͸� null ó���� �Ѵ�.
//								exePos = pExeList->FindIndex(count2) ;				// ī��Ʈ�� �ش��ϴ� ���� ��ġ ������ �޴´�.									
//
//								if( exePos )										// ����Ʈ ���� ��ġ�� ��ȿ�ϸ�,
//								{
//									pExecute = (CQuestExecute*)pExeList->GetAt(exePos) ;	// ����Ʈ ���� ������ �޴´�.
//	
//									if( pExecute )									// ����Ʈ ���� ������ ��ȿ�ϸ�,
//									{
//										CQuestExecute_Item* pExecute_Item = ((CQuestExecute_Item*)pExecute) ;
//										DWORD dwExeKind = pExecute->GetQuestExecuteKind() ;	// ���� Ÿ���� �޴´�.
//
//										switch(dwExeKind)							// ���� Ÿ���� Ȯ���Ѵ�.
//										{
//										case eQuestExecute_TakeItem :				// ���� Ÿ���� �������� �޴� Ÿ���̶��,
//											{
//												ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(pExecute_Item->GetItemKind());
//
//												if( pItemInfo )
//												{
//													AddRequitalItem(pItemInfo, pExecute_Item->GetItemNum()) ;
//												}
//											}
//											break ;
//
//										case eQuestExecute_TakeMoney :				// ���� Ÿ���� ��带 �޴� Ÿ���̶��,
//											{
//												SetGold( pExecute_Item->GetItemNum() ) ;
//											}
//											break ;
//
//										case eQuestExecute_TakeExp :				// ���� Ÿ���� ����ġ�� �޴� Ÿ���̶��,
//											{
//												SetExp( pExecute_Item->GetItemNum() ) ;
//											}
//											break ;
//
//										}
//									}
//								}
//							}
//						}
//					}
//				}
//
//				pTrigger = NULL ;
//			}
//		}
//	}
//}

//void CQuestDialog::QuestDescView(CQuestString* pQuestString)						// ����Ʈ �� ���� ���� �Լ�.
//{
//	if( !m_bActive )			return ;											// ����Ʈ â�� ���� ���°� �ƴ϶��, ���� ó���� �Ѵ�.
//
//	if(!pQuestString)	
//	{
//		m_pQuestDescList->RemoveAll() ;
//
//		int nCellCount = m_pRequitalGrid->GetCellNum() ;
//
//		cIcon* pIcon ;
//
//		for( int count = 0 ; count < nCellCount ; ++count )
//		{
//			pIcon = NULL ;
//			pIcon = m_pRequitalGrid->GetIconForIdx(count) ;
//
//			if( pIcon )
//			{
//				m_pRequitalGrid->DeleteIcon(pIcon) ;
//			}
//		}
//
//		PTRLISTPOS pos = m_QuestRequitalList.GetHeadPosition() ;
//
//		RequitalItem* item ;
//
//		while( pos )
//		{
//			item = NULL ;
//			item = ( RequitalItem* )m_QuestRequitalList.GetNext( pos ) ;
//
//			if( item )
//			{
//				WINDOWMGR->AddListDestroyWindow( item->pIcon );
//				SAFE_DELETE( item ) ;
//			}
//		}
//
//		m_QuestRequitalList.RemoveAll() ;
//
//		SetGold(0) ;
//		SetExp(0) ;
//
//		return;												// ����Ʈ ��Ʈ�� ������ ��ȿ���� ������, ���� ó���� �Ѵ�.
//	}
//
//	char tempBuf[128] = {0, } ;														// �ӽù��۸� �����ϰ� �ʱ�ȭ �Ѵ�.
//
//	PTRLISTPOS pos = NULL;															// ��ġ ������ ���� ������ ������ �����ϰ� nulló�� �Ѵ�.
//
//	DWORD dwMainQuestID = pQuestString->GetQuestID() ;								// ��Ʈ������ ����Ʈ ��ȣ�� �޴´�.
//
//	CQuestInfo* pQuestInfo = QUESTMGR->GetQuestInfo(dwMainQuestID) ;				// ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.
//
//	if( !pQuestInfo )																// ����Ʈ ������ ��ȿ���� �ʴٸ�,
//	{
//		return ;																	// ���� ó���� �Ѵ�.
//	}
//
//	DWORD dwNpcIdx = pQuestInfo->GetNpcIdx() ;										// ����Ʈ �������� npc �ε����� �޴´�.
//
//	stNPCIDNAME* pNpcIdName = QUESTMGR->GetNpcIdName(dwNpcIdx) ;					// ����Ʈ �Ŵ������� npc ���̵�, �̸��� �޴´�.
//
//	int nMapNum = pQuestInfo->GetMapNum() ;											// ����Ʈ �������� �� ��ȣ�� �޴´�.
//	
//	m_pQuestDescList->RemoveAll();													// ����Ʈ ����Ʈ ���̾�α׸� ��� ����.
//
//	////////////////////////////////////////
//	// - ����Ʈ �̸�, �Ƿ���, ��ġ ���� -
//	////////////////////////////////////////
//
//	m_toPreList= -1;																// ���� ����Ʈ�� -1�� �����Ѵ�.
//
//	ITEM* pQStr = NULL;															// ����Ʈ ��Ʈ�� ������ ���� ������ �����ϰ� nulló�� �Ѵ�.
//
//	CQuestString* pTitleString = QUESTMGR->GetQuestString(pQuestString->GetQuestID(), 0) ;
//
//	pos = pTitleString->GetTitle()->GetHeadPosition();								// ����Ʈ ��Ʈ���� �ص� ��ġ�� �����Ѵ�.
//
//	// - ����Ʈ ���� ���� - 
//
//	while(pos)																		// ��ġ ������ ��ȿ�� ���� while���� ������.
//	{
//		pQStr = (ITEM*)pTitleString->GetTitle()->GetNext(pos);					// ��ġ ������ �ش��ϴ� ����Ʈ ��Ʈ�� ������ �޴´�.
//
//		if(pQStr)																	// ����Ʈ ��Ʈ�� ������ ��ȿ�ϴٸ�,
//		{
//			sprintf(tempBuf, "< %s >", pQStr->string) ;								// �ӽ� ���ۿ� ����Ʈ ������ �����Ѵ�.
//			m_pQuestDescList->AddItem(tempBuf, MAINQUEST_TITLE_COLOR);				// ����Ʈ ����Ʈ ���̾�α׿� �߰��Ѵ�.
//			++m_toPreList;															// ���� ����Ʈ ���� �����Ѵ�.
//		}
//	}
//
//	m_pQuestDescList->AddItem(" ", 0);												// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.
//	++m_toPreList;																	// ���� ����Ʈ ���� �����Ѵ�.
//
//	// - �Ƿ��� ���� - 
//	if( pNpcIdName )																// npc ������ ��ȿ�ϸ�,
//	{
//		memset(tempBuf, 0, sizeof(tempBuf)) ;										// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
//		sprintf(tempBuf, "�Ƿ��� - %s", pNpcIdName->name) ;							// �Ƿ��� �̸����� npc �̸��� �����Ѵ�.
//		m_pQuestDescList->AddItem(tempBuf, SUBQUEST_TITLE_COLOR_SEL);				// ����Ʈ ���� ����Ʈ�� �߰��Ѵ�.
//	}
//	else																			// npc ������ ��ȿ���� ������,
//	{
//		ASSERTMSG(0, "����Ʈ ����â�� NPC ������ �ε����� ���߽��ϴ�.") ;			// npc ������ �и��� �־�� �Ѵ�.
//		m_pQuestDescList->AddItem("<*********>", SUBQUEST_TITLE_COLOR_SEL);			// �˾ƺ� �� �ְ� <*********> �� ǥ���Ѵ�.
//	}
//
//	// - ��ġ�� �����Ѵ�. -
//	memset(tempBuf, 0, sizeof(tempBuf)) ;											// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
//	char* pMapName = GetMapName(nMapNum) ;											// �� ��ȣ�� ���� �� �̸��� �޴´�.	
//	sprintf(tempBuf, "��ġ - %s", pMapName) ;										// ��ġ�� �� �̸��� �߰��Ѵ�.
//	m_pQuestDescList->AddItem(tempBuf, SUBQUEST_TITLE_COLOR_SEL);					// ����Ʈ ���� �߰��Ѵ�.
//
//
//	////////////////////////////////////////
//	// - �Ƿ� ���� ���� -
//	////////////////////////////////////////
//
//	m_pQuestDescList->AddItem(" ", 0);												// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.
//	m_pQuestDescList->AddItem("< �Ƿ� ���� > ", MAINQUEST_TITLE_COLOR);				// <�Ƿڳ���>�� �߰��Ѵ�.
//	m_pQuestDescList->AddItem(" ", 0);												// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.
//
//	int nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// ���� ����Ʈ ���� �޴´�.
//
//	cDialogueList* pDialogueList = NULL ;											// ���̾�α� ����Ʈ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	pDialogueList = NPCSCRIPTMGR->GetDialogueList();								// NPC ��ũ��Ʈ �Ŵ������� ���̾�α� ����Ʈ�� �޴´�.
//	if( pDialogueList == NULL ) return ;											// ���̾�α� ������ ��ȿ���� ������, FALSE ���� ó���� �Ѵ�.
//
//	CSubQuestInfo* pSubQuestInfo = NULL ;											// ���� ����Ʈ ������ ���� �����͸� �����ϰ� NULLó���� �Ѵ�.
//	pSubQuestInfo = pQuestInfo->GetSubQuestInfo(0) ;								// ī��Ʈ�� �ش��ϴ� ���� ����Ʈ ������ �޴´�.
//
//	if( !pSubQuestInfo ) return ;
//
//	DWORD dwPage = pSubQuestInfo->GetNpcScriptPage(dwNpcIdx) ;						// NPC �ε����� ������ �ε����� �޴´�.
//	cPage* pPage = NPCSCRIPTMGR->GetPage( dwNpcIdx, dwPage );						// ���� NPC�� ���� ������ ���̵�� ������ ������ �޴´�.
//
//	WORD wIdx = 0;																	// �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
//	WORD LineInfo = 0;																// ���� ������ ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//	DIALOGUE* temp = NULL;															// ���̾�α� ������ ���� ������ ������ �����ϰ� NULLó���� �Ѵ�.
//
//	LINKITEM* pItem = NULL;															// ��ũ ������ ������ ���� ������ ������ �����ϰ� NULL ó���� �Ѵ�.
//	LINKITEM* Prev = NULL;															// ���� ��ũ ������ ������ ���� ������ ������ �����ϰ� NULL ó���� �Ѵ�.
//
//	char npcDialog[1024] = {0, } ;													// npc ��縦 ���� �ӽ� ���۸� �����Ѵ�.
//
//	while(TRUE)
//	{
//		if( pPage )
//		{
//			DWORD dwDialogue = pPage->GetDialogue() ;										// �������� ���̾�α� ��ȣ�� �޴´�.
//
//			memset(npcDialog, 0, 1024) ;
//
//			wIdx = 0 ;
//
//			while(TRUE)																// while���� ������.
//			{
//				temp = pDialogueList->GetDialogue( dwDialogue, wIdx );				// �����ϰ� ���� �������� ���̾�α� ������ �޴´�.
//
//				if( temp == NULL ) break;											// ������ ��ȿ���� ������, while���� ����������.
//
//				strcat(npcDialog, temp->str) ;										// �о���� ��縦 �ӽ� ���ۿ� �߰��Ѵ�.
//
//				++wIdx ;															// �ε����� �����Ѵ�.
//			}
//
//			m_pQuestDescList->AddItemAutoLine(npcDialog, RGBA_MAKE(10, 10, 10, 255)) ;
//
//			m_pQuestDescList->AddItem(" ", 0);										// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.
//
//			HYPERLINK* pHyperLink = pPage->GetHyperText(0) ;
//
//			if( pHyperLink )
//			{
//				dwPage = pHyperLink->dwData ;
//
//				pPage = NPCSCRIPTMGR->GetPage( dwNpcIdx, dwPage );					// ���� NPC�� ���� ������ ���̵�� ������ ������ �޴´�.
//			}
//			else
//			{
//				break ;
//			}
//		}
//		else
//		{
//			break ;
//		}
//	}
//
//	//if( pSubQuestInfo )																// ���� ����Ʈ ������ ��ȿ�ϴٸ�,
//	//{
//	//	DWORD dwPage = pSubQuestInfo->GetNpcScriptPage(dwNpcIdx) ;					// NPC �ε����� ������ �ε����� �޴´�.
//	//	cPage* pPage = NPCSCRIPTMGR->GetPage( dwNpcIdx, dwPage );					// ���� NPC�� ���� ������ ���̵�� ������ ������ �޴´�.
//
//	//	if( pPage )																	// ������ ������ ��ȿ�ϴٸ�,
//	//	{
//	//		WORD wIdx = 0;															// �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
//	//		WORD LineInfo = 0;														// ���� ������ ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//	//		DIALOGUE* temp = NULL;													// ���̾�α� ������ ���� ������ ������ �����ϰ� NULLó���� �Ѵ�.
//
//	//		LINKITEM* pItem = NULL;													// ��ũ ������ ������ ���� ������ ������ �����ϰ� NULL ó���� �Ѵ�.
//	//		LINKITEM* Prev = NULL;													// ���� ��ũ ������ ������ ���� ������ ������ �����ϰ� NULL ó���� �Ѵ�.
//
//	//		DWORD dwDialogue = pPage->GetDialogue() ;								// �������� ���̾�α� ��ȣ�� �޴´�.
//
//	//		// *������ ���̾�α׸� �д� �����, ������ npc ��ȭâ���� �µ��� �Ǿ��־���.
//	//		// ������ ��縦 ��� �о�鿩 �ӽ� ���ۿ� ���, ����ϰ��� �ϴ� ����Ʈ�� 
//	//		// �ؽ�Ʈ ���̿� �°� �ٽ� �ؽ�Ʈ Ŀ�� �۾��� �Ѵ�.
//	//		char npcDialog[1024] = {0, } ;											// npc ��縦 ���� �ӽ� ���۸� �����Ѵ�.
//
//	//		while(TRUE)																// while���� ������.
//	//		{
//	//			temp = pDialogueList->GetDialogue( dwDialogue, wIdx );				// �����ϰ� ���� �������� ���̾�α� ������ �޴´�.
//
//	//			if( temp == NULL ) break;											// ������ ��ȿ���� ������, while���� ����������.
//
//	//			strcat(npcDialog, temp->str) ;										// �о���� ��縦 �ӽ� ���ۿ� �߰��Ѵ�.
//
//	//			++wIdx ;															// �ε����� �����Ѵ�.
//	//		}
//
//	//		m_pQuestDescList->AddItemAutoLine(npcDialog, RGBA_MAKE(10, 10, 10, 255)) ;
//	//	}
//	//}
//
//
//	////////////////////////////////////////
//	// - ���� ����Ʈ ���� ���� -
//	////////////////////////////////////////
//
//	int QuestStringKey = 0 ;														// ����Ʈ ��Ʈ�� Ű�� ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//	CQuestString* pQString = NULL ;													// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	cPtrList* pTitleList = NULL ;													// ���� ����Ʈ ���� ����Ʈ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	cPtrList* pDescList = NULL ;													// ���� ����Ʈ ���� ����Ʈ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	CQuest* pQuest = QUESTMGR->GetQuest( dwMainQuestID );							// ����Ʈ ������ �޴´�.
//
//    for( int count = 1 ; count < nSubQuestCount ; ++count )							// ���� ����Ʈ ��-1��ŭ for���� ������.
//	{
//		COMBINEKEY(dwMainQuestID, count, QuestStringKey) ;							// ����Ʈ, ���� ����Ʈ �ε����� ����Ʈ ��Ʈ���� ���Ѵ�.
//
//		SUBQUEST* pSubQuest = pQuest->GetSubQuest(count) ;							// ���� ����Ʈ ������ �޴´�.
//
//		pQString = QUESTMGR->GetQuestString(QuestStringKey);						// ���� ����Ʈ ��Ʈ���� �޴´�.
//
//		if( pQString )																// ���� ����Ʈ ��Ʈ���� ��ȿ�ϸ�,
//		{
//			// - ���� ����Ʈ Ÿ��Ʋ ���� -
//			pTitleList = pQString->GetTitle() ;										// ���� ����Ʈ�� Ÿ��Ʋ ����Ʈ�� �޴´�.
//
//			if( pTitleList )														// Ÿ��Ʋ ����Ʈ�� ��ȿ�ϸ�,
//			{
//				m_pQuestDescList->AddItem(" ", 0);									// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.
//
//				int nTitleCount = 0 ;												// ���� ����Ʈ�� Ÿ��Ʋ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//				nTitleCount = pTitleList->GetCount() ;								// ���� ����Ʈ�� Ÿ��Ʋ�� �����Ѵ�.
//
//				for( int count2 = 0 ; count2 < nTitleCount ; ++count2 )				// ���� Ÿ��Ʋ ī��Ʈ ��ŭ for���� ������.
//				{
//					pos = NULL ;													// ��ġ ������ ���� �����͸� nulló���� �Ѵ�.
//					pos = pTitleList->FindIndex(count2) ;							// Ÿ��Ʋ�� ��ġ�� �޴´�.
//
//					if( pos )														// ��ġ ������ ��ȿ�ϴٸ�,
//					{
//						ITEM* pTitleLine = (ITEM*)pTitleList->GetAt(pos) ;	// Ÿ��Ʋ ��Ʈ���� �޴´�.
//
//						if( pTitleLine )											// Ÿ��Ʋ ��Ʈ���� ��ȿ�ϴٸ�,
//						{
//							if( pSubQuest )											// ���� ����Ʈ ������ ��ȿ�ϴٸ�,
//							{
//								DWORD dwCurCount = pSubQuest->dwData ;				// ���� ������ �޴´�.
//								DWORD dwMaxCount = pSubQuest->dwMaxCount ;			// �ִ� ������ �޴´�.
//
//								memset(tempBuf, 0, sizeof(tempBuf)) ;				// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
//
//								if( pQString->IsComplete() )
//								{
//									sprintf(tempBuf, "%s (�Ϸ�)", pTitleLine->string) ;							// ���� �ϷḦ �߰��Ѵ�.
//
//									m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(0, 64, 128, 255)) ;			// ����� �ϷḦ ����Ѵ�.
//								}
//								else
//								{
//									// ����� ���� ������ �����Ѵ�.
//									if( pSubQuest->dwMaxCount == 0 )
//									{
//										if( pSubQuest->dwData == 0 )
//										{
//											sprintf(tempBuf, "%s", pTitleLine->string) ;
//										}
//										else
//										{
//											sprintf(tempBuf, "%s (%d)", pTitleLine->string, pSubQuest->dwData) ;
//										}
//									}
//									else
//									{
//										sprintf(tempBuf, "%s (%d/%d)", pTitleLine->string, pSubQuest->dwData, pSubQuest->dwMaxCount) ;
//									}										
//
//									// ����� ���� ������ ����Ʈ�� �߰��Ѵ�.
//									m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(0, 0, 255, 255)) ;
//								}
//							}
//						}
//					}
//				}
//			}
//
//            // - ���� ����Ʈ ���� ����
//			pDescList = pQString->GetDesc() ;										// ���� ����Ʈ ���� ����Ʈ�� �޴´�.
//
//			if( pDescList )															// ���� ����Ʈ ������ ��ȿ�ϴٸ�,
//			{
//				int nDescCount = 0 ;												// ���� ������ ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//				nDescCount = pDescList->GetCount() ;								// ���� ����Ʈ�� ������ ���� �޴´�.
//
//				for( int count2 = 0 ; count2 < nDescCount ; ++count2 )				// ���� ���� ��ŭ for���� ������.
//				{
//					pos = NULL ;													// ��ġ ������ ���� �����͸� nulló���� �Ѵ�.
//					pos = pDescList->FindIndex(count2) ;							// ī��Ʈ�� �ش��ϴ� ��ġ ������ �޴´�.
//
//					if( pos )														// ��ġ ������ ��ȿ�ϴٸ�,
//					{
//						ITEM* pDescLine = (ITEM*)pDescList->GetAt(pos) ;  	// ���� ��Ʈ���� �޴´�.
//
//						if( pDescLine )												// ���� ��Ʈ�� ������ ��ȿ�ϴٸ�,						
//						{
//							m_pQuestDescList->AddItem(pDescLine->string, pDescLine->rgb) ;	// ����Ʈ ���� ����Ʈ�� �߰��� �Ѵ�.
//						}
//					}
//				}
//			}
//
//			pTitleList = NULL ;														// ���� ����Ʈ �����͸� nulló���� �Ѵ�.
//			pDescList  = NULL ;														// ���� ����Ʈ �����͸� nulló���� �Ѵ�.
//		}
//		else																		// ���� ����Ʈ ��Ʈ���� ������,
//		{
//			break ;																	// for���� ����������.
//		}
//
//		pQString = NULL ;															// ��Ʈ�� �����͸� nulló���� �Ѵ�.						
//	}
//
//	////////////////////////////////////////
//	// - ���� ���� ���� -
//	////////////////////////////////////////
//
//	//cIcon* deleteIcon = NULL;														// ������ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	//RequitalItem* pDeleteSQuest = NULL;												// ����Ʈ ������ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	//
//	//PTRLISTSEARCHSTART(m_QuestRequitalList, RequitalItem*, pDeleteSQuest);			// ����Ʈ ������ ����Ʈ���� �˻��� �����Ѵ�.
//	//	if(pDeleteSQuest)															// ����Ʈ ������ ������ ��ȿ���� ������, �˻��� ����������.
//	//	{
//	//		m_pRequitalGrid->DeleteIcon(pDeleteSQuest->wPos, &deleteIcon);			// ����Ʈ ������ �׸��忡�� �������� �����Ѵ�.
//
//	//		if(deleteIcon)															// ������ ������ ��ȿ���� ������ �˻��� ����������.
//	//		{
//	//			deleteIcon->SetActive(FALSE);										// �������� ��Ȱ��ȭ �Ѵ�.
//	//			WINDOWMGR->AddListDestroyWindow(deleteIcon);						// ������ �Ŵ����� �ı��� �����쿡 �������� �߰��Ѵ�.
//	//			m_IndexGenerator.ReleaseIndex(deleteIcon->GetID());					// �ε��� �����⿡�� ������ �ε����� �����Ѵ�.
//	//			m_QuestRequitalList.Remove(pDeleteSQuest);							// ����Ʈ ������ ����Ʈ���� ����Ʈ ������ ������ �����Ѵ�.
//	//		}
//	//	}
//	//PTRLISTSEARCHEND																// ����Ʈ ������ ����Ʈ �˻��� �����Ѵ�.
//
//	//RefreshRequitalItem() ;
//
//	cIcon* pDeleteIcon = NULL ;
//	RequitalItem* pDeleteItem = NULL ;
//
//	PTRLISTPOS deletePos = NULL ;
//	deletePos = m_QuestRequitalList.GetHeadPosition() ;
//
//	while(deletePos)
//	{
//		pDeleteItem = NULL ;
//		pDeleteItem = (RequitalItem*)m_QuestRequitalList.GetNext(deletePos) ;
//
//		if( pDeleteItem )
//		{
//			m_pRequitalGrid->DeleteIcon( pDeleteItem->wPos, &pDeleteItem->pIcon ) ;
//
//			pDeleteItem->pIcon->SetActive(FALSE) ;
//			WINDOWMGR->AddListDestroyWindow(pDeleteItem->pIcon) ;
//
//			m_IndexGenerator.ReleaseIndex( pDeleteItem->pIcon->GetID() ) ;
//
//			m_QuestRequitalList.Remove(pDeleteItem) ;
//
//			delete pDeleteItem ;
//		}
//	}
//
//	m_QuestRequitalList.RemoveAll() ;
//
//	RefreshRequitalItem() ;
//
//
//	pSubQuestInfo = pQuestInfo->GetSubQuestInfo(nSubQuestCount-1) ;					// ������ ���� ����Ʈ ������ �޴´�.
//
//	if( pSubQuestInfo )																// ���� ����Ʈ ������ ��ȿ�ϴٸ�,
//	{
//		cPtrList* pTriggerList = pSubQuestInfo->GetTriggerList() ;					// ������ ���� ����Ʈ�� Ʈ���� ������ �޴´�.
//
//		if( pTriggerList )															// Ʈ���� ������ ��ȿ�ϴٸ�,
//		{
//			int nTriggerCount = 0 ;													// Ʈ���� ī��Ʈ�� ���� ������ �����ϰ�, 0���� �����Ѵ�.
//
//			nTriggerCount = pTriggerList->GetCount() ;								// Ʈ���� ī��Ʈ�� �޴´�.
//
//			CQuestTrigger* pTrigger = NULL ;										// Ʈ���� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//			PTRLISTPOS exePos = NULL ;												// ���� ����Ʈ ��ġ ������ ���� ��ġ �����͸� �����ϰ� null ó���� �Ѵ�.
//
//			for( int count = 0 ; count < nTriggerCount ; ++count )					// Ʈ���� ī��Ʈ �� ��ŭ for���� ������.
//			{
//				pos = NULL ;														// ��ġ ������ ���� �����͸� null ó���� �Ѵ�.
//				pos = pTriggerList->FindIndex(count) ;								// ī��Ʈ�� �ش��ϴ� ��ġ ������ �޴´�.
//
//				if( pos )															// ��ġ ������ ��ȿ�ϴٸ�,
//				{
//					pTrigger = (CQuestTrigger*)pTriggerList->GetAt(pos) ;			// ��ġ�� �ش��ϴ� Ʈ���� ������ �޴´�.
//
//					if( pTrigger )													// Ʈ���� ������ ��ȿ�ϴٸ�,
//					{
//						cPtrList* pExeList = pTrigger->GetExeList() ;				// Ʈ���� ���� ����Ʈ�� �޴´�.
//
//						if( pExeList )												// Ʈ���� ���� ����Ʈ ������ ��ȿ�ϴٸ�,
//						{
//							int nExeCount = pExeList->GetCount() ;					// ���� ����Ʈ ī��Ʈ�� �޴´�.
//
//							CQuestExecute* pExecute = NULL ;						// ����Ʈ ���� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//							for(int count2 = 0 ; count2 < nExeCount ; ++count2 )	// ���� ī��Ʈ ��ŭ for���� ������.
//							{
//								exePos = NULL ;										// ���� ��ġ �����͸� null ó���� �Ѵ�.
//								exePos = pExeList->FindIndex(count2) ;				// ī��Ʈ�� �ش��ϴ� ���� ��ġ ������ �޴´�.									
//
//								if( exePos )										// ����Ʈ ���� ��ġ�� ��ȿ�ϸ�,
//								{
//									pExecute = (CQuestExecute*)pExeList->GetAt(exePos) ;	// ����Ʈ ���� ������ �޴´�.
//	
//									if( pExecute )									// ����Ʈ ���� ������ ��ȿ�ϸ�,
//									{
//										CQuestExecute_Item* pExecute_Item = ((CQuestExecute_Item*)pExecute) ;
//										DWORD dwExeKind = pExecute->GetQuestExecuteKind() ;	// ���� Ÿ���� �޴´�.
//
//										switch(dwExeKind)							// ���� Ÿ���� Ȯ���Ѵ�.
//										{
//										case eQuestExecute_TakeItem :				// ���� Ÿ���� �������� �޴� Ÿ���̶��,
//											{
//												ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(pExecute_Item->GetItemKind());
//
//												if( pItemInfo )
//												{
//													AddRequitalItem(pItemInfo, pExecute_Item->GetItemNum()) ;
//												}
//											}
//											break ;
//
//										case eQuestExecute_TakeMoney :				// ���� Ÿ���� ��带 �޴� Ÿ���̶��,
//											{
//												SetGold( pExecute_Item->GetItemNum() ) ;
//											}
//											break ;
//
//										case eQuestExecute_TakeExp :				// ���� Ÿ���� ����ġ�� �޴� Ÿ���̶��,
//											{
//												SetExp( pExecute_Item->GetItemNum() ) ;
//											}
//											break ;
//
//										}
//									}
//								}
//							}
//						}
//					}
//				}
//
//				pTrigger = NULL ;
//			}
//		}
//	}
//}

//void CQuestDialog::QuestDescView(CQuestString* pQuestString)						// ����Ʈ �� ���� ���� �Լ�.
//{
//	////if( !m_bActive )			return ;											// ����Ʈ â�� ���� ���°� �ƴ϶��, ���� ó���� �Ѵ�.
//
//	//if(!pQuestString)	
//	//{
//	//	m_pQuestDescList->RemoveAll() ;
//
//	//	int nCellCount = m_pRequitalGrid->GetCellNum() ;
//
//	//	cIcon* pIcon ;
//
//	//	for( int count = 0 ; count < nCellCount ; ++count )
//	//	{
//	//		pIcon = NULL ;
//	//		pIcon = m_pRequitalGrid->GetIconForIdx(count) ;
//
//	//		if( pIcon )
//	//		{
//	//			m_pRequitalGrid->DeleteIcon(pIcon) ;
//	//		}
//	//	}
//
//	//	PTRLISTPOS pos = m_QuestRequitalList.GetHeadPosition() ;
//
//	//	RequitalItem* item ;
//
//	//	while( pos )
//	//	{
//	//		item = NULL ;
//	//		item = ( RequitalItem* )m_QuestRequitalList.GetNext( pos ) ;
//
//	//		if( item )
//	//		{
//	//			WINDOWMGR->AddListDestroyWindow( item->pIcon );
//	//			SAFE_DELETE( item ) ;
//	//		}
//	//	}
//
//	//	m_QuestRequitalList.RemoveAll() ;
//
//	//	SetGold(0) ;
//	//	SetExp(0) ;
//
//	//	return;												// ����Ʈ ��Ʈ�� ������ ��ȿ���� ������, ���� ó���� �Ѵ�.
//	//}
//
//	//char tempBuf[128] = {0, } ;														// �ӽù��۸� �����ϰ� �ʱ�ȭ �Ѵ�.
//
//	//PTRLISTPOS pos = NULL;															// ��ġ ������ ���� ������ ������ �����ϰ� nulló�� �Ѵ�.
//
//	//DWORD dwMainQuestID = pQuestString->GetQuestID() ;								// ��Ʈ������ ����Ʈ ��ȣ�� �޴´�.
//
//	//CQuestInfo* pQuestInfo = QUESTMGR->GetQuestInfo(dwMainQuestID) ;				// ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.
//
//	//if( !pQuestInfo )																// ����Ʈ ������ ��ȿ���� �ʴٸ�,
//	//{
//	//	return ;																	// ���� ó���� �Ѵ�.
//	//}
//
//	//DWORD dwNpcIdx = pQuestInfo->GetNpcIdx() ;										// ����Ʈ �������� npc �ε����� �޴´�.
//
//	//stNPCIDNAME* pNpcIdName = QUESTMGR->GetNpcIdName(dwNpcIdx) ;					// ����Ʈ �Ŵ������� npc ���̵�, �̸��� �޴´�.
//
//	//int nMapNum = pQuestInfo->GetMapNum() ;											// ����Ʈ �������� �� ��ȣ�� �޴´�.
//	//
//	//m_pQuestDescList->RemoveAll();													// ����Ʈ ����Ʈ ���̾�α׸� ��� ����.
//
//	//////////////////////////////////////////
//	//// - ����Ʈ �̸�, �Ƿ���, ��ġ ���� -
//	//////////////////////////////////////////
//
//	//m_toPreList= -1;																// ���� ����Ʈ�� -1�� �����Ѵ�.
//
//	//QString* pQStr = NULL;															// ����Ʈ ��Ʈ�� ������ ���� ������ �����ϰ� nulló�� �Ѵ�.
//
//	//CQuestString* pTitleString = QUESTMGR->GetQuestString(pQuestString->GetQuestID(), 0) ;
//
//	//pos = pTitleString->GetTitle()->GetHeadPosition();								// ����Ʈ ��Ʈ���� �ص� ��ġ�� �����Ѵ�.
//
//	//// - ����Ʈ ���� ���� - 
//
//	//while(pos)																		// ��ġ ������ ��ȿ�� ���� while���� ������.
//	//{
//	//	pQStr = (QString*)pTitleString->GetTitle()->GetNext(pos);					// ��ġ ������ �ش��ϴ� ����Ʈ ��Ʈ�� ������ �޴´�.
//
//	//	if(pQStr)																	// ����Ʈ ��Ʈ�� ������ ��ȿ�ϴٸ�,
//	//	{
//	//		sprintf(tempBuf, "< %s >", pQStr->Str) ;								// �ӽ� ���ۿ� ����Ʈ ������ �����Ѵ�.
//	//		m_pQuestDescList->AddItem(tempBuf, MAINQUEST_TITLE_COLOR);				// ����Ʈ ����Ʈ ���̾�α׿� �߰��Ѵ�.
//	//		++m_toPreList;															// ���� ����Ʈ ���� �����Ѵ�.
//	//	}
//	//}
//
//	//m_pQuestDescList->AddItem(" ", 0);												// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.
//	//++m_toPreList;																	// ���� ����Ʈ ���� �����Ѵ�.
//
//	//// - �Ƿ��� ���� - 
//	//if( pNpcIdName )																// npc ������ ��ȿ�ϸ�,
//	//{
//	//	memset(tempBuf, 0, sizeof(tempBuf)) ;										// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
//	//	sprintf(tempBuf, "�Ƿ��� - %s", pNpcIdName->name) ;							// �Ƿ��� �̸����� npc �̸��� �����Ѵ�.
//	//	m_pQuestDescList->AddItem(tempBuf, SUBQUEST_TITLE_COLOR_SEL);				// ����Ʈ ���� ����Ʈ�� �߰��Ѵ�.
//	//}
//	//else																			// npc ������ ��ȿ���� ������,
//	//{
//	//	ASSERTMSG(0, "����Ʈ ����â�� NPC ������ �ε����� ���߽��ϴ�.") ;			// npc ������ �и��� �־�� �Ѵ�.
//	//	m_pQuestDescList->AddItem("<*********>", SUBQUEST_TITLE_COLOR_SEL);			// �˾ƺ� �� �ְ� <*********> �� ǥ���Ѵ�.
//	//}
//
//	//// - ��ġ�� �����Ѵ�. -
//	//memset(tempBuf, 0, sizeof(tempBuf)) ;											// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
//	//char* pMapName = GetMapName(nMapNum) ;											// �� ��ȣ�� ���� �� �̸��� �޴´�.	
//	//sprintf(tempBuf, "��ġ - %s", pMapName) ;										// ��ġ�� �� �̸��� �߰��Ѵ�.
//	//m_pQuestDescList->AddItem(tempBuf, SUBQUEST_TITLE_COLOR_SEL);					// ����Ʈ ���� �߰��Ѵ�.
//
//
//	//////////////////////////////////////////
//	//// - �Ƿ� ���� ���� -
//	//////////////////////////////////////////
//
//	//m_pQuestDescList->AddItem(" ", 0);												// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.
//	//m_pQuestDescList->AddItem("< �Ƿ� ���� > ", MAINQUEST_TITLE_COLOR);				// <�Ƿڳ���>�� �߰��Ѵ�.
//	//m_pQuestDescList->AddItem(" ", 0);												// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.
//
//	//int nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// ���� ����Ʈ ���� �޴´�.
//
//	//cDialogueList* pDialogueList = NULL ;											// ���̾�α� ����Ʈ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	//pDialogueList = NPCSCRIPTMGR->GetDialogueList();								// NPC ��ũ��Ʈ �Ŵ������� ���̾�α� ����Ʈ�� �޴´�.
//	//if( pDialogueList == NULL ) return ;											// ���̾�α� ������ ��ȿ���� ������, FALSE ���� ó���� �Ѵ�.
//
//	//CSubQuestInfo* pSubQuestInfo = NULL ;											// ���� ����Ʈ ������ ���� �����͸� �����ϰ� NULLó���� �Ѵ�.
//	//pSubQuestInfo = pQuestInfo->GetSubQuestInfo(0) ;								// ī��Ʈ�� �ش��ϴ� ���� ����Ʈ ������ �޴´�.
//
//	//if( pSubQuestInfo )																// ���� ����Ʈ ������ ��ȿ�ϴٸ�,
//	//{
//	//	DWORD dwPage = pSubQuestInfo->GetNpcScriptPage(dwNpcIdx) ;					// NPC �ε����� ������ �ε����� �޴´�.
//	//	cPage* pPage = NPCSCRIPTMGR->GetPage( dwNpcIdx, dwPage );					// ���� NPC�� ���� ������ ���̵�� ������ ������ �޴´�.
//
//	//	if( pPage )																	// ������ ������ ��ȿ�ϴٸ�,
//	//	{
//	//		WORD wIdx = 0;															// �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
//	//		WORD LineInfo = 0;														// ���� ������ ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//	//		DIALOGUE* temp = NULL;													// ���̾�α� ������ ���� ������ ������ �����ϰ� NULLó���� �Ѵ�.
//
//	//		LINKITEM* pItem = NULL;													// ��ũ ������ ������ ���� ������ ������ �����ϰ� NULL ó���� �Ѵ�.
//	//		LINKITEM* Prev = NULL;													// ���� ��ũ ������ ������ ���� ������ ������ �����ϰ� NULL ó���� �Ѵ�.
//
//	//		DWORD dwDialogue = pPage->GetDialogue() ;								// �������� ���̾�α� ��ȣ�� �޴´�.
//
//	//		// *������ ���̾�α׸� �д� �����, ������ npc ��ȭâ���� �µ��� �Ǿ��־���.
//	//		// ������ ��縦 ��� �о�鿩 �ӽ� ���ۿ� ���, ����ϰ��� �ϴ� ����Ʈ�� 
//	//		// �ؽ�Ʈ ���̿� �°� �ٽ� �ؽ�Ʈ Ŀ�� �۾��� �Ѵ�.
//	//		char npcDialog[1024] = {0, } ;											// npc ��縦 ���� �ӽ� ���۸� �����Ѵ�.
//
//	//		while(TRUE)																// while���� ������.
//	//		{
//	//			//temp = pDialogueList->GetDialogue( dwDialogue, wIdx );				// �����ϰ� ���� �������� ���̾�α� ������ �޴´�.
//
//	//			//if( temp == NULL ) break;											// ������ ��ȿ���� ������, while���� ����������.
//
//	//			//pItem = new LINKITEM;												// ������ �޸𸮸� �Ҵ��Ͽ� �����ͷ� �ѱ��.
//
//	//			//strcpy(pItem->string, temp->str);									// �ӽ� ������ ��Ʈ���� ������ ��Ʈ���� ���縦 �Ѵ�.
//
//	//			////pItem->rgb = RGBA_MAKE(10, 10, 10, 255);							// �ӽ� ������ ������ ������ �������� �����Ѵ�.
//	//			//pItem->line = m_pQuestDescList->GetItemCount() ;					// ������ �����Ѵ�.
//
//	//			//m_pQuestDescList->cListItem::AddItem(pItem) ;						// ����Ʈ�� ��Ʈ�� �������� �߰��Ѵ�.
//
//	//			//++wIdx;																// �ε��� ������ �����Ѵ�.
//
//	//			temp = pDialogueList->GetDialogue( dwDialogue, wIdx );				// �����ϰ� ���� �������� ���̾�α� ������ �޴´�.
//
//	//			if( temp == NULL ) break;											// ������ ��ȿ���� ������, while���� ����������.
//
//	//			strcat(npcDialog, temp->str) ;										// �о���� ��縦 �ӽ� ���ۿ� �߰��Ѵ�.
//
//	//			++wIdx ;															// �ε����� �����Ѵ�.
//	//		}
//
//	//		m_pQuestDescList->AddItemAutoLine(npcDialog, RGBA_MAKE(10, 10, 10, 255)) ;
//	//	}
//	//}
//
//
//	//////////////////////////////////////////
//	//// - ���� ����Ʈ ���� ���� -
//	//////////////////////////////////////////
//
//	//int QuestStringKey = 0 ;														// ����Ʈ ��Ʈ�� Ű�� ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//	//CQuestString* pQString = NULL ;													// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	//cPtrList* pTitleList = NULL ;													// ���� ����Ʈ ���� ����Ʈ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	//cPtrList* pDescList = NULL ;													// ���� ����Ʈ ���� ����Ʈ�� ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	//CQuest* pQuest = QUESTMGR->GetQuest( dwMainQuestID );							// ����Ʈ ������ �޴´�.
//
// //   for( int count = 1 ; count < nSubQuestCount ; ++count )							// ���� ����Ʈ ��-1��ŭ for���� ������.
//	//{
//	//	COMBINEKEY(dwMainQuestID, count, QuestStringKey) ;							// ����Ʈ, ���� ����Ʈ �ε����� ����Ʈ ��Ʈ���� ���Ѵ�.
//
//	//	SUBQUEST* pSubQuest = pQuest->GetSubQuest(count) ;							// ���� ����Ʈ ������ �޴´�.
//
//	//	pQString = QUESTMGR->GetQuestString(QuestStringKey);						// ���� ����Ʈ ��Ʈ���� �޴´�.
//
//	//	if( pQString )																// ���� ����Ʈ ��Ʈ���� ��ȿ�ϸ�,
//	//	{
//	//		// - ���� ����Ʈ Ÿ��Ʋ ���� -
//	//		pTitleList = pQString->GetTitle() ;										// ���� ����Ʈ�� Ÿ��Ʋ ����Ʈ�� �޴´�.
//
//	//		if( pTitleList )														// Ÿ��Ʋ ����Ʈ�� ��ȿ�ϸ�,
//	//		{
//	//			m_pQuestDescList->AddItem(" ", 0);									// ����Ʈ ����Ʈ�� ��ĭ�� �߰��Ѵ�.
//
//	//			int nTitleCount = 0 ;												// ���� ����Ʈ�� Ÿ��Ʋ ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//	//			nTitleCount = pTitleList->GetCount() ;								// ���� ����Ʈ�� Ÿ��Ʋ�� �����Ѵ�.
//
//	//			for( int count2 = 0 ; count2 < nTitleCount ; ++count2 )				// ���� Ÿ��Ʋ ī��Ʈ ��ŭ for���� ������.
//	//			{
//	//				pos = NULL ;													// ��ġ ������ ���� �����͸� nulló���� �Ѵ�.
//	//				pos = pTitleList->FindIndex(count2) ;							// Ÿ��Ʋ�� ��ġ�� �޴´�.
//
//	//				if( pos )														// ��ġ ������ ��ȿ�ϴٸ�,
//	//				{
//	//					QString* pTitleLine = (QString*)pTitleList->GetAt(pos) ;	// Ÿ��Ʋ ��Ʈ���� �޴´�.
//
//	//					if( pTitleLine )											// Ÿ��Ʋ ��Ʈ���� ��ȿ�ϴٸ�,
//	//					{
//	//						if( pSubQuest )											// ���� ����Ʈ ������ ��ȿ�ϴٸ�,
//	//						{
//	//							DWORD dwCurCount = pSubQuest->dwData ;				// ���� ������ �޴´�.
//	//							DWORD dwMaxCount = pSubQuest->dwMaxCount ;			// �ִ� ������ �޴´�.
//
//	//							memset(tempBuf, 0, sizeof(tempBuf)) ;				// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
//
//	//							if( pQString->IsComplete() )
//	//							{
//	//								sprintf(tempBuf, "%s (�Ϸ�)", pTitleLine->Str) ;							// ���� �ϷḦ �߰��Ѵ�.
//
//	//								m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(0, 64, 128, 255)) ;			// ����� �ϷḦ ����Ѵ�.
//	//							}
//	//							else
//	//							{
//	//								// ����� ���� ������ �����Ѵ�.
//	//								if( pSubQuest->dwMaxCount == 0 )
//	//								{
//	//									if( pSubQuest->dwData == 0 )
//	//									{
//	//										sprintf(tempBuf, "%s", pTitleLine->Str) ;
//	//									}
//	//									else
//	//									{
//	//										sprintf(tempBuf, "%s (%d)", pTitleLine->Str, pSubQuest->dwData) ;
//	//									}
//	//								}
//	//								else
//	//								{
//	//									sprintf(tempBuf, "%s (%d/%d)", pTitleLine->Str, pSubQuest->dwData, pSubQuest->dwMaxCount) ;
//	//								}										
//
//	//								// ����� ���� ������ ����Ʈ�� �߰��Ѵ�.
//	//								m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(0, 0, 255, 255)) ;
//	//							}
//
//	//							//if( dwCurCount == dwMaxCount )						// ������ ��ġ �� ���,
//	//							//{
//	//							//	if( dwCurCount > 0 )							// ���� ������ 1�� �̻��̸�,															
//	//							//	{
//	//							//		sprintf(tempBuf, "%s (�Ϸ�)", pTitleLine->Str) ;							// ���� �ϷḦ �߰��Ѵ�.
//
//	//							//		m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(0, 64, 128, 255)) ;			// ����� �ϷḦ ����Ѵ�.
//	//							//	}
//	//							//	else											// ���� ������ 0�̰ų� �� ������ ��,
//	//							//	{
//	//							//		m_pQuestDescList->AddItem(pTitleLine->Str, RGBA_MAKE(0, 64, 128, 255)) ;	// ���� ����Ѵ�.
//	//							//	}
//	//							//}
//	//							//else												// ������ ��ġ���� �������,
//	//							//{
//	//							//	if( dwCurCount > 0 )							// ���� ������ �ϳ� �̻��̸�,
//	//							//	{
//	//							//		// ����� ���� ������ �����Ѵ�.
//	//							//		if( pSubQuest->dwMaxCount == 0 )
//	//							//		{
//	//							//			sprintf(tempBuf, "%s (%d)", pTitleLine->Str, pSubQuest->dwData) ;
//	//							//		}
//	//							//		else
//	//							//		{
//	//							//			sprintf(tempBuf, "%s (%d/%d)", pTitleLine->Str, pSubQuest->dwData, pSubQuest->dwMaxCount) ;
//	//							//		}										
//
//	//							//		// ����� ���� ������ ����Ʈ�� �߰��Ѵ�.
//	//							//		m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(0, 64, 128, 255)) ;
//	//							//	}
//	//							//}
//	//						}
//	//					}
//	//				}
//	//			}
//	//		}
//
// //           // - ���� ����Ʈ ���� ����
//	//		pDescList = pQString->GetDesc() ;										// ���� ����Ʈ ���� ����Ʈ�� �޴´�.
//
//	//		if( pDescList )															// ���� ����Ʈ ������ ��ȿ�ϴٸ�,
//	//		{
//	//			int nDescCount = 0 ;												// ���� ������ ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//	//			nDescCount = pDescList->GetCount() ;								// ���� ����Ʈ�� ������ ���� �޴´�.
//
//	//			for( int count2 = 0 ; count2 < nDescCount ; ++count2 )				// ���� ���� ��ŭ for���� ������.
//	//			{
//	//				pos = NULL ;													// ��ġ ������ ���� �����͸� nulló���� �Ѵ�.
//	//				pos = pDescList->FindIndex(count2) ;							// ī��Ʈ�� �ش��ϴ� ��ġ ������ �޴´�.
//
//	//				if( pos )														// ��ġ ������ ��ȿ�ϴٸ�,
//	//				{
//	//					QString* pDescLine = (QString*)pDescList->GetAt(pos) ;  	// ���� ��Ʈ���� �޴´�.
//
//	//					if( pDescLine )												// ���� ��Ʈ�� ������ ��ȿ�ϴٸ�,						
//	//					{
//	//						m_pQuestDescList->AddItem(pDescLine->Str, pDescLine->Color) ;	// ����Ʈ ���� ����Ʈ�� �߰��� �Ѵ�.
//	//					}
//	//				}
//	//			}
//	//		}
//
//	//		pTitleList = NULL ;														// ���� ����Ʈ �����͸� nulló���� �Ѵ�.
//	//		pDescList  = NULL ;														// ���� ����Ʈ �����͸� nulló���� �Ѵ�.
//	//	}
//	//	else																		// ���� ����Ʈ ��Ʈ���� ������,
//	//	{
//	//		break ;																	// for���� ����������.
//	//	}
//
//	//	pQString = NULL ;															// ��Ʈ�� �����͸� nulló���� �Ѵ�.						
//	//}
//
//	//////////////////////////////////////////
//	//// - ���� ���� ���� -
//	//////////////////////////////////////////
//
//	//cIcon* deleteIcon = NULL;														// ������ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	//RequitalItem* pDeleteSQuest = NULL;												// ����Ʈ ������ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	//
//	//PTRLISTSEARCHSTART(m_QuestRequitalList, RequitalItem*, pDeleteSQuest);			// ����Ʈ ������ ����Ʈ���� �˻��� �����Ѵ�.
//	//	if(pDeleteSQuest)															// ����Ʈ ������ ������ ��ȿ���� ������, �˻��� ����������.
//	//	{
//	//		m_pRequitalGrid->DeleteIcon(pDeleteSQuest->wPos, &deleteIcon);			// ����Ʈ ������ �׸��忡�� �������� �����Ѵ�.
//
//	//		if(deleteIcon)															// ������ ������ ��ȿ���� ������ �˻��� ����������.
//	//		{
//	//			deleteIcon->SetActive(FALSE);										// �������� ��Ȱ��ȭ �Ѵ�.
//	//			WINDOWMGR->AddListDestroyWindow(deleteIcon);						// ������ �Ŵ����� �ı��� �����쿡 �������� �߰��Ѵ�.
//	//			m_IndexGenerator.ReleaseIndex(deleteIcon->GetID());					// �ε��� �����⿡�� ������ �ε����� �����Ѵ�.
//	//			m_QuestRequitalList.Remove(pDeleteSQuest);							// ����Ʈ ������ ����Ʈ���� ����Ʈ ������ ������ �����Ѵ�.
//	//		}
//	//	}
//	//PTRLISTSEARCHEND																// ����Ʈ ������ ����Ʈ �˻��� �����Ѵ�.
//
//	//RefreshRequitalItem() ;
//
//
//	//pSubQuestInfo = pQuestInfo->GetSubQuestInfo(nSubQuestCount-1) ;					// ������ ���� ����Ʈ ������ �޴´�.
//
//	//if( pSubQuestInfo )																// ���� ����Ʈ ������ ��ȿ�ϴٸ�,
//	//{
//	//	cPtrList* pTriggerList = pSubQuestInfo->GetTriggerList() ;					// ������ ���� ����Ʈ�� Ʈ���� ������ �޴´�.
//
//	//	if( pTriggerList )															// Ʈ���� ������ ��ȿ�ϴٸ�,
//	//	{
//	//		int nTriggerCount = 0 ;													// Ʈ���� ī��Ʈ�� ���� ������ �����ϰ�, 0���� �����Ѵ�.
//
//	//		nTriggerCount = pTriggerList->GetCount() ;								// Ʈ���� ī��Ʈ�� �޴´�.
//
//	//		CQuestTrigger* pTrigger = NULL ;										// Ʈ���� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	//		PTRLISTPOS exePos = NULL ;												// ���� ����Ʈ ��ġ ������ ���� ��ġ �����͸� �����ϰ� null ó���� �Ѵ�.
//
//	//		for( int count = 0 ; count < nTriggerCount ; ++count )					// Ʈ���� ī��Ʈ �� ��ŭ for���� ������.
//	//		{
//	//			pos = NULL ;														// ��ġ ������ ���� �����͸� null ó���� �Ѵ�.
//	//			pos = pTriggerList->FindIndex(count) ;								// ī��Ʈ�� �ش��ϴ� ��ġ ������ �޴´�.
//
//	//			if( pos )															// ��ġ ������ ��ȿ�ϴٸ�,
//	//			{
//	//				pTrigger = (CQuestTrigger*)pTriggerList->GetAt(pos) ;			// ��ġ�� �ش��ϴ� Ʈ���� ������ �޴´�.
//
//	//				if( pTrigger )													// Ʈ���� ������ ��ȿ�ϴٸ�,
//	//				{
//	//					cPtrList* pExeList = pTrigger->GetExeList() ;				// Ʈ���� ���� ����Ʈ�� �޴´�.
//
//	//					if( pExeList )												// Ʈ���� ���� ����Ʈ ������ ��ȿ�ϴٸ�,
//	//					{
//	//						int nExeCount = pExeList->GetCount() ;					// ���� ����Ʈ ī��Ʈ�� �޴´�.
//
//	//						CQuestExecute* pExecute = NULL ;						// ����Ʈ ���� ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	//						for(int count2 = 0 ; count2 < nExeCount ; ++count2 )	// ���� ī��Ʈ ��ŭ for���� ������.
//	//						{
//	//							exePos = NULL ;										// ���� ��ġ �����͸� null ó���� �Ѵ�.
//	//							exePos = pExeList->FindIndex(count2) ;				// ī��Ʈ�� �ش��ϴ� ���� ��ġ ������ �޴´�.									
//
//	//							if( exePos )										// ����Ʈ ���� ��ġ�� ��ȿ�ϸ�,
//	//							{
//	//								pExecute = (CQuestExecute*)pExeList->GetAt(exePos) ;	// ����Ʈ ���� ������ �޴´�.
//	//
//	//								if( pExecute )									// ����Ʈ ���� ������ ��ȿ�ϸ�,
//	//								{
//	//									CQuestExecute_Item* pExecute_Item = ((CQuestExecute_Item*)pExecute) ;
//	//									DWORD dwExeKind = pExecute->GetQuestExecuteKind() ;	// ���� Ÿ���� �޴´�.
//
//	//									switch(dwExeKind)							// ���� Ÿ���� Ȯ���Ѵ�.
//	//									{
//	//									case eQuestExecute_TakeItem :				// ���� Ÿ���� �������� �޴� Ÿ���̶��,
//	//										{
//	//											ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(pExecute_Item->GetItemKind());
//
//	//											if( pItemInfo )
//	//											{
//	//												AddRequitalItem(pItemInfo, pExecute_Item->GetItemNum()) ;
//	//											}
//
//
//	//											/*QUEST_ITEM_INFO* pData = QUESTMGR->GetQuestItemInfo( pExecute_Item->GetItemKind() );
//	//											if( pData )
//	//											{
//	//												AddRequitalItem(pData, pExecute_Item->GetItemNum()) ;
//	//											}*/
//	//										}
//	//										break ;
//
//	//									case eQuestExecute_TakeMoney :				// ���� Ÿ���� ��带 �޴� Ÿ���̶��,
//	//										{
//	//											SetGold( pExecute_Item->GetItemNum() ) ;
//	//										}
//	//										break ;
//
//	//									case eQuestExecute_TakeExp :				// ���� Ÿ���� ����ġ�� �޴� Ÿ���̶��,
//	//										{
//	//											SetExp( pExecute_Item->GetItemNum() ) ;
//	//										}
//	//										break ;
//
//	//									}
//	//								}
//	//							}
//	//						}
//	//					}
//	//				}
//	//			}
//
//	//			pTrigger = NULL ;
//	//		}
//	//	}
//	//}
//}


void CQuestDialog::AddRequitalItem(ITEM_INFO* pItemInfo, DWORD Count)
{
	if( !pItemInfo ) return ;

	RequitalItem* pSQuest = new RequitalItem;										// ����Ʈ ������ �޸𸮸� �Ҵ��Ѵ�.
	memcpy(&pSQuest->itemInfo, pItemInfo, sizeof(ITEM_INFO)) ;						// ����Ʈ ������ ������ ���ڷ� �Ѿ�� ������ �����Ѵ�.
	pSQuest->nItemCount = (int)Count;												// ����Ʈ �������� ī��Ʈ�� �����Ѵ�.
	pSQuest->wPos = 0;																// ����Ʈ �������� ��ġ�� -1�� �����Ѵ�.

	cIcon* pIcon = new cIcon;														// ������ �޸𸮸� �Ҵ��Ѵ�.
	cImage Image;																	// �̹����� �����Ѵ�.

	SCRIPTMGR->GetImage(pSQuest->itemInfo.Image2DNum, &Image, PFT_ITEMPATH);		// ����Ʈ ������ ������ �̹����� �̹����� �����Ѵ�.
	pIcon->SetBasicImage(&Image);													// �������� �⺻ �̹����� ������ ������ �̹����� �����Ѵ�.
	pIcon->Init(0, 0, 40, 40, &Image, m_IndexGenerator.GenerateIndex() );			// �������� �ʱ�ȭ �Ѵ�.
	pIcon->SetMovable(FALSE);														// �̵� ���θ� FALSE�� �����Ѵ�.
	pIcon->SetActive(FALSE);														// Ȱ��ȭ ���θ� FALSE�� �����Ѵ�.

	pSQuest->pIcon = pIcon ;

	m_QuestRequitalList.AddTail(pSQuest);											// ����Ʈ ������ ������ ����Ʈ ������ ����Ʈ�� �߰��Ѵ�.

	WINDOWMGR->AddWindow(pIcon);													// ������ �Ŵ����� ������ ������ �߰��Ѵ�.

	RefreshRequitalItem() ;
}

//void CQuestDialog::AddRequitalItem(ITEM_INFO* pItemInfo, DWORD Count)
//{
//	if( !pItemInfo ) return ;
//
//	RequitalItem* pSQuest = new RequitalItem;										// ����Ʈ ������ �޸𸮸� �Ҵ��Ѵ�.
//	memcpy(&pSQuest->itemInfo, pItemInfo, sizeof(ITEM_INFO)) ;						// ����Ʈ ������ ������ ���ڷ� �Ѿ�� ������ �����Ѵ�.
//	pSQuest->nItemCount = (int)Count;												// ����Ʈ �������� ī��Ʈ�� �����Ѵ�.
//	pSQuest->wPos = 0;																// ����Ʈ �������� ��ġ�� -1�� �����Ѵ�.
//
//	m_QuestRequitalList.AddTail(pSQuest);											// ����Ʈ ������ ������ ����Ʈ ������ ����Ʈ�� �߰��Ѵ�.
//
//	// �ִ� ������ �����.
//
//	cIcon* pIcon = new cIcon;														// ������ �޸𸮸� �Ҵ��Ѵ�.
//	cImage Image;																	// �̹����� �����Ѵ�.
//
//	SCRIPTMGR->GetImage(pSQuest->itemInfo.Image2DNum, &Image, PFT_ITEMPATH);		// ����Ʈ ������ ������ �̹����� �̹����� �����Ѵ�.
//	pIcon->SetBasicImage(&Image);													// �������� �⺻ �̹����� ������ ������ �̹����� �����Ѵ�.
//	pIcon->Init(0, 0, 40, 40, &Image, m_IndexGenerator.GenerateIndex() );			// �������� �ʱ�ȭ �Ѵ�.
//	pIcon->SetMovable(FALSE);														// �̵� ���θ� FALSE�� �����Ѵ�.
//	pIcon->SetActive(FALSE);														// Ȱ��ȭ ���θ� FALSE�� �����Ѵ�.
//
//	pSQuest->pIcon = pIcon ;
//
//	WINDOWMGR->AddWindow(pIcon);													// ������ �Ŵ����� ������ ������ �߰��Ѵ�.
//
//	RefreshRequitalItem() ;
//}

void CQuestDialog::AddListEmptyLine()												// �� ������ �߰��ϴ� �Լ�.
{
	m_pQuestListDlg->AddItem(" ", 0);												// ����Ʈ ���̾�α׿� �� ������ �߰��Ѵ�.
	++m_ListLineIdx;																// ����Ʈ ���� �ε����� �����Ѵ�.
	m_QuestList[m_QListCount] = NULL;												// ����Ʈ ����Ʈ��, ����Ʈ ����Ʈ ī��Ʈ�� �ش��ϴ� �迭�� nulló�� �Ѵ�.											
	++m_QListCount;																	// ����Ʈ ����Ʈ ī��Ʈ�� �����Ѵ�.
}

void CQuestDialog::GiveupQuestDelete(DWORD QuestIdx)								// ����Ʈ ���� ���� �Լ�.
{
//	if(QuestIdx > 100)			return;

	QuestTree* pQTree = NULL;														// Ʈ�� ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.

	PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pQTree);							// ����Ʈ Ʈ�� ����Ʈ�� �˻��Ѵ�.
	{
		if( pQTree )																// ����Ʈ Ʈ�� ������ ��ȿ���� üũ�Ѵ�.
		{
			if(pQTree->QuestIdx == QuestIdx)										// ����Ʈ Ʈ�� �ε����� ������ �ε����� ������,
			{
				pQTree->list.RemoveAll();											// Ʈ�� ������ ����Ʈ�� ��� ����.
				pQTree->State = eQTree_Close;										// Ʈ�� ������ ���¸� �����ִ� ���·� �����Ѵ�.
				pQTree->Count = 0;													// Ʈ�� ������ ī��Ʈ�� 0���� �����Ѵ�.
				pQTree->QuestIdx = 0;												// Ʈ�� ������ ����Ʈ �ε������� 0���� �����Ѵ�.
				m_QuestTree.Remove(pQTree);											// ����Ʈ Ʈ�� ����Ʈ���� Ʈ�� ������ ����.
				delete pQTree;														// Ʈ�� ������ �����Ѵ�.
				--m_QTreeCount;														// ����Ʈ Ʈ�� ī��Ʈ�� �����Ѵ�.
				break;																// �˻��� �������´�.
			}
		}
	}
	PTRLISTSEARCHEND																// �˻��� �����Ѵ�.

	DWORD Idx = 0;																	// �ε��� ������ �����ϰ� 0���� �����Ѵ�.
	cIcon* pIcon = NULL;															// ������ �����͸� �����ϰ� nulló���� �Ѵ�.
	sQuestItem* pSQuest = NULL;														// ����Ʈ ������ �����͸� �����ϰ� nulló���� �Ѵ�.

	PTRLISTSEARCHSTART(m_QuestItemList, sQuestItem*, pSQuest);						// ����Ʈ ������ ����Ʈ�� �˻��Ѵ�.
		if(!pSQuest)			break;												// ����Ʈ ������ ������ ��ȿ���� ������ �˻��� ����������.

		Idx = pSQuest->pQuestItemInfo->QuestKey/100;								// ����Ʈ ������ �ε����� �޴´�.

		if(Idx == QuestIdx)															// �ε����� �����Ϸ��� ����Ʈ �ε����� ������,
		{
			m_pIconGridDlg->DeleteIcon(pSQuest->Pos, &pIcon);						// ����Ʈ ������ �׸��忡�� �������� �����Ѵ�.

			if(!pIcon)			break;												// ������ ������ ��ȿ���� ������ �˻��� ����������.

			WINDOWMGR->AddListDestroyWindow(pIcon);									// ������ �Ŵ����� �ı��� ������� �������� �߰��Ѵ�.
			m_IndexGenerator.ReleaseIndex(pIcon->GetID());							// �ε��� �����⿡�� �������� ���� �Ѵ�.
			m_QuestItemList.Remove(pSQuest);										// ����Ʈ ������ ����Ʈ����, �������� �����Ѵ�.
			--m_QuestItemCount;														// ����Ʈ ������ ī��Ʈ�� �����Ѵ�.
		}	
	PTRLISTSEARCHEND																// �˻��� �����Ѵ�.

	QuestListReset();																// ����Ʈ ����Ʈ�� ���ΰ�ħ�Ѵ�.
	RefreshQuestItem(m_QuestItemPage);												// ����Ʈ ������ �׸��带 ���ΰ�ħ�Ѵ�.
}

void CQuestDialog::QuestItemAdd(QUEST_ITEM_INFO* pQuestItemInfo, DWORD Count)		// ����Ʈ �������� �߰��ϴ� �Լ�.
{
	if(!pQuestItemInfo)			return;												// ����Ʈ ������ ������ ��ȿ���� ������ ���� ó���� �Ѵ�.

	// 070511 LYW --- QuestDialog : Check quest item.
	sQuestItem* pQuestItem = NULL ;													// ����Ʈ ������ ������ ���� �����͸� �����ϰ� NULLó���� �Ѵ�.

	PTRLISTSEARCHSTART(m_QuestItemList,sQuestItem*,pQuestItem)						// ����Ʈ ������ ����Ʈ�� �˻��Ѵ�.
		if( pQuestItem->pQuestItemInfo->ItemIdx == pQuestItemInfo->ItemIdx )		// ����Ʈ ������ ������ ������ �ε�����, ���ڷ� �Ѿ�� ������ �ε����� ������,
		{
			pQuestItem->Count = Count ;												// ����Ʈ ������ ����Ʈ�� ������ ī��Ʈ�� �����Ѵ�.
			return ;																// ���� ó���� �Ѵ�.
		}
	PTRLISTSEARCHEND																// ����Ʈ ������ ����Ʈ �˻��� �����Ѵ�.

	sQuestItem* pSQuest = new sQuestItem;											// ����Ʈ ������ �޸𸮸� �Ҵ��Ѵ�.
	pSQuest->pQuestItemInfo = pQuestItemInfo;										// ����Ʈ ������ ������ ���ڷ� �Ѿ�� ������ �����Ѵ�.
	pSQuest->Count = Count;															// ����Ʈ �������� ī��Ʈ�� �����Ѵ�.
	pSQuest->Pos = -1;																// ����Ʈ �������� ��ġ�� -1�� �����Ѵ�.
	
	//
	cIcon* pIcon = new cIcon;														// ������ �޸𸮸� �Ҵ��Ѵ�.
	cImage Image;																	// �̹����� �����Ѵ�.

	SCRIPTMGR->GetImage(pSQuest->pQuestItemInfo->Image2DNum, &Image, PFT_ITEMPATH);	// ����Ʈ ������ ������ �̹����� �̹����� �����Ѵ�.
	pIcon->SetBasicImage(&Image);													// �������� �⺻ �̹����� ������ ������ �̹����� �����Ѵ�.
	pIcon->Init(0, 0, 40, 40, &Image, m_IndexGenerator.GenerateIndex() );			// �������� �ʱ�ȭ �Ѵ�.
	pIcon->SetMovable(FALSE);														// �̵� ���θ� FALSE�� �����Ѵ�.
	pIcon->SetActive(FALSE);														// Ȱ��ȭ ���θ� FALSE�� �����Ѵ�.

	pSQuest->pIcon = pIcon;															// ����Ʈ ������ ������ �������� ������ ������ ���������� �����Ѵ�.

	m_QuestItemList.AddTail(pSQuest);												// ����Ʈ ������ ������ ����Ʈ ������ ����Ʈ�� �߰��Ѵ�.

	WINDOWMGR->AddWindow(pIcon);													// ������ �Ŵ����� ������ ������ �߰��Ѵ�.
	//

	++m_QuestItemCount;																// ����Ʈ ������ ī��Ʈ�� �����Ѵ�.

	RefreshQuestItem(m_QuestItemPage);												// ����Ʈ ������ �������� ���ΰ�ħ�Ѵ�.
}


// ������������������������������������������������������������
// ��														 ��
// ��	QuestItem											 ��
// ��														 ��
// ������������������������������������������������������������
//void CQuestDialog::QuestItemAdd(QUEST_ITEM_INFO* pQuestItemInfo, DWORD Count)		// ����Ʈ �������� �߰��ϴ� �Լ�.
//{
//	if(!pQuestItemInfo)			return;												// ����Ʈ ������ ������ ��ȿ���� ������ ���� ó���� �Ѵ�.
//
//	// 070511 LYW --- QuestDialog : Check quest item.
//	sQuestItem* pQuestItem = NULL ;													// ����Ʈ ������ ������ ���� �����͸� �����ϰ� NULLó���� �Ѵ�.
//
//	PTRLISTSEARCHSTART(m_QuestItemList,sQuestItem*,pQuestItem)						// ����Ʈ ������ ����Ʈ�� �˻��Ѵ�.
//		if( pQuestItem->pQuestItemInfo->ItemIdx == pQuestItemInfo->ItemIdx )		// ����Ʈ ������ ������ ������ �ε�����, ���ڷ� �Ѿ�� ������ �ε����� ������,
//		{
//			pQuestItem->Count = Count ;												// ����Ʈ ������ ����Ʈ�� ������ ī��Ʈ�� �����Ѵ�.
//			return ;																// ���� ó���� �Ѵ�.
//		}
//	PTRLISTSEARCHEND																// ����Ʈ ������ ����Ʈ �˻��� �����Ѵ�.
//
//	sQuestItem* pSQuest = new sQuestItem;											// ����Ʈ ������ �޸𸮸� �Ҵ��Ѵ�.
//	pSQuest->pQuestItemInfo = pQuestItemInfo;										// ����Ʈ ������ ������ ���ڷ� �Ѿ�� ������ �����Ѵ�.
//	pSQuest->Count = Count;															// ����Ʈ �������� ī��Ʈ�� �����Ѵ�.
//	pSQuest->Pos = -1;																// ����Ʈ �������� ��ġ�� -1�� �����Ѵ�.
//
//	m_QuestItemList.AddTail(pSQuest);												// ����Ʈ ������ ������ ����Ʈ ������ ����Ʈ�� �߰��Ѵ�.
//	
//	//
//	cIcon* pIcon = new cIcon;														// ������ �޸𸮸� �Ҵ��Ѵ�.
//	cImage Image;																	// �̹����� �����Ѵ�.
//
//	SCRIPTMGR->GetImage(pSQuest->pQuestItemInfo->Image2DNum, &Image, PFT_ITEMPATH);	// ����Ʈ ������ ������ �̹����� �̹����� �����Ѵ�.
//	pIcon->SetBasicImage(&Image);													// �������� �⺻ �̹����� ������ ������ �̹����� �����Ѵ�.
//	pIcon->Init(0, 0, 40, 40, &Image, m_IndexGenerator.GenerateIndex() );			// �������� �ʱ�ȭ �Ѵ�.
//	pIcon->SetMovable(FALSE);														// �̵� ���θ� FALSE�� �����Ѵ�.
//	pIcon->SetActive(FALSE);														// Ȱ��ȭ ���θ� FALSE�� �����Ѵ�.
//
//	pSQuest->pIcon = pIcon;															// ����Ʈ ������ ������ �������� ������ ������ ���������� �����Ѵ�.
//	WINDOWMGR->AddWindow(pIcon);													// ������ �Ŵ����� ������ ������ �߰��Ѵ�.
//	//
//
//	++m_QuestItemCount;																// ����Ʈ ������ ī��Ʈ�� �����Ѵ�.
//
//	RefreshQuestItem(m_QuestItemPage);												// ����Ʈ ������ �������� ���ΰ�ħ�Ѵ�.
//}

//void CQuestDialog::QuestItemDelete(DWORD ItemIdx)									// ����Ʈ �������� �����ϴ� �Լ�.
//{
//	cIcon* pIcon = NULL;															// ������ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	sQuestItem* pSQuest = NULL;														// ����Ʈ ������ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//	
//	PTRLISTSEARCHSTART(m_QuestItemList, sQuestItem*, pSQuest);						// ����Ʈ ������ ����Ʈ���� �˻��� �����Ѵ�.
//		if(!pSQuest)			break;												// ����Ʈ ������ ������ ��ȿ���� ������, �˻��� ����������.
//
//		//
//		if(pSQuest->pQuestItemInfo->ItemIdx == ItemIdx)								// ����Ʈ ������ ������ �ε����� ���ڷ� �Ѿ�� �ε����� ������,
//		{
//			m_pIconGridDlg->DeleteIcon(pSQuest->Pos, &pIcon);						// ����Ʈ ������ �׸��忡�� �������� �����Ѵ�.
//
//			if(!pIcon)			break;												// ������ ������ ��ȿ���� ������ �˻��� ����������.
//
//			pIcon->SetActive(FALSE);												// �������� ��Ȱ��ȭ �Ѵ�.
//			WINDOWMGR->AddListDestroyWindow(pIcon);									// ������ �Ŵ����� �ı��� �����쿡 �������� �߰��Ѵ�.
//			m_IndexGenerator.ReleaseIndex(pIcon->GetID());							// �ε��� �����⿡�� ������ �ε����� �����Ѵ�.
//			m_QuestItemList.Remove(pSQuest);										// ����Ʈ ������ ����Ʈ���� ����Ʈ ������ ������ �����Ѵ�.
//			break;																	// �˻��� ����������.
//		}
//	PTRLISTSEARCHEND																// ����Ʈ ������ ����Ʈ �˻��� �����Ѵ�.
//
//	--m_QuestItemCount;																// ����Ʈ ������ ī��Ʈ�� �����Ѵ�.
//
//	RefreshQuestItem(m_QuestItemPage);												// ����Ʈ ������ �������� ���ΰ�ħ�Ѵ�.
//}

void CQuestDialog::QuestItemDelete(DWORD ItemIdx)									// ����Ʈ �������� �����ϴ� �Լ�.
{
	sQuestItem* pItem;

	PTRLISTPOS pos = NULL ;
	pos = m_QuestItemList.GetHeadPosition() ;

	while(pos)
	{
		pItem = NULL ;
		pItem = (sQuestItem*)m_QuestItemList.GetNext(pos) ;

		if( pItem )
		{
			if( pItem->pQuestItemInfo->ItemIdx == ItemIdx )
			{
				m_pIconGridDlg->DeleteIcon(pItem->Pos, &pItem->pIcon) ;

				pItem->pIcon->SetActive(FALSE);									
				WINDOWMGR->AddListDestroyWindow(pItem->pIcon);					
				m_IndexGenerator.ReleaseIndex(pItem->pIcon->GetID());			

				m_QuestItemList.Remove(pItem) ;

				delete pItem ;

				--m_QuestItemCount;
			}
		}
	}

	RefreshQuestItem(m_QuestItemPage);
}

DWORD CQuestDialog::QuestItemUpdate(DWORD type, DWORD ItemIdx, DWORD data)			// ����Ʈ �������� ������Ʈ �Ѵ�.
{
	sQuestItem* pSQuest = NULL;														// ����Ʈ ������ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.

	PTRLISTSEARCHSTART(m_QuestItemList, sQuestItem*, pSQuest);						// ����Ʈ ������ ����Ʈ���� ������ ������ �˻��Ѵ�.

		if(!pSQuest)			break;												// ����Ʈ ������ ������ ��ȿ���� ������, �˻��� ����������.

		if(pSQuest->pQuestItemInfo->ItemIdx == ItemIdx)								// ����Ʈ ������ ����Ʈ ���� ������ �ε�����, ���ڷ� ���� ������ �ε����� ������,
		{
			switch(type)															// ������Ʈ Ÿ���� Ȯ���Ѵ�.
			{
			case eQItem_AddCount:													// ����Ʈ ������ ī��Ʈ�� �߰��ϴ� Ÿ���̸�,
				if(pSQuest)		pSQuest->Count += data;								// ����Ʈ ������ ������ ��ȿ�ϸ�, ����Ʈ ������ ������ ī��Ʈ�� �����͸� ���Ѵ�.
				break;
			case eQItem_SetCount:													// ����Ʈ ������ ī��Ʈ�� �����ϴ� Ÿ���̸�,
				if(pSQuest)		pSQuest->Count = data;								// ����Ʈ ������ ������ ��ȿ�ϸ�, ����Ʈ ������ ������ ī��Ʈ�� ������ ������ �����Ѵ�.
				break;
			case eQItem_GetCount:													// ����Ʈ ������ ī��Ʈ�� ��ȯ�ϴ� Ÿ���̸�,
				if(pSQuest)		return pSQuest->Count;								// ����Ʈ ������ ������ ��ȿ�ϸ�, ����Ʈ ������ ������ ī��Ʈ�� ��ȯ�Ѵ�.
				break;
			}
			break;																	// �˻��� ����������.
		}
	PTRLISTSEARCHEND																// ����Ʈ ������ ����Ʈ �˻��� �����Ѵ�.
	
	return 0;																		// 0�� �����Ѵ�.
}

void CQuestDialog::RefreshRequitalItem()
{
	cIcon* pIcon = NULL;															// ������ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.

	int Count = 0;																	// ī��Ʈ ������ �����ϰ� 0���� �����Ѵ�.
	int Pos = 0;																	// ��ġ ������ ���� ������ �����ϰ� 0���� �����Ѵ�.

	RequitalItem* pSQuest = NULL;													// ����Ʈ ������ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.

	int	nStart = 0 ;																// ���� �ε����� �����Ѵ�.

	//
	for(int i=0; i<m_pRequitalGrid->GetCellNum(); i++)									// ������ �׸����� �� ���� ��ŭ for���� ������.
	{
		m_pRequitalGrid->DeleteIcon(i, &pIcon);										// ������ �׸��忡�� �������� �����Ѵ�.
	}														// ����Ʈ ������ ����Ʈ �˻��� �����Ѵ�.

	RequitalItem* pItem ;

	PTRLISTPOS pos = NULL ;
	pos = m_QuestRequitalList.GetHeadPosition() ;

	while(pos)
	{
		pItem = NULL ;
		pItem = (RequitalItem*)m_QuestRequitalList.GetNext(pos) ;

		if( pItem )
		{
			if( Count >= nStart && Count < 6 )
			{
				pItem->wPos = Pos ;
				m_pRequitalGrid->AddIcon( pItem->wPos, pItem->pIcon ) ;

				pItem->pIcon->SetActive(TRUE) ;

				++Pos ;
			}
			else
			{
				pItem->wPos = -1 ;
				pItem->pIcon->SetActive(FALSE) ;
			}

			++Count ;
		}
	}
}

//void CQuestDialog::RefreshRequitalItem()
//{
//	cIcon* pIcon = NULL;															// ������ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	int Count = 0;																	// ī��Ʈ ������ �����ϰ� 0���� �����Ѵ�.
//	int Pos = 0;																	// ��ġ ������ ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//	RequitalItem* pSQuest = NULL;													// ����Ʈ ������ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.
//
//	int	nStart = 0 ;																// ���� �ε����� �����Ѵ�.
//
//	//
//	for(int i=0; i<m_pRequitalGrid->GetCellNum(); i++)									// ������ �׸����� �� ���� ��ŭ for���� ������.
//	{
//		m_pRequitalGrid->DeleteIcon(i, &pIcon);										// ������ �׸��忡�� �������� �����Ѵ�.
//	}
//	
//	
//	PTRLISTSEARCHSTART(m_QuestRequitalList, RequitalItem*, pSQuest);				// ����Ʈ ������ ����Ʈ���� ����Ʈ ������ ������ �˻��Ѵ�.
//
//		if(!pSQuest)			break;												// ����Ʈ ������ ������ ��ȿ���� ������, �˻��� �������´�.
//
//		if(Count>=nStart && Count < 6)												// ���� �μ��� ī��Ʈ ������ �۰�, ������ �� ���� �μ��� ī��Ʈ ���� ������,
//		{
//			// Add
//			pSQuest->wPos = Pos;													// ����Ʈ ������ ������ ��ġ ������ �����Ѵ�.
//			m_pRequitalGrid->AddIcon(pSQuest->wPos, pSQuest->pIcon);				// ����Ʈ ������ ������ �׸��忡, ������ ������ �����Ѵ�.
//			pSQuest->pIcon->SetActive(TRUE);										// ����Ʈ ������ �������� Ȱ��ȭ �Ѵ�.
//			++Pos;																	// ��ġ ������ �����Ѵ�.
//		}
//		else																		// ���� �μ� ������ ���� ������,
//		{
//			pSQuest->wPos = -1;														// ����Ʈ ������ ��ġ ������ -1�� �����Ѵ�.
//			pSQuest->pIcon->SetActive(FALSE);										// ����Ʈ �������� �������� ��Ȱ��ȭ ��Ų��.
//		}
//
//		++Count;																	// ī��Ʈ ���� ������Ų��.
//
//	PTRLISTSEARCHEND																// ����Ʈ ������ ����Ʈ �˻��� �����Ѵ�.
//}

void CQuestDialog::RefreshQuestItem(DWORD Page)										// ����Ʈ ������ �������� ���ΰ�ħ�ϴ� �Լ�.
{	
	cIcon* pIcon = NULL;															// ������ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.

	int Count = 0;																	// ī��Ʈ ������ �����ϰ� 0���� �����Ѵ�.
	int Pos = 0;																	// ��ġ ������ ���� ������ �����ϰ� 0���� �����Ѵ�.

	sQuestItem* pSQuest = NULL;														// ����Ʈ ������ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.

	int	nStart = Page*VIEW_QUESTITEM_PERPAGE;										// ���� �ε����� �����Ѵ�.

	// PageButton PushState Setting
	for(int i=0; i<MAX_QUEST_PAGEBTN; ++i)											// ������ ī��Ʈ ��ŭ for���� ������.
	{
		m_pPageBtn[i]->SetPush(FALSE);												// ������ ��ư�� ������ ���� ���·� �����Ѵ�.
	}
	m_pPageBtn[Page]->SetPush(TRUE);												// ���ڷ� �Ѿ�� ������ ��ư�� ���� ���·� �����Ѵ�.

	//
	for(i=0; i<m_pIconGridDlg->GetCellNum(); i++)									// ������ �׸����� �� ���� ��ŭ for���� ������.
	{
		m_pIconGridDlg->DeleteIcon(i, &pIcon);										// ������ �׸��忡�� �������� �����Ѵ�.
	}
	
	
	PTRLISTSEARCHSTART(m_QuestItemList, sQuestItem*, pSQuest);						// ����Ʈ ������ ����Ʈ���� ����Ʈ ������ ������ �˻��Ѵ�.

		if(!pSQuest)			break;												// ����Ʈ ������ ������ ��ȿ���� ������, �˻��� �������´�.

		if(Count>=nStart && Count < nStart+VIEW_QUESTITEM_PERPAGE)					// ���� �μ��� ī��Ʈ ������ �۰�, ������ �� ���� �μ��� ī��Ʈ ���� ������,
		{
			// Add
			pSQuest->Pos = Pos;														// ����Ʈ ������ ������ ��ġ ������ �����Ѵ�.
			m_pIconGridDlg->AddIcon(pSQuest->Pos, pSQuest->pIcon);					// ����Ʈ ������ ������ �׸��忡, ������ ������ �����Ѵ�.
			pSQuest->pIcon->SetActive(TRUE);										// ����Ʈ ������ �������� Ȱ��ȭ �Ѵ�.
			++Pos;																	// ��ġ ������ �����Ѵ�.
		}
		else																		// ���� �μ� ������ ���� ������,
		{
			pSQuest->Pos = -1;														// ����Ʈ ������ ��ġ ������ -1�� �����Ѵ�.
			pSQuest->pIcon->SetActive(FALSE);										// ����Ʈ �������� �������� ��Ȱ��ȭ ��Ų��.
		}

		++Count;																	// ī��Ʈ ���� ������Ų��.

	PTRLISTSEARCHEND																// ����Ʈ ������ ����Ʈ �˻��� �����Ѵ�.

	m_QuestItemPage = Page;															// ����Ʈ ������ �������� �����Ѵ�.
}

void CQuestDialog::ShowRequitalItemToolTip(DWORD Index)
{
	RequitalItem* pSQuest = NULL;													// ����Ʈ ������ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.

	char buf[64] = { 0, };															// �ӽ� ���۸� �����Ѵ�.

	DWORD dwCount = 0;																// ī��Ʈ ������ �����ϰ� 0���� �����Ѵ�.

	PTRLISTSEARCHSTART(m_QuestRequitalList, RequitalItem*, pSQuest);				// ����Ʈ ������ ����Ʈ����, ����Ʈ ������ ������ �˻��Ѵ�.

		if(!pSQuest)			break;												// ����Ʈ ������ ������ ��ȿ���� ������, �˻��� ����������.

		if(dwCount == m_RequitalItemTooltipIdx)										// ���� �ε����� ī��Ʈ ���� ������,
		{
			pSQuest->pIcon->SetToolTip("");											// ����Ʈ ������ ������ ������ �������� �����Ѵ�.
		}

		if(dwCount == Index)														// ī��Ʈ ����, ���ڷ� �Ѿ�� �ε����� ������,
		{
			cImage Image;															// �̹����� �����Ѵ�.
			SCRIPTMGR->GetImage(0, &Image, PFT_HARDPATH);							// �̹����� �����Ѵ�.
			pSQuest->pIcon->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &Image, TTCLR_ITEM_CANEQUIP );	// ������ ������ �������� �����Ѵ�.

			wsprintf(buf, "[%s]", pSQuest->itemInfo.ItemName);				// �ӽ� ���ۿ� ������ �̸��� �����Ѵ�.
			pSQuest->pIcon->AddToolTipLine(buf, ITEMMGR->GetItemNameColor( pSQuest->itemInfo ));						// ����Ʈ �������� ���� ���ο� �ӽ� ���۸� �߰��Ѵ�.
			//pSQuest->pIcon->AddToolTipLine("");										// ����Ʈ �������� ���� ���ο� ������ �߰��Ѵ�.
			
			ITEMMGR->AddToolTip( pSQuest->pIcon, pSQuest->itemInfo.ItemTooltipIdx );

			//CItem* requitalItem = ITEMMGR->GetItem( pSQuest->itemInfo.ItemIdx );

			/*CItemShow* selectedItem = (CItemShow *)m_pIconGridDlg->GetIconForIdx( WORD( m_pIconGridDlg->GetCurSelCellPos() ) );

			ITEMMGR->SetToolTip((cIcon*)selectedItem, NULL, NULL, pSQuest->itemInfo.ItemIdx) ;*/

			//if( pTooltipStr )
			//{
			//	AddTooltipInfo(pTooltipStr->strToolTip, pSQuest->pIcon);	// ����Ʈ ������ ������ ������ �������� ������ �߰��Ѵ�.
			//}
			//else
			//{
			//	ASSERTMSG(0, "����Ʈ ���� �������� ������ ����!") ;
			//}
		}

		++dwCount;																	// ī��Ʈ ���� �����Ѵ�.

	PTRLISTSEARCHEND																// ����Ʈ ������ ����Ʈ �˻��� �����Ѵ�.
		
	m_RequitalItemTooltipIdx = Index;												// ���� �ε����� ���ڷ� �Ѿ�� �ε����� �����Ѵ�.
}

void CQuestDialog::ShowItemToolTip(DWORD Index)										// ������ ������ �����ִ� �Լ�.
{
	sQuestItem* pSQuest = NULL;														// ����Ʈ ������ ������ ���� �����͸� �����ϰ� nulló���� �Ѵ�.

	char buf[64] = { 0, };															// �ӽ� ���۸� �����Ѵ�.

	DWORD dwCount = 0;																// ī��Ʈ ������ �����ϰ� 0���� �����Ѵ�.

	PTRLISTSEARCHSTART(m_QuestItemList, sQuestItem*, pSQuest);						// ����Ʈ ������ ����Ʈ����, ����Ʈ ������ ������ �˻��Ѵ�.

		if(!pSQuest)			break;												// ����Ʈ ������ ������ ��ȿ���� ������, �˻��� ����������.

		if(dwCount == m_TooltipIdx)													// ���� �ε����� ī��Ʈ ���� ������,
		{
			pSQuest->pIcon->SetToolTip("");											// ����Ʈ ������ ������ ������ �������� �����Ѵ�.
		}

		if(dwCount == Index)														// ī��Ʈ ����, ���ڷ� �Ѿ�� �ε����� ������,
		{
			cImage Image;															// �̹����� �����Ѵ�.
			SCRIPTMGR->GetImage(0, &Image, PFT_HARDPATH);							// �̹����� �����Ѵ�.
			pSQuest->pIcon->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &Image, TTCLR_ITEM_CANEQUIP );	// ������ ������ �������� �����Ѵ�.

			wsprintf(buf, "[%s]", pSQuest->pQuestItemInfo->ItemName);				// �ӽ� ���ۿ� ������ �̸��� �����Ѵ�.
			pSQuest->pIcon->AddToolTipLine(buf, TTTC_DEFAULT);						// ����Ʈ �������� ���� ���ο� �ӽ� ���۸� �߰��Ѵ�.
			pSQuest->pIcon->AddToolTipLine("");										// ����Ʈ �������� ���� ���ο� ������ �߰��Ѵ�.
			AddTooltipInfo(pSQuest->pQuestItemInfo->ItemDesc, pSQuest->pIcon);		// ����Ʈ ������ ������ ������ �������� ������ �߰��Ѵ�.
		}

		++dwCount;																	// ī��Ʈ ���� �����Ѵ�.

	PTRLISTSEARCHEND																// ����Ʈ ������ ����Ʈ �˻��� �����Ѵ�.
		
	m_TooltipIdx = Index;															// ���� �ε����� ���ڷ� �Ѿ�� �ε����� �����Ѵ�.
}

void CQuestDialog::AddTooltipInfo(char* pString, cIcon* pIcon)						// ���� ������ �߰��ϴ� �Լ�.
{
	if(strlen(pString) > MAX_ITEMDESC_LINE)											// �ִ� ������ ���� ���κ��� ��Ʈ�� ���̰� ũ��,
	{
		char* pStr = pString;														// ���ڿ� �����͸� �����ϰ� ��Ʈ�� ������ �޴´�.
		char tb[MAX_ITEMDESC_LINE+2] = { 0,  };										// �ӽ� ���۸� �����Ѵ�.
		int len = 0;																// ���̸� 0���� �����Ѵ�.
		
		while(*pStr)																// ���ڿ� ������ ��ȿ�� ���� while���� ������.
		{						
			tb[len]=*pStr;															// �ӽ� ������ ���̿� ���ڸ� ��´�.

			if( pStr + 1 != CharNext( CharPrev( pStr, pStr + 1 ) ) )				// ���� ������ ���� ���ڸ� üũ�Ͽ� 2����Ʈ ���ڸ�, 
				tb[++len] = *(++pStr);												// �� ����Ʈ�� �� ��´�.

			++len;																	// ���� ���� �����Ѵ�.
			++pStr;																	// ���� �����͸� �����Ѵ�.
			
			// �ϴ��� �׳� ����.. ��.��;;
			if(tb[0] == ' ')		{	len = 0;	}								// �ӽ� ������ ù ����Ʈ�� ��ĭ�̸�, ���̸� 0���� �����Ѵ�.
			
			if(strlen(tb) >= MAX_ITEMDESC_LINE)										// �ӽ� ���� ���̰� �ִ� ������ ���� ���κ��� ũ��,
			{
				pIcon->AddToolTipLine(tb, TTTC_DESCRIPTION);						// �ӽ� ���۸� ���� ���ο� �߰��Ѵ�.
				memset(tb, 0, MAX_ITEMDESC_LINE+2);									// �ӽ� ���۸� �޸� ���Ѵ�.
				len = 0;															// ���̸� 0���� �����Ѵ�.
			}
		}

		if(len)																		// ���� ������ ������,
		{
			pIcon->AddToolTipLine(tb, TTTC_DESCRIPTION);							// �����ܿ� ������ �߰��Ѵ�.
			memset(tb, 0, MAX_ITEMDESC_LINE+2);										// �ӽ� ���۸� �޸� ���Ѵ�.
			len = 0;																// ���� ���� 0���� �����Ѵ�.
		}
	}
	else																			// �ִ� ������ ���� ���κ��� ��Ʈ�� ���̰� ������,
		pIcon->AddToolTipLine(pString, TTTC_DESCRIPTION);							// �����ܿ� ������ �߰��Ѵ�.
}

//void CQuestDialog::AddQuestTree(QuestTree* pData)
//{
//	if( !pData )
//	{
//		return ;
//	}
//
//	m_QuestTree.AddTail(pData) ;
//}

void CQuestDialog::AddQuestTree(int nMapNum)
{
	QuestTree* pData = new QuestTree;									// Ʈ���� �����Ѵ�.

	CQuestString* pQuestString = new CQuestString;						// Ʈ���� ���� ����Ʈ ��Ʈ���� �����Ѵ�.

	int nTitleLine = 0 ;												// Ÿ��Ʋ ������ ���� ������ �����ϰ� 0���� �����Ѵ�.
	pQuestString->AddLine(GetMapName(nMapNum), nTitleLine, TRUE) ;		// ����Ʈ ��Ʈ���� �� �̸��� Ÿ��Ʋ�� �߰��Ѵ�.
	pQuestString->SetIndex(nMapNum, 0);									// Ʈ�� �ε����� �� ��ȣ�� 0���� �����Ѵ�.
	pQuestString->SetTreeTitle(TRUE) ;									// Ʈ���� �������� �����Ѵ�.
	pData->list.AddHead(pQuestString);									// ���� ������ Ʈ���� �� Ÿ��Ʋ�� �߰��Ѵ�.
	++pData->Count;														// Ʈ���� ��Ʈ�� ī��Ʈ�� �����Ѵ�.
	pData->State = eQTree_Open ;										// Ʈ���� ���¸� ���� ���·� �����Ѵ�.
	pData->nMapNum = nMapNum ;											// Ʈ���� �� ��ȣ�� �����Ѵ�.

	m_QuestTree.AddTail(pData) ;
	++m_QTreeCount ;
}

void CQuestDialog::AddQuestTitle(CQuestString* pQuestString, int nMapNum)
{
	if( !pQuestString )
	{
		return ;
	}

	QuestTree* pTree ;

	PTRLISTPOS pos = NULL ;
	pos = m_QuestTree.GetHeadPosition() ;

	while(pos)
	{
		pTree = NULL ;
		pTree = (QuestTree*)m_QuestTree.GetNext(pos) ;

		if( pTree )
		{
			if(pTree->nMapNum == nMapNum)
			{
				CQuestString* pNewQuestString = new CQuestString ;

				DWORD dwMainQuestIdx = pQuestString->GetQuestID() ;
				DWORD dwSubQuestIdx  = pQuestString->GetSubID() ;

				pNewQuestString->SetQuestID( dwMainQuestIdx ) ;
				pNewQuestString->SetSubID( dwSubQuestIdx ) ;
				pNewQuestString->SetIndex( dwMainQuestIdx, dwSubQuestIdx ) ;

				pNewQuestString->SetComplete( pQuestString->IsComplete() ) ;

				pNewQuestString->SetCount( pQuestString->GetCount() ) ;
				pNewQuestString->SetTotalCount( pQuestString->GetTotalCount() ) ;


				ITEM* pItem = NULL ;

				cPtrList* titleList = NULL ;
				titleList = pQuestString->GetTitle() ;

				if( titleList )
				{
					PTRLISTPOS titlePos = NULL ;
					titlePos = titleList->GetHeadPosition() ;

					while(titlePos)
					{
						pItem = NULL ;
						pItem = (ITEM*)titleList->GetNext(titlePos) ;

						if( pItem )
						{
							ITEM* pNewItem = new ITEM ;

							//memcpy(pNewItem, pItem, sizeof(ITEM)) ;

							memcpy(pNewItem->string, pItem->string, sizeof(pItem->string)) ;
							pNewItem->line		= pItem->line ;
							pNewItem->nAlpha	= pItem->nAlpha ;
							pNewItem->nFontIdx	= pItem->nFontIdx ;
							pNewItem->rgb		= pItem->rgb ;

							pNewQuestString->GetTitle()->AddTail(pNewItem) ;
						}
					}
				}

				cPtrList* descList = NULL ;
				descList = pQuestString->GetDesc() ;

				if( descList )
				{
					PTRLISTPOS descPos = NULL ;
					descPos = descList->GetHeadPosition() ;

					while(descPos)
					{
						pItem = NULL ;
						pItem = (ITEM*)descList->GetNext(descPos) ;

						if( pItem )
						{
							ITEM* pNewItem = new ITEM ;

							//memcpy(pNewItem, pItem, sizeof(ITEM)) ;

							memcpy(pNewItem->string, pItem->string, sizeof(pItem->string)) ;
							pNewItem->line		= pItem->line ;
							pNewItem->nAlpha	= pItem->nAlpha ;
							pNewItem->nFontIdx	= pItem->nFontIdx ;
							pNewItem->rgb		= pItem->rgb ;

							pNewQuestString->GetDesc()->AddTail(pNewItem) ;
						}
					}
				}

				pNewQuestString->SetTreeTitle( FALSE ) ;

				pTree->list.AddTail(pNewQuestString) ;
				++pTree->Count;
			}
		}
	}
}






void CQuestDialog::StartQuest(DWORD dwQuestIdx)
{
	CQuestInfo* pQuestInfo =NULL ;														// ����Ʈ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pQuestInfo = QUESTMGR->GetQuestInfo(dwQuestIdx) ;									// ����Ʈ �ε����� ����Ʈ ������ �޴´�.

	if(!pQuestInfo)																		// ����Ʈ ������ ��ȿ���� ������,
	{
		/*char tempBuf[128] = {0, } ;
		sprintf(tempBuf, "[CheckMapTree] - (%d)�� ����Ʈ ������ ���� ���ؼ� ����Ʈ�� �߰����� ���߽��ϴ�.", dwQuestIdx) ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;
	}

	int nMapNum = pQuestInfo->GetMapNum() ;												// ����Ʈ ������ �ִ� �� ��ȣ�� �޴´�.

	QuestTree* pTree ;																	// ����Ʈ Ʈ�� ������ ���� �����͸� �����Ѵ�.
	
	PTRLISTPOS pos = NULL ;																// Ʈ���� ��ġ�� ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pos = m_QuestTree.GetHeadPosition() ;												// ����Ʈ Ʈ�� ����Ʈ�� ���� �����ϰ� ��ġ�� �޴´�.

	while(pos)																			// Ʈ�� ��ġ ������ ��ȿ�� ���� whlie���� ������.
	{
		pTree = NULL ;																	// Ʈ�� ������ ���� �����͸� null ó���� �Ѵ�.
		pTree = (QuestTree*)m_QuestTree.GetNext(pos) ;									// ���� ��ġ�� Ʈ�� ������ �ް�, ��ġ�� �������� �����Ѵ�.

		if( !pTree ) continue ;															// Ʈ�� ������ ��ȿ���� ������ continue ó���� �Ѵ�.

		if( pTree->nMapNum != nMapNum ) continue ;										// ���� Ʈ���� �� ��ȣ��, �񱳴���� �� ��ȣ�� �ٸ���, continue ó���� �Ѵ�.

		BOOL b_SameQuestIdx = FALSE ;													// Ʈ�� ������ ���� �ε����� �ִ��� ������� ���� ������ �����ϰ� false�� �����Ѵ�.

		DWORD* pQuestIdx ;																// ����Ʈ �ε��� ��ȣ�� ���� �����͸� �����Ѵ�.

		PTRLISTPOS idPos = NULL ;														// ����Ʈ �ε��� ��ġ�� ���� �����͸� �����ϰ� null ó���� �Ѵ�.
		idPos = pTree->list.GetHeadPosition() ;											// ���� Ʈ���� ����Ʈ�� ���� �����ϰ�, �ε��� ��ġ�� �޴´�.

		while(idPos)																	// �ε��� ��ġ ������ ��ȿ�� ���� while���� ������.
		{
			pQuestIdx = NULL ;															// ����Ʈ �ε����� ���� �����͸� null ó���� �Ѵ�.
			pQuestIdx = (DWORD*)pTree->list.GetNext(idPos) ;							// ���� ��ġ�� ����Ʈ �ε����� �ް�, ��ġ�� �������� �����Ѵ�.

			if( !pQuestIdx ) continue ;													// ����Ʈ �ε��� ������ ��ȿ���� ������ continue ó���� �Ѵ�.

			if( *pQuestIdx == dwQuestIdx )												// ����Ʈ �ε����� �� ����� ����Ʈ �ε����� ������,
			{
				b_SameQuestIdx = TRUE ;													// ��� ���� true�� �����Ѵ�.
				break ;																	// ���� while���� Ż���Ѵ�.
			}
		}

		if( b_SameQuestIdx )															// ��� ���� true �̸�,
		{
			/*char tempBuf[128] = {0, } ;
			sprintf(tempBuf, "[CheckMapTree] - (%d)�� �� Ʈ���� (%d)�� ����Ʈ�� �̹� �����ϱ� ������, ����Ʈ�� �߰����� ���߽��ϴ�.", nMapNum, dwQuestIdx) ;
			CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
			return ;
		}
		else																			// ���� ����Ʈ�� ���ٸ�,
		{
			DWORD* pNewQuest = new DWORD ;												// ����Ʈ �ε����� ���� ������ �����Ѵ�.
			*pNewQuest = dwQuestIdx ;													// ����Ʈ �ε��� ���� �����Ѵ�.

			pTree->list.AddTail(pNewQuest) ;											// ���� �� Ʈ���� ����Ʈ �ε����� �߰��Ѵ�.

			++m_nQuestCount ;

			//CQuestString* pQString = NULL ;
			//pQString = QUESTMGR->GetQuestString(dwQuestIdx, 0) ;

			//cPtrList* p = pQString->GetTitle();												// ����Ʈ �����͸� �����Ͽ�, ������ �޴´�.
			//ITEM* string = (ITEM*)p->GetHead();										// ����Ʈ�� ��带 ����Ʈ ��Ʈ�� �����Ϳ� �޴´�.

			//CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(221), string->string );			// ����Ʈ�� �����Ѵٴ� �޽����� ����Ѵ�.

			return ;
		}
	}

	AddMapTree(nMapNum, dwQuestIdx) ;													// ���ο� �� Ʈ���� �߰��ϰ�, ����Ʈ�� �߰��Ѵ�.

	RefreshQuestList() ;																// ����Ʈ ����Ʈ�� ���ΰ�ħ�Ѵ�.
}





void CQuestDialog::EndQuest(DWORD dwQuestIdx)
{
	CQuestInfo* pQuestInfo =NULL ;														// ����Ʈ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pQuestInfo = QUESTMGR->GetQuestInfo(dwQuestIdx) ;									// ����Ʈ �ε����� ����Ʈ ������ �޴´�.

	if(!pQuestInfo)																		// ����Ʈ ������ ��ȿ���� ������,
	{
		/*char tempBuf[128] = {0, } ;
		sprintf(tempBuf, "[CheckMapTree] - (%d)�� ����Ʈ ������ ���� ���ؼ� ����Ʈ�� �߰����� ���߽��ϴ�.", dwQuestIdx) ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;
	}

	int nMapNum = pQuestInfo->GetMapNum() ;												// ����Ʈ ������ �ִ� �� ��ȣ�� �޴´�.

	QuestTree* pTree ;																	// ����Ʈ Ʈ�� ������ ���� �����͸� �����Ѵ�.
	
	PTRLISTPOS pos = NULL ;																// Ʈ���� ��ġ�� ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pos = m_QuestTree.GetHeadPosition() ;												// ����Ʈ Ʈ�� ����Ʈ�� ���� �����ϰ� ��ġ�� �޴´�.

	while(pos)																			// Ʈ�� ��ġ ������ ��ȿ�� ���� whlie���� ������.
	{
		pTree = NULL ;																	// Ʈ�� ������ ���� �����͸� null ó���� �Ѵ�.
		pTree = (QuestTree*)m_QuestTree.GetNext(pos) ;									// ���� ��ġ�� Ʈ�� ������ �ް�, ��ġ�� �������� �����Ѵ�.

		if( !pTree ) continue ;															// Ʈ�� ������ ��ȿ���� ������ continue ó���� �Ѵ�.

		if( pTree->nMapNum != nMapNum ) continue ;										// ���� Ʈ���� �� ��ȣ��, �񱳴���� �� ��ȣ�� �ٸ���, continue ó���� �Ѵ�.

		DWORD* pQuestIdx ;																// ����Ʈ �ε��� ��ȣ�� ���� �����͸� �����Ѵ�.

		PTRLISTPOS idPos = NULL ;														// ����Ʈ �ε��� ��ġ�� ���� �����͸� �����ϰ� null ó���� �Ѵ�.
		idPos = pTree->list.GetHeadPosition() ;											// ���� Ʈ���� ����Ʈ�� ���� �����ϰ�, �ε��� ��ġ�� �޴´�.

		while(idPos)																	// �ε��� ��ġ ������ ��ȿ�� ���� while���� ������.
		{
			pQuestIdx = NULL ;															// ����Ʈ �ε����� ���� �����͸� null ó���� �Ѵ�.
			pQuestIdx = (DWORD*)pTree->list.GetNext(idPos) ;							// ���� ��ġ�� ����Ʈ �ε����� �ް�, ��ġ�� �������� �����Ѵ�.

			if( !pQuestIdx ) continue ;													// ����Ʈ �ε��� ������ ��ȿ���� ������ continue ó���� �Ѵ�.

			if( *pQuestIdx == dwQuestIdx )												// ����Ʈ �ε����� �� ����� ����Ʈ �ε����� ������,
			{
				pTree->list.Remove(pQuestIdx) ;
				delete pQuestIdx ;
			}
		}

		if( pTree->list.GetCount() < 1 ) DeleteMapTree(nMapNum) ;
	}

	RefreshQuestList() ;																// ����Ʈ ����Ʈ�� ���ΰ�ħ�Ѵ�.

	m_SelectedQuestID = GetQuestIdx(1) ;
	SetSelectedQuestIdx(m_SelectedQuestID) ;
}





void CQuestDialog::AddMapTree(int nMapNum, DWORD dwQuestIdx) 
{
	QuestTree* pNewQuestTree = new QuestTree ;
	pNewQuestTree->nMapNum = nMapNum ;
	pNewQuestTree->State = eQTree_Open ;

	DWORD* pNewQuest = new DWORD ;
	*pNewQuest = dwQuestIdx ;

	pNewQuestTree->list.AddTail(pNewQuest) ;

	m_QuestTree.AddTail(pNewQuestTree) ;
}





void CQuestDialog::DeleteMapTree(int nMapNum)
{
	QuestTree* pTree ;																	// ����Ʈ Ʈ�� ������ ���� �����͸� �����Ѵ�.
	
	PTRLISTPOS pos = NULL ;																// Ʈ���� ��ġ�� ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pos = m_QuestTree.GetHeadPosition() ;												// ����Ʈ Ʈ�� ����Ʈ�� ���� �����ϰ� ��ġ�� �޴´�.

	while(pos)																			// Ʈ�� ��ġ ������ ��ȿ�� ���� whlie���� ������.
	{
		pTree = NULL ;																	// Ʈ�� ������ ���� �����͸� null ó���� �Ѵ�.
		pTree = (QuestTree*)m_QuestTree.GetNext(pos) ;									// ���� ��ġ�� Ʈ�� ������ �ް�, ��ġ�� �������� �����Ѵ�.

		if( !pTree ) continue ;															// Ʈ�� ������ ��ȿ���� ������ continue ó���� �Ѵ�.

		if( pTree->nMapNum != nMapNum ) continue ;										// ���� Ʈ���� �� ��ȣ��, �񱳴���� �� ��ȣ�� �ٸ���, continue ó���� �Ѵ�.

		DWORD* pQuestIdx ;																// ����Ʈ �ε��� ��ȣ�� ���� �����͸� �����Ѵ�.

		PTRLISTPOS idPos = NULL ;														// ����Ʈ �ε��� ��ġ�� ���� �����͸� �����ϰ� null ó���� �Ѵ�.
		idPos = pTree->list.GetHeadPosition() ;											// ���� Ʈ���� ����Ʈ�� ���� �����ϰ�, �ε��� ��ġ�� �޴´�.

		while(idPos)																	// �ε��� ��ġ ������ ��ȿ�� ���� while���� ������.
		{
			pQuestIdx = NULL ;															// ����Ʈ �ε����� ���� �����͸� null ó���� �Ѵ�.
			pQuestIdx = (DWORD*)pTree->list.GetNext(idPos) ;							// ���� ��ġ�� ����Ʈ �ε����� �ް�, ��ġ�� �������� �����Ѵ�.

			if( !pQuestIdx ) continue ;													// ����Ʈ �ε��� ������ ��ȿ���� ������ continue ó���� �Ѵ�.

			pTree->list.Remove(pQuestIdx) ;

			delete pQuestIdx ;
		}

		pTree->list.RemoveAll() ;

		m_QuestTree.Remove(pTree) ;

		delete pTree ;
	}
}






void CQuestDialog::RefreshQuestList()
{
	char buf[256] = { 0, };																// �ӽ� ���۸� �����Ѵ�.	

	int BackupLine = m_pQuestListDlg->GetTopListItemIdx();							// ��� ������ �����Ѵ�.

	CQuestQuickViewDialog* pQuickView = NULL ;
	pQuickView = GAMEIN->GetQuestQuickViewDialog() ;

	if( !pQuickView )
	{
		/*sprintf(buf, "����Ʈ �˸�â ������ �޾ƿ��� ���� ����Ʈ ����Ʈ�� ���ΰ�ħ ���� ���߽��ϴ�.") ;
		CHATMGR->AddMsg(CTC_QUEST, buf) ;*/
		return ;
	}

	DWORD* pCheckIdx ;
	PTRLISTPOS checkPos ;																// ����Ʈ �˸�â�� ��ġ ������ ���� �����͸� �����Ѵ�.

	cPtrList* pCheckList = NULL ;
	pCheckList = pQuickView->GetViewList() ;

	if( !pCheckList )
	{
		/*sprintf(buf, "����Ʈ �˸�â�� ����Ʈ ������ �޾ƿ��� ���� ����Ʈ ����Ʈ�� ���ΰ�ħ ���� ���߽��ϴ�.") ;
		CHATMGR->AddMsg(CTC_QUEST, buf) ;*/
		return ;
	}

	m_pQuestListDlg->RemoveAll();														// ����Ʈ ����Ʈ ���̾�α׸� ��� ����.

	m_QListCount = 0 ;
	memset(m_dwQuestList, 0, sizeof(CQuestString*)*MAX_PROCESSQUEST);					// ����Ʈ ����Ʈ�� �޸� �� �Ѵ�.
	
	DWORD Color = 0;																	// ���� ���� ���� ������ �����ϰ� 0���� �����Ѵ�.

	QuestTree* pTree ;																	// ����Ʈ Ʈ�� ������ ���� �����͸� �����Ѵ�.
	PTRLISTPOS pos ;																	// Ʈ���� ��ġ�� ���� �����͸� �����Ѵ�.

	char* pMapName ;																	// �� �̸��� ���� �����͸� �����Ѵ�.

	DWORD* pQuestIdx ;																	// ����Ʈ �ε����� ���� �����͸� �����Ѵ�.
	CQuestInfo* pQuestInfo ;															// ����Ʈ ������ ���� �����͸� �����Ѵ�.

	PTRLISTPOS idPos ;																	// �ε��� ��ġ ������ ���� �����͸� �����Ѵ�.
	CQuestString* pQuestString ;														// ����Ʈ ��Ʈ�� ������ ���� �����͸� �����Ѵ�.
	
	pos = NULL ;																		// Ʈ���� ��ġ�� ���� �����͸� null ó���� �Ѵ�.
	pos = m_QuestTree.GetHeadPosition() ;												// ����Ʈ Ʈ�� ����Ʈ�� ���� �����ϰ� ��ġ�� �޴´�.

	while(pos)																			// Ʈ�� ��ġ ������ ��ȿ�� ���� whlie���� ������.
	{
		pTree = NULL ;																	// Ʈ�� ������ ���� �����͸� null ó���� �Ѵ�.
		pTree = (QuestTree*)m_QuestTree.GetNext(pos) ;									// ���� ��ġ�� Ʈ�� ������ �ް�, ��ġ�� �������� �����Ѵ�.

		if( !pTree ) continue ;															// Ʈ�� ������ ��ȿ���� ������ continue ó���� �Ѵ�.

		if( pTree->list.GetCount() < 1 )
		{
			/*sprintf(buf, "(%d)�� ���� ��ϵ� ����Ʈ�� ���, (%d)�� �� ����Ʈ�� �߰����� ���߽��ϴ�.", pTree->nMapNum) ;
			CHATMGR->AddMsg(CTC_QUEST, buf) ;*/
			continue ;
		}

		pMapName = NULL ;																// �� �̸��� ���� �����͸� null ó���� �Ѵ�.
		pMapName = GetMapName(pTree->nMapNum) ;											// �� ��ȣ�� ���� �� �̸��� �޴´�.	

		if( !pMapName )
		{
			/*sprintf(buf, "(%d)�� �� �̸��� �޾ƿ��� ���� (%d)�� �� ����Ʈ�� �߰����� ���߽��ϴ�.", pTree->nMapNum) ;
			CHATMGR->AddMsg(CTC_QUEST, buf) ;*/
			continue ;
		}

		Color = RGBA_MAKE(255, 150, 0, 255) ;											// �� Ʈ���� ������ �����Ѵ�.	
		sprintf(buf, "* ") ;															// �ӽ� ���۸� *�� �����Ѵ�.
		strcat(buf, pMapName) ;															// �ӽ� ���ۿ� �� �̸��� �߰��Ѵ�.

		m_pQuestListDlg->AddItemWithFont(buf, Color, cFont::FONT1) ;							// ����Ʈ ����Ʈ�� �� �̸��� �߰��Ѵ�.
		//m_pQuestListDlg->AddItem(buf, Color) ;											// ����Ʈ ����Ʈ�� �� �̸��� �߰��Ѵ�.

		m_dwQuestList[m_QListCount] = pTree->nMapNum ;									// ����Ʈ ����Ʈ�� ����Ʈ ��Ʈ�� ������ �����Ѵ�.
		++m_QListCount;																	// ����Ʈ ����Ʈ ī��Ʈ�� �����Ѵ�.

		if( pTree->State == eQTree_Close ) continue ;									// Ʈ���� ������ ���¶��, continue ó���� �Ѵ�.

		idPos = NULL ;																	// �ε��� ��ġ ������ ���� ������ null ó���� �Ѵ�.
		idPos = pTree->list.GetHeadPosition() ;											// ���� �� Ʈ���� ����Ʈ�� ���� �����ϰ� ��ġ�� �޴´�.

		while(idPos)																	// ��ġ ������ ��ȿ�� ���� while���� ������.
		{
			pQuestIdx = NULL ;															// ����Ʈ �ε����� ���� �����͸� null ó���� �Ѵ�.
			pQuestIdx = (DWORD*)pTree->list.GetNext(idPos) ;							// ���� ��ġ�� ����Ʈ �ε����� �ް�, ��ġ�� �������� �����Ѵ�.

			if( !pQuestIdx ) continue ;													// ����Ʈ �ε����� ��ȿ���� ������ continue ó���� �Ѵ�.

			memset(buf, 0, sizeof(buf)) ;												// �ӽ� ���۸� �޸� ���Ѵ�.

			pQuestString = NULL ;														// ����Ʈ ��Ʈ�� ������ ���� �����͸� null ó���� �Ѵ�.
			pQuestString = QUESTMGR->GetQuestString(*pQuestIdx, 0) ;					// ���� ����Ʈ�� ���� ��Ʈ���� �޾ƿ´�.

			if( !pQuestString )															// ����Ʈ ��Ʈ�� ������ ��ȿ���� ������,
			{
				/*sprintf(buf, "(%d)�� ����Ʈ ������ �޾ƿ��� ���� (%d)�� ����Ʈ ������ �߰����� ���߽��ϴ�.", pTree->nMapNum) ;
				CHATMGR->AddMsg(CTC_QUEST, buf) ;*/
				continue ;	
			}

			if( m_SelectedQuestID == pQuestString->GetQuestID() )
			{
				Color = RGBA_MAKE(255, 255, 0, 255) ;
			}
			else
			{
				Color = RGBA_MAKE(255, 255, 255, 255) ;
			}

			checkPos = NULL ;
			checkPos = pCheckList->GetHeadPosition() ;

			BOOL bResult = FALSE ;

			while(checkPos)
			{
				pCheckIdx = NULL ;
				pCheckIdx = (DWORD*)pCheckList->GetNext(checkPos) ;

				if( !pCheckIdx ) continue ;

				if( *pCheckIdx == pQuestString->GetQuestID() )
				{
					bResult = TRUE ;
					break ;
				}
			}

			if( bResult == FALSE )
			{
				sprintf(buf, "	- ");												// �ӽ� ���۸� -�� �����Ѵ�.
			}
			else
			{
				sprintf(buf, "	��");												// �ӽ� ���۸� -�� �����Ѵ�.
			}

			ITEM* pItem ;

			cPtrList* pTitleList = NULL ;
			pTitleList = pQuestString->GetTitle() ; 

			PTRLISTPOS titlePos = NULL ;
			titlePos = pTitleList->GetHeadPosition() ;

			while(titlePos)
			{
				pItem = NULL ;
				pItem = (ITEM*)pTitleList->GetNext(titlePos) ;

				if( !pItem ) continue ;

				strcat(buf, pItem->string) ;
			}

			CQuest* pQuest = QUESTMGR->GetQuest(pQuestString->GetQuestID()) ;

			int nSubQuestCount = 0 ;
			pQuestInfo = QUESTMGR->GetQuestInfo(pQuestString->GetQuestID()) ;				// ����Ʈ ��ȣ�� ����Ʈ ������ �޴´�.

			if( pQuestInfo )
			{
				nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// ���� ����Ʈ ���� �޴´�.
			}

			if( nSubQuestCount-1 == pQuest->GetCurSubQuestIdx() )
			{
				strcat(buf, CHATMGR->GetChatMsg(1463)) ;
			}

			m_pQuestListDlg->AddItemWithFont(buf, Color, cFont::FONT0) ;							// ����Ʈ ����Ʈ�� �� �̸��� �߰��Ѵ�.
			//m_pQuestListDlg->AddItem(buf, Color) ;											// ����Ʈ ����Ʈ�� �� �̸��� �߰��Ѵ�.

			m_dwQuestList[m_QListCount] = *pQuestIdx ;										// ����Ʈ ����Ʈ�� ����Ʈ ��Ʈ�� ������ �����Ѵ�.
			++m_QListCount;																	// ����Ʈ ����Ʈ ī��Ʈ�� �����Ѵ�.
		}
	}

	m_pQuestListDlg->ResetGuageBarPos() ;
	m_pQuestListDlg->SetTopListItemIdx(BackupLine);								// �������� ���� �ε����� �����Ѵ�.

	SetQuestCount() ;

	CQuestString* pCurString = NULL ;
	pCurString = QUESTMGR->GetQuestString(m_SelectedQuestID, 0) ;
	// 071012 LYW --- QuestDialog : Modified.
	//QuestDescView(pCurString) ;	// ����Ʈ�� ������, ���������� ǥ�� �ߴ� ������ ��������� null�� �߰��Ѵ�.
	if( m_nQuestCount <= 0 ) 
	{
		QuestDescView(NULL) ;
	}
	else
	{
		QuestDescView(pCurString) ;
	}
}



























