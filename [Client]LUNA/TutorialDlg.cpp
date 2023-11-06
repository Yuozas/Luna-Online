//=================================================================================================
//	FILE		: TutorialDlg.cpp
//	PURPOSE		: Implemention part of tutorial dialog.
//	PROGRAMMER	: Yongs Lee
//	DATE		: October 16, 2007
//	ADDITION	:
//	ATTENTION	:
//=================================================================================================





//=========================================================================
//		INCLUDE HEADER FILES
//=========================================================================
#include "stdafx.h"															// ǥ�� ��������� �����Ѵ�.
#include "../WindowIDEnum.h"												// ������ ���̵� ���� ����� �����Ѵ�.

#include "./Interface/cButton.h"											// ��ư ����� �����Ѵ�.
#include "./interface/cFont.h"												// ��Ʈ ����� �����Ѵ�.
#include "./Interface\cListDialog.h"										// ����Ʈ ���̾�α� ����� �����Ѵ�.
#include "./Interface\cStatic.h"											// ����ƽ ����� �����Ѵ�.

#include "../[CC]Header/GameResourceManager.h"								// ���� ���ҽ� �Ŵ��� ����� �����Ѵ�.
#include "../ObjectManager.h"												// ������Ʈ �Ŵ��� ����� �����Ѵ�.
#include "./interface/cWindowManager.h"										// ������ �Ŵ��� ����� �����Ѵ�.
#include "./Interface/cScriptManager.h"										// ��Ʈ��Ʈ �Ŵ��� ����� �����Ѵ�.
#include "./Audio/MHAudioManager.h"											// ���� ����� �Ŵ��� ����� �����Ѵ�.
#include "TutorialManager.h"												// Ʃ�丮�� �Ŵ��� ����� �����Ѵ�.

#include "Tutorial.h"														// Ʃ�丮�� ����� �����Ѵ�.
#include "Tutorial_Msg.h"													// Ʃ�丮�� �޽��� ����� �����Ѵ�.

#include "Tutorialdlg.h"													// Ʃ�丮�� ���̾�α� ����� �����Ѵ�.





//=========================================================================
//		Ʃ�丮�� ���̾�α� ������ �Լ�.
//=========================================================================
cTutorialDlg::cTutorialDlg(void)											// ������ �Լ�.
{
	DISPLAY_INFO	dispInfo ;												// ���÷��� ������ ������ ����ü�� �����Ѵ�.

	GAMERESRCMNGR->GetDispInfo( &dispInfo ) ;								// ���÷��� ������ ���´�.

	m_nDispWidth  = dispInfo.dwWidth ;										// ���÷��� ���� ����� �����Ѵ�.
	m_nDispHeight = dispInfo.dwHeight ;										// ���÷��� ���� ����� �����Ѵ�.

	m_nBtnStartXpos = 0 ;													// ��ư ���� x��ǥ�� 0���� �����Ѵ�.
	m_nBtnStartYpos = 0 ;													// ��ư ���� y��ǥ�� 0���� �����Ѵ�.

	m_vImagePos.x = 0 ;														// ��� ��ġ x �� 0���� �����Ѵ�.
	m_vImagePos.y = 0 ;														// ��� ��ġ y �� 0���� �����Ѵ�.

	m_nImageAlpha = 0 ;														// NPC �̹����� ���� ���� 0���� �����Ѵ�.

	m_pSpeechList	= NULL ;												// Ʃ�丮�� ��� ����Ʈ ���̾�α׸� null ó���Ѵ�.

	m_pOkBtn		= NULL ;												// Ȯ�� ��ư �����͸� null ó���Ѵ�.
	m_pPrevBtn		= NULL ;												// ���� ��ư �����͸� null ó���Ѵ�.
	m_pNextBtn		= NULL ;												// ���� ��ư �����͸� null ó���Ѵ�.
	m_pPassBtn		= NULL ;												// �н� ��ư �����͸� null ó���Ѵ�.

	m_pSpeechBack	= NULL ;												// ��� ��� ����ƽ �����͸� null ó���Ѵ�.

	m_pTutorial = NULL ;													// ��� Ʃ�丮�� �����͸� null ó���� �Ѵ�.
	m_pCurSubTutorial = NULL ;												// ��� ���� ���� Ʃ�丮�� �����͸� null ó���� �Ѵ�.

	m_bShowTutorial = FALSE ;												// Ʃ�丮�� �����ֱ⸦ false�� �����Ѵ�.

	m_nCurSubIndex = 0 ;													// ���� ���� Ʃ�丮�� �ε����� 0���� �����Ѵ�.
	m_nTotalSubCount = 0 ;													// �� ���� Ʃ�丮�� ī��Ʈ�� 0���� �����Ѵ�.
}





//=========================================================================
//		Ʃ�丮�� ���̾�α� �Ҹ��� �Լ�.
//=========================================================================
cTutorialDlg::~cTutorialDlg(void)											// �Ҹ��� �Լ�.
{
}





