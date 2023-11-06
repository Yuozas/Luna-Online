#include "stdafx.h"
#include "Player.h"
//#include "Monster.h"
#include "PartyManager.h"
#include "Party.h"
#include "CharMove.h"
#include "PackedData.h"
//#include "..\[CC]Header\GameResourceManager.h"
#include "MapDBMsgParser.h"
#include "ItemManager.h"
#include "../[cc]skill/server/manager/skillmanager.h"
#include "KyungGongManager.h"
#include "ObjectEvent.h"
#include "ObjectStateManager.h"
#include "streetstallmanager.h"
//#include "PeaceWarModManager.h"
/*�����Ƽ ���� - #include "AbilityManager.h"*/
#include "CharacterCalcManager.h"
//#include "Wantedmanager.h"
#include "GuildManager.h"
#include "SkillTreeManager.h"
#include "../[cc]skill/Server/info/SkillInfo.h"
#include "../[cc]skill/Server/info/BuffSkillInfo.h"
#include "../[cc]skill/Server/info/ActiveSkillInfo.h"
#include "Showdown/Battle_Showdown.h"
#include "StateMachinen.h"
#include "MHError.h"

#include "CQuestBase.h"
#include "Quest.h"

#include "TileManager.h"
#include "mhtimemanager.h"

#include "LootingManager.h"
#include "PKManager.h"
#include "QuestManager.h"
#include "StorageManager.h"
#include "FameManager.h"
#include "EventMapMgr.h"
#include "UserTable.h"
#include "GuildFieldWarMgr.h"
#include "PartyWarMgr.h"
#include "QuestMapMgr.h"
#include "GuildTournamentMgr.h"
#include "SiegeWarMgr.h"
#include "SiegeWarProfitMgr.h"
#include "StreetStall.h"
#include "QuickManager.h"
#include "WeatherManager.h"
#include "FishingManager.h"

#include "../[CC]BattleSystem/BattleSystem_Server.h"
#include "../[cc]skill/server/tree/SkillTree.h"
#include "../[CC]Skill\Server\Delay\Delay.h"

#include "AttackManager.h"

// desc_hseos_���͹���01
// S ���͹��� �߰� added by hseos 2007.05.23	2007.07.08
#include "../hseos/Common/SHMath.h"
#include "../hseos/Monstermeter/SHMonstermeterManager.h"
// E ���͹��� �߰� added by hseos 2007.05.23	2007.07.08

// desc_hseos_����ý���_01
// S ����ý��� �߰� added by hseos 2008.01.22
#include "../hseos/Farm/SHFarmManager.h"
// E ����ý��� �߰� added by hseos 2008.01.22

// desc_hseos_�йи�01
// S �йи� �߰� added by hseos 2007.07.15
#include "../hseos/Family/SHFamilyManager.h"
// E �йи� �߰� added by hseos 2007.07.15
// desc_hseos_����Ʈ ��_01
// S ����Ʈ �� �߰� added by hseos 2007.11.27
#include "../hseos/Date/SHDateManager.h"
// E ����Ʈ �� �߰� added by hseos 2007.11.27

// desc_hseos_��ȥ_01
// S ��ȥ �߰� added by hseos 2007.12.11
#include "../hseos/Marriage/SHMarriageManager.h"
// E ��ȥ �߰� added by hseos 2007.12.11

//#include "MapDBMsgParser.h"
// 071206 KTH BuffSkillObject �˻��� ���� �߰�
#include "../[cc]skill/server/Object\BuffSkillObject.h"

// 080225 LUJ, ��� ���� ������ ���� �߰�
#include "Guild.h"
#include "Network.h"
// 080516 LUJ, ��ų ��Ÿ���� ���� ȸ���̻� ���� �� ���� �����ϱ� ���� ����
#include "MapNetworkMsgParser.h"

#include "Pet.h"
#include "petmanager.h"


#define INSERTLOG_TIME			1800000 // 10 min
#define INSERTLOG_TIME_CHINA	1800000 // 30 min
#define INSERTLOG_TIME_HK		1800000 // 30 min

extern int	g_nHackCheckNum;
extern int	g_nHackCheckWriteNum;
extern int	g_nServerSetNum;

WORD CLASS[3] = { 20, 25, 15 };


CPlayer::CPlayer() :
// 080617 LUJ, ��Ÿ�� ���� �޸𸮸� �Ҵ�
mSkillCoolTimeMap( new SkillCoolTimeMap ),
mCoolTimeMap( new CoolTimeMap ),
// 080616 LUJ, ���� ������ ���� �����̳ʸ� �Ҵ�
mTrapSet( new TrapSet )
{
	/*�����Ƽ ���� - m_AbilityGroup.SetOwenerObject(this);*/
//	m_SkillList.Initialize(10);
	m_SkillFailCount = 0;

	m_SkillTree.Init( this );
	mExceptionTable.Initialize(1);
	mPetInfoTable.Initialize(1);

	mpBattleStateDelay = new cDelay;
	mpBattleStateDelay->Init( 10000 );

	m_God = FALSE;

	mIsEmergency = FALSE;

	mTargetMap = 0;
	mTargetPosX = 0;
	mTargetPosZ = 0;

	// 071128 LYW --- Player : HPMP ����.
	m_byHP_Point = 0;
	m_byMP_Point = 0;

	m_bResetSkill	=	false;
	m_bResetStat	=	false;

	mpCurPet = NULL;
}

CPlayer::~CPlayer()
{
	delete mpBattleStateDelay;

	// 080617 LUJ, �Ҵ�� ��Ÿ�� �� �޸𸮸� ����
	SAFE_DELETE( mSkillCoolTimeMap );
	SAFE_DELETE( mCoolTimeMap );
	// 080616 LUJ, �Ҵ�� ���� ���� ����
	SAFE_DELETE( mTrapSet );

//	m_SkillList.RemoveAll();
}

void CPlayer::InitClearData()
{
	m_ItemContainer.Init();
	//m_ItemContainer.SetInit(eItemTable_Inventory,	TP_INVENTORY_START,		SLOT_INVENTORY_NUM,		&m_InventorySlot);
	m_ItemContainer.SetInit(eItemTable_Inventory,	TP_INVENTORY_START,		SLOT_MAX_INVENTORY_NUM,		&m_InventorySlot);

	m_ItemContainer.SetInit(eItemTable_Weared,		TP_WEAR_START,			SLOT_WEAR_NUM,			&m_WearSlot);
	m_ItemContainer.SetInit(eItemTable_Storage,		TP_STORAGE_START,		SLOT_STORAGE_NUM,		&m_StorageSlot);
	m_ItemContainer.SetInit(eItemTable_Shop,		TP_SHOPITEM_START,		SLOT_SHOPITEM_NUM,		&m_ShopItemSlot);	
	m_ItemContainer.SetInit(eItemTable_ShopInven,	TP_SHOPINVEN_START,		SLOT_SHOPINVEN_NUM/2,	&m_ShopInvenSlot);
	// member data init
	memset(&m_HeroCharacterInfo,0,sizeof(CHARACTER_TOTALINFO));
	memset(&m_HeroInfo,0,sizeof(HERO_TOTALINFO));
	//memset(&m_itemStats,0,sizeof(m_itemStats));
	// RaMa - 04.11.08 ( ShopItem���� �߰��Ǵ� �ɼ� )
//	memset(&m_ShopItemOption,0,sizeof(SHOPITEMOPTION));
//	for(int i=eAvatar_Weared_Hair; i<eAvatar_Max; i++)
//		m_ShopItemOption.Avatar[i]=1;
//	memset(&m_AvatarOption, 0 , sizeof(AVATARITEMOPTION));

	m_ItemArrayList.Initialize(10);
	m_FollowMonsterList.Initialize(5);


//SW050920 Rare
	//m_ItemRareOptionList.Initialize(MAX_ITEM_OPTION_NUM);
	m_QuestList.Initialize(30);
//	m_WantedListHashTable.Initialize(2);

	m_QuestGroup.Initialize( this );
//	m_ShopItemManager.Init( this );

	// 071115 LYW --- Player : ���� ������ ���� �߰�.
	//m_ChargeItemManager.Init( this ) ;

	m_InitPosition = 0;
	m_bSavePointInited = FALSE;
	m_bPetInvenInit = FALSE;

	//m_pCurPet = NULL;
	m_wKillMonsterLevel = 0;

	// 06. 03 �������� - �̿���
	m_dwMussangTime = 0;
	m_wMussangStat = 0;

	m_bGotWarehouseItems = FALSE;
	m_SkillFailCount = 0;

	memset( &m_DateMatching, 0, sizeof(DATE_MATCHING_INFO));			// ����Ʈ ��Ī �ֹ���.
	// desc_hseos_�ֹε��01
	// S �ֹε�� �߰� added by hseos 2007.06.09
	m_DateMatching.nSerchTimeTick = gCurTime;
	m_DateMatching.nRequestChatTimeTick = gCurTime;
	// E �ֹε�� �߰� added by hseos 2007.06.09
	memset( &mPassiveStatus, 0, sizeof( Status ) );
	memset( &mBuffStatus, 0, sizeof( Status ) );
	memset( &mRatePassiveStatus, 0, sizeof( Status ) );
	memset( &mRateBuffStatus, 0, sizeof( Status ) );
	memset( &mAbnormalStatus, 0, sizeof( AbnormalStatus ) );

	// desc_hseos_���͹���01
	// S ���͹��� �߰� added by hseos 2007.05.23
	ZeroMemory(&m_stMonstermeterInfo, sizeof(m_stMonstermeterInfo));
	m_stMonstermeterInfo.nPlayTimeTick = gCurTime;
	// E ���͹��� �߰� added by hseos 2007.05.23

	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.07.06	2007.07.17
	m_pcFamilyEmblem = NULL;
	// E �йи� �߰� added by hseos 2007.07.06	2007.07.17

	// desc_hseos_����ý���_01
	// S ����ý��� �߰� added by hseos 2007.08.23
	ZeroMemory(&m_stFarmInfo, sizeof(m_stFarmInfo));
	m_stFarmInfo.nCropPlantRetryTimeTick  = gCurTime;
	m_stFarmInfo.nCropManureRetryTimeTick = gCurTime;
	// 080519 KTH
	m_stFarmInfo.nAnimalFeedRetryTimeTick = gCurTime;
	m_stFarmInfo.nAnimalCleanRetryTimeTick = gCurTime;
	// E ����ý��� �߰� added by hseos 2007.08.23

	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2007.11.27	2007.11.28	2007.12.05
	m_nChallengeZoneEnterFreq = 0;
	m_nChallengeZoneEnterBonusFreq = 0;
	m_nChallengeZonePartnerID = 0;
	m_nChallengeZoneSection = 0;
	m_nChallengeZoneStartState = 0;
	m_nChallengeZoneStartTimeTick = 0;
	m_nChallengeZoneMonsterNum = 0;
	m_nChallengeZoneKillMonsterNum = 0;
	m_bChallengeZoneNeedSaveEnterFreq = FALSE;
	m_bChallengeZoneCreateMonRightNow = FALSE;
	m_nChallengeZoneMonsterNumTillNow = 0;
	m_nChallengeZoneClearTime = 0;
	// E ����Ʈ �� �߰� added by hseos 2007.11.27	2007.11.28	2007.12.05

	// desc_hseos_��ȥ_01
	// S ��ȥ �߰� added by hseos 2007.12.11
	m_nMarriageRequestPartnerID = 0;
	m_nMarriagePartnerID = 0;
	m_nMarriageTeleportFreq = 0;
	ZeroMemory(m_szMarriageDivorceDate, sizeof(m_szMarriageDivorceDate));
	// E ��ȥ �߰� added by hseos 2007.12.11

	m_God = FALSE;

//---KES Aggro 070918
//---��׷� ���� ���� �ʱ�ȭ
	m_htAggroedTable.Initialize(10);
//-------------------
	
	mExceptionTable.RemoveAll();

	mbBuffSkillLoad = FALSE;
	mbPetLoad = FALSE;

	mIsEmergency = FALSE;

	mTargetMap = 0;
	mTargetPosX = 0;
	mTargetPosZ = 0;

	m_bResetSkill	=	false;
	m_bResetStat	=	false;

	m_dwReturnSkillMoney = 0;

//---KES AUTONOTE
	m_dwAutoNoteIdx = 0;
	m_dwLastActionTime = 0;
//---------------

	FishingInfoClear();
	m_dwFM_MissionCode = 1000000;

	int i;
	for(i=0; i<eFishItem_Max; i++)
	{
		m_fFishItemRate[i] = 0.0f;
	}
	m_lstGetFishList.clear();
	

	m_wFishingLevel = 1;
	m_dwFishingExp = 0;
	m_dwFishPoint = 0;

	// 071122
//	m_htTimeCheckItem.Initialize(10) ; //��� �� 10���� ���� ������ ������
	// 071221 KTH -- �� ��� �ʱ�ȭ
	//m_MemoryStoneInfoMap.clear();
	mpCurPet = NULL;

	memset(tempHideParts, 0, sizeof(tempHideParts));
}

BOOL CPlayer::Init(EObjectKind kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo)
{
	m_StreetStallTitle[0] = 0;
	m_bPetInvenInit = FALSE;
	m_bStorageInit = 0;
	m_bShopInit = FALSE;
	m_WantedIDX = 0;
	m_MurdererIDX = 0;
	m_MurdererKind = 0;
	m_MurderObjKind = 0;
	m_bPenaltyByDie = FALSE;

	m_bReadyToRevive = FALSE;
	m_bSavePointInited = FALSE;
//KES SHOWDOWN 031101
	m_bShowdown		= FALSE;
//KES EXIT
	m_bExit			= FALSE;
	m_bNormalExit	= FALSE;
//
	CObject::Init(kind, AgentNum, pBaseObjectInfo); //���ϡ�a������A eObjectState_NoneA����I ������c��U.

//KES 040827
	OBJECTSTATEMGR_OBJ->StartObjectState( this, eObjectState_Immortal, 0 );
	// 06.08.29. RaMa.
	OBJECTSTATEMGR_OBJ->EndObjectState( this, eObjectState_Immortal, 30000 );
//

	m_CurComboNum = 0;
	SetGravity(0);
	m_pGuetStall = NULL;

//KES
	m_bNeedRevive = TRUE;
	ZeroMemory( &m_GameOption, sizeof( m_GameOption ) );
//
	//m_pItemContainer = new CItemContainer;


//	if( !m_pQuestGroup )
//		m_pQuestGroup = new CQuestGroup(this);

	m_LoginTime = 0;
	m_ContinuePlayTime = 0;

//HACK CHECK INIT
	m_nHackCount = 0;
	m_dwHackStartTime = gCurTime;
	m_dwLastSocietyAction = 0;
//

	m_wKillMonsterLevel = 0;

	m_ReviveTime = 0;

	m_bDieForGFW = FALSE;
	m_bDieInSpecialMap = FALSE;
	
	//SW051112 ���ָ��
	InitMussangPoint();
	m_dwMussangStartTime = 0;
	m_bIsMussangReady = FALSE;
	m_bMussangFlag = 0;

	//m_pCurPet	= NULL;

	m_bWaitExitPlayer = TRUE;

	// 06. 03 �������� - �̿���
	m_dwMussangTime = 0;
	m_wMussangStat = 0;

	m_bWaitExitPlayer = TRUE;

//	m_SkillList.RemoveAll();
	m_SkillFailCount = 0;

	for( BYTE i = 0; i < 8; i++ )
	{
		m_QuickSlot[i].Init( GetID(), i );
	}

	// desc_hseos_���͹���01
	// S ���͹��� �߰� added by hseos 2007.05.23
	// ..�α׸� ���ܼ� �׽�Ʈ �� �� ��� ���� Init, InitClearData, Monstermeter_Load, Init �� ������ ����ȴ�.
	// ..��� ��쿡�� Init, InitClearData, Init, Monstermeter_Load �� �Ǳ⵵ �Ѵ�.
	// ..�׷��� ���⼭�� ���͹��� ������ �ʱ�ȭ �ϸ� �� �ȴ�. InitClearData �� DB���� �д� �κп��� �ʱ�ȭ�� �Ǳ� ������
	// ..������ ���� ����.
//	ZeroMemory(&m_stMonstermeterInfo, sizeof(m_stMonstermeterInfo));
	m_stMonstermeterInfo.nPlayTimeTick = gCurTime;
	// E ���͹��� �߰� added by hseos 2007.05.23
	
	mExceptionTable.RemoveAll();
	
	mIsEmergency = FALSE;

	mTargetMap = 0;
	mTargetPosX = 0;
	mTargetPosZ = 0;

	m_dwLastTimeCheckItemDBUpdate = gCurTime;
	m_dwLastTimeCheckItem = gCurTime;

	m_bNoExpPenaltyByPK = FALSE;

	// 080514 LUJ, ��ų �ִϸ��̼� �ð� �ʱ�ȭ
	mSkillAnimationTime	= 0;
	// 080515 LUJ, ��Ÿ�� üũ�� ����ü �ʱ�ȭ
	ZeroMemory( &mCheckCoolTime, sizeof( mCheckCoolTime ) );
	// 080616 LUJ, ���� ���� ������ �ʱ�ȭ�Ѵ�
	mTrapSet->clear();
	mpCurPet = NULL;
	return TRUE;
}
void CPlayer::Release()
{
//---KES Aggro 070918
//---��׷� ����Ʈ ������
	RemoveAllAggroed();
//-------------------

	FishingData_Update(GetID(), GetFishingLevel(), GetFishingExp(), GetFishPoint());
	// 080808 LUJ, ���� ������ �α׷� ����
	Log_Fishing(
		GetID(),
		eFishingLog_Regular,
		0,
		GetFishPoint(),
		0,
		0,
		GetFishingExp(),
		GetFishingLevel() );

	GUILDMGR->RemovePlayer( this );
	ITEMMGR->RemoveCoolTime( GetID() );
	// 080617 LUJ, ������ ������ ����
	mCoolTimeMap->clear();
	// 080509 LUJ, ��ų ��Ÿ���� ������ �����̳ʸ� �ʱ�ȭ�Ѵ�
	// 080617 LUJ, ������ ������ ����
	mSkillCoolTimeMap->clear();

	{
		for( POSTYPE position = 0; position < m_ItemContainer.GetSize(); ++position )
		{
			const ITEMBASE* item = m_ItemContainer.GetItemInfoAbs( position );

			if( ! item )
			{
				continue;
			}

			ITEMMGR->RemoveOption( item->dwDBIdx );
		}

		m_ItemContainer.Release();
	}

	{
		m_ItemArrayList.SetPositionHead();

		for(
			ITEMOBTAINARRAYINFO* pInfo;
			pInfo = m_ItemArrayList.GetData(); )
		{
			ITEMMGR->Free(this, pInfo);
		}

		m_ItemArrayList.RemoveAll();
	}

	m_FollowMonsterList.RemoveAll();
	
	{		
		m_QuestList.SetPositionHead();

		for(
			CQuestBase* pQuest;
			pQuest = m_QuestList.GetData(); )
		{
			delete pQuest;
		}

		m_QuestList.RemoveAll();
	}
	

	m_InventoryPurse.Release();
	m_StoragePurse.Release();

	m_WantedList.RemoveAll();

	m_QuestGroup.Release();

//	m_ShopItemManager.Release();

	// 071115 LYW --- Player : ���� ������ ���� �߰�.
	//m_ChargeItemManager.Release() ;

	CObject::Release();


//	if( m_pQuestGroup )
//	{
//		delete m_pQuestGroup;
//		m_pQuestGroup = NULL;
//	}
//	m_SkillList.RemoveAll();
	m_SkillFailCount = 0;

	m_SkillTree.Release();

	SAFE_DELETE_ARRAY(m_pcFamilyEmblem);	

	mExceptionTable.RemoveAll();

	if( mpCurPet )
	{
		char log[256];
		sprintf( log, "./log/pet%02d.txt", g_pServerSystem->GetMapNum() );
		FILE* fp = fopen( log, "a+" );

		if( fp )
		{
			fprintf( fp, "USERRELEASE:\t%d\n", mpCurPet );
			fclose( fp );
		}

		PETMGR->SealPet( mpCurPet );
	}
	// 071122
//	m_htTimeCheckItem.RemoveAll();
}


