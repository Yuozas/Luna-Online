/**************±¾×ÊÔ´À´×Ô»¥ÁªÍø ¿ª·¢ÕßÖĞĞÄÌáÊ¾½ûÉÌÒµ ½öÑ§Ï°*******************/


/*************Õ¾³¤ ĞÇÆÚ°Ë ¸ĞĞ»¹ã´óÓÃ»§¶Ô¿ª·¢ÕßÖĞĞÄÓÎÏ·×ÊÔ´ÉçÇøµÄ´óÁ¦Ö§³Ö*****************/
/**************±¾×ÊÔ´À´×Ô»¥ÁªÍø ¿ª·¢ÕßÖĞĞÄÌáÊ¾½ûÉÌÒµ ½öÑ§Ï°*******************/


/*************Õ¾³¤ ĞÇÆÚ°Ë ¸ĞĞ»¹ã´óÓÃ»§¶Ô¿ª·¢ÕßÖĞĞÄÓÎÏ·×ÊÔ´ÉçÇøµÄ´óÁ¦Ö§³Ö*****************/
// AbilityManager.cpp: implementation of the CAbilityManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AbilityManager.h"
#include "MHFile.h"
#include "AbilityGroup.h"
#include "AbilityUpdater.h"
#include "Player.h"
#ifdef _MHCLIENT_
#include "ChatManager.h"
#else
#include "MapDBMsgParser.h"
#endif
#include "QuickManager.h"

GLOBALTON(CAbilityManager);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAbilityManager::CAbilityManager()
{
	m_InfoTable.Initialize(8);
	m_pPool = new CMemoryPoolTempl<CAbility>;
#ifdef _MHCLIENT_
	m_pPool->Init(50,10,"ABILITY");
#else
	m_pPool->Init(1000,100,"ABILITY");
#endif
	m_CalcInfoTable.Initialize( 30 );
}

CAbilityManager::~CAbilityManager()
{
	delete m_pPool;
	Release();
}

void CAbilityManager::Init()
{
	CMHFile file;

#ifdef _FILE_BIN_
	if(!file.Init("./Resource/AbilityBaseInfos.bin", "rb"))
		return;
#else
	if(!file.Init("./Resource/AbilityBaseInfo.txt", "rt"))
		return;
#endif

	while(1)
	{
		if(file.IsEOF())
			break;

		CAbilityInfo* pInfo = new CAbilityInfo(&file);
		m_InfoTable.Add(pInfo,pInfo->GetIdx());
	}


	file.Release();	

	ReadCalcInfo();
}

void CAbilityManager::ReadCalcInfo()
{
	CMHFile file;	
#ifdef _FILE_BIN_
	if(!file.Init("./Resource/AbilityCalcInfos.bin", "rb"))
		return;

#else
	if(!file.Init("./Resource/AbilityCalcInfo.txt", "rt"))
		return;
#endif

	while( 1 )
	{
		if( file.IsEOF() )	break;

		ABILITY_CALC* pInfo = new ABILITY_CALC;
		DWORD level = file.GetDword();
		pInfo->dwPhyAttack = file.GetDword();
		pInfo->fAttribAttack = file.GetFloat();
		pInfo->fAttribRegist = file.GetFloat();
		pInfo->dwLife = file.GetDword();
		pInfo->dwDeffence = file.GetDword();
		pInfo->dwNearyuk = file.GetDword();
		pInfo->dwShield = file.GetDword();
		pInfo->dwUngi = file.GetDword();
		pInfo->dwStat = file.GetDword();
		pInfo->fKyunggong = file.GetFloat();
		pInfo->fNoAttrib = file.GetFloat();
		pInfo->dwSkillDamage = file.GetDword();
		pInfo->dwCriticalDamage = file.GetDword();
		pInfo->dwTitanRidingPlusTime = file.GetDword();	//2007. 11. 6. CBH - Å¸ÀÌÅº ¼ÒÈ¯ ´ÜÃà ½Ã°£
		m_CalcInfoTable.Add( pInfo, level );
	}

	file.Release();
}

