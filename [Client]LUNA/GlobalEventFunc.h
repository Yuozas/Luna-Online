#pragma once

/* 메인 타이틀 */
struct FUNC
{
	void (*_func)(LONG lId, void * p, DWORD we);
	const char * _funcName;
};
extern FUNC g_mt_func[];
extern int FUNCSEARCH(char * funcName);


//pjs 로그아이디 창에서의 버튼 처리...
extern void		MT_LogInOkBtnFunc(LONG lId, void* p, DWORD we);	 
extern void		MT_ExitBtnFunc(LONG lId, void* p, DWORD we);
extern void		MT_EditReturnFunc(LONG lId, void* p, DWORD we); //3
extern void		MT_DynamicBtnFunc(LONG lId, void* p, DWORD we); //3
extern void		MT_DynamicEditReturnFunc(LONG lId, void* p, DWORD we);

//pjs 로그캐렉터 창에서 버튼 처리...
extern void		CS_BtnFuncCreateChar(LONG lId, void* p, DWORD we);	 
extern void		CS_BtnFuncDeleteChar(LONG lId, void* p, DWORD we);	 
extern void		CS_BtnFuncFirstChar(LONG lId, void* p, DWORD we);	 
extern void		CS_BtnFuncSecondChar(LONG lId, void* p, DWORD we);	 
extern void		CS_BtnFuncThirdChar(LONG lId, void* p, DWORD we);	 
extern void		CS_BtnFuncFourthchar(LONG lId, void* p, DWORD we);	 
extern void		CS_BtnFuncFifthchar(LONG lId, void* p, DWORD we);	 
extern void		CS_BtnFuncEnter(LONG lId, void* p, DWORD we);
extern void		CS_FUNC_OkISee(LONG lId, void* p, DWORD we);	 //8->11
extern void		CS_BtnFuncLogOut(LONG lId, void* p, DWORD we);	 //9->12
// 061218 LYW --- Add callback function to process event from change serverlist button.
extern void		CS_BtnFuncChangeServer( LONG lId, void* p, DWORD we ) ;
// 061219 LYW --- Add function to process events from certificate dialog.
void CTF_CBFunc( LONG lId, void* p, DWORD we ) ;
//케리터 생성창 [5/13/2003]
// 061217 LYW --- Delete this code.
//extern void	CM_OverlapCheckBtnFunc(LONG lId, void* p, DWORD we);
//extern void	CM_ComboBoxCheckFunc(LONG lId, void* p, DWORD we);
extern void	CM_CharMakeBtnFunc(LONG lId, void* p, DWORD we);
extern void	CM_CharCancelBtnFunc(LONG lId, void* p, DWORD we);
// 061217 LYW --- Delete this code.
/*
extern void CMFUNC_OkISee(LONG lId, void* p, DWORD we); //5->16
extern void CM_OptionSelectBtnFunc(LONG lId, void* p, DWORD we);
*/

/*main interface*/
extern void MI_CharBtnFunc(LONG lId, void* p, DWORD we);
extern void MI_ExchangeBtnFunc(LONG lId, void* p, DWORD we);
extern void MI_InventoryBtnFunc(LONG lId, void* p, DWORD we);
extern void MI_SkillBtnFunc(LONG lId, void* p, DWORD we);
//extern void MI_SystemBtnFunc(LONG lId, void* p, DWORD we);
extern void MI_ChatEditBoxFunc(LONG lId, void * p, DWORD we); //7->23
extern void MI_DlgFunc(LONG lId, void * p, DWORD we);

/*Inventory interface*/
extern void IN_CreateStreetStallFunc(LONG lId, void * p, DWORD we);
extern void IN_DlgFunc(LONG lId, void * p, DWORD we);
/*Skill Dialog Interface*/
extern void MGI_DlgFunc(LONG lId, void * p, DWORD we);
extern void MGI_SkillDlgFunc(LONG lId, void * p, DWORD we);
extern void STD_SkillTreeDlgFunc(LONG lId, void * p, DWORD we);
extern void MGI_SuryunDlgFunc(LONG lId, void * p, DWORD we);
/*character infomation interface*/
extern void CI_DlgFunc(LONG lId, void * p, DWORD we);
extern void CI_AddExpPoint(LONG lId, void * p, DWORD we);

