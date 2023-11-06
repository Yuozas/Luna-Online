#pragma once


#define ITEMMGR USINGTON(CItemManager)


#include "item.h"


class CHero;
class CPlayer;
class cImage;
class cWindow;
class CVirtualItem;
class CItemShow;
class cQuickItem;
class CExchangeItem;
class CBuyItem;

//---for item tooltip
struct TOOLTIP_TEXT
{
	TOOLTIP_TEXT() { strToolTip = NULL; pNext = NULL; }
	char*			strToolTip;
	TOOLTIP_TEXT*	pNext;
};


// 조합 아이템 기본 자료구조
struct ItemMixResult
{
	DWORD	mItemIndex;
	WORD	mRequiredLevel;
	DWORD	mMoney;
	float	mSuccessRate;

	// 키: 아이템 인덱스, 값: 소요 개수
	typedef stdext::hash_map< DWORD, DWORD >	Material;
	Material									mMaterial;
};


typedef std::list< ItemMixResult >	MixScript;


// 세트 아이템 기본 자료 구조
struct SetScript
{
	std::string mName;

	// 세트로 구성되는 아이템 
	typedef stdext::hash_set< DWORD >	Item;
	Item								mItem;

	struct Element
	{
		PlayerStat	mStat;

		// 키: 스킬 인덱스, 값: 레벨 증감
		typedef std::map< DWORD, BYTE >	Skill;
		Skill							mSkill;
	};

	// 키: 능력이 발현되는 세트 아이템 구성 개수
	typedef stdext::hash_map< int, Element >	Ability;
	Ability										mAbility;
};


struct DissolveScript
{
	typedef struct
	{
		DWORD mItemIndex;
		DWORD mQuantity;
	}
	Result;

	// 분해시 무조건 출현하는 아이템
	typedef std::list< Result > StaticResult;
	StaticResult				mStaticResult;

	// 분해시 확률에 따라 출현하는 아이템
	typedef std::map< DWORD, Result >	DynamicResult;
	DynamicResult								mDynamicResult;

	// 분해가능한 최소 레벨
	DWORD mLevel;
};


struct ReinforceScript
{
	enum eType
	{
		eTypeNone,
		eTypeStrength,
		eTypeDexterity,
		eTypeVitality,
		eTypeIntelligence,
		eTypeWisdom,
		eTypeLife,
		eTypeMana,
		eTypeLifeRecovery,
		eTypeManaRecovery,
		eTypePhysicAttack,
		eTypePhysicDefence,
		eTypeMagicAttack,
		eTypeMagicDefence,
		eTypeMoveSpeed,
		eTypeEvade,
		eTypeAccuracy,
		eTypeCriticalRate,
		eTypeCriticalDamage,
		eTypeMax
	};

	eType	mType;		// 강화로 상승되는 능력 종류
	DWORD	mMax;		// 강화로 올릴 수 있는 최대 수치
	float	mBias;		// 강화를 위한 값
	BOOL	mForRare;	// 레어 아이템 강화 가능 여부

	// 해당 재료로 강화할 수 있는 아이템 종류(장착 슬롯으로 구분)
	typedef stdext::hash_set< eEquipSlot >	Slot;
	Slot									mSlot;
};


// 070810 웅주, 인챈트 스크립트
struct EnchantScript
{
	// 인챈트를 부여하는 아이템(인챈트 스크롤 등)이 가지는 인덱스
	DWORD mItemIndex;

	enum eType
	{
		eTypeNone,
		eTypeStrength,
		eTypeDexterity,
		eTypeVitality,
		eTypeIntelligence,
		eTypeWisdom,
		eTypeLife,
		eTypeMana,
		eTypeLifeRecovery,
		eTypeManaRecovery,
		eTypePhysicAttack,
		eTypePhysicDefence,
		eTypeMagicAttack,
		eTypeMagicDefence,
		eTypeMoveSpeed,
		eTypeEvade,
		eTypeAccuracy,
		eTypeCriticalRate,
		eTypeCriticalDamage,
		eTypeMax
	};

	// 인챈 가능한 아이템 착용 제한 레벨 범위
	struct
	{
		DWORD mMin;
		DWORD mMax;
	}
	mItemLevel;

	DWORD mEnchantMaxLevel;	// 최대로 인챈트할 수 있는 단계

	typedef stdext::hash_set< eType >	Ability;
	Ability								mAbility;

