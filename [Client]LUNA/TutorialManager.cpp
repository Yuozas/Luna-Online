//=================================================================================================
//	FILE		: TutorialManager.cpp
//	PURPOSE		: Implemention part of tutorial manager class.
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

#include "GameIn.h"															// ���� �� ����� �����Ѵ�.

#include "./Audio/MHAudioManager.h"											// ���� ����� �Ŵ��� ��� ������ �����Ѵ�.
#include "ObjectManager.h"													// ������Ʈ �Ŵ��� ��� ������ �����Ѵ�.
#include "./Tutorialmanager.h"												// Ʃ�丮�� �Ŵ����� ���ǵ� ��� ������ �����Ѵ�.

#include "./interface/cWindowManager.h"										// ������ �Ŵ��� ��� ������ �����Ѵ�.
#include "./Interface/cStatic.h"											// ����ƽ ����� �����Ѵ�.

#include "../InventoryExDialog.h"											// �κ��丮 ���̾�α� ����� �����Ѵ�.

#include "Item.h"															// ������ Ŭ���� ����� �����Ѵ�.
#include "Tutorial.h"														// Ʃ�丮�� Ŭ���� ����� �����Ѵ�.
#include "Tutorial_Msg.h"													// Ʃ�丮�� �޽��� ����� �����Ѵ�.
#include "TutorialDlg.h"													// Ʃ�丮�� ���̾�α� ����� �����Ѵ�.
#include "TutorialBtnDlg.h"													// Ʃ�丮�� ��ư ���̾�α� ����� �����Ѵ�.





//=========================================================================
//		
//=========================================================================
GLOBALTON(cTutorialManager)





//=========================================================================
//		Ʃ�丮�� �Ŵ��� ������ �Լ�.
//=========================================================================
cTutorialManager::cTutorialManager(void)									// ������ �Լ�.
{
	m_bActiveTutorial = TRUE ;												// Ʃ�丮���� Ȱ��ȭ �Ѵ�.

	m_bUpdated = FALSE ;													// ������Ʈ ���θ� false�� �����Ѵ�.

	m_nCurTutorialIdx	= 0 ;												// ���� Ʃ�丮�� �ε����� 0�� �����Ѵ�.

	m_nShow_BtnSndIdx = 0 ;													// ��ư �⿬ ���� �ε����� 0���� �����Ѵ�.

	m_nEndTutorial_SndIdx = 0 ;												// Ʃ�丮�� �Ϸ� ���� �ε����� 0���� �����Ѵ�.
}





//=========================================================================
//		Ʃ�丮�� �Ŵ��� �Ҹ��� �Լ�.
//=========================================================================
cTutorialManager::~cTutorialManager(void)									// �Ҹ��� �Լ�.
{
	Release() ;																// ���� �Լ��� ȣ���Ѵ�.
}





//=========================================================================
//		Ʃ�丮�� �Ŵ��� �ʱ�ȭ �Լ�.
//=========================================================================
void cTutorialManager::Init() 												// �ʱ�ȭ �Լ�.
{
	if( !LoadMsg(TUTORIAL_MSG_PATH) )										// Ʃ�丮�� �޽��� �ε忡 �����ϸ�,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		MessageBox(NULL, ERR_MSG, INIT_CATION, MB_OK);						// �޽��� ���� ���� ���� �޽����� ����.
		#endif
	}

	if( !LoadTutorial(TUTORIAL_SCRIPT_PATH) )								// Ʃ�丮�� ��ũ��Ʈ �ε忡 �����ϸ�,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		MessageBox(NULL, ERR_TUTORIAL, INIT_CATION, MB_OK);					// �޽��� ���� ���� ���� �޽����� ����.
		#endif
	}

	if( !LoadHelperInfo(HELPER_LINKLIST_PATH) )
	{
		#ifdef _GMTOOL_														// GM�� �����,
		MessageBox(NULL, ERR_HELPERLINK, INIT_CATION, MB_OK);				// �޽��� ���� ���� ���� �޽����� ����.
		#endif
	}
}





//=========================================================================
//		Ʃ�丮�� �Ŵ��� ���� �Լ�.
//=========================================================================
void cTutorialManager::Release() 											// ���� �Լ�.
{
	ReleaseMsg() ;															// Ʃ�丮�� �޽��� ���� �Լ��� ȣ���Ѵ�.
	ReleaseTutorial() ;														// Ʃ�丮�� ���� ���� �Լ��� ȣ���Ѵ�.
	ReleaseHelperInfo() ;													// ���� ���� ���� �Լ��� ȣ���Ѵ�.
}





//=========================================================================
//		Ʃ�丮�� ������ �ε��ϴ� �Լ�.
//=========================================================================
BOOL cTutorialManager::LoadTutorial(char* strFileName) 						// Ʃ�丮�� ������ �ε��ϴ� �Լ�.
{
	CMHFile file ;															// ���� ������ �����Ѵ�.

	if( !strFileName )														// ���ڷ� �Ѿ�� ���ϸ��� ��ȿ���� ������,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		MessageBox(NULL, TERR_MSG1, LT_CATION, MB_OK);						// �߸� �� ���ϸ��̶�� �޽��� �ڽ��� ����.
		#endif			

		return FALSE ;														// false return ó���� �Ѵ�.
	}

	if( !file.Init( strFileName, "rb") )									// ���� �����̸���, ���� ��忡 ���� ������ ����, ����� �޴´�.
	{																		// �����ߴٸ�,
		#ifdef _GMTOOL_														// GM�� �����,
		char tempBuf[128] = {0, } ;											// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
		sprintf(tempBuf, TERR_MSG2, strFileName) ;							// ���� �޽����� �ϼ��Ѵ�.
		MessageBox(NULL, tempBuf, LT_CATION, MB_OK);						// ���Ͽ��⿡ �����ߴٴ� �޽��� �ڽ��� ����.
		#endif

		return FALSE;														// false return ó���� �Ѵ�.
	}

	int nCount = 0 ;														// ī��Ʈ ������ �����ϰ� 0���� �ʱ�ȭ �Ѵ�.

	char Token[256];														// ��ū �� �ӽ� ���۸� �����Ѵ�.

	if( !file.IsEOF() )
	{
		m_nShow_BtnSndIdx = file.GetInt() ;									// ��ư �⿬ ���� �ε����� �޴´�.
		m_nEndTutorial_SndIdx = file.GetInt() ;								// Ʃ�丮�� �Ϸ� ���� �ε����� �޴´�.
	}

	while( !file.IsEOF() )													// ������ ���� �ƴ� ���� while���� ������.
	{
		memset(Token, 0, sizeof(Token)) ;									// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
		file.GetString( Token );											// ��Ʈ���� �о� �ӽ� ���ۿ� ��´�.

		char commant ;
		memcpy(&commant, Token, sizeof(char)) ;

		if( strcmp(Token,"$TUTORIAL") == 0 )								// �о���� ��Ʈ���� $TUTORIAL�� ���ٸ�,
		{
			cTutorial* pTutorial = NULL ;									// Ʃ�丮�� ������ ���� �����͸� �����ϰ� null ó�� �Ѵ�.
			pTutorial = LoadMainTutorial(&file) ;							// ���� Ʃ�丮�� ������ �ε��Ѵ�.

			if( !pTutorial )	return FALSE ;								// false return ó���� �Ѵ�.

			m_Tutorial_List.AddTail(pTutorial) ;							// Ʃ�丮�� ����Ʈ�� Ʃ�丮���� �߰��Ѵ�.

			++nCount ;														// ī��Ʈ ������ �����Ѵ�.
		}
		else if( strcmp(Token,"") == 0 )									// �о���� ��Ʈ���� ���� �̶��,
		{
			continue ;														// continue ó���� �Ѵ�.
		}
		else if( commant == '@' )
		{
			char commantBuf[128] ;
			file.GetLine(commantBuf, sizeof(commantBuf)) ;
		}
		else																// TUTORIAL�� �ƴϰ�, ������ �ƴϸ�, ��Ÿ��~!!
		{	
			#ifdef _GMTOOL_													// GM�� �����,
			char tempBuf[64] = {0, } ;										// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
			sprintf(tempBuf, TERR_MSG3, nCount) ;							// ���� �޽����� �ϼ��Ѵ�.
			MessageBox(NULL, tempBuf, LT_CATION, MB_OK);					// ���� �޽����� �޽��� �ڽ��� ����.
			#endif

			return FALSE ;													// false return ó���� �Ѵ�.
		}
	}

	return TRUE ;															// true return ó���� �Ѵ�.
}





//=========================================================================
//		���� Ʃ�丮�� ������ �ε��ϴ� �Լ�.
//=========================================================================
cTutorial* cTutorialManager::LoadMainTutorial(CMHFile* pFile) 				// ���� Ʃ�丮�� ������ �ε��ϴ� �Լ�.
{
	if( !pFile )															// ���ڷ� �Ѿ�� ���� ������ ������ ��ȿ���� ������,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		MessageBox(NULL, CERR_MSG1, LMT_CATION, MB_OK);						// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

		return NULL ;														// null return ó���� �Ѵ�.
	}

	int nTutorialCount = 0 ;												// Ʃ�丮���� ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.
	nTutorialCount = m_Tutorial_List.GetCount() ;							// ���� Ʃ�丮�� ����Ʈ�� ī��Ʈ�� �޴´�.

	int nIndex ;															// Ʃ�丮�� �ε����� ���� ������ �����Ѵ�.

	int nStartCondition1 ;													// �� 1 �ߵ� ������ ���� ������ �����Ѵ�.
	int nStartCondition2 ;													// �� 2 �ߵ� ������ ���� ������ �����Ѵ�.

	// Setting index of tutorial.
	nIndex = -1 ;															// Ʃ�丮�� �ε��� ������ -1�� �����Ѵ�.
	nIndex = pFile->GetInt() ;												// ���Ϸ� ���� Ʃ�丮�� �ε����� �о���δ�.

	if( nIndex < 0 || nIndex != nTutorialCount )							// �о���� Ʃ�丮�� �ε����� 0���ϰ�, Ʃ�丮�� ī��Ʈ�� ���� ������,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		char tempBuf[64] = {0, } ;											// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
		sprintf(tempBuf, TERR_MSG4, nTutorialCount) ;						// ���� �޽����� �ϼ��Ѵ�.
		MessageBox(NULL, tempBuf, LMT_CATION, MB_OK);						// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

		return NULL ;														// null return ó���� �Ѵ�.
	}

	// Setting first start condition of tutorial.
	nStartCondition1 = -1 ;													// �� 1 �ߵ� ���� ������ -1�� �����Ѵ�.
	nStartCondition1 = pFile->GetInt() ;									// ���Ϸ� ���� �� 1 �ߵ� ������ �о���δ�.

	if( nStartCondition1 < 0 || nStartCondition1 >= e_MAX_SCONDITION)		// �� 1 �ߵ� ������ 0���ϰų� �ߵ� ���� �ִ���� ������,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		char tempBuf[64] = {0, } ;											// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
		sprintf(tempBuf, TERR_MSG5, nTutorialCount) ;						// ���� �޽����� �ϼ��Ѵ�.
		MessageBox(NULL, tempBuf, LMT_CATION, MB_OK);						// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

		return NULL ;														// null return ó���� �Ѵ�.
	}

	// Setting second start condition of tutorial.
	nStartCondition2 = -1 ;													// �� 2 �ߵ� ���� ������ -1�� �����Ѵ�.
	nStartCondition2 = pFile->GetInt() ;									// ���Ϸ� ���� �� 2 �ߵ� ������ �о���δ�.

	// Create tutorial.
	cTutorial* pTutorial = NULL ;											// Ʃ�丮�� ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pTutorial = new cTutorial ;												// Ʃ�丮���� �����ϰ�, Ʃ�丮�� ������ �����ͷ� �޴´�.

	if( !pTutorial )														// Ʃ�丮�� ������ ��ȿ���� ������,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		char tempBuf[64] = {0, } ;											// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
		sprintf(tempBuf, TERR_MSG6, nTutorialCount) ;						// ���� �޽����� �ϼ��Ѵ�.
		MessageBox(NULL, tempBuf, LMT_CATION, MB_OK);						// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

		return NULL ;														// null return ó���� �Ѵ�.
	}

	// Setting tutorial info.
	pTutorial->Set_Index(nIndex) ;											// Ʃ�丮���� �ε����� �����Ѵ�.
	pTutorial->Set_StartCondition1(nStartCondition1) ;						// �� 1 �ߵ� ������ �����Ѵ�.
	pTutorial->Set_StartCondition2(nStartCondition2) ;						// �� 2 �ߵ� ������ �����Ѵ�.
    
	if( !LoadSubTutorial(pFile, pTutorial) ) return NULL ;					// ���� Ʃ�丮�� �ε��� �����ϸ�, null return ó���� �Ѵ�.

	return pTutorial ;														// Ʃ�丮�� ������ return ó�� �Ѵ�.
}





