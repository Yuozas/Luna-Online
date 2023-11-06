#ifndef __GAMERESOURCESTRUCT_H__
#define __GAMERESOURCESTRUCT_H__

class cActiveSkillInfo;

//#define MAX_ITEMNAME_LENGTH	30

//-----------------------------------------------------------------------
//	직업별 기본 포인트 수치와 레벨업시 포인터 가중치
//-----------------------------------------------------------------------
struct CharBasicStats
{
	WORD	Str;						// 근골
	WORD	Dex;							// 민첩
	WORD	Vit;							// 체력
	WORD	Int;
	WORD	Wis;						// 심맥
	//새로운 기획 수정으로 없어짐
	//BYTE	BonusPoint;						// 보너스 포인트 general-15
};
class RECALLINFO
{
public:
	DWORD	RecallType;
	DWORD	RecallID;
	DWORD	RecallMonsterNum;
	DWORD	RecallRange;
};
class ATTACKPATTERNINFO
{
public:
	WORD	AttackPatternNum;
	WORD	* AttackPattern;		// 0->1->2->2->1->....
};
class CAttackPatternInfo
{
public:
	ATTACKPATTERNINFO * pAttackPatternInfo;
	virtual ~CAttackPatternInfo()
	{
		delete [] pAttackPatternInfo;
	}
	void Init(WORD wTotalNum)
	{
		MaxAttackNum = wTotalNum;
		pAttackPatternInfo = new ATTACKPATTERNINFO[MaxAttackNum];
		memset(pAttackPatternInfo, 0, sizeof(ATTACKPATTERNINFO)*MaxAttackNum);
	}
	void InitPattern(WORD PNum, WORD wTotalNum)
	{
		pAttackPatternInfo[PNum].AttackPatternNum = wTotalNum;
		pAttackPatternInfo[PNum].AttackPattern = new WORD[wTotalNum];
		memset(pAttackPatternInfo[PNum].AttackPattern, 0, sizeof(WORD)*wTotalNum);
	}
	void SetVal(WORD PNum, WORD PIdx, WORD PVal)
	{
		pAttackPatternInfo[PNum].AttackPattern[PIdx] = PVal;
	}
	WORD		MaxAttackNum;
};
class CRecallInfo
{
public:
	RECALLINFO * pRecallInfo;

	CRecallInfo(){ memset(this, 0, sizeof(CRecallInfo)); }
	virtual ~CRecallInfo()
	{
		if(pRecallInfo)
		delete [] pRecallInfo;
	}
	void Init(DWORD nRecall)
	{
		MaxRecallNum = nRecall;
		pRecallInfo = new RECALLINFO[MaxRecallNum];
		memset(pRecallInfo, 0, sizeof(RECALLINFO)*MaxRecallNum);
	}
	
	RECALLINFO * GetRecallInfo(DWORD dwIdx)
	{
		ASSERT(dwIdx < MaxRecallNum);
		return &pRecallInfo[dwIdx];
	}
	void SetRecallType(DWORD dwIdx, DWORD dwType)
	{
		pRecallInfo[dwIdx].RecallType = dwType;
	}
	void SetRecallID(DWORD dwIdx, DWORD dwID)
	{
		pRecallInfo[dwIdx].RecallID = dwID;
	}
	void SetRecallMonsterNum(DWORD dwIdx, DWORD dwNum)
	{
		pRecallInfo[dwIdx].RecallMonsterNum = dwNum;
	}
	void SetRecallRange(DWORD dwIdx, DWORD dwRange)
	{
		pRecallInfo[dwIdx].RecallRange = dwRange;
	}
protected:
	DWORD	MaxRecallNum;
	
};
//-----------------------------------------------------------------------
// 몬스터 리스트 정보
//-----------------------------------------------------------------------

class BASE_MONSTER_LIST
{
public:
	
