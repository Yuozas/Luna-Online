// GameResourceManager.cpp: implementation of the CGameResourceManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResourceManager.h"


GLOBALTON(CGameResourceManager);

CGameResourceManager::CGameResourceManager()
{
	m_ItemInfoList.Initialize(MAX_ITEM_NUM);
	m_SkillInfoList.Initialize(5000);
	m_KyunggongInfoList.Initialize(50);
	m_AbilityInfoList.Initialize(64);
	m_NpcList.Initialize(50);

	m_MonsterList.Initialize(300);

	m_QuestStringTable.Initialize( 100 );

	m_MapNameTable.Initialize( 100 );

	m_MoneyLogTypeTable.Initialize( 100 );
	m_MugongLogTypeTable.Initialize( 100 );
	m_ItemMoneyLogTypeTable.Initialize( 100 );
	m_ExpPointLogTypeTable.Initialize( 100 );
	m_RMToolLogTypeTable.Initialize( 100 );
	m_PetLogTypeTable.Initialize( 100 );
	/// 060831 PKH 문파토너먼트
	m_GuildTNLogTypeTable.Initialize( 100 );
	/// 060831 PKH 공성전
	m_SiegeWarLogTypeTable.Initialize( 100 );
	/// 060915 PKH GMTOOLLOG
	m_GMToolLogTypeTable.Initialize( 1000 );

	m_PetNameTable.Initialize(100);
}

CGameResourceManager::~CGameResourceManager()
{
	char* pData = NULL;
	m_MapNameTable.SetPositionHead();
	while( pData = m_MapNameTable.GetData() )
		delete pData;
	m_MapNameTable.RemoveAll();

	CQuestString* pQuestString = NULL;
	m_QuestStringTable.SetPositionHead();
	while( pQuestString = m_QuestStringTable.GetData() )
		delete pQuestString;
	m_QuestStringTable.RemoveAll();

	ITEM_INFO * pItemInfo = NULL;
	m_ItemInfoList.SetPositionHead();
	while(pItemInfo = m_ItemInfoList.GetData())
		delete pItemInfo;
	m_ItemInfoList.RemoveAll();

	SKILLINFO * pSkillInfo = NULL;
	m_SkillInfoList.SetPositionHead();
	while(pSkillInfo = m_SkillInfoList.GetData())
		delete pSkillInfo;
	m_SkillInfoList.RemoveAll();

	KYUNGGONG_INFO * pKInfo = NULL;
	m_KyunggongInfoList.SetPositionHead();
	while(pKInfo = m_KyunggongInfoList.GetData())
		delete pKInfo;
	m_KyunggongInfoList.RemoveAll();

	CAbilityInfo* pAInfo;
	m_AbilityInfoList.SetPositionHead();
	while(pAInfo = m_AbilityInfoList.GetData())
		delete pAInfo;
	m_AbilityInfoList.RemoveAll();

	NPC_LIST* pNInfo;
	m_NpcList.SetPositionHead();
	while(pNInfo = m_NpcList.GetData())
		delete pNInfo;
	m_NpcList.RemoveAll();

	MONSTEREX_LIST* pMInfo;
	m_MonsterList.SetPositionHead();
	while(pMInfo = m_MonsterList.GetData())
	{
		delete pMInfo->AttackIndex;
		delete pMInfo->AttackRate;
		delete pMInfo->SkillInfo;

		delete pMInfo;
	}
	m_MonsterList.RemoveAll();

	ReleaseLogTypeTable();

	for( int i = 0; i < MAX_ETC_MSGNUM; ++i )
	{
		if( m_pEtcMsg[i] )
		{
			delete[] m_pEtcMsg[i];
			m_pEtcMsg[i] = NULL;
		}
	}

}

BOOL CGameResourceManager::Init()
{
	if( !LoadExpPoint() )
		return FALSE;

	if( !LoadItemList() )
		return FALSE;

	if( !LoadSkillInfoList() )
		return FALSE;

	if( !LoadKyunggongInfoList() )
		return FALSE;

	if( !LoadAbilityInfoList() )
		return FALSE;

	if( !LoadNpcList() )
		return FALSE;

	if( !LoadMonsterList() )
		return FALSE;

	//!//
	if( !LoadPetList() )
		return FALSE;

	LoadQuestString();

	LoadMapNameList();

	ReadLogType();

	LoadEtcMsgList();

	return TRUE;
}

BOOL CGameResourceManager::LoadExpPoint()
{
	char filename[64];
	CMHFile file;
#ifdef _FILE_BIN_
	sprintf(filename,"Resource/characterExpPoint.bin");	
	if(!file.Init(filename, "rb"))
		return FALSE;
#else
	sprintf(filename,"Resource/characterExpm_ItemInfoListPoint.txt");	
	if(!file.Init(filename, "rt"))
		return FALSE;
#endif

	int idx =0;
	while(1)
	{
		if(idx > MAX_CHARACTER_LEVEL_NUM-1) break;
		if(file.IsEOF())
			break;
		file.GetWord();												// 레벨
		m_CharacterExpPoint[idx] = file.GetExpPoint();				// 경험치
		idx++;
	}

	file.Release();

	return TRUE;	
}


BOOL CGameResourceManager::LoadItemList()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if(!file.Init("Resource/ItemList.bin", "rb"))
		return FALSE;
#else
	if(!file.Init("Resource/ItemList.txt", "rt"))
		return FALSE;
