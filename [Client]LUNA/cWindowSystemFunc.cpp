#include "stdafx.h"
#include "cWindowSystemFunc.h"

#include "WindowIDEnum.h"
#include "interface/cWindowManager.h"
#include "globaleventfunc.h"
#include "GameIn.h"
#include "CharMake.h"
#include "./Audio/MHAudioManager.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "InventoryExDialog.h"
#include "StorageDialog.h"
#include "ExchangeDialog.h"
#include "StallKindSelectDlg.h"
#include "StreetStall.h"
#include "MonsterGuageDlg.h"
#include "DebugDlg.h"
#include "ChannelDialog.h"
#include "QuickManager.h"
#include "ItemManager.h"
#include "Item.h"
#include "cSkillBase.h"
#include "ExchangeItem.h"
#include "cMsgBox.h"
#include "cImeEx.h"

#include "MainTitle.h"
#include "ServerListDialog.h"

#include "MenuSlotDialog.h"
#include "PKManager.h"

// 061214 LYW --- CharMake : Include CharMakeNewDlg's header file.
#include "CharMakeNewDlg.h"

// 061215 LYW --- CharProfile : Include profile dialog's header file.
#include "CharSelect.h"

// 061215 LYW --- Include profile dialog's header file.
#include "ProfileDlg.h"

// 061219 LYW --- Include certificate dialog's header file.
#include "CertificateDlg.h"

// 070611 LYW --- cWindowSystemFunc : Include header file of matching dialog.
#include "./DateMatchingDlg.h"

#ifdef TAIWAN_LOCAL
#include "ChinaAdviceDlg.h"
#endif

#include "QuickSlotDlg.h"
#include "petweareddialog.h"


// 
// Drag & drop Process callback Function
// 
void cbDragDropProcess(LONG curDragX, LONG curDragY, LONG id, LONG beforeDragX, LONG beforeDragY)
{
	cDialog * srcDlg = WINDOWMGR->GetWindowForID(id);
	if(!srcDlg) return;
	ASSERT(srcDlg);

	// 070611 LYW --- cWindowSystemFunc : Add check routine.
	// check favor item part.
	if( srcDlg->GetType() == WT_ICON )
	{
		if( id >= IG_FAVOR_STARTINDEX && id <= IG_MATCH_ENDINDEX )
		{
			favorDragDropProcess(curDragX, curDragY, id, beforeDragX, beforeDragY) ;
			return ;
		}
	}

	cDialog * ptDlg = WINDOWMGR->GetWindowForXYExceptIcon(beforeDragX,beforeDragY);
	cDialog * ptardlg = WINDOWMGR->GetWindowForXYExceptIcon(curDragX,curDragY);
	if(ptDlg && !ptardlg)
	{
		if( ptDlg->GetType() == WT_ITEMSHOPDIALOG )
		{
			srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);
//---임시 : 진군에게 물어보기
			if(!srcDlg->GetParent())
				return;
			if(!srcDlg->GetParent()->IsActive())
				srcDlg->SetActive(FALSE);

			srcDlg->SetDepend(TRUE);
			WINDOWMGR->SetMouseInputProcessed();
			return;
		}
	}

