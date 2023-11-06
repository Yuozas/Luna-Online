// FamilyDialog.cpp: implementation of the CFamilyDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "../Input/Mouse.h"															// ���콺 ��� ������ �ҷ��´�.

#include "WindowIDEnum.h"															// ������ ���̵� ��������� �ҷ��´�.

#include "./Interface/cWindowManager.h"												// ������ �Ŵ��� ��������� �ҷ��´�.
#include "../Interface/cResourceManager.h"											// ���ҽ� �Ŵ��� ��������� �ҷ��´�.

#include "ObjectManager.h"															// ������Ʈ �Ŵ��� ��������� �ҷ��´�.
//#include "FamilyManager.h"															// �йи� �Ŵ��� ��������� �ҷ��´�.
#include "ChatManager.h"															// ä�� �Ŵ��� ��������� �ҷ��´�.

#include "../hseos/family/shfamilymanager.h"										// �йи� �Ŵ��� �ش������� �ҷ��´�.

#include "./Interface/cScriptManager.h"												// ��ũ��Ʈ �Ŵ��� ��������� �ҷ��´�.

#include "cFont.h"	// �׽�Ʈ�� ���� ��Ʈ�� �ҷ��´�.


#include "./interface/cStatic.h"													// ����ƽ �������̽� ��������� �ҷ��´�.
#include "./interface/cComboBox.h"													// �޺��ڽ� �������̽� ��������� �ҷ��´�.
#include "./interface/cListDialog.h"												// ����Ʈ ���̾�α� �������̽� ��������� �ҷ��´�.
#include "./interface/cPushupButton.h"												// Ǫ���� ��ư �������̽� ��������� �ҷ��´�.
#include "cMsgBox.h"																// �޽��� �ڽ� �������̽� ��������� �ҷ��´�.

#include "GameIn.h"																	// ������ ��������� �ҷ��´�.
//#include "Family.h"																	// �йи� ��������� �ҷ��´�.

#include "FamilyDialog.h"															// �йи� ���̾�α� ��������� �ҷ��´�.
#include "FamilyRankDialog.h"														// �йи� ��ŷ ���̾�α� ��������� �ҷ��´�.
#include "FamilyNickNameDialog.h"													// �йи� ȣĪ ���� ���̾�α� ��������� �ҷ��´�.
//#include "FamilyUnion.h"	

#include "cImageSelf.h"
#include "FamilyMarkDialog.h"
// desc_hseos_����ý���_01
// S ����ý��� �߰� added by hseos 2007.10.11
#include "../hseos/Farm/SHFarmManageDlg.h"
// E ����ý��� �߰� added by hseos 2007.10.11

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFamilyDialog::CFamilyDialog()														// ������ �Լ�.
{
	m_pFamilyName		= NULL ;													// �йи� �̸��� NULL ó�� �Ѵ�.
	m_pMasterName		= NULL ;													// ������ �̸��� NULL ó�� �Ѵ�.
	m_pHonorPoint		= NULL ;													// �� ����Ʈ�� NULL ó�� �Ѵ�.

	m_pListDlg			= NULL ;													// ��� ����Ʈ ���̾�α׸� NULL ó�� �Ѵ�.

	m_pMemberID			= NULL ;													// ��� ���̵� NULL ó�� �Ѵ�.
	m_pMemberLevel		= NULL ;													// ��� ������ NULL ó�� �Ѵ�.
	m_pMemberClass		= NULL ;													// ��� Ŭ������ NULL ó�� �Ѵ�.
	m_pMemberLogin		= NULL ;													// ��� �α����� NULL ó�� �Ѵ�.
	m_pMemberNickName	= NULL ;													// ��� ȣĪ�� NULL ó�� �Ѵ�.
	m_pMemberGuildName  = NULL ;													// ��� ������ NULL ó�� �Ѵ�.

	m_pChangeNickName	= NULL ;													// ȣĪ���� ��ư�� NULL ó�� �Ѵ�.
	m_pInvite			= NULL ;													// �ʴ��û ��ư�� NULL ó�� �Ѵ�.
	m_pRegistMark		= NULL ;													// ������ ��ư�� NULL ó�� �Ѵ�.

	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.10.22
	m_pLeaveBtn			= NULL;
	m_pExpelBtn			= NULL;
	m_pFarmManageBtn	= NULL;
	// E �йи� �߰� added by hseos 2007.10.22

	for( int count = 0 ; count < MAX_MEMBER_COUNT ; ++count )						// �ִ� ��� ����ŭ for���� ������.
	{
		//m_pClassImg[count] = NULL ;												// ī��Ʈ�� �ش��ϴ� �̹����� NULLó���� �Ѵ�.

		ZeroMemory( &m_pClassImg[count], sizeof(cImage) ) ;
	}

	for( int count = 0 ; count < MAX_MEMBER_COUNT ; ++count )						// �ִ� ��� ����ŭ for���� ������.
	{
		//m_pLongInImg[count] = NULL ;												// �α��� �̹����� NULL ó���� �Ѵ�.

		ZeroMemory( &m_pLongInImg[count], sizeof(cImage) ) ;
	}

	for( int count = 0 ; count < MAX_MEMBER_COUNT ; ++count )						// �ִ� ��� ����ŭ for���� ������.
	{
		//m_pLogOutImg[count] = NULL ;												// �α׾ƿ� �̹����� NULL ó���� �Ѵ�.

		ZeroMemory( &m_pLogOutImg[count], sizeof(cImage) ) ;						
	}

	m_nCurSelectIdx = 0 ;															// ���� ���� �� ����� 0������ �����Ѵ�.

	//m_ClassIconList.RemoveAll() ;													// Ŭ���� ������ ����Ʈ�� ����ش�.
}

CFamilyDialog::~CFamilyDialog()														// �Ҹ��� �Լ�.
{
	//ReleaseIconList() ;																// Ŭ���� ������ ����Ʈ�� �����Ѵ�.
}

void CFamilyDialog::Linking()														// ��Ʈ�ѵ��� ��ũ�ϴ� �Լ�.
{
	m_pFamilyName		= (cStatic*)GetWindowForID(FMY_NAME) ;						// �йи� �̸��� ��ũ�Ѵ�.
	m_pMasterName		= (cStatic*)GetWindowForID(FMY_MASTERNAME) ;				// ������ �̸��� ��ũ�Ѵ�.
	m_pHonorPoint		= (cStatic*)GetWindowForID(FMY_HONORPOINT) ;				// �� ����Ʈ�� ��ũ�Ѵ�.

	m_pListDlg			= (cListDialog*)GetWindowForID(FMY_MEMBERLIST) ;			// ��� ����Ʈ ���̾�α׸� ��ũ�Ѵ�.

	m_pMemberID			= (cStatic*)GetWindowForID(FMY_MEMBERID) ;					// ��� ���̵� ��ũ�Ѵ�.
	m_pMemberLevel		= (cStatic*)GetWindowForID(FMY_MEMBERLEVEL) ;				// ��� ������ ��ũ�Ѵ�.
	m_pMemberClass		= (cStatic*)GetWindowForID(FMY_MEMBERCLASS) ;				// ��� Ŭ������ ��ũ�Ѵ�.
	m_pMemberLogin		= (cStatic*)GetWindowForID(FMY_MEMBERLOGIN) ;				// ��� �α����� ��ũ�Ѵ�.
	m_pMemberNickName	= (cStatic*)GetWindowForID(FMY_MEMBERNICKNAME) ;			// ��� ȣĪ�� ��ũ�Ѵ�.
	m_pMemberGuildName  = (cStatic*)GetWindowForID(FMY_MEMBERGUILDNAME) ;			// ��� ������ ��ũ�Ѵ�.

	m_pChangeNickName	= (cButton*)GetWindowForID(FMY_CHANGENICKNAME) ;			// ȣĪ���� ��ư�� ��ũ�Ѵ�.
	m_pInvite			= (cButton*)GetWindowForID(FMY_INVITE) ;					// �ʴ��û ��ư�� ��ũ�Ѵ�.
	m_pRegistMark		= (cButton*)GetWindowForID(FMY_REGISTMARK) ;				// ������ ��ư�� ��ũ�Ѵ�.

	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.10.22
	m_pLeaveBtn			= (cButton*)GetWindowForID(FMY_LEAVE);
	m_pExpelBtn			= (cButton*)GetWindowForID(FMY_EXPEL);
	m_pFarmManageBtn	= (cButton*)GetWindowForID(FMY_FARM_MANAGE);
	// E �йи� �߰� added by hseos 2007.10.22
}

void CFamilyDialog::UpdateAllInfo() 												// �йи� â�� ���̴� ��� ������ ������Ʈ �Ѵ�.
{
	CSHFamily* pFamily = HERO->GetFamily() ;										// �йи� ������ �޾ƿ´�.

	if( pFamily )																	// �йи� ������ ��ȿ���� üũ�Ѵ�.
	{
		m_pFamilyName->SetStaticText(pFamily->Get()->szName) ;						// �йи����� �����Ѵ�.

		m_pMasterName->SetStaticText(pFamily->Get()->szMasterName) ;				// �������� �̸��� �����Ѵ�.

		SetMemberList(pFamily) ;													// ��� ����Ʈ�� �����Ѵ�.

		SetHonorPoint(0) ;															// �� ����Ʈ�� �����Ѵ�.

		CSHFamilyMember* pMember = pFamily->GetMember(0) ;							// ��� ������ �޾ƿ´�.

		if( pMember )																// ��� ������ ��ȿ���� üũ�Ѵ�.
		{
			SetMemberInfo( pMember) ;												// ��� ������ �����Ѵ�.
		}

		ITEM* pItem = m_pListDlg->GetItem(0) ;										// �־��� �ε����� ������ ������ �޴´�.

		if( pItem )																	// ������ ������ ��ȿ���� üũ�Ѵ�.
		{
			pItem->rgb = RGB_HALF(255, 255, 0) ; 									// �������� ������ �ٽ� �����Ѵ�.

			m_nCurSelectIdx = 0 ;													// ���� ���� �� ��� �ε����� �����Ѵ�.
		}

  		if (pFamily->Get()->nMasterID == HERO->GetID())
		{
			m_pLeaveBtn->SetDisable(TRUE);
			m_pLeaveBtn->SetImageRGB(RGB( 190, 190, 190 ));
		}
		else
		{
			m_pExpelBtn->SetDisable(TRUE);
			m_pExpelBtn->SetImageRGB(RGB( 190, 190, 190 ));
			m_pInvite->SetDisable(TRUE);	
			m_pInvite->SetImageRGB(RGB( 190, 190, 190 ));
			m_pRegistMark->SetDisable(TRUE);
			m_pRegistMark->SetImageRGB(RGB( 190, 190, 190 ));
		}
	}
}

