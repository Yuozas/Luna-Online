#ifndef __COMMONGAMEDEFINE_H__
#define __COMMONGAMEDEFINE_H__


enum RaceType
{
	RaceType_Human,
	RaceType_Elf,

	RaceType_Max,

	RaceType_GM,
};

//for GMPower
enum eGM_POWER
{
	eGM_POWER_MASTER = 0,
	eGM_POWER_MONITOR,
	eGM_POWER_PATROLLER,
	eGM_POWER_AUDITOR,
	eGM_POWER_EVENTER,
	eGM_POWER_QA,
	eGM_POWER_MAX,
};

//for Network

enum eTRAFFIC_KIND
{
	eTraffic_User_To_Agent			= 1,
	eTraffic_Agent_To_User			= 2,
	
	eTraffic_Agent_To_Map			= 3,
	eTraffic_Map_To_Agent			= 4,
	eTraffic_Agent_to_Agent			= 5,
	eTraffic_Map_to_Map				= 6,
	eTraffic_User_To_Distribute		= 7,
	eTraffic_Distribute_To_User		= 8,
};


//for Exit
#define EXIT_COUNT	5
enum eEXITCODE
{
	eEXITCODE_OK,
	eEXITCODE_PKMODE,
	eEXITCODE_LOOTING,
	eEXITCODE_SPEEDHACK,
	eEXITCODE_DIEONEXIT,
	eEXITCODE_NOT_ALLOW_STATE,
};
//

#define SPEEDHACK_CHECKTIME	60000	//1¡§¡þ¡§¡E

//for PK
#define PKLOOTING_ITEM_NUM			20
#define PKLOOTING_DLG_DELAY_TIME	2000
#define PKLOOTING_LIMIT_TIME		10000

enum ePKCODE
{
	ePKCODE_OK,
	ePKCODE_ALREADYPKMODEON,
	ePKCODE_SHOWDOWN,
	ePKCODE_STATECONFLICT,
	ePKCODE_NOTALLAW,
};

enum eLOOTINGERROR
{
	eLOOTINGERROR_OK,
	eLOOTINGERROR_NOMORE_CHANCE,
	eLOOTINGERROR_NOMORE_ITEMLOOTCOUNT,
	eLOOTINGERROR_INVALID_POSITION,
	eLOOTINGERROR_ALREADY_SELECTED,
	eLOOTINGERROR_OVER_DISTANCE,
	eLOOTINGERROR_NO_LOOTINGROOM,
};

//

#define NPC_TALKING_DISTANCE		700
#define PK_LOOTING_DISTANCE			1000

enum eDROPITEMKIND
{
	// 070423 LYW --- GameResourceStruct : Delete not use memeber.
	//eNOITEM,
	eMONEY,
	eITEM1,
	eITEM2,
	eITEM3,
	eITEM4,
	eITEM5,
	eITEM6,
	eITEM7,
	eITEM8,
	eITEM9,
	eITEM10,
	eITEM11,
	eITEM12,
	eITEM13,
	eITEM14,
	eITEM15,

	eDROPITEMKIND_MAX = 16,
};


enum
{
	eUSERLEVEL_GOD = 1,
	eUSERLEVEL_PROGRAMMER,
	eUSERLEVEL_DEVELOPER,
	eUSERLEVEL_GM,
	eUSERLEVEL_SUPERUSER,
	eUSERLEVEL_USER,
	eUSERLEVEL_SUBUSER,	
};

#define MAX_ITEMBUY_NUM		100
#define MAX_INVENTORY_MONEY	4000000000


enum eSTATUSICON
{
	eStatusIcon_FireContinueDamage = 1,
	eStatusIcon_WaterContinueDamage,

	eStatusIcon_SpeedDown,
	
	eStatusIcon_TieUpCanMove,
	eStatusIcon_TieUpCanAttack,
	eStatusIcon_TieUpAll,

	eStatusIcon_HealContinue,

	eStatusIcon_PhyAttackUp,
	eStatusIcon_PhyDefenceUp,

	eStatusIcon_MaxLifeUp,	//10
	eStatusIcon_MaxManaUp,

	eStatusIcon_PhyCounter,

	eStatusIcon_AttrDefenceFire,
	eStatusIcon_AttrDefenceWater,
	eStatusIcon_AttrDefenceTree,
	eStatusIcon_AttrDefenceIron,
	eStatusIcon_AttrDefenceEarth,
				
	eStatusIcon_AttrCounterFire,
	eStatusIcon_AttrCounterWater,	//20			
	eStatusIcon_AttrCounterTree,
	eStatusIcon_AttrCounterIron,
	eStatusIcon_AttrCounterEarth,
	eStatusIcon_AttrCounterAll,
	eStatusIcon_Immune,

	eStatusIcon_AmpliedPower,
	eStatusIcon_PoisonContinueDamage,
	eStatusIcon_IronContinueDamage,

	eStatusIcon_PhyAttackDown,
	eStatusIcon_PhyDefenseDown,	//30
	eStatusIcon_MaxLifeDown,
	eStatusIcon_MaxManaDown,
	
	eStatusIcon_DamageDown	= 34,
	eStatusIcon_AttackUp	= 35,
	eStatusIcon_ContinueRecover	= 36,
	eStatusIcon_ReverseVampiricMana	= 37,
	eStatusIcon_AttackPhyLastUp			= 39,
	eStatusIcon_AttackAttLastUp			= 40,
	eStatusIcon_ReverseVampiricLife		= 45,

	eStatusIcon_MussangMode				= 48,
	eStatusIcon_PartyHelp				= 49,

	// ShopItem용 Icon추가
	eStatusIcon_ShopItem_55101 = 50,
	eStatusIcon_ShopItem_55102 = 51,
	eStatusIcon_ShopItem_55103 = 52,
	eStatusIcon_ShopItem_55104 = 53,
	eStatusIcon_ShopItem_55105 = 54,
	eStatusIcon_ShopItem_55106 = 55,
	eStatusIcon_ShopItem_55107 = 56,
	eStatusIcon_ShopItem_55108 = 57,

	eStatusIcon_ShopItem_55311 = 58,
	eStatusIcon_ShopItem_55312 = 59,
	
	eStatusIcon_ShopItem_55322 = 60,	

	////////////////////////////////////////////////////////
	//06. 06 2차 전직 - 이영준
	//이펙트 생략(무초)
	eStatusIcon_SkipEffect	= 133,
	////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////
	//06. 06 2차 전직 - 이영준
	//은신/혜안
	eStatusIcon_Hide = 134,
	eStatusIcon_Detect = 135,
	////////////////////////////////////////////////////////
	eStatusIcon_QuestTime				= 127,		// quest time

	 
	eStatusIcon_GuildPlustime_SuRyun = 140,
	eStatusIcon_GuildPlustime_Skill = 141,
	eStatusIcon_GuildPlustime_Exp = 142,
	eStatusIcon_GuildPlustime_DamageUp = 143,

	eStatusIcon_FamilyPlustime_SuRyun = 150,
	eStatusIcon_FamilyPlustime_Skill = 151,
	eStatusIcon_FamilyPlustime_Exp = 152,
	eStatusIcon_FamilyPlustime_DamageUp = 153,


	eStatusIcon_Max = 160,
};


enum eBOSSMONSTERKIND
{
/*	eBOSSKIND_EMPERIORTOMB = 151,
	eBOSSKIND_EMPERIORTOMB_EVENT = 158,

	eBOSSKIND_MOOUN = 181,
	eBOSSKIND_COWF = 182,
	eBOSSKIND_COWL = 183,
	eBOSSKIND_COWKING = 184,

	eBOSSKIND_GENERAL=1001,
	eBOSSKIND_YUKJI=1002,*/
	eBOSSKIND_TARINTUS = 261,
	// 080128 KTH 
	eBOSSKIND_KIERRA = 267,
	eBOSSKIND_MAX,
};
enum eBOSSMONSTERTYPE
{
/*	eBOSSTYPE_EMPERIORTOMB,
	eBOSSTYPE_MOOUN,
	eBOSSTYPE_COWF,
	eBOSSTYPE_COWL,
	eBOSSTYPE_COWKING,
	eBOSSTYPE_GENERAL,
	eBOSSTYPE_YUKJI,*/
	eBOSSTYPE_TARINTUS,
	eBOSSTYPE_KIERRA,
	eBOSSTYPE_MAX,
};
#define RECALL_GROUP_ID			-1
#define EVENTRECALL_GROUP_ID	-2
#define QUESTRECALL_GROUP_ID	-3
// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.11.29
#define CHALLENGERECALL_GROUP_ID	-4
// E 데이트 존 추가 added by hseos 2007.11.29

#define ITEMRECALL_GROUP_ID	1001	// 몬스터소환 아이템 사용

enum eOPTION	//¡§u￠®¨I¨Ioo¡E?I ¡Ii¡E?AICI￠®¨¡O ¡E¡þAA¨I¡þ¡§oA¡E?A.
{
	eOPT_NULL		= 0,
	eOPT_NODEAL		= 1,
	eOPT_NOPARTY	= 2,
	eOPT_NOFRIEND	= 4,
	eOPT_NOSHOWDOWN		= 8,
	eOPT_NOWHISPER	= 16,
};


#define eOPT_HIDEPARTS_NONE				0x00000000
#define eOPT_HIDEPARTS_EQUIP_BAND		0x00000001
#define eOPT_HIDEPARTS_EQUIP_GLASSES	0x00000002
#define eOPT_HIDEPARTS_EQUIP_WING		0x00000004
#define eOPT_HIDEPARTS_COSTUME_HEAD		0x00000008
#define eOPT_HIDEPARTS_COSTUME_DRESS	0x00000010
#define eOPT_HIDEPARTS_COSTUME_GLOVE	0x00000020
#define eOPT_HIDEPARTS_COSTUME_SHOES	0x00000040
#define eOPT_HIDEPARTS_ALL				( eOPT_HIDEPARTS_EQUIP_BAND			|\
											eOPT_HIDEPARTS_EQUIP_GLASSES	|\
											eOPT_HIDEPARTS_EQUIP_WING		|\
											eOPT_HIDEPARTS_COSTUME_HEAD		|\
											eOPT_HIDEPARTS_COSTUME_DRESS	|\
											eOPT_HIDEPARTS_COSTUME_GLOVE	|\
											eOPT_HIDEPARTS_COSTUME_SHOES )



//---murim net
#define MAXPLAYER_IN_MURIMNET	50000
#define MAXPLAYER_IN_FRONT		10000
#define MAXPLAYER_IN_CHANNEL	300
#define MAXPLAYER_IN_PLAYROOM	300
#define MAXPLAYER_PERTEAM		100

#define MAXCHANNEL_IN_MURIMNET	10000
#define MAXPLAYROOM_IN_MURIMNET	10000
#define MAXSERVER_IN_MURIMNET	100

enum eBATTLE_TEAM	//battle team enum ￠®¨¡u AeCOCIAU CONFIRM
{
	eTEAM_LEFT,
	eTEAM_RIGHT,
	eTEAM_OBSERVER,
	eTEAM_MAX,
};

enum eCHANNEL_MODE
{
	eCNL_MODE_ID,
	eCNL_MODE_CHANNEL,
	eCNL_MODE_PLAYROOM,
	eCNL_MODE_MAX,
};

enum ePR_CHATCLASS
{
	PRCTC_WHOLE,
	PRCTC_TEAM,
	PRCTC_WHISPER,
	PRCTC_SYSTEM,
};
//-------------


enum eBATTLE_KIND
{
	eBATTLE_KIND_NONE = 0,		// Player vs Monster ¡§￠®o¡§uOAC ￠®ic¨IⓒªECO¡EO￠®¡¿AC Battle
	eBATTLE_KIND_MURIMFIELD,	// murim ￠®¨¡u munpa¡E¡IA ¡E¡IU¡E¡þ| ￠®¨¡A¡EOo ¡E?¨Io￠®io
	eBATTLE_KIND_SHOWDOWN,
	eBATTLE_KIND_MUNPAFIELD,
	eBATTLE_KIND_GTOURNAMENT,
	eBATTLE_KIND_SIEGEWAR,
	
	eBATTLE_KIND_MAX,
};

enum GRID_TYPE
{
	eGridType_General, //eBATTLE_KIND_NONE
	eGridType_Murim, //eBATTLE_KIND_MURIMFIELD
};

enum eMURIMBATTLE_KIND
{
	eMurimBattle_MaxKill = 0,

	eMurimBattle_Max,
};

enum Friend_ERROR_CODE
{
	//add friend
	eFriend_AddSuccess=0,
	eFriend_AddInvalidUser,
	eFriend_AddFullList,
	eFriend_AddDeny,
	eFriend_NotLogin,
	eFriend_AlreadyRegist,
//	eFriend_RegistSelf,
	//delete friend
	eFriend_DelInvaliedUser,
//----
	eFriend_OptionNoFriend,
};

enum {
LOGIN_SUCCESS = 0,
LOGIN_ERROR_INVALID_VERSION,
LOGIN_ERROR_OVERLAPPEDLOGIN,
LOGIN_ERROR_NOAGENTSERVER,
LOGIN_ERROR_NODISTRIBUTESERVER,
LOGIN_ERROR_INVALIDUSERLEVEL,
LOGIN_ERROR_WRONGIDPW,
LOGIN_ERROR_INVALIDDISTAUTHKEY,
LOGIN_ERROR_DISTSERVERISBUSY,
LOGIN_ERROR_AGENTSERVERISBUSY,
LOGIN_ERROR_AGENTMAXCONNECTION,
LOGIN_ERROR_BLOCKUSERLEVEL,
LOGIN_ERROR_INVALID_IP,	//ipcheck
LOGIN_ERROR_DISTCONNET_ERROR,
LOGIN_ERROR_MAXUSER,
LOGIN_ERROR_OVERLAPPEDLOGININOTHERSERVER,
LOGIN_ERROR_MINOR_INADULTSERVER,
LOGIN_ERROR_NOT_CLOSEBETAWINNER,
LOGIN_ERROR_SECEDEDACCOUNT,
LOGIN_ERROR_NOREMAINTIME,
LOGIN_ERROR_NOIPREGEN,
LOGIN_ERROR_NONACCADMINIP,
// 080111 LYW --- CommonGameDefine : 로그인 보안 비밀번호 관련 에러 처리 추가.
LOGIN_ERROR_INVALID_SEC_PASS,				// 보안 비밀번호 인증에 실패했다는 에러메시지.
LOGIN_ERROR_CHANGE_SEC_PASS,				// 잘못된 보안 비빌번호, 홈페이지에서 보안 비밀번호를 재 등록 하시오.
LOGIN_ERROR_FILE_CRC,
LOGIN_ERROR_LOGINSESSION_INVALID,
};

#define MHSHADOW_OFF	0
#define MHSHADOW_CIRCLE	1
#define MHSHADOW_DETAIL	2

// ¨Io¡Ec¡§￠®ACE¡IiaAu
#define MAX_MUNPAFIELD_MEMBERNUM		30

// Murim Battle
#define	MAX_MURIMFIELD_MEMBERNUM		30

// E¡E¡þ¡§¡þ¨Io ¡§oA￠®¨¡￠®I
#define RECOVERTIMEUNIT					3000
// E¡E¡þ¡§¡þ¨Io ￠®¨¡￠®I￠®¨¡Y
#define RECOVERINTERVALUNIT				10

// A¡E¡þAI ￠®icAIAi(CN A¡E¡þAIAC A¡§I￠®¨ua)
#define fTILE_SIZE 50.f

#define SKILLAREA_ATTR_DAMAGE			0x00000001
#define SKILLAREA_ATTR_BLOCK			0x00000002
#define SKILLAREA_ATTR_SAFETYZONE		0x00000003
#define SKILLAREA_ATTR_OTHEREFFECT		0x00000004

typedef BYTE AREATILE;
#define GetAreaTile	GetByte

#define MAX_ITEM_NUM 3000	//confirm
#define MAX_ITEM_OPTION_NUM	90

// INDEXGENERATER ID START INDEX
#define MAX_ITEMICON_NUM	999

// 080203 LYW --- 행동 관련 아이콘 최대 개수 정의.
#define MAX_CONDUCTICON_NUM	999

#define IG_ITEM_STARTINDEX				10000
#define IG_MSGBOX_STRARTINDEX			12000
#define IG_STREETSTALLITEM_STARTINDEX	13000	//LBS 03.09.19
#define IG_EXCHANGEITEM_STARTINDEX		14000	//KES EXCHANGE 030920
#define IG_MENUSLOTITEM_STARTINDEX		15000	//KES 040510	//Ao￠®¨uY ¡§uE¡§u¡E¡I¡E¡IU. Ao¡E?i￠®¨ui.
#define IG_PKLOOTINGDLG_START			16000	//KES PKLOOTING 040720
#define IG_DEBUG_START					20000	//KES DEBUG 040720
#define IG_QUESTITEM_STARTINDEX			25000	//RaMa
#define IG_SHOPITEM_STARTINDEX			28000	//RaMa 04.11.17
#define IG_DEALITEM_START				30000
#define IG_MAINBARICON_START			40000
//#define IG_ABILITYICON_START			31000
//#define IG_ABILITYPOINT_START			40000	//RaMa 특기치 재분배
#define IG_QUICKITEM_STARTINDEX			60000
// 070216 LYW --- CommonGameDefine : Define start index of conduct part.
#define IG_CONDUCT_STARTINDEX			70000

// 070608 LYW --- CommonGameDefine : Define start index of matching favor icon part.
#define IG_FAVOR_STARTINDEX				150000
#define IG_IDENTIFICATION_STARTINDEX	151000
#define IG_MATCH_MYINFO_STARTINDEX		152000
#define IG_MATCH_PARTNERINFO_STARTINDEX	153000
#define IG_MATCH_ENDINDEX				154000


// 071121 웅주, (강PM님 요청)오브젝트 인덱스는 그 값이 고유해야 한다. 인덱스 중에 유일하게 플레이어 인덱스는
//		DB에서 생성하므로 int 범위에서 계속 증가된다. 따라서 플레이어 인덱스와 겹쳐지지 않게 스킬/NPC/몬스터 
//		인덱스를 충분히 뒤에 배치한다.
//#define NEWOBJECT_STARTINDEX			2000001
//#define NEWRECALLOBJECT_STARTINDEX		3000001

#define IG_SKILL_STARTINDEX				(UINT_MAX - 500000)
#define NEWOBJECT_STARTINDEX			(UINT_MAX - 1000000)
#define NEWRECALLOBJECT_STARTINDEX		(UINT_MAX - 1500000)

//////////////////////////////////////////////////////////////////////////
// A¨I¨￡¨Ioy ¡E?¡E￠c￠®￠´? AU¡Iia
//----------------- ￠®¨¡¨I￠®¡E¡þA ----------------------------------
//pjs
// ￠®¨¡E￠®io A¡E¡þAO ¡§u￠®¨u¡§u¨I¡I..
enum SEARCH_TYPE
{
	MP_BY_SEARCHPRICE,
	MP_I_REGISTERED,
	MP_I_JOINED,
};
// d￠®￠´A A¡E¡þAO ¡§u￠®¨u¡§u¨I¡I..
enum SORT_TYPE
{
	MP_BY_WORD,
	MP_BY_PRICE,
	MP_BY_IMMEDIATEPRICE,
	MP_BY_DUEDATE,
	MP_BY_JOINAMOUNT,
};

enum CANCEL_TYPE
{
	MP_REG,
	MP_JOIN,
};
//----------------- ￠®¨¡¨I￠®¡E¡þA ----------------------------------


//------------------------------------------------//
/*
#define INVENTORY_CELL_NUM			80
#define SKILL_CELL_NUM				20
#define JINBUB_CELL_NUM				5
#define SIMBUB_CELL_NUM				5
#define WEAR_CELL_NUM				10
#define MAX_MUNPA_WAREITEM_PERTAB_NUM 45
#define MAX_MUNPA_WAREHOUSETAB_NUM 7
#define MUNPAITEM_CELL_NUM		(MAX_MUNPA_WAREHOUSETAB_NUM*MAX_MUNPA_WAREITEM_PERTAB_NUM)	// 7*45 = 315
#define	MAX_STORAGE_WAREITEM_PERTAB_NUM	30
#define MAX_STORAGE_NUM				5
#define MAX_STORAGEITEM_NUM			(MAX_STORAGE_WAREITEM_PERTAB_NUM*MAX_STORAGE_NUM)//45*5=225 -> 30*5 = 150
*/
// CAUTION !!
// ITEM & SKILL SHARING POSITION VALUE 
// ITEM ABSOLUTE POSITION	
// define￠®¨¡¡§¡I ¨IoU¨I¡Aa¡§u￠®¨I ¨Io¡Ec¡§￠®A ¡§u¡§￠®AIAU ¡§￠®￠®AAo¡§uCAI ¡§¡þ?CI¡E¡þe Cy¡§u￠®¨uAI¡E?¡E￠c￠®¨¡O CEE￠®A ¡E?￠®ⓒ­¡EOo! <- db ¡§uod
//CoAc STORAGEITEM_STARTPOSITION : 586, STORAGEITEM_ENDPOSITION : 736

