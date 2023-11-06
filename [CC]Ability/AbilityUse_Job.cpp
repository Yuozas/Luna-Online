/**************±¾×ÊÔ´À´×Ô»¥ÁªÍø ¿ª·¢ÕßÖÐÐÄÌáÊ¾½ûÉÌÒµ ½öÑ§Ï°*******************/


/*************Õ¾³¤ ÐÇÆÚ°Ë ¸ÐÐ»¹ã´óÓÃ»§¶Ô¿ª·¢ÕßÖÐÐÄÓÎÏ·×ÊÔ´ÉçÇøµÄ´óÁ¦Ö§³Ö*****************/
/**************±¾×ÊÔ´À´×Ô»¥ÁªÍø ¿ª·¢ÕßÖÐÐÄÌáÊ¾½ûÉÌÒµ ½öÑ§Ï°*******************/


/*************Õ¾³¤ ÐÇÆÚ°Ë ¸ÐÐ»¹ã´óÓÃ»§¶Ô¿ª·¢ÕßÖÐÐÄÓÎÏ·×ÊÔ´ÉçÇøµÄ´óÁ¦Ö§³Ö*****************/
// AbilityUse_Job.cpp: implementation of the CAbilityUse_Job class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AbilityUse_Job.h"

#include "UngiJosikManager.h"
#ifdef _MHCLIENT_
#include "GameIn.h"
#include "MainBarDialog.h"
#include "UpgradeDlg.h"
#include "MixDialog.h"
#include "ReinforceDlg.h"
#include "DissolutionDialog.h"


#include "AbilityManager.h"
#include "ObjectManager.h"
#include "ExchangeManager.h"
#include "ChatManager.h"
#include "VimuManager.h"
#include "StreetStallManager.h"
#include "StallFindDlg.h"

#include "Battle.h"
#include "BattleSystem_Client.h"
#include "./input/UserInput.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "cMsgBox.h"
#include "StreetStall.h"
#include "StreetBuyStall.h"
#include "AutoNoteDlg.h"
#include "PartyMatchingDlg.h"
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CAbilityUse_Job::CAbilityUse_Job()
{

}

CAbilityUse_Job::~CAbilityUse_Job()
{

}


