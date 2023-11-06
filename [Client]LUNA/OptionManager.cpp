// OptionManager.cpp: implementation of the COptionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OptionManager.h"
#include "MacroManager.h"
#include "MouseCursor.h"
#include "MousePointer.h"
#include "./Input/UserInput.h"
#include "ChatManager.h"
#include ".\Effect\EffectManager.h"
#include "cChatTipManager.h"
#include "ObjectManager.h"
#include "MHAudioManager.h"
#include "MHCamera.h"
#include "GameIn.h"
// 061220 LYW --- Delete this code.
//#include "MainBarDialog.h"
#include "./interface/cWindowManager.h"
#include "OptionDialog.h"
#include "./interface/cDialog.h"
#include "./interface/cGuageBar.h"
#include "WindowIDEnum.h"
#include "WeatherManager.h" // 2005.12.30일 추가 눈 On/Off
#include "MHMap.h"

// 070402 LYW --- Include chatting dialog.
#include "ChattingDlg.h"

#include "Hero.h"

// 071026 LYW --- include tutorial manager.
#include "TutorialManager.h"

// 071026 LYW --- Include tutorial button dialog.
#include "TutorialBtnDlg.h"

//#include "UserInfoManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(COptionManager);

COptionManager::COptionManager()
{
	ZeroMemory(&m_GameOption, sizeof(m_GameOption));														// 게임 옵션을 담고 있는 멤버 구조체를 초기화 한다.
}

COptionManager::~COptionManager()
{
}

void COptionManager::Init()
{
	if( !LoadGameOption() )																					// 게임 옵션을 로딩한다. 실패하면,
	{
		SetDefaultOption();																					// 기본 옵션을 세팅한다.
	}

	m_bBGMSoundChanged	= FALSE;																			// 배경음악이 바뀌었는지 여부를 FALSE로 세팅한다.
	
	AUDIOMGR->SetBGMSoundOn( m_GameOption.bSoundBGM );														// 옵션 값에 따라 배경 음악의 ON/OFF 를 설정한다.

	m_wSendOption		= 0;																				// 멤버 옵션 플래그 값을 0으로 세팅한다.
	
	m_bShowMemberDamage = TRUE;																				// 파티원 데미지 보기를 TRUE로 세팅한다.

	// 071026 LYW --- OptionManager : Setting option of tutorial.
	if( m_GameOption.bShowTutorial )
	{
		TUTORIALMGR->Set_ActiveTutorial(TRUE) ;							// 튜토리얼을 비활성화 상태로 세팅한다.
	}
	else
	{
		TUTORIALMGR->Set_ActiveTutorial(FALSE) ;							// 튜토리얼을 비활성화 상태로 세팅한다.
	}
}

void COptionManager::SendOptionMsg()
{
	WORD wOption = MakeOptionFlag();																		// 세팅되어 있는 거부 관련 옵션값을 받는다.

	m_wSendOption = wOption;																				// 옵션 플래그 값을 멤버 플래그로 받는다.

	MSG_WORD msgOption;																						// WORD를 가지고 있는 메시지 구조체를 선언한다.
	msgOption.Category		= MP_OPTION;																	// 카테고리를 옵션으로 세팅한다.
	msgOption.Protocol		= MP_OPTION_SET_SYN;															// 프로토콜을 옵션 세팅 싱크로 세팅한다.
	msgOption.dwObjectID	= HEROID;																		// 오브젝트 아이디를 현재 캐릭터 아이디로 세팅한다.
	msgOption.wData			= wOption;																		// WORD형 데이터에 옵션 플래그를 세팅한다.

	NETWORK->Send( &msgOption, sizeof( MSG_WORD ) );														// 서버로 현재 세팅된 내용을 전송한다.
}

WORD COptionManager::MakeOptionFlag()
{
	WORD wOption = 0;																						// 옵션값을 담을 변수.

	if( m_GameOption.bNoDeal )		wOption |= eOPT_NODEAL;													// 거래 거부가 체크되어 있으면 추가한다.
	if( m_GameOption.bNoParty )		wOption |= eOPT_NOPARTY;												// 파티 거부가 체크되어 있으면 추가한다.
	if( m_GameOption.bNoFriend )	wOption |= eOPT_NOFRIEND;												// 친구등록 거부가 체크되어 있으면 추가한다.
	if( m_GameOption.bNoShowdown )	wOption |= eOPT_NOSHOWDOWN;												// 결투 거부가 체크되어 있으면 추가한다.
	if( m_GameOption.bNoWhisper )	wOption |= eOPT_NOWHISPER;												// 귓속말 거부가 체크되어 있으면 추가한다.

	return wOption;																							// 세팅된 옵션값을 리턴한다.
}

void COptionManager::SetGameOption( sGAMEOPTION* pOption )
{
	if( m_GameOption.bSoundBGM != pOption->bSoundBGM )														// 입력 받은 배경음악 값과, 멤버의 값이 같지 않으면,
	{
		m_bBGMSoundChanged = TRUE;																			// 배경음악이 바뀌었는지 여부를 TRUE로 세팅한다.
	}

	m_GameOption = *pOption;																				// 멤버의 옵션 구조체를 입력받은 구조체로 세팅한다.

	SaveGameOption();																						// 옵션을 저장한다.
}

BOOL COptionManager::LoadGameOption()
{
	HANDLE	hFile;																							// 파일 핸들을 선언한다.

	hFile = CreateFile( "./system/GameOption.opt", GENERIC_READ, 0, NULL,									// 지정한 파일로 핸들을 받는다.(읽기모드)
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE )																		// 핸들 받기에 실패하면,
		return FALSE;																						// 리턴 처리를 한다.

	DWORD dwRead = 0;																						// 실제로 읽은 바이트 수를 리턴받기 위한 출력용 인수.

	DWORD dwVersion;																						// 읽어들이는 데이터를 저장할 버퍼.

	if( !ReadFile( hFile, &dwVersion, sizeof(DWORD), &dwRead, NULL ) )										// 받은 핸들을 참조해 데이터를 읽는다. 실패하면,
	{
		CloseHandle( hFile );																				// 받은 핸들을 닫는다.
		return FALSE;																						// 리턴 처리를 한다.
	}
	if( dwVersion != OPTIONVERSION )																		// 버전이 주어진 수와 같지 않으면,
	{
//		SetDefaultOption() ;																				// 기본 옵션을 세팅한다.
//		ApplySettings() ;																					// 기본 옵션을 적용한다.

		CloseHandle( hFile );																				// 받은 핸들을 닫는다.
		return FALSE;																						// 리턴 처리를 한다.
	}

	if( !ReadFile( hFile, &m_GameOption, sizeof(m_GameOption), &dwRead, NULL ) )							// 게임 옵션 구조체 만큼 멤버 구조체에 데이터를 받는다. 실패하면,
	{
		CloseHandle( hFile );																				// 받은 핸들을 닫는다.
		return FALSE;																						// 리턴 처리를 한다.
	}
	
	if( sizeof( m_GameOption ) != dwRead )																	// 읽어들인 바이트수를 비교한다. 같지 않으면,
	{
		CloseHandle( hFile );																				// 받은 핸들을 닫는다.
		return FALSE;																						// 리턴 처리를 한다.
	}

	CloseHandle( hFile );																					// 핸들을 닫는다.
	return TRUE;																							// 리턴 처리를 한다.
}

