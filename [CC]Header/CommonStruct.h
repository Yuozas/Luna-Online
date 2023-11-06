#pragma once

#include "AddableInfo.h"
#include <hash_map>
#include <hash_set>
#include <list>
#include <map>
#include <set>
#include <string>

#pragma pack(push,1)
/**********************************************************************************/
/*			�Ʒ��� Ŭ�������� ���Ǵ� DEFINE			 						  */
/**********************************************************************************/
#define MAX_NETWORK_MSG	256

// 080530 LYW --- CommonStruct : Define a 64bit data type for experience of character.
typedef unsigned long long DWORDEX ;

typedef WORD MAPTYPE;
// 080530 LYW --- CommonStruct : Modified the experience type to extended dword.
//typedef DWORD EXPTYPE;
typedef DWORDEX EXPTYPE ;

typedef WORD LEVELTYPE;

typedef DWORD DURTYPE;
typedef WORD POSTYPE;
typedef DWORD ITEMPARAM;

typedef int FAMETYPE;
typedef DWORD WANTEDTYPE;

typedef DWORD MARKNAMETYPE;

typedef DWORD			MONEYTYPE;
#define MAXMONEY		MAXULONG_PTR		// 0xffffffff(4294967295)

struct MSGROOT
{
	BYTE CheckSum;
#ifdef _CRYPTCHECK_ 
	MSGROOT():Code(0){}
	char Code;
#endif
	BYTE Category;
	BYTE Protocol;
};
struct MSGBASE :public MSGROOT
{
	DWORD dwObjectID;
};

struct MAPSERVER_TRANSDATAONOFF : public MSGROOT // ON/OFF ���� ��Ŷ
{
	int MapServerGroupNum;
	int MapServerNum;
	int HardNum;
};

struct MAPSERVER_TRANSDATA : public MSGROOT // �Ϲ� ���� ��Ŷ
{
	int MapServerGroupNum;
	int MapServerNum;
	int HardNum;
};

struct MSGBASEDATA :public MSGROOT //�޽��� ���ۿ�
{
	int MapServerGroupNum;
	int MapServerNum;
	char Messagebuff[1028];
};

struct MSGCRYPTDATA : public MSGROOT
{
#ifdef _CRYPTCHECK_ 
	HselInit eninit;
	HselInit deinit;
#endif
};

struct COMPRESSEDPOS
{
	WORD wx;
	WORD wz;
	void Compress(VECTOR3* pPos)
	{
		wx = WORD(pPos->x);
		wz = WORD(pPos->z);
	}
	void Decompress(VECTOR3* pRtPos)
	{
		pRtPos->x = wx;
		pRtPos->z = wz;
		pRtPos->y = 0;
	}
};
struct COMPRESSEDTARGETPOS
{
	void Init()
	{
		PosNum = 0;
		memset( this, 0, sizeof(COMPRESSEDTARGETPOS) );
	}
	void AddTargetPos(VECTOR3 * pPos)
	{
		if( PosNum > MAX_CHARTARGETPOSBUF_SIZE )
		{
			ASSERT(0);
			return;
		}
		pos[PosNum++].Compress(pPos);
	}
	int GetSize()
	{
		return sizeof(COMPRESSEDTARGETPOS) - sizeof(COMPRESSEDPOS)*(MAX_CHARTARGETPOSBUF_SIZE-PosNum);
	}
	int GetSurplusSize()
	{
		return sizeof(COMPRESSEDPOS)*(MAX_CHARTARGETPOSBUF_SIZE-PosNum);
	}
	BYTE PosNum;
	COMPRESSEDPOS pos[MAX_CHARTARGETPOSBUF_SIZE];
};

struct SEND_MOVEINFO
{
	COMPRESSEDPOS CurPos;
	bool MoveMode;
	WORD KyungGongIdx;
	float AddedMoveSpeed;
};

struct BASEMOVE_INFO
{
	VECTOR3 CurPosition;
private:
	BYTE CurTargetPositionIdx;
	BYTE MaxTargetPositionIdx;
	VECTOR3 TargetPositions[MAX_CHARTARGETPOSBUF_SIZE];
public:
	bool bMoving;
	bool MoveMode;				// 0:�ٱ� 1:�ȱ�
	float AddedMoveSpeed;

	WORD KyungGongIdx;			// ���Idx 0: ������� �ƴϴ�  !0: ������� �����ȣ
	
	void SetFrom(SEND_MOVEINFO* pInfo)
	{
		bMoving = FALSE;
		pInfo->CurPos.Decompress(&CurPosition);
		MoveMode = pInfo->MoveMode;
		KyungGongIdx = pInfo->KyungGongIdx;
		AddedMoveSpeed = pInfo->AddedMoveSpeed;
	}
	BYTE GetCurTargetPosIdx() { return CurTargetPositionIdx; }
	BYTE GetMaxTargetPosIdx() { return MaxTargetPositionIdx; }
	BYTE & GetMaxTargetPosIdxRef() { return MaxTargetPositionIdx; }
	void SetCurTargetPosIdx(BYTE idx) { CurTargetPositionIdx=idx; }
	void SetMaxTargetPosIdx(BYTE idx) { MaxTargetPositionIdx=idx; }

	VECTOR3 * GetTargetPosition(BYTE idx) { return &TargetPositions[idx]; }
	VECTOR3 * GetTargetPositionArray() { return TargetPositions; }
	void InitTargetPosition()
	{
		CurTargetPositionIdx = 0;
		MaxTargetPositionIdx = 0;
		memset( TargetPositions, 0, sizeof( VECTOR3 ) * MAX_CHARTARGETPOSBUF_SIZE );
	}
	void SetTargetPosition( VECTOR3* pos )	{ memcpy( TargetPositions, pos, sizeof(VECTOR3)*MAX_CHARTARGETPOSBUF_SIZE );	}
	void SetTargetPosition( BYTE idx, VECTOR3 pos )	{ TargetPositions[idx] = pos;	}
};

struct MOVE_INFO : public BASEMOVE_INFO
{
	BOOL m_bLastMoving; // Ŭ���̾�Ʈ������ ����
	BOOL m_bEffectMoving;

	DWORD m_dwMoveCalculateTime;
	BOOL m_bMoveCalculateTime;

	// ���������� ����� �ð�.
	// ���������� ����� �ð��� �󸶵��� �ʾ����� �Ǵٽ� ������� �ʴ´�
	DWORD Move_LastCalcTime;

	DWORD Move_StartTime;
	VECTOR3 Move_StartPosition;
	VECTOR3 Move_Direction;

	float Move_EstimateMoveTime;
};

struct STATE_INFO
{
	int State_Start_Motion;
	int State_Ing_Motion;
	int State_End_Motion;
	DWORD State_End_MotionTime;
	DWORD State_End_Time;
	DWORD State_Start_Time;
	BYTE BeforeState;
	BYTE bEndState;
	float MotionSpeedRate;
};


struct HERO_TOTALINFO
{
	DWORD	Str;							// Strength
	DWORD	Dex;						// ��ø
	DWORD	Vit;						// ü��
	DWORD	Int;
	DWORD	Wis;						// �ɸ�

	DWORD	Mana;						// ����
	DWORD	MaxMana;					// �ִ� ����

	FAMETYPE	Fame;						// ��
	WORD	wKarma;							// ���ǵ�(��ġ��)

	EXPTYPE	ExpPoint;						// ����ġ
	DWORD	MaxMussangPoint;				// ���� �ִ�ġ
	LEVELTYPE	LevelUpPoint;				// ����������Ʈ		//??????
	MONEYTYPE	Money;						// ��

	DWORD	PartyID;						// ��Ƽ �ε��� 	
//	char	MunpaName[MAX_MUNPA_NAME+1];	// ���� �̸�
	WORD	KyungGongGrade;					// ��� ����

	// 070105 LYW --- Add variable to save index of family.
	DWORD	FamilyIdx ;
	
	DWORD	Playtime;						// Play Time
	DWORD	LastPKModeEndTime;				// ������ ����� off�ð�
	LEVELTYPE MaxLevel;						// �ö����� �ְ� ����
	char	MunpaCanEntryDate[11];			// can create guild/be member left time 
	char	FamilyCanEntryDate[11];

	DWORD	SkillPoint;
	// 080703 LUJ, �̵��� ��� ���� ����
	BOOL	bUsingTwoBlade;
};

struct MONSTER_TOTALINFO
{
	DWORD Life;
	WORD MonsterKind;
	WORD Group;	
	MAPTYPE MapNum;
	//char Name;					//�̰� �ߺ���..BASEOBJECT_INFO��  taiyo
};

struct NPC_TOTALINFO
{
	WORD NpcKind;
	WORD Group;
	MAPTYPE MapNum;
	WORD NpcUniqueIdx;
	WORD NpcJob;				// NPC_ROLE�� �ٲ�� ��! : taiyo
	
};

struct STATIC_NPCINFO
{
	MAPTYPE	MapNum;
	WORD	wNpcJob;
	WORD	wNpcUniqueIdx;
	VECTOR3	vPos;
	char NpcName[MAX_NAME_LENGTH+1];
};

struct QUESTNPCINFO
{
	DWORD	dwObjIdx;
	MAPTYPE	wMapNum;
	WORD	wNpcKind;
	char	sName[MAX_NAME_LENGTH+1];
	WORD	wNpcIdx;
	VECTOR3	vPos;
	float	fDir;	
};

struct PET_MODEL_NAME
{
	char Name[ 32 ];
};

struct PET_INFO
{
	DWORD	Index;
	char	Name[MAX_NAME_LENGTH+1];
	char	ModelName[ 32 ];
	DWORD	Image;
	DWORD	Sight;
};

struct PET_FRIENDLY_PENALTY
{
	BYTE	Friendly;
	float	Penalty;
};

struct PET_STATUS_INFO
{
	BYTE	Level;
	DWORD	STR;
	DWORD	DEX;
	DWORD	VIT;
	DWORD	INT;
	DWORD	WIS;
};

struct PET_HP_MP_INFO
{
	BYTE	Index;
	BYTE	HP;
	BYTE	MP;
};

struct PET_OBJECT_INFO
{
	DWORD	ID;
	DWORD	ItemDBIdx;
	DWORD	MasterIdx;
	char	MasterName[MAX_NAME_LENGTH+1];
	DWORD	Kind;
	BYTE	Level;
	BYTE	Grade;
	BYTE	SkillSlot;
	WORD	Exp;
	BYTE	Friendly;
	DWORD	HP;
	DWORD	MAXHP;
	DWORD	MP;
	DWORD	MAXMP;
	BYTE	AI;
	BYTE	State;
	BYTE	Type;
	DWORD	FriendlyCheckTime;
};

//////////////////////////////////////////////////////////////////////////
// taiyo : ITEMBASE�� SKILLBASE�� ���� Ŭ������ ����
struct ICONBASE
{
	DWORD	dwDBIdx;
	DWORD	wIconIdx;		// ������->itemIdx, ����->skillIdx
	POSTYPE Position;		// 
};

struct QUICKBASE
{
	WORD tab;
	WORD pos;
};


struct ITEM_OPTION
{
	// 080130 LUJ, �ɼ� �ε��� ��� ������ DB �ε����� ���
	DWORD	mItemDbIndex;
	// DWORD mIndex;

	// ��ȭ�� ��� ��ġ
	struct Reinforce
	{
		DWORD	mStrength;
		DWORD	mDexterity;
		DWORD	mVitality;
		DWORD	mIntelligence;
		DWORD	mWisdom;
		DWORD	mLife;
		DWORD	mMana;
		DWORD	mManaRecovery;
		DWORD	mLifeRecovery;
		DWORD	mPhysicAttack;
		DWORD	mPhysicDefence;
		DWORD	mMagicAttack;
		DWORD	mMagicDefence;	
		DWORD	mCriticalRate;
		DWORD	mCriticalDamage;
		DWORD	mMoveSpeed;
		DWORD	mEvade;
		DWORD	mAccuracy;
		char	mMadeBy[ MAX_NAME_LENGTH + 1 ];
	}
	mReinforce;

	// ���սÿ� �����ϰ� ȹ��Ǵ� ��ġ
	struct Mix
	{
		DWORD	mStrength;
		DWORD	mIntelligence;
		DWORD	mDexterity;
		DWORD	mWisdom;
		DWORD	mVitality;
		char	mMadeBy[ MAX_NAME_LENGTH + 1 ];
	}
	mMix;

	// ��æƮ�� ��� ��ġ. �� ��ġ�� ��� �����ϴ�.
	struct Enchant
	{
		DWORD	mIndex;
		BYTE	mLevel;
		char	mMadeBy[ MAX_NAME_LENGTH + 1 ];
	}
	mEnchant;
	
	// 080130 LUJ, ��� �ɼ� �߰�
	struct Drop
	{
		enum Key
		{
			KeyNone,
			KeyPlusStrength			= 100,
			KeyPlusIntelligence,
			KeyPlusDexterity,
			KeyPlusWisdom,
			KeyPlusVitality,
			KeyPlusPhysicalAttack,
			KeyPlusPhysicalDefence,
			KeyPlusMagicalAttack,
			KeyPlusMagicalDefence,
			KeyPlusCriticalRate,
			KeyPlusCriticalDamage,
			KeyPlusAccuracy,
			KeyPlusEvade,
			KeyPlusMoveSpeed,
			KeyPlusLife,
			KeyPlusMana,
			KeyPlusLifeRecovery,
			KeyPlusManaRecovery,
			KeyPercentStrength		= 200,
			KeyPercentIntelligence,
			KeyPercentDexterity,
			KeyPercentWisdom,
			KeyPercentVitality,
			KeyPercentPhysicalAttack,
			KeyPercentPhysicalDefence,
			KeyPercentMagicalAttack,
			KeyPercentMagicalDefence,
			KeyPercentCriticalRate,
			KeyPercentCriticalDamage,
			KeyPercentAccuracy,
			KeyPercentEvade,
			KeyPercentMoveSpeed,
			KeyPercentLife,
			KeyPercentMana,
			KeyPercentLifeRecovery,
			KeyPercentManaRecovery,
		};

        struct Value
		{
			Key		mKey;
			float	mValue;
		}
		mValue[ 5 ];
	}
	mDrop;
};


struct SKILLBASE : public ICONBASE
{
	EXPTYPE	ExpPoint;		// ��������ġ
	BYTE	Level;			// ��������
	BYTE	bWear;			// ������Ʈ �ؾ� �ϴ����� ����
	POSTYPE	QuickPosition;	// ���� �����ܿ� ������ ��ġ

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2�� ���� - �̿���
	// ���� ��ȯ �߰�
	WORD	OptionIndex;	// ���� ��ȯ �ɼ� �ε���
	//////////////////////////////////////////////////////////////////////////
};

struct SKILL_BASE
{
	DWORD	dwDBIdx;
	DWORD	wSkillIdx;
	BYTE	Level;
};

#define MAX_JOB_GRADE	6
#define MAX_JOB_SKILL	20

struct SKILL_TREE
{
	SKILL_BASE	Skill[ MAX_JOB_GRADE ][ MAX_JOB_SKILL ];
};

struct SKILLTREE_INFO
{
	SKILLBASE	skill[SLOT_SKILLTOTAL_NUM];		// ����+����
};

// 071119 LYW --- CommonStruct : ������ �ð� ���� enum �߰�.
enum ITEM_TIMEKIND
{
	eKIND_REALTIME = 0,		// ���� �ð� �������� ��� �ð��� ó��.
	eKIND_PLAYTIME ,		// ������ �÷��� ���� �� �ð��� ó��.
} ;

//////////////////////////////////////////////////////////////////////////

// 071123 KTH --- CommonStruct : �������� ���� ���� enum �߰�.
enum ITEM_SEAL_TYPE
{
	eITEM_TYPE_SEAL_NORMAL = 0,
	eITEM_TYPE_SEAL,
	eITEM_TYPE_UNSEAL,
};

//////////////////////////////////////////////////////////////////////////
struct ITEMBASE : public ICONBASE
{
//	DWORD DBIdx;
//	WORD ItemIdx;
	DURTYPE Durability;
//	POSTYPE bPosition;
	POSTYPE	QuickPosition;	// ���� �����ܿ� ������ ��ġ
	//BYTE MapNum;
	ITEMPARAM ItemParam;	

	// 071119 LYW --- CommonStruct : �ð� ���� �߰�.
	//DWORD dwRemainMinute ;	// ���� �ܿ��ð�(�д���)�� ���� ����.
	//BOOL bTimeKind ;

	// 071122 LYW --- CommonStruct : �ð� ���� �߰�.
	int nSealed ;		
//	int	nEndTime ;
	int nRemainSecond ;

	DWORD			LastCheckTime;
};
struct SLOTINFO
{
	BOOL	bLock;
	WORD	wPassword;
	WORD	wState;
};

struct ITEM_TOTALINFO
{
	//ITEMBASE Inventory[SLOT_INVENTORY_NUM];
	// 071210 KTH -- �κ��丮�� ũ�⸦ Ȯ���κ��丮 ũ����� ����
	ITEMBASE Inventory[SLOT_MAX_INVENTORY_NUM];
	ITEMBASE WearedItem[eWearedItem_Max];
	ITEMBASE ShopInventory[TABCELL_SHOPINVEN_NUM];
	//WORD	QuickItem[MAX_QUICKITEM_NUM];	// ��������� 4
};
struct ScriptCheckValue
{
	ScriptCheckValue()
	{
		ZeroMemory( this, sizeof( *this ) );
	}

	LONGLONG mValue;
};

struct STORAGE_ITEM
{
	ITEMBASE Storage[SLOT_STORAGE_NUM];
};

//////////////////////////////////////////////////////////////////////////


//------ ��Ƽ���� ����ϴ� ����ü -------------------------------
struct SEND_PARTY_GAUGE : public MSGBASE
{
	DWORD PlayerID;
	float GaugePercent;
};

struct PARTY_MEMBER
{
	DWORD dwMemberID;
	BOOL bLogged;
	BYTE LifePercent;
	BYTE ManaPercent;
	char Name[MAX_NAME_LENGTH+1];
	LEVELTYPE Level;	
	WORD	posX;
	WORD	posZ;

	// 070420 LYW --- CommonStruct : Add racial and gender to struct PARTY_MEMBER for face of party member dialog.
	BYTE	Race;
	BYTE	Gender;
};

struct PARTY_INFO : public MSGBASE 
{
	DWORD PartyDBIdx;
	BYTE Option;
	PARTY_MEMBER Member[MAX_PARTY_LISTNUM];
};

struct SEND_PARTY_MEMBER_POS : public MSGBASE
{
	DWORD	dwMemberID;
	WORD	posX;
	WORD	posZ;
};

struct SEND_PARTY_MEMBER_INFO : public MSGBASE 
{
	PARTY_MEMBER MemberInfo;
	DWORD PartyID;
};

struct SEND_CHANGE_PARTY_MEMBER_INFO : public MSGBASE
{
	DWORD PartyID;
	char Name[MAX_NAME_LENGTH+1];
	LEVELTYPE Level;

	// 070420 LYW --- CommonStruct ; Add racial and gender.
	BYTE racial ;
	BYTE gender ;
};

//------ ��Ƽ���� ����ϴ� ����ü -------------------------------

//////////////////////////////////////////////////////////////////////////



struct MAPSELECT_PARAM			// �ɸ��� ���鶧 ���ο��� ���̴� ����ü
{
	/*
	MAPSELECT_PARAM()
	{
		*Name=0;
		JobType =  0;
		SexType =  0;
		BodyType = 0;
		HairType = 0;
		FaceType = 0;
		StartArea = 0;
		bDuplCheck=FALSE;
	}
	*/
	char Name[MAX_NAME_LENGTH+1];			// ĳ���� �̸�
	BYTE	JobType;
	BYTE	SexType;
	BYTE	BodyType;
	BYTE	HairType;
	BYTE	FaceType;
	BYTE	StartArea;
	BOOL	bDuplCheck;
};

struct CHARACTERMAKEINFO : public MSGBASE
{
	char Name[MAX_NAME_LENGTH+1];			// ĳ���� �̸�
	DWORD	UserID;
	BYTE	RaceType;
	BYTE	SexType;
	BYTE	HairType;
	BYTE	FaceType;
	BYTE	JobType;
	BYTE    StandingArrayNum;				// pjs �ɸ����� ��ġ ���� [5/22/2003]
	BOOL	bDuplCheck;
};

//////////////////////////////////////////////////////////////////////////
// taiyo : ���������� ����ϳ�?
struct CALCCHARACTER
{
	WORD wAttack;
	WORD wDefense;
	WORD wHit;
	WORD wDisengage;
	WORD wMove;
	WORD wLife;
	WORD wInner;

	WORD wHwa;
	WORD wSu;
	WORD wMok;
	WORD wKum;
	WORD wTo;
	WORD wDok;
};
//////////////////////////////////////////////////////////////////////////
// �Խ��� ����
struct AUCTION_BOARDTITLE
{
	DWORD BidIdx;		// ��Ź�ȣ(�ʼ������� �����ϴ� PrimaryKey)
	WORD ItemIdx;
	WORD ItemNum;
	DWORD EndDate;
	DWORD EndTime;
	DWORD CurrentPrice;
	DWORD ImmediatePrice;
};
//////////////////////////////////////////////////////////////////////////

// Weather System
struct stWeatherTime
{
	WORD	Index;
	WORD	StartDay;
	WORD	StartHour;
	WORD	StartMinute;
	WORD	EndDay;
	WORD	EndHour;
	WORD	EndMinute;
	WORD	State;
};

struct MSG_WEATHER : public MSGBASE
{
	WORD MapNum;
	stWeatherTime WeatherTime;
};

struct MSG_INT : public MSGBASE
{
	int nData;
};

struct MSG_INT2 : public MSGBASE
{
	int nData1;
	int nData2;
};

struct MSG_DWORD : public MSGBASE
{
	DWORD dwData;
};

// 080602 LYW --- CommonStruct : DWORDEX Ÿ���� �޽��� �߰�.
struct MSG_DWORDEX : public MSGBASE
{
	DWORDEX dweData ;
} ;

struct MSG_DWORDEX2 : public MSGBASE
{
	DWORDEX dweData1 ;
	DWORDEX dweData2 ;
} ;

struct MSG_DWORDEX3 : public MSGBASE
{
	DWORDEX dweData1 ;
	DWORDEX dweData2 ;
	DWORDEX dweData3 ;
} ;

struct MSG_DWORDEX4 : public MSGBASE
{
	DWORDEX dweData1 ;
	DWORDEX dweData2 ;
	DWORDEX dweData3 ;
	DWORDEX dweData4 ;
} ;

struct MSG_DWORD2 : public MSGBASE
{
	DWORD dwData1;
	DWORD dwData2;
};

struct MSG_DWORDBYTE : public MSGBASE
{
	DWORD dwData;
	BYTE bData;
};

struct MSG_DWORD2BYTE : public MSGBASE
{
	DWORD dwData1;
	DWORD dwData2;
	BYTE bData;
};

struct MSG_DWORDBYTE2 : public MSGBASE
{
	DWORD dwData;
	BYTE bData1;
	BYTE bData2;
};

struct MSG_DWORD3 : public MSGBASE
{
	DWORD dwData1;
	DWORD dwData2;
	DWORD dwData3;
};

struct MSG_DWORD4 : public MSGBASE
{
	DWORD dwData1;
	DWORD dwData2;
	DWORD dwData3;
	DWORD dwData4;
};

struct MSG_DWORD5 : public MSGBASE
{
	DWORD dwData1;
	DWORD dwData2;
	DWORD dwData3;
	DWORD dwData4;
	DWORD dwData5;
};

struct MSG_DWORD3BYTE2 : public MSGBASE
{
	DWORD	dwData1;
	DWORD	dwData2;
	DWORD	dwData3;
	BYTE	bData1;
	BYTE	bData2;
};

struct MSG_WORD : public MSGBASE
{
	WORD wData;
};

struct MSG_WORD2 : public MSGBASE
{
	WORD wData1;
	WORD wData2;
};
struct MSG_WORD3 : public MSGBASE
{
	WORD wData1;
	WORD wData2;
	WORD wData3;
};
struct MSG_WORD4 : public MSGBASE
{
	WORD wData1;
	WORD wData2;
	WORD wData3;
	WORD wData4;
};

struct MSG_DWORD_WORD : public MSGBASE
{
	DWORD dwData;
	WORD wData;
};

struct MSG_DWORD_NAME : public MSGBASE
{
	DWORD dwData;
	char Name[MAX_NAME_LENGTH+1];
};

struct MSG_BYTE : public MSGBASE
{
	BYTE bData;
};

// 070416 LYW --- CommonStruct : Add struct for two byte.
struct MSG_BYTE2 : public MSGBASE
{
	BYTE bData1 ;
	BYTE bData2 ;
} ;

// 070429 LYW --- CommonStruct : Add struct for test.
//#ifdef _DEBUG
struct MSG_EXP_RESULT : public MSGBASE
{
	BYTE expKind ;

	DWORD sendExp ;
	EXPTYPE monsterExp ;

	LEVELTYPE monsterLevel ;
	LEVELTYPE playerLevel ;
	LEVELTYPE bigLevel ;
	LEVELTYPE partyTotalLevel ;

	DWORD dwTotalDamage ;
	DWORD dwMyDamage ;
	DWORD dwFirstDamage ;

	BOOL bFirstKiller ;

	BYTE killerCount ;
	BYTE partyMemberCount ;
} ;
//#endif //_DEBUG

// 070506 LYW --- CommonStruct : Add struct for test.
//#ifdef _DEBUG
struct MSG_DROPMONEY_RESULT : public MSGBASE
{
	DWORD dwMonstersMoney ;
	
	DWORD dwMoneyDropRate ;

	WORD  wRandomRate ;

	float fPickupPercent ;

	DWORD dwResultMoney ;
} ;

struct MSG_DROPITEM_RESULT : public MSGBASE
{
	DWORD dwItemIdx[5] ;

	DWORD dwItemDropRate[5] ;

	WORD  wRandomRate[5] ;

	BYTE  byDropCount[5] ;

	DWORD dwPlayerID[5] ;

	BOOL  bParty ;

	BYTE byCount ;
} ;
//#endif //_DEBUG

struct MSG_GUILDNOTICE : public MSGBASE
{
	DWORD dwGuildId;
	char Msg[MAX_GUILD_NOTICE+1];
	int GetMsgLength() { return sizeof(MSG_GUILDNOTICE) - MAX_GUILD_NOTICE + strlen(Msg); }
};

//struct MSG_FAMILYNOTICE : public MSGBASE
//{
//	DWORD dwFamilyId;
//	char Msg[MAX_FAMILY_NOTICE+1];
//	int GetMsgLength() { return sizeof(MSG_FAMILYNOTICE) - MAX_FAMILY_NOTICE + strlen(Msg); }
//};

struct TESTMSG : public MSGBASE
{
	char Msg[MAX_CHAT_LENGTH+1];
	int GetMsgLength() { return sizeof(TESTMSG) - MAX_CHAT_LENGTH + strlen(Msg); }
};

