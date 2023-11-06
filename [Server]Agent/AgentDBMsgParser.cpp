#include "stdafx.h"
#include "CommonDBMsgParser.h"
#include "AgentDBMsgParser.h"
#include "DataBase.h"
#include "UserTable.h"
#include "ServerTable.h"
#include "Console.h"
#include "Network.h"
#include "MsgTable.h"
#include "ServerSystem.h"
#include "GMPowerList.h"

#include "JackpotManager_Agent.h"

// desc_hseos_∆–π–∏Æ01
// S ∆–π–∏Æ √ﬂ∞° added by hseos 2007.07.09
#include "../hseos/Family/SHFamilyManager.h"
#include "../hseos/Farm/SHFarmManager.h"
// E ∆–π–∏Æ √ﬂ∞° added by hseos 2007.07.09


//---KES PUNISH
#include "PunishManager.h"
//-------------

// 080507 LYW --- AgentDBMsgParser : √§∆√πÊ ∏≈¥œ¡Æ «Ï¥ı ∆˜«‘.
#include "ChatRoomMgr.h"

#ifdef _HACK_SHIELD_
#include "HackShieldManager.h"
#endif
#ifdef _NPROTECT_
#include "NProtectManager.h"
#endif
#include ".\giftmanager.h"

//MSG_CHAT g_WisperTemp;
//MSG_CHAT g_MunpaTemp;
//MSG_CHAT g_PartyTemp;

extern int g_nServerSetNum;

//-----------------------------------------------------------------------
// DB√Ñ√µ¬∏¬Æ ¬∏¬Æ√Ö√è √á√î¬º√∂¬∞¬° ¬¥√É¬æ√Æ¬≥¬Ø¬∂¬ß¬∏¬∂¬¥√ô √É√ü¬∞¬°
// enum Query ?√á ¬º√∏¬º¬≠¬∞¬° ¬π√ù¬µ√•¬Ω√É!!!! ?√è√Ñ¬°√á√ò¬æ√ü √á√ë¬¥√ô.
DBMsgFunc g_DBMsgFunc[MaxQuery] =
{
	NULL,
	RUserIDXSendAndCharacterBaseInfo,	// √Ñ¬≥¬∏¬Ø√Ö√ç ¬∏¬Æ¬Ω¬∫√Ü¬Æ Query
	RCreateCharacter,
	NULL,
	RDeleteCharacter,
	RCharacterNameCheck,
	NULL,
	NULL,
	RSearchWhisperUserAndSend, /// √Ñ¬≥¬∏¬Ø¬∏√≠?¬∏¬∑√é √Å¬¢¬º√ì¬ø¬©¬∫√é¬π√ó ¬∏√ä¬π√∏√à¬£ ¬æ√≤¬æ√Æ¬ø?¬±√¢
	NULL,							//SavePoint 
	NULL,							/// ¬∏√ä¬ø¬°¬º¬≠ ¬≥¬™¬∞¬°¬∏√© ¬Ω√á√á√†√á√ë¬¥√ô
	NULL,							/// ¬∏√ä¬º¬≠¬π√∂ √Å¬æ¬∑√°¬Ω√É ¬Ω√á√á√†
	RFriendIsValidTarget,		//FriendGetUserIDXbyName
	RFriendAddFriend,
	RFriendIsValidTarget,
	RFriendDelFriend,
	RFriendDelFriendID,
	NULL, //√Ñ¬£¬±¬∏¬∏¬Æ¬Ω¬∫√Ü¬Æ ¬ª√®√Å¬¶(√Ñ¬≥¬∏¬Ø√Ö√ç ¬ª√®√Å¬¶¬Ω√É)
	RFriendNotifyLogintoClient,
	RFriendGetFriendList,
	RFriendGetLoginFriends,
	RNoteIsNewNote,
	NULL,
	RNoteList,
	RNoteDelete,
	RWantedDelete,
	NULL,
//For GM-Tool	
	RGM_BanCharacter,
	RGM_UpdateUserLevel,			/// eGM_UpdateUserLevel,
	RGM_WhereIsCharacter,
	RGM_Login,
	RGM_GetGMPowerList,
//	
	NULL,
	NULL,//RBillingRemainTimeUpdate,
	NULL,	//eBilling_UpdateAll	
	NULL,							// 050106 event
	RCheckGuildMasterLogin,			// checkguildmasterlogin
	NULL,							// 050203 event

	NULL,							// for billing
	NULL,

	NULL,							// 050322 event

	RJackpotLoadTotalMoney,			// eJackpot_TotalMoney_Load
	RCheckGuildFieldWarMoney,		// check guildfieldwarmoney
	RAddGuildFieldWarMoney,			// addd guildfieldwarmoney
	
	RChaseFindUser,					// eChaseFindUser

	RGetCharacterSlotCount,			// eJP_CharacterSlot

	NULL,							// 051108 event
	NULL,							// eEventItemUse2

	RGM_UpdateUserState,

	NULL,							// eLogGMToolUse

	NULL,

	NULL,
	NULL,
	// desc_hseos_º∫∫∞º±≈√01
	// S º∫∫∞º±≈√ √ﬂ∞° added by hseos 2007.06.15
	NULL,							// eInitMonstermeterInfoOfDB
	RGetUserSexKind,				// eGetUserSexKind
	// E º∫∫∞º±≈√ √ﬂ∞° added by hseos 2007.06.15

	// desc_hseos_∆–π–∏Æ01
	// S ∆–π–∏Æ √ﬂ∞° added by hseos 2007.07.09	2007.07.10	2007.07.14	2007.10.11
	RFamily_LoadInfo,					// eFamily_LoadInfo
	RFamily_SaveInfo,					// eFamily_SaveInfo
	RFamily_Member_LoadInfo,			// eFamily_Member_LoadInfo
	RFamily_Member_LoadExInfo01,		// eFamily_Member_LoadExInfo01,
	NULL,								// eFamily_Member_SaveInfo
	RFamily_CheckName,					// eFamily_CheckName
	RFamily_Leave_LoadInfo,				// eFamily_Leave_LoadInfo
	NULL,								// eFamily_Leave_SaveInfo
	// E ∆–π–∏Æ √ﬂ∞° added by hseos 2007.07.09	2007.07.10	2007.07.14	2007.10.11
	NULL,								// eChange_Character_Name

	// desc_hseos_≥Û¿ÂΩ√Ω∫≈€_01
	// S ≥Û¿ÂΩ√Ω∫≈€ √ﬂ∞° added by hseos 2008.01.22
	RFarm_SendNote_DelFarm,				// eFarm_SendNote_DelFarm
	// E ≥Û¿ÂΩ√Ω∫≈€ √ﬂ∞° added by hseos 2008.01.22

//---KES PUNISH
	NULL,								//ePunishList_Add,	//---¡¶¿Á∏Ò∑œ
	RPunishListLoad,					//ePunishList_Load,	//---¡¶¿Á∏Ò∑œ
	RPunishCountAdd,					//ePunishCount_Add, //---∞…∏∞ »∏ºˆ √ﬂ∞°

	RGiftEvent,							//eGiftEvent,
//-------------

};	

// √Ñ¬¥¬∏¬Ø√Ö√ç ¬∏¬Æ¬Ω¬∫√Ü¬Æ ¬∞¬°√Å¬Æ¬ø?¬¥√Ç DB√Ñ√µ¬∏¬Æ
void UserIDXSendAndCharacterBaseInfo(DWORD UserIDX, DWORD AuthKey, DWORD dwConnectionIndex)
{
	g_DB.FreeQuery(eCharacterBaseQuery, dwConnectionIndex, "EXEC MP_CHARACTER_SelectByUserIDX %d, %d", UserIDX, AuthKey);
}

void CheatLog(DWORD CharacterIDX,DWORD CheatKind)
{
	char txt[256];
	sprintf(txt,"INSERT TB_LogHacking (Character_idx,CheatKind,LogDate) values(%d,%d,getdate())",
				CharacterIDX,CheatKind);
	g_DB.LogQuery(eQueryType_FreeQuery,0,0,txt);
}

void CreateCharacter(CHARACTERMAKEINFO* pMChar, WORD ServerNo, DWORD dwConnectionIndex)
{
	CHARACTERMAKEINFO* pMsg = pMChar;
	char txt[512];

	// 070406 LYW --- AgentDBMsgParser : Distribute base state when the new character created.
	/*
	BYTE StrArray[6] = {16, 13, 12, 11, 8, 7} ;
	BYTE DexArray[6] = {10, 12, 14, 15, 7, 8} ;
	BYTE VitArray[6] = {14, 13, 12, 11, 11, 9} ;
	BYTE IntArray[6] = {6, 7, 7, 8, 13, 14} ;
	BYTE WisArray[6] = {7, 8, 8, 8, 14, 15} ;
	*/

	BYTE StrArray[2][3] = { {16, 12,  8}, { 13, 11,  7} } ;
	BYTE DexArray[2][3] = { {10, 14,  7}, { 12, 15,  8} } ;
	BYTE VitArray[2][3] = { {14, 12, 11}, { 13, 11,  9} } ;
	BYTE IntArray[2][3] = { { 6,  7, 13}, {  7,  8, 14} } ;
	BYTE WisArray[2][3] = { { 7,  8, 14}, {  8,  8, 15} } ;;

	BYTE byRacial  = pMsg->RaceType ;
	BYTE byJopType = pMsg->JobType -1 ;

	BYTE Str = StrArray[byRacial][byJopType] * 5 ;
	BYTE Dex = DexArray[byRacial][byJopType] * 5 ;
	BYTE Vit = VitArray[byRacial][byJopType] * 5 ;
	BYTE Int = IntArray[byRacial][byJopType] * 5 ;
	BYTE Wis = WisArray[byRacial][byJopType] * 5 ;


	DWORD item[3][2];

	item[0][0] = 11000001;
	item[0][1] = 12000032;

	item[1][0] = 11000187;
	item[1][1] = 12000032;

	item[2][0] = 11000249;
	item[2][1] = 12000001;

	char ip[16];
	WORD port;
	g_Network.GetUserAddress( dwConnectionIndex, ip, &port );

	int LoginPoint = 2019;
	BYTE bStartMap = 19;


	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, \'%s\', %d, %d, %d, %d, %d, %d, %d, %d, \'%s\', %u, %u", 
		"MP_CHARACTER_CreateCharacter", pMsg->UserID, Str, Dex, Vit, Int, Wis, 
		pMsg->Name, pMsg->FaceType, pMsg->HairType, bStartMap, pMsg->SexType,
		pMsg->RaceType, pMsg->JobType, LoginPoint, ServerNo, ip, item[byJopType%3][0], item[byJopType%3][1]);
		
	if(g_DB.Query(eQueryType_FreeQuery, eCreateCharacter, dwConnectionIndex, txt) == FALSE)
	{
	}
}

void CharacterNameCheck(char* pName, DWORD dwConnectionIndex)
{
	g_DB.FreeQuery(eNewCharacterCheckName, dwConnectionIndex, "EXEC %s \'%s\'", "MP_CHARACTER_NameCheck", pName);
}

void LoginCheckDelete(DWORD UserID)//, DWORD dwConnectionIndex)
{
//	g_DB.FreeQuery(eLoginCheckDelete, 0, "EXEC %s %d", "MP_CHARACTER_LoginDelete", UserID);

	static char txt[128];
//	sprintf(txt, "EXEC %s %d", "Up_Member_CheckOut", UserID);
	
	sprintf(txt, "EXEC %s %d", "Up_GameLogOut", UserID);
	
	g_DB.LoginQuery(101, eLoginCheckDelete, 0, txt);

	//update BillingTime;

}

void SetUserCurAndMaxCount(DWORD CurCount, DWORD MaxCount)
{
	// desc_hseos_«¡∑ŒΩ√¿˙¡§∏Æ01
	// S «¡∑ŒΩ√¿˙¡§∏Æ √ﬂ∞° added by hseos 2007.06.21
	return;
	// E «¡∑ŒΩ√¿˙¡§∏Æ √ﬂ∞° added by hseos 2007.06.21
	g_DB.FreeQuery(eSetCurMaxCount, 0, "EXEC %s %d, %d", "SetUserLogCount", CurCount, MaxCount);
}
void DeleteCharacter(DWORD dwPlayerID, WORD ServerNo, DWORD dwConnectionIndex)
{
	USERINFO* pinfo = g_pUserTable->FindUser(dwConnectionIndex);
	if(!pinfo)
		return;
	CHARSELECTINFO * SelectInfoArray = (CHARSELECTINFO*)pinfo->SelectInfoArray;
	
	for(int i = 0; i < MAX_CHARACTER_NUM; i++)
	{
		if(SelectInfoArray[i].dwCharacterID == dwPlayerID)
			break;
		if(i == MAX_CHARACTER_NUM - 1)	// ?¬Ø?√∫¬∞¬° ¬∞¬°√Å√∂¬∞√≠ ?√ñ¬¥√Ç √Ñ¬≥¬∏¬Ø¬∞√∫ ¬¥√ô¬∏¬• √Ñ¬≥¬∏¬Ø?√å ¬º¬±√Ö√É¬µ√ä
		{
			// ¬ø¬¨¬∞√°¬µ√à √Ñ¬≥¬∏¬Ø¬∞√∫ √Å√∂¬ø√Ø√Ñ¬≥¬∏¬Ø?√å ?√è√Ñ¬°√á√è√Å√∂ ¬æ√ä¬Ω?¬¥√è¬¥√ô
			return;
		}
	}

	char ip[16];
	WORD port;
	g_Network.GetUserAddress( dwConnectionIndex, ip, &port );

	//√Ü√Ñ√Ü¬º¬≥¬™ ¬π¬Æ√Ü√Ñ?√•?√å¬∏√©  ¬ø¬°¬∑¬Ø√Ñ√ö¬µ√• ¬∏¬Æ√Ö√è
	g_DB.FreeQuery(eDeleteCharacter, dwConnectionIndex, "EXEC %s %d, %d, \'%s\'", "MP_CHARACTER_DeleteCharacter", dwPlayerID, ServerNo, ip );
}

void SearchWhisperUserAndSend( DWORD dwPlayerID, char* CharacterName, DWORD dwKey )
{
	g_DB.FreeQuery(eLoginMapInfoSearchForName, dwKey, "EXEC %s \'%s\', %d", "MP_LoginCharacterSearchForName", CharacterName, dwPlayerID );
}

void SaveMapChangePointUpdate(DWORD CharacterIDX, WORD MapChangePoint_Idx)
{
	g_DB.FreeQuery(eSavePoint, CharacterIDX, "EXEC  %s %d, %d", "MP_CHARACTER_MapchangePointUpdate", CharacterIDX, MapChangePoint_Idx);
	
}

void UnRegistLoginMapInfo(DWORD CharacterIDX)
{
	g_DB.FreeQuery(eUnRegistLoginMapInfo, CharacterIDX, "EXEC %s %d", "MP_LoginMapInfo_UnRegist", CharacterIDX);
}
/*
void MapUserUnRegistLoginMapInfo(WORD MapPort)
{
	g_DB.FreeQuery(eMapUserUnRegistMapInfo, 0, "EXEC %s %d", "MP_LoginMapInfo_MapUserUnRegist", MapPort);
}
*/
void FriendGetUserIDXbyName(DWORD CharacterIDX, char* TargetName)
{
	g_DB.FreeQuery(eFriendGetTargetMemeberIDX, CharacterIDX, "EXEC %s \'%s\', %u", "MP_FRIEND_GetTargetIDX", TargetName, CharacterIDX);
}

void FriendAddFriend(DWORD CharacterIDX, DWORD TargetID) //CharacterIDX : ¬Ω√Ö√É¬ª?√é, TargetID : ¬Ω√Ç¬≥¬´?√é
{
	g_DB.FreeQuery(eFriendAdd, CharacterIDX, "EXEC %s %u, %u", "MP_FRIEND_AddFriend", CharacterIDX, TargetID);
}