//=========================================================================
//		Ʃ�丮�� ���̾�α� ���� ��Ʈ�� ��ũ �Լ�.
//=========================================================================
void cTutorialDlg::Linking() 												// ���̾�α� ���� ��Ʈ�ѵ��� ��ũ�ϴ� �Լ�.
{
	m_pSpeechList = (cListDialog*)GetWindowForID(TTR_SPEECH_LIST) ;			// ��� ����Ʈ ���̾�α׸� ��ũ�Ѵ�.
	m_pSpeechList->SetStyle(LDS_FONTSHADOW) ;								// �׸��� ��µǴ� ����Ʈ ��Ÿ�Ϸ� �����Ѵ�.
	m_pSpeechList->Set_ShadowFontIndex(TUTORIAL_FONTIDX) ;					// ��� ��Ʈ�� �����Ѵ�.
	m_pSpeechList->Set_ShadowDistance(2) ;									// �׸��� �Ÿ��� 2�� �����Ѵ�.

	m_pOkBtn		= (cButton*)GetWindowForID(TTR_OKBTN) ;					// Ȯ�� ��ư�� ��ũ�Ѵ�.
	m_pOkBtn->SetActive(FALSE) ;											// Ȯ�� ��ư�� ��Ȱ��ȭ �Ѵ�.

	m_pPrevBtn		= (cButton*)GetWindowForID(TTR_PREVBTN) ;				// ���� ��ư�� ��ũ�Ѵ�.
	m_pPrevBtn->SetActive(FALSE) ;											// ���� ��ư�� ��Ȱ��ȭ �Ѵ�.

	m_pNextBtn		= (cButton*)GetWindowForID(TTR_NEXTBTN) ;				// ���� ��ư�� ��ũ�Ѵ�.
	m_pNextBtn->SetActive(FALSE) ;											// ���� ��ư�� ��Ȱ��ȭ �Ѵ�.

	m_pPassBtn		= (cButton*)GetWindowForID(TTR_PASSBTN) ;				// ��� ��ư�� ��ũ�Ѵ�.
	m_pPassBtn->SetActive(FALSE) ;											// ��� ��ư�� ��Ȱ��ȭ �Ѵ�.

	m_pSpeechBack	= (cStatic*)GetWindowForID(TTR_SP_BACK) ;				// ��� ��� ����ƽ�� ��ũ�Ѵ�.
	m_pSpeechBack->SetActive(FALSE) ;										// ��� ��� ����ƽ�� ��Ȱ��ȭ �Ѵ�.
}





//=========================================================================
//		Ʃ�丮�� ���̾�α� ���� �Լ�.
//=========================================================================
void cTutorialDlg::Render()													// ���� �Լ�.
{
	if( !IsActive() ) return ;												// Ȱ��ȭ ���°� �ƴϸ�, return ó���� �Ѵ�.

	if( !m_pTutorial ) return ;												// Ʃ�丮�� ������ ��ȿ���� ������ return ó���� �Ѵ�.

	//m_nImageAlpha += 2 ;													// ���� ���� 2�� �����Ѵ�.

	//if ( m_nImageAlpha >= 255 ) m_nImageAlpha = 255 ;						// ���� ���� 255 �̻��̸�, 255�� �����Ѵ�.

	//SetAlpha((BYTE)m_nImageAlpha) ;

	if( m_nImageAlpha < 250 )
	{
		DWORD dwCurTick = GetTickCount() ;

		DWORD dwResultTime = dwCurTick - m_dwStartTime ;

		m_nImageAlpha = (int)(2.5f * (dwResultTime/10)) ;
	}
	else
	{
		m_nImageAlpha = 255 ;
	}

	m_NpcImage.RenderSprite(NULL, NULL, 0, &m_vImagePos, 
		RGBA_MAKE(255,255,255,m_nImageAlpha)) ;								// npc �̹����� ����Ѵ�.

	//int itemCount = m_pSpeechList->GetItemCount() ;

	//ITEM* pItem ;

	//for( int count = 0 ; count < itemCount ; ++count )
	//{
	//	pItem = NULL ;
	//	pItem = m_pSpeechList->GetItem(count) ;

	//	if( !pItem )
	//	{
	//		// ���� ó�� �Ѵ�.
	//		continue ;
	//	}


	//}

	cDialog::RenderWindow();												// �����츦 �����Ѵ�.
	cDialog::RenderComponent();												// ���� ��ü���� �����Ѵ�.
}