void CAbilityManager::Release()
{
	CAbilityInfo* pInfo;
	m_InfoTable.SetPositionHead();
	while(pInfo = m_InfoTable.GetData())
	{
		delete pInfo;
	}
	m_InfoTable.RemoveAll();

	ABILITY_CALC* pData = NULL;
	m_CalcInfoTable.SetPositionHead();
	while( pData = m_CalcInfoTable.GetData() )

		delete pData;
	m_CalcInfoTable.RemoveAll();

	//SingletonÀ¸·Î ¸¸µë ¸Ş¸ğ¸® ÇØÁ¦ ÇÊ¿ä¾øÀ½
	//PTRLISTSEARCHSTART(m_UpdaterList,CAbilityUpdater*,pUpdater)
	//	delete pUpdater;
	//PTRLISTSEARCHEND

	m_UpdaterList.RemoveAll();
}

void CAbilityManager::AddUpdater(CAbilityUpdater* pUpdater)
{
	m_UpdaterList.AddTail(pUpdater);
}

void CAbilityManager::InitAbility(CPlayer* pObject,ABILITY_TOTALINFO* pTotalInfo,CAbilityGroup* pAbilityGroup)
{
	pAbilityGroup->Init(pTotalInfo);
	
	// update
	UpdateAbilityState(ABILITYUPDATE_ABILITYINITED,0,pAbilityGroup);
}

void CAbilityManager::InitAbilityExp(CAbilityGroup* pAbilityGroup,DWORD AbilityExp)
{
	pAbilityGroup->SetAbilityExp(AbilityExp);
}

CAbilityInfo* CAbilityManager::GetAbilityInfo(WORD AbilityIndex)
{
	CAbilityInfo* pInfo = m_InfoTable.GetData(AbilityIndex);
//	ASSERT(pInfo);
	return pInfo;	
}

int CAbilityManager::GetState(CAbilityGroup* pGroup,CAbilityInfo* pAbilityInfo)
{
	if(pGroup->GetAbility(pAbilityInfo->GetIdx()) != NULL)
		return eAIS_Usable;
	
	// º¸ÀÌ´ÂÁö °Ë»ç
	if( pAbilityInfo->GetInfo()->Ability_AcquireKind == eAAK_CharacterLevel )
	{
		ASSERT(pGroup->GetOwenerObject()->GetObjectKind() & eObjectKind_Player);
		LEVELTYPE level = ((CPlayer*)pGroup->GetOwenerObject())->GetLevel();
		LEVELTYPE needlevel = pAbilityInfo->GetInfo()->Ability_AcquireParam1;
		// 06. 01. ¾îºô¸®Æ¼Ã¢ ÀÎÅÍÆäÀÌ½º ¼öÁ¤ - ÀÌ¿µÁØ
		// ÇÊ¿ä·¹º§ 100ÀÌ¸é ¾Èº¸ÀÌ°í ³ª¸ÓÁö´Â ÀÏ´Ü º¸ÀÎ´Ù
		if(needlevel == 100)
			return eAIS_NotUsable;
		if( level >= needlevel )
			return eAIS_OnlyVisible;
	}
	else if( pAbilityInfo->GetInfo()->Ability_AcquireKind == eAAK_CharacterStage )
	{
		ASSERT(pGroup->GetOwenerObject()->GetObjectKind() & eObjectKind_Player);
		BYTE stage = ((CPlayer*)pGroup->GetOwenerObject())->GetStage();
		if( stage != eStage_Normal )	
		{
			BYTE needstage = (BYTE)pAbilityInfo->GetInfo()->Ability_AcquireParam1;
			if( needstage )
			{
				if( stage & needstage )	
					return eAIS_OnlyVisible;
			}
			else
				return eAIS_OnlyVisible;
		}

		// 06. 01. ¾îºô¸®Æ¼Ã¢ ÀÎÅÍÆäÀÌ½º ¼öÁ¤ - ÀÌ¿µÁØ
		// È­°æ ±Ø¸¶ Á¶°Ç ¹ÌÇÊ½Ã ¾Æ¿¹ ¾Èº¸ÀÎ´Ù
		return eAIS_NotUsable;
	}

	return eAIS_NotAvailable;
}