	WORD		MonsterKind;			// 번호
	WORD		ObjectKind;				// Object번호 32 일반 33 보스 35 필드보스 36 필드보스부하
	char		Name[MAX_MONSTER_NAME_LENGTH +1];				// 몬스터이름	
//	char		EngName[MAX_NAME_LENGTH+1];			// 영어이름
	LEVELTYPE	Level;					// 몬스터레벨	
//	int			MoveDramaNum;				// 이동연출 번호	
//	DWORD		MotionID;
	char		ChxName[32];			// 몬스터 CHX_Index	
	float		Scale;					// 몬스터 크기	
	DWORD		Gravity;				// 몬스터 비중
	float		Weight;					// 몬스터 무게	
	DWORD		MonsterRadius;
	DWORD		Life;					// 생명력	
	// 080530 LYW --- GameResourceStruct : Modified a data type of monster experience to DWORD.
	//EXPTYPE		ExpPoint;				// 경험치
	DWORD		ExpPoint ;
	WORD		Tall;					// 키

	WORD		Accuracy;
	WORD		Avoid;

	WORD		AttackPhysicalMin;// 공격 최소물리 공격력	
	WORD		AttackPhysicalMax;// 공격 최대물리 공격력
	WORD		AttackMagicalMin;// 공격 최소마법 공격력	
	WORD		AttackMagicalMax;// 공격 최대마법 공격력
	WORD		CriticalPercent;// 크리티컬 퍼센트
	WORD		PhysicalDefense;// 물리 방어력	
	WORD		MagicalDefense;// 마법 방어력	
//	ATTRIBUTEREGIST AttribResist;		// 속성저항력

	WORD		WalkMove;// 몬스터걷기이동력	
	WORD		RunMove;// 몬스터뛰기이동력	
	WORD		RunawayMove;// 몬스터도망이동력

	int			Damage1DramaNum;// 맞음 1 연출 번호	
	int			Die1DramaNum;// 죽음 1 연출 번호	
	int			Die2DramaNum;// 죽음 2 연출 번호	

	DWORD		StandTime;
	BYTE		StandRate;
	BYTE		WalkRate;
	BYTE		RunRate;
	DWORD		DomainRange;					// 영역 범위 : 반지름
	DWORD		PursuitForgiveTime;
//	DWORD		PursuitForgiveStartTime;		//not inclue in txt list
	DWORD		PursuitForgiveDistance;	

	BOOL		bForeAttack;					// 선공격
	DWORD		SearchPeriodicTime;				// 탐색 주기
	WORD		TargetSelect;					// 타겟 선택 : FIND_CLOSE, FIND_FIRST
	DWORD		SearchRange;
	DWORD		SearchAngle;
	WORD		TargetChange;

	WORD		AttackKind;
	DWORD		AttackNum;
	DWORD *		AttackIndex;
	DWORD *		AttackRate;
	cActiveSkillInfo ** SkillInfo;	
	
	
	WORD		InitHelpType;		// 0 : no request, 1~~
	BOOL		bHearing;
	DWORD		HearingDistance;

	WORD		SpecialType;
	WORD		MonsterType;
	WORD		MonsterRace;
	WORD		MonsterAttribute;

};

class SMONSTER_LIST : public BASE_MONSTER_LIST
{
public:
	/*
	virtual ~SMONSTER_LIST()
	{
		for(int i = 0 ; i < MaxAttackNum ; ++i )
			delete [] AttackNumPattern[i];
		delete [] AttackNumPattern;
		
		delete [] MaxAttackIndexNum;
	}
	WORD		MaxAttackNum;
	WORD		* MaxAttackIndexNum;
	WORD		** AttackNumPattern;
	*/

	CAttackPatternInfo AttackPatternClass;
	CRecallInfo RecallInfoClass;
};

// 070423 LYW --- GameResourceStruct : Add struct for drip item.
struct MONSTER_DROPITEM
{
	DWORD wItemIdx;
	DWORD dwDropPercent;
	BYTE byCount ;
} ;