void CFamilyDialog::SetHonorPoint( DWORD dwPoint )									// �� ����Ʈ�� �����ϴ� �Լ�.
{
	CSHFamily* pFamily = HERO->GetFamily() ;										// �йи� ������ �޾ƿ´�.

	if( pFamily )																	// �йи� ������ ��ȿ���� üũ�Ѵ�.
	{
 		char tempBuf[10+5] = {0, } ;												// �йи� ����Ʈ�� ���ڿ��� �ٲپ� �� �ӽ� ���� ����.

		itoa(pFamily->GetEx()->nHonorPoint, tempBuf, 10) ;							// �йи� ����Ʈ�� ���ڷ� ��ȯ�Ѵ�.

		m_pHonorPoint->SetStaticText(tempBuf) ;										// �йи� ����Ʈ�� �����Ѵ�.
	}

	// �� ����Ʈ�� �����Ǵ� ��Ȳ���� dwPoint �� - int ���� �ֱ� ������  
	// �ݵ�� int �� ĳ���� �ؼ� ������� Ȯ���ؾ� ��. by hseos
	//if( (int)dwPoint > 0 )															// �Ѿ�� �� ����Ʈ�� 0���� ũ��,
	//{
	//	char tempBuf[128] = {0, } ;													// �� ����Ʈ�� ���ڿ��� �ٲپ� �� �ӽ� ���� ����.								

	//	sprintf(tempBuf, "�� ����Ʈ�� %d ����Ʈ ����Ͽ����ϴ�.", dwPoint) ;		// �� ����Ʈ�� ���ڷ� ��ȯ�Ѵ�.

	//	CHATMGR->AddMsg(CTC_SYSMSG, tempBuf) ;										// ä��â�� ����Ʈ �� ������ �Ѵ�.
	//}
}

void CFamilyDialog::SetMemberInfo( CSHFamilyMember* pMember )						// �йи� â �ϴܺ�, ����� �� ������ ��Ÿ���� �Լ�.
{
	m_pMemberID->SetStaticText( pMember->Get()->szName ) ;							// ��� �̸��� �����Ѵ�.

	char tempBuf[10] = {0, } ;														// ��� ������ ���ڷ� ��ȯ �� �ӽ� ���۸� �����Ѵ�.
	itoa(pMember->Get()->nLV, tempBuf, 10) ;										// ��� ������ ���ڷ� ��ȯ�Ѵ�.
	m_pMemberLevel->SetStaticText( tempBuf ) ;										// ��� ������ �����Ѵ�.

	int nClass		= pMember->Get()->nJobFirst ;									// ����� Ŭ���� �迭 ���� �޴´�.
	int nRace		= pMember->Get()->nRace ;										// ����� ���� ��ȣ�� �޴´�.
	int nJobGrade	= pMember->Get()->nJobGrade ;									// ����� Ŭ���� �ܰ踦 �޴´�.
	int nJobNum		= pMember->Get()->nJobCur ;										// ����� Ŭ���� ��ȣ�� �޴´�.

	if( nJobGrade == 1 )													// ���� �ܰ谡 1�϶���, �迭���� �Ѱ��� �����ۿ� ���� ������
	{
		nJobNum = 1 ;														// ���� ��ȣ�� 1�� �����Ѵ�.
	}

	int nClassIdx = ((nClass)*1000) + ((nRace+1)*100) + (nJobGrade*10) + nJobNum ;	// ����� Ŭ���� ��ȣ�� �޴´�.

	m_pMemberClass->SetStaticText(RESRCMGR->GetMsg(RESRCMGR->GetClassNameNum(nClassIdx))) ;	// ��� Ŭ������ �����Ѵ�.


	ZeroMemory(tempBuf, sizeof(tempBuf)) ;											// �ӽ� ���۸� �ʱ�ȭ �Ѵ�.

	int nIsLogIn = pMember->Get()->eConState ;										// ����� �α��� ���θ� �޴´�.
	char* pChatMsg = NULL;

	switch(nIsLogIn)																// �α��� ���θ� Ȯ���Ѵ�.
	{
	//case MEMBER_CONSTATE_LOGOFF :	strcpy(tempBuf, "���Ӿ���") ;					// ���� ���� ���·� �����Ѵ�.
	//case MEMBER_CONSTATE_LOGIN :	strcpy(tempBuf, "������") ;						// ���� ���� ���·� �����Ѵ�.

	case CSHGroupMember::MEMBER_CONSTATE_LOGOFF:
		{
			// ���� ���� ���·� �����Ѵ�.
			//strcpy(tempBuf, CHATMGR->GetChatMsg( 1443 )) ;
			pChatMsg = CHATMGR->GetChatMsg( 1443 );// 080306 NYJ --- Buffer Overflow�� �����ϱ� ���� �����ͷ� �޾ƿ��°� �����ҵ�.
		}
		break ;
	case CSHGroupMember::MEMBER_CONSTATE_LOGIN:
		{
			// ���� ���� ���·� �����Ѵ�.
			//strcpy(tempBuf, CHATMGR->GetChatMsg( 1442 )) ;
			pChatMsg = CHATMGR->GetChatMsg( 1442 );// 080306 NYJ --- Buffer Overflow�� �����ϱ� ���� �����ͷ� �޾ƿ��°� �����ҵ�.
		}
		break ;
	}
	
	m_pMemberLogin->SetStaticText(/*tempBuf*/pChatMsg) ;										// ����� �α��� ���θ� �����Ѵ�.
	m_pMemberNickName->SetStaticText( pMember->Get()->szNickname ) ;				// ����� ȣĪ�� �����Ѵ�.
	m_pMemberGuildName->SetStaticText( pMember->GetEx()->szGuild ) ;				// ����� ������ �����Ѵ�.
}

void CFamilyDialog::SetMemberList(CSHFamily* pFamily)
{
	DWORD dwColor = 0xffffffff ;													// ���� ������ �����Ѵ�.

	int nClassBlank = 8 ;															// Ŭ���� �̹����� ����� ������ ��� ���� ��� ��������.
	int nLevelBlank = 0 ;															// ������ ����� ������ ��� ���� ��� ���� ����.
	int nNameBlank  = 0 ;															// �̸��� ����� ������ ��� ���� ��� ���� ����.

	int nClass		= 0 ;															// ��� Ŭ������ ���� ������ �����Ѵ�.
	int nRace		= 0 ;															// ����� ������ ���� ������ �����Ѵ�.
	int nJobGrade	= 0 ;															// ����� Ŭ���� �ܰ踦 ���� ������ �����Ѵ�.
	int nJobNum		= 0 ;															// ����� Ŭ���� ��ȣ�� ���� ������ �����Ѵ�.

	int nLogIn		= 0 ;															// ��� �α��� ���θ� ���� ������ �����Ѵ�.

	char tempBuf[128]	= {" ",} ;													// ��� ������ ���� �ӽ� ���۸� �����Ѵ�.
	char classBuf[8]	= {" ",} ;													// ��� Ŭ������ ��ĭ�� ��Ÿ�� ���۸� �����Ѵ�.
	char levelBuf[8]	= {" ",} ;													// ��� ������ ���� ���۸� �����Ѵ�.
	char membername[MAX_NAME_LENGTH+1] = {" ",} ;									// ��� �̸��� ���� ���۸� �����Ѵ�.

	CSHFamilyMember* pMember = NULL ;												// ��� ������ ���� �����͸� �����Ѵ�.

	m_pListDlg->RemoveAll() ;														// ����Ʈ�� ����.

	int nMemberCount = pFamily->Get()->nMemberNum ;									// ��� ���� ���Ѵ�.

	for( int bigCount = 0 ; bigCount < nMemberCount ; ++bigCount )					// ��� ����ŭ ������ ������.
	{
		pMember = pFamily->GetMember(bigCount) ;									// ī��Ʈ�� �ش��ϴ� ��� ������ �޴´�.

		if( pMember && pMember->Get()->nID != 0 )									// ��� ������ ��ȿ���� üũ�Ѵ�.
		{

			nClass		= pMember->Get()->nJobFirst ;								// ����� Ŭ���� �迭 ���� �޴´�.
			nRace		= pMember->Get()->nRace ;									// ����� ���� ��ȣ�� �޴´�.
			nJobGrade	= pMember->Get()->nJobGrade ;								// ����� Ŭ���� �ܰ踦 �޴´�.
			nJobNum		= pMember->Get()->nJobCur ;

			if( nJobGrade == 1 )													// ���� �ܰ谡 1�϶���, �迭���� �Ѱ��� �����ۿ� ���� ������
			{
				nJobNum = 1 ;														// ���� ��ȣ�� 1�� �����Ѵ�.
			}

			int nClassIdx = ((nClass)*1000) + ((nRace+1)*100) + (nJobGrade*10) + nJobNum ;	// ����� Ŭ���� ��ȣ�� �޴´�.

			//int nIconNum = GetClassIconNum(nClassIdx) ;								// Ŭ������ �ش��ϴ� �̹��� ��ȣ�� �޴´�.
			//if( nIconNum >= 0 && m_ClassIconList.GetCount() >= nIconNum )			// ������ �̹��� ��ȣ�� ������ Ȯ���Ѵ�.
			// 080320 LYW --- FamilyDialog : Ŭ���� ������ ����� ���ҽ� �Ŵ����� �ű�
 			int nIconNum = RESRCMGR->GetClassIconNum(nClassIdx) ;
 			if( nIconNum >= 0 && RESRCMGR->GetClassIconCount() >= nIconNum )
			{
				SCRIPTMGR->GetImage( nIconNum, &m_pClassImg[bigCount], PFT_CLASS_ICON_PATH ) ;	// Ŭ���� ������ �̹����� �Ҵ�޴´�.
			}

			for( int count = 0 ; count < nClassBlank-1 ; ++count )					// Ŭ���� ������ ���̸�ŭ ������ ������.
			{
				classBuf[count] = 0x20 ;											// Ŭ���� ������ ���̸� ��ĭ���� ä���.
			}

			int nLevel = pMember->Get()->nLV ;

			itoa(nLevel, levelBuf, 10) ;											// ��� ������ ���� ���ۿ� ���ڷ� ��ȯ�Ͽ� ��´�.

			//itoa(pMember->Get()->nLV, levelBuf, 12) ;								// ��� ������ ���� ���ۿ� ���ڷ� ��ȯ�Ͽ� ��´�.

			nLevelBlank = strlen(levelBuf) ;										// ���� ������ ���̸� ���Ѵ�.

			for( int count = nLevelBlank ; count < 7 ; ++count )					// ���� ���̷� �ʱⰪ�� ���߰�, ������ ������.
			{
				levelBuf[count] = 0x20 ;											// ������ ������ ��ĭ���� ä���.
			}


			SafeStrCpy(membername, pMember->Get()->szName, MAX_NAME_LENGTH+1) ;		// ��� �̸��� �����Ѵ�.

			nNameBlank = strlen(membername) ;										// ��� �̸��� ���̸� ���Ѵ�.

			for(int count = nNameBlank ; count < MAX_NAME_LENGTH ; ++count)			// �̸� ���̷� �ʱⰪ�� ���߰�, ������ ������.
			{																		
				membername[count] = 0x20 ;											// �̸��� ������ ��ĭ���� ä���.
			}


			sprintf(tempBuf, "%s %s %s", classBuf, levelBuf, membername);			// �ӽ� ���۸� Ŭ���� ������, ����, �̸����� ä���.

			if(pMember->Get()->eConState==CSHGroupMember::MEMBER_CONSTATE_LOGOFF)	// ����� �α��� ���̶��,
			{
				dwColor = RGB_HALF(200, 200, 200) ;									// ȸ������ ������ �����Ѵ�.
			}
			else																	// ����� �α׾ƿ� ���¶��,
			{
				dwColor = RGB_HALF(10, 10, 10) ;									// ���������� ������ �����Ѵ�.				
			}

			m_pListDlg->AddItem( tempBuf, dwColor ) ;								// ��� ����Ʈ�� ������ �����Ѵ�.


			nLogIn = pMember->Get()->eConState ;									// ����� �α��� ���θ� �޴´�.

			if( nLogIn )															// ����� �α��� ���̶��,
			{
				SCRIPTMGR->GetImage( 102, &m_pLongInImg[bigCount], PFT_HARDPATH ) ;	// �α��� �̹����� �����Ѵ�.
			}
			else																	// ����� �α׾ƿ� ���̶��,
			{
				SCRIPTMGR->GetImage( 103, &m_pLongInImg[bigCount], PFT_HARDPATH ) ;	// �α׾ƿ� �̹����� �����Ѵ�.
			}
		}
	}
}