void CAbilityManager::UpdateAbilityState(DWORD Flag,DWORD Param,CAbilityGroup* pAbilityGroup)
{
	if( !pAbilityGroup->GetOwenerObject() )	return;
//#ifndef _MHCLIENT_
//	if( !pAbilityGroup->GetOwenerObject()->GetInited() )	return;
//#endif

	PTRLISTSEARCHSTART(m_UpdaterList,CAbilityUpdater*,pUpdater)
		pUpdater->Update(Flag,Param,pAbilityGroup,&m_InfoTable);
	PTRLISTSEARCHEND
}

void CAbilityManager::GetAbilityTotalInfo(CAbilityGroup* pGroup,ABILITY_TOTALINFO* pOutAbilityTotalInfo)
{
	CAbility* pAbility;
	for(int n=0;n<eAGK_Max;++n)
	{
		memset(pOutAbilityTotalInfo->AbilityDataArray[n],ABILITYLEVEL_DB_KEY,MAX_ABILITY_NUM_PER_GROUP);
		pOutAbilityTotalInfo->AbilityDataArray[n][MAX_ABILITY_NUM_PER_GROUP] = 0;

		memset(pOutAbilityTotalInfo->AbilityQuickPositionArray[n],ABILITYQUICKPOSITION_NULL,MAX_ABILITY_NUM_PER_GROUP);
		pOutAbilityTotalInfo->AbilityQuickPositionArray[n][MAX_ABILITY_NUM_PER_GROUP] = 0;
	}
	//"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
	
	// to each Ability

	// make Array like.. "DDAAAABAAAAAAAACAAAAAAAAAAAAAAAAAAAAAAA"
	BYTE Kind,Position;
	pGroup->SetPositionHead();
	while(pAbility = pGroup->GetNextAbility(eAGK_Max))
	{
		GetKindPosition_WithAbilityIndex(pAbility->GetIndex(),&Kind,&Position);
		pOutAbilityTotalInfo->AbilityDataArray[Kind][Position] += pAbility->GetLevel();

		if(pAbility->GetQuickPosition())
		{
			// 06. 01 ÀÌ¿µÁØ - ´ÜÃàÃ¢ º¯°æ
			/*
			pOutAbilityTotalInfo->AbilityQuickPositionArray[Kind][Position] = 
				ABILITYQUICKPOSITION_KEY + pAbility->GetQuickPosition() - TP_QUICK_START;
			*/
			CQuickManager::AddAbilityQuickPosition(Kind, Position, pAbility->GetQuickPosition(), pOutAbilityTotalInfo);
		}
	}
}

BOOL CAbilityManager::UpgradeAbility(WORD AbilityIndex, CAbilityGroup* pAbilityGroup)
{
	if( !pAbilityGroup->GetOwenerObject() )	return FALSE;
//#ifndef _MHCLIENT_
//	if( !pAbilityGroup->GetOwenerObject()->GetInited() )	return FALSE;
//#endif

	CAbilityInfo* pAbilInfo = GetAbilityInfo(AbilityIndex);
	if(pAbilInfo == NULL)
		return FALSE;
	if(!CanUpgrade(pAbilInfo, pAbilityGroup))
		return FALSE;
	
	//ÀÍÈ÷·Á´Â ¾îºô ·¹º§

	BYTE AbilityLevel = 0;
	CAbility* pAbility = pAbilityGroup->GetAbility(AbilityIndex);
	if(pAbility)
		AbilityLevel = pAbility->GetLevel();

	DWORD NeedAbilityExp = pAbilInfo->GetAbilityGradeExp(AbilityLevel+1);
	DWORD FromAbilityExp = pAbilityGroup->GetAbilityExp();
	DWORD ToAbilityExp = FromAbilityExp - NeedAbilityExp;

#ifdef _MAPSERVER_
	if( FromAbilityExp < NeedAbilityExp )
	{
		AbilityLevelupLog(pAbilityGroup->GetOwenerObject(),
			AbilityIndex,(pAbility ? pAbility->GetLevel() : 0) +100,
			FromAbilityExp,ToAbilityExp);

		return FALSE;
	}
#endif
	
	pAbilityGroup->SetAbilityExp(ToAbilityExp);

	BYTE AbilLevel = 0;
	if(pAbility == NULL)
	{
		pAbilityGroup->AddAbility(pAbilInfo,1,0);
		AbilLevel = 1;
	}
	else
	{		
		pAbility->IncreaseLevel();
		AbilLevel = pAbility->GetLevel();
	}

	// update
	UpdateAbilityState(ABILITYUPDATE_ABILITYLEVEL_CHANGED,0,pAbilityGroup);
	
#ifdef _MAPSERVER_
	CharacterHeroInfoUpdate(pAbilityGroup->GetOwenerObject());
	AbilityLevelupLog(pAbilityGroup->GetOwenerObject(),
					AbilityIndex,AbilLevel,
					FromAbilityExp,ToAbilityExp);
#endif
	
	return TRUE;
}

