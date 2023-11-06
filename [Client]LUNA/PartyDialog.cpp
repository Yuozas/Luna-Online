// PartyDialog.cpp: implementation of the CPartyDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartyDialog.h"
#include "PartyMemberDlg.h"
#include "PartyBtnDlg.h"

#include "PartyManager.h"
#include "ObjectManager.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"

#include "GameIn.h"

// 080318 LUJ, 버프 스킬 표시위해 참조
#include "..\interface\cCheckBox.h"
#include "StatusIconDlg.h"
#include "../[cc]skill/Client/Info/BuffSkillInfo.h"
#include "../[cc]skill/Client/Object/BuffSkillObject.h"
#include "../[cc]skill/client/manager/skillmanager.h"

// 061220 LYW --- Delete this code.
//#include "MainBarDialog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPartyDialog::CPartyDialog() 
{
	Init();
}

CPartyDialog::~CPartyDialog()
{

}

void CPartyDialog::Init()
{
	m_bActive = FALSE;
	m_ClickedMemberID = 0;
	m_bShowMember = TRUE;
	m_bOption = TRUE;
}

void CPartyDialog::RegistMemberDlg(int i, CPartyMemberDlg* pDlg)
{
	m_MemberDlg[i] = pDlg;
	m_MemberDlg[i]->Linking(i);
//	m_MemberDlg[i]->SetPartyBtnDlg( m_PartyBtnDlg );
}

void CPartyDialog::RegistBtnDlg(CPartyBtnDlg* pDlg)
{
	m_PartyBtnDlg = pDlg;
	m_PartyBtnDlg->Linking();

	for( int i = 0; i <MAX_PARTY_LISTNUM-1; ++i )
		m_MemberDlg[i]->SetPartyBtnDlg( m_PartyBtnDlg );
}

BOOL CPartyDialog::IsActive()
{
	return m_bActive;
}

void CPartyDialog::SetActive(BOOL bState)
{
	m_bActive = bState;

	if(bState == TRUE)
	{
		if(PARTYMGR->CanActivate() == FALSE)
		{
			m_bActive = FALSE;
		}
	}
	for(int j=0; j<MAX_PARTY_LISTNUM-1; ++j)
		m_MemberDlg[j]->SetActive(m_bActive);
	// 070409 LYW --- PartyDialog : Active party btn dialog.
	m_PartyBtnDlg->SetActive(m_bActive);

//--- main bar icon
//	CMainBarDialog* pDlg = GAMEIN->GetMainInterfaceDialog();
//	if( pDlg )
//		pDlg->SetPushBarIcon( OPT_PARTYDLGICON, m_bActive );
}

void CPartyDialog::RefreshDlg()
{
	if(HEROID == 0)
		return;

	for(int i=0; i<MAX_PARTY_LISTNUM-1; ++i)
		m_MemberDlg[i]->SetMemberData(NULL);

	int membercount=0;
	for(i=0; i<MAX_PARTY_LISTNUM; ++i)
	{
		DWORD PlayerID = PARTYMGR->GetPartyMemberID(i);
		if((PlayerID != 0) && (HEROID != PlayerID))
		{
			ASSERT( membercount < 6 );
			if( membercount < 6 )	//오류가 나서 파티가 꼬이더라도 다운은 안되도록
				m_MemberDlg[membercount++]->SetMemberData(PARTYMGR->GetPartyMemberInfo(i));
		}
	}

	m_PartyBtnDlg->RefreshDlg();
}

/*
BOOL CPartyDialog::IsActive()
{
	if(m_bMinDlg == MIN_PARTYDLG_MODE)
	{
		return m_pPartyMinDlg->IsActive();
	}
	else
	{
		return m_pPartyMaxDlg->IsActive();
	}
}

void CPartyDialog::SetActive(BOOL bbb)
{
	if(bbb == TRUE)
	{
		if(m_bMinDlg == MIN_PARTYDLG_MODE)
		{
			m_pPartyMaxDlg->SetActive(FALSE);
			m_pPartyMinDlg->SetActive(TRUE);
			m_pPartyMinDlg->RefreshDlg();
		}
		else
		{
			m_pPartyMinDlg->SetActive(FALSE);
			m_pPartyMaxDlg->SetActive(TRUE);
			m_pPartyMaxDlg->RefreshDlg();
		}
	}
	else
	{
		m_pPartyMaxDlg->SetActive(FALSE);
		m_pPartyMinDlg->SetActive(FALSE);
	}
}

void CPartyDialog::RefreshDlg()
{
	if(m_bMinDlg == MIN_PARTYDLG_MODE)
	{
		m_pPartyMinDlg->RefreshDlg();
	}
	else
	{
		m_pPartyMaxDlg->RefreshDlg();
	}
}

void CPartyDialog::SetDlgMode(BOOL state)
{
	m_bMinDlg = state;
}

void CPartyDialog::RegistMaxDlg(CPartyMaxDialog* pDlg)
{
	m_pPartyMaxDlg = pDlg;
	m_pPartyMaxDlg->Linking();
}
void CPartyDialog::RegistMinDlg(CPartyMinDialog* pDlg)
{
	m_pPartyMinDlg = pDlg;
	m_pPartyMinDlg->Linking();
}



void CPartyDialog::MemberBtnPushUp(int n)
{
	m_pPartyMaxDlg->MemberBtnPushUp(n);
}

  */

