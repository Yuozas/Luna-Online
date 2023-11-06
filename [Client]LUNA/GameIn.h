#pragma once


#include "GameState.h"
#include "PartyDialog.h"

class CInventoryExDialog;
class CExchangeDialog;
class CMixDialog;
class CDealDialog;
class CCharacterDialog;
// 061204 LYW --- Delete Preexistence Chatting Dialog.
/*
class CChatDialog;
*/

class cJackpotDialog;

// 061220 LYW --- Delete this code.
//class CMainBarDialog;
class CDebugDlg;

// LYJ 051017 구입노점상 추가
class CStallKindSelectDlg;
class CStreetBuyStall;
class CBuyRegDialog;

class CStreetStall;
class cNpcScriptDialog;

class CChatOptionDialog;
class COptionDialog;
class cDialog;
class CCharStateDialog;
class CMenuSlotDialog;

class cAuctionBoard;
class CMoneyDlg;
class cQuickDlg;
class CExitDialog;

// desc_hseos_몬스터미터_01
// S 몬스터미터 추가 added by hseos 2007.04.09
class CSHMonstermeterDlg;
// E 몬스터미터 추가 added by hseos 2007.04.09

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23	
class CSHFarmBuyDlg;
class CSHFarmUpgradeDlg;
class CSHFarmManageDlg;
// E 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23

// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2008.01.24
class CSHDateZoneListDlg;
class CSHChallengeZoneListDlg;
class CSHChallengeZoneClearNo1Dlg;
// E 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2008.01.24

class CGuildCreateDialog;
class CGuildDialog;
class CGuildInviteDialog;
class CGuildMarkDialog;
class CGuildLevelUpDialog;
class CGuildNickNameDialog;
class CGuildRankDialog;
class CGuildWarehouseDialog;
class CGuildUnionCreateDialog;
class CMiniMapDlg;
class CStorageDialog;
class CNoteDialog;
class CFriendDialog;
class CMiniNoteDialog;
class CMiniFriendDialog;
class CReviveDialog;
class CMonsterGuageDlg;
class CReinforceDlg;
class CWantNpcDialog;
class CWantRegistDialog;
class CQuestTotalDialog;
class CQuestDialog;
class CQuestQuickViewDialog;
class CGameNotifyDlg ;
class CWantedDialog;
class CJournalDialog;
class CPartyCreateDlg;
class CPartyInviteDlg;
class CBailDialog;
class CDissolveDialog;
class CEventNotifyDialog;
class CItemShopDialog;
class CMoveDialog;
class CPointSaveDialog;

class CMHMap;
class CMHCamera;

class CGFWarDeclareDlg;
class CGFWarResultDlg;
class CGFWarInfoDlg;
class CGuildWarInfoDlg;
class CGuildPlusTimeDlg;

// 070619 LYW --- Deactive SkillPointRedist.
// 현재 사용하지 않아서 비활성화.
//class CSkillPointRedist;
class CSkillPointNotify;

class CMallNoticeDialog;

class CPartyWarDialog;

class CShoutDialog;
class CFishingDialog;
class CFishingGaugeDialog;
class CFishingPointDialog;

class CWeatherDialog;
// 061204 LYW --- Delete Preexistence Chatting Dialog.
/*
class CShoutchatDialog;
*/
class CChaseDialog;
class CChaseinputDialog;
class CNameChangeDialog;
class CNameChangeNotifyDlg;

class CBigMapDlg;

class CGTRegistDialog;
class CGTRegistcancelDialog;
class CGTStandingDialog;
class CGTBattleListDialog;
class CGTScoreInfoDialog;

class CSWTimeRegDlg;
class CSWProtectRegDlg;
class CSWInfoDlg;

class CSealDialog;
class CSWProfitDlg;
class CSWEngraveDialog;
class CSWTimeDialog;

class CChangeJobDialog;

class CSWStartTimeDialog;
class CGuageDialog;
//class CReinforceResetDlg;
//class CRareCreateDialog;

// 06. 02. 강화 재료 안내 인터페이스 추가 - 이영준
class CReinforceGuideDialog;
// 06. 02. 내정보창 팁보기 추가 - 이영준
class CTipBrowserDlg;
class CKeySettingTipDlg;

class CGuildNoteDlg;
class CUnionNoteDlg;

class CGuildNoticeDlg;
class CSkillOptionChangeDlg;
class CSkillOptionClearDlg;

class CGuildJoinDialog;

class CScreenShotDlg;

class cSkillTreeDlg;
// 061127 LYW --- Add New Chatting class.
class CChattingDlg ;
// 061130 LYW --- Add New Outside Chatting class.
class COSChattingDlg ;
// 061219 LYW --- Include main system dialog's class.
class CMainSystemDlg ;

class cNpcImageDlg;

class CDateMatchingDlg;			// 데이트 매칭 다이얼로그.	2007/03/26 - 이진영
// 070605 LYW --- GameIn : Include class of identification part.
class CIdentification ;
class CFavorIcon ;
class CDateMatchingInfoDlg ;
class CDateMatchingRecordDlg ;
class CFamilyCreateDialog;
class CFamilyDialog;
class CFamilyInviteDialog;
class CFamilyMarkDialog;
class CFamilyLevelUpDialog;
class CFamilyNickNameDialog;
class CFamilyRankDialog;
class CFamilyWarehouseDialog;
class CFamilyUnionCreateDialog;
class CFamilyPlusTimeDlg;
class CFamilyNoteDlg;
class CFamilyNoticeDlg;
class CFamilyJoinDialog;

class cSkillTrainingDlg;

// 071017 LYW --- GameIn : Add class tutorial dialog.
class cTutorialDlg ;

// 071017 LYW --- GameIn : Add class help dialog.
class cHelperDlg ;

// 071023 LYW --- GameIn : Add class tutorial button dialog.
class cTutorialBtnDlg ;
class cItemMallBtnDlg ;
class cItemPopupDlg ;

// 071201 LYW --- GameIn : Add class mapmove dialog.
class cMapMoveDialog ;

// 071227 LYW --- GameIn : 캐릭터 이름 변경 추가.
class cChangeNameDialog ;

// 080109 LYW --- cWindowManager : 숫자 패드 다이얼로그 추가.
class cNumberPadDialog ;

//---KES AUTONOTE
class CAutoNoteDlg;
class CAutoAnswerDlg;
//--------------
//---KES 상점검색
class CStoreSearchDlg;
//---------------

//---BROWSER
class CItemShopDlg;

// 080403 LYW --- GameIn : Declare interfaces for chatroom system.
class CChatRoomMainDlg ;
class CChatRoomCreateDlg ;
class CChatRoomDlg ;
class CChatRoomGuestListDlg ;
class CChatRoomOptionDlg ;
class CChatRoomJoinDlg ;
//class CChatRoomLobbyDlg ;

// 080509 KTH --
class CAnimalDialog;

class CPetInfoDialog;
class CPetStateDialog;
class CPetWearedDialog;
class CPetInvenDialog;
class CPetResurrectionDialog;


#define GAMEIN USINGTON(CGameIn)

class CGameIn : public CGameState  
{
	///////////////////// Dialog Pointer //////////////////////////////////
	cSkillTreeDlg*		m_pSkillTreeDlg;

	CInventoryExDialog	* m_pInventoryDlg;
	CExchangeDialog		* m_pExchangeDlg;
//	CRecvExchangeDialog	* m_pRecvExchangeDlg;
	CMixDialog			* m_pMixDlg;
	CDealDialog			* m_pDealDlg;

	CCharacterDialog	* m_pCharDlg;
	// 061204 LYW --- Delete Preexistence Chatting Dialog.
	/*
	CChatDialog			* m_pChatDlg;
	*/

	// SW050715
//	cJackpotDialog		* m_pJackpotDlg;
	
	// 061220 LYW --- Delete this code.
	//CMainBarDialog			* m_pMainDlg;
	CDebugDlg			* m_pDebugDlg;

// LYJ 051017 구입노점상 추가
	CStallKindSelectDlg * m_pStallKindSelectDlg;
	CStreetBuyStall		* m_pStreetBuyStallDlg;
	CBuyRegDialog		* m_pBuyRegDlg;
	
// LBS 03.10.16
	CStreetStall		* m_pStreetStallDlg;
	cNpcScriptDialog	* m_pNpcScriptDlg;

//KES
//	CMacroModeDialog	* m_pMacroModeDlg;
	CChatOptionDialog	* m_pChatOptionDlg;
	COptionDialog		* m_pOptionDlg;
	CExitDialog			* m_pExitDlg;