struct MSG_USER_ADD_ITEM : public MSGBASE
{
	char	CharacterName[32];
	DWORD	dwIconIdx;
	//int GetMsgLength() { return sizeof(MSG_USER_ADD_ITEM) - MAX_CHAT_LENGTH + strlen(Msg); }
};

struct TESTMSGID : public MSGBASE
{
	DWORD dwSenderID;
	char Msg[MAX_CHAT_LENGTH+1];
	int GetMsgLength() { return sizeof(TESTMSGID) - MAX_CHAT_LENGTH + strlen(Msg); }
};

struct MSG_STR : public MSGBASE
{
	char Data[256];
};

struct MSG_EVENT_MONSTERREGEN : public MSGBASE
{
	WORD	MonsterKind;
	BYTE	cbMobCount;		//mob count
	WORD	wMap;			//map
	BYTE	cbChannel;		//channel	= 0:all
	VECTOR3 Pos;
	WORD	wRadius;
	DWORD	ItemID;
	DWORD	dwDropRatio;	//item drop ratio
//	BYTE	bBoss;			//�ʿ��Ѱ�?
};

struct MSG_EVENT_NPCSUMMON : public MSGBASE
{
	WORD	NpcKind;
	BYTE	cbNpcCount;		//mob count
	WORD	wMap;			//map
	BYTE	cbChannel;		//channel	= 0:all
	VECTOR3 Pos;
	WORD	wRadius;
};


struct MSG_FAME : public MSGBASE
{
	FAMETYPE Fame;
};


struct MSG_LOGIN_SYN : public MSGBASE
{
	DWORD AuthKey;
	char id[21];
	char pw[21];
	char Version[16];

	// 080109 LYW --- CommonStruct : ���Ⱥ�й�ȣ�� �����Ѵ�.
	// ��) (ABCD) 0895 -- ������ ���� ���ڴ� 895 �̴�.
	// 08/1000 = A, 08/100 = B, 95/10 = C, 95/1 = D �� �����͸� �����ϵ��� �Ѵ�.
	char strSecurityPW[5] ;
	
	ScriptCheckValue Check; //disable script check
	char mLoginKey[MAX_PATH];
/*
	MSG_LOGIN_SYN()
	{
		AuthKey = 0 ;
		memset(id, 0, sizeof(id)) ;
		memset(pw, 0, sizeof(pw)) ;
		memset(Version, 0, sizeof(Version)) ;

		memset(strSecurityPW, 0, sizeof(strSecurityPW)) ;
	}
*/
};

struct MSG_USE_DYNAMIC_SYN : public MSGBASE
{
	char id[MAX_NAME_LENGTH+1];
};

struct MSG_USE_DYNAMIC_ACK : public MSGBASE
{
	char mat[32];
};

struct MSG_LOGIN_DYNAMIC_SYN : public MSGBASE
{
	DWORD AuthKey;
	char id[21];
	char pw[21];
	char Version[16];
};

struct MSG_LOGIN_ACK : public MSGBASE
{
	char agentip[16];
	WORD agentport;
	DWORD userIdx;
	BYTE cbUserLevel;
};

struct MOVE_ONETARGETPOS : public MSGBASE
{
	DWORD			dwMoverID;
	COMPRESSEDPOS	sPos;
	COMPRESSEDPOS	tPos;

	void SetStartPos( VECTOR3 * pos )
	{
		sPos.Compress(pos);
	}
	void SetTargetPos( VECTOR3* pos )
	{
		tPos.Compress( pos );
	}
	void GetStartPos( VECTOR3 * pos )
	{
		sPos.Decompress( pos );
	}
	void GetTargetInfo(MOVE_INFO * pMoveInfo)
	{
		pMoveInfo->SetCurTargetPosIdx(0);
		pMoveInfo->SetMaxTargetPosIdx(1);
		tPos.Decompress(pMoveInfo->GetTargetPosition(0));
	}
	int GetSize()
	{
		return sizeof(MOVE_ONETARGETPOS);
	}
};

struct MOVE_ONETARGETPOS_FROMSERVER : public MSGBASE
{
	COMPRESSEDPOS	sPos;
	COMPRESSEDPOS	tPos;
	float AddedMoveSpeed;

	void SetStartPos( VECTOR3 * pos )
	{
		sPos.Compress(pos);
	}
	void GetStartPos( VECTOR3 * pos )
	{
		sPos.Decompress( pos );
	}
	void SetTargetPos( VECTOR3* pos )
	{
		tPos.Compress( pos );
	}
	void GetTargetInfo(MOVE_INFO * pMoveInfo)
	{
		pMoveInfo->SetCurTargetPosIdx(0);
		pMoveInfo->SetMaxTargetPosIdx(1);
		tPos.Decompress(pMoveInfo->GetTargetPosition(0));
	}
	int GetSize()
	{
		return sizeof(MOVE_ONETARGETPOS_FROMSERVER);
	}
};

struct MOVE_TARGETPOS : public MSGBASE
{
	DWORD dwMoverID;
	float AddedMoveSpeed;
	COMPRESSEDPOS spos;
	COMPRESSEDTARGETPOS tpos;
	void Init()
	{
		tpos.Init();
	}
	void SetStartPos(VECTOR3 * pos)
	{
		spos.Compress(pos);
	}
	void AddTargetPos(VECTOR3 * pos)
	{
		tpos.AddTargetPos(pos);
	}
	void GetStartPos(VECTOR3 * pos)
	{
		spos.Decompress(pos);
	}
	void GetTargetInfo(MOVE_INFO * pMoveInfo)
	{
		pMoveInfo->SetCurTargetPosIdx(0);//CurTargetPositionIdx = 0;
		pMoveInfo->SetMaxTargetPosIdx(tpos.PosNum);//pMoveInfo->MaxTargetPositionIdx = tpos.PosNum;
		ASSERT( tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE );
		for(int i = 0 ; i < tpos.PosNum ; ++i)
			tpos.pos[i].Decompress(pMoveInfo->GetTargetPosition(i));//&pMoveInfo->TargetPositions[i]);
	}
	int GetSize()
	{
		return sizeof(MOVE_TARGETPOS) - tpos.GetSurplusSize();
	}
};


struct MOVE_POS : public MSGBASE
{
	DWORD dwMoverID;
	COMPRESSEDPOS cpos;
};


struct MOVE_POS_USER : public MSGBASE
{
	char	Name[MAX_NAME_LENGTH+1];
	COMPRESSEDPOS cpos;
};


struct MSG_WHERE_INFO : public MSGBASE
{
	WORD	wMapNum;		//�ʹ�ȣ
	BYTE	bChannel;		//ä�ι�ȣ
	COMPRESSEDPOS cpos;		//��ġ
};


struct BASEOBJECT_INFO
{
	BASEOBJECT_INFO()
	{
		BattleID = 0;
		BattleTeam = 0;
	}
	DWORD dwObjectID;
	DWORD dwUserID;
	// 080218 LYW --- CommonStruct : ������Ʈ �̸��� ���̸� �ø��� �۾�.
	//char ObjectName[MAX_NAME_LENGTH+1];
	char ObjectName[32+1];
	DWORD BattleID;
	BYTE BattleTeam;
	BYTE ObjectState;
	BYTE ObjectBattleState;
};

#define		MAX_INTRODUCE_LEN	300

// ����Ʈ ��Ī. CPlayer���� ���δ�.
// �ֹ������ε� ���� �� �ִ�. certificate of residence
// desc_hseos_�ֹε��01
// S �ֹε�� �߰� added by hseos 2007.06.09
enum																	// ä�� ��û ����
{
	DATE_MATCHING_CHAT_REQUEST_STATE_NONE = 0,							// �ƹ����� �ƴ�
	DATE_MATCHING_CHAT_REQUEST_STATE_REQUEST,							// ���������� ��ȭ ��û ����
	DATE_MATCHING_CHAT_REQUEST_STATE_RECV_REQUEST,						// ���������� ��ȭ ��û�� ���� ����
	DATE_MATCHING_CHAT_REQUEST_STATE_CHATTING,							// ä�� ��
};

typedef enum															// ä�� ���
{
	DATE_MATCHING_CHAT_RESULT_REQUEST_AND_WAIT = 0,						// ..��û�ڿ��� ��ûó���� ��⸦ �˸�
	DATE_MATCHING_CHAT_RESULT_REQUESTED,								// ..����ڿ��� �������� ä�� ��û�� �˸�
	DATE_MATCHING_CHAT_RESULT_CANCEL,									// ..��û�ڱ� ��û ���
	DATE_MATCHING_CHAT_RESULT_BUSY,										// ..����ڰ� �̹� ä�� �� or ä�� ��û�� or ä�� ��û�޴� �� 
	DATE_MATCHING_CHAT_RESULT_TIMEOUT,									// ..�ð� �ʰ�
	DATE_MATCHING_CHAT_RESULT_REFUSE,									// ..������
	DATE_MATCHING_CHAT_RESULT_START_CHAT,								// ..�¶��Ͽ� ä�� ����
	DATE_MATCHING_CHAT_RESULT_END_CHAT,									// ..ä�� ��
} DATE_MATCHING_CHAT_RESULT;

// E �ֹε�� �߰� added by hseos 2007.06.09

struct DATE_MATCHING_INFO						// date matching 2007/03/28
{
	BYTE	bIsValid;							// �ֹ��� �߱� �޾Ҵ°�? ����.
	BYTE	bActive;							// Ȱ��ȭ ��°�.? Ŭ���̾�Ʈ���� 'search' �ϴ� ���� Ȱ��ȭ �ȴ�.
	BYTE	byGender;							// ����.	GENDER_MALE = 0, GENDER_FEMALE = 1
	DWORD	dwAge;								// ����.
	// desc_hseos_�ֹε��01
	// S �ֹε�� �߰� added by hseos 2007.06.06	2007.06.09
	// DWORD	dwAgeIndex;						// ������ �з��ڵ�.			// �̰� ����� ���̴��� üũ��.
	DWORD	nRace;								// ����
	// E �ֹε�� �߰� added by hseos 2007.06.06	2007.06.09
	DWORD	dwRegion;							// ��� ����					enum DATE_MATCHING_REGION �� ����.
	DWORD	dwGoodFeelingDegree;				// ȣ�� ����.
	DWORD	pdwGoodFeeling[3];					// �����ϴ� ����				enum DATE_MATCHING_FEELING �� ����.
	DWORD	pdwBadFeeling[2];					// �Ⱦ��ϴ� ����
	char	szIntroduce[MAX_INTRODUCE_LEN+1];	// �Ұ�. �ϴ� char�� 300��(�ѱ� 150��)����.
	char	szName[MAX_NAME_LENGTH+1];			// ĳ���� �̸�. BASEOBJECT_INFO::ObjectName �� ����.
	// desc_hseos_�ֹε��01
	// S �ֹε�� �߰� added by hseos 2007.06.06	2007.06.09	2007.12.18
	DWORD	nSerchTimeTick;						// �˻��� ��û���� ���� �ð� ƽ. �����ð� �������� �˻� ���.
	DWORD	nChatPlayerID;						// ä�� ����� ID
	DWORD	nRequestChatState;					// ä�� ��û ����
	DWORD	nRequestChatTimeTick;				// ä�� ��û ���� �ð� ƽ
	DWORD	nChatPlayerAgentID;					// ä�� ����� ������Ʈ ID
	BYTE	nMatchingPoint;						// ��Ī����Ʈ	
	// E �ֹε�� �߰� added by hseos 2007.06.06	2007.06.09	2007.12.18
};

#define MAX_JOB_GRADE	6
struct CHARACTER_TOTALINFO
{
	DWORD	Life;							// �����			//?
	DWORD	MaxLife;						// �ִ� �����		//?
	
	BYTE	Race;
	BYTE	Gender;							// ����
	BYTE	FaceType;						// �󱼹�ȣ
	BYTE	HairType;						// �Ӹ���� ��ȣ

	DWORD	WearedItemIdx[eWearedItem_Max];	// ������ �����Ƶ��� ����
	bool	HideParts[eWearedItem_Max];		// �������������
	BYTE	WeaponEnchant;

	BYTE	JobGrade;
	BYTE	Job[ MAX_JOB_GRADE ];
	LEVELTYPE	Level;						// ����				//?
	MAPTYPE	CurMapNum;						// �����			//?
	MAPTYPE	LoginMapNum;					// �α��θ�			//?

	bool	bPeace;							// ��/��ȭ��� FALSE �� ���� TRUE �� ��ȭ	//?
	WORD	MapChangePoint_Index;			// ����ȯ�� ���	//?
	WORD	LoginPoint_Index;				// �α��ν� ���	//?

	DWORD	MunpaID;						// ���� �ε���
	BYTE	PositionInMunpa;				// ���� ���� 

	DWORD   FamilyID;
	BYTE	PositionInFamily;
	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.11.22
	DWORD	nFamilyEmblemChangedFreq;
	// E �йи� �߰� added by hseos 2007.11.22

	MARKNAMETYPE MarkName;					// guild mark name	
	MARKNAMETYPE FamilyMarkName;					// guild mark name	
	bool	bVisible;						// ī�޶� ��带 ���� ����
	bool	bPKMode;						//PK��� ����
	BOOL	bMussangMode;					//���ָ�� ����
	
	FAMETYPE	BadFame;					//�Ǹ�(PK��ġ)

	float	Height;							// Ű
	float	Width;							// ü��

	char	NickName[MAX_GUILD_NICKNAME+1]; // ��忡���� ȣĪ
	char	GuildName[MAX_GUILD_NAME+1];	// ���� �̸�

	char    FamilyNickName[MAX_GUILD_NICKNAME+1];
	char	FamilyName[MAX_GUILD_NAME+1];
	
	DWORD	dwGuildUnionIdx;					// ����
	char	sGuildUnionName[MAX_GUILD_NAME+1];	// �����̸�
	DWORD	dwGuildUnionMarkIdx;				// ���͸�ũ

	//DWORD	dwFamilyUnionIdx;					// ����
	//char	sFamilyUnionName[MAX_GUILD_NAME+1];	// �����̸�
	//DWORD	dwFamilyUnionMarkIdx;				// ���͸�ũ

	bool	bRestraint;						// ����

	WORD	HideLevel;
	WORD	DetectLevel;

	// 071210 KTH -- �ɸ��Ͱ� ������ �ִ� Ȯ���κ��丮�� ��.
	WORD	wInventoryExpansion;

	DATE_MATCHING_INFO				DateMatching;		// ����Ʈ ��Ī ����.

	// desc_hseos_��ȥ_01
	// S ��ȥ �߰� added by hseos 2007.12.11	2007.12.12
	char	szMarriagePartnerName[MAX_NAME_LENGTH+1];
	char	szMarriagePartnerNickName[MAX_NAME_LENGTH+1];
	// E ��ȥ �߰� added by hseos 2007.12.11	2007.12.12
};

struct SHOPITEMOPTION
{
	DWORD		Avatar[eAvatar_Max];

	WORD		Str;				// �ٰ�ġ
	WORD		Dex;			// ��øġ
	WORD		Vit;			// ü��ġ
	WORD		Int;
	WORD		Wis;				// �ɸ�ġ

	WORD		Life;				// �ִ�ü������ġ
	WORD		Mana;			// �ִ볻������ġ

	WORD		AddExp;				// �߰� ����ġ			100% => 100
	WORD		AddItemDrop;		// �߰� �����۵����	100% => 100
	char		ExpPeneltyPoint;	// ���� ����ġ �г�Ƽ����Ʈ
	char		MoneyPeneltyPoint;	// ���� �� �г�Ƽ����Ʈ

	WORD		AddLevel;			// Ưġ��
	char		NeagongDamage;		// ����������		100% => 100
	char		WoigongDamage;		// �ܰ�������		100% => 100
	char		ComboDamage;		// �⺻�޺� ������  100% => 100

	char		RecoverRate;		// ����ȸ����		100% => 100

	WORD		Critical;			// ũ��Ƽ��
	char		StunByCri;			// ũ��Ƽ�ý� ���ϰɸ� Ȯ��  100% => 100
	WORD		Decisive;			// ���� ����(�ϰ�)

	char		ItemMixSuccess;		// ������ ���� ����Ȯ��		100% => 100
	WORD		StatePoint;			// �� �� �ִ� ��������Ʈ ��
	
	char		RegistPhys;			// �������� ����ġ	100% => 100
	char		RegistAttr;			// �Ӽ����� ����ġ	100% => 100
	char		ManaSpend;		// ���¼Ҹ� ����ġ		100% => 100 ����

	DWORD		SkillPoint;			// �� �� �ִ� ����Ʈ
	DWORD		UseSkillPoint;		// �̹� ���� �� �� �ִ� ����Ʈ
	
	char		ProtectCount;		// ����ġ+����ȣ�ֹ����� ���� Ƚ��.
	
	WORD		AddSkillExp;		// ��������ġ ����ġ	100% => 100

	//char		PlustimeExp;		// �÷���Ÿ�� ����ġ
	//char		PlustimeAbil;		// �÷���Ÿ�� Ư��ġ
	//char		PlustimeMana;	// �÷���Ÿ�� ���¼Ҹ�
	
	BYTE		bKyungGong;			// ���
	BYTE		KyungGongSpeed;		// ����ӵ�
	
	BYTE		EquipLevelFree;		// ��� �������� ����
	BYTE		ReinforceAmp;		// ��ȭ���� ��ġ		100% => 100

	BYTE		bStreetStall;		// �������� ��밡������ ����

//	WORD		BuyStallInven;		// ���Գ��� �߰� �κ� ��

//	char		ReviveLife;			// ��Ȱ�ֹ��� ���� Life ȭ������		100% => 100
//	char		ReviveExp;			// ��Ȱ�ֹ��� ���� ����ġ ȸ������	100% => 100
//	char		ReviveMana;		// ��Ȱ�ֹ��� ���� ���� ȸ������		100% => 100	
};

struct AVATARITEMOPTION
{
	WORD		Life;				// �ִ�����
	WORD		Mana;			// �ִ볻��

	BYTE		Attack;				// ���ݷ�
	BYTE		Critical;			// ũ��Ƽ��
	BYTE		Decisive;			// ���� �ϰ�(����)

	BYTE		Str;				// �ٰ�
	BYTE		Dex;			// ��ø
	BYTE		Vit;			// ü��
	BYTE		Int;
	BYTE		Wis;				// �ɸ�

	WORD		CounterPercent;		// ī���� Ȯ��
	WORD		CounterDamage;		// ī���ͽ� ��������

	BYTE		bKyungGong;			// ���
	
	BYTE		ManaSpend;		// ���¼Ҹ𰨼�ġ
	
	WORD		NeagongDamage;		// ����������		100% => 100
	WORD		WoigongDamage;		// �ܰ�������		100% => 100

	WORD		TargetPhyDefDown;	// ���� �������� ����
	WORD		TargetAttrDefDown;	// ���� �Ӽ����׷� ����
	WORD		TargetAtkDown;		// ������ ���ݷ� ����

	WORD		RecoverRate;		// ȸ����
	WORD		KyunggongSpeed;		// ����ӵ�

	WORD		MussangCharge;		// ��ȥ������ �����ӵ�
	BYTE		NaeruykspendbyKG;	// ������ ���¼Ҹ� ����
};

struct SEND_MONSTER_TOTALINFO	:	public MSGBASE
{
	BASEOBJECT_INFO BaseObjectInfo;
	MONSTER_TOTALINFO TotalInfo;
	SEND_MOVEINFO MoveInfo;

	BYTE bLogin;
	
	CAddableInfoList AddableInfo;

	WORD GetMsgLength()	{	return sizeof(SEND_MONSTER_TOTALINFO) - sizeof(CAddableInfoList) + AddableInfo.GetInfoLength();	}
};

struct SEND_NPC_TOTALINFO	:	public MSGBASE
{
	BASEOBJECT_INFO BaseObjectInfo;
	NPC_TOTALINFO TotalInfo;
	SEND_MOVEINFO MoveInfo;
	
	float Angle;
	BYTE bLogin;

	CAddableInfoList AddableInfo;

	WORD GetMsgLength()	{	return sizeof(SEND_NPC_TOTALINFO) - sizeof(CAddableInfoList) + AddableInfo.GetInfoLength();	}
};

struct SEND_CHARACTER_TOTALINFO : public MSGBASE
{
	BASEOBJECT_INFO BaseObjectInfo;
	CHARACTER_TOTALINFO TotalInfo;	
	SEND_MOVEINFO MoveInfo;
	SHOPITEMOPTION	ShopItemOption;

	BYTE bLogin;
	
	CAddableInfoList AddableInfo;

	WORD GetMsgLength()	{	return sizeof(SEND_CHARACTER_TOTALINFO) - sizeof(CAddableInfoList) + AddableInfo.GetInfoLength();	}
};

struct SEND_HERO_TOTALINFO : public MSGBASE
{
	BASEOBJECT_INFO BaseObjectInfo;
	CHARACTER_TOTALINFO ChrTotalInfo;
	HERO_TOTALINFO HeroTotalInfo;
	SEND_MOVEINFO SendMoveInfo;
	DWORD UniqueIDinAgent;
	
	SHOPITEMOPTION	ShopItemOption;
	WORD	SkillNum;
	ITEM_TOTALINFO ItemTotalInfo;
	WORD	OptionNum;
	WORD	RareOptionNum;
	WORD	PetNum;

	SYSTEMTIME		ServerTime;			//ĳ���� ���� �α��� �ð�

	// 071228 LUJ
	DWORD	StorageSize;

	//SW051004 ����
	CAddableInfoList AddableInfo;

	WORD GetMsgLength()
	{
		return sizeof( SEND_HERO_TOTALINFO ) - sizeof(CAddableInfoList) + AddableInfo.GetInfoLength();
	}
};

// 071129 LYW --- CommonStruct : ��ų �ʱ�ȭ ���� ���� ����ü �߰�.
struct SEND_SKILL_RESETINFO : public MSGBASE	
{
	CAddableInfoList AddableInfo ;
	WORD SkillNum ;
	int nSkillPoint ;
} ;


struct SEND_CHARSELECT_INFO : public MSGBASE
{
#ifdef _CRYPTCHECK_ 
	HselInit eninit;
	HselInit deinit;
#endif
	int CharNum;
	WORD StandingArrayNum[MAX_CHARACTER_NUM];				// pjs �ɸ����� ��ġ ���� [5/22/2003]
	BASEOBJECT_INFO BaseObjectInfo[MAX_CHARACTER_NUM];
	CHARACTER_TOTALINFO ChrTotalInfo[MAX_CHARACTER_NUM];
};

/*
struct SEND_CHARMAKE_INFO : public MSGBASE
{
	DWORD	UserID;
	char	Name[MAX_NAME_LENGTH+1];
	BYTE	Job;
	BYTE	FaceType;
	BYTE	HeadType;
	MAPTYPE	Map;
	BYTE	Gender;
	
	char Name[MAX_NAME_LENGTH+1];			// ĳ���� �̸�
	DWORD	UserID;
	BYTE	JobType;
	BYTE	SexType;
	BYTE	BodyType;
	BYTE	HairType;
	BYTE	FaceType;
	BYTE	StartArea;
	BOOL	bDuplCheck;
};
*/

struct MSG_NAME : public MSGBASE
{
	char Name[MAX_NAME_LENGTH+1];
};

struct MSG_NAME2 : public MSGBASE
{
	char str1[MAX_NAME_LENGTH+1];
	char str2[MAX_NAME_LENGTH+1];
};

struct MSG_NAME_DWORD : public MSGBASE
{
	char Name[MAX_NAME_LENGTH+1];
	DWORD dwData;
};

struct MSG_NAME_WORD : public MSGBASE
{
	char Name[MAX_NAME_LENGTH+1];
	WORD wData;
};

struct MSG_NAME_DWORD2 : public MSGBASE
{
	char Name[MAX_NAME_LENGTH+1];
	DWORD dwData1;
	DWORD dwData2;
};

//ÆÑ µ¥?ÌÅÍ °ú·Ã ////////////////////////////////////////////////
struct MSG_NAME_DWORD3 : public MSGBASE
{
	char	Name[MAX_NAME_LENGTH+1];
	DWORD	dwData1;
	DWORD	dwData2;
	DWORD	dwData3;
};

struct MSG_NAME2_DWORD : public MSGBASE
{
	char	Name1[MAX_NAME_LENGTH+1];
	char	Name2[MAX_NAME_LENGTH+1];
	DWORD	dwData;
};

// desc_hseos_����Ʈ ��_01
// S ����Ʈ �� �߰� added by hseos 2008.01.23
struct MSG_NAME2_DWORD2 : public MSGBASE
{
	char	Name1[MAX_NAME_LENGTH+1];
	char	Name2[MAX_NAME_LENGTH+1];
	DWORD	dwData1;
	DWORD	dwData2;
};
// E ����Ʈ �� �߰� added by hseos 2008.01.23

//�� ������ ���� ////////////////////////////////////////////////
struct SEND_PACKED_DATA : public MSGBASE
{
	WORD wRealDataSize;
	WORD wReceiverNum;
	char Data[MAX_PACKEDDATA_SIZE];
};
struct SEND_PACKED_TOMAPSERVER_DATA : public MSGBASE
{
	MAPTYPE ToMapNum;
	WORD wRealDataSize;
	char Data[MAX_PACKEDDATA_SIZE];
	WORD GetMsgLength()
	{
		return sizeof(SEND_PACKED_TOMAPSERVER_DATA) - MAX_PACKEDDATA_SIZE + wRealDataSize;
	}
};
// ������ ���� ��Ʈ�p �޽��� ////////////////////////////////////

struct MSG_ITEM : public MSGBASE
{
	ITEMBASE ItemInfo;
};

struct MSG_ITEM_WITH_OPTION : public MSGBASE
{
	//BOOL IsOptionItem;
	//BOOL IsRareOptionItem;
	ITEMBASE ItemInfo;
	ITEM_OPTION OptionInfo;
	//ITEM_RARE_OPTION_INFO RareOptionInfo;

	//MSG_ITEM_WITH_OPTION() : IsOptionItem(0) /*, IsRareOptionItem(0)*/ {}

	//DWORD GetMsgLength() const
	//{
	//	if( IsOptionItem )
	//	{
	//		return sizeof( MSG_ITEM_WITH_OPTION );
	//	}
	//	else
	//	{
	//		return sizeof( MSG_ITEM_WITH_OPTION ) - sizeof( OptionInfo );
	//	}
	//}
};


// 080130 LUJ, ������ �ɼ��� �����ϴ� �޽��� ����ü
struct MSG_ITEM_OPTION : public MSGBASE
{
	DWORD		mSize;
	ITEM_OPTION mOption[ 100 ];

	DWORD GetSize() const
	{
		return sizeof( MSG_ITEM_OPTION ) - ( sizeof( mOption ) / sizeof( *mOption ) - mSize ) * sizeof( *mOption );
	}
};

