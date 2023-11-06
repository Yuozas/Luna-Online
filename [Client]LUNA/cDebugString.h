#pragma once

#include "Npc.h"
#include "Player.h"
//#ifdef _DEBUG
enum DEBUG_INFO
{
	e_print_npc_totalinfo = 1,
	e_print_npc_listinfo,
	e_print_player_totalInfo,
	e_print_hpmp_determination,
	e_print_palyer_experience,
	e_print_itemInfo,
	e_print_dropitem,
} ;


struct ITEM_INFO;


class cDebugString
{
	BOOL m_bActive ;

	BYTE m_byOption ;
public:
	cDebugString(void);
	~cDebugString(void);

	void set_debug_string(WORD wInfoType, BOOL bOnOff ) ;

	void SetActive( BOOL val ) { m_bActive = val ; }
	BOOL IsActive() { return m_bActive ; }

	void SetPos(BYTE index, VECTOR2* vPos) ;
	void SetOption(BYTE index, BYTE opNum ) ;

	void print_debug_string() ;
	void print_debug_string(WORD wInfoType, BOOL bPrint) ;

////////////////////////////////////////////
// NPC PART
////////////////////////////////////////////
private :
	CNpc* m_pNpc ;

	BOOL m_bOnNpcTotalInfo ;
	BOOL m_bOnNpcListInfo ;

	VECTOR2 m_vNpcTotal ;
	VECTOR2 m_vNpcList ;
public :
	void SetNpc( CNpc* pNpc ) { m_pNpc = pNpc ; }

	void print_npc_totalinfo() ;
	void print_npc_listinfo() ;

////////////////////////////////////////////
// CHARACTER PART
////////////////////////////////////////////
private :
	CPlayer* m_pPlayer ;

	BOOL m_bOnCharTotalInfo ;

	VECTOR2 m_vCharTotal ;
public :
	void SetPlayer( CPlayer* pPlayer ) { m_pPlayer = pPlayer ; }

	void print_char_totalInfo() ;

////////////////////////////////////////////
// HP MP PART
////////////////////////////////////////////
private :
	BOOL m_bOnHPMP ;

	BYTE m_bHpPoint ;
	BYTE m_bMpPoint ;

	VECTOR2 m_vHPMP ;
public :
	void SetHpMpPoint(BYTE hp, BYTE mp) ;
	void print_hpmp_determination() ;

////////////////////////////////////////////
// EXPERIENCE PART
////////////////////////////////////////////
private :
	BOOL m_bOnExperience ;
	BOOL m_bIsSetInfo ;

	MSG_EXP_RESULT m_Msg ;

	VECTOR2 m_vExperience ;
public :
	void SetExpInfo(MSG_EXP_RESULT* pMsg) ; 
	void print_ExpInfo() ;

////////////////////////////////////////////
// ITEM INFO
////////////////////////////////////////////
private :
	ITEM_INFO*	m_pItemInfo ;
	VECTOR2		m_vItemInfo ;

	BOOL		m_bOnItemInfo ;

public :
	void SetItemInfo(ITEM_INFO* pItemInfo) ;
	void print_ItemInfo() ;

////////////////////////////////////////////
// DROPITEM INFO
////////////////////////////////////////////
private :
	BOOL m_bOnDropItem ;
	BOOL m_bInitialized ;

	MSG_DROPMONEY_RESULT m_DropMoney ;
	MSG_DROPITEM_RESULT m_DropItem ;

	VECTOR2 m_vDropItem ;

public :
	void Set_DropMoney(MSG_DROPMONEY_RESULT* pMoney) ;
	void Set_DropItem(MSG_DROPITEM_RESULT* pItem) ;

	void print_DropItem() ;


////////////////////////////////////////////
// QUEST PART
////////////////////////////////////////////
};
//#endif