void FriendIsValidTarget(DWORD CharacterIDX, DWORD TargetID, char* FromName)
{
	g_DB.FreeQuery(eFriendIsValidTarget, CharacterIDX, "EXEC %s %d, %d, \'%s\'", "MP_FRIEND_IsValidTarget", CharacterIDX, TargetID, FromName);
}

void FriendDelFriend(DWORD CharacterIDX, char* TargetName)
{
	g_DB.FreeQuery(eFriendDel, CharacterIDX, "EXEC %s %u, \'%s\'", "MP_FRIEND_DelFriend", CharacterIDX, TargetName);
}

void FriendDelFriendID(DWORD CharacterIDX, DWORD TargetID, DWORD bLast)
{
	g_DB.FreeQuery(eFriendDelID, CharacterIDX, "EXEC %s %u, %u, %d", "MP_FRIEND_DelFriendID", CharacterIDX, TargetID, bLast);
}

void FriendNotifyLogintoClient(DWORD CharacterIDX)
{
	g_DB.FreeQuery(eFriendNotifyLogin, CharacterIDX, "EXEC %s %u", "MP_FRIEND_NotifyLogin", CharacterIDX); //¬≥¬™¬∏¬¶ ¬µ√Æ¬∑√è√á√ë ¬ª√ß¬∂√∑¬µ√©
}

void FriendGetLoginFriends(DWORD CharacterIDX)
{
	g_DB.FreeQuery(eFriendGetLoginFriends, CharacterIDX, "EXEC %s %u", "MP_FRIEND_LoginFriend", CharacterIDX);//¬≥¬ª¬∞¬° ¬µ√Æ¬∑√è√á√ë ¬ª√ß¬∂√∑¬µ√©
}

void FriendGetFriendList(DWORD CharacterIDX)
{
	g_DB.FreeQuery(eFriendGetFriendList, CharacterIDX, "EXEC %s %u", "MP_FRIEND_GetFriendList", CharacterIDX);
}

void NoteIsNewNote(DWORD PlayerID)
{
	g_DB.FreeQuery(eNoteIsNewNote, PlayerID, "EXEC %s %u", "MP_NOTE_IsNewNote", PlayerID);
}

void NoteSendtoPlayer(DWORD FromIDX, char* FromName, char* ToName, char* Note)
{
	g_DB.FreeLargeQuery(RNoteSendtoPlayer, FromIDX, "EXEC %s \'%s\', \'%s\', \'%s\'", "MP_NOTE_SendNote", ToName, FromName, Note);	
}

void NoteServerSendtoPlayer(DWORD FromIDX, char* FromName, char* ToName, char* Note)
{
	g_DB.FreeLargeQuery(RNoteServerSendtoPlayer, FromIDX, "EXEC %s \'%s\', \'%s\', \'%s\'", "MP_NOTE_SendNote", ToName, FromName, Note);	
}

void NoteSendtoPlayerID(DWORD FromIDX, char* FromName, DWORD ToIDX, char* Note)
{
	g_DB.FreeLargeQuery(RNoteSendtoPlayer, FromIDX, "EXEC %s %u, \'%s\', %u, \'%s\'", "MP_NOTE_SendNoteID", FromIDX, FromName, ToIDX, Note);
}

void NoteDelAll(DWORD CharacterIDX)
{
	g_DB.FreeQuery(eNoteDelAll, 0, "EXEC %s %u", "MP_NOTE_DelAllNote", CharacterIDX);
}

void NoteList(DWORD CharacterIDX, WORD Page, WORD Mode)
{	
	USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(CharacterIDX);
	if(!userinfo)
		return;
	g_DB.FreeQuery(eNoteList, CharacterIDX, "EXEC %s %u, %d, %u, %d", "MP_NOTE_GetNoteList", CharacterIDX, 10, Page, Mode); //10¬∞¬≥¬æ¬ø ¬π√û¬æ√Ü¬ø√Ç¬¥√ô. 
}

void NoteRead(DWORD CharacterIDX, DWORD NoteIDX, DWORD IsFront)
{
	g_DB.FreeLargeQuery(RNoteRead, CharacterIDX, "EXEC %s %u, %u, %u", "MP_NOTE_ReadNote", CharacterIDX, NoteIDX, IsFront);
}

void NoteDelete(DWORD PlayerID, DWORD NoteID, BOOL bLast)
{	
	g_DB.FreeQuery(eNoteDelete, PlayerID, "EXEC %s %u, %u, %d", "MP_NOTE_DelNote", PlayerID, NoteID, bLast);
}

void WantedDelete(DWORD CharacterIDX)
{
	g_DB.FreeQuery(eWantedDelete, CharacterIDX, "EXEC %s %u", "MP_WANTED_Delete", CharacterIDX);
}

void WantedDelWantInfo(DWORD WantedIDX)
{
	g_DB.FreeQuery(eWantedDelWantInfo, 0, "EXEC %s %u", "MP_WANTED_DeleteWantedInfo", WantedIDX);
}

//---for GM_Tool
void GM_WhereIsCharacter(DWORD dwID, char* CharacterName, DWORD dwSeacherID )
{
	g_DB.FreeQuery(eGM_WhereIsCharacter, dwID, "EXEC %s \'%s\', %d", "MP_LoginCharacterSearchForName", CharacterName, dwSeacherID );
}

void GM_BanCharacter(DWORD dwID, char* CharacterName, DWORD dwSeacherID )
{
	g_DB.FreeQuery(eGM_BanCharacter, dwID, "EXEC %s \'%s\', %d", "MP_LoginCharacterSearchForName", CharacterName, dwSeacherID );
}

void GM_UpdateUserLevel(DWORD dwID, DWORD dwServerGroup, char* Charactername, BYTE UserLevel)
{
    char txt[128];
	sprintf(txt, "EXEC %s %d, \'%s\', %d", "MP_GMTOOL_UpdateUserLevel", dwServerGroup, Charactername, UserLevel);
	g_DB.LoginQuery(eQueryType_FreeQuery, eGM_UpdateUserLevel, dwID, txt);
}

void GM_Login( DWORD dwConnectionIdx, char* strID, char* strPW, char* strIP )
{
	char txt[128];
	sprintf(txt, "EXEC RP_OperatorLoginCheck \'%s\', \'%s\', \'%s\'", strID, strPW, strIP);	
	g_DB.LoginQuery(eQueryType_FreeQuery, eGM_Login, dwConnectionIdx, txt);
}

void GM_GetGMPowerList( DWORD dwStartIdx, DWORD dwFlag )
{
	char txt[128];
	sprintf(txt, "EXEC %s %d, %d", "RP_OperatorInfo", dwStartIdx, dwFlag );
	g_DB.LoginQuery(eQueryType_FreeQuery, eGM_GetGMPowerList, 0, txt);
}

void BillingRemainTimeUpdate(DWORD dwUserIdx, int nRemainTime )
{
	char txt[128];
	sprintf(txt, "EXEC %s %d, %d", "MP_REMAINTIME_Update", dwUserIdx, nRemainTime );
	g_DB.LoginQuery(eQueryType_FreeQuery, eBilling_RemainTimeUpdate, 0, txt);
}

void BillingUpdateAll( WORD wAgentNum )
{
	char txt[128];
	sprintf(txt, "EXEC %s %d", "up_AgentLastTime", wAgentNum );
	g_DB.LoginQuery(eQueryType_FreeQuery, eBilling_UpdateAll, 0, txt );
}


/*
void GM_MoveToCharacter( DWORD dwTargetID, char* CharacterName, DWORD dwSeacherID )
{
	g_DB.FreeQuery(eGM_MoveToCharacter, dwSeacherID, "EXEC %s \'%s\', %d", "MP_LoginCharacterSearchForName", CharacterName, dwTargetID );
}
*/

//---------

//---------------------------------------------------------------------------------------------------------
//
// Qeury Result Function
//
//---------------------------------------------------------------------------------------------------------
void	RUserIDXSendAndCharacterBaseInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	if(atoi((char*)pData[0].Data[0]) == 0)
		count = 0;
	DWORD AgentAuthKey = atoi((char*)pData[0].Data[eCL_AuthKey]);
	USERINFO* pInfo = g_pUserTable->FindUser(pMessage->dwID);
	if(pInfo == NULL)		// ?√å¬π√å ¬≥¬™¬∞¬¨?¬Ω
		return;
	if(pInfo->dwUniqueConnectIdx != AgentAuthKey)	// ¬≥¬™¬∞¬°¬∞√≠ ¬¥√ô¬∏¬•¬≥√ë?√å ¬µ√©¬æ√Æ¬ø√à
		return;

	if(count > dMAXCHARACTER)	// √Ñ¬≥¬∏¬Ø√Ö√ç √Å¬§¬∫¬∏¬π√û¬±√¢ ¬Ω√á√Ü√ê
	{
		count = dMAXCHARACTER;
		char temp[256];
		sprintf(temp,"√Ñ√â¬∏¬Ø√Ö√ç¬∞¬° 5¬∏√≠ ?√å¬ª√≥?√å¬æ√∫?¬Ω!!! useridx : %d", pInfo->dwUserID );
		ASSERTMSG(0,temp);
	}


	SEND_CHARSELECT_INFO msg;

	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTERLIST_ACK;

//---KES Crypt
#ifdef _CRYPTCHECK_ 
	msg.eninit = *pInfo->crypto.GetEnKey();
	msg.deinit = *pInfo->crypto.GetDeKey();
#endif
//--------
	if( !count ) /// ¬æ√Ü√Å√∑ ¬∏¬∏¬µ√ß √Ñ¬≥¬∏¬Ø√Ö√ç¬∞¬° ¬æ√∏¬¥√ô.
	{
		msg.CharNum = 0;			// ?√å √Ñ¬´¬ø√Æ√Ü¬Æ¬∞¬° 0?√å¬∏√© √Ñ¬≥¬∏¬Ø?√å √á√è¬≥¬™¬µ√é ¬æ√∏¬¥√Ç¬∞√ç?√å¬¥√ô

		g_Network.Send2User(pMessage->dwID, (char*)&msg, sizeof(SEND_CHARSELECT_INFO));

#ifdef _CRYPTCHECK_
		pInfo->crypto.SetInit( TRUE );		// init on	
#endif

#ifdef _NPROTECT_
		//¿Ã∞˜¿∫ ø©∑Ø±∫µ•º≠ µÈæÓø√ºˆµµ ¿÷¥Ÿ.(√π∑Œ±◊¿ŒΩ√, ƒ≥∏Ø≈Õª˝º∫√¢ø°º≠, ∞‘¿”ø°º≠)
		if( pInfo->m_nCSAInit == 0 )	//√π ¿Œ¡ı¿Ã æ»µ«æÓ ¿÷¥Ÿ∏È
		{
			pInfo->m_nCSAInit = 1;		//√π ¿Œ¡ı Ω√¿€
			NPROTECTMGR->SendAuthQuery(pInfo);

		}
#endif

		return;
	}

	msg.CharNum = (BYTE)(count);
		
	for( DWORD i=0; i<count; i++)
	{
		// √Ñ¬≥¬∏¬Ø√Ö√ç ¬±√¢¬∫¬ª√Å¬§¬∫¬∏ ¬º√Ç√Ü√É√á√è¬¥√Ç¬∞√∑
		msg.BaseObjectInfo[i].dwObjectID = atoi((char*)pData[i].Data[eCL_ObjectID]);
		msg.StandingArrayNum[i] = atoi((char*)pData[i].Data[eCL_StandIndex]);
//		strcpy(msg.BaseObjectInfo[i].ObjectName, (char*)pData[i].Data[eCL_ObjectName]);
		SafeStrCpy( msg.BaseObjectInfo[i].ObjectName, (char*)pData[i].Data[eCL_ObjectName], MAX_NAME_LENGTH+1 );
		msg.BaseObjectInfo[i].ObjectState = 0;
		msg.ChrTotalInfo[i].FaceType = atoi((char*)pData[i].Data[eCL_BodyType]);
		msg.ChrTotalInfo[i].HairType = atoi((char*)pData[i].Data[eCL_HeadType]);
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Hat] = 0;
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Weapon] = atoi((char*)pData[i].Data[eCL_Weapon]);
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Dress] = atoi((char*)pData[i].Data[eCL_Dress]);
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Shoes] = atoi((char*)pData[i].Data[eCL_shoes]);
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Earring1] = 0;
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Earring2] = 0;
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Glove] = atoi((char*)pData[i].Data[eCL_Glove]);
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Necklace] = 0;
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Ring1] = 0;
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Ring2] = 0;
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Belt] = 0;	
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Shield] = 0;
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Band] = 0;
		msg.ChrTotalInfo[i].Level = atoi((char*)pData[i].Data[eCL_Grade]);
		msg.ChrTotalInfo[i].LoginMapNum = atoi((char*)pData[i].Data[eCL_Map]);
		msg.ChrTotalInfo[i].Gender = atoi((char*)pData[i].Data[eCL_Gender]);
		msg.ChrTotalInfo[i].Race = atoi((char*)pData[i].Data[eCL_Race]);
		msg.ChrTotalInfo[i].JobGrade = atoi((char*)pData[i].Data[eCL_JobGrade]);
		msg.ChrTotalInfo[i].Job[0] = atoi((char*)pData[i].Data[eCL_Job1]);
		msg.ChrTotalInfo[i].Job[1] = atoi((char*)pData[i].Data[eCL_Job2]);
		msg.ChrTotalInfo[i].Job[2] = atoi((char*)pData[i].Data[eCL_Job3]);
		msg.ChrTotalInfo[i].Job[3] = atoi((char*)pData[i].Data[eCL_Job4]);
		msg.ChrTotalInfo[i].Job[4] = atoi((char*)pData[i].Data[eCL_Job5]);
		msg.ChrTotalInfo[i].Job[5] = atoi((char*)pData[i].Data[eCL_Job6]);
		msg.ChrTotalInfo[i].HideLevel = 0;
		msg.ChrTotalInfo[i].DetectLevel = 0;

		SafeStrCpy( msg.ChrTotalInfo[i].GuildName, (char*)pData[i].Data[eCL_GuildName], MAX_GUILD_NAME+1 );

		msg.ChrTotalInfo[i].Height = 1;
		msg.ChrTotalInfo[i].Width = 1;
//		msg.ChrTotalInfo[i].Stage = atoi((char*)pData[i].Data[eCL_Stage]);

		// desc_hseos_∞·»•_01
		// S ∞·»• √ﬂ∞° added by hseos 2007.12.13
		ZeroMemory(msg.ChrTotalInfo[i].szMarriagePartnerName,	  sizeof(msg.ChrTotalInfo[i].szMarriagePartnerName));
		ZeroMemory(msg.ChrTotalInfo[i].szMarriagePartnerNickName, sizeof(msg.ChrTotalInfo[i].szMarriagePartnerNickName));
		// E ∞·»• √ﬂ∞° added by hseos 2007.12.13

		pInfo->SelectInfoArray[i].dwCharacterID = msg.BaseObjectInfo[i].dwObjectID;
		pInfo->SelectInfoArray[i].Level = msg.ChrTotalInfo[i].Level;
		pInfo->SelectInfoArray[i].MapNum = msg.ChrTotalInfo[i].LoginMapNum;
		pInfo->SelectInfoArray[i].Gender = msg.ChrTotalInfo[i].Gender;

		//// 080331 LYW --- AgentDBMsgParser : Setting more info for chatroom system.
		//// Setting class index.
		//WORD wClassIndex = 0 ;
		//wClassIndex = 1000*msg.ChrTotalInfo[i].Job[0] + 
		//			  100*(msg.ChrTotalInfo[i].Race+1) + 
		//			  10*msg.ChrTotalInfo[i].JobGrade + 
		//			  msg.ChrTotalInfo[i].Job[msg.ChrTotalInfo[i].JobGrade-1] ;

		//pInfo->SelectInfoArray[i].wClassIndex = wClassIndex ;

		// Setting character name.
		SafeStrCpy(pInfo->SelectInfoArray[i].name, msg.BaseObjectInfo[i].ObjectName, 32) ;
		// End add. LYW

		// ¿Â∫Òº˚±‚±‚ ±‚¥…¿∫ ≤ˆ¥Ÿ.
		memset(msg.ChrTotalInfo[i].HideParts, 0, sizeof(msg.ChrTotalInfo[i].HideParts));
	}
	
	g_Network.Send2User(pMessage->dwID, (char*)&msg, sizeof(SEND_CHARSELECT_INFO));

