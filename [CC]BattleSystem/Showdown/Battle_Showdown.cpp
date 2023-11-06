#include "stdafx.h"
#include "Battle_Showdown.h"
#include "BattleTeam_Showdown.h"
#include "Object.h"
#include "ObjectStateManager.h"
#include "WantedManager.h"
#include "PartyManager.h"
#include "Party.h"
#include "PKManager.h"
#include "pet.h"
#ifdef _MAPSERVER_
#include "ObjectStateManager.h"
#include "UserTable.h"
#include "Player.h"
#include "PackedData.h"
#include "../[cc]skill/server/manager/skillmanager.h"
#include "../[cc]skill/server/object/skillobject.h"
#else
#include "./interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"
#include "ObjectManager.h"
#include "TileManager.h"
#include "MHMap.h"
#include "ChatManager.h"
#endif

// 070201 LYW --- Include game resorce manager.
#include "../[CC]Header/GameResourceManager.h"

CBattle_Showdown::CBattle_Showdown()
{
	m_BattleFlag = BATTLE_FLAG_NONE;
#ifndef _MAPSERVER_
	m_pCurShowImage	= NULL;
#endif
}

CBattle_Showdown::~CBattle_Showdown()
{

}

void CBattle_Showdown::Initialize(BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2)
{
	CBattle::Initialize(pCreateInfo,&m_Team[0],&m_Team[1]);
}


//////////////////////////////////////////////////////////////////////////
// { start virtual func �ݵ�� �������̵� �ؾ���




// Battle ���� ����
#ifdef _MAPSERVER_
void CBattle_Showdown::GetBattleInfo(char* pInfo,WORD* size)
{
	BATTLE_INFO_SHOWDOWN* info = (BATTLE_INFO_SHOWDOWN*)pInfo;

	memcpy(info,&m_ShowdownInfo,sizeof(BATTLE_INFO_SHOWDOWN));
	
	ToEachTeam(pTeam)
		CBattleTeam_Showdown* pTeamVM	= (CBattleTeam_Showdown*)pTeam;
		info->Character[TeamNumber] = pTeamVM->GetCharacterID();
	EndToEachTeam
	
	*size = sizeof(BATTLE_INFO_SHOWDOWN);
}
/*
void CBattle_Showdown::GetBattleInfo(BATTLE_INFO_BASE*& pInfo,int& size)
{
	pInfo = &m_ShowdownInfo;
	size = sizeof(BATTLE_INFO_BASE);
}
*/
#endif