DWORD CPlayer::GetGravity()
{
	DWORD totalG = 0;
	CMonster * pObject;
	m_FollowMonsterList.SetPositionHead();
	DWORD gravity = 0;
	while(pObject = m_FollowMonsterList.GetData())
	{
//---KES ��Ʈ��Ʈ ���� ���� ����
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

BOOL CPlayer::AddFollowList(CMonster * pMob)
{	
//	ASSERT(m_FollowMonsterList.GetData(pMob->GetID()) == NULL);
//	if(m_FollowMonsterList.GetData(pMob->GetID()) != NULL)
//	{
//		MHERROR->OutputFile("Debug.txt", MHERROR->GetStringArg("addfollow pID:%d,mID:%d,pstate:%d,mstate:%d,pG:%d,mobG:%d", 
//										GetID(),
//										pMob->GetID(), 
//										GetState(), 
//										pMob->GetState(),
//										GetGravity(), 
//										pMob->GetGravity()));
//	}
//KES ���� �ְ�! ���߿� ������ ����ϵ��� ��������.
//	if( GetGravity() + pMob->GetGravity() <= 100 )
	if( m_FollowMonsterList.GetDataNum() < 50 )		//max 50����
	{
		m_FollowMonsterList.Add(pMob, pMob->GetID());
		return TRUE;
	}
	else
	{
		return FALSE;
	}
//	else
//	{
//		MHERROR->OutputFile("Debug.txt", MHERROR->GetStringArg("Addfollow 100 over pID:%d,mID:%d,pstate:%d,mstate:%d,pG:%d,mobG:%d", 
//										GetID(),
//										pMob->GetID(), 
//										GetState(), 
//										pMob->GetState(),
//										GetGravity(), 
//										pMob->GetGravity()));
//	}
}
BOOL CPlayer::RemoveFollowAsFarAs(DWORD GAmount, CObject* pMe )
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

void CPlayer::RemoveFollowList(DWORD ID)
{
	m_FollowMonsterList.Remove(ID);
//	int a = m_FollowMonsterList.GetDataNum();
}

ITEMOBTAINARRAYINFO * CPlayer::GetArray(WORD id)
{
	return m_ItemArrayList.GetData(id);
}
void CPlayer::AddArray(ITEMOBTAINARRAYINFO * pInfo)
{
	m_ItemArrayList.Add(pInfo, pInfo->wObtainArrayID);
}
void CPlayer::RemoveArray(DWORD id)
{
	m_ItemArrayList.Remove(id);
}
void CPlayer::InitCharacterTotalInfo(CHARACTER_TOTALINFO* pTotalInfo)
{
	memcpy(&m_HeroCharacterInfo,pTotalInfo,sizeof(CHARACTER_TOTALINFO));
	if( GetUserLevel() == eUSERLEVEL_GM )
		m_HeroCharacterInfo.bVisible = FALSE;
	else
		m_HeroCharacterInfo.bVisible = TRUE;

	// 071226 KTH -- �κ��丮�� ũ�⸦ �����Ͽ� �ش�.
	m_InventorySlot.SetSlotNum(SLOT_INVENTORY_NUM + GetInventoryExpansionSize());

}

void CPlayer::InitHeroTotalInfo(HERO_TOTALINFO* pHeroInfo)
{
	memcpy(&m_HeroInfo,pHeroInfo,sizeof(HERO_TOTALINFO));

	m_HeroInfo.MaxMussangPoint = GetMussangMaxPoint();
	/* Money Setting */
	m_ItemContainer.GetSlot(eItemTable_Inventory)->CreatePurse(&m_InventoryPurse, this, m_HeroInfo.Money, MAX_INVENTORY_MONEY);
	/* BadFame Reduce Time Init*/
	m_dwLastReduceHour = ( m_HeroInfo.Playtime - m_HeroInfo.LastPKModeEndTime ) / 3600; //��A�ơ�
}

void CPlayer::InitItemTotalInfo(ITEM_TOTALINFO* pItemInfo)
{
	//m_pIconTable[eItemTable_Inventory]->Init(MAX_INVENTORY_NUM, pItemInfo->Inventory, GetID(), INVENTORY_STARTPOSITION );
	//m_pIconTable[eItemTable_Weared]->Init(eWearedItem_Max, pItemInfo->WearedItem, GetID(), WEAR_STARTPOSITION);
	//m_pIconTable[eItemTable_StreetStall]->Init(MAX_STREETSTALL_ITEMNUM, pStallItem, GetID(), STALL_STARTPOSITION);

	m_ItemContainer.GetSlot(eItemTable_Inventory)->SetItemInfoAll(pItemInfo->Inventory);	
	m_ItemContainer.GetSlot(eItemTable_Weared)->SetItemInfoAll(pItemInfo->WearedItem);

	//----------



	//-----------

	// ����������
	//m_ItemContainer.GetSlot(eItemTable_ShopInven)->SetItemInfoAll(pItemInfo->ShopInventory);
}

//void CPlayer::InitShopInvenInfo(ITEMBASE* pShopWareHouse)
//{
//	m_ItemContainer.GetSlot(eItemTable_ShopInven)->SetItemInfoAll(pShopWareHouse);	
//}

void CPlayer::AddStorageItem(ITEMBASE * pStorageItem)
{
	CStorageSlot * pSlot = (CStorageSlot *)m_ItemContainer.GetSlot(eItemTable_Storage);
	pSlot->InsertItemAbs(this, pStorageItem->Position, pStorageItem);
}

void CPlayer::InitStorageInfo(BYTE Storagenum,MONEYTYPE money)
{
	CStorageSlot * pSlot = (CStorageSlot *)m_ItemContainer.GetSlot(eItemTable_Storage);
	pSlot->SetStorageNum(Storagenum);

	MONEYTYPE maxmoney = 0;
	if(Storagenum)
	{		
		STORAGELISTINFO* pInfo = STORAGEMGR->GetStorageInfo(Storagenum);
		ASSERT(pInfo);
		maxmoney = pInfo ? pInfo->MaxMoney : 10;
	}
	else
	{
		ASSERT(money == 0);
		maxmoney = 0;
	}
	pSlot->CreatePurse(&m_StoragePurse, this, money, maxmoney);	
}

void CPlayer::InitStorageItemInfo(STORAGE_ITEM* pItemInfo)
{
	int rt;
//	char errbuf[128];
	CStorageSlot * pSlot;
	pSlot = (CStorageSlot *)m_ItemContainer.GetSlot(eItemTable_Storage);
	if(m_bStorageInit == FALSE)
	{
		pSlot->SetItemInfoAll(pItemInfo->Storage);
		m_bStorageInit = TRUE;
	}
	else
	{
		for(int i=0; i<SLOT_STORAGE_NUM; ++i )
		{
			if(pItemInfo->Storage[i].dwDBIdx != 0)
			{		
				rt = pSlot->InsertItemAbs(this, pItemInfo->Storage[i].Position, &pItemInfo->Storage[i]);
				if( rt != EI_TRUE)
				{
			//		sprintf(errbuf, "InitStorageItemInfo Error Type: %d, itemDBIdx : %d, CharNO: %u", rt, pItemInfo->Storage[i].dwDBIdx, GetID());
			//		ASSERTMSG(0, errbuf);
				}
			}
		}
	}
}

void CPlayer::InitShopItemInfo(SHOP_ITEM* pItemInfo)
{
	CShopItemSlot* pSlot = (CShopItemSlot*)m_ItemContainer.GetSlot(eItemTable_Shop);
	if(m_bShopInit == FALSE)
	{
		pSlot->SetItemInfoAll(pItemInfo->Item);

		for(int i=0; i<SLOT_SHOPITEM_IMSI; i++)
		{
			if( pItemInfo->Item[i].dwDBIdx != 0 )			
				pSlot->SetLock(pItemInfo->Item[i].Position, TRUE);

			ITEM_INFO * pItemListInfo = ITEMMGR->GetItemInfo(pItemInfo->Item[i].wIconIdx);

			//SW060328 �����۸� �� ����
/*			if(pItemListInfo)
			{
				if( pItemListInfo->ItemKind == eSHOP_ITEM_PET )
				{
					PET_TOTALINFO* pPetInfo = m_PetManager.GetPetTotalInfo(pItemInfo->Item[i].dwDBIdx, eWithNULL );
					if( NULL == pPetInfo )
					{
						g_pServerSystem->CreateNewPet(this, pItemListInfo->ItemIdx, pItemInfo->Item[i].dwDBIdx,  pItemListInfo->Plus_SkillIdx);
					}
				}
			}*/
		}
		
		m_bShopInit = TRUE;
	}
	else
	{
		// 080621 LYW --- Player : 32�� ���� ����.
		// ������ ���� �������̽����� �������� ����ȭ�鿡 �����ϴ� ����.
		// ����, ������ �� â���� �������� �̵� ��, ������ �� ������ �ٽ� ���� ��, 
		// �Ʒ� ������ �������� ������, ������ ������ �پ���� �ʰ� ������ ������ �ϳ���
		// �и��� ������, ������ ������ ���� ����, 
		// �� ������ ������ ���� ������ �ʱ�ȭ �� ������ ������ �߰��ϵ��� ����.
		for(int count=0; count<SLOT_SHOPITEM_IMSI; count++)
		{
			pSlot->ClearItemBaseAndSlotInfo(TP_SHOPITEM_START+count) ;
		}



		for(int i=0; i<SLOT_SHOPITEM_IMSI; i++)
		{
			if( pItemInfo->Item[i].dwDBIdx != 0 )
			{				
				if( pSlot->InsertItemAbs( this, pItemInfo->Item[i].Position, &pItemInfo->Item[i] ) != EI_TRUE )
				{
					ASSERT(0);
				}
			}
		}
	}
}

void CPlayer::SetStorageNum(BYTE n)
{
	CStorageSlot * pSlot = (CStorageSlot *)m_ItemContainer.GetSlot(eItemTable_Storage);
	pSlot->SetStorageNum(n);
}

BYTE CPlayer::GetStorageNum()
{
	CStorageSlot * pSlot = (CStorageSlot *)m_ItemContainer.GetSlot(eItemTable_Storage);
	return pSlot->GetStorageNum();
}

MONEYTYPE CPlayer::GetMoney(eITEMTABLE tableIdx)
{
	CItemSlot* pSlot = m_ItemContainer.GetSlot(tableIdx);
	return pSlot->GetMoney();
}

void CPlayer::GetCharacterTotalInfo(CHARACTER_TOTALINFO* pRtInfo)
{
	*pRtInfo = m_HeroCharacterInfo;
}


const CHARACTER_TOTALINFO& CPlayer::GetCharacterTotalInfo() const
{
	return m_HeroCharacterInfo;
}


// 080225 LUJ, �⺻ ���� ��ȯ
HERO_TOTALINFO* CPlayer::GetHeroTotalInfo()
{
	return &m_HeroInfo;
}

void CPlayer::GetHeroTotalInfo(HERO_TOTALINFO* pRtInfo)
{
	memcpy(pRtInfo,&m_HeroInfo,sizeof(HERO_TOTALINFO));
}

void CPlayer::GetItemtotalInfo(ITEM_TOTALINFO* pRtInfo,DWORD dwFlag)
{
	if(dwFlag & GETITEM_FLAG_INVENTORY)
	{
		m_ItemContainer.GetSlot(eItemTable_Inventory)->GetItemInfoAll(pRtInfo->Inventory);
		m_ItemContainer.GetSlot(eItemTable_ShopInven)->GetItemInfoAll(pRtInfo->ShopInventory);
	}
	else
	{
		//memset(pRtInfo->Inventory,0,sizeof(ITEMBASE)*SLOT_INVENTORY_NUM);
		memset(pRtInfo->Inventory,0,sizeof(ITEMBASE)*SLOT_MAX_INVENTORY_NUM);
		memset(pRtInfo->ShopInventory,0,sizeof(ITEMBASE)*SLOT_SHOPINVEN_NUM);
	}
	if(dwFlag & GETITEM_FLAG_WEAR)
		m_ItemContainer.GetSlot(eItemTable_Weared)->GetItemInfoAll(pRtInfo->WearedItem);
	else
		memset(pRtInfo->WearedItem,0,sizeof(ITEMBASE)*eWearedItem_Max);

	/*if(dwFlag & GETITEM_FLAG_PETWEAR)
		m_ItemContainer.GetSlot(eItemTable_PetWeared)->GetItemInfoAll(pRtInfo->PetWearedItem);
	else
		memset(pRtInfo->WearedItem,0,sizeof(ITEMBASE)*SLOT_PETWEAR_NUM);*/
	///m_pIconTable[eItemTable_Inventory]->GetItemInfoAll(pRtInfo->Inventory);
	//m_pIconTable[eItemTable_Weared]->GetItemInfoAll(pRtInfo->WearedItem);
}

void CPlayer::GetStorageItemInfo(STORAGE_ITEM * pRtInfo)
{
	m_ItemContainer.GetSlot(eItemTable_Storage)->GetItemInfoAll(pRtInfo->Storage);
}

void CPlayer::SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin)
{
	if( m_dwUserLevel <= eUSERLEVEL_GM && m_HeroCharacterInfo.bVisible == FALSE )
	{
		*pMsgLen = 0;
		return;
	}

	SEND_CHARACTER_TOTALINFO* pMsg = (SEND_CHARACTER_TOTALINFO*)pAddMsg;
	pMsg->AddableInfo.Init();

	pMsg->Category = MP_USERCONN;
	pMsg->Protocol = MP_USERCONN_CHARACTER_ADD;
	pMsg->dwObjectID = dwReceiverID;
	GetSendMoveInfo(&pMsg->MoveInfo,&pMsg->AddableInfo);
	/*�����Ƽ ���� - pMsg->MoveInfo.AbilityKyungGongLevel = GetAbilityStats()->KyunggongLevel;*/
	GetBaseObjectInfo(&pMsg->BaseObjectInfo);
	GetCharacterTotalInfo(&pMsg->TotalInfo);	

	// desc_hseos_�ֹε��01
	// S �ֹε�� �߰� added by hseos 2007.06.06
	/*
	/// 070523 ����, �׽�Ʈ. �ٸ� �÷��̾ ��Ī ������ �ӽ÷� ������.
	{
		pMsg->TotalInfo.DateMatching.bIsValid	=	TRUE;						// �ֹ��� �߱� ���� ����.
		pMsg->TotalInfo.DateMatching.byGender	=	pMsg->TotalInfo.Gender;	// ĳ���� ����?
		pMsg->TotalInfo.DateMatching.dwAge		=	10;							// �ϴ� �ʴ�.
		pMsg->TotalInfo.DateMatching.dwRegion	=	eDATE_MATCHING_REGION_SEOUL;	// �ϴ� ����
		pMsg->TotalInfo.DateMatching.dwGoodFeelingDegree	=	10;					// �׳�..
		pMsg->TotalInfo.DateMatching.pdwBadFeeling[0]	=	eDATE_MATCHING_FEELING_DYEDHAIR;		// ù��° �Ⱦ��ϴ� ��. ������ �Ӹ�.
		pMsg->TotalInfo.DateMatching.pdwBadFeeling[1]	=	eDATE_MATCHING_FEELING_MAKEUP;			// �ι�° �Ⱦ��ϴ� ��. ȭ��.

		pMsg->TotalInfo.DateMatching.pdwGoodFeeling[0]	=	eDATE_MATCHING_FEELING_BLACKHAIR;		// ù��° �����ϴ� ��. ���� �Ӹ�.
		pMsg->TotalInfo.DateMatching.pdwGoodFeeling[1]	=	eDATE_MATCHING_FEELING_FORMALDRESS;		// �ι�° �����ϴ� ��. ����.
		pMsg->TotalInfo.DateMatching.pdwGoodFeeling[2]	=	eDATE_MATCHING_FEELING_CASUALWEAR;		// ����° �����ϴ� ��. ĳ�־� ����.
		strcpy( pMsg->TotalInfo.DateMatching.szIntroduce, _T("���� �ڱ� ^.~"));					// �ڱ�Ұ� �ӽ�.
	}
	*/
	pMsg->TotalInfo.DateMatching = m_DateMatching;
	// E �ֹε�� �߰� added by hseos 2007.06.06

//	memcpy(&pMsg->ShopItemOption, GetShopItemStats(), sizeof(SHOPITEMOPTION));
	pMsg->bLogin = bLogin;

	cStreetStall* pStall = STREETSTALLMGR->FindStreetStall(this);
	if( pStall != NULL)
	{
		char StallTitle[MAX_STREETSTALL_TITLELEN+1];
		GetStreetStallTitle(StallTitle);
		StallTitle[MAX_STREETSTALL_TITLELEN] = 0;
		
		if( pStall->GetStallKind() == eSK_SELL)
			pMsg->AddableInfo.AddInfo(CAddableInfoList::StreetStall,strlen(StallTitle)+1,StallTitle);
		else if( pStall->GetStallKind() == eSK_BUY)
			pMsg->AddableInfo.AddInfo(CAddableInfoList::StreetBuyStall,strlen(StallTitle)+1,StallTitle);
	}

	if( GetBattle()->GetBattleKind() == eBATTLE_KIND_SHOWDOWN )	//����n��o��iA�ϡ�AI ��O����
	{
		COMPRESSEDPOS ShowdownPos;

		BATTLE_INFO_SHOWDOWN info;
		WORD wSize;
		GetBattle()->GetBattleInfo( (char*)&info, &wSize );

		ShowdownPos.Compress(&info.vStgPos);

		pMsg->AddableInfo.AddInfo(CAddableInfoList::ShowdownInfo,sizeof(COMPRESSEDPOS),&ShowdownPos);
	}
	
	int nTeam = 0;//PARTYWARMGR->IsMemberInPartyWar( this );
	if( nTeam != 0 )
	{
		pMsg->AddableInfo.AddInfo(CAddableInfoList::PartyWar, sizeof(nTeam), &nTeam);
	}

	// RaMa - 05.05.27
	if( GetBattle()->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT )
	{
		nTeam = GetBattle()->GetBattleTeamID( this );		
		pMsg->AddableInfo.AddInfo(CAddableInfoList::GTournament, sizeof(nTeam), &nTeam);
	}
	else if( GetBattle()->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
	{
		SEIGEWAR_CHARADDINFO info;
		info.Team = (BYTE)GetBattle()->GetBattleTeamID( this );
		if( SIEGEWARMGR->GetEngraveIdx() == GetID() )
			info.bEngrave = 1;
		else
			info.bEngrave = 0;		

		pMsg->AddableInfo.AddInfo(CAddableInfoList::SiegeWar, sizeof(SEIGEWAR_CHARADDINFO), &info);
	}

	*pMsgLen = pMsg->GetMsgLength();

}

void CPlayer::SendMsg(MSGBASE* pMsg,int MsgLen)
{
	CObject::SendMsg(pMsg, MsgLen);
}

void CPlayer::SetStreetStallTitle(char* title)
{
	SafeStrCpy( m_StreetStallTitle, title, MAX_STREETSTALL_TITLELEN+1 );
}

void CPlayer::GetStreetStallTitle(char* title)
{
	SafeStrCpy( title, m_StreetStallTitle, MAX_STREETSTALL_TITLELEN+1);
}


void CPlayer::DoSetInited()
{
	CHARCALCMGR->Initialize( this );
	//CHARCALCMGR->CalcCharStats( this );

	SetPlayerLevelUpPoint(GetPlayerLevelUpPoint());

	m_dwProgressTime = gCurTime;
}


void CPlayer::CalcState()
{
	//CHARCALCMGR->CalcItemStats(this);
	CHARCALCMGR->CalcCharStats( this );

	SetLife(m_HeroCharacterInfo.Life);
	SetMana(m_HeroInfo.Mana);
}


/*
void CPlayer::AddMoneyAndSendToPlayer(DWORD Money)
{
	DWORD CurM = GetMoney();
	SetMoney(Money + CurM);
	
	MSG_DWORD msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_MONEY;
	msg.dwObjectID = GetID();
	msg.dwData = GetMoney();
	SendMsg(&msg,sizeof(msg));
}
*/
/*
void CPlayer::AddHuntItemAndSendToPlayer(WORD DropItemNum)
{
	CIconTable * pInventory = GetIconTable(eItemTable_Inventory);
	POSTYPE absPosOut;
	MSG_WORD msg;
	WORD ItemIdx = DropItemNum;
	if(!pInventory->GetEmptyCellAbsPosRef(absPosOut)) goto HUNTITEM_FAILED;

	
	ITEMBASE item;
	item.dwDBIdx = 0;
	item.Durability = 0;
	item.Position = absPosOut;
	item.QuickPosition = 0;
	item.wIconIdx = DropItemNum;

	pInventory->InsertItemAbsDB(absPosOut, &item);
	
	//CharacterItemGetHuntIDX(GetID(), ItemIdx, 0, absPosOut);
	return;

HUNTITEM_FAILED:
	msg.Protocol = MP_ITEM_OBTAIN_FAILED_NOTIFY;
	SendMsg(&msg,sizeof(MSG_WORD));
	return;
}


BOOL CPlayer::HuntItemDBResult(DWORD dwPlayerID, ITEMBASE * itemInfo)
{
	MSG_ITEM msg;
	CIconTable * pInventory = GetIconTable(eItemTable_Inventory);
	if(this == NULL) goto HUNTITEM_FAILED;
	pInventory->InsertItemAbsDB(itemInfo->Position, itemInfo);


	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_OBTAIN_SUCCESS_NOTIFY;
	msg.dwObjectID = GetID();
	msg.ItemInfo = *itemInfo;
	SendMsg(&msg,sizeof(msg));

	return TRUE;

HUNTITEM_FAILED:
	MSG_WORD msg2;
	msg2.Category = MP_ITEM;
	msg2.Protocol = MP_ITEM_OBTAIN_FAILED_NOTIFY;
	SendMsg(&msg2,sizeof(MSG_WORD));
	return FALSE;
}
*/
// LBS 03.11.04 flagA���������ˢ� ��o��������C��ic����a��?I ����E����a�����ˡ��������� (CoAc) 0:����E����a, 1:��ic����a, 
// 3:(DB��u��ACI����i, A������OoAI��u�ϡ̡�����c��?�ˢ� ��uE�ˡ���c�ˡ�e��u���� �ˡ�������u��uAo�ˡ�| ��Oc��?iAo ��uE�ˡ�A�ˡ�U.)

void CPlayer::MoneyUpdate( MONEYTYPE money )
{	
	m_HeroInfo.Money = money;

/*
	MSG_DWORD2 msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_MONEY;
	msg.dwData1 = 0;//GetMoney();
	msg.dwData2 = flag;
	SendMsg(&msg,sizeof(msg));
*/
}

/*
void CPlayer::SetMoneyNoMsg(DWORD money)
{	
	m_HeroInfo.Money = money;	

	// DB��?�ˢ� ��u��A��i��IAI������c
	CharacterHeroInfoUpdate(this);
}
*/

/*
void CPlayer::SetMoneyNoMsgNoUpdate(DWORD money)
{
	m_HeroInfo.Money = money;	
}
*/
/*
void CPlayer::ObtainMoney(DWORD addmoney)		//�ˡ���AA������?��I��u���� ��uo��uu;��O�����ˡ��ˡ� E��IAa
{
	m_HeroInfo.Money += addmoney;	

	// DB��?�ˢ� ��u��A��i��IAI������c
	CharacterHeroInfoUpdate(this);

	MSG_DWORD msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_OBTAIN_MONEY;
	msg.dwObjectID = GetID();
	msg.dwData = m_HeroInfo.Money;
	SendMsg(&msg,sizeof(msg));		
}
*/
void CPlayer::SetStrength(DWORD val)
{
	m_HeroInfo.Str = val;

	CHARCALCMGR->CalcCharStats(this);

	// DB��?�ˢ� ��u��A��i��IAI������c
	CharacterHeroInfoUpdate(this);

	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_STR_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}

void CPlayer::SetDexterity(DWORD val)
{
	m_HeroInfo.Dex = val;

	CHARCALCMGR->CalcCharStats(this);

	// DB��?�ˢ� ��u��A��i��IAI������c
	CharacterHeroInfoUpdate(this);

	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_DEX_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}
void CPlayer::SetVitality(DWORD val)
{
	m_HeroInfo.Vit = val;

	// ��iy�ˡ�i����A, ��o�Ϣ���ui����A; �ˡ�U��oA ����e��ie
	CHARCALCMGR->CalcCharStats(this);

	// DB��?�ˢ� ��u��A��i��IAI������c
	CharacterHeroInfoUpdate(this);

	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_VIT_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}
void CPlayer::SetWisdom(DWORD val)
{
	m_HeroInfo.Wis = val;

	// �ϩ���i����A; �ˡ�U��oA ����e��ie
	CHARCALCMGR->CalcCharStats(this);

	// DB��?�ˢ� ��u��A��i��IAI������c
	CharacterHeroInfoUpdate(this);

	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_WIS_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}

// 070411 LYW --- Player : Add function to setting intelligence.
void CPlayer::SetIntelligence( DWORD val )
{
	m_HeroInfo.Int = val ;

	CHARCALCMGR->CalcCharStats(this);

	CharacterHeroInfoUpdate(this);

	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_INT_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}

//-------------- USE ITEM ------------------------------------------------------------
BOOL CPlayer::UseItem(WORD abs_pos)
{
	return TRUE;
}

// AI������IAa�ˡ���c, AaA�Ϩ�A��E, �ˡ�UAaA��E
BOOL CPlayer::UseConsumptionItem(WORD abs_pos)
{
	// ��uO�ˡ��ϡ̡�u���� ��u����AIAU

	return TRUE;
}

BOOL CPlayer::UseEquipItem(WORD abs_pos)
{
	// ��u����AIAU

	return TRUE;
}

BOOL CPlayer::UseSkillItem(WORD abs_pos)
{
	// ��u����AIAU

	return TRUE;
}


CItemSlot * CPlayer::GetSlot(POSTYPE absPos)
{
	// if( TP_INVENTORY_START <= absPos && absPos < TP_STORAGE_END )
	if( TP_INVENTORY_START <= absPos && absPos < TP_SHOPITEM_END )
	{
		// 071210 KTH -- Ȯ�� �κ��丮�� ũ�⸦ ������ ������ üũ�Ѵ�.
		/*WORD wExpansionSize =  GetInventoryExpansion() * TABCELL_INVENTORY_NUM;

		if( absPos >= TP_EXTENDED_INVENTORY1_START + wExpansionSize && absPos < TP_EXTENDED_INVENTORY2_END )
			return NULL;*/

		return m_ItemContainer.GetSlot(absPos);
	}
	else if( TP_GUILDWAREHOUSE_START <= absPos && absPos < TP_GUILDWAREHOUSE_END )
	{
		return (CItemSlot*)GUILDMGR->GetSlot( GetGuildIdx());
	}
	else if( TP_SHOPITEM_START <= absPos && absPos < TP_SHOPITEM_END )
	{
		return m_ItemContainer.GetSlot(absPos);
	}
	else if( TP_SHOPINVEN_START <= absPos && absPos < TP_SHOPINVEN_END )
	{
		return m_ItemContainer.GetSlot(absPos);
	}
	return NULL;
}

CItemSlot * CPlayer::GetSlot(eITEMTABLE tableIdx)
{
	if(tableIdx < eItemTable_TableMax)
		return m_ItemContainer.GetSlot(tableIdx);
	else if( tableIdx == eItemTable_GuildWarehouse)
	{
		return (CItemSlot*)GUILDMGR->GetSlot(GetGuildIdx());
	}
	return NULL;
}

// ��o��i�����Ϩ���u����AA�ˡ��ˡ� ��?E
/*
BOOL CPlayer::UseQuickItem(WORD rel_pos)
{
	ASSERT(rel_pos < MAX_QUICKITEMPERSHEET_NUM*MAX_QUICKSHEET_NUM);
	SetQuickSelectIdx(rel_pos);
	QUICKITEM * pQuickItem = &m_QuickItem[m_quick_info.cur_selected_page][m_quick_info.cur_selected_rel_idx];
	ASSERT(pQuickItem != NULL);

	

	return TRUE;
}
void CPlayer::SetQuickSelectIdx(WORD idx) 
{ 
	if(idx >= MAX_QUICKITEMPERSHEET_NUM*MAX_QUICKSHEET_NUM) return;

	if(idx == 5)
	{
		//KEY_6 for test
	//	SetPlayerLevel(GetPlayerLevel()+1);
	}

	m_quick_info.cur_selected_page = idx/MAX_QUICKITEMPERSHEET_NUM;
	m_quick_info.cur_selected_rel_idx = idx%MAX_QUICKITEMPERSHEET_NUM;
	//m_quick_info.cur_selected_abs_idx = 
}
*/
//---------------------- ��uoA�ˢ�  ---------------------------------------------------





//-------------------------------------------------------------------------------------------------
//	NAME : SetLifeForce
//	DESC : 080625 LYW 
//		   ĳ���Ͱ� ���� ���¿��� �����̳� ƨ�� ������ �߻� �� ��, 
//		   ĳ������ ����� 30%�� ������ �־�� �Ѵ�. �׷��� ���°� ���� ���¸�, 
//		   ���� �Լ��� return ó���� �ϹǷ�, ������ ���� ���θ� ������ �� �ִ� �Լ��� �߰��Ѵ�.
//-------------------------------------------------------------------------------------------------
void CPlayer::SetLifeForce(DWORD Life, BYTE byForce, BOOL bSendMsg) 
{
	// ���� ���� ���θ� Ȯ���Ѵ�.
	if(byForce == FALSE)
	{
		// ĳ���Ͱ� ���� ���¶��, return ó���� �Ѵ�.
		if(GetState() == eObjectState_Die) return ;
	}


	// ���� ĳ���� ������ �´� �ִ� ������� �޴´�.
	DWORD maxlife = 0 ;
	maxlife = GetMaxLife() ;


	// ����� ��ġ ��ȿ üũ.
	if(Life > maxlife) Life = maxlife ;


	// ���ڷ� �Ѿ�� �������� ���� ������ ���� ������, return ó���� �Ѵ�.
	if( m_HeroCharacterInfo.Life >= Life ) return ;

	
	// ���� ����� / ���ڷ� �Ѿ�� ������� ���� ������,
	if(m_HeroCharacterInfo.Life != Life)
	{
		// �޽��� ���� �ϴ� ��Ȳ�̶��,
		if(bSendMsg == TRUE)
		{
			// ���ο� ������� ����Ѵ�.
			DWORD dwNewLife = 0 ;
			dwNewLife = Life - GetLife() ;

			// �޽����� �����Ѵ�.
			MSG_INT msg ;
			msg.Category = MP_CHAR ;
			msg.Protocol = MP_CHAR_LIFE_ACK ;
			msg.dwObjectID = GetID() ;
			msg.nData = dwNewLife ;
			SendMsg(&msg,sizeof(msg)) ;
		}
		
		// ��Ƽ���̶��,
		if(GetPartyIdx())
		{
			// ��Ƽ������ ����� ��ȭ�� �����Ѵ�.
			SendPlayerLifeToParty(Life, GetPartyIdx()) ;
		}
	}
		

	// ĳ������ ������� �����Ѵ�.
	m_HeroCharacterInfo.Life = Life ;
}





void CPlayer::SetLife(DWORD val,BOOL bSendMsg) 
{
	if(GetState() == eObjectState_Die)
		return;

	DWORD maxlife = GetMaxLife();
	if(val > maxlife)
		val = maxlife;
	
	if(m_HeroCharacterInfo.Life != val)	// ��i��I��oIAo AI�����Ϣ���?i��?�ˢ�ˡ�A ��?����Aa��?�ˢ硧u���� ��i��IA�����ˡ�U.
	{
		if(bSendMsg == TRUE)
		{
			// To A������OoAI��u�ϡ̡�����c -------------------------------
			MSG_INT msg;
			msg.Category = MP_CHAR;
			msg.Protocol = MP_CHAR_LIFE_ACK;
			msg.dwObjectID = GetID();
			// 070419 LYW --- Player : Modified function SetLife.
			msg.nData = val - GetLife();
			//msg.nData = val ;
			SendMsg(&msg,sizeof(msg));
		}
		
		if(GetPartyIdx())
			SendPlayerLifeToParty(val, GetPartyIdx());
	}
		
	m_HeroCharacterInfo.Life = val;
}

void CPlayer::SendPlayerLifeToParty(DWORD val, DWORD PartyID)
{	
	CParty* pParty = PARTYMGR->GetParty(PartyID);
	if(pParty == NULL)
		return;
	
	SEND_PARTY_GAUGE msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_MEMBERLIFE;
	msg.PlayerID = GetID();
	if( GetMaxLife() )
		msg.GaugePercent = (float)(val*100/GetMaxLife());
	else
	{
		ASSERT(0);
		msg.GaugePercent = 0;
	}
//	pParty->SendMsgExceptOne(&msg, sizeof(msg), msg.dwData1);
	pParty->SendMsgExceptOneinChannel(&msg, sizeof(msg), msg.PlayerID, GetGridID());
}





//-------------------------------------------------------------------------------------------------
//	NAME : SetLifeForce
//	DESC : 080625 LYW 
//		   ĳ���Ͱ� ���� ���¿��� �����̳� ƨ�� ������ �߻� �� ��, 
//		   ĳ������ ���� 30%�� ������ �־�� �Ѵ�. �׷��� ���°� ���� ���¸�, 
//		   ���� �Լ��� return ó���� �ϹǷ�, ������ ���� ���θ� ������ �� �ִ� �Լ��� �߰��Ѵ�.
//-------------------------------------------------------------------------------------------------
void CPlayer::SetManaForce(DWORD Mana, BYTE byForce, BOOL bSendMsg) 
{
	// ���� ���� ���θ� Ȯ���Ѵ�.
	if(byForce == FALSE)
	{
		// ĳ���Ͱ� ���� ���¶��, return ó���� �Ѵ�.
		if(GetState() == eObjectState_Die) return ;
	}


	// ĳ������ ���� ������ �ִ� ���� ��ġ�� �޴´�.
	DWORD MaxMana = 0 ;
	MaxMana = GetMaxMana() ;


	// ���ڷ� �Ѿ�� ������ ��ȿ ������ üũ.
	if(Mana > MaxMana) Mana = MaxMana ;


	// ���ڷ� �Ѿ�� �������� ���� ������ ���� ������, return ó���� �Ѵ�.
	if( m_HeroInfo.Mana >= Mana ) return ;


	// �������� / ���ڷ� �Ѿ�� ������ ���� ������,
	if( m_HeroInfo.Mana != Mana)
	{
		// �޽��� ���� ���ΰ� TRUE �̸�,
		if(bSendMsg)
		{
			// ������ �����Ѵ�.
			MSG_DWORD msg ;
			msg.Category = MP_CHAR ;
			msg.Protocol = MP_CHAR_MANA_ACK ;
			msg.dwObjectID = GetID() ;
			msg.dwData = Mana ;
			SendMsg(&msg,sizeof(msg)) ;
		}
		
		// ��Ƽ���̶��,
		if(m_HeroInfo.PartyID)
		{
			// ���� ��ȭ���� �����Ѵ�.
			SendPlayerManaToParty(Mana, m_HeroInfo.PartyID) ;
		}
	}
	

	// ĳ������ ������ �����Ѵ�.
	m_HeroInfo.Mana = Mana ; 
}





void CPlayer::SetMana(DWORD val,BOOL bSendMsg)
{ 
	if(GetState() == eObjectState_Die)
		return;

	DWORD MaxMana = GetMaxMana();
	if(val > MaxMana)
		val = MaxMana;

	// To A������OoAI��u�ϡ̡�����c -------------------------------
	if( m_HeroInfo.Mana != val)
	{
		if(bSendMsg)
		{
			MSG_DWORD msg;
			msg.Category = MP_CHAR;
			msg.Protocol = MP_CHAR_MANA_ACK;
			msg.dwObjectID = GetID();
			msg.dwData = val;
			SendMsg(&msg,sizeof(msg));
		}
		
		if(m_HeroInfo.PartyID)
			SendPlayerManaToParty(val, m_HeroInfo.PartyID);
	}
	
	m_HeroInfo.Mana = val; 
}

void CPlayer::SendPlayerManaToParty(DWORD val, DWORD PartyID)
{
	CParty* pParty = PARTYMGR->GetParty(PartyID);
	if(pParty == NULL)
		return;
	
	SEND_PARTY_GAUGE msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_MEMBERMANA;
	msg.PlayerID = GetID();
	if( GetMaxMana() )
		msg.GaugePercent = (float)((val*100)/GetMaxMana());
	else
	{
		ASSERT(0);
		msg.GaugePercent = 0;
	}

//	pParty->SendMsgExceptOne(&msg, sizeof(msg), msg.dwData1);
	pParty->SendMsgExceptOneinChannel(&msg, sizeof(msg), msg.PlayerID, GetGridID());
}

void CPlayer::SendMoveInfotoParty()
{
/*	CParty* pParty = PARTYMGR->GetParty(PartyID);
	if(pParty == NULL)
		return;

	SEND_PARTYICON_MOVEINFO msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_SENDPOS;
*/
}

void CPlayer::SetMaxLife(DWORD val)
{
	m_HeroCharacterInfo.MaxLife = val;

	// To A������OoAI��u�ϡ̡�����c -------------------------------
	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_MAXLIFE_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}

void CPlayer::SetMaxMana(DWORD val)
{
	m_HeroInfo.MaxMana= val;

	// To A������OoAI��u�ϡ̡�����c -------------------------------
	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_MAXMANA_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}
void CPlayer::AddStrength(int val)
{
	SetStrength(m_HeroInfo.Str+val);
}
void CPlayer::AddWisdom(int val)
{
	SetWisdom(m_HeroInfo.Wis+val);
}
void CPlayer::AddDexterity(int val)
{
	SetDexterity(m_HeroInfo.Dex+val);
}
void CPlayer::AddVitality(int val)
{
	SetVitality(m_HeroInfo.Vit+val);
}
// 070411 LYW --- Player : Add function to setting intelligence.
void CPlayer::AddIntelligence( int val )
{
	SetIntelligence( m_HeroInfo.Int+ val ) ;
}
void CPlayer::SetPlayerLevelUpPoint(LEVELTYPE val) 
{ 
	m_HeroInfo.LevelUpPoint=val;
	
	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_LEVELUPPOINT_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}
/*****************************************************************/
/* 1. AE����a��?�ˢ� A�ϩ��ˡ�?AI�����ˢ� ��iy��u������iC��uu;��O���� from DBResultQuery
/* 2. SetPlayerExpPoint()��?�ˢ硧u���� AO�ˡ�e �����Ϣ�CeA�ˢ�ˡ�| �ϩ�N��ui ��u��o; ��O���� call��iE
/*
/*
/*****************************************************************/

void CPlayer::SetLevel(LEVELTYPE level)
{
	if(level >= MAX_PLAYERLEVEL_NUM)
	{
		ASSERT(0);
		return;
	}

	if(m_HeroCharacterInfo.Level == level) return;

	if(level > m_HeroCharacterInfo.Level)
	{
		if(m_HeroInfo.MaxLevel < level)
		{
			m_HeroInfo.MaxLevel = level;
			/////////////////////////////////////////////////////////////
			// 06. 06. 2�� ���� - �̿���
			// 70�̻� �������� ȹ�� ���� ����
			// 070411 LYW --- Player : Modified stats point.
			/*
			if( level > 70 )
				SetPlayerLevelUpPoint(GetPlayerLevelUpPoint()+8);
			else
				SetPlayerLevelUpPoint(GetPlayerLevelUpPoint()+6);
			*/
//			if( (level%5) == 0 )
			{
				SetPlayerLevelUpPoint(GetPlayerLevelUpPoint()+1);
			}
			/////////////////////////////////////////////////////////////

			DWORD skillpoint = (DWORD)(level / 4) + 1;

			// 080611 LYW --- Player : ��ų����Ʈ ������Ʈ ó���� ������.
			// (���������� ��ų ����Ʈ�� �߰��ϴ� ����� ����� ����.)
			//SetSkillPoint( skillpoint );
			SetSkillPoint( skillpoint, FALSE ) ;

			// �������� ����
			// GMEvent01( GetUserID(), GetID(), m_HeroInfo.MaxLevel, g_nServerSetNum );
			//

			//SW060719 ��������Ʈ	//���Ŀ� ������ ���� ����.
			//GUILDMGR->GuildMemberLevelUpToGuildPoint(this);
		}
		
		m_HeroCharacterInfo.Level = level;
		
		//CHARCALCMGR->CalcItemStats(this);
		CHARCALCMGR->CalcCharStats( this );

		SetLife(GetMaxLife());
		SetMana(GetMaxMana());
		SetMussangMaxPoint();
		
		// Journal DB�� �߰�
		JournalInsertLevel(GetID(), eJournal_Levelup, level);

		// Client�� �˸�
		SEND_JOURNALINFO msg;
		msg.Category = MP_JOURNAL;
		msg.Protocol = MP_JOURNAL_ADD;
		msg.dwObjectID = GetID();
		msg.m_Info.Param = GetLevel();
		msg.m_Info.Kind = eJournal_Levelup;
		SendMsg(&msg, sizeof(msg));


		MSG_LEVEL msg1;
		msg1.Category = MP_CHAR;
		msg1.Protocol = MP_CHAR_LEVEL_NOTIFY;
		msg1.dwObjectID = GetID();
		msg1.Level = GetLevel();
		msg1.MaxExpPoint = GAMERESRCMNGR->GetMaxExpPoint(level);
		msg1.CurExpPoint = GetPlayerExpPoint();
		SendMsg(&msg1,sizeof(msg1));

		MSG_DWORD msg2;
		msg2.Category = MP_CHAR;
		msg2.Protocol = MP_CHAR_PLAYERLEVELUP_NOTIFY;
		msg2.dwObjectID = GetID();
		msg2.dwData = GetID();
		PACKEDDATA_OBJ->QuickSendExceptObjectSelf(this,&msg2,sizeof(msg2));	


		QUESTMGR->AddQuestEvent( this, &CQuestEvent( eQuestEvent_Level, level, 1 ) );
// jsd quest
// ��?��I����a��u���� levelup event��o�ϡ���iy
//	QUESTEVENT qe;
//	qe.Init( (void*)this, eQuestEvent_LevelUp, m_HeroCharacterInfo.Level, 0 );
//	QUESTEVENTMGR->AddQuestEvent( qe );
//
	}
	else
	{
		// 070411 LYW --- Player : Modified level down part.
		/*
		m_HeroCharacterInfo.Level = level;

		CHARCALCMGR->CalcItemStats(this);

		MSG_LEVEL msg1;
		msg1.Category = MP_CHAR;
		msg1.Protocol = MP_CHAR_LEVEL_NOTIFY;
		msg1.dwObjectID = GetID();
		msg1.Level = GetLevel();
		msg1.MaxExpPoint = GAMERESRCMNGR->GetMaxExpPoint(level);
		msg1.CurExpPoint = GetPlayerExpPoint();
		SendMsg(&msg1,sizeof(msg1));
		*/

		m_HeroCharacterInfo.Level = level;
		
		//CHARCALCMGR->CalcItemStats(this);
		CHARCALCMGR->CalcCharStats( this );		

//		SetLife(GetMaxLife());
//		SetMana(GetMaxMana());
//		SetMussangMaxPoint();
		
		// Journal DB�� �߰�
		JournalInsertLevel(GetID(), eJournal_Levelup, level);

		// Client�� �˸�
		SEND_JOURNALINFO msg;
		msg.Category = MP_JOURNAL;
		msg.Protocol = MP_JOURNAL_ADD;
		msg.dwObjectID = GetID();
		msg.m_Info.Param = GetLevel();
		msg.m_Info.Kind = eJournal_Levelup;
		SendMsg(&msg, sizeof(msg));


		MSG_LEVEL msg1;
		msg1.Category = MP_CHAR;
		msg1.Protocol = MP_CHAR_LEVEL_NOTIFY;
		msg1.dwObjectID = GetID();
		msg1.Level = GetLevel();
		msg1.MaxExpPoint = GAMERESRCMNGR->GetMaxExpPoint(level);
		msg1.CurExpPoint = GetPlayerExpPoint();
		SendMsg(&msg1,sizeof(msg1));

		MSG_DWORD msg2;
		msg2.Category = MP_CHAR;
		msg2.Protocol = MP_CHAR_PLAYERLEVELUP_NOTIFY;
		msg2.dwObjectID = GetID();
		msg2.dwData = GetID();
		PACKEDDATA_OBJ->QuickSendExceptObjectSelf(this,&msg2,sizeof(msg2));	


		QUESTMGR->AddQuestEvent( this, &CQuestEvent( eQuestEvent_Level, level, 1 ) );
	}
	
	/*�����Ƽ ���� - ABILITYMGR->UpdateAbilityState(ABILITYUPDATE_CHARACTERLEVEL_CHANGED,GetLevel(),GetAbilityGroup());*/

	if(GetPartyIdx())
		PARTYMGR->MemberLevelUp(GetPartyIdx(), GetID(), GetLevel());
	if(GetGuildIdx())
		GUILDMGR->MemberLevelUp(GetGuildIdx(), GetID(), GetLevel());

	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.07.15
	// ..���� ��ȭ �� �йи� ������� �˸���
	g_csFamilyManager.SRV_UpdateMapPlayerInfo(this);
	// E �йи� �߰� added by hseos 2007.07.15
}

// 080611 LYW --- Player : ��ų����Ʈ ������Ʈ ó���� ������.
// (���������� ��ų ����Ʈ�� �߰��ϴ� ����� ����� ����.)
//void CPlayer::SetSkillPoint( DWORD point )
void CPlayer::SetSkillPoint( DWORD point, BYTE byForced )
{
	// ���� ������Ʈ ���� üũ.
	ASSERT(byForced <= TRUE) ;
	if(byForced > TRUE) return ;


	// ��ų ����Ʈ ������Ʈ.
	GetHeroTotalInfo()->SkillPoint += point;


	// Ŭ���̾�Ʈ ����
	MSG_DWORD msg;

	msg.Category = MP_SKILLTREE;
	msg.Protocol = MP_SKILLTREE_POINT_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = point;

	SendMsg(&msg, sizeof(msg));


	// DB ������Ʈ
	SkillPointUpdate( GetID(), GetSkillPoint() );


	// 071129 LYW --- Player : ���� ��ų ����Ʈ ������Ʈ.
	//DB_UpdateAccumulateSkillPoint(GetID(), FALSE, point) ;
	DB_UpdateAccumulateSkillPoint(GetID(), byForced, point) ;


	// 071114 ����. �α�
	{
		const SKILL_BASE emptyData = { 0 };

		InsertLogSkill( this, &emptyData, eLog_SkillGetPoint );
	}
}

DWORD CPlayer::GetSkillPoint()
{
	return GetHeroTotalInfo()->SkillPoint;
}

void CPlayer::SetPlayerExpPoint( EXPTYPE point, BYTE bDie )													// �÷��̾� ����ġ�� ó���� �ִ� �Լ�.
{
	// 071119 ����, �ѹ��� ���� �ܰ踦 �������� �� �ֵ��� �����ϰ� �ڵ带 �ܼ�ȭ��

	const LEVELTYPE& level = m_HeroCharacterInfo.Level;

	ASSERT( level <= MAX_CHARACTER_LEVEL_NUM );

	if( level == MAX_CHARACTER_LEVEL_NUM )
	{
		return;
	}	
	
	// ����ġ�� ���� �ܰ迡�� �䱸�ϴ� �ͺ��� �ξ� ���� �� �����Ƿ�,
	// ��� üũ�ؼ� ����������
	{
		EXPTYPE nextPoint = GAMERESRCMNGR->GetMaxExpPoint( level );

		while( point >= nextPoint )
		{
			SetLevel( level + 1 );

			CharacterHeroInfoUpdate( this );
			CharacterTotalInfoUpdate( this );

			InsertLogCharacter( GetID(), m_HeroCharacterInfo.Level, &m_HeroInfo );
			InsertLogExp( eExpLog_LevelUp, GetID(), level, 0, GetPlayerExpPoint(), 0, 0, 0 );

			CObjectEvent::ObjectEvent( this, OE_LEVELUP, &m_HeroCharacterInfo.Level );

			point		-=	nextPoint;
			nextPoint	=	GAMERESRCMNGR->GetMaxExpPoint( level );
		}

		m_HeroInfo.ExpPoint = point;
	}

	{
		MSG_EXPPOINT message;
		message.Category	= MP_CHAR;
		message.Protocol	= MP_CHAR_EXPPOINT_ACK;
		message.ExpPoint	= point;

		SendMsg( &message, sizeof( message ) );
	}

	/*
	// �����ڵ�: �ѹ��� �������� 2�ܰ��̻� �Ҷ� ������ ���������� �־����� �ʴ� ���� ����

	EXPTYPE pointForLevel = GAMERESRCMNGR->GetMaxExpPoint(GetLevel());										// �÷��̾��� ���� �������� �������� �ϱ� ���� ��ǥ ����ġ.

	int levelCount = 0 ;

	while(TRUE)
	{
		if(point >= pointForLevel)																				// ���ڷ� �Ѿ�� ����ġ�� ��ǥ ����ġ���� ũ�ų� ������,
		{
			levelCount++;

			if( levelCount >= 10 )
			{
				break ;
			}

			point -= pointForLevel;
			pointForLevel = GAMERESRCMNGR->GetMaxExpPoint(GetLevel()+levelCount);							// ���� ������ ��ǥ ����ġ�� ���Ѵ�.

			SetLevel(m_HeroCharacterInfo.Level+levelCount);												// �÷��̾��� ���� + ���� ī��Ʈ�� ������ �������ش�.

			CharacterHeroInfoUpdate(this);																// �÷��̾��� hero������ ������Ʈ�Ѵ�.
			CharacterTotalInfoUpdate(this);																// �÷��̾��� ��� ������ ������Ʈ�Ѵ�.

			// character info log
			InsertLogCharacter( GetID(), m_HeroCharacterInfo.Level, &m_HeroInfo );

			//LEVEL downA�� reduceexppoint�������� ������e
			InsertLogExp( eExpLog_LevelUp, GetID(), GetLevel(), 0, GetPlayerExpPoint(), 0, 0, 0);
			// Object AI������I������c��?�ˢ� �ˡ�eCN A��uA��I�ˡ�| CN�ˡ�U.
			CObjectEvent::ObjectEvent(this, OE_LEVELUP, &m_HeroCharacterInfo.Level);


			continue;
		}
		else
		{
			m_HeroInfo.ExpPoint = point;																// �÷��̾��� ����ġ�� ���� ����ġ�� �������ش�.

			MSG_EXPPOINT msg;
			msg.Category = MP_CHAR;
			msg.Protocol = MP_CHAR_EXPPOINT_ACK;
			msg.dwObjectID = GetID();
			msg.ExpPoint = m_HeroInfo.ExpPoint;
			SendMsg(&msg,sizeof(msg));

			break ;
		}
	}
}

// 070411 LYW --- Player : Add function to set experience.
void CPlayer::SetPlayerExpPointDown(EXPTYPE point,BYTE bDie) 
{
	/*
	ASSERT(GetLevel() <= MAX_CHARACTER_LEVEL_NUM);
	if(GetLevel() == MAX_CHARACTER_LEVEL_NUM)
		return;
	// ������o����������u��A ������AAI������c AIAo A��uA��I!!!
	EXPTYPE pointForLevel = GAMERESRCMNGR->GetMaxExpPoint(GetLevel());
	if(point >= pointForLevel)
	{
		m_HeroInfo.ExpPoint = point - pointForLevel;
		SetLevel(m_HeroCharacterInfo.Level-1);
		// YH
		CharacterHeroInfoUpdate(this);
		CharacterTotalInfoUpdate(this);
		
		// character info log
		InsertLogCharacter( GetID(), m_HeroCharacterInfo.Level, &m_HeroInfo );
		//LEVEL downA�� reduceexppoint�������� ������e
		InsertLogExp( eExpLog_LevelUp, GetID(), GetLevel(), 0, GetPlayerExpPoint(), 0, 0, 0
		�����Ƽ ���� - GetPlayerAbilityExpPoint()
	);

		// Object AI������I������c��?�ˢ� �ˡ�eCN A��uA��I�ˡ�| CN�ˡ�U.
		CObjectEvent::ObjectEvent(this, OE_LEVELUP, &m_HeroCharacterInfo.Level);
	}
	else
	{
		m_HeroInfo.ExpPoint = point; 

		// to client -------------------------------------
		MSG_EXPPOINT msg;
		msg.Category = MP_CHAR;
		msg.Protocol = MP_CHAR_EXPPOINT_ACK;
		msg.dwObjectID = GetID();
		msg.ExpPoint = m_HeroInfo.ExpPoint;
		if(bDie == 0)
			msg.ExpKind = MSG_EXPPOINT::EXPPOINTKIND_ACQUIRE;
		else if(bDie == 1)
			msg.ExpKind = MSG_EXPPOINT::EXPPOINTKIND_DIE;

		SendMsg(&msg,sizeof(msg));
	}
	*/
}
void CPlayer::AddPlayerExpPoint(EXPTYPE Exp)
{
	if( Exp == 0 ) return;
	// 071204 KTH -- ����ġ Buff�� ���� ����ġ ����� ����
//	EXPTYPE exp = 0;
//	exp = EXPTYPE(Exp * (GetRateBuffStatus()->GetExp/100));

	EXPTYPE NewExp = GetPlayerExpPoint() + Exp;

	SetPlayerExpPoint(NewExp);

	// 080602 LYW --- Player : ����ġ ��ġ (__int32) ���� (__int64) ������� ���� ó��.
	//MSG_DWORD2 msg ;
	//
	//msg.Category	= MP_USERCONN ;
	//msg.Protocol	= MP_USERCONN_CHARACTER_APPLYEXP_NOTICE ;
	//msg.dwObjectID	= GetID() ;
	//msg.dwData1		= GetPlayerExpPoint() ;
	//msg.dwData2		= Exp;

	MSG_DWORDEX2 msg ;

	msg.Category	= MP_USERCONN ;
	msg.Protocol	= MP_USERCONN_CHARACTER_APPLYEXP_NOTICE ;
	msg.dwObjectID	= GetID() ;
	msg.dweData1		= GetPlayerExpPoint() ;
	msg.dweData2		= Exp;

	SendMsg(&msg, sizeof(msg)) ;
}

void CPlayer::ReduceExpPoint(EXPTYPE minusExp, BYTE bLogType)
{
	LEVELTYPE minuslevel = 0;

	// 080602 LYW --- Player : ����ġ ��ġ (__int32) ���� (__int64) ������� ���� ó��.
	//DWORD CurExp = GetPlayerExpPoint();
	EXPTYPE CurExp = GetPlayerExpPoint();

	if(GetLevel() <= 1 && CurExp < minusExp)	//��������1A�� 0��iAo���� ��iAI��U.
		minusExp = CurExp;

	InsertLogExp( bLogType, GetID(), GetLevel(), minusExp, GetPlayerExpPoint(), m_MurdererKind, m_MurdererIDX, 0/*�����Ƽ ���� - GetPlayerAbilityExpPoint()*/);
	
	while(1)
	{
		if(CurExp < minusExp)
		{
			minusExp -= CurExp;
			++minuslevel;
			CurExp = GAMERESRCMNGR->GetMaxExpPoint(GetLevel()-minuslevel) - 1;
			ASSERT(minuslevel<2);	//E����A���� C������
			if(minuslevel > 3)		//E����A���� CI��A... ����CN��cCA ����Ao��e
				break;
		}
		else
		{
			CurExp -= minusExp;
			break;
		}
	}

	if(minuslevel > 0)
	{
		SetLevel(m_HeroCharacterInfo.Level-minuslevel);

		// character info log
		InsertLogCharacter( GetID(), m_HeroCharacterInfo.Level, &m_HeroInfo );
	}
	SetPlayerExpPoint(CurExp,TRUE);	

	CharacterLvlExpUpdate(GetID(), GetLevel(), GetPlayerExpPoint());
}
/*�����Ƽ ���� - 
void CPlayer::SetPlayerAbilityExpPoint(EXPTYPE point,BOOL bUse) 
{
	// 2005 ũ�������� �̺�ũ �ڵ�
	// �� �������� ����ġ 10 ����
	if( WEATHERMGR->GetWeatherState() == eWS_Snow )
		point += 10;

	DWORD FromAbilityExp = m_AbilityGroup.GetAbilityExp();
	DWORD ChangeAbilityExp = point - FromAbilityExp;
	if((point > 1000000000 || ChangeAbilityExp > 1000000000) && GetUserLevel() > eUSERLEVEL_GM )
	{
		AbilityLevelupLog(this,
			0,99,
			FromAbilityExp,point);
		
		return;
	}
	
	m_AbilityGroup.SetAbilityExp(point);

	// to client -------------------------------------
	MSG_EXPPOINT msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_ABILITYEXPPOINT_ACK;
	msg.dwObjectID = GetID();
	msg.ExpPoint = GetPlayerAbilityExpPoint();
	if(bUse == 0)
		msg.ExpKind = MSG_EXPPOINT::EXPPOINTKIND_ACQUIRE;
	else
		msg.ExpKind = MSG_EXPPOINT::EXPPOINTKIND_ABILITYUSE;

	SendMsg(&msg,sizeof(msg));
}
void CPlayer::AddAbilityExp(DWORD AbilityExp)
{
	EXPTYPE NewExp = GetPlayerAbilityExpPoint() + AbilityExp;

	SetPlayerAbilityExpPoint(NewExp);
}
*/
int CPlayer::GetSkillLevel( DWORD SkillIdx )
{
	SKILL_BASE* pskill = m_SkillTree.GetSkillBase( SkillIdx );

	if( pskill )
	{
		return pskill->Level;
	}
	else
	{
		return 0;
	}
}

void CPlayer::SetPlayerSkillLevel(DWORD SkillIdx, BYTE level, WORD Reason)
{
	SKILL_BASE* pSkill = m_SkillTree.GetSkillBase(SkillIdx);
	if(pSkill == NULL)
		return;

	if(pSkill->dwDBIdx == 0 || pSkill->Level == 0)
		return;
	
	cSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(pSkill->wSkillIdx);
	if(pSkillInfo == NULL)
		return;

	pSkill->Level = level;

	MSG_WORD3 msg;
	msg.Category = MP_SKILLTREE;
	msg.Protocol = MP_SKILLTREE_LEVEL_NOTIFY;
	msg.dwObjectID = GetID();
	msg.wData1 = (WORD)SkillIdx;
	msg.wData2 = level;
	msg.wData3 = Reason;
	SendMsg(&msg,sizeof(msg));

	MSGBASE msg1;
	SetProtocol( &msg1, MP_SKILLTREE, MP_SKILLTREE_LEVEL_LEVELUP );
	msg1.dwObjectID = GetID();
	PACKEDDATA_OBJ->QuickSendExceptObjectSelf( this, &msg1, sizeof(msg1) );

	SkillUpdateToDB(pSkill, GetID());

	//InsertLogSkill( Reason, GetID(), pSkill->wSkillIdx, pSkill->dwDBIdx, level, 0 );
	InsertLogSkill( this, pSkill, Reason );

//	CharacterSkillUpdate(pSkill);
}

void CPlayer::SkillLevelDown(DWORD SkillIdx, WORD Reason)
{
	SKILL_BASE* pSkill = m_SkillTree.GetSkillBase(SkillIdx);
	if(pSkill == NULL)
		return;

	if(pSkill->dwDBIdx == 0 || pSkill->Level == 0)
		return;
	
	cSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(pSkill->wSkillIdx);
	if(pSkillInfo == NULL)
		return;
	
	BYTE newLevel = pSkill->Level - 1;
	SetPlayerSkillLevel(pSkill->wSkillIdx,newLevel,Reason);
}

BYTE CPlayer::GetLifePercent()
{
	BYTE lifepercent = (BYTE)((GetLife() / (float)GetMaxLife()) * 100);
	return lifepercent;
}

BYTE CPlayer::GetManaPercent()
{
	BYTE manapercent = (BYTE)((m_HeroInfo.Mana / (float)GetMaxMana()) * 100);
	return manapercent;
}
void CPlayer::OnStartObjectState(BYTE State,DWORD dwParam)
{
	switch(State)
	{
	case eObjectState_Ungijosik:
		m_LifeRecoverTime.lastCheckTime = gCurTime;
		m_ManaRecoverTime.lastCheckTime = gCurTime;
		break;
	}
}

void CPlayer::OnEndObjectState(BYTE State)
{
	switch(State)
	{
	case eObjectState_Die:
		GetBattle()->OnTeamMemberRevive(GetBattleTeam(),this);
		m_bNeedRevive = TRUE;
		break;
	}

}

WORD CPlayer::GetWeaponEquipType()
{
	DWORD WeaponItemIdx = GetWearedWeapon();
	if(WeaponItemIdx == 0)
		return 0;
	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(WeaponItemIdx);
	ASSERT(pItemInfo);
	return pItemInfo->WeaponType;
}

// 080703 LUJ, ��ȯ Ÿ���� enum���� ����
eWeaponAnimationType CPlayer::GetWeaponAniType()
{
	const ITEM_INFO* leftInfo	= ITEMMGR->GetItemInfo( GetWearedItemIdx( eWearedItem_Weapon ) );
	const ITEM_INFO* rightInfo	= ITEMMGR->GetItemInfo( GetWearedItemIdx( eWearedItem_Shield ) );

	const eWeaponAnimationType	leftType	= eWeaponAnimationType( leftInfo ? leftInfo->WeaponAnimation : eWeaponAnimationType_None );
	const eWeaponAnimationType	rightType	= eWeaponAnimationType( rightInfo ? rightInfo->WeaponAnimation : eWeaponAnimationType_None );

	// 080703 LUJ, ����� ���Ⱑ �ٸ��� �̵����� �ƴϴ�. �޼տ� ���⸦ �� ����� ��쵵 ���������̴�.
	if( leftType != rightType ||
		leftType == eWeaponAnimationType_None )
	{
		return leftType;
	}

	return eWeaponAnimationType_TwoBlade;
}

/*
void CPlayer::Revive()
{	
	MOVE_POS msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTER_REVIVE;
	msg.dwObjectID = GetID();
	msg.dwMoverID = GetID();
	
	WORD ServerMapNum = (WORD)GAMERESRCMNGR->GetLoadMapNum();
	
	VECTOR3 pos,RandPos;
	GAMERESRCMNGR->GetRegenPoint(pos,RandPos,m_HeroCharacterInfo.LoginPoint_Index,m_HeroCharacterInfo.MapChangePoint_Index);
	
	pos = g_pServerSystem->GetMap()->GetTileManager()->GetNextCloserPos(this,&RandPos,&pos,70,170);
	
	msg.cpos.Compress(&pos);
	
	//A�ϩ��ˡ�?AI ��iy�ˡ�i����A Full����I A�ˢ���?o
	SetLife(m_HeroCharacterInfo.MaxLife);
	
	CCharMove::SetPosition(this,&pos);
	
	PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
}
*/

void CPlayer::ReviveAfterShowdown( BOOL bSendMsg )
{
	ClearMurderIdx();
	m_bNeedRevive = TRUE;

	if( bSendMsg )
	{
		MOVE_POS msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTER_REVIVE;
		msg.dwObjectID = GetID();
		msg.dwMoverID = GetID();
	
		msg.cpos.Compress(CCharMove::GetPosition(this));
		
		PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
	}

	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die);

	m_YYLifeRecoverTime.bStart = FALSE;
	m_YYManaRecoverTime.bStart = FALSE;
	SetLife( GetMaxLife() * 30 / 100 );	//��uoA�ˢ� ��ui��O��i����O CO����i.
}
	
// 080602 LYW --- Player : ����ġ ��ġ (__int32) ���� (__int64) ������� ���� ó��.
//DWORD CPlayer::RevivePenalty(BOOL bAdditionPenalty)								// ���ڸ� ��Ȱ�� �߰� ����ġ �϶��� ó���ϴ� �Լ�.
EXPTYPE CPlayer::RevivePenalty(BOOL bAdditionPenalty)								// ���ڸ� ��Ȱ�� �߰� ����ġ �϶��� ó���ϴ� �Լ�.
{
	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2007.11.30
	// ..ç���� ������ �׾ �ʾƿ��� ���� ���Ƽ ����
	if (g_csDateManager.IsChallengeZoneHere())
	{
		return FALSE;
	}
	// E ����Ʈ �� �߰� added by hseos 2007.11.30

	DWORD PenaltyNum = 0 ;														// �߰� ����ġ �϶��� 2%�� �����Ѵ�.

	if( bAdditionPenalty )
	{
		PenaltyNum = 1 ;
	}
	else
	{
		PenaltyNum = random(1, 3) ;
	}
	
	// 071217 KTH --- Status�� ProtectExp�� ȿ���� �����ϸ� ����ġ�� ���� ��Ű�� �ʴ´�.//
	Status* pStatus;
	pStatus = this->GetBuffStatus();

	if( pStatus->IsProtectExp )
	{
		return 0;
	}
	//////////////////////////////////////////////////////////////////////////////////////

	LEVELTYPE CurLevel = GetLevel() ;											// �÷��̾��� ���� ������ ���Ѵ�.

	EXPTYPE CurExp	= GetPlayerExpPoint() ;										// �÷��̾��� ���� ����ġ�� ���Ѵ�.
	
	EXPTYPE GoalExp	= GAMERESRCMNGR->GetMaxExpPoint(CurLevel) ;					// �÷��̾��� ������ ����ġ�� ���Ѵ�. 

	//---KES CHECK : GoalExp�� �ſ� ū ���̴�. * PeanltyNum�� ���� ��� DWORD�� �Ѿ �� �ִ�.
	// 080602 LYW --- Player : ����ġ ��ġ (__int32) ���� (__int64) ������� ���� ó��.
	//DWORD dwExpA = GoalExp * PenaltyNum ;										// �г�Ƽ ��ġ�� ���Ѵ�.
	EXPTYPE dwExpA = GoalExp * PenaltyNum ;										// �г�Ƽ ��ġ�� ���Ѵ�.
	
	EXPTYPE PenaltyExp = (EXPTYPE)(dwExpA / 100) ;								// �г�Ƽ ����ġ�� ���Ѵ�.

	// 080602 LYW --- Player : ����ġ ��ġ (__int32) ���� (__int64) ������� ���� ó��.
	//DWORD dwExp = 0 ;															// ������ ����ġ ������ �����ϰ� 0���� �����Ѵ�.
	EXPTYPE dwExp = 0 ;															// ������ ����ġ ������ �����ϰ� 0���� �����Ѵ�.

	BOOL bLevelDown = FALSE ;													// ���� �ٿ�� ������ �����ϰ� FALSE ������ �Ѵ�.

	if( CurExp >= PenaltyExp )													// ���� ����ġ�� �г�Ƽ ����ġ �̻��� ���.
	{
		dwExp = CurExp - PenaltyExp ;											// ������ ����ġ�� �����Ѵ�.

		ASSERT( dwExp >= 0 ) ;													// ����ġ�� 0�̻��̾�߸� �Ѵ�.

		SetPlayerExpPoint( dwExp ) ;											// �÷��̾��� ����ġ�� �����Ѵ�.

		// 080602 LYW --- Player : ����ġ ��ġ (__int32) ���� (__int64) ������� ���� ó��.
		//MSG_DWORD3 msg ;														// ����ġ �϶� �޽����� ������ ����ü�� �����Ѵ�.			

		//msg.Category	= MP_USERCONN ;											// ī�װ��� MP_USERCONN�� �����Ѵ�.
		//msg.Protocol	= MP_USERCONN_CHARACTER_DOWNEXP_NOTICE ;				// ���������� ����ġ �϶����� �����Ѵ�.
		//msg.dwObjectID	= GetID() ;												// �÷��̾��� ���̵� �����Ѵ�.
		//msg.dwData1		= (DWORD)PenaltyNum ;									// �г�Ƽ ��ġ�� �����Ѵ�.
		//msg.dwData2		= dwExp ;												// ������ ����ġ�� �����Ѵ�.

		//if( bAdditionPenalty )
		//{
		//	msg.dwData3		= TRUE ;												// �߰� ����ġ �϶� ���θ� TRUE�� �����Ѵ�.
		//}
		//else
		//{
		//	msg.dwData3		= FALSE ;												// �߰� ����ġ �϶� ���θ� FALSE�� �����Ѵ�.
		//}

		MSG_DWORDEX3 msg ;

		msg.Category	= MP_USERCONN ;											// ī�װ��� MP_USERCONN�� �����Ѵ�.
		msg.Protocol	= MP_USERCONN_CHARACTER_DOWNEXP_NOTICE ;				// ���������� ����ġ �϶����� �����Ѵ�.
		msg.dwObjectID	= GetID() ;												// �÷��̾��� ���̵� �����Ѵ�.
		msg.dweData1	= (DWORD)PenaltyNum ;									// �г�Ƽ ��ġ�� �����Ѵ�.
		msg.dweData2	= dwExp ;												// ������ ����ġ�� �����Ѵ�.

		if( bAdditionPenalty )
		{
			msg.dweData3		= TRUE ;												// �߰� ����ġ �϶� ���θ� TRUE�� �����Ѵ�.
		}
		else
		{
			msg.dweData3		= FALSE ;												// �߰� ����ġ �϶� ���θ� FALSE�� �����Ѵ�.
		}

		SendMsg(&msg, sizeof(msg)) ;											// �÷��̾�� �޽����� �����Ѵ�.
	}
	else																		// ���� �÷��̾��� ����ġ�� �г�Ƽ ����ġ���� �������.
	{
		bLevelDown = TRUE ;														// ���� �ٿ� ���θ� TRUE�� �����Ѵ�.

		dwExp = PenaltyExp - CurExp ;											// ������ ����ġ�� �����Ѵ�.

		ASSERT( dwExp >= 0 ) ;													// ����ġ�� 0�̻��̾�߸� �Ѵ�.

		GoalExp = GAMERESRCMNGR->GetMaxExpPoint(CurLevel-1) ;					// �Ѵܰ� ���� ������ ������ ��ǥ ����ġ�� ���Ѵ�.
		SetLevel( CurLevel -1 ) ;												// �÷��̾��� ������ �ٿ��� ������ �����Ѵ�.
		SetPlayerExpPoint(GoalExp-dwExp) ;										// �÷��̾��� ����ġ�� �����Ѵ�.

		//level down.
		// 080602 LYW --- Player : ����ġ ��ġ (__int32) ���� (__int64) ������� ���� ó��.
		//MSG_DWORD4 msg ;														// ����ġ �϶� �޽����� ������ ����ü�� �����Ѵ�.

		//msg.Category	= MP_USERCONN ;											// ī�װ��� MP_USERCONN�� �����Ѵ�.
		//msg.Protocol	= MP_USERCONN_CHARACTER_DOWNLEVEL_NOTICE ;				// ���������� ����ġ �϶����� �����Ѵ�.
		//msg.dwObjectID	= GetID() ;												// �÷��̾��� ���̵� �����Ѵ�.
		//msg.dwData1		= GetLevel() ;											// �÷��̾��� ������ �����Ѵ�.
		//msg.dwData2		= GoalExp-dwExp ;										// �÷��̾��� ����ġ�� �����Ѵ�.
		//msg.dwData3		= (DWORD)PenaltyNum ;									// �г�Ƽ ��ġ�� �����Ѵ�.

		//if( bAdditionPenalty )
		//{
		//	msg.dwData4		= TRUE ;												// �߰� ����ġ �϶� ���θ� TRUE�� �����Ѵ�.
		//}
		//else
		//{
		//	msg.dwData4		= FALSE ;												// �߰� ����ġ �϶� ���θ� FALSE�� �����Ѵ�.
		//}

		MSG_DWORDEX4 msg ;

		msg.Category	= MP_USERCONN ;											// ī�װ��� MP_USERCONN�� �����Ѵ�.
		msg.Protocol	= MP_USERCONN_CHARACTER_DOWNLEVEL_NOTICE ;				// ���������� ����ġ �϶����� �����Ѵ�.
		msg.dwObjectID	= GetID() ;												// �÷��̾��� ���̵� �����Ѵ�.
		msg.dweData1	= (DWORDEX)GetLevel() ;									// �÷��̾��� ������ �����Ѵ�.
		msg.dweData2	= GoalExp-dwExp ;										// �÷��̾��� ����ġ�� �����Ѵ�.
		msg.dweData3	= (DWORDEX)PenaltyNum ;									// �г�Ƽ ��ġ�� �����Ѵ�.


		if( bAdditionPenalty )
		{
			msg.dweData4		= TRUE ;												// �߰� ����ġ �϶� ���θ� TRUE�� �����Ѵ�.
		}
		else
		{
			msg.dweData4		= FALSE ;												// �߰� ����ġ �϶� ���θ� FALSE�� �����Ѵ�.
		}

		SendMsg(&msg, sizeof(msg)) ;											// �÷��̾�� �޽����� �����Ѵ�.
	}

	// 080414 LUJ, ����ġ �ս� �� �սǵ� ����ġ ��ŭ �α׸� �����
	InsertLogExp(
		eExpLog_LosebyRevivePresent,
		GetID(),
		GetLevel(),
		PenaltyExp,
		dwExp,
		0,
		0,
		0 );

	return PenaltyExp;
}


void CPlayer::RevivePresentSpot()
{	
	if(GetState() != eObjectState_Die)
	{
		ASSERT(0);
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 1; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return;
	}

	if( LOOTINGMGR->IsLootedPlayer( GetID() ) )	//����c����AA�ϡ���?��I
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 2; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return;
	}

	if( IsExitStart() )
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 4; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return;
	}

	m_bNeedRevive = TRUE;
	MOVE_POS msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTER_REVIVE;
	msg.dwObjectID = GetID();
	msg.dwMoverID = GetID();
	
	msg.cpos.Compress(CCharMove::GetPosition(this));
		
	PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));

	LEVELTYPE curLevel = GetLevel() ;

	if( curLevel >= 10 && m_bNoExpPenaltyByPK == FALSE )
	{
		RevivePenalty(FALSE) ;
		RevivePenalty(TRUE) ;
	
		if( !g_csDateManager.IsChallengeZoneHere() )
			DecreaseCount( eBuffSkillCountType_Dead );
	}

	// 070419 LYW --- Player : Delete protect money and exp.
	/*
	if( !EVENTMAPMGR->IsEventMap() )
	if(GetLevel() >= 5 && m_bDieForGFW == FALSE )
	{
		// RaMa -> ����ġ+����ȣ
		DWORD ItemIdx = 0;
		SHOPITEMWITHTIME* pShopItem = NULL;
		if( GetShopItemStats()->ProtectCount )		
		{
			DWORD ItemIdx = m_ShopItemManager.GetProtectItemIdx();
			if( ItemIdx == 0 )
				goto ProtectAll_UseFail;
			pShopItem = m_ShopItemManager.GetUsingItemInfo( ItemIdx );
			if( !pShopItem )
				goto ProtectAll_UseFail;

			// Log�߰�
			LogItemMoney( GetID(), GetObjectName(), 0, "",
				eLog_ShopItemProtectAll, GetMoney(eItemTable_Inventory), 0, 0,
				ItemIdx, pShopItem->ShopItem.ItemBase.dwDBIdx, pShopItem->ShopItem.ItemBase.Position, 0, pShopItem->ShopItem.ItemBase.Durability, GetPlayerExpPoint());
			InsertLogExp( eExpLog_ProtectExp, GetID(), GetLevel(), GetShopItemStats()->ProtectCount, GetPlayerExpPoint(), m_MurdererKind, m_MurdererIDX, 0) ;///*�����Ƽ ���� - GetPlayerAbilityExpPoint());

			--GetShopItemStats()->ProtectCount;
			if( GetShopItemStats()->ProtectCount <= 0 )
			{
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_USEEND, ItemIdx);
				m_ShopItemManager.DeleteUsingShopItem( ItemIdx );
				m_ShopItemManager.SetProtectItemIdx( 0 );
			}
			else
			{
				// DB�� ������Ʈ
				ShopItemUseParamUpdateToDB( 0,  GetID(), ItemIdx, GetShopItemStats()->ProtectCount );
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_PROTECTALL, GetShopItemStats()->ProtectCount);
			}
		}
		else
		{
ProtectAll_UseFail:
			// RaMa -> ����ȣ �ֹ���
			if( m_ShopItemManager.GetUsingItemInfo( eIncantation_MoneyProtect ) )
			{
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_MONEYPROTECT, eIncantation_MoneyProtect);
				m_ShopItemManager.DeleteUsingShopItem( eIncantation_MoneyProtect );
			}
			else
			{
				CItemSlot* pSlot = m_ItemContainer.GetSlot(eItemTable_Inventory);
				DWORD CurMoney = pSlot->GetMoney();
				if( !m_bDieForGFW )
					SetMoney((MONEYTYPE)(CurMoney*0.06), MONEY_SUBTRACTION, MF_LOST, eItemTable_Inventory, eMoneyLog_LoseDie, 0);

//				if( m_bDieForGFW )
//					SetMoney((MONEYTYPE)(CurMoney*0.03), MONEY_SUBTRACTION, MF_LOST, eItemTable_Inventory, eMoneyLog_LoseDie, 0);
//				else
//					SetMoney((MONEYTYPE)(CurMoney*0.06), MONEY_SUBTRACTION, MF_LOST, eItemTable_Inventory, eMoneyLog_LoseDie, 0);
			}
			
			// RaMa -> ����ġ��ȣ �ֹ���
			if( m_ShopItemManager.GetUsingItemInfo( eIncantation_ExpProtect ) )
			{
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_EXPPROTECT, eIncantation_ExpProtect);
				m_ShopItemManager.DeleteUsingShopItem( eIncantation_ExpProtect );
			}
			else
			{
				EXPTYPE pointForLevel = GAMERESRCMNGR->GetMaxExpPoint(GetLevel());
				EXPTYPE minusExp = 0;

				if( !m_bDieForGFW )	minusExp = (EXPTYPE)(pointForLevel * 0.03);
//				if( m_bDieForGFW )	minusExp = (EXPTYPE)(pointForLevel * 0.015);
//				else				minusExp = (EXPTYPE)(pointForLevel * 0.03);

				ReduceExpPoint(minusExp, eExpLog_LosebyRevivePresent);
			}
		}
	}
	*/
	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die);
	
	DWORD MaxLife = GetMaxLife();
	DWORD MaxMana = GetMaxMana();

	int nReviveVal = (int)(MaxLife*0.3) ;

	MSG_INT ReviveLife;
	ReviveLife.Category = MP_CHAR;
	ReviveLife.Protocol = MP_CHAR_LIFE_ACK;
	ReviveLife.dwObjectID = GetID();
	ReviveLife.nData = nReviveVal ;
	SendMsg(&ReviveLife,sizeof(ReviveLife));			
		
	if(GetPartyIdx())
	{
		SendPlayerLifeToParty(nReviveVal, GetPartyIdx());
	}
		
	m_HeroCharacterInfo.Life = nReviveVal;

	// 070417 LYW --- Player : Modified setting mana when the character revived.
	DWORD dwManaRate = (DWORD)(GetStatRound((WORD)(MaxMana*0.3))) ;
	if( GetMana() < dwManaRate )
	{
		SetMana(dwManaRate);
	}
	
	m_YYLifeRecoverTime.bStart = FALSE;
	m_YYManaRecoverTime.bStart = FALSE;
	ClearMurderIdx();

	m_bDieForGFW = FALSE;
	m_bDieInSpecialMap = FALSE;
}