	// desc_hseos_몬스터미터_01
	// S 몬스터미터 추가 added by hseos 2007.04.09
	CSHMonstermeterDlg	*m_pMonstermeterDlg;
	// E 몬스터미터 추가 added by hseos 2007.04.09

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23
	CSHFarmBuyDlg		*m_pFarmBuyDlg;
	CSHFarmUpgradeDlg	*m_pFarmUpgradeDlg;
	CSHFarmManageDlg	*m_pFarmManageDlg;
	// E 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2008.01.24
	CSHDateZoneListDlg			*m_pDateZoneListDlg;
	CSHChallengeZoneListDlg		*m_pChallengeZoneListDlg;
	CSHChallengeZoneClearNo1Dlg	*m_pChallengeZoneClearNo1Dlg;
	// E 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2008.01.24

	CCharStateDialog	* m_pCharStateDlg;
	CMenuSlotDialog		* m_pMenuSlotDlg;
	CDissolveDialog	* m_pDissolveDig;
	CEventNotifyDialog	* m_pEventNotifyDlg;
	CMallNoticeDialog	* m_pMallNoticeDlg;
//

	cAuctionBoard		* m_pAuctionBoard;

	CMoneyDlg			* m_MoneyDlg;
	cQuickDlg		* m_pQuickDlg;
	CGuageDialog		* m_pGuageDlg;

	CPartyDialog		m_PartyDlg;
	CPartyCreateDlg		* m_pPartyCreateDlg;
	CPartyInviteDlg		* m_pPartyInviteDlg;

	CGuildCreateDialog	* m_pGuildCreateDlg;
	CGuildDialog		* m_pGuildDlg;
	CGuildInviteDialog	* m_pGuildInviteDlg;
	CGuildMarkDialog	* m_pGuildMarkDlg;
	CGuildLevelUpDialog	* m_pGuildLevelUpDlg;
	CGuildNickNameDialog* m_pGuildNickNameDlg;
	CGuildRankDialog	* m_pGuildRankDlg;
	CGuildWarehouseDialog * m_pGuildWarehouseDlg;
	CGuildUnionCreateDialog * m_pGuildUnionCreateDlg;

	CMiniMapDlg			* m_pMiniMapDlg;
	CBigMapDlg			* m_pBigMapDlg;

	CStorageDialog		* m_pStorageDialog;
	CNoteDialog			* m_pNoteDialog;
	CFriendDialog		* m_pFriendDialog;
	CMiniNoteDialog		* m_pMiniNoteDialog;
	CMiniFriendDialog	* m_pMiniFriendDialog;
	CReviveDialog		* m_pReviveDialog;
	CWantNpcDialog		* m_pWantNpcDialog;
	CWantRegistDialog	* m_pWantRegistDialog;
	CMonsterGuageDlg	* m_pMonsterGuageDlg;
	CReinforceDlg		* m_pReinforceDlg;

	CQuestTotalDialog			* m_pQuestTotalDlg;
	CQuestDialog				* m_pQuestDlg;
	CQuestQuickViewDialog		* m_pQuestQuickViewDlg ;

	CGameNotifyDlg				* m_pGameNotifyDlg ;

	CWantedDialog				* m_pWantedDlg;
	CJournalDialog				* m_pJournalDlg;

	CBailDialog					* m_pBailDlg;

	CItemShopDialog		* m_pItemShopDialog;
	CMoveDialog			* m_pMoveDialog;
	CPointSaveDialog	* m_pPointSaveDialog;

	// guildfieldwar
	CGFWarDeclareDlg*	m_pGFWarDeclareDlg;
	CGFWarResultDlg*	m_pGFWarResultDlg;
	CGFWarInfoDlg*		m_pGFWarInfoDlg;
	CGuildWarInfoDlg*	m_pGuildWarInfoDlg;
	CGuildPlusTimeDlg*  m_pGuildPlusTimeDlg;//Add 060803 by wonju
	///////////////////////////////////////////////////////////////////////

	//
	// 070619 LYW --- Deactive SkillPointRedist.
	// 현재 사용하지 않아서 비활성화.
	//CSkillPointRedist*	m_pSkPointDlg;
	CSkillPointNotify*	m_pSkPointNotifyDlg;

	// partywar
	CPartyWarDialog*	m_pPartyWarDlg;

	// 외치기
	CShoutDialog*		m_pShoutDlg;
	// 061204 LYW --- Delete Preexistence Chatting Dialog.
	/*
	CShoutchatDialog*	m_pShoutchatDlg;
	*/

	// 낚시
	CFishingDialog*		m_pFishingDlg;
	CFishingGaugeDialog* m_pFishingGaugeDlg;
	CFishingPointDialog* m_pFishingPointDlg;

	// 날씨
	CWeatherDialog*		m_pWeatherDlg;

	//
	CChaseDialog*		m_pChaseDlg;
	CChaseinputDialog*	m_pChaseinputDlg;

	//
	CNameChangeDialog*	m_pNameChangeDlg;
	CNameChangeNotifyDlg*	m_pNameChangeNotifyDlg;

	// Guild Tournament
	CGTRegistDialog*			m_pGTRegistDlg;
	CGTRegistcancelDialog*		m_pGTRegistcancelDlg;
	CGTStandingDialog*			m_pGTStandingDlg;
	CGTBattleListDialog*		m_pGTBattleListDlg;
	CGTScoreInfoDialog*			m_pGTScoreInfoDlg;

	// seigewar
	CSWTimeRegDlg*				m_pSWTimeRegDlg;
	CSWProtectRegDlg*			m_pSWProtectRegDlg;
	CSWInfoDlg*					m_pSWInfoDlg;
	CSWStartTimeDialog*			m_pSWStartTimeDlg;

	// ItemSeal
	CSealDialog*			m_pSealDlg;

	CSWProfitDlg*				m_pSWProfitDlg;
	CSWEngraveDialog*			m_pEngraveDlg;
	CSWTimeDialog*				m_pSWTimeDlg;

	//
	CChangeJobDialog*			m_pChangeJobDlg;

	//
	//CReinforceResetDlg*			m_pReinforceResetDlg;
	//CRareCreateDialog*			m_pRareCreateDlg;



	// 06. 02. 강화 재료 안내 인터페이스 추가 - 이영준
	CReinforceGuideDialog*		m_pReinforceGuideDlg;
	// 06. 02. 내정보창 팁보기 추가 - 이영준
	CTipBrowserDlg*				m_pTipBrowserDlg;
	CKeySettingTipDlg*			m_pKeySettingTipDlg;

	CGuildNoteDlg*				m_pGuildNoteDlg;
	CUnionNoteDlg*				m_pUnionNoteDlg;

	CGuildNoticeDlg*			m_pGuildNoticeDlg;

	/////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환
	CSkillOptionChangeDlg*		m_pSkillOptionChangeDlg;
	CSkillOptionClearDlg*		m_pSkillOptionClearDlg;
	/////////////////////////////////////////////////////////////

	//SW060713 문하생
	CGuildJoinDialog* m_pGuildInvitationKindSelectDlg;

	//스크린샷
	CScreenShotDlg*			m_pScreenShotDlg;
	// 061127 LYW --- Add New Chatting Class Pointer In GameIn class.
	CChattingDlg*			m_pChattingDlg ;
	// 061130 LYW --- Add New Outside Chatting Clas Pointer In GameIn class.
	COSChattingDlg*			m_pOSChattingDlg ;
	// 061219 LYW --- Add main system dialog.
	CMainSystemDlg*			m_pMainSystemDlg ;

	cNpcImageDlg*			m_pNpcImageDlg;

	CDateMatchingDlg*		m_pDateMatchingDlg;			// 데이트 매칭 다이얼로그  2007/03/26 - 이진영

