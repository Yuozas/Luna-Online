//=================================================================================================
//	FILE		: CMainSystemDlg.cpp
//	PURPOSE		: Implementation part for main system dialog.
//	PROGRAMMER	: Yongs Lee
//	DATE		: December 19, 2006
//	ADDITION	:
//	ATTENTION	:
//=================================================================================================


//=================================================================================================
//	INCLUDE PART
//=================================================================================================
#include "stdafx.h"
#include ".\mainsystemdlg.h"
#include "WindowIDEnum.h"

#include "./interface/cTabDialog.h"

#include "GameIn.h"

#include "CharacterDialog.h"
#include "QuestDialog.h"
#include "ChatManager.h"
#include "ExitManager.h"
#include "FriendManager.h"
#include "GameEventManager.h"
#include "ObjectManager.h"
#include "NoteManager.h"
#include "cWindowManager.h"

// 070402 LYW --- Include cButton.
#include "./interface/cButton.h"

#include "HelpDialog.h"

// 070112 LYW --- Include header files to need.
#include "cMsgBox.h"

// 070115 LYW --- Include header file tp need.
#include "../Input/Mouse.h"

// 070122 LYW --- Include header file.
#include "cResourceManager.h"

#include "../DateMatchingDlg.h"
#include "../FamilyDialog.h"

// 071025 LYW --- MainSystemDlg : Include helperdlg.
#include "HelperDlg.h"

// 080403 LYW --- MainSystemDlg : Include chatroom manager.
#include "./ChatRoomMgr.h"
#include <Wincrypt.h>
/*
#include <atlbase.h>    
#include <atlcom.h>     
#include <atlhost.h>    
#include <Exdispid.h>

#import "shdocvw.dll"
*/

//---BROWSWER
#include "ItemShopDlg.h"

extern int g_nServerSetNum;
extern HWND _g_hWnd;
/*
class DWebBrowserEventsImpl : public DWebBrowserEvents
{
	// IUnknown methods
    STDMETHOD(QueryInterface)(REFIID riid, LPVOID* ppv)
	{
		*ppv = NULL;

		if (IID_IUnknown == riid || __uuidof(SHDocVw::DWebBrowserEventsPtr) == riid)
		{
			*ppv = (LPUNKNOWN)(SHDocVw::DWebBrowserEventsPtr*)this;
		}
		else if (IID_IOleClientSite == riid)
		{
			*ppv = (IOleClientSite*)this;
		}
		else if (IID_IDispatch == riid)
		{
			*ppv = (IDispatch*)this;
		}
		else
		{
			return E_NOTIMPL;
		}

		AddRef();

		return NOERROR;
	}

    STDMETHOD_(ULONG, AddRef)() { return 1; }
    STDMETHOD_(ULONG, Release)() { return 0; }

	// IDispatch methods
	STDMETHOD(GetTypeInfoCount)(UINT* pctinfo) { return E_NOTIMPL; }
	STDMETHOD(GetTypeInfo)(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo)  { return E_NOTIMPL; }
	STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId)  { return E_NOTIMPL; }
	STDMETHOD(Invoke)(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS __RPC_FAR *pDispParams, VARIANT __RPC_FAR *pVarResult, EXCEPINFO __RPC_FAR *pExcepInfo, UINT __RPC_FAR *puArgErr)
	{
		switch (dispIdMember)
		{
		case DISPID_BEFORENAVIGATE:
			{
				BeforeNavigate( _bstr_t( pDispParams->rgvarg[5].bstrVal ),
					0,
					_bstr_t( pDispParams->rgvarg[3].bstrVal ),
					NULL,
					_bstr_t(""),
					NULL);

				break;

			}
		case DISPID_NAVIGATECOMPLETE:
			{
				NavigateComplete(_bstr_t( pDispParams->rgvarg[3].bstrVal ));
				break;
			}
		}

		return NOERROR;
	}

    // Methods:
    HRESULT BeforeNavigate (_bstr_t URL, long Flags, _bstr_t TargetFrameName, VARIANT * PostData, _bstr_t Headers, VARIANT_BOOL * Cancel)
	{
		//MessageBox(NULL, (char*)URL, "BeforeNavigate", MB_OK);
		return S_OK;
	}
    HRESULT NavigateComplete ( _bstr_t URL ) 
	{ 
		//MessageBox(NULL, (char*)URL, "NavigateComplete", MB_OK);
		return S_OK; 
	}
    HRESULT StatusTextChange ( _bstr_t Text ) { return S_OK; }
    HRESULT ProgressChange (long Progress, long ProgressMax ) { return S_OK; }
    HRESULT DownloadComplete() { return S_OK; }
    HRESULT CommandStateChange (long Command, VARIANT_BOOL Enable ) { return S_OK; }
    HRESULT DownloadBegin () { return S_OK; }
    HRESULT NewWindow (_bstr_t URL, long Flags, _bstr_t TargetFrameName, VARIANT * PostData, _bstr_t Headers, VARIANT_BOOL * Processed ) { return S_OK; }
    HRESULT TitleChange ( _bstr_t Text ) { return S_OK; }
    
	HRESULT FrameBeforeNavigate (_bstr_t URL, long Flags, _bstr_t TargetFrameName, VARIANT * PostData, _bstr_t Headers, VARIANT_BOOL * Cancel ) { return S_OK; }
    HRESULT FrameNavigateComplete (_bstr_t URL ) { return S_OK; }
    HRESULT FrameNewWindow (_bstr_t URL, long Flags, _bstr_t TargetFrameName, VARIANT * PostData, _bstr_t Headers, VARIANT_BOOL * Processed ) { return S_OK; }
    
	HRESULT Quit (VARIANT_BOOL * Cancel ) { return S_OK; }
    
	HRESULT WindowMove ( ) { return S_OK; }
    HRESULT WindowResize ( ) { return S_OK; }
    HRESULT WindowActivate ( ) { return S_OK; }
    
	HRESULT PropertyChange (_bstr_t Property ) { return S_OK; }
};
*/