#ifdef _CRYPTCHECK_
	pInfo->crypto.SetInit( TRUE );		// init on	
#endif

#ifdef _HACK_SHIELD_
		HACKSHIELDMGR->SendGUIDReq(pInfo);
#endif

#ifdef _NPROTECT_
		//¿Ã∞˜¿∫ ø©∑Ø±∫µ•º≠ µÈæÓø√ºˆµµ ¿÷¥Ÿ.(√π∑Œ±◊¿ŒΩ√, ƒ≥∏Ø≈Õª˝º∫√¢ø°º≠, ∞‘¿”ø°º≠)
		if( pInfo->m_nCSAInit == 0 )	//√π ¿Œ¡ı¿Ã æ»µ«æÓ ¿÷¥Ÿ∏È
		{
			pInfo->m_nCSAInit = 1;		//√π ¿Œ¡ı Ω√¿€
			NPROTECTMGR->SendAuthQuery(pInfo);
		}
#endif
}

/// ƒ…∏Ø≈Õ ª˝º∫Ω√ø° ªÁøÎ
void ItemInsertToDB(DWORD CharacterIdx, DWORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, DWORD dwKey, WORD bSeal)
{
	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d ", "dbo.MP_ITEM_Insert", CharacterIdx, wItemIdx, Durability, bPosition, bSeal);
	g_DB.Query(eQueryType_FreeQuery, eItemInsert, dwKey, txt);
}

void SkillInsertToDB(DWORD CharacterIDX, DWORD SkillIDX, BYTE level/*, POSTYPE SkillPos, BYTE bWeared, BYTE bLevel, WORD Option*/)
{
	char txt[128];
//	ASSERT(SkillPos);
	//sprintf(txt, "EXEC %s %u, %d, %d, %d, %d, %d", STORED_SKILL_INSERT, CharacterIDX, SkillIDX, SkillPos, bWeared, bLevel, Option);
	sprintf(txt, "EXEC %s %u, %d, %d", "MP_SKILLTREE_Insert", CharacterIDX, SkillIDX, level);
	g_DB.Query(eQueryType_FreeQuery, eSkillInsert, CharacterIDX, txt);
}

// desc_hseos_∏ÛΩ∫≈ÕπÃ≈Õ01
// S ∏ÛΩ∫≈ÕπÃ≈Õ √ﬂ∞° added by hseos 2007.05.29
// ..«√∑π¿ÃΩ√∞£, ∏ÛΩ∫≈Õ≈≥ ºˆ √ ±‚»≠
void InitMonstermeterInfoOfDB(DWORD nPlayerID)
{
	char txt[128];
	sprintf(txt, "EXEC  dbo.MP_MONSTERMETER_InitInfo %d", nPlayerID);
	g_DB.Query(eQueryType_FreeQuery, eInitMonstermeterInfoOfDB, 0, txt);
}
// E ∏ÛΩ∫≈ÕπÃ≈Õ √ﬂ∞° added by hseos 2007.05.29

// desc_hseos_º∫∫∞º±≈√01
// S º∫∫∞º±≈√ √ﬂ∞° added by hseos 2007.06.15
void GetUserSexKind(DWORD nUserID)
{
	char txt[128];
	sprintf(txt, "EXEC  MP_USER_SexKind %d", nUserID);
	g_DB.LoginQuery(eQueryType_FreeQuery, eGetUserSexKind, nUserID, txt);
}

void RGetUserSexKind(LPQUERY pData, LPDBMESSAGE pMessage)
{
	int result = atoi((char*)pData->Data[0]);
	DWORD dwConnectionIndex = pMessage->dwID;

	USERINFO* pInfo = g_pUserTableForUserID->FindUser(dwConnectionIndex);
	if(pInfo == NULL) return;

	pInfo->nSexKind = result;

	// ≈¨∂Û¿Ãæ∆Æø° æÀ∏Æ±‚
	MSG_DWORD msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_USER_SEXKIND;
	msg.dwData	 = result;
	g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
}
// E º∫∫∞º±≈√ √ﬂ∞° added by hseos 2007.06.15