//���� �۾�
struct ITEMOBTAINARRAY : public MSGBASE
{
	ITEMOBTAINARRAY()
	{
		ItemNum = 0;
	}
	void Init( BYTE cg, BYTE pt, DWORD dwID )
	{
		Category	= cg;
		Protocol	= pt;
		dwObjectID	= dwID;
	}
	//void AddItem( DWORD DBIdx, DWORD ItemIdx, DURTYPE Durability, POSTYPE bPosition, POSTYPE QuickPosition, ITEMPARAM Param, DWORD RareIdx = 0,int Sealed, int RemainSecond )
	void AddItem( DWORD DBIdx, DWORD ItemIdx, DURTYPE Durability, POSTYPE bPosition, POSTYPE QuickPosition, ITEMPARAM Param, DWORD RareIdx,int Sealed, int RemainSecond = 0)
	{
		ItemInfo[ItemNum].dwDBIdx		= DBIdx;
		ItemInfo[ItemNum].wIconIdx		= ItemIdx;
		ItemInfo[ItemNum].Durability	= Durability;
		ItemInfo[ItemNum].Position		= bPosition;
		ItemInfo[ItemNum].QuickPosition	= QuickPosition;
		ItemInfo[ItemNum].ItemParam		= Param;
		//SW050920 RareItem
		// 071127 KTH -- ���ΰ� ��ȿ�ð��� �־��ش�.
		ItemInfo[ItemNum].nSealed		= Sealed;
		ItemInfo[ItemNum].nRemainSecond = RemainSecond;
		ItemNum++;
	}
	void AddItem( WORD idx, ITEMBASE * item)
	{
		memcpy( &ItemInfo[idx], item, sizeof( ITEMBASE ) );
		ItemNum++;
	}
	ITEMBASE * GetItem( WORD idx ) { return &ItemInfo[idx]; }
	int GetSize()
	{
		//return ( sizeof( ITEMOBTAINARRAY ) - ( sizeof( ITEMBASE ) * ( SLOT_INVENTORY_NUM - ItemNum ) ) );
		// 071213 KTH --
		return ( sizeof( ITEMOBTAINARRAY ) - ( sizeof( ITEMBASE ) * ( SLOT_MAX_INVENTORY_NUM - ItemNum ) ) );
	}
	void Clear()
	{
		memset( this, 0, sizeof( ITEMOBTAINARRAY ) );
		ItemNum = 0;
	}
	BYTE		ItemNum;
	WORD		wObtainCount;
	//ITEMBASE	ItemInfo[SLOT_INVENTORY_NUM];
	// 071213 KTH --
	ITEMBASE	ItemInfo[SLOT_MAX_INVENTORY_NUM];
};

typedef void (*DBResult)(void * pPlayer, WORD ArrayID);
typedef void (*DBResultEx)(void* pPlayer, WORD ArrayID, void* pRareOptionInfo);
struct ITEMOBTAINARRAYINFO
{
	DWORD			wObtainArrayID;
	DBResult		CallBack;
	DBResultEx		CallBackEx;
	DWORD			dwFurnisherIdx;
	WORD			wType;

	ITEMOBTAINARRAY	ItemArray;
};

struct MSG_ITEM_ERROR : public MSGBASE
{
	int ECode;
};

struct MSG_ITEM_MOVE_SYN : public MSGBASE
{
	POSTYPE FromPos;
	DWORD wFromItemIdx;
	POSTYPE ToPos;
	DWORD wToItemIdx;
	DWORD wIconIdx;
};

struct MSG_PET_ITEM_MOVE_SYN : public MSGBASE
{
	POSTYPE PetPos;
	DWORD PetItemIdx;
	POSTYPE PlayerPos;
	DWORD PlayerItemIdx;
};

struct MSG_ITEM_MOVE_ACK : public MSG_ITEM_MOVE_SYN
{
};
struct MSG_ITEM_COMBINE_SYN : public MSGBASE
{
	DWORD wItemIdx;
	POSTYPE FromPos;
	POSTYPE ToPos;
	DURTYPE FromDur;
	DURTYPE ToDur;
};
struct MSG_ITEM_COMBINE_ACK : public MSG_ITEM_COMBINE_SYN
{
};
struct MSG_ITEM_DIVIDE_SYN : public MSGBASE
{
	DWORD wItemIdx;
	POSTYPE FromPos;
	POSTYPE ToPos;
	DURTYPE FromDur;
	DURTYPE ToDur;
};

struct MSG_ITEM_DISCARD_SYN : public MSGBASE
{
	POSTYPE TargetPos;
	DWORD wItemIdx;
	DURTYPE ItemNum;
};
struct MSG_ITEM_DISCARD_ACK : public MSG_ITEM_DISCARD_SYN
{
};

struct MSG_ITEM_BUY_SYN : public MSGBASE
{
	DWORD wBuyItemIdx;
	WORD BuyItemNum;
	WORD wDealerIdx;
};

struct MSG_ITEM_SELL_SYN : public MSGBASE
{
	POSTYPE TargetPos;
	DWORD	wSellItemIdx;
	WORD	SellItemNum;
	WORD	wDealerIdx;
};
struct MSG_ITEM_SELL_ACK : public MSG_ITEM_SELL_SYN
{
};


struct MSG_ITEM_ENCHANT_SYN : public MSGBASE
{
	POSTYPE ItemPos;
	DWORD	wItemIdx;

	POSTYPE MaterialItemPos;
	DWORD	wMaterialItemIdx;

	// 080228 LUJ, ��ȣ ������
	ICONBASE mProtectionItem;
};


struct MSG_ITEM_ENCHANT_ACK : public MSGBASE
{
	POSTYPE		TargetPos;
	ITEM_OPTION	OptionInfo;
};


// 071217 LUJ, Ŭ���̾�Ʈ �˻�����θ� ���
struct MSG_ITEM_MIX_SYN : public MSGBASE
{
	DWORD	mResultIndex;
	DWORD	mSourceIndex;	// �迭���� �⺻ �������� ���° �迭�� ��ġ���ִ��� �˷��ش�
	DWORD	mMixSize;		// ���� ������ ����� ��� ����
	DWORD	mSourceSize;	// ��ᰡ �����ϴ� ���� ����
	DWORD	mResultSize;	// ����� �����ϴ� ���� ����

	// 080228 LUJ, ��ȣ ������
	ICONBASE mProtectionItem;

	struct Material
	{
		DWORD	mItemIndex;
		POSTYPE	mPosition;
		DWORD	mQuantity;
	}
	mMaterial[ SLOT_MAX_INVENTORY_NUM * 2 ];	// ��� ���԰� ��� ������ ��� �ʿ��ϹǷ� �κ��丮 ������ �ι踦 ��´�

	int GetSize()
	{
		return sizeof( MSG_ITEM_MIX_SYN ) - ( sizeof( mMaterial ) / sizeof( Material ) - ( mSourceSize + mResultSize ) ) * sizeof( Material );
	}
};



// 071217 LUJ, ����� ���������� ������ �����ϹǷ� �ʿ� ������ �ѱ��
struct MSG_ITEM_MIX_SYN2 : public MSGBASE
{
	POSTYPE	mSourceItemPosition;
	DWORD	mSourceItemDbIndex;
	DWORD	mMixSize;
	DWORD	mResultIndex;

	// 080228 LUJ, ��ȣ ������
	ICONBASE mProtectionItem;
};


// 071217 LUJ
// ���⿡�� �ð� ������ ó���� ������ ������ ���簡 ����. ó������ �о ������ 0�̸� �����ϰ� 2�̸� 2�� ������Ʈ�ϸ� �ȴ�.
// ����: ���ߺ� �������� ��� Durability = UINT_MAX�� ��� ������ ��������
// ����: ���̴� ������ ���� ���´�. ���� ���� ������ �������� mMaterlal + mRemoveSize�� �����͸� ��ȸ�Ѵ�
struct MSG_ITEM_MIX_ACK	: public MSGBASE
{
	DWORD		mSize;
	POSTYPE		mSourceItemPosition;
	DWORD		mSourceItemDbIndex;
	DWORD		mMixSize;
	DWORD		mResultIndex;
	
	ITEMBASE	mItem[ SLOT_MAX_INVENTORY_NUM ];

	DWORD GetSize() const
	{
		return sizeof( MSG_ITEM_MIX_ACK ) - ( sizeof( mItem ) / sizeof( *mItem ) - mSize ) * sizeof( *mItem );
	}
};


//struct MSG_ITEM_MIX_RARE_ACK : public MSG_ITEM_MIX_SYN
//{
//	ITEM_RARE_OPTION_INFO RareOptionInfo;
//};

struct MSG_ITEM_USE_SYN : public MSGBASE
{
	POSTYPE TargetPos;
	DWORD	wItemIdx;
};

struct MSG_ITEM_USE_ACK : public MSG_ITEM_USE_SYN
{
};
/// ������ �ɼ� ���� //////////////////////////////////
struct ITEM_JEWEL_POS
{
	DWORD	wItemIdx;
	POSTYPE pos;
};


struct MSG_ITEM_DISSOLVE_SYN : public MSGBASE
{
	POSTYPE	mSourceItemPosition;	// ������ ��� ������ ��ġ
	DWORD	mSize;

	struct Slot
	{
		DWORD	mItemIndex;
		POSTYPE	mPosition;
		DURTYPE	mQuantity;
	}
	//mSlot[ SLOT_INVENTORY_NUM ];
	// 071213 KTH --
	mSlot[ SLOT_MAX_INVENTORY_NUM ];

	DWORD GetSize() const
	{
		//return sizeof( MSG_ITEM_DISSOLVE_SYN ) - ( SLOT_INVENTORY_NUM - mSize ) * sizeof( Slot );
		return sizeof( MSG_ITEM_DISSOLVE_SYN ) - ( SLOT_MAX_INVENTORY_NUM - mSize ) * sizeof( Slot );
	}

	//DWORD	wTargetItemIdx;
	//POSTYPE TargetPos;
	//DURTYPE	wOptionIdx;
};


struct MSG_ITEM_DISSOLVE_SYN2 : public MSGBASE
{
	POSTYPE	mSourceItemPosition;
	DWORD	mSourceItemDbIndex;
};


struct MSG_ITEM_DISSOLVE_ACK : public MSGBASE
{
	DWORD	mSourceItemPosition;
	DWORD	mSourceItemDbIndex;
	DWORD	mSize;
	
	ITEMBASE mItem[ SLOT_MAX_INVENTORY_NUM ];

	DWORD GetSize() const
	{
		return sizeof( MSG_ITEM_DISSOLVE_ACK ) - ( sizeof( mItem ) / sizeof( *mItem ) - mSize ) * sizeof( *mItem );
	}
};


struct MSG_ITEM_REINFORCE_SYN : public MSGBASE
{
	DWORD	mSourceItemIndex;		// ��ȭ�� ��� ������ ��ȣ
	POSTYPE mSourceItemPosition;	// ��ȭ�� ��� ������ ��ġ
	DWORD	mSourceItemDbIndex;
	WORD	mSize;					// ��� ����

	// 080228 LUJ, ��ȣ ������
	ICONBASE mProtectionItem;

	ITEMBASE mItem[ SLOT_MAX_INVENTORY_NUM ];	// 071213 KTH --

	DWORD GetSize() const
	{
		return sizeof(MSG_ITEM_REINFORCE_SYN) - ( sizeof( mItem ) / sizeof( *mItem ) - mSize ) * sizeof( *mItem );
	}	
};


struct MSG_ITEM_REINFORCE_OPTION_ACK : public MSGBASE
{
	DWORD		mItemDbIndex;
	ITEM_OPTION mOption;
};


struct MSG_ITEM_REINFORCE_ACK : public MSGBASE
{
	DWORD		mSourceItemDbIndex;
	
	DWORD		mSize;
	ITEMBASE	mItem[ SLOT_MAX_INVENTORY_NUM ];

	DWORD GetSize() const
	{
		return sizeof( MSG_ITEM_REINFORCE_ACK ) - ( sizeof( mItem ) / sizeof( *mItem ) - mSize ) * sizeof( *mItem );
	}
};


//struct MSG_ITEM_RAREITEM_GET : public MSGBASE
//{
//	DWORD		wTargetItemIdx;
//	ITEMBASE	RareItemBase;
//	ITEM_RARE_OPTION_INFO RareInfo;
//};
///////////////////////////////////////////////////////
// ����â
struct SLOT_INFO
{
	BYTE	kind;
	DWORD	dbIdx;
	DWORD	idx;
	WORD	data;
};

struct TAB_INFO
{
	BYTE		tabNum;
	SLOT_INFO	slot[10];
};

struct MSG_QUICK_INFO : public MSGBASE
{
	TAB_INFO	tab[8];
};

struct MSG_QUICK_ADD_SYN : public MSGBASE
{
	BYTE		tabNum;
	BYTE		pos;
	SLOT_INFO	slot;
};

struct MSG_QUICK_REMOVE_SYN : public MSGBASE
{
	BYTE		tabNum;
	BYTE		pos;
};

struct MSG_QUICK_CHANGE_SYN : public MSGBASE
{
	BYTE	tabNum1;
	BYTE	pos1;
	BYTE	tabNum2;
	BYTE	pos2;
};

//////////// ���� ���� ��Ʈ�p �޽��� /////////////////////////////
//-------------- ����(��ų) �̵� (in ����â) ---------------------

struct MSG_SKILL_MOVE_SYN : public MSGBASE
{
	POSTYPE FromPos;
	POSTYPE ToPos;
};

struct MSG_SKILL_MOVE_ACK : public MSG_SKILL_MOVE_SYN
{
};

struct MSG_SKILL_REM_SYN : public MSGBASE
{
	WORD	wSkillIdx;
	POSTYPE TargetPos;
};

struct MSG_SKILL_REM_ACK : public MSG_SKILL_REM_SYN
{
};

struct MSG_SKILL_ADD_ACK : public MSGBASE
{
	SKILLBASE SkillBase;
};

//KES
struct MSG_SKILL_DESTROY : public MSGBASE
{
	WORD		wSkillIdx;
	POSTYPE		TargetPos;
	BYTE		cbReason;	//�����ı� ����
};

struct MSG_SKILL_UPDATE : public MSGBASE
{
	SKILL_BASE SkillBase;
};

// 080417 LUJ, ��ų ���� ���� ����ü
struct MSG_SKILL_LIST : public MSGBASE
{
	DWORD		mSize;
	SKILL_BASE	mData[ 100 ];

	DWORD GetSize() const
	{
		return sizeof( MSG_SKILL_LIST ) - ( sizeof( mData ) / sizeof( *mData ) - mSize ) * sizeof( *mData );
	}
};

/*

struct MSG_SKILL_ADD_NACK : public MSGBASE
{
};
struct MSG_SKILL_MOVEINFO : public MSGBASE
{
	DWORD		dwSkillDBIdx;
	POSTYPE		wToPosition;
	POSTYPE		wFromPosition;
};
struct MSG_SKILL_DELINFO : public MSGBASE
{
	DWORD		dwSkillDBIdx;
	POSTYPE		wPosition;
};


struct MSG_SKILL_ADDINVDEL_SYN : public MSGBASE
{
	BYTE	bSkillType;				// 0,1,2
	DWORD	dwItemDBIdx;				// ������ db index
	POSTYPE	wItemPosition;				// ������ pos
};
struct MSG_SKILL_ADDINVDEL_ACK : public MSGBASE
{
	BYTE		bSkillType;				// 0,1,2
	POSTYPE		wItemPosition;				// ������ pos
	SKILLBASE	skill;
};
struct MSG_SKILL_DELINVADD_SYN : public MSGBASE
{
	BYTE	bSkillType;				// 0,1,2
	WORD 	wSkillIdx;					
	//WORD	wItemIdx;
	POSTYPE	wItemInvPosition;
};
struct MSG_SKILL_DELINVADD_ACK : public MSGBASE
{
	BYTE		bSkillType;				// 0,1,2
	ITEMBASE	itemBase;			
};
struct MSG_SKILL_DELGRDADD_SYN : public MSGBASE
{
	BYTE		bSkillType;				// 0,1,2
	WORD 		wSkillIdx;					
	//WORD		wItemIdx;
	VECTOR3		vItemGroundPos;
};
struct MSG_SKILL_DELGRDADD_ACK : public MSGBASE
{
	BYTE		bSkillType;				// 0,1,2
};*/

//////////////////////////////////////////////////////////////////
///////// ����â ���� ��Ʈ�� �޽��� /////////////////////////////////
/*
struct MSG_QUICK_ADD_SYN :  public MSGBASE
{
	WORD	wIconIdx;
	POSTYPE	wQuickAbsPosition;
	POSTYPE	wSrcAbsPosition;
};
struct MSG_QUICK_ADD_ACK : public MSGBASE
{
	WORD	wIconIdx;
	POSTYPE	wSrcAbsPosition;
	POSTYPE	wQuickAbsPosition;
};
struct MSG_QUICK_REM_SYN : public MSG_ITEM_REM_SYN
{
};
struct MSG_QUICK_REM_ACK : public MSGBASE
{
	POSTYPE	wQuickAbsPosition;
	POSTYPE	wSrcAbsPosition;
};

struct MSG_QUICK_USE_SYN :  public MSGBASE
{
	POSTYPE	wQuickRelPosition;
	POSTYPE	wSrcPosition;
};
struct MSG_QUICK_MOVE_SYN : public MSGBASE
{
	POSTYPE		wToAbsPosition;
	POSTYPE		wFromAbsPosition;
};
struct MSG_QUICK_MOVE_ACK : public MSGBASE
{
	POSTYPE		wToAbsPosition;
	POSTYPE		wFromAbsPosition;
};
struct MSG_QUICK_UPD_ACK : public MSGBASE
{
	POSTYPE		wAbsPosition;
	POSTYPE		wSrcPosition;
};
*/


//KES EXCHANGE 030923
////////////////////
//��ȯâ ������ ����
struct MSG_LINKITEM : public MSG_ITEM
{
	POSTYPE		wAbsPosition;
};

//SW050920 Rare
struct MSG_LINKITEM_ADD : public MSG_LINKITEM	//��ȯ�� �ɼ������� �Ѱ��ٶ� ����.
{
	BYTE				bPetInfo;
	//BYTE				bOptionItem;
	//BYTE				bRareOptionItem;
	ITEM_OPTION			sOptionInfo;
	//ITEM_RARE_OPTION_INFO sRareOptionInfo;

	//MSG_LINKITEM_ADD() : bPetInfo(0), bOptionItem(0), bRareOptionItem(0) {}
	//int GetSize()
	//{
	//	if(!bRareOptionItem)
	//	{
	//		return sizeof(MSG_LINKITEM_ADD) - sizeof(ITEM_OPTION)*(1-(bOptionItem));
	//	}
	//	else
	//	{
	//		return sizeof(MSG_LINKITEM_ADD);
	//	}
	//}
};

struct MSG_LINKITEMEX : public MSG_LINKITEM
{
	MONEYTYPE	dwMoney;
};

struct MSG_LINKBUYITEMEX : public MSG_LINKITEMEX
{
	WORD wVolume;
};

struct MSG_ITEMEX : public MSG_ITEM
{
	//BYTE count;
	//BYTE RareCount;
	BYTE PetInfoCount;
	ITEM_OPTION	sOptionInfo;
	//ITEM_RARE_OPTION_INFO sRareOptionInfo;

	MSG_ITEMEX() {memset(this, 0, sizeof(MSG_ITEMEX));}

	//int GetSize()
	//{
	//	if(!RareCount)
	//	{
	//		return sizeof(MSG_LINKITEM_ADD) - sizeof(ITEM_OPTION)*(1-(count));
	//	}
	//	else
	//	{
	//		return sizeof(MSG_LINKITEM_ADD);
	//	}
	//}
};

struct MSG_BUYITEM : public MSG_ITEMEX
{
	DWORD dwData;
};

struct MSG_LOOTINGIEM : public MSG_LINKITEM
{
	DWORD				dwDiePlayerIdx;
	//BYTE				bOptionItem;
	//BYTE				bRareOptionItem;
	BYTE				bPetSummonItem;
	ITEM_OPTION			sOptionInfo;
	//ITEM_RARE_OPTION_INFO	sRareOptionInfo;
	
	//MSG_LOOTINGIEM() : bOptionItem(0), bRareOptionItem(0), bPetSummonItem(0) {}
	//int GetSize()
	//{
	//	if(!bRareOptionItem)
	//	{
	//		return sizeof(MSG_LOOTINGIEM) - sizeof(ITEM_OPTION)*(!bOptionItem);
	//	}
	//	else
	//	{
	//		return sizeof(MSG_LOOTINGIEM);
	//	}
	//}
};

struct SEND_LINKITEM_TOTALINFO : public MSG_LINKITEMEX
{
	//BYTE count;
	//BYTE RareCount;
	BYTE PetInfoCount;
	ITEM_OPTION sOptionInfo;
	//ITEM_RARE_OPTION_INFO sRareOptionInfo;

	//int GetSize()
	//{
	//	if(!RareCount)
	//	{
	//		return sizeof(SEND_LINKITEM_TOTALINFO) - sizeof(ITEM_OPTION)*(!count);
	//	}
	//	else
	//	{
	//		return sizeof(SEND_LINKITEM_TOTALINFO);
	//	}
	//}
};

struct SEND_LINKBUYITEM_TOTALINFO : public SEND_LINKITEM_TOTALINFO
{
	WORD wVolume;
};

struct MSG_EXCHANGE_REMOVEITEM : public MSGBASE
{
	POSTYPE		wAbsPosition;
};

// 071026 LYW --- CommonStruct : Add struct to change item count.
struct MSG_EXCHANGE_DECREASEITEM : public MSGBASE
{
	POSTYPE		wAbsPosition ;
	int			nItemCount ;
} ;

struct MSG_ITEM_DESTROY : public MSGBASE
{
	POSTYPE		wAbsPosition;
	BYTE		cbReason;
};


//////////////////////////////////////

struct REGIST_MAP : public MSGBASE
{
	WORD mapServerPort;
	MAPTYPE mapnum;
};


struct REGIST_BASEECONOMY
{
	MAPTYPE MapNum;
	WORD OriginNum;
	WORD OriginPrice;
	WORD OriginAmount;
	WORD RequireNum;
	WORD RequirePrice;
	WORD RequireAmount;
	BYTE BuyRates;
	BYTE SellRates;
};
//////////////////////////////////////////////////////////////////////////

struct AUCTION_LISTFIELD
{
	WORD ItemIdx;
	WORD SellNum;
	WORD SellerID;
	WORD EndDate;
	WORD EndTime;
	DWORD StartPrice;
	DWORD ImmediatePrice;
	char Memo[256];
};

struct AUCTION_ONPAGELIST : public MSGBASE
{
	AUCTION_LISTFIELD Auction_List[10];
};

struct AUCTION_NEW_BOARDCONTENTS : public MSGBASE
{	
	WORD ItemIdx;
	WORD ItemNum;
	DWORD SellerID;
	DWORD EndDate;
	DWORD EndTime;
	DWORD StartPrice;
	DWORD ImmediatePrice;
	WORD MemoLength;
	char Memo[256];

	WORD GetMsgLength()	{	return sizeof(AUCTION_NEW_BOARDCONTENTS) - 256 + MemoLength + 1;	}
};
struct AUCTION_BOARDCONTENTS_INFO : public MSGBASE
{
	DWORD BidIdx;		// ��Ź�ȣ(�ʼ������� �����ϴ� PrimaryKey)
	WORD ItemIdx;
	WORD ItemNum;
	DWORD SellerID;
	DWORD EndDate;
	DWORD EndTime;
	DWORD StartPrice;
	DWORD ImmediatePrice;

	DWORD CurrentPrice;
	DWORD BidderNum;
	DWORD CurrentBidder;

	WORD MemoLength;
	char Memo[256];
};

struct AUCTION_BOARDPAGE_INFO : public MSGBASE
{
	WORD wCurPage;
	WORD wTotalPageNum;
	AUCTION_BOARDTITLE Title[MAX_BOARDTITLE_NUM];
};

// LBS ������ ����..
struct MSG_STREETSTALL_TITLE : public MSGBASE
{
	char Title[MAX_STREETSTALL_TITLELEN+1];
	WORD StallKind;
};

struct MSG_STREETSTALL_ITEMSTATUS : public MSG_ITEM
{
	POSTYPE		wAbsPosition;
	MONEYTYPE		dwMoney;
	WORD	wVolume;
};


struct STREETSTALLITEM
{
	WORD		wVolume; // ���ų����� ����
	DWORD		wIconIdx;
	DWORD		dwDBIdx;
	DURTYPE		Durability;
	DWORD		dwRareIdx;
	MONEYTYPE		money;
	char		Locked;			// ����
	char		Fill;
	// 071215 LYW --- CommonStruct : ���ξ������� ������ �÷��� ��, ������ ������ ���� �߰��ߴ�.
	int			nSeal ;

	ITEMPARAM ItemParam;
};

struct STREETSTALL_INFO : public MSGBASE
{
	DWORD StallOwnerID;
	char Title[MAX_STREETSTALL_TITLELEN+1];
	STREETSTALLITEM Item[SLOT_STREETSTALL_NUM];
	WORD StallKind;

	WORD count;
	WORD RareCount;
	WORD PetItemCount;
	
	CAddableInfoList AddableInfo;

	WORD GetSize()
	{
		return sizeof( STREETSTALL_INFO ) - sizeof(CAddableInfoList) + AddableInfo.GetInfoLength();
	}
};

struct STREETSTALLTITLE_INFO : public MSGBASE
{
	char Title[MAX_STREETSTALL_TITLELEN+1];
};

struct STREETSTALL_BUYINFO : public MSG_ITEM
{
	DWORD	StallOwnerID;
	POSTYPE StallPos;
	DWORD	dwBuyPrice;
};

// LBS StreetStall 03.11.27
struct MSG_REGISTITEM : public MSG_ITEM
{
	DWORD dwData;
};

struct MSG_REGISTITEMEX : public MSG_REGISTITEM
{
	MONEYTYPE dwMoney;
};

struct MSG_REGISTBUYITEMEX : public MSG_REGISTITEMEX
{
	WORD wVolume;
	WORD wAbsPosition;
};

struct MSG_SELLITEM : public MSG_REGISTITEM
{
	DWORD count;
};

struct MSG_STALLMSG	: public MSGBASE
{
	DWORD	SellerID;
	DWORD	BuyerID;
	DWORD	ItemIdx;
	WORD	count;
	DWORD	money;
};

struct ITEM_SELLACK_INFO : public MSGBASE
{
	//DWORD ItemDBIdx;			// check�� ����  �ӽ� ����
	MONEYTYPE Money;
	POSTYPE Pos;
};

//struct APPEARANCE_INFO : public MSGBASE
struct MSG_APPERANCE_ADD : public MSGBASE
{
	//DWORD	PlayerID;
	DWORD	mSlotIndex;
	DWORD	mItemIndex;

	//DWORD WearedItem[eWearedItem_Max];
};


struct MSG_APPERANCE_REMOVE : public MSGBASE
{
	DWORD	mSlotIndex;
};


//pjs
//��� �˻� ���.
struct SEARCHLIST : public MSGBASE
{
	SEARCH_TYPE search_type;
	SORT_TYPE sort_type;
	char name[15];
	int pagenum;
};
//��� ���� ���.
struct SORTLIST : public MSGBASE
{
	SORT_TYPE sort_type;
	char name[15];
	int pagenum;
};

