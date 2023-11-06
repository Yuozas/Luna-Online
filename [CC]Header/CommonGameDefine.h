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

#define SPEEDHACK_CHECKTIME	60000	//1���ע������ע�E

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

	// ShopItem�� Icon�߰�
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
	//06. 06 2�� ���� - �̿���
	//����Ʈ ����(����)
	eStatusIcon_SkipEffect	= 133,
	////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////
	//06. 06 2�� ���� - �̿���
	//����/����
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
// desc_hseos_����Ʈ ��_01
// S ����Ʈ �� �߰� added by hseos 2007.11.29
#define CHALLENGERECALL_GROUP_ID	-4
// E ����Ʈ �� �߰� added by hseos 2007.11.29

#define ITEMRECALL_GROUP_ID	1001	// ���ͼ�ȯ ������ ���

enum eOPTION	//����u�ˢ硧I��Ioo��E?I ��Ii��E?AICI�ˢ硧��O ��E����AA��I��������oA��E?A.
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

enum eBATTLE_TEAM	//battle team enum �ˢ硧��u AeCOCIAU CONFIRM
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
	eBATTLE_KIND_NONE = 0,		// Player vs Monster ���סˢ�o����uOAC �ˢ�ic��I�Ϩ�ECO��EO�ˢ碮��AC Battle
	eBATTLE_KIND_MURIMFIELD,	// murim �ˢ硧��u munpa��E��IA ��E��IU��E����| �ˢ硧��A��EOo ��E?��Io�ˢ�io
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
// 080111 LYW --- CommonGameDefine : �α��� ���� ��й�ȣ ���� ���� ó�� �߰�.
LOGIN_ERROR_INVALID_SEC_PASS,				// ���� ��й�ȣ ������ �����ߴٴ� �����޽���.
LOGIN_ERROR_CHANGE_SEC_PASS,				// �߸��� ���� �����ȣ, Ȩ���������� ���� ��й�ȣ�� �� ��� �Ͻÿ�.
LOGIN_ERROR_FILE_CRC,
LOGIN_ERROR_LOGINSESSION_INVALID,
};

#define MHSHADOW_OFF	0
#define MHSHADOW_CIRCLE	1
#define MHSHADOW_DETAIL	2

// ��Io��Ec���סˢ�ACE��IiaAu
#define MAX_MUNPAFIELD_MEMBERNUM		30

// Murim Battle
#define	MAX_MURIMFIELD_MEMBERNUM		30

// E��E�������ע�����Io ����oA�ˢ硧���ˢ�I
#define RECOVERTIMEUNIT					3000
// E��E�������ע�����Io �ˢ硧���ˢ�I�ˢ硧��Y
#define RECOVERINTERVALUNIT				10

// A��E����AI �ˢ�icAIAi(CN A��E����AIAC A����I�ˢ硧ua)
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

// 080203 LYW --- �ൿ ���� ������ �ִ� ���� ����.
#define MAX_CONDUCTICON_NUM	999

#define IG_ITEM_STARTINDEX				10000
#define IG_MSGBOX_STRARTINDEX			12000
#define IG_STREETSTALLITEM_STARTINDEX	13000	//LBS 03.09.19
#define IG_EXCHANGEITEM_STARTINDEX		14000	//KES EXCHANGE 030920
#define IG_MENUSLOTITEM_STARTINDEX		15000	//KES 040510	//Ao�ˢ硧uY ����uE����u��E��I��E��IU. Ao��E?i�ˢ硧ui.
#define IG_PKLOOTINGDLG_START			16000	//KES PKLOOTING 040720
#define IG_DEBUG_START					20000	//KES DEBUG 040720
#define IG_QUESTITEM_STARTINDEX			25000	//RaMa
#define IG_SHOPITEM_STARTINDEX			28000	//RaMa 04.11.17
#define IG_DEALITEM_START				30000
#define IG_MAINBARICON_START			40000
//#define IG_ABILITYICON_START			31000
//#define IG_ABILITYPOINT_START			40000	//RaMa Ư��ġ ��й�
#define IG_QUICKITEM_STARTINDEX			60000
// 070216 LYW --- CommonGameDefine : Define start index of conduct part.
#define IG_CONDUCT_STARTINDEX			70000

// 070608 LYW --- CommonGameDefine : Define start index of matching favor icon part.
#define IG_FAVOR_STARTINDEX				150000
#define IG_IDENTIFICATION_STARTINDEX	151000
#define IG_MATCH_MYINFO_STARTINDEX		152000
#define IG_MATCH_PARTNERINFO_STARTINDEX	153000
#define IG_MATCH_ENDINDEX				154000


// 071121 ����, (��PM�� ��û)������Ʈ �ε����� �� ���� �����ؾ� �Ѵ�. �ε��� �߿� �����ϰ� �÷��̾� �ε�����
//		DB���� �����ϹǷ� int �������� ��� �����ȴ�. ���� �÷��̾� �ε����� �������� �ʰ� ��ų/NPC/���� 
//		�ε����� ����� �ڿ� ��ġ�Ѵ�.
//#define NEWOBJECT_STARTINDEX			2000001
//#define NEWRECALLOBJECT_STARTINDEX		3000001

#define IG_SKILL_STARTINDEX				(UINT_MAX - 500000)
#define NEWOBJECT_STARTINDEX			(UINT_MAX - 1000000)
#define NEWRECALLOBJECT_STARTINDEX		(UINT_MAX - 1500000)

//////////////////////////////////////////////////////////////////////////
// A��I���̡�Ioy ��E?��E��c�ˢ�ˢ�? AU��Iia
//----------------- �ˢ硧����I�ˢ碮E����A ----------------------------------
//pjs
// �ˢ硧��E�ˢ�io A��E����AO ����u�ˢ硧u����u��I��I..
enum SEARCH_TYPE
{
	MP_BY_SEARCHPRICE,
	MP_I_REGISTERED,
	MP_I_JOINED,
};
// d�ˢ�ˢ�A A��E����AO ����u�ˢ硧u����u��I��I..
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
//----------------- �ˢ硧����I�ˢ碮E����A ----------------------------------


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
// define�ˢ硧�����ע�I ��IoU��I��Aa����u�ˢ硧I ��Io��Ec���סˢ�A ����u���סˢ�AIAU ���סˢ�ˢ�AAo����uCAI ���ע���?CI��E����e Cy����u�ˢ硧uAI��E?��E��c�ˢ硧��O CEE�ˢ�A ��E?�ˢ�ϡ���EOo! <- db ����uod
//CoAc STORAGEITEM_STARTPOSITION : 586, STORAGEITEM_ENDPOSITION : 736