void	RCreateCharacter(LPQUERY pData, LPDBMESSAGE pMessage)
{
	int result = atoi((char*)pData->Data[0]);
	DWORD dwConnectionIndex = pMessage->dwID;

	switch(result)
	{
	case 0:
		{
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pInfo)
			{
				MSGBASE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_CHARACTER_MAKE_NACK;
				g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
				return;
			}

			ASSERT(pInfo->dwUserID);

			if(pInfo->dwUserID == 0)
			{
				ASSERTMSG(0, "UserID¬∞¬° 0?√å¬¥√ô.");

				MSGBASE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_CHARACTER_MAKE_NACK;
				g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
				return;
			}
			
			/// ±‚∫ª ª˝º∫ æ∆¿Ã≈€

			DWORD idx = atoi((char*)pData->Data[1]);
			
			int job = atoi((char*)pData->Data[2]);
	
			switch( job )
			{
			case 1:
				{
					SkillInsertToDB( idx, 1101101, 1 );
					SkillInsertToDB( idx, 1101201, 1 );
				}
				break;
			case 2:
				{
					SkillInsertToDB( idx, 2101001, 1 );
				}
				break;
			case 3:
				{
					SkillInsertToDB( idx, 3200101, 1 );
				}
				break;
			}

			UserIDXSendAndCharacterBaseInfo(pInfo->dwUserID,pInfo->dwUniqueConnectIdx,dwConnectionIndex);
/*
			pInfo->SelectInfoArray[ MAX_CHARACTER_NUM - 1].dwCharacterID = idx;
			pInfo->SelectInfoArray[ MAX_CHARACTER_NUM - 1].MapNum = 19;

			// 080430 LYW --- AgentDBMsgParser : √§∆√πÊ Ω√Ω∫≈€¿∏∑Œ ¿Œ«ÿ, ƒ≥∏Ø≈Õ ¿Ã∏ßµµ √ﬂ∞°«‘.
			pInfo->SelectInfoArray[ MAX_CHARACTER_NUM - 1 ].Level = 1 ;
			SafeStrCpy(pInfo->SelectInfoArray[ MAX_CHARACTER_NUM - 1 ].name, (char*)pData->Data[3], 32) ;

			// 080507 LYW --- AgentDBMsgParser : √§∆√πÊ Ω√Ω∫≈€¿ª ¿ß«ÿ, ¿Ø¿˙ ¡§∫∏∏¶ Dist º≠πˆø° √ﬂ∞°«—¥Ÿ.
			ST_CR_USER user ;
			memset(&user, 0, sizeof(ST_CR_USER)) ;

			user.byLevel	= pInfo->SelectInfoArray[ MAX_CHARACTER_NUM - 1 ].Level ;
			user.byMapNum	= pInfo->SelectInfoArray[ MAX_CHARACTER_NUM - 1].MapNum ;
			user.dwPlayerID	= pInfo->SelectInfoArray[ MAX_CHARACTER_NUM - 1].dwCharacterID ;
			user.dwUserID	= pInfo->dwUserID ;

			SafeStrCpy(user.name, pInfo->SelectInfoArray[ MAX_CHARACTER_NUM - 1 ].name, MAX_NAME_LENGTH) ;

			// ¿Ø¿˙ ¡§∫∏∏¶ Dist∑Œ µÓ∑œΩ√≈∞∞Ì, ¿Ø¿˙∞° µÓ∑œ µ» ªÛ≈¬∑Œ ºº∆√«—¥Ÿ.
			if(CHATROOMMGR->RegistPlayer_To_Lobby(&user))
			{
				pInfo->byAddedChatSystem = TRUE ;
			}
			// END

			MSG_DWORD msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHARACTER_MAKE_ACK;
			msg.dwData = idx;
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
*/		}
		break;
	case 1:
		{
			// √Ñ¬≥¬∏¬Ø√Ö√ç¬∞¬° ¬≤√ã√É¬°¬¥√ô.
			MSGBASE msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHARACTER_MAKE_NACK;
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	case 2:
		{
			// ?√å¬∏¬ß?√å √Å√ü¬∫¬π¬µ√ä ¬ø?¬∑√π
			MSG_WORD msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHARACTER_NAMECHECK_NACK;
			msg.wData = result;
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	case 3:
		{
			// ?√å¬∏¬ß?√å NULL?√è¬∂¬ß
			MSG_WORD msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHARACTER_NAMECHECK_NACK;
			msg.wData = result;
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	case 4:
		{
			// ¬π¬Æ√Ü√Ñ ?√å¬∏¬ß¬∞√∫ ¬∞√£√Ñ¬• ¬∂¬ß
			MSG_WORD msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHARACTER_NAMECHECK_NACK;
			msg.wData = result;
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	default:
		ASSERT(0);
		return;
	}
}
void	RCharacterNameCheck(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if(atoi((char*)pData->Data[0])==0)
	{
		// ?√å¬∏¬ß √Å√ü¬∫¬π ¬æ√à¬µ√ä
		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTER_NAMECHECK_ACK;
		g_Network.Send2User(pMessage->dwID, (char*)&msg, sizeof(msg));
	}
	else
	{
		// ?√å¬∏¬ß?√å √Å√ü¬∫¬π¬µ√ä ¬ø?¬∑√π
		MSG_WORD msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTER_NAMECHECK_NACK;
		msg.wData = 2;
		g_Network.Send2User(pMessage->dwID, (char*)&msg, sizeof(msg));
	}
}

void RSearchWhisperUserAndSend(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	if(!count)
	{
		//return;
		ASSERT(0);
	}
	else
	{
		MSG_CHAT* pMsgChat = g_MsgTable.GetMsg( pMessage->dwID );
		if( pMsgChat == NULL ) return;

		USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pMsgChat->dwObjectID );
		if( !pSenderInfo ) 
		{
			g_MsgTable.RemoveMsg( pMessage->dwID );
			return;
		}

		int nError	= atoi((char*)pData->Data[0]);
		int nLenEr	= strlen((char*)pData->Data[0]);	//050118 fix Error for ID including '1'
		if( nLenEr == 1 && ( nError == CHATERR_NO_NAME || nError == CHATERR_NOT_CONNECTED ) )
		{
			MSG_BYTE msg;
			msg.Category	= MP_CHAT;
			msg.Protocol	= MP_CHAT_WHISPER_NACK;
			msg.dwObjectID	= pMsgChat->dwObjectID;
			msg.bData		= nError;
			g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		else
		{
			DWORD dwReceiverID = (DWORD)atoi((char*)pData->Data[1]);
			USERINFO* pReceiverInfo = g_pUserTableForObjectID->FindUser( dwReceiverID );
			
			if( pReceiverInfo )	//¬π√û?¬ª ¬ª√ß¬∂√∑?√å ?√å ¬º¬≠¬π√∂¬ø¬° ?√ñ¬¥√Ç√Å√∂ ¬∞√ã¬ª√ß
			{
				if( pReceiverInfo->GameOption.bNoWhisper && pSenderInfo->UserLevel != eUSERLEVEL_GM )
				{
					MSG_BYTE msg;
					msg.Category	= MP_CHAT;
					msg.Protocol	= MP_CHAT_WHISPER_NACK;
					msg.dwObjectID	= pMsgChat->dwObjectID;	//¬∫¬∏¬≥¬Ω¬ª√ß¬∂√∑ ¬æ√Ü?√å¬µ√∞
					msg.bData		= CHATERR_OPTION_NOWHISPER;

					g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
				}
				else
				{
					//¬∫¬∏¬≥¬Ω ¬ª√ß¬∂√∑¬ø¬°¬∞√î ¬∫¬∏¬≥¬ª¬∞√≠,
					MSG_CHAT msgToSender = *pMsgChat;
					msgToSender.Category = MP_CHAT;
					msgToSender.Protocol = MP_CHAT_WHISPER_ACK;
					g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msgToSender, msgToSender.GetMsgLength() );	//CHATMSG 040324

					//¬π√û¬¥√Ç ¬ª√ß¬∂√∑¬ø¬°¬∞√î ¬∫¬∏¬≥¬ª¬∞√≠,
					MSG_CHAT msgToReceiver = *pMsgChat;
					msgToReceiver.Category = MP_CHAT;
					if( pSenderInfo->UserLevel == eUSERLEVEL_GM )
						msgToReceiver.Protocol = MP_CHAT_WHISPER_GM;
					else
						msgToReceiver.Protocol = MP_CHAT_WHISPER;
					SafeStrCpy( msgToReceiver.Name, (char*)pData->Data[0], MAX_NAME_LENGTH + 1 );	//¬∫¬∏¬≥¬Ω¬ª√ß¬∂√∑?√á ?√å¬∏¬ß?¬∏¬∑√é ¬π√ô¬≤√û
					g_Network.Send2User( pReceiverInfo->dwConnectionIndex, (char*)&msgToReceiver, msgToReceiver.GetMsgLength() );
				}
			}
			else
			{
				MSG_WHISPER msgWhisper;
				msgWhisper.Category		= MP_CHAT;
				if( pSenderInfo->UserLevel == eUSERLEVEL_GM )
					msgWhisper.Protocol		= MP_CHAT_WHISPER_GM_SYN;
				else
					msgWhisper.Protocol		= MP_CHAT_WHISPER_SYN;

				msgWhisper.dwObjectID	= pMsgChat->dwObjectID;					//¬∫¬∏¬≥¬Ω¬ª√ß¬∂√∑
				msgWhisper.dwReceiverID	= (DWORD)atoi((char*)pData->Data[1]);	//¬π√û¬¥√Ç¬ª√ß¬∂√∑
				SafeStrCpy( msgWhisper.SenderName, (char*)pData->Data[0], MAX_NAME_LENGTH + 1 ); //¬∫¬∏¬≥¬Ω¬ª√ß¬∂√∑?√á ?√å¬∏¬ß
				SafeStrCpy( msgWhisper.ReceiverName, pMsgChat->Name, MAX_NAME_LENGTH + 1 ); //¬π√û¬¥√Ç¬ª√ß¬∂√∑?√á ?√å¬∏¬ß
				SafeStrCpy( msgWhisper.Msg, pMsgChat->Msg, MAX_CHAT_LENGTH + 1 );	//√É¬§√Ü√É¬≥¬ª¬ø√´

				g_Network.Broadcast2AgentServerExceptSelf( (char*)&msgWhisper, msgWhisper.GetMsgLength() );	//CHATMSG 040324
			}
		}
	}

	g_MsgTable.RemoveMsg( pMessage->dwID );
}

void RFriendDelFriend(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(pRecverInfo)
	{
		if(atoi((char*)pData->Data[eFr_IsSuccess]) != 0)
		{
			MSG_NAME_DWORD msg;
			msg.Category = MP_FRIEND;
			msg.Protocol = MP_FRIEND_DEL_ACK;
			SafeStrCpy( msg.Name, (char*)pData->Data[eFr_targetname], MAX_NAME_LENGTH + 1 );
			msg.dwData = atoi((char*)pData->Data[eFr_IsSuccess]); //ack ?√è¬∂¬ß friendidx return
			msg.dwObjectID = pMessage->dwID;

			g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(MSG_NAME_DWORD));
		}
		else
		{
			MSG_FRIEND_MEMBER_ADDDELETEID msg;
			msg.Category	= MP_FRIEND;
			msg.Protocol	= MP_FRIEND_DEL_NACK;
			SafeStrCpy( msg.Name, (char*)pData->Data[eFr_targetname], MAX_NAME_LENGTH + 1 );
			msg.dwObjectID	= pMessage->dwID;
			msg.PlayerID	= atoi((char*)pData->Data[eFr_IsSuccess]);

			g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
		}
	}
}


void RFriendAddFriend(LPQUERY pData, LPDBMESSAGE pMessage)
{
	//¬µ√Æ¬∑√è ¬Ω√Ö√É¬ª√á√ë ¬ª√ß¬∂√∑
	MSG_FRIEND_MEMBER_ADDDELETEID msg;
	msg.Category = MP_FRIEND;
	SafeStrCpy( msg.Name, (char*)pData->Data[eFr_addToName], MAX_NAME_LENGTH + 1 );
	msg.PlayerID = atoi((char*)pData->Data[eFr_addToIDX]);
	msg.dwObjectID = atoi((char*)pData->Data[eFr_addFromIDX]);

	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(atoi((char*)pData->Data[eFr_addFromIDX]));
	if(pRecverInfo)
	{
		if(atoi((char*)pData->Data[eFr_addFromErr]) == 0)
			msg.Protocol = MP_FRIEND_ADD_ACK;
		else
		{
			msg.PlayerID = atoi((char*)pData->Data[eFr_addFromErr]);
			msg.Protocol = MP_FRIEND_ADD_NACK;
		}
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
	}		
	else 
	{
		//¬¥√ô¬∏¬• ¬ø¬°?√å?√º√Ü¬Æ¬ø¬° ?√ñ¬¥√ô. 
		if(atoi((char*)pData->Data[eFr_addFromErr]) == 0)
			msg.Protocol = MP_FRIEND_ADD_ACK_TO_AGENT;
		else
		{
			msg.PlayerID = atoi((char*)pData->Data[eFr_addFromErr]);
			msg.Protocol = MP_FRIEND_ADD_NACK_TO_AGENT;
		}
		g_Network.Broadcast2AgentServerExceptSelf((char*)&msg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
	}
	
	//¬µ√Æ¬∑√è¬µ√á¬æ√Æ√Å√∂¬¥√Ç ¬ª√ß¬∂√∑
	MSG_FRIEND_MEMBER_ADDDELETEID bmsg;
	bmsg.Category = MP_FRIEND;
	SafeStrCpy(bmsg.Name, (char*)pData->Data[eFr_addFromName], MAX_NAME_LENGTH+1);
	bmsg.dwObjectID = atoi((char*)pData->Data[eFr_addToIDX]);
	bmsg.PlayerID = atoi((char*)pData->Data[eFr_addFromIDX]);
	USERINFO* pToRecverInfo = g_pUserTableForObjectID->FindUser(atoi((char*)pData->Data[eFr_addToIDX]));
	if(pToRecverInfo)
	{
		if(atoi((char*)pData->Data[eFr_addToErr]) == 0) //ack
			bmsg.Protocol = MP_FRIEND_ADD_ACCEPT_ACK;
		else	//nack
		{
			bmsg.PlayerID = atoi((char*)pData->Data[eFr_addToErr]);
			bmsg.Protocol = MP_FRIEND_ADD_ACCEPT_NACK;
		}
		g_Network.Send2User(pToRecverInfo->dwConnectionIndex, (char*)&bmsg, sizeof(bmsg));
	}
	else //another agent
	{
		if(atoi((char*)pData->Data[eFr_addToErr]) == 0) //ack
			bmsg.Protocol = MP_FRIEND_ADD_ACCEPT_TO_AGENT;
		else //nack
		{
			bmsg.PlayerID = atoi((char*)pData->Data[eFr_addToErr]);
			bmsg.Protocol = MP_FRIEND_ADD_ACCEPT_NACK_TO_AGENT;
		}
		g_Network.Broadcast2AgentServerExceptSelf((char*)&bmsg, sizeof(bmsg));
	}
}

void RFriendIsValidTarget(LPQUERY pData, LPDBMESSAGE pMessage)
{
	MSG_FRIEND_MEMBER_ADDDELETEID msg;
	memset(&msg, 0, sizeof(msg));

	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(atoi((char*)pData->Data[eFr_vtTargetid]));
	if(!pSenderInfo)
		return;

	if(atoi((char*)pData->Data[eFr_Err]) != 0)
	{		
		//nack 
		msg.Category = MP_FRIEND;
		msg.dwObjectID = pMessage->dwID;
		msg.Protocol = MP_FRIEND_ADD_NACK;
		SafeStrCpy( msg.Name, (char*)pData->Data[eFr_vtToname], MAX_NAME_LENGTH + 1 );
		msg.PlayerID = atoi((char*)pData->Data[eFr_Err]);	//errcode insert

		g_Network.Send2User(pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
	else
	{
		if(pRecverInfo)
		{
			if(pRecverInfo->GameOption.bNoFriend == TRUE)
			{
				msg.Category = MP_FRIEND;
				msg.dwObjectID = pMessage->dwID;
				msg.Protocol = MP_FRIEND_ADD_NACK;
				SafeStrCpy( msg.Name, (char*)pData->Data[eFr_vtToname], MAX_NAME_LENGTH + 1 );
				msg.PlayerID = eFriend_OptionNoFriend;	//errcode insert

				g_Network.Send2User(pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
			}
			else
			{
				msg.Category = MP_FRIEND;
				msg.dwObjectID = atoi((char*)pData->Data[eFr_vtTargetid]);
				msg.Protocol = MP_FRIEND_ADD_INVITE;
				SafeStrCpy( msg.Name, (char*)pData->Data[eFr_vtFromname], MAX_NAME_LENGTH + 1 );
				msg.PlayerID = pMessage->dwID;
				g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
			}
		}
		else
		{
			// ¬¥√ô¬∏¬• Agent¬ø¬° ¬º√ì√á√ò?√ñ¬¥√ô.
			msg.Category = MP_FRIEND;
			msg.Protocol = MP_FRIEND_ADD_INVITE_TO_AGENT;
			SafeStrCpy( msg.Name, (char*)pData->Data[eFr_vtFromname], MAX_NAME_LENGTH + 1 );
			msg.PlayerID = pMessage->dwID;
			
			msg.dwObjectID = atoi((char*)pData->Data[eFr_vtTargetid]);

			g_Network.Broadcast2AgentServerExceptSelf((char*)&msg, sizeof(msg));
		}
	}
}

void RFriendDelFriendID(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(pRecverInfo)
	{
		MSG_DWORD_WORD msg;
		msg.Category = MP_FRIEND;
		msg.Protocol = MP_FRIEND_DELID_ACK;
		msg.wData = atoi((char*)pData->Data[0]); //bLast
		msg.dwData = atoi((char*)pData->Data[1]); //targetid
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
}

void RFriendNotifyLogintoClient(LPQUERY pData, LPDBMESSAGE pMessage)
{
	MSG_NAME_DWORD msg;
	msg.Category = MP_FRIEND;
	msg.Protocol = MP_FRIEND_LOGIN_NOTIFY;
	SafeStrCpy( msg.Name, (char*)pData[0].Data[eFr_LLoggedname], MAX_NAME_LENGTH + 1 );
	msg.dwData = pMessage->dwID;

	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(atoi((char*)pData[i].Data[eFr_LTargetID]));
		msg.dwObjectID = atoi((char*)pData[i].Data[eFr_LTargetID]);
		if(pRecverInfo)
		{
			MSG_NAME_DWORD msgTemp = msg;
			g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msgTemp, sizeof(msgTemp));
		}
		else
		{
			//¬¥√ô¬∏¬• ¬ø¬°?√å?√º√Ü¬Æ
			msg.Protocol = MP_FRIEND_LOGIN_NOTIFY_TO_AGENT;
			g_Network.Broadcast2AgentServerExceptSelf((char*)&msg, sizeof(msg));
		}
	}
}

void RFriendGetLoginFriends(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(!pRecverInfo)
		return;

	MSG_NAME msg;
	msg.Category = MP_FRIEND;
	msg.Protocol = MP_FRIEND_LOGIN_FRIEND;
	msg.dwObjectID = pMessage->dwID;
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
//		strcpy(msg.Name, (char*)pData[i].Data[eFr_LFFriendName]);
		SafeStrCpy( msg.Name, (char*)pData[i].Data[eFr_LFFriendName], MAX_NAME_LENGTH + 1 );
		
		MSG_NAME msgTemp = msg;
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msgTemp, sizeof(msgTemp));
	}
}

void RFriendGetFriendList(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(!pRecverInfo)
		return;	

	MSG_FRIEND_LIST_DLG msg;
	memset(&msg, 0, sizeof(msg));
	msg.Category = MP_FRIEND;
	msg.Protocol = MP_FRIEND_LIST_ACK;

	msg.count = pMessage->dwResult <= MAX_FRIEND_NUM ? (BYTE)pMessage->dwResult : MAX_FRIEND_NUM;
	
	if(pMessage->dwResult > MAX_FRIEND_NUM)
	{
		ASSERT(pMessage->dwResult <= MAX_FRIEND_NUM);
		msg.count = MAX_FRIEND_NUM;
	}
	for(DWORD i=0; i< msg.count; ++i)
	{
		msg.FriendList[i].Id = atol((char*)pData[i].Data[eFr_FLFriendid]);
		msg.FriendList[i].IsLoggIn = atoi((char*)pData[i].Data[eFr_FLIsLogin]);
		SafeStrCpy( msg.FriendList[i].Name, (char*)pData[i].Data[eFr_FLFriendname], MAX_NAME_LENGTH + 1 );
	}
	msg.dwObjectID = pMessage->dwID;
	
	g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, msg.GetMsgLength());
}

void RNoteIsNewNote(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(!pRecverInfo)
		return;
	
	if(atoi((char*)pData->Data[0]) == 1)
	{
		MSGBASE msg;
		msg.Category = MP_NOTE;
		msg.Protocol = MP_NOTE_NEW_NOTE;
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
}

void RNoteSendtoPlayer(LPLARGEQUERY pData, LPDBMESSAGE pMessage)
{	
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	// desc_hseos_≥Û¿ÂΩ√Ω∫≈€_01
	// S ≥Û¿ÂΩ√Ω∫≈€ √ﬂ∞° added by hseos 2008.01.18
	// ≥Û¿Â «ÿ√º Ω√ Ω√Ω∫≈€ ∏ﬁºº¡ˆ∏¶ ∫∏≥ªπ«∑Œ ¬ ¡ˆ πﬁ¿Ω ∏ﬁºº¡ˆ¥¬ æ¯æÓµµ µ… µÌ~
	/*
	// ..Ω√Ω∫≈€ ¬ ¡ˆ¿« ∞ÊøÏ ID ∞° 0 ¿Ã∞Ì ∫∏≥ª¥¬ ¿Ã∏ß¿Ã <SYSTEM> ¿Ã¥Ÿ
	if (pMessage->dwID == 0)
	{
		// Ω√Ω∫≈€ ¬ ¡ˆ ¿¸º€¿Ã º∫∞¯«œ∏È πﬁ¥¬ «√∑π¿ÃæÓø°∞‘∏∏ æÀ∏∞¥Ÿ.
		if (atoi((char*)pData->Data[eFr_NErr]) == 0)
		{
			DWORD Toidx = atoi((char*)pData->Data[eFr_NToId]);
			if(Toidx == 0) //¬∑√é¬±√ó¬æ√Ü¬ø√¥ ¬ª√≥√Ö√Ç
				return;
			MSGBASE rmsg;
			rmsg.Category = MP_NOTE;
			rmsg.Protocol = MP_NOTE_RECEIVENOTE;
			rmsg.dwObjectID = Toidx;

			USERINFO* pToRecverInfo = g_pUserTableForObjectID->FindUser(Toidx);
			if(pToRecverInfo)
			{
				g_Network.Send2User(pToRecverInfo->dwConnectionIndex, (char*)&rmsg, sizeof(rmsg));
			}
			else //¬¥√ô¬∏¬• ¬ø¬°?√å?√º√Ü¬Æ¬ø¬° ?√ñ¬¥√ô. 
			{
				g_Network.Broadcast2AgentServerExceptSelf( (char*)&rmsg, sizeof(rmsg) );
			}
		}
		return;
	}
	*/
	// E ≥Û¿ÂΩ√Ω∫≈€ √ﬂ∞° added by hseos 2008.01.18

	if(!pRecverInfo)
		return;

	if(atoi((char*)pData->Data[eFr_NErr]) == 0) //success
	{
		MSG_NAME msg;
		msg.Category = MP_NOTE;
		msg.Protocol = MP_NOTE_SENDNOTE_ACK;
		SafeStrCpy( msg.Name, (char*)pData->Data[eFr_NToName], MAX_NAME_LENGTH + 1 );
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));

	}
	else
	{
		MSG_NAME_WORD msg;
		msg.Category = MP_NOTE;
		msg.Protocol = MP_NOTE_SENDNOTE_NACK;
		msg.wData = atoi((char*)pData->Data[eFr_NErr]); // 2:invalid user, 3: full space
		SafeStrCpy( msg.Name, (char*)pData->Data[eFr_NToName], MAX_NAME_LENGTH + 1 );
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
		return;
	}

	//√Ç√ä√Å√∂ ¬π√û¬¥√Ç ¬ª√ß¬∂√∑
	DWORD Toidx = atoi((char*)pData->Data[eFr_NToId]);
	if(Toidx == 0) //¬∑√é¬±√ó¬æ√Ü¬ø√¥ ¬ª√≥√Ö√Ç
		return;
	MSGBASE rmsg;
	rmsg.Category = MP_NOTE;
	rmsg.Protocol = MP_NOTE_RECEIVENOTE;
	rmsg.dwObjectID = Toidx;

	USERINFO* pToRecverInfo = g_pUserTableForObjectID->FindUser(Toidx);
	if(pToRecverInfo)
	{
		g_Network.Send2User(pToRecverInfo->dwConnectionIndex, (char*)&rmsg, sizeof(rmsg));
	}
	else //¬¥√ô¬∏¬• ¬ø¬°?√å?√º√Ü¬Æ¬ø¬° ?√ñ¬¥√ô. 
	{
		g_Network.Broadcast2AgentServerExceptSelf( (char*)&rmsg, sizeof(rmsg) );
	}
}

void RNoteServerSendtoPlayer(LPLARGEQUERY pData, LPDBMESSAGE pMessage)
{	
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(!pRecverInfo)
		return;

	if(atoi((char*)pData->Data[eFr_NErr]) == 0) //success
	{
/*		MSG_NAME msg;
		msg.Category = MP_NOTE;
		msg.Protocol = MP_NOTE_SENDNOTE_ACK;
		SafeStrCpy( msg.Name, (char*)pData->Data[eFr_NToName], MAX_NAME_LENGTH + 1 );
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
*/
	}
	else
	{
		MSG_NAME_WORD msg;
		msg.Category = MP_NOTE;
		msg.Protocol = MP_NOTE_SENDNOTE_NACK;
		msg.wData = atoi((char*)pData->Data[eFr_NErr]); // 2:invalid user, 3: full space
		SafeStrCpy( msg.Name, (char*)pData->Data[eFr_NToName], MAX_NAME_LENGTH + 1 );
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
		return;

	}

	//√Ç√ä√Å√∂ ¬π√û¬¥√Ç ¬ª√ß¬∂√∑
	DWORD Toidx = atoi((char*)pData->Data[eFr_NToId]);
	if(Toidx == 0) //¬∑√é¬±√ó¬æ√Ü¬ø√¥ ¬ª√≥√Ö√Ç
		return;
	MSGBASE rmsg;
	rmsg.Category = MP_NOTE;
	rmsg.Protocol = MP_NOTE_RECEIVENOTE;
	rmsg.dwObjectID = Toidx;

	USERINFO* pToRecverInfo = g_pUserTableForObjectID->FindUser(Toidx);
	if(pToRecverInfo)
	{
		g_Network.Send2User(pToRecverInfo->dwConnectionIndex, (char*)&rmsg, sizeof(rmsg));
	}
	else //¬¥√ô¬∏¬• ¬ø¬°?√å?√º√Ü¬Æ¬ø¬° ?√ñ¬¥√ô. 
	{
		g_Network.Broadcast2AgentServerExceptSelf( (char*)&rmsg, sizeof(rmsg) );
	}
}

