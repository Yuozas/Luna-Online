#include "stdafx.h"
#include ".\pet.h"
#include "Player.h"
#include "petmanager.h"
#include "PackedData.h"
#include "..\[CC]Header\GameResourceStruct.h"
#include "itemmanager.h"
#include "Monster.h"
#include "StateMachinen.h"
#include "CharMove.h"
#include "MHError.h"
#include "ObjectStateManager.h"
#include "MapDBMsgParser.h"

CPet::CPet(void)
{
	mItemSlot = new ITEMBASE[ ePetEquipSlot_Max ];
	m_FollowMonsterList.Initialize(5);
}

CPet::~CPet(void)
{
	delete[] mItemSlot;

	Release();
}

BOOL CPet::Init(EObjectKind kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo)
{
	memset( mItemSlot, 0, sizeof( ITEMBASE ) * ePetEquipSlot_Max );

	memset( &mPassiveStatus, 0, sizeof( Status ) );
	memset( &mBuffStatus, 0, sizeof( Status ) );
	memset( &mRatePassiveStatus, 0, sizeof( Status ) );
	memset( &mRateBuffStatus, 0, sizeof( Status ) );
	memset( &mAbnormalStatus, 0, sizeof( AbnormalStatus ) );
//---KES Aggro 070918
//---어그로 관련 변수 초기화
	m_htAggroedTable.Initialize(10);
//-------------------

	mCheckTime = gCurTime;

	mbBuffSkillLoad = FALSE;


	return CObject::Init( kind, AgentNum, pBaseObjectInfo );
}

void CPet::Release()
{
//---KES Aggro 070918
//---어그로 리스트 릴리스
	RemoveAllAggroed();
//-------------------
	CMonster * pObject = NULL;
	m_FollowMonsterList.SetPositionHead();
	while(pObject = (CMonster *)m_FollowMonsterList.GetData())
	{
		pObject->SetTObject(NULL);
	}
	m_FollowMonsterList.RemoveAll();

	CObject::Release();

	mpPetObjectInfo = NULL;
	mpMaster = NULL;
	memset( mItemSlot, 0, sizeof( ITEMBASE ) * ePetEquipSlot_Max );
}

DWORD CPet::GetGravity()
{
	DWORD totalG = 0;
	CMonster * pObject;
	m_FollowMonsterList.SetPositionHead();
	DWORD gravity = 0;
	while(pObject = m_FollowMonsterList.GetData())
	{
//---KES 어트렉트 비중 버프 적용
		if( pObject->GetAbnormalStatus()->IsAttract )
		{
			gravity = pObject->GetGravity() / 4;	//25%
			if( gravity == 0 ) gravity = 1;
		}
		else
		{
			gravity = pObject->GetGravity();
		}

		totalG += gravity;
//------------------------------
//		totalG += pObject->GetGravity();
	}
//	ASSERT(totalG <= 100);
	return totalG;
}

void CPet::SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin)
{
	MSG_PET_ADD* pMsg = (MSG_PET_ADD*)pAddMsg;
	pMsg->AddableInfo.Init();

	pMsg->Category = MP_USERCONN;
	pMsg->Protocol = MP_USERCONN_PET_ADD;
	pMsg->MasterIdx = mpMaster->GetID();
	memcpy( &( pMsg->PetObjectInfo ), mpPetObjectInfo, sizeof( PET_OBJECT_INFO ) );
 	memcpy( &( pMsg->BaseObjectInfo ), &m_BaseObjectInfo, sizeof( BASEOBJECT_INFO ) );
	memcpy( pMsg->ItemInfo, mItemSlot, sizeof( ITEMBASE ) * ePetEquipSlot_Max );
	GetSendMoveInfo(&pMsg->MoveInfo,&pMsg->AddableInfo);
	pMsg->IsLogin = bLogin;
 
	*pMsgLen = pMsg->GetMsgLength();

	// 080716 LUJ, 로그
	LogPet( *mpPetObjectInfo, ePetLogSummon );
}