/*
#define INVENTORY_STARTPOSITION		((POSTYPE)100)												// 80￠®¨¡¨Iⓒª 
#define INVENTORY_ENDPOSITION		((POSTYPE)INVENTORY_STARTPOSITION+INVENTORY_CELL_NUM)						//80
#define WEAR_STARTPOSITION			((POSTYPE)INVENTORY_ENDPOSITION)							//10￠®¨¡¨Iⓒª
#define WEAR_ENDPOSITION			((POSTYPE)WEAR_STARTPOSITION+WEAR_CELL_NUM)					//10			
#define STALL_STARTPOSITION			((POSTYPE)WEAR_ENDPOSITION)						
#define STALL_ENDPOSITION			((POSTYPE)WEAR_ENDPOSITION+20)								//20
#define QUICK_STARTPOSITION			((POSTYPE)STALL_ENDPOSITION)						
#define QUICK_ENDPOSITION			((POSTYPE)STALL_ENDPOSITION+MAX_QUICKITEMPERSHEET_NUM*MAX_QUICKSHEET_NUM)
//------------------------------------------------//
// SKILL ABSOLUTE POSITION
#define SKILL_WEAREDPOSITION		((POSTYPE)QUICK_ENDPOSITION)							
#define SKILL_STARTPOSITION		((POSTYPE)SKILL_WEAREDPOSITION+1)						
#define SKILL_ENDPOSITION			((POSTYPE)SKILL_STARTPOSITION+SKILL_CELL_NUM)			
#define JINBUB_WEAREDPOSITION		((POSTYPE)SKILL_ENDPOSITION)							
#define JINBUB_STARTPOSITION		((POSTYPE)JINBUB_WEAREDPOSITION+1)							
#define JINBUB_ENDPOSITION			((POSTYPE)JINBUB_STARTPOSITION+JINBUB_CELL_NUM)	
#define SIMBUB_WEAREDPOSITION		((POSTYPE)JINBUB_ENDPOSITION)							
#define SIMBUB_STARTPOSITION		((POSTYPE)SIMBUB_WEAREDPOSITION+1)						
#define SIMBUB_ENDPOSITION			((POSTYPE)SIMBUB_STARTPOSITION+SIMBUB_CELL_NUM)	

#define MAX_TOTALSKILL_NUM			((POSTYPE)SIMBUB_ENDPOSITION-SKILL_WEAREDPOSITION)

//------------------------------------------------//
// MUNPA ABSOLUTE POSITION
#define MUNPAITEM_STARTPOSITION		((POSTYPE)SIMBUB_ENDPOSITION)	
#define MUNPAITEM_ENDPOSITION		((POSTYPE)MUNPAITEM_STARTPOSITION+MUNPAITEM_CELL_NUM)
//------------------------------------------------//
// STORAGE ABSOLUTE POSITION
#define STORAGEITEM_STARTPOSITION	((POSTYPE)MUNPAITEM_ENDPOSITION)
#define STORAGEITEM_ENDPOSITION		((POSTYPE)MUNPAITEM_ENDPOSITION+MAX_STORAGEITEM_NUM)
*/

// Npc_Role￠®¨u¡E¡þ¡§¡þ¡§¡E
enum NPC_ROLE
{
	OBJECT_ROLE = 0,
	DEALER_ROLE = 1,
	AUCTION_ROLE = 2,
	MUNPA_ROLE = 3,
	CHANGGO_ROLE = 4,
	FYOKUK_ROLE = 5,
	TALKER_ROLE = 6,
	WANTED_ROLE = 9,
	SURYUN_ROLE = 10,
	SYMBOL_ROLE = 11,
	CASTLE_ROLE = 12,
	FAMILY_ROLE = 15,
	BOMUL_ROLE	= 23,
	MAPCHANGE_ROLE = 27,	
	FISHINGPLACE_ROLE = 28,
	// 070605 LYW --- CommonGameDefine : Add identification role.
	IDENTIFICATION_ROLE = 38,
};


enum eMoveMode
{
	eMoveMode_Run = 0,
	eMoveMode_Walk = 1,
};


/*********/


////¨Iⓒª￠®￠?¡§￠®¡Ec¡E?￠®A ¡§iⓒo¡Ei¨u¡§￠®¡Ec￠®￠´￠®¨¡AA¡E?¡E￠c¡§u￠®¨I ￠®ic¡E?eCI¡E¡IA define  ///////////////////////////////////////////////
#define MAX_PACKEDDATA_SIZE 65535 // WORD type for SEND_PACKED_DATA


// AI¡§¡þI¡§¡þ¡§¡E¡E?¡E￠c ￠®ic¡E?e + Client Move¡E?¡E￠c¡§u￠®¨I ￠®ic¡E?e
enum eMONSTER_ACTION
{
	eMA_STAND,
	eMA_WALKAROUND,
	eMA_PERSUIT,
	eMA_RUNAWAY,
	eMA_ATTACK,
	eMA_REST,
};

enum 
{
	eRunawayType_None,
	eRunawayType_Help = 1,
	eRunawayType_General,
	eRunawayType_Terminate,
};


/// GameResource¡E?¡E￠c ￠®ic¡E?e¡IiC¡E¡IA define ///////////////////////////////////////////////
enum eQUICKICON_KIND { eSKILL_ICON=1, eITEM_ICON, eABILITY_ICON,};
enum eSKILLITEM_KIND{eSKILLBOOK, eJINBUBBOOK, eSIMBUBBOOK, eERROR_BOOK,};
enum eITEM_KINDBIT 
{ 
	// RaMa - 04.11.02
	eSHOP_ITEM				= 256,
	eSHOP_ITEM_PREMIUM		= 257,	// 프리미엄
	eSHOP_ITEM_CHARM		= 258,	// 부적
	eSHOP_ITEM_HERB			= 259,	// 물약
	eSHOP_ITEM_INCANTATION	= 260,	// 주문서
	eSHOP_ITEM_MAKEUP		= 261,	// 변신
	eSHOP_ITEM_DECORATION	= 262,	// 꾸미기
	eSHOP_ITEM_SUNDRIES		= 263,	// 잡화
	eSHOP_ITEM_EQUIP		= 264,	// 장비아이템

	eYOUNGYAK_ITEM			= 512,

	eSKILL_ITEM			= 1024,
	eSKILL_ITEM_SIMBUB		= 1037,
	eSKILL_ITEM_JINBUB		= 1038,
	eSKILL_ITEM_KYUNGGONG	= 1039,
	eSKILL_ITEM_OPTION		= 1040,

	/// 장착 아이템류
	eEQUIP_ITEM				= 2048,
	
	/// 무기류
	eEQUIP_ITEM_TWOHANDED,
	eEQUIP_ITEM_ONEHANDED,
	eEQUIP_ITEM_DAGGER,
	eEQUIP_ITEM_BOW,
	eEQUIP_ITEM_STAFF,
	eEQUIP_ITEM_MACE,
	eEQUIP_ITEM_SHIELD,

	/// 방어구류
	/// 3D 모델을 갖는 것들
	eEQUIP_ITEM_HAT,			
	eEQUIP_ITEM_SHOES,		
	eEQUIP_ITEM_DRESS,		
	eEQUIP_ITEM_GLOVE,

	/// 아이콘만 표시되는 것들
	eEQUIP_ITEM_EARRING,
	eEQUIP_ITEM_RING,	
	eEQUIP_ITEM_BELT,
	eEQUIP_ITEM_NECKLACE,	

			
	eEXTRA_ITEM				= 4096,
	eEXTRA_ITEM_JEWEL		= 4097,
	eEXTRA_ITEM_MATERIAL	= 4098,
	eEXTRA_ITEM_METAL		= 4099,
	eEXTRA_ITEM_BOOK		= 4100,
	eEXTRA_ITEM_HERB		= 4101,
	eEXTRA_ITEM_ETC			= 4102,
	eEXTRA_ITEM_COMP		= 4104,
	eEXTRA_ITEM_QUEST		= 4105,
	eEXTRA_ITEM_USABLE		= 4106,
	
	eEXTRA_ITEM_UPGRADE			= 8192,
	eEXTRA_ITEM_UPGRADE_ATTACK	= 8193,
	eEXTRA_ITEM_UPGRADE_DEFENSE	= 8194,

	eQUEST_ITEM			= 16384,
	eQUEST_ITEM_START	= 16385,	//	 
	eQUEST_ITEM_EQUIP	= 16386,	//
	
	eCHANGE_ITEM		= 32768,	// eable to change
	eCHANGE_ITEM_LOCK	= 32769,	// not exchange & not streetstall 

	eKIND_ITEM_MAX,
};

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.05.14
enum ITEM_KIND
{
	ITEM_KIND_HP_RECOVER		= 1,	// 생명력 회복제
	ITEM_KIND_MP_RECOVER		= 2,	// 기력 회복제
//---KES 
	ITEM_KIND_HP_RECOVER_INSTANT	= 3,	// 생명력 회복제
	ITEM_KIND_MP_RECOVER_INSTANT	= 4,	// 기력 회복제
	ITEM_KIND_HPMP_RECOVER			= 5,	// 생명기력 동시 회복제
	ITEM_KIND_HPMP_RECOVER_INSTANT	= 6,	// 생명기력 동시 회복제
//-----
	ITEM_KIND_FARM_PLANT		= 7,	// 농장의 농작물 심기
	ITEM_KIND_FARM_MANURE		= 8,	// 농장의 농작물 비료 주기
	ITEM_KIND_BUFF_ITEMS		= 11,	// 버프의 기능들을 사용한 유료 아이템들.
	ITEM_KIND_FARM_HARVEST		= 12,	// 농장의 농작물 수확
	ITEM_KIND_RESET_STATS		= 50,	// 스탯 초기화 아이템
	ITEM_KIND_RESET_SKILL		= 51,	// 스킬 초기화 아이템

	// 080611 LYW --- CommonGameDefine : 아이템 타입 추가.
	ITEM_KIND_GET_SKILLPOINT	= 52,	// 스킬 포인트 획득.

	ITEM_KIND_PROTECT_EXP		= 53,	// 경험치 보호 아이템
	ITEM_KIND_RECALL_MOVE		= 54,	// 기억이동 주문서
	ITEM_KIND_EXTEND_INVEN		= 55,	// 인벤토리 확장 아이템
	ITEM_KIND_EXTEND_STORAGE	= 56,	// 창고 확장 아이템

	// 080607 LYW --- CommonGameDefine : 아이템 타입 추가.
	ITEM_KIND_OPENSTORAGE_BYITEM = 58,	// 창고 소환 아이템.

	ITEM_KIND_CHANGE_NAME		= 66,	// 이름 변경 아이템
	ITEM_KIND_MAP_MOVE_SCROLL	= 67,	// 071201 맵이동 주문서
	ITEM_KIND_JOIN_EVENT		= 77,	// 080204 KTH -- 이벤트 응모 관련
	ITEM_KIND_WORLDSHOUT		= 80,	// 080312 NYJ -- 전체외치기(월드확성기) 아이템
	ITEM_KIND_FISHING_BAIT		= 81,	// 080410 NYJ -- 낚시 미끼아이템
	ITEM_KIND_FISHING_FISH		= 82,	// 080410 NYJ -- 낚시 물고기
	ITEM_KIND_RESET_REGIDENT_REGIST	= 88,// 080215 KTH -- 주민등록 초기화 아이템
	ITEM_KIND_RETURN_SCROLL		= 99,	// 귀환 주문서

	ITEM_KIND_CHALLENGEZONE_ENTER_FREQ	= 200,	// 챌린지 존 입장 회수 증가

	ITEM_KIND_PET							= 600,
	ITEM_KIND_PET_SKILL						= 601,
	ITEM_KIND_PET_COSTUME					= 602,
	ITEM_KIND_PET_EQUIP						= 603,

	ITEM_KIND_PET_EGG						= 700,
	ITEM_KIND_PET_FOOD						= 701,
	ITEM_KIND_PET_NORMAL_RESURRECTION		= 702,
	ITEM_KIND_PET_SPECIAL_RESURRECTION		= 703,
	ITEM_KIND_PET_HP_RECOVER				= 704,
	ITEM_KIND_PET_MP_RECOVER				= 705,
	ITEM_KIND_PET_EVOLUTION_PYHSIC_TYPE		= 706,
	ITEM_KIND_PET_EVOLUTION_MAGIC_TYPE		= 707,
	ITEM_KIND_PET_EVOLUTION_SUPPORT_TYPE	= 708,


	// --- 임시...
	ITEM_KIND_ANIMAL			= 500,	// 080415 KTH -- 가축
	ITEM_KIND_ANIMAL_FOOD		= 501,	// 080507 KTH -- 먹이
	ITEM_KIND_ANIMAL_CLEANER	= 502,	// 080514 KTH -- 축사 청소 세재
	
	ITEM_KIND_SUMMON_NPC					= 800,
	ITEM_KIND_SUMMON_MONSTER				= 801,
	ITEM_KIND_SUMMON_EFFECT					= 802,

};
// E 농장시스템 추가 added by hseos 2007.05.14

/// 아이템 구분 재정의
enum eItemCategory
{
	eItemCategory_Etc,
	eItemCategory_Equip,
	eItemCategory_Expend,
	eItemCategory_Material,
	eItemCategory_Quest,
	eItemCategory_Max,
};

enum eEquipType
{
	eEquipType_None,
	eEquipType_Weapon,
	eEquipType_Armor,
	eEquipType_Accessary,
	eEquipType_Pet,
	eEquipType_Max,
};

enum eEquipSlot
{
	eEquipSlot_None,
	eEquipSlot_hand,
	eEquipSlot_Shield,
	eEquipSlot_Armor,
	eEquipSlot_Helmet,
	eEquipSlot_Glove,
	eEquipSlot_Boots,
	eEquipSlot_Ring1,
	eEquipSlot_Ring2,
	eEquipSlot_Necklace,
	eEquipSlot_Earring1,
	eEquipSlot_Earring2,
	eEquipSlot_Belt,
	eEquipSlot_Band,

	eEquipSlot_Glasses,
	eEquipSlot_Mask,
	eEquipSlot_Wing,

	eEquipSlot_Costume_Head,
	eEquipSlot_Costume_Dress,
	eEquipSlot_Costume_Glove,
	eEquipSlot_Costume_Shoes,

	eEquipSlot_Max,

	ePetEquipSlot_Start = 70,
};

enum ePetEquipSlot
{
	ePetEquipSlot_Helmet,
	ePetEquipSlot_Armor,
	ePetEquipSlot_Glove,
	ePetEquipSlot_Accessaries,
	ePetEquipSlot_Weapon,
	
	ePetEquipSlot_Face,
	ePetEquipSlot_Head,
	ePetEquipSlot_Back,
	ePetEquipSlot_Tail,

	ePetEquipSlot_Skill1,
	ePetEquipSlot_Skill2,
	ePetEquipSlot_Skill3,
	ePetEquipSlot_Skill4,
	ePetEquipSlot_Skill5,
	ePetEquipSlot_Skill6,


	ePetEquipSlot_Max,
};

enum eWeaponAnimationType
{
	eWeaponAnimationType_None,
	eWeaponAnimationType_OneHand,
	eWeaponAnimationType_TwoHand,
	eWeaponAnimationType_Dagger,
	eWeaponAnimationType_Staff,
	eWeaponAnimationType_Bow,
	eWeaponAnimationType_Gun,
	eWeaponAnimationType_TwoBlade,
	eWeaponAnimationType_Max,
};

enum eWeaponType
{
	eWeaponType_None,
	eWeaponType_Sword,
	eWeaponType_Mace,
	eWeaponType_Axe,
	eWeaponType_Dagger,
	eWeaponType_Staff,
	eWeaponType_Bow,
	eWeaponType_Gun,
	eWeaponType_FishingPole,
	eWeaponType_Max,
};

enum eArmorType
{
	eArmorType_None,
	eArmorType_Robe,
	eArmorType_Leather,
	eArmorType_Metal,
	eArmorType_Max,
};

enum eAccessaryType
{
	eAccessaryType_None,
	eAccessaryType_Ring,
	eAccessaryType_Earring,
	eAccessaryType_Necklace,
	eAccessaryType_Belt,
	eAccessaryType_Band,
	eAccessaryType_Max,
};
/// 아이템 구분 재정의

#define MAX_QUERY_RESULT 100
#define MAX_JOB		3
#define MAX_ATTACKSKILL_NUM	10
#define MAX_NAEGONG_NUM	10
#define MAX_KYUNGGONG_NUM	10
#define MAX_SKILLITEM_NUM	10
#define MAX_YOUNGYAKITEM_NUM	10
#define MAX_EQUIPITEM_NUM	300

#define MAX_PLAYER_NUM	135
// 필드보스 - 05.12 이영준
// 몬스터 수 확장 190 -> 300
#define MAX_MONSTER_NUM 300
#define MAX_MONSTER_REGEN_NUM 100
#define MAX_NPC_NUM 200
#define MAX_DROPITEM_NUM 20
#define MAX_NPC_REGEN_NUM 100
#define MONSTER_ATTACKTYPE1		0
#define MONSTER_ATTACKTYPE2		1
#define MONSTER_ATTACKTYPE3		2
#define MAX_MONSTERATTACK_TYPE  3

#define MAX_MONSTER_GROUPNUM	200
#define MAX_NPC_GROUPNUM		200

#define MAX_CHARACTER_LEVEL_NUM		150			// A¨Iⓒª¡E¡þ?AI ￠®￠´¨Iiⓒo¡Ei¨u￠®¡¿ ¡§uo

#define MAX_PLAYERLEVEL_NUM	151
#define MAX_MONSTERLEVELPOINTRESTRICT_NUM	9
#define MONSTERLEVELRESTRICT_LOWSTARTNUM	6

// ￠®ic¡E?e¡IiC¡E¡IA ￠®¨¡￠®A : ¨Io￠®i￠®¨¡¨I¨￡¡E¡þ¡Ec¡§uO¡§o¡§¡þAI¡Ii|¡§o¡§¡þ
enum WP_KIND{WP_GUM=1,WP_GWUN,WP_DO,WP_CHANG,WP_GUNG,WP_AMGI,WP_KEY,WP_EVENT,WP_MAX,WP_ERR};
enum WP_ATTR{WP_FIRE=1, WP_WATER, WP_TREE, WP_GOLD, WP_EARTH, WP_NONEATTR};
enum RE_TARGET_RANGE{TR_PERSONAL, TR_WHOLE,};
enum SKILL_TYPE{SKILLTYPE_NORMAL, SKILLTYPE_JINBUB, SKILLTYPE_SIMBUB, SKILLTYPE_MAX,};

// A¡§¡ⓒ¡§¡þ¡E¡þ ¨Io¨I¨￡E￠®I(1¨Io¨I¨￡A¡§¡ⓒ¡§¡þ¡E¡þAC ¨Io¨I¨￡E￠®IAI¡E¡IU,2¨Io¨I¨￡ A¡§¡ⓒ¡§¡þ¡E¡þ¡E¡IA +1 3¨Io¨I¨￡A¡§¡ⓒ¡§¡þ¡E¡þ¡E¡IA +2....)
#define COMBO_GUM_MIN 1
#define COMBO_GWUN_MIN 7
#define COMBO_DO_MIN 13
#define COMBO_CHANG_MIN 19
#define COMBO_GUNG_MIN 25
#define COMBO_AMGI_MIN 31
#define COMBO_EVENT_MIN 50

#define MAX_COMBO_NUM		3
#define SKILL_COMBO_NUM		100
#define MAX_COMBOATTACK_LIST (WP_MAX*MAX_COMBO_NUM)		 // 6(¨Io￠®i￠®¨ua~￠®￠´u) * 6(AO¡E¡IeA¡§¡ⓒ¡§¡þ¡E¡þ)

// ¡§o¡§¡þA¨Iⓒª ¨Io¨I¨￡E￠®I
#define MAX_SKILLATTACK_LIST (WP_MAX*50)				// ¨Io￠®i￠®¨ua~￠®￠´u * ¨Io￠®i￠®¨ua¡E¡þ¡EO¡E¡IUAC ¡§o¡§¡þA¨Iⓒª ¡§uo
#define MAX_LEVEL_EXP 7									// 7¡§u¡§¡þ￠®¨uiAoAC ￠®¨¡¨I￠®CeA¡E￠c
#define MAX_LEVEL 12									