void RNoteList(LPQUERY pData, LPDBMESSAGE pMessage)
{	
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(!pRecverInfo)
		return;

	SYSTEMTIME ti;
	GetLocalTime(&ti);
	
	char tempDate[64];

	static MSG_FRIEND_NOTE_LIST msg;
	memset(&msg,0,sizeof(MSG_FRIEND_NOTE_LIST));
	
	msg.Category = MP_NOTE;
	msg.Protocol = MP_NOTE_NOTELIST_ACK;
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		SafeStrCpy( tempDate, (char*)pData[i].Data[eFr_NSentDate], 64 );
/*		if(IsSameDay(tempDate, ti.wYear, ti.wMonth, ti.wDay))
		{
			SafeStrCpy( tempDate, (char*)pData[i].Data[eFr_NSentTime], 64 );
			int len = strlen(tempDate)-3;
			//.SendData¬π√∂√Ü√õ¬∞¬° 16?√å¬¥√ô. 	//KES
			if( len >= 16 )
				SafeStrCpy( msg.NoteList[i].SendDate, tempDate, 16 );
			else
			{
				sprintf( msg.NoteList[i].SendDate, "%d-%d ", ti.wMonth, ti.wDay );
				strncat( msg.NoteList[i].SendDate, tempDate, len );
				//strncpy(msg.NoteList[i].SendDate, tempDate, len);
			}
		}
		else
		{
			SafeStrCpy( msg.NoteList[i].SendDate, (char*)pData[i].Data[eFr_NSentDate], 11);
		}*/

		char buf1[64] = { 0, };
		char buf2[64] = { 0, };
		strncpy( buf1, &tempDate[5], strlen(tempDate)-5);
		strcpy( tempDate, (char*)pData[i].Data[eFr_NSentTime] );
		strncpy( buf2, tempDate, strlen(tempDate)-3 );
		sprintf( msg.NoteList[i].SendDate, "%s - %s", buf1, buf2 );
				
		SafeStrCpy( msg.NoteList[i].FromName, (char*)pData[i].Data[eFr_NSender], MAX_NAME_LENGTH + 1 );
		
		msg.NoteList[i].NoteID = atoi((char*)pData[i].Data[eFr_NNoteID]);
		msg.NoteList[i].bIsRead = atoi((char*)pData[i].Data[eFr_NbIsRead]);
		msg.dwObjectID = pMessage->dwID; 
		
	}
	msg.TotalPage = atoi((char*)pData[0].Data[eFr_NTotalpage]);
	
	g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
	
}

void RNoteRead(LPLARGEQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(!pRecverInfo)
		return;

	MSG_FRIEND_READ_NOTE msg;
	msg.Category = MP_NOTE;
	msg.Protocol = MP_NOTE_READNOTE_ACK;
	SafeStrCpy( msg.FromName, (char*)pData->Data[eFr_NRNSender], MAX_NAME_LENGTH + 1 );
	SafeStrCpy( msg.Note, (char*)pData->Data[eFr_NRNNote], MAX_NOTE_LENGTH + 1 );
	msg.NoteID = atoi((char*)pData->Data[eFr_NRNNoteID]);
	msg.ItemIdx = atoi((char*)pData->Data[eFr_NRNItemIdx]);
	
	g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, msg.GetMsgLength());
}

void RNoteDelete(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(!pRecverInfo)
		return;

	MSG_FRIEND_DEL_NOTE msg;
	msg.Category = MP_NOTE;
	msg.Protocol = MP_NOTE_DELNOTE_ACK;
	msg.bLast = atoi((char*)pData->Data[eFr_NdbLast]);
	msg.NoteID 	= atoi((char*)pData->Data[eFr_NdNoteID]);
	g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
}

void RDeleteCharacter(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pInfo = g_pUserTable->FindUser( pMessage->dwID );
	if( !pInfo )			return;

	MSG_DWORD msg;
	msg.Category = MP_USERCONN;

	if(atoi((char*)pData->Data[0]) != 0)
	{
		msg.Protocol = MP_USERCONN_CHARACTER_REMOVE_NACK;
		msg.dwData = atoi((char*)pData->Data[0]);
	}
	else
	{
		msg.Protocol = MP_USERCONN_CHARACTER_REMOVE_ACK;
		//√á√∂¬ª√≥¬±√ù √Å√∂¬ø√≤
		DWORD CharacterIdx = atoi((char*)pData->Data[1]);
		WantedDelete( CharacterIdx );

		for(int i=0; i<MAX_CHARACTER_NUM; ++i)
		{
			if( pInfo->SelectInfoArray[i].dwCharacterID == CharacterIdx )
				memset( &pInfo->SelectInfoArray[i], 0, sizeof(CHARSELECTINFO) );
		}
	}

	g_Network.Send2User(pMessage->dwID, (char*)&msg, sizeof(msg));
}

void RWantedDelete(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if(atoi((char*)pData->Data[0]) == 0) //√á√∂¬ª√≥¬±√ù¬ø¬° ¬∞√º¬∑√É¬µ√à¬∞√î ¬æ√∏¬¥√ô.
		return;
	//FOR SERVER TO SERVER
	MSGBASE msg;
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_DELETE_BY_DELETECHR;
	msg.dwObjectID = atoi((char*)pData[0].Data[0]);  //wanted_idx 
	g_Network.Broadcast2MapServer((char*)&msg, sizeof(msg));

	//FOR CLIENT TO SERVER
	USERINFO* pUserInfo = NULL;
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		MSG_WORD msg;
		msg.Category = MP_WANTED;
		msg.dwObjectID = atoi((char*)pData[i].Data[1]); //character_idx
		msg.wData = atoi((char*)pData[0].Data[0]); //wanted_idx 

		pUserInfo = g_pUserTableForObjectID->FindUser(atoi((char*)pData->Data[1]));
		if(!pUserInfo)
		{
			//broadcasting to another agent
			msg.Protocol = MP_WANTED_NOTCOMPLETE_TO_AGENT;
			g_Network.Broadcast2AgentServerExceptSelf((char*)&msg, sizeof(msg));
		}
		else
		{			
			msg.Protocol = MP_WANTED_NOTCOMPLETE_BY_DELCHR;
			g_Network.Send2Server(pUserInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));
		}
	}
	if(pMessage->dwResult < MAX_QUERY_RESULT)
	{
		WantedDelWantInfo(atoi((char*)pData[0].Data[0]));
	}
	else
	{
		//√É√ü√à√Ñ ?√õ¬æ√∑
	}
}

//---for GM_Tool
void RGM_WhereIsCharacter(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	if(!count)
	{

	}
	else
	{
		//(DWORD)atoi((char*)pData->Data[2])	: ¬∏√ä¬π√∏√à¬£
		//(DWORD)atoi((char*)pData->Data[1])	: √É¬£?¬∫ objectID

		USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
		if( !pSenderInfo ) return;

		int nError = atoi((char*)pData->Data[0]);
		int nLenEr	= strlen((char*)pData->Data[0]);	//050118 Error for ID including '1'
		if( nLenEr && ( nError == CHATERR_NO_NAME || nError == CHATERR_NOT_CONNECTED ) )
		{
			MSG_BYTE msg;
			msg.Category	= MP_CHEAT;
			msg.Protocol	= MP_CHEAT_WHEREIS_NACK;
			msg.dwObjectID	= pMessage->dwID;
			msg.bData		= nError;
			g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		else
		{
			DWORD dwTargetID	= (DWORD)atoi((char*)pData->Data[1]);
			DWORD dwMapNum		= (DWORD)atoi((char*)pData->Data[2]);

//			USERINFO* pTargetInfo = g_pUserTableForObjectID->FindUser( dwTargetID );

			if( dwMapNum > 0 )
			{
				MSG_DWORD msg;
				msg.Category	= MP_CHEAT;
				msg.Protocol	= MP_CHEAT_WHEREIS_SYN;
				msg.dwObjectID	= pMessage->dwID;
				msg.dwData		= dwTargetID;	//√É¬£?¬∫ ¬æ√Ü?√å¬µ√∞

				WORD wServerPort = g_pServerTable->GetServerPort( eSK_MAP, (WORD)dwMapNum );
				SERVERINFO* pInfo = g_pServerTable->FindServer( wServerPort );				

				if( pInfo )
					g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
			}
			else
			{
				MSG_WORD msg;
				msg.Category	= MP_CHEAT;
				msg.Protocol	= MP_CHEAT_WHEREIS_ACK;
				msg.dwObjectID	= pMessage->dwID;
				msg.wData		= (WORD)dwMapNum;
				g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
			}
		}
	}
}

void RGM_BanCharacter(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	if(!count)
	{

	}
	else
	{
		//(char*)pData->Data[0]					: ¬∫¬∏¬≥¬Ω¬ª√ß¬∂√∑ ?√å¬∏¬ß
		//(DWORD)atoi((char*)pData->Data[1])	: √É¬£?¬∫ objectID

		USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
		if( !pSenderInfo ) return;

		int nError = atoi((char*)pData->Data[0]);
		int nLenEr	= strlen((char*)pData->Data[0]);	//050118 Error for ID including '1'
		if( nLenEr && ( nError == CHATERR_NO_NAME || nError == CHATERR_NOT_CONNECTED ) )
		{
			MSG_BYTE msg;
			msg.Category	= MP_CHEAT;
			msg.Protocol	= MP_CHEAT_BANCHARACTER_NACK;
			msg.dwObjectID	= pMessage->dwID;
			msg.bData		= nError;
			g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		else
		{
			DWORD dwTargetID = (DWORD)atoi((char*)pData->Data[1]);
			USERINFO* pTargetInfo = g_pUserTableForObjectID->FindUser( dwTargetID );

			if( pTargetInfo )
			{
				DWORD dwConIdx = pTargetInfo->dwConnectionIndex;
				OnDisconnectUser( dwConIdx );
				DisconnectUser( dwConIdx );

				MSGBASE msg;
				msg.Category	= MP_CHEAT;
				msg.Protocol	= MP_CHEAT_BANCHARACTER_ACK;
				msg.dwObjectID	= pMessage->dwID;
				g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );

				// 06.09.12 RaMa
				LogGMToolUse( pMessage->dwID, eGMLog_Disconnect_User, MP_CHEAT_BANCHARACTER_ACK, dwTargetID, 0 );
			}
			else
			{
				MSG_DWORD2 msg;
				msg.Category	= MP_CHEAT;
				msg.Protocol	= MP_CHEAT_BANCHARACTER_SYN;
				msg.dwData1		= dwTargetID;
				msg.dwData2		= pMessage->dwID;

				g_Network.Broadcast2AgentServerExceptSelf( (char*)&msg, sizeof(msg) );
			}
		}
	}
}

void RGM_UpdateUserLevel(LPQUERY pData, LPDBMESSAGE pMessage)
{
	// pMessage->dwID
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	MSGBASE msg;
	msg.Category	= MP_CHEAT;
	msg.dwObjectID	= pMessage->dwID;

	if(atoi((char*)pData->Data[0])==0)
	{
		// ¬±√ó¬∑¬± ?√å¬∏¬ß ¬æ√∏¬æ√Æ~~
		msg.Protocol = MP_CHEAT_BLOCKCHARACTER_NACK;
	}
	else
	{
		msg.Protocol = MP_CHEAT_BLOCKCHARACTER_ACK;
		// ¬æ√∑¬µ¬•?√å√Ü¬Æ ¬º¬∫¬∞√∏~~

		DWORD useridx = atoi((char*)pData->Data[1]);
		DWORD state = atoi((char*)pData->Data[2]);

		// 06.09.12 RaMa
		LogGMToolUse( pMessage->dwID, eGMLog_Block, useridx, state, 0 );
	}
	g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
}


void RGM_Login(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	DWORD dwConnectionIndex = pMessage->dwID;

	USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionIndex );

	if( count == 0 || pUserInfo == NULL ) // ?√å¬ª√≥ ¬ø?¬∑√π
	{
		MSG_BYTE msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_GM_LOGIN_NACK;
		msg.bData		= 0;

		g_Network.Send2User( dwConnectionIndex, (char*)&msg, sizeof(msg) );
		return;
	}
/*
enum eOperInfo
{
	eOI_ErroCode = 0, eOI_OperIdx, eOI_OperID, eOI_OperName, eOI_OperPwd, eOI_OperPower, eOI_Date, eOI_Time, 
	eOI_IPIdx, eOI_IPAdress, eOI_IPDate, eOI_IPTime, 
};
*/
	WORD check = atoi((char*)pData[0].Data[0]);

	if( check != 0 ) // √Å¬¢¬º√ì ¬∫√í¬∞¬°
	{
		MSG_BYTE msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_GM_LOGIN_NACK;
		msg.bData		= 1;

		g_Network.Send2User( dwConnectionIndex, (char*)&msg, sizeof(msg) );
		return;
	}

	int nPower = atoi((char*)pData[0].Data[5]);

	if( nPower < 0 || nPower >= eGM_POWER_MAX )
	{
		MSG_BYTE msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_GM_LOGIN_NACK;
		msg.bData		= 2;

		g_Network.Send2User( dwConnectionIndex, (char*)&msg, sizeof(msg) );
		return;		
	}

	DWORD dwIdx = (DWORD)atoi((char*)pData[0].Data[1]);
	char szName[MAX_NAME_LENGTH+1];

	SafeStrCpy( szName, (char*)pData[0].Data[2], MAX_NAME_LENGTH+1 );

	GMINFO->AddGMList( dwConnectionIndex, nPower, dwIdx, szName );

	MSG_DWORD Msg;
	Msg.Category	= MP_CHEAT;
	Msg.Protocol	= MP_CHEAT_GM_LOGIN_ACK;
	Msg.dwData		= nPower;

	g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );
}


void RGM_GetGMPowerList(LPQUERY pData, LPDBMESSAGE pMessage)
{
/*
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count )
	{
		DWORD dwFlag = atoi((char*)pData[0].Data[0]);
		if( dwFlag == 0 )
			GMINFO->Release();

		GM_POWER pw;
		DWORD startIdx = 0;
		for( DWORD i = 0; i < count; ++i )
		{			
			startIdx = atoi((char*)pData[i].Data[1]);
			SafeStrCpy( pw.GM_ID, (char*)pData[i].Data[2], MAX_NAME_LENGTH+1 );
			pw.dwUserID = 0;
			pw.nPower = atoi((char*)pData[i].Data[5]);

			GMINFO->AddGMList( &pw );
		}
	
		if( count >= 100 )
			GM_GetGMPowerList( startIdx, count );
	}
*/
}


/* --; √á√ä¬ø√§¬æ√∏¬∞√ö¬≥√ü. ?√ü¬∏√∏√Ç¬•¬µ√ª.
void RGM_MoveToCharacter(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	if(!count)
	{

	}
	else
	{
		
		//(DWORD)atoi((char*)pData->Data[1])	: √É¬£?¬∫ objectID
		//(char*)pData->Data[0]					: ¬∫¬∏¬≥¬Ω¬ª√ß¬∂√∑ ?√å¬∏¬ß

		USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
		if( !pSenderInfo ) return;

		int nError = atoi((char*)pData->Data[0]);
		if( nError == CHATERR_NO_NAME || nError == CHATERR_NOT_CONNECTED )
		{
			MSG_BYTE msg;
			msg.Category	= MP_CHEAT;
			msg.Protocol	= MP_CHEAT_MOVETOCHAR_NACK;
			msg.dwObjectID	= pMessage->dwID;
			msg.bData		= nError;
			g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		else
		{
			DWORD dwTargetID = (DWORD)atoi((char*)pData->Data[1]);
			USERINFO* pTargetInfo = g_pUserTableForObjectID->FindUser( (DWORD)atoi((char*)pData->Data[1]) );

			//?√å¬º¬≠¬π√∂¬ø¬° ?√ñ¬≥¬™?
			if( pTargetInfo )
			{
				//---¬≥¬™√Å√ü¬ø¬° gm¬∏√≠¬∑√â?¬∫ ¬¥√ô¬∏¬•¬µ¬•¬∑√é....
				//obejctid¬∏¬¶ ¬∏√ä¬º¬≠¬π√∂¬∑√é ¬∫¬∏¬≥¬ª?√ö!

			}
			else
			{

			}
		}


	}
}
*/