//=========================================================================
//		���� Ʃ�丮�� ������ �ε��ϴ� �Լ�.
//=========================================================================
BOOL cTutorialManager::LoadSubTutorial(CMHFile* pFile, cTutorial* pTutorial)// ���� Ʃ�丮�� ������ �ε��ϴ� �Լ�.
{
	if( !pFile )															// ���ڷ� �Ѿ�� ���� �����Ͱ� ��ȿ���� ������,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		MessageBox(NULL, CERR_MSG1, LST_CATION, MB_OK);						// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

		return FALSE ;														// false return ó���� �Ѵ�.
	}

	if( !pTutorial )														// ���ڷ� �Ѿ�� Ʃ�丮�� ������ ��ȿ���� ������,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		MessageBox(NULL, TERR_MSG7, LST_CATION, MB_OK);						// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

		return FALSE ;														// false return ó���� �Ѵ�.
	}

	// Loading sub tutorial.
	int nMT_Idx = pTutorial->Get_Index() ;									// ���� Ʃ�丮�� �ε����� �޴´�.

	int nMsg_Count = m_Tutorial_Msg.GetCount() ;							// Ʃ�丮�� �޽ñ� ī��Ʈ�� �޴´�.
	int nSound_Count = AUDIOMGR->Get_SoundCount() ;							// ���� ������ �޴´�.

	int nCount2 = 0 ;														// ���� Ʃ�丮�� ī��Ʈ ������ �����ϰ�, 0���� �ʱ�ȭ �Ѵ�.
	
	int nOpenCount = 0 ;													// ������ ��ȣ ī��Ʈ ���� 0���� �����Ѵ�.
	int nCloseCount= 0 ;													// ������ ��ȣ ī��Ʈ ���� 0���� �����Ѵ�.

	char Token[256];														// ��ū �� �ӽ� ���۸� �����Ѵ�.

	while( !pFile->IsEOF() )												// ���� �����Ͱ� ������ ���� �������� ���� ����	while���� ������.
	{
		memset(Token, 0, sizeof(Token)) ;									// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
		pFile->GetString( Token );											// ��Ʈ���� �о� �ӽ� ���ۿ� ��´�.

		if( Token[0] == '}' )												// }�� ������,
		{
			if( nCloseCount > 0 )											// �ݱ� ��ȣ�� 0���� ũ��,
			{
				#ifdef _GMTOOL_												// GM�� �����,
				char tempBuf[128] = {0, } ;									// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
				sprintf(tempBuf, CERR_MSG2, nMT_Idx, nCount2) ;				// ���� �޽����� �ϼ��Ѵ�.
				MessageBox(NULL, tempBuf, LST_CATION, MB_OK);				// ���� �޽����� �޽��� �ڽ��� ����.
				#endif

				return FALSE ;												// false return ó���� �Ѵ�.
			}

			++nCloseCount ;													// ������ ��ȣ ī��Ʈ ���� �����Ѵ�.
			break;															// while���� Ż���Ѵ�.
		}
		else if( Token[0] == '{' )											// {�� ������,
		{
			if( nOpenCount > 0 )											// ���� ��ȣ�� 0���� ũ��,
			{
				#ifdef _GMTOOL_												// GM�� �����,
				char tempBuf[128] = {0, } ;									// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
				sprintf(tempBuf, CERR_MSG3, nMT_Idx, nCount2) ;				// ���� �޽����� �ϼ��Ѵ�.
				MessageBox(NULL, tempBuf, LST_CATION, MB_OK);				// ���� �޽����� �޽��� �ڽ��� ����.
				#endif

				return FALSE ;												// false return ó���� �Ѵ�.
			}

			++nOpenCount ;													// ������ ��ȣ ī��Ʈ ���� �����Ѵ�.
			continue ;														// continue ó���� �Ѵ�.
		}
		else if( strcmp( Token, "$SUBTUTORIAL" ) == 0 )						// $SUBTUTORIAL�� ������,
		{
			SUB_TUTORIAL stST ;												// ���� Ʃ�丮�� ������ ���� ����ü�� �����Ѵ�.
			memset(&stST, 0, sizeof(SUB_TUTORIAL)) ;						// ���� Ʃ�丮�� ����ü�� �ʱ�ȭ �Ѵ�.

			stST.nIndex = pTutorial->Get_SubTutorial_Count() ;				// ���� Ʃ�丮���� �ε����� �����Ѵ�.

			stST.nSpeech_Idx = pFile->GetInt() ;							// ���Ϸ� ���� Ʃ�丮�� ��� �ε����� �޴´�.
			if(stST.nSpeech_Idx < 0 || stST.nSpeech_Idx >= nMsg_Count)		// ��� �ε����� 0���� �۰ų�, �ִ� �޽��� ī��Ʈ �̻��̸�,
			{
				#ifdef _GMTOOL_												// GM�� �����,
				char tempBuf[128] = {0, } ;									// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
				sprintf(tempBuf, TERR_MSG8, nMT_Idx, nCount2) ;				// ���� �޽����� �ϼ��Ѵ�.
				MessageBox(NULL, tempBuf, LST_CATION, MB_OK);				// ���� �޽����� �޽��� �ڽ��� ����.
				#endif

				return FALSE ;												// false return ó���� �Ѵ�.
			}

			stST.nSound_Idx = pFile->GetInt() ;								// ���Ϸ� ���� ���� �ε����� �޴´�.
			if(stST.nSound_Idx < 0 || stST.nSound_Idx >= nSound_Count)		// ���� �ε����� 0���� �۰ų�, �ִ� �޽��� ī��Ʈ �̻��̸�,
			{
				#ifdef _GMTOOL_												// GM�� �����,
				char tempBuf[128] = {0, } ;									// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
				sprintf(tempBuf, TERR_MSG9, nMT_Idx, nCount2) ;				// ���� �޽����� �ϼ��Ѵ�.
				MessageBox(NULL, tempBuf, LST_CATION, MB_OK);				// ���� �޽����� �޽��� �ڽ��� ����.
				#endif

				return FALSE ;												// false return ó���� �Ѵ�.
			}

			stST.nNpc_Idx = pFile->GetInt() ;								// ���Ϸ� ���� npc �ε����� �޴´�.

			if(stST.nNpc_Idx < 1 )											// npc �ε����� 1���� �۰ų�, �ִ� �޽��� ī��Ʈ �̻��̸�,
			{
				#ifdef _GMTOOL_												// GM�� �����,
				char tempBuf[128] = {0, } ;									// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
				sprintf(tempBuf, TERR_MSG10, nMT_Idx, nCount2) ;			// ���� �޽����� �ϼ��Ѵ�.
				MessageBox(NULL, tempBuf, LST_CATION, MB_OK);				// ���� �޽����� �޽��� �ڽ��� ����.
				#endif

				return FALSE ;												// false return ó���� �Ѵ�.
			}

			stST.nFace_Idx = pFile->GetInt() ;								// ���Ϸ� ���� �� �ε����� �޴´�.
			if(stST.nFace_Idx < 0 || stST.nFace_Idx >= NPC_EMOTION_MAX)		// npc �ε����� 0���� �۰ų�, �ִ� �޽��� ī��Ʈ �̻��̸�,
			{
				#ifdef _GMTOOL_												// GM�� �����,
				char tempBuf[128] = {0, } ;									// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
				sprintf(tempBuf, TERR_MSG11, nMT_Idx, nCount2) ;			// ���� �޽����� �ϼ��Ѵ�.
				MessageBox(NULL, tempBuf, LST_CATION, MB_OK);				// ���� �޽����� �޽��� �ڽ��� ����.
				#endif

				return FALSE ;												// false return ó���� �Ѵ�.
			}

			stST.nComplete_Condition1 = pFile->GetInt() ;					// ���Ϸ� ���� �� 1 �Ϸ� ������ �޴´�.
			if(stST.nComplete_Condition1 < 0 || 
				stST.nComplete_Condition1 >= e_MAX_CCONDITION)				// npc �ε����� 0���� �۰ų�, �ִ� �޽��� ī��Ʈ �̻��̸�,
			{
				#ifdef _GMTOOL_												// GM�� �����,
				char tempBuf[128] = {0, } ;									// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
				sprintf(tempBuf, TERR_MSG12, nMT_Idx, nCount2) ;			// ���� �޽����� �ϼ��Ѵ�.
				MessageBox(NULL, tempBuf, LST_CATION, MB_OK);				// ���� �޽����� �޽��� �ڽ��� ����.
				#endif

				return FALSE ;												// false return ó���� �Ѵ�.
			}

			switch(stST.nComplete_Condition1)								// ù��° �ߵ� ������ Ȯ���Ѵ�.
			{
			case e_CLICK_OKBTN :											// Ȯ�� ��ư�� ������ ���̸�,
				{
					stST.nComplete_Condition2 = pFile->GetInt() ;			// ���Ϸ� ���� �� 2 �Ϸ� ������ �޴´�.
				}
				break ;

			case e_NPC_TALK :												// Ư�� npc�� ��ȭ�ϴ� ���̸�,
				{
					stST.nComplete_Condition2 = pFile->GetInt() ;			// ���Ϸ� ���� �� 2 �Ϸ� ������ �޴´�.
				}
				break ;

			case e_OPEN_DIALOG :											// Ư�� â�� ����� �ϴ� ���̸�,
				{
					strcpy(stST.szCompleteCondition2, pFile->GetString()) ;	// ���Ϸ� ���� �ι�° �Ϸ� ������ �޴´�.		
				}
				break ;
			}

			SUB_TUTORIAL* pSubTutorial = NULL ;								// ���� Ʃ�丮�� ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
			pSubTutorial = new SUB_TUTORIAL ;								// ���� Ʃ�丮���� �����Ѵ�.

			if( !pSubTutorial )												// ���� Ʃ�丮�� ������ ��ȿ���� ������,
			{
				#ifdef _GMTOOL_												// GM�� �����,
				char tempBuf[128] = {0, } ;									// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
				sprintf(tempBuf, TERR_MSG14, nMT_Idx, nCount2) ;			// ���� �޽����� �ϼ��Ѵ�.
				MessageBox(NULL, tempBuf, LST_CATION, MB_OK);				// ���� �޽����� �޽��� �ڽ��� ����.
				#endif

				return FALSE ;												// false return ó���� �Ѵ�.
			}
			
			pSubTutorial->nIndex				= stST.nIndex ;				// ���� Ʃ�丮���� �ε����� �����Ѵ�.
			pSubTutorial->nSpeech_Idx			= stST.nSpeech_Idx ;		// ��ȭ �ε����� �����Ѵ�.
			pSubTutorial->nSound_Idx			= stST.nSound_Idx ;			// ���� �ε����� �����Ѵ�.
			pSubTutorial->nNpc_Idx				= stST.nNpc_Idx ;			// NPC �ε����� �����Ѵ�.
			pSubTutorial->nFace_Idx				= stST.nFace_Idx ;			// �� ǥ�� �ε����� �����Ѵ�.
			pSubTutorial->nComplete_Condition1	= stST.nComplete_Condition1 ;	// ù��° �Ϸ� ������ �����Ѵ�.

			switch(pSubTutorial->nComplete_Condition1)						// ù���� �Ϸ� ������ Ȯ���Ѵ�.
			{
			case e_CLICK_OKBTN :											// Ȯ�� ��ư�� ������ ���̸�,
				{
					pSubTutorial->nComplete_Condition2	= stST.nComplete_Condition2 ;	// �ι�° �Ϸ� ������ �����Ѵ�.
				}
				break ;

			case e_NPC_TALK :												// Ư�� npc�� ��ȭ�ϴ� ���̸�,
				{
					pSubTutorial->nComplete_Condition2	= stST.nComplete_Condition2 ;	// �ι�° �Ϸ� ������ �����Ѵ�.
				}
				break ;

			case e_OPEN_DIALOG :											// Ư�� â�� ����� �ϴ� ���̸�,
				{
					strcpy(pSubTutorial->szCompleteCondition2, stST.szCompleteCondition2) ;	// �ι�° �Ϸ� ������ �����Ѵ�.
				}
				break ;
			}

			++nCount2 ;														// ���� ī��Ʈ ������ �����Ѵ�.

			pTutorial->Add_SubTutorial( pSubTutorial ) ;					// ���� Ʃ�丮���� �߰��Ѵ�.
		}
		else																// SUBTUTORIAL, {, }, �� �ƴϸ�, ��Ÿ��.
		{
			#ifdef _GMTOOL_													// GM�� �����,
			char tempBuf[128] = {0, } ;										// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
			sprintf(tempBuf, TERR_MSG13, nMT_Idx, nCount2) ;				// ���� �޽����� �ϼ��Ѵ�.
			MessageBox(NULL, tempBuf, LST_CATION, MB_OK);					// ���� �޽����� �޽��� �ڽ��� ����.
			#endif

			return FALSE ;													// false return ó���� �Ѵ�.
		}
	}

	return TRUE ;															// true return ó���� �Ѵ�.
}