// SkillResult
#define SKILLRESULT_ATTACK				0x00000001
#define SKILLRESULT_RECOVERLIFE			0x00000010
#define SKILLRESULT_RECOVERMANA		0x00000100

#define SKILLTARGETTYPE_ENEMY			0
#define SKILLTARGETTYPE_TEAMMATE		1
#define SKILLTARGETTYPE_SELF			2
#define SKILLTARGETTYPE_ENEMYANDTEAM	3
#define SKILLTARGETTYPE_NOTENEMYTARGET	4

#define SKILLRESULTKIND_NONE			0
#define SKILLRESULTKIND_POSITIVE		1
#define SKILLRESULTKIND_NEGATIVE		2

#define ATTACKTYPE_MELEE				0
#define ATTACKTYPE_RANGE				0

//////////////////////////////////////////////////////////////////////////
// AI¨IoY￠®¨¡¨I¨￡￠®¨¡Y or ¡E¡IUA¨I¡þ￠®¨¡¨I¨￡￠®¨¡Y
#define ATTACKTYPE_NORMAL		0
#define ATTACKTYPE_HALFCIRCLE	1
#define ATTACKTYPE_CIRCLE		2
#define ATTACKTYPE_LINE			3
#define ATTACKTYPE_RECTANGLE	4

// ¡E¡IUA¨I¡þ￠®¨¡¨I¨￡￠®¨¡YAC ¨Iou'AC ￠®¨uaA¡§¨￡a
#define	TARGETTYPE_TARGET	0
#define TARGETTYPE_ATTACKER 1
#define TARGETTYPE_GROUND	2
//////////////////////////////////////////////////////////////////////////

// ¡§u¡§￠®AIAU ￠®¨u¡E¡þ¡§¡þ￠®¨¡¡E?¡E￠c ¡E¡IeCN enum
/*
enum ICON_GROUP_KINDS
{ 
	eIGK_ATTACK_SKILL,
	eIGK_DEFENSE_SKILL, 
	eIGK_ASSIST_SKILL, 
	eIGK_SPECIAL_SKILL, 
	eIGK_YOUNGYAK_ITEM, 
	eIGK_SKILL_ITEM, 
	eIGK_EQUIP_ITEM,
	eIGK_Max,
};*/


enum eSKILL_KIND
{ 
	eATTACK_SKILL, 
	eDEFENSE_SKILL, 
	eASSIST_SKILL, 
	eSPECIAL_SKILL, 
	eNAEGONG,
	eKYUNGGONG,
	eMAX_SKILL, 
	eERROR_SKILL
};

enum eItemUse_Err
{
	eItemUseSuccess,
	// desc_hseos_체인지아이템_01
	// S 체인지아이템 추가 added by hseos 2007.08.29
	eItemUseSuccess_ChangeItem,
	// E 체인지아이템 추가 added by hseos 2007.08.29

	eItemUseErr_PreInsert,
	eItemUseErr_Move,
	eItemUseErr_Combine,
	eItemUseErr_Divide,
	eItemUseErr_Discard,
	eItemUseErr_Quest,
	eItemUseErr_Err,
	eItemUseErr_Upgrade,
	eItemUseErr_Lock,
	eItemUseErr_AlreadyUse,
	eItemUseErr_Unabletime,
	eItemUseErr_AlreadyPoint,
	eItemUseErr_AlredyChange,
	eItemuseErr_DontUseToday,
	eItemUseErr_DontDupUse,
	eItemUseErr_UseFull,
	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.08.29
	eItemUseErr_FullInven,
	eItemUseErr_WithNoneMsg,
	// E 농장시스템 추가 added by hseos 2007.08.29

	eItemUseUnsealed,	//080317 NYJ 봉인해제

	// 080607 LYW --- CommonGameDefine : 창고 소환 아이템 사용 실패 에러 추가.
	eItemUse_OpenStorage_byItem_Failed_Storage_IsZero,
	// 080609 LYW --- CommonGameDefine : 창고 소환 아이템 사용 성공 타입 추가.
	eItemUse_OpenStorage_Success,

	eItemUseErr_NoSummonInfo,

	eItemUseErr_ForbiddenSummon,	// 081027 NYJ 소환금지
};

/// GameResource¡E?¡E￠c ￠®ic¡E?e¡IiC¡E¡IA define ///////////////////////////////////////////////
#define MAX_YOUNGYAKITEM_DUPNUM			100
// 061211 LYW --- Adjust max character count.
//#define MAX_CHARACTER_NUM	4
#define MAX_CHARACTER_NUM	5

#define MAX_BOARDTITLE_NUM	10
#define MAX_BOARDTITLE_LENGTH 64
#define MAX_CONTENTS_LENGTH	65535

//#define MAX_STREETSTALL_TITLELEN 38
#define MAX_STREETSTALL_TITLELEN 66

#define MAX_FILE_NAME		32

//¡§u¡§￠®AIAU Define
//¨Iⓒª¨I¡AAU 
#define ManBlueDress		23010 //¡§￠®o￠®io¡§¡þ¨Io
#define ManBlueNothing		23011
#define ManDefaultShoose	27000 //¨Io￠®i¡E¡þiE￠®¨I 
//¡E?¡§IAU 
//#define WomanRedDress		12210 //¡§￠®o￠®io¡§¡þ¨Io
#define WomanRedDress		23010
#define WomanDefalutShoose	27000 //¨Io￠®i¡E¡þiE￠®¨I

//  [5/27/2003]

// A¨Iⓒª¡E¡þ?AIA￠®E¡E?¡E￠c ¡§￠®￠®AAIAI ¨Ioe¡§¡þ¡§¡E￠®¨¡u￠®￠´A
// 070111 LYW --- Modified this line.
//enum { STR_POINT, WIS_POINT, DEX_POINT, VIT_POINT, MAX_BTN_POINT};
enum { STR_POINT, WIS_POINT, DEX_POINT, VIT_POINT, INT_POINT, MAX_BTN_POINT};

#define GENDER_MALE 0
#define GENDER_FEMALE 1
#define GENDER_MAX 2

#define MAX_PARTY_LISTNUM 7
#define MAX_PARTY_LOAD_NUM 100

// 070105 LYW --- Define max count for family member.
#define MAX_FAMILY_LISTNUM 5

#define MAX_MUNPA_NAME 20
#define MAX_MUNPALOAD_LIST	100
#define MAX_MUNPA_LIST	10
#define MAX_MUNPA_INTROMSG	568
#define MAX_MUNPA_IMAGE_NUM 32

#define MUNPA_MASTER		20
#define MUNPA_NOTMEMBER		0	//¨Io¡Ec¡§￠®A¡E?¨I¨￡AI ¡§u¡§￠®¡E¡IN ￠®ic¡EO￠®A
#define MUNPA_SEMIMEMBER	1	//¨Io¡Ec¡§￠®A ￠®¨¡¡E￠cAO¡§oAA￠®iAI
#define MUNPA_MEMBER		2	//¨Io¡Ec¡§￠®A¡E?¨I¨￡
#define MUNPA_SENIOR		3	//Aa￠®￠´I
#define MUNPA_VICE_MASTER	10	//¡§¡þI¨Io¡EcAO

#define MUNPA_KIND_JUNG	1	//d¡§￠®A
#define MUNPA_KIND_SA	2	//￠®ic¡§￠®A
#define MUNPA_KIND_WHANG	3	//E¨I¡A￠®¨uA
#define MUNPA_KIND_MU	4	//¨Io￠®i¡§uO¡§uO

#define MAX_MUNPA_BOARD_NUM	5	//¨Io¡Ec¡§￠®A ￠®¨¡O¡§oA¡§￠®C
#define MAX_MUNPA_BOARD_NAME 16		//￠®¨¡O¡§oA¡§￠®C AI¡E¡þ￠®¡¿
#define MAX_MUNPA_REGULARMEMBERLIST_NUM 10  //¡E¡þ¡Ec¡§iⓒo¡Ei¨u¡§￠®¡Ec¡E?¡E￠c ¡§¡þ¡E¡þ¡E?¡§IAo¡E¡IA ¨Io¡Ec¡§￠®A¡E?¨I¨￡¡Iie ¡§uo
#define MAX_MUNPA_BATTLEKIND 3
#define MAX_MUNPA_SEMIMEMBERLIST_NUM 10 //¡E¡þ¡Ec¡§iⓒo¡Ei¨u¡§￠®¡Ec¡E?¡E￠c ¡§¡þ¡E¡þ¡E?¡§IAo¡E¡IA ¡§oAA￠®iAI¡Iie ¡§uo
#define MAX_MUNPA_BOARD_SUBJECT 64
#define MAX_MUNPA_BOARD_CONTENT 2048
#define MAX_MUNPA_BOARD_LIST 10

#define MAX_MUNPA_WARE_TABNUM 7
#define MAX_BASESTORAGELIST_NUM 2
#define MAX_STORAGELIST_NUM 5
//#define MAX_STORAGELOAD_ITEM 100
//#define MAX_OPTIONLOAD_ITEM 100
//#define MAX_SHOPLOAD_ITEM	100
//#define MAX_LOAD_ITEM		100
//#define MAX_PETINVENLOAD_ITEM 100
//#define MAX_PINVENLOAD_ITEM 100
#define MAX_FRIEND_NUM 30
#define MAX_FRIEND_LIST 10
#define MAX_FRIEND_PAGE MAX_FRIEND_NUM/MAX_FRIEND_LIST // 30/10 = 10

#define MAX_NOTENUM 40
#define MAX_NOTE_PAGE 4
#define NOTENUM_PER_PAGE MAX_NOTENUM/MAX_NOTE_PAGE // 30/3 = 10
#define MAX_NOTE_LENGTH 255

#define MAX_CHANNEL_NUM 10
#define MAX_CHANNEL_NAME 16

#define MAX_TATIC_ABILITY_NUM 10

enum eWearedItem
{
	eWearedItem_Weapon,
	eWearedItem_Shield,
	eWearedItem_Dress,
	eWearedItem_Hat,
	eWearedItem_Glove,
	eWearedItem_Shoes,

	eWearedItem_Ring1,
	eWearedItem_Ring2,
	eWearedItem_Necklace,
	eWearedItem_Earring1,
	eWearedItem_Earring2,
	eWearedItem_Belt,
	eWearedItem_Band,

	eWearedItem_Glasses,
	eWearedItem_Mask,
	eWearedItem_Wing,

	eWearedItem_Costume_Head,
	eWearedItem_Costume_Dress,
	eWearedItem_Costume_Glove,
	eWearedItem_Costume_Shoes,

	eWearedItem_Max,
};

enum WantedBuyErrCode
{
	eBWR_Success,
	eBWR_Err_Err,
	eBWR_Err_FullVolunteer, 
};
#define MAX_WANTED_NUM 20
#define MAX_WANTED_VOLUNTEER 100
#define WANTUNIT_PER_PAGE 12

enum ERROR_GUILD
{
	eGuildErr_None	= 0,
	eGuildErr_Err	= 1,
			
	eGuildErr_Create_Invalid_Name,
	eGuildErr_Create_Already,
	eGuildErr_Create_Low_Level,
	eGuildErr_Create_Insufficient_Money,
	eGuildErr_Create_Unable_Entry_Data,
	
	eGuildErr_BreakUp,
	
	eGuildErr_DeleteMember,
	eGuildErr_DeleteMember_NothaveAuthority,
	eGuildErr_DeleteMember_NotMember,
	
	eGuildErr_AddMember_NotPlayer,
	eGuildErr_AddMember_OtherGuild,
	eGuildErr_AddMember_AlreadyMember,
	eGuildErr_AddMember_FullMember,	
	eGuildErr_AddMember_TargetNotDay,
	eGuildErr_AddMember_NothaveAuthority,
	
	eGuildErr_InviteApt_NoMaster,

	eGuildErr_Mark_NoGuild,
	eGuildErr_Mark_NoMark,

	eGuildErr_LevelUp_NotAbil,
	eGuildErr_LevelUp_NotMoney,
	eGuildErr_LevelUp_Complete,

	eGuildErr_ChangeRank_NoGuild,
	eGuildErr_ChangeRank_NotMember,
	eGuildErr_ChangeRank_LowerLevel,
	eGuildErr_ChangeRank_Master,
	eGuildErr_ChangeRank_AlreadyHave,
	eGuildErr_ChangeRank_NoAuthority,
	eGuildErr_ChangeRank_NoGraduatedStudent,
	eGuildErr_ChangeRank_Student,

	eGuildErr_NoGuild,

	eGuildErr_Nick_Filter,
	eGuildErr_Nick_NotMember,
	eGuildErr_Nick_NotAvailableName,
	eGuildErr_Nick_NotLogIn,

	eGuildErr_AddStudent_TooHighLevelAsStudent,
	eGuildErr_AddStudent_Full,

	// 080417 LUJ, 길드 스킬 추가 에러
	eGuildErr_InsufficentMoney	=	100,
	eGuildErr_InsufficentScore,
	eGuildErr_NoMaster,
	eGuildErr_NoScript,
	eGuildErr_Skill_Max,
	eGuildErr_Skill_InvalidLevel,
	eGuildErr_Skill_NotExist,
};
enum
{
	eGuild_Delete,
	eGuild_Secede,		
};

enum ERROR_FAMILY
{
	eFamilyErr_Err = 1,
			
	eFamilyErr_Create_Name,
	eFamilyErr_Create_Intro,
	
	eFamilyErr_BreakUp,
	
	eFamilyErr_DeleteMember,
	eFamilyErr_DeleteMember_NothaveAuthority,
	eFamilyErr_DeleteMember_NotMember,
	
	eFamilyErr_AddMember_NotPlayer,
	eFamilyErr_AddMember_OtherFamily,
	eFamilyErr_AddMember_AlreadyMember,
	eFamilyErr_AddMember_FullMember,	
	eFamilyErr_AddMember_TargetNotDay,
	eFamilyErr_AddMember_NothaveAuthority,

	eFamilyErr_InviteApt_NoMaster,

	eFamilyErr_Mark_NoFamily,
	eFamilyErr_Mark_NoMark,

	eFamilyErr_LevelUp_NotAbil,
	eFamilyErr_LevelUp_NotMoney,
	eFamilyErr_LevelUp_Complete,

	eFamilyErr_ChangeRank_NoFamily,
	eFamilyErr_ChangeRank_NotMember,

	eFamilyErr_NoFamily,

	eFamilyErr_Nick_Filter,
	eFamilyErr_Nick_NotMember,
	eFamilyErr_Nick_NotAvailableName,
	eFamilyErr_Nick_NotLogIn,

	eFamilyErr_AddStudent_TooHighLevelAsStudent,
};
enum
{
	eFamily_Delete,
	eFamily_Secede,		
};

//--------¡E?¡E￠c￠®￠´? ¡E¡þ¡§¡ⓒ¡§oAAo----------------------------------------------------------
enum DEAL_BUY_ERROR { NOT_ERROR, NOT_MONEY = 101, NOT_SPACE, NOT_EXIST, NOT_PLAYER, NOT_OWNERMONEY, NOT_SAMEPRICE, NOT_SAMEINFO, NOT_FISHINGPOINT, OVER_INVENMONEY};
enum ERROR_MUNPABOARDCONTENTS {
	ERROR_MUNPABOARDCONTENTS_NOCONTENTS = 1,
	ERROR_MUNPABOARDCONTENTS_NOAUTHORITY,
};

enum ERROR_PARTY 
{
	eErr_Create_DB,
	eErr_Add_NoPlayer,
	eErr_Add_AlreadyinParty,
	eErr_Add_OptionNoParty,
	eErr_Add_DiedPlayer,
	eErr_ChangingMaster,
	eErr_Add_Full, //can not join cause party is full!
	eErr_BrokenParty, // Party has already broken.
	eErr_Create_MapHasParty, //map has party but client doesn't have party
	eErr_Add_NotOurGuild,		// During Guild Tournament
	// 070110 LYW --- Add error message for party.
	eErr_Fail_Change_PickupItem,
	eErr_Success_Change_PickupItem,
};

enum PARTY_OPTION
{
	ePartyOpt_Random,
	ePartyOpt_Damage,
	ePartyOpt_Sequence,
};
//--------¡E?¡E￠c￠®￠´? ¡E¡þ¡§¡ⓒ¡§oAAo----------------------------------------------------------

enum TABNUMBER
{
	TAB_INVENTORY_NUM			= 2,
	TAB_MUNPAWAREHOUSE_NUM		= 7,
	TAB_BASE_STORAGE_NUM		= 2,
	TAB_EXTENDED_STORAGE1_NUM	= 1,
	TAB_EXTENDED_STORAGE2_NUM	= 1,
	TAB_EXTENDED_STORAGE3_NUM	= 1,
	TAB_STORAGE_NUM				= TAB_BASE_STORAGE_NUM + TAB_EXTENDED_STORAGE1_NUM + TAB_EXTENDED_STORAGE2_NUM + TAB_EXTENDED_STORAGE3_NUM,
	TAB_QUICK_NUM				= 4,
	TAB_ABILITY_NUM				= 4,	//SW051129 Pet 4->5
	TAB_MENUSLOT_NUM			= 4,	//KES 040510
	TAB_QITEMINVEN_NUM			= 4,
	TAB_GUILDWAREHOUSE_NUM		= 2,
	TAB_SHOPITEM_NUM			= 1,
	TAB_SHOPINVEN_NUM			= 2,
	TAB_FAMILYWAREHOUSE_NUM		= 2,

};

enum CELLPERTAB
{
	TABCELL_INVENTORY_NUM		= 20,
	TABCELL_MUNPAWAREHOUSE_NUM	= 45,
	TABCELL_STORAGE_NUM			= 36,
	TABCELL_QUICK_NUM			= 10,
	TABCELL_ABILITY_NUM			= 40,
	TABCELL_MENUSLOT_NUM		= 7,	//KES 040510
	TABCELL_QITEMINVEN_NUM		= 20,
	TABCELL_GUILDWAREHOUSE_NUM	= 36,
//	TABCELL_SHOPITEM_NUM		= 24,	// RaMa 04.11.03
	TABCELL_SHOPITEM_NUM		= 36,	// 071126 KTH
	TABCELL_SHOPINVEN_NUM		= 20,
	TABCELL_FAMILYWAREHOUSE_NUM	= 30,

};
#define INVENTORY_MAX_EXPANSION	2
enum
{
	SLOT_INVENTORY_NUM			= TAB_INVENTORY_NUM * TABCELL_INVENTORY_NUM,
	SLOT_EXTENDED_INVENTORY1_NUM = TABCELL_INVENTORY_NUM,
	SLOT_EXTENDED_INVENTORY2_NUM = TABCELL_INVENTORY_NUM,
	// 071206 KTH
	SLOT_MAX_INVENTORY_NUM		= SLOT_INVENTORY_NUM + SLOT_EXTENDED_INVENTORY1_NUM + SLOT_EXTENDED_INVENTORY2_NUM,	// 80

	SLOT_WEAR_NUM				= 20,

	SLOT_MUNPAWAREHOUSE_NUM		= TAB_MUNPAWAREHOUSE_NUM * TABCELL_MUNPAWAREHOUSE_NUM,	//315
	SLOT_STORAGE_NUM				= TAB_STORAGE_NUM * TABCELL_STORAGE_NUM,//150

	SLOT_QUICK_NUM				= TAB_QUICK_NUM * TABCELL_QUICK_NUM,//11

	SLOT_ABILITY_NUM			= TAB_ABILITY_NUM * TABCELL_ABILITY_NUM,

	SLOT_QITEMINVEN_NUM			= TAB_QITEMINVEN_NUM * TABCELL_QITEMINVEN_NUM,//100
	
	SLOT_GUILDWAREHOUSE_NUM		= TAB_GUILDWAREHOUSE_NUM * TABCELL_GUILDWAREHOUSE_NUM,	//60
	
	SLOT_FAMILYWAREHOUSE_NUM		= TAB_FAMILYWAREHOUSE_NUM * TABCELL_FAMILYWAREHOUSE_NUM,	//60

	SLOT_SHOPITEM_NUM			= TAB_SHOPITEM_NUM * TABCELL_SHOPITEM_NUM,
	
	SLOT_SHOPINVEN_NUM			= TAB_SHOPINVEN_NUM * TABCELL_SHOPINVEN_NUM,
	SLOT_WEAR_SHOPITEM_NUM		= 10,

