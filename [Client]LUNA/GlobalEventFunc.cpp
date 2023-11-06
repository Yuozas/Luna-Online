#include "stdafx.h"
#include "GlobalEventFunc.h"

#include "MainGame.h"
#include "CharMake.h"
#include "CharSelect.h"
#include "GameIn.h"
#include "MainTitle.h"
#include "cComboBoxEx.h"

#include "ObjectManager.h"
#include "MoveManager.h"
#include "AppearanceManager.h"

#include "WindowIdEnum.h"
#include "interface/cWindowManager.h"
#include "PartyManager.h"
#include "GuildManager.h"
//#include "FamilyManager.h"
#include "StorageManager.h"
#include "FriendManager.h"
#include "NoteManager.h"
#include "ChatManager.h"
#include "ItemManager.h"
#include "ExchangeManager.h"
#include "StreetStallManager.h"
#include "ShowdownManager.h"
#include "FilteringTable.h"
#include "CharMakeManager.h"
#include "WantedManager.h"

#include "ChannelDialog.h"
#include "CharacterDialog.h"
#include "InventoryExDialog.h"
#include "MixDialog.h"
#include "ProgressDialog.h"
// LYJ 051017 구입노점 추가
#include "StallKindSelectDlg.h"
#include "StreetBuyStall.h"
#include "BuyRegDialog.h"
#include "StreetStall.h"
#include "BuyItem.h"

#include "cListDialogEx.h"
#include "NpcScriptDialog.h"
#include "HelpDialog.h"
#include "HelperManager.h"
#include "HelperSpeechDlg.h"
#include "MoneyDlg.h"
#include "DealDialog.h"
#include "MiniFriendDialog.h"
#include "FriendDialog.h"
#include "MiniNoteDialog.h"
#include "NoteDialog.h"
#include "ChatOptionDialog.h"
#include "OptionDialog.h"
#include "ExchangeDialog.h"
#include "CharStateDialog.h"
#include "MenuSlotDialog.h"
#include "PKLootingDialog.h"
#include "StorageDialog.h"
#include "MiniMapDlg.h"
#include "ExitDialog.h"
//#include "EnchantDialog.h"
//#include "MixDialog.h"
#include "ReinforceDlg.h"
//#include "DissolveDialog.h"
#include "WantNpcDialog.h"
#include "WantRegistDialog.h"
#include "QuestDialog.h"
#include "QuestQuickViewDialog.h"
#include "JournalDialog.h"
#include "PartyCreateDlg.h"
#include "PartyInviteDlg.h"
#include "QuestManager.h"
#include "GuildDialog.h"
//#include "GuildRankDialog.h"
//#include "GuildMarkDialog.h"
//#include "GuildNickNameDialog.h"
//#include "GuildWarehouseDialog.h"
#include "FamilyDialog.h"
#include "FamilyRankDialog.h"
#include "FamilyMarkDialog.h"
#include "FamilyNickNameDialog.h"
#include "FamilyWarehouseDialog.h"
#include "MHMap.h"
#include "MHCamera.h"

#include "cMsgBox.h"
#include "cDivideBox.h"
#include "./input/UserInput.h"
#include "./Audio/MHAudioManager.h"
#include "cImeEx.h"
#include "PKManager.h"
#include "ObjectStateManager.h"
#include "ReviveDialog.h"
// 061220 LYW --- Delete this code.
//#include "MainBarDialog.h"
#include "UserInfoManager.h"
#include "JournalManager.h"
#include "ExitManager.h"
#include "BailDialog.h"
#include "InventoryExDialog.h"
#include "PartyIconManager.h"
#include "ExchangeItem.h"

//#include "GuildMarkManager.h"
//#include "FamilyMarkManager.h"
#include "ItemShopDialog.h"
#include "MoveDialog.h"
#include "PointSaveDialog.h"
#include "GuildFieldWarDialog.h"
#include "GuildFieldWar.h"
#include "MallNoticeDialog.h"
#include "Shellapi.h"
#include "SkillPointRedist.h"
#include "SkillPointNotify.h"
#include "BarIcon.h"
// 061220 LYW --- Delete this code.
//#include "MainBarDialog.h"
#include "GameEventManager.h"
#include "PartyWar.h"
#include "ShoutDialog.h"
#include "ChaseDialog.h"
#include "ChaseinputDialog.h"
#include "NameChangeDialog.h"
#include "NameChangeNotifyDlg.h"

// 071002 LYW --- GlobalEventFunc : Include header file of checkbox class.
#include "PartyBtnDlg.h"

// 070109 LYW --- Include monster guage dilaog's header file.
#include "MonsterGuageDlg.h"

// Guild Tournament
#include "GTRegistDialog.h"
#include "GTRegistcancelDialog.h"
#include "GTStandingDialog.h"
#include "GTBattleListDialog.h"
#include "GTScoreInfoDialog.h"

// guildunion
#include "GuildUnion.h"
#include "GuildUnionMarkMgr.h"
//#include "FamilyUnion.h"
//#include "FamilyUnionMarkMgr.h"

#include "SeigeWarDialog.h"
#include "SealDialog.h"
#include "ChangeJobDialog.h"
#include "SiegeWarMgr.h"
#include "GuageDialog.h"
//#include "ReinforceResetDlg.h"
//#include "RareCreateDialog.h"
#include "ReinforceGuideDialog.h"
#include "TipBrowserDlg.h"

/////////////
//---MurimNet
#include "MNStgPlayRoom.h"
#include "MNPlayRoomDialog.h"

#include "MNStgChannel.h"
#include "MNChannelDialog.h"

#include "MurimNet.h"
#include "MNPlayerManager.h"
#include "MNPlayer.h"

#include "ServerListDialog.h"

#include "../Interface/cResourceManager.h"

//#include "GuildNoteDlg.h"
//#include "UnionNoteDlg.h"
//#include "GuildNoticeDlg.h"

//#include "guildjoindialog.h"
#include "familyjoindialog.h"

//#include "GuildPlusTimeDialog.h"

#include "cSkillTreeDlg.h"
#include "ChattingDlg.h"
#include "CharMakeNewDlg.h"
#include "CertificateDlg.h"
#include "MainSystemDlg.h"
#include "NpcScriptManager.h"
#include "ActionTarget.h"
#include "NpcImageDlg.h"

#include "cJobSkillDlg.h"

#include "QuickDlg.h"
#include "cSkillTrainingDlg.h"

#include "DateMatchingDlg.h"

#include "../KeySettingTipDlg.h"

#include "../hseos/ResidentRegist/SHResidentRegistManager.h"

#include "../hseos/family/shfamilymanager.h"										// 패밀리 매니져 해더파일을 불러온다.

// desc_hseos_패밀리01
// S 패밀리 추가 added by hseos 2007.07.04
#include "../hseos/Family/SHFamilyManager.h"
// E 패밀리 추가 added by hseos 2007.07.04

#include "QuestTotalDialog.h"

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23
#include "../hseos/Farm/SHFarmManager.h"
#include "../hseos/Farm/SHFarmBuyDlg.h"
#include "../hseos/Farm/SHFarmUpgradeDlg.h"
#include "../hseos/Farm/SHFarmManageDlg.h"
// E 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23

// 071025 LYW --- GlovalEventFunc : Include tutorial manager.
#include "TutorialManager.h"

// 071025 LYW --- GlovalEventFunc : Inlucde tutorial button dialog.
#include "TutorialBtnDlg.h"

// 071201 LYW --- GlovalEventFunc : Include map move dialog.
#include "cMapMoveDialog.h"

// 080109 LYW --- GlobalEventFunc : 숫자 패드 추가 처리.
#include "./Interface/cNumberPadDialog.h"
// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2008.01.24
#include "../hseos/Date/SHDateZoneListDlg.h"
#include "../hseos/Date/SHChallengeZoneListDlg.h"
#include "../hseos/Date/SHChallengeZoneClearNo1Dlg.h"
// E 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2008.01.24
// desc_hseos_결혼_01
// S 결혼 추가 added by hseos 2007.12.11
#include "../hseos/Marriage/SHMarriageManager.h"
// E 결혼 추가 added by hseos 2007.12.11


//---KES AUTONOTE
#include "AutoNoteDlg.h"
#include "AutoAnswerDlg.h"

//---KES 상점검색
#include "StoreSearchDlg.h"
//---------------

// 080403 LYW --- GlovalEventFunc : Include dialogs for chatroom system.
#include "./ChatRoomMgr.h"
#include "./ChatRoomMainDlg.h"
#include "./ChatRoomCreateDlg.h"
#include "./ChatRoomJoinDlg.h"
#include "./ChatRoomDlg.h"
#include "./ChatRoomGuestListDlg.h"
#include "./ChatRoomOptionDlg.h"
//#include "./ChatRoomLobbyDlg.h"
#include "FishingManager.h"
#include "FishingDialog.h"
#include "FishingPointDialog.h"

#include "petstatedialog.h"
#include "PetResurrectionDialog.h"

#include "../hseos/Farm/FarmAnimalDlg.h"

#include "petweareddialog.h"

extern HWND _g_hWnd;

// test
extern char g_DistributeAddr[16];
extern WORD g_DistributePort;
//
extern int	g_nServerSetNum;
extern ScriptCheckValue g_Check;

FUNC g_mt_func[] =
{
	{MT_LogInOkBtnFunc,"MT_LogInOkBtnFunc"},
	{MT_EditReturnFunc, "MT_EditReturnFunc"},
	{MT_ExitBtnFunc,"MT_ExitBtnFunc"},
	{MT_DynamicBtnFunc,"MT_DynamicBtnFunc"},
	{MT_DynamicEditReturnFunc, "MT_DynamicEditReturnFunc"},

	{CS_BtnFuncCreateChar,"CS_BtnFuncCreateChar"}, 
	{CS_BtnFuncDeleteChar,"CS_BtnFuncDeleteChar"},	 
	{CS_BtnFuncFirstChar,"CS_BtnFuncFirstChar"},	 
	{CS_BtnFuncSecondChar, "CS_BtnFuncSecondChar"},
	{CS_BtnFuncThirdChar,"CS_BtnFuncThirdChar"},
	{CS_BtnFuncFourthchar,"CS_BtnFuncFourthchar"},
	{CS_BtnFuncFifthchar,"CS_BtnFuncFifthchar"},
	{CS_BtnFuncEnter,"CS_BtnFuncEnter"},//¡E￠c¡§￠®OAO￠®¡×oAAU ¡§Ioo￠®¡×¡E￠c¡E￠c¡§￠® //10
	{CS_FUNC_OkISee,"CS_FUNC_OkISee"},
	{CS_BtnFuncLogOut,"CS_BtnFuncLogOut"},
	// 061218 LYW --- Add callback function to process events from change serverlist button.
	{CS_BtnFuncChangeServer, "CS_BtnFuncChangeServer"},
	// 061219 LYW --- Add callback function to process events from certificate dialog.
	{CTF_CBFunc, "CTF_CBFunc"},

	{MI_CharBtnFunc, "MI_CharBtnFunc"},
	{MI_ExchangeBtnFunc, "MI_ExchangeBtnFunc"},
	{MI_InventoryBtnFunc, "MI_InventoryBtnFunc"},
	{MI_SkillBtnFunc, "MI_SkillBtnFunc"},
	{MI_ChatEditBoxFunc, "MI_ChatEditBoxFunc"},

	{IN_CreateStreetStallFunc, "IN_CreateStreetStallFunc"},
	{IN_DlgFunc, "IN_DlgFunc"}, 
	{MGI_DlgFunc, "MGI_DlgFunc"},	//20
	{MGI_SuryunDlgFunc, "MGI_SuryunDlgFunc"},	
	{MGI_SkillDlgFunc, "MGI_SkillDlgFunc"},	
	{STD_SkillTreeDlgFunc, "STD_SkillTreeDlgFunc"},
	{CI_DlgFunc, "CI_DlgFunc"},
	{CI_AddExpPoint, "CI_AddExpPoint"},
	// 070111 LYW --- Add callback function to process events.
	{ CI_ActionEventFun, "CI_ActionEventFun" },
	{PA_DlgFunc, "PA_DlgFunc"},

	{SO_DlgFunc, "SO_DlgFunc"},

	{BRS_DlgFunc, "BRS_DlgFunc"},
	{BRS_CloseFunc, "BRS_CloseFunc"},
	{BRS_DeleteFunc, "BRS_DeleteFunc"},
	{BRS_TypeListFunc, "BRS_TypeListFunc"},
	{BRS_ItemListFunc, "BRS_ItemListFunc"},
	{BRS_ClassListFunc, "BRS_ClassListFunc"},
	{BRS_VolumeEditFunc, "BRS_VolumeEditFunc"},
	{BRS_MoneyEditFunc, "BRS_MoneyEditFunc"},
	{BRS_REGBtnFunc, "BRS_REGBtnFunc"},

	{BS_DlgFunc, "BS_DlgFunc"},
	{BS_TitleEditBoxFunc, "BS_TitleEditBoxFunc"},
	{BS_MoneyEditBoxFunc, "BS_MoneyEditBoxFunc"},
	{BS_SellBtnFunc, "BS_SellBtnFunc"}, 
	{BS_REGBtnFunc, "BS_REGBtnFunc"}, 
	{BS_SPFunc, "BS_SPFunc"},	

	{SSI_DlgFunc, "SSI_DlgFunc"},
	{SSI_TitleEditBoxFunc, "SSI_TitleEditBoxFunc"},
	{SSI_MoneyEditBoxFunc, "SSI_MoneyEditBoxFunc"},
	{SSI_BuyBtnFunc, "SSI_BuyBtnFunc"}, 
	{SSI_RegistBtnFunc, "SSI_RegistBtnFunc"}, //30
	{SSI_EditBtnFunc, "SSI_EditBtnFunc"}, 

	{NSI_DlgFunc, "NSI_DlgFunc"}, // NPC ￠®E￠®Ie¡E￠cicA¡E￠cE￠®E?￠®E¡Ec ¡E￠cic￠®E?e//
	{NSI_HyperLinkFunc, "NSI_HyperLinkFunc"},

	{CMI_MoneyOkFunc, "CMI_MoneyOkFunc"},
	{CMI_MoneySpinFunc, "CMI_MoneySpinFunc"}, 

	{DE_BuyItemFunc, "DE_BuyItemFunc"},//40
	{DE_SellItemFunc, "DE_SellItemFunc"},
	// 070503 LYW --- GlobalEventFunc : Add global event function .
	{ DE_CancelFunc, "DE_CancelFunc" },
	{DE_DlgFunc, "DE_DlgFunc"},
	{DE_CloseHideBtn, "DE_CloseHideBtn"},

	{PA_BtnFunc, "PA_BtnFunc"},

	// 080318 LUJ, 파티 멤버 창 이벤트 처리
	{PartyMemberFunc, "PartyMemberFunc" },

	{QI_QuickDlgFunc, "QI_QuickDlgFunc"},

	// 061217 LYW --- Delete this code.
	//{CM_OverlapCheckBtnFunc, "CM_OverlapCheckBtnFunc"},//AE¡E￠c¡E￠￥￠®¡×￠®ⓒ­AI ¡E￠ciy￠®¡×u￠®¡×￠®ⓒ­ A¡E￠cE ¡§Ioo￠®¡×¡E￠c¡E￠c¡§￠® CO￠®¡×uo
	{CM_CharMakeBtnFunc, "CM_CharMakeBtnFunc"},
	{CM_CharCancelBtnFunc, "CM_CharCancelBtnFunc"}, 
	// 061217 LYW --- Delete this code.
	/*
	{CM_ComboBoxCheckFunc,"CM_ComboBoxCheckFunc"}, 
	{CMFUNC_OkISee,"CMFUNC_OkISee"}, //50	
	{CM_OptionSelectBtnFunc, "CM_OptionSelectBtnFunc"},
	*/

	{PYO_DlgBtnFunc, "PYO_DlgBtnFunc"},
	{Note_DlgBtnFunc, "Note_DlgBtnFunc"},
	{Friend_DlgBtnFunc, "Friend_DlgBtnFunc"},
	{CR_DlgBtnFunc, "CR_DlgBtnFunc"},

	{MNM_DlgFunc,"MNM_DlgFunc"},
	{ITD_DlgFunc,"ITD_DlgFunc"},
	{ITMD_DlgFunc,"ITMD_DlgFunc"},  //60
	{ITR_DlgFunc, "ITR_DlgFunc"}, //

	// 080215 LUJ, 옵션 적용 창을 위한 함수
	{ApplyOptionDialogFunc, "ApplyOptionDialogFunc"},
	{CHA_DlgBtnFunc, "CHA_DlgBtnFunc"},

	//KES DIALOG
	// 061212 LYW --- Delete this code for a moment.
	//{CTI_DlgFunc, "CTI_DlgFunc"},	//A￠®E¡E¡I￠®¡×¡E￠cAA¡E￠cE
	{COI_DlgFunc, "COI_DlgFunc"},	//A￠®E¡E¡I￠®¡×¡E￠cA￠®E?E￠®¡×uCA¡E￠cE
	{OTI_DlgFunc, "OTI_DlgFunc"},	//¡E￠c¡§￠®OAO￠®E?E￠®¡×uCA¡E￠cE
	{EXT_DlgFunc, "EXT_DlgFunc"},	//A￠®¡×u¡E￠c¡E￠￥aA¡E￠cE	//

	// desc_hseos_몬스터미터_01
	// S 몬스터미터 추가 added by hseos 2007.04.09
	{MONSTERMETER_DlgFunc, "MONSTERMETER_DlgFunc"},
	// E 몬스터미터 추가 added by hseos 2007.04.09

	{XCI_DlgFunc, "XCI_DlgFunc"},	//¡E￠c¡§u¡§I¨I¨￡E?A¡E￠cE
	{CSS_DlgFunc, "CSS_DlgFunc"},	//A¡§I¨I¨￡￠®E￠®ⓒ­?AI¡E￠cioAAA¡E￠cE 
	{MSI_MenuSlotDlgFunc, "MSI_MenuSlotDlgFunc"},	//70
	{PLI_DlgFunc, "PLI_DlgFunc"},
	{CNA_BtnOkFunc, "CNA_BtnOkFunc"},
	{DIS_DlgFunc, "DIS_DlgFunc"},	//ºÐCØ
	{MI_DlgFunc, "MI_DlgFunc"},
	{DIVIDEBOX_Func, "DIVIDEBOX_Func"},	//Divide Box
	{ITEM_DlgFunc, "ITEM_DlgFunc" },
	/////////////
	//---MurimNet
	{MNPRI_DlgFunc, "MNPRI_DlgFunc"},
	{MNCNL_DlgFunc, "MNCNL_DlgFunc"}, 
	/////////////
	{SL_DlgBtnFunc, "SL_DlgBtnFunc" },

	{WANTNPC_DlgFunc, "WANTNPC_DlgFunc"},
	{WANTREG_DlgFunc, "WANTREG_DlgFunc"},

	{QUE_QuestDlgFunc, "QUE_QuestDlgFunc"},
	{QUE_JournalDlgFunc, "QUE_JournalDlgFunc"},	//80
	{MP_RegistDlgFunc, "MP_RegistDlgFunc"},

	{BAIL_BailDlgFunc, "BAIL_BailDlgFunc"},
	{ITMALL_DlgBtnFunc, "ITMALL_DlgBtnFunc"},
	{SA_DlgBtnFunc, "SA_DlgBtnFunc"},
	// 061212 LYW --- Delete this code for a moment.
	//{CHA_DlgFunc, "CHA_DlgFunc"},

	{GD_WarehouseFunc,	"GD_WarehouseFunc"	},
	{GD_DlgFunc,		"GD_DlgFunc"		},
	{GDREVIVAL_DlgFunc,	"GDREVIVAL_DlgFunc"	},
	{GDLEVEL_DlgFunc,	"GDLEVEL_DlgFunc"	},
	{GDRANK_DlgFunc,	"GDRANK_DlgFunc"	},
	{GDMARK_DlgFunc,	"GDMARK_DlgFunc"	},
	{GDCREATE_DlgFunc,	"GDCREATE_DlgFunc"	},
	{GDINVITE_DlgFunc,	"GDINVITE_DlgFunc"	},
	{GDWHRANK_DlgFunc,	"GDWHRANK_DlgFunc"	},
	{GDUnion_DlgFunc,	"GDUnion_DlgFunc"	},
	{GDNICK_DlgFunc,	"GDNICK_DlgFunc"	},

	{PROGRESS_DlgFunc, "PROGRESS_DlgFunc" },

	{FD_DlgFunc, "FD_DlgFunc"},

	// guildfieldwar
	{GFW_DlgFunc, "GFW_DlgFunc" },

	{SK_DlgBtnFunc, "SK_DlgBtnFunc"},

	// partywar
	{PW_DlgFunc, "PW_DlgFunc"},

	// chase
	{CHS_DlgFunc, "CHS_DlgFunc"},

	//
	{CHAN_DlgFunc, "CHAN_DlgFunc"},

	// Guild Tournament
	{GDT_DlgFunc, "GDT_DlgFunc"},

	// seigewar
	{SW_DlgFunc, "SW_DlgFunc" },

	{IT_DlgFunc, "IT_DlgFunc"},

	//
	{CJOB_DlgFunc, "CJOB_DlgFunc"},

	{CG_DlgFunc, "CG_DlgFunc"},

	// Reinforce Reset
	//{RFDefault_DlgFunc, "RFDefault_DlgFunc"},

	//{RareCreate_DlgFunc, "RareCreate_DlgFunc"},



	// 06. 02. 강화 재료 안내 인터페이스 추가 - 이영준
	{RFGUIDE_DLGFunc, "RFGUIDE_DLGFunc"},
	// 06. 02. 내정보탕 팁보기 추가 - 이영준
	{TB_DlgFunc, "TB_DlgFunc"},
	{TB_STATE_DlgFunc, "TB_STATE_DlgFunc"},

	{GN_DlgBtnFunc, "GN_DlgBtnFunc"},
	{AN_DlgBtnFunc, "AN_DlgBtnFunc"},
	// 06. 03. 문파공지 - 이영준
	{GNotice_DlgBtnFunc, "GNotice_DlgBtnFunc"},

	//060802 GuildPlusTime - wonju
	{GuildPlusTime_DlgFunc, "GuildPlusTime_DlgFunc"},

	// 061127 LYW --- Add callback function for new chatting system.
	{ Chatting_DlgFunc, "Chatting_DlgFunc"},
	// 061217 LYW --- Add function to call callback function.
	{ CM_CMNCbFunc, "CM_CMNCbFunc" },
	// 061219 LYW --- Add callback function to processing event from controls of main system dialog.
	{ MDS_CBFunc, "MDS_CBFunc" },
	// 070109 LYW --- Add callback function to processing event from controls of target dialog.
	//{ Target_BtnFunc, "Target_BtnFunc" },
	// 070110 LYW --- Add callback function to processing event from controls of character main dialog.
	{ CharMain_BtnFunc, "CharMain_BtnFunc" },

	{JO_DlgFunc, "JO_DlgFunc"},

	{DMD_DateMatchingDlgFunc, "DMD_DateMatchingDlgFunc"},			// 매칭 다이얼로그 테스트. 2007/03/15 이진영

	// 070607 LYW --- GlovalEventFunc : Add function to call callback function of partner idalog.
	{DMD_PartnerDlgFunc, "DMD_PartnerDlgFunc"}, 

	// 070606 LYW --- GlovalEventFunc : Add function to call callback function of identification dialog.
	{DMD_IDT_Func, "DMD_IDT_Func"}, 

	// 070607 LYW --- GlovalEventFunc : Add function to call callback function of favor icon dialog.
	{DMD_FID_Func, "DMD_FID_Func"},

	// 070607 LYW --- GlovalEventFunc : Add function to call callback function of my info dialog.
	{DMD_MID_Func, "DMD_MID_Func"}, 

	// 070607 LYW --- GlovalEventFunc : Add function to call callback function of partner info dialog.
	{DMD_PID_Func, "DMD_PID_Func"}, 

	// 070607 LYW --- GlovalEventFunc : Add function to call callback function of record info dialog.
	{DMD_RID_Func, "DMD_RID_Func"}, 

	// 070618 LYW --- GlovalEventFunc : Add function to call callback function of key setting dialog.
	{KS_Func, "KS_Func"}, 

	{SkillTrainDlgFunc, "SkillTrainDlgFunc"},

	// desc_hseos_패밀리01
	// S 패밀리 추가 added by hseos 2007.07.02
	{FD_GuildDlg,			"FD_GuildDlg"},
	// E 패밀리 추가 added by hseos 2007.07.02

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23
	{FARM_BUY_DlgFunc,		"FARM_BUY_DlgFunc"},
	{FARM_UPGRADE_DlgFunc,	"FARM_UPGRADE_DlgFunc"},
	{FARM_MANAGE_DlgFunc ,	"FARM_MANAGE_DlgFunc"},
	// E 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23

	// 071018 LYW --- GlovalEventFunc : Add event function for tutorial dialog.
	{TTR_DlgFunc, "TTR_DlgFunc"}, 

	// 071023 LYW --- GlovalEventFunc : Add envet function for tutorial button dialog.
	{TTR_BTNDlgFunc, "TTR_BTNDlgFunc"}, 
	{ITM_BTNDlgFunc, "ITM_BTNDlgFunc"}, 

	// 071024 LYW --- GlovalEventFunc : Add event function for helper dialog.
	{HD_DlgFunc, "HD_DlgFunc"}, 

	// 071130 LYW --- GlovalEventFunc : Add event function for map move dialog.
	{MapMove_DlgFunc, "MapMove_DlgFunc"}, 

	// 071228 LYW --- GlovalEventFunc : 캐릭터 이름 변경 관련 추가.
	{Change_Name_Func, "Change_Name_Func"}, 

	// 080109 LYW --- GlovalEventFunc : 숫자 패드 다이얼로그 함수 추가.
	{NumberPad_DlgFunc, "NumberPad_DlgFunc"}, 

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2008.01.24
	{DATE_ZONE_LIST_DlgFunc ,		"DATE_ZONE_LIST_DlgFunc"},
	{CHALLENGE_ZONE_LIST_DlgFunc ,	"CHALLENGE_ZONE_LIST_DlgFunc"},
	{CHALLENGE_ZONE_LIST_DlgFunc ,	"CHALLENGE_ZONE_CLEARNO1_DlgFunc"},
	// E 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2008.01.24

//---KES AUTONOTE
	{AutoNoteDlg_Func, "AutoNoteDlg_Func"},
	{AutoAnswerDlg_Func, "AutoAnswerDlg_Func"},
//---------------

//---KES 상점검색
	{StoreSearchDlg_Func, "StoreSearchDlg_Func"},
//---------------
// 080313 NYJ --- 전체외치기Func
	{Shout_DlgBtnFunc, "Shout_DlgBtnFunc"},

	// 080414 LUJ, 외양 변경 창
	{BodyChangeFunc, "BodyChangeFunc"},

	// 080403 LYW --- GlobalEventFunc : Add event function for chatroom system.
	{ChatRoomMainDlg_Func,		"ChatRoomMainDlg_Func"},
	{ChatRoomCreateDlg_Func,	"ChatRoomCreateDlg_Func"},
	{ChatRoomDlg_Func,			"ChatRoomDlg_Func"}, 
	{ChatRoomGuestListDlg_Func, "ChatRoomGuestListDlg_Func"}, 
	{ChatRoomJoinDlg_Func,		"ChatRoomJoinDlg_Func"}, 
	{ChatRoomOptionDlg_Func,	"ChatRoomOptionDlg_Func"},
	{ChatRoomMainDlg_Func,		"ChatRoomMainDlg_Func"},
// 080410 NYJ --- 낚시시스템Func
	{Fishing_DlgBtnFunc, "Fishing_DlgBtnFunc"},
	{Animal_DlgBtnFunc,	"Animal_DlgBtnFunc"},

	{PET_STATUS_DLG_FUNC, "PET_STATUS_DLG_FUNC"},
	{PET_RES_DLG_FUNC, "PET_RES_DLG_FUNC"},

	{NULL, ""},	//마지막줄에 이거 꼭 놔두세요.
};

int FUNCSEARCH(char * funcName)
{
	for(int i = 0 ;; i++)
	{
		if(g_mt_func[i]._func == NULL)
			break;
		if(strcmp(funcName, g_mt_func[i]._funcName)==0)
			return i;
	}

	return -1; 
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// 061217 LYW --- Delete this code.
/*
extern void CMFUNC_OkISee(LONG lId, void* p, DWORD we)
{
	cStatic * CharacterMessageDlg = (cStatic *)WINDOWMGR->GetWindowForIDEx(CMID_MessageBox);
	CharacterMessageDlg->SetActive(FALSE);

}
*/

////////////////////////////////////// NewCharSelect /////////////////////////////////////////////
// 061218 LYW --- Add callback function to process event from change serverlist button.
void CS_BtnFuncChangeServer( LONG lId, void* p, DWORD we )
{
	CHARSELECT->BackToMainTitle();
	TITLE->SetServerList();
	CAMERA->ResetInterPolation();
}

void CS_BtnFuncLogOut(LONG lId, void* p, DWORD we)
{
	// 061218 LYW --- Adjust this code to exit.
	/*
	CHARSELECT->BackToMainTitle();
	TITLE->SetServerList();
	CAMERA->ResetInterPolation();
	*/
		PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
}
void CS_BtnFuncCreateChar(LONG lId, void* p, DWORD we)
{
	// AE￠®E￠®ⓒ­? ￠®E￠®ⓒ­￠®E￠®ⓒ­￠®Iie¡E￠c¡§ua¡E￠c¡E￠￥I AI￠®Ii￠®E?.. [5/21/2003]
	if(CHARSELECT->IsFull() == FALSE)
	{
		MAINGAME->SetGameState(eGAMESTATE_CHARMAKE);

		//
		CAMERA->MoveStart(eCP_CHARMAKE);
	}
	else
	{		
		// 070125 LYW --- CharSelect : Modified message number.
		//CHARSELECT->DisplayNotice(1340);
		CHARSELECT->DisplayNotice(17);
	}
}

//￠®E￠®I¡E￠c¡§I¡E￠c¡E￠￥?￠®E￠®ⓒ­￠®E￠®ⓒ­Ao¡E￠c¡§￠®i ￠®¡×uE￠®¡×¡E￠c¡E￠cE￠®¡×ui¡§I¨I¨￡￠®¡×￠®I￠®E?A￠®E￠®IA C￠®¡×￠®I￠®¡×o¡E￠c¨I¡ⓒ¡§Ioo￠®¡×¡E￠c¡E￠c¡§￠®AI CE￠®E?aCI￠®E￠®IU.
void		CS_BtnFuncFirstChar(LONG lId, void* p, DWORD we)
{
	if( we & WE_PUSHDOWN )
		CHARSELECT->SelectPlayer(0);
	else if( we & WE_PUSHUP )
		//		((cPushupButton*)WINDOWMGR->GetWindowForIDEx( lId ))->SetPush( TRUE );
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( TRUE );
}
void		CS_BtnFuncSecondChar(LONG lId, void* p, DWORD we)
{
	if( we & WE_PUSHDOWN )
		CHARSELECT->SelectPlayer(1);
	else if( we & WE_PUSHUP )
		//		((cPushupButton*)WINDOWMGR->GetWindowForIDEx( lId ))->SetPush( TRUE );
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( TRUE );
}
void		CS_BtnFuncThirdChar(LONG lId, void* p, DWORD we)
{
	if( we & WE_PUSHDOWN )
		CHARSELECT->SelectPlayer(2);
	else if( we & WE_PUSHUP )
		//		((cPushupButton*)WINDOWMGR->GetWindowForIDEx( lId ))->SetPush( TRUE );
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( TRUE );
}
void		CS_BtnFuncFourthchar(LONG lId, void* p, DWORD we)
{
	if( we & WE_PUSHDOWN )
		CHARSELECT->SelectPlayer(3);
	else if( we & WE_PUSHUP )
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( TRUE );
}
void		CS_BtnFuncFifthchar(LONG lId, void* p, DWORD we)
{
	if( we & WE_PUSHDOWN )
		CHARSELECT->SelectPlayer(4);
	else if( we & WE_PUSHUP )
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( TRUE );
}
void CS_BtnFuncEnter(LONG lId, void* p, DWORD we)
{
	if(	CHARSELECT->GetCurSelectedPlayer() == NULL)
		CHARSELECT->DisplayNotice(18);
	else
	{
		CHARSELECT->SendMsgGetChannelInfo();
		CHARSELECT->SetDisablePick(TRUE);

		// 카메라 이동보간 리셋, 하이트필드 적용
		//		CAMERA->ResetInterPolation();
	} 
}

void CS_FUNC_OkISee(LONG lId, void* p, DWORD we)	 
{
	cStatic * CharacterSelectMessageDlg = (cStatic *)WINDOWMGR->GetWindowForIDEx(CSID_MessageBox);
	CharacterSelectMessageDlg->SetActive(FALSE);
}

void CS_BtnFuncDeleteChar(LONG lId, void* p, DWORD we)
{
	int nChar = CHARSELECT->GetCurSelectedPlayerNum();
	int MaxChar = MAX_CHARACTER_NUM-1;

	// 070202 LYW --- CharSelect : Modified position of msg box about delete character.
	/*
	int msgposx[MAX_CHARACTER_NUM] = { 130, 370, 550, 500 };
	int msgposy[MAX_CHARACTER_NUM] = { 350, 490, 360, 520 };

	if( nChar >= 0 && nChar <= MaxChar )
	{
		// 070122 LYW --- Modified this line.
		//cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_DELETECHAR, MBT_YESNO, CHATMGR->GetChatMsg(282) );
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_DELETECHAR, MBT_YESNO, RESRCMGR->GetMsg( 267 ) );
		if( pMsgBox )
			pMsgBox->SetAbsXY( msgposx[nChar], msgposy[nChar] );
		//			pMsgBox->SetAbsXY( nChar * 180 + 150, 450 );
		CHARSELECT->SetDisablePick( TRUE );
	}
	else
	{
		// AE￠®E￠®ⓒ­?AI￠®E￠®ⓒ­| ￠®¡×u¡E￠c¡§uAACN EA ¡E￠cieA| ¡§Ioo￠®¡×¡E￠c¡E￠c¡§￠®A¡E￠ci ￠®E￠®I¡E￠c¡§I¡E￠c¡E￠￥?AO￠®¡×u￠®¡×u￠®E?a.
		// 070125 LYW --- CharSelect : Modified message number.
		//CHARSELECT->DisplayNotice( 20 );
		CHARSELECT->DisplayNotice( 20 );
	}
	*/
		DISPLAY_INFO	dispInfo ;
	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;

	LONG Xpos = (LONG)(dispInfo.dwWidth/2 - 100) ;	// 100 is half of msgbox's width.
	LONG Ypos = (LONG)(dispInfo.dwHeight/2 - 57) ;	// 57 is half of msgbox's height.

	// 070205 LYW --- Add a conditional sentence.
	if( nChar >= 0 && nChar <= MaxChar )
	{
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_DELETECHAR, MBT_YESNO, RESRCMGR->GetMsg( 267 ) );

		if( pMsgBox )
			pMsgBox->SetAbsXY( Xpos, Ypos );
	}
	else
	{
		CHARSELECT->DisplayNotice( 20 );
	}

	CHARSELECT->SetDisablePick( TRUE );
}