	// 해당 재료로 인챈트할 수 있는 아이템 종류(장착 슬롯으로 구분)
	typedef stdext::hash_set< eEquipSlot >	Slot;
	Slot									mSlot;
};


struct ApplyOptionScript
{
	struct
	{
		int	mMin;
		int mMax;
	}
	mLevel;

	// 키: 확률, 값: 부여될 옵션 개수
	typedef std::map< float, DWORD >	OptionSizeMap;
	OptionSizeMap						mOptionSizeMap;

	// 적용될 아이템 종류
	typedef stdext::hash_set< LONGLONG >	TargetTypeSet;
	TargetTypeSet							mTargetTypeSet;

	struct Option
	{
		ITEM_OPTION::Drop::Key	mKey;

		float	mBeginValue;
		float	mEndValue;
	};

	// 확률마다 선택될 옵션 종류와 수치 범위를 담고 있다
	// 키: 선택 확률
	typedef std::map< float, Option >	OptionTypeMap;
	OptionTypeMap						mOptionTypeMap;
};

struct EnchantEffect
{
	DWORD	ItemIdx;

	WORD	Effect[13][3];
};

struct EVENTINFO
{
	DWORD	Index;
	DWORD	Delay;
	DWORD	Next;
	DWORD	Item;
	DWORD	Count;
	DWORD	Window;
	char*	Msg;
};

class CItemManager  
{
	//CItemCalc m_ItemCalc;
	CYHHashTable<EVENTINFO>			mEventInfoTable;

	DWORD			m_GiftEventTime;
	DWORD			m_GifeEventCheckTime;

	CYHHashTable<CItem> m_ItemHash;

	CYHHashTable<ITEM_INFO> m_ItemInfoList;

	CYHHashTable<EnchantEffect> mEffectList;
	/////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 은신/혜안
	// 은신시 사용못하는 아이템
	CYHHashTable<ITEM_INFO> m_UnfitItemInfoListForHide;
	/////////////////////////////////////////////

	CYHHashTable<TOOLTIP_TEXT>		m_ItemToolTipTextList;
	//CYHHashTable<ITEM_OPTION>	m_ItemOptionList;

	// 080124 LUJ
	// 키: 아이템 DB 인덱스
	typedef stdext::hash_map< DWORD, ITEM_OPTION* >	OptionMap;
	OptionMap										mOptionMap;
	//CYHHashTable<SHOPITEMBASE>		m_UsedItemList;

	CIndexGenerator m_IconIndexCreator;
	BOOL			m_bAddPrice;
	ITEMBASE		m_TempDeleteItem;

	//---KES ItemDivide Fix 071020
	POSTYPE			m_DividePos;
	int				m_DivideDur;
	//----------------------------

	// 071127 LYW --- ItemManager : 삭제 아이템 ( 봉인 관련 ) 위치 저장 변수 추가.
	int			m_nSealItemDelPos ;

public:
	//	//MAKESINGLETON(CItemManager)

	CItemManager();
	virtual ~CItemManager();

	//---KES ItemDivide Fix 071020
	void SetDivideItemInfo( POSTYPE pos, int dur )	{ m_DividePos = pos; m_DivideDur = dur; }
	POSTYPE GetDivideItemInfoPos()					{ return m_DividePos; }
	int		GetDivideItemInfoDur()					{ return m_DivideDur; }
	//----------------------------

	void				AddOption	( const ITEM_OPTION& );
	const ITEM_OPTION&	GetOption	( const ITEMBASE& ) const;
	const ITEM_OPTION&	GetOption	( DWORD itemDbIndex ) const;
	void				RemoveOption( DWORD itemDbIndex );


	void Release();

	CItem* MakeNewItem( const ITEMBASE*, const char* strKind);

	void ItemDelete(CItem* pItem);
	void FakeDeleteItem(CItem* pItem);
	void SendDeleteItem();

	cImage* GetIconImage(DWORD ItemIdx, cImage * pImageGet);
	CItem* GetItem(DWORD dwDBIdx);

	// 071215 LYW --- ItemManager : 퀵슬롯의 소셜아이콘 툴팀 처리 함수 추가.
	void SetSocialIconTooltip(cQuickItem* pItem, SLOT_INFO* pSlotInfo) ;

public:
	void AddToolTip( cWindow*, DWORD toolTipIndex, DWORD color = TTTC_DESCRIPTION );
	void AddToolTip( cIcon* );
	void AddToolTip( CVirtualItem* );
	void AddToolTip( CItemShow* );
	void AddToolTip( CItem* );
	void AddToolTip( cQuickItem* );
	void AddToolTip( CExchangeItem* );
	void AddToolTip( CBuyItem* );

