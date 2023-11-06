// CheatMsgParser.h: interface for the CCheatMsgParser class.
//
//////////////////////////////////////////////////////////////////////


//#ifdef _CHEATENABLE_ //KES CHEAT!!!

#if !defined(AFX_CHEATMSGPARSER_H__F7D74F56_32EC_4F80_B767_348E65ACC54C__INCLUDED_)
#define AFX_CHEATMSGPARSER_H__F7D74F56_32EC_4F80_B767_348E65ACC54C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
struct EVENTITEM
{
	char	strName[20];
};
*/

#include "Object.h"

#define CHEATMGR USINGTON(CCheatMsgParser)
class CCheatMsgParser  
{
	BOOL m_bCheatEnable;
	CObject* m_pTargetObj;
//	EVENTITEM	m_eventitem[10][20];
//	WORD		m_itemCount[10];
//	BOOL m_bEvent;

	//SW051011 ·¹¾î/°­È­ Ä¡Æ®
	//BOOL	m_bItCantBeRare;
public:
	enum{ eKindOption_None=0, eKindOption_Reinforce=1 /*, eKindOption_Rare=2,*/ };
	WORD	m_wItemOptionKind;
	DWORD	m_wTargetItemIdx;
	POSTYPE	m_ItemAbsPos;
	WORD	m_OptionCount;
	//ITEM_OPTION	m_OptionInfo;
	BOOL	m_bReadyToOptionCheat;

//	//MAKESINGLETON(CCheatMsgParser);

	CCheatMsgParser();
	virtual ~CCheatMsgParser();

	void NetworkMsgParse(BYTE Category,BYTE Protocol,MSGBASE* pMsg);

	BOOL IsCheatEnable() { return m_bCheatEnable; }
	void SetCheatEnable( BOOL bCheat ) { m_bCheatEnable = bCheat; }

	void SetCheatTargetObject(CObject* pObj)	{	m_pTargetObj = pObj;	}
	CObject* GetCheatTargetObject()	{ return m_pTargetObj;	}
	// ?ÃŒÂºÂ¥Ã†Â®Â·ÃŽ Ã‡Ã”Â¼Ã¶ ÃƒÃŸÂ±Ã¢
//	void LoadItem();
//	void EventMonsterRegen(WORD type, WORD count = 30);
//	WORD ExtractItem(WORD idx);
//	VECTOR3 GetRegenPos();

	// 071128 LYW --- CheatMsgParser : ³×Æ®¿öÅ© ¸Þ½ÃÁö ÆÄ¼­ ºÎºÐ Ã³¸® ÇÔ¼öÈ­ ÀÛ¾÷.
#ifdef _CHEATENABLE_
	void Cheat_ChangeMap_Ack( void* pMsg ) ;
	//void Cheat_ChangeMap_Nack( void* pMsg ) ;
	void Cheat_WhereIs_Ack( void* pMsg ) ;
	void Cheat_WhereIs_MapServer_Ack( void* pMsg ) ;
	void Cheat_WhereIs_Nack( void* pMsg ) ;
	void Cheat_BlockCharacter_Ack( void* pMsg ) ;
	void Cheat_BlockCharacter_Nack( void* pMsg ) ;
	void Cheat_BanCharacter_Ack( void* pMsg ) ;
	//void Cheat_BanCharacter_Nack( void* pMsg ) ;
	void Cheat_Item_Ack( void* pMsg ) ;
	void Cheat_AgentCheck_Ack( void* pMsg ) ;
#ifdef _DEBUG
	void Cheat_HPMP_Determination_Ack( void* pMsg ) ;
	void Cheat_HPMP_Determination_Nack( void* pMsg ) ;
	void Cheat_Experience_Notice( void* pMsg ) ;
	void Cheat_DropMoney_Info_Ack( void* pMsg ) ;
	void Cheat_DropItem_Info_Ack( void* pMsg ) ;
#endif // _DEBUG

#endif	//_CHEATENABLE_
	void Cheat_Move_Ack( void* pMsg ) ;
	void Cheat_Hide_Ack( void* pMsg ) ;		
	
#ifdef _GMTOOL_
	void Cheat_GM_Login_Ack( void* pMsg ) ;
	void Cheat_GM_Login_Nack( void* pMsg ) ;
	void Cheat_PartyInfo_Ack( void* pMsg ) ;
	void Cheat_EventMap_Result( void* pMsg ) ;
	void Cheat_EventNotify_On( void* pMsg ) ;
	void Cheat_EventNotify_Off( void* pMsg ) ;
	void Cheat_NpcHide_Ack( void* pMsg ) ;
	void Cheat_Default_Stats_Ack( void* pMsg ) ;
	void Cheat_Default_Stats_Nack( void* pMsg ) ;	
	void Cheat_Default_Skill_Ack( void* pMsg ) ;
	void Cheat_Default_Skill_Nack( void* pMsg ) ;
	void Cheat_PvP_Damage_Rate( void* pMsg ) ;
#endif // _GMTOOL_
};

#ifdef _CHEATENABLE_
BOOL CheatFunc(char* cheat);

// 070427 LYW --- CheatMsgParser : Add for test.
//#ifdef _DEBUG
BOOL StartDebug(int command, int onoff, int Xpos, int Ypos) ;
//#endif //_DEBUG
#endif

EXTERNGLOBALTON(CCheatMsgParser)

#endif // !defined(AFX_CHEATMSGPARSER_H__F7D74F56_32EC_4F80_B767_348E65ACC54C__INCLUDED_)
//#endif //_CHEATENABLE_	//KES CHEAT!!!