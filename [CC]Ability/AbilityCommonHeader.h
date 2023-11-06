/**************本资源来自互联网 开发者中心提示禁商业 仅学习*******************/



/*************站长 星期八 感谢广大用户对开发者中心游戏资源社区的大力支持*****************/
#ifndef _ABILITYHEADER_
#define _ABILITYHEADER_

WORD GetAbilityIndex_WithKindPosition(BYTE Kind,BYTE Position);
void GetKindPosition_WithAbilityIndex(WORD AbilityIndex,BYTE* pOutKind,BYTE* pOutPosition);
BYTE GetAbilityLevel_WithDBValue(BYTE DBValue);
// 06. 01 捞康霖 - 窜绵芒 函版
// 捞 窃荐甫 荤侩窍瘤 臼绰促
//WORD GetAbilityQuickPosition(BYTE DBValue);

#define ABILITYUPDATE_CHARACTERLEVEL_CHANGED			0x00000001
#define ABILITYUPDATE_ABILITYUPITEM_USE					0x00000002
#define ABILITYUPDATE_ABILITYLEVEL_CHANGED				0x00000004
#define ABILITYUPDATE_ABILITYEXP_CHANGED				0x00000008
#define ABILITYUPDATE_ABILITYQUICK_CHANGED				0x00000010
#define ABILITYUPDATE_ABILITYINITED						0x00000020
#define ABILITYUPDATE_ABILITYLEVEL_CHANGEDBYSHOPITEM	0x00000100
#define ABILITYUPDATE_CHARACTERSTAGE_CHANGED			0x00000200

enum eAbilityGradeUpMethod
{
	eAGM_OnOff,		//0: 1雀己 己厘屈 : 茄锅 荐访窍咯 劳洒搁 瓤苞甫 啊瘤霸 登哥 弊 捞惑狼 瓤苞刘柳篮 绝促. (On/Off屈)
	eAGM_GradeUp,	//1: 殿鞭 己厘屈 : 漂扁摹甫 荤侩窍咯 拌加秦辑 己厘秦 唱哎荐 乐绰 屈怕
	eAGM_Restrict,	//2: 力茄 己厘屈 : 漂沥 漂扁啊 秦寸 饭骇俊 档崔窍扁 傈俊绰 漂扁甫 己厘矫懦荐 绝绰 屈怕
};

enum eAbilityUseType
{
	eAUT_Use,		//0: 1雀己 鸥涝
	eAUT_Passive,	//1: 菩矫宏 鸥涝
	eAUT_Active,	//2: ON/OFF
};

enum eAbilityGroupKind
{
	eAGK_Battle,
	eAGK_KyungGong,
	eAGK_Society,
	eAGK_Job,

	eAGK_Max,
};

#define ABILITYLEVEL_DB_KEY		'A'

#define ABILITYQUICKPOSITION_NULL	' '
#define ABILITYQUICKPOSITION_KEY	'A'

#define ABILITY_MAX_LEVEL		30

#define MAX_TOTAL_ABILITY	(MAX_ABILITY_NUM_PER_GROUP*eAGK_Max)
#define MAX_ABILITY_NUM_PER_GROUP	40
#define MAX_ABILITY_LEVEL 30

#define ABILITY_STARTINDEX_BATTLE		100
#define ABILITY_STARTINDEX_KYUNGGONG	200
#define ABILITY_STARTINDEX_SOCIETY		300

#define ABILITY_STARTINDEX_JOB			400

#define ABILITY_STARTINDEX_INTERVAL		100



struct ABILITY_STATS
{
	DWORD PhyAttackUp[WP_MAX];
	ATTRIBUTE_VAL<float> AttAttackUp;
	ATTRIBUTE_VAL<float> AttRegistUp;
	DWORD DefenceUp;

	DWORD LifeUp;
	DWORD NaeRyukUp;
	DWORD ShieldUp;
	
	DWORD UngiUpVal;

