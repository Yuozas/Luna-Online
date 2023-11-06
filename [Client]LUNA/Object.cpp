#include "stdafx.h"
#include "Object.h"
#include "MoveManager.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "ObjectStateManager.h"
#include "interface/cWindowManager.h"
#include "Interface/cScriptManager.h"
#include "ObjectBalloon.h"
#include "OBalloonChat.h"
#include "OBalloonName.h"
#include "StreetStallTitleTip.h"

#include "ObjectGuagen.h"
#include "GameResourceManager.h"
#include "MOTIONDESC.h"
#include "BattleSystem_Client.h"
#include "MonsterGuageDlg.h"
#include "ChatManager.h"

// 070228 LYW --- Object : Include header file for test.
#include "FacialManager.h"

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.10.24
#include "../hseos/Farm/SHFarmManager.h"
// E 농장시스템 추가 added by hseos 2007.10.24

#include "PKManager.h"
// desc_hseos_결혼_01
// S 결혼 추가 added by hseos 2007.12.11
#include "../hseos/Marriage/SHMarriageManager.h"
// E 결혼 추가 added by hseos 2007.12.11

#include "PKManager.h"
#include "petmanager.h"

CObject::CObject()
{
	m_pObjectBalloon		= NULL;
	m_bObjectBalloonBits	= 0;
	m_StateEffectList.Initialize(3);
	m_pMotionDESC			= NULL;
	m_bSelected				= FALSE;

	m_ObjectKind			= 0;
}

CObject::~CObject()
{
	Release();
}


BOOL CObject::Init(EObjectKind kind,BASEOBJECT_INFO* pBaseObjectInfo)
{
	m_bMoveSkipCount = 0;
	m_bIsYMoving = FALSE;

	// desc_hseos_전투평화상태01
	// S 전투평화상태 주석처리 added by hseos 2007.08.02
	pBaseObjectInfo->ObjectBattleState = 0;
	// pBaseObjectInfo->ObjectBattleState = eObjectBattleState_Battle;
	// E 전투평화상태 주석처리 added by hseos 2007.08.02
	memcpy(&m_BaseObjectInfo,pBaseObjectInfo,sizeof(m_BaseObjectInfo));

	m_ObjectKind = kind;
	m_ObjectState.bEndState = FALSE;

	SetFlag(FALSE);
//m_bDieFlag = FALSE;
	m_bInited = TRUE;

	mBattleStateCheckDelay.Init( 10000 );

	return TRUE;
}


// 071214 LUJ, 코드 정리
void CObject::InitObjectBalloon(BYTE bitFlag)
{
	m_pObjectBalloon		= new CObjectBalloon;
	m_bObjectBalloonBits	= bitFlag;

	switch( GetObjectKind() )
	{
	case eObjectKind_Monster:
		{
			// 080616 LUJ, 잘못된 정보가 와서 가끔 튕기고 있음. 이를 막기 위해 널포인터 검사 추가
			const BASE_MONSTER_LIST* monsterList = ( ( CMonster* )this )->GetSInfo();

			if( monsterList )
			{
				m_pObjectBalloon->InitBalloon( this, monsterList->Tall, bitFlag );
			}

			break;
		}
	case eObjectKind_Player:
		{
			m_pObjectBalloon->InitBalloon(this, 180, bitFlag);
			break;
		}
	case eObjectKind_Npc:
		{
			CNpc* npc = ( CNpc* )this;
			m_pObjectBalloon->InitBalloon( npc, npc->GetSInfo()->Tall, bitFlag );
			break;
		}
	case eObjectKind_FarmObj:
		{
			m_pObjectBalloon->InitBalloon(this, g_csFarmManager.CLI_GetFarmRenderObjList(((CSHFarmRenderObj*)this)->GetInfo()->nKind)->nNameYpos, bitFlag);
			break;
		}
	case eObjectKind_Pet:
		{
			m_pObjectBalloon->InitBalloon(this, PETMGR->GetNameHeight(), bitFlag);
			break;
		}
	default:
		{
			m_pObjectBalloon->InitBalloon(this, 180, bitFlag);
			break;
		}
	}

	if(! ( bitFlag & ObjectBalloon_Name ) )
	{
		return;
	}

	COBalloonName* balloonName =  m_pObjectBalloon->GetOBalloonName();

	if( ! balloonName )
	{
		return;
	}

	switch( GetObjectKind() )
	{
	case eObjectKind_Monster:
		{			
			balloonName->InitName(((CMonster *)this)->GetSInfo()->Name);
			break;
		}
	case eObjectKind_Player:
		{	
			balloonName->InitName(m_BaseObjectInfo.ObjectName);

			CPlayer* player = ( CPlayer* )this;

			if( player->GetGuildMarkName() ) 
			{
				SetGuildMark();
			}

			SetNickName();

			if( player->GetFamilyMarkName() ) 
			{
				SetFamilyMark();
			}

			m_pObjectBalloon->SetFamilyNickName( player->GetFamilyNickName() );

			// desc_hseos_결혼_01
			// S 결혼 추가 added by hseos 2007.12.11
			// ..결혼을 했으면 이름앞에 호칭 추가
//			g_csMarriageManager.CLI_SetNickname(player, player->GetCharacterTotalInfo()->szMarriagePartnerName, player->GetCharacterTotalInfo()->szMarriagePartnerNickName);
			// E 결혼 추가 added by hseos 2007.12.11
			break;
		}
	case eObjectKind_Npc:
		{				
			CNpc* npc = ( CNpc* )this;

			if( npc->GetSInfo()->ShowJob )
			{
				m_pObjectBalloon->SetNickName( npc->GetSInfo()->Name );
			}

			const MAPCHANGE_INFO* info  = GAMERESRCMNGR->GetMapChangeInfo( npc->GetNpcUniqueIdx() );

			balloonName->InitName( info ? info->ObjectName : m_BaseObjectInfo.ObjectName );
			break;
		}
	case eObjectKind_FarmObj:
		{
			balloonName->InitName( m_BaseObjectInfo.ObjectName );
			break;
		}
	default:
		{
			balloonName->InitName( m_BaseObjectInfo.ObjectName );
			break;
		}
	}
}