void COptionManager::SaveGameOption()
{
	HANDLE	hFile;																							// 파일 핸들을 선언한다.

	hFile=CreateFile( "./system/GameOption.opt", GENERIC_WRITE, 0, NULL,									// 지정한 파일로 핸들을 받는다.(쓰기모드)
					  CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE )																		// 핸들 받기에 실패하면,
		return;																								// 리턴 처리를 한다.
	
	DWORD dwWritten;																						// 실제로 기록한 바이트 수를 리턴받기 위한 변수.
	DWORD dwVersion = OPTIONVERSION ;																		// 버전을 세팅한다.
	WriteFile( hFile, &dwVersion, sizeof(DWORD), &dwWritten, NULL );										// 버전을 기록한다.
	WriteFile( hFile, &m_GameOption, sizeof(m_GameOption), &dwWritten, NULL );								// 옵션구조체 만큼 기록한다.
	
	CloseHandle( hFile );																					// 핸들을 닫는다.
}

void COptionManager::ApplySettings()
{
	ApplySound() ;																							// 사운드 파트를 적용한다.
	
	ApplyGraphics() ;																						// 그래픽 파트를 적용한다.

	ApplyChatting() ;																						// 채팅 파트를 적용한다.

	ApplyEnvironmemt() ;																					// 환경 파트를 적용한다.
}

void COptionManager::ApplySound()
{
	if( m_bBGMSoundChanged )																				// 배경음악의 변경여부가 TRUE 이면,
	{
		AUDIOMGR->SetBGMSoundOn( m_GameOption.bSoundBGM );													// 옵션에 담긴 값으로 배경음악을 ON/OFF 세팅한다.

		if( m_GameOption.bSoundBGM )																		// 배경음악 옵션값이 ON이면, 
		{
			AUDIOMGR->PlayLastBGM();																		// 오디오 매니져의 마지막 배경음악을 플레이 시킨다.
		}
		else																								// 배경음악 옶션값이 OFF 이면, 
		{
			AUDIOMGR->StopBGM();																			// 오디오 매니져의 배경음악을 스톱 시킨다.
		}

		m_bBGMSoundChanged = FALSE;																			// 배경음악의 변경여부를 FALSE로 세팅한다.
	}

	if( m_GameOption.bSoundBGM )	//bgm이 play중이 아닐때 볼륨조절하면 뻑난다.							// 배경음악이 플레이 중이면,
	{
		AUDIOMGR->SetBGMVolume( (float)m_GameOption.nVolumnBGM / 100.0f );									// 옵션의 BGM Volume을 백분율로 해서 배경음악 볼륨을 세팅한다.
	}
	
	AUDIOMGR->SetEffectSoundOn( m_GameOption.bSoundEnvironment );											// 옵션의 사운드 값에 따라 이펙트 사운드를 ON/OFF 한다.

	if( m_GameOption.bSoundEnvironment )																	// 옵션의 사운드 값이 TRUE 이면,
	{
		AUDIOMGR->SetSoundVolume( (float)m_GameOption.nVolumnEnvironment / 100.0f ) ;						// 옵션의 볼륨값을 백분율로 해서 사운드 볼륨을 세팅한다.
	}
	else																									// 옵션의 사운드값이 FALSE 이면,
	{
		AUDIOMGR->StopAll();																				// 모두 스톱시킨다.
	}
}

void COptionManager::ApplyGraphics()
{
	CURSOR->SetActive( !m_GameOption.bGraphicCursor );														// 옵션의 커서 값에 따라 커서를 ON/OFF 한다.

	CAMERA->SetSightDistanceRate( (float)m_GameOption.nSightDistance );										// 시야 거리율을 세팅한다.

	// 070723 LYW --- OptionManager : 포그를 빼버림.
	//MAPDESC* pmapdesc = MAP->GetMapDesc();																	// 맵 디스크립트 정보를 받아온다.

	//if( pmapdesc )																							// 정보가 유효한지 체크한다.
	//{
	//	// 카메라가 설정된 값을 받아온다.
	//	float fFogDistance = pmapdesc->DefaultSight * (float)(100-(100-(float)m_GameOption.nSightDistance)*1.5) / 100.0f ;

	//	pmapdesc->fogdesc.fEnd = fFogDistance * 0.88f ;														// 안개의 끝 지점을 세팅한다.
	//	pmapdesc->fogdesc.fStart = fFogDistance * 0.22f ;													// 안개의 시작 지점을 세팅한다.

	//	g_pExecutive->GetGeometry()->EnableFog(0);															
	//	g_pExecutive->GetGeometry()->SetFog(&pmapdesc->fogdesc,0);											// 안개를 설정한다.
	//}

	DWORD dwShadowOption = eOO_SHADOW_NONE;																	// 그림자 옵션을 담을 변수를 선언 및 초기화 한다.

	if( m_GameOption.bShadowHero )		dwShadowOption |= eOO_SHADOW_ONLYHERO;								// 자신이 체크되어 있으면 추가한다.
	if( m_GameOption.bShadowMonster )	dwShadowOption |= eOO_SHADOW_MONSTER;								// 몬스터나 NPC가 체크되어 있으면 추가한다.
	if( m_GameOption.bShadowOthers )	dwShadowOption |= eOO_SHADOW_OTHERCHARACTER;						// 다른캐릭터가 체크되어 있으면 추가한다.

	OBJECTMGR->SetShadowOption( dwShadowOption );															// 오브젝트 매니져에 옵션을 적용한다.

	//SW 기획, 그래픽 전달은 0레벨이 최소 버텍스지만 여기서는 2 레벨이 최소이다.
	g_pExecutive->SetModelLODUsingMode( m_GameOption.nLODMode, 2 ) ;										// LOD 사용모드를 설정한다.
	//Effect
	OBJECTMGR->SetEffectOption( m_GameOption.nEffectMode ) ;												// 이펙트 모드를 설정한다.
}