	// 071125 LYW --- ItemManager : 시간 툴팁 전용 설정함수 추가.
	void AddTimeToolTip( cIcon* icon, const ITEM_OPTION&, BOOL isFirst ) ;

private:
	void AddNametoolTip			( cIcon&, const ITEMBASE& );
	void AddOptionToolTip		( cIcon&, const ITEMBASE& );
	void AddFunctionToolTip		( cIcon&, const ITEMBASE& );
	void AddMakerToolTip		( cIcon&, const ITEMBASE& );
	void AddSetToolTip			( cIcon&, const ITEMBASE& );
	void AddPetToolTip			( cIcon&, const ITEMBASE& );
	void AddDescriptionToolTip	( cIcon&, const ITEMBASE&, DWORD color = TTTC_DESCRIPTION );
	
	// 071115 LYW --- ItemManager : 남은 사용시간 툴팁 추가.
	void AddRemainTimeToolTip( cIcon&, const ITEMBASE&, BOOL bFirst );

	// 071126 LYW --- ItemManager : 남은 사용시간 툴팁 추가.
	void AddUseableTimeToolTip( cIcon&, const ITEMBASE& );


public:
	void NetworkMsgParse(BYTE Protocol,void* pMsg);

	void MoveItem( MSG_ITEM_MOVE_ACK * pMsg );
	void PetMoveItem( MSG_ITEM_MOVE_ACK * pMsg );
	void DivideItem(ITEMOBTAINARRAY * pMsg );
	/*	
	void MoveItemToMunpa( MSG_ITEM_MUNPA_MOVE_SYN* pMsg );
	void MoveItemToStorage(MSG_ITEM_MUNPA_MOVE_SYN* pMsg);
	*/
	void MoveItemToGuild( MSG_ITEM_MOVE_ACK* pMsg );
	void MoveItemToStorage( MSG_ITEM_MOVE_ACK* pMsg );
	void MoveItemToShop(MSG_ITEM_MOVE_ACK* pMsg);
	void MoveItemToShopInven(MSG_ITEM_MOVE_ACK* pMsg);

	void CombineItem( MSG_ITEM_COMBINE_ACK * pMsg );
	//	void DivideItem( MSG_ITEM_DIVIDE_ACK * pMsg );
	//SW051012 수정
	// 080218 LUJ, 쓰지 않는 옵션 정보 추출을 제거함
	void DeleteItem( POSTYPE absPos, CItem** ppItemOut );
	BOOL AddItem(CItem* pItem);

	//	void GetYoungyakItemToolTip(char * tooltip, int lens, ITEM_INFO * pInfo);
	//	void GetSkillItemToolTip(char * tooltip, int lens, ITEM_INFO * pInfo);
	//	void GetEquipItemToolTip(char * tooltip, int lens, ITEM_INFO * pInfo);
	//	void GetExtraItemToolTip(char * tooltip, int lens, ITEM_INFO * pInfo);

	BYTE GetTableIdxForAbsPos(POSTYPE absPos);
	CItem* GetItemofTable(WORD TableIDX, POSTYPE absPos);

	// yunho StatsCalcManager 공유를 위해
	const ITEMBASE* GetItemInfoAbsIn( CPlayer*, POSTYPE absPos);

	//void DeleteItemofTable(WORD TableIdx, POSTYPE absPos, ITEM_OPTION * pItemOptionInfoOut=NULL/*, ITEM_RARE_OPTION_INFO* pItemRareOptionInfoOut = NULL*/);
	// 071202 LYW --- ItemManager : 봉인 아이템이 삭제 될 때, 순수 삭제될 때와 상점에 팔 때에도 같은 이 함수를 사용하기 때문에, 
	//								함수 뒤쪽 인자로 팔때인지 삭제인지 여부를 체크하도록 수정함.
	// 080218 LUJ, 쓰지 않는 옵션 추출을 제거함
	void DeleteItemofTable(WORD TableIdx, POSTYPE absPos, BOOL bSellDel ) ;
	BOOL IsEqualTableIdxForPos(WORD TableIdx, POSTYPE absPos );

	void RefreshAllItem();
	void RefreshItem( CItem* pItem );
	void RefreshItemToolTip(DWORD ItemDBIdx);
	BOOL CanEquip(DWORD wItemIdx);
	BOOL CanUse(DWORD wItemIdx);
	BOOL CanConvertToSkill(DWORD wItemIdx,SKILL_TYPE SkillType=SKILLTYPE_MAX);

