#include "stdafx.h"
#include ".\cdebugstring.h"
//#ifdef _DEBUG
#include "cFont.h"

#include "Npc.h"
#include "Player.h"
#include "ObjectManager.h"
#include "ItemManager.h"
// 070413 LYW --- Distributer : Include CommonCalcFunc.
#include "..\[CC]Header\CommonCalcFunc.h"

cDebugString::cDebugString(void)
{
	m_bActive	= FALSE ;

	m_byOption	= 0 ;

	////////////////////////////////////////////
	// NPC PART
	////////////////////////////////////////////
	m_pNpc = NULL ;

	m_bOnNpcTotalInfo	= FALSE ;
	m_bOnNpcListInfo	= FALSE ;

	m_vNpcTotal.x = 100 ;
	m_vNpcTotal.y = 100 ;

	m_vNpcList.x = 100 ;
	m_vNpcList.y = 100 ;

	////////////////////////////////////////////
	// CHARACTER PART
	////////////////////////////////////////////
	m_pPlayer = NULL ;

	m_bOnCharTotalInfo = FALSE ;

	m_vCharTotal.x = 100 ;
	m_vCharTotal.y = 100 ;

	////////////////////////////////////////////
	// HP MP PART
	////////////////////////////////////////////
	m_bOnHPMP = FALSE ;

	m_bHpPoint = 0 ;
	m_bMpPoint = 0 ;

	m_vHPMP.x = 100 ;
	m_vHPMP.y = 100 ;

	////////////////////////////////////////////
	// EXPERIENCE PART
	////////////////////////////////////////////
	m_bOnExperience = FALSE ;
	m_bIsSetInfo    = FALSE ;

	memset(&m_Msg, 0, sizeof(MSG_EXP_RESULT)) ;

	m_vExperience.x = 100 ;
	m_vExperience.y = 100 ;

	////////////////////////////////////////////
	// ITEM INFO
	////////////////////////////////////////////
	m_pItemInfo   = NULL ;

	m_bOnItemInfo = FALSE ;

	m_vItemInfo.x = 100 ;
	m_vItemInfo.y = 100 ;

	////////////////////////////////////////////
	// DROPITEM INFO
	////////////////////////////////////////////
	m_bOnDropItem	= FALSE ;
	m_bInitialized	= FALSE ;

	memset(&m_DropMoney, 0, sizeof(MSG_DROPMONEY_RESULT)) ;
	memset(&m_DropItem, 0, sizeof(MSG_DROPITEM_RESULT)) ;

	m_vDropItem.x = 100 ;
	m_vDropItem.y = 100 ;
}

cDebugString::~cDebugString(void)
{
}

void cDebugString::set_debug_string(WORD wInfoType, BOOL bOnOff ) 
{
	switch( wInfoType )
	{
		case e_print_npc_totalinfo :		m_bOnNpcTotalInfo	= bOnOff ;	break ;
		case e_print_npc_listinfo :			m_bOnNpcListInfo	= bOnOff ;	break ;
		case e_print_player_totalInfo :		m_bOnCharTotalInfo	= bOnOff ;	break ;
		case e_print_hpmp_determination :	m_bOnHPMP			= bOnOff ;	break ;
		case e_print_palyer_experience :	m_bOnExperience		= bOnOff ;	break ;
		case e_print_itemInfo :				m_bOnItemInfo		= bOnOff ;	break ;
		case e_print_dropitem :				m_bOnDropItem		= bOnOff ;  break ;
	}
}

void cDebugString::print_debug_string()
{
	if(!m_bActive) return ;

	print_npc_totalinfo() ;
	print_npc_listinfo() ;
	print_char_totalInfo() ;
	print_hpmp_determination() ;
	print_ExpInfo() ;
	print_ItemInfo() ;
	print_DropItem() ;
}

void cDebugString::print_debug_string(WORD wInfoType, BOOL bPrint) 
{
	
}