//=========================================================================
//		Ʃ�丮�� ��ư �̺�Ʈ�� ó���ϴ� �Լ�.
//=========================================================================
void cTutorialDlg::OnActionEvent(LONG lId, void* p, DWORD we) 				// ���� �� ��Ʈ�� �̺�Ʈ�� ó�� �� �Լ�.
{
	//int nCondition1 = m_pCurSubTutorial->nComplete_Condition1 ;				// ù��° �Ϸ� ������ �޴´�.
	//int nCondition2 = m_pCurSubTutorial->nComplete_Condition2 ;				// �ι�° �Ϸ� ������ �޴´�.

	switch(lId)																// ��Ʈ�� ���̵� Ȯ���Ѵ�.
	{
	case TTR_OKBTN :														// Ȯ�� ��ư�� �����ٸ�,
		{
			m_pTutorial->Set_Complete(TRUE) ;
			UpdateTutorial() ;												// Ʃ�丮���� �����ϰ�, ������Ʈ �Ѵ�.
		}
		break ;

	case TTR_PREVBTN :														// ���� ��ư�� �����ٸ�,
		{
			if( m_nCurSubIndex <= 0 )
			{
				// ����ó�� �Ѵ�~~~!!!
				// 0�� ���� ������ ���� ��ư�� ��??!!!
				return ;
			}

			SUB_TUTORIAL* pPrevSubTutorial = NULL ;
			pPrevSubTutorial = m_pTutorial->Get_SubTutorial(m_nCurSubIndex-1) ;		// ���� ���� Ʃ�丮�� ������ �޴´�.

			if( pPrevSubTutorial )											// ���� ���� Ʃ�丮���� �������,
			{
				--m_nCurSubIndex ;											// ���� Ʃ�丮�� �ε����� �����Ѵ�.
				Prepare_Tutorial() ;										// Ʃ�丮�� ������ �غ��Ѵ�.
			}
			else															// ���� ���� Ʃ�丮���� �������,
			{
				// ���� ó�� �ؾ� �Ѵ�.
				int a = 0 ;
				return ;
			}
		}
		break ;

	case TTR_NEXTBTN :														// ���� ��ư�� �����ٸ�,
		{
			SUB_TUTORIAL* pNextSubTutorial = NULL ;
			pNextSubTutorial = m_pTutorial->Get_SubTutorial(m_nCurSubIndex+1) ;		// ���� ���� Ʃ�丮�� ������ �޴´�.

			if( pNextSubTutorial )											// ���� ���� Ʃ�丮���� �������,
			{
				++m_nCurSubIndex ;											// ���� Ʃ�丮�� �ε����� �����Ѵ�.
				Prepare_Tutorial() ;										// Ʃ�丮�� ������ �غ��Ѵ�.
			}
			else															// ���� ���� Ʃ�丮���� �������,
			{
				// ���� ó�� �ؾ� �Ѵ�.
				int a = 0 ;
				return ;
			}
		}
		break ;

	case TTR_PASSBTN :														// ��� ��ư�� �����ٸ�,
		{
			m_pTutorial->Set_Complete(TRUE) ;
			UpdateTutorial() ;												// Ʃ�丮���� �����ϰ�, ������Ʈ �Ѵ�.
		}
		break ;
	}
}





//=========================================================================
//		Ʃ�丮���� �����ϴ� �Լ�.
//=========================================================================
void cTutorialDlg::Set_TutorialInfo(cTutorial* pTutorial) 					// Ʃ�丮�� ������ �����ϴ� �Լ�.
{
	if( !pTutorial )
	{
		#ifdef _GMTOOL_														// GM���� ���,
		MessageBox(NULL, TDERR_MSG1, "cTutorialDlg::Start_Tutorial", MB_OK);// ���ڷ� �Ѿ�� Ʃ�丮�� ���� ��ȿ~!!
		#endif

		return ;															// return ó���� �Ѵ�.
	}

	m_pTutorial = pTutorial ;												// ��� Ʃ�丮�� �����ͷ� Ʃ�丮�� ������ �޴´�.
}





//=========================================================================
//		Ʃ�丮���� �����ϴ� �Լ�.
//=========================================================================
void cTutorialDlg::Start_Tutorial() 										// Ʃ�丮�� �����Լ�.
{
	m_nCurSubIndex = 0 ;													// ���� ���� Ʃ�丮�� �ε����� 0���� �����Ѵ�.
	m_nTotalSubCount = m_pTutorial->Get_SubTutorial_Count() ;				// ���� Ʃ�丮���� ���� Ʃ�丮�� ī��Ʈ�� �޴´�.

	Prepare_Tutorial() ;													// Ʃ�丮�� ������ �غ��Ѵ�.

	SetActive(TRUE) ;														// Ʃ�丮�� ���̾�α׸� Ȱ��ȭ �Ѵ�.

	m_pSpeechBack->SetActive(TRUE) ;										// ��� ����� Ȱ��ȭ �Ѵ�.

	m_nImageAlpha = 0 ;														// npc �̹����� ���� ���� 0���� �����Ѵ�.

	m_dwStartTime = GetTickCount() ;

	cDialog* npcScriptDialog	= WINDOWMGR->GetWindowForID( NSI_SCRIPTDLG );// npc ��ȭâ�� ������ �޴´�.

	if( !npcScriptDialog ) return ;											// npc ��ȭâ ������ ��ȿ���� ������ return ó���� �Ѵ�.

	if( npcScriptDialog->IsActive() ) npcScriptDialog->SetActive(FALSE) ;	// npc ��ȭâ�� ������ ���¸�, �ݴ´�.
}