void CPet::SetRemoveMsg(char* pRemoveMsg,WORD* pMsgLen,DWORD dwReceiverID)
{
	MSG_DWORD2* pMsg = ( MSG_DWORD2* )pRemoveMsg;

	pMsg->Category = MP_USERCONN;
	pMsg->Protocol = MP_USERCONN_PET_REMOVE;
	pMsg->dwObjectID = dwReceiverID;
	pMsg->dwData1 = GetID();
	pMsg->dwData2 = mpMaster->GetID();

	*pMsgLen = sizeof( MSG_DWORD2 );
}

float CPet::DoGetMoveSpeed()	
{
	return mpMaster->DoGetMoveSpeed();
}

void CPet::AddExp()
{
	if( mpPetObjectInfo->Level > mpMaster->GetLevel() )
		return;

	if( mpPetObjectInfo->Level >= PETMGR->GetMaxLevel() )
		return;

	if( PETMGR->GetGradeUpLevel( mpPetObjectInfo->Grade ) && mpPetObjectInfo->Level >= PETMGR->GetGradeUpLevel( mpPetObjectInfo->Grade ) )
		return;

	WORD exp = mpPetObjectInfo->Exp + 1;

	if( exp >= PETMGR->GetMaxEXP( mpPetObjectInfo->Grade ) )
	{
		LevelUp();
	}
	else
	{
		mpPetObjectInfo->Exp = exp;

		MSG_WORD msg;
		msg.Category = MP_PET;
		msg.Protocol = MP_PET_EXP_NOTIFY;
		msg.dwObjectID = mpMaster->GetID();
		msg.wData = exp;

		mpMaster->SendMsg( &msg, sizeof( msg ) );
	}
}

void CPet::LevelUp()
{
	mpPetObjectInfo->Exp = 0;
	mpPetObjectInfo->Level++;

	MSG_DWORD msg;
	msg.Category = MP_PET;
	msg.Protocol = MP_PET_LEVELUP_NOTIFY;
	msg.dwObjectID = mpMaster->GetID();
	msg.dwData = GetID();

	CalcStats();

	DBInfoUpdate();

	PACKEDDATA_OBJ->QuickSend( mpMaster, &msg, sizeof( MSG_DWORD ) );

	// 080716 LUJ, 펫 로그
	LogPet( *mpPetObjectInfo, ePetLogChangeLevel );
}

void CPet::SetLevel( BYTE level )
{
	mpPetObjectInfo->Exp = 0;
	mpPetObjectInfo->Level = level;

	MSG_DWORDBYTE msg;
	msg.Category = MP_PET;
	msg.Protocol = MP_PET_LEVEL_NOTIFY;
	msg.dwObjectID = mpMaster->GetID();
	msg.dwData = GetID();
	msg.bData = level;

	CalcStats();

	DBInfoUpdate();

	PACKEDDATA_OBJ->QuickSend( mpMaster, &msg, sizeof( MSG_DWORDBYTE ) );
}

void CPet::GradeUp()
{
	if( mpPetObjectInfo->Grade == PETMGR->GetMaxGrade() )
		return;

	BYTE slot = PETMGR->GetExtendedSkillSlot( mpPetObjectInfo->Grade );

	mpPetObjectInfo->SkillSlot += slot;
	mpPetObjectInfo->Grade++;

	MSG_DWORDBYTE2 msg;
	msg.Category = MP_PET;
	msg.Protocol = MP_PET_GRADEUP_NOTIFY;
	msg.dwObjectID = mpMaster->GetID();
	msg.dwData = GetID();
	msg.bData1 = mpPetObjectInfo->Type;
	msg.bData2 = mpPetObjectInfo->SkillSlot;

	CalcStats();
	
	DBInfoUpdate();

	PACKEDDATA_OBJ->QuickSend( mpMaster, &msg, sizeof( MSG_DWORDBYTE2 ) );

	// 080716 LUJ, 로그
	LogPet( *mpPetObjectInfo, ePetLogChangeGrade );
}

void CPet::SetType( BYTE type )
{
	mpPetObjectInfo->Type = type;
}

void CPet::FriendlyUp( BYTE val ) 
{ 
	mpPetObjectInfo->Friendly += val; 
	
	if( mpPetObjectInfo->Friendly > 100 ) 
		mpPetObjectInfo->Friendly = 100; 

	MSG_BYTE msg;
	msg.Category = MP_PET;
	msg.Protocol = MP_PET_FRIENDLY_SYNC;
	msg.bData = mpPetObjectInfo->Friendly;
		
	mpMaster->SendMsg( &msg, sizeof( msg ) );

}