//void CObject::AddChatBalloon( char * chatMsg )
//{
//	ShowChatBalloon( TRUE, chatMsg );
//}

//-----
void CObject::SetOverInfoOption( DWORD dwOption )
{
	if( m_pObjectBalloon )	
		m_pObjectBalloon->SetOverInfoOption( dwOption );
}

void CObject::SetGuildMark()
{
	if( m_pObjectBalloon )
		m_pObjectBalloon->SetGuildMark( ((CPlayer*)this)->GetGuildMarkName() );
}

void CObject::SetFamilyMark()
{
	if( m_pObjectBalloon )
		m_pObjectBalloon->SetFamilyMark( ((CPlayer*)this)->GetFamilyMarkName() );
}

void CObject::SetNickName()
{
	if( m_pObjectBalloon )
		m_pObjectBalloon->SetNickName( ((CPlayer*)this)->GetNickName() );
}

void CObject::ShowObjectName( BOOL bShow, DWORD dwColor )		//show/hide
{
	if( !m_pObjectBalloon )								return;
	if( !(m_bObjectBalloonBits & ObjectBalloon_Name) )	return;

	m_pObjectBalloon->ShowObjectName( bShow, dwColor );
}

void CObject::ShowChatBalloon( BOOL bShow, char* chatMsg, DWORD dwColor, DWORD dwAliveTime )
{
	if( !m_pObjectBalloon )								return;
	if( !(m_bObjectBalloonBits & ObjectBalloon_Chat) )	return;

	m_pObjectBalloon->ShowChatBalloon( bShow, chatMsg, dwColor, dwAliveTime );
}

//-----

void CObject::InitMove(BASEMOVE_INFO* pMoveInfo)
{
	memset(&m_MoveInfo,0,sizeof(MOVE_INFO));
	memcpy(&m_MoveInfo,pMoveInfo,sizeof(BASEMOVE_INFO));
}

void CObject::Release()
{
	m_StatusList.RemoveAll();
	
	m_bMoveSkipCount = 0;
	m_bInited = FALSE;

	if(m_ShadowObj.IsInited())
	{
		m_ShadowObj.Release();
	}
	RemoveAllObjectEffect();

	m_ObjectKind = 0;
	m_BaseObjectInfo.ObjectState = -1;
	m_EngineObject.Release();
	
	SAFE_DELETE( m_pObjectBalloon );
	m_bObjectBalloonBits = 0;
}

void CObject::GetBaseObjectInfo(BASEOBJECT_INFO* pRtInfo)
{
	memcpy(pRtInfo,&m_BaseObjectInfo,sizeof(BASEOBJECT_INFO));
}

void CObject::GetBaseMoveInfo(BASEMOVE_INFO* pRtInfo)
{
	memcpy(pRtInfo,&m_MoveInfo,sizeof(BASEMOVE_INFO));
}

void CObject::Process()
{
	OBJECTSTATEMGR->StateProcess(this);
	MOVEMGR->MoveProcess(this);

	if(m_ShadowObj.IsInited())
		m_ShadowObj.SetEngObjPosition(&GetCurPosition());
	
	/*
	HEFFPROC hEff;
	m_StateEffectList.SetPositionHead();
	while(hEff = m_StateEffectList.GetData())
		EFFECTMGR->Process(gCurTime,hEff);
	*/

	// desc_hseos_전투평화상태01
	// S 전투평화상태 주석처리 added by hseos 2007.08.02
	if( m_BaseObjectInfo.ObjectBattleState )
	{
		if( !mBattleStateCheckDelay.Check() )
		{
			SetObjectBattleState( eObjectBattleState_Peace );
		}
	}
	// E 전투평화상태 주석처리 added by hseos 2007.08.02
}