void COptionManager::ApplyChatting()
{	
	sChatOption ChatOption;

	ChatOption.bOption[CTO_NOWHISPERMSG]	= m_GameOption.bNoWhisper ;										// 귓속말 거부를 세팅한다.
	ChatOption.bOption[CTO_NOCHATMSG]		= m_GameOption.bNoChatting ;									// 일반채팅 끄기를 세팅한다.
	ChatOption.bOption[CTO_NOSHOUTMSG]		= m_GameOption.bNoShoutChat ;									// 외치기 끄기를 세팅한다.
	ChatOption.bOption[CTO_NOPARTYMSG]		= m_GameOption.bNoPartyChat ;									// 파티 채팅 끄기를 세팅한다.
	ChatOption.bOption[CTO_NOFAMILYMSG]		= m_GameOption.bNoFamilyChat ;									// 패밀리 채팅 끄기를 세팅한다.
	ChatOption.bOption[CTO_NOGUILDMSG]		= m_GameOption.bNoGuildChat ;									// 길드 채팅 끄기를 세팅한다.
	ChatOption.bOption[CTO_NOALLIANCEMSG]	= m_GameOption.bNoAllianceChat ;								// 동맹 채팅 끄기를 세팅한다.
	ChatOption.bOption[CTO_NOSYSMSG]		= m_GameOption.bNoSystemMsg ;									// 시스템 메시지 끄기를 세팅한다.
	ChatOption.bOption[CTO_NOEXPMSG]		= m_GameOption.bNoExpMsg ;										// 경험치 획득 메시지 끄기를 세팅한다.
	ChatOption.bOption[CTO_NOITEMMSG]		= m_GameOption.bNoItemMsg ;										// 아이템 획득 메시지 끄기를 세팅한다.

	CHATMGR->SetOption( &ChatOption );																		// 채팅 매니져에 옵션을 적용한다.
}

void COptionManager::ApplyEnvironmemt()
{
	DWORD dwOverInfoOption = eOO_NAME_CLICKEDONLY ;	//---default											// 오버 정보 옵션을 담을 변수 선언.

	if( m_GameOption.bNameOthers )	dwOverInfoOption |= eOO_NAME_WHOLE ;									// 다른 캐릭터 이름표시를 추가한다.
	if( m_GameOption.bNoMemberDamage )	dwOverInfoOption |= eOO_DAMAGE_MEMBERS ;							// 파티원 데미지 보기를 추가한다.
	if( m_GameOption.bNameParty )	dwOverInfoOption |= eOO_NAME_PARTY ;									// 파티원 이름 표시를 추가한다.
	if( m_GameOption.bNameMunpa )	dwOverInfoOption |= eOO_NAME_MUNPA ;									// 길드원 이름 표시를 추가한다.
	if( m_GameOption.bNoBalloon )	dwOverInfoOption |= eOO_CHAT_NOBALLOON ;								// 말풍선이 TRUE면 추가한다.

	OBJECTMGR->SetOverInfoOption( dwOverInfoOption ) ;														// 오브젝트 매니져에 오버 정보 옵션을 적용한다.

	CHATTIPMGR->SetActive( m_GameOption.bNoGameTip ) ;														// 게임 팁 보기를 설정한다.

	OPTIONMGR->SetMemberDemageActive( m_GameOption.bNoMemberDamage ) ;										// 파티원 데미지 보기를 설정한다.

	MACROMGR->SetMacroMode( m_GameOption.nMacroMode ) ;														// 채팅우선, 단축키 우선모드를 설정한다.
}

void COptionManager::CancelSettings()
{
	//직접 영향 주었던 것들 제대로 돌려놓기
}

void COptionManager::SetDefaultOption()
{
	m_GameOption.bNoDeal 			= FALSE;																// 거래 거부.
	m_GameOption.bNoParty 			= FALSE;																// 파티 거부.
	m_GameOption.bNoFriend 			= FALSE;																// 친구등록 거부.
	m_GameOption.bNoShowdown 		= FALSE;																// 결투 거부.

	m_GameOption.bNameMunpa 		= TRUE;																	// 길드원 이름 표시.
	m_GameOption.bNameParty 		= TRUE;																	// 파티원 이름 표시.
	m_GameOption.bNameOthers 		= TRUE;																	// 다른 캐릭터 이름 표시.
	m_GameOption.bNoMemberDamage 	= TRUE;																	// 파티원 데미지 보기.
	m_GameOption.bNoGameTip 		= TRUE;																// 게임 팁 보기.

	m_GameOption.nMacroMode 		= 1 ;																	// 채팅우선모드/단축키우선모드.

	m_GameOption.bNoWhisper			= FALSE;																// 귓속말 거부.
	m_GameOption.bNoChatting		= FALSE;																// 일반채팅 끄기.
	m_GameOption.bNoBalloon			= FALSE;																// 말풍선 끄기.
	m_GameOption.bNoPartyChat		= FALSE ;																// 파티 채팅 끄기.
	m_GameOption.bNoFamilyChat		= FALSE ;																// 패밀리 채팅 끄기.
	m_GameOption.bNoShoutChat		= FALSE;																// 외치기 끄기.
	m_GameOption.bNoGuildChat		= FALSE;																// 길드채팅 끄기.
	m_GameOption.bNoAllianceChat	= FALSE;																// 동맹채팅 끄기.

	m_GameOption.bNoSystemMsg		= FALSE;																// 시스템메시지 끄기.
	m_GameOption.bNoExpMsg			= FALSE;																// 경험치.수련치 획득메시지 끄기.
	m_GameOption.bNoItemMsg			= FALSE;																// 돈/아이템 획득메시지 끄기.

	m_GameOption.nSightDistance		= 155;//m_SigntGBMaxValue-50;													// 시야거리 조절값.
	m_GameOption.bGraphicCursor		= FALSE;																// 커서 온오프?
	m_GameOption.bShadowHero		= TRUE;																	// 그림자 표시 자신.
	m_GameOption.bShadowMonster		= TRUE;																	// 그림자 표시 몬스터/NPC.
	m_GameOption.bShadowOthers		= TRUE;																	// 그림자 표시 다른캐릭터.

	m_GameOption.bAutoCtrl			= FALSE;																// 게임 화면 자동 조절.
	m_GameOption.bAmbientMax		= TRUE;																	// 캐릭터 밝기?
	m_GameOption.nLODMode			= 0;																	// 그래픽 상태.
	m_GameOption.nEffectMode		= 0;																	// 이펙트 모드.
	m_GameOption.nEffectSnow        = 0;																	// 눈 이펙트 끄기.

	m_GameOption.bSoundBGM			= TRUE;																	// 배경음악 ON/OFF.
	m_GameOption.bSoundEnvironment	= TRUE;																	// 효과음 ON/OFF.
	m_GameOption.nVolumnBGM			= 100;																	// 배경음악 볼륨값.
	m_GameOption.nVolumnEnvironment	= 100;																	// 효과음 볼륨값.

	// 071025 LYW --- OptionManager : Setting tutorial option to default.
	m_GameOption.bShowTutorial		= TRUE ;																// 기본 튜토리얼 보이기로 세팅.
}

void COptionManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_OPTION_SET_ACK:
		{

		}
		break;

	case MP_OPTION_SET_NACK:
		{

		}
		break;
	}
}