void CAbilityUse_Job::Use(BYTE Level,CAbilityInfo* pAbilityInfo)
{
#ifdef _MHCLIENT_
	ySWITCH(pAbilityInfo->GetInfo()->Ability_effect_Param1)
		yCASE(eAUKJOB_Ungijosik) // Â¿Ã®Â±Ã¢?Â¶Â½?
			UNGIMGR->ToggleHero_UngiMode();
		yCASE(eAUKJOB_Vimu) 	 // ÂºÃ±Â¹Â«Â½??Â»
			if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
				GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
				return;
			if(ABILITYMGR->CheckAbilityLevel(ABILITYINDEX_VIMU,HERO->GetAbilityGroup(),1) == FALSE)
			{
				CHATMGR->AddMsg( CTC_ABILITYUPGRADE_FAILED, CHATMGR->GetChatMsg( 139 ));
			}
			else
			{
				VIMUMGR->ApplyVimu();


			}
		yCASE(eAUKJOB_Party) 	 // Â¹Ã¦???Â¢Â¼Â³
			//Â¹Ã¦??Â°Â¡ Â¾Ã¸?Â»Â½?Â¿Â¡Â¸Â¸??Â°Â¡? Â¾?Â´?Â¸Ã© Â´???Â¾Ã³Â·?Â±? Â²Ã´Â°Ã­ ?Â°Â´?Â°?ÂµÂµ Âµ?Â´?Â°?Â°Â¡? 
			// -- Â¹Ã¦??Â°Â¡ Â¾Ã¸?Â» Â¶Â§Â¸Â¸??Â´?Â´? by hs
			if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
				GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
				return;
			CPartyDialog* pDlg = GAMEIN->GetPartyDialog();

			if( !pDlg ) return;
			pDlg->SetActive( TRUE );

			
		yCASE(eAUKJOB_Guild) 	 // Â¹Â®???Â¢Â¼Â³		//passive skill??ÂµÃ­.
			if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
				GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)

				return;
		yCASE(eAUKJOB_StreetStall)// Â³Ã«?Â¡Â»Ã³Â°Â³Â¼Â³
			// ?„ìž¬ ?¸ì? ê²€?‰ì??´ë©´ ?¸ì????´ê¸° ë¶ˆê? by Stiner(8)

			if( GAMEIN->GetStallFindDlg()->IsActive() == TRUE )
			{
				WINDOWMGR->MsgBox( MBI_OK, MBT_OK, CHATMGR->GetChatMsg(1697) );
				return;
			}
			
			if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
				GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
				return;
			
			////////////////////////////////////////////////////////////////////////////////
			// 06. 06. 2ì°??„ì? - ?´ì?ì¤€
			// ?€??/ ?œì?
			// ?€???¨ë???

			if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1329) );
				return;
			}
			////////////////////////////////////////////////////////////////////////////////
			if(ABILITYMGR->CheckAbilityLevel(ABILITYINDEX_STALL,HERO->GetAbilityGroup(),1) == FALSE)
			{
				CHATMGR->AddMsg( CTC_ABILITYUPGRADE_FAILED, CHATMGR->GetChatMsg( 139 ));

			}
			else
			{


//				CHATMGR->AddMsg( CTC_SYSMSG, "Â³Ã«?Â¡Â»Ã³ Â±Ã¢Â´??? ??Â½??Ã»?Â¸Â·? ??Â´?Âµ?Â¾ÃºÂ½?Â´?Â´?. ??Â¿Ã«Â¿Â¡ Âº??Ã­?Â» ÂµÃ¥Â·? ??Â¼???Â´?Â´?." );


				if( ( HERO->GetState() == eObjectState_None || HERO->GetState() == eObjectState_Immortal )
					&& !VIMUMGR->IsVimuing() )
				{

					STREETSTALLMGR->ChangeDialogState();
				}


			}
 
		yCASE(eAUKJOB_Deal) 	 // Â±Â³?Â¯	
			if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
				GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
				return;
			if(ABILITYMGR->CheckAbilityLevel(ABILITYINDEX_EXCHANGE,HERO->GetAbilityGroup(),1) == FALSE)
			{
				CHATMGR->AddMsg( CTC_ABILITYUPGRADE_FAILED, CHATMGR->GetChatMsg( 139 ));
			}
			else
			{
				EXCHANGEMGR->ApplyExchange();

			}

		yCASE(eAUKJOB_Upgrade) 	 // ÂµÃ®Â±?Â¾Ã·	
			if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
				GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
				return;
			if( HERO->GetState() != eObjectState_Die )
			{
				if( GAMEIN->GetUpgradeDialog()->IsActive() && 

					!GAMEIN->GetUpgradeDialog()->IsDisable() )
				{
					GAMEIN->GetUpgradeDialog()->Release();
					GAMEIN->GetUpgradeDialog()->SetActiveRecursive( FALSE );					
				}
				else
				{
					GAMEIN->GetUpgradeDialog()->SetActiveRecursive(TRUE);
				}
			}

		yCASE(eAUKJOB_Mix) 	 // ?Â¶??	
			if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
				GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
				return;
			if( HERO->GetState() != eObjectState_Die )
			{
				if( GAMEIN->GetMixDialog()->IsActive() &&
					!GAMEIN->GetMixDialog()->IsDisable() )
				{
					GAMEIN->GetMixDialog()->Release();
					GAMEIN->GetMixDialog()->SetActiveRecursive( FALSE );

				}
				else
				{
					GAMEIN->GetMixDialog()->SetActiveRecursive(TRUE);
				}

			}
		yCASE(eAUKJOB_Reinforce) 	 // Â°Â­?Â­	

			if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
				GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
				return;
			if( HERO->GetState() != eObjectState_Die )
			{
				if( GAMEIN->GetReinforceDialog()->IsActive() &&
					!GAMEIN->GetReinforceDialog()->IsDisable() )
				{
					GAMEIN->GetReinforceDialog()->Release();
					GAMEIN->GetReinforceDialog()->SetActiveRecursive( FALSE );
				}
				else
				{
					GAMEIN->GetReinforceDialog()->SetActiveRecursive(TRUE);
				}				
			}
		yCASE(eAUKJOB_Dissolve) 	 // Âº???
			if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
				GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)

				return;
			
			if( HERO->GetState() != eObjectState_Die )
			{
				if( GAMEIN->GetDissolutionDialog()->IsActive() &&
					!GAMEIN->GetDissolutionDialog()->IsDisable() )
				{
					GAMEIN->GetDissolutionDialog()->SetActive( FALSE );
				}
				else
				{
					GAMEIN->GetDissolutionDialog()->SetActive(TRUE);
				}				
			}
		yCASE(eAUKJOB_Restraint)

			if( BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() != eBATTLE_KIND_SIEGEWAR )
				return;

			MSGBASE msg;
			SetProtocol( &msg, MP_SIEGEWAR, MP_SIEGEWAR_RESTRAINT_SYN );
			msg.dwObjectID = HEROID;
			NETWORK->Send( &msg, sizeof(msg) );
		yCASE(eAUKJOB_Attack)
			g_UserInput.GetHeroMove()->AttackSelectedObject();

		yCASE(eAUKJOB_StallFind)
			// ?¸ì??ì´ ?´ë ¤?ˆì??Œë? ê²€??ë¶ˆê? by Stiner(8)
			if( GAMEIN->GetStreetStallDialog()->IsActive() == TRUE 
				|| GAMEIN->GetStreetBuyStallDialog()->IsActive() == TRUE
				|| GAMEIN->GetStallKindSelectDialog()->IsActive() == TRUE )

			{
				WINDOWMGR->MsgBox( MBI_OK, MBT_OK, CHATMGR->GetChatMsg(1696) );

				return;
			}

			// ?¸ì???ê²€?‰ì°½ ?„ìš°ê¸?by Stiner(8)
			GAMEIN->GetStallFindDlg()->SetActive(TRUE);
		yCASE(eAUKJOB_PartyFind)	//2008. 6. 4. CBH - ë°©í? ì°¾ê¸° ì¶”ê?)
			if( HERO->GetState() != eObjectState_Die )

			{
				GAMEIN->GetPartyMatchingDlg()->SetActive(TRUE);
			}

		yCASE(eAUKJOB_AutoNote)	// AutoNote
			GAMEIN->GetAutoNoteDlg()->SetActive(TRUE);

		yENDSWITCH
#endif
}




