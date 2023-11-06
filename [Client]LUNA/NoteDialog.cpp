// NoteDialog.cpp: implementation of the CNoteDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NoteDialog.h"
#include "WindowIDEnum.h"
#include ".\Interface\cWindowManager.h"
#include "NoteManager.h"
#include "GameIn.h"
// 061220 LYW --- Delete this code.
//#include "MainBarDialog.h"
#include "cRitemEx.h"

// 071022 LYW --- NoteDialog : Include header file of tutorial manager.
#include "TutorialManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNoteDialog::CNoteDialog()
{
	m_type = WT_NOTEDLG;
	m_SelectedNotePge = 1;
	m_CurNoteMode = eMode_NormalNote;
	m_CurNoteID = 0;
}

CNoteDialog::~CNoteDialog()
{

}

void CNoteDialog::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cDialog::Init(x,y,wid,hei,basicImage,ID);
	m_type = WT_NOTEDLG;	
}

void CNoteDialog::Linking()
{
	//////////////////////////////////////////////////////////////////////////
	//eNoteMode_List
	m_pWriteNoteBtn = (cButton*)GetWindowForID(NOTE_WRITENOTEBTN);
	m_pDelNoteBtn = (cButton*)GetWindowForID(NOTE_DELNOTEBTN);
	m_pNoteBtn = (cPushupButton*)GetWindowForID(NOTE_TABBTN1);
	m_pPsNoteBtn = (cPushupButton*)GetWindowForID(NOTE_TABBTN2);

	m_pNoteListLCtrl = (cListCtrl*)GetWindowForID(NOTE_NOTELISTLCTL);
	for(int i=0; i<NOTENUM_PER_PAGE; ++i)
	{
		m_pNoteChk[i] = (cCheckBox*)GetWindowForID(NOTE_NOTELISTCHK1+i);
	}
	m_pNoteChkAll = (cCheckBox*)GetWindowForID(NOTE_NOTELISTCHK11);

	for(i=0; i<MAX_NOTE_PAGE; ++i)
	{
		m_pNotePageBtn[i] = (cPushupButton*)GetWindowForID(NOTE_PAGE1BTN+i);
	}

	SetMode( eMode_NormalNote );

	//////////////////////////////////////////////////////////////////////////
}

void CNoteDialog::SetActive( BOOL val )
{
	cDialog::SetActive( val );

//--- main bar icon
	// 061220 LYW --- Delete this code.
	/*
	CMainBarDialog* pDlg = GAMEIN->GetMainInterfaceDialog();
	if( pDlg )
	{
		pDlg->SetPushBarIcon( OPT_NOTEDLGICON, m_bActive );
		if( val == TRUE )
		{
			pDlg->SetAlram( OPT_NOTEDLGICON, FALSE );
		}
	}
	*/
			
	if( !val )
		SetMode( eMode_NormalNote );
	else
	{
		if( m_pNoteChkAll )
			m_pNoteChkAll->SetChecked( FALSE );
	}

	// 071022 LYW --- NoteDialog : Check open dialog.
	TUTORIALMGR->Check_OpenDialog(this->GetID(), val) ;
}


void CNoteDialog::SetMode(WORD mode)
{
	m_CurNoteMode = mode;
	
	if( mode == eMode_NormalNote )
	{
#ifndef TAIWAN_LOCAL
		m_pNoteBtn->SetPush( TRUE );
		m_pPsNoteBtn->SetPush( FALSE );
#endif
	}
	else
	{
#ifndef TAIWAN_LOCAL
		m_pNoteBtn->SetPush( FALSE );
		m_pPsNoteBtn->SetPush( TRUE );
#endif
	}
}


void CNoteDialog::SetNoteList(MSG_FRIEND_NOTE_LIST* pmsg)
{
	// 쪽지를 읽을 때 새로고침하기 위해 저장한다
	mNoteList = *pmsg;

	ShowNotePageBtn(pmsg->TotalPage);

	m_pNoteListLCtrl->DeleteAllItems();
	
	for(int i=0; i<NOTENUM_PER_PAGE; ++i)
	{
		m_pNoteChk[i]->SetChecked(FALSE);
		m_pNoteChk[i]->SetActive(FALSE);

		if(pmsg->NoteList[i].NoteID != 0)
		{
			cRITEMEx *ritem = new cRITEMEx(2);
			
			//이름 6글자 까지 보여준다. 
			char Name[13] = {0,};
			int Len = strlen( pmsg->NoteList[i].FromName ); 
			if(Len > 12)
			{
				strncpy( Name, pmsg->NoteList[i].FromName, 12 );
				Name[12] = 0;
				strcpy(ritem->pString[0],Name);
			}
			else 
				strcpy(ritem->pString[0],pmsg->NoteList[i].FromName);

			sprintf(ritem->pString[1], pmsg->NoteList[i].SendDate);
			
			DWORD color = RGBA_MAKE( 255, 255, 255, 0 );
			if( pmsg->NoteList[i].bIsRead )
				color = RGBA_MAKE( 255, 255, 0, 0 );

			for( int r=0; r<2; r++)
				ritem->rgb[r] = color;

			ritem->dwID = pmsg->NoteList[i].NoteID; 
			m_pNoteListLCtrl->InsertItem(i, ritem);
			m_pNoteChk[i]->SetActive(TRUE);
		}
	}

	SetActive(TRUE);
}