void COptionManager::GraphicAutoCtrlProcess()																
{
	WEATHERMGR->SetWeather(m_GameOption.nEffectSnow);														// 게임 옵션값으로 날씨를 세팅한다.

	if( !m_GameOption.bAutoCtrl ) return;																	// 자동 조절값이 FALSE면 리턴처리를 한다.

	return;																									// 현재 일부러 리턴처리 한듯.

	DWORD Frame = 0;																						// 프레임을 담을 변수.

	//10초간의 평균 프레임을 구해 기준 프레임과 비교하여 처리한다.
	if(gTickTime)																							// 틱 정보가 유효한지 체크한다.
	{
		Frame = 1000 / gTickTime;																			// 프레임 값을 받는다.											
		++m_dwCountFrame;																					// 프레임 카운트를 증가한다.
		m_dwFrameAverage += Frame;																			// 프레임 평균값에 프레임 값을 더한다.
	}
	//Effect
	//State counter
	if( gCurTime - m_dwLastCheckTime > 10000 )																// 10초가 흘렀으면,
	{
		m_dwFrameAverage /= m_dwCountFrame;																	// 평균값을 프레임 카운트로 나눠서 받는다.

		if( STANDARD_FRAME > m_dwFrameAverage )																// 표준 프레임보다 평균프레임이 작으면,
		{
			m_nCurOptionState--;																			// 현재 옵션 상태를 감소시킨다.
		}
		else if( NICE_FRAME < m_dwFrameAverage )															// 나이스 프레임보다 평균 프레임이 작으면,
		{
			m_nCurOptionState++;																			// 현재 옵션 상태를 증가 시킨다.
		}

		m_dwFrameAverage = m_dwCountFrame = 0;																// 평균 프레임과, 프레임 카운트를 0으로 세팅한다.

		if( m_nCurOptionState < LOWEST )																	// 현재 옵션 상태가 최악보다 못하면, 
		{
			m_nCurOptionState = 0;																			// 현재 옵션 상태를 0으로 세팅한다.
		}
		else if( m_nCurOptionState > HIGH )																	// 현재 옵션 상태가 높은편 보다 크면,
		{
			m_nCurOptionState = 5;																			// 현재 옵션 상태를 5로 세팅한다.
		}

		m_dwLastCheckTime = gCurTime;																		// 마지막 체크시간을 설정한다.
	}

	switch( m_nCurOptionState )																				// 현재 옵션 상태를 확인한다.
	{
	case LOWEST:																							// 최악이라면,
		{
			if( m_dwCheck == LOWEST ) return;																// m_dwCheck가 LOWEST와 같으면 리턴 처리를 한다.

			m_GameOption.nSightDistance = m_SigntGBMinValue;												//시야거리를 최저로 세팅한다.
			
			m_GameOption.nLODMode = TRUE;																	//LOD 낮추기.
			
			m_GameOption.bShadowHero = FALSE;																// 그림자 표시 - 자신을 끈다.
			m_GameOption.bShadowMonster = FALSE;															// 그림자 표시 - 몬스터/NPC를 끈다.
			m_GameOption.bShadowOthers = FALSE;																// 그림자 표시 - 다른 캐릭터를 끈다.

			m_dwShdowOption = eOO_SHADOW_NONE;																// 그림자 옵션을 끈다.
			
			m_GameOption.nEffectMode = TRUE;																//이펙트를 끈다.
			
			ApplyGraphicSetting();																			// 그래픽 세팅을 적용한다.
			
			m_dwCheck = LOWEST;																				// m_dwCheck를 LOWEST로 세팅한다.	
		}
		break;

	case LOW:																								// 나쁜 상태라면,
		{
			if( m_dwCheck == LOW ) return;																	// m_dwCheck가 LOW와 같으면 리턴 처리를 한다.

			m_GameOption.nSightDistance = m_SigntGBMinValue + ( m_SigntGBMaxValue - m_SigntGBMinValue ) / 4;//시야거리를 세팅한다.
			
			m_GameOption.nLODMode = TRUE;																	//LOD 낮추기.
			
			m_GameOption.bShadowHero = FALSE;																// 그림자 표시 - 자신을 끈다.
			m_GameOption.bShadowMonster = FALSE;															// 그림자 표시 - 몬스터/NPC를 끈다.
			m_GameOption.bShadowOthers = FALSE;																// 그림자 표시 - 다른 캐릭터를 끈다.

			m_dwShdowOption = eOO_SHADOW_NONE;																// 그림자 옵션을 끈다.
			
			m_GameOption.nEffectMode = FALSE;																//이펙트를 켠다.
			
			ApplyGraphicSetting();																			// 그래픽 세팅을 적용한다.
			
			m_dwCheck = LOW;																				// m_dwCheck를 LOW로 세팅한다.	
		}
		break;

	case MID:																								// 보통인 편이라면, 
		{
			if( m_dwCheck == MID ) return;																	// m_dwCheck가 LOW와 같으면 리턴 처리를 한다.
	
			m_GameOption.nSightDistance = m_SigntGBMinValue + ( m_SigntGBMaxValue - m_SigntGBMinValue ) / 2;// 시야거리를 세팅한다.
			
			m_GameOption.nLODMode = TRUE;																	//LOD	낮추기.
			
			m_GameOption.bShadowHero = TRUE;																// 그림자 표시 - 자신을 켠다.
			m_GameOption.bShadowMonster = FALSE;															// 그림자 표시 - 몬스터/NPC를 끈다.
			m_GameOption.bShadowOthers = FALSE;																// 그림자 표시 - 다른 캐릭터를 끈다.

			m_dwShdowOption = eOO_SHADOW_ONLYHERO;															// 그림자 옵션을 자신만 켠다.
			
			m_GameOption.nEffectMode = FALSE;																// 이펙트를 켠다.
			
			ApplyGraphicSetting();																			// 그래픽 세팅을 적용한다.
			
			m_dwCheck = MID;																				// m_dwCheck를 MID로 세팅한다.	
		}
		break;

	case HIGH:																								// 좋은편이라면,
		{
			if( m_dwCheck == HIGH ) return;																	// m_dwCheck가 HIGH와 같으면 리턴 처리를 한다.
		
			m_GameOption.nSightDistance = m_SigntGBMaxValue;												// 시야거리를 최고로 세팅한다.
		
			m_GameOption.nLODMode = FALSE;																	//LOD 원상태로 한다.
			
			m_GameOption.bShadowHero = TRUE;																// 그림자 표시 - 자신을 켠다.
			m_GameOption.bShadowMonster = TRUE;																// 그림자 표시 - 몬스터/NPC를 켠다.
			m_GameOption.bShadowOthers = TRUE;																// 그림자 표시 - 다른 캐릭터를 켠다.

			m_dwShdowOption = (eOO_SHADOW_ONLYHERO|eOO_SHADOW_MONSTER|eOO_SHADOW_OTHERCHARACTER);			// 그림자 옵션을 모두 켠다.

			m_GameOption.nEffectMode = FALSE;																// 이펙트를 켠다.
			
			ApplyGraphicSetting();																			// 그래픽 세팅을 적용한다.
			
			m_dwCheck = HIGH;																				// m_dwCheck를 HIGH로 세팅한다.	
		}
		break;

	default:
		return;
	}
}