	void LoadItemList();
	void LoadItemToolTipList();
	void LoadItemEffectList();
	void LoadGiftEvent();

	EnchantEffect* GetItemEffect( DWORD ItemIdx );
	ITEM_INFO * GetItemInfo(DWORD wItemIdx);
	void SetPreItemData(sPRELOAD_INFO* pPreLoadInfo, int* Level, int Count);
	DWORD m_dwStateParam;

	//SW050920 Rare
	//BOOL IsRareOptionItem( DWORD wItemIdx, DWORD dwRareIdx );
	BOOL IsRare( const ITEM_INFO* ) const;

	//BOOL CanGradeUp(CItem * pItem);
	BOOL IsDupItem( DWORD wItemIdx );
	
	void SetPriceToItem( BOOL bAddPrice );
	void SetDisableDialog(BOOL val, BYTE TableIdx);

	///
	TOOLTIP_TEXT* GetItemToolTipInfo( DWORD wIdx );
	
	// 060911 KKR 샵 아이템 사용시간 뿌리기 
	void PrintShopItemUseTime( CItem* pItem, ITEM_INFO* pItemInfo );
	ITEM_INFO* FindItemInfoForName( char* strName );

	//	BOOL CheckQPosForItemIdx( DWORD wItemIdx );

	void ReLinkQuickPosition(CItem* pItem);	

	//
	// 071121 LYW - Modified.
	void AddUsedItemInfo(ITEMBASE* pInfo) ;
	//void AddUsedItemInfo(SHOPITEMBASE* pInfo);
	//void RemoveUsedItemInfo(DWORD wIndex, DWORD dwDBIdx=0);
	// 071121 LYW - Modified.
	//SHOPITEMBASE* GetUsedItemInfo(DWORD Index);
	//ITEMBASE* GetUsedItemInfo(DWORD Index) ;
	void Process();
	//void DeleteShopItemInfo();
	// 071121 LYW - Modified.
	void AddUsedAvatarItemToolTip( ITEMBASE* pInfo ) ;
	//void AddUsedAvatarItemToolTip( SHOPITEMBASE* pInfo );
	void AddUsedShopEquipItemToolTip( SHOPITEMBASE* pInfo );

	void ReviveOtherOK();
	void ReviveOtherSync();
	void ReviveOtherCancel();
	void ReviveOtherConfirm( BOOL bRevive );

	//void RefreshStatsBuffIcon();
	void AddAvatarItemOptionTooltip( cIcon* pIcon, ITEM_INFO* pItemInfo );
	//	BOOL CheckItemStage( BYTE bItemStage );

	void ItemDropEffect( DWORD wItemIdx );
	void MoneyDropEffect();

	//BOOL LoadRareItemInfo();
	//BOOL IsRareItemAble( DWORD ItemIdx )
	//{
	//	if( m_RareItemInfoTable.GetData(ItemIdx) )			return TRUE;
	//	return FALSE;
	//}

	void SetIconColorFilter(DWORD dwDBIdx, DWORD clrRGBA);
	void SetIconColorFilter(cIcon* pIcon, DWORD clrRGBA);
	//for GM...현재는..
#ifdef _GMTOOL_
	void SetItemIfoPositionHead()	{ m_ItemInfoList.SetPositionHead(); }
	ITEM_INFO* GetItemInfoData()	{ return m_ItemInfoList.GetData(); }
#endif

	/////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 은신/혜안
	// 은신시 사용못하는 아이템
	ITEM_INFO* IsUnfitItemInfoListForHide(WORD idx) { return m_UnfitItemInfoListForHide.GetData(idx); }
	/////////////////////////////////////////////