//	srcDlg->ExcuteCBFunc( WE_DRAGEND );
	
	if(srcDlg->GetType() & WT_ICON) // 아이콘타입이다
	{
		//////////////////////////////////////////////////////////////////////////
		// YHEDIT [8/27/2003]
		WORD wtype = srcDlg->GetType();
		if( !(wtype == WT_ITEM ||
			wtype == WT_SKILL ||
			wtype == WT_JINBUB ||
			wtype == WT_QUICKITEM ||
			wtype == WT_EXCHANGEITEM ||
			wtype == WT_STALLITEM ||
			// 070216 LYW --- cWindowSystemFunc : Check window type.
			wtype == WT_CONDUCT ||
			wtype == WT_DEALITEM) )
		{
			ASSERT(0);
		}
		cIcon* pIcon = (cIcon*)srcDlg;
		cDialog * targetdlg = WINDOWMGR->GetWindowForXYExceptIcon(curDragX,curDragY);
		if(targetdlg)
		{
			// quest 땜시... jsd(2004.09.14)			
			if( pIcon->CanMoveToDialog(targetdlg) == FALSE )
			{
				WINDOWMGR->MsgBox(MBI_QUEST_NONE, MBT_OK, CHATMGR->GetChatMsg(652));
				srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);	// SetDragEnd()가 있는데 필요한가???체크필
			}
			else
			{				
				// 080703 LYW --- cWindowSystemFunc : 단축창 확장 슬롯 추가 처리.
				//if(!targetdlg->FakeMoveIcon(curDragX,curDragY,(cIcon*)srcDlg))
				//{
				//	//되돌림
				//	//WINDOWMGR->BackDragWindow();
				//	srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);	// SetDragEnd()가 있는데 필요한가???체크필
				//	
				//}
				if(wtype == WT_QUICKITEM)
				{
					if(targetdlg->GetID() == QI1_QUICKSLOTDLG || targetdlg->GetID() == QI2_QUICKSLOTDLG)
					{
						cDialog* pSrcDialog = NULL ;
						pSrcDialog = WINDOWMGR->GetWindowForXYExceptIcon(beforeDragX, beforeDragY) ;

						if(pSrcDialog)
						{
							if(!((cQuickSlotDlg*)targetdlg)->FakeMoveIcon(pSrcDialog, curDragX,curDragY,(cIcon*)srcDlg))
							{
								srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY) ;
							}
						}
					}
					else
					{
						( ( cQuickSlotDlg* )ptDlg )->FakeDeleteQuickItem();
						// 080122 LYW --- cWindowSystemFunc : 아이템, 아이콘 버리는 사운드 수정.
						AUDIOMGR->Play(69, HERO);					
						SendDeleteIconMsg((cIcon*)srcDlg);
						srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY) ;
					}
				}
				else
				{
					if(targetdlg->GetID() == QI1_QUICKSLOTDLG || targetdlg->GetID() == QI2_QUICKSLOTDLG)
					{
						if(!((cQuickSlotDlg*)targetdlg)->FakeMoveIcon(NULL, curDragX,curDragY,(cIcon*)srcDlg))
						{
							srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY) ;
						}
					}
					else
					{
						if(!targetdlg->FakeMoveIcon(curDragX,curDragY,(cIcon*)srcDlg))
						{
							srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY) ;
						}
					}
				}


				switch(targetdlg->GetType())
				{
				case WT_QUICKDIALOG:
					{
						AUDIOMGR->Play(61, HERO);
					}
					break;
				case WT_INVENTORYDIALOG:
					break;
				default:
					{
						AUDIOMGR->Play(58, HERO);
					}
				}				
			}
		}
		else
		{
			// drop in GROUND
			// or dump icon and item, skill icon
			if( pIcon->CanDelete() )
			{
				if(GAMEIN->GetGameInInitKind() != eGameInInitKind_SuryunEnter)
				{
					if(pIcon->GetType() == WT_QUICKITEM)
					{
						( ( cQuickSlotDlg* )ptDlg )->FakeDeleteQuickItem();
						// 080122 LYW --- cWindowSystemFunc : 아이템, 아이콘 버리는 사운드 수정.
						AUDIOMGR->Play(69, HERO);
					}

					SendDeleteIconMsg((cIcon*)srcDlg);
				}
			}
			else
				WINDOWMGR->MsgBox(MBI_QUEST_NONE, MBT_OK, CHATMGR->GetChatMsg(652));

			srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);	// SetDragEnd()가 있는데 필요한가???체크필
		}