void COptionManager::ApplyGraphicSetting()
{
	CAMERA->SetSightDistanceRate( (float)m_GameOption.nSightDistance );										// 카메라의 시야거리를 세팅한다.
	g_pExecutive->SetModelLODUsingMode( m_GameOption.nLODMode, 2 );											// LOD사용모드를 설정한다.
	OBJECTMGR->SetShadowOption( m_dwShdowOption );															// 그림자 옵션을 적용한다.
	OBJECTMGR->SetEffectOption( m_GameOption.nEffectMode );													// 이펙트 옵션을 적용한다.
}

void COptionManager::InitForGameIn()
{
	m_pOptionDlg = (COptionDialog*)WINDOWMGR->GetWindowForID( OTI_TABDLG );									// 옵션 다이얼로그 정보를 받는다.
	cDialog* pDlg = (cDialog*)m_pOptionDlg->GetTabSheet(2);													// 세번째 시트를 받는다.
	m_pCammaGB = (cGuageBar*)(pDlg->GetWindowForID( OTI_GB_GAMMA ));										// 밝기 조절 게이지바 정보를 받는다.
	m_pSightGB = (cGuageBar*)(pDlg->GetWindowForID( OTI_GB_SIGHT ));										// 시야거리 조절 게이지바 정보를 받는다.
	m_SigntGBMinValue = m_pSightGB->GetMinValue();															// 시야거리 최소값을 받는다.
	m_SigntGBMaxValue = m_pSightGB->GetMaxValue();															// 시야거리 최대값을 받는다.
	m_dwShdowOption = eOO_SHADOW_NONE;																		// 그림자 옵션을 아무것도 없는것으로 세팅한다.
	m_dwLastCheckTime = m_dwCheck = m_dwFrameAverage = m_dwCountFrame = 0;									// 시간관련 변수들을 0으로 세팅한다.
	m_nCurOptionState = HIGH;																				// 현재 옵션 상태를 HIGH로 설정한다.
}

//void COptionManager::Init()
//{
//	if( !LoadGameOption() )																					
//		SetDefaultOption();																					
//
//	m_bBGMSoundChanged	= FALSE;																			
//	m_wSendOption		= 0;
//	
//	m_bShowMenberDamage = TRUE;
//
//
//	AUDIOMGR->SetBGMSoundOn( m_GameOption.bSoundBGM );
//
//	// desc_hseos_옵션01
//	// S 옵션 추가 added by hseos 2007.06.19
//	m_GameOption.nChatLineNum = 0;
//	m_GameOption.nCurChatMode = 0;
//	// E 옵션 추가 added by hseos 2007.06.19
//}

//void COptionManager::SendOptionMsg()
//{
//	WORD wOption = MakeOptionFlag();
//
////	if( m_wSendOption == wOption )
////		return;
//
//	m_wSendOption = wOption;
//
//	MSG_WORD msgOption;
//	msgOption.Category		= MP_OPTION;
//	msgOption.Protocol		= MP_OPTION_SET_SYN;
//	msgOption.dwObjectID	= HEROID;
//	msgOption.wData			= wOption;
////	m_pOptionDlg->m_WeatherManager.SetWeather(m_GameOption.nEffectSnow);
//
//	NETWORK->Send( &msgOption, sizeof( MSG_WORD ) );
//}

//WORD COptionManager::MakeOptionFlag()
//{
//	WORD wOption = 0;
//
//	// 061204 LYW --- Delete This Code For A Moment.
//	/*
//	if( m_GameOption.bNoDeal )		wOption |= eOPT_NODEAL;
//	if( m_GameOption.bNoParty )		wOption |= eOPT_NOPARTY;
//	if( m_GameOption.bNoFriend )	wOption |= eOPT_NOFRIEND;
//	if( m_GameOption.bNoShowdown )		wOption |= eOPT_NOSHOWDOWN;
//	*/
//	if( m_GameOption.bNoWhisper )	wOption |= eOPT_NOWHISPER;
//
//	return wOption;
//}

//void COptionManager::SetGameOption( sGAMEOPTION* pOption )
//{
//	if( m_GameOption.bSoundBGM != pOption->bSoundBGM )
//		m_bBGMSoundChanged = TRUE;
//
//	m_GameOption = *pOption;
//
//	SaveGameOption();
//}

//BOOL COptionManager::LoadGameOption()
//{
////	char strFilePath[MAX_PATH];
////	wsprintf( strFilePath, "%s\\User\\%s\\%s.OPT", DIRECTORYMGR->GetFullDirectoryName(eLM_Root), USERINFOMGR->GetUserID(), USERINFOMGR->GetUserID() );
//
//	HANDLE	hFile;																							
//
////	hFile = CreateFile( strFilePath, GENERIC_READ, 0, NULL,
//	hFile = CreateFile( "./system/GameOption.opt", GENERIC_READ, 0, NULL,
//						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
//
//	if( hFile == INVALID_HANDLE_VALUE )
//		return FALSE;
//
//	DWORD dwRead = 0;
//	DWORD dwVersion;
//	if( !ReadFile( hFile, &dwVersion, sizeof(DWORD), &dwRead, NULL ) )
//	{
//		CloseHandle( hFile );
//		return FALSE;
//	}
//	if( dwVersion != 0x00050418 )
//	{
//		CloseHandle( hFile );
//		return FALSE;
//	}
//
//	if( !ReadFile( hFile, &m_GameOption, sizeof(m_GameOption), &dwRead, NULL ) )
//	{
//		CloseHandle( hFile );
//		return FALSE;
//	}
//	
//	if( sizeof( m_GameOption ) != dwRead )
//	{
//		CloseHandle( hFile );
//		return FALSE;
//	}
//
////사운드나 감마, 시야거리등의 수치가 유효범위인지를 체크하는 것을 추가해야한다.
////유효범위가 아니면 모두 초기화!	//CONFIRM
//	
//
//	CloseHandle( hFile );
//	return TRUE;
//}