// 070111 LYW --- Add function to process events.
extern void CI_ActionEventFun( LONG lId, void* p, DWORD we ) ;

/*party interface*/
extern void PA_DlgFunc(LONG lId, void * p, DWORD we); //5->28

/*Quick Dialog Interface */
extern void QI_QuickDlgFunc(LONG lId, void * p, DWORD we);

// LYJ 051017 구입노점상 추가
extern void SO_DlgFunc(LONG lId, void * p, DWORD we);

extern void BRS_DlgFunc(LONG lId, void * p, DWORD we);
extern void BRS_CloseFunc(LONG lId, void * p, DWORD we);
extern void BRS_DeleteFunc(LONG lId, void * p, DWORD we);
extern void BRS_TypeListFunc(LONG lId, void * p, DWORD we);
extern void BRS_ItemListFunc(LONG lId, void * p, DWORD we);
extern void BRS_ClassListFunc(LONG lId, void * p, DWORD we);
extern void BRS_REGBtnFunc(LONG lId, void * p, DWORD we);
extern void BRS_VolumeEditFunc(LONG lId, void * p, DWORD we);
extern void BRS_MoneyEditFunc(LONG lId, void * p, DWORD we);

extern void BS_DlgFunc(LONG lId, void * p, DWORD we);
extern void BS_TitleEditBoxFunc(LONG lId, void * p, DWORD we);
extern void BS_MoneyEditBoxFunc(LONG lId, void * p, DWORD we);
extern void BS_SellBtnFunc(LONG lId, void * p, DWORD we);
extern void BS_REGBtnFunc(LONG lId, void * p, DWORD we);
extern void BS_SPFunc(LONG lId, void * p, DWORD we);

/*StreetStall interface*/
extern void SSI_DlgFunc(LONG lId, void * p, DWORD we);
extern void SSI_TitleEditBoxFunc(LONG lId, void * p, DWORD we);
extern void SSI_MoneyEditBoxFunc(LONG lId, void * p, DWORD we);
extern void SSI_BuyBtnFunc(LONG lId, void * p, DWORD we);
extern void SSI_RegistBtnFunc(LONG lId, void * p, DWORD we);
extern void SSI_EditBtnFunc(LONG lId, void * p, DWORD we);	//6->34

/*NPC 대사창*/
extern void NSI_DlgFunc(LONG lId, void * p, DWORD we);
extern void NSI_HyperLinkFunc(LONG lId, void * p, DWORD we);

/*공통창*/
extern void CMI_MoneyOkFunc(LONG lId, void * p, DWORD we);
extern void CMI_MoneySpinFunc(LONG lId, void * p, DWORD we);
extern void CMI_AlertFunc(LONG lId, void * p, DWORD we);

/*상점창*/
extern void DE_DlgFunc(LONG lId, void * p, DWORD we);
extern void DE_BuyItemFunc(LONG lId, void * p, DWORD we);
// 070503 LYW --- GlobalEventFunc : Add function to cancel sell dialog.
extern void DE_CancelFunc(LONG lId, void* p, DWORD we) ;
extern void DE_SellItemFunc(LONG lId, void * p, DWORD we);
extern void DE_CloseHideBtn(LONG lId, void * p, DWORD we); //7->41
/*파티*/
extern void PA_BtnFunc(LONG lId, void * p, DWORD we);

//080318 LUJ, 파티원 멤버 스킬 아이콘 표시 관리
extern void PartyMemberFunc( LONG lId, void * p, DWORD we );

/*길드*/
void GD_WarehouseFunc( LONG id, void* p, DWORD we );
void GD_DlgFunc( LONG id, void* p, DWORD we );
void GDMARK_DlgFunc( LONG id, void* p, DWORD we );
void GDLEVEL_DlgFunc( LONG id, void* p, DWORD we );
void GDRANK_DlgFunc( LONG id, void*, DWORD we );
void GDCREATE_DlgFunc( LONG id, void*, DWORD we );
void GDINVITE_DlgFunc( LONG id, void*, DWORD we );
void GDWHRANK_DlgFunc( LONG id, void*, DWORD we );
void GDREVIVAL_DlgFunc( LONG id, void*, DWORD we );
void GDUnion_DlgFunc( LONG id, void*, DWORD we );
void GDNICK_DlgFunc( LONG id, void*, DWORD we );