ITEMBASE* CPet::GetWearedItem( POSTYPE pos )
{
	if( pos >= ePetEquipSlot_Max )
		return NULL;

	if( mItemSlot[ pos ].dwDBIdx == 0 )
		return NULL;

	return &mItemSlot[ pos ];
}

void CPet::SetWearedItem( POSTYPE pos, ITEMBASE* pitem )
{
	if( pitem )
	{
		mItemSlot[ pos ] = *pitem;
	}
	else
	{
		memset( &mItemSlot[ pos ], 0, sizeof( ITEMBASE ) );
	}

	CalcStats();

	MSG_PET_ITEM_INFO msg;
	msg.Category = MP_PET;
	msg.Protocol = MP_PET_ITEM_INFO;
	msg.ID = GetID();
	memcpy( msg.Item, mItemSlot, sizeof( ITEMBASE ) * ePetEquipSlot_Max );
	PACKEDDATA_OBJ->QuickSendExceptObjectSelf( mpMaster, &msg, sizeof( MSG_PET_ITEM_INFO ) );
}

void CPet::SetAllWearedItem( ITEMBASE* info )
{
	memcpy( mItemSlot, info, sizeof( ITEMBASE ) * ePetEquipSlot_Max );

	CalcStats();
}

void CPet::CalcStats()
{
	memset( &mPetStat, 0, sizeof( PlayerStat ) );
	memset( &mItemStat, 0, sizeof( PlayerStat ) );	

	PET_STATUS_INFO* pInfo = PETMGR->GetPetStatusInfo( mpPetObjectInfo->Type, mpPetObjectInfo->Level );

	for( int i = 0; i < ePetEquipSlot_Max; i++ )
	{
		if( !mItemSlot[ i ].dwDBIdx )
			continue;

		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( mItemSlot[ i ].wIconIdx );

		mItemStat.mPhysicAttack.mPlus += pInfo->PhysicAttack;
		mItemStat.mPhysicDefense.mPlus += pInfo->PhysicDefense;
		mItemStat.mMagicAttack.mPlus += pInfo->MagicAttack;
		mItemStat.mMagicDefense.mPlus += pInfo->MagicDefense;
		mItemStat.mStrength.mPlus += pInfo->ImprovementStr;
		mItemStat.mDexterity.mPlus += pInfo->ImprovementDex;
		mItemStat.mVitality.mPlus += pInfo->ImprovementVit;
		mItemStat.mIntelligence.mPlus += pInfo->ImprovementInt;
		mItemStat.mWisdom.mPlus += pInfo->ImprovementWis;
		mItemStat.mLife.mPlus += pInfo->ImprovementLife;
		mItemStat.mMana.mPlus += pInfo->ImprovementMana;
	}
	
	mPetStat.mStrength.mPlus		= ( pInfo->STR + mItemStat.mStrength.mPlus + GetBuffStatus()->Str) * ( 1.0f + mItemStat.mStrength.mPercent );
	mPetStat.mDexterity.mPlus 		= ( pInfo->DEX + mItemStat.mDexterity.mPlus + GetBuffStatus()->Dex) * ( 1.0f + mItemStat.mDexterity.mPercent );
	mPetStat.mVitality.mPlus		= ( pInfo->VIT + mItemStat.mVitality.mPlus + GetBuffStatus()->Vit) * ( 1.0f + mItemStat.mVitality.mPercent );
	mPetStat.mIntelligence.mPlus	= ( pInfo->INT + mItemStat.mIntelligence.mPlus + GetBuffStatus()->Int) * ( 1.0f + mItemStat.mIntelligence.mPercent );
	mPetStat.mWisdom.mPlus			= ( pInfo->WIS + mItemStat.mWisdom.mPlus + GetBuffStatus()->Wis) * ( 1.0f + mItemStat.mWisdom.mPercent );


	BYTE idx = mpPetObjectInfo->Type * 10 + mpPetObjectInfo->Grade;

	PET_HP_MP_INFO*	pHPMPInfo = PETMGR->GetHPMPInfo( idx );

	SetMaxLife( mPetStat.mVitality.mPlus * ( 10 + ( 0.05 * mpPetObjectInfo->Level * pHPMPInfo->HP ) ) );
	SetMaxMana( mPetStat.mWisdom.mPlus * ( 6 + ( 0.05 * mpPetObjectInfo->Level * pHPMPInfo->MP ) ) );

	if( mpPetObjectInfo->HP > mpPetObjectInfo->MAXHP  ) SetLife( mpPetObjectInfo->MAXHP );
	if( mpPetObjectInfo->MP > mpPetObjectInfo->MAXMP  ) SetMana( mpPetObjectInfo->MAXMP );

	{// 공격력
		DWORD wWeaponAttack		= DWORD( mItemStat.mPhysicAttack.mPlus * ( 1.0f + mItemStat.mPhysicAttack.mPercent ) );

		DWORD wEnchantAttack	= 0;
		DWORD wLevel			= GetLevel() ;
		float wPassiveSkill		= GetPassiveStatus()->PhysicAttack ;

		float fAttackA			= (float)( wWeaponAttack + wEnchantAttack + wLevel + wPassiveSkill + 3) ;
		float wStrengthAll		= mPetStat.mStrength.mPlus;

		wStrengthAll *= 0.2f;

		float wMasteryLevel		= 0 ;

		float wAttackBuffVal	= GetBuffStatus()->PhysicAttack ;

		float fAttackB			= 1000 + (((wStrengthAll -10) * 20) + (wMasteryLevel * 25) + (wAttackBuffVal * 10)) ;

		mPetStat.mPhysicAttack.mPlus = fAttackA * fAttackB / 400 ;
	}

	{// 방어력
		DWORD wDefenseAll		= DWORD( mItemStat.mPhysicDefense.mPlus * ( 1.0f + mItemStat.mPhysicDefense.mPercent ) );

		DWORD wEnchantAll		= 0 ;
		DWORD wLevel			= GetLevel() ;
		float wPassiveSkill		= GetPassiveStatus()->PhysicDefense; ;

		float fDefense			= (float)wDefenseAll / 2 ;
		float fDefenseA			= fDefense + wEnchantAll + wLevel + wPassiveSkill ;

		float wVitalityAll		= mPetStat.mVitality.mPlus;

		float wMasteryLevel		= 0 ;
		wVitalityAll *= 0.2f;

		float wDefenseBuffVal	= GetBuffStatus()->PhysicDefense ;

		float fValA				= (((float)wVitalityAll - 20) * 10) ;
		float fValB				= (float)(wMasteryLevel * 30) ;
		float fValC				= (float)(wDefenseBuffVal * 10) ;

		float fDefenseB			= 500 + (fValA + fValB + fValC) ;

		float fResult			= (fDefenseA * fDefenseB) / 200 ;

		if( fResult <= 0 )
		{
			mPetStat.mPhysicDefense.mPlus = 0 ;
		}
		else
		{
			mPetStat.mPhysicDefense.mPlus	= fResult ;
		}
	}

	{// 마법 공격력
		DWORD wMagicAttack		= DWORD( mItemStat.mMagicAttack.mPlus * ( 1.0f + mItemStat.mMagicAttack.mPercent ) );

		DWORD wEnchantAttack	= 0 ;
		DWORD wLevel			= GetLevel() ;
		float wPassiveSkill		= GetPassiveStatus()->MagicAttack;

		float fAttackA			= (float)(wMagicAttack + wEnchantAttack + wLevel + wPassiveSkill) ;

		float wIntelligenceAll	= mPetStat.mIntelligence.mPlus;

		wIntelligenceAll *= 0.2f;

		float wAttackBuffVal	= GetBuffStatus()->MagicAttack;

		float fValA				= ((float)wIntelligenceAll -10) * 35 ;
		float fValB				= (float)(wAttackBuffVal * 10) ;

		float fAttackB			= 1000 + (fValA + fValB) ;

		float fResult			= (fAttackA * fAttackB) / 250 ;

		mPetStat.mMagicAttack.mPlus	= fResult ;
	}

	{// 마법 방어력
		DWORD wMagDefenseAll		= DWORD( mItemStat.mMagicDefense.mPlus * ( 1.0f + mItemStat.mMagicDefense.mPercent ) );

		DWORD wEnchantAll		= 0 ;
		DWORD wLevel			= GetLevel() ;
		float wPassiveSkill		= GetPassiveStatus()->MagicDefense;

		float fDefense			= (float)wMagDefenseAll / 5 ;
		float fDefenseA			= fDefense + wEnchantAll + wLevel + wPassiveSkill ;

		float wWisdomAll		= mPetStat.mWisdom.mPlus;

		wWisdomAll *= 0.2f;

		float wDefenseBuffVal	= GetBuffStatus()->MagicDefense;

		float fValA				= ((float)wWisdomAll - 20) * 15 ;
		float fValB				= (float)wDefenseBuffVal * 10 ;

		float fDefenseB			= 500 + (fValA + fValB) ;

		float fResult			= (fDefenseA * fDefenseB) / 150 ;

		mPetStat.mMagicDefense.mPlus = fResult;
	}

	{// 명중
		DWORD wLevel			= GetLevel() ;

		float wDexterityAll	= mPetStat.mDexterity.mPlus;
		float wStrengthAll	= mPetStat.mStrength.mPlus;

		wDexterityAll *= 0.2f;
		wStrengthAll *= 0.2f;

		DWORD wClassOrderVal	= 0 ;
		float wPassiveSkill		= GetPassiveStatus()->Accuracy;
		float wBuffSkillVal		= GetBuffStatus()->Accuracy;

		float fValA				= ((float)wDexterityAll / 1.4f) ;
		float fValC				= ((float)wStrengthAll / 4) ;
		float fValB				= (float)(fValA + wLevel + wClassOrderVal + fValC);

		float wAccuracy			= (fValB) ;

		float dwTotalAccuracy	= wAccuracy + wPassiveSkill + wBuffSkillVal;

		{
			const PlayerStat::Value& value = mItemStat.mAccuracy;

			mPetStat.mAccuracy.mPlus = ( dwTotalAccuracy + value.mPlus ) * ( 1.0f + value.mPercent );
		}
	}

	{// 회피
		DWORD wLevel			= GetLevel() ;

		float wDexterityAll	= mPetStat.mDexterity.mPlus;

		wDexterityAll *= 0.2f;

		DWORD wClassOrderVal	= 0 ;
		float wPassiveSkill		= GetPassiveStatus()->Avoid ;
		float wBuffSkillVal		= GetBuffStatus()->Avoid ;

		float fValA				= ((float)wDexterityAll / 2) ;
		float fValB				= (float)(fValA + wLevel + wClassOrderVal);

		float wEvasion			= (fValB) ;

		float dwTotalEvasion	= wEvasion + wPassiveSkill + wBuffSkillVal;

		// 080313 LUJ, 아이템 스탯을 일반과 세트로 분리
		{
			const PlayerStat::Value& value = mItemStat.mEvade;

			mPetStat.mEvade.mPlus = ( dwTotalEvasion + value.mPlus ) * ( 1.0f + value.mPercent );
		}
	}
}


