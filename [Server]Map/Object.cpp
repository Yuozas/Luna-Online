// Object.cpp: implementation of the CObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Object.h"
#include "CharMove.h"
//#include "ServerSystem.h"
#include "ObjectStateManager.h"
#include "BattleSystem_Server.h"
#include "PackedData.h"
#include "Network.h"
#include "..\[CC]Header\CommonCalcFunc.h"
//for event - hs //
#include "Monster.h"
//for event - hs //
#include "Player.h"

#include "UserTable.h"

#include "../[cc]skill/server/status/status.h"
#include "../[cc]skill/server/object/skillobject.h"
#include "../[cc]skill/server/object/buffskillobject.h"
#include "../[cc]skill/server/info/buffskillinfo.h"
#include "../[cc]skill/server/Delay/Delay.h"
// 080616 LUJ, 버프 중 이벤트 처리하기위해 참조
#include "../[cc]skill/Server/Manager/SkillManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CObject::CObject()
{
	m_bInited = FALSE;
	m_GridID = 0;
//	m_MapNum = 0;
//	m_tempMapNum = 0;
//	m_OnOffSkillList.Initialize(10);
	m_BuffSkillList.Initialize(10);
}

CObject::~CObject()
{
}

BOOL CObject::Init(EObjectKind kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo)
{
	SetBattle(pBaseObjectInfo->BattleID,0);

	m_AgentNum = AgentNum;
	m_ObjectKind = kind;

	if(pBaseObjectInfo)
		memcpy(&m_BaseObjectInfo,pBaseObjectInfo,sizeof(m_BaseObjectInfo));
	else
		memset(&m_BaseObjectInfo,0,sizeof(BASEOBJECT_INFO));

	memset( &m_MoveInfo, 0, sizeof(MOVE_INFO) );
	
	OBJECTSTATEMGR_OBJ->InitObjectState(this);

	mEffectMoveCount = 0;
	CurCastingSkillID = 0;

	return TRUE;
}

void CObject::Release()
{
	//m_ObjectKind = 0;
	m_bInited = FALSE;
	m_bGridInited = FALSE;
	mEffectMoveCount = 0;

	EndAllSkill();
}

void CObject::EndSkill()
{
	cSkillObject * pSObj = NULL;
	cPtrList templist;

	m_BuffSkillList.SetPositionHead();
	while( pSObj = m_BuffSkillList.GetData() )
	{
		if( ( ( cBuffSkillInfo* )( pSObj->GetSkillInfo() ) )->GetSkillInfo()->Die == 0 )
		{
			pSObj->SetEndState();
			pSObj->EndState();
			templist.AddTail( pSObj );
		}
	}

	//m_BuffSkillList.RemoveAll();

	PTRLISTPOS pos = templist.GetHeadPosition();
	while( pos )
	{
		pSObj = ( cSkillObject* )templist.GetNext( pos );
		m_BuffSkillList.Remove( pSObj->GetSkillIdx() );
	}
	templist.RemoveAll();

	pSObj = SKILLMGR->GetSkillObject( mCurrentSkillID );

	if( pSObj )
	{
		pSObj->SetEndState();
		pSObj->EndState();
	}

/*	m_OnOffSkillList.SetPositionHead();
	while( pSObj = m_OnOffSkillList.GetData() )
	{
		pSObj->EndState();
	}
	m_OnOffSkillList.RemoveAll();
*/
}


void CObject::EndAllSkill()
{
	cSkillObject * pSObj = NULL;
	cPtrList templist;

	m_BuffSkillList.SetPositionHead();
	while( pSObj = m_BuffSkillList.GetData() )
	{
		pSObj->EndState();
	}
	m_BuffSkillList.RemoveAll();
/*
	m_OnOffSkillList.SetPositionHead();
	while( pSObj = m_OnOffSkillList.GetData() )
	{
		pSObj->EndState();
	}
	m_OnOffSkillList.RemoveAll();
*/
}