void CheckGuildMasterLogin( DWORD dwConnectionIdx, DWORD dwPlayerIdx, char* pSearchName, DWORD dwMoney, BYTE Protocol )
{
	g_DB.FreeQuery( eCheckGuildMasterLogin, dwConnectionIdx, "EXEC MP_GUILDFIELDWAR_CheckMasterLogin %d, \'%s\', %d, %d",
		dwPlayerIdx, pSearchName, dwMoney, Protocol );
}

void RCheckGuildMasterLogin( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD dwConnectionIndex = pMessage->dwID;
	int nFlag = atoi( (char*)pData->Data[0] );

	switch( nFlag )
	{
	case 0:		// login
		{
			DWORD dwSenderIdx = (DWORD)atoi((char*)pData->Data[1]);
			DWORD dwMasterIdx = (DWORD)atoi((char*)pData->Data[2]);
			DWORD dwMap = (DWORD)atoi((char*)pData->Data[3]);
			DWORD dwMoney = (DWORD)atoi((char*)pData->Data[4]);
			BYTE Protocol = (BYTE)atoi((char*)pData->Data[5]);

			MSG_DWORD3 Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = Protocol;
			Msg.dwData1 = dwSenderIdx;
			Msg.dwData2 = dwMasterIdx;
			Msg.dwData3 = dwMoney;

			USERINFO* userinfo = (USERINFO*)g_pUserTable->FindUser( dwConnectionIndex );
			if( userinfo == NULL )	return;
			WORD wPort = g_pServerTable->GetServerPort( eSK_MAP, (WORD)dwMap );
			if( !wPort )	return;
			SERVERINFO* pSInfo = g_pServerTable->FindServer( wPort );
			if( !pSInfo )	return;

			if( userinfo->dwMapServerConnectionIndex == pSInfo->dwConnectionIndex )
			{
				g_Network.Send2Server( userinfo->dwMapServerConnectionIndex, (char*)&Msg, sizeof(Msg) );
			}
			else
			{
				g_Network.Send2Server( userinfo->dwMapServerConnectionIndex, (char*)&Msg, sizeof(Msg) );
				g_Network.Send2Server( pSInfo->dwConnectionIndex, (char*)&Msg, sizeof(Msg) );
			}
			/*			
			g_Network.Broadcast2MapServer( (char*)&Msg, sizeof(Msg) );
			{
				SERVERINFO* pSInfo = g_pServerTable->FindServer( wPort );
				g_Network.Send2Server( pSInfo->dwConnectionIndex, (char*)&Msg, sizeof(Msg) );
			}
			MSG_BYTE Wait;
			Wait.Category = MP_GUILD_WAR;
			Wait.Protocol = MP_GUILD_WAR_WAIT;
			Wait.bData = Protocol;						
			g_Network.Send2User( dwConnectionIndex, (char*)&Wait, sizeof(Wait) );
*/			
		}
		break;
	case 1:		// sender is not master
		{
			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = MP_GUILD_WAR_NACK;
			Msg.bData = nFlag;
			g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );			
		}
		break;
	case 2:		// is not guild
		{
			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = MP_GUILD_WAR_NACK;
			Msg.bData = nFlag;
			g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );
		}
		break;
	case 3:		// same guild	
		{
			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = MP_GUILD_WAR_NACK;
			Msg.bData = nFlag;
			g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );
		}
		break;
	case 4:		// not login
		{
			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = MP_GUILD_WAR_NACK;
			Msg.bData = nFlag;
			g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );
		}
		break;
	}
}

void CheckGuildFieldWarMoney( DWORD dwConnectionIndex, DWORD dwSenderIdx, DWORD dwEnemyGuildIdx, DWORD dwMoney )
{
	g_DB.FreeQuery( eCheckGuildFieldWarMoney, dwConnectionIndex, "EXEC dbo.MP_GUILDFIELDWAR_CheckMoney %d, %d, %d",
		dwSenderIdx, dwEnemyGuildIdx, dwMoney );
}

void RCheckGuildFieldWarMoney( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD dwConnectionIndex = pMessage->dwID;
	int nFlag = atoi( (char*)pData->Data[0] );

	switch( nFlag )
	{
	case 0:		// login
		{
			DWORD dwSenderIdx = (DWORD)atoi((char*)pData->Data[1]);
			DWORD dwEnemyGuildIdx = (DWORD)atoi((char*)pData->Data[2]);
			DWORD dwMoney = (DWORD)atoi((char*)pData->Data[3]);

			MSG_DWORD2 Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = MP_GUILD_WAR_DECLARE_ACCEPT;
			Msg.dwObjectID = dwSenderIdx;
			Msg.dwData1 = dwEnemyGuildIdx;
			Msg.dwData2 = dwMoney;

			USERINFO* userinfo = (USERINFO*)g_pUserTable->FindUser( dwConnectionIndex );
			if( userinfo == NULL )	return;
			g_Network.Send2Server( userinfo->dwMapServerConnectionIndex, (char*)&Msg, sizeof(Msg) );
		}
		break;
	case 1:		// receiver not login
		{
			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = MP_GUILD_WAR_NACK;
			Msg.bData = 4;
			g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );			
		}
		break;
	case 2:		// receiver money not enough
		{
			// sender
			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = MP_GUILD_WAR_NACK;
			Msg.bData = 5;
			g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );

			// receiver	
			DWORD dwReceiverIdx = (DWORD)atoi((char*)pData->Data[1]);
			DWORD dwMap = (DWORD)atoi((char*)pData->Data[2]);

			MSG_BYTE Msg1;
			Msg1.Category = MP_GUILD_WAR;
			Msg1.Protocol = MP_GUILD_WAR_NACK;
			Msg1.dwObjectID = dwReceiverIdx;
			Msg1.bData = 6;			

			WORD wPort = g_pServerTable->GetServerPort( eSK_MAP, (WORD)dwMap );
			if( wPort )
			{
				SERVERINFO* pSInfo = g_pServerTable->FindServer( wPort );
				g_Network.Send2Server( pSInfo->dwConnectionIndex, (char*)&Msg1, sizeof(Msg1) );
			}
		}
		break;
	case 3:		// sender money not enough	
		{
			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = MP_GUILD_WAR_NACK;
			Msg.bData = 6;
			g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );

			// receiver	
			DWORD dwReceiverIdx = (DWORD)atoi((char*)pData->Data[1]);
			DWORD dwMap = (DWORD)atoi((char*)pData->Data[2]);

			MSG_BYTE Msg1;
			Msg1.Category = MP_GUILD_WAR;
			Msg1.Protocol = MP_GUILD_WAR_NACK;
			Msg1.dwObjectID = dwReceiverIdx;
			Msg1.bData = 5;			

			WORD wPort = g_pServerTable->GetServerPort( eSK_MAP, (WORD)dwMap );
			if( wPort )
			{
				SERVERINFO* pSInfo = g_pServerTable->FindServer( wPort );
				g_Network.Send2Server( pSInfo->dwConnectionIndex, (char*)&Msg1, sizeof(Msg1) );
			}
		}
		break;
	}
}

void AddGuildFieldWarMoney( DWORD dwConnectionIndex, DWORD dwPlayerIdx, DWORD dwMoney )
{
	g_DB.FreeQuery( eAddGuildFieldWarMoney, dwConnectionIndex, "EXEC dbo.MP_GUILDFIELDWAR_AddMoney %d, %d",
		dwPlayerIdx, dwMoney );
}

void RAddGuildFieldWarMoney( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD dwConnectionIndex = pMessage->dwID;
	int nFlag = atoi( (char*)pData->Data[0] );

	switch( nFlag )
	{
	case 0:		// login
		{
			DWORD dwMap = (DWORD)atoi((char*)pData->Data[1]);
			DWORD dwPlayerIdx = (DWORD)atoi((char*)pData->Data[2]);
			DWORD dwMoney = (DWORD)atoi((char*)pData->Data[3]);

			MSG_DWORD2 Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = MP_GUILD_WAR_ADDMONEY_TOMAP;
			Msg.dwData1 = dwPlayerIdx;
			Msg.dwData2 = dwMoney;

			WORD wPort = g_pServerTable->GetServerPort( eSK_MAP, (WORD)dwMap );
			if( wPort )
			{
				SERVERINFO* pSInfo = g_pServerTable->FindServer( wPort );
				g_Network.Send2Server( pSInfo->dwConnectionIndex, (char*)&Msg, sizeof(Msg) );
			}
		}
		break;
	case 1:		// not login
		{
			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_WAR;
			Msg.Protocol = MP_GUILD_WAR_NACK;
			Msg.bData = 4;
			g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );			
		}
		break;
	}
}

//	jackpot
void JackpotLoadTotalMoney()
{
	char txt[128];
	sprintf( txt, "EXEC dbo.MP_JACKPOT_LoadMoney" );
	g_DB.Query( eQueryType_FreeQuery, eJackpot_TotalMoney_Load, 0, txt );
}

void RJackpotLoadTotalMoney( LPQUERY pData, LPDBMESSAGE pMessage )
{	//√—±›æ◊ ∞¸∏Æ «œ¥¬ AGENT ∞° æ∆¥œ∏È πﬁ¡ˆ∏¯«‘
	if( pMessage->dwResult == 0 ) return;

	DWORD TotalMoney = (DWORD)atoi((char*)pData->Data[0]);
	JACKPOTMGR->SetTotalMoney( TotalMoney );
//COMMENT:JACKPOT

	JACKPOTMGR->SendMsgUserTotalMoney();

}

// event
void EventItemUse050106( DWORD dwUserIdx )
{
	char txt[128];
	sprintf( txt, "EXEC up_event_insert %d", dwUserIdx );
	g_DB.LoginQuery( eQueryType_FreeQuery, eEventItemUse050106, 0, txt);
}

void EventItemUse050203( DWORD dwUserIdx, char* sCharName, DWORD dwServerNum )
{
	char txt[128];
	sprintf( txt, "EXEC up_event0203_Ginsert %d, \'%s\', %d", dwUserIdx, sCharName, dwServerNum );
	g_DB.LoginQuery( eQueryType_FreeQuery, eEventItemUse050106, 0, txt);
}

void EventItemUse050322( DWORD dwUserIdx )
{
	char txt[128];
	sprintf( txt, "EXEC dbo.up_event0322_insert %d", dwUserIdx );
	g_DB.LoginQuery( eQueryType_FreeQuery, eEventItemUse050322, 0, txt);
}

void EventItemUse051108( DWORD dwUserIdx, char* sCharName, DWORD dwServerNum )
{
	char txt[128];
	sprintf( txt, "EXEC up_event051108_insert %d, \'%s\', %d", dwUserIdx, sCharName, dwServerNum );
	g_DB.LoginQuery( eQueryType_FreeQuery, eEventItemUse051108, 0, txt);
}

void EventItemUse2( DWORD dwUserIdx, char* sCharName, DWORD dwServerNum )
{
	char txt[128];
	sprintf( txt, "EXEC dbo.Up_EventItemUse2 %d, \'%s\', %d", dwUserIdx, sCharName, dwServerNum );
	g_DB.LoginQuery( eQueryType_FreeQuery, eEventItemUse2, 0, txt);
}

// for billing
void InsertBillingTable( DWORD dwUserIdx, DWORD dwBillType, DWORD dwAgentNo, DWORD dwServerSetNum )
{
	char txt[128];
	sprintf( txt, "EXEC dbo.Up_InsertBillingTable %d, %d, %d, %d", dwUserIdx, dwBillType, dwAgentNo, dwServerSetNum );
	g_DB.LoginQuery( eQueryType_FreeQuery, eInsertBillingTable, 0, txt);
}

void DeleteBillingTable( DWORD dwUserIdx, DWORD dwServerSetNum )
{
	char txt[128];
	sprintf( txt, "EXEC dbo.Up_DeleteBillingTable %d, %d", dwUserIdx, dwServerSetNum );
	g_DB.LoginQuery( eQueryType_FreeQuery, eDeleteBillingTable, 0, txt);
}