	// 070605 LYW --- GameIn : Add identification dialog.
	CIdentification*		m_pIdentificationDlg ;
	// 070605 LYW --- GameIn : Add favor icon dialog.
	CFavorIcon*				m_pFavorIconDlg ;
	// 070607 LYW --- GameIn : Add my info dialog.
	CDateMatchingInfoDlg*	m_pMatchMyInfoDlg ;
	// 070607 LYW --- GameIn : Add partner info dialog.
	CDateMatchingInfoDlg*	m_pMatchPartnerInfoDlg ;
	// 070607 LYW --- GameIn : Add record dialog.
	CDateMatchingRecordDlg*	m_pMatchRecordDlg ;

	CFamilyCreateDialog	* m_pFamilyCreateDlg;
	CFamilyDialog		* m_pFamilyDlg;
	CFamilyInviteDialog	* m_pFamilyInviteDlg;
	CFamilyMarkDialog	* m_pFamilyMarkDlg;
	CFamilyLevelUpDialog	* m_pFamilyLevelUpDlg;
	CFamilyNickNameDialog* m_pFamilyNickNameDlg;
	CFamilyRankDialog	* m_pFamilyRankDlg;
	CFamilyWarehouseDialog * m_pFamilyWarehouseDlg;
	CFamilyUnionCreateDialog * m_pFamilyUnionCreateDlg;
	CFamilyPlusTimeDlg*  m_pFamilyPlusTimeDlg;//Add 060803 by wonju
	CFamilyNoteDlg*				m_pFamilyNoteDlg;
	CFamilyNoticeDlg*			m_pFamilyNoticeDlg;
	CFamilyJoinDialog*		m_pFamilyInvitationKindSelectDlg;

	cSkillTrainingDlg*		m_pSkillTrainingDlg;

	// 071017 LYW --- GameIn : Add memder to tutorial dialog.
	cTutorialDlg*			m_TutorialDlg ;

	// 071017 LYW --- GameIn : Add memder to help dialog.
	cHelperDlg*				m_HelpDlg ;

	// 071023 LYW --- GameIn : Add member to tutorial button dialog.
	cTutorialBtnDlg*		m_TutorialBtnDlg ;

	cItemMallBtnDlg*		m_ItemMallBtnDlg ;
	cItemPopupDlg*			m_ItemPopupDlg ;

	// 071130 LYW --- GameIn : Add member to map move dialog.
	cMapMoveDialog*			m_MapMoveDlg ;

	// 071227 LYW --- GameIn : 캐릭터 이름변경창 추가.
	cChangeNameDialog*		m_pChangeNameDlg ;

	// 080109 LYW --- GameIn : 숫자 패드 다이얼로그 추가.
	cNumberPadDialog*		m_pNumberPadDlg ;

	//---KES AUTONOTE
	CAutoNoteDlg*			m_pAutoNoteDlg;
	CAutoAnswerDlg*			m_pAutoAnswerDlg;
	//--------------
	//---KES 상점검색
	CStoreSearchDlg*		m_pStoreSearchDlg;
	//---------------

	//---BROWSER
	CItemShopDlg*			m_pItemShopDlg;

	// 080403 LYW --- GameIn : Add dialogs for chatroom system.
	CChatRoomMainDlg*		m_pChatRoomMainDlg ;
	CChatRoomCreateDlg*		m_pChatRoomCreateDlg ;
	CChatRoomDlg*			m_pChatRoomDlg ;
	CChatRoomGuestListDlg*	m_pChatRoomGuestListDlg ;
	CChatRoomOptionDlg*		m_pChatRoomOptionDlg ;
	CChatRoomJoinDlg*		m_pChatRoomJoinDlg ;
	//CChatRoomLobbyDlg*		m_pChatRoomLobbyDlg ;

	// 080509 KTH -- GameIn : Add dialog for Animal control system
	CAnimalDialog*			m_pAnimalDialog;

	CPetInfoDialog*			mPetInfoDialog;
	CPetStateDialog*		mPetStateDialog;
	CPetWearedDialog*		mPetWearedDialog;
	CPetInvenDialog*		mPetInvenDialog;
	cDialog*				mPetUIDialog;
	CPetResurrectionDialog*	mPetResurrectionDialog;

	BOOL	m_bInitForGame;
	int		m_GameInInitKind;	
	DWORD	m_MovePoint;
	DWORD	m_MoveMap;
	DWORD	m_GuildIdx;		// Guild Tournament
	DWORD	m_BattleIdx;	// Guild Tournament - ObserverMode
	DWORD	m_SiegeWarMapNum;		// SiegeWar MapNum

public:	
	BOOL m_bGameInAcked;
	CGameIn();
	virtual ~CGameIn();

//	//MAKESINGLETON(CGameIn)
	BOOL Init(void* pInitParam);
	void Release(CGameState* pNextGameState);

//KES
	BOOL InitForGame();			//게임로딩->게임 일때 초기화
	void ReleaseForGame();		//게임->게임로딩 일때 릴리스
//

	void Process();
	void BeforeRender();
	void AfterRender();

	void NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg);
	
	BOOL IsGameInAcked()		{	return m_bGameInAcked;	}
	int GetGameInInitKind()	{ return m_GameInInitKind; 	}

	virtual void OnDisconnect();

	DWORD GetMoveMap()			{	 return m_MoveMap;	}
	void SetGuildIdx( DWORD GuildIdx )		{	m_GuildIdx = GuildIdx;	}
	void SetMoveMap( DWORD MapNum )			{	m_MoveMap = MapNum;		}
	void SetBattleIdx( DWORD BattleIdx )	{	m_BattleIdx = BattleIdx;	}
	void SetSiegeWarMapNum( DWORD MapNum )	{	m_SiegeWarMapNum = MapNum;	}
	
	///// custom
	CMonsterGuageDlg	*	GetMonsterGuageDlg()	{ return m_pMonsterGuageDlg;	}
	void	SetMonsterGuageDlg(CMonsterGuageDlg	* dlg)	{ m_pMonsterGuageDlg = dlg;	}

	CCharacterDialog * GetCharacterDialog(){ return m_pCharDlg; }
	void SetCharacterDialog(CCharacterDialog * dlg){ m_pCharDlg=dlg; }
	// 061220 LYW --- Delete this code.
	//CMainBarDialog	* GetMainInterfaceDialog(){ return m_pMainDlg; }
	//void SetMainInterfaceDialog(CMainBarDialog	* Dlg){ m_pMainDlg=Dlg; }

	// 061204 LYW --- Delete Preexistence Chatting Dialog.
	/*
	void SetChatDialog(CChatDialog	* Dlg){ m_pChatDlg=Dlg; }
	CChatDialog * GetChatDialog(){ return m_pChatDlg; }
	*/

	//SW050722 attach to minimap
//	void SetJackpotDialog(cJackpotDialog * Dlg){ m_pJackpotDlg=Dlg; }
//	cJackpotDialog* GetJackpotDialog(){ return m_pJackpotDlg; }

	void SetDebugDlg(CDebugDlg* Dlg){m_pDebugDlg = Dlg;}
	CDebugDlg* GetDebugDlg(){return m_pDebugDlg;}

	cSkillTreeDlg* GetSkillTreeDlg() { return m_pSkillTreeDlg; }
	void SetSkillTreeDlg( cSkillTreeDlg* dlg ) { m_pSkillTreeDlg = dlg; }

    CInventoryExDialog * GetInventoryDialog(){ return m_pInventoryDlg; }
	void SetInventoryDialog(CInventoryExDialog * dlg){ m_pInventoryDlg=dlg; }
	CExchangeDialog * GetExchangeDialog(){ return m_pExchangeDlg; }
	void SetExchangeDialog(CExchangeDialog * dlg){ m_pExchangeDlg=dlg; }
//	CRecvExchangeDialog * GetRecvExchangeDialog(){ return m_pRecvExchangeDlg; }
//	void SetRecvExchangeDialog(CRecvExchangeDialog * dlg){ m_pRecvExchangeDlg=dlg; }
	CMixDialog * GetMixDialog(){ return m_pMixDlg; }
	void SetMixDialog(CMixDialog * dlg){ m_pMixDlg=dlg; }
	CReinforceDlg * GetReinforceDialog(){ return m_pReinforceDlg; }
	void SetReinforceDialog(CReinforceDlg * dlg){ m_pReinforceDlg=dlg; }
		
	CDealDialog * GetDealDialog(){ return m_pDealDlg; }
	void SetDealDialog(CDealDialog * dlg){ m_pDealDlg=dlg; }

	// LYJ 051017 구입노점상 추가
	CStallKindSelectDlg * GetStallKindSelectDialog(){ return m_pStallKindSelectDlg; }
	void SetStallKindSelectDialog(CStallKindSelectDlg * dlg){ m_pStallKindSelectDlg=dlg; }	
	CStreetBuyStall * GetStreetBuyStallDialog(){ return m_pStreetBuyStallDlg; }
	void SetStreetBuyStallDialog(CStreetBuyStall * dlg){ m_pStreetBuyStallDlg=dlg; }
	CBuyRegDialog * GetBuyRegDialog(){ return m_pBuyRegDlg; }
	void SetBuyRegDialog(CBuyRegDialog * dlg){ m_pBuyRegDlg=dlg; }
	
	