class MONSTEREX_LIST : public BASE_MONSTER_LIST
{
public:
	// 070423 LYW --- GameResourceStruct : Modified drop money part.
	/*
	MONEYTYPE MinDropMoney;				// 최소돈 액수	
	MONEYTYPE MaxDropMoney;				// 최대 돈 액수	
	*/

	// 070419 LYW --- GameResourceStruct : Add new drop item part.
	MONEYTYPE dropMoney ;
	DWORD	  dwMoneyDropRate ;
	//MONSTER_ITEM dropItem[eDROPITEMKIND_MAX-1] ;
	MONSTER_DROPITEM dropItem[eDROPITEMKIND_MAX-1] ;
	
	// 070423 LYW --- GameResourceStruct : Delete not use memeber.
	//WORD DropRate[eDROPITEMKIND_MAX];
	
	// 서버에서 사용
	// 초기화가 필요!!
	// 070423 LYW --- GameResourceStruct : Delete not use memeber.
	//WORD CurDropRate[eDROPITEMKIND_MAX];


	// 070423 LYW --- GameResourceStruct : Modified drop item part.
	/*
	WORD ItemListNum1;				// 해당아이템1리스트표번호
	WORD ItemListNum2;				// 해당아이템2리스트표번호 - 추가 
	WORD ItemListNum3;				// 해당아이템3리스트표번호 - 추가 
	// 070419 LYW --- GameResourceStruct : Add more item list.
	WORD ItemListNum4 ;
	WORD ItemListNum5 ;
	*/


	// 070423 LYW --- GameResourceStruct : Delete not use memeber.
	//BYTE MaxDropItemNum;			// 최대아이템 갯수

	float fRunawayLifeRate;			// 생명력 확률 0~100
	WORD RunawayRate;				// 도망 확률
	WORD RunawayHelpRate;			// 도망 도움확률
	WORD RunawayDistance;			// 도망 거리

//	WORD Talk1;						// 대사 1
//	WORD Talk2;				
//	WORD Talk3;

	WORD HelpRequestCondition;
	WORD HelpRequestDistance;
	WORD HelpRequestTargetNum;

//	WORD RecallCondition;
//	WORD RecallObjectID;
//	WORD RecallObjectNum;	
};
//////////////////////////////////////////////////////////////////////////
// 몬스터 chx 파일의 리스트
//////////////////////////////////////////////////////////////////////////
typedef struct _CHXLIST
{
	StaticString FileName;
}CHXLIST;

//-----------------------------------------------------------------------
// 맵전환 좌표기억 구조체
//-----------------------------------------------------------------------
typedef struct _MAPCHANGE_INFO
{
	WORD Kind;

	// 071214 LUJ, 맵 이름이 MAX_NAME_LENGTH 길이를 초과하여 길이를 새로 지정 
	char CurMapName[ MAX_MONSTER_NAME_LENGTH + 1 ];
	char ObjectName[ MAX_MONSTER_NAME_LENGTH + 1 ];
	//char CurMapName[MAX_NAME_LENGTH+1];
	//char ObjectName[MAX_NAME_LENGTH+1];

	WORD CurMapNum;
	WORD MoveMapNum;
	VECTOR3 CurPoint;
	VECTOR3 MovePoint;
	WORD chx_num;
	
	// 방향설정
}MAPCHANGE_INFO;

//-----------------------------------------------------------------------
// LogIn 좌표기억 구조체
//-----------------------------------------------------------------------
typedef struct _LOGINPOINT_INFO
{
	WORD Kind;
	char MapName[MAX_NAME_LENGTH+1];
	WORD MapNum;
	BYTE MapLoginPointNum;
	VECTOR3 CurPoint[10];
	WORD chx_num;

	// 방향설정
}LOGINPOINT_INFO;

//-----------------------------------------------------------------------
// NPC 정보
//-----------------------------------------------------------------------
struct NPC_LIST
{
	WORD NpcKind;			// Npc 종류
	char Name[MAX_NPC_NAME_LENGTH+1];			// Npc 이름
	WORD ModelNum;			// MonsterChxList에서의 번호
	WORD JobKind;			// 직업종류별구분
	float Scale;			// Npc 크기
	WORD Tall;				// 키
	BOOL ShowJob;
};