void CNoteDialog::Refresh( DWORD noteID )
{
	m_pNoteListLCtrl->DeleteAllItems();

	for(int i=0; i<NOTENUM_PER_PAGE; ++i)
	{
		cCheckBox* checkBox = m_pNoteChk[i];

		FRIEND_NOTE& note = mNoteList.NoteList[ i ];

		if( ! note.NoteID )
		{
			checkBox->SetChecked(FALSE);
			checkBox->SetActive(FALSE);

			break;
		}
		else if( note.NoteID == noteID )
		{
			note.bIsRead = TRUE;
		}

		checkBox->SetActive(TRUE);

		cRITEMEx *ritem = new cRITEMEx(2);

		char Name[13] = {0,};
		int Len = strlen( note.FromName ); 
		if(Len > 12)
		{
			strncpy( Name, note.FromName, 12 );
			Name[12] = 0;
			strcpy(ritem->pString[0],Name);
		}
		else 
		{
			strcpy(ritem->pString[0],note.FromName);
		}

		sprintf(ritem->pString[1], note.SendDate);

		{
			const DWORD color = note.bIsRead ? RGBA_MAKE( 255, 255 ,0 ,0 ) : RGBA_MAKE( 255, 255, 255, 0 );

			ritem->rgb[ 0 ] = color;
			ritem->rgb[ 1 ] = color;
		}

		ritem->dwID = note.NoteID; 
		m_pNoteListLCtrl->InsertItem( i, ritem);
	}
}


//void CNoteDialog::RefreshDlg()
//{
//
//}

void CNoteDialog::CheckedNoteID()
{
	DWORD IDs[NOTENUM_PER_PAGE];
	SelectedNoteIDChk(NOTENUM_PER_PAGE,NOTE_NOTELISTCHK1,m_pNoteListLCtrl,IDs);	
	
	int lastidx = 0;
	for(int i=0;i<NOTENUM_PER_PAGE;++i)
	{
		if(IDs[i] != 0)
			lastidx = i;
	}
	
	for(i=0;i<NOTENUM_PER_PAGE;++i)
	{
		if(IDs[i] != 0)
		{
			BOOL bLast;
			if(i==lastidx)
				bLast = TRUE;
			else
				bLast = FALSE;
			NOTEMGR->DelNoteSyn(IDs[i],bLast);
		}
	} 
}

void CNoteDialog::SelectedNoteIDChk(int count, LONG ChkBtnIDStar,cListCtrl* pListCtrl,DWORD* IDs)
{
	cCheckBox* pChkArray[NOTENUM_PER_PAGE];
	for(int i=0; i<count; ++i)
	{
		pChkArray[i] = (cCheckBox*)GetWindowForID(ChkBtnIDStar+i);
	}
	GetCheckedNoteIDs(count,pChkArray,pListCtrl,IDs);
}


void CNoteDialog::GetCheckedNoteIDs(int count,cCheckBox** pChkBtnArray,cListCtrl* pListCtrl,DWORD* pOutIDs)
{
	memset(pOutIDs,0,sizeof(DWORD)*count);
	for(int i=0; i<count; ++i)
	{	
		cCheckBox* Chk = pChkBtnArray[i];
		
		if(Chk->IsChecked())
		{
			cRITEMEx * rItem = (cRITEMEx *)pListCtrl->GetRItem(i);
			if(!rItem) return;
			if(rItem->dwID)
			{
				pOutIDs[i] = rItem->dwID;
			}
		}
	}
}


DWORD CNoteDialog::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we = cDialog::ActionEvent(mouseInfo);
	
	if(we & WE_ROWCLICK)
	{
		cRITEMEx * pRItem = (cRITEMEx *)m_pNoteListLCtrl->GetRItem(m_pNoteListLCtrl->GetSelectedRowIdx());
		if(pRItem != NULL)
		{	
			NOTEMGR->ReadNote(pRItem->dwID, 1);
		}
	}
	return we;
}

void CNoteDialog::ShowNotePageBtn(BYTE TotalPage)
{
	if(TotalPage == 0)
	{
		for(BYTE i=0; i<MAX_NOTE_PAGE; ++i)
		{
			if( m_pNotePageBtn[i] )
				m_pNotePageBtn[i]->SetActive(FALSE);
		}
		return;
	}
	
	for(DWORD i=0; i<MAX_NOTE_PAGE; ++i)
	{
		if( m_pNotePageBtn[i] )
		{
			m_pNotePageBtn[i]->SetTextValue(i+1);
			m_pNotePageBtn[i]->SetActive(i+1 <= TotalPage);
		}
	}

	// 080121 LYW --- NoteManager : 선택 된 탭만 푸쉬 상태로 처리 함.
	int nSelectedTab = m_SelectedNotePge-1 ;

	for( int count = 0 ;  count < MAX_NOTE_PAGE ; ++count )
	{
		if( !m_pNotePageBtn[count] ) continue ;

		if( nSelectedTab == count )
		{
			m_pNotePageBtn[count]->SetPush(TRUE) ;
		}
		else
		{
			m_pNotePageBtn[count]->SetPush(FALSE) ;
		}
	}
}

void CNoteDialog::SetChkAll()
{
	if( m_pNoteChkAll == NULL ) return;

	BOOL Chk = m_pNoteChkAll->IsChecked();
	
	for(int i=0; i<NOTENUM_PER_PAGE; ++i)
	{
		if( m_pNoteChk[i]->IsActive() )		
			m_pNoteChk[i]->SetChecked( Chk );
	}
}