// LBS
	CStreetStall * GetStreetStallDialog(){ return m_pStreetStallDlg; }
	void SetStreetStallDialog(CStreetStall * dlg){ m_pStreetStallDlg=dlg; }
	cNpcScriptDialog* GetNpcScriptDialog() { return m_pNpcScriptDlg; }
	void SetNpcScriptDialog(cNpcScriptDialog* dlg) { m_pNpcScriptDlg = dlg; }
////
	CMoneyDlg* GetMoneyDialog()	{	return m_MoneyDlg;	}
	void SetMoneyDialog(CMoneyDlg* dlg)	{	m_MoneyDlg = dlg;	}
	cQuickDlg* GetQuickDlg()	{	return m_pQuickDlg;	}
	void SetQuickDlg(cQuickDlg* dlg)	{	m_pQuickDlg = dlg;	}

//KES
//	CMacroModeDialog * GetMacroModeDialog(){ return m_pMacroModeDlg; }
//	void SetMacroModeDialog(CMacroModeDialog * dlg){ m_pMacroModeDlg=dlg; }
	CChatOptionDialog * GetChatOptionDialog() { return m_pChatOptionDlg; }
	void SetChatOptionDialog(CChatOptionDialog * dlg) { m_pChatOptionDlg = dlg; }	
	COptionDialog * GetOptionDialog() { return m_pOptionDlg; }
	void SetOptionDialog(COptionDialog * dlg) { m_pOptionDlg = dlg; }
	CExitDialog * GetExitDialog() { return m_pExitDlg; }
	void SetExitDialog(CExitDialog * dlg) { m_pExitDlg = dlg; }

	// desc_hseos_몬스터미터_01
	// S 몬스터미터 추가 added by hseos 2007.04.09
	CSHMonstermeterDlg *GetMonstermeterDlg() { return m_pMonstermeterDlg; }
	void SetMonstermeterDlg(CSHMonstermeterDlg * dlg) { m_pMonstermeterDlg = dlg; }
	// E 몬스터미터 추가 added by hseos 2007.04.09

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23
	CSHFarmBuyDlg *GetFarmBuyDlg() { return m_pFarmBuyDlg; }
	void SetFarmBuyDlg(CSHFarmBuyDlg * dlg) { m_pFarmBuyDlg = dlg; }

	CSHFarmUpgradeDlg *GetFarmUpgradeDlg() { return m_pFarmUpgradeDlg; }
	void SetFarmUpgradeDlg(CSHFarmUpgradeDlg * dlg) { m_pFarmUpgradeDlg = dlg; }

	CSHFarmManageDlg *GetFarmManageDlg() { return m_pFarmManageDlg; }
	void SetFarmManageDlg(CSHFarmManageDlg * dlg) { m_pFarmManageDlg = dlg; }
	// E 농장시스템 추가 added by hseos 2007.08.24	2007.09.10	2007.10.23

	// desc_hseos_데이트 존_01
	// S 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2008.01.24
	CSHDateZoneListDlg *GetDateZoneListDlg() { return m_pDateZoneListDlg; }
	void SetDateZoneListDlg(CSHDateZoneListDlg * dlg) { m_pDateZoneListDlg = dlg; }

	CSHChallengeZoneListDlg *GetChallengeZoneListDlg() { return m_pChallengeZoneListDlg; }
	void SetChallengeZoneListDlg(CSHChallengeZoneListDlg * dlg) { m_pChallengeZoneListDlg = dlg; }

	CSHChallengeZoneClearNo1Dlg *GetChallengeZoneClearNo1Dlg() { return m_pChallengeZoneClearNo1Dlg; }
	void SetChallengeZoneClearNo1Dlg(CSHChallengeZoneClearNo1Dlg * dlg) { m_pChallengeZoneClearNo1Dlg = dlg; }
	// E 데이트 존 추가 added by hseos 2007.11.14  2007.11.15	2008.01.24

	CCharStateDialog* GetCharStateDialog() { return m_pCharStateDlg; }
	void SetCharStateDialog(CCharStateDialog* dlg) { m_pCharStateDlg = dlg; }

	void SetMenuSlotDialog( CMenuSlotDialog* pDlg ) { m_pMenuSlotDlg = pDlg; }
	CMenuSlotDialog* GetMenuSlotDialog() { return m_pMenuSlotDlg; }

	void SetDissolveDialog( CDissolveDialog* pDlg ) { m_pDissolveDig = pDlg; }
	CDissolveDialog* GetDissolveDialog() { return m_pDissolveDig; }

	void SetEventNotifyDialog( CEventNotifyDialog* pDlg ) { m_pEventNotifyDlg = pDlg; }
	CEventNotifyDialog* GetEventNotifyDialog() { return m_pEventNotifyDlg; }

	void SetMallNoticeDialog( CMallNoticeDialog* pDlg ) { m_pMallNoticeDlg = pDlg; }
	CMallNoticeDialog* GetMallNoticeDialog() { return m_pMallNoticeDlg; }
