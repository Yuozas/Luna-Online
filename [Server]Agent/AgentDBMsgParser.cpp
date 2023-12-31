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

// desc_hseos_패밀리01
// S 패밀리 추가 added by hseos 2007.07.09
#include "../hseos/Family/SHFamilyManager.h"
#include "../hseos/Farm/SHFarmManager.h"
// E 패밀리 추가 added by hseos 2007.07.09


//---KES PUNISH
#include "PunishManager.h"
//-------------

// 080507 LYW --- AgentDBMsgParser : 채팅방 매니져 헤더 포함.
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
// DB횆천쨍짰 쨍짰횇횕 횉횚쩌철째징 쨈횄쩐챤쨀짱쨋짠쨍쨋쨈횢 횄횩째징
// enum Query ?횉 쩌첩쩌짯째징 쨔횦쨉책쩍횄!!!! ?횕횆징횉횠쩐횩 횉횗쨈횢.
DBMsgFunc g_DBMsgFunc[MaxQuery] =
{
	NULL,
	RUserIDXSendAndCharacterBaseInfo,	// 횆쨀쨍짱횇횒 쨍짰쩍쨘횈짰 Query
	RCreateCharacter,
	NULL,
	RDeleteCharacter,
	RCharacterNameCheck,
	NULL,
	NULL,
	RSearchWhisperUserAndSend, /// 횆쨀쨍짱쨍챠?쨍쨌횓 횁짖쩌횙쩔짤쨘횓쨔횞 쨍횎쨔첩횊짙 쩐챵쩐챤쩔?짹창
	NULL,							//SavePoint 
	NULL,							/// 쨍횎쩔징쩌짯 쨀짧째징쨍챕 쩍횉횉횪횉횗쨈횢
	NULL,							/// 쨍횎쩌짯쨔철 횁쩐쨌찼쩍횄 쩍횉횉횪
	RFriendIsValidTarget,		//FriendGetUserIDXbyName
	RFriendAddFriend,
	RFriendIsValidTarget,
	RFriendDelFriend,
	RFriendDelFriendID,
	NULL, //횆짙짹쨍쨍짰쩍쨘횈짰 쨩챔횁짝(횆쨀쨍짱횇횒 쨩챔횁짝쩍횄)
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
	// desc_hseos_성별선택01
	// S 성별선택 추가 added by hseos 2007.06.15
	NULL,							// eInitMonstermeterInfoOfDB
	RGetUserSexKind,				// eGetUserSexKind
	// E 성별선택 추가 added by hseos 2007.06.15

	// desc_hseos_패밀리01
	// S 패밀리 추가 added by hseos 2007.07.09	2007.07.10	2007.07.14	2007.10.11
	RFamily_LoadInfo,					// eFamily_LoadInfo
	RFamily_SaveInfo,					// eFamily_SaveInfo
	RFamily_Member_LoadInfo,			// eFamily_Member_LoadInfo
	RFamily_Member_LoadExInfo01,		// eFamily_Member_LoadExInfo01,
	NULL,								// eFamily_Member_SaveInfo
	RFamily_CheckName,					// eFamily_CheckName
	RFamily_Leave_LoadInfo,				// eFamily_Leave_LoadInfo
	NULL,								// eFamily_Leave_SaveInfo
	// E 패밀리 추가 added by hseos 2007.07.09	2007.07.10	2007.07.14	2007.10.11
	NULL,								// eChange_Character_Name

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2008.01.22
	RFarm_SendNote_DelFarm,				// eFarm_SendNote_DelFarm
	// E 농장시스템 추가 added by hseos 2008.01.22

//---KES PUNISH
	NULL,								//ePunishList_Add,	//---제재목록
	RPunishListLoad,					//ePunishList_Load,	//---제재목록
	RPunishCountAdd,					//ePunishCount_Add, //---걸린 회수 추가

	RGiftEvent,							//eGiftEvent,
//-------------

};	