void CPartyDialog::MemberBtnPushUp(DWORD id)
{
	for(int i=0; i<MAX_PARTY_LISTNUM-1; ++i)
	{
		if(m_MemberDlg[i]->m_MemberID ==id)
		{
			m_MemberDlg[i]->SetNameBtnPushUp(TRUE);
			m_ClickedMemberID = id;
		}
		else
			m_MemberDlg[i]->SetNameBtnPushUp(FALSE);
	}
}

void CPartyDialog::SetClickedMemberID(DWORD id)
{
	if( id == 0 )
	{
		for( int i = 0; i <MAX_PARTY_LISTNUM-1; ++i )
			m_MemberDlg[i]->SetNameBtnPushUp( FALSE );
		m_ClickedMemberID = 0;
		return;
	}
	else
	{
		CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( id );
//		if( !pPlayer )
//		{
//			for( int i = 0; i <MAX_PARTY_LISTNUM-1; ++i )
//				m_MemberDlg[i]->SetNameBtnPushUp( FALSE );
//			m_ClickedMemberID = 0;
//		}
//		else
//		{
			MemberBtnPushUp(id);
//		}

			if(pPlayer)
			{
				pPlayer->ShowObjectName( TRUE, NAMECOLOR_SELECTED );

				OBJECTMGR->SetSelectedObject( pPlayer );
			}
	}

//	if(m_ClickedMemberID == id)
//	{
//		OBJECTMGR->SetSelectedObject( OBJECTMGR->GetObject( id ) );
//		return;
//	}
	
//	MemberBtnPushUp(id);
}

DWORD CPartyDialog::GetClickedMemberID()
{
	return m_ClickedMemberID;
}

void CPartyDialog::ShowMember()
{
	m_bShowMember = !m_bShowMember;

	for( int i = 0; i < MAX_PARTY_LISTNUM-1; ++i )
	{
		m_MemberDlg[i]->ShowMember( m_bShowMember );
		m_MemberDlg[i]->SetActive( m_bShowMember );
	}
}

void CPartyDialog::ShowOption()
{
	m_bOption = !m_bOption;

	m_PartyBtnDlg->ShowOption( m_bOption );

	for( int i = 0; i < MAX_PARTY_LISTNUM-1; ++i )
		m_MemberDlg[i]->ShowOption( m_bOption );
}

void CPartyDialog::SetOptionActive( BOOL bFlag )
{
	m_bOption = bFlag;
	ShowOption();
}

void CPartyDialog::SetMemberActive( BOOL bFlag )
{
	m_bShowMember = bFlag;
	ShowMember();
}

// 070420 LYW --- PartyDialog : Add function to return member dialog.
CPartyMemberDlg* CPartyDialog::GetMemberDlg( BYTE index )
{
	if(index<0 || index >= 6) return NULL ;
	
	return m_MemberDlg[index] ;
}


// 080318 LUJ, 파티원 버프 스킬 표시 추가
void CPartyDialog::AddSkill( const SKILLOBJECT_INFO& info )
{
	const BYTE size = sizeof( m_MemberDlg ) / sizeof( *m_MemberDlg );

	for( BYTE index = 0; size > index; ++index )
	{
		CPartyMemberDlg* memberDialog = GetMemberDlg( index );

		if(	!	memberDialog ||
				memberDialog->GetPlayerIndex() != info.MainTarget.dwMainTargetID )
		{
			continue;
		}

		CStatusIconDlg* statusIconDialog = memberDialog->GetStatusIconDialog();

		if( ! statusIconDialog )
		{
			continue;
		}

		statusIconDialog->AddSkillIcon( info.SkillObjectIdx, info.SkillIdx, info.RemainTime, info.Count );
		break;
	}
}


// 080318 LUJ, 파티원 버프 스킬 표시 제거
void CPartyDialog::RemoveSkill( const SKILLOBJECT_INFO& info )
{
	const BYTE size = sizeof( m_MemberDlg ) / sizeof( *m_MemberDlg );

	for( BYTE index = 0; size > index; ++index )
	{
		CPartyMemberDlg* memberDialog = GetMemberDlg( index );

		if(	!	memberDialog ||
				memberDialog->GetPlayerIndex() != info.MainTarget.dwMainTargetID )
		{
			continue;
		}

		CStatusIconDlg* statusIconDialog = memberDialog->GetStatusIconDialog();

		if( ! statusIconDialog )
		{
			continue;
		}

		statusIconDialog->RemoveSkillIcon( info.SkillObjectIdx, info.SkillIdx );
		break;
	}
}