	SLOT_STREETSTALL_NUM		= 24,	// 무공확장창으로 쓴다.
	SLOT_STREETBUYSTALL_NUM		= 5,	
	SLOT_SKILL_NUM				= 36,	
	SLOT_JINBUB_NUM				= 5,		// 5
	SLOT_SKILLTOTAL_NUM			= 36 * 4,	// 50	

};

// < ¡E?¨I¨￡A￠®E > : taiyo
// TP_INVENTORY_START¡E¡IA			¨IoY¡Iia¡§oA = 0
// TP_MUNPAWAREHOUSE_START¡E¡IA	¨IoY¡Iia¡§oA = TP_SIMBUB_END
// TP_MAX¡E¡IA						¨IoY¡Iia¡§oA = TP_STORAGE_END,
/*
enum
{
	//ITEMTABLE¡E?¡E￠c¡§u￠®¨I ￠®ic¡E?e
	TP_INVENTORY_START			= 0,
	TP_INVENTORY_END			= TP_INVENTORY_START		+ SLOT_INVENTORY_NUM,	// 80
	TP_WEAR_START				= TP_INVENTORY_END,									// 80
	TP_WEAR_END					= TP_WEAR_START				+ SLOT_WEAR_NUM,		// 90

	TP_STORAGE_START				= TP_WEAR_END,										// 90
	TP_STORAGE_END				= TP_STORAGE_START			+ SLOT_STORAGE_NUM,		// 240

	//ItemTableAI¡E?U¡E?¡E￠c¡§u￠®¨I ￠®ic¡E?e
	TP_MUNPAWAREHOUSE_START		= TP_STORAGE_END,										// 240
	TP_MUNPAWAREHOUSE_END		= TP_MUNPAWAREHOUSE_START	+ SLOT_MUNPAWAREHOUSE_NUM,	// 555

	TP_SKILL1_START			= TP_MUNPAWAREHOUSE_END,								// 555
	TP_SKILL1_END				= TP_SKILL1_START			+ SLOT_SKILL_NUM,			// 580
	
	TP_SKILL2_START			= TP_SKILL1_END,										// 580
	TP_SKILL2_END				= TP_SKILL2_START			+ SLOT_SKILL_NUM,			// 605

	TP_QUICK_START				= TP_SKILL2_END,										// 605
	TP_QUICK_END				= TP_QUICK_START			+ SLOT_QUICK_NUM,			// 645

	TP_ABILITY_START			= TP_QUICK_END,											// 645
	TP_ABILITY_END				= TP_ABILITY_START			+ SLOT_ABILITY_NUM,			// 805

	TP_SHOPITEM_START			= TP_STORAGE_END,										// 240
	TP_SHOPITEM_END				= TP_SHOPITEM_START			+ SLOT_SHOPITEM_NUM,		// 390

	TP_SHOPINVEN_START			= TP_SHOPITEM_END,										// 390
	TP_SHOPINVEN_END			= TP_SHOPINVEN_START		+ SLOT_SHOPINVEN_NUM,		// 430
	
//	TP_MAX						= TP_STORAGE_END + SLOT_SHOPITEM_NUM + SLOT_SHOPINVEN_NUM,	// 240+150+40=430
	TP_MAX						= TP_STORAGE_END + SLOT_SHOPITEM_NUM + SLOT_SHOPINVEN_NUM,// + SLOT_PETINVEN_NUM + SLOT_PETWEAR_NUM,	// (430) + 60 + 3
	
	TP_GUILDWAREHOUSE_START		= TP_MAX,													// 430
	TP_GUILDWAREHOUSE_END		= TP_GUILDWAREHOUSE_START + SLOT_GUILDWAREHOUSE_NUM,		// 490

	TP_JINBUB_START				= TP_GUILDWAREHOUSE_END,									// 490
	TP_JINBUB_END				= TP_JINBUB_START			+ SLOT_JINBUB_NUM,				// 495

	TP_PETINVEN_START			= 850,
	TP_PETINVEN_END				= TP_PETINVEN_START			+ SLOT_PETINVEN_NUM,		// +60

	TP_PETWEAR_START			= TP_PETINVEN_END,
	TP_PETWEAR_END				= TP_PETWEAR_START			+ SLOT_PETWEAR_NUM,			// +3
};
*/
enum
{
	//ITEMTABLE¡E?¡E￠c¡§u￠®¨I ￠®ic¡E?e
	TP_INVENTORY_START			= 0,
	TP_INVENTORY_END			= TP_INVENTORY_START		+ SLOT_INVENTORY_NUM,		// 40

	TP_EXTENDED_INVENTORY1_START	= TP_INVENTORY_END,
	TP_EXTENDED_INVENTORY1_END	= TP_EXTENDED_INVENTORY1_START	+ SLOT_EXTENDED_INVENTORY1_NUM,		// 60
	
	TP_EXTENDED_INVENTORY2_START	= TP_EXTENDED_INVENTORY1_END,
	TP_EXTENDED_INVENTORY2_END	= TP_EXTENDED_INVENTORY2_START	+ SLOT_EXTENDED_INVENTORY2_NUM,		// 80


	TP_WEAR_START				= TP_EXTENDED_INVENTORY2_END,										
	TP_WEAR_END					= TP_WEAR_START				+ SLOT_WEAR_NUM,			// 100

	TP_STORAGE_START			= TP_WEAR_END,											// 100
	TP_STORAGE_END				= TP_STORAGE_START			+ SLOT_STORAGE_NUM,			// 280

	TP_SHOPITEM_START			= TP_STORAGE_END,										// 240 <- 071126 280 같은대;; KTH;;
	TP_SHOPITEM_END				= TP_SHOPITEM_START			+ SLOT_SHOPITEM_NUM,		// 390 <- 071126 316 정도가 될듯...;; KTH;;

	TP_SHOPINVEN_START			= TP_SHOPITEM_END,										// 390
	TP_SHOPINVEN_END			= TP_SHOPINVEN_START		+ SLOT_SHOPINVEN_NUM,		// 430
	

	TP_MAX						= TP_SHOPITEM_END,										// 493
//	TP_MAX						= TP_STORAGE_END + SLOT_SHOPITEM_NUM + SLOT_SHOPINVEN_NUM,// + SLOT_PETINVEN_NUM + SLOT_PETWEAR_NUM,	// (430) + 60 + 3

	TP_GUILDWAREHOUSE_START		= 500,													// 500		// 430
	TP_GUILDWAREHOUSE_END		= TP_GUILDWAREHOUSE_START + SLOT_GUILDWAREHOUSE_NUM,	// 560		// 490

	TP_SKILL1_START			= 600,													// 600		// 555
	TP_SKILL1_END				= TP_SKILL1_START			+ SLOT_SKILL_NUM,			// 625		// 580

	TP_SKILL2_START			= TP_SKILL1_END,										// 625		// 580
	TP_SKILL2_END				= TP_SKILL2_START			+ SLOT_SKILL_NUM,			// 650		// 605

	TP_JINBUB_START				= TP_SKILL2_END,										// 650		// 490
	TP_JINBUB_END				= TP_JINBUB_START			+ SLOT_JINBUB_NUM,			// 655		// 495

	TP_QUICK_START				= 700,													// 700		// 605
	TP_QUICK_END				= TP_QUICK_START			+ SLOT_QUICK_NUM,			// 740		// 645

	TP_ABILITY_START			= TP_QUICK_END,											// 745		// 645
	TP_ABILITY_END				= TP_ABILITY_START			+ SLOT_ABILITY_NUM,			// 905		// 805

	TP_FAMILYWAREHOUSE_START		= 1000,													// 500		// 430
	TP_FAMILYWAREHOUSE_END		= TP_FAMILYWAREHOUSE_START + SLOT_FAMILYWAREHOUSE_NUM,	// 560		// 490

};

enum eITEMTABLE
{
	eItemTable_Inventory,
	eItemTable_Weared,
	eItemTable_Storage,
	eItemTable_Shop,
	eItemTable_ShopInven,
	eItemTable_TableMax,
	eItemTable_MunpaWarehouse,

	eItemTable_StreetStall,
	eItemTable_Exchange,
	eItemTable_Mix,
	eItemTable_RecvExchange,
	
	eItemTable_Skill,
	eItemTable_Quick,
	eItemTable_Deal,
	eItemTable_Ability,

	eItemTable_FamilyWarehouse,
	
	eItemTable_Max,
};

//temp
#define eItemTable_GuildWarehouse eItemTable_MunpaWarehouse

enum MONEY_FLAG {
	MF_NOMAL = 0, MF_OBTAIN, MF_LOST, MF_NONE, MF_PUTINGUILD = 20, MF_PUTOUTGUILD, MF_PUTINFAMILY, MF_PUTOUTFAMILY
};



enum EMAP_NUMBER
{
	eMapNum_BukKyung_Town,
	eMapNum_BukKyung_Field,
	eMapNum_BukKyung_Dungeon,

	eMapNum_NDryed_Gazell_Fall_Town,
	eMapNum_NDryed_Gazell_Fall_Field,
	eMapNum_NDryed_Gazell_Fall_Dungeon,

	eMapNum_GaeBong_Town,
	eMapNum_GaeBong_Field,
	eMapNum_GaeBong_Dungeon,
		
	eMapNum_JangAn_Town,
	eMapNum_JangAn_Field,
	eMapNum_JangAn_Dungeon,
	
	eMapNum_Max,
};

enum EAPPEAR_PART
{
	eAppearPart_Body,
	eAppearPart_Hair,
	eAppearPart_Face,
	eAppearPart_Hand,
	eAppearPart_Foot,
	eAppearPart_Weapon,
	
	eAppearPart_Max
};

// 071221 KTH -- 최대 기억할수 있는 맵의 수량
#define MAX_MEMORY_STONE_INFO	20

// ---- ¨Io￠®i￠®¨¡¨I¨￡A￠®E ¡§u¡§￠®AIAU AI¡Ii|¡§o¡§¡þ ¡§u¨Iⓒªd -----------------------------
#define MIN_SKILL_INDEX		100
#define MAX_SKILL_INDEX		4000
#define MIN_NAEGONG_INDEX		6000
#define MAX_NAEGONG_INDEX		7000
#define MIN_KYUNGGONG_INDEX		7000
#define MAX_KYUNGGONG_INDEX		8000
//----- ¡§u¡§u¡§¡þI ¨Io￠®i￠®¨¡¨I¨￡AI¡Ii|¡§o¡§¡þ ¡§u¨Iⓒªd ----------------------------------
#define MIN_ATTACK_SKILL_INDEX		100
#define MAX_ATTACK_SKILL_INDEX		2000
#define MIN_DEFENSE_SKILL_INDEX	2000
#define MAX_DEFENSE_SKILL_INDEX	3000
#define MIN_ASSIST_SKILL_INDEX		3000
#define MAX_ASSIST_SKILL_INDEX		4000
#define ATTACK_SKILL_INTERVAL		200
#define DEFENSE_SKILL_INTERVAL		100
#define ASSIST_SKILL_INTERVAL		100

#define MIN_GRIDITEM_AREA		0
#define MAX_GRIDITEM_AREA		1000


// ---- AI¡§¡þ￠®I¡§￠®¡Ec¡E¡þ¡EcA￠®E ¡§u¡§￠®AIAU AI¡Ii|¡§o¡§¡þ ¡§u¨Iⓒªd -----------------------------
#define MIN_YOUNGYAKITEM_INDEX	8000
#define MAX_YOUNGYAKITEM_INDEX	9000
#define MIN_SKILLITEM_INDEX	9000
#define MAX_SKILLITEM_INDEX	11000
#define MIN_EQUIPMENTITEM_INDEX 11000
#define MAX_EQUIPMENTITEM_INDEX 32000
// ---- AbilityIndex
#define MIN_ABILITY_INDEX	60000
#define MAX_ABILITY_INDEX	61000


#define WALKSPEED		425//425//225
#define RUNSPEED		650//650//450


#define MOVE_LIMIT_ERRORDISTANCE	1000

#define TEMPITEMDBIDX 0xffffffff
#define MAX_SERVERINFO_LIST		100

#define MAX_MAP_NUM 100
#define MAX_NAME_LENGTH				16
#define MAX_SKILL_NAME_LENGTH		32
#define MAX_NPC_NAME_LENGTH		32
#define MAX_MAP_NAME_LENGTH		32
#define MAX_MONSTER_NAME_LENGTH		60
#define MAX_CHAT_LENGTH				127
#define	MAX_CHANNELTITLE_LENGTH		64
#define	MAX_PLAYROOMTITLE_LENGTH	64
#define MAX_ITEMNAME_LENGTH			32
#define MAX_PETNAME_LENGTH			32		//임시로 설정했음		

//#define MAX_CHARACTER_NUM			4
#define MIN_GRIDITEM_AREA			0
#define MAX_GRIDITEM_AREA			1000
 
#define MAX_PICKITEM_DISTANCE	500

#define MAX_AUCTIONBOARD_DISTANCE 500


#define MAX_BOARDTITLE_NUM	10
#define MAX_BOARDTITLE_LENGTH 64
#define MAX_CONTENTS_LENGTH	65535


#define MAX_TARGET_NUM		300

//-----¨Io￠®i￠®¨¡¨I¨￡ ￠®¨¡¨I¨￡￠®¨¡Y--------------------------------------
enum {GENERAL_ATTACK=0,SKILL_ATTACK=1};
//#define GENERAL_ATTACK_DISTANCE		200
// 070316 LYW --- CommonGameDefine : Modified distance
//#define POINT_VALID_DISTANCE		3000			// 10m: ¡§￠®￠®AAI¡§￠®¡Ec E¨Io¡Ii¨I￠® /E￠®I ¨Iou'
// 070507 LYW --- CommonGameDefine : Modified distance
#define POINT_VALID_DISTANCE		6500


#define REMOVEFADE_START_TIME		2500
#define MONSTERREMOVE_TIME			5000
#define PLAYERREVIVE_TIME			5000
#define	PLAYERREVIVE_TIME_IN_BOSSMAP	60000


// 낚시관련 Define
#define MAX_FISHING_LEVEL			4			// 낚시숙련도 단계
#define MAX_FISHING_MISSION			3			// 낚시미션개수
#define MAX_FISHLIST_FORPOINT		12			// 물고기리스트 : 포인트변환시 사용
#define MAX_FISHINGMISSIONCODE		99			// 
#define MAX_FISHING_DISTANCE		2300.0f		// 낚시 가능한 거리
#define MAX_FISHPOINT				9999999		// 최대 물고기포인트 9,999,999
#define FISHINGTIME_STARTPULLING	2000		// 낚시질 시작시간
#define FISHINGMISSION_INVALID		1000000		// 미션코드는 000000 ~ 999999 범위이므로 이를 초과하면 무효값.

enum BASICEFFECT
{
	eEffect_NewCharacter = 1,	//A¨Iⓒª¡E¡þ?AI ¡IiiAa
	eEffect_Standard = 2,		//A¨Iⓒª¡E¡þ?AI ￠®¨ua¡§¡þ￠®i(¨Io￠®i￠®¨ua¡§u¨I¨￡AI)
	eEffect_Walk,	//￠®¨¡E￠®¨ua (¨Io￠®i￠®¨ua ¡§u¨I¨￡AI)
	eEffect_Run,	//¡EOU￠®¨ua (¨Io￠®i￠®¨ua¡§u¨I¨￡AI)
	eEffect_BattleWalk_Gum,		//AuAo ￠®¨¡E￠®¨ua(￠®¨¡E_¨Io￠®i￠®¨uaAaA¨I¨￡)
	eEffect_BattleWalk_Gwun,		//AuAo ￠®¨¡E￠®¨ua(￠®¨uC¨Io￠®i￠®¨uaAaA¨I¨￡)
	eEffect_BattleWalk_Do,		//AuAo ￠®¨¡E￠®¨ua(¡Ii¡Ii_¨Io￠®i￠®¨uaAaA¨I¨￡)
	eEffect_BattleWalk_Chang,		//AuAo ￠®¨¡E￠®¨ua(A￠®E_¨Io￠®i￠®¨uaAaA¨I¨￡)
	eEffect_BattleWalk_Gung,		//AuAo ￠®¨¡E￠®¨ua(￠®¨uA_¨Io￠®i￠®¨uaAaA¨I¨￡)
	eEffect_BattleWalk_Amgi,		//AuAo ￠®¨¡E￠®¨ua(¡§uI￠®¨ua_¨Io￠®i￠®¨uaAaA¨I¨￡)
	eEffect_BattleRun_Gum,		//AuAo ¡EOU￠®¨ua(￠®¨¡E_¨Io￠®i￠®¨uaAaA¨I¨￡)
	eEffect_BattleRun_Gwon,		//AuAo ¡EOU￠®¨ua(￠®¨uC_¨Io￠®i￠®¨uaAaA¨I¨￡)
	eEffect_BattleRun_Do,		//AuAo ¡EOU￠®¨ua(¡Ii¡Ii_¨Io￠®i￠®¨uaAaA¨I¨￡)
	eEffect_BattleRun_Chang,		//AuAo ¡EOU￠®¨ua(A￠®E¨Io￠®i￠®¨uaAaA¨I¨￡)
	eEffect_BattleRun_Gung,		//AuAo ¡EOU￠®¨ua(￠®¨uA_¨Io￠®i￠®¨uaAaA¨I¨￡)
	eEffect_BattleRun_Amgi,		//AuAo ¡EOU￠®¨ua(¡§uI￠®¨ua_¨Io￠®i￠®¨uaAaA¨I¨￡)
	eEffect_UngijosikStart,	//¡E?i￠®¨uav¡§oA(¡§uE¡E¡IA¡Ii¡E?AU)
	eEffect_Ungijosik,	//¡E?i￠®¨uav¡§oA
	eEffect_Ungijosik_End,	//¡E?i￠®¨uav¡§oA (¨Iⓒª¡E￠c¨Iⓒª￠®¨IEA)
	eEffect_UngijosikDamage,	//¡E?i￠®¨uav¡§oA A¨I¡þ¡E?¡E￠c ¡Ii￠®I¨IoIAo
	eEffect_StallStart,	//¨Iⓒªea￠®io ¡Ii¡E?AU1 (¡§uE￠®¨ua)
	eEffect_Stall,	//¨Iⓒªea￠®io ¡Ii¡E?AU1 (¡§uE¡§u¡§￠® AO￠®¨ua)
	eEffect_StallEnd,	//¨Iⓒªea￠®io ¡Ii¡E?AU1 (¡§u￠®¨I￠®¨ua)
	eEffect_NormalDamage = 13,		//¡Ii￠®I¨IoIAo1 (AA)
	eEffect_CriticalDamage = 14,		//¡Ii￠®I¨IoIAo2 (¡E?i)
	eEffect_Die1 = 16,		//A￠®￠?= 1
	eEffect_Died1 = 16,		//A￠®￠?¡§ui¡§u￠®¨I ¡E¡I¡§I¡E?o AO¡E¡IA ¡E¡þ¨I¡I¡§oA 1
	eEffect_Die2 = 15,		//A￠®￠?= 2
	eEffect_Died2 = 15,		//A￠®￠?¡§ui¡§u￠®¨I ¡E¡I¡§I¡E?o AO¡E¡IA ¡E¡þ¨I¡I¡§oA 2
	eEffect_LevelUpSentence = 3,	//￠®￠´¨Iiⓒo¡Ei¨u￠®¡¿¡§u￠®A ￠®¨uUAU
	eEffect_MouseMovePoint,	//¡E¡þ¡EO¡E?i¡§o¡§¡þ AI¡Ii¡E?¡§￠®￠®AAI¡§￠®¡Ec
	
	eEffect_GetMoney = 5,	// ¡E¡þ￠®AA￠®￠?¡E?¡§I¡§u￠®¨I ¡Ii￠®￠´ ¡§uo￠®¨ua
	eEffect_GetItem_Accessory = 25,	// ¡E¡þ￠®AA￠®￠?¡E?¡§I¡§u￠®¨I ¡§u¡§￠®AIA¡§￠® ¡§uo￠®¨ua