#endif

	ITEM_INFO * pInfo = NULL;
	while(1)
	{
		if(file.IsEOF())
			break;

		ASSERT(!pInfo);
		pInfo = new ITEM_INFO;

		pInfo->ItemIdx = file.GetWord();
		strcpy(pInfo->ItemName, file.GetString());
		pInfo->ItemTooltipIdx = file.GetWord();
		pInfo->Image2DNum = file.GetWord();	
		pInfo->ItemKind = file.GetWord();			
		pInfo->BuyPrice = file.GetDword();			
		pInfo->SellPrice = file.GetDword();			
		pInfo->Rarity = file.GetWord();			
		pInfo->WeaponType = file.GetWord();		
		pInfo->GenGol = file.GetWord();			
		pInfo->MinChub = file.GetWord();			
		pInfo->CheRyuk = file.GetWord();			
		pInfo->SimMek = file.GetWord();			
		pInfo->Life = file.GetDword();			
		pInfo->Shield = file.GetDword();				
		pInfo->NaeRyuk = file.GetWord();			
		pInfo->AttrRegist.SetElement_Val(ATTR_FIRE,file.GetFloat());
		pInfo->AttrRegist.SetElement_Val(ATTR_WATER,file.GetFloat());
		pInfo->AttrRegist.SetElement_Val(ATTR_TREE,file.GetFloat());
		pInfo->AttrRegist.SetElement_Val(ATTR_IRON,file.GetFloat());
		pInfo->AttrRegist.SetElement_Val(ATTR_EARTH,file.GetFloat());
		pInfo->LimitJob = file.GetWord();			
		pInfo->LimitGender = file.GetWord();		
		pInfo->LimitLevel = file.GetLevel();		
		pInfo->LimitGenGol = file.GetWord();		
		pInfo->LimitMinChub = file.GetWord();		
		pInfo->LimitCheRyuk = file.GetWord();		
		pInfo->LimitSimMek = file.GetWord();		
		pInfo->ItemGrade = file.GetWord();			
		pInfo->RangeType = file.GetWord();			
		pInfo->EquipKind = file.GetWord();			
		pInfo->Part3DType = file.GetWord();		
		pInfo->Part3DModelNum = file.GetWord();	
		pInfo->MeleeAttackMin = file.GetWord();	
		pInfo->MeleeAttackMax = file.GetWord();	
		pInfo->RangeAttackMin = file.GetWord();	
		pInfo->RangeAttackMax = file.GetWord();	
		pInfo->CriticalPercent = file.GetWord();	
		pInfo->AttrAttack.SetElement_Val(ATTR_FIRE,file.GetFloat());
		pInfo->AttrAttack.SetElement_Val(ATTR_WATER,file.GetFloat());
		pInfo->AttrAttack.SetElement_Val(ATTR_TREE,file.GetFloat());
		pInfo->AttrAttack.SetElement_Val(ATTR_IRON,file.GetFloat());
		pInfo->AttrAttack.SetElement_Val(ATTR_EARTH,file.GetFloat());
		pInfo->PhyDef = file.GetWord();			
		pInfo->Plus_MugongIdx = file.GetWord();	
		pInfo->Plus_Value = file.GetWord();		
		pInfo->AllPlus_Kind = file.GetWord();		
		pInfo->AllPlus_Value = file.GetWord();		
		pInfo->MugongNum = file.GetWord();			
		pInfo->MugongType = file.GetWord();		
		pInfo->LifeRecover = file.GetWord();		
		pInfo->LifeRecoverRate = file.GetFloat();
		pInfo->NaeRyukRecover = file.GetWord();	
		pInfo->NaeRyukRecoverRate = file.GetFloat();	
		pInfo->ItemType = file.GetWord();
		
#ifdef _JAPAN_LOCAL_
		pInfo->wItemAttr = file.GetWord();
		pInfo->wAcquireSkillIdx1 = file.GetWord();
		pInfo->wAcquireSkillIdx2 = file.GetWord();
		pInfo->wDeleteSkillIdx = file.GetWord();
#endif
				
		ASSERT(m_ItemInfoList.GetData(pInfo->ItemIdx) == FALSE);
		m_ItemInfoList.Add(pInfo, pInfo->ItemIdx);
		pInfo = NULL;
	}
	file.Release();

	return TRUE;
}

BOOL CGameResourceManager::LoadPetList()
{
	CMHFile file;
	int colnum=36; //필요없는 컬럼수

#ifdef _FILE_BIN_
	if(!file.Init("Resource/PetList.bin", "rb"))
		return FALSE;
#else
	if(!file.Init("Resource/PetList.txt", "rt"))
		return FALSE;
#endif

	PET_NAME * pInfo = NULL;
	while(1)
	{
		if(file.IsEOF())
			break;

		ASSERT(!pInfo);
		pInfo = new PET_NAME;

		pInfo->PetIdx = file.GetWord();
		strcpy(pInfo->ItemName, file.GetString());
		for(int i = 0 ; i < colnum; ++i){
			file.GetWord();
		}
		m_PetNameTable.Add(pInfo, pInfo->PetIdx);
		pInfo = NULL;
	}
	file.Release();

	return TRUE;
}

// 06.08.04 - pkh [pet] 펫 리소스의 코드로 이름가져옴
char* CGameResourceManager::GetPetNamechar( DWORD dwNameidx )
{
	PET_NAME* pData = m_PetNameTable.GetData( dwNameidx );
	return pData->ItemName;
}