void CObject::GetBaseObjectInfo(BASEOBJECT_INFO* pRtInfo)
{
	memcpy(pRtInfo,&m_BaseObjectInfo,sizeof(BASEOBJECT_INFO));
}

void CObject::GetSendMoveInfo(SEND_MOVEINFO* pRtInfo,CAddableInfoList* pAddInfoList)
{
	pRtInfo->CurPos.Compress(CCharMove::GetPosition(this));
	pRtInfo->KyungGongIdx = m_MoveInfo.KyungGongIdx;
	pRtInfo->MoveMode = m_MoveInfo.MoveMode;
	pRtInfo->AddedMoveSpeed = m_MoveInfo.AddedMoveSpeed;
	/*어빌리티 삭제 - pRtInfo->AbilityKyungGongLevel = m_MoveInfo.AbilityKyungGongLevel;*/

	if(CCharMove::IsMoving(this))
	{
		if(pAddInfoList == NULL)
		{
			char temp[256];
			sprintf(temp,"Object %s,  Invalid Move State (Not Inited But Moving)");
			ASSERTMSG(0,temp);
			for(int i=0;i<m_MoveInfo.GetMaxTargetPosIdx();++i)
			{
				VECTOR3* pPos = m_MoveInfo.GetTargetPosition(i);
				sprintf(temp,"tpos %d,   (%4.2f,  %4.2f)",i,pPos->x,pPos->z);
				ASSERTMSG(0,temp);
			}
			return;
		}

		COMPRESSEDTARGETPOS tpos;
		tpos.Init();
		for(int i=0;i<m_MoveInfo.GetMaxTargetPosIdx();++i)
			tpos.AddTargetPos(m_MoveInfo.GetTargetPosition(i));
		pAddInfoList->AddInfo(CAddableInfoList::MoveInfo,tpos.GetSize(),&tpos);
	}
}
/*
VECTOR3 * CObject::GetCurTargetPosition()
{
	return &m_MoveInfo.TargetPositions[m_MoveInfo.CurTargetPositionIdx];
}*/
void CObject::SetRemoveMsg(char* pRemoveMsg,WORD* pMsgLen,DWORD dwReceiverID)
{
	MSG_DWORD* pMsg = (MSG_DWORD*)pRemoveMsg;
	pMsg->Category = MP_USERCONN;
	pMsg->Protocol = MP_USERCONN_OBJECT_REMOVE;
	pMsg->dwObjectID = dwReceiverID;
	pMsg->dwData = GetID();
	
	*pMsgLen = sizeof(MSG_DWORD);
}

void CObject::SetInited()
{
	DoSetInited();
	if(GetObjectKind() != eObjectKind_Player)
	{
		/*
		if(GridPos.x == -1 ||
			GridPos.z == -1 ||
			GridPos.LastX == -1 ||
			GridPos.LastZ == -1)
		{
			ASSERT(0);
		}
		*/
	}

	m_bInited = TRUE;
}

void CObject::SetNotInited()
{
	m_bInited = FALSE;
}

void CObject::GetPosition(VECTOR3* pos)
{
	*pos = *CCharMove::GetPosition(this);
}

void CObject::SetState(BYTE state)
{
	m_ObjectState.BeforeState	= m_BaseObjectInfo.ObjectState;
	m_BaseObjectInfo.ObjectState		= state;
	m_ObjectState.State_Start_Time = gCurTime;
	m_ObjectState.bEndState		= FALSE;
}

int CObject::GetObjectTileSize()
{
	return 0;
}

void CObject::CalcCounterDamage(WORD Attrib,WORD AttrDamage,WORD PhyDamage,RESULTINFO* pDamageInfo)
{
	DWORD CounterDamage = 0;
/*	PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
		pSL->GetCounterAttackDamage(Attrib,AttrDamage,PhyDamage,pDamageInfo,CounterDamage);
	PTRLISTSEARCHEND;
*/	pDamageInfo->CounterDamage = (WORD)CounterDamage;
}