	eEffect_Emoticon_Bow,	//AI￠®ic
	eEffect_Emoticon_Thanks,	//￠®¨¡￠®¡×￠®ic
	eEffect_Emoticon_Yes,	//￠®¨uad 
	eEffect_Emoticon_No,	//¡§¡þId
	eEffect_Emoticon_Pleasure,	//￠®¨ua￠®iY
	eEffect_Emoticon_Sadness,	//¡§o¡§oCA
	eEffect_Emoticon_Love,	//￠®ic¡EOu
	eEffect_Emoticon_Hostility,	//Au¡E¡Ie
	eEffect_Emoticon_Disappoint,	//¡§oC¡E¡þA
	eEffect_Emoticon_Angry,	//¡§¡þ¡§¡E¨Iⓒªe
	eEffect_Emoticon_Suprised,	//¨Iⓒªi¡EO￠®A
	eEffect_Emoticon_Evasion,	//E¡E¡þCC
	eEffect_Emoticon_Declare,	//¡§u￠®¨u¡§u¨I¡I
	eEffect_Defence_Gum,		//¨Iiⓒo|i¡¾¡§ui¡Ii¡E?AU(￠®¨¡E)
	eEffect_Defence_Gwun,		//¨Iiⓒo|i¡¾¡§ui¡Ii¡E?AU(￠®¨uC)
	eEffect_Defence_Do,		//¨Iiⓒo|i¡¾¡§ui¡Ii¡E?AU(¡Ii¡Ii)
	eEffect_Defence_Chang,		//¨Iiⓒo|i¡¾¡§ui¡Ii¡E?AU(A￠®E)
	eEffect_Defence_Gung,		//¨Iiⓒo|i¡¾¡§ui¡Ii¡E?AU(￠®¨uA)
	eEffect_Defence_Amgi,		//¨Iiⓒo|i¡¾¡§ui¡Ii¡E?AU(¡§uI￠®¨ua)

	eEffect_GetItem_Weapon = 7,		//¨Iiⓒo|i¡¾¡§ui¡Ii¡E?AU(¡§uI￠®¨ua)
	eEffect_GetItem_Dress = 8,
	eEffect_GetItem_Quest = 10,
	eEffect_GetItem_Youngyak = 6,

	eEffect_MonRegen = 80,				// 앞으로 몬스터 리젠은 eEffect_NewCharacter 이펙트를 사용함
	eEffect_ChangeStage_Hwa = 98,
	eEffect_ChangeStage_Geuk = 99,

	eEffect_Jackpot = 1034,				// 잿팟당첨

	eEffect_PetSummon = 3151,			// 펫소환
	eEffect_PetSeal		= 3152,			// 펫봉인
	eEffect_FeedUp		= 3153,			// 펫먹이
	eEffect_PetWarp		= 3154,			// 펫워프

	eEffect_ShopItem_Life = 3200,		// 금창약	
	eEffect_ShopItem_NaeRuyk,			// 요상단	
	eEffect_ShopItem_Revive,			// 부활주문서
	eEffect_ShopItem_Boom1,				// 폭죽(나비)
	eEffect_ShopItem_Boom2,				// 폭죽(별)
	eEffect_ShopItem_Hair1,				// 가발1	
	eEffect_ShopItem_Hair2,				// 가발2	
	eEffect_ShopItem_Hair3,				// 가발3	
	eEffect_ShopItem_Teleport1,			// 텔레포트1
	eEffect_ShopItem_Teleport2,			// 텔레포트2
	eEffect_ShopItem_Teleport3,			// 텔레포트3
	eEffect_ShopItem_MaxLife,			// 생명의 물약
	eEffect_ShopItem_MaxNaeRuyk,		// 내력의 물약

	// 070528 LYW CommonGameDefine : Add effect of emoticon part.
	eEffect_Emoticon1 = 51, 
	eEffect_Emoticon2 = 52, 
	eEffect_Emoticon3 = 53, 
	eEffect_Emoticon4 = 54, 
	eEffect_Emoticon5 = 55, 
	eEffect_Emoticon6 = 56, 

	eEffect_GetItem_Unique	= 40,
	eEffect_GetItem_Rare	= 41,

	// 071206 LUJ 길드 졸업 효과
	eEffect_Guild_Graduate	= 49,
};


enum
{
	Alker_Farm = 2,
	Moon_Blind_Swamp = 13,		//E￠®¨IA￠®iAo			Moon_Blind_Swamp
	Red_Orc_Outpost,			//¡E¡Ie¡§uEA¡§u			Red_Orc_Outpost
	Moon_Blind_Forest,			//A¨I¨￡¡§oAE¨Ii¨o?i¨u¡§¡I			Moon_Blind_Forest
	Haunted_Mine_1F = 16,
	Haunted_Mine_2F = 17,
	The_Gate_Of_Alker = 19,			//¡E?y¡Ec￠®¨¡u			The_Gate_Of_Alker
	Alker_Harbor,		//¡E¡þi¡§u¡§￠®￠®ie			Alker_Harbor

	Ruins_Of_Draconian,		//CuAO				Ruins_Of_Draconian
	Zakandia,		//¡§uc¡§uc				Zakandia
	Tarintus,			//Aa￠®ic				Tarintus
	Mont_Blanc_Port = 25,		//¡Ii¡E?dE￠®I			Mont_Blanc_Port
	Dryed_Gazell_Fall,			//¡§uC¡§uc￠®￠´c			Dryed_Gazell_Fall
	Zakandia_Outpost,			//C￠®￠?AO				Zakandia_Outpost
	The_Dark_Portal,			//¡§uOAO				Tournament
	Distorted_crevice = 29,
	The_Way_To_The_Howling_Ravine = 31,
	Howling_Ravine = 32,
	Howling_Cave_1F = 33,
	Howling_Cave_2F = 34,
	Ghost_Tree_Swamp = 41, 
	Lair_of_Kierra = 42,
	The_Valley_Of_Fairy = 51,
	The_Town_of_Nera_Castle = 52,
	The_Great_Garden = 53,
	// 080714 LUJ, "기사단의 묘지" 맵 추가
	TheKnightsGrave	= 54,
	A_Harbor_of_Nera = 55,
	DateDungeon_Cemetery = 91,
	DateDungeon_BrokenHouse = 92,
	DateDungeon_DarkCave = 93,


	MAX_MAP_ENUMCOUNT,
};

enum EObjectState
{
	eObjectState_None,						// ¡§u¡§￠®¨Io￠®i￠®ioAA ¡§u¡§￠®¡E¡IO..
	eObjectState_Enter,						// A¨Iⓒª¡E¡þ?AI¨Iⓒª¡§¡I ¡E¡þo¡§o¡§¡þAI￠®¨¡¡E￠c A¨Iⓒª= ¡IiiAaCO¡EO￠®¡¿ AC ￠®ioAA
	eObjectState_Move,						// AAC￠®I AI¡Ii¡E? ￠®ioAA
	eObjectState_Ungijosik,					// ¡E?i￠®¨ua ￠®ioAA
	eObjectState_Rest,						// ¡E¡þo¡§o¡§¡þAIAC E¡§¡ⓒ¡§oA ￠®ioAA
	
//	eObjectState_Speech,					// Npc¡E?I ¡E¡IeE￠®¨I ￠®ioAA
	eObjectState_Deal,						// ￠®ioa Open ￠®ioAA

	eObjectState_Exchange,					// ￠®¨u¨IⓒªE? ￠®ioAA
	eObjectState_StreetStall_Owner,			// ¨Iⓒªea￠®io Open ￠®ioAA
	eObjectState_StreetStall_Guest,			// ¨Iⓒªea￠®io ¡§uO¡E¡IO ￠®ioAA
	eObjectState_PrivateWarehouse,			// ￠®¨¡¨IⓒªAIA￠®E￠®¨¡i 10
	eObjectState_Munpa,						//¨Io¡Ec¡§￠®A ¡E¡þ¡Ec¡§iⓒo¡Ei¨u¡§￠®¡Ec¨Iⓒª¡§¡I ¨Io¡Ec¡§￠®AA￠®E

	eObjectState_SkillStart,				// ¡§o¡§¡þA¨Iⓒª ¡§oAAu ￠®ioAA
	eObjectState_SkillSyn,					//
	eObjectState_SkillBinding,				//
	eObjectState_SkillUsing,				//
	eObjectState_SkillDelay,				// A¡§¡ⓒ¡§¡þ¡E¡þ￠®¨¡¡E￠c ¨I¡A￠®!¨¡a; ¡EO￠®¡¿, ¨Io￠®i￠®¨¡¨I㎕鈐??¨I¡A￠®¨¡i¨Iⓒª￠®¨IEAAC ¡Iio￠®￠´¨IoAI
	
	eObjectState_TiedUp_CanMove,			// ￠®¨¡a¨IoU ￠®ioAA
	eObjectState_TiedUp_CanSkill,
	eObjectState_TiedUp,
	
	eObjectState_Die,						// A￠®￠?: ￠®ioAA 20	
	
	eObjectState_BattleReady,
	eObjectState_Exit,						//KES 040707 A¨Iⓒª¡E¡þ?AI￠®¨¡¡E￠c ¨Iⓒª¡§¡I￠®¨¡￠®I¡EO￠®¡¿
	eObjectState_Immortal,					//¡¤I¡¾¡¿AI, ￠￢EAuE?, AuAa'A￠® ¡ii¨u¨¡ⓒø¨￡¡¾a¨oA ⓒo¡iAu¡ioAA

	eObjectState_Society,					//KES A¿¹A´IÆ¼ μ¿AU
	
	eObjectState_ItemUse,

	eObjectState_TournamentReady,
	eObjectState_TournamentProcess,
	eObjectState_TournamentEnd,
	eObjectState_TournamentDead,

	eObjectState_Engrave,

	// 070503 LYW --- CommonGameDefine : Add enum to setting rest damage.
	eObjectState_RestDamage,
	eObjectState_Connect,					// 웅주 070823

	// 080428 NYJ --- 낚시상태
	eObjectState_Fishing,
	eObjectState_FishingResult,
	
	eObjectState_MAX,	
};

enum eObjectBattleState
{
	eObjectBattleState_Peace,
	eObjectBattleState_Battle,
	eObjectBattleState_Max,
};
//---￠®¨u¨IⓒªE?¡§u¡§u¡§¡þI￠®ioAA(CoAc ￠®ic¡E?e¡§uECI¡E¡IAA¨I¡þ)
enum eEXCHANGE_STATE
{
	eEXSTT_NONE,
	eEXSTT_APPLYING,	//￠®¨u¨IⓒªE?¡§oAA￠®i; CN ￠®ioAA
	eEXSTT_APPLIED,		//￠®¨u¨IⓒªE?¡§oAA￠®i; ¨Io¡§¡ⓒ: ￠®ioAA
	eEXSTT_EXCHANGEING,	//￠®¨u¨IⓒªE?; A¨I¨￡CaA¨I¡þAI ￠®ioAA : eObjectState_Exchange

	eEXSTT_MAX,
};



// ￠®¨¡￠®E~ ID¡E?¡E￠c ¡E¡IeCN [6/2/2003]
// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.05.07
#define FARM_RENDER_OBJ_ID_START	0x8fffffff
// E 농장시스템 추가 added by hseos 2007.05.07
#define STATICNPC_ID_START		0xa0000000
#define STATICNPC_ID_MAX		0x10000000
#define SKILLOBJECT_ID_START	0xc0000000
#define SKILLOBJECT_ID_MAX		0x0000ffff
#define BATTLE_ID_START			0xd0000000
#define BATTLE_ID_MAX			0x0000ffff
#define TEMP_SKILLOBJECT_ID		0xffffffff
#define BATTLE_TEAMEFFECT_ID	0xfffffffe
#define BATTLE_STAGEEFFECT_ID	0xfffffffd
#define STATE_EFFECT_ID			0xfffffffc
#define PK_EFFECT_ID			0xfffffffb
#define NPCMARK_1_ID			0xfffffffa		// !
#define NPCMARK_2_ID			0xfffffff9		// ?
#define PARTYWAR_STAGEEFFECT_ID	0xfffffff8
#define BATTLE_GTTEAM1_EFFECTID	0xfffffff7
#define BATTLE_GTTEAM2_EFFECTID	0xfffffff6
#define JACKPOT_EFFECTID		0xfffffff5
#define ENGRAVE_EFFECTID		0xfffffff4
#define ENGRAVE_ACK_EFFECTID	0xfffffff3
#define ENGRAVE_NACK_EFFECTID	0xfffffff2
#define LIFE_RECOVER_EFFECTID	0xfffffff1
#define MANA_RECOVER_EFFECTID	0xfffffff0
#define ALL_RECOVER_EFFECTID	0xffffffee

// 06. 03 국내무쌍 - 이영준
#define MUSSANG_EFFECT_ID1		0xffffffed
#define MUSSANG_EFFECT_ID2		0xffffffec
#define MUSSANG_EFFECT_ID3		0xffffffeb

#define DETECT_EFFECT_ID		0xffffffea

#define HELMET_EFFECT			0xffffffe9
#define SHIELD_EFFECT			0xffffffe8
#define HEART_EFFECT			0xffffffe7
#define ITEM_WORKING_SUCCESS_EFFECT	0xffffffe6
#define ITEM_WORKING_FAIL_EFFECT	0xffffffe5
#define ITEM_WORKING_EFFECT			0xffffffe4

#define FISHING_START_EFFECT		0xffffffe3
#define FISHING_SUCCESS_EFFECT		0xffffffe2
#define FISHING_FAIL_EFFECT			0xffffffe1

#define ITEM_SUMMON_EFFECTACTIONID	0xffffffde
#define ITEM_SUMMON_EFFECTID		0xffffffdd

#define MAX_SKILLMAINTAIN_EFFECT	3

// ¨Io¨I￠®Ca AI¡Ii|¡§iⓒo¡Ei¨u¡E?¡E￠c ¡E¡IeCN ¡§u¨Iⓒªd
typedef BYTE DIRINDEX;
#define MAX_DIRECTION_INDEX		32
#define DEGTODIR(angleDeg)	(BYTE)((angleDeg)/360*MAX_DIRECTION_INDEX)
#define DIRTODEG(DirIndex)	(((float)DirIndex)*360/MAX_DIRECTION_INDEX)
#define DIRTORAD(DirIndex)	(DEGTORAD(DIRTODEG(DirIndex)))
#define RADTODIR(angleRad)	(DEGTODIR(RADTODEG(angleRad)))

// A￠®i¡E¡þ¡§¡ⓒ¡EOo ¡§u¨Iⓒªd
#define CAMERA_NEAR		100
#define MAX_CHARTARGETPOSBUF_SIZE	15

//system
#define MAX_IPADDRESS_SIZE	16

// type : 0->¡§oA￠®¨¡￠®I¡EO￠®¡¿¡§¡þ￠®¨¡ ¨Iⓒª¨I¡A￠®¨ue. 1~100->AId￠®￠´¡Ec AI￠®io¡§¡þ?E￠®¨I¡§oA ¨Iⓒª¨I¡A￠®¨ue(￠®¨¡¡E￠c￠®ie), 101~200->(￠®¨¡￠®¡×￠®ie)
// 1->￠®ioa¡E?¡E￠c ¡§￠®C¡E¡þA, 2->¨Iⓒªea¡E?¡E￠c¡§u￠®¨I ¡§￠®C¡E¡þA, 3->¡E¡þo¡§o¡§¡þAI¡E?¡E￠c￠®¨¡O E¨Io¡Ii¨I￠®, 4->￠®¨u¨IⓒªE?, 5->A￠®E￠®¨¡i¡E?¡E￠c¡§u￠®¨I ￠®i￠®ⓒ­¡EO￠®¡¿, 6->¨Io¡Ec¡§￠®AA￠®E￠®¨¡i¡E?¡E￠c¡§u￠®¨I ￠®i￠®ⓒ­¡EO￠®¡¿
// 101->￠®ioa¡E?¡E￠c¡§u￠®¨I ￠®ii¡EO￠®¡¿, 102->¨Iⓒªea¡E?¡E￠c¡§u￠®¨I ￠®ii¡EO￠®¡¿, 103->A￠®￠?¡§uu;¡EO￠®¡¿, 104->￠®¨u¨IⓒªE?, 105->A￠®E￠®¨¡i¡E?¡E￠c ¨IⓒªO=, 106->¨Io¡Ec¡§￠®AA￠®E￠®¨¡i¡E?¡E￠c ¨IⓒªO=, 107->C￠®I￠®¨u¨Io ￠®ii¡EO￠®¡¿
enum eMoneyLogtype
{
	eMoneyLog_Time				= 0,
// ￠®¨¡¡E￠c￠®ie
	eMoneyLog_GetStall			= 1,
	eMoneyLog_GetStreetStall	= 2,
	eMoneyLog_GetMonster		= 3,
	eMoneyLog_GetExchange		= 4,
	eMoneyLog_GetStorage			= 5,
	eMoneyLog_GetGuild			= 6,
	eMoneyLog_GetWantedPrize	= 7,
	eMoneyLog_GetPKLooting		= 8,
	eMoneyLog_GetSuryunFail		= 9,
	eMoneyLog_GetPrize			= 10,
	eMoneyLog_GetSWProfit		= 11,
	eMoneyLog_GetFromQuest		= 12,
	eMoneyLog_GetGMTool			= 13,
	eMoneyLog_GetFamily,
	// desc_hseos_몬스터미터01
	// S 몬스터미터 추가 added by hseos 2007.07.30
	eMoneyLog_GetMonstermeter,
	// E 몬스터미터 추가 added by hseos 2007.07.30

	
// ￠®¨¡￠®¡×￠®ie
	eMoneyLog_LoseStall			= 101,
	eMoneyLog_LoseStreetStall	= 102,
	eMoneyLog_LoseDie			= 103,
	eMoneyLog_LoseExchange		= 104,
	eMoneyLog_LoseStorage		= 105,
	eMoneyLog_LoseGuild			= 106,
	eMoneyLog_LoseStorageBuy		= 107,
	eMoneyLog_LoseWantedRegist	= 108,
	eMoneyLog_LoseWantedBuyRight = 109,
	eMoneyLog_LosePKLooted		= 110,
	eMoneyLog_LosePKModeExitPanelty = 111,
	eMoneyLog_LoseBadFrameBail	= 112,
	eMoneyLog_LoseSuryunFee		= 113,
	eMoneyLog_LoseGuildLevelUp	= 114,
	eMoneyLog_LoseChangeLocation	= 115,
	eMoneyLog_LoseGMTool		= 116,
	eMoneyLog_LoseFamily,
	eMoneyLog_LoseFamilyLevelUp,
	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.08.23	2007.09.10	2008.01.15
	eMoneyLog_LoseFarmBuy,
	eMoneyLog_LoseFarmUpgrade,
	eMoneyLog_LoseFarmTax,
	// E 농장시스템 추가 added by hseos 2007.08.23	2007.09.10	2008.01.15

//
	eMoneyLog_GetCheatMoney = 201,
};

enum eLogSkill
{
	eLog_SkillLearn = 0,
	eLog_SkillDiscard,
	eLog_SkillDestroyByWanted,
	eLog_SkillDestroyByBadFame,
	eLog_SkillLevelup,
	eLog_SkillCheatLevelup,
	eLog_SkillLevelDownByWanted,
	eLog_SkillLevelDownByBadFame,
	eLog_SkillDestroyByGetJulCho,
	eLog_SkillDestroyByBadFameJulChoDown,
	eLog_SkillDestroyByGetNextLevel,		// for japan

	////////////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환
	eLog_SkillOption,
	eLog_SkillOptionClear,
	eLog_SkillDestroyByOptionClear,
	////////////////////////////////////////////////////////////////////////////////

	eLog_Skill_RM	= 100,
	eLog_SkillLearn_RM,
	eLog_SkillLevel_RM,
	eLog_SkillRemove_RM,

	eLog_SkillGetPoint,
};


enum eFamilyLog
{
	// 포인트
	eLog_FamilyPoint	= 0,
	eLog_FamilyPointAdd,
	// 080610 LUJ, 포인트 변경
	eLog_FamilyPointUpdate,

	// 농장
	eLog_FamilyFarm		= 100,
	eLog_FamilyFarmPutCrop,		// 파종
	eLog_FamilyFarmDieCrop,		// 고사
	eLog_FamilyFarmGrowCrop,	// 생장
	eLog_FamilyFarmCareCrop,	// 비료
	eLog_FamilyFarmHarvestCrop,	// 수확
	eLog_FamilyFarmLostRight,	// 권리 상실(패밀리 탈퇴 등으로 인한 권리 상실)
	eLog_FamilyFarmDisband,		// 농장 해체(패밀리 해산으로 인한)
	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2008.01.16
	eLog_FamilyFarmTax,			// 농장 해체(관리비 미납)
	eLog_FamilyFarmMystery,		// 농장 해체(농장 해체 시 로그를 추가하지 않았을 경우)
	// E 농장시스템 추가 added by hseos 2008.01.16
	// 080425 KTH -- 축사 관련 로그 추가 --------------
	eLog_FamilyFarmInstallAnimal,
	eLog_FamilyFarmDieAnimal,
	eLog_FamilyFarmFeedAnimal,
	eLog_FamilyFarmCleanAnimal,
	eLog_FamilyFarmAnimalReward,
	// 081205 LUJ, 패밀리 행동
	eLog_FamilyCreate = 200,
	eLog_FamilyAddMember,
	eLog_FamilyChangeNick,
	eLog_FamilyRegistEmblem,
	eLog_FamilyLeaveMember,
	eLog_FamilyExpelMember,
	eLog_FamilyBreakup,
};

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2008.01.23
enum FARM_LOG_KIND																			// 농장 로그 종류
{
	FARM_LOG_KIND_BUY = 0,																	// ..구입
	FARM_LOG_KIND_UPGRADE,																	// ..업그레이드
	FARM_LOG_KIND_PAY_TAX,																	// ..관리비 납부
	FARM_LOG_KIND_DEL_FAMILY_BREAKUP,														// ..삭제 - 패밀리 해체
	FARM_LOG_KIND_DEL_TAX_ARREARAGE,														// ..삭제 - 관리비 체납
	FARM_LOG_KIND_MYSTERY = 9999,
};

