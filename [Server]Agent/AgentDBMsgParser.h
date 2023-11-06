
#ifndef __AGENTDBMSGPARSER_H__
#define __AGENTDBMSGPARSER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "db.h"
#include "CommonDBMsgParser.h"


//////////////////////////////////////////////////////////////////
// DB¿¡¼­ ¾²ÀÌ´Â ¼öÄ¡Ä¡È¯.

#define dMAXCHARACTER 5

//extern MSG_CHAT g_WisperTemp;
//extern MSG_CHAT g_MunpaTemp;
//extern MSG_CHAT g_PartyTemp;

enum BasicCharacterInfo
{
	eCL_ObjectID = 0, eCL_StandIndex, eCL_ObjectName, eCL_BodyType, eCL_HeadType, 
	eCL_Glove, eCL_Dress, eCL_shoes, eCL_Weapon, 
	eCL_Grade, eCL_Map, eCL_Gender, eCL_Race, 
	eCL_JobGrade, eCL_Job1, eCL_Job2, eCL_Job3, eCL_Job4, eCL_Job5, eCL_Job6, 
	eCL_GuildName, eCL_AuthKey,
};

enum MakeCharacterInfo
{
	eMC_UserID = 0, eMC_Gengoal, eMC_DEX, eMC_STA, eMC_Simmak, eMC_Name, eMC_Job, eMC_BodyType, eMC_HeadType
};

enum Friendinfo
{
	eFr_TargetID=0, eFr_FromID, eFr_ToPlayerName, eFr_FromPlayerName, eFr_Err, 
};
enum Friendisvalidtarget
{
	eFr_vtFromid , eFr_vtTargetid , eFr_vtFromname , eFr_vtToname, eFr_vtvailderr, 
};
enum Frienddel
{
	eFr_targetname, eFr_IsSuccess, 
};
enum Friendadd 
{
	eFr_addToErr, eFr_addToIDX, eFr_addToName, eFr_addFromErr, eFr_addFromIDX, eFr_addFromName, 
};
enum Friendlist
{
	eFr_FriendName, eFr_IsLoggin,
};
enum Friendlogin
{
	eFr_LTargetID, eFr_LLoggedname,
};
enum Friendloginedfriend
{
	eFr_LFFriendID, eFr_LFFriendName,
};
enum 
{
	eFr_FLFriendid, eFr_FLFriendname, eFr_FLIsLogin, eFr_FLTotalPage,
};
enum Friendsendnote
{
	eFr_NErr, eFr_NToName, eFr_NToId,
};
enum FriendNotelist
{
	eFr_NNoteID, eFr_NSender, eFr_NSentDate, eFr_NSentTime, eFr_NTotalpage, eFr_NbIsRead,
};
enum FriendReadnote
{
	eFr_NRNNoteID, eFr_NRNSender, eFr_NRNNote, eFr_NRNItemIdx,
};
enum FriendDeletenote
{
	eFr_NdNoteID, eFr_NdbLast,
};