// 080602 LYW --- Player : ����ġ ��ġ (__int32) ���� (__int64) ������� ���� ó��.
//DWORD CPlayer::ReviveBySkill()
EXPTYPE CPlayer::ReviveBySkill()
{	
	// 080602 LYW --- Player : ����ġ ��ġ (__int32) ���� (__int64) ������� ���� ó��.
	//DWORD exp = 0;
	EXPTYPE exp = 0;

	if(GetState() != eObjectState_Die)
	{
		ASSERT(0);
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 1; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return 0;
	}

	if( LOOTINGMGR->IsLootedPlayer( GetID() ) )	//����c����AA�ϡ���?��I
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 2; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return 0;
	}

	if( IsExitStart() )
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 4; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return 0;
	}

	m_bNeedRevive = TRUE;
	MOVE_POS msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTER_REVIVE;
	msg.dwObjectID = GetID();
	msg.dwMoverID = GetID();
	
	msg.cpos.Compress(CCharMove::GetPosition(this));
		
	PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));

	LEVELTYPE curLevel = GetLevel() ;

	if( curLevel >= 10 && m_bNoExpPenaltyByPK == FALSE )
	{
		exp = RevivePenalty(FALSE) ;

		if( !g_csDateManager.IsChallengeZoneHere() )
			DecreaseCount( eBuffSkillCountType_Dead );
	}

	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die);
	
	DWORD MaxLife = GetMaxLife();
	DWORD MaxMana = GetMaxMana();

	int nReviveVal = (int)(MaxLife*0.3) ;

	MSG_INT ReviveLife;
	ReviveLife.Category = MP_CHAR;
	ReviveLife.Protocol = MP_CHAR_LIFE_ACK;
	ReviveLife.dwObjectID = GetID();
	ReviveLife.nData = nReviveVal ;
	SendMsg(&ReviveLife,sizeof(ReviveLife));			
		
	if(GetPartyIdx())
	{
		SendPlayerLifeToParty(nReviveVal, GetPartyIdx());
	}
		
	m_HeroCharacterInfo.Life = nReviveVal;

	// 070417 LYW --- Player : Modified setting mana when the character revived.
	DWORD dwManaRate = (DWORD)(GetStatRound((WORD)(MaxMana*0.3))) ;
	if( GetMana() < dwManaRate )
	{
		SetMana(dwManaRate);
	}
	
	m_YYLifeRecoverTime.bStart = FALSE;
	m_YYManaRecoverTime.bStart = FALSE;
	ClearMurderIdx();

	m_bDieForGFW = FALSE;
	m_bDieInSpecialMap = FALSE;

	return exp;
}

