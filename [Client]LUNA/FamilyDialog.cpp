// FamilyDialog.cpp: implementation of the CFamilyDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "../Input/Mouse.h"															// 마우스 헤더 파일을 불러온다.

#include "WindowIDEnum.h"															// 윈도우 아이디 헤더파일을 불러온다.

#include "./Interface/cWindowManager.h"												// 윈도우 매니져 헤더파일을 불러온다.
#include "../Interface/cResourceManager.h"											// 리소스 매니져 헤더파일을 불러온다.

#include "ObjectManager.h"															// 오브젝트 매니져 헤더파일을 불러온다.
//#include "FamilyManager.h"															// 패밀리 매니져 헤더파일을 불러온다.
#include "ChatManager.h"															// 채팅 매니져 헤더파일을 불러온다.

#include "../hseos/family/shfamilymanager.h"										// 패밀리 매니져 해더파일을 불러온다.

#include "./Interface/cScriptManager.h"												// 스크립트 매니져 헤더파일을 불러온다.

#include "cFont.h"	// 테스트를 위해 폰트를 불러온다.


#include "./interface/cStatic.h"													// 스태틱 인터페이스 헤더파일을 불러온다.
#include "./interface/cComboBox.h"													// 콤보박스 인터페이스 헤더파일을 불러온다.
#include "./interface/cListDialog.h"												// 리스트 다이얼로그 인터페이스 헤더파일을 불러온다.
#include "./interface/cPushupButton.h"												// 푸쉬업 버튼 인터페이스 헤더파일을 불러온다.
#include "cMsgBox.h"																// 메시지 박스 인터페이스 헤더파일을 불러온다.

#include "GameIn.h"																	// 게임인 헤더파일을 불러온다.
//#include "Family.h"																	// 패밀리 헤더파일을 불러온다.

#include "FamilyDialog.h"															// 패밀리 다이얼로그 헤더파일을 불러온다.
#include "FamilyRankDialog.h"														// 패밀리 랭킹 다이얼로그 헤더파일을 불러온다.
#include "FamilyNickNameDialog.h"													// 패밀리 호칭 변경 다이얼로그 헤더파일을 불러온다.
//#include "FamilyUnion.h"	

#include "cImageSelf.h"
#include "FamilyMarkDialog.h"
// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.10.11
#include "../hseos/Farm/SHFarmManageDlg.h"
// E 농장시스템 추가 added by hseos 2007.10.11

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFamilyDialog::CFamilyDialog()														// 생성자 함수.
{
	m_pFamilyName		= NULL ;													// 패밀리 이름를 NULL 처리 한다.
	m_pMasterName		= NULL ;													// 마스터 이름를 NULL 처리 한다.
	m_pHonorPoint		= NULL ;													// 명예 포인트를 NULL 처리 한다.

	m_pListDlg			= NULL ;													// 멤버 리스트 다이얼로그를 NULL 처리 한다.

	m_pMemberID			= NULL ;													// 멤버 아이디를 NULL 처리 한다.
	m_pMemberLevel		= NULL ;													// 멤버 레벨을 NULL 처리 한다.
	m_pMemberClass		= NULL ;													// 멤버 클래스를 NULL 처리 한다.
	m_pMemberLogin		= NULL ;													// 멤버 로그인을 NULL 처리 한다.
	m_pMemberNickName	= NULL ;													// 멤버 호칭을 NULL 처리 한다.
	m_pMemberGuildName  = NULL ;													// 멤버 길드명을 NULL 처리 한다.

	m_pChangeNickName	= NULL ;													// 호칭변경 버튼을 NULL 처리 한다.
	m_pInvite			= NULL ;													// 초대신청 버튼을 NULL 처리 한다.
	m_pRegistMark		= NULL ;													// 문장등록 버튼을 NULL 처리 한다.

	// desc_hseos_패밀리01
	// S 패밀리 추가 added by hseos 2007.10.22
	m_pLeaveBtn			= NULL;
	m_pExpelBtn			= NULL;
	m_pFarmManageBtn	= NULL;
	// E 패밀리 추가 added by hseos 2007.10.22

	for( int count = 0 ; count < MAX_MEMBER_COUNT ; ++count )						// 최대 멤버 수만큼 for문을 돌린다.
	{
		//m_pClassImg[count] = NULL ;												// 카운트에 해당하는 이미지를 NULL처리를 한다.

		ZeroMemory( &m_pClassImg[count], sizeof(cImage) ) ;
	}

	for( int count = 0 ; count < MAX_MEMBER_COUNT ; ++count )						// 최대 멤버 수만큼 for문을 돌린다.
	{
		//m_pLongInImg[count] = NULL ;												// 로그인 이미지를 NULL 처리를 한다.

		ZeroMemory( &m_pLongInImg[count], sizeof(cImage) ) ;
	}

	for( int count = 0 ; count < MAX_MEMBER_COUNT ; ++count )						// 최대 멤버 수만큼 for문을 돌린다.
	{
		//m_pLogOutImg[count] = NULL ;												// 로그아웃 이미지를 NULL 처리를 한다.

		ZeroMemory( &m_pLogOutImg[count], sizeof(cImage) ) ;						
	}

	m_nCurSelectIdx = 0 ;															// 현재 선택 된 멤버를 0번으로 설정한다.

	//m_ClassIconList.RemoveAll() ;													// 클래스 아이콘 리스트를 비워준다.
}

CFamilyDialog::~CFamilyDialog()														// 소멸자 함수.
{
	//ReleaseIconList() ;																// 클래스 아이콘 리스트를 해제한다.
}

