



#include "stdafx.h"																	// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.

#include "WindowIdEnum.h"															// ������ ���̵� ���� �Ǿ� �ִ� ��� ������ �ҷ��´�.

#include "MHFile.h"																	// ���� ���� Ŭ���� ��������� �ҷ��´�.

#include "../[Client]LUNA/Interface/cScriptManager.h"								// ��ũ��Ʈ �Ŵ��� ��������� �ҷ��´�.
#include "../[CC]Header/GameResourceManager.h"										// ���ҽ� �Ŵ��� ��������� �ҷ��´�.
#include "./interface/cWindowManager.h"												// ������ �Ŵ��� ��������� �ҷ��´�.

#include ".\npcimagedlg.h"															// NPC �̹��� ���̾�α� Ŭ���� ��������� �ҷ��´�.


cNpcImageDlg::cNpcImageDlg(void)													// ������ �Լ�.
{
	COLOR = RGBA_MAKE(255,255,255,150);												// ������ �����Ѵ�.
	COLOR2 = RGBA_MAKE(255,255,255,255);												// ������ �����Ѵ�.

	SCALE.x = 450.f/512.f;															// �������� �����Ѵ�.
	SCALE.y = 450.f/512.f;

	//BACK_SCALE.x = 180;																// ��� �������� �����Ѵ�.
	//BACK_SCALE.y = 150;

	//BACK_SCALE.x = 640.0f;																// ��� �������� �����Ѵ�.
	//BACK_SCALE.y = 1.0f;

	BACK_POS.x = 0;																	// ��� ��ġ�� �����Ѵ�.
	BACK_POS.y = 0;

	mpCurImage = NULL;																// ���� �̹��� �����͸� null ó���Ѵ�.

	mEmotion = EMOTION_NORMAL;														// �̸���� �⺻���� �����Ѵ�.

	mpCurImage = new NPC_IMAGE ;													// �̹��� �޸𸮸� �Ҵ��Ͽ� �����ͷ� �ѱ��.

	SCRIPTMGR->GetImage( 12, &mImageBack, PFT_HARDPATH );							// �޹�� ���� �̹����� �޴´�.

	m_pTopImage	= NULL ;															// ž �̹��� �����͸� NULL ó���� �Ѵ�.
	m_pBottomImage = NULL ;															// ���� �̹��� �����͸� NULL ó���� �Ѵ�.

	m_nDispWidth	= 0 ;															// ���÷��� ���� ����� 0���� �����Ѵ�.
	m_nDispHeight	= 0 ;															// ���÷��� ���� ����� 0���� �����Ѵ�.
}

cNpcImageDlg::~cNpcImageDlg(void)													// �Ҹ��� �Լ�.
{
	if( mpCurImage )																// ���� �̹��� ������ ��ȿ�ϸ�,
	{
		delete mpCurImage ;															// ���� �̹����� �����Ѵ�.

		mpCurImage = NULL ;															// ���� �̹��� �����͸� null ó���� �Ѵ�.
	}
}

void cNpcImageDlg::Linking()														// ��ũ �Լ�.
{
	m_pTopImage	= (cStatic*)GetWindowForID(NI_TOPIMG) ;								// ž �̹����� ��ũ�Ѵ�.
	m_pBottomImage = (cStatic*)GetWindowForID(NI_BOTTOMIMG) ;						// ���� �̹����� ��ũ�Ѵ�.

	SetDisplayWH() ;																// ���÷��� ���� ���� ����� �����Ѵ�.
}

void cNpcImageDlg::SetDisplayWH()													// ���÷��� ���� ���� ����� �����ϴ� �Լ�.
{
	DISPLAY_INFO	dispInfo ;														// ���÷��� ������ ������ ����ü�� �����Ѵ�.

	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;										// ���÷��� ������ ���´�.

	m_nDispWidth  = dispInfo.dwWidth ;												// ���÷��� ���� ����� �����Ѵ�.
	m_nDispHeight = dispInfo.dwHeight ;												// ���÷��� ���� ����� �����Ѵ�.
}

