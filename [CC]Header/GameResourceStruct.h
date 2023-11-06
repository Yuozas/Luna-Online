#ifndef __GAMERESOURCESTRUCT_H__
#define __GAMERESOURCESTRUCT_H__

class cActiveSkillInfo;

//#define MAX_ITEMNAME_LENGTH	30

//-----------------------------------------------------------------------
//	������ �⺻ ����Ʈ ��ġ�� �������� ������ ����ġ
//-----------------------------------------------------------------------
struct CharBasicStats
{
	WORD	Str;						// �ٰ�
	WORD	Dex;							// ��ø
	WORD	Vit;							// ü��
	WORD	Int;
	WORD	Wis;						// �ɸ�
	//���ο� ��ȹ �������� ������
	//BYTE	BonusPoint;						// ���ʽ� ����Ʈ general-15
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
// ���� ����Ʈ ����
//-----------------------------------------------------------------------

class BASE_MONSTER_LIST
{
public:
	
	WORD		MonsterKind;			// ��ȣ
	WORD		ObjectKind;				// Object��ȣ 32 �Ϲ� 33 ���� 35 �ʵ庸�� 36 �ʵ庸������
	char		Name[MAX_MONSTER_NAME_LENGTH +1];				// �����̸�	
//	char		EngName[MAX_NAME_LENGTH+1];			// �����̸�
	LEVELTYPE	Level;					// ���ͷ���	
//	int			MoveDramaNum;				// �̵����� ��ȣ	
//	DWORD		MotionID;
	char		ChxName[32];			// ���� CHX_Index	
	float		Scale;					// ���� ũ��	
	DWORD		Gravity;				// ���� ����
	float		Weight;					// ���� ����	
	DWORD		MonsterRadius;
	DWORD		Life;					// �����	
	// 080530 LYW --- GameResourceStruct : Modified a data type of monster experience to DWORD.
	//EXPTYPE		ExpPoint;				// ����ġ
	DWORD		ExpPoint ;
	WORD		Tall;					// Ű

	WORD		Accuracy;
	WORD		Avoid;

	WORD		AttackPhysicalMin;// ���� �ּҹ��� ���ݷ�	
	WORD		AttackPhysicalMax;// ���� �ִ빰�� ���ݷ�
	WORD		AttackMagicalMin;// ���� �ּҸ��� ���ݷ�	
	WORD		AttackMagicalMax;// ���� �ִ븶�� ���ݷ�
	WORD		CriticalPercent;// ũ��Ƽ�� �ۼ�Ʈ
	WORD		PhysicalDefense;// ���� ����	
	WORD		MagicalDefense;// ���� ����	
//	ATTRIBUTEREGIST AttribResist;		// �Ӽ����׷�

	WORD		WalkMove;// ���Ͱȱ��̵���	
	WORD		RunMove;// ���Ͷٱ��̵���	
	WORD		RunawayMove;// ���͵����̵���

	int			Damage1DramaNum;// ���� 1 ���� ��ȣ	
	int			Die1DramaNum;// ���� 1 ���� ��ȣ	
	int			Die2DramaNum;// ���� 2 ���� ��ȣ	

	DWORD		StandTime;
	BYTE		StandRate;
	BYTE		WalkRate;
	BYTE		RunRate;
	DWORD		DomainRange;					// ���� ���� : ������
	DWORD		PursuitForgiveTime;
//	DWORD		PursuitForgiveStartTime;		//not inclue in txt list
	DWORD		PursuitForgiveDistance;	

	BOOL		bForeAttack;					// ������
	DWORD		SearchPeriodicTime;				// Ž�� �ֱ�
	WORD		TargetSelect;					// Ÿ�� ���� : FIND_CLOSE, FIND_FIRST
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
	MONEYTYPE MinDropMoney;				// �ּҵ� �׼�	
	MONEYTYPE MaxDropMoney;				// �ִ� �� �׼�	
	*/

