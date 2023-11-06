// PartyMemberDlg.cpp: implementation of the CPartyMemberDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartyMemberDlg.h"
#include "PartyManager.h"
#include "GameIn.h"

#include "WindowIDEnum.h"
#include "./Interface/cPushupButton.h"
#include "./ObjectGuagen.h"
#include "./Interface/cStatic.h"
#include "PartyBtnDlg.h"

// 070419 LYW --- PartyMemberDlg : Include header file of CPlayer.
#include "Player.h"

// 070419 LYW --- PartyMemberDlg : Include header file of ObjectManager.
#include "ObjectManager.h"

#include "./Interface/cScriptManager.h"

// 080318 LUJ, 파티 멤버의 스킬 표시 테스트위해 포함
#include "StatusIconDlg.h"
#include "..\interface\cCheckBox.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DWORD PMD_LOGIN_BASICCOLOR  = RGBA_MAKE(255,255,255,255);
DWORD PMD_LOGIN_OVERCOLOR  = RGBA_MAKE(255,255,255,255);
DWORD PMD_LOGIN_PRESSCOLOR  = RGBA_MAKE(255,234,0,255);

DWORD PMD_LOGOUT_BASICCOLOR  = RGBA_MAKE(172,182,199,255);
DWORD PMD_LOGOUT_OVERCOLOR  = RGBA_MAKE(172,182,199,255);
DWORD PMD_LOGOUT_PRESSCOLOR  = RGBA_MAKE(255,234,0,255);

CPartyMemberDlg::CPartyMemberDlg() :
// 080318 LUJ, 파티원 버프 스킬 창 초기화
mStatusIconDialog( new CStatusIconDlg ),
// 080318 LUJ, 체크 박스 초기화
mCheckBox( 0 )
{
	m_MemberID = 0;
	m_bRealActive = FALSE;
	
	m_bSetTopOnActive = FALSE;	//KES추가

	m_pPartyBtnDlg = NULL;
	m_nIndex = -1;
	m_bOption = TRUE;
	m_bMember = TRUE;

	m_bShowFace = FALSE ;

	// 071003 LYW --- PartyMemberDlg : Initialize master mark part.
	m_bRenderMasterMark = FALSE ;

	// 071003 LYW --- PartyMemberDlg : Initialize class mark part.
	//m_bRenderClassMark = FALSE ;
}

CPartyMemberDlg::~CPartyMemberDlg()
{
	SAFE_DELETE( mStatusIconDialog );
}

void CPartyMemberDlg::SetActive(BOOL val)
{
	if( m_bDisable ) return;
	if( m_bMember == FALSE )
		val = FALSE;	
	m_bRealActive = val;	
	if(m_MemberID == 0)
		val = FALSE;
	cDialog::SetActive(val);

	// 080318 LUJ, 회원 창이 꺼지면 스킬 아이콘도 함께 안 보이도록 해야한다
	if( mStatusIconDialog )
	{
		mStatusIconDialog->SetObject( val ? HERO : 0 );
	}
}

void CPartyMemberDlg::Linking(int i)
{
	m_pName = (cPushupButton *)GetWindowForID(PA_MEMBER1NAME+i);
	m_pLife = (CObjectGuagen *)GetWindowForID(PA_GUAGEMEMBER1LIFE+i);
	m_pMana = (CObjectGuagen *)GetWindowForID(PA_GUAGEMEMBER1MANA+i);
	m_pLevel = (cStatic *)GetWindowForID(PA_MEMBER1LEVEL+i);

	// 070419 LYW --- PartyMember : Add member static for character image.
	/*m_pFace[0] = (cStatic*)GetWindowForID(PA_CHAR1_H_M+i) ;
	m_pFace[1] = (cStatic*)GetWindowForID(PA_CHAR1_H_W+i) ;
	m_pFace[2] = (cStatic*)GetWindowForID(PA_CHAR1_E_M+i) ;
	m_pFace[3] = (cStatic*)GetWindowForID(PA_CHAR1_E_W+i) ;*/
	m_nIndex = i;

	// 080318 LUJ, 파티원 버프 스킬 창 초기화
	if( mStatusIconDialog )
	{
		const float defaultSize = 14.0f;
		VECTOR2		position	= { 0 };
		{
			position.x	= GetAbsX() + GetWidth();
			position.y	= GetAbsY();		
		}		

		mStatusIconDialog->SetPosition( position );
		mStatusIconDialog->SetIconSize( defaultSize, defaultSize );
		mStatusIconDialog->SetMaxIconQuantity( 36 );
		mStatusIconDialog->Init( HERO, &position, 12 );
	}

	// 080318 LUJ, 체크 박스 초기화
	{
		switch( GetID() )
		{
		case PA_MEMBER1DLG:
			{
				mCheckBox = ( cCheckBox* )GetWindowForID( PA_MEMBER1_SKILL_ICON_BUTTON );
				break;
			}
		case PA_MEMBER2DLG:
			{
				mCheckBox = ( cCheckBox* )GetWindowForID( PA_MEMBER2_SKILL_ICON_BUTTON );
				break;
			}
		case PA_MEMBER3DLG:
			{
				mCheckBox = ( cCheckBox* )GetWindowForID( PA_MEMBER3_SKILL_ICON_BUTTON );
				break;
			}
		case PA_MEMBER4DLG:
			{
				mCheckBox = ( cCheckBox* )GetWindowForID( PA_MEMBER4_SKILL_ICON_BUTTON );
				break;
			}
		case PA_MEMBER5DLG:
			{
				mCheckBox = ( cCheckBox* )GetWindowForID( PA_MEMBER5_SKILL_ICON_BUTTON );
				break;
			}
		case PA_MEMBER6DLG:
			{
				mCheckBox = ( cCheckBox* )GetWindowForID( PA_MEMBER6_SKILL_ICON_BUTTON );
				break;
			}
		}

		if( mCheckBox && 
			mCheckBox->GetType() != WT_CHECKBOX )
		{
			mCheckBox = 0;
		}
	}
}