//---KES Aggro 070918
//void CMonster::CalcAggro() 함수에서만 호출한다. 다른곳에서 호출 금지!
void CPet::AddToAggroed( CMonster* pMonster )
{
	m_htAggroedTable.Add( pMonster, pMonster->GetID() );
}

//void CMonster::RemoveFromAggro( CPlayer* pPlayer ) 함수에서만 호출한다. 다른곳에서 호출 금지!
void CPet::RemoveFromAggroed( DWORD dwObjectID )
{
	m_htAggroedTable.Remove( dwObjectID );
}

void CPet::RemoveAllAggroed()
{
	m_htAggroedTable.SetPositionHead();
	CMonster* pMonster = NULL;
	while( pMonster = m_htAggroedTable.GetData() )
	{
		pMonster->RemoveFromAggro( GetID() );		//몬스터가 가지고 있는 어그로 리스트에서 삭제
	}

	m_htAggroedTable.RemoveAll();
}

//내가 가진 어그로몬스터리스트를 이용해, pTarget의 어그로를 올린다. (pTarget은 자신이 될 수 있다.)
void CPet::AddAggroToMyMonsters( int nAggroAdd, CObject* pTarget )
{
	m_htAggroedTable.SetPositionHead();
	CMonster* pMonster = NULL;
	while( pMonster = m_htAggroedTable.GetData() )
	{
		pMonster->CalcAggro( pTarget, nAggroAdd );
	}
}

