// PartyBtnDlg.cpp: implementation of the CPartyBtnDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartyBtnDlg.h"
#include "WindowIDEnum.h"
#include "./Interface/cButton.h"
#include "./Interface/cStatic.h"
#include "PartyManager.h"
#include "ObjectManager.h"

#include "./Interface/cCheckBox.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPartyBtnDlg::CPartyBtnDlg()
{
	m_bSetTopOnActive = FALSE;	//KESÃß°¡;
	// 070416 LYW --- PartyBtnDlg : Default setting to FALSE.
	//m_bOption = TRUE;
	m_bOption = IsActive();

	m_pSecedeBtn= NULL ;
	m_pTransferBtn= NULL ;
	m_pForcedSecedeBtn= NULL ;
	m_pAddMemberBtn= NULL ;
	m_pBreakUpBtn= NULL ;
	m_pWarSuggestBtn= NULL ;
	m_pMemberBtn= NULL ;
	
	m_CB_Random = NULL ;
	m_CB_Damage = NULL ;
	m_CB_Sequence = NULL ;
}

CPartyBtnDlg::~CPartyBtnDlg()
{
}

void CPartyBtnDlg::Linking()
{
	//m_pBackGround = (cStatic*)GetWindowForID(PA_BACKGROUND);
	m_pSecedeBtn = (cButton*)GetWindowForID(PA_SECEDEBTN);
	m_pTransferBtn = (cButton*)GetWindowForID(PA_TRANSFERBTN);
	m_pForcedSecedeBtn = (cButton*)GetWindowForID(PA_FORCEDSECEDEBTN);
	m_pAddMemberBtn = (cButton*)GetWindowForID(PA_ADDPARTYMEMBERBTN);
	m_pBreakUpBtn = (cButton*)GetWindowForID(PA_BREAKUPBTN);
	m_pWarSuggestBtn = (cButton*)GetWindowForID(PA_WARSUGGESTBTN);
	//m_pOptionBtn = (cButton*)GetWindowForID(PA_OPTIONBTN);
	//m_pMemberBtn = (cButton*)GetWindowForID(PA_MEMBERBTN);

	m_CB_Random = (cCheckBox*)GetWindowForID(PA_OP_RANDOM) ;
	m_CB_Random->SetChecked(FALSE) ;

	m_CB_Damage = (cCheckBox*)GetWindowForID(PA_OP_DAMAGE) ;
	m_CB_Damage->SetChecked(FALSE) ;

	m_CB_Sequence = (cCheckBox*)GetWindowForID(PA_OP_SEQUENCE) ;
	m_CB_Sequence->SetChecked(FALSE) ;
}

void CPartyBtnDlg::RefreshDlg()
{
	if(HERO->GetPartyIdx() == 0)
		ShowNonPartyDlg();
	else if(PARTYMGR->GetMasterID() == HEROID)
	{
		ShowPartyMasterDlg();
	}
	else
	{
		ShowPartyMemberDlg();
	}
}

void CPartyBtnDlg::ShowNonPartyDlg()
{
	m_pSecedeBtn->SetActive(TRUE);
	m_pTransferBtn->SetActive(TRUE);
	m_pForcedSecedeBtn->SetActive(TRUE);
	m_pAddMemberBtn->SetActive(TRUE);
	m_pBreakUpBtn->SetActive(TRUE);
	m_pWarSuggestBtn->SetActive(TRUE);
	//m_pOptionBtn->SetActive(TRUE);
	//m_pMemberBtn->SetActive(TRUE);
}

void CPartyBtnDlg::ShowPartyMasterDlg()
{
/*	m_pSecedeBtn->SetActive(TRUE);
	m_pTransferBtn->SetActive(TRUE);
	m_pForcedSecedeBtn->SetActive(TRUE);
	m_pAddMemberBtn->SetActive(TRUE);
	m_pBreakUpBtn->SetActive(TRUE);
	m_pWarSuggestBtn->SetActive(TRUE);
	m_pOptionBtn->SetActive(TRUE);
	m_pMemberBtn->SetActive(TRUE);
*/
	//m_pBackGround->SetActive(m_bOption);
	m_pSecedeBtn->SetActive(m_bOption);
	m_pTransferBtn->SetActive(m_bOption);
	m_pForcedSecedeBtn->SetActive(m_bOption);
	m_pAddMemberBtn->SetActive(m_bOption);
	m_pBreakUpBtn->SetActive(m_bOption);
	m_pWarSuggestBtn->SetActive(m_bOption);
	//m_pOptionBtn->SetActive(TRUE);
	//m_pMemberBtn->SetActive(TRUE);

	m_pSecedeBtn->SetImageRGB( ICONCLR_DISABLE );
	m_pTransferBtn->SetImageRGB( ICONCLR_USABLE );
	m_pForcedSecedeBtn->SetImageRGB( ICONCLR_USABLE );
	m_pAddMemberBtn->SetImageRGB( ICONCLR_USABLE );
	m_pBreakUpBtn->SetImageRGB( ICONCLR_USABLE );
	m_pWarSuggestBtn->SetImageRGB( ICONCLR_USABLE );

	m_pSecedeBtn->SetDisable( TRUE );
	m_pTransferBtn->SetDisable( FALSE );
	m_pForcedSecedeBtn->SetDisable( FALSE );
	m_pAddMemberBtn->SetDisable( FALSE );
	m_pBreakUpBtn->SetDisable( FALSE );
	m_pWarSuggestBtn->SetDisable( FALSE );

	m_CB_Random->SetDisable(FALSE) ;
	m_CB_Damage->SetDisable(FALSE) ;
	m_CB_Sequence->SetDisable(FALSE) ;
}