BOOL CAbilityManager::UpgradeAbilitySkPoint(WORD AbilityIndex, CAbilityGroup* pAbilityGroup)
{
	if( !pAbilityGroup->GetOwenerObject() )	return FALSE;
#ifndef _MHCLIENT_
	if( !pAbilityGroup->GetOwenerObject()->GetInited() )	return FALSE;
#endif

	CAbilityInfo* pAbilInfo = GetAbilityInfo(AbilityIndex);
	if(pAbilInfo == NULL)
		return FALSE;
	if(!CanUpgradeSkPoint(pAbilInfo, pAbilityGroup))
		return FALSE;
	
#ifdef _MAPSERVER_
	SHOPITEMWITHTIME* pItem = pAbilityGroup->GetOwenerObject()->GetShopItemManager()->GetUsingItemInfo( eIncantation_SkPointRedist );		
	SHOPITEMOPTION* pOption = pAbilityGroup->GetOwenerObject()->GetShopItemStats();
	if( !pItem || !pOption )
		return FALSE;
#endif
	
	//ÀÍÈ÷·Á´Â ¾îºô ·¹º§
	BYTE AbilityLevel = 0;
	CAbility* pAbility = pAbilityGroup->GetAbility(AbilityIndex);
	if(pAbility)
		AbilityLevel = pAbility->GetLevel();

	DWORD NeedAbilityExp = pAbilInfo->GetAbilityGradeExp(AbilityLevel+1);

	// RaMa - Æ¯±âÄ¡ÀçºĞ¹è°ü·Ã ¼öÁ¤ 
	DWORD UseSkExp = pAbilityGroup->GetOwenerObject()->GetShopItemStats()->UseSkillPoint;
	DWORD FromAbilityExp = pAbilityGroup->GetAbilityExp();
	DWORD ToAbilityExp = FromAbilityExp;

#ifdef _MAPSERVER_
	if( (FromAbilityExp+UseSkExp) < NeedAbilityExp )
	{
		AbilityLevelupLog(pAbilityGroup->GetOwenerObject(),
			AbilityIndex,(pAbility ? pAbility->GetLevel() : 0) +100,
			FromAbilityExp,ToAbilityExp);

		return FALSE;
	}
#endif
	
	if( NeedAbilityExp > UseSkExp )
	{
		DWORD remnant = NeedAbilityExp - UseSkExp;
		ToAbilityExp = FromAbilityExp - remnant;
		UseSkExp = 0;
	}
	else
		UseSkExp -= NeedAbilityExp;

	pAbilityGroup->SetAbilityExp(ToAbilityExp);
	pAbilityGroup->GetOwenerObject()->GetShopItemStats()->UseSkillPoint = UseSkExp;

	BYTE AbilLevel = 0;
	if(pAbility == NULL)
	{
		pAbilityGroup->AddAbility(pAbilInfo,1,0);
		AbilLevel = 1;
	}
	else
	{		
		pAbility->IncreaseLevel();
		AbilLevel = pAbility->GetLevel();
	}

	// update
	UpdateAbilityState(ABILITYUPDATE_ABILITYLEVEL_CHANGED,0,pAbilityGroup);
	
#ifdef _MAPSERVER_
	CharacterHeroInfoUpdate(pAbilityGroup->GetOwenerObject());
	AbilityLevelupLog(pAbilityGroup->GetOwenerObject(),

					AbilityIndex,AbilLevel,
					FromAbilityExp,ToAbilityExp);

	// ShopItemOption Update ( Param = SkillPoint, EndTime = UseSkillPoint )
	ShopItemUpdateUseInfoToDB( pAbilityGroup->GetOwenerObject()->GetID(), pItem->ShopItem.ItemBase.dwDBIdx, 
		pOption->SkillPoint, pOption->UseSkillPoint );
#endif
	
	return TRUE;
}