// 횆쨈쨍짱횇횒 쨍짰쩍쨘횈짰 째징횁짰쩔?쨈횂 DB횆천쨍짰
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
	// desc_hseos_프로시저정리01
	// S 프로시저정리 추가 added by hseos 2007.06.21
	return;
	// E 프로시저정리 추가 added by hseos 2007.06.21
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
		if(i == MAX_CHARACTER_NUM - 1)	// ?짱?첬째징 째징횁철째챠 ?횜쨈횂 횆쨀쨍짱째첬 쨈횢쨍짜 횆쨀쨍짱?횑 쩌짹횇횄쨉횎
		{
			// 쩔짭째찼쨉횊 횆쨀쨍짱째첬 횁철쩔챦횆쨀쨍짱?횑 ?횕횆징횉횕횁철 쩐횎쩍?쨈횕쨈횢
			return;
		}
	}

	char ip[16];
	WORD port;
	g_Network.GetUserAddress( dwConnectionIndex, ip, &port );

	//횈횆횈쩌쨀짧 쨔짰횈횆?책?횑쨍챕  쩔징쨌짱횆횣쨉책 쨍짰횇횕
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

void FriendAddFriend(DWORD CharacterIDX, DWORD TargetID) //CharacterIDX : 쩍횇횄쨩?횓, TargetID : 쩍횂쨀짬?횓
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
	g_DB.FreeQuery(eFriendNotifyLogin, CharacterIDX, "EXEC %s %u", "MP_FRIEND_NotifyLogin", CharacterIDX); //쨀짧쨍짝 쨉챤쨌횕횉횗 쨩챌쨋첨쨉챕
}