//------------------


BOOL CPet::AddFollowList(CMonster * pMob)
{	
	if( m_FollowMonsterList.GetDataNum() < 50 )		//max 50마리
	{
		m_FollowMonsterList.Add(pMob, pMob->GetID());
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CPet::RemoveFollowAsFarAs(DWORD GAmount, CObject* pMe )
{	
	VECTOR3 * ObjectPos	= CCharMove::GetPosition(this);
	BOOL bMe = FALSE;

	while(GAmount > 100)
	{	
		CMonster * pObject = NULL;
		CMonster * MaxObject = NULL;
		float	MaxDistance	= -1;
		float	Distance	= 0;

		m_FollowMonsterList.SetPositionHead();
		while(pObject = m_FollowMonsterList.GetData())
		{
			VECTOR3 * TObjectPos = CCharMove::GetPosition(pObject);
			if((Distance = CalcDistanceXZ( ObjectPos, TObjectPos )) > MaxDistance)
			{
				MaxDistance = Distance;
				MaxObject = pObject;
			}
		}
		if(MaxObject)
		{
			if(GAmount > MaxObject->GetGravity())
				GAmount -= MaxObject->GetGravity();
			else
				GAmount = 0;

			MaxObject->SetTObject(NULL);

			GSTATEMACHINE.SetState(MaxObject, eMA_WALKAROUND);

			if( pMe == MaxObject )
				bMe = TRUE;
		}
		else
		{
			MHERROR->OutputFile("Debug.txt", MHERROR->GetStringArg("amount != 0"));
			GAmount = 0;
		}
	}

	return bMe;	
}

void CPet::RemoveFollowList(DWORD ID)
{
	m_FollowMonsterList.Remove(ID);
}

void CPet::DoDie(CObject* pAttacker)
{
	if( pAttacker )
		OBJECTSTATEMGR_OBJ->StartObjectState(this,eObjectState_Die,pAttacker->GetID());
	else
		OBJECTSTATEMGR_OBJ->StartObjectState(this,eObjectState_Die,0);
//---KES Aggro 070918
//---어그로 리스트 릴리스
	RemoveAllAggroed();	//*주의: 아래 FollowMonsterList해제하기 이전에 해주어야, 다음 어그로를 타겟으로 잡을 수 있다.
//-------------------

	CMonster * pObject = NULL;
	m_FollowMonsterList.SetPositionHead();
	while(pObject = (CMonster *)m_FollowMonsterList.GetData())
	{
		pObject->SetTObject(NULL);
	}
	m_FollowMonsterList.RemoveAll();


	//---KES 죽으면 이동을 멈춰주어야 한다.
	if( CCharMove::IsMoving(this) )
	{
		VECTOR3 pos;
		GetPosition( &pos );
		CCharMove::EndMove( this, gCurTime, &pos );
	}

	mpPetObjectInfo->HP = 0;
	mpPetObjectInfo->MP = 0;
	mpPetObjectInfo->Friendly = 0;
	mpPetObjectInfo->State = ePetState_Die;

	MSG_DWORD msg;
	msg.Category = MP_PET;
	msg.Protocol = MP_PET_DIE_NOTIFY;
	msg.dwData = mpPetObjectInfo->ItemDBIdx;

	mpMaster->SendMsg( &msg, sizeof( msg ) );

	char log[256];
	sprintf( log, "./log/pet%02d.txt", g_pServerSystem->GetMapNum() );
	FILE* fp = fopen( log, "a+" );

	if( fp )
	{
		fprintf( fp, "PETDIE:\t%d\n", this );
		fclose( fp );
	}
	// 080716 LUJ, 로그. SealPet()에서 오브젝트를 소멸시키므로 이전에 호출되어야 한다
	LogPet( *mpPetObjectInfo, ePetLogDie );

	PETMGR->SealPet( this );
}

DWORD CPet::Damage(CObject* pAttacker,RESULTINFO* pDamageInfo)
{
	DWORD life = GetLife();
	DWORD beforelife = life;

	if(life > pDamageInfo->RealDamage)
	{
		life -= pDamageInfo->RealDamage;
	}
	else
	{
		life = 0;
	}
	
	SetLife(life,FALSE);

	DoDamage(pAttacker,pDamageInfo,beforelife);

	return life;
}

void CPet::SetLife(DWORD val,BOOL bSendMsg) 
{
	if(GetState() == eObjectState_Die)
		return;

	DWORD maxlife = mpPetObjectInfo->MAXHP;
	if(val > maxlife)
		val = maxlife;
	
	if(mpPetObjectInfo->HP != val)	
	{
		if(bSendMsg == TRUE)
		{
			MSG_INT msg;
			msg.Category = MP_PET;
			msg.Protocol = MP_PET_LIFE_NOTIFY;
			msg.dwObjectID = GetID();
			msg.nData = val - GetLife();
			mpMaster->SendMsg(&msg,sizeof(msg));
		}
	}
		
	mpPetObjectInfo->HP = val;
}

void CPet::SetMana(DWORD val,BOOL bSendMsg)
{ 
	if(GetState() == eObjectState_Die)
		return;

	DWORD MaxMana = mpPetObjectInfo->MAXMP;
	if(val > MaxMana)
		val = MaxMana;

	if( mpPetObjectInfo->MP != val)
	{
		if(bSendMsg)
		{
			MSG_DWORD msg;
			msg.Category = MP_PET;
			msg.Protocol = MP_PET_MANA_NOTIFY;
			msg.dwObjectID = GetID();
			msg.dwData = val;
			mpMaster->SendMsg(&msg,sizeof(msg));
		}
	}
	
	mpPetObjectInfo->MP = val; 
}

void CPet::FriendlyProcess()
{
	mpPetObjectInfo->FriendlyCheckTime += gCurTime - mCheckTime;
	mCheckTime = gCurTime;

	if( mpPetObjectInfo->FriendlyCheckTime > PETMGR->GetFriendlyReduceTime() )
	{
		mpPetObjectInfo->FriendlyCheckTime = 0;
		mpPetObjectInfo->Friendly -= PETMGR->GetFriendlyReducePoint();

		MSG_BYTE msg;
		msg.Category = MP_PET;
		msg.Protocol = MP_PET_FRIENDLY_SYNC;
		msg.bData = mpPetObjectInfo->Friendly;
		mpMaster->SendMsg( &msg, sizeof( msg ) );

		if( mpPetObjectInfo->Friendly == 0 )
		{
			DoDie( NULL );
		}
	}
}

void CPet::SetMaxLife(DWORD val)
{
	mpPetObjectInfo->MAXHP = val;

	MSG_DWORD msg;
	msg.Category = MP_PET;
	msg.Protocol = MP_PET_MAXLIFE_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	mpMaster->SendMsg(&msg,sizeof(msg));
}

void CPet::SetMaxMana(DWORD val)
{
	mpPetObjectInfo->MAXMP = val;

	MSG_DWORD msg;
	msg.Category = MP_PET;
	msg.Protocol = MP_PET_MAXMANA_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	mpMaster->SendMsg(&msg,sizeof(msg));
}

void CPet::DBInfoUpdate()
{
	PetInfoUpdate( mpMaster->GetID(), 
				   mpPetObjectInfo->ItemDBIdx, 
				   mpPetObjectInfo->Level, 
				   mpPetObjectInfo->Grade, 
				   mpPetObjectInfo->Type, 
				   mpPetObjectInfo->SkillSlot, 
				   mpPetObjectInfo->Exp, 
				   mpPetObjectInfo->Friendly, 
				   mpPetObjectInfo->MAXHP, 
				   mpPetObjectInfo->HP, 
				   mpPetObjectInfo->MAXMP, 
				   mpPetObjectInfo->MP, 
				   mpPetObjectInfo->AI, 
				   mpPetObjectInfo->State, 
				   mpPetObjectInfo->FriendlyCheckTime );
}

void CPet::MasterInfoUpdate()
{
	PetMasterUpdate( mpMaster->GetID(), 
					 mpPetObjectInfo->MasterIdx, 
					 mpPetObjectInfo->ItemDBIdx );
}

void CPet::StateProcess()
{
    if( !mbBuffSkillLoad )
	{
		CharacterBuffLoad( GetID() );
		mbBuffSkillLoad = TRUE;
	}
}
