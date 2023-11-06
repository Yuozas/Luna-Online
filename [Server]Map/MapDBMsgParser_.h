#pragma once


class CPlayer;
class CParty;
class CSHFarm;
class CSHCrop;
class CGuildScore;
// 080425 KTH
class CAnimal;


enum   DBMESSAGEIDFUNC 
{
	eCharacterInfoQuery,
	eCharacterSkillTreeQuery,
	eCharacterItemSlotQuery,	
	eCharacterItemQuery,
	eShopItemInfoQuery,			// ItemMall Item쿼리
	//ePetInfoQuery,				// CharacterPetInfo

	eSSItemInsert,
	
	/* New Item Work */
	eItemCombineUpdate,
	eItemMoveUpdate,
	eItemMoveStorageUpdate,
	eItemMoveGuildUpdate,
	eItemInsert,
	eItemDelete,
	eItemShopInvenInfo,
	eItemShopItemUseInfo,
	eItemShopItemUsing,
	eItemShopItemDelete,
	eItemShopItemUpdatetime,
	eItemShopItemUpdateParam,
	eItemShopItemUpdateUseParam,
	eItemShopItemUpdateUseInfo,
	eItemShopItemMPInfo,
	eItemShopMPUpdate,
	eItemShopMPDel,
	eItemShopGetItem,
	eItemMoveShopUpdate,
	eItemShopChangeName,
	eItemCharChange,

	eRegistLoginMapInfo,
	eUnRegistLoginMapInfo,

	/* New Skill Work */
	eSkillUpdate2,
	eSkillMoveUpdate2,
	eSkillInsert,
	eSkillDelete,				/// ¹≪°ø≫ef

	eBaseEconomy,				/// ±aAØ½A¼¼C￥Ao¸®
	eAuctionRegist,				/// °æ¸Aμi·ICwI½An
	eCharacterUpdate,			/// A³¸?dº¸ ¾÷μ￥AIÆ®
	eHeroInfoUpdate,			/// HeroInfo ¾÷μ￥AIÆ®
	eTotalInfoUpdate,			/// TotalInfo ¾÷μ￥AIÆ®
	eBadFameUpdate,
	
	eAuctionCheck,
	eAuctionPageList,
	eAuctionConfirm,
	ePartyRegistMapServer,		//¸E¼­¹o A×; ¶§ ÆAÆ¼¸| ¹Þ¾Æ¿A´U. 
	ePartyInfoByUserLogin,		///Au°¡ ·I±×AI CI¸e ÆAÆ¼ dº¸¸| ¹Þ¾Æ¿A´U.
	ePartyCreate,
	ePartyBreakup,
	ePartyCharacterTbUpdate,	//A³¸?AI A×AIºiAC ÆAÆ¼ AIμ|½º¸| 08·I ¸¸μc´U.
	ePartyAddMember,			//¸a¹oAß°¡
	ePartyDelMember,
	ePartyChangeMaster,

	eSavePoint,					// SavePoint 
	eSaveMapChangePoint,		// MapChange
	
	eStorageBuyStorage,
	eCharacterStorageInfo,
	eCharacterStorageItemInfo,
	eStorageMoneyUpdate,

	eFriendNotifyLogout,
	eFameCharacterUpdate,
	eBadFameCharacterUpdate,
	ePKCharacterUpdate,

	eWantedLoad,
	eWantedGetList,
	eWantedBuyRight,
	eWantedRegist,
	eWantedGiveupRight,
	eWantedComplete,
	eWantedDestroyed,
	eWantedOrderList,
	eWantedMyList,
	eWantedSearch,
	
	eQuestTotalInfo,	// AEº¸AU °¡AIμa
	eQuestUpdate,
	eQuestMainQuestLoad,
	eQuestSubQuestLoad,
	eQuestItemLoad,
	eQuestMainQuestInsert,
	eQuestSubQuestInsert,
	eQuestItemInsert,
	eQuestMainQuestUpdate,
	eQuestSubQuestUpdate,
	eQuestItemUpdate,
	eQuestMainQuestDelete,
	eQuestSubQuestDelete,
	eQuestSubQuestDeleteAll,
	eQuestItemDelete,
	eQuestItemDeleteAll,
	eQuestMainQuestUpdateCheckTime,

	eJournalGetList,
	eJournalInsertQuest,
	eJournalInsertWanted,
	eJournalInsertLevel,
	eJournalSaveUpdate,
	eJournalDelete,
	
	eCharacterInfoByTime,
	eSkillExpByTime,
	eSpeedHackCheck,

	eGuildSelectForReward,	// 길드전 보상을 하기 위해 길드 목록을 가져온다
	eGuildItemSelect,

	eGuildLoadGuild,		// 전체 길드 정보를 읽는다
	eGuildLoadMember,
	eGuildBreakUp,
	eGuildDeleteMember,
	eGuildSecedeMember,
	eGuildChangeMemberRank,
	eGuildGiveNickName,
	eGuildAddScore,
	eGuildUpdateScore,

	eGuildTournament_LoadAll,
	eGuildTournament_CancelLoad,
	eGuildTournament_CancelInsert,
	eGuildTournament_Insert,
	eGuildTournament_Delete,
	eGuildTournament_UpdateGuildInfo,
	eGuildTournament_UpdateTournamentInfo,	

	eConfirmUserOut,
	eSaveChrInfoBeforeOut,

	eCharacterInfoQueryToEventMap,

	eLoadGuildWarRecord,
	eDeleteGuildWarRecord,
	eLoadGuildWar,
	eInsertGuildWar,
	eDeleteGuildWar,	
	eUpdateGuildWarRecord,
	
	
//	eJackpot_TotalMoney_Load,
	eJackpot_TotalMoney_Update,
	eJackpot_PrizeInfo_Update,

	eGuildUnionLoad,
	eGuildUnionCreate,
	eGuildUnionDestroy,
	eGuildUnionAddGuild,
	eGuildUnionRemoveGuild,
	eGuildUnionSecedeGuild,

	eSiegeWarInfoLoad,
	eSiegeWarInfoInsert,
	eSiegeWarInfoUpdate,
	eSiegeWarGuildInfoLoad,
	eSiegeWarGuildInsert,
	eSiegeWarGuildUpdate,
	eSiegeWarGuildDelete,

	eChangeCharacterAttr,

	eSiegeWarProfitInfoLoad,
	eSiegeWarProfitUpdateGuild,
	eSiegeWarProfitUpdateTaxRate,
	eSiegeWarProfitUpdateProfitMoney,

	eChangeGuildLocation,

	/*어빌리티 삭제 - 이영준 eChangeCharacterStageAbility,*/

	//eGuildLoadNotice,
	eGuildUpdateNotice,

	eQuestEventCheck,
	eQuestEventEnd,

	eGMEvent01,

	eGuildTournamentAddLog,
	eSiegeWarAddLog,

	eQuickInfo,
	eQuickUpdate,

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.04.23	2007.09.12	2008.01.16
	eFarm_LoadFarmState,		// 농장 상태 로드
	eFarm_LoadCropInfo,			// 농작물 정보 로드
	eFarm_LoadTimeDelay,		// 타임 딜레이 정보 로드
	eFarm_SetFarmState,			// 농장 상태 저장
	eFarm_SetCropInfo,			// 농장물 정보 저장
	eFarm_SetTimeDelay,			// 타임 딜레이 정보 저장
	eFarm_SetTaxInfo,			// 관리비 정보 저장
	// E 농장시스템 추가 added by hseos 2007.04.23	2007.09.12	2008.01.16

	// desc_hseos_몬스터미터01
	// S 몬스터미터 추가 added by hseos 2007.05.28
	eMonsterMeter_LoadInfo,
	eMonsterMeter_SetInfo,
	// E 몬스터미터 추가 added by hseos 2007.05.28

	eSkillPointUpdate,
	eJobUpdate,

	eCharacterLogoutPointUpdate,

	// 070917 LYW --- MapDBMsgParser : Add query.
	eSaveNpcMapChangePoint,

	// 071023 LYW --- MapDBMsgParser : Add query to load tutorial info.
	eTutorialLoad,

	// 071023 LYW --- MapDBMsgParser : Add query to update tutorial info.
	eTutorialUpdate,

	// 071106 LYW --- MapDBMsgParser : 기본 스탯으로 업데이트 하는 함수 포인터.
	eUpdateToDefaultStats,

	// 071117 LYW --- MapDBMsgParser : 퀘스트 관련 추가 함수.
	eEndQuest_New,
	eEndSubQuest_New,
	eDeleteQuest_New,

	eCharacterBuffLoad,
	eCharacterBuffAdd,
	eCharacterBuffRemove,
	eCharacterBuffUpdate,

	// 071122 LYW --- MapDBMsgParser : 봉인 해제 처리.
	eUnSealItemByRealTime,
	eUnSealItemByGameTime,

	eUpdateRemainTime,

	// 071129 LYW --- MapDBMsgParser : 스킬 초기화.
	eSkill_Reset,

	// 071129 LYW --- MapDBMsgParser : 누적 스킬 포인트를 업데이트.
	eUpdateAccumulateSkillpoint,

	eConnectionCheck,

	eIncreaseCharacterInventory,
	eResetCharacterInventory,

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.26	2007.11.27	2007.12.01	2008.01.23
	eChallengeZone_SavePos_SetInfo,

	eChallengeZone_EnterFreq_LoadInfo,
	eChallengeZone_EnterFreq_SetInfo,

	eChallengeZone_Success_SetInfo,

	eChallengeZone_ClearNo1_LoadInfo,
	// E 데이트 존 추가 added by hseos 2007.11.26	2007.11.27	2007.12.01	2008.01.23
/*
	// desc_hseos_결혼_01
	// S 결혼 추가 added by hseos 2007.12.11
	eMarriage_LoadInfo,
	eMarriage_SetInfo,

	eMarriage_Divorce_SetInfo,

	eMarriage_TeleportFreq_SetInfo,

	eMarriage_Nickname_SetInfo,
	// E 결혼 추가 added by hseos 2007.12.11
*/
	// 071221 KTH -- 케릭터 이름 변경
	eChangeCharacterName,
	// 071227 KTH -- 기억석 정보 읽기
	/*eMemoryStoneInfoLoad,
	eMemoryStoneInfoAdd,*/
	// 080111 KTH -- 이름변경 로그 쿼리 추가
	//---KES AUTONOTE
	eAutoNoteList_Load,	//---자신이 잡은 오토
	eAutoNoteList_Add,	//---자신이 잡은 오토
	// 080204 KTH -- 이벤트 응모
	eJoinEvent,
	eResidentRegist_Reset,	// RResidentRegist_Reset
//---------------

	eGiftItemInsert,

	ePetInfoLoad,
	ePetInfoAdd,
	ePetInfoRemove,
	ePetInfoUpdate,
	ePetMasterUpdate,

	eItemMoveUpdatePetInven,
	ePetItemInfoLoad,

	eFarm_SetAnimalInfo,
	eFarm_LoadAnimalInfo,