void cDebugString::SetPos(BYTE index, VECTOR2* vPos) 
{
	switch(index)
	{
		case e_print_npc_totalinfo :		memcpy(&m_vNpcTotal, vPos, sizeof(VECTOR2)) ;	break ;
		case e_print_npc_listinfo :			memcpy(&m_vNpcList, vPos, sizeof(VECTOR2)) ;	break ;
		case e_print_player_totalInfo :		memcpy(&m_vCharTotal, vPos, sizeof(VECTOR2)) ;	break ;
		case e_print_hpmp_determination :	memcpy(&m_vHPMP, vPos, sizeof(VECTOR2)) ;		break ;
		case e_print_palyer_experience :	memcpy(&m_vExperience, vPos, sizeof(VECTOR2)) ;	break ;
		case e_print_itemInfo :				memcpy(&m_vItemInfo, vPos, sizeof(VECTOR2)) ;	break ;
		case e_print_dropitem :				memcpy(&m_vDropItem, vPos, sizeof(VECTOR2)) ;	break ;
	}
}

void cDebugString::SetOption(BYTE index, BYTE opNum ) 
{
	switch(index)
	{
		case e_print_npc_totalinfo :		break ;
		case e_print_npc_listinfo :			break ;
		case e_print_player_totalInfo :		break ;
		case e_print_hpmp_determination :	break ;
		case e_print_palyer_experience :	break ;
		case e_print_itemInfo :				m_byOption = opNum ;	break ;
		case e_print_dropitem :				m_byOption = opNum ;	break ;
	}
}

void cDebugString::print_npc_totalinfo()
{
	if(!m_bOnNpcTotalInfo) return ;

	char  tempStr[1024] = {0, } ;
	RECT  tempRect = {(WORD)m_vNpcTotal.x, (WORD)m_vNpcTotal.y, 300, 300} ;
	DWORD tempColor = RGB(0, 0, 255) ;
	WORD  wFontIndex = 2 ;

	if( m_pNpc == NULL )
	{
		sprintf(tempStr, "Select npc that you want to view info.") ;
		CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

		return ;
	}

	NPC_TOTALINFO npcTotalInfo ; 
	memset(&npcTotalInfo, 0, sizeof(NPC_TOTALINFO)) ;
	memcpy(&npcTotalInfo, m_pNpc->GetNpcTotalInfo(), sizeof(NPC_TOTALINFO)) ;

	/*
	sprintf(tempStr, "NPC 종류 1 = eMNMICON_WEAPON") ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 12 ;
	sprintf(tempStr, "NPC 종류 2 = eMNMICON_DRESS") ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 12 ;
	sprintf(tempStr, "NPC 종류 3 = eMNMICON_DRESS") ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 12 ;
	sprintf(tempStr, "NPC 종류 4 = eMNMICON_DOCTOR") ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 12 ;
	sprintf(tempStr, "NPC 종류 5 = eMNMICON_BOOK") ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 12 ;
	sprintf(tempStr, "NPC 종류 8 = eMNMICON_MUNPA") ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 12 ;
	sprintf(tempStr, "NPC 종류 10 = eMNMICON_WAREHOUSE") ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 12 ;
	sprintf(tempStr, "NPC 종류 11 = eMNMICON_CASTLEMANAGER") ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 12 ;
	sprintf(tempStr, "NPC 종류 12 = eMNMICON_STORAGE") ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 12 ;
	sprintf(tempStr, "NPC 종류 13 = eMNMICON_STORAGE") ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 12 ;
	sprintf(tempStr, "NPC 종류 15 = eMNMICON_YEONMU") ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 12 ;
	sprintf(tempStr, "NPC 종류 22 = eMNMICON_YEONMU") ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 12 ;
	sprintf(tempStr, "NPC 종류 25 = eMNMICON_STORAGE") ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 12 ;
	sprintf(tempStr, "NPC 종류 30 = eMNMICON_LOGIN") ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 12 ;
	sprintf(tempStr, "NPC 종류 31 = eMNMICON_MAPCHANGE") ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;
	*/
	//tempRect.top += 24 ;
	sprintf(tempStr, "MAP_NUM = %d", npcTotalInfo.MapNum) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 12 ;
	sprintf(tempStr, "NPC_GROUP = %d", npcTotalInfo.Group) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 12 ;
	sprintf(tempStr, "NPC_JOB = %d", npcTotalInfo.NpcJob) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 12 ;
	sprintf(tempStr, "NPC_KIND = %d", npcTotalInfo.NpcKind) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 12 ;
	sprintf(tempStr, "NPC_INDEX = %d", npcTotalInfo.NpcUniqueIdx) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;
}

