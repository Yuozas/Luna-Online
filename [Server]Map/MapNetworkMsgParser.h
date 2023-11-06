
#ifndef __MAPNETWORKMSGPARSER_H__
#define __MAPNETWORKMSGPARSER_H__

void MP_POWERUPMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_MAPSERVERMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_CHATMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_CHARMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_USERCONNMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_MOVEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_ITEMMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_SKILLTREEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_CHEATMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_QUICKMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_PARTYMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_UNGIJOSIKMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
// 070124 LYW --- PeaceWar : Delete peacewar part.
//void MP_PEACEWARMODEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_STORAGEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_WANTEDMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_SkillMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_BattleMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_REVIVEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_OPTIONMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_MurimnetMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_MonitorMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_JOURNALMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

//KES EXCHANGE 030922
void MP_EXCHANGEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_HACKCHECKMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_SOCIETYACTMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);


// LBS 03.09.25 노점상
void MP_STREETSTALLMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

void MP_NPCMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

void MP_QUESTMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

void MP_PKMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void MP_HACKCHECKMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_GUILDMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_GUILDFIELDWARMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_PARTYWARMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_GTOURNAMENTMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_SIEGEWARMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_GUILUNIONMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void MP_SIEGEWARPROFITMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );

// desc_hseos_소스분석용테스트_01
// S 소스분석용테스트 추가 added by hseos 2007.04.05
void MP_HSEOS_TESTMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
// E 소스분석용테스트 추가 added by hseos 2007.04.05

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.04.11
void MP_FARM_MsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
// E 농장시스템 추가 added by hseos 2007.04.11

// desc_hseos_주민등록01
// S 주민등록 추가 added by hseos 2007.06.04
void MP_RESIDENTREGIST_MsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
// E 주민등록 추가 added by hseos 2007.06.04

void MP_FAMILYMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.11.19
void MP_DATE_MsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
// E 데이트 존 추가 added by hseos 2007.11.19

// desc_hseos_결혼_01
// S 결혼 추가 added by hseos 2007.12.11
void MP_MARRIAGE_MsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
// E 결혼 추가 added by hseos 2007.12.11

// Weather System
void MP_WEATHERMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );

//---KES AUTONOTE
void MP_AUTONOTE_MsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
//---------------

// 080327 NYJ --- 낚시시스템
void MP_FISHING_ServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

void MP_PET_MsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

// 070228 LYW --- MapNetworkMsgParser : Add message parser for facial part.
void MP_FACIALMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

// 070228 LYW --- MapNetworkMsgParser : Add message parser for emotion part.
void MP_EMOTIONMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) ;

BOOL CheckHackNpc( CPlayer* pPlayer, WORD wNpcIdx );


// 070205 LYW --- Add functions to process network msg.
/// MONITORMSGPARSER PART
void MornitorMapServer_NoticeSend_Syn() ;
void MornitorMapServer_Ping_Syn(MSGROOT* pTempMsg, DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) ;
void MornitorMapServer_QueryUserCount_Syn(DWORD dwConnectionIndex, char* pMsg) ;
void MornitorMapServer_Query_ChannelInfo_Syn(DWORD dwConnectionIndex) ;
void MornitorMapServer_AssertMsgBox_Syn(char* pMsg) ;
void MornitorMapServer_ServerOff_Syn() ;

/// CHARMSGPARSER PART
//void Char_LevelDown_Syn(char* pMsg) ;
void Char_PointAdd_Syn(char* pMsg) ;
void Char_State_Notify(char* pMsg) ;
//void Char_Ability_Upgrade_Syn(char* pMsg) ;
//void Char_Ability_Upgrade_SkPoint_Syn(char* pMsg) ;
//void Char_Ability_Downgrade_SkPoint_Syn(char* pMsg) ;
void Char_ExitStart_Syn(char* pMsg) ;
void Char_Emergency_Syn(char* pMsg) ;
void Char_Exit_Syn(char* pMsg) ;
void Char_BadFame_Syn(char* pMsg) ;
void Char_PlayTime_Syn(char* pMsg) ;
void Char_PointMinus_Syn(char* pMsg) ;
void Char_Mussang_Syn(char* pMsg, DWORD dwLength) ;