//=================================================================================================
// NAME			: CMainSystemDlg()
// PURPOSE		: The Function Constructor.
// ATTENTION	:
//=================================================================================================
CMainSystemDlg::CMainSystemDlg(void)
{
	// 070115 LYW --- Delete this line.
	//m_type = WT_MAINSYSTEM_DLG ;

	// 070115 LYW --- Add variable for count.
	int count = 0 ;

	// 070115 LYW --- Add menu button.
	for( count = 0 ; count < MAX_MENU_BTN ; ++count )
	{
		m_pMenuBtn[ count ] = NULL ;
	}

	// 070115 LYW --- Add static controls for background image of sub dialog.
	m_pSystemSet		= NULL ;
	m_pGameSystem		= NULL ;

	// 070115 LYW --- Add controls for systemset part.
	for( count = 0 ; count < MAX_SYSTEMSET_BTN ; ++count )
	{
		m_pMenuSystemSet[ count ] = NULL ;
	}
	m_pTitleSystemSet		= NULL ;
	m_pMarkSystemSet		= NULL ;

	// 070115 LYW --- Add controls for gamesystem part.
	for( count = 0 ; count < MAX_GAMESYSTEM_BTN ; ++count )
	{
		m_pMenuGameSystem[ count ] = NULL ;
	}
	m_pTitleGameSystem		= NULL ;
	m_pMarkGameSystem		= NULL ;

	// 070115 LYW --- Add variables whether show sub dialog or not.
	m_bShowSystemSet	= FALSE ;
	m_bShowGameSystem	= FALSE ;

	// 080403 LYW --- MainSystemDlg : Initialize the button to call main dialog of chatroom system.
	//m_pChatRoom = NULL ;
}


//=================================================================================================
// NAME			: ~CMainSystemDlg()
// PURPOSE		: The Function Destructor.
// ATTENTION	:
//=================================================================================================
CMainSystemDlg::~CMainSystemDlg(void)
{
}