void CPlayer::ReviveLogIn()
{	
	if(GetState() != eObjectState_Die)
	{
		ASSERT(0);
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 1; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return;
	}
	
	if( LOOTINGMGR->IsLootedPlayer( GetID() ) )	//����c����AA�ϡ���?��I
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 2; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return;
	}

	if( IsExitStart() )
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 4; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return;
	}

	m_bNeedRevive = TRUE;
	MOVE_POS msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTER_REVIVE;
	msg.dwObjectID = GetID();
	msg.dwMoverID = GetID();
	
/*	WORD ServerMapNum = (WORD)GAMERESRCMNGR->GetLoadMapNum();
	
	VECTOR3 pos,RandPos;
	if( BATTLESYSTEM->GetBattle( this )->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
	{
		DWORD LoginIndex = 0;
		if( GetBattleTeam() == eBattleTeam1 )			LoginIndex = 1103;
		else											LoginIndex = 1102;
		GAMERESRCMNGR->GetRegenPoint(pos,RandPos,m_HeroCharacterInfo.LoginPoint_Index,(WORD)LoginIndex);
	}
	else
		GAMERESRCMNGR->GetRegenPoint(pos,RandPos,m_HeroCharacterInfo.LoginPoint_Index,m_HeroCharacterInfo.MapChangePoint_Index);
	
	pos = RandPos;//g_pServerSystem->GetMap()->GetTileManager()->GetNextCloserPos(this,&RandPos,&pos,70,170);
*/
	VECTOR3* ppos;
	VECTOR3 pos;

	ppos = GAMERESRCMNGR->GetRevivePoint();

	int temp;
	temp = rand() % 500 - 250;
	pos.x = ppos->x + temp;
	temp = rand() % 500 - 250;
	pos.z = ppos->z + temp;
	pos.y = 0;

	msg.cpos.Compress(&pos);
	
	CCharMove::SetPosition(this,&pos);

	PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
		
	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die);

	//ReviveLogInPenelty();
	const LEVELTYPE curLevel = GetLevel() ;
	
	if(	curLevel >= 10 && !m_bDieForGFW && m_bNoExpPenaltyByPK == FALSE )
	{
		RevivePenalty(FALSE);
		
		if( !g_csDateManager.IsChallengeZoneHere() )
            DecreaseCount( eBuffSkillCountType_Dead );
	}

	m_bDieForGFW = FALSE;
	
	DWORD CurLife = GetMaxLife();
	int nReviveVal = (int)(CurLife*0.3) ;

	MSG_INT ReviveLife;
	ReviveLife.Category = MP_CHAR;
	ReviveLife.Protocol = MP_CHAR_LIFE_ACK;
	ReviveLife.dwObjectID = GetID();
	ReviveLife.nData = nReviveVal ;
	SendMsg(&ReviveLife,sizeof(ReviveLife));	

	m_HeroCharacterInfo.Life = nReviveVal;

	/*DWORD CurMana = GetMaxMana();
	SetMana(0);*/

	DWORD MaxMana = GetMaxMana();

	DWORD dwManaRate = (DWORD)(GetStatRound((WORD)(MaxMana*0.3))) ;
	if( GetMana() < dwManaRate )
	{
		SetMana(dwManaRate);
	}
	
//30AE�ơ� ����Au
	OBJECTSTATEMGR_OBJ->StartObjectState(this,eObjectState_Immortal,0);
	// 06.08.29. RaMa.
	OBJECTSTATEMGR_OBJ->EndObjectState( this, eObjectState_Immortal, 30000 );
	
	m_YYLifeRecoverTime.bStart = FALSE;
	m_YYManaRecoverTime.bStart = FALSE;

	if(m_HeroInfo.PartyID)
	{	
		CParty* pParty = PARTYMGR->GetParty(m_HeroInfo.PartyID);
		if(!pParty)		return;

		SEND_PARTYICON_REVIVE msg;		
		msg.Category = MP_PARTY;
		msg.Protocol = MP_PARTY_REVIVEPOS;
		msg.dwMoverID = m_BaseObjectInfo.dwObjectID;
		msg.Pos.posX = (WORD)pos.x;
		msg.Pos.posZ = (WORD)pos.z;
		pParty->SendMsgToAll(&msg, sizeof(msg));
	}
	ClearMurderIdx();

	m_bDieInSpecialMap = FALSE;
}

void CPlayer::ReviveLogInPenelty()
{
/*	// RaMa -04.11.24   ->�����̾� �������� �г�Ƽ ����.
	// RaMa -04.12.14����   ->����ġ�� ������ ������, ����� Client�� ����
	if( !EVENTMAPMGR->IsEventMap() && !QUESTMAPMGR->IsQuestMap() )
	if(GetLevel() >= 5 && m_bDieForGFW == FALSE )
	{		
		BOOL bExchanged = FALSE;
		// RaMa -> ����ġ+����ȣ
		DWORD ItemIdx = 0;
		SHOPITEMWITHTIME* pShopItem = NULL;
/*		if( GetShopItemStats()->ProtectCount )		
		{
			DWORD ItemIdx = m_ShopItemManager.GetProtectItemIdx();
			if( ItemIdx == 0 )
				goto ProtectAll_UseFail;
			pShopItem = m_ShopItemManager.GetUsingItemInfo( ItemIdx );
			if( !pShopItem )
				goto ProtectAll_UseFail;

			// Log�߰�
			LogItemMoney( GetID(), GetObjectName(), 0, "",
				eLog_ShopItemProtectAll, GetMoney(eItemTable_Inventory), 0, 0,
				ItemIdx, pShopItem->ShopItem.ItemBase.dwDBIdx, pShopItem->ShopItem.ItemBase.Position, 0, pShopItem->ShopItem.ItemBase.Durability, GetPlayerExpPoint());
			InsertLogExp( eExpLog_ProtectExp, GetID(), GetLevel(), GetShopItemStats()->ProtectCount, GetPlayerExpPoint(), m_MurdererKind, m_MurdererIDX, 0/*�����Ƽ ���� - GetPlayerAbilityExpPoint()*///);

/*			--GetShopItemStats()->ProtectCount;
			if( GetShopItemStats()->ProtectCount <= 0 )
			{
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_USEEND, ItemIdx);
				m_ShopItemManager.DeleteUsingShopItem( ItemIdx );
				m_ShopItemManager.SetProtectItemIdx( 0 );
			}
			else
			{
				// DB�� ������Ʈ
				ShopItemUseParamUpdateToDB( 0, GetID(), ItemIdx, GetShopItemStats()->ProtectCount );
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_PROTECTALL, GetShopItemStats()->ProtectCount);
			}
		}
		else
		{
ProtectAll_UseFail:
			if( m_ShopItemManager.GetUsingItemInfo( eIncantation_MoneyProtect ) )
			{
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_MONEYPROTECT, eIncantation_MoneyProtect);
				m_ShopItemManager.DeleteUsingShopItem( eIncantation_MoneyProtect );
			}
			else
*//*			{
				CItemSlot* pSlot = m_ItemContainer.GetSlot(eItemTable_Inventory);
				DWORD CurMoney = pSlot->GetMoney();
				if( !m_bDieForGFW )
					SetMoney( (MONEYTYPE)(CurMoney*0.04), MONEY_SUBTRACTION, MF_LOST , eItemTable_Inventory, eMoneyLog_LoseDie, 0 );
//				if( m_bDieForGFW )
//					SetMoney( (MONEYTYPE)(CurMoney*0.02), MONEY_SUBTRACTION, MF_LOST , eItemTable_Inventory, eMoneyLog_LoseDie, 0 );
//				else
//					SetMoney( (MONEYTYPE)(CurMoney*0.04), MONEY_SUBTRACTION, MF_LOST , eItemTable_Inventory, eMoneyLog_LoseDie, 0 );
/*			}

			// RaMa - 04.12.28     ->�ֹ����߰�
			if( m_ShopItemManager.GetUsingItemInfo( eIncantation_ExpProtect ) )
			{
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_EXPPROTECT, eIncantation_ExpProtect);
				m_ShopItemManager.DeleteUsingShopItem( eIncantation_ExpProtect );
			}
			else
			{
				EXPTYPE pointForLevel = GAMERESRCMNGR->GetMaxExpPoint(GetLevel());
				EXPTYPE minusExp = 0;				

				float fRate = 0.02f;
				if( GetBattle()->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
					fRate = 0.01f;

				if( !m_bDieForGFW )	minusExp = (EXPTYPE)(pointForLevel * fRate);				
//				if( m_bDieForGFW )	minusExp = (EXPTYPE)(pointForLevel * 0.01);
//				else				minusExp = (EXPTYPE)(pointForLevel * 0.02);				

				ReduceExpPoint(minusExp, eExpLog_LosebyReviveLogIn);
			}
		}

		// RaMa - �����߰�
/*		if( bExchanged && m_ShopItemOption.ExpPeneltyPoint==0 && m_ShopItemOption.MoneyPeneltyPoint==0 )
		{
			MSG_DWORD msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_MSG;
			msg.dwData = eShopItem_PeneltyCountZero;
			SendMsg(&msg, sizeof(msg));
		}*/
//	}
	if( GetLevel() >= 10 && m_bNoExpPenaltyByPK == FALSE )
	{
		RevivePenalty(FALSE) ;
		RevivePenalty(TRUE) ;
		
		if( !g_csDateManager.IsChallengeZoneHere() )
			DecreaseCount( eBuffSkillCountType_Dead );
	}

	DWORD CurLife = GetMaxLife();
	DWORD CurMana = GetMaxMana();

	// 080625 LYW --- Player : ����� ������ �϶�� �ϳ�, ĳ���Ͱ� ���� �����̱� ������, 
	// ����� ���� �Լ��� ����� ����� ���� �ʴ´�. ������, ������ ������� �����ϴ� 
	// �Լ��� �����Ѵ�.
	//SetLife((DWORD)(CurLife*0.3));
	//SetMana(0);

	// ����� ����.
	DWORD dwNewLife = 0 ;
	dwNewLife = (DWORD)(CurLife * 0.3f) ;

	SetLifeForce(dwNewLife, TRUE) ;

	// ���� ����.
	DWORD dwNewMana = 0 ;
	dwNewMana = (DWORD)(CurMana* 0.3f) ;

	SetManaForce(dwNewMana, TRUE) ;

	m_bDieForGFW = FALSE;
}


void CPlayer::DoDie(CObject* pAttacker)
{
	OBJECTSTATEMGR_OBJ->StartObjectState(this,eObjectState_Die,pAttacker->GetID());

	m_bNoExpPenaltyByPK = FALSE;

	if( pAttacker->GetObjectKind() == eObjectKind_Pet )
		pAttacker = ( ( CPet* )pAttacker )->GetMaster();

	if(pAttacker->GetObjectKind() == eObjectKind_Player)
	{
		CPlayer* pAttackPlayer = (CPlayer*)pAttacker;
		CBattle* pBattle = pAttacker->GetBattle();
		if(pBattle->GetBattleKind() == eBATTLE_KIND_NONE || pAttacker->GetBattleID() != GetBattleID() )
		{
			// for pk
			if( LOOTINGMGR->IsLootingSituation( this, pAttackPlayer ) )
			{
				//SW060831 ���� �� �� //���� ����� üũ ����
				//if(PARTYWARMGR->IsMemberInPartyWar( this ))
				//{
				//	SetPenaltyByDie(TRUE);
				//}

				LOOTINGMGR->CreateLootingRoom( this, (CPlayer*)pAttacker );
			}

			//���� PK�� ���װ�, ���� PK�� �״�. 
			if( IsPKMode() == FALSE && pAttackPlayer->IsPKMode() == TRUE
				/*&& WANTEDMGR->IsOwner((CPlayer*)pAttacker, GetWantedIdx()) == FALSE*/ )	// attacker is pk
			{
				SetMurderIDX(pAttacker->GetID());
				SetMurderKind(pAttacker->GetObjectKind());

				m_bNoExpPenaltyByPK = TRUE;

				//---KES ��� �ּ�
				//WANTEDMGR->SendRegistWanted(this, (CPlayer*)pAttacker);

				//journal insert
				JouranlInsertWanted(GetID(), eJournal_Wanted, eJournal_MurderedbyChr, ((CPlayer*)pAttacker)->GetObjectName());

				// partywar
				//if( PARTYWARMGR->PlayerDie( this, (CPlayer*)pAttacker ) )
				//{
				//	SetReadyToRevive( FALSE );
				//}

				//---KES ��� �ּ�
				//WANTEDMGR->Complete((CPlayer*)pAttacker, this);
			}
			else
			{
				//---KES ���⸸ �ִ� ������?
				//SetReadyToRevive(TRUE);

				// partywar
				//if( PARTYWARMGR->PlayerDie( this, (CPlayer*)pAttacker ) )
				//{
				//	SetReadyToRevive( FALSE );
				//}
				//else
				{
					// guildfieldwar
					if( GUILDWARMGR->JudgeGuildWar( this, (CPlayer*)pAttacker ) &&
						! m_bDieForGFW )
					{
						SetDieForGFW( TRUE );

						const CGuildManager::ScoreSetting& setting = GUILDMGR->GetScoreSetting();

						GuildUpdateScore( pAttackPlayer, this, setting.mKillerScore, setting.mCorpseScore );
					}

					//---KES ��� �ּ�
					//WANTEDMGR->Complete((CPlayer*)pAttacker, this);
				}
			}

			//---KES PK ������ �ű�
			SetReadyToRevive(TRUE);

		}
	}
	else if(pAttacker->GetObjectKind() & eObjectKind_Monster )
	{		
		SetMurderKind(((CMonster*)pAttacker)->GetMonsterKind());
			
		// 080616 LUJ, ������ ��쿡�� ���Ƽ�� ���� �ʵ��� �Ѵ�
		if( pAttacker->GetObjectKind() != eObjectKind_Trap )
		{
			//SW060831 ���� �� �� //���� ����� üũ ����
			SetPenaltyByDie(TRUE);
		}

		SetReadyToRevive(TRUE);

		// add quest event
		QUESTMGR->AddQuestEvent( this, &CQuestEvent( eQuestEvent_Die, g_pServerSystem->GetMapNum(), 0 ) );
	}

	//KES EXIT
	ExitCancel();
	//KES EXCHANGE 031002
	EXCHANGEMGR->CancelExchange( this );
	STREETSTALLMGR->UserLogOut( this );
	//KES PKLOOTING
	LOOTINGMGR->LootingCancel( this );
	//KES PKPlayerPanelty
	PKMGR->DiePanelty( this, pAttacker );

//---KES Aggro 070918
//---��׷� ����Ʈ ������
	RemoveAllAggroed();	//*����: �Ʒ� FollowMonsterList�����ϱ� ������ ���־��, ���� ��׷θ� Ÿ������ ���� �� �ִ�.
//-------------------

	CMonster * pObject = NULL;
	m_FollowMonsterList.SetPositionHead();
	while(pObject = (CMonster *)m_FollowMonsterList.GetData())
	{
		pObject->SetTObject(NULL);
	}
	m_FollowMonsterList.RemoveAll();

	//SW051112 ���ָ��
	InitMussangPoint();
	SendMussangInfo();

	//���ָ�� �����϶��� ��������ش�
	if(IsMussangMode())
	{
		SetMussangMode(FALSE);
		SendMussangEnd();
	}

	//---KES ������ �̵��� �����־�� �Ѵ�.
	if( CCharMove::IsMoving(this) )
	{
		VECTOR3 pos;
		GetPosition( &pos );
		CCharMove::EndMove( this, gCurTime, &pos );
	}

	QUESTMAPMGR->DiePlayer( this );

	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2007.11.29
	g_csDateManager.SRV_EndChallengeZone(this, CSHDateManager::CHALLENGEZONE_END_ALL_DIE);
	// E ����Ʈ �� �߰� added by hseos 2007.11.29

	if( mpCurPet )
	{
		mpCurPet->DoDie( pAttacker );
	}
}

float CPlayer::DoGetMoveSpeed()
{
	if( GetAbnormalStatus()->IsMoveStop )
	{
		return 0;
	}

	float speed = 0;

	if(m_MoveInfo.KyungGongIdx)
	{
		CKyungGongInfo* pKGInfo = KYUNGGONGMGR->GetKyungGongInfo(m_MoveInfo.KyungGongIdx);
		ASSERT(pKGInfo);
		if(pKGInfo == NULL)
			return 0;
		speed = pKGInfo->GetSpeed()/*�����Ƽ ���� - +GetAbilityStats()->Kyunggong*/;

		// �ƹ�Ÿ������ ����ӵ� ���
//        speed += m_AvatarOption.KyunggongSpeed;

		// �����?����ӵ� ���
//		speed += m_ShopItemOption.KyungGongSpeed;			
		return speed;
	}
	else
	{
		if(m_MoveInfo.MoveMode == eMoveMode_Run)
			speed = RUNSPEED;
		else
			speed = WALKSPEED;
	}

	// 080630 LUJ, ��Ʈ ������ ��ġ�� ����ǵ��� ��
	float addrateval = ( GetRateBuffStatus()->MoveSpeed + GetRatePassiveStatus()->MoveSpeed ) / 100.f + m_itemStats.mMoveSpeed.mPercent + m_SetItemStats.mMoveSpeed.mPercent;
	float addval = GetBuffStatus()->MoveSpeed + GetPassiveStatus()->MoveSpeed + m_itemStats.mMoveSpeed.mPlus + m_SetItemStats.mMoveSpeed.mPlus;

	m_MoveInfo.AddedMoveSpeed = speed * addrateval + addval;

	speed += m_MoveInfo.AddedMoveSpeed;

	cprintf( "move_speed: %f\n", speed );

	return max( 0, speed );
}