void CFamilyDialog::Render()														// Ŭ���� �̹����� �α��� �̹����� ����� ���� �Լ�.
{
	if( !IsActive() ) return ;														// �йи� â�� Ȱ��ȭ ���� �ƴ϶��, �����Ѵ�.

	cDialog::RenderWindow();														// �����츦 �����Ѵ�.

	cDialog::RenderComponent();														// ������ ���۳�Ʈ���� �����Ѵ�.


	CSHFamily* pFamily = HERO->GetFamily() ;										// �йи� ������ �޾ƿ´�.

	if( pFamily && pFamily->Get()->nMasterID != 0 )									// �йи� ������ ��ȿ�ϰ�, ������ ���̵� 0�� �ƴϸ�,
	{
		CSHFamilyMember* pMember = NULL ;											// ��� ������ ���� �����͸� �����Ѵ�.

		VECTOR2 scale;																// �̹��� �������� ������ ���͸� �����Ѵ�.

		scale.x = 1.0f;																// ������ X�� 1�� �����Ѵ�.
		scale.y = 1.0f;																// ������ Y�� 1�� �����Ѵ�.

		VECTOR2 vPos;																// �̹��� ��ġ�� ������ ���͸� �����Ѵ�.

		DWORD dwColor = 0xffffffff ;												// �̹��� ��¿� ������ ������ �����Ѵ�.

		for( UINT count = 0 ; count < pFamily->Get()->nMemberNum ; ++count )		// ��� ����ŭ ������ ������.
		{
			vPos.y = (float)(GetAbsY() + 156) + (16*count) ;						// �̹��� �����ġ Y�� �����Ѵ�.

			vPos.x = (float)(GetAbsX() + 30) ;										// �̹��� �����ġ X�� �����Ѵ�.

			if( !m_pClassImg[count].IsNull() )										// ��� Ŭ���� �̹����� ���� �ƴ϶��,
			{
				m_pClassImg[count].RenderSprite(&scale, NULL, 0, &vPos, dwColor) ;	// ��� Ŭ���� �̹����� ����Ѵ�.
			}

			vPos.x += 190  ;														// �α��� ��ġ�� ����� �����Ѵ�.

			if( !m_pLongInImg[count].IsNull() )										// ��� �α��� ���� �̹����� ���� �ƴ϶��,
			{
				m_pLongInImg[count].RenderSprite(&scale, NULL, 0, &vPos, dwColor) ;	// ��� �α��� ���� �̹����� ����Ѵ�.
			}
		}
	}

	cImageSelf* pcsFamilyEmblem = HERO->GetFamilyEmblem();
	if (pcsFamilyEmblem)
	{
		VECTOR2 scale, pos2;
		scale.x = 1; scale.y = 1;
		pos2.x = GetAbsX()+210;
		pos2.y = GetAbsY()+66;
		pcsFamilyEmblem->RenderSprite(&scale, NULL, 0 , &pos2, 0xffffffff);	
	}
}

DWORD CFamilyDialog::ActionEvent( CMouse* mouseInfo )								// �йи� â�� �̺�Ʈ�� ó���ϴ� �Լ�.
{
	DWORD we = WE_NULL;																// ������ ������ �̺�Ʈ ������ �����Ѵ�.

	if( !m_bActive )		return we;												// �����찡 Ȱ��ȭ ���°� �ƴϸ�, �̺�Ʈ�� �����Ѵ�.

	we = cDialog::ActionEvent(mouseInfo);											// �⺻ ���̾�α��� �̺�Ʈ�� �޾ƿ´�.

	if(m_pListDlg->PtIdxInRow(mouseInfo->GetMouseX(),mouseInfo->GetMouseY()) != -1)	// ��� ����Ʈ���� ��콺�� ��ġ�� �ִٸ�,
	{
		if(we & WE_LBTNCLICK)														// ���콺 ���� ��ư�� Ŭ���ߴٸ�,
		{ 
			if( WINDOWMGR->IsMouseDownUsed() == FALSE)								// ������ �Ŵ����� ���콺 �ٿ� ����� FALSE�� ���ٸ�,
			{		
				int Idx = m_pListDlg->GetCurSelectedRowIdx();						// ����Ʈ���� ������ ���� �޾ƿ´�.

				if(Idx != -1)														// ������ ���� -1�� ���� �ʴٸ�,
				{
					CSHFamily* pFamily = HERO->GetFamily() ;						// �йи� ������ �޾ƿ´�.

					if( pFamily )													// �йи� ������ ��ȿ���� üũ�Ѵ�.
					{
						CSHFamilyMember* pMember = pFamily->GetMember(Idx) ;		// ���� �� �ε����� ��� ������ �޴´�.

						if( pMember )												// ��� ������ ��ȿ���� üũ�Ѵ�.
						{
							SetMemberList(pFamily) ;								// ��� ����Ʈ�� �ѹ� �����Ѵ�.
							SetMemberInfo( pMember) ;								// ��� ������ �����Ѵ�.

							ITEM* pItem = m_pListDlg->GetItem(Idx) ;				// �־��� �ε����� ������ ������ �޴´�.

							if( pItem )												// ������ ������ ��ȿ���� üũ�Ѵ�.
							{
								pItem->rgb = RGB_HALF(255, 255, 0) ;				// �������� ������ �ٽ� �����Ѵ�.

								m_nCurSelectIdx = Idx ;								// ���� ���� �� ��� �ε����� �����Ѵ�.
							}
						}
					}
				}
			}
		}
	}

	return we;																		// ������ �̺�Ʈ�� ���� ó�� �Ѵ�.
}