	// Change Stage
	DWORD StatGen;	// 辟
	DWORD StatMin;	// 刮
	DWORD StatChe;	// 眉
	DWORD StatSim;	// 缴
	float Kyunggong;	// 版脚过
	WORD  KyunggongLevel;
	float fNoAttrib;	// 公倾傍
	DWORD SkillDamage;	// 公傍单固瘤
	DWORD CriticalDamage;	// 老拜单固瘤
	DWORD dwTitanRidingPlusTime;	//2007. 11. 6. CBH - 鸥捞藕 啪铰 矫埃

	DWORD GetPhyAttackUp(WORD WeaponKind)
	{
		return PhyAttackUp[WeaponKind-1];
	}
	void SetPhyAttackUp(WORD WeaponKind,DWORD UpVal)
	{
		PhyAttackUp[WeaponKind-1] = UpVal;
	}

	void Clear()
	{
		memset(this,0,sizeof(ABILITY_STATS));
	}
};

struct ABILITY_CALC
{

	DWORD	dwPhyAttack;	// 拱府 傍拜
	float	fAttribAttack;	// 加己 傍拜
	float	fAttribRegist;	// 加己 历亲
	DWORD	dwLife;			// 积疙仿
	DWORD	dwDeffence;		// 规绢仿
	DWORD	dwNearyuk;		// 郴仿
	DWORD	dwShield;		// 龋脚
	DWORD	dwUngi;			// 款扁
	DWORD	dwStat;			// 辟, 刮, 眉, 缴
	float	fKyunggong;		// 版傍胶乔靛
	float	fNoAttrib;		// 公倾傍
	DWORD	dwSkillDamage;	// 公傍单固瘤
	DWORD	dwCriticalDamage;	// 老拜单固瘤
	DWORD	dwTitanRidingPlusTime;	//2007. 11. 6. CBH - 鸥捞藕 啪铰 矫埃
};

enum eABILITY_USE_KIND
{
	eAUK_KYUNGGONG = 1,
	eAUK_JOB,

	eAUK_PHYATTACK,
	eAUK_ATTRIB,

	eAUK_MAXLIFE,
	eAUK_DEFENCE,
	eAUK_MAXNEARYUK,
	eAUK_MAXSHIELD,
	eAUK_UNGISPEED,

	eAUK_SOCIETY = 10,	//捞扒 10捞扼绊 沥沁促.
	eAUK_PET	= 11,	//11 abilitybaseinfo.bin -> effect_kind
};

enum eABILITY_USE_KIND_STAGE	// 傈流矫 瓤苞甸 11~100
{
	eAUK_STAT1				= 11,	// 辟
	eAUK_STAT2				= 12,	// 刮
	eAUK_STAT3				= 13,	// 眉
	eAUK_STAT4				= 14,	// 缴
	eAUK_KYUNGGONG_SPEED	= 15,	// 版脚过
	eAUK_JUSOOL				= 16,	// 林贱
	eAUK_NOATTRIB			= 17,	// 公倾傍
	eAUK_SKILL_DAMAGE		= 18,	// 公傍单固瘤
	eAUK_CRITICAL_DAMAGE	= 19,	// 老拜单固瘤
};

enum eABILITY_USE_JOB
{
	eAUKJOB_Ungijosik = 101,	// 款扁炼侥
	eAUKJOB_Vimu = 102,			// 厚公脚没
	eAUKJOB_Party = 103,		// 规颇芒汲
	eAUKJOB_Guild = 104,		// 巩颇芒汲
	eAUKJOB_Deal = 105,			// 芭贰
	eAUKJOB_StreetStall = 106,  // 畴痢惑俺汲

	eAUKJOB_Restraint = 108,	// 斑力
	eAUKJOB_Attack = 109,		// 傍拜

	eAUKJOB_StallFind = 110,	// 畴痢惑 八祸 by Stiner(8)
	eAUKJOB_AutoNote = 111,	// 坷配畴飘
	eAUKJOB_PartyFind = 112,	//2008. 6. 4. CBH - 规颇 茫扁 眠啊

	eAUKJOB_TitanExpert = 120,	//鸥捞藕 槛访