//=========================================================================
//		Ʃ�丮���� �����ϴ� �Լ�.
//=========================================================================
//void cTutorialDlg::Start_Tutorial(cTutorial* pTutorial) 					// Ʃ�丮�� �����Լ�.
//{
//	if( !pTutorial )
//	{
//		#ifdef _GMTOOL_														// GM���� ���,
//		MessageBox(NULL, TDERR_MSG1, "cTutorialDlg::Start_Tutorial", MB_OK);// ���ڷ� �Ѿ�� Ʃ�丮�� ���� ��ȿ~!!
//		#endif
//
//		return ;															// return ó���� �Ѵ�.
//	}
//
//	m_pTutorial = pTutorial ;												// ��� Ʃ�丮�� �����ͷ� Ʃ�丮�� ������ �޴´�.
//
//	m_nCurSubIndex = 0 ;													// ���� ���� Ʃ�丮�� �ε����� 0���� �����Ѵ�.
//	m_nTotalSubCount = pTutorial->Get_SubTutorial_Count() ;					// ���� Ʃ�丮���� ���� Ʃ�丮�� ī��Ʈ�� �޴´�.
//
//	Prepare_Tutorial() ;													// Ʃ�丮�� ������ �غ��Ѵ�.
//
//	SetActive(TRUE) ;														// Ʃ�丮�� ���̾�α׸� Ȱ��ȭ �Ѵ�.
//
//	m_nImageAlpha = 0 ;														// npc �̹����� ���� ���� 0���� �����Ѵ�.
//
//	cDialog* npcScriptDialog	= WINDOWMGR->GetWindowForID( NSI_SCRIPTDLG );// npc ��ȭâ�� ������ �޴´�.
//
//	if( !npcScriptDialog ) return ;											// npc ��ȭâ ������ ��ȿ���� ������ return ó���� �Ѵ�.
//
//	if( npcScriptDialog->IsActive() ) npcScriptDialog->SetActive(FALSE) ;	// npc ��ȭâ�� ������ ���¸�, �ݴ´�.
//}





//=========================================================================
//		Ʃ�丮�� ������ ���� �غ� ó�� �Լ�.	
//=========================================================================
void cTutorialDlg::LoadNpcImage() 											// ncp �̹����� �ε��ϴ� �Լ�.
{
	if( !m_pCurSubTutorial )												// ���� ���� Ʃ�丮���� ��ȿ���� ������,
	{
		#ifdef _GMTOOL_														// GM���� ���,
		MessageBox(NULL, TDERR_MSG6, "cTutorialDlg::LoadNpcImage", MB_OK) ;	// ���� ���� Ʃ�丮�� ���� ��ȿ~!!
		#endif
	}

	CMHFile file;															// ���� ������ �����Ѵ�.

	file.Init("Data/Interface/Windows/NpcImageList.bin","rb");				// npc �̹��� ����Ʈ�� �б���� ����.

	if(file.IsInited() == FALSE)											// ���⿡ �����ϸ�,
	{
		#ifdef _GMTOOL_														// GM���� ���,
		MessageBox(NULL, TERR_MSG2, "cTutorialDlg::LoadNpcImage", MB_OK) ;	// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

		return ;															// return ó���� �Ѵ�.
	}

	char buf[256];															// �ӽ� ���۸� �����Ѵ�.

	WORD wIndex;															// �ε����� ���� ������ �����Ѵ�.

	BYTE emotion;															// ����� ���� ������ �����Ѵ�.

	memset(&m_NpcImage, 0, sizeof(cImageSelf)) ;							// ���� �̹����� �޸� �� �Ѵ�.

	while(!file.IsEOF())													// while���� ������.
	{
		wIndex  = file.GetWord();											// �ε����� �о���δ�.
		emotion = file.GetByte();											// ����� �о���δ�.
		char*	szFileName	=	file.GetString();							// ���ϸ��� �����Ѵ�.

		if( strlen(szFileName) == 0 )										// ���ϸ��� 0�� ���ٸ�, while���� Ż���Ѵ�.
		{
			#ifdef _GMTOOL_													// GM���� ���,
			char tempBuf[128] = {0, } ;										// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
			sprintf(tempBuf, TDERR_MSG7, wIndex, emotion) ;					// npc �̹��� ���ϸ� �ޱ� ����~!!
			MessageBox(NULL, tempBuf, "cTutorialDlg::LoadNpcImage", MB_OK) ;// ���� �޽����� �޽��� �ڽ��� ����.
			#endif

			return ;														// return ó���� �Ѵ�.
		}

		if( wIndex != m_pCurSubTutorial->nNpc_Idx ) continue ;				// npc �ε����� ���� ������ continue ó���� �Ѵ�.

		if( emotion != m_pCurSubTutorial->nFace_Idx ) continue ;			// emotion �ε����� ���� ������, continue ó���� �Ѵ�.

		sprintf( buf, "%s%s", NPC_IMAGE_PATH, szFileName);					// �ӽ� ���ۿ� ��ι�, ���ϸ��� �����Ѵ�.
			
		if( !m_NpcImage.LoadSprite(buf) )									// npc �̹��� �ε忡 ���� �ߴٸ�,
		{
			#ifdef _GMTOOL_													// GM���� ���,
			char tempBuf[128] = {0, } ;										// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
			sprintf(tempBuf, TDERR_MSG8, wIndex, emotion) ;					// npc ��������Ʈ �̹��� �ε� ����~!!
			MessageBox(NULL, tempBuf, "cTutorialDlg::LoadNpcImage", MB_OK) ;// ���� �޽����� �޽��� �ڽ��� ����.
			#endif
		}

		int nWidth = m_NpcImage.GetImageRect()->right ;						// �̹����� ���� ���̸� �޴´�.
		int nHeight = m_NpcImage.GetImageRect()->bottom ;					// �̹����� ���� ���̸� �޴´�.

		m_vImagePos.x = (float)(m_nDispWidth - nWidth) ;					// ��� ��ġ X��ǥ�� �����Ѵ�.
		m_vImagePos.y = (float)(m_nDispHeight - nHeight) ;					// ��� ��ġ Y��ǥ�� �����Ѵ�.

		return ;															// return ó���� �Ѵ�.
	}
}