//=================================================================================================
//	NAME		: Linking()
//	PURPOSE		: Add function to linking controls to window manager.
//	DATE		: January 15, 2007 LYW
//	ATTENTION	:
//=================================================================================================
void CMainSystemDlg::Linking()
{
	int count = 0 ;

	for( count = 0 ; count < MAX_MENU_BTN ; ++count )
	{
		m_pMenuBtn[ count ] = ( cButton* )GetWindowForID( MSD_MB_HELPER + count ) ;
	}

	m_pSystemSet		= ( cStatic* )GetWindowForID( MSD_BG_SYSTEMSET ) ;

	m_pGameSystem		= ( cStatic* )GetWindowForID( MSD_BG_GAMESYSTEM ) ;

	for( count = 0 ; count < MAX_SYSTEMSET_BTN ; ++count )
	{
		m_pMenuSystemSet[ count ] = ( cButton* )GetWindowForID( MSD_SSBTN_GAME + count ) ;
		m_pMenuSystemSet[ count ]->SetActive( FALSE ) ;
	}
	m_pTitleSystemSet		= ( cStatic* )GetWindowForID( MSD_TITLE_SYSTEMSET ) ;
	// 070118 LYW --- Delete this line.
	//m_pTitleSystemSet->SetStaticText( "시스템 설정창" ) ;

	m_pMarkSystemSet		= ( cStatic* )GetWindowForID( MSD_MARK_SYSTEMSET ) ;

	for( count = 0 ; count < MAX_GAMESYSTEM_BTN ; ++count )
	{
		m_pMenuGameSystem[ count ] = ( cButton* )GetWindowForID( MSD_GSBTN_CHARINFO + count ) ;
		m_pMenuGameSystem[ count ]->SetActive( FALSE ) ;
	}
	m_pTitleGameSystem		= ( cStatic* )GetWindowForID( MSD_TITLE_GAMESYSTEM ) ;
	// 070118 LYW --- Delete this line.
	//m_pTitleGameSystem->SetStaticText("게임 시스템") ;

	m_pMarkGameSystem		= ( cStatic* )GetWindowForID( MSD_MARK_GAMESYSTEM ) ;

	//// 080403 LYW --- MainSystemDlg : Add a button to call main dialog of chatroom system.
	//m_pChatRoom = (cButton*)GetWindowForID(MSD_GSBTN_CHAT) ;

	//ASSERT(m_pChatRoom) ;

	//if(!m_pChatRoom)
	//{
	//	MessageBox(	NULL, "Failed to create button", "Failed to create chatroom button.", MB_OK) ;
	//	return ;
	//}
}


//=================================================================================================
//	NAME		: ActionEvent()
//	PURPOSE		: Add function ActionEvent.
//	DATE		: January 15, 2007 LYW
//	ATTENTION	:
//=================================================================================================
DWORD CMainSystemDlg::ActionEvent( CMouse* mouseInfo )
{
	DWORD we = WE_NULL ;

	we |= cDialog::ActionEvent( mouseInfo ) ;

	return we ;
}