//=========================================================================
//		Ʃ�丮�� ������ �����ϴ� �Լ�.
//=========================================================================
void cTutorialManager::ReleaseTutorial() 									// Ʃ�丮�� ������ �����ϴ� �Լ�.
{
	cTutorial* pTutorial ;													// Ʃ�丮�� ������ ���� �����͸� �����Ѵ�.

	PTRLISTPOS pDeletePos = NULL ;											// ����Ʈ ���� ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pDeletePos = m_Tutorial_List.GetHeadPosition() ;						// Ʃ�丮�� ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	while(pDeletePos)														// ��ġ ������ ��ȿ�� ���� while���� ������.
	{
		pTutorial = NULL ;													// Ʃ�丮�� ������ ���� �����͸� null ó���� �Ѵ�.
		pTutorial = (cTutorial*)m_Tutorial_List.GetNext(pDeletePos) ;		// ���� ��ġ�� �ش��ϴ� Ʃ�丮�� ������ �ް�, ��ġ�� �������� �����Ѵ�.

		if( !pTutorial ) continue ;											// Ʃ�丮�� ������ ��ȿ���� ������ continue ó���� �Ѵ�.

		delete pTutorial ;													// Ʃ�丮�� ������ �����Ѵ�.
	}

	m_Tutorial_List.RemoveAll() ;											// Ʃ�丮�� ����Ʈ�� ��� ����.
}





//=========================================================================
//		������ �ε����� Ʃ�丮�� ������ ��ȯ�ϴ� �Լ�.
//=========================================================================
cTutorial* cTutorialManager::Get_Tutorial(int nIndex) 						// �ε����� �ش��ϴ� Ʃ�丮�� ������ �����ϴ� �Լ�.
{
	int nTutorial_Count = 0 ;												// Ʃ�丮�� ���� ���� ������ �����ϰ� 0���� �ʱ�ȭ �Ѵ�.
	nTutorial_Count = m_Tutorial_List.GetCount() ;							// Ʃ�丮�� ī��Ʈ�� �޴´�.

	if( nIndex < 0 || nIndex >= nTutorial_Count ) return NULL ;				// ���ڷ� �Ѿ�� �ε�����, 0���� �۰ų�, ���� Ʃ�丮�� ������ ũ��, null return ó���� �Ѵ�.

	PTRLISTPOS returnPos = NULL ;											// �ε����� �ش��ϴ� Ʃ�丮�� ��ġ�� ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	returnPos = m_Tutorial_List.FindIndex(nIndex) ;							// �ε����� �ش��ϴ� Ʃ�丮���� ��ġ������ �޴´�.

	if( !returnPos ) return NULL ;											// ��ġ ������ ��ȿ���� ������, null return ó���� �Ѵ�.

	return (cTutorial*)m_Tutorial_List.GetAt(returnPos) ;					// ��ġ�� �ش��ϴ�, Ʃ�丮�� ������ return ó���� �Ѵ�.
}






//=========================================================================
//		Ʃ�丮�� �޽����� �ε��ϴ� �Լ�.
//=========================================================================
BOOL cTutorialManager::LoadMsg(char* strFileName) 							// Ʃ�丮�� �޽����� �ε��ϴ� �Լ�.
{
	CMHFile file ;															// ���� ������ �����Ѵ�.

	if( !strFileName )														// ���ڷ� �Ѿ�� ���ϸ��� ��ȿ���� ������,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		MessageBox(NULL, TERR_MSG1, LM_CATION, MB_OK);						// �߸� �� ���ϸ��̶�� �޽��� �ڽ��� ����.
		#endif			

		return FALSE ;														// false return ó���� �Ѵ�.
	}

	if( !file.Init( strFileName, "rb") )									// ���� �����̸���, ���� ��忡 ���� ������ ����, ����� �޴´�.
	{																		// �����ߴٸ�,
		#ifdef _GMTOOL_														// GM�� �����,
		char tempBuf[128] = {0, } ;											// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
		sprintf(tempBuf, TERR_MSG2, strFileName) ;							// ���� �޽����� �ϼ��Ѵ�.
		MessageBox(NULL, tempBuf, LM_CATION, MB_OK);						// ���Ͽ��⿡ �����ߴٴ� �޽��� �ڽ��� ����.
		#endif

		return FALSE;														// false return ó���� �Ѵ�.
	}

	int nMsgCount = 0 ;														// �޽��� ī��Ʈ�� ���� ������ �����ϰ� 0���� �����Ѵ�.

	int nMsgIndex = 0 ;														// �޽��� �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.

	char buff[128] ;														// �ӽ� ���۸� �����Ѵ�.

	while(!file.IsEOF())													// while���� ������.
	{
		nMsgCount = m_Tutorial_Msg.GetCount() ;								// �޽��� ī��Ʈ�� �޴´�.

		memset(buff, 0, sizeof(buff)) ;										// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
		file.GetString(buff);												// �ӽù��۷� ��Ʈ���� �о���δ�.

		if( strlen(buff) > 80 )												// ���� ���̰� 80���� ũ��,
		{
			#ifdef _GMTOOL_													// GM�� �����,
			char tempBuf[128] = {0, } ;										// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
			sprintf(tempBuf, MERR_MSG1, nMsgCount) ;						// ���� �޽����� �ϼ��Ѵ�.
			MessageBox(NULL, tempBuf, LM_CATION, MB_OK);					// �޽��� ���� ���� ���� �޽����� ����.
			#endif

			return FALSE ;													// false return ó���� �Ѵ�.
		}

		if( strcmp(buff, "#Msg") == 0 )										// ���۰� #Msg�� ���ٸ�,
		{
			cTutorial_Msg* pMsg = NULL ;									// Ʃ�丮�� �޽��� ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
			pMsg = new cTutorial_Msg ;										// Ʃ�丮�� �޽����� �����Ѵ�.

			if( !pMsg )														// Ʃ�丮�� �޽��� ������ �����ϸ�,
			{
				#ifdef _GMTOOL_												// GM�� �����,
				char tempBuf[128] = {0, } ;									// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
				sprintf(tempBuf, MERR_MSG2, nMsgCount) ;					// ���� �޽����� �ϼ��Ѵ�.
				MessageBox(NULL, tempBuf, LM_CATION, MB_OK);				// �޽��� ���� ���� ���� �޽����� ����.
				#endif

				return FALSE ;												// false return ó���� �Ѵ�.
			}

			nMsgIndex = file.GetInt() ;
			if( nMsgIndex < 0 || nMsgIndex != nMsgCount )					// Ʃ�丮�� �޽��� ������ �����ϸ�,
			{
				#ifdef _GMTOOL_												// GM�� �����,
				char tempBuf[128] = {0, } ;									// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
				sprintf(tempBuf, MERR_MSG4, nMsgCount) ;					// ���� �޽����� �ϼ��Ѵ�.
				MessageBox(NULL, tempBuf, LM_CATION, MB_OK);				// �޽��� ���� ���� ���� �޽����� ����.
				#endif

				return FALSE ;												// false return ó���� �Ѵ�.
			}


			pMsg->Set_MsgIndex(nMsgIndex) ;									// ���� �� �޽����� �ε����� �����Ѵ�.

			if( !ReadMsgLine(&file, pMsg) ) return FALSE ;					// Ʃ�丮�� �޽����� ���� �бⰡ �����ϸ�, false return ó���� �Ѵ�.

			m_Tutorial_Msg.AddTail(pMsg) ;									// Ʃ�丮�� �޽��� ����Ʈ��, �޽����� �߰��Ѵ�.

			continue ;														// continue ó���� �Ѵ�.
		}
		else if( strcmp(buff, "") == 0 )									// ���۰� #Msg�� ������ ���ٸ�,
		{
			continue ;														// continue ó���� �Ѵ�.
		}
		else																// #Msg�� �ƴϰ�, ������ �ƴϸ�, ������~!!
		{
			#ifdef _GMTOOL_													// GM�� �����,
			char tempBuf[128] = {0, } ;										// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
			sprintf(tempBuf, MERR_MSG3, nMsgCount) ;						// ���� �޽����� �ϼ��Ѵ�.
			MessageBox(NULL, tempBuf, LM_CATION, MB_OK);					// ���� �޽����� �޽��� �ڽ��� ����.
			#endif

			return FALSE ;													// false return ó���� �Ѵ�.
		}
	}

	return TRUE ;															// true return ó���� �Ѵ�.
}





