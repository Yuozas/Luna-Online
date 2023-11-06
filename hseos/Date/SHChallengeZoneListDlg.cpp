#include "stdafx.h"
#include "SHChallengeZoneListDlg.h"
#include "GameIn.h"
#include "GameResourceManager.h"
#include "WindowIDEnum.h"
#include "cStatic.h"
#include "cScriptManager.h"
#include "cResourceManager.h"
#include "cRITEMEx.h"
#include "../hseos/Date/SHDateManager.h"

CSHChallengeZoneListDlg::CSHChallengeZoneListDlg()
{
	m_nSelChallengeZone = 0;
}

CSHChallengeZoneListDlg::~CSHChallengeZoneListDlg()
{
	m_pcsChallengeZoneList->DeleteAllItems() ;
}

void CSHChallengeZoneListDlg::SetActive( BOOL val )
{
	cDialog::SetActive( val );
}

void CSHChallengeZoneListDlg::Linking()
{
	m_pcsChallengeZoneList = ( cListCtrl* )GetWindowForID( DMD_CHALLENGEZONE_MOVELIST_LISTCTRL ) ;

	for(int i=0; i<m_pcsChallengeZoneList->GetRowsNum(); i++)
	{
		cRITEMEx *ritem = new cRITEMEx(1);

		strcpy(ritem->pString[0], RESRCMGR->GetMsg(m_pcsChallengeZoneList->GetRowsMsgID(i)));
		if (i == 0) ritem->rgb[0] = RGBA_MAKE(255, 255, 255, 255);
		else		ritem->rgb[0] = RGBA_MAKE(10, 10, 10, 255);

		m_pcsChallengeZoneList->InsertItem(i, ritem);
	}
} 

void CSHChallengeZoneListDlg::Render()
{
 	if( !IsActive() ) return;

	cDialog::Render();
}

void CSHChallengeZoneListDlg::OnActionEvent( LONG lId, void* p, DWORD we )																	
{
 	if( we & WE_BTNCLICK )																											
	{
 		switch( lId )																												
		{
		case DMD_CHALLENGEZONE_MOVELIST_OK:
			g_csDateManager.CLI_RequestDateMatchingEnterChallengeZone(m_nSelChallengeZone);
			SetActive(FALSE);
			break;
		case DMD_CHALLENGEZONE_MOVELIST_CANCEL:
			SetActive(FALSE);
			break;
		}
	}
}

DWORD CSHChallengeZoneListDlg::ActionEvent(CMouse* mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we = cDialog::ActionEvent(mouseInfo);
	
	int nRowIndex = m_pcsChallengeZoneList->GetSelectedRowIdx();
	if(we & WE_ROWCLICK)
	{
		SelectChallengeZone(nRowIndex);
	}
	else if( we & WE_ROWDBLCLICK )
	{
		g_csDateManager.CLI_RequestDateMatchingEnterChallengeZone(nRowIndex);
		SetActive(FALSE);
	}

	return we;
}

void CSHChallengeZoneListDlg::SelectChallengeZone(int nRowIndex)
{
	for(int i=0; i<m_pcsChallengeZoneList->GetItemCount(); i++)
	{
		cRITEMEx* pRItem = (cRITEMEx*)m_pcsChallengeZoneList->GetRItem(i);
		if (i == nRowIndex)
		{
			pRItem->rgb[0] = RGBA_MAKE(255, 255, 255, 255);
			m_nSelChallengeZone = nRowIndex;
		}
		else
		{
			pRItem->rgb[0] = RGBA_MAKE(10, 10, 10, 255);
		}
	}
}