/*조합/분해/강화/인챈트를 위한 상태 진행 창*/
void PROGRESS_DlgFunc( LONG id, void*, DWORD we );

/*패밀리*/
void FD_DlgFunc( LONG lId, void* p, DWORD we );

/*표국*/
extern void PYO_DlgBtnFunc(LONG lId, void * p, DWORD we);
extern void Note_DlgBtnFunc(LONG lId, void * p, DWORD we);
extern void Friend_DlgBtnFunc(LONG lId, void * p, DWORD we);

/*전체외치기*/
extern void Shout_DlgBtnFunc(LONG lId, void * p, DWORD we);

/*낚시시스템*/
extern void Fishing_DlgBtnFunc(LONG lId, void * p, DWORD we);

/*캐릭터 재생*/
extern void CR_DlgBtnFunc(LONG lId, void * p, DWORD we);

/*현상금NPC*/
extern void WANTNPC_DlgFunc( LONG lId, void* p, DWORD we );
extern void WANTREG_DlgFunc( LONG lId, void* p, DWORD we );

/*퀘스트*/
extern void QUE_QuestDlgFunc( LONG lId, void* p, DWORD we );
extern void QUE_JournalDlgFunc( LONG lId, void* p, DWORD we );

/*수련장*/
extern void MP_RegistDlgFunc( LONG lId, void* p, DWORD we );

/*보석금*/
extern void BAIL_BailDlgFunc( LONG lId, void* p, DWORD we );

//KES DIALOG
/*매크로*/
//extern void MAC_ModeDlgFunc(LONG lId, void * p, DWORD we);
/*채팅창*/
// 061204 LYW --- Delete Preexistence Chatting Dialog.
/*
extern void CTI_DlgFunc(LONG lId, void * p, DWORD we);
*/
extern void COI_DlgFunc(LONG lId, void * p, DWORD we);
extern void GAMEIN_ChatFunc(LONG lId, void * p, DWORD we);
/*옵션창*/
extern void OTI_DlgFunc(LONG lId, void * p, DWORD we);
/*종료창*/
extern void EXT_DlgFunc(LONG lId, void * p, DWORD we);

// desc_hseos_몬스터미터_01
// S 몬스터미터 추가 added by hseos 2007.04.09
extern void MONSTERMETER_DlgFunc(LONG lId, void * p, DWORD we);
// E 몬스터미터 추가 added by hseos 2007.04.09

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23
extern void FARM_BUY_DlgFunc(LONG lId, void * p, DWORD we);
extern void FARM_UPGRADE_DlgFunc(LONG lId, void * p, DWORD we);
extern void FARM_MANAGE_DlgFunc(LONG lId, void * p, DWORD we);
// E 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23

// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2008.01.24
extern void DATE_ZONE_LIST_DlgFunc(LONG lId, void * p, DWORD we);
extern void CHALLENGE_ZONE_LIST_DlgFunc(LONG lId, void * p, DWORD we);
extern void CHALLENGE_ZONE_CLEARNO1_DlgFunc(LONG lId, void * p, DWORD we);
// E 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2008.01.24

/*교환창*/
extern void XCI_DlgFunc(LONG lId, void * p, DWORD we);
/*캐릭터상태창*/
extern void CSS_DlgFunc(LONG lId, void * p, DWORD we);
extern void MSI_MenuSlotDlgFunc( LONG lId, void* p, DWORD we );
/*PKLOOTING*/
extern void PLI_DlgFunc( LONG lId, void* p, DWORD we );
/*CNA_BtnOKFunc*/
extern void CNA_BtnOkFunc( LONG lId, void* p, DWORD we );
/*아이템분해*/
extern void DIS_DlgFunc( LONG lId, void* p, DWORD we );
extern void ITEM_DlgFunc( LONG lId, void* p, DWORD we );

/*무림넷*/
extern void MNPRI_DlgFunc(LONG lId, void * p, DWORD we);
extern void MNPRI_ChatFunc( LONG lId, void * p, DWORD we );
extern void MNCNL_DlgFunc(LONG lId, void * p, DWORD we);
extern void MNCNL_ChatFunc(LONG lId, void * p, DWORD we);

