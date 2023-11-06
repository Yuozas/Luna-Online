
#ifndef __MSDBMSGPARSER_H__
#define __MSDBMSGPARSER_H__

#include "db.h"


typedef	void (*DBMsgFunc)(LPQUERY pData, LPDBMESSAGE pMessage);
typedef	void (*DBMiddleMsgFunc)(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);
typedef	void (*DBLargeMsgFunc)(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
extern DBMsgFunc g_DBMsgFunc[];

#ifdef DBID
#undef DBID
#endif
#define DBID(a) a, 

#ifdef DBRETID
#undef DBRETID
#endif
#define DBRETID(a)


enum   DBMESSAGEIDFUNC 
{
	#include "MSDBMsgID.h"
	MaxQuery,
};

/*
#ifdef DBID
#undef DBID
#endif
#define DBID(a)

#ifdef DBRETID
#undef DBRETID
#endif
#define DBRETID(a) void a(LPQUERY pData, LPDBMESSAGE pMessage); 

#include "MSDBMsgID.h"
*/

enum eOperInfo
{
	eOI_ErroCode = 0, eOI_OperIdx, eOI_OperID, eOI_OperName, eOI_OperPwd, eOI_OperPower, eOI_Date, eOI_Time, 
	eOI_IPIdx, eOI_IPAdress, eOI_IPDate, eOI_IPTime, 
};

#define eOI_MyIdx	eOI_ErroCode

enum eIPInfo
{
	eII_IPIdx, eII_IPAdress, eII_Date, eII_Time,
};

// Admin Info
void InsertIpAdress( WORD wConnectIdx, WORD wTempIdx, char* strIp );
void RInsertIpAdress( LPQUERY pData, LPDBMESSAGE pMessage );
void InsertOperator( WORD wConnectIdx, WORD wTempIdx, char* strId, char* strName, char* strPwd, WORD power );
void RInsertOperator( LPQUERY pData, LPDBMESSAGE pMessage );
void DeleteIpAdress( WORD wConnectIdx, WORD wTempIdx, DWORD ipIdx );
void DeleteOperator( WORD wConnectIdx, WORD wTempIdx, DWORD operIdx );
void TotalIpAdressInfo( WORD wConnectIdx, WORD wTempIdx, DWORD startIdx );
void RTotalIpAdressInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void TotalOperatorInfo( WORD wConnectIdx, WORD wTempIdx, DWORD startIdx, DWORD operIdx );
void RTotalOperatorInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void OperatorLoginCheck( DWORD dwConnectIdx, char* strId, char* strPwd, char* strIp );
void ROperatorLoginCheck( LPQUERY pData, LPDBMESSAGE pMessage );
/// 060829 PKH - GM툴 허용 아이피
void TotalGMIpAdressInfo( WORD wConnectIdx, WORD wTempIdx, DWORD startIdx );
void RTotalGMIpAdressInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void InsertGMIpAdress( WORD wConnectIdx, WORD wTempIdx, char* strIp );
void RInsertGMIpAdress( LPQUERY pData, LPDBMESSAGE pMessage );
void DeleteGMIpAdress( WORD wConnectIdx, WORD wTempIdx, DWORD ipIdx );


// User Info
void SearchUserInfoById(WORD wConnectIdx, WORD wTempIdx, WORD wServer, char* strId);
void SearchUserInfoByIdCard(WORD wConnectIdx, WORD wTempIdx, WORD wServer, char* strIdCard);
void SearchUserInfoByCharName(WORD wConnectIdx, WORD wTempIdx, WORD wServer, char* strCharName);
void RSearchUserInfo( LPMIDDLEQUERY pData, LPDBMESSAGE pMessage );

// Character Info
void CharacterBaseInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx );
void RCharacterBaseInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void CharacterInvenInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx );
void RCharacterInvenInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void CharacterPyogukInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx );
void RCharacterPyogukInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void CharacterPyogukItemInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx, POSTYPE startPos );
void RCharacterPyogukItemInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void CharacterMugongInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx );
void RCharacterMugongInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void CharacterAbilityInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx );
void RCharacterAbilityInfo( LPMIDDLEQUERY pData, LPDBMESSAGE pMessage );
void CharacterItemOptionInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx, DWORD dwCharIdx, DWORD StartIdx );
void RCharacterItemOptionInfo( LPQUERY pData, LPDBMESSAGE pMessage );

void UpdateCharacterBaseInfo( WORD wConnectIdx, WORD wTempIdx, TMSG_CHARACTER_UPDATEBASEINFO* Info );
void UpdateCharacterPyogukInfo( WORD wConnectIdx, WORD wTempIdx, DWORD UserIdx, WORD PyogukNum, MONEYTYPE Money );