/*		if(targetdlg)
		{
			// jds quest item (2004.09.08)
			if( ((CItem*)srcDlg)->GetItemKind() != eQUEST_ITEM_EQUIP )
			{
				if(!targetdlg->FakeMoveIcon(curDragX,curDragY,(cIcon*)srcDlg))
				{
					//되돌림
					//WINDOWMGR->BackDragWindow();
					srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);	// SetDragEnd()가 있는데 필요한가???체크필
					
				}
				switch(targetdlg->GetType())
				{
				case WT_QUICKDIALOG:
					{
						AUDIOMGR->Play(61, HERO);
					}
					break;
				case WT_INVENTORYDIALOG:
					break;
				default:
					{
						AUDIOMGR->Play(58, HERO);
					}
				}
			}
			else
			{
				if( targetdlg->GetType() == WT_INVENTORYDIALOG )
				{
					if(!targetdlg->FakeMoveIcon(curDragX,curDragY,(cIcon*)srcDlg))
					{
						srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);
					}					
				}
				else
				{
					srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);
					WINDOWMGR->MsgBox(MBI_QUEST_NONE, MBT_OK, CHATMGR->GetChatMsg(652));
				}
			}				
		}
		else
		{
			// jds quest item (2004.09.08)
			if( ((CItem*)srcDlg)->GetItemKind() != eQUEST_ITEM_EQUIP )
			{
				// drop in GROUND
				// or dump icon and item, skill icon
				SendDeleteIconMsg((cIcon*)srcDlg);
				srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);	// SetDragEnd()가 있는데 필요한가???체크필
			}
			else
			{
				srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);
				WINDOWMGR->MsgBox(MBI_QUEST_NONE, MBT_OK, CHATMGR->GetChatMsg(652));
			}
		}
		*/
		//////////////////////////////////////////////////////////////////////////

		// NACK가 왔을 때 원래 자리로 되돌림
		//srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);	// SetDragEnd()가 있는데 필요한가???체크필

		ASSERT(srcDlg->GetParent());
		if(srcDlg->GetParent())		//임시다!!! 퀵아이이템 에러다. 이런경우 안생기게 수정하시오!
		{
			if(!srcDlg->GetParent()->IsActive())
				srcDlg->SetActive(FALSE);

			srcDlg->SetDepend(TRUE);
		}
		else if( srcDlg->GetType() == WT_QUICKITEM )
		{
		//	QUICKMGR->ReleaseQuickItem( (CQuickItem*)srcDlg );
		}

		WINDOWMGR->SetMouseInputProcessed();
	}
}

// 070611 LYW --- cWindowSystemFunc : Add function to processing drag and drop of favor item part.
void favorDragDropProcess(LONG curDragX, LONG curDragY, LONG id, LONG beforeDragX, LONG beforeDragY)
{
	cDialog* srcDlg = WINDOWMGR->GetWindowForID(id) ;

	// check srcDlg.
	if( !srcDlg ) return ;
	ASSERT(srcDlg) ;

	// check prev dialog and next dialog.
	cDialog* prevDlg = WINDOWMGR->GetWindowForXYExceptIcon(beforeDragX, beforeDragY) ;
	cDialog* nextDlg = WINDOWMGR->GetWindowForXYExceptIcon(curDragX, curDragY) ;

	// drop to ground.
	if( !prevDlg || !nextDlg )
	{
		// return icon.
		ReturnIconToPrevDlg(srcDlg, beforeDragX, beforeDragY) ;
		return ;
	}

	// check type of next dialog.
	LONG lPrevDlgID = prevDlg->GetID() ;
	LONG lNextDlgID = nextDlg->GetID() ;

	// check same dlg.
	if( lPrevDlgID == lNextDlgID )
	{
		// return icon.
		ReturnIconToPrevDlg(srcDlg, beforeDragX, beforeDragY) ;
		return ;
	}

	// npc image dialog.
	if( lNextDlgID == NI_DIALOG )
	{
		switch(lPrevDlgID)
		{
		case DMD_IDENTIFICATIONDLG :
		case DMD_IDT_GOOD_FEELING_ICONGRID :
		case DMD_IDT_BAD_FEELING_ICONGRID :
			{
				srcDlg->SetActive(FALSE) ;

				// delete icon.
				CIdentification* pDlg = GAMEIN->GetIdentificationDlg() ;

				if( pDlg )
				{
					pDlg->DeleteItem((CFavorItem*)srcDlg) ;
				}

				// return icon.
				ReturnIconToPrevDlg(srcDlg, beforeDragX, beforeDragY) ;
				return ;
			}
			break ;

		case DMD_FAVOR_ICON :
		case DMD_FAVOR_ICON_DLG :
			{
				// return to favor icon dialog.
				ReturnIconToPrevDlg(srcDlg, beforeDragX, beforeDragY) ;
			}
			break ;

		case NI_DIALOG :
			{
				srcDlg->SetActive(FALSE) ;
				// delete icon.
				CIdentification* pDlg = GAMEIN->GetIdentificationDlg() ;

				if( pDlg )
				{
					pDlg->DeleteItem((CFavorItem*)srcDlg) ;
				}

				// return icon.
				ReturnIconToPrevDlg(srcDlg, beforeDragX, beforeDragY) ;
				return ;
			}
			break ;

		default :
			break ;
		}
	}
	// not npc image dialog. common dialog.
	else
	{
		// return to prev dialog.
		ReturnIconToPrevDlg(srcDlg, beforeDragX, beforeDragY) ;

		// check favor icon dialog.
		if( (lPrevDlgID == DMD_FAVOR_ICON_DLG || lPrevDlgID == NI_DIALOG) && lNextDlgID == DMD_IDENTIFICATIONDLG )
		{
			nextDlg->FakeMoveIcon(curDragX, curDragY, (cIcon*)srcDlg) ;
		}
	}
}