/*
BOOL CGameResourceManager::LoadSkillInfoList()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if(!file.Init("Resource/SkillList.bin","rb"))
		return FALSE;
#else
	if(!file.Init("Resource/SkillList.txt","rt"))
		return FALSE;
#endif

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;

		SKILLINFO* pInfo = new SKILLINFO;
		
		pInfo->SkillIdx = file.GetWord();
		file.GetString(pInfo->SkillName);
		pInfo->SkillTooltipIdx = file.GetWord();
		pInfo->RestrictLevel = file.GetWord();
		pInfo->LowImage = file.GetInt();
		pInfo->HighImage = file.GetInt();
		pInfo->SkillKind = file.GetWord();
		pInfo->WeaponKind = file.GetWord();
		pInfo->SkillRange = file.GetWord();
		pInfo->TargetKind = file.GetWord();
		pInfo->TargetRange = file.GetWord();
		pInfo->TargetAreaIdx = file.GetWord();
		pInfo->TargetAreaPivot = file.GetWord();
		pInfo->TargetAreaFix = file.GetWord();
		pInfo->MoveTargetArea = file.GetWord();
		pInfo->MoveTargetAreaDirection = file.GetWord();
		pInfo->MoveTargetAreaVelocity = file.GetFloat();
		pInfo->Duration = file.GetDword();
		pInfo->Interval = file.GetWord();
		pInfo->DelaySingleEffect = file.GetWord();	
		pInfo->ComboNum = file.GetWord();
		pInfo->Life = file.GetWord();
		pInfo->BindOperator = file.GetWord();
		pInfo->EffectStartTime = file.GetWord();
		file.GetString();
		file.GetString();
		file.GetString();
		file.GetString();
		file.GetString();
		pInfo->DelayTime = file.GetWord();
		pInfo->FatalDamage = file.GetWord();
		for(int n=0;n<12;++n)
			pInfo->NeedExp[n] = file.GetExpPoint();
		file.GetWord(pInfo->NeedNaeRyuk,12);
		pInfo->Attrib = file.GetWord();
		pInfo->NegativeResultTargetType = file.GetWord();
		pInfo->TieUpType = file.GetWord();
		pInfo->ChangeSpeedType = file.GetWord();
		pInfo->ChangeSpeedRate = file.GetWord();
		pInfo->Dispel = file.GetWord();
		pInfo->PositiveResultTargetType = file.GetWord();
		pInfo->Immune = file.GetWord();
		pInfo->AIObject = file.GetWord();
		pInfo->MineCheckRange = file.GetWord();
		pInfo->MineCheckStartTime = file.GetWord();
		
		pInfo->CounterDodgeKind = file.GetWord();
		file.GetString();
		pInfo->DamageDecreaseForDist = file.GetWord();
		
		WORD AdditiveAttr;
		WORD temp[12];
		for(n=0;n<6;++n)
		{
			AdditiveAttr = file.GetWord();
			ySWITCH(AdditiveAttr)
			yCASE(0)	file.GetWord(temp,12);
			yCASE(11)	file.GetFloat(pInfo->FirstPhyAttack,12);
			yCASE(12)	file.GetFloat(pInfo->UpPhyAttack,12);
			yCASE(13)	file.GetWord(pInfo->FirstAttAttackMin,12);
			yCASE(14)	file.GetWord(pInfo->FirstAttAttackMax,12);
			yCASE(15)	file.GetWord(pInfo->ContinueAttAttack,12);
			
			yCASE(20)	file.GetFloat(pInfo->CounterPhyAttack,12);
			yCASE(21)	file.GetFloat(pInfo->CounterAttAttack,12);
			yCASE(22)	file.GetFloat(pInfo->UpPhyDefence,12);
			yCASE(23)	file.GetFloat(pInfo->UpAttDefence,12);
			yCASE(24)	file.GetWord(pInfo->UpMaxNaeRyuk,12);
			yCASE(25)	file.GetWord(pInfo->UpMaxLife,12);
			yCASE(26)	file.GetWord(pInfo->ContinueRecoverNaeRyuk,12);
			yCASE(27)	file.GetWord(pInfo->FirstRecoverNaeRyuk,12);
			yCASE(28)	file.GetWord(pInfo->ContinueRecoverLife,12);
			yCASE(29)	file.GetWord(pInfo->FirstRecoverLife,12);
			yCASE(30)	file.GetFloat(pInfo->CounterDodgeRate,12);
			
			yCASE(31)	file.GetFloat(pInfo->CriticalRate,12);
			yCASE(32)	file.GetFloat(pInfo->StunRate,12);
			yCASE(33)	file.GetWord(pInfo->StunTime,12);
			
			
			yCASE(34)	file.GetWord(pInfo->UpMaxShield,12);			
			yCASE(35)	file.GetWord(pInfo->AmplifiedPowerPhy,12);			
			yCASE(36)	file.GetFloat(pInfo->VampiricLife,12);
			yCASE(37)	file.GetFloat(pInfo->VampiricNaeryuk,12);			
			yCASE(38)	file.GetFloat(pInfo->RecoverStateAbnormal,12);
			
			yCASE(39)	file.GetFloat(pInfo->DispelAttackFeelRate,12);
			
			
			yCASE(41)	file.GetWord(pInfo->DownMaxLife,12);			
			yCASE(42)	file.GetWord(pInfo->DownMaxNaeRyuk,12);			
			yCASE(43)	file.GetWord(pInfo->DownMaxShield,12);
			yCASE(44)	file.GetFloat(pInfo->DownPhyDefence,12);			
			yCASE(45)	file.GetFloat(pInfo->DownAttDefence,12);	
			yCASE(46)	file.GetFloat(pInfo->DownPhyAttack,12);
			
			yCASE(47)	file.GetDword(pInfo->SkillAdditionalTime,12);
			yCASE(48)	file.GetWord(pInfo->AmplifiedPowerAtt,12);
			yCASE(49)	file.GetFloat(pInfo->AmplifiedPowerAttRate,12);
			
			yCASE(50)	file.GetFloat(pInfo->FirstAttAttack,12);
			yCASE(51)	file.GetFloat(pInfo->ContinueAttAttackRate,12);
			yENDSWITCH
		}	
		
//		ASSERT(m_SkillInfoList.GetData(pInfo->SkillIdx) == NULL);
		m_SkillInfoList.Add(pInfo, pInfo->SkillIdx);
	}
	
	return TRUE;
}
*/
BOOL CGameResourceManager::LoadSkillInfoList()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if(!file.Init("Resource/SkillList.bin","rb"))
		return FALSE;
#else
	if(!file.Init("Resource/SkillList.txt","rt"))
		return FALSE;
#endif
	
	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;
		
		SKILLINFO* pInfo = new SKILLINFO;
		pInfo->SkillIdx = file.GetWord();
		file.GetString(pInfo->SkillName);