//-----------------------------------------------------------------------
// DBÄõ¸® ¸®ÅÏ ÇÔ¼ö°¡ ´Ã¾î³¯¶§¸¶´Ù Ãß°¡
// °è¼Ó Ãß°¡µÇ´Â ºÎºÐ
enum 
{
	eCharacterBaseQuery = 1,		/// ÄÉ¸¯ÅÍ ±âº»Á¤º¸¸¦ ¾ò¾î¿Â´Ù
	eCreateCharacter,			/// Ä³¸¯»ý¼º
	eLoginCheckDelete,			/// Á¢¼ÓÃ¼Å©»èÁ¦
	eDeleteCharacter,			/// Ä³¸¯ÅÍ »èÁ¦
	eNewCharacterCheckName,     /// »õ·Î¿î Ä³¸¯ÅÍ ÀÌ¸§ Áßº¹Ã¼Å© 
	eForcedDisconnect,          /// °­Á¦·Î ±âÁ¸À¯Àú¸¦ ²÷À½.
	eSetCurMaxCount,			/// ÇöÀçÀ¯Àú¿Í MaxÀ¯Àú¼ö ±â·Ï
	eLoginMapInfoSearchForName, /// Ä³¸¯¸íÀ¸·Î Á¢¼Ó¿©ºÎ¹× ¸Ê¹øÈ£ ¾ò¾î¿À±â
	eSavePoint,					/// SavePoint 
	eUnRegistLoginMapInfo,		/// ¸Ê¿¡¼­ ³ª°¡¸é ½ÇÇàÇÑ´Ù
	eMapUserUnRegistMapInfo,	/// ¸Ê¼­¹ö Á¾·á½Ã ½ÇÇà
	eFriendGetTargetMemeberIDX,	/// Ä£±¸ µî·Ï/»èÁ¦½Ã Å¸°Ù ÇÃ·¹ÀÌ¾î ¾ÆÀÌµð ¾ò¾î¿Í¼­ ¸Þ½ÃÁö 
	eFriendAdd,					/// Ä£±¸ µî·Ï
	eFriendIsValidTarget,		/// Ä£±¸ µî·Ï ÇÒ¼ö ÀÖ´ÂÁö
	eFriendDel,					/// Ä£±¸ »èÁ¦
	eFriendDelID,				/// Ä£±¸ »èÁ¦ (¾ÆÀÌµð·Î)
	eFriendDelList,				/// Ä£±¸¸®½ºÆ® »èÁ¦(Ä³¸¯ÅÍ »èÁ¦½Ã)
	eFriendNotifyLogin,			/// ·Î±×ÀÎ ¾Ë¸²
	eFriendGetFriendList,		/// Ä£±¸¸®½ºÆ®
	eFriendGetLoginFriends,		/// ·Î±×ÀÎµÇ¾îÀÖ´Â Ä£±¸ ¾Ë¾Æ¿À±â
	eNoteIsNewNote,				
	eNoteDelAll,				/// ÂÊÁö ¸ðµÎ »èÁ¦
	eNoteList,					/// ÂÊÁö¸®½ºÆ®
	eNoteDelete,				/// ÂÊÁö »èÁ¦
	eWantedDelete,				/// Ä³¸¯ÅÍ »èÁ¦·Î ÀÎÇÑ Ã´»ì±Ç °¡Áø Ä³¸¯ÅÍ Á¤º¸ º¯°æ
	eWantedDelWantInfo,			/// Ä³¸¯ÅÍ »èÁ¦·Î ÀÎÇÑ Çö»ó±Ý Á¤º¸ »èÁ¦

//For GM-Tool	
	eGM_BanCharacter,
	eGM_UpdateUserLevel,
	eGM_WhereIsCharacter,
	eGM_Login,
	eGM_GetGMPowerList,
//	
	eAgentDown,					/// ¿¡ÀÌÀüÆ® Å°¸é ÇØ´ç ¿¡ÀÌÀüÆ®¿¡ ¼ÓÇÑ ¾Öµé ·Î±× ¾Æ¿ô Ã³¸®
//----China Billing
	eBilling_RemainTimeUpdate,
	eBilling_UpdateAll,
//----
	eEventItemUse050106,		// 050106 event
	eCheckGuildMasterLogin,		// checkguildmasterlogin
	eEventItemUse050203,		// 050203 event

	eInsertBillingTable,		// for billling
	eDeleteBillingTable,

	eEventItemUse050322,		// 050322 event

	eJackpot_TotalMoney_Load,	// jackpot TotalMoneyLoad

	eCheckGuildFieldWarMoney,	// check guildfieldwarmoney
	eAddGuildFieldWarMoney,		// add guildfieldwarmoney
	
	eChaseFindUser,

	eJP_CharacterSlot,			// Get characterslot count

	eEventItemUse051108,			// 051108 event
	eEventItemUse2,					

	eGM_UpdateUserState,
	eLogGMToolUse,					// 
	eNProtectBlock,


	eItemInsert,
	eSkillInsert,

	// desc_hseos_몬스터미터01
	// S 몬스터미터 추가 added by hseos 2007.05.29
	eInitMonstermeterInfoOfDB,
	// E 몬스터미터 추가 added by hseos 2007.05.29

	// desc_hseos_성별선택01
	// S 성별선택 추가 added by hseos 2007.06.15
	eGetUserSexKind,				// RGetUserSexKind
	// E 성별선택 추가 added by hseos 2007.06.15