/*메세지박스*/
extern void MessageBox_Func(LONG lId, void * p, DWORD we);
extern void DIVIDEBOX_Func(LONG lId, void * p, DWORD we);
/*미니맵*/
extern void MNM_DlgFunc(LONG lId, void * p, DWORD we);

/*등급업 창*/
extern void ITD_DlgFunc(LONG lId, void * p, DWORD we);
/*조합 창*/
extern void ITMD_DlgFunc(LONG lId, void * p, DWORD we);
/* 강화 창*/
extern void ITR_DlgFunc(LONG lId, void * p, DWORD we);

// 080215 LUJ, 옵션 적용 창
extern void ApplyOptionDialogFunc(LONG lId, void * p, DWORD we);

/*채널 창*/
extern void CHA_DlgBtnFunc(LONG lId, void * p, DWORD we);

/*아이템몰창고*/
extern void ITMALL_DlgBtnFunc(LONG lId, void * p, DWORD we);

/*이동창*/
extern void SA_DlgBtnFunc(LONG lId, void * p, DWORD we);

/*저장위치*/
// 061204 LYW --- Delete Preexistence Chatting Dialog.
/*
extern void CHA_DlgFunc(LONG lId, void * p, DWORD we);
*/

// ServerList
extern void SL_DlgBtnFunc( LONG lId, void* p, DWORD we );

// guildfieldwar
extern void GFW_DlgFunc( LONG lId, void* p, DWORD we );

// SkillPoint Redistribute
extern void SK_DlgBtnFunc( LONG lId, void* p, DWORD we );

// partywar
extern void PW_DlgFunc( LONG lId, void* p, DWORD we );

// chase
extern void CHS_DlgFunc( LONG lId, void* p, DWORD we );

//
extern void CHAN_DlgFunc(LONG lId, void * p, DWORD we);

// Guild Tournament
extern void GDT_DlgFunc(LONG lId, void * p, DWORD we);

// SeigeWar
extern void SW_DlgFunc(LONG lId, void* p, DWORD we);

extern void IT_DlgFunc(LONG lId, void* p, DWORD we);

//
extern void CJOB_DlgFunc(LONG lId, void* p, DWORD we);

extern void CG_DlgFunc(LONG lId, void *p, DWORD we);

//extern void RFDefault_DlgFunc(LONG lId, void *p, DWORD we);
//extern void RareCreate_DlgFunc(LONG lId, void *p, DWORD we);

// 06. 02. 강화 재료 안내 인터페이스 추가 - 이영준
extern void RFGUIDE_DLGFunc(LONG lId, void *p, DWORD we);

// 06. 02. 내정보창 팁보기 추가 - 이영준
extern void TB_DlgFunc(LONG lId, void *p, DWORD we);
extern void TB_STATE_DlgFunc(LONG lId, void *p, DWORD we);

extern void GN_DlgBtnFunc(LONG lId, void *p, DWORD we);
extern void AN_DlgBtnFunc(LONG lId, void *p, DWORD we);

// 06. 03. 문파공지 - 이영준
extern void GNotice_DlgBtnFunc(LONG lId, void *p, DWORD we);

extern void GuildPlusTime_DlgFunc(LONG IId, void* p, DWORD we);//Add 060803 by wonju

// 061127 LYW --- Add Callback Function For New Chatting System.
void Chatting_DlgFunc( LONG lId, void* p, DWORD we ) ;
// 061217 LYW --- Add function to call callback function.
void CM_CMNCbFunc( LONG lId, void* p, DWORD we) ;
// 061219 LYW --- Add callback function to processing event from controls of main system dialog.
void MDS_CBFunc( LONG lId, void* p, DWORD we ) ;
// 070109 LYW --- Add callback function to processing event from controls of target dialog.
//void Target_BtnFunc( LONG lId, void* p, DWORD we ) ;
// 070110 LYW --- Add callback function to processing event from controls of character main dialog.
void CharMain_BtnFunc( LONG lId, void* p, DWORD we ) ;

extern void JO_DlgFunc(LONG lId, void* p, DWORD we);

extern void DMD_LogInOkBtnFunc( LONG lId, void* p, DWORD we ) ;		// 매칭 다이얼로그 테스트 2007/03/15 이진영
extern void DMD_DateMatchingDlgFunc( LONG lId, void* p, DWORD we ) ;		// 매칭 다이얼로그 테스트 2007/03/16 이진영