enum FARM_UPGRADE_LOG_KIND																	// 농장 업그레이드 로그 종류
{
	FARM_UPGRADE_LOG_KIND_GARDEN = 0,														// ..텃밭
	FARM_UPGRADE_LOG_KIND_FENCE,															// ..울타리
	FARM_UPGRADE_LOG_KIND_HOUSE,															// ..집
	FARM_UPGRADE_LOG_KIND_WAREHOUSE,														// ..창고
	FARM_UPGRADE_LOG_KIND_ANIMALCAGE,														// ..축사
	FARM_UPGRADE_LOG_KIND_MYSTERY = 9999,
};
// E 농장시스템 추가 added by hseos 2008.01.23

enum eLogitemmoney
{
	eLog_ItemNone		= 0,
	eLog_ItemDiscard	= 1,
	eLog_PetItemDiscard,
	
	//item move
	eLog_ItemMoveInvenToStorage = 100,
	eLog_ItemMoveStorageToInven,
	eLog_ItemMoveStorageToStorage,
	eLog_ItemMoveInvenToGuild,
	eLog_ItemMoveInvenToInven,
	eLog_ItemMoveGuildToInven,
	eLog_ItemMoveGuild,
	eLog_MoneyDepositToGuild,
	eLog_MoneyWithdrwaGuild,
	// 080716 LUJ, 펫<->플레이어 이동
	eLog_ItemMoveFromPlayerToPet,
	eLog_ItemMoveFromPetToPlayer,
	
	//item obtain/destory
	eLog_ItemObtainBuy = 200,
	eLog_ItemObtainDivide,
	eLog_ItemObtainCheat,
	eLog_ItemObtainMonster,
	eLog_ItemObtainQuest,
	eLog_ItemObtainPK,
	eLog_ItemObtainDissolve,
	eLog_ItemObtainMix,
	eLog_ItemObtainFromChangeItem,
	eLog_ItemRareObtainCheat,
	eLog_ItemOptionObtainCheat,
	// desc_hseos_몬스터미터01
	// S 몬스터미터 추가 added by hseos 2007.07.08
	eLog_ItemObtainMonstermeter,
	// E 몬스터미터 추가 added by hseos 2007.07.08

	// desc_hseos_결혼_01
	// S 결혼 추가 added by hseos 2007.12.13
	eLog_ItemObtainMarriage,
	// E 결혼 추가 added by hseos 2007.12.13

	eLog_ItemObtainFishing,
	// 080509 LUJ, 선물 획득
	eLog_ItemObtainGift,
	eLog_ItemObtainMerge,

	eLog_ItemSell = 300,
	eLog_ItemUse,
	
	eLog_StreetStallBuyAll = 400,
	eLog_StreetStallBuyDivide,
	eLog_StreetStallSellDivide,

	eLog_Exchange = 500,

	eLog_ItemReinforceSuccess = 600,
	eLog_ItemReinforceFail,
	eLog_ItemReinforceDataHaveTooManyMaterials,
	eLog_ItemReinforceSourceInvalid,
	eLog_ItemReinforceMaterialIsInvalid,
	eLog_ItemReinforceWhichMaterialHasZero,
	eLog_ItemReinfoceYouCannotOver100ToReinforce,
	eLog_ItemReinforceSourceItemNotExist,
	eLog_ItemRerinfoceServerHasNoReinforceScript,
	eLog_ItemReinforceMaterialSourceItemCannnotReinforce,
	eLog_ItemReinforceSourceItemCanDuplicate,
	eLog_ItemReinforceRemovingMaterialIsFailed,
	eLog_ItemReinforceUpdatingItemIsFailed,

	eLog_ItemMixSuccess = 700,
	eLog_ItemMixFail,
	eLog_ItemMixBigFail,
	eLog_ItemMixInsufficentSpace,
	eLog_ItemMixServerError,
	eLog_ItemMixUse,
	
	eLog_ItemDissolveSuccess,
	eLog_ItemDissolvePlayerNotExist,
	eLog_ItemDissolveSourceItemNotExist,
	eLog_ItemDissolveServerHaveNoDissolvingData,
	eLog_ItemDissolveItemDataNotMatched,
	eLog_ItemDissolveItNeedMoreLevel,
	eLog_ItemDissolveRemovingItemIsFailed,
	eLog_ItemDissolveAddingItemIsFailed,
	eLog_ItemDissolvePlayserHasNoInventory,
	eLog_ItemDissolveInsufficentSpace,
	eLog_ItemDissolveNoResult,
	eLog_ItemDissolveServerError,
	eLog_ItemDissolveScriptInvalid,
	eLog_ItemDissolveInsert,		// 빈 슬롯에 아이템을 넣음
	eLog_ItemDissolveAddDuplicated,	// 분해시 아이템이 든 슬롯에 겹쳐지는 아이템을 추가함

	eLog_ItemEnchantSuccess,
	eLog_ItemEnchantItemInvalid,
	eLog_ItemEnchantServerError,
	eLog_ItemEnchantHacking,
	eLog_ItemEnchantItemUpdateFailed,
	eLog_ItemEnchantUse,
	eLog_ItemEnchantDestroy,

	eLog_ItemMixSystemFault,	// 조합 시 시스템 오류
	eLog_ItemMixNotFound,		// 조합할 대상을 찾지 못함
	eLog_itemMixBadRequest,		// 잘못된 요청
	eLog_ItemMixBadCondition,	// 조합할 조건이 되지 않음
	eLog_ItemMixBadQuantity,	// 개수가 잘못됨
		
	eLog_ItemDestroyMix = 800,
	eLog_ItemDestroyReinforce,
	eLog_ItemDestroyUpgrade,
	eLog_ItemDestroyByWanted, // eLog_ItemDestroyByBadFame¸| AIAEA¸·I ³≫·A¾ßAo
	eLog_ItemDestroyByBadFame, //die panelty with bad fame.
	eLog_ItemDestroyDissolution, // dissolve base item
	eLog_ItemDestroyGiveQuest, // by complete quest 
	eLog_ItemDestroyDeleteQuest,
	eLog_ItemDestroybyChangeItem,
	eLog_ItemDestroyReinforceWithShopItem,

	eLog_MoneyObtainPK = 1000,
	//exp obtain
	eLog_ExpObtainPK,

	eLog_ShopItemUse = 1500,	
	eLog_ShopItemUseEnd,
	eLog_ShopItemMoveToInven,
	eLog_ShopItemProtectAll,
	eLog_ShopItemSeal,
	eLog_ShopItemUnseal,
	eLog_ShopItemUnseal_Failed,
	eLog_ShopItem_ReinforceReset,
	eLog_ShopItemGetCheat,

	eLog_ShopItemUse_MixSuccess=1600,

	// 080602 LUJ, 스킬 사용 시 소모
	eLog_SkillItemUse	= 1700,
	eLog_SkillMoneyUse,
	
	eLog_Item = 2000,
	eLog_ItemOptionAdd_RM,
	eLog_ItemOptionRemove_RM,
	eLog_ItemAdd,
	eLog_ItemRemove,
	// desc_hseos_결혼_01
	// S 결혼 추가 added by hseos 2008.01.29
	eLog_ItemRemoveMarriageDivorce,
	// 081027 LUJ, 드롭 옵션 적용
	eLog_ItemDropOptionAdd,
		
	eLog_ItemMoveInvenToPetInven = 2100,
	eLog_ItemMovePetInvenToInven,

	eLog_ItemSkillLearn	= 2200,

	eLog_GTournamentReg = 2300,		// 길드토너먼트 등록비
	// 081205 LUJ, 농장
	eLog_FarmTaxPaid = 2400,

	eLog_Max,
};


enum eGuildLog
{
	// member
	eGuildLog_MemberSecede = 1,
	eGuildLog_MemberBan,
	eGuildLog_MemberAdd,
	eGuildLog_StudentAdd,

	// master 
	eGuildLog_MasterChangeRank = 100,

	// guild
	eLogGuild_GuildCreate = 200,
	eGuildLog_GuildBreakUp,
	eGuildLog_GuildLevelUp,
	eGuildLog_GuildLevelDown,
	eGuildLog_Retire,
	eGuildLog_SkillAdd,

	// guildunion
	eGuildLog_CreateUnion = 400,
	eGuildLog_DestroyUnion,
	eGuildLog_AddUnion,
	eGuildLog_RemoveUnion,	
	eGuildLog_SecedeUnion,

	// 점수
	eGuildLog_Score	= 500,
	eGuildLog_TimeScoreAdd,
	eGuildLog_ScoreAddByKill,		// 길드전 상대 제거
	eGuildLog_ScoreRemoveByMurder,	// 길드전에서 사망
	eGuildLog_ScoreAddByWin,		// 길드전 승리
	eGuildLog_ScoreRemoveByDefeat,	// 길드전 패배
	eGuildLog_ScoreCheat,			// 치트
	eGuildLog_ScoreLevelUp,			// 레벨업에 사용
	eGuildLog_ScoreSkillUse,		// 080602 LUJ, 스킬 발동에 소모

	// gm 툴
	eGuildLog_GM	= 600,
	eGuildLog_GM_ChangeRank,
	eGuildLog_GM_KIck_Member,
};


enum eQuestLog
{
	eQuestLog_None	= 0,

	eQuestLog_GM	= 1,

	eQuestLog_User	= 100,
	eQuestLog_Main_Begin,
	eQuestLog_Sub_Begin,
	eQuestLog_Main_End,
	eQuestLog_Sub_End,
	eQuestLog_Main_Delete,
	eQuestLog_Sub_Delete,
	eQuestLOg_ItemTake,
	eQuestLOg_ItemGive,
	eQuestLOg_ItemUpdate,
};


enum eLogExppoint
{
	eExpLog_Time,
		
	eExpLog_LevelUp = 10,

	eExpLog_LosebyBadFame = 50, //reduce 
	eExpLog_LosebyReviveLogIn,
	eExpLog_LosebyRevivePresent,
	eExpLog_LosebyLooting,
	eExpLog_LosebyBreakupGuild,
	eExpLog_LosebyReviveExpProtect,
	eExpLog_LosebyReviveReviveOther,
	eExpLog_LosebyGuildFieldWar,
	eExpLog_LosebyBreakupFamily,
	// desc_hseos_패밀리01
	// S 패밀리 추가 added by hseos 2007.10.11
	eExpLog_LosebyLeaveFamily,
	eExpLog_LosebyExpleFamilyMember,
	// E 패밀리 추가 added by hseos 2007.10.11
	eExpLog_LoseByPKMode,
	// 080602 LUJ, 스킬 사용으로 인한 경험치 소모
	eExpLog_LoseBySkillUse,

	eExpLog_GetbyQuest = 100,
	eExpLog_SExpGetbyQuest = 101,
	
	eExpLog_ProtectExp = 150,
	eExpLog_ReviveExp,
};

enum eLogToolType	// Tool A¾·u
{
	eToolLog_GM = 1,
	eToolLog_RM,
	eToolLog_Server,
};

enum eGMLogKind		// GMTool ·I±× A¾·u
{
	eGMLog_LogIn = 1,
	eGMLog_Move,	
	eGMLog_Item,
	eGMLog_Money,
	eGMLog_Summon,
	eGMLog_SummonDelete,
	eGMLog_MonsterKill,
	eGMLog_PKAllow,
	eGMLog_Disconnect_Map,
	eGMLog_Disconnect_User,
	eGMLog_Block,
	eGMLog_Event,
	eGMLog_Jackpot,
	eGMLog_PlusTime,

	eGMLog_Skill,
	eGMLog_Ability,
	eGMLog_Chat_Map,
	eGMLog_Chat_User,
	eGMLog_PK,
	eGMLog_Regen,
};

enum eFishingLog
{
	eFishingLog_SetPointFromCheat = 1,	// 치트로부터 물고기포인트 설정
	eFishingLog_GetPointFromFish,		// 물고기로부터 물고기포인트 획득
	eFishingLog_UsePointForBuyItem,		// 아이템을 사기위해 물고기포인트 사용
	eFishingLog_SetLevel,				// 080808 LUJ, 낚시 레벨 변경
	eFishingLog_Regular,				// 080808 LUJ, 정기 로그 저장
};

enum eRMLogKind		// RMTool ·I±× A¾·u
{
	eRMLog_OperInsert = 1,
	eRMLog_OperDelete,
	eRMLog_IpInsert,
	eRMLog_IpDelete,
	/// 060829 PKH - gm툴 허용아이피 등록
	eRMLog_GMIpInsert,
	/// 060829 PKH - gm툴 허용 아아피 삭제
	eRMLog_GMIpDelete,

	eRMLog_ChangeCharacterInfo = 100,
	eRMLog_ChangeWareInfo,
	eRMLog_SkillInsert,
	eRMLog_SkillDelete,
	eRMLog_SkillUpdate,

	eRMLog_AbilityUpdate,

	eRMLog_QuestDelete,
	
	eRMLog_ChangeLoginPoint,
	
	eRMLog_ChangeItemPosition,
	eRMLog_ChangeWareItemPosition,
	eRMLog_ChangeSkillPosition,

	eRMLog_ItemInsert = 200,
	eRMLog_ItemDelete,
	eRMLog_ItemUpdate,	

	eRMLog_ChangeUserLevel = 300,
	eRMLog_RecoverCharacter,

	eRMLog_ChangePetInfo = 400,			//06.08.04 - pkh [펫]rm툴로그 기록
};


// 080716 LUJ, 펫 로그
enum ePetLog
{
	ePetLogSummon,
	ePetLogDie,
	ePetLogRevive,
	ePetLogChangeGrade,
	ePetLogChangeLevel,
	ePetLogRegular,		// 080716 LUJ, 일정 간격으로 상태 저장
};


// 길드토너먼트 로그
enum eGTLogKind
{
	eGTLogKind_None=0,
	eGTLogKind_Regist,				// 등록
	eGTLogKind_Win,					// 승
	eGTLogKind_Lose,				// 패
};


// 공성전 로그
enum eSWLogKind
{
	eSWLogKind_None=0,
	eSWLogKind_DefenceProposal,			// 수성참여 신청
	eSWLogKind_Attack,					// 공성문파
	eSWLogKind_Defence,					// 수성측 문파
	eSWLogKind_DefenceAccept,			// 수성참여 허락으로 수성문파 참여
	eSWLogKind_DefenceUnion,			// 동맹결성으로 수성문파 참여
	eSWLogKind_CancelByCastleGuild,		// 성문파가 동맹탈퇴로 다른성문파의 수성참여 취소
	eSWLogKind_CancelByUnionSecession,	// 성문파와의 동맹 탈퇴로 수성참여 취소
	eSWLogKind_StartSiegeWar,			// 공성시작
	eSWLogKind_SucceedCastleGuild,		// 공성성공으로 성을 차지한 문파
	eSWLogKind_SucceedMoveToDefence,	// 공성성공으로 성을 차지한 문파의 동맹은 수성으로 이동
	eSWLogKind_SucceedAttack,			// 공성성공 공성은 공성 그대로인 문파
	eSWLogKind_SucceedMoveToAttack,		// 공성측의 공성성공으로 수성->공성으로 옮겨간 문파
	eSWLogKind_EndCastleGuild,			// 공성종료후 성을 차지한 문파
	eSWLogKind_EndDefenceUnionGuild,	// 공성종료후 동맹이어서 수성으로 등록되는 문파
	eSWLogKind_EngraveSyn,				// 각인시도
	eSWLogKind_EngraveNack,				// 각인실패 맞아 죽음
	eSWLogKind_EngraveNackCancel,		// 각인실패 취소
	eSWLogKind_EngraveNackLogOut,		// 각인실패 로그아웃
	eSWLogKind_SiegeWarSucceed,			// 각인성공
	eSWLogKind_EndSiegeWar,				// 공성종료
};


// AuⓒøI
#define MAX_JOURNAL_ITEM			100
#define JOURNALVIEW_PER_PAGE		5

enum 
{
	eJournal_Quest,
	eJournal_Wanted,
	eJournal_Levelup,
	
	eJournal_Update,
	eJournal_Delete,
};
enum
{
	eJournal_Wanted_Doing,
	eJournal_Wanted_Succeed,
	eJournal_Wanted_FailbyOther, // other player completed
	eJournal_Wanted_FailbyDelChr, //wanted player is deleted
		
	eJournal_MurderedbyChr, //murder by PK Player
	eJournal_Wanted_FailbyBeWantedChr, // player is registed wanted
	eJournal_Wanted_FailbyTime, // destroy wanted info by time 
	eJournal_Wanted_FailbyDie, // die by wanted player cause wanted right can use one time.
};


// 짝짰짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짱
// 짝짯횁횜쨔짰쩌짯 쩐횈?횑횇횤  RaMa - 04.11.09                            짝짯
// 짝짹짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝짭짝째
enum eShopItemIncantation
{
	eIncantation_mugiup,
	eIncantation_armorup,
	eIncantation_mapmove,
	eIncantation_backhome,
	eIncantation_recovery,
};


//-----------------------------------------------------------------------
// PreLoadData
//-----------------------------------------------------------------------

#define MAX_KIND_PERMAP		150

enum
{
	ePreLoad_Monster,
	ePreLoad_Item,

	ePreLoad_Max,
};


struct sPRELOAD_INFO
{
	int		MapNum;
	int		Count[ePreLoad_Max];
	int		Kind[ePreLoad_Max][MAX_KIND_PERMAP];

	sPRELOAD_INFO()
	{
		MapNum		= 0;
		memset(Count, 0, sizeof(int)*ePreLoad_Max);
		memset(Kind, 0, sizeof(int)*ePreLoad_Max*MAX_KIND_PERMAP);
	}
};

#define BAIL_PRICE				1000
#define MIN_BADFAME_FOR_BAIL	100

#define MAX_ALLOWANCE	10
#define MAX_GUILD_NAME 16
#define MAX_GUILD_INTRO		60
// 06. 03. 문파공지 - 이영준
#define MAX_GUILD_NOTICE	150
#define MAX_GUILD_UNION		7
#define MAX_GUILD_NICKNAME	16

#define MAX_MASTERNAME_LENGTH	32

#define GUILD_MASTER		60
#define GUILD_VICEMASTER	50
#define GUILD_SENIOR		40
#define GUILD_JUNIOR		30
#define GUILD_MEMBER		20
#define GUILD_STUDENT		10
#define GUILD_NOTMEMBER		0

#define GUILD_STUDENT_MAX_LEVEL	40

//#define GUILD_1LEVEL		1
//#define GUILD_2LEVEL		2
//#define GUILD_3LEVEL		3
//#define GUILD_4LEVEL		4
//#define GUILD_5LEVEL		5

//#define MAX_GUILD_LEVEL		5
//#define MAX_GUILD_STEP		5

//#define GUILD_CREATELEVEL	15

#define MAX_GUILD_MEMBER	80 //60 + 20(수련생)

#define MAX_FAMILY_NAME 16
//#define MAX_FAMILY_INTRO		60
// 06. 03. 문파공지 - 이영준
//#define MAX_FAMILY_NOTICE	150
//#define MAX_FAMILY_UNION		7
//#define MAX_FAMILY_NICKNAME	16

#define MAX_MASTERNAME_LENGTH	32

#define FAMILY_MASTER		50
#define FAMILY_VICEMASTER	40
#define FAMILY_SENIOR		30
#define FAMILY_MEMBER		20
#define FAMILY_STUDENT		10
#define FAMILY_NOTMEMBER		0
//
//#define FAMILY_1LEVEL		1
//#define FAMILY_2LEVEL		2
//#define FAMILY_3LEVEL		3
//#define FAMILY_4LEVEL		4
//#define FAMILY_5LEVEL		5
//
#define MAX_FAMILY_LEVEL		5
//#define MAX_FAMILY_STEP		5
//
//#define FAMILY_CREATELEVEL	15