//		SafeStrCpy( pInfo->SkillName, file.GetString(), MAX_NAME_LENGTH+1 );
		pInfo->SkillTooltipIdx = file.GetWord();
		pInfo->RestrictLevel = file.GetWord();
		pInfo->LowImage = file.GetInt();
		pInfo->HighImage = file.GetInt();
		pInfo->SkillKind = file.GetWord();
		pInfo->WeaponKind = file.GetWord();
		pInfo->SkillRange = file.GetWord();
		pInfo->TargetKind = file.GetWord();
		pInfo->TargetRange = file.GetWord();
		pInfo->TargetAreaIdx = file.GetWord();
		pInfo->TargetAreaPivot = file.GetWord();
		pInfo->TargetAreaFix = file.GetWord();
		pInfo->MoveTargetArea = file.GetWord();
		pInfo->MoveTargetAreaDirection = file.GetWord();
		pInfo->MoveTargetAreaVelocity = file.GetFloat();
		pInfo->Duration = file.GetDword();
		pInfo->Interval = file.GetWord();
		pInfo->DelaySingleEffect = file.GetWord();	
		pInfo->ComboNum = file.GetWord();
		pInfo->Life = file.GetWord();
		pInfo->BindOperator = file.GetWord();
		pInfo->EffectStartTime = file.GetWord();
		file.GetString();
		file.GetString();
		file.GetString();
		file.GetString();
		file.GetString();
		pInfo->DelayTime = file.GetDword();
		pInfo->FatalDamage = file.GetWord();
		for(int n=0;n<12;++n)
			pInfo->NeedExp[n] = file.GetExpPoint();
		file.GetWord(pInfo->NeedNaeRyuk,12);
		pInfo->Attrib = file.GetWord();
		pInfo->NegativeResultTargetType = file.GetWord();
		pInfo->TieUpType = file.GetWord();
		pInfo->ChangeSpeedType = file.GetWord();
		pInfo->ChangeSpeedRate = file.GetWord();
		pInfo->Dispel = file.GetWord();
		pInfo->PositiveResultTargetType = file.GetWord();
		pInfo->Immune = file.GetWord();
		pInfo->AIObject = file.GetWord();
		pInfo->MineCheckRange = file.GetWord();
		pInfo->MineCheckStartTime = file.GetWord();		
		pInfo->CounterDodgeKind = file.GetWord();
		file.GetString();
		//041213 KES decrease skill power
		pInfo->DamageDecreaseForDist = file.GetWord();
		
		WORD AdditiveAttr;
		WORD temp[12];		
		for(n=0;n<6;++n)
		{
			AdditiveAttr = file.GetWord();
			ySWITCH(AdditiveAttr)
			yCASE(0)	file.GetWord(temp,12);
			yCASE(11)	file.GetFloat(pInfo->FirstPhyAttack,12);
			yCASE(12)	file.GetFloat(pInfo->UpPhyAttack,12);
			yCASE(13)	file.GetWord(pInfo->FirstAttAttackMin,12);
			yCASE(14)	file.GetWord(pInfo->FirstAttAttackMax,12);
			yCASE(15)	file.GetWord(pInfo->ContinueAttAttack,12);
			
			yCASE(20)	file.GetFloat(pInfo->CounterPhyAttack,12);
			yCASE(21)	file.GetFloat(pInfo->CounterAttAttack,12);
			yCASE(22)	file.GetFloat(pInfo->UpPhyDefence,12);
			yCASE(23)	file.GetFloat(pInfo->UpAttDefence,12);
			yCASE(24)	file.GetWord(pInfo->UpMaxNaeRyuk,12);
			yCASE(25)	file.GetWord(pInfo->UpMaxLife,12);
			yCASE(26)	file.GetWord(pInfo->ContinueRecoverNaeRyuk,12);
			yCASE(27)	file.GetWord(pInfo->FirstRecoverNaeRyuk,12);
			yCASE(28)	file.GetWord(pInfo->ContinueRecoverLife,12);
			yCASE(29)	file.GetWord(pInfo->FirstRecoverLife,12);
			yCASE(30)	file.GetFloat(pInfo->CounterDodgeRate,12);
			
			yCASE(31)	file.GetFloat(pInfo->CriticalRate,12);
			yCASE(32)	file.GetFloat(pInfo->StunRate,12);
			yCASE(33)	file.GetWord(pInfo->StunTime,12);			
			
			yCASE(34)	file.GetWord(pInfo->UpMaxShield,12);			
			yCASE(35)	file.GetWord(pInfo->AmplifiedPowerPhy,12);			
			yCASE(36)	file.GetFloat(pInfo->VampiricLife,12);
			yCASE(37)	file.GetFloat(pInfo->VampiricNaeryuk,12);			
			yCASE(38)	file.GetFloat(pInfo->RecoverStateAbnormal,12);
			
			yCASE(39)	file.GetFloat(pInfo->DispelAttackFeelRate,12);
			yCASE(40)	file.GetFloat(pInfo->ChangeSpeedProbability,12);
			
			yCASE(41)	file.GetWord(pInfo->DownMaxLife,12);			
			yCASE(42)	file.GetWord(pInfo->DownMaxNaeRyuk,12);			
			yCASE(43)	file.GetWord(pInfo->DownMaxShield,12);
			yCASE(44)	file.GetFloat(pInfo->DownPhyDefence,12);			
			yCASE(45)	file.GetFloat(pInfo->DownAttDefence,12);	
			yCASE(46)	file.GetFloat(pInfo->DownPhyAttack,12);
			
			yCASE(47)	file.GetDword(pInfo->SkillAdditionalTime,12);
			yCASE(48)	file.GetWord(pInfo->AmplifiedPowerAtt,12);
			yCASE(49)	file.GetFloat(pInfo->AmplifiedPowerAttRate,12);
			
			yCASE(50)	file.GetFloat(pInfo->FirstAttAttack,12);
			yCASE(51)	file.GetFloat(pInfo->ContinueAttAttackRate,12);
			
			yCASE(52)	file.GetFloat(pInfo->DamageRate,12);
			yCASE(53)	file.GetFloat(pInfo->AttackRate,12);
			yCASE(54)	file.GetWord(pInfo->ContinueRecoverShield,12);
			yCASE(55)	file.GetFloat(pInfo->AttackLifeRate,12);
			yCASE(56)	file.GetFloat(pInfo->AttackShieldRate,12);
			yCASE(57)	file.GetFloat(pInfo->AttackSuccessRate,12);
			yCASE(58)	file.GetFloat(pInfo->VampiricReverseLife,12);
			yCASE(59)	file.GetFloat(pInfo->VampiricReverseNaeryuk,12);
			yCASE(60)	file.GetDword(pInfo->AttackPhyLastUp,12);
			yCASE(61)	file.GetDword(pInfo->AttackAttLastUp,12);
//			yCASE(62)	file.GetFloat(pInfo->UpCriticalRate,12);

			////////////////////////////////////////////////////////
			//06.07.20 박경희
			//스킬 추가되서 배열 안맞아 이름 안나오는것 묵향 클라이언트에
			//영준씨가 작업한것 가져다 붙여 순서 맞춰줌
			////////////////////////////////////////////////////////
			yCASE(100)	
				pInfo->SkipEffect = file.GetWord();
				for(int i = 0; i < 11; i++)
					file.GetWord();
			yCASE(101)	
				pInfo->SpecialState = file.GetWord();
				for(int i = 0; i < 11; i++)
					file.GetWord();


			yENDSWITCH
		}	

		////////////////////////////////////////////////////////
		//06.07.20 박경희
		//스킬 추가되서 배열 안맞아 이름 안나오는것 묵향 클라이언트에
		//영준씨가 작업한것 가져다 붙여 순서 맞춰줌
		////////////////////////////////////////////////////////
		pInfo->CanSkipEffect = file.GetBool();
		pInfo->ChangeKind = file.GetWord();

		// 자기중심범위무공인데 사정거리보다 범위가 작은경우
		if( pInfo->TargetAreaPivot == 1 && 
			pInfo->TargetRange != 0 &&
			pInfo->SkillRange > pInfo->TargetRange )
			pInfo->TargetRange = pInfo->SkillRange;

		//////////////////////////////////////////////////////////////////////////