//=========================================================================
//		Ʃ�丮�� �޽����� �����ϴ� �Լ�.
//=========================================================================
void cTutorialManager::ReleaseMsg() 										// Ʃ�丮�� �޽����� �����ϴ� �Լ�.
{
	cTutorial_Msg* pMsg ;													// ������ �޽��� ������ ���� �����͸� �����Ѵ�.

	PTRLISTPOS pDeletePos = NULL ;											// ������ �޽��� ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pDeletePos = m_Tutorial_Msg.GetHeadPosition() ;							// Ʃ�丮�� �޽��� ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	while( pDeletePos )														// ���� ��ġ ������ ��ȿ�� ���� while���� ������.
	{
		pMsg = NULL ;														// �޽��� �����͸� null ó���� �Ѵ�.
		pMsg = (cTutorial_Msg*)m_Tutorial_Msg.GetNext(pDeletePos) ;			// ��ġ�� �ش��ϴ� �޽��� ������ �ް�, ��ġ������ �������� �����Ѵ�.

		if( !pMsg ) continue ;												// �޽��� ������ ��ȿ���� ������, continue ó���� �Ѵ�.

		//---KES �޸� �� ����
		pMsg->Release();
		//--------------------
		delete pMsg ;														// �޽��� ������ �����Ѵ�.
	}

	m_Tutorial_Msg.RemoveAll() ;											// Ʃ�丮�� �޽��� ����Ʈ�� ��� ����.
}





//=========================================================================
//		Ʃ�丮�� �޽��� ������ �д� �Լ�.
//=========================================================================
BOOL cTutorialManager::ReadMsgLine(CMHFile* pFile, cTutorial_Msg* pMsg) 	// Ʃ�丮�� �޽����� ������ �д� �Լ�.
{
	if( !pFile )															// ���ڷ� �Ѿ�� ���� �����Ͱ� ��ȿ���� ������,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		MessageBox(NULL, CERR_MSG1, RML_CATION, MB_OK);						// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

		return FALSE ;														// false return ó���� �Ѵ�.
	}

	if( !pMsg )																// ���ڷ� �Ѿ�� Ʃ�丮�� ������ ��ȿ���� ������,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		MessageBox(NULL, MERR_MSG3, RML_CATION, MB_OK);						// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

		return FALSE ;														// false return ó���� �Ѵ�.
	}

	int nMsgIdx = pMsg->Get_MsgIndex() ;									// ���� �޽����� �ε����� �޴´�.

	int nLineCount = 0 ;													// �޽����� ���� ���� ���� ������ �����ϰ� 0���� �����Ѵ�.

	int nOpenCount = 0 ;													// ������ ��ȣ ī��Ʈ ���� 0���� �����Ѵ�.
	int nCloseCount= 0 ;													// ������ ��ȣ ī��Ʈ ���� 0���� �����Ѵ�.

	char buff[128] ;														// �ӽ� ���۸� �����Ѵ�.

	memset(buff, 0, sizeof(buff)) ;											// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
	pFile->GetLine(buff, 128) ;												// �ӽ� ���ۿ� ������ �о���δ�.

	while( !pFile->IsEOF() )												// ������ ���� �ƴ϶�� ��� while���� ������.
	{
		nLineCount = pMsg->Get_LineCount() ;								// �޽����� ���� ī��Ʈ�� �޴´�.

		memset(buff, 0, sizeof(buff)) ;										// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
		pFile->GetLine(buff, 128) ;											// �ӽ� ���ۿ� ������ �о���δ�.

		if( strcmp( buff, "{") == 0 )
		{
			if( nOpenCount > 0 )											// ���� ��ȣ�� 0���� ũ��,
			{
				#ifdef _GMTOOL_												// GM�� �����,
				char tempBuf[128] = {0, } ;									// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
				sprintf(tempBuf, CERR_MSG3, nMsgIdx, nLineCount) ;			// ���� �޽����� �ϼ��Ѵ�.
				MessageBox(NULL, tempBuf, RML_CATION, MB_OK);				// ���� �޽����� �޽��� �ڽ��� ����.
				#endif

				return FALSE ;												// false return ó���� �Ѵ�.
			}

			++nOpenCount ;													// ������ ��ȣ ī��Ʈ ���� �����Ѵ�.
			continue ;
		}
		else if( strcmp( buff, "}") == 0 )
		{
			if( nCloseCount > 0 )											// �ݱ� ��ȣ�� 0���� ũ��,
			{
				#ifdef _GMTOOL_												// GM�� �����,
				char tempBuf[128] = {0, } ;									// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
				sprintf(tempBuf, CERR_MSG2, nMsgIdx, nLineCount) ;			// ���� �޽����� �ϼ��Ѵ�.
				MessageBox(NULL, tempBuf, RML_CATION, MB_OK);				// ���� �޽����� �޽��� �ڽ��� ����.
				#endif

				return FALSE ;												// false return ó���� �Ѵ�.
			}

			++nCloseCount ;													// ������ ��ȣ ī��Ʈ ���� �����Ѵ�.
			break ;
		}
		else
		{
			if( strlen(buff) > 80 )											// �޽��� ���̰� 80�� ������,
			{
				#ifdef _GMTOOL_												// GM�� �����,
				char tempBuf[128] = {0, } ;									// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
				sprintf(tempBuf, MERR_MSG1, nMsgIdx, nLineCount) ;			// ���� �޽����� �ϼ��Ѵ�.
				MessageBox(NULL, tempBuf, RML_CATION, MB_OK);				// ���� �޽����� �޽��� �ڽ��� ����.
				#endif

				return FALSE ;												// false return ó���� �Ѵ�.
			}
			else															// �޽��� ���̰� 64 ���ϸ�,
			{
				pMsg->Add_MsgLine(buff) ;									// �޽����� ������ �߰��Ѵ�.
			}
		}
	}

	return TRUE ;															// true return ó���� �Ѵ�.
}





//=========================================================================
//		������ �ε����� Ʃ�丮�� �޽����� ��ȯ�ϴ� �Լ�.
//=========================================================================
cTutorial_Msg* cTutorialManager::Get_TutorialMsg(int nIndex) 				// �ε����� �ش��ϴ� Ʃ�丮�� �޽��� ������ �����ϴ� �Լ�.
{
	int nMst_Count = 0 ;													// Ʃ�丮�� �޽��� ���� ���� ������ �����ϰ� 0���� �ʱ�ȭ �Ѵ�.
	nMst_Count = m_Tutorial_Msg.GetCount() ;								// Ʃ�丮�� �޽��� ī��Ʈ�� �޴´�.

	if( nIndex < 0 || nIndex >= nMst_Count ) return NULL ;					// ���ڷ� �Ѿ�� �ε�����, 0���� �۰ų�, ���� �޽��� ������ ũ��, null return ó���� �Ѵ�.

	PTRLISTPOS returnPos = NULL ;											// �ε����� �ش��ϴ� �޽��� ��ġ�� ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	returnPos = m_Tutorial_Msg.FindIndex(nIndex) ;							// �ε����� �ش��ϴ� �޽����� ��ġ������ �޴´�.

	if( !returnPos ) return NULL ;											// ��ġ ������ ��ȿ���� ������, null return ó���� �Ѵ�.

	return (cTutorial_Msg*)m_Tutorial_Msg.GetAt(returnPos) ;				// ��ġ�� �ش��ϴ�, �޽��� ������ return ó���� �Ѵ�.
}





//=========================================================================
//		���� ������ �ε��ϴ� �Լ�.
//=========================================================================
BOOL cTutorialManager::LoadHelperInfo(char* strFileName) 					// ���� ������ �ε��ϴ� �Լ�.
{
	CMHFile file ;															// ���� ������ �����Ѵ�.

	if( !strFileName )														// ���ڷ� �Ѿ�� ���ϸ��� ��ȿ���� ������,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		MessageBox(NULL, TERR_MSG1, LHI_CATION, MB_OK);						// �߸� �� ���ϸ��̶�� �޽��� �ڽ��� ����.
		#endif			

		return FALSE ;														// false return ó���� �Ѵ�.
	}

	if( !file.Init( strFileName, "rb") )									// ���� �����̸���, ���� ��忡 ���� ������ ����, ����� �޴´�.
	{																		// �����ߴٸ�,
		#ifdef _GMTOOL_														// GM�� �����,
		char tempBuf[128] = {0, } ;											// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
		sprintf(tempBuf, TERR_MSG2, strFileName) ;							// ���� �޽����� �ϼ��Ѵ�.
		MessageBox(NULL, tempBuf, LHI_CATION, MB_OK);						// ���Ͽ��⿡ �����ߴٴ� �޽��� �ڽ��� ����.
		#endif

		return FALSE;														// false return ó���� �Ѵ�.
	}

	int nLinkCount = 0 ;													// ���� ��ũ ī��Ʈ�� ���� ������ �����ϰ� 0���� �ʱ�ȭ �Ѵ�.

	int nIndex ;															// ��ũ �ε����� ���� ������ �����Ѵ�.

	char LinkTitle[128] ;													// ��ũ ������ ���� �ӽ� ���۸� �����Ѵ�.

	int nTutorialIndex ;													// Ʃ�丮�� �ε����� ���� ������ �����Ѵ�.

	while(!file.IsEOF())													// while���� ������.
	{
		nLinkCount = m_Helper_List.GetCount() ;								// ���� ��ũ ����Ʈ�� ī��Ʈ�� �޴´�.

		nIndex = 0 ;														// ��ũ �ε����� ���� ������ 0���� �ʱ�ȭ �Ѵ�.
		nIndex = file.GetInt() ;											// ���Ϸ� ���� ��ũ �ε����� �޴´�.

		//if( nIndex < 0 || nIndex >= nLinkCount )							// �ε����� 0���� �۰ų�, ���� ���� ����Ʈ ī��Ʈ �̻��̸�,
		//{
		//	#ifdef _GMTOOL_													// GM�� �����,
		//	char tempBuf[64] = {0, } ;										// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
		//	sprintf(tempBuf, HERR_MSG1, nLinkCount) ;						// ���� �޽����� �ϼ��Ѵ�.
		//	MessageBox(NULL, tempBuf, LHI_CATION, MB_OK);					// ���� �޽����� �޽��� �ڽ��� ����.
		//	#endif

  //          return FALSE ;													// false return ó���� �Ѵ�.
		//}

		nTutorialIndex = 0 ;												// Ʃ�丮�� �ε����� ���� ������ 0���� �����Ѵ�.
		nTutorialIndex = file.GetInt() ;									// ���Ϸ� ���� Ʃ�丮�� ��ȣ�� �о���δ�.

		if( nTutorialIndex < 0 || nTutorialIndex > m_Tutorial_List.GetCount() ) // Ʃ�丮�� �ε����� 0���� �۰ų�, Ʃ�丮�� �� ���� ũ��,
		{
			#ifdef _GMTOOL_													// GM�� �����,
			char tempBuf[128] = {0, } ;										// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
			sprintf(tempBuf, HERR_MSG3, nLinkCount) ;						// ���� �޽����� �ϼ��Ѵ�.
			MessageBox(NULL, tempBuf, LHI_CATION, MB_OK);					// ���� �޽����� �޽��� �ڽ��� ����.
			#endif

            return FALSE ;													// false return ó���� �Ѵ�.
		}

		memset(LinkTitle, 0, sizeof(LinkTitle)) ;							// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.
		file.GetLine(LinkTitle, 128);										// ���Ϸ� ���� ��ũ ������ �о���δ�.

		if( strlen(LinkTitle) < 2 )											// ��ũ ������ �ѱ���
		{
			#ifdef _GMTOOL_													// GM�� �����,
			char tempBuf[128] = {0, } ;										// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
			sprintf(tempBuf, HERR_MSG2, nLinkCount) ;						// ���� �޽����� �ϼ��Ѵ�.
			MessageBox(NULL, tempBuf, LHI_CATION, MB_OK);					// ���� �޽����� �޽��� �ڽ��� ����.
			#endif

            return FALSE ;													// false return ó���� �Ѵ�.
		}

		HELPLINK* pHelpLink = NULL ;
		pHelpLink = new HELPLINK ;

		if( !pHelpLink )
		{
			#ifdef _GMTOOL_													// GM�� �����,
			char tempBuf[128] = {0, } ;										// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
			sprintf(tempBuf, HERR_MSG4, nLinkCount) ;						// ���� �޽����� �ϼ��Ѵ�.
			MessageBox(NULL, tempBuf, LHI_CATION, MB_OK);					// ���� �޽����� �޽��� �ڽ��� ����.
			#endif

            return FALSE ;													// false return ó���� �Ѵ�.
		}

		pHelpLink->nIndex = nIndex ;										// ���� �ε����� �����Ѵ�.
		strcpy(pHelpLink->Title, LinkTitle) ;								// ���� ��ũ ������ �����Ѵ�.
		pHelpLink->nTutorial_Idx = nTutorialIndex ;							// ���� ��ũ Ʃ�丮�� �ε����� �����Ѵ�.

		m_Helper_List.AddTail( pHelpLink ) ;								// ���� ����Ʈ�� ��ũ�� �߰��Ѵ�.
	}

	return TRUE ;															// true return ó���� �Ѵ�.
}