void CPartyBtnDlg::ShowPartyMemberDlg()
{
/*	m_pSecedeBtn->SetActive(TRUE);
	m_pTransferBtn->SetActive(TRUE);
	m_pForcedSecedeBtn->SetActive(TRUE);
	m_pAddMemberBtn->SetActive(TRUE);
	m_pBreakUpBtn->SetActive(TRUE);
	m_pWarSuggestBtn->SetActive(TRUE);
	m_pOptionBtn->SetActive(TRUE);
	m_pMemberBtn->SetActive(TRUE);
*/
	//m_pBackGround->SetActive(m_bOption);
	m_pSecedeBtn->SetActive(m_bOption);
	m_pTransferBtn->SetActive(m_bOption);
	m_pForcedSecedeBtn->SetActive(m_bOption);
	m_pAddMemberBtn->SetActive(m_bOption);
	m_pBreakUpBtn->SetActive(m_bOption);
	m_pWarSuggestBtn->SetActive(m_bOption);
	//m_pOptionBtn->SetActive(TRUE);
	//m_pMemberBtn->SetActive(TRUE);
	
	m_pSecedeBtn->SetImageRGB( ICONCLR_USABLE );
	m_pTransferBtn->SetImageRGB( ICONCLR_DISABLE );
	m_pForcedSecedeBtn->SetImageRGB( ICONCLR_DISABLE );
	m_pAddMemberBtn->SetImageRGB( ICONCLR_DISABLE );
	m_pBreakUpBtn->SetImageRGB( ICONCLR_DISABLE );
	m_pWarSuggestBtn->SetImageRGB( ICONCLR_DISABLE );

	m_pSecedeBtn->SetDisable( FALSE );
	m_pTransferBtn->SetDisable( TRUE );
	m_pForcedSecedeBtn->SetDisable( TRUE );
	m_pAddMemberBtn->SetDisable( TRUE );
	m_pBreakUpBtn->SetDisable( TRUE );
	m_pWarSuggestBtn->SetDisable( TRUE );

	m_CB_Random->SetDisable(TRUE) ;
	m_CB_Damage->SetDisable(TRUE) ;
	m_CB_Sequence->SetDisable(TRUE) ;
}

void CPartyBtnDlg::ShowOption( BOOL bOption )
{
	m_bOption = bOption;

	//m_pBackGround->SetActive(m_bOption);
	m_pSecedeBtn->SetActive(m_bOption);
	m_pTransferBtn->SetActive(m_bOption);
	m_pForcedSecedeBtn->SetActive(m_bOption);
	m_pAddMemberBtn->SetActive(m_bOption);
	m_pBreakUpBtn->SetActive(m_bOption);
	m_pWarSuggestBtn->SetActive(m_bOption);
	//m_pOptionBtn->SetActive(TRUE);
	//m_pMemberBtn->SetActive(TRUE);

//	if( m_bOption )
//	{
//		m_pMemberBtn->SetAbsY( m_pOptionBtn->GetAbsY() + 100 );
//	}
//	else
//	{
//		m_pMemberBtn->SetAbsY( m_pOptionBtn->GetAbsY() + 20 );
//	}
}

void CPartyBtnDlg::Render()
{
	// 070209 LYW --- CPartyBtnDlg : Modified Render part.
	if( IsMovedWnd() )
	{
		/*if( m_bOption )
		{
			m_pMemberBtn->SetAbsY( (LONG)(m_pOptionBtn->GetAbsY() + 100) );
		}
		else
		{
			m_pMemberBtn->SetAbsY( (LONG)(m_pOptionBtn->GetAbsY() + 20) );
		}*/

		SetMovedWnd( FALSE ) ;
	}

	cDialog::Render();
}

// 071002 LYW --- PartyBtnDlg : Add function to setting checkbox.
void CPartyBtnDlg::SetCheckBox(int option)
{
	m_CB_Random->SetChecked(FALSE) ;
	m_CB_Damage->SetChecked(FALSE) ;
	m_CB_Sequence->SetChecked(FALSE) ;

	switch(option)
	{
	case 0 :	m_CB_Random->SetChecked(TRUE);	break ;
	case 1 :	m_CB_Damage->SetChecked(TRUE);	break ;
	case 2 :	m_CB_Sequence->SetChecked(TRUE);	break ;
	}
}