/*
#define INVENTORY_STARTPOSITION		((POSTYPE)100)												// 80�ˢ硧����I�Ϩ� 
#define INVENTORY_ENDPOSITION		((POSTYPE)INVENTORY_STARTPOSITION+INVENTORY_CELL_NUM)						//80
#define WEAR_STARTPOSITION			((POSTYPE)INVENTORY_ENDPOSITION)							//10�ˢ硧����I�Ϩ�
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

// Npc_Role�ˢ硧u��E�������ע������ע�E
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


////��I�Ϩ��ˢ��?���סˢ碮Ec��E?�ˢ�A ����i��o��Ei��u���סˢ碮Ec�ˢ�ˢ��ˢ硧��AA��E?��E��c����u�ˢ硧I �ˢ�ic��E?eCI��E��IA define  ///////////////////////////////////////////////
#define MAX_PACKEDDATA_SIZE 65535 // WORD type for SEND_PACKED_DATA


// AI���ע���I���ע������ע�E��E?��E��c �ˢ�ic��E?e + Client Move��E?��E��c����u�ˢ硧I �ˢ�ic��E?e
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


/// GameResource��E?��E��c �ˢ�ic��E?e��IiC��E��IA define ///////////////////////////////////////////////
enum eQUICKICON_KIND { eSKILL_ICON=1, eITEM_ICON, eABILITY_ICON,};
enum eSKILLITEM_KIND{eSKILLBOOK, eJINBUBBOOK, eSIMBUBBOOK, eERROR_BOOK,};
enum eITEM_KINDBIT 
{ 
	// RaMa - 04.11.02
	eSHOP_ITEM				= 256,
	eSHOP_ITEM_PREMIUM		= 257,	// �����̾�
	eSHOP_ITEM_CHARM		= 258,	// ����
	eSHOP_ITEM_HERB			= 259,	// ����
	eSHOP_ITEM_INCANTATION	= 260,	// �ֹ���
	eSHOP_ITEM_MAKEUP		= 261,	// ����
	eSHOP_ITEM_DECORATION	= 262,	// �ٹ̱�
	eSHOP_ITEM_SUNDRIES		= 263,	// ��ȭ
	eSHOP_ITEM_EQUIP		= 264,	// ��������

	eYOUNGYAK_ITEM			= 512,

	eSKILL_ITEM			= 1024,
	eSKILL_ITEM_SIMBUB		= 1037,
	eSKILL_ITEM_JINBUB		= 1038,
	eSKILL_ITEM_KYUNGGONG	= 1039,
	eSKILL_ITEM_OPTION		= 1040,

	/// ���� �����۷�
	eEQUIP_ITEM				= 2048,
	
	/// �����
	eEQUIP_ITEM_TWOHANDED,
	eEQUIP_ITEM_ONEHANDED,
	eEQUIP_ITEM_DAGGER,
	eEQUIP_ITEM_BOW,
	eEQUIP_ITEM_STAFF,
	eEQUIP_ITEM_MACE,
	eEQUIP_ITEM_SHIELD,

	/// ����
	/// 3D ���� ���� �͵�
	eEQUIP_ITEM_HAT,			
	eEQUIP_ITEM_SHOES,		
	eEQUIP_ITEM_DRESS,		
	eEQUIP_ITEM_GLOVE,

	/// �����ܸ� ǥ�õǴ� �͵�
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

// desc_hseos_����ý���_01
// S ����ý��� �߰� added by hseos 2007.05.14
enum ITEM_KIND
{
	ITEM_KIND_HP_RECOVER		= 1,	// ����� ȸ����
	ITEM_KIND_MP_RECOVER		= 2,	// ��� ȸ����
//---KES 
	ITEM_KIND_HP_RECOVER_INSTANT	= 3,	// ����� ȸ����
	ITEM_KIND_MP_RECOVER_INSTANT	= 4,	// ��� ȸ����
	ITEM_KIND_HPMP_RECOVER			= 5,	// ������ ���� ȸ����
	ITEM_KIND_HPMP_RECOVER_INSTANT	= 6,	// ������ ���� ȸ����
//-----
	ITEM_KIND_FARM_PLANT		= 7,	// ������ ���۹� �ɱ�
	ITEM_KIND_FARM_MANURE		= 8,	// ������ ���۹� ��� �ֱ�
	ITEM_KIND_BUFF_ITEMS		= 11,	// ������ ��ɵ��� ����� ���� �����۵�.
	ITEM_KIND_FARM_HARVEST		= 12,	// ������ ���۹� ��Ȯ
	ITEM_KIND_RESET_STATS		= 50,	// ���� �ʱ�ȭ ������
	ITEM_KIND_RESET_SKILL		= 51,	// ��ų �ʱ�ȭ ������

	// 080611 LYW --- CommonGameDefine : ������ Ÿ�� �߰�.
	ITEM_KIND_GET_SKILLPOINT	= 52,	// ��ų ����Ʈ ȹ��.

	ITEM_KIND_PROTECT_EXP		= 53,	// ����ġ ��ȣ ������
	ITEM_KIND_RECALL_MOVE		= 54,	// ����̵� �ֹ���
	ITEM_KIND_EXTEND_INVEN		= 55,	// �κ��丮 Ȯ�� ������
	ITEM_KIND_EXTEND_STORAGE	= 56,	// â�� Ȯ�� ������

	// 080607 LYW --- CommonGameDefine : ������ Ÿ�� �߰�.
	ITEM_KIND_OPENSTORAGE_BYITEM = 58,	// â�� ��ȯ ������.

	ITEM_KIND_CHANGE_NAME		= 66,	// �̸� ���� ������
	ITEM_KIND_MAP_MOVE_SCROLL	= 67,	// 071201 ���̵� �ֹ���
	ITEM_KIND_JOIN_EVENT		= 77,	// 080204 KTH -- �̺�Ʈ ���� ����
	ITEM_KIND_WORLDSHOUT		= 80,	// 080312 NYJ -- ��ü��ġ��(����Ȯ����) ������
	ITEM_KIND_FISHING_BAIT		= 81,	// 080410 NYJ -- ���� �̳�������
	ITEM_KIND_FISHING_FISH		= 82,	// 080410 NYJ -- ���� �����
	ITEM_KIND_RESET_REGIDENT_REGIST	= 88,// 080215 KTH -- �ֹε�� �ʱ�ȭ ������
	ITEM_KIND_RETURN_SCROLL		= 99,	// ��ȯ �ֹ���

	ITEM_KIND_CHALLENGEZONE_ENTER_FREQ	= 200,	// ç���� �� ���� ȸ�� ����

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


	// --- �ӽ�...
	ITEM_KIND_ANIMAL			= 500,	// 080415 KTH -- ����
	ITEM_KIND_ANIMAL_FOOD		= 501,	// 080507 KTH -- ����
	ITEM_KIND_ANIMAL_CLEANER	= 502,	// 080514 KTH -- ��� û�� ����
	
	ITEM_KIND_SUMMON_NPC					= 800,
	ITEM_KIND_SUMMON_MONSTER				= 801,
	ITEM_KIND_SUMMON_EFFECT					= 802,

};
// E ����ý��� �߰� added by hseos 2007.05.14

/// ������ ���� ������
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
/// ������ ���� ������

#define MAX_QUERY_RESULT 100
#define MAX_JOB		3
#define MAX_ATTACKSKILL_NUM	10
#define MAX_NAEGONG_NUM	10
#define MAX_KYUNGGONG_NUM	10
#define MAX_SKILLITEM_NUM	10
#define MAX_YOUNGYAKITEM_NUM	10
#define MAX_EQUIPITEM_NUM	300

#define MAX_PLAYER_NUM	135
// �ʵ庸�� - 05.12 �̿���
// ���� �� Ȯ�� 190 -> 300
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

#define MAX_CHARACTER_LEVEL_NUM		150			// A��I�Ϩ���E����?AI �ˢ�ˢ���Ii��o��Ei��u�ˢ碮�� ����uo

#define MAX_PLAYERLEVEL_NUM	151
#define MAX_MONSTERLEVELPOINTRESTRICT_NUM	9
#define MONSTERLEVELRESTRICT_LOWSTARTNUM	6

// �ˢ�ic��E?e��IiC��E��IA �ˢ硧���ˢ�A : ��Io�ˢ�i�ˢ硧����I���̢�E������Ec����uO����o���ע���AI��Ii|����o���ע���
enum WP_KIND{WP_GUM=1,WP_GWUN,WP_DO,WP_CHANG,WP_GUNG,WP_AMGI,WP_KEY,WP_EVENT,WP_MAX,WP_ERR};
enum WP_ATTR{WP_FIRE=1, WP_WATER, WP_TREE, WP_GOLD, WP_EARTH, WP_NONEATTR};
enum RE_TARGET_RANGE{TR_PERSONAL, TR_WHOLE,};
enum SKILL_TYPE{SKILLTYPE_NORMAL, SKILLTYPE_JINBUB, SKILLTYPE_SIMBUB, SKILLTYPE_MAX,};

// A���ע��Ϣ��ע�����E���� ��Io��I����E�ˢ�I(1��Io��I����A���ע��Ϣ��ע�����E����AC ��Io��I����E�ˢ�IAI��E��IU,2��Io��I���� A���ע��Ϣ��ע�����E������E��IA +1 3��Io��I����A���ע��Ϣ��ע�����E������E��IA +2....)
#define COMBO_GUM_MIN 1
#define COMBO_GWUN_MIN 7
#define COMBO_DO_MIN 13
#define COMBO_CHANG_MIN 19
#define COMBO_GUNG_MIN 25
#define COMBO_AMGI_MIN 31
#define COMBO_EVENT_MIN 50

#define MAX_COMBO_NUM		3
#define SKILL_COMBO_NUM		100
#define MAX_COMBOATTACK_LIST (WP_MAX*MAX_COMBO_NUM)		 // 6(��Io�ˢ�i�ˢ硧ua~�ˢ�ˢ�u) * 6(AO��E��IeA���ע��Ϣ��ע�����E����)

// ����o���ע���A��I�Ϩ� ��Io��I����E�ˢ�I
#define MAX_SKILLATTACK_LIST (WP_MAX*50)				// ��Io�ˢ�i�ˢ硧ua~�ˢ�ˢ�u * ��Io�ˢ�i�ˢ硧ua��E������EO��E��IUAC ����o���ע���A��I�Ϩ� ����uo
#define MAX_LEVEL_EXP 7									// 7����u���ע����ˢ硧uiAoAC �ˢ硧����I�ˢ�CeA��E��c
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
// AI��IoY�ˢ硧����I���̡ˢ硧��Y or ��E��IUA��I�����ˢ硧����I���̡ˢ硧��Y
#define ATTACKTYPE_NORMAL		0
#define ATTACKTYPE_HALFCIRCLE	1
#define ATTACKTYPE_CIRCLE		2
#define ATTACKTYPE_LINE			3
#define ATTACKTYPE_RECTANGLE	4

// ��E��IUA��I�����ˢ硧����I���̡ˢ硧��YAC ��Iou'AC �ˢ硧uaA���ס���a
#define	TARGETTYPE_TARGET	0
#define TARGETTYPE_ATTACKER 1
#define TARGETTYPE_GROUND	2
//////////////////////////////////////////////////////////////////////////

// ����u���סˢ�AIAU �ˢ硧u��E�������ע����ˢ硧����E?��E��c ��E��IeCN enum
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
	// desc_hseos_ü����������_01
	// S ü���������� �߰� added by hseos 2007.08.29
	eItemUseSuccess_ChangeItem,
	// E ü���������� �߰� added by hseos 2007.08.29

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
	// desc_hseos_����ý���_01
	// S ����ý��� �߰� added by hseos 2007.08.29
	eItemUseErr_FullInven,
	eItemUseErr_WithNoneMsg,
	// E ����ý��� �߰� added by hseos 2007.08.29

	eItemUseUnsealed,	//080317 NYJ ��������

	// 080607 LYW --- CommonGameDefine : â�� ��ȯ ������ ��� ���� ���� �߰�.
	eItemUse_OpenStorage_byItem_Failed_Storage_IsZero,
	// 080609 LYW --- CommonGameDefine : â�� ��ȯ ������ ��� ���� Ÿ�� �߰�.
	eItemUse_OpenStorage_Success,

	eItemUseErr_NoSummonInfo,

	eItemUseErr_ForbiddenSummon,	// 081027 NYJ ��ȯ����
};

/// GameResource��E?��E��c �ˢ�ic��E?e��IiC��E��IA define ///////////////////////////////////////////////
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

//����u���סˢ�AIAU Define
//��I�Ϩ���I��AAU 
#define ManBlueDress		23010 //���סˢ�o�ˢ�io���ע�����Io
#define ManBlueNothing		23011
#define ManDefaultShoose	27000 //��Io�ˢ�i��E����iE�ˢ硧I 
//��E?����IAU 
//#define WomanRedDress		12210 //���סˢ�o�ˢ�io���ע�����Io
#define WomanRedDress		23010
#define WomanDefalutShoose	27000 //��Io�ˢ�i��E����iE�ˢ硧I

//  [5/27/2003]

// A��I�Ϩ���E����?AIA�ˢ�E��E?��E��c ���סˢ�ˢ�AAIAI ��Ioe���ע������ע�E�ˢ硧��u�ˢ�ˢ�A
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
#define MUNPA_NOTMEMBER		0	//��Io��Ec���סˢ�A��E?��I����AI ����u���סˢ碮E��IN �ˢ�ic��EO�ˢ�A
#define MUNPA_SEMIMEMBER	1	//��Io��Ec���סˢ�A �ˢ硧����E��cAO����oAA�ˢ�iAI
#define MUNPA_MEMBER		2	//��Io��Ec���סˢ�A��E?��I����
#define MUNPA_SENIOR		3	//Aa�ˢ�ˢ�I
#define MUNPA_VICE_MASTER	10	//���ע���I��Io��EcAO

#define MUNPA_KIND_JUNG	1	//d���סˢ�A
#define MUNPA_KIND_SA	2	//�ˢ�ic���סˢ�A
#define MUNPA_KIND_WHANG	3	//E��I��A�ˢ硧uA
#define MUNPA_KIND_MU	4	//��Io�ˢ�i����uO����uO

#define MAX_MUNPA_BOARD_NUM	5	//��Io��Ec���סˢ�A �ˢ硧��O����oA���סˢ�C
#define MAX_MUNPA_BOARD_NAME 16		//�ˢ硧��O����oA���סˢ�C AI��E�����ˢ碮��
#define MAX_MUNPA_REGULARMEMBERLIST_NUM 10  //��E������Ec����i��o��Ei��u���סˢ碮Ec��E?��E��c ���ע�����E������E?����IAo��E��IA ��Io��Ec���סˢ�A��E?��I���̢�Iie ����uo
#define MAX_MUNPA_BATTLEKIND 3
#define MAX_MUNPA_SEMIMEMBERLIST_NUM 10 //��E������Ec����i��o��Ei��u���סˢ碮Ec��E?��E��c ���ע�����E������E?����IAo��E��IA ����oAA�ˢ�iAI��Iie ����uo
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

	// 080417 LUJ, ��� ��ų �߰� ����
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

//--------��E?��E��c�ˢ�ˢ�? ��E�������ע��Ϣ���oAAo----------------------------------------------------------
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
//--------��E?��E��c�ˢ�ˢ�? ��E�������ע��Ϣ���oAAo----------------------------------------------------------

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

	SLOT_STREETSTALL_NUM		= 24,	// ����Ȯ��â���� ����.
	SLOT_STREETBUYSTALL_NUM		= 5,	
	SLOT_SKILL_NUM				= 36,	
	SLOT_JINBUB_NUM				= 5,		// 5
	SLOT_SKILLTOTAL_NUM			= 36 * 4,	// 50	

};

// < ��E?��I����A�ˢ�E > : taiyo
// TP_INVENTORY_START��E��IA			��IoY��Iia����oA = 0
// TP_MUNPAWAREHOUSE_START��E��IA	��IoY��Iia����oA = TP_SIMBUB_END
// TP_MAX��E��IA						��IoY��Iia����oA = TP_STORAGE_END,
/*
enum
{
	//ITEMTABLE��E?��E��c����u�ˢ硧I �ˢ�ic��E?e
	TP_INVENTORY_START			= 0,
	TP_INVENTORY_END			= TP_INVENTORY_START		+ SLOT_INVENTORY_NUM,	// 80
	TP_WEAR_START				= TP_INVENTORY_END,									// 80
	TP_WEAR_END					= TP_WEAR_START				+ SLOT_WEAR_NUM,		// 90

	TP_STORAGE_START				= TP_WEAR_END,										// 90
	TP_STORAGE_END				= TP_STORAGE_START			+ SLOT_STORAGE_NUM,		// 240

	//ItemTableAI��E?U��E?��E��c����u�ˢ硧I �ˢ�ic��E?e
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
	//ITEMTABLE��E?��E��c����u�ˢ硧I �ˢ�ic��E?e
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

	TP_SHOPITEM_START			= TP_STORAGE_END,										// 240 <- 071126 280 ������;; KTH;;
	TP_SHOPITEM_END				= TP_SHOPITEM_START			+ SLOT_SHOPITEM_NUM,		// 390 <- 071126 316 ������ �ɵ�...;; KTH;;

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

// 071221 KTH -- �ִ� ����Ҽ� �ִ� ���� ����
#define MAX_MEMORY_STONE_INFO	20

// ---- ��Io�ˢ�i�ˢ硧����I����A�ˢ�E ����u���סˢ�AIAU AI��Ii|����o���ע��� ����u��I�Ϩ�d -----------------------------
#define MIN_SKILL_INDEX		100
#define MAX_SKILL_INDEX		4000
#define MIN_NAEGONG_INDEX		6000
#define MAX_NAEGONG_INDEX		7000
#define MIN_KYUNGGONG_INDEX		7000
#define MAX_KYUNGGONG_INDEX		8000
//----- ����u����u���ע���I ��Io�ˢ�i�ˢ硧����I����AI��Ii|����o���ע��� ����u��I�Ϩ�d ----------------------------------
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


// ---- AI���ע����ˢ�I���סˢ碮Ec��E������EcA�ˢ�E ����u���סˢ�AIAU AI��Ii|����o���ע��� ����u��I�Ϩ�d -----------------------------
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
#define MAX_PETNAME_LENGTH			32		//�ӽ÷� ��������		

//#define MAX_CHARACTER_NUM			4
#define MIN_GRIDITEM_AREA			0
#define MAX_GRIDITEM_AREA			1000
 
#define MAX_PICKITEM_DISTANCE	500

#define MAX_AUCTIONBOARD_DISTANCE 500


#define MAX_BOARDTITLE_NUM	10
#define MAX_BOARDTITLE_LENGTH 64
#define MAX_CONTENTS_LENGTH	65535


#define MAX_TARGET_NUM		300

//-----��Io�ˢ�i�ˢ硧����I���� �ˢ硧����I���̡ˢ硧��Y--------------------------------------
enum {GENERAL_ATTACK=0,SKILL_ATTACK=1};
//#define GENERAL_ATTACK_DISTANCE		200
// 070316 LYW --- CommonGameDefine : Modified distance
//#define POINT_VALID_DISTANCE		3000			// 10m: ���סˢ�ˢ�AAI���סˢ碮Ec E��Io��Ii��I�ˢ� /E�ˢ�I ��Iou'
// 070507 LYW --- CommonGameDefine : Modified distance
#define POINT_VALID_DISTANCE		6500


#define REMOVEFADE_START_TIME		2500
#define MONSTERREMOVE_TIME			5000
#define PLAYERREVIVE_TIME			5000
#define	PLAYERREVIVE_TIME_IN_BOSSMAP	60000


// ���ð��� Define
#define MAX_FISHING_LEVEL			4			// ���ü��õ� �ܰ�
#define MAX_FISHING_MISSION			3			// ���ù̼ǰ���
#define MAX_FISHLIST_FORPOINT		12			// ����⸮��Ʈ : ����Ʈ��ȯ�� ���
#define MAX_FISHINGMISSIONCODE		99			// 
#define MAX_FISHING_DISTANCE		2300.0f		// ���� ������ �Ÿ�
#define MAX_FISHPOINT				9999999		// �ִ� ���������Ʈ 9,999,999
#define FISHINGTIME_STARTPULLING	2000		// ������ ���۽ð�
#define FISHINGMISSION_INVALID		1000000		// �̼��ڵ�� 000000 ~ 999999 �����̹Ƿ� �̸� �ʰ��ϸ� ��ȿ��.

enum BASICEFFECT
{
	eEffect_NewCharacter = 1,	//A��I�Ϩ���E����?AI ��IiiAa
	eEffect_Standard = 2,		//A��I�Ϩ���E����?AI �ˢ硧ua���ע����ˢ�i(��Io�ˢ�i�ˢ硧ua����u��I����AI)
	eEffect_Walk,	//�ˢ硧��E�ˢ硧ua (��Io�ˢ�i�ˢ硧ua ����u��I����AI)
	eEffect_Run,	//��EOU�ˢ硧ua (��Io�ˢ�i�ˢ硧ua����u��I����AI)
	eEffect_BattleWalk_Gum,		//AuAo �ˢ硧��E�ˢ硧ua(�ˢ硧��E_��Io�ˢ�i�ˢ硧uaAaA��I����)
	eEffect_BattleWalk_Gwun,		//AuAo �ˢ硧��E�ˢ硧ua(�ˢ硧uC��Io�ˢ�i�ˢ硧uaAaA��I����)
	eEffect_BattleWalk_Do,		//AuAo �ˢ硧��E�ˢ硧ua(��Ii��Ii_��Io�ˢ�i�ˢ硧uaAaA��I����)
	eEffect_BattleWalk_Chang,		//AuAo �ˢ硧��E�ˢ硧ua(A�ˢ�E_��Io�ˢ�i�ˢ硧uaAaA��I����)
	eEffect_BattleWalk_Gung,		//AuAo �ˢ硧��E�ˢ硧ua(�ˢ硧uA_��Io�ˢ�i�ˢ硧uaAaA��I����)
	eEffect_BattleWalk_Amgi,		//AuAo �ˢ硧��E�ˢ硧ua(����uI�ˢ硧ua_��Io�ˢ�i�ˢ硧uaAaA��I����)
	eEffect_BattleRun_Gum,		//AuAo ��EOU�ˢ硧ua(�ˢ硧��E_��Io�ˢ�i�ˢ硧uaAaA��I����)
	eEffect_BattleRun_Gwon,		//AuAo ��EOU�ˢ硧ua(�ˢ硧uC_��Io�ˢ�i�ˢ硧uaAaA��I����)
	eEffect_BattleRun_Do,		//AuAo ��EOU�ˢ硧ua(��Ii��Ii_��Io�ˢ�i�ˢ硧uaAaA��I����)
	eEffect_BattleRun_Chang,		//AuAo ��EOU�ˢ硧ua(A�ˢ�E��Io�ˢ�i�ˢ硧uaAaA��I����)
	eEffect_BattleRun_Gung,		//AuAo ��EOU�ˢ硧ua(�ˢ硧uA_��Io�ˢ�i�ˢ硧uaAaA��I����)
	eEffect_BattleRun_Amgi,		//AuAo ��EOU�ˢ硧ua(����uI�ˢ硧ua_��Io�ˢ�i�ˢ硧uaAaA��I����)
	eEffect_UngijosikStart,	//��E?i�ˢ硧uav����oA(����uE��E��IA��Ii��E?AU)
	eEffect_Ungijosik,	//��E?i�ˢ硧uav����oA
	eEffect_Ungijosik_End,	//��E?i�ˢ硧uav����oA (��I�Ϩ���E��c��I�Ϩ��ˢ硧IEA)
	eEffect_UngijosikDamage,	//��E?i�ˢ硧uav����oA A��I������E?��E��c ��Ii�ˢ�I��IoIAo
	eEffect_StallStart,	//��I�Ϩ�ea�ˢ�io ��Ii��E?AU1 (����uE�ˢ硧ua)
	eEffect_Stall,	//��I�Ϩ�ea�ˢ�io ��Ii��E?AU1 (����uE����u���סˢ� AO�ˢ硧ua)
	eEffect_StallEnd,	//��I�Ϩ�ea�ˢ�io ��Ii��E?AU1 (����u�ˢ硧I�ˢ硧ua)
	eEffect_NormalDamage = 13,		//��Ii�ˢ�I��IoIAo1 (AA)
	eEffect_CriticalDamage = 14,		//��Ii�ˢ�I��IoIAo2 (��E?i)
	eEffect_Die1 = 16,		//A�ˢ��?= 1
	eEffect_Died1 = 16,		//A�ˢ��?����ui����u�ˢ硧I ��E��I����I��E?o AO��E��IA ��E������I��I����oA 1
	eEffect_Die2 = 15,		//A�ˢ��?= 2
	eEffect_Died2 = 15,		//A�ˢ��?����ui����u�ˢ硧I ��E��I����I��E?o AO��E��IA ��E������I��I����oA 2
	eEffect_LevelUpSentence = 3,	//�ˢ�ˢ���Ii��o��Ei��u�ˢ碮������u�ˢ�A �ˢ硧uUAU
	eEffect_MouseMovePoint,	//��E������EO��E?i����o���ע��� AI��Ii��E?���סˢ�ˢ�AAI���סˢ碮Ec
	
	eEffect_GetMoney = 5,	// ��E�����ˢ�AA�ˢ��?��E?����I����u�ˢ硧I ��Ii�ˢ�ˢ� ����uo�ˢ硧ua
	eEffect_GetItem_Accessory = 25,	// ��E�����ˢ�AA�ˢ��?��E?����I����u�ˢ硧I ����u���סˢ�AIA���סˢ� ����uo�ˢ硧ua

	eEffect_Emoticon_Bow,	//AI�ˢ�ic
	eEffect_Emoticon_Thanks,	//�ˢ硧���ˢ碮���ˢ�ic
	eEffect_Emoticon_Yes,	//�ˢ硧uad 
	eEffect_Emoticon_No,	//���ע���Id
	eEffect_Emoticon_Pleasure,	//�ˢ硧ua�ˢ�iY
	eEffect_Emoticon_Sadness,	//����o����oCA
	eEffect_Emoticon_Love,	//�ˢ�ic��EOu
	eEffect_Emoticon_Hostility,	//Au��E��Ie
	eEffect_Emoticon_Disappoint,	//����oC��E����A
	eEffect_Emoticon_Angry,	//���ע������ע�E��I�Ϩ�e
	eEffect_Emoticon_Suprised,	//��I�Ϩ�i��EO�ˢ�A
	eEffect_Emoticon_Evasion,	//E��E����CC
	eEffect_Emoticon_Declare,	//����u�ˢ硧u����u��I��I
	eEffect_Defence_Gum,		//��Ii��o|i��������ui��Ii��E?AU(�ˢ硧��E)
	eEffect_Defence_Gwun,		//��Ii��o|i��������ui��Ii��E?AU(�ˢ硧uC)
	eEffect_Defence_Do,		//��Ii��o|i��������ui��Ii��E?AU(��Ii��Ii)
	eEffect_Defence_Chang,		//��Ii��o|i��������ui��Ii��E?AU(A�ˢ�E)
	eEffect_Defence_Gung,		//��Ii��o|i��������ui��Ii��E?AU(�ˢ硧uA)
	eEffect_Defence_Amgi,		//��Ii��o|i��������ui��Ii��E?AU(����uI�ˢ硧ua)

	eEffect_GetItem_Weapon = 7,		//��Ii��o|i��������ui��Ii��E?AU(����uI�ˢ硧ua)
	eEffect_GetItem_Dress = 8,
	eEffect_GetItem_Quest = 10,
	eEffect_GetItem_Youngyak = 6,

	eEffect_MonRegen = 80,				// ������ ���� ������ eEffect_NewCharacter ����Ʈ�� �����
	eEffect_ChangeStage_Hwa = 98,
	eEffect_ChangeStage_Geuk = 99,

	eEffect_Jackpot = 1034,				// ���̴�÷

	eEffect_PetSummon = 3151,			// ���ȯ
	eEffect_PetSeal		= 3152,			// �����
	eEffect_FeedUp		= 3153,			// �����
	eEffect_PetWarp		= 3154,			// �����

	eEffect_ShopItem_Life = 3200,		// ��â��	
	eEffect_ShopItem_NaeRuyk,			// ����	
	eEffect_ShopItem_Revive,			// ��Ȱ�ֹ���
	eEffect_ShopItem_Boom1,				// ����(����)
	eEffect_ShopItem_Boom2,				// ����(��)
	eEffect_ShopItem_Hair1,				// ����1	
	eEffect_ShopItem_Hair2,				// ����2	
	eEffect_ShopItem_Hair3,				// ����3	
	eEffect_ShopItem_Teleport1,			// �ڷ���Ʈ1
	eEffect_ShopItem_Teleport2,			// �ڷ���Ʈ2
	eEffect_ShopItem_Teleport3,			// �ڷ���Ʈ3
	eEffect_ShopItem_MaxLife,			// ������ ����
	eEffect_ShopItem_MaxNaeRuyk,		// ������ ����

	// 070528 LYW CommonGameDefine : Add effect of emoticon part.
	eEffect_Emoticon1 = 51, 
	eEffect_Emoticon2 = 52, 
	eEffect_Emoticon3 = 53, 
	eEffect_Emoticon4 = 54, 
	eEffect_Emoticon5 = 55, 
	eEffect_Emoticon6 = 56, 

	eEffect_GetItem_Unique	= 40,
	eEffect_GetItem_Rare	= 41,

	// 071206 LUJ ��� ���� ȿ��
	eEffect_Guild_Graduate	= 49,
};


enum
{
	Alker_Farm = 2,
	Moon_Blind_Swamp = 13,		//E�ˢ硧IA�ˢ�iAo			Moon_Blind_Swamp
	Red_Orc_Outpost,			//��E��Ie����uEA����u			Red_Orc_Outpost
	Moon_Blind_Forest,			//A��I���̢���oAE��Ii��o?i��u���ע�I			Moon_Blind_Forest
	Haunted_Mine_1F = 16,
	Haunted_Mine_2F = 17,
	The_Gate_Of_Alker = 19,			//��E?y��Ec�ˢ硧��u			The_Gate_Of_Alker
	Alker_Harbor,		//��E����i����u���סˢ�ˢ�ie			Alker_Harbor

	Ruins_Of_Draconian,		//CuAO				Ruins_Of_Draconian
	Zakandia,		//����uc����uc				Zakandia
	Tarintus,			//Aa�ˢ�ic				Tarintus
	Mont_Blanc_Port = 25,		//��Ii��E?dE�ˢ�I			Mont_Blanc_Port
	Dryed_Gazell_Fall,			//����uC����uc�ˢ�ˢ�c			Dryed_Gazell_Fall
	Zakandia_Outpost,			//C�ˢ��?AO				Zakandia_Outpost
	The_Dark_Portal,			//����uOAO				Tournament
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
	// 080714 LUJ, "������ ����" �� �߰�
	TheKnightsGrave	= 54,
	A_Harbor_of_Nera = 55,
	DateDungeon_Cemetery = 91,
	DateDungeon_BrokenHouse = 92,
	DateDungeon_DarkCave = 93,


	MAX_MAP_ENUMCOUNT,
};

enum EObjectState
{
	eObjectState_None,						// ����u���סˢ硧Io�ˢ�i�ˢ�ioAA ����u���סˢ碮E��IO..
	eObjectState_Enter,						// A��I�Ϩ���E����?AI��I�Ϩ����ע�I ��E����o����o���ע���AI�ˢ硧����E��c A��I�Ϩ�= ��IiiAaCO��EO�ˢ碮�� AC �ˢ�ioAA
	eObjectState_Move,						// AAC�ˢ�I AI��Ii��E? �ˢ�ioAA
	eObjectState_Ungijosik,					// ��E?i�ˢ硧ua �ˢ�ioAA
	eObjectState_Rest,						// ��E����o����o���ע���AIAC E���ע��Ϣ���oA �ˢ�ioAA
	
//	eObjectState_Speech,					// Npc��E?I ��E��IeE�ˢ硧I �ˢ�ioAA
	eObjectState_Deal,						// �ˢ�ioa Open �ˢ�ioAA

	eObjectState_Exchange,					// �ˢ硧u��I�Ϩ�E? �ˢ�ioAA
	eObjectState_StreetStall_Owner,			// ��I�Ϩ�ea�ˢ�io Open �ˢ�ioAA
	eObjectState_StreetStall_Guest,			// ��I�Ϩ�ea�ˢ�io ����uO��E��IO �ˢ�ioAA
	eObjectState_PrivateWarehouse,			// �ˢ硧����I�Ϩ�AIA�ˢ�E�ˢ硧��i 10
	eObjectState_Munpa,						//��Io��Ec���סˢ�A ��E������Ec����i��o��Ei��u���סˢ碮Ec��I�Ϩ����ע�I ��Io��Ec���סˢ�AA�ˢ�E

	eObjectState_SkillStart,				// ����o���ע���A��I�Ϩ� ����oAAu �ˢ�ioAA
	eObjectState_SkillSyn,					//
	eObjectState_SkillBinding,				//
	eObjectState_SkillUsing,				//
	eObjectState_SkillDelay,				// A���ע��Ϣ��ע�����E�����ˢ硧����E��c ��I��A�ˢ�!����a; ��EO�ˢ碮��, ��Io�ˢ�i�ˢ硧����I��̢??��I��A�ˢ硧��i��I�Ϩ��ˢ硧IEAAC ��Iio�ˢ�ˢ���IoAI
	
	eObjectState_TiedUp_CanMove,			// �ˢ硧��a��IoU �ˢ�ioAA
	eObjectState_TiedUp_CanSkill,
	eObjectState_TiedUp,
	
	eObjectState_Die,						// A�ˢ��?: �ˢ�ioAA 20	
	
	eObjectState_BattleReady,
	eObjectState_Exit,						//KES 040707 A��I�Ϩ���E����?AI�ˢ硧����E��c ��I�Ϩ����ע�I�ˢ硧���ˢ�I��EO�ˢ碮��
	eObjectState_Immortal,					//����I��������AI, �ˡ�EAuE?, AuAa'A�ˢ� ��ii��u�����ϩ����̢���a��oA ��o��iAu��ioAA

	eObjectState_Society,					//KES A����A��I���� �좯AU
	
	eObjectState_ItemUse,

	eObjectState_TournamentReady,
	eObjectState_TournamentProcess,
	eObjectState_TournamentEnd,
	eObjectState_TournamentDead,

	eObjectState_Engrave,

	// 070503 LYW --- CommonGameDefine : Add enum to setting rest damage.
	eObjectState_RestDamage,
	eObjectState_Connect,					// ���� 070823

	// 080428 NYJ --- ���û���
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
//---�ˢ硧u��I�Ϩ�E?����u����u���ע���I�ˢ�ioAA(CoAc �ˢ�ic��E?e����uECI��E��IAA��I����)
enum eEXCHANGE_STATE
{
	eEXSTT_NONE,
	eEXSTT_APPLYING,	//�ˢ硧u��I�Ϩ�E?����oAA�ˢ�i; CN �ˢ�ioAA
	eEXSTT_APPLIED,		//�ˢ硧u��I�Ϩ�E?����oAA�ˢ�i; ��Io���ע���: �ˢ�ioAA
	eEXSTT_EXCHANGEING,	//�ˢ硧u��I�Ϩ�E?; A��I����CaA��I����AI �ˢ�ioAA : eObjectState_Exchange

	eEXSTT_MAX,
};



// �ˢ硧���ˢ�E~ ID��E?��E��c ��E��IeCN [6/2/2003]
// desc_hseos_����ý���_01
// S ����ý��� �߰� added by hseos 2007.05.07
#define FARM_RENDER_OBJ_ID_START	0x8fffffff
// E ����ý��� �߰� added by hseos 2007.05.07
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

// 06. 03 �������� - �̿���
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

// ��Io��I�ˢ�Ca AI��Ii|����i��o��Ei��u��E?��E��c ��E��IeCN ����u��I�Ϩ�d
typedef BYTE DIRINDEX;
#define MAX_DIRECTION_INDEX		32
#define DEGTODIR(angleDeg)	(BYTE)((angleDeg)/360*MAX_DIRECTION_INDEX)
#define DIRTODEG(DirIndex)	(((float)DirIndex)*360/MAX_DIRECTION_INDEX)
#define DIRTORAD(DirIndex)	(DEGTORAD(DIRTODEG(DirIndex)))
#define RADTODIR(angleRad)	(DEGTODIR(RADTODEG(angleRad)))

// A�ˢ�i��E�������ע��Ϣ�EOo ����u��I�Ϩ�d
#define CAMERA_NEAR		100
#define MAX_CHARTARGETPOSBUF_SIZE	15

//system
#define MAX_IPADDRESS_SIZE	16

// type : 0->����oA�ˢ硧���ˢ�I��EO�ˢ碮�����ע����ˢ硧�� ��I�Ϩ���I��A�ˢ硧ue. 1~100->AId�ˢ�ˢ���Ec AI�ˢ�io���ע���?E�ˢ硧I����oA ��I�Ϩ���I��A�ˢ硧ue(�ˢ硧����E��c�ˢ�ie), 101~200->(�ˢ硧���ˢ碮���ˢ�ie)
// 1->�ˢ�ioa��E?��E��c ���סˢ�C��E����A, 2->��I�Ϩ�ea��E?��E��c����u�ˢ硧I ���סˢ�C��E����A, 3->��E����o����o���ע���AI��E?��E��c�ˢ硧��O E��Io��Ii��I�ˢ�, 4->�ˢ硧u��I�Ϩ�E?, 5->A�ˢ�E�ˢ硧��i��E?��E��c����u�ˢ硧I �ˢ�i�ˢ�ϡ���EO�ˢ碮��, 6->��Io��Ec���סˢ�AA�ˢ�E�ˢ硧��i��E?��E��c����u�ˢ硧I �ˢ�i�ˢ�ϡ���EO�ˢ碮��
// 101->�ˢ�ioa��E?��E��c����u�ˢ硧I �ˢ�ii��EO�ˢ碮��, 102->��I�Ϩ�ea��E?��E��c����u�ˢ硧I �ˢ�ii��EO�ˢ碮��, 103->A�ˢ��?����uu;��EO�ˢ碮��, 104->�ˢ硧u��I�Ϩ�E?, 105->A�ˢ�E�ˢ硧��i��E?��E��c ��I�Ϩ�O=, 106->��Io��Ec���סˢ�AA�ˢ�E�ˢ硧��i��E?��E��c ��I�Ϩ�O=, 107->C�ˢ�I�ˢ硧u��Io �ˢ�ii��EO�ˢ碮��
enum eMoneyLogtype
{
	eMoneyLog_Time				= 0,
// �ˢ硧����E��c�ˢ�ie
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
	// desc_hseos_���͹���01
	// S ���͹��� �߰� added by hseos 2007.07.30
	eMoneyLog_GetMonstermeter,
	// E ���͹��� �߰� added by hseos 2007.07.30

	
// �ˢ硧���ˢ碮���ˢ�ie
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
	// desc_hseos_����ý���_01
	// S ����ý��� �߰� added by hseos 2007.08.23	2007.09.10	2008.01.15
	eMoneyLog_LoseFarmBuy,
	eMoneyLog_LoseFarmUpgrade,
	eMoneyLog_LoseFarmTax,
	// E ����ý��� �߰� added by hseos 2007.08.23	2007.09.10	2008.01.15

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
	// 06. 06. 2�� ���� - �̿���
	// ���� ��ȯ
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
	// ����Ʈ
	eLog_FamilyPoint	= 0,
	eLog_FamilyPointAdd,
	// 080610 LUJ, ����Ʈ ����
	eLog_FamilyPointUpdate,

	// ����
	eLog_FamilyFarm		= 100,
	eLog_FamilyFarmPutCrop,		// ����
	eLog_FamilyFarmDieCrop,		// ���
	eLog_FamilyFarmGrowCrop,	// ����
	eLog_FamilyFarmCareCrop,	// ���
	eLog_FamilyFarmHarvestCrop,	// ��Ȯ
	eLog_FamilyFarmLostRight,	// �Ǹ� ���(�йи� Ż�� ������ ���� �Ǹ� ���)
	eLog_FamilyFarmDisband,		// ���� ��ü(�йи� �ػ����� ����)
	// desc_hseos_����ý���_01
	// S ����ý��� �߰� added by hseos 2008.01.16
	eLog_FamilyFarmTax,			// ���� ��ü(������ �̳�)
	eLog_FamilyFarmMystery,		// ���� ��ü(���� ��ü �� �α׸� �߰����� �ʾ��� ���)
	// E ����ý��� �߰� added by hseos 2008.01.16
	// 080425 KTH -- ��� ���� �α� �߰� --------------
	eLog_FamilyFarmInstallAnimal,
	eLog_FamilyFarmDieAnimal,
	eLog_FamilyFarmFeedAnimal,
	eLog_FamilyFarmCleanAnimal,
	eLog_FamilyFarmAnimalReward,
	// 081205 LUJ, �йи� �ൿ
	eLog_FamilyCreate = 200,
	eLog_FamilyAddMember,
	eLog_FamilyChangeNick,
	eLog_FamilyRegistEmblem,
	eLog_FamilyLeaveMember,
	eLog_FamilyExpelMember,
	eLog_FamilyBreakup,
};

// desc_hseos_����ý���_01
// S ����ý��� �߰� added by hseos 2008.01.23
enum FARM_LOG_KIND																			// ���� �α� ����
{
	FARM_LOG_KIND_BUY = 0,																	// ..����
	FARM_LOG_KIND_UPGRADE,																	// ..���׷��̵�
	FARM_LOG_KIND_PAY_TAX,																	// ..������ ����
	FARM_LOG_KIND_DEL_FAMILY_BREAKUP,														// ..���� - �йи� ��ü
	FARM_LOG_KIND_DEL_TAX_ARREARAGE,														// ..���� - ������ ü��
	FARM_LOG_KIND_MYSTERY = 9999,
};

enum FARM_UPGRADE_LOG_KIND																	// ���� ���׷��̵� �α� ����
{
	FARM_UPGRADE_LOG_KIND_GARDEN = 0,														// ..�Թ�
	FARM_UPGRADE_LOG_KIND_FENCE,															// ..��Ÿ��
	FARM_UPGRADE_LOG_KIND_HOUSE,															// ..��
	FARM_UPGRADE_LOG_KIND_WAREHOUSE,														// ..â��
	FARM_UPGRADE_LOG_KIND_ANIMALCAGE,														// ..���
	FARM_UPGRADE_LOG_KIND_MYSTERY = 9999,
};
// E ����ý��� �߰� added by hseos 2008.01.23

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
	// 080716 LUJ, ��<->�÷��̾� �̵�
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
	// desc_hseos_���͹���01
	// S ���͹��� �߰� added by hseos 2007.07.08
	eLog_ItemObtainMonstermeter,
	// E ���͹��� �߰� added by hseos 2007.07.08

	// desc_hseos_��ȥ_01
	// S ��ȥ �߰� added by hseos 2007.12.13
	eLog_ItemObtainMarriage,
	// E ��ȥ �߰� added by hseos 2007.12.13

	eLog_ItemObtainFishing,
	// 080509 LUJ, ���� ȹ��
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
	eLog_ItemDissolveInsert,		// �� ���Կ� �������� ����
	eLog_ItemDissolveAddDuplicated,	// ���ؽ� �������� �� ���Կ� �������� �������� �߰���

	eLog_ItemEnchantSuccess,
	eLog_ItemEnchantItemInvalid,
	eLog_ItemEnchantServerError,
	eLog_ItemEnchantHacking,
	eLog_ItemEnchantItemUpdateFailed,
	eLog_ItemEnchantUse,
	eLog_ItemEnchantDestroy,

	eLog_ItemMixSystemFault,	// ���� �� �ý��� ����
	eLog_ItemMixNotFound,		// ������ ����� ã�� ����
	eLog_itemMixBadRequest,		// �߸��� ��û
	eLog_ItemMixBadCondition,	// ������ ������ ���� ����
	eLog_ItemMixBadQuantity,	// ������ �߸���
		
	eLog_ItemDestroyMix = 800,
	eLog_ItemDestroyReinforce,
	eLog_ItemDestroyUpgrade,
	eLog_ItemDestroyByWanted, // eLog_ItemDestroyByBadFame��| AIAEA����I ����A����Ao
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

	// 080602 LUJ, ��ų ��� �� �Ҹ�
	eLog_SkillItemUse	= 1700,
	eLog_SkillMoneyUse,
	
	eLog_Item = 2000,
	eLog_ItemOptionAdd_RM,
	eLog_ItemOptionRemove_RM,
	eLog_ItemAdd,
	eLog_ItemRemove,
	// desc_hseos_��ȥ_01
	// S ��ȥ �߰� added by hseos 2008.01.29
	eLog_ItemRemoveMarriageDivorce,
	// 081027 LUJ, ��� �ɼ� ����
	eLog_ItemDropOptionAdd,
		
	eLog_ItemMoveInvenToPetInven = 2100,
	eLog_ItemMovePetInvenToInven,

	eLog_ItemSkillLearn	= 2200,

	eLog_GTournamentReg = 2300,		// �����ʸ�Ʈ ��Ϻ�
	// 081205 LUJ, ����
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

	// ����
	eGuildLog_Score	= 500,
	eGuildLog_TimeScoreAdd,
	eGuildLog_ScoreAddByKill,		// ����� ��� ����
	eGuildLog_ScoreRemoveByMurder,	// ��������� ���
	eGuildLog_ScoreAddByWin,		// ����� �¸�
	eGuildLog_ScoreRemoveByDefeat,	// ����� �й�
	eGuildLog_ScoreCheat,			// ġƮ
	eGuildLog_ScoreLevelUp,			// �������� ���
	eGuildLog_ScoreSkillUse,		// 080602 LUJ, ��ų �ߵ��� �Ҹ�

	// gm ��
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
	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.10.11
	eExpLog_LosebyLeaveFamily,
	eExpLog_LosebyExpleFamilyMember,
	// E �йи� �߰� added by hseos 2007.10.11
	eExpLog_LoseByPKMode,
	// 080602 LUJ, ��ų ������� ���� ����ġ �Ҹ�
	eExpLog_LoseBySkillUse,

	eExpLog_GetbyQuest = 100,
	eExpLog_SExpGetbyQuest = 101,
	
	eExpLog_ProtectExp = 150,
	eExpLog_ReviveExp,
};

enum eLogToolType	// Tool A����u
{
	eToolLog_GM = 1,
	eToolLog_RM,
	eToolLog_Server,
};

enum eGMLogKind		// GMTool ��I���� A����u
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
	eFishingLog_SetPointFromCheat = 1,	// ġƮ�κ��� ���������Ʈ ����
	eFishingLog_GetPointFromFish,		// �����κ��� ���������Ʈ ȹ��
	eFishingLog_UsePointForBuyItem,		// �������� ������� ���������Ʈ ���
	eFishingLog_SetLevel,				// 080808 LUJ, ���� ���� ����
	eFishingLog_Regular,				// 080808 LUJ, ���� �α� ����
};

enum eRMLogKind		// RMTool ��I���� A����u
{
	eRMLog_OperInsert = 1,
	eRMLog_OperDelete,
	eRMLog_IpInsert,
	eRMLog_IpDelete,
	/// 060829 PKH - gm�� �������� ���
	eRMLog_GMIpInsert,
	/// 060829 PKH - gm�� ��� �ƾ��� ����
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

	eRMLog_ChangePetInfo = 400,			//06.08.04 - pkh [��]rm���α� ���
};


// 080716 LUJ, �� �α�
enum ePetLog
{
	ePetLogSummon,
	ePetLogDie,
	ePetLogRevive,
	ePetLogChangeGrade,
	ePetLogChangeLevel,
	ePetLogRegular,		// 080716 LUJ, ���� �������� ���� ����
};


// �����ʸ�Ʈ �α�
enum eGTLogKind
{
	eGTLogKind_None=0,
	eGTLogKind_Regist,				// ���
	eGTLogKind_Win,					// ��
	eGTLogKind_Lose,				// ��
};


// ������ �α�
enum eSWLogKind
{
	eSWLogKind_None=0,
	eSWLogKind_DefenceProposal,			// �������� ��û
	eSWLogKind_Attack,					// ��������
	eSWLogKind_Defence,					// ������ ����
	eSWLogKind_DefenceAccept,			// �������� ������� �������� ����
	eSWLogKind_DefenceUnion,			// ���ͰἺ���� �������� ����
	eSWLogKind_CancelByCastleGuild,		// �����İ� ����Ż��� �ٸ��������� �������� ���
	eSWLogKind_CancelByUnionSecession,	// �����Ŀ��� ���� Ż��� �������� ���
	eSWLogKind_StartSiegeWar,			// ��������
	eSWLogKind_SucceedCastleGuild,		// ������������ ���� ������ ����
	eSWLogKind_SucceedMoveToDefence,	// ������������ ���� ������ ������ ������ �������� �̵�
	eSWLogKind_SucceedAttack,			// �������� ������ ���� �״���� ����
	eSWLogKind_SucceedMoveToAttack,		// �������� ������������ ����->�������� �Űܰ� ����
	eSWLogKind_EndCastleGuild,			// ���������� ���� ������ ����
	eSWLogKind_EndDefenceUnionGuild,	// ���������� �����̾ �������� ��ϵǴ� ����
	eSWLogKind_EngraveSyn,				// ���νõ�
	eSWLogKind_EngraveNack,				// ���ν��� �¾� ����
	eSWLogKind_EngraveNackCancel,		// ���ν��� ���
	eSWLogKind_EngraveNackLogOut,		// ���ν��� �α׾ƿ�
	eSWLogKind_SiegeWarSucceed,			// ���μ���
	eSWLogKind_EndSiegeWar,				// ��������
};


// Au�ϩ�I
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


// ¦®¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¯
// ¦­ÁÖ¹®¼­ ¾Æ?ÌÅÛ  RaMa - 04.11.09                            ¦­
// ¦±¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦°
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
// 06. 03. ���İ��� - �̿���
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

#define MAX_GUILD_MEMBER	80 //60 + 20(���û�)

#define MAX_FAMILY_NAME 16
//#define MAX_FAMILY_INTRO		60
// 06. 03. ���İ��� - �̿���
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
//#define MAX_FAMILYMEMBER_NUM 55 //30 + 25(�������ϻ�)

enum
{
	eShopItem_PeneltyCountZero,
};


//-----------------------------------------------------------------------------------
// Avatar Item
//-----------------------------------------------------------------------------------
enum
{
	eAvatar_Hat=0,			// ����
	eAvatar_Hair,			// �Ӹ�
	eAvatar_Face,			// ��
	eAvatar_Mask,			// ����
	eAvatar_Glasses,		// �Ȱ�
	eAvatar_Mustache,		// ����
	eAvatar_Dress,			// ��
	eAvatar_Shoulder,		// ���
	eAvatar_Back,			// ��
	eAvatar_Shoes,			// �Ź�
	eAvatar_Effect,			// ����Ʈ
	eAvatar_Hand,			// ��

	eAvatar_Weared_Hair,	// �Դ� ������ �Ӹ�
	eAvatar_Weared_Face,	// �Դ� ������ ��
	eAvatar_Weared_Hat,		// �Դ� ������ �Ӹ���	
	eAvatar_Weared_Dress,	// �Դ� ������ ��
	eAvatar_Weared_Shoes,	// �Դ� ������ �Ź�	
	eAvatar_Weared_Gum,		// ���빫�� ��
	eAvatar_Weared_Gwun,	// ���빫�� ��
	eAvatar_Weared_Do,		// ���빫�� ��
	eAvatar_Weared_Chang,	// ���빫�� â
	eAvatar_Weared_Gung,	// ���빫�� ��
	eAvatar_Weared_Amgi,	// ���빫�� �ϱ�

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
	eIncantation_TownMove			= 55301,		// �����̵�
	eIncantation_MemoryMove			= 55302,		// ����̵�
	eIncantation_TownMove15			= 55303,		// �����̵�15��
	eIncantation_MemoryMove15		= 55304,		// ����̵�15��
	eIncantation_TownMove7			= 57508,		// �����̵�7��
	eIncantation_TownMove7_NoTrade	= 57509,		// �����̵�7�� ��ȯ�Ұ�
	eIncantation_MemoryMove7		= 57510,		// ����̵�7��
	eIncantation_MemoryMove7_NoTrade = 57511,		// ����̵�7�� ��ȯ�Ұ�
	eIncantation_MoneyProtect		= 55311,		// ���� �ູ
	eIncantation_ExpProtect			= 55312,		// ������ �ູ
	eIncantation_ProtectAll			= 55313,		// ��ȣ���� �ֹ���
	eIncantation_StatePoint			= 55321,		// ��������Ʈ ��й�
	eIncantation_MixUp				= 55322,		// ����Ȯ�� ���
	eIncantation_OtherRevive		= 55331,		// ��Ȱ	
	eIncantation_MemoryStone		= 55601,		// ��Ｎ
//	eIncantation_SkPointRedist_2	= 55322,
	eIncantation_SkPointRedist_4	= 55323,
	eIncantation_SkPointRedist		= 55300,	
	eIncantation_ShowStorage			= 55351,		// â���ȯ �ֹ���15��
	eIncantation_ShowStorage7		= 57506,		// â���ȯ �ֹ���7��
	eIncantation_ShowStorage7_NoTrade = 57507,		// â���ȯ �ֹ���7�� ��ȯ�Ұ�
	eIncantation_ChangeName			= 55352,		// �̸�����
	eIncantation_Tracking			= 55353,		// ����� ����15��
	eIncantation_Tracking7			= 57504,		// ����� ����7��
	eIncantation_Tracking7_NoTrade	= 57505,		// ����� ����7�� ��ȯ�Ұ�
	eIncantation_Tracking_Jin 		= 55387,		// �������ֹ���
	eIncantation_CharChange			= 55354,		// ������ȯ�ֹ���
	eIncantation_ShapeChange		= 55355,
	eIncantation_ItemSeal			= 55356,		// ����� �ֹ���
	eIncantation_ProtectAll15		= 55358,		// ��ȣ���� �ֹ���(15��)
	eIncantation_ChangeJob			= 55360,		// ȭ��/�ظ� ���� �ֹ���
	eIncantation_SkillExtend		= 55361,		// ����â Ȯ�� �ֹ���
	eIncantation_ReinforceReset		= 55364,		// ��ȭ�ʱ�ȸ �ֹ���
	eIncantation_MemoryMoveExtend	= 55365,		// ���âȮ�� �̵��ֹ���
	eIncantation_MemoryMove2		= 55371,		// ����̵� 2��
	eIncantation_ProtectAll1		= 55372,		// ��ȣ�� 1��

	eIncantation_PetRevival			= 55382,		// �� ��Ȱ �ֹ���
	eIncantation_PetGrade2 			= 55384,		// �� ���� �ֹ��� 2�ܰ�
	eIncantation_PetGrade3 			= 55385,		// �� ���� �ֹ��� 3�ܰ�
	eIncantation_StorageExtend 		= 57544,		// â��Ȯ���ֹ���	
	eIncantation_CharacterSlot 		= 57543,		// ĳ���� �߰� ����
	eIncantation_InvenExtend 		= 57542,		// �κ��丮Ȯ�� �ֹ���
	eIncantation_ChangeName_Dntrade = 55352,		// �̸�����(�ŷ��ȵǴ�)
	eSundries_Boom_Butterfly		= 55621,		// ����(����)
	eSundries_Boom_Star				= 55622,		// ����(��)
	eSundries_Shout					= 55631,		// ��ġ��
	eSundries_Shout_Once			= 55632,		// ��ġ��1ȸ
	eSundries_Shout_Once_NoTrade	= 57593,		// ��ġ��1ȸ �ŷ��Ұ�
	eSundries_RareItemCreate50		= 55640,
	eSundries_RareItemCreate70		= 55641,
	eSundries_RareItemCreate90		= 55642,
};


enum
{
	eShopItemUseParam_Realtime = 1,				// ����ð�
	eShopItemUseParam_Playtime = 2,				// �÷��̽ð�
	eShopItemUseParam_Continue = 3,				// ����� ���� ���߿� ȿ���� �ߵ��Ǵ°�
	eShopItemUseParam_Forever = 4,				// ���� ������

	eShopItemUseParam_EquipAvatar = 10,			// ������� �ƹ�Ÿ ������
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


//������������������������������������������������������������������
//��Don't Duplication shopItem									  ��
//������������������������������������������������������������������
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




//��������������������������������������������������������
//��Guild Tournament									��
//��������������������������������������������������������
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
	eGTWin_Unearned,		// ������
	eGTWin_RemainPlayer,	// ���������
	eGTWin_LowLevel,		// �������� ����
	eGTWin_LowExp,			// ����ġ�� ����.
};


//������������������������������������������������������������������
//��SiegeWar													  ��
//������������������������������������������������������������������
#define MAX_CASTLEGATE_NUM		20



//��������������������������������������������������������
//��Npcscript Param										��
//��������������������������������������������������������
enum
{
	eNpcParam_ShowpStorage=1,
};




//��������������������������������������������������������
//��Item Parameter										��
//��������������������������������������������������������
#define ITEM_PARAM_SEAL		0x00000001
#define ITEM_PARAM_CHEAT	0x00000010
#define ITEM_PARAM_PRESENT	0x00000100



// 071119 LYW --- CommonGameDefine : ���ο� ������ Parameter �߰�.
//��������������������������������������������������������
//��New Item Parameter									��
//��������������������������������������������������������
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
	eStage_H_Fighter			= 1111,	// ������
	eStage_H_Guard				= 1121,	// ����
	eStage_H_Warrior			= 1122,	// ����
	eStage_H_InfantryMan		= 1131,	// ����
	eStage_H_SwordMan			= 1132,	// �ҵ��
	eStage_H_Mercenary			= 1133,	// �뺴
	eStage_H_Phalanx			= 1141, // �ȶ�ũ��
	eStage_H_Knight				= 1142, // ���
	eStage_H_Gladiator			= 1143, // ������
	eStage_H_RunicKnight		= 1144, // �� ����Ʈ
	eStage_H_Paladin			= 1151, // �ӷ���
	eStage_H_Panzer				= 1152, // �Ⱙ�δ��
	eStage_H_Crusader			= 1153, // ���ڱ�����
	eStage_H_Destroyer			= 1154, // �ı���
	eStage_H_SwordMaster		= 1155, // �ҵ帶����
	eStage_H_Magners			= 1156, // �ű׳ʽ�
	eStage_H_Lord				= 1161, // �ε�
	eStage_H_DeathKNight		= 1162, // ��������Ʈ
	eStage_H_ArcTemplar			= 1163, // ��ũ���÷�

	eStage_E_Fighter			= 1211,	// ������
	eStage_E_Guard				= 1221,	// ����
	eStage_E_Warrior			= 1222,	// ����
	eStage_E_InfantryMan		= 1231,	// ����
	eStage_E_SwordMan			= 1232,	// �ҵ��
	eStage_E_Mercenary			= 1233,	// �뺴
	eStage_E_Phalanx			= 1241, // �ȶ�ũ��
	eStage_E_Knight				= 1242, // ���
	eStage_E_Gladiator			= 1243, // ������
	eStage_E_RunicKnight		= 1244, // �� ����Ʈ
	eStage_E_Paladin			= 1251, // �ӷ���
	eStage_E_Panzer				= 1252, // �Ⱙ�δ��
	eStage_E_Crusader			= 1253, // ���ڱ�����
	eStage_E_Destroyer			= 1254, // �ı���
	eStage_E_SwordMaster		= 1255, // �ҵ帶����
	eStage_E_Magners			= 1256, // �ű׳ʽ�
	eStage_E_Lord				= 1261, // �ε�
	eStage_E_DeathKNight		= 1262, // ��������Ʈ
	eStage_E_ArcTemplar			= 1263, // ��ũ���÷�
	

	eStage_H_Rogue				= 2111, // �����
	eStage_H_Voyager			= 2121, // ������
	eStage_H_Ruffian			= 2122, // ������
	eStage_H_Archer				= 2131, // �ü�
	eStage_H_Chief				= 2132, // ����
	eStage_H_Scout				= 2133, // ������
	eStage_H_Ranger				= 2141, // �δ��
	eStage_H_TreasureHunter 	= 2142, // ������ɲ�
	eStage_H_Assassin			= 2143, // �ϻ���
	eStage_H_RunicWorker		= 2144, // ���Ŀ
	eStage_H_Sniper				= 2151, // ���ݺ�
	eStage_H_Intraper			= 2152, // 
	eStage_H_BladeTaker			= 2153, //
	eStage_H_TemperMaster		= 2154, //

	// 080709 LYW --- CommonGameDefine : ��ũ������ ������ �߰��Ѵ�.
	eState_H_ArcRanger			= 2155,	// ��ũ������

	eStage_H_SoulEye			= 2161, //
	eStage_H_BloodStalker		= 2162, //
	eStage_H_ArcBridger			= 2163, //

	eStage_E_Rogue				= 2211, // �����
	eStage_E_Voyager			= 2221, // ������
	eStage_E_Ruffian			= 2222, // ������
	eStage_E_Archer				= 2231, // �ü�
	eStage_E_Chief				= 2232, // ����
	eStage_E_Scout				= 2233, // ������
	eStage_E_Ranger				= 2241, // �δ��
	eStage_E_TreasureHunter 	= 2242, // ������ɲ�
	eStage_E_Assassin			= 2243, // �ϻ���
	eStage_E_RunicWorker		= 2244, // ���Ŀ
	eStage_E_Sniper				= 2251, // ���ݺ�		
	eStage_E_Intraper			= 2252, // 
	eStage_E_BladeTaker			= 2253, //
	eStage_E_TemperMaster		= 2254, //

	// 080709 LYW --- CommonGameDefine : ��ũ������ ������ �߰��Ѵ�.
	eState_E_ArcRanger			= 2255,	// ��ũ������

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
	eStage_Normal	= 0,	// �Ϲ�
	eStage_Hwa		= 64,	// ����	  		
	eStage_Hyun		= 65,	// ����	  		
	eStage_Geuk		= 128,	// �ظ�	  		
	eStage_Tal		= 129,	// Ż��	  		
};

enum eItemStage
{
	eItemStage_Normal		= 0,
	eItemStage_ChangeStage	= 32,
	///////////////////////////////
	// 06. 06. 2�� ���� - �̿���
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


//��������������������������������������������������������
//��For Japan_Local										��
//��������������������������������������������������������
enum eCharacter_Attr	// ȭ->��->��->��->��
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
	eCAR_Give_Help			= 1,	// �����ִ� ���
	eCAR_Strong_Opposite	= 2,	// ���� ���� ���
	eCAR_Weak_Opposite		= 3,	// ���� ������ ���
	eCAR_Need_Help			= 4,	// ����޴� ���	
};


#define GIVEN_CHARACTER_SLOT		2
#define GIVEN_INVENTORY_SLOT		2
#define GIVEN_STORAGE_SLOT			2
#define GIVEN_SKILL_SLOT			10
#define SKILL_SLOT_ADDCOUNT		5

#define EVENT_MONSTER_SANTA1			145			// ���� 99 ��Ÿ�ǿ���
#define EVENT_MONSTER_SANTA2			160			// ���� 15 ��Ÿ�ǿ���
#define EVENT_DAMAGE					2005		// ��Ÿ�ǿ����� ������ ���ݽ� �ߴ� ������

#define EVENT_ITEM_GLOVE			50509			// ����尩	
#define EVENT_ITEM_SNOW				50508			// ������
#define EVENT_ITEM_FIRECRACKER		53111			// ����

#define EVENT_EFFECT_FIRECRACKER	1036			// ���� ����Ʈ

#define EVENT_SHOPITEM_SNOWMAN_HAT			55572	// �ƹ�Ÿ ������ �����(����)
#define EVENT_SHOPITEM_SNOWMAN_DRESS		55573	// �ƹ�Ÿ ������ �����(��)
#define EVENT_SHOPITEM_RUDOLP_HAT			55574	// �ƹ�Ÿ ������ �絹��(����)
#define EVENT_SHOPITEM_RUDOLP_DRESS			55575	// �ƹ�Ÿ ������ �絹��(��)

#define EVENT_SHOPITEM_SNOWMAN_HAT2			55580	// �ƹ�Ÿ ������ �����(����) �ð���
#define EVENT_SHOPITEM_SNOWMAN_DRESS2		55581	// �ƹ�Ÿ ������ �����(��) �ð���
#define EVENT_SHOPITEM_RUDOLP_HAT2			55582	// �ƹ�Ÿ ������ �絹��(����) �ð���
#define EVENT_SHOPITEM_RUDOLP_DRESS2		55583	// �ƹ�Ÿ ������ �絹��(��) �ð���

// ������
enum eWeatherState
{
	//eWS_None,	// 0
	//eWS_Snow,	// 1
	eWS_Clear,	// ����
	eWS_Cloudy,	// �帲
	eWS_Rainy,	// ��
	eWS_Windy,	// �ٶ�
	eWS_Foggy,	// �Ȱ�
	eWS_Typoon,	// ��ǳ

	eWS_Max,	
};

// 080410 NYJ --- ���ð��� �����ڵ� 
enum eFishingErrorCode
{
	eFishingError_None = 0,		// NotError, Success
	eFishingError_InvaildHero,	// ��ȿ���� ���� �÷��̾�
	eFishingError_InvaildPlace,	// ��ȿ���� ���� ������
	eFishingError_NoPole,		// ���ô� ������
	eFishingError_NoBait,		// �̳� �̻��
	eFishingError_OverTime,		// �ð��ʰ�
	eFishingError_UserCancel,	// ������ ��ҿ�û
	eFishingError_InvenFull,	// �κ��� ����������
	eFishingError_InvalidGrade,	// ���õ�� ����ġ
	eFishingError_EarlyTime,	// �ð�������
	eFishingError_InvaildState,	// ��ȿ���� ���� ����
	eFishingError_OverMaxFishPoint,	// �ִ� ���������Ʈ �ʰ�
	eFishingError_FailFishPoint,// ���������Ʈ��ȯ ����
	eFishingError_OverDistance,	// �Ÿ��ʰ�
};

// 080414 NYJ --- ���ð��� ����������Ÿ��
enum eFishItemType
{
	eFishItem_Poor = 0,		// [0]�Ƕ��
	eFishItem_Common,		// [1]�ؾ�
	eFishItem_UnCommon,		// [2]�׾�
	eFishItem_Rare,			// [3]Ȳ���׾�, Ȳ���ڶ�
	eFishItem_Unique,		// [4]������ ������
	eFishItem_Event,		// [5]ģȯ��̳�
	eFishItem_Reserve1,		// [6]����1
	eFishItem_Reserve2,		// [7]����1
	eFishItem_Reserve3,		// [8]����1
	eFishItem_Reserve4,		// [9]����1
	
	eFishItem_Max,
};

////////////////////////////////////////////////////////
//06. 06 2�� ���� - �̿���
//����Ʈ ����(����)
enum eSkipEffect
{
	eSkipEffect_None,
	eSkipEffect_Start,

	eSkipEffect_Max,
};
////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// 06. 06. 2�� ���� - �̿���
// ���� ��ȯ �߰�
#define MAX_SKILLOPTION_COUNT	8	// �ִ� �ɼ� ��

enum eSkillOption_Kind{
	
	eSkillOption_None,				// �ɼ� ����

	eSkillOption_Range,				// 1. ��Ÿ�
	eSkillOption_ReduceMana,		// 2. ���¼Ҹ�
	eSkillOption_PhyAtk,			// 3. ��ų��������
	eSkillOption_BaseAtk,			// 4. �Ϲݹ�������
	eSkillOption_AttAtk,			// 5. �Ӽ�����

	eSkillOption_Life,				// 6. �ִ�����
	eSkillOption_Mana,			// 7. �ִ볻��
	eSkillOption_PhyDef,			// 9. �������
	eSkillOption_AttDef,			// 10. �Ӽ����

	eSkillOption_Duration,			// 11. ���ӽð�

	eSkillOption_Max,
};
//////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////
// 06. 06. 2�� ���� - �̿���
// ���� ��ȯ
// ��ȯ ���� - 0: ��ȯ�Ұ�, 1: ������, 2:������, 3:��ΰ���
enum eSkillChangeKind{
	
	eSkillChange_None,		// ��ȯ�Ұ�
	eSkillChange_Offense,	// ������
	eSkillChange_Defense,	// ������
	eSkillChange_All,		// ��� ����

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
//	//��������Ʈ
//	int GuildPoint;
//	//��ɸ�������
//	int GuildHuntedMonsterCount;
//	//��ɸ���������
//	int GuildHuntedMonsterTotalCount;
//	//DB������Ʈ �ð�
//	DWORD DBProcessTime;
//	//��빮���÷���Ÿ��
//	DWORD GuildPlusTimeflg;
//	//�����÷���Ÿ�� ����
//	GUILDUSING_PLUSTIME_INFO GuildUsingPlusTimeInfo[eGPT_Kind_Max];
//	//��빫������(����)
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

enum	DATE_MATCHING_REGION				// ����Ʈ ��Ī �ý��ۿ��� ��� ���� ����. ���ö���¡ �Ϸ��� ������������ ������ ������ ��.?
{
	eDATE_MATCHING_REGION_KYUNGKI,			// ���,
	eDATE_MATCHING_REGION_SEOUL,			// ����
	eDATE_MATCHING_REGION_INCHON,			// ��õ
	eDATE_MATCHING_REGION_KANGWON,			// ����
	eDATE_MATCHING_REGION_CHUNGNAM,			// �泲
	eDATE_MATCHING_REGION_CHUNGBUK,			// ���
	eDATE_MATCHING_REGION_DAEJEON,			// ����
	eDATE_MATCHING_REGION_KYUNGBUK,			// ���
	eDATE_MATCHING_REGION_KYUNGNAM,			// �泲
	eDATE_MATCHING_REGION_DAEGU,			// �뱸
	eDATE_MATCHING_REGION_BUSAN,			// �λ�
	eDATE_MATCHING_REGION_ULSAN,			// ���
	eDATE_MATCHING_REGION_JEONBUK,			// ����
	eDATE_MATCHING_REGION_JEONNAM,			// ����
	eDATE_MATCHING_REGION_KWANGJU,			// ����
	eDATE_MATCHING_REGION_JEJU,				// ����
	eDATE_MATCHING_REGION_OVERSEAS,			// �ؿ�
};

enum	DATE_MATCHING_FEELING				// ����Ʈ ��Ī �ý����� ����.
{
	eDATE_MATCHING_FEELING_BLACKHAIR,		// ���� �Ӹ�
	eDATE_MATCHING_FEELING_DYEDHAIR,			// ������ �Ӹ�
	eDATE_MATCHING_FEELING_FORMALDRESS,		// ����
	eDATE_MATCHING_FEELING_CASUALWEAR,		// ĳ�־� ����
	eDATE_MATCHING_FEELING_MAKEUP,			// ȭ��
	eDATE_MATCHING_FEELING_PERFUME,			// ���
	eDATE_MATCHING_FEELING_THIN,				// ����
	eDATE_MATCHING_FEELING_FAT,				// �׶���
	eDATE_MATCHING_FEELING_BEARD,			// ����
	eDATE_MATCHING_FEELING_EYEGLASS,			// �Ȱ�
	eDATE_MATCHING_FEELING_HAT,				// ����
	eDATE_MATCHING_FEELING_ACCESSORIES,		// �׼�����
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
	// 071217 KTH --- ����ġ ��ȣ
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
	// 080616 LUJ, 5�� ��ų �߰�
	eStatusKind_ConvertHealthWithGivedDamage	= 106,
	eStatusKind_ConvertManaWithGivedDamage		= 107,
	eStatusKind_EventSkillToVictim				= 108,
	eStatusKind_EventSkillToKiller				= 109,
	// 080703 LUJ, �̵��� ��� ����
	eStatusKind_EnableTwoBlade					= 110,
	// 081203 LUJ, ��ũ��Ʈ ���
	eStatusKind_Script							= 111,
	// 071204 KTH -- ����ġ ���� %
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

// 080602 LUJ, ��ų Ÿ�� ���. Target.h���� �Űܿ�
enum TARGET_KIND
{
	TARGET_KIND_SINGLE,
	TARGET_KIND_MULTI,
	TARGET_KIND_SINGLE_CIRCLE,
	TARGET_KIND_MULTI_CIRCLE,
	TARGET_KIND_MULTI_RECT,
	TARGET_KIND_MULTI_ARC,
	// 080602 LUJ, ���� ���� ��ų
	TARGET_KIND_WORLD,
	// 080708 LUJ, ���� ��� ���� ��ų
	TARGET_KIND_MULTI_MONSTER,
	TARGET_KIND_MAX,
};

// 080602 LUJ, ��ų ���� ���� Ÿ��. Target.h���� �Űܿ�
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

// 080602 LUJ, ��ų ���. Ŭ���̾�Ʈ�� ���� ������ ��Ÿ���� ���� [cc]skill/server/manager/skillManager.h���� �Űܿ�
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