	MaxQuery
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

// DB·I ³?¸± Stored Procedure ¶C´A Query ¹® dAC
#define STORED_TEST_QUERY	"UP_GAME_TEST_QUERY"
#define STORED_CHARACTER_SELECT				"dbo.MP_CHARACTER_SelectByCharacterIDX"		// ¼±AACN A³¸?dº¸ ¾o=
#define STORED_CHARACTER_KYUNGGONG			"dbo.MP_CHARACTER_KyungGong"				// °æ°ødº¸¸| ¹Þ¾Æ¿A´U
#define STORED_CHARACTER_NAEGONG			"dbo.MP_CHARACTER_NaeGong"					// ³≫°ødº¸¸| ¹Þ¾Æ¿A´U
#define STORED_CHARACTER_SKILLINFO			"dbo.MP_CHARACTER_SkillInfo"				// ¹≪°ødº¸¸| ¹Þ¾Æ¿A´U
#define STORED_CHARACTER_ITEMSLOTINFO		"dbo.MP_CHARACTER_ItemSlotInfo_JP"			// ItemSlotInfo
//#define STORED_CHARACTER_ITEMINFO			"dbo.MP_CHARACTER_ItemInfo"					// ¾ÆAIAU dº¸¸| ¹Þ¾Æ¿A´U
// 071122 LYW --- 시간 관련 추가.
#define STORED_CHARACTER_ITEMINFO			"dbo.MP_CHARACTER_ItemInfo_New"					
#define STORED_CHARACTER_ITEMOPTIONINFO		"dbo.MP_CHARACTER_ItemOptionInfo"			// ¾ÆAIAU ¿E¼Cdº¸¸| ¹Þ¾Æ¿A´U
#define STORED_CHARACTER_SKILLINFO			"dbo.MP_CHARACTER_SkillInfo"				// ½ºA³ dº¸¸| ¹Þ¾Æ¿A´U
#define STORED_CHARACTER_BASEECONOMY		"dbo.MP_MAP_BaseEconomy"					// ¸Eº°±aAØ½A¼¼Ao¸®
#define STORED_CHARACTER_AUCTIONREGIST		"dbo.MP_Auction_Regist"						// °æ¸Aμi·I
#define STORED_CHARACTER_AUCTIONONEPAGE		"dbo.MP_Auction_PageCalculate"				// °æ¸A¸®½ºÆ® COÆaAIAoAo¸®
#define STORED_CHARACTER_HEROINFOUPDATE		"dbo.MP_CHARACTER_HeroInfoUpdate"
#define STORED_CHARACTER_TOTALINFOUPDATE	"dbo.MP_CHARACTER_TotalInfoUpdate"
#define STORED_CHARACTER_BADFAMEUPDATE		"dbo.MP_CHARACTER_BadFameUpdate"
#define STORED_CHARACTER_REGISTLOGINMAPINFO "dbo.MP_LogInMapInfo_Regist"				//·I±×AI½A  ¸E AuAa ¹y¼®¾¾ Ao¿iAo ¸¶¼¼¿a!
#define STORED_CHARACTER_UNREGISTLOGINMAPINFO "dbo.MP_LogInMapInfo_UnRegist"			//·I±×¾Æ¿o  ¸E A￢¸®¾i ¹y¼®¾¾ Ao¿iAo ¸¶¼¼¿a!
#define STORED_CHARACTER_SAVEINFOBEFORELOGOUT "dbo.MP_CHARACTER_SaveInfoBeforeLogOut"

/* New Item CwI½An */
#define STORED_ITEM_COMBINEUPDATE			"dbo.MP_ITEM_CombineUpdate"
#define STORED_ITEM_MOVEUPDATE_INVEN		"dbo.MP_ITEM_MoveUpdate"
#define STORED_ITEM_MOVEUPDATE_STORAGE		"dbo.MP_ITEM_MoveUpdateStorage"
#define STORED_ITEM_MOVEUPDATE_GUILD		"dbo.MP_ITEM_MoveUpdateMunpa"
#define STORED_ITEM_INSERT_INVEN			"dbo.MP_ITEM_Insert"

/* New Skill */
#define STORED_SKILL_UPDATE				"dbo.MP_SKILLTREE_Update"
#define STORED_SKILL_MOVEUPDATE			"dbo.MP_SKILLTREE_MoveUpdate"
#define STORED_SKILL_INSERT				"dbo.MP_SKILLTREE_Insert"
#define STORED_SKILL_DELETE				"dbo.MP_SKILLTREE_Delete"

#define STORED_AUCTION_CHECK		"dbo.MP_AUCTION_CHECK"
#define STORED_AUCTION_SEARCH		"dbo.MP_AUCTION_SEARCH"
#define STORED_AUCTION_SORT			"dbo.MP_AUCTION_SORT"
#define STORED_AUCTION_REGISTER		"dbo.MP_AUCTION_REGISTER"
#define STORED_AUCTION_JOIN			"dbo.MP_AUCTION_JOIN"
#define STORED_AUCTION_CANCEL		"dbo.MP_AUCTION__REGISTER_CANCEL"

#define STORED_PARTY_REGIST_MAPSERVER	"dbo.MP_PARTY_Regist"
#define STORED_PARTY_INFO_BY_USERLOGIN	"dbo.MP_PARTY_PartyInfoByUserLogin"
#define STORED_PARTY_CREATE				"dbo.MP_PARTY_CreateParty"
#define STORED_PARTY_BREAK				"dbo.MP_PARTY_BreakupParty"
#define STORED_PARTY_CHARACTERUPDATE	"dbo.MP_PARTY_DelPartyidxinCharacterTB"
#define STORED_PARTY_UPDATE				"dbo.MP_PARTY_UpdateMember"
#define STORED_PARTY_ADDMEMBER			"dbo.MP_PARTY_AddMember"
#define STORED_PARTY_DELMEMBER			"dbo.MP_PARTY_DelMember"
#define STORED_PARTY_CHANGEMASTER		"dbo.MP_PARTY_ChangeMaster"

//guild
#define STORED_GUILD_CREATE				"dbo.MP_GUILD_Create"
#define STORED_GUILD_BREAKUP			"dbo.MP_GUILD_BreakUp"
#define STORED_GUILD_DELETEMEMBER		"dbo.MP_GUILD_DeleteMember"
#define STORED_GUILD_LOADGUILD			"dbo.MP_GUILD_LoadGuild"
#define STORED_GUILD_MARKUPDATE			"dbo.MP_GUILD_MarkUpdate"
#define STORED_GUILD_LOADMARK			"dbo.MP_GUILD_LoadMark"
//#define STORED_GUILD_LEVELUP			"dbo.MP_GUILD_LevelUp"
//#define STORED_GUILD_CHANGRANK			"dbo.MP_GUILD_ChangeRank"
#define STORED_GUILD_GIVENICKNAME		"dbo.MP_GUILD_GiveMemberNickName"
#define STORED_GUILD_MONEYUPDATE		"dbo.MP_GUILD_MoneyUpdate"
#define STORED_GUILD_MARKREGIST			"dbo.MP_GUILD_MarkRegist"

//Family[DB]
#define STORED_FAMILY_CREATE				"dbo.MP_FAMILY_Create"
#define STORED_FAMILY_BREAKUP			"dbo.MP_FAMILY_BreakUp"
#define STORED_FAMILY_DELETEMEMBER		"dbo.MP_FAMILY_DeleteMember"
#define STORED_FAMILY_ADDMEMBER			"dbo.MP_FAMILY_AddMember"
#define STORED_FAMILY_LOADFAMILY			"dbo.MP_FAMILY_LoadFamily"
#define STORED_FAMILY_LOADNOTICE			"dbo.MP_FAMILY_LoadNotice"
#define STORED_FAMILY_UPDATENOTICE		"dbo.MP_FAMILY_UpdateNotice"
#define STORED_FAMILY_LOADMEMBER			"dbo.MP_FAMILY_LoadMember"
#define STORED_FAMILY_MARKREGIST			"dbo.MP_FAMILY_MarkRegist"	
#define STORED_FAMILY_MARKUPDATE			"dbo.MP_FAMILY_MarkUpdate"
#define STORED_FAMILY_LOADMARK			"dbo.MP_FAMILY_LoadMark"
#define STORED_FAMILY_LEVELUP			"dbo.MP_FAMILY_LevelUp"
#define STORED_FAMILY_CHANGRANK			"dbo.MP_FAMILY_ChangeRank"
#define STORED_FAMILY_LOADITEM			"dbo.MP_FAMILY_LoadItem"
#define STORED_FAMILY_GIVENICKNAME		"dbo.MP_FAMILY_GiveMemberNickName"
#define STORED_FAMILY_MONEYUPDATE		"dbo.MP_FAMILY_MoneyUpdate"
#define STORED_FAMILY_ITEMOPTION			"dbo.MP_FAMILY_ItemOption_Info"
#define STORED_FAMILY_MARKREGIST			"dbo.MP_FAMILY_MarkRegist"

#define STORED_STORAGE_BUYSTORAGE			"dbo.MP_STORAGE_Buy"
#define STORED_STORAGE_MONEYUPDATE		"dbo.MP_STORAGE_MoneyUpdate"

#define STORED_STORAGE_INFO				"dbo.MP_STORAGE_Info"
#define STORED_STORAGE_ITEMINFO			"dbo.MP_STORAGE_ItemInfo"
#define STORED_STORAGE_MONEYUPDATE		"dbo.MP_STORAGE_MoneyUpdate"

#define STORED_FRIEND_NOTIFYLOGOUT		"dbo.MP_FRIEND_NotifyLogout"

// ItemMall Item
#define STORED_SHOPINVEN_INFO			"dbo.MP_SHOPITEM_InvenInfo"
#define STORED_SHOPITEM_INFO			"dbo.MP_SHOPITEM_ItemInfo"
#define STORED_SHOPITEM_USEINFO			"dbo.MP_SHOPITEM_UseInfo"
#define STORED_SHOPITEM_USING			"dbo.MP_SHOPITEM_Using"
#define STORED_SHOPITEM_USING_JP		"dbo.MP_SHOPITEM_Using_JP"
#define STORED_SHOPITEM_DELETE			"dbo.MP_SHOPITEM_Delete"
#define STORED_SHOPITEM_UPDATETIME		"dbo.MP_SHOPITEM_Updatetime"
#define STORED_SHOPITEM_UPDATEPARAM		"dbo.MP_SHOPITEM_UpdateParam"
#define STORED_SHOPITEM_UPDATEUSEPARAM	"dbo.MP_SHOPITEM_UpdateUseParam"
#define STORED_SHOPITEM_UPDATEUSEPARAM_JP	"dbo.MP_SHOPITEM_UpdateUseParam_JP"
#define STORED_SHOPITEM_GETITEM			"dbo.MP_SHOPITEM_GetItem"
#define STORED_ITEM_MOVEUPDATE_SHOP		"dbo.MP_ITEM_MoveUpdateShop"
#define STORED_SHOPITEM_CHANGENAME		"dbo.MP_character_rename"
#define STORED_SHOPITEM_CHARCHANGE		"dbo.MP_SHOPITEM_CharChange"

#define STORED_SHOPITEM_MPINFO			"dbo.MP_MOVEPOINT_GetInfo"
#define STORED_SHOPITEM_MPINSERT		"dbo.MP_MOVEPOINT_Insert"
#define STORED_SHOPITEM_MPUPDATE		"dbo.MP_MOVEPOINT_Update"
#define STORED_SHOPITEM_MPDEL			"dbo.MP_MOVEPOINT_Delete"

#define STORED_SHOPITEM_UPDATEUSEINFO	"dbo.MP_SHOPITEM_UpdateUseInfo"
//¸i¼º ¾÷μ￥AIÆ® 
#define STORED_FAME_CHARACTERUPDATE		"dbo.MP_FAME_CharacterUpdate"

//¾C¸i ¾÷μ￥AIÆ®
#define STORED_BADFAME_CHARACTERUPDATE	"dbo.MP_BADFAME_CharacterUpdate"
#define STORED_PK_CHARACTERUPDATE		"dbo.MP_PK_CharacterUpdate"

// Co≫o±Y
#define STORED_WANTED_LOAD				"dbo.MP_WANTED_LoadList"
#define STORED_WANTED_INFO				"dbo.MP_WANTED_InfoByUserLogIn"
#define STORED_WANTED_BUYRIGHT			"dbo.MP_WANTED_BuyRight"
#define STORED_WANTED_REGIST			"dbo.MP_WANTED_Regist"
#define STORED_WANTED_GIVEUP			"dbo.MP_WANTED_GiveUpRight"
#define STORED_WANTED_COMPLETE			"dbo.MP_WANTED_Complete"
#define STORED_WANTED_DESTROYED			"dbo.MP_WANTED_Destroyed"
#define STORED_WANTED_ORDERLIST			"dbo.MP_WANTED_OrderList"
#define STORED_WANTED_MYLIST			"dbo.MP_WANTED_MyList"
#define STORED_WANTED_SEARCH			"dbo.MP_WANTED_Search"

// quest
#define STORED_QUEST_MAINQUEST_LOAD		"dbo.MP_MAINQUEST_Load"
#define STORED_QUEST_MAINQUEST_INSERT	"dbo.MP_MAINQUEST_Insert"
#define STORED_QUEST_MAINQUEST_DELETE	"dbo.MP_MAINQUEST_Delete"
#define STORED_QUEST_MAINQUEST_UPDATE	"dbo.MP_MAINQUEST_Update"
#define STORED_QUEST_SUBQUEST_LOAD		"dbo.MP_SUBQUEST_Load"

// 071117 LYW --- QuestUpdate : DB 업데이트 수정.
#define STORED_QUEST_ENDQUEST_NEW		"dbo.MP_QUEST_ENDQUEST_NEW"
#define STORED_QUEST_ENDSUBQUEST_NEW	"dbo.MP_QUEST_ENDSUBQUEST_NEW"
#define STORED_QUEST_DELETEQUEST_NEW	"dbo.MP_QUEST_DELETEQUEST_NEW"


#define STORED_QUEST_SUBQUEST_INSERT	"dbo.MP_SUBQUEST_Insert"
#define STORED_QUEST_SUBQUEST_DELETE_ALL "dbo.MP_SUBQUEST_Delete_All"
#define STORED_QUEST_SUBQUEST_DELETE	"dbo.MP_SUBQUEST_Delete"
#define STORED_QUEST_SUBQUEST_UPDATE	"dbo.MP_SUBQUEST_Update"
#define STORED_QUEST_ITEM_LOAD			"dbo.MP_QUESTITEM_Load"
#define STORED_QUEST_ITEM_INSERT		"dbo.MP_QUESTITEM_Insert"
#define STORED_QUEST_ITEM_DELETE		"dbo.MP_QUESTITEM_Delete"
#define STORED_QUEST_ITEM_DELETE_ALL	"dbo.MP_QUESTITEM_Delete_All"
#define STORED_QUEST_ITEM_UPDATE		"dbo.MP_QUESTITEM_Update"

#define STORED_JOURNAL_GETLIST			"dbo.MP_JOURNAL_GetList"
#define STORED_JOURNAL_INSERTQUEST		"dbo.MP_JOURNAL_InsertQuest"
#define STORED_JOURNAL_INSERTWANTED		"dbo.MP_JOURNAL_InsertWanted"
#define STROED_JOURNAL_INSERTLEVEL		"dbo.MP_JOURNAL_InsertLevel"
#define STROED_JOURNAL_SAVEUPDATE		"dbo.MP_JOURNAL_SaveUpdate"
#define STROED_JOURNAL_DELETE			"dbo.MP_JOURNAL_Delete"

// ¸EAuE? A³¸?dº¸
#define STORED_MAPCHANGE_CHARINFO		"dbo.MP_MAPCHANGE_CharInfo"
#define STORED_MAPCHANGE_SKILLTREEINFO	"dbo.MP_CHARACTER_SkillTreeInfo"

#define STORED_SAVE_MAPCHAGEPOINT		"dbo.MP_CHARACTER_MapchangePointUpdate"
#define STORED_SAVE_MAPCHAGEPOINTRET	"dbo.MP_CHARACTER_MapchangePointReturn"
#define STORED_SAVE_LOGINPOINT			"dbo.MP_CHARACTER_LoginPointUpdate"
// AIº￥Aa¸® ¾ÆAIAU ≫ðAO
//#define STORED_CHARACTER_ITEMINSERT		"MP_CHARACTER_ItemInsert"

// Quest

// SavePoint
#define STORED_MAP_SAVEPOINT			"dbo.MP_MAP_SavePoint"

// Guild Tournament
#define STORED_GT_LOADALL				"dbo.MP_GT_LoadAll"
#define STORED_GT_CANCELLOAD			"dbo.MP_GT_CancelLoad"
#define STORED_GT_CANCELINSERT			"dbo.MP_GT_CancelInsert"
#define STORED_GT_TOURNAMENTINFO_UPDATE	"dbo.MP_GT_TrInfoUpdate"
#define STORED_GT_GUILDINFO_INSERT		"dbo.MP_GT_Insert"
#define STORED_GT_GUILDINFO_DELETE		"dbo.MP_GT_Delete"
#define STORED_GT_GUILDINFO_UPDATE		"dbo.MP_GT_Update"

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//┃SiegeWar													  ┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
#define STORED_SIEGE_INFOLOAD			"dbo.MP_SIEGEWAR_InfoLoad"
#define STORED_SIEGE_INFOINSERT			"dbo.MP_SIEGEWAR_InfoInsert"
#define STORED_SIEGE_INFOUPDATE			"dbo.MP_SIEGEWAR_InfoUpdate"
#define STORED_SIEGE_GUILDLOAD			"dbo.MP_SIEGEWAR_GuildInfoLoad"
#define STORED_SIEGE_GUILDINSERT		"dbo.MP_SIEGEWAR_GuildInfoInsert"
#define STORED_SIEGE_GUILDUPDATE		"dbo.MP_SIEGEWAR_GuildInfoUpdate"
#define STORED_SIEGE_GUILDDELETE		"dbo.MP_SIEGEWAR_GuildInfoDelete"

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.04.23	2007.09.12	2008.01.16
#define STORED_FARM_LOAD_FARMSTATE		"dbo.MP_FARM_LoadFarmState"
#define STORED_FARM_LOAD_CROPINFO		"dbo.MP_FARM_LoadCropInfo"
#define STORED_FARM_LOAD_DELAYTIME		"dbo.MP_FARM_LoadTimeDelay"

// 080416 KTH -- 축사 정보를 가져온다.
//-------------------------------------------------------------------------------
#define STORED_FARM_LOAD_ANIMALINFO		"dbo.MP_FARM_LoadAnimalInfo"
#define STORED_FARM_SET_ANIMALINFO		"dbo.MP_FARM_SetAnimalInfo"
//-------------------------------------------------------------------------------

#define STORED_FARM_SET_FARMSTATE		"dbo.MP_FARM_SetFarmState"
#define STORED_FARM_SET_CROPINFO		"dbo.MP_FARM_SetCropInfo"
#define STORED_FARM_SET_DELAYTIME		"dbo.MP_FARM_SetTimeDelay"
#define STORED_FARM_SET_TAXINFO			"dbo.MP_FARM_SetTaxInfo"
// E 농장시스템 추가 added by hseos 2007.04.23	2007.09.12	2008.01.16

// desc_hseos_몬스터미터01
// S 몬스터미터 추가 added by hseos 2007.05.28
#define STORED_MONSTERMETER_LOAD_INFO	"dbo.MP_MONSTERMETER_LoadInfo"
#define STORED_MONSTERMETER_SET_INFO	"dbo.MP_MONSTERMETER_SetInfo"
// E 몬스터미터 추가 added by hseos 2007.05.28

// desc_hseos_주민등록01
// S 주민등록 추가 added by hseos 2007.06.04
#define STORED_RESIDENTREGIST_LOAD_INFO				"dbo.MP_RESIDENTREGIST_LoadInfo"
#define STORED_RESIDENTREGIST_SAVE_INFO				"dbo.MP_RESIDENTREGIST_SaveInfo"
#define STORED_RESIDENTREGIST_UPDATE_INTRODUCTION	"dbo.MP_RESIDENTREGIST_UpdateIntroduction"
// 080215 KTH -- RESET RESIDENT REGIST
#define STORED_RESIDENTREGIST_RESET					"dbo.MP_RESIDENTREGIST_Reset"
// E 주민등록 추가 added by hseos 2007.06.04

// desc_hseos_패밀리01
// S 패밀리 추가 added by hseos 2007.07.04
#define STORED_FAMILY_LOAD_INFO			"dbo.MP_FAMILY_LoadInfo"
#define STORED_FAMILY_SAVE_INFO			"dbo.MP_FAMILY_SaveInfo"

#define STORED_FAMILY_MEMBER_LOAD_INFO		"dbo.MP_FAMILY_MEMBER_LoadInfo"
#define STORED_FAMILY_MEMBER_SAVE_INFO		"dbo.MP_FAMILY_MEMBER_SaveInfo"
// E 패밀리 추가 added by hseos 2007.07.04

// 071023 LYW --- MapDBMsgParser : Define procedure name to load tutorial info.
#define STORED_TUTORIAL_LOAD			"dbo.MP_TUTORIAL_Load"

// 071023 LYW --- MapDBMsgParser : Define procedure name to update tutorial info.
#define STORED_TUTORIAL_UPDATE			"dbo.MP_TUTORIAL_Update"

// 071106 LYW --- MapDBMsgParser : Define procedure name to update stats to default.
#define STORED_DEFAULT_STATS			"dbo.MP_DEFAULT_STATS_Update"

// 071129 LYW --- MapDBMsgParser : Define procedure name to resetting skills to default.
#define STORED_DEFAULT_SKILL			"dbo.MP_SKILLTREE_Reset"

// 071129 LYW --- MapDBMsgParser : Define procedure name to update accmulate skill point.
#define STORED_UPDATE_ACCSKILLPOINT		"dbo.MP_SKILLTREE_SkillPoint_Update_Accumulate"

// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.11.26	2007.11.27	2008.01.23
#define STORED_CHALLENGEZONE_SAVEPOS_SAVE_INFO		"dbo.MP_CHALLENGEZONE_SAVEPOS_SaveInfo"
#define STORED_CHALLENGEZONE_ENTERFREQ_SAVE_INFO	"dbo.MP_CHALLENGEZONE_ENTER_SaveInfo"
#define STORED_CHALLENGEZONE_ENTERFREQ_LOAD_INFO	"dbo.MP_CHALLENGEZONE_ENTER_LoadInfo"
#define STORED_CHALLENGEZONE_SUCCESS_SAVE_INFO		"dbo.MP_CHALLENGEZONE_SUCCESS_SaveInfo"
#define STORED_CHALLENGEZONE_CLEAR_NO1_LOAD_INFO	"dbo.MP_CHALLENGEZONE_CLEAR_NO1_LoadInfo"
// E 데이트 존 추가 added by hseos 2007.11.26	2007.11.27	2008.01.23

// desc_hseos_결혼_01
// S 결혼 추가 added by hseos 2007.12.11
#define STORED_MARRIAGE_LOAD_INFO					"dbo.MP_MARRIAGE_LoadInfo"
#define STORED_MARRIAGE_SAVE_INFO					"dbo.MP_MARRIAGE_SaveInfo"
#define STORED_MARRIAGE_DIVORCE_SAVE_INFO			"dbo.MP_MARRIAGE_DIVORCE_SaveInfo"
#define STORED_MARRIAGE_TELEPORTFREQ_SAVE_INFO		"dbo.MP_MARRIAGE_TELEPORTFREQ_SaveInfo"
#define STORED_MARRIAGE_NICKNAME_SAVE_INFO			"dbo.MP_MARRIAGE_NICKNAME_SaveInfo"
// E 결혼 추가 added by hseos 2007.12.11

//////////////////////////////////////////////////////////////////////////

// 071210 KTH -- 인벤토리 확장
#define STORED_INCREASE_CHARACTER_INVENTORY		"dbo.MP_INCREASE_CHARACTER_INVENTORY"
#define STORED_RESET_CHARACTER_INVENTORY		"dbo.MP_RESET_CHARACTER_INVENTORY"
// 071228 KTH -- 위치 기억석 정보 로드
#define STORED_MEMORY_STONE_INFO_LOAD			"dbo.MP_MEMORY_STONE_INFO_LOAD"
#define STORED_MEMORY_STONE_INFO_ADD			"dbo.MP_MEMORY_STONE_INFO_ADD"

// 071226 KTH -- 케릭터 이름 변경
#define STORED_CHARACTER_CHANGE_NAME			"dbo.MP_CHARACTER_CHANGE_NAME"


//////////////////////////////////////////////////////////////////////////
//pjs ¿ⓒ±a´A ³≫°¡ °iAA¾ß °U´U..
enum AuctionPage			// °æ¸A¸®½ºÆ® ¹Þ;¶§ ≫c¿e
{
	eAT_DBIdx, eAT_ItemDBIdx, eAT_ItemIdx, eAT_SellNum,  eAT_SellerID, eAT_EndDate, eAT_EndTime,
	eAT_StartPrice, eAT_ImmediatePrice, eAT_CurrentPrice, eAT_BidderNum, eAT_CurrentBidder, eAT_RegDate
};
//DB·I ºIAI ¹Þ´A AU·a¸| 'CN dAC
//eAuctionPageLIst
// AI¹IAo 1> °æ¸A ¹øE￡, ¹°C°¸i, ¼o·®, AO°i °¡°Y, Ai½A ±¸¸A°¡°Y, ¸¶°¨½A°￡, ³≪Au/·AAU, Au¿ⓒ¼o 
// AI¹IAo 3> ³²:½A°￡, °æ¸AAU
// ¾ÆAIAUAC AIμ|½º, 
enum AuctionList
{
	ePRO_NAME, ePRO_Amount, eBID_High_Price, eBID_Immediate_Price, eBID_DueDate, eHigh_Price_Name, eJOIN_Amount, 
	eBID_Left_Time, eAuctioneer,ePRO_Index ,eBID_Starttime,eREG_Index, 
};

//eAuctionSuccess
// AI¹IAo 4> °æ¸A¹°, °¹¼o, °≫¸AAU 
// ¾ÆAIAUAC AIμ|½º, °æ¸A μi·I ¹øE￡ ???
enum AuctionSuccess
{
	sePRO_NAME, sePRO_Amount, seAuctioneer,
	seREG_Index

};
//eAuctionConfirm
enum AuctionConfirm
{
	bAuctionConfirm
};
// DB¿¡¼­ ¾²AI´A ¼oA¡A¡E?.
enum CHSelectInfo	// A³¸?dº¸¸| ¹Þ;¶§ ¾¸
{
	eCS_ObjectID = 0, eCS_UserID, eCS_PartyID,eCS_Gender, eCS_Str, eCS_Life, eCS_Energe, eCS_Dex, eCS_Vit, eCS_Int, eCS_Wis, eCS_Fame, 
	eCS_Name, eCS_Repute, /*eCS_Stage,*/ eCS_Grade, eCS_Expoint, eCS_GradeUpPoint, eCS_Money, 
	eCS_Wing, eCS_Vitality, eCS_Map, eCS_FaceType, eCS_HeadType, eCS_Hat, eCS_Weapon, eCS_Dress, eCS_Shoes, 
	eCS_Costume_Hat, eCS_Costume_Dress, eCS_Glove, eCS_Costume_Shoes, eCS_Costume_Glove, eCS_Mask, eCS_MunpaIDX, eCS_BadFame, 
	eCS_MapChangePoint, eCS_LoginPoint, eCS_Inventory /*071210 KTH*/, eCS_Pos_X, eCS_Pos_Y, eCS_Playtime, eCS_LastPKEndTime, eCS_MaxLevel, eCS_MarkName, eCS_MunpaCanEntryDate,
	eCS_NickName, eCS_Race, eCS_JobGrade, eCS_Job1, eCS_Job2, eCS_Job3, eCS_Job4, eCS_Job5, eCS_Job6, eCS_Shield, eCS_Glasses, eCS_Band,
	eCS_SkillPoint, eCS_FamilyID, eCS_FamilyNickName, 
};

enum CHSkillTree
{
	eCM_ObjectID = 0, eCM_DBIDX, eCM_IDX, eCM_Level, eCM_ExpPoint, eCM_Position, eCM_QPosition, eCM_Wear, eCM_Option
};

// 071122 LYW --- MapDBMsgParser : CHItem 재정의.
enum CHItem
{
		eCI_ObjectID = 0
	,	eCI_DBIDX
	,	eCI_IDX
	,	eCI_Position
	,	eCI_QPosition
	,	eCI_Durability
	/*
	,	eCI_PyogukIdx
	,	eCI_MunpaIdx
	,	eCI_FamilyIdx
	,	eCI_ShopIdx
	*/
	,	eCI_Param
	,	eCI_RareIdx
	,	eCI_Seal
	,	eCI_EndTime
	,	eCI_RemainTime
} ;

enum CHSkill		// A³¸?AI ½ºA³ dº¸¸| ¹Þ;¶§ ≫c¿e
{
	eCK_ObjectID = 0, eCK_DBIDX, eCK_IDX, eCK_Position, eCK_QPosition, eCK_Durability
};

enum CHItemGroundDrop		// ¼­¹iº¾i±¼­ ¸E¿¡ ¾ÆAIAU Aß°¡EA dº¸¸| ¹Þ;¶§ ≫c¿e
{
	eIG_ObjectID, eIG_DBIDX, eIG_IDX, eIG_POSX, eIG_POSZ
};

enum MMapBaseEconomy			// ¸Eº° ±aAØ½A¼¼¸| ¹Þ;¶§ ≫c¿e
{
	eME_MEPNUM, eME_ORIGINNUM, eME_ORIGINPRICE, eME_ORIGINAMOUNT, eME_REQUIRENUM, eME_REQUIREPRICE, eME_REQUIREAMOUNT,
	eME_BUYRATES, eME_SELLRATES 
};

enum PartyInfoMap
{
	ePA_IDX, 
};
enum Partyinfobyuserlogin
{
	ePA_LIMemberId, ePA_LIMemberName, ePA_LIMemberCurrentMap, ePA_LIMemberLevel, ePA_LIOption,
};
enum Partychangemaster
{
	ePA_CMErr, ePA_CMTargetID,
};
enum PartyMember 
{
	 ePA_ERR,ePA_PARTYID, ePA_MASTER, ePA_OPTION
};
enum Partydelmember
{
	ePA_DMErr, ePA_DMTargetID,
};
enum Munpainfo //¸E¼­¹o°¡ ½AAUCO ¶§ ¹Þ´A ¹®ÆA dº¸ 
{
	eMu_MunpaID, eMunpaMasterID, eMu_MunpaName, eFamous, eMemberNum, eMunpaRank, eMunpaMoney
};

enum Munpaloadsyn
{
	eMu_MLStotalnum, eMu_MLSmunpaid, eMu_MLSmunpaname, eMu_MLSmastername, eMu_MLSmunpafamous, eMu_MLSmembernum, eMu_MLSKind
};
enum Munpacreate
{
	eMu_MCErr, eMu_MCMunpaid, eMu_MCMasterid, eMu_MCMunpaname, eMu_MCMunpaRank, eMu_MCMasterFame
};
enum Munpabreakup
{
	eMu_MBerr, eMu_MBmasterid, eMu_MBmasterfame,
};
enum Munpabreakupmember
{
	eMu_MBmemberid, eMu_MBmemberfame,
};
enum Munpahomeinfo
{
	eMu_MHIMunpaid, eMu_MHIIntroMsg, eMu_MHIMunpaKind, 
	eMu_MHIMunpaName, eMu_MHIMasterName, eMu_MHIFamous, 
	eMu_MHICreateDate, eMu_MHIMemberNum, eMu_MHIFieldWarWin, 
	eMu_MHIFieldWarLoss, eMu_MHIBuildingWarWin, eMu_MHIBuildingWarLoss, 
	eMu_MHICastleWarWin, eMu_MHICastleWarLoss, eMu_MHIBoardIDX, 
	eMu_MHIRSRank, eMu_MHIRCRank, eMu_MHIWCRank, 
	eMu_MHIDCRank, eMu_MHIBoardName, 
};
enum Munpamembersinfo
{
	eMu_MMTotalNum, eMu_MMunpaid, eMu_MMemberid, 
	eMu_MMemberName, eMu_MMemberRank, eMu_MMemberLevel, 
	eMu_MMemberFamous, eMu_MMember_LastVisitDay, eMu_MMember_EntryDay, 
	eMu_MMember_bLogin,
};
enum Munpachangememberank
{
	eMu_MCRchangeerr, eMu_MCRtargetid, eMu_MCRmunpaid, eMu_MCRposition, eMu_MCRtargetname, eMu_MCRfame, 
};
enum Munpabanmember
{
	eMu_MBMerr, eMu_MBMtargetid, eMu_MBMmunpaid, eMu_MBMblast, eMu_MBMtargetname, eMu_MBMfame, 
};
enum Munpadenymember
{
	eMu_MDMerr,  eMu_MDMtargetid, eMu_MDMmunpaid, eMu_MDMbLast, eMu_MDMtargetname,  
};
enum Munpaacceptmember
{
	eMu_MACerr, eMu_MACmunpaid, eMu_MACtargetid, eMu_MACblast, eMu_MACtargetname, eMu_MACMunpaname
};
enum MunpaBattleInfo
{
	eMu_MMunpaName, eMu_MMasterName, eMu_MMunpaFamous, eMu_MMemberNum, eMu_MCreateDate,
	eMu_MFBattleTotal, eMu_MBBattleTotal, eMu_MCBattleTotal, eMu_MTBattleTotal,
	eMu_MFBattleWin, eMu_MBBattleWin, eMu_MCBattleWin, eMu_MTBattleWin, 
	eMu_MFBattleLoss, eMu_MBBattleLoss, eMu_MCBattleLoss, eMu_MTBattleLoss
};
enum Munpajoinmemberinfo
{
	eMu_MJMTotalNum, eMu_MJMMunpaid, eMu_MJMCharacterID, 
	eMu_MJMCharacterName, eMu_MJMCharacterLevel, eMu_MJMCharacterFame, 
	eMu_MJMRequestJoinDay
};
enum Munpaboardinfo
{
	eMu_MCurBoardNum, eMu_MBoardsIdx, eMu_MBoardsName, eMu_MTotalNum,
};

enum Munpaitem
{
	eMu_IMunpaID, eMu_IDBIDX, eMu_IIDX, eMu_IPosition, eMu_IDurability, eMu_IRareIdx,
};

enum Munpacreateboardinfo
{
	eMu_MBoardidx, eMu_MRSubjectRank, eMu_MRContentRank, eMu_MWContentRank, eMu_MDContentRank, eMu_MBoardName
};
enum Munpaboardlistinfo
{
	eMu_MBLITotalPage, eMu_MBLIContentIDx, eMu_MBLIContentNum, eMu_MBLISubject, eMu_MBLIWriter, eMu_MBLICount, eMu_MBLIRegDate, eMu_MBLIRegTime, eMu_MBLIDepth
};
enum Munpaboardcontentsinfo
{
	eMu_MBCErr, eMu_MBCIContent, eMu_MBCIContentIDx, eMu_MBCIContentNum, eMu_MBCISubject, eMu_MBCIWriter, eMu_MBCICount, eMu_MBCIRegDate, eMu_MBCDCRank, eMu_MBCWCRank
};
enum Munpamoney
{
	eMu_MWMunpaID, eMu_MWMunpaMoney, eMu_MWPutInOut,
};

enum Storagebuy
{
	ePyo_errbuy, ePyo_Kind
};

enum StorageInfo
{
	ePyo_StorageNum, ePyo_Money,
};

enum StorageItemInfo
{
	ePI_DBIdx, ePI_IconIdx, ePI_Positon, ePI_QPosition, ePI_Durability, ePI_RareIdx, ePI_Seal, ePI_EndTime, ePI_RemainTime, 
};

enum Storageitem
{
	ePyo_Userid, ePyo_MaxPosition, ePyo_Durability, ePyo_Idx, ePyo_DBIdx, ePyo_Position, 
};

enum Friendlogout
{
	eFr_LToPlayerID, eFr_LLogoutName,
};
enum Friendnotelist
{
	eFr_NLFromName, eFr_NLNote, eFr_NLSendDate, eFr_NLSendTime, eFr_NLNoteID
};
enum Friendlist
{
	eFr_FLFriendid, eFr_FLFriendname, eFr_FLIsLogin,
};
enum Famemunpaupdate
{
	eFa_MunpaIDX, eFa_Fameval,
};
enum Wantedload
{
	eWa_LWantedIDX, eWa_LPlayerID, eWa_LPlayerName, eWa_LTotlaPrize, eWa_LRegistChridx, eWa_LTotalVolunteerNum,
};
enum Wantedregist
{
	eWa_RWanted_idx, eWa_RWantedChr_idx, eWa_RWanted_name, eWa_RTotalPrize, 
};
enum Wantedcomplete
{
	eWa_CCharacteridx, eWa_CCompleterName, eWa_CCompleteType,
};
enum Wantedorderlist
{
	eWa_OLTotal, eWa_OLWantedIDX, eWa_OLWantedChrName, eWa_OLPrize, eWa_OLVolunteerNum,
};
enum Wantedlogininfo
{
	eWa_LIMyWantedIDX, eWa_LIWantedIDX, eWa_LIWantedChrID, eWa_LIWantedName, eWa_LIRegistDate, 
};
enum QuestInfo
{
	eQi_QuestIdx, eQi_QuestState, eQi_EndParam, eQi_RegistTime, eQi_CheckType, eQi_CheckTime,
};
enum QuestSubDataInfo
{
	eQsi_QuestIdx, eQsi_SubQuestIdx, eQsi_Data, eQsi_RegistTime,
};
enum QuestItem
{
	eQit_ItemIdx, eQit_ItemCount, eQit_QuestIdx,
};
enum Journal
{
	eJnl_Index, eJnl_Kind, eJnl_Param, eJnl_Param_2, eJnl_Param_3, eJnl_bSaved, eJnl_ParamName, eJnl_RegDate,
};

enum ShopItem
{
	eMItm_ItemDBIdx, eMItm_ItemIdx, /*eMItm_Position,*/ eMItm_Durability, eMItm_Param, eMItm_Seal, eMItm_EndTime, eMItm_RemainTime,
};
enum ShopItemUseInfo
{
	eUMtm_ItemIdx, eUMtm_DBIdx, eUMtm_Param, eUMtm_BTime, eUMtm_RTime, eUMtm_Position,
};
enum MovePointInfo
{
	eMPt_DBIdx, eMPt_Name, eMPt_MapNum, eMPt_MapPoint,
};
enum AvatarItemOption
{
	eAO_OptionIdx, 
};
enum eGTInfo
{
	eGT_GuildIdx, eGT_Position, eGT_Ranking, eGT_Process, eGT_WaitTime,
};
enum eSW_Info
{
	eSWI_SiegeWarIdx, eSWI_RegistTime, eSWI_SiegeWarTime, eSWI_SiegeWarLevel,
};
enum eSW_GuildInfo
{
	eSWGI_GuildDBIdx, eSWGI_GuildIdx, eSWGI_GuildType,
};

enum eGuildNotice
{
	eGu_GNDBIdx, eGu_GNGuildIdx, eGu_GNGuildNotice,
};

//enum eFamilyNotice
//{
//	eFu_GNDBIdx, eFu_GNFamilyIdx, eFu_GNFamilyNotice,
//};

enum eQuick
{
	eQuick_CharacterIdx,
	eQuick_Sheet,
	eQuick_Kind,
	eQuick_DBIdx,
	eQuick_Idx,
	eQuick_Data,
};

// description by hseos S
// DB에서 데이터 가져오기 위한 절차
// description by hseos E

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.04.23
enum eFarm_FarmState
{
	eFarm_FarmState_Zone,
	eFarm_FarmState_Farm,
	eFarm_FarmState_State,
	eFarm_FarmState_OwnerID,
	eFarm_FarmState_GardenGrade,
	eFarm_FarmState_HouseGrade,
	eFarm_FarmState_WarehouseGrade,
	eFarm_FarmState_AnimalCageGrade,
	eFarm_FarmState_FenceGrade,
};

enum eFarm_CropInfo
{
	eFarm_CropInfo_Zone,
	eFarm_CropInfo_Farm,
	eFarm_CropInfo_Crop,
	eFarm_CropInfo_OwnerID,
	eFarm_CropInfo_Kind,
	eFarm_CropInfo_Step,
	eFarm_CropInfo_Life,
	eFarm_CropInfo_NextStepTime,
	eFarm_CropInfo_SeedGrade,
};

enum eFarm_AnimalInfo
{
	eFarm_AnimalInfo_Zone,
	eFarm_AnimalInfo_Farm,
	eFarm_AnimalInfo_Animal,
	eFarm_AnimalInfo_OwnerID,
	eFarm_AnimalInfo_Kind,
	eFarm_AnimalInfo_Step,
	eFarm_AnimalInfo_Life,
	eFarm_AnimalInfo_NextStepTime,
	eFarm_AnimalInfo_Contentment,
	eFarm_AnimalInfo_Interest,
	//eFarm_CropInfo_SeedGrade,
};
// E 농장시스템 추가 added by hseos 2007.04.23

// desc_hseos_몬스터미터01
// S 몬스터미터 추가 added by hseos 2007.05.28
enum eMonsterMeter_Info
{
	eMonsterMeter_PlayTime,
	eMonsterMeter_KillMonNum,
	eMonsterMeter_PlayTimeTotal,
	eMonsterMeter_KillMonNumTotal,
};
// E 몬스터미터 추가 added by hseos 2007.05.28

// desc_hseos_주민등록01
// S 주민등록 추가 added by hseos 2007.06.04
enum eResidentRegistInfo
{
	eResidentRegist_NickName,
	eResidentRegist_Sex,
	eResidentRegist_Age,
	eResidentRegist_Location,
	eResidentRegist_Favor,
	eResidentRegist_PropensityLike01,
	eResidentRegist_PropensityLike02,
	eResidentRegist_PropensityLike03,
	eResidentRegist_PropensityDisLike01,
	eResidentRegist_PropensityDisLike02,
	eResidentRegist_Introduction,
};
// E 주민등록 추가 added by hseos 2007.06.04

// desc_hseos_패밀리01
// S 패밀리 추가 added by hseos 2007.07.05
enum eFamily_Info
{
	eFamily_ID,
	eFamily_Name,
	eFamily_MasterID,
	eFamily_HonorPoint,
	eFamily_NicknameON,
};

enum eFamily_Member_Info
{
	eFamily_Member_FamilyID,
	eFamily_Member_MemberID,
	eFamily_Member_MemberNickname,
};
// E 패밀리 추가 added by hseos 2007.07.05

// 071023 LYW --- MapDBMsgParser : Add enum for tutorial info.
enum eTutorialInfo
{
	eTutorial_Idx, 
	eTutorial_IsComplete,
} ;

enum eUpdateDefaultStats
{
	eStrength, 
	eDexterity,
	eVitality,
	eIntelligence,
	eWisdom,
	eRemainPoint,
} ;

enum ePetObjectInfo
{
	ePetObjectInfo_ID,
	ePetObjectInfo_ItemDBIdx,
	ePetObjectInfo_MasterIdx,
	ePetObjectInfo_Kind,
	ePetObjectInfo_Level,
	ePetObjectInfo_Grade,
	ePetObjectInfo_SkillSlot,
	ePetObjectInfo_Exp,
	ePetObjectInfo_Friendly,
	ePetObjectInfo_HP,
	ePetObjectInfo_MAXHP,
	ePetObjectInfo_MP,
	ePetObjectInfo_MAXMP,
	ePetObjectInfo_AI,
	ePetObjectInfo_State,
	ePetObjectInfo_Type,
	ePetObjectInfo_CheckTime,
};

void TestQuery();
void CharacterNumSendAndCharacterInfo(DWORD CharacterIDX, DWORD Protocol);
void CharacterNumSendAndCharacterInfo2(DWORD CharacterIDX, DWORD dwTeam);
void CharacterSkillTreeInfo(DWORD CharacterIDX, DWORD Protocol);
void CharacterItemSlotInfo(DWORD CharacterIDX);
void CharacterItemInfo(DWORD CharacterIDX, DWORD Protocol);
void CharacterItemInfo( DWORD CharacterIDX );

void MapBaseEconomy(BYTE MapNum);
void AuctionRegist(BYTE ItemIdx, DWORD SellerID, DWORD EndDate, DWORD EndTime, DWORD StartPrice, DWORD ImmediatePrice, char Memo[256]);
void AuctionPageQuery(DWORD CharacterIDX, BYTE PageNum);

void SSItemInsert(DWORD CharacterIdx, DWORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, DWORD FromChrIdx, WORD wSeal=0,int Sealed = 0, int RemainSecond = 0);


/* New Item Qurey Func */
void ItemUpdateToDB(DWORD CharacterIdx, DWORD dwDBIdx, DWORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, WORD qPosition );
//void ItemStorageMunpaUpdateToDB(DWORD UserIDX, DWORD MunpaIDX, DWORD dwDBIdx, WORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, WORD qPosition);
void ItemCombineUpdateToDB(DWORD dwfromDBIdx, DURTYPE fromDur, DWORD dwToDBIdx, DURTYPE toDur, DWORD CharacterIdx);
void ItemMoveUpdateToDB(DWORD CharacterIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos, DWORD UserIdx, DWORD MunpaIdx);
void ItemMoveStorageUpdateToDB(DWORD CharacterIDX, DWORD UserIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos);
void ItemMoveGuildUpdateToDB(DWORD CharacterIDX, DWORD MunpaIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos);
//SW050920 RareIdx 추가
void ItemInsertToDB(DWORD CharacterIdx, DWORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, DWORD dwKey, WORD wSeal=0);
// 071231 KTH -- GetID Add
void ItemDeleteToDB(DWORD dwDBIdx);
// 080402 LUJ, 인자 이름을 아이템 DB인덱스로 변경
void ItemOptionDelete( DWORD dwDBIdx);
// 081118 LUJ, 캐릭터 번호를 쿼리를 실행한 DB 스레드를 선택할 키값으로 사용되도록 함
void ItemOptionInsert( const ITEM_OPTION&, DWORD CharacterIdx );
void ItemDropOptionInsert( const ITEM_OPTION&, DWORD CharacterIdx );

/* New Skill Qurey Func */
// 071231 KTH -- CharacterIDX Add
void SkillUpdateToDB( SKILL_BASE* msg, DWORD CharacterIDX );
void SkillMoveUpdateToDB(DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos, char* type, DWORD CharacterIDX);
void SkillInsertToDB(DWORD CharacterIDX, DWORD SkillIDX, BYTE level = 1/*, POSTYPE SkillPos, BYTE bWeared, BYTE bLevel, WORD Option = 0*/);
void SkillDeleteToDB(DWORD dwDBIdx);

void CheckAuction(MSGBASE* );
void AuctionSearch(SEARCHLIST* );
void AuctionSort(SORTLIST* );
void AuctionRegister(REGISTERAUCTION* );
void AuctionJoin(JOINAUCTION* );
void AuctionCancel(AUCTIONCANCEL *);
//////////////////////////////////////////////////////////////////////////
void PartyLoad(DWORD PartyIDX);
void PartyInfoByUserLogin(DWORD PartyIDX, DWORD CharacterIDX);
void PartyBreakup(DWORD PartyIDX, DWORD CharacterIDX);
void PartyCreate(DWORD MasterIDX, BYTE Option);
void PartyMemberUpdate(CParty* pParty, DWORD PartyIDx);
void PartyAddMember(DWORD PartyIDX, DWORD TargetID);
void PartyDelMember(DWORD PartyIDX, DWORD TargetID);
void PartyChangeMaster(DWORD PartyID, DWORD PlayerID, DWORD TargetPlayerID);

//////////////////////////////////////////////////////////////////////////
// MapChange
void ChangeMapGetCharacterInfo(DWORD CharacterIDX, DWORD dwConnectionIndex);
void ChangeMapGetCharacterSkillTreeInfo(DWORD CharacterIDX, DWORD dwConnectionIndex);
//////////////////////////////////////////////////////////////////////////


//guild
void GuildLoadGuild(DWORD StartGuildIdx);
void GuildLoadMember(DWORD StartCharacterIdx);
void GuildCreate( DWORD CharacterIdx, const char* CharacterName, LEVELTYPE CharacterLvl, const char* GuildName, const char* Intro, int masterLevel  );
void GuildBreakUp(DWORD GuildIdx, DWORD MasterIdx);
void GuildDeleteMember(DWORD GuildIdx, DWORD MemberIDX);
void GuildSecedeMember(DWORD GuildIdx, DWORD PlayerIdx);
void GuildAddMember(DWORD GuildIdx, DWORD playerIndex, const char* guildName, BYTE rank );
void RGuildAddMember( LPMIDDLEQUERY, LPDBMESSAGE );
void GuildMarkRegist(DWORD GuildIdx, char* ImgData, DWORD CharacterIdx);
//void GuildLevelUp(DWORD GuildIdx, BYTE Level);
void GuildChangeMemberRank(DWORD GuildIdx, DWORD MemberIdx, BYTE Rank);

void GuildGiveMemberNickName(DWORD TargetId, char * NickName);
void GuildUpdateNotice( DWORD guildIndex, const char*, DWORD CharacterIDX );
void StorageBuyStorage(DWORD PlayerIDX);


void CharacterStorageInfo(DWORD UserIdx, DWORD CharacterIdx);
//void CharacterStorageItemInfo(DWORD CharacterIdx, DWORD StartDBIdx);
// 071220 KTH
void CharacterStorageItemInfo(DWORD CharacterIdx, DWORD UserIdx, DWORD StartDBIdx);
// 071231 KTH -- CharacterIDX Add
void StorageMoneyUpdateToDB(DWORD UserIdx, MONEYTYPE StorageMoney, DWORD CharacterIDX);

void FriendNotifyLogouttoClient(DWORD PlayerID);
//////////////////////////////////////////////////////////////////////////
//void FameMunpaUpdate(DWORD StartMunpaIDX, DWORD MapNum);
void FameCharacterUpdate(DWORD PlayerIDX, FAMETYPE val);
void BadFameCharacterUpdate(DWORD PlayerIDX, FAMETYPE val);
void PKCharacterUpdate(DWORD PlayerIDX, DWORD val);
//////////////////////////////////////////////////////////////////////////
// SavePoint
void SavePointUpdate(DWORD CharacterIDX, WORD LoginPoint_Idx, WORD MapNum);
void SaveMapChangePointUpdate(DWORD CharacterIDX, WORD MapChangePoint_Idx);
 //맵 이동시 너무 느려 플레이어가 다른 맵에 들어가기 전에 업데이트 확인 후 맵 채인지 프로토콜을 보냄
// 081218 LUJ, 플레이어 소지금 추가
void SaveMapChangePointReturn(DWORD CharacterIDX, WORD MapChangePoint_Idx, DWORD dwConnectionIdx, LEVELTYPE lvl, EXPTYPE ExpPoint, MONEYTYPE inventoryMoney, MONEYTYPE StorageMoney);
// 070917 LYW --- MapDBMsgParser : Add function to save map change point to return.
// 081218 LUJ, 플레이어 소지금 추가
void SaveNpcMapChangePointReturn(DWORD CharacterIDX, WORD MapChangePoint_Idx, DWORD dwConnectionIdx, LEVELTYPE lvl, EXPTYPE ExpPoint, MONEYTYPE inventoryMoney, MONEYTYPE StorageMoney) ;
//////////////////////////////////////////////////////////////////////////
//Co≫o±Y
void WantedLoad(WANTEDTYPE StartWantedIDX);
void WantedInfoByUserLogIn(DWORD CharacterIDX);
void WantedBuyRight(DWORD CharacterIDX, WANTEDTYPE WantedIDX, MONEYTYPE AddPrize);
void WantedRegist(DWORD CharacterIDX, DWORD TargetIDX, MONEYTYPE Prize);
void WantedGiveUpRight(DWORD CharacterIDX, DWORD WantedIDX);
void WantedComplete(DWORD CharacterIDX, char* TargetName, WANTEDTYPE WantedIDX);
void WantedDestroyed(DWORD WantedIDX); // destroyed by time
void WantedOrderedList(DWORD CharacterIDX, DWORD Page, char* OrderType, BYTE PageUnit);
void WantedMyList(DWORD CharacterIDX, DWORD Page, BYTE PageUnit);
void WantedSearch(DWORD CharacterIDX, char* WantedName, DWORD Page, BYTE PageUnit);
//////////////////////////////////////////////////////////////////////////
// Update¹®
//void CharacterSkillUpdate(SKILLBASE* msg);
//void CharacterSkillMoveUpdate(DWORD toDBIdx, POSTYPE toPos, DWORD fromDBIdx, POSTYPE fromPos);
//void CharacterInfoUpdate(CPlayer*, DWORD dwConnectionIndex);
void CharacterHeroInfoUpdate(CPlayer*);
void CharacterTotalInfoUpdate(CPlayer*);
void CharacterBadFameUpdate(DWORD PlayerID, int BadFame, DWORD Money);
//void CharacterQuickInfoUpdate(CPlayer * pPlayer);
void CharacterSkillTreeInfoUpdate(CPlayer*);
void CharacterTablePartyIdxUpdate(DWORD PlayerID);
void StorageMoneyUpdate(DWORD PlayerID, BYTE StorageNum, DWORD Money);
void RegistLoginMapInfo(DWORD CharacterIDX, char* CharacterName, BYTE MapNum, WORD MapPort);
void UnRegistLoginMapInfo(DWORD CharacterIDX);
void CharacterLvlExpUpdate(DWORD PlayerID, LEVELTYPE level, EXPTYPE exppoint);
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Au½ºÆ® (AEº¸AU °¡AIμa Æ÷CO)
void QuestTotalInfo(DWORD PlayerID);
void QuestUpdateToDB(DWORD PlayerID, DWORD QuestIdx, QSTATETYPE State, BYTE bEnd);
//RaMa
void QuestItemload(DWORD PlayerID, int QuestIdx = -1 );
void QuestItemInsert(DWORD PlayerID, DWORD dwQuestIdx, DWORD ItemIdx, DWORD dwItemNum );
void QuestItemDelete(DWORD PlayerID, DWORD ItemIdx);
void QuestItemDeleteAll(DWORD PlayerID, DWORD dwQuestIdx);
void QuestItemUpdate(DWORD PlayerID, DWORD dwQuestIdx, DWORD ItemIdx, DWORD ItemNum);

void QuestMainQuestLoad(DWORD PlayerID, int QuestIdx = -1 );
void QuestMainQuestUpdateToDB(DWORD PlayerID, DWORD mQuestIdx, QSTATETYPE State, DWORD EndParam, DWORD Time);
void QuestMainQuestDelete(DWORD PlayerID, DWORD mQuestIdx);
void QuestMainQuestInsert(DWORD PlayerID, DWORD mQuestIdx, QSTATETYPE State, DWORD Time, BOOL repeat);

void QuestSubQuestLoad(DWORD PlayerID, int QuestIdx = -1 );

// 071117 LYW --- MapDBMsgParser : 퀘스트 관련 추가 함수.
void EndQuest_New(DWORD dwCharacter_Idx, DWORD dwQuest_Idx, DWORD dwSubQuest_Do, DWORD dwEnd_Param, DWORD dwRegist_Time) ;
void EndSubQuest_New(DWORD dwCharacter_Idx, DWORD dwQuest_Idx, DWORD dwSubQuestIdx, DWORD dwSubQuest_Do, DWORD dwRegist_Time) ;
void DeleteQuest_New(DWORD dwCharacter_Idx, DWORD dwQuest_Idx, DWORD param) ;

void QuestSubQuestInsert(DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx, QSTATETYPE Data, QSTATETYPE Time);
void QuestSubQuestUpdateToDB(DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx, QSTATETYPE Data, QSTATETYPE Time);
void QuestSubQuestDelete(DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx);
void QuestSubQuestDeleteAll(DWORD PlayerID, DWORD mQuestIdx);

void QuestMainQuestUpdateCheckTime(DWORD dwCharIdx, DWORD dwQuestIdx, DWORD dwCheckType, DWORD dwCheckTime );

// 071023 LYW --- MapDBMsgParser : Add Load query function.
void TutorialLoad(DWORD dwPlayerID, int tutorialIdx = -1, BOOL isComplete = false) ;
// 071023 LYW --- MapDBMsgParser : Add Update query function.
void TutorialUpdateToDB(DWORD dwPlayerID, int tutorialIdx, BOOL isComplete) ;
// 071106 LYW --- MapDBMsgParser : 기본 스탯으로 업데이트 하는 함수.
void DB_UpdateToDefaultStats(DWORD dwPlayerID, int nStr, int nDex, int nVit, int nInt, int nWis, int nRemainPoint) ;

// 071129 LYW --- MapDBMsgParser : 스킬 초기화 쿼리 함수 추가.
void DB_ResetToDefaultSkill(DWORD dwPlayerID) ;

// 071129 LYW --- MapDBMsgParser : 누적 스킬 포인트 업데이트 함수 추가.
void DB_UpdateAccumulateSkillPoint(DWORD dwPlayerID, BOOL bForce, int nPoint = 0) ;

//////////////////////////////////////////////////////////////////////////
//journal
void JournalGetList(DWORD CharacterIDX);
void JouranlInsertQuest(DWORD CharacterIDX, BYTE Kind, DWORD QuestIDX, DWORD SubQuestIDX, int bCompleted);
void JouranlInsertWanted(DWORD CharacterIDX, BYTE Kind, DWORD Param, char* ParamName);
void JournalInsertLevel(DWORD CharacterIDX, BYTE Kind, DWORD Level);
void JournalSaveUpdate(DWORD CharacterIDX, DWORD JournalIndex);
void JournalDelete(DWORD CharacterIDX, DWORD JournalIndex);
//////////////////////////////////////////////////////////////////////////
// ShopItem
void ShopItemInvenInfo( DWORD CharacterIdx );
void CharacterShopItemInfo(DWORD CharacterIDX, DWORD StartDBIdx);
void UsingShopItemInfo( DWORD CharacterIdx );
void SavedMovePointInfo( DWORD CharacterIdx );
void SavedMovePointInsert( DWORD CharacterIdx, char* Name, DWORD MapIdx, DWORD Position );
void SavedMovePointUpdate( DWORD CharacterIdx, DWORD DBIdx, char* Name );
void SavedMovePointDelete( DWORD CharacterIdx, DWORD DBIdx );
void UsingShopItemUpdateToDB( DWORD UserIdx, DWORD CharacterIdx, DWORD dwItemIdx, DWORD dwDBIdx, DWORD Param, DWORD StartTime, DWORD RemainTime );
void ShopItemDeleteToDB( DWORD CharacterIdx, DWORD dwDBIdx );
void ShopItemUpdatetimeToDB( DWORD CharacterIdx, DWORD dwItemIdx, DWORD RemainTime );
void ShopItemParamUpdateToDB( DWORD CharacterIdx, DWORD dwItemIdx, DWORD Param );
void ShopItemUseParamUpdateToDB( DWORD UserIdx, DWORD CharacterIdx, DWORD dwItemIdx, DWORD Param );
void ShopItemGetItem( DWORD CharacterIdx, WORD wItemIdx, DWORD Dur );
void ItemMoveShopUpdateToDB(DWORD CharacterIDX, DWORD UserIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos);
void ShopItemUpdateUseInfoToDB( DWORD CharacterIdx, DWORD dwDBIdx, DWORD Param, DWORD RemainTime );
void CharacterChangeName( DWORD CharacterIdx, char* Name, DWORD DBIdx );
void CharacterChangeInfoToDB( DWORD CharacterIdx, DWORD Gender, DWORD HairType, DWORD FaceType, float Height, float Width );

// 로그
void InsertLogCharacter( DWORD charIdx, LEVELTYPE level, HERO_TOTALINFO*  );
void InsertLogExp( BYTE bType, DWORD charIdx, LEVELTYPE level, EXPTYPE changeValue, EXPTYPE NowExp, WORD MurdererKind, DWORD MurdererIdx, DWORD CurAbilPoint);
void InsertLogMoney( BYTE type, DWORD charIdx, MONEYTYPE InvenMoney, MONEYTYPE ChangeValue, DWORD TargetID, MONEYTYPE TargetMoney );
void InsertLogWorngMoney( BYTE type, DWORD charIdx, MONEYTYPE changeValue, MONEYTYPE InvenMoney, MONEYTYPE StorageMoney, DWORD targetIdx );

//void InsertLogSkill( WORD type, DWORD charIdx, DWORD skillIdx, DWORD skillDBIdx, LEVELTYPE skillLevel, EXPTYPE skillExp );

// 071109 웅주
void InsertLogSkill( CPlayer*, const SKILL_BASE*, DWORD type );
void InsertLogGuildWarehouse( CPlayer*, POSTYPE previousPosition, const ITEMBASE*, MONEYTYPE, eLogitemmoney );
void InsertLogJob( CPlayer*, DWORD classIndex, DWORD jobIndex, DWORD jobGrade );
void InsertLogGuildScore( DWORD guildIndex, float score, eGuildLog );
void InsertLogFarmCrop( CSHFarm*, CSHCrop*,  eFamilyLog );
// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2008.01.22
void InsertLogFarm( DWORD nZone, DWORD nFarm, DWORD nKind, DWORD nPlayerID, DWORD nValue01 = 0, DWORD nValue02 = 0);
// E 농장시스템 추가 added by hseos 2008.01.22

// 071129 웅주
// index는 몬스터 또는 아이템 인덱스일 수 있음
// variation은 몬스터 사냥 수 혹은 아이템 개수
void InsertLogQuest( CPlayer*, DWORD mainQuestIndex, DWORD subQuestIndex, int index, int variation, eQuestLog );

void InsertLogSkillExp( DWORD charIdx, EXPTYPE changeValue, DWORD skillIdx, DWORD skillDBIdx, LEVELTYPE skillLevel, EXPTYPE skillExp, POSTYPE skillPos );
void LogItemMoney(DWORD FromChrIdx, char* FromChrName, DWORD ToChrIdx, char* ToChrName,
				eLogitemmoney LogType, MONEYTYPE FromTotalMoney, MONEYTYPE ToTotalMoney, MONEYTYPE ChangeMoney,
				DWORD ItemIdx, DWORD ItemDBIdx, POSTYPE ItemFromPos, POSTYPE ItemToPos,
				DURTYPE ItemDur, EXPTYPE ExpPoint);
void LogItemOption(DWORD CharacterIdx, DWORD ItemDBIdx, const ITEM_OPTION&, eLogitemmoney type );
// 080327 LUJ, 드롭 옵션 로그 저장
void LogItemDropOption( const ITEM_OPTION&, eLogitemmoney );
void LogCheat(DWORD Character_Idx, BYTE LogType, MONEYTYPE Param, DWORD ItemIdx, DWORD ItemDBIdx);
void InsertLogTool( DWORD dwLogType, DWORD dwLogKind, DWORD dwOperIdx, char* sOperName, DWORD dwTargetIdx, char* sTargetName,
				    DWORD dwParam1, DWORD dwParam2, DWORD dwParam3, DWORD dwParam4 );
void LogGuild(DWORD CharacterIdx, DWORD GuildIdx, WORD LogType, DWORD Param1 = 0, DWORD Param2 = 0);
void LogFamily(DWORD CharacterIdx, DWORD FamilyIdx, WORD LogType, DWORD Param1 = 0, DWORD Param2 = 0);
void LogGMToolUse( DWORD CharacterIdx, DWORD GMLogtype, DWORD Logkind, DWORD Param1, DWORD Param2 );
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
void UpdateCharacterInfoByTime(DWORD charIdx, EXPTYPE exp, /*어빌리티 삭제 - 이영준 EXPTYPE abilityExp, */MONEYTYPE money, DWORD time, DWORD LastPKEndTime );
void UpdateSkillExpByTime(DWORD charIdx, DWORD DBIdx, EXPTYPE exp);

void InsertSpeedHackCheck(DWORD charIdx, char* charName, DWORD Distance, DWORD skillDelay);

// 071122 LYW --- MapDBMsgParser : 봉인 해제 처리.
void UnSealItemByRealTime(DWORD dwPlayerID, DWORD dwDBIdx, DWORD dwTime) ;
void UnSealItemByGameTime(DWORD dwPlayerID, DWORD dwDBIdx, DWORD dwTime) ;
void UpdateRemainTime(DWORD dwPlayerID, DWORD dwDBIdx, DWORD dwTime) ;
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// DB·IºIAI ¸Þ½AAo¸| ¹Þ¾Æ¼­ A³¸®CI´A CO¼oμe.
void ReceiveMessageFromDB(LPQUERY,  LPDBMESSAGE);
void RTestQuery(LPQUERY, LPDBMESSAGE);
void RCharacterNumSendAndCharacterInfo(LPQUERY, LPDBMESSAGE);
void RCharacterInfoQueryToEventMap(LPQUERY, LPDBMESSAGE);
void RCharacterSkillTreeInfo(LPQUERY, LPDBMESSAGE);
void RCharacterItemSlotInfo(LPQUERY, LPDBMESSAGE);
void RCharacterItemInfo(LPQUERY, LPDBMESSAGE);
void RCharacterItemOption(LPQUERY, LPDBMESSAGE);
/*어빌리티 삭제 - 이영준 void RCharacterAbilityInfo(LPMIDDLEQUERY pData, LPDBMESSAGE);*/

//void RCharacterGMControlInfo(LPMIDDLEQUERY pData, LPDBMESSAGE);

//void RCharacterGMControlInsert(LPMIDDLEQUERY pData, LPDBMESSAGE);

void RMapBaseEconomy(LPQUERY, LPDBMESSAGE);
void RAuctionPageQuery(LPQUERY, LPDBMESSAGE);

/* Item return func */
void RCharacterItemGetBuyIDX(LPQUERY, LPDBMESSAGE);
void RCharacterItemInsert(LPQUERY, LPDBMESSAGE);
void RSSItemInsert(LPQUERY, LPDBMESSAGE); // LBS 03.10.09
void RCharacterItemGetCheatIDX(LPQUERY, LPDBMESSAGE);
void RCharacterItemGetDivideIDX(LPQUERY, LPDBMESSAGE);
void RCharacterDivideNewItemIDX(LPQUERY, LPDBMESSAGE);
void RCharacterItemGetHuntIDX(LPQUERY, LPDBMESSAGE);
void RCharacterDupItemInsertIDX(LPQUERY, LPDBMESSAGE);
void RShopItemInvenInfo( LPQUERY, LPDBMESSAGE );
void RUsingShopItemInfo( LPQUERY, LPDBMESSAGE );
void RSavedMovePointInfo( LPMIDDLEQUERY pData, LPDBMESSAGE );
void RSavedMovePointInsert( LPMIDDLEQUERY pData, LPDBMESSAGE );
void RCharacterChangeName( LPQUERY, LPDBMESSAGE );

/* New Item Return Func */
void RItemInsert(LPQUERY, LPDBMESSAGE);

/* New Skill Return Func */
void RSkillInsert(LPQUERY, LPDBMESSAGE);

//pjs ¿ⓒ±a¿¡´A ACUTION¿¡ ±­·AμC Ai¶￠i¾¹® ¹YE? A³¸® CO¼o ¼±¾ð..
void RAuctionCheck(LPQUERY, LPDBMESSAGE);
void RAuctionPageList(LPQUERY, LPDBMESSAGE);
void RAuctionConfirm(LPQUERY, LPDBMESSAGE);
//////////////////////////////////////////////////////////////////////////
//ÆAÆ¼
void RPartyLoad(LPQUERY, LPDBMESSAGE);
void RPartyCreate(LPQUERY, LPDBMESSAGE);
void RPartyInfoByUserLogin(LPQUERY, LPDBMESSAGE);
void RPartyChangeMaster(LPQUERY, LPDBMESSAGE);
void RPartyDelMember(LPQUERY, LPDBMESSAGE);
//////////////////////////////////////////////////////////////////////////

void RCharacterStatsHistory(LPQUERY, LPDBMESSAGE);
void RSavePointUpdate(LPQUERY, LPDBMESSAGE);

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//guild
void RGuildLoadGuild(LPQUERY, LPDBMESSAGE);
void RGuildLoadMember(LPQUERY, LPDBMESSAGE);
void RGuildLoadMark(LPLARGEQUERY pData, LPDBMESSAGE);
void RGuildCreate(LPMIDDLEQUERY, LPDBMESSAGE);
void RGuildBreakUp( LPQUERY, LPDBMESSAGE );
void RGuildDeleteMember( LPQUERY, LPDBMESSAGE );
void RGuildSecedeMember( LPQUERY, LPDBMESSAGE );
//void RGuildItemLoad( LPQUERY, LPDBMESSAGE );
//void RGuildItemOption(LPQUERY, LPDBMESSAGE);
void RGuildMarkRegist(LPLARGEQUERY pData, LPDBMESSAGE);
//void RGuildLoadNotice(LPMIDDLEQUERY pData, LPDBMESSAGE);
//////////////////////////////////////////////////////////////////////////

void RCharacterStorageInfo(LPQUERY, LPDBMESSAGE);
void RCharacterStorageItemInfo(LPQUERY, LPDBMESSAGE);

//////////////////////////////////////////////////////////////////////////
void RFriendNotifyLogouttoClient(LPQUERY, LPDBMESSAGE);

//////////////////////////////////////////////////////////////////////////
//¸i¼º

//////////////////////////////////////////////////////////////////////////
//Co≫o±Y
void RWantedLoad(LPQUERY, LPDBMESSAGE);
void RWantedInfoByUserLogIn(LPQUERY, LPDBMESSAGE);
void RWantedRegist(LPQUERY, LPDBMESSAGE);
void RWantedComplete(LPQUERY, LPDBMESSAGE);
void RWantedDestroyed(LPQUERY, LPDBMESSAGE);
void RWantedOrderedList(LPQUERY, LPDBMESSAGE);
void RWantedMyList(LPQUERY, LPDBMESSAGE);
void RWantedSearch(LPQUERY, LPDBMESSAGE);
//////////////////////////////////////////////////////////////////////////
// Au½ºÆ® (AEº¸AU °¡AIμa Æ÷CO)
void RQuestTotalInfo(LPQUERY, LPDBMESSAGE);
void RQuestMainQuestLoad(LPQUERY, LPDBMESSAGE);
void RQuestSubQuestLoad(LPQUERY, LPDBMESSAGE);
void RQuestItemLoad(LPQUERY, LPDBMESSAGE);
//////////////////////////////////////////////////////////////////////////
void RJournalGetList(LPQUERY, LPDBMESSAGE);
//////////////////////////////////////////////////////////////////////////
void RSaveMapChangePointReturn(LPQUERY, LPDBMESSAGE);
//////////////////////////////////////////////////////////////////////////
//
void RCharacterShopItemInfo(LPQUERY, LPDBMESSAGE);

//void UpdateCharacterMoney( DWORD dwCharIdx, MONEYTYPE money, BYTE flag );
//void RUpdateCharacterMoney( LPQUERY, LPDBMESSAGE );

void ConfirmUserOut( DWORD dwConnectIdx, DWORD dwUserIdx, DWORD dwCharIdx, DWORD dwKind );
void RConfirmUserOut( LPQUERY, LPDBMESSAGE );


void SaveCharInfoBeforeLogout(DWORD PlayerIdx, DWORD dwConnectionIndex, DWORD UserIdx, MONEYTYPE StorageMoney);
void RSaveCharInfoBeforeLogout( LPQUERY, LPDBMESSAGE );

// guildfieldwar
void LoadGuildWarRecord( DWORD dwIdx );
void RLoadGuildWarRecord( LPQUERY, LPDBMESSAGE );
void DeleteGuildWarRecord( DWORD dwGuildIdx );
void LoadGuildWar( DWORD dwIdx );
void RLoadGuildWar( LPQUERY, LPDBMESSAGE );
void InsertGuildWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2, DWORD dwMoney );
void DeleteGuildWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2 );
void UpdateGuildWarRecordToDB( DWORD dwGuildIdx, DWORD dwVitory, DWORD dwDraw, DWORD dwLoose );