// 080318 LUJ, 해당 파티원이 가진 모든 스킬 표시를 제거한다
void CPartyDialog::RemoveAllSkill( DWORD playerIndex )
{
	const BYTE size = sizeof( m_MemberDlg ) / sizeof( *m_MemberDlg );

	for( BYTE index = 0; size > index; ++index )
	{
		CPartyMemberDlg* memberDialog = GetMemberDlg( index );

		if( !	memberDialog ||
				memberDialog->GetPlayerIndex() != playerIndex )
		{
			continue;
		}

		CStatusIconDlg* statusIconDialog = memberDialog->GetStatusIconDialog();

		if( ! statusIconDialog )
		{
			continue;
		}

		statusIconDialog->Release();
		break;
	}
}


// 080318 LUJ, 파티원의 버프 스킬 표시를 토글한다
void CPartyDialog::OnActionEvent( LONG lId, void* p, DWORD we )
{
	switch( lId )
	{
	case PA_SKILL_ICON_BUTTON:
		{
			cCheckBox* checkBox = 0;
			{
				cDialog*	dialog	= WINDOWMGR->GetWindowForID( PA_BTNDLG );
				checkBox			= ( dialog ? ( cCheckBox* )dialog->GetWindowForID( PA_SKILL_ICON_BUTTON ) : 0 );

				if( !	checkBox						||
						checkBox->GetType() != WT_CHECKBOX )
				{
					break;
				}
			}

			const BYTE size = sizeof( m_MemberDlg ) / sizeof( *m_MemberDlg );

			for( BYTE index = 0; size > index; ++index )
			{
				CPartyMemberDlg* dialog = GetMemberDlg( index );

				if( dialog )
				{
					dialog->SetVisibleSkill( checkBox->IsChecked() );
				}
			}

			break;
		}
	case PA_MEMBER1_SKILL_ICON_BUTTON:
		{
			CPartyMemberDlg* dialog = GetMemberDlg( 0 );

			if( dialog )
			{
				dialog->SetVisibleSkill( ! dialog->IsVisibleSkill() );
			}

			break;
		}
	case PA_MEMBER2_SKILL_ICON_BUTTON:
		{
			CPartyMemberDlg* dialog = GetMemberDlg( 1 );

			if( dialog )
			{
				dialog->SetVisibleSkill( ! dialog->IsVisibleSkill() );
			}

			break;
		}
	case PA_MEMBER3_SKILL_ICON_BUTTON:
		{
			CPartyMemberDlg* dialog = GetMemberDlg( 2 );

			if( dialog )
			{
				dialog->SetVisibleSkill( ! dialog->IsVisibleSkill() );
			}

			break;
		}
	case PA_MEMBER4_SKILL_ICON_BUTTON:
		{
			CPartyMemberDlg* dialog = GetMemberDlg( 3 );

			if( dialog )
			{
				dialog->SetVisibleSkill( ! dialog->IsVisibleSkill() );
			}

			break;
		}
	case PA_MEMBER5_SKILL_ICON_BUTTON:
		{
			CPartyMemberDlg* dialog = GetMemberDlg( 4 );

			if( dialog )
			{
				dialog->SetVisibleSkill( ! dialog->IsVisibleSkill() );
			}

			break;
		}
	case PA_MEMBER6_SKILL_ICON_BUTTON:
		{
			CPartyMemberDlg* dialog = GetMemberDlg( 5 );

			if( dialog )
			{
				dialog->SetVisibleSkill( ! dialog->IsVisibleSkill() );
			}

			break;
		}
	}
}