//
	cAuctionBoard * GetAuctionBoard(){ return m_pAuctionBoard; }
	void SetAuctionBoard(cAuctionBoard * dlg){ m_pAuctionBoard=dlg; }

	CGuageDialog * GetGuageDlg(){ return m_pGuageDlg; }
	void SetGuageDlg(CGuageDialog * dlg){ m_pGuageDlg=dlg; }

	CPartyDialog* GetPartyDialog() { return &m_PartyDlg; }
	
	CPartyCreateDlg* GetPartyCreateDialog() { return m_pPartyCreateDlg;	}
	void SetPartyCreateDialog(CPartyCreateDlg* dlg)	{ m_pPartyCreateDlg = dlg;	}

	CPartyInviteDlg* GetPartyInviteDialog() { return m_pPartyInviteDlg;	}
	void SetPartyInviteDialog(CPartyInviteDlg* dlg)	{ m_pPartyInviteDlg = dlg;	}
	
	CGuildCreateDialog* GetGuildCreateDlg() { return m_pGuildCreateDlg; }
	void SetGuildCreateDlg(CGuildCreateDialog* dlg) { m_pGuildCreateDlg = dlg;	}

	CGuildUnionCreateDialog* GetGuildUnionCreateDlg() { return m_pGuildUnionCreateDlg; }
	void SetGuildUnionCreateDlg(CGuildUnionCreateDialog* dlg) { m_pGuildUnionCreateDlg = dlg;	}	
	
	CGuildDialog* GetGuildDlg() { return m_pGuildDlg; }
	void SetGuildDlg(CGuildDialog* dlg) { m_pGuildDlg = dlg;	}

	CGuildInviteDialog* GetGuildInviteDlg() { return m_pGuildInviteDlg; }
	void SetGuildInviteDlg(CGuildInviteDialog* dlg) { m_pGuildInviteDlg = dlg;	}
	
	CGuildMarkDialog* GetGuildMarkDlg() { return m_pGuildMarkDlg; }
	void SetGuildMarkDlg(CGuildMarkDialog* dlg) { m_pGuildMarkDlg = dlg;	}
	
	CGuildLevelUpDialog* GetGuildLevelUpDlg() { return m_pGuildLevelUpDlg; }
	void SetGuildLevelUpDlg(CGuildLevelUpDialog* dlg) { m_pGuildLevelUpDlg = dlg;	}

	CGuildNickNameDialog* GetGuildNickNameDlg() { return m_pGuildNickNameDlg; }
	void SetGuildNickNameDlg(CGuildNickNameDialog* dlg) { m_pGuildNickNameDlg = dlg;	}

	CGuildRankDialog* GetGuildRankDlg() { return m_pGuildRankDlg; }
	void SetGuildRankDlg(CGuildRankDialog* dlg) { m_pGuildRankDlg = dlg;	}
	
	CGuildWarehouseDialog* GetGuildWarehouseDlg() { return m_pGuildWarehouseDlg; }
	void SetGuildWarehouseDlg(CGuildWarehouseDialog* dlg) { m_pGuildWarehouseDlg = dlg;	}

	CMiniMapDlg* GetMiniMapDialog() { return m_pMiniMapDlg; 	}
	void SetMiniMapDialog(CMiniMapDlg* dlg) { m_pMiniMapDlg = dlg; 	}

	CBigMapDlg* GetBigMapDialog() { return m_pBigMapDlg; 	}
	void SetBigMapDialog(CBigMapDlg* dlg) { m_pBigMapDlg = dlg; 	}

	CStorageDialog* GetStorageDialog() { return m_pStorageDialog;	}
	void SetStorageDialog(CStorageDialog* dlg) { m_pStorageDialog = dlg; 	}
	
	CFriendDialog* GetFriendDialog() { return m_pFriendDialog; }
	void SetFriendDialog(CFriendDialog* dlg) { m_pFriendDialog = dlg; }

	CNoteDialog* GetNoteDialog() { return m_pNoteDialog; }
	void SetNoteDialog(CNoteDialog* dlg) { m_pNoteDialog = dlg;	}
	
	CMiniNoteDialog* GetMiniNoteDialog() { return m_pMiniNoteDialog;	}
	void SetMiniNoteDialog(CMiniNoteDialog* dlg) { m_pMiniNoteDialog = dlg;	}
	
	CMiniFriendDialog * GetMiniFriendDialog() { return m_pMiniFriendDialog; 	}
	void SetMiniFriendDialog(CMiniFriendDialog* dlg) { m_pMiniFriendDialog = dlg;	}
	
	CReviveDialog * GetReviveDialog() { return m_pReviveDialog; 	}
	void SetReviveDialog(CReviveDialog* dlg) { m_pReviveDialog = dlg;	}

	CWantNpcDialog * GetWantNpcDialog() { return m_pWantNpcDialog; 	}
	void SetWantNpcDialog(CWantNpcDialog* dlg) { m_pWantNpcDialog = dlg; }
	
	CWantRegistDialog * GetWantRegistDialog() { return m_pWantRegistDialog; }
	void SetWantRegistDialog(CWantRegistDialog * dlg) { m_pWantRegistDialog = dlg;	}

	CQuestTotalDialog * GetQuestTotalDialog() { return m_pQuestTotalDlg;	}
	void SetQuestTotalDialog(CQuestTotalDialog * dlg) { m_pQuestTotalDlg = dlg;	}
	CQuestDialog* GetQuestDialog()			{ return m_pQuestDlg;	}
	void SetQuestDialog(CQuestDialog* dlg)	{ m_pQuestDlg = dlg;	}

	CQuestQuickViewDialog* GetQuestQuickViewDialog() { return m_pQuestQuickViewDlg ; }
	void SetQuestQuickViewDialog(CQuestQuickViewDialog* pDlg) { m_pQuestQuickViewDlg = pDlg ; }

	CGameNotifyDlg* GetGameNotifyDlg() { return m_pGameNotifyDlg ; }
	void SetGameNotifyDlg(CGameNotifyDlg* pDlg) { m_pGameNotifyDlg = pDlg ; }

	CWantedDialog* GetWantedDialog() { return m_pWantedDlg; }
	void SetWantedDialog(CWantedDialog* dlg) { m_pWantedDlg = dlg;	}
	CJournalDialog* GetJournalDialog() { return m_pJournalDlg;	}
	void SetJournalDialog(CJournalDialog* dlg) { m_pJournalDlg = dlg; }


	CBailDialog* GetBailDialog()			{	return m_pBailDlg;	}
	void SetBailDialog(CBailDialog* dlg)	{	m_pBailDlg = dlg; }

	CItemShopDialog* GetItemShopDialog()			{	return m_pItemShopDialog;	}
	void SetItemShopDialog(CItemShopDialog* dlg)	{	m_pItemShopDialog = dlg; 	}
	
	CMoveDialog* GetMoveDialog()					{	return m_pMoveDialog;	}
	void SetMoveDialog(CMoveDialog* dlg)			{	m_pMoveDialog = dlg;	}
	CPointSaveDialog* GetPointSaveDialog()			{	return m_pPointSaveDialog;	}
	void SetPointSaveDialog(CPointSaveDialog* dlg)	{	m_pPointSaveDialog = dlg;	}

	// guildfieldwar
	CGFWarDeclareDlg* GetGFWarDeclareDlg()			{	return m_pGFWarDeclareDlg;	}
	void SetGFWarDeclareDlg(CGFWarDeclareDlg* dlg)	{	m_pGFWarDeclareDlg = dlg;	}
	CGFWarResultDlg* GetGFWarResultDlg()			{	return m_pGFWarResultDlg;	}
	void SetGFWarResultDlg(CGFWarResultDlg* dlg)	{	m_pGFWarResultDlg = dlg;	}
	CGFWarInfoDlg* GetGFWarInfoDlg()				{	return m_pGFWarInfoDlg;	}
	void SetGFWarInfoDlg(CGFWarInfoDlg* dlg)		{	m_pGFWarInfoDlg = dlg;	}
	CGuildWarInfoDlg* GetGuildWarInfoDlg()			{	return m_pGuildWarInfoDlg;	}
	void SetGuildWarInfoDlg(CGuildWarInfoDlg* dlg)	{	m_pGuildWarInfoDlg = dlg;	}
	CGuildPlusTimeDlg* GetGuildPlusTimeDlg()			{	return m_pGuildPlusTimeDlg;	}//Add 060803 by wonju
	void SetGuildPlusTimeDlg(CGuildPlusTimeDlg* dlg)	{	m_pGuildPlusTimeDlg = dlg;	}//Add 060803 by wonju
	
	// 
	// 070619 LYW --- Deactive SkillPointRedist.
	// 현재 사용하지 않아서 비활성화.
	//CSkillPointRedist*	GetSkPointDlg()					{	return m_pSkPointDlg;	}
	//void SetSkPointDlg(CSkillPointRedist* dlg)			{	m_pSkPointDlg = dlg;	}
	CSkillPointNotify*	GetSkPointNotifyDlg()			{	return m_pSkPointNotifyDlg;	}
	void SetSkPointNotifydlg(CSkillPointNotify* dlg)	{	m_pSkPointNotifyDlg = dlg;	}

	// partywar
	CPartyWarDialog*	GetPartyWarDlg()				{	return m_pPartyWarDlg;	}
	void SetPartyWarDlg(CPartyWarDialog* dlg)			{	m_pPartyWarDlg = dlg;	}

	// 외치기
	CShoutDialog*	GetShoutDlg()						{	return m_pShoutDlg;		}
	void SetShoutDlg(CShoutDialog* dlg)					{	m_pShoutDlg = dlg;		}
	// 061204 LYW --- Delete Preexistence Chatting Dialog.
	/*
	CShoutchatDialog*	GetShoutchatDlg()				{	return m_pShoutchatDlg;		}
	void SetShoutchatDlg(CShoutchatDialog* dlg)			{	m_pShoutchatDlg = dlg;		}
	*/

	// 낚시
	CFishingDialog* GetFishingDlg()						{	return m_pFishingDlg;	}
	void SetFishingDlg(CFishingDialog* dlg)				{	m_pFishingDlg = dlg;	}

	CFishingGaugeDialog* GetFishingGaugeDlg()			{	return m_pFishingGaugeDlg;	}
	void SetFishingGaugeDlg(CFishingGaugeDialog* dlg)	{	m_pFishingGaugeDlg = dlg;	}

	CFishingPointDialog* GetFishingPointDlg()			{	return m_pFishingPointDlg;	}
	void SetFishingPointDlg(CFishingPointDialog* dlg)	{	m_pFishingPointDlg = dlg;	}


	// 날씨
	CWeatherDialog* GetWeatherDlg()						{	return m_pWeatherDlg;	}
	void SetWeatherDlg(CWeatherDialog* dlg)				{	m_pWeatherDlg = dlg;	}


	//
	CChaseDialog*	GetChaseDlg()						{	return m_pChaseDlg;		}
	void SetChaseDlg(CChaseDialog* dlg)					{	m_pChaseDlg = dlg;		}
	CChaseinputDialog*	GetChaseinputDlg()					{	return m_pChaseinputDlg;	}
	void SetChaseinputDlg(CChaseinputDialog* dlg)			{	m_pChaseinputDlg = dlg;		}

	//
	CNameChangeDialog*	GetNameChangeDlg()				{	return m_pNameChangeDlg;	}
	void SetNameChangeDlg(CNameChangeDialog* dlg)		{	m_pNameChangeDlg = dlg;		}
	CNameChangeNotifyDlg*	GetNameChangeNotifyDlg()		{	return m_pNameChangeNotifyDlg;	}
	void SetNameChangeNotifyDlg(CNameChangeNotifyDlg* dlg)	{	m_pNameChangeNotifyDlg = dlg;		}

	//
	//CRareCreateDialog*	GetRareCreateDlg()				{	return m_pRareCreateDlg;	}
	//void SetRareCreateDialog( CRareCreateDialog* dlg )	{	m_pRareCreateDlg = dlg;		}

	//스크린샷
	CScreenShotDlg*	GetScreenShotDlg()				{	return m_pScreenShotDlg;	}
	void SetScreenShotDlg( CScreenShotDlg* dlg )	{	m_pScreenShotDlg = dlg;		}

	// 061127 LYW --- Add Return & Setting Function For New Chatting System.
	CChattingDlg* GetChattingDlg() { return m_pChattingDlg ; }
	void SetChattingDlg( CChattingDlg* dlg ) { m_pChattingDlg = dlg ; }

	// 061127 LYW --- Add Return & Setting Function For New Outside Chatting Dialog.
	COSChattingDlg* GetOSChattingDlg() { return m_pOSChattingDlg ; }
	void SetOSChattingDlg( COSChattingDlg* dlg ) { m_pOSChattingDlg = dlg ; }

	// 061219 LYW --- Add function to return and setting main system dialog.
	void SetMainSystemDlg( CMainSystemDlg* pDlg ) { m_pMainSystemDlg = pDlg ; }
	CMainSystemDlg* GetMainSystemDlg() { return m_pMainSystemDlg ; }
	//-----------

	cNpcImageDlg* GetNpcImageDlg() { return m_pNpcImageDlg; }
	void SetNpcImageDlg( cNpcImageDlg* dlg ) { m_pNpcImageDlg = dlg; }

	BOOL	m_bDieInBossMap;
	int		m_nCountInBossMap;
	DWORD	m_dwTimeInBossMap;

	void	ProcessInBossMap();


	// Guild Tournament
	CGTRegistDialog*	GetGTRegistDlg()					{	return m_pGTRegistDlg;			}
	void SetGTRegistDlg(CGTRegistDialog* dlg)				{	m_pGTRegistDlg = dlg;			}
	CGTRegistcancelDialog*	GetGTRegistcancelDlg()			{	return m_pGTRegistcancelDlg;	}
	void SetGTRegistcancelDlg(CGTRegistcancelDialog* dlg)	{	m_pGTRegistcancelDlg = dlg;		}
	CGTStandingDialog*	GetGTStandingDlg()					{	return m_pGTStandingDlg;		}
	void SetGTStandingDlg(CGTStandingDialog* dlg)			{	m_pGTStandingDlg = dlg;			}
	CGTBattleListDialog*	GetGTBattleListDlg()			{	return m_pGTBattleListDlg;		}
	void SetGTBattleListDlg(CGTBattleListDialog* dlg)		{	m_pGTBattleListDlg = dlg;		}
	CGTScoreInfoDialog*	GetGTScoreInfoDlg()					{	return m_pGTScoreInfoDlg;		}
	void SetGTScoreInfoDlg(CGTScoreInfoDialog* dlg)			{	m_pGTScoreInfoDlg = dlg;		}

	// seigewar
	CSWTimeRegDlg*		GetSWTimeRegDlg()					{ return m_pSWTimeRegDlg; }
	void				SetSWTimeRegDlg( CSWTimeRegDlg* dlg )	{ m_pSWTimeRegDlg = dlg; }
	CSWProtectRegDlg*	GetSWProtectRegDlg()				{ return m_pSWProtectRegDlg; }
	void				SetSWProtectRegDlg( CSWProtectRegDlg* dlg )	{ m_pSWProtectRegDlg = dlg; }
	CSWInfoDlg*			GetSWInfoDlg()						{ return m_pSWInfoDlg; }
	void				SetSWInfoDlg( CSWInfoDlg* dlg )		{ m_pSWInfoDlg = dlg; }
	CSWProfitDlg*		GetSWProfitDlg()					{ return m_pSWProfitDlg; }
	void				SetSWProfitDlg( CSWProfitDlg* dlg )	{ m_pSWProfitDlg = dlg; }
	CSWEngraveDialog*	GetSWEngraveDlg()					{ return m_pEngraveDlg;	}
	void				SetSWEngraveDlg( CSWEngraveDialog* dlg)	{	m_pEngraveDlg = dlg;	}
	CSWTimeDialog*		GetSWTimeDlg()						{ return m_pSWTimeDlg;	}
	void				SetSWTimeDlg( CSWTimeDialog* dlg)	{ m_pSWTimeDlg = dlg;	}
	CSWStartTimeDialog*	GetSWStartTimeDlg()					{ return m_pSWStartTimeDlg;	}
	void				SetSWStartTimeDlg( CSWStartTimeDialog* dlg )	{ m_pSWStartTimeDlg = dlg; }
	
	// ItemSeal
	CSealDialog* GetSealDlg()								{	return m_pSealDlg;			}
	void SetSealDlg( CSealDialog* dlg )						{	m_pSealDlg = dlg;			}

	// 
	CChangeJobDialog*	GetChangeJobDlg()					{	return m_pChangeJobDlg;		}
	void				SetChangeJobDlg( CChangeJobDialog* dlg )		{	m_pChangeJobDlg = dlg;		}

	// ReinforceReset
	//CReinforceResetDlg*	GetReinforceResetDlg()				{	return m_pReinforceResetDlg;	}
	//void				SetReinforceResetDlg( CReinforceResetDlg* dlg )		{	m_pReinforceResetDlg = dlg;		}

	// 06. 02. 강화 재료 안내 인터페이스 추가 - 이영준
	CReinforceGuideDialog* GetReinforceGuideDlg()	{ return m_pReinforceGuideDlg; }
	void SetReinforceGuideDlg(CReinforceGuideDialog* dlg) { m_pReinforceGuideDlg = dlg; }

	// 06. 02. 내정보창 팁보기 추가 - 이영준
	CTipBrowserDlg* GetTipBrowserDlg() { return m_pTipBrowserDlg; }
	void SetTipBrowserDlg(CTipBrowserDlg* dlg) { m_pTipBrowserDlg = dlg; }

	CKeySettingTipDlg* GetKeySettingTipDlg() { return m_pKeySettingTipDlg; }
	void SetKeySettingTipDlg(CKeySettingTipDlg* dlg) { m_pKeySettingTipDlg = dlg; }

	CGuildNoteDlg*	GetGuildNoteDlg() { return	m_pGuildNoteDlg; }
	void SetGuildNoteDlg(CGuildNoteDlg* dlg) { m_pGuildNoteDlg = dlg; }
	CUnionNoteDlg*	GetUnionNoteDlg() { return	m_pUnionNoteDlg; }
	void SetUnionNoteDlg(CUnionNoteDlg* dlg) { m_pUnionNoteDlg = dlg; }

	// 06. 03. 문파공지 - 이영준
	CGuildNoticeDlg* GetGuildNoticeDlg() { return m_pGuildNoticeDlg; }
	void SetGuildNoticeDlg(CGuildNoticeDlg *dlg) { m_pGuildNoticeDlg = dlg; }

	////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환
	CSkillOptionChangeDlg* GetSkillOptionChangeDlg() { return m_pSkillOptionChangeDlg; }
	void SetSkillOptionChangeDlg(CSkillOptionChangeDlg *dlg) { m_pSkillOptionChangeDlg = dlg; }
	
	CSkillOptionClearDlg* GetSkillOptionClearDlg() { return m_pSkillOptionClearDlg; }
	void SetSkillOptionClearDlg(CSkillOptionClearDlg *dlg) { m_pSkillOptionClearDlg = dlg; }
	////////////////////////////////////////////////////////////////////////

	//SW060713 문하생
	CGuildJoinDialog* GetGuildJoinDialog() {	return m_pGuildInvitationKindSelectDlg;	}
	void SetGuildJoinDialog(CGuildJoinDialog* dlg)	{	m_pGuildInvitationKindSelectDlg = dlg;	}

	CDateMatchingDlg*	GetDateMatchingDlg(){ return m_pDateMatchingDlg;}					// 데이트 매칭 다이얼로그. 2007/03/26 - 이진영
	void	SetDateMatchingDlg( CDateMatchingDlg* pDlg){ m_pDateMatchingDlg = pDlg;}

	// 070605 LYW --- GameIn : Add function to setting and return identification dialog.
	void SetIdentificationDlg( CIdentification* pDlg )  { m_pIdentificationDlg = pDlg ; }
	CIdentification* GetIdentificationDlg()				{ return m_pIdentificationDlg ; }

	// 070605 LYW --- GameIn : Add function to setting and return favor icon dialog.
	void SetFavorIconDlg( CFavorIcon* pDlg )			{ m_pFavorIconDlg = pDlg ; }
	CFavorIcon* GetFavorIconDlg()						{ return m_pFavorIconDlg ; }

	// 070607 LYW --- GameIn : Add function to setting and return my info dialog.
	void SetMatchMyInfoDlg( CDateMatchingInfoDlg* pDlg ) { m_pMatchMyInfoDlg = pDlg ; }
	CDateMatchingInfoDlg* GetMatchMyInfoDlg()			 { return m_pMatchMyInfoDlg ; }

	// 070607 LYW --- GameIn : Add function to setting and return partner info dialog.
	void SetMatchPartnerInfoDlg( CDateMatchingInfoDlg* pDlg ) { m_pMatchPartnerInfoDlg = pDlg ; }
	CDateMatchingInfoDlg* GetMatchPartnerInfoDlg()			  { return m_pMatchPartnerInfoDlg ; }

	// 070607 LYW --- GameIn : Add function to setting and return record dialog.
	void SetMatchRecordDlg(CDateMatchingRecordDlg* pDlg )	{ m_pMatchRecordDlg = pDlg ; }
	CDateMatchingRecordDlg* GetMatchRecordDlg()				{ return m_pMatchRecordDlg ; }

	CFamilyCreateDialog* GetFamilyCreateDlg() { return m_pFamilyCreateDlg; }
	void SetFamilyCreateDlg(CFamilyCreateDialog* dlg) { m_pFamilyCreateDlg = dlg;	}

	CFamilyUnionCreateDialog* GetFamilyUnionCreateDlg() { return m_pFamilyUnionCreateDlg; }
	void SetFamilyUnionCreateDlg(CFamilyUnionCreateDialog* dlg) { m_pFamilyUnionCreateDlg = dlg;	}	
	
	CFamilyDialog* GetFamilyDlg() { return m_pFamilyDlg; }
	void SetFamilyDlg(CFamilyDialog* dlg) { m_pFamilyDlg = dlg;	}

	CFamilyInviteDialog* GetFamilyInviteDlg() { return m_pFamilyInviteDlg; }
	void SetFamilyInviteDlg(CFamilyInviteDialog* dlg) { m_pFamilyInviteDlg = dlg;	}
	
	CFamilyMarkDialog* GetFamilyMarkDlg() { return m_pFamilyMarkDlg; }
	void SetFamilyMarkDlg(CFamilyMarkDialog* dlg) { m_pFamilyMarkDlg = dlg;	}
	
	CFamilyLevelUpDialog* GetFamilyLevelUpDlg() { return m_pFamilyLevelUpDlg; }
	void SetFamilyLevelUpDlg(CFamilyLevelUpDialog* dlg) { m_pFamilyLevelUpDlg = dlg;	}

	CFamilyNickNameDialog* GetFamilyNickNameDlg() { return m_pFamilyNickNameDlg; }
	void SetFamilyNickNameDlg(CFamilyNickNameDialog* dlg) { m_pFamilyNickNameDlg = dlg;	}

	CFamilyRankDialog* GetFamilyRankDlg() { return m_pFamilyRankDlg; }
	void SetFamilyRankDlg(CFamilyRankDialog* dlg) { m_pFamilyRankDlg = dlg;	}

	CFamilyWarehouseDialog* GetFamilyWarehouseDlg() { return m_pFamilyWarehouseDlg; }
	void SetFamilyWarehouseDlg(CFamilyWarehouseDialog* dlg) { m_pFamilyWarehouseDlg = dlg;	}

	CFamilyJoinDialog* GetFamilyJoinDialog() {	return m_pFamilyInvitationKindSelectDlg;	}
	void SetFamilyJoinDialog(CFamilyJoinDialog* dlg)	{	m_pFamilyInvitationKindSelectDlg = dlg;	}

	CFamilyPlusTimeDlg* GetFamilyPlusTimeDlg()			{	return m_pFamilyPlusTimeDlg;	}//Add 060803 by wonju
	void SetFamilyPlusTimeDlg(CFamilyPlusTimeDlg* dlg)	{	m_pFamilyPlusTimeDlg = dlg;	}//Add 060803 by wonju

	CFamilyNoteDlg*	GetFamilyNoteDlg() { return	m_pFamilyNoteDlg; }
	void SetFamilyNoteDlg(CFamilyNoteDlg* dlg) { m_pFamilyNoteDlg = dlg; }

	CFamilyNoticeDlg* GetFamilyNoticeDlg() { return m_pFamilyNoticeDlg; }
	void SetFamilyNoticeDlg(CFamilyNoticeDlg *dlg) { m_pFamilyNoticeDlg = dlg; }


	CPetInfoDialog*			GetPetInfoDlg()											{ return mPetInfoDialog; }
	void					SetPetInfoDlg( CPetInfoDialog* pDlg )					{ mPetInfoDialog = pDlg; }
	CPetStateDialog*		GetPetStateDlg()										{ return mPetStateDialog; }
	void					SetPetStateDlg( CPetStateDialog* pDlg )					{ mPetStateDialog = pDlg; }
	CPetWearedDialog*		GetPetWearedDlg()										{ return mPetWearedDialog; }
	void					SetPetWearedDlg( CPetWearedDialog* pDlg )				{ mPetWearedDialog = pDlg; }
	CPetInvenDialog*		GetPetInvenDlg()										{ return mPetInvenDialog; }
	void					SetPetInvenDlg( CPetInvenDialog* pDlg )					{ mPetInvenDialog = pDlg; }
	cDialog*				GetPetUIDlg()											{ return mPetUIDialog; }
	void					SetPetUIDlg( cDialog* pDlg )							{ mPetUIDialog = pDlg; }
	CPetResurrectionDialog*	GetPetResurrectionDlg()									{ return mPetResurrectionDialog; }
	void					SetPetResurrectionDlg( CPetResurrectionDialog* pDlg )	{ mPetResurrectionDialog = pDlg; }

	// 080509 KTH --
	CAnimalDialog*	GetAnimalDialog()						{	return m_pAnimalDialog;	}
	void			SetAnimalDialog(CAnimalDialog* dlg)		{	m_pAnimalDialog = dlg;	}

	cSkillTrainingDlg* GetSkillTrainingDlg() { return m_pSkillTrainingDlg; }
	void SetSkillTrainingDlg( cSkillTrainingDlg *dlg ) { m_pSkillTrainingDlg = dlg; }
	//캐릭터 로그인 시간
	SYSTEMTIME	GameinServerTime;			//서버에서 받아온 캐릭터 로그인 시간
	DWORD				m_Clientdate;		//클라이언트에서 받아온 캐릭터 로그인 날짜
	DWORD				m_Clienttime;		//클라이언트에서 받아온 캐릭터 로그인 시간

	void	SetLoginTime(SYSTEMTIME Time);
	SYSTEMTIME	GetLoginTime();

	void	GetClientLoginTime(DWORD& date, DWORD& time);
	void	SetClientLoginTime(DWORD date, DWORD time);

	// 070205 LYW --- Add functions to process network msg.