void CPlayer::SetInitedGrid()
{
	MSGBASE msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_GRIDINIT;
	SendMsg(&msg,sizeof(msg));

	CGridUnit::SetInitedGrid();
	
	CBattle* pBattle = BATTLESYSTEM->GetBattle(this->GetBattleID());
	if(pBattle && pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle(pBattle, this);

	QUESTMGR->AddQuestEvent( this, &CQuestEvent( eQuestEvent_GameEnter, 0, 1 ) );
	QUESTMGR->AddQuestEvent( this, &CQuestEvent( eQuestEvent_MapChange, 0, g_pServerSystem->GetMapNum() ) );

	//Pet SummonningPet
	//m_PetManager.CheckSummonningPet();
}
// RaMa - 04.11.10    -> ShopItemOption �߰�   AvatarOption�߰�(05.02.16)
DWORD CPlayer::DoGetCritical()
{	
	return (DWORD)mCriticalRate;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 06. 07 ���� ����(�ϰ�) - �̿���
DWORD CPlayer::DoGetDecisive()
{	
	return (DWORD)mCriticalRate;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CPlayer::DoGetPhyAttackPowerMin()
{
	return (DWORD)mPhysicAttackMin;
}

DWORD CPlayer::DoGetPhyAttackPowerMax()
{
	return (DWORD)mPhysicAttackMax;
}

void CPlayer::DoDamage(CObject* pAttacker,RESULTINFO* pDamageInfo,DWORD beforeLife)
{
	SetObjectBattleState( eObjectBattleState_Battle );
	if ( pAttacker )
	{
		pAttacker->SetObjectBattleState( eObjectBattleState_Battle );
	}

	if( GetAbnormalStatus()->IsSlip )
	{
		EndBuffSkillByStatus( eStatusKind_Slip );
	}
		
	cSkillObject* pSkill = SKILLMGR->GetSkillObject( mCurrentSkillID );

	if( pSkill )
	{
		if( SKILL_STATE_CASTING == pSkill->GetSkillState() )
		{
			cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )pSkill->GetSkillInfo();

			if( pSkillInfo->GetSkillInfo()->Cancel )
			{
				WORD rate = rand() % 100;

				if( rate < pSkillInfo->GetSkillInfo()->Cancel )
				{
					MSG_DWORD msg;
					msg.Category = MP_SKILL;
					msg.Protocol = MP_SKILL_CANCEL_NOTIFY;
					msg.dwObjectID = GetID();
					msg.dwData = pSkill->GetID();

					PACKEDDATA_OBJ->QuickSend( this, &msg, sizeof( MSG_DWORD ) );

					pSkill->SetEndState();
					pSkill->EndState();
				}
			}
		}
	}
}

void CPlayer::InitBaseObjectInfo(BASEOBJECT_INFO* pBaseInfo)
{
	ASSERT(GetInitState() == PLAYERINITSTATE_ONLY_ADDED);
	memcpy(&m_BaseObjectInfo,pBaseInfo,sizeof(BASEOBJECT_INFO));
}

/* ��oC����?E���Ϣ�����c Return */
MONEYTYPE CPlayer::SetMoney( MONEYTYPE ChangeValue, BYTE bOper, BYTE MsgFlag, eITEMTABLE tableIdx, BYTE LogType, DWORD TargetIdx )
{
	CPurse* pPurse = m_ItemContainer.GetPurse(tableIdx);
	if( !pPurse) return 0;

	MONEYTYPE Real = 0;
	if( bOper == MONEY_SUBTRACTION )
	{
		Real = pPurse->Subtraction( ChangeValue, MsgFlag );
	}
	else
	{
		Real = pPurse->Addition( ChangeValue, MsgFlag );
	}

	// Log Money AId������c AI��io ����?CO��O���� 
	if( tableIdx == eItemTable_Inventory || tableIdx == eItemTable_ShopInven )
	{
		if( Real >= 10000 )
			InsertLogWorngMoney( LogType, GetID(), Real, GetMoney(), GetMoney(eItemTable_Storage), TargetIdx );
	}

	return Real;
}

/* ��i����; �ϩ�O; A�Ϣ���������CN �����Ϩ�������IAI AO�ˡ�AAo�ˡ�| ��o?�ˡ�A�ˡ�U.*/
BOOL CPlayer::IsEnoughAdditionMoney(MONEYTYPE money, eITEMTABLE tableIdx )
{
	CPurse* pPurse = m_ItemContainer.GetPurse(tableIdx);
	if(!pPurse) return FALSE;

	return pPurse->IsAdditionEnough( money );
}


MONEYTYPE CPlayer::GetMaxPurseMoney(eITEMTABLE TableIdx)
{
	CPurse* pPurse = m_ItemContainer.GetPurse(TableIdx);
	if( !pPurse) return FALSE;
	return pPurse->GetMaxMoney();
}

void CPlayer::SetMaxPurseMoney(eITEMTABLE TableIdx, DWORD MaxMoney)
{
	//C��I������o; ��ic�ˡ�e ��i���� �����ˡ�����u �ˡ�������o����A�ˢ碮���ˢ� �ˡ�A��ui�ϩ����ϡˡ�U.
	if(TableIdx != eItemTable_Storage)
	{
		ASSERT(0);
		return;
	}
	CPurse* pPurse = m_ItemContainer.GetPurse(TableIdx);
	if( !pPurse) return;
	pPurse->SetMaxMoney(MaxMoney);
}

void CPlayer::AddQuest(CQuestBase* pQuest)
{
	if(m_QuestList.GetData(pQuest->GetQuestIdx()))
	{
		ASSERT(0);
		return;
	}
	m_QuestList.Add( pQuest, pQuest->GetQuestIdx() );
}

void CPlayer::RemoveQuest(DWORD QuestIdx)
{
	m_QuestList.Remove(QuestIdx);
}

BOOL CPlayer::SetQuestState(DWORD QuestIdx, QSTATETYPE value)
{
	CQuestBase* pQuest;
	pQuest = m_QuestList.GetData(QuestIdx);

	if( !pQuest ) 
	{
//		char buff[256] = {0,};
//		sprintf(buff, "��?a����a�ˡ�A Au��o����������c�����ˢ� xAc ��uECN�ˡ�U�ϩ��ϡ� CI��?��I������I ��oy��u��c��?�ˢ碮��O ��uE����AAa��?���� [QUEST ID : %d]", QuestIdx);
//		ASSERTMSG(0, buff);
		return FALSE;
	}

	pQuest->SetValue(value);
	BOOL bEnd = pQuest->IsComplete();

	// DB��?�ˢ� ��uA����ACN�ˡ�U.
	QuestUpdateToDB( GetID(), QuestIdx, value, bEnd );

	if( bEnd )
	{
		m_QuestList.Remove(QuestIdx);

		MSG_DWORD msg;
		msg.Category = MP_QUEST;
		msg.Protocol = MP_QUEST_REMOVE_NOTIFY;
		msg.dwObjectID = GetID();
		msg.dwData = QuestIdx;
		SendMsg(&msg, sizeof(msg));
	}

	return TRUE;
}

void CPlayer::SetInitState(int initstate,DWORD protocol)
{
	if(initstate == PLAYERINITSTATE_ONLY_ADDED)
	{
		m_initState = PLAYERINITSTATE_ONLY_ADDED;
		InitClearData();
	}
	else
		m_initState |= initstate;
	
	if ((m_initState & PLAYERINITSTATE_SKILL_INFO) &&
		(m_initState & PLAYERINITSTATE_ITEM_INFO) &&
		(m_initState & PLAYERINITSTATE_QUICK_INFO) &&
//		(m_initState & PLAYERINITSTATE_GMCONTROL_INFO) &&
		(m_initState & PLAYERINITSTATE_HERO_INFO) &&
//---KES
		(m_initState & PLAYERINITSTATE_QEUST_INFO) &&
		(m_initState & PLAYERINITSTATE_FARM_INFO) &&
//----------
		// desc_hseos_��ȥ_01
		// S ��ȥ �߰� added by hseos 2007.12.12
		(m_initState & PLAYERINITSTATE_CHALLENGEZONE_INFO) &&
//		(m_initState & PLAYERINITSTATE_MARRIAGE_INFO) &&
		// E ��ȥ �߰� added by hseos 2007.12.12
		!(m_initState & PLAYERINITSTATE_STORAGE_INFO) )
	{
		CharacterStorageInfo(GetUserID(), GetID());
		return;
	}

	if ((m_initState & PLAYERINITSTATE_SKILL_INFO) &&
		(m_initState & PLAYERINITSTATE_ITEM_INFO) &&
		(m_initState & PLAYERINITSTATE_QUICK_INFO) &&
//		(m_initState & PLAYERINITSTATE_GMCONTROL_INFO) &&
		(m_initState & PLAYERINITSTATE_HERO_INFO) && 
//---KES
		(m_initState & PLAYERINITSTATE_QEUST_INFO) &&
		(m_initState & PLAYERINITSTATE_FARM_INFO) &&
//----------
		// desc_hseos_��ȥ_01
		// S ��ȥ �߰� added by hseos 2007.12.12
		(m_initState & PLAYERINITSTATE_CHALLENGEZONE_INFO) &&
//		(m_initState & PLAYERINITSTATE_MARRIAGE_INFO) &&
		// E ��ȥ �߰� added by hseos 2007.12.12
		(m_initState & PLAYERINITSTATE_STORAGE_INFO) &&
		!(m_initState & PLAYERINITSTATE_SHOPITEM_USEDINFO) )
	{
		// Mall Item�� �޾ƿ����� �Ѵ�.
		UsingShopItemInfo( GetID() );
		SavedMovePointInfo( GetID() );
		return;
	}

	if ((m_initState & PLAYERINITSTATE_SKILL_INFO) &&
		(m_initState & PLAYERINITSTATE_ITEM_INFO) &&
		(m_initState & PLAYERINITSTATE_QUICK_INFO) &&
		(m_initState & PLAYERINITSTATE_HERO_INFO) &&
//---KES
		(m_initState & PLAYERINITSTATE_QEUST_INFO) &&
		(m_initState & PLAYERINITSTATE_FARM_INFO) &&
//----------
		// desc_hseos_��ȥ_01
		// S ��ȥ �߰� added by hseos 2007.12.12
		(m_initState & PLAYERINITSTATE_CHALLENGEZONE_INFO) &&
//		(m_initState & PLAYERINITSTATE_MARRIAGE_INFO) &&
		// E ��ȥ �߰� added by hseos 2007.12.12
		(m_initState & PLAYERINITSTATE_STORAGE_INFO) &&
		(m_initState & PLAYERINITSTATE_SHOPITEM_USEDINFO) &&
		(m_initState & PLAYERINITSTATE_OPTION_INFO))
	{
		//SW050907 �̻��� �ڵ� ���´�.
		//PWMODMGR_OBJ->Init_PeaceWarMode(this);
		SetPeaceMode( g_pServerSystem->GetMap()->IsVillage() );

		//---KES �ʿ���� �ڵ� ����
		//SW060718 ���̵� ���ȯ ���� �� ��ġ�̵�. SetInitedGrid() ����..
		//GetPetManager()->CheckSummonningPet();

		for(POSTYPE part = TP_WEAR_START ; part < TP_WEAR_END ; ++part )
		{
			const ITEMBASE * pTargetItemBase = ITEMMGR->GetItemInfoAbsIn(this, part);
			
			if( pTargetItemBase && 
				pTargetItemBase->dwDBIdx )
			{
				m_HeroCharacterInfo.WearedItemIdx[part-TP_WEAR_START] = pTargetItemBase->wIconIdx;
			}
			else
			{
				m_HeroCharacterInfo.WearedItemIdx[part-TP_WEAR_START] = 0;
			}
		}

		const ITEMBASE * pWeaponItemBase = ITEMMGR->GetItemInfoAbsIn(this, TP_WEAR_START + eWearedItem_Weapon );
		
		if( pWeaponItemBase && pWeaponItemBase->dwDBIdx )
		{
			const ITEM_OPTION& option = ITEMMGR->GetOption( *pWeaponItemBase );

			if( option.mItemDbIndex )
			{
				m_HeroCharacterInfo.WeaponEnchant = option.mEnchant.mLevel;
			}
		}

		// GuildTournament Observer
		//if( g_pServerSystem->GetMapNum() == GTMAPNUM && m_ObserverBattleIdx )
		//	m_HeroCharacterInfo.bVisible = FALSE;
		if( (g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetSiegeMapNum()) && 
			SIEGEWARMGR->IsProcessSiegewar() && m_ObserverBattleIdx )
			m_HeroCharacterInfo.bVisible = FALSE;

		// 080917 NYJ - DB���� ������ ��������� ������ CharacterTotalInfo�� �����Ѵ�.
		//              (Ŭ���̾�Ʈ�� �������� ���� ������ �Ϸ�Ǿ�� �Ѵ�.)
		memcpy(m_HeroCharacterInfo.HideParts, tempHideParts, sizeof(m_HeroCharacterInfo.HideParts));

		SEND_HERO_TOTALINFO msg;
		memset( &msg, 0, sizeof(msg) );
//		msg.InitOptionInfo();
		GetBaseObjectInfo(&msg.BaseObjectInfo);
		GetCharacterTotalInfo(&msg.ChrTotalInfo);
		GetHeroTotalInfo(&msg.HeroTotalInfo);

//		GetSkillTreeInfo(&msg.SkillTreeInfo);
		// MallItemInfo ���ٰ� �����ش�. �߰�
		DWORD getItem_Flag;
		if(protocol == MP_SURYUN_ENTER_SYN)
			getItem_Flag = GETITEM_FLAG_WEAR;
		else
			getItem_Flag = GETITEM_FLAG_INVENTORY | GETITEM_FLAG_WEAR | GETITEM_FLAG_PETWEAR;
		GetItemtotalInfo(&msg.ItemTotalInfo,getItem_Flag);
		GetSendMoveInfo(&msg.SendMoveInfo,NULL);
		// ShopItemOption �߰�
//		memcpy(&msg.ShopItemOption, GetShopItemStats(), sizeof(SHOPITEMOPTION));
		

		ASSERT(!(msg.SendMoveInfo.CurPos.wx == 0 && msg.SendMoveInfo.CurPos.wz == 0));
/*�����Ƽ ���� - 
		ABILITYMGR->InitAbilityExp(GetAbilityGroup(),m_HeroInfo.AbilityExp);
		ABILITYMGR->UpdateAbilityState(ABILITYUPDATE_CHARACTERLEVEL_CHANGED,msg.ChrTotalInfo.Level,GetAbilityGroup());

		ABILITYMGR->GetAbilityTotalInfo(GetAbilityGroup(),&msg.AbilityInfo);		
		*/
		msg.ChrTotalInfo.CurMapNum = GAMERESRCMNGR->GetLoadMapNum();
		msg.UniqueIDinAgent = GetUniqueIDinAgent();

		SKILL_BASE SkillTreeInfo[ 1024 ];
		WORD SkillNum = 0;

		m_SkillTree.SetPositionHead();
	
		while( SKILL_BASE* pSkill = m_SkillTree.GetSkillBase() )
		{
			memcpy( &SkillTreeInfo[ SkillNum ], pSkill, sizeof( SKILL_BASE ) );
			SkillNum++;

			if( SkillNum >= 1024 )
				break;
		}

		if( SkillNum )
		{
			msg.AddableInfo.AddInfo(CAddableInfoList::SkillTree, sizeof( SKILL_BASE ) * SkillNum, SkillTreeInfo);
			msg.SkillNum = SkillNum;
		}

		srand( GetTickCount());

		// desc_hseos_�ֹε��01
		// S �ֹε�� �߰� added by hseos 2007.06.04
		// ..DB���� �ε��� ������ Ŭ���̾�Ʈ�� ������ ���� ����
		msg.ChrTotalInfo.DateMatching = m_DateMatching;
		// E �ֹε�� �߰� added by hseos 2007.06.04

		//ĳ���� ���� �α��� �ð�
		GetLocalTime(&msg.ServerTime);
		
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_GAMEIN_ACK;

		// 071227 LUJ
		msg.StorageSize	= GetStorageNum();
		SendMsg( &msg, msg.GetMsgLength() );

		// 080328 NYJ --- �����ý���
		WEATHERMGR->SendWeather( this );

		// 080424 NYJ --- ���ü��õ�
		// 080602 LYW --- Player : ����ġ ��ġ (__int32) ���� (__int64) ������� ���� ó��.
		//MSG_DWORD_WORD msgFishingExp;
		//msgFishingExp.Category = MP_FISHING;
		//msgFishingExp.Protocol = MP_FISHING_EXP_ACK;
		//msgFishingExp.wData    = m_wFishingLevel;
		//msgFishingExp.dwData   = m_dwFishingExp;
		//SendMsg( &msgFishingExp, sizeof(msgFishingExp) );

		MSG_DWORDEX2 msgFishingExp;
		msgFishingExp.Category = MP_FISHING;
		msgFishingExp.Protocol = MP_FISHING_EXP_ACK;
		msgFishingExp.dweData1 = (DWORDEX)m_wFishingLevel;
		msgFishingExp.dweData2 = m_dwFishingExp;
		SendMsg( &msgFishingExp, sizeof(msgFishingExp) );

		// 080424 NYJ --- ���������Ʈ
		MSG_DWORD msgFishPoint;
		msgFishPoint.Category = MP_FISHING;
		msgFishPoint.Protocol = MP_FISHING_POINT_ACK;
		msgFishPoint.dwData   = m_dwFishPoint;
		SendMsg( &msgFishPoint, sizeof(msgFishPoint) );

		VECTOR3 pos;
		msg.SendMoveInfo.CurPos.Decompress(&pos);
//		g_Console.Log(eLogDisplay,4 ,"ConnectUser : %d %s, (x,z):(%f,%f)", 
//										GetID(), GetObjectName(), pos.x, pos.z);
		
		//KESA�ϡ������ˢ�
		MSG_WORD3 msgMapDesc;
		msgMapDesc.Category		= MP_USERCONN;
		msgMapDesc.Protocol		= MP_USERCONN_MAPDESC;
		msgMapDesc.wData1		= g_pServerSystem->GetMap()->IsVillage();
		msgMapDesc.wData2		= g_pServerSystem->GetMap()->IsPKAllow();
		msgMapDesc.wData3		= (WORD)GetCurChannel();
		SendMsg( &msgMapDesc, sizeof(msgMapDesc) );

		m_initState = PLAYERINITSTATE_ACKMSG_SEND;
	
		SERVERINFO* pserver = g_pServerTable->GetSelfServer();
		RegistLoginMapInfo(GetID(), GetObjectName(), (BYTE)GAMERESRCMNGR->GetLoadMapNum(), (WORD)pserver->wPortForServer);

		BYTE MapNum = (BYTE)GAMERESRCMNGR->GetLoadMapNum();
		if(MapNum == GTMAPNUM)
			MapNum = GTRETURNMAPNUM;
		RegistLoginMapInfo(GetID(), GetObjectName(), MapNum, (WORD)pserver->wPortForServer);

		m_initState = PLAYERINITSTATE_INITED;
		SetInited();
		
		if(protocol != MP_SURYUN_ENTER_SYN)
		{
			BOOL bNotifyUserLogin = TRUE;
			if(protocol != MP_SURYUN_LEAVE_SYN && 
				protocol != MP_USERCONN_GAMEIN_SYN)
				bNotifyUserLogin = FALSE;
			PARTYMGR->UserLogIn(this,bNotifyUserLogin);
			
			GUILDMGR->UserLogIn(this);			
		}

		WantedInfoByUserLogIn(GetID());

		QUICKMNGR->SendQuickInfo( this );

		// desc_hseos_���͹���01
		// S ���͹��� �߰� added by hseos 2007.05.29
		{
			MSG_DWORD2 msg;

			msg.Category	= MP_CHAR;
			msg.Protocol	= MP_CHAR_MONSTERMETER_PLAYTIME;
			msg.dwData1		= m_stMonstermeterInfo.nPlayTime;
			msg.dwData2		= m_stMonstermeterInfo.nPlayTimeTotal;
			SendMsg(&msg, sizeof(msg));

			msg.Category	= MP_CHAR;
			msg.Protocol	= MP_CHAR_MONSTERMETER_KILLMONSTER;
			msg.dwData1		= m_stMonstermeterInfo.nKillMonsterNum;
			msg.dwData2		= m_stMonstermeterInfo.nKillMonsterNumTotal;
			SendMsg(&msg, sizeof(msg));
		}
		// E ���͹��� �߰� added by hseos 2007.05.29

		// desc_hseos_����ý���_01
		// S ����ý��� �߰� added by hseos 2007.10.30
		// ..�� ���� �� �÷��̾� ���� ���� ����
		g_csFarmManager.SRV_SendPlayerFarmInfo(this);
		// E ����ý��� �߰� added by hseos 2007.10.30

		// desc_hseos_����Ʈ ��_01
		// S ����Ʈ �� �߰� added by hseos 2007.11.27
		g_csDateManager.SRV_SendChallengeZoneEnterFreq(this);
		// E ����Ʈ �� �߰� added by hseos 2007.11.27

		// desc_hseos_��ȥ_01
		// S ��ȥ �߰� added by hseos 2007.12.11
//		g_csMarriageManager.SRV_SendMarriageInfo(this);
		// E ��ȥ �߰� added by hseos 2007.12.11

		GUILDMGR->AddPlayer( this );
		GUILDWARMGR->AddPlayer( this );		

		// GuildTournament
		GTMGR->AddPlayer( this );
		// SiegeWar
		SIEGEWARMGR->AddPlayer( this );
		SWPROFITMGR->AddPlayer( this );

		PassiveSkillCheckForWeareItem();
	}
}

int CPlayer::CanExitStart()	//~����av����C
{
//	if( GetState() != eObjectState_None && GetState() != eObjectState_Move )
//		return FALSE;
	if( IsPKMode() )
		return eEXITCODE_PKMODE;
	if( LOOTINGMGR->IsLootedPlayer(GetID()) )	//PK����c����A; �ˡ�cCI�ˡ�A A�ϡ�AI�����ˢ�?
		return eEXITCODE_LOOTING;

	if( GetState() == eObjectState_Exchange )	//����E?A������ A����aCO ��o ������U.
		return eEXITCODE_NOT_ALLOW_STATE;

	if( GetState() == eObjectState_StreetStall_Owner ||
		GetState() == eObjectState_StreetStall_Guest )	//��eA��A������ A����aCO ��o ������U.
		return eEXITCODE_NOT_ALLOW_STATE;

	if( GetState() == eObjectState_Deal )	//��oA�� AI��eA������ A����aCO ��o ������U.
		return eEXITCODE_NOT_ALLOW_STATE;
	
	return eEXITCODE_OK;
}

void CPlayer::SetExitStart( BOOL bExit )
{
	m_bExit				= bExit;
	m_dwExitStartTime	= gCurTime;
}

int CPlayer::CanExit()
{
	DWORD lCurTime = MHTIMEMGR_OBJ->GetNewCalcCurTime();
	if( lCurTime - m_dwExitStartTime < EXIT_COUNT*1000 - 2000 )	//8.0	//���۸� ����� ����
		return eEXITCODE_SPEEDHACK;

	if( IsPKMode() )							//PK�ˡ��ϡ̡�iaAI�����ˢ�?
		return eEXITCODE_PKMODE;
	if( LOOTINGMGR->IsLootedPlayer(GetID()) )	//PK����c����A; �ˡ�cCI�ˡ�A A�ϡ�AI�����ˢ�?
		return eEXITCODE_LOOTING;

//---KES AUTONOTE
	if( GetAutoNoteIdx() )
		return eEXITCODE_NOT_ALLOW_STATE;
//---------------
	
	return eEXITCODE_OK;
}

void CPlayer::ExitCancel()
{
	if( IsExitStart() )
	{
		SetExitStart( FALSE );
		MSG_BYTE msgNack;
		msgNack.Category	= MP_CHAR;
		msgNack.Protocol	= MP_CHAR_EXIT_NACK;
		msgNack.bData		= eEXITCODE_DIEONEXIT;
		SendMsg(&msgNack, sizeof(msgNack));		
	}
}

int CPlayer::PKModeOn()
{
	if( IsPKMode() ) return ePKCODE_ALREADYPKMODEON;		//AI��oI PK�ˡ��ϡ̡�ia
	if( IsShowdown() ) return ePKCODE_SHOWDOWN;		//����n��o��iA�ϡ�AI�ˡ�e ��uE��iE�ˡ�U

	//pk������a A�Ƣ�e ����Au��oAA C��A|
	if( GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( this, eObjectState_Immortal );

	if( GetState() == eObjectState_Die )
		return ePKCODE_STATECONFLICT;	//�ˡ�U�ˡ���I��ioAAAI��O�����ˡ�A ��uE��iE�ˡ�U.
	
	m_HeroCharacterInfo.bPKMode = TRUE;
	m_dwPKModeStartTime			= gCurTime;

//---KES PK 071124
	m_dwPKContinueTime			= 20*60*1000 + ( GetBadFame() / 75 ) * 5*60*1000;	//�⺻ 30�� + �Ǹ�ġ 75���� 5��
//----------------

	return ePKCODE_OK;
}

BOOL CPlayer::PKModeOff()
{
	if( !IsPKMode() ) return FALSE;

	if( gCurTime - m_dwPKModeStartTime >= m_dwPKContinueTime || ( GetUserLevel() <= eUSERLEVEL_GM && PKMGR->IsPKEvent() ) )
	{
		m_HeroCharacterInfo.bPKMode = FALSE;
		SetPKModeEndtime();
		PKCharacterUpdate( GetID(), m_HeroInfo.LastPKModeEndTime );
		return TRUE;
	}

	return FALSE;
}

void CPlayer::PKModeOffForce()
{
	m_HeroCharacterInfo.bPKMode = FALSE;
}

void CPlayer::StateProcess()
{
	if( m_BaseObjectInfo.ObjectBattleState )
	{
		if( !mpBattleStateDelay->Check() )
		{
			SetObjectBattleState( eObjectBattleState_Peace );
		}
	}


	if( !mbBuffSkillLoad )
	{
		CharacterBuffLoad( GetID() );
		mbBuffSkillLoad = TRUE;
	}

	if( !mbPetLoad )
	{
		PetInfoLoad( GetID(), GetUserID() );
		mbPetLoad = TRUE;
	}

	switch( GetState() )
	{
	case eObjectState_Die:
		{
			//---KES PK 071202	���� ��� �ð��� ��� ���� (�ð��� �Ȱ�����)
			SetPKStartTimeReset();
			//----------------

			if( m_bNeedRevive )
			{
				DWORD dwElapsed = gCurTime - m_ObjectState.State_Start_Time;

				if( dwElapsed > PLAYERREVIVE_TIME )
				{
					//if( m_bDieInSpecialMap )	// quest map
					//{
					//	SendPlayerToMap();
					//	break;
					//}
					// ��������� ����� ��� �Ϲ����� ������� �ٸ� �޽����� �������Ѵ�.
					/*else*/ if( m_bDieForGFW )
					{
						MSGBASE message;
						message.Category	= MP_USERCONN;
						message.Protocol	= MP_USERCONN_READY_TO_REVIVE_BY_GFW;
						
						SendMsg( &message, sizeof( message ) );

						m_bNeedRevive = FALSE;

						break;
					}

					if( LOOTINGMGR->IsLootedPlayer( GetID() ) )
						break;
					if( IsReadyToRevive() != TRUE )
						break;
					
					MSGBASE msg;
					msg.Category = MP_USERCONN;
					msg.Protocol = MP_USERCONN_READY_TO_REVIVE;
					SendMsg( &msg, sizeof(msg) );

					m_bNeedRevive = FALSE;

/*					if( g_pServerSystem->GetMapNum() == BOSSMONSTER_MAP ||
						g_pServerSystem->GetMapNum() == BOSSMONSTER_2NDMAP )	// BossMap
					{
						m_bDieInSpecialMap = TRUE;
						m_ObjectState.State_Start_Time = gCurTime;
						break;
					}
*/				}
			}

			if( m_bDieInSpecialMap )	// BossMap
			{
/*				if( g_pServerSystem->GetMapNum() == BOSSMONSTER_MAP ||
					g_pServerSystem->GetMapNum() == BOSSMONSTER_2NDMAP )	// BossMap
				if( dwElapsed > 60000 )
				{
					m_bDieInSpecialMap = TRUE;
					m_wReturnMapNum = 12;
					m_bNeedRevive = TRUE;
					break;
				}
*/			}
		}
		break;
	}

	//in Korea : per 10min
	//in China : per 30min
	DWORD dwInsertLogTime = INSERTLOG_TIME;

	if( GetInited() )
	if( gCurTime - m_dwProgressTime >= dwInsertLogTime )
	{
		// 080716 LUJ, �� �α�
		{
			const PET_OBJECT_INFO* info = GetPetInfo( GetSummonWaitPet() );

			if( info )
			{
				LogPet( *info, ePetLogRegular );
			}
		}

		InsertLogExp( eExpLog_Time, GetID(), GetLevel(), 0, GetPlayerExpPoint(), 0, 0, 0/*�����Ƽ ���� - GetPlayerAbilityExpPoint()*/);
		InsertLogMoney(0, GetID(), GetMoney(), 0, GetUserID(), GetMoney(eItemTable_Storage));
		
		if( IsPKMode() ) m_HeroInfo.LastPKModeEndTime = 0;
		// ��oA������I�������� ��i���� ��o���� �����Ϣ�CeA�ˢ� AuAa		
		UpdateCharacterInfoByTime(GetID(), GetPlayerExpPoint(), /*�����Ƽ ���� - GetPlayerAbilityExpPoint(),*/ GetMoney(), m_HeroInfo.Playtime, m_HeroInfo.LastPKModeEndTime );
		
/*		for(int i=0; i<SLOT_SKILLTOTAL_NUM;++i)
		{
			if( m_HeroSkillInfo.skill[i].dwDBIdx != 0 )
			{
				// ��o��i�����Ϩ� d�����ˡ� ����I��������
				InsertLogSkillExp(GetID(), 0, m_HeroSkillInfo.skill[i].wIconIdx, m_HeroSkillInfo.skill[i].dwDBIdx, 
									m_HeroSkillInfo.skill[i].Level, m_HeroSkillInfo.skill[i].ExpPoint, m_HeroSkillInfo.skill[i].Position );

				// ��oA������I�������� ��o��i�����Ϩ� d�����ˡ� AuAa
				UpdateSkillExpByTime(GetID(), m_HeroSkillInfo.skill[i].dwDBIdx, m_HeroSkillInfo.skill[i].ExpPoint);				
			}
		}*/
		// DB�� ����
		MonsterMeter_Save(GetID(), m_stMonstermeterInfo.nPlayTime, m_stMonstermeterInfo.nKillMonsterNum, m_stMonstermeterInfo.nPlayTimeTotal, m_stMonstermeterInfo.nKillMonsterNumTotal);

		// �������� DB�� ����
		FishingData_Update(GetID(), GetFishingLevel(), GetFishingExp(), GetFishPoint());

		m_dwProgressTime = gCurTime;
	}

	// Play Time
	// ��iA��CC ��cCA��좥A��A ��u��I������a��A��I.. ��A AOA����I��i ���ϡ�a���� CIAU.. �������� RaMa
	m_ContinuePlayTime += gTickTime;
	DWORD dwPT = m_ContinuePlayTime/1000;
	if(dwPT)
	{
		m_HeroInfo.Playtime += dwPT;
		m_ContinuePlayTime -= dwPT*1000;
	}

	AddBadFameReduceTime();	
//	m_ShopItemManager.CheckEndTime();

	// 071115 LYW --- Player : ���� ������ ���� �߰�.
	//m_ChargeItemManager.CheckEndTime() ;


	// ��Ȱ�ֹ��� ���� �ð�üũ
	if( m_ReviveTime )
	{
		m_ReviveTime -= gTickTime;

		MSG_WORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_NACK;

		CPlayer* pTarget  = (CPlayer*)g_pUserTable->FindUser(m_ReviveData.TargetID);
		if(!pTarget)
		{
			msg.wData = eShopItemErr_Revive_Fail;
			SendMsg( &msg, sizeof(msg) );

			m_ReviveTime = 0;
			memset( &m_ReviveData, 0, sizeof(m_ReviveData) );
		}				
		else if( m_ReviveTime <= 0 )
		{
			msg.wData = eShopItemErr_Revive_TimeOver;
			SendMsg( &msg, sizeof(msg) );
			pTarget->SendMsg( &msg, sizeof(msg) );
			
			m_ReviveTime = 0;
			memset( &m_ReviveData, 0, sizeof(m_ReviveData) );
		}
		else if( GetChannelID() != pTarget->GetChannelID() )
		{
			msg.wData = eShopItemErr_Revive_Fail;
			SendMsg( &msg, sizeof(msg) );
			pTarget->SendMsg( &msg, sizeof(msg) );

			m_ReviveTime = 0;
			memset( &m_ReviveData, 0, sizeof(m_ReviveData) );

			/*
			VECTOR3 mypos, targetpos;
			GetPosition( &mypos );
			pTarget->GetPosition( &targetpos );
			if( CalcDistanceXZ( &mypos, &targetpos ) > (float)(REVIVE_OTHER_DIST + 4000) )	//50m����
			{
				msg.wData = eShopItemErr_Revive_TooFar;
				SendMsg( &msg, sizeof(msg) );	
				pTarget->SendMsg( &msg, sizeof(msg) );

				m_ReviveTime = 0;
				memset( &m_ReviveData, 0, sizeof(m_ReviveData) );
			}*/
		}
	}

	// desc_hseos_���͹���01
	// S ���͹��� �߰� added by hseos 2007.05.23
	ProcMonstermeterPlayTime();
	// E ���͹��� �߰� added by hseos 2007.05.23

	// desc_hseos_�ֹε��01
	// S �ֹε�� �߰� added by hseos 2007.06.09
	g_csResidentRegistManager.SRV_ProcDateMatchingChatTimeOut(this);
	// E �ֹε�� �߰� added by hseos 2007.06.09

	// desc_hseos_����ý���_01
	// S ����ý��� �߰� added by hseos 2007.08.23
	ProcFarmTime();
	// E ����ý��� �߰� added by hseos 2007.08.23

	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2007.11.27	2007.11.28
	g_csDateManager.SRV_Process(this);
	// E ����Ʈ �� �߰� added by hseos 2007.11.27	2007.11.28

	ProcCoolTime();

}

void CPlayer::SetWearedItemIdx(DWORD WearedPosition,DWORD ItemIdx)
{	
	const size = sizeof( m_HeroCharacterInfo.WearedItemIdx ) / sizeof( DWORD );

	if( size > WearedPosition )
	{
		m_HeroCharacterInfo.WearedItemIdx[WearedPosition] = ItemIdx;
	}
	else
	{
		ASSERT( 0 );
	}	
}

void CPlayer::AddWanted(WANTEDTYPE WantedIDX)
{
	m_WantedList.AddTail((void*)WantedIDX);	
}

void CPlayer::DeleteWanted(WANTEDTYPE WantedIDX)
{
	m_WantedList.Remove((void*)WantedIDX);
}

void CPlayer::ClearWantedList()
{
	m_WantedList.RemoveAll();
}

BOOL CPlayer::IsWantedOwner(WANTEDTYPE WantedIDX)
{
	PTRLISTSEARCHSTART(m_WantedList, WANTEDTYPE, id)
		if( id == WantedIDX )
		{
			return TRUE;
		}
	PTRLISTSEARCHEND
		return FALSE;
}

// jsd quest
/*void CPlayer::AddQuestScript( DWORD dwQuestIdx )
{
	m_pQuest = new CQuestScript;
//	*m_pQuest = *TESTQUESTMNG->GetQuest( dwQuestIdx );	
}

BOOL CPlayer::CheckQuestEnd( DWORD dwMonsterIdx )
{
	if( m_pQuest )
	if( m_pQuest->m_QuestCondition.dwMonsterIdx == dwMonsterIdx )
	if( m_pQuest->m_QuestCondition.dwMonsterNum )
	{
		--m_pQuest->m_QuestCondition.dwMonsterNum;
		MSG_DWORD2 msg;
		msg.Category = MP_QUEST;
		msg.Protocol = MP_QUEST_TEST_RUN_ACK;
		msg.dwObjectID = GetID();
		msg.dwData1 = m_pQuest->m_QuestInfo.dwQuestIdx;
		msg.dwData2 = m_pQuest->m_QuestCondition.dwMonsterNum;
		SendMsg( &msg, sizeof(msg) );
		return TRUE;
	}
	return FALSE;
}
*/

void CPlayer::QuestProcess()
{
	m_QuestGroup.Process();
//	if( m_pQuestGroup )
//		m_pQuestGroup->Process();
}


void CPlayer::SetPKModeEndtime()
{
	//CA����AI��A�ơ�A����I ������ACN��U.
/*	
	m_HeroInfo.LastPKModeEndTime = GetPlayTime();
	m_dwLastReduceHour = 0;

	//DB Update
	PKCharacterUpdate( GetID(), m_HeroInfo.LastPKModeEndTime );
*/
	
	m_HeroInfo.LastPKModeEndTime = 0;
}


void CPlayer::AddBadFameReduceTime()
{
	if( g_pServerSystem->GetMap()->IsVillage() )	return;
	if( GetState() == eObjectState_Immortal )		return;
	if( IsPKMode() )								return;
	if( GetBadFame() == 0 && GetWantedIdx() == 0 )	return;

	m_HeroInfo.LastPKModeEndTime += gTickTime;

	//---KES PK 071202
	if( m_HeroInfo.LastPKModeEndTime > 90*60*1000 )
	{
		m_HeroInfo.LastPKModeEndTime -= 30*60*1000;	//
		PKCharacterUpdate( GetID(), m_HeroInfo.LastPKModeEndTime );

        if( GetBadFame() > 5 )
		{
			SetBadFame( GetBadFame()-5 );
		}
		else
		{
			SetBadFame( 0 );
			m_HeroInfo.LastPKModeEndTime = 0;
		}

		BadFameCharacterUpdate( GetID(), GetBadFame() );
		FAMEMGR->SendBadFameMsg( this, GetBadFame() );
	}


/*
	if( GetBadFame() == 0 ) //�������  
	{
		if(m_HeroInfo.LastPKModeEndTime > WANTED_DESTROYED_TIME)
		{
			if(GetWantedIdx() != 0 )
			{	
				WANTEDMGR->DestroyWanted(this);
			}
		}     
	}
	else if( m_HeroInfo.LastPKModeEndTime > 3*3600*1000 )
	{
		m_HeroInfo.LastPKModeEndTime -= 3600*1000;
		PKCharacterUpdate( GetID(), m_HeroInfo.LastPKModeEndTime );

		SetBadFame( GetBadFame()-1 );
		if(GetBadFame() == 0)
			m_HeroInfo.LastPKModeEndTime = 0;
		BadFameCharacterUpdate( GetID(), GetBadFame() );
		FAMEMGR->SendBadFameMsg( this, GetBadFame() );
	}
*/
}



void CPlayer::SpeedHackCheck()
{
//	if( m_dwHackStartTime == 0 )
//		m_dwHackStartTime = gCurTime;

	++m_nHackCount;

	if( gCurTime - m_dwHackStartTime >= 60*1000 )	//1��
	{
		if( m_nHackCount >= g_nHackCheckWriteNum )
		{
			InsertSpeedHackCheck( GetID(), GetObjectName(), m_nHackCount, 1 );
		}
		if( m_nHackCount >= g_nHackCheckNum )
		{
			MSGBASE Msg;
			Msg.Category = MP_HACKCHECK;
			Msg.Protocol = MP_HACKCHECK_BAN_USER_TOAGENT;
			SendMsg( &Msg, sizeof(Msg) );
		}
		m_nHackCount		= 1;
		m_dwHackStartTime	= gCurTime;
	}
}

void CPlayer::ClearMurderIdx()
{
	SetReadyToRevive(FALSE);
	m_MurdererKind = 0;
	m_MurdererIDX = 0;
	m_MurderObjKind = 0;
	m_bPenaltyByDie = FALSE;
}


DWORD CPlayer::Damage(CObject* pAttacker,RESULTINFO* pDamageInfo)
{
	DWORD life = GetLife();
	DWORD beforelife = life;
	// TEST
	//pDamageInfo->RealDamage = 1;
	/*if( pAttacker->GetObjectKind() == eObjectKind_Player)
	{
		float damage = 0;
		damage = (pDamageInfo->RealDamage * (ATTACKMGR->GetPvpDamageRatio() / 100 ));
		pDamageInfo->RealDamage = (DWORD)damage;
	}*/
	if(life > pDamageInfo->RealDamage)
	{
		life -= pDamageInfo->RealDamage;
	}
	else
	{
		if( GetUserLevel() == eUSERLEVEL_GM || m_God ) // && g_pServerSystem->GetNation() == eNATION_KOREA )
		{
			life = 1;		//gm�� �������� �޾Ƶ� ���� �ʵ���
		}
		else
		{
			life = 0;
		}
	}
	
	SetLife(life,FALSE);

	DoDamage(pAttacker,pDamageInfo,beforelife);

	return life;
}

void CPlayer::SetGuildInfo(DWORD GuildIdx, BYTE Grade, const char* GuildName, MARKNAMETYPE MarkName)
{
	m_HeroCharacterInfo.MunpaID = GuildIdx;
	m_HeroCharacterInfo.PositionInMunpa = Grade;
	SafeStrCpy(m_HeroCharacterInfo.GuildName, GuildName, sizeof( m_HeroCharacterInfo.GuildName ) );
	m_HeroCharacterInfo.MarkName = MarkName;
}

void CPlayer::RSetMoney( MONEYTYPE money, BYTE flag )
{
	CPurse* pPurse = m_ItemContainer.GetPurse(eItemTable_Inventory);
	if( !pPurse ) return;

	pPurse->RSetMoney( money, flag );
	m_HeroInfo.Money = money;
}

void CPlayer::SetGuildMarkName(MARKNAMETYPE MarkName)
{
	m_HeroCharacterInfo.MarkName = MarkName;
}

char* CPlayer::GetGuildCanEntryDate()
{ 
	return m_HeroInfo.MunpaCanEntryDate;
}

void CPlayer::SetGuildCanEntryDate(char* date)
{
	SafeStrCpy(m_HeroInfo.MunpaCanEntryDate, date, 11);
}

void CPlayer::ClrGuildCanEntryDate()
{
	SafeStrCpy(m_HeroInfo.MunpaCanEntryDate, "2004.01.01", 11);
}

void CPlayer::SetFamilyInfo(DWORD FamilyIdx, BYTE Grade, char* FamilyName, MARKNAMETYPE MarkName)
{
	m_HeroCharacterInfo.FamilyID = FamilyIdx;
	m_HeroCharacterInfo.PositionInFamily = Grade;
	SafeStrCpy(m_HeroCharacterInfo.FamilyName, FamilyName, sizeof( m_HeroCharacterInfo.FamilyName ) );
	m_HeroCharacterInfo.FamilyMarkName = MarkName;
}

void CPlayer::SetFamilyMarkName(MARKNAMETYPE MarkName)
{
	m_HeroCharacterInfo.FamilyMarkName = MarkName;
}

char* CPlayer::GetFamilyCanEntryDate()
{ 
	return m_HeroInfo.FamilyCanEntryDate;
}

void CPlayer::SetFamilyCanEntryDate(char* date)
{
	SafeStrCpy(m_HeroInfo.FamilyCanEntryDate, date, 11);
}

void CPlayer::ClrFamilyCanEntryDate()
{
	SafeStrCpy(m_HeroInfo.FamilyCanEntryDate, "2004.01.01", 11);
}

void CPlayer::SendShopItemInfo()
{
	SEND_SHOPITEM_INFO ShopItem;
	ShopItem.Category = MP_ITEM;
	ShopItem.Protocol = MP_ITEM_SHOPITEM_INFO_ACK;	
	ShopItem.ItemCount = 0;


	POSTYPE startpos = TP_SHOPITEM_START;

	for(int i=0; i<SLOT_SHOPITEM_IMSI; i++)
	{
		if( m_ShopItemSlot.IsEmpty(startpos+i) == FALSE )
		{
			ShopItem.Item[ShopItem.ItemCount] = *m_ShopItemSlot.GetItemInfoAbs(startpos+i);
			// 071126 KTH --- �켱 ������� �ʾƼ� �ּ�
/*			if( ShopItem.Item[ShopItem.ItemCount].ItemParam & ITEM_PARAM_PRESENT )
			{
				((ITEMBASE*)m_ShopItemSlot.GetItemInfoAbs(startpos+i))->ItemParam ^= ITEM_PARAM_PRESENT;				
				ShopItemParamUpdateToDB( GetID(), ShopItem.Item[ShopItem.ItemCount].dwDBIdx, m_ShopItemSlot.GetItemInfoAbs(startpos+i)->ItemParam );
			}*/
			++ShopItem.ItemCount;
		}
	}

	SendMsg(&ShopItem, ShopItem.GetSize());
}


void CPlayer::UpdateLogoutToDB(BOOL val)
{
//	m_ShopItemManager.UpdateLogoutToDB();

	// 071115 LYW --- Player : ���� ������ ���� �߰�.
	//m_ChargeItemManager.UpdateLogoutToDB() ;

	if( val )
	{
		//SetMapMoveInfo( GAMERESRCMNGR->GetLoadMapNum(), (DWORD)(m_MoveInfo.CurPosition.x), (DWORD)(m_MoveInfo.CurPosition.z) );
		//CharacterLogoutPointUpdate( GetID(), GAMERESRCMNGR->GetLoadMapNum(), (DWORD)(m_MoveInfo.CurPosition.x), (DWORD)(m_MoveInfo.CurPosition.z) );	
	}
	else
	{
		if( mpCurPet )
		{
			mpCurPet->SetPetObjectState( ePetState_Summon );
		}
	}
	for( int i = 0; i < 8; i++ )
	{
		m_QuickSlot[i].DBUpdate();
	}
}


BOOL CPlayer::IsAbleReviveOther()
{
	if(GetState() != eObjectState_Die)
	{
		ASSERT(0);
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 1; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return FALSE;
	}

	if( LOOTINGMGR->IsLootedPlayer( GetID() ) )	//��c��AA������
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 2; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return FALSE;
	}
	
	if( IsExitStart() )
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 4; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return FALSE;
	}

	return TRUE;
}


BOOL CPlayer::ReviveShopItem()
{
	MOVE_POS msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTER_REVIVE;
	msg.dwObjectID = GetID();
	msg.dwMoverID = GetID();
	msg.cpos.Compress(CCharMove::GetPosition(this));
	PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
	

/*	if(GetLevel() >= 5)
	{
		// �� ������  0.04->0.02
		CItemSlot* pSlot = m_ItemContainer.GetSlot(eItemTable_Inventory);
		DWORD CurMoney = pSlot->GetMoney();
		SetMoney( (MONEYTYPE)(CurMoney*0.02), MONEY_SUBTRACTION, MF_LOST , eItemTable_Inventory, eMoneyLog_LoseDie, 0 );

		// ����ġ ������..  0.02->0.01
		EXPTYPE pointForLevel = GAMERESRCMNGR->GetMaxExpPoint(GetLevel());
		EXPTYPE minusExp = (EXPTYPE)(pointForLevel * 0.01);
		ReduceExpPoint(minusExp, eExpLog_LosebyReviveReviveOther);
	}*/
//	InsertLogExp( eExpLog_ReviveExp, GetID(), GetLevel(), GetShopItemStats()->ProtectCount, GetPlayerExpPoint(), m_MurdererKind, m_MurdererIDX, 0/*�����Ƽ ���� - GetPlayerAbilityExpPoint()*/);

	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die);
	
	DWORD CurLife = GetMaxLife();
	DWORD CurMana = GetMaxMana();
	SetLife((DWORD)(CurLife));
	SetMana((DWORD)(CurMana));

	m_YYLifeRecoverTime.bStart = FALSE;
	m_YYManaRecoverTime.bStart = FALSE;
	ClearMurderIdx();


	MSG_DWORD2	msg1;
	msg1.Category = MP_ITEM;
	msg1.Protocol = MP_ITEM_SHOPITEM_EFFECT;				
	msg1.dwData1 = GetID();
	msg1.dwData2 = eEffect_ShopItem_Revive;
	PACKEDDATA_OBJ->QuickSend( this, &msg1, sizeof(msg1) );

	m_bDieForGFW = FALSE;
	m_bDieInSpecialMap = FALSE;

	return TRUE;
}