void cNpcImageDlg::LoadingImage(WORD idx)											// �̹����� �ε��ϴ� �Լ�.
{
	CMHFile file;																	// ���� ������ �����Ѵ�.

	file.Init("Data/Interface/Windows/NpcImageList.bin","rb");						// npc �̹��� ����Ʈ�� �б���� ����.

	if(file.IsInited() == FALSE)													// ���⿡ �����ϸ�,
	{
		return;																		// ���� ó���� �Ѵ�.
	}

	DISPLAY_INFO	dispInfo ;														// ���÷��� ������ ������ ����ü�� �����Ѵ�.

	VECTOR2 vDisp1, vDisp2, vDisp3 ;												// ��ġ ���͸� �����Ѵ�.

	vDisp1.x = file.GetFloat() ;													// ��ġ ���� 1�� ������ �о���δ�.
	vDisp1.y = file.GetFloat() ;

	vDisp2.x = file.GetFloat() ;													// ��ġ ���� 2�� ������ �о���δ�.
	vDisp2.y = file.GetFloat() ;

	vDisp3.x = file.GetFloat() ;													// ��ġ ���� 3�� ������ �о���δ�.
	vDisp3.y = file.GetFloat() ;

	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;										// ���÷��� ������ ���´�.

	BACK_POS.y = (float)dispInfo.dwHeight - mImageBack.GetImageRect()->bottom ;

	BACK_SCALE.x = (float)dispInfo.dwWidth/2 ;										// ��� �������� �����Ѵ�.
	BACK_SCALE.y = 1.0f;

	//cImageScale sc ;																// �̹��� �������� �����Ѵ�.

	switch( dispInfo.dwWidth )														// ���÷��� ������ ���̸� Ȯ���Ѵ�.
	{
	case 800 :																		// ���̰� 800�̸�,
		{
			POS.x = vDisp1.x ;														// ��ġ ������ ���� 1�� �����Ѵ�.
			POS.y = vDisp1.y ;

			//sc.x = 0.8f ;															// �������� �����Ѵ�.
			//sc.y = 0.5f ;

			//m_pTopImage->SetScale(&sc) ;											// ž �̹����� �������� �����Ѵ�.
			//m_pBottomImage->SetScale(&sc) ;											// ���� �̹����� �������� �����Ѵ�.

			//m_pBottomImage->SetAbsXY(0, (LONG)500) ;								// ���� �̹����� ��ġ�� �����Ѵ�.
		}
		break ;

	case 1024 :																		// ���̰� 1024�̸�,
		{
			POS.x = vDisp2.x ;														// ��ġ ������ ���� 2�� �����Ѵ�.
			POS.y = vDisp2.y ;

			//sc.x = 1.0f ;															// �������� �����Ѵ�.
			//sc.y = 0.6f ;

			//m_pTopImage->SetScale(&sc) ;											// ž �̹����� �������� �����Ѵ�.
			//m_pBottomImage->SetScale(&sc) ;											// ���� �̹����� �������� �����Ѵ�.

			//m_pBottomImage->SetAbsXY(0, (LONG)640) ;								// ���� �̹����� ��ġ�� �����Ѵ�.
		}
		break ;

	case 1280 :																		// ���̰� 1280�� ���ٸ�,
		{
			POS.x = vDisp3.x ;														// ��ġ ������ 3���� �����Ѵ�.
			POS.y = vDisp3.y ;

			//sc.x = 1.3f ;															// �������� �����Ѵ�.
			//sc.y = 1.0f ;

			//m_pTopImage->SetScale(&sc) ;											// ž�̹����� �������� �����Ѵ�.
			//m_pBottomImage->SetScale(&sc) ;											// ���� �̹����� �������� �����Ѵ�.
		}
		break ;
	}

	char buf[256];																	// �ӽ� ���۸� �����Ѵ�.

	WORD wIndex;																	// �ε����� ���� ������ �����Ѵ�.

	BYTE emotion;																	// ����� ���� ������ �����Ѵ�.

	memset(mpCurImage, 0, sizeof(NPC_IMAGE)) ;										// ���� �̹����� �޸� �� �Ѵ�.

	while(1)																		// while���� ������.
	{
		if(file.IsEOF() != FALSE)													// ���� �����Ͱ� ������ ���� �������� �ʾҴٸ�,
		{
			break;																	// while���� Ż���Ѵ�.
		}

		wIndex  = file.GetWord();													// �ε����� �о���δ�.
		emotion = file.GetByte();													// ����� �о���δ�.
		char*	szFileName	=	file.GetString();									// ���ϸ��� �����Ѵ�.

		if( wIndex == idx )															// �ӽ� �ε����� ���� �ε����� ���ٸ�,
		{
			if( strlen( szFileName) == 0) break;									// ���ϸ��� 0�� ���ٸ�, while���� Ż���Ѵ�.

			sprintf( buf, "%s%s", "./data/interface/2dimage/npciamge/", szFileName);// �ӽ� ���ۿ� ��ι�, ���ϸ��� �����Ѵ�.

			mpCurImage->idx = idx;													// ���� �̹����� �ε����� �����Ѵ�.
			mpCurImage->image[ emotion ].LoadSprite( buf );							// ���� �̹����� ��ǿ� ���� ��������Ʈ�� �ε��Ѵ�.
		}
	}
}