void CObject::SetFlag(BOOL val)
{
	m_bDieFlag = val; 
}

void CObject::SetDieFlag()
{
	m_DiedTime = gCurTime;

	// 죽었을때의 처리
	SetFlag(TRUE);
//	m_bDieFlag = TRUE;
	MOVEMGR->MoveStop(this);
	HERO->ClearTarget(this);

	if( HEROPET )
		HEROPET->ClearTarget(this);

	// LBS 03.11.21
	if( OBJECTMGR->GetSelectedObjectID() == GetID() )
	{
		// 070510 LYW --- Object : Modified.
		//OBJECTMGR->SetSelectedObjectID(0);
		GAMEIN->GetMonsterGuageDlg()->StartFadeOut() ;

		//KES 추가	//여기서 하는게 이상하다. 몬스터게이지를 살려두기위해서 좀 꼬였다.
		OnDeselected();
		if( OBJECTMGR->CanOverInfoChange( this ) )
			ShowObjectName( FALSE );
		else
			ShowObjectName( TRUE, OBJECTMGR->GetObjectNameColor(this) );
		//
	}
	
//	OBJECTSTATEMGR->StartObjectState(this, eObjectState_Die);
	
	//CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(133), );
	// 죽으면 클릭 안되게 함
	if( m_ObjectKind == eObjectKind_Player )
		m_EngineObject.EnablePick();
	else
		m_EngineObject.DisablePick();
}

void CObject::Revive(VECTOR3* pRevivePos)
{
	SetFlag(FALSE);
//	m_bDieFlag = FALSE;
	MOVEMGR->SetPosition(this,pRevivePos);
}

void CObject::SetState(BYTE State)
{
	SetMotionInState(State);
	
	if(m_ObjectState.State_Start_Motion != -1)
	{
//		if(State != m_BaseObjectInfo.ObjectState)
			ChangeMotion(m_ObjectState.State_Start_Motion,FALSE);
		if(m_ObjectState.State_Ing_Motion != -1)
			ChangeBaseMotion(m_ObjectState.State_Ing_Motion);
	}
	m_ObjectState.BeforeState	= m_BaseObjectInfo.ObjectState;
	m_BaseObjectInfo.ObjectState		= State;
	m_ObjectState.State_Start_Time = gCurTime;
	m_ObjectState.bEndState		= FALSE;
}

BYTE CObject::GetState()
{
	return m_BaseObjectInfo.ObjectState;
}

void CObject::SetPosition(VECTOR3* pPos)
{
	GetEngineObject()->SetEngObjPosition(pPos);
}
void CObject::SetAngle(float AngleRad)
{
	GetEngineObject()->SetEngObjAngle(AngleRad);
}
void CObject::GetPosition(VECTOR3* pPos)
{
	GetEngineObject()->GetEngObjPosition(pPos);
	ASSERTMSG(pPos->x > -1000,"이 메세지 도 윤호한테 연락을...");
	ASSERTMSG(pPos->z > -1000,"이 메세지 도 윤호한테 연락을...");
}
float CObject::GetAngle()
{
	return GetEngineObject()->GetEngObjAngle();
}
DIRINDEX CObject::GetDirectionIndex()
{
	float angleRad = GetAngle();
	DIRINDEX dir = RADTODIR(angleRad);
	while(dir >= MAX_DIRECTION_INDEX)
		dir -= MAX_DIRECTION_INDEX;
	return dir;
}
/*
void CObject::AddStatus(CStatus* pStatus)
{
	m_StatusList.AddTail(pStatus);
}
void CObject::RemoveStatus(CStatus* pStatus)
{
	BOOL rt = m_StatusList.Remove(pStatus);
}
*/
void CObject::ChangeMotion(int motion,BOOL bRoop, float rate)
{
	GetEngineObject()->ChangeMotion(motion,bRoop,rate);
}
void CObject::ChangeBaseMotion(int motion)
{
	GetEngineObject()->ChangeBaseMotion(motion);
}

// 070222 LYW --- Object : Add function to change emotion of hero.
void CObject::ChangeEmotion( int emotion ) 
{
	GetEngineObject()->ChangeMotion(emotion,FALSE);
	((CPlayer*)this)->SetRunningEmotion(TRUE) ;
}