void CPlayer::ReviveVillage()
{
	if(GetState() != eObjectState_Die)
	{
		ASSERT(0);
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 1; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return;
	}

	//---KES PK 071202
	if( IsPKMode() || LOOTINGMGR->IsLootedPlayer( GetID() ) )
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 2; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return;
	}

	if( IsExitStart() )
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 4; //errorcode
		SendMsg( &msg, sizeof(msg) );

		return;
	}

	if( !EVENTMAPMGR->IsEventMap() )
	if(GetLevel() >= 5 && m_bDieForGFW == FALSE )
	{
		// RaMa -> ����ġ+����ȣ
		DWORD ItemIdx = 0;
		SHOPITEMWITHTIME* pShopItem = NULL;
/*		if( GetShopItemStats()->ProtectCount )		
		{
			DWORD ItemIdx = m_ShopItemManager.GetProtectItemIdx();
			if( ItemIdx == 0 )
				goto ProtectAll_UseFail;
			pShopItem = m_ShopItemManager.GetUsingItemInfo( ItemIdx );
			if( !pShopItem )
				goto ProtectAll_UseFail;

			// Log�߰�
			LogItemMoney( GetID(), GetObjectName(), 0, "",
				eLog_ShopItemProtectAll, GetMoney(eItemTable_Inventory), 0, 0,
				ItemIdx, pShopItem->ShopItem.ItemBase.dwDBIdx, pShopItem->ShopItem.ItemBase.Position, 0, pShopItem->ShopItem.ItemBase.Durability, GetPlayerExpPoint());
			InsertLogExp( eExpLog_ProtectExp, GetID(), GetLevel(), GetShopItemStats()->ProtectCount, GetPlayerExpPoint(), m_MurdererKind, m_MurdererIDX, 0) ;//�����Ƽ ���� - GetPlayerAbilityExpPoint());

			--GetShopItemStats()->ProtectCount;
			if( GetShopItemStats()->ProtectCount <= 0 )
			{
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_USEEND, ItemIdx);
				m_ShopItemManager.DeleteUsingShopItem( ItemIdx );
				m_ShopItemManager.SetProtectItemIdx( 0 );
			}
			else
			{
				// DB�� ������Ʈ
				ShopItemUseParamUpdateToDB( 0, GetID(), ItemIdx, GetShopItemStats()->ProtectCount );
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_PROTECTALL, GetShopItemStats()->ProtectCount);
			}
		}
		else
*/		{
//ProtectAll_UseFail:
			// RaMa -> ����ȣ �ֹ���
/*			if( m_ShopItemManager.GetUsingItemInfo( eIncantation_MoneyProtect ) )
			{
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_MONEYPROTECT, eIncantation_MoneyProtect);
				m_ShopItemManager.DeleteUsingShopItem( eIncantation_MoneyProtect );
			}
			else
*/			{
				CItemSlot* pSlot = m_ItemContainer.GetSlot(eItemTable_Inventory);
				DWORD CurMoney = pSlot->GetMoney();
				if( !m_bDieForGFW )
					SetMoney((MONEYTYPE)(CurMoney*0.04), MONEY_SUBTRACTION, MF_LOST, eItemTable_Inventory, eMoneyLog_LoseDie, 0);
			}
			
			// RaMa -> ����ġ��ȣ �ֹ���
/*			if( m_ShopItemManager.GetUsingItemInfo( eIncantation_ExpProtect ) )
			{
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_EXPPROTECT, eIncantation_ExpProtect);
				m_ShopItemManager.DeleteUsingShopItem( eIncantation_ExpProtect );
			}
			else
*/			{
				EXPTYPE pointForLevel = GAMERESRCMNGR->GetMaxExpPoint(GetLevel());
				EXPTYPE minusExp = 0;

				if( !m_bDieForGFW )	minusExp = (EXPTYPE)(pointForLevel * 0.01);

				ReduceExpPoint(minusExp, eExpLog_LosebyRevivePresent);
			}
		}
	}

	if(GetState() == eObjectState_Die)
		OBJECTSTATEMGR_OBJ->EndObjectState( this ,eObjectState_Die );

	RevivePenalty(FALSE) ;

	MSG_DWORD msg;
	// 070125 LYW --- Revival : Modified category and protocol and map number.
	/*
	msg.Category = MP_SIEGEWAR;
	msg.Protocol = MP_SIEGEWAR_RETURNTOMAP;
	msg.dwData = GetReturnMapNum();
	*/
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHANGEMAP_ACK;
	CHARACTER_TOTALINFO totalInfo ;
	GetCharacterTotalInfo(&totalInfo) ;
	msg.dwData = totalInfo.CurMapNum ;

	SendMsg( &msg, sizeof(msg) );

	g_pServerSystem->RemovePlayer( GetID() );
}

void CPlayer::SetNickName(char* NickName)
{
	SafeStrCpy(m_HeroCharacterInfo.NickName,NickName, MAX_GUILD_NICKNAME+1);
}

void CPlayer::SetFamilyNickName(char* NickName)
{
	SafeStrCpy(m_HeroCharacterInfo.FamilyNickName,NickName, sizeof( m_HeroCharacterInfo.FamilyNickName ) );
}

BOOL CPlayer::CanSocietyAction( DWORD curTime )
{
	DWORD dwElapsedTime = curTime - m_dwLastSocietyAction;

	if( dwElapsedTime > 2000 )	//2��
	{
		m_dwLastSocietyAction = curTime;
		return TRUE;
	}

	return FALSE;
}

LEVELTYPE CPlayer::GetLevel()
{ 
	return m_HeroCharacterInfo.Level; 
}

DWORD CPlayer::GetLife() 
{ 
	return m_HeroCharacterInfo.Life; 
}

DWORD CPlayer::GetMana()
{ 
	return m_HeroInfo.Mana; 
}

DWORD CPlayer::DoGetMaxLife()
{ 
	return m_HeroCharacterInfo.MaxLife; 
}

DWORD CPlayer::DoGetMaxMana()
{ 
	return m_HeroInfo.MaxMana; 
}

float	CPlayer::GetMaxLifeWithItem()
{
	return mMaxLife;
}

float	CPlayer::GetMaxManaWithItem()
{
	return mMaxMana;
}

DWORD CPlayer::DoGetPhyDefense()
{ 
	float plus = m_charStats.mPhysicDefense.mPlus + m_itemStats.mPhysicDefense.mPlus;

	plus += ( plus * m_itemStats.mPhysicDefense.mPercent );

	return DWORD( max( 1, plus ) );
}

float CPlayer::DoGetAttDefense(WORD Attrib)
{ 
	return 0/*�����Ƽ ���� - +GetAbilityStats()->AttRegistUp.GetElement_Val(Attrib)*/; 
}	

float CPlayer::GetAttribPlusPercent(WORD Attrib)
{ 
	return 0/*�����Ƽ ���� - +GetAbilityStats()->AttAttackUp.GetElement_Val(Attrib)*/;	
}

// 070415 LYW --- Player : Active and modified functions to setting and return stage.
//void CPlayer::SetStage( BYTE stage )
// 070416 LYW --- Player ; Modified function SetStage.
void CPlayer::SetStage( BYTE grade, BYTE index )
{
	// 070415 LYW --- Player : Modified function to setting stage.
	/*
	if( m_HeroCharacterInfo.Stage == stage )	return;
	if( stage == eStage_Normal )
	{
		m_HeroCharacterInfo.Stage = stage;

		MSG_BYTE Msg;
		Msg.Category = MP_CHAR;
		Msg.Protocol = MP_CHAR_STAGE_NOTIFY;
		Msg.dwObjectID = GetID();
		Msg.bData = stage;
		PACKEDDATA_OBJ->QuickSend( this, &Msg, sizeof(Msg) );

		CharacterTotalInfoUpdate( this );	
	}
	else
	{
//		if( m_HeroCharacterInfo.Stage == eStage_Normal )
		{
			m_HeroCharacterInfo.Stage = stage;
			
			MSG_BYTE Msg;
			Msg.Category = MP_CHAR;
			Msg.Protocol = MP_CHAR_STAGE_NOTIFY;
			Msg.dwObjectID = GetID();
			Msg.bData = stage;
			PACKEDDATA_OBJ->QuickSend( this, &Msg, sizeof(Msg) );

			CharacterTotalInfoUpdate( this );	
		}
		*/
//		else
//		{
//			if( !(m_HeroCharacterInfo.Stage & stage) )	return;

//			m_HeroCharacterInfo.Stage = stage;
			
//			MSG_BYTE Msg;
//			Msg.Category = MP_CHAR;
//			Msg.Protocol = MP_CHAR_STAGE_NOTIFY;
//			Msg.dwObjectID = GetID();
//			Msg.bData = stage;
//			PACKEDDATA_OBJ->QuickSend( this, &Msg, sizeof(Msg) );

//			CharacterTotalInfoUpdate( this );
//		}
	//}

	// 06. 03 �������� - �̿���
	//������ �����ִ�����Ʈ ����
	//SetMussangMaxPoint();

	//if( m_HeroCharacterInfo.JobGrade == stage )	return;
	
	/*
	m_HeroCharacterInfo.JobGrade = stage;

	MSG_BYTE Msg;
	Msg.Category = MP_CHAR;
	Msg.Protocol = MP_CHAR_STAGE_NOTIFY;
	Msg.dwObjectID = GetID();
	Msg.bData = stage;
	PACKEDDATA_OBJ->QuickSend( this, &Msg, sizeof(Msg) );

	CharacterTotalInfoUpdate( this );	
	*/

	MSG_BYTE2 msg ;

	msg.Category	= MP_CHAR ;
	msg.Protocol	= MP_CHAR_STAGE_NOTIFY ;
	msg.dwObjectID	= GetID() ;
	msg.bData1		= grade ;
	msg.bData2		= index ;
	PACKEDDATA_OBJ->QuickSend( this, &msg, sizeof(msg) );

	CharacterTotalInfoUpdate( this );	
}

// 070415 LYW --- Plsyer : Add function to setting job.
void CPlayer::SetJob( BYTE jobGrade, BYTE jobIdx )
{
	MSG_BYTE2 msg ;

	msg.Category	= MP_CHAR ;
	msg.Protocol	= MP_CHAR_STAGE_NOTIFY ;
	msg.dwObjectID	= GetID() ;
	msg.bData1		= jobGrade ;
	msg.bData2		= jobIdx ;

	// 070522 LYW --- Player : Modified send job notify.
	//PACKEDDATA_OBJ->QuickSend( this, &msg, sizeof(msg) );
	SendMsg( &msg, sizeof(msg) );

	m_HeroCharacterInfo.JobGrade = jobGrade ;
	m_HeroCharacterInfo.Job[jobGrade - 1] = jobIdx ;

	CharacterJobUpdate( GetID(), jobGrade,
						m_HeroCharacterInfo.Job[0],
						m_HeroCharacterInfo.Job[1],
						m_HeroCharacterInfo.Job[2],
						m_HeroCharacterInfo.Job[3],
						m_HeroCharacterInfo.Job[4],
						m_HeroCharacterInfo.Job[5] );


	// 071112 ����, Ŭ���� �α׸� �����
	InsertLogJob( this, m_HeroCharacterInfo.Job[0], jobGrade, jobIdx );

	// 080225 LUJ, ��� ȸ���� ��� ���� ���� ������ �� ������ �����ؾ��Ѵ�
	{
		CGuild* guild = GUILDMGR->GetGuild( GetGuildIdx() );

		if( ! guild )
		{
			return;
		}

		GUILDMEMBERINFO* member = guild->GetMemberInfo( GetID() );

		if( ! member )
		{
			return;
		}

		member->mJobGrade	= jobGrade;
		member->mRace		= m_HeroCharacterInfo.Race;
		memcpy( member->mJob, m_HeroCharacterInfo.Job, sizeof( member->mJob ) );

		SEND_GUILD_MEMBER_INFO message;
		message.Category	= MP_GUILD;
		message.Protocol	= MP_GUILD_SET_MEMBER_TO_MAP;
		message.GuildIdx	= GetGuildIdx();
		message.MemberInfo	= *member;

		g_Network.Send2AgentServer( ( char* )&message, sizeof( message ) );

		GUILDMGR->NetworkMsgParse( message.Protocol, &message );
	}	
}
/*
void CPlayer::DoDie(CObject* pAttacker)
{
//KES EXIT
	ExitCancel();
//KES EXCHANGE 031002
	EXCHANGEMGR->CancelExchange( this );
	STREETSTALLMGR->UserLogOut( this );
//KES PKLOOTING
	LOOTINGMGR->LootingCancel( this );

	OBJECTSTATEMGR_OBJ->StartObjectState(this,eObjectState_Die,pAttacker->GetID());
	
	if(pAttacker->GetObjectKind() == eObjectKind_Player)
	{
		CPlayer* pAttackPlayer = (CPlayer*)pAttacker;
		CBattle* pBattle = pAttacker->GetBattle();
		if(pBattle->GetBattleKind() == eBATTLE_KIND_NONE || pAttacker->GetBattleID() != GetBattleID() )
		{
			if( GUILDWARMGR->JudgeGuildWar( this, (CPlayer*)pAttacker ) == FALSE )
			{
				//PKLooting!
				//IsLootingSituation����������A ��n����A��AIAo ��E��c ��ECN��U. ������������ ���ϡ�a��U�Ƣ� ��O��u��U.
				if( LOOTINGMGR->IsLootingSituation( this, pAttacker ) )
				{
					LOOTINGMGR->CreateLootingRoom( this, (CPlayer*)pAttacker );
				}
				
				if( IsPKMode() == FALSE &&
					pAttackPlayer->IsPKMode() == TRUE && 
					WANTEDMGR->IsOwner((CPlayer*)pAttacker, GetWantedIdx()) == FALSE )
				{
					SetMurderIDX(pAttacker->GetID());
					SetMurderKind(pAttacker->GetObjectKind());
					WANTEDMGR->SendRegistWanted(this, (CPlayer*)pAttacker);
					//journal insert
					JouranlInsertWanted(GetID(), eJournal_Wanted, eJournal_MurderedbyChr, ((CPlayer*)pAttacker)->GetObjectName());
				}
				else
					SetReadyToRevive(TRUE);
				WANTEDMGR->Complete((CPlayer*)pAttacker, this);
			}
			else
			{
				SetReadyToRevive(TRUE);
				WANTEDMGR->Complete((CPlayer*)pAttacker, this);
			}

			// partywar
			if( PARTYWARMGR->PlayerDie( this, (CPlayer*)pAttacker ) )
			{
				SetReadyToRevive( FALSE );
			}
		}
	}
	else if(pAttacker->GetObjectKind() & eObjectKind_Monster )
	{		
		SetMurderKind(((CMonster*)pAttacker)->GetMonsterKind());
		SetReadyToRevive(TRUE);
	}

	//KES PKPlayerPanelty
	PKMGR->DiePanelty( this, pAttacker );

	// ��oI��?i
	//SetGravity(0);
	CMonster * pObject = NULL;
	m_FollowMonsterList.SetPositionHead();
	while(pObject = (CMonster *)m_FollowMonsterList.GetData())
	{
		pObject->SetTObject(NULL);
		//totalG += pObject->GetGravity();
	}
	m_FollowMonsterList.RemoveAll();
}
*/