void CFamilyDialog::Linking()														// 컨트롤들을 링크하는 함수.
{
	m_pFamilyName		= (cStatic*)GetWindowForID(FMY_NAME) ;						// 패밀리 이름를 링크한다.
	m_pMasterName		= (cStatic*)GetWindowForID(FMY_MASTERNAME) ;				// 마스터 이름를 링크한다.
	m_pHonorPoint		= (cStatic*)GetWindowForID(FMY_HONORPOINT) ;				// 명예 포인트를 링크한다.

	m_pListDlg			= (cListDialog*)GetWindowForID(FMY_MEMBERLIST) ;			// 멤버 리스트 다이얼로그를 링크한다.

	m_pMemberID			= (cStatic*)GetWindowForID(FMY_MEMBERID) ;					// 멤버 아이디를 링크한다.
	m_pMemberLevel		= (cStatic*)GetWindowForID(FMY_MEMBERLEVEL) ;				// 멤버 레벨을 링크한다.
	m_pMemberClass		= (cStatic*)GetWindowForID(FMY_MEMBERCLASS) ;				// 멤버 클래스를 링크한다.
	m_pMemberLogin		= (cStatic*)GetWindowForID(FMY_MEMBERLOGIN) ;				// 멤버 로그인을 링크한다.
	m_pMemberNickName	= (cStatic*)GetWindowForID(FMY_MEMBERNICKNAME) ;			// 멤버 호칭을 링크한다.
	m_pMemberGuildName  = (cStatic*)GetWindowForID(FMY_MEMBERGUILDNAME) ;			// 멤버 길드명을 링크한다.

	m_pChangeNickName	= (cButton*)GetWindowForID(FMY_CHANGENICKNAME) ;			// 호칭변경 버튼을 링크한다.
	m_pInvite			= (cButton*)GetWindowForID(FMY_INVITE) ;					// 초대신청 버튼을 링크한다.
	m_pRegistMark		= (cButton*)GetWindowForID(FMY_REGISTMARK) ;				// 문장등록 버튼을 링크한다.

	// desc_hseos_패밀리01
	// S 패밀리 추가 added by hseos 2007.10.22
	m_pLeaveBtn			= (cButton*)GetWindowForID(FMY_LEAVE);
	m_pExpelBtn			= (cButton*)GetWindowForID(FMY_EXPEL);
	m_pFarmManageBtn	= (cButton*)GetWindowForID(FMY_FARM_MANAGE);
	// E 패밀리 추가 added by hseos 2007.10.22
}