// Guild Tournament
void GuildTournamentInfoLoad();
void GuildTournamentCancelIdxLoad( DWORD TournamentIdx );
void GuildTournamentCancelIdxInsert( DWORD TournamentIdx, DWORD GuildIdx );
void GuildTournamentGuildInfoInsert( DWORD GuildIdx, char* GuildName, DWORD Position );
void GuildTournamentGuildInfoDelete( DWORD GuildIdx );
void GuildTournamentGuildInfoUpdate( DWORD GuildIdx, DWORD Ranking, DWORD ProcessTournament );
void GuildTournamentInfoUpdate( DWORD TournamentIdx, DWORD TournamentFight, DWORD WaitTime );
void RGuildTournamentInfoLoad( LPQUERY, LPDBMESSAGE );
void RGuildTournamentCancelIdxLoad( LPQUERY, LPDBMESSAGE );
void GuildTournamentAddLog( DWORD TournamentIdx, DWORD GuildIdx, DWORD Logkind, DWORD Ranking );

// Jackpot
//void JackpotLoadTotalMoney();
//void RJackpotLoadTotalMoney( LPQUERY, LPDBMESSAGE );
void JackpotAddTotalMoney( MONEYTYPE MapMoney );
//void RJackpotAddTotalMoney( LPQUERY, LPDBMESSAGE );
void JackpotPrizeInfo( MONEYTYPE MapMoney, DWORD PlayerID, DWORD PrizeKind, DWORD PrizePercentage, DWORD dwAbsMoney = 0);
void RJackpotPrizeInfo( LPQUERY, LPDBMESSAGE );