//-----------------------------------------------------------------------
// NPC 리젠
//-----------------------------------------------------------------------
typedef struct _NPC_REGEN
{

	_NPC_REGEN():dwObjectID(0),NpcKind(0)
	{
		Pos.x = Pos.y = Pos.z = 0;
	}
	DWORD	dwObjectID;		// 신규 추가 taiyo
	MAPTYPE	MapNum;			// Map번호
	WORD	NpcKind;		// NpcKind
	char	Name[MAX_NAME_LENGTH+1];		// Npc 이름
	WORD	NpcIndex;		// NpcIndex(고유번호로 사용)
	VECTOR3 Pos;			// 몬스터 위치
	float	Angle;			// 몬스터 각도
}NPC_REGEN;


//-----------------------------------------------------------------------
// MOD 리스트 
//-----------------------------------------------------------------------
struct MOD_LIST
{
	MOD_LIST()
	{
		MaxModFile = 0;
		ModFile = NULL;
	}
	~MOD_LIST()
	{
		if(MaxModFile == 0)
			return;
		SAFE_DELETE_ARRAY(ModFile);
	}
	DWORD MaxModFile;
	StaticString* ModFile;
	StaticString BaseObjectFile;	
};

//---------------<아이템 통합 리스트>------------------------------------------------------
/*
struct ITEM_INFO
{
	DWORD ItemIdx;			//		Item_Index	
	char ItemName[MAX_ITEMNAME_LENGTH+1];		//		Item_name
	WORD ItemTooltipIdx;	//		Item설명 툴팁 index
	WORD Image2DNum;		//		2D_Image	
	WORD ItemKind;			//		아이템종류	"0 : 영약 1 : 무공서 2 : 장착아이템 3 : 기타"
	DWORD BuyPrice;			//		구입가격	
	DWORD SellPrice;		//		판매가격	
	WORD Rarity;			//		희소가치	
	WORD WeaponType;		//		무기유형	장착 / 무공서
	WORD Str;				//		근골+(영구)	영약과 장착 아이템에 공통사용
	WORD Dex;			//		민첩+(영구)	영약과 장착 아이템에 공통사용
	WORD Vit;			//		체력+(영구)	영약과 장착 아이템에 공통사용
	WORD Int;
	WORD Wis;			//		심맥+(영구)	영약과 장착 아이템에 공통사용
	DWORD Life;				//		생명력+(영구)	영약과 장착 아이템에 공통사용
	WORD Mana;			//		내력+(영구)	영약과 장착 아이템에 공통사용
	WORD LimitRace;
	WORD LimitJob;			//		장착가능 직업	장착 / 무공서에만 사용 / 상승무공-화경, 극마
	WORD LimitGender;		//		장착가능 남녀	장착 / 무공서에만 사용
	LEVELTYPE LimitLevel;		//		장착가능 레벨	장착 / 무공서에만 사용
	WORD LimitStr;		//		장착가능 근골	장착 / 무공서에만 사용
	WORD LimitDex;		//		장착가능 민첩	장착 / 무공서에만 사용
	WORD LimitVit;		//		장착가능 체력	장착 / 무공서에만 사용
	WORD LimitInt;		//		장착가능 심맥	장착 / 무공서에만 사용
	WORD LimitWis;		//		장착가능 심맥	장착 / 무공서에만 사용

	WORD ItemGrade;			//	장착아이템	아이템 등급	
	WORD RangeType;			//				거리유형	
	WORD EquipKind;			//				장착종류	
	WORD Part3DType;		//				3D파츠번호	
	WORD Part3DModelNum;	//				3D파츠모델번호	
	WORD MeleeAttackMin;	//				근거리 최소공격력
	WORD MeleeAttackMax;	//				근거리 최대공격력
	WORD RangeAttackMin;	//				원거리 최소공격력
	WORD RangeAttackMax;	//				원거리 최대공격력
	WORD MagicAttackMin;	//				마법 최소공격력
	WORD MagicAttackMax;	//				마법 최대공격력
	WORD CriticalPercent;	//				크리티컬 퍼센트

	WORD PhyDef;			//				물리 방어력+ 
	WORD MagicDef;
	WORD Plus_SkillIdx;	//				특정 무공+ index
	WORD Plus_Value;		//				특정 무공+ 정도
	WORD AllPlus_Kind;		//				무공 유형별 무공+ 
	WORD AllPlus_Value;		//				무공 유형별 무공+ 정도

	WORD SkillNum;			//	무공서	연결 무공  번호
	WORD SkillType;		//			무공유형

	WORD LifeRecover;		//	영약	생명력+(회복) 고정수치	영약 전용
	float LifeRecoverRate;		//			생명력+(회복)  	영약 전용
	WORD ManaRecover;	//			내력+(회복) 고정수치	영약 전용
	float ManaRecoverRate;	//			내력+(회복)    	영약 전용

	BOOL Trade;
	WORD ItemType;			//	기타	아이템 속성	=>퀘스트, 비극서, 재료
	WORD wItemAttr;			// 아이템속성(화, 수, 목, 금, 토) => 일본 로컬관련
	WORD wAcquireSkillIdx1;	// 필요한 스킬 인덱스 => 일본로컬관련
	WORD wAcquireSkillIdx2;	// 필요한 스킬 인덱스 => 일본로컬관련
	WORD wDeleteSkillIdx;	// 지울 스킬 인덱스 => 일본로컬관련
};
*/
struct ITEM_INFO
{
	DWORD ItemIdx;			//		Item_Index	
	char ItemName[MAX_ITEMNAME_LENGTH+1];		//		Item_name
	DWORD ItemTooltipIdx;	//		Item설명 툴팁 index
	WORD Image2DNum;		//		2D_Image	
	WORD Part3DType;		//				3D파츠번호	
	WORD Part3DModelNum;	//				3D파츠모델번호	