//		ASSERT(m_SkillInfoList.GetData(pInfo->SkillIdx) == NULL);
		m_SkillInfoList.Add(pInfo, pInfo->SkillIdx);
	}

	return TRUE;
}


BOOL CGameResourceManager::LoadKyunggongInfoList()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if(file.Init("Resource/KyungGongInfo.bin","rb") == FALSE)
		return FALSE;
#else
	if(file.Init("Resource/KyungGongInfo.txt","rt") == FALSE)
		return FALSE;
#endif

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;

		KYUNGGONG_INFO* pInfo = new KYUNGGONG_INFO;

		pInfo->KyungGongIdx = file.GetWord();		//무공 index
		file.GetString(pInfo->KyungGongName);		//경공이름
		pInfo->NeedNaeRyuk = file.GetWord();		//소모 내력
		pInfo->MoveType = file.GetWord();			//이동 타입
		pInfo->Speed = file.GetFloat();			//이동 속도
		pInfo->ChangeTime = file.GetWord();		//경공 모드 전환 시간
		pInfo->StartEffect = file.GetWord();		//경공 사용 연출
		pInfo->IngEffect = file.GetWord();			//경공 연출 파일
		pInfo->EndEffect = file.GetWord();			//경공 끝 연출

		ASSERT(m_KyunggongInfoList.GetData(pInfo->KyungGongIdx) == FALSE);
		m_KyunggongInfoList.Add(pInfo, pInfo->KyungGongIdx);
	}	

	return TRUE;
}

BOOL CGameResourceManager::LoadAbilityInfoList()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if(!file.Init("./Resource/AbilityBaseInfo.bin", "rb"))
		return FALSE;
#else
	if(!file.Init("./Resource/AbilityBaseInfo.txt", "rt"))
		return FALSE;
#endif

	while(1)
	{
		if(file.IsEOF())
			break;

		CAbilityInfo* pInfo = new CAbilityInfo(&file);
		m_AbilityInfoList.Add(pInfo,pInfo->GetIdx());
	}

	file.Release();

	return TRUE;
}

BOOL CGameResourceManager::LoadNpcList()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if( !file.Init("./Resource/StaticNpc.bin", "rb") )
		return FALSE;
#else
	if( !file.Init("./Resource/StaticNpc.txt", "rt") )
		return FALSE;
#endif

	while(1)
	{
		if(file.IsEOF())
			break;

		NPC_LIST* pInfo = new NPC_LIST;
		file.GetDword();
		pInfo->NpcKind = file.GetWord();
		file.GetString(pInfo->Name);
		pInfo->ModelNum = file.GetWord();
		pInfo->JobKind = file.GetWord();
		file.GetDword();
		file.GetDword();

		if( pInfo->ModelNum < 1000 )
			m_NpcList.Add(pInfo,pInfo->ModelNum);
		else
			delete pInfo;
	}

	file.Release();
	
	return TRUE;	
}