// Item
void CharacterWearItemUpdate( DWORD dwCharIdx, BYTE type, WORD wItemIdx );
void InvenItemInsert( WORD wConnectIdx, WORD wTempIdx, char* sCharName, DWORD dwCharIdx, ITEMBASE* pItem, ITEM_OPTION* pOptionInfo );
void RInvenItemInsert( LPQUERY pData, LPDBMESSAGE pMessage );
void PyogukItemInsert( WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx, ITEMBASE* pItem, ITEM_OPTION* pOptionInfo );
void RPyogukItemInsert( LPQUERY pData, LPDBMESSAGE pMessage );
void MunpaItemInsert( WORD wConnectIdx, WORD wTempIdx, DWORD dwMunpaIdx, ITEMBASE* pItem, ITEM_OPTION* pOptionInfo );
void RMunpaItemInsert( LPQUERY pData, LPDBMESSAGE pMessage );

void ItemDelete( DWORD dwItemDBIdx, DWORD dwOptionDBIdx );
void ItemUpdate( DWORD dwDBIdx, ITEMBASE* pItem );
void ItemOptionUpdate( DWORD dwItemDBIdx, ITEM_OPTION* pOptionInfo );
void ItemOptionInsert( DWORD dwItemDBIdx, ITEM_OPTION* pOptionInfo );
void ItemOptionDelete( DWORD dwItemDBIdx );

// Mugong
//void MugongInsert( DWORD dwCharIdx, MUGONGBASE* pMugong );
//void MugongUpdate( DWORD dwMugongDBIdx, MUGONGBASE* pMugong );
//void MugongDelete( DWORD dwMugongDBIdx );
//
//// Ability 
//void AbilityUpdate( DWORD dwCharIdx, ABILITY_TOTALINFO* pAbility );

// Munpa
void SearchMunpaIdxByName( WORD wConnectIdx, WORD wTempIdx, WORD wServer, char* strName );
void RSearchMunpaIdx( LPQUERY pData, LPDBMESSAGE pMessage );
void MunpaBaseInfo( WORD wConnectIdx, WORD wTempIdx, DWORD MunpaIdx );
void RMunpaBaseInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void MunpaItemInfo( WORD wConnectIdx, WORD wTempIdx, DWORD MunpaIdx, POSTYPE startPos );
void RMunpaItemInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void RMunpaMemberInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void MunpaMemberInfo( WORD wConnectIdx, WORD wTempIdx, DWORD MunpaIdx, DWORD startIdx );

// log
void GameLogItemMoveLog( WORD wConnectIdx, WORD wTempIdx, DWORD startIdx, WORD wServer, char* strId, BYTE bDay, char* strStart, char* strEnd );
void RGameLogItemMoveLog( LPQUERY pData, LPDBMESSAGE pMessage );
void GameLogItemObtainLog( WORD wConnectIdx, WORD wTempIdx, DWORD startIdx, WORD wServer, char* strId, BYTE bDay, char* strStart, char* strEnd );
void RGameLogItemObtainLog( LPQUERY pData, LPDBMESSAGE pMessage );
void GameLogBaseInfoLog( WORD wConnectIdx, WORD wTempIdx, DWORD startIdx, WORD wServer, char* strId, BYTE bDay, char* strStart, char* strEnd );
void RGameLogBaseInfoLog( LPQUERY pData, LPDBMESSAGE pMessage );
void GameLogMoneyLog( WORD wConnectIdx, WORD wTempIdx, DWORD startIdx, WORD wServer, char* strId, BYTE bDay, char* strStart, char* strEnd );
void RGameLogMoneyLog( LPQUERY pData, LPDBMESSAGE pMessage );
void GameLogItemFromChrName( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, WORD wServer, BYTE bDay, char* strStart, char* strEnd, char* strChrName );
void RGameLogItemFromChrName( LPQUERY pData, LPDBMESSAGE pMessage );
void GameLogItemFromItemDBIndex( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, WORD wServer, BYTE bDay, char* strStart, char* strEnd, DWORD dwItemDBIdx );
void RGameLogItemFromItemDBIndex( LPQUERY pData, LPDBMESSAGE pMessage );

// pet
void PetInfoUpdate( DWORD dwPetDBIdx, TMSG_PET_UPDATEBASEINFO* PetInfo);

// jsd add 04.08.25 ---------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------

// IP Check 
void TotalIpInfo( DWORD startIdx );							// ip
void RTotalIpInfo( LPQUERY pData, LPDBMESSAGE pMessage );

