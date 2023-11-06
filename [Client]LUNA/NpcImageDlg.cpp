



#include "stdafx.h"																	// 표준 시스템과, 프로젝트가 지정한, 자주쓰는 해더들을 모은 해더파일을 불러온다.

#include "WindowIdEnum.h"															// 윈도우 아이디가 정의 되어 있는 헤더 파일을 불러온다.

#include "MHFile.h"																	// 묵향 파일 클래스 헤더파일을 불러온다.

#include "../[Client]LUNA/Interface/cScriptManager.h"								// 스크립트 매니져 헤더파일을 불러온다.
#include "../[CC]Header/GameResourceManager.h"										// 리소스 매니져 헤더파일을 불러온다.
#include "./interface/cWindowManager.h"												// 윈도우 매니져 헤더파일을 불러온다.

#include ".\npcimagedlg.h"															// NPC 이미지 다이얼로그 클래스 헤더파일을 불러온다.


cNpcImageDlg::cNpcImageDlg(void)													// 생성자 함수.
{
	COLOR = RGBA_MAKE(255,255,255,150);												// 색상을 세팅한다.
	COLOR2 = RGBA_MAKE(255,255,255,255);												// 색상을 세팅한다.

	SCALE.x = 450.f/512.f;															// 스케일을 세팅한다.
	SCALE.y = 450.f/512.f;

	//BACK_SCALE.x = 180;																// 배경 스케일을 세팅한다.
	//BACK_SCALE.y = 150;

	//BACK_SCALE.x = 640.0f;																// 배경 스케일을 세팅한다.
	//BACK_SCALE.y = 1.0f;

	BACK_POS.x = 0;																	// 배경 위치를 세팅한다.
	BACK_POS.y = 0;

	mpCurImage = NULL;																// 현재 이미지 포인터를 null 처리한다.

	mEmotion = EMOTION_NORMAL;														// 이모션을 기본으로 세팅한다.

	mpCurImage = new NPC_IMAGE ;													// 이미지 메모리를 할당하여 포인터로 넘긴다.

	SCRIPTMGR->GetImage( 12, &mImageBack, PFT_HARDPATH );							// 뒷배경 상하 이미지를 받는다.

	m_pTopImage	= NULL ;															// 탑 이미지 포인터를 NULL 처리를 한다.
	m_pBottomImage = NULL ;															// 버텀 이미지 포인터를 NULL 처리를 한다.

	m_nDispWidth	= 0 ;															// 디스플레이 가로 사이즈를 0으로 세팅한다.
	m_nDispHeight	= 0 ;															// 디스플레이 세로 사이즈를 0으로 세팅한다.
}

cNpcImageDlg::~cNpcImageDlg(void)													// 소멸자 함수.
{
	if( mpCurImage )																// 현재 이미지 정보가 유효하면,
	{
		delete mpCurImage ;															// 현재 이미지를 삭제한다.

		mpCurImage = NULL ;															// 현재 이미지 포인터를 null 처리를 한다.
	}
}

void cNpcImageDlg::Linking()														// 링크 함수.
{
	m_pTopImage	= (cStatic*)GetWindowForID(NI_TOPIMG) ;								// 탑 이미지를 링크한다.
	m_pBottomImage = (cStatic*)GetWindowForID(NI_BOTTOMIMG) ;						// 버텀 이미지를 링크한다.

	SetDisplayWH() ;																// 디스플레이 가로 세로 사이즈를 세팅한다.
}

void cNpcImageDlg::SetDisplayWH()													// 디스플레이 가로 세로 사이즈를 세팅하는 함수.
{
	DISPLAY_INFO	dispInfo ;														// 디스플레이 정보를 세팅할 구조체를 선언한다.

	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;										// 디스플레이 정보를 얻어온다.

	m_nDispWidth  = dispInfo.dwWidth ;												// 디스플레이 가로 사이즈를 세팅한다.
	m_nDispHeight = dispInfo.dwHeight ;												// 디스플레이 세로 사이즈를 세팅한다.
}