public :
	/// USER CONNECTION PART.
	void UserConn_NetworkMsgParse(BYTE Protocol,void* pMsg) ;
	void UserConn_Object_Remove( void* pMsg ) ;
	void UserConn_Monster_Die( void* pMsg ) ;
	void UserConn_Character_Die( void* pMsg ) ;
	// 070418 LYW --- GameIn : Add function to notice apply exp.
	void UserConn_ApplyExp( void* pMsg ) ;
	// 070418 LYW --- GameIn : Add function to notice down exp.
	void UserConn_DownExp( void* pMsg ) ;
	// 070418 LYW --- GameIn : Add function to notice down level.
	void UserConn_DownLevel( void* pMsg ) ;
	void UserConn_Ready_To_Revive() ;
	void UserConn_Character_Revive( void* pMsg ) ;
	void UserConn_Character_Revive_Nack( void* pMsg ) ;
	void UserConn_Npc_Add( void* pMsg ) ;
	void UserConn_GridInit() ;
	void UserConn_GameIn_Ack( void* pMsg ) ;
	void UserConn_Character_Add( void* pMsg ) ;
	void UserConn_Monster_Add( void* pMsg ) ;
	void UserConn_BossMonster_Add( void* pMsg ) ;
	void UserConn_ChangeMap_Ack( void* pMsg ) ;

	void UserConn_Pet_Add( void* pMsg ) ;
	void UserConn_Pet_Remove( void* pMsg ) ;

	// 070918 LYW --- GameIn : Add function to change map through the npc.
	void UserConn_Npc_ChangeMap_Ack(void* pMsg) ;
	void UserConn_Npc_ChangeMap_Nack() ;
	void UserConn_Cheat_ChangeMap_Ack( void* pMsg ) ;
	void UserConn_ChangeMap_Nack() ;
	void UserConn_CharacterList_Ack( void* pMsg ) ;
	void UserConn_BackToCharSel_Nack() ;
	void UserConn_MapDesc( void* pMsg ) ;
	void UserConn_SetVisible( void* pMsg ) ;
	void UserConn_Map_Out_To_EventMap( void* pMsg ) ;
	void UserConn_Map_Out_To_EventBeforeMap() ;

	/// NPC PART.
	void NPC_NetworkMsgParse(BYTE Protocol,void* pMsg) ;
	void Npc_Speech_Ack( void* pMsg ) ;
	void Npc_Speech_Nack( void* pMsg ) ;
	void Npc_CloseBomul_Ack() ;
	void Npc_Die_Ack( void* pMsg ) ;
	void Npc_DoJob_Nack() ;

	/// SIGNAL PART.
	void SIGNAL_NetworkMsgParse(BYTE Protocol,void* pMsg) ;
	void Signal_CommonUser( void* pMsg ) ;
	// 080114 KTH -- 아이템 획득을 알림
	void Signal_UserAddItem( void* pMsg ) ;
	void Signal_ShowDown_Result( void* pMsg ) ;

	// 071017 LYW --- GameIn : Add function to setting and return tutorial dialog.
	void SetTutorialDlg(cTutorialDlg* pDlg) { m_TutorialDlg = pDlg ; }
	cTutorialDlg* GetTutorialDlg() { return m_TutorialDlg ; }

	// 071017 LYW --- GameIn : Add function to setting and return help dialog.
	void SetHelpDlg(cHelperDlg* pDlg) { m_HelpDlg = pDlg ; }
	cHelperDlg* GetHelpDlg() { return m_HelpDlg ; }

	// 071023 LYW --- GameIn : Add function to setting and return notice button dialog of tutorial.
	void SetTutorialBtnDlg(cTutorialBtnDlg* pDlg) { m_TutorialBtnDlg = pDlg ; }
	cTutorialBtnDlg* GetTutorialBtnDlg() { return m_TutorialBtnDlg ; }

	void SetItemMallBtnDlg(cItemMallBtnDlg* pDlg) { m_ItemMallBtnDlg = pDlg ; }
	cItemMallBtnDlg* GetItemMallBtnDlg() { return m_ItemMallBtnDlg ; }
	void SetItemPopupDlg(cItemPopupDlg* pDlg) { m_ItemPopupDlg = pDlg ; }
	cItemPopupDlg* GetItemPopupDlg() { return m_ItemPopupDlg ; }
	// 071130 LYW --- GameIn : Add function to setting and return map move dialog.
	void SetMapMoveDlg(cMapMoveDialog* pDlg) { m_MapMoveDlg = pDlg ; }
	cMapMoveDialog* GetMapMoveDlg() { return m_MapMoveDlg ; }

	// 071227 LYW --- GameIn : Add function to setting and return change name dialog.
	void SetChangeNameDlg( cChangeNameDialog* pDlg ) { m_pChangeNameDlg = pDlg ; }
	cChangeNameDialog* GetChangeNameDlg() { return m_pChangeNameDlg ; }

	// 080109 LYW --- cWindowManager : 숫자 패드 다이얼로그 추가.
	void SetNumberPadDlg( cNumberPadDialog* pDlg ) { m_pNumberPadDlg = pDlg ; }
	cNumberPadDialog* GetNumberPadDlg() { return m_pNumberPadDlg ; }

	//---KES AUTONOTE
	void SetAutoNoteDlg( CAutoNoteDlg* pDlg )		{ m_pAutoNoteDlg = pDlg; }
	CAutoNoteDlg* GetAutoNoteDlg()					{ return m_pAutoNoteDlg; }
	void SetAutoAnswerDlg( CAutoAnswerDlg* pDlg )	{ m_pAutoAnswerDlg = pDlg; }
	CAutoAnswerDlg* GetAutoAnswerDlg()				{ return m_pAutoAnswerDlg; }
	//--------------

	//---KES 상점검색
	void SetStoreSearchDlg( CStoreSearchDlg* pDlg )		{ m_pStoreSearchDlg = pDlg; }
	CStoreSearchDlg* GetStoreSearchDlg()				{ return m_pStoreSearchDlg; }
	//---------------

	//---BROWSER
	void SetItemShopDlg( CItemShopDlg* pDlg )		{ m_pItemShopDlg = pDlg; }
	CItemShopDlg* GetItemShopDlg()				{ return m_pItemShopDlg; }

	// 080403 LYW --- GameIn : Add functions to setting and return dialogs for chatroom system.
	void SetChatRoomMainDlg( CChatRoomMainDlg* pDlg )			{ m_pChatRoomMainDlg = pDlg ; }
	CChatRoomMainDlg* GetChatRoomMainDlg()						{ return m_pChatRoomMainDlg ; }

	void SetChatRoomCreateDlg( CChatRoomCreateDlg* pDlg )		{ m_pChatRoomCreateDlg = pDlg ; }
	CChatRoomCreateDlg* GetChatRoomCreateDlg()					{ return m_pChatRoomCreateDlg ; }

	void SetChatRoomDlg( CChatRoomDlg* pDlg )					{ m_pChatRoomDlg = pDlg ; }
	CChatRoomDlg* GetChatRoomDlg()								{ return m_pChatRoomDlg ; }

	void SetChatRoomGuestListDlg( CChatRoomGuestListDlg* pDlg )	{ m_pChatRoomGuestListDlg = pDlg ; }
	CChatRoomGuestListDlg* GetChatRoomGuestListDlg()			{ return m_pChatRoomGuestListDlg ; }

	void SetChatRoomOptionDlg( CChatRoomOptionDlg* pDlg )		{ m_pChatRoomOptionDlg = pDlg ; }
	CChatRoomOptionDlg*	GetChatRoomOptionDlg()					{ return m_pChatRoomOptionDlg ; }

	void SetChatRoomJoinDlg( CChatRoomJoinDlg* pDlg )			{ m_pChatRoomJoinDlg = pDlg ; }
	CChatRoomJoinDlg*	GetChatRoomJoinDlg()					{ return m_pChatRoomJoinDlg ; }

	/*void SetChatRoomLobbyDlg( CChatRoomLobbyDlg* pDlg )			{ m_pChatRoomLobbyDlg = pDlg ; }
	CChatRoomLobbyDlg*	GetChatRoomLobbyDlg()					{ return m_pChatRoomLobbyDlg ; }*/
};


EXTERNGLOBALTON(CGameIn)