// 080318 LUJ, 파티원이 현재 클라이언트에 가진 버프를 상태 아이콘 창에 표시한다
void CPartyDialog::InitializeSkill( const PARTY_MEMBER& member )
{
	// 080318 LUJ, 적당한 상태창을 얻어온다
	CStatusIconDlg* statusIconDialog = 0;
	{
		for(
			BYTE index = sizeof( m_MemberDlg ) / sizeof( *m_MemberDlg );
			index--; )
		{
			CPartyMemberDlg* dialog = GetMemberDlg( index );

			if( dialog &&
				dialog->GetPlayerIndex() == member.dwMemberID )
			{
				statusIconDialog = dialog->GetStatusIconDialog();
				break;
			}
		}

		if( ! statusIconDialog )
		{
			return;
		}

		statusIconDialog->Release();
		statusIconDialog->SetObject( HERO );
	}

	// 080318 LUJ, 스킬 오브젝트를 모두 검색해서 타겟이 추가된 파티원인 경우, 추가해준다
	{
		CYHHashTable< cSkillObject >& hashTable = SKILLMGR->GetSkillObjectHashTable();
		hashTable.SetPositionHead();
		
		for(
			cSkillObject* object = 0;
			object = hashTable.GetData(); )
		{
			if( ! object )
			{
				continue;
			}

			const sSKILL_CREATE_INFO* createInfo = object->GetSkillObjectInfo();

			// 080318 LUJ, 추가된 파티원에게 적용된 스킬인지 검사한다
			if( !	createInfo	||
					createInfo->mainTarget.dwMainTargetID != member.dwMemberID )
			{
				continue;
			}

			cBuffSkillInfo* skillInfo = ( cBuffSkillInfo* )createInfo->pSkillInfo;

			if( !	skillInfo ||
					skillInfo->GetKind() != SKILLKIND_BUFF )
			{
				continue;
			}

			cBuffSkillObject*		buffSkillObject = ( cBuffSkillObject* )object;
			const BUFF_SKILL_INFO*	buffSkillInfo	= skillInfo->GetSkillInfo();

			if( ! buffSkillInfo )
			{
				continue;
			}

			DWORD time = 0;

			// 080318 LUJ, 버프 스킬의 유효 시간은 클라이언트에서 업데이트되지 않으므로, 설정 시각을 기준으로 계산한다
			if( createInfo->remainTime )
			{
				time = createInfo->remainTime + buffSkillObject->GetBeginTime();
			}
			else if( buffSkillInfo->DelayTime )
			{
				time = buffSkillInfo->DelayTime + buffSkillObject->GetBeginTime();
			}

			statusIconDialog->AddSkillIcon( object->GetID(), skillInfo->GetIndex(), time, buffSkillInfo->Count );
		}
	}
}

// 080613 LUJ, 파티 버프 표시 위치를 다시 설정한다. 버프 아이콘이 많을 수록 처리 시간이 지연되므로, 프로세스 단에서는 호출하지 않도록 하자.
//
//	참조: 	원래 RefreshDlg()에 있었으나, 자주 호출되어 클라이언트 처리 속도가 저하되었음. 별도의 함수로 분리하고 이벤트 발생 시에만
//			처리하도록 함
void CPartyDialog::RelocateSkill()
{
	// 080318 LUJ, 스킬 정보 보관
	typedef std::list< StatusInfo >	SkillList;

	// 080318 LUJ, 파티원 별 표시 중인 버프 스킬을 보관한다
	typedef std::map< DWORD, SkillList >	SkillMap;
	SkillMap								skillMap;

	// 080318 LUJ,	정보를 갱신하기 전에 스탯 창에서 스킬들을 가져와서 플레이어 인덱스 별로 저장하자
	//				그래서 파티 멤버 정보 창 표시 위치가 변경되어도 스킬 위치가 정상적으로 표시되도록 한다.
	for(
		BYTE index = sizeof( m_MemberDlg ) / sizeof( *m_MemberDlg );
		index--; )
	{
		CPartyMemberDlg* memberDialog = GetMemberDlg( index );

		if( !	memberDialog	||
			!	memberDialog->IsActive() )
		{
			continue;
		}

		CStatusIconDlg* statusIconDialog = memberDialog->GetStatusIconDialog();

		if( ! statusIconDialog )
		{
			continue;
		}

		SkillList& skillList = skillMap[ memberDialog->GetPlayerIndex() ];

		statusIconDialog->Copy( skillList );
		statusIconDialog->Release();
	}

	// 080318 LUJ,	파티원이 탈퇴/가입하면 위치가 변동되어 스킬 표시 위치가 바꾸어주어야 한다.
	//				데이터를 복사해서 옮겨야한다.
	for(
		BYTE index = sizeof( m_MemberDlg ) / sizeof( *m_MemberDlg );
		index--; )
	{
		CPartyMemberDlg* memberDialog = GetMemberDlg( index );

		if( ! memberDialog )
		{
			continue;
		}

		SkillMap::const_iterator list_it = skillMap.find( memberDialog->GetPlayerIndex() );

		if( skillMap.end() == list_it )
		{
			continue;
		}

		CStatusIconDlg* statusIconDialog = memberDialog->GetStatusIconDialog();

		if( ! statusIconDialog )
		{
			continue;
		}

		statusIconDialog->Release();
		statusIconDialog->SetObject( HERO );

		const SkillList& skillList = list_it->second;

		// 080318 LUJ, 현재 표시 중인 스킬을 버리고 새로 추가한다
		for(
			SkillList::const_iterator it = skillList.begin();
			skillList.end() != it;
		++it )
		{
			const StatusInfo& info = *it;

			statusIconDialog->AddSkillIcon( info.ID, info.Index, info.Time, info.Count );
		}
	}
}