void cDebugString::print_npc_listinfo()
{
	if(!m_bOnNpcListInfo) return ;

	char  tempStr[1024] = {0, } ;
	RECT  tempRect = {(WORD)m_vNpcList.x, (WORD)m_vNpcList.y, 300, 300} ;
	DWORD tempColor = RGB(255, 0, 0) ;
	WORD  wFontIndex = 2 ;

	if( m_pNpc == NULL )
	{
		sprintf(tempStr, "Select npc that you want to view info.") ;
		CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

		return ;
	}

	NPC_LIST npcListInfo ;
	memset(&npcListInfo, 0, sizeof(NPC_LIST)) ;
	memcpy(&npcListInfo, m_pNpc->GetNpcListInfo(), sizeof(NPC_LIST)) ;

	sprintf(tempStr, "NPC_JOB_KIND = %d", npcListInfo.JobKind) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 12 ;
	sprintf(tempStr, "NPC_MODEL_NUM = %d", npcListInfo.ModelNum) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 12 ;
	sprintf(tempStr, "NPC_NAME = %s", npcListInfo.Name) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 12 ;
	sprintf(tempStr, "NPC_KIND = %d", npcListInfo.NpcKind) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 12 ;
	sprintf(tempStr, "NPC_SCALE = %f", npcListInfo.Scale) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 12 ;
	sprintf(tempStr, "NPC_TALL = %d", npcListInfo.Tall) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;
}

////////////////////////////////////////////
// CHARACTER PART
////////////////////////////////////////////
void cDebugString::print_char_totalInfo()
{
	if(!m_bOnCharTotalInfo) return ;

	char  tempStr[1024] = {0, } ;
	RECT  tempRect = {(WORD)m_vCharTotal.x, (WORD)m_vCharTotal.y, 300, 300} ;
	DWORD tempColor = RGB(255, 0, 0) ;
	WORD  wFontIndex = 2 ;

	if( !m_pPlayer )
	{
		sprintf(tempStr, "Select player that you want to view info.") ;
		CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

		return ;
	}

	CHARACTER_TOTALINFO totalInfo ;
	memset(&totalInfo, 0, sizeof(NPC_LIST)) ;
	m_pPlayer->GetCharacterTotalInfo(&totalInfo) ;

	BYTE JobGrade = 0 ;
	BYTE ClassIndex = 0 ;
	switch(totalInfo.Job[JobGrade])
	{
	case 1 : sprintf(tempStr, "BASE_CLASS = FIGHTER") ; break ;
	case 2 : sprintf(tempStr, "BASE_CLASS = ROGUE") ; break ;
	case 3 : sprintf(tempStr, "BASE_CLASS = MAGE") ; break ;
	}
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	switch(totalInfo.Race)
	{
	case 0 : sprintf(tempStr, "RACE = HUMAN") ; break ;
	case 1 : sprintf(tempStr, "RACE = ELF") ; break ;
	}
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	JobGrade = totalInfo.JobGrade ;
	sprintf(tempStr, "JOB_GRADE = %d", JobGrade) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	if(JobGrade == 1)
	{
		ClassIndex = 1 ;
	}
	else
	{
		ClassIndex = totalInfo.Job[JobGrade-1] ;
	}
	sprintf(tempStr, "CURRENT_CLASS = %d", ClassIndex) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	sprintf(tempStr, "CLASSINDEX = %d%d%d%d", totalInfo.Job[0], totalInfo.Race+1, JobGrade, ClassIndex) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;
}

////////////////////////////////////////////
// HP MP PART
////////////////////////////////////////////
void cDebugString::SetHpMpPoint(BYTE hp, BYTE mp) 
{
	m_bHpPoint = hp ;
	m_bMpPoint = mp ;
}