//=========================================================================
//		���� ������ �����ϴ� �Լ�.
//=========================================================================
void cTutorialManager::ReleaseHelperInfo() 									// ���� ������ �����ϴ� �Լ�.
{
	HELPLINK* pHelpLink ;													// ���� ��ũ ������ ���� �����͸� �����Ѵ�.

	PTRLISTPOS pDeletePos = NULL ;											// ���� ��ġ ������ ���� ��ġ �����͸� �����ϰ� null ó���� �Ѵ�.
	pDeletePos = m_Helper_List.GetHeadPosition() ;							// ���� ��ũ�� ��� �ִ� ����Ʈ�� ���� �����ϰ� ��ġ������ �޴´�.

	while( pDeletePos )														// ���� ��ġ ������ ��ȿ�� ���� while���� ������.
	{
		pHelpLink = NULL ;													// ���� ��ũ ���� �����͸� null ó���� �Ѵ�.
		pHelpLink = (HELPLINK*)m_Helper_List.GetNext(pDeletePos) ;			// ��ġ�� �ش��ϴ� ���� ��ũ ������ �ް�, ���� ��ġ�� �����Ѵ�.

		if( !pHelpLink ) continue ;											// ���� ��ũ ������ ��ȿ���� ������, continue ó���� �Ѵ�.

		delete pHelpLink ;													// ���� ��ũ ������ �����Ѵ�.
	}

	m_Helper_List.RemoveAll() ;												// ���� ��ũ ����Ʈ�� ��� ����.
}





//=========================================================================
//		������ �ε����� ���� ��ũ�� ��ȯ�ϴ� �Լ�.
//=========================================================================
HELPLINK* cTutorialManager::Get_HelpLink(int nIndex) 						// �ε����� �ش��ϴ� ���� ��ũ�� �����ϴ� �Լ�.
{
	int nLink_Count = 0 ;													// ���� ��ũ ���� ���� ������ �����ϰ� 0���� �ʱ�ȭ �Ѵ�.
	nLink_Count = m_Helper_List.GetCount() ;								// ���� ��ũ ���� �޴´�.

	if( nIndex < 0 || nIndex >= nLink_Count ) return NULL ;					// ���ڷ� �Ѿ�� �ε�����, 0���� �۰ų�, ���� ��ũ ������ ũ��, null return ó���� �Ѵ�.

	PTRLISTPOS returnPos = NULL ;											// �ε����� �ش��ϴ� ��ũ ��ġ�� ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	returnPos = m_Helper_List.FindIndex(nIndex) ;							// �ε����� �ش��ϴ� ��ũ�� ��ġ������ �޴´�.

	if( !returnPos ) return NULL ;											// ��ġ ������ ��ȿ���� ������, null return ó���� �Ѵ�.

	return (HELPLINK*)m_Helper_List.GetAt(returnPos) ;						// ��ġ�� �ش��ϴ�, ��ũ ������ return ó���� �Ѵ�.
}





//=========================================================================
//		���� ��ũ���� ���� �� Ʃ�丮���� �����ϴ� �Լ�.
//=========================================================================
void cTutorialManager::Start_HelpTutorial(cTutorial* pTutorial) 			// ���� ��ũ���� ���� �� Ʃ�丮���� �����ϴ� �Լ�.
{
	return;

	if( !pTutorial )														// ���ڷ� �Ѿ�� Ʃ�丮�� ������ ��ȿ���� ������,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		MessageBox(NULL, TERR_MSG7, SHT_CATION, MB_OK);						// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

        return ;															// return ó���� �Ѵ�.		
	}

	m_nCurTutorialIdx = pTutorial->Get_Index() ;							// ���� Ʃ�丮�� �ε����� �����Ѵ�.

	m_pTutorialDlg->Set_TutorialInfo(pTutorial) ;							// Ʃ�丮�� ���̾�α׿��� Ʃ�丮���� �����Ѵ�.
	m_pTutorialDlg->Start_Tutorial() ;										// Ʃ�丮���� �����Ѵ�.
}





//=========================================================================
//		��Ʈ��ũ �޽��� �Ľ� ó���� �ϴ� �Լ�.
//=========================================================================
void cTutorialManager::NetworkMsgParse(BYTE Protocol,void* pMsg) 			// ��Ʈ��ũ �޽��� �Ľ� �Լ�.
{
	if(!pMsg) return ;														// ���ڷ� �Ѿ�� �޽��� ������ ��ȿ���� ������, return ó���� �Ѵ�.

	switch(Protocol)														// �������� ������ Ȯ���Ѵ�.
	{
	case MP_TUTORIAL_MAINDATALOAD :											// ���������� Ʃ�丮�� �����͸� �ε��ϴ� ���̸�,
		{
			SEND_TUTORIAL_DATA* msg = (SEND_TUTORIAL_DATA*)pMsg ;			// Ʃ�丮�� ������ ������ �޽����� �޴´�.

			if(!msg)														// �޽��� ������ ��ȿ���� ������,
			{
				#ifdef _GMTOOL_												// GM�� �����,
				MessageBox(NULL, NERR_MSG1, NMP_CATION, MB_OK);				// ���� �޽����� �޽��� �ڽ��� ����.
				#endif

				return ;													// return ó���� �Ѵ�.
			}

			int		nIndex ;												// �ε����� ���� ������ �����Ѵ�.
			BOOL	isComplete ;											// �Ϸ� ���θ� ���� ������ �����Ѵ�.

			cTutorial* pTutorial ;											// Ʃ�丮�� ������ ���� �����͸� �����Ѵ�.

			PTRLISTPOS pos ;												// ��ġ ������ ���� �����͸� �����Ѵ�.

			pos = NULL ;													// pos�� null�� �����Ѵ�.
			pos = m_Tutorial_List.GetHeadPosition() ;						// Ʃ�丮�� ����Ʈ�� ���� �����Ѵ�.

			while( pos )													// ��ġ ������ ��ȿ�ҵ��� while���� ������.
			{
				pTutorial = NULL ;											// Ʃ�丮�� �����͸� null�� �����Ѵ�.
				pTutorial = (cTutorial*)m_Tutorial_List.GetNext(pos) ;		// ��ġ�� �ش��ϴ� Ʃ�丮�� ������ �ް�, ��ġ�� �������� �����Ѵ�.

				if( !pTutorial ) continue ;									// Ʃ�丮�� ������ ������ continue ó���� �Ѵ�.

				pTutorial->Set_Complete(FALSE) ;							// Ʃ�丮�� �Ϸ� ���θ� false�� �����Ѵ�.
			}

			for( WORD count = 0 ; count < msg->wCount ; ++count )			// �޽��� ī��Ʈ ��ŭ for���� ������.
			{
				nIndex = -1 ;												// �޽��� Ʃ�丮���� �ε����� ���� ������ -1�� �����Ѵ�.
				nIndex = msg->TutorialList[count].nTutorialIdx ;			// �޽��� Ʃ�丮���� �ε����� �޴´�.

				if( nIndex < 0 )											// �ε����� 0���� ������,
				{
					#ifdef _GMTOOL_											// GM�� �����,
					MessageBox(NULL, NERR_MSG2, NMP_CATION, MB_OK);			// ���� �޽����� �޽��� �ڽ��� ����.
					#endif

					continue ;												// continue ó���� �Ѵ�.
				}

				pos = NULL ;												// ��ġ ������ ���� �����͸� null ó���� �Ѵ�.
				pos = m_Tutorial_List.FindIndex( nIndex ) ;					// �޽��� Ʃ�丮�� �ε����� ���� Ʃ�丮�� ��ġ�� �޴´�.

				if( !pos )													// ��ġ ������ ��ȿ���� ������,
				{
					#ifdef _GMTOOL_											// GM�� �����,
					char tempBuf[128] = {0, } ;								// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
					sprintf(tempBuf, NERR_MSG3, nIndex) ;					// ���� �޽����� �ϼ��Ѵ�.
					MessageBox(NULL, tempBuf, NMP_CATION, MB_OK);			// ���� �޽����� �޽��� �ڽ��� ����.
					#endif

					continue ;												// continue ó���� �Ѵ�.
				}

				pTutorial = NULL ;											// Ʃ�丮�� ������ ���� �����͸� null ó���� �Ѵ�.
				pTutorial = (cTutorial*)m_Tutorial_List.GetAt(pos) ;		// ��ġ�� �ش��ϴ� Ʃ�丮�� ������ �޴´�.

				if( !pTutorial ) 
				{
					#ifdef _GMTOOL_											// GM�� �����,
					char tempBuf[128] = {0, } ;								// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
					sprintf(tempBuf, NERR_MSG4, nIndex) ;					// ���� �޽����� �ϼ��Ѵ�.
					MessageBox(NULL, tempBuf, NMP_CATION, MB_OK);			// ���� �޽����� �޽��� �ڽ��� ����.
					#endif

					continue ;												// continue ó���� �Ѵ�.
				}

				isComplete = FALSE ;										// �Ϸ� ���θ� false�� �����Ѵ�.
				isComplete = msg->TutorialList[count].IsComplete ;			// �޽��� Ʃ�丮���� �Ϸ� ���θ� �޴´�.

				pTutorial->Set_Complete(isComplete) ;						// ���� Ʃ�丮�� �Ϸ� ���θ� �����Ѵ�.
			}
		}
		break ;
	}
}





