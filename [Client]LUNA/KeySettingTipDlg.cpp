#include "stdafx.h"
#include "WindowIDEnum.h"

#include "KeySettingTipDlg.h"

// 070202 LYW --- Include header file.
#include "../[CC]Header/GameResourceManager.h"
#include "../Interface/cScriptManager.h"

// 071022 LYW --- KeySettingTipDlg : Include header file of tutorial manager.
#include "TutorialManager.h"

CKeySettingTipDlg::CKeySettingTipDlg(void)
{
	m_wMode = 2;

	m_scale.x = 1.0f ;
	m_scale.y = 1.0f ;
}

CKeySettingTipDlg::~CKeySettingTipDlg(void)
{
}

void CKeySettingTipDlg::Linking()
{
	RECT rt;
	rt.top = rt.left = 0;
	// 070202 LYW --- CKeySettingTipDlg : Modified limited area.
	/*
	rt.right = 1024;
	rt.bottom = 768;
	*/
	DISPLAY_INFO	dispInfo ;
	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;

	rt.right = dispInfo.dwWidth;
	rt.bottom = dispInfo.dwHeight;
	// 070202 LYW --- End.

	//// 070514 LYW --- KeySettingTipDlg : Modified tip image.
	//m_KeyImage[0].LoadSprite("Data/Interface/2DImage/image/KeySetting1.tga");
	//m_KeyImage[0].SetImageSrcRect(&rt);
	//m_KeyImage[1].LoadSprite("Data/Interface/2DImage/image/KeySetting2.tga");
	//m_KeyImage[1].SetImageSrcRect(&rt);

	/*m_KeyImage[0].LoadSprite("./Data/Interface/2dimage/keysetting.tif") ;
	m_KeyImage[0].SetImageSrcRect(&rt) ;
	m_KeyImage[1].LoadSprite("./Data/Interface/2dimage/keysetting.tif") ;
	m_KeyImage[1].SetImageSrcRect(&rt) ;*/

	SCRIPTMGR->GetImage( 96, &m_KeyImage );
	SCRIPTMGR->GetImage( 97, &m_KeyImage2 );

	m_pCloseButton = (cButton*)GetWindowForID(KS_CLOSEBTN) ;
	m_pCloseButton->SetActive(TRUE) ;
	m_pCloseButton->SetDepend(TRUE) ;
}