	WORD LimitRace;
	WORD LimitGender;		//		장착가능 남녀	장착 / 무공서에만 사용
	WORD LimitLevel;		//		장착가능 레벨	장착 / 무공서에만 사용

	WORD Shop;
	WORD Stack;
	WORD Trade;
	WORD Deposit;
	WORD Sell;
	WORD Decompose;
	WORD Repair;
	BOOL IsEnchant;		// 업그레이드 가능 여부
	WORD Improvement;

	DWORD EnchantValue;	// 업글 성공 확률 변수
	DWORD EnchantDeterm;	// 업글 단계마다 부여되는 수치

	DWORD DecomposeIdx;
	DWORD Time;
	DWORD BuyPrice;			//		구입가격	
	DWORD SellPrice;		//		판매가격	
	WORD Grade;
	DWORD Durability;

	int PhysicAttack;
	int MagicAttack;
	int PhysicDefense;
	int MagicDefense;

	WORD Category;
	WORD EquipType;
	WORD EquipSlot;
	WORD WeaponAnimation;
	WORD WeaponType;
	WORD ArmorType;
	WORD AccessaryType;

	int ImprovementStr;			
	int ImprovementDex;			
	int ImprovementVit;			
	int ImprovementInt;
	int ImprovementWis;			
	int ImprovementLife;			
	int ImprovementMana;		
	int ImprovementSkill;

	DWORD Skill;

	WORD SupplyType;
	DWORD SupplyValue;

	/// 상황별 예외
	BOOL	Battle;
	BOOL	Peace;
	BOOL	Move;
	BOOL	Stop;
	BOOL	Rest;

//	BOOL	bSeal;
	WORD	wSeal;
	int		nTimeKind;
	DWORD	dwUseTime;

	DWORD	RequiredBuff;