// guildunion
void GuildUnionLoad( DWORD dwStartGuildUnionIdx );
void RGuildUnionLoad( LPQUERY, LPDBMESSAGE );
void GuildUnionLoadMark( DWORD dwMarkIdx );
void RGuildUnionLoadMark( LPLARGEQUERY pData, LPDBMESSAGE );

void GuildUnionCreate( DWORD dwCharacterIdx, DWORD dwGuildIdx, char* sUnionName );
void RGuildUnionCreate( LPQUERY, LPDBMESSAGE );
void GuildUnionDestroy( DWORD dwGuildUnionIdx, DWORD dwMarkIdx, DWORD dwMasterIdx, DWORD dwTime );
void GuildUnionAddGuild( DWORD dwGuildUnionIdx, int nIndex, DWORD dwGuildIdx );
void GuildUnionRemoveGuild( DWORD dwGuildUnionIdx, int nIndex, DWORD dwGuildIdx );
void GuildUnionSecedeGuild( DWORD dwGuildUnionIdx, int nIndex, DWORD dwGuildIdx, DWORD dwTime );

void GuildUnionMarkRegist( DWORD dwCharacterIdx, DWORD dwMarkIdx, DWORD dwGuildUnionIdx, char* pImgData );
void RGuildUnionMarkRegist( LPLARGEQUERY pData, LPDBMESSAGE );

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//┃SiegeWar													  ┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
void SiegeWarInfoLoad( DWORD MapNum );
void RSiegeWarInfoLoad( LPQUERY, LPDBMESSAGE );
void SiegeWarInfoInsert( DWORD SiegeWarIdx, DWORD RegistTime, DWORD SiegeWarTime, DWORD Level, DWORD MapNum );
void SiegeWarInfoUpdate( DWORD SiegeWarIdx, DWORD RegistTime, DWORD SiegeWarTime, DWORD Level, DWORD MapNum );
void SiegeWarGuildInfoLoad( DWORD StartDBIdx, DWORD SiegeWarIdx, DWORD MapNum );
void RSiegeWarGuildInfoLoad( LPQUERY, LPDBMESSAGE );
void SiegeWarGuildInsert( DWORD SiegeWarIdx, DWORD GuildIdx, DWORD Type, DWORD MapNum );
void SiegeWarGuildUpdate( DWORD SiegeWarIdx, DWORD GuildIdx, DWORD Type, DWORD MapNum );
void SiegeWarGuildDelete( DWORD SiegeWarIdx, DWORD GuildIdx, DWORD MapNum );
void SiegeWarAddLog( DWORD SiegeWarIdx, DWORD MapNum, DWORD GuildIdx, DWORD Logkind, DWORD Value );