//=========================================================================
//		Ʃ�丮�� ������ ���� �غ� ó�� �Լ�.	
//=========================================================================
void cTutorialDlg::Prepare_Tutorial() 										// Ʃ�丮�� ������ �̸� �غ��ϴ� �Լ�.
{
	m_pCurSubTutorial = NULL ;												// ���� ���� Ʃ�丮���� ���� �����͸� null ó���� �Ѵ�.
	m_pCurSubTutorial = m_pTutorial->Get_SubTutorial(m_nCurSubIndex) ;		// ���� ���� Ʃ�丮�� ������ �޴´�.

	if( !m_pTutorial )														// Ʃ�丮�� ������ ��ȿ���� ������,
	{
		#ifdef _GMTOOL_														// GM���� ���,
		char tempBuf[128] = {0, } ;											// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
		MessageBox(NULL, TDERR_MSG1, "cTutorialDlg::Prepare_Tutorial", MB_OK);	// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

		return ;															// return ó���� �Ѵ�.
	}

	if( !m_pCurSubTutorial )												// ���� Ʃ�丮�� ������ �޴µ� ���� �ߴٸ�,
	{
		#ifdef _GMTOOL_														// GM���� ���,
		char tempBuf[128] = {0, } ;											// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
		sprintf(tempBuf, TDERR_MSG2, m_pTutorial->Get_Index()) ;			// ���� Ʃ�丮�� �ޱ� ����~!!
		MessageBox(NULL, tempBuf, "cTutorialDlg::Prepare_Tutorial", MB_OK);	// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

		return ;															// return ó���� �Ѵ�.
	}

	int nMsgIndex = m_pCurSubTutorial->nSpeech_Idx ;						// ���� ���� Ʃ�丮���� �޽��� ��ȣ�� �޴´�.
	cTutorial_Msg* pMsg = TUTORIALMGR->Get_TutorialMsg( nMsgIndex ) ;		// ���� ���� Ʃ�丮���� �޽����� �޴´�.

	if( !pMsg )																// �޽����� �޴µ� �����ߴٸ�,
	{
		#ifdef _GMTOOL_														// GM���� ���,
		char tempBuf[128] = {0, } ;											// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
		sprintf(tempBuf, TDERR_MSG3, nMsgIndex) ;							// Ʃ�丮�� �޽��� �ޱ� ����~!!
		MessageBox(NULL, tempBuf, "cTutorialDlg::Prepare_Tutorial", MB_OK);	// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

		return ;															// return ó���� �Ѵ�.
	}

	m_pSpeechList->RemoveAll() ;											// ��� ��� ����Ʈ�� ��� ����.

	//ResetSpeechMsgList() ;													// Ʃ�丮�� ��� ����Ʈ�� �ʱ�ȭ �Ѵ�.

	int nMsgCount = 0 ;														// �޽��� ī��Ʈ�� ���� ������ �����ϰ� 0���� �ʱ�ȭ �Ѵ�.
	nMsgCount = pMsg->Get_LineCount() ;										// �޽ñ� ī��Ʈ�� �޴´�.

	if( nMsgCount < 0 )
	{
		#ifdef _GMTOOL_														// GM���� ���,
		char tempBuf[128] = {0, } ;											// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
		sprintf(tempBuf, TDERR_MSG4, nMsgIndex) ;							// �޽����� ������ ����~!!
		MessageBox(NULL, tempBuf, "cTutorialDlg::Prepare_Tutorial", MB_OK);	// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

		return ;															// return ó���� �Ѵ�.
	}

	MSG_LINE* pMsgLine ;													// �޽��� ������ ���� �����͸� �����Ѵ�.
	for( int count = 0 ; count < nMsgCount ; ++count )						// �޽��� ���� �� ��ŭ for���� ������.
	{
		pMsgLine = NULL ;													// �޽��� ���� �����͸� null ó���� �Ѵ�.
		pMsgLine = pMsg->Get_MsgLine(count) ;								// ī��Ʈ�� �ش��ϴ� �޽��� ������ �޴´�.

		if( !pMsgLine )
		{
			#ifdef _GMTOOL_													// GM���� ���,
			char tempBuf[128] = {0, } ;										// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
			sprintf(tempBuf, TDERR_MSG5, nMsgIndex, count) ;				// �޽��� ���� �ޱ� ����~!!
			MessageBox(NULL, tempBuf, "cTutorialDlg::Prepare_Tutorial", MB_OK);// ���� �޽����� �޽��� �ڽ��� ����.
			#endif

			continue ;														// continue ó���� �Ѵ�.
		}

		m_pSpeechList->AddItem(pMsgLine->msg, RGB(255, 255, 255), count) ;	// Ʃ�丮�� ��縦 �߰��Ѵ�.

        if( count != (nMsgCount - 1) ) continue ;							// ���� ī��Ʈ��, ������ ī��Ʈ�� �ƴϸ� continue ó���� �Ѵ�.

		m_nBtnStartXpos = int(m_pSpeechList->GetAbsX() + 
						  CFONT_OBJ->GetTextWidth(TUTORIAL_FONTIDX) + 
					      CFONT_OBJ->GetTextExtentWidth( TUTORIAL_FONTIDX, 
						  pMsgLine->msg, strlen(pMsgLine->msg)) ) ;			// ��ư ����� ���� x��ǥ�� ���Ѵ�.

		m_nBtnStartYpos = int(m_pSpeechList->GetAbsY() + 6 + 
						  (CFONT_OBJ->GetTextHeight(TUTORIAL_FONTIDX) *
						  (nMsgCount-1)));									// ��ư ����� ���� y��ǥ�� ���Ѵ�.
	}

	DeActiveAllBtn() ;														// ��� ��ư���� ��Ȱ��ȭ �Ѵ�.

	int nCompleteCondition = 0 ;											// ù ��° �Ϸ� ������ ���� ������ �����Ѵ�.
	nCompleteCondition = m_pCurSubTutorial->nComplete_Condition1 ;			// ù ���� �Ϸ� ������ �޴´�.

	switch(nCompleteCondition)												// �Ϸ� ������ Ȯ���Ѵ�.
	{
	case e_CLICK_OKBTN :	ActiveBtns() ; break ;							// ��ư Ŭ������ �Ϸ��ϴ� ��Ȳ�̸�, ��Ȳ�� �´� ��ư�� Ȱ��ȭ ��Ų��.

	case e_NPC_TALK :break ;												// npc�� ��ȭ�� �ؼ� �Ϸ��ؾ� �ϴ� ��Ȳ�̸�,

	case e_OPEN_DIALOG :													// â�� ���� �Ϸ��ؾ� �ϴ� ��Ȳ�̸�,
		{
			cWindow* pWindow = WINDOWMGR->GetWindowForID(IDSEARCH(m_pCurSubTutorial->szCompleteCondition2)) ;

			if( pWindow )
			{
				pWindow->SetActive(FALSE) ;
			}
		}
		break ;											
	}


	LoadNpcImage() ;														// Ʃ�丮�� npc �̹����� �ε��Ѵ�.

	AUDIOMGR->StopAll() ;													// play ���̴� ���带 ����.

	int nSoundIdx = m_pCurSubTutorial->nSound_Idx ;							// ���� ���� Ʃ�丮�� ������ ���� �ε����� �޴´�.

	if( nSoundIdx < 0 )														// ���� �ε����� 0�����̸�,
	{
		#ifdef _GMTOOL_														// GM���� ���,
		char tempBuf[128] = {0, } ;											// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
		sprintf(tempBuf, TDERR_MSG10, m_pTutorial->Get_Index(), 
			m_pCurSubTutorial->nIndex, m_pCurSubTutorial->nSound_Idx) ;		// npc �̹��� ���ϸ� �ޱ� ����~!!
		MessageBox(NULL, tempBuf, "cTutorialDlg::Prepare_Tutorial", MB_OK) ;// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

		return ;															// return ó���� �Ѵ�.
	}

	AUDIOMGR->Play(nSoundIdx, HERO);										// ���� ���� Ʃ�丮���� ���带 play �Ѵ�.
}





