#pragma once


enum ATTRIBUTE_ELEMENT
{
	ATTR_NONE = 0,
	ATTR_FIRE = 1,
	ATTR_WATER = 2,
	ATTR_TREE = 3,
	ATTR_IRON = 4,
	ATTR_EARTH = 5,

	ATTR_MAX = 5,

	ATTR_NOATTR = 6,
	ATTR_ALLATTR = 7,
};

template < class T >
union ATTRIBUTE_VAL
{
private:
	struct{
	T FireElement;
	T WaterElement;
	T TreeElement;
	T GoldElement;
	T EarthElement;
	T NoneElement;
	};

	T Element[ATTR_MAX+1];
public:
	ATTRIBUTE_VAL()
	{
		NoneElement = 0;
	}
	T GetElement_Val(WORD Attrib)
	{
		if(Attrib == ATTR_NONE)	
			return 0;

#ifndef _RMTOOL_
		ASSERTMSG(Attrib != ATTR_ALLATTR,"?ü¼Ó¼º?º °ª?» ºÒ·¯¿Ã ¼ö ¾ø½?´Ï´Ù.(Call to yunho plz~ ^-^)");
#else
		ASSERT(Attrib != ATTR_ALLATTR);
#endif _RMTOOL_

		if(Attrib > ATTR_MAX+1)
		{
#ifndef _RMTOOL_
			ASSERTMSG(0,"?±È£¸¦ ºÒ·¯ÁÖ¼¼¿ä~ -0-");
#else
			ASSERT(0);
#endif _RMTOOL_
			return 0;
		}
		return Element[Attrib-1];
	}
	void SetElement_Val(WORD Attrib,T val)
	{
		if(Attrib == ATTR_NONE)
			return;
#ifndef _RMTOOL_		
		ASSERTMSG(Attrib != ATTR_ALLATTR,"?ü¼Ó¼º?º °ª?» ºÒ·¯¿Ã ¼ö ¾ø½?´Ï´Ù.(Call to yunho plz~ ^-^)");
		if(Attrib > ATTR_MAX+1)
		{
			ASSERTMSG(0,"atrrib is too big");
			return;
		}
#endif
		Element[Attrib-1] = val;
	}
	void AddATTRIBUTE_VAL(ATTRIBUTE_VAL<T>& attr_val)
	{
		for(int n=0;n<ATTR_MAX+1;++n)
			Element[n] += attr_val.Element[n];
	}
	void AddATTRIBUTE_VAL(ATTRIBUTE_VAL<T>& attr_val,float fRate)
	{
		for(int n=0;n<ATTR_MAX+1;++n)
			Element[n] += (T)(attr_val.Element[n]*fRate);
	}
	void CheckLimit(T max_val)
	{
		for(int n=0;n<ATTR_MAX+1;++n)
		{
			if(Element[n] > max_val)
				Element[n] = max_val;
		}
	}
};

typedef ATTRIBUTE_VAL<float>	ATTRIBUTEATTACK;
typedef ATTRIBUTE_VAL<float>	ATTRIBUTEREGIST;


// 080130 LUJ, ��ġ�� �� ������ �ڷḦ ó���� �� �ֵ��� ��
struct PlayerStat
{
	struct Value
	{
		float mPlus;
		float mPercent;
	};

	Value mStrength;
	Value mDexterity;
	Value mVitality;
	Value mIntelligence;
	Value mWisdom;
	Value mPhysicAttack;
	Value mPhysicDefense;
	Value mMagicAttack;
	Value mMagicDefense;
	Value mMana;
	Value mLife;
	Value mRecoveryLife;
	Value mRecoveryMana;
	Value mCriticalRate;
	Value mCriticalDamage;
	Value mMoveSpeed;
	Value mAccuracy;
	Value mEvade;
};

struct monster_stats
{
	WORD	Attack1OccurProportion;					// °ø°Ý1 ¹ß»ý È®·ü
	// ¹°¸® °ø°Ý
	WORD	PhysicalAttackMin1;						// °ø°Ý1 ÃÖ¼Ò °ø°Ý·Â
	WORD	PhysicalAttackMax1;
	WORD	PhysicalAttackMin2;						// °ø°Ý1 ÃÖ¼Ò °ø°Ý·Â
	WORD	PhysicalAttackMax2;

	// ¼Ó¼º °ø°Ý
	WORD	AttributeAttackMin1;
	WORD	AttributeAttackMax1;
	WORD	AttributeAttackMin2;
	WORD	AttributeAttackMax2;

	// ¹°¸® ¹æ¾î
	WORD	PhysicalDefense;	

	ATTRIBUTEREGIST AttributeResist;
};


struct TILEINDEX
{
	int nx,nz;	
	inline TILEINDEX operator +(const TILEINDEX &ti)
	{
		TILEINDEX result;
		result.nx	=	this->nx + ti.nx;
		result.nz	=	this->nz + ti.nz;
		return	result;
	}
	inline BOOL operator ==(const TILEINDEX &ti)
	{
		return (this->nx == ti.nx) && (this->nz == ti.nz);
	}
};


#define MAX_REGIST_HYPERLINK  70

enum LINKTYPE {
	emLink_Null,
	emLink_Page,
	emLink_End,	
	emLink_Open,
	emLink_Quest,
	emLink_QuestLink,			//---- 5
	emLink_QuestStart,			//---- 6 
	emLink_QuestContinue,		//---- 7
	emLink_QuestEnd,			//---- 8
	emLink_MapChange,			//---- 9
	emLink_EventQuestStart,
	emLink_Image,
	// 070416 LYW --- CommonGameStruct : Add enum to change stage.
	emLink_ChangeStage,
	// 070918 LYW --- CommonGameStruct : Add enum to change map through the npc.
	emLink_ChangeMapPage,
	emLink_FishPointChange,		//---- 14
};

struct DIALOGUE {
	void Init()
	{
		wType = 0;
		dwColor = RGB(255,255,255);
		memset(str, 0, 256);
		wLine = 0;
	}
	WORD	wType; // 0 : ÅØ½ºÆ®, 1 : ?Ì¹ÌÁö, 2 : HyperLink
	DWORD	dwColor;
	char	str[256];
	WORD	wLine;
};

struct HYPERLINK				// ?�이??링크 구조�?
{
	WORD		wLinkId;		// 링크 ?�이??
	WORD		wLinkType;		// 링크 ?�??
	DWORD		dwData;			// ?�이??

	// 070917 LYW --- CommonGameStruct : Add member for map change.
	float		fXpos ;
	float		fZpos ;

};

struct  HYPER					// ?�이??구조�?
{
	void Init()					// 초기???�수.
	{
		bUse = FALSE;			// ?�용?��?�?FALSE�??�팅?�다.
		dwListItemIdx = 0;		// 리스???�이???�덱?��? 0?�로 ?�팅?�다.

		memset(&sHyper, 0, sizeof(HYPERLINK)) ;
	}

	HYPERLINK	sHyper;
	DWORD		dwListItemIdx;
	BOOL		bUse;
};