	eAUKJOB_Upgrade = 201,		//殿鞭诀
	eAUKJOB_Mix = 202,			//炼钦
	eAUKJOB_Reinforce = 203,	//碍拳
	eAUKJOB_Dissolve = 204,		//盒秦
};

enum eABILITY_USE_PET
{
	eAUKPET_State	= 150,
	eAUKPET_Inven	= 151,
	eAUKPET_Skill	= 152,
	eAUKPET_Rest	= 153,
	eAUKPET_Seal	= 154,
};

enum eABILITY_USE_SOCIETY
{
	eAUKJOB_Bow		= 301,		// 牢荤(器鼻)
	eAUKJOB_Happy	= 302,		// 扁惠
	eAUKJOB_Sad		= 303,		// 浇悄
	eAUKJOB_Yes		= 304,		// 编沥

	eAUKJOB_No		= 305,		// 何沥
	eAUKJOB_Sit		= 306,		// 旧扁/老绢唱扁
//	eAUKJOB_Stand	= 307,		// 老绢唱扁
	eAUKJOB_BigBow	= 307,		// 拜侥牢荤
};

enum eAbilityIconState
{
	// 06. 01. 绢呼府萍芒 牢磐其捞胶 荐沥 - 捞康霖
	// 焊捞绰 炼扒 函版
	eAIS_NotUsable,
	eAIS_NotAvailable,
	eAIS_OnlyVisible,
	eAIS_Usable,
};


enum AbilityAcquireKind
{
	eAAK_Item,				//酒捞袍阑 荤侩窍咯 漂扁甫 掘澜
	eAAK_Quest,				//涅胶飘甫 烹窍咯 漂扁甫 掘澜
	eAAK_OtherAbility,		//漂沥 漂扁啊 老沥 殿鞭 捞惑捞 档崔窍搁 掘绰促.
	eAAK_CharacterLevel,	//某腐磐狼 饭骇捞 漂沥窜拌俊 档崔窍搁 嚼垫
	eAAK_CharacterStage,	//傈流沁阑版快 嚼垫
	
	eAAK_Max,

};


#define ABILITYINDEX_UNGI	401	//款扁炼侥
#define ABILITYINDEX_VIMU	402	//厚公脚没
#define ABILITYINDEX_PARTY	403	//规颇芒汲
#define ABILITYINDEX_GUILD	404	//巩颇芒汲
#define ABILITYINDEX_EXCHANGE	405	//芭贰
#define ABILITYINDEX_STALL	406	//畴痢惑俺汲


#define ABILITYINDEX_ITEMUPGRADE	407	//酒捞袍殿鞭诀
#define ABILITYINDEX_ITEMMIX		408	//酒捞袍炼钦

#define ABILITYINDEX_ITEMREINFORCE	409	//酒捞袍碍拳
#define ABILITYINDEX_ITEMDISSOLVE	410	//酒捞袍盒秦
#define ABILITYINDEX_LEADERSHIP		411	//瘤档仿
#define	ABILITYINDEX_RESTRAINT		412	// 斑力

/*DB 累诀饶
#define ABILITYINDEX_PETSTATE		501	//脐惑怕沥焊
#define ABILITYINDEX_PETINVEN		502	//脐牢亥配府
#define ABILITYINDEX_PETSKILL		503	//脐胶懦

#define ABILITYINDEX_PETREST		504	//脐绒侥惑怕
#define ABILITYINDEX_PETSEAL		505	//脐豪牢
*/
//烙矫
#define ABILITYINDEX_PETSTATE		450	//脐惑怕沥焊
#define ABILITYINDEX_PETINVEN		451	//脐牢亥配府
#define ABILITYINDEX_PETSKILL		452	//脐胶懦
#define ABILITYINDEX_PETREST		453	//脐绒侥惑怕
#define ABILITYINDEX_PETSEAL		454	//脐豪牢

#define ABILITYINDEX_TACTIC_HWA		122	//傈捧-柳过(林贱)
#define ABILITYINDEX_TACTIC_GEUK	128	//傈捧-柳过(林贱)
#define ABILITYINDEX_TACTIC_MUSSANG	129 //公街


#endif