//=========================================================================
//		Ʃ�丮�� ��� ����Ʈ�� �ʱ�ȭ �ϴ� �Լ�.
//=========================================================================
void cTutorialDlg::ResetSpeechMsgList() 									// ��� ����Ʈ�� �ٽ� �����ϴ� �Լ�.
{
	MSG_LINE* pMsgLine ;													// �޽��� ������ ���� �����͸� �����Ѵ�.
	
	PTRLISTPOS pos = NULL ;													// ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pos = m_SpeechMsgList.GetHeadPosition() ;								// Ʃ�丮�� ���� ��� ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	while(pos)																// ��ġ ������ ��ȿ�� ���� while���� ������.
	{
		pMsgLine = NULL ;													// �޽��� ���� �����͸� null ó���� �Ѵ�.
		pMsgLine = (MSG_LINE*)m_SpeechMsgList.GetNext(pos) ;				// �޽��� ���� ������ �ް�, ��ġ�� �������� �����Ѵ�.

		if( !pMsgLine ) continue ;											// �޽��� ���� ������ ������, continue ó���� �Ѵ�.

		delete pMsgLine ;													// �޽��� ������ �����Ѵ�.
	}

	m_SpeechMsgList.RemoveAll() ;											// Ʃ�丮�� ���� ��� ����Ʈ�� ��� ����.
}





//=========================================================================
//		��� ��ư�� ��Ȱ��ȭ �ϴ� ��ư.
//=========================================================================
void cTutorialDlg::DeActiveAllBtn() 										// ��� ��ư�� ��Ȱ��ȭ �ϴ� �Լ�.
{
	m_pOkBtn->SetActive(FALSE) ;											// Ȯ�� ��ư�� ��Ȱ��ȭ �Ѵ�.
	m_pPrevBtn->SetActive(FALSE) ;											// ���� ��ư�� ��Ȱ��ȭ �Ѵ�.
	m_pNextBtn->SetActive(FALSE) ;											// ���� ��ư�� ��Ȱ��ȭ �Ѵ�.
	m_pPassBtn->SetActive(FALSE) ;											// ��� ��ư�� ��Ȱ��ȭ �Ѵ�.
}