void CFamilyDialog::OnActionEvent( LONG lId, void* p, DWORD we )																		// �йи� â�� ��ư�� �̺�Ʈ ó���� �ϴ� �Լ�.
{
	CSHFamily* pFamily = HERO->GetFamily() ;																							// �йи� ������ �޾ƿ´�.

	if( we & WE_BTNCLICK )																												// D���� ��ư�� Ŭ���� �̺�Ʈ�� �Ѿ�Դٸ�,
	{
		switch( lId )																													// �Ѿ�� ��ư�� ���̵� Ȯ���Ѵ�.
		{
		case FMY_CHANGENICKNAME :																										// ȣĪ������ Ŭ���Ͽ��ٸ�, (ȣĪ ������ ��� ����� ����� �� �ִ�.)
			{
				if( pFamily )																											// �йи� ������ ��ȿ���� üũ�Ѵ�.
				{
					CSHFamilyMember* pMember = pFamily->GetMember(m_nCurSelectIdx) ;													// ������ ��� ������ �޾ƿ´�.

					if( pMember )																										// ��� ������ ��ȿ���� üũ�Ѵ�.
					{
						CFamilyNickNameDialog* pDlg = GAMEIN->GetFamilyNickNameDlg() ;													// ȣĪ ����â ������ �޴´�.

						if( pDlg )																										// ȣĪ ����â ������ ��ȿ���� üũ�Ѵ�.
						{
							pDlg->SetActive(TRUE) ;																						// ȣĪ ����â�� Ȱ��ȭ ��Ų��.
						}
					}
				}
			}
			break ;

		case FMY_INVITE :																												// �ʴ��û�� Ŭ���Ͽ��ٸ�,
			{
				if( pFamily && pFamily->Get()->nMasterID == HERO->GetID() )																// �йи� ������ ��ȿ�ϰ�, HERO�� ������ �̸�,
				{
					CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetSelectedObject() ;														// ������ �÷��̾� ������ �޴´�.

					if( pPlayer )																										// �÷��̾� ������ ��ȿ���� üũ�Ѵ�.
					{
						if( pPlayer->GetObjectKind() == eObjectKind_Player )															// �÷��̾� Ÿ���� �������� üũ�Ѵ�.
						{
							if( pPlayer->GetID() != HERO->GetID() )																		// ������ ������ �������� üũ�Ѵ�.
							{
								CSHFamilyMember* pMember = NULL ;																		// �йи� ��� ������ ���� �����͸� �����Ѵ�.			

								for( int count = 0 ; count < MAX_MEMBER_COUNT ; ++count )												// ����� ���̵�� ���ϱ� ���� for���� ������.
								{
									pMember = pFamily->GetMember(count) ;																// ����� ������ �޴´�.

									if( pMember )																						// ��� ������ ��ȿ���� üũ�Ѵ�.
									{
										if( pMember->Get()->nID != pPlayer->GetID() )													// ����� ���̵�� ������ ����� ���̵� ���� ������,
										{
											g_csFamilyManager.CLI_RequestInvite(pPlayer) ;												// �ʴ� ��û�� �Ѵ�.
											return ;																					// ���� ó���� �Ѵ�.
										}
										else																							// ������ ����� ���̵�� ����� ���̵� ������,
										{
											WINDOWMGR->MsgBox( MBI_FAMILY_ERROR_NOTARGET, MBT_OK, CHATMGR->GetChatMsg( 1316) );
										}
									}

									pMember = NULL ;																					// ��� ������ ���� �����͸� NULL ó���� �Ѵ�.
								}
							}
							else
							{
								WINDOWMGR->MsgBox( MBI_FAMILY_ERROR_NOTARGET, MBT_OK, CHATMGR->GetChatMsg( 1317 ) );
							}							
						}
						else
						{
							WINDOWMGR->MsgBox( MBI_FAMILY_ERROR_NOTARGET, MBT_OK, CHATMGR->GetChatMsg( 1318 ) );
						}
					}
					else																												// �÷��̾� ������ ��ȿ���� �ʴٸ�,
					{
						WINDOWMGR->MsgBox( MBI_FAMILY_ERROR_NOTARGET, MBT_OK, CHATMGR->GetChatMsg( 1319 ) );
					}
				}
				else
				{
					WINDOWMGR->MsgBox( MBI_FAMILY_ABLE_MASTER, MBT_OK, CHATMGR->GetChatMsg( 732 ) );									// ���� ó���� �Ѵ�.
				}
			}
			break ;

		case FMY_REGISTMARK :																											// ���� ����� Ŭ���Ͽ��ٸ�,
			{
 				GAMEIN->GetFamilyMarkDlg()->SetActive(TRUE);			
//				WINDOWMGR->MsgBox( MBI_FAMILY_ERROR_NOTARGET, MBT_OK, CHATMGR->GetChatMsg(528) );										// ���� ó���� �Ѵ�.
			}
			break ;

		// desc_hseos_�йи�01
		// S �йи� �߰� added by hseos 2007.10.22
		case FMY_LEAVE:
			{
		 		WINDOWMGR->MsgBox( MBI_FAMILY_LEAVE, MBT_YESNO, CHATMGR->GetChatMsg( 1142 ), HERO->GetFamily()->Get()->szName, int((CSHFamilyManager::LEAVE_PENALTY_EXP_DOWN_RATE[CSHFamilyManager::FLK_LEAVE]+0.001)*100), CSHFamilyManager::LEAVE_PENALTY_REJOIN_TIME[CSHFamilyManager::FLK_LEAVE] );
			}
			break;
		case FMY_EXPEL:
			{
				if (HERO->GetID() == HERO->GetFamily()->GetMember(GAMEIN->GetFamilyDlg()->GetSelectedMemberIdx())->Get()->nID)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1163 ));
				}
				else
				{
	 		   		WINDOWMGR->MsgBox( MBI_FAMILY_EXPEL, MBT_YESNO, CHATMGR->GetChatMsg( 1143 ), HERO->GetFamily()->GetMember(GAMEIN->GetFamilyDlg()->GetSelectedMemberIdx())->Get()->szName, int((CSHFamilyManager::LEAVE_PENALTY_EXP_DOWN_RATE[CSHFamilyManager::FLK_EXPEL]+0.001)*100), CSHFamilyManager::LEAVE_PENALTY_REJOIN_TIME[CSHFamilyManager::FLK_EXPEL] );
				}
			}
			break;
		case FMY_FARM_MANAGE:
			{
				GAMEIN->GetFarmManageDlg()->SetActive( TRUE );
			}
			break;
		// E �йи� �߰� added by hseos 2007.10.22
		}
	}
}

//void CFamilyDialog::LoadClassIcon() 												// Ŭ���� �������� �ε��ϴ� �Լ�.
//{
//	CMHFile fp;																		// ���� ������ �����Ѵ�.
//
//	fp.Init( FILE_IMAGE_CLASS_ICON_PATH, "rb" ) ;									// Ŭ���� ������ ������ �о���δ�.
//
//	int nNull = 0 ;																	// �ǹ̾��� ���� ���� ������ �����Ѵ�.
//
//	while(TRUE)																		// while���� ������.
//	{
//		if( fp.IsEOF() != FALSE ) break ;											// ������ ���� �����ϸ�, break.
//
//		IDINDEX* pIdIdx = new IDINDEX ;												// �ε����� Ŭ���� �ε����� ���� �޸𸮸� �Ҵ��Ѵ�.
//
//		pIdIdx->nIdx = fp.GetInt() ;												// ���Ϸ� ���� �ε����� �޴´�.
//
//		pIdIdx->nClassIdx = fp.GetInt() ;											// ���Ϸ� ���� Ŭ���� �ε����� �޴´�.
//
//		m_ClassIconList.AddTail(pIdIdx) ;											// Ŭ���� ������ ����Ʈ�� �߰��Ѵ�.
//
//		for( int count = 0 ; count < 5 ; ++count )									// ������ ���� ���� �� ��ŭ for���� ������.
//		{
//			nNull = fp.GetInt() ;													// ���⼭ ������� �ʴ� ���� �׳� �б⸸ �Ѵ�.
//		}
//	}
//
//	fp.Release() ;																	// Ŭ���� ������ ������ �����Ѵ�.
//}
//
//void CFamilyDialog::ReleaseIconList()												// Ŭ���� ������ ����Ʈ�� �����Ѵ�.
//{
//	IDINDEX* pIdIdx = NULL ;														// �ε����� Ŭ���� �ε��� ������ ���� �����͸� �����Ѵ�.
//
//	PTRLISTPOS pos	= NULL ;														// ����Ʈ���� ��ġ ������ ���� �����͸� �����Ѵ�.
//
//	pos = m_ClassIconList.GetHeadPosition() ;										// Ŭ���� ������ ����Ʈ�� �ش��� �����Ѵ�.
//
//	while(pos)																		// ��ġ������ �ִµ���, while�� ������.
//	{
//		pIdIdx = (IDINDEX*)m_ClassIconList.GetNext(pos) ;							// pos�� �ش��ϴ� �ε���, Ŭ���� �ε��� ������ �޴´�.
//
//		if( pIdIdx )																// ���� ������ ��ȿ���� üũ�Ѵ�.
//		{
//			delete pIdIdx ;															// ���� �ּ��� �޸𸮸� �����Ѵ�.
//
//			pIdIdx = NULL ;															// �ּҸ� ����Ű�� �����͸� NULL ó�� �Ѵ�.
//		}
//	}
//
//	m_ClassIconList.RemoveAll() ;													// ����Ʈ�� ��� ����.
//}

//int CFamilyDialog::GetClassIconNum(int nClass)
//{
//	IDINDEX* pIdIdx = NULL ;														// �ε����� Ŭ���� �ε��� ������ ���� �����͸� �����Ѵ�.
//
//	PTRLISTPOS pos	= NULL ;														// ����Ʈ���� ��ġ ������ ���� �����͸� �����Ѵ�.
//
//	pos = m_ClassIconList.GetHeadPosition() ;										// Ŭ���� ������ ����Ʈ�� �ش��� �����Ѵ�.
//
//	while(pos)																		// ��ġ������ �ִµ���, while�� ������.
//	{
//		pIdIdx = (IDINDEX*)m_ClassIconList.GetNext(pos) ;							// pos�� �ش��ϴ� �ε���, Ŭ���� �ε��� ������ �޴´�.
//
//		if( pIdIdx )																// ���� ������ ��ȿ���� üũ�Ѵ�.
//		{
//			if( pIdIdx->nClassIdx == nClass )										// ���ڷ� �Ѿ�� Ŭ���� �ε�����, ����Ʈ�� Ŭ���� �ε����� ���ٸ�,
//			{
//				return pIdIdx->nIdx ;												// �ε����� �����Ѵ�.
//			}
//		}
//	}
//
//	return -1 ;																		// ���� Ŭ���� �ε����� ������� -1�� �����Ѵ�.
//}