// 061219 LYW --- Add function to process events from certificate dialog.
void CTF_CBFunc( LONG lId, void* p, DWORD we )
{
	CHARSELECT->GetCertificateDlg()->OnActionEvent( lId, p, we ) ;
}

// AE¡E￠c¡E￠￥￠®¡×￠®ⓒ­AI ¡E￠ciy￠®¡×u￠®¡×￠®ⓒ­A¡E￠cE CO￠®¡×uo [5/13/2003]
//////////////////////////////////////////////////////////////////////////
// 061217 LYW --- Delete this code.
/*
extern void	CM_OverlapCheckBtnFunc(LONG lId, void* p, DWORD we)
{
	//PJS ￠®E?￠®¡×u¡§I¨I¨￡?￠®¡×uO￠®¡×i¨Io￠®Ei¡§u￠®E?￠®E¡Ec￠®¡×u¡E￠c¡§I AU￠®Iia￠®E￠®ⓒ­| ¡§Ii¨Io￠®Ei¡§o￠®¡×u￠®¡×¡E￠c ¡§I¨I¨￡O￠®E￠®IA￠®E￠®IU. [5/13/2003]

	cEditBox * editboxName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(CMID_IDEDITBOX);
	char* pName = editboxName->GetEditText();
	int nLen	= strlen(pName);

	if( nLen == 0 )
	{
		//AI¡§IoI ￠®E￠®ⓒ­￠®¡×￠®¨I￠®¡×u￠®¡×uAo ¡§IoU￠®¡×i¨Io￠®Ei¡§u¡E￠c¡§￠®￠®E¡Ec ￠®EO¡E￠c¡§￠®AOA￠®E￠®ⓒ­￠®E￠®ⓒ­e ￠®¡×u¡§I¡§¡I￠®¡×uO¡E￠c¡§￠®i ￠®E￠®IU￠®¡×oA ￠®E￠®ⓒ­￠®E￠®ⓒ­￠®Iie¡E￠c¡§ua.
		//￠®¡×u￠®¡×¡E￠c￠®E￠®II￠®E￠®ⓒ­e MAKE DIALOG￠®E￠®ⓒ­| ￠®Ii¡§I￠®I￠®¡×i¨Io￠®Ei¡§u￠®E?￠®E¡EcAI￠®¡×￠®ⓒ­i...(¡E￠c¡§ua￠®E￠®IEAI AO￠®EO¡§I￠®"￠c¡§￠®￠®E¡Ec --; )
		CHARMAKE->DisplayNotice( 11 );
		return;
	}
	else if( nLen < 4 )
	{
		CHARMAKE->DisplayNotice( 19 );		
		return;
	}

	//¡E￠cic￠®E?eCIAo ￠®E￠®ⓒ­¡§I¡§¡ICI￠®E￠®IA ¡§Io￠®EcAU¡E￠c¡§￠®￠®E¡Ec ￠®Iie￠®¡×ui¡E￠c¡§￠®¡E￠c¨I¡ⓒ¡§I¨I¨￡￠®¡×￠®I?
	if( FILTERTABLE->IsInvalidCharInclude((unsigned char*)pName) )
	{
		//AI¡§IoI ￠®E￠®ⓒ­￠®¡×￠®¨I￠®¡×u￠®¡×uAo ¡§IoU￠®¡×i¨Io￠®Ei¡§u¡E￠c¡§￠®￠®E¡Ec ￠®EO¡E￠c¡§￠®AOA￠®E￠®ⓒ­￠®E￠®ⓒ­e ￠®¡×u¡§I¡§¡I￠®¡×uO¡E￠c¡§￠®i ￠®E￠®IU￠®¡×oA ￠®E￠®ⓒ­￠®E￠®ⓒ­￠®Iie¡E￠c¡§ua.
		//¡E￠cic￠®E?eCO ￠®¡×uo ￠®¡×u¡§I¡§¡I￠®E￠®IA A¡§I¨I¨￡￠®E￠®ⓒ­?￠®E￠®ⓒ­iAO￠®E￠®II￠®E￠®IU.
		CHARMAKE->DisplayNotice( 14 );
		return;
	}

	//¡E￠cic￠®E?eCIAo ￠®E￠®ⓒ­¡§I¡§¡ICI￠®E￠®IA A¡§I¨I¨￡￠®E￠®ⓒ­?￠®E￠®ⓒ­iAI¡E￠c¡§￠®￠®E¡Ec?
	if( !FILTERTABLE->IsUsableName(pName) )
	{
		CHARMAKE->DisplayNotice( 14 );
		return;
	}

	MSG_NAME msg;

	msg.Category = MP_USERCONN;
	msg.Protocol = 	MP_USERCONN_CHARACTER_NAMECHECK_SYN;
	strcpy(msg.Name,pName);
	NETWORK->Send(&msg,sizeof(msg));

	CHARMAKE->SetDisableDlg( TRUE );
}
*/
extern void	CM_CharMakeBtnFunc(LONG lId, void* p, DWORD we)
{
	// PJS ¡E￠cio¡E￠c¡E￠￥I ￠®E￠®ⓒ­￠®E￠®ⓒ­￠®Iic AE￠®E￠®ⓒ­?AIAC A￠®E¡E¡I￠®¡×￠®ⓒ­￠®E￠®ⓒ­￠®E￠®ⓒ­| ￠®¡×u¡E￠c¡§I¡§Ioo￠®E?￠®E¡Ec AO¡E￠c¡§￠®i ￠®E￠®IU￠®¡×oA AE¡E￠c¡E￠￥￠®¡×￠®ⓒ­AI ￠®¡×u¡E￠c¡§uAAA￠®E￠®ⓒ­¡E￠c¡E￠￥I ￠®Ii¡§Io￠®¡×u￠®¡×¡E￠c ¡E￠c¡§￠®¡E￠cI￠®E￠®IU. [5/13/2003]
	// 061217 LYW --- Change this code.
	//cEditBox * editboxName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(CMID_IDEDITBOX);
	cEditBox * editboxName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(CM_IDEDITBOX);
	char* pName = editboxName->GetEditText(); 
	int nLen	= strlen(pName);

	if( nLen == 0 )
	{
		// 070125 LYW --- CharMake : Modified message number.
		//CHARMAKE->DisplayNotice( 11 );
		CHARMAKE->DisplayNotice( 11 );
		return;
	}
	else if( nLen < 4 )
	{
		CHARMAKE->DisplayNotice( 19 );
		return;
	}

	//¡E￠cic￠®E?eCIAo ￠®E￠®ⓒ­¡§I¡§¡ICI￠®E￠®IA ¡§Io￠®EcAU¡E￠c¡§￠®￠®E¡Ec ￠®Iie￠®¡×ui¡E￠c¡§￠®¡E￠c¨I¡ⓒ¡§I¨I¨￡￠®¡×￠®I?
	if( FILTERTABLE->IsInvalidCharInclude((unsigned char*)pName) )
	{
		//AI¡§IoI ￠®E￠®ⓒ­￠®¡×￠®¨I￠®¡×u￠®¡×uAo ¡§IoU￠®¡×i¨Io￠®Ei¡§u¡E￠c¡§￠®￠®E¡Ec ￠®EO¡E￠c¡§￠®AOA￠®E￠®ⓒ­￠®E￠®ⓒ­e ￠®¡×u¡§I¡§¡I￠®¡×uO¡E￠c¡§￠®i ￠®E￠®IU￠®¡×oA ￠®E￠®ⓒ­￠®E￠®ⓒ­￠®Iie¡E￠c¡§ua.
		//¡E￠cic￠®E?eCO ￠®¡×uo ￠®¡×u¡§I¡§¡I￠®E￠®IA A¡§I¨I¨￡￠®E￠®ⓒ­?￠®E￠®ⓒ­iAO￠®E￠®II￠®E￠®IU.
		CHARMAKE->DisplayNotice( 14 );
		return;
	}

	//¡E￠cic￠®E?eCIAo ￠®E￠®ⓒ­¡§I¡§¡ICI￠®E￠®IA A¡§I¨I¨￡￠®E￠®ⓒ­?￠®E￠®ⓒ­iAI¡E￠c¡§￠®￠®E¡Ec?
	if( !FILTERTABLE->IsUsableName(pName) )
	{
		CHARMAKE->DisplayNotice( 14 );
		// 061219 LYW --- Setting to null editbox.
		cEditBox* pEdit = ( cEditBox* )CHARMAKE->GetCharMakeNewDlg()->GetWindowForID( CM_IDEDITBOX ) ;
		pEdit->SetEditText("") ;
		return;
	}


	// 061227 LYW --- Change this code.
	/*
	CHARACTERMAKEINFO msg;
	memcpy(&msg, CHARMAKEMGR->GetCharacterMakeInfo(), sizeof(msg) );
	*/
		CHARACTERMAKEINFO msg;
	ZeroMemory(&msg, sizeof(CHARACTERMAKEINFO)) ;
	CHARMAKEMGR->GetCharMakeNewDlg()->ApplyOption( &msg ) ;

	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTER_MAKE_SYN;
	msg.StandingArrayNum = -1;
	strcpy(msg.Name, editboxName->GetEditText());

	NETWORK->Send(&msg,sizeof(msg));

	CHARMAKE->SetDisableDlg( TRUE );
	//	CHARMAKE->GetCharMakeDialog()->SetDisable( TRUE );
}

extern void	CM_CharCancelBtnFunc(LONG lId, void* p, DWORD we)
{
	MSGBASE msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_DIRECTCHARACTERLIST_SYN;
	NETWORK->Send(&msg,sizeof(msg));	

	//
	CAMERA->MoveStart(eCP_CHARSEL);
}
// 061217 LYW --- Delete this code.
/*
extern void CM_ComboBoxCheckFunc( LONG lId, void* p, DWORD we )
{
	if( we != WE_COMBOBOXSELECT ) return;
	//	cComboBoxEx* pBox = (cComboBoxEx*)p;	//PPP

	cComboBoxEx* pBox = (cComboBoxEx*)((cDialog*)p)->GetWindowForID( lId );

	if( CHARMAKEMGR->ReplaceCharMakeInfo( (CHAR_M)pBox->GetOptionValue(), pBox->GetCurSelectedIdx() ) )
	{
		APPEARANCEMGR->AddCharacterPartChange(1);
		APPEARANCEMGR->SetScalebyGuageBar(1);
	}
	//KES
	WINDOWMGR->SetMouseDownUsed();
}
*/
// 061217 LYW --- Delete this code.
/*
extern void CM_OptionSelectBtnFunc(LONG lId, void* p, DWORD we)
{
	if( we != WE_BTNCLICK ) return;

	CHARMAKE->GetCharMakeDialog()->OnActionEvent(lId, p, we);
}
*/


/////////////////////////////////////// NewMainTitle ///////////////////////////////////////////
//#include "MainTitle.h"
extern char g_szHeroIDName[];
extern char g_CLIENTVERSION[];
extern BOOL alreadyDecypte;
//=========================================================================
//	NAME : MT_LogInOkBtnFunc
//	DESC : The function to management action event of all controls in main title. 080109 LYW
//=========================================================================
void MT_LogInOkBtnFunc(LONG lId, void* p, DWORD we)
{
	ScriptCheckValue check;
	SetScriptCheckValue( check );

	if( check.mValue != g_Check.mValue )
	{
#ifndef _GMTOOL_1
		// 090120 LUJ, 패치 버전을 최초로 돌린다
		{
			const char* const	versionFile = "LunaVerInfo.ver";
			FILE* const			file		= fopen( versionFile, "r+" );
			if( file )
			{
				// 090109 LUJ, 4글자로 구성된 버전 헤더를 파일에서 읽어와 문자열 변수에 담는다
				const size_t headerSize = 4;
				char header[ headerSize + 1 ] = { 0 };
				fread(
					header,
					headerSize,
					sizeof( *header ),
					file );

				// 090109 LUJ, 전체 패치를 수행할 버전 번호를 만든다
				char versionText[ MAX_PATH ] = { 0 };
				sprintf(
					versionText,
					"%s00000000",
					header );
				// 090109 LUJ, 패치 버전 문자열을 파일에 쓴다
				fseek(
					file,
					0,
					SEEK_SET );
				fwrite(
					versionText,
					sizeof( *versionText ),
					strlen( versionText ),
					file );
				fclose( file );
			}
		}

		PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
		return ;
#endif
	}
	if( !p ) return ;															// 외부에서 넘어오는 인자의 무결성을 꼭 확인하자.

	cDialog * dlg = NULL ;														// 다이얼로그 포인터를 선언하고 꼭 null 초기화 하자.
	dlg = (cDialog *)p ;														// 다이얼로그를 받는다.

	if( !dlg ) return ;															// 다이얼로그의 무결성을 체크하자.

	cNumberPadDialog* pPad = NULL ;
	pPad = ((cNumberPadDialog*)WINDOWMGR->GetWindowForID( NUMBERPAD_DLG )) ;//NUMBERPAD_DLG	// 숫자 패드 다이얼로그를 받는다.

	if( !pPad ) return ;														// 숫자 패드의 무결성을 체크하자.

	cEditBox * editboxId = NULL ;												// 아이디 에디트 박스를 받는다.
	editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX) ;					

	if( !editboxId ) return ;													// 아이디 에디트 박스의 무결성을 체크하자.

	cEditBox * editboxPwd = NULL ;												// 패스워드 에디트 박스를 받는다.
	editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX) ;				

	if( !editboxPwd ) return ;													// 패스워드 에디트 박스의 무결성을 체크하자.

	cDialog* pIDDlg = NULL ;
	pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG ) ;							// 로그인 다이얼로그를 받는다.

	if( !pIDDlg ) return ;														// 로그인 다이얼로그의 무결성을 체크한다.

	cMsgBox* pMsgBox = NULL ;													

	//--------------------------------------------------------------------------

	char * userID = editboxId->GetEditText() ;									// 아이디를 받는다.
	char * userPWD = editboxPwd->GetEditText() ;								// 패스워드를 받는다.

	//------------------------------------------------------------------------- // 아이디와 비밀 번호를 제대로 입력 했다면,

	if((strcmp(userID, "") != 0) && (strcmp(userPWD, "") != 0))					
	{
		MSG_LOGIN_SYN msg ;														// 로그인 정보를 담을 메시지 구조체를 선언한다.
		memset(&msg, 0, sizeof(MSG_LOGIN_SYN)) ;								// 메시지 구조체를 초기화 한다.

		msg.Category = MP_USERCONN ;											// 카테고리와 프로토콜을 세팅한다.
		msg.Protocol = MP_USERCONN_LOGIN_SYN ;

		SafeStrCpy(msg.id, userID, 21 ) ;						// 아이디 세팅.
		SafeStrCpy(msg.pw, userPWD, 21 ) ;						// 비밀번호 세팅.

		// 보안 비밀번호 추가 처리.
		strcpy(msg.strSecurityPW, pPad->GetNumberStr()) ;

		if ( ! alreadyDecypte )
		{
			for(int i=0;i<strlen(g_CLIENTVERSION);i++)
				g_CLIENTVERSION[i] ^= 8;
			alreadyDecypte = true;
		}

		SafeStrCpy(msg.Version,g_CLIENTVERSION, 16 ) ;							// 클라이언트 버전 세팅.

		msg.AuthKey = TITLE->GetDistAuthKey() ;									// 인증키를 담는다.
		
		msg.Check.mValue = check.mValue; //disable script check

		SafeStrCpy(
			msg.mLoginKey,
			TITLE->GetLoginKey().c_str(),
			_countof(msg.mLoginKey));
		NETWORK->Send(&msg,sizeof(msg)) ;										// 메시지를 전송한다.

//---KES 로그인 실패후 재 로그인이 잘 안되던 것 수정
		NETWORK->m_bDisconnecting = FALSE;
//---------------------------------------------

		SafeStrCpy(g_szHeroIDName, userID, MAX_NAME_LENGTH + 1 ) ;				// 아이디를 받는다.

		pMsgBox = WINDOWMGR->MsgBox( MBI_WAIT_LOGINCHECK,						// 로그인 중이라는 메시지 박스를 받는다.
								 MBT_CANCEL, 
								 RESRCMGR->GetMsg( 265 ) ) ;

		if( pMsgBox )
		{
			float fXpos = pIDDlg->GetAbsX() ;									// X좌표를 받는다.
			float fYpos = pIDDlg->GetAbsY() + pIDDlg->GetHeight() ;				// Y좌표를 받는다.

			pMsgBox->SetAbsXY( fXpos, fYpos ) ;									// 메시지 박스의 위치를 세팅한다.
		}

		TITLE->StartWaitConnectToAgent( TRUE ) ;								// 에이전트 연결을 기다린다.

		USERINFOMGR->SetUserID( userID ) ;										// 유저 아이디를 설정한다.
	}

	//------------------------------------------------------------------------- // 아이디와 패스워드 중 공란이 있다면,

	else	
	{
		cMsgBox* pMsgBox = NULL ;
		pMsgBox = WINDOWMGR->MsgBox(											// 아이디와 패스워드를 모두 입력하라는 메시지 박스를 받는다.
										MBI_LOGINEDITCHECK, 
										MBT_OK, 
										CHATMGR->GetChatMsg(208) 
									) ; 

		if( pMsgBox )
		{
			float fXpos = pIDDlg->GetAbsX() ;									// X좌표를 받는다.
			float fYpos = pIDDlg->GetAbsY() + pIDDlg->GetHeight() ;				// Y좌표를 받는다.

			pMsgBox->SetAbsXY( fXpos, fYpos ) ;									// 메시지 박스의 위치를 세팅한다.
		}
	}

	editboxId->SetFocusEdit(FALSE) ;											// 아이디, 비번 에디트 박스의 포커스를 해제 한다.
	editboxPwd->SetFocusEdit(FALSE) ;
	dlg->SetDisable( TRUE ) ;													// 아이디, 비번 창을 비활성화 한다.
}

void MT_DynamicBtnFunc(LONG lId, void * p, DWORD we)
{
	cDialog* dlg = (cDialog*)p;

	if( lId == MT_DYNAMICOKBTN )
	{
		cEditBox* editboxPwd = (cEditBox*)dlg->GetWindowForID(MT_DYNAMICPWDEDITBOX);
		char* userPWD = editboxPwd->GetEditText();
		if( strcmp(userPWD, "") != 0 )
		{
			MSG_LOGIN_DYNAMIC_SYN msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_LOGIN_DYNAMIC_SYN;
			msg.AuthKey = TITLE->GetDistAuthKey();
			SafeStrCpy( msg.id, g_szHeroIDName, 21 );
			SafeStrCpy( msg.pw, userPWD, 21 );
			NETWORK->Send( &msg, sizeof(msg) );

			cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID( MBI_WAIT_LOGINCHECK );
			if( pMsgBox )
			{
				pMsgBox->SetDisable( FALSE );
				pMsgBox->SetActive( TRUE );
				cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
				if( pIDDlg )
				{
					pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
				}
			}

			TITLE->StartWaitConnectToAgent( TRUE );
		}
		else
		{
			// 070126 LYW --- Modified this line.
			//cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_CANTUSEDYNAMIC, MBT_OK, CHATMGR->GetChatMsg(362) ); 
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_CANTUSEDYNAMIC, MBT_OK, CHATMGR->GetChatMsg(208) ); 
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
				// 080111 LYW --- GlovalEventFunc : 숫자 패드 비활성화 처리.
				cDialog* pDlg = NULL ;
				pDlg = WINDOWMGR->GetWindowForID( NUMBERPAD_DLG ) ;//NUMBERPAD_DLG

				if( !pDlg ) return ;

				pDlg->SetActive(FALSE) ;
			}
		}

		editboxPwd->SetFocusEdit(FALSE);
		dlg->SetDisable( TRUE );
	}
	else if( lId == MT_DYNAMICEXITBTN )
	{
		((cEditBox*)dlg->GetWindowForID(MT_DYNAMICPWDEDITBOX))->SetEditText( "" );

		dlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
		((cEditBox*)dlg->GetWindowForID(MT_IDEDITBOX))->SetEditText( "" );
		((cEditBox*)dlg->GetWindowForID(MT_PWDEDITBOX))->SetEditText( "" );

		TITLE->NoDisconMsg();
		TITLE->ShowServerList();
	}
}

void MT_DynamicEditReturnFunc( LONG lId, void* p, DWORD we )
{
	if(we == WE_RETURN)
	{
		cDialog* dlg = (cDialog*)p;		
		if( lId == MT_DYNAMICPWDEDITBOX )
		{
			MT_DynamicBtnFunc( MT_DYNAMICOKBTN, p, we );
		}
	}
}

void MT_EditReturnFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_RETURN)
	{
		cDialog* dlg = (cDialog*)p;
		cEditBox* pEdit;
		if( lId == MT_IDEDITBOX )
		{
			pEdit = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
			if( *pEdit->GetEditText() != 0 )
			{
				WINDOWMGR->SetNextEditFocus();
			}
		}
		else if( lId == MT_PWDEDITBOX )
		{
			pEdit = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
			if( *pEdit->GetEditText() != 0 )
			{
				MT_LogInOkBtnFunc( MT_OKBTN, p, WE_BTNCLICK );
			}
		}
	}
}

void MT_ExitBtnFunc(LONG lId, void * p, DWORD we)
{
	cDialog* dlg = (cDialog *)p;
	cEditBox* editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
	cEditBox* editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);

	editboxId->SetEditText("");
	editboxPwd->SetEditText("");
	editboxId->SetFocusEdit(TRUE);
	editboxPwd->SetFocusEdit(FALSE);

	TITLE->NoDisconMsg();
	TITLE->ShowServerList();

	// 080111 LYW --- MainTitle : 숫자 패드 비활성화 처리.
	cDialog* pDlg = NULL ;
	pDlg = WINDOWMGR->GetWindowForID( NUMBERPAD_DLG ) ;

	if( !pDlg ) return ;

	pDlg->SetActive(FALSE) ;
}

////////////////////////////////////// Main Interface //////////////////////////////////////////
void MI_CharBtnFunc(LONG lId, void* p, DWORD we)
{
	if(we == WE_PUSHUP)
	{
		GAMEIN->GetCharacterDialog()->SetActive(FALSE);
	}
	else if(we == WE_PUSHDOWN)
	{
		GAMEIN->GetCharacterDialog()->UpdateData();
		GAMEIN->GetCharacterDialog()->SetActive(TRUE);
	}
}
void MI_ExchangeBtnFunc(LONG lId, void* p, DWORD we)
{
	static BOOL run = 0;
	if(run)
	{
		MSGBASE msg;
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_WALKMODE;
		msg.dwObjectID = HEROID;
		NETWORK->Send(&msg,sizeof(msg));
		run = 0;
		MOVEMGR->SetWalkMode(HERO);
	}
	else
	{
		MSGBASE msg;
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_RUNMODE;
		msg.dwObjectID = HEROID;
		NETWORK->Send(&msg,sizeof(msg));
		run = 1;
		MOVEMGR->SetRunMode(HERO);
	}
}

void MI_InventoryBtnFunc(LONG lId, void* p, DWORD we)
{
	if(we == WE_PUSHUP)
	{
		GAMEIN->GetInventoryDialog()->SetActive(FALSE);
	}
	else if(we == WE_PUSHDOWN)
	{
		GAMEIN->GetInventoryDialog()->SetActive(TRUE);
	}
}

void MI_SkillBtnFunc(LONG lId, void* p, DWORD we)
{
	//MessageBox(NULL,"¡§Io¡E￠ci¡E￠c¡§￠®¡§I¡§¡IA¡E￠cE Aa¡E￠c¡E￠￥A",NULL,NULL);
	if(we == WE_PUSHUP)
	{
		GAMEIN->GetSkillTreeDlg()->SetActive(FALSE);
	}
	else if(we == WE_PUSHDOWN)
	{
		GAMEIN->GetSkillTreeDlg()->SetActive(TRUE);
	}

}
/*
//￠®¡×¡E￠cA￠®¡×¡E￠c￠®¡×uA¡E￠cE
void MI_PartyBtnFunc(LONG lId, void * p, DWORD we) 
{
	if(we == WE_PUSHUP) 
	{
		GAMEIN->GetPartyDialog()->SetActive(FALSE);
	}
	else if(we == WE_PUSHDOWN) 
	{
		GAMEIN->GetPartyDialog()->SetActive(TRUE);
		GAMEIN->GetPartyDialog()->RefreshDlg();
	} 
}
*/
void IN_DlgFunc(LONG lId, void * p, DWORD we)
{
	//	if(we == WE_CLOSEWINDOW)
	//	{
	//		GAMEIN->GetInventoryDialog()->SetActive(FALSE);
	//	}
	if(we == WE_LBTNCLICK ||/* we == WE_RBTNCLICK || */we == WE_RBTNDBLCLICK || we == WE_LBTNDBLCLICK || we == WE_RBTNDBLCLICK || WE_RBTNCLICK)
	{
		GAMEIN->GetInventoryDialog()->OnActionEvnet(lId, p, we);
	}

	/*if(lId == IN_SHOPITEMBTN)
	GAMEIN->GetInventoryDialog()->OnShopItemBtn();
	else if(lId == IN_TABBTN1 || lId == IN_TABBTN2 || lId == IN_TABBTN3 || lId == IN_TABBTN4)
		GAMEIN->GetInventoryDialog()->OffShopItemBtn(lId);		*/


		if(lId == IN_TABBTN1 || lId == IN_TABBTN2)
			GAMEIN->GetInventoryDialog()->OffShopItemBtn(lId);		
}
/*Skill Dialog Interface*/
void MGI_DlgFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_PUSHDOWN || we == WE_PUSHUP)
	{
		//		GAMEIN->GetSkillDialog()->ActiveMogongGrid( lId );
	}
}
void MGI_SkillDlgFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_LBTNCLICK || /*we == WE_RBTNCLICK ||*/ we == WE_RBTNDBLCLICK || we == WE_LBTNDBLCLICK)
	{
		//		GAMEIN->GetSkillDialog()->OnActionEvnet(lId, p, we);
	}
}
void STD_SkillTreeDlgFunc(LONG lId, void * p, DWORD we)
{
	if( lId == STD_SKILLTRAININGBTN && we & WE_BTNCLICK )
	{
		GAMEIN->GetSkillTrainingDlg()->OpenDialog();
	}
	else if( p )
	{
		( ( cJobSkillDlg* )p )->OnActionEvnet(lId, p, we);
	}
}

void MGI_SuryunDlgFunc(LONG lId, void * p, DWORD we)
{

}
/*character infomation interface*/
void CI_DlgFunc(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK && lId == CI_BESTTIP )
	{
		GAMEIN->GetTipBrowserDlg()->Show();
	}

	// 070114 LYW --- Process close button.
	if( lId == CI_CLOSEBTN )
	{
		GAMEIN->GetCharacterDialog()->SetActive( !( GAMEIN->GetCharacterDialog()->IsActive() ) );
		// 070115 LYW --- Close tree dialog.
		GAMEIN->GetCharacterDialog()->SetViewTreeDlg( FALSE ) ;
	}

	//	if(we == WE_CLOSEWINDOW)
	//	{
	//		GAMEIN->GetMainInterfaceDialog()->GetPushupBtn(2)->SetPush(FALSE);
	//	}
}
void CI_AddExpPoint(LONG lId, void * p, DWORD we)
{
	BYTE whats=STR_POINT;
	switch(lId)
	{
		// 070111 LYW --- Modified this part.
		/*
	case CI_STR_POINT:
	case CI_STR_POINT2:
		whats=STR_POINT;
		break;
	case CI_WIS_POINT:
	case CI_WIS_POINT2:
		whats=WIS_POINT;
		break;
	case CI_DEX_POINT:
	case CI_DEX_POINT2:
		whats=DEX_POINT;
		break;
	case CI_VIT_POINT:
	case CI_VIT_POINT2:
		whats=VIT_POINT;
		break;
		*/

	case CI_STATUS_BTN_0:
		whats=STR_POINT;
		break;
	case CI_STATUS_BTN_1:
		whats=DEX_POINT;
		break;
	case CI_STATUS_BTN_2:
		whats=VIT_POINT;
		break;
	case CI_STATUS_BTN_3:
		whats=INT_POINT ;
		break;
	case CI_STATUS_BTN_4 :
		whats=WIS_POINT;
		break ;
	}

	// 070114 LYW --- Modified this part.
	/*
	if( lId==CI_STR_POINT || lId==CI_WIS_POINT || lId==CI_DEX_POINT || lId==CI_VIT_POINT )
		GAMEIN->GetCharacterDialog()->OnAddPoint(whats);
	else
		GAMEIN->GetCharacterDialog()->OnMinusPoint(whats);
	*/

		if( lId==CI_STATUS_BTN_0 || lId==CI_STATUS_BTN_1 || lId==CI_STATUS_BTN_2 || lId==CI_STATUS_BTN_3 || lId==CI_STATUS_BTN_4 )
			GAMEIN->GetCharacterDialog()->OnAddPoint(whats);
}