// For JAPAN
void ChangeCharacterAttr( DWORD dwPlayerIdx, DWORD dwAttr );

// siegewarprofit
void SiegeWarProfitInfoLoad();
void RSiegeWarProfitInfoLoad( LPQUERY, LPDBMESSAGE );
void SiegeWarProfitUpdateGuild( int nMapNum, DWORD dwGuildIdx );
void SiegeWarProfitUpdateTaxRate( int nMapNum, int nTaxRate, DWORD dwRegDate );
void SiegeWarProfitUpdateProfitMoney( int nMapNum, DWORD dwMoney, int nType = 0 );
void RSiegeWarProfitUpdateProfitMoney( LPQUERY, LPDBMESSAGE );

void ChangeGuildLocation( DWORD dwGuildIdx, WORD wMapNum );


/*어빌리티 삭제 - 이영준 void ChangeCharacterStageAbility( DWORD dwCharacterIdx, BYTE stage, CAbilityGroup* pGroup );*/

void QuestEventCheck( DWORD dwUserIdx, DWORD dwCharacterIdx, DWORD dwNpcIdx, DWORD dwQuestIdx, char* pTime );
void RQuestEventCheck( LPQUERY, LPDBMESSAGE );

void QuestEventEnd( DWORD dwUserIdx, DWORD dwCharacterIdx, char* pTime );