//=========================================================================
//	NAME : SendPlayerToMap
//	DESC : The function to move player to specified destination. 080104 LYW
//=========================================================================
void CPlayer::SendPlayerToMap(MAPTYPE mapNum, float xpos, float zpos)
{
	MSG_DWORD3 msg ;														// �޽��� ����ü�� �����Ѵ�.
	memset(&msg, 0, sizeof(MSG_DWORD3)) ;									// �޽��� �ʱ�ȭ.

	msg.Category	= MP_USERCONN ;
	msg.Protocol	= MP_USERCONN_BACKTOBEFOREMAP_SYN ;						// ī�װ��� ���������� �����Ѵ�.

	msg.dwObjectID	= GetID() ;												// ������Ʈ ���̵� �����Ѵ�.

	msg.dwData1		= (DWORD)mapNum ;										// ���� ������ �� ��ȣ�� �����Ѵ�.
	msg.dwData2		= (DWORD)xpos ;											// X��ǥ�� �����Ѵ�.
	msg.dwData3		= (DWORD)zpos ;											// Z��ǥ�� �����Ѵ�.

	SendMsg( &msg, sizeof(MSG_DWORD3) ) ;									// �޽����� �����Ѵ�.
}

//void CPlayer::SendPlayerToMap()
//{
//	if( !m_wReturnMapNum )	return;
//
//	MSG_WORD2 Msg;
//	Msg.Category = MP_USERCONN;
//	Msg.Protocol = MP_USERCONN_BACKTOBEFOREMAP_SYN;
//	Msg.wData1 = g_pServerSystem->GetMapNum();
//	Msg.wData2 = m_wReturnMapNum;
//
//	SendMsg( &Msg, sizeof(Msg) );
//
//	m_bDieInSpecialMap = FALSE;
//	m_wReturnMapNum = 0;
//	m_bNeedRevive = FALSE;
//}

DOUBLE CPlayer::GetPlayerTotalExpPoint()
{
	DOUBLE exp = 0;

	for(int i=1; i<GetLevel(); ++i)
	{
		exp += GAMERESRCMNGR->GetMaxExpPoint( i );
	}

	exp += GetPlayerExpPoint();

	return exp;
}
/*�����Ƽ ���� - 
void CPlayer::SetChangeAbilityKyungGongLevel( WORD wLevel )
{
	MSG_DWORD2 msg;
	msg.Category = MP_KYUNGGONG;
	msg.Protocol = MP_KYUNGGONG_ABILITY_CHANGE_NOTIFY;
	msg.dwData1 = this->GetID();
	msg.dwData2 = wLevel;
	PACKEDDATA_OBJ->QuickSendExceptObjectSelf( this, &msg, sizeof(msg) );	
}
*/
void CPlayer::InitGuildUnionInfo( DWORD dwGuildUnionIdx, char* pGuildUnionName, DWORD dwMarkIdx )
{
	m_HeroCharacterInfo.dwGuildUnionIdx = dwGuildUnionIdx;
	strncpy( m_HeroCharacterInfo.sGuildUnionName, pGuildUnionName, MAX_GUILD_NAME+1 );
	m_HeroCharacterInfo.dwGuildUnionMarkIdx = dwMarkIdx;
}

void CPlayer::SetGuildUnionInfo( DWORD dwGuildUnionIdx, char* pGuildUnionName, DWORD dwMarkIdx )
{
	m_HeroCharacterInfo.dwGuildUnionIdx = dwGuildUnionIdx;
	strncpy( m_HeroCharacterInfo.sGuildUnionName, pGuildUnionName, sizeof( m_HeroCharacterInfo.sGuildUnionName ) );
	m_HeroCharacterInfo.dwGuildUnionMarkIdx = dwMarkIdx;

	MSG_NAME_DWORD3 Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = MP_GUILD_UNION_PLAYER_INFO;
	Msg.dwData1 = GetID();
	Msg.dwData2 = dwGuildUnionIdx;
	Msg.dwData3 = dwMarkIdx;
	strncpy( Msg.Name, pGuildUnionName, sizeof( Msg.Name ) );

	PACKEDDATA_OBJ->QuickSendExceptObjectSelf( this, &Msg, sizeof(Msg) );
}

// 070801 ����, ���� �йи� �����ϸ� �ּ�ó��

//void CPlayer::InitFamilyUnionInfo( DWORD dwFamilyUnionIdx, char* pFamilyUnionName, DWORD dwMarkIdx )
//{
//	m_HeroCharacterInfo.dwFamilyUnionIdx = dwFamilyUnionIdx;
//	strncpy( m_HeroCharacterInfo.sFamilyUnionName, pFamilyUnionName, MAX_GUILD_NAME+1 );
//	m_HeroCharacterInfo.dwFamilyUnionMarkIdx = dwMarkIdx;
//}

// 070801 ����, ���� �йи� �����ϸ� �ּ�ó��

//void CPlayer::SetFamilyUnionInfo( DWORD dwFamilyUnionIdx, char* pFamilyUnionName, DWORD dwMarkIdx )
//{
//	m_HeroCharacterInfo.dwFamilyUnionIdx = dwFamilyUnionIdx;
//	strncpy( m_HeroCharacterInfo.sFamilyUnionName, pFamilyUnionName, MAX_GUILD_NAME+1 );
//	m_HeroCharacterInfo.dwFamilyUnionMarkIdx = dwMarkIdx;
//
//	MSG_NAME_DWORD3 Msg;
//	Msg.Category = MP_FAMILY_UNION;
//	Msg.Protocol = MP_FAMILY_UNION_PLAYER_INFO;
//	Msg.dwData1 = GetID();
//	Msg.dwData2 = dwFamilyUnionIdx;
//	Msg.dwData3 = dwMarkIdx;
//	strncpy( Msg.Name, m_HeroCharacterInfo.sFamilyUnionName, MAX_GUILD_NAME+1 );
//
//	PACKEDDATA_OBJ->QuickSendExceptObjectSelf( this, &Msg, sizeof(Msg) );
//}

void CPlayer::SetCharChangeInfo( CHARACTERCHANGE_INFO* pInfo )
{
	m_HeroCharacterInfo.Gender = pInfo->Gender;
	m_HeroCharacterInfo.HairType = pInfo->HairType;
	m_HeroCharacterInfo.FaceType = pInfo->FaceType;
	m_HeroCharacterInfo.Height = pInfo->Height;
	m_HeroCharacterInfo.Width = pInfo->Width;
}

//SW051112 ���ָ��
void CPlayer::SetMussangMaxPoint()
{//���� �����ÿ� ����.
	int iMaxMussangPoint = GetMussangMaxPoint();

	if(iMaxMussangPoint < 0)
	{
		iMaxMussangPoint = 0;
		ASSERT(0);
	}

	if( m_HeroInfo.MaxMussangPoint != (DWORD)iMaxMussangPoint )
	{
		m_HeroInfo.MaxMussangPoint = iMaxMussangPoint;

		MSG_DWORDBYTE msg;	//SendMussangInfo �� ���� ����. ������ �ߵ��غ� ���� �̷�� ���� �ֱ⶧���� ���� �Ѵ�.
		msg.Category = MP_CHAR;
		msg.Protocol = MP_CHAR_MUSSANG_INFO;
		msg.dwObjectID = GetID();
		msg.dwData = m_HeroInfo.MaxMussangPoint;
		msg.bData = eMSS_LevelUp;
		
		SendMsg(&msg,sizeof(msg));
	}
}

void CPlayer::AddMussangPointFromLevel(DWORD dwMonsterLevel)
{
	if( m_bIsMussangReady ) return;
	//���� ���
	int dwAddPoint = dwMonsterLevel + dwMonsterLevel - GetLevel();
	//int dwAddPoint = 100;	//!!!test

	//�ּ� 1
	if( dwAddPoint < 1 )
		dwAddPoint = 1;
	// Rama - 06.06.09
//	if( m_AvatarOption.MussangCharge )
//		dwAddPoint = int(dwAddPoint*(1+m_AvatarOption.MussangCharge*0.01f));
	//�����ְ�
	m_dwMussangPoint += dwAddPoint;	
	//�ִ밪����
	if( m_dwMussangPoint >= m_HeroInfo.MaxMussangPoint )
	{
		m_dwMussangPoint = m_HeroInfo.MaxMussangPoint;
	}
	//�������� (�ִ밪�����϶���)
	if( !m_bIsMussangReady )
	{
		//�ִ밪�̸� �غ����
		if( m_dwMussangPoint == m_HeroInfo.MaxMussangPoint )
			m_bIsMussangReady = TRUE;
		SendMussangInfo();
	}
}

DWORD CPlayer::GetMussangMaxPoint()	//ĳ���� DB���� ������ ���.
{
//	return ( ( GetLevel() + 3 ) * 120 ) - ( ( 30 - GetLevel() ) * 10 );

	// 06. 03 �������� - �̿���
/*	switch(m_HeroCharacterInfo.Stage)
	{
	case eStage_Normal:	
*/		return GetLevel()*130 + 780;	//( GetLevel() + 9 ) * 120 ) - ( ( 30 - GetLevel() ) * 10 )
/*	case eStage_Hwa:		
	case eStage_Geuk:
		return GetLevel()*250 + 1620;	//( GetLevel() + 8 ) * 240 ) - ( ( 30 - GetLevel() ) * 10 )
	case eStage_Hyun:	  		
	case eStage_Tal:	
		return GetLevel()*370 + 2220;	//( GetLevel() + 7 ) * 360 ) - ( ( 30 - GetLevel() ) * 10 )
	}
*/
	return 0;
}

void CPlayer::SendMussangInfo()
{
	//���� ���� ����Ʈ�� ������.
	MSG_DWORDBYTE msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_MUSSANG_INFO;
	msg.dwObjectID = GetID();
	msg.dwData	= m_dwMussangPoint;

	if( m_bIsMussangReady )	m_bMussangFlag |= eMSS_Ready;	//�����̸鼭 ������ Ǯ(���)������ �� ���ִ�. ���� ���¿��� ������ ����.
//	if( m_HeroCharacterInfo.bMussangMode )	m_bMussangFlag |= eMSS_InMode;
//	else m_bMussangFlag |= eMSS_None;

	msg.bData	= m_bMussangFlag;

	SendMsg(&msg,sizeof(msg));

	m_bMussangFlag = 0;
}

int	CPlayer::StartMussangMode()
{
	if( !m_bIsMussangReady )
	{
		ASSERTMSG(0, "MussangPoint:Sync Err");
		return -99;
	}

	m_HeroCharacterInfo.bMussangMode = TRUE;

	m_bIsMussangReady = FALSE;	// ��� ���� �ʱ�

	InitMussangPoint();	// ���� ����Ʈ �ʱ�

	SetMussangMode(TRUE);	// ���� ���

	SendMussangInfo();
	
	m_dwMussangStartTime = gCurTime;

	// 06. 03 �������� - �̿���
/*	switch(m_HeroCharacterInfo.Stage)
	{
	case eStage_Normal:	
		{
*/			m_wMussangStat = 0;
			m_dwMussangTime = 60000;
/*		}
		break;
	case eStage_Hwa:		
	case eStage_Geuk:
		{
			m_wMussangStat = 10;
			m_dwMussangTime = 90000;
		}
		break;
	case eStage_Hyun:
	case eStage_Tal:
		{
			m_wMussangStat = 15;
			m_dwMussangTime = 120000;
		}
		break;
	}
*/
	m_dwMussangTime += (m_HeroCharacterInfo.Level / 10) * 10000;

	CalcState();

	return 1;
}

void CPlayer::CheckMussangTime()
{
	if( !m_HeroCharacterInfo.bMussangMode ) return;	//���� ���°� �ƴϸ� ����

	// 06. 03 �������� - �̿���
	if( gCurTime - m_dwMussangStartTime > m_dwMussangTime )
	{
		SetMussangMode(FALSE);
		SendMussangEnd();

		m_wMussangStat = 0;
	
		CalcState();
	}
}

void CPlayer::SendMussangEnd()
{
	MSGBASE msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_MUSSANG_END;
	msg.dwObjectID = GetID();
	
	PACKEDDATA_OBJ->QuickSend(this, &msg, sizeof(MSGBASE));
}

void CPlayer::CheatMussangReady()
{
	m_dwMussangPoint = m_HeroInfo.MaxMussangPoint;
	m_bIsMussangReady = TRUE;

	SendMussangInfo();
}

float CPlayer::GetAccuracy()
{
	return mAccuracy;
}

float CPlayer::GetAvoid()
{
	return mAvoid;
}

// 080910 LUJ, ��� ��ġ�� �������� ��ȯ(��ȹ: ������)
float CPlayer::GetBlock()
{
	const float rate	= mRateBuffStatus.Block + mRatePassiveStatus.Block + 1.0f;
	const float value	= ( mBuffStatus.Block + mPassiveStatus.Block ) / 100.0f;
	float		bonus	= 0;

	switch( GetJop( 0 ) )
	{
		// 080910 LUJ, ������
	case 1:
		{
			bonus = 0.15f;
			break;
		}
		// 080910 LUJ, �α�
	case 2:
		{
			bonus = 0.10f;
			break;
		}
		// 080910 LUJ, ������
	case 3:
		{
			bonus = 0.05f;
			break;
		}
	}

	return ( bonus + ( float( GetDexterity() ) * 0.0001f ) + value ) * rate;
}

float CPlayer::GetPhysicAttackMax()
{
	return mPhysicAttackMax;
}

float CPlayer::GetPhysicAttackMin()
{
	return mPhysicAttackMin;
}

float CPlayer::GetMagicAttack()
{
	return mMagicAttack;
}

float CPlayer::GetPhysicDefense()
{
	return mPhysicDefense;
}

float CPlayer::GetMagicDefense()
{
	return mMagicDefense;
}

float CPlayer::GetCriticalRate()
{
	return mCriticalRate;
}

float CPlayer::GetCriticalDamage()
{
	return mCriticalDamage;
}

float CPlayer::GetLifeRecover()
{
	return mLifeRecover;
}

float CPlayer::GetManaRecover()
{
	return mManaRecover;
}

////////////////////////////////////////////////////////////////////////////
// 06. 07. ���°������� - �̿���
// �����̻� ��ų �����н� Ư���� ���¸� ������ ������ ���´� ��� �ʱ�ȭ
// �������� ���¿����� ���� �ʱ�ȭ
// ���а��� �������� �ʴ´�.
// ���� ���нÿ��� ��������
BOOL CPlayer::CanSkillState()
{
	//���� 5ȸ �̻� ���н�
	if(m_SkillFailCount >= 5)
	{
		switch(m_BaseObjectInfo.ObjectState)
		{
		// ��ų�� ����Ҽ� �ִ� ���¸� ���� �ʱ�ȭ�ϰ� TRUE ����
		case eObjectState_None:
		case eObjectState_Move:
		case eObjectState_TiedUp_CanSkill:
			{
				m_SkillFailCount = 0;
				return TRUE;
			}
			break;
		// ��ų�� ����Ҽ� ���� ������ Ǯ���൵ ������ ���¸� ���� �ʱ�ȭ �ϰ�
		// ���¸� �ʱ�ȭ �ѵ� TRUE ����
		case eObjectState_SkillStart:
		case eObjectState_SkillSyn:	
		case eObjectState_SkillBinding:
		case eObjectState_SkillUsing:
		case eObjectState_SkillDelay:
		case eObjectState_Society:
		case eObjectState_Enter:
		case eObjectState_Ungijosik:
		case eObjectState_Rest:
			{
				m_SkillFailCount = 0;
				m_BaseObjectInfo.ObjectState = eObjectState_None;
				return TRUE;
			}
			break;
		// �� ���� ��쿣 ���� �ʱ�ȭ �ϰ� FALSE ����
		default:
			{
				m_SkillFailCount = 0;
				return FALSE;
			}
			break;
		}
	}

	// 5ȸ ���� �϶� ��ų�� ����Ҽ� ���� ���¸�
	// ���� �����ϰ� FALSE ����
	if(m_BaseObjectInfo.ObjectState != eObjectState_None &&
	   m_BaseObjectInfo.ObjectState != eObjectState_Move &&
	   m_BaseObjectInfo.ObjectState != eObjectState_TiedUp_CanSkill )
	{
		m_SkillFailCount++;
		return FALSE;
	}

	// ���� �����϶� ���� �ʱ�ȭ�ϰ� TRUE ����
	m_SkillFailCount = 0;
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////

BOOL CPlayer::AddQuick( BYTE sheet, WORD pos, SLOT_INFO* pSlot )
{
	return m_QuickSlot[ sheet ].Update( pos, pSlot );
}

SLOT_INFO*	CPlayer::GetQuick( BYTE sheet, WORD pos )
{
	return m_QuickSlot[ sheet ].GetQuick( pos );
}

// desc_hseos_���͹���01
// S ���͹��� �߰� added by hseos 2007.05.23	2007.07.08
void CPlayer::ProcMonstermeterPlayTime()
{
	if (gCurTime - m_stMonstermeterInfo.nPlayTimeTick > SHMath_MINUTE(1))
	{
		m_stMonstermeterInfo.nPlayTimeTick = gCurTime;
		m_stMonstermeterInfo.nPlayTime++;
		m_stMonstermeterInfo.nPlayTimeTotal++;

		// DB�� ����
		//MonsterMeter_Save(GetID(), m_stMonstermeterInfo.nPlayTime, m_stMonstermeterInfo.nKillMonsterNum, m_stMonstermeterInfo.nPlayTimeTotal, m_stMonstermeterInfo.nKillMonsterNumTotal);

		// �÷��̽ð��� Ŭ���̾�Ʈ������ ��� �����ϹǷ� Ŭ���̾�Ʈ���� ����ϵ��� �ϰ�
		// Ȥ�� �� ���������� 10�и��� ������ ��ġ�� �����ش�.
		if ((m_stMonstermeterInfo.nPlayTime%10) == 0)
		{
			// Ŭ���̾�Ʈ�� �˸���
			MSG_DWORD2 msg;
			msg.Category = MP_CHAR;
			msg.Protocol = MP_CHAR_MONSTERMETER_PLAYTIME;
			msg.dwObjectID = GetID();
			msg.dwData1 = m_stMonstermeterInfo.nPlayTime;
			msg.dwData2 = m_stMonstermeterInfo.nPlayTimeTotal;
			SendMsg(&msg, sizeof(msg));
		}

		// ���� ó��
		g_csMonstermeterManager.ProcessReward(this, CSHMonstermeterManager::RBT_PLAYTIME, m_stMonstermeterInfo.nPlayTimeTotal);
	}
}
// E ���͹��� �߰� added by hseos 2007.05.23	2007.07.08

// desc_hseos_���͹���01
// S ���͹��� �߰� added by hseos 2007.05.23	2007.07.08
void CPlayer::ProcMonstermeterKillMon()
{
	m_stMonstermeterInfo.nKillMonsterNum++;
	m_stMonstermeterInfo.nKillMonsterNumTotal++;

	// DB�� ����
	//MonsterMeter_Save(GetID(), m_stMonstermeterInfo.nPlayTime, m_stMonstermeterInfo.nKillMonsterNum, m_stMonstermeterInfo.nPlayTimeTotal, m_stMonstermeterInfo.nKillMonsterNumTotal);

	// Ŭ���̾�Ʈ�� �˸���
	MSG_DWORD2 msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_MONSTERMETER_KILLMONSTER;
	msg.dwObjectID = GetID();
	msg.dwData1 = m_stMonstermeterInfo.nKillMonsterNum;
	msg.dwData2 = m_stMonstermeterInfo.nKillMonsterNumTotal;
	SendMsg(&msg, sizeof(msg));

	// ���� ó��
	g_csMonstermeterManager.ProcessReward(this, CSHMonstermeterManager::RBT_MONSTERKILL, m_stMonstermeterInfo.nKillMonsterNumTotal);
}
// E ���͹��� �߰� added by hseos 2007.05.23	2007.07.08

// desc_hseos_�ֹε��01
// S �ֹε�� �߰� added by hseos 2007.06.04	2007.06.09
void CPlayer::SetResidentRegistInfo(DATE_MATCHING_INFO* pInfo)
{
	memcpy(&m_DateMatching, pInfo, sizeof(m_DateMatching));
	SafeStrCpy(m_stRegistInfo.szNickName, m_DateMatching.szName, MAX_NAME_LENGTH+1);
	m_stRegistInfo.nRace				= (BYTE)m_DateMatching.nRace;
	m_stRegistInfo.nSex					= m_DateMatching.byGender;
	m_stRegistInfo.nAge					= (WORD)m_DateMatching.dwAge;
	m_stRegistInfo.nLocation			= (WORD)m_DateMatching.dwRegion;
	m_stRegistInfo.nFavor				= (WORD)m_DateMatching.dwGoodFeelingDegree;
	m_stRegistInfo.nPropensityLike01	= (WORD)m_DateMatching.pdwGoodFeeling[0];
	m_stRegistInfo.nPropensityLike02	= (WORD)m_DateMatching.pdwGoodFeeling[1];
	m_stRegistInfo.nPropensityLike03	= (WORD)m_DateMatching.pdwGoodFeeling[2];
	m_stRegistInfo.nPropensityDisLike01	= (WORD)m_DateMatching.pdwBadFeeling[0];
	m_stRegistInfo.nPropensityDisLike02	= (WORD)m_DateMatching.pdwBadFeeling[1];
}
// E �ֹε�� �߰� added by hseos 2007.06.04	2007.06.09

// desc_hseos_����ý���_01
// S ����ý��� �߰� added by hseos 2007.08.23
void CPlayer::ProcFarmTime()
{
	if (m_stFarmInfo.nCropPlantRetryTime)
	{
		if (gCurTime - m_stFarmInfo.nCropPlantRetryTimeTick > SHMath_MINUTE(1))
		{
			m_stFarmInfo.nCropPlantRetryTimeTick = gCurTime;
			m_stFarmInfo.nCropPlantRetryTime--;

			// DB�� ����
			Farm_SetTimeDelay(GetID(), CSHFarmManager::FARM_TIMEDELAY_KIND_PLANT, m_stFarmInfo.nCropPlantRetryTime);
		}
	}

	if (m_stFarmInfo.nCropManureRetryTime)
	{
		if (gCurTime - m_stFarmInfo.nCropManureRetryTimeTick > SHMath_MINUTE(1))
		{
			m_stFarmInfo.nCropManureRetryTimeTick = gCurTime;
			m_stFarmInfo.nCropManureRetryTime--;

			// DB�� ����
			Farm_SetTimeDelay(GetID(), CSHFarmManager::FARM_TIMEDELAY_KIND_MANURE, m_stFarmInfo.nCropManureRetryTime);
		}
	}

	// 080430 KTH Animal Delay Add  (��)������ ���⼭ ��ġ�� Decrease ���ִ±�...
	if( m_stFarmInfo.nAnimalCleanRetryTime )
	{
		if( gCurTime - m_stFarmInfo.nAnimalCleanRetryTimeTick > SHMath_MINUTE(1) )
		{
			m_stFarmInfo.nAnimalCleanRetryTimeTick = gCurTime;
			m_stFarmInfo.nAnimalCleanRetryTime--;

			Farm_SetTimeDelay(GetID(), CSHFarmManager::FARM_TIMEDELAY_KIND_CLEAN, m_stFarmInfo.nAnimalCleanRetryTime);
		}
	}

	if( m_stFarmInfo.nAnimalFeedRetryTime )
	{
		if( gCurTime - m_stFarmInfo.nAnimalFeedRetryTimeTick > SHMath_MINUTE(1) )
		{
			m_stFarmInfo.nAnimalFeedRetryTimeTick = gCurTime;
			m_stFarmInfo.nAnimalFeedRetryTime--;

			Farm_SetTimeDelay(GetID(), CSHFarmManager::FARM_TIMEDELAY_KIND_FEED, m_stFarmInfo.nAnimalFeedRetryTime);
		}
	}
}
// E ����ý��� �߰� added by hseos 2007.08.23

BOOL CPlayer::IsInventoryPosition( POSTYPE position )
{
	const CItemSlot* slot = GetSlot( eItemTable_Inventory );

    const POSTYPE begin = slot->GetStartPos();
	const POSTYPE end	= slot->GetSlotNum() + begin;

	return begin <= position && end >= position;
}


void CPlayer::ResetSetItemStatus()
{
	mStoredSkill.clear();
	mStoredSetSkill.clear();
	mSetItemLevel.clear();

	// 080313 LUJ, ��Ʈ ������ ���� �ʱ�ȭ
	ZeroMemory( &m_SetItemStats, sizeof( m_SetItemStats ) );
}


const CPlayer::SetItemLevel& CPlayer::GetSetItemLevel() const
{
	return mSetItemLevel;
}


CPlayer::SetItemLevel& CPlayer::GetSetItemLevel()
{
	return mSetItemLevel;
}


void CPlayer::AddSetSkill( DWORD skillIndex, BYTE level )
{
	// �нú� ��ų�� �ƴ� ���� Ŭ���̾�Ʈ���� ó���ϸ� �ȴ�...
	//{
	//	cSkillInfo* info = SKILLMGR->GetSkillInfo( skillIndex );

	//	if( !	info								||
	//			info->GetKind() != SKILLKIND_PASSIVE )
	//	{
	//		return;
	//	}
	//}

	// ���� ����� ��ų�� �����ϵ�, �÷��̾ ���� ���� �ִ� ��ų�� �����Ͽ�
	// ��Ʈ ȿ���� ���ŵ� �� �����Ѵ�
	{
		SKILL_BASE* skill = m_SkillTree.GetSkillBase( skillIndex );

		if( skill )
		{
			// DB �ε����� �ִ� ��ų�� �÷��̾ ���� ���� �ִ� ��ų�̹Ƿ� �����Ѵ�.
			if( skill->dwDBIdx )
			{
				// �ι� ����Ǹ� � ������ �����ؾ����� ������ �� ���� �ȴ�. �̷� ������ �߻��ϸ� ��� �ذ��ؾ� �Ѵ�.
				ASSERT( mStoredSkill.end() == mStoredSkill.find( skillIndex ) );

				mStoredSkill[ skillIndex ] = *skill;
			}
			
			mStoredSetSkill[ skillIndex ].push( level );

			level = level + skill->Level;

			m_SkillTree.RemoveSkillBase( skill );
		}
	}

	// ���� ��Ʈ ��ų�� �����Ѵ�.
	if( 0 != level )
	{
		SKILL_BASE* skill	= m_SkillTree.NewSkillBase();
		skill->dwDBIdx		= 0;
		skill->wSkillIdx	= skillIndex;
		skill->Level		= level;

		m_SkillTree.AddSkillBase( skill );
	}
}


void CPlayer::RemoveSetSkill( DWORD skillIndex )
{	
	BYTE previousSkillLevel = 0;

	// 080417 LUJ, ����� ��Ʈ ��ų�� ������. DB�ε����� ������ ��Ʈ ��ų���� ����
	{
		SKILL_BASE* skill	= m_SkillTree.GetSkillBase( skillIndex );

		if(		skill &&
			!	skill->dwDBIdx )
		{
			previousSkillLevel	= skill->Level;

			m_SkillTree.RemoveSkillBase( skill );
		}	
	}

	// �� �̻� ����� ��Ʈ ��ų�� ���� ���� ��ų�� ������ ��������
	if( mStoredSetSkill.end() == mStoredSetSkill.find( skillIndex ) )
	{
		StoredSkill::iterator it = mStoredSkill.find( skillIndex );

		if( mStoredSkill.end() != it )
		{
			SKILL_BASE* skill = m_SkillTree.NewSkillBase();
			*skill = it->second;

			mStoredSkill.erase( it );

			m_SkillTree.AddSkillBase( skill );
		}
	}
	else
	{
		SetSkill& setSkill = mStoredSetSkill[ skillIndex ];
		ASSERT( setSkill.size() );

		const BYTE level = setSkill.top();

		SKILL_BASE* skill	= m_SkillTree.NewSkillBase();
		skill->dwDBIdx		= 0;
		skill->Level		= previousSkillLevel - level;
		skill->wSkillIdx	= skillIndex;

		m_SkillTree.AddSkillBase( skill );

		setSkill.pop();

		if( setSkill.empty() )
		{
			mStoredSetSkill.erase( skillIndex );
		}
	}
}


SKILL_BASE* CPlayer::GetStoredSkill( DWORD index )
{
	StoredSkill::iterator it = mStoredSkill.find( index );

	return mStoredSkill.end() == it ? 0 : &( it->second );
}


void CPlayer::SetStoredSkill( const SKILL_BASE& skill )
{
	ASSERT( mStoredSkill.end() == mStoredSkill.find( skill.wSkillIdx ) );

	mStoredSkill[ skill.wSkillIdx ]	= skill;
}


BOOL CPlayer::IsSetSkill( DWORD index )
{
	// DB �ε����� ������ ��Ʈ ���������� ���� �ο��� ��ų

	const SKILL_BASE* skill = m_SkillTree.GetSkillBase( index );

	return skill && ! skill->dwDBIdx;
}

void CPlayer::SetHide( WORD level ) 
{ 
	m_HeroCharacterInfo.HideLevel = level; 
	
	if( level )
	{
		m_HeroCharacterInfo.bVisible = false;
	}
	else
	{
		m_HeroCharacterInfo.bVisible = true;
	}

	MSG_WORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_HIDE_NOTIFY;
	msg.dwObjectID = GetID();
	msg.wData = level;

	PACKEDDATA_OBJ->QuickSend( this, &msg, sizeof( msg ) );	
}

void CPlayer::SetDetect( WORD level ) 
{ 
	m_HeroCharacterInfo.DetectLevel = level; 
	
	MSG_WORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_DETECT_NOTIFY;
	msg.dwObjectID = GetID();
	msg.wData = level;

	PACKEDDATA_OBJ->QuickSend( this, &msg, sizeof( msg ) );	
}	

//---KES Aggro 070918
//void CMonster::CalcAggro() �Լ������� ȣ���Ѵ�. �ٸ������� ȣ�� ����!
void CPlayer::AddToAggroed( CMonster* pMonster )
{
	m_htAggroedTable.Add( pMonster, pMonster->GetID() );
}

//void CMonster::RemoveFromAggro( CPlayer* pPlayer ) �Լ������� ȣ���Ѵ�. �ٸ������� ȣ�� ����!
void CPlayer::RemoveFromAggroed( DWORD dwObjectID )
{
	m_htAggroedTable.Remove( dwObjectID );
}

void CPlayer::RemoveAllAggroed()
{
	m_htAggroedTable.SetPositionHead();
	CMonster* pMonster = NULL;
	while( pMonster = m_htAggroedTable.GetData() )
	{
		pMonster->RemoveFromAggro( GetID() );		//���Ͱ� ������ �ִ� ��׷� ����Ʈ���� ����
	}

	m_htAggroedTable.RemoveAll();
}

//���� ���� ��׷θ��͸���Ʈ�� �̿���, pTarget�� ��׷θ� �ø���. (pTarget�� �ڽ��� �� �� �ִ�.)
void CPlayer::AddAggroToMyMonsters( int nAggroAdd, CObject* pTarget )
{
	m_htAggroedTable.SetPositionHead();
	CMonster* pMonster = NULL;
	while( pMonster = m_htAggroedTable.GetData() )
	{
		pMonster->CalcAggro( pTarget, nAggroAdd );
	}
}

// 080910 LUJ, ������ ������ ��ȯ�Ѵ�
DWORD CPlayer::GetShieldDefense()
{
	return mShieldDefense;
}

// 080910 LUJ, ������ ������ �����Ѵ�
void CPlayer::SetShieldDefence( DWORD shieldDefense )
{
	mShieldDefense = shieldDefense;
}

void CPlayer::AddSkillException( EXCEPTION* pException )
{
	EXCEPTION* pOldException = NULL;

	pOldException = mExceptionTable.GetData( pException->TargetIndex );

	if( pOldException )
	{
		if( pOldException->Grade > pException->Grade )
		{
			return;
		}
		else
		{
			mExceptionTable.Remove( pOldException->TargetIndex );
			mExceptionTable.Add( pException, pException->TargetIndex );
		}
	}
	else
	{
		mExceptionTable.Add( pException, pException->TargetIndex );
	}

}

void CPlayer::RemoveSkillException( EXCEPTION* pException )
{
	EXCEPTION* pOldException = NULL;

	pOldException = mExceptionTable.GetData( pException->TargetIndex );

	if( pOldException == pException )
	{
		mExceptionTable.Remove( pException->TargetIndex );
	}
}

EXCEPTION* CPlayer::GetException( WORD TargetKind, DWORD TargetIndex )
{
	EXCEPTION* pException = NULL;

	pException = mExceptionTable.GetData( TargetIndex );

	if( TargetKind == pException->TargetKind )
	{
		return pException;
	}
	else
	{
		return NULL;
	}
	
}


BOOL CPlayer::AddCoolTime( DWORD coolTimeGroupIndex, DWORD coolTime )
{
	ProcCoolTime();

	// 080617 LUJ, ������ ������ ����
	if( mCoolTimeMap->end() != mCoolTimeMap->find( coolTimeGroupIndex ) )
	{
		const CoolTime& time = ( *mCoolTimeMap )[ coolTimeGroupIndex ];

		return FALSE;
	}

	// 080617 LUJ, ������ ������ ����
	CoolTime&	time = ( *mCoolTimeMap )[ coolTimeGroupIndex ];
	const DWORD tick = GetTickCount();

	time.mBeginTick = tick;
	time.mEndTick	= tick + coolTime;

	return TRUE;
}


void CPlayer::ProcCoolTime()
{
	const DWORD tick = GetTickCount();

	std::set< DWORD > group;

	// 080617 LUJ, ������ ������ ����
	for( CoolTimeMap::iterator it = mCoolTimeMap->begin(); mCoolTimeMap->end() != it; ++it )
	{
		const CoolTime& time = it->second;

		if( ( time.mBeginTick < time.mEndTick && time.mEndTick < tick ) ||
			( time.mBeginTick > time.mEndTick && time.mBeginTick > tick && time.mEndTick < tick ) )
		{
			// ����: ������ ���Ŀ��� �ݺ��ڰ� �߸��ȴٴ� ����� ���. ���� ���� ���μ������� ó���ؾ� �Ѵ�.
			group.insert( it->first );
		}
	}

	for( std::set< DWORD > ::const_iterator it = group.begin(); group.end() != it; ++it )
	{
		// 080617 LUJ, ������ ������ ����
		mCoolTimeMap->erase( *it );
	}
}

DWORD CPlayer::GetVitality() 
{ 
	/// �ɸ��� �⺻ ��ġ�� �������� ��ġ�� ���Ѵ�(����/��ȭ ����)
	float Result = m_charStats.mVitality.mPlus + m_itemStats.mVitality.mPlus;

	/// ��Ʈ ������ �ɷ�ġ�� �����ش�
	const PlayerStat::Value& value = m_SetItemStats.mVitality;
	Result += value.mPlus;

	/// ��ų �ɷ�ġ�� �����ش�
	Result += ( mPassiveStatus.Vit + mBuffStatus.Vit );

	/// %���� �ɼ����� ������ ��ġ�� �����ش�/
	float rate = ( mRatePassiveStatus.Vit + mRateBuffStatus.Vit + ( value.mPercent * 100.f ) ) / 100.f + m_itemStats.mVitality.mPercent;
	Result += ( m_charStats.mVitality.mPlus * rate );

	if( Result < 0 )
	{
		Result = 0;
	}

	return (DWORD)Result;
}

DWORD CPlayer::GetWisdom() 
{ 
	/// �ɸ��� �⺻ ��ġ�� �������� ��ġ�� ���Ѵ�(����/��ȭ ����)
	float Result = m_charStats.mWisdom.mPlus + m_itemStats.mWisdom.mPlus;

	/// ��Ʈ ������ �ɷ�ġ�� �����ش�
	const PlayerStat::Value& value = m_SetItemStats.mWisdom;
	Result += value.mPlus;

	/// ��ų �ɷ�ġ�� �����ش�
	Result += ( mPassiveStatus.Wis + mBuffStatus.Wis );

	/// %���� �ɼ����� ������ ��ġ�� �����ش�/
	float rate = ( mRatePassiveStatus.Wis + mRateBuffStatus.Wis + ( value.mPercent * 100.f ) ) / 100.f + m_itemStats.mWisdom.mPercent;
	Result += ( m_charStats.mWisdom.mPlus * rate );

	if( Result < 0 )
	{
		Result = 0;
	}

	return (DWORD)Result;
}

DWORD CPlayer::GetStrength() 
{
	/// �ɸ��� �⺻ ��ġ�� �������� ��ġ�� ���Ѵ�(����/��ȭ ����)
	float Result = m_charStats.mStrength.mPlus + m_itemStats.mStrength.mPlus;

	/// ��Ʈ ������ �ɷ�ġ�� �����ش�
	const PlayerStat::Value& value = m_SetItemStats.mStrength;
	Result += value.mPlus;

	/// ��ų �ɷ�ġ�� �����ش�
	Result += ( mPassiveStatus.Str + mBuffStatus.Str );

	/// %���� �ɼ����� ������ ��ġ�� �����ش�/
	float rate = ( mRatePassiveStatus.Str + mRateBuffStatus.Str + ( value.mPercent * 100.f ) ) / 100.f + m_itemStats.mStrength.mPercent;
	Result += ( m_charStats.mStrength.mPlus * rate );

	if( Result < 0 )
	{
		Result = 0;
	}

	return (DWORD)Result;
}

DWORD CPlayer::GetDexterity() 
{ 
	/// �ɸ��� �⺻ ��ġ�� �������� ��ġ�� ���Ѵ�(����/��ȭ ����)
	float Result = m_charStats.mDexterity.mPlus + m_itemStats.mDexterity.mPlus;

	/// ��Ʈ ������ �ɷ�ġ�� �����ش�
	const PlayerStat::Value& value = m_SetItemStats.mDexterity;
	Result += value.mPlus;

	/// ��ų �ɷ�ġ�� �����ش�
	Result += ( mPassiveStatus.Dex + mBuffStatus.Dex );

	/// %���� �ɼ����� ������ ��ġ�� �����ش�/
	float rate = ( mRatePassiveStatus.Dex + mRateBuffStatus.Dex + ( value.mPercent * 100.f ) ) / 100.f + m_itemStats.mDexterity.mPercent;
	Result += ( m_charStats.mDexterity.mPlus * rate );

	if( Result < 0 )
	{
		Result = 0;
	}

	return (DWORD)Result;
}

DWORD CPlayer::GetIntelligence() 
{
	/// �ɸ��� �⺻ ��ġ�� �������� ��ġ�� ���Ѵ�(����/��ȭ ����)
	float Result = m_charStats.mIntelligence.mPlus + m_itemStats.mIntelligence.mPlus;

	/// ��Ʈ ������ �ɷ�ġ�� �����ش�
	const PlayerStat::Value& value = m_SetItemStats.mIntelligence;
	Result += value.mPlus;

	/// ��ų �ɷ�ġ�� �����ش�
	Result += ( mPassiveStatus.Int + mBuffStatus.Int );

	/// %���� �ɼ����� ������ ��ġ�� �����ش�/
	float rate = ( mRatePassiveStatus.Int + mRateBuffStatus.Int + ( value.mPercent * 100.f ) ) / 100.f + m_itemStats.mIntelligence.mPercent;
	Result += ( m_charStats.mIntelligence.mPlus * rate );

	if( Result < 0 )
	{
		Result = 0;
	}

	return (DWORD)Result;
}

void CPlayer::SetObjectBattleState( BYTE state ) 
{ 
	m_BaseObjectInfo.ObjectBattleState = state; 

	if( state )	//eObjectBattleState_Battle
	{
		mpBattleStateDelay->Start();

		//---KES PK 071202
		//SetPKStartTimeReset();
	}
}


// 071122 KTH --- delete
/*void CPlayer::AddTimeCheckItem(ITEMBASE* pItemBase )
{
	if( !pItemBase ) return ;

	m_htTimeCheckItem.Add( pItemBase, pItemBase->dwDBIdx );
}
*/
void CPlayer::ProcessTimeCheckItem( BOOL bForceDBUpdate )
{
	DWORD dwElapsedMili = gCurTime - m_dwLastTimeCheckItem;
	if(  dwElapsedMili < 60 * 1000 && !bForceDBUpdate )
		return;

	DWORD dwElapsedSecond = dwElapsedMili / 1000;

	m_dwLastTimeCheckItem = gCurTime - ( dwElapsedMili - dwElapsedSecond * 1000 ) ;

	BOOL bDBUpdate = bForceDBUpdate ? TRUE : FALSE;

	if( gCurTime - m_dwLastTimeCheckItemDBUpdate > 5 * 60 * 1000 )
	{
		bDBUpdate = TRUE;
		m_dwLastTimeCheckItemDBUpdate = gCurTime;
	}

	int iCheckItemMaxNum = TP_WEAR_END;
	if( IsGotWarehouseItems() )
	{
		iCheckItemMaxNum = TP_STORAGE_END;
	}

	for( int i = TP_INVENTORY_START ; i <= iCheckItemMaxNum ; ++i )
	{
		ITEMBASE* pItemBase = (ITEMBASE*)m_ItemContainer.GetItemInfoAbs( i );

		if( pItemBase->dwDBIdx == 0 ) continue;

		if( pItemBase->nSealed == eITEM_TYPE_UNSEAL )
		{
			if( pItemBase->nRemainSecond > (int)dwElapsedSecond )
			{
				pItemBase->nRemainSecond -= dwElapsedSecond;

				// 071125 KTH --- Player "RemainSecond�� 1�� �̸��� ��� Ŭ���̾�Ʈ���� ������ �����ش�."
				if( pItemBase->nRemainSecond <= 60 )
				{
					MSG_DWORD2 msg;
					msg.Category = MP_ITEM;
					msg.Protocol = MP_ITEM_TIMELIMT_ITEM_ONEMINUTE;
					msg.dwData1 = pItemBase->wIconIdx;
					msg.dwData2 = pItemBase->Position;
	
					SendMsg(&msg, sizeof(msg));
				}

				if( bDBUpdate )
				{
					//DB Update
					ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pItemBase->wIconIdx );
		
					if( pItemInfo )	
					if( pItemInfo->nTimeKind == eKIND_PLAYTIME )
					{
						//�ð� ���� DB ���ν��� ȣ��	//dbidx, nRemainSecond
						UpdateRemainTime(this->GetID(), pItemBase->dwDBIdx, pItemBase->nRemainSecond) ; 
					}
				}
			}
			else
			{
				POSTYPE	position = pItemBase->Position;
				DWORD	iconIdx = pItemBase->wIconIdx;
				int returnValue = ITEMMGR->DiscardItem( this, pItemBase->Position, pItemBase->wIconIdx, 1 );
				if( EI_TRUE == returnValue )
				{
					////---Ŭ���̾�Ʈ�� �޼���...  �Ʒ��޼����� �׽�Ʈ��
					MSG_ITEM_DISCARD_ACK msg;
					msg.Category = MP_ITEM;
					msg.Protocol = MP_ITEM_DISCARD_ACK;
					msg.TargetPos = position;
					msg.wItemIdx = iconIdx;
					msg.ItemNum = 1;
					SendMsg(&msg, sizeof(msg));	

					LogItemMoney(
						GetID(),
						GetObjectName(),
						0,
						"",
						eLog_ShopItemUseEnd,
						GetMoney(),
						0,
						0,
						iconIdx,
						pItemBase->dwDBIdx,
						pItemBase->Position,
						0,
						1,
						GetPlayerExpPoint() );
				}
			}
		}
	}
}