// ��,�Ʊ� ����
BOOL CBattle_Showdown::IsEnemy(CObject* pOperator,CObject* pTarget)
{
	if( pOperator->GetObjectKind() == eObjectKind_Player )
	{
		if( pTarget->GetObjectKind() & eObjectKind_Monster )	// ���ʹ� ���̴�
		{
			return TRUE;
		}
		else if( pTarget->GetObjectKind() == eObjectKind_Player )	//Ÿ���� �÷��̾� �ϰ��
		{
			/////���� �����ϰ�!!!!!!!!!
			/////�񹫷� �װ� �ٽ� �ǻ�� ����! �׶� �񹫷� �׾����� PK�� �׾����� Ȯ���Ұ�!

			if( pOperator->GetBattleID() == pTarget->GetBattleID() &&
				pOperator->GetBattleTeam() != pTarget->GetBattleTeam() &&
				GetBattleState() == eBATTLE_STATE_FIGHT )
				return TRUE;	// ��Ʋ���̵� ���� ���� �ٸ��ٸ� ���̴�.	//�񹫻� ��

#ifdef _MAPSERVER_
			if( ((CPlayer*)pOperator)->GetPartyIdx() != 0 )
			{
				CParty* pParty = PARTYMGR->GetParty( ((CPlayer*)pOperator)->GetPartyIdx() );
				if( pParty )
				if( pParty->IsPartyMember( pTarget->GetID() ) )
				{
					return FALSE;	//��Ƽ���� ���� �ƴϴ�.
				}
			}
			if( PKMGR->IsPKAllow() )// || ((CPlayer*)pOperator)->GetShopItemManager()->GetUsingItemInfo( eIncantation_Tracking_Jin ) )
			if( WANTEDMGR->IsOwner((CPlayer*)pOperator, ((CPlayer*)pTarget)->GetWantedIdx()) == TRUE )
				return TRUE;		//ô�� ����ڴ� ���̴�.
#else
			if( pOperator == HERO )
			{
				if( PARTYMGR->IsPartyMember( pTarget->GetID() ) )
				{
//					if( OBJECTMGR->GetSelectedObjectID() == pTarget->GetID() )
//					{ //������������ ��Ƽ���� �����Ҷ��� �޼��� ���� �ϱ� ����
//						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(630) );
//					}
					return FALSE;	//��Ƽ���� ���� �ƴϴ�.
				}
				if( PKMGR->IsPKAllow() )
				if( WANTEDMGR->IsChrIDInList( pTarget->GetID() ) )
					return TRUE;	//ô�� ����ڴ� ���̴�.
			}
#endif
//---KES CHEAT PKEVENT �Ʒ� �ּ�ó���Ͽ� ������� �ƴ϶� �̺�Ʈ �����ϵ���
//			if( PKMGR->IsPKAllow() )
			{
				if( ((CPlayer*)pOperator)->IsPKMode() )				
				{
					if(pOperator == pTarget )
						return FALSE;
					else
						return TRUE;	//�ڽ��� PK����̸� �ڽ��� �� �������� ���̴�.
				}
				if( ((CPlayer*)pTarget)->IsPKMode() )
					return TRUE;	//��밡 PK����̸� ���̴�.
			}

			return FALSE;
		}
		else if( pOperator->GetObjectKind() & eObjectKind_Monster )
		{	// Operator �� �����ϰ��	//��� ���� �ɸ� ���� ����. ���ʹ� �񹫸� ���ϹǷ�
			if( pTarget->GetObjectKind() & eObjectKind_Monster )	// ���ʹ� ���� �ƴ�
				return FALSE;
		}
		else if(pTarget->GetObjectKind() == eObjectKind_SkillObject)
		{
			return TRUE;
		}
else if(pTarget->GetObjectKind() == eObjectKind_Pet )
		{
#ifdef _MAPSERVER_
			if( ( ( CPet* )pTarget )->GetMaster() )
			{
				return IsEnemy( pOperator, ( ( CPet* )pTarget )->GetMaster() );
			}
			else
				return FALSE;
#else
			if( ( ( CPet* )pTarget )->GetMaster() )
			{
				return IsEnemy( pOperator, ( ( CPet* )pTarget )->GetMaster() );
			}
			else
				return FALSE;
#endif
		}
		else	//�������� ���� �ƴϴ�.
		{
			return FALSE;
		}
	}
	else if(pOperator->GetObjectKind() == eObjectKind_Pet )
	{
#ifdef _MAPSERVER_
		if( ( ( CPet* )pOperator )->GetMaster() )
		{
			return IsEnemy( ( ( CPet* )pOperator )->GetMaster(), pTarget );
		}
		else
			return FALSE;
#else
		if( ( ( CPet* )pOperator )->GetMaster() )
		{
			return IsEnemy( ( ( CPet* )pOperator )->GetMaster(), pTarget );
		}
		else
			return FALSE;
#endif
	}
	return TRUE;
}


BOOL CBattle_Showdown::IsFriend(CObject* pOperator,CObject* pTarget)
{
	if(pOperator->GetBattleID() != pTarget->GetBattleID())
		return FALSE;
	
	if(pOperator->GetBattleTeam() != pTarget->GetBattleTeam())
		return FALSE;
	
	return TRUE;
}