//=================================================================================================
// NAME			: OnActionEvent()
// PURPOSE		: The function for linking controls to window manager.
// ATTENTION	:
//=================================================================================================
void CMainSystemDlg::OnActionEvent(LONG lId, void* p, DWORD we )
{
	cDialog* pHandler = NULL ;

	// 070115 LYW --- Delete this part.
	/*
	if( we & WE_PUSHDOWN || we & WE_PUSHUP )
	{
		switch( lId )
		{
		case MSD_MB_GAMESYSTEM :
			{
				if( GetTabSheet(0)->IsActive() )
				{
					GetTabSheet(0)->SetActive( FALSE ) ;
				}
				else
				{
					GetTabSheet(0)->SetActive( TRUE ) ;
					GetTabSheet(1)->SetActive( FALSE ) ;
				}
			}
			break ;

		case MSD_MB_SYSTEMSET :			
			{
				if( GetTabSheet(1)->IsActive() )
				{
					GetTabSheet(1)->SetActive( FALSE ) ;
				}
				else
				{
					GetTabSheet(1)->SetActive( TRUE ) ;
					GetTabSheet(0)->SetActive( FALSE ) ;
				}
			}
			break ;
		}
	}
	*/
	// 070115 LYW --- Modified this line.
	//else if( we & WE_BTNCLICK )
	if( we & WE_BTNCLICK )
	{
		switch( lId )
		{
			// 070115 LYW --- Add two part.
		case MSD_MB_GAMESYSTEM :
			{
				if( m_bShowGameSystem )
				{
					m_bShowGameSystem = FALSE ;
				}
				else
				{
					m_bShowGameSystem = TRUE ;
				}

				SetGameSystem() ;

				// 070115 LYW --- Close other sub dialog.
				if( m_bShowSystemSet )
				{
					m_bShowSystemSet = FALSE ;
					SetSystemSet() ;
				}
			}
			break ;

		case MSD_MB_SYSTEMSET :			
			{
				if( m_bShowSystemSet )
				{
					m_bShowSystemSet = FALSE ;
				}
				else
				{
					m_bShowSystemSet = TRUE ;
				}

				SetSystemSet() ;

				// 070115 LYW --- Close other sub dialog.
				if( m_bShowGameSystem )
				{
					m_bShowGameSystem = FALSE ;
					SetGameSystem() ;
				}
			}
			break ;

		case MSD_MB_HELPER :
			{
				if( GAMEIN->GetItemShopDlg() == NULL ) break;
				if( GAMEIN->GetItemShopDlg()->IsActive() )
				{
					GAMEIN->GetItemShopDlg()->SetActive( FALSE );
					break;
				}

				char csBuffer[1024];
				char csDigest[1024];

				sprintf( csBuffer, "%d%d%s%s%s",
				g_nServerSetNum,
				HEROID,
				g_szHeroIDName,
				HERO->GetObjectName(),
				"fnsk@hsfk@ls!!"
				);

/*
				sprintf( csBuffer, "%d%d%s%s%s",
				1,
				114,
				"payletter2",
				"payletter",
				"fnsk@hsfk@ls!!"
				);
*/
				HCRYPTPROV hCryptProv; 
				HCRYPTHASH hHash; 
				BYTE bHash[0x7f]; 
				DWORD dwHashLen= 16; // The MD5 algorithm always returns 16 bytes. 
				DWORD cbContent= strlen(csBuffer);//csBuffer.GetLength(); 
				BYTE* pbContent= (BYTE*)csBuffer;//csBuffer.GetBuffer(cbContent); 


				if(CryptAcquireContext(&hCryptProv, 
					NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET)) 
				{

					if(CryptCreateHash(hCryptProv, 
						CALG_MD5,	// algorithm identifier definitions see: wincrypt.h
						0, 0, &hHash)) 
					{
						if(CryptHashData(hHash, pbContent, cbContent, 0))
						{

							if(CryptGetHashParam(hHash, HP_HASHVAL, bHash, &dwHashLen, 0)) 
							{
								// Make a string version of the numeric digest value
								//csDigest.Empty();
								memset( csDigest, 0, sizeof(char)*1024 );
								//CString tmp;
								char tmp[32];
								for (int i = 0; i<16; i++)
								{
									//tmp.Format("%02x", bHash[i]);
									sprintf( tmp, "%02x", bHash[i] );
									strcat( csDigest, tmp );//csDigest+=tmp;
								}

							}
							//else csDigest=_T("Error getting hash param"); 

						}
						//else csDigest=_T("Error hashing data"); 
					}
					//else csDigest=_T("Error creating hash"); 

				}
				//else csDigest=_T("Error acquiring context"); 


				CryptDestroyHash(hHash); 
				CryptReleaseContext(hCryptProv, 0); 
				//csBuffer.ReleaseBuffer();

				char URL[1024];

				sprintf( URL, "%s%s%d%s%d%s%s%s%s%s%s",
					"http://www.mengluna.com/bill.asp?",
					"server_index=", g_nServerSetNum,
					"&m_idPlayer=", HEROID,
					"&m_szName=", HERO->GetObjectName(),
					"&user_id=", g_szHeroIDName,
					"&md5=", csDigest
					);

/*
				sprintf( URL, "%s%s%d%s%d%s%s%s%s%s%s",
					"http://bill3.omg.com.tw/charge/luna/lunaitemlist.asp?",
					"server_index=", 1,
					"&m_idPlayer=", 114,
					"&m_szName=", "payletter",
					"&user_id=", "payletter2",
					"&md5=", csDigest
					);
*/
			
				if( GAMEIN->GetItemShopDlg() )
				{
					GAMEIN->GetItemShopDlg()->Navigate( URL );
					GAMEIN->GetItemShopDlg()->SetActive( TRUE );				
				}


/*
				//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 528 ) );
				//break;

				//CString csBuffer;
				char csBuffer[1024];
				char csDigest[1024];

				sprintf( csBuffer, "%d%d%s%s%s",
				g_nServerSetNum,
				gUserID,
				g_szHeroIDName,
				HERO->GetObjectName(),
				"fnsk@hsfk@ls!!"
				);

//				sprintf( csBuffer, "%d%d%s%s%s",
//				1,
//				114,
//				"payletter2",
//				"payletter",
//				"fnsk@hsfk@ls!!"
//				);

				HCRYPTPROV hCryptProv; 
				HCRYPTHASH hHash; 
				BYTE bHash[0x7f]; 
				DWORD dwHashLen= 16; // The MD5 algorithm always returns 16 bytes. 
				DWORD cbContent= strlen(csBuffer);//csBuffer.GetLength(); 
				BYTE* pbContent= (BYTE*)csBuffer;//csBuffer.GetBuffer(cbContent); 


				if(CryptAcquireContext(&hCryptProv, 
					NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET)) 
				{

					if(CryptCreateHash(hCryptProv, 
						CALG_MD5,	// algorithm identifier definitions see: wincrypt.h
						0, 0, &hHash)) 
					{
						if(CryptHashData(hHash, pbContent, cbContent, 0))
						{

							if(CryptGetHashParam(hHash, HP_HASHVAL, bHash, &dwHashLen, 0)) 
							{
								// Make a string version of the numeric digest value
								//csDigest.Empty();
								memset( csDigest, 0, sizeof(char)*1024 );
								//CString tmp;
								char tmp[32];
								for (int i = 0; i<16; i++)
								{
									//tmp.Format("%02x", bHash[i]);
									sprintf( tmp, "%02x", bHash[i] );
									strcat( csDigest, tmp );//csDigest+=tmp;
								}

							}
							//else csDigest=_T("Error getting hash param"); 

						}
						//else csDigest=_T("Error hashing data"); 
					}
					//else csDigest=_T("Error creating hash"); 

				}
				//else csDigest=_T("Error acquiring context"); 


				CryptDestroyHash(hHash); 
				CryptReleaseContext(hCryptProv, 0); 
				//csBuffer.ReleaseBuffer();

				char URL[1024];

				sprintf( URL, "%s%s%d%s%d%s%s%s%s%s%s",
					"http://bill3.omg.com.tw/charge/luna/lunaitemlist.asp?",
					"server_index=", g_nServerSetNum,
					"&m_idPlayer=", gUserID,
					"&m_szName=", g_szHeroIDName,
					"&user_id=", HERO->GetObjectName(),
					"&md5=", csDigest
					);
			
				DWebBrowserEventsImpl	m_events;
				CComQIPtr<IWebBrowser2>  m_pWebBrowser;
				
				POINT pt;
				pt.x = pt.y = 0;
				ClientToScreen(_g_hWnd, &pt);

				RECT rt;
				GetClientRect(_g_hWnd, &rt);

				int x = ( ( rt.right - rt.left ) / 2 - 400 );
				int y = ( ( rt.bottom - rt.top ) / 2 - 300 );

				HWND hwndIE = CreateWindow("AtlAxWin71", 
				"http://www.microsoft.com", WS_POPUP|WS_CHILD|WS_VISIBLE|WS_SYSMENU|WS_CAPTION, 
				x, y, 800, 600, 
				_g_hWnd, (HMENU)0, GetModuleHandle(NULL), NULL); 

//				HWND hwndIE = CreateWindow("AtlAxWin71", "ItemShop", WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN, 
//					x, y, 800, 570, _g_hWnd, (HMENU)0, GetModuleHandle(NULL), NULL); 

				if( !hwndIE )
				{
					return;
				}

				CComPtr<IUnknown>    punkIE; 

				if (AtlAxGetControl(hwndIE, &punkIE) == S_OK) 
				{ 
					m_pWebBrowser = punkIE; 

					if( m_pWebBrowser ) 
					{ 
						LPCONNECTIONPOINTCONTAINER pCPC = NULL;
						LPCONNECTIONPOINT pCP = NULL;

						m_pWebBrowser->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pCPC);
						pCPC->FindConnectionPoint(__uuidof(SHDocVw::DWebBrowserEventsPtr), &pCP);

						DWORD dwCookie;
						pCP->Advise((LPUNKNOWN)&m_events, &dwCookie);

						pCPC->Release();

						// 메뉴같은 것들을 안나오게 한다.
						//m_pWebBrowser->put_FullScreen(VARIANT_TRUE); 

						// 초기에 뜰 웹사이트를 지정할 수 있다. 
						//char pszBuf[ 256 ] = "http://www.naver.com";
						CComVariant vempty, vUrl(URL);

						m_pWebBrowser->Navigate( _bstr_t(URL), &vempty, &vempty, &vempty, &vempty);             

						pCP->Unadvise(dwCookie);
						pCP->Release();
					} 
					else 
					{ 
						return; 
					} 
				} 
				else 
				{ 
					return; 
				} 
*/

/*
				sprintf( URL, "%s%s%d%s%d%s%s%s%s%s%s",
					"http://bill3.omg.com.tw/charge/luna/lunaitemlist.asp?",
					"server_index=", 1,
					"&m_idPlayer=", 114,
					"&m_szName=", "payletter",
					"&user_id=", "payletter2",
					"&md5=", csDigest
					);
*/
/*				ShellExecute( 
					0, 
					"open", 
					URL,
					0,
					0,
					SW_SHOWNORMAL );
*/				
/*				// 080801 LUJ, 아이템몰로 연결한다
				ShellExecute( 
					0, 
					"open", 
					"explorer",
					"http://luna.eyainteractive.com/item_zone/shop_zone/itemMall.asp",
					0,
					SW_SHOWNORMAL );
*/			}
			break ;

		case MSD_MB_ITEMMALL :
			{
				pHandler = (cDialog*)GAMEIN->GetMallNoticeDialog();
				if( !pHandler ) return;

				if( pHandler->IsActive() )
				{
					pHandler->SetActive( FALSE );
				}
				else
				{
					pHandler->SetActive( TRUE );
				}

				// 070115 LYW --- Close other sub dialog.
				if( m_bShowSystemSet )
				{
					m_bShowSystemSet = FALSE ;
					SetSystemSet() ;
				}
				if( m_bShowGameSystem )
				{
					m_bShowGameSystem = FALSE ;
					SetGameSystem() ;
				}
			}
			break ;

		case MSD_GSBTN_CHARINFO : 
			{
				pHandler = GAMEIN->GetCharacterDialog();
				if( !pHandler )	return;

				if( pHandler->IsActive() )
				{
					pHandler->SetActive( FALSE );

					// 070115 LYW --- Close tree dialog.
					GAMEIN->GetCharacterDialog()->SetViewTreeDlg( FALSE ) ;
				}
				else 
				{
					((CCharacterDialog*)pHandler)->UpdateData();
					pHandler->SetActive( TRUE );	
					
					GAMEEVENTMGR->AddEvent( eGameEvent_CharInterface );
				}
			}
			break ;

		case MSD_GSBTN_SKILL : 
			{
				pHandler = (cDialog*)GAMEIN->GetSkillTreeDlg();
				if( !pHandler ) return ;

				if( pHandler->IsActive() )
				{
					//ANIMGR->StartFade(ANI_FADE_OUT, 255, 0, 50, pHandler );
					pHandler->SetActive( FALSE );
				}
				else 
				{
					//ANIMGR->StartFade(ANI_FADE_IN, 0, 255, 100, pHandler );
					pHandler->SetActive( TRUE );

					GAMEEVENTMGR->AddEvent( eGameEvent_SkillInterface );
				}
			}
			break ;

		case MSD_GSBTN_QUEST : 
			{
				pHandler = (cDialog*)GAMEIN->GetQuestTotalDialog();
				if( !pHandler ) return ;

				if( pHandler->IsActive() )
				{
					pHandler->SetActive(FALSE);
				}
				else
				{
					// 080103 LYW --- MainSystemDlg : 퀘스트 트리가 지워지는 원인이 되는 함수 삭제.
					//GAMEIN->GetQuestDialog()->CheckQuestDate();
					
					pHandler->SetActive(TRUE);

					GAMEEVENTMGR->AddEvent( eGameEvent_QuestInterface );
				}
			}
			break ;

		case MSD_GSBTN_GUILD : 
			{
				pHandler =  (cDialog*)GAMEIN->GetGuildDlg();
				if( !pHandler ) return ;
				if( pHandler->IsActive() )
				{
					pHandler->SetActive(FALSE);
				}
				else
				{
					if(HERO->GetGuildIdx() != 0)
						pHandler->SetActive(TRUE);
					else
						// 070330 LYW --- MainSystemDlg : Modified message number of guild part.
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 578 ));

					GAMEEVENTMGR->AddEvent( eGameEvent_MunpaInterface );
				}
			}
			break ;

		case MSD_GSBTN_FRIEND : 
			{
				pHandler = (cDialog*)GAMEIN->GetFriendDialog();
				if( !pHandler ) return ;
				if( pHandler->IsActive() )
				{
					pHandler->SetActive(FALSE);
				}
				else
				{
					FRIENDMGR->FriendListSyn(1);

					GAMEEVENTMGR->AddEvent( eGameEvent_FriendInterface );
				}
			}
			break ;

		case MSD_GSBTN_NOTE : 
			{
				pHandler = (cDialog*)GAMEIN->GetNoteDialog();
				if( !pHandler ) return ;
				if( pHandler->IsActive() )
				{
					pHandler->SetActive(FALSE);
				}
				else
				{
					NOTEMGR->NoteListSyn(1);

					GAMEEVENTMGR->AddEvent( eGameEvent_NoteInterface );
				}
			}
			break ;

		case MSD_GSBTN_INVENTORY : 
			{
				pHandler = (cDialog*)GAMEIN->GetInventoryDialog();
				if( !pHandler ) return ;
				if( pHandler->IsActive() )
				{
					pHandler->SetActive( FALSE );
				}
				else 
				{
					pHandler->SetActive( TRUE );

					GAMEEVENTMGR->AddEvent( eGameEvent_InvenInterface );
				}
			}
			break ;

			// 070618 LYW --- MainSystemDlg : Add matching menu.
		case MSD_GSBTN_MATCHING :
			{
				/*pHandler = (cDialog*)GAMEIN->GetDateMatchingDlg();
				if( !pHandler ) return ;
				if( pHandler->IsActive() )
				{
					pHandler->SetActive( FALSE );
				}
				else 
				{
					pHandler->SetActive( TRUE );

					GAMEEVENTMGR->AddEvent( eGameEvent_MatchingInterface );
				}*/

				// check identification.
				const DATE_MATCHING_INFO& dateInfo = HERO->GetDateMatchingInfo() ;

				if(!dateInfo.bIsValid)
				{
					// notice can't open dialog.
					//WINDOWMGR->MsgBox( MBI_IDENTIFICATION_ERROR, MBT_OK, "주민등록증을 발급 받아야 사용하실 수 있습니다.") ;
					WINDOWMGR->MsgBox( MBI_IDENTIFICATION_ERROR, MBT_OK, CHATMGR->GetChatMsg( 1284 ) );
				}
				else
				{
					// open dialog.
					CDateMatchingDlg* pDlg = GAMEIN->GetDateMatchingDlg() ;

					if( pDlg )
					{
						if( pDlg->IsActive() )
						{
							// check chatting.
							CDateMatchingChatDlg* pChatDlg = GAMEIN->GetDateMatchingDlg()->GetChatingDlg() ;
							if( pChatDlg )
							{
								if( pChatDlg->IsOnChatMode() )
								{
									// notice end chatting.
									//WINDOWMGR->MsgBox( MBI_END_CHATTING_CLOSE, MBT_YESNO, "대화를 종료 하시겠습니까?" );
									WINDOWMGR->MsgBox( MBI_END_CHATTING_CLOSE, MBT_YESNO, CHATMGR->GetChatMsg( 1304 ) );
								}
								else
								{
									pDlg->SetActive(FALSE) ;
								}
							}
						}
						else
						{
							pDlg->GetPartnerListDlg()->SettingControls() ;
							//pDlg->GetPartnerListDlg()->UpdateInfo() ;
							pDlg->SetActive(TRUE) ;
						}
					}
				}
			}
			break ;

			// 070618 LYW --- MainSystemDlg : Add family menu.
		case MSD_GSBTN_FAMILY :
			{
				/*pHandler = (cDialog*)GAMEIN->GetFamilyDlg();
				if( !pHandler ) return ;
				if( pHandler->IsActive() )
				{
					pHandler->SetActive( FALSE );
				}
				else 
				{
					pHandler->SetActive( TRUE );

					GAMEEVENTMGR->AddEvent( eGameEvent_FamilyInterface );
				}*/

				// 070621 LYW --- MacroManager : 패밀리 기능이 알파에 들어가지 않기 때문에 토글키 처리 수정.
				CSHFamily* pFamily = HERO->GetFamily() ;	

				if( pFamily && pFamily->Get()->nMasterID != 0 )
				{
					CFamilyDialog* pDlg = GAMEIN->GetFamilyDlg() ;

					if( pDlg )
					{
						if( pDlg->IsActive() )
						{
							pDlg->SetActive(FALSE) ;
						}
						else
						{
							pDlg->UpdateAllInfo() ;
							pDlg->SetActive(TRUE) ;
						}
					}
				}
				else
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 369 ));
				}
			}
			break ;

		case MSD_SSBTN_GAME : 
			{
				pHandler = (cDialog*)GAMEIN->GetOptionDialog();
				if( !pHandler ) return ;
				if( pHandler->IsActive() )
				{
					pHandler->SetActive(FALSE);
				}
				else
				{
					pHandler->SetActive(TRUE);

					GAMEEVENTMGR->AddEvent( eGameEvent_OptionInterface );
				}
			}
			break ;

			// 070117  LYW --- Delete this part for a moment.
			/*

		case MSD_SSBTN_GRAPHIC : 
			{
				pHandler = (cDialog*)GAMEIN->GetOptionDialog();
				if( !pHandler ) return ;
				if( pHandler->IsActive() )
				{
					pHandler->SetActive(FALSE);
				}
				else
				{
					pHandler->SetActive(TRUE);

					GAMEEVENTMGR->AddEvent( eGameEvent_OptionInterface );
				}
			}
			break ;
			*/

			// 070115 LYW --- Delete this part.
			/*
		case MSD_SSBTN_SOUND : 
			{
				pHandler = (cDialog*)GAMEIN->GetOptionDialog();
				if( !pHandler ) return ;
				if( pHandler->IsActive() )
				{
					pHandler->SetActive(FALSE);
				}
				else
				{
					pHandler->SetActive(TRUE);

					GAMEEVENTMGR->AddEvent( eGameEvent_OptionInterface );
				}
			}
			break ;
			*/

		case MSD_SSBTN_CHARSELECT : 
			{
				//// 080410 LYW --- GlobalEventFunc : 채팅 방 사용중인지 검사.
				//if(CHATROOMMGR->Get_ChatRoomState() == e_State_In_ChatRoom)
				//{
				//	WINDOWMGR->MsgBox( NULL, MBT_OK, CHATMGR->GetChatMsg(1493) );
				//	return ;
				//}

				WINDOWMGR->MsgBox( MBI_MOVE_TO_CHARSELECT, MBT_YESNO, CHATMGR->GetChatMsg( 1329 ) );
			}
			break ;

		case MSD_SSBTN_ENDGAME : 
			{
				// 070122 Modified this line.
				//WINDOWMGR->MsgBox( MBI_EXIT, MBT_YESNO, CHATMGR->GetChatMsg( 280 ) );
				//WINDOWMGR->MsgBox( MBI_EXIT, MBT_YESNO, RESRCMGR->GetMsg( 263 ) );
				// 080410 LYW --- MainSystemDlg : Modified.
				/*if(CHATROOMMGR->Get_ChatRoomState() == e_State_In_ChatRoom)
				{
					WINDOWMGR->MsgBox( NULL, MBT_OK, CHATMGR->GetChatMsg(1493) );
					return ;
				}*/

				WINDOWMGR->MsgBox( MBI_EXIT, MBT_YESNO, RESRCMGR->GetMsg( 263 ) );
			}
			break ;

		// 071025 LYW --- MainSystemDlg : Add help button.
		case MSD_GSBTN_HELP :
			{
				cHelperDlg* pDlg = NULL ;
				pDlg = GAMEIN->GetHelpDlg() ;

				if( !pDlg )
				{
					#ifdef _GMTOOL_
					MessageBox(NULL, "Failed to open help dlg", "MainSystemDlg::OnActionEvent", MB_OK);
					#endif //_GMTOOL

					return ;
				}

				pDlg->SetActive( !pDlg->IsActive() ) ;
			}
			break ;

		//// 080403 LYW --- MainSystemDlg : Add a button for chatroom system.
 	//	case MSD_GSBTN_CHAT :
 	//		{
 	//			CHATROOMMGR->ToggleChatRoomMainDlg() ;
 	//		}
 	//		break ;
		}
	}
}