// 070111 LYW --- Add function to process events.
void CI_ActionEventFun( LONG lId, void* p, DWORD we )
{
	GAMEIN->GetCharacterDialog()->OnActionEvent( lId, p, we ) ;
}

/*party dialog interface */
void PA_DlgFunc(LONG lId, void * p, DWORD we)
{
	//	if(we == WE_CLOSEWINDOW)
	//	{
	//	GAMEIN->GetMainInterfaceDialog()->GetPushupBtn(4)->SetPush(FALSE);
	//	}
}

//////////// A￠®E¡E¡I￠®¡×¡E￠cA A¡E￠cE ////////////////////////////////////////
void MI_ChatEditBoxFunc(LONG lId, void * p, DWORD we)			//00000
{
	// KES 030820 A￠®E¡E¡I￠®¡×¡E￠cA￠®E￠®ⓒ­￠®¡×￠®¨I￠®E￠®IIA￠®Ec¡E￠c¡E￠￥I ￠®E?A¡E￠c¡§￠®U ¡E￠c¡§￠®¡E￠c¨I¡ⓒA￠®¡×o
	/*
	if(we == WE_RETURN)
	{
	cDialog * dlg = (cDialog *)p;
	cEditBox * editbox = (cEditBox *)dlg->GetWindowForID(MI_CHATEDITBOX);
	char * msg = editbox->GetEditText();

	if( strlen(msg) != 0 )
	{
	BOOL bSend = TRUE;
	if(msg[0] == '/')	//¡E￠c¡§uO￠®E￠®ⓒ­¡E￠ciA¡§I¨I¨￡￠®E￠®ⓒ­￠®EcC￠®¡×¡§¡I￠®¡×u¡§I￠®ⓒ­CO
	{
	if(strlen(msg) == 1)
	bSend = FALSE;
	else
	bSend = CheatFunc(&msg[1]);
	}

	if(bSend)
	{
	TESTMSG data;
	data.Category = MP_CHAT;
	data.Protocol = MP_CHAT_ALL;
	data.dwObjectID	= HEROID;
	//strcpy(data.Msg, FILTERTABLE->Filetering(msg));
	strcpy(data.Msg, msg);
	NETWORK->Send(&data,sizeof(data));
	}

	editbox->SetEditText("");
	}
	}
	*/
}



void IN_CreateStreetStallFunc(LONG lId, void * p, DWORD we)
{
	//	GAMEIN->GetStreetStallDialog()->ShowSellStall();
}

void SSI_TitleEditBoxFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_RETURN)
	{
		// A|￠®E￠®ⓒ­n￠®E?￠®E¡Ec ￠®E￠®IeCN ￠®E?¡§Io￠®E?UA¡§I¨I¨￡￠®E￠®ⓒ­￠®Ec CO¡E￠c¡§￠®I..
		STREETSTALLMGR->EditTitle();
	}
	else if( we == WE_SETFOCUSON )
	{
		GAMEIN->GetStreetStallDialog()->OnTitleEditClick();
	}
}

void SSI_MoneyEditBoxFunc(LONG lId, void * p, DWORD we)
{
	/*
	if(we == WE_RETURN)
	{
	if( GAMEIN->GetStreetStallDialog()->SelectedItemCheck() ) return;

	if( GAMEIN->GetStreetStallDialog()->MoneyEditCheck() )
	{
	GAMEIN->GetStreetStallDialog()->RegistMoney();
	STREETSTALLMGR->ItemStatus( TRUE );
	}
	}
	else if( we == WE_SETFOCUSON )
	{
	GAMEIN->GetStreetStallDialog()->OnMoneyEditClick();
	}
	*/
}

void SSI_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetStreetStallDialog()->OnActionEvnet(lId, p, we);
}

void SSI_BuyBtnFunc(LONG lId, void * p, DWORD we)
{
	CStreetStall* pStallDlg = GAMEIN->GetStreetStallDialog();

	if( pStallDlg->GetCurSelectedItemNum() == -1 ) return;

	if( we == WE_BTNCLICK )
	{

		if( !pStallDlg->SelectedItemCheck() ) return;

		// FOR FIND_SS_BUG
		if( ITEMMGR->IsDupItem( pStallDlg->GetCurSelectedItemIdx() ) ) // ￠®E?￠®Ii￠®¡×uaAI¡§I¨I¨￡￠®¡×￠®I ￠®¡×¡E￠c?¡E￠cie¡§Io¡E￠c¡§￠® ￠®¡×u￠®¡×¡E￠cAIAUAI ¡E￠c¡§￠®¡§I¡E￠c￠®E?i
		{
			if( pStallDlg->GetCurSelectedItemDur() == 1 )
				STREETSTALLMGR->BuyItem();
			else
				GAMEIN->GetStreetStallDialog()->ShowDivideBox();
		}
		else
		{
			STREETSTALLMGR->BuyItem();
		}
	}
}

void SSI_RegistBtnFunc(LONG lId, void * p, DWORD we)
{
	/*
	if( GAMEIN->GetStreetStallDialog()->GetCurSelectedItemNum() == -1 ) return;

	if( we == WE_BTNCLICK )
	{
	if( GAMEIN->GetStreetStallDialog()->SelectedItemCheck() ) return;

	if( GAMEIN->GetStreetStallDialog()->MoneyEditCheck() )
	{
	GAMEIN->GetStreetStallDialog()->RegistMoney();
	STREETSTALLMGR->ItemStatus( TRUE );
	}
	}
	*/
}

void SSI_EditBtnFunc(LONG lId, void * p, DWORD we)
{
	if( GAMEIN->GetStreetStallDialog()->GetCurSelectedItemNum() == -1 ) return;

	CStreetStall* pStall = (CStreetStall*)p;
	if(we == WE_BTNCLICK)
	{

		CExchangeItem* pExItem = pStall->FindItem( pStall->GetCheckDBIdx() );
		if( pExItem == NULL ) return;

		//		if( !GAMEIN->GetStreetStallDialog()->SelectedItemCheck() ) return;
		//락이 걸려있나 안있나만 보자..
		if( pExItem->IsLocked() == TRUE ) return;

		STREETSTALLMGR->ItemStatus( pExItem, FALSE );
	}
}

// LYJ 051017 구입노점상 추가
void SO_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetStallKindSelectDialog()->OnActionEvnet(lId, p, we);
}

void BS_TitleEditBoxFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_RETURN)
	{
		// A|￠®E￠®ⓒ­n￠®E?￠®E¡Ec ￠®E￠®IeCN ￠®E?¡§Io￠®E?UA¡§I¨I¨￡￠®E￠®ⓒ­￠®Ec CO¡E￠c¡§￠®I..
		STREETSTALLMGR->EditTitle();
	}
	else if( we == WE_SETFOCUSON )
	{
		GAMEIN->GetStreetBuyStallDialog()->OnTitleEditClick();
	}
}

void BS_MoneyEditBoxFunc(LONG lId, void * p, DWORD we)
{
}

void BS_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetStreetBuyStallDialog()->OnActionEvnet(lId, p, we);
}

void BS_SellBtnFunc(LONG lId, void * p, DWORD we)
{
	CStreetBuyStall* pStallBuyDlg = GAMEIN->GetStreetBuyStallDialog();

	if( pStallBuyDlg->GetCurSelectedItemNum() == -1 ) return;

	if( we == WE_BTNCLICK )
	{

		if( !pStallBuyDlg->SelectedItemCheck() ) return;

		STREETSTALLMGR->BuyItem();
	}
}

void BS_REGBtnFunc(LONG lId, void * p, DWORD we)
{
}

void BS_SPFunc(LONG lId, void * p, DWORD we)
{
}

void BRS_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetBuyRegDialog()->OnActionEvnet(lId, p, we);
}

void BRS_CloseFunc(LONG lId, void * p, DWORD we)
{
	/*	CBuyItem* pItem = (CBuyItem*)GAMEIN->GetStreetBuyStallDialog()->GetCurSelectedItem();
	POSTYPE pos = GAMEIN->GetStreetBuyStallDialog()->GetCurSelectedItemNum();

	//선택된 곳에 아이템이 있었다면 삭제한다
	if( pos != -1 && pItem )
	GAMEIN->GetStreetBuyStallDialog()->FakeDeleteItem(pos);

	*/	
	MSGBASE msg;
msg.Category = MP_STREETSTALL;
msg.dwObjectID = HEROID;
msg.Protocol = MP_STREETSTALL_UPDATEEND_SYN;

NETWORK->Send( &msg, sizeof(MSGBASE) );

GAMEIN->GetBuyRegDialog()->Close();
}

void BRS_DeleteFunc(LONG lId, void * p, DWORD we)
{
	CBuyItem* pItem = (CBuyItem*)GAMEIN->GetStreetBuyStallDialog()->GetCurSelectedItem();
	POSTYPE pos = GAMEIN->GetStreetBuyStallDialog()->GetCurSelectedItemNum();

	//선택된 곳에 아이템이 있었다면 삭제한다
	if( pos != -1 && pItem )
		GAMEIN->GetStreetBuyStallDialog()->FakeDeleteItem(pos);

	GAMEIN->GetBuyRegDialog()->Close();
}


void BRS_TypeListFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetBuyRegDialog()->UpdateType();
}

void BRS_ItemListFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetBuyRegDialog()->UpdateItem();
}

void BRS_ClassListFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetBuyRegDialog()->UpdateClass();
}

void BRS_REGBtnFunc(LONG lId, void * p, DWORD we)
{
	ITEM_INFO* pItem = GAMEIN->GetBuyRegDialog()->GetSelectItemInfo();

	if(!pItem)
		return;

	BUY_REG_INFO RegInfo;

	if( !GAMEIN->GetBuyRegDialog()->GetBuyRegInfo(RegInfo) )
		return;

	GAMEIN->GetStreetBuyStallDialog()->FakeRegistItem(RegInfo, pItem);
	GAMEIN->GetBuyRegDialog()->Close();
}

void BRS_VolumeEditFunc(LONG lId, void * p, DWORD we)
{

}

void BRS_MoneyEditFunc(LONG lId, void * p, DWORD we)
{

}


// LBS 03.10.20 Npc￠®E￠®Ie¡E￠cicA¡E￠cE
void NSI_DlgFunc(LONG lId, void * p, DWORD we)
{
	//	if( we == WE_CLOSEWINDOW )
	//	{
	//		GAMEIN->GetNpcScriptDialog()->EndDialog();
	//	}
}

void NSI_HyperLinkFunc(LONG lId, void * p, DWORD we)
{
	cListDialogEx* pListDlg = (cListDialogEx*)((cDialog*)p)->GetWindowForID( lId );
	int nIdx = pListDlg->GetCurSelectedRowIdx();
	if( nIdx == -1 ) return;

	cNpcScriptDialog* pDlg = GAMEIN->GetNpcScriptDialog() ;

	if( pDlg )
	{
		if( we == WE_BTNCLICK )
		{
			pDlg->OnActionEvent( lId, p, we ) ;
		}

		BOOL bConversation = FALSE ;

		if( lId == NSI_LISTDLG )
		{
			bConversation = TRUE ;
		}

		pDlg->HyperLinkParser( nIdx, bConversation );
		WINDOWMGR->SetMouseInputProcessed();	
	}
}

void CMI_MoneyOkFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetMoneyDialog()->OkPushed();
}
void CMI_MoneySpinFunc(LONG lId, void * p, DWORD we)
{
	//	int a=0;
}
void CMI_AlertFunc(LONG lId, void * p, DWORD we)
{

}

/*¡E￠cioA￠®E¡EcA¡E￠cE*/
void DE_DlgFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_CLOSEWINDOW)
	{
		CDealDialog* pDlg = GAMEIN->GetDealDialog();

		if( pDlg && pDlg->IsShow() )
		{
			pDlg->SetActive(FALSE);
		}
	}

	if(we == WE_LBTNCLICK)
	{
		GAMEIN->GetDealDialog()->OnSelectedItem();
	}
	else if(/*we == WE_LBTNCLICK || we == WE_RBTNCLICK || we == WE_RBTNDBLCLICK ||*/ we == WE_LBTNDBLCLICK)
	{
		GAMEIN->GetDealDialog()->OnActionEvnet(lId, p, we);
	}
}
void DE_BuyItemFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetDealDialog()->OnBuyPushed();
}
void DE_SellItemFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetDealDialog()->OnSellPushed();
}
// 070503 LYW --- GlobalEventFunc : Add function to cancel sell dialog.
void DE_CancelFunc(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetDealDialog()->SetActive(FALSE) ;
}
void DE_CloseHideBtn(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetDealDialog()->HideDealer();
}

//////////////////////////////////////////////////////////////////////////
//*￠®¡×¡E￠cA￠®¡×¡E￠c￠®¡×uA¡E￠cE*/
void PA_BtnFunc(LONG lId, void * p, DWORD we) 
{
	if(GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
		return;

	switch(lId) 
	{		
	case PA_SECEDEBTN: //A¡E￠ciA¡§I￠®I 
		{
			// partywar
			if( PARTYWAR->IsPartyWar() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(885));
				return;
			}

			if( HEROID != PARTYMGR->GetMasterID() )
				WINDOWMGR->MsgBox( MBI_PARTY_SECEDE, MBT_YESNO, CHATMGR->GetChatMsg(400));	
			else
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1042)) ;
				return ;
			}
		}	
		break;

	case PA_FORCEDSECEDEBTN: //¡E￠c¡§￠®¡E￠c¡§IA|A¡E￠ciA¡§I￠®I
		{
			if( HEROID != PARTYMGR->GetMasterID() )	return;

			// partywar
			if( PARTYWAR->IsPartyWar() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(654));
				return;
			}

			CObject* pTarget = NULL ;
			pTarget = OBJECTMGR->GetSelectedObject();

			if( !pTarget ) 
			{
				/*CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1037)) ;
				return ;*/

					DWORD DeleteID = GAMEIN->GetPartyDialog()->GetClickedMemberID();

				PARTYMGR->BanPartyMemberSyn(DeleteID);
			}
			else
			{
				if( !PARTYMGR->IsPartyMember(pTarget->GetID()) )
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1041)) ;
					return ;
				}

				if( pTarget->GetID() == HEROID )
				{
					//CHATMGR->AddMsg(CTC_SYSMSG, "자기자신을 파티에서 추방할 수 없습니다.") ;
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1268 ) ) ;
					return ;
				}

				PARTYMGR->BanPartyMemberSyn(pTarget->GetID());
			}
		}
		break;

	case PA_TRANSFERBTN: //￠®E￠®ⓒ­￠®Ec￠®E￠®Io¡E￠c¡§uC AI￠®¡×uc
		{
			if( HEROID != PARTYMGR->GetMasterID() )	return;

			// partywar
			if( PARTYWAR->IsPartyWar() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(654));
				return;
			}
			CObject* pTarget = NULL ;
			pTarget = OBJECTMGR->GetSelectedObject();

			if( !pTarget ) 
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1037)) ;
				return ;
			}

			if( !PARTYMGR->IsPartyMember(pTarget->GetID()) )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1041)) ;
				return ;
			}

			if( pTarget->GetID() == HEROID )
			{
				//CHATMGR->AddMsg(CTC_SYSMSG, "이미 파티장 입니다.") ;
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1300 ) );
				return ;
			}

			/*DWORD NewMasterID = GAMEIN->GetPartyDialog()->GetClickedMemberID();
			if(NewMasterID == 0)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(413));
				return;
			}*/
				PARTYMGR->ChangeMasterPartySyn(HEROID, pTarget->GetID());
		}
		break;

	case PA_BREAKUPBTN: //C￠®¡×¡§¡I¡E￠cie
		{
			/*CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(528));
			return;*/

				if( HEROID != PARTYMGR->GetMasterID() )	return;

			// partywar
			if( PARTYWAR->IsPartyWar() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(654));
				return;
			}

			if(PARTYMGR->IsProcessing() == TRUE)
				return;
			WINDOWMGR->MsgBox(MBI_PARTYBREAKUP, MBT_YESNO, CHATMGR->GetChatMsg(651));
		}
		break;

	case PA_ADDPARTYMEMBERBTN: //￠®E￠®ⓒ­a¡§Ioo A¡§I￠®ⓒ­¡E￠c¡§￠®￠®E¡Ec
		{
			if( HEROID != PARTYMGR->GetMasterID() )	return;

			// partywar
			if( PARTYWAR->IsPartyWar() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(654));
				return;
			}

			CObject* pObject = OBJECTMGR->GetSelectedObject();
			if(!pObject)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1037)) ;
				return;
			}
			else 
			{
				if( pObject->GetID() == HEROID )
				{
					//CHATMGR->AddMsg(CTC_SYSMSG, "자기자신은 파티멤버로 초대할 수 없습니다.") ;
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1269 ) );
					return ;
				}

				if( PARTYMGR->IsPartyMember(pObject->GetID()) )
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1038)) ;
					return ;
				}

				if(pObject->GetObjectKind() == eObjectKind_Player)
					PARTYMGR->AddPartyMemberSyn(OBJECTMGR->GetSelectedObject()->GetID());
				else
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(18));
			}
		}
		break;
	case PA_CREATEOK:
		{
			// 070314 LYW --- GlobalEventFunc : Modified CreateOK part of create party dialog.
			/*
			GAMEIN->GetPartyCreateDialog()->SetActive(FALSE);
			cComboBox* pDistirbute = (cComboBox*)WINDOWMGR->GetWindowForIDEx(PA_CREATECOMBOX);
			char buff[16];
			BYTE Opt;
			strcpy(buff,pDistirbute->GetComboText());

			CMD_ST(buff)
				CMD_CS( RESRCMGR->GetMsg(483) )
				Opt = ePartyOpt_Random;
			CMD_CS( RESRCMGR->GetMsg(484) )
				Opt = ePartyOpt_Damage;
			CMD_EN

				PARTYMGR->CreatePartySyn(Opt);
			*/

				GAMEIN->GetPartyCreateDialog()->SetActive(FALSE);
			cComboBox* pDistirbute = (cComboBox*)WINDOWMGR->GetWindowForIDEx(PA_CREATECOMBOX);
			char buff[128];
			BYTE Opt = 0x00 ;

			strcpy(buff,pDistirbute->GetComboText());

			CMD_ST(buff)
				CMD_CS( RESRCMGR->GetMsg(433) )
				Opt = ePartyOpt_Random;
			CMD_CS( RESRCMGR->GetMsg(434) )
				Opt = ePartyOpt_Damage;
			CMD_CS( RESRCMGR->GetMsg(432) )
				Opt = ePartyOpt_Sequence;
			CMD_EN

				PARTYMGR->CreatePartySyn(Opt);

		}
		break;
	case PA_CREATECANCEL:
		{
			GAMEIN->GetPartyCreateDialog()->SetActive(FALSE);
		}
		break;
	case PA_INVITEOK:
		{
			GAMEIN->GetPartyInviteDialog()->SetActive(FALSE);
			PARTYMGR->PartyInviteAccept();
		}
		break;
	case PA_INVITECANCEL:
		{
			PARTYMGR->PartyInviteDeny();
			GAMEIN->GetPartyInviteDialog()->SetActive(FALSE);
		}
		break;
	case PA_WARSUGGESTBTN:
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(528));
			return;

			/*if( HEROID != PARTYMGR->GetMasterID() )	return;

			CObject* pTarget = NULL ;
			pTarget = OBJECTMGR->GetSelectedObject();

			if( !pTarget ) 
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1037)) ;
				return ;
			}

			PARTYWAR->SendPartyWarSuggest();*/
		}
		break;
		/*case PA_OPTIONBTN:
		{
		GAMEIN->GetPartyDialog()->ShowOption();
		}
		break;
		case PA_MEMBERBTN:
		{
		GAMEIN->GetPartyDialog()->ShowMember();
		}
		break;*/

		// 071002 LYW --- GlobalEventFunc : Add process to change party option.

	case PA_OP_RANDOM :
		{
			if( HEROID != PARTYMGR->GetMasterID() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1116)) ;
				return ;
			}

			CPartyBtnDlg* pPartySet = NULL ;
			pPartySet = (CPartyBtnDlg*)WINDOWMGR->GetWindowForID( PA_BTNDLG );

			if( pPartySet )
			{
				cCheckBox* pRandomBox = NULL ;
				pRandomBox = pPartySet->GetRandomCheckBox() ;
				if( !pRandomBox ) return ;

				PARTYMGR->SetPickupItem(e_OPTION_RANDOM) ;
			}
		}
		break ;

	case PA_OP_DAMAGE :
		{
			if( HEROID != PARTYMGR->GetMasterID() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1116)) ;
				return ;
			}

			CPartyBtnDlg* pPartySet = NULL ;
			pPartySet = (CPartyBtnDlg*)WINDOWMGR->GetWindowForID( PA_BTNDLG );

			if( pPartySet )
			{
				cCheckBox* pDamageBox = NULL ;
				pDamageBox = pPartySet->GetDamageCheckBox() ;
				if( !pDamageBox ) return ;

				PARTYMGR->SetPickupItem(e_OPTION_DAMAGE) ;
			}
		}
		break ;

	case PA_OP_SEQUENCE :
		{
			if( HEROID != PARTYMGR->GetMasterID() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1116)) ;
				return ;
			}

			CPartyBtnDlg* pPartySet = NULL ;
			pPartySet = (CPartyBtnDlg*)WINDOWMGR->GetWindowForID( PA_BTNDLG );

			if( pPartySet )
			{
				cCheckBox* pSequenceBox = NULL ;
				pSequenceBox = pPartySet->GetSequenceCheckBox() ;
				if( !pSequenceBox ) return ;

				PARTYMGR->SetPickupItem(e_OPTION_SEQUENCE) ;
			}
		}
		break ;
	// 080318 LUJ, 표시된 파티원의 버프 스킬 아이콘을 토글한다
	case PA_SKILL_ICON_BUTTON:
		{
			CPartyDialog* dialog = GAMEIN->GetPartyDialog();

			if( dialog )
			{
				dialog->OnActionEvent( lId, p, we );
			}

			break;
		}
	}
}
//￠®¡×¡E￠cA￠®¡×¡E￠c￠®¡×u
//////////////////////////////////////////////////////////////////////////


// 080318 LUJ, 파티 멤버 창 이벤트 처리
void PartyMemberFunc( LONG lId, void* p, DWORD we )
{
	CPartyDialog* dialog = GAMEIN->GetPartyDialog();

	if( dialog )
	{
		dialog->OnActionEvent( lId, p, we );
	}
}


// ￠®E￠®IUAaA¡E￠cE ¡E￠c¡§￠®u¡E￠c¡E￠￥A
void QI_QuickDlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetQuickDlg()->OnActionEvnet(lId, p, we);
}

void PYO_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	switch(lId) 
	{
		// 070326 LYW --- GlobalEventFunc : add close button.
	case PYO_STORAGE_CLOSE :
		{
			GAMEIN->GetStorageDialog()->ShowStorageDlg(FALSE) ;
		}
		break ;
	case PYO_STORAGELISTBTN:
		{
			GAMEIN->GetStorageDialog()->SetStorageListInfo();
			GAMEIN->GetStorageDialog()->ShowStorageMode(eStorageMode_StorageListInfo);
			GAMEIN->GetStorageDialog()->AddStorageMode(eStorageMode_StorageListInfo);

			// 070326 LYW --- GlobalEventFunc : Modified active function.
			//GAMEIN->GetStorageDialog()->SetActive(TRUE);
			GAMEIN->GetStorageDialog()->ShowStorageDlg(TRUE) ;

			// 길드창을 끄자
			{
				cDialog* dialog = WINDOWMGR->GetWindowForID( GD_GUILDDLG );
				ASSERT( dialog );

				dialog->SetActive( FALSE );
			}
		}
		break;
	case PYO_BUYSTORAGEBTN:
		{
			if(HERO->GetStorageNum() >= MAX_BASESTORAGELIST_NUM)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(535) );
				return;
			}
			// 070324 LYW --- GlobalEventFunc : Modified message of messagebox.
			WINDOWMGR->MsgBox( MBI_STORAGEBUY, MBT_YESNO, CHATMGR->GetChatMsg(531), 
				HERO->GetStorageNum()+1, AddComma( GAMEIN->GetStorageDialog()->GetPrice(HERO->GetStorageNum() ) ) );
		}
		break;

	case PYO_STORAGE1BTN: case PYO_STORAGE2BTN: case PYO_STORAGE3BTN:
	case PYO_STORAGE4BTN: case PYO_STORAGE5BTN:
		{
			int num = lId-PYO_STORAGE1BTN;
			CStorageDialog* pDlg = (CStorageDialog*)GAMEIN->GetStorageDialog();
			if( !pDlg )
			{
				ASSERT(0);
				return;
			}

			cPushupButton* pstoragebtn[TAB_STORAGE_NUM];

			if( HERO->GetStorageNum() == 0 )
			{
				for(int i=0; i<TAB_STORAGE_NUM; ++i)
				{
					pstoragebtn[i] = (cPushupButton*)pDlg->GetWindowForID(PYO_STORAGE1BTN+i);
					pstoragebtn[i]->SetPush(FALSE);
				}

				return;
			}
			for(int i=0; i<TAB_STORAGE_NUM; ++i)
			{
				pstoragebtn[i] = (cPushupButton*)pDlg->GetWindowForID(PYO_STORAGE1BTN+i);
				if(i==num)
				{
					pstoragebtn[i]->SetPush(TRUE);
				}
				else
				{
					pstoragebtn[i]->SetPush(FALSE);
				}
			}
			// ¡§Ioo￠®¡×¡E￠c¡E￠c¡§￠®￠®E?￠®E¡Ec ￠®Iiu￠®EOo ￠®¡×￠®ⓒ­￠®E￠®ⓒ­￠®E?￠®¡×IAo￠®E￠®IA ￠®E￠®ⓒ­¡§I￠®I￠®¡×oA￠®E￠®ⓒ­￠®E￠®ⓒ­ ￠®¡×￠®ⓒ­?E¡E￠c¡§ICN￠®E￠®IU.
			GAMEIN->GetStorageDialog()->ShowStorageMode(eStorageMode_StorageWare1+num);
			GAMEIN->GetStorageDialog()->AddStorageMode(eStorageMode_StorageWare1+num);
		}
		break;

	case PYO_PUTINMONEYBTN:
		{
			STORAGEMGR->PutInOutMoney(1);
		}
		break;
	case PYO_PUTOUTMONEYBTN:
		{
			STORAGEMGR->PutInOutMoney(0);
		}
		break;
	case PYO_NOBUYBTN: case PYO_NOBUYSTORAGEBTN:
		{
			// 070326 LYW --- GlobalEventFunc : Modified active function.
			/*
			GAMEIN->GetStorageDialog()->SetActive(FALSE);
			GAMEIN->GetInventoryDialog()->SetActive(FALSE);
			*/
				GAMEIN->GetStorageDialog()->ShowStorageDlg(FALSE) ;
		}
		break;
	}
}


void Friend_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	switch(lId) 
	{
	case FRI_PAGE1BTN: case FRI_PAGE2BTN: case FRI_PAGE3BTN: 
		{
			GAMEIN->GetFriendDialog()->SetFriendList(lId - FRI_PAGE1BTN + 1);
		}
		break;
	case FRI_ADDFRIENDBTN:
		{
			if(GAMEIN->GetMiniFriendDialog()->IsActive())
			{
				GAMEIN->GetMiniFriendDialog()->SetActive(FALSE);
			}
			else
			{
				GAMEIN->GetMiniFriendDialog()->SetActive(TRUE);
				char Name[MAX_NAME_LENGTH+1] = {0};
				CObject* pTarget = OBJECTMGR->GetSelectedObject();
				if(pTarget && (pTarget->GetObjectKind() == eObjectKind_Player))
				{
					//strcpy(Name, OBJECTMGR->GetSelectedObject()->GetObjectName());
					SafeStrCpy(Name, OBJECTMGR->GetSelectedObject()->GetObjectName(), MAX_NAME_LENGTH+1);
					GAMEIN->GetMiniFriendDialog()->SetName(Name);
				}
			}
		}
		break;
	case FRI_DELFRIENDBTN:
		{
			GAMEIN->GetFriendDialog()->CheckedDelFriend();
			//￠®E￠®ⓒ­￠®Ec￠®¡×i¨Io￠®Ei¡§u￠®¡×¡E￠c￠®Ec AA￠®¡×¡E￠c￠®Ec¡E￠c¡E￠￥N￠®E?￠®E¡Ec￠®¡×u¡E￠c¡§I? 
			//	char Name[MAX_NAME_LENGTH+1] = GAMEIN->GetFriendDialog()->GetSelectedFriendID();
			//	FRIENDMGR->AddDelFriendSyn(, MP_FRIEND_DEL_SYN);
		}
		break;
	case FRI_ADDOKBTN:
		{		
			cEditBox * pName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(FRI_NAMEEDIT);
			char code[MAX_NAME_LENGTH+1] = {0,};			
			sscanf(pName->GetEditText(),"%s",code);
			strupr(code);
			if(strcmp("",code) == 0)
				return;
			char heroname[MAX_NAME_LENGTH+1] = {0,};
			sscanf(HERO->GetObjectName(), "%s", heroname);
			strupr(heroname);
			if(strcmp(heroname, code) == 0)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(676) );
				return;
			}				
			FRIENDMGR->AddDelFriendSyn(pName->GetEditText(), MP_FRIEND_ADD_SYN);
			GAMEIN->GetMiniFriendDialog()->SetActive(FALSE);
		}
		break;
	case FRI_ADDCANCELBTN:
		{
			GAMEIN->GetMiniFriendDialog()->SetActive(FALSE);
		}
		break;
	case FRI_SENDNOTEBTN:
		{
			CMiniNoteDialog* dialog = GAMEIN->GetMiniNoteDialog();
			ASSERT( dialog );

			if( ! strlen( GAMEIN->GetFriendDialog()->GetChkedName() ) )
			{
				dialog->SetMiniNote( "", "", 0 );
			}
			else
			{
				dialog->SetMiniNote(GAMEIN->GetFriendDialog()->GetChkedName(), "", 0);
			}

			dialog->ShowMiniNoteMode(eMinNoteMode_Write);
			dialog->SetActive( TRUE );
		}
		break;
	case FRI_SENDWHISPERBTN:
		{
			char temp[MAX_NAME_LENGTH+5];
			if(strcmp(GAMEIN->GetFriendDialog()->GetChkedName(), "") == 0)
				return;
			sprintf(temp,"/%s ",GAMEIN->GetFriendDialog()->GetChkedName());
			//061212 LYW --- Change chatting dialog.
			//cEditBox* pChatBox = GAMEIN->GetChattingDlg()->GetInputBox();
			//070102 LYW --- Delete this code.
			
			cEditBox* pChatBox = GAMEIN->GetChattingDlg()->GetInputBox();
			pChatBox->SetFocusEdit(TRUE);
			pChatBox->SetEditText(temp);
			

		}
		break;
	}
}