//#define MAX_FAMILYMEMBER_NUM 30
//#define MAX_FAMILYMEMBER_NUM 55 //30 + 25(국내문하생)

enum
{
	eShopItem_PeneltyCountZero,
};


//-----------------------------------------------------------------------------------
// Avatar Item
//-----------------------------------------------------------------------------------
enum
{
	eAvatar_Hat=0,			// 모자
	eAvatar_Hair,			// 머리
	eAvatar_Face,			// 얼굴
	eAvatar_Mask,			// 가면
	eAvatar_Glasses,		// 안경
	eAvatar_Mustache,		// 수염
	eAvatar_Dress,			// 옷
	eAvatar_Shoulder,		// 어깨
	eAvatar_Back,			// 등
	eAvatar_Shoes,			// 신발
	eAvatar_Effect,			// 이펙트
	eAvatar_Hand,			// 손

	eAvatar_Weared_Hair,	// 입는 아이템 머리
	eAvatar_Weared_Face,	// 입는 아이템 얼굴
	eAvatar_Weared_Hat,		// 입는 아이템 머리띠	
	eAvatar_Weared_Dress,	// 입는 아이템 옷
	eAvatar_Weared_Shoes,	// 입는 아이템 신발	
	eAvatar_Weared_Gum,		// 착용무기 검
	eAvatar_Weared_Gwun,	// 착용무기 권
	eAvatar_Weared_Do,		// 착용무기 도
	eAvatar_Weared_Chang,	// 착용무기 창
	eAvatar_Weared_Gung,	// 착용무기 궁
	eAvatar_Weared_Amgi,	// 착용무기 암기

	eAvatar_Max,
};

enum
{
	eAvatar_NotUse=0,
	eAvatar_Use,	
};

enum
{
	eAvatar_Attach_Head,
	eAvatar_Change_Dress,
	
};



enum
{
	eIncantation_TownMove			= 55301,		// 마을이동
	eIncantation_MemoryMove			= 55302,		// 기억이동
	eIncantation_TownMove15			= 55303,		// 마을이동15일
	eIncantation_MemoryMove15		= 55304,		// 기억이동15일
	eIncantation_TownMove7			= 57508,		// 마을이동7일
	eIncantation_TownMove7_NoTrade	= 57509,		// 마을이동7일 교환불가
	eIncantation_MemoryMove7		= 57510,		// 기억이동7일
	eIncantation_MemoryMove7_NoTrade = 57511,		// 기억이동7일 교환불가
	eIncantation_MoneyProtect		= 55311,		// 돈의 축복
	eIncantation_ExpProtect			= 55312,		// 생명의 축복
	eIncantation_ProtectAll			= 55313,		// 수호자의 주문서
	eIncantation_StatePoint			= 55321,		// 스탯포인트 재분배
	eIncantation_MixUp				= 55322,		// 조합확률 향상
	eIncantation_OtherRevive		= 55331,		// 부활	
	eIncantation_MemoryStone		= 55601,		// 기억석
//	eIncantation_SkPointRedist_2	= 55322,
	eIncantation_SkPointRedist_4	= 55323,
	eIncantation_SkPointRedist		= 55300,	
	eIncantation_ShowStorage			= 55351,		// 창고소환 주문서15일
	eIncantation_ShowStorage7		= 57506,		// 창고소환 주문서7일
	eIncantation_ShowStorage7_NoTrade = 57507,		// 창고소환 주문서7일 교환불가
	eIncantation_ChangeName			= 55352,		// 이름변경
	eIncantation_Tracking			= 55353,		// 현상범 추적15일
	eIncantation_Tracking7			= 57504,		// 현상범 추적7일
	eIncantation_Tracking7_NoTrade	= 57505,		// 현상범 추적7일 교환불가
	eIncantation_Tracking_Jin 		= 55387,		// 진추적주문서
	eIncantation_CharChange			= 55354,		// 성별전환주문서
	eIncantation_ShapeChange		= 55355,
	eIncantation_ItemSeal			= 55356,		// 재봉인 주문서
	eIncantation_ProtectAll15		= 55358,		// 수호자의 주문서(15일)
	eIncantation_ChangeJob			= 55360,		// 화경/극마 전직 주문서
	eIncantation_SkillExtend		= 55361,		// 무공창 확장 주문서
	eIncantation_ReinforceReset		= 55364,		// 강화초기회 주문서
	eIncantation_MemoryMoveExtend	= 55365,		// 기억창확장 이동주문서
	eIncantation_MemoryMove2		= 55371,		// 기억이동 2일
	eIncantation_ProtectAll1		= 55372,		// 수호자 1일

	eIncantation_PetRevival			= 55382,		// 펫 부활 주문서
	eIncantation_PetGrade2 			= 55384,		// 펫 성장 주문서 2단계
	eIncantation_PetGrade3 			= 55385,		// 펫 성장 주문서 3단계
	eIncantation_StorageExtend 		= 57544,		// 창고확장주문서	
	eIncantation_CharacterSlot 		= 57543,		// 캐릭터 추가 슬롯
	eIncantation_InvenExtend 		= 57542,		// 인벤토리확장 주문서
	eIncantation_ChangeName_Dntrade = 55352,		// 이름변경(거래안되는)
	eSundries_Boom_Butterfly		= 55621,		// 폭죽(나비)
	eSundries_Boom_Star				= 55622,		// 폭죽(별)
	eSundries_Shout					= 55631,		// 외치기
	eSundries_Shout_Once			= 55632,		// 외치기1회
	eSundries_Shout_Once_NoTrade	= 57593,		// 외치기1회 거래불가
	eSundries_RareItemCreate50		= 55640,
	eSundries_RareItemCreate70		= 55641,
	eSundries_RareItemCreate90		= 55642,
};


enum
{
	eShopItemUseParam_Realtime = 1,				// 현재시간
	eShopItemUseParam_Playtime = 2,				// 플레이시간
	eShopItemUseParam_Continue = 3,				// 사용해 놓고 나중에 효과가 발동되는것
	eShopItemUseParam_Forever = 4,				// 영구 아이템

	eShopItemUseParam_EquipAvatar = 10,			// 사용중인 아바타 아이템
};


enum
{
	eStateItem_NotUsed,
	eStateItem_Able,
	eStateItem_UnAble,
};


enum
{
	eShopItemErr_Revive_Fail=1,
	eShopItemErr_Revive_NotDead,
	eShopItemErr_Revive_NotUse,
	eShopItemErr_Revive_Refuse,
	eShopItemErr_Revive_TooFar,
	eShopItemErr_Revive_TimeOver,	
	eShopItemErr_Revive_NotReady,
};


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//┃Don't Duplication shopItem									  ┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
enum
{
	eShopItemDupType_None=0,
	eShopItemDupType_Charm,
	eShopItemDupType_Herb,
	eShopItemDupType_Incantation,
	eShopItemDupType_Sundries,    
};

enum DONTDUP_INCANTATION
{
	eDontDupUse_MemoryMove		= 2,
	eDontDupUse_ProtectAll		= 4,
	eDontDupUse_LevelCancel50	= 8,
	eDontDupUse_LevelCancel70	= 16,
	eDontDupUse_LevelCancel90	= 32,
	eDontDupUse_ShowStorage		= 64,
	eDontDupUse_Chase			= 128,
	eDontDupUse_TownMove		= 256,

	/*
	eDontDupUse_MemoryMove		= 0x00000001,		// 1
	eDontDupUse_ProtectAll		= 0x00000010,		// 16
	eDontDupUse_LevelCancel50	= 0x00000100,		// 256
	eDontDupUse_LevelCancel70	= 0x00001000,		// 4096
	eDontDupUse_LevelCancel90	= 0x00010000,		// 65536
	eDontDupUse_ShowStorage		= 0x00100000,		// 262144
	*/
};

enum DONTDUP_CHARM
{
	eDontDupUse_NaegongDamage	= 2,
	eDontDupUse_WoigongDamage	= 4,
	eDontDupUse_Exppoint		= 8,
	eDontDupUse_Reinforce		= 16,
};

enum DONTDUP_HERB
{

};

enum DONTDUP_SUNDRIES
{
	eDontDupUse_StreeStall = 2,
};


//#define SLOT_SHOPITEM_IMSI		90
#define SLOT_SHOPITEM_IMSI		36		// 071126 KTH
#define REVIVE_OTHER_DIST		1000



enum
{
	eMode_NormalNote = 0,
	eMode_PresentNote,
};



#define MAX_SHOUT_LENGTH	60




//┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//┃Guild Tournament									┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛
#define LIMITMEMBER_GUILDTOURNAMENT		5
#define LIMITLEVEL_GUILDTOURNAMENT		2
#define MAXGUILD_INTOURNAMENT			32

enum
{
	eGTError_NOERROR=0,					// No Error
	eGTError_NOTREGISTDAY,				// Not a Regist-day
	eGTError_NOTREGISTCANCELDAY,		// Not a Registcancel-day
	eGTError_DONTFINDGUILDINFO,			// Don't find a GuildInfo
	eGTError_MAXGUILDOVER,				// Over MAXGUILD_INTOURNAMENT
	eGTError_ALREADYREGISTE,			// Already Registed GuildTournament
	eGTError_ALREADYCANCELREGISTE,		// Already Cancel Registed GuildTournament
	eGTError_UNDERLIMITEMEMBER,			// Under LIMITMEMBER_GUILDTOURNAMENT
	eGTError_UNDERLEVEL,				// Under LIMITLEVEL_GUILDTOURNAMENT
	eGTError_DELETEAPPLIED,				// CancelError Regited GuildTournament
	eGTError_NOGUILDMASTER,				// Not a GuildMaster
	eGTError_DONTFINDPOSITION,			// GuildTournament-Position Find Error
	eGTError_DONTFINDBATTLE,			// Don't find a Battle	
	eGTError_BATTLEJOINFAIL,			// Failed a join in Battle
	eGTError_NOTENTERTIME,				// Not a time of enter the battle;
	eGTError_STUDENTCANTENTER,			// a guild student can't enter the battle
	eGTError_ERROR,
};

enum
{
	eGTState_BeforeRegist=0,
	eGTState_Regist,
	eGTState_BeforeEntrance,
	eGTState_Entrance,
	eGTState_Process,
	eGTState_Leave,
};

enum
{
	eGTStanding_1ST = 1,
	eGTStanding_2ND = 2,
	eGTStanding_3RD = 3,
	eGTStanding_4TH = 4,
	eGTStanding_8TH = 8,
	eGTStanding_16TH = 16,
	eGTStanding_32TH = 32,
};

enum
{
	eGTFight_32		= 0,
	eGTFight_16		= 1,
	eGTFight_8		= 2,
	eGTFight_4		= 3,
	eGTFight_3_4	= 4,
	eGTFight_1_2	= 5,

	eGTT_MAX,

	eGTFight_End,
};

enum
{
	eGTDay_SUNDAY=0,
	eGTDay_MONDAY,
	eGTDay_TUESDAY,
	eGTDay_WEDNESDAY,
	eGTDay_THURSDAY,
	eGTDay_FRIDAY,
	eGTDay_SATURDAY,
};

enum
{
	eGTWin_None = 0,
	eGTWin_Unearned,		// 부전승
	eGTWin_RemainPlayer,	// 남은사람수
	eGTWin_LowLevel,		// 레벨합이 낮다
	eGTWin_LowExp,			// 경험치가 낮다.
};


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//┃SiegeWar													  ┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
#define MAX_CASTLEGATE_NUM		20



//┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//┃Npcscript Param										┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛
enum
{
	eNpcParam_ShowpStorage=1,
};




//┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//┃Item Parameter										┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛
#define ITEM_PARAM_SEAL		0x00000001
#define ITEM_PARAM_CHEAT	0x00000010
#define ITEM_PARAM_PRESENT	0x00000100



// 071119 LYW --- CommonGameDefine : 새로운 아이템 Parameter 추가.
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//┃New Item Parameter									┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛
enum
{
	eITEM_PARAM_NORMAL	= 0
,	eITEM_PARAM_SEAL
,	eITEM_PARAM_UNSEAL
//,	eITEM_PARAM_CHEAT
//,	eITEM_PARAM_PRESENT
} ;


//-----------------------------------------------------------------------------------
// Move Dialog
//-----------------------------------------------------------------------------------
#define MAX_MOVEDATA_PERPAGE		10
#define MAX_MOVEPOINT_PAGE			2
#define MAX_SAVEDMOVE_NAME			21





//-----------------------------------------------------------------------------------
// Item Parameter
//-----------------------------------------------------------------------------------
#define ITEMPARAM_ISLOCK(Param)					( (DWORD)Param>>31	)		// 2147483648
#define ITEMPARAM_SETLOCK(Param, Option)		( Param = ((Param<<1)|(Option<<31)))

// 070414 LYW --- CommonGameDefine : Modified eCharacterStage.
enum eCharacterStage
{
	eStage_H_Fighter			= 1111,	// 파이터
	eStage_H_Guard				= 1121,	// 가드
	eStage_H_Warrior			= 1122,	// 전사
	eStage_H_InfantryMan		= 1131,	// 보병
	eStage_H_SwordMan			= 1132,	// 소드맨
	eStage_H_Mercenary			= 1133,	// 용병
	eStage_H_Phalanx			= 1141, // 팔랑크스
	eStage_H_Knight				= 1142, // 기사
	eStage_H_Gladiator			= 1143, // 검투사
	eStage_H_RunicKnight		= 1144, // 룬 나이트
	eStage_H_Paladin			= 1151, // 팰러딘
	eStage_H_Panzer				= 1152, // 기갑부대원
	eStage_H_Crusader			= 1153, // 십자군전사
	eStage_H_Destroyer			= 1154, // 파괴자
	eStage_H_SwordMaster		= 1155, // 소드마스터
	eStage_H_Magners			= 1156, // 매그너스
	eStage_H_Lord				= 1161, // 로드
	eStage_H_DeathKNight		= 1162, // 데스나이트
	eStage_H_ArcTemplar			= 1163, // 아크템플러

	eStage_E_Fighter			= 1211,	// 파이터
	eStage_E_Guard				= 1221,	// 가드
	eStage_E_Warrior			= 1222,	// 전사
	eStage_E_InfantryMan		= 1231,	// 보병
	eStage_E_SwordMan			= 1232,	// 소드맨
	eStage_E_Mercenary			= 1233,	// 용병
	eStage_E_Phalanx			= 1241, // 팔랑크스
	eStage_E_Knight				= 1242, // 기사
	eStage_E_Gladiator			= 1243, // 검투사
	eStage_E_RunicKnight		= 1244, // 룬 나이트
	eStage_E_Paladin			= 1251, // 팰러딘
	eStage_E_Panzer				= 1252, // 기갑부대원
	eStage_E_Crusader			= 1253, // 십자군전사
	eStage_E_Destroyer			= 1254, // 파괴자
	eStage_E_SwordMaster		= 1255, // 소드마스터
	eStage_E_Magners			= 1256, // 매그너스
	eStage_E_Lord				= 1261, // 로드
	eStage_E_DeathKNight		= 1262, // 데스나이트
	eStage_E_ArcTemplar			= 1263, // 아크템플러
	

	eStage_H_Rogue				= 2111, // 방랑자
	eStage_H_Voyager			= 2121, // 항해자
	eStage_H_Ruffian			= 2122, // 무법자
	eStage_H_Archer				= 2131, // 궁수
	eStage_H_Chief				= 2132, // 시프
	eStage_H_Scout				= 2133, // 정찰병
	eStage_H_Ranger				= 2141, // 부대원
	eStage_H_TreasureHunter 	= 2142, // 보물사냥꾼
	eStage_H_Assassin			= 2143, // 암살자
	eStage_H_RunicWorker		= 2144, // 룬워커
	eStage_H_Sniper				= 2151, // 저격병
	eStage_H_Intraper			= 2152, // 
	eStage_H_BladeTaker			= 2153, //
	eStage_H_TemperMaster		= 2154, //

	// 080709 LYW --- CommonGameDefine : 아크레인저 직업을 추가한다.
	eState_H_ArcRanger			= 2155,	// 아크레인저

	eStage_H_SoulEye			= 2161, //
	eStage_H_BloodStalker		= 2162, //
	eStage_H_ArcBridger			= 2163, //

	eStage_E_Rogue				= 2211, // 방랑자
	eStage_E_Voyager			= 2221, // 항해자
	eStage_E_Ruffian			= 2222, // 무법자
	eStage_E_Archer				= 2231, // 궁수
	eStage_E_Chief				= 2232, // 시프
	eStage_E_Scout				= 2233, // 정찰병
	eStage_E_Ranger				= 2241, // 부대원
	eStage_E_TreasureHunter 	= 2242, // 보물사냥꾼
	eStage_E_Assassin			= 2243, // 암살자
	eStage_E_RunicWorker		= 2244, // 룬워커
	eStage_E_Sniper				= 2251, // 저격병		
	eStage_E_Intraper			= 2252, // 
	eStage_E_BladeTaker			= 2253, //
	eStage_E_TemperMaster		= 2254, //

	// 080709 LYW --- CommonGameDefine : 아크레인저 직업을 추가한다.
	eState_E_ArcRanger			= 2255,	// 아크레인저

	eStage_E_SoulEye			= 2261, //
	eStage_E_BloodStalker		= 2262, //
	eStage_E_ArcBridger			= 2263, //

	eStage_H_Mage				= 3111, //
	eStage_H_Cleric				= 3121, //
	eStage_H_Wiard				= 3122, //
	eStage_H_Priest				= 3131, //
	eStage_H_Sorcerer			= 3132, //
	eStage_H_Monk				= 3133, //
	eStage_H_Bishop				= 3141, //
	eStage_H_Warlock			= 3142, //
	eStage_H_Inquirer			= 3143, //
	eStage_H_ElementalMaster	= 3144, //
	eStage_H_Cardinal			= 3151, //
	eStage_H_SoulArbiter		= 3152, //
	eStage_H_GrandMaster		= 3153, //
	eStage_H_Necromancer		= 3154, //
	eStage_H_RunicMaster		= 3155, //
	eStage_H_Saint				= 3161, //
	eStage_H_DarkArchon			= 3162, //
	eStage_H_ArcCelebrant		= 3163, //

	eStage_E_Mage				= 3211, //
	eStage_E_Cleric				= 3221, //
	eStage_E_Wiard				= 3222, //
	eStage_E_Priest				= 3231, //
	eStage_E_Sorcerer			= 3232, //
	eStage_E_Monk				= 3233, //
	eStage_E_Bishop				= 3241, //
	eStage_E_Warlock			= 3242, //
	eStage_E_Inquirer			= 3243, //
	eStage_E_ElementalMaster	= 3244, //
	eStage_E_Cardinal			= 3251, //
	eStage_E_SoulArbiter		= 3252, //
	eStage_E_GrandMaster		= 3253, //
	eStage_E_Necromancer		= 3254, //
	eStage_E_RunicMaster		= 3255, //
	eStage_E_Saint				= 3261, //
	eStage_E_DarkArchon			= 3262, //
	eStage_E_ArcCelebrant		= 3263, //

};

/*
enum eCharacterStage
{
	eStage_Normal	= 0,	// 일반
	eStage_Hwa		= 64,	// 현경	  		
	eStage_Hyun		= 65,	// 현경	  		
	eStage_Geuk		= 128,	// 극마	  		
	eStage_Tal		= 129,	// 탈마	  		
};

enum eItemStage
{
	eItemStage_Normal		= 0,
	eItemStage_ChangeStage	= 32,
	///////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	eItemStage_ChangeStage2	= 33,
	///////////////////////////////
	eItemStage_OverHwa		= 64,
	eItemStage_Hwa			= 65,
	eItemStage_Hyun			= 66,
	eItemStage_OverGeuk		= 128,
	eItemStage_Geuk			= 129,
	eItemStage_Tal			= 130,
};
*/
// guildunion
enum eGuildUnion_Err
{
	eGU_Not_ValidName,
	eGU_Not_InGuild,
	eGU_Not_GuildMaster,
	eGU_Not_Level,
	eGU_Not_GuildUnionMaster,
	eGU_Not_InGuildUnion,
	eGU_Aleady_InGuildUnion,
	eGU_Time,
	eGU_Other_Not_InGuild,
	eGU_Other_Not_InGuildUnion,
	eGU_No_Lone_Union,
	eGU_Other_Not_GuildMaster,
	eGU_Other_Aleady_InGuildUnion,
	eGU_Other_Time,
	eGU_Is_Full,	
	eGU_Is_GuildFieldWar,
	eGU_Not_ValidMap,
};