//��� ���, ���� ��� 
struct AUCTIONCANCEL : public MSGBASE
{
	CANCEL_TYPE cancel_type;
	int regIndex;
	char name[15];
	
};

//��� ��� ���� 
struct  REGISTERAUCTION : public MSGBASE
{
	int	 PRO_Index;
	int  PRO_Amount;
	char BID_DUEDATE[10];
	int  BID_HighPrice;
	int  BID_ImmediatePrice;
	char Auctioneer[15];
};

//��� ���� ���� 
struct JOINAUCTION : public MSGBASE
{
	int REG_Index;
	int BID_Price;
	char JOIN_Name[15];	
};

//��� ���� ������ ������ ����ü
struct REAUCTIONSUCCESS 
{
	char PRO_Name[15];
	int PRO_Amount;
	char Auctioneer[15];
	int PRO_Index;
	
};

//�޾ƿ� ������ ���� ����ü
struct AUCTIONSUCCESS : public MSGBASE
{	//���߿� ��ġ�� ����Ʈ�� 
	REAUCTIONSUCCESS resultlist[10];	
};

//����Ʈ ������ �޾ƿ� ����ü
struct REBASELIST 
{
	int REG_Index;
	char PRO_Name[15];
	int  PRO_Amount;
	int  BID_HighPrice;
	int  BID_ImmediatePrice;
	DWORD BID_DUEDATE[10];
	char BID_HighName[15];
	int  JOIN_Amount;
	DWORD  BID_LeftTime;
	char Auctioneer[15];
	int	 PRO_Index;
};
//�޾ƿ� ������ ���� ����ü
struct BASELIST : public MSGBASE
{
	REBASELIST imfolist[10];
};




///////////////////////////////////////

struct MSG_LEVEL : public MSGBASE
{
	enum EXPPOINTKIND
	{
		EXPPOINTKIND_ACQUIRE,
		EXPPOINTKIND_DIE,
	};
	LEVELTYPE Level;
	EXPTYPE CurExpPoint;
	EXPTYPE MaxExpPoint;
};
struct MSG_EXPPOINT : public MSGBASE
{
	enum EXPPOINTKIND
	{
		EXPPOINTKIND_ACQUIRE,
		EXPPOINTKIND_DIE,
	};
	EXPTYPE ExpPoint;
	BYTE ExpKind;
};
struct MSG_SKILLEXP : public MSGBASE
{
	EXPTYPE ExpPoint;
	POSTYPE absPos;
};
struct M2C_DAMAGE_MSG : public MSGBASE
{
	DWORD dwDamage;
};


//////////////////////////////////////////////////////////////////////////
// Skill ����
struct MAINTARGET
{
	enum
	{
		MAINTARGETKIND_OBJECTID,
		MAINTARGETKIND_POS,
	};
	BYTE MainTargetKind;
	union {
		DWORD dwMainTargetID;		
		COMPRESSEDPOS cpTargetPos;
	};
	void SetMainTarget(DWORD id)
	{	MainTargetKind = MAINTARGETKIND_OBJECTID;	dwMainTargetID = id;	}
	void SetMainTarget(VECTOR3* pp)
	{	MainTargetKind = MAINTARGETKIND_POS;	cpTargetPos.Compress(pp);	}
	MAINTARGET()	{}
	MAINTARGET(DWORD id)	{	SetMainTarget(id);	}
	MAINTARGET(VECTOR3* pp){	SetMainTarget(pp);	}
};

struct SKILLOBJECT_INFO
{
	DWORD SkillObjectIdx;
	DWORD SkillIdx;

	VECTOR3 Pos;
	DWORD StartTime;
	DIRINDEX Direction;
	BYTE SkillLevel;
	DWORD Operator;

	MAINTARGET MainTarget;

	DWORD BattleID;
	WORD Option;
	float Rate;
	DWORD RemainTime;
	WORD Count;
};

enum SKILLKIND
{
	SKILLKIND_PHYSIC,
	SKILLKIND_MAGIC,
	SKILLKIND_PASSIVE,
	SKILLKIND_ONOFF,
	SKILLKIND_BUFF,
	SKILLKIND_MAX
};

enum UNITKIND
{
	UNITKIND_NONE,
	UNITKIND_PHYSIC_ATTCK,
	UNITKIND_MAGIC_ATTCK,
	UNITKIND_LIFE_RECOVER,
	UNITKIND_RESURRECTION,
	UNITKIND_DEBUFF,
	UNITKIND_FAKE_DAMAGE,
	UNITKIND_DECOY,
	UNITKIND_MANA_RECOVER,
	UNITKIND_RETURN,
	UNITKIND_SPELLBREAK,
	// 080616 LUJ, ���� ��ȯ �߰�
	UNITKIND_TRAP,
	UNITKIND_MAX,
};

#define	MAX_SKILL_LEVEL		30
#define MAX_BUFF_COUNT		5
#define MAX_STATUS_COUNT	3

// 070215 LYW --- CommonStruct : Add struct for conduct part.
struct CONDUCT_INFO
{
	char	ConductName[MAX_NAME_LENGTH+1] ;

	WORD	ConductIdx ;
	WORD	ConductTootipIdx ;
	WORD	ConductKind ;
	WORD	ConductPos ;

	int		HighImage ;

	int		MotionIdx ;
} ;

struct SKILL_INFO
{
	/// ��ų �ε���
	DWORD	Index;
	/// ��ų �̸�
	char	Name[MAX_SKILL_NAME_LENGTH+1];
	/// ���� �ε���
	DWORD	Tooltip;
	
	/// �̹��� ������
	int		Image;

	/// ��ų ����
	WORD	Kind;
	WORD	Attribute;

	/// ��ų ����
	WORD	Level;

	/// �ʿ� ���� ������
	WORD	EquipItem[2];

	/// ��Ȳ�� ����
	BOOL	Battle;
	BOOL	Peace;
	BOOL	Move;
	BOOL	Stop;
	BOOL	Rest;

	BOOL	Die;


	/// �Ҹ� ǰ��
	WORD	Item[2];
	WORD	ItemCount[2];
	DWORD	Money;

	// 080616 LUJ, ����/HP �ҿ䰡 ������ �ƴ϶� �ۼ�Ʈ�ε� �����ϰ� �Ѵ�
	struct Value
	{
		float mPercent;
		float mPlus;
	};

	Value	mLife;
	Value	mMana;
};

struct BUFF_SKILL_INFO : public SKILL_INFO
{
	/// ���� �ð�
	DWORD	DelayTime;

	/// ��ų ����Ʈ
	int		SkillEffect;

	// 080618 LUJ, � ������ ���� ��ų���� ��Ÿ��. enum������ ����
	eStatusKind	Status;

	// 080616 LUJ, ���� � ���·� ó���ϴ��� ��Ÿ��. enum������ ����
	enum DataType
	{
		StatusTypeNone,
		StatusTypeAdd,
		StatusTypePercent,
	}
	StatusDataType;

	float	StatusData;

	/// �ߵ� ����
	WORD	ActiveRule;
	WORD	ActiveRuleType;
	WORD	ActiveRuleData;

	WORD	Count;
	BYTE	CountType;

	// 080219 LUJ, �Ⱓ�� ��ų ����
	BOOL	IsEndTime;
	// 080616 LUJ, �̺�Ʈ�� �ߵ��� ��ų
	DWORD	mEventSkillIndex;
};

struct ACTIVE_SKILL_INFO : public SKILL_INFO
{
	/// ��ų ����Ʈ
	int		OperatorEffect;
	int		TargetEffect;
	int		SkillEffect;

	/// ���� �ð�
	DWORD	AnimationTime;
	DWORD	CastingTime;
	WORD	DelayType;
	DWORD	DelayTime;
	DWORD	CoolTime;

	/// ��� ����
	WORD	Target;

	WORD	Range;

	// 080602 LUJ, �������� ���� enum������ ����
	TARGET_KIND			Area;
	// 080602 LUJ, �������� ���� enum������ ����
	eSkillAreaTarget	AreaTarget;
	WORD				AreaData;
	//WORD				AreaData[3];
	WORD	Unit;
	WORD	UnitDataType;
	WORD	UnitData;

	/// ��ų Ȯ��
	WORD	Success;
	WORD	Cancel;

	/// ���� ����
	WORD	TrainPoint;
	DWORD	TrainMoney;

	/// ���� ����
	DWORD	Buff[ MAX_BUFF_COUNT ];
	WORD	BuffRate[ MAX_BUFF_COUNT ];

	DWORD	RequiredBuff;

	BOOL	IsMove;

	// 080310 LUJ, ���� ���� ����
	DWORD	RequiredPlayerLevel;
	
	// 080602 LUJ, ��ų ��� �� �Ҹ�Ǵ� ������
	ITEMBASE	mConsumeItem;

	// 080602 LUJ, ��ų ��� �� �Ҹ�Ǵ� ����Ʈ
	struct ConsumePoint
	{
		// 080602 LUJ, �Ҹ�Ǵ� ����Ʈ ����
		enum Type
		{
			TypeNone,
			TypeGuild,
			TypeFamily,
			TypePlayerKill,
			TypeExperience,
		}
		mType;

		DWORD	mValue;
	}
	mConsumePoint;
};

struct EXCEPTION
{
	DWORD	Index;
	DWORD	BuffIndex;
	WORD	TargetKind;
	DWORD	TargetIndex;
	WORD	Type;
	WORD	Grade;
};

enum eExceptionTargetKind
{
	eExceptionKind_None,
	eExceptionKind_Item,
	eExceptionKind_Active,
	eExceptionKind_Buff,
	eExceptionKind_Max,
};

enum eExceptionType
{	
	eExceptionType_Possible = 1,
	eExceptionType_Impossible = 2,
	eExceptionType_Destroy = 1,
	eExceptionType_Interrupt = 2,
};

#define MAX_SKILL_TREE 255

struct SKILL_TREE_INFO
{
	WORD	ClassIndex;
	WORD	Count;
	DWORD	SkillIndex[ MAX_SKILL_TREE ];
};
//////////////////////////////////////////////////////////////////////////
// 06. 06. 2�� ���� - �̿���
// ���� ��ȯ �߰�
struct SKILLOPTION{
	WORD Index;				// �ɼ� Index
	WORD SkillKind;			// �������� ��ų ����
	WORD OptionKind;		// �ɼ� ����
	WORD OptionGrade;		// �ɼ� ���
	WORD ItemIndex;			// ������ Index

	int Life;				// �����
	int Mana;			// ����
	float PhyDef;			// ��������
	float AttDef;			// �Ӽ�����

	int Range;				// �����Ÿ�
	float ReduceMana;	// ���¼Ҹ�
	float PhyAtk;			// ��������
	float BaseAtk;			// �⺻��������
	float AttAtk;			// �Ӽ�����

	long Duration;			// ���ӽð�
};

struct SKILLSTATSOPTION		// ���ݿ� ����Ǵ� �ɼǵ�
{
	int Life;				// �����
	int Mana;			// ����
	float PhyDef;			// ��������
	float AttDef;			// �Ӽ�����

	int Range;				// �����Ÿ�
	float ReduceMana;	// ���¼Ҹ�
	float PhyAtk;			// ��������
	float BaseAtk;			// �⺻��������
	float AttAtk;			// �Ӽ�����
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 06. 06. 2�� ���� - �̿���
// ����/����
// Ư�����¿� ����ü
struct SPECIAL_STATE_INFO{
	WORD	Index;

	DWORD	TickTime;		// �����ð�
	WORD	Mana;		// �����ð��� �Ҹ𳻷�

	DWORD	DelayTime;		// ���� ��� �ð�
	DWORD	DurationTime;	// ���ӽð�

	WORD	IconIdx;		// ���� ������ ��ȣ
};
//////////////////////////////////////////////////////////////////////////
#include ".\TargetList\TargetList.h"
#include ".\TargetList\TargetListIterator.h"
struct MSG_SKILL_START_SYN : public MSGBASE
{
	DWORD SkillIdx;
	DWORD Operator;
	// 080602 LUJ, ����� ������ ����. �������� ��ȸ�Ϸ��� �ð��� ���� �ɸ��Ƿ� Ŭ���̾�Ʈ���� �Ѵ�
	ITEMBASE	mConsumeItem;
	DIRINDEX SkillDir;
	MAINTARGET MainTarget;
	CTargetList TargetList;

	void InitMsg(DWORD wSkillIdx,MAINTARGET* pMainTarget,float AngleDeg,DWORD pOperator)
	{
		Category = MP_SKILL;
		Protocol = MP_SKILL_START_SYN;
		SkillIdx = wSkillIdx;
		TargetList.Clear();
		SkillDir = DEGTODIR(AngleDeg);
        Operator = pOperator;
		memcpy(&MainTarget,pMainTarget,sizeof(MAINTARGET));
	}
	WORD GetMsgLength()
	{
		return sizeof(MSG_SKILL_START_SYN) - sizeof(CTargetList) + TargetList.GetTotalMsgLen();
	}
};

// 071203 KTH --- ��ų�� ����ϴ� �������� ����Ҷ� �޴´�.
struct MSG_ITEM_SKILL_START_SYN : public MSGBASE
{
	MSG_SKILL_START_SYN	SkillStartSyn;
	POSTYPE	Pos;
	DWORD	Idx;
};

// 080602 LUJ, ������ ��� ��ų �ߵ�
struct MSG_ITEM_SKILL_START_TO_MAP : public MSGBASE
{
	SKILL_BASE	mSkill;
	DWORD		mGuildIndex;
	DWORD		mFamilyIndex;
};
struct MSG_SCRIPT_CHECK : public MSGBASE
{
	ScriptCheckValue mCheckValue;
};
struct	MSG_SKILL_START_SYN2 : public MSGBASE
{
	DWORD SkillIdx;
	DWORD Operator;
	DIRINDEX SkillDir;
	MAINTARGET MainTarget;

	void InitMsg(DWORD wSkillIdx,MAINTARGET* pMainTarget,float AngleDeg,DWORD pOperator)
	{
		Category = MP_SKILL;
		Protocol = MP_SKILL_START_SYN;
		SkillIdx = wSkillIdx;
		SkillDir = DEGTODIR(AngleDeg);
        Operator = pOperator;
		memcpy(&MainTarget,pMainTarget,sizeof(MAINTARGET));
	}
};

struct MSG_SKILL_OPERATE : public MSGBASE
{
	DWORD SkillObjectID;
	DWORD RequestorID;
	MAINTARGET MainTarget;
	CTargetList TargetList;

	void InitMsg(BYTE bProtocol,MAINTARGET* pMainTarget)
	{
		Category = MP_SKILL;
		Protocol = bProtocol;
		TargetList.Clear();
		memcpy(&MainTarget,pMainTarget,sizeof(MAINTARGET));
	}
	WORD GetMsgLength()
	{
		return sizeof(MSG_SKILL_OPERATE) - sizeof(CTargetList) + TargetList.GetTotalMsgLen();
	}
};

struct MSG_SKILLOBJECT_ADD : public MSGBASE
{
	SKILLOBJECT_INFO SkillObjectInfo;
	bool bCreate;
	CTargetList TargetList;

	void InitMsg(SKILLOBJECT_INFO* pSkillObjectInfo,bool Create)
	{
		Category = MP_SKILL;
		Protocol = MP_SKILL_SKILLOBJECT_ADD;
		memcpy(&SkillObjectInfo,pSkillObjectInfo,sizeof(SkillObjectInfo));
		SkillObjectInfo.StartTime = gCurTime - SkillObjectInfo.StartTime;
		TargetList.Clear();
		bCreate = Create;
	}
	WORD GetMsgLength()
	{
		return sizeof(MSG_SKILLOBJECT_ADD) - sizeof(CTargetList) + TargetList.GetTotalMsgLen();
	}
};

struct MSG_SKILLOBJECT_ADD2 : public MSGBASE
{
	SKILLOBJECT_INFO SkillObjectInfo;
	bool bCreate;

	void InitMsg(SKILLOBJECT_INFO* pSkillObjectInfo,bool Create)
	{
		Category = MP_SKILL;
		Protocol = MP_SKILL_SKILLOBJECT_ADD;
		memcpy(&SkillObjectInfo,pSkillObjectInfo,sizeof(SkillObjectInfo));
		SkillObjectInfo.StartTime = gCurTime - SkillObjectInfo.StartTime;
		bCreate = Create;
	}
};

struct MSG_SKILL_SINGLE_RESULT : public MSGBASE
{
	DWORD SkillObjectID;
	DWORD SingleUnitNum;
	CTargetList TargetList;

	void InitMsg(DWORD SOID,DWORD SUNum)
	{
		Category = MP_SKILL;
		Protocol = MP_SKILL_SKILL_SINGLE_RESULT;
		SkillObjectID = SOID;
		SingleUnitNum = SUNum;		
		TargetList.Clear();
	}
	WORD GetMsgLength()
	{
		return sizeof(MSG_SKILL_SINGLE_RESULT) - sizeof(CTargetList) + TargetList.GetTotalMsgLen();
	}

};

struct MSG_SKILL_RESULT : public MSGBASE
{
	DWORD SkillObjectID;
	bool SkillDamageKind;
	CTargetList TargetList;

	void InitMsg(DWORD SOID)
	{
		Category = MP_SKILL;
		Protocol = MP_SKILL_SKILL_RESULT;
		SkillObjectID = SOID;
		TargetList.Clear();
	}
	WORD GetMsgLength()
	{
		return sizeof(MSG_SKILL_SINGLE_RESULT) - sizeof(CTargetList) + TargetList.GetTotalMsgLen();
	}

};

struct MSG_SKILL_UPDATE_TARGET : public MSGBASE
{
	DWORD SkillObjectID;
	CTargetList TargetList;
	WORD Range;
	WORD IDx;

	void InitMsg(BYTE bProtocol, DWORD SOID, WORD GetRange, WORD GetSkillID)
	{
		Category = MP_SKILL;
		Protocol = bProtocol;
		SkillObjectID = SOID;
		Range = GetRange;
		IDx = GetSkillID;
		TargetList.Clear();
	}
	WORD GetMsgLength()
	{
		return sizeof(MSG_SKILL_SINGLE_RESULT) - sizeof(CTargetList) + TargetList.GetTotalMsgLen();
	}
};

struct MSG_SKILL_DAMAGE_TARGETLIST : public MSGBASE
{
	DWORD SkillObjectID;
	DWORD RequestorID;
	MAINTARGET MainTarget;
	CTargetList TargetList;
	
	void InitMsg(BYTE bProtocol,MAINTARGET* pMainTarget)
	{
		Category = MP_SKILL;
		Protocol = bProtocol;
		TargetList.Clear();
		memcpy(&MainTarget,pMainTarget,sizeof(MAINTARGET));
	}
	WORD GetMsgLength()
	{
		return sizeof(MSG_SKILL_DAMAGE_TARGETLIST) - sizeof(CTargetList) + TargetList.GetTotalMsgLen();
	}
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ��� ����
struct KYUNGGONG_INFO
{
	WORD KyungGongIdx;		//���� index
	char KyungGongName[MAX_NAME_LENGTH+1];		//����̸�
	WORD NeedMana;		//�Ҹ� ����
	WORD MoveType;			//�̵� Ÿ��
	float Speed;			//�̵� �ӵ�
	WORD ChangeTime;		//��� ��� ��ȯ �ð�
	int StartEffect;		//��� ��� ����
	int IngEffect;			//��� ���� ����
	int EndEffect;			//��� �� ����
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//���� ����
struct MUNPA_BOARD_INFO
{
	char MunpaBoardName[MAX_MUNPA_BOARD_NAME+1];
	DWORD RSubjectRank;
	DWORD RContentRank;
	DWORD WContentRank;
	DWORD DContentRank;
	DWORD BoardIDX;
};

struct MUNPA_BATTLERECORD_INFO 
{
	DWORD Win;
	DWORD Loss;
	DWORD Total;
}; 

struct MUNPA_HOME_INFO : public MSGBASE
{
	char IntroMsg[MAX_MUNPA_INTROMSG+1];
	DWORD MunpaID;
	DWORD BoardNum;
	MUNPA_BOARD_INFO BoardInfo[MAX_MUNPA_BOARD_NUM];
	BYTE Kind;
	
	char MunpaName[MAX_MUNPA_NAME+1];
	char MasterName[MAX_NAME_LENGTH+1];
	FAMETYPE MunpaFamous;
	BYTE MemberNum;
	char CreateDay[16];
	MUNPA_BATTLERECORD_INFO MunpaBattleRecordInfo[MAX_MUNPA_BATTLEKIND];
	BOOL bIsInMap;
};

struct MUNPALISTINFO_MAP : public MSGBASE
{
	DWORD MunpaID;
	DWORD MasterID;
	char MunpaName[MAX_MUNPA_NAME+1];
	FAMETYPE Famous;
	BYTE MemberNum;
	DWORD MunpaRank;
	MONEYTYPE MunpaMoney;
};

struct MUNPALISTINFO
{
	DWORD MunpaID;
	char MunpaMasterName[MAX_NAME_LENGTH+1];
	char MunpaName[MAX_MUNPA_NAME+1];
	FAMETYPE Famous;
	BYTE MemberNum;
	BYTE Kind;
};

struct SEND_MUNPACCEPT_INFO : public MSGBASE
{
	DWORD MunpaID;
	DWORD TargetID;
	BOOL bLast;
	char MunpaName[MAX_MUNPA_NAME+1];
};

struct SEND_MUNPA_ACCEPT_TO_MAP : public MSGBASE
{
	DWORD MunpaID;
	char MunpaName[MAX_MUNPA_NAME+1];
};

struct SEND_MUNPA_INFO : public MSGBASE
{
	MUNPALISTINFO Munpa[MAX_MUNPA_LIST];
	DWORD TotalResultNum;
};

struct SEND_MUNPAMEMBER_RANK_CHANGE : public MSGBASE
{
	DWORD TargetID;
	DWORD MunpaID;
	DWORD Rank;
};

struct SEND_MUNPAMEMBER_RANK_CHANGE_ACK : public MSGBASE
{
	DWORD MunpaID;
	BYTE Rank;
	FAMETYPE Fame;
};

struct SEND_MUNPALIST_SYN : public MSGBASE
{
	DWORD GotoPage;
	char OrderType[16];
};

struct MUNPA_SEARCH_SYN : public MSGBASE
{
	char SearchKey[32];
	DWORD GotoPage;
};

struct MUNPA_CREATE_INFO : public MSGBASE
{
	char MunpaName[MAX_MUNPA_NAME+1];
	char MunpaIntro[MAX_MUNPA_INTROMSG+1];
	BYTE Kind;
};

struct MUNPA_MODIFY_INTRO : public MSGBASE
{
	char MunpaIntro[MAX_MUNPA_INTROMSG+1];
	DWORD MunpaID;
};

struct MUNPA_SEMIMEMBER_INFO
{
	DWORD id;
	char Name[MAX_NAME_LENGTH+1];
	LEVELTYPE Level;
	FAMETYPE Famous;
	char RequestJoinDay[16];
};

struct SEND_MUNPA_SEMIMEMBER_LIST : public MSGBASE
{
	MUNPA_SEMIMEMBER_INFO MunpaMemberList[MAX_MUNPA_SEMIMEMBERLIST_NUM];
	DWORD TotalResultNum;
	DWORD MunpaID;
};

struct MUNPA_REGULARMEMBER_INFO
{
	DWORD id;
	char Name[MAX_NAME_LENGTH+1];
	DWORD Rank;
	LEVELTYPE Level;
	FAMETYPE Famous;
	BOOL bLogin;
	char lastvisitdate[16];
	char entrydate[16];
};

struct SEND_MUNPA_REGULARMEMBER_LIST : public MSGBASE
{
	MUNPA_REGULARMEMBER_INFO MunpaRegularMemberList[MAX_MUNPA_SEMIMEMBERLIST_NUM];
	DWORD TotalResultNum;
	DWORD MunpaID;
};

struct MUNPA_BOARD_NAME_NUM 
{
	char BoardName[MAX_MUNPA_BOARD_NAME+1];
	DWORD WritingsNum;
};

struct SEND_MUNPA_BOARD_INFO : public MSGBASE
{
	DWORD CurMunpaBoardNum;
	DWORD BoardIDX[MAX_MUNPA_BOARD_NUM];
	MUNPA_BOARD_NAME_NUM BoardNameNum[MAX_MUNPA_BOARD_NUM];
};

struct SEND_MUNPA_CREATE_INFO : public MSGBASE
{
	char MunpaName[MAX_MUNPA_NAME+1];
	DWORD dwData;
};

struct SEND_MUNPA_BOARD_RANK_INFO : public MSGBASE
{
	char MunpaBoardName[MAX_MUNPA_BOARD_NAME+1];
	DWORD RSubjectRank;
	DWORD RContentRank;
	DWORD WContentRank;
	DWORD DContentRank;
	DWORD ID; // C->S : MUNPAIDX , S->C : BOARDIDX but modify�� c->s : BOARDIDX
};

struct SEND_BOARD_CONTENT_INFO : public MSGBASE
{
	DWORD BoardIDX;
	DWORD RootContentsID;
	char Subject[MAX_MUNPA_BOARD_SUBJECT+1];
	char Content[MAX_MUNPA_BOARD_CONTENT+1];
	WORD GetMsgLength()
	{
		return sizeof(SEND_BOARD_CONTENT_INFO) - (MAX_MUNPA_BOARD_CONTENT+1) + strlen(Content)+1;
	}
};

struct BOARD_LIST_INFO
{
	char WriterName[MAX_NAME_LENGTH+1];
	char RegDate[32];
	int Count; //��ȸ��
	char Subject[MAX_MUNPA_BOARD_SUBJECT+1];
	int  Seq; //�۹�ȣ
	DWORD WritingsIDX;
	BYTE Depth;
};

struct SEND_MUNPA_BOARD_LIST_INFO : public MSGBASE
{
	BOARD_LIST_INFO BoardListInfo[MAX_MUNPA_BOARD_LIST];
	DWORD TotalPage;
};

struct SEND_MUNPA_BOARD_CONTENT_INFO : public MSGBASE
{
	BOARD_LIST_INFO BoardListInfo;
	BYTE DCRank;
	BYTE WCRank;
	char Content[MAX_MUNPA_BOARD_CONTENT+1];
	WORD GetMsgLength()
	{
		return sizeof(SEND_MUNPA_BOARD_CONTENT_INFO) - (MAX_MUNPA_BOARD_CONTENT+1) + strlen(Content)+1;
	}
};

struct MUNPA_WAREHOUSE_ITEM_INFO
{
//	ITEMBASE WarehouseItem[MAX_MUNPA_WAREITEM_PERTAB_NUM];
	ITEMBASE WarehouseItem[TABCELL_GUILDWAREHOUSE_NUM];
	MONEYTYPE money;
};

struct SEND_MUNPA_WAREHOUSE_ITEM_INFO : public MSGBASE
{
	MUNPA_WAREHOUSE_ITEM_INFO info;
	WORD wOptionCount;
	WORD wRareOptionCount;
	WORD wPetInfoCount;
	BYTE bTabNum;