void Note_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	switch(lId) 
	{
	case NOTE_WRITENOTEBTN:
		{
			if(GAMEIN->GetMiniNoteDialog()->IsActive())
			{
				if(GAMEIN->GetMiniNoteDialog()->GetCurMode() == eMinNoteMode_Read)
					GAMEIN->GetMiniNoteDialog()->ShowMiniNoteMode(eMinNoteMode_Write);
				else
					GAMEIN->GetMiniNoteDialog()->SetActive(FALSE);
			}
			else
			{
				GAMEIN->GetMiniNoteDialog()->SetActive(TRUE);
				GAMEIN->GetMiniNoteDialog()->ShowMiniNoteMode(eMinNoteMode_Write);			
			}
		}
		break;
	case NOTE_DELNOTEBTN:
		{
			GAMEIN->GetNoteDialog()->CheckedNoteID();
		}
		break;

	case NOTE_PAGE1BTN:
	case NOTE_PAGE2BTN:
	case NOTE_PAGE3BTN:
	case NOTE_PAGE4BTN:
	case NOTE_PAGE5BTN:
		{
			NOTEMGR->NoteListSyn( lId - NOTE_PAGE1BTN +1, GAMEIN->GetNoteDialog()->GetMode() );
		}
		break;
		//////////////////////////////////////////////////////////////////////////
		//////MINI NOTE
	case NOTE_MSENDOKBTN:
		{
			cEditBox * pReceiver = (cEditBox *)WINDOWMGR->GetWindowForIDEx(NOTE_MRECEIVEREDIT);
			char pname[MAX_NAME_LENGTH+1] = {0};
			SafeStrCpy(pname, pReceiver->GetEditText(), MAX_NAME_LENGTH+1);
			if(strlen(pname) == 0)
				return;

			cTextArea * pNote = (cTextArea *)WINDOWMGR->GetWindowForIDEx(NOTE_MWNOTETEXTREA);
			char FullText[200];
			pNote->GetScriptText(FullText);
			int len = strlen(FullText);

			if( len == 0 )
			{
				return;
			}

			if(len >= MAX_NOTE_LENGTH)
			{
				len = MAX_NOTE_LENGTH -1;
			}
			char Note[MAX_NOTE_LENGTH];
			strncpy(Note, FullText, len);
			Note[len] = 0;
			NOTEMGR->WriteNote(Note,  pname);
			GAMEIN->GetMiniNoteDialog()->SetActive(FALSE);			
		}
		break;
	case NOTE_MSENDCANCELBTN:
		{
			GAMEIN->GetMiniNoteDialog()->SetActive(FALSE);
		}
		break;
	case NOTE_MREPLYBTN:
		{
			char ToName[MAX_NAME_LENGTH+1];
			strcpy(ToName, GAMEIN->GetMiniNoteDialog()->GetSenderName());
			if(strlen(ToName) == 0 )
				return;

			GAMEIN->GetMiniNoteDialog()->SetMiniNote(ToName, "", 0);
			GAMEIN->GetMiniNoteDialog()->ShowMiniNoteMode(eMinNoteMode_Write);

		}
		break;
	case NOTE_MDELETEBTN:
		{
			NOTEMGR->DelNoteSyn(GAMEIN->GetMiniNoteDialog()->GetNoteID(),TRUE);
		}
		break;
	case NOTE_TABBTN1:
		{
			GAMEIN->GetNoteDialog()->SetMode( eMode_NormalNote );
			NOTEMGR->NoteListSyn( 1, eMode_NormalNote );
		}
		break;
	case NOTE_TABBTN2:
		{
			GAMEIN->GetNoteDialog()->SetMode( eMode_PresentNote );
			NOTEMGR->NoteListSyn( 1, eMode_PresentNote );
		}
		break;
	case NOTE_NOTELISTCHK11:
		{
			GAMEIN->GetNoteDialog()->SetChkAll();
		}
		break;
	}
}

// 080313 NYJ --- 전체외치기 추가
void Shout_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	switch(lId)
	{
	case SHOUT_BTN_OK:
		{
			GAMEIN->GetShoutDlg()->SendShoutMsgSyn();
			GAMEIN->GetShoutDlg()->SetActive(FALSE);
		}
		break;

	case SHOUT_BTN_CANCEL:
		{
			GAMEIN->GetShoutDlg()->SetActive(FALSE);
		}
		break;
	}
}

//080410 NYJ --- 낚시시스템
void Fishing_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	switch(lId)
	{
	case FISHING_BTN_CLOSE:
		{
			GAMEIN->GetFishingDlg()->SetActive(FALSE);
		}
		break;
	case FISHING_BTN_START:
		{
			// 이전버튼 상태는 현재상태의 반대.
			BOOL bLastPushed = !GAMEIN->GetFishingDlg()->IsPushedStartBtn();
			
			if(bLastPushed)
				GAMEIN->GetFishingDlg()->Fishing_Cancel();
			else
				GAMEIN->GetFishingDlg()->Fishing_Start();
				

			GAMEIN->GetFishingDlg()->SetLockStartBtn(TRUE);
		}
		break;

	case FISHING_BTN_CLOSEPOINT:
	case FISHING_BTN_CLOSEPOINTX:
		{
			// 포인트 창닫기 X버튼
			GAMEIN->GetFishingPointDlg()->SetActive(FALSE);
		}
		break;

	case FISHING_BTN_CHANGEPOINT:
		{
			// 포인트변환
			GAMEIN->GetFishingPointDlg()->SendFishPoint();
		}
		break;

	case FISHING_BTN_CLEARSLOT:
		{
			// 슬롯초기화
			GAMEIN->GetFishingPointDlg()->ClearFishIcon();
		}
		break;
	}
}

void CR_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	if( EXITMGR->IsExitWaiting() )	//종료중엔 부활 안됨
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(643) );
		GAMEIN->GetReviveDialog()->SetActive( FALSE );
		return;
	}

	LEVELTYPE heroLevel = HERO->GetLevel() ;

	if( heroLevel >= 10 )
	{
		switch(lId) 
		{
		case CR_PRESENTSPOT:
			{
				WINDOWMGR->MsgBox(MBI_REVIVE_PRESENTSPOT, MBT_YESNO, CHATMGR->GetChatMsg(850)) ;
			}
			break;

		case CR_TOWNSPOT:
			{
				WINDOWMGR->MsgBox(MBI_REVIVE_TOWNSOPT, MBT_YESNO, CHATMGR->GetChatMsg(851)) ;
			}
			break;
		}
	}
	else
	{
		switch(lId)
		{
		case CR_PRESENTSPOT :
			{
				MSGBASE msg;
				msg.Category = MP_CHAR_REVIVE;
				msg.Protocol = MP_CHAR_REVIVE_PRESENTSPOT_SYN;
				msg.dwObjectID = HEROID;
				NETWORK->Send(&msg, sizeof(msg));
			}
			break ;

		case CR_TOWNSPOT :
			{
				MSGBASE msg;
				msg.Category = MP_CHAR_REVIVE;
				msg.Protocol = MP_CHAR_REVIVE_VILLAGESPOT_SYN;
				msg.dwObjectID = HEROID;
				NETWORK->Send(&msg, sizeof(msg));
			}
			break ;
		}
	}

	GAMEIN->GetReviveDialog()->SetDisable(TRUE);
}

//KES
/*Macro Dialog Interface*/
//void MAC_ModeDlgFunc(LONG lId, void * p, DWORD we)
//{
//	GAMEIN->GetMacroModeDialog()->OnActionEvent(lId, p, we);
//}

/*Chat Dialog Interface*/
// 061212 LYW --- Change chatting dialog.
/*
void CTI_DlgFunc(LONG lId, void * p, DWORD we)
{
	if( lId == CHA_CREATEOKBTN )
		GAMEIN->GetShoutDlg()->SendShoutMsgSyn();
	else
		// 061212 LYW --- Change chatting dialog.
		//GAMEIN->GetChatDialog()->OnActionEvent(lId, p, we);
		GAMEIN->GetChattingDlg()->OnActionEvent(lId, p, we);
}
*/

void COI_DlgFunc(LONG lId, void * p, DWORD we)
{
	//	GAMEIN->GetChatOptionDialog()->OnActionEvent(lId, p, we);
}

void ITEM_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetMallNoticeDialog()->OnActionEvent(lId, p, we);
}

/*Option Dialog Interface*/
void OTI_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetOptionDialog()->OnActionEvent(lId, p, we);
}

void EXT_DlgFunc(LONG lId, void * p, DWORD we)
{
	//A¡E￠cAA¡E￠cE¡E￠c¡§u￠®E￠®ⓒ­Co
	if( we == WE_BTNCLICK )
	{
		if( lId == EXT_BTN_EXITGAME )
		{
			// 080410 LYW --- GlobalEventFunc : Modified.
			//GAMEIN->GetExitDialog()->SetActive( FALSE );
			//// 070122 Modified this line.
			////WINDOWMGR->MsgBox( MBI_EXIT, MBT_YESNO, CHATMGR->GetChatMsg( 280 ) );
			//WINDOWMGR->MsgBox( MBI_EXIT, MBT_YESNO, RESRCMGR->GetMsg( 263 ) );

			/*if(CHATROOMMGR->Get_ChatRoomState() == e_State_In_ChatRoom)
			{
				WINDOWMGR->MsgBox( NULL, MBT_OK, CHATMGR->GetChatMsg(1493) );
				return ;
			}*/

			GAMEIN->GetExitDialog()->SetActive( FALSE ) ;
			WINDOWMGR->MsgBox( MBI_EXIT, MBT_YESNO, RESRCMGR->GetMsg( 263 ) ) ;
		}		
		else if( lId == EXT_BTN_SELECTCHAR )
		{
			//// 080410 LYW --- GlobalEventFunc : 채팅 방 사용중인지 검사.
			//if(CHATROOMMGR->Get_ChatRoomState() == e_State_In_ChatRoom)
			//{
			//	WINDOWMGR->MsgBox( NULL, MBT_OK, CHATMGR->GetChatMsg(1493) );
			//	return ;
			//}

			GAMEIN->GetExitDialog()->SetActive( FALSE );
			int nErrorCode;
			if( ( nErrorCode = EXITMGR->CanExit() ) == eEXITCODE_OK )
			{
				EXITMGR->SendExitMsg( eEK_CHARSEL );
			}
			else
			{
				switch( nErrorCode )
				{
				case eEXITCODE_PKMODE:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1261) );
					break;
				case eEXITCODE_LOOTING:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1262) );
					break;
				default:
					// 070622 LYW --- GlobalEventFunc : 메시지 번호 수정.
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(219) );
					break;
				}
			}
		}
		else if( lId == EXT_BTN_OPTION )
		{
			COptionDialog* pDlg = GAMEIN->GetOptionDialog() ;					

			if( pDlg )															
			{
				pDlg->SetActive( TRUE ) ;										
			}
		}
		else if( lId == EXT_BTN_EMERGENCY )
		{
			//// 080410 LYW --- GlobalEventFunc : 채팅 방 사용중인지 검사.
			//if(CHATROOMMGR->Get_ChatRoomState() == e_State_In_ChatRoom)
			//{
			//	WINDOWMGR->MsgBox( NULL, MBT_OK, CHATMGR->GetChatMsg(1493) );
			//	return ;
			//}

			MSGBASE msg;
			msg.Category	= MP_CHAR;
			msg.Protocol	= MP_CHAR_EMERGENCY_SYN;
			msg.dwObjectID	= HEROID;	
			NETWORK->Send( &msg, sizeof(msg) );
			EXITMGR->SendExitMsg( eEK_CHARSEL );
		}

	}
}

// desc_hseos_소스분석용테스트_01
// S 소스분석용테스트 추가 added by hseos 2007.04.09
void MONSTERMETER_DlgFunc(LONG lId, void * p, DWORD we)
{
}
// E 소스분석용테스트 추가 added by hseos 2007.04.09

void XCI_DlgFunc( LONG lId, void* p, DWORD we )
{
	GAMEIN->GetExchangeDialog()->OnActionEvent(lId, p, we);
}

void CSS_DlgFunc( LONG lId, void* p, DWORD we )
{
	GAMEIN->GetCharStateDialog()->OnActionEvent(lId, p, we);
}

void MSI_MenuSlotDlgFunc( LONG lId, void* p, DWORD we )
{
	GAMEIN->GetMenuSlotDialog()->OnActionEvent(lId, p, we);
}
/*PKLOOTING*/
void PLI_DlgFunc( LONG lId, void* p, DWORD we )
{
	if( p )
	{
		if( ((cDialog*)p)->GetType() == WT_PKLOOTINGDLG )
		{
			((CPKLootingDialog*)p)->OnActionEvent( lId, p, we );
		}
		else
		{
			int a = 0;
		}
	}
	//GAMEIN->GetPKLootingDialog()->OnActionEvent(lId, p, we);
	//pk manager￠®E?￠®E¡Ec￠®¡×u¡E￠c¡§I A¡§I¨I¨￡￠®E￠®ⓒ­￠®EcC￠®¡×¡§¡IAO￠®¡×ui￠®¡×u¡§I￠®ⓒ­ CO¡E￠c¡§￠®I ¡E￠c¡§￠®¡E￠c¡§￠®￠®E￠®IU.... ￠®¡×ui￠®E￠®IA ¡E￠c¡§￠®IA¡E￠ci E¡E￠cIAaC￠®¡×¡§¡IAUAo￠®E￠®ⓒ­|.
	//PKMGR->
}

void CNA_BtnOkFunc( LONG lId, void* p, DWORD we )
{
	if( p )
	{
		((cDialog*)p)->SetActive( FALSE );
	}
	TITLE->SetServerList();
}

void DIS_DlgFunc( LONG lId, void* p, DWORD we )
{
	// send event to CDissloveDlg
	cDialog* dialog = WINDOWMGR->GetWindowForID( DIS_DISSOLUTIONDLG );
	ASSERT( dialog );

	dialog->OnActionEvent( lId, p, we );
}

void GAMEIN_ChatFunc(LONG lId, void * p, DWORD we)
{
	//	if( we == eIMEE_INSERTTEXT )
	//	{
	//		char* str = ((cEditBox*)p)->GetEditText();

	//		if( strlen( str ) == 1 )
	//		if( *str == '/' )
	//		{
	//			CHATMGR->SetAutoWhisper();
	//		}
	//	}
	if( we == eIMEE_SETFOCUS )
	{
		// 061212 LYW --- Delete this code for a moment.
		//if( CHATMGR->GetOption()->bOption[CTO_AUTOHIDECHAT] )
		//GAMEIN->GetChatDialog()->HideChatDialog( FALSE );
	}
	else if( we == eIMEE_KILLFOCUS )
	{
		// 061212 LYW --- Delete this code for a moment.
		//if( CHATMGR->GetOption()->bOption[CTO_AUTOHIDECHAT] )
		//GAMEIN->GetChatDialog()->HideChatDialog( TRUE );
	}
}
/*
void MAINTITLE_IDFunc(LONG lId, void * p, DWORD we)
{
	//lId -- editAC AO￠®¡×uO	//p -- editAC ￠®¡×¡E￠c¡E￠cAAIAI
	cEditBox* pEdit = (cEditBox*)p;

	if( we == eIMEE_ENTER )
	{
		if( *pEdit->GetEditText() != 0 )
		{
			WINDOWMGR->SetNextEditFocus();
		}
	}
}

void MAINTITLE_PWFunc(LONG lId, void * p, DWORD we)
{
	cEditBox* pEdit = (cEditBox*)p;

	if( we == eIMEE_ENTER )
	{
		if( *pEdit->GetEditText() != 0 )
		{
			cDialog* pDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );

			MT_LogInOkBtnFunc( MT_OKBTN, pDlg, WE_BTNCLICK );
		}
	}
}
*/
/////////////
//---MurimNet
void MNPRI_DlgFunc(LONG lId, void * p, DWORD we)
{
	CMNStgPlayRoom::GetInterface()->OnActionEvent( lId, p, we );
}

void MNPRI_ChatFunc( LONG lId, void * p, DWORD we )
{
	cEditBox* pEdit = (cEditBox*)p;

	if( strlen( pEdit->GetEditText() ) == 0 ) return;

	TESTMSGID msg;
	msg.Category	= MP_MURIMNET;
	msg.Protocol	= MP_MURIMNET_CHAT_ALL;
	msg.dwObjectID	= MURIMNET->GetMNPlayerManager()->GetMNHeroID();
	msg.dwSenderID	= msg.dwObjectID;
	strcpy( msg.Msg, pEdit->GetEditText() );

	//	NETWORK->Send( &msg, sizeof(msg) );
	NETWORK->Send( &msg, msg.GetMsgLength() ); //CHATMSG 040324

	pEdit->SetEditText("");
}

void MNCNL_DlgFunc(LONG lId, void * p, DWORD we)
{
	CMNStgChannel::GetInterface()->OnActionEvent( lId, p, we );
}


void MNCNL_ChatFunc(LONG lId, void * p, DWORD we)
{
	cEditBox* pEdit = (cEditBox*)p;

	if( strlen( pEdit->GetEditText() ) == 0 ) return;

	TESTMSGID msg;
	msg.Category	= MP_MURIMNET;
	msg.Protocol	= MP_MURIMNET_CHAT_ALL;
	msg.dwObjectID	= MURIMNET->GetMNPlayerManager()->GetMNHeroID();
	msg.dwSenderID	= msg.dwObjectID;
	strcpy( msg.Msg, pEdit->GetEditText() );

	//NETWORK->Send( &msg, sizeof(msg) );
	NETWORK->Send( &msg, msg.GetMsgLength() );	//CHATMSG 040324

	pEdit->SetEditText("");
}


void DIVIDEBOX_Func(LONG lId, void * p, DWORD we)
{
	if( we == WE_RETURN )
	{
		((cDivideBox*)p)->ExcuteDBFunc( WE_RETURN );		
	}
}

// desc_hseos_소스분석용테스트_01
// S 소스분석용테스트 추가 added by hseos 2007.04.09
// 공통 메세지 박스 처리
// E 소스분석용테스트 추가 added by hseos 2007.04.09

