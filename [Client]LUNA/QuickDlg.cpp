#include "stdafx.h"
#include "WindowIDEnum.h"
#include "./Interface/cStatic.h"
#include ".\quickdlg.h"
#include ".\quickslotdlg.h"
#include "./interface/cWindowManager.h"

cQuickDlg::cQuickDlg(void)
{
	m_type = WT_QUICKDIALOG;
}

cQuickDlg::~cQuickDlg(void)
{
//	if(!m_pComponentList) return;
/*	
	PTRLISTPOS pos = m_pComponentList->GetHeadPosition();
	while(pos)
	{
		cWindow* window = (cWindow *)m_pComponentList->GetNext(pos);

		if( window == mpSlot[ 0 ] || window == mpSlot[ 1 ] )
		{
			continue;
		}

		SAFE_DELETE(window);
	}	

	m_pComponentList->RemoveAll();
	SAFE_DELETE(m_pComponentList);
*/

}

void cQuickDlg::Linking()
{
	mpExtendBtn = ( cButton* )GetWindowForID(QI_QUICK2BTN) ;

	mpSlot = ( cQuickSlotDlg* )GetWindowForID( QI1_QUICKSLOTDLG );

	WINDOWMGR->AddWindow( ( cWindow* )mpSlot );

	m_pComponentList.Remove( ( void* )mpSlot );

	mpSlot->Linking();

	// 080702 LYW --- QuickDlg : 확장 슬롯 추가 처리.
	mpExSlot = ( cQuickSlotDlg* )GetWindowForID( QI2_QUICKSLOTDLG ) ;

	WINDOWMGR->AddWindow( ( cWindow* )mpExSlot ) ;

	m_pComponentList.Remove( (void*)mpExSlot ) ;

	mpExSlot->Linking() ;
}

void cQuickDlg::SetActiveSheet( WORD page1, WORD page2 )
{
	mpSlot->SelectPage( page1 );

	// 080702 LYW --- QuickDlg : 확장 슬롯 추가 처리.
	mpExSlot->SelectPage( page2 ) ;
}

void cQuickDlg::OnActionEvnet(LONG lId, void * p, DWORD we)
{
	//// 080702 LYW --- QuickDlg : 확장 슬롯 추가 처리.
	//if( p == mpSlot )
	//{
	//	mpSlot->OnActionEvnet( lId, p, we );
	//}

	if( ((cQuickSlotDlg*)p)->GetID() == QI1_QUICKSLOTDLG )
	{
		mpSlot->OnActionEvnet( lId, p, we );
	}
	else if( ((cQuickSlotDlg*)p)->GetID() == QI2_QUICKSLOTDLG )
	{
		mpExSlot->OnActionEvnet( lId, p, we );
	}
	
	if(lId == QI_QUICK2BTN)
	{
		Toggle_ExSlot() ;
	}
}

cQuickSlotDlg* cQuickDlg::GetSlotDlg( WORD num )
{
	return mpSlot;
}

// 080702 LYW --- QuickDlg : 확장 슬롯 추가 처리.
cQuickSlotDlg* cQuickDlg::GetExSlotDlg( WORD num )
{
	return mpExSlot ;
}

// 080703 LYW --- QuickDlg : 확장 슬롯 토글 함수 추가.
void cQuickDlg::Toggle_ExSlot()
{
	mpExSlot->SetActive(!mpExSlot->IsActive()) ;
}