BOOL CGameResourceManager::LoadMonsterList()
{
	char filename[64];
	char value[64] = {0,};
	CMHFile file;

#ifdef _FILE_BIN_
	sprintf(filename,"./Resource/MonsterList.bin");	
	if(!file.Init(filename, "rb"))
		return FALSE;
#else
	sprintf(filename,"./Resource/MonsterList.txt");	
	if(!file.Init(filename, "rt"))
		return FALSE;
#endif

	int n =0;
	MONSTEREX_LIST * pList = NULL;
	while(1)
	{
		if(n == MAX_MONSTER_NUM)
			break;

		if(file.IsEOF())
			break;
		
		pList = new MONSTEREX_LIST;
		memset(pList, 0, sizeof(MONSTEREX_LIST));
		pList->MonsterKind				= file.GetWord();		// 번호
		file.GetString(pList->Name);						// 몬스터이름	
		file.GetString(pList->EngName);						// 영어이름
		pList->Level					= file.GetLevel();		// 몬스터레벨	
		file.GetString();	
		pList->MotionID					= file.GetDword();	
		file.GetString(pList->ChxName);
		pList->Scale					= file.GetFloat();// 몬스터 크기	
		pList->Gravity					= file.GetDword();
		pList->Weight					= file.GetFloat();// 몬스터 무게
		pList->MonsterRadius			= file.GetWord();// 몬스터 	
		pList->Life						= file.GetDword();
		pList->Shield					= file.GetDword();
		pList->ExpPoint					= file.GetExpPoint();// 경험치
		pList->Tall						= file.GetWord();

		pList->AttackPhysicalMin		= file.GetWord();// 공격 최소물리 공격력	
		pList->AttackPhysicalMax		= file.GetWord();// 공격 최대물리 공격력	
		pList->CriticalPercent			= file.GetWord();// 크리티컬 퍼센트
		pList->PhysicalDefense			= file.GetWord();// 물리 방어력	
		pList->AttribResist.SetElement_Val(ATTR_FIRE,file.GetFloat());// 속성저항력(화)% 	
		pList->AttribResist.SetElement_Val(ATTR_WATER,file.GetFloat());// 속성저항력(수)% 	
		pList->AttribResist.SetElement_Val(ATTR_TREE,file.GetFloat());// 속성저항력(목)% 	
		pList->AttribResist.SetElement_Val(ATTR_IRON,file.GetFloat());// 속성저항력(금)% 	
		pList->AttribResist.SetElement_Val(ATTR_EARTH,file.GetFloat());// 속성저항력(토)% 

		pList->WalkMove					= file.GetWord();// 몬스터걷기이동력	
		pList->RunMove					= file.GetWord();// 몬스터뛰기이동력	
		pList->RunawayMove				= file.GetWord();// 몬스터도망이동력
		
		file.GetString();// 맞음 1 연출 번호	
		file.GetString();// 죽음 1 연출 번호	
		file.GetString();// 죽음 2 연출 번호	

		pList->StandTime				= file.GetDword();
		pList->StandRate				= file.GetByte();
		pList->WalkRate					= file.GetByte();
		pList->RunRate					= file.GetByte();
		pList->DomainRange				= file.GetDword();		// 영역 범위 : 반지름
		pList->PursuitForgiveTime		= file.GetDword();
		pList->PursuitForgiveDistance	= file.GetDword();
		
		pList->bForeAttack				= file.GetBool();					// 선공격
		pList->SearchPeriodicTime		= file.GetDword();				// 탐색 주기
		pList->TargetSelect				= file.GetWord();					// 타겟 선택 : FIND_CLOSE, FIND_FIRST
		pList->SearchRange				= file.GetWord();
		pList->SearchAngle				= file.GetByte();
		pList->TargetChange				= file.GetWord();

		pList->AttackNum				= file.GetWord();
		pList->AttackIndex				= new DWORD[2];
		pList->AttackRate				= new DWORD[2];
		pList->SkillInfo				= new CSkillInfo*[2];
			
		pList->AttackIndex[0]			= file.GetWord();
		pList->AttackIndex[1]			= file.GetWord();
//		pList->SkillInfo[0]				= SKILLMGR->GetSkillInfo((WORD)pList->AttackIndex[0]);
//		pList->SkillInfo[1]				= SKILLMGR->GetSkillInfo((WORD)pList->AttackIndex[1]);
		pList->AttackRate[0]			= file.GetWord();
		pList->AttackRate[1]			= file.GetWord();

		pList->MinDropMoney				= file.GetDword();// 최소돈 액수	
		pList->MaxDropMoney				= file.GetDword();// 최대 돈 액수	
		ASSERT(pList->MaxDropMoney >= pList->MinDropMoney);
		pList->DropRate[eMONEY]			= file.GetWord();// 돈 확률(%)	
		pList->DropRate[eITEM1]			= file.GetWord();// 아이템 확률(%)	
		pList->DropRate[eITEM2]			= file.GetWord();// 아이템 확률(%)	
		pList->DropRate[eITEM3]			= file.GetWord();// 아이템 확률(%)	
		pList->DropRate[eNOITEM]		= file.GetWord();// 노템 확률(%)	

		for(int i=0;i<eDROPITEMKIND_MAX;++i)
			pList->CurDropRate[i] = pList->DropRate[i];

		pList->ItemListNum1				= file.GetWord();// 해당아이템리스트표번호	
		pList->ItemListNum2				= file.GetWord();// 해당아이템리스트표번호	
		pList->ItemListNum3				= file.GetWord();// 해당아이템리스트표번호	

		pList->MaxDropItemNum			= file.GetByte();// 최대아이템 갯수

		pList->fRunawayLifeRate			= file.GetFloat();
		pList->RunawayRate				= file.GetWord();
		pList->RunawayHelpRate			= file.GetWord();
		pList->RunawayDistance			= file.GetWord();	//????

		pList->Talk1					= file.GetWord();				// 대사 1
		pList->Talk2					= file.GetWord();				
		pList->Talk3					= file.GetWord();

		pList->HelpRequestCondition		= file.GetWord();
		pList->HelpRequestDistance		= file.GetWord();
		pList->HelpRequestTargetNum		= file.GetWord();

		pList->RecallCondition			= file.GetWord();
		pList->RecallObjectID			= file.GetWord();
		pList->RecallObjectNum			= file.GetWord();

		pList->InitHelpType				= file.GetWord();
		pList->bHearing					= file.GetBool();
		pList->HearingDistance			= file.GetDword();

//		ASSERT(!m_SMonsterListTable.GetData(pList->MonsterKind));

#ifdef _CHINA_LOCAL_
		pList->ObjectKind				= 32;
#else
		pList->ObjectKind				= file.GetWord();
#endif
		m_MonsterList.Add(pList, pList->MonsterKind);

		++n;
	}

	return TRUE;	
}