void cDebugString::print_hpmp_determination() 
{
	if(!m_bOnHPMP) return ;

	char  tempStr[1024] = {0, } ;
	RECT  tempRect = {(WORD)m_vHPMP.x, (WORD)m_vHPMP.y, 300, 300} ;
	DWORD tempColor = RGB(255, 0, 0) ;
	WORD  wFontIndex = 2 ;

	if( !m_pPlayer )
	{
		sprintf(tempStr, "Select player that you want to view info.") ;
		CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

		return ;
	}

	CHARACTER_TOTALINFO totalInfo ;
	memset(&totalInfo, 0, sizeof(NPC_LIST)) ;
	m_pPlayer->GetCharacterTotalInfo(&totalInfo) ;

	HERO_TOTALINFO heroInfo ;
	memset(&heroInfo, 0, sizeof(HERO_TOTALINFO)) ;
	CHero* pHero = ((CHero*)m_pPlayer) ;
	pHero->GetHeroTotalInfo(&heroInfo) ;

	sprintf(tempStr, "HP_POINT = %d", m_bHpPoint) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	sprintf(tempStr, "MP_POINT = %d", m_bMpPoint) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	DWORD info_vit = heroInfo.Vit ;
	DWORD item_vit = DWORD( pHero->GetItemStats().mVitality.mPlus );
	WORD shop_vit = m_pPlayer->GetShopItemStats()->Vit ;
	WORD avat_vit = m_pPlayer->GetAvatarOption()->Vit ;
	DWORD cur_vit  = info_vit + item_vit + shop_vit + avat_vit ;
	sprintf(tempStr, "CUR_VIT(%d) = Info(%d) + item(%d) + ShopItem(%d) + AvatarOption(%d)", cur_vit, info_vit, item_vit, shop_vit, avat_vit) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	DWORD base_vit = info_vit ;
	sprintf(tempStr, "BASE_VIT = %d", base_vit) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	WORD wLevel = m_pPlayer->GetLevel() ;
	double HP_A = ( cur_vit * (10 + (0.05 * ( wLevel* m_bHpPoint))) ) ;
	sprintf(tempStr, "BASE_HP(%f) = (CUR_VIT(%d) * (10 + (0.05 * (LEVEL(%d) * HP_POINT(%d)) ) ) ) ;", HP_A, cur_vit, wLevel, m_bHpPoint) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	double HP_B = (( base_vit + m_bHpPoint ) * 0.5) * (wLevel-1) ;
	sprintf(tempStr, "INCREASE_HP(%f) = ( (BASE_VIT(%d) + HP_POINT(%d)) * 0.5 ) * (LEVEL-1(%d)) ;", HP_B, base_vit, m_bHpPoint, wLevel-1) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	sprintf(tempStr, "DECREASE_HP(%f) is same to INCREASE_HP(%f)", HP_B, HP_B) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	double HP_C = HP_A + HP_B ;
	sprintf(tempStr, "CURRENT_HP(%f) is BASE_HP(%f) + INCREASE_HP(%f).", HP_C, HP_A, HP_B) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	sprintf(tempStr, "-----------------------------------------------------") ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	DWORD info_wis = heroInfo.Wis ;
	DWORD item_wis = DWORD( pHero->GetItemStats().mWisdom.mPlus );
	WORD shop_wis = m_pPlayer->GetShopItemStats()->Wis ;
	WORD avat_wis = m_pPlayer->GetAvatarOption()->Wis ;
	DWORD cur_wis  = info_wis + item_wis + shop_wis + avat_wis ;
	sprintf(tempStr, "CUR_WIS(%d) = Info(%d) + item(%d) + ShopItem(%d) + AvatarOption(%d)", cur_wis, info_wis, item_wis, shop_wis, avat_wis) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	DWORD base_wis = info_wis ;
	sprintf(tempStr, "BASE_WIS = %d", base_wis) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	double MP_A = ( cur_wis * (6 + (0.05 * ( wLevel* m_bMpPoint))) ) ;
	sprintf(tempStr, "BASE_MP(%f) = (CUR_WIS(%d) * (6 + (0.05 * (LEVEL(%d) * MP_POINT(%d)) ) ) ) ;", MP_A, cur_wis, wLevel, m_bMpPoint) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	double MP_B = (( base_wis + m_bMpPoint ) * 0.5) * (wLevel-1) ;
	sprintf(tempStr, "INCRESE_MP(%f) = ( (BASE_WIS(%d) + MP_POINT(%d)) * 0.5 ) * (LEVEL-1(%d)) ;", MP_B, base_wis, m_bMpPoint, wLevel-1) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	sprintf(tempStr, "DECREASE_MP(%f) is same to INCREASE_MP(%f)", MP_B, MP_B) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	double MP_C = MP_A + MP_B ;
	sprintf(tempStr, "CURRENT_MP(%f) is BASE_MP(%f) + INCREASE_MP(%f).", MP_C, MP_A, MP_B) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 28 ;
	sprintf(tempStr, "HP(%f)", HP_C) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	sprintf(tempStr, "MP(%f)", MP_C) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;
}