float CObject::GetAttDefense(WORD Attrib)
{
	if(Attrib == ATTR_NOATTR)		// 무속성은 속성방어력이 0이다.
		return 0;
	
	float Ori = DoGetAttDefense(Attrib);
	float Up = 0,Down = 0;
/*	PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
		pSL->GetAttDefense(Attrib,Ori,Up,Down);
	PTRLISTSEARCHEND;
*/	float res = 0;//STATUSCALC(Ori,Up,Down);
	if(res > 1)
		res = 1;
	return res;
}

void CObject::OnSelected()
{
 	GetEngineObject()->EnableSelfIllumin();
	m_bSelected = TRUE;
}

void CObject::OnDeselected()
{
	// desc_hseos_라이트효과01
	// S 라이트효과 추가 added by hseos 2007.06.13
	//	GetEngineObject()->DisableSelfIllumin();
	// E 라이트효과 추가 added by hseos 2007.06.13
	m_bSelected = FALSE;
}

void CObject::SetBattle(DWORD BattleID,BYTE Team)
{
	m_BaseObjectInfo.BattleID = BattleID;
	m_BaseObjectInfo.BattleTeam = Team;
}

void CObject::AddObjectEffect(DWORD KeyValue,OBJECTEFFECTDESC* EffectNumArray,WORD NumOfEffect,CObject* pSkillOperator)
{
	// test
//	if(GetState() == eObjectState_Die)
//	{
//		int a=0;
//	}

	
	HEFFPROC heff;

	// 기존에 있던 이펙트를 찾는다.
	BOOL bFound = FALSE;
	m_StateEffectList.StartGetMultiData(KeyValue);
	while(heff = (HEFFPROC)m_StateEffectList.GetMultiData())
	{
		EFFECTMGR->IncEffectRefCount(heff);
		bFound = TRUE;
	}

	// 있었으면 리턴		//kes***
	if(bFound == TRUE)
		return;

	// 없었으면 새로 만든다.
	for(int n=0;n<NumOfEffect;++n)
	{
		if(EffectNumArray[n].Effect == 0)
			continue;
		heff = EFFECTMGR->StartSkillMainTainEffect(
			EffectNumArray[n].Effect,this,EffectNumArray[n].Flag,&EffectNumArray[n].Position,pSkillOperator);
		if(heff == NULL)
			continue;
		m_StateEffectList.Add((void*)heff,KeyValue);
	}
}

void CObject::RemoveObjectEffect(DWORD KeyValue)
{
	HEFFPROC heff;
	// 기존에 있던 이펙트를 찾는다.
	BOOL bFound = FALSE;
	m_StateEffectList.StartGetMultiData(KeyValue);
	
	cPtrList list;

	while(heff = (HEFFPROC)m_StateEffectList.GetMultiData())
	{
		if(EFFECTMGR->ForcedEndEffect(heff) == TRUE)
		{
//			m_StateEffectList.RemoveCurMultiData();
			list.AddTail((void*)KeyValue);
		}
	}

	PTRLISTPOS pos = list.GetHeadPosition();
	while( pos )
	{
		DWORD k = (HEFFPROC)list.GetNext(pos);
		m_StateEffectList.Remove(k);
	}
	list.RemoveAll();

}
void CObject::RemoveAllObjectEffect()
{
	HEFFPROC heff;
	m_StateEffectList.SetPositionHead();
	while(heff = (HEFFPROC)m_StateEffectList.GetData())
	{
		while(EFFECTMGR->ForcedEndEffect(heff) == FALSE)
		{}
	}
	m_StateEffectList.RemoveAll();	//KES 리스트에서 지워도 되나? forceEnd가 안된경우는..confirm
}
int	CObject::GetMotionIDX(int mainMotion, int subMotion)
{
	return m_pMotionDESC->GetMotionIDX(mainMotion, subMotion);
}

void CObject::Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive)
{
}

BOOL CObject::IsNpcMark( DWORD dwValue )
{
	HEFFPROC heff = (HEFFPROC)m_StateEffectList.GetData( dwValue );
	if( heff )
		return TRUE;
	return FALSE;
}

void CObject::SetKyungGongLevel( WORD wLevel )
{
}

void CObject::SetObjectBattleState( BYTE state )
{
	if( state )
	{
		mBattleStateCheckDelay.Start();
	}
	else
	{
		mBattleStateCheckDelay.End();
	}

	if( m_BaseObjectInfo.ObjectBattleState != state )
	{
		m_BaseObjectInfo.ObjectBattleState = state;

		if( GetObjectKind() == eObjectKind_Player )
		{
			((CPlayer*)this)->SetBaseMotion();
			if(GetState() == eObjectState_None)
				OBJECTSTATEMGR->EndObjectState(this,eObjectState_None);

		}


/*		if( this == HERO )
		{
			if( state )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, "전투를 시작합니다!" );
			}
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, "전투가 종료되었습니다!" );
			}
		}
*/	}
}