	CAddableInfoList AddableInfo;

	WORD GetSize()
	{
		return sizeof( SEND_MUNPA_WAREHOUSE_ITEM_INFO ) - sizeof(CAddableInfoList) + AddableInfo.GetInfoLength();
	}
};

struct STORAGELIST
{
	DWORD MaxCellNum;
	MONEYTYPE MaxMoney;
	MONEYTYPE BuyPrice;
};

struct SEND_STORAGE_ITEM_INFO : public MSGBASE
{
	MONEYTYPE money;
	ITEMBASE StorageItem[SLOT_STORAGE_NUM];

	WORD wOptionCount;
	WORD wRareOptionCount;
	WORD wPetInfoCount;

	CAddableInfoList AddableInfo;

	WORD GetSize()
	{
		return sizeof( SEND_STORAGE_ITEM_INFO ) - sizeof(CAddableInfoList) + AddableInfo.GetInfoLength();
	}
	
};

struct MSG_FRIEND_MEMBER_ADDDELETEID : public MSGBASE
{
	DWORD PlayerID;
	char Name[MAX_NAME_LENGTH+1];
};

struct FRIEND
{
	char Name[MAX_NAME_LENGTH+1];
	DWORD Id;
	BOOL IsLoggIn;
};

struct FRIEND_LIST
{
	FRIEND Friend[MAX_FRIEND_LIST];
	int totalnum;
};

struct MSG_FRIEND_LIST_DLG : public MSGBASE
{
	BYTE count;
	FRIEND FriendList[MAX_FRIEND_NUM];
	WORD GetMsgLength()
	{
		return sizeof(MSG_FRIEND_LIST_DLG) - (sizeof(FRIEND) * (MAX_FRIEND_NUM - count));
	}
};

struct MSG_FRIEND_SEND_NOTE : public MSGBASE
{
	DWORD FromId;
	char FromName[MAX_NAME_LENGTH+1];
	char ToName[MAX_NAME_LENGTH+1];
	char Note[MAX_NOTE_LENGTH+1];
	WORD GetMsgLength()
	{
		return sizeof(MSG_FRIEND_SEND_NOTE) - (MAX_NOTE_LENGTH+1) + strlen(Note)+1;
	}
};

struct MSG_GUILD_SEND_NOTE : public MSGBASE
{
	DWORD FromId;
	char FromName[MAX_NAME_LENGTH+1];
	char Note[MAX_NOTE_LENGTH+1];
	WORD GetMsgLength()
	{
		return sizeof(MSG_GUILD_SEND_NOTE) - (MAX_NOTE_LENGTH+1) + strlen(Note)+1;
	}
};

//struct MSG_FAMILY_SEND_NOTE : public MSGBASE
//{
//	DWORD FromId;
//	char FromName[MAX_NAME_LENGTH+1];
//	char Note[MAX_NOTE_LENGTH+1];
//	WORD GetMsgLength()
//	{
//		return sizeof(MSG_FAMILY_SEND_NOTE) - (MAX_NOTE_LENGTH+1) + strlen(Note)+1;
//	}
//};

struct MSG_FRIEND_SEND_NOTE_ID : public MSGBASE
{
	DWORD TargetID;
	char FromName[MAX_NAME_LENGTH+1];
	char Note[MAX_NOTE_LENGTH+1];
	WORD GetMsgLength()
	{
		return sizeof(MSG_FRIEND_SEND_NOTE_ID) - (MAX_NOTE_LENGTH+1) + strlen(Note)+1;
	}
};

struct FRIEND_NOTE
{
	//note�� ���� �ҷ��´�. 
	char FromName[MAX_NAME_LENGTH+1];
	DWORD NoteID;
	char SendDate[16];
	BYTE bIsRead;
};

struct MSG_FRIEND_NOTE_LIST : public MSGBASE
{
	FRIEND_NOTE NoteList[NOTENUM_PER_PAGE];
	BYTE TotalPage;
};

struct MSG_FRIEND_DEL_NOTE : public MSGBASE
{
	DWORD NoteID;
	BOOL bLast;
};

struct MSG_FRIEND_READ_NOTE : public MSGBASE
{
	char FromName[MAX_NAME_LENGTH+1];
	
	DWORD NoteID;
	WORD ItemIdx;
	char Note[MAX_NOTE_LENGTH+1];
	WORD GetMsgLength()
	{
		return sizeof(MSG_FRIEND_READ_NOTE) - (MAX_NOTE_LENGTH+1) + strlen(Note)+1;
	}
};

//////////////////////////////////////////////////////////////////////////
//ä��
struct MSG_CHANNEL_INFO : public MSGBASE
{
	WORD PlayerNum[MAX_CHANNEL_NUM];
	char ChannelName[MAX_CHANNEL_NAME+1];
	DWORD dwUniqueIDinAgent;
	BYTE Count;
};

struct MSG_CHANNEL_INFO_MORNITOR : public MSGBASE
{
	MSG_CHANNEL_INFO	ChannelInfo;
	char				sServerIP[MAX_IPADDRESS_SIZE];
	WORD				wServerPort;
};
//////////////////////////////////////////////////////////////////////////


//------------ AUTOPATCH ���� �޽��� ----------------------
typedef struct tagServerTraffic : public MSGROOT
{
	LONG UserNum;
	int Returnkey;
	
}ServerTraffic;


//////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// ä�ð��ø޼��� //////////////////////////////////////

struct MSG_CHAT : public MSGBASE
{
	char	Name[MAX_NAME_LENGTH+1];
	char	Msg[MAX_CHAT_LENGTH+1];
	int		GetMsgLength() { return sizeof(MSG_CHAT) - MAX_CHAT_LENGTH + strlen(Msg); }
};

struct MSG_CHAT_WITH_SENDERID : public MSGBASE
{
	DWORD	dwSenderID;
	char	Name[MAX_NAME_LENGTH+1];
	char	Msg[MAX_CHAT_LENGTH+1];
	int		GetMsgLength() { return sizeof(MSG_CHAT_WITH_SENDERID) - MAX_CHAT_LENGTH + strlen(Msg); }
};

struct MSG_WHISPER : public MSGBASE
{
	DWORD	dwReceiverID;
	char	SenderName[MAX_NAME_LENGTH+1];
	char	ReceiverName[MAX_NAME_LENGTH+1];
	char	Msg[MAX_CHAT_LENGTH+1];
	int		GetMsgLength() { return sizeof(MSG_WHISPER) - MAX_CHAT_LENGTH + strlen(Msg); }
};

struct SEND_PARTY_CHAT : public MSGBASE
{
	DWORD	MemberID[MAX_PARTY_LISTNUM];
	BYTE	MemberNum;

	char	Name[MAX_NAME_LENGTH+1];
	char	Msg[MAX_CHAT_LENGTH+1];
	int		GetMsgLength() { return sizeof(SEND_PARTY_CHAT) - MAX_CHAT_LENGTH + strlen(Msg); }
};

// 070105 LYW --- Add struct for chatting of family system.
struct SEND_FAMILY_CHAT : public MSGBASE
{
	DWORD	MemberID[ MAX_FAMILY_LISTNUM ] ;
	BYTE	MemberNum ;

	char	Name[ MAX_NAME_LENGTH + 1 ] ;
	char	Msg[ MAX_CHAT_LENGTH + 1 ] ;
	int		GetMsgLength() { return sizeof( SEND_FAMILY_CHAT ) - MAX_CHAT_LENGTH + strlen( Msg ) ; }
} ;

struct MSG_CHAT_WORD : public MSGBASE
{
	WORD	wData;
	char	Msg[MAX_CHAT_LENGTH+1];
	int		GetMsgLength() { return sizeof(MSG_CHAT_WORD) - MAX_CHAT_LENGTH + strlen(Msg); }
};

/*
struct SEND_PARTY_CHAT : public MSG_CHAT
{
	DWORD MemberID[MAX_PARTY_LISTNUM];
	BYTE MemberNum;
};
*/


enum eChatError
{
	CHATERR_ERROR,
	CHATERR_NO_NAME,
	CHATERR_NOT_CONNECTED,
	CHATERR_OPTION_NOWHISPER,
};


//KES EXCHANGE 030922
//struct MSG_EXCHANGE_APPLY : public MSGBASE
//{
//	BYTE bAccept;	
//};


//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Battle System 

struct BATTLE_TEAMMEMBER_INFO
{
	DWORD MemberID;
	char MemberName[MAX_NAME_LENGTH+1];
	void set(DWORD id,char* name)
	{
		MemberID = id;
//		strcpy(MemberName,name);
		strncpy( MemberName, name, MAX_NAME_LENGTH );
		MemberName[MAX_NAME_LENGTH] = 0;

		MemberKillNum = 0;
		MemberDieNum = 0;
	}
	BYTE MemberKillNum;
	BYTE MemberDieNum;
};

struct BATTLE_INFO_BASE
{
	DWORD BattleID;
	BYTE BattleKind;
	BYTE BattleState;
	DWORD BattleTime;
};

struct MUNPAFIELD_TEAMINFO
{
	BYTE TotalKill;
	BYTE TotalDied;
	BATTLE_TEAMMEMBER_INFO TeamMember[MAX_MUNPAFIELD_MEMBERNUM];

	BATTLE_TEAMMEMBER_INFO* FindMember(DWORD id)
	{
		for(int n=0;n<MAX_MUNPAFIELD_MEMBERNUM;++n)
		{
			if(TeamMember[n].MemberID == id)
				return &TeamMember[n];
		}
		return NULL;
	}
	void AddTeamMember(DWORD id)
	{		
		for(int n=0;n<MAX_MUNPAFIELD_MEMBERNUM;++n)
		{
			if(TeamMember[n].MemberID == 0)
			{
				TeamMember[n].MemberID = id;
				break;
			}
		}
	}
};

struct MSG_BATTLESETTING_SYN_MUNPAFIELD : public BATTLE_INFO_BASE
{
	MUNPAFIELD_TEAMINFO TeamInfo[2];
};

//---------------------------------------------------------------------------------------
// Murim Field
struct MURIMFIELD_TEAMINFO
{
	BYTE TotalKill;
	BYTE TotalDied;
	BATTLE_TEAMMEMBER_INFO TeamMember[MAX_MURIMFIELD_MEMBERNUM];

	BATTLE_TEAMMEMBER_INFO* FindMember(DWORD id)
	{
		for(int n=0;n<MAX_MURIMFIELD_MEMBERNUM;++n)
		{
			if(TeamMember[n].MemberID == id)
				return &TeamMember[n];
		}
		return NULL;
	}
	void AddTeamMember(DWORD id)
	{		
		for(int n=0;n<MAX_MURIMFIELD_MEMBERNUM;++n)
		{
			if(TeamMember[n].MemberID == 0)
			{
				TeamMember[n].MemberID = id;
				break;
			}
		}
	}
};

struct MURIMFIELD_BATTLE_INFO
{
	WORD	wMurimBattleKind;	// ���� ���� ���� ����
	WORD	wMurimBattleReward;	// ���� ���� ����
};

struct MSG_BATTLESETTING_SYN_MURIMFIELD : public BATTLE_INFO_BASE
{
	MURIMFIELD_BATTLE_INFO	MurimBattleInfo;
	MURIMFIELD_TEAMINFO		TeamInfo[3];
};
//---------------------------------------------------------------------------------------


#define MAX_BATTLE_INFO_LENGTH	2048
struct MSG_BATTLE_INFO : public MSGBASE
{
	BYTE BattleKind;
	char info[MAX_BATTLE_INFO_LENGTH];
	WORD msglen;

	int GetMsgLength()
	{
		return sizeof(MSGBASE) + msglen + sizeof(BYTE);
	}
};

struct MSG_BATTLE_TEAMMEMBER_ADDDELETE : public MSGBASE

{
	BATTLE_TEAMMEMBER_INFO Member;
	BYTE Team;
};


//==============
//For Showdown
struct MSG_BATTLE_SHOWDOWN_CREATESTAGE : public MSGBASE
{
	DWORD	dwBattleID;
	VECTOR3 vPosStage;		//�̰� ������. �Ʒ��� �ٲ���.
//	COMPRESSEDPOS vPosStage
};
//===============


struct MSG_MONEY : public MSGBASE
{
	MONEYTYPE dwTotalMoney;		// ��ü �ݾ�
	BYTE bFlag;					// MsgFlag
};

//////////////////
//---for MURIM NET
struct MNPLAYER_BASEINFO
{
	DWORD		dwObjectID;
	WORD		wRankPoint;
	char		strPlayerName[MAX_NAME_LENGTH];
	LEVELTYPE	Level;
	char		strNick[MAX_NAME_LENGTH];
	WORD		wPlayCount;
	WORD		wWin;
	WORD		wLose;
	char		strMunpa[MAX_NAME_LENGTH];
	char		cbPositionInMunpa;
	char		cbTeam;
};

struct MSG_MNPLAYER_BASEINFO : public MSGBASE
{
	MNPLAYER_BASEINFO	Info;
};

struct MSG_MNPLAYER_BASEINFOLIST : public MSGBASE
{
	DWORD				dwTotalPlayerNum;

	MNPLAYER_BASEINFO	PlayerInfo[MAXPLAYER_IN_CHANNEL];

	DWORD GetMsgLength()
	{
		return sizeof(MSG_MNPLAYER_BASEINFOLIST)
				- sizeof(MNPLAYER_BASEINFO)*(MAXPLAYER_IN_CHANNEL - dwTotalPlayerNum);
	}	
};

struct CHANNEL_BASEINFO
{
	DWORD		dwChannelIndex;
	char		strChannelTitle[MAX_CHANNELTITLE_LENGTH];
	char		cbChannelKind;
	WORD		wMaxPlayer;
	WORD		wPlayerNum;
};

struct MSG_CHANNEL_BASEINFO : public MSGBASE
{
	CHANNEL_BASEINFO	Info;
};

struct MSG_CHANNEL_BASEINFOLIST : public MSGBASE
{
	DWORD				dwTotalChannelNum;

	CHANNEL_BASEINFO	ChannelInfo[MAXCHANNEL_IN_MURIMNET];

	DWORD GetMsgLength()
	{
		return sizeof(MSG_CHANNEL_BASEINFOLIST)
				- sizeof(CHANNEL_BASEINFO)*(MAXCHANNEL_IN_MURIMNET - dwTotalChannelNum);
	}
};

struct PLAYROOM_BASEINFO
{
	DWORD		dwPlayRoomIndex;
	char		strPlayRoomTitle[MAX_PLAYROOMTITLE_LENGTH];
	char		cbPlayRoomKind;
	WORD		wMaxObserver;
	WORD		wMaxPlayerPerTeam;
	MONEYTYPE	MoneyForPlay;
	WORD		wPlayerNum;
	char		cbStart;
	MAPTYPE		wMapNum;
};

struct MSG_PLAYROOM_BASEINFO : public MSGBASE
{
	PLAYROOM_BASEINFO	Info;
};

struct MSG_PLAYROOM_BASEINFOLIST : public MSGBASE
{
	DWORD				dwTotalPlayRoomNum;

	PLAYROOM_BASEINFO	PlayRoomInfo[MAXPLAYROOM_IN_MURIMNET];

	DWORD GetMsgLength()
	{
		return sizeof(MSG_PLAYROOM_BASEINFOLIST)
				- sizeof(PLAYROOM_BASEINFO)*(MAXPLAYROOM_IN_MURIMNET - dwTotalPlayRoomNum);
	}		
};

struct MSG_MNTEAMCHANGE : public MSGBASE
{
	DWORD	dwMoverID;
	BYTE	cbFromTeam;
	BYTE	cbToTeam;
};


//---������ (to Murim Server)
struct MSPLAYER_INFO
{
	DWORD	dwObjectID;
	BYTE	cbTeam;
};
struct MSG_MNPLAYROOM_INFO : public MSGBASE
{
	int		nIndex;
	int		nGameKind;
	int		nTotalPlayerNum;
	MAPTYPE wMapNum;

	MSPLAYER_INFO PlayerInfo[100];

	WORD GetMsgLength()
	{
		return sizeof(MSG_MNPLAYROOM_INFO) - sizeof(MSPLAYER_INFO)*(100 - nTotalPlayerNum);
	}
};

struct WANTEDINFO
{
	WANTEDTYPE WantedIDX;
	MONEYTYPE TotalPrize;
	DWORD WantedChrIDX;
	char WantedChrName[MAX_NAME_LENGTH+1];
	DWORD RegistChrIDX;
	int TotalVolunteerNum;
};

struct WANTEDINFO_LOAD
{
	WANTEDINFO m_WantedInfo[MAX_QUERY_RESULT];
	int count;
};

struct SEND_WANTEDINFO_TO_SERVER : public MSGBASE
{
	WANTEDINFO Info;
};

struct WANTEDLIST
{
	WANTEDTYPE WantedIDX;
	DWORD WantedChrID;
	char WantedName[MAX_NAME_LENGTH+1];
	char RegistDate[11];
};

struct SEND_WANTEDLIST_TO_CLIENT : public MSGBASE
{
	WANTEDLIST List;
};

struct SEND_WANTEDLIST : public MSGBASE
{
	WORD WantedIdx; // himself
	WANTEDLIST List[MAX_WANTED_NUM];
};

struct SEND_WANTED_NOTCOMPLETE : public MSGBASE
{
	WANTEDTYPE WantedIDX;
	char CompleterName[MAX_NAME_LENGTH+1];
	BYTE type;
};

struct SEND_WANTED_ORDER_SYN : public MSGBASE
{
	WORD page;
	BYTE type;
};

struct WANTNPCLIST
{
	WANTEDTYPE WantedIDX;
	char WantedName[MAX_NAME_LENGTH+1];
	MONEYTYPE Prize;
	BYTE VolunteerNum;
};

struct SEND_WANTNPCLIST : public MSGBASE
{
	WANTNPCLIST WantNpcList[WANTUNIT_PER_PAGE];
	WORD TotalPage;
};

struct SEND_GUILD_CREATE_SYN : public MSGBASE
{
	char GuildName[MAX_GUILD_NAME+1];
	char Intro[MAX_GUILD_INTRO+1];
	int GetMsgLength()
	{
		return sizeof(SEND_GUILD_CREATE_SYN) - (MAX_GUILD_INTRO+1) + strlen(Intro)+1;
	}
};
struct GUILDINFO
{
	DWORD GuildIdx;
	DWORD MasterIdx;
	char MasterName[MAX_NAME_LENGTH+1];
	char GuildName[MAX_GUILD_NAME+1];
	// 06. 03. ���İ��� - �̿���
	//char GuildNotice[MAX_GUILD_NOTICE+1];
	DWORD	UnionIdx;
	BOOL	mIsUnionMaster;
	MAPTYPE MapNum;
	BYTE GuildLevel;
	MARKNAMETYPE MarkName;
};


struct GUILDMEMBERINFO
{
	DWORD MemberIdx;
	char MemberName[MAX_NAME_LENGTH+1];
	LEVELTYPE Memberlvl;
	BYTE Rank;
	BOOL bLogged;

	// 080225 LUJ, ���� ���� ����
	BYTE	mJobGrade;
	BYTE	mJob[ MAX_JOB_GRADE ];
	BYTE	mRace;
};


struct SEND_GUILD_CREATE_ACK : public MSGBASE
{
	DWORD GuildIdx;
	char GuildName[MAX_GUILD_NAME+1];
};


struct SEND_GUILD_CREATE_NOTIFY : public MSGBASE
{
	GUILDINFO		GuildInfo;
	LEVELTYPE		MasterLvl;

	// 080225 LUJ, ������ ȸ�� ����
	GUILDMEMBERINFO	mMaster;
};


struct SEND_GUILD_MEMBER_INFO : public MSGBASE
{
	DWORD GuildIdx;
	char GuildName[MAX_GUILD_NAME+1];	
	GUILDMEMBERINFO MemberInfo;
};
struct SEND_GUILD_TOTALINFO : public MSGBASE
{
	GUILDINFO GuildInfo;
	//SW060719 ��������Ʈ
	//GUILDPOINT_INFO PointInfo;
	int membernum;
	GUILDMEMBERINFO MemberInfo[MAX_GUILD_MEMBER];
	void InitGuildInfo( const GUILDINFO& pInfo)
	{
		GuildInfo.GuildIdx = pInfo.GuildIdx;
		GuildInfo.GuildLevel = pInfo.GuildLevel;
		strcpy(GuildInfo.GuildName, pInfo.GuildName);
		GuildInfo.MapNum = pInfo.MapNum;
		GuildInfo.MasterIdx = pInfo.MasterIdx;
		// 06. 03. ���İ��� - �̿���
		//strcpy(GuildInfo.GuildNotice, pInfo.GuildNotice);
		GuildInfo.UnionIdx = pInfo.UnionIdx;
		strcpy(GuildInfo.MasterName, pInfo.MasterName);
		GuildInfo.MarkName = pInfo.MarkName;
	}
	//void InitPointInfo(GUILDPOINT_INFO* pInfo)
	//{
	//	memcpy(&PointInfo, pInfo, sizeof(GUILDPOINT_INFO));
	//}
	int GetMsgLength() { return sizeof(SEND_GUILD_TOTALINFO) - (MAX_GUILD_MEMBER-membernum)*sizeof(GUILDMEMBERINFO);	}
};
struct SEND_GUILD_INVITE : public MSGBASE
{
	DWORD MasterIdx;
	char MasterName[MAX_NAME_LENGTH+1];
	char GuildName[MAX_GUILD_NAME+1];
};

struct SEND_GUILD_NICKNAME : public MSGBASE
{
	DWORD TargetIdx;
	char NickName[MAX_GUILD_NICKNAME+1];
};

struct SEND_GUILD_NAME : public MSGBASE
{
	char GuildName[MAX_GUILD_NAME+1];
};

struct SEND_GUILDMEMBER_LOGININFO : public MSGBASE
{
	DWORD MemberIdx;
	BOOL bLogIn;
};

struct SEND_GUILD_HUNTEDMOB_INFO : public MSG_DWORD2
{};

#define GUILDMARKBPP	2
#define GUILDMARKWIDTH	16
#define GUILDMARKHEIGHT	16
#define GUILDMARK_BUFSIZE	(GUILDMARKBPP*GUILDMARKWIDTH*GUILDMARKHEIGHT)
#define GUILDUNIONMARKHEIGHT	16
#define GUILDUNIONMARK_BUFSIZE	(GUILDMARKBPP*GUILDMARKWIDTH*GUILDUNIONMARKHEIGHT)

struct MSG_GUILDMARK_IMG : public MSGBASE
{
	DWORD GuildIdx;
	MARKNAMETYPE MarkName;
	char imgData[GUILDMARK_BUFSIZE];
};

struct MSG_GUILDUNIONMARK_IMG : public MSGBASE
{
	DWORD	dwMarkIdx;
	DWORD	dwGuildUnionIdx;
	char	imgData[GUILDUNIONMARK_BUFSIZE];
};


//struct MSG_GUILDWAR_INFO : public MSGBASE
//{
//	DWORD		dwEnemyGuildNum;
//	GUILDINFO	EnemyGuildInfo[100];
//	
//	MSG_GUILDWAR_INFO()
//	{
//		dwEnemyGuildNum = 0;
//	}
//	void	AddEnemyGuildInfo( const GUILDINFO& pInfo, DWORD dwMoney = 0 )
//	{
//		if( dwEnemyGuildNum < 100 )
//		{
//			EnemyGuildInfo[dwEnemyGuildNum] = pInfo;
//			EnemyGuildInfo[dwEnemyGuildNum].MarkName = dwMoney;
//			++dwEnemyGuildNum;
//		}
//	}
//	int		GetMsgLength()	{ return sizeof(MSG_GUILDWAR_INFO) - (100-dwEnemyGuildNum)*sizeof(GUILDINFO); }
//};


// 071015 ����, ���Ͱ��� ������ ���������� ���� ���� �̸��� �˷��ֵ��� MSG_GUILDWAR_INFO�� Ȯ���
struct MSG_GUILD_LIST : public MSGBASE
{
	DWORD mSize;

	struct Data : public GUILDINFO
	{
		char mUnionName[ MAX_NAME_LENGTH + 1 ];
	}
	mData[ 100 ];

	DWORD GetSize() const
	{
		return sizeof( MSG_GUILD_LIST ) - ( sizeof( mData ) / sizeof( Data ) - mSize ) * sizeof( Data );
	};
};


// 071017 ����, ������� ����� �ٸ� �ʼ����� �����Ѵ�.
struct MSG_GUILD_WAR_RESULT : public MSGBASE
{
	struct Data
	{
		DWORD	mGuildIndex;
		DWORD	mScoreValue;		// �ֽ� ����
		float	mScoreVariation;	// ��ȭ�� ��
	};
	
	Data	mWinner;
	Data	mLoser;
};


// 070801 ����. �ʿ� �÷��̾ ���ӽ� ��� ��� ȸ�� ������ �����Ͽ� ��´�
//struct MSG_GUILD_MEMBER_ALL : public MSGBASE
//{
//	DWORD mSize;
//
//	struct Member
//	{
//		DWORD	mIndex;
//		char	mName[ MAX_NAME_LENGTH + 1 ];
//		DWORD	mLevel;
//		DWORD	mRank;
//		BOOL	mIsLogin;
//	}
//	mMember[ MAX_GUILD_MEMBER ];
//
//	DWORD GetSize() const
//	{
//		return sizeof( MSG_GUILD_MEMBER_ALL ) - ( sizeof( mMember ) / sizeof( Member ) - mSize ) * sizeof( Member );
//	}
//};


// 070801 ����. �÷��̾� ���� ��, �� ���� �÷��̾�� ���ӵ� ���� ������ �����Ѵ�.
//struct MSG_GUILD_MEMBER_LOGIN : public MSGBASE
//{
//	DWORD mMapIndex;
//	DWORD mSize;	
//	DWORD mPlayerIndex[ MAX_GUILD_MEMBER ];
//
//	DWORD GetSize() const
//	{
//		return sizeof( MSG_GUILD_MEMBER_LOGIN ) - ( sizeof( mPlayerIndex ) / sizeof( DWORD ) - mSize ) * sizeof( DWORD );
//	}
//};
//
//// 070809 ����, �÷��̾� �α׾ƿ� ��, �� ������ �˸���.
//struct MSG_GUILD_MEMBER_LOGOUT : public MSGBASE
//{
//	DWORD	mGuildIndex;	// �Ҽӵ� ���
//	DWORD	mPlayerIndex;	// ���������� �÷��̾�
//};


// 070801 ����. �÷��̾� ���� ��, ����� �� ������ �����Ͽ� ��´�
struct MSG_GUILD_DATA : public MSGBASE
{
	DWORD	mIndex;	
	DWORD	mLevel;
	MAPTYPE	mLocation;
	DWORD	mUnionIndex;
	DWORD	mScore;
	DWORD	mWarehouseSize;
	DWORD	mWarehouseRank;
	char	mName	[ MAX_GUILD_NAME	+ 1 ];
	char	mIntro	[ MAX_GUILD_INTRO	+ 1 ];
	char	mMark	[ MAX_NAME_LENGTH	+ 1 ];	// TODO: ���� ��ũ�� ���ؼ� ���� ���� �ʿ�
	char	mNotice	[ MAX_GUILD_NOTICE  + 1 ];