BOOL CAbilityManager::DowngradeAbilitySkPoint(WORD AbilityIndex, CAbilityGroup* pAbilityGroup)
{
	CAbilityInfo* pAbilInfo = GetAbilityInfo(AbilityIndex);

	if(pAbilInfo == NULL)
		return FALSE;
	if(!CanDowngradeSkPoint(pAbilInfo, pAbilityGroup))
		return FALSE;
	
#ifdef _MAPSERVER_
	SHOPITEMWITHTIME* pItem = pAbilityGroup->GetOwenerObject()->GetShopItemManager()->GetUsingItemInfo( eIncantation_SkPointRedist );		
	SHOPITEMOPTION* pOption = pAbilityGroup->GetOwenerObject()->GetShopItemStats();
	if( !pItem || !pOption )
		return FALSE;
#endif
	
	// »©·Á´Â ¾îºô ·¹º§
	BYTE AbilityLevel = 0;
	CAbility* pAbility = pAbilityGroup->GetAbility(AbilityIndex);
	if(pAbility)
		AbilityLevel = pAbility->GetLevel();
	else
	{
#ifdef _MHCLIENT_
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(743) );		
#endif
		return FALSE;
	}

	DWORD MinusAbilityExp = pAbilInfo->GetAbilityGradeExp(AbilityLevel);

	// RaMa - Æ¯±âÄ¡ÀçºĞ¹è°ü·Ã ¼öÁ¤ 
	DWORD SkillPoint = pAbilityGroup->GetOwenerObject()->GetShopItemStats()->SkillPoint;

	if( MinusAbilityExp > SkillPoint )
	{
#ifdef _MHCLIENT_
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(742) );		
#endif
		return FALSE;
	}

	else
	{		
		pAbilityGroup->GetOwenerObject()->GetShopItemStats()->SkillPoint -= MinusAbilityExp;
		pAbilityGroup->GetOwenerObject()->GetShopItemStats()->UseSkillPoint += MinusAbilityExp;
	}


	if( AbilityLevel == 1 )
	{
		// »èÁ¦
		pAbilityGroup->DeleteAbility( pAbility, pAbility->GetQuickPosition() );
		AbilityLevel = 0;
	}
	else
	{
		// ´Ù¿î
		pAbility->DecreaseLevel();
		AbilityLevel = pAbility->GetLevel();
	}

	// update
	UpdateAbilityState(ABILITYUPDATE_ABILITYLEVEL_CHANGED,0,pAbilityGroup);
	
#ifdef _MAPSERVER_
	CharacterHeroInfoUpdate(pAbilityGroup->GetOwenerObject());
	AbilityLevelupLog(pAbilityGroup->GetOwenerObject(),
					AbilityIndex,AbilityLevel,
					SkillPoint,pAbilityGroup->GetOwenerObject()->GetShopItemStats()->SkillPoint);

	// ShopItemOption Update ( Param = SkillPoint, EndTime = UseSkillPoint )
	ShopItemUpdateUseInfoToDB( pAbilityGroup->GetOwenerObject()->GetID(), pItem->ShopItem.ItemBase.dwDBIdx, 
		pOption->SkillPoint, pOption->UseSkillPoint );