	// 070419 LYW --- GameResourceStruct : Add new drop item part.
	MONEYTYPE dropMoney ;
	DWORD	  dwMoneyDropRate ;
	//MONSTER_ITEM dropItem[eDROPITEMKIND_MAX-1] ;
	MONSTER_DROPITEM dropItem[eDROPITEMKIND_MAX-1] ;
	
	// 070423 LYW --- GameResourceStruct : Delete not use memeber.
	//WORD DropRate[eDROPITEMKIND_MAX];
	
	// �������� ���
	// �ʱ�ȭ�� �ʿ�!!
	// 070423 LYW --- GameResourceStruct : Delete not use memeber.
	//WORD CurDropRate[eDROPITEMKIND_MAX];


	// 070423 LYW --- GameResourceStruct : Modified drop item part.
	/*
	WORD ItemListNum1;				// �ش������1����Ʈǥ��ȣ
	WORD ItemListNum2;				// �ش������2����Ʈǥ��ȣ - �߰� 
	WORD ItemListNum3;				// �ش������3����Ʈǥ��ȣ - �߰� 
	// 070419 LYW --- GameResourceStruct : Add more item list.
	WORD ItemListNum4 ;
	WORD ItemListNum5 ;
	*/


	// 070423 LYW --- GameResourceStruct : Delete not use memeber.
	//BYTE MaxDropItemNum;			// �ִ������ ����

	float fRunawayLifeRate;			// ����� Ȯ�� 0~100
	WORD RunawayRate;				// ���� Ȯ��
	WORD RunawayHelpRate;			// ���� ����Ȯ��
	WORD RunawayDistance;			// ���� �Ÿ�

//	WORD Talk1;						// ��� 1
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
// ���� chx ������ ����Ʈ
//////////////////////////////////////////////////////////////////////////
typedef struct _CHXLIST
{
	StaticString FileName;
}CHXLIST;

//-----------------------------------------------------------------------
// ����ȯ ��ǥ��� ����ü
//-----------------------------------------------------------------------
typedef struct _MAPCHANGE_INFO
{
	WORD Kind;

	// 071214 LUJ, �� �̸��� MAX_NAME_LENGTH ���̸� �ʰ��Ͽ� ���̸� ���� ���� 
	char CurMapName[ MAX_MONSTER_NAME_LENGTH + 1 ];
	char ObjectName[ MAX_MONSTER_NAME_LENGTH + 1 ];
	//char CurMapName[MAX_NAME_LENGTH+1];
	//char ObjectName[MAX_NAME_LENGTH+1];

	WORD CurMapNum;
	WORD MoveMapNum;
	VECTOR3 CurPoint;
	VECTOR3 MovePoint;
	WORD chx_num;
	
	// ���⼳��
}MAPCHANGE_INFO;

//-----------------------------------------------------------------------
// LogIn ��ǥ��� ����ü
//-----------------------------------------------------------------------
typedef struct _LOGINPOINT_INFO
{
	WORD Kind;
	char MapName[MAX_NAME_LENGTH+1];
	WORD MapNum;
	BYTE MapLoginPointNum;
	VECTOR3 CurPoint[10];
	WORD chx_num;

	// ���⼳��
}LOGINPOINT_INFO;

//-----------------------------------------------------------------------
// NPC ����
//-----------------------------------------------------------------------
struct NPC_LIST
{
	WORD NpcKind;			// Npc ����
	char Name[MAX_NPC_NAME_LENGTH+1];			// Npc �̸�
	WORD ModelNum;			// MonsterChxList������ ��ȣ
	WORD JobKind;			// ��������������
	float Scale;			// Npc ũ��
	WORD Tall;				// Ű
	BOOL ShowJob;
};

//-----------------------------------------------------------------------
// NPC ����
//-----------------------------------------------------------------------
typedef struct _NPC_REGEN
{

	_NPC_REGEN():dwObjectID(0),NpcKind(0)
	{
		Pos.x = Pos.y = Pos.z = 0;
	}
	DWORD	dwObjectID;		// �ű� �߰� taiyo
	MAPTYPE	MapNum;			// Map��ȣ
	WORD	NpcKind;		// NpcKind
	char	Name[MAX_NAME_LENGTH+1];		// Npc �̸�
	WORD	NpcIndex;		// NpcIndex(������ȣ�� ���)
	VECTOR3 Pos;			// ���� ��ġ
	float	Angle;			// ���� ����
}NPC_REGEN;


//-----------------------------------------------------------------------
// MOD ����Ʈ 
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

//---------------<������ ���� ����Ʈ>------------------------------------------------------
/*
struct ITEM_INFO
{
	DWORD ItemIdx;			//		Item_Index	
	char ItemName[MAX_ITEMNAME_LENGTH+1];		//		Item_name
	WORD ItemTooltipIdx;	//		Item���� ���� index
	WORD Image2DNum;		//		2D_Image	
	WORD ItemKind;			//		����������	"0 : ���� 1 : ������ 2 : ���������� 3 : ��Ÿ"
	DWORD BuyPrice;			//		���԰���	
	DWORD SellPrice;		//		�ǸŰ���	
	WORD Rarity;			//		��Ұ�ġ	
	WORD WeaponType;		//		��������	���� / ������
	WORD Str;				//		�ٰ�+(����)	����� ���� �����ۿ� ������
	WORD Dex;			//		��ø+(����)	����� ���� �����ۿ� ������
	WORD Vit;			//		ü��+(����)	����� ���� �����ۿ� ������
	WORD Int;
	WORD Wis;			//		�ɸ�+(����)	����� ���� �����ۿ� ������
	DWORD Life;				//		�����+(����)	����� ���� �����ۿ� ������
	WORD Mana;			//		����+(����)	����� ���� �����ۿ� ������
	WORD LimitRace;
	WORD LimitJob;			//		�������� ����	���� / ���������� ��� / ��¹���-ȭ��, �ظ�
	WORD LimitGender;		//		�������� ����	���� / ���������� ���
	LEVELTYPE LimitLevel;		//		�������� ����	���� / ���������� ���
	WORD LimitStr;		//		�������� �ٰ�	���� / ���������� ���
	WORD LimitDex;		//		�������� ��ø	���� / ���������� ���
	WORD LimitVit;		//		�������� ü��	���� / ���������� ���
	WORD LimitInt;		//		�������� �ɸ�	���� / ���������� ���
	WORD LimitWis;		//		�������� �ɸ�	���� / ���������� ���

	WORD ItemGrade;			//	����������	������ ���	
	WORD RangeType;			//				�Ÿ�����	
	WORD EquipKind;			//				��������	
	WORD Part3DType;		//				3D������ȣ	
	WORD Part3DModelNum;	//				3D�����𵨹�ȣ	
	WORD MeleeAttackMin;	//				�ٰŸ� �ּҰ��ݷ�
	WORD MeleeAttackMax;	//				�ٰŸ� �ִ���ݷ�
	WORD RangeAttackMin;	//				���Ÿ� �ּҰ��ݷ�
	WORD RangeAttackMax;	//				���Ÿ� �ִ���ݷ�
	WORD MagicAttackMin;	//				���� �ּҰ��ݷ�
	WORD MagicAttackMax;	//				���� �ִ���ݷ�
	WORD CriticalPercent;	//				ũ��Ƽ�� �ۼ�Ʈ

	WORD PhyDef;			//				���� ����+ 
	WORD MagicDef;
	WORD Plus_SkillIdx;	//				Ư�� ����+ index
	WORD Plus_Value;		//				Ư�� ����+ ����
	WORD AllPlus_Kind;		//				���� ������ ����+ 
	WORD AllPlus_Value;		//				���� ������ ����+ ����

	WORD SkillNum;			//	������	���� ����  ��ȣ
	WORD SkillType;		//			��������

	WORD LifeRecover;		//	����	�����+(ȸ��) ������ġ	���� ����
	float LifeRecoverRate;		//			�����+(ȸ��)  	���� ����
	WORD ManaRecover;	//			����+(ȸ��) ������ġ	���� ����
	float ManaRecoverRate;	//			����+(ȸ��)    	���� ����

	BOOL Trade;
	WORD ItemType;			//	��Ÿ	������ �Ӽ�	=>����Ʈ, ��ؼ�, ���
	WORD wItemAttr;			// �����ۼӼ�(ȭ, ��, ��, ��, ��) => �Ϻ� ���ð���
	WORD wAcquireSkillIdx1;	// �ʿ��� ��ų �ε��� => �Ϻ����ð���
	WORD wAcquireSkillIdx2;	// �ʿ��� ��ų �ε��� => �Ϻ����ð���
	WORD wDeleteSkillIdx;	// ���� ��ų �ε��� => �Ϻ����ð���
};
*/
struct ITEM_INFO
{
	DWORD ItemIdx;			//		Item_Index	
	char ItemName[MAX_ITEMNAME_LENGTH+1];		//		Item_name
	DWORD ItemTooltipIdx;	//		Item���� ���� index
	WORD Image2DNum;		//		2D_Image	
	WORD Part3DType;		//				3D������ȣ	
	WORD Part3DModelNum;	//				3D�����𵨹�ȣ	