	DWORD			mMemberSize;
	GUILDMEMBERINFO	mMember[ MAX_GUILD_MEMBER ];

	DWORD GetSize() const
	{
		return sizeof( MSG_GUILD_DATA ) - ( sizeof( mMember ) / sizeof( GUILDMEMBERINFO ) - mMemberSize ) * sizeof( GUILDMEMBERINFO );
	}
};


// 070801 ����, �� �ʿ� �α��ε� ��� ȸ�� ������ �����ϵ��� ��û�Ѵ�.
struct MSG_GUILD_MEMBER_SEARCH : public MSGBASE
{
	DWORD	mGuildIndex;
};


// 070809 ����, �������� ��� ������ ��´�.
struct MSG_GUILD_LEVEL_UP_ACK : public MSGBASE
{
	DWORD	mGuildIndex;
	DWORD	mGuildScore;
	DWORD	mUsedMoney;
};


// 070808 ����, ��� â�� ���ٿ� ���� ��� ��ȯ
struct MSG_GUILD_OPEN_WAREHOUSE : public MSGBASE
{
	DWORD	mPlayerIndex;
	char	mName[ MAX_NAME_LENGTH ];
	DWORD	mMoney;
	USHORT	mWarehouseSize;	// ��� â�� ũ��
	BYTE	mWarehouseRank;	// ��� â�� ���� ����
};


struct MSG_GUILD_SET_WAREHOUSE_RANK : public MSGBASE
{
	DWORD	mGuildIndex;
	BYTE	mRank;			// â�� ���� ���� ����
};


// 070808 ����, ��� ������ ����
struct MSG_GUILD_ITEM : public MSGBASE
{
	DWORD		mSize;
	ITEMBASE	mItem[ SLOT_GUILDWAREHOUSE_NUM ];

	DWORD GetSize() const
	{
		return sizeof( MSG_GUILD_ITEM ) - ( sizeof( mItem ) / sizeof( ITEMBASE ) - mSize ) * sizeof( ITEMBASE );
	}
};


// 070808 ����, ��� �������� �ɼ� ����
struct MSG_GUILD_ITEM_OPTION : public MSGBASE
{
	DWORD		mSize;
	ITEM_OPTION	mOption[ SLOT_GUILDWAREHOUSE_NUM ];

	DWORD GetSize() const
	{
		return sizeof( MSG_GUILD_ITEM_OPTION ) - ( sizeof( mOption ) / sizeof( ITEM_OPTION ) - mSize ) * sizeof( ITEM_OPTION );
	}
};


//struct SEND_FAMILY_CREATE_SYN : public MSGBASE
//{
//	char FamilyName[MAX_FAMILY_NAME+1];
//	char Intro[MAX_FAMILY_INTRO+1];
//	int GetMsgLength()
//	{
//		return sizeof(SEND_FAMILY_CREATE_SYN) - (MAX_FAMILY_INTRO+1) + strlen(Intro)+1;
//	}
//};
//
//
//enum eFamilyPlusTimeKindList
//{
//	eFamilyPlusTime_SuRyun_OneHalf,
//	eFamilyPlusTime_Skill_OneHalf,
//	eFamilyPlusTime_Exp_OneHalf,
//	eFamilyPlusTime_DamageUp_20Pct,
//	eFamilyPlusTime_Max,
//};
//
//enum eFamilyPlustTimeKind
//{
//	//eGPT_None,
//	eFPT_SuRyun,
//	eFPT_Skill,
//	eFPT_Exp,
//	eFPT_DamageUp,
//	eFPT_Kind_Max,
//};
//
//struct FAMILYUSING_PLUSTIME_INFO
//{
//	int PlusTimeIdx;
//	DWORD PlusTimeEndTime;
//};

//struct FAMILYPOINT_INFO
//{
//	FAMILYPOINT_INFO() {memset(this,0,sizeof(FAMILYPOINT_INFO));}
//
//	//��������Ʈ
//	int FamilyPoint;
//	//��ɸ�������
//	int FamilyHuntedMonsterCount;
//	//��ɸ���������
//	int FamilyHuntedMonsterTotalCount;
//	//DB������Ʈ �ð�
//	DWORD DBProcessTime;
//	//��빮���÷���Ÿ��
//	DWORD FamilyPlusTimeflg;
//	//�����÷���Ÿ�� ����
//	FAMILYUSING_PLUSTIME_INFO FamilyUsingPlusTimeInfo[eGPT_Kind_Max];
//	//��빫������(����)
//	//DWORD Family
//};

//struct FAMILYINFO
//{
//	DWORD FamilyIdx;
//	DWORD MasterIdx;
//	char MasterName[MAX_NAME_LENGTH+1];
//	char FamilyName[MAX_FAMILY_NAME+1];
//	// 06. 03. ���İ��� - �̿���
//	char FamilyNotice[MAX_FAMILY_NOTICE+1];
//	DWORD UnionIdx;
//	MAPTYPE MapNum;
//	BYTE FamilyLevel;
//	MARKNAMETYPE MarkName;
//};
//struct FAMILYMEMBERINFO
//{
//	DWORD MemberIdx;
//	char MemberName[MAX_NAME_LENGTH+1];
//	LEVELTYPE Memberlvl;
//	BYTE Rank;
//	BOOL bLogged;
//	void InitInfo(DWORD PlayerIDX, char* PlayerName, LEVELTYPE Playerlvl, BYTE PlayerRank = FAMILY_MEMBER, BOOL bLogin = FALSE)
//	{
//		MemberIdx = PlayerIDX;
//		strcpy(MemberName, PlayerName);
//		Memberlvl = Playerlvl;
//		Rank = PlayerRank;
//		bLogged = bLogin;
//	}
//};
//struct SEND_FAMILY_CREATE_ACK : public MSGBASE
//{
//	DWORD FamilyIdx;
//	char FamilyName[MAX_FAMILY_NAME+1];
//};
//struct SEND_FAMILY_CREATE_NOTIFY : public MSGBASE
//{
//	FAMILYINFO FamilyInfo;
//	LEVELTYPE MasterLvl;
//};
//struct SEND_FAMILY_MEMBER_INFO : public MSGBASE
//{
//	DWORD FamilyIdx;
//	char FamilyName[MAX_FAMILY_NAME+1];	
//	FAMILYMEMBERINFO MemberInfo;
//};
//struct SEND_FAMILY_TOTALINFO : public MSGBASE
//{
//	FAMILYINFO FamilyInfo;
//	//SW060719 ��������Ʈ
//	FAMILYPOINT_INFO PointInfo;
//	int membernum;
//	FAMILYMEMBERINFO MemberInfo[MAX_FAMILYMEMBER_NUM];
//	void InitFamilyInfo(FAMILYINFO* pInfo)
//	{
//		FamilyInfo.FamilyIdx = pInfo->FamilyIdx;
//		FamilyInfo.FamilyLevel = pInfo->FamilyLevel;
//		strcpy(FamilyInfo.FamilyName, pInfo->FamilyName);
//		FamilyInfo.MapNum = pInfo->MapNum;
//		FamilyInfo.MasterIdx = pInfo->MasterIdx;
//		// 06. 03. ���İ��� - �̿���
//		strcpy(FamilyInfo.FamilyNotice, pInfo->FamilyNotice);
//		FamilyInfo.UnionIdx = pInfo->UnionIdx;
//		strcpy(FamilyInfo.MasterName, pInfo->MasterName);
//		FamilyInfo.MarkName = pInfo->MarkName;
//	}
//	void InitPointInfo(FAMILYPOINT_INFO* pInfo)
//	{
//		memcpy(&PointInfo, pInfo, sizeof(FAMILYPOINT_INFO));
//	}
//	int GetMsgLength() { return sizeof(SEND_FAMILY_TOTALINFO) - (MAX_FAMILYMEMBER_NUM-membernum)*sizeof(FAMILYMEMBERINFO);	}
//};
//struct SEND_FAMILY_INVITE : public MSGBASE
//{
//	DWORD MasterIdx;
//	char MasterName[MAX_NAME_LENGTH+1];
//	char FamilyName[MAX_FAMILY_NAME+1];
//};
//
//struct SEND_FAMILY_NICKNAME : public MSGBASE
//{
//	DWORD TargetIdx;
//	char NickName[MAX_FAMILY_NICKNAME+1];
//};
//
//struct SEND_FAMILY_NAME : public MSGBASE
//{
//	char FamilyName[MAX_FAMILY_NAME+1];
//};
//
//struct SEND_FAMILYMEMBER_LOGININFO : public MSGBASE
//{
//	DWORD MemberIdx;
//	BOOL bLogIn;
//};
//
//struct SEND_FAMILY_HUNTEDMOB_INFO : public MSG_DWORD2
//{};

#define FAMILYMARKBPP	2
#define FAMILYMARKWIDTH	16
#define FAMILYMARKHEIGHT	12
#define FAMILYMARK_BUFSIZE	(FAMILYMARKBPP*FAMILYMARKWIDTH*FAMILYMARKHEIGHT)
#define FAMILYUNIONMARKHEIGHT	15
#define FAMILYUNIONMARK_BUFSIZE	(FAMILYMARKBPP*FAMILYMARKWIDTH*FAMILYUNIONMARKHEIGHT)

struct MSG_FAMILYMARK_IMG : public MSGBASE
{
	DWORD FamilyIdx;
	MARKNAMETYPE MarkName;
	char imgData[FAMILYMARK_BUFSIZE];
};

struct MSG_FAMILYUNIONMARK_IMG : public MSGBASE
{
	DWORD	dwMarkIdx;
	DWORD	dwFamilyUnionIdx;
	char	imgData[FAMILYUNIONMARK_BUFSIZE];
};


// RaMa ( 04.07.12 )
#pragma pack(1)

typedef DWORD	QSTATETYPE;
#define MAX_BIT ((sizeof(QSTATETYPE))*(8))

#define YEARTOSECOND	31536000
#define MONTHTOSECOND	2592000
#define DAYTOSECOND		86400
#define HOURTOSECOND	3600
#define MINUTETOSECOND	60

extern DWORD DayOfMonth[];
extern DWORD DayOfMonth_Yundal[];

//������������������������������������������������������������������
//��stTIME														  ��
//������������������������������������������������������������������
struct stTIME{
	QSTATETYPE		value;

	stTIME():value(0) {}

	void SetTime(DWORD time)	{	value = time;	}
	void SetTime(DWORD year, DWORD month, DWORD day, DWORD hour, DWORD minute, DWORD second)
	{
		value=0;
		QSTATETYPE ch=0;

		ch = year;
		value = (value | (ch<<28));
		ch = month;
		value = (value | (ch<<24));
		ch = day;
		value = (value | (ch<<18));
		ch = hour;
		value = (value | (ch<<12));
		ch = minute;
		value = (value | (ch<<6));
		ch = second;
		value = (value | ch);
	}

	//
	DWORD GetYear()		{	return value>>28;	}
	DWORD GetMonth()	{	DWORD msk = value<<4;		return msk>>28;		}
	DWORD GetDay()		{	DWORD msk = value<<8;		return msk>>26;		}
	DWORD GetHour()		{	DWORD msk = value<<14;		return msk>>26;		}
	DWORD GetMinute()	{	DWORD msk = value<<20;		return msk>>26;		}
	DWORD GetSecond()	{	DWORD msk = value<<26;		return msk>>26;		}
	inline BOOL	operator >(const stTIME& time);
	inline void operator +=(const stTIME& time);
	inline void operator -=(const stTIME& time);
	inline void operator =(const stTIME& time);
};

inline BOOL	 stTIME::operator >(const stTIME& time)
{
	BOOL bResult = FALSE;
	stTIME ctime = time;

	if( this->GetYear() == ctime.GetYear() )
	{
		if( this->GetMonth() > ctime.GetMonth() )
			bResult = TRUE;
		else if( this->GetMonth() == ctime.GetMonth() )
		{
			if( this->GetDay() > ctime.GetDay() )
				bResult = TRUE;
			else if( this->GetDay() == ctime.GetDay() )
			{
				if( this->GetHour() > ctime.GetHour() )
					bResult = TRUE;
				else if( this->GetHour() == ctime.GetHour() )
				{
					if( this->GetMinute() > ctime.GetMinute() )
						bResult = TRUE;
					else if( this->GetMinute() == ctime.GetMinute() )
						if( this->GetSecond() > ctime.GetSecond() )
							bResult = TRUE;
				}						
			}
		}
	}
	else if( this->GetYear() > ctime.GetYear() )
		bResult = TRUE;

	return bResult;
}

//---KES ETC 701020
inline void	 stTIME::operator +=(const stTIME& time)
{
	stTIME atime = time;
	int year, month, day, hour, minute, second, calcmonth;
	SYSTEMTIME systime;
	GetLocalTime(&systime);

	year = this->GetYear() + atime.GetYear();
	month = this->GetMonth() + atime.GetMonth();
	day = this->GetDay() + atime.GetDay();
	hour = this->GetHour() + atime.GetHour();
	minute = this->GetMinute() + atime.GetMinute();
	second = this->GetSecond() + atime.GetSecond();

	if( this->GetMonth() == 1 )			calcmonth = 11;
	else								calcmonth = this->GetMonth()-1;

	if(second >= 60)
	{
		++minute;
		second -= 60;
	}
	if(minute >= 60)
	{
		++hour;
		minute -= 60;
	}
	if(hour >= 24)
	{
		++day;
		hour -= 24;
	}
	if( (systime.wYear%4) == 0 )
	{
		if(day > (int)(DayOfMonth_Yundal[calcmonth]))
		{
			++month;
			day -= DayOfMonth_Yundal[calcmonth];
		}
	}
	else
	{
		if(day > (int)(DayOfMonth[calcmonth]))
		{
			++month;
			day -= DayOfMonth[calcmonth];
		}
	}
	if(month > 12)
	{
		++year;
		month -= 12;
	}
	
	this->SetTime(year, month, day, hour, minute, second);
}
inline void	 stTIME::operator -=(const stTIME& time)
{
	stTIME atime = time;
	int year, month, day, hour, minute, second, calcmonth;
	SYSTEMTIME systime;
	GetLocalTime(&systime);

	year = this->GetYear() - atime.GetYear();
	month = this->GetMonth() - atime.GetMonth();
	day = this->GetDay() - atime.GetDay();
	hour = this->GetHour() - atime.GetHour();
	minute = this->GetMinute() - atime.GetMinute();
	second = this->GetSecond() - atime.GetSecond();

	if( this->GetMonth() == 1 )			calcmonth = 11;
	else								calcmonth = this->GetMonth()-2;

	if(second < 0)
	{
		--minute;
		second += 60;
	}
	if(minute < 0)
	{
		--hour;
		minute += 60;
	}
	if(hour < 0)
	{
		--day;
		hour += 24;
	}
	if( (systime.wYear%4) == 0 )
	{
		if(day < 0)
		{
			--month;
			day += DayOfMonth_Yundal[calcmonth]; 
		}		
	}
	else
	{
		if(day < 0)
		{
			--month;
			day += DayOfMonth[calcmonth]; 
		}
	}
	if(month <= 0)
	{
		--year;
		month += 12;
	}
	
	this->SetTime(year, month, day, hour, minute, second);
}
//-------------------
inline void	 stTIME::operator =(const stTIME& time)
{
	stTIME atime = time;
	this->SetTime(atime.GetYear(), atime.GetMonth(), atime.GetDay(), atime.GetHour(), atime.GetMinute(), atime.GetSecond());
}
//



struct stPlayTime {
	DWORD value;

	stPlayTime::stPlayTime()
	{
		value = 0;
	}
	void GetTime(int& Year, int& Day, int& Hour, int& Minute, int& Second)
	{
		int mv = 0;

		Year = value/YEARTOSECOND;
		mv = value%YEARTOSECOND;

		Day = mv/DAYTOSECOND;
		mv = mv%DAYTOSECOND;

		Hour = mv/HOURTOSECOND;
		mv = mv%HOURTOSECOND;

		Minute = mv/MINUTETOSECOND;
		Second = mv%MINUTETOSECOND;		
	}
};


// LBS 04.01.06----------------------------------
typedef struct tagField{
	DWORD b0:1; 
	DWORD b1:1;
	DWORD b2:1;
	DWORD b3:1;
	DWORD b4:1;
	DWORD b5:1;
	DWORD b6:1;
	DWORD b7:1;

	DWORD b8:1;
	DWORD b9:1;
	DWORD b10:1;
	DWORD b11:1;
	DWORD b12:1;
	DWORD b13:1;
	DWORD b14:1;
	DWORD b15:1;

	DWORD b16:1;
	DWORD b17:1;
	DWORD b18:1;
	DWORD b19:1;
	DWORD b20:1;
	DWORD b21:1;
	DWORD b22:1;
	DWORD b23:1;

	DWORD b24:1;
	DWORD b25:1;
	DWORD b26:1;
	DWORD b27:1;
	DWORD b28:1;
	DWORD b29:1;
	DWORD b30:1;
	DWORD b31:1;

} stFIELD, *pFIELD;

typedef union tagQuestFlag{
	QSTATETYPE	value;
	stFIELD		flag;

	tagQuestFlag():value(0) {}

	void SetField(BYTE bit, BOOL bSetZero=FALSE) // nBit�� ��ȿ ���ڴ� DWORD�ΰ��1 ~ 32 �̴�
	{
		if( bit  < 1 || bit > MAX_BIT ) return;

		QSTATETYPE ch;
		bSetZero ? ch=1 : ch=0;
		value = (value | (ch<<(MAX_BIT- bit)));
	}

	BOOL IsSet(BYTE bit)
	{
		// ������ �������� ��� ���� ����Ǿ��ٰ� �˸���.
		if( bit  < 1 || bit > MAX_BIT ) return TRUE;

		QSTATETYPE ch = 1;
		return (value & (ch<<(MAX_BIT-bit)))? TRUE:FALSE;
	}

} QFLAG, *pQFLAG;


struct QBASE
{
	WORD		QuestIdx;
	QSTATETYPE	state;
};

struct QMBASE
{
	WORD		QuestIdx;	
	QFLAG		state;
	DWORD		EndParam;
	DWORD		Time;
	BYTE		CheckType;
	DWORD		CheckTime;
};

// 071023 LYW --- CommonStruct : Add struct for tutorial info.
struct TIBASE
{
	int			nTutorialIdx ;
	BOOL		IsComplete ;
} ;

struct QSUBASE
{
	WORD		QuestIdx;
	WORD		SubQuestIdx;
	QSTATETYPE	state;
	QSTATETYPE	time;
};

struct QITEMBASE
{
	DWORD		ItemIdx;
	WORD		Count;
	WORD		QuestIdx;
};

struct SEND_QUEST_IDX_SIMPLE : public MSGBASE
{
	DWORD MainQuestIdx;
};

struct SEND_QUEST_IDX : public MSGBASE
{
	WORD		MainQuestIdx;
	WORD		SubQuestIdx;
	DWORD		dwFlag;
	DWORD		dwData;
	DWORD		dwTime;
};

struct SEND_QUESTITEM_IDX : public MSGBASE
{
	DWORD	dwItemIdx;
	DWORD	dwItemNum;
};

struct SEND_QUEST_TOTALINFO : public MSGBASE
{
	int GetSize() {
		return (sizeof(SEND_QUEST_TOTALINFO) - sizeof(QBASE)*(50 - wCount));
	}

	WORD	wCount;
	QBASE	QuestList[50];
};

struct SEND_MAINQUEST_DATA : public MSGBASE
{
	int GetSize() {
		return (sizeof(SEND_MAINQUEST_DATA) - sizeof(QMBASE)*(100 - wCount));
	}

	WORD	wCount;
	QMBASE	QuestList[100];
};

// 071023 LYW --- CommonStruct : Add struct for tutorial data.
struct SEND_TUTORIAL_DATA : public MSGBASE
{
	int GetSize() {
		return (sizeof(SEND_TUTORIAL_DATA) - sizeof(TIBASE)*(100 - wCount));
	}

	WORD	wCount;
	TIBASE	TutorialList[100];
};

// 071106 LYW --- CommonStruct : Add structure for send message about update to default stats.
struct SEND_DEFAULT_STATS : public MSGBASE
{
	int nStr ;
	int nDex ;
	int nVit ;
	int nInt ;
	int nWis ;
	int nPoint ;

	int nErrorCode ;
} ;

struct SEND_SUBQUEST_DATA : public MSGBASE
{
	int GetSize() {
		return (sizeof(SEND_SUBQUEST_DATA) -sizeof(QSUBASE)*(100-wCount));
	}

	WORD	wCount;
	QSUBASE	QuestList[100];
};

struct SEND_SUBQUEST_UPDATE : public MSGBASE
{
	WORD	wQuestIdx;
	WORD	wSubQuestIdx;
	DWORD	dwMaxCount;
	DWORD	dwData;
	DWORD	dwTime;
};

struct SEND_QUESTITEM : public MSGBASE
{
	int GetSize() {
		return (sizeof(SEND_QUESTITEM)-sizeof(QITEMBASE)*(100-wCount));
	}

	WORD		wCount;
	QITEMBASE	ItemList[100];
};

struct QUESTINFO {
	DWORD	m_dwIdx;		// Quest idx
	QFLAG	m_flag;			// Quest State Value
};

struct JOURNALINFO
{
	DWORD Index;
	BYTE Kind;
	DWORD Param;
	DWORD Param_2;			// SubQuestIndex
	DWORD Param_3;			// Quest �������� �Ϸ�����
	BYTE bSaved;
	char ParamName[MAX_NAME_LENGTH+1];
	char RegDate[11];
};

struct SEND_JOURNALLIST : public MSGBASE
{
	WORD	wCount;
	JOURNALINFO m_Info[MAX_JOURNAL_ITEM];
	int GetSize() {
	return (sizeof(SEND_JOURNALLIST)-sizeof(JOURNALINFO)*(MAX_JOURNAL_ITEM-wCount));
	}

};

struct SEND_JOURNALINFO : public MSGBASE
{
	JOURNALINFO		m_Info;
};

struct SEND_JOURNAL_DWORD : public MSGBASE
{
	DWORD Param;
};




// PartyIcon
struct MoveCoord
{
	WORD posX;
	WORD posZ;

	MoveCoord::MoveCoord()
	{
		posX = posZ = 0;
	}
};

struct PARTYICON_MOVEINFO
{
	DWORD	dwMoverID;
	BYTE	tgCount;
	WORD	KyungGongIdx;
	BYTE	MoveState;
	MoveCoord	cPos;
	MoveCoord	tgPos[MAX_CHARTARGETPOSBUF_SIZE];

	PARTYICON_MOVEINFO::PARTYICON_MOVEINFO()
	{
		dwMoverID	= 0;
		tgCount		= 0;
		KyungGongIdx = 0;
		MoveState	= 0;
		memset(&cPos, 0, sizeof(MoveCoord));
		memset(tgPos, 0, sizeof(MoveCoord)*MAX_CHARTARGETPOSBUF_SIZE);
	}

	int GetSize()
	{
		return sizeof(PARTYICON_MOVEINFO)-sizeof(MoveCoord)*(MAX_CHARTARGETPOSBUF_SIZE-tgCount);
	}
};

struct SEND_PARTYICON_MOVEINFO : public MSGBASE
{
	PARTYICON_MOVEINFO MoveInfo;

	int GetSize()
	{
		return sizeof(MSGBASE)+MoveInfo.GetSize();
	}
};

struct SEND_PARTYICON_REVIVE : public MSGBASE
{
	DWORD		dwMoverID;
	MoveCoord	Pos;
};


// ��������������������������������������������������������������
// �� RaMa - ItemMall Item                                     ��
// ��������������������������������������������������������������
struct SHOP_ITEM
{
	ITEMBASE	Item[SLOT_SHOPITEM_NUM];
};

struct SHOPITEMBASE
{
	ITEMBASE	ItemBase;	
	DWORD		Param;				// ����� �ð��� �����θ� ���δ�. 1-����ð�, 2-�÷��̽ð�
	stTIME		BeginTime;
	DWORD		Remaintime;

	SHOPITEMBASE::SHOPITEMBASE()
	{
		memset(&ItemBase, 0, sizeof(ITEMBASE));
		Param = 0;
		BeginTime.value = 0;
		Remaintime=0;
	}
};

struct SHOPITEMWITHTIME
{
	SHOPITEMBASE	ShopItem;
	DWORD			LastCheckTime;

	SHOPITEMWITHTIME::SHOPITEMWITHTIME()
	{
		LastCheckTime = 0;
	}
};

struct SHOPITEMUSEBASE
{
	POSTYPE ShopItemPos;
	DWORD	ShopItemIdx;
	POSTYPE TargetItemPos;
	DWORD	TargetItemIdx;
};

struct SHOPITEM_SIMPLE
{
	ITEMBASE	Item;
	stTIME		EndTime;

	SHOPITEM_SIMPLE::SHOPITEM_SIMPLE()
	{
		memset(&Item, 0, sizeof(ITEMBASE));
		EndTime.value = 0;
	}
};

struct MSG_SHOPITEM_USE_SYN : public MSGBASE
{
	SHOPITEMUSEBASE UseBaseInfo;
};

struct SEND_SHOPITEM_BASEINFO : public MSGBASE
{
	//SHOPITEMBASE	ShopItemBase;
	ITEMBASE		ShopItemBase ;
	POSTYPE			ShopItemPos;
	DWORD			ShopItemIdx;
};


// 071122 LYW --- CommonStruct : ITEMBASE ���� �޽��� �߰�.
struct SEND_ITEM_BASEINFO : public MSGBASE
{
	ITEMBASE		ItemBase ;
	POSTYPE			ItemPos;
	DWORD			ItemIdx;
};


struct SEND_SHOPITEM_USEONE : public MSGBASE
{
	SHOPITEMBASE	ShopItemBase;
};

struct SEND_SHOPITEM_USEDINFO : public MSGBASE
{
	WORD ItemCount;
	//SHOPITEMBASE Item[100];
	ITEMBASE Item[100] ;


	SEND_SHOPITEM_USEDINFO::SEND_SHOPITEM_USEDINFO()
	{
		ItemCount = 0;
		//memset(Item, 0, sizeof(SHOPITEMBASE)*100);
		memset(Item, 0, sizeof(ITEMBASE)*100);
	}

	int GetSize()	{
		//return (sizeof(SEND_SHOPITEM_USEDINFO) - sizeof(SHOPITEMBASE)*(100-ItemCount));
		return (sizeof(SEND_SHOPITEM_USEDINFO) - sizeof(ITEMBASE)*(100-ItemCount));
	}
};

struct SEND_SHOPITEM_INFO : public MSGBASE
{
	WORD		ItemCount;
	ITEMBASE	Item[SLOT_SHOPITEM_NUM];

