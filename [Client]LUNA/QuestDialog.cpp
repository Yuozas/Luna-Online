// QuestDialog.cpp: implementation of the CQuestDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"																	// 표준 시스템과, 프로젝트가 지정한, 자주쓰는 해더들을 모은 해더파일을 불러온다.

#include "WindowIDEnum.h"															// 윈도우 아이디 이넘 헤더를 불러온다.

#include "../Input/Mouse.h"															// 마우스 클래스 헤더를 불러온다.

#include "GameIn.h"

#include "ChatManager.h"															// 채팅 매니져 클래스 헤더를 불러온다.
#include "ObjectManager.h"															// 오브젝트 매니져 클래스 헤더를 불러온다.
#include "QuestManager.h"															// 퀘스트 매니져 클래스 헤더를 불러온다.

#include "./Interface/cScriptManager.h"												// 스크립트 매니져 클래스 헤더를 불러온다.
#include "./Interface/cWindowManager.h"												// 윈도우 매니져 클래스 헤더를 불러온다.

#include "../ItemManager.h"

#include "NpcScriptManager.h"														// npc 스크립트 매니져 헤더를 불러온다.

#include "./Interface/cFont.h"														// 폰트 클래스 헤더를 불러온다.
#include "./Interface/cIcon.h"														// 아이콘 클래스 헤더를 불러온다.
#include "./Interface/cIconGridDialog.h"											// 아이콘 그리드 다이얼로그 클래스 헤더를 불러온다.
#include "./Interface/cListDialog.h"												// 리스트 다이얼로그 클래스 헤더를 불러온다.
#include "./Interface/cPushupButton.h"												// 푸시업 버튼 클래스 헤더를 불러온다.
#include "./Interface/cStatic.h"													// 스태틱 클래스 헤더를 불러온다.

#include "cDialogueList.h"															// 다이얼로그 리스트 헤더를 불러온다.
#include "cMsgBox.h"																// 메시지 박스 클래스 헤더를 불러온다.
#include "cPage.h"																	// 페이지 클래스 헤더를 불러온다.
#include "QuestDialog.h"															// 퀘스트 다이얼로그 클래스 헤더를 불러온다.
#include "Quest.h"																	// 퀘스트 클래스 헤더를 불러온다.

#include "..\[CC]Quest\QuestExecute.h"												// 퀘스트 실행 클래스 헤더를 불러온다.
#include "..\[CC]Quest\QuestExecute_Item.h"											// 퀘스트 실행 아이템 클래스 헤더를 불러온다.
#include "..\[CC]Quest\QuestInfo.h"													// 퀘스트 정보 클래스 헤더를 불러온다.
#include "..\[CC]Quest\QuestNpcScript.h"											// 퀘스트 npc 스크립트 클래스 헤더를 불러온다.
#include "..\[CC]Quest\QuestNpcScriptData.h"										// 퀘스트 npc 스크립트 데이터 클래스 헤더를 불러온다.
#include "..\[CC]Quest\QuestString.h"												// 퀘스트 스트링 클래스 헤더를 불러온다.
#include "..\[CC]Quest\QuestTrigger.h"												// 퀘스트 트리거 클래스 헤더를 불러온다.
#include "..\[CC]Quest\SubQuestInfo.h"												// 서브 퀘스트 정보 클래스 헤더를 불러온다.


#include "QuestQuickViewDialog.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestDialog::CQuestDialog()														// 생성자 함수.
{
	m_type	= WT_QUESTDIALOG;														// 퀘스트 다이얼로그의 타입을 WT_QUESTDIALOG로 세팅한다.
	State	= eQView_QList;															// 다이얼로그 상태를 eQView_QList로 세팅한다.
	
	m_SelectedQuestID	= 0;														// 선택 된 퀘스트 아이디를 -1로 세팅한다.
	m_pMsgBox			= NULL;														// 메시지 박스 정보를 NULL처리 한다.
	m_QTreeCount		= 0;														// 퀘스트 트리 카운트를 0으로 세팅한다.
	m_QListCount		= 0;														// 퀘스트 리스트 카운트를 0으로 세팅한다.
	m_ChangedRow		= -1;														// 변경 된 줄 번호를 -1로 세팅한다.
	m_PreSelectedIdx	= 0;														// 선택 된 이전 인덱스를 0으로 세팅한다.
	m_ListLineIdx		= 0;														// 리스트 라인 인덱스를 0으로 세팅한다.
	m_QuestItemCount	= 0;														// 퀘스트 아이템 카운트를 0으로 세팅한다.
	m_QuestItemPage		= 0;														// 퀘스트 아이템 페이지를 0으로 세팅한다.
	m_TooltipIdx		= -1;														// 툴팁 인덱스를 -1로 세팅한다.
	m_RequitalItemTooltipIdx = -1 ;
	m_PreDescQuestString	= NULL;													// 이전 퀘스트 스트링 설명 정보를 NULL처리 한다.

	memset(m_QuestList, 0, sizeof(CQuestString*)*MAX_PROCESSQUEST);					// 퀘스트 리스트를 메모리 셋한다.

	m_IndexGenerator.Init( 999, IG_QUESTITEM_STARTINDEX);							// 퀘스트 아이템 시작 인덱스를 초기화 한다.

	m_pQuestCount		= NULL ;													// 퀘스트 보유수를 출력하는 스태틱 포인터를 NULL처리한다.

	m_pNoticeBtn		= NULL ;													// 퀘스트 알림이 버튼 포인터를 NULL 처리 한다.
	m_pGiveupBtn		= NULL ;													// 퀘스트 포기 버튼 포인터를 NULL처리 한다.

	m_pQuestDescList	= NULL ;													// 퀘스트 설명 리스트 다이얼로그 포인터를 NULL 처리 한다.

	m_pRequitalGrid		= NULL ;													// 퀘스트 보상 아이템 다이얼로그 포인터를 NULL 처리 한다.

	m_pRequitalGold		= NULL ;													// 골드 보상 스태틱 포인터를 NULL 처리 한다.
	m_pExperience		= NULL ;													// 경험치 보상 스태틱 포인터를 NULL 처리 한다.

	m_nQuestCount = 0 ;

	m_dwNpcIdx = 0 ;
	m_dwScriptPage = 0 ;
}

CQuestDialog::~CQuestDialog()														// 소멸자 함수.
{
	m_IndexGenerator.Release();														// 인덱스 생성기를 해제한다.

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

//CQuestDialog::~CQuestDialog()														// 소멸자 함수.
//{
//	m_IndexGenerator.Release();														// 인덱스 생성기를 해제한다.
//
//																					// 퀘스트 아이템을 제거한다
//	//{
//	//	PTRLISTPOS pos = m_QuestItemList.GetHeadPosition();							// 퀘스트 아이템 리스트를 헤드로 세팅한다.
//
//	//	sQuestItem* item;															// 퀘스트 아이템 정보를 받을 포인터를 선언한다.
//
//	//	while( item = ( sQuestItem* )m_QuestItemList.GetNext( pos ) )				// 퀘스트 아이템 정보가 유효할 동안 while문을 돌린다.
//	//	{
//	//		SAFE_DELETE( item );													// 퀘스트 아이템 정보를 안전하게 삭제한다.
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
//																					// 퀘스트 트리를 제거한다. 각 트리의 원소마다 트리가 또 달려있다. 함께 제거해주자
//	//{
//	//	PTRLISTPOS pos = m_QuestTree.GetHeadPosition();								// 퀘스트 트리 리스트를 헤드로 세팅한다.
//
//	//	QuestTree* tree;															// 퀘스트 트리 정보를 담을 포인터를 선언한다.
//
//	//	while( tree = ( QuestTree* )m_QuestTree.GetNext( pos ) )					// 퀘스트 트리 정보가 유효할 동안 while문을 돌린다.
//	//	{
//	//		PTRLISTPOS		innerPos = tree->list.GetHeadPosition();				// 퀘스트 트리의 리스트를 헤드로 세팅하고 위치 정보를 받는다.
//
//	//		CQuestString*	text;													// 퀘스트 스트링 정보를 받을 포인터를 선언한다.
//
//	//		while( text = ( CQuestString* )( tree->list.GetNext( innerPos ) ) )		// 퀘스트 스트링 정보가 유효할 동안 while문을 돌린다.
//	//		{
//	//																				/// 왜 인지 모르겠지만... GIVEUP_ID는 여기서 생성하지만, 다른 ID는 다른 곳에서 생성해서 넘긴다.
//	//			if( QUEST_GIVEUP_ID == text->GetSubID() )							// 서브 아이디가 퀘스트 포기 아이디와 같으면,
//	//			{
//	//				SAFE_DELETE( text );											// 스트링 정보를 안전하게 삭제한다.
//	//			}
//	//		}
//	//		
//	//		SAFE_DELETE( tree );													// 안전하게 트리 정보를 삭제한다.
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

void CQuestDialog::Linking()														// 하위 컨트롤들을 링크하는 함수.
{
	for(int i=0; i<MAX_QUEST_PAGEBTN; ++i)											// 최대 퀘스트 페이지 수 만큼 for문을 돌린다.
	{
		m_pPageBtn[i] = (cPushupButton*)GetWindowForID(QUE_PAGE1BTN+i);				// 퀘스트 아이템 페이지 정보를 링크한다.
	}

	m_pPageBtn[0]->SetPush(TRUE);													// 첫 페이지 버튼을 눌린 상태로 세팅한다.
	
//	m_pTitleBtn = (cButton*)GetWindowForID(QUE_TITLEBTN);
	m_pQuestListDlg = (cListDialog*)GetWindowForID(QUE_QUESTLIST);					// 퀘스트 리스트 정보를 링크한다.
	m_pIconGridDlg = (cIconGridDialog*)GetWindowForID(QUE_ITEM_GRID);				// 퀘스트 아이템의 아이콘 그리드 정보를 링크한다.

	m_pQuestCount = (cStatic*)GetWindowForID(QUE_COUNTSTATIC) ;						// 퀘스트 보유수를 출력하는 스태틱 정보를 링크한다.

	m_pNoticeBtn = (cButton*)GetWindowForID(QUE_NOTICEBTN) ;						// 퀘스트 알림이 버튼 정보를 링크한다.
	m_pGiveupBtn = (cButton*)GetWindowForID(QUE_GIVEUPBTN) ;						// 퀘스트 포기 버튼 정보를 링크한다.

	m_pQuestDescList	= (cListDialog*)GetWindowForID(QUE_DESCDLG) ;				// 퀘스트 설명 리스트 다이얼로그 정보를 링크한다.

	m_pRequitalGrid		= (cIconGridDialog*)GetWindowForID(QUE_REQUITAL_GRID) ;		// 퀘스트 보상 아이템 다이얼로그 정보를 링크한다.

	m_pRequitalGold		= (cStatic*)GetWindowForID(QUE_REQUITAL_GOLD) ;				// 골드 보상 스태틱 포인터를 정보를 링크한다.
	m_pExperience		= (cStatic*)GetWindowForID(QUE_REQUITAL_EXP) ;				// 경험치 보상 스태틱 포인터를 정보를 링크한다.
}

DWORD CQuestDialog::ActionEvent(CMouse* mouseInfo)										// 퀘스트 다이얼로그의 이벤트를 처리하는 함수.
{
	DWORD we = WE_NULL;																	// 이벤트를 담을 변수를 선언하고 NULL 처리를 한다.

	if( !m_bActive )																	// 퀘스트 다이얼로그가 열려있는 상태가 아니라면, 
	{
		return we;																		// 이벤트를 리턴한다.
	}

	we = cDialog::ActionEvent(mouseInfo);												// 다이얼로그의 이벤트를 받는다.

	if(m_pMsgBox)																		// 메시지 박스가 열려있으면,
	{
		return we;																		// 이벤트를 리턴한다.
	}

	int nMouseX = mouseInfo->GetMouseX() ;												// 이벤트가 발생하는 마우스 X좌표를 받는다.
	int nMouseY = mouseInfo->GetMouseY() ;												// 이벤트가 발생하는 마우스 Y좌표를 받는다.
    																					// Show QuestItem Tooltip
	if(m_pIconGridDlg->PtInWindow(nMouseX,nMouseY))										// 마우스가 아이콘 그리드 안에 있으면,
	{
		WORD Pos = -1;																	// 위치 변수를 선언하고 -1로 세팅한다.

		if(m_pIconGridDlg->GetPositionForXYRef(nMouseX,nMouseY,Pos))					// 아이콘 그리드 상에서 위치를 받는다. 위치가 유효하면,
		{
			WORD RealPos = Pos + m_QuestItemPage*VIEW_QUESTITEM_PERPAGE;				// 페이지를 참조하여 실제 위치를 받는다.

			if(m_TooltipIdx != RealPos)													// 툴팁 인덱스가 페이지와 같지 않으면,
			{
				ShowItemToolTip(RealPos);												// 현재 위치의 툴팁을 보여준다.
			}

			return 0;																	// 0을 리턴한다.
		}
	}

	if( m_pRequitalGrid->PtInWindow(nMouseX,nMouseY))
	{
		WORD Pos = -1;																	// 위치 변수를 선언하고 -1로 세팅한다.

		if(m_pRequitalGrid->GetPositionForXYRef(nMouseX,nMouseY,Pos))					// 아이콘 그리드 상에서 위치를 받는다. 위치가 유효하면,
		{
			WORD RealPos = Pos ;														// 페이지를 참조하여 실제 위치를 받는다.

			ShowRequitalItemToolTip(RealPos);											// 현재 위치의 툴팁을 보여준다.
			return 0;																	// 0을 리턴한다.
		}
	}

	if( m_pQuestListDlg->PtInWindow(nMouseX, nMouseY) )									// 마우스가 퀘스트 리스트 안에 있으면,
	{
		int nSelectedLine = -1 ;														// 리스트에서 선택 된 라인을 받을 변수를 선언하고 -1로 세팅한다.

		nSelectedLine = m_pQuestListDlg->GetCurSelectedRowIdx() ;						// 퀘스트 리스트에서 선택 된 라인을 받는다.

		m_ChangedRow = nSelectedLine ;													// 색상을 바꿀 라인번호를 세팅한다.

		ITEM* pItem = NULL ;
		pItem = m_pQuestListDlg->GetItem(nSelectedLine) ;

		if( !pItem ) return we ;

		//if( pItem->rgb == RGBA_MAKE(200, 128, 0, 255) )									// 선택 된 줄의 색상이 맵 트리 색상이면,
		if(pItem->nFontIdx == cFont::FONT1)
		{
			if( we & WE_LBTNDBLCLICK )													// 더블 클릭을 했다면,
			{
				QuestTreeStateCheck(nSelectedLine) ;
			}
		}
		else
		{
			if( nSelectedLine != -1 )													// 선택 된 라인이 -1과 같지 않으면,
			{
				DWORD QuestIdx = NULL ;
			
				QuestIdx = m_dwQuestList[nSelectedLine] ;								// 선택 된 라인의 퀘스트 스트링을 받는다.

				if( QuestIdx != 0xFFFFFFFF )											// 퀘스트 스트링 정보가 유효하면,
				{				
					if( we & WE_LBTNCLICK )												// 클릭을 했다면,
					{
						m_SelectedQuestID = QuestIdx ;									// 선택 된 퀘스트 인덱스를 세팅한다.

						RefreshQuestList() ;											// 퀘스트 리스트를 새로고침한다.

						CQuestString* pQuestString = NULL ;
						pQuestString = QUESTMGR->GetQuestString(QuestIdx, 0) ;

						QuestDescView(pQuestString);									// 퀘스트 설명을 보여준다.
					}
				}
			}
		}
	}

	return we;
}

//DWORD CQuestDialog::ActionEvent(CMouse* mouseInfo)										// 퀘스트 다이얼로그의 이벤트를 처리하는 함수.
//{
//	DWORD we = WE_NULL;																	// 이벤트를 담을 변수를 선언하고 NULL 처리를 한다.
//
//	if( !m_bActive )																	// 퀘스트 다이얼로그가 열려있는 상태가 아니라면, 
//	{
//		return we;																		// 이벤트를 리턴한다.
//	}
//
//	we = cDialog::ActionEvent(mouseInfo);												// 다이얼로그의 이벤트를 받는다.
//
//	if(m_pMsgBox)																		// 메시지 박스가 열려있으면,
//	{
//		return we;																		// 이벤트를 리턴한다.
//	}
//
//	int nMouseX = mouseInfo->GetMouseX() ;												// 이벤트가 발생하는 마우스 X좌표를 받는다.
//	int nMouseY = mouseInfo->GetMouseY() ;												// 이벤트가 발생하는 마우스 Y좌표를 받는다.
//    																					// Show QuestItem Tooltip
//	if(m_pIconGridDlg->PtInWindow(nMouseX,nMouseY))										// 마우스가 아이콘 그리드 안에 있으면,
//	{
//		WORD Pos = -1;																	// 위치 변수를 선언하고 -1로 세팅한다.
//
//		if(m_pIconGridDlg->GetPositionForXYRef(nMouseX,nMouseY,Pos))					// 아이콘 그리드 상에서 위치를 받는다. 위치가 유효하면,
//		{
//			WORD RealPos = Pos + m_QuestItemPage*VIEW_QUESTITEM_PERPAGE;				// 페이지를 참조하여 실제 위치를 받는다.
//
//			if(m_TooltipIdx != RealPos)													// 툴팁 인덱스가 페이지와 같지 않으면,
//			{
//				ShowItemToolTip(RealPos);												// 현재 위치의 툴팁을 보여준다.
//			}
//
//			return 0;																	// 0을 리턴한다.
//		}
//	}
//
//	if( m_pRequitalGrid->PtInWindow(nMouseX,nMouseY))
//	{
//		WORD Pos = -1;																	// 위치 변수를 선언하고 -1로 세팅한다.
//
//		if(m_pRequitalGrid->GetPositionForXYRef(nMouseX,nMouseY,Pos))					// 아이콘 그리드 상에서 위치를 받는다. 위치가 유효하면,
//		{
//			WORD RealPos = Pos ;														// 페이지를 참조하여 실제 위치를 받는다.
//
//			ShowRequitalItemToolTip(RealPos);										// 현재 위치의 툴팁을 보여준다.
//
//			//if(m_RequitalItemTooltipIdx == RealPos)										// 툴팁 인덱스가 페이지와 같지 않으면,
//			//{
//			//	ShowRequitalItemToolTip(RealPos);										// 현재 위치의 툴팁을 보여준다.
//			//}
//
//			return 0;																	// 0을 리턴한다.
//		}
//	}
//
//	if( m_pQuestListDlg->PtInWindow(nMouseX, nMouseY) )									// 마우스가 퀘스트 리스트 안에 있으면,
//	{
//		int nSelectedLine = -1 ;														// 리스트에서 선택 된 라인을 받을 변수를 선언하고 -1로 세팅한다.
//
//		nSelectedLine = m_pQuestListDlg->GetCurSelectedRowIdx() ;						// 퀘스트 리스트에서 선택 된 라인을 받는다.
//
//		m_ChangedRow = nSelectedLine ;													// 색상을 바꿀 라인번호를 세팅한다.
//
//		if( nSelectedLine != -1 )														// 선택 된 라인이 -1과 같지 않으면,
//		{
//			CQuestString* pQuestString = NULL ;											// 퀘스트 스트링을 받을 포인터를 선언하고 NULL 처리를 한다.
//		
//			pQuestString = m_QuestList[nSelectedLine] ;									// 선택 된 라인의 퀘스트 스트링을 받는다.
//
//			if( pQuestString )															// 퀘스트 스트링 정보가 유효하면,
//			{
//				if( pQuestString->IsTreeTitle() )										// 퀘스트 제목인지 체크한다. 맵 타이틀이라면,
//				{
//					if( we & WE_LBTNDBLCLICK )											// 더블 클릭을 했다면,
//					{
//						QuestTreeStateCheck(nSelectedLine) ;
//					}
//				}
//				else																	// 퀘스트 타이틀 이라면,
//				{
//					if( we & WE_LBTNCLICK )												// 클릭을 했다면,
//					{
//						m_SelectedQuestID = pQuestString->GetQuestID() ;				// 선택 된 퀘스트 인덱스를 세팅한다.
//
//						QuestListReset() ;												// 퀘스트 리스트를 새로고침한다.
//
//						QuestDescView(pQuestString);									// 퀘스트 설명을 보여준다.
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
//	// 스트링에서 퀘스트 인덱스를 받는다.
//	CQuestString* pQuestString = NULL ;													// 퀘스트 스트링을 받을 포인터를 선언하고 NULL 처리를 한다.
//		
//	pQuestString = m_QuestList[m_dwSeletedQuestIdx] ;									// 선택 된 라인의 퀘스트 스트링을 받는다.
//
//	DWORD QuestIdx = 0 ;
//
//	if( pQuestString )
//	{
//		QuestIdx = pQuestString->GetQuestID() ;
//	}
//
//	if( QUESTMGR->IsQuestComplete(QuestIdx) )											// 퀘스트 인덱스로 완료한 퀘스트인지 체크한다.
//	{
//		return;																			// 완료한 퀘스트라면, 리턴처리를 한다.
//	}
//
//	SEND_QUEST_IDX msg;																	// 퀘스트 정보를 담을 구조체를 선언한다.
//
//	msg.Category = MP_QUEST;															// 카테고리를 퀘스트로 세팅한다.
//	msg.Protocol = MP_QUEST_DELETE_SYN;													// 프로토콜을 퀘스트 삭세 싱크로 세팅한다.
//	msg.dwObjectID = HERO->GetID();														// HERO 아이디를 세팅한다.
//	msg.MainQuestIdx = (WORD)QuestIdx;													// 퀘스트 인덱스를 세팅한다.
//
//	NETWORK->Send(&msg, sizeof(msg));													// 세팅된 정보를 전송한다.
//
//	// 리스트에서 퀘스트 삭제 처리를 한다.
//
//	//test
//	//m_QuestUpdater->DeleteQuest(QuestIdx);															// 퀘스트 업데이터의 퀘스트 삭제함수를 호출한다.
//}

//DWORD CQuestDialog::ActionEvent(CMouse* mouseInfo)									// 이벤트를 처리하는 함수.
//{
//	DWORD we = WE_NULL;																// 리턴할 이벤트 변수를 NULL로 세팅한다.
//
//	if( !m_bActive )		return we;												// 퀘스트 다이얼로그가 열려있지 않으면, 리턴 처리를 한다.
//
//	we = cDialog::ActionEvent(mouseInfo);											// 퀘스트 다이얼로그에서 발생한 이벤트를 받는다.
//
//	if(m_pMsgBox)			return we;												// 메시지 박스가 활성화 되어 있다면, 리턴 처리를 한다.
//
//	int nMouseX = mouseInfo->GetMouseX() ;											// 마우스 X좌표를 받는다.
//	int nMouseY = mouseInfo->GetMouseY() ;											// 마우스 Y좌표를 받는다.
//																					// Show QuestItem Tooltip
//	if(m_pIconGridDlg->PtInWindow(nMouseX,nMouseY))									// 마우스가 아이콘 그리드 영역 안이라면,
//	{
//		WORD Pos = -1;																// 위치를 담는 변수를 -1로 세팅한다.(-값이 안들어가는데 ㅡㅡ;)
//
//		if(m_pIconGridDlg->GetPositionForXYRef(nMouseX,nMouseY,Pos))				// 아이콘 그리드의 몇번째 칸에 있는지 체크하여 성공하면,
//		{
//			WORD RealPos = Pos + m_QuestItemPage*VIEW_QUESTITEM_PERPAGE;			// 실제 위치를 페이지도 감안하여 세팅한다.
//
//			if(m_TooltipIdx != RealPos)												// 툴팁 인덱스와 실제 위치가 같지 않으면,
//			{
//				ShowItemToolTip(RealPos);											// 아이템 툴팁을 보여준다.
//			}
//
//			return 0;																// 리턴 처리를 한다.
//		}
//	}
//
//	if(m_pQuestListDlg->PtIdxInRow(nMouseX,nMouseY) == -1)							// 퀘스트 리스트에서 선택 된 것이 없으면,
//	{
//		m_ChangedRow = -1;															// 변경 된 줄 번호를 -1로 세팅한다.
//
//		if(m_PreSelectedIdx > -1)													// 선택된게 있을때 화면 밖으로 나가면 선택취소시켜준다.
//		{
//			if(State == eQView_QList)												// 퀘스트창 상태가 퀘스트 리스트 들을 보는 상태라면,
//			{
//				QuestListReset();													// 퀘스트 리스트를 새로고침 한다.
//			}
//			else if(State == eQView_QDesc)											// 퀘스트창 상태가 퀘스트 설명을 보는 상태라면,
//			{
//				QuestDescView(m_PreDescQuestString);								// 퀘스트 설명을 보여준다.
//			}
//
//			m_PreSelectedIdx = -1;													// 선택 된 인덱스는 -1로 세팅한다.
//		}
//		return 0;																	// 리턴 처리를 한다.
//	}
//	
//	int Idx = m_pQuestListDlg->GetSelectRowIdx() + m_pQuestListDlg->GetTopListItemIdx();	// 리스트에서 선택 된 줄과, 탑 리스트 아이템 인덱스로 인덱스를 얻는다.
//
//	m_ChangedRow = Idx;																// 변경된 줄 번호를 인덱스로 세팅한다.
//
//	if(State==eQView_QList)															// 퀘스트창 상태가 리스트를 보는 상태라면,
//	{	
//		if(we & WE_LBTNCLICK)														// 마우스 왼쪽 버튼을 눌렀다면,
//		{
//			QuestTreeStateCheck(Idx);												// 인덱스로 퀘스트 트리 상태를 체크한다.
//		}
//		else if(m_PreSelectedIdx != m_ChangedRow)									// 이전에 선택 된 인덱스가, 변경 된 줄 번호와 다르면,
//		{
//			QuestListReset();														// 퀘스트 리스트를 새로고침한다.
//			m_PreSelectedIdx = m_ChangedRow;										// 변경 된 줄 번호르 이전에 선택 된 인덱스를 세팅한다.
//		}
//	}
//	else if(State==eQView_QDesc)													// 퀘스트창 상태가 설명을 보고 있는 상태라면,
//	{
//		if(we & WE_LBTNCLICK && m_toPreList == m_ChangedRow)						// 왼쪽 버튼을 클릭 했고, 이전 리스트가 변경 된 줄 번호와 같으면,
//		{
//			QuestListReset();														// 퀘스트 리스트를 새로고침한다.
//		}
//		else if(m_PreSelectedIdx != m_ChangedRow)									// 이전에 선택 된 인덱스와 변경 된 줄 번호가 같지 않으면,
//		{
//			QuestDescView(m_PreDescQuestString);									// 퀘스트 설명을 보여준다.
//			m_PreSelectedIdx = m_ChangedRow;										// 이전 선택된 인덱스를 변경 된 줄 번호로 세팅한다.
//		}
//	}	
//
//	return we;																		// 이벤트를 리턴한다.
//}


void CQuestDialog::Render()															// 렌더 함수.
{
	cDialog::Render();																// 다이얼로그를 렌더한다.

	int Count = 0;																	// 카운트를 선언하고 0으로 세팅한다.
	sQuestItem* pSQuest = NULL;														// 퀘스트 아이템 포인터를 선언하고 NULL처리를 한다.
	int	nStart = m_QuestItemPage*VIEW_QUESTITEM_PERPAGE;							// 스타트 인덱스를 세팅한다.
	
	PTRLISTSEARCHSTART(m_QuestItemList, sQuestItem*, pSQuest);						// 퀘스트 아이템 리스트에서 퀘스트 아이템 정보를 찾는다.
		if(!pSQuest)				break;											// 퀘스트 아이템 정보가 유효하지 않으면, 빠져나간다.

		if(Count >= nStart)															// 카운트가 스타트 인덱스 이상이면,
		{
																					// 대충 위치 잡아서 찍는다.
			if(pSQuest->Count > 1)													// 퀘스트 아이템이 하나 이상이면,
			{
				static char nums[4];
				int absX=0;	int absY=0;												// 출력 위치 변수를 선언하고 0으로 세팅한다.

				wsprintf(nums,"%3d", pSQuest->Count);								// 세자리 수 까지 아이템 개수를 세팅한다.

				if(m_pIconGridDlg->GetCellAbsPos(pSQuest->Pos, absX, absY))			// 아이콘 그리드에서 위치 정보가 유효하면,
				{
					// 070209 LYW --- QuestDialog : Modified render part.
					m_renderRect.left	= (LONG)absX+16 ;							// 출력 위치를 세팅한다.
					m_renderRect.top	= (LONG)absY+24 ;
					m_renderRect.right	= 1 ;
					m_renderRect.bottom	= 1 ;
																					// 퀘스트 아이템 개수를 출력한다.
					CFONT_OBJ->RenderFontShadow(0, 1, nums,strlen(nums),&m_renderRect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100 ));
				}
			}
		}
	
		++Count;																	// 카운트를 증가시킨다.

		if(Count >= nStart+VIEW_QUESTITEM_PERPAGE)		break;						// 카운트가 페이지 당 보여지는 아이템 수를 넘어가면, 빠져나온다.

	PTRLISTSEARCHEND																// 리스트 검색을 마친다.


	int Count2 = 0;																	// 카운트를 선언하고 0으로 세팅한다.
	RequitalItem* pSQuest2 = NULL;													// 퀘스트 아이템 포인터를 선언하고 NULL처리를 한다.
	int	nStart2 = 0*MAX_REQUITAL_ICON_CELL;											// 스타트 인덱스를 세팅한다.

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
					m_renderRect.left	= (LONG)absX+16 ;							// 출력 위치를 세팅한다.
					m_renderRect.top	= (LONG)absY+24 ;
					m_renderRect.right	= 1 ;
					m_renderRect.bottom	= 1 ;
																					// 퀘스트 아이템 개수를 출력한다.
					CFONT_OBJ->RenderFontShadow(0, 1, nums,strlen(nums),&m_renderRect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100 ));
				}
			}
		}

		++Count2;																	// 카운트를 증가시킨다.

		if(Count2 >= nStart2 + MAX_REQUITAL_ICON_CELL)		break;					// 카운트가 페이지 당 보여지는 아이템 수를 넘어가면, 빠져나온다.
	PTRLISTSEARCHEND
}

// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃														 ┃
// ┃	QuestList											 ┃
// ┃														 ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
//void CQuestDialog::MainQuestAdd(CQuestString* pQuestString, int nMapNum)			// 메인 퀘스트 추가 함수.
//{
//	//if(!pQuestString)		return;													// 퀘스트 스트링 정보가 유효하지 않으면, 리턴 처리를 한다.
//
//	//int nLine = 0 ;
//
//	//QuestTree* pTree = NULL ;														// 트리 정보를 받을 포인터를 선언하고 NULL 처리를 한다.
//
//	//PTRLISTPOS pos = NULL ;															// 퀘스트 트리 리스트를 헤드로 세팅하고 위치 정보를 받는다.
//
//	//int nTreeCount = m_QuestTree.GetCount() ;										// 퀘스트 트리의 아이템 수를 구한다.
//
//	//BOOL bAdded = FALSE ;															// 추가 여부 변수를 FALSE로 세팅한다.
//
//	//for(int count =0 ; count < nTreeCount ; ++count )								// 트리 카운트 수 만큼 for문을 돌린다.
//	//{
//	//	pos = m_QuestTree.FindIndex(count) ;										// 카운트에 해당하는 위치 정보를 받는다.
//
//	//	if( pos )																	// 위치 정보가 유효한지 체크한다.
//	//	{
//	//		pTree = (QuestTree*)m_QuestTree.GetAt(pos) ;							// 위치에 해당하는 퀘스트 트리 정보를 받는다.
//
//	//		if( pTree )																// 퀘스트 트리 정보가 유효한지 체크한다.
//	//		{
//	//			if( pTree->nQuestMapNum == nMapNum )								// 퀘스트 트리의 맵 번호와, 인자로 넘어온 맵 번호가 같은지 체크한다.
//	//			{
//	//				bAdded = TRUE ;													// 추가 여부를 TRUE로 세팅한다.
//	//				
//	//				pQuestString->SetKey(pTree->Count) ;							// 스트링 인덱스를 세팅한다.
//	//				pTree->list.AddTail(pQuestString) ;								// 퀘스트 스트링을 추가한다.
//	//				++pTree->Count;													// 퀘스트 트리의 카운트를 증가시킨다.
//
//	//				break ;
//	//			}
//	//		}
//	//	}
//	//}
//
//	//if( !bAdded )																	// 퀘스트 스트링이 추가되지 않았으면,
//	//{
//	//	QuestTree* pData = new QuestTree;											// 퀘스트 트리 메모리를 할당한다.
//	//	pData->State = eQTree_Close;												// 퀘스트 트리를 닫혀진 상태로 세팅한다.
//	//	pData->QuestIdx = nTreeCount ;												// 퀘스트 인덱스를 세팅한다.
//	//	pQuestString->SetKey(1);													// 맵 이름 인덱스를 세팅한다.
//	//	pData->list.AddHead(pQuestString) ;											// 퀘스트 스트링을 추가한다.
//	//	++pData->Count;																// 퀘스트 트리의 카운트를 증가시킨다.
//
//	//	char* pMapName = GetMapName(nMapNum) ;										// 맵 번호에 따른 맵 이름을 받는다.	
//
//	//	CQuestString* pQSMapName = new CQuestString;								// 퀘스트 포기 메모리를 할당한다.
//	//	pQSMapName->SetIndex(0, 0);													// 맵 이름 인덱스를 세팅한다.
//	//	pQSMapName->SetKey(0);														// 맵 이름 인덱스를 세팅한다.
//	//	pQSMapName->AddLine(pMapName, nLine, TRUE) ;								// 스트링에 맵 이름을 세팅한다.
//
//	//	pData->list.AddHead(pQSMapName) ;											// 맵 이름을 리스트로 추가한다.
//	//	++pData->Count;																// 퀘스트 트리의 카운트를 증가시킨다.
//
//	//	pData->nQuestMapNum = nMapNum ;												// 맵 번호를 저장한다.
//
//	//	m_QuestTree.AddTail(pData);													// 퀘스트 트리 리스트에 퀘스트 트리를 추가한다.
//	//	++m_QTreeCount;																// 퀘스트 트리 카운트를 증가한다.
//	//}
//}

void CQuestDialog::SubQuestAdd(CQuestString* pQuestString)							// 서브 퀘스트 추가 함수.
{
	if(!pQuestString)		return;													// 퀘스트 스트링 정보가 유효하지 않으면, 리턴처리를 한다.

	QuestTree* pData = new QuestTree;												// 퀘스트 트리 메모리를 할당한다.
	pData->QuestIdx = pQuestString->GetQuestID();									// 퀘스트 인덱스를 세팅한다.
	pData->State = eQTree_Close;													// 퀘스트 트리 상태를 닫혀진 상태로 세팅한다.
	pData->list.AddHead(pQuestString);												// 퀘스트 스트링을 트리 리스트에 추가한다.
	++pData->Count;																	// 퀘스트 트리의 카운트를 증가한다.
	
	m_QuestTree.AddTail(pData);														// 퀘스트 트리 리스트에 트리 정보를 추가한다.
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

void CQuestDialog::ProcessQuestAdd(CQuestString* pQuestString, int nMapNum)			// 퀘스트 추가 프로세스.
{
	if(!pQuestString)		return;													// 퀘스트 정보가 유효하지 않으면, 리턴 처리를 한다.

	QuestTree* pData = NULL;														// 퀘스트 트리 정보를 담을 포인터를 선언하고 NULL처리 한다.
	CQuestString* pQuestGiveup = NULL;												// 퀘스트 포기 스트링 정보를 담을 포인터를 선언하고 NULL처리 한다.
	CQuestString* pQuestStringList = NULL;											// 퀘스트 스트링 정보를 담을 포인터를 선언하고 NULL처리 한다.

	BOOL bQuestAdd = FALSE;															// 퀘스트 추가 여부를 FALSE로 세팅한다.

																					// MainQuest Add
	if(pQuestString->GetSubID() == 0)												// 서브 퀘스트 아이디가 0과 같으면,
	{
		PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pData);							// 퀘스트 트리 리스트 검색을 시작한다.
			if(!pData)		break;													// 퀘스트 트리 정보가 유효하지 않으면, 검색을 빠져나간다.
			
																					// 메인퀘스트가 나중에 로딩됐을때
			if(pData->QuestIdx == pQuestString->GetQuestID())						// 퀘스트 인덱스가 스트링의 인덱스와 같을경우,
			{
				PTRLISTSEARCHSTART(pData->list, CQuestString*, pQuestStringList);	// 퀘스트 트리의 인덱스에서 검색을 시작한다.
					if(pQuestStringList->GetSubID() == 0)							// 퀘스트 스트링 리스트의 서브 퀘스트 아이기가 0과 같으면,
						return;														// 검색을 빠져나간다.
				PTRLISTSEARCHEND													// 검색을 종료한다.

				pData->State = eQTree_Close;										// 퀘스트 트리 상태를 닫혀진 상태로 세팅한다.
				pData->list.AddHead(pQuestString);									// 퀘스트 트리 정보에 퀘스트 스트링 정보를 추가한다.
				++pData->Count;														// 퀘스트 트리 정보의 카운트를 증가한다.
				
				// Quest Giveup Add
				pQuestGiveup = new CQuestString;									// 퀘스트 포기 관련 스트링 정보 메모리를 할당한다.
				pQuestGiveup->SetIndex(pQuestString->GetQuestID(), QUEST_GIVEUP_ID);// 퀘스트 보기의 인덱스를 세팅한다.
				pData->list.AddTail(pQuestGiveup);									// 퀘스트 트리 정보 리스트에 포기 스트링을 추가한다.
				++pData->Count;														// 퀘스트 트리 정보의 카운트를 증가한다.

				++m_QTreeCount;														// 퀘스트 트리 카운트를 증가한다.
				bQuestAdd = TRUE;													// 퀘스트 추가 여부를 TRUE로 세팅한다.
				break;																// 검색을 빠져나간다.	
			}

		PTRLISTSEARCHEND															// 검색을 종료 한다.

		//if(!bQuestAdd)																// 퀘스트 추가 여부가 FALSE라면,
		//{
		//	MainQuestAdd(pQuestString, nMapNum);									// 메인 퀘스트로 스트링을 추가한다.
		//}
	}
	// SubQuest Add
	else																			// 퀘스트 스트링에 서브 퀘스트 아이디가 있으면,
	{
		PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pData);							// 퀘스트 트리 검색을 시작한다.
			if(!pData)			break;												// 퀘스트 트리 정보가 유효하지 않으면, 검색을 빠져나간다.
			
			if(pData->QuestIdx == pQuestString->GetQuestID())						// 퀘스트 스트링의 퀘스트 아이디와, 퀘스트 트리의 퀘스트 인덱스가 같으면,
			{
				PTRLISTPOS pos = pData->list.GetTailPosition();						// 퀘스트 트리의 리스트의 마지막 위치 정보를 받는다.
				if(pos)																// 위치 정보가 유효하면,
				{
					CQuestString* pQString = (CQuestString*)pData->list.GetTail();	// 퀘스트 트리 리스트에서 스트링 정보를 받는다.
					if(pQString->GetSubID() == QUEST_GIVEUP_ID)						// 서브 아이디가 포기와 같으면,
					{
						pData->list.InsertBefore(pos, pQuestString);				// 퀘스트 트리 리스트에 퀘스트 스트링을 앞으로 집어 넣는다.
					}
					else															// 서브 아이디가 포기가 아니라면,
					{
						pData->list.AddTail(pQuestString);							// 퀘스트 트리 리스트에 뒤로 스트링을 추가한다.
					}

					++pData->Count;													// 퀘스트 트리의 카운트를 증가한다.
					bQuestAdd = TRUE;												// 퀘스트 추가 여부를 TRUE로 세팅한다.
				}				
				break;																// 검색을 빠져 나간다.
			}

		PTRLISTSEARCHEND															// 검색을 종료한다.

		if(!bQuestAdd)																// 퀘스트 추가 여부가 FALSE와 같으면,
		{
			SubQuestAdd(pQuestString);												// 서브 퀘스트를 추가한다.
		}
	}

	//
	QuestListReset();																// 퀘스트 리스트를 새로고침 한다.
}


// 끝난 퀘스트가 DB에 제대로 셋팅이 안되는 경우가 있음. 확인후 처리. - RaMa 04.10.16
void CQuestDialog::CheckQuestDate()													// 퀘스트 날짜를 체크하는 함수.
{	
	QuestTree* pQTree = NULL;														// 퀘스트 트리 정보를 받을 포인터를 선언하고 NULL 처리 한다.

	PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pQTree);							// 퀘스트 트리 리스트에서 검색을 한다.
		// 2개이라면.. MainQuest와 협행포기이다. 삭제...
		if(pQTree && pQTree->list.GetCount() == 2)									// 퀘스트 트리 정보가 유효하고, 리스트 개수가 2개이면,
		{	
																					// 서버로 보낸다.
			SEND_QUEST_IDX_SIMPLE msg;												// 메시지 구조체를 선언한다.
			msg.Category = MP_QUEST;												// 카테고리를 퀘스트로 세팅한다.
			msg.Protocol = MP_QUEST_DELETE_CONFIRM_SYN;								// 프로토콜을 퀘스트 삭제 컨펌 싱크로 세팅한다.
			msg.dwObjectID = HERO->GetID();											// HERO 아이디를 세팅한다.
			msg.MainQuestIdx = pQTree->QuestIdx;									// 메인 퀘스트 인덱스를 세팅한다.

			NETWORK->Send(&msg, sizeof(msg));										// 메시지를 전송한다.
		}
		// 혹시라도 MainQuest는 종료했는데 SubQuest가 남아 있을 경우. 지워준다.
		else if(pQTree)																// 퀘스트 트리 정보가 유효하고, 리스트 개수가 2개가 아니면,
		{
			CQuestString* pQuestString = (CQuestString*)pQTree->list.GetHead();		// 퀘스트 트리정보의 리스트에서 헤드 스트링을 받는다.

			if(pQuestString && pQuestString->GetSubID() != 0)						// 스트링 정보가 유효하고, 서브 아이디가 0이 아니면,
			{
				// 
				m_QuestTree.Remove(pQTree);											// 퀘스트 트리 리스트에서 트리 정보를 지운다.	
				delete pQTree;														// 퀘스트 트리 정보를 삭제한다.
				pQTree = NULL;														// 퀘스트 트리 정보를 받는 포인터를 NULL 처리 한다.

				// 아이템도 지워야 한다.
			}
		}
	PTRLISTSEARCHEND																// 검색을 종료한다.
		
	QuestListReset();																// 퀘스트 리스트를 새로고침한다.
}


void CQuestDialog::ProcessQuestDelete(CQuestString* pQuestString)					// 퀘스트 삭제 프로세스 함수.
{
	if(!pQuestString)		return;													// 스트링 정보가 유효하지 않으면, 리턴처리를 한다.

	QuestTree* pQTree = NULL;														// 퀘스트 트리 정보를 받을 포인터를 선언하고 NULL 처리를 한다.
	CQuestString* pData = NULL;														// 퀘스트 스트링 정보를 받을 포인터를 선언하고 NULL 처리를 한다.

	PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pQTree);							// 퀘스트 트리 리스트에서 검색을 시작한다.

		if(pQTree && pQTree->QuestIdx==pQuestString->GetQuestID())					// 퀘스트 트리 정보가 유효하고, 퀘스트 인덱스가 스트링의 퀘스트 아이디와 같으면,
		{
			PTRLISTSEARCHSTART(pQTree->list, CQuestString*, pData);					// 트리 정보의 리스트에서 검색을 시작한다.
				if(pData && pData==pQuestString)									// 트리 정보가 유효하고, 퀘스트 스트링 정보와 트리 정보가 같으면,
				{
					pQTree->list.Remove(pQuestString);								// 트리 정보에서 퀘스트 스트링 정보를 비운다.
					--pQTree->Count;												// 퀘스트 트리의 카운트를 감소한다.

																					// 모든 퀘스트가 삭제 되었을때
					if(pQTree->Count<=0 || pQuestString->GetSubID()==0)				// 퀘스트 카운트가 0이하, 퀘스트 스트링의 서브 아이디가 0과 같다면,
					{
						CompleteQuestDelete(pQuestString);							// 완벽하게 퀘스트 삭제를 한다.
					}
					
					break;															// 검색을 빠져 나간다.
				}
			PTRLISTSEARCHEND														// 검색을 종료한다.

			break;																	// 검색을 빠져나간다.
		}
	PTRLISTSEARCHEND																// 퀘스트 검색을 종료한다.

	//
	QuestListReset();																// 퀘스트 리스트를 새로 고침한다.
}

void CQuestDialog::CompleteQuestDelete(CQuestString* pQuestString)					// 완전히 퀘스트를 삭제하는 함수.
{
	if(!pQuestString)		return;													// 퀘스트 정보가 유효하지 않으면, 리턴 처리를 한다.
	
	QuestTree* pQTree = NULL;														// 퀘스트 트리 정보를 받을 포인터를 선언하고 NULL처리를 한다.
	
	PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pQTree);							// 퀘스트 트리 리스트에서 퀘스트 트리 정보를 검색한다.

		if(pQTree && pQTree->QuestIdx == pQuestString->GetQuestID())				// 퀘스트 트리 정보가 유효하고, 퀘스트 스트링의 퀘스트 아이디와, 트리 정보의 퀘스트 인덱스가 같으면,
		{
			pQTree->State = eQTree_NotUse;											// 퀘스트 상태를 사용하지 않음 상태로 세팅한다.
			pQTree->Count = 0;														// 퀘스트 트리의 카운트를 0으로 세팅한다.
			pQTree->list.RemoveAll();												// 퀘스트 트리 정보의 리스트를 모두 비운다.
			m_QuestTree.Remove(pQTree);												// 퀘스트 트리 리스트에서 퀘스트 트리 정보를 삭제한다.
			delete pQTree;															// 퀘스트 트리 정보를 삭제한다.
			--m_QTreeCount;															// 퀘스트 트리 카운트를 감소한다.
			break;																	// 검색을 빠져나간다.
		}	
	PTRLISTSEARCHEND																// 퀘스트 트리 리스트 검색을 종료한다.
		
	QuestListReset();																// 퀘스트 트리스를 새로고침 한다.
}

//void CQuestDialog::CompleteQuestDelete(CQuestString* pQuestString)					// 완전히 퀘스트를 삭제하는 함수.
//{
//	if(!pQuestString)		return;													// 퀘스트 정보가 유효하지 않으면, 리턴 처리를 한다.
//	
//	QuestTree* pQTree = NULL;														// 퀘스트 트리 정보를 받을 포인터를 선언하고 NULL처리를 한다.
//	
//	PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pQTree);							// 퀘스트 트리 리스트에서 퀘스트 트리 정보를 검색한다.
//
//		if(pQTree && pQTree->QuestIdx == pQuestString->GetQuestID())				// 퀘스트 트리 정보가 유효하고, 퀘스트 스트링의 퀘스트 아이디와, 트리 정보의 퀘스트 인덱스가 같으면,
//		{
//			pQTree->State = eQTree_NotUse;											// 퀘스트 상태를 사용하지 않음 상태로 세팅한다.
//			pQTree->Count = 0;														// 퀘스트 트리의 카운트를 0으로 세팅한다.
//			pQTree->list.RemoveAll();												// 퀘스트 트리 정보의 리스트를 모두 비운다.
//			m_QuestTree.Remove(pQTree);												// 퀘스트 트리 리스트에서 퀘스트 트리 정보를 삭제한다.
//			delete pQTree;															// 퀘스트 트리 정보를 삭제한다.
//			--m_QTreeCount;															// 퀘스트 트리 카운트를 감소한다.
//			break;																	// 검색을 빠져나간다.
//		}	
//	PTRLISTSEARCHEND																// 퀘스트 트리 리스트 검색을 종료한다.
//		
//	QuestListReset();																// 퀘스트 트리스를 새로고침 한다.
//}

//void CQuestDialog::QuestTreeStateCheck(int RowIdx)									// 퀘스트 트리 상태를 체크하는 함수.
//{
//	if(RowIdx<0)			return;													// 줄 인덱스가 0이상이면, 리턴 처리를 한다.
//
//	QuestTree* pQTree = NULL;														// 퀘스트 트리 정보를 받을 포인터를 선언하고, NULL처리를 한다.
//	CQuestString* pQuestString = m_QuestList[RowIdx];								// 퀘스트 스트링 정보를 받을 포인터를 선언하고, 
//
//	if(!pQuestString)			return;												// 스트링 정보가 유효하지 않으면 리턴 처리를 한다.
//
//	// 퀘스트 포기
//	
//	DWORD dwKey = pQuestString->GetKey() ;											// 키 값을 받는다.
//
//	if(pQuestString->IsTreeTitle())													// 선택한 트리가 맵 이름이면,
//	{
//		PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pQTree);						// 퀘스트 트리 리스트 검색을 한다.
//
//			if(pQTree && pQTree->nMapNum == pQuestString->GetQuestID())				// 트리 정보가 유효하고, 트리의 맵 번호와, 스트링의 맵 번호가 일치하면,
//			{
//				if(pQTree->State == eQTree_Open)									// 퀘스트 트리가 열린 상태라면,
//				{
//					pQTree->State = eQTree_Close;									// 트리 상태를 닫힘으로 세팅한다.
//				}
//				else if(pQTree->State == eQTree_Close)								// 퀘스트 트리가 닫혀있는 상태라면,
//				{
//					pQTree->State = eQTree_Open;									// 퀘스트 트리를 열림 상태로 세팅한다.
//				}
//
//				QuestListReset();													// 퀘스트 리스트를 새로고침한다.
//
//				break;																// 트리 리스트 검색을 빠져나간다.
//			}
//		PTRLISTSEARCHEND															// 트리 리스트 검색을 종료한다.
//
//		QuestListReset() ;
//	}																					
//	else																			// 퀘스트 스트링의 서브 아이디가 0과 같지 않으면,
//		QuestDescView(pQuestString);												// 퀘스트 상세 내용을 본다.
//}

void CQuestDialog::QuestTreeStateCheck(int RowIdx)									// 퀘스트 트리 상태를 체크하는 함수.
{
	if(RowIdx<0)			return;													// 줄 인덱스가 0이상이면, 리턴 처리를 한다.

	DWORD mapNum = m_dwQuestList[RowIdx];											// 퀘스트 스트링 정보를 받을 포인터를 선언하고, 

	QuestTree* pTree ;

	PTRLISTPOS pos = NULL ;
	pos = m_QuestTree.GetHeadPosition() ;

	while(pos)
	{
		pTree = NULL ;
		pTree = (QuestTree*)m_QuestTree.GetNext(pos) ;

		if( !pTree ) continue ;

		if( pTree->nMapNum != mapNum ) continue ;

		if(pTree->State == eQTree_Open)									// 퀘스트 트리가 열린 상태라면,
		{
			pTree->State = eQTree_Close;									// 트리 상태를 닫힘으로 세팅한다.
		}
		else if(pTree->State == eQTree_Close)								// 퀘스트 트리가 닫혀있는 상태라면,
		{
			pTree->State = eQTree_Open;									// 퀘스트 트리를 열림 상태로 세팅한다.
		}

		break;																// 트리 리스트 검색을 빠져나간다.
	}

	RefreshQuestList();													// 퀘스트 리스트를 새로고침한다.



	//if(!pQuestString)			return;												// 스트링 정보가 유효하지 않으면 리턴 처리를 한다.

	//// 퀘스트 포기
	//
	//DWORD dwKey = pQuestString->GetKey() ;											// 키 값을 받는다.

	//if(pQuestString->IsTreeTitle())													// 선택한 트리가 맵 이름이면,
	//{
	//	PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pQTree);						// 퀘스트 트리 리스트 검색을 한다.

	//		if(pQTree && pQTree->nMapNum == pQuestString->GetQuestID())				// 트리 정보가 유효하고, 트리의 맵 번호와, 스트링의 맵 번호가 일치하면,
	//		{
	//			if(pQTree->State == eQTree_Open)									// 퀘스트 트리가 열린 상태라면,
	//			{
	//				pQTree->State = eQTree_Close;									// 트리 상태를 닫힘으로 세팅한다.
	//			}
	//			else if(pQTree->State == eQTree_Close)								// 퀘스트 트리가 닫혀있는 상태라면,
	//			{
	//				pQTree->State = eQTree_Open;									// 퀘스트 트리를 열림 상태로 세팅한다.
	//			}

	//			QuestListReset();													// 퀘스트 리스트를 새로고침한다.

	//			break;																// 트리 리스트 검색을 빠져나간다.
	//		}
	//	PTRLISTSEARCHEND															// 트리 리스트 검색을 종료한다.

	//	QuestListReset() ;
	//}																					
	//else																			// 퀘스트 스트링의 서브 아이디가 0과 같지 않으면,
	//	QuestDescView(pQuestString);												// 퀘스트 상세 내용을 본다.
}


void CQuestDialog::QuestListReset()													// 퀘스트 리스트 새로고침 함수.
{
	//int BackupLine = m_pQuestListDlg->GetTopListItemIdx();							// 백업 라인을 받는다.
	////
	//m_pQuestListDlg->RemoveAll();													// 퀘스트 리스트 다이얼로그를 모두 비운다.

	//memset(m_QuestList, 0, sizeof(CQuestString*)*MAX_PROCESSQUEST);					// 퀘스트 리스트를 메모리 셋 한다.

	//m_QListCount=0;																	// 퀘스트 리스트 카운트를 0으로 세팅한다.
	//m_ListLineIdx = 0;																// 리스트 라인 인덱스를 0으로 세팅한다.

	////
	//QuestTree* pQTree ;																// 퀘스트 트리 정보를 받을 포인터를 선언한다.
	//CQuestString* pQuestString = NULL;												// 퀘스트 스트링 정보를 받을 포인터를 선언하고 NULL처리 한다.

	//PTRLISTPOS pos = NULL ;															// 위치 정보를 받을 포인터를 선언하고 null 처리를 한다.
	//pos = m_QuestTree.GetHeadPosition() ;											// 퀘스트 트리를 헤드 포지션으로 세팅하고 위치 정보를 받는다.

	//while(pos)																		// 위치정보가 유효할 동안 while문을 돌린다.
	//{
	//	pQTree = NULL ;																// 트리 정보를 받을 포인터를 null 처리를 한다.
	//	pQTree = (QuestTree*)m_QuestTree.GetNext(pos) ;								// 트리의 정보를 받고 위치를 다음으로 세팅한다.

	//	if( !pQTree ) continue ;													// 트리 정보가 유효하지 않으면 continue 처리를 한다.

	//	if( pQTree->list.GetCount() <= 1 )											// 퀘스트 트리의 아이템 수가 1나 이하면,
	//	{
	//		DeleteTree(pQTree) ;													// 퀘스트 트리를 삭제한다.

	//		QuestDescView(QuestTitleString(1)) ;									// 퀘스트 설명창을 새로 고침한다.
	//		m_pQuestDescList->ResetGuageBarPos() ;									// 퀘스트 설명창의 위치를 다시 세팅한다.
	//	}

	//	if(pQTree->State == eQTree_Close)											// 트리 상태가 닫혀있는 상태와 같으면,
	//	{
	//		pQuestString = NULL ;													// 퀘스트 스트링 정보를 받을 포인터를 null 처리를 한다.
	//		pQuestString = (CQuestString*)pQTree->list.GetHead();					// 헤드의 퀘스트 스트링 정보를 받는다.
	//		QuestListAdd(pQuestString);												// 퀘스트 리스트에 스트링을 추가한다.
	//	}
	//	else if(pQTree->State == eQTree_Open)										// 퀘스트가 열려있는 상태라면,
	//	{
	//		PTRLISTPOS openPos = NULL ;												// 트리의 리스트 위치 정보를 받을 포인터를 선언하고 null 처리를 한다.
	//		openPos = pQTree->list.GetHeadPosition() ;								// 퀘스트 트리 리스트의 헤드 위치를 받는다.

	//		while(openPos)															// 위치 정보가 유효할 동안 while문을 돌린다.
	//		{
	//			pQuestString = NULL ;												// 퀘스트 스트링 정보를 받을 포인터를 null 처리를 한다.
	//			pQuestString = (CQuestString*)pQTree->list.GetNext(openPos) ;		// 위치에 따른 퀘스트 스트링 정보를 받고, 리스트의 다음 위치를 받는다.

	//			if(!pQuestString) continue ;										// 퀘스트의 스트링 정보가 유효하지 않으면 continue 처리를 한다.

	//			QuestListAdd(pQuestString) ;										// 퀘스트 리스트에 스트링을 추가한다.
	//		}
	//	}	
	//}

	//m_ListLineIdx = -1;																// 리스트 라인 인덱스를 -1로 세팅한다.

	//m_pQuestListDlg->SetTopListItemIdx(BackupLine);									// 퀘스트 리스트 다이얼로그의 탑리스트 아이템 인덱스를 백업라인으로 세팅한다.

	//State = eQView_QList;															// 퀘스트 다이얼로그의 상태를 eQView_QList로 세팅한다.
}

//void CQuestDialog::QuestListReset()													// 퀘스트 리스트 새로고침 함수.
//{
//	//m_SelectedQuestID = 0 ;
//	int BackupLine = m_pQuestListDlg->GetTopListItemIdx();							// 백업 라인을 받는다.
//	//
//	m_pQuestListDlg->RemoveAll();													// 퀘스트 리스트 다이얼로그를 모두 비운다.
//
//	memset(m_QuestList, 0, sizeof(CQuestString*)*MAX_PROCESSQUEST);					// 퀘스트 리스트를 메모리 셋 한다.
//
//	m_QListCount=0;																	// 퀘스트 리스트 카운트를 0으로 세팅한다.
//	m_ListLineIdx = 0;																// 리스트 라인 인덱스를 0으로 세팅한다.
//
//	//
//	QuestTree* pQTree = NULL;														// 퀘스트 트리 정보를 받을 포인터를 선언하고 NULL처리 한다.
//	CQuestString* pQuestString = NULL;												// 퀘스트 스트링 정보를 받을 포인터를 선언하고 NULL처리 한다.
//
//	PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pQTree);							// 퀘스트 트리 리스트를 검색한다.
//
//		if( pQTree )																// 트리 정보가 유효한지 체크한다.
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
//			if(pQTree->State == eQTree_Close)										// 트리 상태가 닫혀있는 상태와 같으면,
//			{
//				pQuestString = (CQuestString*)pQTree->list.GetHead();				// 퀘스트 스트링 정보를 받는다.
//				QuestListAdd(pQuestString);											// 퀘스트 리스트에 스트링을 추가한다.
//			}
//			else if(pQTree->State == eQTree_Open)									// 퀘스트가 열려있는 상태라면,
//			{
//				PTRLISTSEARCHSTART(pQTree->list, CQuestString*, pQuestString);		// 트리 정보의 리스트 검색을 한다.
//					QuestListAdd(pQuestString, pQuestString->GetSubID());			// 퀘스트 스트링을 퀘스트 리스트에 추가한다.
//				PTRLISTSEARCHEND													// 트리 정보의 리스트 검색 종료.
//			}		
//		}
//	PTRLISTSEARCHEND																// 트리 리스트 검색을 종료한다.
//
//	m_ListLineIdx = -1;																// 리스트 라인 인덱스를 -1로 세팅한다.
//
//	//if(!m_pMsgBox)		m_SelectedQuestID = 0;										// 알림창이 열려있지 않으면, 선택 된 퀘스트 아이디를 -1로 세팅한다.
//
//	m_pQuestListDlg->SetTopListItemIdx(BackupLine);									// 퀘스트 리스트 다이얼로그의 탑리스트 아이템 인덱스를 백업라인으로 세팅한다.
//
//	State = eQView_QList;															// 퀘스트 다이얼로그의 상태를 eQView_QList로 세팅한다.
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