void cNpcImageDlg::SetActive( BOOL val )											// Ȱ��, ��Ȱ��ȭ �Լ�.
{
	if( val )																		// val�� TRUE�� ���ٸ�,
	{
		if( IsActive() ) return ;													// Ȱ��ȭ ���¶��,

		//WINDOWMGR->m_pActivedWindowList->RemoveAll() ;								// Ȱ��ȭ �� ��� â�� ����.
		WINDOWMGR->CloseAllWindow();												// ��� �����츦 �ݴ´�.��
	}
	else																			// val�� FALSE�� ���ٸ�,
	{
		WINDOWMGR->ShowAllActivedWindow();											// ��� Ȱ��ȭ �����츦 �����ش�.

		WINDOWMGR->SetOpendAllWindows( TRUE ) ;										// ��� Ȱ��ȭ �����츦 ����.

		WINDOWMGR->ShowBaseWindow() ;												// �⺻ �����츦 �����ش�.
	}
	
	cDialog::SetActive(val);														// ���̾�α��� Ȱ��ȭ ���θ� �����Ѵ�.
}

void cNpcImageDlg::SetNpc( WORD NpcUniqueIdx )										// Npc������ �����ϴ� �Լ�.
{
	mNpcIdx = NpcUniqueIdx;															// Npc�ε����� �����Ѵ�.

	LoadingImage(mNpcIdx) ;															// �̹����� �ε��Ѵ�.
}

void cNpcImageDlg::Render()															// ���� �Լ�.
{
	if( !m_bActive )																// Ȱ��ȭ �Ǿ� ���� ������,
	{				
		return;																		// ���� ó���� �Ѵ�.
	}

	mImageBack.RenderSprite( &BACK_SCALE, NULL, 0, &BACK_POS, COLOR );				// ����� ����Ѵ�.

	if( mpCurImage )																// ���� �̹��� ������  ��ȿ�ϸ�,
	{
		int nWidth  = mpCurImage->image[ mEmotion ].GetImageRect()->right ;			// ���� �̹����� ���� ����� �޴´�.
		int nHeight = mpCurImage->image[ mEmotion ].GetImageRect()->bottom ;		// ���� �̹����� ���� ����� �޴´�.

		POS.x = (float)(m_nDispWidth - nWidth) ;									// ��� ��ġ X��ǥ�� �����Ѵ�.
		POS.y = (float)(m_nDispHeight - nHeight) ;									// ��� ��ġ Y��ǥ�� �����Ѵ�.

		mpCurImage->image[ mEmotion ].RenderSprite( NULL, NULL, 0, &POS, COLOR2 );	// ���� �̹����� ����Ѵ�.
	}

	//cDialog::Render();																// ���̾�α׸� ���� �Ѵ�.
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
//		char*	szFileName	=	file.GetString();								// ���� �̸��� ���������� �׽�Ʈ �غ���,
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