	// 070205 LYW --- Add functions to process network msg.
private:
	void Item_TotalInfo_Local( void* pMsg ) ;
	void Item_Storage_Item_Info_Ack( void* pMsg ) ;
	//void Item_Monster_RareObtain_Notify( void* pMsg ) ;
	void Item_ReInForce_Ack( void* pMsg ) ;
	void Item_Reinforce_Option_Ack( void* );
	void Item_ReInForce_Nack( void* pMsg ) ;
	void Item_Move_Ack( void* pMsg ) ;
	void Item_Pet_Move_Ack( void* pMsg ) ;
	void Item_Combine_Ack( void* pMsg ) ;
	void Item_Divide_Ack( void* pMsg ) ;
	void Item_Monster_Obtain_Notify( void* pMsg ) ;
	void Item_Move_Nack( void* pMsg ) ;
	void Item_Divite_Nack( void* pMsg ) ;
	void Item_Combine_Nack( void* pMsg ) ;
	void Item_Discard_Nack( void* pMsg ) ;
	void Item_Discard_Ack( void* pMsg ) ;
	void Item_Use_Notify( void* pMsg ) ;
	void Item_Use_Ack( void* pMsg ) ;

private:
	void Item_Mix_Ack( void* pMsg ) ;
	void Item_Mix_Nack( void* pMsg );
	void Item_Mix_GetItem( void* );

private:
	// DB 인덱스가 없는 아이템을 보관하는 컨테이너. DB간의 랙으로 인해 갱신, 삭제에 비해
	// 삽입은 매우 느리다. 따라서 미리 사용 불가능한 아이콘을 생성해놓고, 응답이 올때
	// 사용가능하도록 할 것이다. 조합, 분해가 성공될 때만 해당된다
	typedef std::list< CItem* > PreIconList;
	PreIconList					mPreIconList;


	void Item_Working_Success( void* pMsg ) ;
	void Item_Working_Fail( void* pMsg ) ;
	void Item_Working_Start( void* pMsg ) ;
	void Item_Working_Stop( void* pMsg ) ;
	void Item_TimeLimit_Item_OneMinute( void* pMsg ) ;
	void Item_ShopItem_Use_Ack( void* pMsg ) ;
	void Item_ShopItem_Use_Nack( void* pMsg ) ;
	void Item_Enchant_Success_Ack( void* pMsg ) ;
	void Item_Enchant_Failed_Ack( void* pMsg );
	void Item_Enchant_Nack( void* pMsg ) ;
	void Item_Use_Nack( void* pMsg ) ;
	void Item_Sell_Ack( void* pMsg ) ;
	void Item_Sell_Nack( void* pMsg ) ;
	void Item_Buy_Ack( void* pMsg ) ;
	void Item_Buy_Nack( void* pMsg ) ;
	void Item_Dealer_Ack( void* pMsg ) ;
	//void Item_Appearance_Change( void* pMsg ) ;
	void Item_Appearance_Add( void* );
	void Item_Appearance_Remove( void* );
	void Item_Money( void* pMsg ) ;
	void Item_Obtain_Money( void* pMsg ) ;
	void Item_Divide_NewItem_Notify( void* pMsg ) ;
	void Item_Guild_Move_Ack( void* pMsg ) ;
	void Item_Guild_Move_Nack( void* pMsg ) ;
	//void Item_Guild_WareHouse_Info_Ack( void* pMsg ) ;
	//void Item_Guild_WareHouse_Info_Nack( void* pMsg ) ;
	void Item_DeleteFrom_GuildWare_Notify( void* pMsg ) ;
	void Item_AddTo_GuildWare_Notify( void* pMsg ) ;
	void Item_Storage_Move_Ack( void* pMsg ) ;
	void Item_Dissolve_GetItem( void* pMsg ) ;
	void Item_Dissolve_Ack( void* pMsg ) ;
	void Item_Dissolve_Nack( void* pMsg ) ;
	void Item_Error_Nack( void* pMsg ) ;
	void Item_ChangeItem_Nack( void* pMsg ) ;
	void Item_Use_ChangeItem_Ack( void* pMsg ) ;
	void Item_EventItem_Use( void* pMsg ) ;
	void Item_ShopItem_Info_Ack( void* pMsg ) ;
	// 071204 LYW --- ItemManager :
	void Item_Reset_Stats_Ack( void* pMsg ) ;
	// 071208 LYW --- ItemManager : 스탯 초기화 실패 처리.
	void Item_Reset_Stats_Nack( void* pMsg ) ;
	// 071210 LYW --- ItemManager : 인벤토리 확장 성공, 실패 함수 추가.
	void Item_Increase_Inventory_Ack( void* pMsg ) ;
	void Item_Increase_Inventory_Nack( void* pMsg ) ;
	// 071226 LYW --- ItemManager : 이름 변경 아이템 사용 성공, 실패 함수 추가.
	void Item_Change_Character_Name_Ack( void* pMsg ) ;
	void Item_Change_Character_Name_Nack( void* pMsg ) ;

	// 071203 LYW --- ItemManager
	void Item_Skill_Ack( void* pMsg ) ;

	void Item_Gift_Event_Notify( void* pMsg );