void CObject::CalcVampiric(RESULTINFO* pDamageInfo)
{
	float VampiricLifeRate = 0;
	float VampiricManaRate = 0;
/*	PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
		pSL->GetVampiricLife(VampiricLifeRate);
		pSL->GetVampiricMana(VampiricManaRate);
	PTRLISTSEARCHEND;
*/	pDamageInfo->Vampiric_Life = (TARGETDATA)(pDamageInfo->RealDamage * VampiricLifeRate);
	pDamageInfo->Vampiric_Mana = (TARGETDATA)(pDamageInfo->RealDamage * VampiricManaRate);

	if( (DWORD)(pDamageInfo->Vampiric_Life) )
	{
		DWORD CurLife = GetLife();
		DWORD MaxLife = GetMaxLife();
//		ASSERT(CurLife <= MaxLife);
		if(CurLife + pDamageInfo->Vampiric_Life > MaxLife)
		{
			pDamageInfo->Vampiric_Life = MaxLife - CurLife;
		}
		SetLife(CurLife+pDamageInfo->Vampiric_Life,FALSE);
	}
	
	if( (DWORD)(pDamageInfo->Vampiric_Mana) )
	{
		DWORD CurMana = GetMana();
		DWORD MaxMana = GetMaxMana();
//		ASSERT(CurMana <= MaxMana);
		if(CurMana + pDamageInfo->Vampiric_Mana > MaxMana)
		{
			pDamageInfo->Vampiric_Mana = MaxMana - CurMana;
		}
		SetMana(CurMana+pDamageInfo->Vampiric_Mana,FALSE);
	}

}

void CObject::CalcReverseVampiric(RESULTINFO* pDamageInfo)
{
	float VampiricLifeRate = 0.0f;
	float VampiricManaRate = 0.0f;
/*	PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
		pSL->GetReverseVampiricLife(VampiricLifeRate);
		pSL->GetReverseVampiricMana(VampiricManaRate);
	PTRLISTSEARCHEND;
*/	DWORD VampiricLife		= (DWORD)(pDamageInfo->RealDamage * VampiricLifeRate);
	DWORD VampiricMana	= (DWORD)(pDamageInfo->RealDamage * VampiricManaRate);

	if( VampiricLife )
	{
		DWORD CurLife = GetLife();
		DWORD MaxLife = GetMaxLife();
//		ASSERT(CurLife <= MaxLife);
		if(CurLife + VampiricLife > MaxLife)
		{
			VampiricLife = MaxLife - CurLife;
		}
		SetLife(CurLife+VampiricLife,TRUE);
	}
	
	if( VampiricMana )
	{
		DWORD CurMana = GetMana();
		DWORD MaxMana = GetMaxMana();
//		ASSERT(CurMana <= MaxMana);
		if(CurMana + VampiricMana > MaxMana)
		{
			VampiricMana = MaxMana - CurMana;
		}
		SetMana(CurMana+VampiricMana,TRUE);
	}
}