void CPartyMemberDlg::SetMemberData(PARTY_MEMBER* pInfo)
{
	if( pInfo )
	{
		if( !pInfo->bLogged ) m_bShowFace = FALSE ;
	}

	if(pInfo == NULL)
	{
		m_MemberID = 0;
	}
	else
	{
		// 070423 LYW --- PartyMemberDlg : Setting face of character.
		//SetFace(pInfo->Race, pInfo->Gender) ;
		char buf[10];
		// 070419 LYW --- PartyMemberDlg : Modified level string.
		//sprintf(buf, "Lv.%d", pInfo->Level);
		sprintf(buf, "%d", pInfo->Level);
		m_MemberID = pInfo->dwMemberID;

		// 071003 LYW --- PartyMemberDlg : Setting master mark part.
		if( m_MemberID == PARTYMGR->GetMasterID() )
		{
			m_bRenderMasterMark = TRUE ;
			SCRIPTMGR->GetImage( MASTER_MARK_NUM, &m_MasterMark, PFT_HARDPATH ) ;
		}

		if(pInfo->bLogged)
		{
			// 070622 LYW --- PartyMemberDlg : Modified face part.
			ZeroMemory(&m_pFace, sizeof(cImage)) ;

			// setting face num.
			int nFaceNum = 0 ;
			if( pInfo->Race == PARTY_MEMBER_RACE_HUMAN )
			{
				// setting human.
				if( pInfo->Gender == PARTY_MEMBER_GENDER_MAN)
				{
					nFaceNum = PARTY_MEMBER_FACE_HUMAN_MAN ;
				}
				else
				{
					nFaceNum = PARTY_MEMBER_FACE_HUMAN_WOMAN ;
				}
			}
			else
			{
				// setting human.
				if( pInfo->Gender == PARTY_MEMBER_GENDER_MAN)
				{
					nFaceNum = PARTY_MEMBER_FACE_ELF_MAN ;
				}
				else
				{
					nFaceNum = PARTY_MEMBER_FACE_ELF_WOMAN ;
				}
			}

			// setting face image.
			SCRIPTMGR->GetImage( nFaceNum, &m_pFace, PFT_HARDPATH ) ;

			m_bShowFace = TRUE ;

			m_pName->SetText(pInfo->Name, PMD_LOGIN_BASICCOLOR, PMD_LOGIN_OVERCOLOR, PMD_LOGIN_PRESSCOLOR);
			m_pLife->SetValue(pInfo->LifePercent*0.01f, 0);
			m_pMana->SetValue(pInfo->ManaPercent*0.01f, 0);
			//m_pLevel->SetFGColor(PMD_LOGIN_BASICCOLOR) ;
			m_pLevel->SetStaticText(buf);
			m_pLevel->SetFGColor(0x0a0a0aff) ;
		}

		else
		{
			m_bShowFace = FALSE ;

			m_pName->SetText(pInfo->Name, PMD_LOGOUT_BASICCOLOR, PMD_LOGOUT_OVERCOLOR, PMD_LOGOUT_PRESSCOLOR);
			m_pLife->SetValue(0, 0);
			m_pMana->SetValue(0, 0);
			//m_pLevel->SetFGColor(PMD_LOGOUT_BASICCOLOR) ;
			m_pLevel->SetStaticText("");
			m_pLevel->SetFGColor(0x0a0a0aff) ;
		}		
	}

	SetActive(m_bRealActive);
}

DWORD CPartyMemberDlg::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we = cDialog::ActionEvent(mouseInfo);
	
	if(we & WE_LBTNCLICK)
	{
		GAMEIN->GetPartyDialog()->SetClickedMemberID(m_MemberID);
	}
	return we;
}

void CPartyMemberDlg::SetNameBtnPushUp(BOOL val)
{
	m_pName->SetPush(val);
}