//=========================================================================
//		��Ȳ�� �´� ��ư�� Ȱ��ȭ �ϴ� �Լ�.
//=========================================================================
void cTutorialDlg::ActiveBtns() 											// ��Ȳ�� �´� ��ư�� Ȱ��ȭ �ϴ� �Լ�.
{
	int nCurSubIndex = m_pCurSubTutorial->nIndex ;							// ���� ���� Ʃ�丮���� �ε����� ���Ѵ�.
	int nLastSubIndex = m_nTotalSubCount-1 ;								// ������ ���� Ʃ�丮���� �ε����� ���Ѵ�.

	int nBtnDistance = 5 ;													// ��ư ���� �Ÿ� ���� 5�� �����Ѵ�.

	if( nCurSubIndex == 0 )													// ���� ���� Ʃ�丮�� �ε����� 0�� ������,
	{
		if( nCurSubIndex == nLastSubIndex )									// ó������ �������̸�,
		{
			m_pOkBtn->SetAbsXY(m_nBtnStartXpos, m_nBtnStartYpos) ;			// ok ��ư�� ��ġ�� ���,
			m_pOkBtn->SetActive(TRUE) ;										// Ȱ��ȭ ��Ų��.
		}
		else																// �ټ� ������ �� ó���̸�,
		{
			m_pNextBtn->SetAbsXY(m_nBtnStartXpos, m_nBtnStartYpos) ;		// ok ��ư�� ��ġ�� ���,
			m_pNextBtn->SetActive(TRUE) ;									// Ȱ��ȭ ��Ų��.

			int nSecontX = m_nBtnStartXpos + m_pNextBtn->GetWidth() + nBtnDistance ;	// ���� ��ư�� ���� ��ǥ�� ���Ѵ�.

			m_pPassBtn->SetAbsXY(nSecontX, m_nBtnStartYpos) ;				// ���� ��ư�� ��ġ�� ���,
			m_pPassBtn->SetActive(TRUE) ;									// Ȱ��ȭ ��Ų��.
		}
	}
	else if( nCurSubIndex == nLastSubIndex )								// ���� ���� Ʃ�丮���� ������ Ʃ�丮���̸�,
	{
		if(m_nTotalSubCount > 1)											// ���� Ʃ�丮�� �� �������� 1������ �̻��̸�,
		{
			m_pPrevBtn->SetAbsXY(m_nBtnStartXpos, m_nBtnStartYpos) ;		// ���� ��ư�� ��ġ�� ���,
			m_pPrevBtn->SetActive(TRUE) ;									// Ȱ��ȭ ��Ų��.

			int nSecontX = m_nBtnStartXpos + m_pPrevBtn->GetWidth() + nBtnDistance ;	// Ȯ�� ��ư�� ��ǥ�� ���Ѵ�.

			m_pOkBtn->SetAbsXY(nSecontX, m_nBtnStartYpos) ;					// Ȯ�� ��ư�� ��ġ�� ���,
			m_pOkBtn->SetActive(TRUE) ;										// Ȱ��ȭ ��Ų��.
		}
		else																// ���� ���� Ʃ�丮�� �������� ������ ���������,
		{
			m_pOkBtn->SetAbsXY(m_nBtnStartXpos, m_nBtnStartYpos) ;			// Ȯ�� ��ư�� ��ġ�� ���,
			m_pOkBtn->SetActive(TRUE) ;										// Ȱ��ȭ ��Ų��.
		}
	}
	else																	// ù�������� ������ �������� �ƴϸ�,
	{		
		m_pPrevBtn->SetAbsXY(m_nBtnStartXpos, m_nBtnStartYpos) ;			// ���� ��ư�� ��ġ�� ��´�.
		m_pPrevBtn->SetActive(TRUE) ;										// Ȱ��ȭ ��Ų��.
	
		int nSecontX = m_nBtnStartXpos + m_pPrevBtn->GetWidth() + nBtnDistance ;	// ���� ��ư�� ���� ��ǥ�� ���Ѵ�.

		m_pNextBtn->SetAbsXY(nSecontX, m_nBtnStartYpos) ;					// ���� ��ư�� ��ġ�� ���,
		m_pNextBtn->SetActive(TRUE) ;										// Ȱ��ȭ ��Ų��.

		int nThirdX = nSecontX + m_pNextBtn->GetWidth() + nBtnDistance ;	// ��� ��ư�� ���� ��ǥ�� ���Ѵ�.

		m_pPassBtn->SetAbsXY(nThirdX, m_nBtnStartYpos) ;					// ��� ��ư�� ��ġ�� ���,
		m_pPassBtn->SetActive(TRUE) ;										// Ȱ��ȭ ��Ų��.
	}
}