	enum eKind
	{
		eKindNormal,
		eKindHighClass,
		eKindMasterPiece,
		eKindRare,
		eKindUnique,
		eKindSetNormal,
		eKindSetHighClass,
		eKindSetMasterPiece,
		eKindSetRare,
		eKindSetUnique,
	}
	kind;

	DWORD	dwBuyFishPoint; //		구입물고기포인트
//	WORD EquipSlot;
//	eItemPointType wPointType;
//	DWORD dwPointTypeValue1;
//	DWORD dwPointTypeValue2;
//	DWORD dwType; // Item Type
//	DWORD dwTypeDetail; // Item Type Detail
//	WORD Part3DType;
};

class ITEM_REINFORCE_INFO
{
public:
	WORD			wItemIdx;
	char			szItemName[MAX_ITEMNAME_LENGTH+1];
	LEVELTYPE		ItemLevel;
	WORD			ReinforceType;
	float			w;
};


//---아아템강화
struct sITEM_REINFORCE_INFO
{
	DWORD			wItemIdx;
	WORD			ReinforceType;
	float			fMaxReinforce;
	float			fw;
};
//-----------------------------------
struct sITEM_RAREREINFORCE_INFO	: public sITEM_REINFORCE_INFO
{
	WORD			wMaterialGravity;
};

//---아아템분해
struct sITEM_DISSOLUTIONITEM
{
	DWORD			wUnitItemIdx;					//분해될 아이템 인덱스
	WORD			wUnitItemNum;						//분해될 아이템 개수
};

struct sITEM_DISSOLUTION_INFO
{
	DWORD					wDissolutionItemIdx;	//분해할 베이스아이템 인덱스
	WORD					wUnitKindNum;			//분해될 아이템 종류 개수
	WORD					wUnitTotalNum;			//분해될 아이템 총 개수
	sITEM_DISSOLUTIONITEM*	pUnitItem;				//분해아이템 인포
};
//-----------------------------------



//-----------------------------------------------------------------------------------------------------------//
//		클라이언트만 사용하는 부분
#ifdef _CLIENT_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//

typedef struct _PRELOAD
{
	StaticString FileName;
}PRELOAD;



struct SETSHOPITEM
{
	DWORD ItemIdx;
	char Name[MAX_ITEMNAME_LENGTH+1];
};


#ifdef _TESTCLIENT_

//trustpak
struct TESTCHARACTER_INFO
{
	BYTE byGender;
	float fX;
	float fY;
	float fZ;
	float fAngle;
};

#define MAX_TEST_CHARACTER_NO 8
//


struct TESTCLIENTINFO {
	TESTCLIENTINFO()
	{
		//trustpak
		memset(this, 0, sizeof(TESTCLIENTINFO));
		//

		LightEffect = 1;
		Effect = 0;
		SkillIdx = 0;
		WeaponIdx = MIN_EQUIPMENTITEM_INDEX+1;
		Gender = GENDER_MALE;
		x = 30000.f;
		z = 30000.f;
		MonsterEffect = 1;
		MonsterNum = 1;
		MonsterKind = 13;
		Map = 17;

		DressIdx = 0;
		HatIdx = 0;
		ShoesIdx = 0;
		HairType = 0;
		FaceType = 0;
		

		CharacterNum = 0;
		BossMonsterNum = 0;
		BossMonsterKind = 1000;
		
	}
	float x,z;

	WORD DressIdx;
	WORD HatIdx;
	WORD ShoesIdx;
	WORD ShieldIdx;

	BYTE HairType;
	BYTE FaceType;

	WORD MonsterKind;

	WORD BossMonsterKind;
	WORD BossMonsterNum;
	char BossInfofile[17];

	int Effect;
	WORD SkillIdx;
	WORD WeaponIdx;
	BYTE Race;
	BYTE Gender;
	BYTE Job;
	int MonsterEffect;
	WORD MonsterNum;
	WORD CharacterNum;
	MAPTYPE Map;

	BOOL LightEffect;
	