// event 060627 - 운영팀 이벤트
void GMEvent01( DWORD dwUserIdx, DWORD dwCharacterIdx, DWORD dwLevel, int nServerSetNum );
void QuickInfo( DWORD characterIdx );
void RQuickInfo( LPQUERY, LPDBMESSAGE );
void QuickUpdate( DWORD characterIdx, BYTE sheet, SLOT_INFO* pSlot );

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.04.23	2007.09.12	2008.01.16
void FarmLoadFarmInfo(WORD nMapNum);
BOOL FarmLoadFarmInfoNew(WORD nMapNum);
void RFarm_LoadFarmState( LPQUERY, LPDBMESSAGE );
void RFarm_LoadCropInfo( LPQUERY, LPDBMESSAGE );

//--------------------------------------------------------------------------------------
// 080416 KTH -- 가축 정보
void RFarm_LoadAnimalInfo(LPQUERY pData, LPDBMESSAGE pMessage);
// 080425 KTH -- 가축 로그
void InsertLogFarmAnimal( CSHFarm*, CAnimal*,  eFamilyLog );
//--------------------------------------------------------------------------------------

void Farm_LoadTimeDelay(DWORD nPlayerID);
void Farm_SetTimeDelay(DWORD nPlayerID, DWORD nKind, DWORD nRemainSecond);
void RFarm_LoadTimeDelay(LPQUERY pData, LPDBMESSAGE pMessage);