//KES MESSAGEBOX
//￠®E?￠®¡×I¡E￠c¡§ua￠®¡×u¡E￠c¡§I ￠®¡×u￠®¡×¡E￠cAI￠®Ii¡§I￠®I￠®E￠®ⓒ­￠®E￠®ⓒ­ ¡E￠c¡§u￠®E￠®ⓒ­￠®¡×￠®ⓒ­￠®¡×￠®EC￠®¡×¡§¡IAO￠®E￠®ⓒ­e.....
void MessageBox_Func( LONG lId, void * p, DWORD we )
{
	switch( lId )
	{
	case MBI_EXIT:
		{
			if( we == MBI_YES )
			{
				//				MSGBASE data;		//agent¡E￠c¡§￠®￠®E¡Ec ￠®¡×uE￠®¡×u￠®¡×¡E￠c￠®¡×u¡E￠c¡§I ￠®¡×￠®ⓒ­￠®E￠®ⓒ­¡§I¨I¨￡¡E￠ciAU
				//				data.Category	= MP_USERCONN;
				//				data.Protocol	= MP_USERCONN_DISCONNECTED;
				//				data.dwObjectID	= HEROID;
				//				NETWORK->Send(&data,sizeof(data));
				//￠®¡×u¡E￠c¡§I¡§Ioo¡E￠c¡E￠￥I ￠®E￠®ⓒ­￠®¡×￠®¨I￠®¡×oAAo¡E￠c¡§￠®￠®E¡Ec A¡§I￠®ⓒ­ ￠®Ii￠®IiA¡§I¡§¡ICI¡E￠c¡§￠®i￠®¡×u¡E￠c¡§I ¡§I¨I¨￡￠®E¡Ec¡§I¨I¨￡￠®¡×￠®I￠®E￠®IA ¡E￠c¡§￠®CAo..?

				if( MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN )
				{
					//					MAINGAME->SetGameState( eGAMESTATE_END );
					PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
					break;
				}

				int nErrorCode;
				if( ( nErrorCode = EXITMGR->CanExit() ) == eEXITCODE_OK )
				{
					EXITMGR->SendExitMsg( eEK_GAMEEXIT );
				}
				else
				{
					switch( nErrorCode )
					{
					case eEXITCODE_PKMODE:
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1261) );
						break;
					case eEXITCODE_LOOTING:
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1262) );
						break;
					default:
						// 070622 LYW --- GlobalEventFunc : 메시지 번호 수정.
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(219) );
						break;
					}
				}
			}
		}
		break;

	// 080121 LYW --- GlobalEventFunc : 캐릭터 선택 화면으로 이동확인 메시지 처리.
	case MBI_MOVE_TO_CHARSELECT :
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetExitDialog()->SetActive( FALSE );
				int nErrorCode;
				if( ( nErrorCode = EXITMGR->CanExit() ) == eEXITCODE_OK )
				{
					EXITMGR->SendExitMsg( eEK_CHARSEL );
				}
				else
				{
					switch( nErrorCode )
					{
					case eEXITCODE_PKMODE:
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1261) );
						break;
					case eEXITCODE_LOOTING:
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1262) );
						break;
					default:
						// 070622 LYW --- GlobalEventFunc : 메시지 번호 수정.
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(219) );
						break;
					}
				}
			}
		}
		break ;

	case MBI_PASSERROR:
		{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
			PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
			cEditBox* pEditId = (cEditBox*)WINDOWMGR->GetWindowForIDEx( MT_IDEDITBOX );
			cEditBox* pEditPwd = (cEditBox*)WINDOWMGR->GetWindowForIDEx( MT_PWDEDITBOX );
			if( pEditId )
				pEditId->SetFocusEdit( FALSE );
			if( pEditPwd )
			{
				pEditPwd->SetEditText( "" );
				pEditPwd->SetFocusEdit( TRUE );
			}
			cDialog* pDlg1 = (cDialog*)WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			pDlg1->SetDisable( FALSE );

			cDialog* pDlg2 = (cDialog*)WINDOWMGR->GetWindowForID( MT_DYNAMICDLG );

			if( pDlg2 )
				if( pDlg2->IsActive() )				
				{
					pDlg1->SetActive( TRUE );
					pDlg2->SetDisable( FALSE );
					pDlg2->SetActive( FALSE	);
					pEditPwd = (cEditBox*)WINDOWMGR->GetWindowForIDEx( MT_DYNAMICPWDEDITBOX );
					pEditPwd->SetEditText( "" );
					//				pEditPwd->SetFocusEdit( TRUE );
				}
		}
		break;
	case MBI_OTHERUSER_CONNECTTRY:
		{

		}
		break;

	case MBI_DISCONNECTED_OVERLAPPEDLOGIN:
		{
			//MAINGAME->SetGameState(eGAMESTATE_END);
			//PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
			MAINGAME->SetGameState(eGAMESTATE_TITLE);
			TITLE->ShowServerList();
		}
		break;
	case MBI_PARTYBREAKUP:
		{
			if(we == MBI_YES)
			{
				PARTYMGR->BreakUpSyn();
			}
		}
		break;
	case MBI_STORAGEBUY:
		{
			if(we == MBI_YES)
			{
				STORAGEMGR->BuyStorageSyn();
			}
		}
		break;
		// 070326 LYW --- GlobalEventFunc : Add case part for messagebox of storage part.
		/*
	case MBI_BUY_COMPLETE :
	case MBI_NOTENOUGH_MONEY :
	case MBI_NOTJOIN_GUILD :
	case MBI_BUY_ALL :
		{
			// 070326 LYW --- GlobalEventFunc : Modified active function.
			//GAMEIN->GetStorageDialog()->SetActive(FALSE);
			GAMEIN->GetStorageDialog()->ShowStorageDlg(FALSE) ;

			CNpc* pNpc = NPCSCRIPTMGR->GetLastNpc() ;
			NPCSCRIPTMGR->StartNpcScript( pNpc ) ;
		}
		break ;
		*/
	case MBI_CANT_CHARGE :
		{
			CNpc* pNpc = NPCSCRIPTMGR->GetLastNpc() ;
			NPCSCRIPTMGR->StartNpcScript( pNpc ) ;
		}
		break ;
	case MBI_SKILLDELETE:
		{
			if( we == MBI_YES )
			{
				//				GAMEIN->GetSkillDialog()->SendDeleteSkill();
				AUDIOMGR->Play(67, HERO);
			}
			else if( we == MBI_NO )
			{
				//				GAMEIN->GetSkillDialog()->SetDisable( FALSE );
			}
		}
		break;
		// LBS A¡§I￠®ⓒ­¡E￠c¡§￠®￠®E¡Ec 03.09.17 (¡§I¨I¨￡eA￠®E¡Ec¡E￠cio ¡E￠c¡§￠®u¡E￠c¡E￠￥A)
	case MBI_BUYREG:
		{

		}
		break;
	case MBI_MAKESTREETSTALL:
		{
			if(we == MBI_YES)
			{
				STREETSTALLMGR->OpenStreetStall();
			}
			else if( we == MBI_NO )
			{
				STREETSTALLMGR->SetOpenMsgBox(TRUE);
			}
		}
		break;

	case MBI_DELETECHAR:
		{
			if( we == MBI_YES )
			{
				// 061218 LYW --- Adjust this code for ask once more.
				//CHARSELECT->DeleteCharacter();
				CHARSELECT->GetCertificateDlg()->MakeRandomNumber() ;
				CHARSELECT->GetCertificateDlg()->SetActive(TRUE) ;
			}
			else
			{
				CHARSELECT->SetDisablePick( FALSE );
			}
		}
		break;

		// 061219 LYW --- Add process when pushed cancel button on msgbox.
	case MBI_CANCEL :
		{
			CHARSELECT->SetDisablePick( FALSE );
		}
		break ;

	case MBI_EXCHANGEAPPLY:
		{
			if(we == MBI_YES)
			{
				EXCHANGEMGR->AcceptExchange( TRUE );
			}
			else if( we == MBI_NO )
			{
				EXCHANGEMGR->AcceptExchange( FALSE );
			}
		}
		break;

	case MBI_EXCHANGEWAITING:
		{
			if(we == MBI_CANCEL)
			{
				EXCHANGEMGR->CancelApply();				
			}
		}
		break;

	case MBI_SELECTLOGINPOINT:
		{
			if(we == MBI_YES)
			{
				cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID( MBI_SELECTLOGINPOINT );				
				CNpc* pNpc = (CNpc*)OBJECTMGR->GetObject( pMsgBox->GetParam() );
				if( pNpc )
				{
					g_UserInput.GetHeroMove()->SelectLoginPoint( pNpc->GetNpcUniqueIdx() );
				}
				else
				{
					//error ¡E￠c¡E￠￥I¡E￠c¡§u¡E￠c¡E?AI￠®¡×¡E￠c¡E￠cAAI￠®¡×¡E￠c￠®Ec￠®E￠®ⓒ­| AuAaCO ￠®¡×uo ￠®¡×u¡§I¡§¡I￠®¡×oA￠®E￠®II￠®E￠®IU.
				}

				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			}
		}
		break;

	case MBI_LOGINEDITCHECK:
		{

			if( we == MBI_OK )
			{
				cEditBox * pIdEdit =(cEditBox *) WINDOWMGR->GetWindowForIDEx(MT_IDEDITBOX); // pjs ID￠®E?￠®E¡Ec ￠®¡×¡E￠c¡E￠cAA￠®E?￠®¡×o￠®¡×￠®ⓒ­ [5/20/2003]
				cEditBox * pPwEdit =(cEditBox *) WINDOWMGR->GetWindowForIDEx(MT_PWDEDITBOX); // pjs ID￠®E?￠®E¡Ec ￠®¡×¡E￠c¡E￠cAA￠®E?￠®¡×o￠®¡×￠®ⓒ­ [5/20/2003]

				if( *pIdEdit->GetEditText() == 0 )
					pIdEdit->SetFocusEdit(TRUE);
				else
					pPwEdit->SetFocusEdit(TRUE);
			}
			cDialog* pDlg= (cDialog*)WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			pDlg->SetDisable( FALSE );
			/*
			#ifdef TAIWAN_LOCAL
			cCheckBox* pCheckBox = (cCheckBox*)pDlg->GetWindowForID( MT_BTN_DYNAMIC );
			if( !pCheckBox ) break;
			if( pCheckBox->IsActive() && pCheckBox->IsChecked() )
			pDlg->GetWindowForID( MT_PWDEDITBOX )->SetDisable( TRUE );
			#endif
			*/
		}
		break;
	case MBI_DELETEICONCHECK:
		{
			switch( we )
			{
			case MBI_YES:
				{
					ITEMMGR->SendDeleteItem();
					// 071004 LYW --- GlobalEventFunc : 아이템 버릴 때 나는 소리 변경.
					//AUDIOMGR->Play(67, HERO);
					AUDIOMGR->Play(69, HERO);
					break;
				}
			case MBI_NO:
				{
					GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
					GAMEIN->GetStorageDialog()->SetDisable( FALSE );

					// 길드창을 사용 가능하게 한다.
					{
						cDialog* dialog = WINDOWMGR->GetWindowForID( GD_GUILDDLG );
						ASSERT( dialog );

						dialog->SetDisable( FALSE );
					}

					break;
				}
			}
		}
		break;
	case MBI_DELETEPETICONCHECK:
		{
			switch( we )
			{
			case MBI_YES:
				{
					GAMEIN->GetPetWearedDlg()->SendDelete();
					AUDIOMGR->Play(69, HERO);
					break;
				}
			case MBI_NO:
				{
					GAMEIN->GetPetWearedDlg()->SetDisable( FALSE );
				}
			}
		}
		break;

	case MBI_FRIEND_ADD:
		{
			if(we == MBI_YES)
			{
				FRIENDMGR->FriendInviteAccept();

			}
			else if( we == MBI_NO )
			{
				FRIENDMGR->FriendInviteDeny();
			}
		}
		break;
	case MBI_SHOWDOWNAPPLY:
		{
			if(we == MBI_YES)
			{
				SHOWDOWNMGR->AcceptShowdown( TRUE );
			}
			else if( we == MBI_NO )
			{
				SHOWDOWNMGR->AcceptShowdown( FALSE );
			}
		}
		break;
	case MBI_SHOWDOWNWAITING:
		{
			if(we == MBI_CANCEL)
			{
				SHOWDOWNMGR->CancelApply();
			}
		}
		break;
	case MBI_CHARSELERROR:
		{
			CHARSELECT->SetDisablePick( FALSE );
		}
		break;
	case MBI_CHARMAKEERROR:
		{
			CHARMAKE->SetDisableDlg( FALSE );

			// 070619 LYW --- GlobalEventFunc : Setting To Disable Gender button.
			// 성별을 선택할 수 없도록 하기 위해 Disable로 세팅해 준다.
			CHARMAKE->GetCharMakeNewDlg()->GetPushUpButton(e_PB_MAN)->SetDisable(TRUE) ;
			CHARMAKE->GetCharMakeNewDlg()->GetPushUpButton(e_PB_WOMAN)->SetDisable(TRUE) ;
		}
		break;
	case MBI_PARTY_SECEDE:
		{
			if(we == MBI_YES)
			{
				// 070110 LYW --- Modified this code.
				//PARTYMGR->DelPartyMemberSyn();
				if( GAMEIN->GetGuageDlg()->GetHeroState() == e_CAPTAIN )
				{
					GAMEIN->GetGuageDlg()->SecessionPartyCaptain() ;
					PARTYMGR->DelPartyMemberSyn();
				}
				else
				{
					PARTYMGR->DelPartyMemberSyn();
				}

				// 070316 LYW --- GlobalEventFunc : .
				PARTYMGR->BanPartyMemberSyn(HERO->GetID());
			}
		}
		break;
	case MBI_MAPCHANGE:
		{
			GAMEIN->GetMoveDialog()->SetItemToMapServer( 0, 0, FALSE );

			if(we == MBI_YES)
			{
				cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID( MBI_MAPCHANGE );
				CNpc* pNpc = (CNpc*)OBJECTMGR->GetObject( pMsgBox->GetParam() );
				if( pNpc )
				{
					if( !HERO->IsPKMode() && !PKMGR->IsPKLooted() && !HERO->IsDied() )	//·cÆA´cCI°i AOA≫¶§, A×¾uA≫¶§μμ AIμ¿ ¸øCI°O ¸·A½.
						g_UserInput.GetHeroMove()->MapChange( pNpc->GetNpcUniqueIdx() );
				}
				else
				{
					//error ￠®E￠®ⓒ­EAI￠®Ii￠®E?A¡E￠ci CO ￠®¡×uo ￠®¡×u¡§I¡§¡I￠®¡×oA￠®E￠®II￠®E￠®IU.
				}
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			}
			else if( we == MBI_NO )
			{
				// Server￠®E?￠®E¡Ec ¡E￠cioAA￠®E￠®ⓒ­| ￠®¡×uE￠®E￠®ⓒ­¡E￠c¡§￠®￠®E￠®IU.
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			}
		}
		break;
	case MBI_OVERLAPPEDLOGININOTHERSERVER:
		{
			TITLE->NoDisconMsg();		//
			TITLE->ShowServerList();
/*
			cDialog* dlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( dlg )
			{
				dlg->SetDisable( FALSE );
				cEditBox* editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
				cEditBox* editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
				if( editboxId )
				{
					editboxId->SetEditText("");
					editboxId->SetFocusEdit(TRUE);
				}
				if( editboxPwd )
				{
					editboxPwd->SetEditText("");
					editboxPwd->SetFocusEdit(FALSE);
				}
			}
*/
		}
		break;
	case MBI_OVERLAPPEDLOGIN:
		{			
			if(we == MBI_YES)
			{
				TITLE->NoDisconMsg();		
				TITLE->ShowServerList();
				//for (int i=0; i<5; i++)
				//{
				//	if(i<4)
				//	{
				//		MSGBASE msg;
				//		msg.Category = MP_USERCONN;
				//		msg.Protocol = MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN;
				//		NETWORK->Send(&msg,sizeof(msg));
				//	}
				//	else if (i==4)
				//	{
				//		TITLE->NoDisconMsg();		
				//		TITLE->ShowServerList();
				//		//cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORRETURN, MBT_OK, CHATMGR->GetChatMsg(4) );
				//		//cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
				//		//if( pIDDlg && pMsgBox )
				//		//{
				//		//	pIDDlg->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
				//		//}
				//	}
				//}
					// 080111 LYW --- GlovalEventFunc : 숫자 패드 비활성화 처리.
/*					cDialog* pDlg = NULL ;
					pDlg = WINDOWMGR->GetWindowForID( NUMBERPAD_DLG ) ;

					if( !pDlg ) return ;

					pDlg->SetActive(FALSE) ;
*/				
			}
			else if( we == MBI_NO )
			{
				TITLE->NoDisconMsg();		//
				TITLE->ShowServerList();
/*				cDialog* dlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
				if( dlg )
				{
					dlg->SetDisable( FALSE );
					cEditBox* editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
					cEditBox* editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
					if( editboxId )
					{
						editboxId->SetEditText("");
						editboxId->SetFocusEdit(TRUE);
					}
					if( editboxPwd )
					{
						editboxPwd->SetEditText("");
						editboxPwd->SetFocusEdit(FALSE);
					}
				}				
*/			}
		}
		break;
	case MBI_SEND_DISCONNECT_OVERLAPPEDLOGIN:
		{

		}
		break;
	case MBI_LOGINERRORCLOSE:
		{
			PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
/*			//			MAINGAME->SetGameState(eGAMESTATE_END);
			cDialog* dlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( dlg )
			{
				dlg->SetDisable( FALSE );
				cEditBox* editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
				if( editboxId )
				{
					editboxId->SetEditText("");
					editboxId->SetFocusEdit(TRUE);
				}
				cEditBox* editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
				if( editboxPwd )
				{
					editboxPwd->SetEditText("");
					editboxPwd->SetFocusEdit(FALSE);
				}
			}

			TITLE->NoDisconMsg();		//
			TITLE->ShowServerList();
*/		}
		break;
	case MBI_LOGINERRORRETURN:
		{
			TITLE->NoDisconMsg();		//
			TITLE->ShowServerList();
		}
		break;
	case MBI_CANTUSEDYNAMIC:
		{
			cDialog* pDlg = WINDOWMGR->GetWindowForID( MT_DYNAMICDLG );
			if( pDlg )
			{
				pDlg->SetDisable( FALSE );
				((cEditBox*)pDlg->GetWindowForID( MT_DYNAMICPWDEDITBOX ))->SetEditText( "" );
				((cEditBox*)pDlg->GetWindowForID( MT_DYNAMICPWDEDITBOX ))->SetFocusEdit( TRUE );
			}			
		}
		break;
	case MBI_SERVERDISCONNECT:
		{
			//			MAINGAME->SetGameState(eGAMESTATE_END);
			//PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
			MAINGAME->SetGameState(eGAMESTATE_TITLE);
			TITLE->ShowServerList();
		}
		break;
	case MBI_DISTCONNECT_ERROR:
		{
			WINDOWMGR->CloseAllMsgBoxNoFunc();
			TITLE->ShowServerList();
		}
		break;
	case MBI_MAPSERVER_CLOSE:
		{
			CHARSELECT->SetDisablePick( FALSE );
		}
		break;
	case MBI_SELLITEM:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetDealDialog()->SendSellItemMsg();
			}
			else
			{
				CDealDialog::OnCancelSellItem(0,NULL,0,GAMEIN->GetDealDialog(),NULL);
				//GAMEIN->GetDealDialog()->CancelSellItem();
			}
		}
		break;
	case MBI_BUYITEM:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetDealDialog()->SendBuyItemMsg();
			}
			else
			{
				GAMEIN->GetDealDialog()->CancelBuyItem();
			}
		}
		break;
	case MBI_PKMODE:
		{
			if( we == MBI_YES )
			{
				PKMGR->SendMsgPKOn();
			}
			else
			{

			}
		}
		break;
	case MBI_NOT_GM:
		{
			//			MAINGAME->SetGameState( eGAMESTATE_END );
			PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
		}
		break;

	case MBI_WAIT_LOGINCHECK:
		{
			//￠®E?￠®¡×I¡E￠c¡§ua￠®¡×u¡E￠c¡§I ￠®E￠®ⓒ­￠®¡×￠®¨I￠®¡×u￠®¡×uAo￠®E￠®ⓒ­| ￠®¡×￠®ⓒ­￠®E￠®ⓒ­¡§I¨I¨￡¡E￠ci¡E￠c¡§￠®i ¡§Ii¨Io￠®Ei¡§o￠®¡×u￠®¡×¡E￠c￠®¡×u¡E￠c¡§I A¡§I¨I¨￡￠®E￠®ⓒ­￠®Ec? ¡E￠c¡E￠￥¡E￠cEAI￠®EO¡E￠c￠®￠?￠®E￠®IA AI¡E￠c¡§￠®I￠®Ii￠®Ii A¡§I¨I¨￡￠®E￠®ⓒ­￠®Ec ￠®¡×uE￠®IiE¡E￠c¡§￠®I ￠®¡×u￠®¡×¡E￠c￠®E￠®IN¡E￠c¡§￠®￠®E¡Ec?

			cDialog* dlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( dlg )
			{
				cEditBox* editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
				cEditBox* editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
				if( editboxId )
				{
					editboxId->SetEditText("");
					editboxId->SetFocusEdit(TRUE);
				}
				if( editboxPwd )
				{
					editboxPwd->SetEditText("");
					editboxPwd->SetFocusEdit(FALSE);
				}
			}

			TITLE->NoDisconMsg();		//￠®¡×u￠®¡×¡E￠c¡E￠c¡E￠￥￠®E¡Ec showserverlist￠®E?￠®E¡Ec￠®¡×u¡E￠c¡§I A¡E￠cE￠®¡×uO ¡§I￠®"￠cAA¡E￠ci￠®EO¡E￠c￠®￠? ¡§I￠®Ao¡E￠c¡§ue ￠®E￠®ⓒ­￠®¡×￠®¨I￠®¡×u￠®¡×uAo ￠®¡×u¡§I¡§¡I￠®Ii￠®Ii¡E￠c¡E￠￥I.
			TITLE->ShowServerList();
			TITLE->StartWaitConnectToAgent( FALSE );
		}
		break;
	case MBI_TIMEOVER_TOCONNECT:
		{
			//			TITLE->NoDisconMsg();		//￠®¡×u￠®¡×¡E￠c¡E￠c¡E￠￥￠®E¡Ec showserverlist￠®E?￠®E¡Ec￠®¡×u¡E￠c¡§I A¡E￠cE￠®¡×uO ¡§I￠®"￠cAA¡E￠ci￠®EO¡E￠c￠®￠? ¡§I￠®Ao¡E￠c¡§ue ￠®E￠®ⓒ­￠®¡×￠®¨I￠®¡×u￠®¡×uAo ￠®¡×u¡§I¡§¡I￠®Ii￠®Ii¡E￠c¡E￠￥I.
			//			TITLE->ShowServerList();
			//			TITLE->StartWaitConnectToAgent( FALSE );
			TITLE->GetServerListDialog()->SetActive( TRUE );
		}
		break;


	case MBI_STREETBUYMSG:
		{
			if( we == MBI_YES )
			{
				STREETSTALLMGR->SendBuyItemMsg();
			}
			else
			{
			}
		}
		break;

	case MBI_REGEDITMSG:
		{
			if( we == MBI_YES )
			{
				if(!GAMEIN->GetStreetBuyStallDialog()->IsActive())
					return;

				POSTYPE pos = GAMEIN->GetStreetBuyStallDialog()->GetCurSelectedItemNum();
				BUY_REG_INFO RegInfo = GAMEIN->GetStreetBuyStallDialog()->GetBuyRegInfo(pos);
				GAMEIN->GetBuyRegDialog()->SetRegInfo(RegInfo);
				GAMEIN->GetBuyRegDialog()->Show();	

			}
			else
			{
				MSGBASE msg;
				msg.Category = MP_STREETSTALL;
				msg.dwObjectID = HEROID;
				msg.Protocol = MP_STREETSTALL_UPDATEEND_SYN;

				NETWORK->Send( &msg, sizeof(MSGBASE) );
			}
		}
		break;
	case MBI_STREETSELLMSG:
		{
			CStreetBuyStall* pStall = GAMEIN->GetStreetBuyStallDialog();

			if( we == MBI_YES )
				if(pStall && pStall->IsActive() && STREETSTALLMGR->SendSellItemMsg())
					break;
				else
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1218) );

			if(pStall) GAMEIN->GetStreetBuyStallDialog()->SetData(NULL);

			CItem* pItem = pStall->GetSellItem();

			if(pItem)	pItem->SetLock(FALSE);

			pStall->DelSellItem();
		}
		break;

	case MBI_QUESTGIVEUP:
		{
			if( we == MBI_YES )
				QUESTMGR->QuestAbandon();
			else
				QUESTMGR->QuestAbandonClose();
		}
		break;
	case MBI_BAILNOTICEMSG:
		{
			if( we == MBI_YES )
				GAMEIN->GetBailDialog()->SetBadFrameSync();
			else
				GAMEIN->GetBailDialog()->SetDisable( FALSE );

		}
		break;

	case MBI_QUESTSTART:
		{
			if( we == MBI_YES )
				GAMEIN->GetInventoryDialog()->ItemUseForQuestStart();

		}
		break;
	case MBI_BAILNOTICEERR:
		{
			GAMEIN->GetBailDialog()->SetDisable( FALSE );
		}
		break;
	case MBI_BAILLOWBADFAME:
		{
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		}
		break;
	case MBI_GUILD_SECEDE:
		{
			if( we == MBI_YES )
			{
				//GUILDMGR->SecedeSyn();

				if( SIEGEMGR->GetSiegeWarMapNum() == MAP->GetMapNum() )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(595) );
					return;
				}
				MSGBASE msg;
				msg.Category = MP_GUILD;
				msg.Protocol = MP_GUILD_SECEDE_SYN;
				msg.dwObjectID = HEROID;
				NETWORK->Send(&msg, sizeof(msg));
				GAMEIN->GetGuildDlg()->SetActive(FALSE);
			}
		}
		break;
	case MBI_GUILD_BREAKUP:
		{
			if( we == MBI_YES )
			{
				if( GUILDWARMGR->IsValid( 3 ) )
				{
					//GUILDMGR->BreakUpGuildSyn();

					if(HERO->GetGuildIdx() == 0)
					{
						char buf[128];
						sprintf(buf, "HOW CAN U SHOW DLG? chridx : %u", HEROID);
						ASSERTMSG(0, buf);
						return;
					}
					if(HERO->GetGuildMemberRank() != GUILD_MASTER)
					{
						char buf[128];
						sprintf(buf, "HOW CAN U BREAKUP? chridx : %u, rank : %d", HEROID, HERO->GetGuildMemberRank());
						ASSERTMSG(0, buf);
						return;
					}
					if( HERO->GetGuildUnionIdx() != 0 )
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(591));
						return;
					}
					//if(IsWaiting() == TRUE)
					//{
					//	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(592));
					//	return;
					//}
				
					MSGBASE msg;
					msg.Category = MP_GUILD;
					msg.Protocol = MP_GUILD_BREAKUP_SYN;
					msg.dwObjectID = HEROID;
				
					NETWORK->Send(&msg, sizeof(msg));	
				}
			}
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		}
		break;
	case MBI_GUILD_BAN:
		{
			CGuildDialog* dialog = GAMEIN->GetGuildDlg();

			if( ! dialog )
			{
				break;
			}

			// 080417 LUJ, 메시지 창 표시 전에 잠갔으므로 반드시 다시 풀어주자
			dialog->SetDisable( FALSE );

			if(we == MBI_YES)
			{
				//GUILDMGR->DeleteMemberSyn();
				if( SIEGEMGR->GetSiegeWarMapNum() == MAP->GetMapNum() )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(595) );
					return;
				}
			
				MSG_DWORD msg;
				msg.Category	= MP_GUILD;
				msg.Protocol	= MP_GUILD_DELETEMEMBER_SYN;
				msg.dwObjectID	= HEROID;
				msg.dwData		= dialog->GetKickMemberIndex();
				NETWORK->Send(&msg, sizeof(msg));
			}
		}
		break;
	case MBI_SHOPITEM_LOCK:
		{
			if( we == MBI_YES )
				GAMEIN->GetInventoryDialog()->ItemUnSealingSync();
		}
		break;

	case MBI_SAVEDPOINTMOVE:
		{
			if( we == MBI_YES )
				GAMEIN->GetMoveDialog()->MapMoveOK();
			else if( we == MBI_NO )
				GAMEIN->GetMoveDialog()->SetActive( FALSE );
		}
		break;
	case MBI_SAVEDPOINTDEL:
		{
			if( we == MBI_YES )
				GAMEIN->GetMoveDialog()->DelMoveInfoSync();
			else if( we == MBI_NO )
				GAMEIN->GetMoveDialog()->SetActive( FALSE );
		}
		break;
	case MBI_OTHERREVIVE:
		{
			if( we == MBI_YES )			
				ITEMMGR->ReviveOtherSync();			
			else if( we == MBI_NO )			
				ITEMMGR->ReviveOtherCancel();			
		}
		break;
	case MBI_REVIVECONFIRM:
		{
			if( we == MBI_YES )
				ITEMMGR->ReviveOtherConfirm( TRUE );
			else if( we == MBI_NO )
				ITEMMGR->ReviveOtherConfirm( FALSE );
		}
		break;

	case MBI_REVIVE_PRESENTSPOT :
		{
			if( we == MBI_YES )
			{
				MSGBASE msg;
				msg.Category = MP_CHAR_REVIVE;
				msg.Protocol = MP_CHAR_REVIVE_PRESENTSPOT_SYN;
				msg.dwObjectID = HEROID;
				NETWORK->Send(&msg, sizeof(msg));

				cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_REVIVECONFIRM );
				if( pDlg )
				{
					WINDOWMGR->AddListDestroyWindow( pDlg );
					ITEMMGR->ReviveOtherConfirm( FALSE );
				}
			}
			else
			{
				GAMEIN->GetReviveDialog()->SetDisable(FALSE);
			}
		}
		break ;

	case MBI_REVIVE_TOWNSOPT :
		{
			if( we == MBI_YES )
			{
				MSGBASE msg;
				msg.Category = MP_CHAR_REVIVE;
				msg.Protocol = MP_CHAR_REVIVE_VILLAGESPOT_SYN;
				msg.dwObjectID = HEROID;
				NETWORK->Send(&msg, sizeof(msg));

				cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_REVIVECONFIRM );
				if( pDlg )
				{
					WINDOWMGR->AddListDestroyWindow( pDlg );
					ITEMMGR->ReviveOtherConfirm( FALSE );
				}
			}
			else
			{
				GAMEIN->GetReviveDialog()->SetDisable(FALSE);
			}
		}
		break ;

		// guildfieldwar -----------------------------------------------------------------
	case MBI_GUILDFIELDWAR_DECLARE_ACCEPT:
		{
			GAMEIN->GetGFWarResultDlg()->SetDisable( FALSE );
			if( we == MBI_YES )
				GUILDWARMGR->SendDeclareAcceptDeny( MP_GUILD_WAR_DECLARE_ACCEPT );
		}
		break;
	case MBI_GUILDFIELDWAR_DECLARE_DENY:
		{
			GAMEIN->GetGFWarResultDlg()->SetDisable( FALSE );
			if( we == MBI_YES )
				GUILDWARMGR->SendDeclareAcceptDeny( MP_GUILD_WAR_DECLARE_DENY );
		}
		break;	
	case MBI_GUILDFIELDWAR_SUGGEST:
		{
			GAMEIN->GetGFWarInfoDlg()->SetDisable( FALSE );
			//			GAMEIN->GetGFWarInfoDlg()->SetActive( FALSE );
			if( we == MBI_YES )
				GUILDWARMGR->SendSuggest();
		}
		break;
	case MBI_GUILDFIELDWAR_SURREND:
		{
			GAMEIN->GetGFWarInfoDlg()->SetDisable( FALSE );
			//			GAMEIN->GetGFWarInfoDlg()->SetActive( FALSE );
			if( we == MBI_YES )
				GUILDWARMGR->SendSurrender();
		}
		break;
	case MBI_GUILDFIELDWAR_SUGGEST_ACCEPTDENY:
		{
			if( we == MBI_YES )
			{
				GUILDWARMGR->SendSuggestAcceptDeny( MP_GUILD_WAR_SUGGESTEND_ACCEPT );
			}
			else if( we == MBI_NO )
			{
				GUILDWARMGR->SendSuggestAcceptDeny( MP_GUILD_WAR_SUGGESTEND_DENY );
			}
		}
		break;
	case MBI_GUILDFIELDWAR_REVIVE:
		{
			if( MBI_YES == we )
			{
				// 부활 선택 창 끄고
				{
					cDialog* dialog = WINDOWMGR->GetWindowForID( GD_REVIVALDLG );
					ASSERT( dialog );

					dialog->SetActive( FALSE );
				}

				// 부활 요청
				{
					MSGBASE message;

					message.Category	= MP_CHAR_REVIVE;
					message.Protocol	= MP_CHAR_REVIVE_LOGINSPOT_SYN;
					message.dwObjectID	= HEROID;

					NETWORK->Send( &message, sizeof( message ) );
				}
			}
		}
		break;
		//--------------------------------------------------------------------------------
		// guildunion
	case MBI_UNION_INVITE:
		{
			if( we == MBI_YES )
				GUILDUNION->SendInviteAcceptDeny( MP_GUILD_UNION_INVITE_ACCEPT );
			else if( we == MBI_NO )
				GUILDUNION->SendInviteAcceptDeny( MP_GUILD_UNION_INVITE_DENY );
		}
		break;
	case MBI_UNION_REMOVE:
		{
			if( we == MBI_YES )
				GUILDUNION->SendRemoveGuildUnion();			
		}
		break;
	case MBI_UNION_DESTROY:
		{
			if( we == MBI_YES )
				GUILDUNION->SendDestroyGuildUnion();
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		}
		break;
	case MBI_UNION_LEAVE:
		{
			if( we == MBI_YES )
			{
				MSGBASE message;
				message.Category	= MP_GUILD_UNION;
				message.Protocol	= MP_GUILD_UNION_SECEDE_SYN;
				message.dwObjectID	= HEROID;
				
				NETWORK->Send( &message, sizeof( message ) );
			}
		}
		break;
		//----------------------------------------------------------------------------------
	case MBI_PRESENT_NOTIFY:
		{
			//
			GAMEIN->GetNoteDialog()->SetActive( TRUE );
			GAMEIN->GetNoteDialog()->SetMode( eMode_PresentNote );
			NOTEMGR->NoteListSyn( 1, eMode_PresentNote );
			GAMEEVENTMGR->AddEvent( eGameEvent_NoteInterface );

			// 061220 LYW --- Delete this code.
			/*
			CBarIcon* pIcon = GAMEIN->GetMainInterfaceDialog()->GetBarIcon( OPT_NOTEDLGICON );
			if( pIcon )
				pIcon->SetPush( TRUE );
			*/
		}
		break;
		// partywar
	case MBI_PARTYWAR_SUGGEST:
		{
			if( we == MBI_YES )
				PARTYWAR->SendPartyWarSuggestReturn( MP_PARTYWAR_SUGGEST_ACCEPT );
			else if( we == MBI_NO )
				PARTYWAR->SendPartyWarSuggestReturn( MP_PARTYWAR_SUGGEST_DENY );
		}
		break;
	case MBI_CHANGENAME_ACK:
	case MBI_JOBCHANGE_ACK:
		{
			// 대기화면으로
			EXITMGR->SendExitMsg( eEK_CHARSEL );
		}
		break;
	case MBI_WANTBUY:
		{
			if( we == MBI_YES )
			{
				DWORD WantedID = GAMEIN->GetWantNpcDialog()->GetCurWantedIdx();
				WANTEDMGR->BuyWantedRightSyn(WantedID);
			}
		}
		break;
	case MBI_WANTGIVEUP:
		{
			if( we == MBI_YES )
			{
				DWORD WantedID = GAMEIN->GetWantNpcDialog()->GetCurWantedIdx();
				WANTEDMGR->GiveUpWantedRightSyn(WantedID);
			}
		}
		break;
	case MBI_GTREGIST_SYN:
		{
			if( we == MBI_YES )
			{
				if( GAMEIN->GetGTRegistDlg() )
				{
					DWORD res;
					if( (res = GAMEIN->GetGTRegistDlg()->TournamentRegistSyn()) != eGTError_NOERROR )
					{
						char buf[256];
						if( res == eGTError_NOGUILDMASTER )
							sprintf( buf, CHATMGR->GetChatMsg( 976 ) );
						else if( res == eGTError_UNDERLEVEL )
							sprintf( buf, CHATMGR->GetChatMsg( 977 ), LIMITLEVEL_GUILDTOURNAMENT );
						else if( res == eGTError_UNDERLIMITEMEMBER )
							sprintf( buf, CHATMGR->GetChatMsg( 978 ), LIMITMEMBER_GUILDTOURNAMENT );
						else if( res == eGTError_NOTREGISTDAY )
							sprintf( buf, CHATMGR->GetChatMsg( 979 ), LIMITMEMBER_GUILDTOURNAMENT );

						CHATMGR->AddMsg( CTC_SYSMSG, buf );
					}
				}
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				if( GAMEIN->GetGTRegistDlg() )
				{
					if( GAMEIN->GetGTRegistDlg()->IsActive() )
						GAMEIN->GetGTRegistDlg()->SetActive( FALSE );
				}
			}
		}
		break;
	case MBI_GTREGISTCANCEL_SYN:
		{
			if( we == MBI_YES )
			{
				if( GAMEIN->GetGTRegistcancelDlg() )
				{
					GAMEIN->GetGTRegistcancelDlg()->TournamentRegistCancelSyn();
				}
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				if( GAMEIN->GetGTRegistcancelDlg() )
				{
					if( GAMEIN->GetGTRegistcancelDlg()->IsActive() )
						GAMEIN->GetGTRegistcancelDlg()->SetActive( FALSE );
				}
			}
		}
		break;
	case MBI_GTBATTLELIST_OBSERVER:
		{
			if( we == MBI_YES )
			{
				if( GAMEIN->GetGTBattleListDlg() )
				{
					if( GAMEIN->GetGTBattleListDlg()->EnterBattleonObserverSyn() == FALSE )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(958) );
						if( HERO->GetState() == eObjectState_Deal )
							OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
						if( GAMEIN->GetGTBattleListDlg()->IsActive() )
							GAMEIN->GetGTBattleListDlg()->SetActive( FALSE );
					}
				}
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				if( GAMEIN->GetGTBattleListDlg() )
				{
					if( GAMEIN->GetGTBattleListDlg()->IsActive() )
						GAMEIN->GetGTBattleListDlg()->SetActive( FALSE );
				}
			}
		}
		break;
	case MBI_GTSCORE_EXIT:
		{
			if( we == MBI_YES )
			{
				MSG_DWORD msg;
				msg.Category = MP_GTOURNAMENT;
				msg.Protocol = MP_GTOURNAMENT_OBSERVER_EXIT;
				msg.dwData = HERO->GetBattleID();
				msg.dwObjectID = HEROID;
				NETWORK->Send( &msg, sizeof(msg) );
			}
		}
		break;
	case MBI_SW_PROTECTREG_OK:
		{
			if( we == MBI_YES )
			{
				DWORD dwGuildIdx = GAMEIN->GetSWProtectRegDlg()->GetSelectedGuildIdx();
				if( !dwGuildIdx )	return;

				MSG_DWORD Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol = MP_SIEGEWAR_ACCEPTGUILD_SYN;
				Msg.dwObjectID = HEROID;
				Msg.dwData = dwGuildIdx;
				NETWORK->Send( &Msg, sizeof(Msg) );

				GAMEIN->GetSWProtectRegDlg()->SetDisable( FALSE );
				GAMEIN->GetSWProtectRegDlg()->SetActive( FALSE );
			}
			else if( we == MBI_NO )
				GAMEIN->GetSWProtectRegDlg()->SetDisable( FALSE );
		}
		break;
	case MBI_SW_TAXCHANGE_OK:
		{
			if( we == MBI_YES )
			{
				cEditBox* pTax = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_CHANGE_TAX);
				if( pTax )
				{
					if( SWPROFIT->CheckSWProfit( 0, atoi(pTax->GetEditText()), 0 ) )
					{
						SWPROFIT->SendChangTexrateSyn( atoi(pTax->GetEditText()) );
						GAMEIN->GetSWProfitDlg()->SetDisable( FALSE );
						GAMEIN->GetSWProfitDlg()->SetActive( FALSE );
					}
				}
			}
			else if( we == MBI_NO )
				GAMEIN->GetSWProfitDlg()->SetDisable( FALSE );
		}
		break;
	case MBI_SW_PROFIT_OK:
		{
			if( we == MBI_YES )
			{
				cEditBox* pMoney = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_OUT_MONEY);
				if( pMoney )
				{
					if( SWPROFIT->CheckSWProfit( 1, 0, atoi(pMoney->GetEditText()) ) )
					{
						SWPROFIT->SendGetMoneySyn( atoi(pMoney->GetEditText()) );
						GAMEIN->GetSWProfitDlg()->SetDisable( FALSE );
						GAMEIN->GetSWProfitDlg()->SetActive( FALSE );
					}
				}
			}
			else if( we == MBI_NO )
				GAMEIN->GetSWProfitDlg()->SetDisable( FALSE );
		}
		break;
		/*
		071209 LUJ, 해당 기능 사용하지 않음
	case MBI_GUILD_CHANGE_LOCATION:
		{
			if( we == MBI_YES )
			{
				GUILDMGR->SendChangeLocation();
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
			}
		}
		break;
		*/
	case MBI_SW_ENGRAVE_SYN:
		{
			if( we == MBI_YES )
			{
				MSG_DWORD msg;
				SetProtocol( &msg, MP_SIEGEWAR, MP_SIEGEWAR_ENGRAVE_SYN );
				msg.dwObjectID = HEROID;
				msg.dwData = HERO->GetGuildIdx();
				NETWORK->Send( &msg, sizeof(msg) );			
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
			}
		}
		break;
	case MBI_SW_PROPOSAL_SYN:
		{
			if( we == MBI_YES )
			{
				MSGBASE Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol = MP_SIEGEWAR_DEFENCE_REGIST_SYN;
				Msg.dwObjectID = HEROID;
				NETWORK->Send( &Msg, sizeof(Msg) );
			}
		}
		break;
	case MBI_SW_ATTACKREGIST_SYN:
		{
			if( we == MBI_YES )
			{
				MSGBASE Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol = MP_SIEGEWAR_ATTACK_REGIST_SYN;
				Msg.dwObjectID = HEROID;
				NETWORK->Send( &Msg, sizeof(Msg) );
			}
		}
		break;
	case MBI_SW_OBSERVER_SYN:
		{
			if( we == MBI_YES )
			{
				MSG_DWORD2 Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol =  MP_SIEGEWAR_MOVEIN_SYN;
				Msg.dwObjectID = HEROID;
				Msg.dwData1 = HERO->GetGuildIdx();
				Msg.dwData2 = 2;
				NETWORK->Send( &Msg, sizeof(Msg) );
			}
		}
		break;

		// 070607 LYW --- GlobalEventFunc : Add identification part.
	case MBI_IDENTIFICATION_REGIST_INFO :
		{
			CIdentification* pDlg = GAMEIN->GetIdentificationDlg() ;

			if( we == MBI_YES )
			{
				if( pDlg )
				{
					// check all data is ok.
					CSHResidentRegistManager::stRegistInfo* pRegistInfo = pDlg->GetRegistInfo() ;

					// setting name.
					strcpy(pRegistInfo->szNickName, HERO->GetObjectName()) ;

					// setting gender.
					if( strcmp(pDlg->GetGender()->GetStaticText(), "남") == 0 )
					{
						pRegistInfo->nSex = 0 ;
					}
					else
					{
						pRegistInfo->nSex = 1 ;
					}

					// setting age.
					pRegistInfo->nAge = (WORD)atoi(pDlg->GetAge()->GetEditText()) ;

					// setting location.
					pRegistInfo->nLocation = pDlg->GetRegion()->GetCurSelectedIdx()+1 ;

					// setting favor.
					pRegistInfo->nFavor = 0 ;

					g_csResidentRegistManager.CLI_RequestRegist(pRegistInfo) ;
				}
			}
			else
			{
				pDlg->CancelRegist() ;
			}
		}
		break ;

	case MBI_IDENTIFICATION_ALREADY_HAVE :
		{
			// Deactive npc script dialog.
			if( HERO->GetState() != eObjectState_Die )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal) ;
		}
		break ;

	case MBI_MATCHAT_INVITE :
		{
			// check.
			if( we == MBI_YES )
			{
				// start chatting.
				g_csResidentRegistManager.CLI_StartDateMatchingChat() ;
			}
			else
			{
				// send message.
				g_csResidentRegistManager.CLI_RefuseDateMatchingChat() ;
			}
		}
		break ;

	case MBI_MATCHAT_CANCEL :
		{
			if( we == MBI_CANCEL )
			{
				// cancel invite.
				g_csResidentRegistManager.CLI_CancelDateMatchingChat();
			}
		}
		break ;

	case MBI_END_CHATTING_PARTNERLIST :
		{
			if( we == MBI_YES )
			{
				// end chatting.
				g_csResidentRegistManager.CLI_EndDateMatchingChat() ;

				// return to partner list.
				GAMEIN->GetDateMatchingDlg()->SetUpperDlg(e_PartnerListDlg) ;
			}
			else if( we == MBI_NO )
			{
				GAMEIN->GetDateMatchingDlg()->GetPartnerTabBtn()->SetPush(FALSE) ;
			}
		}
		break ;

	case MBI_END_CHATTING_CLOSE :
		{
			if( we == MBI_YES )
			{
				// end chatting.
				g_csResidentRegistManager.CLI_EndDateMatchingChat() ;

				// close matching dialog.
				GAMEIN->GetDateMatchingDlg()->SetActive(FALSE) ;
			}
		}
		break ;

	case MBI_FAMILY_INVITE_YOU :
		{
			/*enum FAMILY_INVITE_RESULT
			{
				FIR_ACCEPT_MASTER = 0,
					FIR_ACCEPT_PLAYER,
					FIR_DENY_MASTER,
					FIR_DENY_PLAYER,
			};*/
			if( we == MBI_YES )
			{
				g_csFamilyManager.CLI_RequestAnswerToInvite(CSHFamilyManager::FIR_ACCEPT_PLAYER) ;
			}
			else
			{
				g_csFamilyManager.CLI_RequestAnswerToInvite(CSHFamilyManager::FIR_DENY_PLAYER) ;
			}
		}
		break ;

		// 길드 권한 이양 확인 메시지 창
	case MBI_GUILD_RETIRE_NOTIFY:
		{
			CGuildDialog* dialog = ( CGuildDialog* )WINDOWMGR->GetWindowForID( GD_GUILDDLG );
			ASSERT( dialog );

			dialog->SetDisable( FALSE );

			const GUILDMEMBERINFO* member = dialog->GetSelectedMember();
			ASSERT( member );

			if( we == MBI_YES )
			{
				MSG_DWORD message;
				message.Category	= MP_GUILD;
				message.Protocol	= MP_GUILD_RETIRE_SYN;
				message.dwObjectID	= HEROID;
				message.dwData		= member->MemberIdx;

				NETWORK->Send( &message, sizeof( message ) );
			}
		}
		break;
	case MBI_GUILD_RETIRE_REQUEST:
		{
			// 길드 창에서 길드 마스터 인덱스를 가져온다.
			CHero* hero = OBJECTMGR->GetHero();
			ASSERT( hero );

			MSG_DWORD message;
			message.Category	= MP_GUILD;
			message.dwObjectID	= hero->GetID();
			message.dwData		= hero->GetID();
			message.Protocol	= ( MBI_YES == we ? MP_GUILD_RETIRE_ACK : MP_GUILD_RETIRE_NACK );

			NETWORK->Send( &message, sizeof( message ) );
		}
		break;
		// 아이템 조합을 복수로 할때 실행 여부에 대한 확인 창을 표시한다.
	case MBI_ITEM_MIX_CONFIRM:
		{
			// 진행 표시가 끝난 후 서버에 작업을 요청할 것이다
			if( we == MBI_YES )
			{
				CProgressDialog* dialog = ( CProgressDialog* )WINDOWMGR->GetWindowForID( PROGRESS_DIALOG );
				ASSERT( dialog );

				dialog->Wait( CProgressDialog::eActionMix );
			}
			else
			{
				CMixDialog* dialog = ( CMixDialog* )WINDOWMGR->GetWindowForID( ITMD_MIXDLG );
				ASSERT( dialog );

				dialog->Restore();
			}
		}
		break;
		// 아이템 강화 옵션이 있는 경우 옵션이 낮아질 수 있음에 대한 경고를 표시한다
	case MBI_ITEM_REINFORCE_CONFIRM:
		{
			// 진행 표시가 끝난 후 서버에 작업을 요청할 것이다
			if( we == MBI_YES )
			{
				CProgressDialog* dialog = ( CProgressDialog* )WINDOWMGR->GetWindowForID( PROGRESS_DIALOG );
				ASSERT( dialog );

				dialog->Wait( CProgressDialog::eActionReinforce );
			}
			else
			{
				CReinforceDlg* dialog = ( CReinforceDlg* )WINDOWMGR->GetWindowForID( ITR_REINFORCEDLG );
				ASSERT( dialog );

				dialog->Restore();
			}
		}
		break;
	// desc_hseos_패밀리01
	// S 패밀리 추가 added by hseos 2007.10.16
	// ..탈퇴
	case MBI_FAMILY_LEAVE:
		{
			if( we == MBI_YES )
			{
				g_csFamilyManager.CLI_RequestLeave();
			}
		}
		break;
	// ..추방
	case MBI_FAMILY_EXPEL:
		{
			if( we == MBI_YES )
			{
				g_csFamilyManager.CLI_RequestExpel(HERO->GetFamily()->GetMember(GAMEIN->GetFamilyDlg()->GetSelectedMemberIdx())->Get()->nID);
			}
		}
		break;
	// ..해체
	case MBI_FAMILY_BREAKUP:
		{
			if( we == MBI_YES )
			{
				g_csFamilyManager.CLI_RequestBreakUp();
			}
		}
		break;

	case MBI_SKILLTRAIN:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetSkillTrainingDlg()->Training();
			}
			else
			{
				GAMEIN->GetSkillTrainingDlg()->SetDisable( FALSE );
			}
		}
		break;
	// E 패밀리 추가 added by hseos 2007.10.16

	// 071201 LYW --- GlovalEventFunc : 
	case MBI_MAPMOVE_AREYOUSURE :
		{
			cMapMoveDialog* pDlg = NULL ;
			pDlg = GAMEIN->GetMapMoveDlg() ;

			if( !pDlg ) return ;

			if( we == MBI_YES )
			{
				pDlg->MoveMapSyn() ;
			}
			else if( we == MBI_NO )
			{
				pDlg->SetDisableState(FALSE) ;
			}
		}
		break ;

	// 071203 LYW --- GlovalEventFunc : 현재 마나 감소 효과를 취소하고 다시 적용할 것인지 물어본다.
	case MBI_FORCE_USEBUFFITEM_AREYOUSURE :
	case MBI_USEBUFFITEM_AREYOUSURE :
		{
			CInventoryExDialog* pDlg = NULL ;
			pDlg = GAMEIN->GetInventoryDialog() ;

			if( !pDlg ) return ;

			if( we == MBI_YES )
			{
				CItem* pItem = NULL ;
				// 인벤에 아이템 등록 하고 사용하면, 현재 선택 된 아이템이 없을 때 처리를 못하기 때문에
				// GetCurSelectedItem은 사용하지 않는다.
				//pItem = pDlg->GetCurSelectedItem(INV) ;	

				pItem = pDlg->Get_QuickSelectedItem() ;

				if( !pItem ) return ;

				ITEMMGR->UseItem_ItemMallItem(pItem) ;
			}
		}
		break ;

	// 071204 LYW --- GlovalEventFunc : 스탯 초기화 확인 메시지 처리.
	case MBI_RESETSTATS_AREYOUSURE :
		{
			CInventoryExDialog* pDlg = NULL ;
			pDlg = GAMEIN->GetInventoryDialog() ;

			if( !pDlg ) return ;

			CItem* pItem = NULL ;
			pItem = pDlg->Get_QuickSelectedItem() ;

			if( !pItem ) return ;

			if( we == MBI_YES )
			{
				ITEMMGR->UseItem_ResetStats(pItem) ;

				pItem->SetLock(FALSE) ;
			}

			pItem->SetLock(FALSE) ;
			pDlg->Set_QuickSelectedItem(NULL) ;
		}
		break ;

	// 071204 LYW --- GlovalEventFunc : 스킬 초기화 확인 메시지 처리.
	case MBI_RESETSKILL_AREYOUSURE :
		{
			CInventoryExDialog* pDlg = NULL ;
			pDlg = GAMEIN->GetInventoryDialog() ;

			if( !pDlg ) return ;

			CItem* pItem = NULL ;
			pItem = pDlg->Get_QuickSelectedItem() ;

			if( !pItem ) return ;

			if( we == MBI_YES )
			{
				ITEMMGR->UseItem_ResetSkill(pItem) ;

				pItem->SetLock(FALSE) ;
			}

			pItem->SetLock(FALSE) ;
			pDlg->Set_QuickSelectedItem(NULL) ;
		}
		break ;
	// 080218 KTH -- ResetRegidentRegist
	case MBI_RESET_REGIDENTREGIST :
		{
			CInventoryExDialog* pDlg = NULL;
			pDlg = GAMEIN->GetInventoryDialog() ;

			if( !pDlg ) return;

			CItem* pItem = NULL;
			pItem = pDlg->Get_QuickSelectedItem();

			if( !pItem ) return;

			if( we == MBI_YES )
			{
				ITEMMGR->UseItem_ResetRegistResident(pItem);
				pItem->SetLock(FALSE);
			}
			
			pItem->SetLock(FALSE);
			pDlg->Set_QuickSelectedItem(NULL);
		}
		break;

	// 071205 LYW --- GlovalEventFunc : 경험치 증가 확인 메시지 처리.
	case MBI_INCEXPRATE_AREYOUSURE :
		{
			CInventoryExDialog* pDlg = NULL ;
			pDlg = GAMEIN->GetInventoryDialog() ;

			if( !pDlg ) return ;

			if( we == MBI_YES )
			{
				CItem* pItem = NULL ;
				pItem = pDlg->Get_QuickSelectedItem() ;

				if( !pItem ) return ;

				ITEMMGR->UseItem_ItemMallItem(pItem) ;

				pDlg->Set_QuickSelectedItem(NULL) ;
			}
		}
		break ;

	// 071205 LYW --- GlovalEventFunc : 드랍률 증가 확인 메시지 처리.
	case MBI_INCDROPRATE_AREYOUSURE :
		{
			CInventoryExDialog* pDlg = NULL ;
			pDlg = GAMEIN->GetInventoryDialog() ;

			if( !pDlg ) return ;

			if( we == MBI_YES )
			{
				CItem* pItem = NULL ;
				pItem = pDlg->Get_QuickSelectedItem() ;

				if( !pItem ) return ;

				ITEMMGR->UseItem_ItemMallItem(pItem) ;

				pDlg->Set_QuickSelectedItem(NULL) ;
			}
		}
		break ;
	// 071210 LYW --- GlobalEventFunc : 인벤토리 확장 사용 메시지 처리.
	case MBI_EXTENDINVEN_AREYOUSURE :
		{
			CInventoryExDialog* pDlg = NULL ;
			pDlg = GAMEIN->GetInventoryDialog() ;

			if( !pDlg ) return ;

			CItem* pItem = NULL ;
			pItem = pDlg->Get_QuickSelectedItem() ;

			if( !pItem ) return ;

			if( we == MBI_YES )
			{
				ITEMMGR->UseItem_ExtendInventory(pItem) ;
			}

			pItem->SetLock(FALSE) ;
			pDlg->Set_QuickSelectedItem(NULL) ;
		}
		break ;
	// desc_hseos_결혼_01
	// S 결혼 추가 added by hseos 2007.12.11
	// ..결혼신청
	case MBI_MARRIAGE_MARRIAGE:
		{
			if( we == MBI_YES )
			{
//				g_csMarriageManager.CLI_RequestMarriage(FALSE);
			}
		}
		break;
	// ..결혼신청 받음
	case MBI_MARRIAGE_MARRIAGE_REQUESTED:
		{
			if( we == MBI_YES )
			{
//				g_csMarriageManager.CLI_RequestAcceptMarriage(TRUE);
			}
			else
			{
//				g_csMarriageManager.CLI_RequestAcceptMarriage(FALSE);
			}
		}
		break;
	// ..합의이혼 신청
	case MBI_MARRIAGE_AGREE_DIVORCE:
		{
			if( we == MBI_YES )
			{
//				g_csMarriageManager.CLI_RequestAgreeDivorce();
			}
		}
		break;
	// ..합의이혼 신청 받음
	case MBI_MARRIAGE_AGREE_DIVORCE_REQUESTED:
		{
			if( we == MBI_YES )
			{
//				g_csMarriageManager.CLI_RequestAcceptAgreeDivorce(TRUE);
			}
			else
			{
//				g_csMarriageManager.CLI_RequestAcceptAgreeDivorce(FALSE);
			}
		}
		break;
	// ..강제이혼
	case MBI_MARRIAGE_FORCE_DIVORCE:
		{
			if( we == MBI_YES )
			{
//				g_csMarriageManager.CLI_RequestForceDivorce();
			}
		}
		break;
	// ..텔레포트
	case MBI_MARRIAGE_TELEPORT_TO_PARTNER:
		{
			if( we == MBI_YES )
			{
//				g_csMarriageManager.CLI_RequestTeleportToPartner();
			}
		}
		break;
	// E 결혼 추가 added by hseos 2007.12.11

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2008.01.17
	case MBI_FARM_TAX_PAY_REQUEST:
		{
			if( we == MBI_YES )
			{
				g_csFarmManager.CLI_RequestPayTax(1);
			}
		}
		break;
	// E 농장시스템 추가 added by hseos 2008.01.17
		// 080414 LUJ, 외양 변경 아이템 사용
	case MBI_USEBODYCHANGEITEM:
		{
			cDialog* dialog = WINDOWMGR->GetWindowForID( BODY_CHANGE_DIALOG );

			if( we == MBI_YES &&
				dialog )
			{
				dialog->OnActionEvent( MBI_USEBODYCHANGEITEM, 0, 0 );
			}			
		}
		break;
	// 080507 KTH -- 가축아이템 사용
	case MBI_FARM_ANIMAL_INSTALL :
		{
			CInventoryExDialog* pDlg = NULL ;
			pDlg = GAMEIN->GetInventoryDialog() ;

			if( !pDlg ) return ;

			CItem* pItem = NULL ;
			pItem = pDlg->Get_QuickSelectedItem() ;

			if( !pItem ) return ;

			if( we == MBI_YES )
			{
				g_csFarmManager.CLI_RequestInstallAnimalInAnimalCage(pItem->GetPosition(), pItem->GetItemIdx());
			}

			pItem->SetLock(FALSE) ;
			pDlg->Set_QuickSelectedItem(NULL) ;
		}
		break ;

	case MBI_CR_SECRETCODE_ERR :
 		{
 			if( we != MBI_OK ) return ;
 
 			CChatRoomCreateDlg* pDlg = NULL ;
 			pDlg = GAMEIN->GetChatRoomCreateDlg() ;
 
 			if( !pDlg ) return ;
 
 			cEditBox* pCodeBox = NULL ;
 			pCodeBox = pDlg->GetCodeBox() ;
 
 			if( !pCodeBox ) return ;
 
 			pCodeBox->SetFocus(TRUE) ;
 			// 혹여 채팅창에 포커스가 있으면 ㅡㅡ; 해재 하자.
 		}
 		break ;

	case MBI_CR_ENDCHAT :														// 채팅방 종료 버튼 처리.
		{
			if( we == MBI_YES )
			{
				CChatRoomDlg* pDlg = NULL ;										// 채팅방 정보를 받는다.
				pDlg = GAMEIN->GetChatRoomDlg() ;

				ASSERT(pDlg) ;

				if(!pDlg)
				{
					MessageBox( NULL, "ERR-FRCRD", "GEF-CR_EC", MB_OK) ;
					return ;
				}

				MSG_DWORD msg ;													// 종료 요청 메시지를 보낸다.
				memset(&msg, 0, sizeof(MSG_DWORD)) ;

				msg.Category	= MP_CHATROOM ;
				msg.Protocol	= MP_CHATROOM_OUT_ROOM_SYN ;
				msg.dwObjectID	= HEROID ;

				msg.dwData		= CHATROOMMGR->GetMyRoomOwnerIdx() ;

				NETWORK->Send( &msg, sizeof(MSG_DWORD) ) ;
			}
		}
		break;
	case MBI_PETGRADEUP:
		{
			CInventoryExDialog* pDlg = GAMEIN->GetInventoryDialog();
			if( we == MBI_YES )
			{
				pDlg->PetGradeUp();
			}
			else
			{
				pDlg->SetDisable( FALSE );
			}
		}
		break;
	}
}