/// USERCONNMSGPARSER
void UserConn_ChannelInfo_Syn(MSGBASE* pTempMsg, DWORD dwConnectionIndex, char* pMsg) ;
void UserConn_GameIn_OtherMap_Syn(char* pMsg) ;
void UserConn_GameIn_Syn(DWORD dwConnectionIndex, char* pMsg) ;
void UserConn_GameInPos_Syn(DWORD dwConnectionIndex, char* pMsg) ;
void UserConn_GameIn_Nack(MSGBASE* pTempMsg) ;
void UserConn_GameOut_Syn(char* pMsg) ;
void UserConn_NoWaitExitPlayer(char* pMsg) ;
void UserConn_Disconnected(DWORD dwConnectionIndex, char* pMsg) ;
void UserConn_ChangeMap_Syn(DWORD dwConnectionIndex, char* pMsg) ;
void UserConn_NpcAdd_Syn(char* pMsg);	//080402 NYJ
void UserConn_NpcDie_Syn(char* pMsg);	//080402 NYJ
void UserConn_ChangeMap_Nack() ;
// 070917 LYW --- MapNetworkMsgParser : Add function to process change map through the npc.
void UserConn_Npc_ChangeMap_Syn(DWORD dwConnectionIndex, char* pMsg) ;
void UserConn_SavePoint_Syn(char* pMsg) ;
void UserConn_BackToCharSel_Syn(MSGBASE* pTempMsg, DWORD dwConnectionIndex, char* pMsg) ;
void UserConn_SetVisible(char* pMsg) ;
void UserConn_BackToBeforeMap_Syn(char* pMsg) ;
void UserConn_Enter_EventMap_Syn(DWORD dwConnectionIndex, char* pMsg) ;
void UserConn_Return_Syn(DWORD dwConnectionIndex, char* pMsg) ;

/// MOVEMSGPARSER PART
void Move_Target(char* pMsg, DWORD dwLength) ;
void Move_OneTarget(char* pMsg) ;
void Move_Stop(char* pMsg, DWORD dwLength) ;
void Move_WalkMode(char* pMsg, DWORD dwLength) ;
void Move_RunMode(char* pMsg, DWORD dwLength) ;
void Move_Kyunggong_Syn(char* pMsg, DWORD dwLength) ;
void Move_EffectMove(char* pMsg) ;
void Move_Pet_Move(char* pMsg) ;
void Move_Pet_Stop(char* pMsg, DWORD dwLength) ;