////////////////////////////////////////////
// EXPERIENCE PART
////////////////////////////////////////////
void cDebugString::SetExpInfo(MSG_EXP_RESULT* pMsg) 
{
	memcpy(&m_Msg, pMsg, sizeof(MSG_EXP_RESULT)) ;

	m_bIsSetInfo = TRUE ;
}

void cDebugString::print_ExpInfo()
{
	if(!m_bOnExperience) return ;

	char  tempStr[1024] = {0, } ;
	RECT  tempRect = {(WORD)m_vExperience.x, (WORD)m_vExperience.y, 300, 300} ;
	DWORD tempColor = RGB(255, 0, 0) ;
	WORD  wFontIndex = 2 ;

	if( !m_bIsSetInfo )
	{
		sprintf(tempStr, "Let's go to monster hunting!!") ;
		CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

		return ;
	}

	switch(m_Msg.expKind)
	{
	case 0 :
		{
			sprintf(tempStr, "[[ YOU GOT A SOLOING EXP POINT. ]]") ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 28 ;
			sprintf(tempStr, "Low Lv 6~10 is 50%% of exp.") ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Low Lv 11~15 is 25%% of exp.") ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Low Lv 16 is 1.") ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			if(HERO)
			{
				tempRect.top += 28 ;
				sprintf(tempStr, "YOUR_LEVEL = %d", HERO->GetLevel()) ;
				CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;
			}

			tempRect.top += 14 ;
			sprintf(tempStr, "MONSTER_LEVEL = %d", m_Msg.monsterLevel) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "MONSTER_EXP = %d", m_Msg.monsterExp) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "SERVER_CALC_EXP = %d", m_Msg.sendExp) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 28 ;
			sprintf(tempStr, "50%% exp is %f", m_Msg.monsterExp*0.5) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "25%% exp is %f", m_Msg.monsterExp*0.25) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			if(HERO)
			{
				float a = (float)(HERO->GetLevel()+5) ;
				float b = 1.95f;
				float result = (float)pow(a, b) ;
				sprintf(tempStr, "More than 6 level's Exp(%.1f) = ((YourLevel(%.1f) + 5)^1.95) + 15.", GetStatRound((DWORD)(result+15)), HERO->GetLevel()) ;
				CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;
			}
		}
		break ;

	case 1 :
		{
			sprintf(tempStr, "[[ YOU GOT A STEALED EXP POINT. ]]") ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			if(m_Msg.bFirstKiller)
			{
				tempRect.top += 14 ;
				sprintf(tempStr, "YOU ARE FIRST KILLER.") ;
				CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;
			}
			else
			{
				tempRect.top += 14 ;
				sprintf(tempStr, "YOU ARE NOT FIRST KILLER.") ;
				CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;
			}

			tempRect.top += 28 ;
			sprintf(tempStr, "Biggest level is %d", m_Msg.bigLevel) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			

			LEVELTYPE KillerLevel  = m_Msg.bigLevel ;
			LEVELTYPE MonsterLevel = m_Msg.monsterLevel ;
			LEVELTYPE levelGap = 0 ;

			EXPTYPE MonsterExp = m_Msg.monsterExp ;

			DWORD PenaltyExp = 0 ;

			if( MonsterLevel > KillerLevel )
			{
				levelGap = MonsterLevel - KillerLevel ;
			}
			else
			{
				levelGap = KillerLevel - MonsterLevel ;
			}

			if( MonsterLevel > KillerLevel + 5 )
			{
				DWORD dwValueA =  ( KillerLevel + 5 ) * 195 ;
				DWORD dwValueB = dwValueA / 100 ;

				PenaltyExp = dwValueB + 15 ;
			}
			else if( MonsterLevel < KillerLevel )
			{
				if( levelGap >= 6 && levelGap <= 10 )
				{
					PenaltyExp = (DWORD)(MonsterExp * 0.5) ;
				}
				else if( levelGap >= 11 && levelGap <= 15 )
				{
					PenaltyExp = (DWORD)(MonsterExp * 0.25) ;
				}
				else if( levelGap > 15 )
				{
					PenaltyExp = 1 ;
				}
			}	

			tempRect.top += 14 ;
			sprintf(tempStr, "Monster level is %d", MonsterLevel) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Monster exp is %d", MonsterExp) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Penalty exp is %d", PenaltyExp) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			if(m_Msg.bFirstKiller)
			{
				tempRect.top += 28 ;
				DWORD myDamage = m_Msg.dwMyDamage ;
				float fExp = (float)(( (myDamage + (myDamage*0.3)) / m_Msg.dwTotalDamage ) * (MonsterExp-PenaltyExp)) ;
				sprintf(tempStr, "YOU_EXP(%f) = ( (YourDamage(%d) + (YourDamage*0.3)(%f)) / TotalDamage(%d) ) * MonsterExp(%d) ;", 
								 fExp, myDamage, myDamage*0.3, m_Msg.dwTotalDamage, (MonsterExp - PenaltyExp)) ;
				CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;
			}
			else
			{
				tempRect.top += 28 ;
				DWORD firstDamage = m_Msg.dwFirstDamage ;
				DWORD myDamage    = m_Msg.dwMyDamage ;
				BYTE killerCount  = m_Msg.killerCount ;
				float fPriority   = (float)((firstDamage*0.3) / (killerCount-1)) ;

				float fExp = 0.0f ;

				if(myDamage > fPriority)
				{
					fExp = (float)(((myDamage - fPriority) / m_Msg.dwTotalDamage ) * (MonsterExp-PenaltyExp)) ;
				}
				sprintf(tempStr, "YOU_EXP(%f) = ((YourDamage(%d) - ((FirstDamage*0.3(%f)) / (killerCount-1(%d))) / TotalDamage(%d)) * MonsterExp(%d) ;", 
								 fExp, myDamage, firstDamage*0.3, killerCount-1, m_Msg.dwTotalDamage, (MonsterExp-PenaltyExp)) ;
				CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;
			}

			tempRect.top += 14 ;
			sprintf(tempStr, "SERVER_CALC_EXP = %d", m_Msg.sendExp) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;
		}
		break ;

		case 2 :
			{
				EXPTYPE MonsterExp = m_Msg.monsterExp ;

				float applyRate = 0.0 ;
				switch( m_Msg.partyMemberCount )
				{
				case 2 : applyRate = 0.4f ; break ;
				case 3 : applyRate = 0.6f ; break ;
				case 4 : applyRate = 0.7f ; break ;
				case 5 : applyRate = 0.8f ; break ;
				case 6 : applyRate = 0.9f ; break ;
				case 7 : applyRate = 1 ; break ;
				}

				float fExpA = (float)m_Msg.playerLevel/(float)m_Msg.partyTotalLevel ;
				float fExpB = (float)(MonsterExp*(1+applyRate)) ;
				float fExp  = (float)(fExpA * fExpB) ;

				sprintf(tempStr, "[[ YOU GOT A PARTY EXP POINT. ]]") ;
				CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

				tempRect.top += 28 ;
				sprintf(tempStr, "Monster exp is %d", MonsterExp) ;
				CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

				tempRect.top += 14 ;
				sprintf(tempStr, "Monster level is %d", m_Msg.monsterLevel) ;
				CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

				tempRect.top += 14 ;
				sprintf(tempStr, "Biggest level is %d", m_Msg.bigLevel) ;
				CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

				tempRect.top += 14 ;
				sprintf(tempStr, "Your level is %d", m_Msg.playerLevel) ;
				CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

				tempRect.top += 14 ;
				sprintf(tempStr, "Total level of members is %d", m_Msg.partyTotalLevel) ;
				CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

				tempRect.top += 14 ;
				sprintf(tempStr, "Total damage is %d", m_Msg.dwTotalDamage) ;
				CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

				tempRect.top += 14 ;
				sprintf(tempStr, "Loged in member count is %d", m_Msg.partyMemberCount) ;
				CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

				tempRect.top += 14 ;
				sprintf(tempStr, "Bonus exp of party is %f", MonsterExp*applyRate) ;
				CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

				tempRect.top += 28 ;
				sprintf(tempStr, "YOU_EXP(%f) = (YourLevel/TotalLevel(%f)) * (MonsterExt*(1+PartyBonus)(%f)) ;", fExp, fExpA, fExpB) ;
				CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

				tempRect.top += 14 ;
				sprintf(tempStr, "SERVER_CALC_EXP = %d", m_Msg.sendExp) ;
				CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;
			}
			break ;

		case 3 :
			{
				sprintf(tempStr, "[[ YOU ARE NOT IN AREA OF PARTY EXP. ]]") ;
				CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;
			}
			break ;
	}
}