void cNpcImageDlg::LoadingImage(WORD idx)											// 이미지를 로드하는 함수.
{
	CMHFile file;																	// 묵향 파일을 선언한다.

	file.Init("Data/Interface/Windows/NpcImageList.bin","rb");						// npc 이미지 리스트를 읽기모드로 연다.

	if(file.IsInited() == FALSE)													// 열기에 실패하면,
	{
		return;																		// 리턴 처리를 한다.
	}

	DISPLAY_INFO	dispInfo ;														// 디스플레이 정보를 세팅할 구조체를 선언한다.

	VECTOR2 vDisp1, vDisp2, vDisp3 ;												// 위치 벡터를 선언한다.

	vDisp1.x = file.GetFloat() ;													// 위치 벡터 1의 정보를 읽어들인다.
	vDisp1.y = file.GetFloat() ;

	vDisp2.x = file.GetFloat() ;													// 위치 벡터 2의 정보를 읽어들인다.
	vDisp2.y = file.GetFloat() ;

	vDisp3.x = file.GetFloat() ;													// 위치 벡터 3의 정보를 읽어들인다.
	vDisp3.y = file.GetFloat() ;

	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;										// 디스플레이 정보를 얻어온다.

	BACK_POS.y = (float)dispInfo.dwHeight - mImageBack.GetImageRect()->bottom ;

	BACK_SCALE.x = (float)dispInfo.dwWidth/2 ;										// 배경 스케일을 세팅한다.
	BACK_SCALE.y = 1.0f;

	//cImageScale sc ;																// 이미지 스케일을 선언한다.

	switch( dispInfo.dwWidth )														// 디스플레이 정보의 넓이를 확인한다.
	{
	case 800 :																		// 넓이가 800이면,
		{
			POS.x = vDisp1.x ;														// 위치 정보를 벡터 1로 세팅한다.
			POS.y = vDisp1.y ;

			//sc.x = 0.8f ;															// 스케일을 적용한다.
			//sc.y = 0.5f ;

			//m_pTopImage->SetScale(&sc) ;											// 탑 이미지에 스케일을 적용한다.
			//m_pBottomImage->SetScale(&sc) ;											// 버텀 이미지에 스케일을 적용한다.

			//m_pBottomImage->SetAbsXY(0, (LONG)500) ;								// 버텀 이미지의 위치를 세팅한다.
		}
		break ;

	case 1024 :																		// 넓이가 1024이면,
		{
			POS.x = vDisp2.x ;														// 위치 정보를 벡터 2로 세팅한다.
			POS.y = vDisp2.y ;

			//sc.x = 1.0f ;															// 스케일을 적용한다.
			//sc.y = 0.6f ;

			//m_pTopImage->SetScale(&sc) ;											// 탑 이미지에 스케일을 적용한다.
			//m_pBottomImage->SetScale(&sc) ;											// 버텀 이미지에 스케일을 적용한다.

			//m_pBottomImage->SetAbsXY(0, (LONG)640) ;								// 버텀 이미지의 위치를 세팅한다.
		}
		break ;

	case 1280 :																		// 넓이가 1280과 같다면,
		{
			POS.x = vDisp3.x ;														// 위치 정보를 3으로 세팅한다.
			POS.y = vDisp3.y ;

			//sc.x = 1.3f ;															// 스케일을 적용한다.
			//sc.y = 1.0f ;

			//m_pTopImage->SetScale(&sc) ;											// 탑이미지에 스케일을 적용한다.
			//m_pBottomImage->SetScale(&sc) ;											// 버텀 이미지에 스케일을 적용한다.
		}
		break ;
	}

	char buf[256];																	// 임시 버퍼를 선언한다.

	WORD wIndex;																	// 인덱스를 담을 변수를 선언한다.

	BYTE emotion;																	// 모션을 담을 변수를 선언한다.

	memset(mpCurImage, 0, sizeof(NPC_IMAGE)) ;										// 현재 이미지를 메모리 셋 한다.

	while(1)																		// while문을 돌린다.
	{
		if(file.IsEOF() != FALSE)													// 파일 포인터가 파일의 끝에 도달하지 않았다면,
		{
			break;																	// while문을 탈출한다.
		}

		wIndex  = file.GetWord();													// 인덱스를 읽어들인다.
		emotion = file.GetByte();													// 모션을 읽어들인다.
		char*	szFileName	=	file.GetString();									// 파일명을 세팅한다.

		if( wIndex == idx )															// 임시 인덱스와 들어온 인덱스가 같다면,
		{
			if( strlen( szFileName) == 0) break;									// 파일명이 0과 같다면, while문을 탈출한다.

			sprintf( buf, "%s%s", "./data/interface/2dimage/npciamge/", szFileName);// 임시 버퍼에 경로및, 파일명을 세팅한다.

			mpCurImage->idx = idx;													// 현재 이미지의 인덱스를 세팅한다.
			mpCurImage->image[ emotion ].LoadSprite( buf );							// 현재 이미지의 모션에 따른 스프라이트를 로드한다.
		}
	}
}