// event func
void CBattle_Showdown::OnCreate(BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2)
{
	CBattle::OnCreate(pCreateInfo,pTeam1,pTeam2);

	memcpy(&m_ShowdownInfo,pCreateInfo,sizeof(BATTLE_INFO_SHOWDOWN));

	m_Team[0].SetCharacterID(m_ShowdownInfo.Character[SHOWDOWNTEAM_BLUE]);	
	m_Team[1].SetCharacterID(m_ShowdownInfo.Character[SHOWDOWNTEAM_RED]);

#ifndef _MAPSERVER_	
	m_ImageNumber.Init( 32, 0 );
	m_ImageNumber.SetFillZero( FALSE );
	m_ImageNumber.SetLimitCipher( 2 );
	//m_ImageNumber.SetPosition( 640 - 16, 80 );
	m_ImageNumber.SetScale( 3.0f, 3.0f );
	DISPLAY_INFO	dispInfo2 ;
	GAMERESRCMNGR->GetDispInfo( &dispInfo2 ) ;
	m_ImageNumber.SetPosition( (dispInfo2.dwWidth-16)/2, 150 );

	m_dwFadeOutStartTime = 0;
	m_bFadeOut	 = FALSE;

//	SCRIPTMGR->GetImage( 59, &m_ImageReady, PFT_HARDPATH );
	SCRIPTMGR->GetImage( 59, &m_ImageStart, PFT_HARDPATH );
	SCRIPTMGR->GetImage( 60, &m_ImageWin, PFT_HARDPATH );
	SCRIPTMGR->GetImage( 61, &m_ImageLose, PFT_HARDPATH );
	SCRIPTMGR->GetImage( 62, &m_ImageDraw, PFT_HARDPATH );
	
//	m_pCurShowImage = &m_ImageReady;

	// 070202 LYW --- CBattle_Showdown : Modified limited area.
	//m_vTitlePos.x = (1280 - 512) / 2;	//128 width
	DISPLAY_INFO	dispInfo ;
	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;
	m_vTitlePos.x = (float)(dispInfo.dwWidth - 512)/2;	//128 width
	// 070202 LYW --- End.
	m_vTitlePos.y = 32;
	m_vTitleScale.x = 1.0f;
	m_vTitleScale.y = 1.0f;
	
	CTileManager* pTileManager = MAP->GetTileManager();

	//������ ������...
	VECTOR3 vPos	= m_ShowdownInfo.vStgPos;
	VECTOR3 vStgPos;

	float fx, fz = vPos.z;
//	RECT rc = { -100.0f, -50.0f, -50.0f, 0.0f };
	float fAdd = -50.0f;

	for( fx = vPos.x - 1000.0f ; fx <= vPos.x + 1000.0f + fAdd ; fx += 50.0f )//
	{
		vStgPos.x = fx;
		vStgPos.z = fz - 1000.0f;
		pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.z = fz + 1000.0f + fAdd;
		pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}

	fx = vPos.x;
	for( fz = vPos.z - 1000.0f ; fz <= vPos.z + 1000.0f + fAdd; fz += 50.0f )
	{
		vStgPos.x = fx - 1000.0f;
		vStgPos.z = fz;
		pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.x = fx + 1000.0f + fAdd;
		vStgPos.z = fz;
		pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}

//---�ӽ�	
	fz = vPos.z;
	for( fx = vPos.x - 1050.0f ; fx <= vPos.x + 1050.0f + fAdd ; fx += 50.0f )//
	{
		vStgPos.x = fx;
		vStgPos.z = fz - 1050.0f;
		pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.z = fz + 1050.0f + fAdd;
		pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}

	fx = vPos.x;
	for( fz = vPos.z - 1050.0f ; fz <= vPos.z + 1050.0f + fAdd ; fz += 50.0f )
	{
		vStgPos.x = fx - 1050.0f;
		vStgPos.z = fz;
		pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.x = fx + 1050.0f + fAdd;
		vStgPos.z = fz;
		pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}
//------------------

	//�Ÿ��ȿ� �ֳ�? ������? confirm
	
	CPlayer* pPlayer1 = (CPlayer*)OBJECTMGR->GetObject( m_ShowdownInfo.Character[0] );
	CPlayer* pPlayer2 = (CPlayer*)OBJECTMGR->GetObject( m_ShowdownInfo.Character[1] );
	if( pPlayer1 )
	if( pPlayer1->GetState() == eObjectState_BattleReady )
		OBJECTSTATEMGR->EndObjectState(pPlayer1, eObjectState_BattleReady );
	if( pPlayer2 )
	if( pPlayer2->GetState() == eObjectState_BattleReady )
		OBJECTSTATEMGR->EndObjectState(pPlayer2, eObjectState_BattleReady );

#else

//	m_bDieByOp = FALSE;
	m_bDieByOp[0] = FALSE;
	m_bDieByOp[1] = FALSE;

	CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser(m_ShowdownInfo.Character[0]);
	CPlayer* pPlayer2 = (CPlayer*)g_pUserTable->FindUser(m_ShowdownInfo.Character[1]);
	if( pPlayer1 )
	if( pPlayer1->GetState() == eObjectState_BattleReady )
		OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer1, eObjectState_BattleReady);

	if( pPlayer2 )
	if( pPlayer2->GetState() == eObjectState_BattleReady )
		OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer2, eObjectState_BattleReady);

#endif


//���� ó��....
//-����Ʈ�� ó���Ѵ�.
//-��Ʈ�� ������ �ʰ� ��븦 ���ݰ����ϵ��� �Ѵ�.
//-�ٸ� ĳ������ PK�� ���´�.

}