//void COptionManager::SaveGameOption()
//{
////	char strFilePath[MAX_PATH];
//
//	//make directory
////	wsprintf( strFilePath, "%s\\User", DIRECTORYMGR->GetFullDirectoryName(eLM_Root) );
////	CreateDirectory( strFilePath, NULL );
////	wsprintf( strFilePath, "%s\\User\\%s", DIRECTORYMGR->GetFullDirectoryName(eLM_Root), USERINFOMGR->GetUserID() );
////	CreateDirectory( strFilePath, NULL );
////	wsprintf( strFilePath, "%s\\User\\%s\\%s.UII", DIRECTORYMGR->GetFullDirectoryName(eLM_Root), USERINFOMGR->GetUserID(), USERINFOMGR->GetUserID() );
//
//	HANDLE	hFile;
//
//
////	hFile=CreateFile( strFilePath, GENERIC_WRITE, 0, NULL, 
//	hFile=CreateFile( "./system/GameOption.opt", GENERIC_WRITE, 0, NULL, 
//					  CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
//
//	if( hFile == INVALID_HANDLE_VALUE )
//		return;
//	
//	DWORD dwWritten;
//	DWORD dwVersion = 0x00050418;
//	WriteFile( hFile, &dwVersion, sizeof(DWORD), &dwWritten, NULL );
//	WriteFile( hFile, &m_GameOption, sizeof(m_GameOption), &dwWritten, NULL );
//	
//	CloseHandle( hFile );
//}
//
//void COptionManager::ApplySettings()
//{
//////////
////사운드
//	if( m_bBGMSoundChanged )
//	{
//		AUDIOMGR->SetBGMSoundOn( m_GameOption.bSoundBGM );
//
//		if( m_GameOption.bSoundBGM )
//			AUDIOMGR->PlayLastBGM();
//		else
//			AUDIOMGR->StopBGM();
//
//		m_bBGMSoundChanged = FALSE;
//	}
//
//	if( m_GameOption.bSoundBGM )	//bgm이 play중이 아닐때 볼륨조절하면 뻑난다.
//		AUDIOMGR->SetBGMVolume( (float)m_GameOption.nVolumnBGM / 100.0f );
//	
//	AUDIOMGR->SetEffectSoundOn( m_GameOption.bSoundEnvironment );
//	if( m_GameOption.bSoundEnvironment )
//		AUDIOMGR->SetSoundVolume( (float)m_GameOption.nVolumnEnvironment / 100.0f );
//	else
//		AUDIOMGR->StopAll();
//	
//////////
////그래픽
//
////-게임 감마
////-이름표시 옵션 
////-그래픽커서
//	CURSOR->SetActive( !m_GameOption.bGraphicCursor );
////	MOUSE->SetGraphicCursor( m_GameOption.bGraphicCursor );
////	CURSOR->SetActive( TRUE );//MouseTest용
//	//taiyo test
//	//WINDOWMGR->GetMouseWindow()->SetActive( m_GameOption.bGraphicCursor );
//
////시야거리
//	//CAMERA->SetSightDistanceRate( (float)m_GameOption.nSightDistance );
//	CAMERA->SetSightDistanceRate( (float)100 );
//
////그림자 출력여부
//	DWORD dwShadowOption = eOO_SHADOW_NONE;
//	if( m_GameOption.bShadowHero )		dwShadowOption |= eOO_SHADOW_ONLYHERO;
//	if( m_GameOption.bShadowMonster )	dwShadowOption |= eOO_SHADOW_MONSTER;
//	if( m_GameOption.bShadowOthers )	dwShadowOption |= eOO_SHADOW_OTHERCHARACTER;
//	OBJECTMGR->SetShadowOption( dwShadowOption );
//
////이름	
//	DWORD dwOverInfoOption = eOO_NAME_CLICKEDONLY;	//---default
//	dwOverInfoOption |= eOO_NAME_WHOLE;
//	dwOverInfoOption |= eOO_DAMAGE_MEMBERS;
//	dwOverInfoOption |= eOO_NAME_PARTY;
//	dwOverInfoOption |= eOO_NAME_MUNPA;
//
//	// 061204 LYW --- Delete This Code For A Moment.
//	/*
//	if( m_GameOption.bNameOthers )	dwOverInfoOption |= eOO_NAME_WHOLE;
//	if( m_GameOption.bNoMemberDamage )	dwOverInfoOption |= eOO_DAMAGE_MEMBERS;
//	if( m_GameOption.bNameParty )	dwOverInfoOption |= eOO_NAME_PARTY;
//	if( m_GameOption.bNameMunpa )	dwOverInfoOption |= eOO_NAME_MUNPA;
//	if( m_GameOption.bNoBalloon )	dwOverInfoOption |= eOO_CHAT_NOBALLOON;
//	*/
//	OBJECTMGR->SetOverInfoOption( dwOverInfoOption );
//	
////////
////채팅
//	// 061206 LYW --- Modified.
//	// Before
//	/*
//	sChatOption ChatOption;
//	ChatOption.bOption[CTO_AUTOHIDECHAT]	= m_GameOption.bAutoHide;
//	ChatOption.bOption[CTO_NOSYSMSG]		= m_GameOption.bNoSystemMsg;
//	ChatOption.bOption[CTO_NOITEMMSG]		= m_GameOption.bNoItemMsg;
//	ChatOption.bOption[CTO_NOEXPMSG]		= m_GameOption.bNoExpMsg;
//	ChatOption.bOption[CTO_NOCHATMSG]		= m_GameOption.bNoChatting;
//	ChatOption.bOption[CTO_NOSHOUTMSG]		= m_GameOption.bNoShoutChat;
//	ChatOption.bOption[CTO_NOGUILDMSG]		= m_GameOption.bNoGuildChat;
//	ChatOption.bOption[CTO_NOALLIANCEMSG]	= m_GameOption.bNoAllianceChat;
//	CHATMGR->SetOption( &ChatOption );
//	*/
//	// After
//
//	// 070402 LYW --- Delete OSChattingDlg.
//	/*
//	if( GAMEIN->GetOSChattingDlg() )
//	{
//		BYTE Alpha ;
//		cDialog* pDlg = ( cDialog*)m_pOptionDlg->GetTabSheet(0) ;
//		cGuageBar* pGuageBar = ( cGuageBar* )pDlg->GetWindowForID( OTI_GB_CHATALPHA ) ;
//		Alpha = (BYTE)pGuageBar->GetCurValue() ;
//		// 061225 LYW --- Delete this code.
//		//GAMEIN->GetChattingDlg()->SetOptionAlpha( Alpha ) ;
//		GAMEIN->GetChattingDlg()->SetAlpha( Alpha ) ;
//		pGuageBar = ( cGuageBar* )pDlg->GetWindowForID( OTI_GB_CHATALPHA2 ) ;
//		Alpha = (BYTE)pGuageBar->GetCurValue() ;
//		// 061225 LYW --- Delete this code.
//		//GAMEIN->GetOSChattingDlg()->SetOptionAlpha( Alpha ) ;
//		GAMEIN->GetOSChattingDlg()->SetAlpha( Alpha ) ;
//	}
//	*/
//
//	// End
//
////SW050822 Add GraphicCtrl
//
//	//LOD
//	g_pExecutive->SetModelLODUsingMode( m_GameOption.nLODMode, 2 );	//SW 기획, 그래픽 전달은 0레벨이 최소 버텍스지만 여기서는 2 레벨이 최소이다.
//	//Effect
//	OBJECTMGR->SetEffectOption( m_GameOption.nEffectMode );
////!
//	if( m_GameOption.bAutoCtrl )
//	{
//		
//	}
////
//	//SW060904 Add MapAmbientCtrlOptn
////	if( m_GameOption.bAmbientMax )
////	{
////		g_pExecutive->GetGeometry()->SetAmbientColor(0,0xffffffff);
////	}
////	else
//	{
//		MAPDESC* pmapdesc = MAP->GetMapDesc();
//		if(pmapdesc)
//			g_pExecutive->GetGeometry()->SetAmbientColor(0,pmapdesc->Ambient);
//	}
//
//
//	// 061204 LYW --- Delete This Code For A Moment.
//	/*
//	CHATTIPMGR->SetActive( m_GameOption.bNoGameTip );
//	OPTIONMGR->SetMemberDemageActive( m_GameOption.bNoMemberDamage );
//	MACROMGR->SetMacroMode( m_GameOption.nMacroMode );
//	*/
//	
////////////////////
////단축키 툴팁 변경
//	// 061220 LYW --- Delete this code.
//	/*
//	if( GAMEIN->GetMainInterfaceDialog() )
//		GAMEIN->GetMainInterfaceDialog()->Refresh();
//		*/
//
////////
////
//
////메시지보내기
////	if( bMsg )
////		SendOptionMsg();
//}