////////////////////////////////////////////
// ITEM INFO
////////////////////////////////////////////
void cDebugString::SetItemInfo(ITEM_INFO* pItemInfo) 
{
	m_pItemInfo = pItemInfo ;
}

void cDebugString::print_ItemInfo()
{
	if(!m_bOnItemInfo) return ;

	char  tempStr[1024] = {0, } ;
	RECT  tempRect = {(WORD)m_vItemInfo.x, (WORD)m_vItemInfo.y, 300, 300} ;
	DWORD tempColor = RGB(255, 0, 0) ;
	WORD  wFontIndex = 2 ;

	if( !m_pItemInfo )
	{
		sprintf(tempStr, "Choose item that you want to view info.") ;
		CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

		return ;
	}

	sprintf(tempStr, "Select option to type command 'SD_OPTION'.") ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	sprintf(tempStr, "Page 0 ~ 6. EX : /SD_OPTION 6 0") ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	switch(m_byOption)
	{
	case 0 :
		{
			tempRect.top += 14 ;
			sprintf(tempStr, "Item_Index = %d", m_pItemInfo->ItemIdx ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Item_name = %s", m_pItemInfo->ItemName ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Tooltip_Index = %d", m_pItemInfo->ItemTooltipIdx ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Image2DNum = %d", m_pItemInfo->Image2DNum ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Part3DType = %d", m_pItemInfo->Part3DType ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Part3DModelNum = %d", m_pItemInfo->Part3DModelNum ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;
		}
		break ;

	case 1 :
		{
			tempRect.top += 14 ;
			sprintf(tempStr, "Shop = %d", m_pItemInfo->Shop ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Stack = %d", m_pItemInfo->Stack ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Trade = %d", m_pItemInfo->Trade ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Deposit = %d", m_pItemInfo->Deposit ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Sell = %s", m_pItemInfo->Sell ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Decompose = %d", m_pItemInfo->Decompose ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Repair = %d", m_pItemInfo->Repair ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Enchant = %d", m_pItemInfo->IsEnchant ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Improvement = %d", m_pItemInfo->Improvement ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;
		}
		break ;

	case 2 :
		{
			tempRect.top += 14 ;
			sprintf(tempStr, "DecomposeIdx = %d", m_pItemInfo->DecomposeIdx ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Time = %d", m_pItemInfo->Time ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "BuyPrice = %d", m_pItemInfo->BuyPrice ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "SellPrice = %d", m_pItemInfo->SellPrice ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Grade = %d", m_pItemInfo->Grade ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Durability = %d", m_pItemInfo->Durability ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;
		}
		break ;

	case 3 :
		{
			tempRect.top += 14 ;
			sprintf(tempStr, "PhysicAttack = %d", m_pItemInfo->PhysicAttack ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "MagicAttack = %d", m_pItemInfo->MagicAttack ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "PhysicDefense = %d", m_pItemInfo->PhysicDefense ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "MagicDefense = %d", m_pItemInfo->MagicDefense ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;
		}
		break ;

	case 4 :
		{
			tempRect.top += 14 ;
			sprintf(tempStr, "Category = %d", m_pItemInfo->Category ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "EquipType = %d", m_pItemInfo->EquipType ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "EquipSlot = %d", m_pItemInfo->EquipSlot ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "WeaponAnimation = %d", m_pItemInfo->WeaponAnimation ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "WeaponType = %d", m_pItemInfo->WeaponType ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "ArmorType = %d", m_pItemInfo->ArmorType ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "AccessaryType = %d", m_pItemInfo->AccessaryType ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;
		}
		break ;

	case 5 :
		{
			tempRect.top += 14 ;
			sprintf(tempStr, "ImprovementStr = %d", m_pItemInfo->ImprovementStr ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "ImprovementDex = %d", m_pItemInfo->ImprovementDex ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "ImprovementVit = %d", m_pItemInfo->ImprovementVit ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "ImprovementInt = %d", m_pItemInfo->ImprovementInt ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "ImprovementWis = %d", m_pItemInfo->ImprovementWis ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "ImprovementLife = %d", m_pItemInfo->ImprovementLife ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "ImprovementMana = %d", m_pItemInfo->ImprovementMana ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "ImprovementSkill = %d", m_pItemInfo->ImprovementSkill ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;
		}
		break ;

	case 6 :
		{
			tempRect.top += 14 ;
			sprintf(tempStr, "Skill = %d", m_pItemInfo->Skill ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "SupplyType = %d", m_pItemInfo->SupplyType ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "SupplyValue = %d", m_pItemInfo->SupplyValue ) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;
		}
		break ;
	}
}

////////////////////////////////////////////
// DROPITEM INFO
////////////////////////////////////////////
void cDebugString::Set_DropMoney(MSG_DROPMONEY_RESULT* pMoney)
{
	memcpy(&m_DropMoney, pMoney, sizeof(MSG_DROPMONEY_RESULT)) ;

	m_bInitialized = TRUE ;
}

void cDebugString::Set_DropItem(MSG_DROPITEM_RESULT* pItem)
{
	memcpy(&m_DropItem, pItem, sizeof(MSG_DROPITEM_RESULT)) ;
}

void cDebugString::print_DropItem()
{
	if(!m_bOnDropItem) return ;

	char  tempStr[1024] = {0, } ;
	RECT  tempRect = {(WORD)m_vDropItem.x, (WORD)m_vDropItem.y, 300, 300} ;
	DWORD tempColor = RGB(255, 0, 0) ;
	WORD  wFontIndex = 2 ;

	if( !m_bInitialized )
	{
		sprintf(tempStr, "Hunting monster if you want to view drop item info.") ;
		CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

		tempRect.top += 14 ;
		sprintf(tempStr, "And choice info.") ;
		CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

		tempRect.top += 14 ;
		sprintf(tempStr, "EX) /sd_option 7 1") ;
		CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

		tempRect.top += 14 ;
		sprintf(tempStr, "0. Drop money.") ;
		CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

		tempRect.top += 14 ;
		sprintf(tempStr, "1~5. Drop item.") ;
		CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

		return ;
	}

	sprintf(tempStr, "[[ PRINT_MONEY_RESULT ]]") ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 20 ;	
	sprintf(tempStr, "Monster_MaxMoney = %d", m_DropMoney.dwMonstersMoney) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	sprintf(tempStr, "Money_DropRate = %d", m_DropMoney.dwMoneyDropRate) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	sprintf(tempStr, "Money_RandomRate = %d", m_DropMoney.wRandomRate) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	sprintf(tempStr, "Pickup_Percent = %.0f%%", m_DropMoney.fPickupPercent) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	sprintf(tempStr, "Pickup_Money(%d) = (Monster_MaxMoney(%d) * Pickup_Percent(%.0f%%)) ;", 
			m_DropMoney.dwResultMoney, m_DropMoney.dwMonstersMoney, m_DropMoney.fPickupPercent) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	tempRect.top += 14 ;
	sprintf(tempStr, "Pickup_Money = %d", m_DropMoney.dwResultMoney) ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	if( m_DropItem.bParty )
	{
		sprintf(tempStr, "[[ PRINT_DROP_ITEM(%d). Drop to person. ]]", m_byOption) ;
	}
	else
	{
		sprintf(tempStr, "[[ PRINT_DROP_ITEM(%d). Drop to party. ]]", m_byOption) ;
	}

	tempRect.top += 30 ;
	CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

	for( BYTE count = 0 ; count < 5; ++count )
	{
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( m_DropItem.dwItemIdx[count] ) ;
		CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(m_DropItem.dwPlayerID[count]);

		if(pInfo)
		{
			tempRect.top += 20 ;
			sprintf(tempStr, "Item_Name = %s", pInfo->ItemName) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Item_Index = %d", m_DropItem.dwItemIdx[count]) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Item_DropRate = %d", m_DropItem.dwItemDropRate[count]) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Item_DropPercent = %d", m_DropItem.dwItemDropRate[count]) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Item_Random%% = %d", m_DropItem.wRandomRate[count]) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Drop_Count = %d", m_DropItem.byDropCount[count]) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			if(pPlayer)
			{
				sprintf(tempStr, "Player_ID = %s", pPlayer->GetObjectName()) ;
			}
			else
			{
				sprintf(tempStr, "There is no receiver.") ;
			}
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;
		}
		else
		{
			tempRect.top += 20 ;
			sprintf(tempStr, "There is no item to drop. Item(%d)", count) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;

			tempRect.top += 14 ;
			sprintf(tempStr, "Item_Index = %d", m_DropItem.dwItemIdx[count]) ;
			CFONT_OBJ->RenderFont(wFontIndex, tempStr, strlen(tempStr), &tempRect, tempColor) ;
		}
	}
}
//#endif