void cNpcImageDlg::SetActive( BOOL val )											// 활성, 비활성화 함수.
{
	if( val )																		// val가 TRUE와 같다면,
	{
		if( IsActive() ) return ;													// 활성화 상태라면,

		//WINDOWMGR->m_pActivedWindowList->RemoveAll() ;								// 활성화 된 모든 창을 비운다.
		WINDOWMGR->CloseAllWindow();												// 모든 윈도우를 닫는다.ㅏ
	}
	else																			// val이 FALSE와 같다면,
	{
		WINDOWMGR->ShowAllActivedWindow();											// 모든 활성화 윈도우를 보여준다.

		WINDOWMGR->SetOpendAllWindows( TRUE ) ;										// 모든 활성화 윈도우를 연다.

		WINDOWMGR->ShowBaseWindow() ;												// 기본 윈도우를 보여준다.
	}
	
	cDialog::SetActive(val);														// 다이얼로그의 활성화 여부를 세팅한다.
}

void cNpcImageDlg::SetNpc( WORD NpcUniqueIdx )										// Npc정보를 세팅하는 함수.
{
	mNpcIdx = NpcUniqueIdx;															// Npc인덱스를 세팅한다.

	LoadingImage(mNpcIdx) ;															// 이미지를 로드한다.
}

void cNpcImageDlg::Render()															// 렌더 함수.
{
	if( !m_bActive )																// 활성화 되어 있지 않으면,
	{				
		return;																		// 리턴 처리를 한다.
	}

	mImageBack.RenderSprite( &BACK_SCALE, NULL, 0, &BACK_POS, COLOR );				// 배경을 출력한다.

	if( mpCurImage )																// 현재 이미지 정보가  유효하면,
	{
		int nWidth  = mpCurImage->image[ mEmotion ].GetImageRect()->right ;			// 현재 이미지의 가로 사이즈를 받는다.
		int nHeight = mpCurImage->image[ mEmotion ].GetImageRect()->bottom ;		// 현재 이미지의 세로 사이즈를 받는다.

		POS.x = (float)(m_nDispWidth - nWidth) ;									// 출력 위치 X좌표를 세팅한다.
		POS.y = (float)(m_nDispHeight - nHeight) ;									// 출력 위치 Y좌표를 세팅한다.

		mpCurImage->image[ mEmotion ].RenderSprite( NULL, NULL, 0, &POS, COLOR2 );	// 현재 이미지를 출력한다.
	}

	//cDialog::Render();																// 다이얼로그를 렌더 한다.
}