//void COptionManager::CancelSettings()
//{
//	//직접 영향 주었던 것들 제대로 돌려놓기
//}

//void COptionManager::SetDefaultOption()
//{
//	m_GameOption.bWhisper			= FALSE ;																//
//	m_GameOption.bTalk				= FALSE ;
//	m_GameOption.bBalloon			= TRUE ;
//	m_GameOption.bSystem			= TRUE ;
//	m_GameOption.bExperience1		= TRUE ;
//	m_GameOption.bItem1				= TRUE ;
//	m_GameOption.bMoney1			= TRUE ;
//	m_GameOption.bCombat1			= TRUE ;
//	m_GameOption.bEmoticon			= TRUE ;
//
//	m_GameOption.bNoWhisper			= FALSE;
//	m_GameOption.bNoChatting		= FALSE;
//	m_GameOption.bNoBalloon			= FALSE;
//	m_GameOption.bAutoHide			= FALSE;
//	m_GameOption.bNoShoutChat		= FALSE;
//	m_GameOption.bNoGuildChat		= FALSE;
//	m_GameOption.bNoAllianceChat	= FALSE;
//
//	m_GameOption.bNoSystemMsg		= FALSE;
//	m_GameOption.bNoExpMsg			= FALSE;
//	m_GameOption.bNoItemMsg			= FALSE;
//
//	m_GameOption.nGamma				= 50;	
//	m_GameOption.nSightDistance		= 75;	
//	m_GameOption.bGraphicCursor		= FALSE;
//	m_GameOption.bShadowHero		= TRUE;
//	m_GameOption.bShadowMonster		= TRUE;
//	m_GameOption.bShadowOthers		= TRUE;
//
//	m_GameOption.bAutoCtrl			= FALSE;
//	m_GameOption.bAmbientMax		= TRUE;
//	m_GameOption.nLODMode			= 0;	
//	m_GameOption.nEffectMode		= 0;	
//	m_GameOption.nEffectSnow        = 0;    
//
//	m_GameOption.bSoundBGM			= TRUE;
//	m_GameOption.bSoundEnvironment	= TRUE;
//	m_GameOption.nVolumnBGM			= 100;	
//	m_GameOption.nVolumnEnvironment	= 100;	
//}

//void COptionManager::SetDefaultOption()
//{
//	// 061204 LYW --- Change member variables name.
//	// befor modified.
//	/*
//	m_GameOption.bNoDeal			= FALSE;
//	m_GameOption.bNoParty			= FALSE;
//	m_GameOption.bNoFriend			= FALSE;
//	m_GameOption.bNoShowdown			= FALSE;
//	m_GameOption.bNameMunpa			= TRUE;
//	m_GameOption.bNameParty			= TRUE;
//	m_GameOption.bNameOthers		= TRUE;
//	m_GameOption.bNoMemberDamage	= TRUE;
//	m_GameOption.bNoGameTip			= TRUE;
//	m_GameOption.nMacroMode			= 1;	//MM_MACRO
//	*/
//	// after modified.
//	m_GameOption.bWhisper			= FALSE ;
//	m_GameOption.bTalk				= FALSE ;
//	m_GameOption.bBalloon			= TRUE ;
//	m_GameOption.bSystem			= TRUE ;
//	//m_GameOption.bNotify			= TRUE ;
//	m_GameOption.bExperience1		= TRUE ;
//	//m_GameOption.bExperience2		= TRUE ;
//	m_GameOption.bItem1				= TRUE ;
//	//m_GameOption.bItem2				= TRUE ;
//	m_GameOption.bMoney1			= TRUE ;
//	//m_GameOption.bMoney2			= TRUE ;
//	m_GameOption.bCombat1			= TRUE ;
//	//m_GameOption.bCombat2			= TRUE ;
//	m_GameOption.bEmoticon			= TRUE ;
//	// end modify.
//
//	m_GameOption.bNoWhisper			= FALSE;
//	m_GameOption.bNoChatting		= FALSE;
//	m_GameOption.bNoBalloon			= FALSE;
//	m_GameOption.bAutoHide			= FALSE;
//	m_GameOption.bNoShoutChat		= FALSE;
//	m_GameOption.bNoGuildChat		= FALSE;
//	m_GameOption.bNoAllianceChat	= FALSE;
//
//	m_GameOption.bNoSystemMsg		= FALSE;
//	m_GameOption.bNoExpMsg			= FALSE;
//	m_GameOption.bNoItemMsg			= FALSE;
//
//	m_GameOption.nGamma				= 50;	//%?
//	m_GameOption.nSightDistance		= 75;	//50~100%?
//	m_GameOption.bGraphicCursor		= FALSE;
//	m_GameOption.bShadowHero		= TRUE;
//	m_GameOption.bShadowMonster		= TRUE;
//	m_GameOption.bShadowOthers		= TRUE;
//
//
//	//SW050822
//	m_GameOption.bAutoCtrl			= FALSE;
//	//SW060904 Add MapAmbientCtrlOptn
//	m_GameOption.bAmbientMax		= TRUE;
//	m_GameOption.nLODMode			= 0;	//MODEL_LOD_USING_MODE_DEFAULT
//	m_GameOption.nEffectMode		= 0;	//eOO_EFFECT_DEFAULT
//	m_GameOption.nEffectSnow        = 0;    // Snow_EFFECT_DEFAULT (2005.12.28추가)
//
//
//	m_GameOption.bSoundBGM			= TRUE;
//	m_GameOption.bSoundEnvironment	= TRUE;
//	m_GameOption.nVolumnBGM			= 100;	//1-100
//	m_GameOption.nVolumnEnvironment	= 100;	//1-100
//
////	m_GameOption.wKyungGongIdx		= 0;
////	m_GameOption.bIsKyungGongMode	= FALSE;
//}