void CObject::CalcRealAttack(CObject* pTarget, WORD PhyDamage, WORD AttrDamage, RESULTINFO* pDamageInfo, BOOL bIsContinueAttack  )
{
	DWORD Ori = PhyDamage + AttrDamage;
	DWORD Up = 0,Down = 0;
/*	PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
		pSL->GetAttackDamage(Ori,Up,Down);
	PTRLISTSEARCHEND;
*///	pDamageInfo->RealDamage = (WORD)STATUSCALC(Ori,Up,Down);	

//	Up = 0;	Down = 0;
	DWORD PhyUp = 0, AttrUp = 0;
	DWORD PhyDown = 0, AttrDown = 0;

	// 2005 크리스마스 이벤트
	if( GetObjectKind() == eObjectKind_Player )
		if( ((CPlayer*)this)->GetWeaponEquipType() == WP_EVENT )
		{
			if(pTarget->GetObjectKind() == eObjectKind_Monster)
			{
				CMonster* pMonster = (CMonster*)pTarget;
				
				if( pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA1 || pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA2)
				{
					pDamageInfo->RealDamage = EVENT_DAMAGE;
					return;
				}
				else
				{
					pDamageInfo->RealDamage = 0;
					return;
				}
			}
			else
			{
				pDamageInfo->RealDamage = 0;
				return;
			}
		}	

	if( bIsContinueAttack == FALSE )	//지속데미지 공격이 아닌 경우에만 더해준다.
	{
		if( PhyDamage )
		{
/*			PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
				pSL->GetAttackPhyDamagePlus(PhyDamage,PhyUp,PhyDown);
			PTRLISTSEARCHEND;
*/		}
		else
		{
/*			PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
				pSL->GetAttackAttDamagePlus(AttrDamage,AttrUp,AttrDown);
			PTRLISTSEARCHEND;
*/		}
	}

	DWORD TotalUp = Up + PhyUp + AttrUp;
	DWORD TotalDown = Down + PhyDown + AttrDown;

	pDamageInfo->RealDamage = Ori;//(WORD)STATUSCALC(Ori,TotalUp,TotalDown);
}


void CObject::CalcRealDamage(CObject* pOperator,WORD PhyDamage,WORD AttrDamage,RESULTINFO* pDamageInfo)
{//CPlayer 에 재정의
	
	// 2005 크리스마스 이벤트
	if( pOperator->GetObjectKind() == eObjectKind_Player )
		if( ((CPlayer*)pOperator)->GetWeaponEquipType() == WP_EVENT )
		{
			if(GetObjectKind() == eObjectKind_Monster)
			{
				CMonster* pMonster = (CMonster*)this;
				
				if( pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA1 || pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA2)
				{
					pDamageInfo->RealDamage = EVENT_DAMAGE;
					return;
				}
				else
				{
					pDamageInfo->RealDamage = 0;
					return;
				}
			}
			else
			{
				pDamageInfo->RealDamage = 0;
				return;
			}
		}

	// 운기조식중일 경우에는 데미지를 50% 추가한다.
	if(GetState() == eObjectState_Ungijosik)
	{
		PhyDamage = (WORD)(PhyDamage*1.5f);
		AttrDamage = (WORD)(AttrDamage*1.5f);		
	}
	
	DWORD Ori = PhyDamage + AttrDamage;
	DWORD Up = 0,Down = 0;
/*	PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
		pSL->GetDamage(Ori,Up,Down);
	PTRLISTSEARCHEND;
*/
	pDamageInfo->RealDamage = Ori;//(WORD)STATUSCALC(Ori,Up,Down);
}

// 06. 03 국내무쌍 - 이영준
void CObject::Die(CObject* pAttacker)
{
	// 현재 속해 있는 Battle에서 처리해보고 처리하지 못했다면(return FALSE),
	// 각 클래스들에게 처리를 맡긴다.
	EndSkill();

	DWORD AttackerID;
	if(pAttacker == NULL)
		AttackerID = 0;
	else 
		AttackerID = pAttacker->GetID();
	if(m_pBattle->OnTeamMemberDie(GetBattleTeam(),GetID(),AttackerID) == FALSE)
		DoDie(pAttacker);
}