void CPartyMemberDlg::Render()
{
	/*
	if( m_pPartyBtnDlg && m_nIndex != -1 )
	{
		float x = m_pPartyBtnDlg->GetAbsX();
		float y = m_pPartyBtnDlg->GetAbsY();

		if( m_bOption )
		{
			this->SetAbsXY( (LONG)x, (LONG)(y+134 + ( 48 * m_nIndex )) );
			m_pName->SetAbsXY( (LONG)(x+9), (LONG)(y+148 + ( 48 * m_nIndex )) );
			m_pLife->SetAbsXY( (LONG)(x+6), (LONG)(y+168 + ( 48 * m_nIndex )) );
			m_pMana->SetAbsXY( (LONG)(x+6), (LONG)(y+174 + ( 48 * m_nIndex )) );
			m_pLevel->SetAbsXY( (LONG)(x+9), (LONG)(y+135 + ( 48 * m_nIndex )) );
		}
		else
		{
			this->SetAbsXY( (LONG)x, (LONG)(y + 54 + ( 48 * m_nIndex )) );
			m_pName->SetAbsXY( (LONG)(x+9), (LONG)(y + 68 + ( 48 * m_nIndex )) );
			m_pLife->SetAbsXY( (LONG)(x+6), (LONG)(y + 88 + ( 48 * m_nIndex )) );
			m_pMana->SetAbsXY( (LONG)(x+6), (LONG)(y + 94 + ( 48 * m_nIndex )) );
			m_pLevel->SetAbsXY( (LONG)(x+9), (LONG)(y + 55 + ( 48 * m_nIndex )) );
		}
	}
	*/

	if( !IsActive() ) return ;

	// 080318 LUJ, 버프 스킬 표시
	// 080613 LUJ, 액티브 상태에서만 렌더링되도록 위치 이동
	if( mStatusIconDialog	&&
		mCheckBox			&&
		mCheckBox->IsChecked() )
	{
		mStatusIconDialog->Render();
	}

	cDialog::RenderWindow();
	cDialog::RenderComponent();

	VECTOR2 scale;
	scale.x = 1.0f;
	scale.y = 1.0f;

	// render face.
	VECTOR2 vPos;
	/*vPos.x = (float)(GetAbsX() + 8) ;
	vPos.y = (float)(GetAbsY() + 6) ;*/

	int nExtraW = 5 ;
	int nMarkW = 14 ;

	vPos.x = (float)(GetWidth() - nExtraW - nMarkW ) ;
	vPos.y = (float)(GetAbsY() + 6) ;

	DWORD dwColor = 0xffffffff ;

	/*if(m_bShowFace)
	{
		m_pFace.RenderSprite(&scale, NULL, 0, &vPos, dwColor) ;
	}*/

	// 0701003 LYW --- PartyMemberDlg : Render master mark.
	if( m_bRenderMasterMark )
	{
		m_MasterMark.RenderSprite(&scale, NULL, 0, &vPos, dwColor) ;
	}

	//// 071003 LYW --- PartyMemberDlg : Render class mark.
	//if( m_bRenderClassMark )
	//{
	//	if( m_bRenderMasterMark )
	//	{
	//		vPos.x = (float)(GetAbsX() + 24) ;
	//	}
	//
	//	m_ClassMark.RenderSprite(&scale, NULL, 0, &vPos, dwColor) ;
	//}
}

// 070420 LYW --- PartyMember : Add function to setting member face.
void CPartyMemberDlg::SetFace(BYTE racial, BYTE gender)
{
	//if(racial == 0)	// human
	//{
	//	if( gender == 0 ) // man
	//	{
	//		m_pFace[0]->SetActive( TRUE ) ;
	//	}
	//	else			  // woman
	//	{
	//		m_pFace[1]->SetActive( TRUE ) ;
	//	}
	//}
	//else			// elf
	//{
	//	if( gender == 0 ) // man
	//	{
	//		m_pFace[2]->SetActive( TRUE ) ;
	//	}
	//	else			  // woman
	//	{
	//		m_pFace[3]->SetActive( TRUE ) ;
	//	}
	//}
}


// 080318 LUJ, 버프 아이콘 표시 위치 갱신 위해 오버라이딩
void CPartyMemberDlg::SetAbsXY( LONG x, LONG y )
{
	cDialog::SetAbsXY( x, y );

	if( mStatusIconDialog )
	{
		VECTOR2 position = { 0 };
		position.x	= float( x ) + GetHeight();
		position.y	= float( y );

		mStatusIconDialog->SetPosition( position );
	}
}


// 080318 LUJ, 버프 스킬을 표시하거나 끈다
void CPartyMemberDlg::SetVisibleSkill( BOOL isVisible )
{
	if( mStatusIconDialog )
	{
		mStatusIconDialog->SetObject( isVisible ? HERO : 0 );
	}

	if( mCheckBox )
	{
		mCheckBox->SetChecked( isVisible );
	}
}


// 080318 LUJ, 버프 스킬 표시 여부를 알린다
BOOL CPartyMemberDlg::IsVisibleSkill() const
{
	if( mStatusIconDialog )
	{
		return 0 != mStatusIconDialog->GetObject();
	}

	return FALSE;	
}