// 070611 LYW --- cWindowSystemFunc : Add function to return icon.
void ReturnIconToPrevDlg(cDialog* srcDlg, LONG beforeDragX, LONG beforeDragY)
{
	// check parent dialog.
	if( !srcDlg->GetParent() ) return ;

	// check active.
	if( !srcDlg->GetParent()->IsActive() )
	{
		srcDlg->SetActive(FALSE) ;
	}

	// setting icon position to before position.
	srcDlg->SetAbsXY(beforeDragX, beforeDragY) ;

	srcDlg->SetDepend(TRUE) ;
	WINDOWMGR->SetMouseInputProcessed() ;
}

void SendDeleteIconMsg(cIcon * pIcon)
{
	if(pIcon->GetType() == WT_ITEM)
	{
		if( PKMGR->IsPKLooted() ) return;

		if( pIcon->GetParent() == (cObject*)( GAMEIN->GetPetWearedDlg() ) )
		{
			GAMEIN->GetPetWearedDlg()->FakeDelete( pIcon );
			GAMEIN->GetPetWearedDlg()->SetDisable( TRUE );
			WINDOWMGR->MsgBox( MBI_DELETEPETICONCHECK, MBT_YESNO, CHATMGR->GetChatMsg(209) );
		}
		else
		{
			ITEMMGR->FakeDeleteItem((CItem *)pIcon);
			// 070126 LYW --- Item : Modified message number.
			//WINDOWMGR->MsgBox( MBI_DELETEICONCHECK, MBT_YESNO, CHATMGR->GetChatMsg(363) );
			WINDOWMGR->MsgBox( MBI_DELETEICONCHECK, MBT_YESNO, CHATMGR->GetChatMsg(209) );
			GAMEIN->GetInventoryDialog()->SetDisable( TRUE );
			//		GAMEIN->GetStorageDialog()->SetDisable( TRUE );
			//	GAMEIN->GetGuildDlg()->SetDisable( TRUE );
		}
	}
	else if(pIcon->GetType() == WT_SKILL || pIcon->GetType() == WT_JINBUB )
	{
	}
//---KES 071205 사기 방지를 위해 아이템을 교환창에서 지울 수 없도록 수정
/*
	else if( pIcon->GetType() == WT_EXCHANGEITEM )
	{
		GAMEIN->GetExchangeDialog()->FakeDeleteItem(((CExchangeItem*)pIcon)->GetPosition());
		AUDIOMGR->Play(67, HERO);
	}
*/
	else if( pIcon->GetType() == WT_STALLITEM )
	{
		GAMEIN->GetStreetStallDialog()->FakeDeleteItem( ((CExchangeItem*)pIcon)->GetPosition() );
		AUDIOMGR->Play(67, HERO);
	}
	else if( pIcon->GetType() == WT_MENUICON )
	{
		GAMEIN->GetMenuSlotDialog()->FakeDeleteIcon( (CMenuIcon*)pIcon );
		AUDIOMGR->Play(67, HERO);
	}
}