//void CFamilyDialog::SetInfo(char* guildname, BYTE guildlvl, char* mastername, BYTE membernum, BYTE location)
//{
//	/*m_pFamilyName->SetStaticText(guildname);
//	m_pFamilyLvl->SetStaticValue(guildlvl);*/
//}
//
//void CFamilyDialog::DeleteMemberAll()
//{	
//	m_pListDlg->RemoveAll();
//	m_MemberList.clear();//Add 060803 by wonju
//	
//}
//
//void CFamilyDialog::ResetMemberInfo(FAMILYMEMBERINFO * pInfo)
//{
//	char buf[64] = {" ",};
//	char rank[8] = {0,};
//	int namelen;
//	int ranklen;
//	char membername[MAX_NAME_LENGTH+1];
//	char memberrank[7];
//	m_MemberList.push_back(*pInfo);//�������� ���� ������ �ִ´�.//Add 060803 by wonju
//	SafeStrCpy(membername, pInfo->MemberName, MAX_NAME_LENGTH+1);
//	namelen = strlen(pInfo->MemberName);
//	for(int i=namelen; i<MAX_NAME_LENGTH; ++i)
//		membername[i] = 0x20;
//	
//	SafeStrCpy(memberrank, FAMILYMGR->GetRankName(pInfo->Rank), 7);
//	ranklen = strlen(memberrank);
//	for(i=ranklen; i<6; ++i)
//		memberrank[i] = 0x20;
//	sprintf(buf, "%s %10s %4d", membername, memberrank, pInfo->Memberlvl);
//	if(pInfo->bLogged == TRUE)
//		m_pListDlg->AddItem(buf, 0xffffffff);
//	else
//		m_pListDlg->AddItem(buf, RGBA_MAKE(172,182,199,255));
//}
//void CFamilyDialog::SetFamilyInfo( char* FamilyName, char* MasterName, char* MapName, BYTE FamilyLevel, BYTE MemberNum, char* pUnionName)
//{
//	/*m_pFamilyName->SetStaticText(FamilyName);		
//	m_pFamilyLvl->SetStaticValue(FamilyLevel);
//	m_pMasterName->SetStaticText(MasterName);
//	m_pMemberNum->SetStaticValue(MemberNum);
//	m_pLocation->SetStaticText(MapName);
//	m_pUnionName->SetStaticText(pUnionName);*/
//}
//
////void CFamilyDialog::SetActive(BOOL val)
////{
////	if(val == TRUE)
////	{
////		ShowFamilyMode(eFamilyMode_FamilyInfo);
////	}
////	if(val == FALSE)
////	{
////		cMsgBox* pSece =  (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_FAMILY_SECEDE);
////		if(pSece)
////			WINDOWMGR->AddListDestroyWindow( pSece );
////		cMsgBox* pBan =  (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_FAMILY_BAN);
////		if(pBan)
////			WINDOWMGR->AddListDestroyWindow( pBan );
//////		if(GAMEIN->GetFamilyRankDlg()->IsActive() == TRUE)
//////			GAMEIN->GetFamilyRankDlg()->SetActive(FALSE);
////	}
////	cDialog::SetActive(val);
////}
//
//void CFamilyDialog::RefreshFamilyInfo(CFamily* pFamily)
//{
//	/*m_pFamilyName->SetStaticText(pFamily->GetFamilyName());		
//	m_pFamilyLvl->SetStaticValue(pFamily->GetLevel());
//	m_pMasterName->SetStaticText(pFamily->GetMasterName());
//	m_pMemberNum->SetStaticValue(pFamily->GetMemberNum());
//	m_pLocation->SetStaticText(GetMapName(pFamily->GetLocation()));
//	m_pUnionName->SetStaticText(FAMILYUNION->GetFamilyUnionName());*/
//}
//
//void CFamilyDialog::SetFamilyUnionName()
//{
//	//m_pUnionName->SetStaticText(FAMILYUNION->GetFamilyUnionName());
//}
//
////DWORD CFamilyDialog::ActionEvent(CMouse* mouseInfo)
////{
////	/*DWORD we = WE_NULL;
////	if( !m_bActive )		return we;
////
////	we = cDialog::ActionEvent(mouseInfo);
////
////	if(m_pListDlg->PtIdxInRow(mouseInfo->GetMouseX(),mouseInfo->GetMouseY()) != -1)
////	{
////		if(we & WE_LBTNCLICK)
////		{
////			if( WINDOWMGR->IsMouseDownUsed() == FALSE)
////			{		
////				int Idx = m_pListDlg->GetCurSelectedRowIdx();
////				if(Idx != -1)
////				{
////					FAMILYMGR->SetSelectedMemberNum(Idx);
////					if(GAMEIN->GetFamilyRankDlg()->IsActive() == TRUE)
////						GAMEIN->GetFamilyRankDlg()->SetActive(FALSE);
////					if(GAMEIN->GetFamilyNickNameDlg()->IsActive() == TRUE)
////						GAMEIN->GetFamilyNickNameDlg()->SetActive(FALSE);
////					cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_FAMILY_BAN);
////					if(pMsgBox)
////						WINDOWMGR->AddListDestroyWindow( pMsgBox );
////				}
////			}
////		}
////	}
////	return we;*/
////
////	DWORD we = WE_NULL ;
////
////	return we ;
////}
//
//void CFamilyDialog::ShowFamilyMode(BYTE ShowMode)
//{
//	/*SetFamilyPushupBtn(ShowMode);
//	if(m_CurFamilyMode == ShowMode)	return;
//
//	SetActiveFamilyMode(ShowMode,TRUE);
//	m_CurFamilyMode = ShowMode;*/
//
////	if(m_CurFamilyMode  != -1)
////	{
////		ClearDisableBtn();
////		SetActiveFamilyMode(m_CurFamilyMode ,FALSE);
////	}
////
////	SetActiveFamilyMode(ShowMode,TRUE);
////	m_CurFamilyMode = ShowMode;
//
////	SetDisableFuncBtn(HERO->GetFamilyMemberRank());
//}
//
//void CFamilyDialog::SetActiveFamilyMode(int showmode,BOOL bActive)
//{
//	//if( showmode == eFamilyMode_FamilyInfo )
//	//{
//	//	for( int i = 0; i < 12; ++i )
//	//		m_pFamilyStatic[i]->SetActive( TRUE );
//	//	for( i = 0; i < 13; ++i )
//	//	{
//	//		if( m_pFamilyBtn[i] )
//	//		{
//	//			m_pFamilyBtn[i]->SetDisable( FALSE );
//	//			m_pFamilyBtn[i]->SetActive( FALSE );
//	//		}
//	//	}		
//	//}
//	//else if( showmode == eFamilyMode_Func )
//	//{
//	//	for( int i = 0; i < 12; ++i )
//	//		m_pFamilyStatic[i]->SetActive( FALSE );
//
//	//	for( i = 0; i < 13; ++i )
//	//	{
//	//		if( m_pFamilyBtn[i] )
//	//		{
//	//			m_pFamilyBtn[i]->SetActive( TRUE );
//	//			m_pFamilyBtn[i]->SetDisable( TRUE );									
//	//		}
//	//	}
//	//	BYTE bRank = HERO->GetFamilyMemberRank();
//	//	switch( bRank )
//	//	{
//	//	case FAMILY_MASTER:
//	//		{
//	//			m_pFamilyBtn[0]->SetDisable( FALSE );
//	//			m_pFamilyBtn[1]->SetDisable( FALSE );
//	//			m_pFamilyBtn[2]->SetDisable( FALSE );
//	//			m_pFamilyBtn[3]->SetDisable( FALSE );
//	//			// 06. 03. ���İ��� - �̿���
//	//			m_pFamilyBtn[5]->SetDisable( FALSE );
//	//			
//	//			if( FAMILYUNION->GetFamilyUnionIdx() )
//	//			{
//	//				if( FAMILYUNION->IsFamilyUnionMaster( HERO ) )
//	//				{
//	//					m_pFamilyBtn[6]->SetDisable( FALSE );
//	//					m_pFamilyBtn[8]->SetDisable( FALSE );
//	//				}
//	//				else
//	//					m_pFamilyBtn[7]->SetDisable( FALSE );
//	//			}					
//	//			m_pFamilyBtn[9]->SetDisable( FALSE );
//	//			m_pFamilyBtn[10]->SetDisable( FALSE );
//	//			m_pFamilyBtn[11]->SetDisable( FALSE );
//	//			m_pFamilyBtn[12]->SetDisable( FALSE );
//	//		}
//	//		break;
//	//	case FAMILY_VICEMASTER:
//	//		{
//	//			m_pFamilyBtn[3]->SetDisable( FALSE );
//	//			m_pFamilyBtn[4]->SetDisable( FALSE );
//	//			// 06. 03. ���İ��� - �̿���
//	//			m_pFamilyBtn[5]->SetDisable( FALSE );
//	//			m_pFamilyBtn[12]->SetDisable( FALSE );
//	//		}
//	//		break;
//	//	case FAMILY_SENIOR:
//	//		{
//	//			m_pFamilyBtn[4]->SetDisable( FALSE );
//	//			m_pFamilyBtn[12]->SetDisable( FALSE );
//	//		}
//	//		break;
//	//	case FAMILY_MEMBER:
//	//	case FAMILY_STUDENT:
//	//		{
//	//			m_pFamilyBtn[4]->SetDisable( FALSE );
//	//		}
//	//		break;
//	//	}
//	//}
////	PTRLISTSEARCHSTART(m_FamilyModeArray[showmode],cWindow*,pWin)
////		pWin->SetActive(bActive);
////	PTRLISTSEARCHEND
//}
//
//void CFamilyDialog::SetFamilyPushupBtn(BYTE ShowMode)
//{
//	/*if((ShowMode < 0) || (ShowMode > 2))
//	{
//		ASSERT(0);
//		return;
//	}
//	for(int i=0; i<2; ++i)
//		m_pFamilyPushupBtn[i]->SetPush(FALSE);
//	m_pFamilyPushupBtn[ShowMode]->SetPush(TRUE);*/
//}
//
//void CFamilyDialog::ClearDisableBtn()
//{
////	PTRLISTSEARCHSTART(m_FamilyModeArray[eFamilyMode_Func],cWindow*,pWin)
////		pWin->SetDisable(FALSE);
////	PTRLISTSEARCHEND
//}
//
//void CFamilyDialog::SetDisableFuncBtn(BYTE Rank)
//{
///*	int i = 0;
//	switch(Rank)
//	{
//	case FAMILY_MASTER:	i = eInvailedFunc;	break;
//	case FAMILY_VICEMASTER:	i = eMasterFunc;	break;
//	case FAMILY_SENIOR:	i = eViceMasterFunc;	break;
//	case FAMILY_MEMBER:	i = eSeniorFunc;	break;
//	}
//
//	for(int j=0; j<i; ++j)
//	{		
//		PTRLISTSEARCHSTART(m_FamilyFuncCtrlArray[j],cWindow*,pWin)
//			pWin->SetDisable(FALSE);
//		PTRLISTSEARCHEND
//	}
//	for(j=i; j<eMaxFunc; ++j)
//	{		
//		PTRLISTSEARCHSTART(m_FamilyFuncCtrlArray[j],cWindow*,pWin)
//			pWin->SetDisable(TRUE);
//		PTRLISTSEARCHEND
//	}
//	if(Rank == FAMILY_MASTER)
//	{
//		m_pSecedeBtn->SetDisable(TRUE);
//	}
//	*/
//}
//
//void CFamilyDialog::SetFamilyPosition( char* MapName )
//{
//	//m_pLocation->SetStaticText( MapName );
//}
//
//void CFamilyDialog::SortMemberListbyPosition()//Add 060803 by wonju
//{
//	if(m_nPositionFlag)
//	{
//		std::list<FAMILYMEMBERINFO> ChangeList;
//		std::list<FAMILYMEMBERINFO>::iterator iter;
//		//�������� ���� - ���� ���� �ڷ� ����.
//		std::vector<FAMILYMEMBERINFO>* prevList = &m_MemberList;
//
//		for(unsigned int i = 0; i < prevList->size(); ++i)
//		{
//			FAMILYMEMBERINFO curmemberinfo = (*prevList)[i]; //�˻��� �ɹ�
//
//			if(ChangeList.size() > 0)
//			{
//				bool bEnd = TRUE;
//
//				for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//���� �ٲ� ����Ʈ�� �˻�
//				{
//					BYTE Rank = iter->Rank;//�ٲ� ����Ʈ ���� ��
//
//				    if(curmemberinfo.Rank > Rank) //���� ����� ���� ����Ʈ ���� ������ ũ�ٸ�
//					{
//						//�����ϰ� ������.
//                        ChangeList.insert(iter,curmemberinfo);
//						bEnd = FALSE;
//						break;
//					}
//				}
//
//				//������ ������ �������� ���� �ڿ� �����Ѵ�.
//				if(TRUE == bEnd)
//                ChangeList.push_back(curmemberinfo);
//
//			}
//			else
//			{
//				ChangeList.push_back(curmemberinfo);
//			}
//		}
//
//		//���� ����Ʈ�� �׸��� �� �����.
//		m_MemberList.clear();
//		m_pListDlg->RemoveAll();
//
//		for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//�ٲ��׸��� ���� �ٽ� �ִ´�.
//		{
//			FAMILYMEMBERINFO info = *iter;
//			ResetMemberInfo(&info);
//			
//		}
//
//
//		
//	}
//	else
//	{
//		std::list<FAMILYMEMBERINFO> ChangeList;
//		std::list<FAMILYMEMBERINFO>::iterator iter;
//		//�������� ���� - ���� ���� �ڷ� ����.
//		std::vector<FAMILYMEMBERINFO>* prevList = &m_MemberList;
//
//		unsigned int size = prevList->size();
//
//		for(unsigned int i = 0; i < prevList->size(); ++i)
//		{
//			FAMILYMEMBERINFO curmemberinfo = (*prevList)[i]; //�˻��� �ɹ�
//
//			if(ChangeList.size() > 0)
//			{
//				bool bEnd = TRUE;
//
//				for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//���� �ٲ� ����Ʈ�� �˻�
//				{
//					BYTE Rank = iter->Rank;//�ٲ� ����Ʈ ���� ��
//
//					if(curmemberinfo.Rank < Rank) //���� ����� ���� ����Ʈ ���� ������ ũ�ٸ�
//					{
//						//�����ϰ� ������.
//						ChangeList.insert(iter,curmemberinfo);
//						bEnd = FALSE;
//						break;
//					}
//				}
//
//				//������ ������ �������� ���� �ڿ� �����Ѵ�.
//				if(TRUE == bEnd)
//					ChangeList.push_back(curmemberinfo);
//
//			}
//			else
//			{
//				ChangeList.push_back(curmemberinfo);
//			}
//		}
//
//		//���� ����Ʈ�� �׸��� �� �����.
//		m_MemberList.clear();
//		m_pListDlg->RemoveAll();
//
//		for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//�ٲ��׸��� ���� �ٽ� �ִ´�.
//		{
//			FAMILYMEMBERINFO info = *iter;
//			ResetMemberInfo(&info);
//			
//		}
//	}
//
//	m_nPositionFlag ^= 1;
//}
//
//void CFamilyDialog::SortMemberListbyLevel()//Add 060803 by wonju
//{
//	if(m_nLevelFlag)
//	{
//		std::list<FAMILYMEMBERINFO> ChangeList;
//		std::list<FAMILYMEMBERINFO>::iterator iter;
//		//�������� ���� - ���� ���� �ڷ� ����.
//		std::vector<FAMILYMEMBERINFO>* prevList = &m_MemberList;
//
//		for(unsigned int i = 0; i < prevList->size(); ++i)
//		{
//			FAMILYMEMBERINFO curmemberinfo = (*prevList)[i]; //�˻��� �ɹ�
//
//			if(ChangeList.size() > 0)
//			{
//				bool bEnd = TRUE;
//
//				for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//���� �ٲ� ����Ʈ�� �˻�
//				{
//					WORD lvl = iter->Memberlvl;//�ٲ� ����Ʈ ���� ��
//
//					if(curmemberinfo.Memberlvl > lvl) //���� ����� ���� ����Ʈ ���� ������ ũ�ٸ�
//					{
//						//�����ϰ� ������.
//						ChangeList.insert(iter,curmemberinfo);
//						bEnd = FALSE;
//						break;
//					}
//				}
//
//				//������ ������ �������� ���� �ڿ� �����Ѵ�.
//				if(TRUE == bEnd)
//					ChangeList.push_back(curmemberinfo);
//
//			}
//			else
//			{
//				ChangeList.push_back(curmemberinfo);
//			}
//		}
//
//		//���� ����Ʈ�� �׸��� �� �����.
//		m_MemberList.clear();
//		m_pListDlg->RemoveAll();
//
//		for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//�ٲ��׸��� ���� �ٽ� �ִ´�.
//		{
//			FAMILYMEMBERINFO info = *iter;
//			ResetMemberInfo(&info);
//
//		}
//		
//
//		
//	}
//	else
//	{
//		std::list<FAMILYMEMBERINFO> ChangeList;
//		std::list<FAMILYMEMBERINFO>::iterator iter;
//		//�������� ���� - ���� ���� �ڷ� ����.
//		std::vector<FAMILYMEMBERINFO>* prevList = &m_MemberList;
//
//		for(unsigned int i = 0; i < prevList->size(); ++i)
//		{
//			FAMILYMEMBERINFO curmemberinfo = (*prevList)[i]; //�˻��� �ɹ�
//
//			if(ChangeList.size() > 0)
//			{
//				bool bEnd = TRUE;
//
//				for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//���� �ٲ� ����Ʈ�� �˻�
//				{
//					WORD lvl = iter->Memberlvl;//�ٲ� ����Ʈ ���� ��
//
//					if(curmemberinfo.Memberlvl < lvl) //���� ����� ���� ����Ʈ ���� ������ ũ�ٸ�
//					{
//						//�����ϰ� ������.
//						ChangeList.insert(iter,curmemberinfo);
//						bEnd = FALSE;
//						break;
//					}
//				}
//
//				//������ ������ �������� ���� �ڿ� �����Ѵ�.
//				if(TRUE == bEnd)
//					ChangeList.push_back(curmemberinfo);
//
//			}
//			else
//			{
//				ChangeList.push_back(curmemberinfo);
//			}
//		}
//
//		//���� ����Ʈ�� �׸��� �� �����.
//		m_MemberList.clear();
//		m_pListDlg->RemoveAll();
//
//		for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//�ٲ��׸��� ���� �ٽ� �ִ´�.
//		{
//			FAMILYMEMBERINFO info = *iter;
//			ResetMemberInfo(&info);
//
//		}
//	}
//
//	m_nLevelFlag ^= 1;
//
//}