void MNM_DlgFunc(LONG lId, void * p, DWORD we)
{
	ySWITCH(lId)
		yCASE(MNM_PARTYDISPLAY_ONOFF)

		yCASE(MNM_TOGGLEFULL)
		GAMEIN->GetMiniMapDialog()->ToggleMinimapMode();
	// 070430 LYW --- GlobalEventFunc : Add MNM_ONOFF.
	yCASE(MNM_ONOFF)
		GAMEIN->GetMiniMapDialog()->SetActive(!GAMEIN->GetMiniMapDialog()->IsActive()) ;
	yENDSWITCH
}

void ITD_DlgFunc(LONG id, void * p, DWORD we)
{
	// send event o CEnchantDialog
	cDialog* dialog = WINDOWMGR->GetWindowForID( ITD_UPGRADEDLG	 );
	ASSERT( dialog );

	dialog->OnActionEvent( id, p, we );
}

void ITMD_DlgFunc(LONG lId, void * p, DWORD we)
{
	// send event to CMixDialog
	cDialog* dialog = WINDOWMGR->GetWindowForID( ITMD_MIXDLG );
	ASSERT( dialog );

	dialog->OnActionEvent( lId, p, we );

}


void ITR_DlgFunc(LONG lId, void * p, DWORD we)
{
	// send event to CReinforceDlg
	cDialog* dialog = WINDOWMGR->GetWindowForID( ITR_REINFORCEDLG );
	ASSERT( dialog );

	dialog->OnActionEvent( lId, p, we );
}


void ApplyOptionDialogFunc( LONG lId, void * p, DWORD we )
{
	cDialog* dialog = WINDOWMGR->GetWindowForID( APPLY_OPTION_DIALOG );
	ASSERT( dialog );

	dialog->OnActionEvent( lId, p, we );
}


void CHA_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	switch(lId)
	{
	case CHA_CONNNECTOK:
		{
			CHARSELECT->GetChannelDialog()->OnConnect();	
		}
		break;
	case CHA_CONNNECTCANCEL:
		{
			CHARSELECT->GetChannelDialog()->SetActive(FALSE);
			CHARSELECT->SetDisablePick(FALSE);
		}
		break;
	}
}

void SL_DlgBtnFunc( LONG lId, void* p, DWORD we )
{
	CServerListDialog* pDlg = (CServerListDialog*)p;
	switch( lId )
	{
	case SL_BTN_CONNECT:
		TITLE->ConnectToServer( pDlg->GetSelectedIndex() );
		break;

	case SL_BTN_EXIT:
		//		MAINGAME->SetGameState( eGAMESTATE_END );
		PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
		break;
	}
}


void MI_DlgFunc(LONG lId, void * p, DWORD we)
{
	// 061220 LYW --- Delete this code.
	/*
	GAMEIN->GetMainInterfaceDialog()->OnActionEvent(lId,p,we);
	*/
}

void WANTNPC_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch(lId)
	{
	case WANTNPC_LISTORDERCOMBOBOX:
		{
			if(we != WE_COMBOBOXSELECT) return;	

			BYTE OrderType = eWan_Date;
			cComboBox * btnOrder = (cComboBox *)WINDOWMGR->GetWindowForIDEx(WANTNPC_LISTORDERCOMBOBOX);

			OrderType = btnOrder->GetCurSelectedIdx();

			GAMEIN->GetWantNpcDialog()->SetOrderType(OrderType);
			WANTEDMGR->SortWantedListSyn(1);
		}
		break;
		/*	case WANTNPC_BUY1BTN: case WANTNPC_BUY2BTN: case WANTNPC_BUY3BTN: case WANTNPC_BUY4BTN:
		{
		DWORD WantIDX = GAMEIN->GetWantNpcDialog()->GetSelectedWantedIDX(lId - WANTNPC_BUY1BTN);
		WANTEDMGR->BuyWantedRightSyn(WantIDX);
		}
		break; */
	case WANTNPC_SEARCHMODEBTN:
		{
			GAMEIN->GetWantNpcDialog()->ShowWantMode(eWantNpcMode_Search);
			SEND_WANTNPCLIST Info;
			memset(&Info, 0, sizeof(SEND_WANTNPCLIST));
			GAMEIN->GetWantNpcDialog()->SetWantedList(&Info);

		}
		break;
	case WANTNPC_BUYNPCBTN: 	case WANTNPC_BUYSEARCHBTN:
		{
			if(GAMEIN->GetWantNpcDialog()->GetCurWantedIdx())
			{
				char name[MAX_NAME_LENGTH+1] = {0,};
				if( GAMEIN->GetWantNpcDialog()->GetCurWantedName() )
				{
					SafeStrCpy( name, GAMEIN->GetWantNpcDialog()->GetCurWantedName(), MAX_NAME_LENGTH+1 );
				}
				char buf[128];
				//				sprintf(buf, CHATMGR->GetChatMsg( 886 ), GAMEIN->GetWantNpcDialog()->GetCurWantedName());
				sprintf(buf, CHATMGR->GetChatMsg( 886 ), name );

				WINDOWMGR->MsgBox( MBI_WANTBUY, MBT_YESNO, buf);
			}
		}
		break;
	case WANTNPC_FRONTLISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD GotoPage = int((CurPage-MAX_WANTNPC_PAGEUNIT)/MAX_WANTNPC_PAGEUNIT)*MAX_WANTNPC_PAGEUNIT +1;
			WANTEDMGR->SortWantedListSyn(GotoPage);
		}
		break;
	case WANTNPC_1LISTBTN: case WANTNPC_2LISTBTN: case WANTNPC_3LISTBTN: case WANTNPC_4LISTBTN: 
	case WANTNPC_5LISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD CurUnit = (CurPage-1) / MAX_WANTNPC_PAGEUNIT; //MAX_WANTNPC_PAGEBTN-2 : front, back¡§Ioo￠®¡×¡E￠c¡E￠c¡§￠® ¡E￠ci¡E￠c¡§I 
			WANTEDMGR->SortWantedListSyn((lId-WANTNPC_1LISTBTN+1) + (CurUnit)*MAX_WANTNPC_PAGEUNIT);
		}
		break;
	case WANTNPC_BACKLISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD GotoPage = int((CurPage-1)/MAX_WANTNPC_PAGEUNIT)*MAX_WANTNPC_PAGEUNIT +1 + MAX_WANTNPC_PAGEUNIT;
			WANTEDMGR->SortWantedListSyn(GotoPage);
		}
		break;
	case WANTNPC_MYFRONTLISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD GotoPage = int((CurPage-MAX_WANTNPC_PAGEUNIT)/MAX_WANTNPC_PAGEUNIT)*MAX_WANTNPC_PAGEUNIT +1;
			WANTEDMGR->MyWantedListSyn(GotoPage);
		}
		break;
	case WANTNPC_MY1LISTBTN: case WANTNPC_MY2LISTBTN: case WANTNPC_MY3LISTBTN: case WANTNPC_MY4LISTBTN: 
	case WANTNPC_MY5LISTBTN:
		{
			WANTEDMGR->MyWantedListSyn(lId-WANTNPC_MY1LISTBTN+1);
		}
		break;
	case WANTNPC_MYBACKLISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD GotoPage = int((CurPage-1)/MAX_WANTNPC_PAGEUNIT)*MAX_WANTNPC_PAGEUNIT +1 + MAX_WANTNPC_PAGEUNIT;
			WANTEDMGR->MyWantedListSyn(GotoPage);
		}
		break;
	case WANTNPC_MYWANTBTN: case WANTNPC_MYWANTSEARCHBTN:
		{
#ifdef _TESTCLIENT_
			GAMEIN->GetWantNpcDialog()->ShowWantMode(eWantNpcMode_My);
#else
			WANTEDMGR->MyWantedListSyn(1);
#endif
		}
		break;
	case WANTNPC_SEARCHFRONTLISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD GotoPage = int((CurPage-MAX_WANTNPC_PAGEUNIT)/MAX_WANTNPC_PAGEUNIT)*MAX_WANTNPC_PAGEUNIT +1;
			cEditBox* pEdit = (cEditBox*)WINDOWMGR->GetWindowForIDEx(WANTNPC_SEARCHEDIT);
			WANTEDMGR->SearchSyn(pEdit->GetEditText(), GotoPage);
		}
		break;
	case WANTNPC_SEARCH1LISTBTN: case WANTNPC_SEARCH2LISTBTN: case WANTNPC_SEARCH3LISTBTN: case WANTNPC_SEARCH4LISTBTN: 
	case WANTNPC_SEARCH5LISTBTN:
		{
			cEditBox* pEdit = (cEditBox*)WINDOWMGR->GetWindowForIDEx(WANTNPC_SEARCHEDIT);
			WANTEDMGR->SearchSyn(pEdit->GetEditText(), lId-WANTNPC_SEARCH1LISTBTN+1);
		}
		break;
	case WANTNPC_SEARCHBACKLISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD GotoPage = int((CurPage-1)/MAX_WANTNPC_PAGEUNIT)*MAX_WANTNPC_PAGEUNIT +1 + MAX_WANTNPC_PAGEUNIT;
			cEditBox* pEdit = (cEditBox*)WINDOWMGR->GetWindowForIDEx(WANTNPC_SEARCHEDIT);

			WANTEDMGR->SearchSyn(pEdit->GetEditText(), GotoPage);
		}
		break;
	case WANTNPC_SEARCHBTN:
		{
			cEditBox* pEdit = (cEditBox*)WINDOWMGR->GetWindowForIDEx(WANTNPC_SEARCHEDIT);
			WANTEDMGR->SearchSyn(pEdit->GetEditText(), 1);
		}
		break;
		/*	case WANTNPC_GIVEUP1BTN: case WANTNPC_GIVEUP2BTN: case WANTNPC_GIVEUP3BTN: case WANTNPC_GIVEUP4BTN: 
		{
		DWORD WantIDX = GAMEIN->GetWantNpcDialog()->GetSelectedWantedIDX(lId - WANTNPC_GIVEUP1BTN);
		WANTEDMGR->GiveUpWantedRightSyn(WantIDX);
		}
		break;	 */
	case WANTNPC_SORTBTN:
		{
#ifdef _TESTCLIENT_
			GAMEIN->GetWantNpcDialog()->ShowWantMode(eWantNpcMode_Npc);
#else
			WANTEDMGR->SortWantedListSyn(1);
#endif
		}
		break;
	case WANTNPC_GIVEUPBTN:
		{
			if(GAMEIN->GetWantNpcDialog()->GetCurWantedIdx())
			{
				char name[MAX_NAME_LENGTH+1] = {0,};
				if( GAMEIN->GetWantNpcDialog()->GetCurWantedName() )
				{
					SafeStrCpy( name, GAMEIN->GetWantNpcDialog()->GetCurWantedName(), MAX_NAME_LENGTH+1 );
				}

				char buf[128];
				//				sprintf(buf, CHATMGR->GetChatMsg( 887 ), GAMEIN->GetWantNpcDialog()->GetCurWantedName());
				sprintf(buf, CHATMGR->GetChatMsg( 887 ), name );
				WINDOWMGR->MsgBox( MBI_WANTGIVEUP, MBT_YESNO, buf);			
			}
		}
		break;
	}
}

void WANTREG_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch(lId)
	{
	case WANTREG_OK:
		{
			CWantRegistDialog* pDlg = (CWantRegistDialog*)p;
			cEditBox* pEdit = (cEditBox*)pDlg->GetWindowForID(WANTREG_PRIZEEDIT);
			char* temp = RemoveComma(pEdit->GetEditText());
			DWORD prize = atoi(temp);
			if((prize < MIN_PRIZE) || (prize > MAX_PRIZE))
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(559) );
				return;
			}			
			WANTEDMGR->RegistKillerSyn(prize);
		}
		break;
	case WANTREG_CANCEL:
		{
			GAMEIN->GetWantRegistDialog()->SetActive(FALSE);
			WANTEDMGR->SetWaiting(eNone);
			WANTEDMGR->SetMurdererID(0);
			//GAMEIN->GetReviveDialog()->SetAlwaysTop(TRUE);
			//GAMEIN->GetReviveDialog()->SetActive(TRUE);

			MSGBASE msg;
			msg.Category = MP_WANTED;
			msg.Protocol = MP_WANTED_REGIST_CANCEL;
			msg.dwObjectID = HEROID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	}
}


void QUE_QuestDlgFunc( LONG lId, void* p, DWORD we )
{
	CQuestDialog* pDlg = (CQuestDialog*)p;

	switch(lId)
	{
		//case QUE_JOURNALBTN:
		//	JOURNALMGR->GetListSyn();
		//	break;
	case QUE_ITEM_GRID:
		//		pDlg->GetPositionForXYRef();
		break;
	case QUE_PAGE1BTN:
		pDlg->RefreshQuestItem(0);
		break;
	case QUE_PAGE2BTN:
		pDlg->RefreshQuestItem(1);
		break;
	case QUE_PAGE3BTN:
		pDlg->RefreshQuestItem(2);
		break;
	case QUE_PAGE4BTN:
		pDlg->RefreshQuestItem(3);
		break;
	case QUE_PAGE5BTN:
		pDlg->RefreshQuestItem(4);
		break;

		// 070125 LYW --- Quest : Modified this line.
	case QUE_PAGE6BTN :
		{
			pDlg->RefreshQuestItem(5);
		}
		break ;

	case QUE_NOTICEBTN :
		{
			DWORD QuestIdx = 0 ;																			// 퀘스트 인덱스를 담을 변수를 선언하고 0으로 세팅한다.

			CQuestDialog* pDlg = NULL ;																		// 퀘스트 다이얼로그 정보를 받을 포인터를 선언하고 null처리를 한다.
			pDlg = GAMEIN->GetQuestTotalDialog()->GetQuestDlg() ;											// 퀘스트 다이얼로그 정보를 받는다.

			if( pDlg )																						// 퀘스트 다이얼로그 정보가 유효하다면,
			{
				QuestIdx = pDlg->GetSelectedQuestID() ;														// 선택 된 퀘스트 인덱스를 받는다.
			}

			if( QuestIdx < 1 )																				// 퀘스트 인덱스가 1보다 작으면,
			{
				//WINDOWMGR->MsgBox( MBI_QUEST_DELETE, MBT_OK, "등록하고자 하는 퀘스트를 선택해 주세요." );	// 알림 메시지를 띄운다.
				WINDOWMGR->MsgBox( MBI_QUEST_DELETE, MBT_OK, CHATMGR->GetChatMsg( 1301 ) );
				return ;																					// 리턴 처리를 한다.
			}
			else
			{
				CQuestQuickViewDialog* pQuickDlg = GAMEIN->GetQuestQuickViewDialog() ;

				if( pQuickDlg )
				{
					pQuickDlg->RegistQuest(QuestIdx) ;

					CQuestDialog* pQuestDlg = GAMEIN->GetQuestDialog() ;

					if(pQuestDlg)
					{
						pQuestDlg->RefreshQuestList() ;	
					}
				}
			}
		}
		break ;

	case QUE_GIVEUPBTN :
		{
			DWORD QuestIdx = 0 ;																			// 퀘스트 인덱스를 담을 변수를 선언하고 0으로 세팅한다.

			CQuestDialog* pDlg = NULL ;																		// 퀘스트 다이얼로그 정보를 받을 포인터를 선언하고 null처리를 한다.
			pDlg = GAMEIN->GetQuestTotalDialog()->GetQuestDlg() ;											// 퀘스트 다이얼로그 정보를 받는다.

			if( pDlg )																						// 퀘스트 다이얼로그 정보가 유효하다면,
			{
				QuestIdx = pDlg->GetSelectedQuestID() ;														// 선택 된 퀘스트 인덱스를 받는다.
			}

			if( QuestIdx < 1 )																				// 퀘스트 인덱스가 1보다 작으면,
			{
				//WINDOWMGR->MsgBox( MBI_QUEST_DELETE, MBT_OK, "포기하고자 하는 퀘스트를 선택해 주세요." );						// 알림 메시지를 띄운다.
				WINDOWMGR->MsgBox( MBI_QUEST_DELETE, MBT_OK, CHATMGR->GetChatMsg( 1302 ) );
				return ;																					// 리턴 처리를 한다.
			}
			else
			{
				//WINDOWMGR->MsgBox( MBI_QUESTGIVEUP, MBT_YESNO, "정말로 선택한 퀘스트를 포기하시겠습니까?" );// 알림 메시지를 띄운다.
				WINDOWMGR->MsgBox( MBI_QUESTGIVEUP, MBT_YESNO, CHATMGR->GetChatMsg( 233 ) );
			}
		}
		break ;
	}
}