	float Width;
	float Height;

	//trustpak
	DWORD				dwTestCharacterNo;
	TESTCHARACTER_INFO	aTestCharacterInfo[MAX_TEST_CHARACTER_NO];
	
	//

};
#endif

//-----------------------------------------------------------------------
// game config 정보 
//-----------------------------------------------------------------------
struct GAMEDESC_INI
{
	GAMEDESC_INI()
	{
		strcpy(DistributeServerIP,"211.233.35.36");
		DistributeServerPort = 400;

		AgentServerPort = 100;
		
		bShadow = MHSHADOW_CIRCLE;
		
		dispInfo.dwWidth = 1024;		//WINDOW SIZE
		dispInfo.dwHeight = 768;
		dispInfo.dwBPS = 4;
		dispInfo.dispType = WINDOW_WITH_BLT;
		//dispInfo.bWindowed = TRUE;
		dispInfo.dwRefreshRate = 70;

		MaxShadowTexDetail = 256;
		MaxShadowNum = 4;

		FramePerSec = 30;
		TickPerFrame = 1000/(float)FramePerSec;

		MasterVolume = 1;
		SoundVolume = 1;
		BGMVolume = 1;
		
		CameraMinAngleX = 0;
		CameraMaxAngleX = 89.f;
		CameraMinDistance = 200.f;
		CameraMaxDistance = 1000.f;

		LimitDay = 0;
		LimitID[0] = 0;
		LimitPWD[0] = 0;

		strWindowTitle[0] = 0;
	}

	float MasterVolume;
	float SoundVolume;
	float BGMVolume;

	char DistributeServerIP[32];
	WORD DistributeServerPort;
	
	WORD AgentServerPort;
	
	DISPLAY_INFO dispInfo;

	BYTE bShadow;
	DWORD MaxShadowNum;
	DWORD MaxShadowTexDetail;

	DWORD FramePerSec;
	float TickPerFrame;

	char MovePoint[64];

	float CameraMinAngleX;
	float CameraMaxAngleX;
	float CameraMinDistance;
	float CameraMaxDistance;

	DWORD LimitDay;
	char LimitID[MAX_NAME_LENGTH+1];
	char LimitPWD[MAX_NAME_LENGTH+1];

	char strWindowTitle[128];
};

struct SEVERLIST
{
	char	DistributeIP[16];
	WORD	DistributePort;
	char	ServerName[64];
	WORD	ServerNo;
	BOOL	bEnter;

	SEVERLIST()
	{
		ServerNo = 1;
		strcpy( DistributeIP, "211.233.35.36" );
		DistributePort = 400;
		strcpy( ServerName, "Test" );
		bEnter = TRUE;
	}
};

struct PET_FRIENDLY_STATE
{
	BYTE	Friendly;
	DWORD	SpeechIndex;
	BYTE	SpeechRate;
	DWORD	EmoticonIndex;
	BYTE	EmoticonRate;
};
//-----------------------------------------------------------------------------------------------------------//
#endif //_CLIENT_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//


//-----------------------------------------------------------------------------------------------------------//
//		서버만 사용하는 부분
#ifdef _SERVER_RESOURCE_FIELD_

//-----------------------------------------------------------------------------------------------------------//
#include "ServerTable.h"
//-----------------------------------------------------------------------
// 캐릭터 IN/OUT POINT 설정
//-----------------------------------------------------------------------
typedef struct _CHARACTERINOUTPOINT
{
	WORD MapNum;
	VECTOR3 MapInPoint[MAX_MAP_NUM];
	VECTOR3 MapOutPoint[MAX_MAP_NUM];
}CHARACTERINOUTPOINT;


//---KES 스킬초기화돈	071207
struct SKILL_MONEY
{
	DWORD SkillIdx;
	DWORD SP;
	DWORD Money;
};

//----------------------------


//-----------------------------------------------------------------------------------------------------------//
#endif //_SERVER_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//
#endif //__GAMERESOURCESTRUCT_H__