	// 070329 LYW --- ItemManager : Add function to make string.
	void CompositString( char* inputStr, char* str, ... ) ;


	// 070613 웅주, 세트 아이템 관리
public:	
	const SetScript*	GetSetScript( DWORD itemIndex ) const;
	int					GetSetItemSize( CPlayer*, const SetScript* );

private:
	void LoadSetScript();

	typedef std::map< DWORD, std::string > SetItemName;

	void ParseSetScript( std::list< std::string >&, SetScript&, const SetItemName& );

	// 세트 아이템 정보를 보관
	std::list< SetScript >	mSetScript;

	// 아이템 값을 받아 가지고 있는 세트 정보를 반환
	// 키: 아이템 인덱스, 값: 세트 아이템 정보
	typedef stdext::hash_map< DWORD, const SetScript* >	SetScriptMap;
	SetScriptMap										mSetScriptMap;


	// 070703 웅주, 조합 아이템 관리
public:	
	const MixScript*		GetMixScript( DWORD itemIndex ) const;
	const ItemMixResult*	GetMixResult( DWORD itemIndex, DWORD level ) const;

	// 080228 LUJ, 조합 보호 아이템 여부
	BOOL IsMixProtectionItem( const ITEMBASE& ) const;

private:
	void LoadMixScript();

	// 키: 아이템 인덱스
	typedef stdext::hash_map< DWORD, MixScript >	MixScriptMap;
	MixScriptMap									mMixScriptMap;

	// 080228 LUJ, 조합 보호 아이템 관리
	typedef stdext::hash_set< DWORD >	MixProtectionItemSet;
	MixProtectionItemSet				mMixProtectionItemSet;


	// 070709 아이템 이름 색상을 가져옴
public:
	DWORD	GetItemNameColor( const ITEM_INFO& ) const;


	// 070710 웅주, 분해 아이템 관리
public:
	const DissolveScript*	GetDissolveScript( DWORD itemIndex ) const;

private:
	void LoadDissolveScript();

	// 키: 아이템 인덱스
	typedef stdext::hash_map< DWORD, DissolveScript >	DissolveScriptMap;
	DissolveScriptMap									mDissolveScriptMap;


	// 070710 웅주, 강화 아이템 관리	
public:
	// 키: 아이템 인덱스
	typedef std::map< DWORD, ReinforceScript >	ReinforceScriptMap;

	const ReinforceScriptMap&	GetReinforceScript() const;
	const ReinforceScript*		GetReinforceScript( DWORD itemIndex ) const;

	// 080228 LUJ, 강화 보호 아이템 여부
	BOOL IsReinforceProtectionItem( const ITEMBASE& ) const;

private:
	void LoadReinforceScript();

	ReinforceScriptMap mReinforceScriptMap;

	// 080228 LUJ, 강화 보호 아이템 관리
	typedef stdext::hash_set< DWORD >	ReinforceProtectionItemSet;
	ReinforceProtectionItemSet			mReinforceProtectionItemSet;


	// 070810 웅주, 인챈트 아이템 관리
public:
	const EnchantScript*	GetEnchantScript( DWORD itemIndex ) const;
	void AddEnchantValue( ITEM_OPTION&, const EnchantScript::Ability&, int value ) const;

	// 080228 LUJ, 인챈트 가능한 아이템 여부
	BOOL IsEnchantableItem( const ITEMBASE&, BOOL isProtect ) const;

	// 080228 LUJ, 인챈트 보호 아이템 관리
	struct EnchantProtection
	{
		// 080320 LUJ, 보호 정보
		struct Data
		{
			// 080328 LUJ, 범위 구조체 추가
			struct Range
			{
				DWORD mMin;
				DWORD mMax;
			};

			// 080320 LUJ, 보호되는 인챈트 레벨 범위
			Range mEnchantLevelRange;

			// 080328 LUJ, 보호 가능한 아이템 장착 레벨
			Range mItemLimitLevelRange;

			// 080328 LUJ, 인챈트 마이너스 보너스 적용 시에도 보호되는 하한 인챈트 레벨
			DWORD mMinimumEnchantLevel;

			// 080328 LUJ, 인챈트 확률에 가감되는 값
			int mAddedPercent;

			// 080328 LUJ, 인챈트 성공/실패 시 인챈트 레벨이 얼마나 상승/하락할 지 결정하는 확률 맵. 키: 확률, 값: 추가될 인챈트 레벨
			typedef std::map< float, int > BonusMap;