void CBattle_Showdown::OnFightStart()
{
	CBattle::OnFightStart();

#ifndef _MAPSERVER_
	m_pCurShowImage = &m_ImageStart;
	m_ImageNumber.SetFillZero( TRUE );
	m_ImageNumber.SetFadeOut( 0 );	//fadeout off
	m_ImageNumber.SetScale( 1.0f, 1.0f );
//	m_ImageNumber.SetPosition( 640 + 32, 130 );
	DISPLAY_INFO	dispInfo2 ;
	GAMERESRCMNGR->GetDispInfo( &dispInfo2 ) ;
	m_ImageNumber.SetPosition( (dispInfo2.dwWidth-32)/2, 150 );
#endif
	
}

void CBattle_Showdown::OnDestroy()
{

#ifndef _MAPSERVER_	

	CTileManager* pTileManager = MAP->GetTileManager();
	//������ ��ü
	VECTOR3 vPos	= m_ShowdownInfo.vStgPos;
	VECTOR3 vStgPos;

	float fx, fz = vPos.z;
	float fAdd = -50.0f;
	
	for( fx = vPos.x - 1000.0f ; fx <= vPos.x + 1000.0f + fAdd ; fx += 50.0f )
	{
		vStgPos.x = fx;
		vStgPos.z = fz - 1000.0f;
		pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.z = fz + 1000.0f + fAdd;
		pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}
	fx = vPos.x;
	for( fz = vPos.z - 1000.0f ; fz <= vPos.z + 1000.0f + fAdd ; fz += 50.0f )
	{
		vStgPos.x = fx - 1000.0f;
		vStgPos.z = fz;
		pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.x = fx + 1000.0f + fAdd;
		vStgPos.z = fz;
		pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}

//---�ӽ�	
	fz = vPos.z;
	for( fx = vPos.x - 1050.0f ; fx <= vPos.x + 1050.0f + fAdd ; fx += 50.0f )//
	{
		vStgPos.x = fx;
		vStgPos.z = fz - 1050.0f;
		pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.z = fz + 1050.00f + fAdd;
		pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}

	fx = vPos.x;
	for( fz = vPos.z - 1050.0f ; fz <= vPos.z + 1050.0f + fAdd; fz += 50.0f )
	{
		vStgPos.x = fx - 1050.0f;
		vStgPos.z = fz;
		pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.x = fx + 1050.0f + fAdd;
		vStgPos.z = fz;
		pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}
//------------------

#else

	CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser(m_ShowdownInfo.Character[0]);
	CPlayer* pPlayer2 = (CPlayer*)g_pUserTable->FindUser(m_ShowdownInfo.Character[1]);
	if( pPlayer1 )
	{
		pPlayer1->SetBattleID(pPlayer1->GetGridID());
		pPlayer1->SetShowdown( FALSE );

		/////////////////////////////////////////////////////////////////////////////////
		// 06. 06. ���ÿ� ������ 2�� Player�� �� ���� ó�� �ȵǴ� ���� �ذ� - �̿���
		// m_bDieByOp -> m_bDieByOp[0]
		if( pPlayer1->GetState() == eObjectState_Die && m_bDieByOp[0] )
		{
			pPlayer1->ReviveAfterShowdown();
			m_bDieByOp[0] = FALSE;
		}
	}
	if( pPlayer2 )
	{
		pPlayer2->SetBattleID(pPlayer2->GetGridID());
		pPlayer2->SetShowdown( FALSE );
		
		/////////////////////////////////////////////////////////////////////////////////
		// 06. 06. ���ÿ� ������ 2�� Player�� �� ���� ó�� �ȵǴ� ���� �ذ� - �̿���
		// m_bDieByOp -> m_bDieByOp[1]
		if( pPlayer2->GetState() == eObjectState_Die && m_bDieByOp[1] )
		{
			pPlayer2->ReviveAfterShowdown();
			m_bDieByOp[1] = FALSE;
		}
	}

#endif

	CBattle::OnDestroy();
	
}