// 080502 LYW --- TutorialManager : Ʃ�丮�� Process �Լ� ȣ�� ����.
// Ʃ�丮���� ����/�ϷḦ �̺�Ʈ �߻� �� üũ�ϴ� ������� ���� ��.
////=========================================================================
////		Ʃ�丮�� �Ŵ����� ���μ��� �Լ�.
////=========================================================================
//void cTutorialManager::Process() 											// ���μ��� �Լ�.
//{
//	if( !m_bActiveTutorial ) return ;										// Ʃ�丮���� ��Ȱ��ȭ ���¸�, return ó���� �Ѵ�.
//
//	if(m_bUpdated) return ;													// Ʃ�丮�� �̺�Ʈ�� ������Ʈ �Ǿ�����, return ó���� �Ѵ�.
//	
//	cTutorial* pCurTutorial ;												// ���� Ʃ�丮�� ������ ���� �����͸� �����Ѵ�.
//
//	PTRLISTPOS pos = NULL ;													// Ʃ�丮�� ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
//	pos = m_Tutorial_List.GetHeadPosition() ;								// Ʃ�丮�� ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.
//
//	while(pos)																// ��ġ ������ ��ȿ�� ���� whlie���� ������.
//	{
//		pCurTutorial = NULL ;												// Ʃ�丮�� ������ ���� �����͸� null ó���� �Ѵ�.
//		pCurTutorial = (cTutorial*)m_Tutorial_List.GetNext(pos) ;			// ��ġ�� �ش��ϴ� Ʃ�丮�� ������ �ް�, ��ġ�� �������� �����Ѵ�.
//
//		if( !pCurTutorial ) continue ;										// Ʃ�丮�� ������ ��ȿ���� ������ continue ó���� �Ѵ�.
//
//		if( pCurTutorial->Is_Completed() ) continue ;						// �Ϸ��� Ʃ�丮���̸�, �ǳʶڴ�.
//
//		if(Check_StartCondition(pCurTutorial)) return ;						// ���� Ʃ�丮���� ���� ������ üũ�ϰ� return ó���� �Ѵ�.
//	}
//}





//=========================================================================
//		������ Ʃ�丮���� ���� ������ üũ�ϴ� �Լ�.
//=========================================================================
BOOL cTutorialManager::Check_StartCondition(cTutorial* pTutorial) 			// Ʃ�丮�� �ߵ� ������ üũ�ϴ� �Լ�.
{
	if( !pTutorial )														// ���ڷ� �Ѿ�� Ʃ�丮�� ������ ��ȿ���� ������,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		MessageBox(NULL, TERR_MSG7, CSC_CATION, MB_OK);						// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

        return FALSE ;														// return ó���� �Ѵ�.		
	}

	int nCondition = 0 ;													// ���� �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.
	nCondition = pTutorial->Get_StartCondition1() ;							// ���� Ʃ�丮���� ���� ������ �޴´�.

	if( nCondition < 0 || nCondition >= e_MAX_SCONDITION )					// ������ 0���� �۰ų�, �ִ� ������ �Ѿ��,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		char tempBuf[128] = {0, } ;											// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
		sprintf(tempBuf, TERR_MSG5, pTutorial->Get_Index()) ;				// ���� �޽����� �ϼ��Ѵ�.
		MessageBox(NULL, tempBuf, CSC_CATION, MB_OK);						// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

        return FALSE ;														// return ó���� �Ѵ�.
	}

	switch(nCondition)														// ���� ������ Ȯ���Ѵ�.
	{
	case e_FIRST_GAMEIN :													// ó�� ���ӿ� �α��� ���� �� �����ϴ� �����̸�,
		{
			if( pTutorial->Is_Completed() ) return FALSE ;					// false return ó���� �Ѵ�.

			m_bUpdated = TRUE ;												// ������Ʈ ���θ� true�� �����Ѵ�.

			Start_CurTutorial(pTutorial) ;									// ���� Ʃ�丮���� �����Ѵ�.
		}
		break ;

	case e_COMPLETE_TUTORIAL :												// ���� ������ �ٸ� Ʃ�丮�� �Ϸ���,
		{
			int nCompleteCondition = pTutorial->Get_StartCondition2() ;		// ���� Ʃ�丮���� ���� ������ �޴´�.

			cTutorial* pCheckTutorial = NULL ;								// üũ �� Ʃ�丮�� �����͸� �����ϰ� null ó���� �Ѵ�.
			pCheckTutorial = Get_Tutorial(nCompleteCondition) ;				// üũ �� Ʃ�丮�� ������ �޴´�.

			if( !pCheckTutorial )											// üũ �� Ʃ�丮�� ������ ���� ���ߴٸ�,
			{
				#ifdef _GMTOOL_												// GM�� �����,
				char tempBuf[128] = {0, } ;									// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
				sprintf(tempBuf, TERR_MSG15, nCompleteCondition) ;			// ���� �޽����� �ϼ��Ѵ�.
				MessageBox(NULL, tempBuf, CSC_CATION, MB_OK);				// ���� �޽����� �޽��� �ڽ��� ����.
				#endif

				return FALSE ;												// false return ó���� �Ѵ�.
			}

			if( !pCheckTutorial->Is_Completed() ) return FALSE ;			// false return ó���� �Ѵ�.

			m_bUpdated = TRUE ;												// ������Ʈ ���θ� true�� �����Ѵ�.
				
			Start_CurTutorial(pTutorial) ;									// ���� Ʃ�丮���� �����Ѵ�.

			return TRUE ;													// true return ó���� �Ѵ�.
		}
		break ;

	case e_LEVEL :															// ���� ������ ���� ���� �����̶��,
		{
			int nLevelCondition = pTutorial->Get_StartCondition2() ;		// ���� Ʃ�丮���� ���� ������ �޴´�.

			if( nLevelCondition <= HERO->GetLevel() )						// HERO�� ������ ���� ���� �̻��̸�,
			{
				m_bUpdated = TRUE ;											// ������Ʈ ���θ� true�� �����Ѵ�.
				
				Start_CurTutorial(pTutorial) ;								// ���� Ʃ�丮���� �����Ѵ�.

				return TRUE ;												// true return ó���� �Ѵ�.
			}
		}
		break ;

	default : break ;														// ���ݱ��� ���� ����...
	}

	return FALSE ;															// false return ó���� �Ѵ�.
}





//=========================================================================
//		Ư�� npc���� ��ȭ ���θ� üũ�ϴ� �Լ�.
//=========================================================================
BOOL cTutorialManager::Check_NpcTalk(void* pMsg) 							// npc�� ��ȭ�� �ϴ� ������ üũ�ϴ� �Լ�.
{
	if( !pMsg )
	{
		#ifdef _GMTOOL_														// GM�� �����,
		MessageBox(NULL, CERR_MSG4, CNT_CATION, MB_OK);						// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

		return FALSE ;														// return ó���� �Ѵ�.
	}

	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;									// ���ڷ� �Ѿ�� �����͸� MSG_DWORD2* ������ �޴´�.

	CNpc* pNpc = NULL ;														// npc ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pNpc = (CNpc*)OBJECTMGR->GetObject(pmsg->dwData1);						// npc ������ �޴´�.

	if( !pNpc )																// npc ������ ��ȿ���� �ʴٸ�,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		char tempBuf[128] = {0, } ;											// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
		sprintf(tempBuf, PERR_MSG2, pmsg->dwData1) ;						// ���� �޽����� �ϼ��Ѵ�.
		MessageBox(NULL, tempBuf, CNT_CATION, MB_OK);						// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

		return FALSE ;														// return ó���� �Ѵ�.
	}

	if( Check_NpcTalk_EndCondition(pmsg->dwData2) )		return FALSE ;		// ���� npc �ε����� �Ϸ� ������ üũ�Ѵ�.

	if( Check_NpcTalk_StartCondition(pmsg->dwData2) )						// ���� ������ �����ϸ�, 
	{
		return TRUE ;														// true return ó���� �Ѵ�.
	}
	else																	// ���� ������ �����ϸ�,
	{
		return FALSE ;														// false return ó���� �Ѵ�.
	}
}





//=========================================================================
//		npc�� ��ȭ ���� ��, ���� ������ üũ�ϴ� �Լ�.
//=========================================================================
BOOL cTutorialManager::Check_NpcTalk_StartCondition(DWORD dwNpcIdx) 		// npc�� ��ȭ ���� ��, ���� ������ üũ�ϴ� �Լ�.
{
	if( !m_bActiveTutorial ) return FALSE ;									// Ʃ�丮���� ��Ȱ��ȭ ���¸�, return ó���� �Ѵ�.

	if(m_bUpdated) return FALSE ;											// Ʃ�丮�� �̺�Ʈ�� ������Ʈ �Ǿ�����, return ó���� �Ѵ�.
	
	cTutorial* pCurTutorial ;												// ���� Ʃ�丮�� ������ ���� �����͸� �����Ѵ�.

	PTRLISTPOS pos = NULL ;													// Ʃ�丮�� ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pos = m_Tutorial_List.GetHeadPosition() ;								// Ʃ�丮�� ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	while(pos)																// ��ġ ������ ��ȿ�� ���� whlie���� ������.
	{
		pCurTutorial = NULL ;												// Ʃ�丮�� ������ ���� �����͸� null ó���� �Ѵ�.
		pCurTutorial = (cTutorial*)m_Tutorial_List.GetNext(pos) ;			// ��ġ�� �ش��ϴ� Ʃ�丮�� ������ �ް�, ��ġ�� �������� �����Ѵ�.

		if( !pCurTutorial ) continue ;										// Ʃ�丮�� ������ ��ȿ���� ������ continue ó���� �Ѵ�.

		if( pCurTutorial->Is_Completed() ) continue ;						// �Ϸ��� Ʃ�丮���̸�, �ǳʶڴ�.

		if( pCurTutorial->Get_StartCondition1() != e_TALK_NPC ) continue ;	// npc�� ��ȭ�ϴ� ���� ������ �ƴϸ� �ǳʶڴ�.

		if( pCurTutorial->Get_StartCondition2() != dwNpcIdx ) continue ;	// ���� Ʃ�丮���� �ι�° ���ǰ� npc �ε����� �ٸ���, continue ó���� �Ѵ�.

		m_bUpdated = TRUE ;													// ������Ʈ ���θ� true�� �����Ѵ�.
		
		Start_CurTutorial(pCurTutorial) ;									// ���� Ʃ�丮���� �����Ѵ�.

		return TRUE ;														// ���� ó���� �Ѵ�.
	}

	return FALSE ;															// false return ó���� �Ѵ�.
}





//=========================================================================
//		npc�� ��ȭ ���� ��, �Ϸ� ������ üũ�ϴ� �Լ�.
//=========================================================================
BOOL cTutorialManager::Check_NpcTalk_EndCondition(DWORD dwNpcIdx) 			// npc�� ��ȭ ���� ��, �Ϸ� ������ üũ�ϴ� �Լ�.
{
	if( m_pTutorialDlg->IsSameEndNpc(dwNpcIdx) )							// ���� ���ǰ� ���� npc ���,
	{
		if(m_pTutorialDlg->IsHaveNextSubTutorial())							// ���� ���� Ʃ�丮�� ������ �ִٸ�,
		{
			m_pTutorialDlg->StartNextSubTutorial() ;						// ���� ���� Ʃ�丮���� �����Ѵ�.
		}
		else																// ���� ���� ���丮�� ������ ������,
		{
			m_pTutorialDlg->UpdateTutorial() ;								// Ʃ�丮���� ������Ʈ �Ѵ�.
		}
	}

	return FALSE ;															// false return ó���� �Ѵ�.
}