// 070607 LYW --- GlovalEventFunc : Add function to call callback function of partner idalog.
extern void DMD_PartnerDlgFunc( LONG lId, void* p, DWORD we ) ;

// 070606 LYW --- GlovalEventFunc : Add function to call callback function of identification dialog.
extern void DMD_IDT_Func(LONG lId, void* p, DWORD we) ;
// 070607 LYW --- GlovalEventFunc : Add function to call callback function of favor icon dialog.
void DMD_FID_Func(LONG lId, void* p, DWORD we) ;

// 070607 LYW --- GlovalEventFunc : Add function to call callback function of my info dialog.
void DMD_MID_Func(LONG lId, void* p, DWORD we) ;

// 070607 LYW --- GlovalEventFunc : Add function to call callback function of partner info dialog.
void DMD_PID_Func(LONG lId, void* p, DWORD we) ;

// 070607 LYW --- GlovalEventFunc : Add function to call callback function of record info dialog.
void DMD_RID_Func(LONG lId, void* p, DWORD we) ;

// 070618 LYW --- GlovalEventFunc : Add function to call callback function of key setting dialog.
void KS_Func(LONG lId, void* p, DWORD we) ;

extern void SkillTrainDlgFunc( LONG lId, void* p, DWORD we );

// desc_hseos_패밀리01
// S 패밀리 추가 added by hseos 2007.07.02
void FD_GuildDlg(LONG IId, void* p, DWORD we);
// E 패밀리 추가 added by hseos 2007.07.02

// 071018 LYW --- GlovalEventFunc : Add event function for tutorial dialog.
void TTR_DlgFunc( LONG id, void* p, DWORD we ) ;

// 071023 LYW --- GlovalEventFunc : Add event function for tutorial button dialog.
void TTR_BTNDlgFunc( LONG id, void* p, DWORD we ) ;

void ITM_BTNDlgFunc( LONG id, void* p, DWORD we ) ;

// 071024 LYW --- GlovalEventFunc : Add event function for helper dialog.
void HD_DlgFunc(LONG lId, void* p, DWORD we ) ;

// 071130 LYW --- GlovalEventFunc : Add event function for map move dialog.
void MapMove_DlgFunc(LONG lId, void* p, DWORD we ) ;

// 071228 LYW --- GlovalEventFunc : Add event function for change name dialog.
void Change_Name_Func(LONG lId, void* p, DWORD we ) ;

// 080109 LYW --- GlovalEventFunc : 숫자 패드 다이얼로그 함수 추가.
void NumberPad_DlgFunc(LONG lId, void* p, DWORD we ) ;

//---KES AUTONOTE
void AutoNoteDlg_Func( LONG lId, void* p, DWORD we );
void AutoAnswerDlg_Func( LONG lId, void* p, DWORD we );
//---------------

//---KES 상점검색
void StoreSearchDlg_Func( LONG lId, void* p, DWORD we );
//---------------

// 080414 LUJ, 외양 변경 창
void BodyChangeFunc( LONG lId, void* p, DWORD we );


// 080403 LYW --- GlovalEventFunc : Add event functions for chatroom system.
void ChatRoomMainDlg_Func(LONG lId, void* p, DWORD we ) ;
void ChatRoomCreateDlg_Func(LONG lId, void* p, DWORD we ) ;
void ChatRoomDlg_Func(LONG lId, void* p, DWORD we ) ;
void ChatRoomGuestListDlg_Func(LONG lId, void* p, DWORD we ) ;
void ChatRoomJoinDlg_Func(LONG lId, void* p, DWORD we ) ;
void ChatRoomOptionDlg_Func(LONG lId, void* p, DWORD we ) ;
//void ChatRoomLobbyDlg_Func(LONG lId, void* p, DWORD we ) ;

// 080414 KTH -- 가축 다이얼로그 추가
extern void Animal_DlgBtnFunc(LONG lId, void * p, DWORD we);

void PET_STATUS_DLG_FUNC(LONG lId, void* p, DWORD we ) ;
void PET_RES_DLG_FUNC(LONG lId, void* p, DWORD we ) ;

//void MNM_DlgFunc(LONG lId, void * p, DWORD we);