void Farm_SetTaxInfo(DWORD nFarmZone, DWORD nFarmID, DWORD nTaxArrearageFreq, DWORD nTaxPayPlayerID);
// E 농장시스템 추가 added by hseos 2007.04.23	2007.09.12	2008.01.16

// desc_hseos_몬스터미터01
// S 몬스터미터 추가 added by hseos 2007.05.28
void MonsterMeter_Load(DWORD nPlayerID);
void MonsterMeter_Save(DWORD nPlayerID, DWORD nPlayTime, DWORD nKillMonNum, DWORD nPlayTimeTotal, DWORD nKillMonNumTotal);
void RMonsterMeter_LoadInfo( LPQUERY, LPDBMESSAGE );
// E 몬스터미터 추가 added by hseos 2007.05.28

// desc_hseos_주민등록01
// S 주민등록 추가 added by hseos 2007.06.04
void ResidentRegist_LoadInfo(DWORD nPlayerID);
void ResidentRegist_SaveInfo(DWORD nPlayerID, void* _pInfo, int nIntroduction = 0);
void RResidentRegist_LoadInfo(LPMIDDLEQUERY pData, LPDBMESSAGE);
// 080215 KTH -- Reset Resident Regist
void ResidentRegist_Reset(DWORD nPlayerID);
void RResidentRegist_ResetResult(LPQUERY pData, LPDBMESSAGE pMessage);
// E 주민등록 추가 added by hseos 2007.06.04