#endif
	
	return TRUE;
}


BOOL CAbilityManager::SetAbilityQuickPosition(WORD AbilityIndex, POSTYPE QuickPosition,CAbilityGroup* pAbilityGroup)
{
	CAbility* pAbility = pAbilityGroup->GetAbility(AbilityIndex);
	if(pAbility == NULL)
		return FALSE;

	pAbility->SetQuickPosition(QuickPosition);

	// update
	UpdateAbilityState(ABILITYUPDATE_ABILITYQUICK_CHANGED,0,pAbilityGroup);
	
	return TRUE;
}

BOOL CAbilityManager::CanUpgrade(CAbilityInfo* pAbilInfo, CAbilityGroup* pAbilityGroup)
{
	//ÀÍÈ÷·Á´Â ¾îºô ·¹º§
	BYTE AbilityLevel = 0;
	CAbility* pAbility = pAbilityGroup->GetAbility(pAbilInfo->GetIdx());
	if(pAbility)
		AbilityLevel = pAbility->GetLevel();

	DWORD NeedAbilityExp = pAbilInfo->GetAbilityGradeExp(AbilityLevel+1);
	DWORD CurAbilityExp = pAbilityGroup->GetAbilityExp();
	if(NeedAbilityExp > CurAbilityExp)
	{
#ifdef _MHCLIENT_
		if( NeedAbilityExp != (DWORD)(-1) )
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(624) );
#endif
		return FALSE;
	}


 	//abil hack check
 	if( pAbilInfo->GetInfo()->Ability_AcquireKind == eAAK_CharacterLevel )
 	{
 		if( pAbilInfo->GetInfo()->Ability_AcquireParam1 > ((CPlayer*)pAbilityGroup->GetOwenerObject())->GetLevel() )
 			return FALSE;
 	}
	// check stage
	else if( pAbilInfo->GetInfo()->Ability_AcquireKind == eAAK_CharacterStage )
	{
		BYTE stage = ((CPlayer*)pAbilityGroup->GetOwenerObject())->GetStage();
		if( stage == eStage_Normal )	return FALSE;
		BYTE needstage = (BYTE)pAbilInfo->GetInfo()->Ability_AcquireParam1;
		if( needstage )
		{
			if( !(stage & needstage) )
				return FALSE;

			////////////////////////////////////////////////////////////
			// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
			// ÀüÅõÆ¯±â
			// Æ¯±âÂÊ¿¡ ÀÚ¸®°¡ ¾ø¾î¼­ ¾îÂ¿¼ö ¾øÀÌ
			// ·¹º§¾÷ Á¦ÇÑÀ» ÇÏµåÄÚµùÀ¸·Î Á¦ÇÑ
			if(pAbility)
			{
				WORD index = pAbility->GetIndex();


				if(index >= 117 && index <= 122)
				{
					if(AbilityLevel >= 10 && stage != eStage_Hyun)
						return FALSE;
				}
				if(index >= 123 && index <= 128)
				{
					if(AbilityLevel >= 10 && stage != eStage_Tal)
						return FALSE;
				}
			}
			////////////////////////////////////////////////////////////

		}
	}
	return TRUE;
}

BOOL CAbilityManager::CanUpgradeSkPoint(CAbilityInfo* pAbilInfo, CAbilityGroup* pAbilityGroup)
{
	//ÀÍÈ÷·Á´Â ¾îºô ·¹º§
	BYTE AbilityLevel = 0;
	CAbility* pAbility = pAbilityGroup->GetAbility(pAbilInfo->GetIdx());
	if(pAbility)
		AbilityLevel = pAbility->GetLevel();

	DWORD NeedAbilityExp = pAbilInfo->GetAbilityGradeExp(AbilityLevel+1);	
	DWORD TotalPoint = pAbilityGroup->GetAbilityExp()+pAbilityGroup->GetOwenerObject()->GetShopItemStats()->UseSkillPoint;

	if(NeedAbilityExp > TotalPoint)
	{
#ifdef _MHCLIENT_
		if( NeedAbilityExp != (DWORD)(-1) )
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(624) );
#endif
		return FALSE;
	}
	
	return TRUE;
}