//=========================================================================
//		������ ��, ���� ������ tutorial�� �ִ��� Ȯ�� ��, ������ ����.
//=========================================================================
void cTutorialManager::Check_LevelUp()
{
	if( !m_bActiveTutorial ) return ;										// Ʃ�丮���� ��Ȱ��ȭ ���¸�, return ó���� �Ѵ�.

	if(m_bUpdated) return ;													// Ʃ�丮�� �̺�Ʈ�� ������Ʈ �Ǿ�����, return ó���� �Ѵ�.
	
	cTutorial* pCurTutorial ;												// ���� Ʃ�丮�� ������ ���� �����͸� �����Ѵ�.

	int nCondition1 ;														// Ʃ�丮���� �ߵ� ������ ���� ������ �����Ѵ�.
	int nCondition2 ;														// �ι�° �ߵ� ������ ���� ������ �����Ѵ�.

	PTRLISTPOS pos = NULL ;													// Ʃ�丮�� ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pos = m_Tutorial_List.GetHeadPosition() ;								// Ʃ�丮�� ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	while(pos)																// ��ġ ������ ��ȿ�� ���� whlie���� ������.
	{
		pCurTutorial = NULL ;												// Ʃ�丮�� ������ ���� �����͸� null ó���� �Ѵ�.
		pCurTutorial = (cTutorial*)m_Tutorial_List.GetNext(pos) ;			// ��ġ�� �ش��ϴ� Ʃ�丮�� ������ �ް�, ��ġ�� �������� �����Ѵ�.

		if( !pCurTutorial ) continue ;										// Ʃ�丮�� ������ ��ȿ���� ������ continue ó���� �Ѵ�.

		if( pCurTutorial->Is_Completed() ) continue ;						// �Ϸ��� Ʃ�丮���̸�, �ǳʶڴ�.

		nCondition1 = -1 ;													// �ߵ� ���� ������ -1�� �����Ѵ�.
		nCondition1 = pCurTutorial->Get_StartCondition1() ;					// ���� Ʃ�丮���� �ߵ� ������ �޴´�.

		if( nCondition1 != e_LEVEL ) continue ;								// ���� �� �� �ߵ��ϴ� Ʃ�丮���� �ƴϸ� continue ó���� �Ѵ�.

		nCondition2 = -1 ;													// �ι�° �ߵ� ������ -1�� �����Ѵ�.
		nCondition2 = pCurTutorial->Get_StartCondition2() ;					// �ι�° �ߵ� ������ �޴´�.

		if( nCondition2-1 > HERO->GetLevel() ) continue ;					// ���� �̴��̸� continue �Ѵ�.
		
		Start_CurTutorial(pCurTutorial) ;									// ���� Ʃ�丮���� �����Ѵ�.

		return ;
	}
}





//=========================================================================
//		�������� �������� �� Ʃ�丮�� ���� ���θ� üũ�ϴ� �Լ�.
//=========================================================================
void cTutorialManager::Check_GetItem(CItem* pItem)							// �������� �������� �� Ʃ�丮�� ���� ���θ� üũ�ϴ� �Լ�.
{
	if( !pItem )															// ���ڷ� �Ѿ�� ������ ������ ��ȿ���� ������,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		MessageBox(NULL, PERR_MSG3, CGI_CATION, MB_OK);						// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

		return ;															// return ó���� �Ѵ�.
	}

	if( !m_bActiveTutorial ) return ;										// Ʃ�丮���� ��Ȱ��ȭ ���¸�, return ó���� �Ѵ�.

	if(m_bUpdated) return ;													// Ʃ�丮�� �̺�Ʈ�� ������Ʈ �Ǿ�����, return ó���� �Ѵ�.
	
	cTutorial* pCurTutorial ;												// ���� Ʃ�丮�� ������ ���� �����͸� �����Ѵ�.

	int nCondition1 ;														// Ʃ�丮���� �ߵ� ������ ���� ������ �����Ѵ�.
	int nCondition2 ;														// �ι�° �ߵ� ������ ���� ������ �����Ѵ�.

	PTRLISTPOS pos = NULL ;													// Ʃ�丮�� ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pos = m_Tutorial_List.GetHeadPosition() ;								// Ʃ�丮�� ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	while(pos)																// ��ġ ������ ��ȿ�� ���� whlie���� ������.
	{
		pCurTutorial = NULL ;												// Ʃ�丮�� ������ ���� �����͸� null ó���� �Ѵ�.
		pCurTutorial = (cTutorial*)m_Tutorial_List.GetNext(pos) ;			// ��ġ�� �ش��ϴ� Ʃ�丮�� ������ �ް�, ��ġ�� �������� �����Ѵ�.

		if( !pCurTutorial ) continue ;										// Ʃ�丮�� ������ ��ȿ���� ������ continue ó���� �Ѵ�.

		if( pCurTutorial->Is_Completed() ) continue ;						// �Ϸ��� Ʃ�丮���̸�, �ǳʶڴ�.

		nCondition1 = -1 ;													// �ߵ� ���� ������ -1�� �����Ѵ�.
		nCondition1 = pCurTutorial->Get_StartCondition1() ;					// ���� Ʃ�丮���� �ߵ� ������ �޴´�.

		if( nCondition1 != e_GET_ITEM ) continue ;							// ������ ����� �ߵ��ϴ� Ʃ�丮���� �ƴϸ� continue ó���� �Ѵ�.

		nCondition2 = -1 ;													// �ι�° �ߵ� ������ -1�� �����Ѵ�.
		nCondition2 = pCurTutorial->Get_StartCondition2() ;					// �ι�° �ߵ� ������ �޴´�.

		if( nCondition2 == e_GET_STACK_ITEM )								// �ι�° �ߵ� ������ ���� �������� ���,
		{
			CInventoryExDialog* pInven = NULL ;								// �κ��丮 ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
			pInven = GAMEIN->GetInventoryDialog() ;							// �κ��丮 ������ �޴´�.

			if( !pInven )
			{
				#ifdef _GMTOOL_												// GM�� �����,
				MessageBox(NULL, PERR_MSG4, CGI_CATION, MB_OK);				// ���� �޽����� �޽��� �ڽ��� ����.
				#endif

				continue ;													// continue ó���� �Ѵ�.
			}

			CItem* pCheckItem = NULL ;										// üũ �������� ���� �����͸� �����ϰ� null ó���� �Ѵ�.
			pCheckItem = pInven->GetItemForIdx( pItem->GetItemIdx() ) ;		// �κ����� ���� �������� �ε����� üũ �������� �޴´�.

			if( !pCheckItem ) continue ;									// �κ��丮���� üũ �������� ã�� ���ϸ�, continue ó���� �Ѵ�.

			if(pCheckItem->GetItemBaseInfo().Durability > 1)				// üũ �������� ������ 1�� �̻��̸�,
			{
				Start_CurTutorial(pCurTutorial) ;							// ���� Ʃ�丮���� �����Ѵ�.

				return  ;													// return ó���� �Ѵ�.
			}
		}
		else																// �ι�° �ߵ� ������ ���� �������� �ƴ� ���,
		{
			Start_CurTutorial(pCurTutorial) ;								// ���� Ʃ�丮���� �����Ѵ�.

			return ;														// return ó���� �Ѵ�.
		}
	}
}





//=========================================================================
//		Ʃ�丮�� ������ ������Ʈ �ϴ� �Լ�.
//=========================================================================
void cTutorialManager::Check_OpenDialog(DWORD dwWindowID, BOOL bVal) 		// â�� ���� �Ϸ� ������ üũ�ϴ� �Լ�.
{
	if( !m_pTutorialDlg )													// Ʃ�丮�� ���̾�α� ������ ������,
	{
		//#ifdef _GMTOOL_														// GM�� �����,
		//MessageBox(NULL, CERR_MSG5, COD_CATION, MB_OK);						// ���� �޽����� �޽��� �ڽ��� ����.
		//#endif

		return ;															// return ó���� �Ѵ�.
	}

	if( !m_pTutorialDlg )													// Ʃ�丮�� ���̾�α� ������ ������,
	{
		return ;															// return ó���� �Ѵ�.
	}

	if( m_nCurTutorialIdx < 0 )												// Ʃ�丮�� �ε����� 0���� ������,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		char tempBuf[128] = {0, } ;											// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
		sprintf(tempBuf, TERR_MSG4, m_nCurTutorialIdx) ;					// ���� �޽����� �ϼ��Ѵ�.
		MessageBox(NULL, tempBuf, COD_CATION, MB_OK);						// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

		return ;															// return ó���� �Ѵ�.
	}

	cTutorial* pTutorial = NULL ;											// Ʃ�丮�� ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pTutorial = Get_Tutorial(m_nCurTutorialIdx) ;							// ���� �������� Ʃ�丮�� ������ �޴´�.

	if( !pTutorial )														// Ʃ�丮�� ������ ��ȿ���� ������,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		char tempBuf[128] = {0, } ;											// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
		sprintf(tempBuf, NERR_MSG4, m_nCurTutorialIdx) ;					// ���� �޽����� �ϼ��Ѵ�.
		MessageBox(NULL, tempBuf, COD_CATION, MB_OK);						// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

		return ;															// return ó���� �Ѵ�.
	}

	//if( pTutorial->Is_Completed() ) return ;								// �Ϸ��� Ʃ�丮�� �̸�, return ó���� �Ѵ�.

	SUB_TUTORIAL* pSub = NULL ;												// ���� Ʃ�丮���� ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pSub = m_pTutorialDlg->GetCurSubTutorial() ;							// ���� Ʃ�丮���� ���� ���� Ʃ�丮�� ������ �޴´�.

	if( !pSub )
	{
		//#ifdef _GMTOOL_														// GM�� �����,
		//MessageBox(NULL, TERR_MSG16, COD_CATION, MB_OK);					// ���� �޽����� �޽��� �ڽ��� ����.
		//#endif

		return ;															// return ó���� �Ѵ�.
	}

	if( pSub->nComplete_Condition1 != e_OPEN_DIALOG ) return ;				// �Ϸ� ������ â�� ���� ���� �ƴϸ�, return ó���� �Ѵ�.

	DWORD dwCheckID = 0 ;													// üƮ�� ������ ���̵� �����ϰ� 0���� �����Ѵ�.

	LONG lWindwID = 0 ;														// ������ ���̵� 0���� �����Ѵ�.
	lWindwID = IDSEARCH(pSub->szCompleteCondition2) ;						// ����� �ϴ� â�� ���̵� �޴´�.
	cWindow* pWindow = WINDOWMGR->GetWindowForID(lWindwID) ;				// ������ ������ �޴´�.

	if( !pWindow )
	{
		#ifdef _GMTOOL_														// GM�� �����,
		char tempBuf[128] = {0, } ;											// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
		sprintf(tempBuf, TERR_MSG17, pSub->szCompleteCondition2) ;			// ���� �޽����� �ϼ��Ѵ�.
		MessageBox(NULL, tempBuf, COD_CATION, MB_OK);						// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

		return ;															// return ó���� �Ѵ�.
	}

	dwCheckID = pWindow->GetID() ;											// �������� ���̵� �޴´�.

	if( dwCheckID == dwWindowID )											// ���ڷ� �Ѿ�� ���̵�� üũ ���̵� ������,
	{
		if( bVal )															// Ȱ��ȭ ���� true �̸�,
		{
			if( m_pTutorialDlg->IsHaveNextSubTutorial() )					// ���� Ʃ�丮���� ������,
			{
				m_pTutorialDlg->StartNextSubTutorial() ;					// ���� Ʃ�丮���� �����Ѵ�.
			}
			else															// ���� Ʃ�丮���� ������,
			{
				m_pTutorialDlg->UpdateTutorial() ;							// Ʃ�丮���� �Ϸ� �Ѵ�.
			}
		}
	}
}