void SkillPointUpdate(DWORD CharacterIdx, DWORD SkillPoint);

void CharacterJobUpdate( DWORD CharacterIdx, WORD Grade, WORD Job1, WORD Job2, WORD Job3, WORD Job4, WORD Job5, WORD Job6 );

// 070731 웅주, 길드 점수를 추가
void GuildAddScore( DWORD guildIndex, float score );
void RGuildAddScore( LPQUERY, LPDBMESSAGE );

// 080417 LUJ, 길드 스킬 획득
void RGuildAddSkill( LPMIDDLEQUERY, LPDBMESSAGE );

// 080417 LUJ, 길드 스킬 로딩
void RGuildGetSkill( LPMIDDLEQUERY, LPDBMESSAGE );

// 071005 웅주, 다른 길드원을 죽이거나 죽었을때 길드 포인트 갱신
void GuildUpdateScore( CPlayer* killer, CPlayer* corpse, float killerPoint, float corpsePoint );
void RGuildUpdateScore( LPQUERY, LPDBMESSAGE );

// 070801 웅주, 접속 시 해당 플레이어에게 전송할 길드 정보를 쿼리한다.
//void GuildReadMember( DWORD playerIndex, DWORD guildIndex );
//void RGuildMemberSelect( LPQUERY, LPDBMESSAGE );

// 길드 기본 정보를 가져옴
void GuildReadData( DWORD playerIndex, DWORD guildIndex );
void RGuildReadData( LPMIDDLEQUERY, LPDBMESSAGE );

// 길드 레벨 상승
void GuildSetLevel( DWORD guildIndex, DWORD playerIndex, DWORD requiredMoney, DWORD requiredScore, DWORD requiredQuest, DWORD warehouse_size, DWORD level );
void RGuildSetLevel( LPMIDDLEQUERY, LPDBMESSAGE );

// 길드 창고 초기화
void GuildResetWarehouse( MAPTYPE mapIndex );

// 길드 창고를 열거나 닫는다
void GuildOpenWarehouse( DWORD guildIndex, DWORD playerIndex, MAPTYPE );
void RGuildOpenWarehouse( LPMIDDLEQUERY, LPDBMESSAGE );

void GuildCloseWarehouse( DWORD guildIndex, DWORD playerIndex, MAPTYPE );
void RGuildCloseWarehouse( LPMIDDLEQUERY, LPDBMESSAGE );

// 길드 아이템 및 옵션 정보를 읽는다
void GuildItemSelect( DWORD playerIndex, DWORD guildIndex, DWORD beginItemDbIndex );
void RGuildItemSelect( LPQUERY, LPDBMESSAGE );

// 길드 창고 사용 가능 권한을 부여한다
void GuildSetWarehouseRank( DWORD guildIndex, DWORD playerIndex, BYTE rank );
void RGuildSetWarehouseRank( LPMIDDLEQUERY, LPDBMESSAGE );

// 길드 마스터를 바꾼다
void GuildChangeMaster( DWORD guildIndex, DWORD newMasterPlayerIndex, BYTE masterRank, BYTE exMasterRank );
void RGuildChangeMaster( LPMIDDLEQUERY, LPDBMESSAGE );

// 길드 창고의 돈을 업데이트한다
// 071218 LUJ, 길드 창고 입출금 기능 제거
//void GuildMoneyUpdate(DWORD GuildIdx, MONEYTYPE Money, BOOL isDeposit, MONEYTYPE maxMoney );
//void RGuildMoneyUpdate( LPQUERY, LPDBMESSAGE );

// 071011 웅주, 길드전에 따른 점수를 처리한다
void GuildSelectForRewardByWar( DWORD winnerGuildIndex, DWORD winnerUnionIndex, DWORD loserGuildIndex, DWORD loserUnionIndex );
void RGuildSelectForReward( LPQUERY, LPDBMESSAGE );

void CharacterLogoutPointUpdate( DWORD characterIdx, WORD map, DWORD pos_x, DWORD pos_y );

// 070917 LYW --- MapDBMsgParser : Add function to change map point to return result.
void RSaveNpcMapChangePointReturn(LPQUERY pData, LPDBMESSAGE pMessage) ;

// 071023 LYW --- MapDBMsgParser : 튜토리얼 로드 쿼리문 처리에 대한 결과 처리를 하기 위한 함수.
void RTutorialLoad(LPQUERY pData, LPDBMESSAGE pMessage) ;

// 071023 LYW --- MapDBMsgParser : 튜토리얼 업데이트 쿼리문 처리에 대한 결과 처리를 하기 위한 함수.
void RTutorialUpdate(LPQUERY pData, LPDBMESSAGE pMessage) ;

// 071207 LYW --- MapDBMsgParser : 스탯 초기화 프로시저 실행 결과 함수.
void RUpdateToDefaultStats(LPQUERY pData, LPDBMESSAGE pMessage) ;

// 071129 LYW --- MapDBMsgParser : 스킬 초기화 프로시저 실행 결과 함수.
void RSkill_Reset(LPQUERY pData, LPDBMESSAGE pMessage) ;
// 080602 LUJ, 전서버 대상 스킬이 사용 가능한지 알린다
void RSkillCheckUsable( LPMIDDLEQUERY, LPDBMESSAGE );

// 080602 LUJ, 패밀리 멤버 목록을 쿼리해서 스킬을 적용한다. 패밀리 정보가 맵 서버에 없고
//				에이전트에 있기 때문에 번거롭지만 DB에서 조회하는 편이 편하다.
//
//				주의: 미들 쿼리이므로 최대 20개밖에 쿼리하지 못한다. 그러나 패밀리 최대 인원이 다섯명이므로 사용한다
void RSkillAddToFamily( LPMIDDLEQUERY, LPDBMESSAGE );
// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.11.26	2007.11.27	2007.12.01	2008.01.23
void ChallengeZone_SavePos_Save(DWORD nPlayerID, DWORD nMapNum, DWORD nPosX, DWORD nPosY);
void ChallengeZone_EnterFreq_Save(DWORD nPlayerID, DWORD nEnterFreq, DWORD nEnterBonusFreq);
void ChallengeZone_EnterFreq_Load(DWORD nPlayerID);
void RChallengeZone_EnterFreq_Load(LPQUERY pData, LPDBMESSAGE pMessage);
void ChallengeZone_Success_Save(DWORD nPlayerID, DWORD nPartnerID, UINT nSection, UINT nClearTime);
void ChallengeZone_ClearNo1_Load(DWORD nPlayerID, UINT nSection);
void RChallengeZone_ClearNo1_Load(LPQUERY pData, LPDBMESSAGE pMessage);
// E 데이트 존 추가 added by hseos 2007.11.26	2007.11.27	2007.12.01	2008.01.23
void CharacterBuffLoad( DWORD characteridx );
void RCharacterBuffLoad(LPQUERY pData, LPDBMESSAGE pMessage);
void CharacterBuffAdd( DWORD characteridx, DWORD skillidx, DWORD remaintime, WORD count, BOOL isEndDate );
void CharacterBuffUpdate( DWORD characteridx, DWORD skillidx, DWORD remaintime, WORD count );
void CharacterBuffRemove( DWORD characteridx, DWORD skillidx );

void ConnectionCheck();

// 071210 KTH -- 인벤토리 확장
void IncreaseCharacterInventory( DWORD characteridx );
// 071210 KTH -- 확장 인벤토리 결과값을 받아온다.
void RIncreaseCharacterInventory(LPQUERY pData, LPDBMESSAGE pMessage);
// 071212 KTH -- 확장 인벤토리를 초기화 시켜준다.
void ResetCharacterInventory( DWORD characteridx );
// 071221 KTH -- 위치 기억을 불러 온다.
/*void MemoryStoneInfoLoad( int characterIdx );
void RMemoryStoneInfoLoad(LPQUERY pData, LPDBMESSAGE pMessage);
void MemoryStoneInfoAdd( int characterIdx, char* pMemoryName, MAPTYPE mapNum, float posX, float posZ, int positionIdx );
*/
// 071224 KTH -- 케릭터 이름 변경
void ChangeCharacterName( char* szName, int characterIdx, DWORD itemIdx, POSTYPE pos);
void RChangeCharacterName(LPQUERY pData, LPDBMESSAGE pMessage);
/*
// desc_hseos_결혼_01
// S 결혼 추가 added by hseos 2007.12.11
void Marriage_Load(DWORD nPlayerID);
void RMarriage_Load(LPQUERY pData, LPDBMESSAGE pMessage);
void Marriage_Save(DWORD nPlayerID, DWORD nPartnerID, UINT nTeleportFreq);
void Marriage_Divorce_Save(DWORD nPlayerID);
void Marriage_TeleportFreq_Save(DWORD nPlayerID, UINT nTeleportFreq);
void Marriage_Nickname_Save(DWORD nPlayerID, char* pszNickname);
// E 결혼 추가 added by hseos 2007.12.11
*/
//---KES AUTONOTE
void AutoNoteListAdd( DWORD dwCharacterIdx, char* pStrAutoName, DWORD dwAutoCharacterIdx, DWORD dwAutoUserIdx );
void RAutoNoteListAdd( LPQUERY pData, LPDBMESSAGE pMessage );
void AutoNoteListLoad( DWORD dwCharacterIdx  );
void RAutoNoteListLoad( LPQUERY pData, LPDBMESSAGE pMessage );
//---------------

// 080204 KTH -- 이벤트 관련 쿼리
void UpdateEventFromGame( DWORD dwCharacterIdx, char* pCharacterName );

void GiftItemInsert( DWORD characterIdx, DWORD useridx, DWORD eventIdx, DWORD itemIdx, DWORD count, BOOL isStack );
void RGiftItemInsert( LPQUERY pData, LPDBMESSAGE pMessage );

// 080424 NYJ --- 낚시관련
void FishingData_Load(DWORD dwPlayerID);
void RFishingData_Load(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);

// 080602 LYW --- Player : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
//void FishingData_Update(DWORD dwPlayerID, WORD wFishingLevel, DWORD dwFishingExp, DWORD dwFishPoint)
void FishingData_Update(DWORD dwPlayerID, WORD wFishingLevel, EXPTYPE dwFishingExp, DWORD dwFishPoint) ;
// 080808 LUJ, 낚시 경험치/레벨 추가, 더 이상 낚시 포인트만 저장하는 로그가 아니므로 함수명을 변경
void Log_Fishing(DWORD dwPlayerID, CHAR cLogType, DWORD dwDiffPoint, DWORD dwTotalPoint, DWORD dwBuyItemIdx, DWORD dwUseFish, EXPTYPE dwExp, DWORD dwLevel );

// 080616 LUJ, 함정이 공격할 적 길드원을 추가한다
void RAddEnemyToTrap( LPMIDDLEQUERY, LPDBMESSAGE );

void PetInfoLoad( DWORD characterIdx, DWORD userIdx );
void PetInfoAdd( DWORD characterIdx, DWORD userIdx, DWORD itemDBIdx, WORD kind, BYTE SkillSlot );
void PetInfoRemove( DWORD characterIdx, DWORD itemDBIdx );
void PetInfoUpdate( DWORD characterIdx, DWORD itemDBIdx, BYTE level, BYTE grade, BYTE type, BYTE slot, WORD exp, BYTE friendy, DWORD maxhp, DWORD hp, DWORD maxmp, DWORD mp, BYTE ai, BYTE state, DWORD time );
void PetMasterUpdate( DWORD characterIdx, DWORD userIdx, DWORD itemDBIdx );

void RPetInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void RPetInfoAdd( LPQUERY pData, LPDBMESSAGE pMessage );

void ItemMoveUpdatePetInven( DWORD characterIdx, DWORD userIdx, DWORD petIdx, DWORD fromItemIdx, DWORD fromItemPos, DWORD toItemIdx, DWORD toItemPos, BYTE param );

void PetItemInfoLoad( DWORD characterIdx, DWORD petIdx );
void RPetItemInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage );

// 080716 LUJ, 펫 로그
void LogPet( const PET_OBJECT_INFO&, ePetLog );

void Option_Load(DWORD dwPlayerIdx);
void ROption_Load(LPMIDDLEQUERY query, LPDBMESSAGE dbMessage);
void Option_Update(CPlayer* pPlayer) ;
// 081218 LUJ, 맵 이동 전에 필수 정보를 업데이트시킨다
void UpdatePlayerOnMapOut( DWORD playerIndex, DWORD userIndex, DWORD inventoryMoney, DWORD storageMoney, DWORD mapIndex, WORD channelIndex );
// 081218 LUJ, 필수 정보가 처리되었으면 맵 이동시킨다
void RUpdatePlayerOnMapOut( LPMIDDLEQUERY query, LPDBMESSAGE dbMessage );