//
// Window Create and WindowSystem add Func
//
// 
void CreateMainTitle_m()
{
	// 070117 LYW --- Modified bin file name.
	//cWindow* window = WINDOWMGR->GetDlgInfoFromFile("./Data/Interface/Windows/IDDlg.bin", "rb");
	cWindow* window = WINDOWMGR->GetDlgInfoFromFile("./Data/Interface/Windows/IDPass.bin", "rb");

	WINDOWMGR->AddWindow(window);

	cEditBox* pEdit;
	pEdit = (cEditBox*)((cDialog*)window)->GetWindowForID( MT_IDEDITBOX );
	if(GAMERESRCMNGR->m_GameDesc.LimitDay != 0 && GAMERESRCMNGR->m_GameDesc.LimitDay >= GetCurTimeValue())
		pEdit->SetEditText(GAMERESRCMNGR->m_GameDesc.LimitID);
	pEdit->SetValidCheck( VCM_ID );
	pEdit = (cEditBox*)((cDialog*)window)->GetWindowForID( MT_PWDEDITBOX );
	if(GAMERESRCMNGR->m_GameDesc.LimitDay != 0 && GAMERESRCMNGR->m_GameDesc.LimitDay >= GetCurTimeValue())
		pEdit->SetEditText(GAMERESRCMNGR->m_GameDesc.LimitPWD);
	pEdit->SetValidCheck( VCM_PASSWORD );

#ifndef TAIWAN_LOCAL
	cWindow* window1 = WINDOWMGR->GetDlgInfoFromFile("./Data/Interface/Windows/copyright.bin", "rb");
	WINDOWMGR->AddWindow(window1);

#else	//TAIWAN_LOCAL
	cWindow* window2 = WINDOWMGR->GetDlgInfoFromFile( "./Data/Interface/Windows/CNAdviceDlg.bin", "rb" );
	((CChinaAdviceDlg*)window2)->Linking();
	WINDOWMGR->AddWindow( window2 );
#endif
	
	// 070117 LYW --- Modified bin file name.
	//cWindow* window3 = WINDOWMGR->GetDlgInfoFromFile( "./Data/Interface/Windows/ServerListDlg.bin", "rb" );
	cWindow* window3 = WINDOWMGR->GetDlgInfoFromFile( "./Data/Interface/Windows/ServerSelect.bin", "rb" );
	WINDOWMGR->AddWindow( window3 );

	// dynamic dlg
#ifdef TAIWAN_LOCAL
	cWindow* window4 = WINDOWMGR->GetDlgInfoFromFile( "./Data/Interface/Windows/Dynamic.bin", "rb" );
	WINDOWMGR->AddWindow( window4 );
#endif

	// 080109 LYW --- cWindowManager : 숫자 패드 다이얼로그를 생성한다.
	WINDOWMGR->CreateNumberPadDlg() ; //disable
}


void NewCreateCharSelect_m()
{
	// 061215 LYW --- Change bin file.
	// 070117 LYW --- Modified bin file name.
	//cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./Data/Interface/Windows/CharSelectNewDlg.bin", "rb");
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./Data/Interface/Windows/CharSelect.bin", "rb");
	WINDOWMGR->AddWindow(window);
}

// 061218 LYW --- Add dialog for certificate some condition.
void CreateCertificateDlg()
{
	// 070116 LYW --- Modified bin file name.
	//cWindow* window = WINDOWMGR->GetDlgInfoFromFile("./Data/Interface/Windows/CertificateDlg.bin", "rb") ;
	cWindow* window = WINDOWMGR->GetDlgInfoFromFile("./Data/Interface/Windows/CharDel.bin", "rb") ;
	WINDOWMGR->AddWindow(window) ;
	CHARSELECT->SetCertificateDlg((CCertificateDlg*)window) ;
	CHARSELECT->GetCertificateDlg()->Linking() ;
}

// 061215 LYW --- Add profile dialog for character information.
void CreateProfileDlg()
{
	// 070117 LYW --- Modified bin file name.
	//cWindow* window = WINDOWMGR->GetDlgInfoFromFile("./Data/Interface/Windows/ProfileDlg.bin", "rb") ;
	cWindow* window = WINDOWMGR->GetDlgInfoFromFile("./Data/Interface/Windows/CharMakeProfile.bin", "rb") ;
	WINDOWMGR->AddWindow(window) ;
	
	CHARSELECT->SetProfileDlg((CProfileDlg*)window) ;
	CHARSELECT->GetProfileDlg()->Linking() ;
}