void QUE_JournalDlgFunc( LONG lId, void* p, DWORD we )
{
	/*CJournalDialog* pDlg = (CJournalDialog*)p;

	switch(lId)
	{
	case QUE_JOURNALFRONTBTN:
		pDlg->SetBasePage(FALSE);
		break;
	case QUE_JOURNALBACKBTN:
		pDlg->SetBasePage(TRUE);
		break;		
	case QUE_JOURNALPAGE1BTN:
		pDlg->SetPage(0);		
		break;
	case QUE_JOURNALPAGE2BTN:
		pDlg->SetPage(1);		
		break;
	case QUE_JOURNALPAGE3BTN:
		pDlg->SetPage(2);		
		break;
	case QUE_JOURNALPAGE4BTN:
		pDlg->SetPage(3);		
		break;
	case QUE_JOURNALPAGE5BTN:
		pDlg->SetPage(4);		
		break;
	case QUE_JOURNALCHECKBOX1BTN:
		pDlg->SetItemCheck(0);
		break;
	case QUE_JOURNALCHECKBOX2BTN:
		pDlg->SetItemCheck(1);
		break;
	case QUE_JOURNALCHECKBOX3BTN:
		pDlg->SetItemCheck(2);
		break;
	case QUE_JOURNALCHECKBOX4BTN:
		pDlg->SetItemCheck(3);
		break;
	case QUE_JOURNALCHECKBOX5BTN:
		pDlg->SetItemCheck(4);
		break;
	case QUE_JOURNALSAVE:
		if(pDlg->IsSavedJournal())
			pDlg->SelectedJournalDelete();
		else
			pDlg->SelectedJournalSave();
		break;
	case QUE_JOURNALSAVEDLIST:
		pDlg->ViewJournalListToggle();
		break;
	}*/
}


void BAIL_BailDlgFunc( LONG lId, void* p, DWORD we )
{
	CBailDialog* pDlg = (CBailDialog*)p;

	switch(lId)
	{
	case BAIL_OK:
		pDlg->SetFame();
		break;
	case BAIL_CANCEL:
		pDlg->Close();
		break;
	}
}

//수련장 skill practice
void MP_RegistDlgFunc( LONG id, void* p, DWORD we )
{
	/*
	switch(lId)
	{
	case MP_ROKBTN:
		{
			if(PETMGR->GetCurSummonPet())
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1277));
				return;
			}
			GAMEIN->GetMPRegistDialog()->SetDisable(TRUE);
			GAMEIN->GetInventoryDialog()->SetDisable(TRUE);
			GAMEIN->GetSkillSuryunDialog()->SetDisable(TRUE);
			SURYUNMGR->GoSuryunMapSyn();
		}
		break;
	case MP_RCANCELBTN:
		{
			GAMEIN->GetMPRegistDialog()->SetActive(FALSE);
		}
	}	*/
}


void GDRANK_DlgFunc( LONG id, void* p, DWORD we )
{
	// send event to CGuildRankDialog
	cDialog* dialog = WINDOWMGR->GetWindowForID( GDR_RANKDLG );
	ASSERT( dialog );

	dialog->OnActionEvent( id, p, we );
}


void GDLEVEL_DlgFunc( LONG id, void* p, DWORD we )
{
	// send event to CGuildLevelUpDialog
	cDialog* dialog = WINDOWMGR->GetWindowForID( GD_LEVELUPDLG );
	ASSERT( dialog );

	dialog->OnActionEvent( id, p, we );
}


void GDMARK_DlgFunc( LONG id, void* p, DWORD we )
{
	// send event to CGuildMarkDialog
	cDialog* dialog = WINDOWMGR->GetWindowForID( GDM_MARKREGISTDLG );
	ASSERT( dialog );

	dialog->OnActionEvent( id, p, we );
}


void GD_WarehouseFunc( LONG id, void* p, DWORD we )
{
	// send event to CGuildWarehouseDialog
	cDialog* dialog = WINDOWMGR->GetWindowForID( GDW_WAREHOUSEDLG );
	ASSERT( dialog );

	dialog->OnActionEvent( id, p, we );
}


void GDCREATE_DlgFunc(LONG id, void* p, DWORD we )
{
	// send event to CGuildCreateDialog
	cDialog* dialog = WINDOWMGR->GetWindowForID( GD_CREATEDLG );
	ASSERT( dialog );

	dialog->OnActionEvent( id, p, we );
}


void GDINVITE_DlgFunc( LONG id, void* p, DWORD we )
{
	// send event to CGuildInviteDialog
	cDialog* dialog = WINDOWMGR->GetWindowForID( GD_INVITEDlG );
	ASSERT( dialog );

	dialog->OnActionEvent( id, p, we );
}


void GDWHRANK_DlgFunc( LONG id, void* p, DWORD we )
{
	// send event to CGuildWarehouseRankDialog
	cDialog* dialog = WINDOWMGR->GetWindowForID( GDWHR_RANKDLG );
	ASSERT( dialog );

	dialog->OnActionEvent( id, p, we );
}


void GDREVIVAL_DlgFunc( LONG id, void* p, DWORD we )
{
	// send evenCGuildRevivalDialog
	cDialog* dialog = WINDOWMGR->GetWindowForID( GD_REVIVALDLG );
	ASSERT( dialog );

	dialog->OnActionEvent( id, p, we );
}


void GDUnion_DlgFunc( LONG id, void* p, DWORD we )
{
	// send event to CGuildUnionCreateDialog
	cDialog* dialog = WINDOWMGR->GetWindowForID( GDU_CREATEDLG );
	ASSERT( dialog );

	dialog->OnActionEvent( id, p, we );
}


void GDNICK_DlgFunc( LONG id, void* p, DWORD we )
{
	// send event to CGuildUnionCreateDialog
	cDialog* dialog = WINDOWMGR->GetWindowForID( GD_NICKNAMEDLG );
	ASSERT( dialog );

	dialog->OnActionEvent( id, p, we );
}


void GD_DlgFunc( LONG id, void* p, DWORD we )
{
	// send event to cGuildDialog
	cDialog* dialog = WINDOWMGR->GetWindowForID( GD_GUILDDLG );
	ASSERT( dialog );

	dialog->OnActionEvent( id, p, we );
}


void FD_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch(lId)
	{
		///////////////////////////////////////////////////
		//create
	case FD_CCREATEOKBTN:
		{
			if(HERO->GetFamilyIdx())
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(543));
				return;
			}
			cEditBox * pFamilyName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(FD_CNAME);
			if(strcmp(pFamilyName->GetEditText(), "") == 0)
				return;
			cTextArea * pIntro = (cTextArea *)WINDOWMGR->GetWindowForIDEx(FD_CINTROTEXT);
			char FullText[100];
			pIntro->GetScriptText(FullText);
			int len = strlen(FullText);
			if(len > MAX_GUILD_INTRO)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(544));
				return;
			}
			// desc_hseos_패밀리01
			// S 패밀리 추가 added by hseos 2007.07.04
			// FAMILYMGR->CreateFamilySyn(pFamilyName->GetEditText(), FullText);
			g_csFamilyManager.CLI_RequestCreateFamily(pFamilyName->GetEditText());
			// E 패밀리 추가 added by hseos 2007.07.04
		}
		break;
		///////////////////////////////////////////////////
		//invite
	case FD_IOKBTN:
		{
			// 070801 웅주, 기존 패밀리 제거하며 주석처리

			//// 070330 LYW --- GlobalEventFunc : Add for test.
			//FAMILYMGR->SetInvitedKind(AsMember) ;
			//if( FAMILYMGR->GetInvitedKind() == AsMember )
			//{
			//	FAMILYMGR->AddMemberAccept();
			//}
			//else if( FAMILYMGR->GetInvitedKind() == AsStudent )
			//{
			//	FAMILYMGR->AddStudentAccept();
			//}
		}
		break;
	case FD_ICANCELBTN:
		{
			// 070801 웅주, 기존 패밀리 제거하며 주석처리

			//FAMILYMGR->AddMemberDeny();
		}
		break;
		///////////////////////////////////////////////////
		//guild info / func 
	case FD_MEMBERINFOPUSHUPBTN:
		{
			//GAMEIN->GetFamilyDlg()->ShowFamilyMode(eFamilyMode_FamilyInfo);
		}
		break;
	case FD_GUILFUNCPUSHUPBTN:
		{
			//GAMEIN->GetFamilyDlg()->ShowFamilyMode(eFamilyMode_Func);
		}
		break;
	case FD_BANMEMBER:
		{
			// 070801 웅주, 기존 패밀리 제거하며 주석처리

			//if(HERO->GetFamilyMemberRank() < FAMILY_MASTER)
			//	return;

			//if((GUILDMGR->GetSelectedMemberID() == 0) || (GUILDMGR->GetSelectedMemberID() == HEROID))
			//{
			//	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(545));
			//}
			//else
			//{
			//	char msg[128];
			//	sprintf(msg, CHATMGR->GetChatMsg( 546 ), FAMILYMGR->GetSelectedMemberName());
			//	WINDOWMGR->MsgBox( MBI_FAMILY_BAN, MBT_YESNO, msg);
			//	FAMILYMGR->SetActiveFunc(TRUE);
			//}				
		}
		break;
	case FD_INVITEMEMBER:
		{
			/*
			CObject * targetObj = OBJECTMGR->GetSelectedObject();
			if(targetObj)
			{
				if(targetObj->GetObjectKind()  == eObjectKind_Player)
				{
					if(((CPlayer*)targetObj)->GetFamilyIdx())
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(38));
						return;
					}
					else
						GUILDMGR->AddMemberSyn(targetObj->GetID());
				}
			}*/
				CObject * targetObj = OBJECTMGR->GetSelectedObject();
			if(targetObj)
			{
				if(targetObj->GetObjectKind()  == eObjectKind_Player)
				{
					if(((CPlayer*)targetObj)->GetFamilyIdx())
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(547));
						return;
					}
					else
						GAMEIN->GetFamilyJoinDialog()->SetActive(TRUE);
				}
			}
		}
		break;
	case FD_GIVEMEMBERNICK:
		{	
			GAMEIN->GetFamilyNickNameDlg()->SetActive(TRUE);
		}
		break;
	case FD_GIVEMEMBERRANK:
		{
			if(HERO->GetFamilyMemberRank() < GUILD_VICEMASTER)
				return;
			GAMEIN->GetFamilyRankDlg()->SetActive(TRUE);
		}
		break;
	case FD_SECEDE:
		{
			if(HERO->GetFamilyIdx() == 0)
				return;
			if(( FAMILY_STUDENT <= HERO->GetFamilyMemberRank() ) && (HERO->GetFamilyMemberRank() <= FAMILY_VICEMASTER) )
				WINDOWMGR->MsgBox( MBI_FAMILY_SECEDE, MBT_YESNO, CHATMGR->GetChatMsg( 467 ) );
		}
		break;
		// 06. 03. 문파공지 - 이영준
	case FD_NOTICE:
		{
			//CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(710));
			//			GAMEIN->GetFamilyNoticeDlg()->SetActive(TRUE);			
		}
		break;
	case FD_INVITEUNION:
		{
			// 070801 웅주, 기존 패밀리 제거하며 주석처리

			//CObject* targetObj = OBJECTMGR->GetSelectedObject();
			//if( targetObj )
			//{
			//	if( targetObj->GetObjectKind() == eObjectKind_Player )
			//	{
			//		FAMILYUNION->SendInviteFamilyUnion( (CPlayer*)targetObj );
			//	}
			//}
		}
		break;			
	case FD_SECEDEUNION:
		{
			// 070801 웅주, 기존 패밀리 제거하며 주석처리

			//FAMILYUNION->SendSecedeFamilyUnion();
		}
		break;
		/*	case GD_REMOVEUNION:
		{
		GAMEIN->GetGFWarInfoDlg()->ShowFamilyUnion();
		}
		break;
		case GD_DECLARE:
		{
		if( GUILDWARMGR->CheckFamilyFieldWarCondition( 0 ) )
		GAMEIN->GetGFWarDeclareDlg()->SetActive( TRUE );
		}
		break;
		case GD_SUGGEST:
		{
		if( GUILDWARMGR->CheckFamilyFieldWarCondition( 1 ) )
		GAMEIN->GetGFWarInfoDlg()->ShowSuggest();
		}
		break;
		case GD_SURRENDER:
		{
		if( GUILDWARMGR->CheckFamilyFieldWarCondition( 2 ) )
		GAMEIN->GetGFWarInfoDlg()->ShowSurrend();
		}
		break;
		*/
	case FD_POSITION://직위
		{
			//GAMEIN->GetFamilyDlg()->SortByPosition();//Add 060803 by wonju
		}
		break;
	case FD_RANK:    //레벨
		{
			//GAMEIN->GetFamilyDlg()->SortByLevel();//Add 060803 by wonju
		}
		break;
	case FD_PLUSTIME: //플러스 타임
		{
			//			GAMEIN->GetFamilyPlusTimeDlg()->SetActive( TRUE ); //Add 060803 by wonju
		}
		break;

	case FD_RELATIONBTN: //동맹관계
		{
			//			GAMEIN->GetFamilyWarInfoDlg()->SetActive( TRUE );
		}
		break;


		///////////////////////////////////////////////////
		//mark
	case FDM_REGISTOKBTN:
		{
			cEditBox* pMarkName = (cEditBox*)WINDOWMGR->GetWindowForIDEx(FDM_NAMEEDIT);
			if(strcmp(pMarkName->GetEditText(), "") == 0)
				return;

			BOOL bResult = g_csFamilyManager.CLI_RequestRegistEmblem(pMarkName->GetEditText());
			if (bResult)
			{
				GAMEIN->GetFamilyMarkDlg()->SetActive(FALSE);			
			}
			else
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(549));
			}
		}
		break;

	case FUM_REGISTOKBTN:
		{
			// 070801 웅주, 기존 패밀리 제거하며 주석처리

			//cEditBox* pMarkName = (cEditBox*)WINDOWMGR->GetWindowForIDEx( FDM_NAMEEDIT );
			//if( strcmp(pMarkName->GetEditText(), "") == 0 )	return;
			//if( FAMILYUNION->CheckFamilyUnionCondition( 5 ) )
			//{
			//	if( !FAMILYUNIONMARKMGR->RegistFamilyUnionMark( g_nServerSetNum, HERO->GetFamilyUnionIdx(), 
			//		HERO->GetFamilyUnionMarkIdx(), pMarkName->GetEditText() ) )
			//		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(549));
			//	GAMEIN->GetFamilyMarkDlg()->SetActive(FALSE);			
			//}
		}
		break;

		///////////////////////////////////////////////////
		//levelup
	case FD_LUOKBTN:
		{
			// 070801 웅주, 기존 패밀리 제거하며 주석처리

			//FAMILYMGR->LevelUpSyn();
		}
		break;

		///////////////////////////////////////////////////
		//nick name 
	case FD_NREGISTOKBTN:
		{
			/*if(FAMILYMGR->GetSelectedMemberID())
			{
				cEditBox* pName = (cEditBox*)WINDOWMGR->GetWindowForIDEx(FD_NICKNAMEEDIT);
				if(strcmp(pName->GetEditText(), "") == 0)
					return;
				if(strlen(pName->GetEditText()) > MAX_FAMILY_NICKNAME)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(550));
					return;
				}
				if((  FILTERTABLE->IsUsableName(pName->GetEditText(), FALSE ) == FALSE ) || (FILTERTABLE->IsCharInString(pName->GetEditText(), " '") == TRUE))
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(551));
					return;
				}
				FAMILYMGR->GiveNickNameSyn(pName->GetEditText());
				GAMEIN->GetFamilyNickNameDlg()->SetActive(FALSE);
			}
			else
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(545));*/

				CFamilyDialog* pDlg = GAMEIN->GetFamilyDlg() ;													// 패밀리 다이얼로그 정보를 받아온다.

				if( pDlg )																						// 다이얼로그 정보가 유효한지 체크한다.
				{
					int nMemberIdx = pDlg->GetSelectedMemberIdx() ;												// 선택 된 멤버 인덱스를 받는다.

					if( nMemberIdx >= 0 )																		// 멤버 인덱스가 유효한지 체크한다.
					{
						CSHFamily* pFamily = HERO->GetFamily() ;												// 패밀리 정보를 받는다.

						if( pFamily )																			// 패밀리 정보가 유효한지 체크한다.
						{
							CSHFamilyMember* pMember = pFamily->GetMember( nMemberIdx ) ;						// 받은 인덱스로 멤버 정보를 받는다.

							if( pMember )																		// 멤버 정보가 유효한지 체크한다.
							{
								char nameBuf[32] = {0, } ;														// 호칭을 담을 임시 버퍼를 선언한다.

								DWORD dwMemberID = pMember->Get()->nID ;										// 멤버의 아이디를 받는다.

								CFamilyNickNameDialog* pChangeDlg = GAMEIN->GetFamilyNickNameDlg() ;			// 호칭 변경창 정보를 받는다.

								if( pChangeDlg )																// 변경창 정보가 유효한지 체크한다.
								{
									cEditBox* pEditBox = pChangeDlg->GetEditBox() ;								// 호칭 변경창의 에디트 박스 정보를 받아온다.

									if( pEditBox )																// 에디트 박스의 정보가 유효한지 체크한다.
									{
										strcpy(nameBuf, pEditBox->GetEditText()) ;								// 변경 된 호칭을 복사한다.

										if( strlen(nameBuf) <=1 )												// 이름 길이를 확인한다.
										{
											//WINDOWMGR->MsgBox( MBI_FAMILY_CHANGENAME_FAIL, MBT_OK, "호칭명을 바르게 입력해 주세요." );	// 알림창을 띄운다.
											WINDOWMGR->MsgBox( MBI_FAMILY_CHANGENAME_FAIL, MBT_OK, CHATMGR->GetChatMsg( 1303) );
											return ;															// 리턴 처리를 한다.
										}
									}

									g_csFamilyManager.CLI_RequestChangeNickname(dwMemberID, nameBuf) ;			// 호칭 변경을 신청한다.

									pChangeDlg->SetActive(FALSE) ;												// 호칭 변경 창을 닫는다.

									return ;
								}
							}
						}
					}
				}
		}
		break;

		///////////////////////////////////////////////////
		//rank
	case FDR_OKBTN:
		{
			// 070801 웅주, 기존 패밀리 제거하며 주석처리

			//GAMEIN->GetFamilyRankDlg()->SetActive(FALSE);
			//BYTE Rank=0;
			//cComboBox* pRankCombo = (cComboBox*)WINDOWMGR->GetWindowForIDEx(FDR_RANKCOMBOBOX);
			//char buff[16];
			//strcpy(buff,pRankCombo->GetComboText());
			//
			//CMD_ST(buff)
			//CMD_CS( CHATMGR->GetChatMsg(552) )
			//Rank = FAMILY_MEMBER;
			//CMD_CS( CHATMGR->GetChatMsg(553) )
			//Rank = FAMILY_SENIOR;
			//// 070622 LYW --- GlobalEventFunc : 메시지 번호 수정.
			//CMD_CS( CHATMGR->GetChatMsg(219) )
			//Rank = FAMILY_VICEMASTER;
			//CMD_EN
			//
			//FAMILYMGR->ChangeMemberRank(Rank);
		}
		break;
	case FDR_DOKBTN:
		{
			// 070801 웅주, 기존 패밀리 제거하며 주석처리

			//GAMEIN->GetFamilyRankDlg()->SetActive(FALSE);
			//BYTE DRank;
			//cComboBox* pDRankCombo = (cComboBox*)WINDOWMGR->GetWindowForIDEx(FDR_DRANKCOMBOBOX);
			//char buff[16];
			//strcpy(buff,pDRankCombo->GetComboText());
			//
			//CMD_ST(buff)
			//CMD_CS( CHATMGR->GetChatMsg(556) )
			//DRank = FAMILY_MEMBER;
			//CMD_CS( CHATMGR->GetChatMsg(555) )
			//DRank = FAMILY_VICEMASTER;
			//CMD_EN			
			//GUILDMGR->ChangeMemberRank(DRank);
		}
		break;
	case FDW_WAREHOUSE1BTN: case FDW_WAREHOUSE2BTN:
		{
			GAMEIN->GetFamilyWarehouseDlg()->OnActionEvent(lId, p, we);
		}
		break;
	case FDW_PUTINMONEYBTN:
		{
			GAMEIN->GetFamilyWarehouseDlg()->PutInOutMoney(1);
		}
		break;
	case FDW_PUTOUTMONEYBTN:
		{
			GAMEIN->GetFamilyWarehouseDlg()->PutInOutMoney(0);
		}
		break;

		// guild union create
	case FDU_OKBTN:
		{
			// 070801 웅주, 기존 패밀리 제거하며 주석처리

			//cEditBox* pFamilyUnionName = (cEditBox*)WINDOWMGR->GetWindowForIDEx(FDU_NAME);
			//if( strcmp(pFamilyUnionName->GetEditText(), "") == 0 )	return;
			//if( strlen(pFamilyUnionName->GetEditText()) > MAX_GUILD_NAME )	return;
			//FAMILYUNION->SendCreateFamilyUnion( pFamilyUnionName->GetEditText() );
		}
		break;


	default:
		{
			int a = 0;
		}
		break;
	}
}

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23
void FARM_BUY_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch(lId)
	{
	case FARM_BUY_OKBTN:
		{
			g_csFarmManager.CLI_RequestBuyFarmToSrv();
		}
		break;
	case FARM_BUY_CANCELBTN:
		{
			GAMEIN->GetFarmBuyDlg()->SetActive(FALSE);
		}
		break;
	}
}

void FARM_UPGRADE_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch(lId)
	{
	case FARM_UPGRADE_OKBTN:
		{
			g_csFarmManager.CLI_RequestUpgradeFarm((CSHFarmZone::FARM_ZONE)GAMEIN->GetFarmUpgradeDlg()->GetFarmZone(),
													GAMEIN->GetFarmUpgradeDlg()->GetFarmID(),
													GAMEIN->GetFarmUpgradeDlg()->GetFarmUpgradeKind());
		}
		break;
	case FARM_UPGRADE_CANCELBTN:
		{
			GAMEIN->GetFarmUpgradeDlg()->SetActive(FALSE);
		}
		break;
	}
}

void FARM_MANAGE_DlgFunc( LONG lId, void* p, DWORD we )
{
	GAMEIN->GetFarmManageDlg()->OnActionEvent(lId, p, we);
}
// E 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23

// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2008.01.24
void DATE_ZONE_LIST_DlgFunc( LONG lId, void* p, DWORD we )
{
	GAMEIN->GetDateZoneListDlg()->OnActionEvent(lId, p, we);
}

void CHALLENGE_ZONE_LIST_DlgFunc( LONG lId, void* p, DWORD we )
{
	GAMEIN->GetChallengeZoneListDlg()->OnActionEvent(lId, p, we);
}

void CHALLENGE_ZONE_CLEARNO1_DlgFunc( LONG lId, void* p, DWORD we )
{
	GAMEIN->GetChallengeZoneClearNo1Dlg()->OnActionEvent(lId, p, we);
}
// E 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2008.01.24

// 아이템몰 창고
void ITMALL_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
//	if(lId == ITMALL_BTN1 || lId == ITMALL_BTN2 || lId == ITMALL_BTN3)
//		GAMEIN->GetItemShopDialog()->TabChange(lId-ITMALL_BTN1);
}


// 이동창
void SA_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	if( lId == SA_MOVETABBTN1 )
	{
		GAMEIN->GetMoveDialog()->SetButton( 0 );
		return;
	}
	if( lId == SA_MOVETABBTN2 )
	{
		GAMEIN->GetMoveDialog()->SetButton( 1 );
		return;
	}

	if( !GAMEIN->GetMoveDialog()->IsTownMove() && !GAMEIN->GetMoveDialog()->GetSelectedDBIdx() )
		return;

	switch(lId)
	{
	case SA_CHANGNAME:
		GAMEIN->GetPointSaveDialog()->SetDialogStatus( FALSE );			
		GAMEIN->GetPointSaveDialog()->SetActive( TRUE );
		break;
	case SA_MOVEOK:		
		if( GAMEIN->GetMoveDialog()->IsTownMove() )
			WINDOWMGR->MsgBox( MBI_SAVEDPOINTMOVE, MBT_YESNO, CHATMGR->GetChatMsg(781) );
		else
			WINDOWMGR->MsgBox( MBI_SAVEDPOINTMOVE, MBT_YESNO, CHATMGR->GetChatMsg(759) );
		break;
	case SA_MOVEDEL:
		WINDOWMGR->MsgBox( MBI_SAVEDPOINTDEL, MBT_YESNO, CHATMGR->GetChatMsg(760) );
		break;
	case CMI_CLOSEBTN:
		GAMEIN->GetMoveDialog()->SetItemToMapServer( 0, 0, FALSE );
		break;
	}
}


// 저장위치
// 061212 LYW --- Delete preexistence chatting dialog.
/*
extern void CHA_DlgFunc(LONG lId, void * p, DWORD we)
{
	switch(lId)
	{
	case CHA_NAME_OKBTN:
		GAMEIN->GetPointSaveDialog()->ChangePointName();
		break;
	case CHA_NAME_CANCELBTN:
		GAMEIN->GetPointSaveDialog()->CancelPointName();
		break;
	}
}
*/
// guildfieldwar
void GFW_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch( lId )
	{
		// declare
	case GFW_DECLARE_OKBTN:
		{
			cEditBox* pName = (cEditBox*)WINDOWMGR->GetWindowForIDEx( GFW_DECLARE_EDITBOX );
			cEditBox* pMoney = (cEditBox*)WINDOWMGR->GetWindowForIDEx( GFW_MONEY_EDITBOX );
			GUILDWARMGR->SendDeclare( pName->GetEditText(), RemoveComma(pMoney->GetEditText()) );
		}
		break;
	case GFW_DECLARE_CANCELBTN:
		{
			GAMEIN->GetGFWarDeclareDlg()->SetActive( FALSE );
		}
		break;
	case GFW_MONEY_CHK:
		{
			GAMEIN->GetGFWarDeclareDlg()->ShowMoneyEdit();
		}
		break;

		// result
	case GFW_RESULT_OKBTN:
		{
			if( GUILDWARMGR->IsValid( 4 ) )
			{
				GAMEIN->GetGFWarResultDlg()->SetDisable( TRUE );
				WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_DECLARE_ACCEPT, MBT_YESNO, CHATMGR->GetChatMsg( 957 ) );
			}
		}
		break;
	case GFW_RESULT_CANCELBTN:
		{
			GAMEIN->GetGFWarResultDlg()->SetDisable( TRUE );
			WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_DECLARE_DENY, MBT_YESNO, CHATMGR->GetChatMsg( 958 ) );
		}
		break;
	case GFW_RESULT_CONFIRMBTN:
		{
			GAMEIN->GetGFWarResultDlg()->SetActive( FALSE );
		}
		break;
	// 종전 제안
	case GFW_INFO_SUGGEST_OKBTN:
		{
			CGFWarInfoDlg* dialog = GAMEIN->GetGFWarInfoDlg();
			ASSERT( dialog );

			const GUILDINFO* pInfo = GUILDWARMGR->GetEnemyFromListIndex( GAMEIN->GetGFWarInfoDlg()->GetSelectedListIdx() );

			if( ! pInfo )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 895 ) );
				break;
			}
			else if( 	pInfo->UnionIdx &&
					!	pInfo->mIsUnionMaster )
			{
				const MSG_GUILD_LIST::Data* data = GUILDWARMGR->GetUnionMasterEnemy( pInfo->UnionIdx );

				if( data )
				{
					//CHATMGR->AddMsg( CTC_SYSMSG, "동맹 리더인 %s 길드에게 제의하십시오", data->GuildName );
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1270 ), data->GuildName );
				}

				break;
			}

			WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_SUGGEST, MBT_YESNO, CHATMGR->GetChatMsg( 983 ), pInfo->GuildName );
			dialog->SetDisable( TRUE );
		}
		break;
	// 항복 제안
	case GFW_INFO_SURREND_OKBTN:
		{
			CGFWarInfoDlg* dialog = GAMEIN->GetGFWarInfoDlg();
			ASSERT( dialog );

			const GUILDINFO* pInfo = GUILDWARMGR->GetEnemyFromListIndex( GAMEIN->GetGFWarInfoDlg()->GetSelectedListIdx() );

			if( ! pInfo )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 894 ) );
				break;
			}
			else if( 	pInfo->UnionIdx &&
					!	pInfo->mIsUnionMaster )
			{
				const MSG_GUILD_LIST::Data* data = GUILDWARMGR->GetUnionMasterEnemy( pInfo->UnionIdx );

				if( data )
				{
					//CHATMGR->AddMsg( CTC_SYSMSG, "동맹 리더인 %s 길드에게 제의하십시오", data->GuildName );
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1270 ), data->GuildName );
				}
				
				break;
			}

			WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_SURREND, MBT_YESNO, CHATMGR->GetChatMsg( 984 ), pInfo->GuildName );
			dialog->SetDisable( TRUE );
		}
		break;
	case GFW_GDU_REMOVE_OKBTN:	// guildunion remove
		{
			const char* pName = GUILDUNION->GetNameFromListIndex( GAMEIN->GetGFWarInfoDlg()->GetSelectedListIdx() );
			if( pName )
			{
				WINDOWMGR->MsgBox( MBI_UNION_REMOVE, MBT_YESNO, CHATMGR->GetChatMsg( 985 ), pName );
			}
			// 080128 LYW --- GlobalEventFunc : [MP회복] 출력 삭제.
			/*else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 847 ) );
			}*/
		}
		break;
	case GFW_INFO_CANCELBTN:
		{
			GAMEIN->GetGFWarInfoDlg()->SetActive( FALSE );
		}
		break;

		// warinfo
	case GW_INFOCLOSEBTN:
		{
			GAMEIN->GetGuildWarInfoDlg()->SetActive( FALSE );
		}
		break;
	}
}


// SkillPoint Redistribute
extern void SK_DlgBtnFunc( LONG lId, void* p, DWORD we )
{

}

void PW_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch( lId )
	{
	case PW_CB_MEMBER0:	case PW_CB_MEMBER1:	case PW_CB_MEMBER2:	case PW_CB_MEMBER3:	case PW_CB_MEMBER4:
	case PW_CB_MEMBER5:	case PW_CB_MEMBER6:	case PW_CB_MEMBER7:	case PW_CB_MEMBER8:	case PW_CB_MEMBER9:
	case PW_CB_MEMBER10:	case PW_CB_MEMBER11:	case PW_CB_MEMBER12:	case PW_CB_MEMBER13:
		{
			PARTYWAR->SendPartyWarMember( lId - PW_CB_MEMBER0 );
		}
		break;
	case PW_BTN_LOCK:	PARTYWAR->SendPartyWarLock( TRUE );		break;
	case PW_BTN_UNLOCK:	PARTYWAR->SendPartyWarLock( FALSE );	break;
	case PW_BTN_START:	PARTYWAR->SendPartyWarStart();			break;
	case PW_BTN_CANCEL:	PARTYWAR->SendPartyWarCancel();			break;
	}

}

void CHS_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch( lId )
	{
	case CHASE_SEEOKBTN:
		GAMEIN->GetChaseDlg()->SetActive( FALSE );
		break;
	case CHASE_OKBTN:
		GAMEIN->GetChaseinputDlg()->WantedChaseSyn();
		break;
	case CHASE_CANCELBTN:
		GAMEIN->GetChaseinputDlg()->SetActive( FALSE );
		break;
	}
}