//주의: CPlayer::Damage를 만들었다. 수정사항이 있다면 CPlayer::Damage도 수정하도록 하시오. KES
DWORD CObject::Damage(CObject* pAttacker,RESULTINFO* pDamageInfo)
{
	DWORD life = GetLife();
	DWORD beforelife = life;
	
	//for event - hs //
/*
	if(GetObjectKind() == eObjectKind_Monster)
	{
		CMonster* pMonster = (CMonster*)this;
		if( pMonster->GetMonsterKind() == 143 || pMonster->GetMonsterKind() == 139 
			|| pMonster->GetMonsterKind() == 141 )
		{
			pDamageInfo->RealDamage = 66;
		}
		else if( pMonster->GetMonsterKind() == 144 || pMonster->GetMonsterKind() == 140
			|| pMonster->GetMonsterKind() == 142 )
		{
			pDamageInfo->RealDamage = 88;
		}
		else if( pMonster->GetMonsterKind() == 170 || pMonster->GetMonsterKind() == 171 )
		{
			pDamageInfo->RealDamage = 1000;
		}
		else if( pMonster->GetMonsterKind() == 172 || pMonster->GetMonsterKind() == 173		// 초코바
			|| pMonster->GetMonsterKind() == 174 || pMonster->GetMonsterKind() == 175 )		// 발렌타인
		{
			pDamageInfo->RealDamage = 10;
		}
		else if( pMonster->GetMonsterKind() == 176 || pMonster->GetMonsterKind() == 177 || pMonster->GetMonsterKind() == 178 )	// 차바차바
		{
			pDamageInfo->RealDamage = 7;
		}
	}

	
	//	2005 크리스마스 이벤트 코드
	if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
		CPlayer* pPlayer = (CPlayer*) pAttacker;
		
		if( pPlayer->GetWeaponEquipType() == WP_EVENT )
		{
			if(GetObjectKind() == eObjectKind_Monster)
			{
				CMonster* pMonster = (CMonster*)this;
				
				if( pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA1 || pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA2)
					pDamageInfo->RealDamage = EVENT_DAMAGE;
				else
					pDamageInfo->RealDamage = 0;
			}
			else
				pDamageInfo->RealDamage = 0;
		}
	}
*/		//for event//



	if(life > pDamageInfo->RealDamage)
		life -= pDamageInfo->RealDamage;
	else
		life = 0;
	
	SetLife(life,FALSE);

	DoDamage(pAttacker,pDamageInfo,beforelife);

	return life;
}

void CObject::AddLife(DWORD val,DWORD* realAddLife,BOOL bSendMsg)
{	
	DWORD life = GetLife();
	DWORD maxlife = GetMaxLife();
	
	if(life+val > maxlife)
	{
		val = maxlife - life;
	}
	SetLife(life+val,bSendMsg);

	if(realAddLife)
		*realAddLife = val;
}

// 080710 LUJ, 스킬 사용 시 HP를 소모할 수 있도록 추가
void CObject::ReduceLife( DWORD value )
{
	SetLife( GetLife() < value ? 0 : GetLife() - value );
}

// RaMa 04.11.04
void CObject::AddMana(DWORD val,DWORD* realAddMana)
{
	DWORD mana = GetMana();
	DWORD maxmana = GetMaxMana();
	
	if(mana+val > maxmana)
	{
		val = maxmana - mana;
	}
	SetMana(mana+val);

	if(realAddMana)
		*realAddMana = val;
}

void CObject::ReduceMana(DWORD val)
{
	if(GetMana() < val)
		SetMana(0);
	else
		SetMana(GetMana()-val);
}

void CObject::CheckLifeValid()
{
	DWORD MaxLife = GetMaxLife();
	if(GetLife() > MaxLife)
		SetLife(MaxLife);
}
void CObject::CheckManaValid()
{
	DWORD MaxMana = GetMaxMana();
	if(GetMana() > MaxMana)
		SetMana(MaxMana);
}

//////////////////////////////////////////////////////////////////////////
// Status

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

//////////////////////////////////////////////////////////////////////////
// battle
void CObject::SetBattle( DWORD BattleID, BYTE Team, DWORD Param )
{
	m_BaseObjectInfo.BattleID = BattleID;
	m_BaseObjectInfo.BattleTeam = Team;
	m_pBattle = BATTLESYSTEM->GetBattle(BattleID);
	
	if(m_pBattle == NULL)
	{
		ASSERT(0);
		return;
	}

	if(m_bInited == FALSE)
		return;

	MSG_DWORD4 msg;
	msg.Category	= MP_BATTLE;
	msg.Protocol	= MP_BATTLE_CHANGE_OBJECTBATTLE;
	msg.dwObjectID	= GetID();
	msg.dwData1		= BattleID;
	msg.dwData2		= Team;
	msg.dwData3		= m_pBattle->GetBattleKind();
	msg.dwData4		= Param;

	PACKEDDATA_OBJ->QuickSend( this, &msg, sizeof(msg) );
}