	SEND_SHOPITEM_INFO::SEND_SHOPITEM_INFO() {
		ItemCount = 0;
		memset(Item, 0, sizeof(ITEMBASE)*SLOT_SHOPITEM_NUM);
	}
	int GetSize() {
		return (sizeof(SEND_SHOPITEM_INFO) - sizeof(ITEMBASE)*(SLOT_SHOPITEM_NUM-ItemCount));
	}
};


struct SEND_SHOPITEMOPTION : public MSGBASE
{
	SHOPITEMOPTION	Info;

	SEND_SHOPITEMOPTION::SEND_SHOPITEMOPTION() {
		memset(&Info, 0, sizeof(Info));
	}
};

//


// ��������������������������������������������������������������
// �� RaMa - MoveDialog		                                   ��
// ��������������������������������������������������������������
struct stMOVEPOINT
{
	DWORD value;

	stMOVEPOINT::stMOVEPOINT()
	{
		value = 0;
	}
	void SetMovePoint(WORD wx, WORD wz)
	{
		value = 0;
		DWORD msk = 0;
		
		msk = wx;
		value = (value | (msk<<16));
		msk = wz;
		value = (value | msk);
	}
	void GetMovePoint(WORD& wx, WORD& wz)
	{
		DWORD msk = 0;

		wx = (WORD)(value>>16);
		msk = value<<16;
		wz = (WORD)(msk>>16);
	}	
};


struct MOVEDATA
{
	DWORD		DBIdx;
	char		Name[MAX_SAVEDMOVE_NAME];
	WORD		MapNum;
	stMOVEPOINT Point;
	
	MOVEDATA::MOVEDATA()
	{
		DBIdx = 0;
		memset(Name, 0, MAX_SAVEDMOVE_NAME);
		MapNum = 0;
		Point.value = 0;
	}
};


struct SEND_MOVEDATA_INFO : public MSGBASE
{
	BYTE		bInited;
	WORD		Count;
	MOVEDATA	Data[MAX_MOVEDATA_PERPAGE*MAX_MOVEPOINT_PAGE];

	SEND_MOVEDATA_INFO::SEND_MOVEDATA_INFO()
	{
		bInited = 0;
		Count = 0;
		memset(Data, 0, sizeof(MOVEDATA)*MAX_MOVEDATA_PERPAGE*MAX_MOVEPOINT_PAGE);
	}

	int GetSize()
	{
		return (sizeof(SEND_MOVEDATA_INFO) - sizeof(MOVEDATA)*(MAX_MOVEDATA_PERPAGE*MAX_MOVEPOINT_PAGE-Count));
	}
};


struct SEND_MOVEDATA_SIMPLE : public MSGBASE
{
	MOVEDATA Data;
};


struct SEND_MOVEDATA_WITHITEM : public MSGBASE
{
	MOVEDATA			Data;
	SHOPITEMUSEBASE		Item;
};

struct SEND_GAMEIN_USEITEM : public MSGBASE
{
	DWORD dwData1;
	DWORD dwData2;
	DWORD dwData3;

	DWORD dwPosition;
	DWORD dwItemIdx;
	DWORD dwItemPos;
};



//

struct REVIVEDATA
{
	DWORD TargetID;
	DWORD ItemIdx;
	POSTYPE ItemPos;

	REVIVEDATA::REVIVEDATA()
	{
		TargetID = 0;
		ItemIdx = 0;
		ItemPos = 0;
	}
};


struct SEND_REVIVEOTHER : public MSGBASE
{
	DWORD	TargetID;
	char	TargetName[MAX_NAME_LENGTH+1];

	SEND_REVIVEOTHER::SEND_REVIVEOTHER()
	{
		TargetID = 0;
		memset( TargetName, 0, sizeof(char)*(MAX_NAME_LENGTH+1) );
	}
};



struct AVATARITEM
{
	BYTE	Gender;				// 0-����, 1-����, 2-�Ѵ�
	BYTE	Position;
	DWORD	Item[eAvatar_Max];

	AVATARITEM::AVATARITEM()
	{
		Gender = 2;
		Position = 0;
		memset( Item, 0, sizeof(WORD)*eAvatar_Max );
		for(int i=eAvatar_Weared_Hair; i<eAvatar_Max; i++)
			Item[i]=1;
	}
};


struct SEND_AVATARITEM_INFO : public MSGBASE
{
	DWORD	PlayerId;
	DWORD	ItemIdx;
	WORD	ItemPos;
	DWORD	Avatar[eAvatar_Max];

	SEND_AVATARITEM_INFO::SEND_AVATARITEM_INFO()
	{
		PlayerId=0;
		ItemIdx=0;
		ItemPos=0;
		memset( Avatar, 0, sizeof(WORD)*eAvatar_Max );
		for(int i=eAvatar_Weared_Hair; i<eAvatar_Max; i++)
			Avatar[i]=1;
	}
};


struct CHARACTERCHANGE_INFO
{
	BYTE	Gender;
	BYTE	FaceType;
	BYTE	HairType;
	float	Height;
	float	Width;

	CHARACTERCHANGE_INFO::CHARACTERCHANGE_INFO()
	{
		Gender = 0;
		FaceType = 0;
		HairType = 0;
		Height = 0;
		Width = 0;
	}
};

// 080414 LUJ, �ܾ� ���� �޽���
struct MSG_CHARACTER_CHANGE : public MSGBASE
{
	CHARACTERCHANGE_INFO	mInfo;
	ITEMBASE				mItem;
};

struct SHOPITEMDUP
{
	DWORD Index;
	DWORD DupType;
	DWORD Param;

	SHOPITEMDUP::SHOPITEMDUP()
	{
		Index = 0;
		DupType = 0;
		Param = 0;
	}
};




// partywar
struct MSG_PW_CREATE : public MSGBASE
{
	DWORD	dwTeam;
	VECTOR3	vPosition;

};




// ��������������������������������������������������������������
// ��Memory Checker                                            ��
// ��������������������������������������������������������������
struct stMemoryInfo
{
	DWORD type;
	DWORD Created;
	DWORD Released;

	int MemorySize;

	stMemoryInfo::stMemoryInfo()
	{
		type=0;
		Created=0;
		Released=0;
		MemorySize=0;
	}
	void stMemoryInfo::Increase( int size )
	{
		++Created;
		MemorySize += size;
	}
	void stMemoryInfo::Decrease( int size )
	{
		++Released;
		MemorySize -= size;
	}
};

enum
{
	eSkillFactory_MakeSkillObject=0,
	eObjectFactory_Player,
	eObjectFactory_Monster,
	eObjectFactory_Npc,
	eObjectFactory_BossMonster,
	ePackedData_Init,

	eMemoryChecker_Max,
};

// ��������������������������������������������������������������
// �� RaMa												       ��
// ��������������������������������������������������������������
struct SHOUTBASE
{
	DWORD CharacterIdx;
	char Msg[MAX_SHOUT_LENGTH+1];
};

struct SHOUTRECEIVE
{
	BYTE Count;
	WORD Time;
	DWORD CharacterIdx;
};

struct SEND_SHOUTBASE : public MSGBASE
{
	BYTE		Count;
	SHOUTBASE	ShoutMsg[3];

	SEND_SHOUTBASE::SEND_SHOUTBASE()
	{
		Count = 0;
		memset( ShoutMsg, 0, sizeof(SHOUTBASE)*3 );
	}
	DWORD SEND_SHOUTBASE::GetSize()
	{
		return (sizeof(SEND_SHOUTBASE) - sizeof(SHOUTBASE)*(3-Count));
	}
};

struct SEND_SHOUTRECEIVE : public MSGBASE
{
	SHOUTRECEIVE	Receive;
};

struct SEND_SHOUTBASE_ITEMINFO : public MSGBASE
{	
	DWORD dwItemIdx;
	DWORD dwItemPos;
	char Name[MAX_NAME_LENGTH+1];
	char Msg[MAX_NAME_LENGTH+MAX_SHOUT_LENGTH+1];

	SEND_SHOUTBASE_ITEMINFO::SEND_SHOUTBASE_ITEMINFO()
	{
		dwItemIdx = 0;
		dwItemPos = 0;
		memset( Name, 0, sizeof(char)*MAX_NAME_LENGTH+1 );
		memset( Msg, 0, sizeof(char)*MAX_SHOUT_LENGTH+1 );
	}

	int	GetMsgLength() { return sizeof(SEND_SHOUTBASE_ITEMINFO) - MAX_SHOUT_LENGTH + strlen(Msg); }
};



struct SEND_CHASEBASE : public MSGBASE
{	
	char	WantedName[MAX_NAME_LENGTH+1];
	DWORD	dwItemIdx;
};

struct SEND_CHASEINFO : public MSGBASE
{
	DWORD CharacterIdx;
	char WandtedName[MAX_NAME_LENGTH+1];
	COMPRESSEDPOS Pos;
	MAPTYPE	MapNum;
	MAPTYPE	EventMapNum;
};

struct SEND_CHANGENAMEBASE : public MSGBASE
{
	char	Name[MAX_NAME_LENGTH+1];
	DWORD	DBIdx;
};



///////////////////////////////////
enum eCheatEvent
{
	eEvent_None=0,
	eEvent_ExpRate,
	eEvent_ItemRate,
	eEvent_MoneyRate,
	eEvent_DamageReciveRate,
	eEvent_DamageRate,
	eEvent_ManaRate,
	eEvent_UngiSpeed,
	eEvent_PartyExpRate,
	eEvent_AbilRate,
	eEvent_GetMoney,
	eEvent_SkillExp,

	eEvent_Max,
};


struct MSG_GM_MOVE : public MSGBASE
{
	char Name[MAX_NAME_LENGTH+1];
	DWORD dwData1;
	DWORD dwData2;
	DWORD dwGMIndex;
	char strGMID[MAX_NAME_LENGTH+1];
};

struct MSG_EVENTNOTIFY_ON : public MSGBASE
{
	char strTitle[32];
	char strContext[128];
	BYTE EventList[eEvent_Max];
};

struct GAMEEVENT 
{
	void Init(BYTE kind, DWORD data)
	{
		bEventKind = kind;
		dwData = data;
	}

	BYTE bEventKind;			// �̺�Ʈ�� ����
	DWORD dwData;				// ������1
};

enum  eGameEvent1 {
	eGameEvent1_LevelUp,
	eGameEvent1_HeroDie,
	eGameEvent1_SavePoint,
	eGameEvent1_Ungijosik,
	eGameEvent1_GameIn,
	eGameEvent1_MonsterKill,
	eGameEvent1_ObtainItem,
	eGameEvent1_Max,
};

enum  eGameEvent
{
	eGameEvent_CharInterface,
	eGameEvent_InvenInterface,
	eGameEvent_SkillInterface,
	eGameEvent_QuestInterface,
	eGameEvent_MapInterface,
	eGameEvent_MunpaInterface,
	eGameEvent_FriendInterface,
	eGameEvent_NoteInterface,
	eGameEvent_OptionInterface,
	eGameEvent_ExitInterface,
	eGameEvent_PKInterface,
	
	eGameEvent_Hunt,
	eGameEvent_Die,
	eGameEvent_DieAfter5,
	eGameEvent_NpcClick,
	eGameEvent_DieFromPK,
	eGameEvent_KillPK,
	eGameEvent_ObtainItem,
	eGameEvent_ItemSetting,
	eGameEvent_LoginPoint,
	eGameEvent_Skill0,
	eGameEvent_Skill1,
	
	eGameEvent_LevelUp,
	eGameEvent_MapChange,

	// 070618 LYW --- CommonStruct : Add interface event.
	eGameEvent_MatchingInterface,
	eGameEvent_FamilyInterface,

	eGameEvent_Max,
};


// ��������������������������������������������������������������
// �� Guild Tournament									       ��
// ��������������������������������������������������������������

struct stTournamentTime
{
	WORD	Day;
	WORD	Hour;
	WORD	Minute;
};

// from DB
struct GTDBLoadInfo
{
	DWORD	GuildIdx;
	WORD	Ranking;
	WORD	Position;
	WORD	ProcessTournament;
};

struct REGISTEDGUILDINFO
{
	DWORD			GuildIdx;
	char			ImageData[GUILDMARK_BUFSIZE];
	char			GuildName[MAX_GUILD_NAME+1];
	DWORD			MarkExist;
	BYTE			Position;
	BYTE			Ranking;
	BYTE			ProcessTournament;
};

struct GUILDRANKING
{
	DWORD			GuildIdx;
	BYTE			Ranking;
};


struct GTTEAM_MEMBERINFO
{
	DWORD		TeamGuildIdx[2];
	WORD		TeamMember[2];
	WORD		TeamDieMember[2];
};

struct SEND_GTWINLOSE : public MSGBASE
{
	WORD		TournamentCount;
	DWORD		GuildIdx;
	char		GuildName[MAX_GUILD_NAME+1];
	BYTE		bWin;
	DWORD		Param;			// ��� �̱�� ���°�, client-��� ����ΰ�
	BYTE		bUnearnedWin;	// �������ΰ�
};

struct SEND_BATTLEJOIN_INFO : public MSGBASE
{
	DWORD		AgentIdx;
	DWORD		UserLevel;
	DWORD		GuildIdx;
	DWORD		BattleIdx;
	DWORD		ReturnMapNum;
};


struct SEND_BATTLESCORE_INFO : public MSGBASE
{
	char		GuildName[2][MAX_GUILD_NAME+1];
	DWORD		Score[2];
	DWORD		FightTime;
	DWORD		EntranceTime;
	DWORD		State;
	BYTE		Team;

	SEND_BATTLESCORE_INFO::SEND_BATTLESCORE_INFO()
	{
		memset( GuildName[0], 0, sizeof(char)*(MAX_GUILD_NAME+1) );
		memset( GuildName[1], 0, sizeof(char)*(MAX_GUILD_NAME+1) );
		Score[0] = 0;
		Score[1] = 0;
		FightTime = 0;
		EntranceTime = 0;
		State = 0;
		Team = 0;
	}
};


struct SEND_REGISTEDGUILDINFO : public MSGBASE
{
	BYTE				Count;
	BYTE				CurTournament;
	DWORD				PlayerID;
	REGISTEDGUILDINFO	GInfo[MAXGUILD_INTOURNAMENT];

	SEND_REGISTEDGUILDINFO::SEND_REGISTEDGUILDINFO()
	{
		Count = 0;
		CurTournament = 0;
		PlayerID = 0;
		memset( GInfo, 0, sizeof(REGISTEDGUILDINFO)*MAXGUILD_INTOURNAMENT );
	}
	DWORD SEND_REGISTEDGUILDINFO::GetSize()
	{
		return (sizeof(SEND_REGISTEDGUILDINFO) - sizeof(REGISTEDGUILDINFO)*(MAXGUILD_INTOURNAMENT-Count));
	}
};

struct SEND_GUILDRANKING : public MSGBASE
{
	BYTE			Count;
	GUILDRANKING	Ranking[MAXGUILD_INTOURNAMENT];

	SEND_GUILDRANKING::SEND_GUILDRANKING()
	{
		Count = 0;
		memset( Ranking, 0, sizeof(GUILDRANKING)*MAXGUILD_INTOURNAMENT );
	}
	DWORD SEND_GUILDRANKING::GetSize()
	{
		return (sizeof(SEND_GUILDRANKING) - sizeof(GUILDRANKING)*(MAXGUILD_INTOURNAMENT-Count));
	}
};


struct GTBATTLE_INFO
{
	BYTE	Group;		// A, B, C, D
	DWORD	BattleID;
	char	GuildName1[MAX_GUILD_NAME+1];
	char	GuildName2[MAX_GUILD_NAME+1];
};


struct SEND_GTBATTLE_INFO : public MSGBASE
{
	BYTE			Count;
	BYTE			PlayOff;		// ����-1, ����-2
	GTBATTLE_INFO	BattleInfo[MAXGUILD_INTOURNAMENT/2];
	
	SEND_GTBATTLE_INFO::SEND_GTBATTLE_INFO()
	{
		Count = 0;
		memset( BattleInfo, 0, sizeof(GTBATTLE_INFO)*(MAXGUILD_INTOURNAMENT/2) );
	}
	DWORD SEND_GTBATTLE_INFO::GetSize()
	{
		return (sizeof(SEND_GTBATTLE_INFO) - sizeof(GTBATTLE_INFO)*(MAXGUILD_INTOURNAMENT/2-Count));
	}
};


struct GTBATTLE_INFOBROAD
{
	DWORD	BattleID;
	DWORD	Group;			// �����
	DWORD	GuildIdx[2];
};


struct SEND_GTBATTLE_INFOBROAD : public MSGBASE
{
	BYTE					Count;
	DWORD					CurTournament;
	GTBATTLE_INFOBROAD		BattleInfo[MAXGUILD_INTOURNAMENT/2];
	
	SEND_GTBATTLE_INFOBROAD::SEND_GTBATTLE_INFOBROAD()
	{
		Count = 0;
		memset( BattleInfo, 0, sizeof(GTBATTLE_INFOBROAD)*MAXGUILD_INTOURNAMENT/2 );
	}
	DWORD SEND_GTBATTLE_INFOBROAD::GetSize()
	{
		return (sizeof(SEND_GTBATTLE_INFOBROAD) - sizeof(GTBATTLE_INFOBROAD)*(MAXGUILD_INTOURNAMENT/2-Count));
	}
};


#define		MAX_GUILD_UNION_NUM		7

struct sGUILDIDXNAME
{
	DWORD	dwGuildIdx;
	char	sGuildName[MAX_GUILD_NAME+1];
};

struct SEND_GUILD_UNION_INFO : public MSGBASE
{
	DWORD			dwGuildUnionIdx;
	char			sGuildUnionName[MAX_GUILD_NAME+1];
	DWORD			dwGuildUnionMarkIdx;
	int				nMaxGuildNum;
	sGUILDIDXNAME	GuildInfo[MAX_GUILD_UNION_NUM];
	
	SEND_GUILD_UNION_INFO()
	{
		dwGuildUnionIdx = 0;
		memset( sGuildUnionName, 0, MAX_GUILD_NAME+1 );
		dwGuildUnionMarkIdx = 0;
		memset( GuildInfo, 0, sizeof(sGUILDIDXNAME)*MAX_GUILD_UNION_NUM );
		nMaxGuildNum = 0;
	}
	void AddGuildInfo( DWORD dwGuildIdx, char* pGuildName )
	{
		GuildInfo[nMaxGuildNum].dwGuildIdx = dwGuildIdx;
		strncpy( GuildInfo[nMaxGuildNum].sGuildName, pGuildName, MAX_GUILD_NAME+1 );
		++nMaxGuildNum;
	}
	int GetSize()
	{
		return ( sizeof(SEND_GUILD_UNION_INFO) - sizeof(sGUILDIDXNAME)*(MAX_GUILD_UNION_NUM-nMaxGuildNum) );
	}
};


//#define		MAX_FAMILY_UNION_NUM		7

//struct sFAMILYIDXNAME
//{
//	DWORD	dwFamilyIdx;
//	char	sFamilyName[MAX_FAMILY_NAME+1];
//};
//
//struct SEND_FAMILY_UNION_INFO : public MSGBASE
//{
//	DWORD			dwFamilyUnionIdx;
//	char			sFamilyUnionName[MAX_FAMILY_NAME+1];
//	DWORD			dwFamilyUnionMarkIdx;
//	int				nMaxFamilyNum;
//	sFAMILYIDXNAME	FamilyInfo[MAX_FAMILY_UNION_NUM];
//	
//	SEND_FAMILY_UNION_INFO()
//	{
//		dwFamilyUnionIdx = 0;
//		memset( sFamilyUnionName, 0, MAX_FAMILY_NAME+1 );
//		dwFamilyUnionMarkIdx = 0;
//		memset( FamilyInfo, 0, sizeof(sFAMILYIDXNAME)*MAX_FAMILY_UNION_NUM );
//		nMaxFamilyNum = 0;
//	}
//	void AddFamilyInfo( DWORD dwFamilyIdx, char* pFamilyName )
//	{
//		FamilyInfo[nMaxFamilyNum].dwFamilyIdx = dwFamilyIdx;
//		strncpy( FamilyInfo[nMaxFamilyNum].sFamilyName, pFamilyName, MAX_FAMILY_NAME+1 );
//		++nMaxFamilyNum;
//	}
//	int GetSize()
//	{
//		return ( sizeof(SEND_FAMILY_UNION_INFO) - sizeof(sFAMILYIDXNAME)*(MAX_FAMILY_UNION_NUM-nMaxFamilyNum) );
//	}
//};

struct SEND_CHEAT_ITEM_OPTION : public MSGBASE
{
	SEND_CHEAT_ITEM_OPTION(){memset(this, 0, sizeof(SEND_CHEAT_ITEM_OPTION));}
	POSTYPE bPosition;
	DWORD wItemIdx;
	WORD wOptionKind;

	ITEM_OPTION		OptionInfo;
};


// ��������������������������������������������������������������
// �� SiegeWar											       ��
// ��������������������������������������������������������������

#define SIEGEWAR_MAXGUILDCOUNT_PERTEAM		128
#define SIEGEWAR_MAX_SIEGEMAP				20
#define SIEGEWAR_MAX_AFFECTED_MAP			10
#define CASTLEGATE_NAME_LENGTH				16
enum
{
	eCastleGateLevel_Level0 = 0,
	eCastleGateLevel_Level1,
	eCastleGateLevel_Level2,
	eCastleGateLevel_Level3,
	eCastleGateLevel_Max,
};



struct MAPOBJECT_INFO
{
	DWORD	Life;
	DWORD	MaxLife;
	DWORD	PhyDefence;
	float	Radius;


	ATTRIBUTEREGIST		AttrRegist;
};



struct CASTLEGATE_BASEINFO
{
	DWORD		Index;
	DWORD		MapNum;
	char		Name[CASTLEGATE_NAME_LENGTH+1];
	DWORD		Kind;
	char		DataName[33];
	DWORD		Life[eCastleGateLevel_Max];
	DWORD		Defence;
	ATTRIBUTEREGIST	Regist;
	float		Radius;		
	VECTOR3		Position;
	DWORD		Width;
	DWORD		Wide;
	float		Angle;
	float		Scale;
};



struct stCASTLEGATELEVEL{
	/// 4bit : ����, 5bit : �ε���, 2bit : ����
	DWORD		value;

	stCASTLEGATELEVEL():value(0) {}

	void stCASTLEGATELEVEL::AddGate( DWORD Index, DWORD Level )
	{
		DWORD tmp = value;
		if( Index > 1000 )			Index -= 1000;

		DWORD Count = tmp >> 28;
		DWORD msk = Index;

		// �ε����� ������ �����Ų msk�� �����
		msk = msk << 2;
		msk |= Level;
		msk = msk << (7*(4-(Count+1)));

		// ������ ����Ÿ�� �����Ų��.
		tmp = tmp << 4;
		tmp = tmp >> 4;
		tmp |= msk;

		// count�ϳ� �ø���
		++Count;
		msk = Count;
		msk = msk << 28;		
		tmp |= msk;

		// �ٽ� ����
		value = tmp;
	}
	BOOL stCASTLEGATELEVEL::SetLevel( DWORD Index, DWORD Level )
	{
		DWORD tmp = value;
		if( Index > 1000 )			Index -= 1000;

		DWORD Count = tmp >> 28;
		for(DWORD i=0; i<Count; ++i)
		{
			DWORD t = tmp << (4+(7*i));
			t = t >> 25;
			DWORD data = t;
			
			DWORD idx = data >> 2;			
			if( idx == Index )
			{
				DWORD msk = Level;
				idx = idx << 2;
				msk |= idx;
				msk = msk << (7*((4-i)-1));

				DWORD msk1 = tmp >> 7*(4-i);
				msk1 = msk1 << 7*(4-i);
				DWORD msk2 = tmp << (4+(7*(i+1)));
				msk2 = msk2 >> (4+(7*(i+1)));

				msk1 |= msk2;
				msk |= msk1;

				value = msk;
				return TRUE;
			}
		}
		return FALSE;
	}
	DWORD stCASTLEGATELEVEL::GetLevel( DWORD Index )
	{
		DWORD tmp = value;
		if( Index > 1000 )			Index -= 1000;

		DWORD Count = tmp >> 28;
		for(DWORD i=0; i<Count; ++i)
		{
			DWORD t = tmp << (4+(7*i));
			t = t >> 25;
			DWORD data = t;
			
			DWORD idx = data >> 2;			
			if( idx == Index )
			{
				DWORD level = t << 30;
				level = level >> 30;
				return level;
			}
		}

		return 0;
	}
	void stCASTLEGATELEVEL::SetZeroAllLevel()
	{
		DWORD tmp = value;
		DWORD Count = tmp >> 28;

		for(DWORD i=0; i<Count; ++i)
		{
			DWORD t = tmp << (4+(7*i));
			t = t >> 25;
			DWORD data = t;
			
			DWORD idx = data >> 2;

			DWORD msk = 0;
			idx = idx << 2;
			msk |= idx;
			msk = msk << 7*((4-i)-1);

			DWORD msk1 = tmp >> 7*(4-i);
			msk1 = msk1 << 7*(4-i);
			DWORD msk2 = tmp << (4+(7*(i+1)));
			msk2 = msk2 >> (4+(7*(i+1)));

			msk1 |= msk2;
			msk |= msk1;

			value = msk;		
		}
	}
};


struct CASTLEGATE_SENDINFO
{
	DWORD		Index;
	DWORD		Life;
	BYTE		Level;
};

struct SEND_SIEGEWAR_JOININFO : public MSGBASE
{
	DWORD		AgentIdx;
	DWORD		UserLevel;
	DWORD		GuildIdx;
	BYTE		bObserver;
	DWORD		ReturnMapNum;
};

enum SIEGEGUILDTYPE
{
	eSGT_DEFENCEPROPOSALGUILD = 1,
	eSGT_DEFENCEGUILD,
	eSGT_ATTACKGUILD,
};

struct SIEGEWAR_GUILDDBINFO
{
	DWORD	GuildIdx;
	BYTE	Type;				// 1 - CastleGuild, 2 - DefenceGuild, 3 - DefenceProposal, 4 - AttackGuild

	SIEGEWAR_GUILDDBINFO()
	{
		GuildIdx = 0;
		Type = 0;
	}
};

struct SEIGEWAR_CHARADDINFO
{
	BYTE Team;
	BYTE bEngrave;
};

struct SIEGEWAR_GUILDSENDINFO
{
	SIEGEWAR_GUILDDBINFO	Info;
	char					GuildName[MAX_GUILD_NAME+1];

	SIEGEWAR_GUILDSENDINFO()
	{
		memset( GuildName, 0, sizeof(char)*(MAX_GUILD_NAME+1) );
	}
};

struct SEND_SW_PROPOSALGUILDLIST : public MSGBASE
{
	WORD						Count;
	SIEGEWAR_GUILDSENDINFO		GuildList[SIEGEWAR_MAXGUILDCOUNT_PERTEAM];