void CQuestDialog::QuestListAdd(CQuestString* pQuestString, int SubID)				// 퀘스트 리스트에 스트링을 추가하는 함수.
{	
	if(!pQuestString)			return;												// 퀘스트 스트링 정보가 유효하지 않으면, 리턴 처리를 한다.

	DWORD dwKey = pQuestString->GetKey() ;

	DWORD Color = 0;																// 색상 값을 담을 변수를 선언하고 0으로 세팅한다.

	ITEM* pQStr = NULL;															// 퀘스트 스트링 정보를 담을 포인터를 선언하고 null 처리 한다.

	char buf[256] = { 0, };															// 임시 버퍼를 선언한다.	

	PTRLISTPOS spos = pQuestString->GetTitle()->GetHeadPosition();					// 퀘스트 스트링의 헤드 위치를 받는다.

	while(spos)																		// 위치 정보가 유효할 동안 while문을 돌린다.
	{
		pQStr = (ITEM*)pQuestString->GetTitle()->GetNext(spos);					// 위치 정보에 해당하는 퀘스트 스트링 정보를 받는다.

		if(pQStr)																	// 스트링 정보가 유효하면,
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

			if(pQuestString->IsTreeTitle())										// 스트링이 트리 타이틀이라면,
			{
				sprintf(buf, "* ") ;												// 임시 버퍼를 *로 세팅한다.
			}
			else																	// 서브 아이디가 있으면,
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
					sprintf(buf, "	- ");												// 임시 버퍼를 -로 세팅한다.
				}
				else
				{
					sprintf(buf, "	√");												// 임시 버퍼를 -로 세팅한다.
				}
			}

			strcat(buf, pQStr->string);

			int nSubQuestCount = 0 ;
			CQuestInfo* pQuestInfo = QUESTMGR->GetQuestInfo(pQuestString->GetQuestID()) ;				// 퀘스트 번호로 퀘스트 정보를 받는다.

			if( pQuestInfo )
			{
				nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// 서브 퀘스트 수를 받는다.
			}

			if( pQuestString->IsComplete() || nSubQuestCount == 2 )
			{
				strcat(buf, CHATMGR->GetChatMsg(1463)) ;
			}

			m_pQuestListDlg->AddItem(buf, pQStr->rgb) ;
			++m_ListLineIdx;														// 리스트 라인 인덱스를 증가한다.
			memset(buf, 0, 256);													// 임시 버퍼를 메모리 셋 한다.
			
			// ListDialog 
			m_QuestList[m_QListCount] = pQuestString;								// 퀘스트 리스트에 퀘스트 스트링 정보를 대입한다.
			++m_QListCount;															// 퀘스트 리스트 카운트를 증가한다.
		}
	}
}

//void CQuestDialog::QuestListAdd(CQuestString* pQuestString, int SubID)				// 퀘스트 리스트에 스트링을 추가하는 함수.
//{	
//	if(!pQuestString)			return;												// 퀘스트 스트링 정보가 유효하지 않으면, 리턴 처리를 한다.
//
//	DWORD dwKey = pQuestString->GetKey() ;
//
//	DWORD Color = 0;																// 색상 값을 담을 변수를 선언하고 0으로 세팅한다.
//
//	QString* pQStr = NULL;															// 퀘스트 스트링 정보를 담을 포인터를 선언하고 null 처리 한다.
//
//	char buf[256] = { 0, };															// 임시 버퍼를 선언한다.	
//
//	PTRLISTPOS spos = pQuestString->GetTitle()->GetHeadPosition();					// 퀘스트 스트링의 헤드 위치를 받는다.
//
//	while(spos)																		// 위치 정보가 유효할 동안 while문을 돌린다.
//	{
//		pQStr = (QString*)pQuestString->GetTitle()->GetNext(spos);					// 위치 정보에 해당하는 퀘스트 스트링 정보를 받는다.
//
//		if(pQStr)																	// 스트링 정보가 유효하면,
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
//			//if(m_ListLineIdx == m_ChangedRow)										// 리스트 라인 인덱스가, 변경된 줄 번호와 같으면,
//			//{
//			//	if(pQuestString->IsTreeTitle())										// 스트링이 트리 타이틀이라면,
//			//	{
//			//		//Color=MAINQUEST_TITLE_COLOR_SEL ;								// 색상을 메인 퀘스트 색상으로 세팅한다.
//			//		Color= RGB(0, 64, 64) ;								// 색상을 메인 퀘스트 색상으로 세팅한다.
//			//	}
//			//	else																// 서브 아이디가 0이 아니면,
//			//	{
//			//		//Color=SUBQUEST_TITLE_COLOR_SEL;									// 색상을 서브 퀘스트 색상으로 세팅한다.
//			//		Color= RGB(255, 100, 0) ;									// 색상을 서브 퀘스트 색상으로 세팅한다.
//			//	}
//			//}
//			//else																	// 리스트 라인 인덱스가, 변경 된 줄 번호와 같지 않으면,
//			//{
//			//	if(pQuestString->IsTreeTitle())										// 스트링이 트리 타이틀이라면,
//			//	{
//			//		//Color=MAINQUEST_TITLE_COLOR ;									// 색상을 메인 퀘스트 색상으로 세팅한다.
//			//		Color= RGB(100, 50, 0) ;									// 색상을 메인 퀘스트 색상으로 세팅한다.
//			//	}
//			//	else																// 서브 아이디가 있으면,
//			//	{
//			//		//Color=SUBQUEST_TITLE_COLOR;										// 색상을 서브 퀘스트 색상으로 세팅한다.
//			//		Color= RGB(0, 100, 50) ;										// 색상을 서브 퀘스트 색상으로 세팅한다.
//			//	}
//			//}
//
//			if(pQuestString->IsTreeTitle())										// 스트링이 트리 타이틀이라면,
//			{
//				sprintf(buf, "* ") ;												// 임시 버퍼를 *로 세팅한다.
//			}
//			else																	// 서브 아이디가 있으면,
//			{
//				BOOL bResult = FALSE ;
//
//				for(int count = 0 ; count < 5 ; ++count )
//				{
//					if( QUESTMGR->GetQuestIDFromQuickView(count) == pQuestString->GetQuestID() )
//					{
//						bResult = TRUE ;
//						sprintf(buf, "	√");												// 임시 버퍼를 -로 세팅한다.
//						break ;
//					}
//				}
//
//				if( bResult == FALSE )
//				{
//					sprintf(buf, "	- ");												// 임시 버퍼를 -로 세팅한다.
//				}
//			}
//
//			strcat(buf, pQStr->Str);												// 퀘스트 스트링과 임시 버퍼 내용을 연결한다.
//			//strcat(buf, pQuestString->GetTitleStr());												// 퀘스트 스트링과 임시 버퍼 내용을 연결한다.
//
//			//DWORD dwSubID = pQuestString->GetSubID() ;
//			//DWORD dwSubID = 0 ;
//
//			//if(dwSubID)																// 퀘스트 스트링의 서브 아이디가 유효하면,
//			//{
//			//	CQuest* pQuest = QUESTMGR->GetQuest(dwSubID);						// 퀘스트 아이디로 퀘스트 정보를 받는다.
//			//	SUBQUEST* pSubQuest = pQuest->GetSubQuest(dwSubID);					// 서브 퀘스트 아이디로 서브 퀘스트 정보를 받는다.
//
//			//	if(pSubQuest->dwMaxCount)											// 서브 퀘스트의 최대 카운트가 하나 이상이면,
//			//	{
//			//		char tbuf[32] = { 0, };											// 임시 버퍼를 선언한다.
//			//		sprintf(tbuf, "  (%d/%d)", pSubQuest->dwData, pSubQuest->dwMaxCount);	// 퀘스트 데이터와 카운트를 임시 버퍼에 복사한다.
//			//		strcat(buf, tbuf);												// 임시 버퍼 끼리 연결한다.
//			//	}
//			//}
//
//			CQuestInfo* pQuestInfo = QUESTMGR->GetQuestInfo(pQuestString->GetQuestID()) ;				// 퀘스트 번호로 퀘스트 정보를 받는다.
//			int nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// 서브 퀘스트 수를 받는다.
//
//			if( pQuestString->IsComplete() || nSubQuestCount == 2 )
//			{
//				strcat(buf, "(완료)") ;
//			}
//
//			m_pQuestListDlg->AddItem(buf, Color);									// 퀘스트 리스트에 임시 버퍼를 추가한다.
//			++m_ListLineIdx;														// 리스트 라인 인덱스를 증가한다.
//			memset(buf, 0, 256);													// 임시 버퍼를 메모리 셋 한다.
//			
//			// ListDialog 
//			m_QuestList[m_QListCount] = pQuestString;								// 퀘스트 리스트에 퀘스트 스트링 정보를 대입한다.
//			++m_QListCount;															// 퀘스트 리스트 카운트를 증가한다.
//		}
//	}
//}


//void CQuestDialog::QuestListAdd(CQuestString* pQuestString, int SubID)				// 퀘스트 리스트에 스트링을 추가하는 함수.
//{	
//	if(!pQuestString)			return;												// 퀘스트 스트링 정보가 유효하지 않으면, 리턴 처리를 한다.
//
//	DWORD Color = 0;																// 색상 값을 담을 변수를 선언하고 0으로 세팅한다.
//
//	QString* pQStr = NULL;															// 퀘스트 스트링 정보를 담을 포인터를 선언하고 null 처리 한다.
//
//	char buf[256] = { 0, };															// 임시 버퍼를 선언한다.	
//
//																					// Quest Giveup Message
//	if(SubID == QUEST_GIVEUP_ID)													// 서브 아이디가 포기아이디와 같다면,
//	{
//		if(m_ListLineIdx==m_ChangedRow)												// 리스트 라인 인덱스가 변경된 줄 번호와 같으면,
//		{
//			Color=QUEST_GIVEUP_COLOR_SEL ;											// 색상을 퀘스트 포기 선택 색상으로 세팅한다.
//		}
//		else																		// 같지 않으면,
//		{
//			Color=QUEST_GIVEUP_COLOR;												// 퀘스트 포기 색상(일반)으로 세팅한다.
//		}
//
//		m_pQuestListDlg->AddItem(CHATMGR->GetChatMsg(232), Color);					// 퀘스트 리스트 다이얼로그에, < 퀘스트 포기 > 스트링을 추가한다.
//		++m_ListLineIdx;															// 리스트 라인 인덱스를 증가한다.
//		m_QuestList[m_QListCount] = pQuestString;									// 퀘스트 리스트에 스트링을 추가한다.
//		++m_QListCount;																// 리스트 카운트를 증가한다.
//
//		return;																		// 리턴 처리를 한다.
//	}
//
//	PTRLISTPOS spos = pQuestString->GetTitle()->GetHeadPosition();					// 퀘스트 스트링의 헤드 위치를 받는다.
//
//	while(spos)																		// 위치 정보가 유효할 동안 while문을 돌린다.
//	{
//		pQStr = (QString*)pQuestString->GetTitle()->GetNext(spos);					// 위치 정보에 해당하는 퀘스트 스트링 정보를 받는다.
//
//		if(pQStr)																	// 스트링 정보가 유효하면,
//		{
//			if(m_ListLineIdx == m_ChangedRow)										// 리스트 라인 인덱스가, 변경된 줄 번호와 같으면,
//			{
//				if(SubID==0)														// 서브 아이디가 0과 같으면,
//				{
//					Color=MAINQUEST_TITLE_COLOR_SEL ;								// 색상을 메인 퀘스트 색상으로 세팅한다.
//				}
//				else																// 서브 아이디가 0이 아니면,
//				{
//					Color=SUBQUEST_TITLE_COLOR_SEL;									// 색상을 서브 퀘스트 색상으로 세팅한다.
//				}
//			}
//			else																	// 리스트 라인 인덱스가, 변경 된 줄 번호와 같지 않으면,
//			{
//				if(SubID==0)														// 서브 아이디가 없으면,
//				{
//					Color=MAINQUEST_TITLE_COLOR ;									// 색상을 메인 퀘스트 색상으로 세팅한다.
//				}
//				else																// 서브 아이디가 있으면,
//				{
//					Color=SUBQUEST_TITLE_COLOR;										// 색상을 서브 퀘스트 색상으로 세팅한다.
//				}
//			}
//
//			if(SubID==0)															// 서브 아이디가 없으면, 
//			{
//				sprintf(buf, "* ") ;												// 임시 버퍼를 *로 세팅한다.
//			}
//			else																	// 서브 아이디가 있으면,
//			{
//				sprintf(buf, "	- ");												// 임시 버퍼를 -로 세팅한다.
//			}
//
//			strcat(buf, pQStr->Str);												// 퀘스트 스트링과 임시 버퍼 내용을 연결한다.
//
//			//DWORD dwSubID = pQuestString->GetSubID() ;
//			DWORD dwSubID = 0 ;
//
//			if(dwSubID)																// 퀘스트 스트링의 서브 아이디가 유효하면,
//			{
//				CQuest* pQuest = QUESTMGR->GetQuest(dwSubID);						// 퀘스트 아이디로 퀘스트 정보를 받는다.
//				SUBQUEST* pSubQuest = pQuest->GetSubQuest(dwSubID);					// 서브 퀘스트 아이디로 서브 퀘스트 정보를 받는다.
//
//				if(pSubQuest->dwMaxCount)											// 서브 퀘스트의 최대 카운트가 하나 이상이면,
//				{
//					char tbuf[32] = { 0, };											// 임시 버퍼를 선언한다.
//					sprintf(tbuf, "  (%d/%d)", pSubQuest->dwData, pSubQuest->dwMaxCount);	// 퀘스트 데이터와 카운트를 임시 버퍼에 복사한다.
//					strcat(buf, tbuf);												// 임시 버퍼 끼리 연결한다.
//				}
//			}
//
//			m_pQuestListDlg->AddItem(buf, Color);									// 퀘스트 리스트에 임시 버퍼를 추가한다.
//			++m_ListLineIdx;														// 리스트 라인 인덱스를 증가한다.
//			memset(buf, 0, 256);													// 임시 버퍼를 메모리 셋 한다.
//			
//			// ListDialog 
//			m_QuestList[m_QListCount] = pQuestString;								// 퀘스트 리스트에 퀘스트 스트링 정보를 대입한다.
//			++m_QListCount;															// 퀘스트 리스트 카운트를 증가한다.
//		}
//	}
//}

//void CQuestDialog::QuestDescView(CQuestString* pQuestString)						// 퀘스트 상세 설명 보기 함수.
//{
//	if(!pQuestString)			return;												// 퀘스트 스트링 정보가 유효하지 않으면, 리턴 처리를 한다.
//	
//	int BackupLine = m_pQuestListDlg->GetTopListItemIdx();							// 백업 라인을 세팅한다.
//
//	m_pQuestDescList->RemoveAll();													// 퀘스트 리스트 다이얼로그를 모두 비운다.
//
//	memset(m_QuestList, 0, sizeof(CQuestString*)*MAX_PROCESSQUEST);					// 퀘스트 리스트를 메모리 셋 한다.
//
//	m_QListCount=0;																	// 퀘스트 리스트 카운트를 0으로 세팅한다.
//	m_toPreList= -1;																// 이전 리스트를 -1로 세팅한다.
//
//	QString* pQStr=NULL;															// 퀘스트 스트링 정보를 받을 변수를 선언하고 null처리 한다.
//
//	PTRLISTPOS pos = NULL;															// 위치 정보를 받을 포인터 변수를 선언하고 null처리 한다.
//
//	// Title
//	pos = pQuestString->GetTitle()->GetHeadPosition();								// 퀘스트 스트링을 해드 위치로 세팅한다.
//
//	while(pos)																		// 위치 정보가 유효할 동안 while문을 돌린다.
//	{
//		pQStr = (QString*)pQuestString->GetTitle()->GetNext(pos);					// 위치 정보에 해당하는 퀘스트 스트링 정보를 받는다.
//
//		if(pQStr)																	// 퀘스트 스트링 정보가 유효하다면,
//		{
//			m_pQuestDescList->AddItem(pQStr->Str, SUBQUEST_TITLE_COLOR_SEL);		// 퀘스트 리스트 다이얼로그에 추가한다.
//			++m_toPreList;															// 이전 리스트 값을 증가한다.
//		}
//	}
//	m_pQuestDescList->AddItem(" ", 0);												// 퀘스트 리스트에 빈칸을 추가한다.
//	++m_toPreList;																	// 이전 리스트 값을 증가한다.
//
//	// 의뢰자를 추가한다.
//	m_pQuestDescList->AddItem("의뢰자 - 리엔", SUBQUEST_TITLE_COLOR_SEL);	
//	// 위치를 추가한다.
//	m_pQuestDescList->AddItem("위치 - 알케르 평원입구", SUBQUEST_TITLE_COLOR_SEL);	
//
//	m_pQuestDescList->AddItem(" ", 0);												// 퀘스트 리스트에 빈칸을 추가한다.
//
//	// <의뢰내용>을 추가한다.
//
//	// Desc	
//	int preline = -1;																// 이전 라인을 -1로 세팅한다.
//	pos = pQuestString->GetDesc()->GetHeadPosition();								// 퀘스트 스트링을 헤드 포지션으로 세팅한다.
//
//	while(pos)																		// 위치 정보가 유효할 동안 while문을 돌린다.
//	{
//		pQStr = (QString*)pQuestString->GetDesc()->GetNext(pos);					// 퀘스트 스트링 정보를 받는다.
//
//		if(pQStr)																	// 퀘스트 스트링 정보가 유효하면,
//		{
//			m_pQuestDescList->AddItem(pQStr->Str, pQStr->Color, pQStr->Line);		// 퀘스트 리스트 다이얼로그에 아이템을 추가한다.
//
//																					// 라인이 바뀔때만 이전화면 라인을 셋팅.
//			if(preline != pQStr->Line)												// 이전 라인과 퀘스트 스트링 라인이 같으면, 
//			{
//				++m_toPreList;														// 이전 리스트 값을 증가시킨다.
//				preline = pQStr->Line;												// 이전 라인을 퀘스트 스트링의 라인으로 세팅한다.
//			}
//		}
//	}
//
//	DWORD Color = 0;																// 색상값을 담을 변수를 선언하고 0으로 세팅한다.
//	m_toPreList+=2;																	// 이전 리스트  2기를 더 증가한다.
//
//	if(m_ChangedRow==m_toPreList)													// 변경 된 줄 번호와 이전 리스트 값이 같으면,
//	{
//		Color=SELECT_COLOR_SEL ;													// 색상을 선택 된 색상으로  세팅한다.
//	}
//	else																			// 그렇지 않으면,
//	{
//		Color=SELECT_COLOR;															// 색상을 일반 색상으로 세팅한다.
//	}
//
//	// Back
//	//m_pQuestListDlg->AddItem(" ", 0);												// 퀘스트 리스트 다이얼로그에 빈칸 아이템을 추가한다.
//	//// 070125 LYW --- Quest : Modified this line.
//	////m_pQuestListDlg->AddItem(CHATMGR->GetChatMsg(601), Color);
//	//m_pQuestListDlg->AddItem(CHATMGR->GetChatMsg(229), Color);						// 퀘스트 리스트 다이얼로그에  < 이전 화면 >을 출력한다.
//
//	// 현재 진행과정(2/5) 추가해야 한다.
//
//	m_PreDescQuestString = pQuestString;											// 이전 설명 퀘스트 스트링을 받는다.
//	m_pQuestDescList->SetTopListItemIdx(BackupLine);								// 아이템의 툴팁 인덱스를 세팅한다.
//	State = eQView_QDesc;															// 퀘스트 창 상태를 eQView_QDesc로 세팅한다.
//}