void FriendGetLoginFriends(DWORD CharacterIDX)
{
	g_DB.FreeQuery(eFriendGetLoginFriends, CharacterIDX, "EXEC %s %u", "MP_FRIEND_LoginFriend", CharacterIDX);//쨀쨩째징 쨉챤쨌횕횉횗 쨩챌쨋첨쨉챕
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
	g_DB.FreeQuery(eNoteList, CharacterIDX, "EXEC %s %u, %d, %u, %d", "MP_NOTE_GetNoteList", CharacterIDX, 10, Page, Mode); //10째쨀쩐쩔 쨔횧쩐횈쩔횂쨈횢. 
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
	if(pInfo == NULL)		// ?횑쨔횑 쨀짧째짭?쩍
		return;
	if(pInfo->dwUniqueConnectIdx != AgentAuthKey)	// 쨀짧째징째챠 쨈횢쨍짜쨀횗?횑 쨉챕쩐챤쩔횊
		return;

	if(count > dMAXCHARACTER)	// 횆쨀쨍짱횇횒 횁짚쨘쨍쨔횧짹창 쩍횉횈횖
	{
		count = dMAXCHARACTER;
		char temp[256];
		sprintf(temp,"횆횋쨍짱횇횒째징 5쨍챠 ?횑쨩처?횑쩐첬?쩍!!! useridx : %d", pInfo->dwUserID );
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
	if( !count ) /// 쩐횈횁첨 쨍쨍쨉챌 횆쨀쨍짱횇횒째징 쩐첩쨈횢.
	{
		msg.CharNum = 0;			// ?횑 횆짬쩔챤횈짰째징 0?횑쨍챕 횆쨀쨍짱?횑 횉횕쨀짧쨉횓 쩐첩쨈횂째횒?횑쨈횢

		g_Network.Send2User(pMessage->dwID, (char*)&msg, sizeof(SEND_CHARSELECT_INFO));

#ifdef _CRYPTCHECK_
		pInfo->crypto.SetInit( TRUE );		// init on	
#endif

#ifdef _NPROTECT_
		//이곳은 여러군데서 들어올수도 있다.(첫로그인시, 캐릭터생성창에서, 게임에서)
		if( pInfo->m_nCSAInit == 0 )	//첫 인증이 안되어 있다면
		{
			pInfo->m_nCSAInit = 1;		//첫 인증 시작
			NPROTECTMGR->SendAuthQuery(pInfo);

		}
#endif

		return;
	}

	msg.CharNum = (BYTE)(count);
		
	for( DWORD i=0; i<count; i++)
	{
		// 횆쨀쨍짱횇횒 짹창쨘쨩횁짚쨘쨍 쩌횂횈횄횉횕쨈횂째첨
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

		// desc_hseos_결혼_01
		// S 결혼 추가 added by hseos 2007.12.13
		ZeroMemory(msg.ChrTotalInfo[i].szMarriagePartnerName,	  sizeof(msg.ChrTotalInfo[i].szMarriagePartnerName));
		ZeroMemory(msg.ChrTotalInfo[i].szMarriagePartnerNickName, sizeof(msg.ChrTotalInfo[i].szMarriagePartnerNickName));
		// E 결혼 추가 added by hseos 2007.12.13

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

		// 장비숨기기 기능은 끈다.
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
		//이곳은 여러군데서 들어올수도 있다.(첫로그인시, 캐릭터생성창에서, 게임에서)
		if( pInfo->m_nCSAInit == 0 )	//첫 인증이 안되어 있다면
		{
			pInfo->m_nCSAInit = 1;		//첫 인증 시작
			NPROTECTMGR->SendAuthQuery(pInfo);
		}
#endif
}

/// 케릭터 생성시에 사용
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

// desc_hseos_몬스터미터01
// S 몬스터미터 추가 added by hseos 2007.05.29
// ..플레이시간, 몬스터킬 수 초기화
void InitMonstermeterInfoOfDB(DWORD nPlayerID)
{
	char txt[128];
	sprintf(txt, "EXEC  dbo.MP_MONSTERMETER_InitInfo %d", nPlayerID);
	g_DB.Query(eQueryType_FreeQuery, eInitMonstermeterInfoOfDB, 0, txt);
}
// E 몬스터미터 추가 added by hseos 2007.05.29

// desc_hseos_성별선택01
// S 성별선택 추가 added by hseos 2007.06.15
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

	// 클라이언트에 알리기
	MSG_DWORD msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_USER_SEXKIND;
	msg.dwData	 = result;
	g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
}
// E 성별선택 추가 added by hseos 2007.06.15

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
				ASSERTMSG(0, "UserID째징 0?횑쨈횢.");

				MSGBASE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_CHARACTER_MAKE_NACK;
				g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
				return;
			}
			
			/// 기본 생성 아이템

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

			// 080430 LYW --- AgentDBMsgParser : 채팅방 시스템으로 인해, 캐릭터 이름도 추가함.
			pInfo->SelectInfoArray[ MAX_CHARACTER_NUM - 1 ].Level = 1 ;
			SafeStrCpy(pInfo->SelectInfoArray[ MAX_CHARACTER_NUM - 1 ].name, (char*)pData->Data[3], 32) ;

			// 080507 LYW --- AgentDBMsgParser : 채팅방 시스템을 위해, 유저 정보를 Dist 서버에 추가한다.
			ST_CR_USER user ;
			memset(&user, 0, sizeof(ST_CR_USER)) ;

			user.byLevel	= pInfo->SelectInfoArray[ MAX_CHARACTER_NUM - 1 ].Level ;
			user.byMapNum	= pInfo->SelectInfoArray[ MAX_CHARACTER_NUM - 1].MapNum ;
			user.dwPlayerID	= pInfo->SelectInfoArray[ MAX_CHARACTER_NUM - 1].dwCharacterID ;
			user.dwUserID	= pInfo->dwUserID ;

			SafeStrCpy(user.name, pInfo->SelectInfoArray[ MAX_CHARACTER_NUM - 1 ].name, MAX_NAME_LENGTH) ;

			// 유저 정보를 Dist로 등록시키고, 유저가 등록 된 상태로 세팅한다.
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
			// 횆쨀쨍짱횇횒째징 짼횏횄징쨈횢.
			MSGBASE msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHARACTER_MAKE_NACK;
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	case 2:
		{
			// ?횑쨍짠?횑 횁횩쨘쨔쨉횎 쩔?쨌첫
			MSG_WORD msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHARACTER_NAMECHECK_NACK;
			msg.wData = result;
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	case 3:
		{
			// ?횑쨍짠?횑 NULL?횕쨋짠
			MSG_WORD msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHARACTER_NAMECHECK_NACK;
			msg.wData = result;
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	case 4:
		{
			// 쨔짰횈횆 ?횑쨍짠째첬 째찾횆짜 쨋짠
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
		// ?횑쨍짠 횁횩쨘쨔 쩐횊쨉횎
		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTER_NAMECHECK_ACK;
		g_Network.Send2User(pMessage->dwID, (char*)&msg, sizeof(msg));
	}
	else
	{
		// ?횑쨍짠?횑 횁횩쨘쨔쨉횎 쩔?쨌첫
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
			
			if( pReceiverInfo )	//쨔횧?쨩 쨩챌쨋첨?횑 ?횑 쩌짯쨔철쩔징 ?횜쨈횂횁철 째횏쨩챌
			{
				if( pReceiverInfo->GameOption.bNoWhisper && pSenderInfo->UserLevel != eUSERLEVEL_GM )
				{
					MSG_BYTE msg;
					msg.Category	= MP_CHAT;
					msg.Protocol	= MP_CHAT_WHISPER_NACK;
					msg.dwObjectID	= pMsgChat->dwObjectID;	//쨘쨍쨀쩍쨩챌쨋첨 쩐횈?횑쨉챨
					msg.bData		= CHATERR_OPTION_NOWHISPER;

					g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
				}
				else
				{
					//쨘쨍쨀쩍 쨩챌쨋첨쩔징째횚 쨘쨍쨀쨩째챠,
					MSG_CHAT msgToSender = *pMsgChat;
					msgToSender.Category = MP_CHAT;
					msgToSender.Protocol = MP_CHAT_WHISPER_ACK;
					g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msgToSender, msgToSender.GetMsgLength() );	//CHATMSG 040324

					//쨔횧쨈횂 쨩챌쨋첨쩔징째횚 쨘쨍쨀쨩째챠,
					MSG_CHAT msgToReceiver = *pMsgChat;
					msgToReceiver.Category = MP_CHAT;
					if( pSenderInfo->UserLevel == eUSERLEVEL_GM )
						msgToReceiver.Protocol = MP_CHAT_WHISPER_GM;
					else
						msgToReceiver.Protocol = MP_CHAT_WHISPER;
					SafeStrCpy( msgToReceiver.Name, (char*)pData->Data[0], MAX_NAME_LENGTH + 1 );	//쨘쨍쨀쩍쨩챌쨋첨?횉 ?횑쨍짠?쨍쨌횓 쨔횢짼횧
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

				msgWhisper.dwObjectID	= pMsgChat->dwObjectID;					//쨘쨍쨀쩍쨩챌쨋첨
				msgWhisper.dwReceiverID	= (DWORD)atoi((char*)pData->Data[1]);	//쨔횧쨈횂쨩챌쨋첨
				SafeStrCpy( msgWhisper.SenderName, (char*)pData->Data[0], MAX_NAME_LENGTH + 1 ); //쨘쨍쨀쩍쨩챌쨋첨?횉 ?횑쨍짠
				SafeStrCpy( msgWhisper.ReceiverName, pMsgChat->Name, MAX_NAME_LENGTH + 1 ); //쨔횧쨈횂쨩챌쨋첨?횉 ?횑쨍짠
				SafeStrCpy( msgWhisper.Msg, pMsgChat->Msg, MAX_CHAT_LENGTH + 1 );	//횄짚횈횄쨀쨩쩔챘

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
			msg.dwData = atoi((char*)pData->Data[eFr_IsSuccess]); //ack ?횕쨋짠 friendidx return
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
	//쨉챤쨌횕 쩍횇횄쨩횉횗 쨩챌쨋첨
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
		//쨈횢쨍짜 쩔징?횑?체횈짰쩔징 ?횜쨈횢. 
		if(atoi((char*)pData->Data[eFr_addFromErr]) == 0)
			msg.Protocol = MP_FRIEND_ADD_ACK_TO_AGENT;
		else
		{
			msg.PlayerID = atoi((char*)pData->Data[eFr_addFromErr]);
			msg.Protocol = MP_FRIEND_ADD_NACK_TO_AGENT;
		}
		g_Network.Broadcast2AgentServerExceptSelf((char*)&msg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
	}
	
	//쨉챤쨌횕쨉횉쩐챤횁철쨈횂 쨩챌쨋첨
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
			// 쨈횢쨍짜 Agent쩔징 쩌횙횉횠?횜쨈횢.
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
			//쨈횢쨍짜 쩔징?횑?체횈짰
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
	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2008.01.18
	// 농장 해체 시 시스템 메세지를 보내므로 쪽지 받음 메세지는 없어도 될 듯~
	/*
	// ..시스템 쪽지의 경우 ID 가 0 이고 보내는 이름이 <SYSTEM> 이다
	if (pMessage->dwID == 0)
	{
		// 시스템 쪽지 전송이 성공하면 받는 플레이어에게만 알린다.
		if (atoi((char*)pData->Data[eFr_NErr]) == 0)
		{
			DWORD Toidx = atoi((char*)pData->Data[eFr_NToId]);
			if(Toidx == 0) //쨌횓짹횞쩐횈쩔척 쨩처횇횂
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
			else //쨈횢쨍짜 쩔징?횑?체횈짰쩔징 ?횜쨈횢. 
			{
				g_Network.Broadcast2AgentServerExceptSelf( (char*)&rmsg, sizeof(rmsg) );
			}
		}
		return;
	}
	*/
	// E 농장시스템 추가 added by hseos 2008.01.18

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

	//횂횎횁철 쨔횧쨈횂 쨩챌쨋첨
	DWORD Toidx = atoi((char*)pData->Data[eFr_NToId]);
	if(Toidx == 0) //쨌횓짹횞쩐횈쩔척 쨩처횇횂
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
	else //쨈횢쨍짜 쩔징?횑?체횈짰쩔징 ?횜쨈횢. 
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

	//횂횎횁철 쨔횧쨈횂 쨩챌쨋첨
	DWORD Toidx = atoi((char*)pData->Data[eFr_NToId]);
	if(Toidx == 0) //쨌횓짹횞쩐횈쩔척 쨩처횇횂
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
	else //쨈횢쨍짜 쩔징?횑?체횈짰쩔징 ?횜쨈횢. 
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
			//.SendData쨔철횈횤째징 16?횑쨈횢. 	//KES
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
		//횉철쨩처짹횦 횁철쩔챵
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
	if(atoi((char*)pData->Data[0]) == 0) //횉철쨩처짹횦쩔징 째체쨌횄쨉횊째횚 쩐첩쨈횢.
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
		//횄횩횊횆 ?횤쩐첨
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
		//(DWORD)atoi((char*)pData->Data[2])	: 쨍횎쨔첩횊짙
		//(DWORD)atoi((char*)pData->Data[1])	: 횄짙?쨘 objectID

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
				msg.dwData		= dwTargetID;	//횄짙?쨘 쩐횈?횑쨉챨

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
		//(char*)pData->Data[0]					: 쨘쨍쨀쩍쨩챌쨋첨 ?횑쨍짠
		//(DWORD)atoi((char*)pData->Data[1])	: 횄짙?쨘 objectID

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
		// 짹횞쨌짹 ?횑쨍짠 쩐첩쩐챤~~
		msg.Protocol = MP_CHEAT_BLOCKCHARACTER_NACK;
	}
	else
	{
		msg.Protocol = MP_CHEAT_BLOCKCHARACTER_ACK;
		// 쩐첨쨉짜?횑횈짰 쩌쨘째첩~~

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

	if( count == 0 || pUserInfo == NULL ) // ?횑쨩처 쩔?쨌첫
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

	if( check != 0 ) // 횁짖쩌횙 쨘횘째징
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


/* --; 횉횎쩔채쩐첩째횣쨀횩. ?횩쨍첩횂짜쨉청.
void RGM_MoveToCharacter(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	if(!count)
	{

	}
	else
	{
		
		//(DWORD)atoi((char*)pData->Data[1])	: 횄짙?쨘 objectID
		//(char*)pData->Data[0]					: 쨘쨍쨀쩍쨩챌쨋첨 ?횑쨍짠

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

			//?횑쩌짯쨔철쩔징 ?횜쨀짧?
			if( pTargetInfo )
			{
				//---쨀짧횁횩쩔징 gm쨍챠쨌횋?쨘 쨈횢쨍짜쨉짜쨌횓....
				//obejctid쨍짝 쨍횎쩌짯쨔철쨌횓 쨘쨍쨀쨩?횣!

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
{	//총금액 관리 하는 AGENT 가 아니면 받지못함
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
	// 로그인 안했을 경우
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
	// 척살목록에 없는 경우
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
		// 짹횞쨌짹 ?횑쨍짠 쩐첩쩐챤~~
		msg.Protocol = MP_CHEAT_BLOCKCHARACTER_NACK;
	}
	else
	{
		msg.Protocol = MP_CHEAT_BLOCKCHARACTER_ACK;
		// 쩐첨쨉짜?횑횈짰 쩌쨘째첩~~
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

// desc_hseos_패밀리01
// S 패밀리 추가 added by hseos 2007.07.09	2007.07.10	2007.07.14	2007.10.11
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
		// 패밀리 이름을 '' 로 안 묶어주니까 1231패밀리 와 같은 순서로 숫자 한글조합(영문은 확인 안 해봤음)을 문자열로 인식하지 않는다.
		// SQL서버에서 숫자가 먼저 나오면 문자열로 인식하지 않는 듯...
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
	// 패밀리 생성 시 패밀리의 ID를 받기 위해.. 
	// ..패밀리를 처음 생성 시에는 DB에 저장한 후 실제로 패밀리를 생성한다. 생성 후 저장시에는 Update 만 하기 때문에 여기로 오지 않는다.
	// ..사실 ID만 받아서 설정하면 되는데, 받는 김에 모든 정보를 다 받아서 받은 정보로 설정!!!
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	if( pMessage->dwResult == 0 )
	{
	}
	else
	{
		// 패밀리 정보
		CSHFamily::stINFO		stInfo;
		CSHFamily::stINFO_EX	stInfoEx;

		ZeroMemory(&stInfo, sizeof(stInfo));
		ZeroMemory(&stInfoEx, sizeof(stInfoEx));
		stInfo.nID				= atoi((char*)pData[0].Data[0]);
		SafeStrCpy(stInfo.szName, (char*)pData[0].Data[1], MAX_NAME_LENGTH+1);
		stInfo.nMasterID		= atoi((char*)pData[0].Data[2]);
		stInfoEx.nHonorPoint	= atoi((char*)pData[0].Data[3]);
		stInfoEx.nNicknameON	= atoi((char*)pData[0].Data[4]);

		// 멤버(자신) 정보 (접속 상태)
		CSHFamilyMember::stINFO		stMemberInfo;

		ZeroMemory(&stMemberInfo, sizeof(stMemberInfo));
		stMemberInfo.nID		= pUserInfo->dwCharacterID;
		stMemberInfo.eConState	= CSHFamilyMember::MEMBER_CONSTATE_LOGIN;

		CSHFamilyMember csFamilyMember;
		csFamilyMember.Set(&stMemberInfo);

		// 최종 패밀리 생성
		CSHFamily csFamily;
		csFamily.Set(&stInfo, &stInfoEx);
		g_csFamilyManager.ASRV_CreateFamily(pUserInfo, stInfo.szName, CSHFamilyManager::FCS_COMPLETE, &csFamily);

		// 마스터 정보 읽기
		Family_Member_LoadInfo(pUserInfo->dwCharacterID, stInfo.nID);
		// 081205 LUJ, 로그
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
		// 이미 테이블에 있다면 포인터만 가져온다.
		CSHFamily* pcsAleadyFamily = g_csFamilyManager.GetFamily(atoi((char*)pData[0].Data[0]));
		if (pcsAleadyFamily)
		{
			pUserInfo->pcsFamily = pcsAleadyFamily;
			// 접속 상태 설정
			int nLoginCnt = 0;
			for(UINT i=0; i<pUserInfo->pcsFamily->Get()->nMemberNum; i++)
			{
				if (pUserInfo->dwCharacterID == pUserInfo->pcsFamily->GetMember(i)->Get()->nID)
				{
					CSHFamilyMember::stINFO stInfo = *pUserInfo->pcsFamily->GetMember(i)->Get();
					
					stInfo.eConState = CSHFamilyMember::MEMBER_CONSTATE_LOGIN;
					pUserInfo->pcsFamily->GetMember(i)->Set(&stInfo);

					// 맵서버에 호칭 알리기
					g_csFamilyManager.ASRV_SendNicknameToMap(pUserInfo, pUserInfo->pcsFamily->GetMember(i)->Get()->szNickname);
				}

				if (pUserInfo->pcsFamily->GetMember(i)->Get()->eConState == CSHFamilyMember::MEMBER_CONSTATE_LOGIN)
				{
					nLoginCnt++;
				}
			}

			// 현재 접속해 있는 멤버가 2명이 되면 명예 포인트 시간을 설정한다.
			if (nLoginCnt == CSHFamilyManager::HONOR_POINT_CHECK_MEMBER_NUM)
			{
				CSHFamily::stINFO_EX stInfoEx = *pUserInfo->pcsFamily->GetEx();
				stInfoEx.nHonorPointTimeTick = gCurTime;
				pUserInfo->pcsFamily->SetEx(&stInfoEx);
			}

			// 다른 에이전트에 알리기
			g_csFamilyManager.ASRV_SendMemberConToOtherAgent(pUserInfo->pcsFamily, pUserInfo->dwCharacterID, CSHFamilyMember::MEMBER_CONSTATE_LOGIN);
			// 클라이언트에 정보 보내기
			g_csFamilyManager.ASRV_SendInfoToClient(pUserInfo->pcsFamily);
			// 문장 로드
			Family_LoadEmblemInfo(pUserInfo->dwCharacterID, pUserInfo->pcsFamily->Get()->nID);
			// 맵서버에 농장UI 정보 요청
			g_csFarmManager.ASRV_RequestFarmUIInfoToMap(pUserInfo->dwCharacterID, pUserInfo->pcsFamily);
		}
		// 패밀리 테이블에 추가한다.
		else
		{
			// 정보 설정
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

			// 테이블에 추가
			pUserInfo->pcsFamily = g_csFamilyManager.AddFamilyToTbl(&csFamily);
			// 불러온 패밀리 정보 중 패밀리ID 로 패밀리 멤버 데이터를 불러온다.
			Family_Member_LoadInfo(pMessage->dwID, stInfo.nID);
			// 맵서버에 농장UI 정보 요청
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
		// 아래의 NULL 은 그냥 자리수를 맞춰주기 위한 텍스트임. 어떤 걸 넣어놔도 프로시저에서
		// insert 에는 반영하지 않도록 되어 있음.
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
		// RFamily_LoadInfo 함수에서 추가한 패밀리 데이터에 멤버 테이터를 입력한다.
		//
		if (pUserInfo->pcsFamily == NULL) return;

		// ..멤버 데이터 설정
		USERINFO* pMember;
		CSHFamilyMember				csMember;
		CSHFamilyMember::stINFO		stInfo;

		for(DWORD i=0; i<pMessage->dwResult; i++)
		{
			// 맴버 수가 초과되어 DB에 저장되어 있을 경우 처리(비정상적인 경우)
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

			// 맴버 설정
			((CSHGroupMember*)&csMember)->Set(&stInfo);
			pUserInfo->pcsFamily->SetMember(&csMember, i);

			// 맴버 수 증가
			CSHFamily::stINFO	stFamilyInfo = *pUserInfo->pcsFamily->Get();
			stFamilyInfo.nMemberNum++;
			((CSHGroup*)pUserInfo->pcsFamily)->Set(&stFamilyInfo);
		}
		// ..기본 패밀리 데이터 입력
		g_csFamilyManager.SetFamily(pUserInfo->pcsFamily);

		// 다른 테이블에서 참조해야 할 정보 불러오기
		for(UINT i=0; i<pUserInfo->pcsFamily->Get()->nMemberNum; i++)
		{
			Family_Member_LoadExInfo01(pUserInfo->dwCharacterID, pUserInfo->pcsFamily->GetMember(i)->Get()->nID);
		}
		// 다른 에이전트에 알리기
		g_csFamilyManager.ASRV_SendMemberConToOtherAgent(pUserInfo->pcsFamily, pUserInfo->dwCharacterID, CSHFamilyMember::MEMBER_CONSTATE_LOGIN);
		// 문장 로드
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

		// 다른 테이블로부터의 참조 정보 설정
		// ..맴버 설정
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

				// 마스터 이름 설정
				if (stInfo.nID == pUserInfo->pcsFamily->Get()->nMasterID)
				{
					CSHFamily::stINFO stFamilyInfo = *pUserInfo->pcsFamily->Get();
					SafeStrCpy(stFamilyInfo.szMasterName, stInfo.szName, MAX_NAME_LENGTH+1);
					pUserInfo->pcsFamily->Set(&stFamilyInfo);
				}

				// 맵서버에 호칭 알리기
				if (pMessage->dwID == pUserInfo->pcsFamily->GetMember(i)->Get()->nID)
				{
					g_csFamilyManager.ASRV_SendNicknameToMap(pUserInfo, pUserInfo->pcsFamily->GetMember(i)->Get()->szNickname);
				}

				break;
			}
		}

		for(UINT i=0; i<pUserInfo->pcsFamily->Get()->nMemberNum; i++)
		{
			// 삭제되거나 유효하지 않은 멤버 제거
			if (pUserInfo->pcsFamily->GetMember(i)->Get()->szName[0] == '@')
			{
				Family_Leave_SaveInfo(pUserInfo->pcsFamily->GetMember(i)->Get()->nID, CSHFamilyManager::FLK_LEAVE, TRUE);
				pUserInfo->pcsFamily->DelMember(pUserInfo->pcsFamily->GetMember(i)->Get()->nID);
				i--;
			}
		}

		// ..최종 패밀리 데이터 입력
		g_csFamilyManager.SetFamily(pUserInfo->pcsFamily);
		// 클라이언트에 정보 보내기
		g_csFamilyManager.ASRV_SendInfoToClient(pUserInfo->pcsFamily);
	}
}

void Family_CheckName(DWORD nPlayerID, char* pszName)
{
	// 패밀리 이름을 '' 로 안 묶어주니까 1231패밀리 와 같은 순서로 숫자 한글조합(영문은 확인 안 해봤음)을 문자열로 인식하지 않는다.
	// SQL서버에서 숫자가 먼저 나오면 문자열로 인식하지 않는 듯...
	sprintf(txt, "EXEC  %s '%s'", STORED_FAMILY_CHECKNAME, pszName);
	g_DB.Query(eQueryType_FreeQuery, eFamily_CheckName, nPlayerID, txt);
}

void RFamily_CheckName(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	if( stricmp((char*)pData[0].Data[0], "") == 0 )
	{
		// 이미 DB에 존재하는 이름이다..
		g_csFamilyManager.ASRV_CreateFamily(pUserInfo, NULL, CSHFamilyManager::FCS_NAME_ERR);
	}
	else
	{
		// DB에 없는 이름. 사용할 수 있다. 다시 패밀리 생성 시도
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

// E 패밀리 추가 added by hseos 2007.07.09	2007.07.10	2007.07.14	2007.10.11

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2008.01.22
void Farm_SendNote_DelFarm(DWORD nFarmOwnerID)
{
	sprintf(txt, "EXEC  dbo.MP_FARM_SendNote_DelFarm %d", nFarmOwnerID);
	g_DB.Query(eQueryType_FreeQuery, eFarm_SendNote_DelFarm, nFarmOwnerID, txt);
}

void RFarm_SendNote_DelFarm(LPQUERY pData, LPDBMESSAGE pMessage)
{
	// 패밀리 멤버 이름을 얻어서 쪽지를 보낸다.
	for(DWORD i=0; i<pMessage->dwResult; i++)
	{
		NoteSendtoPlayer(0, "<SYSTEM>", (char*)pData[0].Data[0], "61");
	}
}
// E 농장시스템 추가 added by hseos 2008.01.22

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
			PunishListAdd( pMessage->dwID, ePunish_Login, POW( 2, nCount-1 ) * 1 * 60 * 60 );	//1시간부터 더블로!
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



// 081205 LUJ, 패밀리 포인트 로그
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