			// 080328 LUJ, 인챈트 성공 시 사용하는 플러스 보너스 맵
			BonusMap mPlusBonusMap;

			// 080328 LUJ, 인챈트 실패 시 사용하는 마이너스 보너스 맵
			BonusMap mMinusBonusMap;			
		};

		// 080228 LUJ, 인챈트 보호 아이템
		// 080320 LUJ, 맵으로 바꾸고, 인챈트 보호 아이템의 종류를 값으로 설정함
		typedef stdext::hash_map< DWORD, Data >	ItemMap;
		ItemMap									mItemMap;
	};

	// 080228 LUJ, 인챈트 보호 조건을 반환
	const EnchantProtection& GetEnchantProtection() const;

private:
	void LoadEnchantScript();

	// 키: 아이템 인덱스
	typedef stdext::hash_map< DWORD, EnchantScript >	EnchantScriptMap;
	EnchantScriptMap									mEnchantScriptMap;

	// 080228 LUJ, 인챈트 보호 아이템 관리
	EnchantProtection mEnchantProtection;

	// 080218 LUJ, 옵션 부여 아이템 관리
public:
	BOOL IsApplyOptionItem( DWORD itemIndex ) const;
	const ApplyOptionScript* GetApplyOptionScript( DWORD itemIndex ) const;
	LONGLONG GetItemKey( eEquipSlot, eWeaponType, eArmorType, eWeaponAnimationType ) const;
	
private:
	void LoadApplyOptionScript();

	typedef stdext::hash_map< DWORD, ApplyOptionScript >	ApplyOptionScriptMap;
	ApplyOptionScriptMap									mApplyOptionScriptMap;


	// 070914 웅주, 쿨 타임 스크립트
public:
	DWORD GetCoolTimeGroupIndex( DWORD itemIndex ) const;

	// 해당 아이템이 쿨타임 중인지 체크한다.
	BOOL IsCoolTime( DWORD playerIndex, DWORD itemIndex ) const;

	// 아이템을 쿨타임 상태로 등록하거나 해제
	BOOL AddCoolTime( DWORD playerIndex, DWORD itemIndex );

	// 플레이어의 쿨타임 정보를 지운다.
	void RemoveCoolTime( DWORD playerIndex );

	// 080326 NYJ --- 쿨타임갱신
	void UpdateDlgCoolTime(DWORD itemIndex);	//HERO에게만 사용됨.


private:
	void ProcessCoolTime();
	void LoadCoolTimeScript();

	struct CoolTimeScript
	{
		struct Unit
		{
			DWORD mIndex;	// 쿨타임 그룹 인덱스
			DWORD mTime;	// 쿨타임 적용 시간: 밀리세컨드
		};

		// 키: 아이템 인덱스
		typedef stdext::hash_map< DWORD, Unit >	ItemData;
		ItemData								mItemData;

		// 값: 아이템 인덱스
		typedef stdext::hash_set< DWORD >				ItemIndex;

		// 키: 그룹 인덱스
		typedef stdext::hash_map< DWORD, ItemIndex >	GroupData;
		GroupData										mGroupData;
	}
	mCoolTimeScript;	

	// 쿨타임이 빨리 해제되는 순으로 정렬시키는데 사용하는 유틸리티 클래스.
	class CoolTimeSort
	{
	public:
		struct Tick
		{
			DWORD mPlayerIndex;		// 해당 쿨타임 유닛을 소유한 플레이어 인덱스
			DWORD mCoolTimeGroup;	// 쿨타임 그룹 인덱스
			DWORD mBegin;			// 쿨타임 시작 시간
			DWORD mEnd;				// 쿨타임 해제 시간
		};

		bool operator() ( const Tick& lhs, const Tick& rhs ) const
		{
			const DWORD	 maxTick		= UINT_MAX;
			const DWORD lhsRemainTime	= ( lhs.mBegin > lhs.mEnd ? maxTick - lhs.mBegin + lhs.mEnd : lhs.mEnd - lhs.mBegin );
			const DWORD rhsRemainTime	= ( rhs.mBegin > rhs.mEnd ? maxTick - rhs.mBegin + rhs.mEnd : rhs.mEnd - rhs.mBegin );

			return lhsRemainTime > rhsRemainTime;
		}
	};

	// 남은 시간이 적게 
	// 주의사항: 자료는 반드시 값 복사로 넣어야한다. 포인터로 넣을 경우 정렬하면서 힙 주소를 잃어버린다.
	std::priority_queue< CoolTimeSort::Tick, std::vector< CoolTimeSort::Tick >, CoolTimeSort > mCoolTimeQueue;