//=========================================================================
//		Ʃ�丮�� ������ ������Ʈ �ϴ� �Լ�.
//=========================================================================
void cTutorialManager::UpdateTutorial(int nCompleteIndex) 					// Ʃ�丮�� ������ ������Ʈ �ϴ� �Լ�.
{
	m_bUpdated = FALSE ;													// ������Ʈ ���� ������ false�� �����Ѵ�.

	int		nIndex ;														// �ε����� ���� ������ �����Ѵ�.

	cTutorial* pTutorial ;													// Ʃ�丮�� ������ ���� �����͸� �����Ѵ�.

	PTRLISTPOS pos = NULL ;													// ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pos = m_Tutorial_List.GetHeadPosition() ;								// Ʃ�丮�� ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	int nCompleteIdx = -1 ;
	while(pos)																// ��ġ ������ ��ȿ�� ���� while���� ������.
	{
		pTutorial = NULL ;													// Ʃ�丮�� ������ ���� �����͸� null ó���� �Ѵ�.
		pTutorial = (cTutorial*)m_Tutorial_List.GetNext(pos) ;				// ��ġ�� �ش��ϴ� Ʃ�丮�� ������ �ް�, ��ġ�� �������� �����Ѵ�.

		if( !pTutorial ) continue ;											// Ʃ�丮�� ������ ��ȿ���� ������, continue ó���� �Ѵ�.

		if(pTutorial->Get_StartCondition1() == e_COMPLETE_TUTORIAL)
		{
			if(nCompleteIdx == pTutorial->Get_StartCondition2() && !pTutorial->Is_Completed()) 
			{
				Start_CurTutorial(pTutorial) ;
				return ;
			}
		}
		else if(pTutorial->Get_StartCondition1() == e_LEVEL)
		{
			if(HERO->GetLevel() >= pTutorial->Get_StartCondition2() && !pTutorial->Is_Completed())
			{
				Start_CurTutorial(pTutorial) ;
				return ;
			}
		}

		nIndex = -1 ;														// �ε����� -1�� �����Ѵ�.
		nIndex = pTutorial->Get_Index() ;									// ���� Ʃ�丮���� �ε����� �޴´�.

		if( nIndex < 0 )
		{
			#ifdef _GMTOOL_													// GM�� �����,
			char tempBuf[128] = {0, } ;										// ���� ��¿� �ӽ� ���۸� �����Ѵ�.
			sprintf(tempBuf, TERR_MSG4, nIndex) ;							// ���� �޽����� �ϼ��Ѵ�.
			MessageBox(NULL, tempBuf, COD_CATION, MB_OK);					// ���� �޽����� �޽��� �ڽ��� ����.
			#endif

			continue ;														// continue ó���� �Ѵ�.
		}

		if( nIndex != nCompleteIndex ) continue ;							// ���ڷ� �Ѿ�� �ε����� ���� Ʃ�丮�� �ε����� �ٸ��� continue ó���� �Ѵ�.

		pTutorial->Set_Complete( TRUE ) ;									// Ʃ�丮���� �Ϸ� ���θ� true �� �����Ѵ�.

		MSG_INT Msg;														// �޽��� ����ü�� �����Ѵ�.

		Msg.Category = MP_TUTORIAL;											// ī�װ��� Ʃ�丮��� �����Ѵ�.
		Msg.Protocol = MP_TUTORIAL_UPDATE_SYN;								// ���������� Ʃ�丮�� ������Ʈ ��û���� �����Ѵ�.
		Msg.dwObjectID = HERO->GetID();										// HERO ���̵� �����Ѵ�.
		Msg.nData = pTutorial->Get_Index() ;								// ���� Ʃ�丮�� �ε����� �����Ѵ�.

		NETWORK->Send( &Msg, sizeof(Msg) );									// ���� �� ������ �޽����� �����Ѵ�.

		AUDIOMGR->Play(Get_EndTutorialSndIdx(), HERO);						// Ʃ�丮�� �Ϸ� ���� �ε����� �÷����Ѵ�.

		nCompleteIdx = pTutorial->Get_Index() ;
	}
}





//=========================================================================
//		���� Ʃ�丮���� �����϶�� �Լ�.
//=========================================================================
void cTutorialManager::Start_CurTutorial(cTutorial* pTutorial)				// ���� Ʃ�丮���� �����϶�� �Լ�.
{
	if( !m_bActiveTutorial ) return ;										// Ʃ�丮���� Ȱ��ȭ ���°� �ƴϸ�, return ó���� �Ѵ�.

	if( !pTutorial )														// ���ڷ� �Ѿ�� Ʃ�丮�� ������ ��ȿ���� ������,
	{
		#ifdef _GMTOOL_														// GM�� �����,
		MessageBox(NULL, TERR_MSG7, SCT_CATION, MB_OK);						// ���� �޽����� �޽��� �ڽ��� ����.
		#endif

        return ;															// return ó���� �Ѵ�.		
	}

	m_nCurTutorialIdx = pTutorial->Get_Index() ;							// ���� Ʃ�丮�� �ε����� �����Ѵ�.

	if( m_pTutorialDlg->GetSpeechBack()->IsActive() ) return ;

	m_pTutorialDlg->Set_TutorialInfo(pTutorial) ;							// Ʃ�丮�� ���̾�α׿��� Ʃ�丮���� �����Ѵ�.

	if( pTutorial->Get_StartCondition1() == 0 )								// �ߵ� ������ ���ӿ� ó�� �������� �� �̸�, 
	{
		m_pTutorialDlg->Start_Tutorial() ;									// Ʃ�丮���� �ٷ� �����Ѵ�.
		return ;															// return ó���� �Ѵ�.
	}

	if( m_pTutorialDlg->GetSpeechBack()->IsActive() ) return ;				// ���� Ʃ�丮�� â�� ���� ���¸�, ��ư�� Ȱ��ȭ ���� �ʴ´�.

	m_pBtnDlg->Active_NoticeBtn() ;											// Ʃ�丮�� �˸� ��ư�� Ȱ��ȭ �Ѵ�.
}





//=========================================================================
//		��� ���̾�α׵��� ������ �ϴ� �Լ�.
//=========================================================================
void cTutorialManager::Reset_MemberDlg() 									// ��� ���̾�α׵��� ������ �ϴ� �Լ�.
{
	m_bUpdated = FALSE ;													// ������Ʈ ���θ� false�� �����Ѵ�.
	m_nCurTutorialIdx	= 0 ;												// ���� Ʃ�丮�� �ε����� 0�� �����Ѵ�.

	m_pTutorialDlg	= NULL ;												// Ʃ�丮�� ������ ���� ���̾�α� ������.
	m_pHelperDlg	= NULL ;												// ���� ��� ������ ���� ���̾�α� ������.
	m_pBtnDlg		= NULL ;												// Ʃ�丮�� �˸� ��ư ���̾�α� ������.
}





//=========================================================================
// 080502 LYW : ���� �� Ÿ���� Ʃ�丮���� �Ϸ� �Ǿ����� ��ȯ�ϴ� �Լ� �߰�.
//=========================================================================
BYTE cTutorialManager::IsCompleteTutorial(BYTE byTutorialType)
{
	if( !m_bActiveTutorial ) return FALSE ;									// Ʃ�丮���� ��Ȱ��ȭ ���¸�, return ó���� �Ѵ�.���� �Ѵ�.
	
	cTutorial* pCurTutorial ;												// ���� Ʃ�丮�� ������ ���� �����͸� �����Ѵ�.

	PTRLISTPOS pos = NULL ;													// Ʃ�丮�� ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pos = m_Tutorial_List.GetHeadPosition() ;								// Ʃ�丮�� ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	while(pos)																// ��ġ ������ ��ȿ�� ���� whlie���� ������.
	{
		pCurTutorial = NULL ;												// Ʃ�丮�� ������ ���� �����͸� null ó���� �Ѵ�.
		pCurTutorial = (cTutorial*)m_Tutorial_List.GetNext(pos) ;			// ��ġ�� �ش��ϴ� Ʃ�丮�� ������ �ް�, ��ġ�� �������� �����Ѵ�.

		if( !pCurTutorial ) continue ;										// Ʃ�丮�� ������ ��ȿ���� ������ continue ó���� �Ѵ�.

		if( pCurTutorial->Get_StartCondition1() != byTutorialType ) continue ;	// Ʃ�丮�� Ÿ�� üũ.

		if( pCurTutorial->Is_Completed() ) return TRUE ;					// ��� return ó��.
		else return FALSE ;
	}

	ASSERT(TRUE) ;

#ifdef _GMTOOL_
	MessageBox(NULL, "Invalid tutorial type!!", __FUNCTION__, MB_OK) ;
#endif //_GMTOOL_
	return FALSE ;
}





//=========================================================================
// 080502 LYW : ���� �� Ÿ���� Ʃ�丮���� �����ϴ� �Լ� �߰�.
//=========================================================================
void cTutorialManager::Start_Specification_Tutorial(BYTE byTutorialType)
{
	if( !m_bActiveTutorial ) return ;										// Ʃ�丮���� ��Ȱ��ȭ ���¸�, return ó���� �Ѵ�.���� �Ѵ�.
	
	cTutorial* pCurTutorial ;												// ���� Ʃ�丮�� ������ ���� �����͸� �����Ѵ�.

	PTRLISTPOS pos = NULL ;													// Ʃ�丮�� ��ġ ������ ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	pos = m_Tutorial_List.GetHeadPosition() ;								// Ʃ�丮�� ����Ʈ�� ���� �����ϰ� ��ġ ������ �޴´�.

	while(pos)																// ��ġ ������ ��ȿ�� ���� whlie���� ������.
	{
		pCurTutorial = NULL ;												// Ʃ�丮�� ������ ���� �����͸� null ó���� �Ѵ�.
		pCurTutorial = (cTutorial*)m_Tutorial_List.GetNext(pos) ;			// ��ġ�� �ش��ϴ� Ʃ�丮�� ������ �ް�, ��ġ�� �������� �����Ѵ�.

		if( !pCurTutorial ) continue ;										// Ʃ�丮�� ������ ��ȿ���� ������ continue ó���� �Ѵ�.

		if( pCurTutorial->Get_StartCondition1() != byTutorialType ) continue ;	// Ʃ�丮�� Ÿ�� üũ.

		if( pCurTutorial->Is_Completed() ) return ;

		Start_CurTutorial(pCurTutorial) ;

		return ;
	}

	ASSERT(TRUE) ;

#ifdef _GMTOOL_
	MessageBox(NULL, "Invalid tutorial type!!", __FUNCTION__, MB_OK) ;
#endif //_GMTOOL_
}