//=================================================================================================
//	NAME		: SetSystemSet()
//	PURPOSE		: Add function to setting systemset part.
//	DATE		: January 15, 2007 LYW
//	ATTENTION	:
//=================================================================================================
void CMainSystemDlg::SetSystemSet()
{
	int count = 0 ;

	if( m_bShowSystemSet )
	{
		for( count = 0 ; count < MAX_SYSTEMSET_BTN ; ++count ) 
		{
			m_pMenuSystemSet[ count ]->SetActive( TRUE ) ;
		}
		m_pTitleSystemSet->SetActive( TRUE ) ;
		m_pMarkSystemSet->SetActive( TRUE ) ;
		m_pSystemSet->SetActive( TRUE ) ;
	}
	else
	{
		for( count = 0 ; count < MAX_SYSTEMSET_BTN ; ++count ) 
		{
			m_pMenuSystemSet[ count ]->SetActive( FALSE ) ;
		}
		m_pTitleSystemSet->SetActive( FALSE ) ;
		m_pMarkSystemSet->SetActive( FALSE ) ;
		m_pSystemSet->SetActive( FALSE ) ;
	}
}


//=================================================================================================
//	NAME		: SetGameSystem()
//	PURPOSE		: Add function to setting gamesystem part.
//	DATE		: January 15, 2007 LYW
//	ATTENTION	:
//=================================================================================================
void CMainSystemDlg::SetGameSystem()
{
	int count = 0 ;

	if( m_bShowGameSystem )
	{
		for( count = 0 ;  count < MAX_GAMESYSTEM_BTN ; ++count ) 
		{
			m_pMenuGameSystem[ count ]->SetActive( TRUE ) ;
		}
		m_pTitleGameSystem->SetActive( TRUE ) ;
		m_pMarkGameSystem->SetActive( TRUE ) ;
		m_pGameSystem->SetActive( TRUE ) ;
	}
	else
	{
		for( count = 0 ;  count < MAX_GAMESYSTEM_BTN ; ++count ) 
		{
			m_pMenuGameSystem[ count ]->SetActive( FALSE ) ;
		}
		m_pTitleGameSystem->SetActive( FALSE ) ;
		m_pMarkGameSystem->SetActive( FALSE ) ;
		m_pGameSystem->SetActive( FALSE ) ;
	}
}
