	// 값: 쿨타임 그룹
	typedef stdext::hash_set< DWORD >	CoolTimeGroup;

	// 키: 플레이어 인덱스, 값: 사용 중인 쿨타임 아이템 그룹 인덱스
	typedef stdext::hash_map< DWORD, CoolTimeGroup >	UsingCoolTimeGroup;
	UsingCoolTimeGroup									mUsingCoolTimeGroup;

	// 080326 NYJ --- 쿨타임정보 저장
	std::map<DWORD, CoolTimeSort::Tick> m_mapCoolTime;


	// 070927. 변환 아이템 사용시 프로그레스바 표시하도록 함
public:
	BOOL IsChangeItem( DWORD itemIndex ) const;

	// 071203 LYW --- ItemManager : 스탯 초기화 아이템 처리.
	void UseItem_ResetStats(CItem* pItem) ;
	// 071203 LYW --- ItemManager : 스킬 초기화 아이템 처리.
	void UseItem_ResetSkill(CItem* pItem) ;
	// 071203 LYW --- ItemManager : 맵 이동 스크롤 처리.
	void UseItem_MapMoveScroll(CItem* pItem) ;
	void Item_MapMoveScroll_Nack( void* pMsg ) ;
	// 071206 LYW --- ItemManager : 창고 확장 아이템 처리.
	void UseItem_ExtendStorage(CItem* pItem) ;
	// 080611 LYW --- 스킬 포인트 추가 주문서 처리.
	void GetSkillPointByItem(CItem* pItem) ;

	// 080607 LYW --- ItemManager : 창고 소환 함수 추가.
	void OpenStorageByItem(CItem* pItem) ;

	// 071210 LYW --- ItemManager : 인벤토리 확장 아이템 처리.
	void UseItem_ExtendInventory(CItem* pItem) ;
	// 071206 LYW --- ItemManager : 이미 사용하여 버프가 활성화 중인지 체크하는 함수 추가.
	BOOL IsUsingItem(CItem* pItem) ;
	// 071206 LYW --- ItemManager : 아이템몰 아이템 사용 함수 추가.
	void UseItem_ItemMallItem(CItem* pItem) ;
	// 071228 LYW --- ItemManager : 이름 변경 아이템 사용 함수 추가.
	void UseItem_ChangeName(char* pName) ;

	// 080218 KTH -- Reset RegistResident
	void UseItem_ResetRegistResident(CItem* pItem);

	// 080312 NYJ --- ItemManager : 전체외치기 아이템 처리.
	void UseItem_WorldShout(CItem* pItem) ;
	//void Item_WorldShout_ack( void* pMsg ) ;
	void Item_WorldShout_Nack( void* pMsg ) ;

	// 080326 NYJ --- 쿨타임정보 초기화
	void ClearCoolTimeInfo() {m_mapCoolTime.clear();}

	// 080412 LYW --- ItemManager : Add a function to check whether megaphone item or not.
	BOOL IsMegaphoneItem(DWORD dwItemIdx)
	{
		CoolTimeScript::ItemData::const_iterator it = mCoolTimeScript.mItemData.find(dwItemIdx) ;

		if(it != mCoolTimeScript.mItemData.end())
		{
			const CoolTimeScript::Unit& unit = it->second ;

			if( unit.mIndex == 10 ) return TRUE ;
			else return FALSE ;
		}
		else return FALSE ;
	}

private:
	void LoadChangeItem();

	typedef stdext::hash_set< DWORD >	ChangeItemSet;
	ChangeItemSet						mChangeItemSet;

	// 080414 LUJ, 외양 변환 스크립트 처리
public:
	struct BodyChangeScript
	{
		DWORD		mGender;
		RaceType	mRace;
		PART_TYPE	mPart;
		
		// 080414 LUJ, 텍스트 번호(InterfaceMsg.bin 참조)
		DWORD		mText;
		DWORD		mValue;
	};

	typedef std::list< BodyChangeScript >					BodyChangeScriptList;
	typedef stdext::hash_map< DWORD, BodyChangeScriptList >	BodyChangeScriptListMap;
	
	const BodyChangeScriptListMap&	GetBodyChangeScriptListMap() const;
	
private:	
	BodyChangeScriptListMap	mBodyChangeScriptListMap;
	
	void LoadBodyChangeScript();
};


EXTERNGLOBALTON(CItemManager)