//
void ChaseFindUser( DWORD UserIdx, char* WantedName, DWORD dwItemIdx )
{	
	g_DB.FreeQuery(eChaseFindUser, UserIdx, "EXEC %s \'%s\', %d", "MP_character_name_chk", WantedName, dwItemIdx );
}
void RChaseFindUser( LPQUERY pData, LPDBMESSAGE pMessage )
{
	// ∑Œ±◊¿Œ æ»«ﬂ¿ª ∞ÊøÏ
	if(atoi((char*)pData->Data[0]) == 0)
	{
		MSG_DWORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_CHASE_NACK;
		msg.dwData = 1;
		USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
		if( pInfo )
			g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
	// √¥ªÏ∏Ò∑œø° æ¯¥¬ ∞ÊøÏ
	else if(atoi((char*)pData->Data[0]) == 1)
	{
		MSG_DWORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_CHASE_NACK;
		msg.dwData = 3;
		USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
		if( pInfo )
			g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
	else
	{
		DWORD CharacterIdx = atoi((char*)pData->Data[0]);
		DWORD MapNum = atoi((char*)pData->Data[1]);
		DWORD ItemIdx = atoi((char*)pData->Data[2]);

		SERVERINFO* pInfo = g_pServerTable->FindMapServer( (WORD)MapNum );
		if( pInfo )
		{
			MSG_DWORD3 msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_CHASE_SYN;
			msg.dwData1 = pMessage->dwID;
			msg.dwData2 = CharacterIdx;
			msg.dwData3 = ItemIdx;

			g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
	}
}



void GetCharacterSlotCount( DWORD UserIdx, DWORD AuthKey, DWORD dwConnectionIndex )
{
	g_DB.FreeQuery(eJP_CharacterSlot, dwConnectionIndex, "EXEC MP_CHARACTER_GetCharacterSlot_JP %d, %d", UserIdx, AuthKey);
}

void RGetCharacterSlotCount( LPQUERY pData, LPDBMESSAGE pMessage )
{

}

void GM_UpdateUserState(DWORD dwID, DWORD dwServerGroup, char* Charactername, BYTE UserState)
{
	g_DB.FreeQuery( eGM_UpdateUserState, dwID, "EXEC dbo.MP_GMTOOL_UpdateUserState %d, \'%s\', %d", dwServerGroup, Charactername, UserState );
}

void RGM_UpdateUserState(LPQUERY pData, LPDBMESSAGE pMessage)
{
	// pMessage->dwID
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	MSGBASE msg;
	msg.Category	= MP_CHEAT;
	msg.dwObjectID	= pMessage->dwID;

	if(atoi((char*)pData->Data[0])==0)
	{
		// ¬±√ó¬∑¬± ?√å¬∏¬ß ¬æ√∏¬æ√Æ~~
		msg.Protocol = MP_CHEAT_BLOCKCHARACTER_NACK;
	}
	else
	{
		msg.Protocol = MP_CHEAT_BLOCKCHARACTER_ACK;
		// ¬æ√∑¬µ¬•?√å√Ü¬Æ ¬º¬∫¬∞√∏~~
	}
	g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );

}

void LogGMToolUse( DWORD CharacterIdx, DWORD GMLogtype, DWORD Logkind, DWORD Param1, DWORD Param2 )
{
	/*char txt[128] = { 0, };
	sprintf( txt, "EXEC dbo.Up_GMToolUseLog %d, %d, %d, %d, %d",
		CharacterIdx,
		GMLogtype,
		Logkind,
		Param1,
		Param2
	);
	g_DB.LogQuery( eQueryType_FreeQuery, eLogGMToolUse, 0, txt );*/
}

// desc_hseos_∆–π–∏Æ01
// S ∆–π–∏Æ √ﬂ∞° added by hseos 2007.07.09	2007.07.10	2007.07.14	2007.10.11
#define STORED_FAMILY_LOAD_INFO				"dbo.MP_FAMILY_LoadInfo"
#define STORED_FAMILY_LOAD_INFO_EMBLEM		"dbo.MP_FAMILY_LoadInfo_Emblem"
#define STORED_FAMILY_SAVE_INFO				"dbo.MP_FAMILY_SaveInfo"
#define STORED_FAMILY_SAVE_INFO_HONORPOINT	"dbo.MP_FAMILY_SaveInfo_HonorPoint"
#define STORED_FAMILY_SAVE_INFO_EMBLEM		"dbo.MP_FAMILY_SaveInfo_Emblem"

#define STORED_FAMILY_MEMBER_LOAD_INFO		"dbo.MP_FAMILY_MEMBER_LoadInfo"
#define STORED_FAMILY_MEMBER_LOAD_EXINFO_01	"dbo.MP_FAMILY_MEMBER_LoadExInfo01"
#define STORED_FAMILY_MEMBER_SAVE_INFO		"dbo.MP_FAMILY_MEMBER_SaveInfo"

#define STORED_FAMILY_CHECKNAME				"dbo.MP_FAMILY_CheckName"

#define STORED_FAMILY_LEAVE_LOAD_INFO		"dbo.MP_FAMILY_LEAVE_LoadInfo"
#define STORED_FAMILY_LEAVE_SAVE_INFO		"dbo.MP_FAMILY_LEAVE_SaveInfo"

char txt[128];
void Family_SaveInfo(DWORD nMasterID, char* pszFamilyName, int nHonorPoint, BOOL bNicknameON, int nSaveType)
{
	if (nSaveType == 0)
	{
		// ∆–π–∏Æ ¿Ã∏ß¿ª '' ∑Œ æ» π≠æÓ¡÷¥œ±Ó 1231∆–π–∏Æ øÕ ∞∞¿∫ º¯º≠∑Œ º˝¿⁄ «—±€¡∂«’(øµπÆ¿∫ »Æ¿Œ æ» «ÿ∫√¿Ω)¿ª πÆ¿⁄ø≠∑Œ ¿ŒΩƒ«œ¡ˆ æ ¥¬¥Ÿ.
		// SQLº≠πˆø°º≠ º˝¿⁄∞° ∏’¿˙ ≥™ø¿∏È πÆ¿⁄ø≠∑Œ ¿ŒΩƒ«œ¡ˆ æ ¥¬ µÌ...
		sprintf(txt, "EXEC  %s '%s', %d, %d, %d", STORED_FAMILY_SAVE_INFO, pszFamilyName, nMasterID, nHonorPoint, bNicknameON);
		g_DB.Query(eQueryType_FreeQuery, eFamily_SaveInfo, nMasterID, txt);
	}
	else if (nSaveType == 1)
	{
		sprintf(txt, "EXEC  %s %d, %d", STORED_FAMILY_SAVE_INFO_HONORPOINT,  nMasterID, nHonorPoint);
		g_DB.Query(eQueryType_FreeQuery, eFamily_SaveInfo, nMasterID, txt);
	}
}

void RFamily_SaveInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	// ∆–π–∏Æ ª˝º∫ Ω√ ∆–π–∏Æ¿« ID∏¶ πﬁ±‚ ¿ß«ÿ.. 
	// ..∆–π–∏Æ∏¶ √≥¿Ω ª˝º∫ Ω√ø°¥¬ DBø° ¿˙¿Â«— »ƒ Ω«¡¶∑Œ ∆–π–∏Æ∏¶ ª˝º∫«—¥Ÿ. ª˝º∫ »ƒ ¿˙¿ÂΩ√ø°¥¬ Update ∏∏ «œ±‚ ∂ßπÆø° ø©±‚∑Œ ø¿¡ˆ æ ¥¬¥Ÿ.
	// ..ªÁΩ« ID∏∏ πﬁæ∆º≠ º≥¡§«œ∏È µ«¥¬µ•, πﬁ¥¬ ±Ëø° ∏µÁ ¡§∫∏∏¶ ¥Ÿ πﬁæ∆º≠ πﬁ¿∫ ¡§∫∏∑Œ º≥¡§!!!
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	if( pMessage->dwResult == 0 )
	{
	}
	else
	{
		// ∆–π–∏Æ ¡§∫∏
		CSHFamily::stINFO		stInfo;
		CSHFamily::stINFO_EX	stInfoEx;

		ZeroMemory(&stInfo, sizeof(stInfo));
		ZeroMemory(&stInfoEx, sizeof(stInfoEx));
		stInfo.nID				= atoi((char*)pData[0].Data[0]);
		SafeStrCpy(stInfo.szName, (char*)pData[0].Data[1], MAX_NAME_LENGTH+1);
		stInfo.nMasterID		= atoi((char*)pData[0].Data[2]);
		stInfoEx.nHonorPoint	= atoi((char*)pData[0].Data[3]);
		stInfoEx.nNicknameON	= atoi((char*)pData[0].Data[4]);

		// ∏‚πˆ(¿⁄Ω≈) ¡§∫∏ (¡¢º” ªÛ≈¬)
		CSHFamilyMember::stINFO		stMemberInfo;

		ZeroMemory(&stMemberInfo, sizeof(stMemberInfo));
		stMemberInfo.nID		= pUserInfo->dwCharacterID;
		stMemberInfo.eConState	= CSHFamilyMember::MEMBER_CONSTATE_LOGIN;

		CSHFamilyMember csFamilyMember;
		csFamilyMember.Set(&stMemberInfo);

		// √÷¡æ ∆–π–∏Æ ª˝º∫
		CSHFamily csFamily;
		csFamily.Set(&stInfo, &stInfoEx);
		g_csFamilyManager.ASRV_CreateFamily(pUserInfo, stInfo.szName, CSHFamilyManager::FCS_COMPLETE, &csFamily);

		// ∏∂Ω∫≈Õ ¡§∫∏ ¿–±‚
		Family_Member_LoadInfo(pUserInfo->dwCharacterID, stInfo.nID);
		// 081205 LUJ, ∑Œ±◊
		InsertLogFamily(
			eLog_FamilyCreate,
			stInfo.nID,
			stInfo.nMasterID,
			stInfo.szName );
	}
}

void Family_SaveEmblemInfo(DWORD nPlayerID, DWORD nFamilyID, char* pcImg)
{
	char buf[CSHFamilyManager::EMBLEM_BUFSIZE*2+256];
	sprintf(buf, "EXEC  %s %d, 0x", STORED_FAMILY_SAVE_INFO_EMBLEM,  nFamilyID);

	int curpos = strlen(buf);
	for(int n=0;n<CSHFamilyManager::EMBLEM_BUFSIZE;++n)
	{
		sprintf(&buf[curpos],"%02x",(BYTE)pcImg[n]);
		curpos += 2;
	}

	g_DB.FreeLargeQuery(NULL, 0, buf);
}

void Family_LoadInfo(DWORD nPlayerID)
{
	sprintf(txt, "EXEC  %s %d", STORED_FAMILY_LOAD_INFO, nPlayerID);
	g_DB.Query(eQueryType_FreeQuery, eFamily_LoadInfo, nPlayerID, txt);
}

void RFamily_LoadInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	if( pMessage->dwResult == 0 )
	{
	}
	else
	{
		// ¿ÃπÃ ≈◊¿Ã∫Ìø° ¿÷¥Ÿ∏È ∆˜¿Œ≈Õ∏∏ ∞°¡Æø¬¥Ÿ.
		CSHFamily* pcsAleadyFamily = g_csFamilyManager.GetFamily(atoi((char*)pData[0].Data[0]));
		if (pcsAleadyFamily)
		{
			pUserInfo->pcsFamily = pcsAleadyFamily;
			// ¡¢º” ªÛ≈¬ º≥¡§
			int nLoginCnt = 0;
			for(UINT i=0; i<pUserInfo->pcsFamily->Get()->nMemberNum; i++)
			{
				if (pUserInfo->dwCharacterID == pUserInfo->pcsFamily->GetMember(i)->Get()->nID)
				{
					CSHFamilyMember::stINFO stInfo = *pUserInfo->pcsFamily->GetMember(i)->Get();
					
					stInfo.eConState = CSHFamilyMember::MEMBER_CONSTATE_LOGIN;
					pUserInfo->pcsFamily->GetMember(i)->Set(&stInfo);

					// ∏ º≠πˆø° »£ƒ™ æÀ∏Æ±‚
					g_csFamilyManager.ASRV_SendNicknameToMap(pUserInfo, pUserInfo->pcsFamily->GetMember(i)->Get()->szNickname);
				}

				if (pUserInfo->pcsFamily->GetMember(i)->Get()->eConState == CSHFamilyMember::MEMBER_CONSTATE_LOGIN)
				{
					nLoginCnt++;
				}
			}

			// «ˆ¿Á ¡¢º”«ÿ ¿÷¥¬ ∏‚πˆ∞° 2∏Ì¿Ã µ«∏È ∏Ìøπ ∆˜¿Œ∆Æ Ω√∞£¿ª º≥¡§«—¥Ÿ.
			if (nLoginCnt == CSHFamilyManager::HONOR_POINT_CHECK_MEMBER_NUM)
			{
				CSHFamily::stINFO_EX stInfoEx = *pUserInfo->pcsFamily->GetEx();
				stInfoEx.nHonorPointTimeTick = gCurTime;
				pUserInfo->pcsFamily->SetEx(&stInfoEx);
			}

			// ¥Ÿ∏• ø°¿Ã¿¸∆Æø° æÀ∏Æ±‚
			g_csFamilyManager.ASRV_SendMemberConToOtherAgent(pUserInfo->pcsFamily, pUserInfo->dwCharacterID, CSHFamilyMember::MEMBER_CONSTATE_LOGIN);
			// ≈¨∂Û¿Ãæ∆Æø° ¡§∫∏ ∫∏≥ª±‚
			g_csFamilyManager.ASRV_SendInfoToClient(pUserInfo->pcsFamily);
			// πÆ¿Â ∑ŒµÂ
			Family_LoadEmblemInfo(pUserInfo->dwCharacterID, pUserInfo->pcsFamily->Get()->nID);
			// ∏ º≠πˆø° ≥Û¿ÂUI ¡§∫∏ ø‰√ª
			g_csFarmManager.ASRV_RequestFarmUIInfoToMap(pUserInfo->dwCharacterID, pUserInfo->pcsFamily);
		}
		// ∆–π–∏Æ ≈◊¿Ã∫Ìø° √ﬂ∞°«—¥Ÿ.
		else
		{
			// ¡§∫∏ º≥¡§
			CSHFamily::stINFO		stInfo;
			CSHFamily::stINFO_EX	stInfoEx;

			ZeroMemory(&stInfo,		sizeof(stInfo));
			ZeroMemory(&stInfoEx,	sizeof(stInfoEx));
			stInfo.nID				= atoi((char*)pData[0].Data[0]);
			SafeStrCpy(stInfo.szName, (char*)pData[0].Data[1], MAX_NAME_LENGTH+1);
			stInfo.nMasterID		= atoi((char*)pData[0].Data[2]);
			stInfoEx.nHonorPoint	= atoi((char*)pData[0].Data[3]);
			stInfoEx.nNicknameON	= atoi((char*)pData[0].Data[4]);

			CSHFamily csFamily;
			csFamily.Set(&stInfo, &stInfoEx);

			// ≈◊¿Ã∫Ìø° √ﬂ∞°
			pUserInfo->pcsFamily = g_csFamilyManager.AddFamilyToTbl(&csFamily);
			// ∫“∑Øø¬ ∆–π–∏Æ ¡§∫∏ ¡ﬂ ∆–π–∏ÆID ∑Œ ∆–π–∏Æ ∏‚πˆ µ•¿Ã≈Õ∏¶ ∫“∑Øø¬¥Ÿ.
			Family_Member_LoadInfo(pMessage->dwID, stInfo.nID);
			// ∏ º≠πˆø° ≥Û¿ÂUI ¡§∫∏ ø‰√ª
			g_csFarmManager.ASRV_RequestFarmUIInfoToMap(pUserInfo->dwCharacterID, pUserInfo->pcsFamily);
		}
	}
}

void Family_LoadEmblemInfo(DWORD nPlayerID, DWORD nFamilyID)
{
	sprintf(txt, "EXEC  %s %d", STORED_FAMILY_LOAD_INFO_EMBLEM, nFamilyID);
	g_DB.FreeLargeQuery(RFamily_LoadEmblemInfo, nPlayerID, txt);
}

int convertCharToInt(char c)
{
	if('0' <= c && c <= '9')
		return c - '0';
	if('A' <= c && c <= 'F')
		return c - 'A' +10;
	if('a' <= c && c <= 'f')
		return c - 'a' +10;
	return 0;
}

HexToByte(char* pStr)
{
	int n1 = convertCharToInt(pStr[0]);
	int n2 = convertCharToInt(pStr[1]);

	return n1 * 16 + n2;
}

void RFamily_LoadEmblemInfo(LPLARGEQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	if( pMessage->dwResult == 0 )
	{
	}
	else
	{
		char Img[CSHFamilyManager::EMBLEM_BUFSIZE];
		char *pStr = (char*)pData->Data[1];
		char tempBuf[3] = {0,};
		int curpos = 0;
		for(int n=0;n<CSHFamilyManager::EMBLEM_BUFSIZE;++n)
		{
			strncpy(tempBuf,&pStr[curpos],2); // "FF"
			Img[n] = HexToByte(tempBuf);
			curpos += 2;
		}

		g_csFamilyManager.ASRV_RegistEmblem(pUserInfo, atoi((char*)pData->Data[0]), Img, 1);
	}
}

void Family_Member_SaveInfo(DWORD nFamilyID, DWORD nMemberID, char* pszMemberNickname)
{
	if (pszMemberNickname)
	{
		sprintf(txt, "EXEC  %s %d, %d, '%s'", STORED_FAMILY_MEMBER_SAVE_INFO, nFamilyID, nMemberID, pszMemberNickname);
	}
	else
	{
		// æ∆∑°¿« NULL ¿∫ ±◊≥… ¿⁄∏Æºˆ∏¶ ∏¬√Á¡÷±‚ ¿ß«— ≈ÿΩ∫∆Æ¿”. æÓ∂≤ ∞… ≥÷æÓ≥ˆµµ «¡∑ŒΩ√¿˙ø°º≠
		// insert ø°¥¬ π›øµ«œ¡ˆ æ µµ∑œ µ«æÓ ¿÷¿Ω.
		sprintf(txt, "EXEC  %s %d, %d, NULL", STORED_FAMILY_MEMBER_SAVE_INFO, nFamilyID, nMemberID);
	}
	g_DB.Query(eQueryType_FreeQuery, eFamily_Member_SaveInfo, 0, txt);
}

void Family_Member_LoadInfo(DWORD nPlayerID, DWORD nFamilyID)
{
	sprintf(txt, "EXEC  %s %d", STORED_FAMILY_MEMBER_LOAD_INFO, nFamilyID);
	g_DB.Query(eQueryType_FreeQuery, eFamily_Member_LoadInfo, nPlayerID, txt);
}