	WORD LimitRace;
	WORD LimitGender;		//		�������� ����	���� / ���������� ���
	WORD LimitLevel;		//		�������� ����	���� / ���������� ���

	WORD Shop;
	WORD Stack;
	WORD Trade;
	WORD Deposit;
	WORD Sell;
	WORD Decompose;
	WORD Repair;
	BOOL IsEnchant;		// ���׷��̵� ���� ����
	WORD Improvement;

	DWORD EnchantValue;	// ���� ���� Ȯ�� ����
	DWORD EnchantDeterm;	// ���� �ܰ踶�� �ο��Ǵ� ��ġ

	DWORD DecomposeIdx;
	DWORD Time;
	DWORD BuyPrice;			//		���԰���	
	DWORD SellPrice;		//		�ǸŰ���	
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

	/// ��Ȳ�� ����
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

	DWORD	dwBuyFishPoint; //		���Թ��������Ʈ
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


//---�ƾ��۰�ȭ
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

//---�ƾ��ۺ���
struct sITEM_DISSOLUTIONITEM
{
	DWORD			wUnitItemIdx;					//���ص� ������ �ε���
	WORD			wUnitItemNum;						//���ص� ������ ����
};

struct sITEM_DISSOLUTION_INFO
{
	DWORD					wDissolutionItemIdx;	//������ ���̽������� �ε���
	WORD					wUnitKindNum;			//���ص� ������ ���� ����
	WORD					wUnitTotalNum;			//���ص� ������ �� ����
	sITEM_DISSOLUTIONITEM*	pUnitItem;				//���ؾ����� ����
};
//-----------------------------------



//-----------------------------------------------------------------------------------------------------------//
//		Ŭ���̾�Ʈ�� ����ϴ� �κ�
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
// game config ���� 
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
//		������ ����ϴ� �κ�
#ifdef _SERVER_RESOURCE_FIELD_

//-----------------------------------------------------------------------------------------------------------//
#include "ServerTable.h"
//-----------------------------------------------------------------------
// ĳ���� IN/OUT POINT ����
//-----------------------------------------------------------------------
typedef struct _CHARACTERINOUTPOINT
{
	WORD MapNum;
	VECTOR3 MapInPoint[MAX_MAP_NUM];
	VECTOR3 MapOutPoint[MAX_MAP_NUM];
}CHARACTERINOUTPOINT;


//---KES ��ų�ʱ�ȭ��	071207
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
