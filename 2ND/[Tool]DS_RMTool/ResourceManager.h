// GameResourceManager.h: interface for the CGameResourceManager class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __RESOURCEMANAGER_H__
#define __RESOURCEMANAGER_H__

//#include "..\[CC]Header\GameResourceStruct.h"
//#include ".\common\Commondefine.h"
#include "MHFile.h"
#include "..\[CC]Ability\AbilityInfo.h"
#include "..\[CC]Quest\QuestString.h"

#define GAMERESRCMNGR	USINGTON(CGameResourceManager)

#define	MAX_ETC_MSGNUM	100

class CGameResourceManager  
{
protected:
//	CharBasicStats m_BasicStatsForJob[MAX_JOB]; // √Å√∑¬æ√∑¬∫¬∞ ¬Ω¬∫√Ö√à
	DWORD m_CharacterExpPoint[MAX_CHARACTER_LEVEL_NUM]; // ¬∑¬π¬∫¬ß¬∫¬∞ √á√ä¬ø√§ ¬∞√¶√á√®√Ñ¬°

	CYHHashTable<ITEM_INFO>			m_ItemInfoList;
	CYHHashTable<SKILLINFO>			m_SkillInfoList;
	CYHHashTable<KYUNGGONG_INFO>	m_KyunggongInfoList;
	
	CYHHashTable<CAbilityInfo>		m_AbilityInfoList;

	CYHHashTable<NPC_LIST>			m_NpcList;
	CYHHashTable<MONSTEREX_LIST>		m_MonsterList;

	CYHHashTable<CQuestString>		m_QuestStringTable;
	
	CYHHashTable<char>				m_MapNameTable;
	CYHHashTable<PET_NAME>			m_PetNameTable;
	
	char*							m_pEtcMsg[MAX_ETC_MSGNUM];
	char							m_NULL;							//√§∆√ ∏ﬁºº¡ˆ∞° æ¯¥¬ π¯»£¿œ∞ÊøÏ ∏Æ≈œ«œ±‚¿ß«— ≈€«¡∏‚πˆ

public:
	CGameResourceManager();
	virtual ~CGameResourceManager();

	virtual BOOL Init();

	//////////////////////////////////////////////////////////
	////// Data Loading
//	BOOL LoadCharCreateBasicStats();
	BOOL LoadExpPoint();

	BOOL LoadItemList();
	BOOL LoadSkillInfoList();
	BOOL LoadKyunggongInfoList();
	BOOL LoadAbilityInfoList();
	BOOL LoadNpcList();
	BOOL LoadMonsterList();
	BOOL LoadMapNameList();
	BOOL LoadPetList();
	void	LoadEtcMsgList();
	char*	GetMapName( DWORD dwMapNum );
	DWORD	GetMapNum( char* sMapName );
	void			LoadQuestString();
	CQuestString*	ReadQuestString(CMHFile* pFile);
	char*			GetQuestTitle( DWORD dwQuestIdx );
	char*			GetSubQuestTitle( DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	//////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////
	////// Check Func
	BOOL IsDupItem( WORD wItemIdx );
	BOOL IsOptionItem( WORD wItemIdx, WORD wDurability );
	//////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////
	////// Data √à¬π¬µ√¶ √á√î¬º√∂
	DWORD				GetMaxExpPoint(WORD level)		{ return m_CharacterExpPoint[level-1]; }
	ITEM_INFO*			GetItemInfo(WORD wItemIdx)		{ return m_ItemInfoList.GetData(wItemIdx); }
	SKILLINFO*			GetSkillInfo(WORD wSkillIdx)	{ return m_SkillInfoList.GetData(wSkillIdx); }
	CAbilityInfo*		GetAbilityInfo(WORD wAbilityIdx) { return m_AbilityInfoList.GetData(wAbilityIdx); }
	KYUNGGONG_INFO*		GetKyunggongInfo(WORD wKyuggongIdx) { return m_KyunggongInfoList.GetData(wKyuggongIdx); }
	NPC_LIST*			GetNpcList(DWORD dwKind) { return m_NpcList.GetData(dwKind); }
	MONSTEREX_LIST*		GetMonsterInfo(WORD kind) { return m_MonsterList.GetData(kind);	}
	
	WORD				GetItemIdx_ByItemName(char* name);
	WORD				GetMugongIdx_ByMugongName(char* name);

	CYHHashTable<ITEM_INFO>*			GetItemInfo()	{ return &m_ItemInfoList; }
	CYHHashTable<SKILLINFO>*			GetSkillInfo()	{ return &m_SkillInfoList; }
	CYHHashTable<CAbilityInfo>*			GetAbilityInfo() { return &m_AbilityInfoList; }
	CYHHashTable<char>*					GetMapNameTable()	{ return &m_MapNameTable; }
	CYHHashTable<PET_NAME>*				GetPetName()	{ return &m_PetNameTable; }
	/// 060915 PKH GMTOOLLOG
	CYHHashTable<char>*					GetGMToolLogTypeTable()	{ return &m_GMToolLogTypeTable; }
	//////////////////////////////////////////////////////////

protected:
	CYHHashTable<char>		m_MoneyLogTypeTable;
	CYHHashTable<char>		m_MugongLogTypeTable;
	CYHHashTable<char>		m_ItemMoneyLogTypeTable;
	CYHHashTable<char>		m_ExpPointLogTypeTable;
	CYHHashTable<char>		m_RMToolLogTypeTable;
	CYHHashTable<char>		m_GMToolLogTypeTable;
	CYHHashTable<char>		m_PetLogTypeTable;
	/// 060831 PKH πÆ∆ƒ ≈‰≥ ∏’∆Æ
	CYHHashTable<char>		m_GuildTNLogTypeTable;
	/// 060831 PKH ∞¯º∫¿¸
	CYHHashTable<char>		m_SiegeWarLogTypeTable;
	/// 060915 PKH GMTOOLLOG
	CYHHashTable<char>		m_GMToolLogLogTypeTable;

public:
	void	ReleaseLogTypeTable();
	BOOL	ReadLogType();
	void	ReadLogTypeFromKind( CMHFile* pFile, DWORD dwKind );

	char*	GetMoneyLogType( DWORD dwLogType, char* temp );
	char*	GetMugongLogType( DWORD dwLogType, char* temp );
	char*	GetItemMoneyLogType( DWORD dwLogType, char* temp );
	char*	GetExpPointLogType( DWORD dwLogType, char* temp );
	char*	GetRMToolLogType( DWORD dwLogType, char* temp );
	char*	GetGMToolLogType( DWORD dwLogType, char* temp );
	char*	GetPetLogType( DWORD dwLogType, char* temp );
	/// 060831 PKH πÆ∆ƒ≈‰≥ ∏’∆Æ
	char*	GetGuildTNLogType( DWORD dwLogType, char* temp );
	/// 060831 PKH ∞¯º∫¿¸
	char*	GetSiegeWarLogType( DWORD dwLogType, char* temp );
	char*	GetEtcMsg( int nMsgNum );


	//06.08.04 - [pkh] 
	char*	GetPetNamechar( DWORD dwNameidx );
};

EXTERNGLOBALTON(CGameResourceManager);

#endif //__GAME_RESOURCE_STRUCT__
