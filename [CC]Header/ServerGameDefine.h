#ifndef __SERVERGAMEDEFINE_H__
#define __SERVERGAMEDEFINE_H__



#define MAX_TOTAL_PLAYER_NUM		500
#define MAX_TOTAL_PET_NUM			200
#define MAX_TOTAL_MONSTER_NUM		1000
#define MAX_TOTAL_BOSSMONSTER_NUM	1
#define MAX_TOTAL_NPC_NUM			50
#define MAX_MAPOBJECT_NUM			20



#define GRID_BIT 5


extern DWORD gCurTime;
extern DWORD gTickTime;

// Â°Ã¦Ã‡Ã¨Ã„Â¡ & Â¾Ã†?ÃŒÃ…Ã› & ÂµÂ· ÂµÃ¥Â¶Ã¸?Â² ÃÂ¶?Ã½ - RaMa 04.10.16 
extern float gExpRate;
extern float gAbilRate;
extern float gItemRate;
extern float gMoneyRate;
//
extern float gDamageReciveRate;		// Â¹ÃÂ´Ã‚ ÂµÂ¥Â¹ÃŒÃÃ¶
extern float gDamageRate;			// ÃÃ–Â´Ã‚ ÂµÂ¥Â¹ÃŒÃÃ¶
extern float gNaeRuykRate;			// Â³Â»Â·Ã‚Â¼Ã’Â¸Ã°
extern float gUngiSpeed;			// Â¿Ã®Â±Ã¢ÃÂ¶Â½Ã„ Â½ÂºÃ‡Ã‡ÂµÃ¥
extern float gPartyExpRate;			// Ã†Ã„Ã†Â¼Â°Ã¦Ã‡Ã¨Ã„Â¡
extern float gGetMoney;				// Â¾Ã²Â´Ã‚ÂµÂ·?Ã‡ Â¹Ã¨Â¼Ã¶
extern float gSkillExpRate;	// ¹«°ø°æÇèÄ¡ ¹è¼ö
// Etc
extern float gDefence;				// Â¹Ã¦Â¾Ã®Â·Ã‚

extern float gEventRate[];
extern float gEventRateFile[];

#define _SERVER_RESOURCE_FIELD_			// Â¼Â­Â¹Ã¶Â¸Â¸?Â» Â±Â¸ÂºÃÃ‡ÃÂ´Ã‚ Â¿ÂµÂ¿Âª

#define START_LOGIN			0
#define CHANGE_LOGIN		1

#define MONSTERGROUPUNIT	2
#define NPCGROUPUINT		1


#define OBJ_REGEN_START_INDEX	100001

enum SERVER_KIND
{
	ERROR_SERVER,
	DISTRIBUTE_SERVER,
	AGENT_SERVER,
	MAP_SERVER,
	CHAT_SERVER,
	MURIM_SERVER,
	MONITOR_AGENT_SERVER,
	MONITOR_SERVER,
	MAX_SERVER_KIND,
};
#define MAX_IPADDRESS_SIZE	16
#define MAX_MAP_NUM		100
#define MAX_SERVER_COUNT	30

enum CHEAT_LOG
{
	eCHT_Item,
	eCHT_Money,
	eCHT_Hide,
	eCHT_AddSkill,
	eCHT_SkillLevel,
	eCHT_LevelUp,
/*¾îºô¸®Æ¼ »èÁ¦ - 	eCHT_AbilityExp,*/
	eCHT_Str,
	eCHT_Dex,
	eCHT_Vit,
	eCHT_Wis,
	eCHT_Int,
};

enum eBOSSEVENT
{
	eBOSSEVENT_LIFE,
};

enum ebossstate
{	
	//////////////////////////////////////////////////////////////////////////
	//NORMAL STATE
	eBossState_Stand,
	eBossState_WalkAround,
	eBossState_Pursuit,
	eBossState_RunAway,
	eBossState_Attack,

	//////////////////////////////////////////////////////////////////////////
	//EVENT STATE
	eBossState_Recover,
	eBossState_Summon,
			
	eBossState_Max,
};

enum ebossaction
{
	eBOSSACTION_RECOVER = 1,
	eBOSSACTION_SUMMON,
};

enum ebosscondition
{
	eBOSSCONDITION_LIFE = 1,	
};

//---KES PUNISH  //ÁÖÀÇ: ¾Æ·¡ÀÇ µÎ enumÀÇ ¹øÈ£°ªÀº DB·Î ÀúÀåÀÌ µÈ´Ù. ¼ø¼­¸¦ ¹Ù²ÙÁö ¸¶½Ã¿À.
enum ePunishKind			
{
	ePunish_Login,
	ePunish_Chat,
	ePunish_Trade,
	ePunish_AutoNote,		//---¿ÀÅä³ëÆ® »ç¿ëÁ¦ÇÑ
	ePunish_Max,
};

enum ePunishCountKind
{
	ePunishCount_NoManner,
	ePunishCount_TradeCheat,		//---±³È¯ ³ëÁ¡µî »ç±â
	ePunishCount_AutoUse,		//---¿ÀÅä»ç¿ë
	ePunishCount_Max,
};
//-------------


#endif //__SERVERGAMEDEFINE_H__