enum eFamilyUnion_Err
{
	eFU_Not_ValidName,
	eFU_Not_InFamily,
	eFU_Not_FamilyMaster,
	eFU_Not_Level,
	eFU_Not_FamilyUnionMaster,
	eFU_Not_InFamilyUnion,
	eFU_Aleady_InFamilyUnion,
	eFU_Time,
	eFU_Other_Not_InFamily,
	eFU_Other_Not_InFamilyUnion,
	eFU_Other_Not_FamilyMaster,
	eFU_Other_Aleady_InFamilyUnion,
	eFU_Other_Time,
	eFU_Is_Full,	
	eFU_Is_FamilyFieldWar,
	eFU_Not_ValidMap,
};

enum eSWProfit_Err
{
	eSW_Not_Profit,
	eSW_Not_ProfitGuild,
	eSW_Not_ProfitGuildMaster,	
	eSW_No_TaxRate,
	eSW_No_Time,
	eSW_No_Money,
};


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//┃For Japan_Local										┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛
enum eCharacter_Attr	// 화->토->금->수->목
{
	eCA_None	= 0,
	eCA_Fire	= 1,		
	eCA_Earth	= 2,
	eCA_Iron	= 3,
	eCA_Water	= 4,
	eCA_Tree	= 5,	
};

enum eItem_Attr
{
	eIA_All			= 0,
	eIA_Fire		= 1,
	eIA_Earth		= 2,
	eIA_Iron		= 3,	
	eIA_Water		= 4,	
	eIA_Tree		= 5,	
	eIA_Fire_Only	= 11,
	eIA_Earth_Only	= 12,
	eIA_Iron_Only	= 13,
	eIA_Water_Only	= 14,
	eIA_Tree_Only	= 15,	
};

enum eCharacter_Attr_Relation
{
	eCAR_None				= -1,
	eCAR_Same				= 0,
	eCAR_Give_Help			= 1,	// 도움주는 상생
	eCAR_Strong_Opposite	= 2,	// 내가 강한 상극
	eCAR_Weak_Opposite		= 3,	// 내가 약점인 상극
	eCAR_Need_Help			= 4,	// 도움받는 상생	
};


#define GIVEN_CHARACTER_SLOT		2
#define GIVEN_INVENTORY_SLOT		2
#define GIVEN_STORAGE_SLOT			2
#define GIVEN_SKILL_SLOT			10
#define SKILL_SLOT_ADDCOUNT		5

#define EVENT_MONSTER_SANTA1			145			// 레벨 99 산타의요정
#define EVENT_MONSTER_SANTA2			160			// 레벨 15 산타의요정
#define EVENT_DAMAGE					2005		// 산타의요정을 눈으로 공격시 뜨는 데미지

#define EVENT_ITEM_GLOVE			50509			// 벙어리장갑	
#define EVENT_ITEM_SNOW				50508			// 눈덩이
#define EVENT_ITEM_FIRECRACKER		53111			// 폭죽

#define EVENT_EFFECT_FIRECRACKER	1036			// 폭죽 이펙트

#define EVENT_SHOPITEM_SNOWMAN_HAT			55572	// 아바타 아이템 눈사람(모자)
#define EVENT_SHOPITEM_SNOWMAN_DRESS		55573	// 아바타 아이템 눈사람(옷)
#define EVENT_SHOPITEM_RUDOLP_HAT			55574	// 아바타 아이템 루돌프(모자)
#define EVENT_SHOPITEM_RUDOLP_DRESS			55575	// 아바타 아이템 루돌프(옷)

#define EVENT_SHOPITEM_SNOWMAN_HAT2			55580	// 아바타 아이템 눈사람(모자) 시간제
#define EVENT_SHOPITEM_SNOWMAN_DRESS2		55581	// 아바타 아이템 눈사람(옷) 시간제
#define EVENT_SHOPITEM_RUDOLP_HAT2			55582	// 아바타 아이템 루돌프(모자) 시간제
#define EVENT_SHOPITEM_RUDOLP_DRESS2		55583	// 아바타 아이템 루돌프(옷) 시간제

// 기상상태
enum eWeatherState
{
	//eWS_None,	// 0
	//eWS_Snow,	// 1
	eWS_Clear,	// 맑음
	eWS_Cloudy,	// 흐림
	eWS_Rainy,	// 비
	eWS_Windy,	// 바람
	eWS_Foggy,	// 안개
	eWS_Typoon,	// 태풍

	eWS_Max,	
};

// 080410 NYJ --- 낚시관련 에러코드 
enum eFishingErrorCode
{
	eFishingError_None = 0,		// NotError, Success
	eFishingError_InvaildHero,	// 유효하지 않은 플레이어
	eFishingError_InvaildPlace,	// 유효하지 않은 낚시터
	eFishingError_NoPole,		// 낚시대 미착용
	eFishingError_NoBait,		// 미끼 미사용
	eFishingError_OverTime,		// 시간초과
	eFishingError_UserCancel,	// 유저의 취소요청
	eFishingError_InvenFull,	// 인벤이 가득찬상태
	eFishingError_InvalidGrade,	// 숙련등급 불일치
	eFishingError_EarlyTime,	// 시간미충족
	eFishingError_InvaildState,	// 유효하지 않은 상태
	eFishingError_OverMaxFishPoint,	// 최대 물고기포인트 초과
	eFishingError_FailFishPoint,// 물고기포인트변환 실패
	eFishingError_OverDistance,	// 거리초과
};

// 080414 NYJ --- 낚시관련 물고기아이템타입
enum eFishItemType
{
	eFishItem_Poor = 0,		// [0]피라미
	eFishItem_Common,		// [1]붕어
	eFishItem_UnCommon,		// [2]잉어
	eFishItem_Rare,			// [3]황금잉어, 황금자라
	eFishItem_Unique,		// [4]빛나는 쓰레기
	eFishItem_Event,		// [5]친환경미끼
	eFishItem_Reserve1,		// [6]예약1
	eFishItem_Reserve2,		// [7]예약1
	eFishItem_Reserve3,		// [8]예약1
	eFishItem_Reserve4,		// [9]예약1
	
	eFishItem_Max,
};

////////////////////////////////////////////////////////
//06. 06 2차 전직 - 이영준
//이펙트 생략(무초)
enum eSkipEffect
{
	eSkipEffect_None,
	eSkipEffect_Start,

	eSkipEffect_Max,
};
////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// 06. 06. 2차 전직 - 이영준
// 무공 변환 추가
#define MAX_SKILLOPTION_COUNT	8	// 최대 옵션 수

enum eSkillOption_Kind{
	
	eSkillOption_None,				// 옵션 없음

	eSkillOption_Range,				// 1. 사거리
	eSkillOption_ReduceMana,		// 2. 내력소모
	eSkillOption_PhyAtk,			// 3. 스킬물리공격
	eSkillOption_BaseAtk,			// 4. 일반물리공격
	eSkillOption_AttAtk,			// 5. 속성공격

	eSkillOption_Life,				// 6. 최대생명력
	eSkillOption_Mana,			// 7. 최대내력
	eSkillOption_PhyDef,			// 9. 물리방어
	eSkillOption_AttDef,			// 10. 속성방어

	eSkillOption_Duration,			// 11. 지속시간

	eSkillOption_Max,
};
//////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////
// 06. 06. 2차 전직 - 이영준
// 무공 변환
// 변환 유형 - 0: 변환불가, 1: 공격형, 2:보조형, 3:모두가능
enum eSkillChangeKind{
	
	eSkillChange_None,		// 변환불가
	eSkillChange_Offense,	// 공격형
	eSkillChange_Defense,	// 보조형
	eSkillChange_All,		// 모두 가능

	eSkillChange_Max,
};
////////////////////////////////////////////////////////

enum eGuildPlusTimeKindList
{
	eSuRyun_OneHalf,
	eSkill_OneHalf,
	eExp_OneHalf,
	eDamageUp_20Pct,
	eGuildPlusTime_Max,
};

enum eGuildPlustTimeKind
{
	//eGPT_None,
	eGPT_SuRyun,
	eGPT_Skill,
	eGPT_Exp,
	eGPT_DamageUp,
	eGPT_Kind_Max,
};

//struct GUILDUSING_PLUSTIME_INFO
//{
//	int PlusTimeIdx;
//	DWORD PlusTimeEndTime;
//};

//struct GUILDPOINT_INFO
//{
//	GUILDPOINT_INFO() {memset(this,0,sizeof(GUILDPOINT_INFO));}
//
//	//문파포인트
//	int GuildPoint;
//	//사냥몬스터점수
//	int GuildHuntedMonsterCount;
//	//사냥몬스터총점수
//	int GuildHuntedMonsterTotalCount;
//	//DB업데이트 시간
//	DWORD DBProcessTime;
//	//사용문파플러스타임
//	DWORD GuildPlusTimeflg;
//	//문파플러스타임 정보
//	GUILDUSING_PLUSTIME_INFO GuildUsingPlusTimeInfo[eGPT_Kind_Max];
//	//사용무공정보(예정)
//	//DWORD Guild
//};

enum PART_TYPE
{
	ePartType_None,
	ePartType_OneHand,
	ePartType_TwoHand,
	ePartType_TwoBlade,
	ePartType_Shield,

	ePartType_Body,
	ePartType_Hand = 7,
	ePartType_Foot,

	ePartType_Hair,
	ePartType_Face,
	ePartType_Head,

	ePartType_Helmet = 15,
	ePartType_Glasses,
	ePartType_Mask,

	ePartType_Head_Chicken = 20,
	ePartType_Head_Panda,
	ePartType_Head_Tights,
	ePartType_Head_Bag,
	ePartType_Head_Mask,

	ePartType_Costume_Body = 30,
	ePartType_Costume_Foot,
	ePartType_Costume_Hand,

	ePartType_Wing = 40,
	ePartType_Wing2,


	ePartType_Pet_Face = 70,
	ePartType_Pet_Head,
	ePartType_Pet_Wing,
	ePartType_Pet_Tail,
	
	ePartType_Max,
};

enum	DATE_MATCHING_REGION				// 데이트 매칭 시스템에서 사는 지역 선언. 로컬라이징 하려면 파일형식으로 밖으로 빼야할 듯.?
{
	eDATE_MATCHING_REGION_KYUNGKI,			// 경기,
	eDATE_MATCHING_REGION_SEOUL,			// 서울
	eDATE_MATCHING_REGION_INCHON,			// 인천
	eDATE_MATCHING_REGION_KANGWON,			// 강원
	eDATE_MATCHING_REGION_CHUNGNAM,			// 충남
	eDATE_MATCHING_REGION_CHUNGBUK,			// 충북
	eDATE_MATCHING_REGION_DAEJEON,			// 대전
	eDATE_MATCHING_REGION_KYUNGBUK,			// 경북
	eDATE_MATCHING_REGION_KYUNGNAM,			// 경남
	eDATE_MATCHING_REGION_DAEGU,			// 대구
	eDATE_MATCHING_REGION_BUSAN,			// 부산
	eDATE_MATCHING_REGION_ULSAN,			// 울산
	eDATE_MATCHING_REGION_JEONBUK,			// 전북
	eDATE_MATCHING_REGION_JEONNAM,			// 전남
	eDATE_MATCHING_REGION_KWANGJU,			// 광주
	eDATE_MATCHING_REGION_JEJU,				// 제주
	eDATE_MATCHING_REGION_OVERSEAS,			// 해외
};

enum	DATE_MATCHING_FEELING				// 데이트 매칭 시스템의 성향.
{
	eDATE_MATCHING_FEELING_BLACKHAIR,		// 검은 머리
	eDATE_MATCHING_FEELING_DYEDHAIR,			// 염색한 머리
	eDATE_MATCHING_FEELING_FORMALDRESS,		// 정장
	eDATE_MATCHING_FEELING_CASUALWEAR,		// 캐주얼 복장
	eDATE_MATCHING_FEELING_MAKEUP,			// 화장
	eDATE_MATCHING_FEELING_PERFUME,			// 향수
	eDATE_MATCHING_FEELING_THIN,				// 마름
	eDATE_MATCHING_FEELING_FAT,				// 뚱뚱함
	eDATE_MATCHING_FEELING_BEARD,			// 수염
	eDATE_MATCHING_FEELING_EYEGLASS,			// 안경
	eDATE_MATCHING_FEELING_HAT,				// 모자
	eDATE_MATCHING_FEELING_ACCESSORIES,		// 액세서리
};

enum eStatusKind
{
	eStatusKind_None,
	eStatusKind_Str,
	eStatusKind_Dex,
	eStatusKind_Vit,
	eStatusKind_Int,
	eStatusKind_Wis,
	eStatusKind_All,
	eStatusKind_PhysicAttack = 11,
	eStatusKind_PhysicDefense,
	eStatusKind_MagicAttack,
	eStatusKind_MagicDefense,
	eStatusKind_Accuracy,
	eStatusKind_Avoid,
	eStatusKind_CriticalRate,
	eStatusKind_Range,
	eStatusKind_MagicCriticalRate,
	eStatusKind_SkillRange = 21,
	eStatusKind_CriticalDamage,
	eStatusKind_MoveSpeed,
	eStatusKind_Block,
	eStatusKind_CoolTime,
	eStatusKind_CastingProtect,

	eStatusKind_Hide,
	eStatusKind_Detect,

	eStatusKind_IncreaseDropRate = 29,
	eStatusKind_DecreaseManaRate = 30,
	eStatusKind_MaxLife = 31,
	eStatusKind_MaxMana,
	eStatusKind_LifeRecoverRate,
	eStatusKind_ManaRecoverRate,
	eStatusKind_LifeRecover,
	eStatusKind_ManaRecover,
	eStatusKind_Poison = 40,
	eStatusKind_Paralysis,
	eStatusKind_Stun,
	eStatusKind_Slip,
	eStatusKind_Freezing,
	eStatusKind_Stone,
	eStatusKind_Silence,
	eStatusKind_BlockAttack,
	eStatusKind_God,
	eStatusKind_Shield,
	eStatusKind_Bleeding,
	eStatusKind_Burning,
	eStatusKind_HolyDamage,
	eStatusKind_MoveStop,
	eStatusKind_Attract,

	eStatusKind_Reflect = 61,
	eStatusKind_Absorb,
	eStatusKind_DamageToLife,
	eStatusKind_DamageToMana,
	eStatusKind_GetLife,
	eStatusKind_GetMana,
	eStatusKind_GetExp,
	eStatusKind_GetGold,
	// 071217 KTH --- 경험치 보호
	eStatusKind_ProtectExp = 70,
	eStatusKind_Attrib_None = 71,
	eStatusKind_Attrib_Earth,
	eStatusKind_Attrib_Water,
	eStatusKind_Attrib_Divine,
	eStatusKind_Attrib_Wind,
	eStatusKind_Attrib_Fire,
	eStatusKind_Attrib_Dark,
	eStatusKind_Sword = 81,
	eStatusKind_Mace,
	eStatusKind_Axe,
	eStatusKind_Staff,
	eStatusKind_Bow,
	eStatusKind_Gun,
	eStatusKind_Dagger,
	eStatusKind_Spear,
	eStatusKind_TwoHanded,
	eStatusKind_TwoBlade,
	eStatusKind_OneHanded,

	eStatusKind_RobeArmor,
	eStatusKind_LightArmor,
	eStatusKind_HeavyArmor,
	eStatusKind_ShieldArmor,
	
	eStatusKind_NormalSpeedRate = 100,
	eStatusKind_PhysicSkillSpeedRate,
	eStatusKind_MagicSkillSpeedRate,	
	eStatusKind_NormalPhysicSkillSpeedRate = 105,
	// 080616 LUJ, 5차 스킬 추가
	eStatusKind_ConvertHealthWithGivedDamage	= 106,
	eStatusKind_ConvertManaWithGivedDamage		= 107,
	eStatusKind_EventSkillToVictim				= 108,
	eStatusKind_EventSkillToKiller				= 109,
	// 080703 LUJ, 이도류 사용 가능
	eStatusKind_EnableTwoBlade					= 110,
	// 081203 LUJ, 스크립트 사용
	eStatusKind_Script							= 111,
	// 071204 KTH -- 경험치 증가 %
	eStatusKind_IncreaseExpRate = 120,

	eStatusKind_Max,
};

enum eSkillTargetKind
{
	eSkillTargetKind_None,
	eSkillTargetKind_OnlyOthers,
	eSkillTargetKind_Ground,
	eSkillTargetKind_OnlySelf,
	eSkillTargetKind_AllObject,
};

// 080602 LUJ, 스킬 타겟 대상. Target.h에서 옮겨옴
enum TARGET_KIND
{
	TARGET_KIND_SINGLE,
	TARGET_KIND_MULTI,
	TARGET_KIND_SINGLE_CIRCLE,
	TARGET_KIND_MULTI_CIRCLE,
	TARGET_KIND_MULTI_RECT,
	TARGET_KIND_MULTI_ARC,
	// 080602 LUJ, 세계 범위 스킬
	TARGET_KIND_WORLD,
	// 080708 LUJ, 몬스터 대상 범위 스킬
	TARGET_KIND_MULTI_MONSTER,
	TARGET_KIND_MAX,
};

// 080602 LUJ, 스킬 범위 유형 타겟. Target.h에서 옮겨옴
enum eSkillAreaTarget
{
	eSkillAreaTargetNone,
	eSkillAreaTargetMonster,
	eSkillAreaTargetParty,
	eSkillAreaTargetFamily,
	eSkillAreaTargetGuild,
	eSkillAreaTargetAll,
	eSkillAreaTargetAllExceptMe,
};

// 080602 LUJ, 스킬 결과. 클라이언트에 실패 이유를 나타내기 위해 [cc]skill/server/manager/skillManager.h에서 옮겨옴
enum SKILL_RESULT
{
	SKILL_SUCCESS = 0,
	SKILL_OPERATOR_INVALUED,
	SKILL_INFO_INVALUED,
	SKILL_LEVEL_INVALUED,
	SKILL_STATE_INVALUED,
	SKILL_TARGET_INVALUED,
	SKILL_RANGE_NOTENOUGH,
	SKILL_COOLING,
	SKILL_FAIL_ETC,
	SKILL_DESTROY,
};

enum eBuffSkillActiveRule
{
	eBuffSkillActiveRule_None,
	eBuffSkillActiveRule_Current_HP,
	eBuffSkillActiveRule_Current_MP,
	eBuffSkillActiveRule_Max_HP,
	eBuffSkillActiveRule_Max_MP,

	eBuffSkillActiveRule_Active = 0,
	eBuffSkillActiveRule_Delete,
	eBuffSkillActiveRule_Stop,
};

enum eBuffSkillCountType
{
	eBuffSkillCountType_None,
	eBuffSkillCountType_Dead,
	eBuffSkillCountType_AllDamage,
	eBuffSkillCountType_PhysicDamage,
	eBuffSkillCountType_MagicDamage,
	eBuffSkillCountType_HP_Potion,
	eBuffSkillCountType_MP_Potion,
	eBuffSkillCountType_Recover,
	eBuffSkillCountType_Dodge,
	eBuffSkillCountType_Block,
};

enum eBuffSkillWeapon
{
	eBuffSkillWeapon_None,
	eBuffSkillWeapon_Sword,
	eBuffSkillWeapon_Shield,
	eBuffSkillWeapon_Mace,
	eBuffSkillWeapon_Axe,
	eBuffSkillWeapon_Staff,
	eBuffSkillWeapon_Bow,
	eBuffSkillWeapon_Gun,
	eBuffSkillWeapon_Dagger,
	eBuffSkillWeapon_Spear,
	eBuffSkillWeapon_TwoHanded,
	eBuffSkillWeapon_OneHanded,
	eBuffSkillWeapon_TwoBlade,
};

enum eBuffSkillArmor
{
	eBuffSkillArmor_None,
	eBuffSkillArmor_Robe,
	eBuffSkillArmor_Leather,
	eBuffSkillArmor_Metal,
};


enum eAutoNoteError
{
	eAutoNoteError_None,
	eAutoNoteError_CantFind,
	eAutoNoteError_NotProperState,
	eAutoNoteError_AlreadyAsked,
	eAutoNoteError_AlreadyAsking,
	eAutoNoteError_CantUseMap,
};

enum ePetType
{
	ePetType_Basic,
	ePetType_Physic,
	ePetType_Magic,
	ePetType_Support,
	ePetType_Max,
};

enum ePetState
{
	ePetState_None,
	ePetState_Summon,
	ePetState_Die,
};
 
#endif //__COMMONGAMEDEFINE_H__