	SEND_SW_PROPOSALGUILDLIST()
	{
		Count = 0;
		memset( GuildList, 0, sizeof(SIEGEWAR_GUILDSENDINFO)*SIEGEWAR_MAXGUILDCOUNT_PERTEAM );
	}
	int GetSize()
	{
		return ( sizeof(SEND_SW_PROPOSALGUILDLIST) - 
			sizeof(SIEGEWAR_GUILDSENDINFO)*(SIEGEWAR_MAXGUILDCOUNT_PERTEAM-Count) );
	}
};

struct SEND_SW_GUILDLIST : public MSGBASE
{
	WORD						DefenceCount;
	WORD						AttackCount;
	SIEGEWAR_GUILDSENDINFO		GuildList[SIEGEWAR_MAXGUILDCOUNT_PERTEAM*2];

	SEND_SW_GUILDLIST()
	{
		DefenceCount = 0;
		AttackCount = 0;
		memset( GuildList, 0, sizeof(SIEGEWAR_GUILDSENDINFO)*SIEGEWAR_MAXGUILDCOUNT_PERTEAM*2 );
	}
	int GetSize()
	{
		return ( sizeof(SEND_SW_GUILDLIST) - 
			sizeof(SIEGEWAR_GUILDSENDINFO)*(SIEGEWAR_MAXGUILDCOUNT_PERTEAM*2-(DefenceCount+AttackCount)) );
	}
};

struct SEND_AFFECTED_MAPLIST : public MSGBASE
{
	DWORD		Param;
	WORD		Count;
	WORD		MapList[SIEGEWAR_MAX_AFFECTED_MAP];

	SEND_AFFECTED_MAPLIST()
	{
		Param = 0;
		Count = 0;
		memset( MapList, 0, sizeof(WORD)*10 );
	}
	int GetSize()
	{
		return ( sizeof(SEND_AFFECTED_MAPLIST) - sizeof(WORD)*(SIEGEWAR_MAX_AFFECTED_MAP-Count) );
	}
};

struct SEND_SW_BTGUILDLIST : public MSGBASE
{
	WORD		Team;
	WORD		DefenceCount;
	WORD		AttackCount;
	DWORD		GuildList[SIEGEWAR_MAXGUILDCOUNT_PERTEAM*2];

	SEND_SW_BTGUILDLIST()
	{
		Team = 0;
		DefenceCount = 0;
		AttackCount = 0;
		memset( GuildList, 0, sizeof(DWORD)*SIEGEWAR_MAXGUILDCOUNT_PERTEAM*2 );
	}
	int GetSize()
	{
		return ( sizeof(SEND_SW_BTGUILDLIST) - 
			sizeof(DWORD)*(SIEGEWAR_MAXGUILDCOUNT_PERTEAM*2-(DefenceCount+AttackCount)) );
	}
};

struct SEND_SW_SUCCESSINFO : public MSGBASE
{
	char GuildName[MAX_GUILD_NAME+1];
};

struct SEND_SW_ENGRAVE : public MSGBASE
{
	char CharacterName[MAX_NAME_LENGTH+1];
	char GuildName[MAX_GUILD_NAME+1];
};

struct SEND_SW_INITINFO : public MSGBASE
{
	DWORD	Time;
	char	GuildName[MAX_GUILD_NAME+1];
	BYTE	GateCount;
	CASTLEGATE_SENDINFO	GateInfo[5];
	
	SEND_SW_INITINFO()
	{
		GateCount = 0;
		memset( GateInfo, 0, sizeof(CASTLEGATE_SENDINFO)*5 );
	}
	int GetSize()
	{
		return ( sizeof(SEND_SW_INITINFO) - sizeof(CASTLEGATE_SENDINFO)*(5-GateCount) );
	}
};



//----------------------------------
struct MSG_ASSERT : public MSGBASE
{
	DWORD UserID;
	DWORD CharacterID;
	char strAssertMsg[256];
	void SetMsg(DWORD UserID,DWORD CharacterID,char* Msg)
	{
		Category = MP_DEBUG;
		Protocol = MP_DEBUG_CLIENTASSERT;
		this->UserID = UserID;
		this->CharacterID = CharacterID;
		strncpy(strAssertMsg,Msg,255);
		strAssertMsg[255] = 0;
	}
};

struct MSG_JACKPOT_PRIZE_NOTIFY : public MSGBASE
{
	MSG_JACKPOT_PRIZE_NOTIFY():nPrizeKind(-1),dwPrizeUserID(0),dwPrizeMoney(0),dwRestTotalMoney(0){};
	int nPrizeKind;				// prize kind
	DWORD dwPrizeUserID;
	MONEYTYPE dwPrizeMoney;
	MONEYTYPE dwRestTotalMoney;		// DB TotalMoney
	char Name[MAX_NAME_LENGTH+1];	// prize character name
};

//struct MSG_JACKPOT_TOTALMONEY : public MSGBASE
//{
//	MSG_JACKPOT_TOTALMONEY():dwServerTotalMoney(0){};
//	DWORD dwServerTotalMoney;
//};

struct MSG_SW_PROFIT_INFO : public MSGBASE
{
	DWORD	dwGuildIdx;
	char	sGuildName[MAX_GUILD_NAME+1];
	int		nTaxRateForStore;				// 100����
	DWORD	dwTotalMoney;	

	MSG_SW_PROFIT_INFO()
	{
		dwGuildIdx = 0;
		memset( sGuildName, 0, MAX_GUILD_NAME+1 );
		nTaxRateForStore = 0;
		dwTotalMoney = 0;		
	}
};



// RareItem
//enum { eRI_LIFE, eRI_MANA, ValueKindMAX = 2 };
//enum { eRI_Str, eRI_Dex, eRI_Vit, eRI_Wis, StatesKindMAX = 4 };
//enum eRareItemKind{eRI_GUM,eRI_GWUN,eRI_DOW,eRI_CHANG,eRI_GOONG,eRI_AMGI,eRI_DRESS,eRI_HAT,eRI_SHOES,eRI_RING,eRI_CAPE,eRI_NECKLACE,eRI_ARMLET,eRI_BELT,RareItemKindMAX};
//
//struct sRareOptionBase
//{
//	sRareOptionBase():BaseMin(0),BaseMax(0){}
//	DWORD BaseMin;	//(����)�⺻ �ɼ� �ּҰ� (����:+���������� ��:+������� �Ǽ�����:+����)
//	DWORD BaseMax;
//};

//struct sRareOptionWeaponInfo : public sRareOptionBase
//{
//	sRareOptionWeaponInfo():AddMin(0),AddMax(0){}
//	DWORD AddMin;	//(����)�ΰ� �ɼ� �ּҰ� (����:+�����,etc. ��:+�Ӽ����׷� �Ǽ�����:����)
//	DWORD AddMax;
//};
//
//struct sRareOptionProtectorInfo : public sRareOptionBase
//{
//	sRareOptionProtectorInfo():AttribMin(0),AttribMax(0){}
//	DWORD AttribMin;
//	DWORD AttribMax;
//	DWORD PlusValue[ValueKindMAX];
//};
//
//struct sRareItemInfo
//{
//	sRareItemInfo() : ItemIdx(0),RareProb(0){}
//	DWORD ItemIdx;
//	DWORD RareProb;
//};

//struct sRareOptionInfo : public ITEM_RARE_OPTION_INFO
//{
//	sRareOptionInfo() { memset(this, 0, sizeof(this)); }
//
//	WORD	str_min;
//	WORD	dex_min;
//	WORD	vit_min;
//	WORD	wis_min;
//	DWORD	Life_Min;
//	WORD	mana_min;
//	WORD	PhyAttack_Min;
//	//	WORD	CriticalPercent_Min;
//	WORD	PhyDefense_Min;
//	ATTRIBUTEREGIST AttrRegist_Min;
//	ATTRIBUTEATTACK AttrAttack_Min;
//};
struct MSG_SINGLE_SPECIAL_STATE_NACK : public MSGBASE
{
	WORD	State;
	WORD	ErrCode;
};
struct MSG_SINGLE_SPECIAL_STATE : public MSGBASE
{
	DWORD	Operator;
	WORD	State;
	bool	bVal;
	bool	Forced;

	MSG_SINGLE_SPECIAL_STATE()
	{
		Operator = 0;
		State = 0;
		bVal = 0;
		Forced = 0;
	}
};

struct StatusData 
{
	DWORD Index;
	WORD  ValueType;
	DWORD Value;
};

struct AbnormalStatus
{
	bool	IsParalysis;
	bool	IsStun;
	bool	IsSlip;
	bool	IsFreezing;
	bool	IsStone;
	bool	IsSilence;
	bool	IsGod;
	bool	IsBlockAttack;
	bool	IsPoison;
	bool	IsShield;
	bool	IsBleeding;
	bool	IsBurning;
	bool	IsHolyDamage;
	bool	IsMoveStop;
	bool	IsAttract;

	float	Poison;
	float	Shield;
	float	Bleeding;
	float	Burning;
	float	HolyDamage;
};

struct Status
{
	float	Str;
	float	Dex;
	float	Vit;
	float	Int;
	float	Wis;

	float	PhysicAttack;
	float	PhysicDefense;
	float	MagicAttack;
	float	MagicDefense;
	
	float	Accuracy;
	float	Avoid;
	float	CriticalRate;
	float	Range;
	float	MagicCriticalRate;

	float	SkillRange;
	float	CriticalDamage;
	float	MoveSpeed;
	float	Block;
	float	CoolTime;
	float	CastingProtect;

	float	MaxLife;
	float	MaxMana;
	float	LifeRecoverRate;
	float	ManaRecoverRate;
	float	LifeRecover;
	float	ManaRecover;

	float	Reflect;
	float	Absorb;
	float	DamageToLife;
	float	DamageToMana;
	float	GetLife;
	float	GetMana;
	float	GetExp;
	float	GetGold;
	
	float	Attrib_None;
	float	Attrib_Earth;
	float	Attrib_Water;
	float	Attrib_Divine;
	float	Attrib_Wind;
	float	Attrib_Fire;
	float	Attrib_Dark;
	
	float	Sword;
	float	Mace;
	float	Axe;
	float	Staff;
	float	Bow;
	float	Gun;
	float	Dagger;
	float	Spear;
	float	OneHanded;
	float	TwoHanded;
	float	TwoBlade;
	float	Robe;
	float	LightArmor;
	float	HeavyArmor;
	float	Shield;

	float	NormalSpeedRate;
	float	PhysicSkillSpeedRate;
	float	MagicSkillSpeedRate;

	// 071203 KTH -- ��ų ���� ���� ���� %
	float	DecreaseManaRate;
	// 071204 KTH -- ����� ��� %
	float	IncreaseDropRate;
	BOOL	IsProtectExp;
};

struct	DATE_MATCHING_SEARCH_OUTPUT
{
	char		szName[MAX_NAME_LENGTH+1];									// �˻��� ����.
	DWORD		dwGoodFeelingScore;						// ȣ������
	DWORD		dwScore;								// �˻� ��� ����.
	DWORD		dwAge;									// ����.
	DWORD		dwUserID;								// ���� ID
};

#define	DATE_MATCHING_LIST_MAX	20

struct	MSG_DATEMATCHING_SEARCH_RESULT: public MSGBASE				// s2c ����Ʈ ��� ����Ʈ�� ������.
{
	DWORD	dwResultCount;
	DATE_MATCHING_SEARCH_OUTPUT		pList[DATE_MATCHING_LIST_MAX];
	int	GetMsgLength(){ return	sizeof(MSG_DATEMATCHING_SEARCH_RESULT) - (sizeof(DATE_MATCHING_SEARCH_OUTPUT)*DATE_MATCHING_LIST_MAX) + sizeof(DATE_MATCHING_SEARCH_OUTPUT)*dwResultCount;}
};

struct	MSG_DATEMATCHING_REQUEST_TARGET_INFO: public MSGBASE	// c2s
{
//	char	szTargetName[MAX_CHAT_LENGTH+1];
	DWORD	dwTargetUserID;							// ã������ UserID;
};

struct	MSG_DATEMATCHING_TARGET_INFO: public MSGBASE			// s2c
{
	DATE_MATCHING_INFO	TargetInfo;
};

struct	MSG_DATEMATCHING_MODIFY_INTRODUCE:	public MSGBASE		// c2s
{
	// �� �Ұ� ������ �������� ����.
	char	szIntroduce[MAX_INTRODUCE_LEN+1];
};

// desc_hseos_���͹���01
// S ���͹��� �߰� added by hseos 2007.05.23
typedef struct
{
	DWORD			nPlayTimeTick;
	DWORD			nPlayTime;
	DWORD			nKillMonsterNum;
	DWORD			nPlayTimeTotal;
	DWORD			nKillMonsterNumTotal;
} stMONSTERMETER;
// E ���͹��� �߰� added by hseos 2007.05.23

// desc_hseos_�󱼱�ü01
// S �󱼱�ü �߰� added by hseos 2007.06.07	2007.06.08
enum
{
	FACE_ACTION_KIND_EYE_BLINK = 0,
	FACE_ACTION_KIND_EYE_CHATTING_CMD, 
};

enum
{
	PLAYER_FACE_SHAPE_EYE_OPEN = 0,
	PLAYER_FACE_SHAPE_EYE_CLOSE,
};

typedef struct
{
	DWORD	nFaceShapeTimeTick;					// �� ��� �ð� ƽ
	int		nFaceShapeState;					// �� ��� ���� ( �ִϸ��̼ǽ� �ʿ�. - ��������)
	int		nFaceActionKind;					// �� ���� ���� ( 0: �⺻ ��������, 1: ä��â ��ɾ� �� ����)
} stFACEACTION;
// E �󱼱�ü �߰� added by hseos 2007.06.07	2007.06.08

// desc_hseos_�йи�01
// S �йи� �߰� added by hseos 2007.07.09
// ..�������� ����� ����� ����
enum RESULT
{
	RESULT_OK = 0,								// ����
	RESULT_OK_02,
	RESULT_OK_03,
	RESULT_OK_04,
	RESULT_OK_05,
	RESULT_OK_06,

	RESULT_FAIL_INVALID,						// ����(�ý��� ������ ����� ����)

	RESULT_FAIL_01,								// ����01
	RESULT_FAIL_02,								// ����02
	RESULT_FAIL_03,								// ����03
	RESULT_FAIL_04,								// ����04
	RESULT_FAIL_05,								// ����05
	RESULT_FAIL_06,								// ����06
	RESULT_FAIL_07,								// ����07
	RESULT_FAIL_08,								// ����08
	RESULT_FAIL_09,								// ����09
	RESULT_FAIL_10,								// ����10
	RESULT_FAIL_11,								// ����11
	RESULT_FAIL_12,								// ����12
	RESULT_FAIL_13,								// ����13
	RESULT_FAIL_14,								// ����14
};
// E �йи� �߰� added by hseos 2007.07.09


//---KES Distribute Encrypt 071003
struct MSG_DIST_CONNECTSUCCESS : public MSG_DWORD
{
	HselInit eninit;
	HselInit deinit;
};

// 071218 KTH -- ����ġ ����ϱ� ���� ����ü
/*struct MEMORY_MAP_POINT
{
	char	MemoryName[128];	// ����� �̸�
	MAPTYPE MapNum;				// �� ��ȣ
	VECTOR3	Vector;				// �� ��ǥ

	MEMORY_MAP_POINT() {
		memset(MemoryName, NULL, sizeof(MemoryName));
		MapNum = 0;
		memset(&Vector, NULL, sizeof(VECTOR3));
	}
};*/

// 071221 KTH
/*
struct MSG_USE_MEMORY_STONE_SYN : public MSG_ITEM_USE_SYN
{
	st_MemoryStoneInfo	MemoryStoneInfo;
};
*/
// 071226 KTH -- �̸� ���� ������ ���
struct MSG_CHANGE_CHARACTER_NAME_SYN : public MSG_ITEM_USE_SYN
{
	char	changeName[17];
};

struct MSG_CHANGE_CHARACTER_NAME_ACK : public MSG_CHANGE_CHARACTER_NAME_SYN
{
};

struct MSG_CHANGE_CHARACTER_NAME_AGENT : public MSGBASE
{
	char changeName[17];
	int CharacterIdx;
	int UserIdx;
};

struct MSG_AUTOLIST_ADD : public MSGBASE
{
	char Name[MAX_NAME_LENGTH+1];
	char Date[20];		//2008.01.25 00:00:00  (19����)
};

struct MSG_AUTOLIST_ALL : public MSGBASE
{
	struct AUTOLIST_ROW
	{
		char Name[MAX_NAME_LENGTH+1];
		char Date[20];
	};

	int nCount;
	AUTOLIST_ROW	row[100];

	int	GetMsgLength()
	{
		return sizeof(MSG_AUTOLIST_ALL) - ( sizeof(AUTOLIST_ROW) * ( 100-nCount ) );
	}
};

struct MSG_AUTONOTE_IMAGE : public MSGBASE
{
	BYTE Image[128*32*3];
};





// 080404 LYW --- CommonStruct : ä�ù� �ý��ۿ� �ʿ��� ������ ����ü��.

#define MAX_ROOM_COUNT				250								// �ִ� ���� ä�ù� ��.
#define MAX_USER_PER_ROOM			20								// �� ä�ù� �� �ִ� ������ ��.

#define TITLE_SIZE					64								// ���� ������.

#define ROOM_COUNT_PER_PAGE			19								// ������ �� ä�ù� ��.

#define CHATROOM_DELAY_TIME			1500							// ������ ������ �� �ִ� ������ �ð�.

#define SECRET_CODE_SIZE			20								// ��ȣ ������.

struct ST_CR_USER													// ���� ����.
{
	BYTE byIdx ;													// �������� ä�ù� �ε���.

	DWORD dwUserID ;												// ���� �ε���.
	DWORD dwPlayerID ;												// ĳ���� �ε���.

	//WORD  wClassIdx ;												// Ŭ���� �ε���.

	BYTE  byLevel ;													// ĳ���� ����.
	BYTE  byMapNum ;												// ���� ĳ���� ��.

	char  name[ MAX_NAME_LENGTH+1 ] ;								// ĳ���� �̸�.

	DWORD dwConnectionIdx ;
} ;

struct MSG_CR_USER : public MSGBASE									// ���� ���� ���ۿ� �޽���.
{
	ST_CR_USER user ;												// ���� ����.
} ;

struct ST_CR_ROOM_SRV												// ���� ������ ä�ù� ����.
{
	BYTE byIdx ;													// ä�ù� �ε���.

	char code[SECRET_CODE_SIZE+1] ;									// ä�ù� ��ȣ.

	DWORD dwOwnerIdx ;												// ���� �ε���.

	BYTE bySecretMode ;												// ����/����� ���.
	BYTE byRoomType ;												// ä�ù� �з�.

	BYTE byCurGuestCount ;											// ���� ���� ���� ��.
	BYTE byTotalGuestCount ;										// �ִ� ���� ���� ��.

	char title[ TITLE_SIZE+1 ] ;									// ä�ù� ����.

	DWORD dwUser[MAX_USER_PER_ROOM] ;								// ������ ���̵�.
} ;

struct ST_CR_ROOM_CLT												// Ŭ���̾�Ʈ ���� ä�ù� ����.
{
	BYTE byIdx ;													// ä�ù� �ε���.

	BYTE bySecretMode ;												// ����/����� ���.
	BYTE byRoomType ;												// ä�ù� �з�.

	BYTE byCurGuestCount ;											// ���� ���� ���� ��.
	BYTE byTotalGuestCount ;										// �ִ� ���� ���� ��.

	char title[ TITLE_SIZE+1 ] ;									// ä�ù� ����.
} ;

struct MSG_CR_ROOMINFO : public MSGBASE								// �ϳ��� ä�ù� ������ ��� ����ü.
{
	BYTE byCount ;													// ���� ī��Ʈ.

	ST_CR_ROOM_SRV room ;											// ������.
	ST_CR_USER user[MAX_USER_PER_ROOM] ;							// ������ ����.

	int GetMsgSize()
	{
		return sizeof(MSG_CR_ROOMINFO) - (MAX_USER_PER_ROOM-byCount)*sizeof(ST_CR_USER) ;
	}
} ;

struct MSG_CR_ROOMLIST : public MSGBASE								// ä�ù��� ���� ����Ʈ ����.
{
	BYTE byCurPage ;												// ���� ������.
	BYTE byTotalPage ;												// �� ������.

	BYTE byRoomType ;												// ä�ù� Ÿ��.

	BYTE byCount ;													// ä�ù� ����.

	ST_CR_ROOM_CLT room[ROOM_COUNT_PER_PAGE] ;						// Ŭ���̾�Ʈ ���� ä�ù�.

	int GetMsgSize()												// ������ ��ȯ �Լ�.
	{
		return sizeof(MSG_CR_ROOMLIST) - (ROOM_COUNT_PER_PAGE-byCount)*sizeof(ST_CR_ROOM_CLT) ;
	}
} ;

struct MSG_CR_ELEMENT : public MSGBASE								// �� ���� ��� ����.
{
	BYTE byIdx ;													// �� �ε���.

	char code[SECRET_CODE_SIZE+1] ;									// ä�ù� ��ȣ.

	BYTE bySecretMode ;												// ����/�����.
	BYTE byRoomType ;												// ä�ù� �з�.

	BYTE byTotalGuestCount ;										// �ִ� ���� �ο�.

	char title[ TITLE_SIZE+1 ] ;									// ä�ù� ����.
} ;

struct MSG_CR_CHANGE_OWNER : public MSGBASE							// ���� ���� ó���� �ϴ� �޽���.
{
	DWORD dwOwnerID ;												// ���� ���̵�.
	char  name[ MAX_NAME_LENGTH+1 ] ;								// ���� �̸�.
} ;	

struct MSG_CR_MSG : public MSGBASE									// �� ������� �޽����� ������ �� ����ϴ� ����ü.
{
	char  name[ MAX_NAME_LENGTH+1 ] ;								// �̸�.
	char Msg[MAX_CHAT_LENGTH+1] ;									// �޽���.
} ;

struct MSG_CR_MSG_BROADCAST : public MSGBASE						// ä�ù濡 ������ ����鿡�� �޽����� ������ �� ����ϴ� ����ü.
{
	char  name[ MAX_NAME_LENGTH+1 ] ;								// ������� �̸�.

	BYTE byCount ;													// ������ ��.
	DWORD dwUser[MAX_USER_PER_ROOM] ;								// ������ ���̵�.

	char Msg[MAX_CHAT_LENGTH+1] ;									// �޽���.
} ;

struct MSG_CR_ROOM_USER : public MSGBASE							// ������ ������ �ʿ��� �� ����ϴ� �޽���.
{
	BYTE byCount ;													// ������ ī��Ʈ.
	DWORD dwUser[MAX_USER_PER_ROOM] ;								// ������ ���̵�.
} ;

struct MSG_CR_IDNAME : public MSG_CR_ROOM_USER						// �⺻ ������ ������, Ư���� ���̵�, �̸��� �߰� �� �޽���.
{
	DWORD dwID ;													// Ư���� ���̵�.
	char  name[ MAX_NAME_LENGTH+1 ] ;								// �̸�.
} ;

struct MSG_CR_MSG_RESULT : public MSGBASE							// �⺻ ������ ������, ä�ù� ��, ������ �޽���.
{
	char  name[ MAX_NAME_LENGTH+1 ] ;								// �̸�.
	char Msg[MAX_CHAT_LENGTH+1] ;									// �޽���.
} ;

struct MSG_CR_ROOM_NOTICE : public MSG_CR_ROOM_USER					// �⺻ ������ ������, ä�ù� �� ������ ���� �޽���.
{
	ST_CR_ROOM_SRV room ;											// ä�ù� ����.
} ;

struct MSG_CR_JOIN_NOTICE	: public MSG_CR_ROOM_USER				// �⺻ ������ ������, ä�ù� ������ ���� �޽���.
{
	ST_CR_USER user ;												// ������ ����� ����.
} ;

struct MSG_CR_KICK_ACK : public MSG_CR_ROOMLIST
{
	DWORD dwKickPlayer ;
} ;

struct MSG_CR_SEARCH_NAME : public MSGBASE							// �̸� �˻��� ����ü.
{
	BYTE byRoomIdx ;												// ���ε���.
	char name[MAX_NAME_LENGTH+1] ;									// �̸�.
} ;

struct MSG_CR_SEARCH_TITLE : public MSGBASE							// ���� �˻��� ����ü.
{
	BYTE byRoomIdx ;												// ���ε���.
	char title[ TITLE_SIZE+1 ] ;									// ����.
} ;

struct MSG_CR_JOIN_SYN : public MSGBASE								// ä�ù濡 ������ �� ����� �޽���.
{
	BYTE byRoomIdx ;
	char code[SECRET_CODE_SIZE+1] ;									// ä�ù� ��ȣ.
} ;

struct MSG_CR_ROOMINFO_TO_NOTICE : public MSGBASE					// ������ Ŭ���̾�Ʈ�� ������ ������ �� ���� ����ü.
{
	ST_CR_ROOM_SRV room ;
} ;

struct MSG_FISHING_READY_SYN : public MSGBASE
{
	DWORD dwPlaceIdx;
	DWORD dwPoleItemIdx;
	DWORD dwBaitItemIdx;
	WORD wBaitItemPos;
};

struct MSG_FISHING_GETFISH_SYN : public MSGBASE
{
	DWORD dwPlaceIdx;
	DWORD dwPoleItemIdx;
	WORD  wPerpectCnt;
	WORD  wGreatCnt;
	WORD  wGoodCnt;
	WORD  wMissCnt;
};

struct MSG_FISHING_GETFISH_ACK : public MSGBASE
{
	WORD wResCode;
	DWORD dwItemIdx;
};

struct MSG_FISHING_MISSION_CODE : public MSGBASE
{
	DWORD dwItem1;
	DWORD dwItem2;
	DWORD dwItem3;
	DWORD dwProcessTime;
};

struct FISHITEMINFO
{
	DWORD dwItemIdx;
	DURTYPE dur;
	POSTYPE pos;
};

struct MSG_FISHING_FPCHANGE_SYN : public MSGBASE
{
	DWORD dwAddFishPoint;
	FISHITEMINFO FishList[12];
};

struct MSG_PET_ADD : public MSGBASE
{
	DWORD	MasterIdx;
	BASEOBJECT_INFO BaseObjectInfo;
	SEND_MOVEINFO MoveInfo;
	PET_OBJECT_INFO PetObjectInfo;
	ITEMBASE ItemInfo[ ePetEquipSlot_Max ];
	BOOL	IsLogin;
	CAddableInfoList AddableInfo;

	WORD GetMsgLength()	{	return sizeof(SEND_CHARACTER_TOTALINFO) - sizeof(CAddableInfoList) + AddableInfo.GetInfoLength();	}
};

struct MSG_PET_INFO : public MSGBASE
{
	PET_OBJECT_INFO PetObjectInfo;
};

struct MSG_PET_ITEM_INFO : public MSGBASE
{
	DWORD	ID;
	ITEMBASE	Item[ePetEquipSlot_Max];
};

struct MSG_OPTION_HIDEPARTS : public MSGBASE
{
	bool bHideParts[eWearedItem_Max];
};
//
#pragma pack(pop)