void CKeySettingTipDlg::Render()
{
	if( !m_bActive ) return;
	if(m_wMode > 1) return;

	cDialog::RenderWindow();

	// 080617 LYW --- KeySettingTipDlg : 해상도에 따른, 닫기 버튼을 프로그램 소스에서 지정해 주던 것을,
	// 스크립트에서 수정할 수 있도록 수정 함.
	DISPLAY_INFO	dispInfo ;
	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;

	LONG lAbsX = 0 ;
	LONG lAbsY = 0 ;

	switch( dispInfo.dwWidth )
	{
	case 800 :	m_pRenderImage = &m_KeyImage2 ; break ;
	default :	m_pRenderImage = &m_KeyImage ;	break ;
	}

	if( m_pRenderImage )
	{
		m_pRenderImage->RenderSprite(&m_scale, NULL, 0, &m_absPos, RGBA_MAKE(255,255,255,255) );
	}

	cDialog::RenderComponent();



	//if( IsMovedWnd() )
	//{
	//	DISPLAY_INFO	dispInfo ;
	//	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;

	//	// 080617 LYW --- KeySettingTipDlg : 키 세팅 다이얼로그의 닫기 버튼 출력 수정.
	//	// 현재 해상도에 따른, 키 세팅 이미지 크기가 게임상에서 다르게 보이나, 
	//	// 리소스 상의 이미지 크기는 동일합니다. 리소스 크기는 각 해상도에 따라 고정이나, 
	//	// 실제로 보이는 이미지의 크기만 다를 뿐입니다. 따라서, 해상도에 따라 닫기 버튼이 
	//	// 출력 될 위치가 달라져야 할 필요가 있습니다.

	//	LONG lAbsX = 0 ;
	//	LONG lAbsY = 0 ;

	//	switch( dispInfo.dwWidth )
	//	{
	//	case 800 :
	//		{
	//			m_pRenderImage = &m_KeyImage2 ;

	//			//m_absPos.x = (float)(512 - m_KeyImage2.GetImageRect()->right  / 2) ;
	//			//m_absPos.y = (float)(256 - m_KeyImage2.GetImageRect()->bottom / 2) ;

	//			//// have to change height.
	//			//m_height = (WORD)(m_KeyImage2.GetImageRect()->bottom) ;

	//			//// setting button position.
	//			//// 이미지 가로 사이즈는 1024 이지만 실제로 사용하는 이미지는 800 이다. 때문에 800으로 고정해 줘야 함.
	//			//// 실제 이미지 가로 사이즈를 800으로 하면 이미지 깨지는 현상 있음.
	//			////lAbsX = (LONG)(m_absPos.x + 800 - 30) ;
	//			////lAbsY = (LONG)(m_absPos.y + 13) ;

	//			//lAbsX = (LONG)(m_absPos.x + 767) ;
	//			//lAbsY = (LONG)(m_absPos.y + 9) ;

	//			//m_pCloseButton->SetAbsXY( lAbsX, lAbsY ) ;
	//		}
	//		break ;

	//	case 1024 :
	//		{
	//			m_pRenderImage = &m_KeyImage ;
	//			//m_absPos.x = (float)(512 - m_KeyImage.GetImageRect()->right  / 2) ;
	//			//m_absPos.y = (float)(389 - m_KeyImage.GetImageRect()->bottom / 2) ;

	//			//// have to change height.
	//			//m_height = (WORD)(m_KeyImage.GetImageRect()->bottom) ;

	//			//// setting button position.
	//			////lAbsX = (LONG)(m_absPos.x + m_KeyImage.GetImageRect()->right - 30) ;
	//			////lAbsY = (LONG)(m_absPos.y + 13) ;
	//			//lAbsX = (LONG)(m_absPos.x + 893) ;
	//			//lAbsY = (LONG)(m_absPos.y + 9) ;

	//			//m_pCloseButton->SetAbsXY( lAbsX, lAbsY ) ;
	//		}
	//		break ; 

	//	case 1280 :
	//		{
	//			m_pRenderImage = &m_KeyImage ;
	//			//m_absPos.x = (float)(640 - m_KeyImage.GetImageRect()->right  / 2) ;
	//			//m_absPos.y = (float)(512 - m_KeyImage.GetImageRect()->bottom / 2) ;

	//			//// have to change height.
	//			//m_height = (WORD)(m_KeyImage.GetImageRect()->bottom) ;

	//			//// setting button position.
	//			////lAbsX = (LONG)(m_absPos.x + m_KeyImage.GetImageRect()->right - 30) ;
	//			////lAbsY = (LONG)(m_absPos.y + 13) ;

	//			//lAbsX = (LONG)(m_absPos.x + 893) ;
	//			//lAbsY = (LONG)(m_absPos.y + 9) ;

	//			//m_pCloseButton->SetAbsXY( lAbsX, lAbsY ) ;
	//		}
	//		break ;
	//	}

		//SetMovedWnd( FALSE ) ;
	//}

	//if( m_pRenderImage )
	//{
	//	m_pRenderImage->RenderSprite(&m_scale, NULL, 0, &m_absPos, RGBA_MAKE(255,255,255,255) );
	//}

	//cDialog::RenderComponent();
}

void CKeySettingTipDlg::OnActionEvent(LONG lId, void* p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		if(lId == KS_CLOSEBTN)
		{
			SetActive(FALSE) ;
		}
	}
}

// 071022 LYW --- KeySettingTipDlg : Add function to setting active or deactive dialog.
void CKeySettingTipDlg::SetActive(BOOL val)
{
	cDialog::SetActive( val );

	// 071022 LYW --- KeySettingTipDlg : Check open dialog.
	TUTORIALMGR->Check_OpenDialog(this->GetID(), val) ;
}