BOOL CPlayer::IncreaseInventoryExpansion()
{
	if( GetInventoryExpansion() >= 2 )
		return FALSE;

	IncreaseCharacterInventory(this->GetID());

	return TRUE;
}

void CPlayer::DecreaseCount( BYTE kind )
{
	cBuffSkillObject * pSObj = NULL;

	GetBuffSkillList()->SetPositionHead();
	while( pSObj = ( cBuffSkillObject* )GetBuffSkillList()->GetData() )
	{
		pSObj->DecreaseCount( kind );
	}
}

void CPlayer::PassiveSkillCheckForWeareItem()
{
	memset( &mPassiveStatus, 0, sizeof( Status ) );
	memset( &mRatePassiveStatus, 0, sizeof( Status ) );

	SKILL_BASE* pSkillBase = NULL;

	m_SkillTree.SetPositionHead();

	BOOL WeaponCheck, ArmorCheck;

	while( pSkillBase = m_SkillTree.GetSkillBase() )
	{
		DWORD skillidx = pSkillBase->wSkillIdx - 1 + pSkillBase->Level;

		cActiveSkillInfo* pSkill = ( cActiveSkillInfo* )SKILLMGR->GetSkillInfo( skillidx );

		if( pSkill->GetKind() != SKILLKIND_PASSIVE )
			continue;

		for( DWORD cnt = 0; cnt < MAX_BUFF_COUNT; cnt++ )			
		{
			DWORD index = pSkill->GetSkillInfo()->Buff[ cnt ];		

			if( index == 0 )
			{
				continue;
			}

			WeaponCheck = FALSE;

			cBuffSkillInfo* pSkillInfo = ( cBuffSkillInfo* )SKILLMGR->GetSkillInfo( index );

			switch( pSkillInfo->GetSkillInfo()->EquipItem[0] )
			{
			case eBuffSkillWeapon_None:
				{
					WeaponCheck = TRUE;
				}
				break;
			case eBuffSkillWeapon_Sword:
				{
					if( GetWeaponEquipType() == eWeaponType_Sword )
					{
						WeaponCheck = TRUE;
					}
				}
				break;
			case eBuffSkillWeapon_Shield:
				{
					if( GetWearedItemIdx( eWearedItem_Shield ) )
					{
						WeaponCheck = TRUE;
					}
				}
				break;
			case eBuffSkillWeapon_Mace:
				{
					if( GetWeaponEquipType() == eWeaponType_Mace )
					{
						WeaponCheck = TRUE;
					}
				}
				break;
			case eBuffSkillWeapon_Axe:
				{
					if( GetWeaponEquipType() == eWeaponType_Axe )
					{
						WeaponCheck = TRUE;
					}
				}
				break;
			case eBuffSkillWeapon_Staff:
				{
					if( GetWeaponEquipType() == eWeaponType_Staff )
					{
						WeaponCheck = TRUE;
					}
				}
				break;
			case eBuffSkillWeapon_Bow:
				{
					if( GetWeaponEquipType() == eWeaponType_Bow )
					{
						WeaponCheck = TRUE;
					}
				}
				break;
			case eBuffSkillWeapon_Gun:
				{
					if( GetWeaponEquipType() == eWeaponType_Gun )
					{
						WeaponCheck = TRUE;
					}
				}
				break;
			case eBuffSkillWeapon_Dagger:
				{
					if( GetWeaponEquipType() == eWeaponType_Dagger )
					{
						WeaponCheck = TRUE;
					}
				}
				break;
			case eBuffSkillWeapon_Spear:
				{
					WeaponCheck = TRUE;
				}
				break;
			case eBuffSkillWeapon_TwoHanded:
				{
					if( GetWeaponAniType() == eWeaponAnimationType_TwoHand )
					{
						WeaponCheck = TRUE;
					}
				}
				break;
			case eBuffSkillWeapon_OneHanded:
				{
					if( GetWeaponAniType() == eWeaponAnimationType_OneHand )
					{
						WeaponCheck = TRUE;
					}
				}
				break;
			case eBuffSkillWeapon_TwoBlade:
				{
					if( GetWeaponAniType() == eWeaponAnimationType_TwoBlade )
					{
						WeaponCheck = TRUE;
					}
				}
				break;
			}

			WORD armortype = 0;
			ArmorCheck = FALSE;

			DWORD Armor = GetWearedItemIdx( eWearedItem_Dress );
			DWORD Boots = GetWearedItemIdx( eWearedItem_Shoes );
			DWORD Glove = GetWearedItemIdx( eWearedItem_Glove );

			ITEM_INFO* pArmor = ITEMMGR->GetItemInfo( Armor );
			ITEM_INFO* pBoots = ITEMMGR->GetItemInfo( Boots );
			ITEM_INFO* pGlove = ITEMMGR->GetItemInfo( Glove );
			
			if( pArmor && pBoots && pGlove )
			{
				if( ( pArmor->ArmorType == pBoots->ArmorType ) && ( pArmor->ArmorType == pGlove->ArmorType ) )
				{
					armortype = pArmor->ArmorType;
				}
			}

			switch( pSkillInfo->GetSkillInfo()->EquipItem[1] )
			{
			case eBuffSkillArmor_None:
				{
					ArmorCheck = TRUE;
				}
				break;
			default:
				{
					if( pSkillInfo->GetSkillInfo()->EquipItem[1] == armortype )
					{
						ArmorCheck = TRUE;
					}
				}
			}

			if( WeaponCheck && ArmorCheck )
			{
				pSkillInfo->AddPassiveStatus( this );
			}
		}
	}
}
/*
BOOL CPlayer::ClearInventory()
{
	for( int i = TP_INVENTORY_START ; i < TP_WEAR_START ; ++i )
	{
		ITEMBASE* pItemBase = (ITEMBASE*)m_ItemContainer.GetItemInfoAbs( i );
		if( pItemBase->dwDBIdx == 0 ) continue;

		POSTYPE	position = pItemBase->Position;
		DWORD	iconIdx = pItemBase->wIconIdx;
		int returnValue = ITEMMGR->DiscardItem( this, pItemBase->Position, pItemBase->wIconIdx, pItemBase->Durability );
		if( EI_TRUE == returnValue )
		{
			continue;
		}
	}
	return TRUE;
}
*/
void CPlayer::FishingInfoClear()
{
	SetFishingPlace(0);
	SetFishingStartTime(0);
	SetFishingProcessTime(0);
	SetFishingBait(0);

	int i;
	for(i=0; i<eFishItem_Max; i++)
		m_fFishBaitRate[i] = 0.0f;
}

void CPlayer::SetHideParts(bool* pHideParts)
{
	if(!pHideParts)		return;

	memcpy(m_HeroCharacterInfo.HideParts, pHideParts, sizeof(m_HeroCharacterInfo.HideParts));
}

// 080602 LYW --- Player : ����ġ ��ġ (__int32) ���� (__int64) ������� ���� ó��.
//void CPlayer::SetFishingExp(DWORD dwExp)
void CPlayer::SetFishingExp(EXPTYPE dwExp)
{
	const LEVELTYPE& level = m_wFishingLevel;

	if(MAX_FISHING_LEVEL <= level)
	{
		m_dwFishingExp = 0;
		return;
	}	
	
	// ����ġ�� ���� �ܰ迡�� �䱸�ϴ� �ͺ��� �ξ� ���� �� �����Ƿ�,
	// ��� üũ�ؼ� ����������
	{
		// 080602 LYW --- Player : ����ġ ��ġ (__int32) ���� (__int64) ������� ���� ó��.
		//DWORD nextPoint = GAMERESRCMNGR->GetFishingMaxExpPoint( level );
		EXPTYPE nextPoint = 0 ;
		nextPoint = GAMERESRCMNGR->GetFishingMaxExpPoint( level ) ;

		if(nextPoint == 0)
		{
			//MessageBox( NULL, "Error occurred!!", __FUNCTION__, MB_OK) ;
			return ;
		}

		while( dwExp >= nextPoint )
		{
			m_wFishingLevel++;

			Log_Fishing(
				GetID(),
				eFishingLog_SetLevel,
				0,
				GetFishPoint(),
				0,
				0,
				GetFishingExp(),
				GetFishingLevel() );

			//CObjectEvent::ObjectEvent( this, OE_LEVELUP, &m_HeroCharacterInfo.Level );
			MSG_WORD msg;
			msg.Category = MP_FISHING;
			msg.Protocol = MP_FISHING_LEVELUP_ACK;
			msg.wData = m_wFishingLevel;
			SendMsg(&msg, sizeof(msg));	

			dwExp		-=	nextPoint;
			nextPoint	=	GAMERESRCMNGR->GetFishingMaxExpPoint( level );
		}

		m_dwFishingExp = dwExp;
	}
}

// 080509 LUJ, ��ų ��Ÿ���� ������ �ʾ����� ���� ��ȯ�Ѵ�
// 080514 LUJ, ��ų �ִϸ��̼� �ð� üũ
// 080515 LUJ, ��Ÿ�Ӱ� �ִϸ��̼� �ð� üũ�� ���� ��ų�� �󸶳� �����ϴ��� �����ϱ� ���� �α׸� �ۼ��Ѵ�
// 080516 LUJ, ��Ÿ�� üũ ���а� ��� ���� �̻��� �� ������ �����Ŵ
BOOL CPlayer::IsCoolTime( const ACTIVE_SKILL_INFO& skill )
{
	// 080515 LUJ, ��Ÿ�� �α׸� ���� Ŭ���� Ŭ����
	static struct
	{
	public:
		void Try()
		{
			++mTryCount;
		}

		void Fail( DWORD playerIndex, DWORD skillIndex, DWORD animationTime, DWORD coolTime )
		{
			++mFailCount;

			// 080515 LUJ, ���Ͽ� �α׸� ����Ѵ�
			{
				SYSTEMTIME time = { 0 };
				GetLocalTime( &time );

				char buffer[ MAX_PATH ] = { 0 };
				sprintf(
					buffer,
					"log/SkillCoolTime.%04d%02d%02d.log",
					time.wYear,
					time.wMonth,
					time.wDay );

				FILE* file = fopen( buffer, "r" );

				if( ! file )
				{
					file = fopen( buffer, "a" );

					if( ! file )
					{
						return;
					}

					// 080516 LUJ, �α׿� �� ��ȣ �߰�
					const char* title = "Time\tPlayer\tSkill\tRemained ani.tick\tRemaind cool tick\tFailure/Try\tMap\n";

					fwrite(
						title,
						sizeof( *title ),
						strlen( title ),
						file );
				}

				fclose( file );
				file = fopen( buffer, "a" );

				if( ! file )
				{
					return;
				}
				
				// 080516 LUJ, �α׿� �� ��ȣ �߰�
				sprintf(
					buffer,
					"%02d:%02d:%02d\t%u\t%u\t%u\t%u\t[%u/%u]\t%d\n",
					time.wHour,
					time.wMinute,
					time.wSecond,
					playerIndex,
					skillIndex,
					gCurTime > animationTime	? 0 : animationTime - gCurTime,
					gCurTime > coolTime			? 0 : coolTime - gCurTime,
					mFailCount,
					mTryCount,
					g_pServerSystem->GetMapNum() );

				fwrite(
					buffer,
					sizeof( *buffer ),
					strlen( buffer ),
					file );
				
				fclose( file );
			}
			
		}

	private:
		DWORD mTryCount;
		DWORD mFailCount;
	}
	log;

	log.Try();

	// 080516 LUJ, ��Ÿ�� ���а� �߻��ص� ���� ȸ�� �̻��� ����Ѵ�. �� �̻��� �߻��ϸ� ������ ������ �����Ų��
	struct
	{
		void operator() ( CPlayer& player, CPlayer::CheckCoolTime& checkCoolTime )
		{
			const DWORD maxCheckTick = 1000 * 60;

			// 080516 LUJ, ��Ÿ�� ���а� �߻��� �� ������ �ð��� �������� üũ �����͸� �ʱ�ȭ�Ѵ�
			// 080519 LUJ, maxCheckTick ���� �߻��� ���� üũ�� ���� ���ϴ� �� ����
			if( maxCheckTick < ( checkCoolTime.mCheckedTick - gCurTime ) )
			{
				// 080519 LUJ, üũ �ð��� ���ݺ��� maxCheckTick�������� �Ѵ�
				checkCoolTime.mCheckedTick	= gCurTime + maxCheckTick;
				checkCoolTime.mFailureCount	= 0;
			}

			const DWORD maxCheckCount = 10;

			// 080516 LUJ, ��Ÿ�� ���а� ��� ȸ�� �����̸�, ó������ �ʴ´�
			if( maxCheckCount > ++checkCoolTime.mFailureCount )
			{
				return;
			}

			// 080516 LUJ, ���� ȸ���̻� ������ ��� ������ �����Ų��
			{
				MSG_DWORD message;
				ZeroMemory( &message, sizeof( message ) );
				message.Category	= MP_USERCONN;
				message.Protocol	= MP_USERCONN_GAMEIN_NACK;
				message.dwData		= player.GetID();
				
				g_Network.Broadcast2AgentServer( (char*)&message, sizeof( message ) );
			}
		}
	}
	Punish;

	// 080514 LUJ, �ִϸ��̼��� ǥ�õǴ� �������� ��ų�� ����� �� ����
	if( mSkillAnimationTime > gCurTime )
	{
		// 080516 LUJ, ��Ÿ�� üũ�� ���� ���� �̻� �������� �� ��Ģ�� �ο��Ѵ�
		Punish( *this, mCheckCoolTime );

		// 080515 LUJ, �˻� ����� ���� �α׸� �ۼ��Ѵ�
		log.Fail( GetID(), skill.Index, mSkillAnimationTime, 0 );

		// 080519 LUJ, ��Ÿ�� üũ ���� ���� ��� �����ϰ� ��ȯ�Ѵ�. ���� ȸ�� �̻� ���� �� ��Ģ�� �ο��ϱ� ����
		return FALSE;
	}

	// 080617 LUJ, ������ ������ ����
	const SkillCoolTimeMap::const_iterator it = mSkillCoolTimeMap->find( skill.Index );

	// 080509 LUJ, ���� ��Ÿ�� ������ üũ�Ѵ�
	if( mSkillCoolTimeMap->end() == it )
	{
		return FALSE;
	}

	const DWORD endTime		= it->second;
	const BOOL	isCoolTime	=  endTime > gCurTime;

	if( isCoolTime )
	{
		// 080516 LUJ, ��Ÿ�� üũ�� ���� ���� �̻� �������� �� ��Ģ�� �ο��Ѵ�
		Punish( *this, mCheckCoolTime );

		// 080515 LUJ, �˻� ����� ���� �α׸� �ۼ��Ѵ�
		log.Fail( GetID(), skill.Index, 0, endTime );
	}
	
	// 080519 LUJ, ��Ÿ�� üũ ���� ���� ��� �����ϰ� ��ȯ�Ѵ�. ���� ȸ�� �̻� ���� �� ��Ģ�� �ο��ϱ� ����
	return FALSE;
}

// 080511 LUJ, ��ų ��Ÿ���� �߰��Ѵ�
// 080514 LUJ, ��ų �ִϸ��̼� ���� �ð� ����
// 080605 LUJ, ��ų ������ ���� �ִϸ��̼� �ð��� �����Ѵ�
void CPlayer::SetCoolTime( const ACTIVE_SKILL_INFO& skill )
{
	// 080605 LUJ, �ִϸ��̼� Ÿ���� ��ų ������ ���� ������Ű�� ���� ���� ����
	float animationTime = float( skill.AnimationTime );

	// 080605 LUJ, ��ų ������ ���� �ִϸ��̼� Ÿ���� ������ �����Ѵ�.
	{
		const Status* ratePassiveStatus = GetRatePassiveStatus();
		const Status* rateBuffStatus	= GetRateBuffStatus();

		if( rateBuffStatus &&
			ratePassiveStatus )
		{
			if( ( skill.Index / 100000 ) % 10 )
			{
				switch( skill.Unit )
				{
				case UNITKIND_MAGIC_ATTCK:
					{
						animationTime = animationTime * ( 1.0f - ( rateBuffStatus->MagicSkillSpeedRate + ratePassiveStatus->MagicSkillSpeedRate ) / 100.0f );
						break;
					}
				case UNITKIND_PHYSIC_ATTCK:
					{
						animationTime = animationTime * ( 1.0f - ( rateBuffStatus->PhysicSkillSpeedRate + ratePassiveStatus->PhysicSkillSpeedRate ) / 100.0f );
						break;
					}
				}
			}
			else
			{
				animationTime = animationTime * ( 1.0f - ( rateBuffStatus->NormalSpeedRate + ratePassiveStatus->NormalSpeedRate ) / 100.0f );
			}
		}
	}
	
	// 080514 LUJ, �ִϸ��̼��� ������ �ð��� �����Ѵ�. ��Ʈ��ũ ������ �����Ͽ� 0.1�� ������ ����Ѵ�
	// 080520 LUJ, �׽�Ʈ ����� 0.1->0.3�ʷ� ��� �ð� ����
	// 080605 LUJ, �ּ� 0�� ���� ����Ѵ�. animationTime�� �Ǽ��� ����Ǿ� �����÷� ������ �ֱ� ����
	mSkillAnimationTime = DWORD( max( 0, animationTime ) ) + gCurTime - 300;

	// 080511 LUJ, ��Ʈ��ũ ������ �����Ͽ� 0.5�� ������ ����Ѵ�
	// 080514 LUJ, ��Ʈ��ũ ������ ���� ��� ������ 0.5 -> 0.1�ʷ� ���δ�.
	// 080520 LUJ, �׽�Ʈ ����� 0.1->0.3�ʷ� ��� �ð� ����
	// 080617 LUJ, ������ ������ ����
	( *mSkillCoolTimeMap )[ skill.Index ] = skill.CoolTime + gCurTime - 300;	
}