void CFamilyDialog::UpdateAllInfo() 												// 패밀리 창에 보이는 모든 정보를 업데이트 한다.
{
	CSHFamily* pFamily = HERO->GetFamily() ;										// 패밀리 정보를 받아온다.

	if( pFamily )																	// 패밀리 정보가 유효한지 체크한다.
	{
		m_pFamilyName->SetStaticText(pFamily->Get()->szName) ;						// 패밀리명을 세팅한다.

		m_pMasterName->SetStaticText(pFamily->Get()->szMasterName) ;				// 마스터의 이름을 세팅한다.

		SetMemberList(pFamily) ;													// 멤버 리스트를 세팅한다.

		SetHonorPoint(0) ;															// 명예 포인트를 설정한다.

		CSHFamilyMember* pMember = pFamily->GetMember(0) ;							// 멤버 정보를 받아온다.

		if( pMember )																// 멤버 정보가 유효한지 체크한다.
		{
			SetMemberInfo( pMember) ;												// 멤버 정보를 세팅한다.
		}

		ITEM* pItem = m_pListDlg->GetItem(0) ;										// 주어진 인덱스의 아이템 정보를 받는다.

		if( pItem )																	// 아이템 정보가 유효한지 체크한다.
		{
			pItem->rgb = RGB_HALF(255, 255, 0) ; 									// 아이템의 색상을 다시 설정한다.

			m_nCurSelectIdx = 0 ;													// 현재 선택 된 멤버 인덱스를 세팅한다.
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

void CFamilyDialog::SetHonorPoint( DWORD dwPoint )									// 명예 포인트를 설정하는 함수.
{
	CSHFamily* pFamily = HERO->GetFamily() ;										// 패밀리 정보를 받아온다.

	if( pFamily )																	// 패밀리 정보가 유효한지 체크한다.
	{
 		char tempBuf[10+5] = {0, } ;												// 패밀리 포인트를 문자열로 바꾸어 줄 임시 버퍼 선언.

		itoa(pFamily->GetEx()->nHonorPoint, tempBuf, 10) ;							// 패밀리 포인트를 문자로 변환한다.

		m_pHonorPoint->SetStaticText(tempBuf) ;										// 패밀리 포인트를 세팅한다.
	}

	// 명예 포인트가 절감되는 상황에는 dwPoint 에 - int 값을 넣기 때문에  
	// 반드시 int 로 캐스팅 해서 양수인지 확인해야 함. by hseos
	//if( (int)dwPoint > 0 )															// 넘어온 명예 포인트가 0보다 크면,
	//{
	//	char tempBuf[128] = {0, } ;													// 명예 포인트를 문자열로 바꾸어 줄 임시 버퍼 선언.								

	//	sprintf(tempBuf, "명예 포인트가 %d 포인트 상승하였습니다.", dwPoint) ;		// 명예 포인트를 문자로 변환한다.

	//	CHATMGR->AddMsg(CTC_SYSMSG, tempBuf) ;										// 채팅창에 포인트 업 공지를 한다.
	//}
}

void CFamilyDialog::SetMemberInfo( CSHFamilyMember* pMember )						// 패밀리 창 하단부, 멤버의 상세 정보를 나타내는 함수.
{
	m_pMemberID->SetStaticText( pMember->Get()->szName ) ;							// 멤버 이름을 세팅한다.

	char tempBuf[10] = {0, } ;														// 멤버 레벨을 문자로 변환 할 임시 버퍼를 선언한다.
	itoa(pMember->Get()->nLV, tempBuf, 10) ;										// 멤버 레벨을 문자로 변환한다.
	m_pMemberLevel->SetStaticText( tempBuf ) ;										// 멤버 레벨을 세팅한다.

	int nClass		= pMember->Get()->nJobFirst ;									// 멤버의 클래스 계열 값을 받는다.
	int nRace		= pMember->Get()->nRace ;										// 멤버의 종족 번호를 받는다.
	int nJobGrade	= pMember->Get()->nJobGrade ;									// 멤버의 클래스 단계를 받는다.
	int nJobNum		= pMember->Get()->nJobCur ;										// 멤버의 클래스 번호를 받는다.

	if( nJobGrade == 1 )													// 직업 단계가 1일때는, 계열마다 한가지 직업밖에 없기 때문에
	{
		nJobNum = 1 ;														// 직업 번호를 1로 세팅한다.
	}

	int nClassIdx = ((nClass)*1000) + ((nRace+1)*100) + (nJobGrade*10) + nJobNum ;	// 멤버의 클래스 번호를 받는다.

	m_pMemberClass->SetStaticText(RESRCMGR->GetMsg(RESRCMGR->GetClassNameNum(nClassIdx))) ;	// 멤버 클래스를 세팅한다.


	ZeroMemory(tempBuf, sizeof(tempBuf)) ;											// 임시 버퍼를 초기화 한다.

	int nIsLogIn = pMember->Get()->eConState ;										// 멤버의 로그인 여부를 받는다.
	char* pChatMsg = NULL;

	switch(nIsLogIn)																// 로그인 여부를 확인한다.
	{
	//case MEMBER_CONSTATE_LOGOFF :	strcpy(tempBuf, "접속안함") ;					// 접속 안한 상태로 세팅한다.
	//case MEMBER_CONSTATE_LOGIN :	strcpy(tempBuf, "접속중") ;						// 접속 중인 상태로 세팅한다.

	case CSHGroupMember::MEMBER_CONSTATE_LOGOFF:
		{
			// 접속 안한 상태로 세팅한다.
			//strcpy(tempBuf, CHATMGR->GetChatMsg( 1443 )) ;
			pChatMsg = CHATMGR->GetChatMsg( 1443 );// 080306 NYJ --- Buffer Overflow를 방지하기 위해 포인터로 받아오는게 안전할듯.
		}
		break ;
	case CSHGroupMember::MEMBER_CONSTATE_LOGIN:
		{
			// 접속 중인 상태로 세팅한다.
			//strcpy(tempBuf, CHATMGR->GetChatMsg( 1442 )) ;
			pChatMsg = CHATMGR->GetChatMsg( 1442 );// 080306 NYJ --- Buffer Overflow를 방지하기 위해 포인터로 받아오는게 안전할듯.
		}
		break ;
	}
	
	m_pMemberLogin->SetStaticText(/*tempBuf*/pChatMsg) ;										// 멤버의 로그인 여부를 세팅한다.
	m_pMemberNickName->SetStaticText( pMember->Get()->szNickname ) ;				// 멤버의 호칭을 세팅한다.
	m_pMemberGuildName->SetStaticText( pMember->GetEx()->szGuild ) ;				// 멤버의 길드명을 세팅한다.
}

void CFamilyDialog::SetMemberList(CSHFamily* pFamily)
{
	DWORD dwColor = 0xffffffff ;													// 정보 색상을 설정한다.

	int nClassBlank = 8 ;															// 클래스 이미지를 출력할 공간을 비울 수를 담는 변수선언.
	int nLevelBlank = 0 ;															// 레벨을 출력할 공간을 비울 수를 담는 변수 선언.
	int nNameBlank  = 0 ;															// 이름을 출력할 공간을 비울 수를 담는 변수 선언.

	int nClass		= 0 ;															// 멤버 클래스를 담을 변수를 선언한다.
	int nRace		= 0 ;															// 멤버의 종족을 담을 변수를 선언한다.
	int nJobGrade	= 0 ;															// 멤버의 클래스 단계를 담을 변수를 선언한다.
	int nJobNum		= 0 ;															// 멤버의 클래스 번호를 담을 변수를 선언한다.

	int nLogIn		= 0 ;															// 멤버 로그인 여부를 담을 변수를 선언한다.

	char tempBuf[128]	= {" ",} ;													// 멤버 정보를 담을 임시 버퍼를 선언한다.
	char classBuf[8]	= {" ",} ;													// 멤버 클래스의 빈칸을 나타낼 버퍼를 선언한다.
	char levelBuf[8]	= {" ",} ;													// 멤버 레벨을 담을 버퍼를 선언한다.
	char membername[MAX_NAME_LENGTH+1] = {" ",} ;									// 멤버 이름을 담을 버퍼를 선언한다.

	CSHFamilyMember* pMember = NULL ;												// 멤버 정보를 담을 포인터를 선언한다.

	m_pListDlg->RemoveAll() ;														// 리스트를 비운다.

	int nMemberCount = pFamily->Get()->nMemberNum ;									// 멤버 수를 구한다.

	for( int bigCount = 0 ; bigCount < nMemberCount ; ++bigCount )					// 멤버 수만큼 포문을 돌린다.
	{
		pMember = pFamily->GetMember(bigCount) ;									// 카운트에 해당하는 멤버 정보를 받는다.

		if( pMember && pMember->Get()->nID != 0 )									// 멤버 정보가 유효한지 체크한다.
		{

			nClass		= pMember->Get()->nJobFirst ;								// 멤버의 클래스 계열 값을 받는다.
			nRace		= pMember->Get()->nRace ;									// 멤버의 종족 번호를 받는다.
			nJobGrade	= pMember->Get()->nJobGrade ;								// 멤버의 클래스 단계를 받는다.
			nJobNum		= pMember->Get()->nJobCur ;

			if( nJobGrade == 1 )													// 직업 단계가 1일때는, 계열마다 한가지 직업밖에 없기 때문에
			{
				nJobNum = 1 ;														// 직업 번호를 1로 세팅한다.
			}

			int nClassIdx = ((nClass)*1000) + ((nRace+1)*100) + (nJobGrade*10) + nJobNum ;	// 멤버의 클래스 번호를 받는다.

			//int nIconNum = GetClassIconNum(nClassIdx) ;								// 클래스에 해당하는 이미지 번호를 받는다.
			//if( nIconNum >= 0 && m_ClassIconList.GetCount() >= nIconNum )			// 아이콘 이미지 번호의 영역을 확인한다.
			// 080320 LYW --- FamilyDialog : 클래스 아이콘 기능을 리소스 매니져로 옮김
 			int nIconNum = RESRCMGR->GetClassIconNum(nClassIdx) ;
 			if( nIconNum >= 0 && RESRCMGR->GetClassIconCount() >= nIconNum )
			{
				SCRIPTMGR->GetImage( nIconNum, &m_pClassImg[bigCount], PFT_CLASS_ICON_PATH ) ;	// 클래스 아이콘 이미지를 할당받는다.
			}

			for( int count = 0 ; count < nClassBlank-1 ; ++count )					// 클래스 여백의 길이만큼 포문을 돌린다.
			{
				classBuf[count] = 0x20 ;											// 클래스 여백의 길이를 빈칸으로 채운다.
			}

			int nLevel = pMember->Get()->nLV ;

			itoa(nLevel, levelBuf, 10) ;											// 멤버 레벨을 레벨 버퍼에 문자로 변환하여 담는다.

			//itoa(pMember->Get()->nLV, levelBuf, 12) ;								// 멤버 레벨을 레벨 버퍼에 문자로 변환하여 담는다.

			nLevelBlank = strlen(levelBuf) ;										// 레벨 문자의 길이를 구한다.

			for( int count = nLevelBlank ; count < 7 ; ++count )					// 레벨 길이로 초기값을 맞추고, 포문을 돌린다.
			{
				levelBuf[count] = 0x20 ;											// 레벨의 여백을 빈칸으로 채운다.
			}


			SafeStrCpy(membername, pMember->Get()->szName, MAX_NAME_LENGTH+1) ;		// 멤버 이름을 복사한다.

			nNameBlank = strlen(membername) ;										// 멤버 이름의 길이를 구한다.

			for(int count = nNameBlank ; count < MAX_NAME_LENGTH ; ++count)			// 이름 길이로 초기값을 맞추고, 포문을 돌린다.
			{																		
				membername[count] = 0x20 ;											// 이름의 여백을 빈칸으로 채운다.
			}


			sprintf(tempBuf, "%s %s %s", classBuf, levelBuf, membername);			// 임시 버퍼를 클래스 영역과, 레벨, 이름으로 채운다.

			if(pMember->Get()->eConState==CSHGroupMember::MEMBER_CONSTATE_LOGOFF)	// 멤버가 로그인 중이라면,
			{
				dwColor = RGB_HALF(200, 200, 200) ;									// 회색으로 색상을 설정한다.
			}
			else																	// 멤버가 로그아웃 상태라면,
			{
				dwColor = RGB_HALF(10, 10, 10) ;									// 검은색으로 색상을 설정한다.				
			}

			m_pListDlg->AddItem( tempBuf, dwColor ) ;								// 멤버 리스트에 정보를 세팅한다.


			nLogIn = pMember->Get()->eConState ;									// 멤버의 로그인 여부를 받는다.

			if( nLogIn )															// 멤버가 로그인 중이라면,
			{
				SCRIPTMGR->GetImage( 102, &m_pLongInImg[bigCount], PFT_HARDPATH ) ;	// 로그인 이미지로 세팅한다.
			}
			else																	// 멤버가 로그아웃 중이라면,
			{
				SCRIPTMGR->GetImage( 103, &m_pLongInImg[bigCount], PFT_HARDPATH ) ;	// 로그아웃 이미지로 세팅한다.
			}
		}
	}
}

void CFamilyDialog::Render()														// 클래스 이미지와 로그인 이미지를 출력할 렌더 함수.
{
	if( !IsActive() ) return ;														// 패밀리 창이 활성화 중이 아니라면, 리턴한다.

	cDialog::RenderWindow();														// 윈도우를 렌더한다.

	cDialog::RenderComponent();														// 윈도우 컴퍼넌트들을 렌더한다.


	CSHFamily* pFamily = HERO->GetFamily() ;										// 패밀리 정보를 받아온다.

	if( pFamily && pFamily->Get()->nMasterID != 0 )									// 패밀리 정보가 유효하고, 마스터 아이디가 0이 아니면,
	{
		CSHFamilyMember* pMember = NULL ;											// 멤버 정보를 담을 포인터를 선언한다.

		VECTOR2 scale;																// 이미지 스케일을 적용할 벡터를 선언한다.

		scale.x = 1.0f;																// 스케일 X를 1로 세팅한다.
		scale.y = 1.0f;																// 스케일 Y를 1로 세팅한다.

		VECTOR2 vPos;																// 이미지 위치를 적용할 벡터를 선언한다.

		DWORD dwColor = 0xffffffff ;												// 이미지 출력에 적용할 색상을 선언한다.

		for( UINT count = 0 ; count < pFamily->Get()->nMemberNum ; ++count )		// 멤버 수만큼 포문을 돌린다.
		{
			vPos.y = (float)(GetAbsY() + 156) + (16*count) ;						// 이미지 출력위치 Y를 세팅한다.

			vPos.x = (float)(GetAbsX() + 30) ;										// 이미지 출력위치 X를 세팅한다.

			if( !m_pClassImg[count].IsNull() )										// 멤버 클래스 이미지가 널이 아니라면,
			{
				m_pClassImg[count].RenderSprite(&scale, NULL, 0, &vPos, dwColor) ;	// 멤버 클래스 이미지를 출력한다.
			}

			vPos.x += 190  ;														// 로그인 위치를 출력할 세팅한다.

			if( !m_pLongInImg[count].IsNull() )										// 멤버 로그인 여부 이미지가 널이 아니라면,
			{
				m_pLongInImg[count].RenderSprite(&scale, NULL, 0, &vPos, dwColor) ;	// 멤버 로그인 여부 이미지를 출력한다.
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

DWORD CFamilyDialog::ActionEvent( CMouse* mouseInfo )								// 패밀리 창의 이벤트를 처리하는 함수.
{
	DWORD we = WE_NULL;																// 리턴할 윈도우 이벤트 변수를 선언한다.

	if( !m_bActive )		return we;												// 윈도우가 활성화 상태가 아니면, 이벤트를 리턴한다.

	we = cDialog::ActionEvent(mouseInfo);											// 기본 다이얼로그의 이벤트를 받아온다.

	if(m_pListDlg->PtIdxInRow(mouseInfo->GetMouseX(),mouseInfo->GetMouseY()) != -1)	// 멤버 리스트내에 모우스가 위치해 있다면,
	{
		if(we & WE_LBTNCLICK)														// 마우스 왼쪽 버튼을 클릭했다면,
		{ 
			if( WINDOWMGR->IsMouseDownUsed() == FALSE)								// 윈도우 매니져의 마우스 다운 사용이 FALSE와 같다면,
			{		
				int Idx = m_pListDlg->GetCurSelectedRowIdx();						// 리스트에서 선택한 줄을 받아온다.

				if(Idx != -1)														// 선택한 줄이 -1과 같이 않다면,
				{
					CSHFamily* pFamily = HERO->GetFamily() ;						// 패밀리 정보를 받아온다.

					if( pFamily )													// 패밀리 정보가 유효한지 체크한다.
					{
						CSHFamilyMember* pMember = pFamily->GetMember(Idx) ;		// 선택 된 인덱스의 멤버 정보를 받는다.

						if( pMember )												// 멤버 정보가 유효한지 체크한다.
						{
							SetMemberList(pFamily) ;								// 멤버 리스트를 한번 갱신한다.
							SetMemberInfo( pMember) ;								// 멤버 정보를 세팅한다.

							ITEM* pItem = m_pListDlg->GetItem(Idx) ;				// 주어진 인덱스의 아이템 정보를 받는다.

							if( pItem )												// 아이템 정보가 유효한지 체크한다.
							{
								pItem->rgb = RGB_HALF(255, 255, 0) ;				// 아이템의 색상을 다시 설정한다.

								m_nCurSelectIdx = Idx ;								// 현재 선택 된 멤버 인덱스를 세팅한다.
							}
						}
					}
				}
			}
		}
	}

	return we;																		// 윈도우 이벤트를 리턴 처리 한다.
}

void CFamilyDialog::OnActionEvent( LONG lId, void* p, DWORD we )																		// 패밀리 창의 버튼의 이벤트 처리를 하는 함수.
{
	CSHFamily* pFamily = HERO->GetFamily() ;																							// 패밀리 정보를 받아온다.

	if( we & WE_BTNCLICK )																												// D왼쪽 버튼을 클릭한 이벤트가 넘어왔다면,
	{
		switch( lId )																													// 넘어온 버튼의 아이디를 확인한다.
		{
		case FMY_CHANGENICKNAME :																										// 호칭변경을 클릭하였다면, (호칭 변경은 모든 멤버가 사용할 수 있다.)
			{
				if( pFamily )																											// 패밀리 정보가 유효한지 체크한다.
				{
					CSHFamilyMember* pMember = pFamily->GetMember(m_nCurSelectIdx) ;													// 선택한 대상 정보를 받아온다.

					if( pMember )																										// 멤버 정보가 유효한지 체크한다.
					{
						CFamilyNickNameDialog* pDlg = GAMEIN->GetFamilyNickNameDlg() ;													// 호칭 변경창 정보를 받는다.

						if( pDlg )																										// 호칭 변경창 정보가 유효한지 체크한다.
						{
							pDlg->SetActive(TRUE) ;																						// 호칭 변경창을 활성화 시킨다.
						}
					}
				}
			}
			break ;

		case FMY_INVITE :																												// 초대신청을 클릭하였다면,
			{
				if( pFamily && pFamily->Get()->nMasterID == HERO->GetID() )																// 패밀리 정보가 유효하고, HERO가 마스터 이면,
				{
					CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetSelectedObject() ;														// 선택한 플레이어 정보를 받는다.

					if( pPlayer )																										// 플레이어 정보가 유효한지 체크한다.
					{
						if( pPlayer->GetObjectKind() == eObjectKind_Player )															// 플레이어 타입이 유저인지 체크한다.
						{
							if( pPlayer->GetID() != HERO->GetID() )																		// 본인을 선택한 상태인지 체크한다.
							{
								CSHFamilyMember* pMember = NULL ;																		// 패밀리 멤버 정보를 담을 포인터를 선언한다.			

								for( int count = 0 ; count < MAX_MEMBER_COUNT ; ++count )												// 멤버의 아이디와 비교하기 위해 for문을 돌린다.
								{
									pMember = pFamily->GetMember(count) ;																// 멤버의 정보를 받는다.

									if( pMember )																						// 멤버 정보가 유효한지 체크한다.
									{
										if( pMember->Get()->nID != pPlayer->GetID() )													// 멤버의 아이디와 선택한 대상의 아이디가 같지 않으면,
										{
											g_csFamilyManager.CLI_RequestInvite(pPlayer) ;												// 초대 신청을 한다.
											return ;																					// 리턴 처리를 한다.
										}
										else																							// 선택한 대상의 아이디와 멤버의 아이디가 같으면,
										{
											WINDOWMGR->MsgBox( MBI_FAMILY_ERROR_NOTARGET, MBT_OK, CHATMGR->GetChatMsg( 1316) );
										}
									}

									pMember = NULL ;																					// 멤버 정보를 담을 포인터를 NULL 처리를 한다.
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
					else																												// 플레이어 정보가 유효하지 않다면,
					{
						WINDOWMGR->MsgBox( MBI_FAMILY_ERROR_NOTARGET, MBT_OK, CHATMGR->GetChatMsg( 1319 ) );
					}
				}
				else
				{
					WINDOWMGR->MsgBox( MBI_FAMILY_ABLE_MASTER, MBT_OK, CHATMGR->GetChatMsg( 732 ) );									// 예외 처리를 한다.
				}
			}
			break ;

		case FMY_REGISTMARK :																											// 문장 등록을 클릭하였다면,
			{
 				GAMEIN->GetFamilyMarkDlg()->SetActive(TRUE);			
//				WINDOWMGR->MsgBox( MBI_FAMILY_ERROR_NOTARGET, MBT_OK, CHATMGR->GetChatMsg(528) );										// 예외 처리를 한다.
			}
			break ;

		// desc_hseos_패밀리01
		// S 패밀리 추가 added by hseos 2007.10.22
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
		// E 패밀리 추가 added by hseos 2007.10.22
		}
	}
}

//void CFamilyDialog::LoadClassIcon() 												// 클래스 아이콘을 로드하는 함수.
//{
//	CMHFile fp;																		// 묵향 파일을 선언한다.
//
//	fp.Init( FILE_IMAGE_CLASS_ICON_PATH, "rb" ) ;									// 클래스 아이콘 파일을 읽어들인다.
//
//	int nNull = 0 ;																	// 의미없는 수를 담을 변수를 선언한다.
//
//	while(TRUE)																		// while문을 돌린다.
//	{
//		if( fp.IsEOF() != FALSE ) break ;											// 파일의 끝에 도달하면, break.
//
//		IDINDEX* pIdIdx = new IDINDEX ;												// 인덱스와 클래스 인덱스를 담을 메모리를 할당한다.
//
//		pIdIdx->nIdx = fp.GetInt() ;												// 파일로 부터 인덱스를 받는다.
//
//		pIdIdx->nClassIdx = fp.GetInt() ;											// 파일로 부터 클래스 인덱스를 받는다.
//
//		m_ClassIconList.AddTail(pIdIdx) ;											// 클래스 아이콘 리스트에 추가한다.
//
//		for( int count = 0 ; count < 5 ; ++count )									// 나머지 뒤의 숫자 수 만큼 for문을 돌린다.
//		{
//			nNull = fp.GetInt() ;													// 여기서 사용하지 않는 수는 그냥 읽기만 한다.
//		}
//	}
//
//	fp.Release() ;																	// 클래스 아이콘 파일을 해제한다.
//}
//
//void CFamilyDialog::ReleaseIconList()												// 클래스 아이콘 리스트를 해제한다.
//{
//	IDINDEX* pIdIdx = NULL ;														// 인덱스와 클래스 인덱스 정보를 받을 포인터를 선언한다.
//
//	PTRLISTPOS pos	= NULL ;														// 리스트상의 위치 정보를 받을 포인터를 선언한다.
//
//	pos = m_ClassIconList.GetHeadPosition() ;										// 클래스 아이콘 리스트를 해더로 세팅한다.
//
//	while(pos)																		// 위치정보가 있는동안, while을 돌린다.
//	{
//		pIdIdx = (IDINDEX*)m_ClassIconList.GetNext(pos) ;							// pos에 해당하는 인덱스, 클래스 인덱스 정보를 받는다.
//
//		if( pIdIdx )																// 받은 정보가 유효한지 체크한다.
//		{
//			delete pIdIdx ;															// 받은 주소의 메모리를 삭제한다.
//
//			pIdIdx = NULL ;															// 주소를 가리키는 포인터를 NULL 처리 한다.
//		}
//	}
//
//	m_ClassIconList.RemoveAll() ;													// 리스트를 모두 비운다.
//}

//int CFamilyDialog::GetClassIconNum(int nClass)
//{
//	IDINDEX* pIdIdx = NULL ;														// 인덱스와 클래스 인덱스 정보를 받을 포인터를 선언한다.
//
//	PTRLISTPOS pos	= NULL ;														// 리스트상의 위치 정보를 받을 포인터를 선언한다.
//
//	pos = m_ClassIconList.GetHeadPosition() ;										// 클래스 아이콘 리스트를 해더로 세팅한다.
//
//	while(pos)																		// 위치정보가 있는동안, while을 돌린다.
//	{
//		pIdIdx = (IDINDEX*)m_ClassIconList.GetNext(pos) ;							// pos에 해당하는 인덱스, 클래스 인덱스 정보를 받는다.
//
//		if( pIdIdx )																// 받은 정보가 유효한지 체크한다.
//		{
//			if( pIdIdx->nClassIdx == nClass )										// 인자로 넘어온 클래스 인덱스와, 리스트의 클래스 인덱스가 같다면,
//			{
//				return pIdIdx->nIdx ;												// 인덱스를 리턴한다.
//			}
//		}
//	}
//
//	return -1 ;																		// 같은 클래스 인덱스가 없을경우 -1을 리턴한다.
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
//	m_MemberList.push_back(*pInfo);//가공되지 않은 정보를 넣는다.//Add 060803 by wonju
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
//	//			// 06. 03. 문파공지 - 이영준
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
//	//			// 06. 03. 문파공지 - 이영준
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
//		//오름차순 정렬 - 작은 값이 뒤로 들어간다.
//		std::vector<FAMILYMEMBERINFO>* prevList = &m_MemberList;
//
//		for(unsigned int i = 0; i < prevList->size(); ++i)
//		{
//			FAMILYMEMBERINFO curmemberinfo = (*prevList)[i]; //검사할 맴버
//
//			if(ChangeList.size() > 0)
//			{
//				bool bEnd = TRUE;
//
//				for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//새로 바뀐 리스트와 검사
//				{
//					BYTE Rank = iter->Rank;//바뀐 리스트 내의 값
//
//				    if(curmemberinfo.Rank > Rank) //현재 적용될 값이 리스트 내의 값보다 크다면
//					{
//						//삽입하고 나간다.
//                        ChangeList.insert(iter,curmemberinfo);
//						bEnd = FALSE;
//						break;
//					}
//				}
//
//				//루프가 끝까지 돌았으면 가장 뒤에 삽입한다.
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
//		//이전 리스트의 항목을 다 지운다.
//		m_MemberList.clear();
//		m_pListDlg->RemoveAll();
//
//		for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//바뀐항목의 값을 다시 넣는다.
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
//		//오름차순 정렬 - 작은 값이 뒤로 들어간다.
//		std::vector<FAMILYMEMBERINFO>* prevList = &m_MemberList;
//
//		unsigned int size = prevList->size();
//
//		for(unsigned int i = 0; i < prevList->size(); ++i)
//		{
//			FAMILYMEMBERINFO curmemberinfo = (*prevList)[i]; //검사할 맴버
//
//			if(ChangeList.size() > 0)
//			{
//				bool bEnd = TRUE;
//
//				for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//새로 바뀐 리스트와 검사
//				{
//					BYTE Rank = iter->Rank;//바뀐 리스트 내의 값
//
//					if(curmemberinfo.Rank < Rank) //현재 적용될 값이 리스트 내의 값보다 크다면
//					{
//						//삽입하고 나간다.
//						ChangeList.insert(iter,curmemberinfo);
//						bEnd = FALSE;
//						break;
//					}
//				}
//
//				//루프가 끝까지 돌았으면 가장 뒤에 삽입한다.
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
//		//이전 리스트의 항목을 다 지운다.
//		m_MemberList.clear();
//		m_pListDlg->RemoveAll();
//
//		for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//바뀐항목의 값을 다시 넣는다.
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
//		//오름차순 정렬 - 작은 값이 뒤로 들어간다.
//		std::vector<FAMILYMEMBERINFO>* prevList = &m_MemberList;
//
//		for(unsigned int i = 0; i < prevList->size(); ++i)
//		{
//			FAMILYMEMBERINFO curmemberinfo = (*prevList)[i]; //검사할 맴버
//
//			if(ChangeList.size() > 0)
//			{
//				bool bEnd = TRUE;
//
//				for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//새로 바뀐 리스트와 검사
//				{
//					WORD lvl = iter->Memberlvl;//바뀐 리스트 내의 값
//
//					if(curmemberinfo.Memberlvl > lvl) //현재 적용될 값이 리스트 내의 값보다 크다면
//					{
//						//삽입하고 나간다.
//						ChangeList.insert(iter,curmemberinfo);
//						bEnd = FALSE;
//						break;
//					}
//				}
//
//				//루프가 끝까지 돌았으면 가장 뒤에 삽입한다.
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
//		//이전 리스트의 항목을 다 지운다.
//		m_MemberList.clear();
//		m_pListDlg->RemoveAll();
//
//		for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//바뀐항목의 값을 다시 넣는다.
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
//		//오름차순 정렬 - 작은 값이 뒤로 들어간다.
//		std::vector<FAMILYMEMBERINFO>* prevList = &m_MemberList;
//
//		for(unsigned int i = 0; i < prevList->size(); ++i)
//		{
//			FAMILYMEMBERINFO curmemberinfo = (*prevList)[i]; //검사할 맴버
//
//			if(ChangeList.size() > 0)
//			{
//				bool bEnd = TRUE;
//
//				for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//새로 바뀐 리스트와 검사
//				{
//					WORD lvl = iter->Memberlvl;//바뀐 리스트 내의 값
//
//					if(curmemberinfo.Memberlvl < lvl) //현재 적용될 값이 리스트 내의 값보다 크다면
//					{
//						//삽입하고 나간다.
//						ChangeList.insert(iter,curmemberinfo);
//						bEnd = FALSE;
//						break;
//					}
//				}
//
//				//루프가 끝까지 돌았으면 가장 뒤에 삽입한다.
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
//		//이전 리스트의 항목을 다 지운다.
//		m_MemberList.clear();
//		m_pListDlg->RemoveAll();
//
//		for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//바뀐항목의 값을 다시 넣는다.
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
//	m_MemberList.push_back(*pInfo);//가공되지 않은 정보를 넣는다.//Add 060803 by wonju
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
//				// 06. 03. 문파공지 - 이영준
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
//				// 06. 03. 문파공지 - 이영준
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
//		//오름차순 정렬 - 작은 값이 뒤로 들어간다.
//		std::vector<FAMILYMEMBERINFO>* prevList = &m_MemberList;
//
//		for(unsigned int i = 0; i < prevList->size(); ++i)
//		{
//			FAMILYMEMBERINFO curmemberinfo = (*prevList)[i]; //검사할 맴버
//
//			if(ChangeList.size() > 0)
//			{
//				bool bEnd = TRUE;
//
//				for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//새로 바뀐 리스트와 검사
//				{
//					BYTE Rank = iter->Rank;//바뀐 리스트 내의 값
//
//				    if(curmemberinfo.Rank > Rank) //현재 적용될 값이 리스트 내의 값보다 크다면
//					{
//						//삽입하고 나간다.
//                        ChangeList.insert(iter,curmemberinfo);
//						bEnd = FALSE;
//						break;
//					}
//				}
//
//				//루프가 끝까지 돌았으면 가장 뒤에 삽입한다.
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
//		//이전 리스트의 항목을 다 지운다.
//		m_MemberList.clear();
//		m_pListDlg->RemoveAll();
//
//		for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//바뀐항목의 값을 다시 넣는다.
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
//		//오름차순 정렬 - 작은 값이 뒤로 들어간다.
//		std::vector<FAMILYMEMBERINFO>* prevList = &m_MemberList;
//
//		unsigned int size = prevList->size();
//
//		for(unsigned int i = 0; i < prevList->size(); ++i)
//		{
//			FAMILYMEMBERINFO curmemberinfo = (*prevList)[i]; //검사할 맴버
//
//			if(ChangeList.size() > 0)
//			{
//				bool bEnd = TRUE;
//
//				for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//새로 바뀐 리스트와 검사
//				{
//					BYTE Rank = iter->Rank;//바뀐 리스트 내의 값
//
//					if(curmemberinfo.Rank < Rank) //현재 적용될 값이 리스트 내의 값보다 크다면
//					{
//						//삽입하고 나간다.
//						ChangeList.insert(iter,curmemberinfo);
//						bEnd = FALSE;
//						break;
//					}
//				}
//
//				//루프가 끝까지 돌았으면 가장 뒤에 삽입한다.
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
//		//이전 리스트의 항목을 다 지운다.
//		m_MemberList.clear();
//		m_pListDlg->RemoveAll();
//
//		for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//바뀐항목의 값을 다시 넣는다.
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
//		//오름차순 정렬 - 작은 값이 뒤로 들어간다.
//		std::vector<FAMILYMEMBERINFO>* prevList = &m_MemberList;
//
//		for(unsigned int i = 0; i < prevList->size(); ++i)
//		{
//			FAMILYMEMBERINFO curmemberinfo = (*prevList)[i]; //검사할 맴버
//
//			if(ChangeList.size() > 0)
//			{
//				bool bEnd = TRUE;
//
//				for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//새로 바뀐 리스트와 검사
//				{
//					WORD lvl = iter->Memberlvl;//바뀐 리스트 내의 값
//
//					if(curmemberinfo.Memberlvl > lvl) //현재 적용될 값이 리스트 내의 값보다 크다면
//					{
//						//삽입하고 나간다.
//						ChangeList.insert(iter,curmemberinfo);
//						bEnd = FALSE;
//						break;
//					}
//				}
//
//				//루프가 끝까지 돌았으면 가장 뒤에 삽입한다.
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
//		//이전 리스트의 항목을 다 지운다.
//		m_MemberList.clear();
//		m_pListDlg->RemoveAll();
//
//		for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//바뀐항목의 값을 다시 넣는다.
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
//		//오름차순 정렬 - 작은 값이 뒤로 들어간다.
//		std::vector<FAMILYMEMBERINFO>* prevList = &m_MemberList;
//
//		for(unsigned int i = 0; i < prevList->size(); ++i)
//		{
//			FAMILYMEMBERINFO curmemberinfo = (*prevList)[i]; //검사할 맴버
//
//			if(ChangeList.size() > 0)
//			{
//				bool bEnd = TRUE;
//
//				for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//새로 바뀐 리스트와 검사
//				{
//					WORD lvl = iter->Memberlvl;//바뀐 리스트 내의 값
//
//					if(curmemberinfo.Memberlvl < lvl) //현재 적용될 값이 리스트 내의 값보다 크다면
//					{
//						//삽입하고 나간다.
//						ChangeList.insert(iter,curmemberinfo);
//						bEnd = FALSE;
//						break;
//					}
//				}
//
//				//루프가 끝까지 돌았으면 가장 뒤에 삽입한다.
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
//		//이전 리스트의 항목을 다 지운다.
//		m_MemberList.clear();
//		m_pListDlg->RemoveAll();
//
//		for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)//바뀐항목의 값을 다시 넣는다.
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