// 061214 LYW --- CharMake : Add new function to create dialog for character property.
void CreateCharMakeNewDlg()
{
	// 070117 LYW --- Modified bin file name.
	//cWindow* window = WINDOWMGR->GetDlgInfoFromFile( "./Data/Interface/Windows/CharMakeList.bin", "rb" ) ;
	cWindow* window = WINDOWMGR->GetDlgInfoFromFile( "./Data/Interface/Windows/CharMake.bin", "rb" ) ;
	WINDOWMGR->AddWindow( window ) ;
	
	CHARMAKE->SetCharMakeNewDlg( (CCharMakeNewDlg*)window ) ;
	CHARMAKE->GetCharMakeNewDlg()->Linking() ;

	cEditBox* pEdit = ( cEditBox* )CHARMAKE->GetCharMakeNewDlg()->GetWindowForID( CM_IDEDITBOX ) ;
	pEdit->SetValidCheck( VCM_CHARNAME ) ;
}

void CharMessageDlg_m()
{

}
cWindow * CreateGameLoading_m()
{
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./Data/Interface/Windows/NewLoadDlg.bin", "rb");
	WINDOWMGR->AddWindow(window);

	return window;
}

//KES LOGO 031107
void CreateInitLogo_m()
{
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./Data/Interface/Windows/InitDlg.bin", "rb");
	// 070201 LYW --- Change scale of title image.
	//DISPLAY_INFO	dispInfo ;

	//GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;
/*	const DISPLAY_INFO& dispInfo = GAMERESRCMNGR->GetResolution();
	cImageScale* imageScale = window->GetImageScale() ;

	switch( dispInfo.dwWidth )
	{
	case 800 :
		{
			imageScale->x = 0.79f ;
			imageScale->y = 0.79f ;
		}
		break ;

	case 1024 :
		{
			imageScale->x = 1.0f ;
			imageScale->y = 1.0f ;
		}
		break ;

	case 1280 :
		{
			imageScale->x = 1.25f ;
			imageScale->y = 1.35f ;
		}
		break ;
	}

	WINDOWMGR->AddWindow(window);*/
	if(0 == window)
	{
		return;
	}

	WINDOWMGR->AddWindow(
		window);

	SHORT_RECT rect = {0};
	WORD value = 0;
	g_pExecutive->GetRenderer()->GetClientRect(
		&rect,
		&value,
		&value);

	VECTOR2 scale = {
		float(rect.right) / window->GetWidth(),
		float(rect.bottom) / window->GetHeight()
	};

	window->SetScale(
		&scale);
	window->SetAbsXY(
		0,
		0);
}



void CreateMoneyDlg()
{
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./Data/Interface/Windows/moneyDlg.bin", "rb");
	//window->SetID();
	WINDOWMGR->AddWindow(window);
	//GAMEIN->SetMoneyDialog((CMoneyDlg * )window);
}

void CreateMonsterGuageDlg()
{
	// 070116 LYW --- Modified bin file name of target dialog.
	//cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./Data/Interface/Windows/targetmsg.bin", "rb");
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./Data/Interface/Windows/Target.bin", "rb");
	WINDOWMGR->AddWindow(window);
	GAMEIN->SetMonsterGuageDlg((CMonsterGuageDlg * )window);
	GAMEIN->GetMonsterGuageDlg()->Linking();
}

void CreateChannelDlg()
{
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./Data/Interface/Windows/Channel.bin", "rb");

	WINDOWMGR->AddWindow(window);
}

void CreateLoadingDlg(int MapNum)
{
	char filename[256];
	sprintf(filename,"./Data/Interface/Windows/Loading/%02d.bin",MapNum);
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile(filename, "rb");
	//cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./Data/Interface/Windows/%02d.bin", "rb");
	
	if(0 == window)
	{
		//cprintf( "%02d.bin loading is failed\n",MapNum );
		return;
	}

	WINDOWMGR->AddWindow(
		window);

	SHORT_RECT rect = {0};
	WORD value = 0;
	g_pExecutive->GetRenderer()->GetClientRect(
		&rect,
		&value,
		&value);

	VECTOR2 scale = {
		float(rect.right) / window->GetWidth(),
		float(rect.bottom) / window->GetHeight()
	};

	window->SetScale(
		&scale);
	window->SetAbsXY(
		0,
		0);
}