void CQuestDialog::UpdateQuestDescView(DWORD dwQuestIdx, DWORD dwSubQuestIdx)		// 퀘스트 설명창을 업데이트 하는 함수.
{
	int nQuestStringKey = 0 ;														// 퀘스트 스트링 키 값을 담을 변수를 선언하고 0으로 세팅한다.

	COMBINEKEY(dwQuestIdx, dwSubQuestIdx, nQuestStringKey) ;						// 메인 퀘스트, 서브 퀘스트 인덱스로, 스크링 키 값을 받는다.

	CQuestString* pQuestString = NULL ;												// 퀘스트 스트링을 받을 포인터를 선언하고 NULL 처리를 한다.
		
	pQuestString = QUESTMGR->GetQuestString(dwQuestIdx, dwSubQuestIdx);				// 퀘스트 매니져에서 키 값으로 퀘스트 스트링을 받아온다.

	if( pQuestString )
	{
		QuestDescView(pQuestString) ;												// 퀘스트 설명창을 업데이트 한다.
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
							PTRLISTPOS spos = pQuestString->GetTitle()->GetHeadPosition();					// 퀘스트 스트링의 헤드 위치를 받는다.

							m_pQuestListDlg->RemoveItem(count) ;

							char tempBuf[256] = {0, } ;

							strcat(tempBuf, " - ") ;

							while(spos)																		// 위치 정보가 유효할 동안 while문을 돌린다.
							{
								ITEM* pQStr = NULL;															// 퀘스트 스트링 정보를 담을 포인터를 선언하고 null 처리 한다.
								pQStr = (ITEM*)pQuestString->GetTitle()->GetNext(spos);					// 위치 정보에 해당하는 퀘스트 스트링 정보를 받는다.

								if(pQStr)																	// 스트링 정보가 유효하면,
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
		/*sprintf(tempBuf, "[SetQuestPrevInfo] - 퀘스트 스트링 정보가 유효하지 않아, 퀘스트 기본 정보를 세팅하지 못했습니다.") ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;
	}

	DWORD dwQuestIdx = pQuestString->GetQuestID() ;

	// - 퀘스트 제목 세팅 - 
	CQuestString* pTitleString = NULL ;												// 퀘스트 제목 스트링 정보를 받을 포인터를 선언하고 null처리 한다.
	pTitleString = QUESTMGR->GetQuestString(dwQuestIdx, 0) ;						// 퀘스트 제목 스트링을 받는다.

	if( !pTitleString )
	{
		/*sprintf(tempBuf, "[SetQuestPrevInfo] - (%d)번 퀘스트 스트링을 받지 못해 퀘스트 제목 처리를 하지 못했습니다.", pQuestString->GetQuestID()) ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;
	}

	ITEM* pItem ;

	PTRLISTPOS pos = NULL ;
	pos = pTitleString->GetTitle()->GetHeadPosition();								// 퀘스트 스트링을 해드 위치로 세팅한다.

	while(pos)																		// 위치 정보가 유효할 동안 while문을 돌린다.
	{
		pItem = NULL ;
		pItem = (ITEM*)pTitleString->GetTitle()->GetNext(pos);						// 위치 정보에 해당하는 퀘스트 스트링 정보를 받고, 위치를 다음으로 세팅한다.

		if(!pItem) continue ;														// 퀘스트 스트링 정보가 유효하지 않으면 continue 처리를 한다.

		sprintf(tempBuf, "< %s >", pItem->string) ;									// 임시 버퍼에 퀘스트 제목을 복사한다.
		m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(255, 150, 0, 255));					// 퀘스트 리스트 다이얼로그에 추가한다.
	}

	m_pQuestDescList->AddItem(" ", 0);												// 퀘스트 리스트에 빈칸을 추가한다.

	CQuest* pQuest = NULL ;
	pQuest = QUESTMGR->GetQuest(dwQuestIdx) ;

	if( !pQuest )
	{
		/*sprintf(tempBuf, "[SetQuestPrevInfo] - (%d)번 퀘스트를 받지 못해서 퀘스트 의뢰자를 세팅하지 못했습니다.", dwQuestIdx) ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;
	}

	DWORD dwCurSubQuestIdx = pQuest->GetCurSubQuestIdx() ;							// 현재 진행중인 서브 퀘스트 인덱스를 받는다.

	if( dwCurSubQuestIdx != 0 )
	{
		--dwCurSubQuestIdx ;
	}

	CSubQuestInfo* pSubQuestInfo = NULL ;											// 서브 퀘스트 정보를 받을 포인터를 선언한다.
	pSubQuestInfo = pQuest->GetSubQuestInfo(dwCurSubQuestIdx) ;

	if( !pSubQuestInfo ) 
	{
		/*sprintf(tempBuf, "[SetQuestPrevInfo] - (%d)번 퀘스트의 (%d)번 서브 퀘스트 정보를 받지 못해서 의뢰자를 세팅하지 못했습니다.", dwQuestIdx, dwCurSubQuestIdx-1) ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;
	}

	cPtrList* pList = pSubQuestInfo->GetQuestNpcScriptList() ;

	if( !pList ) 
	{
		/*sprintf(tempBuf, "[SetQuestPrevInfo] - (%d)번 퀘스트의 (%d)번 서브 퀘스트의 npc 스트립트 리스트를 받지 못해 의뢰자를 세팅하지 못했습니다.", dwQuestIdx, dwCurSubQuestIdx-1) ;
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

			stNPCIDNAME* pNpcIdName = QUESTMGR->GetNpcIdName(m_dwNpcIdx) ;			// 퀘스트 매니져에서 npc 아이디, 이름을 받는다.

			if( !pNpcIdName ) continue ;

			memset(tempBuf, 0, sizeof(tempBuf)) ;									// 임시 버퍼를 초기화 한다.
			sprintf(tempBuf, CHATMGR->GetChatMsg( 1446 ), pNpcIdName->name) ;
			m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(255, 255, 0, 255));		// 퀘스트 설명 리스트에 추가한다.

			bNpcResult = TRUE ;

			break ;
		}

		if( bNpcResult ) break ;
	}

	CQuestInfo* pQuestInfo = QUESTMGR->GetQuestInfo(dwQuestIdx) ;					// 퀘스트 번호로 퀘스트 정보를 받는다.

	if( !pQuestInfo )																// 퀘스트 정보가 유효하지 않다면,
	{
		/*sprintf(tempBuf, "[SetQuestPrevInfo] - (%d)번 퀘스트 정보를 받아오지 못해 퀘스트 수락 위치를 세팅하지 못했습니다.", dwQuestIdx) ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;																	// 리턴 처리를 한다.
	}

	// - 위치를 세팅한다. -
	int nMapNum = pQuestInfo->GetMapNum() ;											// 퀘스트 정보에서 맵 번호를 받는다.
	memset(tempBuf, 0, sizeof(tempBuf)) ;											// 임시 버퍼를 초기화 한다.
	char* pMapName = GetMapName(nMapNum) ;											// 맵 번호에 따른 맵 이름을 받는다.	
	sprintf(tempBuf, CHATMGR->GetChatMsg( 1447 ), pMapName) ;
	m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(255, 255, 0, 255));				// 퀘스트 설명에 추가한다.
}





void CQuestDialog::RequestInfo(CQuestString* pQuestString)
{
	char tempBuf[128] = {0, } ;

	if( !pQuestString )
	{
		/*sprintf(tempBuf, "[RequestInfo] - 퀘스트 스트링 정보가 유효하지 않아, 의뢰내용을 세팅하지 못했습니다.") ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;
	}

	DWORD dwQuestIdx = pQuestString->GetQuestID() ;

	CQuest* pQuest = NULL ;
	pQuest = QUESTMGR->GetQuest(dwQuestIdx) ;

	CQuestInfo* pQuestInfo = QUESTMGR->GetQuestInfo(dwQuestIdx) ;				// 퀘스트 번호로 퀘스트 정보를 받는다.

	if( !pQuestInfo )																// 퀘스트 정보가 유효하지 않다면,
	{
		/*sprintf(tempBuf, "[QuestDescView] - (%d)번 퀘스트 정보를 받아오지 못해 퀘스트 설명창을 실행하지 못했습니다.", dwQuestIdx) ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;																	// 리턴 처리를 한다.
	}

	CSubQuestInfo* pSubQuestInfo = NULL ;											// 서브 퀘스트 정보를 받을 포인터를 선언한다.
	CQuestString* pQString ;

	PTRLISTPOS pos ;

	cPtrList* pTitleList ;
	cPtrList* pDescList ;

	int QuestStringKey = 0 ;

	int nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// 서브 퀘스트 수를 받는다.

    for( int count = 0 ; count < nSubQuestCount ; ++count )							// 서브 퀘스트 수 만큼 for문을 돌린다.
	{
		pSubQuestInfo = NULL ;													// 서브 퀘스트 정보를 받을 포인터를 null처리 한다.
		pSubQuestInfo = pQuest->GetSubQuestInfoArray(count) ;					// 카운트에 해당하는 서브 퀘스트를 받는다.

		if( !pSubQuestInfo ) continue ;											// 서브 퀘스트 정보가 유효하지 않으면 continue한다.
		
		COMBINEKEY(dwQuestIdx, count, QuestStringKey) ;							// 퀘스트, 서브 퀘스트 인덱스로 퀘스트 스트링을 구한다.

		SUBQUEST* pSubQuest = pQuest->GetSubQuest(count) ;							// 서브 퀘스트 정보를 받는다.

		pQString = NULL ;
		pQString = QUESTMGR->GetQuestString(QuestStringKey);						// 서브 퀘스트 스트링을 받는다.

		if( !pQString )	continue ;													// 퀘스트 스트링 정보가 유효하지 않으면 continue한다.

		// - 서브 퀘스트 타이틀 세팅 -
		pTitleList = NULL ;
		pTitleList = pQString->GetTitle() ;											// 서브 퀘스트의 타이틀 리스트를 받는다.

		if( !pTitleList ) continue ;												// 타이틀 리스트가 유효하지 않으면 continue한다.

		m_pQuestDescList->AddItem(" ", 0);											// 퀘스트 리스트에 빈칸을 추가한다.

		int nTitleCount = 0 ;														// 서브 퀘스트의 타이틀 카운트를 담을 변수를 선언하고 0으로 세팅한다.

		nTitleCount = pTitleList->GetCount() ;										// 서브 퀘스트의 타이틀을 세팅한다.

		for( int count2 = 0 ; count2 < nTitleCount ; ++count2 )						// 서브 타이틀 카운트 만큼 for문을 돌린다.
		{
			pos = NULL ;															// 위치 정보를 받을 포인터를 null처리를 한다.
			pos = pTitleList->FindIndex(count2) ;									// 타이틀의 위치를 받는다.

			if( !pos ) continue ;													// 위치 정보가 유효하지 않으면 continue한다.

			ITEM* pTitleLine = (ITEM*)pTitleList->GetAt(pos) ;						// 타이틀 스트링을 받는다.

			if( !pTitleLine ) continue ;											// 타이틀 스트링이 유효하지 않으면, continue한다.

			if( !pSubQuest ) continue ;												// 서브 퀘스트 정보가 유효하지 않으면 continue한다.

			DWORD dwCurCount = pSubQuest->dwData ;									// 현재 개수를 받는다.
			DWORD dwMaxCount = pSubQuest->dwMaxCount ;								// 최대 개수를 받는다.

			memset(tempBuf, 0, sizeof(tempBuf)) ;									// 임시 버퍼를 초기화 한다.

			if( pQString->IsComplete() )											// 퀘스트가 완료 되었으면,
			{
				//sprintf(tempBuf, "%s (완료)", pTitleLine->string) ;					// 제목에 완료를 추가한다.
				sprintf(tempBuf, "%s", pTitleLine->string) ;
				strcat(tempBuf, CHATMGR->GetChatMsg(1463)) ;

				m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(255, 255, 0, 255)) ;	// 제목과 완료를 출력한다.
			}
			else																	// 완료한 퀘스트가 아니면,
			{	
				if( pSubQuest->dwMaxCount == 0 )									// 서브 퀘스트의 토탈 카운트가 0과 같으면,
				{
					if( pSubQuest->dwData == 0 )									// 서브 퀘스트의 현재 카운트가 0과 같으면,
					{
						sprintf(tempBuf, "%s", pTitleLine->string) ;				// 퀘스트 제목만 임시버퍼에 복사한다.
					}
					else															// 서브 퀘스트의 현재 카운트가 0이 아니면,
					{
						sprintf(tempBuf, "%s (%d)", pTitleLine->string, pSubQuest->dwData) ; // 서브 퀘스트의 제목과, 개수를 임시 버퍼에 복사한다.
					}
				}
				else																// 서브 퀘스트의 토탈 카운트가 0이 아니면,
				{
					sprintf(tempBuf, "%s (%d/%d)", pTitleLine->string, pSubQuest->dwData, pSubQuest->dwMaxCount) ; // 서브 퀘스트의 제목과 진행 카운트를 임시 버퍼에 복사한다.
				}										

				m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(255, 255, 0, 255)) ;		// 세팅된 스트링을 퀘스트 설명 리스트 다이얼로그에 추가한다.
			}
		}

        // - 서브 퀘스트 설명 세팅
		pDescList = pQString->GetDesc() ;										// 서브 퀘스트 설명 리스트를 받는다.

		if( !pDescList ) continue ;												// 설명 리스트 정보가 유효하지 않으면, continue한다.

		int nDescCount = 0 ;													// 설명 개수를 받을 변수를 선언하고 0으로 세팅한다.

		nDescCount = pDescList->GetCount() ;									// 설명 리스트의 아이템 수를 받는다.

		for( int count2 = 0 ; count2 < nDescCount ; ++count2 )					// 설명 개수 만큼 for문을 돌린다.
		{
			pos = NULL ;														// 위치 정보를 담을 포인터를 null처리를 한다.
			pos = pDescList->FindIndex(count2) ;								// 카운트에 해당하는 위치 정보를 받는다.

			if( !pos ) continue ;												// 위치 정보가 유효하지 않으면 continue한다.

			ITEM* pDescLine = (ITEM*)pDescList->GetAt(pos) ;  					// 설명 스트링을 받는다.

			if( pDescLine )														// 설명 스트링 정보가 유효하다면,						
			{
				m_pQuestDescList->AddItem(pDescLine->string, pDescLine->rgb) ;	// 퀘스트 설명 리스트에 추가를 한다.
			}
		}

		pTitleList = NULL ;														// 제목 리스트 포인터를 null처리를 한다.
		pDescList  = NULL ;														// 설명 리스트 포인터를 null처리를 한다.

		pQString = NULL ;														// 스트링 포인터를 null처리를 한다.			
	}

	m_pQuestDescList->AddItem(" ", 0);												// 퀘스트 리스트에 빈칸을 추가한다.
	m_pQuestDescList->AddItem(CHATMGR->GetChatMsg( 1448 ), RGBA_MAKE(255, 255, 0, 255));				// <의뢰내용>을 추가한다.
	if( m_dwNpcIdx != 0 )
	{
		m_pQuestDescList->AddItem(" ", 0);												// 퀘스트 리스트에 빈칸을 추가한다.
	}

	cDialogueList* pDialogueList = NULL ;											// 다이얼로그 리스트를 받을 포인터를 선언하고 null처리를 한다.
	pDialogueList = NPCSCRIPTMGR->GetDialogueList();								// NPC 스크립트 매니져에서 다이얼로그 리스트를 받는다.

	cPage* pPage = NPCSCRIPTMGR->GetPage( m_dwNpcIdx, m_dwScriptPage );					// 현재 NPC와 현재 페이지 아이디로 페이지 정보를 받는다.

	WORD wIdx = 0;																	// 인덱스를 담을 변수를 선언하고 0으로 세팅한다.
	WORD LineInfo = 0;																// 라인 정보를 받을 변수를 선언하고 0으로 세팅한다.

	DIALOGUE* temp = NULL;															// 다이얼로그 정보를 담을 포인터 변수를 선언하고 NULL처리를 한다.

	LINKITEM* pItem = NULL;															// 링크 아이템 정보를 담을 포인터 변수를 선언하고 NULL 처리를 한다.
	LINKITEM* Prev = NULL;															// 이전 링크 아이템 정보를 담을 포인터 변수를 선언하고 NULL 처리를 한다.

	char npcDialog[1024] = {0, } ;													// npc 대사를 담을 임시 버퍼를 선언한다.

	if( m_dwDialogType == 0 )
	{
		while(TRUE)
		{
			if( pPage )
			{
				DWORD dwDialogue = pPage->GetDialogue() ;										// 페이지의 다이얼로그 번호를 받는다.

				memset(npcDialog, 0, 1024) ;

				wIdx = 0 ;

				while(TRUE)																// while문을 돌린다.
				{
					temp = pDialogueList->GetDialogue( dwDialogue, wIdx );				// 랜덤하게 뽑은 페이지로 다이얼로그 정보를 받는다.

					if( temp == NULL ) break;											// 정보가 유효하지 않으면, while문을 빠져나간다.

					strcat(npcDialog, temp->str) ;										// 읽어들인 대사를 임시 버퍼에 추가한다.

					++wIdx ;															// 인덱스를 증가한다.
				}

				m_pQuestDescList->AddItemAutoLine(npcDialog, RGBA_MAKE(255, 255, 255, 255)) ;

				m_pQuestDescList->AddItem(" ", 0);										// 퀘스트 리스트에 빈칸을 추가한다.

				HYPERLINK* pHyperLink = pPage->GetHyperText(0) ;

				if( pHyperLink )
				{
					m_dwScriptPage = pHyperLink->dwData ;

					pPage = NPCSCRIPTMGR->GetPage( m_dwNpcIdx, m_dwScriptPage );			// 현재 NPC와 현재 페이지 아이디로 페이지 정보를 받는다.
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
		int nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// 서브 퀘스트 수를 받는다.

		cDialogueList* pDialogueList = NULL ;											// 다이얼로그 리스트를 받을 포인터를 선언하고 null처리를 한다.
		pDialogueList = NPCSCRIPTMGR->GetDialogueList();								// NPC 스크립트 매니져에서 다이얼로그 리스트를 받는다.
		if( pDialogueList == NULL ) return ;											// 다이얼로그 정보가 유효하지 않으면, FALSE 리턴 처리를 한다.

		CSubQuestInfo* pSubQuestInfo = NULL ;											// 서브 퀘스트 정보를 받을 포인터를 선언하고 NULL처리를 한다.
		pSubQuestInfo = pQuestInfo->GetSubQuestInfo(0) ;								// 카운트에 해당하는 서브 퀘스트 정보를 받는다.

		if( !pSubQuestInfo ) return ;

		DWORD dwPage = pSubQuestInfo->GetNpcScriptPage(m_dwNpcIdx) ;						// NPC 인덱스로 페이지 인덱스를 받는다.
		cPage* pPage = NPCSCRIPTMGR->GetPage( m_dwNpcIdx, dwPage );						// 현재 NPC와 현재 페이지 아이디로 페이지 정보를 받는다.

		while(TRUE)
		{
			if( pPage )
			{
				DWORD dwDialogue = pPage->GetDialogue() ;										// 페이지의 다이얼로그 번호를 받는다.

				memset(npcDialog, 0, 1024) ;

				wIdx = 0 ;

				while(TRUE)																// while문을 돌린다.
				{
					temp = pDialogueList->GetDialogue( dwDialogue, wIdx );				// 랜덤하게 뽑은 페이지로 다이얼로그 정보를 받는다.

					if( temp == NULL ) break;											// 정보가 유효하지 않으면, while문을 빠져나간다.

					strcat(npcDialog, temp->str) ;										// 읽어들인 대사를 임시 버퍼에 추가한다.

					++wIdx ;															// 인덱스를 증가한다.
				}

				m_pQuestDescList->AddItemAutoLine(npcDialog, RGBA_MAKE(255, 255, 255, 255)) ;

				m_pQuestDescList->AddItem(" ", 0);										// 퀘스트 리스트에 빈칸을 추가한다.

				HYPERLINK* pHyperLink = pPage->GetHyperText(0) ;

				if( pHyperLink )
				{
					dwPage = pHyperLink->dwData ;

					pPage = NPCSCRIPTMGR->GetPage( m_dwNpcIdx, dwPage );					// 현재 NPC와 현재 페이지 아이디로 페이지 정보를 받는다.
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

	// 080103 LYW --- QuestDialog : 보상 경험치와 보상 머니 초기화 처리 추가.
	SetGold(0) ;
	SetExp(0) ;

	if( !pQuestString )
	{
		/*sprintf(tempBuf, "[RequitalInfo] - 퀘스트 스트링 정보가 유효하지 않아, 의뢰내용을 세팅하지 못했습니다.") ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;
	}

	DWORD dwQuestIdx = pQuestString->GetQuestID() ;

	CQuest* pQuest = NULL ;
	pQuest = QUESTMGR->GetQuest(dwQuestIdx) ;

	CQuestInfo* pQuestInfo = QUESTMGR->GetQuestInfo(dwQuestIdx) ;				// 퀘스트 번호로 퀘스트 정보를 받는다.

	if( !pQuestInfo )																// 퀘스트 정보가 유효하지 않다면,
	{
		/*sprintf(tempBuf, "[RequitalInfo] - (%d)번 퀘스트 정보를 받아오지 못해 퀘스트 설명창을 실행하지 못했습니다.", dwQuestIdx) ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;																	// 리턴 처리를 한다.
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


	int nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// 서브 퀘스트 수를 받는다.

	if( nSubQuestCount < 1 )
	{
		/*sprintf(tempBuf, "[RequitalInfo] - (%d)번 퀘스트의 서브 퀘스트 정보가 없어, 보상 아이템 처리를 하지 못했습니다.", dwQuestIdx) ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;																	// 리턴 처리를 한다.
	}

	CSubQuestInfo* pSubQuestInfo = pQuestInfo->GetSubQuestInfo(nSubQuestCount-1) ;					// 마지막 서브 퀘스트 정보를 받는다.

	if( !pSubQuestInfo )																// 서브 퀘스트 정보가 유효하다면,
	{
		/*sprintf(tempBuf, "[RequitalInfo] - (%d)번 퀘스트의 서브 퀘스트 정보가 없어, 보상 아이템 처리를 하지 못했습니다.", dwQuestIdx) ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;																	// 리턴 처리를 한다.
	}

	cPtrList* pTriggerList = pSubQuestInfo->GetTriggerList() ;					// 마지막 서브 퀘스트의 트리거 정보를 받는다.

	if( !pTriggerList )															// 트리거 정보가 유효하다면,
	{
		/*sprintf(tempBuf, "[RequitalInfo] - (%d)번 퀘스트의 트리거 정보를 받지 못해, 보상 아이템 처리를 하지 못했습니다.", dwQuestIdx) ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;																	// 리턴 처리를 한다.
	}

	int nTriggerCount = 0 ;													// 트리거 카운트를 담을 변수를 선언하고, 0으로 세팅한다.

	nTriggerCount = pTriggerList->GetCount() ;								// 트리거 카운트를 받는다.

	CQuestTrigger* pTrigger = NULL ;										// 트리거 정보를 받을 포인터를 선언하고 null처리를 한다.

	PTRLISTPOS pos ;

	PTRLISTPOS exePos = NULL ;												// 실행 리스트 위치 정보를 받을 위치 포인터를 선언하고 null 처리를 한다.

	for( int count = 0 ; count < nTriggerCount ; ++count )					// 트리거 카운트 수 만큼 for문을 돌린다.
	{
		pos = NULL ;														// 위치 정보를 받을 포인터를 null 처리를 한다.
		pos = pTriggerList->FindIndex(count) ;								// 카운트에 해당하는 위치 정보를 받는다.

		if( !pos ) continue ;												// 위치 정보가 유효하다면,

		pTrigger = NULL ;
		pTrigger = (CQuestTrigger*)pTriggerList->GetAt(pos) ;			// 위치에 해당하는 트리거 정보를 받는다.

		if( !pTrigger )	 continue ;										// 트리거 정보가 유효하다면,

		cPtrList* pExeList = NULL ;
		pExeList = pTrigger->GetExeList() ;								// 트리거 실행 리스트를 받는다.

		if( !pExeList )	continue ;										// 트리거 실행 리스트 정보가 유효하다면,

		int nExeCount = pExeList->GetCount() ;					// 실행 리스트 카운트를 받는다.

		CQuestExecute* pExecute = NULL ;						// 퀘스트 실행 정보를 받을 포인터를 선언하고 null처리를 한다.

		for(int count2 = 0 ; count2 < nExeCount ; ++count2 )	// 실행 카운트 만큼 for문을 돌린다.
		{
			exePos = NULL ;										// 실행 위치 포인터를 null 처리를 한다.
			exePos = pExeList->FindIndex(count2) ;				// 카운트에 해당하는 실행 위치 정보를 받는다.									

			if( !exePos ) continue ;										// 퀘스트 실행 위치가 유효하면,

			pExecute = (CQuestExecute*)pExeList->GetAt(exePos) ;	// 퀘스트 실행 정보를 받는다.

			if( !pExecute ) continue ;								// 퀘스트 실행 정보가 유효하면,

			CQuestExecute_Item* pExecute_Item = ((CQuestExecute_Item*)pExecute) ;
			DWORD dwExeKind = pExecute->GetQuestExecuteKind() ;	// 실행 타입을 받는다.

			switch(dwExeKind)							// 실행 타입을 확인한다.
			{
			case eQuestExecute_TakeItem :				// 실행 타입이 아이템을 받는 타입이라면,
				{
					ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(pExecute_Item->GetItemKind());

					if( pItemInfo )
					{
						AddRequitalItem(pItemInfo, pExecute_Item->GetItemNum()) ;
					}
				}
				break ;

			case eQuestExecute_TakeMoney :				// 실행 타입이 골드를 받는 타입이라면,
				{
					SetGold( pExecute_Item->GetItemNum() ) ;
				}
				break ;

			case eQuestExecute_TakeExp :				// 실행 타입이 경험치를 받는 타입이라면,
				{
					SetExp( pExecute_Item->GetItemNum() ) ;
				}
				break ;

			}
		}
	}
}





void CQuestDialog::QuestDescView(CQuestString* pQuestString)						// 퀘스트 상세 설명을 처리 하는 함수.
{
	if( !m_bActive ) return ;														// 퀘스트 창이 열린 상태가 아니라면, 리턴 처리를 한다.

	if(!pQuestString)																// 인자로 넘어온 퀘스트 스트링 정보가 유요하지 않으면 리턴처리한다.
	{
		m_pQuestDescList->RemoveAll() ;												// 설명 리스트 다이얼로그르 모두 비운다.

		int nCellCount = m_pRequitalGrid->GetCellNum() ;							// 아이템 보상 그리드의 셀 카운트를 받는다.

		cIcon* pIcon ;																// 아이콘 정보를 받을 포인터를 선언한다.

		for( int count = 0 ; count < nCellCount ; ++count )							// 그리스 셀 카운트  만큼 for문을 돌린다.
		{
			pIcon = NULL ;															// 아이콘 정보를 받을 포인터를 null처리 한다.
			pIcon = m_pRequitalGrid->GetIconForIdx(count) ;							// 카운트에 해당하는 셀에서 아이콘 정보를 받는다.

			if( pIcon )																// 아이콘 정보가 유효하면,
			{
				m_pRequitalGrid->DeleteIcon(pIcon) ;								// 보상 아이템 그리드에서 아이콘을 삭제한다.
			}
		}

		RequitalItem* item ;														// 보상 아이템의 정보를 받을 포인터를 선언한다.

		PTRLISTPOS pos = m_QuestRequitalList.GetHeadPosition() ;					// 퀘스트 보상 아이템 리스트를 헤드로 세팅하고 위치 정보를 받는다.

		while( pos )																// 위치 정보가 유효할 동안 while문을 돌린다.
		{
			item = NULL ;															// 보상 아이템 정보를 받을 포인터를 null처리 한다.
			item = ( RequitalItem* )m_QuestRequitalList.GetNext( pos ) ;			// 위치에 해당하는 보상아이템 정보를 받고, 위치를 다음으로 세팅한다.

			if( item )																// 아이템 정보가 유효하면,
			{
				WINDOWMGR->AddListDestroyWindow( item->pIcon );						// 윈도우 매니져에 삭제할 윈도우로 현재 아이템의 아이콘을 등록한다.
				SAFE_DELETE( item ) ;												// 아이템 정보를 안전하게 삭제한다.
			}
		}

		m_QuestRequitalList.RemoveAll() ;											// 보상 아이템 리스트를 모두 비운다.

		SetGold(0) ;																// 보상 골드 수치를 0으로 세팅한다.
		SetExp(0) ;																	// 보상 경험치 수치를 0으로 세팅한다.

		return;																		// return 처리 한다.
	}

	m_pQuestDescList->RemoveAll() ;													// 설명 리스트 다이얼로그르 모두 비운다.

	SetQuestPrevInfo(pQuestString) ;

	RequestInfo(pQuestString) ;

	RequitalInfo(pQuestString) ;

	m_pQuestDescList->ResetGuageBarPos() ;
}

//void CQuestDialog::QuestDescView(CQuestString* pQuestString)						// 퀘스트 상세 설명을 처리 하는 함수.
//{
//	if( !m_bActive ) return ;														// 퀘스트 창이 열린 상태가 아니라면, 리턴 처리를 한다.
//
//	if(!pQuestString)																// 인자로 넘어온 퀘스트 스트링 정보가 유요하지 않으면 리턴처리한다.
//	{
//		m_pQuestDescList->RemoveAll() ;												// 설명 리스트 다이얼로그르 모두 비운다.
//
//		int nCellCount = m_pRequitalGrid->GetCellNum() ;							// 아이템 보상 그리드의 셀 카운트를 받는다.
//
//		cIcon* pIcon ;																// 아이콘 정보를 받을 포인터를 선언한다.
//
//		for( int count = 0 ; count < nCellCount ; ++count )							// 그리스 셀 카운트  만큼 for문을 돌린다.
//		{
//			pIcon = NULL ;															// 아이콘 정보를 받을 포인터를 null처리 한다.
//			pIcon = m_pRequitalGrid->GetIconForIdx(count) ;							// 카운트에 해당하는 셀에서 아이콘 정보를 받는다.
//
//			if( pIcon )																// 아이콘 정보가 유효하면,
//			{
//				m_pRequitalGrid->DeleteIcon(pIcon) ;								// 보상 아이템 그리드에서 아이콘을 삭제한다.
//			}
//		}
//
//		RequitalItem* item ;														// 보상 아이템의 정보를 받을 포인터를 선언한다.
//
//		PTRLISTPOS pos = m_QuestRequitalList.GetHeadPosition() ;					// 퀘스트 보상 아이템 리스트를 헤드로 세팅하고 위치 정보를 받는다.
//
//		while( pos )																// 위치 정보가 유효할 동안 while문을 돌린다.
//		{
//			item = NULL ;															// 보상 아이템 정보를 받을 포인터를 null처리 한다.
//			item = ( RequitalItem* )m_QuestRequitalList.GetNext( pos ) ;			// 위치에 해당하는 보상아이템 정보를 받고, 위치를 다음으로 세팅한다.
//
//			if( item )																// 아이템 정보가 유효하면,
//			{
//				WINDOWMGR->AddListDestroyWindow( item->pIcon );						// 윈도우 매니져에 삭제할 윈도우로 현재 아이템의 아이콘을 등록한다.
//				SAFE_DELETE( item ) ;												// 아이템 정보를 안전하게 삭제한다.
//			}
//		}
//
//		m_QuestRequitalList.RemoveAll() ;											// 보상 아이템 리스트를 모두 비운다.
//
//		SetGold(0) ;																// 보상 골드 수치를 0으로 세팅한다.
//		SetExp(0) ;																	// 보상 경험치 수치를 0으로 세팅한다.
//
//		return;																		// return 처리 한다.
//	}
//
//	char tempBuf[128] = {0, } ;														// 임시버퍼를 선언하고 초기화 한다.
//
//	PTRLISTPOS pos = NULL;															// 위치 정보를 받을 포인터 변수를 선언하고 null처리 한다.
//
//	DWORD dwMainQuestID = pQuestString->GetQuestID() ;								// 스트링에서 퀘스트 번호를 받는다.
//
//	CQuestInfo* pQuestInfo = QUESTMGR->GetQuestInfo(dwMainQuestID) ;				// 퀘스트 번호로 퀘스트 정보를 받는다.
//
//	if( !pQuestInfo )																// 퀘스트 정보가 유효하지 않다면,
//	{
//		return ;																	// 리턴 처리를 한다.
//	}
//
//	int nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// 서브 퀘스트 수를 받는다.
//	
//	m_pQuestDescList->RemoveAll();													// 퀘스트 리스트 다이얼로그를 모두 비운다.
//
//	ITEM* pQStr = NULL ;
//
//	int QuestStringKey = 0 ;														// 퀘스트 스트링 키를 담을 변수를 선언하고 0으로 세팅한다.
//
//	CQuestString* pQString = NULL ;													// 퀘스트 스트링 정보를 받을 포인터를 선언하고 null처리를 한다.
//
//	cPtrList* pTitleList = NULL ;													// 서브 퀘스트 제목 리스트를 받을 포인터를 선언하고 null처리를 한다.
//	cPtrList* pDescList = NULL ;													// 서브 퀘스트 설명 리스트를 받을 포인터를 선언하고 null처리를 한다.
//
//	CQuest* pQuest = QUESTMGR->GetQuest( dwMainQuestID );							// 퀘스트 정보를 받는다.
//
//	if( !pQuest ) return ;															// 퀘스트 정보가 유효하지 않으면, return 처리 한다.
//
//	CSubQuestInfo* pDescSubQuestInfo ;												// 서브 퀘스트 정보를 받을 포인터를 선언한다.
////	CSubQuestInfo* pDescCurSubQuestInfo ;
//
//	////////////////////////////////////////
//	// - 퀘스트 이름, 의뢰자, 위치 세팅 -
//	////////////////////////////////////////
//	// - 퀘스트 제목 세팅 - 
//	CQuestString* pTitleString = NULL ;												// 퀘스트 제목 스트링 정보를 받을 포인터를 선언하고 null처리 한다.
//	pTitleString = QUESTMGR->GetQuestString(pQuestString->GetQuestID(), 0) ;		// 퀘스트 제목 스트링을 받는다.
//
//	pos = pTitleString->GetTitle()->GetHeadPosition();								// 퀘스트 스트링을 해드 위치로 세팅한다.
//
//	while(pos)																		// 위치 정보가 유효할 동안 while문을 돌린다.
//	{
//		pQStr = (ITEM*)pTitleString->GetTitle()->GetNext(pos);						// 위치 정보에 해당하는 퀘스트 스트링 정보를 받고, 위치를 다음으로 세팅한다.
//
//		if(pQStr)																	// 퀘스트 스트링 정보가 유효하다면,
//		{
//			sprintf(tempBuf, "< %s >", pQStr->string) ;								// 임시 버퍼에 퀘스트 제목을 복사한다.
//			m_pQuestDescList->AddItem(tempBuf, MAINQUEST_TITLE_COLOR);				// 퀘스트 리스트 다이얼로그에 추가한다.
//			++m_toPreList;															// 이전 리스트 값을 증가한다.
//		}
//	}
//
//	m_pQuestDescList->AddItem(" ", 0);												// 퀘스트 리스트에 빈칸을 추가한다.
//	++m_toPreList;																	// 이전 리스트 값을 증가한다.
//
//	DWORD dwNpcIdx = 0 ;															// 퀘스트 정보에서 npc 인덱스를 받는다. 받는다.
//	DWORD dwScriptPage = 0 ;
//	DWORD dwDialogType = 0 ;
//	DWORD dwCurSubQuestIdx = pQuest->GetCurSubQuestIdx() ;							// 현재 진행중인 서브 퀘스트 인덱스를 받는다.
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
//			stNPCIDNAME* pNpcIdName = QUESTMGR->GetNpcIdName(dwNpcIdx) ;					// 퀘스트 매니져에서 npc 아이디, 이름을 받는다.
//
//			if( !pNpcIdName ) continue ;
//
//			memset(tempBuf, 0, sizeof(tempBuf)) ;										// 임시 버퍼를 초기화 한다.
//			sprintf(tempBuf, "의뢰자 - %s", pNpcIdName->name) ;					// 의뢰자 이름으로 npc 이름을 세팅한다.
//			m_pQuestDescList->AddItem(tempBuf, SUBQUEST_TITLE_COLOR_SEL);				// 퀘스트 설명 리스트에 추가한다.
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
//	// for( int count = 0 ; count < nSubQuestCount ; ++count )							// 서브 퀘스트 수 만큼 for문을 돌린다.
//	//{
//	//	pDescSubQuestInfo = NULL ;													// 서브 퀘스트 정보를 받을 포인터를 null처리 한다.
//	//	pDescSubQuestInfo = pQuest->GetSubQuestInfoArray(count) ;					// 카운트에 해당하는 서브 퀘스트를 받는다.
//
//	//	if( !pDescSubQuestInfo ) continue ;											// 서브 퀘스트 정보가 유효하지 않으면 continue한다.
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
//	//			memset(tempBuf, 0, sizeof(tempBuf)) ;										// 임시 버퍼를 초기화 한다.
//	//			sprintf(tempBuf, "의뢰자 - %s", pNpcInfo->Name) ;							// 의뢰자 이름으로 npc 이름을 세팅한다.
//	//			m_pQuestDescList->AddItem(tempBuf, SUBQUEST_TITLE_COLOR_SEL);				// 퀘스트 설명 리스트에 추가한다.
//	//		}
//	//	}
//	//}
//
//	// - 위치를 세팅한다. -
//	int nMapNum = pQuestInfo->GetMapNum() ;											// 퀘스트 정보에서 맵 번호를 받는다.
//	memset(tempBuf, 0, sizeof(tempBuf)) ;											// 임시 버퍼를 초기화 한다.
//	char* pMapName = GetMapName(nMapNum) ;											// 맵 번호에 따른 맵 이름을 받는다.	
//	sprintf(tempBuf, "위치 - %s", pMapName) ;										// 위치에 맵 이름을 추가한다.
//	m_pQuestDescList->AddItem(tempBuf, SUBQUEST_TITLE_COLOR_SEL);					// 퀘스트 설명에 추가한다.
//
//	//m_toPreList= -1;																// 이전 리스트를 -1로 세팅한다.
//
//	//ITEM* pQStr = NULL;																// 퀘스트 스트링 정보를 받을 변수를 선언하고 null처리 한다.
//
//	//// - 퀘스트 제목 세팅 - 
//	//CQuestString* pTitleString = NULL ;												// 퀘스트 제목 스트링 정보를 받을 포인터를 선언하고 null처리 한다.
//	//pTitleString = QUESTMGR->GetQuestString(pQuestString->GetQuestID(), 0) ;		// 퀘스트 제목 스트링을 받는다.
//
//	//pos = pTitleString->GetTitle()->GetHeadPosition();								// 퀘스트 스트링을 해드 위치로 세팅한다.
//
//	//while(pos)																		// 위치 정보가 유효할 동안 while문을 돌린다.
//	//{
//	//	pQStr = (ITEM*)pTitleString->GetTitle()->GetNext(pos);						// 위치 정보에 해당하는 퀘스트 스트링 정보를 받고, 위치를 다음으로 세팅한다.
//
//	//	if(pQStr)																	// 퀘스트 스트링 정보가 유효하다면,
//	//	{
//	//		sprintf(tempBuf, "< %s >", pQStr->string) ;								// 임시 버퍼에 퀘스트 제목을 복사한다.
//	//		m_pQuestDescList->AddItem(tempBuf, MAINQUEST_TITLE_COLOR);				// 퀘스트 리스트 다이얼로그에 추가한다.
//	//		++m_toPreList;															// 이전 리스트 값을 증가한다.
//	//	}
//	//}
//
//	//m_pQuestDescList->AddItem(" ", 0);												// 퀘스트 리스트에 빈칸을 추가한다.
//	//++m_toPreList;																	// 이전 리스트 값을 증가한다.
//
//	//// - 의뢰자 세팅 - 
//	//if( pNpcIdName )																// npc 정보가 유효하면,
//	//{
//	//	memset(tempBuf, 0, sizeof(tempBuf)) ;										// 임시 버퍼를 초기화 한다.
//	//	sprintf(tempBuf, "의뢰자 - %s", pNpcIdName->name) ;							// 의뢰자 이름으로 npc 이름을 세팅한다.
//	//	m_pQuestDescList->AddItem(tempBuf, SUBQUEST_TITLE_COLOR_SEL);				// 퀘스트 설명 리스트에 추가한다.
//	//}
//	//else																			// npc 정보가 유효하지 않으면,
//	//{
//	//	ASSERTMSG(0, "퀘스트 설명창에 NPC 정보를 로딩하지 못했습니다.") ;			// npc 정보는 분명히 있어야 한다.
//	//	m_pQuestDescList->AddItem("<*********>", SUBQUEST_TITLE_COLOR_SEL);			// 알아볼 수 있게 <*********> 로 표시한다.
//	//}
//
//	//// - 위치를 세팅한다. -
//	//memset(tempBuf, 0, sizeof(tempBuf)) ;											// 임시 버퍼를 초기화 한다.
//	//char* pMapName = GetMapName(nMapNum) ;											// 맵 번호에 따른 맵 이름을 받는다.	
//	//sprintf(tempBuf, "위치 - %s", pMapName) ;										// 위치에 맵 이름을 추가한다.
//	//m_pQuestDescList->AddItem(tempBuf, SUBQUEST_TITLE_COLOR_SEL);					// 퀘스트 설명에 추가한다.
//
//
//	//////////////////////////////////////////
//	//// - 의뢰 내용 세팅 -
//	//////////////////////////////////////////
//
//	m_pQuestDescList->AddItem(" ", 0);												// 퀘스트 리스트에 빈칸을 추가한다.
//	m_pQuestDescList->AddItem("< 의뢰 내용 > ", MAINQUEST_TITLE_COLOR);				// <의뢰내용>을 추가한다.
//	m_pQuestDescList->AddItem(" ", 0);												// 퀘스트 리스트에 빈칸을 추가한다.
//
//	cDialogueList* pDialogueList = NULL ;											// 다이얼로그 리스트를 받을 포인터를 선언하고 null처리를 한다.
//	pDialogueList = NPCSCRIPTMGR->GetDialogueList();								// NPC 스크립트 매니져에서 다이얼로그 리스트를 받는다.
//
//	cPage* pPage = NPCSCRIPTMGR->GetPage( dwNpcIdx, dwScriptPage );					// 현재 NPC와 현재 페이지 아이디로 페이지 정보를 받는다.
//
//	WORD wIdx = 0;																	// 인덱스를 담을 변수를 선언하고 0으로 세팅한다.
//	WORD LineInfo = 0;																// 라인 정보를 받을 변수를 선언하고 0으로 세팅한다.
//
//	DIALOGUE* temp = NULL;															// 다이얼로그 정보를 담을 포인터 변수를 선언하고 NULL처리를 한다.
//
//	LINKITEM* pItem = NULL;															// 링크 아이템 정보를 담을 포인터 변수를 선언하고 NULL 처리를 한다.
//	LINKITEM* Prev = NULL;															// 이전 링크 아이템 정보를 담을 포인터 변수를 선언하고 NULL 처리를 한다.
//
//	char npcDialog[1024] = {0, } ;													// npc 대사를 담을 임시 버퍼를 선언한다.
//
//	if( dwDialogType == 0 )
//	{
//		while(TRUE)
//		{
//			if( pPage )
//			{
//				DWORD dwDialogue = pPage->GetDialogue() ;										// 페이지의 다이얼로그 번호를 받는다.
//
//				memset(npcDialog, 0, 1024) ;
//
//				wIdx = 0 ;
//
//				while(TRUE)																// while문을 돌린다.
//				{
//					temp = pDialogueList->GetDialogue( dwDialogue, wIdx );				// 랜덤하게 뽑은 페이지로 다이얼로그 정보를 받는다.
//
//					if( temp == NULL ) break;											// 정보가 유효하지 않으면, while문을 빠져나간다.
//
//					strcat(npcDialog, temp->str) ;										// 읽어들인 대사를 임시 버퍼에 추가한다.
//
//					++wIdx ;															// 인덱스를 증가한다.
//				}
//
//				m_pQuestDescList->AddItemAutoLine(npcDialog, RGBA_MAKE(10, 10, 10, 255)) ;
//
//				m_pQuestDescList->AddItem(" ", 0);										// 퀘스트 리스트에 빈칸을 추가한다.
//
//				HYPERLINK* pHyperLink = pPage->GetHyperText(0) ;
//
//				if( pHyperLink )
//				{
//					dwScriptPage = pHyperLink->dwData ;
//
//					pPage = NPCSCRIPTMGR->GetPage( dwNpcIdx, dwScriptPage );			// 현재 NPC와 현재 페이지 아이디로 페이지 정보를 받는다.
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
//		int nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// 서브 퀘스트 수를 받는다.
//
//		cDialogueList* pDialogueList = NULL ;											// 다이얼로그 리스트를 받을 포인터를 선언하고 null처리를 한다.
//		pDialogueList = NPCSCRIPTMGR->GetDialogueList();								// NPC 스크립트 매니져에서 다이얼로그 리스트를 받는다.
//		if( pDialogueList == NULL ) return ;											// 다이얼로그 정보가 유효하지 않으면, FALSE 리턴 처리를 한다.
//
//		CSubQuestInfo* pSubQuestInfo = NULL ;											// 서브 퀘스트 정보를 받을 포인터를 선언하고 NULL처리를 한다.
//		pSubQuestInfo = pQuestInfo->GetSubQuestInfo(0) ;								// 카운트에 해당하는 서브 퀘스트 정보를 받는다.
//
//		if( !pSubQuestInfo ) return ;
//
//		DWORD dwPage = pSubQuestInfo->GetNpcScriptPage(dwNpcIdx) ;						// NPC 인덱스로 페이지 인덱스를 받는다.
//		cPage* pPage = NPCSCRIPTMGR->GetPage( dwNpcIdx, dwPage );						// 현재 NPC와 현재 페이지 아이디로 페이지 정보를 받는다.
//
//		while(TRUE)
//		{
//			if( pPage )
//			{
//				DWORD dwDialogue = pPage->GetDialogue() ;										// 페이지의 다이얼로그 번호를 받는다.
//
//				memset(npcDialog, 0, 1024) ;
//
//				wIdx = 0 ;
//
//				while(TRUE)																// while문을 돌린다.
//				{
//					temp = pDialogueList->GetDialogue( dwDialogue, wIdx );				// 랜덤하게 뽑은 페이지로 다이얼로그 정보를 받는다.
//
//					if( temp == NULL ) break;											// 정보가 유효하지 않으면, while문을 빠져나간다.
//
//					strcat(npcDialog, temp->str) ;										// 읽어들인 대사를 임시 버퍼에 추가한다.
//
//					++wIdx ;															// 인덱스를 증가한다.
//				}
//
//				m_pQuestDescList->AddItemAutoLine(npcDialog, RGBA_MAKE(10, 10, 10, 255)) ;
//
//				m_pQuestDescList->AddItem(" ", 0);										// 퀘스트 리스트에 빈칸을 추가한다.
//
//				HYPERLINK* pHyperLink = pPage->GetHyperText(0) ;
//
//				if( pHyperLink )
//				{
//					dwPage = pHyperLink->dwData ;
//
//					pPage = NPCSCRIPTMGR->GetPage( dwNpcIdx, dwPage );					// 현재 NPC와 현재 페이지 아이디로 페이지 정보를 받는다.
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
//	// for( int count = 0 ; count < nSubQuestCount ; ++count )							// 서브 퀘스트 수 만큼 for문을 돌린다.
//	//{
//	//	pDescSubQuestInfo = NULL ;													// 서브 퀘스트 정보를 받을 포인터를 null처리 한다.
//	//	pDescSubQuestInfo = pQuest->GetSubQuestInfoArray(count) ;					// 카운트에 해당하는 서브 퀘스트를 받는다.
//
//	//	if( !pDescSubQuestInfo ) continue ;											// 서브 퀘스트 정보가 유효하지 않으면 continue한다.
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
//	//			cPage* pPage = NPCSCRIPTMGR->GetPage( dwNpcIdx, dwScriptPage );					// 현재 NPC와 현재 페이지 아이디로 페이지 정보를 받는다.
//
//	//			WORD wIdx = 0;																	// 인덱스를 담을 변수를 선언하고 0으로 세팅한다.
//	//			WORD LineInfo = 0;																// 라인 정보를 받을 변수를 선언하고 0으로 세팅한다.
//
//	//			DIALOGUE* temp = NULL;															// 다이얼로그 정보를 담을 포인터 변수를 선언하고 NULL처리를 한다.
//
//	//			LINKITEM* pItem = NULL;															// 링크 아이템 정보를 담을 포인터 변수를 선언하고 NULL 처리를 한다.
//	//			LINKITEM* Prev = NULL;															// 이전 링크 아이템 정보를 담을 포인터 변수를 선언하고 NULL 처리를 한다.
//
//	//			char npcDialog[1024] = {0, } ;													// npc 대사를 담을 임시 버퍼를 선언한다.
//
//	//			while(TRUE)
//	//			{
//	//				if( pPage )
//	//				{
//	//					DWORD dwDialogue = pPage->GetDialogue() ;										// 페이지의 다이얼로그 번호를 받는다.
//
//	//					memset(npcDialog, 0, 1024) ;
//
//	//					wIdx = 0 ;
//
//	//					while(TRUE)																// while문을 돌린다.
//	//					{
//	//						temp = pDialogueList->GetDialogue( dwDialogue, wIdx );				// 랜덤하게 뽑은 페이지로 다이얼로그 정보를 받는다.
//
//	//						if( temp == NULL ) break;											// 정보가 유효하지 않으면, while문을 빠져나간다.
//
//	//						strcat(npcDialog, temp->str) ;										// 읽어들인 대사를 임시 버퍼에 추가한다.
//
//	//						++wIdx ;															// 인덱스를 증가한다.
//	//					}
//
//	//					m_pQuestDescList->AddItemAutoLine(npcDialog, RGBA_MAKE(10, 10, 10, 255)) ;
//
//	//					m_pQuestDescList->AddItem(" ", 0);										// 퀘스트 리스트에 빈칸을 추가한다.
//
//	//					HYPERLINK* pHyperLink = pPage->GetHyperText(0) ;
//
//	//					if( pHyperLink )
//	//					{
//	//						dwPage = pHyperLink->dwData ;
//
//	//						pPage = NPCSCRIPTMGR->GetPage( dwNpcIdx, dwPage );					// 현재 NPC와 현재 페이지 아이디로 페이지 정보를 받는다.
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
//	////int nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// 서브 퀘스트 수를 받는다.
//
//	////cDialogueList* pDialogueList = NULL ;											// 다이얼로그 리스트를 받을 포인터를 선언하고 null처리를 한다.
//	////pDialogueList = NPCSCRIPTMGR->GetDialogueList();								// NPC 스크립트 매니져에서 다이얼로그 리스트를 받는다.
//	////if( pDialogueList == NULL ) return ;											// 다이얼로그 정보가 유효하지 않으면, FALSE 리턴 처리를 한다.
//
//	////CSubQuestInfo* pSubQuestInfo = NULL ;											// 서브 퀘스트 정보를 받을 포인터를 선언하고 NULL처리를 한다.
//	////pSubQuestInfo = pQuestInfo->GetSubQuestInfo(0) ;								// 카운트에 해당하는 서브 퀘스트 정보를 받는다.
//
//	////if( !pSubQuestInfo ) return ;
//
//	////DWORD dwPage = pSubQuestInfo->GetNpcScriptPage(dwNpcIdx) ;						// NPC 인덱스로 페이지 인덱스를 받는다.
//	////cPage* pPage = NPCSCRIPTMGR->GetPage( dwNpcIdx, dwPage );						// 현재 NPC와 현재 페이지 아이디로 페이지 정보를 받는다.
//
//	////WORD wIdx = 0;																	// 인덱스를 담을 변수를 선언하고 0으로 세팅한다.
//	////WORD LineInfo = 0;																// 라인 정보를 받을 변수를 선언하고 0으로 세팅한다.
//
//	////DIALOGUE* temp = NULL;															// 다이얼로그 정보를 담을 포인터 변수를 선언하고 NULL처리를 한다.
//
//	////LINKITEM* pItem = NULL;															// 링크 아이템 정보를 담을 포인터 변수를 선언하고 NULL 처리를 한다.
//	////LINKITEM* Prev = NULL;															// 이전 링크 아이템 정보를 담을 포인터 변수를 선언하고 NULL 처리를 한다.
//
//	////char npcDialog[1024] = {0, } ;													// npc 대사를 담을 임시 버퍼를 선언한다.
//
//	////while(TRUE)
//	////{
//	////	if( pPage )
//	////	{
//	////		DWORD dwDialogue = pPage->GetDialogue() ;										// 페이지의 다이얼로그 번호를 받는다.
//
//	////		memset(npcDialog, 0, 1024) ;
//
//	////		wIdx = 0 ;
//
//	////		while(TRUE)																// while문을 돌린다.
//	////		{
//	////			temp = pDialogueList->GetDialogue( dwDialogue, wIdx );				// 랜덤하게 뽑은 페이지로 다이얼로그 정보를 받는다.
//
//	////			if( temp == NULL ) break;											// 정보가 유효하지 않으면, while문을 빠져나간다.
//
//	////			strcat(npcDialog, temp->str) ;										// 읽어들인 대사를 임시 버퍼에 추가한다.
//
//	////			++wIdx ;															// 인덱스를 증가한다.
//	////		}
//
//	////		m_pQuestDescList->AddItemAutoLine(npcDialog, RGBA_MAKE(10, 10, 10, 255)) ;
//
//	////		m_pQuestDescList->AddItem(" ", 0);										// 퀘스트 리스트에 빈칸을 추가한다.
//
//	////		HYPERLINK* pHyperLink = pPage->GetHyperText(0) ;
//
//	////		if( pHyperLink )
//	////		{
//	////			dwPage = pHyperLink->dwData ;
//
//	////			pPage = NPCSCRIPTMGR->GetPage( dwNpcIdx, dwPage );					// 현재 NPC와 현재 페이지 아이디로 페이지 정보를 받는다.
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
//	//// - 서브 퀘스트 내용 세팅 -
//	//////////////////////////////////////////
//
//    for( int count = 0 ; count < nSubQuestCount ; ++count )							// 서브 퀘스트 수 만큼 for문을 돌린다.
//	{
//		pDescSubQuestInfo = NULL ;													// 서브 퀘스트 정보를 받을 포인터를 null처리 한다.
//		pDescSubQuestInfo = pQuest->GetSubQuestInfoArray(count) ;					// 카운트에 해당하는 서브 퀘스트를 받는다.
//
//		if( !pDescSubQuestInfo ) continue ;											// 서브 퀘스트 정보가 유효하지 않으면 continue한다.
//		
//		COMBINEKEY(dwMainQuestID, count, QuestStringKey) ;							// 퀘스트, 서브 퀘스트 인덱스로 퀘스트 스트링을 구한다.
//
//		SUBQUEST* pSubQuest = pQuest->GetSubQuest(count) ;							// 서브 퀘스트 정보를 받는다.
//
//		pQString = QUESTMGR->GetQuestString(QuestStringKey);						// 서브 퀘스트 스트링을 받는다.
//
//		if( !pQString )	continue ;													// 퀘스트 스트링 정보가 유효하지 않으면 continue한다.
//
//		// - 서브 퀘스트 타이틀 세팅 -
//		pTitleList = pQString->GetTitle() ;											// 서브 퀘스트의 타이틀 리스트를 받는다.
//
//		if( !pTitleList ) continue ;												// 타이틀 리스트가 유효하지 않으면 continue한다.
//
//		m_pQuestDescList->AddItem(" ", 0);											// 퀘스트 리스트에 빈칸을 추가한다.
//
//		int nTitleCount = 0 ;														// 서브 퀘스트의 타이틀 카운트를 담을 변수를 선언하고 0으로 세팅한다.
//
//		nTitleCount = pTitleList->GetCount() ;										// 서브 퀘스트의 타이틀을 세팅한다.
//
//		for( int count2 = 0 ; count2 < nTitleCount ; ++count2 )						// 서브 타이틀 카운트 만큼 for문을 돌린다.
//		{
//			pos = NULL ;															// 위치 정보를 받을 포인터를 null처리를 한다.
//			pos = pTitleList->FindIndex(count2) ;									// 타이틀의 위치를 받는다.
//
//			if( !pos ) continue ;													// 위치 정보가 유효하지 않으면 continue한다.
//
//			ITEM* pTitleLine = (ITEM*)pTitleList->GetAt(pos) ;						// 타이틀 스트링을 받는다.
//
//			if( !pTitleLine ) continue ;											// 타이틀 스트링이 유효하지 않으면, continue한다.
//
//			if( !pSubQuest ) continue ;												// 서브 퀘스트 정보가 유효하지 않으면 continue한다.
//
//			DWORD dwCurCount = pSubQuest->dwData ;									// 현재 개수를 받는다.
//			DWORD dwMaxCount = pSubQuest->dwMaxCount ;								// 최대 개수를 받는다.
//
//			memset(tempBuf, 0, sizeof(tempBuf)) ;									// 임시 버퍼를 초기화 한다.
//
//			if( pQString->IsComplete() )											// 퀘스트가 완료 되었으면,
//			{
//				sprintf(tempBuf, "%s (완료)", pTitleLine->string) ;					// 제목에 완료를 추가한다.
//
//				m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(0, 64, 128, 255)) ;	// 제목과 완료를 출력한다.
//			}
//			else																	// 완료한 퀘스트가 아니면,
//			{	
//				if( pSubQuest->dwMaxCount == 0 )									// 서브 퀘스트의 토탈 카운트가 0과 같으면,
//				{
//					if( pSubQuest->dwData == 0 )									// 서브 퀘스트의 현재 카운트가 0과 같으면,
//					{
//						sprintf(tempBuf, "%s", pTitleLine->string) ;				// 퀘스트 제목만 임시버퍼에 복사한다.
//					}
//					else															// 서브 퀘스트의 현재 카운트가 0이 아니면,
//					{
//						sprintf(tempBuf, "%s (%d)", pTitleLine->string, pSubQuest->dwData) ; // 서브 퀘스트의 제목과, 개수를 임시 버퍼에 복사한다.
//					}
//				}
//				else																// 서브 퀘스트의 토탈 카운트가 0이 아니면,
//				{
//					sprintf(tempBuf, "%s (%d/%d)", pTitleLine->string, pSubQuest->dwData, pSubQuest->dwMaxCount) ; // 서브 퀘스트의 제목과 진행 카운트를 임시 버퍼에 복사한다.
//				}										
//
//				m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(0, 0, 255, 255)) ;		// 세팅된 스트링을 퀘스트 설명 리스트 다이얼로그에 추가한다.
//			}
//		}
//
//        // - 서브 퀘스트 설명 세팅
//		pDescList = pQString->GetDesc() ;										// 서브 퀘스트 설명 리스트를 받는다.
//
//		if( !pDescList ) continue ;												// 설명 리스트 정보가 유효하지 않으면, continue한다.
//
//		int nDescCount = 0 ;													// 설명 개수를 받을 변수를 선언하고 0으로 세팅한다.
//
//		nDescCount = pDescList->GetCount() ;									// 설명 리스트의 아이템 수를 받는다.
//
//		for( int count2 = 0 ; count2 < nDescCount ; ++count2 )					// 설명 개수 만큼 for문을 돌린다.
//		{
//			pos = NULL ;														// 위치 정보를 담을 포인터를 null처리를 한다.
//			pos = pDescList->FindIndex(count2) ;								// 카운트에 해당하는 위치 정보를 받는다.
//
//			if( !pos ) continue ;												// 위치 정보가 유효하지 않으면 continue한다.
//
//			ITEM* pDescLine = (ITEM*)pDescList->GetAt(pos) ;  					// 설명 스트링을 받는다.
//
//			if( pDescLine )														// 설명 스트링 정보가 유효하다면,						
//			{
//				m_pQuestDescList->AddItem(pDescLine->string, pDescLine->rgb) ;	// 퀘스트 설명 리스트에 추가를 한다.
//			}
//		}
//
//		pTitleList = NULL ;														// 제목 리스트 포인터를 null처리를 한다.
//		pDescList  = NULL ;														// 설명 리스트 포인터를 null처리를 한다.
//
//		pQString = NULL ;														// 스트링 포인터를 null처리를 한다.			
//	}
//
//	//////////////////////////////////////
//	// - 보상 내용 세팅 -
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
//	CSubQuestInfo* pSubQuestInfo = pQuestInfo->GetSubQuestInfo(nSubQuestCount-1) ;					// 마지막 서브 퀘스트 정보를 받는다.
//
//	if( pSubQuestInfo )																// 서브 퀘스트 정보가 유효하다면,
//	{
//		cPtrList* pTriggerList = pSubQuestInfo->GetTriggerList() ;					// 마지막 서브 퀘스트의 트리거 정보를 받는다.
//
//		if( pTriggerList )															// 트리거 정보가 유효하다면,
//		{
//			int nTriggerCount = 0 ;													// 트리거 카운트를 담을 변수를 선언하고, 0으로 세팅한다.
//
//			nTriggerCount = pTriggerList->GetCount() ;								// 트리거 카운트를 받는다.
//
//			CQuestTrigger* pTrigger = NULL ;										// 트리거 정보를 받을 포인터를 선언하고 null처리를 한다.
//
//			PTRLISTPOS exePos = NULL ;												// 실행 리스트 위치 정보를 받을 위치 포인터를 선언하고 null 처리를 한다.
//
//			for( int count = 0 ; count < nTriggerCount ; ++count )					// 트리거 카운트 수 만큼 for문을 돌린다.
//			{
//				pos = NULL ;														// 위치 정보를 받을 포인터를 null 처리를 한다.
//				pos = pTriggerList->FindIndex(count) ;								// 카운트에 해당하는 위치 정보를 받는다.
//
//				if( pos )															// 위치 정보가 유효하다면,
//				{
//					pTrigger = (CQuestTrigger*)pTriggerList->GetAt(pos) ;			// 위치에 해당하는 트리거 정보를 받는다.
//
//					if( pTrigger )													// 트리거 정보가 유효하다면,
//					{
//						cPtrList* pExeList = pTrigger->GetExeList() ;				// 트리거 실행 리스트를 받는다.
//
//						if( pExeList )												// 트리거 실행 리스트 정보가 유효하다면,
//						{
//							int nExeCount = pExeList->GetCount() ;					// 실행 리스트 카운트를 받는다.
//
//							CQuestExecute* pExecute = NULL ;						// 퀘스트 실행 정보를 받을 포인터를 선언하고 null처리를 한다.
//
//							for(int count2 = 0 ; count2 < nExeCount ; ++count2 )	// 실행 카운트 만큼 for문을 돌린다.
//							{
//								exePos = NULL ;										// 실행 위치 포인터를 null 처리를 한다.
//								exePos = pExeList->FindIndex(count2) ;				// 카운트에 해당하는 실행 위치 정보를 받는다.									
//
//								if( exePos )										// 퀘스트 실행 위치가 유효하면,
//								{
//									pExecute = (CQuestExecute*)pExeList->GetAt(exePos) ;	// 퀘스트 실행 정보를 받는다.
//	
//									if( pExecute )									// 퀘스트 실행 정보가 유효하면,
//									{
//										CQuestExecute_Item* pExecute_Item = ((CQuestExecute_Item*)pExecute) ;
//										DWORD dwExeKind = pExecute->GetQuestExecuteKind() ;	// 실행 타입을 받는다.
//
//										switch(dwExeKind)							// 실행 타입을 확인한다.
//										{
//										case eQuestExecute_TakeItem :				// 실행 타입이 아이템을 받는 타입이라면,
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
//										case eQuestExecute_TakeMoney :				// 실행 타입이 골드를 받는 타입이라면,
//											{
//												SetGold( pExecute_Item->GetItemNum() ) ;
//											}
//											break ;
//
//										case eQuestExecute_TakeExp :				// 실행 타입이 경험치를 받는 타입이라면,
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

//void CQuestDialog::QuestDescView(CQuestString* pQuestString)						// 퀘스트 상세 설명 보기 함수.
//{
//	if( !m_bActive )			return ;											// 퀘스트 창이 열린 상태가 아니라면, 리턴 처리를 한다.
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
//		return;												// 퀘스트 스트링 정보가 유효하지 않으면, 리턴 처리를 한다.
//	}
//
//	char tempBuf[128] = {0, } ;														// 임시버퍼를 선언하고 초기화 한다.
//
//	PTRLISTPOS pos = NULL;															// 위치 정보를 받을 포인터 변수를 선언하고 null처리 한다.
//
//	DWORD dwMainQuestID = pQuestString->GetQuestID() ;								// 스트링에서 퀘스트 번호를 받는다.
//
//	CQuestInfo* pQuestInfo = QUESTMGR->GetQuestInfo(dwMainQuestID) ;				// 퀘스트 번호로 퀘스트 정보를 받는다.
//
//	if( !pQuestInfo )																// 퀘스트 정보가 유효하지 않다면,
//	{
//		return ;																	// 리턴 처리를 한다.
//	}
//
//	DWORD dwNpcIdx = pQuestInfo->GetNpcIdx() ;										// 퀘스트 정보에서 npc 인덱스를 받는다.
//
//	stNPCIDNAME* pNpcIdName = QUESTMGR->GetNpcIdName(dwNpcIdx) ;					// 퀘스트 매니져에서 npc 아이디, 이름을 받는다.
//
//	int nMapNum = pQuestInfo->GetMapNum() ;											// 퀘스트 정보에서 맵 번호를 받는다.
//	
//	m_pQuestDescList->RemoveAll();													// 퀘스트 리스트 다이얼로그를 모두 비운다.
//
//	////////////////////////////////////////
//	// - 퀘스트 이름, 의뢰자, 위치 세팅 -
//	////////////////////////////////////////
//
//	m_toPreList= -1;																// 이전 리스트를 -1로 세팅한다.
//
//	ITEM* pQStr = NULL;															// 퀘스트 스트링 정보를 받을 변수를 선언하고 null처리 한다.
//
//	CQuestString* pTitleString = QUESTMGR->GetQuestString(pQuestString->GetQuestID(), 0) ;
//
//	pos = pTitleString->GetTitle()->GetHeadPosition();								// 퀘스트 스트링을 해드 위치로 세팅한다.
//
//	// - 퀘스트 제목 세팅 - 
//
//	while(pos)																		// 위치 정보가 유효할 동안 while문을 돌린다.
//	{
//		pQStr = (ITEM*)pTitleString->GetTitle()->GetNext(pos);					// 위치 정보에 해당하는 퀘스트 스트링 정보를 받는다.
//
//		if(pQStr)																	// 퀘스트 스트링 정보가 유효하다면,
//		{
//			sprintf(tempBuf, "< %s >", pQStr->string) ;								// 임시 버퍼에 퀘스트 제목을 복사한다.
//			m_pQuestDescList->AddItem(tempBuf, MAINQUEST_TITLE_COLOR);				// 퀘스트 리스트 다이얼로그에 추가한다.
//			++m_toPreList;															// 이전 리스트 값을 증가한다.
//		}
//	}
//
//	m_pQuestDescList->AddItem(" ", 0);												// 퀘스트 리스트에 빈칸을 추가한다.
//	++m_toPreList;																	// 이전 리스트 값을 증가한다.
//
//	// - 의뢰자 세팅 - 
//	if( pNpcIdName )																// npc 정보가 유효하면,
//	{
//		memset(tempBuf, 0, sizeof(tempBuf)) ;										// 임시 버퍼를 초기화 한다.
//		sprintf(tempBuf, "의뢰자 - %s", pNpcIdName->name) ;							// 의뢰자 이름으로 npc 이름을 세팅한다.
//		m_pQuestDescList->AddItem(tempBuf, SUBQUEST_TITLE_COLOR_SEL);				// 퀘스트 설명 리스트에 추가한다.
//	}
//	else																			// npc 정보가 유효하지 않으면,
//	{
//		ASSERTMSG(0, "퀘스트 설명창에 NPC 정보를 로딩하지 못했습니다.") ;			// npc 정보는 분명히 있어야 한다.
//		m_pQuestDescList->AddItem("<*********>", SUBQUEST_TITLE_COLOR_SEL);			// 알아볼 수 있게 <*********> 로 표시한다.
//	}
//
//	// - 위치를 세팅한다. -
//	memset(tempBuf, 0, sizeof(tempBuf)) ;											// 임시 버퍼를 초기화 한다.
//	char* pMapName = GetMapName(nMapNum) ;											// 맵 번호에 따른 맵 이름을 받는다.	
//	sprintf(tempBuf, "위치 - %s", pMapName) ;										// 위치에 맵 이름을 추가한다.
//	m_pQuestDescList->AddItem(tempBuf, SUBQUEST_TITLE_COLOR_SEL);					// 퀘스트 설명에 추가한다.
//
//
//	////////////////////////////////////////
//	// - 의뢰 내용 세팅 -
//	////////////////////////////////////////
//
//	m_pQuestDescList->AddItem(" ", 0);												// 퀘스트 리스트에 빈칸을 추가한다.
//	m_pQuestDescList->AddItem("< 의뢰 내용 > ", MAINQUEST_TITLE_COLOR);				// <의뢰내용>을 추가한다.
//	m_pQuestDescList->AddItem(" ", 0);												// 퀘스트 리스트에 빈칸을 추가한다.
//
//	int nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// 서브 퀘스트 수를 받는다.
//
//	cDialogueList* pDialogueList = NULL ;											// 다이얼로그 리스트를 받을 포인터를 선언하고 null처리를 한다.
//	pDialogueList = NPCSCRIPTMGR->GetDialogueList();								// NPC 스크립트 매니져에서 다이얼로그 리스트를 받는다.
//	if( pDialogueList == NULL ) return ;											// 다이얼로그 정보가 유효하지 않으면, FALSE 리턴 처리를 한다.
//
//	CSubQuestInfo* pSubQuestInfo = NULL ;											// 서브 퀘스트 정보를 받을 포인터를 선언하고 NULL처리를 한다.
//	pSubQuestInfo = pQuestInfo->GetSubQuestInfo(0) ;								// 카운트에 해당하는 서브 퀘스트 정보를 받는다.
//
//	if( !pSubQuestInfo ) return ;
//
//	DWORD dwPage = pSubQuestInfo->GetNpcScriptPage(dwNpcIdx) ;						// NPC 인덱스로 페이지 인덱스를 받는다.
//	cPage* pPage = NPCSCRIPTMGR->GetPage( dwNpcIdx, dwPage );						// 현재 NPC와 현재 페이지 아이디로 페이지 정보를 받는다.
//
//	WORD wIdx = 0;																	// 인덱스를 담을 변수를 선언하고 0으로 세팅한다.
//	WORD LineInfo = 0;																// 라인 정보를 받을 변수를 선언하고 0으로 세팅한다.
//
//	DIALOGUE* temp = NULL;															// 다이얼로그 정보를 담을 포인터 변수를 선언하고 NULL처리를 한다.
//
//	LINKITEM* pItem = NULL;															// 링크 아이템 정보를 담을 포인터 변수를 선언하고 NULL 처리를 한다.
//	LINKITEM* Prev = NULL;															// 이전 링크 아이템 정보를 담을 포인터 변수를 선언하고 NULL 처리를 한다.
//
//	char npcDialog[1024] = {0, } ;													// npc 대사를 담을 임시 버퍼를 선언한다.
//
//	while(TRUE)
//	{
//		if( pPage )
//		{
//			DWORD dwDialogue = pPage->GetDialogue() ;										// 페이지의 다이얼로그 번호를 받는다.
//
//			memset(npcDialog, 0, 1024) ;
//
//			wIdx = 0 ;
//
//			while(TRUE)																// while문을 돌린다.
//			{
//				temp = pDialogueList->GetDialogue( dwDialogue, wIdx );				// 랜덤하게 뽑은 페이지로 다이얼로그 정보를 받는다.
//
//				if( temp == NULL ) break;											// 정보가 유효하지 않으면, while문을 빠져나간다.
//
//				strcat(npcDialog, temp->str) ;										// 읽어들인 대사를 임시 버퍼에 추가한다.
//
//				++wIdx ;															// 인덱스를 증가한다.
//			}
//
//			m_pQuestDescList->AddItemAutoLine(npcDialog, RGBA_MAKE(10, 10, 10, 255)) ;
//
//			m_pQuestDescList->AddItem(" ", 0);										// 퀘스트 리스트에 빈칸을 추가한다.
//
//			HYPERLINK* pHyperLink = pPage->GetHyperText(0) ;
//
//			if( pHyperLink )
//			{
//				dwPage = pHyperLink->dwData ;
//
//				pPage = NPCSCRIPTMGR->GetPage( dwNpcIdx, dwPage );					// 현재 NPC와 현재 페이지 아이디로 페이지 정보를 받는다.
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
//	//if( pSubQuestInfo )																// 서브 퀘스트 정보가 유효하다면,
//	//{
//	//	DWORD dwPage = pSubQuestInfo->GetNpcScriptPage(dwNpcIdx) ;					// NPC 인덱스로 페이지 인덱스를 받는다.
//	//	cPage* pPage = NPCSCRIPTMGR->GetPage( dwNpcIdx, dwPage );					// 현재 NPC와 현재 페이지 아이디로 페이지 정보를 받는다.
//
//	//	if( pPage )																	// 페이지 정보가 유효하다면,
//	//	{
//	//		WORD wIdx = 0;															// 인덱스를 담을 변수를 선언하고 0으로 세팅한다.
//	//		WORD LineInfo = 0;														// 라인 정보를 받을 변수를 선언하고 0으로 세팅한다.
//
//	//		DIALOGUE* temp = NULL;													// 다이얼로그 정보를 담을 포인터 변수를 선언하고 NULL처리를 한다.
//
//	//		LINKITEM* pItem = NULL;													// 링크 아이템 정보를 담을 포인터 변수를 선언하고 NULL 처리를 한다.
//	//		LINKITEM* Prev = NULL;													// 이전 링크 아이템 정보를 담을 포인터 변수를 선언하고 NULL 처리를 한다.
//
//	//		DWORD dwDialogue = pPage->GetDialogue() ;								// 페이지의 다이얼로그 번호를 받는다.
//
//	//		// *기존의 다이얼로그를 읽는 방식은, 순전히 npc 대화창에만 맞도록 되어있었다.
//	//		// 때문에 대사를 모두 읽어들여 임시 버퍼에 담고, 출력하고자 하는 리스트의 
//	//		// 텍스트 길이에 맞게 다시 텍스트 커팅 작업을 한다.
//	//		char npcDialog[1024] = {0, } ;											// npc 대사를 담을 임시 버퍼를 선언한다.
//
//	//		while(TRUE)																// while문을 돌린다.
//	//		{
//	//			temp = pDialogueList->GetDialogue( dwDialogue, wIdx );				// 랜덤하게 뽑은 페이지로 다이얼로그 정보를 받는다.
//
//	//			if( temp == NULL ) break;											// 정보가 유효하지 않으면, while문을 빠져나간다.
//
//	//			strcat(npcDialog, temp->str) ;										// 읽어들인 대사를 임시 버퍼에 추가한다.
//
//	//			++wIdx ;															// 인덱스를 증가한다.
//	//		}
//
//	//		m_pQuestDescList->AddItemAutoLine(npcDialog, RGBA_MAKE(10, 10, 10, 255)) ;
//	//	}
//	//}
//
//
//	////////////////////////////////////////
//	// - 서브 퀘스트 내용 세팅 -
//	////////////////////////////////////////
//
//	int QuestStringKey = 0 ;														// 퀘스트 스트링 키를 담을 변수를 선언하고 0으로 세팅한다.
//
//	CQuestString* pQString = NULL ;													// 퀘스트 스트링 정보를 받을 포인터를 선언하고 null처리를 한다.
//
//	cPtrList* pTitleList = NULL ;													// 서브 퀘스트 제목 리스트를 받을 포인터를 선언하고 null처리를 한다.
//	cPtrList* pDescList = NULL ;													// 서브 퀘스트 설명 리스트를 받을 포인터를 선언하고 null처리를 한다.
//
//	CQuest* pQuest = QUESTMGR->GetQuest( dwMainQuestID );							// 퀘스트 정보를 받는다.
//
//    for( int count = 1 ; count < nSubQuestCount ; ++count )							// 서브 퀘스트 수-1만큼 for문을 돌린다.
//	{
//		COMBINEKEY(dwMainQuestID, count, QuestStringKey) ;							// 퀘스트, 서브 퀘스트 인덱스로 퀘스트 스트링을 구한다.
//
//		SUBQUEST* pSubQuest = pQuest->GetSubQuest(count) ;							// 서브 퀘스트 정보를 받는다.
//
//		pQString = QUESTMGR->GetQuestString(QuestStringKey);						// 서브 퀘스트 스트링을 받는다.
//
//		if( pQString )																// 서브 퀘스트 스트링이 유효하면,
//		{
//			// - 서브 퀘스트 타이틀 세팅 -
//			pTitleList = pQString->GetTitle() ;										// 서브 퀘스트의 타이틀 리스트를 받는다.
//
//			if( pTitleList )														// 타이틀 리스트가 유효하면,
//			{
//				m_pQuestDescList->AddItem(" ", 0);									// 퀘스트 리스트에 빈칸을 추가한다.
//
//				int nTitleCount = 0 ;												// 서브 퀘스트의 타이틀 카운트를 담을 변수를 선언하고 0으로 세팅한다.
//
//				nTitleCount = pTitleList->GetCount() ;								// 서브 퀘스트의 타이틀을 세팅한다.
//
//				for( int count2 = 0 ; count2 < nTitleCount ; ++count2 )				// 서브 타이틀 카운트 만큼 for문을 돌린다.
//				{
//					pos = NULL ;													// 위치 정보를 받을 포인터를 null처리를 한다.
//					pos = pTitleList->FindIndex(count2) ;							// 타이틀의 위치를 받는다.
//
//					if( pos )														// 위치 정보가 유효하다면,
//					{
//						ITEM* pTitleLine = (ITEM*)pTitleList->GetAt(pos) ;	// 타이틀 스트링을 받는다.
//
//						if( pTitleLine )											// 타이틀 스트링이 유효하다면,
//						{
//							if( pSubQuest )											// 서브 퀘스트 정보가 유효하다면,
//							{
//								DWORD dwCurCount = pSubQuest->dwData ;				// 현재 개수를 받는다.
//								DWORD dwMaxCount = pSubQuest->dwMaxCount ;			// 최대 개수를 받는다.
//
//								memset(tempBuf, 0, sizeof(tempBuf)) ;				// 임시 버퍼를 초기화 한다.
//
//								if( pQString->IsComplete() )
//								{
//									sprintf(tempBuf, "%s (완료)", pTitleLine->string) ;							// 제목에 완료를 추가한다.
//
//									m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(0, 64, 128, 255)) ;			// 제목과 완료를 출력한다.
//								}
//								else
//								{
//									// 제목과 진행 개수를 세팅한다.
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
//									// 제목과 진행 개수를 리스트에 추가한다.
//									m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(0, 0, 255, 255)) ;
//								}
//							}
//						}
//					}
//				}
//			}
//
//            // - 서브 퀘스트 설명 세팅
//			pDescList = pQString->GetDesc() ;										// 서브 퀘스트 설명 리스트를 받는다.
//
//			if( pDescList )															// 설명 리스트 정보가 유효하다면,
//			{
//				int nDescCount = 0 ;												// 설명 개수를 받을 변수를 선언하고 0으로 세팅한다.
//
//				nDescCount = pDescList->GetCount() ;								// 설명 리스트의 아이템 수를 받는다.
//
//				for( int count2 = 0 ; count2 < nDescCount ; ++count2 )				// 설명 개수 만큼 for문을 돌린다.
//				{
//					pos = NULL ;													// 위치 정보를 담을 포인터를 null처리를 한다.
//					pos = pDescList->FindIndex(count2) ;							// 카운트에 해당하는 위치 정보를 받는다.
//
//					if( pos )														// 위치 정보가 유효하다면,
//					{
//						ITEM* pDescLine = (ITEM*)pDescList->GetAt(pos) ;  	// 설명 스트링을 받는다.
//
//						if( pDescLine )												// 설명 스트링 정보가 유효하다면,						
//						{
//							m_pQuestDescList->AddItem(pDescLine->string, pDescLine->rgb) ;	// 퀘스트 설명 리스트에 추가를 한다.
//						}
//					}
//				}
//			}
//
//			pTitleList = NULL ;														// 제목 리스트 포인터를 null처리를 한다.
//			pDescList  = NULL ;														// 설명 리스트 포인터를 null처리를 한다.
//		}
//		else																		// 서브 퀘스트 스트링이 없으면,
//		{
//			break ;																	// for문을 빠져나간다.
//		}
//
//		pQString = NULL ;															// 스트링 포인터를 null처리를 한다.						
//	}
//
//	////////////////////////////////////////
//	// - 보상 내용 세팅 -
//	////////////////////////////////////////
//
//	//cIcon* deleteIcon = NULL;														// 아이콘 정보를 받을 포인터를 선언하고 null처리를 한다.
//	//RequitalItem* pDeleteSQuest = NULL;												// 퀘스트 아이템 정보를 받을 포인터를 선언하고 null처리를 한다.
//	//
//	//PTRLISTSEARCHSTART(m_QuestRequitalList, RequitalItem*, pDeleteSQuest);			// 퀘스트 아이템 리스트에서 검색을 시작한다.
//	//	if(pDeleteSQuest)															// 퀘스트 아이템 정보가 유효하지 않으면, 검색을 빠져나간다.
//	//	{
//	//		m_pRequitalGrid->DeleteIcon(pDeleteSQuest->wPos, &deleteIcon);			// 퀘스트 아이콘 그리드에서 아이콘을 삭제한다.
//
//	//		if(deleteIcon)															// 아이콘 정보가 유효하지 않으면 검색을 빠져나간다.
//	//		{
//	//			deleteIcon->SetActive(FALSE);										// 아이콘을 비활성화 한다.
//	//			WINDOWMGR->AddListDestroyWindow(deleteIcon);						// 윈도우 매니져에 파괴할 윈도우에 아이콘을 추가한다.
//	//			m_IndexGenerator.ReleaseIndex(deleteIcon->GetID());					// 인덱스 생성기에서 아이콘 인덱스를 해제한다.
//	//			m_QuestRequitalList.Remove(pDeleteSQuest);							// 퀘스트 아이템 리스트에서 퀘스트 아이템 정보를 삭제한다.
//	//		}
//	//	}
//	//PTRLISTSEARCHEND																// 퀘스트 아이템 리스트 검색을 종료한다.
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
//	pSubQuestInfo = pQuestInfo->GetSubQuestInfo(nSubQuestCount-1) ;					// 마지막 서브 퀘스트 정보를 받는다.
//
//	if( pSubQuestInfo )																// 서브 퀘스트 정보가 유효하다면,
//	{
//		cPtrList* pTriggerList = pSubQuestInfo->GetTriggerList() ;					// 마지막 서브 퀘스트의 트리거 정보를 받는다.
//
//		if( pTriggerList )															// 트리거 정보가 유효하다면,
//		{
//			int nTriggerCount = 0 ;													// 트리거 카운트를 담을 변수를 선언하고, 0으로 세팅한다.
//
//			nTriggerCount = pTriggerList->GetCount() ;								// 트리거 카운트를 받는다.
//
//			CQuestTrigger* pTrigger = NULL ;										// 트리거 정보를 받을 포인터를 선언하고 null처리를 한다.
//
//			PTRLISTPOS exePos = NULL ;												// 실행 리스트 위치 정보를 받을 위치 포인터를 선언하고 null 처리를 한다.
//
//			for( int count = 0 ; count < nTriggerCount ; ++count )					// 트리거 카운트 수 만큼 for문을 돌린다.
//			{
//				pos = NULL ;														// 위치 정보를 받을 포인터를 null 처리를 한다.
//				pos = pTriggerList->FindIndex(count) ;								// 카운트에 해당하는 위치 정보를 받는다.
//
//				if( pos )															// 위치 정보가 유효하다면,
//				{
//					pTrigger = (CQuestTrigger*)pTriggerList->GetAt(pos) ;			// 위치에 해당하는 트리거 정보를 받는다.
//
//					if( pTrigger )													// 트리거 정보가 유효하다면,
//					{
//						cPtrList* pExeList = pTrigger->GetExeList() ;				// 트리거 실행 리스트를 받는다.
//
//						if( pExeList )												// 트리거 실행 리스트 정보가 유효하다면,
//						{
//							int nExeCount = pExeList->GetCount() ;					// 실행 리스트 카운트를 받는다.
//
//							CQuestExecute* pExecute = NULL ;						// 퀘스트 실행 정보를 받을 포인터를 선언하고 null처리를 한다.
//
//							for(int count2 = 0 ; count2 < nExeCount ; ++count2 )	// 실행 카운트 만큼 for문을 돌린다.
//							{
//								exePos = NULL ;										// 실행 위치 포인터를 null 처리를 한다.
//								exePos = pExeList->FindIndex(count2) ;				// 카운트에 해당하는 실행 위치 정보를 받는다.									
//
//								if( exePos )										// 퀘스트 실행 위치가 유효하면,
//								{
//									pExecute = (CQuestExecute*)pExeList->GetAt(exePos) ;	// 퀘스트 실행 정보를 받는다.
//	
//									if( pExecute )									// 퀘스트 실행 정보가 유효하면,
//									{
//										CQuestExecute_Item* pExecute_Item = ((CQuestExecute_Item*)pExecute) ;
//										DWORD dwExeKind = pExecute->GetQuestExecuteKind() ;	// 실행 타입을 받는다.
//
//										switch(dwExeKind)							// 실행 타입을 확인한다.
//										{
//										case eQuestExecute_TakeItem :				// 실행 타입이 아이템을 받는 타입이라면,
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
//										case eQuestExecute_TakeMoney :				// 실행 타입이 골드를 받는 타입이라면,
//											{
//												SetGold( pExecute_Item->GetItemNum() ) ;
//											}
//											break ;
//
//										case eQuestExecute_TakeExp :				// 실행 타입이 경험치를 받는 타입이라면,
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

//void CQuestDialog::QuestDescView(CQuestString* pQuestString)						// 퀘스트 상세 설명 보기 함수.
//{
//	////if( !m_bActive )			return ;											// 퀘스트 창이 열린 상태가 아니라면, 리턴 처리를 한다.
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
//	//	return;												// 퀘스트 스트링 정보가 유효하지 않으면, 리턴 처리를 한다.
//	//}
//
//	//char tempBuf[128] = {0, } ;														// 임시버퍼를 선언하고 초기화 한다.
//
//	//PTRLISTPOS pos = NULL;															// 위치 정보를 받을 포인터 변수를 선언하고 null처리 한다.
//
//	//DWORD dwMainQuestID = pQuestString->GetQuestID() ;								// 스트링에서 퀘스트 번호를 받는다.
//
//	//CQuestInfo* pQuestInfo = QUESTMGR->GetQuestInfo(dwMainQuestID) ;				// 퀘스트 번호로 퀘스트 정보를 받는다.
//
//	//if( !pQuestInfo )																// 퀘스트 정보가 유효하지 않다면,
//	//{
//	//	return ;																	// 리턴 처리를 한다.
//	//}
//
//	//DWORD dwNpcIdx = pQuestInfo->GetNpcIdx() ;										// 퀘스트 정보에서 npc 인덱스를 받는다.
//
//	//stNPCIDNAME* pNpcIdName = QUESTMGR->GetNpcIdName(dwNpcIdx) ;					// 퀘스트 매니져에서 npc 아이디, 이름을 받는다.
//
//	//int nMapNum = pQuestInfo->GetMapNum() ;											// 퀘스트 정보에서 맵 번호를 받는다.
//	//
//	//m_pQuestDescList->RemoveAll();													// 퀘스트 리스트 다이얼로그를 모두 비운다.
//
//	//////////////////////////////////////////
//	//// - 퀘스트 이름, 의뢰자, 위치 세팅 -
//	//////////////////////////////////////////
//
//	//m_toPreList= -1;																// 이전 리스트를 -1로 세팅한다.
//
//	//QString* pQStr = NULL;															// 퀘스트 스트링 정보를 받을 변수를 선언하고 null처리 한다.
//
//	//CQuestString* pTitleString = QUESTMGR->GetQuestString(pQuestString->GetQuestID(), 0) ;
//
//	//pos = pTitleString->GetTitle()->GetHeadPosition();								// 퀘스트 스트링을 해드 위치로 세팅한다.
//
//	//// - 퀘스트 제목 세팅 - 
//
//	//while(pos)																		// 위치 정보가 유효할 동안 while문을 돌린다.
//	//{
//	//	pQStr = (QString*)pTitleString->GetTitle()->GetNext(pos);					// 위치 정보에 해당하는 퀘스트 스트링 정보를 받는다.
//
//	//	if(pQStr)																	// 퀘스트 스트링 정보가 유효하다면,
//	//	{
//	//		sprintf(tempBuf, "< %s >", pQStr->Str) ;								// 임시 버퍼에 퀘스트 제목을 복사한다.
//	//		m_pQuestDescList->AddItem(tempBuf, MAINQUEST_TITLE_COLOR);				// 퀘스트 리스트 다이얼로그에 추가한다.
//	//		++m_toPreList;															// 이전 리스트 값을 증가한다.
//	//	}
//	//}
//
//	//m_pQuestDescList->AddItem(" ", 0);												// 퀘스트 리스트에 빈칸을 추가한다.
//	//++m_toPreList;																	// 이전 리스트 값을 증가한다.
//
//	//// - 의뢰자 세팅 - 
//	//if( pNpcIdName )																// npc 정보가 유효하면,
//	//{
//	//	memset(tempBuf, 0, sizeof(tempBuf)) ;										// 임시 버퍼를 초기화 한다.
//	//	sprintf(tempBuf, "의뢰자 - %s", pNpcIdName->name) ;							// 의뢰자 이름으로 npc 이름을 세팅한다.
//	//	m_pQuestDescList->AddItem(tempBuf, SUBQUEST_TITLE_COLOR_SEL);				// 퀘스트 설명 리스트에 추가한다.
//	//}
//	//else																			// npc 정보가 유효하지 않으면,
//	//{
//	//	ASSERTMSG(0, "퀘스트 설명창에 NPC 정보를 로딩하지 못했습니다.") ;			// npc 정보는 분명히 있어야 한다.
//	//	m_pQuestDescList->AddItem("<*********>", SUBQUEST_TITLE_COLOR_SEL);			// 알아볼 수 있게 <*********> 로 표시한다.
//	//}
//
//	//// - 위치를 세팅한다. -
//	//memset(tempBuf, 0, sizeof(tempBuf)) ;											// 임시 버퍼를 초기화 한다.
//	//char* pMapName = GetMapName(nMapNum) ;											// 맵 번호에 따른 맵 이름을 받는다.	
//	//sprintf(tempBuf, "위치 - %s", pMapName) ;										// 위치에 맵 이름을 추가한다.
//	//m_pQuestDescList->AddItem(tempBuf, SUBQUEST_TITLE_COLOR_SEL);					// 퀘스트 설명에 추가한다.
//
//
//	//////////////////////////////////////////
//	//// - 의뢰 내용 세팅 -
//	//////////////////////////////////////////
//
//	//m_pQuestDescList->AddItem(" ", 0);												// 퀘스트 리스트에 빈칸을 추가한다.
//	//m_pQuestDescList->AddItem("< 의뢰 내용 > ", MAINQUEST_TITLE_COLOR);				// <의뢰내용>을 추가한다.
//	//m_pQuestDescList->AddItem(" ", 0);												// 퀘스트 리스트에 빈칸을 추가한다.
//
//	//int nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// 서브 퀘스트 수를 받는다.
//
//	//cDialogueList* pDialogueList = NULL ;											// 다이얼로그 리스트를 받을 포인터를 선언하고 null처리를 한다.
//	//pDialogueList = NPCSCRIPTMGR->GetDialogueList();								// NPC 스크립트 매니져에서 다이얼로그 리스트를 받는다.
//	//if( pDialogueList == NULL ) return ;											// 다이얼로그 정보가 유효하지 않으면, FALSE 리턴 처리를 한다.
//
//	//CSubQuestInfo* pSubQuestInfo = NULL ;											// 서브 퀘스트 정보를 받을 포인터를 선언하고 NULL처리를 한다.
//	//pSubQuestInfo = pQuestInfo->GetSubQuestInfo(0) ;								// 카운트에 해당하는 서브 퀘스트 정보를 받는다.
//
//	//if( pSubQuestInfo )																// 서브 퀘스트 정보가 유효하다면,
//	//{
//	//	DWORD dwPage = pSubQuestInfo->GetNpcScriptPage(dwNpcIdx) ;					// NPC 인덱스로 페이지 인덱스를 받는다.
//	//	cPage* pPage = NPCSCRIPTMGR->GetPage( dwNpcIdx, dwPage );					// 현재 NPC와 현재 페이지 아이디로 페이지 정보를 받는다.
//
//	//	if( pPage )																	// 페이지 정보가 유효하다면,
//	//	{
//	//		WORD wIdx = 0;															// 인덱스를 담을 변수를 선언하고 0으로 세팅한다.
//	//		WORD LineInfo = 0;														// 라인 정보를 받을 변수를 선언하고 0으로 세팅한다.
//
//	//		DIALOGUE* temp = NULL;													// 다이얼로그 정보를 담을 포인터 변수를 선언하고 NULL처리를 한다.
//
//	//		LINKITEM* pItem = NULL;													// 링크 아이템 정보를 담을 포인터 변수를 선언하고 NULL 처리를 한다.
//	//		LINKITEM* Prev = NULL;													// 이전 링크 아이템 정보를 담을 포인터 변수를 선언하고 NULL 처리를 한다.
//
//	//		DWORD dwDialogue = pPage->GetDialogue() ;								// 페이지의 다이얼로그 번호를 받는다.
//
//	//		// *기존의 다이얼로그를 읽는 방식은, 순전히 npc 대화창에만 맞도록 되어있었다.
//	//		// 때문에 대사를 모두 읽어들여 임시 버퍼에 담고, 출력하고자 하는 리스트의 
//	//		// 텍스트 길이에 맞게 다시 텍스트 커팅 작업을 한다.
//	//		char npcDialog[1024] = {0, } ;											// npc 대사를 담을 임시 버퍼를 선언한다.
//
//	//		while(TRUE)																// while문을 돌린다.
//	//		{
//	//			//temp = pDialogueList->GetDialogue( dwDialogue, wIdx );				// 랜덤하게 뽑은 페이지로 다이얼로그 정보를 받는다.
//
//	//			//if( temp == NULL ) break;											// 정보가 유효하지 않으면, while문을 빠져나간다.
//
//	//			//pItem = new LINKITEM;												// 아이템 메모리를 할당하여 포인터로 넘긴다.
//
//	//			//strcpy(pItem->string, temp->str);									// 임시 정보의 스트링을 아이템 스트링에 복사를 한다.
//
//	//			////pItem->rgb = RGBA_MAKE(10, 10, 10, 255);							// 임시 정보의 색상을 아이템 색상으로 세팅한다.
//	//			//pItem->line = m_pQuestDescList->GetItemCount() ;					// 라인을 세팅한다.
//
//	//			//m_pQuestDescList->cListItem::AddItem(pItem) ;						// 리스트에 스트링 아이템을 추가한다.
//
//	//			//++wIdx;																// 인덱스 정보를 증가한다.
//
//	//			temp = pDialogueList->GetDialogue( dwDialogue, wIdx );				// 랜덤하게 뽑은 페이지로 다이얼로그 정보를 받는다.
//
//	//			if( temp == NULL ) break;											// 정보가 유효하지 않으면, while문을 빠져나간다.
//
//	//			strcat(npcDialog, temp->str) ;										// 읽어들인 대사를 임시 버퍼에 추가한다.
//
//	//			++wIdx ;															// 인덱스를 증가한다.
//	//		}
//
//	//		m_pQuestDescList->AddItemAutoLine(npcDialog, RGBA_MAKE(10, 10, 10, 255)) ;
//	//	}
//	//}
//
//
//	//////////////////////////////////////////
//	//// - 서브 퀘스트 내용 세팅 -
//	//////////////////////////////////////////
//
//	//int QuestStringKey = 0 ;														// 퀘스트 스트링 키를 담을 변수를 선언하고 0으로 세팅한다.
//
//	//CQuestString* pQString = NULL ;													// 퀘스트 스트링 정보를 받을 포인터를 선언하고 null처리를 한다.
//
//	//cPtrList* pTitleList = NULL ;													// 서브 퀘스트 제목 리스트를 받을 포인터를 선언하고 null처리를 한다.
//	//cPtrList* pDescList = NULL ;													// 서브 퀘스트 설명 리스트를 받을 포인터를 선언하고 null처리를 한다.
//
//	//CQuest* pQuest = QUESTMGR->GetQuest( dwMainQuestID );							// 퀘스트 정보를 받는다.
//
// //   for( int count = 1 ; count < nSubQuestCount ; ++count )							// 서브 퀘스트 수-1만큼 for문을 돌린다.
//	//{
//	//	COMBINEKEY(dwMainQuestID, count, QuestStringKey) ;							// 퀘스트, 서브 퀘스트 인덱스로 퀘스트 스트링을 구한다.
//
//	//	SUBQUEST* pSubQuest = pQuest->GetSubQuest(count) ;							// 서브 퀘스트 정보를 받는다.
//
//	//	pQString = QUESTMGR->GetQuestString(QuestStringKey);						// 서브 퀘스트 스트링을 받는다.
//
//	//	if( pQString )																// 서브 퀘스트 스트링이 유효하면,
//	//	{
//	//		// - 서브 퀘스트 타이틀 세팅 -
//	//		pTitleList = pQString->GetTitle() ;										// 서브 퀘스트의 타이틀 리스트를 받는다.
//
//	//		if( pTitleList )														// 타이틀 리스트가 유효하면,
//	//		{
//	//			m_pQuestDescList->AddItem(" ", 0);									// 퀘스트 리스트에 빈칸을 추가한다.
//
//	//			int nTitleCount = 0 ;												// 서브 퀘스트의 타이틀 카운트를 담을 변수를 선언하고 0으로 세팅한다.
//
//	//			nTitleCount = pTitleList->GetCount() ;								// 서브 퀘스트의 타이틀을 세팅한다.
//
//	//			for( int count2 = 0 ; count2 < nTitleCount ; ++count2 )				// 서브 타이틀 카운트 만큼 for문을 돌린다.
//	//			{
//	//				pos = NULL ;													// 위치 정보를 받을 포인터를 null처리를 한다.
//	//				pos = pTitleList->FindIndex(count2) ;							// 타이틀의 위치를 받는다.
//
//	//				if( pos )														// 위치 정보가 유효하다면,
//	//				{
//	//					QString* pTitleLine = (QString*)pTitleList->GetAt(pos) ;	// 타이틀 스트링을 받는다.
//
//	//					if( pTitleLine )											// 타이틀 스트링이 유효하다면,
//	//					{
//	//						if( pSubQuest )											// 서브 퀘스트 정보가 유효하다면,
//	//						{
//	//							DWORD dwCurCount = pSubQuest->dwData ;				// 현재 개수를 받는다.
//	//							DWORD dwMaxCount = pSubQuest->dwMaxCount ;			// 최대 개수를 받는다.
//
//	//							memset(tempBuf, 0, sizeof(tempBuf)) ;				// 임시 버퍼를 초기화 한다.
//
//	//							if( pQString->IsComplete() )
//	//							{
//	//								sprintf(tempBuf, "%s (완료)", pTitleLine->Str) ;							// 제목에 완료를 추가한다.
//
//	//								m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(0, 64, 128, 255)) ;			// 제목과 완료를 출력한다.
//	//							}
//	//							else
//	//							{
//	//								// 제목과 진행 개수를 세팅한다.
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
//	//								// 제목과 진행 개수를 리스트에 추가한다.
//	//								m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(0, 0, 255, 255)) ;
//	//							}
//
//	//							//if( dwCurCount == dwMaxCount )						// 개수가 일치 할 경우,
//	//							//{
//	//							//	if( dwCurCount > 0 )							// 현재 개수가 1개 이상이면,															
//	//							//	{
//	//							//		sprintf(tempBuf, "%s (완료)", pTitleLine->Str) ;							// 제목에 완료를 추가한다.
//
//	//							//		m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(0, 64, 128, 255)) ;			// 제목과 완료를 출력한다.
//	//							//	}
//	//							//	else											// 현재 개수가 0이거나 그 이하일 때,
//	//							//	{
//	//							//		m_pQuestDescList->AddItem(pTitleLine->Str, RGBA_MAKE(0, 64, 128, 255)) ;	// 제목만 출력한다.
//	//							//	}
//	//							//}
//	//							//else												// 개수가 일치하지 않을경우,
//	//							//{
//	//							//	if( dwCurCount > 0 )							// 현재 개수가 하나 이상이면,
//	//							//	{
//	//							//		// 제목과 진행 개수를 세팅한다.
//	//							//		if( pSubQuest->dwMaxCount == 0 )
//	//							//		{
//	//							//			sprintf(tempBuf, "%s (%d)", pTitleLine->Str, pSubQuest->dwData) ;
//	//							//		}
//	//							//		else
//	//							//		{
//	//							//			sprintf(tempBuf, "%s (%d/%d)", pTitleLine->Str, pSubQuest->dwData, pSubQuest->dwMaxCount) ;
//	//							//		}										
//
//	//							//		// 제목과 진행 개수를 리스트에 추가한다.
//	//							//		m_pQuestDescList->AddItem(tempBuf, RGBA_MAKE(0, 64, 128, 255)) ;
//	//							//	}
//	//							//}
//	//						}
//	//					}
//	//				}
//	//			}
//	//		}
//
// //           // - 서브 퀘스트 설명 세팅
//	//		pDescList = pQString->GetDesc() ;										// 서브 퀘스트 설명 리스트를 받는다.
//
//	//		if( pDescList )															// 설명 리스트 정보가 유효하다면,
//	//		{
//	//			int nDescCount = 0 ;												// 설명 개수를 받을 변수를 선언하고 0으로 세팅한다.
//
//	//			nDescCount = pDescList->GetCount() ;								// 설명 리스트의 아이템 수를 받는다.
//
//	//			for( int count2 = 0 ; count2 < nDescCount ; ++count2 )				// 설명 개수 만큼 for문을 돌린다.
//	//			{
//	//				pos = NULL ;													// 위치 정보를 담을 포인터를 null처리를 한다.
//	//				pos = pDescList->FindIndex(count2) ;							// 카운트에 해당하는 위치 정보를 받는다.
//
//	//				if( pos )														// 위치 정보가 유효하다면,
//	//				{
//	//					QString* pDescLine = (QString*)pDescList->GetAt(pos) ;  	// 설명 스트링을 받는다.
//
//	//					if( pDescLine )												// 설명 스트링 정보가 유효하다면,						
//	//					{
//	//						m_pQuestDescList->AddItem(pDescLine->Str, pDescLine->Color) ;	// 퀘스트 설명 리스트에 추가를 한다.
//	//					}
//	//				}
//	//			}
//	//		}
//
//	//		pTitleList = NULL ;														// 제목 리스트 포인터를 null처리를 한다.
//	//		pDescList  = NULL ;														// 설명 리스트 포인터를 null처리를 한다.
//	//	}
//	//	else																		// 서브 퀘스트 스트링이 없으면,
//	//	{
//	//		break ;																	// for문을 빠져나간다.
//	//	}
//
//	//	pQString = NULL ;															// 스트링 포인터를 null처리를 한다.						
//	//}
//
//	//////////////////////////////////////////
//	//// - 보상 내용 세팅 -
//	//////////////////////////////////////////
//
//	//cIcon* deleteIcon = NULL;														// 아이콘 정보를 받을 포인터를 선언하고 null처리를 한다.
//	//RequitalItem* pDeleteSQuest = NULL;												// 퀘스트 아이템 정보를 받을 포인터를 선언하고 null처리를 한다.
//	//
//	//PTRLISTSEARCHSTART(m_QuestRequitalList, RequitalItem*, pDeleteSQuest);			// 퀘스트 아이템 리스트에서 검색을 시작한다.
//	//	if(pDeleteSQuest)															// 퀘스트 아이템 정보가 유효하지 않으면, 검색을 빠져나간다.
//	//	{
//	//		m_pRequitalGrid->DeleteIcon(pDeleteSQuest->wPos, &deleteIcon);			// 퀘스트 아이콘 그리드에서 아이콘을 삭제한다.
//
//	//		if(deleteIcon)															// 아이콘 정보가 유효하지 않으면 검색을 빠져나간다.
//	//		{
//	//			deleteIcon->SetActive(FALSE);										// 아이콘을 비활성화 한다.
//	//			WINDOWMGR->AddListDestroyWindow(deleteIcon);						// 윈도우 매니져에 파괴할 윈도우에 아이콘을 추가한다.
//	//			m_IndexGenerator.ReleaseIndex(deleteIcon->GetID());					// 인덱스 생성기에서 아이콘 인덱스를 해제한다.
//	//			m_QuestRequitalList.Remove(pDeleteSQuest);							// 퀘스트 아이템 리스트에서 퀘스트 아이템 정보를 삭제한다.
//	//		}
//	//	}
//	//PTRLISTSEARCHEND																// 퀘스트 아이템 리스트 검색을 종료한다.
//
//	//RefreshRequitalItem() ;
//
//
//	//pSubQuestInfo = pQuestInfo->GetSubQuestInfo(nSubQuestCount-1) ;					// 마지막 서브 퀘스트 정보를 받는다.
//
//	//if( pSubQuestInfo )																// 서브 퀘스트 정보가 유효하다면,
//	//{
//	//	cPtrList* pTriggerList = pSubQuestInfo->GetTriggerList() ;					// 마지막 서브 퀘스트의 트리거 정보를 받는다.
//
//	//	if( pTriggerList )															// 트리거 정보가 유효하다면,
//	//	{
//	//		int nTriggerCount = 0 ;													// 트리거 카운트를 담을 변수를 선언하고, 0으로 세팅한다.
//
//	//		nTriggerCount = pTriggerList->GetCount() ;								// 트리거 카운트를 받는다.
//
//	//		CQuestTrigger* pTrigger = NULL ;										// 트리거 정보를 받을 포인터를 선언하고 null처리를 한다.
//
//	//		PTRLISTPOS exePos = NULL ;												// 실행 리스트 위치 정보를 받을 위치 포인터를 선언하고 null 처리를 한다.
//
//	//		for( int count = 0 ; count < nTriggerCount ; ++count )					// 트리거 카운트 수 만큼 for문을 돌린다.
//	//		{
//	//			pos = NULL ;														// 위치 정보를 받을 포인터를 null 처리를 한다.
//	//			pos = pTriggerList->FindIndex(count) ;								// 카운트에 해당하는 위치 정보를 받는다.
//
//	//			if( pos )															// 위치 정보가 유효하다면,
//	//			{
//	//				pTrigger = (CQuestTrigger*)pTriggerList->GetAt(pos) ;			// 위치에 해당하는 트리거 정보를 받는다.
//
//	//				if( pTrigger )													// 트리거 정보가 유효하다면,
//	//				{
//	//					cPtrList* pExeList = pTrigger->GetExeList() ;				// 트리거 실행 리스트를 받는다.
//
//	//					if( pExeList )												// 트리거 실행 리스트 정보가 유효하다면,
//	//					{
//	//						int nExeCount = pExeList->GetCount() ;					// 실행 리스트 카운트를 받는다.
//
//	//						CQuestExecute* pExecute = NULL ;						// 퀘스트 실행 정보를 받을 포인터를 선언하고 null처리를 한다.
//
//	//						for(int count2 = 0 ; count2 < nExeCount ; ++count2 )	// 실행 카운트 만큼 for문을 돌린다.
//	//						{
//	//							exePos = NULL ;										// 실행 위치 포인터를 null 처리를 한다.
//	//							exePos = pExeList->FindIndex(count2) ;				// 카운트에 해당하는 실행 위치 정보를 받는다.									
//
//	//							if( exePos )										// 퀘스트 실행 위치가 유효하면,
//	//							{
//	//								pExecute = (CQuestExecute*)pExeList->GetAt(exePos) ;	// 퀘스트 실행 정보를 받는다.
//	//
//	//								if( pExecute )									// 퀘스트 실행 정보가 유효하면,
//	//								{
//	//									CQuestExecute_Item* pExecute_Item = ((CQuestExecute_Item*)pExecute) ;
//	//									DWORD dwExeKind = pExecute->GetQuestExecuteKind() ;	// 실행 타입을 받는다.
//
//	//									switch(dwExeKind)							// 실행 타입을 확인한다.
//	//									{
//	//									case eQuestExecute_TakeItem :				// 실행 타입이 아이템을 받는 타입이라면,
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
//	//									case eQuestExecute_TakeMoney :				// 실행 타입이 골드를 받는 타입이라면,
//	//										{
//	//											SetGold( pExecute_Item->GetItemNum() ) ;
//	//										}
//	//										break ;
//
//	//									case eQuestExecute_TakeExp :				// 실행 타입이 경험치를 받는 타입이라면,
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

	RequitalItem* pSQuest = new RequitalItem;										// 퀘스트 아이템 메모리를 할당한다.
	memcpy(&pSQuest->itemInfo, pItemInfo, sizeof(ITEM_INFO)) ;						// 퀘스트 아이템 정보에 인자로 넘어온 정보를 세팅한다.
	pSQuest->nItemCount = (int)Count;												// 퀘스트 아이템의 카운트를 세팅한다.
	pSQuest->wPos = 0;																// 퀘스트 아이템의 위치를 -1로 세팅한다.

	cIcon* pIcon = new cIcon;														// 아이콘 메모리를 할당한다.
	cImage Image;																	// 이미지를 선언한다.

	SCRIPTMGR->GetImage(pSQuest->itemInfo.Image2DNum, &Image, PFT_ITEMPATH);		// 퀘스트 아이템 정보의 이미지로 이미지를 세팅한다.
	pIcon->SetBasicImage(&Image);													// 아이콘의 기본 이미지로 위에서 세팅한 이미지를 설정한다.
	pIcon->Init(0, 0, 40, 40, &Image, m_IndexGenerator.GenerateIndex() );			// 아이콘을 초기화 한다.
	pIcon->SetMovable(FALSE);														// 이동 여부를 FALSE로 세팅한다.
	pIcon->SetActive(FALSE);														// 활성화 여부를 FALSE로 세팅한다.

	pSQuest->pIcon = pIcon ;

	m_QuestRequitalList.AddTail(pSQuest);											// 퀘스트 아이템 정보를 퀘스트 아이템 리스트에 추가한다.

	WINDOWMGR->AddWindow(pIcon);													// 윈도우 매니져에 아이콘 정보를 추가한다.

	RefreshRequitalItem() ;
}

//void CQuestDialog::AddRequitalItem(ITEM_INFO* pItemInfo, DWORD Count)
//{
//	if( !pItemInfo ) return ;
//
//	RequitalItem* pSQuest = new RequitalItem;										// 퀘스트 아이템 메모리를 할당한다.
//	memcpy(&pSQuest->itemInfo, pItemInfo, sizeof(ITEM_INFO)) ;						// 퀘스트 아이템 정보에 인자로 넘어온 정보를 세팅한다.
//	pSQuest->nItemCount = (int)Count;												// 퀘스트 아이템의 카운트를 세팅한다.
//	pSQuest->wPos = 0;																// 퀘스트 아이템의 위치를 -1로 세팅한다.
//
//	m_QuestRequitalList.AddTail(pSQuest);											// 퀘스트 아이템 정보를 퀘스트 아이템 리스트에 추가한다.
//
//	// 있던 아이템 지우기.
//
//	cIcon* pIcon = new cIcon;														// 아이콘 메모리를 할당한다.
//	cImage Image;																	// 이미지를 선언한다.
//
//	SCRIPTMGR->GetImage(pSQuest->itemInfo.Image2DNum, &Image, PFT_ITEMPATH);		// 퀘스트 아이템 정보의 이미지로 이미지를 세팅한다.
//	pIcon->SetBasicImage(&Image);													// 아이콘의 기본 이미지로 위에서 세팅한 이미지를 설정한다.
//	pIcon->Init(0, 0, 40, 40, &Image, m_IndexGenerator.GenerateIndex() );			// 아이콘을 초기화 한다.
//	pIcon->SetMovable(FALSE);														// 이동 여부를 FALSE로 세팅한다.
//	pIcon->SetActive(FALSE);														// 활성화 여부를 FALSE로 세팅한다.
//
//	pSQuest->pIcon = pIcon ;
//
//	WINDOWMGR->AddWindow(pIcon);													// 윈도우 매니져에 아이콘 정보를 추가한다.
//
//	RefreshRequitalItem() ;
//}

void CQuestDialog::AddListEmptyLine()												// 빈 라인을 추가하는 함수.
{
	m_pQuestListDlg->AddItem(" ", 0);												// 리스트 다이얼로그에 빈 라인을 추가한다.
	++m_ListLineIdx;																// 리스트 라인 인덱스를 증가한다.
	m_QuestList[m_QListCount] = NULL;												// 퀘스트 리스트에, 퀘스트 리스트 카운트에 해당하는 배열을 null처리 한다.											
	++m_QListCount;																	// 퀘스트 리스트 카운트를 증가한다.
}

void CQuestDialog::GiveupQuestDelete(DWORD QuestIdx)								// 퀘스트 포기 삭제 함수.
{
//	if(QuestIdx > 100)			return;

	QuestTree* pQTree = NULL;														// 트리 정보를 받을 포인터를 선언하고 null 처리를 한다.

	PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pQTree);							// 퀘스트 트리 리스트를 검색한다.
	{
		if( pQTree )																// 퀘스트 트리 정보가 유효한지 체크한다.
		{
			if(pQTree->QuestIdx == QuestIdx)										// 퀘스트 트리 인덱스가 삭제할 인덱스와 같으면,
			{
				pQTree->list.RemoveAll();											// 트리 정보의 리스트를 모두 비운다.
				pQTree->State = eQTree_Close;										// 트리 정보의 상태를 닫혀있는 상태로 세팅한다.
				pQTree->Count = 0;													// 트리 정보의 카운트를 0으로 세팅한다.
				pQTree->QuestIdx = 0;												// 트리 정보의 퀘스트 인덱스르르 0으로 세팅한다.
				m_QuestTree.Remove(pQTree);											// 퀘스트 트리 리스트에서 트리 정보를 비운다.
				delete pQTree;														// 트리 정보를 삭제한다.
				--m_QTreeCount;														// 퀘스트 트리 카운트를 감소한다.
				break;																// 검색을 빠져나온다.
			}
		}
	}
	PTRLISTSEARCHEND																// 검색을 종료한다.

	DWORD Idx = 0;																	// 인덱스 변수를 선언하고 0으로 세팅한다.
	cIcon* pIcon = NULL;															// 아이콘 포인터를 선언하고 null처리를 한다.
	sQuestItem* pSQuest = NULL;														// 퀘스트 아이템 포인터를 선언하고 null처리를 한다.

	PTRLISTSEARCHSTART(m_QuestItemList, sQuestItem*, pSQuest);						// 퀘스트 아이템 리스트를 검색한다.
		if(!pSQuest)			break;												// 퀘스트 아이템 정보가 유효하지 않으면 검색을 빠져나간다.

		Idx = pSQuest->pQuestItemInfo->QuestKey/100;								// 퀘스트 아이템 인덱스를 받는다.

		if(Idx == QuestIdx)															// 인덱스가 포기하려는 퀘스트 인덱스와 같으면,
		{
			m_pIconGridDlg->DeleteIcon(pSQuest->Pos, &pIcon);						// 퀘스트 아이템 그리드에서 아이템을 삭제한다.

			if(!pIcon)			break;												// 아이콘 정보가 유효하지 않으면 검색을 빠져나간다.

			WINDOWMGR->AddListDestroyWindow(pIcon);									// 윈도우 매니저에 파괴할 윈도우로 아이콘을 추가한다.
			m_IndexGenerator.ReleaseIndex(pIcon->GetID());							// 인덱스 생성기에서 아이콘을 해제 한다.
			m_QuestItemList.Remove(pSQuest);										// 퀘스트 아이템 리스트에서, 아이템을 삭제한다.
			--m_QuestItemCount;														// 퀘스트 아이템 카운트를 감소한다.
		}	
	PTRLISTSEARCHEND																// 검색을 종료한다.

	QuestListReset();																// 퀘스트 리스트를 새로고침한다.
	RefreshQuestItem(m_QuestItemPage);												// 퀘스트 아이템 그리드를 새로고침한다.
}

void CQuestDialog::QuestItemAdd(QUEST_ITEM_INFO* pQuestItemInfo, DWORD Count)		// 퀘스트 아이템을 추가하는 함수.
{
	if(!pQuestItemInfo)			return;												// 퀘스트 아이템 정보가 유효하지 않으면 리턴 처리를 한다.

	// 070511 LYW --- QuestDialog : Check quest item.
	sQuestItem* pQuestItem = NULL ;													// 퀘스트 아이템 정보를 받을 포인터를 선언하고 NULL처리를 한다.

	PTRLISTSEARCHSTART(m_QuestItemList,sQuestItem*,pQuestItem)						// 퀘스트 아이템 리스트를 검색한다.
		if( pQuestItem->pQuestItemInfo->ItemIdx == pQuestItemInfo->ItemIdx )		// 퀘스트 아이템 정보의 아이템 인덱스와, 인자로 넘어온 아이템 인덱스가 같으면,
		{
			pQuestItem->Count = Count ;												// 퀘스트 아이템 리스트의 아이템 카운트를 세팅한다.
			return ;																// 리턴 처리를 한다.
		}
	PTRLISTSEARCHEND																// 퀘스트 아이템 리스트 검색을 종료한다.

	sQuestItem* pSQuest = new sQuestItem;											// 퀘스트 아이템 메모리를 할당한다.
	pSQuest->pQuestItemInfo = pQuestItemInfo;										// 퀘스트 아이템 정보에 인자로 넘어온 정보를 세팅한다.
	pSQuest->Count = Count;															// 퀘스트 아이템의 카운트를 세팅한다.
	pSQuest->Pos = -1;																// 퀘스트 아이템의 위치를 -1로 세팅한다.
	
	//
	cIcon* pIcon = new cIcon;														// 아이콘 메모리를 할당한다.
	cImage Image;																	// 이미지를 선언한다.

	SCRIPTMGR->GetImage(pSQuest->pQuestItemInfo->Image2DNum, &Image, PFT_ITEMPATH);	// 퀘스트 아이템 정보의 이미지로 이미지를 세팅한다.
	pIcon->SetBasicImage(&Image);													// 아이콘의 기본 이미지로 위에서 세팅한 이미지를 설정한다.
	pIcon->Init(0, 0, 40, 40, &Image, m_IndexGenerator.GenerateIndex() );			// 아이콘을 초기화 한다.
	pIcon->SetMovable(FALSE);														// 이동 여부를 FALSE로 세팅한다.
	pIcon->SetActive(FALSE);														// 활성화 여부를 FALSE로 세팅한다.

	pSQuest->pIcon = pIcon;															// 퀘스트 아이템 정보의 아이콘을 위에서 설정한 아이콘으로 세팅한다.

	m_QuestItemList.AddTail(pSQuest);												// 퀘스트 아이템 정보를 퀘스트 아이템 리스트에 추가한다.

	WINDOWMGR->AddWindow(pIcon);													// 윈도우 매니져에 아이콘 정보를 추가한다.
	//

	++m_QuestItemCount;																// 퀘스트 아이템 카운트를 증가한다.

	RefreshQuestItem(m_QuestItemPage);												// 퀘스트 아이템 페이지를 새로고침한다.
}


// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃														 ┃
// ┃	QuestItem											 ┃
// ┃														 ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
//void CQuestDialog::QuestItemAdd(QUEST_ITEM_INFO* pQuestItemInfo, DWORD Count)		// 퀘스트 아이템을 추가하는 함수.
//{
//	if(!pQuestItemInfo)			return;												// 퀘스트 아이템 정보가 유효하지 않으면 리턴 처리를 한다.
//
//	// 070511 LYW --- QuestDialog : Check quest item.
//	sQuestItem* pQuestItem = NULL ;													// 퀘스트 아이템 정보를 받을 포인터를 선언하고 NULL처리를 한다.
//
//	PTRLISTSEARCHSTART(m_QuestItemList,sQuestItem*,pQuestItem)						// 퀘스트 아이템 리스트를 검색한다.
//		if( pQuestItem->pQuestItemInfo->ItemIdx == pQuestItemInfo->ItemIdx )		// 퀘스트 아이템 정보의 아이템 인덱스와, 인자로 넘어온 아이템 인덱스가 같으면,
//		{
//			pQuestItem->Count = Count ;												// 퀘스트 아이템 리스트의 아이템 카운트를 세팅한다.
//			return ;																// 리턴 처리를 한다.
//		}
//	PTRLISTSEARCHEND																// 퀘스트 아이템 리스트 검색을 종료한다.
//
//	sQuestItem* pSQuest = new sQuestItem;											// 퀘스트 아이템 메모리를 할당한다.
//	pSQuest->pQuestItemInfo = pQuestItemInfo;										// 퀘스트 아이템 정보에 인자로 넘어온 정보를 세팅한다.
//	pSQuest->Count = Count;															// 퀘스트 아이템의 카운트를 세팅한다.
//	pSQuest->Pos = -1;																// 퀘스트 아이템의 위치를 -1로 세팅한다.
//
//	m_QuestItemList.AddTail(pSQuest);												// 퀘스트 아이템 정보를 퀘스트 아이템 리스트에 추가한다.
//	
//	//
//	cIcon* pIcon = new cIcon;														// 아이콘 메모리를 할당한다.
//	cImage Image;																	// 이미지를 선언한다.
//
//	SCRIPTMGR->GetImage(pSQuest->pQuestItemInfo->Image2DNum, &Image, PFT_ITEMPATH);	// 퀘스트 아이템 정보의 이미지로 이미지를 세팅한다.
//	pIcon->SetBasicImage(&Image);													// 아이콘의 기본 이미지로 위에서 세팅한 이미지를 설정한다.
//	pIcon->Init(0, 0, 40, 40, &Image, m_IndexGenerator.GenerateIndex() );			// 아이콘을 초기화 한다.
//	pIcon->SetMovable(FALSE);														// 이동 여부를 FALSE로 세팅한다.
//	pIcon->SetActive(FALSE);														// 활성화 여부를 FALSE로 세팅한다.
//
//	pSQuest->pIcon = pIcon;															// 퀘스트 아이템 정보의 아이콘을 위에서 설정한 아이콘으로 세팅한다.
//	WINDOWMGR->AddWindow(pIcon);													// 윈도우 매니져에 아이콘 정보를 추가한다.
//	//
//
//	++m_QuestItemCount;																// 퀘스트 아이템 카운트를 증가한다.
//
//	RefreshQuestItem(m_QuestItemPage);												// 퀘스트 아이템 페이지를 새로고침한다.
//}

//void CQuestDialog::QuestItemDelete(DWORD ItemIdx)									// 퀘스트 아이템을 삭제하는 함수.
//{
//	cIcon* pIcon = NULL;															// 아이콘 정보를 받을 포인터를 선언하고 null처리를 한다.
//	sQuestItem* pSQuest = NULL;														// 퀘스트 아이템 정보를 받을 포인터를 선언하고 null처리를 한다.
//	
//	PTRLISTSEARCHSTART(m_QuestItemList, sQuestItem*, pSQuest);						// 퀘스트 아이템 리스트에서 검색을 시작한다.
//		if(!pSQuest)			break;												// 퀘스트 아이템 정보가 유효하지 않으면, 검색을 빠져나간다.
//
//		//
//		if(pSQuest->pQuestItemInfo->ItemIdx == ItemIdx)								// 퀘스트 아이템 정보의 인덱스와 인자로 넘어온 인덱스가 같으면,
//		{
//			m_pIconGridDlg->DeleteIcon(pSQuest->Pos, &pIcon);						// 퀘스트 아이콘 그리드에서 아이콘을 삭제한다.
//
//			if(!pIcon)			break;												// 아이콘 정보가 유효하지 않으면 검색을 빠져나간다.
//
//			pIcon->SetActive(FALSE);												// 아이콘을 비활성화 한다.
//			WINDOWMGR->AddListDestroyWindow(pIcon);									// 윈도우 매니져에 파괴할 윈도우에 아이콘을 추가한다.
//			m_IndexGenerator.ReleaseIndex(pIcon->GetID());							// 인덱스 생성기에서 아이콘 인덱스를 해제한다.
//			m_QuestItemList.Remove(pSQuest);										// 퀘스트 아이템 리스트에서 퀘스트 아이템 정보를 삭제한다.
//			break;																	// 검색을 빠져나간다.
//		}
//	PTRLISTSEARCHEND																// 퀘스트 아이템 리스트 검색을 종료한다.
//
//	--m_QuestItemCount;																// 퀘스트 아이템 카운트를 삭제한다.
//
//	RefreshQuestItem(m_QuestItemPage);												// 퀘스트 아이템 페이지를 새로고침한다.
//}

void CQuestDialog::QuestItemDelete(DWORD ItemIdx)									// 퀘스트 아이템을 삭제하는 함수.
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

DWORD CQuestDialog::QuestItemUpdate(DWORD type, DWORD ItemIdx, DWORD data)			// 퀘스트 아이템을 업데이트 한다.
{
	sQuestItem* pSQuest = NULL;														// 퀘스트 아이템 정보를 받을 포인터를 선언하고 null처리를 한다.

	PTRLISTSEARCHSTART(m_QuestItemList, sQuestItem*, pSQuest);						// 퀘스트 아이템 리스트에서 아이템 정보를 검색한다.

		if(!pSQuest)			break;												// 퀘스트 아이템 정보가 유효하지 않으면, 검색을 빠져나간다.

		if(pSQuest->pQuestItemInfo->ItemIdx == ItemIdx)								// 퀘스트 아이템 리스트 상의 아이템 인덱스와, 인자로 들어온 아이템 인덱스가 같으면,
		{
			switch(type)															// 업데이트 타입을 확인한다.
			{
			case eQItem_AddCount:													// 퀘스트 아이템 카운트를 추가하는 타입이면,
				if(pSQuest)		pSQuest->Count += data;								// 퀘스트 아이템 정보가 유효하면, 퀘스트 아이템 정보의 카운트에 데이터를 더한다.
				break;
			case eQItem_SetCount:													// 퀘스트 아이템 카운트를 세팅하는 타입이면,
				if(pSQuest)		pSQuest->Count = data;								// 퀘스트 아이템 정보가 유효하면, 퀘스트 아이템 정보의 카운트를 데이터 값으로 세팅한다.
				break;
			case eQItem_GetCount:													// 퀘스트 아이템 카운트를 반환하는 타입이면,
				if(pSQuest)		return pSQuest->Count;								// 퀘스트 아이템 정보가 유효하면, 퀘스트 아이템 정보의 카운트를 반환한다.
				break;
			}
			break;																	// 검색을 빠져나간다.
		}
	PTRLISTSEARCHEND																// 퀘스트 아이템 리스트 검색을 종료한다.
	
	return 0;																		// 0을 리턴한다.
}

void CQuestDialog::RefreshRequitalItem()
{
	cIcon* pIcon = NULL;															// 아이콘 정보를 받을 포인터를 선언하고 null처리를 한다.

	int Count = 0;																	// 카운트 변수를 선언하고 0으로 세팅한다.
	int Pos = 0;																	// 위치 정보를 담을 변수를 선언하고 0으로 세팅한다.

	RequitalItem* pSQuest = NULL;													// 퀘스트 아이템 정보를 담을 포인터를 선언하고 null처리를 한다.

	int	nStart = 0 ;																// 시작 인덱스를 세팅한다.

	//
	for(int i=0; i<m_pRequitalGrid->GetCellNum(); i++)									// 아이콘 그리드의 셀 개수 만큼 for문을 돌린다.
	{
		m_pRequitalGrid->DeleteIcon(i, &pIcon);										// 아이콘 그리드에서 아이콘을 삭제한다.
	}														// 퀘스트 아이템 리스트 검색을 종료한다.

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
//	cIcon* pIcon = NULL;															// 아이콘 정보를 받을 포인터를 선언하고 null처리를 한다.
//
//	int Count = 0;																	// 카운트 변수를 선언하고 0으로 세팅한다.
//	int Pos = 0;																	// 위치 정보를 담을 변수를 선언하고 0으로 세팅한다.
//
//	RequitalItem* pSQuest = NULL;													// 퀘스트 아이템 정보를 담을 포인터를 선언하고 null처리를 한다.
//
//	int	nStart = 0 ;																// 시작 인덱스를 세팅한다.
//
//	//
//	for(int i=0; i<m_pRequitalGrid->GetCellNum(); i++)									// 아이콘 그리드의 셀 개수 만큼 for문을 돌린다.
//	{
//		m_pRequitalGrid->DeleteIcon(i, &pIcon);										// 아이콘 그리드에서 아이콘을 삭제한다.
//	}
//	
//	
//	PTRLISTSEARCHSTART(m_QuestRequitalList, RequitalItem*, pSQuest);				// 퀘스트 아이템 리스트에서 퀘스트 아이템 정보를 검색한다.
//
//		if(!pSQuest)			break;												// 퀘스트 아이템 정보가 유효하지 않으면, 검색을 빠져나온다.
//
//		if(Count>=nStart && Count < 6)												// 시작 인수가 카운트 값보다 작고, 페이지 당 시작 인수가 카운트 보다 작으면,
//		{
//			// Add
//			pSQuest->wPos = Pos;													// 퀘스트 아이템 정보의 위치 정보를 세팅한다.
//			m_pRequitalGrid->AddIcon(pSQuest->wPos, pSQuest->pIcon);				// 퀘스트 아이템 아이콘 그리드에, 아이콘 정보를 세팅한다.
//			pSQuest->pIcon->SetActive(TRUE);										// 퀘스트 아이템 아이콘을 활성화 한다.
//			++Pos;																	// 위치 정보를 증가한다.
//		}
//		else																		// 시작 인수 조건이 맞지 않으면,
//		{
//			pSQuest->wPos = -1;														// 퀘스트 아이템 위치 정보를 -1로 세팅한다.
//			pSQuest->pIcon->SetActive(FALSE);										// 퀘스트 아이템의 아이콘을 비활성화 시킨다.
//		}
//
//		++Count;																	// 카운트 값을 증가시킨다.
//
//	PTRLISTSEARCHEND																// 퀘스트 아이템 리스트 검색을 종료한다.
//}

void CQuestDialog::RefreshQuestItem(DWORD Page)										// 퀘스트 아이템 페이지를 새로고침하는 함수.
{	
	cIcon* pIcon = NULL;															// 아이콘 정보를 받을 포인터를 선언하고 null처리를 한다.

	int Count = 0;																	// 카운트 변수를 선언하고 0으로 세팅한다.
	int Pos = 0;																	// 위치 정보를 담을 변수를 선언하고 0으로 세팅한다.

	sQuestItem* pSQuest = NULL;														// 퀘스트 아이템 정보를 담을 포인터를 선언하고 null처리를 한다.

	int	nStart = Page*VIEW_QUESTITEM_PERPAGE;										// 시작 인덱스를 세팅한다.

	// PageButton PushState Setting
	for(int i=0; i<MAX_QUEST_PAGEBTN; ++i)											// 페이지 카운트 만큼 for문을 돌린다.
	{
		m_pPageBtn[i]->SetPush(FALSE);												// 페이지 버튼을 눌리지 않은 상태로 세팅한다.
	}
	m_pPageBtn[Page]->SetPush(TRUE);												// 인자로 넘어온 페이지 버튼을 눌린 상태로 세팅한다.

	//
	for(i=0; i<m_pIconGridDlg->GetCellNum(); i++)									// 아이콘 그리드의 셀 개수 만큼 for문을 돌린다.
	{
		m_pIconGridDlg->DeleteIcon(i, &pIcon);										// 아이콘 그리드에서 아이콘을 삭제한다.
	}
	
	
	PTRLISTSEARCHSTART(m_QuestItemList, sQuestItem*, pSQuest);						// 퀘스트 아이템 리스트에서 퀘스트 아이템 정보를 검색한다.

		if(!pSQuest)			break;												// 퀘스트 아이템 정보가 유효하지 않으면, 검색을 빠져나온다.

		if(Count>=nStart && Count < nStart+VIEW_QUESTITEM_PERPAGE)					// 시작 인수가 카운트 값보다 작고, 페이지 당 시작 인수가 카운트 보다 작으면,
		{
			// Add
			pSQuest->Pos = Pos;														// 퀘스트 아이템 정보의 위치 정보를 세팅한다.
			m_pIconGridDlg->AddIcon(pSQuest->Pos, pSQuest->pIcon);					// 퀘스트 아이템 아이콘 그리드에, 아이콘 정보를 세팅한다.
			pSQuest->pIcon->SetActive(TRUE);										// 퀘스트 아이템 아이콘을 활성화 한다.
			++Pos;																	// 위치 정보를 증가한다.
		}
		else																		// 시작 인수 조건이 맞지 않으면,
		{
			pSQuest->Pos = -1;														// 퀘스트 아이템 위치 정보를 -1로 세팅한다.
			pSQuest->pIcon->SetActive(FALSE);										// 퀘스트 아이템의 아이콘을 비활성화 시킨다.
		}

		++Count;																	// 카운트 값을 증가시킨다.

	PTRLISTSEARCHEND																// 퀘스트 아이템 리스트 검색을 종료한다.

	m_QuestItemPage = Page;															// 퀘스트 아이템 페이지를 세팅한다.
}

void CQuestDialog::ShowRequitalItemToolTip(DWORD Index)
{
	RequitalItem* pSQuest = NULL;													// 퀘스트 아이템 정보를 받을 포인터를 선언하고 null처리를 한다.

	char buf[64] = { 0, };															// 임시 버퍼를 선언한다.

	DWORD dwCount = 0;																// 카운트 변수를 선언하고 0으로 세팅한다.

	PTRLISTSEARCHSTART(m_QuestRequitalList, RequitalItem*, pSQuest);				// 퀘스트 아이템 리스트에서, 퀘스트 아이템 정보를 검색한다.

		if(!pSQuest)			break;												// 퀘스트 아이템 정보가 유효하지 않으면, 검색을 빠져나간다.

		if(dwCount == m_RequitalItemTooltipIdx)										// 툴팁 인덱스와 카운트 값이 같으면,
		{
			pSQuest->pIcon->SetToolTip("");											// 퀘스트 아이템 정보의 툴팁을 공란으로 세팅한다.
		}

		if(dwCount == Index)														// 카운트 값과, 인자로 넘어온 인덱스가 같으면,
		{
			cImage Image;															// 이미지를 선언한다.
			SCRIPTMGR->GetImage(0, &Image, PFT_HARDPATH);							// 이미지를 세팅한다.
			pSQuest->pIcon->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &Image, TTCLR_ITEM_CANEQUIP );	// 아이콘 툴팁을 공란으로 세팅한다.

			wsprintf(buf, "[%s]", pSQuest->itemInfo.ItemName);				// 임시 버퍼에 아이템 이름을 세팅한다.
			pSQuest->pIcon->AddToolTipLine(buf, ITEMMGR->GetItemNameColor( pSQuest->itemInfo ));						// 퀘스트 아이템의 툴팁 라인에 임시 버퍼를 추가한다.
			//pSQuest->pIcon->AddToolTipLine("");										// 퀘스트 아이템의 툴팁 라인에 공란을 추가한다.
			
			ITEMMGR->AddToolTip( pSQuest->pIcon, pSQuest->itemInfo.ItemTooltipIdx );

			//CItem* requitalItem = ITEMMGR->GetItem( pSQuest->itemInfo.ItemIdx );

			/*CItemShow* selectedItem = (CItemShow *)m_pIconGridDlg->GetIconForIdx( WORD( m_pIconGridDlg->GetCurSelCellPos() ) );

			ITEMMGR->SetToolTip((cIcon*)selectedItem, NULL, NULL, pSQuest->itemInfo.ItemIdx) ;*/

			//if( pTooltipStr )
			//{
			//	AddTooltipInfo(pTooltipStr->strToolTip, pSQuest->pIcon);	// 퀘스트 아이템 정보에 아이템 설명으로 툴팁을 추가한다.
			//}
			//else
			//{
			//	ASSERTMSG(0, "퀘스트 보상 아이템의 툴팁이 없음!") ;
			//}
		}

		++dwCount;																	// 카운트 값을 증가한다.

	PTRLISTSEARCHEND																// 퀘스트 아이템 리스트 검색을 종료한다.
		
	m_RequitalItemTooltipIdx = Index;												// 툴팁 인덱스를 인자로 넘어온 인덱스로 세팅한다.
}

void CQuestDialog::ShowItemToolTip(DWORD Index)										// 아이템 툴팁을 보여주는 함수.
{
	sQuestItem* pSQuest = NULL;														// 퀘스트 아이템 정보를 받을 포인터를 선언하고 null처리를 한다.

	char buf[64] = { 0, };															// 임시 버퍼를 선언한다.

	DWORD dwCount = 0;																// 카운트 변수를 선언하고 0으로 세팅한다.

	PTRLISTSEARCHSTART(m_QuestItemList, sQuestItem*, pSQuest);						// 퀘스트 아이템 리스트에서, 퀘스트 아이템 정보를 검색한다.

		if(!pSQuest)			break;												// 퀘스트 아이템 정보가 유효하지 않으면, 검색을 빠져나간다.

		if(dwCount == m_TooltipIdx)													// 툴팁 인덱스와 카운트 값이 같으면,
		{
			pSQuest->pIcon->SetToolTip("");											// 퀘스트 아이템 정보의 툴팁을 공란으로 세팅한다.
		}

		if(dwCount == Index)														// 카운트 값과, 인자로 넘어온 인덱스가 같으면,
		{
			cImage Image;															// 이미지를 선언한다.
			SCRIPTMGR->GetImage(0, &Image, PFT_HARDPATH);							// 이미지를 세팅한다.
			pSQuest->pIcon->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &Image, TTCLR_ITEM_CANEQUIP );	// 아이콘 툴팁을 공란으로 세팅한다.

			wsprintf(buf, "[%s]", pSQuest->pQuestItemInfo->ItemName);				// 임시 버퍼에 아이템 이름을 세팅한다.
			pSQuest->pIcon->AddToolTipLine(buf, TTTC_DEFAULT);						// 퀘스트 아이템의 툴팁 라인에 임시 버퍼를 추가한다.
			pSQuest->pIcon->AddToolTipLine("");										// 퀘스트 아이템의 툴팁 라인에 공란을 추가한다.
			AddTooltipInfo(pSQuest->pQuestItemInfo->ItemDesc, pSQuest->pIcon);		// 퀘스트 아이템 정보에 아이템 설명으로 툴팁을 추가한다.
		}

		++dwCount;																	// 카운트 값을 증가한다.

	PTRLISTSEARCHEND																// 퀘스트 아이템 리스트 검색을 종료한다.
		
	m_TooltipIdx = Index;															// 툴팁 인덱스를 인자로 넘어온 인덱스로 세팅한다.
}