BOOL CAbilityManager::CanDowngradeSkPoint(CAbilityInfo* pAbilInfo, CAbilityGroup* pAbilityGroup)
{
	//ÀÍÈ÷·Á´Â ¾îºô ·¹º§
	BYTE AbilityLevel = 0;
	CAbility* pAbility = pAbilityGroup->GetAbility(pAbilInfo->GetIdx());
	if(pAbility)
		AbilityLevel = pAbility->GetLevel();
	else
	{
#ifdef _MHCLIENT_
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 741 ), pAbilInfo->GetInfo()->Ability_name );
#endif
		return FALSE;
	}

	DWORD NeedAbilityExp = pAbilInfo->GetAbilityGradeExp(AbilityLevel);	
	DWORD TotalPoint = pAbilityGroup->GetOwenerObject()->GetShopItemStats()->SkillPoint;

	if(NeedAbilityExp > TotalPoint)
	{
#ifdef _MHCLIENT_
		if( NeedAbilityExp != (DWORD)(-1) )
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(742) );
#endif
		return FALSE;
	}
	
	return TRUE;
}

// magi82(42) - ¼¥¾ÆÀÌÅÛ Ãß°¡(¼ö·ÃÄ¡ ÃÊ±âÈ­)
BOOL CAbilityManager::ResetAbilitySkPoint(WORD AbilityIndex, CAbilityGroup* pAbilityGroup)
{
	CPlayer* pPlayer = pAbilityGroup->GetOwenerObject();
	if( !pPlayer )
		return FALSE;

#ifdef _MAPSERVER_
	if( !pPlayer->GetInited() )
		return FALSE;

#endif

	CAbilityInfo* pInfo = GetAbilityInfo(AbilityIndex);
	if( NULL == pInfo )
		return FALSE;

	// ¸®¼Â °¡´ÉÇÑ ½ºÅ³ÀÎÁö Ã¼Å©
#ifdef _MAPSERVER_
	if( FALSE == CheckCanResetPoint(pPlayer, pInfo) )
		return FALSE;
#endif

	// ÃÊ±âÈ­ ÇÏ·Á´Â ¼ö·ÃÄ¡
	int nLevel = 0;
	DWORD dwResetTotalPoint = 0;
	DWORD dwPoint = 0;
	CAbility* pAbility = pAbilityGroup->GetAbility(AbilityIndex);
	if( pAbility )


	{
		nLevel = pAbility->GetLevel();

		// ÃÊ±âÈ­ µÇ´Â ¼ö·ÃÄ¡ÀÇ ÃÑ Æ÷ÀÎÆ®
		for( int i = 0; i < nLevel; ++i )
		{
            dwPoint = pInfo->GetAbilityGradeExp( i + 1 );
			dwResetTotalPoint += dwPoint;
		}
	}
	else
	{
		return FALSE;
	}

	// ³²Àº ¼ö·ÃÄ¡ Æ÷ÀÎÆ® °»½Å
	DWORD dwBeforePoint = pAbilityGroup->GetAbilityExp();
    	pAbilityGroup->SetAbilityExp( dwBeforePoint + dwResetTotalPoint );

	// ½ºÅ³ »èÁ¦
	pAbilityGroup->DeleteAbility( pAbility, pAbility->GetQuickPosition() );
	nLevel = 0;

	// update
	UpdateAbilityState(ABILITYUPDATE_ABILITYLEVEL_CHANGED,0,pAbilityGroup);

#ifdef _MAPSERVER_
	CharacterHeroInfoUpdate(pPlayer);
	AbilityLevelupLog(pPlayer, AbilityIndex,nLevel, dwBeforePoint,pAbilityGroup->GetAbilityExp());
#endif

	return TRUE; 

}