BOOL CGameResourceManager::IsDupItem( WORD wItemIdx )
{
	ITEM_INFO* pItemInfo = GetItemInfo(wItemIdx);

	switch( pItemInfo->ItemKind )
	{
	case eYOUNGYAK_ITEM:
	case eEXTRA_ITEM_JEWEL:
	case eEXTRA_ITEM_MATERIAL:
	case eEXTRA_ITEM_METAL:
	case eEXTRA_ITEM_BOOK:
	case eEXTRA_ITEM_HERB:
	case eEXTRA_ITEM_ETC:
	case eSHOP_ITEM_CHARM:
	case eSHOP_ITEM_HERB:
	case eSHOP_ITEM_SUNDRIES:
		{
			if( wItemIdx == eSundries_Shout )
				return FALSE;
		}
		return TRUE;
	case eSHOP_ITEM_INCANTATION:
		{
			if( wItemIdx == eIncantation_TownMove15 || wItemIdx == eIncantation_MemoryMove15 ||
				wItemIdx == 55357 ||
				wItemIdx == eIncantation_ShowPyoguk || wItemIdx == eIncantation_ChangeName ||
				wItemIdx == eIncantation_Tracking || wItemIdx == eIncantation_ChangeJob )
				return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

WORD CGameResourceManager::GetItemIdx_ByItemName(char* name)
{
	ITEM_INFO * pItemInfo = NULL;
	m_ItemInfoList.SetPositionHead();
	while(pItemInfo = m_ItemInfoList.GetData())
	{
		if( strcmp(pItemInfo->ItemName, name) == 0)
			return pItemInfo->ItemIdx;
	}

	return 0;
}


WORD CGameResourceManager::GetMugongIdx_ByMugongName(char* name)
{
	SKILLINFO* pSkillInfo = NULL;
	m_SkillInfoList.SetPositionHead();
	while(pSkillInfo = m_SkillInfoList.GetData())
	{
		if( strcmp(pSkillInfo->SkillName, name) == 0 )
			return pSkillInfo->SkillIdx;
	}

	return 0;
}

BOOL CGameResourceManager::IsOptionItem( WORD wItemIdx, WORD wDurability )
{
	if(wDurability != 0 && !IsDupItem(wItemIdx))
		return TRUE;
	return FALSE;
}

void CGameResourceManager::LoadQuestString()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if( !file.Init( "./Resource/QuestString.bin", "rb") )
		return;
#else
	if( !file.Init( "./Resource/QuestString.bin", "rt") )
		return;
#endif

	char Token[256];
	DWORD Key=0;
	DWORD Key_1=0;
	DWORD Key_2=0;
	CQuestString* pQuestString = NULL;
	
	while( !file.IsEOF() )
	{
		file.GetString(Token);

		if(strcmp(Token,"$SUBQUESTSTR") == 0)	
		{
			Key_1 = file.GetDword();
			Key_2 = file.GetDword();
			COMBINEKEY(Key_1, Key_2, Key);
			pQuestString = ReadQuestString(&file);
			if(pQuestString)
			{
				m_QuestStringTable.Add(pQuestString, Key);
				pQuestString->SetIndex(Key_1, Key_2);				
			}
		}
	}
}

CQuestString* CGameResourceManager::ReadQuestString(CMHFile* pFile)
{
	CQuestString* pQStr = new CQuestString;

	char buf[1024];
	char Token[256];
	int	tline=0;
	int dline=0;

	while( !pFile->IsEOF() )
	{
		pFile->GetString(Token);

		if(Token[0] ==  '}')
			break;
		
		if(strcmp(Token,"#TITLE") == 0)
		{
			pFile->GetLine(buf, 1024);
			pQStr->AddLine(buf, tline, TRUE);
		}
		if(strcmp(Token,"#DESC") == 0)
		{
			while( !pFile->IsEOF() ) 
			{
				pFile->GetLine(buf, 1024);

				if( buf[1] == '{' )				continue;
				else if( buf[1] == '}' )		break;

				pQStr->AddLine(buf, dline);
				++dline;
			}
		}
	}

	return pQStr;
}

char* CGameResourceManager::GetQuestTitle( DWORD dwQuestIdx )
{
	DWORD Key = 0;
	COMBINEKEY(dwQuestIdx, 0, Key);
	CQuestString* pQuestString = m_QuestStringTable.GetData( Key );
	if( pQuestString )
	{
		QString* pStr = (QString*)(pQuestString->GetTitle()->GetHead());
		return pStr->Str;
	}
	return "";
}

char* CGameResourceManager::GetSubQuestTitle( DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	DWORD Key = 0;
	COMBINEKEY(dwQuestIdx, dwSubQuestIdx, Key);
	CQuestString* pQuestString = m_QuestStringTable.GetData( Key );
	if( pQuestString )
	{
		QString* pStr = (QString*)(pQuestString->GetTitle()->GetHead());
		return pStr->Str;
	}
	return "";
}

BOOL CGameResourceManager::LoadMapNameList()
{
	CMHFile file;
//#ifdef _FILE_BIN_
//	if( !file.Init( "./Resource/MapName.bin", "rb") )
//		return FALSE;
//#else
	if( !file.Init( "./Resource/MapName.txt", "rt") )
		return FALSE;
//#endif

	DWORD dwMax = file.GetDword();

	DWORD dwMapNum = 0;
	for( DWORD i = 0; i < dwMax; ++i )
	{
		dwMapNum = file.GetInt();
		char* pMapName = new char[128];
		file.GetStringInQuotation( pMapName );

		m_MapNameTable.Add( pMapName, dwMapNum );		
	}

	file.Release();

	return TRUE;
}

char* CGameResourceManager::GetMapName( DWORD dwMapNum )
{
	char* pData = m_MapNameTable.GetData( dwMapNum );
	return pData;
}

DWORD CGameResourceManager::GetMapNum( char* sMapName )
{
	if( !sMapName )
		return 0;

	DWORD mapnum = 0;
	char* pName = NULL;
	m_MapNameTable.SetPositionHead();
	while( pName = m_MapNameTable.GetData() )
	{		
		++mapnum;
		if( strcmp( pName, sMapName ) == 0 )
			return mapnum;
	}

	return mapnum;
}

void CGameResourceManager::ReleaseLogTypeTable()
{
	char* pData = NULL;
	m_MoneyLogTypeTable.SetPositionHead();
	while( pData = m_MoneyLogTypeTable.GetData() )
		delete pData;
	m_MoneyLogTypeTable.RemoveAll();

	m_MugongLogTypeTable.SetPositionHead();
	while( pData = m_MugongLogTypeTable.GetData() )
		delete pData;
	m_MugongLogTypeTable.RemoveAll();

	m_ItemMoneyLogTypeTable.SetPositionHead();
	while( pData = m_ItemMoneyLogTypeTable.GetData() )
		delete pData;
	m_ItemMoneyLogTypeTable.RemoveAll();

	m_ExpPointLogTypeTable.SetPositionHead();
	while( pData = m_ExpPointLogTypeTable.GetData() )
		delete pData;
	m_ExpPointLogTypeTable.RemoveAll();

	m_RMToolLogTypeTable.SetPositionHead();
	while( pData = m_RMToolLogTypeTable.GetData() )
		delete pData;
	m_RMToolLogTypeTable.RemoveAll();

	m_GMToolLogTypeTable.SetPositionHead();
	while( pData = m_GMToolLogTypeTable.GetData() )
		delete pData;
	m_GMToolLogTypeTable.RemoveAll();

	m_PetLogTypeTable.SetPositionHead();
	while( pData = m_PetLogTypeTable.GetData() )
		delete pData;
	m_PetLogTypeTable.RemoveAll();

	/// 060831 PKH 문파토너먼트
	m_GuildTNLogTypeTable.SetPositionHead();
	while( pData = m_GuildTNLogTypeTable.GetData() )
		delete pData;
	m_GuildTNLogTypeTable.RemoveAll();

	/// 060831 PKH 공성전
	m_SiegeWarLogTypeTable.SetPositionHead();
	while( pData = m_SiegeWarLogTypeTable.GetData() )
		delete pData;
	m_SiegeWarLogTypeTable.RemoveAll();

}

BOOL CGameResourceManager::ReadLogType()
{
	CMHFile file;

	if( !file.Init("Resource/LogType.txt", "rt") )
		return FALSE;

	char temp[256] = {0, };
	while( 1 )
	{
		if( file.IsEOF() )
			break;

		file.GetString( temp );

		if( strcmp( temp, "#MONEYLOGTYPE" ) == 0 )
		{
			ReadLogTypeFromKind( &file, 0 );
		}
		else if( strcmp( temp, "#MUGONGLOGTYPE" ) == 0 )
		{
			ReadLogTypeFromKind( &file, 1 );
		}
		else if( strcmp( temp, "#ITEMMONEYLOGTYPE" ) == 0 )
		{
			ReadLogTypeFromKind( &file, 2 );
		}		
		else if( strcmp( temp, "#EXPPOINTLOGTYPE" ) == 0 )
		{
			ReadLogTypeFromKind( &file, 3 );
		}
		else if( strcmp( temp, "#RMTOOLLOGTYPE" ) == 0 )
		{
			ReadLogTypeFromKind( &file, 4 );
		}
		else if( strcmp( temp, "#GMTOOLLOGTYPE" ) == 0 )
		{
			ReadLogTypeFromKind( &file, 5 );
		}
		else if( strcmp( temp, "#PETLOGTYPE" ) == 0 )
		{
			ReadLogTypeFromKind( &file, 6 );
		}
		else if( strcmp( temp, "#PETLOGTYPE" ) == 0 )
		{
			ReadLogTypeFromKind( &file, 6 );
		}
		else if( strcmp( temp, "#GUILDTNLOGTYPE" ) == 0 )
		{
			ReadLogTypeFromKind( &file, 7 );
		}
		else if( strcmp( temp, "#SIEGEWARLOGTYPE" ) == 0 )
		{
			ReadLogTypeFromKind( &file, 8 );
		}
		/// 060915 PKH GMTOOLLOG
		else if( strcmp( temp, "#GMTOOLLOGTYPE" ) == 0 )
		{
			ReadLogTypeFromKind( &file, 9 );
		}
	}

	file.Release();

	return TRUE;
}

void CGameResourceManager::ReadLogTypeFromKind( CMHFile* pFile, DWORD dwKind )
{
	char temp[256] = {0, };
	while( 1 )
	{
		pFile->GetString( temp );
		if( temp[0] == '}' )
			return;

		if( strcmp( temp, "*TYPE" ) == 0 )
		{
			int ntype = pFile->GetInt();
			char* pType = new char[128];
			pFile->GetString( pType );

			switch( dwKind )
			{
			case 0:		m_MoneyLogTypeTable.Add( pType, ntype );		break;
			case 1:		m_MugongLogTypeTable.Add( pType, ntype );		break;
			case 2:		m_ItemMoneyLogTypeTable.Add( pType, ntype );	break;
			case 3:		m_ExpPointLogTypeTable.Add( pType, ntype );		break;
			case 4:		m_RMToolLogTypeTable.Add( pType, ntype );		break;
			case 5:		m_GMToolLogTypeTable.Add( pType, ntype );		break;
			case 6:		m_PetLogTypeTable.Add( pType, ntype );			break;
			case 7:		m_GuildTNLogTypeTable.Add( pType, ntype );		break;
			case 8:		m_SiegeWarLogTypeTable.Add( pType, ntype );		break;
			case 9:		m_GMToolLogTypeTable.Add( pType, ntype );		break;
			}
		}
	}	
}

void CGameResourceManager::LoadEtcMsgList()
{
	CMHFile file;
	int nIndex = 0;
	int nLen = 0;

	if( !file.Init("Resource/EtcMsg.txt", "rt") )
		return;

	char temp[256] = {0, };	
	while( 1 )
	{
		nIndex = file.GetInt();
		if( file.IsEOF() ) break;
		if( nIndex < 0 || nIndex >= MAX_ETC_MSGNUM ) break;
	
		file.GetStringInQuotation(temp);

		nLen = strlen( temp );
		if( nLen > 0 )
		{
			m_pEtcMsg[nIndex] = new char[nLen+1];
			memcpy( m_pEtcMsg[nIndex], temp, nLen+1 );
			file.GetLine(temp, 256);
		}
	}

	file.Release();
}

char* CGameResourceManager::GetMoneyLogType( DWORD dwLogType, char* temp )
{
	char* pData = NULL;
	if( pData = m_MoneyLogTypeTable.GetData( dwLogType ) )
		return pData;
	else
		return itoa(dwLogType, temp, 10);
}

char* CGameResourceManager::GetMugongLogType( DWORD dwLogType, char* temp )
{
	char* pData = NULL;
	if( pData = m_MugongLogTypeTable.GetData( dwLogType ) )
		return pData;
	else
		return itoa(dwLogType, temp, 10);
}

char* CGameResourceManager::GetItemMoneyLogType( DWORD dwLogType, char* temp )
{
	char* pData = NULL;
	if( pData = m_ItemMoneyLogTypeTable.GetData( dwLogType ) )
		return pData;
	else
		return itoa(dwLogType, temp, 10);
}

char* CGameResourceManager::GetExpPointLogType( DWORD dwLogType, char* temp )
{
	char* pData = NULL;
	if( pData = m_ExpPointLogTypeTable.GetData( dwLogType ) )
		return pData;
	else
		return itoa(dwLogType, temp, 10);
}

char* CGameResourceManager::GetRMToolLogType( DWORD dwLogType, char* temp )
{
	char* pData = NULL;
	if( pData = m_RMToolLogTypeTable.GetData( dwLogType ) )
		return pData;
	else
		return itoa(dwLogType, temp, 10);
}

char* CGameResourceManager::GetPetLogType( DWORD dwLogType, char* temp )
{
	char* pData = NULL;
	if( pData = m_PetLogTypeTable.GetData( dwLogType ) )
		return pData;
	else
		return itoa(dwLogType, temp, 10);
}

/// 060831 PKH 문파토너먼트
char* CGameResourceManager::GetGuildTNLogType( DWORD dwLogType, char* temp )
{
	char* pData = NULL;
	if( pData = m_GuildTNLogTypeTable.GetData( dwLogType ) )
		return pData;
	else
		return itoa(dwLogType, temp, 10);
}

/// 060831 PKH 공성전
char* CGameResourceManager::GetSiegeWarLogType( DWORD dwLogType, char* temp )
{
	char* pData = NULL;
	if( pData = m_SiegeWarLogTypeTable.GetData( dwLogType ) )
		return pData;
	else
		return itoa(dwLogType, temp, 10);
}

/// 060915 PKH GMTOOLLOG
/* --------------------------------------------------------------------------
* 함수이름 : GetGMToolLogType
* 목    적 : logtype숫자 값으로 logtype명을 꺼내옴
* 주의사항 :
*---------------------------------------------------------------------------*/
char* CGameResourceManager::GetGMToolLogType( DWORD dwLogType, char* temp )
{
	char* pData = NULL;
	if( pData = m_GMToolLogTypeTable.GetData( dwLogType ) )
		return pData;
	else
		return itoa(dwLogType, temp, 10);
}

char* CGameResourceManager::GetEtcMsg( int nMsgNum )
{
	if( m_pEtcMsg[nMsgNum] )
		return m_pEtcMsg[nMsgNum];
	else
		return &m_NULL;
}