//void COptionManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
//{
//	switch(Protocol)
//	{
//	case MP_OPTION_SET_ACK:
//		{
//
//		}
//		break;
//
//	case MP_OPTION_SET_NACK:
//		{
//
//		}
//		break;
//	}
//}
/*
void COptionManager::SetKyungGongIdx(WORD KyungGondIdx)
{ 
	m_GameOption.wKyungGongIdx = KyungGondIdx;
	SaveGameOption();
}

BOOL COptionManager::GetKyungGongMode()
{
	return m_GameOption.bIsKyungGongMode;
}

void COptionManager::SetKyungGongMode(BOOL bMode)
{
	m_GameOption.bIsKyungGongMode = bMode;
	SaveGameOption();
}

void COptionManager::ClrKyungGong()
{
	m_GameOption.wKyungGongIdx = 0;
	m_GameOption.bIsKyungGongMode = FALSE;
	SaveGameOption();
}
*/


//void COptionManager::GraphicAutoCtrlProcess()
//{
//	// 기샹예보 2005.12.30 추가 
//	WEATHERMGR->SetWeather(m_GameOption.nEffectSnow);
//	if( !m_GameOption.bAutoCtrl ) return;
//
//	return;
//
//	DWORD Frame = 0;
//	if(gTickTime)
//	{
//		Frame = 1000 / gTickTime;
//		//10초간의 평균 프레임을 구해 기준 프레임과 비교하여 처리한다.
//		++m_dwCountFrame;
//		m_dwFrameAverage += Frame;
//	}
//	//Effect
//	//State counter
//	if( gCurTime - m_dwLastCheckTime > 10000 )
//	{
//		m_dwFrameAverage /= m_dwCountFrame;
//		if( STANDARD_FRAME > m_dwFrameAverage ) m_nCurOptionState--;
//		else if( NICE_FRAME < m_dwFrameAverage ) m_nCurOptionState++;
//		m_dwFrameAverage = m_dwCountFrame = 0;
//		if( m_nCurOptionState < LOWEST )
//			m_nCurOptionState = 0;
//		else if( m_nCurOptionState > HIGH )
//			m_nCurOptionState = 5;
//		m_dwLastCheckTime = gCurTime;
//	}
//
//	switch( m_nCurOptionState )
//	{
//	case LOWEST:
//		{
//			if( m_dwCheck == LOWEST ) return;
//			//시야거리 최저
//			m_GameOption.nSightDistance = m_SigntGBMinValue;
//			//LOD 낮추기
//			m_GameOption.nLODMode = TRUE;
//			//그림자 끄기
//			m_GameOption.bShadowHero = FALSE;
//			m_GameOption.bShadowMonster = FALSE;
//			m_GameOption.bShadowOthers = FALSE;
//			m_dwShdowOption = eOO_SHADOW_NONE;
//			//이펙트 끄기
//			m_GameOption.nEffectMode = TRUE;
//
//			
//			ApplyGraphicSetting();
//			
//			m_dwCheck = LOWEST;			
//		}
//		break;
//	case LOW:
//		{
//			if( m_dwCheck == LOW ) return;
//			//시야거리 최저
//			m_GameOption.nSightDistance = m_SigntGBMinValue + ( m_SigntGBMaxValue - m_SigntGBMinValue ) / 4;
//			//LOD 낮추기
//			m_GameOption.nLODMode = TRUE;
//			//그림자 낮추기
//			m_GameOption.bShadowHero = FALSE;
//			m_GameOption.bShadowMonster = FALSE;
//			m_GameOption.bShadowOthers = FALSE;
//			m_dwShdowOption = eOO_SHADOW_NONE;
//			//이펙트 켜기
//			m_GameOption.nEffectMode = FALSE;
//			
//			ApplyGraphicSetting();
//			
//			m_dwCheck = LOW;			
//		}
//		break;
//	case MID:
//		{
//			if( m_dwCheck == MID ) return;
//			//시야거리 최저
//			m_GameOption.nSightDistance = m_SigntGBMinValue + ( m_SigntGBMaxValue - m_SigntGBMinValue ) / 2;
//			//LOD	낮추기
//			m_GameOption.nLODMode = TRUE;
//			//그림자 켜기
//			m_GameOption.bShadowHero = TRUE;
//			m_GameOption.bShadowMonster = FALSE;
//			m_GameOption.bShadowOthers = FALSE;
//			m_dwShdowOption = eOO_SHADOW_ONLYHERO;
//			//이펙트 켜기
//			m_GameOption.nEffectMode = FALSE;
//			
//			ApplyGraphicSetting();
//			
//			m_dwCheck = MID;
//		}
//		break;
//	case HIGH:
//		{
//			if( m_dwCheck == HIGH ) return;
//			//시야거리 최고
//			m_GameOption.nSightDistance = m_SigntGBMaxValue;
//			//LOD 원상태
//			m_GameOption.nLODMode = FALSE;
//			//그림자 켜기
//			m_GameOption.bShadowHero = TRUE;
//			m_GameOption.bShadowMonster = TRUE;
//			m_GameOption.bShadowOthers = TRUE;
//			m_dwShdowOption = (eOO_SHADOW_ONLYHERO|eOO_SHADOW_MONSTER|eOO_SHADOW_OTHERCHARACTER);
//			//이펙트 켜기
//			m_GameOption.nEffectMode = FALSE;
//			
//			ApplyGraphicSetting();
//			
//			m_dwCheck = HIGH;			
//		}
//		break;
//
//	default:
//		return;
//	}
//}
//
//void COptionManager::ApplyGraphicSetting()
//{
//	CAMERA->SetSightDistanceRate( (float)m_GameOption.nSightDistance );
//	g_pExecutive->SetModelLODUsingMode( m_GameOption.nLODMode, 2 );
//	OBJECTMGR->SetShadowOption( m_dwShdowOption );
//	OBJECTMGR->SetEffectOption( m_GameOption.nEffectMode );
////	m_dwLowestCount = m_dwLowCount = m_dwMidCount = m_dwHighCount = 0;
//}
//
//void COptionManager::InitForGameIn()
//{
//	//SW050825 그래픽옵션Tab: GraphicAutoCtrlProcess()
//	m_pOptionDlg = (COptionDialog*)WINDOWMGR->GetWindowForID( OTI_TABDLG );
//	cDialog* pDlg = (cDialog*)m_pOptionDlg->GetTabSheet(2);
//	m_pCammaGB = (cGuageBar*)(pDlg->GetWindowForID( OTI_GB_GAMMA ));		//현재 적용은 안됨
//	m_pSightGB = (cGuageBar*)(pDlg->GetWindowForID( OTI_GB_SIGHT ));
//	m_SigntGBMinValue = m_pSightGB->GetMinValue();
//	m_SigntGBMaxValue = m_pSightGB->GetMaxValue();
//	m_dwShdowOption = eOO_SHADOW_NONE;
//	m_dwLastCheckTime = m_dwCheck = m_dwFrameAverage = m_dwCountFrame = 0;
//	m_nCurOptionState = HIGH;
//}