void CBattle_Showdown::OnTeamMemberAdd( int Team, DWORD MemberID, char* Name )
{
#ifdef _MAPSERVER_
/*
	if( Team == eBattleTeam1 )
	{
		CObject* pObject = g_pUserTable->FindUser(MemberID);
		ASSERT(pObject);

		MSG_BATTLE_SHOWDOWN_CREATESTAGE msg;
		msg.Category = MP_BATTLE;
		msg.Protocol = MP_BATTLE_SHOWDOWN_CREATESTAGE;
		msg.dwObjectID = pObject->GetID();
		msg.dwBattleID = pObject->GetBattleID();
		msg.vPosStage = m_ShowdownInfo.vStgPos;
		
		PACKEDDATA_OBJ->QuickSend( pObject, &msg, sizeof(msg) );
	}
*/
	CObject* pObject = g_pUserTable->FindUser(MemberID);
	ASSERT(pObject);

	MSG_BATTLE_SHOWDOWN_CREATESTAGE msg;
	msg.Category = MP_BATTLE;
	msg.Protocol = MP_BATTLE_SHOWDOWN_CREATESTAGE;
	msg.dwObjectID = pObject->GetID();
	msg.dwBattleID = pObject->GetBattleID();
	msg.vPosStage = m_ShowdownInfo.vStgPos;
	pObject->SendMsg( &msg, sizeof(msg) );

//#else
	
#endif
	//���� ����Ʈ ������.
}