// magi82(42) - ¼¥¾ÆÀÌÅÛ Ãß°¡(¼ö·ÃÄ¡ ÃÊ±âÈ­)
BOOL CAbilityManager::CheckCanResetPoint(CPlayer* pPlayer, CAbilityInfo* pAbilInfo)

{
	// 1ÀÌ¸é ¸®¼Â °¡´É, 0ÀÌ¸é ¸®¼Â ºÒ°¡´É
	if( 0 == pAbilInfo->GetResetFlag() )
		return FALSE;
	
	// »ıÈ°Æ¯±âÁß ±âº»Æ¯±â´Â ÀçºĞ¹è ÇÒ ¼ö ¾ø´Ù.
	//if( pAbilInfo->GetInfo()->Ability_index >= 401 && pAbilInfo->GetInfo()->Ability_index <= 406 )
	//	return FALSE;

	// Áöµµ·ÂÀº ÃÊ±âÈ­ ÇÒ¼ö ¾ø´Ù.
#define ABILITY_LEADERSHIP	411
	if( pPlayer->GetGuildMemberRank()==GUILD_MASTER &&
		pAbilInfo->GetInfo()->Ability_index == ABILITY_LEADERSHIP )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CAbilityManager::CheckAbilityLevel(WORD AbilityIndex,CAbilityGroup* pAbilityGroup,BYTE Level)
{
	BYTE AbilityLevel = 0;
	CAbility* pAbility = pAbilityGroup->GetAbility(AbilityIndex);
	if(pAbility)
		AbilityLevel = pAbility->GetLevel();

	return AbilityLevel >= Level;
}
BYTE CAbilityManager::GetAbilityLevel(WORD AbilityIndex,CAbilityGroup* pAbilityGroup)
{
	BYTE AbilityLevel = 0;
	CAbility* pAbility = pAbilityGroup->GetAbility(AbilityIndex);
	if(pAbility)
		AbilityLevel = pAbility->GetLevel();

	return AbilityLevel;
}
CAbility* CAbilityManager::MakeNewAbility()
{
	return m_pPool->Alloc();
}

void CAbilityManager::ReleaseAbility(CAbility* pAbility)
{
	m_pPool->Free(pAbility);
}

ABILITY_CALC* CAbilityManager::GetAbilityCalcInfo( BYTE bAbilityLevel )
{
	if( bAbilityLevel == 0 )
		return NULL;
	else if( bAbilityLevel > 30 )
		bAbilityLevel = 30;
	return m_CalcInfoTable.GetData( bAbilityLevel );
}

float CAbilityManager::GetAbilityKyungGongSpeed( WORD wLevel )
{
	if( wLevel > 30 )	wLevel = 30;
	ABILITY_CALC* pInfo = m_CalcInfoTable.GetData( wLevel );
	if( pInfo )	return pInfo->fKyunggong;


	return 0.0f;

}




void CAbilityManager::__TranData()
{
	CMHFile file;
	file.Init("src.txt","r");
	FILE* fpout = fopen("out.txt","w");
	ABILITY_TOTALINFO abiltotalinfo;

	char name[20];
	while(1)
	{

		if(file.IsEOF() != FALSE)
			break;

		DWORD PlayerID = file.GetDword();
		file.GetString(name);
		file.GetString(abiltotalinfo.AbilityDataArray[0]);
		file.GetString(abiltotalinfo.AbilityDataArray[1]);
		file.GetString(abiltotalinfo.AbilityDataArray[2]);
		file.GetString(abiltotalinfo.AbilityDataArray[3]);

		CAbilityGroup group;
		group.Init(&abiltotalinfo);
		group.SetPositionHead();
		while(CAbility* pAbility = group.GetNextAbility(eAGK_Max))
		{
			CAbilityInfo* pInfo = pAbility->GetAbilityInfo();
			fprintf(fpout,"%d\t%s\t%d\t%s\n",PlayerID,name,pInfo->GetIdx(),pInfo->GetInfo()->Ability_name);

		}

		group.Release();
	}

	fclose(fpout);
	file.Release();
}