/// CHEATMSGPARSER PART
void Cheat_WhereIs_Syn(DWORD dwConnectionIndex, char* pMsg) ;
void Cheat_Hide_Syn(char* pMsg) ;
//	void Cheat_BanCharacter_Syn(char* pMsg) ;
void Cheat_PkAllow_Syn(CPlayer* pSender, char* pMsg) ;
void Cheat_MoveToChar_Syn(char* pMsg) ;
void Cheat_Move_Syn(char* pMsg) ;
void Cheat_MoveUser_Syn(char* pMsg) ;
void Cheat_ChangeMap_Syn(char* pMsg) ;
void Cheat_Life_Syn() ;
void Cheat_Mp_Syn() ;
void Cheat_Item_Syn(char* pMsg) ;
//void Cheat_Item_Option_Syn(char* pMsg) ;
void Cheat_Mana_Syn(char* pMsg) ;
void Cheat_AddSkill_Syn(char* pMsg) ;
void Cheat_SkillLevel_Syn(char* pMsg) ;
void Cheat_Update_Skill_Syn(char* pMsg) ;
void Cheat_Money_Syn(char* pMsg) ;
void Cheat_ReLoading() ;
void Cheat_LevelUp_Syn(char* pMsg) ;
void Cheat_PetLevelUp_Syn(char* pMsg) ;
// 070411 LYW --- MapNetworkMsgParser : Add function to level down.
void Cheat_LevelDown_Syn(char* pMsg) ;
/*어빌리티 삭제 - 이영준 
void Cheat_AbilityExp_Syn(char* pMsg) ;
*/
void Cheat_Event_Monster_Regen(char* pMsg) ;
void Cheat_Event_Monster_Delete(char* pMsg) ;
void Cheat_Str_Syn(char* pMsg) ;
void Cheat_Dex_Syn(char* pMsg) ;
void Cheat_Vit_Syn(char* pMsg) ;
void Cheat_Wis_Syn(char* pMsg) ;
void Cheat_Int_Syn(char* pMsg) ;
void Cheat_Kill_AllMonster_Syn() ;
void Cheat_PartyInfo_Syn(char* pMsg) ;
void Cheat_Event_Syn(char* pMsg) ;
void Cheat_PlusTime_AllOff(char* pMsg) ;
void Cheat_PlusTime_On(char* pMsg) ;
void Cheat_PlusTime_Off(char* pMsg) ;
void Cheat_MunpaDateClr_Syn(char* pMsg) ;
void Cheat_Change_EventMap_Syn(char* pMsg) ;
void Cheat_Event_Start_Syn(char* pMsg) ;
void Cheat_Event_Ready_Syn(char* pMsg) ;
void Cheat_GetShopItem(char* pMsg) ;
/*	void Cheat_Stage_Syn(char* pMsg) ;
*/
void Cheat_BossMob_Syn(char* pMsg) ;
void Cheat_Jackpot_GetPrize(char* pMsg) ;
void Cheat_Jackpot_OnOff(char* pMsg) ;
void Cheat_Jackpot_MoneyPerMonster(char* pMsg) ;
void Cheat_Jackpot_Probability(char* pMsg) ;
void Cheat_Jackpot_Control(char* pMsg) ;
void Cheat_GuildUnion_Clear(char* pMsg) ;
void Cheat_Mob_Chat(char* pMsg) ;
void Cheat_Mussang_Ready(char* pMsg) ;
void Cheat_GuildPoint_Syn(char* pMsg) ;
//void Cheat_GuildHunted_MonsterCount_Syn(char* pMsg) ;
// 06. 05 HIDE NPC - 이영준
void Cheat_NpcHide_Syn(char* pMsg) ;
// 070429 LYW --- MapNetworkMsgParser : Add function to check hpmp determination for test.
//#ifdef _DEBUG
void Cheat_HPMP_Determination_Syn(char* pMsg) ;
void Cheat_DropItem_Syn(char* pMsg) ;
void Cheat_SkillPoint_Syn( char *pMsg );
void Cheat_God_Syn( char *pMsg );
// 071212 KTH -- 확장 인벤토리의 정보를 초기화 하여준다.
void Cheat_Reset_Inventory(char* pMsg) ;
// 080115 KTH -- PVP 데미지 비율 설정
void Cheat_PvP_Damage_Rate(char* pMsg) ;

// 071106 LYW --- MapNetworkMsgParser : Add function to setting default hero stats.
void Cheat_Default_Stats(char* pMsg) ;

// 071106 LYW --- MapNetworkMsgParser : Add function to setting default hero skill.
void Cheat_Default_Skill(char* pMsg) ;

//#endif //_DEBUG

// 071023 LYW --- MapNetworkMsgParser : Add message parser for tutorial part.
void MP_TUTORIALMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength) ;

// 080426 NYJ --- 낚시관련 치트키
void Cheat_Fishing_Exp(char* pMsg);
void Cheat_Fishing_FishPoint(char* pMsg);

#endif //__MAPNETWORKMSGPARSER_H__