	// desc_hseos_패밀리01
	// S 패밀리 추가 added by hseos 2007.07.09	2007.07.14
	eFamily_LoadInfo,
	eFamily_SaveInfo,
	eFamily_Member_LoadInfo,
	eFamily_Member_LoadExInfo01,
	eFamily_Member_SaveInfo,
	eFamily_CheckName,
	eFamily_Leave_LoadInfo,
	eFamily_Leave_SaveInfo,
	// E 패밀리 추가 added by hseos 2007.07.09	2007.07.14
	
// 080121 KTH --
	eChange_Character_Name,

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2008.01.22
	eFarm_SendNote_DelFarm,
	// E 농장시스템 추가 added by hseos 2008.01.22

//---KES PUNISH
	ePunishList_Add,	//---제재목록
	ePunishList_Load,	//---제재목록

	ePunishCount_Add,	//---걸린 회수 추가
//-------------

	eGiftEvent,

	MaxQuery,
};


//---------------------------------------------------------------------------
// ÇÔ¼ö ÇÁ·ÎÅä Å¸ÀÔ
// °è¼Ó Ãß°¡µÇ´Â ºÎºÐ
//---------------------------------------------------------------------------
// Query function
// ÇÔ¼ö¸¸µéÁö ¾Ê°í ¹Ù·Î »ç¿ëÇÏ´Â °Ô ??
void CheatLog(DWORD CharacterIDX,DWORD CheatKind);
void UserIDXSendAndCharacterBaseInfo(DWORD UserIDX, DWORD AuthKey, DWORD dwConnectionIndex);
void LoginCheckDelete(DWORD UserID);//, DWORD dwConnectionIndex);
//void CreateCharacter(CHARACTERMAKEINFO* pMChar, DWORD dwConnectionIndex);
void CreateCharacter(CHARACTERMAKEINFO* pMChar, WORD ServerNo, DWORD dwConnectionIndex);

//void DeleteCharacter(DWORD dwPlayerID, DWORD dwConnectionIndex);
void DeleteCharacter(DWORD dwPlayerID, WORD ServerNo, DWORD dwConnectionIndex);

void CharacterNameCheck(char* pName, DWORD dwConnectionIndex);
void SetUserCurAndMaxCount(DWORD CurCount, DWORD MaxCount);
void SearchWhisperUserAndSend(DWORD dwPlayerID, char* CharacterName, DWORD dwKey );
void SearchPartyChatSendOnPlayer(DWORD dwPlayerID, DWORD dwKey );
//void SearchMunpaChatSendOnPlayer(DWORD dwPlayerID, DWORD dwKey );
void SaveMapChangePointUpdate(DWORD CharacterIDX, WORD MapChangePoint_Idx);
void UnRegistLoginMapInfo(DWORD CharacterIDX);
//void MapUserUnRegistLoginMapInfo(WORD MapPort);
void FriendGetUserIDXbyName(DWORD CharacterIDX, char* TargetName);
void FriendAddFriend(DWORD CharacterIDX, DWORD TargetID);
void FriendIsValidTarget(DWORD CharacterIDX, DWORD TargetID, char* FromName);
void FriendDelFriend(DWORD CharacterIDX, char* TargetName);
void FriendDelFriendID(DWORD CharacterIDX, DWORD TargetID, DWORD bLast);
void FriendNotifyLogintoClient(DWORD CharacterIDX);
void FriendGetLoginFriends(DWORD CharacterIDX);
void FriendGetFriendList(DWORD CharacterIDX);
void NoteIsNewNote(DWORD PlayerID);
void NoteSendtoPlayer(DWORD FromIDX, char* FromName, char* ToName, char* Note);
void NoteServerSendtoPlayer(DWORD FromIDX, char* FromName, char* ToName, char* Note);
void NoteSendtoPlayerID(DWORD FromIDX, char* FromName, DWORD ToIDX, char* Note);
void NoteDelAll(DWORD CharacterIDX);
void NoteList(DWORD CharacterIDX, WORD Page, WORD Mode);
void NoteRead(DWORD CharacterIDX, DWORD NoteIDX, DWORD IsFront);
void NoteDelete(DWORD PlayerID, DWORD NoteID, BOOL bLast);
void WantedDelete(DWORD CharacterIDX);
void WantedDelWantInfo(DWORD WantedIDX);
void ChaseFindUser(DWORD UserIdx, char* WantedName, DWORD dwItemIdx);