// Log
void ItemMoneyLogFromType( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, WORD wLogType, DWORD dwCharIdx, DWORD dwItem, int nDay, char* sSDate, char* sEDate );
void RItemMoneyLogFromType( LPQUERY pData, LPDBMESSAGE pMessage );
void CharacterLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate );
void RCharacterLog( LPQUERY pData, LPDBMESSAGE pMessage );
void ExpPointLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate );
void RExpPointLog( LPQUERY pData, LPDBMESSAGE pMessage );
void MugongLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate );
void RMugongLog( LPQUERY pData, LPDBMESSAGE pMessage );
void MugongExpLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate );
void RMugongExpLog( LPQUERY pData, LPDBMESSAGE pMessage );
void MoneyLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate );
void RMoneyLog( LPQUERY pData, LPDBMESSAGE pMessage );
void MoneyWrongLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate );
void RMoneyWrongLog( LPQUERY pData, LPDBMESSAGE pMessage );
void AbilityLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate );
void RAbilityLog( LPQUERY pData, LPDBMESSAGE pMessage );
void ItemOptionLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate );
void ItemOptionLog2( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwDBIdx, int nDay, char* sSDate, char* sEDate );
void RItemOptionLog( LPQUERY pData, LPDBMESSAGE pMessage );

void CheatLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate );
void RCheatLog( LPQUERY pData, LPDBMESSAGE pMessage );
void HackingLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate );
void RHackingLog( LPQUERY pData, LPDBMESSAGE pMessage );

void ToolLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwLogType, char* sName, int nDay, char* sSDate, char* sEDate );
void RToolLog( LPQUERY pData, LPDBMESSAGE pMessage );
void PetLogFromType( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, WORD wLogType, DWORD dwCharIdx, DWORD dwItem, int nDay, char* sSDate, char* sEDate );
void RPetLogFromType( LPQUERY pData, LPDBMESSAGE pMessage );

/// 060831 PKH 문파토너먼트로그정보
void GuildTNLogFromType( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, WORD wLogType, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate );
void RGuildTNLogFromType( LPQUERY pData, LPDBMESSAGE pMessage );

/// 060831 PKH 공성전로그정보
void SiegeWarLogFromType( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, WORD wLogType, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate );
void RSiegeWarLogFromType( LPQUERY pData, LPDBMESSAGE pMessage );

/// 060915 PKH GMTOOL
void GMToolLogFromType( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, WORD wLogType, DWORD dwCharIdx, DWORD dwItem, int nDay, char* sSDate, char* sEDate );
void RGMToolLogFromType( LPQUERY pData, LPDBMESSAGE pMessage );


// user info
void SearchMemberNumFromID( WORD wConnectIdx, WORD wTempIdx, char* sID );
void RSearchMemberNumFromID( LPQUERY pData, LPDBMESSAGE pMessage );
void SearchMemberNumFromCharName( WORD wConnectIdx, WORD wTempIdx, char* sCharName );
void RSearchMemberNumFromCharName( LPQUERY pData, LPDBMESSAGE pMessage );
void SearchCharIdxFromCharName( WORD wConnectIdx, WORD wTempIdx, char* sCharName );
void RSearchCharIdxFromCharName( LPQUERY pData, LPDBMESSAGE pMessage );

/// 060831 PKH 문파정보
void SearchGuildIdxFromGuildName( WORD wConnectIdx, WORD wTempIdx, char* sCharName );
void RSearchGuildIdxFromGuildName( LPQUERY pData, LPDBMESSAGE pMessage );

/// 060831 PKH 공성전
void SearchSWGuildIdxFromGuildName( WORD wConnectIdx, WORD wTempIdx, char* sCharName );
void RSearchSWGuildIdxFromGuildName( LPQUERY pData, LPDBMESSAGE pMessage );

void SearchUserInfoFromMemberNum( WORD wConnectIdx, WORD wTempIdx, DWORD dwMemberNum, WORD wServer );
void RSearchUserInfoFromMemberNum( LPMIDDLEQUERY pData, LPDBMESSAGE pMessage );
void SimpleCharacterInfoFromMN( WORD wConnectIdx, WORD wTempIdx, DWORD dwMemberNum );
void RSimpleCharacterInfoFromMN( LPQUERY pData, LPDBMESSAGE pMessage );
void CharacterCreateInfoFromMN( WORD wConnectIdx, WORD wTempIdx, DWORD dwMemberNum );
void RCharacterCreateInfoFromMN( LPQUERY pData, LPDBMESSAGE pMessage );

void CheckCharacterName( WORD wConnectIdx, WORD wTempIdx, char* sName );
void RCheckCharacterName( LPQUERY pData, LPDBMESSAGE pMessage );
void CharacterRecover( WORD wConnectIdx, WORD wTempIdx, DWORD dwChrIdx, char* sName );
void RCharacterRecover( LPQUERY pData, LPDBMESSAGE pMessage );

// quest
void CharacterMainQuestInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx );
void RCharacterMainQuestInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void MainQuestDelete( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx, DWORD dwQuestIdx );
void CharacterSubQuestInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx, DWORD dwQuestIdx );
void RCharacterSubQuestInfo( LPQUERY pData, LPDBMESSAGE pMessage );