//#include "stdafx.h"
//#include ".\npcimagedlg.h"
//#include "MHFile.h"
//#include "../[Client]LUNA/Interface/cScriptManager.h"
//#include "./interface/cWindowManager.h"
//
//// 070328 LYW --- NpcImageDlg : Include game resource manager.
//#include "../[CC]Header/GameResourceManager.h"
//
//// 070328 LYW --- NpcImageDlg : Include window id.
//#include "WindowIdEnum.h"
//
//cNpcImageDlg::cNpcImageDlg(void)
//{
//	COLOR = RGBA_MAKE(255,255,255,255);
//	SCALE.x = 450.f/512.f;
//	SCALE.y = 450.f/512.f;
//	BACK_SCALE.x = 180;
//	BACK_SCALE.y = 128;
//	BACK_POS.x = 0;
//	BACK_POS.y = 0;
//	mpCurImage = NULL;
//	mEmotion = EMOTION_NORMAL;
//	// 070507 LYW --- NpcImageDlg : Delete image table.
//	//mImageTable.Initialize(30);
//
//	// 070507 LYW --- NpcImageDlg : Delete notice part.
//	/*
//	// 070328 LYW --- NpcImageDlg : Initialize notice.
//	m_pNotice = NULL ;
//
//	// 070328 LYW --- NpcImageDlg : Initialize notice.
//	m_bActivedNotice = FALSE ;
//
//	// 070329 LYW --- NpcImageDlg : Initialize variables for alpha.
//	m_nAlphaValue	 = 0 ;
//	m_nContinueCount = 0 ;
//	*/
//
//	// 070507 LYW --- NpcImageDlg : Initialize current image.
//	mpCurImage = new NPC_IMAGE ;
//
//	// 070507 LYW --- NpcImageDlg : Loading background image.
//	SCRIPTMGR->GetImage( 12, &mImageBack, PFT_HARDPATH );
//
//	m_pTopImage	= NULL ;
//	m_pBottomImage = NULL ;
//}
//
//cNpcImageDlg::~cNpcImageDlg(void)
//{
//	// 070507 LYW --- NpcImageDlg : Delete image table.
//	/*
//	NPC_IMAGE* pImage = NULL;
//
//	mImageTable.SetPositionHead();
//	while(pImage = mImageTable.GetData())
//	{
//		delete pImage;
//	}
//	mImageTable.RemoveAll();
//	*/
//
//	// 070507 LYW --- NpcImageDlg : Delete current image.
//	if( mpCurImage )
//	{
//		delete mpCurImage ;
//
//		mpCurImage = NULL ;
//	}
//}
//
//// 070620 LYW --- NpcImageDlg : Add function to linking constrols.
//void cNpcImageDlg::Linking()
//{
//	m_pTopImage	= (cStatic*)GetWindowForID(NI_TOPIMG) ;
//	m_pBottomImage = (cStatic*)GetWindowForID(NI_BOTTOMIMG) ;
//}
//
//// 070507 LYW --- NpcImageDlg : Delete function to linking image dlg.
///*
//void cNpcImageDlg::Linking()
//{
//	CMHFile file;
//	file.Init("Data/Interface/Windows/NpcImageList.bin","rb");
//
//	if(file.IsInited() == FALSE)
//	{
//		return;
//	}
//
//	POS.x = file.GetFloat();
//	POS.y = file.GetFloat();
//
//	char buf[256];
//
//	WORD idx;
//	BYTE emotion;
//	NPC_IMAGE* pImage;
//
//	while(1)
//	{
//		if(file.IsEOF() != FALSE)
//			break;
//
//		idx = file.GetWord();
//		emotion = file.GetByte();
//		char*	szFileName	=	file.GetString();								// 파일 이름이 멀쩡한지를 테스트 해보고,
//		if( strlen( szFileName) == 0)
//			break;
////			_asm nop;
//		sprintf( buf, "%s%s", "./data/interface/2dimage/npc/", szFileName);
//		//sprintf( buf, "%s%s", "./data/interface/2dimage/npc/", file.GetString() );
//
//		pImage = mImageTable.GetData( idx );
//
//		if( pImage )
//		{
//			if( pImage->image[ emotion ].IsNull() )
//			{
//				pImage->image[ emotion ].LoadSprite( buf );
//			}
//		}
//		else
//		{
//			pImage = new NPC_IMAGE;
//			pImage->idx = idx;
//			pImage->image[ emotion ].LoadSprite( buf );
//			mImageTable.Add( pImage, idx );
//		}
//	}
//
////	SCRIPTMGR->GetImage( 10, &mImageTop, PFT_HARDPATH );
////	SCRIPTMGR->GetImage( 11, &mImageBottom, PFT_HARDPATH );
//	SCRIPTMGR->GetImage( 12, &mImageBack, PFT_HARDPATH );
//
//	// 070328 LYW --- NpcImageDlg : Reference display information.
//	DISPLAY_INFO	dispInfo ;
//	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;
//
//	// 070328 LYW --- NpcImageDlg : Setting notice.
//	m_pNotice = (cStatic*)GetWindowForID(NI_NOTICE) ;
//	m_pNotice->SetAbsXY( 0, (LONG)dispInfo.dwHeight - 240 ) ;
//	m_pNotice->SetWH( (LONG)dispInfo.dwWidth - 40, 200 ) ;
//}
//*/
//
//// 070508 LYW --- NpcImageDlg : Add function to loading image.
//void cNpcImageDlg::LoadingImage(WORD idx)
//{
//	CMHFile file;
//	file.Init("Data/Interface/Windows/NpcImageList.bin","rb");
//
//	if(file.IsInited() == FALSE)
//	{
//		return;
//	}
//
//	// 070620 LYW --- NpcImageDlg : Modified NpcPosition.
//	/*POS.x = file.GetFloat();
//	POS.y = file.GetFloat();*/
//	DISPLAY_INFO	dispInfo ;
//
//	VECTOR2 vDisp1, vDisp2, vDisp3 ;
//
//	vDisp1.x = file.GetFloat() ;
//	vDisp1.y = file.GetFloat() ;
//
//	vDisp2.x = file.GetFloat() ;
//	vDisp2.y = file.GetFloat() ;
//
//	vDisp3.x = file.GetFloat() ;
//	vDisp3.y = file.GetFloat() ;
//
//	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;
//
//	cImageScale sc ;
//
//	switch( dispInfo.dwWidth )
//	{
//	case 800 :
//		{
//			POS.x = vDisp1.x ;
//			POS.y = vDisp1.y ;
//
//			sc.x = 0.8f ;
//			sc.y = 0.5f ;
//
//			m_pTopImage->SetScale(&sc) ;
//			m_pBottomImage->SetScale(&sc) ;
//
//			m_pBottomImage->SetAbsXY(0, (LONG)500) ;
//		}
//		break ;
//
//	case 1024 :
//		{
//			POS.x = vDisp2.x ;
//			POS.y = vDisp2.y ;
//
//			sc.x = 1.0f ;
//			sc.y = 0.6f ;
//
//			m_pTopImage->SetScale(&sc) ;
//			m_pBottomImage->SetScale(&sc) ;
//
//			m_pBottomImage->SetAbsXY(0, (LONG)640) ;
//		}
//		break ;
//
//	case 1280 :
//		{
//			POS.x = vDisp3.x ;
//			POS.y = vDisp3.y ;
//
//			sc.x = 1.3f ;
//			sc.y = 1.0f ;
//
//			m_pTopImage->SetScale(&sc) ;
//			m_pBottomImage->SetScale(&sc) ;
//		}
//		break ;
//	}
//
//	char buf[256];
//
//	WORD wIndex;
//	BYTE emotion;
//
//	memset(mpCurImage, 0, sizeof(NPC_IMAGE)) ;
//
//	while(1)
//	{
//		if(file.IsEOF() != FALSE)
//			break;
//
//		wIndex  = file.GetWord();
//		emotion = file.GetByte();
//		char*	szFileName	=	file.GetString();		
//
//		if( wIndex == idx )
//		{
//			if( strlen( szFileName) == 0) break;
//			sprintf( buf, "%s%s", "./data/interface/2dimage/npciamge/", szFileName);
//
//			mpCurImage->idx = idx;
//			mpCurImage->image[ emotion ].LoadSprite( buf );
//		}
//	}
//}
//
//void cNpcImageDlg::SetActive( BOOL val )
//{
//	/*
//	if( val )
//	{
//		if( !mpCurImage )
//		{
//			return;
//		}
//
//		if( mpCurImage->image[ mEmotion ].IsNull() )
//		{
//			return;
//		}
//	}
//	*/
//
//	// 070126 LYW --- Modified this part.	
//	if( val )
//	{
//		// 070326 LYW --- NpcImageDlg : Add function to check active.
//		if( IsActive() ) return ;
//
//		WINDOWMGR->m_pActivedWindowList->RemoveAll() ;
//		WINDOWMGR->CloseAllWindow();
//	}
//	else
//	{
//		WINDOWMGR->ShowAllActivedWindow();
//
//		WINDOWMGR->SetOpendAllWindows( TRUE ) ;
//
//		WINDOWMGR->ShowBaseWindow() ;
//	}
//	
//	cDialog::SetActive(val);
//}
//
//void cNpcImageDlg::SetNpc( WORD NpcUniqueIdx )
//{
//	mNpcIdx = NpcUniqueIdx;
//	// 070507 LYW --- NpcImageDlg : Delete image table.
//	//mpCurImage = mImageTable.GetData( mNpcIdx );
//
//	// 070507 LYW --- NpcImageDlg : Loading image.
//	LoadingImage(mNpcIdx) ;
//}
//
//void cNpcImageDlg::Render()
//{
//	if( !m_bActive )
//	{
//		return;
//	}
//
//	// 070424 LYW --- NpcImageDlg : Modified check mpCurImage.
//	/*
//	if( !mpCurImage )
//	{
//		return;
//	}	
//
//	mImageBack.RenderSprite( &BACK_SCALE, NULL, 0, &BACK_POS, COLOR );
//	mpCurImage->image[ mEmotion ].RenderSprite( NULL, NULL, 0, &POS, COLOR );
//	*/
//	mImageBack.RenderSprite( &BACK_SCALE, NULL, 0, &BACK_POS, COLOR );
//
//	if( mpCurImage )
//	{
//		mpCurImage->image[ mEmotion ].RenderSprite( NULL, NULL, 0, &POS, COLOR );
//	}
//
//	// 070328 LYW --- NpcImageDlg : Render notice.
//	// 070504 LYW --- NpcImageDlg : DeActive notice.
//	/*
//	if( m_bActivedNotice)
//	{
//		if( m_nAlphaValue < 255 && m_nContinueCount == 0 )
//		{
//			m_nAlphaValue += 10 ;
//
//			if( m_nAlphaValue >= 255 )
//			{
//				m_nAlphaValue = 255 ;
//			}
//		}
//		else
//		{
//			if( m_nContinueCount < 200 )
//			{
//				m_nContinueCount += 10 ;
//			}
//			else
//			{
//				if( m_nAlphaValue > 0 )
//				{
//					m_nAlphaValue -= 10 ;
//
//					if(m_nAlphaValue <= 0 )
//					{
//						m_nAlphaValue = 0 ;
//					}
//				}
//				else
//				{
//					m_pNotice->SetActive( FALSE ) ;
//					m_bActivedNotice = FALSE ;
//				}
//			}
//		}
//
//		m_pNotice->SetAlpha( (BYTE)m_nAlphaValue ) ;
//	}
//	*/
///*
//	VECTOR2 pos;
//	
//	pos.x = 0;
//	pos.y = 0;
//
//	mImageTop.RenderSprite( NULL, NULL, 0, &pos, COLOR );
//
//	pos.y = 1024 - 218;
//	mImageBottom.RenderSprite( NULL, NULL, 0, &pos, COLOR );
//*/
//	cDialog::Render();
//}
//
//// 070328 LYW --- NpcImageDlg : Add function to initialize notice.
//void cNpcImageDlg::InitializeNotice( char* msgStr )
//{
//	/*
//	m_bActivedNotice = TRUE ;
//
//	m_nAlphaValue = 100 ;
//	m_nContinueCount = 0 ;
//
//	m_pNotice->SetStaticText( msgStr ) ;
//	m_pNotice->SetActive( TRUE ) ;
//	m_pNotice->SetAlpha( (BYTE)m_nAlphaValue ) ;
//	m_pNotice->SetAlwaysTop( TRUE ) ;
//	*/
//}