//// FamilyDialog.cpp: implementation of the CFamilyDialog class.
////
////////////////////////////////////////////////////////////////////////
//
//#include "stdafx.h"
//#include "FamilyDialog.h"
//#include "./Interface/cWindowManager.h"
//#include "./interface/cStatic.h"
//#include "./interface/cComboBox.h"
//#include "./interface/cListDialog.h"
//#include "./interface/cPushupButton.h"
//#include "WindowIDEnum.h"
//#include "ObjectManager.h"
//#include "FamilyManager.h"
//#include "ChatManager.h"
//#include "../Interface/cResourceManager.h"
//#include "cMsgBox.h"
//#include "Family.h"
//#include "../Input/Mouse.h"
//#include "GameIn.h"
//#include "FamilyRankDialog.h"
//#include "FamilyNickNameDialog.h"
//#include "FamilyUnion.h"
////////////////////////////////////////////////////////////////////////
//// Construction/Destruction
////////////////////////////////////////////////////////////////////////
//
//CFamilyDialog::CFamilyDialog()
//{
//	m_type = WT_FAMILYDLG;
//	m_CurFamilyMode = -1;
//	m_nPositionFlag = 1;
//	m_nLevelFlag = 1;
//	
//}
//
//CFamilyDialog::~CFamilyDialog()
//{
///*	for(int n=0;n<eMaxFunc;++n)
//	{
//		m_FamilyFuncCtrlArray[n].RemoveAll();
//	}
//	for(n=0; n<eFamilyMode_Max; ++n)
//	{
//		m_FamilyModeArray[n].RemoveAll();
//	}
//	*/
//	
//}
//
//void CFamilyDialog::Linking()
//{
//	m_pFamilyName = (cStatic*)GetWindowForID(FD_NAME);
//	m_pFamilyLvl = (cStatic*)GetWindowForID(FD_LEVEL);
//	m_pMasterName = (cStatic*)GetWindowForID(FD_MASTER);
//	m_pMemberNum = (cStatic*)GetWindowForID(FD_MEMBERNUM);
//	m_pLocation = (cStatic*)GetWindowForID(FD_LOCATION);
//	m_pUnionName = (cStatic*)GetWindowForID(FD_UNIONNAME);
//	m_pListDlg = (cListDialog*)GetWindowForID(FD_MEMBERLIST);	
//	m_pListDlg->SetShowSelect(TRUE);	
//	m_pListDlg->SetHeight(158);
//
//	for( int i = 0; i < 12; ++i )
//		m_pFamilyStatic[i] = (cStatic*)GetWindowForID(FD_NAME+i);
//	
//	for( i = 0; i < 2; ++i )
//		m_pFamilyPushupBtn[i] = (cPushupButton*)GetWindowForID(FD_MEMBERINFOPUSHUPBTN+i);
//	m_pRelationBtn = (cButton*)GetWindowForID( FD_RELATIONBTN );
//	
//	for( i = 0; i < 13; ++i )
//		m_pFamilyBtn[i] = (cButton*)GetWindowForID(FD_BANMEMBER+i);
//
//
///*	for(i=0; i<10; ++i)
//	{
//		m_FamilyModeArray[eFamilyMode_FamilyInfo].AddTail((cStatic*)GetWindowForID(GD_NAME+i));
//	}
//	for(i=0; i<12; ++i)
//	{
//		m_FamilyModeArray[eFamilyMode_Func].AddTail((cButton*)GetWindowForID(GD_BANMEMBER+i));
//	}
//	for(i=0; i<4; ++i)
//		m_FamilyFuncCtrlArray[eInvailedFunc].AddTail((cButton*)GetWindowForID(GD_INVITEUNION+i));
//*/
//
///*	m_pBanMemberBtn = (cButton*)GetWindowForID(GD_BANMEMBER);
//	m_pGiveMemberNickBtn = (cButton*)GetWindowForID(GD_GIVEMEMBERNICK);
//	m_pGiveMemberRankBtn = (cButton*)GetWindowForID(GD_GIVEMEMBERRANK);
//	m_pInviteMemberBtn = (cButton*)GetWindowForID(GD_INVITEMEMBER);
//	m_pInviteUnionBtn = (cButton*)GetWindowForID(GD_INVITEUNION);
//	m_pSecedeBtn = (cButton*)GetWindowForID(GD_SECEDE);
//	m_pDeclareBtn = (cButton*)GetWindowForID(GD_DECLARE);
//	m_pSuggestBtn = (cButton*)GetWindowForID(GD_SUGGEST);
//	m_pSurrenderBtn = (cButton*)GetWindowForID(GD_SURRENDER);
//
//	m_FamilyFuncCtrlArray[eMasterFunc].AddTail(m_pInviteUnionBtn);
//	m_FamilyFuncCtrlArray[eMasterFunc].AddTail(m_pGiveMemberRankBtn);
//	m_FamilyFuncCtrlArray[eMasterFunc].AddTail(m_pGiveMemberNickBtn);
//	m_FamilyFuncCtrlArray[eMasterFunc].AddTail(m_pBanMemberBtn);
//	m_FamilyFuncCtrlArray[eViceMasterFunc].AddTail(m_pInviteMemberBtn);
//	m_FamilyFuncCtrlArray[eMemberFunc].AddTail(m_pSecedeBtn);
//	m_FamilyFuncCtrlArray[eMasterFunc].AddTail(m_pDeclareBtn);
//	m_FamilyFuncCtrlArray[eMasterFunc].AddTail(m_pSuggestBtn);
//	m_FamilyFuncCtrlArray[eMasterFunc].AddTail(m_pSurrenderBtn);	
//*/	
//}
//
//void CFamilyDialog::SetInfo(char* guildname, BYTE guildlvl, char* mastername, BYTE membernum, BYTE location)
//{
//	m_pFamilyName->SetStaticText(guildname);
//	m_pFamilyLvl->SetStaticValue(guildlvl);
//}
//
//void CFamilyDialog::DeleteMemberAll()
//{	
//	m_pListDlg->RemoveAll();
//	m_MemberList.clear();//Add 060803 by wonju
//	
//}
//
//void CFamilyDialog::ResetMemberInfo(FAMILYMEMBERINFO * pInfo)
//{
//	char buf[64] = {" ",};
//	char rank[8] = {0,};
//	int namelen;
//	int ranklen;
//	char membername[MAX_NAME_LENGTH+1];
//	char memberrank[7];
//	m_MemberList.push_back(*pInfo);//�������� ���� ������ �ִ´�.//Add 060803 by wonju
//	SafeStrCpy(membername, pInfo->MemberName, MAX_NAME_LENGTH+1);
//	namelen = strlen(pInfo->MemberName);
//	for(int i=namelen; i<MAX_NAME_LENGTH; ++i)
//		membername[i] = 0x20;
//	
//	SafeStrCpy(memberrank, FAMILYMGR->GetRankName(pInfo->Rank), 7);
//	ranklen = strlen(memberrank);
//	for(i=ranklen; i<6; ++i)
//		memberrank[i] = 0x20;
//	sprintf(buf, "%s %10s %4d", membername, memberrank, pInfo->Memberlvl);
//	if(pInfo->bLogged == TRUE)
//		m_pListDlg->AddItem(buf, 0xffffffff);
//	else
//		m_pListDlg->AddItem(buf, RGBA_MAKE(172,182,199,255));
//}
//void CFamilyDialog::SetFamilyInfo( char* FamilyName, char* MasterName, char* MapName, BYTE FamilyLevel, BYTE MemberNum, char* pUnionName)
//{
//	m_pFamilyName->SetStaticText(FamilyName);		
//	m_pFamilyLvl->SetStaticValue(FamilyLevel);
//	m_pMasterName->SetStaticText(MasterName);
//	m_pMemberNum->SetStaticValue(MemberNum);
//	m_pLocation->SetStaticText(MapName);
//	m_pUnionName->SetStaticText(pUnionName);
//}
//
//void CFamilyDialog::SetActive(BOOL val)
//{
//	if(val == TRUE)
//	{
//		ShowFamilyMode(eFamilyMode_FamilyInfo);
//	}
//	if(val == FALSE)
//	{
//		cMsgBox* pSece =  (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_FAMILY_SECEDE);
//		if(pSece)
//			WINDOWMGR->AddListDestroyWindow( pSece );
//		cMsgBox* pBan =  (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_FAMILY_BAN);
//		if(pBan)
//			WINDOWMGR->AddListDestroyWindow( pBan );
////		if(GAMEIN->GetFamilyRankDlg()->IsActive() == TRUE)
////			GAMEIN->GetFamilyRankDlg()->SetActive(FALSE);
//	}
//	cDialog::SetActive(val);
//}
//
//void CFamilyDialog::RefreshFamilyInfo(CFamily* pFamily)
//{
//	m_pFamilyName->SetStaticText(pFamily->GetFamilyName());		
//	m_pFamilyLvl->SetStaticValue(pFamily->GetLevel());
//	m_pMasterName->SetStaticText(pFamily->GetMasterName());
//	m_pMemberNum->SetStaticValue(pFamily->GetMemberNum());
//	m_pLocation->SetStaticText(GetMapName(pFamily->GetLocation()));
//	m_pUnionName->SetStaticText(FAMILYUNION->GetFamilyUnionName());
//}
//
//void CFamilyDialog::SetFamilyUnionName()
//{
//	m_pUnionName->SetStaticText(FAMILYUNION->GetFamilyUnionName());
//}
//
//DWORD CFamilyDialog::ActionEvent(CMouse* mouseInfo)
//{
//	DWORD we = WE_NULL;
//	if( !m_bActive )		return we;
//
//	we = cDialog::ActionEvent(mouseInfo);
//
//	if(m_pListDlg->PtIdxInRow(mouseInfo->GetMouseX(),mouseInfo->GetMouseY()) != -1)
//	{
//		if(we & WE_LBTNCLICK)
//		{
//			if( WINDOWMGR->IsMouseDownUsed() == FALSE)
//			{		
//				int Idx = m_pListDlg->GetCurSelectedRowIdx();
//				if(Idx != -1)
//				{
//					FAMILYMGR->SetSelectedMemberNum(Idx);
//					if(GAMEIN->GetFamilyRankDlg()->IsActive() == TRUE)
//						GAMEIN->GetFamilyRankDlg()->SetActive(FALSE);
//					if(GAMEIN->GetFamilyNickNameDlg()->IsActive() == TRUE)
//						GAMEIN->GetFamilyNickNameDlg()->SetActive(FALSE);
//					cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_FAMILY_BAN);
//					if(pMsgBox)
//						WINDOWMGR->AddListDestroyWindow( pMsgBox );
//				}
//			}
//		}
//	}
//	return we;
//}
//
//void CFamilyDialog::ShowFamilyMode(BYTE ShowMode)
//{
//	SetFamilyPushupBtn(ShowMode);
//	if(m_CurFamilyMode == ShowMode)	return;
//
//	SetActiveFamilyMode(ShowMode,TRUE);
//	m_CurFamilyMode = ShowMode;
//
////	if(m_CurFamilyMode  != -1)
////	{
////		ClearDisableBtn();
////		SetActiveFamilyMode(m_CurFamilyMode ,FALSE);
////	}
////
////	SetActiveFamilyMode(ShowMode,TRUE);
////	m_CurFamilyMode = ShowMode;
//
////	SetDisableFuncBtn(HERO->GetFamilyMemberRank());
//}
//
//void CFamilyDialog::SetActiveFamilyMode(int showmode,BOOL bActive)
//{
//	if( showmode == eFamilyMode_FamilyInfo )
//	{
//		for( int i = 0; i < 12; ++i )
//			m_pFamilyStatic[i]->SetActive( TRUE );
//		for( i = 0; i < 13; ++i )
//		{
//			if( m_pFamilyBtn[i] )
//			{
//				m_pFamilyBtn[i]->SetDisable( FALSE );
//				m_pFamilyBtn[i]->SetActive( FALSE );
//			}
//		}		
//	}
//	else if( showmode == eFamilyMode_Func )
//	{
//		for( int i = 0; i < 12; ++i )
//			m_pFamilyStatic[i]->SetActive( FALSE );
//
//		for( i = 0; i < 13; ++i )
//		{
//			if( m_pFamilyBtn[i] )
//			{
//				m_pFamilyBtn[i]->SetActive( TRUE );
//				m_pFamilyBtn[i]->SetDisable( TRUE );									
//			}
//		}
//		BYTE bRank = HERO->GetFamilyMemberRank();
//		switch( bRank )
//		{
//		case FAMILY_MASTER:
//			{
//				m_pFamilyBtn[0]->SetDisable( FALSE );
//				m_pFamilyBtn[1]->SetDisable( FALSE );
//				m_pFamilyBtn[2]->SetDisable( FALSE );
//				m_pFamilyBtn[3]->SetDisable( FALSE );
//				// 06. 03. ���İ��� - �̿���
//				m_pFamilyBtn[5]->SetDisable( FALSE );
//				
//				if( FAMILYUNION->GetFamilyUnionIdx() )
//				{
//					if( FAMILYUNION->IsFamilyUnionMaster( HERO ) )
//					{
//						m_pFamilyBtn[6]->SetDisable( FALSE );
//						m_pFamilyBtn[8]->SetDisable( FALSE );
//					}
//					else
//						m_pFamilyBtn[7]->SetDisable( FALSE );
//				}					
//				m_pFamilyBtn[9]->SetDisable( FALSE );
//				m_pFamilyBtn[10]->SetDisable( FALSE );
//				m_pFamilyBtn[11]->SetDisable( FALSE );
//				m_pFamilyBtn[12]->SetDisable( FALSE );
//			}
//			break;
//		case FAMILY_VICEMASTER:
//			{
//				m_pFamilyBtn[3]->SetDisable( FALSE );
//				m_pFamilyBtn[4]->SetDisable( FALSE );
//				// 06. 03. ���İ��� - �̿���
//				m_pFamilyBtn[5]->SetDisable( FALSE );
//				m_pFamilyBtn[12]->SetDisable( FALSE );
//			}
//			break;
//		case FAMILY_SENIOR:
//			{
//				m_pFamilyBtn[4]->SetDisable( FALSE );
//				m_pFamilyBtn[12]->SetDisable( FALSE );
//			}
//			break;
//		case FAMILY_MEMBER:
//		case FAMILY_STUDENT:
//			{
//				m_pFamilyBtn[4]->SetDisable( FALSE );
//			}
//			break;
//		}
//	}
////	PTRLISTSEARCHSTART(m_FamilyModeArray[showmode],cWindow*,pWin)
////		pWin->SetActive(bActive);
////	PTRLISTSEARCHEND
//}
//
//void CFamilyDialog::SetFamilyPushupBtn(BYTE ShowMode)
//{
//	if((ShowMode < 0) || (ShowMode > 2))
//	{
//		ASSERT(0);
//		return;
//	}
//	for(int i=0; i<2; ++i)
//		m_pFamilyPushupBtn[i]->SetPush(FALSE);
//	m_pFamilyPushupBtn[ShowMode]->SetPush(TRUE);
//}
//
//void CFamilyDialog::ClearDisableBtn()
//{
////	PTRLISTSEARCHSTART(m_FamilyModeArray[eFamilyMode_Func],cWindow*,pWin)
////		pWin->SetDisable(FALSE);
////	PTRLISTSEARCHEND
//}
//
//void CFamilyDialog::SetDisableFuncBtn(BYTE Rank)
//{
///*	int i = 0;
//	switch(Rank)
//	{
//	case FAMILY_MASTER:	i = eInvailedFunc;	break;
//	case FAMILY_VICEMASTER:	i = eMasterFunc;	break;
//	case FAMILY_SENIOR:	i = eViceMasterFunc;	break;
//	case FAMILY_MEMBER:	i = eSeniorFunc;	break;
//	}
//
//	for(int j=0; j<i; ++j)
//	{		
//		PTRLISTSEARCHSTART(m_FamilyFuncCtrlArray[j],cWindow*,pWin)
//			pWin->SetDisable(FALSE);
//		PTRLISTSEARCHEND
//	}
//	for(j=i; j<eMaxFunc; ++j)
//	{		
//		PTRLISTSEARCHSTART(m_FamilyFuncCtrlArray[j],cWindow*,pWin)
//			pWin->SetDisable(TRUE);
//		PTRLISTSEARCHEND
//	}
//	if(Rank == FAMILY_MASTER)
//	{
//		m_pSecedeBtn->SetDisable(TRUE);
//	}
//	*/
//}
//
//void CFamilyDialog::SetFamilyPosition( char* MapName )
//{
//	m_pLocation->SetStaticText( MapName );
//}
//
//void CFamilyDialog::SortMemberListbyPosition()//Add 060803 by wonju
//{
//	if(m_nPositionFlag)
//	{
//		std::list<FAMILYMEMBERINFO> ChangeList;
//		std::list<FAMILYMEMBERINFO>::iterator iter;
//		//�������� ���� - ���� ���� �ڷ� ����.
//		std::vector<FAMILYMEMBERINFO>* prevList = &m_MemberList;
//
//		for(unsigned int i = 0; i < prevList->size(); ++i)
//		{
//			FAMILYMEMBERINFO curmemberinfo = (*prevList)[i]; //�˻��� �ɹ�
//
//			if(ChangeList.size() > 0)
//			{
//				bool bEnd = TRUE;
//
//				for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//���� �ٲ� ����Ʈ�� �˻�
//				{
//					BYTE Rank = iter->Rank;//�ٲ� ����Ʈ ���� ��
//
//				    if(curmemberinfo.Rank > Rank) //���� ����� ���� ����Ʈ ���� ������ ũ�ٸ�
//					{
//						//�����ϰ� ������.
//                        ChangeList.insert(iter,curmemberinfo);
//						bEnd = FALSE;
//						break;
//					}
//				}
//
//				//������ ������ �������� ���� �ڿ� �����Ѵ�.
//				if(TRUE == bEnd)
//                ChangeList.push_back(curmemberinfo);
//
//			}
//			else
//			{
//				ChangeList.push_back(curmemberinfo);
//			}
//		}
//
//		//���� ����Ʈ�� �׸��� �� �����.
//		m_MemberList.clear();
//		m_pListDlg->RemoveAll();
//
//		for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//�ٲ��׸��� ���� �ٽ� �ִ´�.
//		{
//			FAMILYMEMBERINFO info = *iter;
//			ResetMemberInfo(&info);
//			
//		}
//
//
//		
//	}
//	else
//	{
//		std::list<FAMILYMEMBERINFO> ChangeList;
//		std::list<FAMILYMEMBERINFO>::iterator iter;
//		//�������� ���� - ���� ���� �ڷ� ����.
//		std::vector<FAMILYMEMBERINFO>* prevList = &m_MemberList;
//
//		unsigned int size = prevList->size();
//
//		for(unsigned int i = 0; i < prevList->size(); ++i)
//		{
//			FAMILYMEMBERINFO curmemberinfo = (*prevList)[i]; //�˻��� �ɹ�
//
//			if(ChangeList.size() > 0)
//			{
//				bool bEnd = TRUE;
//
//				for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//���� �ٲ� ����Ʈ�� �˻�
//				{
//					BYTE Rank = iter->Rank;//�ٲ� ����Ʈ ���� ��
//
//					if(curmemberinfo.Rank < Rank) //���� ����� ���� ����Ʈ ���� ������ ũ�ٸ�
//					{
//						//�����ϰ� ������.
//						ChangeList.insert(iter,curmemberinfo);
//						bEnd = FALSE;
//						break;
//					}
//				}
//
//				//������ ������ �������� ���� �ڿ� �����Ѵ�.
//				if(TRUE == bEnd)
//					ChangeList.push_back(curmemberinfo);
//
//			}
//			else
//			{
//				ChangeList.push_back(curmemberinfo);
//			}
//		}
//
//		//���� ����Ʈ�� �׸��� �� �����.
//		m_MemberList.clear();
//		m_pListDlg->RemoveAll();
//
//		for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//�ٲ��׸��� ���� �ٽ� �ִ´�.
//		{
//			FAMILYMEMBERINFO info = *iter;
//			ResetMemberInfo(&info);
//			
//		}
//	}
//
//	m_nPositionFlag ^= 1;
//}
//
//void CFamilyDialog::SortMemberListbyLevel()//Add 060803 by wonju
//{
//	if(m_nLevelFlag)
//	{
//		std::list<FAMILYMEMBERINFO> ChangeList;
//		std::list<FAMILYMEMBERINFO>::iterator iter;
//		//�������� ���� - ���� ���� �ڷ� ����.
//		std::vector<FAMILYMEMBERINFO>* prevList = &m_MemberList;
//
//		for(unsigned int i = 0; i < prevList->size(); ++i)
//		{
//			FAMILYMEMBERINFO curmemberinfo = (*prevList)[i]; //�˻��� �ɹ�
//
//			if(ChangeList.size() > 0)
//			{
//				bool bEnd = TRUE;
//
//				for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//���� �ٲ� ����Ʈ�� �˻�
//				{
//					WORD lvl = iter->Memberlvl;//�ٲ� ����Ʈ ���� ��
//
//					if(curmemberinfo.Memberlvl > lvl) //���� ����� ���� ����Ʈ ���� ������ ũ�ٸ�
//					{
//						//�����ϰ� ������.
//						ChangeList.insert(iter,curmemberinfo);
//						bEnd = FALSE;
//						break;
//					}
//				}
//
//				//������ ������ �������� ���� �ڿ� �����Ѵ�.
//				if(TRUE == bEnd)
//					ChangeList.push_back(curmemberinfo);
//
//			}
//			else
//			{
//				ChangeList.push_back(curmemberinfo);
//			}
//		}
//
//		//���� ����Ʈ�� �׸��� �� �����.
//		m_MemberList.clear();
//		m_pListDlg->RemoveAll();
//
//		for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//�ٲ��׸��� ���� �ٽ� �ִ´�.
//		{
//			FAMILYMEMBERINFO info = *iter;
//			ResetMemberInfo(&info);
//
//		}
//		
//
//		
//	}
//	else
//	{
//		std::list<FAMILYMEMBERINFO> ChangeList;
//		std::list<FAMILYMEMBERINFO>::iterator iter;
//		//�������� ���� - ���� ���� �ڷ� ����.
//		std::vector<FAMILYMEMBERINFO>* prevList = &m_MemberList;
//
//		for(unsigned int i = 0; i < prevList->size(); ++i)
//		{
//			FAMILYMEMBERINFO curmemberinfo = (*prevList)[i]; //�˻��� �ɹ�
//
//			if(ChangeList.size() > 0)
//			{
//				bool bEnd = TRUE;
//
//				for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//���� �ٲ� ����Ʈ�� �˻�
//				{
//					WORD lvl = iter->Memberlvl;//�ٲ� ����Ʈ ���� ��
//
//					if(curmemberinfo.Memberlvl < lvl) //���� ����� ���� ����Ʈ ���� ������ ũ�ٸ�
//					{
//						//�����ϰ� ������.
//						ChangeList.insert(iter,curmemberinfo);
//						bEnd = FALSE;
//						break;
//					}
//				}
//
//				//������ ������ �������� ���� �ڿ� �����Ѵ�.
//				if(TRUE == bEnd)
//					ChangeList.push_back(curmemberinfo);
//
//			}
//			else
//			{
//				ChangeList.push_back(curmemberinfo);
//			}
//		}
//
//		//���� ����Ʈ�� �׸��� �� �����.
//		m_MemberList.clear();
//		m_pListDlg->RemoveAll();
//
//		for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//�ٲ��׸��� ���� �ٽ� �ִ´�.
//		{
//			FAMILYMEMBERINFO info = *iter;
//			ResetMemberInfo(&info);
//
//		}
//	}
//
//	m_nLevelFlag ^= 1;
//
//}