//For GM-Tool
void GM_WhereIsCharacter(DWORD dwID, char* CharacterName, DWORD dwSeacherID );
void GM_BanCharacter(DWORD dwID, char* CharacterName, DWORD dwSeacherID );
void GM_UpdateUserLevel(DWORD dwID, DWORD dwServerGroup, char* Charactername, BYTE UserLevel);
void GM_GetGMPowerList(DWORD dwStartIdx, DWORD dwFlag = 0 );
void GM_Login( DWORD dwConnectionIdx, char* strID, char* strPW, char* strIP );
//


void BillingRemainTimeUpdate(DWORD dwUserIdx, int nRemainTime );
void BillingUpdateAll( WORD wAgentNum );



void ItemInsertToDB(DWORD CharacterIdx, WORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, DWORD dwKey, WORD bSeal=0);

// Query Result function
void RUserIDXSendAndCharacterBaseInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCreateCharacter(LPQUERY pData, LPDBMESSAGE pMessage);
void RLoginCheck(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterNameCheck(LPQUERY pData, LPDBMESSAGE pMessage);
void RSearchWhisperUserAndSend(LPQUERY pData, LPDBMESSAGE pMessage);
//void RSearchMunpaChatSendOnPlayer(LPQUERY pData, LPDBMESSAGE pMessage);
void RFriendDelFriend(LPQUERY pData, LPDBMESSAGE pMessage);
void RFriendDelFriendID(LPQUERY pData, LPDBMESSAGE pMessage);
void RFriendGetFriendList(LPQUERY pData, LPDBMESSAGE pMessage);
void RFriendAddFriend(LPQUERY pData, LPDBMESSAGE pMessage);
void RFriendNotifyLogintoClient(LPQUERY pData, LPDBMESSAGE pMessage);
void RNoteIsNewNote(LPQUERY pData, LPDBMESSAGE pMessage);
void RNoteSendtoPlayer(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
void RNoteServerSendtoPlayer(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
void RNoteList(LPQUERY pData, LPDBMESSAGE pMessage);
void RNoteRead(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
void RNoteDelete(LPQUERY pData, LPDBMESSAGE pMessage);
void RDeleteCharacter(LPQUERY pData, LPDBMESSAGE pMessage);
void RFriendGetLoginFriends(LPQUERY pData, LPDBMESSAGE pMessage);
void RFriendIsValidTarget(LPQUERY pData, LPDBMESSAGE pMessage);
void RWantedDelete(LPQUERY pData, LPDBMESSAGE pMessage);

//For GM-Tool
void RGM_WhereIsCharacter(LPQUERY pData, LPDBMESSAGE pMessage);
void RGM_BanCharacter(LPQUERY pData, LPDBMESSAGE pMessage);
void RGM_UpdateUserLevel(LPQUERY pData, LPDBMESSAGE pMessage);
void RGM_GetGMPowerList(LPQUERY pData, LPDBMESSAGE pMessage);
void RGM_Login(LPQUERY pData, LPDBMESSAGE pMessage);
//

// guildfieldwar
void CheckGuildMasterLogin( DWORD dwConnectionIdx, DWORD dwPlayerIdx, char* pSearchName, DWORD dwMoney, BYTE Protocol );
void RCheckGuildMasterLogin( LPQUERY pData, LPDBMESSAGE pMessage );
void CheckGuildFieldWarMoney( DWORD dwConnectionIndex, DWORD dwSenderIdx, DWORD dwEnemyGuildIdx, DWORD dwMoney );
void RCheckGuildFieldWarMoney( LPQUERY pData, LPDBMESSAGE pMessage );
void AddGuildFieldWarMoney( DWORD dwConnectionIndex, DWORD dwPlayerIdx, DWORD dwMoney );
void RAddGuildFieldWarMoney( LPQUERY pData, LPDBMESSAGE pMessage );

// 050106 event
void EventItemUse050106( DWORD dwUserIdx );
// 050203 event
void EventItemUse050203( DWORD dwUserIdx, char* sCharName, DWORD dwServerNum );
// 050322 event
void EventItemUse050322( DWORD dwUserIdx );
// 051108 event
void EventItemUse051108( DWORD dwUserIdx, char* sCharName, DWORD dwServerNum );
void EventItemUse2( DWORD dwUserIdx, char* sCharName, DWORD dwServerNum );

//	jackpot
void JackpotLoadTotalMoney();
void RJackpotLoadTotalMoney( LPQUERY pData, LPDBMESSAGE pMessage );

void RChaseFindUser( LPQUERY pData, LPDBMESSAGE pMessage );

// for billing
void InsertBillingTable( DWORD dwUserIdx, DWORD dwBillType, DWORD dwAgentNo, DWORD dwServerSetNum );
void DeleteBillingTable( DWORD dwUserIdx, DWORD dwServerSetNum );

// Japan
void GetCharacterSlotCount( DWORD UserIdx, DWORD AuthKey, DWORD dwConnectionIndex );
void RGetCharacterSlotCount( LPQUERY pData, LPDBMESSAGE pMessage );

void GM_UpdateUserLevel(DWORD dwID, DWORD dwServerGroup, char* Charactername, BYTE UserState);
void RGM_UpdateUserState(LPQUERY pData, LPDBMESSAGE pMessage);


// GMlog
void LogGMToolUse( DWORD CharacterIdx, DWORD GMLogtype, DWORD Logkind, DWORD Param1, DWORD Param2 );

// desc_hseos_몬스터미터01
// S 몬스터미터 추가 added by hseos 2007.05.29
void InitMonstermeterInfoOfDB(DWORD nPlayerID);
// E 몬스터미터 추가 added by hseos 2007.05.29

// desc_hseos_성별선택01
// S 성별선택 추가 added by hseos 2007.06.15
void GetUserSexKind(DWORD nUserID);
void RGetUserSexKind(LPQUERY pData, LPDBMESSAGE pMessage);
// E 성별선택 추가 added by hseos 2007.06.15

// desc_hseos_패밀리01
// S 패밀리 추가 added by hseos 2007.07.09	2007.07.14	2007.07.17	2007.10.11	2007.10.19
void Family_SaveInfo(DWORD nMasterID, char* pszFamilyName, int nHonorPoint, BOOL bNicknameON, int nSaveType = 0);
void Family_SaveEmblemInfo(DWORD nPlayerID, DWORD nFamily, char* pcImg);
void RFamily_SaveInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void Family_LoadInfo(DWORD nPlayerID);
void RFamily_LoadInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void Family_LoadEmblemInfo(DWORD nPlayerID, DWORD nFamilyID);
void RFamily_LoadEmblemInfo(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
void Family_Member_SaveInfo(DWORD nFamilyID, DWORD nMemberID, char* pszMemberNickname);
void Family_Member_LoadInfo(DWORD nPlayerID, DWORD nFamilyID);
void Family_Member_LoadExInfo01(DWORD nPlayerID, DWORD nMemberID);
void RFamily_Member_LoadInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RFamily_Member_LoadExInfo01(LPQUERY pData, LPDBMESSAGE pMessage);
void Family_CheckName(DWORD nPlayerID, char* pszName);
void RFamily_CheckName(LPQUERY pData, LPDBMESSAGE pMessage);
void Family_Leave_SaveInfo(DWORD nPlayerID, int nLeaveKind, BOOL bInit, BOOL bBreakUp = FALSE);
void Family_Leave_LoadInfo(DWORD nPlayerID);
void RFamily_Leave_LoadInfo(LPQUERY pData, LPDBMESSAGE pMessage);
// E 패밀리 추가 added by hseos 2007.07.09	2007.07.14	2007.07.17	2007.10.11	2007.10.19

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2008.01.22
void Farm_SendNote_DelFarm(DWORD nFarmOwnerID);
void RFarm_SendNote_DelFarm(LPQUERY pData, LPDBMESSAGE pMessage);
// 081205 LUJ, 패밀리 로그
void InsertLogFamily( eFamilyLog, DWORD familyIndex, DWORD playerIndex, const char* memo = "" );
void InsertLogFamilyPoint( CSHFamily*, eFamilyLog );

//---KES PUNISH
void PunishListAdd( DWORD dwUserIdx, int nPunishKind, DWORD dwPunishTime );
void PunishListLoad( DWORD dwUserIdx );
void RPunishListLoad( LPQUERY pData, LPDBMESSAGE pMessage );

void PunishCountAdd( DWORD dwUserIdx, int nPunishKind, int nPeriodDay );
void RPunishCountAdd( LPQUERY pData, LPDBMESSAGE pMessage );
//-------------

#ifdef _NPROTECT_
void NProtectBlock(DWORD UserIdx, DWORD CharIdx, char* IP, DWORD BlockType);
#endif

void GiftEvent( DWORD dwUserIdx );
void RGiftEvent( LPQUERY pData, LPDBMESSAGE pMessage );

#endif //__AGENTBMSGPARSER_H__
