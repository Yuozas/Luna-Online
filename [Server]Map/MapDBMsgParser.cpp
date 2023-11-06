#include "stdafx.h"
#include "QuickSlot.h"
#include "CommonDBMsgParser.h"
#include "MapDBMsgParser.h"
#include "UserTable.h"
#include "Network.h"
#include "ItemManager.h"
#include "SkillTreeManager.h"
#include "Economy.h"
#include "PartyManager.h"
#include "Party.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "CharMove.h"
#include "GuildManager.h"
#include "StreetStallManager.h"
#include "FameManager.h"
#include "CQuestBase.h"
#include "TileManager.h"
/*어빌리티 삭제 - 이영준 #include "AbilityManager.h"*/
#include "StorageManager.h"
#include "WantedManager.h"
#include "GuildFieldWarMgr.h"
#include "GuildManager.h"
#include "GuildScore.h"
#include "QuestManager.h"
#include "ShopItemManager.h"
#include "GuildTournamentMgr.h"
#include "GuildUnionManager.h"
#include "SiegeWarMgr.h"
#include "SiegeWarProfitMgr.h"
#include "cJackpotManager.h"
#include "../[cc]skill/server/tree/SkillTree.h"
#include "../[cc]skill/server/manager/skillmanager.h"
#include "Guild.h"

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.04.23
#include "../hseos/Farm/SHFarmManager.h"
// E 농장시스템 추가 added by hseos 2007.04.23

// desc_hseos_패밀리01
// S 패밀리 추가 added by hseos 2007.07.06
#include "../hseos/Family/SHFamilyManager.h"
// E 패밀리 추가 added by hseos 2007.07.06

#include "QuickManager.h"

#include "CharacterCalcManager.h"

// 107207 LYW --- MapDBMsgParser : 스탯, 스킬 초기화 시, 캐릭터 로그아웃 처리를 위해 추가.
#include "FriendManager.h"

// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.12.02
#include "../hseos/Date/SHDateManager.h"
// E 데이트 존 추가 added by hseos 2007.12.02
// 080408 LUJ, 스킬 초기화 시 버프 스킬 제거위해 참조
#include "../[cc]skill/Server/Info/ActiveSkillInfo.h"
// 080602 LUJ, 스킬 사용 시 소모 기능위해 참조
#include "../[cc]skill/Server/Object/ActiveSkillObject.h"
// 080616 LUJ, 함정에 아군 추가하기위해 참조
#include "Trap.h"

#include "PetManager.h"
#include "pet.h"
// 080722 LYW --- MapDBMsgParser : 버프 스킬 헤더 포함.
#include "../[cc]skill/server/info/buffskillinfo.h"

// 080204 KTH -- ServerSetNum
extern int g_nServerSetNum;


DBMsgFunc g_DBMsgFunc[MaxQuery] =
{
	RCharacterNumSendAndCharacterInfo,
	RCharacterSkillTreeInfo,
	RCharacterItemSlotInfo,				// eCharacterItemSlotQuery
	RCharacterItemInfo,
	RCharacterShopItemInfo,				// eShopItemInfoQuery
	//	RCharacterPetInfo,

	RSSItemInsert,
	
	/* New Item Work */
	NULL,						// ItemCombineUpdateToDB
	NULL,						// ItemMoveUpdateToDB
	NULL,						// ItemMoveStorageUpdate
	NULL,						// ItemMoveGuildUpdate
	RItemInsert,				// ItemInsertToDB
	NULL,						// ItemDeleteToDB
	RShopItemInvenInfo,			// eItemShopInvenInfo,
	RUsingShopItemInfo,			// eItemShopItemUseInfo
	NULL,						// eItemShopItemUsing    RaMa - 04.11.05
	NULL,						// eItemShopItemDelete
	NULL,						// eItemShopItemUpdatetime
	NULL,						// eItemShopItemUpdateParam
	NULL,						// eItemShopItemUpdateUseParam
	NULL,						// eItemShopItemUpdateUseInfo
	NULL,						// eItemShopItemMPInfo
	NULL,						// eItemShopMPUpdate
	NULL,						// eItemShopMPDel
	NULL,						// eItemShopGetItem
	NULL,						// eItemMoveShopUpdate
	RCharacterChangeName,		// eItemShopChangeName
	NULL,						// eItemCharChange
	
	NULL,						// RegistLoginMapInfo
	NULL,						// UnRegistLoginMapInfo

	NULL,						// SkillUpdateToDB
	NULL,						// SKillMoveUpdateToDB
	RSkillInsert,				// SkillInsertToDB
	NULL,						// SkillDeleteToDB

	//	NULL,
	RMapBaseEconomy,
	NULL,
	NULL,
	NULL,			/// HeroInfo
	NULL,			/// TotalInfo
	NULL,			// eBadFameUpdate,

	RAuctionCheck,
	RAuctionPageList,
	RAuctionConfirm,
	RPartyLoad,
	RPartyInfoByUserLogin,
	RPartyCreate,
	NULL,
	NULL,
	NULL,
	RPartyDelMember,	// PartyDelMember
	RPartyChangeMaster, //PartyChangeMaster

	NULL,
	RSaveMapChangePointReturn,

	NULL,
	RCharacterStorageInfo,			// eCharacterStorageInfo
	RCharacterStorageItemInfo,		// eCharacterStorageItemInfo	
	NULL,					// StorageMoneyUpdateToDB

	RFriendNotifyLogouttoClient,	
	NULL,
	NULL,					//eBadFameCharacterUpdate
	NULL,					//ePKCharacterUpdate

	RWantedLoad,
	RWantedInfoByUserLogIn,
	NULL,
	RWantedRegist,
	NULL,
	RWantedComplete,
	RWantedDestroyed,
	RWantedOrderedList,
	RWantedMyList,
	RWantedSearch,

	RQuestTotalInfo,		// eQuestTotalInfo                         
	NULL,					// eQuestUpdate
	RQuestMainQuestLoad,	// eQuestMainQuestLoad
	RQuestSubQuestLoad,		// eQuestSubQuestLoad
	RQuestItemLoad,			// eQuestItemLoad
	NULL,					// eQuestMainQuestInsert
	NULL,					// eQuestSubQuestInsert
	NULL,					// eQuestItemInsert
	NULL,					// eQuestMainQuestUpdate
	NULL,					// eQuestSubQuestUpdate
	NULL,					// eQuestItemUpdate
	NULL,					// eQuestMainQuestDelete
	NULL,					// eQuestSubQuestDelete
	NULL,					// eQuestSubQuestDeleteAll	
	NULL,					// eQuestItemDelete
	NULL,					// eQuestItemDeleteAll
	NULL,					// eQuestMainQuestUpdateCheckTime

	RJournalGetList,		// eJournalGetList,
	NULL,					// eJournalInsertQuest,
	NULL,					// eJournalInsertWanted,
	NULL,					// eJournalInsertLevel,
	NULL,					// eJournalSaveUpdate,
	NULL,					// eJournalDelete,
	
	NULL,					// eCharacterInfoByTime,
	NULL,					// eSkillExpByTime,
	NULL,					// eSpeedHackCheck,

	RGuildSelectForReward,	// eGuildSelectForReward
	RGuildItemSelect,		// eGuildItemSelect
	
	RGuildLoadGuild,		// eGuildLoadGuild, 
	RGuildLoadMember,		// eGuildLoadMember,
	RGuildBreakUp,			// eGuildBreakUp,
	RGuildDeleteMember,		// eGuildDeleteMember,
	RGuildSecedeMember,		// eGuildSecedeMember
	NULL,					// eGuildChangeMemberRank
	NULL,					// eGuildGiveNickName
	RGuildAddScore,				//eGuildAddScore
	RGuildUpdateScore,			//eGuildUpdateScore 

	RGuildTournamentInfoLoad,			// eGuildTournament_LoadAll,
	RGuildTournamentCancelIdxLoad,		// eGuildTournament_CancelLoad,
	NULL,								// eGuildTournament_CancelInsert
	NULL,								// eGuildTournament_Insert,
	NULL,								// eGuildTournament_Delete,
	NULL,								// eGuildTournament_UpdateGuildInfo,
	NULL,								// eGuildTournament_UpdateTournamentInfo,

	RConfirmUserOut,		// eConfirmUserOut
	RSaveCharInfoBeforeLogout,

	RCharacterInfoQueryToEventMap, //eCharacterInfoQueryToEventMap

	RLoadGuildWarRecord,	// eLoadGuildWarRecord	
	NULL,					// eGuildWarRecordDelete
	RLoadGuildWar,			// eLoadGuildWar
	NULL,					// eInsertGuildFieldWar
	NULL,					// eDeleteGuildFieldWar
	NULL,					// eUpdateGuildFieldWarRecord

	//	NULL,	//	RJackpotLoadTotalMoney,				// eJackpot_TotalMoney_Load
	NULL,	//	RJackpotAddTotalMoney,				//	eJackpot_MapMoney_Update,
	RJackpotPrizeInfo,					//	eJackpot_PrizeInfo_Send,

	RGuildUnionLoad,					// eGuildUnionLoad
	RGuildUnionCreate,					// eGuildUnionCreate
	NULL,								// eGuildUnionDestroy	
	NULL,								// eGuildUnionAddGuild
	NULL,								// eGuildUnionRemoveGuild
	NULL,								// eGuildUnionSecedeGuild

	RSiegeWarInfoLoad,					// eSiegeWarInfoLoad
	NULL,								// eSiegeWarInfoInsert
	NULL,								// eSiegeWarInfoUpdate,
	RSiegeWarGuildInfoLoad,				// eSiegeWarGuildInfoLoad
	NULL,								// eSiegeWarGuildInsert,
	NULL,								// eSiegeWarGuildUpdate,
	NULL,								// eSiegeWarGuildDelete,

	NULL,								// eChangeCharacterAttr

	RSiegeWarProfitInfoLoad,			// eSiegeWarProfitInfoLoad
	NULL,								// eSiegeWarProfitUpdateGuild
	NULL,								// eSiegeWarProfitUpdateTaxRate
	RSiegeWarProfitUpdateProfitMoney,	// eSiegeWarProfitUpdateProfitMoney

	NULL,								// eChangeGuildLocation
	/*어빌리티 삭제 - 이영준 NULL,								// eChangeCharacterStageAbility*/

	//NULL,								// eGuildLoadNotice,
	NULL,								// eGuildUpdateNotice,

	RQuestEventCheck,					// eQuestEventCheck
	NULL,								// eQuestEventEnd

	NULL,								//eGMEvent01

	NULL,								// eGuildTournamentAddLog
	NULL,								// eSiegeWarAddLog

	RQuickInfo,							// eQuickInfo
	NULL,

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.04.23	2007.09.12	2008.01.16
	RFarm_LoadFarmState,				// eFarm_LoadFarmState
	RFarm_LoadCropInfo,					// eFarm_LoadCropInfo
	RFarm_LoadTimeDelay,				// eFarm_LoadTimeDelay
	NULL,								// eFarm_SetFarmState
	NULL,								// eFarm_SetCropInfo
	NULL,								// eFarm_SetTimeDelay
	NULL,								// eFarm_SetTaxInfo
	// E 농장시스템 추가 added by hseos 2007.04.23	2007.09.12	2008.01.16

	// desc_hseos_몬스터미터01
	// S 몬스터미터 추가 added by hseos 2007.05.28
	RMonsterMeter_LoadInfo,				// eMonsterMeter_LoadInfo
	NULL,								// eMonsterMeter_SetInfo
	// E 몬스터미터 추가 added by hseos 2007.05.28

	NULL,
	NULL,

	NULL,
	
	// 070918 LYW --- MapDBMsgParser : Add function pointer.
	RSaveNpcMapChangePointReturn,

	// 071023 LYW --- MapDBMsgParser : 튜토리얼 로드 결과처리를 할 함수 포인터.
	RTutorialLoad,

	// 071023 LYW --- MapDBMsgParser : 튜토리얼 업데이트 결과 처리를 할 함수 포인터.
	RTutorialUpdate,

	// 071106 LYW --- MapDBMsgParser : 기본 스탯으로 업데이트 하는 함수 포인터.
	RUpdateToDefaultStats,

	// 071117 LYW --- MapDBMsgParser : 퀘스트 관련 추가 함수.
	NULL,	//eEndQuest_New,
	NULL,	//eEndSubQuest_New,
	NULL,	//eDeleteQuest_New,

	RCharacterBuffLoad,					//eCharacterBuffLoad
	NULL,								//eCharacterBuffAdd
	NULL,								//eCharacterBuffRemove
	NULL,								//eCharacterBuffUpdate

	// 071122 LYW --- MapDBMsgParser : 봉인 해제 처리.
	NULL,	//eUnSealItemByRealTime,

	NULL,	// eUnSealItemByGameTime,

	NULL,	// eUpdateRemainTime,

	// 071129 LYW --- MapDBMsgParser : 스킬 초기화.
	RSkill_Reset,	//eSkill_Reset,

	// 071129 LYW --- MapDBMsgParser : 누적 스킬 포인트를 업데이트.
	NULL,	// eUpdateAccumulateSkillpoint,

	NULL,

	RIncreaseCharacterInventory,		// eIncreaseCharacterInventory
	NULL,								// eResetCharacterInventory
	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.26		2008.01.23
	NULL,								// eChallengeZone_SavePos_SetInfo

	RChallengeZone_EnterFreq_Load,		// eChallengeZone_EnterFreq_LoadInfo
	NULL,								// eChallengeZone_EnterFreq_SetInfo

	NULL,								// eChallengeZone_Success_SetInfo

	RChallengeZone_ClearNo1_Load,		// eChallengeZone_ClearNo1_LoadInfo
	// E 데이트 존 추가 added by hseos 2007.11.26		2008.01.23

	// desc_hseos_결혼_01
	// S 결혼 추가 added by hseos 2007.12.11
/*	RMarriage_Load,						// eMarriage_LoadInfo
	NULL,								// eMarriage_SetInfo

	NULL,								// eMarriage_Dirvorce_SetInfo
/*
	NULL,								// eMarriage_TeleportFreq_SetInfo

	NULL,								// eMarriage_Nickname_SetInfo
	// E 결혼 추가 added by hseos 2007.12.11

		NULL,								// eMarriage_TeleportFreq_SetInfo

		NULL,								// eMarriage_Nickname_SetInfo
		// E 결혼 추가 added by hseos 2007.12.11
*/
		RChangeCharacterName,				// eCharacterChangeName
//		RMemoryStoneInfoLoad,				// eMemoryStoneInfoLoad
//		NULL,								// eMemoryStoneInfoAdd
//---KES AUTONOTE
		RAutoNoteListLoad,					// eAutoNoteList_Load,	//---자신이 잡은 오토
		RAutoNoteListAdd,					// eAutoNoteList_Add,	//---자신이 잡은 오토
//---------------
		NULL,
	NULL, //RResidentRegist_ResetResult,				// eResidentRegist_Reset

	RGiftItemInsert,	//eGiftItemInsert
	
	RPetInfoLoad,
	RPetInfoAdd,
	NULL,
	NULL,
	NULL,

	NULL,
	RPetItemInfoLoad,

	NULL, //eFarm_SetAnimalInfo,
	RFarm_LoadAnimalInfo,	//eFarm_LoadAnimalInfo
};


char txt[512];

void CheckAuction(MSGBASE* msg)
{
	//	char txt[128];
	int CharacterIDX = msg->dwObjectID;
	sprintf(txt, "EXEC %s %d", STORED_AUCTION_CHECK, CharacterIDX);	
	g_DB.Query(eQueryType_FreeQuery,eAuctionCheck,CharacterIDX,txt,CharacterIDX);

}
void AuctionSearch(SEARCHLIST* msg)
{
	//	char txt[128];
	int searchtype;
	char name[15];

	strcpy(name,((char*)msg->name));
	searchtype = msg->search_type;
	int CharacterIDX = msg->dwObjectID;
	sprintf(txt, "EXEC %s %d \'%s\'", STORED_AUCTION_SEARCH, searchtype, name );
	g_DB.Query(eQueryType_FreeQuery,eAuctionPageList,CharacterIDX,txt,CharacterIDX);
}

void AuctionSort(SORTLIST* msg)
{
	//	char txt[128];
	int type;
	int page;
	type = msg->sort_type;
	page = msg->pagenum;
	int CharacterIDX = msg->dwObjectID;
	sprintf(txt, "EXEC %s %d \'%s\'", STORED_AUCTION_SEARCH, type, page );
	g_DB.Query(eQueryType_FreeQuery,eAuctionPageList,CharacterIDX,txt,CharacterIDX);
}

void AuctionRegister(REGISTERAUCTION* msg)
{
	//	char txt[128];
	int  index = msg->PRO_Index;
	int  amount = msg->PRO_Amount;
	char duedate[10];
	strcpy(duedate,msg->BID_DUEDATE);
	int  price = msg->BID_HighPrice;
	int  immediate = msg->BID_ImmediatePrice;
	char name[15];
	strcpy(name,msg->Auctioneer);

	DWORD characterIDX = msg->dwObjectID;
	int CharacterIDX = msg->dwObjectID;
	sprintf(txt, "EXEC %s %d %d %s %d %d %s", STORED_AUCTION_REGISTER,index,amount,duedate,price,immediate,name);
	g_DB.Query(eQueryType_FreeQuery,eAuctionConfirm,CharacterIDX,txt,CharacterIDX);
}

void AuctionJoin(JOINAUCTION* msg)
{
	//	char txt[128];
	int index = msg->REG_Index;
	int price = msg->BID_Price;
	char name[15];
	strcpy(name,msg->JOIN_Name);
	int CharacterIDX = msg->dwObjectID;
	sprintf(txt, "EXEC %s %d %d %s", STORED_AUCTION_JOIN, index, price, name);
	g_DB.Query(eQueryType_FreeQuery,eAuctionConfirm,CharacterIDX,txt,CharacterIDX);
}

void AuctionCancel(AUCTIONCANCEL *msg)
{
	//	char txt[128];
	int type = 0 ;
	int regindex = 0;
	char name[15];

	int CharacterIDX = msg->dwObjectID;
	sprintf(txt, "EXEC %s %d %d %s", STORED_AUCTION_CANCEL, type, regindex, name);
	g_DB.Query(eQueryType_FreeQuery,eAuctionConfirm,CharacterIDX,txt,CharacterIDX);
}


void CharacterNumSendAndCharacterInfo(DWORD CharacterIDX, DWORD Protocol)
{
	sprintf(txt, "EXEC %s %d", STORED_CHARACTER_SELECT, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterInfoQuery, Protocol, txt, CharacterIDX);

	Option_Load(CharacterIDX);
}

void CharacterNumSendAndCharacterInfo2(DWORD CharacterIDX, DWORD Protocol)
{
	sprintf(txt, "EXEC %s %d", STORED_CHARACTER_SELECT, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterInfoQueryToEventMap, Protocol, txt, CharacterIDX);
}

void CharacterSkillTreeInfo(DWORD CharacterIDX, DWORD Protocol)
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_MAPCHANGE_SKILLTREEINFO, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterSkillTreeQuery, Protocol, txt, CharacterIDX);
}

void CharacterItemSlotInfo(DWORD CharacterIDX)
{
	sprintf(txt, "EXEC %s %d", STORED_CHARACTER_ITEMSLOTINFO, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterItemSlotQuery, CharacterIDX, txt, CharacterIDX);
}

void CharacterItemInfo(DWORD CharacterIDX, DWORD Protocol)
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_CHARACTER_ITEMINFO, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterItemQuery, Protocol, txt, CharacterIDX);
}

void ShopItemInvenInfo( DWORD CharacterIdx)
{
	sprintf(txt, "EXEC %s %d", STORED_SHOPINVEN_INFO, CharacterIdx);
	g_DB.Query(eQueryType_FreeQuery, eItemShopInvenInfo, CharacterIdx, txt, CharacterIdx);
}

// RaMa 04.11.04
void	CharacterShopItemInfo(DWORD CharacterIDX, DWORD StartDBIdx)
{
	sprintf(txt, "EXEC %s %d, %d", STORED_SHOPITEM_INFO, CharacterIDX, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eShopItemInfoQuery, CharacterIDX, txt, CharacterIDX);
}

////////////////////////////////////////////////////////////////////////////////
// RaMa 04.11.05
void UsingShopItemInfo( DWORD CharacterIdx )
{
	sprintf(txt, "EXEC %s %d", STORED_SHOPITEM_USEINFO, CharacterIdx);
	g_DB.Query(eQueryType_FreeQuery, eItemShopItemUseInfo, CharacterIdx, txt, CharacterIdx);
}

void SavedMovePointInfo( DWORD CharacterIdx )
{	
	g_DB.FreeMiddleQuery( RSavedMovePointInfo, CharacterIdx, 
		"EXEC %s %d", STORED_SHOPITEM_MPINFO, CharacterIdx );
}

void SavedMovePointInsert( DWORD CharacterIdx, char* Name, DWORD MapIdx, DWORD Position )
{	
	g_DB.FreeMiddleQuery( RSavedMovePointInsert, CharacterIdx, 
		"EXEC %s %d, \'%s\', %d, %d", STORED_SHOPITEM_MPINSERT, CharacterIdx, Name, MapIdx, Position, CharacterIdx );
}

void SavedMovePointUpdate( DWORD CharacterIdx, DWORD DBIdx, char* Name )
{
	sprintf(txt, "EXEC %s %d, %d, \'%s\'", STORED_SHOPITEM_MPUPDATE, CharacterIdx, DBIdx, Name);
	g_DB.Query(eQueryType_FreeQuery, eItemShopMPUpdate, 0, txt, CharacterIdx);
}

void SavedMovePointDelete( DWORD CharacterIdx, DWORD DBIdx )
{
	sprintf(txt, "EXEC %s %d, %d", STORED_SHOPITEM_MPDEL, CharacterIdx, DBIdx);
	g_DB.Query(eQueryType_FreeQuery, eItemShopMPDel, 0, txt, CharacterIdx);
}

void UsingShopItemUpdateToDB( DWORD UserIdx, DWORD CharacterIdx, DWORD dwItemIdx, DWORD dwDBIdx, DWORD Param, DWORD StartTime, DWORD RemainTime )
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_SHOPITEM_USING, CharacterIdx, dwItemIdx, dwDBIdx, Param, StartTime, RemainTime);
	g_DB.Query(eQueryType_FreeQuery, eItemShopItemUsing, 0, txt, CharacterIdx);
}
void ShopItemDeleteToDB( DWORD CharacterIdx, DWORD dwDBIdx )
{
	sprintf(txt, "EXEC %s %d, %d", STORED_SHOPITEM_DELETE, CharacterIdx, dwDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eItemShopItemDelete, 0, txt, CharacterIdx);
}

void ShopItemUpdatetimeToDB( DWORD CharacterIdx, DWORD dwItemIdx, DWORD RemainTime )
{
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_SHOPITEM_UPDATETIME, CharacterIdx, dwItemIdx, RemainTime);
	g_DB.Query(eQueryType_FreeQuery, eItemShopItemUpdatetime, 0, txt, CharacterIdx);
}

void ShopItemParamUpdateToDB( DWORD CharacterIdx, DWORD dwItemDBIdx, DWORD Param )
{
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_SHOPITEM_UPDATEPARAM, CharacterIdx, dwItemDBIdx, Param);
	g_DB.Query(eQueryType_FreeQuery, eItemShopItemUpdateParam, 0, txt, CharacterIdx);
}

void ShopItemUseParamUpdateToDB( DWORD UserIdx, DWORD CharacterIdx, DWORD dwItemIdx, DWORD Param )
{
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_SHOPITEM_UPDATEUSEPARAM, CharacterIdx, dwItemIdx, Param);
	g_DB.Query(eQueryType_FreeQuery, eItemShopItemUpdateUseParam, 0, txt, CharacterIdx);
}

void ShopItemUpdateUseInfoToDB( DWORD CharacterIdx, DWORD dwDBIdx, DWORD Param, DWORD RemainTime )
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d", STORED_SHOPITEM_UPDATEUSEINFO, CharacterIdx, dwDBIdx, Param, RemainTime);
	g_DB.Query(eQueryType_FreeQuery, eItemShopItemUpdateUseInfo, 0, txt, CharacterIdx);
}

void ShopItemGetItem( DWORD CharacterIdx, WORD wItemIdx, DWORD Dur )
{
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_SHOPITEM_GETITEM, CharacterIdx, wItemIdx, Dur);
	g_DB.Query(eQueryType_FreeQuery, eItemShopGetItem, 0, txt, CharacterIdx);
}

void ItemMoveShopUpdateToDB(DWORD CharacterIDX, DWORD UserIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos)
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_ITEM_MOVEUPDATE_SHOP, CharacterIDX, UserIDX, dwfromDBIdx, frompos, dwtoDBIdx, topos);
	g_DB.Query(eQueryType_FreeQuery, eItemMoveShopUpdate, 0, txt, CharacterIDX);
}

//
/*어빌리티 삭제 - 이영준 
void CharacterAbilityInfo(DWORD CharacterIDX, DWORD Protocol)
{
	g_DB.FreeMiddleQuery(RCharacterAbilityInfo, Protocol, 
		"MP_CHARACTER_SelectAbility %d",CharacterIDX);
}
*/
/*
void CharacterGMControlInfo(DWORD CharacterIDX, DWORD Protocol)
{
	g_DB.FreeMiddleQuery(RCharacterGMControlInfo, Protocol, 
		"MP_GM_Control_GetList %d",CharacterIDX);
}

//TDN
void CharacterGMControlInsert(DWORD GMChrIdx,DWORD CharacterIDX, DWORD UserIdx, int kind, )
{
	g_DB.FreeMiddleQuery(RCharacterGMControlInfo, GMChrIdx, 
		"MP_GM_Control_Insert %d",GMChrIdx,CharacterIDX,UserIdx,kind,);
}
void CharacterGMControlDelete(DWORD GMChrIdx,DWORD ControlIdx)
{
	g_DB.FreeMiddleQuery(NULL, GMChrIdx, 
		"MP_GM_Control_Delete %d",);
}
*/

/*어빌리티 삭제 - 이영준 
void AbilityLevelupLog(CPlayer* pPlayer,WORD AbilityIdx,LEVELTYPE AbilityLevel,EXPTYPE FromExp,EXPTYPE ToExp)
{
	sprintf(txt,
		"INSERT TB_AbilityLog \
		(character_idx,ability_idx,ability_level_to,ability_exp_from,ability_exp_to,logdate) \
		values (%d, %d, %d, %u, %u, getdate())", 
		pPlayer->GetID(), AbilityIdx, AbilityLevel, FromExp, ToExp);

	g_DB.LogQuery(eQueryType_FreeQuery, 0, 0, txt);
}
*/
// 071126 Add Sealed, RemainSecond
void SSItemInsert(DWORD CharacterIdx, DWORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, DWORD FromChrIdx, WORD wSeal, int Sealed, int RemainSecond)
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_ITEM_INSERT_INVEN, CharacterIdx, wItemIdx, Durability, bPosition, wSeal, Sealed, RemainSecond);
	g_DB.Query(eQueryType_FreeQuery, eSSItemInsert, FromChrIdx, txt, CharacterIdx);
}

// description by hseos S
// 유저의 아이템 정보를 DB에 설정
// description by hseos E
void ItemUpdateToDB(DWORD CharacterIdx, DWORD dwDBIdx, DWORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, WORD qPosition )
{
	g_DB.FreeMiddleQuery(
		0,
		0,
		"EXEC DBO.MP_ITEM_Update %d, %d, %d, %d, %d, %d, %d", 
		CharacterIdx,
		dwDBIdx,
		wItemIdx,
		Durability,
		bPosition,
		qPosition,
		0 );
}

/* bDel:0 fromItem Update, bDel:1 fromItem Delete */
void ItemCombineUpdateToDB(DWORD dwfromDBIdx, DURTYPE fromDur, DWORD dwToDBIdx, DURTYPE toDur, DWORD CharacterIdx)
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d", STORED_ITEM_COMBINEUPDATE, dwfromDBIdx, fromDur, dwToDBIdx, toDur);
	g_DB.Query(eQueryType_FreeQuery, eItemCombineUpdate, 0, txt, CharacterIdx);
}

void ItemMoveUpdateToDB(DWORD CharacterIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos, DWORD UserIdx, DWORD MunpaIdx)
{
	// 080410 LUJ, 영역 정보가 바뀌어도 관계없도록 인자로 받도록 함
	sprintf(
		txt,
		"EXEC dbo.MP_ITEM_MoveUpdate %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
		dwfromDBIdx,
		frompos,
		dwtoDBIdx,
		topos,
		CharacterIDX,
		UserIdx,
		MunpaIdx,
		TP_INVENTORY_START,
		// 080611 LUJ, 장비칸을 고려하지 않아 DB 갱신이 실패하는 현상을 수정
		TP_INVENTORY_END + SLOT_EXTENDED_INVENTORY1_NUM + SLOT_EXTENDED_INVENTORY2_NUM + SLOT_WEAR_NUM - 1,
		TP_STORAGE_START,
		TP_STORAGE_END - 1,
		TP_GUILDWAREHOUSE_START,
		TP_GUILDWAREHOUSE_END - 1,
		TP_SHOPITEM_START,
		TP_SHOPITEM_END - 1 );

	g_DB.Query(eQueryType_FreeQuery, eItemMoveUpdate, 0, txt, CharacterIDX);
}

void ItemMoveStorageUpdateToDB(DWORD CharacterIDX, DWORD UserIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos)
{
	// 080410 LUJ, 영역 정보가 바뀌어도 관계없도록 인자로 받도록 함
	sprintf(
		txt,
		"EXEC dbo.MP_ITEM_MoveUpdateStorage %d, %d, %d, %d, %d, %d, %d, %d",
		CharacterIDX,
		UserIDX,
		dwfromDBIdx,
		frompos,
		dwtoDBIdx,
		topos,
		TP_INVENTORY_START,
		TP_INVENTORY_END + SLOT_EXTENDED_INVENTORY1_NUM + SLOT_EXTENDED_INVENTORY2_NUM - 1 );
	
	g_DB.Query(eQueryType_FreeQuery, eItemMoveStorageUpdate, 0, txt, CharacterIDX);
}

void ItemMoveGuildUpdateToDB(DWORD CharacterIDX, DWORD MunpaIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos)
{
	// 080410 LUJ, 영역 정보가 바뀌어도 관계없도록  인자로 받도록 함
	sprintf(
		txt,
		"EXEC dbo.MP_ITEM_MoveUpdateMunpa %d, %d, %d, %d, %d, %d, %d, %d",
		CharacterIDX,
		MunpaIDX,
		dwfromDBIdx,
		frompos,
		dwtoDBIdx,
		topos,
		TP_INVENTORY_START,
		TP_INVENTORY_END + SLOT_EXTENDED_INVENTORY1_NUM + SLOT_EXTENDED_INVENTORY2_NUM - 1 );

	g_DB.Query(eQueryType_FreeQuery, eItemMoveGuildUpdate, 0, txt, CharacterIDX);
}

void ItemInsertToDB(DWORD CharacterIdx, DWORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, DWORD dwKey, WORD wSeal)
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d ", STORED_ITEM_INSERT_INVEN, CharacterIdx, wItemIdx, Durability, bPosition, wSeal);
	g_DB.Query(eQueryType_FreeQuery, eItemInsert, dwKey, txt, CharacterIdx);
}


void ItemDeleteToDB(DWORD dwDBIdx )
{
	g_DB.FreeMiddleQuery(
		0,
		0,
		"EXEC DBO.MP_ITEM_DELETE %d",
		dwDBIdx );

	ITEMMGR->RemoveOption( dwDBIdx );
}

// 080402 LUJ, 인자 이름을 아이템 DB인덱스로 변경
void ItemOptionDelete( DWORD dwDBIdx )
{
	g_DB.FreeMiddleQuery(
		0,
		0,
		"EXEC DBO.MP_ITEM_OPTION_Delete %d",
		dwDBIdx );
}


// 071231 KTH -- CharacterIDX add
void SkillUpdateToDB( SKILL_BASE* msg, DWORD CharacterIDX )
{
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_SKILL_UPDATE, msg->dwDBIdx, msg->wSkillIdx, msg->Level);
	g_DB.Query(eQueryType_FreeQuery, eSkillUpdate2, 0, txt, CharacterIDX);
}
// 071231 KTH -- CharacterIDX add , 사용하지 않음.
void SkillMoveUpdateToDB(DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos, char* type, DWORD CharacterIDX)
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d", STORED_SKILL_MOVEUPDATE, dwfromDBIdx, frompos, dwtoDBIdx, topos);
	g_DB.Query(eQueryType_FreeQuery, eSkillMoveUpdate2, 0, txt, CharacterIDX);
	/*
	frompos == 0 || topos == 0 )
	{
	char temp[256];
	FILE* fp = fopen("MGMoveUpdateToDB.txt","a+");
	sprintf(temp, "%s : [Fdbidx]%d, [Fpos]%d, [Tdbidx]%d, [Tpos]%d\n", type, dwfromDBIdx, frompos, dwtoDBIdx, topos);
	fprintf(fp,temp);
	fclose(fp);
	}
	*/
}

void SkillInsertToDB(DWORD CharacterIDX, DWORD SkillIDX, BYTE level/*, POSTYPE SkillPos, BYTE bWeared, BYTE bLevel, WORD Option*/)
{
	//	char txt[128];
	//	ASSERT(SkillPos);
	//sprintf(txt, "EXEC %s %u, %d, %d, %d, %d, %d", STORED_SKILL_INSERT, CharacterIDX, SkillIDX, SkillPos, bWeared, bLevel, Option);
	sprintf(txt, "EXEC %s %u, %u, %d", STORED_SKILL_INSERT, CharacterIDX, SkillIDX, level);
	g_DB.Query(eQueryType_FreeQuery, eSkillInsert, CharacterIDX, txt, CharacterIDX);
}
// 071231 KTH -- 사용하지 않음
void SkillDeleteToDB(DWORD dwDBIdx)
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_SKILL_DELETE, dwDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eSkillDelete, 0, txt);
}

void MapBaseEconomy(BYTE MapNum)
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_CHARACTER_BASEECONOMY, MapNum);
	g_DB.Query(eQueryType_FreeQuery, eBaseEconomy, 0, txt);
}
// 071231 KTH -- 사용하지 않음
void AuctionRegist(BYTE ItemIdx, DWORD SellerID, DWORD EndDate, DWORD EndTime, DWORD StartPrice, DWORD ImmediatePrice, char Memo[256])
{
	//	char txt[384];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %s", STORED_CHARACTER_AUCTIONREGIST, 
		ItemIdx, SellerID, EndDate, EndTime, StartPrice, ImmediatePrice, Memo);
	g_DB.Query(eQueryType_FreeQuery, eAuctionRegist, SellerID, txt);
}

void AuctionPageQuery(DWORD CharacterIDX, BYTE PageNum)
{
}
/*
void CharacterItemGetHuntIDX(DWORD CharacterIDX, DWORD OwnerID, WORD ItemIDX, DURTYPE Durability, POSTYPE bPosition, BYTE TableIDX)
{
	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_CHARACTER_ITEMHUNT, OwnerID, ItemIDX, bPosition, 0,Durability, TableIDX );
	g_DB.Query(eQueryType_FreeQuery, eItemHunt, CharacterIDX, txt);
	{
	}
}
void CharacterDivideNewItemIDX(DWORD CharacterIDX, DWORD OwnerID, WORD ItemIDX, DURTYPE Durability, POSTYPE bPosition, BYTE TableIDX)
{
	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_CHARACTER_ITEMHUNT, OwnerID, ItemIDX, bPosition, 0,Durability, TableIDX );
	g_DB.Query(eQueryType_FreeQuery, eItemDivideNewItem, CharacterIDX, txt);
	{
	}
}
*/


// --------------------------------------------------------------------------------------------------
// taiyo code
// CharacterDupItemInsertAC wKey￠￥A RCharacterDupItemInsert￠?￠®¨u¡ⓒ E¡IAaAU￠￢| ¡¾￠￢¨￢¨￠CI¡¾a 'CN ¡¾￠￢¨￢¨￠AUAI￠￥U
// 0¡Æu 0AI￠?UAC ¡Æ¨￡; ¡ic￠?eCI￠￥A BOOL¡Æ¨￡AC CuAA￠￢| AeCN￠￥U
/*
void CharacterDupItemInsert( DWORD dwKey, DWORD CharacterIDX, WORD ItemIDX, DURTYPE Durability, POSTYPE bPosition, BYTE TableIDX )
{
	char txt[128];
	sprintf( txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_CHARACTER_ITEMHUNT, CharacterIDX, ItemIDX, bPosition, 0, Durability, TableIDX );
	g_DB.Query(eQueryType_FreeQuery, eItemDupInsert, dwKey, txt);
	{
	}
}
*/

void RegistLoginMapInfo(DWORD CharacterIDX, char* CharacterName, BYTE MapNum, WORD MapPort)
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d, \'%s\', %d, %d", STORED_CHARACTER_REGISTLOGINMAPINFO, CharacterIDX, CharacterName, MapNum, MapPort);
	g_DB.Query(eQueryType_FreeQuery, eRegistLoginMapInfo, 0, txt, CharacterIDX);
}

void UnRegistLoginMapInfo(DWORD CharacterIDX)
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_CHARACTER_UNREGISTLOGINMAPINFO, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eUnRegistLoginMapInfo, 0, txt, CharacterIDX);
}

//////////////////////////////////////////////////////////////////////////
// ¨¡A¨¡¨u ¡Æu¡¤A DBAo￠￢￠c
void PartyLoad(DWORD PartyIDX)
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d ", STORED_PARTY_REGIST_MAPSERVER, PartyIDX);
	g_DB.Query(eQueryType_FreeQuery, ePartyRegistMapServer, 0, txt);
}

void PartyInfoByUserLogin(DWORD PartyIDX, DWORD CharacterIDX) 
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d ", STORED_PARTY_INFO_BY_USERLOGIN, PartyIDX);
	g_DB.Query(eQueryType_FreeQuery, ePartyInfoByUserLogin, CharacterIDX, txt, CharacterIDX);
}
void PartyBreakup(DWORD PartyIDX, DWORD CharacterIDX) 
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_PARTY_BREAK, PartyIDX);
	g_DB.Query(eQueryType_FreeQuery, ePartyBreakup, PartyIDX, txt, CharacterIDX);
}
void PartyCreate(DWORD MasterIDX, BYTE Option) 
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d", STORED_PARTY_CREATE, MasterIDX,Option);
	g_DB.Query(eQueryType_FreeQuery, ePartyCreate, MasterIDX, txt, MasterIDX);
}

void GuildLoadGuild(DWORD StartGuildIdx)
{
	sprintf(txt, "EXEC %s %u", STORED_GUILD_LOADGUILD, StartGuildIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildLoadGuild, 0, txt);
}

// 06. 03. 문파공지 - 이영준
//void GuildLoadNotice(DWORD StartDBIdx)
//{
//	//	sprintf(txt, "EXEC %s %u", STORED_GUILD_LOADNOTICE, StartDBIdx);
//	//	g_DB.Query(eQueryType_FreeQuery, , 0, txt);
//
//	g_DB.FreeMiddleQuery( RGuildLoadNotice, eGuildLoadNotice,
//		"EXEC %s %u", STORED_GUILD_LOADNOTICE, StartDBIdx );
//}

void GuildUpdateNotice( DWORD guildIndex, const char* notice, DWORD CharacterIDX )
{
	sprintf( txt, "EXEC DBO.MP_GUILD_NOTICE_UPDATE %d, \'%s\'", guildIndex, notice, CharacterIDX );

	g_DB.FreeQuery( eGuildUpdateNotice, 0, txt);
}

void GuildLoadMember( DWORD StartCharacterIdx )
{
	g_DB.FreeQuery(
		eGuildLoadMember,
		0,
		"EXEC MP_GUILD_LoadMember %u",
		StartCharacterIdx );
}

void GuildLoadMark(DWORD StartMarkName)
{
	char txt[128];
	sprintf(txt, "EXEC %s %u", STORED_GUILD_LOADMARK, StartMarkName);
	g_DB.FreeLargeQuery(RGuildLoadMark, 0, txt);
}

// 길드 생성
void GuildCreate(DWORD CharacterIdx, const char* CharacterName, LEVELTYPE CharacterLvl, const char* GuildName, const char* Intro, int masterLevel )
{
	g_DB.FreeMiddleQuery(
		RGuildCreate,
		CharacterIdx,
		"EXEC DBO.MP_GUILD_CREATE2 %d, \'%s\', %d, \'%s\', \'%s\', %d, %d",
		CharacterIdx,
		CharacterName,
		CharacterLvl,
		GuildName,
		Intro,
		GAMERESRCMNGR->GetLoadMapNum(),
		masterLevel,
		CharacterIdx );
}


void RGuildCreate( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD				playerIndex = dbMessage->dwID;
	CPlayer*				player		= ( CPlayer* )g_pUserTable->FindUser( playerIndex );

	if( ! player			||
		! dbMessage->dwResult )
	{
		ASSERT( 0 );
		return;
	}	

	const MIDDLEQUERYST&	record		= query[ 0 ];

	if( ! atoi( ( char* )record.Data[ 0 ] ) )
	{
		if( player )
		{
			GUILDMGR->SendNackMsg( player, MP_GUILD_CREATE_NACK, eGuildErr_Create_Invalid_Name );
		}

		return;
	}

	// 창립 자금을 빼낸다
	{
		const CGuildManager::LevelSetting* setting = GUILDMGR->GetLevelSetting( 1 );

		if( ! setting )
		{
			ASSERT( 0 );
			return;
		}

		player->SetMoney( setting->mRequiredMoney, MONEY_SUBTRACTION, MF_LOST, eItemTable_Inventory, 0, 0  );
	}

	{
		GUILDINFO data;

		ZeroMemory( &data, sizeof( data ) );
		data.GuildIdx	= atoi( ( char* )record.Data[ 0 ] );
		data.MapNum		= atoi( ( char* )record.Data[ 1 ] );
		data.MasterIdx	= playerIndex;
		data.GuildLevel = 1;
		data.UnionIdx	= 0;
		data.MarkName	= 0;
		SafeStrCpy(data.GuildName, ( char* )record.Data[ 2 ], sizeof( data.GuildName ) );
		SafeStrCpy(data.MasterName, ( char* )record.Data[ 3 ], sizeof( data.MasterName ) );

		GUILDMGR->CreateGuildResult( atoi( ( char* )record.Data[ 4 ] ), data );

		GuildReadData( playerIndex, data.GuildIdx );
	}
}


void GuildBreakUp(DWORD GuildIdx, DWORD MasterIdx)
{
	sprintf(txt, "EXEC %s %d", STORED_GUILD_BREAKUP, GuildIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildBreakUp, MasterIdx, txt, MasterIdx);
}


void GuildDeleteMember(DWORD GuildIdx, DWORD MemberIDX)
{
	sprintf(txt, "EXEC %s %d, %d", STORED_GUILD_DELETEMEMBER, GuildIdx, MemberIDX);
	g_DB.Query(eQueryType_FreeQuery, eGuildDeleteMember, GuildIdx, txt, MemberIDX);
}


void GuildSecedeMember(DWORD GuildIdx, DWORD PlayerIdx)
{
	sprintf(txt, "EXEC %s %d, %d", STORED_GUILD_DELETEMEMBER, GuildIdx, PlayerIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildSecedeMember, GuildIdx, txt, PlayerIdx);
}

void GuildAddMember(DWORD guildIndex, DWORD playerIndex, const char* guildName, BYTE rank )
{
	g_DB.FreeMiddleQuery(
		RGuildAddMember,
		playerIndex,
		"EXEC MP_GUILD_AddMember %u, %u, \'%s\', %d",
		guildIndex,
		playerIndex,
		guildName,
		rank,
		playerIndex);
}


void RGuildAddMember( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	if( 1 == dbMessage->dwResult )
	{
		const MIDDLEQUERYST&	record		= query[ 0 ];
		const DWORD				guildIndex	= atoi( ( char* )record.Data[ 0 ] );

		//GuildReadData( dbMessage->dwID, guildIndex );

		CPlayer* player = ( CPlayer* )g_pUserTable->FindUser( dbMessage->dwID );

		if( player )
		{
			GUILDMGR->AddPlayer( player );
		}
	}	
}


void GuildMarkRegist(DWORD GuildIdx, char* ImgData, DWORD CharacterIdx)
{
	char buf[ 2048 ];
	sprintf( buf, "EXEC %s %d, 0x", STORED_GUILD_MARKREGIST,GuildIdx);

	int curpos = strlen(buf);
	for(int n=0;n<GUILDMARK_BUFSIZE;++n)
	{
		sprintf(&buf[curpos],"%02x",(BYTE)ImgData[n]);
		curpos += 2;
	}

	g_DB.FreeLargeQuery(RGuildMarkRegist, CharacterIdx,	
		buf, CharacterIdx);
}


void GuildChangeMemberRank(DWORD GuildIdx, DWORD MemberIdx, BYTE Rank)
{
	g_DB.FreeMiddleQuery(
		0,
		0,
		"EXEC MP_GUILD_ChangeRank %u, %u, %d",
		GuildIdx,
		MemberIdx,
		Rank,
		MemberIdx);
}


//void GuildMoneyUpdate( DWORD playerIndex, MONEYTYPE Money, BOOL isDeposit, MONEYTYPE maxMoney )
//{
//	sprintf( txt, "EXEC %s %u, %u, %u, %d", STORED_GUILD_MONEYUPDATE, playerIndex, Money, maxMoney, isDeposit );
//	g_DB.FreeQuery( eGuildMoneyUpdate, playerIndex, txt );
//}
//
//
//void RGuildMoneyUpdate( LPQUERY query, LPDBMESSAGE DBmessaage )
//{
//	if( 1 != DBmessaage->dwResult )
//	{
//		return;
//	}
//
//	CPlayer* player = ( CPlayer* )g_pUserTable->FindUser( DBmessaage->dwID );
//	
//	if( player )
//	{
//		const QUERYST&	record = query[ 0 ];
//		const MONEYTYPE guildMoney	= atoi( ( char* )record.Data[ 0 ] );
//		const MONEYTYPE playerMoney	= atoi( ( char* )record.Data[ 1 ] );		
//		const BOOL		isDeposit	= atoi( ( char* )record.Data[ 2 ] );
//		MSG_DWORD2		message;
//        
//		if( isDeposit )
//		{
//			message.Protocol	= MP_GUILD_PUTIN_MONEY_ACK;
//
//			player->SetMoney( playerMoney, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_GetGuild, player->GetGuildIdx() );
//
//			InsertLogMoney( eMoneyLog_GetGuild, player->GetID(), player->GetMoney(), playerMoney, player->GetGuildIdx(), guildMoney );
//
//			InsertLogGuildWarehouse( player, 0, 0, guildMoney, eLog_MoneyDepositToGuild );
//		}
//		else
//		{
//			message.Protocol	= MP_GUILD_PUTOUT_MONEY_ACK;
//
//			player->SetMoney( playerMoney, MONEY_ADDITION, 0, eItemTable_Inventory, eMoneyLog_GetGuild, player->GetGuildIdx() );
//
//			InsertLogMoney( eMoneyLog_LoseGuild, player->GetID(), player->GetMoney(), playerMoney, player->GetGuildIdx(), guildMoney );
//
//			InsertLogGuildWarehouse( player, 0, 0, guildMoney, eLog_MoneyWithdrwaGuild );
//		}
//
//		message.Category	= MP_GUILD;
//		message.dwData1		= player->GetMoney();
//		message.dwData2		= guildMoney;
//		
//		player->SendMsg( &message, sizeof( message ) );
//	}
//}


void GuildGiveMemberNickName(DWORD TargetId, char * NickName)
{
	sprintf(txt, "EXEC %s %u, \'%s\'", STORED_GUILD_GIVENICKNAME, TargetId, NickName);
	g_DB.Query(eQueryType_FreeQuery, eGuildGiveNickName, 0, txt, TargetId);
}

void StorageBuyStorage(DWORD PlayerIDX)
{
	sprintf(txt, "EXEC %s %d", STORED_STORAGE_BUYSTORAGE, PlayerIDX);
	g_DB.Query(eQueryType_FreeQuery, eStorageBuyStorage, PlayerIDX, txt, PlayerIDX);
}
// 071231 KTH -- CharacterIDX Add
void StorageMoneyUpdateToDB(DWORD UserIdx, MONEYTYPE StorageMoney, DWORD CharacterIDX)
{
	sprintf(txt, "EXEC %s %u, %u", STORED_STORAGE_MONEYUPDATE, UserIdx, StorageMoney);
	g_DB.Query(eQueryType_FreeQuery, eStorageMoneyUpdate, 0, txt, CharacterIDX);
}

void SaveCharInfoBeforeLogout(DWORD PlayerIdx, DWORD dwConnectionIndex, DWORD UserIdx, MONEYTYPE StorageMoney)
{
	sprintf(txt, "EXEC %s %d, %u, %u", STORED_CHARACTER_SAVEINFOBEFORELOGOUT, dwConnectionIndex, UserIdx, StorageMoney);
	g_DB.Query(eQueryType_FreeQuery, eSaveChrInfoBeforeOut, PlayerIdx, txt, PlayerIdx);
}


void CharacterStorageInfo(DWORD UserIdx, DWORD CharacterIdx)
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_STORAGE_INFO, UserIdx);
	g_DB.Query(eQueryType_FreeQuery, eCharacterStorageInfo, CharacterIdx, txt, CharacterIdx);
}

//void CharacterStorageItemInfo(DWORD CharacterIdx, DWORD StartDBIdx)
void CharacterStorageItemInfo(DWORD CharacterIdx,DWORD UserIdx, DWORD StartDBIdx)
{
	//	char txt[128];
	//sprintf(txt, "EXEC %s %d, %d", STORED_STORAGE_ITEMINFO, CharacterIdx, StartDBIdx);
	//g_DB.Query(eQueryType_FreeQuery, eCharacterStorageItemInfo, CharacterIdx, txt);
	sprintf(txt, "EXEC %s %d, %d", STORED_STORAGE_ITEMINFO, UserIdx, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eCharacterStorageItemInfo, CharacterIdx, txt, CharacterIdx);
}

void FriendNotifyLogouttoClient(DWORD PlayerID)
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_FRIEND_NOTIFYLOGOUT, PlayerID);
	g_DB.Query(eQueryType_FreeQuery, eFriendNotifyLogout, PlayerID, txt, PlayerID);
}

void WantedLoad(WANTEDTYPE StartWantedIDX)
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_WANTED_LOAD, StartWantedIDX);
	g_DB.Query(eQueryType_FreeQuery, eWantedLoad, 0, txt);	
}

void WantedInfoByUserLogIn(DWORD CharacterIDX)
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_WANTED_INFO, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eWantedGetList, CharacterIDX, txt, CharacterIDX);
}

void WantedBuyRight(DWORD CharacterIDX, WANTEDTYPE WantedIDX, MONEYTYPE AddPrize)
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_WANTED_BUYRIGHT, CharacterIDX, WantedIDX, AddPrize);
	g_DB.Query(eQueryType_FreeQuery, eWantedBuyRight, 0, txt, CharacterIDX);
}

void WantedRegist(DWORD CharacterIDX, DWORD TargetIDX, MONEYTYPE Prize)
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_WANTED_REGIST, CharacterIDX, TargetIDX, Prize);
	g_DB.Query(eQueryType_FreeQuery, eWantedRegist, CharacterIDX, txt, CharacterIDX);
}

void WantedGiveUpRight(DWORD CharacterIDX, DWORD WantedIDX)
{
	sprintf(txt, "EXEC %s %d, %d", STORED_WANTED_GIVEUP, CharacterIDX, WantedIDX);
	g_DB.Query(eQueryType_FreeQuery, eWantedGiveupRight, 0, txt, CharacterIDX);
}

void WantedComplete(DWORD CharacterIDX, char* TargetName, WANTEDTYPE WantedIDX)
{
	/*A￠￥¡ii¡¾C 100¡Æⓒø¡Æ￠® ￠￢¨¡¨o¨￢*/
	sprintf(txt, "EXEC %s %d, \'%s\', %d", STORED_WANTED_COMPLETE, CharacterIDX, TargetName, WantedIDX);
	g_DB.Query(eQueryType_FreeQuery, eWantedComplete, WantedIDX, txt, CharacterIDX);
}

void WantedDestroyed(DWORD WantedIDX)
{
	sprintf(txt, "EXEC %s %d", STORED_WANTED_DESTROYED, WantedIDX);
	g_DB.Query(eQueryType_FreeQuery, eWantedDestroyed, WantedIDX, txt, WantedIDX);
}

void WantedOrderedList(DWORD CharacterIDX, DWORD Page, char* OrderType, BYTE PageUnit)
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d, \'%s\', %d", STORED_WANTED_ORDERLIST, Page, OrderType, PageUnit);
	g_DB.Query(eQueryType_FreeQuery, eWantedOrderList, CharacterIDX, txt, CharacterIDX);
}

void WantedMyList(DWORD CharacterIDX, DWORD Page, BYTE PageUnit)
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_WANTED_MYLIST, CharacterIDX, Page, PageUnit);
	g_DB.Query(eQueryType_FreeQuery, eWantedMyList, CharacterIDX, txt, CharacterIDX);
}

void WantedSearch(DWORD CharacterIDX, char* WantedName, DWORD Page, BYTE PageUnit)
{
	sprintf(txt, "EXEC %s \'%s\', %d, %d", STORED_WANTED_SEARCH, WantedName, Page, PageUnit);
	g_DB.Query(eQueryType_FreeQuery, eWantedSearch, CharacterIDX, txt, CharacterIDX);
}

void JournalGetList(DWORD CharacterIDX)
{
	//sprintf(txt, "EXEC %s %d", STORED_JOURNAL_GETLIST, CharacterIDX);
	//g_DB.Query(eQueryType_FreeQuery, eJournalGetList, CharacterIDX, txt);
}

void JouranlInsertQuest(DWORD CharacterIDX, BYTE Kind, DWORD QuestIDX, DWORD SubQuestIDX, int bCompleted)
{
	//sprintf(txt, "EXEC %s %d, %d, %d, %d, %d", STORED_JOURNAL_INSERTQUEST, CharacterIDX, Kind, QuestIDX, SubQuestIDX, bCompleted);
	//g_DB.Query(eQueryType_FreeQuery, eJournalInsertQuest, 0, txt);
}

void JouranlInsertWanted(DWORD CharacterIDX, BYTE Kind, DWORD Param, char* ParamName)
{
	//sprintf(txt, "EXEC %s %d, %d, %d, \'%s\'", STORED_JOURNAL_INSERTWANTED, CharacterIDX, Kind, Param, ParamName);
	//g_DB.Query(eQueryType_FreeQuery, eJournalInsertWanted, 0, txt);
}

void JournalInsertLevel(DWORD CharacterIDX, BYTE Kind, DWORD Level)
{
	//sprintf(txt, "EXEC %s %d, %d, %d", STROED_JOURNAL_INSERTLEVEL, CharacterIDX, Kind, Level);
	//g_DB.Query(eQueryType_FreeQuery, eJournalInsertLevel, 0, txt);
}
void JournalSaveUpdate(DWORD CharacterIDX, DWORD JournalIndex)
{
	//sprintf(txt, "EXEC %s %d, %d", STROED_JOURNAL_SAVEUPDATE, CharacterIDX, JournalIndex);
	//g_DB.Query(eQueryType_FreeQuery, eJournalSaveUpdate, 0, txt);
}

void JournalDelete(DWORD CharacterIDX, DWORD JournalIndex)
{
	//sprintf(txt, "EXEC %s %d, %d", STROED_JOURNAL_DELETE, CharacterIDX, JournalIndex);
	//g_DB.Query(eQueryType_FreeQuery, eJournalDelete, 0, txt);
}

//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// 
// Updateⓒo￠c
//void CharacterItemInsert(DWORD CharacterIDX, WORD ItemIDX, DURTYPE Durability, POSTYPE bPosition, WORD qPosition)
/*
void CharacterItemUpdate(DWORD CharacterIDX, DWORD dwDBIdx, WORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, WORD qPosition)
{
	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_CHARACTER_ITEMUPDATE, 
		CharacterIDX, dwDBIdx, wItemIdx, bPosition, qPosition, Durability);
	g_DB.Query(eQueryType_FreeQuery, eItemUpdate, CharacterIDX, txt);
	{
	}
}


void CharacterItemInsert(DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos)
{
	//	char txt[128];
	//	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_CHARACTER_ITEMUPDATE, 
	//		CharacterIDX, dwDBIdx, wItemIdx, bPosition, qPosition, Durability);
	//	g_DB.Query(eQueryType_FreeQuery, eItemUpdate, CharacterIDX, txt);
	//	{
	//	}
}
*/

/*
void CharacterSkillUpdate(SKILLBASE* msg)
{
	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %d", STORED_CHARACTER_SKILLUPDATE, 
		msg->dwDBIdx, msg->wIconIdx, msg->ExpPoint, msg->Level, msg->Position, msg->QuickPosition, msg->bWear);
	g_DB.Query(eQueryType_FreeQuery, eSkillUpdate, 0, txt);
	{
	}
	FILE* fp = fopen("MGTEST.txt","a+");
	fprintf(fp,txt);
	fclose(fp);
}

void CharacterSkillMoveUpdate(DWORD toDBIdx, POSTYPE toPos, DWORD fromDBIdx, POSTYPE fromPos)
{
	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d", STORED_CHARACTER_SKILLMOVEUPDATE, 
		toDBIdx, toPos, fromDBIdx, fromPos);
	g_DB.Query(eQueryType_FreeQuery, eSkillMoveUpdate, 0, txt);
	{
	}
}
*/

void CharacterHeroInfoUpdate(CPlayer* pPlayer)
{
	if( !pPlayer->GetInited() )	return;

	BASEOBJECT_INFO baseinfo;
	HERO_TOTALINFO	heroinfo;

	pPlayer->GetBaseObjectInfo(&baseinfo);
	pPlayer->GetHeroTotalInfo(&heroinfo);

	// 080602 LYW --- Player : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
	char txt[512] = {0,} ;
	//sprintf(txt, "EXEC  %s %d, %d, %d, %d, %d, %d, %d, %d, %d, %u, %d, %u, %d, %d, %d, %d, %d",
	sprintf(txt, "EXEC  %s %d, %d, %d, %d, %d, %d, %d, %d, %d, %0.f, %d, %u, %d, %d, %d, %d, %d",
		STORED_CHARACTER_HEROINFOUPDATE,
		baseinfo.dwObjectID, 
		heroinfo.Str, 
		heroinfo.Mana, 
		heroinfo.Dex, 
		heroinfo.Vit, 
		heroinfo.Int, 
		heroinfo.Wis, 
		0,//heroinfo.Fame,	//now, fame not used... but, it's occur some bugs.
		heroinfo.wKarma, 
		//heroinfo.ExpPoint, 
		(float)heroinfo.ExpPoint, 
		heroinfo.LevelUpPoint, 
		heroinfo.Money, 
		0,
		0,/*어빌리티 삭제 - 이영준 pPlayer->GetPlayerAbilityExpPoint(), */
		heroinfo.Playtime, 
		heroinfo.LastPKModeEndTime,
		heroinfo.MaxLevel);
	g_DB.Query(eQueryType_FreeQuery, eHeroInfoUpdate, 0, txt, pPlayer->GetID());
}

void CharacterTotalInfoUpdate(CPlayer* pPlayer)
{
	if( !pPlayer->GetInited() )	return;

	CHARACTER_TOTALINFO totinfo;
	pPlayer->GetCharacterTotalInfo(&totinfo);

	// 06. 06 - 이영준 중요!!!
	//MP_CHARACTER_TotalInfoUpdate 수정 필요함
		char txt[512];
	sprintf(txt, "EXEC  %s %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u",
		STORED_CHARACTER_TOTALINFOUPDATE,
		pPlayer->GetID(), totinfo.Gender,
		totinfo.Life,
		totinfo.Level,
		totinfo.LoginMapNum, totinfo.FaceType, totinfo.HairType, totinfo.WearedItemIdx[eWearedItem_Hat],
		totinfo.WearedItemIdx[eWearedItem_Weapon], totinfo.WearedItemIdx[eWearedItem_Dress],
		totinfo.WearedItemIdx[eWearedItem_Shoes], 0/*모자*/,
		0/*옷*/, totinfo.WearedItemIdx[eWearedItem_Glove],
		0/*신발*/, 0/*장갑*/,
		0/*마스크*/, totinfo.WearedItemIdx[eWearedItem_Shield],
		totinfo.BadFame, 0/*안경*/, totinfo.WearedItemIdx[eWearedItem_Band] );
	g_DB.Query(eQueryType_FreeQuery, eTotalInfoUpdate, 0, txt, pPlayer->GetID());
}

void CharacterBadFameUpdate(DWORD PlayerID, int BadFame, DWORD Money)
{	
	sprintf(txt, "EXEC  %s %d, %d, %u", STORED_CHARACTER_BADFAMEUPDATE, PlayerID, BadFame, Money);
	g_DB.Query(eQueryType_FreeQuery, eTotalInfoUpdate, 0, txt, PlayerID);
}

void CharacterLvlExpUpdate(DWORD PlayerID, LEVELTYPE level, EXPTYPE exppoint)
{
	// 080605 LYW --- MapDBMsgParser : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
	//sprintf(txt, "update TB_Character \
	//	set character_grade = %d, character_expoint = %u \
	//	where character_idx = %d", level, exppoint, PlayerID);
	//	g_DB.Query(eQueryType_FreeQuery, eTotalInfoUpdate, 0, txt, PlayerID);

	sprintf(txt, "update TB_Character \
		set character_grade = %d, character_expoint = %0.f \
		where character_idx = %d", level, (float)exppoint, PlayerID);
		g_DB.Query(eQueryType_FreeQuery, eTotalInfoUpdate, 0, txt, PlayerID);
}
/*
void CharacterQuickInfoUpdate(CPlayer * pPlayer)
{
	QUICKITEM quickItem;
	for(int i = QUICK_STARTPOSITION ; i < QUICK_ENDPOSITION ; i++)
	{
		if(pPlayer->GetQuickPostion(i, &quickItem))
		{

			switch(quickItem.eIGKinds)
			{
			case eSKILL_ICON:
				{
					SKILLBASE * updSkill = pPlayer->GetSkillBase(quickItem.wAbsPostion);
					CharacterSkillUpdate(updSkill);
				}
				break;
			case eITEM_ICON:
				{
					CIconTable * pTable = pPlayer->GetIconTable(GetTableIdxPosition(quickItem.wAbsPostion));

					ITEMBASE * updItem = pTable->GetItemInfoAbs(quickItem.wAbsPostion);
					ASSERT(updItem->dwDBIdx > 0);
					if(updItem->dwDBIdx > 0)
						CharacterItemUpdate(pPlayer->GetID(), updItem->dwDBIdx, updItem->wIconIdx, updItem->Durability, updItem->Position, i);

				}
				break;
			default:
				{
					//ASSERT(0);
				}
			}
		}
	}
}
*/
void CharacterSkillTreeInfoUpdate(CPlayer* pPlayer)
{
	/*	SKILLBASE* pSkill;

	for(int n=TP_SKILL1_START; n<TP_SKILL2_END; ++n)
	{
	pSkill = pPlayer->GetSkillBase(n);
	if(pSkill == NULL || pSkill->dwDBIdx==0 || pSkill->bWear==0)
	continue;
	SkillUpdateToDB(pSkill, "TOTALUPDATE");
	}

	for(n=TP_JINBUB_START; n<TP_JINBUB_END; ++n)
	{
	pSkill = pPlayer->GetSkillBase(n);
	if(pSkill == NULL || pSkill->dwDBIdx==0 || pSkill->bWear==0)
	continue;
	SkillUpdateToDB(pSkill, "TOTALUPDATE");
	}*/
}

void CharacterTablePartyIdxUpdate(DWORD PlayerID) 
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_PARTY_CHARACTERUPDATE, PlayerID);

	g_DB.Query(eQueryType_FreeQuery, ePartyCharacterTbUpdate, 0, txt, PlayerID);
}

void PartyAddMember(DWORD PartyIDX, DWORD TargetID)
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d ", STORED_PARTY_ADDMEMBER, PartyIDX, TargetID);
	g_DB.Query(eQueryType_FreeQuery, ePartyAddMember, TargetID, txt, TargetID);
}

void PartyDelMember(DWORD PartyIDX, DWORD TargetID)
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d ", STORED_PARTY_DELMEMBER, PartyIDX, TargetID);
	g_DB.Query(eQueryType_FreeQuery, ePartyDelMember, PartyIDX, txt, TargetID);
}

void PartyChangeMaster(DWORD PartyID, DWORD PlayerID, DWORD TargetPlayerID)
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_PARTY_CHANGEMASTER, PartyID, PlayerID, TargetPlayerID);
	g_DB.Query(eQueryType_FreeQuery, ePartyChangeMaster, PartyID, txt, PlayerID);
}

void SavePointUpdate(DWORD CharacterIDX, WORD LoginPoint_Idx, WORD MapNum)
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d",	STORED_SAVE_LOGINPOINT, CharacterIDX, LoginPoint_Idx, MapNum);
	g_DB.Query(eQueryType_FreeQuery, eSavePoint, 0, txt, CharacterIDX);
}

void SaveMapChangePointUpdate(DWORD CharacterIDX, WORD MapChangePoint_Idx)
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d",	STORED_SAVE_MAPCHAGEPOINT, CharacterIDX, MapChangePoint_Idx);
	g_DB.Query(eQueryType_FreeQuery, eSavePoint, 0, txt, CharacterIDX);
}

// 081218 LUJ, 플레이어 소지금 추가
void SaveMapChangePointReturn(DWORD CharacterIDX, WORD MapChangePoint_Idx, DWORD dwConnectionIdx, LEVELTYPE lvl, EXPTYPE ExpPoint, MONEYTYPE inventoryMoney, MONEYTYPE StorageMoney)
{
	sprintf(txt, "EXEC  %s %u, %d, %u, %d, %0.f, %u, %u", STORED_SAVE_MAPCHAGEPOINTRET, CharacterIDX, MapChangePoint_Idx, dwConnectionIdx, lvl, (float)ExpPoint, inventoryMoney, StorageMoney);
	g_DB.Query(eQueryType_FreeQuery, eSaveMapChangePoint, CharacterIDX, txt, CharacterIDX);
}

// 070917 LYW --- MapDBMsgParser : Add function to save map change point to return.
// 081218 LUJ, 플레이어 소지금 추가
void SaveNpcMapChangePointReturn(DWORD CharacterIDX, WORD MapChangePoint_Idx, DWORD dwConnectionIdx, LEVELTYPE lvl, EXPTYPE ExpPoint, MONEYTYPE inventoryMoney, MONEYTYPE StorageMoney)
{
	sprintf(txt, "EXEC  %s %u, %d, %u, %d, %0.f, %u",	STORED_SAVE_MAPCHAGEPOINTRET, CharacterIDX, MapChangePoint_Idx, dwConnectionIdx, lvl, (float)ExpPoint, inventoryMoney, StorageMoney);
	g_DB.Query(eQueryType_FreeQuery, eSaveNpcMapChangePoint, CharacterIDX, txt, CharacterIDX);
}

void FameCharacterUpdate(DWORD PlayerIDX, FAMETYPE val)
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d",	STORED_FAME_CHARACTERUPDATE, PlayerIDX, val);
	g_DB.Query(eQueryType_FreeQuery, eFameCharacterUpdate, 0, txt, PlayerIDX);
}

void BadFameCharacterUpdate(DWORD PlayerIDX, FAMETYPE val)
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d",	STORED_BADFAME_CHARACTERUPDATE, PlayerIDX, val);
	g_DB.Query(eQueryType_FreeQuery, eBadFameCharacterUpdate, 0, txt, PlayerIDX);
}

void PKCharacterUpdate(DWORD PlayerIDX, DWORD val)
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d",	STORED_PK_CHARACTERUPDATE, PlayerIDX, val);
	g_DB.Query(eQueryType_FreeQuery, ePKCharacterUpdate, 0, txt, PlayerIDX);
}

void QuestTotalInfo(DWORD PlayerID)
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d",	"MP_QUEST_TotalInfo", PlayerID);
	g_DB.Query(eQueryType_FreeQuery, eQuestTotalInfo, PlayerID, txt, PlayerID);
}

void QuestUpdateToDB(DWORD PlayerID, DWORD QuestIdx, QSTATETYPE State, BYTE bEnd)
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d",	"MP_QUEST_Update", PlayerID, QuestIdx, State, bEnd);
	g_DB.Query(eQueryType_FreeQuery, eQuestUpdate, 0, txt, PlayerID);
}

// RaMa
void QuestMainQuestLoad(DWORD PlayerID, int QuestIdx )
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d", STORED_QUEST_MAINQUEST_LOAD, PlayerID, QuestIdx );
	g_DB.Query(eQueryType_FreeQuery, eQuestMainQuestLoad, PlayerID, txt, PlayerID);
}

void QuestMainQuestInsert(DWORD PlayerID, DWORD mQuestIdx, QSTATETYPE State, DWORD Time, BOOL repeat)
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d, %d", STORED_QUEST_MAINQUEST_INSERT, PlayerID, mQuestIdx, State, Time, repeat);
	g_DB.Query(eQueryType_FreeQuery, eQuestMainQuestInsert, PlayerID, txt, PlayerID);
}

void QuestMainQuestDelete(DWORD PlayerID, DWORD mQuestIdx)
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d", STORED_QUEST_MAINQUEST_DELETE, PlayerID, mQuestIdx);
	g_DB.Query(eQueryType_FreeQuery, eQuestMainQuestDelete, PlayerID, txt, PlayerID);
}

void QuestMainQuestUpdateToDB(DWORD PlayerID, DWORD mQuestIdx, QSTATETYPE State, DWORD EndParam, DWORD Time)
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d, %d", STORED_QUEST_MAINQUEST_UPDATE, PlayerID, mQuestIdx, State, EndParam, Time);
	g_DB.Query(eQueryType_FreeQuery, eQuestMainQuestUpdate, PlayerID, txt, PlayerID);
}

void QuestSubQuestLoad(DWORD PlayerID, int QuestIdx)
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d", STORED_QUEST_SUBQUEST_LOAD, PlayerID, QuestIdx);
	g_DB.Query(eQueryType_FreeQuery, eQuestSubQuestLoad, PlayerID, txt, PlayerID);
}

// 071117 LYW --- MapDBMsgParser : 퀘스트 관련 추가 함수.
void EndQuest_New(DWORD dwCharacter_Idx, DWORD dwQuest_Idx, DWORD dwSubQuest_Do, DWORD dwEnd_Param, DWORD dwRegist_Time)
{
	sprintf
	(
		txt
	,	"EXEC  %s %d, %d, %d, %d, %d"
	,	STORED_QUEST_ENDQUEST_NEW
	,	dwCharacter_Idx
	,	dwQuest_Idx
	,	dwSubQuest_Do
	,	dwEnd_Param
	,	dwRegist_Time
	) ;

	g_DB.Query(eQueryType_FreeQuery, eEndQuest_New, dwCharacter_Idx, txt, dwCharacter_Idx) ;
}

void EndSubQuest_New(DWORD dwCharacter_Idx, DWORD dwQuest_Idx, DWORD dwSubQuestIdx, DWORD dwSubQuest_Do, DWORD dwRegist_Time)
{
	sprintf
	(
		txt
	,	"EXEC  %s %d, %d, %d, %d, %d"
	,	STORED_QUEST_ENDSUBQUEST_NEW
	,	dwCharacter_Idx
	,	dwQuest_Idx
	,	dwSubQuestIdx
	,	dwSubQuest_Do
	,	dwRegist_Time
	) ;

	g_DB.Query(eQueryType_FreeQuery, eEndSubQuest_New, dwCharacter_Idx, txt, dwCharacter_Idx) ;
}

void DeleteQuest_New(DWORD dwCharacter_Idx, DWORD dwQuest_Idx, DWORD param)
{
	sprintf(txt, "EXEC  %s %d, %d, %d", STORED_QUEST_DELETEQUEST_NEW, dwCharacter_Idx, dwQuest_Idx, param) ;
	g_DB.Query(eQueryType_FreeQuery, eDeleteQuest_New, dwQuest_Idx, txt, dwCharacter_Idx) ;
}

void QuestItemload(DWORD PlayerID, int QuestIdx )
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d", STORED_QUEST_ITEM_LOAD, PlayerID, QuestIdx );
	g_DB.Query(eQueryType_FreeQuery, eQuestItemLoad, PlayerID, txt, PlayerID);
}

void QuestItemInsert(DWORD PlayerID, DWORD dwQuestIdx, DWORD ItemIdx, DWORD dwItemNum )
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d", STORED_QUEST_ITEM_INSERT, PlayerID, ItemIdx, dwItemNum, dwQuestIdx );
	g_DB.Query(eQueryType_FreeQuery, eQuestItemInsert, PlayerID, txt, PlayerID);
}

void QuestItemDelete(DWORD PlayerID, DWORD ItemIdx)
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d", STORED_QUEST_ITEM_DELETE, PlayerID, ItemIdx);
	g_DB.Query(eQueryType_FreeQuery, eQuestItemDelete, PlayerID, txt, PlayerID);
}

void QuestItemDeleteAll(DWORD PlayerID, DWORD dwQuestIdx)
{
	sprintf(txt, "EXEC  %s %d, %d", STORED_QUEST_ITEM_DELETE_ALL, PlayerID, dwQuestIdx);
	g_DB.Query(eQueryType_FreeQuery, eQuestItemDeleteAll, PlayerID, txt, PlayerID);
}

void QuestItemUpdate(DWORD PlayerID, DWORD dwQuestIdx, DWORD ItemIdx, DWORD ItemNum)
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d", STORED_QUEST_ITEM_UPDATE, PlayerID, ItemIdx, ItemNum, dwQuestIdx );
	g_DB.Query(eQueryType_FreeQuery, eQuestItemUpdate, PlayerID, txt, PlayerID);
}

void QuestSubQuestDelete(DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx)
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d", STORED_QUEST_SUBQUEST_DELETE, PlayerID, mQuestIdx, sQuestIdx);
	g_DB.Query(eQueryType_FreeQuery, eQuestSubQuestDelete, PlayerID, txt, PlayerID);
}

void QuestSubQuestDeleteAll( DWORD PlayerID, DWORD mQuestIdx )
{
	sprintf(txt, "EXEC  %s %d, %d", STORED_QUEST_SUBQUEST_DELETE_ALL, PlayerID, mQuestIdx);
	g_DB.Query(eQueryType_FreeQuery, eQuestSubQuestDeleteAll, PlayerID, txt, PlayerID);
}

void QuestSubQuestInsert(DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx, QSTATETYPE Data, QSTATETYPE Time)
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d, %d", STORED_QUEST_SUBQUEST_INSERT, PlayerID, mQuestIdx, sQuestIdx, Data, Time);
	g_DB.Query(eQueryType_FreeQuery, eQuestSubQuestInsert, PlayerID, txt, PlayerID);
}

void QuestSubQuestUpdateToDB(DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx, QSTATETYPE Data, QSTATETYPE Time)
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d, %d", STORED_QUEST_SUBQUEST_UPDATE, PlayerID, mQuestIdx, sQuestIdx, Data, Time);
	g_DB.Query(eQueryType_FreeQuery, eQuestSubQuestUpdate, PlayerID, txt, PlayerID);
}

void QuestMainQuestUpdateCheckTime( DWORD dwCharIdx, DWORD dwQuestIdx, DWORD dwCheckType, DWORD dwCheckTime )
{
	sprintf(txt, "dbo.MP_MAINQUEST_UpdateCheckTime %d, %d, %d, %d", dwCharIdx, dwQuestIdx, dwCheckType, dwCheckTime);
	g_DB.Query(eQueryType_FreeQuery, eQuestMainQuestUpdateCheckTime, dwCharIdx, txt, dwCharIdx);
}

// 071023 LYW --- MapDBMsgParser : Add Load query function.
void TutorialLoad(DWORD dwPlayerID, int tutorialIdx, BOOL isComplete)
{
	sprintf(txt, "EXEC	%s %d, %d, %d", STORED_TUTORIAL_LOAD, dwPlayerID, tutorialIdx, isComplete ) ;
	g_DB.Query(eQueryType_FreeQuery, eTutorialLoad, dwPlayerID, txt, dwPlayerID ) ;
}

// 071023 LYW --- MapDBMsgParser : Add Update query function.
void TutorialUpdateToDB(DWORD dwPlayerID, int tutorialIdx, BOOL isComplete)
{
	sprintf(txt, "EXEC	%s %d, %d, %d", STORED_TUTORIAL_UPDATE, dwPlayerID, tutorialIdx, isComplete) ;
	g_DB.Query(eQueryType_FreeQuery, eTutorialUpdate, dwPlayerID, txt, dwPlayerID) ;
}

// 071106 LYW --- MapDBMsgParser : 기본 스탯으로 업데이트 하는 함수.
void DB_UpdateToDefaultStats(DWORD dwPlayerID, int nStr, int nDex, int nVit, int nInt, int nWis, int nRemainPoint)
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %d", 
			STORED_DEFAULT_STATS, dwPlayerID, nStr, nDex, nVit, nInt, nWis, nRemainPoint) ;

	g_DB.Query(eQueryType_FreeQuery, eUpdateToDefaultStats, dwPlayerID, txt, dwPlayerID) ;
}

// 071129 LYW --- MapDBMsgParser : 스킬 초기화 쿼리 함수 추가.
void DB_ResetToDefaultSkill(DWORD dwPlayerID)
{
	sprintf( txt, "EXEC %s %d", STORED_DEFAULT_SKILL, dwPlayerID ) ;
	g_DB.Query(eQueryType_FreeQuery, eSkill_Reset, dwPlayerID, txt, dwPlayerID) ;
}

// 071129 LYW --- MapDBMsgParser : 누적 스킬 포인트 업데이트 함수 추가.
void DB_UpdateAccumulateSkillPoint(DWORD dwPlayerID, BOOL bForce, int nPoint)
{
	sprintf( txt, "EXEC %s %d, %d, %d", STORED_UPDATE_ACCSKILLPOINT, dwPlayerID, bForce, nPoint ) ;
	g_DB.Query( eQueryType_FreeQuery, eUpdateAccumulateSkillpoint, dwPlayerID, txt, dwPlayerID ) ;
}


void InsertLogCharacter( DWORD charIdx, LEVELTYPE level, HERO_TOTALINFO* pTotalInfo )
{
	g_DB.LogMiddleQuery(
		0,
		0,
		"EXEC dbo.up_CharacterLog %d, %d, %d, %d, %d, %d, %d, %d",
		charIdx,
		level,
		pTotalInfo->Str,
		pTotalInfo->Dex,
		pTotalInfo->Vit,
		pTotalInfo->Int,
		pTotalInfo->Wis,
		pTotalInfo->LevelUpPoint );
}


void InsertLogExp( BYTE bType, DWORD charIdx, LEVELTYPE level, EXPTYPE changeValue, EXPTYPE NowExp, WORD MurdererKind, DWORD MurdererIdx, DWORD CurAbilPoint)
{
	// 080605 LYW --- MapDBMsgParser : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
	//g_DB.LogMiddleQuery(
	//	0,
	//	0,
	//	"EXEC dbo.up_ExpPointLog %d, %d, %d, %u, %u, %d, %d, %d",
	//	bType,
	//	charIdx,
	//	level,
	//	changeValue,
	//	NowExp,
	//	MurdererKind,
	//	MurdererIdx,
	//	CurAbilPoint );

	g_DB.LogMiddleQuery(
		0,
		0,
		"EXEC dbo.up_ExpPointLog %d, %d, %d, %0.f, %0.f, %d, %d, %d",
		bType,
		charIdx,
		level,
		(float)changeValue,
		(float)NowExp,
		MurdererKind,
		MurdererIdx,
		CurAbilPoint );
}


void InsertLogMoney( BYTE type, DWORD charIdx, MONEYTYPE InvenMoney, MONEYTYPE ChangeValue, DWORD TargetID, MONEYTYPE TargetMoney )
{
	g_DB.LogMiddleQuery(
		0,
		0,
		"EXEC dbo.up_MoneyLog %d, %d, %u, %u, %d, %u",	
		type,
		charIdx,
		InvenMoney,
		ChangeValue,
		TargetID,
		TargetMoney );
}

void InsertLogWorngMoney( BYTE type, DWORD charIdx, MONEYTYPE changeValue, MONEYTYPE InvenMoney, MONEYTYPE StorageMoney, DWORD targetIdx )
{
	g_DB.LogMiddleQuery(
		0,
		0,
		"EXEC dbo.up_WrongMoneyLog %d, %d, %u, %u, %u, %d",
		type,
		charIdx,
		changeValue,
		InvenMoney,
		StorageMoney,
		targetIdx );	
}

void LogItemMoney(DWORD FromChrIdx, char* FromChrName, DWORD ToChrIdx, char* ToChrName,
				  eLogitemmoney LogType, MONEYTYPE FromTotalMoney, MONEYTYPE ToTotalMoney, MONEYTYPE ChangeMoney,
				  DWORD ItemIdx, DWORD ItemDBIdx, POSTYPE ItemFromPos, POSTYPE ItemToPos,
				  DURTYPE ItemDur, EXPTYPE ExpPoint)
{
	// 080605 LYW --- MapDBMsgParser : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
	//g_DB.LogMiddleQuery(
	//	0,
	//	0,
	//	"EXEC dbo.up_ItemMoneyLog %d, \'%s\', %d, \'%s\', %d, %u, %u, %u, %d, %d, %d, %d, %u, %u",
	//	FromChrIdx,
	//	FromChrName,
	//	ToChrIdx,
	//	ToChrName, 
	//	LogType,
	//	FromTotalMoney,
	//	ToTotalMoney,
	//	ChangeMoney,
	//	ItemIdx,
	//	ItemDBIdx,
	//	ItemFromPos,
	//	ItemToPos,
	//	ItemDur,
	//	ExpPoint );

	g_DB.LogMiddleQuery(
		0,
		0,
		"EXEC dbo.up_ItemMoneyLog %d, \'%s\', %d, \'%s\', %d, %u, %u, %u, %d, %d, %d, %d, %u, %0.f",
		FromChrIdx,
		FromChrName,
		ToChrIdx,
		ToChrName, 
		LogType,
		FromTotalMoney,
		ToTotalMoney,
		ChangeMoney,
		ItemIdx,
		ItemDBIdx,
		ItemFromPos,
		ItemToPos,
		ItemDur,
		// 아이템 로그에는 경험치를 보여줄 필요가 없다하여, 0으로 처리함.
		0 );
}

// 080111 KTH -- 케릭터 이름이 변경된 내용을 로그에 남긴다.
void LogChangeName(DWORD CharacterIDX, char* beforeName, char* afterName)
{
	g_DB.LogMiddleQuery(
		0,
		0,
		"EXEC dbo.up_charrename_log_insert %d, \'%s\', \'%s\'",
		CharacterIDX,
		beforeName,
		afterName );
}

void LogCheat(DWORD Character_Idx, BYTE LogType, MONEYTYPE Param, DWORD ItemIdx, DWORD ItemDBIdx)
{
	g_DB.LogMiddleQuery(
		0,
		0,
		"EXEC dbo.UP_CHEATE_Log %d, %d, %d, %d, %d",
		Character_Idx,
		LogType,
		Param,
		ItemIdx,
		ItemDBIdx );
}

void LogItemOption(DWORD playerIndex, DWORD itemDbIndex, const ITEM_OPTION& option, eLogitemmoney type )
{
	const ITEM_OPTION::Reinforce&	reinforce	= option.mReinforce;
	const ITEM_OPTION::Mix&			mix			= option.mMix;
	const ITEM_OPTION::Enchant&		enchant		= option.mEnchant;

	g_DB.LogMiddleQuery(
		0,
		0,
		"EXEC DBO.TP_ITEM_OPTION_LOG_INSERT %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \'\'",
		playerIndex,
		itemDbIndex,
		reinforce.mStrength,
		reinforce.mDexterity,
		reinforce.mVitality,
		reinforce.mIntelligence,
		reinforce.mWisdom,
		reinforce.mLife,
		reinforce.mMana,
		reinforce.mLifeRecovery,
		reinforce.mManaRecovery,
		reinforce.mPhysicAttack,
		reinforce.mPhysicDefence,
		reinforce.mMagicAttack,
		reinforce.mMagicDefence,
		reinforce.mCriticalRate,
		reinforce.mCriticalDamage,
		reinforce.mMoveSpeed,
		reinforce.mEvade,
		reinforce.mAccuracy,
		mix.mStrength,
		mix.mIntelligence,
		mix.mDexterity,
		mix.mWisdom,
		mix.mVitality,
		enchant.mIndex,
		enchant.mLevel,
		type );
}

// 080327 LUJ, 드롭 옵션 로그 저장
void LogItemDropOption( const ITEM_OPTION& option, eLogitemmoney type )
{
	const ITEM_OPTION::Drop::Value& value1 = option.mDrop.mValue[ 0 ];
	const ITEM_OPTION::Drop::Value& value2 = option.mDrop.mValue[ 1 ];
	const ITEM_OPTION::Drop::Value& value3 = option.mDrop.mValue[ 2 ];
	const ITEM_OPTION::Drop::Value& value4 = option.mDrop.mValue[ 3 ];
	const ITEM_OPTION::Drop::Value& value5 = option.mDrop.mValue[ 4 ];

	g_DB.LogMiddleQuery(
		0,
		0,
		"INSERT INTO TB_ITEM_DROP_OPTION_LOG ( TYPE, ITEM_DBIDX, KEY1, VALUE1, KEY2, VALUE2, KEY3, VALUE3, KEY4, VALUE4, KEY5, VALUE5 ) VALUES ( %d, %d, %d, %f, %d, %f, %d, %f, %d, %f, %d, %f )",
		type,
		option.mItemDbIndex,
		value1.mKey,
		value1.mValue,
		value2.mKey,
		value2.mValue,
		value3.mKey,
		value3.mValue,
		value4.mKey,
		value4.mValue,
		value5.mKey,
		value5.mValue );
}

void LogGuild(DWORD CharacterIdx, DWORD GuildIdx, WORD LogType, DWORD Param1, DWORD Param2)
{
	g_DB.LogMiddleQuery(
		0,
		0,
		"EXEC dbo.up_GuildLog %d, %d, %d, %d, %d",
		CharacterIdx,
		GuildIdx,
		LogType,
		Param1,
		Param2 );
}

void LogGMToolUse( DWORD CharacterIdx, DWORD GMLogtype, DWORD Logkind, DWORD Param1, DWORD Param2 )
{
	// 080131 LUJ, 프로시저가 없음
	return;

	g_DB.LogMiddleQuery(
		0,
		0,
		"EXEC dbo.Up_GMToolUseLog %d, %d, %d, %d, %d",
		CharacterIdx,
		GMLogtype,
		Logkind,
		Param1,
		Param2 );
}


void InsertLogSkill( CPlayer* player, const SKILL_BASE* skill, DWORD type )
{
	if( ! player ||
		! skill )
	{
		ASSERT( 0 );
		return;
	}

	// type의 값은 CommonGameDefine.h의 eLogSkill 참조할 것
	g_DB.LogMiddleQuery( 0, 0, "EXEC DBO.TP_SKILL_LOG_INSERT %d, %d, %d, %d, %d, %d, \'\'",
		player->GetID(),
		player->GetSkillPoint(),
		skill->dwDBIdx,
		skill->Level,
		skill->wSkillIdx,
		type );
}


void InsertLogGuildWarehouse( CPlayer* player, POSTYPE previousPosition, const ITEMBASE* item, MONEYTYPE money, eLogitemmoney type )
{
	if( ! player  )
	{
		ASSERT( 0 );
		return;
	}

	if( item )
	{
		g_DB.LogMiddleQuery( 0, 0, "EXEC DBO.TP_GUILD_WAREHOUSE_LOG_INSERT %d, %d, %d, %d, %d, %d, %d, %u, %d, \'\'",
			player->GetID(),
			player->GetGuildIdx(),
			previousPosition,
			item->Position,
			item->wIconIdx,
			item->dwDBIdx,
			item->Durability,
			money,
			type );
	}
	else
	{
		g_DB.LogMiddleQuery( 0, 0, "EXEC DBO.TP_GUILD_WAREHOUSE_LOG_INSERT %d, %d, %d, %d, %d, %d, %d, %u, %d, \'\'",
			player->GetID(),
			player->GetGuildIdx(),
			previousPosition,
			0,
			0,
			0,
			0,
			money,
			type );
	}	
}


void InsertLogJob( CPlayer* player, DWORD classIndex, DWORD jobGrade, DWORD jobIndex )
{
	if( ! player )
	{
		ASSERT( 0 );
		return;
	}

	g_DB.LogMiddleQuery(
		0,
		0,
		"EXEC DBO.TP_JOB_LOG_INSERT %d, %d, %d, %d, \'\'",
		player->GetID(),
		classIndex,
		jobGrade,
		jobIndex );
}


void InsertLogGuildScore( DWORD guildIndex, float score, eGuildLog type )
{
	g_DB.LogMiddleQuery(
		0,
		0,
		"EXEC DBO.TP_GUILD_SCORE_LOG_INSERT %d, %0.5f, %d, \'""\'",
		guildIndex,
		score,
		type );
}


void InsertLogFarmCrop( CSHFarm* farm, CSHCrop* crop, eFamilyLog type )
{
	if( ! farm	||
		! crop )
	{
		ASSERT( 0 );
		return;
	}

	g_DB.LogMiddleQuery(
		0,
		0,
		"EXEC DBO.TP_FARM_CROP_LOG_INSERT %d, %d, %d, %d, %d, %d, %d, %d, \'""\'",
		farm->GetID(),
		crop->GetOwner(),
		crop->GetID(),
		crop->GetKind(),
		crop->GetStep(),
		crop->GetLife(),
		crop->GetSeedGrade(),
		type );
}

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2008.01.22
void InsertLogFarm( DWORD nZone, DWORD nFarm, DWORD nKind, DWORD nPlayerID, DWORD nValue01, DWORD nValue02)
{
	g_DB.LogMiddleQuery(
		0,
		0,
		"EXEC DBO.TP_FARM_LOG_INSERT %d, %d, %d, %d, %d, %d",
		nZone,
		nFarm,
		nKind,
		nPlayerID,
		nValue01,
		nValue02);
}
// E 농장시스템 추가 added by hseos 2008.01.22

void InsertLogQuest( CPlayer* player, DWORD mainQuestIndex, DWORD subQuestIndex, int index, int variation, eQuestLog type )
{
	g_DB.LogMiddleQuery(
		0,
		0,
		"EXEC DBO.TP_QUEST_LOG_INSERT %d, %d, %d, %d, %d, %d, \'\'",
		player->GetID(),
		mainQuestIndex,
		subQuestIndex,
		index,
		variation,
		type );
}


void InsertLogSkillExp( DWORD charIdx, EXPTYPE changeValue, DWORD skillIdx, DWORD skillDBIdx, LEVELTYPE skillLevel, EXPTYPE skillExp, POSTYPE skillPos ) 
{
	// 080605 LYW --- MapDBMsgParser : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
	//g_DB.LogMiddleQuery(
	//	0,
	//	0,
	//	"EXEC dbo.up_MungongExpLog %d, %u, %d, %d, %d, %d, %d",
	//	charIdx,
	//	changeValue,
	//	skillIdx,
	//	skillDBIdx,
	//	skillLevel,
	//	skillExp,
	//	skillPos );

	g_DB.LogMiddleQuery(
		0,
		0,
		"EXEC dbo.up_MungongExpLog %d, %0.f, %d, %d, %d, %0.f, %d",
		charIdx,
		(float)changeValue,
		skillIdx,
		skillDBIdx,
		skillLevel,
		(float)skillExp,
		skillPos );
}

void UpdateCharacterInfoByTime(DWORD charIdx, EXPTYPE exp, /*어빌리티 삭제 - 이영준 EXPTYPE abilityExp,*/ MONEYTYPE money, DWORD time, DWORD LastPKEndTime )
{
	// 080605 LYW --- MapDBMsgParser : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
	//	char txt[128];
	// desc_hseos_프로시저정리01
	// S 프로시저정리 추가 added by hseos 2007.06.21
	//sprintf(txt, "EXEC  %s %d, %u, %u, %d, %d",	"dbo.MP_CHARACTER_UpdateInfoByTime", 
	//	charIdx, exp, /*어빌리티 삭제 - 이영준 abilityExp,*/ money, time, LastPKEndTime );
	//sprintf(txt, "EXEC  %s %d, %u, %d, %u, %d, %d",	"dbo.MP_CHARACTER_UpdateInfoByTime", 
	//	charIdx, exp, 0, money, time, LastPKEndTime );
	sprintf(txt, "EXEC  %s %d, %0.f, %d, %u, %d, %d",	"dbo.MP_CHARACTER_UpdateInfoByTime", 
		charIdx, (float)exp, 0, money, time, LastPKEndTime );
	// E 프로시저정리 추가 added by hseos 2007.06.21
	g_DB.Query(eQueryType_FreeQuery, eCharacterInfoByTime, 0, txt, charIdx);
}

void UpdateSkillExpByTime(DWORD charIdx, DWORD DBIdx, EXPTYPE exp)
{
	// 080605 LYW --- MapDBMsgParser : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
	//	char txt[128];
	//sprintf(txt, "EXEC  %s %d, %d, %u",	"dbo.MP_SKILLTREE_UpdateExpBytime", 
	//	charIdx, DBIdx, exp);
	//g_DB.Query(eQueryType_FreeQuery, eSkillExpByTime, 0, txt, charIdx);

	sprintf(txt, "EXEC  %s %d, %d, %0.f",	"dbo.MP_SKILLTREE_UpdateExpBytime", 
		charIdx, DBIdx, (float)exp);
	g_DB.Query(eQueryType_FreeQuery, eSkillExpByTime, 0, txt, charIdx);
}

void InsertSpeedHackCheck(DWORD charIdx, char* charName, DWORD Distance, DWORD skillDelay)
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, \'%s\', %d, %d",	"dbo.up_SpeedHackCheck", 
		charIdx, charName, Distance, skillDelay);
	g_DB.Query(eQueryType_FreeQuery, eSpeedHackCheck, 0, txt, charIdx);
}


void InsertLogTool( DWORD dwLogType, DWORD dwLogKind, DWORD dwOperIdx, char* sOperName, DWORD dwTargetIdx, char* sTargetName,
				   DWORD dwParam1, DWORD dwParam2, DWORD dwParam3, DWORD dwParam4 )
{
	g_DB.LogMiddleQuery(
		0,
		0,
		"EXEC dbo.Up_ToolLog %d, %d, %d, \'%s\', %d, \'%s\', %d, %d, %d, %d",
		dwLogType,
		dwLogKind,
		dwOperIdx,
		sOperName,
		dwTargetIdx,
		sTargetName,
		dwParam1,
		dwParam2,
		dwParam3,
		dwParam4 );
}


// 071122 LYW --- MapDBMsgParser : 봉인 해제 처리.
void UnSealItemByRealTime(DWORD dwPlayerID, DWORD dwDBIdx, DWORD dwTime)
{
	sprintf(txt, "EXEC dbo.MP_ITEM_UnSealingByRealTime %d, %d", dwDBIdx, dwTime) ;
	g_DB.Query(eQueryType_FreeQuery, eUnSealItemByRealTime, dwPlayerID, txt, dwPlayerID) ;
}

void UnSealItemByGameTime(DWORD dwPlayerID, DWORD dwDBIdx, DWORD dwTime)
{
	sprintf(txt, "EXEC dbo.MP_ITEM_UnSealingByGameTime %d, %d", dwDBIdx, dwTime) ;
	g_DB.Query(eQueryType_FreeQuery, eUnSealItemByGameTime, dwPlayerID, txt, dwPlayerID) ;
}

void UpdateRemainTime(DWORD dwPlayerID, DWORD dwDBIdx, DWORD dwTime)
{
	sprintf(txt, "EXEC dbo.MP_ITEM_UpdateRemainTime %d, %d", dwDBIdx, dwTime) ;
	g_DB.Query(eQueryType_FreeQuery, eUpdateRemainTime, dwPlayerID, txt, dwPlayerID) ;
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////
// DB¡¤I¨￢IAI ￠￢¨­¨oAAo￠￢| ⓒi¹￠i½¨u¨¡¨u¡ⓒ Aⓒø￠￢￠cCI￠￥A CO¨uo￥ie.

//pjs
void  RAuctionCheck(LPQUERY pData,LPDBMESSAGE PMessage)
{
	if(atoi((char*)pData->Data[0]) == 0)
	{

	}
	else
	{		
		AUCTIONSUCCESS msg;

		memset(&msg,NULL,sizeof(msg));

		for(int i = 0; i < 10 ; i++)
		{

			strcpy(msg.resultlist[i].PRO_Name,((char*)pData[i].Data[sePRO_NAME]));
			//			msg.resultlist[i].PRO_Name = ((char*)pData[i].Data[sePRO_NAME]);
			msg.resultlist[i].PRO_Amount = atoi((char*)pData[i].Data[sePRO_Amount]);
			strcpy(msg.resultlist[i].Auctioneer,((char*)pData[i].Data[seAuctioneer]));
			//			msg.resultlist[i].Auctioneer = ((char*)pData[i].Data[seAuctioneer]);
			msg.resultlist[i].PRO_Index = atoi((char*)pData[i].Data[seREG_Index]);
		}
		g_Console.Log(eLogDisplay, 4, "AAA");
	}
}

//Select  PRO_Name, PRO_Amount, Auctioneer,REG_Index
//sePRO_NAME, sePRO_Amount, seAuctioneer,	seREG_Index


void RAuctionPageList(LPQUERY pData,LPDBMESSAGE PMessage)
{
	if(atoi((char*)pData->Data[0]) == 0)
	{

	}
	else
	{		
		BASELIST msg;

		memset(&msg,NULL,sizeof(msg));

		for(int i = 0; i < 10 ; i++)
		{
			msg.imfolist[i].REG_Index = atoi((char*)pData[i].Data[eREG_Index]);
			strcpy((char*)(msg.imfolist[i].BID_DUEDATE),((char*)pData[i].Data[eBID_DueDate]));
			strcpy(msg.imfolist[i].Auctioneer,((char*)pData[i].Data[eAuctioneer]));
			strcpy(msg.imfolist[i].BID_HighName,((char*)pData[i].Data[eHigh_Price_Name]));
			msg.imfolist[i].BID_HighPrice = atoi((char*)pData[i].Data[eBID_High_Price]);
			msg.imfolist[i].BID_ImmediatePrice = atoi((char*)pData[i].Data[eBID_Immediate_Price]);
			msg.imfolist[i].BID_LeftTime = atoi((char*)pData[i].Data[eBID_Left_Time]);
			msg.imfolist[i].JOIN_Amount = atoi((char*)pData[i].Data[eJOIN_Amount]);
			msg.imfolist[i].PRO_Amount = atoi((char*)pData[i].Data[ePRO_Amount]);
			msg.imfolist[i].PRO_Index = atoi((char*)pData[i].Data[ePRO_Index]);
			strcpy(msg.imfolist[i].PRO_Name,((char*)pData[i].Data[ePRO_NAME]));
		}
	}
}
/*
enum AuctionList
{
	ePRO_NAME, ePRO_Amount, eBID_High_Price, eBID_Immediate_Price, eBID_DueDate, eHigh_Price_Name, eJOIN_Amount, 
	eBID_Left_Time, eAuctioneer,ePRO_Index ,eBID_Starttime,eREG_Index, 
};
CEATE TABLE TB_IMSI(
					PRO_Name char(15), PRO_Amount  int, BID_HighPrice int, BID_ImmediatePrice int, BID_DueDate dateTime, BID_HighName char(15), JOIN_Amount int,
					BID_LeftTime int, Auctioneer char(15), PRO_Index int, BID_StartTime dateTime, REG_Index int)

					*/
					void RAuctionConfirm(LPQUERY pData, LPDBMESSAGE PMessage)
{

}

void RTestIDPWQuery(LPQUERY pData, LPDBMESSAGE pMessage)
{
	g_Console.Log(eLogDisplay,0 , "ID/PW Result : UserID -- %d", pData->Data[0]);
}

void RCharacterNumSendAndCharacterInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	if(!count)
	{
		// Aⓒø￠￢?AI d¨￢￠￢ⓒi¹￠i½¡¾a ¨oC¨¡¨￠￠?7u
		ASSERTMSG(0,"DB￠?￠® AE￠￢?AI ￥i¡IAIA￠￢¡Æ￠® ¨uⓒª¨o4I￠￥U.");
		return;
	}

	BASEOBJECT_INFO Objinfo;
	CHARACTER_TOTALINFO Totalinfo;
	HERO_TOTALINFO Heroinfo;
	BASEMOVE_INFO Moveinfo;
	memset(&Objinfo,0,sizeof(BASEOBJECT_INFO));
	memset(&Totalinfo,0,sizeof(CHARACTER_TOTALINFO));
	memset(&Heroinfo,0,sizeof(HERO_TOTALINFO));
	memset(&Moveinfo,0,sizeof(BASEMOVE_INFO));



	Objinfo.dwObjectID = atoi((char*)pData->Data[eCS_ObjectID]);
	Objinfo.dwUserID =  atoi((char*)pData->Data[eCS_UserID]);
	Objinfo.BattleID = 0;
	Objinfo.BattleTeam = 0;

	// UserIDX￠￥A ⓒøN¡¾aAo ¨uE￠￥A￠￥U
	Heroinfo.PartyID = atoi((char*)pData->Data[eCS_PartyID]);

	Totalinfo.Gender = atoi((char*)pData->Data[eCS_Gender]);
	Heroinfo.Str = atoi((char*)pData->Data[eCS_Str]);
	Totalinfo.Life = atoi((char*)pData->Data[eCS_Life]);
	Totalinfo.MaxLife = Totalinfo.Life;
	Heroinfo.Mana = atoi((char*)pData->Data[eCS_Energe]);
	Heroinfo.Dex = atoi((char*)pData->Data[eCS_Dex]);
	Heroinfo.Vit = atoi((char*)pData->Data[eCS_Vit]);
	Heroinfo.Int= atoi((char*)pData->Data[eCS_Int]);
	Heroinfo.Wis = atoi((char*)pData->Data[eCS_Wis]);
	Heroinfo.Fame = atoi((char*)pData->Data[eCS_Fame]);
	SafeStrCpy( Objinfo.ObjectName, (char*)pData->Data[eCS_Name], MAX_NAME_LENGTH + 1 );
	//Totalinfo.Stage = atoi((char*)pData->Data[eCS_Stage]);
	Totalinfo.Level = atoi((char*)pData->Data[eCS_Grade]);

	// 080602 LYW --- MapDBMsgParser : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
	//Heroinfo.ExpPoint = atoi((char*)pData->Data[eCS_Expoint]);
	Heroinfo.ExpPoint = _atoi64((char*)pData->Data[eCS_Expoint]);

	Heroinfo.LevelUpPoint = atoi((char*)pData->Data[eCS_GradeUpPoint]);
	Heroinfo.Money = atoi((char*)pData->Data[eCS_Money]);
	Totalinfo.CurMapNum = GAMERESRCMNGR->GetLoadMapNum();
	Totalinfo.LoginMapNum = atoi((char*)pData->Data[eCS_Map]);
	Totalinfo.FaceType = atoi((char*)pData->Data[eCS_FaceType]);
	Totalinfo.HairType = atoi((char*)pData->Data[eCS_HeadType]);
	Totalinfo.WearedItemIdx[eWearedItem_Hat] = atoi((char*)pData->Data[eCS_Hat]);
	Totalinfo.WearedItemIdx[eWearedItem_Weapon] = atoi((char*)pData->Data[eCS_Weapon]);
	Totalinfo.WearedItemIdx[eWearedItem_Dress] = atoi((char*)pData->Data[eCS_Dress]);
	Totalinfo.WearedItemIdx[eWearedItem_Shoes] = atoi((char*)pData->Data[eCS_Shoes]);
	Totalinfo.WearedItemIdx[eWearedItem_Glove] = atoi((char*)pData->Data[eCS_Glove]);
	Totalinfo.WearedItemIdx[eWearedItem_Shield] = atoi((char*)pData->Data[eCS_Shield]);
	Totalinfo.WearedItemIdx[eWearedItem_Band] = atoi((char*)pData->Data[eCS_Band]);

	Totalinfo.WearedItemIdx[eWearedItem_Earring1] = 0;
	Totalinfo.WearedItemIdx[eWearedItem_Earring2] = 0;
	Totalinfo.WearedItemIdx[eWearedItem_Necklace] = 0;
	Totalinfo.WearedItemIdx[eWearedItem_Ring1] = 0;
	Totalinfo.WearedItemIdx[eWearedItem_Ring2] = 0;
	Totalinfo.WearedItemIdx[eWearedItem_Belt] = 0;

	Totalinfo.WearedItemIdx[eWearedItem_Costume_Head] = atoi((char*)pData->Data[eCS_Costume_Hat]);
	Totalinfo.WearedItemIdx[eWearedItem_Costume_Dress] = atoi((char*)pData->Data[eCS_Costume_Dress]);
	Totalinfo.WearedItemIdx[eWearedItem_Costume_Glove] = atoi((char*)pData->Data[eCS_Costume_Glove]);
	Totalinfo.WearedItemIdx[eWearedItem_Costume_Shoes] = atoi((char*)pData->Data[eCS_Costume_Shoes]);
	Totalinfo.WearedItemIdx[eWearedItem_Glasses] = atoi((char*)pData->Data[eCS_Glasses]);
	Totalinfo.WearedItemIdx[eWearedItem_Mask] = atoi((char*)pData->Data[eCS_Mask]);
	Totalinfo.WearedItemIdx[eWearedItem_Wing] = atoi((char*)pData->Data[eCS_Wing]);

	Heroinfo.SkillPoint = (DWORD)atoi((char*)pData->Data[eCS_SkillPoint]);

	Totalinfo.MunpaID = atoi((char*)pData->Data[eCS_MunpaIDX]);
	if( Totalinfo.MunpaID )
	{
		CGuild* pGuild = GUILDMGR->GetGuild( Totalinfo.MunpaID );

		if( pGuild )
		{
			const GUILDMEMBERINFO* member = pGuild->GetMemberInfo( Objinfo.dwObjectID );

			// 071128 웅주, 길드 회원 정보가 0으로 반환되는 경우 있음. 탈퇴가 제대로 안된 경우일 수 있음.
			if( member )
			{
				Totalinfo.PositionInMunpa = member->Rank;
			}

			SafeStrCpy(Totalinfo.GuildName, pGuild->GetGuildName(), sizeof( Totalinfo.GuildName ) );
		}		
	}
	else
	{
		Totalinfo.PositionInMunpa = 0;
		Totalinfo.GuildName[0] = 0;
	}
	SafeStrCpy(Totalinfo.NickName, (char*)pData->Data[eCS_NickName], sizeof( Totalinfo.FamilyNickName ) );

	Totalinfo.FamilyID = atoi((char*)pData->Data[eCS_FamilyID]);

	if( Totalinfo.FamilyID )
	{
		//CFamily* pFamily = FAMILYMGR->GetFamily( Totalinfo.FamilyID );
		//Totalinfo.PositionInFamily = pFamily->GetMemberInfo(Objinfo.dwObjectID)->Rank;
		//SafeStrCpy(Totalinfo.FamilyName, pFamily->GetFamilyName(), MAX_FAMILY_NAME+1);
	}
	else
	{
		Totalinfo.PositionInFamily = 0;
		Totalinfo.FamilyName[0] = 0;
	}
	SafeStrCpy(Totalinfo.FamilyNickName, (char*)pData->Data[eCS_FamilyNickName], sizeof( Totalinfo.FamilyNickName ) );

	Totalinfo.BadFame = atoi((char*)pData->Data[eCS_BadFame]);


	Totalinfo.MapChangePoint_Index = atoi((char*)pData->Data[eCS_MapChangePoint]);
	Totalinfo.LoginPoint_Index = atoi((char*)pData->Data[eCS_LoginPoint]);

	Heroinfo.KyungGongGrade = 0;


	/*어빌리티 삭제 - 이영준 Heroinfo.AbilityExp = atoi((char*)pData->Data[eCS_AbilityExp]);*/

	Totalinfo.Height = 1.0f;
	Totalinfo.Width = 1.0f;
	Heroinfo.Playtime = atoi((char*)pData->Data[eCS_Playtime]);
	Heroinfo.LastPKModeEndTime = atoi((char*)pData->Data[eCS_LastPKEndTime]);

	Heroinfo.MaxLevel = atoi((char*)pData->Data[eCS_MaxLevel]);
	Totalinfo.MarkName	= atoi((char*)pData->Data[eCS_MarkName]);
	SafeStrCpy(Heroinfo.MunpaCanEntryDate, (char*)pData->Data[eCS_MunpaCanEntryDate], 11);

	Totalinfo.Race = atoi((char*)pData->Data[eCS_Race]);
	Totalinfo.JobGrade = atoi((char*)pData->Data[eCS_JobGrade]);
	Totalinfo.Job[0] = atoi((char*)pData->Data[eCS_Job1]);
	Totalinfo.Job[1] = atoi((char*)pData->Data[eCS_Job2]);
	Totalinfo.Job[2] = atoi((char*)pData->Data[eCS_Job3]);
	Totalinfo.Job[3] = atoi((char*)pData->Data[eCS_Job4]);
	Totalinfo.Job[4] = atoi((char*)pData->Data[eCS_Job5]);
	Totalinfo.Job[5] = atoi((char*)pData->Data[eCS_Job6]);

	Totalinfo.HideLevel = 0;
	Totalinfo.DetectLevel = 0;

	// 071210 KTH -- 인벤토리 확장이 얼마나 되어 있는지 가져온다.
	Totalinfo.wInventoryExpansion = atoi((char*)pData->Data[eCS_Inventory]);

	if(Heroinfo.MaxLevel < Totalinfo.Level)
		Heroinfo.MaxLevel = Totalinfo.Level;

//	VECTOR3 pos,RandPos;
//	GAMERESRCMNGR->GetRegenPoint(pos,RandPos,Totalinfo.LoginPoint_Index,Totalinfo.MapChangePoint_Index);

	CPlayer* pPlayer = g_pServerSystem->InitPlayerInfo(&Objinfo, &Totalinfo, &Heroinfo);
	if(pPlayer == NULL)
		return;

	pPlayer->SetLoginTime(Heroinfo.Playtime);

	CItemSlot* slot = pPlayer->GetSlot( eItemTable_Inventory );

/*	DWORD Position = pPlayer->GetInitPoint();
	if( Position )
	{
		WORD x, z;
		stMOVEPOINT Mp;
		Mp.value = Position;
		Mp.GetMovePoint( x, z );
		pPlayer->SetInitPoint( 0 );
		RandPos.x = x;
		RandPos.z = z;

		CCharMove::SetPosition(pPlayer, &RandPos);
	}

	CCharMove::InitMove(pPlayer,&RandPos);
*/
	VECTOR3 pos;

	pos.x = (float)atoi((char*)pData->Data[eCS_Pos_X]);
	pos.y = 0;
	pos.z = (float)atoi((char*)pData->Data[eCS_Pos_Y]);

	CCharMove::InitMove(pPlayer,&pos);

	// guildunion info setting
	GUILDUNIONMGR->SetInfoToPlayer( pPlayer, Totalinfo.MunpaID );

	pPlayer->SetInitState(PLAYERINITSTATE_HERO_INFO,pMessage->dwID);

//---KES AUTONOTE
	AutoNoteListLoad( Objinfo.dwObjectID );

//---------------
}

void RCharacterInfoQueryToEventMap(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD dwTeam = pMessage->dwID;
	DWORD count = pMessage->dwResult;
	if( !count )	return;

	BASEOBJECT_INFO Objinfo;
	CHARACTER_TOTALINFO Totalinfo;
	HERO_TOTALINFO Heroinfo;
	BASEMOVE_INFO Moveinfo;
	memset(&Objinfo,0,sizeof(BASEOBJECT_INFO));
	memset(&Totalinfo,0,sizeof(CHARACTER_TOTALINFO));
	memset(&Heroinfo,0,sizeof(HERO_TOTALINFO));
	memset(&Moveinfo,0,sizeof(BASEMOVE_INFO));	

	Objinfo.dwObjectID = atoi((char*)pData->Data[eCS_ObjectID]);
	Objinfo.dwUserID =  atoi((char*)pData->Data[eCS_UserID]);
	Objinfo.BattleID = 0;
	Objinfo.BattleTeam = 0;

	Heroinfo.PartyID = atoi((char*)pData->Data[eCS_PartyID]);
	Totalinfo.Gender = atoi((char*)pData->Data[eCS_Gender]);
	Heroinfo.Str = atoi((char*)pData->Data[eCS_Str]);
	Totalinfo.Life = atoi((char*)pData->Data[eCS_Life]);
	Totalinfo.MaxLife = Totalinfo.Life;
	Heroinfo.Mana = atoi((char*)pData->Data[eCS_Energe]);
	Heroinfo.Dex = atoi((char*)pData->Data[eCS_Dex]);
	Heroinfo.Vit = atoi((char*)pData->Data[eCS_Vit]);
	Heroinfo.Int= atoi((char*)pData->Data[eCS_Int]);
	Heroinfo.Wis = atoi((char*)pData->Data[eCS_Wis]);
	//	Heroinfo.Fame = atoi((char*)pData->Data[eCS_Fame]);
	Heroinfo.Fame = 0; //now, fame not used... but, it's occur some bugs.
	SafeStrCpy( Objinfo.ObjectName, (char*)pData->Data[eCS_Name], MAX_NAME_LENGTH + 1 );
	//Totalinfo.Stage = atoi((char*)pData->Data[eCS_Stage]);
	Totalinfo.Level = atoi((char*)pData->Data[eCS_Grade]);

	// 080605 LYW --- MapDBMsgParser : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
	//Heroinfo.ExpPoint = atoi((char*)pData->Data[eCS_Expoint]);
	Heroinfo.ExpPoint = _atoi64((char*)pData->Data[eCS_Expoint]);

	Heroinfo.LevelUpPoint = atoi((char*)pData->Data[eCS_GradeUpPoint]);
	Heroinfo.Money = atoi((char*)pData->Data[eCS_Money]);
	Totalinfo.CurMapNum = GAMERESRCMNGR->GetLoadMapNum();
	Totalinfo.LoginMapNum = atoi((char*)pData->Data[eCS_Map]);
	Totalinfo.FaceType = atoi((char*)pData->Data[eCS_FaceType]);
	Totalinfo.HairType = atoi((char*)pData->Data[eCS_HeadType]);
	Totalinfo.WearedItemIdx[eWearedItem_Hat] = atoi((char*)pData->Data[eCS_Hat]);
	Totalinfo.WearedItemIdx[eWearedItem_Weapon] = atoi((char*)pData->Data[eCS_Weapon]);
	Totalinfo.WearedItemIdx[eWearedItem_Dress] = atoi((char*)pData->Data[eCS_Dress]);
	Totalinfo.WearedItemIdx[eWearedItem_Shoes] = atoi((char*)pData->Data[eCS_Shoes]);
	Totalinfo.WearedItemIdx[eWearedItem_Glove] = atoi((char*)pData->Data[eCS_Glove]);
	Totalinfo.WearedItemIdx[eWearedItem_Shield] = atoi((char*)pData->Data[eCS_Shield]);
	Totalinfo.WearedItemIdx[eWearedItem_Band] = atoi((char*)pData->Data[eCS_Band]);

	Totalinfo.WearedItemIdx[eWearedItem_Earring1] = 0;
	Totalinfo.WearedItemIdx[eWearedItem_Earring2] = 0;
	Totalinfo.WearedItemIdx[eWearedItem_Necklace] = 0;
	Totalinfo.WearedItemIdx[eWearedItem_Ring1] = 0;
	Totalinfo.WearedItemIdx[eWearedItem_Ring2] = 0;
	Totalinfo.WearedItemIdx[eWearedItem_Belt] = 0;

	Totalinfo.WearedItemIdx[eWearedItem_Costume_Head] = atoi((char*)pData->Data[eCS_Costume_Hat]);
	Totalinfo.WearedItemIdx[eWearedItem_Costume_Dress] = atoi((char*)pData->Data[eCS_Costume_Dress]);
	Totalinfo.WearedItemIdx[eWearedItem_Costume_Glove] = atoi((char*)pData->Data[eCS_Costume_Glove]);
	Totalinfo.WearedItemIdx[eWearedItem_Costume_Shoes] = atoi((char*)pData->Data[eCS_Costume_Shoes]);
	Totalinfo.WearedItemIdx[eWearedItem_Glasses] = atoi((char*)pData->Data[eCS_Glasses]);
	Totalinfo.WearedItemIdx[eWearedItem_Mask] = atoi((char*)pData->Data[eCS_Mask]);
	Totalinfo.WearedItemIdx[eWearedItem_Wing] = atoi((char*)pData->Data[eCS_Wing]);

	Heroinfo.SkillPoint = (DWORD)atoi((char*)pData->Data[eCS_SkillPoint]);

	Totalinfo.MunpaID = atoi((char*)pData->Data[eCS_MunpaIDX]);
	if( Totalinfo.MunpaID )
	{
		CGuild* pGuild = GUILDMGR->GetGuild( Totalinfo.MunpaID );
		Totalinfo.PositionInMunpa = pGuild->GetMemberInfo(Objinfo.dwObjectID)->Rank;
		SafeStrCpy(Totalinfo.GuildName, pGuild->GetGuildName(), MAX_GUILD_NAME+1);
	}
	else
	{
		Totalinfo.PositionInMunpa = 0;
		Totalinfo.GuildName[0] = 0;
	}
	SafeStrCpy(Totalinfo.NickName, (char*)pData->Data[eCS_NickName], MAX_GUILD_NICKNAME+1);

	Totalinfo.FamilyID = atoi((char*)pData->Data[eCS_FamilyID]);
	if( Totalinfo.FamilyID )
	{
		//CFamily* pFamily = FAMILYMGR->GetFamily( Totalinfo.FamilyID );
		//Totalinfo.PositionInFamily = pFamily->GetMemberInfo(Objinfo.dwObjectID)->Rank;
		//SafeStrCpy(Totalinfo.FamilyName, pFamily->GetFamilyName(), MAX_FAMILY_NAME+1);
	}
	else
	{
		Totalinfo.PositionInFamily = 0;
		Totalinfo.FamilyName[0] = 0;
	}
	SafeStrCpy(Totalinfo.FamilyNickName, (char*)pData->Data[eCS_FamilyNickName], sizeof( Totalinfo.FamilyNickName ) );

	Totalinfo.BadFame = atoi((char*)pData->Data[eCS_BadFame]);

	Totalinfo.MapChangePoint_Index = atoi((char*)pData->Data[eCS_MapChangePoint]);
	Totalinfo.LoginPoint_Index = atoi((char*)pData->Data[eCS_LoginPoint]);

	Heroinfo.KyungGongGrade = 0;

	/*어빌리티 삭제 - 이영준 Heroinfo.AbilityExp = atoi((char*)pData->Data[eCS_AbilityExp]);*/

	Totalinfo.Height = 1.0f;
	Totalinfo.Width = 1.0f;
	Heroinfo.Playtime = atoi((char*)pData->Data[eCS_Playtime]);
	Heroinfo.LastPKModeEndTime = atoi((char*)pData->Data[eCS_LastPKEndTime]);

	Heroinfo.MaxLevel = atoi((char*)pData->Data[eCS_MaxLevel]);
	Totalinfo.MarkName	= atoi((char*)pData->Data[eCS_MarkName]);
	SafeStrCpy(Heroinfo.MunpaCanEntryDate, (char*)pData->Data[eCS_MunpaCanEntryDate], 11);

	if(Heroinfo.MaxLevel < Totalinfo.Level)
		Heroinfo.MaxLevel = Totalinfo.Level;

	Totalinfo.Race = atoi((char*)pData->Data[eCS_Race]);
	Totalinfo.JobGrade = atoi((char*)pData->Data[eCS_JobGrade]);
	Totalinfo.Job[0] = atoi((char*)pData->Data[eCS_Job1]);
	Totalinfo.Job[1] = atoi((char*)pData->Data[eCS_Job2]);
	Totalinfo.Job[2] = atoi((char*)pData->Data[eCS_Job3]);
	Totalinfo.Job[3] = atoi((char*)pData->Data[eCS_Job4]);
	Totalinfo.Job[4] = atoi((char*)pData->Data[eCS_Job5]);
	Totalinfo.Job[5] = atoi((char*)pData->Data[eCS_Job6]);

	Totalinfo.HideLevel = 0;
	Totalinfo.DetectLevel = 0;

	VECTOR3 pos,RandPos;
	GAMERESRCMNGR->GetRegenPointFromEventMap(pos,RandPos,Totalinfo.LoginPoint_Index,Totalinfo.MapChangePoint_Index,dwTeam);

	CPlayer* pPlayer = g_pServerSystem->InitPlayerInfo(&Objinfo, &Totalinfo, &Heroinfo);
	if( pPlayer == NULL )	return;

	pPlayer->SetLoginTime(Heroinfo.Playtime);

	CCharMove::InitMove(pPlayer,&RandPos);

	pPlayer->SetInitState( PLAYERINITSTATE_HERO_INFO, MP_USERCONN_GAMEIN_SYN );
}

void RCharacterSkillTreeInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{

	DWORD count = pMessage->dwResult;
	//	if(count > MAX_TOTALSKILL_NUM)
	if(count > SLOT_SKILLTOTAL_NUM)
	{
		// ￥i¡IAIAI¡Æ￠® AE¡Æu￥i¨¡￠￥U.
	}
	else if((count == 1) && (atoi((char*)pData->Data[0]) == 0))
	{
		// ￥i¡IAIAI¡Æ￠® ¨uⓒª¨ui¨u¡ⓒ PlayerID￠￢| ⓒoYE?CO ¡Æⓒ¡￠?i¡ÆE¡ic
		CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(atoi((char*)pData->Data[1]));
		if(pPlayer == NULL)
			return;

		pPlayer->SetInitState(PLAYERINITSTATE_SKILL_INFO,pMessage->dwID);
	}
	else
	{
		CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser((DWORD)atoi((char*)pData[0].Data[eCM_ObjectID]));
		if(pPlayer == NULL)
			return;

		/*
		SKILLTREE_INFO Skillinfo;
		memset(&Skillinfo, 0, sizeof(SKILLTREE_INFO));
		pPlayer->InitSkillTreeInfo(&Skillinfo);

		SKILLTREE_INFO Skillinfo;
		const int maxoverlap = 100;
		int overlapcount = 0;
		SKILLBASE OverLapSkill[maxoverlap];
		memset(&OverLapSkill, 0, sizeof(SKILLBASE)*maxoverlap);
		memset(&Skillinfo, 0, sizeof(SKILLTREE_INFO));
		//		QUICKITEM quickItem;

		for(DWORD i = 0; i < count; i++)
		{
		WORD Skillidx = (WORD)atoi((char*)pData[i].Data[eCM_IDX]);

		POSTYPE SkillPos = ConvAbsPos2SkillPos(atoi((char*)pData[i].Data[eCM_Position]));
		SKILLBASE* pSkillBase = &Skillinfo.skill[SkillPos];

		//무공 포지션이 겹쳤다.
		if(pSkillBase->dwDBIdx != 0 && overlapcount < maxoverlap)
		{				
		pSkillBase = &OverLapSkill[overlapcount];
		++overlapcount;
		}

		pSkillBase->wIconIdx = Skillidx;//atoi((char*)pData[i].Data[eCM_IDX]);
		pSkillBase->dwDBIdx = atoi((char*)pData[i].Data[eCM_DBIDX]);
		pSkillBase->ExpPoint = atoi((char*)pData[i].Data[eCM_ExpPoint]);
		pSkillBase->Level = atoi((char*)pData[i].Data[eCM_Level]);
		pSkillBase->Position = atoi((char*)pData[i].Data[eCM_Position]);
		pSkillBase->QuickPosition = atoi((char*)pData[i].Data[eCM_QPosition]);
		pSkillBase->bWear = FALSE;

		//////////////////////////////////////////////////////////////////////////
		// 06. 06. 2차 전직 - 이영준
		// 무공 변환 추가
		pSkillBase->OptionIndex = atoi((char*)pData[i].Data[eCM_Option]);
		//////////////////////////////////////////////////////////////////////////
		}

		//중복된 무공이 있으면 빈칸에 넣어준다.
		for(int n=0;n<overlapcount;++n)
		{
		for(int m=0;m<SLOT_SKILL_NUM;++m)
		{
		//빈칸이면 넣어준다.
		if(Skillinfo.skill[m].dwDBIdx == 0)
		{
		Skillinfo.skill[m] = OverLapSkill[n];
		Skillinfo.skill[m].Position = TP_SKILL1_START + m;
		break;
		}
		}

		//만약 빈칸이 없으면 더이상 처리하지 않음
		if(m==SLOT_SKILL_NUM)
		break;
		}

		// Player￠?￠® ⓒo¡i¡Æⓒªd¨￢￠￢ Aⓒ￢¡Æ￠®
		pPlayer->InitSkillTreeInfo(&Skillinfo);
		*/
		cSkillTree* pSkillTree = pPlayer->GetSkillTree();

		for( DWORD i = 0; i < count; ++i )
		{
			SKILL_BASE* pSkillBase = pSkillTree->NewSkillBase();

			pSkillBase->dwDBIdx		= atoi((char*)pData[i].Data[eCM_DBIDX]);
			const DWORD skillIndex	= atoi((char*)pData[i].Data[eCM_IDX]);
			const BYTE skillLevel	= BYTE( atoi((char*)pData[i].Data[eCM_Level]) );

			pSkillBase->wSkillIdx	= skillIndex;
			pSkillBase->Level		= skillLevel;

			// 먼저 세트 아이템으로 스킬이 적용되어 있는 상태인지 확인한다. 아이템과 스킬이 쿼리되나 순차성이
			// 보장되지 않으므로 어떤 쿼리가 먼저 도착할지 알 수 없다. 따라서, 이미 세트 아이템으로 인한
			// 스킬이 적용되어 있는 경우 쿼리된 결과는 저장하고, 세트 스킬또한 다시 적용해주어야 한다.
			{
				SKILL_BASE* currentSkill = pSkillTree->GetSkillBase( skillIndex );

				if(		currentSkill		&&
					!	currentSkill->dwDBIdx )
				{
					pPlayer->SetStoredSkill( *pSkillBase );

					pSkillBase->Level = currentSkill->Level + skillLevel;

					pSkillTree->RemoveSkillBase( currentSkill );
				}

				pSkillTree->AddSkillBase( pSkillBase );
			}
		}

		pPlayer->SetInitState(PLAYERINITSTATE_SKILL_INFO,pMessage->dwID);
	}
}


void RCharacterItemSlotInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pMessage->dwID );
	if( !pPlayer )			return;

	CharacterItemInfo( pPlayer->GetID(), MP_USERCONN_GAMEIN_SYN );
}


void RCharacterItemInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if( ! pMessage->dwResult )
	{
		return;
	}

	const DWORD playerIndex = atoi( ( char* )pData->Data[ 0 ] );

	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser( playerIndex );

	if( ! pPlayer)
	{
		return;
	}

	if( 1 == pMessage->dwResult  &&
		atoi((char*)pData->Data[0]) == 0 )
	{
		ITEM_TOTALINFO Iteminfo;
		memset(&Iteminfo, 0, sizeof(ITEM_TOTALINFO));
		pPlayer->InitItemTotalInfo(&Iteminfo);

		ShopItemInvenInfo( pPlayer->GetID() );

		return;
	}

	MSG_ITEM_OPTION optionMessage;
	{
		ZeroMemory( &optionMessage, sizeof( optionMessage ) );

		optionMessage.Category	= MP_ITEM;
		optionMessage.Protocol	= MP_ITEM_OPTION_ADD_ACK;
	}

	//진짜 주소를 가지고 와야.. InitItemTotalInfo() 함수 안에서의 for문 100번을 막을 수 있다.
	ITEM_TOTALINFO Iteminfo;
	ZeroMemory( &Iteminfo, sizeof( Iteminfo ) );

	const int maxoverlap = 100;
	int overlapcount = 0;
	ITEMBASE OverLapItem[maxoverlap];
	memset(&OverLapItem, 0, sizeof(ITEMBASE)*maxoverlap);
	memset(&Iteminfo, 0, sizeof(ITEM_TOTALINFO));
	for(DWORD  i = 0; i < pMessage->dwResult; i++)
	{
		POSTYPE ItemPos = atoi((char*)pData[i].Data[3]);
		ITEMBASE* pItemBase = NULL;
		// ¡¾¡¿￠￢￠c￥ia(AI¨￢¡IAa￠￢￠c)¨u¨¡AIAU ¨uA¨¡A
		//if(ItemPos >= TP_INVENTORY_START && ItemPos < TP_INVENTORY_END)
		// 071210 KTH -- Fix "TP_INVENTORY_END에서 TP_EXTENDED_INVENTORY2_END로 사이즈를 늘린다."
		if(ItemPos >= TP_INVENTORY_START && ItemPos < TP_EXTENDED_INVENTORY2_END)
		{
			ItemPos -= TP_INVENTORY_START;
			pItemBase = &Iteminfo.Inventory[ItemPos];
		}
		// AaA짢횕짢짙징짠u징짠짖짰AIAU 징짠uA징짠짖짰A
		else if(ItemPos >= TP_WEAR_START && ItemPos < TP_WEAR_END)
		{				
			POSTYPE ItemGrid = ItemPos - TP_WEAR_START;
			pItemBase = &Iteminfo.WearedItem[ItemGrid];
		}

		if(pItemBase == NULL)
		{
			// ￠?¨I¡¾a￠?￠® ￥ie¨ui￠?8e ¨u¨¡AIAU ¨¡¡AAo¨uC ￠?￠®¡¤?AO
			ASSERT(0);
			continue;
		}

		//아이템 포지션이 겹쳤다.
		BOOL bOverlap = FALSE;

		if(pItemBase->dwDBIdx != 0 && overlapcount < maxoverlap)
		{
			pItemBase = &OverLapItem[overlapcount];
			++overlapcount;

			bOverlap = TRUE;
		}
		
		pItemBase->dwDBIdx = atoi((char*)pData[i].Data[1]);
		pItemBase->wIconIdx = atoi((char*)pData[i].Data[2]);
		pItemBase->Position = atoi((char*)pData[i].Data[3]);
		pItemBase->QuickPosition = atoi((char*)pData[i].Data[4]);
		pItemBase->Durability = atoi((char*)pData[i].Data[5]);
		pItemBase->ItemParam = atoi((char*)pData[i].Data[6]);

		// 071122 LYW --- MapDBMsgParser : 시간 관련 추가.
		pItemBase->nSealed = (int)atoi((char*)pData[i].Data[7]) ;

		ITEM_INFO* ItemInfo = ITEMMGR->GetItemInfo( pItemBase->wIconIdx );

		if( ItemInfo == NULL )
			continue;
		
		// 071122 LYW
		if( ItemInfo->nTimeKind == eKIND_REALTIME && pItemBase->nSealed == eITEM_TYPE_UNSEAL )
		{
			pItemBase->nRemainSecond = (int)atoi((char*)pData[i].Data[8]) ;	//DB 에서 음수값이 넘어올 수도 있다.
			
			// 071125 KTH --- MapDBMsgParser : Item Over Time delete ""
			if( pItemBase->nRemainSecond <= 0 )
			{
				// 071231 KTH -- GetID Add
				ItemDeleteToDB(pItemBase->dwDBIdx );
				
				LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
					eLog_ShopItemUseEnd, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
					pItemBase->wIconIdx , pItemBase->dwDBIdx, pItemBase->Position, 0, pItemBase->Durability, pPlayer->GetPlayerExpPoint());

				memset(pItemBase, 0, sizeof(ITEMBASE));

				if( bOverlap )
					--overlapcount;
			}
		}
		else
			pItemBase->nRemainSecond = (int)atoi((char*)pData[i].Data[9]) ;

		/*
		080130 LUJ, 아이템 정보를 가져오는 작업은 (1) 옵션 가져오기 (2) 아이템 정보
					가져오기, 이렇게 두 부분으로 구성되었었다. 테이블이 플레이어나 
					유저에 따라 분리되어 있지 않기 때문에, 부득이하게 매우 큰 크기인
					TB_ITEM 테이블을 두 번 스캔해야했다. 이를 피하기 위해 TB_ITEM과
					TB_ITEM_OPTION을 외부 조인으로 결합시켜 스캔 회수를 1번으로 줄였다.
					한편, 드롭 아이템에 랜덤으로 붙은 TB_ITEM_DROP_OPTION도 조인으로
					결합시켜 가져온다
		*/
		{
			const QUERYST& record = pData[ i ];

			ITEM_OPTION& option = optionMessage.mOption[ optionMessage.mSize ];

			{
				ITEM_OPTION::Reinforce& data = option.mReinforce;

				data.mStrength			= atoi( ( char* )record.Data[ 10 ] );
				data.mDexterity			= atoi( ( char* )record.Data[ 11 ] );
				data.mVitality			= atoi( ( char* )record.Data[ 12 ] );
				data.mIntelligence		= atoi( ( char* )record.Data[ 13 ] );
				data.mWisdom			= atoi( ( char* )record.Data[ 14 ] );
				data.mLife				= atoi( ( char* )record.Data[ 15 ] );
				data.mMana				= atoi( ( char* )record.Data[ 16 ] );
				data.mLifeRecovery		= atoi( ( char* )record.Data[ 17 ] );
				data.mManaRecovery		= atoi( ( char* )record.Data[ 18 ] );
				data.mPhysicAttack		= atoi( ( char* )record.Data[ 19 ] );
				data.mPhysicDefence		= atoi( ( char* )record.Data[ 20 ] );
				data.mMagicAttack		= atoi( ( char* )record.Data[ 21 ] );
				data.mMagicDefence		= atoi( ( char* )record.Data[ 22 ] );
				data.mMoveSpeed			= atoi( ( char* )record.Data[ 23 ] );
				data.mEvade				= atoi( ( char* )record.Data[ 24 ] );
				data.mAccuracy			= atoi( ( char* )record.Data[ 25 ] );
				data.mCriticalRate		= atoi( ( char* )record.Data[ 26 ] );
				data.mCriticalDamage	= atoi( ( char* )record.Data[ 27 ] );
				SafeStrCpy( option.mReinforce.mMadeBy, ( char* )record.Data[ 28 ], sizeof( option.mReinforce.mMadeBy ) );
			}

			{
				ITEM_OPTION::Mix& data = option.mMix;

				data.mStrength		= atoi( ( char* )record.Data[ 29 ] );
				data.mIntelligence	= atoi( ( char* )record.Data[ 30 ] );
				data.mDexterity		= atoi( ( char* )record.Data[ 31 ] );
				data.mWisdom		= atoi( ( char* )record.Data[ 32 ] );
				data.mVitality		= atoi( ( char* )record.Data[ 33 ] );
				SafeStrCpy( option.mMix.mMadeBy, ( char* )record.Data[ 34 ], sizeof( option.mMix.mMadeBy ) );
			}

			{
				ITEM_OPTION::Enchant& data = option.mEnchant;

				data.mIndex	= atoi( ( char* )record.Data[ 35 ] );
				data.mLevel	= atoi( ( char* )record.Data[ 36 ] );
				SafeStrCpy( option.mEnchant.mMadeBy, ( char* )record.Data[ 37 ], sizeof( option.mEnchant.mMadeBy ) );				
			}

			{
				ITEM_OPTION::Drop& data = option.mDrop;

				data.mValue[ 0 ].mKey	= ITEM_OPTION::Drop::Key( atoi( ( char* )record.Data[ 38 ] ) );
				data.mValue[ 0 ].mValue	= float( atof( ( char* )record.Data[ 39 ] ) );

				data.mValue[ 1 ].mKey	= ITEM_OPTION::Drop::Key( atoi( ( char* )record.Data[ 40 ] ) );
				data.mValue[ 1 ].mValue	= float( atof( ( char* )record.Data[ 41 ] ) );

				data.mValue[ 2 ].mKey	= ITEM_OPTION::Drop::Key( atoi( ( char* )record.Data[ 42 ] ) );
				data.mValue[ 2 ].mValue	= float( atof( ( char* )record.Data[ 43 ] ) );

				data.mValue[ 3 ].mKey	= ITEM_OPTION::Drop::Key( atoi( ( char* )record.Data[ 44 ] ) );
				data.mValue[ 3 ].mValue	= float( atof( ( char* )record.Data[ 45 ] ) );

				data.mValue[ 4 ].mKey	= ITEM_OPTION::Drop::Key( atoi( ( char* )record.Data[ 46 ] ) );
				data.mValue[ 4 ].mValue	= float( atof( ( char* )record.Data[ 47 ] ) );
			}

			const ITEM_OPTION emptyOption = { 0 };

			if( memcmp( &emptyOption, &option, sizeof( emptyOption ) ) )
			{
				option.mItemDbIndex	= pItemBase->dwDBIdx;
				ITEMMGR->AddOption( option );

				++optionMessage.mSize;
			}
		}

		// 081202 LUJ, 수량성 아이템의 개수가 0인 경우, 삭제한다
		if(		ItemInfo->Stack &&
			!	pItemBase->Durability )
		{
			LogItemMoney(
				pPlayer->GetID(),
				pPlayer->GetObjectName(),
				0,
				"*invalid",
				eLog_ItemDiscard,
				pPlayer->GetMoney(),
				0,
				0,
				pItemBase->wIconIdx,
				pItemBase->dwDBIdx,
				pItemBase->Position,
				0,
				pItemBase->Durability,
				0 );
			ItemDeleteToDB( pItemBase->dwDBIdx );
			ZeroMemory( pItemBase, sizeof( *pItemBase ) );
		}
	}

	//중복된 아이템이 있으면 빈칸에 넣어준다.
	for(int n=0;n<overlapcount;++n)
	{
		// 071210 KTH -- 확장인벤토리까지 체크한다.
		for(int m=TP_INVENTORY_START;m<TP_EXTENDED_INVENTORY2_END;++m)
		{
			//빈칸이면 넣어준다.
			if(Iteminfo.Inventory[m].dwDBIdx == 0)
			{
				Iteminfo.Inventory[m] = OverLapItem[n];
				Iteminfo.Inventory[m].Position = m;
				break;
			}
		}

		//만약 빈칸이 없으면 더이상 처리하지 않음
		//if(m==TP_INVENTORY_END)
		// 071210 KTH -- 확장인벤토리를 넘으면 break;
		if(m==TP_EXTENDED_INVENTORY2_END)
			break;
	}

	pPlayer->SendMsg( &optionMessage, optionMessage.GetSize() );
	pPlayer->InitItemTotalInfo(&Iteminfo);
	pPlayer->SetInitState(PLAYERINITSTATE_ITEM_INFO, MP_USERCONN_GAMEIN_SYN );

	ShopItemInvenInfo( pPlayer->GetID() );
}

/*
void RCharacterGMControlInfo(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	if(pMessage->dwResult == 0)
		return;

	DWORD CharacterIdx = atoi((char*)pData[0].Data[0]);
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(CharacterIdx);
	if(pPlayer==NULL)
		return;

	for(int n=0;n<pMessage->dwResult;++n)
	{
		//TDN
		//	int kind = atoi((char*)pData[n].Data[1])

	}

	pPlayer->SetInitState(PLAYERINITSTATE_GMCONTROL_INFO,pMessage->dwID);
}

void RCharacterGMControlInsert(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	ASSERT(pMessage->dwResult == 1);

	DWORD CharacterIdx = atoi((char*)pData[0].Data[0]);
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(CharacterIdx);
	if(pPlayer)
	{
		//TDN
		//		int kind = atoi((char*)pData[n].Data[1]);

	}


	DWORD GMChrIdx = pMessage->dwID;
	CPlayer* pGM = (CPlayer *)g_pUserTable->FindUser(CharacterIdx);
	if(pGM==NULL)
		return;

	//TDN
}
*/

void RSSItemInsert(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if((atoi((char*)pData->Data[0]) == 0))
	{
		// Insert ¨oC¨¡¨￠

	}
	else
	{
		// Insert ¨u¨￢¡Æⓒª
		DWORD dwPlayerID = atoi((char*)pData->Data[eCI_ObjectID]);		// Aⓒø￠￢?¨u¨¡AI￥iⓒ￡
		ITEMBASE info;
		info.Position = atoi((char*)pData->Data[eCI_Position]);		// Item_Position
		info.QuickPosition = atoi((char*)pData->Data[eCI_QPosition]);
		info.dwDBIdx = atoi((char*)pData->Data[eCI_DBIDX]);		// Item_DBIDX
		info.Durability = atoi((char*)pData->Data[eCI_Durability]);	// Item_Durability
		info.wIconIdx = atoi((char*)pData->Data[eCI_IDX]);			// Item_IDX
		// 071126 KTH
		info.nSealed = atoi((char*)pData->Data[8]);
		info.nRemainSecond = atoi((char*)pData->Data[9]);

		STREETSTALLMGR->CreateDupItem( dwPlayerID, &info, pMessage->dwID);
	}
}


void	RCharacterItemGetCheatIDX(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if((atoi((char*)pData->Data[0]) == 0))
	{
		// Insert ¨oC¨¡¨￠

	}
	else
	{
		// Insert ¨u¨￢¡Æⓒª
		DWORD dwPlayerID = atoi((char*)pData->Data[eCI_ObjectID]);		// Aⓒø￠￢?¨u¨¡AI￥iⓒ￡
		ITEMBASE info;
		info.Position = atoi((char*)pData->Data[eCI_Position]);		// Item_Position
		info.dwDBIdx = atoi((char*)pData->Data[eCI_DBIDX]);		// Item_DBIDX
		info.QuickPosition = atoi((char*)pData->Data[eCI_QPosition]);
		info.Durability = atoi((char*)pData->Data[eCI_Durability]);	// Item_Durability
		info.wIconIdx = atoi((char*)pData->Data[eCI_IDX]);			// Item_IDX

		CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(dwPlayerID);
		if(pPlayer == NULL)
			return;

		//		ITEMMGR_OBJ->CheatItemDBResult(pPlayer,&info);
	}
}
void	RCharacterItemGetDivideIDX(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if((atoi((char*)pData->Data[0]) == 0))
	{
		// Insert ¨oC¨¡¨￠
	}
	else
	{
		// Insert ¨u¨￢¡Æⓒª
		atoi((char*)pData->Data[eCI_ObjectID]);		// Aⓒø￠￢?¨u¨¡AI￥iⓒ￡
		atoi((char*)pData->Data[eCI_DBIDX]);		// Item_DBIDX
		atoi((char*)pData->Data[eCI_IDX]);			// Item_IDX
		atoi((char*)pData->Data[eCI_Position]);		// Item_Position
		atoi((char*)pData->Data[eCI_QPosition]);		// Item_QuickPosition
		atoi((char*)pData->Data[eCI_Durability]);	// Item_Durability
	}
}

// 081118 LUJ, 캐릭터 번호를 쿼리를 실행한 DB 스레드를 선택할 키값으로 사용되도록 함
void ItemOptionInsert(const ITEM_OPTION& option, DWORD CharacterIdx )
{
	const ITEM_OPTION::Reinforce&	reinforce	= option.mReinforce;
	const ITEM_OPTION::Mix&			mix			= option.mMix;
	const ITEM_OPTION::Enchant&		enchant		= option.mEnchant;

	g_DB.FreeMiddleQuery(
		0,
		CharacterIdx,
		"EXEC dbo.MP_ITEM_OPTION_INSERT %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \'%s\', %d, %d, %d, %d, %d, \'%s\', %d, %d, \'%s\'",
		option.mItemDbIndex,
		reinforce.mStrength,
		reinforce.mDexterity,
		reinforce.mVitality,
		reinforce.mIntelligence,
		reinforce.mWisdom,
		reinforce.mLife,
		reinforce.mMana,
		reinforce.mLifeRecovery,
		reinforce.mManaRecovery,
		reinforce.mPhysicAttack,
		reinforce.mPhysicDefence,
		reinforce.mMagicAttack,
		reinforce.mMagicDefence,
		reinforce.mMoveSpeed,
		reinforce.mEvade,
		reinforce.mAccuracy,
		reinforce.mCriticalRate,
		reinforce.mCriticalDamage,
		reinforce.mMadeBy,
		mix.mStrength,
		mix.mIntelligence,
		mix.mDexterity,
		mix.mWisdom,
		mix.mVitality,
		mix.mMadeBy,
		enchant.mIndex,
		enchant.mLevel,
		enchant.mMadeBy );
}


void ItemDropOptionInsert( const ITEM_OPTION& option, DWORD CharacterIdx )
{
	const ITEM_OPTION::Drop::Value& value1 = option.mDrop.mValue[ 0 ];
	const ITEM_OPTION::Drop::Value& value2 = option.mDrop.mValue[ 1 ];
	const ITEM_OPTION::Drop::Value& value3 = option.mDrop.mValue[ 2 ];
	const ITEM_OPTION::Drop::Value& value4 = option.mDrop.mValue[ 3 ];
	const ITEM_OPTION::Drop::Value& value5 = option.mDrop.mValue[ 4 ];

	g_DB.FreeMiddleQuery(
		0,
		CharacterIdx,
		"EXEC DBO.MP_ITEM_DROP_OPTION_INSERT %d, %d, %f, %d, %f, %d, %f, %d, %f, %d, %f",
		option.mItemDbIndex,
		value1.mKey,
		value1.mValue,
		value2.mKey,
		value2.mValue,
		value3.mKey,
		value3.mValue,
		value4.mKey,
		value4.mValue,
		value5.mKey,
		value5.mValue );
}


/* New Item Return Func */
void RItemInsert(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD lastNo = LOWORD(pMessage->dwID);
	WORD ArrayID = HIWORD(pMessage->dwID);

	if( pMessage->dwResult )
	{
		// Insert ¨u¨￢¡Æⓒª

		DWORD dwObjectID = atoi((char*)pData->Data[eCI_ObjectID]);
		DWORD dwItemIdx = atoi((char*)pData->Data[eCI_IDX]);
		DWORD dwDura = atoi((char*)pData->Data[eCI_Durability]);
		DWORD dwPos = atoi((char*)pData->Data[eCI_Position]);
		DWORD dwQPos = atoi((char*)pData->Data[eCI_QPosition]);
		DWORD dwDBIdx = atoi((char*)pData->Data[eCI_DBIDX]);
		DWORD dwParam = atoi((char*)pData->Data[eCI_Param]);
		int	nSealed = atoi((char*)pData->Data[7]);

		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[eCI_ObjectID]));
		if(pPlayer == NULL)
			return;
		if(pPlayer->GetInited() == FALSE)
			return;

		ITEMOBTAINARRAYINFO * pItemArrayInfo = pPlayer->GetArray(ArrayID);
		if(pItemArrayInfo == NULL)
			return;

		pItemArrayInfo->ItemArray.AddItem( atoi((char*)pData->Data[eCI_DBIDX]), 
			atoi((char*)pData->Data[eCI_IDX]), 
			atoi((char*)pData->Data[eCI_Durability]), 
			atoi((char*)pData->Data[eCI_Position]), 
			atoi((char*)pData->Data[eCI_QPosition]),
			atoi((char*)pData->Data[eCI_Param]),
			0,	//atoi((char*)pData->Data[eCI_RareIdx]),
			atoi((char*)pData->Data[7])
			);

		if( lastNo == pItemArrayInfo->ItemArray.ItemNum )
		{
			pItemArrayInfo->CallBack( pPlayer, ArrayID );
			//ITEMMGR->ObtainItemDBResult( pPlayer, ArrayID);
		}
		else
		{
			LogItemMoney(
				0,
				"",
				pPlayer->GetID(),
				pPlayer->GetObjectName(),
				eLogitemmoney( pItemArrayInfo->wType ),
				0,
				pPlayer->GetMoney(),
				0,
				dwItemIdx,
				dwDBIdx,
				0,
				POSTYPE( dwPos ),
				dwDura,
				pPlayer->GetPlayerExpPoint() );
		}
	}
	else
	{
		ASSERT(0);
	}
}

/* New Skill Return Func */
void RSkillInsert(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if((atoi((char*)pData->Data[0]) == 0))
	{
		// ¨oC¨¡¨￠
		/*
		MSG_SKILL_ADD_NACK msg;
		msg.Category = MP_SKILLTREE;
		msg.Protocol = MP_SKILLTREE_ADD_NACK;
		msg.dwObjectID = atoi((char*)pData->Data[0]);

		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(msg.dwObjectID);
		if(pPlayer == NULL)
			return;
		SKILLMGR_OBJ->AddSkillNACK(pPlayer);
		*/
	}
	else
	{
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[0]));
		if(pPlayer == NULL)
			return;

		SKILL_BASE Skill;

		Skill.dwDBIdx = atoi((char*)pData->Data[eCM_DBIDX]);
		Skill.wSkillIdx = atoi((char*)pData->Data[eCM_IDX]);
		Skill.Level = atoi((char*)pData->Data[eCM_Level]);

		SKILLTREEMGR->AddSkillDBResult( pPlayer, &Skill );

		// 071109 웅주, 스킬 로그 추가
		InsertLogSkill( pPlayer, &Skill, eLog_SkillLearn );
	}
}

void RMapBaseEconomy(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if((atoi((char*)pData->Data[0]) == 0))
	{
		// Ai￠O¡Ei¨u¨oC¨¡¨￠
	}
	else
	{
		REGIST_BASEECONOMY msg;
		msg.MapNum = atoi((char*)pData->Data[eME_MEPNUM]);
		msg.OriginNum = atoi((char*)pData->Data[eME_ORIGINNUM]);
		msg.OriginPrice = atoi((char*)pData->Data[eME_ORIGINPRICE]);
		msg.OriginAmount = atoi((char*)pData->Data[eME_ORIGINAMOUNT]);
		msg.RequireNum = atoi((char*)pData->Data[eME_REQUIRENUM]);
		msg.RequirePrice = atoi((char*)pData->Data[eME_REQUIREPRICE]);
		msg.RequireAmount = atoi((char*)pData->Data[eME_REQUIREAMOUNT]);
		msg.BuyRates = atoi((char*)pData->Data[eME_BUYRATES]);
		msg.SellRates = atoi((char*)pData->Data[eME_SELLRATES]);

		ECONOMY_OBJ->SetRegistEconomy(&msg);
	}
}

void RAuctionPageQuery(LPQUERY pData, LPDBMESSAGE pMessage)
{
	/*	if((atoi((char*)pData->Data[0]) == 0))
	{
	// Ai￠O¡Ei¨u¨oC¨¡¨￠
	}
	else
	*/	{
	AUCTION_ONPAGELIST msg;
	for(int i = 0; i < 10; i++)
	{
		msg.Auction_List[i].ItemIdx = atoi((char*)pData[i].Data[eAT_ItemDBIdx]);
		msg.Auction_List[i].SellNum = atoi((char*)pData[i].Data[eAT_SellNum]);
		msg.Auction_List[i].SellerID = atoi((char*)pData[i].Data[eAT_SellerID]);
		msg.Auction_List[i].EndDate = atoi((char*)pData[i].Data[eAT_EndDate]);
		msg.Auction_List[i].EndTime = atoi((char*)pData[i].Data[eAT_EndTime]);
		msg.Auction_List[i].StartPrice = atoi((char*)pData[i].Data[eAT_StartPrice]);
		msg.Auction_List[i].ImmediatePrice = atoi((char*)pData[i].Data[eAT_ImmediatePrice]);
	}
	}
}


void RPartyLoad(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD PartyIDX = 0;
	for(DWORD i=0; i<pMessage->dwResult;++i)
	{
		PartyIDX = atoi((char*)pData[i].Data[ePA_IDX]);
		PARTYMGR->RegistParty(PartyIDX);
	}
	if(pMessage->dwResult == MAX_PARTY_LOAD_NUM)
	{
		PartyLoad(PartyIDX);
	}
}

void RPartyCreate(LPQUERY pData, LPDBMESSAGE pMessage) 
{
	DWORD CreateErr,PartyIdx,MasterIdx;
	BYTE Option;
	CreateErr = atoi((char*)pData->Data[ePA_ERR]);
	MasterIdx = atoi((char*)pData->Data[ePA_MASTER]);
	Option = atoi((char*)pData->Data[ePA_OPTION]);
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(MasterIdx);
	if(!pPlayer)
		return;

	if(CreateErr == NULL)
	{
		PARTYMGR->SendErrMsg(MasterIdx, eErr_Create_DB, MP_PARTY_CREATE_NACK);
		return;	
	}
	else
	{
		PartyIdx = atoi((char*)pData->Data[ePA_PARTYID]);
	}

	PARTYMGR->CreatePartyResult(pPlayer,PartyIdx,Option);
}

void RPartyInfoByUserLogin(LPQUERY pData, LPDBMESSAGE pMessage) 
{
	PARTYMEMBER member[MAX_PARTY_LISTNUM];
	BYTE opt;
	CPlayer* pPlayer;

	memset(member, 0, sizeof(PARTYMEMBER)*MAX_PARTY_LISTNUM);
	pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer == NULL)
		return;
	for(DWORD n=0;n<pMessage->dwResult;++n)
	{
		member[n].MemberID = atoi((char*)pData[n].Data[ePA_LIMemberId]);
		SafeStrCpy(member[n].Name,(char*)pData[n].Data[ePA_LIMemberName], MAX_NAME_LENGTH+1);
		member[n].bLogged = atoi((char*)pData[n].Data[ePA_LIMemberCurrentMap]) ? TRUE : FALSE;
		member[n].LifePercent = 0;
		member[n].ManaPercent = 0;
		member[n].Level = atoi((char*)pData[n].Data[ePA_LIMemberLevel]);
	}
	opt = atoi((char*)pData[0].Data[ePA_LIOption]);
	PARTYMGR->UserLogInAfterDBQuery(pMessage->dwID, pPlayer->GetPartyIdx(), member, opt);
}

void RPartyChangeMaster(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CParty* pParty = PARTYMGR->GetParty(pMessage->dwID);
	if(!pParty)
	{
		ASSERT(0);
		return;
	}

	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(pParty->GetMasterID());
	if(atoi((char*)pData->Data[ePA_CMErr]) == 0) //change master ack
	{
		PARTYMGR->DoChangeMasterParty(pParty, atoi((char*)pData->Data[ePA_CMTargetID]));
		pParty->SetMasterChanging(FALSE);
		PARTYMGR->NotifyChangesOtherMapServer(atoi((char*)pData->Data[ePA_CMTargetID]),pParty,MP_PARTY_NOTIFYCHANGEMASTER_TO_MAPSERVER, pMessage->dwID);
	}
	else //change master nack
	{
		if(!pMaster)
		{
			pParty->SetMasterChanging(FALSE);
			return;
		}

		MSGBASE msg;
		msg.Category = MP_PARTY;
		msg.Protocol = MP_PARTY_CHANGEMASTER_NACK;
		pMaster->SendMsg(&msg, sizeof(msg));
	}
}

void RPartyDelMember(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CParty* pParty = PARTYMGR->GetParty(pMessage->dwID);
	if(!pParty)
	{
		ASSERT(0);
		return;
	}
	if(atoi((char*)pData->Data[ePA_DMErr]) == 0) //del member ack
	{
		PARTYMGR->DelMember(atoi((char*)pData->Data[ePA_DMTargetID]), pMessage->dwID);
	}
	else //del member nack
	{
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[ePA_DMTargetID]));
		if(!pPlayer)
			return;
		MSG_INT msg;
		msg.Category = MP_PARTY;
		msg.Protocol = MP_PARTY_DEL_NACK;
		msg.nData	 = atoi((char*)pData->Data[ePA_DMErr]);
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
}

void RCharacterStorageInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer)
		return;

	BYTE StorageNum;

	if( pMessage->dwResult == 0 )
	{
		StorageNum = 0;
	}
	else
	{
		StorageNum = atoi((char*)pData->Data[ePyo_StorageNum]);
	}

	pPlayer->InitStorageInfo(StorageNum, atoi((char*)pData->Data[ePyo_Money]));

	pPlayer->SetInitState(PLAYERINITSTATE_STORAGE_INFO,MP_USERCONN_GAMEIN_SYN);
}

void RCharacterStorageItemInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	const DWORD playerIndex = pMessage->dwID;
	CPlayer*	pPlayer		= (CPlayer*)g_pUserTable->FindUser( playerIndex );

	if( ! pPlayer ||
		! pPlayer->GetInited() )
	{
		return;
	}

	STORAGE_ITEM StorageItem;
	memset(&StorageItem, 0, sizeof(STORAGE_ITEM));

	for( BYTE i = 0 ; i < pMessage->dwResult ; ++i )
	{	
		POSTYPE ItemPos = atoi((char*)pData[i].Data[3]);
		if(TP_STORAGE_START<= ItemPos && ItemPos < TP_STORAGE_END)
		{
			ItemPos -= TP_STORAGE_START;

			ITEMBASE& item = StorageItem.Storage[ItemPos];

			item.dwDBIdx		= atoi((char*)pData[i].Data[1]);
			item.wIconIdx		= atoi((char*)pData[i].Data[2]);
			item.Position		= atoi((char*)pData[i].Data[3]);
			item.QuickPosition	= atoi((char*)pData[i].Data[4]);
			item.Durability		= atoi((char*)pData[i].Data[5]);
			
			// 071125 KTH
			const ITEM_INFO* ItemInfo = ITEMMGR->GetItemInfo( item.wIconIdx );

			if( ! ItemInfo )
			{
				continue;
			}

			item.nSealed		= atoi((char*)pData[i].Data[7] );

			if( ItemInfo->nTimeKind == eKIND_REALTIME && item.nSealed == eITEM_TYPE_UNSEAL )
			{
				item.nRemainSecond = (int)atoi((char*)pData[i].Data[8]) ;	//DB 에서 음수값이 넘어올 수도 있다.
			
				// 071125 KTH --- MapDBMsgParser : Item Over Time delete "아이템의 사용시간이 지났으로 Discard 해준다."
				if( item.nRemainSecond <= 0 )
				{
					ItemDeleteToDB( item.dwDBIdx );

					LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
						eLog_ShopItemUseEnd, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
						item.wIconIdx , item.dwDBIdx, item.Position, 0, 
						item.Durability, pPlayer->GetPlayerExpPoint());

					ZeroMemory( &item, sizeof( item ) );
				}
			}
			else
			{
				item.nRemainSecond = (int)atoi((char*)pData[i].Data[9]) ;
			}

			// 옵션 로딩
			{
				const QUERYST& record = pData[ i ];

				ITEM_OPTION option = { 0 };

				{
					ITEM_OPTION::Reinforce& data = option.mReinforce;

					data.mStrength			= atoi( ( char* )record.Data[ 10 ] );
					data.mDexterity			= atoi( ( char* )record.Data[ 11 ] );
					data.mVitality			= atoi( ( char* )record.Data[ 12 ] );
					data.mIntelligence		= atoi( ( char* )record.Data[ 13 ] );
					data.mWisdom			= atoi( ( char* )record.Data[ 14 ] );
					data.mLife				= atoi( ( char* )record.Data[ 15 ] );
					data.mMana				= atoi( ( char* )record.Data[ 16 ] );
					data.mLifeRecovery		= atoi( ( char* )record.Data[ 17 ] );
					data.mManaRecovery		= atoi( ( char* )record.Data[ 18 ] );
					data.mPhysicAttack		= atoi( ( char* )record.Data[ 19 ] );
					data.mPhysicDefence		= atoi( ( char* )record.Data[ 20 ] );
					data.mMagicAttack		= atoi( ( char* )record.Data[ 21 ] );
					data.mMagicDefence		= atoi( ( char* )record.Data[ 22 ] );
					data.mMoveSpeed			= atoi( ( char* )record.Data[ 23 ] );
					data.mEvade				= atoi( ( char* )record.Data[ 24 ] );
					data.mAccuracy			= atoi( ( char* )record.Data[ 25 ] );
					data.mCriticalRate		= atoi( ( char* )record.Data[ 26 ] );
					data.mCriticalDamage	= atoi( ( char* )record.Data[ 27 ] );
					SafeStrCpy( option.mReinforce.mMadeBy, ( char* )record.Data[ 28 ], sizeof( option.mReinforce.mMadeBy ) );
				}

				{
					ITEM_OPTION::Mix& data = option.mMix;

					data.mStrength		= atoi( ( char* )record.Data[ 29 ] );
					data.mIntelligence	= atoi( ( char* )record.Data[ 30 ] );
					data.mDexterity		= atoi( ( char* )record.Data[ 31 ] );
					data.mWisdom		= atoi( ( char* )record.Data[ 32 ] );
					data.mVitality		= atoi( ( char* )record.Data[ 33 ] );
					SafeStrCpy( option.mMix.mMadeBy, ( char* )record.Data[ 34 ], sizeof( option.mMix.mMadeBy ) );
				}

				{
					ITEM_OPTION::Enchant& data = option.mEnchant;

					data.mIndex	= atoi( ( char* )record.Data[ 35 ] );
					data.mLevel	= atoi( ( char* )record.Data[ 36 ] );
					SafeStrCpy( option.mEnchant.mMadeBy, ( char* )record.Data[ 37 ], sizeof( option.mEnchant.mMadeBy ) );
				}

				{
					ITEM_OPTION::Drop& data = option.mDrop;

					data.mValue[ 0 ].mKey	= ITEM_OPTION::Drop::Key( atoi( ( char* )record.Data[ 38 ] ) );
					data.mValue[ 0 ].mValue	= float( atof( ( char* )record.Data[ 39 ] ) );

					data.mValue[ 1 ].mKey	= ITEM_OPTION::Drop::Key( atoi( ( char* )record.Data[ 40 ] ) );
					data.mValue[ 1 ].mValue	= float( atof( ( char* )record.Data[ 41 ] ) );

					data.mValue[ 2 ].mKey	= ITEM_OPTION::Drop::Key( atoi( ( char* )record.Data[ 42 ] ) );
					data.mValue[ 2 ].mValue	= float( atof( ( char* )record.Data[ 43 ] ) );

					data.mValue[ 3 ].mKey	= ITEM_OPTION::Drop::Key( atoi( ( char* )record.Data[ 44 ] ) );
					data.mValue[ 3 ].mValue	= float( atof( ( char* )record.Data[ 45 ] ) );

					data.mValue[ 4 ].mKey	= ITEM_OPTION::Drop::Key( atoi( ( char* )record.Data[ 46 ] ) );
					data.mValue[ 4 ].mValue	= float( atof( ( char* )record.Data[ 47 ] ) );
				}

				const ITEM_OPTION emptyOption = { 0 };

				if( memcmp( &emptyOption, &option, sizeof( emptyOption ) ) )
				{
					option.mItemDbIndex	= item.dwDBIdx;
					ITEMMGR->AddOption( option );
				}
			}

			// 081202 LUJ, 수량성 아이템의 개수가 0인 경우, 삭제한다
			if(		ItemInfo->Stack &&
				!	item.Durability )
			{
				LogItemMoney(
					pPlayer->GetID(),
					pPlayer->GetObjectName(),
					0,
					"*invalid",
					eLog_ItemDiscard,
					pPlayer->GetMoney(),
					0,
					0,
					item.wIconIdx,
					item.dwDBIdx,
					item.Position,
					0,
					item.Durability,
					0 );
				ItemDeleteToDB( item.dwDBIdx );
				ZeroMemory( &item, sizeof( item ) );
			}
		}
	}

	pPlayer->InitStorageItemInfo(&StorageItem);

	if( pMessage->dwResult >= MAX_ROW_NUM)
	{
		const QUERYST&	record		= pData[ MAX_ROW_NUM - 1 ];
		const DWORD		itemDbIndex = atoi( ( char* )record.Data[ 1 ] );

		CharacterStorageItemInfo(
			playerIndex,
			pPlayer->GetUserID(),
			itemDbIndex );
	}
	else //￠￥U A¨￠¨ui￠?8e A¡þ￠OoAI¨uⓒ￡¨¡￠c￠?￠® ¨￢￠￢ⓒø¡iAU. 
	{
		STORAGEMGR->SendStorageItemInfo(pPlayer);
	}
}

void RFriendNotifyLogouttoClient(LPQUERY pData, LPDBMESSAGE pMessage)
{
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		DWORD ID = atoi((char*)pData[i].Data[eFr_LToPlayerID]);
		CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(ID);
		if(!pPlayer)
		{
			MSG_NAME_DWORD cmsg;
			cmsg.Category = MP_FRIEND;
			cmsg.Protocol = MP_FRIEND_LOGOUT_NOTIFY_TO_AGENT;
			SafeStrCpy(cmsg.Name, (char*)pData[0].Data[eFr_LLogoutName], MAX_NAME_LENGTH+1);
			cmsg.dwData = pMessage->dwID;
			cmsg.dwObjectID = atoi((char*)pData[i].Data[eFr_LToPlayerID]);

			g_Network.Broadcast2AgentServer((char*)&cmsg,sizeof(cmsg));
		}
		else
		{
			MSG_NAME_DWORD msg;
			msg.Category = MP_FRIEND;
			msg.Protocol = MP_FRIEND_LOGOUT_NOTIFY_TO_CLIENT;
			SafeStrCpy(msg.Name, (char*)pData[0].Data[eFr_LLogoutName], MAX_NAME_LENGTH+1);
			msg.dwData = pMessage->dwID;			
			pPlayer->SendMsg(&msg, sizeof(msg));
		}
	}
}

void RWantedLoad(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WANTEDINFO_LOAD info;
	memset(&info, 0, sizeof(WANTEDINFO_LOAD));

	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		info.m_WantedInfo[i].WantedIDX = (DWORD)atoi((char*)pData[i].Data[eWa_LWantedIDX]);
		info.m_WantedInfo[i].WantedChrIDX = (DWORD)atoi((char*)pData[i].Data[eWa_LPlayerID]);
		info.m_WantedInfo[i].TotalPrize = (DWORD)atoi((char*)pData[i].Data[eWa_LTotlaPrize]);
		SafeStrCpy(info.m_WantedInfo[i].WantedChrName, (char*)pData[i].Data[eWa_LPlayerName], MAX_NAME_LENGTH+1);
		info.m_WantedInfo[i].RegistChrIDX = (DWORD)atoi((char*)pData[i].Data[eWa_LRegistChridx]);
		info.m_WantedInfo[i].TotalVolunteerNum = (int)atoi((char*)pData[i].Data[eWa_LTotalVolunteerNum]);
		info.count++;
	}

	WANTEDMGR->LoadWantedList(&info);	

	if(pMessage->dwResult >= MAX_QUERY_RESULT)
		WantedLoad(info.m_WantedInfo[i-1].WantedIDX);
	else
		//	g_pServerSystem->SetStart(TRUE);
	{
		GuildLoadGuild(0);
		//FamilyLoadFamily(0);
	}
}

void RWantedInfoByUserLogIn(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;	

	SEND_WANTEDLIST msg;
	memset(&msg, 0, sizeof(msg));
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_INFO_BY_USERLOGIN;
	msg.WantedIdx = (WORD)atoi((char*)pData[0].Data[eWa_LIMyWantedIDX]);
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{ //ⓒo¡iv¡ÆC 1¡Æⓒø￠￥A ⓒøN¨ui￠?A￠￥U.
		if((WORD)atoi((char*)pData[i].Data[eWa_LIWantedIDX]) != 0)
		{
			msg.List[i].WantedIDX = (WORD)atoi((char*)pData[i].Data[eWa_LIWantedIDX]);
			msg.List[i].WantedChrID = (DWORD)atoi((char*)pData[i].Data[eWa_LIWantedChrID]);
			SafeStrCpy(msg.List[i].WantedName, (char*)pData[i].Data[eWa_LIWantedName], MAX_NAME_LENGTH+1);
			SafeStrCpy(msg.List[i].RegistDate, (char*)pData[i].Data[eWa_LIRegistDate], 11);
			pPlayer->AddWanted((WORD)atoi((char*)pData[i].Data[eWa_LIWantedIDX]));
		}		
	}	
	pPlayer->SetWantedIdx(msg.WantedIdx);
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void RWantedRegist(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WANTEDINFO Info;
	Info.WantedIDX =  (WORD)atoi((char*)pData->Data[eWa_RWanted_idx]);
	Info.TotalPrize =  (MONEYTYPE)atoi((char*)pData->Data[eWa_RTotalPrize]);
	Info.WantedChrIDX = (DWORD)atoi((char*)pData->Data[eWa_RWantedChr_idx]);
	SafeStrCpy(Info.WantedChrName, (char*)pData->Data[eWa_RWanted_name], MAX_NAME_LENGTH+1);
	Info.RegistChrIDX = pMessage->dwID;
	Info.TotalVolunteerNum = 0;
	WANTEDMGR->RegistKillerResult(&Info);
}

void RWantedComplete(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = NULL;

	SEND_WANTED_NOTCOMPLETE msg;
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_NOTIFY_NOTCOMPLETE_TO_MAP;
	msg.WantedIDX = (WANTEDTYPE)pMessage->dwID;
	SafeStrCpy(msg.CompleterName, (char*)pData[0].Data[eWa_CCompleterName], MAX_NAME_LENGTH+1);
	msg.type = (BYTE)atoi((char*)pData[0].Data[eWa_CCompleteType]);

	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		msg.dwObjectID = (DWORD)atoi((char*)pData[i].Data[eWa_CCharacteridx]);
		pPlayer = (CPlayer*)g_pUserTable->FindUser((DWORD)atoi((char*)pData[i].Data[eWa_CCharacteridx]));
		if(!pPlayer)
		{
			//broadcasting
			g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
		}
		else
		{	
			WANTEDMGR->CanNotComplete(pPlayer, (WANTEDTYPE)pMessage->dwID, 
				(char*)pData[0].Data[eWa_CCompleterName], 
				(BYTE)atoi((char*)pData[0].Data[eWa_CCompleteType]));
		}
	}
}

void RWantedDestroyed(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = NULL;
	MSG_DWORD msg;
	msg.Category = MP_WANTED;
	msg.dwData = pMessage->dwID;

	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		msg.dwObjectID = (DWORD)atol((char*)pData[i].Data[0]);
		pPlayer = (CPlayer*)g_pUserTable->FindUser(msg.dwObjectID);
		if(pPlayer == 0)
		{
			//broadcasting
			msg.Protocol = MP_WANTED_DESTROYED_TO_MAP;
			g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
		}
		else
		{	
			if( pPlayer->IsWantedOwner((WANTEDTYPE)pMessage->dwID) == TRUE )
			{
				pPlayer->DeleteWanted((WANTEDTYPE)pMessage->dwID);
				msg.Protocol = MP_WANTED_DESTROYED_TO_CLIENT;
				pPlayer->SendMsg(&msg, sizeof(msg));
			}
		}
	}
}

void RWantedOrderedList(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;

	SEND_WANTNPCLIST msg;
	memset(&msg, 0, sizeof(msg));
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_ORDERLIST_ACK;
	msg.TotalPage = (WORD)atoi((char*)pData[0].Data[eWa_OLTotal]);
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		msg.WantNpcList[i].WantedIDX = (WANTEDTYPE)atoi((char*)pData[i].Data[eWa_OLWantedIDX]);
		SafeStrCpy(msg.WantNpcList[i].WantedName, (char*)pData[i].Data[eWa_OLWantedChrName], MAX_NAME_LENGTH+1);
		msg.WantNpcList[i].Prize = (MONEYTYPE)atoi((char*)pData[i].Data[eWa_OLPrize]);
		msg.WantNpcList[i].VolunteerNum = (BYTE)atoi((char*)pData[i].Data[eWa_OLVolunteerNum]);
	}

	pPlayer->SendMsg(&msg, sizeof(msg));
}

void RWantedMyList(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;

	SEND_WANTNPCLIST msg;
	memset(&msg, 0, sizeof(msg));
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_MYLIST_ACK;
	msg.TotalPage = (WORD)atoi((char*)pData[0].Data[eWa_OLTotal]);
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		msg.WantNpcList[i].WantedIDX = (WANTEDTYPE)atoi((char*)pData[i].Data[eWa_OLWantedIDX]);
		SafeStrCpy(msg.WantNpcList[i].WantedName, (char*)pData[i].Data[eWa_OLWantedChrName], MAX_NAME_LENGTH+1);
		msg.WantNpcList[i].Prize = (MONEYTYPE)atoi((char*)pData[i].Data[eWa_OLPrize]);
		msg.WantNpcList[i].VolunteerNum = (BYTE)atoi((char*)pData[i].Data[eWa_OLVolunteerNum]);
	}

	pPlayer->SendMsg(&msg, sizeof(msg));
}

void RWantedSearch(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;

	SEND_WANTNPCLIST msg;
	memset(&msg, 0, sizeof(msg));
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_SEARCH_ACK;
	msg.TotalPage = (WORD)atoi((char*)pData[0].Data[eWa_OLTotal]);
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		msg.WantNpcList[i].WantedIDX = (WANTEDTYPE)atoi((char*)pData[i].Data[eWa_OLWantedIDX]);
		SafeStrCpy(msg.WantNpcList[i].WantedName, (char*)pData[i].Data[eWa_OLWantedChrName], MAX_NAME_LENGTH+1);
		msg.WantNpcList[i].Prize = (MONEYTYPE)atoi((char*)pData[i].Data[eWa_OLPrize]);
		msg.WantNpcList[i].VolunteerNum = (BYTE)atoi((char*)pData[i].Data[eWa_OLVolunteerNum]);
	}

	pPlayer->SendMsg(&msg, sizeof(msg));
}

// Quest
void RQuestTotalInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	SEND_QUEST_TOTALINFO msg;
	memset(&msg.QuestList, 0, sizeof(msg.QuestList));

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	msg.Category = MP_QUEST;
	msg.Protocol = MP_QUEST_TOTALINFO;
	msg.dwObjectID = pMessage->dwID;

	if( pMessage->dwResult == 0 )
	{
		msg.wCount = 0;
		pPlayer->SendMsg( &msg, msg.GetSize() );
	}
	else
	{
		CQuestBase* pQuest;
		QFLAG flag;
		WORD QuestIdx;
		WORD count = 0;

		for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
		{			
			flag.value = (QSTATETYPE)atoi((char*)pData[i].Data[eQi_QuestState]);
			QuestIdx = (WORD)atoi((char*)pData[i].Data[eQi_QuestIdx]);

			if( !flag.IsSet(1) )
			{
				pQuest = new CQuestBase;
				pQuest->Init(QuestIdx, flag.value);

				pPlayer->AddQuest( pQuest );

				msg.QuestList[count].QuestIdx = QuestIdx;
				msg.QuestList[count].state = flag.value;
				count++;
			}
		}

		msg.wCount = count;
		pPlayer->SendMsg( &msg, msg.GetSize() );
	}

	//KES 퀘스트 쿼리 연계
	QuestMainQuestLoad( pMessage->dwID );
} 

void RQuestMainQuestLoad(LPQUERY pData, LPDBMESSAGE pMessage)
{
	SEND_MAINQUEST_DATA msg;
	memset(&msg.QuestList, 0, sizeof(msg.QuestList));
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	msg.Category = MP_QUEST;
	msg.Protocol = MP_QUEST_MAINDATA_LOAD;
	msg.dwObjectID = pMessage->dwID;

	if( pMessage->dwResult == 0 )
	{
		//msg.wCount = 0;
		//pPlayer->SendMsg( &msg, msg.GetSize() );

		//---KES 메인을 다 받고 서브를 연결해 받자.
		QuestSubQuestLoad( pMessage->dwID );
	}
	else
	{
		DWORD state;
		WORD QuestIdx=0;
		DWORD EndParam=0;
		DWORD Time=0;
		BYTE bCheckType = 0;
		DWORD dwCheckTime = 0;
		WORD count = 0;

		typedef std::map< DWORD, DWORD >	QuestMap ;
		QuestMap							questIdx ;

		for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
		{			
			QuestIdx = (WORD)atoi((char*)pData[i].Data[eQi_QuestIdx]);
			state = (QSTATETYPE)atoi((char*)pData[i].Data[eQi_QuestState]);
			EndParam = (DWORD)atoi((char*)pData[i].Data[eQi_EndParam]);
			Time = (DWORD)atoi((char*)pData[i].Data[eQi_RegistTime]);
			bCheckType = (BYTE)atoi((char*)pData[i].Data[eQi_CheckType]);
			dwCheckTime = (DWORD)atoi((char*)pData[i].Data[eQi_CheckTime]);

			++questIdx[QuestIdx] ;

			msg.QuestList[count].QuestIdx = QuestIdx;
			msg.QuestList[count].state.value = state;
			msg.QuestList[count].EndParam = EndParam;
			msg.QuestList[count].Time = Time;
			msg.QuestList[count].CheckType = bCheckType;
			msg.QuestList[count].CheckTime = dwCheckTime;
			++count;

			QUESTMGR->SetMainQuestData( pPlayer, QuestIdx, state, EndParam, Time, bCheckType, dwCheckTime );
		}
/*
		for( QuestMap::const_iterator it = questIdx.begin(); questIdx.end() != it; ++it )
		{
			const DWORD questIndex = it->first;
			const DWORD count = it->second;

			if(count > 1)
			{
				SYSTEMTIME time;
				char szFile[256] = {0,};
				GetLocalTime( &time );

				sprintf( szFile, "./Log/MAIN_QUEST_LOAD%02d_%4d%02d%02d.txt", g_pServerSystem->GetMapNum(), time.wYear, time.wMonth, time.wDay ) ;

				FILE* fp = fopen(szFile,"a+") ;

				fprintf(fp,"PlayerID : %d - QuestID : %d\n", pPlayer->GetID(), QuestIdx) ;

				fclose(fp) ;
			}
		}
*/
		msg.wCount = count;
		pPlayer->SendMsg( &msg, msg.GetSize() );		

		if( count >= 100 )
		{
			QuestMainQuestLoad( pMessage->dwID, (int)QuestIdx );
		}
		else
		{
			//---KES 메인을 다 받고 서브를 연결해 받자.
			QuestSubQuestLoad( pMessage->dwID );
		}
	}	
}


void RQuestSubQuestLoad(LPQUERY pData, LPDBMESSAGE pMessage)
{
	SEND_SUBQUEST_DATA msg;
	memset(&msg.QuestList, 0, sizeof(msg.QuestList));

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	// client￠?￠® ¨￢￠￢ⓒø¨u ¡¾￠￢vA¨u ¡iy¨u¨￢..
	msg.Category = MP_QUEST;
	msg.Protocol = MP_QUEST_SUBDATA_LOAD;
	msg.dwObjectID = pMessage->dwID;				// UserID

	if( pMessage->dwResult == 0 )
	{
		//msg.wCount = 0;
		//pPlayer->SendMsg( &msg, msg.GetSize() );
		QuestItemload( pMessage->dwID );
	}
	else
	{
		WORD QuestIdx;
		WORD SubQuestIdx;
		DWORD state;
		DWORD time;
		WORD count = 0;

		for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
		{			
			QuestIdx = (WORD)atoi((char*)pData[i].Data[eQsi_QuestIdx]);
			SubQuestIdx = (WORD)atoi((char*)pData[i].Data[eQsi_SubQuestIdx]);
			state = (DWORD)atoi((char*)pData[i].Data[eQsi_Data]);
			time = (DWORD)atoi((char*)pData[i].Data[eQsi_RegistTime]);

			// A¡þ￠OoAI¨uⓒ￡¨¡￠c ￠￢¨­¨u¨uAo ¨uA¨¡A..
			msg.QuestList[count].QuestIdx = QuestIdx;
			msg.QuestList[count].SubQuestIdx = SubQuestIdx;
			msg.QuestList[count].state = state;
			msg.QuestList[count].time = time;
			count++;

			QUESTMGR->SetSubQuestData( pPlayer, QuestIdx, SubQuestIdx, state, time );
		}

		msg.wCount = count;
		pPlayer->SendMsg( &msg, msg.GetSize() );

		if( count >= 100 )
		{
			QuestSubQuestLoad( pMessage->dwID, (int)QuestIdx );
		}
		else
		{
			QuestItemload( pMessage->dwID );
		}
	}	
}


void RQuestItemLoad(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	SEND_QUESTITEM msg;
	memset(&msg.ItemList, 0, sizeof(msg.ItemList));
	// client징횏?징횏짖챌 징짠징첸징횏징첸짢횕짤짧징짠u 짖짰짢첬징횏징첸vA징짠u 짖짰iy징짠u징짠징첸..
	msg.Category = MP_QUEST;
	msg.Protocol = MP_QUEST_ITEM_LOAD;
	msg.dwObjectID = pMessage->dwID;				// UserID

	if( pMessage->dwResult == 0 )
	{
		//msg.wCount = 0;
		//pPlayer->SendMsg( &msg, msg.GetSize() );
		TutorialLoad( pMessage->dwID );
	}
	else
	{
		DWORD ItemIdx=0;
		int Count=0;
		WORD wcount=0;
		WORD QuestIdx=0;

		for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
		{
			ItemIdx = (DWORD)atoi((char*)pData[i].Data[eQit_ItemIdx]);
			Count = (int)atoi((char*)pData[i].Data[eQit_ItemCount]);			
			QuestIdx = (WORD)atoi((char*)pData[i].Data[eQit_QuestIdx]);

			// A¡þ￠OoAI¨uⓒ￡¨¡￠c ￠￢¨­¨u¨uAo ¨uA¨¡A..
			msg.ItemList[wcount].ItemIdx = ItemIdx;
			msg.ItemList[wcount].Count = Count;
			msg.ItemList[wcount].QuestIdx = QuestIdx;
			wcount++;

			QUESTMGR->SetQuestItem( pPlayer, QuestIdx, ItemIdx, Count );
		}

		msg.wCount = wcount;
		pPlayer->SendMsg( &msg, msg.GetSize() );

		if( wcount >= 100 )
		{
			QuestItemload( pMessage->dwID, (int)QuestIdx );
		}
		else
		{
			TutorialLoad( pMessage->dwID );
		}
	}
}

void RJournalGetList(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;
	SEND_JOURNALLIST msg;
	memset(&msg, NULL, sizeof(msg));
	msg.Category = MP_JOURNAL;
	msg.Protocol = MP_JOURNAL_GETLIST_ACK;
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		msg.m_Info[i].Index = (DWORD)atoi((char*)pData[i].Data[eJnl_Index]);
		msg.m_Info[i].Kind = (BYTE)atoi((char*)pData[i].Data[eJnl_Kind]);
		msg.m_Info[i].Param = (DWORD)atoi((char*)pData[i].Data[eJnl_Param]);
		msg.m_Info[i].Param_2 = (DWORD)atoi((char*)pData[i].Data[eJnl_Param_2]);
		msg.m_Info[i].Param_3 = (DWORD)atoi((char*)pData[i].Data[eJnl_Param_3]);
		msg.m_Info[i].bSaved = (BYTE)atoi((char*)pData[i].Data[eJnl_bSaved]);
		SafeStrCpy(msg.m_Info[i].ParamName, (char*)pData[i].Data[eJnl_ParamName], MAX_NAME_LENGTH+1);
		SafeStrCpy(msg.m_Info[i].RegDate, (char*)pData[i].Data[eJnl_RegDate], 11);
		char buf[256]={ 0, };
		strcpy(buf, (char*)pData[i].Data[eJnl_RegDate]);
		msg.wCount++;
	}
	pPlayer->SendMsg(&msg, msg.GetSize());
}

void RSaveMapChangePointReturn(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD dwConnectionIndex = (DWORD)atoi((char*)pData->Data[0]);
	DWORD Map = (DWORD)atoi((char*)pData->Data[1]);

	MSG_DWORD msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_MAP_OUT;
	msg.dwObjectID = pMessage->dwID;
	msg.dwData = Map;

	g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
}


// 짝짰짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짱
// 짝짯RaMa		-> ShopItemLoading                             짝짯
// 짝짹짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝째
// 071126 KTH --- Redefine
void RCharacterShopItemInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	if(pPlayer->GetInited() == FALSE)
		return;
	if(pMessage->dwResult == 0)	
		return;

	// 횉철?챌쩍횄째짙 ?첬?책
	SHOP_ITEM	ShopItem;
//	SHOP_ITEM	NewShopItem;
	memset(&ShopItem, 0, sizeof(SHOP_ITEM));
//	memset(&NewShopItem, 0, sizeof(SHOP_ITEM));

//	int itempos=0;
//	DWORD nitemcnt=0;
	DWORD lditemcnt = 0;
	if( pMessage->dwResult<=SLOT_SHOPITEM_IMSI )
		lditemcnt = pMessage->dwResult;
	else
	{
		lditemcnt = SLOT_SHOPITEM_IMSI;
//		pPlayer->GetShopItemManager()->SendMsgDwordToPlayer( MP_ITEM_SHOPITEM_SLOTOVERITEM, pMessage->dwResult );
	}

	//
	//stTIME curtime, endtime;
	//curtime.value = GetCurTime();

/*	for( DWORD i = 0 ; i < lditemcnt ; ++i )
	{
		//itempos = atoi((char*)pData[i].Data[eMItm_Position]);

		if(itempos == 0)
		{
			NewShopItem.Item[nitemcnt].dwDBIdx = (DWORD)atoi((char*)pData[i].Data[eMItm_ItemDBIdx]);
			NewShopItem.Item[nitemcnt].wIconIdx = (WORD)atoi((char*)pData[i].Data[eMItm_ItemIdx]);
			NewShopItem.Item[nitemcnt].Position = 0;
			NewShopItem.Item[nitemcnt].Durability = (WORD)atoi((char*)pData[i].Data[eMItm_Durability]);
			NewShopItem.Item[nitemcnt].ItemParam = (ITEMPARAM)atoi((char*)pData[i].Data[eMItm_Param]);			
			++nitemcnt;
		}
		else
		{				
			itempos -= TP_SHOPITEM_START;

			if( ShopItem.Item[itempos].dwDBIdx != 0 )
			{
				NewShopItem.Item[nitemcnt].dwDBIdx = (DWORD)atoi((char*)pData[i].Data[eMItm_ItemDBIdx]);
				NewShopItem.Item[nitemcnt].wIconIdx = (WORD)atoi((char*)pData[i].Data[eMItm_ItemIdx]);
				NewShopItem.Item[nitemcnt].Position = 0;
				NewShopItem.Item[nitemcnt].Durability = (WORD)atoi((char*)pData[i].Data[eMItm_Durability]);
				NewShopItem.Item[nitemcnt].ItemParam = (ITEMPARAM)atoi((char*)pData[i].Data[eMItm_Param]);			
				++nitemcnt;

				continue;
			}

			ShopItem.Item[itempos].dwDBIdx = (DWORD)atoi((char*)pData[i].Data[eMItm_ItemDBIdx]);
			ShopItem.Item[itempos].wIconIdx = (WORD)atoi((char*)pData[i].Data[eMItm_ItemIdx]);
			ShopItem.Item[itempos].Position = (WORD)atoi((char*)pData[i].Data[eMItm_Position]);
			ShopItem.Item[itempos].Durability = (WORD)atoi((char*)pData[i].Data[eMItm_Durability]);
			ShopItem.Item[itempos].ItemParam = (ITEMPARAM)atoi((char*)pData[i].Data[eMItm_Param]);
		}


		// 사용하려면 프로시저에 Order by item_dbidx추가
		//StartItemDBIdx = ShopItem.Item[itempos].dwDBIdx;
	}*/


	// 포지션이 없는 새로운 아이템이 있으면.
//	if( nitemcnt )
//	{
//		for(DWORD ni=0; ni<nitemcnt; ni++)
//		{
			for(int si=0; si<SLOT_SHOPITEM_IMSI; si++)
			{
				// 자리가 없는것이 있으면 셋팅해준다.
				if( ShopItem.Item[si].dwDBIdx == 0 )
				{
					//ShopItem.Item[si] = NewShopItem.Item[ni];
					ShopItem.Item[si].Position = si+TP_SHOPITEM_START;
					// 071126 KTH
					ShopItem.Item[si].dwDBIdx = (DWORD)atoi((char*)pData[si].Data[eMItm_ItemDBIdx]);
					ShopItem.Item[si].wIconIdx = (DWORD)atoi((char*)pData[si].Data[eMItm_ItemIdx]);
					ShopItem.Item[si].Durability = (WORD)atoi((char*)pData[si].Data[eMItm_Durability]);
					ShopItem.Item[si].ItemParam = (ITEMPARAM)atoi((char*)pData[si].Data[eMItm_Param]);
					ShopItem.Item[si].nSealed = (ITEMPARAM)atoi((char*)pData[si].Data[eMItm_Seal]);
					
					ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(ShopItem.Item[si].wIconIdx);
					if( pItemInfo == NULL )
						continue;

					if( pItemInfo->nTimeKind == eKIND_REALTIME )
					{
						ShopItem.Item[si].nRemainSecond = (int)atoi((char*)pData[si].Data[eMItm_EndTime]) ;
					}
					else
						ShopItem.Item[si].nRemainSecond = (ITEMPARAM)atoi((char*)pData[si].Data[eMItm_RemainTime]);
					
					ITEMBASE& item = ShopItem.Item[ si ];
					
					// 081202 LUJ, 수량성 아이템의 개수가 0인 경우, 삭제한다
					if(		ITEMMGR->IsDupItem( item.wIconIdx ) &&
						!	item.Durability )
					{
						LogItemMoney(
							pPlayer->GetID(),
							pPlayer->GetObjectName(),
							0,
							"*invalid",
							eLog_ItemDiscard,
							pPlayer->GetMoney(),
							0,
							0,
							item.wIconIdx,
							item.dwDBIdx,
							item.Position,
							0,
							item.Durability,
							0 );
						ItemDeleteToDB( item.dwDBIdx );
						ZeroMemory( &item, sizeof( item ) );
					}
				}
			}
//		}
//	}

	pPlayer->InitShopItemInfo( &ShopItem );
//	pPlayer->GetShopItemManager()->CheckAvatarEndtime();
	pPlayer->SendShopItemInfo();
}


void RShopItemInvenInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;


	//
	ITEMBASE ItemBase[TABCELL_SHOPINVEN_NUM];
	ITEMBASE ItemOverlap[TABCELL_SHOPINVEN_NUM];
	memset( ItemBase, 0, sizeof(ITEMBASE)*TABCELL_SHOPINVEN_NUM );
	memset( ItemOverlap, 0, sizeof(ITEMBASE)*TABCELL_SHOPINVEN_NUM );
	int OverlapCount=0;


	if( pMessage->dwResult == 0 )
	{
//		pPlayer->InitShopInvenInfo( ItemBase );
		//pPlayer->SetInitState(PLAYERINITSTATE_ITEM_INFO,MP_USERCONN_GAMEIN_SYN);
		return;
	}

	for(DWORD  i = 0; i < pMessage->dwResult; i++)
	{
		POSTYPE ItemPos = atoi((char*)pData[i].Data[eCI_Position]);

		if(ItemPos >= TP_SHOPINVEN_START && ItemPos < TP_SHOPINVEN_END)
		{
			ItemPos -= TP_SHOPINVEN_START;

			if( ItemPos >= TABCELL_SHOPINVEN_NUM || ItemBase[ItemPos].dwDBIdx )
			{
				ItemOverlap[OverlapCount].dwDBIdx = (DWORD)atoi((char*)pData[i].Data[eCI_DBIDX]);
				ItemOverlap[OverlapCount].wIconIdx = (WORD)atoi((char*)pData[i].Data[eCI_IDX]);
				ItemOverlap[OverlapCount].Position = (POSTYPE)atoi((char*)pData[i].Data[eCI_Position]);
				ItemOverlap[OverlapCount].QuickPosition = (POSTYPE)atoi((char*)pData[i].Data[eCI_QPosition]);
				ItemOverlap[OverlapCount].Durability = (DURTYPE)atoi((char*)pData[i].Data[eCI_Durability]);
				ItemOverlap[OverlapCount].ItemParam = (ITEMPARAM)atoi((char*)pData[i].Data[eCI_Param]);
				++OverlapCount;
			}
			else
			{
				ItemBase[ItemPos].dwDBIdx = (DWORD)atoi((char*)pData[i].Data[eCI_DBIDX]);
				ItemBase[ItemPos].wIconIdx = (WORD)atoi((char*)pData[i].Data[eCI_IDX]);
				ItemBase[ItemPos].Position = (POSTYPE)atoi((char*)pData[i].Data[eCI_Position]);
				ItemBase[ItemPos].QuickPosition = (POSTYPE)atoi((char*)pData[i].Data[eCI_QPosition]);
				ItemBase[ItemPos].Durability = (DURTYPE)atoi((char*)pData[i].Data[eCI_Durability]);
				ItemBase[ItemPos].ItemParam = (ITEMPARAM)atoi((char*)pData[i].Data[eCI_Param]);
			}
		}
	}

	//중복된 아이템이 있으면 빈칸에 넣어준다.
	for(int n=0; n<OverlapCount; n++)
	{
		for(int m=0; m<TABCELL_SHOPINVEN_NUM; m++)
		{
			//빈칸이면 넣어준다.
			if( ItemBase[m].dwDBIdx == 0 )
			{
				ItemBase[m] = ItemOverlap[n];
				ItemBase[m].Position = m+TP_SHOPINVEN_START;

				// DB에 포지션을 업데이트해준다.
				ItemMoveUpdateToDB( pPlayer->GetID(), 0, 0, ItemBase[m].dwDBIdx, ItemBase[m].Position, pPlayer->GetUserID(), pPlayer->GetGuildIdx() );
				break;
			}
		}

		//만약 빈칸이 없으면 더이상 처리하지 않음
		if( m == TABCELL_SHOPINVEN_NUM )
		{
			// 아이템이 다 차있다.;
			break;
		}
	}

//	pPlayer->InitShopInvenInfo( ItemBase );
	//pPlayer->SetInitState(PLAYERINITSTATE_ITEM_INFO,MP_USERCONN_GAMEIN_SYN);
}

// 짝짰짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짱
// 짝짯RaMa - 04.11.05  ->ShopItemUsing                          짝짯
// 짝짹짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝째
void RUsingShopItemInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	if( pMessage->dwResult == 0 )
	{
		pPlayer->SetInitState( PLAYERINITSTATE_SHOPITEM_USEDINFO, MP_USERCONN_GAMEIN_SYN );
		return;
	}

	SEND_SHOPITEM_USEDINFO msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_USEDINFO;

	DWORD dwBeginTime = 0 ;
	for(DWORD i=0; i<pMessage->dwResult; i++)
	{
		msg.Item[msg.ItemCount].wIconIdx		= (WORD)atoi((char*)pData[i].Data[eUMtm_ItemIdx]);
		msg.Item[msg.ItemCount].dwDBIdx			= (DWORD)atoi((char*)pData[i].Data[eUMtm_DBIdx]);
		msg.Item[msg.ItemCount].ItemParam		= (DWORD)atoi((char*)pData[i].Data[eUMtm_Param]);
		dwBeginTime								= (DWORD)atoi((char*)pData[i].Data[eUMtm_BTime]) ;
		//msg.Item[msg.ItemCount].BeginTime.value		= (DWORD)atoi((char*)pData[i].Data[eUMtm_BTime]);
		//msg.Item[msg.ItemCount].dwRemainMinute			= (DWORD)atoi((char*)pData[i].Data[eUMtm_RTime]);
		//msg.Item[msg.ItemCount].nRemainSecond	= (int)atoi((char*)pData[i].Data[eUMtm_RTime]);
		msg.Item[msg.ItemCount].Position		= (POSTYPE)atoi((char*)pData[i].Data[eUMtm_Position]);

//		if( pPlayer->GetShopItemManager()->UsedShopItem(
//			&msg.Item[msg.ItemCount].ItemBase,
//			msg.Item[msg.ItemCount].Param,
//			msg.Item[msg.ItemCount].BeginTime,
//			msg.Item[msg.ItemCount].Remaintime) )
//		{
			// 장비하고 있는 아바타 아이템
//			if( msg.Item[msg.ItemCount].Param == eShopItemUseParam_EquipAvatar )
//				pPlayer->GetShopItemManager()->PutOnAvatarItem( 
//				msg.Item[msg.ItemCount].ItemBase.wIconIdx, 0, FALSE );

//			++msg.ItemCount;
//		}
//		else
		{
			memset( &msg.Item[msg.ItemCount], 0, sizeof(SHOPITEMBASE) );	//클라이언트로 메세지 갈필요가 없다.
		}

		if( msg.ItemCount >= 100 )
		{
			g_Console.LOG( 4, "UsingShopItemInfo over 100 !!" );
			break;
		}

		//		++msg.ItemCount;	//if문 안으로 옮김 KES
	}

//	pPlayer->GetShopItemManager()->CalcPlusTime( 0, 0 );

	//
	pPlayer->SetInitState( PLAYERINITSTATE_SHOPITEM_USEDINFO, MP_USERCONN_GAMEIN_SYN );

	// 
	pPlayer->SendMsg(&msg, msg.GetSize());
}


void RSavedMovePointInfo( LPMIDDLEQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

//	pPlayer->GetShopItemManager()->ReleseMovePoint();

	SEND_MOVEDATA_INFO msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_MPINFO;

	for(DWORD i=0; i<pMessage->dwResult; i++)
	{
		msg.Data[i].DBIdx = (DWORD)atoi((char*)pData[i].Data[eMPt_DBIdx]);
		strncpy(msg.Data[i].Name, (char*)pData[i].Data[eMPt_Name], MAX_SAVEDMOVE_NAME-1);
		msg.Data[i].MapNum = (WORD)atoi((char*)pData[i].Data[eMPt_MapNum]);
		msg.Data[i].Point.value = (DWORD)atoi((char*)pData[i].Data[eMPt_MapPoint]);
		++msg.Count;

		//
//		pPlayer->GetShopItemManager()->AddMovePoint( &msg.Data[i] );
	}

	// 맵에 들어오면 한번 보내준다.
	if( pPlayer->GetSavePointInit() )
	{
		msg.bInited = 1;
		pPlayer->SendMsg(&msg, msg.GetSize());
	}
	else
	{
		msg.bInited = 0;
		pPlayer->SendMsg(&msg, msg.GetSize());
		pPlayer->SetSavePointInit( TRUE );
	}
}

void RSavedMovePointInsert( LPMIDDLEQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	if( pMessage->dwResult == 0 )	
		return;

	SEND_MOVEDATA_SIMPLE msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_SAVEPOINT_ADD_ACK;

	for(DWORD i=0; i<pMessage->dwResult; i++)
	{
		msg.Data.DBIdx = (DWORD)atoi((char*)pData[i].Data[eMPt_DBIdx]);
		strncpy(msg.Data.Name, (char*)pData[i].Data[eMPt_Name], MAX_SAVEDMOVE_NAME-1);
		msg.Data.MapNum = (WORD)atoi((char*)pData[i].Data[eMPt_MapNum]);
		msg.Data.Point.value = (DWORD)atoi((char*)pData[i].Data[eMPt_MapPoint]);

		//
//		pPlayer->GetShopItemManager()->AddMovePoint( &msg.Data );
	}

	//
	pPlayer->SendMsg(&msg, sizeof(msg));

}


void RGuildLoadGuild(LPQUERY pData, LPDBMESSAGE pMessage)
{
	//LOAD_GUILD LoadGuildInfo;
	//DWORD dwTime = 0;
	//memset(&LoadGuildInfo, 0, sizeof(LOAD_GUILD));

	for( DWORD i = 0; i<pMessage->dwResult; ++i )
	{
		const QUERYST& record = pData[ i];

		GUILDINFO info = { 0 };

		info.GuildIdx	= atoi( ( char* )record.Data[ 0 ] );
		info.MasterIdx	= atoi( ( char* )record.Data[ 1 ] );
		SafeStrCpy( info.MasterName, ( char* )record.Data[ 2 ], sizeof( info.MasterName ) );
		SafeStrCpy( info.GuildName, ( char* )record.Data[ 3 ], sizeof( info.GuildName ) );
		info.GuildLevel	= atoi( ( char* )record.Data[ 4 ] );
		info.MapNum		= atoi( ( char* )record.Data[ 5 ] );
		info.UnionIdx	= atoi( ( char* )record.Data[ 6 ] );

		const MONEYTYPE	money = atoi( ( char* )record.Data[ 7 ] );

		info.MarkName	= atoi( ( char* )record.Data[ 8 ] );

		const DWORD time = atoi( ( char* )record.Data[ 9 ] );

		GUILDMGR->RegistGuild( info, money );

		if( time )
		{
			GUILDUNIONMGR->AddEntryTime( info.GuildIdx, time );
		}
	}

	ASSERT(pMessage->dwResult <= MAX_QUERY_RESULT);
	if(pMessage->dwResult >= MAX_QUERY_RESULT)
	{
		const DWORD lastGuildIndex = atoi( ( char* )pData[ MAX_QUERY_RESULT - 1 ].Data[ 0 ] );

		GuildLoadGuild( lastGuildIndex );
	}
	else
	{
		//GuildLoadNotice(0);
		GuildLoadMember(0);
		// RaMa - Guild Tournament - Guild정보 읽어온 후에 읽는다.
		GuildTournamentInfoLoad();
		// RaMa - SiegeWar 추가
		if( SIEGEWARMGR->IsNeedLoadSiegeInfo() )		
			SiegeWarInfoLoad( SIEGEWARMGR->GetSiegeMapNum() );
	}
}

//// 06. 03. 문파공지 - 이영준
//void RGuildLoadNotice(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
//{
//	LOAD_GUILDNOTICE GuildNotice;
//	memset(&GuildNotice, 0, sizeof(LOAD_GUILDNOTICE));
//	for(DWORD i=0; i<pMessage->dwResult; ++i)
//	{
//		GuildNotice.DBIdx[i] = atoi((char*)pData[i].Data[eGu_GNDBIdx]);
//		GuildNotice.GuildIdx[i] = atoi((char*)pData[i].Data[eGu_GNGuildIdx]);
//		strcpy(GuildNotice.GuildNotice[i], (char*)pData[i].Data[eGu_GNGuildNotice]);
//
//		GUILDMGR->RegistGuildNotice(GuildNotice.GuildIdx[i], GuildNotice.GuildNotice[i]);
//	}
//
//	//	ASSERT(pMessage->dwResult <= MAX_QUERY_RESULT);
//	//if(pMessage->dwResult == MAX_QUERY_RESULT)
//	//{
//	//	GuildLoadNotice(GuildNotice.DBIdx[i]);
//	//}
//}

void RGuildLoadMember(LPQUERY pData, LPDBMESSAGE pMessage)
{
	GUILDMEMBERINFO_ID member;
	
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		ZeroMemory( &member, sizeof( member ) );

		QUERYST&			query	= pData[ i ];
		GUILDMEMBERINFO&	info	= member.MemberInfo;

		info.MemberIdx	= atoi( ( char* )query.Data[ 0 ] );
		SafeStrCpy( info.MemberName, (char* )query.Data[ 1 ], sizeof( info.MemberName ) );
		
		info.Memberlvl	= atoi( ( char* )query.Data[ 2 ] );
		member.GuildIdx = atoi( ( char* )query.Data[ 3 ] );
		info.Rank		= atoi( ( char* )query.Data[ 4 ] );

		// 080225 LUJ, 쿼리 정보 추가
		info.mJobGrade	= atoi( ( char* )query.Data[ 5 ] );
		info.mJob[ 0 ]	= atoi( ( char* )query.Data[ 6 ] );
		info.mJob[ 1 ]	= atoi( ( char* )query.Data[ 7 ] );
		info.mJob[ 2 ]	= atoi( ( char* )query.Data[ 8 ] );
		info.mJob[ 3 ]	= atoi( ( char* )query.Data[ 9 ] );
		info.mJob[ 4 ]	= atoi( ( char* )query.Data[ 10 ] );
		info.mJob[ 5 ]	= atoi( ( char* )query.Data[ 11 ] );
		info.mRace		= atoi( ( char* )query.Data[ 12 ] );

		GUILDMGR->LoadMembers(&member) ;
	}

	if( MAX_ROW_NUM == pMessage->dwResult )
	{
		GuildLoadMember( member.MemberInfo.MemberIdx );
	}
	else
	{
		GuildLoadMark(0);
	}	
}

void RGuildLoadMark(LPLARGEQUERY pData, LPDBMESSAGE pMessage)
{
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		MARKNAMETYPE	MarkName	= atoi( ( char* )pData->Data[ 0 ] );
		char*			pImgData	=		( char* )pData->Data[ 1 ];

		GUILDMGR->LoadMark( MarkName, pImgData );
	}
	if(pMessage->dwResult)
	{
		GuildLoadMark(atoi((char*)pData->Data[0]));
	}
	else
	{
		// server start!
		//	g_pServerSystem->SetStart(TRUE);

		//SW060526 문파창고 아이템 정보저장 시점 변경
		//GuildItemLoad(GAMERESRCMNGR->GetLoadMapNum(), 0);	//기존 프로시져는 둔다. //GuildItemOption(.. //GuildItemRareOption(..
		LoadGuildWarRecord( 0 );
	}
}


void RGuildMarkRegist(LPLARGEQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer == 0) return;
	//markidx, guildidx, imgdate

	GUILDMGR->RegistMarkResult(pPlayer, atoi((char*)pData->Data[1]), atoi((char*)pData->Data[0]), (char*)pData->Data[2]);
}

/*
void UpdateCharacterMoney( DWORD dwCharIdx, MONEYTYPE money, BYTE flag )
{
	sprintf(txt, "EXEC RP_UpdateCharacterMoney %d, %d, %d", dwCharIdx, money, flag );
	g_DB.Query(eQueryType_FreeQuery, eUpdateCharacterMoney, 0, txt);
}

void RUpdateCharacterMoney( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD CharacterIdx = atoi((char*)pData[0].Data[0]);
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(CharacterIdx);
	if( pPlayer == NULL ) return;
	pPlayer->RSetMoney( (MONEYTYPE)atoi((char*)pData[0].Data[1]), (BYTE)atoi((char*)pData[0].Data[2]) );
}
*/
void ConfirmUserOut( DWORD dwConnectIdx, DWORD dwUserIdx, DWORD dwCharIdx, DWORD dwKind )
{
	sprintf(txt, "EXEC RP_ConfirmUserOut %d, %d, %d", dwUserIdx, dwCharIdx, dwKind );
	g_DB.Query(eQueryType_FreeQuery, eConfirmUserOut, dwConnectIdx, txt, dwCharIdx);
}

void RConfirmUserOut( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD dwConnectIdx = pMessage->dwID;
	DWORD dwUserIdx = (DWORD)atoi((char*)pData[0].Data[0]);
	DWORD dwCharIdx = (DWORD)atoi((char*)pData[0].Data[1]);
	DWORD dwKind = (DWORD)atoi((char*)pData[0].Data[2]);

	switch( dwKind )
	{
	case 1:		// 캐릭선택창
		{
			MSGBASE msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_BACKTOCHARSEL_ACK;
			msg.dwObjectID = dwCharIdx;

			g_Network.Send2Server(dwConnectIdx, (char*)&msg, sizeof(msg));
		}
		break;
	case 2:
		{
			MSGBASE msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_LOGINCHECK_DELETE;
			msg.dwObjectID = dwUserIdx;

			g_Network.Send2Server(dwConnectIdx, (char*)&msg, sizeof(msg));
		}
		break;
	}	
}

void RSaveCharInfoBeforeLogout( LPQUERY pData, LPDBMESSAGE pMessage )
{
	// 맵아웃처리
	CPlayer * pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer == 0)
		return;
	g_pServerSystem->RemovePlayer(pPlayer->GetID());
	//
	ConfirmUserOut( (DWORD)atoi((char*)pData->Data[0]), pPlayer->GetUserID(), pPlayer->GetID(), 1 );
}

void RGuildBreakUp( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	GUILDMGR->BreakUpGuildResult(pMaster, atoi((char*)pData->Data[0]), (char*)pData->Data[1]);

	DeleteGuildWarRecord( (DWORD)atoi((char*)pData->Data[0]) );
}


void RGuildDeleteMember( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if(atoi((char*)pData->Data[0]) == 0) //err
	{
		CPlayer * pMaster = (CPlayer*)g_pUserTable->FindUser( atoi((char*)pData->Data[1]) );
		if(pMaster)
			GUILDMGR->SendNackMsg(pMaster, MP_GUILD_DELETEMEMBER_NACK, eGuildErr_Err);
	}
	else
		GUILDMGR->DeleteMemberResult(pMessage->dwID, atoi((char*)pData->Data[0]), (char*)pData->Data[1]);
}

void RGuildSecedeMember( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if(atoi((char*)pData->Data[0]) == 0) //err
	{
		CPlayer * pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[1]));
		if(pPlayer)
			GUILDMGR->SendNackMsg(pPlayer, MP_GUILD_SECEDE_NACK, eGuildErr_Err);
	}
	else
		GUILDMGR->SecedeResult(pMessage->dwID, atoi((char*)pData->Data[0]), (char*)pData->Data[1]);
}


void LoadGuildWarRecord( DWORD dwIdx )
{
	sprintf(txt, "EXEC dbo.MP_GUILDWARRECORD_Load %d", dwIdx );
	g_DB.Query(eQueryType_FreeQuery, eLoadGuildWarRecord, 0, txt);
}

void RLoadGuildWarRecord( LPQUERY pData, LPDBMESSAGE pMessage )
{
	int nStartIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		nStartIdx = atoi((char*)pData[i].Data[0]);
		DWORD dwGuildIdx = (DWORD)atoi((char*)pData[i].Data[1]);
		DWORD dwGFWVictory = (DWORD)atoi((char*)pData[i].Data[2]);
		DWORD dwGFWDraw = (DWORD)atoi((char*)pData[i].Data[3]);
		DWORD dwGFWLoose = (DWORD)atoi((char*)pData[i].Data[4]);

		GUILDWARMGR->RegistGuildWarFromRecord( dwGuildIdx, dwGFWVictory, dwGFWDraw, dwGFWLoose );
	}

	if( pMessage->dwResult >= MAX_QUERY_RESULT )
	{
		LoadGuildWarRecord( nStartIdx );
	}	
	else
	{
		LoadGuildWar( 0 );
	}
}

void DeleteGuildWarRecord( DWORD dwGuildIdx )
{
	sprintf(txt, "EXEC dbo.MP_GUILDWARRECORD_Delete %d", dwGuildIdx );
	g_DB.Query(eQueryType_FreeQuery, eDeleteGuildWarRecord, 0, txt);
}

void LoadGuildWar( DWORD dwIdx )
{
	sprintf(txt, "EXEC dbo.MP_GUILDFIELDWAR_Load %d", dwIdx );
	g_DB.Query(eQueryType_FreeQuery, eLoadGuildWar, 0, txt);
}

void RLoadGuildWar( LPQUERY pData, LPDBMESSAGE pMessage )
{
	int nStartIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		nStartIdx = atoi((char*)pData[i].Data[0]);
		DWORD dwGuildIdx1 = (DWORD)atoi((char*)pData[i].Data[1]);
		DWORD dwGuildIdx2 = (DWORD)atoi((char*)pData[i].Data[2]);
		DWORD dwMoney = (DWORD)atoi((char*)pData[i].Data[3]);

		GUILDWARMGR->RegistGuildWar( dwGuildIdx1, dwGuildIdx2, dwMoney );
	}

	if( pMessage->dwResult >= MAX_QUERY_RESULT )
	{
		LoadGuildWar( nStartIdx );
	}	
	else
	{
		//		g_pServerSystem->SetStart( TRUE );
		GuildUnionLoad( 0 );
	}
}

void InsertGuildWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2, DWORD dwMoney )
{
	sprintf(txt, "EXEC dbo.MP_GUILDFIELDWAR_Insert %d, %d, %u", dwGuildIdx1, dwGuildIdx2, dwMoney );
	g_DB.Query(eQueryType_FreeQuery, eInsertGuildWar, 0, txt);
}

void DeleteGuildWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2 )
{
	sprintf(txt, "EXEC dbo.MP_GUILDFIELDWAR_Delete %d, %d", dwGuildIdx1, dwGuildIdx2 );
	g_DB.Query(eQueryType_FreeQuery, eDeleteGuildWar, 0, txt);
}

void UpdateGuildWarRecordToDB( DWORD dwGuildIdx, DWORD dwVitory, DWORD dwDraw, DWORD dwLoose )
{
	sprintf(txt, "EXEC dbo.MP_GUILDFIELDWAR_Record %d, %d, %d, %d", dwGuildIdx, dwVitory, dwDraw, dwLoose );
	g_DB.Query(eQueryType_FreeQuery, eUpdateGuildWarRecord, 0, txt);
}


void CharacterChangeName( DWORD CharacterIdx, char* Name, DWORD DBIdx )
{
	sprintf(txt, "EXEC %s %d, \'%s\', %d", STORED_SHOPITEM_CHANGENAME, CharacterIdx, Name, DBIdx );
	g_DB.Query(eQueryType_FreeQuery, eItemShopChangeName, CharacterIdx, txt, CharacterIdx);
}


void RCharacterChangeName( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer * pPlayer = (CPlayer*)g_pUserTable->FindUser( pMessage->dwID );
	if(!pPlayer)		return;

	DWORD Result = (DWORD)atoi((char*)pData->Data[0]);
	DWORD DBIdx = 0;


	MSG_DWORD msg;
	msg.Category = MP_ITEM;

	switch( Result )
	{
	case 0:		// 성공
		{
			DBIdx = (DWORD)atoi((char*)pData->Data[1]);
			ITEMBASE* pItemBase = NULL;

			// Item Position 확인
			for(int i=0; i<SLOT_SHOPINVEN_NUM/2; i++)
			{
				pItemBase = (ITEMBASE*)ITEMMGR->GetItemInfoAbsIn( pPlayer, i+TP_SHOPINVEN_START );
				if( !pItemBase )	continue;

				if( pItemBase->dwDBIdx == DBIdx )
				{
					if( EI_TRUE != ITEMMGR->DiscardItem( pPlayer, i+TP_SHOPINVEN_START, pItemBase->wIconIdx, 1 ) )
					{
						char buf[64];
						sprintf(buf, "ChangeName OK, DiscardItem Fail : %d", pMessage->dwID);
						ASSERTMSG(0, buf);
					}

					break;
				}
			}

			if( i == SLOT_SHOPINVEN_NUM/2)
			{
				char buf[64];
				sprintf(buf, "ChangeName OK, DeleteItem Fail: %d", pMessage->dwID);
				ASSERTMSG(0, buf);
			}

			DWORD time = GetCurTime();
			UsingShopItemUpdateToDB( 0, pPlayer->GetID(), pItemBase->wIconIdx, pItemBase->dwDBIdx, 0, time, 0 );
//			pPlayer->GetShopItemManager()->DeleteUsingShopItem( pItemBase->wIconIdx );

			SHOPITEMBASE ShopItemBase;
			ShopItemBase.BeginTime.value = time;
			ShopItemBase.Param = 0;
			memcpy( &ShopItemBase.ItemBase, pItemBase, sizeof(ITEMBASE) );
//			pPlayer->GetShopItemManager()->AddShopItemUse( &ShopItemBase );

			msg.Protocol = MP_ITEM_SHOPITEM_NCHANGE_ACK;
			msg.dwData = Result;			
		}
		break;
	default:
		msg.Protocol = MP_ITEM_SHOPITEM_NCHANGE_NACK;
		msg.dwData = Result;
		break;
	}

	pPlayer->SendMsg( &msg, sizeof(msg) );	
}

void CharacterChangeInfoToDB( DWORD CharacterIdx, DWORD Gender, DWORD HairType, DWORD FaceType, float Height, float Width )
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %f, %f", STORED_SHOPITEM_CHARCHANGE, CharacterIdx, Gender, HairType, FaceType, Height, Width );
	g_DB.Query(eQueryType_FreeQuery, eItemCharChange, 0, txt, CharacterIdx);
}


// Guild Tournament
void GuildTournamentInfoLoad()
{
	sprintf(txt, "EXEC %s", STORED_GT_LOADALL );
	g_DB.Query(eQueryType_FreeQuery, eGuildTournament_LoadAll, 0, txt);
}

void GuildTournamentCancelIdxLoad( DWORD TournamentIdx )
{
	sprintf(txt, "EXEC %s %d", STORED_GT_CANCELLOAD, TournamentIdx );
	g_DB.Query(eQueryType_FreeQuery, eGuildTournament_CancelLoad, 0, txt);
}

void GuildTournamentCancelIdxInsert( DWORD TournamentIdx, DWORD GuildIdx )
{
	sprintf(txt, "EXEC %s %d, %d", STORED_GT_CANCELINSERT, TournamentIdx, GuildIdx );
	g_DB.Query(eQueryType_FreeQuery, eGuildTournament_CancelInsert, 0, txt);
}

void GuildTournamentGuildInfoInsert( DWORD GuildIdx, char* GuildName, DWORD Position  )
{
	sprintf(txt, "EXEC %s %d, \'%s\', %d", STORED_GT_GUILDINFO_INSERT, GuildIdx, GuildName, Position );
	g_DB.Query(eQueryType_FreeQuery, eGuildTournament_Insert, 0, txt);
}

void GuildTournamentGuildInfoDelete( DWORD GuildIdx )
{
	sprintf(txt, "EXEC %s %d", STORED_GT_GUILDINFO_DELETE, GuildIdx );
	g_DB.Query(eQueryType_FreeQuery, eGuildTournament_Delete, 0, txt);
}

void GuildTournamentGuildInfoUpdate( DWORD GuildIdx, DWORD Ranking, DWORD ProcessTournament )
{
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_GT_GUILDINFO_UPDATE, GuildIdx, Ranking, ProcessTournament );
	g_DB.Query(eQueryType_FreeQuery, eGuildTournament_UpdateGuildInfo, 0, txt);
}

void GuildTournamentInfoUpdate( DWORD TournamentIdx, DWORD TournamentFight, DWORD WaitTime )
{
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_GT_TOURNAMENTINFO_UPDATE, TournamentIdx, TournamentFight, WaitTime );
	g_DB.Query(eQueryType_FreeQuery, eGuildTournament_UpdateTournamentInfo, 0, txt);
}

void RGuildTournamentInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD TournamentIdx = 0;
	DWORD CurTournament = 0;
	DWORD WaitTime = 0;
	GTDBLoadInfo GuildInfo[MAXGUILD_INTOURNAMENT];
	memset( GuildInfo, 0, sizeof(GTDBLoadInfo)*MAXGUILD_INTOURNAMENT );

	int count = 0;
	for( DWORD i=0; i<pMessage->dwResult; ++i )
	{
		GuildInfo[count].GuildIdx = (DWORD)atoi((char*)pData[i].Data[eGT_GuildIdx]);
		if( GuildInfo[count].GuildIdx == 0 )
		{
			TournamentIdx = (DWORD)atoi((char*)pData[i].Data[eGT_Position]);
			CurTournament = (BYTE)atoi((char*)pData[i].Data[eGT_Process]);
			WaitTime = (DWORD)atoi((char*)pData[i].Data[eGT_WaitTime]);
			continue;
		}
		GuildInfo[count].Position = (BYTE)atoi((char*)pData[i].Data[eGT_Position]);
		GuildInfo[count].Ranking = (BYTE)atoi((char*)pData[i].Data[eGT_Ranking]);
		GuildInfo[count].ProcessTournament= (BYTE)atoi((char*)pData[i].Data[eGT_Process]);
		++count;
	}

	if( TournamentIdx == 0 )			return;

	GTMGR->SetTournamentInfo( TournamentIdx, CurTournament, WaitTime );
	GTMGR->SetGuildInfo( GuildInfo, pMessage->dwResult );
}

void RGuildTournamentCancelIdxLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	for( DWORD i=0; i<pMessage->dwResult; ++i )
	{
		DWORD GuildIdx = (DWORD)atoi((char*)pData[i].Data[eGT_GuildIdx]);
		GTMGR->AddCancelGuild( GuildIdx );
	}
}


void GuildTournamentAddLog( DWORD TournamentIdx, DWORD GuildIdx, DWORD Logkind, DWORD Ranking )
{
	sprintf( txt, "EXEC dbo.Up_GT_AddLog %d, %d, %d, %d", TournamentIdx, GuildIdx, Logkind, Ranking );
	g_DB.LogQuery( eQueryType_FreeQuery, eGuildTournamentAddLog, 0, txt );
}

// jackpot
/*
void JackpotLoadTotalMoney()
{
	sprintf( txt, "EXEC dbo.MP_JACKPOT_LoadMoney" );
	g_DB.Query( eQueryType_FreeQuery, eJackpot_TotalMoney_Load, 0, txt );
}

void RJackpotLoadTotalMoney( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( pMessage->dwResult == 0 ) return;

	DWORD TotalMoney = (DWORD)atoi((char*)pData->Data[0]);
	JACKPOTMGR->SetTotalMoney( TotalMoney );
}*/

void JackpotAddTotalMoney( MONEYTYPE MapMoney )
{
	sprintf( txt, "EXEC dbo.MP_JACKPOT_AddTotalMoney %u", MapMoney );
	g_DB.Query( eQueryType_FreeQuery, eJackpot_TotalMoney_Update, 0, txt );
}
/*
void RJackpotAddTotalMoney( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( pMessage->dwResult == 0 ) return;

	DWORD TotalMoney = (DWORD)atoi((char*)pData->Data[0]);
	JACKPOTMGR->SendMsgMapUserTotalMoney( TotalMoney );
}*/

void JackpotPrizeInfo( MONEYTYPE MapMoney, DWORD PlayerID, DWORD PrizeKind, DWORD PrizePercentage, DWORD dwAbsMoney)
{
	sprintf( txt, "EXEC dbo.MP_JACKPOT_Prize %u, %d, %d, %d, %u", MapMoney, PlayerID, PrizeKind, PrizePercentage, dwAbsMoney );
	g_DB.Query( eQueryType_FreeQuery, eJackpot_PrizeInfo_Update, PlayerID, txt );
}

void RJackpotPrizeInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( pMessage->dwResult == 0 ) return;

	DWORD TotalMoney = (DWORD)atoi((char*)pData->Data[0]);
	DWORD PrizeMoney = (DWORD)atoi((char*)pData->Data[1]);	
	DWORD PrizeKind = (DWORD)atoi((char*)pData->Data[2]);
	//	CPlayer * pPlayer = (CPlayer*)g_pUserTable->FindUser( pMessage->dwID );
	//	if(!pPlayer)		return;
	DWORD PlayerID = pMessage->dwID;

	JACKPOTMGR->SetPrizeInfo( TotalMoney, PrizeMoney, PlayerID, PrizeKind ); //두가지 일을한다. 해당 ID로 PrizeMoney등록과 전서버 공지
}

// guildunion
void GuildUnionLoad( DWORD dwStartGuildUnionIdx )
{
	sprintf( txt, "EXEC dbo.MP_GUILD_UNION_Load %d", dwStartGuildUnionIdx );
	g_DB.Query( eQueryType_FreeQuery, eGuildUnionLoad, 0, txt );
}

void RGuildUnionLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD dwStartGuildIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		dwStartGuildIdx = (DWORD)atoi((char*)pData[i].Data[0]);
		GUILDUNIONMGR->LoadGuildUnion( (DWORD)atoi((char*)pData[i].Data[0])		// guildidx
			, (char*)pData[i].Data[1]					// name
			, (DWORD)atoi((char*)pData[i].Data[2])		// 0
				, (DWORD)atoi((char*)pData[i].Data[3])		// 1
				, (DWORD)atoi((char*)pData[i].Data[4])		// 2
				, (DWORD)atoi((char*)pData[i].Data[5])		// 3
				, (DWORD)atoi((char*)pData[i].Data[6])		// 4
				, (DWORD)atoi((char*)pData[i].Data[7])		// 5
				, (DWORD)atoi((char*)pData[i].Data[8])		// 6
				, (DWORD)atoi((char*)pData[i].Data[9]) );	// mark
	}
	if( pMessage->dwResult >= MAX_QUERY_RESULT )
	{
		GuildUnionLoad( dwStartGuildIdx );
	}	
	else
	{
		GuildUnionLoadMark( 0 );
	}	
}

void GuildUnionLoadMark( DWORD dwMarkIdx )
{
	sprintf( txt, "EXEC dbo.MP_GUILD_UNION_LoadMark %d", dwMarkIdx );
	g_DB.FreeLargeQuery( RGuildUnionLoadMark, 0, txt );
}

void RGuildUnionLoadMark( LPLARGEQUERY pData, LPDBMESSAGE pMessage )
{
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		GUILDUNIONMGR->LoadGuildUnionMark( (DWORD)atoi((char*)pData->Data[0]), (DWORD)atoi((char*)pData->Data[1]), (char*)pData->Data[2] );
	}
	if( pMessage->dwResult )
		GuildUnionLoadMark( (DWORD)atoi((char*)pData->Data[0]) );
	else
	{
		//		g_pServerSystem->SetStart( TRUE );
		SiegeWarProfitInfoLoad();
	}
}

void GuildUnionCreate( DWORD dwCharacterIdx, DWORD dwGuildIdx, char* sUnionName )
{
	sprintf( txt, "EXEC dbo.MP_GUILD_UNION_Create %d, %s", dwGuildIdx, sUnionName );
	g_DB.Query( eQueryType_FreeQuery, eGuildUnionCreate, dwCharacterIdx, txt, dwCharacterIdx );
}

void RGuildUnionCreate( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( !pMessage->dwResult )	return;
	if( atoi((char*)pData->Data[0]) == 0 )
	{
		CPlayer* pPlayer = ( CPlayer* )g_pUserTable->FindUser( pMessage->dwID );
		if( pPlayer )
			GUILDUNIONMGR->SendNackMsg( pPlayer, MP_GUILD_UNION_CREATE_NACK, eGU_Not_ValidName );
		return;
	}
	else
		GUILDUNIONMGR->GuildUnionCreateResult( (DWORD)atoi((char*)pData->Data[0]), (char*)pData->Data[1], (DWORD)atoi((char*)pData->Data[2]) );
}

void GuildUnionDestroy( DWORD dwGuildUnionIdx, DWORD dwMarkIdx, DWORD dwMasterIdx, DWORD dwTime )
{
	sprintf( txt, "EXEC dbo.MP_GUILD_UNION_Destroy %d, %d, %d, %d", dwGuildUnionIdx, dwMarkIdx, dwMasterIdx, dwTime );
	g_DB.Query( eQueryType_FreeQuery, eGuildUnionDestroy, 0, txt, dwMasterIdx );
}

void GuildUnionAddGuild( DWORD dwGuildUnionIdx, int nIndex, DWORD dwGuildIdx )
{
	sprintf( txt, "EXEC dbo.MP_GUILD_UNION_AddGuild %d, %d, %d", dwGuildUnionIdx, nIndex, dwGuildIdx );
	g_DB.Query( eQueryType_FreeQuery, eGuildUnionAddGuild, 0, txt );
}

void GuildUnionRemoveGuild( DWORD dwGuildUnionIdx, int nIndex, DWORD dwGuildIdx )
{
	sprintf( txt, "EXEC dbo.MP_GUILD_UNION_RemoveGuild %d, %d, %d", dwGuildUnionIdx, nIndex, dwGuildIdx );
	g_DB.Query( eQueryType_FreeQuery, eGuildUnionRemoveGuild, 0, txt );
}

void GuildUnionSecedeGuild( DWORD dwGuildUnionIdx, int nIndex, DWORD dwGuildIdx, DWORD dwTime )
{
	sprintf( txt, "EXEC dbo.MP_GUILD_UNION_SecedeGuild %d, %d, %d, %d", dwGuildUnionIdx, nIndex, dwGuildIdx, dwTime );
	g_DB.Query( eQueryType_FreeQuery, eGuildUnionSecedeGuild, 0, txt );
}

void GuildUnionMarkRegist( DWORD dwCharacterIdx, DWORD dwMarkIdx, DWORD dwGuildUnionIdx, char* pImgData )
{
	char buf[2048];
	sprintf( buf, "EXEC dbo.MP_GUILD_UNION_MarkRegist %d, %d, %d, 0x", dwCharacterIdx, dwMarkIdx, dwGuildUnionIdx );

	int curpos = strlen(buf);
	for( int n = 0; n < GUILDUNIONMARK_BUFSIZE; ++n )
	{
		sprintf( &buf[curpos], "%02x", (BYTE)pImgData[n] );
		curpos += 2;
	}

	g_DB.FreeLargeQuery( RGuildUnionMarkRegist, 0, buf, dwCharacterIdx );
}

void RGuildUnionMarkRegist( LPLARGEQUERY pData, LPDBMESSAGE pMessage )
{
	if( !pMessage->dwResult )	return;
	GUILDUNIONMGR->GuildUnionMarkRegistResult( (DWORD)atoi((char*)pData->Data[0]), (DWORD)atoi((char*)pData->Data[1]),
		(DWORD)atoi((char*)pData->Data[2]), (char*)pData->Data[3] );
}

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//┃ SiegeWar													  ┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
void SiegeWarInfoLoad( DWORD MapNum )
{
	sprintf( txt, "EXEC %s %d", STORED_SIEGE_INFOLOAD, MapNum );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarInfoLoad, 0, txt );
}

void RSiegeWarInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD SiegeWarIdx = 0;
	DWORD TaxRate = 0;
	DWORD RegistTime = 0;
	DWORD SiegeWarTime = 0;
	DWORD Level = 0;

	SiegeWarIdx = (DWORD)atoi((char*)pData->Data[eSWI_SiegeWarIdx]);
	RegistTime = (DWORD)atoi((char*)pData->Data[eSWI_RegistTime]);
	SiegeWarTime = (DWORD)atoi((char*)pData->Data[eSWI_SiegeWarTime]);
	Level = (DWORD)atoi((char*)pData->Data[eSWI_SiegeWarLevel]);

	if( SiegeWarIdx == 0 )			return;

	SIEGEWARMGR->SetSiegeWarInfo( SiegeWarIdx, TaxRate, RegistTime, SiegeWarTime, Level );
	SiegeWarGuildInfoLoad( 0, SiegeWarIdx, SIEGEWARMGR->GetSiegeMapNum() );
}

void SiegeWarInfoInsert( DWORD SiegeWarIdx, DWORD RegistTime, DWORD SiegeWarTime, DWORD Level, DWORD MapNum )
{
	sprintf( txt, "EXEC %s %d, %d, %d, %d, %d", STORED_SIEGE_INFOINSERT, SiegeWarIdx, RegistTime, SiegeWarTime, Level, MapNum );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarInfoInsert, 0, txt );
}

void SiegeWarInfoUpdate( DWORD SiegeWarIdx, DWORD RegistTime, DWORD SiegeWarTime, DWORD Level, DWORD MapNum )
{
	sprintf( txt, "EXEC %s %d, %d, %d, %d, %d", STORED_SIEGE_INFOUPDATE, SiegeWarIdx, RegistTime, SiegeWarTime, Level, MapNum );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarInfoUpdate, 0, txt );
}

void SiegeWarGuildInfoLoad( DWORD StartDBIdx, DWORD SiegeWarIdx, DWORD MapNum )
{
	sprintf( txt, "EXEC %s %d, %d, %d", STORED_SIEGE_GUILDLOAD, StartDBIdx, SiegeWarIdx, MapNum );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarGuildInfoLoad, 0, txt );
}

void RSiegeWarGuildInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD LastDBIdx = 0;
	SIEGEWAR_GUILDDBINFO GuildInfo[100];

	for( DWORD i=0; i<pMessage->dwResult; ++i )
	{
		LastDBIdx = (DWORD)atoi((char*)pData[i].Data[eSWGI_GuildDBIdx]);
		GuildInfo[i].GuildIdx = (DWORD)atoi((char*)pData[i].Data[eSWGI_GuildIdx]);
		GuildInfo[i].Type = (BYTE)atoi((char*)pData[i].Data[eSWGI_GuildType]);
	}

	SIEGEWARMGR->SetSiegeGuildInfo( GuildInfo, pMessage->dwResult );

	if( pMessage->dwResult >= 100 )
		SiegeWarGuildInfoLoad( LastDBIdx, SIEGEWARMGR->GetSiegeWarIdx(), SIEGEWARMGR->GetSiegeMapNum() );
	else
	{
		if( g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetSiegeMapNum() )
			SIEGEWARMGR->CreateSiegeBattle();
	}
}

void SiegeWarGuildInsert( DWORD SiegeWarIdx, DWORD GuildIdx, DWORD Type, DWORD MapNum )
{
	sprintf( txt, "EXEC %s %d, %d, %d, %d", STORED_SIEGE_GUILDINSERT, SiegeWarIdx, GuildIdx, Type, MapNum );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarGuildInsert, 0, txt );
}

void SiegeWarGuildUpdate( DWORD SiegeWarIdx, DWORD GuildIdx, DWORD Type, DWORD MapNum )
{
	sprintf( txt, "EXEC %s %d, %d, %d, %d", STORED_SIEGE_GUILDUPDATE, SiegeWarIdx, GuildIdx, Type, MapNum );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarGuildUpdate, 0, txt );
}

void SiegeWarGuildDelete( DWORD SiegeWarIdx, DWORD GuildIdx, DWORD MapNum )
{
	sprintf( txt, "EXEC %s %d, %d, %d", STORED_SIEGE_GUILDDELETE, SiegeWarIdx, GuildIdx, MapNum );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarGuildDelete, 0, txt );
}

void SiegeWarAddLog( DWORD SiegeWarIdx, DWORD MapNum, DWORD GuildIdx, DWORD Logkind, DWORD Value )
{
	sprintf( txt, "EXEC dbo.Up_SW_AddlOg %d, %d, %d, %d, %d", SiegeWarIdx, MapNum, GuildIdx, Logkind, Value );
	g_DB.LogQuery( eQueryType_FreeQuery, eSiegeWarAddLog, 0, txt );
}


void ChangeCharacterAttr( DWORD dwPlayerIdx, DWORD dwAttr )
{
	sprintf( txt, "EXEC dbo.MP_CHARACTER_ChangeAttr_JP %d, %d", dwPlayerIdx, dwAttr );
	g_DB.Query( eQueryType_FreeQuery, eChangeCharacterAttr, 0, txt, dwPlayerIdx );
}

void SiegeWarProfitInfoLoad()
{
	sprintf( txt, "EXEC dbo.MP_SIEGEWAR_PROFIT_InfoLoad" );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarProfitInfoLoad, 0, txt );
}

void RSiegeWarProfitInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	sSWPROFIT sInfo;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		memset( &sInfo, 0, sizeof(sInfo) );
		sInfo.nSWMapNum = atoi((char*)pData[i].Data[0]);
		sInfo.dwGuildIdx = (DWORD)atoi((char*)pData[i].Data[1]);
		sInfo.nTaxRateForStore = atoi((char*)pData[i].Data[2]);
		sInfo.dwTotalMoney = (DWORD)atoi((char*)pData[i].Data[3]);
		sInfo.RegDate.value = (DWORD)atoi((char*)pData[i].Data[4]);
		SWPROFITMGR->AddProfitTable( sInfo );
	}	

	//
	g_pServerSystem->SetStart( TRUE );

	//SW060719 문파포인트
	//GuildLoadGuildPointInfo(0);
}

void SiegeWarProfitUpdateGuild( int nMapNum, DWORD dwGuildIdx )
{
	sprintf( txt, "EXEC dbo.MP_SIEGEWAR_PROFIT_UpdateGuild %d, %d", nMapNum, dwGuildIdx );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarProfitUpdateGuild, 0, txt );
}

void SiegeWarProfitUpdateTaxRate( int nMapNum, int nTaxRate, DWORD dwRegDate )
{
	sprintf( txt, "EXEC dbo.MP_SIEGEWAR_PROFIT_UpdateTaxRate %d, %d, %d", nMapNum, nTaxRate, dwRegDate );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarProfitUpdateTaxRate, 0, txt );
}

void SiegeWarProfitUpdateProfitMoney( int nMapNum, DWORD dwMoney, int nType )
{
	sprintf( txt, "EXEC dbo.MP_SIEGEWAR_PROFIT_UpdateProfitMoney %d, %u, %d", nMapNum, dwMoney, nType );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarProfitUpdateProfitMoney, 0, txt );
}

void RSiegeWarProfitUpdateProfitMoney( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( !pMessage->dwResult ) return;

	SWPROFITMGR->SetProfitMoney( atoi((char*)pData[0].Data[0]), (DWORD)atoi((char*)pData[0].Data[1]) );
}

void ChangeGuildLocation( DWORD dwGuildIdx, WORD wMapNum )
{
	sprintf( txt, "EXEC dbo.MP_GUILD_ChangeLocation %d, %d", dwGuildIdx, wMapNum );
	g_DB.Query( eQueryType_FreeQuery, eChangeGuildLocation, 0, txt );	
}

/*어빌리티 삭제 - 이영준 
void ChangeCharacterStageAbility( DWORD dwCharacterIdx, BYTE stage, CAbilityGroup* pGroup )
{
	ABILITY_TOTALINFO TInfo;
	ABILITYMGR->GetAbilityTotalInfo( pGroup, &TInfo );

	char battle[MAX_ABILITY_NUM_PER_GROUP+1] = {0,};
	char hwa[6] = {0,};
	char geuk[6] = {0,};
	memcpy( battle, TInfo.AbilityDataArray[eAGK_Battle], MAX_ABILITY_NUM_PER_GROUP+1 );
	memcpy( hwa, &TInfo.AbilityDataArray[eAGK_Battle][17], 6 );
	memcpy( geuk, &TInfo.AbilityDataArray[eAGK_Battle][23], 6 );

	if( stage == eStage_Hwa )
	{
		memcpy( &battle[17], "AAAAAA", 6 );
		memcpy( &battle[23], hwa, 6 );
	}
	else if( stage == eStage_Geuk )
	{
		memcpy( &battle[17], geuk, 6 );
		memcpy( &battle[23], "AAAAAA", 6 );
	}
	else if( stage == eStage_Normal )
	{
		memcpy( &battle[17], "AAAAAA", 6 );
		memcpy( &battle[23], "AAAAAA", 6 );
	}

	sprintf( txt, "EXEC dbo.MP_CHARACTER_ChangeStage_Ability %d, \'%s\'", dwCharacterIdx, battle );
	g_DB.Query( eQueryType_FreeQuery, eChangeCharacterStageAbility, 0, txt );
}
*/
//void RShopItemRareInsertToDB(LPQUERY pData,  LPDBMESSAGE pMessage)
//{
//	DWORD characteridx = (DWORD)atoi((char*)pData[0].Data[eCIR_ObjectID]);
//	DWORD pos = (DWORD)atoi((char*)pData[0].Data[eCIR_Position]);
//
//	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( characteridx );
//	if( !pPlayer )			return;
//	
//	ITEMBASE* pItem = (ITEMBASE*)ITEMMGR->GetItemInfoAbsIn( pPlayer, (POSTYPE)pos );
//	if( !pItem )			return;
//
//	if( pItem->RareIdx )
//	{
//		if( FALSE == pPlayer->RemoveItemRareOption( pItem->RareIdx ) )
//		{
//			char szBuf[128];
//			sprintf( szBuf, "ShopItemRareInsert_Error! PlayerID : %d\t ItemDBIdx : %d\n", pPlayer->GetID(),
//				pItem->dwDBIdx );
//			ASSERTMSG(0, szBuf);
//		}
//	}
//
//
//	ITEM_RARE_OPTION_INFO RareOptionInfo;
//	RareOptionInfo.dwRareOptionIdx				= (DWORD)atoi((char*)pData[0].Data[eCIR_RareID]);	//!상수 따로 안만들어도 될듯
//	RareOptionInfo.dwItemDBIdx				= (DWORD)atoi((char*)pData[0].Data[eCIR_ItemDBID]);
//	RareOptionInfo.Str					= (WORD)atoi((char*)pData[0].Data[eCIR_Str]);
//	RareOptionInfo.Dex					= (WORD)atoi((char*)pData[0].Data[eCIR_Dex]);				
//	RareOptionInfo.Vit					= (WORD)atoi((char*)pData[0].Data[eCIR_Vit]);				
//	RareOptionInfo.Int					= (WORD)atoi((char*)pData[0].Data[eCIR_Int]);					
//	RareOptionInfo.Wis					= (WORD)atoi((char*)pData[0].Data[eCIR_Wis]);					
//	RareOptionInfo.Life						= (DWORD)atoi((char*)pData[0].Data[eCIR_Life]);					
//	RareOptionInfo.Mana					= (WORD)atoi((char*)pData[0].Data[eCIR_Mana]);				
//	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[0].Data[eCIR_FireAttrRegist])/100.f );
//	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[0].Data[eCIR_WaterAttrRegist])/100.f );
//	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[0].Data[eCIR_TreeAttrRegist])/100.f );
//	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[0].Data[eCIR_GoldAttrRegist])/100.f );
//	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[0].Data[eCIR_EarthAttrRegist])/100.f );
//	RareOptionInfo.PhyAttack				= (WORD)atoi((char*)pData[0].Data[eCIR_PhyAttack]);
//	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[0].Data[eCIR_FireAttrAttack])/100.f );
//	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[0].Data[eCIR_WaterAttrAttack])/100.f );
//	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[0].Data[eCIR_TreeAttrAttack])/100.f );
//	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[0].Data[eCIR_GoldAttrAttack])/100.f );
//	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[0].Data[eCIR_EarthAttrAttack])/100.f );
//	RareOptionInfo.PhyDefense				= (WORD)atoi((char*)pData[0].Data[eCIR_PhyDefense]);
//
//	pPlayer->AddItemRareOption(&RareOptionInfo);
//
//	pItem->RareIdx = RareOptionInfo.dwRareOptionIdx;
//
//	// Log기록
//	LogItemRareOption(pPlayer->GetID(), pItem->dwDBIdx, &RareOptionInfo);
//
//	MSG_ITEM_RAREITEM_GET msg;
//	SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_RARECREATE_ACK );
//	msg.dwObjectID = pPlayer->GetID();	//pPlayer->GetID();
//	msg.RareItemBase = *pItem;
//	msg.RareInfo = RareOptionInfo;
//	pPlayer->SendMsg(&msg, sizeof(msg));
//}

void QuestEventCheck( DWORD dwUserIdx, DWORD dwCharacterIdx, DWORD dwNpcIdx, DWORD dwQuestIdx, char* pTime )
{
	sprintf( txt, "EXEC dbo.MP_QUEST_EventCheck %d, %d, %d, %d, \'%s\'", dwUserIdx, dwCharacterIdx, dwNpcIdx, dwQuestIdx, pTime );
	g_DB.Query( eQueryType_FreeQuery, eQuestEventCheck, dwCharacterIdx, txt, dwCharacterIdx );

	/*
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( dwCharacterIdx );
	if( !pPlayer )	return;

	MSG_DWORD2 Msg;
	Msg.Category = MP_QUEST; 

	DWORD dwRet = 0;
	switch( dwRet )
	{
	case 0:		// 할수있다
	{
	Msg.Protocol = MP_QUESTEVENT_NPCTALK_ACK;

	}
	break;
	case 1:		// 이미 했다
	{
	Msg.Protocol = MP_QUESTEVENT_NPCTALK_NACK;
	}
	break;
	case 2:		// 할 수 없다
	{
	Msg.Protocol = MP_QUESTEVENT_NPCTALK_NACK;
	}
	break;
	default:
	return;
	}

	Msg.dwData1 = dwQuestIdx;
	Msg.dwData2 = dwRet;
	pPlayer->SendMsg( &Msg, sizeof(Msg) );*/

}

void RQuestEventCheck( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pMessage->dwID );
	if( !pPlayer )	return;

	MSG_DWORD2 Msg;
	Msg.Category = MP_QUEST; 

	DWORD dwNpcIdx = (DWORD)atoi((char*)pData[0].Data[0]);
	DWORD dwQuestIdx = (DWORD)atoi((char*)pData[0].Data[1]);
	DWORD dwRet = (DWORD)atoi((char*)pData[0].Data[2]);
	switch( dwRet )
	{
	case 0:		// 할수있다
		{
			Msg.Protocol = MP_QUESTEVENT_NPCTALK_ACK;

			// quest event
			CQuestEvent qe;
			qe.SetValue( eQuestEvent_NpcTalk, dwNpcIdx, dwQuestIdx );
			QUESTMGR->AddQuestEvent( pPlayer, &qe );
		}
		break;
	case 1:		// 이미 했다
		{
			Msg.Protocol = MP_QUESTEVENT_NPCTALK_NACK;
		}
		break;
	case 2:		// 할 수 없다
		{
			Msg.Protocol = MP_QUESTEVENT_NPCTALK_NACK;
		}
		break;
	default:
		return;
	}

	Msg.dwData1 = dwQuestIdx;
	Msg.dwData2 = dwRet;
	pPlayer->SendMsg( &Msg, sizeof(Msg) );
}

void QuestEventEnd( DWORD dwUserIdx, DWORD dwCharacterIdx, char* pTime )
{
	sprintf( txt, "EXEC dbo.MP_QUEST_EventEnd %d, %d, \'%s\'", dwUserIdx, dwCharacterIdx, pTime );
	g_DB.Query( eQueryType_FreeQuery, eQuestEventCheck, 0, txt, dwCharacterIdx );
}



// event 060627 - 운영팀 이벤트
void GMEvent01( DWORD dwUserIdx, DWORD dwCharacterIdx, DWORD dwLevel, int nServerSetNum )
{
	sprintf( txt, "EXEC dbo.MP_GMEvent01 %d, %d, %d, %d", dwUserIdx, dwCharacterIdx, dwLevel, nServerSetNum );
	g_DB.Query( eQueryType_FreeQuery, eGMEvent01, 0, txt, dwCharacterIdx );
}

void QuickInfo( DWORD characterIdx )
{
	sprintf( txt, "EXEC dbo.MP_QUICK_Info %d", characterIdx );
	g_DB.Query( eQueryType_FreeQuery, eQuickInfo, characterIdx, txt, characterIdx );
}

void RQuickInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	DWORD characterIdx;
	BYTE sheet;

	if( count > 8 )
	{
		/// 단축창 탭정보가 8개보다 많다
	}
	else if( ( count == 1 ) && ( atoi( ( char* )pData->Data[ 0 ] ) == 0 ) )
	{
		/// 단축창 정보가 없다.
		/// 기본 셋팅
		CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(atoi((char*)pData->Data[1]));
		if(pPlayer == NULL)
			return;

		for( BYTE sheet = 0; sheet < 8; ++sheet )
		{
			for( int j = 0; j < 10; ++j )
			{
				SLOT_INFO info;

				info.kind = 0;
				info.dbIdx = 0;
				info.idx = 0;
				info.data = 0;

				pPlayer->AddQuick( (BYTE)sheet, j, &info );
			}
		}

		pPlayer->SetInitState(PLAYERINITSTATE_QUICK_INFO, pMessage->dwID);

	}
	else
	{
		characterIdx =( DWORD )atoi( ( char* )pData[ 0 ].Data[ eQuick_CharacterIdx ] );
		CPlayer* pPlayer = ( CPlayer * )g_pUserTable->FindUser( characterIdx );
		if( pPlayer == NULL )
			return;

		for( DWORD i = 0; i < count; i++ )
		{
			sheet = ( BYTE )atoi( ( char* )pData[ i ].Data[ eQuick_Sheet ] );

			for( int j = 0; j < 10; j++ )
			{
				SLOT_INFO info;

				info.kind = ( BYTE )atoi( ( char* )pData[ i ].Data[ eQuick_Kind + ( j * 4 ) ] );
				info.dbIdx = ( DWORD )atoi( ( char* )pData[ i ].Data[ eQuick_DBIdx + ( j * 4 ) ] );
				info.idx = ( DWORD )atoi( ( char* )pData[ i ].Data[ eQuick_Idx + ( j * 4 ) ] );
				info.data = ( WORD )atoi( ( char* )pData[ i ].Data[ eQuick_Data + ( j * 4 ) ] );

				pPlayer->AddQuick( sheet, j, &info );
			}
		}

		pPlayer->SetInitState(PLAYERINITSTATE_QUICK_INFO, pMessage->dwID);
	}
}

void QuickUpdate( DWORD characterIdx, BYTE sheet, SLOT_INFO* pSlot )
{
	char buf[ 10 ][ 40 ];
	for( int i = 0; i < 10; i++ )
	{
		sprintf( buf[i], "%d, %u, %d, %d", pSlot[i].kind, pSlot[i].dbIdx, pSlot[i].idx, pSlot[i].data );
	}

	sprintf( txt, "EXEC dbo.MP_QUICK_Update %d, %d, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s",
		characterIdx,
		sheet,
		buf[ 0 ],
		buf[ 1 ],
		buf[ 2 ],
		buf[ 3 ],
		buf[ 4 ],
		buf[ 5 ],
		buf[ 6 ],
		buf[ 7 ],	
		buf[ 8 ],
		buf[ 9 ] );

	g_DB.Query( eQueryType_FreeQuery, eQuickUpdate, characterIdx, txt, characterIdx );
}


// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.04.23	2007.09.12	2008.01.16
void FarmLoadFarmInfo(WORD nMapNum)
{
	// 프로시저를 사용해서 DB에서 농장 데이터를 가져오기 위한 설정을 한다.
	for(int i=0; i<CSHFarmZone::FARM_ZONE_MAX; i++)
	{
		// 현재 맵이 해당 농장의 맵이 아니면 건너뛰기
		if (g_csFarmManager.GetFarmZone(i) == NULL || g_csFarmManager.GetFarmZone(i)->GetMapID() != nMapNum) continue;

		CSHFarmZone* pcsFarmZone = g_csFarmManager.GetFarmZone(i);

		for(int j=0; j<pcsFarmZone->GetFarmNum(); j++)
		{
			sprintf(txt, "EXEC  %s %d, %d", STORED_FARM_LOAD_FARMSTATE, i, j);
			g_DB.Query(eQueryType_FreeQuery, eFarm_LoadFarmState, 0, txt);

			CSHFarm* pcsFarm = pcsFarmZone->GetFarm(j);
			CSHGarden* pcsGarden = pcsFarm->GetGarden(0);
			for(int k=0; k<pcsGarden->GetCropNum(); k++)
			{
				CSHCrop* pcsCrop = pcsGarden->GetCrop(k);
				sprintf(txt, "EXEC  %s %d, %d, %d", STORED_FARM_LOAD_CROPINFO, i, j, k);
				g_DB.Query(eQueryType_FreeQuery, eFarm_LoadCropInfo, 0, txt);
			}
			// 080416 KTH -- 축사 정보를 불러 온다.
			CPen*	pcsPen = pcsFarm->GetAnimalCage(0);
			for( int k = 0; k < pcsPen->GetAnimalNum(); k++ )
			{
				CAnimal* pcsAnimal = pcsPen->GetAnimal(k);
				sprintf(txt, "EXEC %s %d, %d, %d", STORED_FARM_LOAD_ANIMALINFO, i, j, k);
				g_DB.Query(eQueryType_FreeQuery, eFarm_LoadAnimalInfo, 0, txt);
			}
		}
	}
}

BOOL FarmLoadFarmInfoNew(WORD nMapNum)
{
	static int j = 0;

	int i =0;
	
	if (g_csFarmManager.GetFarmZone(i) == NULL || g_csFarmManager.GetFarmZone(i)->GetMapID() != nMapNum) return FALSE;

	CSHFarmZone* pcsFarmZone = g_csFarmManager.GetFarmZone(i);
	
	if( j >= pcsFarmZone->GetFarmNum() )
		return FALSE;

	sprintf(txt, "EXEC  %s %d, %d", STORED_FARM_LOAD_FARMSTATE, i, j);
	g_DB.Query(eQueryType_FreeQuery, eFarm_LoadFarmState, 0, txt);

	CSHFarm*	pcsFarm = pcsFarmZone->GetFarm(j);
	CSHGarden*	pcsGarden = pcsFarm->GetGarden(0);
	CPen*		pcsAnimalCage = pcsFarm->GetAnimalCage(0);

	for(int k=0; k<pcsGarden->GetCropNum(); k++)
	{
		CSHCrop* pcsCrop = pcsGarden->GetCrop(k);
		sprintf(txt, "EXEC  %s %d, %d, %d", STORED_FARM_LOAD_CROPINFO, i, j, k);
		g_DB.Query(eQueryType_FreeQuery, eFarm_LoadCropInfo, 0, txt);
	}

	for( int k = 0; k < pcsAnimalCage->GetAnimalNum(); k++ )
	{
		CAnimal* pcsAnimal = pcsAnimalCage->GetAnimal(k);
		sprintf(txt, "EXEC %s %d, %d, %d", STORED_FARM_LOAD_ANIMALINFO, i, j, k);
		g_DB.Query(eQueryType_FreeQuery, eFarm_LoadAnimalInfo, 0, txt);
	}

	j++;

	return TRUE;
}

void RFarm_LoadFarmState(LPQUERY pData, LPDBMESSAGE pMessage)
{
	// DB에서 읽어온 농장 데이터를 서버의 메모리에 설정한다.
	// ..각각의 플레이어에게는 접속하거나 농장에 들어오거나 할 때 정보를 보내기로 한다. DB데이터는 한 번만 읽어오면 되므로.
	CSHFarmManager::PACKET_FARM_STATE_WITH_TAX packet;

	packet.Category = NULL;
	packet.Protocol = NULL;
	packet.dwObjectID = pMessage->dwID;

	if( pMessage->dwResult == 0 )
	{
	}
	else
	{
		packet.nFarmZone			= (WORD)atoi((char*)pData[0].Data[eFarm_FarmState_Zone]);
		packet.nFarmID				= (WORD)atoi((char*)pData[0].Data[eFarm_FarmState_Farm]);
		packet.nFarmState			= (WORD)atoi((char*)pData[0].Data[eFarm_FarmState_State]);
		packet.nFarmOwner			= (DWORD)atoi((char*)pData[0].Data[eFarm_FarmState_OwnerID]);
		packet.nGardenGrade			= (WORD)atoi((char*)pData[0].Data[eFarm_FarmState_GardenGrade]);
		packet.nHouseGrade			= (WORD)atoi((char*)pData[0].Data[eFarm_FarmState_HouseGrade]);
		packet.nWarehouseGrade		= (WORD)atoi((char*)pData[0].Data[eFarm_FarmState_WarehouseGrade]);
		packet.nAnimalCageGrade		= (WORD)atoi((char*)pData[0].Data[eFarm_FarmState_AnimalCageGrade]);
		packet.nFenceGrade			= (WORD)atoi((char*)pData[0].Data[eFarm_FarmState_FenceGrade]);
		packet.nTaxArrearageFreq	= (WORD)atoi((char*)pData[0].Data[9]);
		SafeStrCpy(packet.szTaxPayPlayerName,  (char*)pData[0].Data[10], MAX_NAME_LENGTH+1);

		g_csFarmManager.SRV_SetFarmStateFromDBQueue(NULL, &packet);
	}
}

void RFarm_LoadCropInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	// DB에서 읽어온 농장 데이터를 서버의 메모리에 설정한다.
	// ..각각의 플레이어에게는 접속하거나 농장에 들어오거나 할 때 정보를 보내기로 한다. DB데이터는 한 번만 읽어오면 되므로.
	CSHFarmManager::PACKET_FARM_GARDEN_CROP_PLANT packet;

	packet.Category = NULL;
	packet.Protocol = NULL;
	packet.dwObjectID = pMessage->dwID;

	if( pMessage->dwResult == 0 )
	{
	}
	else
	{
		packet.nFarmZone			= (WORD)atoi((char*)pData[0].Data[eFarm_CropInfo_Zone]);
		packet.nFarmID				= (WORD)atoi((char*)pData[0].Data[eFarm_CropInfo_Farm]);
		packet.nGardenID			= 0;
		packet.nCropID				= (WORD)atoi((char*)pData[0].Data[eFarm_CropInfo_Crop]);
		packet.nCropOwner			= (DWORD)atoi((char*)pData[0].Data[eFarm_CropInfo_OwnerID]);
		packet.nCropKind			= (WORD)atoi((char*)pData[0].Data[eFarm_CropInfo_Kind]);
		packet.nCropStep			= (WORD)atoi((char*)pData[0].Data[eFarm_CropInfo_Step]);
		packet.nCropLife			= (WORD)atoi((char*)pData[0].Data[eFarm_CropInfo_Life]);
		packet.nCropNextStepTime	= (WORD)atoi((char*)pData[0].Data[eFarm_CropInfo_NextStepTime]);
		packet.nCropSeedGrade		= (WORD)atoi((char*)pData[0].Data[eFarm_CropInfo_SeedGrade]);

		g_csFarmManager.SRV_SetCropInfoFromDBQueue(NULL, &packet);
	}
}

// 080416 KTH -- 가축 정보
void RFarm_LoadAnimalInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	// DB에서 읽어온 농장 데이터를 서버의 메모리에 설정한다.
	// ..각각의 플레이어에게는 접속하거나 농장에 들어오거나 할 때 정보를 보내기로 한다. DB데이터는 한 번만 읽어오면 되므로.
	CSHFarmManager::PACKET_FARM_ANIMAL_CAGE_INSTALL_PLANT packet;

	packet.Category = NULL;
	packet.Protocol = NULL;
	packet.dwObjectID = pMessage->dwID;

	if( pMessage->dwResult == 0 )
	{
	}
	else
	{
		packet.nFarmZone			= (WORD)atoi((char*)pData[0].Data[eFarm_AnimalInfo_Zone]);
		packet.nFarmID				= (WORD)atoi((char*)pData[0].Data[eFarm_AnimalInfo_Farm]);
		packet.nAnimalCageID		= 0;
		packet.nAnimalID			= (WORD)atoi((char*)pData[0].Data[eFarm_AnimalInfo_Animal]);
		packet.nAnimalOwner			= (DWORD)atoi((char*)pData[0].Data[eFarm_AnimalInfo_OwnerID]);
		packet.nAnimalKind			= (WORD)atoi((char*)pData[0].Data[eFarm_AnimalInfo_Kind]);
		packet.nAnimalStep			= (WORD)atoi((char*)pData[0].Data[eFarm_AnimalInfo_Step]);
		packet.nAnimalLife			= (WORD)atoi((char*)pData[0].Data[eFarm_AnimalInfo_Life]);
		packet.nAnimalNextStepTime	= (WORD)atoi((char*)pData[0].Data[eFarm_AnimalInfo_NextStepTime]);
		packet.nAnimalContentment	= (WORD)atoi((char*)pData[0].Data[eFarm_AnimalInfo_Contentment]);
		packet.nAnimalInterest		= (WORD)atoi((char*)pData[0].Data[eFarm_AnimalInfo_Interest] );
		//packet.nCropSeedGrade		= (WORD)atoi((char*)pData[0].Data[eFarm_CropInfo_SeedGrade]);

		g_csFarmManager.SRV_SetAnimalInfoFromDBQueue(NULL, &packet);
	}
}

void Farm_LoadTimeDelay(DWORD nPlayerID)
{
	sprintf(txt, "EXEC  %s %d", STORED_FARM_LOAD_DELAYTIME, nPlayerID);
	g_DB.Query(eQueryType_FreeQuery, eFarm_LoadTimeDelay, nPlayerID, txt, nPlayerID);
}

void Farm_SetTimeDelay(DWORD nPlayerID, DWORD nKind, DWORD nRemainSecond)
{
	sprintf(txt, "EXEC  %s %d, %d, %d", STORED_FARM_SET_DELAYTIME, nPlayerID, nKind, nRemainSecond);
	g_DB.Query(eQueryType_FreeQuery, eFarm_SetTimeDelay, 0, txt, nPlayerID);
}

void RFarm_LoadTimeDelay(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	for(DWORD i=0; i<pMessage->dwResult; i++)
	{
		int nKind = atoi((char*)pData[i].Data[0]);
		if (nKind == CSHFarmManager::FARM_TIMEDELAY_KIND_PLANT)
		{
			// 수치 데이터가 변경되었을 경우에 이미 기록이 되어 있는 경우에는 변경된 수치가 적용되지 않고 UI에도 남은 시간이 표시되지 않는다.
			// 그래서 수치 데이터와 DB값을 체크. 남은 시간을 기록해서 그럼..
			DWORD nTime = atoi((char*)pData[i].Data[0]);
			if (nTime > CSHCrop::CROP_PLANT_RETRY_TIME)
				nTime = CSHCrop::CROP_PLANT_RETRY_TIME;
			pPlayer->SetFarmCropPlantRetryTime(atoi((char*)pData[i].Data[1]));
		}
		else if (nKind == CSHFarmManager::FARM_TIMEDELAY_KIND_MANURE)
		{
			// 수치 데이터가 변경되었을 경우에 이미 기록이 되어 있는 경우에는 변경된 수치가 적용되지 않고 UI에도 남은 시간이 표시되지 않는다.
			// 그래서 수치 데이터와 DB값을 체크. 남은 시간을 기록해서 그럼..
			DWORD nTime = atoi((char*)pData[i].Data[1]);
			if (nTime > CSHCrop::CROP_MANURE_RETRY_TIME)
				nTime = CSHCrop::CROP_MANURE_RETRY_TIME;
			pPlayer->SetFarmCropManureRetryTime(atoi((char*)pData[i].Data[1]));
		}
		// 080508 KTH -- 가축 사료주기 딜레이 추가
		else if (nKind == CSHFarmManager::FARM_TIMEDELAY_KIND_FEED)
		{
			DWORD nTime = atoi((char*)pData[i].Data[1]);
			if(nTime > CAnimal::ANIMAL_FEED_DELAY_TIME)
				nTime = CAnimal::ANIMAL_FEED_DELAY_TIME;
			pPlayer->SetFarmAnimalFeedRetryTime(atoi((char*)pData[i].Data[1]));
		}
		else if (nKind == CSHFarmManager::FARM_TIMEDELAY_KIND_CLEAN )
		{
			DWORD nTime = atoi((char*)pData[i].Data[1]);
			if( nTime > CAnimal::ANIMAL_CLEANING_RETRY_TIME)
				nTime = CAnimal::ANIMAL_CLEANING_RETRY_TIME;
			pPlayer->SetFarmAnimalCleanRetryTime(atoi((char*)pData[i].Data[1]));
		}
	}

	//---
	MonsterMeter_Load( pMessage->dwID );

}

void Farm_SetTaxInfo(DWORD nFarmZone, DWORD nFarmID, DWORD nTaxArrearageFreq, DWORD nTaxPayPlayerID)
{
	sprintf(txt, "EXEC  %s %d, %d, %d, %d", STORED_FARM_SET_TAXINFO, nFarmZone, nFarmID, nTaxArrearageFreq, nTaxPayPlayerID);
	g_DB.Query(eQueryType_FreeQuery, eFarm_SetTaxInfo, 0, txt);
}
// E 농장시스템 추가 added by hseos 2007.04.23	2007.09.12	2008.01.16

// desc_hseos_몬스터미터01
// S 몬스터미터 추가 added by hseos 2007.05.28
void MonsterMeter_Load(DWORD nPlayerID)
{
	sprintf(txt, "EXEC  %s %d", STORED_MONSTERMETER_LOAD_INFO, nPlayerID);
	g_DB.Query(eQueryType_FreeQuery, eMonsterMeter_LoadInfo, nPlayerID, txt, nPlayerID);
}

void MonsterMeter_Save(DWORD nPlayerID, DWORD nPlayTime, DWORD nKillMonNum, DWORD nPlayTimeTotal, DWORD nKillMonNumTotal)
{
	sprintf(txt, "EXEC  %s %d, %d, %d, %d, %d", STORED_MONSTERMETER_SET_INFO, nPlayerID, nPlayTime, nKillMonNum, nPlayTimeTotal, nKillMonNumTotal);
	g_DB.Query(eQueryType_FreeQuery, eMonsterMeter_SetInfo, 0, txt, nPlayerID);
}

void RMonsterMeter_LoadInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	if( pMessage->dwResult == 0 )
	{
		g_Console.LOG(4, "MonsterMeter Load Error. Inited Data..");
	}
	else
	{
		pPlayer->SetMonstermeterPlaytime(atoi((char*)pData[0].Data[eMonsterMeter_PlayTime]), atoi((char*)pData[0].Data[eMonsterMeter_PlayTimeTotal]));
		pPlayer->SetMonstermeterKillMonNum(atoi((char*)pData[0].Data[eMonsterMeter_KillMonNum]), atoi((char*)pData[0].Data[eMonsterMeter_KillMonNumTotal]));
	}

	ResidentRegist_LoadInfo( pMessage->dwID );
}
// E 몬스터미터 추가 added by hseos 2007.05.28

// desc_hseos_주민등록01
// S 주민등록 추가 added by hseos 2007.06.04
void ResidentRegist_LoadInfo(DWORD nPlayerID)
{
	sprintf(txt, "EXEC  %s %d", STORED_RESIDENTREGIST_LOAD_INFO, nPlayerID);
	g_DB.FreeMiddleQuery(RResidentRegist_LoadInfo, nPlayerID, txt, nPlayerID);
}

void ResidentRegist_SaveInfo(DWORD nPlayerID, void* _pInfo, int nIntroduction)
{
	// 주민 등록. 등록시에 자기소개는 안 함
	if (nIntroduction == 0)
	{
		CSHResidentRegistManager::stRegistInfo* pInfo = (CSHResidentRegistManager::stRegistInfo*)_pInfo;

		sprintf(txt, "EXEC  %s %d, %s, %d, %d, %d, %d, %d, %d, %d, %d, %d", STORED_RESIDENTREGIST_SAVE_INFO, 
			nPlayerID,
			"XXX",
			pInfo->nSex, 
			pInfo->nAge,
			pInfo->nLocation,
			pInfo->nFavor,
			pInfo->nPropensityLike01,
			pInfo->nPropensityLike02,
			pInfo->nPropensityLike03,
			pInfo->nPropensityDisLike01,
			pInfo->nPropensityDisLike02);
	}
	// 자기소개
	else if (nIntroduction == 1)
	{
		char* pszIntroduction = (char*)_pInfo;

		sprintf(txt, "EXEC  %s %d, '%s'", STORED_RESIDENTREGIST_UPDATE_INTRODUCTION, nPlayerID, pszIntroduction);
	}

	g_DB.FreeMiddleQuery(NULL, 0, txt, nPlayerID);
}
// 080215 KTH -- Reset Resident Regist
void ResidentRegist_Reset(DWORD nPlayerID)
{
	sprintf(txt, "EXEC %s %d", STORED_RESIDENTREGIST_RESET, nPlayerID);
	g_DB.Query(eQueryType_FreeQuery, eResidentRegist_Reset, nPlayerID, txt, nPlayerID);

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(nPlayerID);

	pPlayer->UpdateLogoutToDB();
	PARTYMGR->UserLogOut(pPlayer);
	FRIENDMGR->UserLogOut(pPlayer->GetID());
	GUILDMGR->UserLogOut(pPlayer);

	SaveCharInfoBeforeLogout(pPlayer->GetID(), pPlayer->GetAgentNum(), pPlayer->GetUserID(), pPlayer->GetMoney(eItemTable_Storage));
}
/*
void RResidentRegist_ResetResult(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);

	pPlayer->UpdateLogoutToDB();
	PARTYMGR->UserLogOut(pPlayer);
	FRIENDMGR->UserLogOut(pPlayer->GetID());
	GUILDMGR->UserLogOut(pPlayer);

	SaveCharInfoBeforeLogout(pPlayer->GetID(), pPlayer->GetAgentNum(), pPlayer->GetUserID(), pPlayer->GetMoney(eItemTable_Storage));
}
*/
void RResidentRegist_LoadInfo(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	if( pMessage->dwResult == 0 )
	{
	}
	else
	{
		CSHResidentRegistManager::stRegistInfo stInfo;

		SafeStrCpy(stInfo.szNickName, (char*)pData[0].Data[eResidentRegist_NickName], MAX_NAME_LENGTH+1);
		stInfo.nSex					= atoi((char*)pData[0].Data[eResidentRegist_Sex]);
		stInfo.nAge					= atoi((char*)pData[0].Data[eResidentRegist_Age]);
		stInfo.nLocation			= atoi((char*)pData[0].Data[eResidentRegist_Location]);
		stInfo.nFavor				= atoi((char*)pData[0].Data[eResidentRegist_Favor]);
		stInfo.nPropensityLike01	= atoi((char*)pData[0].Data[eResidentRegist_PropensityLike01]);
		stInfo.nPropensityLike02	= atoi((char*)pData[0].Data[eResidentRegist_PropensityLike02]);
		stInfo.nPropensityLike03	= atoi((char*)pData[0].Data[eResidentRegist_PropensityLike03]);
		stInfo.nPropensityDisLike01	= atoi((char*)pData[0].Data[eResidentRegist_PropensityDisLike01]);
		stInfo.nPropensityDisLike02	= atoi((char*)pData[0].Data[eResidentRegist_PropensityDisLike02]);

		DATE_MATCHING_INFO stInfo2;
		ZeroMemory(&stInfo2, sizeof(stInfo2));

		SafeStrCpy(stInfo2.szName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
		stInfo2.bIsValid			= TRUE;
		stInfo2.byGender			= (BYTE)stInfo.nSex;
		stInfo2.dwAge				= stInfo.nAge;

		CHARACTER_TOTALINFO stExInfo;
		pPlayer->GetCharacterTotalInfo(&stExInfo);

		stInfo2.nRace				= stExInfo.Race;
		stInfo2.dwRegion			= stInfo.nLocation;
		stInfo2.dwGoodFeelingDegree	= stInfo.nFavor;
		stInfo2.pdwGoodFeeling[0]	= stInfo.nPropensityLike01;
		stInfo2.pdwGoodFeeling[1]	= stInfo.nPropensityLike02;
		stInfo2.pdwGoodFeeling[2]	= stInfo.nPropensityLike03;
		stInfo2.pdwBadFeeling[0]	= stInfo.nPropensityDisLike01;
		stInfo2.pdwBadFeeling[1]	= stInfo.nPropensityDisLike02;
		SafeStrCpy(stInfo2.szIntroduce, (char*)pData[0].Data[eResidentRegist_Introduction], MAX_INTRODUCE_LEN+1);
		pPlayer->SetResidentRegistInfo(&stInfo2);
	}

	pPlayer->SetInitState( PLAYERINITSTATE_FARM_INFO, MP_USERCONN_GAMEIN_SYN );
}
// E 주민등록 추가 added by hseos 2007.06.04

void SkillPointUpdate(DWORD CharacterIdx, DWORD SkillPoint)
{
	sprintf(txt, "EXEC dbo.MP_SKILLTREE_SkillPoint_Update %u, %u", CharacterIdx, SkillPoint);

	g_DB.Query(eQueryType_FreeQuery, eSkillPointUpdate, 0, txt, CharacterIdx);
}

void CharacterJobUpdate( DWORD CharacterIdx, WORD Grade, WORD Job1, WORD Job2, WORD Job3, WORD Job4, WORD Job5, WORD Job6 )
{
	sprintf(txt, "EXEC dbo.MP_CHARACTER_Job_Update %u, %d, %d, %d, %d, %d, %d, %d", CharacterIdx, Grade, Job1, Job2, Job3, Job4, Job5, Job6);

	g_DB.Query(eQueryType_FreeQuery, eJobUpdate, 0, txt, CharacterIdx);

}

void GuildAddScore( DWORD guildIndex, float score )
{
	sprintf( txt, "EXEC DBO.MP_GUILD_ADD_SCORE %d, %f", guildIndex, score );

	g_DB.Query( eQueryType_FreeQuery, eGuildAddScore, guildIndex, txt );
}


void RGuildAddScore( LPQUERY query, LPDBMESSAGE message )
{
	if( 1 != message->dwResult )
	{
		ASSERT( 0 );
		return;
	}

	// 다른 맵서버는 알아서 주기적으로 업데이트할 것이다. 자신만 업데이트하면 된다.
	{
		const DWORD	guildIndex	= message->dwID;
		const DWORD	score		= atoi( ( char* )query[ 0 ].Data[ 0 ] );

		MSG_DWORD2 message;
		message.Category	= MP_GUILD;
		message.Protocol	= MP_GUILD_SCORE_UPDATE;
		message.dwData1		= guildIndex;
		message.dwData2		= score;
		
		CGuildScore* guild = GUILDMGR->GetGuildScore( guildIndex );

		if( guild )
		{
			guild->Send( &message, sizeof( message ) );
		}
	}
}

// 080417 LUJ, 길드 스킬 획득
void RGuildAddSkill( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD	playerIndex	= dbMessage->dwID;
	
	CPlayer* player	= ( CPlayer* )g_pUserTable->FindUser( playerIndex );

	if( ! player )
	{
		return;
	}

	CGuild* guild = GUILDMGR->GetGuild( player->GetGuildIdx() );

	if( ! guild )
	{
		return;
	}

	const DWORD		skillIndex	=			atoi( ( char* )query[ 0 ].Data[ 0 ] );
	const BYTE		skillLevel	=			atoi( ( char* )query[ 0 ].Data[ 1 ] );
	const MONEYTYPE	money		= MONEYTYPE(atoi( ( char* )query[ 0 ].Data[ 2 ] ) );
	const float		guildScore	= float(	atof( ( char* )query[ 0 ].Data[ 3 ] ) );

	// 080417 LUJ, 스킬 인덱스가 0으로 반환되면 포인트가 부족해서 스킬 획득 실패
	if( ! skillIndex )
	{
		GUILDMGR->SendNackMsg( player, MP_GUILD_ADD_SKILL_NACK, eGuildErr_InsufficentScore );
		return;
	}
	else if( player->GetMoney() < money )
	{
		GUILDMGR->SendNackMsg( player, MP_GUILD_ADD_SKILL_NACK, eGuildErr_InsufficentMoney );
		return;
	}

	player->SetMoney( money, MONEY_SUBTRACTION );
	
	// 080417 LUJ, 로그
	{
		InsertLogGuildScore(
			guild->GetIdx(),
			guildScore,
			eGuildLog_SkillAdd );

		LogGuild(
			player->GetID(),
			guild->GetIdx(),
			eGuildLog_SkillAdd,
			skillIndex,
			skillLevel );
	}

	// 080417 LUJ, 다른 맵에도 전파한다
	{
		MSG_SKILL_UPDATE message;
		ZeroMemory( &message, sizeof( message ) );
		message.Category				= MP_GUILD;
		message.Protocol				= MP_GUILD_ADD_SKILL_ACK_TO_MAP;
		message.dwObjectID				= guild->GetIdx();
		message.SkillBase.wSkillIdx		= skillIndex;
		message.SkillBase.Level			= skillLevel;

		g_Network.Broadcast2AgentServer( ( char* )&message, sizeof( message ) );

		// 080417 LUJ, 현재 맵에도 전파한다
		GUILDMGR->NetworkMsgParse( MP_GUILD_ADD_SKILL_ACK_TO_MAP, &message );
	}	
}

// 080417 LUJ, 길드 스킬 로딩
void RGuildGetSkill( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD guildIndex = dbMessage->dwID;

	CGuild* guild = GUILDMGR->GetGuild( guildIndex );

	if( ! guild )
	{
		return;
	}

	MSG_SKILL_LIST message;
	{
		ZeroMemory( &message, sizeof( message ) );
		message.Category	= MP_GUILD;
		message.Protocol	= MP_GUILD_GET_SKILL_ACK;
		message.mSize		= dbMessage->dwResult;
	}
	
	for( DWORD row = 0; row < dbMessage->dwResult; ++row )
	{
		const MIDDLEQUERYST& record = query[ row ];

		const DWORD index	=		atoi( ( char* )record.Data[ 0 ] );
		const BYTE	level	= BYTE( atoi( ( char* )record.Data[ 1 ] ) );

		guild->AddSkill( index, level );

		{
			SKILL_BASE& data = message.mData[ row ];

			data.wSkillIdx	= index;
			data.Level		= level;
		}
	}

	// 080417 LUJ, 접속된 플레이어에게 스킬 정보를 전파한다. 플레이어는 직위에 따라 알아서 설정할 것이다
	guild->SendMsgToAll( &message, message.GetSize() );

	// 080417 LUJ, 길드 스킬을 최대로 읽어들였으면 DB에 다시 요청한다
	if( MAX_MIDDLEROW_NUM == dbMessage->dwResult )
	{
		const MIDDLEQUERYST& record			= query[ MAX_MIDDLEROW_NUM - 1 ];
		const DWORD			 lastSkillIndex = atoi( ( char* )record.Data[ 0 ] );

		g_DB.FreeMiddleQuery(
			RGuildGetSkill,
			guildIndex,
			"SELECT TOP %d SKILL_IDX, SKILL_LEVEL FROM TB_GUILD_SKILL WHERE GUILD_IDX = %d AND SKILL_IDX > %d ORDER BY SKILL_IDX",
			MAX_MIDDLEROW_NUM,
			guildIndex,
			lastSkillIndex );
	}
}

void GuildUpdateScore( CPlayer* killer, CPlayer* corpse, float killerPoint, float corpsePoint )
{
	sprintf( txt, "EXEC DBO.MP_GUILD_UPDATE_SCORE %d, %d, %.0f, %d, %d, %.0f",
		killer->GetID(),
		killer->GetGuildIdx(),
		killerPoint,
		corpse->GetID(),
		corpse->GetGuildIdx(),
		corpsePoint );

	g_DB.Query( eQueryType_FreeQuery, eGuildUpdateScore, 0, txt, killer->GetID() );
}


void RGuildUpdateScore( LPQUERY query, LPDBMESSAGE message )
{
	// 레코드 두개가 반환되어야 정상적으로 업데이트된 경우
	if( 2 != message->dwResult )
	{
		ASSERT( 0 );
		return;
	}	

	// 길드 점수가 0인 경우 차감할 길드 점수가 부족하여 갱신하지 못한 경우임.
	// 클라이언트가 이를 판단하여 적절한 메시지를 표시하면 해결됨
	{
		const QUERYST&	killerRecord		= query[ 0 ];
		const QUERYST&	corpseRecord		= query[ 1 ];
		const DWORD		killerPlayerIndex	= atoi( ( char* )killerRecord.Data[ 0 ] );	// 살해한 길드원 플레이어 인덱스
		const DWORD		corpsePlayerIndex	= atoi( ( char* )corpseRecord.Data[ 0 ] );	// 살해당한 길드원 플레이어 인덱스
		

		MSG_DWORD3 message;
		{
			message.Category	= MP_GUILD;
			message.dwData1		= killerPlayerIndex;
			message.dwData2		= corpsePlayerIndex;
		}

		// 첫번째 레코드는 다른 길드의 멤버를 살해하여 갱신된 길드 포인트
		{
			// 갱신이 실패할 경우 -1이 DB에서 전달되고, guildScore에는 UINT_MAX 값이 들어간다
			const DWORD			guildScore	= atoi( ( char* )killerRecord.Data[ 1 ] );
			const DWORD			guildIndex	= atoi( ( char* )killerRecord.Data[ 2 ] );
			const CGuildScore*	guild		= GUILDMGR->GetGuildScore( guildIndex );
			
			if( guild )
			{
				message.Protocol	= MP_GUILD_SCORE_UPDATE_BY_KILL;
				message.dwData3		= guildScore;
				
				guild->Send( &message, sizeof( message ) );
			}
		}

		// 두번째 레코드는 다른 길드원에게 살해당해 갱신된 길드 포인트
		{
			// 갱신이 실패할 경우 -1이 DB에서 전달되고, guildScore에는 UINT_MAX 값이 들어간다
			const DWORD			guildScore	= atoi( ( char* )corpseRecord.Data[ 1 ] );
			const DWORD			guildIndex	= atoi( ( char* )corpseRecord.Data[ 2 ] );
			const CGuildScore*	guild		= GUILDMGR->GetGuildScore( guildIndex );

			if( guild )
			{
				message.Protocol	= MP_GUILD_SCORE_UPDATE_BY_DEAD;
				message.dwData3		= guildScore;
				
				guild->Send( &message, sizeof( message ) );
			}
		}
	}
}


//void GuildReadMember( DWORD playerIndex, DWORD guildIndex )
//{
//	sprintf( txt, "EXEC DBO.MP_GUILD_MEMBER_SELECT %d", guildIndex );
//
//	g_DB.Query( eQueryType_FreeQuery, eGuildMemberSelect, playerIndex, txt );
//}
//
//
//void RGuildMemberSelect( LPQUERY query, LPDBMESSAGE dbMessage )
//{
//	if( 0 == dbMessage->dwResult )
//	{
//		ASSERT( 0 && "It means this guild has no member(at least it has one). It's unable" );
//		return;
//	}
//
//	const DWORD	playerIndex = dbMessage->dwID;
//	CObject*	object		= g_pUserTable->FindUser( playerIndex );
//
//	// 길드 정보의 동기화를 위해 DB에 의존하기 때문에 메모리에 담지 않고 전송한다
//	if( object )
//	{
//		MSG_GUILD_MEMBER_ALL message;
//
//		message.Category	= MP_GUILD;
//		message.Protocol	= MP_GUILD_MEMBER_READ;
//		message.dwObjectID	= playerIndex;
//		message.mSize		= dbMessage->dwResult;
//
//		MSG_GUILD_MEMBER_ALL::Member* member = message.mMember;
//
//		for( DWORD i = 0; i < dbMessage->dwResult; ++i, ++member )
//		{
//			const QUERYST& record = query[ i ];
//
//			SafeStrCpy( member->mName, ( char* )record.Data[ 0 ], sizeof( member->mName ) );
//
//			member->mIndex	= strtoul( ( char* )record.Data[ 1 ], 0, 10 );
//			member->mRank	= strtoul( ( char* )record.Data[ 2 ], 0, 10 );
//			member->mLevel	= strtoul( ( char* )record.Data[ 3 ], 0, 10 );
//		}
//
//		object->SendMsg( &message, message.GetSize() );
//	}
//}


void GuildReadData( DWORD playerIndex, DWORD guildIndex )
{
	sprintf( txt, "EXEC DBO.MP_GUILD_SELECT %d", guildIndex );

	g_DB.FreeMiddleQuery( RGuildReadData, playerIndex, txt );
}


void RGuildReadData( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	if( 1 != dbMessage->dwResult )
	{
		ASSERT( 0 );
		return;
	}

	const DWORD		playerIndex = dbMessage->dwID;
	CObject*		object		= g_pUserTable->FindUser( playerIndex );

	if( ! object )
	{
		return;
	}

	DWORD guildIndex;

	// 길드 정보의 동기화를 위해 DB에 의존하기 때문에 메모리에 담지 않고 전송한다
	{
		const MIDDLEQUERYST& record = query[ 0 ];
		MSG_GUILD_DATA message;

		message.Category		= MP_GUILD;
		message.Protocol		= MP_GUILD_DATA_READ;
		message.dwObjectID		= playerIndex;

		message.mIndex			= atoi( ( char* )record.Data[ 0 ] );
		message.mLevel			= atoi( ( char* )record.Data[ 1 ] );
		message.mUnionIndex		= atoi( ( char* )record.Data[ 2 ] );
		message.mScore			= atoi( ( char* )record.Data[ 3 ] );
		message.mLocation		= atoi( ( char* )record.Data[ 4 ] );
		message.mWarehouseSize	= atoi( ( char* )record.Data[ 5 ] );
		message.mWarehouseRank	= atoi( ( char* )record.Data[ 6 ] );
		
		const DWORD masterIndex	= atoi( ( char* )record.Data[ 7 ] );

		SafeStrCpy( message.mName,	( char* )record.Data[ 8 ], sizeof( message.mName ) );
		SafeStrCpy( message.mMark,	( char* )record.Data[ 9 ], sizeof( message.mMark ) );
		SafeStrCpy( message.mIntro, ( char* )record.Data[ 10 ], sizeof( message.mIntro ) );
		SafeStrCpy( message.mNotice, ( char* )record.Data[ 11 ], sizeof( message.mNotice ) );
		
		guildIndex = message.mIndex;

		{
			CGuildScore* guild = GUILDMGR->GetGuildScore( guildIndex );
			ASSERT( guild );
			
			if( guild )
			{
				guild->SetData( message, masterIndex );
			}
		}

		{
			CGuild* guild = GUILDMGR->GetGuild( guildIndex );
			ASSERT( guild );
			
			if( guild )
			{
				guild->GetTotalMember( &message.mMember[ 0 ] );

				message.mMemberSize	= guild->GetMemberNum();
			}
			else
			{
				message.mMemberSize	= 0;
			}
		}

		object->SendMsg( &message, message.GetSize() );
	}
}


void GuildSetLevel( DWORD guildIndex, DWORD playerIndex, DWORD requiredMoney, DWORD requiredScore, DWORD requiredQuest, DWORD warehouseSize, DWORD level )
{
	g_DB.FreeMiddleQuery(
		RGuildSetLevel,
		playerIndex,
		"EXEC DBO.MP_GUILD_LEVEL_UPDATE %d, %d, %d, %d, %d, %d, %d",
		guildIndex,
		playerIndex,
		requiredMoney,
		requiredScore,
		requiredQuest,
		warehouseSize,
		level,
		playerIndex);
}


void RGuildSetLevel( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{	
	CPlayer* player = ( CPlayer* )g_pUserTable->FindUser( dbMessage->dwID );

	// 길드 점수가 부족했을 때, 퀘스트 조건이 맞지 않을때 실패한다
	if( ! dbMessage->dwResult )
	{
		if( player )
		{
			MSGBASE message;
			message.Category	= MP_GUILD;
			message.Protocol	= MP_GUILD_LEVELUP_NACK;
			
			player->SendMsg( &message, sizeof( message ) );
		}
		
		return;
	}

	const MIDDLEQUERYST& record	= query[ 0 ];

	MSG_GUILD_LEVEL_UP_ACK message;
	{
		message.Category	= MP_GUILD;
		message.Protocol	= MP_GUILD_LEVELUP_NOTIFY_TO_MAP;
		message.dwObjectID	= 0;
		message.mGuildIndex	= atoi( ( char* )record.Data[ 0 ] );
		message.mGuildScore	= atoi( ( char* )record.Data[ 1 ] );
		message.mUsedMoney	= atoi( ( char* )record.Data[ 2 ] );
		
		if( player )
		{
			player->SetMoney( message.mUsedMoney, MONEY_SUBTRACTION, MF_LOST, eItemTable_Inventory, 0, 0  );
		}

		// 해당 맵 서버를 갱신한다
		GUILDMGR->NetworkMsgParse( message.Protocol, ( void* )&message );

		// 다른 맵 서버도 갱신해야 한다
		g_Network.Send2AgentServer( ( char* )&message, sizeof( message ) );
	}

	// 로그
	{
		const float losingScore = float( atof( ( char* )record.Data[ 3 ] ) );

		InsertLogGuildScore( message.mGuildIndex, losingScore, eGuildLog_ScoreLevelUp );
	}
}


void GuildResetWarehouse( MAPTYPE mapIndex )
{
	g_DB.FreeMiddleQuery(
		0,
		0,
		"EXEC DBO.MP_GUILD_WAREHOUSE_RESET %d",
		mapIndex );
}


void GuildOpenWarehouse( DWORD guildIndex, DWORD playerIndex, MAPTYPE mapIndex )
{
	g_DB.FreeMiddleQuery(
		RGuildOpenWarehouse,
		playerIndex,
		"EXEC DBO.MP_GUILD_WAREHOUSE_OPEN %d, %d, %d",
		guildIndex,
		mapIndex,
		playerIndex );
}


void RGuildOpenWarehouse( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD playerIndex = dbMessage->dwID;
	CObject*	object		= g_pUserTable->FindUser( playerIndex );

	if( !	object ||
			dbMessage->dwResult != 1 )
	{
		ASSERT( 1 == dbMessage->dwResult );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	MSG_GUILD_OPEN_WAREHOUSE message;

	{
		message.Category		= MP_GUILD;
		message.mWarehouseSize	= USHORT(	atoi( ( char* )record.Data[ 0 ] ) );
		message.mWarehouseRank	= BYTE(		atoi( ( char* )record.Data[ 1 ] ) );
		message.mMoney			= atoi( ( char* )record.Data[ 2 ] );
		
		SafeStrCpy( message.mName, ( char* )record.Data[ 3 ], sizeof( message.mName ) );
	}

	const DWORD		openedPlayerIndex	= atoi( ( char* )record.Data[ 4 ] );
	const DWORD		guildIndex			= atoi( ( char* )record.Data[ 5 ] );

	CGuildScore*	guild				= GUILDMGR->GetGuildScore( guildIndex );

	if( ! guild )
	{
		ASSERT( 0 );
		return;		
	}

	// 길드 창고를 열었으면 아이템 정보도 가져온다. 일단 옵션 읽고 아이템 정보 읽어야한다
	if( message.mWarehouseSize &&
		playerIndex == openedPlayerIndex )
	{
		guild->SetWarehouseUser( playerIndex );
		
		GuildItemSelect( playerIndex, guildIndex, 0 );

		message.Protocol = MP_GUILD_OPEN_WAREHOUSE_ACK;
	}
	else
	{
		message.Protocol = MP_GUILD_OPEN_WAREHOUSE_NACK;
	}

	object->SendMsg( &message, sizeof( message ) );
}


void GuildCloseWarehouse( DWORD guildIndex, DWORD playerIndex, MAPTYPE mapIndex )
{
	g_DB.FreeMiddleQuery(
		RGuildCloseWarehouse,
		guildIndex,
		"EXEC DBO.MP_GUILD_WAREHOUSE_CLOSE %d, %d, %d",
		guildIndex,
		mapIndex,
		playerIndex );
}


void RGuildCloseWarehouse( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	// 길드마다 창고는 한명만 사용 가능하므로, 창고가 닫히면 다시 접근할 수 있도록 초기화해준다
	const DWORD guildIndex = dbMessage->dwID;

	CGuildScore* guild = GUILDMGR->GetGuildScore( guildIndex );

	if( guild )
	{
		guild->SetWarehouseUser( 0 );
	}
}


void GuildItemSelect( DWORD playerIndex, DWORD guildIndex, DWORD beginItemDbIndex )
{
	sprintf( txt, "EXEC DBO.MP_GUILD_ITEM_SELECT %d, %d", guildIndex, beginItemDbIndex );

	g_DB.FreeQuery( eGuildItemSelect, playerIndex, txt, playerIndex );
}


void RGuildItemSelect( LPQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD playerIndex = dbMessage->dwID;
	CPlayer*	player		= ( CPlayer* )g_pUserTable->FindUser( playerIndex );

	if( ! player )
	{
		return;
	}

	if( SLOT_GUILDWAREHOUSE_NUM < dbMessage->dwResult )
	{
		ASSERT( 0 );
		return;
	}

	CGuild* guild = GUILDMGR->GetGuild( player->GetGuildIdx() );

	if( ! guild )
	{
		return;
	}

	guild->ClearItem();

	MSG_ITEM_OPTION optionMessage;
	{
		ZeroMemory( &optionMessage, sizeof( optionMessage ) );

		optionMessage.Category	= MP_ITEM;
		optionMessage.Protocol	= MP_ITEM_OPTION_ADD_ACK;
	}

	MSG_GUILD_ITEM message;
	{
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_GUILD;
		message.Protocol	= MP_GUILD_ITEM_READ;
		message.mSize		= dbMessage->dwResult;
	}

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		const QUERYST&	record	= query[ i ];
		ITEMBASE&		item	= message.mItem[ i ];

		item.dwDBIdx	= atoi( ( char* )record.Data[ 1 ] );
		item.wIconIdx	= atoi( ( char* )record.Data[ 2 ] );
		item.Position	= atoi( ( char* )record.Data[ 3 ] );
		item.Durability	= atoi( ( char* )record.Data[ 5 ] );
		item.nSealed	= atoi( ( char* )record.Data[ 7 ] );
		
		const ITEM_INFO* itemInfo = ITEMMGR->GetItemInfo( item.wIconIdx );

		if( ! itemInfo )
		{
			continue;
		}

		guild->InitGuildItem( item );

		// 옵션 로딩
		{
			const QUERYST& record = query[ i ];

			ITEM_OPTION& option = optionMessage.mOption[ optionMessage.mSize ];

			{
				ITEM_OPTION::Reinforce& data = option.mReinforce;

				data.mStrength			= atoi( ( char* )record.Data[ 10 ] );
				data.mDexterity			= atoi( ( char* )record.Data[ 11 ] );
				data.mVitality			= atoi( ( char* )record.Data[ 12 ] );
				data.mIntelligence		= atoi( ( char* )record.Data[ 13 ] );
				data.mWisdom			= atoi( ( char* )record.Data[ 14 ] );
				data.mLife				= atoi( ( char* )record.Data[ 15 ] );
				data.mMana				= atoi( ( char* )record.Data[ 16 ] );
				data.mLifeRecovery		= atoi( ( char* )record.Data[ 17 ] );
				data.mManaRecovery		= atoi( ( char* )record.Data[ 18 ] );
				data.mPhysicAttack		= atoi( ( char* )record.Data[ 19 ] );
				data.mPhysicDefence		= atoi( ( char* )record.Data[ 20 ] );
				data.mMagicAttack		= atoi( ( char* )record.Data[ 21 ] );
				data.mMagicDefence		= atoi( ( char* )record.Data[ 22 ] );
				data.mMoveSpeed			= atoi( ( char* )record.Data[ 23 ] );
				data.mEvade				= atoi( ( char* )record.Data[ 24 ] );
				data.mAccuracy			= atoi( ( char* )record.Data[ 25 ] );
				data.mCriticalRate		= atoi( ( char* )record.Data[ 26 ] );
				data.mCriticalDamage	= atoi( ( char* )record.Data[ 27 ] );
				SafeStrCpy( option.mReinforce.mMadeBy, ( char* )record.Data[ 28 ], sizeof( option.mReinforce.mMadeBy ) );				
			}

			{
				ITEM_OPTION::Mix& data = option.mMix;

				data.mStrength		= atoi( ( char* )record.Data[ 29 ] );
				data.mIntelligence	= atoi( ( char* )record.Data[ 30 ] );
				data.mDexterity		= atoi( ( char* )record.Data[ 31 ] );
				data.mWisdom		= atoi( ( char* )record.Data[ 32 ] );
				data.mVitality		= atoi( ( char* )record.Data[ 33 ] );
				SafeStrCpy( option.mMix.mMadeBy, ( char* )record.Data[ 34 ], sizeof( option.mMix.mMadeBy ) );				
			}

			{
				ITEM_OPTION::Enchant& data = option.mEnchant;

				data.mIndex	= atoi( ( char* )record.Data[ 35 ] );
				data.mLevel	= atoi( ( char* )record.Data[ 36 ] );
				SafeStrCpy( option.mEnchant.mMadeBy, ( char* )record.Data[ 37 ], sizeof( option.mEnchant.mMadeBy ) );
			}

			{
				ITEM_OPTION::Drop& data = option.mDrop;

				data.mValue[ 0 ].mKey	= ITEM_OPTION::Drop::Key( atoi( ( char* )record.Data[ 38 ] ) );
				data.mValue[ 0 ].mValue	= float( atof( ( char* )record.Data[ 39 ] ) );

				data.mValue[ 1 ].mKey	= ITEM_OPTION::Drop::Key( atoi( ( char* )record.Data[ 40 ] ) );
				data.mValue[ 1 ].mValue	= float( atof( ( char* )record.Data[ 41 ] ) );

				data.mValue[ 2 ].mKey	= ITEM_OPTION::Drop::Key( atoi( ( char* )record.Data[ 42 ] ) );
				data.mValue[ 2 ].mValue	= float( atof( ( char* )record.Data[ 43 ] ) );

				data.mValue[ 3 ].mKey	= ITEM_OPTION::Drop::Key( atoi( ( char* )record.Data[ 44 ] ) );
				data.mValue[ 3 ].mValue	= float( atof( ( char* )record.Data[ 45 ] ) );

				data.mValue[ 4 ].mKey	= ITEM_OPTION::Drop::Key( atoi( ( char* )record.Data[ 46 ] ) );
				data.mValue[ 4 ].mValue	= float( atof( ( char* )record.Data[ 47 ] ) );
			}

			const ITEM_OPTION emptyOption = { 0 };

			if( memcmp( &emptyOption, &option, sizeof( emptyOption ) ) )
			{
				option.mItemDbIndex	= item.dwDBIdx;
				ITEMMGR->AddOption( option );

				++optionMessage.mSize;
			}
		}

		// 081202 LUJ, 수량성 아이템의 개수가 0인 경우, 삭제한다
		if(		itemInfo->Stack &&
			!	item.Durability )
		{
			LogItemMoney(
				player->GetID(),
				player->GetObjectName(),
				0,
				"*invalid",
				eLog_ItemDiscard,
				player->GetMoney(),
				0,
				0,
				item.wIconIdx,
				item.dwDBIdx,
				item.Position,
				0,
				item.Durability,
				0 );
			ItemDeleteToDB( item.dwDBIdx );
			ZeroMemory( &item, sizeof( item ) );
		}
	}

	player->SendMsg( &optionMessage, optionMessage.GetSize() );
	player->SendMsg( &message, message.GetSize() );

	if( MAX_ROW_NUM == dbMessage->dwResult )
	{
		const QUERYST&	record			= query[ MAX_ROW_NUM - 1 ];
		const DWORD		lastItemDbIndex	= atoi( ( char* )record.Data[ 1 ] );

		GuildItemSelect( playerIndex, player->GetGuildIdx(), lastItemDbIndex );
	}
}


void GuildSetWarehouseRank( DWORD guildIndex, DWORD playerIndex, BYTE rank )
{
	g_DB.FreeMiddleQuery(
		RGuildSetWarehouseRank,
		playerIndex,
		"EXEC DBO.MP_GUILD_WAREHOUSE_RANK_UPDATE %d, %d, %d",
		guildIndex,
		playerIndex,
		rank, playerIndex );
}


void RGuildSetWarehouseRank( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	if( 1 != dbMessage->dwResult )
	{
		ASSERT( 0 );
		return;
	}

	CObject* object = g_pUserTable->FindUser( dbMessage->dwID );

	if( object )
	{
		const MIDDLEQUERYST& record = query[ 0 ];

		MSG_GUILD_SET_WAREHOUSE_RANK message;

		message.Category	= MP_GUILD;
        message.Protocol	= MP_GUILD_SET_WAREHOUSE_RANK_ACK;
		message.mRank		= atoi( ( char* )record.Data[ 0 ] );
        		
		object->SendMsg( &message, sizeof( message ) );
	}
}


void GuildChangeMaster(DWORD guildIndex, DWORD newMasterPlayerIndex, BYTE masterRank, BYTE exMasterRank )
{
	g_DB.FreeMiddleQuery(
		RGuildChangeMaster,
		guildIndex,
		 "EXEC DBO.MP_GUILD_CHANGE_MASTER %d, %d, %d, %d",
		 guildIndex,
		 newMasterPlayerIndex,
		 masterRank,
		 exMasterRank, newMasterPlayerIndex );
}


void RGuildChangeMaster( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	if( !	dbMessage->dwResult ||
		!	atoi( ( char* )query[ 0 ].Data[ 0 ] ) )
	{
		ASSERT( 0 );
		return;
	}

	const MIDDLEQUERYST&	record			= query[ 0 ];
	const DWORD				newMasterIndex	= atoi( ( char* )record.Data[ 0 ] );
	const DWORD				guildIndex		= dbMessage->dwID;
	
	{
		MSG_DWORD2 message;
		message.Category	= MP_GUILD;	
		message.Protocol	= MP_GUILD_CHANGE_MASTER_TO_AGENT;
		message.dwData1		= guildIndex;
		message.dwData2		= newMasterIndex;
		
		// 다른 맵서버 갱신을 요청한다.
		g_Network.Send2AgentServer( ( char* )&message, sizeof( message ) );

		// 해당 맵서버도 갱신해야한다.
		GUILDMGR->NetworkMsgParse( message.Protocol, ( void* )&message );
	}
}


void CharacterLogoutPointUpdate( DWORD characterIdx, WORD map, DWORD pos_x, DWORD pos_y )
{
	if( map == 0 )
		return;

	if( pos_x == 0 )
		return;

	if( pos_y == 0 )
		return;

	g_DB.FreeQuery(
		eCharacterLogoutPointUpdate,
		characterIdx,
		"EXEC DBO.MP_CHARACTER_LogoutPointUpdate %d, %d, %d, %d", characterIdx, map, pos_x, pos_y, characterIdx );
}

// 070917 LYW --- MapDBMsgParser : Add function to change map point to return result.
void RSaveNpcMapChangePointReturn(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD dwConnectionIndex = (DWORD)atoi((char*)pData->Data[0]);
	DWORD Map = (DWORD)atoi((char*)pData->Data[1]);

	MSG_DWORD msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_NPC_MAP_OUT; 
	msg.dwObjectID = pMessage->dwID;
	msg.dwData = Map;

	g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
}


void GuildSelectForRewardByWar( DWORD winnerGuildIndex, DWORD winnerUnionIndex, DWORD loserGuildIndex, DWORD loserUnionIndex )
{
	sprintf( txt, "EXEC DBO.MP_GUILDFIELDWAR_SELECT_FOR_REWARD %d, %d, %d, %d",
		winnerGuildIndex,
		winnerUnionIndex,
		loserGuildIndex,
		loserUnionIndex );

	g_DB.FreeQuery( eGuildSelectForReward, 0, txt );
}


void RGuildSelectForReward( LPQUERY query, LPDBMESSAGE dbMessage )
{
	if( ! dbMessage->dwResult )
	{
		return;
	}

	// 주의:
	// 점수를 얻은 맵이 아닌 다른 맵은 점수를 즉각적으로 갱신하지 않으므로 비동기 현상이 나타난다.
	// 이에 따라 변화된 정도(variation)도 함께 전달해줘야 한다.

	// first: 갱신할 점수값, second: 변화된 값
	typedef std::pair< float, float >	Score;

	// 키: 길드 인덱스, 값: 길드 점수
	typedef std::map< DWORD, Score >	Group;
	Group								winnerGroup;
	Group								loserGroup;

	// 각각을 그룹으로 만든다. 편의상 하나를 승자 그룹, 하나를 패자 그룹으로 나눈다.
	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		const QUERYST&	record		= query[ i ];
		const BOOL		isWinner	= atoi( ( char* )record.Data[ 0 ] );
		const DWORD		guildIndex	= atoi( ( char* )record.Data[ 1 ] );
		const float		score		= float( atof( ( char* )record.Data[ 2 ] ) );

		if( isWinner )
		{
			winnerGroup[ guildIndex ].first = score;
		}
		else
		{
			loserGroup[ guildIndex ].first	= score;
		}
	}

	float losedScore = 0;

	// 패자 그룹의 길드 점수를 규칙에 따라 차감하도록 DB에 요청한다.
	// 차감한 합계를 승리 그룹에게 더해줘야 하므로 차감한 합을 저장하자
	for( Group::iterator it = loserGroup.begin(); loserGroup.end() != it; ++it )
	{
		const DWORD guildIndex	= it->first;
		Score&		score		= it->second;
		//const float score		= it->second;
		const float losingScore = max( -1.0f * score.first, GUILDMGR->GetLosingScore( score.first ) );

		if( ! losingScore )
		{
			continue;
		}

		// losingScore가 음수이고, 뺄 값을 얻어야한다는 점을 유의하자. 가진 점수 이상 빼면 음수가 되는 것도 주의
		GuildAddScore( guildIndex, losingScore );

		// 071114 웅주, 로그
		InsertLogGuildScore( guildIndex, losingScore, eGuildLog_ScoreRemoveByDefeat );

		losedScore += losingScore;

		score.first		+= losingScore;
		score.second	= abs( losingScore );

		//loserGroup[ guildIndex ] = score + losingScore;
	}

	// 승자 그룹의 길드 점수를 규칙에 따라 더하도록 DB에 요청한다
	const CGuildManager::ScoreSetting&	setting		= GUILDMGR->GetScoreSetting();
	const float							addingScore	= abs( losedScore ) * setting.mWinScore.mPercent / winnerGroup.size();

	if( addingScore )
	{
		for( Group::iterator it = winnerGroup.begin(); winnerGroup.end() != it; ++it )
		{
			const DWORD guildIndex	= it->first;
			//const float score		= it->second;
			Score&		score		= it->second;

			GuildAddScore( guildIndex, addingScore );

			// 071114 웅주, 로그
			InsertLogGuildScore( guildIndex, addingScore, eGuildLog_ScoreAddByWin );

			//winnerGroup[ guildIndex ] = score + addingScore;

			score.first		+= addingScore;
			score.second	= addingScore;
		}
	}
	
	// 메시지 전송
	{
		MSG_DWORD4 messageToClient;
		messageToClient.Category = MP_GUILD_WAR;
		messageToClient.Protocol = MP_GUILD_WAR_SURRENDER;

		for( Group::const_iterator it = winnerGroup.begin(); winnerGroup.end() != it; ++it )
		{
			const DWORD		winnerGuildIndex	= it->first;
			//const DWORD winnerScore			= DWORD( it->second );
			const Score&	winnerScore			= it->second;
			CGuild*			winner				= GUILDMGR->GetGuild( winnerGuildIndex );

			if( ! winner )
			{
				ASSERT( 0 );
				continue;
			}

			for( Group::const_iterator inner = loserGroup.begin(); loserGroup.end() != inner; ++inner )
			{
				const DWORD		loserGuildIndex	= inner->first;
				//const DWORD loserScore		= DWORD( inner->second );
				const Score&	loserScore		= inner->second;
				CGuild*			loser			= GUILDMGR->GetGuild( loserGuildIndex );

				if( ! loser )
				{
					ASSERT( 0 );
					continue;
				}

				{
					messageToClient.dwData1 = 0;	// 0 = surrender
					messageToClient.dwData2 = winnerGuildIndex;
					//messageToClient.dwData3	= loserScore;
					messageToClient.dwData3	= DWORD( loserScore.first );	// 바뀔 값
					messageToClient.dwData4	= DWORD( loserScore.second );	// 잃은 정도

					loser->SendMsgToAll( &messageToClient, sizeof( messageToClient ) );
				}

				{
					messageToClient.dwData1 = 1;	// 1 = win
					messageToClient.dwData2 = loserGuildIndex;
					//messageToClient.dwData3	= winnerScore;
					messageToClient.dwData3	= DWORD( winnerScore.first );	// 바뀔 값
					messageToClient.dwData4	= DWORD( winnerScore.second );	// 얻은 정도

					winner->SendMsgToAll( &messageToClient, sizeof( messageToClient ) );
				}

				{
					MSG_GUILD_WAR_RESULT message;
					message.Category	= MP_GUILD_WAR;
					message.Protocol	= MP_GUILD_WAR_SURRENDER_NOTIFY_TOMAP;

					{
						MSG_GUILD_WAR_RESULT::Data& data = message.mWinner;

						data.mGuildIndex		= winnerGuildIndex;
						data.mScoreValue		= DWORD( winnerScore.first );
						data.mScoreVariation	= winnerScore.second;
					}

					{
						MSG_GUILD_WAR_RESULT::Data& data = message.mLoser;

						data.mGuildIndex		= loserGuildIndex;
						data.mScoreValue		= DWORD( loserScore.first );
						data.mScoreVariation	= loserScore.second;
					}

					g_Network.Send2AgentServer( ( char* )&message, sizeof( message ) );
				}

				DeleteGuildWar( winnerGuildIndex, loserGuildIndex );
				GUILDWARMGR->UnregistGuildWar( loserGuildIndex, winnerGuildIndex );

				GUILDWARMGR->UpdateGuildWarRecord( TRUE, 0, loserGuildIndex );
				GUILDWARMGR->UpdateGuildWarRecord( TRUE, 1, winnerGuildIndex );

				//GUILDWARMGR->SendToAllUser( 2, winner->GetGuildName(), loser->GetGuildName() );
			}
		}
	}
}

// 071023 LYW --- MapDBMsgParser : Add function pointer for tutorial.
void RTutorialLoad(LPQUERY pData, LPDBMESSAGE pMessage)
{
	SEND_TUTORIAL_DATA msg;													// 튜토리얼 데이터를 담을 구조체를 선언한다.
	memset(&msg.TutorialList, 0, sizeof(msg.TutorialList));					// 튜토리얼 데이터 구조체를 초기화 한다.

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);	// 넘어온 메시지 아이디로 플레이어 정보를 받는다.

	if( !pPlayer ) return;													// 플레이어 정보가 유효하지 않으면, return 처리를 한다.

	msg.Category = MP_TUTORIAL ;											// 카테고리를 튜토리얼로 세팅한다.				
	msg.Protocol = MP_TUTORIAL_MAINDATALOAD ;								// 프로토콜을 튜토리얼 메인 데이터 로딩으로 세팅한다.
	msg.dwObjectID = pMessage->dwID ;										// 오브젝트 아이디를 세팅한다.

	//if( pMessage->dwResult == 0 ) return ;									// 메시지의 결과 값이 0이면 return 처리를 한다.

	int		nTutorialIdx ;													// 튜토리얼의 인덱스를 담을 변수를 선언한다.
	BOOL	IsComplete ;													// 완료 여부를 담을 변수를 선언한다.

	DWORD	count ;															// for문을 돌리기 위한 카운트를 선언한다.

	for( count = 0 ; count < pMessage->dwResult ; ++count )					// 메시지의 결과값보다 count 가 작을 동안 for문을 돌린다.
	{	
		nTutorialIdx = 0 ;													// 튜토리얼 인덱스를 0으로 세팅한다.
		nTutorialIdx = (int)atoi((char*)pData[count].Data[eTutorial_Idx]) ;	// 튜토리얼 인덱스를 받는다.

		IsComplete = FALSE ;												// 완료여부를 false로 세팅한다.
		IsComplete = (BOOL)atoi((char*)pData[count].Data[eTutorial_IsComplete]) ;	// 완료 여부를 받는다.

		msg.TutorialList[count].nTutorialIdx = nTutorialIdx ;				// 메시지에 튜토리얼 인덱스를 세팅한다.
		msg.TutorialList[count].IsComplete	 = IsComplete ;					// 메시지에 완료 여부를 세팅한다.
	}

	msg.wCount = (WORD)count;												// 메시지의 카운트를 세팅한다.

	pPlayer->SendMsg( &msg, msg.GetSize() );								// 플레이어에게 메시지를 전송한다.

	//---KES 퀘스트 DB 결과 완료
	pPlayer->SetInitState( PLAYERINITSTATE_QEUST_INFO, MP_USERCONN_GAMEIN_SYN );
}

// 071023 LYW --- MapDBMsgParser : 튜토리얼 업데이트 쿼리문 처리에 대한 결과 처리를 하기 위한 함수.
void RTutorialUpdate(LPQUERY pData, LPDBMESSAGE pMessage)
{
	SEND_TUTORIAL_DATA msg;													// 튜토리얼 데이터를 담을 구조체를 선언한다.
	memset(&msg.TutorialList, 0, sizeof(msg.TutorialList));					// 튜토리얼 데이터 구조체를 초기화 한다.

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);	// 넘어온 메시지 아이디로 플레이어 정보를 받는다.

	if( !pPlayer ) return;													// 플레이어 정보가 유효하지 않으면, return 처리를 한다.

	msg.Category = MP_TUTORIAL ;											// 카테고리를 튜토리얼로 세팅한다.				
	msg.Protocol = MP_TUTORIAL_UPDATE_ACK ;									// 프로토콜을 튜토리얼 메인 데이터 로딩으로 세팅한다.
	msg.dwObjectID = pMessage->dwID ;										// 오브젝트 아이디를 세팅한다.

	if( pMessage->dwResult == 0 ) return ;									// 메시지의 결과 값이 0이면 return 처리를 한다.

	int		nTutorialIdx ;													// 튜토리얼의 인덱스를 담을 변수를 선언한다.
	BOOL	IsComplete ;													// 완료 여부를 담을 변수를 선언한다.

	DWORD	count ;															// for문을 돌리기 위한 카운트를 선언한다.

	for( count = 0 ; count < pMessage->dwResult ; ++count )					// 메시지의 결과값보다 count 가 작을 동안 for문을 돌린다.
	{	
		nTutorialIdx = 0 ;													// 튜토리얼 인덱스를 0으로 세팅한다.
		nTutorialIdx = (int)atoi((char*)pData[count].Data[eTutorial_Idx]) ;	// 튜토리얼 인덱스를 받는다.

		IsComplete = FALSE ;												// 완료여부를 false로 세팅한다.
		IsComplete = (BOOL)atoi((char*)pData[count].Data[eTutorial_IsComplete]) ;	// 완료 여부를 받는다.

		msg.TutorialList[count].nTutorialIdx = nTutorialIdx ;				// 메시지에 튜토리얼 인덱스를 세팅한다.
		msg.TutorialList[count].IsComplete	 = IsComplete ;					// 메시지에 완료 여부를 세팅한다.
	}

	msg.wCount = (WORD)count;												// 메시지의 카운트를 세팅한다.

	pPlayer->SendMsg( &msg, msg.GetSize() );	
}






// 071207 LYW --- MapDBMsgParser : 스탯 초기화 프로시저 실행 결과 함수.
void RUpdateToDefaultStats(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);	// 넘어온 메시지 아이디로 플레이어 정보를 받는다.

	if( !pPlayer ) return;													// 플레이어 정보가 유효하지 않으면, return 처리를 한다.

	// 071207 LYW --- MapDBMsgParser : 캐릭터 로그아웃 처리.
	pPlayer->UpdateLogoutToDB();
	PARTYMGR->UserLogOut(pPlayer);
	FRIENDMGR->UserLogOut(pPlayer->GetID());
	GUILDMGR->UserLogOut(pPlayer);
	//GUILDMGR->RemovePlayer( pPlayer->GetGuildIdx(), pPlayer->GetID() );

	SaveCharInfoBeforeLogout(pPlayer->GetID(), pPlayer->GetAgentNum(), pPlayer->GetUserID(), pPlayer->GetMoney(eItemTable_Storage));
}





// 071129 LYW --- MapDBMsgParser : 스킬 초기화 프로시저 실행 결과 함수.
// 080408 LUJ, 보유 중인 버프 스킬을 해제하도록 수정
void RSkill_Reset(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);	// 넘어온 메시지 아이디로 플레이어 정보를 받는다.

	if( !pPlayer ) return;													// 플레이어 정보가 유효하지 않으면, return 처리를 한다.

	if( pMessage->dwResult == 0 ) return ;									// 결과 값이 0과 같으면, return 처리를 한다.

	DWORD dwPlayerID = 0 ;													// 플레이어의 아이디를 담을 변수를 선언하고 0으로 세팅한다.
	dwPlayerID = pMessage->dwID ;											// 플레이어 아이디를 받는다.

	// 080722 LYW --- MapDBMsgParser : 유료 버프를 제외한 모든 버프를 해제한다.
	// 080408 LUJ,	보유했던 스킬이 버프되어 있을 경우 해제한다.
	//{
	//	cSkillTree*					skillTree		= pPlayer->GetSkillTree();
	//	CYHHashTable<cSkillObject>* buffSkillTable	= pPlayer->GetBuffSkillList();

	//	if( skillTree && buffSkillTable )
	//	{
	//		skillTree->SetPositionHead();

	//		// 080407 LUJ, 보유했던 버프 스킬을 별도의 컨테이너에 담는다
	//		for(
	//			SKILL_BASE* skill = 0;
	//			skill = skillTree->GetSkillBase(); )
	//		{
	//			cSkillInfo* skillInfo = SKILLMGR->GetSkillInfo( skill->wSkillIdx );

	//			if( ! skillInfo )
	//			{
	//				continue;
	//			}

	//			const ACTIVE_SKILL_INFO* activeSkillInfo = ( ( cActiveSkillInfo* )skillInfo )->GetSkillInfo();

	//			if( ! activeSkillInfo )
	//			{
	//				continue;
	//			}

	//			cSkillObject * pSObj ;

	//			for( DWORD i = 0; i < MAX_BUFF_COUNT; ++i )
	//			{
	//				const DWORD buffSkillIndex = activeSkillInfo->Buff[ i ];

	//				// 080717 LYW --- MapDBMsgParser : 스킬 초기화 시, 토글형 스킬이 남아있는 현상 수정.
	//				// SetEndState()를 하지 않아서, EndState() 시 활성화 중이던 스킬들이 업데이트가 되는 문제가 있다.
	//				// SetEndState() 호출 추가.
	//				if(buffSkillIndex == 0) continue ;

	//				buffSkillTable->SetPositionHead() ;
	//				while(pSObj = (cSkillObject*)buffSkillTable->GetData())
	//				{
	//					if(pSObj->GetSkillIdx() != buffSkillIndex) continue ;

	//					pSObj->SetEndState() ;
	//					pSObj->EndState();

	//					break ;
	//				}				
	//			}
	//		}
	//	}	
	//}

	// 버프 스킬 해제 처리.(남겨둬야 할 스킬만 남겨둔다.)
	cSkillObject * pSObj = NULL;
	cPtrList templist;

	CYHHashTable<cSkillObject>* buffSkillTable	= pPlayer->GetBuffSkillList();

	buffSkillTable->SetPositionHead();
	while( pSObj = buffSkillTable->GetData() )
	{
		if( ( ( cBuffSkillInfo* )( pSObj->GetSkillInfo() ) )->GetSkillInfo()->Die == 0 )
		{
			pSObj->SetEndState();
			pSObj->EndState();
			templist.AddTail( pSObj );
		}
	}

	PTRLISTPOS pos = templist.GetHeadPosition();
	while( pos )
	{
		pSObj = ( cSkillObject* )templist.GetNext( pos );
		buffSkillTable->Remove( pSObj->GetSkillIdx() );
	}
	templist.RemoveAll();






	cSkillTree* pSkillTree = NULL ;											// 스킬 트리 포인터를 선언하고 null처리를 한다.
	pSkillTree = pPlayer->GetSkillTree() ;									// 스킬 트리를 받는다.

	ASSERT( pSkillTree ) ;													// 스킬트리를 체크한다.
	if( !pSkillTree ) return;

	pSkillTree->RemoveAllSkill() ;											// 현재 스킬 정보를 모두 비운다.

	SKILL_BASE* pSkillBase ;												// 스킬 정보를 담을 포인터를 선언한다.

	DWORD dwCount = 0 ;														// 결과 카운트를 담을 변수를 선언하고 0으로 세팅한다.
	dwCount = pMessage->dwResult ;											// 결과 카운트를 받는다.

	int nSkillPoint = 0 ;													// 스킬 포인트를 받을 변수선언.

	for( DWORD count = 0 ; count < dwCount ; ++count )						// 결과 카운트 만큼 for문을 돌린다.
	{
		pSkillBase = NULL ;													// 스킬 정보 포인터를 null처리 한다.
		pSkillBase = pSkillTree->NewSkillBase() ;							// 스킬 정보 포인터를 받는다.

		pSkillBase->dwDBIdx		= atoi((char*)pData[count].Data[0]) ;		// 스킬 DB인덱스를 세팅한다.
		pSkillBase->wSkillIdx	= atoi((char*)pData[count].Data[1]) ;		// 스킬 인덱스를 세팅한다.
		pSkillBase->Level= BYTE( atoi((char*)pData[count].Data[2]) ) ;		// 스킬 레벨을 세팅한다.
		nSkillPoint = (int)atoi((char*)pData[count].Data[3]) ;				// 스킬 포인트를 받는다.

		pSkillTree->AddSkillBase( pSkillBase ) ;							// 스킬 트리에 스킬 정보를 추가한다.
	}

	//SEND_SKILL_RESETINFO msg ;												// 결과 전송용 메시지 구조체를 선언한다.
	//memset(&msg, 0, sizeof(SEND_SKILL_RESETINFO)) ;							// 메시지 구조체를 초기화 한다.

	//msg.Category	= MP_SKILLTREE ;										// 카테고리를 스킬트리로 세팅한다.
	//msg.Protocol	= MP_SKILLTREE_RESET_SKILL_ACK ;						// 프로토콜을 스킬초기화 성공으로 세팅한다.
	//msg.dwObjectID	= dwPlayerID ;											// 플레이어 아이디를 세팅한다.

	//SKILL_BASE SkillTreeInfo[ 1024 ] ;										// 스킬 정보 배열을 선언한다.
	//WORD SkillNum = 0 ;														// 스킬 수를 담을 변수를 선언한다.

	//pSkillTree->SetPositionHead() ;											// 스킬 트리를 해드로 세팅한다.

	//while( SKILL_BASE* pSkill = pSkillTree->GetSkillBase() )				// 스킬 정보 수 만큼 for문을 돌린다.
	//{
	//	memcpy( &SkillTreeInfo[ SkillNum ], pSkill, sizeof( SKILL_BASE ) ) ;// 스킬 정보를 복사한다.
	//	SkillNum++ ;														// 스킬 카운트를 증가한다.

	//	if( SkillNum >= 1024 )												// 스킬 카운트가 1024를 넘으면 break ;
	//		break ;
	//}

	//if( SkillNum )															// 스킬 정보가 유효하면, 스킬 정보를 담는다.
	//{
	//	msg.AddableInfo.AddInfo(
	//								CAddableInfoList::SkillTree
	//							,	sizeof( SKILL_BASE ) * SkillNum
	//							,	SkillTreeInfo
	//							) ;
	//}

	//msg.SkillNum = SkillNum ;												// 스킬 수를 세팅한다.
	//msg.nSkillPoint = nSkillPoint ;											// 스킬 포인트를 세팅한다.

	pPlayer->GetHeroTotalInfo()->SkillPoint = nSkillPoint ;					// 플레이어의 스킬 포인트를 세팅한다.

	//pPlayer->SendMsg( &msg, sizeof(SEND_SKILL_RESETINFO) ) ; 				// 처리 결과를 전송한다.

	for( int count = 0 ; count < 8 ; ++count )
	{
		for( int count2 = 0 ; count2 < 10 ; ++count2 )
		{
			SLOT_INFO* pSlot = NULL ;
			pSlot = pPlayer->GetQuick(count, count2) ;

			if( !pSlot ) continue ;

			if( pSlot->kind == QuickKind_Skill )
			{
				QUICKMNGR->RemoveQuickInfo( pPlayer, count, count2 ) ;
			}
		}
	}

	//---KES 스킬 습득 돈 돌려주기
	pPlayer->SetMoney( pPlayer->GetReturnSkillMoney(), MONEY_ADDITION, MF_NOMAL );
	pPlayer->SetRetrunSkillMoney( 0 );
	//----------------------------

	// 071206 KTH -- 스킬이 업데이트 될수 있도록 업데이트 해준다.
	pPlayer->SetResetSkill(false);

	//CHARCALCMGR->Initialize(pPlayer) ;

	// 071207 LYW --- MapDBMsgParser : 캐릭터 로그아웃 처리.
	pPlayer->UpdateLogoutToDB();
	PARTYMGR->UserLogOut(pPlayer);
	FRIENDMGR->UserLogOut(dwPlayerID);
	GUILDMGR->UserLogOut(pPlayer);
	//GUILDMGR->RemovePlayer( pPlayer->GetGuildIdx(), pPlayer->GetID() );

	SaveCharInfoBeforeLogout(pPlayer->GetID(), pPlayer->GetAgentNum(), pPlayer->GetUserID(), pPlayer->GetMoney(eItemTable_Storage));
}

// 080602 LUJ, 스킬 사용 시 소모 조건이 부합되는지 DB에서 검사하고 값을 바꾼 상태이다.
//				DB에서 성공으로 전달되었다면 서버 메모리만 갱신하자.
void RSkillCheckUsable( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	// 080602 LUJ, 검사 등이 실패 시 반드시 호출되어야 하는 함수
	struct
	{
		void operator() ( CPlayer& player, DWORD skillObjectIndex, DWORD skillIndex, DWORD itemDbIndex ) const
		{
			cActiveSkillObject* skillObject = ( cActiveSkillObject* )SKILLMGR->GetSkillObject( skillObjectIndex );

			if( skillObject &&
				skillObject->GetType() == cSkillObject::TypeActive )
			{
				skillObject->RemoveAllBuffSkillObject();
				
				MSG_DWORD message;
				ZeroMemory( &message, sizeof( message ) );
				message.Category	= MP_SKILL;
				message.Protocol	= MP_SKILL_CANCEL_SYN;
				message.dwObjectID	= player.GetID();
				message.dwData		= skillObjectIndex;

				SKILLMGR->NetworkMsgParse( message.Protocol, &message );
			}

			// 080602 LUJ, 아이템 잠금을 풀기 위해 꼭 전송해야 한다
			{
				MSG_DWORD2 message;
				ZeroMemory( &message, sizeof( message ) );
				message.Category	= MP_SKILL;
				message.Protocol	= MP_SKILL_CONSUME_NACK;
				message.dwData1		= skillIndex;
				message.dwData2		= itemDbIndex;

				player.SendMsg( &message, sizeof( message ) );
			}
		}
	}
	CancelSkill;

	const DWORD				skillObjectIndex	= dbMessage->dwID;
	const MIDDLEQUERYST&	record				= query[ 0 ];
	const DWORD				playerIndex			=			atoi( ( char* )record.Data[ 0 ] );
	const DWORD				skillIndex			= 			atoi( ( char* )record.Data[ 1 ] );
	const DWORD				itemDbIndex			=			atoi( ( char* )record.Data[ 2 ] );
	const POSTYPE			itemPosition		= POSTYPE(	atoi( ( char* )record.Data[ 3 ] ) );
	
	CPlayer* player = ( CPlayer* )g_pUserTable->FindUser( playerIndex );

	if( !	player ||
			player->GetObjectKind() != eObjectKind_Player )
	{
		return;
	}
	// 080602 LUJ, 스킬 인덱스가 없음은 스킬이 실패했다는 뜻
	else if( ! skillIndex )
	{
		CancelSkill( *player, skillObjectIndex, skillIndex, itemDbIndex );
		return;
	}

	const ACTIVE_SKILL_INFO* skillInfo = 0;
	{
		cActiveSkillInfo* info = ( cActiveSkillInfo* )SKILLMGR->GetSkillInfo( skillIndex );

		if( ! info )
		{
			return;
		}

		skillInfo = info->GetSkillInfo();

		if( ! skillInfo )
		{
			return;
		}
	}
	
	// 080602 LUJ, 포인트 소모 처리. 메모리만 갱신하자
	{
		const ACTIVE_SKILL_INFO::ConsumePoint& point = skillInfo->mConsumePoint;

		switch( point.mType )
		{
		case ACTIVE_SKILL_INFO::ConsumePoint::TypeExperience:
			{
				player->ReduceExpPoint( EXPTYPE( point.mValue ), eExpLog_LoseBySkillUse );
				break;
			}
		case ACTIVE_SKILL_INFO::ConsumePoint::TypeGuild:
			{
				// 080602 LUJ, 0점을 더하면 새로고침과 동일하다
				GuildAddScore( player->GetGuildIdx(), 0 );

				InsertLogGuildScore(
					player->GetGuildIdx(),
					-1.0f * point.mValue,
					eGuildLog_ScoreSkillUse );

				// 080602 LUJ, 길드 포인트 변경은 전 서버에 반영되어야 한다. 그렇지 않으면 클라이언트에서는 
				//				충분한 점수가 있는 것처럼 보여도, 서버에서 사용 불가능한 경우가 생긴다
				{
					MSGBASE message;
					ZeroMemory( &message, sizeof( message ) );
					message.Category	= MP_GUILD;
					message.Protocol	= MP_GUILD_SCORE_UPDATE_TO_MAP;
					message.dwObjectID	= player->GetGuildIdx();

					g_Network.Broadcast2AgentServer( ( char* )&message, sizeof( message ) );
				}

				break;
			}
		case ACTIVE_SKILL_INFO::ConsumePoint::TypeFamily:
			{
				MSG_DWORD message;
				ZeroMemory( &message, sizeof( message ) );
				message.Category	= MP_FAMILY;
				message.Protocol	= MP_FAMILY_HONOR_POINT;
				// 080602 LUJ, 주의! 에이전트에서 int로 캐스팅해서 가감처리를 한다. 개선이 필요하나 서비스 코드라서 건드리지는 않겠음...
				message.dwData		= -1 * point.mValue;

				player->SendMsg( &message, sizeof( message ) );

				// 080602 LUJ, 클라이언트측 점수 변경은 에이전트에서 처리한다. 별도로 재전송하지 않는다.
				break;
			}
		case ACTIVE_SKILL_INFO::ConsumePoint::TypePlayerKill:
			{
				// 080602 LUJ, 이미 DB에는 변경했으므로 메모리만 바꾸자
				player->SetBadFame( player->GetBadFame() - point.mValue );

				// 080602 LUJ, PK는 로그가 없다...
				break;
			}
		}
	}

	// 080602 LUJ, 소지금 소모 처리
	if( skillInfo->Money )
	{
		player->SetMoney(
			skillInfo->Money,
			MONEY_SUBTRACTION,
			MF_LOST );

		LogItemMoney(
			player->GetID(),
			player->GetObjectName(),
			skillInfo->Index,
			"*skill",
			eLog_SkillMoneyUse,
			player->GetMoney() + skillInfo->Money,
			player->GetMoney(),
			skillInfo->Money,
			0,
			0,
			0,
			0,
			0,
			0 );
	}

	// 080602 LUJ, 아이템 소모 처리
	if( skillInfo->mConsumeItem.wIconIdx )
	{
		CItemSlot* slot = player->GetSlot( eItemTable_Inventory );

		if( ! slot )
		{
			return;
		}

		const ITEMBASE&	requiredItem	= skillInfo->mConsumeItem;
		const ITEMBASE* serverItem		= slot->GetItemInfoAbs( itemPosition );

		// 080602 LUJ, 아이템 개수를 변경한다
		if(	ITEMMGR->IsDupItem( serverItem->wIconIdx ) &&
			requiredItem.Durability < serverItem->Durability )
		{
			if( EI_TRUE != slot->UpdateItemAbs(
				player,
				serverItem->Position,
				serverItem->dwDBIdx,
				serverItem->wIconIdx,
				serverItem->Position,
				serverItem->QuickPosition,
				serverItem->Durability - requiredItem.Durability ) )
			{
				return;
			}

			LogItemMoney(
				player->GetID(),
				player->GetObjectName(),
				skillIndex,
				"*skill",
				eLog_SkillItemUse,
				player->GetMoney(),
				0,
				0,
				serverItem->wIconIdx,
				serverItem->dwDBIdx,
				serverItem->Position,
				0,
				serverItem->Durability,
				0 );
		}
		// 080602 LUJ, 아이템을 삭제한다
		else
		{
			ITEMBASE deletedItem;
			ZeroMemory( &deletedItem, sizeof( deletedItem ) );

			if( EI_TRUE != slot->DeleteItemAbs(
				player,
				serverItem->Position,
				&deletedItem ) )
			{
				return;
			}

			LogItemMoney(
				player->GetID(),
				player->GetObjectName(),
				skillIndex,
				"*skill",
				eLog_SkillItemUse,
				player->GetMoney(),
				0,
				0,
				deletedItem.wIconIdx,
				deletedItem.dwDBIdx,
				deletedItem.Position,
				0,
				deletedItem.Durability,
				0 );
		}
	}

	// 080602 LUJ, 스킬 성공 시 소모를 위해 전달한다
	{
		MSG_DWORD2 message;
		ZeroMemory( &message, sizeof( message ) );
		message.Category	= MP_SKILL;
		message.Protocol	= MP_SKILL_START_ACK;
		message.dwData1		= skillIndex;
		message.dwData2		= itemDbIndex;

		player->SendMsg( &message, sizeof( message ) );
	}

	// 080602 LUJ, 전역 서버 스킬인 경우 다른 맵으로 전파
	if( TARGET_KIND_WORLD == skillInfo->Area )
	{
		MSG_ITEM_SKILL_START_TO_MAP message;
		ZeroMemory( &message, sizeof( message ) );
		message.Category		= MP_SKILL;
		message.Protocol		= MP_SKILL_START_TO_MAP;
		message.mGuildIndex		= player->GetGuildIdx();
		message.mFamilyIndex	= player->GetFamilyIdx();

		switch( skillInfo->AreaTarget )
		{
		case eSkillAreaTargetGuild:
			{
				message.dwObjectID	= player->GetGuildIdx();
				break;
			}
		case eSkillAreaTargetFamily:
			{
				message.dwObjectID	= player->GetFamilyIdx();
				break;
			}
		case eSkillAreaTargetParty:
			{
				message.dwObjectID	= player->GetPartyIdx();
				break;
			}
		}

		message.mSkill.wSkillIdx	=		skillIndex;
		message.mSkill.Level		= BYTE( skillInfo->Level );

		SKILLMGR->NetworkMsgParse(
			message.Protocol,
			&message );

		g_Network.Broadcast2AgentServer( ( char* )&message, sizeof( message ) );
	}
}

// 080602 LUJ, 패밀리 멤버 목록을 쿼리해서 스킬을 적용한다. 패밀리 정보가 맵 서버에 없고
//				에이전트에 있기 때문에 번거롭지만 DB에서 조회하는 편이 편하다.
//
//				주의: 미들 쿼리이므로 최대 20개밖에 쿼리하지 못한다. 그러나 패밀리 최대 인원이 다섯명이므로 사용한다
void RSkillAddToFamily( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD skillIndex = dbMessage->dwID;

	const ACTIVE_SKILL_INFO* skillInfo = 0;
	{
		cActiveSkillInfo* info = ( cActiveSkillInfo* )SKILLMGR->GetSkillInfo( skillIndex );

		if( ! info )
		{
			return;
		}

		skillInfo = info->GetSkillInfo();

		if( ! skillInfo )
		{
			return;
		}
	}

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		const MIDDLEQUERYST&	record		= query[ i ];
		const DWORD				playerIndex = atoi( ( char* )record.Data[ 0 ] );
		CPlayer*				player		= ( CPlayer* )g_pUserTable->FindUser( playerIndex );

		if( !	player ||
				player->GetObjectKind() != eObjectKind_Player )
		{
			continue;
		}

		SKILLMGR->AddBuffSkill( *player, *skillInfo );
	}
}

void CharacterBuffLoad( DWORD characteridx )
{
	sprintf(txt, "EXEC dbo.MP_CHARACTER_BUFF_LOAD %d", characteridx);
	g_DB.Query(eQueryType_FreeQuery, eCharacterBuffLoad, characteridx, txt, characteridx);
}

void RCharacterBuffLoad(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if( pMessage->dwResult == 0 ) 
	{
		return;
	}
	
	//CPlayer* player = ( CPlayer* )g_pUserTable->FindUser( pMessage->dwID );

	for( DWORD count = 0 ; count < pMessage->dwResult ; ++count )	
	{
		DWORD	skillidx	= ( DWORD )atoi( ( char* )pData[ count ].Data[ 0 ] );
		DWORD	remaintime	= ( DWORD )atoi( ( char* )pData[ count ].Data[ 1 ] );
		WORD	remaincount = ( WORD )atoi( ( char* )pData[ count ].Data[ 2 ] );

		// 080219 LUJ, 기간제 스킬의 기간이 긴 경우 오버플로될 수 있으므로 별도로 가져온다
		{
			const char* endDate = ( char* )pData[ count ].Data[ 3 ];

			if( strcmp( "0", endDate ) )
			{
				remaintime	= DWORD( atoi( endDate ) ) * 1000;
			}
		}

		SKILLMGR->BuffSkillStart( pMessage->dwID, skillidx, remaintime, remaincount );
	}
}

void CharacterBuffAdd( DWORD characteridx, DWORD skillidx, DWORD remaintime, WORD count, BOOL isEndDate )
{
	sprintf(txt, "EXEC dbo.MP_CHARACTER_BUFF_ADD %d, %d, %u, %d, %d", characteridx, skillidx, remaintime, count, isEndDate );
	g_DB.Query(eQueryType_FreeQuery, eCharacterBuffAdd, characteridx, txt, characteridx);
}

void CharacterBuffUpdate( DWORD characteridx, DWORD skillidx, DWORD remaintime, WORD count  )
{
	sprintf(txt, "EXEC dbo.MP_CHARACTER_BUFF_UPDATE %d, %d, %u, %d, %d", characteridx, skillidx, remaintime, count );
	g_DB.Query(eQueryType_FreeQuery, eCharacterBuffUpdate, characteridx, txt, characteridx);
}

void CharacterBuffRemove( DWORD characteridx, DWORD skillidx )
{
	sprintf(txt, "EXEC dbo.MP_CHARACTER_BUFF_REMOVE %d, %d", characteridx, skillidx);
	g_DB.Query(eQueryType_FreeQuery, eCharacterBuffRemove, characteridx, txt, characteridx);
}

void ConnectionCheck()
{
	sprintf(txt, "EXEC dbo.MP_ConnectionCheck");
	g_DB.Query(eQueryType_FreeQuery, eConnectionCheck, 0, txt);
}
// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.11.26	2007.11.27	2007.12.01	2008.01.23
void ChallengeZone_SavePos_Save(DWORD nPlayerID, DWORD nMapNum, DWORD nPosX, DWORD nPosY)
{
	sprintf(txt, "EXEC  %s %d, %d, %d, %d", STORED_CHALLENGEZONE_SAVEPOS_SAVE_INFO, nPlayerID, nMapNum, nPosX, nPosY);
	g_DB.Query(eQueryType_FreeQuery, eChallengeZone_SavePos_SetInfo, nPlayerID, txt, nPlayerID);
}

void ChallengeZone_EnterFreq_Save(DWORD nPlayerID, DWORD nEnterFreq, DWORD nEnterBonusFreq)
{
	sprintf(txt, "EXEC  %s %d, %d, %d", STORED_CHALLENGEZONE_ENTERFREQ_SAVE_INFO, nPlayerID, nEnterFreq, nEnterBonusFreq);
	g_DB.Query(eQueryType_FreeQuery, eChallengeZone_EnterFreq_SetInfo, nPlayerID, txt, nPlayerID);
}

void ChallengeZone_EnterFreq_Load(DWORD nPlayerID)
{
	sprintf(txt, "EXEC  %s %d", STORED_CHALLENGEZONE_ENTERFREQ_LOAD_INFO, nPlayerID);
	g_DB.Query(eQueryType_FreeQuery, eChallengeZone_EnterFreq_LoadInfo, nPlayerID, txt, nPlayerID);
}

void RChallengeZone_EnterFreq_Load(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	// 챌린지 존 맵에 입장할 때 입장 회수를 증가시킨다. 그런데 DB로 부터의 로드가 그 이후에 이루어지기 때문에
	// 입장 회수가 증가됐는지 체크해서 증가된 만큼 + 설정하고, 다시 DB에 저장한다.
	UINT nAleadyIncreaseFreq = pPlayer->GetChallengeZoneEnterFreq();

	if( pMessage->dwResult == 0 )
	{
	}
	else
	{
		// DB에 저장되어 있던 값을 설정
		pPlayer->SetChallengeZoneEnterFreq(atoi((char*)pData[0].Data[0]));
		pPlayer->SetChallengeZoneEnterBonusFreq(atoi((char*)pData[0].Data[1]));
		// 챌린지 존에 입장했나?
		if (nAleadyIncreaseFreq)
		{
			// 하루 이용 가능 회수를 초과했는지 체크.
			if (nAleadyIncreaseFreq + pPlayer->GetChallengeZoneEnterFreq() > CSHDateManager::ENTER_CHALLENGE_ZONE_FREQ_PER_DAY)
			{
				// 초과했다면 보너스 회수가 있다는 뜻이다. 보너스 회수를 -1 한다.
				// ..정말 Bonus 회수가 있는지 확인 체크
				if (pPlayer->GetChallengeZoneEnterBonusFreq())
				{
					pPlayer->SetChallengeZoneEnterBonusFreq(pPlayer->GetChallengeZoneEnterBonusFreq()-1);
				}
				else
				{
					// Bonus 회수가 없다? 뭔가 잘못됐지만 일단 그냥 입장시킴.
				}
			}
			else
			{
				// 하루 이용 가능 회수를 초과하지 않았으니까 회수 증가
				pPlayer->IncreaseChallengeZoneEnterFreq();
			}
		}
	}

	// 챌린지 존에 입장을 했다면 위의 루틴에 의해서 입장 가능 회수나, 보너스 회수가 변경되었기에 이를 DB에 저장.
	// ..다른 맵에서 챌린지 존에 입장을 할 때 처리하지 않고, 챌린지 존에 입장하는 순간에 처리하기 때문에
	// ..일반적인 DB Load 와는 조금 다름. 
	// ..다른 맵에서 챌린지 존에 입장할 때 회수 값을 저장시키면 이렇게 처리하지 않아도 되지만, 어떤 문제로 인하여
	// ..챌린지 존에 접속하지 못했을 경우에도 회수값이 설정되기 때문에 실제로 챌린지 존에 접속했음을 확인하기 위해 이렇게 처리했음.
	if (nAleadyIncreaseFreq)
	{
		ChallengeZone_EnterFreq_Save(pPlayer->GetID(), pPlayer->GetChallengeZoneEnterFreq(), pPlayer->GetChallengeZoneEnterBonusFreq());
	}

	pPlayer->SetInitState( PLAYERINITSTATE_CHALLENGEZONE_INFO, MP_USERCONN_GAMEIN_SYN );
}

void ChallengeZone_Success_Save(DWORD nPlayerID, DWORD nPartnerID, UINT nSection, UINT nClearTime)
{
	sprintf(txt, "EXEC  %s %d, %d, %d, %d", STORED_CHALLENGEZONE_SUCCESS_SAVE_INFO, nPlayerID, nPartnerID, nSection, nClearTime);
	g_DB.Query(eQueryType_FreeQuery, eChallengeZone_Success_SetInfo, nPlayerID, txt, nPlayerID);
}

void ChallengeZone_ClearNo1_Load(DWORD nPlayerID, UINT nSection)
{
	sprintf(txt, "EXEC  %s %d", STORED_CHALLENGEZONE_CLEAR_NO1_LOAD_INFO, nSection);
	g_DB.Query(eQueryType_FreeQuery, eChallengeZone_ClearNo1_LoadInfo, nPlayerID, txt, nPlayerID);
}

void RChallengeZone_ClearNo1_Load(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	if( pMessage->dwResult == 0 )
	{
		g_csDateManager.SRV_NotifyStartChallengeZone(pPlayer, NULL, NULL, 0, atoi((char*)pData[0].Data[3]));
	}
	else
	{
		g_csDateManager.SRV_NotifyStartChallengeZone(pPlayer, (char*)pData[0].Data[0], (char*)pData[0].Data[1], atoi((char*)pData[0].Data[2]), atoi((char*)pData[0].Data[3]));
	}
}
// E 데이트 존 추가 added by hseos 2007.11.26	2007.11.27	2007.12.01	2008.01.23

// 071210 KTH -- 케릭터의 인벤토리를 확장한다.
void IncreaseCharacterInventory( DWORD characteridx )
{
	sprintf(txt, "EXEC %s %d", STORED_INCREASE_CHARACTER_INVENTORY, characteridx);
	g_DB.Query(eQueryType_FreeQuery, eIncreaseCharacterInventory, characteridx, txt, characteridx);
}

void RIncreaseCharacterInventory(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);

	if( !pPlayer ) return;

	if( pMessage->dwResult != 0 )
	{
		WORD	wInventoryExpansion = atoi((char*)pData[0].Data[0]);

		if( pPlayer->GetInventoryExpansion() >= 2 )
			return;

		pPlayer->SetInventoryExpansion(pPlayer->GetInventoryExpansion() + 1);

		MSG_DWORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_INCREASE_INVENTORY_ACK;
		// 071220 LYW --- MapDBMsgParser : 오브젝트 아이디 세팅.
		msg.dwObjectID = pPlayer->GetID() ;
		msg.dwData = (DWORD)pPlayer->GetInventoryExpansion();

		pPlayer->SendMsg(&msg, sizeof(msg));
	}
}

void ResetCharacterInventory( DWORD characteridx )
{
	sprintf(txt, "EXEC %s %d", STORED_RESET_CHARACTER_INVENTORY, characteridx);
	g_DB.Query(eQueryType_FreeQuery, eResetCharacterInventory, characteridx, txt, characteridx);
}

// 071221 KTH -- 케릭터가 저장한 맵 정보를 요청.
/*void MemoryStoneInfoLoad( int characterIdx )
{
	sprintf(txt, "EXEC %s %d", STORED_MEMORY_STONE_INFO_LOAD, characterIdx);
	g_DB.Query(eQueryType_FreeQuery, eMemoryStoneInfoLoad, characterIdx, txt, characterIdx);
}
// 071221 KTH -- 케릭터에 저장되어 있는 맵 정보.
void RMemoryStoneInfoLoad(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	DWORD count = pMessage->dwResult;

	if( !pPlayer || count == 0 ) return;

	for( DWORD i = 0; i < count; ++i )
	{
		st_MemoryStoneInfo	MemoryStoneInfo;

		MemoryStoneInfo.byIdx = (BYTE)atoi( ( char* )pData[ i ].Data[ 0 ] );
		strcpy(MemoryStoneInfo.memoryName, ( char* )pData[ i ].Data[ 1 ] );
		MemoryStoneInfo.wMapIdx = (MAPTYPE)atoi( ( char* )pData[ i ].Data[ 2 ] );
		MemoryStoneInfo.curPos.x = (FLOAT)atoi( ( char* )pData[ i ].Data[ 3 ] );
		MemoryStoneInfo.curPos.z = (FLOAT)atoi( ( char* )pData[ i ].Data[ 4 ] );

		if( MemoryStoneInfo.byIdx > MAX_MEMORY_STONE_INFO )
		{
			continue;
		}

		pPlayer->m_MemoryStoneInfoMap.insert(std::pair<BYTE, st_MemoryStoneInfo>(MemoryStoneInfo.byIdx, MemoryStoneInfo));
	}

	SEND_MEMORYSTONES msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_MEMORY_MAP_ACK;
	msg.dwObjectID = pPlayer->GetID();

	for( int i = 0; i < MAX_MEMORY_STONE_INFO; i++ )
	{
		std::map<BYTE, st_MemoryStoneInfo>::iterator itr = pPlayer->m_MemoryStoneInfoMap.find(i);

		if( itr != pPlayer->m_MemoryStoneInfoMap.end() )
		{
			msg.memStone[i].byIdx = itr->second.byIdx;
			msg.memStone[i].curPos = itr->second.curPos;
			strcpy(msg.memStone[i].memoryName, itr->second.memoryName);
			msg.memStone[i].wMapIdx = itr->second.wMapIdx;
		}
	}
	pPlayer->SendMsg(&msg, sizeof(SEND_MEMORYSTONES));
}
// 071228 KTH -- 위치를 기억한다.
void MemoryStoneInfoAdd( int characterIdx, char* pMemoryName, MAPTYPE mapNum, float posX, float posZ, int positionIdx )
{
	sprintf(txt, "EXEC %s %d %s %d %d %d %d", STORED_MEMORY_STONE_INFO_ADD, characterIdx, pMemoryName, mapNum, posX, posZ, positionIdx);
	g_DB.Query(eQueryType_FreeQuery, eMemoryStoneInfoAdd, characterIdx, txt, characterIdx);
}
*/
// 071224 KTH -- 케릭터 이름 변경
void ChangeCharacterName( char* szName, int characterIdx, DWORD itemIdx, POSTYPE pos)
{
	sprintf(txt, "EXEC %s %s, %d, %d, %d", STORED_CHARACTER_CHANGE_NAME, szName, characterIdx, itemIdx, pos);
	g_DB.Query(eQueryType_FreeQuery, eChangeCharacterName, characterIdx, txt, characterIdx);
}
// 071226 KTH -- 케릭터의 이름 변경 결과를 가져온다. 아이템의 사용 여부도 여기서 판단한다.
void RChangeCharacterName(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);

	char text[ MAX_NAME_LENGTH + 1 ] = { 0 };

	if( !pPlayer )
		return;

	strcpy( text, pPlayer->GetObjectName() );

	if( atoi((char*)pData->Data[0]) == 0 )
	{
		char changeName[17];
		strcpy(changeName, (char*)pData->Data[1]);
		// 케릭터의 이름을 바꿔준다.
		pPlayer->SetCharacterName(changeName);

		DWORD itemIdx	=	atoi((char*)pData->Data[2]);
		POSTYPE	pos		=	(POSTYPE)atoi((char*)pData->Data[3]);

		ITEMBASE previousItem;
		{
			const ITEMBASE* item = ITEMMGR->GetItemInfoAbsIn( pPlayer, pos );

			if( item )
			{
				previousItem = *item;
			}
		}

		if( ITEMMGR->UseItem(pPlayer, pos, itemIdx) != eItemUseSuccess)
		{
			MSG_DWORD msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_CHANGE_CHARACTER_NAME_NACK;
			msg.dwData = 0; // 0은 아이템 사용이 실패 하였을때 보낸다.
			pPlayer->SendMsg(&msg, sizeof(msg));
			return;
		}

		MSG_CHANGE_CHARACTER_NAME_ACK	msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_CHANGE_CHARACTER_NAME_ACK;
		//msg.dwObjectID = pPlayer->GetID();
		msg.TargetPos = pos;
		msg.wItemIdx = itemIdx;
		strcpy(msg.changeName, pPlayer->GetObjectName());
		pPlayer->SendMsg(&msg, sizeof(msg));

		{
			LogItemMoney(
				pPlayer->GetID(),
				pPlayer->GetObjectName(),
				0,
				text,
				eLog_ItemUse,
				pPlayer->GetMoney(),
				0,
				0,
				previousItem.wIconIdx,
				previousItem.dwDBIdx,
				previousItem.Position,
				0,
				previousItem.Durability - 1,
				0 );
		}
		{
			// 0801
			LogChangeName(pPlayer->GetID(), text, pPlayer->GetObjectName());
		}

		// 080121 KTH -- 케릭터 이름 변경을 에이전트에 알려준다.
		// charIDX, userIDX, characterName, ServerNum <- 요건 에이전트에서 처리 ㅎ;;
		MSG_CHANGE_CHARACTER_NAME_AGENT msg2;
		msg2.Category = MP_ITEM;
		msg2.Protocol = MP_ITEM_CHANGE_CHARACTER_NAME_SYN;
		msg2.dwObjectID = pPlayer->GetID();
		strcpy(msg2.changeName, changeName);
		msg2.CharacterIdx = pPlayer->GetID();
		msg2.UserIdx = pPlayer->GetUserID();

		pPlayer->SendMsg(&msg2, sizeof(MSG_CHANGE_CHARACTER_NAME_AGENT));

		// 071231 LYW --- MapDBMsgParser : 캐릭터 로그아웃 처리.
		pPlayer->UpdateLogoutToDB();
		FRIENDMGR->UserLogOut(pPlayer->GetID());

		SaveCharInfoBeforeLogout(pPlayer->GetID(), pPlayer->GetAgentNum(), pPlayer->GetUserID(), pPlayer->GetMoney(eItemTable_Storage));
	}
	else
	{
		MSG_DWORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_CHANGE_CHARACTER_NAME_NACK;
		msg.dwData = atoi((char*)pData->Data[0]);
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
}

//---KES AUTONOTE
void AutoNoteListAdd( DWORD dwCharacterIdx, char* pStrAutoName, DWORD dwAutoCharacterIdx, DWORD dwAutoUserIdx )
{
	sprintf( txt, "EXEC dbo.MP_AUTONOTELIST_ADD %u, '%s', %u, %u", dwCharacterIdx, pStrAutoName, dwAutoCharacterIdx, dwAutoUserIdx );
	g_DB.Query( eQueryType_FreeQuery, eAutoNoteList_Add, dwCharacterIdx, txt );	
}

void RAutoNoteListAdd( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( pMessage->dwResult == 0 )
		return;

	MSG_AUTOLIST_ADD msg;
	msg.Category	= MP_AUTONOTE;
	msg.Protocol	= MP_AUTONOTE_LIST_ADD;
	msg.dwObjectID	= pMessage->dwID;

	SafeStrCpy( msg.Name, (char*)pData[0].Data[0], MAX_NAME_LENGTH+1 );
	SafeStrCpy( msg.Date, (char*)pData[0].Data[1]+2, 20 ); //+2는 2008을 08로 표시하기 위해

	g_Network.Broadcast2AgentServer( (char*)&msg, sizeof(msg) );	//에이젼트로 브로드캐스팅
}

void AutoNoteListLoad( DWORD dwCharacterIdx  )
{
	sprintf( txt, "EXEC dbo.MP_AUTONOTELIST_LOAD %u", dwCharacterIdx );
	g_DB.Query( eQueryType_FreeQuery, eAutoNoteList_Load, dwCharacterIdx, txt );	
}

void RAutoNoteListLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( pPlayer == NULL ) return;

	int nCount = pMessage->dwResult;
	if( nCount == 0 ) return;

	MSG_AUTOLIST_ALL msg;
	msg.Category	= MP_AUTONOTE;
	msg.Protocol	= MP_AUTONOTE_LIST_ALL;
	msg.dwObjectID	= pMessage->dwID;
	msg.nCount		= nCount;

	for( int i = 0 ; i < nCount ; ++i )
	{
		SafeStrCpy( msg.row[i].Name, (char*)pData[i].Data[0], MAX_NAME_LENGTH+1 );
		SafeStrCpy( msg.row[i].Date, (char*)pData[i].Data[1]+2, 20 ); //+2는 2008을 08로 표시하기 위해
	}

	pPlayer->SendMsg( &msg, msg.GetMsgLength() );
}
//--------------
// 080204 KTH -- 이벤트 관련 쿼리
void UpdateEventFromGame( DWORD dwCharacterIdx, char* pCharacterName )
{
	sprintf( txt, "EXEC dbo.up_event_From_Game_insert %d, '%s', %d", dwCharacterIdx, pCharacterName, g_nServerSetNum );
	g_DB.LoginQuery(eQueryType_FreeQuery, eJoinEvent, dwCharacterIdx, txt);
}

void GiftItemInsert( DWORD characterIdx, DWORD useridx, DWORD eventIdx, DWORD itemIdx, DWORD count, BOOL isStack )
{
	sprintf(txt, "EXEC  %s %d, %d, %d, %d, %d", "MP_GIFT_ITEM_INSERT", useridx, eventIdx, itemIdx, count, isStack );
	g_DB.Query(eQueryType_FreeQuery, eGiftItemInsert, characterIdx, txt);

	// 080716 LUJ, 로그. 숫자 4는 복구툴에 정의된 eItemShopLog의 enum 상수값
	g_DB.LogMiddleQuery(
		0,
		0,
		"INSERT TB_ITEM_SHOP_LOG ( TYPE, USER_IDX, USER_ID, ITEM_IDX, ITEM_DBIDX, SIZE ) VALUES ( 4, %d, '', %d, 0, %d )",
		useridx,
		itemIdx,
		count );
}

void RGiftItemInsert( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( pPlayer == NULL ) return;

	DWORD eventidx = atoi((char*)pData[0].Data[0]);

	MSG_DWORD2 msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_GIFT_EVENT_NOTIFY;
	msg.dwData1 = eventidx;
	msg.dwData2 = 2;

	pPlayer->SendMsg( &msg, sizeof( msg ) );	
}

void FishingData_Load(DWORD dwPlayerID)
{
	g_DB.FreeMiddleQuery(
		RFishingData_Load,
		dwPlayerID,
		"EXEC DBO.MP_FISHINGDATA_LOAD %d",
		dwPlayerID );
}

void RFishingData_Load(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage)
{
	const DWORD playerIndex = dbMessage->dwID;
	CPlayer*	pPlayer		= (CPlayer*)g_pUserTable->FindUser( playerIndex );

	if( !	pPlayer ||
			dbMessage->dwResult != 1 )
	{
		ASSERT( 1 == dbMessage->dwResult );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];

	WORD wLevel = WORD(atoi((char*)record.Data[0]));

	// 080605 LYW --- MapDBMsgParser : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
	//DWORD dwExp = DWORD(atoi((char*)record.Data[1]));
	DWORDEX dwExp = _atoi64((char*)record.Data[1]);

	DWORD dwPoint = DWORD(atoi((char*)record.Data[2]));

	pPlayer->SetFishingLevel(wLevel);
	pPlayer->SetFishingExp(dwExp);
	pPlayer->SetFishPoint(dwPoint);
}

// 080602 LYW --- Player : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
//void FishingData_Update(DWORD dwPlayerID, WORD wFishingLevel, DWORD dwFishingExp, DWORD dwFishPoint)
void FishingData_Update(DWORD dwPlayerID, WORD wFishingLevel, EXPTYPE dwFishingExp, DWORD dwFishPoint)
{
	// 080605 LYW --- MapDBMsgParser : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
	//g_DB.FreeMiddleQuery(
	//	NULL,
	//	dwPlayerID,
	//	"EXEC  DBO.MP_FISHINGDATA_UPDATE %d, %d, %u, %u",
	//	dwPlayerID, wFishingLevel, dwFishingExp, dwFishPoint);

	g_DB.FreeMiddleQuery(
		NULL,
		dwPlayerID,
		"EXEC  DBO.MP_FISHINGDATA_UPDATE %d, %d, %0.f, %u",
		dwPlayerID, wFishingLevel, (float)dwFishingExp, dwFishPoint);
}

// 080808 LUJ, 낚시 경험치/레벨 추가, 더 이상 낚시 포인트만 저장하는 로그가 아니므로 함수명을 변경
void Log_Fishing(DWORD dwPlayerID, CHAR cLogType, DWORD dwDiffPoint, DWORD dwTotalPoint, DWORD dwBuyItemIdx, DWORD dwUseFish, EXPTYPE dwExp, DWORD dwLevel )
{
	// 080808 LUJ, 낚시 경험치/레벨 추가
	g_DB.LogMiddleQuery(
		0,
		0,
		"EXEC dbo.UP_Fishing_log_insert %d, %d, %u, %u, %d, %d, %0.0f, %d",
		dwPlayerID,
		cLogType,
		dwDiffPoint,
		dwTotalPoint,
		dwBuyItemIdx,
		dwUseFish,
		float( dwExp ),
		dwLevel );
}

void PetInfoLoad( DWORD characterIdx, DWORD userIdx )
{
	sprintf( txt, "EXEC  %s %d", "dbo.MP_PET_INFO_LOAD", userIdx );
	g_DB.Query( eQueryType_FreeQuery, ePetInfoLoad, characterIdx, txt, characterIdx );
}

void RPetInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( pPlayer == NULL ) return;
	
	DWORD count = pMessage->dwResult;

	if( count == 0 ) return;

	if( atoi( ( char* )pData->Data[ 0 ] ) == 0 ) return;

	for( DWORD i = 0; i < count; ++i )
	{
		PET_OBJECT_INFO* pInfo = PETMGR->NewPetObjectInfo();

		pInfo->ID					=	(DWORD)atoi((char*)pData[i].Data[ePetObjectInfo_ID]);			
		pInfo->ItemDBIdx			=	(DWORD)atoi((char*)pData[i].Data[ePetObjectInfo_ItemDBIdx]);	
		pInfo->MasterIdx			=	(DWORD)atoi((char*)pData[i].Data[ePetObjectInfo_MasterIdx]);	
		pInfo->Kind					=	(DWORD)atoi((char*)pData[i].Data[ePetObjectInfo_Kind]);	
		pInfo->Level				=	(BYTE)atoi((char*)pData[i].Data[ePetObjectInfo_Level]);	
		pInfo->Grade				=	(BYTE)atoi((char*)pData[i].Data[ePetObjectInfo_Grade]);	
		pInfo->SkillSlot			=	(BYTE)atoi((char*)pData[i].Data[ePetObjectInfo_SkillSlot]);	
		pInfo->Exp					=	(WORD)atoi((char*)pData[i].Data[ePetObjectInfo_Exp]);	
		pInfo->Friendly				=	(BYTE)atoi((char*)pData[i].Data[ePetObjectInfo_Friendly]);	
		pInfo->HP					=	(DWORD)atoi((char*)pData[i].Data[ePetObjectInfo_HP]);	
		pInfo->MAXHP				=	(DWORD)atoi((char*)pData[i].Data[ePetObjectInfo_MAXHP]);	
		pInfo->MP					=	(DWORD)atoi((char*)pData[i].Data[ePetObjectInfo_MP]);	
		pInfo->MAXMP				=	(DWORD)atoi((char*)pData[i].Data[ePetObjectInfo_MAXMP]);	
		pInfo->AI					=	(BYTE)atoi((char*)pData[i].Data[ePetObjectInfo_AI]);	
		pInfo->State				=	(BYTE)atoi((char*)pData[i].Data[ePetObjectInfo_State]);	
		pInfo->Type					=	(BYTE)atoi((char*)pData[i].Data[ePetObjectInfo_Type]);	
		pInfo->FriendlyCheckTime	=	(DWORD)atoi((char*)pData[i].Data[ePetObjectInfo_CheckTime]);	
		SafeStrCpy( pInfo->MasterName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1 );

		pPlayer->AddPetInfo( pInfo );

		MSG_PET_INFO msg;
		msg.Category = MP_PET;
		msg.Protocol = MP_PET_OBJECT_INFO;
		msg.dwObjectID = pPlayer->GetID();

		memcpy( &( msg.PetObjectInfo ), pInfo, sizeof( PET_OBJECT_INFO ) );
		pPlayer->SendMsg( &msg, sizeof( msg ) );

		if( pInfo->State == ePetState_Summon )
		{
			pInfo->State = 0;
			PETMGR->SummonPet( pPlayer, pInfo->ItemDBIdx );
		}
	}
}

void RPetInfoAdd( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( pPlayer == NULL ) return;
	
	DWORD count = pMessage->dwResult;

	if( count == 0 ) return;
	
	if( atoi( ( char* )pData->Data[ 0 ] ) == 0 ) return;

	PET_OBJECT_INFO* pInfo = PETMGR->NewPetObjectInfo();

	pInfo->ID					=	(DWORD)atoi((char*)pData[0].Data[ePetObjectInfo_ID]);			
	pInfo->ItemDBIdx			=	(DWORD)atoi((char*)pData[0].Data[ePetObjectInfo_ItemDBIdx]);	
	pInfo->MasterIdx			=	(DWORD)atoi((char*)pData[0].Data[ePetObjectInfo_MasterIdx]);	
	pInfo->Kind					=	(DWORD)atoi((char*)pData[0].Data[ePetObjectInfo_Kind]);	
	pInfo->Level				=	(BYTE)atoi((char*)pData[0].Data[ePetObjectInfo_Level]);	
	pInfo->Grade				=	(BYTE)atoi((char*)pData[0].Data[ePetObjectInfo_Grade]);	
	pInfo->SkillSlot			=	(BYTE)atoi((char*)pData[0].Data[ePetObjectInfo_SkillSlot]);	
	pInfo->Exp					=	(WORD)atoi((char*)pData[0].Data[ePetObjectInfo_Exp]);	
	pInfo->Friendly				=	(BYTE)atoi((char*)pData[0].Data[ePetObjectInfo_Friendly]);	
	pInfo->HP					=	(DWORD)atoi((char*)pData[0].Data[ePetObjectInfo_HP]);	
	pInfo->MAXHP				=	(DWORD)atoi((char*)pData[0].Data[ePetObjectInfo_MAXHP]);	
	pInfo->MP					=	(DWORD)atoi((char*)pData[0].Data[ePetObjectInfo_MP]);	
	pInfo->MAXMP				=	(DWORD)atoi((char*)pData[0].Data[ePetObjectInfo_MAXMP]);	
	pInfo->AI					=	(BYTE)atoi((char*)pData[0].Data[ePetObjectInfo_AI]);	
	pInfo->State				=	(BYTE)atoi((char*)pData[0].Data[ePetObjectInfo_State]);	
	pInfo->Type					=	(BYTE)atoi((char*)pData[0].Data[ePetObjectInfo_Type]);	
	pInfo->FriendlyCheckTime	=	(DWORD)atoi((char*)pData[0].Data[ePetObjectInfo_CheckTime]);	
	SafeStrCpy( pInfo->MasterName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1 );

	pPlayer->AddPetInfo( pInfo );

	MSG_PET_INFO msg;
	msg.Category = MP_PET;
	msg.Protocol = MP_PET_HATCH;
	msg.dwObjectID = pPlayer->GetID();

	memcpy( &( msg.PetObjectInfo ), pInfo, sizeof( PET_OBJECT_INFO ) );
	pPlayer->SendMsg( &msg, sizeof( msg ) );
}

void PetInfoAdd( DWORD characterIdx, DWORD userIdx, DWORD itemDBIdx, WORD kind, BYTE SkillSlot )
{
	sprintf( txt, "EXEC  %s %d, %d, %d, %d", "dbo.MP_PET_INFO_ADD", userIdx, itemDBIdx, kind, SkillSlot );
	g_DB.Query( eQueryType_FreeQuery, ePetInfoAdd, characterIdx, txt, characterIdx );
}

void PetInfoRemove( DWORD characterIdx, DWORD itemDBIdx )
{
	sprintf( txt, "EXEC  %s %d", "dbo.MP_PET_INFO_REMOVE", itemDBIdx );
	g_DB.Query( eQueryType_FreeQuery, ePetInfoRemove, characterIdx, txt, characterIdx );
}

void PetInfoUpdate( DWORD characterIdx, DWORD itemDBIdx, BYTE level, BYTE grade, BYTE type, BYTE slot, WORD exp, BYTE friendy, DWORD maxhp, DWORD hp, DWORD maxmp, DWORD mp, BYTE ai, BYTE state, DWORD time )
{
	sprintf( txt, "EXEC  %s %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", "dbo.MP_PET_INFO_UPDATE", itemDBIdx, level, grade, type, slot, exp, friendy, maxhp, hp, maxmp, mp, ai, state, time );
	g_DB.Query( eQueryType_FreeQuery, ePetInfoUpdate, characterIdx, txt, characterIdx );
}

void PetMasterUpdate( DWORD characterIdx, DWORD userIdx, DWORD itemDBIdx )
{
	sprintf( txt, "EXEC  %s %d, %d", "dbo.MP_PET_MASTER_UPDATE", userIdx, itemDBIdx );
	g_DB.Query( eQueryType_FreeQuery, ePetMasterUpdate, characterIdx, txt, characterIdx );
}

// 080425 KTH -- 가축 로그 --------------------------------------------------------------------------------------------------
void InsertLogFarmAnimal( CSHFarm* pFarm, CAnimal* pAnimal,  eFamilyLog type )
{
	if( !pFarm ||
		!pAnimal )
	{
		ASSERT(0);
		return;
	}

	g_DB.LogMiddleQuery(
		0,
		0,
		"EXEC DBO.TP_FARM_ANIMAL_LOG_INSERT %d, %d, %d, %d, %d, %d, %d, %d, %d, \'""\'",
		pFarm->GetID(),
		pAnimal->GetOwner(),
		pAnimal->GetID(),
		pAnimal->GetKind(),
		pAnimal->GetStep(),
		pAnimal->GetLife(),
		pAnimal->GetContentment(),
		pAnimal->GetInterest(),
		type );
}
//---------------------------------------------------------------------------------------------------------------------------

// 080616 LUJ, 함정이 공격할 적 길드원을 추가한다
void RAddEnemyToTrap( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	CTrap* trap = ( CTrap* )g_pUserTable->FindUser( dbMessage->dwID );

	if( !	trap ||
			trap->GetObjectKind() != eObjectKind_Trap )
	{
		return;
	}

	// 080616 LUJ, 함정에 적 오브젝트 인덱스를 추가하는 클로저 함수
	struct
	{
		void operator() ( CTrap& trap, DWORD enemeyGuildIndex ) const
		{
			CGuild* guild = GUILDMGR->GetGuild( enemeyGuildIndex );

			if( ! guild )
			{
				return;
			}

			GUILDMEMBERINFO	memberInfo[ MAX_GUILD_MEMBER ];
			ZeroMemory( memberInfo, sizeof( memberInfo ) );

			guild->GetTotalMember( memberInfo );

			for(
				DWORD size = min( MAX_GUILD_MEMBER, guild->GetMemberNum() );
				size--; )
			{
				const GUILDMEMBERINFO& member = memberInfo[ size ];

				trap.AddEnemy( member.MemberIdx );
			}
		}
	}
	AddEnemy;

	for( DWORD i = 0; i < dbMessage->dwResult; ++i )
	{
		const MIDDLEQUERYST&	record				= query[ i ];
		const DWORD				enemyGuildIndex1	= atoi( ( char* )record.Data[ 1 ] );
		const DWORD				enemyGuildIndex2	= atoi( ( char* )record.Data[ 2 ] );
		const DWORD				guildIndex			= atoi( ( char* )record.Data[ 3 ] );
		
		// 080616 LUJ, 길드 전쟁 중인 상대가 두 필드에 나눠있다. 자신의 길드 번호와 비교해서
		//				다른 길드 번호를 적으로 등록시킨다
		AddEnemy(
			*trap,
			enemyGuildIndex1 == guildIndex ? enemyGuildIndex2 : enemyGuildIndex1 );
	}

	if( dbMessage->dwResult == MAX_MIDDLEROW_NUM )
	{
		const MIDDLEQUERYST& record = query[ MAX_MIDDLEROW_NUM - 1 ];

		const DWORD index		= atoi( ( char* )record.Data[ 0 ] );
		const DWORD guildIndex	= atoi( ( char* )record.Data[ 3 ] );

		g_DB.FreeMiddleQuery(
			RAddEnemyToTrap,
			trap->GetID(),
			"EXEC DBO.MP_GUILD_ENEMY_MEMBER_SELECT %d, %d, %d",
			guildIndex,
			index,
			MAX_MIDDLEROW_NUM );
	}
}

void ItemMoveUpdatePetInven( DWORD characterIdx, DWORD userIdx, DWORD petIdx, DWORD fromItemIdx, DWORD fromItemPos, DWORD toItemIdx, DWORD toItemPos, BYTE param )
{
	sprintf( txt, "EXEC  %s %d, %d, %d, %d, %d, %d, %d, %d", "dbo.MP_ITEM_MoveUpdatePetInven", characterIdx, userIdx, petIdx, fromItemIdx, fromItemPos, toItemIdx, toItemPos, param );
	g_DB.Query( eQueryType_FreeQuery, eItemMoveUpdatePetInven, characterIdx, txt, characterIdx );
}

void PetItemInfoLoad( DWORD characterIdx, DWORD petIdx )
{
	sprintf( txt, "EXEC  %s %d", "dbo.MP_PET_ItemInfo", petIdx );
	g_DB.Query( eQueryType_FreeQuery, ePetItemInfoLoad, characterIdx, txt, characterIdx );
}

void RPetItemInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( pPlayer == NULL ) return;

	PET_OBJECT_INFO* pObjectInfo = pPlayer->GetPetInfo( pPlayer->GetSummonWaitPet() );
	PET_INFO* pPetInfo = PETMGR->GetPetInfo( pObjectInfo->Kind );

	if( !pObjectInfo || !pPetInfo )
		return;

	DWORD count = pMessage->dwResult;

	ITEMBASE buf[ ePetEquipSlot_Max ];

	memset( buf, 0, sizeof( ITEMBASE ) * ePetEquipSlot_Max );

	if( count != 0 && atoi( ( char* )pData->Data[ 0 ] ) != 0 )
	{
		for( DWORD i = 0; i < count; ++i )
		{
			POSTYPE pos = atoi((char*)pData[i].Data[3]);

			if( pos >= ePetEquipSlot_Max )
			{
				assert( 0 );
				continue;
			}

			if( pos >= ePetEquipSlot_Skill1 + pObjectInfo->SkillSlot )
			{
				ItemDeleteToDB( atoi((char*)pData[i].Data[1]) );

				LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
					eLog_PetItemDiscard, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
					atoi((char*)pData[i].Data[2]), atoi((char*)pData[i].Data[1]),  pos, 0,
					0, pPlayer->GetPlayerExpPoint());
				
				continue;

			}

			buf[pos].dwDBIdx = atoi((char*)pData[i].Data[1]);
			buf[pos].wIconIdx = atoi((char*)pData[i].Data[2]);
			buf[pos].Position = pos;
		}
	}

	VECTOR3* pPlayerPos = CCharMove::GetPosition( pPlayer );
	VECTOR3 pos;

	int ran=0;
	float dx=0,dz=0;
	ran = rand();
	dx = float(ran%PETMGR->GetAreaDistance()) * (ran%2?1:-1);
	ran = rand();
	dz = float(ran%PETMGR->GetAreaDistance()) * (ran%2?1:-1);
	pos.x = pPlayerPos->x + dx;
	pos.z = pPlayerPos->z + dz;

	if( pos.x < 0 )				pos.x = 0;
	else if( pos.x > 51199 )	pos.x = 51199;

	if( pos.z < 0 )				pos.z = 0;
	else if( pos.z > 51199 )	pos.z = 51199;

	pos.x = ((DWORD)(pos.x/TILECOLLISON_DETAIL)*TILECOLLISON_DETAIL);
	pos.z = ((DWORD)(pos.z/TILECOLLISON_DETAIL)*TILECOLLISON_DETAIL);

	BASEOBJECT_INFO info;
	info.dwObjectID = pObjectInfo->ID;
	info.dwUserID = pObjectInfo->MasterIdx;
	SafeStrCpy(info.ObjectName, pPetInfo->Name, MAX_NAME_LENGTH+1 );
	info.BattleID = pPlayer->GetBattleID();
	info.BattleTeam = pPlayer->GetBattleTeam();
	info.ObjectState = 0;
	info.ObjectBattleState = 0;

	CPet* pPet = g_pServerSystem->AddPet( &info, pObjectInfo, &pos, pPlayer );
	pPet->SetAllWearedItem( buf );
	CCharMove::InitMove( pPet, &pos );

	char log[256];
	sprintf( log, "./log/pet%02d.txt", g_pServerSystem->GetMapNum() );
	FILE* fp = fopen( log, "a+" );

	if( fp )
	{
		fprintf( fp, "SUMMON:\t%d\n", pPet );
		fclose( fp );
	}


}

// 080716 LUJ, 펫 로그
void LogPet( const PET_OBJECT_INFO& info, ePetLog log )
{
	g_DB.LogMiddleQuery(
		0,
		0,
		"INSERT INTO TB_PET_LOG ( TYPE, USER_IDX, PET_IDX, EXPERIENCE, FRIENDSHIP, GRADE, LEVEL, KIND, STATE, SKILL_SLOT ) VALUES ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d )",
		log,
		info.MasterIdx,
		info.ID,
		info.Exp,
		info.Friendly,
		info.Grade,
		info.Level,
		info.Kind,
		info.State,
		info.SkillSlot );
}

void Option_Load(DWORD dwPlayerIdx)
{
	g_DB.FreeMiddleQuery(
		ROption_Load,
		dwPlayerIdx,
		"EXEC DBO.MP_OPTIONLOAD %d",
		dwPlayerIdx );
}

void ROption_Load(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage)
{
	const DWORD playerIndex = dbMessage->dwID;
	CPlayer*	pPlayer		= (CPlayer*)g_pUserTable->FindUser( playerIndex );

	if( !	pPlayer ||
			dbMessage->dwResult != 1 )
	{
		ASSERT( 1 == dbMessage->dwResult );
		return;
	}

	const MIDDLEQUERYST& record = query[ 0 ];
	DWORD dwHideFlag = DWORD(atoi((char*)record.Data[0]));

	// 080917 NYJ - 파츠숨기기 플래그
	memset(pPlayer->tempHideParts, 0, sizeof(pPlayer->tempHideParts));
	
	if(dwHideFlag != eOPT_HIDEPARTS_NONE)
	{
		if(dwHideFlag & eOPT_HIDEPARTS_EQUIP_BAND)		pPlayer->tempHideParts[eWearedItem_Band]			= 1;
		if(dwHideFlag & eOPT_HIDEPARTS_EQUIP_GLASSES)	pPlayer->tempHideParts[eWearedItem_Glasses]			= 1;
		if(dwHideFlag & eOPT_HIDEPARTS_EQUIP_WING)		pPlayer->tempHideParts[eWearedItem_Wing]			= 1;
		if(dwHideFlag & eOPT_HIDEPARTS_COSTUME_HEAD)	pPlayer->tempHideParts[eWearedItem_Costume_Head]	= 1;
		if(dwHideFlag & eOPT_HIDEPARTS_COSTUME_DRESS)	pPlayer->tempHideParts[eWearedItem_Costume_Dress]	= 1;
		if(dwHideFlag & eOPT_HIDEPARTS_COSTUME_GLOVE)	pPlayer->tempHideParts[eWearedItem_Costume_Glove]	= 1;
		if(dwHideFlag & eOPT_HIDEPARTS_COSTUME_SHOES)	pPlayer->tempHideParts[eWearedItem_Costume_Shoes]	= 1;
	}

	pPlayer->SetInitState( PLAYERINITSTATE_OPTION_INFO, MP_USERCONN_GAMEIN_SYN );
}

void Option_Update(CPlayer* pPlayer)
{
	if(!pPlayer)	return;

	const bool* pHideParts = NULL;
	pHideParts = pPlayer->GetHideParts();
	if(pHideParts)
	{
		DWORD dwHideFlag = eOPT_HIDEPARTS_NONE;
		if(pHideParts[eWearedItem_Band])			dwHideFlag |= eOPT_HIDEPARTS_EQUIP_BAND;
		if(pHideParts[eWearedItem_Glasses])			dwHideFlag |= eOPT_HIDEPARTS_EQUIP_GLASSES;
		if(pHideParts[eWearedItem_Wing])			dwHideFlag |= eOPT_HIDEPARTS_EQUIP_WING;
		if(pHideParts[eWearedItem_Costume_Head])	dwHideFlag |= eOPT_HIDEPARTS_COSTUME_HEAD;
		if(pHideParts[eWearedItem_Costume_Dress])	dwHideFlag |= eOPT_HIDEPARTS_COSTUME_DRESS;
		if(pHideParts[eWearedItem_Costume_Glove])	dwHideFlag |= eOPT_HIDEPARTS_COSTUME_GLOVE;
		if(pHideParts[eWearedItem_Costume_Shoes])	dwHideFlag |= eOPT_HIDEPARTS_COSTUME_SHOES;

		g_DB.FreeMiddleQuery(
			NULL,
			pPlayer->GetID(),
			"EXEC  DBO.MP_OPTIONUPDATE %d, %d",
			pPlayer->GetID(), dwHideFlag);
	}
}

// 081218 LUJ, 맵 이동 전에 필수 정보를 업데이트시킨다
void UpdatePlayerOnMapOut( DWORD playerIndex, DWORD userIndex, MONEYTYPE inventoryMoney, MONEYTYPE storageMoney, DWORD mapIndex, WORD channelIndex )
{
	g_DB.FreeMiddleQuery(
		RUpdatePlayerOnMapOut,
		playerIndex,
		"EXEC MP_CHARACTER_UPDATE_ON_MAPOUT %u, %u, %u, %u, %u, %d",
		playerIndex,
		userIndex,
		inventoryMoney,
		storageMoney,
		mapIndex,
		channelIndex );
}

// 081218 LUJ, 필수 정보가 처리되었으면 맵 이동시킨다
void RUpdatePlayerOnMapOut( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage )
{
	const DWORD	playerIndex	= dbMessage->dwID;

	if( 1 != dbMessage->dwResult )
	{
		g_Console.LOG(
			4,
			"Error Message: MP_CHARACTER_UPDATE_ON_MAPOUT is failed. player index is %u",
			playerIndex );
		return;
	}

	const MIDDLEQUERYST&	record			= query[ 0 ];
	const BOOL				isSuccess		= atoi( ( char* )record.Data[ 0 ] );

	// 081218 LUJ, 업데이트에 실패한 경우, 맵이동을 하지 않는 편이 낫다
	if( ! isSuccess )
	{
		return;
	}

	const WORD	mapIndex		= atoi( ( char* )record.Data[ 1 ] );
	const WORD	channelIndex	= atoi( ( char* )record.Data[ 2 ] );

	MSG_WORD2 message;
	ZeroMemory( &message, sizeof( message ) );
	message.Category	= MP_USERCONN;
	message.Protocol	= MP_USERCONN_MAP_OUT_WITHMAPNUM;
	message.dwObjectID	= playerIndex;
	message.wData1		= mapIndex;
	message.wData2		= channelIndex;

	g_Network.Broadcast2AgentServer(
		( char* )&message,
		sizeof( message ) );
}