//=========================================================================
//		Ʃ�丮�� ���� ���ǰ� ���� npc ���� üũ�ϴ� �Լ�.
//=========================================================================
BOOL cTutorialDlg::IsSameEndNpc(DWORD dwNpcIdx) 							// Ʃ�丮�� ���� ���ǰ� ���� npc ���� üũ�ϴ� �Լ�.
{
	if( !m_pCurSubTutorial ) return FALSE ;

	int nEndCondition = 0 ;													// ù ��° �Ϸ� ������ ���� ������ �����Ѵ�.
	nEndCondition = m_pCurSubTutorial->nComplete_Condition1 ;				// ù ���� �Ϸ� ������ �޴´�.

	if( nEndCondition != e_NPC_TALK )										// �Ϸ� ���� ù��°�� npc�� ��ȭ�� �ƴϸ�,
	{
		//#ifdef _GMTOOL_														// GM���� ���,
		//char tempBuf[64] = {0, } ;											// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
		//sprintf(tempBuf, TDERR_MSG9, dwNpcIdx) ;							// �޽��� ���� �ޱ� ����~!!
		//MessageBox(NULL, tempBuf, "cTutorialDlg::IsSameEndNpc", MB_OK);		// ���� �޽����� �޽��� �ڽ��� ����.
		//#endif

		return FALSE ;														// false return ó���� �Ѵ�.
	}

	int nEndCondition2 = 0 ;												// �� ��° �Ϸ� ������ ���� ������ �����Ѵ�.
	nEndCondition2 = m_pCurSubTutorial->nComplete_Condition2 ;				// �� ���� �Ϸ� ������ �޴´�.

	if( dwNpcIdx != nEndCondition2 )										// �Ϸ� ������ npc�� ���ڷ� �Ѿ�� npc �ε����� �ٸ���,
	{
		return FALSE ;														// false return ó���� �Ѵ�.
	}

	return TRUE ;															// true return ó���� �Ѵ�.
}





//=========================================================================
//		���� ���� Ʃ�丮���� �ִ��� ���θ� üũ�ϴ� �Լ�.
//=========================================================================
BOOL cTutorialDlg::IsHavePrevSubTutorial() 									// ���� ���� Ʃ�丮���� �ִ��� ���θ� üũ�ϴ� �Լ�.
{
	if( m_nCurSubIndex == 0 ) return FALSE ;								// ���� ���� Ʃ�丮�� �ε����� 0�� ������ false return ó���� �Ѵ�.

	SUB_TUTORIAL* pPrevSubTutorial = NULL ;									// ���� Ʃ�丮�� ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pPrevSubTutorial = m_pTutorial->Get_SubTutorial(m_nCurSubIndex-1) ;		// ���� ���� Ʃ�丮�� ������ �޴´�.

	if( pPrevSubTutorial )													// ���� ���� Ʃ�丮���� �������,
	{
		return TRUE ;														// true return ó���� �Ѵ�.
	}

	return FALSE ;															// false return ó���� �Ѵ�.
}





//=========================================================================
//		���� ���� Ʃ�丮���� �ִ��� ���θ� üũ�ϴ� �Լ�.
//=========================================================================
BOOL cTutorialDlg::IsHaveNextSubTutorial()									// ���� ���� Ʃ�丮���� �ִ��� ���θ� üũ�ϴ� �Լ�.
{
	SUB_TUTORIAL* pNextSubTutorial = NULL ;									// ���� ���� Ʃ�丮�� ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pNextSubTutorial = m_pTutorial->Get_SubTutorial(m_nCurSubIndex+1) ;		// ���� ���� Ʃ�丮�� ������ �޴´�.

	if( pNextSubTutorial ) return TRUE ;									// ���� ���� Ʃ�丮�� ������ ������ ture return ó���� �Ѵ�.

	return FALSE ;															// false return ó���� �Ѵ�.
}





//=========================================================================
//		���� ���� Ʃ�丮���� �����ϴ� �Լ�.
//=========================================================================
void cTutorialDlg::StartNextSubTutorial() 									// ���� ���� Ʃ�丮���� �����ϴ� �Լ�.
{
	SUB_TUTORIAL* pNextSubTutorial = NULL ;									// ���� ���� Ʃ�丮�� ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pNextSubTutorial = m_pTutorial->Get_SubTutorial(m_nCurSubIndex+1) ;		// ���� ���� Ʃ�丮�� ������ �޴´�.

	if( pNextSubTutorial )													// ���� ���� Ʃ�丮���� �������,
	{
		++m_nCurSubIndex ;													// ���� Ʃ�丮�� �ε����� �����Ѵ�.
		Prepare_Tutorial() ;												// Ʃ�丮�� ������ �غ��Ѵ�.
	}
}





//=========================================================================
//		���� ���� Ʃ�丮���� �����ϴ� �Լ�.
//=========================================================================
void cTutorialDlg::UpdateTutorial() 										// Ʃ�丮���� �Ϸ��ϰ� ������Ʈ �ϴ� �Լ�.
{
	m_pSpeechBack->SetActive(FALSE) ;										// ��� ����� ��Ȱ��ȭ �Ѵ�.

	SetActive(FALSE) ;														// Ʃ�丮�� â�� ��Ȱ��ȭ �Ѵ�.

//	m_pTutorial->Set_Complete(TRUE) ;										// Ʃ�丮���� �ϷḦ true�� �����Ѵ�.
	TUTORIALMGR->UpdateTutorial(m_pTutorial->Get_Index()) ;					// ���� Ʃ�丮���� ������Ʈ �Ѵ�.

	//m_pTutorial = NULL ;
	//m_pCurSubTutorial = NULL ;
}





//=========================================================================
//		���� ���� Ʃ�丮���� ��ȯ�ϴ� �Լ�.
//=========================================================================
SUB_TUTORIAL* cTutorialDlg::GetCurSubTutorial()								// ���� ���� Ʃ�丮���� ��ȯ�ϴ� �Լ�.
{
	if( !m_pCurSubTutorial ) return NULL ;									// ���� ���� Ʃ�丮���� ��ȿ���� ������, null return ó���� �Ѵ�.

	return m_pCurSubTutorial ;												// ���� ���� Ʃ�丮���� return �Ѵ�.
}