void RFamily_Member_LoadInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	if( pMessage->dwResult == 0 )
	{
	}
	else
	{
		// RFamily_LoadInfo «‘ºˆø°º≠ √ﬂ∞°«— ∆–π–∏Æ µ•¿Ã≈Õø° ∏‚πˆ ≈◊¿Ã≈Õ∏¶ ¿‘∑¬«—¥Ÿ.
		//
		if (pUserInfo->pcsFamily == NULL) return;

		// ..∏‚πˆ µ•¿Ã≈Õ º≥¡§
		USERINFO* pMember;
		CSHFamilyMember				csMember;
		CSHFamilyMember::stINFO		stInfo;

		for(DWORD i=0; i<pMessage->dwResult; i++)
		{
			// ∏…πˆ ºˆ∞° √ ∞˙µ«æÓ DBø° ¿˙¿Âµ«æÓ ¿÷¿ª ∞ÊøÏ √≥∏Æ(∫Ò¡§ªÛ¿˚¿Œ ∞ÊøÏ)
			if (i >= pUserInfo->pcsFamily->GetMemberNumMax()) break;

			ZeroMemory(&stInfo, sizeof(stInfo));

			stInfo.nID = atoi((char*)pData[i].Data[0]);
			sprintf(stInfo.szName, "%d", stInfo.nID);
			SafeStrCpy(stInfo.szNickname, (char*)pData[i].Data[1], MAX_NAME_LENGTH+1);
			pMember = g_pUserTableForObjectID->FindUser(stInfo.nID);
			if (pMember)
			{
				stInfo.eConState = CSHFamilyMember::MEMBER_CONSTATE_LOGIN;
			}

			// ∏…πˆ º≥¡§
			((CSHGroupMember*)&csMember)->Set(&stInfo);
			pUserInfo->pcsFamily->SetMember(&csMember, i);

			// ∏…πˆ ºˆ ¡ı∞°
			CSHFamily::stINFO	stFamilyInfo = *pUserInfo->pcsFamily->Get();
			stFamilyInfo.nMemberNum++;
			((CSHGroup*)pUserInfo->pcsFamily)->Set(&stFamilyInfo);
		}
		// ..±‚∫ª ∆–π–∏Æ µ•¿Ã≈Õ ¿‘∑¬
		g_csFamilyManager.SetFamily(pUserInfo->pcsFamily);

		// ¥Ÿ∏• ≈◊¿Ã∫Ìø°º≠ ¬¸¡∂«ÿæﬂ «“ ¡§∫∏ ∫“∑Øø¿±‚
		for(UINT i=0; i<pUserInfo->pcsFamily->Get()->nMemberNum; i++)
		{
			Family_Member_LoadExInfo01(pUserInfo->dwCharacterID, pUserInfo->pcsFamily->GetMember(i)->Get()->nID);
		}
		// ¥Ÿ∏• ø°¿Ã¿¸∆Æø° æÀ∏Æ±‚
		g_csFamilyManager.ASRV_SendMemberConToOtherAgent(pUserInfo->pcsFamily, pUserInfo->dwCharacterID, CSHFamilyMember::MEMBER_CONSTATE_LOGIN);
		// πÆ¿Â ∑ŒµÂ
		Family_LoadEmblemInfo(pUserInfo->dwCharacterID, pUserInfo->pcsFamily->Get()->nID);
	}
}

void Family_Member_LoadExInfo01(DWORD nPlayerID, DWORD nMemberID)
{
	sprintf(txt, "EXEC  %s %d", STORED_FAMILY_MEMBER_LOAD_EXINFO_01, nMemberID);
	g_DB.Query(eQueryType_FreeQuery, eFamily_Member_LoadExInfo01, nPlayerID, txt);
}


void RFamily_Member_LoadExInfo01(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	if( pMessage->dwResult == 0 )
	{
	}
	else
	{
		if (pUserInfo->pcsFamily == NULL) return;

		// ¥Ÿ∏• ≈◊¿Ã∫Ì∑Œ∫Œ≈Õ¿« ¬¸¡∂ ¡§∫∏ º≥¡§
		// ..∏…πˆ º≥¡§
		DWORD nMemberID = atoi((char*)pData[0].Data[0]);
		for(UINT i=0; i<pUserInfo->pcsFamily->Get()->nMemberNum; i++)
		{
			if (nMemberID == pUserInfo->pcsFamily->GetMember(i)->Get()->nID)
			{
				CSHFamilyMember				csMember;
				CSHFamilyMember::stINFO		stInfo		= *pUserInfo->pcsFamily->GetMember(i)->Get();
				CSHFamilyMember::stINFO_EX	stInfoEx	= *pUserInfo->pcsFamily->GetMember(i)->GetEx();

				stInfo.nID			= atoi((char*)pData[0].Data[0]);
				SafeStrCpy(stInfo.szName, (char*)pData[0].Data[1], MAX_NAME_LENGTH+1);
				stInfo.nRace		= atoi((char*)pData[0].Data[2]);
				stInfo.nSex			= atoi((char*)pData[0].Data[3]);
				stInfo.nJobGrade	= atoi((char*)pData[0].Data[4]);
				int nJob[6] = {0};
				nJob[0]				= atoi((char*)pData[0].Data[5]);
				nJob[1]				= atoi((char*)pData[0].Data[6]);
				nJob[2]				= atoi((char*)pData[0].Data[7]);
				nJob[3]				= atoi((char*)pData[0].Data[8]);
				nJob[4] 			= atoi((char*)pData[0].Data[9]);
				nJob[5] 			= atoi((char*)pData[0].Data[10]);
				stInfo.nJobFirst	= nJob[0];
				stInfo.nJobCur		= nJob[stInfo.nJobGrade-1];
				stInfo.nLV			= atoi((char*)pData[0].Data[11]);
				SafeStrCpy(stInfoEx.szGuild, (char*)pData[0].Data[12], MAX_NAME_LENGTH+1);
				
				csMember.Set(&stInfo, &stInfoEx);
				pUserInfo->pcsFamily->SetMember(&csMember, i);

				// ∏∂Ω∫≈Õ ¿Ã∏ß º≥¡§
				if (stInfo.nID == pUserInfo->pcsFamily->Get()->nMasterID)
				{
					CSHFamily::stINFO stFamilyInfo = *pUserInfo->pcsFamily->Get();
					SafeStrCpy(stFamilyInfo.szMasterName, stInfo.szName, MAX_NAME_LENGTH+1);
					pUserInfo->pcsFamily->Set(&stFamilyInfo);
				}

				// ∏ º≠πˆø° »£ƒ™ æÀ∏Æ±‚
				if (pMessage->dwID == pUserInfo->pcsFamily->GetMember(i)->Get()->nID)
				{
					g_csFamilyManager.ASRV_SendNicknameToMap(pUserInfo, pUserInfo->pcsFamily->GetMember(i)->Get()->szNickname);
				}

				break;
			}
		}

		for(UINT i=0; i<pUserInfo->pcsFamily->Get()->nMemberNum; i++)
		{
			// ªË¡¶µ«∞≈≥™ ¿Ø»ø«œ¡ˆ æ ¿∫ ∏‚πˆ ¡¶∞≈
			if (pUserInfo->pcsFamily->GetMember(i)->Get()->szName[0] == '@')
			{
				Family_Leave_SaveInfo(pUserInfo->pcsFamily->GetMember(i)->Get()->nID, CSHFamilyManager::FLK_LEAVE, TRUE);
				pUserInfo->pcsFamily->DelMember(pUserInfo->pcsFamily->GetMember(i)->Get()->nID);
				i--;
			}
		}

		// ..√÷¡æ ∆–π–∏Æ µ•¿Ã≈Õ ¿‘∑¬
		g_csFamilyManager.SetFamily(pUserInfo->pcsFamily);
		// ≈¨∂Û¿Ãæ∆Æø° ¡§∫∏ ∫∏≥ª±‚
		g_csFamilyManager.ASRV_SendInfoToClient(pUserInfo->pcsFamily);
	}
}

void Family_CheckName(DWORD nPlayerID, char* pszName)
{
	// ∆–π–∏Æ ¿Ã∏ß¿ª '' ∑Œ æ» π≠æÓ¡÷¥œ±Ó 1231∆–π–∏Æ øÕ ∞∞¿∫ º¯º≠∑Œ º˝¿⁄ «—±€¡∂«’(øµπÆ¿∫ »Æ¿Œ æ» «ÿ∫√¿Ω)¿ª πÆ¿⁄ø≠∑Œ ¿ŒΩƒ«œ¡ˆ æ ¥¬¥Ÿ.
	// SQLº≠πˆø°º≠ º˝¿⁄∞° ∏’¿˙ ≥™ø¿∏È πÆ¿⁄ø≠∑Œ ¿ŒΩƒ«œ¡ˆ æ ¥¬ µÌ...
	sprintf(txt, "EXEC  %s '%s'", STORED_FAMILY_CHECKNAME, pszName);
	g_DB.Query(eQueryType_FreeQuery, eFamily_CheckName, nPlayerID, txt);
}

void RFamily_CheckName(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	if( stricmp((char*)pData[0].Data[0], "") == 0 )
	{
		// ¿ÃπÃ DBø° ¡∏¿Á«œ¥¬ ¿Ã∏ß¿Ã¥Ÿ..
		g_csFamilyManager.ASRV_CreateFamily(pUserInfo, NULL, CSHFamilyManager::FCS_NAME_ERR);
	}
	else
	{
		// DBø° æ¯¥¬ ¿Ã∏ß. ªÁøÎ«“ ºˆ ¿÷¥Ÿ. ¥ŸΩ√ ∆–π–∏Æ ª˝º∫ Ω√µµ
		g_csFamilyManager.ASRV_CreateFamily(pUserInfo, (char*)pData[0].Data[0], CSHFamilyManager::FCS_NAME_CHECK2);
	}
}

void Family_Leave_SaveInfo(DWORD nPlayerID, int nLeaveKind, BOOL bInit, BOOL bBreakUp)
{
	SYSTEMTIME t;
	GetLocalTime(&t);

	DWORD nLeaveDate = (t.wYear-2000)*100000000 + t.wMonth*1000000 + t.wDay*10000 + t.wHour*100 + t.wMinute;
	if (bInit) nLeaveDate = 0;

	sprintf(txt, "EXEC  %s %d, %u, %d, %d", STORED_FAMILY_LEAVE_SAVE_INFO, nPlayerID, nLeaveDate, nLeaveKind, bBreakUp);
	g_DB.Query(eQueryType_FreeQuery, eFamily_Leave_SaveInfo, 0, txt);
}

void Family_Leave_LoadInfo(DWORD nPlayerID)
{
	sprintf(txt, "EXEC  %s %d", STORED_FAMILY_LEAVE_LOAD_INFO, nPlayerID);
	g_DB.Query(eQueryType_FreeQuery, eFamily_Leave_LoadInfo, nPlayerID, txt);
}

void RFamily_Leave_LoadInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	if( pMessage->dwResult == 0 )
	{
	}
	else
	{
		pUserInfo->nFamilyLeaveDate = atoi((char*)pData[0].Data[0]);
		pUserInfo->nFamilyLeaveKind = atoi((char*)pData[0].Data[1]);
	}
}

// E ∆–π–∏Æ √ﬂ∞° added by hseos 2007.07.09	2007.07.10	2007.07.14	2007.10.11

// desc_hseos_≥Û¿ÂΩ√Ω∫≈€_01
// S ≥Û¿ÂΩ√Ω∫≈€ √ﬂ∞° added by hseos 2008.01.22
void Farm_SendNote_DelFarm(DWORD nFarmOwnerID)
{
	sprintf(txt, "EXEC  dbo.MP_FARM_SendNote_DelFarm %d", nFarmOwnerID);
	g_DB.Query(eQueryType_FreeQuery, eFarm_SendNote_DelFarm, nFarmOwnerID, txt);
}

void RFarm_SendNote_DelFarm(LPQUERY pData, LPDBMESSAGE pMessage)
{
	// ∆–π–∏Æ ∏‚πˆ ¿Ã∏ß¿ª æÚæÓº≠ ¬ ¡ˆ∏¶ ∫∏≥Ω¥Ÿ.
	for(DWORD i=0; i<pMessage->dwResult; i++)
	{
		NoteSendtoPlayer(0, "<SYSTEM>", (char*)pData[0].Data[0], "61");
	}
}
// E ≥Û¿ÂΩ√Ω∫≈€ √ﬂ∞° added by hseos 2008.01.22

#ifdef _NPROTECT_
void NProtectBlock(DWORD UserIdx, DWORD CharIdx, char* IP, DWORD BlockType)
{
    char txt[128];
	sprintf(txt, "EXEC dbo.UP_NProtectBlockLog %d, %d, \'%s\', %d, %d", UserIdx, CharIdx, IP, BlockType, 6 );
	g_DB.LoginQuery( eQueryType_FreeQuery, eNProtectBlock, 0, txt);
}
#endif


void InsertLogFamilyPoint( CSHFamily* family, eFamilyLog type )
{
	if( ! family )
	{
		ASSERT( 0 );
		return;
	}

	const CSHFamily::stINFO_EX*	extendedData	= family->GetEx();
	const CSHFamily::stINFO*	data			= family->Get();

	if( ! data ||
		! extendedData )
	{
		ASSERT( 0 );
		return;
	}

	g_DB.LogMiddleQuery( 0, 0, "EXEC DBO.TP_FAMILY_POINT_LOG_INSERT %d, %d, %d, %d, %d, \'""\'",
		data->nID,
		extendedData->nHonorPoint,
		0,
		0,
		type );
}


//---KES PUNISH
void PunishListAdd( DWORD dwUserIdx, int nPunishKind, DWORD dwPunishTime )
{
	sprintf(txt, "EXEC dbo.MP_PunishList_Add %u, %d, %u", dwUserIdx, nPunishKind, dwPunishTime );
	g_DB.LoginQuery( eQueryType_FreeQuery, ePunishList_Add, dwUserIdx, txt);
}

void PunishListLoad( DWORD dwUserIdx )
{
	sprintf(txt, "EXEC dbo.MP_PunishList_Load %u", dwUserIdx );
	g_DB.LoginQuery( eQueryType_FreeQuery, ePunishList_Load, dwUserIdx, txt);
}

void RPunishListLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	USERINFO* pUserInfo = g_pUserTableForUserID->FindUser( pMessage->dwID );
	if( pUserInfo == NULL ) return;

	int nCount = pMessage->dwResult;
	if( nCount == 0 ) return;
	
	for( int i = 0 ; i < nCount ; ++i )
	{
		int nPunishTime = atoi( (char*)pData[i].Data[1] );
		if( nPunishTime > 0 ) 
			PUNISHMGR->AddPunishUnit( pUserInfo->dwUserID, atoi((char*)pData[i].Data[0]), nPunishTime );
	}
}

void PunishCountAdd( DWORD dwUserIdx, int nPunishKind, int nPeriodDay )
{
	sprintf(txt, "EXEC dbo.MP_PunishCount_Add %u, %d, %d", dwUserIdx, nPunishKind, nPeriodDay );
	g_DB.LoginQuery( eQueryType_FreeQuery, ePunishCount_Add, dwUserIdx, txt);
}

void RPunishCountAdd( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( pMessage->dwResult == NULL ) return;

	int nPunishKind		= atoi((char*)pData[0].Data[0]);
	int nCount			= atoi((char*)pData[0].Data[1]);

	if( nCount == 0 ) return;

	switch( nPunishKind )
	{
	case ePunishCount_AutoUse:
		{
			PunishListAdd( pMessage->dwID, ePunish_Login, POW( 2, nCount-1 ) * 1 * 60 * 60 );	//1Ω√∞£∫Œ≈Õ ¥ı∫Ì∑Œ!
		}
		break;
	};
}

void GiftEvent( DWORD dwUserIdx )
{
	sprintf(txt, "EXEC dbo.up_event20080425_attend_check %u, %d", dwUserIdx, g_nServerSetNum );
	g_DB.LoginQuery( eQueryType_FreeQuery, eGiftEvent, dwUserIdx, txt);
}

void RGiftEvent( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( pMessage->dwResult == NULL ) return;

	USERINFO* pUserInfo = g_pUserTableForUserID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	int result = atoi((char*)pData[0].Data[0]);
	DWORD eventidx = 0;

	switch( result )
	{
	case 1:
		{
			eventidx = 6;
		}
		break;
	case 2:
		{
			eventidx = 7;
		}
		break;
	case 3:
		{
			eventidx = 8;
		}
		break;
	}

	GIFTMGR->ExcuteEvent( pUserInfo->dwCharacterID, eventidx );
}



// 081205 LUJ, ∆–π–∏Æ ∆˜¿Œ∆Æ ∑Œ±◊
void InsertLogFamily( eFamilyLog logType, DWORD familyIndex, DWORD playerIndex, const char* memo )
{
	char buffer[ MAX_NAME_LENGTH + 1 ] = { 0 };
	SafeStrCpy( buffer, memo, sizeof( buffer ) / sizeof( *buffer ) );

	g_DB.LogMiddleQuery(
		0,
		0,
		"EXEC dbo.TP_FAMILY_LOG_INSERT %d, %d, %d, \'%s\'",
		logType,
		familyIndex,
		playerIndex,
		memo );
}