void CQuestDialog::AddTooltipInfo(char* pString, cIcon* pIcon)						// 툴팁 정보를 추가하는 함수.
{
	if(strlen(pString) > MAX_ITEMDESC_LINE)											// 최대 아이템 설명 라인보다 스트링 길이가 크면,
	{
		char* pStr = pString;														// 문자열 포인터를 선언하고 스트링 정보를 받는다.
		char tb[MAX_ITEMDESC_LINE+2] = { 0,  };										// 임시 버퍼를 선언한다.
		int len = 0;																// 길이를 0으로 세팅한다.
		
		while(*pStr)																// 문자열 정보가 유효할 동안 while문을 돌린다.
		{						
			tb[len]=*pStr;															// 임시 버퍼의 길이에 글자를 담는다.

			if( pStr + 1 != CharNext( CharPrev( pStr, pStr + 1 ) ) )				// 현재 글자의 다음 글자를 체크하여 2바이트 글자면, 
				tb[++len] = *(++pStr);												// 한 바이트를 더 담는다.

			++len;																	// 길이 값을 증가한다.
			++pStr;																	// 문자 포인터를 증가한다.
			
			// 일단은 그냥 막자.. ㅡ.ㅡ;;
			if(tb[0] == ' ')		{	len = 0;	}								// 임시 버퍼의 첫 바이트가 빈칸이면, 길이를 0으로 세팅한다.
			
			if(strlen(tb) >= MAX_ITEMDESC_LINE)										// 임시 버퍼 길이가 최대 아이템 설명 라인보다 크면,
			{
				pIcon->AddToolTipLine(tb, TTTC_DESCRIPTION);						// 임시 버퍼를 툴팁 라인에 추가한다.
				memset(tb, 0, MAX_ITEMDESC_LINE+2);									// 임시 버퍼를 메모리 셋한다.
				len = 0;															// 길이를 0으로 세팅한다.
			}
		}

		if(len)																		// 길이 정보가 있으면,
		{
			pIcon->AddToolTipLine(tb, TTTC_DESCRIPTION);							// 아이콘에 툴팁을 추가한다.
			memset(tb, 0, MAX_ITEMDESC_LINE+2);										// 임시 버퍼를 메모리 셋한다.
			len = 0;																// 길이 값을 0으로 세팅한다.
		}
	}
	else																			// 최대 아이템 설명 라인보다 스트링 길이가 작으면,
		pIcon->AddToolTipLine(pString, TTTC_DESCRIPTION);							// 아이콘에 툴팁을 추가한다.
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
	QuestTree* pData = new QuestTree;									// 트리를 생성한다.

	CQuestString* pQuestString = new CQuestString;						// 트리에 담을 퀘스트 스트링을 생성한다.

	int nTitleLine = 0 ;												// 타이틀 라인을 담을 변수를 선언하고 0으로 세팅한다.
	pQuestString->AddLine(GetMapName(nMapNum), nTitleLine, TRUE) ;		// 퀘스트 스트링에 맵 이름을 타이틀로 추가한다.
	pQuestString->SetIndex(nMapNum, 0);									// 트리 인덱스를 맵 번호의 0으로 세팅한다.
	pQuestString->SetTreeTitle(TRUE) ;									// 트리의 제목으로 세팅한다.
	pData->list.AddHead(pQuestString);									// 새로 생성한 트리에 맵 타이틀을 추가한다.
	++pData->Count;														// 트리의 스트링 카운트를 증가한다.
	pData->State = eQTree_Open ;										// 트리의 상태를 닫힌 상태로 세팅한다.
	pData->nMapNum = nMapNum ;											// 트리의 맵 번호를 세팅한다.

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
	CQuestInfo* pQuestInfo =NULL ;														// 퀘스트 정보를 받을 포인터를 선언하고 null 처리를 한다.
	pQuestInfo = QUESTMGR->GetQuestInfo(dwQuestIdx) ;									// 퀘스트 인덱스로 퀘스트 정보를 받는다.

	if(!pQuestInfo)																		// 퀘스트 정보가 유효하지 않으면,
	{
		/*char tempBuf[128] = {0, } ;
		sprintf(tempBuf, "[CheckMapTree] - (%d)번 퀘스트 정보를 받지 못해서 퀘스트를 추가하지 못했습니다.", dwQuestIdx) ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;
	}

	int nMapNum = pQuestInfo->GetMapNum() ;												// 퀘스트 정보에 있는 맵 번호를 받는다.

	QuestTree* pTree ;																	// 퀘스트 트리 정보를 받을 포인터를 선언한다.
	
	PTRLISTPOS pos = NULL ;																// 트리의 위치를 받을 포인터를 선언하고 null 처리를 한다.
	pos = m_QuestTree.GetHeadPosition() ;												// 퀘스트 트리 리스트를 헤드로 세팅하고 위치를 받는다.

	while(pos)																			// 트리 위치 정보가 유효할 동안 whlie문을 돌린다.
	{
		pTree = NULL ;																	// 트리 정보를 받을 포인터를 null 처리를 한다.
		pTree = (QuestTree*)m_QuestTree.GetNext(pos) ;									// 현재 위치의 트리 정보를 받고, 위치를 다음으로 세팅한다.

		if( !pTree ) continue ;															// 트리 정보가 유효하지 않으면 continue 처리를 한다.

		if( pTree->nMapNum != nMapNum ) continue ;										// 현재 트리의 맵 번호와, 비교대상의 맵 번호가 다르면, continue 처리를 한다.

		BOOL b_SameQuestIdx = FALSE ;													// 트리 내에서 같은 인덱스가 있는지 결과값을 담을 변수를 선언하고 false로 세팅한다.

		DWORD* pQuestIdx ;																// 퀘스트 인덱스 번호를 받을 포인터를 선언한다.

		PTRLISTPOS idPos = NULL ;														// 퀘스트 인덱스 위치를 받을 포인터를 선언하고 null 처리를 한다.
		idPos = pTree->list.GetHeadPosition() ;											// 현재 트리의 리스트를 헤드로 세팅하고, 인덱스 위치를 받는다.

		while(idPos)																	// 인덱스 위치 정보가 유효할 동안 while문을 돌린다.
		{
			pQuestIdx = NULL ;															// 퀘스트 인덱스를 받을 포인터를 null 처리를 한다.
			pQuestIdx = (DWORD*)pTree->list.GetNext(idPos) ;							// 현재 위치의 퀘스트 인덱스를 받고, 위치를 다음으로 세팅한다.

			if( !pQuestIdx ) continue ;													// 퀘스트 인덱스 정보가 유효하지 않으면 continue 처리를 한다.

			if( *pQuestIdx == dwQuestIdx )												// 퀘스트 인덱스가 비교 대상의 퀘스트 인덱스와 같으면,
			{
				b_SameQuestIdx = TRUE ;													// 결과 값을 true로 세팅한다.
				break ;																	// 현재 while문을 탈출한다.
			}
		}

		if( b_SameQuestIdx )															// 결과 값이 true 이면,
		{
			/*char tempBuf[128] = {0, } ;
			sprintf(tempBuf, "[CheckMapTree] - (%d)번 맵 트리에 (%d)번 퀘스트가 이미 존재하기 때문에, 퀘스트를 추가하지 못했습니다.", nMapNum, dwQuestIdx) ;
			CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
			return ;
		}
		else																			// 같은 퀘스트가 없다면,
		{
			DWORD* pNewQuest = new DWORD ;												// 퀘스트 인덱스를 담을 변수를 생성한다.
			*pNewQuest = dwQuestIdx ;													// 퀘스트 인덱스 값을 세팅한다.

			pTree->list.AddTail(pNewQuest) ;											// 현재 맵 트리에 퀘스트 인덱스를 추가한다.

			++m_nQuestCount ;

			//CQuestString* pQString = NULL ;
			//pQString = QUESTMGR->GetQuestString(dwQuestIdx, 0) ;

			//cPtrList* p = pQString->GetTitle();												// 리스트 포인터를 선언하여, 제목을 받는다.
			//ITEM* string = (ITEM*)p->GetHead();										// 리스트의 헤드를 퀘스트 스트링 포인터에 받는다.

			//CHATMGR->AddMsg( CTC_QUEST, CHATMGR->GetChatMsg(221), string->string );			// 퀘스트를 시작한다는 메시지를 출력한다.

			return ;
		}
	}

	AddMapTree(nMapNum, dwQuestIdx) ;													// 새로운 맵 트리를 추가하고, 퀘스트도 추가한다.

	RefreshQuestList() ;																// 퀘스트 리스트를 새로고침한다.
}





void CQuestDialog::EndQuest(DWORD dwQuestIdx)
{
	CQuestInfo* pQuestInfo =NULL ;														// 퀘스트 정보를 받을 포인터를 선언하고 null 처리를 한다.
	pQuestInfo = QUESTMGR->GetQuestInfo(dwQuestIdx) ;									// 퀘스트 인덱스로 퀘스트 정보를 받는다.

	if(!pQuestInfo)																		// 퀘스트 정보가 유효하지 않으면,
	{
		/*char tempBuf[128] = {0, } ;
		sprintf(tempBuf, "[CheckMapTree] - (%d)번 퀘스트 정보를 받지 못해서 퀘스트를 추가하지 못했습니다.", dwQuestIdx) ;
		CHATMGR->AddMsg(CTC_QUEST, tempBuf) ;*/
		return ;
	}

	int nMapNum = pQuestInfo->GetMapNum() ;												// 퀘스트 정보에 있는 맵 번호를 받는다.

	QuestTree* pTree ;																	// 퀘스트 트리 정보를 받을 포인터를 선언한다.
	
	PTRLISTPOS pos = NULL ;																// 트리의 위치를 받을 포인터를 선언하고 null 처리를 한다.
	pos = m_QuestTree.GetHeadPosition() ;												// 퀘스트 트리 리스트를 헤드로 세팅하고 위치를 받는다.

	while(pos)																			// 트리 위치 정보가 유효할 동안 whlie문을 돌린다.
	{
		pTree = NULL ;																	// 트리 정보를 받을 포인터를 null 처리를 한다.
		pTree = (QuestTree*)m_QuestTree.GetNext(pos) ;									// 현재 위치의 트리 정보를 받고, 위치를 다음으로 세팅한다.

		if( !pTree ) continue ;															// 트리 정보가 유효하지 않으면 continue 처리를 한다.

		if( pTree->nMapNum != nMapNum ) continue ;										// 현재 트리의 맵 번호와, 비교대상의 맵 번호가 다르면, continue 처리를 한다.

		DWORD* pQuestIdx ;																// 퀘스트 인덱스 번호를 받을 포인터를 선언한다.

		PTRLISTPOS idPos = NULL ;														// 퀘스트 인덱스 위치를 받을 포인터를 선언하고 null 처리를 한다.
		idPos = pTree->list.GetHeadPosition() ;											// 현재 트리의 리스트를 헤드로 세팅하고, 인덱스 위치를 받는다.

		while(idPos)																	// 인덱스 위치 정보가 유효할 동안 while문을 돌린다.
		{
			pQuestIdx = NULL ;															// 퀘스트 인덱스를 받을 포인터를 null 처리를 한다.
			pQuestIdx = (DWORD*)pTree->list.GetNext(idPos) ;							// 현재 위치의 퀘스트 인덱스를 받고, 위치를 다음으로 세팅한다.

			if( !pQuestIdx ) continue ;													// 퀘스트 인덱스 정보가 유효하지 않으면 continue 처리를 한다.

			if( *pQuestIdx == dwQuestIdx )												// 퀘스트 인덱스가 비교 대상의 퀘스트 인덱스와 같으면,
			{
				pTree->list.Remove(pQuestIdx) ;
				delete pQuestIdx ;
			}
		}

		if( pTree->list.GetCount() < 1 ) DeleteMapTree(nMapNum) ;
	}

	RefreshQuestList() ;																// 퀘스트 리스트를 새로고침한다.

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
	QuestTree* pTree ;																	// 퀘스트 트리 정보를 받을 포인터를 선언한다.
	
	PTRLISTPOS pos = NULL ;																// 트리의 위치를 받을 포인터를 선언하고 null 처리를 한다.
	pos = m_QuestTree.GetHeadPosition() ;												// 퀘스트 트리 리스트를 헤드로 세팅하고 위치를 받는다.

	while(pos)																			// 트리 위치 정보가 유효할 동안 whlie문을 돌린다.
	{
		pTree = NULL ;																	// 트리 정보를 받을 포인터를 null 처리를 한다.
		pTree = (QuestTree*)m_QuestTree.GetNext(pos) ;									// 현재 위치의 트리 정보를 받고, 위치를 다음으로 세팅한다.

		if( !pTree ) continue ;															// 트리 정보가 유효하지 않으면 continue 처리를 한다.

		if( pTree->nMapNum != nMapNum ) continue ;										// 현재 트리의 맵 번호와, 비교대상의 맵 번호가 다르면, continue 처리를 한다.

		DWORD* pQuestIdx ;																// 퀘스트 인덱스 번호를 받을 포인터를 선언한다.

		PTRLISTPOS idPos = NULL ;														// 퀘스트 인덱스 위치를 받을 포인터를 선언하고 null 처리를 한다.
		idPos = pTree->list.GetHeadPosition() ;											// 현재 트리의 리스트를 헤드로 세팅하고, 인덱스 위치를 받는다.

		while(idPos)																	// 인덱스 위치 정보가 유효할 동안 while문을 돌린다.
		{
			pQuestIdx = NULL ;															// 퀘스트 인덱스를 받을 포인터를 null 처리를 한다.
			pQuestIdx = (DWORD*)pTree->list.GetNext(idPos) ;							// 현재 위치의 퀘스트 인덱스를 받고, 위치를 다음으로 세팅한다.

			if( !pQuestIdx ) continue ;													// 퀘스트 인덱스 정보가 유효하지 않으면 continue 처리를 한다.

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
	char buf[256] = { 0, };																// 임시 버퍼를 선언한다.	

	int BackupLine = m_pQuestListDlg->GetTopListItemIdx();							// 백업 라인을 세팅한다.

	CQuestQuickViewDialog* pQuickView = NULL ;
	pQuickView = GAMEIN->GetQuestQuickViewDialog() ;

	if( !pQuickView )
	{
		/*sprintf(buf, "퀘스트 알림창 정보를 받아오지 못해 퀘스트 리스트를 새로고침 하지 못했습니다.") ;
		CHATMGR->AddMsg(CTC_QUEST, buf) ;*/
		return ;
	}

	DWORD* pCheckIdx ;
	PTRLISTPOS checkPos ;																// 퀘스트 알림창의 위치 정보를 받을 포인터를 선언한다.

	cPtrList* pCheckList = NULL ;
	pCheckList = pQuickView->GetViewList() ;

	if( !pCheckList )
	{
		/*sprintf(buf, "퀘스트 알림창의 리스트 정보를 받아오지 못해 퀘스트 리스트를 새로고침 하지 못했습니다.") ;
		CHATMGR->AddMsg(CTC_QUEST, buf) ;*/
		return ;
	}

	m_pQuestListDlg->RemoveAll();														// 퀘스트 리스트 다이얼로그를 모두 비운다.

	m_QListCount = 0 ;
	memset(m_dwQuestList, 0, sizeof(CQuestString*)*MAX_PROCESSQUEST);					// 퀘스트 리스트를 메모리 셋 한다.
	
	DWORD Color = 0;																	// 색상 값을 담을 변수를 선언하고 0으로 세팅한다.

	QuestTree* pTree ;																	// 퀘스트 트리 정보를 받을 포인터를 선언한다.
	PTRLISTPOS pos ;																	// 트리의 위치를 받을 포인터를 선언한다.

	char* pMapName ;																	// 맵 이름을 받을 포인터를 선언한다.

	DWORD* pQuestIdx ;																	// 퀘스트 인덱스를 받을 포인터를 선언한다.
	CQuestInfo* pQuestInfo ;															// 퀘스트 정보를 받을 포인터를 선언한다.

	PTRLISTPOS idPos ;																	// 인덱스 위치 정보를 받을 포인터를 선언한다.
	CQuestString* pQuestString ;														// 퀘스트 스트링 정보를 받을 포인터를 선언한다.
	
	pos = NULL ;																		// 트리의 위치를 받을 포인터를 null 처리를 한다.
	pos = m_QuestTree.GetHeadPosition() ;												// 퀘스트 트리 리스트를 헤드로 세팅하고 위치를 받는다.

	while(pos)																			// 트리 위치 정보가 유효할 동안 whlie문을 돌린다.
	{
		pTree = NULL ;																	// 트리 정보를 받을 포인터를 null 처리를 한다.
		pTree = (QuestTree*)m_QuestTree.GetNext(pos) ;									// 현재 위치의 트리 정보를 받고, 위치를 다음으로 세팅한다.

		if( !pTree ) continue ;															// 트리 정보가 유효하지 않으면 continue 처리를 한다.

		if( pTree->list.GetCount() < 1 )
		{
			/*sprintf(buf, "(%d)번 맵의 등록된 퀘스트가 없어서, (%d)번 맵 퀘스트를 추가하지 못했습니다.", pTree->nMapNum) ;
			CHATMGR->AddMsg(CTC_QUEST, buf) ;*/
			continue ;
		}

		pMapName = NULL ;																// 맵 이름을 담을 포인터를 null 처리를 한다.
		pMapName = GetMapName(pTree->nMapNum) ;											// 맵 번호에 따른 맵 이름을 받는다.	

		if( !pMapName )
		{
			/*sprintf(buf, "(%d)번 맵 이름을 받아오지 못해 (%d)번 맵 퀘스트를 추가하지 못했습니다.", pTree->nMapNum) ;
			CHATMGR->AddMsg(CTC_QUEST, buf) ;*/
			continue ;
		}

		Color = RGBA_MAKE(255, 150, 0, 255) ;											// 맵 트리의 색상을 세팅한다.	
		sprintf(buf, "* ") ;															// 임시 버퍼를 *로 세팅한다.
		strcat(buf, pMapName) ;															// 임시 버퍼에 맵 이름을 추가한다.

		m_pQuestListDlg->AddItemWithFont(buf, Color, cFont::FONT1) ;							// 퀘스트 리스트에 맵 이름을 추가한다.
		//m_pQuestListDlg->AddItem(buf, Color) ;											// 퀘스트 리스트에 맵 이름을 추가한다.

		m_dwQuestList[m_QListCount] = pTree->nMapNum ;									// 퀘스트 리스트에 퀘스트 스트링 정보를 대입한다.
		++m_QListCount;																	// 퀘스트 리스트 카운트를 증가한다.

		if( pTree->State == eQTree_Close ) continue ;									// 트리가 닫혀진 상태라면, continue 처리를 한다.

		idPos = NULL ;																	// 인덱스 위치 정보를 받을 포인터 null 처리를 한다.
		idPos = pTree->list.GetHeadPosition() ;											// 현재 맵 트리의 리스트를 헤드로 세팅하고 위치를 받는다.

		while(idPos)																	// 위치 정보가 유효할 동안 while문을 돌린다.
		{
			pQuestIdx = NULL ;															// 퀘스트 인덱스를 받을 포인터를 null 처리를 한다.
			pQuestIdx = (DWORD*)pTree->list.GetNext(idPos) ;							// 현재 위치의 퀘스트 인덱스를 받고, 위치를 다음으로 세팅한다.

			if( !pQuestIdx ) continue ;													// 퀘스트 인덱스가 유효하지 않으면 continue 처리를 한다.

			memset(buf, 0, sizeof(buf)) ;												// 임시 버퍼를 메모리 셋한다.

			pQuestString = NULL ;														// 퀘스트 스트링 정보를 받을 포인터를 null 처리를 한다.
			pQuestString = QUESTMGR->GetQuestString(*pQuestIdx, 0) ;					// 현재 퀘스트의 제목 스트링을 받아온다.

			if( !pQuestString )															// 퀘스트 스트링 정보가 유효하지 않으면,
			{
				/*sprintf(buf, "(%d)번 퀘스트 정보를 받아오지 못해 (%d)번 퀘스트 제목을 추가하지 못했습니다.", pTree->nMapNum) ;
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
				sprintf(buf, "	- ");												// 임시 버퍼를 -로 세팅한다.
			}
			else
			{
				sprintf(buf, "	√");												// 임시 버퍼를 -로 세팅한다.
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
			pQuestInfo = QUESTMGR->GetQuestInfo(pQuestString->GetQuestID()) ;				// 퀘스트 번호로 퀘스트 정보를 받는다.

			if( pQuestInfo )
			{
				nSubQuestCount = pQuestInfo->GetSubQuestCount() ;							// 서브 퀘스트 수를 받는다.
			}

			if( nSubQuestCount-1 == pQuest->GetCurSubQuestIdx() )
			{
				strcat(buf, CHATMGR->GetChatMsg(1463)) ;
			}

			m_pQuestListDlg->AddItemWithFont(buf, Color, cFont::FONT0) ;							// 퀘스트 리스트에 맵 이름을 추가한다.
			//m_pQuestListDlg->AddItem(buf, Color) ;											// 퀘스트 리스트에 맵 이름을 추가한다.

			m_dwQuestList[m_QListCount] = *pQuestIdx ;										// 퀘스트 리스트에 퀘스트 스트링 정보를 대입한다.
			++m_QListCount;																	// 퀘스트 리스트 카운트를 증가한다.
		}
	}

	m_pQuestListDlg->ResetGuageBarPos() ;
	m_pQuestListDlg->SetTopListItemIdx(BackupLine);								// 아이템의 툴팁 인덱스를 세팅한다.

	SetQuestCount() ;

	CQuestString* pCurString = NULL ;
	pCurString = QUESTMGR->GetQuestString(m_SelectedQuestID, 0) ;
	// 071012 LYW --- QuestDialog : Modified.
	//QuestDescView(pCurString) ;	// 퀘스트가 없을때, 마지막으로 표시 했던 설명을 지우기위해 null을 추가한다.
	if( m_nQuestCount <= 0 ) 
	{
		QuestDescView(NULL) ;
	}
	else
	{
		QuestDescView(pCurString) ;
	}
}



