void CHAN_DlgFunc(LONG lId, void * p, DWORD we)
{
	switch( lId )
	{
	case CH_NAME_CHANGE_OKBTN:
		{
			GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
			GAMEIN->GetNameChangeDlg()->NameChangeSyn();
		}		
		break;
	case CH_NAME_CHANGE_CANCELBTN:
		{
			GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
			GAMEIN->GetNameChangeDlg()->SetItemDBIdx( 0 );
			GAMEIN->GetNameChangeDlg()->SetActive( FALSE );
		}
		break;
	case CH_NAME_CHANGE_STARTBTN:
		{
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetCurSelectedItem(SHOPIVN);
			if( pItem )
			{
				GAMEIN->GetNameChangeDlg()->SetItemDBIdx( pItem->GetDBIdx() );
				GAMEIN->GetNameChangeDlg()->SetActive( TRUE );
				GAMEIN->GetNameChangeNotifyDlg()->SetActive( FALSE );
			}
		}
		break;
	}
}


void GDT_DlgFunc(LONG lId, void * p, DWORD we)
{
	switch( lId )
	{
	case GDT_ENTRYBTN:
		{
			WINDOWMGR->MsgBox( MBI_GTREGIST_SYN, MBT_YESNO, CHATMGR->GetChatMsg(960) );
		}
		break;
	case GDT_CANCELBTN:
		{
			WINDOWMGR->MsgBox( MBI_GTREGISTCANCEL_SYN, MBT_YESNO, CHATMGR->GetChatMsg(964) );
		}
		break;
	case GDT_WARVIEWBTN:
		{
			if( GAMEIN->GetGTBattleListDlg() )
			{
				if( GAMEIN->GetGTBattleListDlg()->GetSelectedBattle() > -1 )
					WINDOWMGR->MsgBox( MBI_GTBATTLELIST_OBSERVER, MBT_YESNO, CHATMGR->GetChatMsg(956) );
				else
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(957) );
			}
		}
		break;
	case GDT_VIEWCANCELBTN:
		{
			if( GAMEIN->GetGTBattleListDlg() )
			{
				if( GAMEIN->GetGTBattleListDlg()->IsActive() )
					GAMEIN->GetGTBattleListDlg()->SetActive( FALSE );
			}
		}
		break;
	case GDT_SEMIFINAL_PUSHUPBTN:
		{
			if( GAMEIN->GetGTStandingDlg() )			
				GAMEIN->GetGTStandingDlg()->SetCurPage( 0 );
		}
		break;
	case GDT_AGROUP_PUSHUPBTN:
		{
			if( GAMEIN->GetGTStandingDlg() )			
				GAMEIN->GetGTStandingDlg()->SetCurPage( 1 );
		}
		break;
	case GDT_BGROUP_PUSHUPBTN:
		{
			if( GAMEIN->GetGTStandingDlg() )			
				GAMEIN->GetGTStandingDlg()->SetCurPage( 2 );
		}
		break;
	case GDT_CGROUP_PUSHUPBTN:
		{
			if( GAMEIN->GetGTStandingDlg() )			
				GAMEIN->GetGTStandingDlg()->SetCurPage( 3 );
		}
		break;
	case GDT_DGROUP_PUSHUPBTN:
		{
			if( GAMEIN->GetGTStandingDlg() )			
				GAMEIN->GetGTStandingDlg()->SetCurPage( 4 );
		}
		break;
	case GDT_OUTBTN:
		{
			WINDOWMGR->MsgBox( MBI_GTSCORE_EXIT, MBT_YESNO, CHATMGR->GetChatMsg(969) );
		}
		break;
	}
}

void SW_DlgFunc(LONG lId, void* p, DWORD we)
{
	switch( lId )
	{
	case SW_TIMEREG_OKBTN:
		{
			cEditBox* pYear = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_TIME_YEAR);
			cEditBox* pMon = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_TIME_MON);
			cEditBox* pDay = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_TIME_DAY);
			cEditBox* pHour = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_TIME_HOUR);
			if( !pYear || !pMon || !pDay || !pHour )
			{
				GAMEIN->GetSWTimeRegDlg()->SetActive( FALSE );
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1120) );
				return;
			}
			if( (strcmp(pYear->GetEditText(), "") == 0) || (strcmp(pMon->GetEditText(), "") == 0) ||
				(strcmp(pDay->GetEditText(), "") == 0) || (strcmp(pHour->GetEditText(), "") == 0) )
			{
				GAMEIN->GetSWTimeRegDlg()->SetActive( FALSE );
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1120) );
				return;
			}

			DWORD dwYear = (DWORD)atoi( pYear->GetEditText() );
			DWORD dwMon = (DWORD)atoi( pMon->GetEditText() );
			DWORD dwDay = (DWORD)atoi( pDay->GetEditText() );
			DWORD dwHour = (DWORD)atoi( pHour->GetEditText() );

			if( (dwYear<2005) || (dwMon>12||dwMon<=0) || (dwDay>31||dwDay<=0) || (dwHour>23||dwHour<0) )
			{
				GAMEIN->GetSWTimeRegDlg()->SetActive( FALSE );
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1120) );
				return;
			}
			if( dwYear%4 == 0 )
				if( dwDay > DayOfMonth_Yundal[dwMon] )	return;
				else
					if( dwDay > DayOfMonth[dwMon] )			return;

			stTIME time;
			time.SetTime( dwYear - 2000, dwMon, dwDay, dwHour, 0, 0 );

			MSG_DWORD msg;
			msg.Category = MP_SIEGEWAR;
			msg.Protocol = MP_SIEGEWAR_REGISTTIME_SYN;
			msg.dwObjectID = HEROID;
			msg.dwData = time.value;
			NETWORK->Send( &msg, sizeof(msg) );

			GAMEIN->GetSWTimeRegDlg()->SetActive( FALSE );
		}
		break;
	case SW_PROTECTREG_OKBTN:
		{
			int nIdx = GAMEIN->GetSWProtectRegDlg()->GetSelectedListIdx();
			if( nIdx > -1 )
			{
				WINDOWMGR->MsgBox( MBI_SW_PROTECTREG_OK, MBT_YESNO, CHATMGR->GetChatMsg(1121), GAMEIN->GetSWProtectRegDlg()->GetSelectedGuildName( nIdx ) );
				GAMEIN->GetSWProtectRegDlg()->SetDisable( TRUE );
			}
			else
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1118) );				
		}
		break;
	case SW_INFOCLOSEBTN:
		{
			GAMEIN->GetSWInfoDlg()->SetActive( FALSE );
		}
		break;
	case SW_TAXCHANGE_OKBTN:
		{
			cEditBox* pTax = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_CHANGE_TAX);
			if( pTax )
			{
				if( SWPROFIT->CheckSWProfit( 0, atoi(pTax->GetEditText()), 0 ) )
				{
					WINDOWMGR->MsgBox( MBI_SW_TAXCHANGE_OK, MBT_YESNO, CHATMGR->GetChatMsg(1029), atoi(pTax->GetEditText()) );
					GAMEIN->GetSWProfitDlg()->SetDisable( TRUE );
				}
			}
		}
		break;
	case SW_PROFIT_OKBTN:
		{
			cEditBox* pMoney = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_OUT_MONEY);
			if( pMoney )
			{
				if( SWPROFIT->CheckSWProfit( 1, 0, atoi(pMoney->GetEditText()) ) )
				{
					WINDOWMGR->MsgBox( MBI_SW_PROFIT_OK, MBT_YESNO, CHATMGR->GetChatMsg(1030), atoi(pMoney->GetEditText()) );
					GAMEIN->GetSWProfitDlg()->SetDisable( TRUE );
				}
			}
		}
		break;
	case SW_PROFIT_CANCELBTN:
		{
			GAMEIN->GetSWProfitDlg()->SetActive( FALSE );
		}
		break;
	case SW_ENGRAVECANCEL:
		{
			if( GAMEIN->GetSWEngraveDlg() )
				GAMEIN->GetSWEngraveDlg()->CancelEngraveSyn();
		}
		break;
	case SW_STARTTIME_OKBTN:
		{
			GAMEIN->GetSWStartTimeDlg()->SetActive( FALSE );
		}
		break;
	}
}

void IT_DlgFunc(LONG lId, void* p, DWORD we)
{
	CSealDialog* pDlg = (CSealDialog*)p;

	switch( lId )
	{
	case IT_LOCKOKBTN:
		pDlg->ItemSealSyn();
		break;
	case IT_LOCKCANCERBTN:
		pDlg->ReleaseItem();
		break;
	}
}


void CJOB_DlgFunc(LONG lId, void* p, DWORD we)
{
	CChangeJobDialog* pDlg = (CChangeJobDialog*)p;

	switch( lId )
	{
	case CJOB_OKBTN:
		pDlg->ChangeJobSyn();
		break;
	case CJOB_CANCELBTN:
		pDlg->CancelChangeJob();
		break;		
	}
}

void CG_DlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetGuageDlg()->OnActionEvent(lId, p, we);
}


//void RFDefault_DlgFunc(LONG lId, void *p, DWORD we)
//{
//	CReinforceResetDlg* pDlg = (CReinforceResetDlg*)p;
//
//	switch( lId )
//	{
//	case RF_DefaultOKBTN:
//		pDlg->ItemResetSyn();
//		break;
//	case RF_DefaultCANCERBTN:
//		pDlg->ReleaseItem();
//		break;
//	}
//}

//void RareCreate_DlgFunc(LONG lId, void *p, DWORD we)
//{
//    CRareCreateDialog* pDlg = (CRareCreateDialog*)p;
//
//	switch( lId )
//	{
//	case RareCreate_OKBTN:
//		pDlg->ItemRareCreateSyn();
//		break;
//	case RareCreate_CANCERBTN:
//		pDlg->ReleaseItem();
//		break;
//	}
//}

// 06. 02. 강화 재료 안내 인터페이스 추가 - 이영준
void RFGUIDE_DLGFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetReinforceGuideDlg()->OnActionEvent(lId, p, we);
}

// 06. 02. 내정보창 팁보기 추가 - 이영준
void TB_DlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetTipBrowserDlg()->OnActionEvent(lId, p, we);
}

void TB_STATE_DlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetTipBrowserDlg()->OnActionEvent(lId, p, we);
}

void GN_DlgBtnFunc(LONG id, void *p, DWORD we)
{
	//GAMEIN->GetGuildNoteDlg()->OnActionEvnet( id, p, we);

	// send event to CGuildNoteDlg
	cDialog* dialog = WINDOWMGR->GetWindowForID( GUILDNOTE_DLG );
	ASSERT( dialog );

	dialog->OnActionEvent( id, p, we );
}

void AN_DlgBtnFunc(LONG id, void *p, DWORD we)
{
	//GAMEIN->GetUnionNoteDlg()->OnActionEvnet(id, p, we);

	// send event to CUnionNoteDlg
	cDialog* dialog = WINDOWMGR->GetWindowForID( ALLYNOTE_DLG );
	ASSERT( dialog );

	dialog->OnActionEvent( id, p, we );
}

// 06. 03. 문파공지 - 이영준
void GNotice_DlgBtnFunc(LONG id, void *p, DWORD we)
{
	//GAMEIN->GetGuildNoticeDlg()->OnActionEvnet(id, p, we);

	// send to event CGuildNoticeDlg
	cDialog* dialog = WINDOWMGR->GetWindowForID( GUILDNOTICE_DLG );
	ASSERT( dialog );

	dialog->OnActionEvent( id, p, we );
}

void GuildPlusTime_DlgFunc(LONG id, void* p, DWORD we)
{
	// GAMEIN->GetGuildPlusTimeDlg()->OnActionEvnet(id,p,we);

	cDialog* dialog = WINDOWMGR->GetWindowForID( GP_TIME );
	ASSERT( dialog );

	dialog->OnActionEvent( id, p, we );
}

// 061127 LYW --- Add callback function for new chatting system.
void Chatting_DlgFunc( LONG lId, void* p, DWORD we )
{
	/*if(lId == CHAT_BTN_CHATROOM)
	{
		if(we == WE_LBTNCLICK)
		{
			CHATROOMMGR->ToggleChatRoomMainDlg() ;
		}
	}*/
	//GAMEIN->GetChattingDlg()->OnActionEvent( lId, p, we ) ;
}

// 061215 LYW --- Add function to call callback function.
void CM_CMNCbFunc( LONG lId, void* p, DWORD we)
{
	CHARMAKEMGR->GetCharMakeNewDlg()->OnActionEvent( lId, p, we ) ;
	//cCharMakeNewDlg* pDlg = ( cCharMakeNewDlg* )WINDOWMGR->GetWindowForID( CM_MAKEDLG ) ;
	//pDlg->OnActionEvent( lId, p, we ) ;
}

// 061219 LYW --- Add callback function to processing event from controls of main system dialog.
void MDS_CBFunc( LONG lId, void* p, DWORD we )
{
	GAMEIN->GetMainSystemDlg()->OnActionEvent( lId, p, we ) ;
}

//// 070109 LYW --- Add callback function to processing event from controls of target dialog.
//void Target_BtnFunc( LONG lId, void* p, DWORD we )
//{
//	GAMEIN->GetMonsterGuageDlg()->OnActionEvent( lId, p, we ) ;
//}

// 070110 LYW --- Add callback function to processing event from controls of character main dialog.
void CharMain_BtnFunc( LONG lId, void* p, DWORD we )
{
	GAMEIN->GetGuageDlg()->OnActionEvent( lId, p, we ) ;
}

void JO_DlgFunc(LONG lId, void* p, DWORD we)
{
	cDialog* dialog = WINDOWMGR->GetWindowForID( JOIN_OPTIONDLG );
	ASSERT( dialog );

	dialog->OnActionEvent( lId, p, we );
}


// Date Matching Dialog Procedure
// 2007/03/15	이진영

void DMD_DateMatchingDlgFunc(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetDateMatchingDlg()->OnActionEvent(lId, p, we);
	_asm nop;
}

// 070607 LYW --- GlovalEventFunc : Add function to call callback function of partner idalog.
void DMD_PartnerDlgFunc( LONG lId, void* p, DWORD we )
{
	GAMEIN->GetDateMatchingDlg()->GetPartnerListDlg()->OnActionEvent(lId, p, we) ;
}

// 070606 LYW --- GlovalEventFunc : Add function to call callback function of identification dialog.
void DMD_IDT_Func(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetIdentificationDlg()->OnActionEvent(lId, p, we) ;
	_asm nop ;
}

// 070607 LYW --- GlovalEventFunc : Add function to call callback function of favor icon dialog.
void DMD_FID_Func(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetFavorIconDlg()->OnActionEvent(lId, p, we) ;
	_asm nop ;
}

// 070607 LYW --- GlovalEventFunc : Add function to call callback function of my info dialog.
void DMD_MID_Func(LONG lId, void* p, DWORD we)
{
	CDateMatchingInfoDlg* pDlg = GAMEIN->GetMatchMyInfoDlg() ;

	if( !pDlg ) return ;

	switch(lId)
	{
		/*case DMD_MY_GOOD_FEELING_ICONGRID :
		{
		int a = 0 ;
		}
		break ;

		case DMD_MY_BAD_FEELING_ICONGRID :
		{
		int a = 0 ;
		}
		break ;

		case DMD_MY_INTRODUCTION_EDIT :
		{
		int a = 0 ;
		}
		break ;*/

	case DMD_CHANGE_MY_INTRODUCTION_BTN :
	{
		cTextArea* pTextArea = pDlg->GetIntroduce() ;
		if( pTextArea )
		{
			pTextArea->SetReadOnly(FALSE) ; 
			pTextArea->SetFocusEdit(TRUE) ;
		}
	}
	break ;

	/*case DMD_MY_INTRODUCE_UP :
	{
		int a = 0 ;
	}
	break ;

	case DMD_MY_INTRODECE_DOWN :
		{
			int a = 0 ;
		}
		break ;

	case DMD_MY_INTRODECE_GUAGE :
		{
			int a = 0 ;
		}
		break ;*/

	case DMD_SAVE_MY_INTRODUCTION_BTN :
		{
			cTextArea* pTextArea = pDlg->GetIntroduce() ;

			if( pTextArea )
			{
				// check text.
				char tempBuf[512] = {0, } ;
				pTextArea->GetScriptText(tempBuf) ;

				////if( strlen(tempBuf) <= 0 )
				//{
				//	 notice error.
				//	WINDOWMGR->MsgBox( MBI_INVALID_INTRODUCE, MBT_OK, "소개를 다시 작성해 주세요." ) ;
				//	return ;
				//}
				//else
				//{
				//	g_csResidentRegistManager.CLI_RequestUpdateIntroduction(HERO->GetID(), tempBuf) ;
				//}
				g_csResidentRegistManager.CLI_RequestUpdateIntroduction(tempBuf) ;
			}
		}
		break ;

	case 1876 :
		{
			switch(we)
			{
			case WE_BTNCLICK :
				{
					int a = 0 ;

					// check up button.
					// setting up line.

					// check down button.
					// setting down line.

					// check guage.
					// setting line.
				}
				break ;
			}
		}
		break ;

	default :
		break ;
	}
}

// 070607 LYW --- GlovalEventFunc : Add function to call callback function of partner info dialog.
void DMD_PID_Func(LONG lId, void* p, DWORD we)
{
	switch(lId)
	{
		/*case DMD_PARTNER_GOOD_FEELING_ICONGRID :
		{
		int a = 0 ;
		}
		break ;

		case DMD_PARTNER_BAD_FEELING_ICONGRID :
		{
		int a = 0 ;
		}
		break ;

		case DMD_PARTNER_INTRODUCTION_EDIT :
		{
		int a= 0 ;
		}
		break ;*/

	case DMD_REGIST_PARTNER_BTN :
	{
		//WINDOWMGR->MsgBox( MBI_FRIEND_WAIT, MBT_CANCEL, "상대방에게 친구 추가를 요청했습니다. 수락할 때까지 잠시만 기다려 주세요." );			

		// invite partner to friend.
	}
	break ;

	case DMD_RECOMMEND_PARTNER_BTN :
		{
			//WINDOWMGR->MsgBox( MBI_FRIEND_RECOMMEND, MBT_YESNO, "아이디가 여덟글자 님을 추천하시겠습니까? 추천은 하루에 3번만 가능합니다." );			
		}
		break ;

	default :
		break ;
	}
}

// 070607 LYW --- GlovalEventFunc : Add function to call callback function of record info dialog.
void DMD_RID_Func(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetMatchRecordDlg()->OnActionEvent(lId, p, we) ;
}

// 070618 LYW --- GlovalEventFunc : Add function to call callback function of key setting dialog.
void KS_Func(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetKeySettingTipDlg()->OnActionEvent(lId, p, we) ;
}

void SkillTrainDlgFunc( LONG lId, void* p, DWORD we )
{
	GAMEIN->GetSkillTrainingDlg()->OnActionEvent(lId, p, we);
}

// desc_hseos_패밀리01
// S 패밀리 추가 added by hseos 2007.07.02
void FD_GuildDlg(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetFamilyDlg()->OnActionEvent(lId, p, we);
}
// E 패밀리 추가 added by hseos 2007.07.02


void PROGRESS_DlgFunc( LONG id, void* p, DWORD we )
{
	cDialog* dialog = WINDOWMGR->GetWindowForID( PROGRESS_DIALOG );
	ASSERT( dialog );

	dialog->OnActionEvent( id, p, we );
}


// 071018 LYW --- GlovalEventFunc : Add event function for tutorial dialog.
void TTR_DlgFunc( LONG id, void* p, DWORD we )
{
	cDialog* dialog = WINDOWMGR->GetWindowForID( TUTORIAL_DLG );
	ASSERT( dialog );

	if( we != WE_BTNCLICK ) return ;

	dialog->OnActionEvent( id, p, we );
}


// 071023 LYW --- GlovalEventFunc : Add event function for tutorial button dialog.
void TTR_BTNDlgFunc( LONG id, void* p, DWORD we )
{
	cDialog* dialog = WINDOWMGR->GetWindowForID( TUTORIALBTN_DLG ) ;
	ASSERT( dialog ) ;

	if( we != WE_BTNCLICK ) return ;

	dialog->OnActionEvent( id, p, we ) ;
}

void ITM_BTNDlgFunc( LONG id, void* p, DWORD we )
{
	cDialog* dialog = WINDOWMGR->GetWindowForID( ITEMMALLBTN_DLG ) ;
	ASSERT( dialog ) ;

	if( we != WE_BTNCLICK ) return ;

	dialog->OnActionEvent( id, p, we ) ;
}


// 071024 LYW --- GlovalEventFunc : Add event function for helper dialog.
void HD_DlgFunc(LONG lId, void* p, DWORD we )
{
	cDialog* dialog = WINDOWMGR->GetWindowForID( HELP_DLG ) ;
	ASSERT( dialog ) ;

	if( we & WE_CHECKED )													// 체크 박스 체크 이벤트가 발생했다면,
	{
		if( lId != HD_SHOWTUTORIAL ) return ;								// 체크 박스 아이디와 일치하지 않으면, return 처리를 한다.

		TUTORIALMGR->Set_ActiveTutorial(TRUE) ;								// 튜토리얼을 활성화 상태로 세팅한다.
		TUTORIALMGR->Get_TutorialBtnDlg()->SetActive(TRUE) ;				// 튜토리얼 버튼을 활성화 한다.

		OPTIONMGR->Set_ShowTutorial(TRUE) ;									// 튜토리얼 보이기를 true로 세팅한다.

		/*COptionDialog* pDlg = NULL ;
		pDlg = GAMEIN->GetOptionDialog() ;

		if( pDlg )
		{
			pDlg->UpdateData( TRUE );
		}

		OPTIONMGR->ApplySettings();*/
		OPTIONMGR->SaveGameOption();
		//OPTIONMGR->SendOptionMsg();	
	}
	else if( we & WE_NOTCHECKED )											// 체크 박스 체크 풀기 이벤트가 발생했다면,
	{
		if( lId != HD_SHOWTUTORIAL ) return ;								// 체크 박스 아이디와 일치하지 않으면, return 처리를 한다.

		TUTORIALMGR->Set_ActiveTutorial(FALSE) ;							// 튜토리얼을 비활성화 상태로 세팅한다.
		TUTORIALMGR->Get_TutorialBtnDlg()->SetActive(FALSE) ;				// 튜토리얼 버튼을 비활성화 한다.

		OPTIONMGR->Set_ShowTutorial(FALSE) ;								// 튜토리얼 보이기를 false로 세팅한다.

		/*COptionDialog* pDlg = NULL ;
		pDlg = GAMEIN->GetOptionDialog() ;

		if( pDlg )
		{
			pDlg->UpdateData( TRUE );
		}

		OPTIONMGR->ApplySettings();*/
		OPTIONMGR->SaveGameOption();
		//OPTIONMGR->SendOptionMsg();	
	}

	if( we & WE_BTNCLICK ) 
	{
		dialog->OnActionEvent( lId, p, we ) ;
	}
}





// 071130 LYW --- GlovalEventFunc : Add event function for map move dialog.
void MapMove_DlgFunc(LONG lId, void* p, DWORD we )
{
	cDialog* dialog = WINDOWMGR->GetWindowForID( MAPMOVE_DLG ) ;
	ASSERT( dialog ) ;

	if( we & WE_BTNCLICK ) 
	{
		dialog->OnActionEvent( lId, p, we ) ;
	}
}





//=========================================================================
//	NAME : Change_Name_Func
//	DESC : Add event function for change name dialog. 071228
//=========================================================================
void Change_Name_Func(LONG lId, void* p, DWORD we )
{
	cDialog* dialog = WINDOWMGR->GetWindowForID( CHANGENAME_DLG ) ;
	ASSERT( dialog ) ;

	if( we & WE_BTNCLICK ) 
	{
		dialog->OnActionEvent( lId, p, we ) ;
	}
}


//=========================================================================
//	NAME : NumberPad_DlgFunc
//	DESC : Add event function for number pad dialog. 080109 LYW
//=========================================================================
void NumberPad_DlgFunc(LONG lId, void* p, DWORD we )
{
	cDialog* pDlg = NULL ;
	pDlg = WINDOWMGR->GetWindowForID( NUMBERPAD_DLG ) ; //NUMBERPAD_DLG

	ASSERT( pDlg ) ;

	if( we & WE_BTNCLICK )
	{
		pDlg->OnActionEvent( lId, p, we ) ;
	}
}


//---KES AUTONOTE
void AutoNoteDlg_Func( LONG lId, void* p, DWORD we )
{
	if( GAMEIN->GetAutoNoteDlg() )
		GAMEIN->GetAutoNoteDlg()->OnActionEvent(lId, p, we);
}

void AutoAnswerDlg_Func( LONG lId, void* p, DWORD we )
{
	if( GAMEIN->GetAutoAnswerDlg() )
		GAMEIN->GetAutoAnswerDlg()->OnActionEvent(lId, p, we);
}
//---------------

//---KES 상점검색
void StoreSearchDlg_Func( LONG lId, void* p, DWORD we )
{
	if( GAMEIN->GetStoreSearchDlg() )
		GAMEIN->GetStoreSearchDlg()->OnActionEvent(lId, p, we);
}
//---------------

// 080414 LUJ, 외양 변경 창
void BodyChangeFunc( LONG lId, void* p, DWORD we )
{
	cDialog* dialog = WINDOWMGR->GetWindowForID( BODY_CHANGE_DIALOG );

	if( dialog )
	{
		dialog->OnActionEvent( lId, p, we );
	}
}


// 080403 LYW --- GlovalEventFunc : Add event functions for chatroom system.
void ChatRoomMainDlg_Func(LONG lId, void* p, DWORD we )
{
    ASSERT(p) ;

	if(!p)
	{
		MessageBox( NULL, "ERR-IMP", "CRMDF", MB_OK) ;
		return ;
	}

	CChatRoomMainDlg* pDlg = NULL ;
 	pDlg = GAMEIN->GetChatRoomMainDlg() ;

	ASSERT(pDlg) ;

 	if(!pDlg)
 	{
 		MessageBox(	NULL, "ERR-FRD", "CRMDF", MB_OK) ;
 		return ;
 	}
 
 	pDlg->OnActionEvent(lId, p, we) ;
 }

//=========================================================================
//	NAME : ChatRoomCreateDlg_Func - 080312 : LYW
//	DESC : The functino to pass event to create dialog.
//=========================================================================
void ChatRoomCreateDlg_Func(LONG lId, void* p, DWORD we )
{
	ASSERT(p) ;

	if(!p)
	{
		MessageBox( NULL, "ERR-IMP", "CRCDF", MB_OK) ;
		return ;
	}

	CChatRoomCreateDlg* pDlg = NULL ;
	pDlg = GAMEIN->GetChatRoomCreateDlg() ;											// Receive create dialog.

	ASSERT(pDlg) ;

 	if(!pDlg)
 	{
 		MessageBox(	NULL, "ERR-FRD", "CRCDF", MB_OK) ;
 		return ;
 	}

	pDlg->OnActionEvent(lId, p, we) ;												// Pass event to create dialog.
}





//=========================================================================
//	NAME : ChatRoomDlg_Func - 080312 : LYW
//	DESC : The functino to pass event to chatting dialog.
//=========================================================================
void ChatRoomDlg_Func(LONG lId, void* p, DWORD we )
{
	ASSERT(p) ;

	if(!p)
	{
		MessageBox( NULL, "ERR-IMP", "CRDF", MB_OK) ;
		return ;
	}

	CChatRoomDlg* pDlg = NULL ;
	pDlg = GAMEIN->GetChatRoomDlg() ;												// Receive chatring dialog.

	ASSERT(pDlg) ;

 	if(!pDlg)
 	{
 		MessageBox(	NULL, "ERR-FRD", "CRDF", MB_OK) ;
 		return ;
 	}

	pDlg->OnActionEvent(lId, p, we) ;												// Pass event to chatring dialog.
}





//=========================================================================
//	NAME : ChatRoomGuestListDlg_Func - 080312 : LYW
//	DESC : The functino to pass event to guest list dialog.
//=========================================================================
void ChatRoomGuestListDlg_Func(LONG lId, void* p, DWORD we ) 
{
	ASSERT(p) ;

	if(!p)
	{
		MessageBox( NULL, "ERR-IMP", "CRGLDF", MB_OK) ;
		return ;
	}

	CChatRoomGuestListDlg* pDlg = NULL ;
	pDlg = GAMEIN->GetChatRoomGuestListDlg() ;											// Receive guestList dialog.

	ASSERT(pDlg) ;

 	if(!pDlg)
 	{
 		MessageBox(	NULL, "ERR-FRD", "CRGLDF", MB_OK) ;
 		return ;
 	}

	pDlg->OnActionEvent(lId, p, we) ;												// Pass event to guestList dialog.
}





//=========================================================================
//	NAME : ChatRoomJoinDlg_Func - 080312 : LYW
//	DESC : The functino to pass event to join dialog.
//=========================================================================
void ChatRoomJoinDlg_Func(LONG lId, void* p, DWORD we ) 
{
	ASSERT(p) ;

	if(!p)
	{
		MessageBox( NULL, "ERR-IMP", "CRJDF", MB_OK) ;
		return ;
	}

	CChatRoomJoinDlg* pDlg = NULL ;
	pDlg = GAMEIN->GetChatRoomJoinDlg() ;											// Receive join dialog.

	ASSERT(pDlg) ;

 	if(!pDlg)
 	{
 		MessageBox(	NULL, "ERR-FRD", "CRJDF", MB_OK) ;
 		return ;
 	}

	pDlg->OnActionEvent(lId, p, we) ;												// Pass event to join dialog.
}





//=========================================================================
//	NAME : ChatRoomOptionDlg_Func - 080312 : LYW
//	DESC : The functino to pass event to option dialog.
//=========================================================================
void ChatRoomOptionDlg_Func(LONG lId, void* p, DWORD we ) 
{
	ASSERT(p) ;

	if(!p)
	{
		MessageBox( NULL, "ERR-IMP", "CRODF", MB_OK) ;
		return ;
	}

	CChatRoomOptionDlg* pDlg = NULL ;
	pDlg = GAMEIN->GetChatRoomOptionDlg() ;											// Receive option dialog.

	ASSERT(pDlg) ;

 	if(!pDlg)
 	{
 		MessageBox(	NULL, "ERR-FRD", "CRODF", MB_OK) ;
 		return ;
 	}

	pDlg->OnActionEvent(lId, p, we) ;												// Pass event to option dialog.
}

//void ChatRoomLobbyDlg_Func(LONG lId, void* p, DWORD we )
//{
//	ASSERT(p) ;
//
//	if(!p)
//	{
//		MessageBox( NULL, "ERR-IMP", "CRLDF", MB_OK) ;
//		return ;
//	}
//
//	CChatRoomLobbyDlg* pDlg = NULL ;
//	pDlg = GAMEIN->GetChatRoomLobbyDlg() ;											// Receive option dialog.
//
//	ASSERT(pDlg) ;
//
// 	if(!pDlg)
// 	{
// 		MessageBox(	NULL, "ERR-FRD", "CRLDF", MB_OK) ;
// 		return ;
// 	}
//
//	pDlg->OnActionEvent(lId, p, we) ;												// Pass event to option dialog.
//}

void Animal_DlgBtnFunc(LONG lId, void* p, DWORD we )
{
	ASSERT(p);

	if(!p)
	{
		MessageBox(NULL, "ERR-IMP", "CRLDF", MB_OK);
		return;
	}
	CAnimalDialog* pDlg = NULL;
	pDlg = GAMEIN->GetAnimalDialog();

	ASSERT(pDlg);

	if(!pDlg)
	{
		MessageBox( NULL, "ERR-FRD", "CRLDF", MB_OK );
		return;
	}

	GAMEIN->GetAnimalDialog()->OnActionEvent( lId, p, we );
	//pDlg->OnActionEvent( lId, p, we);
}

void PET_STATUS_DLG_FUNC(LONG lId, void* p, DWORD we )
{
	GAMEIN->GetPetStateDlg()->OnActionEvent(lId, p, we);
}

void PET_RES_DLG_FUNC(LONG lId, void* p, DWORD we )
{
	GAMEIN->GetPetResurrectionDlg()->OnActionEvent(lId, p, we);
}