// log
void InsertItemMoneyLog( DWORD FromChrIdx, char* FromChrName, DWORD ToChrIdx, char* ToChrName,
					     DWORD LogType, MONEYTYPE FromTotalMoney, MONEYTYPE ToTotalMoney, MONEYTYPE ChangeMoney,
						 DWORD ItemIdx, DWORD ItemDBIdx, POSTYPE ItemFromPos, POSTYPE ItemToPos,
						 DURTYPE ItemDur, DWORD ExpPoint );
void InsertLogTool( DWORD dwLogType, DWORD dwLogKind, DWORD dwOperIdx, char* sOperName, DWORD dwTargetIdx, char* sTargetName,
				    DWORD dwParam1, DWORD dwParam2, DWORD dwParam3, DWORD dwParam4 );

// userlevel
void ChangeUserLevel( WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx, DWORD dwUserLevel );
void RChangeUserLevel( LPQUERY pData, LPDBMESSAGE pMessage );

// loginpoint
void ChangeLoginPoint( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharacterIdx, DWORD dwMapNum );
void RChangeLoginPoint( LPQUERY pData, LPDBMESSAGE pMessage );

// characterinveninfo2 
void CharacterInvenInfo2(WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx);
void RCharacterInvenInfo2(LPQUERY pData, LPDBMESSAGE pMessage);

void ChangeItemPosition( WORD wConnectIdx, WORD wTempIdx, DWORD dwChrIdx, DWORD dwDBIdx, DWORD dwPos );
void ChangeWareItemPosition( WORD wConnectIdx, WORD wTempIdx, DWORD dwChrIdx, DWORD dwDBIdx, DWORD dwPos );
void ChangeMugongPosition( WORD wConnectIdx, WORD wTempIdx, DWORD dwChrIdx, DWORD dwDBIdx, DWORD dwPos );

void CharacterShopWareItemInfo( WORD wConnectionIdx, WORD wTempIdx, DWORD dwUserIdx );
void RCharacterShopWareItemInfo( LPQUERY pData, LPDBMESSAGE pMessage );

void ShopWareItemInsert( WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx, ITEMBASE* pItem, ITEM_OPTION* pOptionInfo );
void RShopWareItemInsert( LPQUERY pData, LPDBMESSAGE pMessage );

// Guild
void SearchGuild( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx );
void RSearchGuild( LPQUERY pData, LPDBMESSAGE pMessage );
void SearchGuildInfo( WORD wConnectIdx, WORD wTempIdx, char* pGuildName );
void RSearchGuildInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void SearchGuildMemberInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwGuildIdx );
void RSearchGuildMemberInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void SearchGuildWareItemInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwGuildIdx );
void RSearchGuildWareItemInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void SearchGuildWareItemOptionInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwGuildIdx );
void RSearchGuildWareItemOptionInfo( LPQUERY pData, LPDBMESSAGE pMessage );

// Rare Item
void CharacterRareItemOptionInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx, DWORD dwCharIdx, DWORD StartIdx );
void RCharacterRareItemOptionInfo( LPQUERY pData, LPDBMESSAGE pMessage );

// shop item
void CharacterUsingShopItemInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx );
void RCharacterUsingShopItemInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void CharacterUsingShopItemInfoDelete( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx, DWORD dwItemDBIdx, DWORD dwDelete );			
void CharacterUsingShopItemInfoEdit( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx, DWORD dwItemDBIdx, DWORD dwParam );			


// pet
void SearchPetByidx(WORD wConnectIdx, WORD wTempIdx, WORD wServer, DWORD dwPetIdx);
void RSearchPetInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RSearchPetList(LPQUERY pData, LPDBMESSAGE pMessage);
void RSearchPetWareItemInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void RSearchPetAccItemInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void SearchPetWareItemInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwcharidx, DWORD dwStartIdx, DWORD dwEndIdx );
void SearchPetListByidx(WORD wConnectIdx, WORD wTempIdx, WORD wServer, DWORD loginidx);
void SearchPetAccItemInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwPetIdx, DWORD dwStartIdx, DWORD dwEndIdx );
void RSearchUserLoginChk( LPQUERY pData, LPDBMESSAGE pMessage );
void SearchUserLoginChk( WORD wConnectIdx, WORD wTempIdx, DWORD user_num );
void RCharacterMainPetList( LPQUERY pData, LPDBMESSAGE pMessage );
void CharacterMainPetList( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx );
void SearchPetMemberNumFromID( WORD wConnectIdx, WORD wTempIdx, char* sID );
void RSearchPetMemberNumFromID( LPQUERY pData, LPDBMESSAGE pMessage );

//delete character list
void SearchDelCharList( WORD wConnectIdx, WORD wTempIdx, WORD WServer, char* delcharname );
void RSearchDelCharList( LPQUERY pData, LPDBMESSAGE pMessage );
#endif