void CObject::SendMsg(MSGBASE* pMsg,int MsgLen)
{
	pMsg->dwObjectID = GetID();
	g_Network.Send2Server(GetAgentNum(),(char*)pMsg,MsgLen);
}

DWORD CObject::GetRecoverLife()
{
	DWORD UpLife = 0;
/*	PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
	pSL->GetRecoverLife(UpLife);
	PTRLISTSEARCHEND;

*/	return UpLife;
}

DWORD CObject::GetRecoverMana()
{
	DWORD UpNaryuk = 0;
/*	PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
	pSL->GetRecoverMana(UpNaryuk);
	PTRLISTSEARCHEND;

*/	return UpNaryuk;
}
/*
void CObject::AddStatus( eStatusKind kind, StatusData* pData, Status_Kind flag )
{
	//mStatusArray[ kind ].AddData( pData, flag );
}

void CObject::RemoveStatus( eStatusKind kind, StatusData* pData, Status_Kind flag )
{
	//mStatusArray[ kind ].RemoveData( pData, flag );
}

cStatus* CObject::GetStatus( eStatusKind kind )
{
	//return &( mStatusArray[ kind ] );
	return NULL;
}
*/

void CObject::ProcessSpecialSkill()
{
}
	
void CObject::ProcessAbnormalStatus()
{
}

void CObject::EndBuffSkillByStatus( WORD Status )
{
	cSkillObject * pSObj = NULL;

	m_BuffSkillList.SetPositionHead();
	while( pSObj = m_BuffSkillList.GetData() )
	{
		if( ( ( cBuffSkillInfo* )( pSObj->GetSkillInfo() ) )->GetSkillInfo()->Status == Status )
		{
			pSObj->SetEndState();
			pSObj->EndState();
		}
	}
}

// 080616 LUJ, 이벤트 처리
void CObject::Execute( const CEvent& event )
{
	SkillObjectIndexList::iterator removedIterator = mSkillObjectIndexList.end();

	for(
		SkillObjectIndexList::iterator it = mSkillObjectIndexList.begin();
		mSkillObjectIndexList.end() != it;
		++it )
	{
		cSkillObject* skill = SKILLMGR->GetSkillObject( *it );

		if( ! skill )
		{
			removedIterator = it;
			continue;
		}

		skill->Execute( event );
	}

	// 080616 LUJ, 이벤트 발생 시 마다 하나씩 없는 스킬 오브젝트 인덱스를 삭제하여 부하를 줄인다.
	mSkillObjectIndexList.erase( removedIterator );
}

// 080616 LUJ, 이벤트 시 처리할 버프 스킬을 등록한다
void CObject::AddEventSkill( cSkillObject& object )
{
	if( SKILLKIND_BUFF != object.GetSkillKind() )
	{
		return;
	}

	const BUFF_SKILL_INFO* buffInfo = 0;
	{
		cBuffSkillInfo* info = ( cBuffSkillInfo* )( object.GetSkillInfo() );

		if( info )
		{
			buffInfo = info->GetSkillInfo();
		}
	}

	if( ! buffInfo )
	{
		return;
	}

	switch( buffInfo->Status )
	{
	case eStatusKind_Reflect:
	case eStatusKind_Absorb:
	case eStatusKind_DamageToLife:
	case eStatusKind_DamageToMana:
	case eStatusKind_ConvertHealthWithGivedDamage:
	case eStatusKind_ConvertManaWithGivedDamage:
	case eStatusKind_EventSkillToKiller:
	case eStatusKind_EventSkillToVictim:
		{
			mSkillObjectIndexList.push_back( object.GetID() );
			break;
		}
	}
}