BOOL CBattle_Showdown::OnTeamMemberDie(int Team,DWORD VictimMemberID,DWORD KillerID)
{
	ASSERT(Team < eBattleTeam_Max);
	
#ifdef _MAPSERVER_
	CObject* pVictimMember = g_pUserTable->FindUser(VictimMemberID);
	CObject* pKiller = g_pUserTable->FindUser(KillerID);
	if(pVictimMember == NULL || pKiller == NULL)
	{
		ASSERT(0);
		return FALSE;
	}
	
	ASSERT(pVictimMember->GetBattleID() == GetBattleID());

	if( pVictimMember->GetObjectKind() != eObjectKind_Player )
	{
		return FALSE;
	}
	
	//�����.....
	((CBattleTeam_Showdown*)m_TeamArray[Team])->AddTeamDieNum();

	//Ÿĳ���̳� �������� �׾���.
	//
	if(pKiller->GetBattleID() != GetBattleID())	//���� PK���
	{
		if( pKiller->GetObjectKind() == eObjectKind_SkillObject )
		{
			CObject* pOperator = ((cSkillObject*)pKiller)->GetOperator();

			if( !pOperator )
				return FALSE;

			if( pOperator->GetBattleID() != GetBattleID() )
				return FALSE;

			ASSERT(0);	//debug.. ���������� ��Ʋ���̵�� ��ų��Ʋ���̵� Ʋ����!!
		}
		else if( pKiller->GetObjectKind() == eObjectKind_Pet )
		{
			CObject* pOperator = ((CPet*)pKiller)->GetMaster();

			if( !pOperator )
				return FALSE;

			if( pOperator->GetBattleID() != GetBattleID() )
				return FALSE;

			ASSERT(0);	

		}
		else
		{
			return FALSE;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////
	// 06. 06. ���ÿ� ������ 2�� Player�� �� ���� ó�� �ȵǴ� ���� �ذ� - �̿���
	// �񹫷� �׾����� �Ǵ��ϴ� ������ �ϳ� �ۿ� ���
	// ���ÿ� ������ 2�� �ɸ��ʹ� Ȯ���� �ȵǾ� �Ϲ� ����ó���� �Ǵ� ���� �ذ�
	//	m_bDieByOp = TRUE; //���ĳ������ �׾���.
	if(m_ShowdownInfo.Character[0] == VictimMemberID)
		m_bDieByOp[0] = TRUE; 
	else
		m_bDieByOp[1] = TRUE;

	((CPlayer*)pVictimMember)->m_bNeedRevive = FALSE;		//��Ȱâ�� �ʿ����.
	
	SKILLMGR->ReleaseAllSkillInBattle(GetBattleID());

	MSG_DWORD3 msg;
	SetProtocol(&msg,MP_BATTLE,MP_BATTLE_TEAMMEMBER_DIE_NOTIFY);
	msg.dwData1 = Team;
	msg.dwData2 = VictimMemberID;
	msg.dwData3 = KillerID;
	ToEachTeam(pTeam)
		pTeam->SendTeamMsg(&msg,sizeof(msg));
	EndToEachTeam

	
#endif


//	return TRUE;

//	FALSE�� �����ϸ� ������ ó���� �Ѵ�. CPlayer(CMonster)::DoDie �Լ��� ȣ���Ѵ�.
	return FALSE;
	
}

void CBattle_Showdown::OnTeamObjectCreate(int Team,CBattleObject* pBattleObject)
{
}

void CBattle_Showdown::OnTeamObjectDestroy(int Team,CBattleObject* pBattleObject)
{
}

BOOL CBattle_Showdown::OnTeamMemberDelete(int Team,DWORD MemberID,char* Name)
{	
#ifdef _MAPSERVER_
/*
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( MemberID );

		if( pPlayer )
		if( pPlayer->GetState() == eObjectState_Die )
			pPlayer->ReviveAfterShowdown();	
*/
#endif

	return TRUE;
}


//server������ ���
void CBattle_Showdown::OnTick()
{
	ySWITCH(m_BattleInfo.BattleState)
		yCASE(eBATTLE_STATE_READY)
//			DWORD ElapTime = gCurTime - m_BattleInfo.BattleTime;
//			if( ElapTime > BATTLE_SHOWDOWN_READYTIME)
//			{
//				ToEachTeam(pTeam)
//					if(pTeam->GetAliveTeamMemberNum() == 0)
//						break;
//				EndToEachTeam
				
//				StartBattle();
//			}
			
			DWORD ElapTime = gCurTime - m_BattleInfo.BattleTime;
			if( ElapTime > BATTLE_SHOWDOWN_READYTIME )
			{
				for( int n = 0; n < eBattleTeam_Max; ++n )
				{
					if( m_TeamArray[n]->GetAliveTeamMemberNum() == 0 )
					{
						Draw();
						return;
					}
				}

				StartBattle();
			}

		yCASE(eBATTLE_STATE_FIGHT)
			Judge();

		yCASE(eBATTLE_STATE_RESULT)
			DWORD ElapTime = gCurTime - m_BattleInfo.BattleTime;
			if( ElapTime > BATTLE_SHOWDOWN_RESULTTIME)
			{
				SetDestroyFlag();
				
			}
	
	yENDSWITCH
}

// ���� ����

BOOL CBattle_Showdown::JudgeOneTeamWinsOtherTeam(int TheTeam,int OtherTeam)
{
	CBattleTeam_Showdown* pTheTeam = (CBattleTeam_Showdown*)m_TeamArray[TheTeam];
	CBattleTeam_Showdown* pOtherTeam = (CBattleTeam_Showdown*)m_TeamArray[OtherTeam];

//	if(pOtherTeam->GetAliveTeamMemberNum() == 0)
	if( pTheTeam->GetTeamDieNum() == 0 )				//KKK
	if( pOtherTeam->GetTeamDieNum() != 0 )
	{
		Victory( TheTeam, OtherTeam );
		return TRUE;
	}

	return FALSE;
}

 
BOOL CBattle_Showdown::Judge()
{

	if(JudgeOneTeamWinsOtherTeam(eBattleTeam1,eBattleTeam2) == TRUE)
		return TRUE;

	if(JudgeOneTeamWinsOtherTeam(eBattleTeam2,eBattleTeam1) == TRUE)
		return TRUE;

	if( ((CBattleTeam_Showdown*)m_TeamArray[eBattleTeam1])->GetTeamDieNum()
		&& ((CBattleTeam_Showdown*)m_TeamArray[eBattleTeam2])->GetTeamDieNum() )
		Draw();


	DWORD ElapsedTime = gCurTime - m_BattleInfo.BattleTime;
	if(ElapsedTime > BATTLE_SHOWDOWN_FIGHTTIME)
		Draw();

	return FALSE;
}


void CBattle_Showdown::Victory(int WinnerTeamNum,int LoserTeamNum)
{
	CBattle::Victory(WinnerTeamNum,LoserTeamNum);

#ifndef _MAPSERVER_
	if( HERO->GetBattleTeam() == WinnerTeamNum )
	{
		m_pCurShowImage = &m_ImageWin;
	}
	else if( HERO->GetBattleTeam() == LoserTeamNum )
	{
		m_pCurShowImage = &m_ImageLose;
	}


	m_vTitleScale.x = 1.0f;
	m_vTitleScale.y = 1.0f;
	// 070202 LYW --- CBattle_Showdown : Modified limited area.
	//m_vTitlePos.x = ( 1280 - 512 * m_vTitleScale.x ) / 2;	//128 = width
	DISPLAY_INFO	dispInfo ;
	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;
	m_vTitlePos.x = (float)(dispInfo.dwWidth - 512)/2;	//128 width
	// 070202 LYW --- End.
//	m_vTitlePos.y = ( 768 - 128 * m_vTitleScale.y ) / 2;		//42 = height
	m_vTitlePos.y = 100;

	//AutoAttack����
	HERO->DisableAutoAttack();					//������ �̴��� ��ҵǳ�?
	HERO->SetNextAction(NULL);			//��ų �����͵� ��ҵǳ�?

#else
//	SKILLMGR->ReleaseAllSkillInBattle(GetBattleID());

	CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser(m_ShowdownInfo.Character[0]);
	CPlayer* pPlayer2 = (CPlayer*)g_pUserTable->FindUser(m_ShowdownInfo.Character[1]);
	CPlayer* pWinner = NULL;
	CPlayer* pLoser = NULL;

	if( WinnerTeamNum == 0 )
	{
		pWinner = pPlayer1;
		pLoser = pPlayer2;
	}
	else if( WinnerTeamNum == 1 )
	{
		pWinner = pPlayer2;
		pLoser = pPlayer1;
	}

	if( pPlayer1 == NULL || pPlayer2 == NULL || pWinner == NULL )
		return;

//	char buf[128];
//	sprintf( buf, "%s�԰� %s���� �񹫿��� %s���� �¸��ϼ̽��ϴ�.",
//			 pPlayer1->GetObjectName(),
//			 pPlayer2->GetObjectName(),
//			 pWinner->GetObjectName() );

	MSG_DWORD2 resultMsg;
	resultMsg.Category		= MP_SIGNAL;
	resultMsg.Protocol		= MP_SIGNAL_SHOWDOWN_RESULT;
	resultMsg.dwObjectID	= pWinner->GetID();
	resultMsg.dwData1		= pWinner->GetID();		//winner
	resultMsg.dwData2		= pLoser->GetID();		//loser
	PACKEDDATA_OBJ->QuickSend( pWinner, &resultMsg, sizeof(resultMsg) );
/*
	TESTMSG resultMsg;
	resultMsg.Category		= MP_SIGNAL;
	resultMsg.Protocol		= MP_SIGNAL_SHOWDOWN_RESULT;
	resultMsg.dwObjectID	= pWinner->GetID();
	//strcpy( resultMsg.Msg, buf );
	SafeStrCpy( resultMsg.Msg, buf, MAX_CHAT_LENGTH+1);
	//PACKEDDATA_OBJ->QuickSend( pWinner, (MSGBASE*)&resultMsg, sizeof(TESTMSG) );
	PACKEDDATA_OBJ->QuickSend( pWinner, (MSGBASE*)&resultMsg, resultMsg.GetMsgLength() );	//CHATMSG 040324
*/	

#endif
}

void CBattle_Showdown::Draw()
{
	CBattle::Draw();	

#ifndef _MAPSERVER_
	m_pCurShowImage = &m_ImageDraw;

	m_vTitleScale.x = 1.0f;
	m_vTitleScale.y = 1.0f;
	// 070202 LYW --- CBattle_Showdown : Modified limited area.
	//m_vTitlePos.x = (1280 - 512 * m_vTitleScale.x ) / 2;	//128 = width
	DISPLAY_INFO	dispInfo ;
	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;
	m_vTitlePos.x = (float)(dispInfo.dwWidth - 512)/2;	//128 width
	// 070202 LYW --- End.
//	m_vTitlePos.y = ( 768 - 128 * m_vTitleScale.y ) / 2;		//42 = height
	m_vTitlePos.y = 100;
	
	//AutoAttack����
	HERO->DisableAutoAttack();					//������ �̴��� ��ҵǳ�?
	HERO->SetNextAction(NULL);			//��ų �����͵� ��ҵǳ�?
//#else
////	SKILLMGR->ReleaseAllSkillInBattle(GetBattleID());
//
//	CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser(m_ShowdownInfo.Character[0]);
//	CPlayer* pPlayer2 = (CPlayer*)g_pUserTable->FindUser(m_ShowdownInfo.Character[1]);
//
//	if( pPlayer1 == NULL || pPlayer2 == NULL  )
//		return;
//
//	char buf[128];
//	sprintf( buf, "%s�԰� %s���� �񹫰� ���ºη� �������ϴ�.",
//			 pPlayer1->GetObjectName(),
//			 pPlayer2->GetObjectName() );
//
//	TESTMSG resultMsg;
//	resultMsg.Category		= MP_SIGNAL;
//	resultMsg.Protocol		= MP_SIGNAL_BATTLE;
//	resultMsg.dwObjectID	= pPlayer1->GetID();
//	//strcpy( resultMsg.Msg, buf );	
//	SafeStrCpy( resultMsg.Msg, buf, MAX_CHAT_LENGTH+1 );
//
//	//PACKEDDATA_OBJ->QuickSend( pPlayer1, (MSGBASE*)&resultMsg, sizeof(TESTMSG) );
//	PACKEDDATA_OBJ->QuickSend( pPlayer1, (MSGBASE*)&resultMsg, resultMsg.GetMsgLength() );	//CHATMSG 040324
	
#endif
}



#ifdef _CLIENT_

#include "interface/cFont.h"
//#include "interface/cImage.h"
#include "ImageNumber.h"

void CBattle_Showdown::Render()
{


//Title
//	RECT rect = { 30, 100, 500, 150 };
//	char temp[256];
//	sprintf( temp, "Showdown Battle" );
//	CFONT_OBJ->RenderFont( 0, temp, strlen(temp), &rect, 0xffffffff );

//���ѽð�...

//	SetRect( &rect, 30, 120, 500, 150 );
//	RECT rect = { 30, 100, 500, 150 };
	DWORD ElapsedTime = gCurTime-m_BattleInfo.BattleTime;


	switch(m_BattleInfo.BattleState )
	{
	case eBATTLE_STATE_READY:
		{
			//ȭ���߰���
			DWORD RemainTime = BATTLE_SHOWDOWN_READYTIME > ElapsedTime ? BATTLE_SHOWDOWN_READYTIME - ElapsedTime : 0;
			//sprintf( temp, "Battle Start in %d sec", RemainTime / 1000 );
			m_ImageNumber.SetNumber( RemainTime / 1000 );
			if( m_ImageNumber.IsNumberChanged() )
				m_ImageNumber.SetFadeOut( 500 );

			m_ImageNumber.Render();
			
//			if( m_pCurShowImage )
//				m_pCurShowImage->RenderSprite( &m_vTitleScale, NULL, 0.0f, &m_vTitlePos, 0xffffffff );
		}
		break;

	case eBATTLE_STATE_FIGHT:
		{
			if( m_pCurShowImage )
				m_pCurShowImage->RenderSprite( &m_vTitleScale, NULL, 0.0f, &m_vTitlePos, 0xffffffff );

			//ȭ���߰��� ���Դٰ� ����?
			DWORD RemainTime = BATTLE_SHOWDOWN_FIGHTTIME > ElapsedTime ? BATTLE_SHOWDOWN_FIGHTTIME - ElapsedTime : 0;
			//sprintf( temp, "Battle End in %d sec", RemainTime / 1000 );
			m_ImageNumber.SetNumber( RemainTime / 1000 );

//			m_ImageNumber.SetPosition( 512 + 32, 130 ); //130
//			m_ImageNumber.SetPosition( 512 + 32, 30 ); //130
			m_ImageNumber.Render();			

		}
		break;

	case eBATTLE_STATE_RESULT:
		{
			DWORD dwAlpha = 255;
			if( !m_bFadeOut )
			{
				if( ElapsedTime > 5500 )
				{
					m_dwFadeOutStartTime = gCurTime;
					m_bFadeOut = TRUE;
				}
			}
			else
			{
				DWORD dwElapsed = gCurTime - m_dwFadeOutStartTime;
				DWORD dwMinusAlpha = 255 * dwElapsed / 1000;
				if( dwMinusAlpha < 255 )
				{
					dwAlpha = 255 - dwMinusAlpha;
				}
				else
				{
					dwAlpha = 0;
				}
			}

			if(m_WinnerTeam == eBattleTeam_Max)
			{				
				if( m_pCurShowImage )
					m_pCurShowImage->RenderSprite( &m_vTitleScale, NULL, 0.0f, &m_vTitlePos, 0x00ffffff | (dwAlpha << 24) );
			}
			else
			{
				if(ElapsedTime > 2500)
				{
					if( m_pCurShowImage )
						m_pCurShowImage->RenderSprite( &m_vTitleScale, NULL, 0.0f, &m_vTitlePos, 0x00ffffff | (dwAlpha << 24) );
				}
			}
		}
		break;
	}

	//CFONT_OBJ->RenderFont(0,temp,strlen(temp),&rect,0xffffffff);	

}

#endif
// } end virtual func
//////////////////////////////////////////////////////////////////////////
