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
#include "WeatherManager.h" // 2005.12.30�� �߰� �� On/Off
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
	ZeroMemory(&m_GameOption, sizeof(m_GameOption));														// ���� �ɼ��� ��� �ִ� ��� ����ü�� �ʱ�ȭ �Ѵ�.
}

COptionManager::~COptionManager()
{
}

void COptionManager::Init()
{
	if( !LoadGameOption() )																					// ���� �ɼ��� �ε��Ѵ�. �����ϸ�,
	{
		SetDefaultOption();																					// �⺻ �ɼ��� �����Ѵ�.
	}

	m_bBGMSoundChanged	= FALSE;																			// ��������� �ٲ������ ���θ� FALSE�� �����Ѵ�.
	
	AUDIOMGR->SetBGMSoundOn( m_GameOption.bSoundBGM );														// �ɼ� ���� ���� ��� ������ ON/OFF �� �����Ѵ�.

	m_wSendOption		= 0;																				// ��� �ɼ� �÷��� ���� 0���� �����Ѵ�.
	
	m_bShowMemberDamage = TRUE;																				// ��Ƽ�� ������ ���⸦ TRUE�� �����Ѵ�.

	// 071026 LYW --- OptionManager : Setting option of tutorial.
	if( m_GameOption.bShowTutorial )
	{
		TUTORIALMGR->Set_ActiveTutorial(TRUE) ;							// Ʃ�丮���� ��Ȱ��ȭ ���·� �����Ѵ�.
	}
	else
	{
		TUTORIALMGR->Set_ActiveTutorial(FALSE) ;							// Ʃ�丮���� ��Ȱ��ȭ ���·� �����Ѵ�.
	}
}

void COptionManager::SendOptionMsg()
{
	WORD wOption = MakeOptionFlag();																		// ���õǾ� �ִ� �ź� ���� �ɼǰ��� �޴´�.

	m_wSendOption = wOption;																				// �ɼ� �÷��� ���� ��� �÷��׷� �޴´�.

	MSG_WORD msgOption;																						// WORD�� ������ �ִ� �޽��� ����ü�� �����Ѵ�.
	msgOption.Category		= MP_OPTION;																	// ī�װ��� �ɼ����� �����Ѵ�.
	msgOption.Protocol		= MP_OPTION_SET_SYN;															// ���������� �ɼ� ���� ��ũ�� �����Ѵ�.
	msgOption.dwObjectID	= HEROID;																		// ������Ʈ ���̵� ���� ĳ���� ���̵�� �����Ѵ�.
	msgOption.wData			= wOption;																		// WORD�� �����Ϳ� �ɼ� �÷��׸� �����Ѵ�.

	NETWORK->Send( &msgOption, sizeof( MSG_WORD ) );														// ������ ���� ���õ� ������ �����Ѵ�.
}

WORD COptionManager::MakeOptionFlag()
{
	WORD wOption = 0;																						// �ɼǰ��� ���� ����.

	if( m_GameOption.bNoDeal )		wOption |= eOPT_NODEAL;													// �ŷ� �źΰ� üũ�Ǿ� ������ �߰��Ѵ�.
	if( m_GameOption.bNoParty )		wOption |= eOPT_NOPARTY;												// ��Ƽ �źΰ� üũ�Ǿ� ������ �߰��Ѵ�.
	if( m_GameOption.bNoFriend )	wOption |= eOPT_NOFRIEND;												// ģ����� �źΰ� üũ�Ǿ� ������ �߰��Ѵ�.
	if( m_GameOption.bNoShowdown )	wOption |= eOPT_NOSHOWDOWN;												// ���� �źΰ� üũ�Ǿ� ������ �߰��Ѵ�.
	if( m_GameOption.bNoWhisper )	wOption |= eOPT_NOWHISPER;												// �ӼӸ� �źΰ� üũ�Ǿ� ������ �߰��Ѵ�.

	return wOption;																							// ���õ� �ɼǰ��� �����Ѵ�.
}

void COptionManager::SetGameOption( sGAMEOPTION* pOption )
{
	if( m_GameOption.bSoundBGM != pOption->bSoundBGM )														// �Է� ���� ������� ����, ����� ���� ���� ������,
	{
		m_bBGMSoundChanged = TRUE;																			// ��������� �ٲ������ ���θ� TRUE�� �����Ѵ�.
	}

	m_GameOption = *pOption;																				// ����� �ɼ� ����ü�� �Է¹��� ����ü�� �����Ѵ�.

	SaveGameOption();																						// �ɼ��� �����Ѵ�.
}

BOOL COptionManager::LoadGameOption()
{
	HANDLE	hFile;																							// ���� �ڵ��� �����Ѵ�.

	hFile = CreateFile( "./system/GameOption.opt", GENERIC_READ, 0, NULL,									// ������ ���Ϸ� �ڵ��� �޴´�.(�б���)
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE )																		// �ڵ� �ޱ⿡ �����ϸ�,
		return FALSE;																						// ���� ó���� �Ѵ�.

	DWORD dwRead = 0;																						// ������ ���� ����Ʈ ���� ���Ϲޱ� ���� ��¿� �μ�.

	DWORD dwVersion;																						// �о���̴� �����͸� ������ ����.

	if( !ReadFile( hFile, &dwVersion, sizeof(DWORD), &dwRead, NULL ) )										// ���� �ڵ��� ������ �����͸� �д´�. �����ϸ�,
	{
		CloseHandle( hFile );																				// ���� �ڵ��� �ݴ´�.
		return FALSE;																						// ���� ó���� �Ѵ�.
	}
	if( dwVersion != OPTIONVERSION )																		// ������ �־��� ���� ���� ������,
	{
//		SetDefaultOption() ;																				// �⺻ �ɼ��� �����Ѵ�.
//		ApplySettings() ;																					// �⺻ �ɼ��� �����Ѵ�.

		CloseHandle( hFile );																				// ���� �ڵ��� �ݴ´�.
		return FALSE;																						// ���� ó���� �Ѵ�.
	}

	if( !ReadFile( hFile, &m_GameOption, sizeof(m_GameOption), &dwRead, NULL ) )							// ���� �ɼ� ����ü ��ŭ ��� ����ü�� �����͸� �޴´�. �����ϸ�,
	{
		CloseHandle( hFile );																				// ���� �ڵ��� �ݴ´�.
		return FALSE;																						// ���� ó���� �Ѵ�.
	}
	
	if( sizeof( m_GameOption ) != dwRead )																	// �о���� ����Ʈ���� ���Ѵ�. ���� ������,
	{
		CloseHandle( hFile );																				// ���� �ڵ��� �ݴ´�.
		return FALSE;																						// ���� ó���� �Ѵ�.
	}

	CloseHandle( hFile );																					// �ڵ��� �ݴ´�.
	return TRUE;																							// ���� ó���� �Ѵ�.
}

void COptionManager::SaveGameOption()
{
	HANDLE	hFile;																							// ���� �ڵ��� �����Ѵ�.

	hFile=CreateFile( "./system/GameOption.opt", GENERIC_WRITE, 0, NULL,									// ������ ���Ϸ� �ڵ��� �޴´�.(������)
					  CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE )																		// �ڵ� �ޱ⿡ �����ϸ�,
		return;																								// ���� ó���� �Ѵ�.
	
	DWORD dwWritten;																						// ������ ����� ����Ʈ ���� ���Ϲޱ� ���� ����.
	DWORD dwVersion = OPTIONVERSION ;																		// ������ �����Ѵ�.
	WriteFile( hFile, &dwVersion, sizeof(DWORD), &dwWritten, NULL );										// ������ ����Ѵ�.
	WriteFile( hFile, &m_GameOption, sizeof(m_GameOption), &dwWritten, NULL );								// �ɼǱ���ü ��ŭ ����Ѵ�.
	
	CloseHandle( hFile );																					// �ڵ��� �ݴ´�.
}

void COptionManager::ApplySettings()
{
	ApplySound() ;																							// ���� ��Ʈ�� �����Ѵ�.
	
	ApplyGraphics() ;																						// �׷��� ��Ʈ�� �����Ѵ�.

	ApplyChatting() ;																						// ä�� ��Ʈ�� �����Ѵ�.

	ApplyEnvironmemt() ;																					// ȯ�� ��Ʈ�� �����Ѵ�.
}

void COptionManager::ApplySound()
{
	if( m_bBGMSoundChanged )																				// ��������� ���濩�ΰ� TRUE �̸�,
	{
		AUDIOMGR->SetBGMSoundOn( m_GameOption.bSoundBGM );													// �ɼǿ� ��� ������ ��������� ON/OFF �����Ѵ�.

		if( m_GameOption.bSoundBGM )																		// ������� �ɼǰ��� ON�̸�, 
		{
			AUDIOMGR->PlayLastBGM();																		// ����� �Ŵ����� ������ ��������� �÷��� ��Ų��.
		}
		else																								// ������� ���ǰ��� OFF �̸�, 
		{
			AUDIOMGR->StopBGM();																			// ����� �Ŵ����� ��������� ���� ��Ų��.
		}

		m_bBGMSoundChanged = FALSE;																			// ��������� ���濩�θ� FALSE�� �����Ѵ�.
	}

	if( m_GameOption.bSoundBGM )	//bgm�� play���� �ƴҶ� ���������ϸ� ������.							// ��������� �÷��� ���̸�,
	{
		AUDIOMGR->SetBGMVolume( (float)m_GameOption.nVolumnBGM / 100.0f );									// �ɼ��� BGM Volume�� ������� �ؼ� ������� ������ �����Ѵ�.
	}
	
	AUDIOMGR->SetEffectSoundOn( m_GameOption.bSoundEnvironment );											// �ɼ��� ���� ���� ���� ����Ʈ ���带 ON/OFF �Ѵ�.

	if( m_GameOption.bSoundEnvironment )																	// �ɼ��� ���� ���� TRUE �̸�,
	{
		AUDIOMGR->SetSoundVolume( (float)m_GameOption.nVolumnEnvironment / 100.0f ) ;						// �ɼ��� �������� ������� �ؼ� ���� ������ �����Ѵ�.
	}
	else																									// �ɼ��� ���尪�� FALSE �̸�,
	{
		AUDIOMGR->StopAll();																				// ��� �����Ų��.
	}
}

void COptionManager::ApplyGraphics()
{
	CURSOR->SetActive( !m_GameOption.bGraphicCursor );														// �ɼ��� Ŀ�� ���� ���� Ŀ���� ON/OFF �Ѵ�.

	CAMERA->SetSightDistanceRate( (float)m_GameOption.nSightDistance );										// �þ� �Ÿ����� �����Ѵ�.

	// 070723 LYW --- OptionManager : ���׸� ������.
	//MAPDESC* pmapdesc = MAP->GetMapDesc();																	// �� ��ũ��Ʈ ������ �޾ƿ´�.

	//if( pmapdesc )																							// ������ ��ȿ���� üũ�Ѵ�.
	//{
	//	// ī�޶� ������ ���� �޾ƿ´�.
	//	float fFogDistance = pmapdesc->DefaultSight * (float)(100-(100-(float)m_GameOption.nSightDistance)*1.5) / 100.0f ;

	//	pmapdesc->fogdesc.fEnd = fFogDistance * 0.88f ;														// �Ȱ��� �� ������ �����Ѵ�.
	//	pmapdesc->fogdesc.fStart = fFogDistance * 0.22f ;													// �Ȱ��� ���� ������ �����Ѵ�.

	//	g_pExecutive->GetGeometry()->EnableFog(0);															
	//	g_pExecutive->GetGeometry()->SetFog(&pmapdesc->fogdesc,0);											// �Ȱ��� �����Ѵ�.
	//}

	DWORD dwShadowOption = eOO_SHADOW_NONE;																	// �׸��� �ɼ��� ���� ������ ���� �� �ʱ�ȭ �Ѵ�.

	if( m_GameOption.bShadowHero )		dwShadowOption |= eOO_SHADOW_ONLYHERO;								// �ڽ��� üũ�Ǿ� ������ �߰��Ѵ�.
	if( m_GameOption.bShadowMonster )	dwShadowOption |= eOO_SHADOW_MONSTER;								// ���ͳ� NPC�� üũ�Ǿ� ������ �߰��Ѵ�.
	if( m_GameOption.bShadowOthers )	dwShadowOption |= eOO_SHADOW_OTHERCHARACTER;						// �ٸ�ĳ���Ͱ� üũ�Ǿ� ������ �߰��Ѵ�.

	OBJECTMGR->SetShadowOption( dwShadowOption );															// ������Ʈ �Ŵ����� �ɼ��� �����Ѵ�.

	//SW ��ȹ, �׷��� ������ 0������ �ּ� ���ؽ����� ���⼭�� 2 ������ �ּ��̴�.
	g_pExecutive->SetModelLODUsingMode( m_GameOption.nLODMode, 2 ) ;										// LOD ����带 �����Ѵ�.
	//Effect
	OBJECTMGR->SetEffectOption( m_GameOption.nEffectMode ) ;												// ����Ʈ ��带 �����Ѵ�.
}

void COptionManager::ApplyChatting()
{	
	sChatOption ChatOption;

	ChatOption.bOption[CTO_NOWHISPERMSG]	= m_GameOption.bNoWhisper ;										// �ӼӸ� �źθ� �����Ѵ�.
	ChatOption.bOption[CTO_NOCHATMSG]		= m_GameOption.bNoChatting ;									// �Ϲ�ä�� ���⸦ �����Ѵ�.
	ChatOption.bOption[CTO_NOSHOUTMSG]		= m_GameOption.bNoShoutChat ;									// ��ġ�� ���⸦ �����Ѵ�.
	ChatOption.bOption[CTO_NOPARTYMSG]		= m_GameOption.bNoPartyChat ;									// ��Ƽ ä�� ���⸦ �����Ѵ�.
	ChatOption.bOption[CTO_NOFAMILYMSG]		= m_GameOption.bNoFamilyChat ;									// �йи� ä�� ���⸦ �����Ѵ�.
	ChatOption.bOption[CTO_NOGUILDMSG]		= m_GameOption.bNoGuildChat ;									// ��� ä�� ���⸦ �����Ѵ�.
	ChatOption.bOption[CTO_NOALLIANCEMSG]	= m_GameOption.bNoAllianceChat ;								// ���� ä�� ���⸦ �����Ѵ�.
	ChatOption.bOption[CTO_NOSYSMSG]		= m_GameOption.bNoSystemMsg ;									// �ý��� �޽��� ���⸦ �����Ѵ�.
	ChatOption.bOption[CTO_NOEXPMSG]		= m_GameOption.bNoExpMsg ;										// ����ġ ȹ�� �޽��� ���⸦ �����Ѵ�.
	ChatOption.bOption[CTO_NOITEMMSG]		= m_GameOption.bNoItemMsg ;										// ������ ȹ�� �޽��� ���⸦ �����Ѵ�.

	CHATMGR->SetOption( &ChatOption );																		// ä�� �Ŵ����� �ɼ��� �����Ѵ�.
}

void COptionManager::ApplyEnvironmemt()
{
	DWORD dwOverInfoOption = eOO_NAME_CLICKEDONLY ;	//---default											// ���� ���� �ɼ��� ���� ���� ����.

	if( m_GameOption.bNameOthers )	dwOverInfoOption |= eOO_NAME_WHOLE ;									// �ٸ� ĳ���� �̸�ǥ�ø� �߰��Ѵ�.
	if( m_GameOption.bNoMemberDamage )	dwOverInfoOption |= eOO_DAMAGE_MEMBERS ;							// ��Ƽ�� ������ ���⸦ �߰��Ѵ�.
	if( m_GameOption.bNameParty )	dwOverInfoOption |= eOO_NAME_PARTY ;									// ��Ƽ�� �̸� ǥ�ø� �߰��Ѵ�.
	if( m_GameOption.bNameMunpa )	dwOverInfoOption |= eOO_NAME_MUNPA ;									// ���� �̸� ǥ�ø� �߰��Ѵ�.
	if( m_GameOption.bNoBalloon )	dwOverInfoOption |= eOO_CHAT_NOBALLOON ;								// ��ǳ���� TRUE�� �߰��Ѵ�.

	OBJECTMGR->SetOverInfoOption( dwOverInfoOption ) ;														// ������Ʈ �Ŵ����� ���� ���� �ɼ��� �����Ѵ�.

	CHATTIPMGR->SetActive( m_GameOption.bNoGameTip ) ;														// ���� �� ���⸦ �����Ѵ�.

	OPTIONMGR->SetMemberDemageActive( m_GameOption.bNoMemberDamage ) ;										// ��Ƽ�� ������ ���⸦ �����Ѵ�.

	MACROMGR->SetMacroMode( m_GameOption.nMacroMode ) ;														// ä�ÿ켱, ����Ű �켱��带 �����Ѵ�.
}

void COptionManager::CancelSettings()
{
	//���� ���� �־��� �͵� ����� ��������
}

void COptionManager::SetDefaultOption()
{
	m_GameOption.bNoDeal 			= FALSE;																// �ŷ� �ź�.
	m_GameOption.bNoParty 			= FALSE;																// ��Ƽ �ź�.
	m_GameOption.bNoFriend 			= FALSE;																// ģ����� �ź�.
	m_GameOption.bNoShowdown 		= FALSE;																// ���� �ź�.

	m_GameOption.bNameMunpa 		= TRUE;																	// ���� �̸� ǥ��.
	m_GameOption.bNameParty 		= TRUE;																	// ��Ƽ�� �̸� ǥ��.
	m_GameOption.bNameOthers 		= TRUE;																	// �ٸ� ĳ���� �̸� ǥ��.
	m_GameOption.bNoMemberDamage 	= TRUE;																	// ��Ƽ�� ������ ����.
	m_GameOption.bNoGameTip 		= TRUE;																// ���� �� ����.

	m_GameOption.nMacroMode 		= 1 ;																	// ä�ÿ켱���/����Ű�켱���.

	m_GameOption.bNoWhisper			= FALSE;																// �ӼӸ� �ź�.
	m_GameOption.bNoChatting		= FALSE;																// �Ϲ�ä�� ����.
	m_GameOption.bNoBalloon			= FALSE;																// ��ǳ�� ����.
	m_GameOption.bNoPartyChat		= FALSE ;																// ��Ƽ ä�� ����.
	m_GameOption.bNoFamilyChat		= FALSE ;																// �йи� ä�� ����.
	m_GameOption.bNoShoutChat		= FALSE;																// ��ġ�� ����.
	m_GameOption.bNoGuildChat		= FALSE;																// ���ä�� ����.
	m_GameOption.bNoAllianceChat	= FALSE;																// ����ä�� ����.

	m_GameOption.bNoSystemMsg		= FALSE;																// �ý��۸޽��� ����.
	m_GameOption.bNoExpMsg			= FALSE;																// ����ġ.����ġ ȹ��޽��� ����.
	m_GameOption.bNoItemMsg			= FALSE;																// ��/������ ȹ��޽��� ����.

	m_GameOption.nSightDistance		= 155;//m_SigntGBMaxValue-50;													// �þ߰Ÿ� ������.
	m_GameOption.bGraphicCursor		= FALSE;																// Ŀ�� �¿���?
	m_GameOption.bShadowHero		= TRUE;																	// �׸��� ǥ�� �ڽ�.
	m_GameOption.bShadowMonster		= TRUE;																	// �׸��� ǥ�� ����/NPC.
	m_GameOption.bShadowOthers		= TRUE;																	// �׸��� ǥ�� �ٸ�ĳ����.

	m_GameOption.bAutoCtrl			= FALSE;																// ���� ȭ�� �ڵ� ����.
	m_GameOption.bAmbientMax		= TRUE;																	// ĳ���� ���?
	m_GameOption.nLODMode			= 0;																	// �׷��� ����.
	m_GameOption.nEffectMode		= 0;																	// ����Ʈ ���.
	m_GameOption.nEffectSnow        = 0;																	// �� ����Ʈ ����.

	m_GameOption.bSoundBGM			= TRUE;																	// ������� ON/OFF.
	m_GameOption.bSoundEnvironment	= TRUE;																	// ȿ���� ON/OFF.
	m_GameOption.nVolumnBGM			= 100;																	// ������� ������.
	m_GameOption.nVolumnEnvironment	= 100;																	// ȿ���� ������.

	// 071025 LYW --- OptionManager : Setting tutorial option to default.
	m_GameOption.bShowTutorial		= TRUE ;																// �⺻ Ʃ�丮�� ���̱�� ����.
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
	WEATHERMGR->SetWeather(m_GameOption.nEffectSnow);														// ���� �ɼǰ����� ������ �����Ѵ�.

	if( !m_GameOption.bAutoCtrl ) return;																	// �ڵ� �������� FALSE�� ����ó���� �Ѵ�.

	return;																									// ���� �Ϻη� ����ó�� �ѵ�.

	DWORD Frame = 0;																						// �������� ���� ����.

	//10�ʰ��� ��� �������� ���� ���� �����Ӱ� ���Ͽ� ó���Ѵ�.
	if(gTickTime)																							// ƽ ������ ��ȿ���� üũ�Ѵ�.
	{
		Frame = 1000 / gTickTime;																			// ������ ���� �޴´�.											
		++m_dwCountFrame;																					// ������ ī��Ʈ�� �����Ѵ�.
		m_dwFrameAverage += Frame;																			// ������ ��հ��� ������ ���� ���Ѵ�.
	}
	//Effect
	//State counter
	if( gCurTime - m_dwLastCheckTime > 10000 )																// 10�ʰ� �귶����,
	{
		m_dwFrameAverage /= m_dwCountFrame;																	// ��հ��� ������ ī��Ʈ�� ������ �޴´�.

		if( STANDARD_FRAME > m_dwFrameAverage )																// ǥ�� �����Ӻ��� ����������� ������,
		{
			m_nCurOptionState--;																			// ���� �ɼ� ���¸� ���ҽ�Ų��.
		}
		else if( NICE_FRAME < m_dwFrameAverage )															// ���̽� �����Ӻ��� ��� �������� ������,
		{
			m_nCurOptionState++;																			// ���� �ɼ� ���¸� ���� ��Ų��.
		}

		m_dwFrameAverage = m_dwCountFrame = 0;																// ��� �����Ӱ�, ������ ī��Ʈ�� 0���� �����Ѵ�.

		if( m_nCurOptionState < LOWEST )																	// ���� �ɼ� ���°� �־Ǻ��� ���ϸ�, 
		{
			m_nCurOptionState = 0;																			// ���� �ɼ� ���¸� 0���� �����Ѵ�.
		}
		else if( m_nCurOptionState > HIGH )																	// ���� �ɼ� ���°� ������ ���� ũ��,
		{
			m_nCurOptionState = 5;																			// ���� �ɼ� ���¸� 5�� �����Ѵ�.
		}

		m_dwLastCheckTime = gCurTime;																		// ������ üũ�ð��� �����Ѵ�.
	}

	switch( m_nCurOptionState )																				// ���� �ɼ� ���¸� Ȯ���Ѵ�.
	{
	case LOWEST:																							// �־��̶��,
		{
			if( m_dwCheck == LOWEST ) return;																// m_dwCheck�� LOWEST�� ������ ���� ó���� �Ѵ�.

			m_GameOption.nSightDistance = m_SigntGBMinValue;												//�þ߰Ÿ��� ������ �����Ѵ�.
			
			m_GameOption.nLODMode = TRUE;																	//LOD ���߱�.
			
			m_GameOption.bShadowHero = FALSE;																// �׸��� ǥ�� - �ڽ��� ����.
			m_GameOption.bShadowMonster = FALSE;															// �׸��� ǥ�� - ����/NPC�� ����.
			m_GameOption.bShadowOthers = FALSE;																// �׸��� ǥ�� - �ٸ� ĳ���͸� ����.

			m_dwShdowOption = eOO_SHADOW_NONE;																// �׸��� �ɼ��� ����.
			
			m_GameOption.nEffectMode = TRUE;																//����Ʈ�� ����.
			
			ApplyGraphicSetting();																			// �׷��� ������ �����Ѵ�.
			
			m_dwCheck = LOWEST;																				// m_dwCheck�� LOWEST�� �����Ѵ�.	
		}
		break;

	case LOW:																								// ���� ���¶��,
		{
			if( m_dwCheck == LOW ) return;																	// m_dwCheck�� LOW�� ������ ���� ó���� �Ѵ�.

			m_GameOption.nSightDistance = m_SigntGBMinValue + ( m_SigntGBMaxValue - m_SigntGBMinValue ) / 4;//�þ߰Ÿ��� �����Ѵ�.
			
			m_GameOption.nLODMode = TRUE;																	//LOD ���߱�.
			
			m_GameOption.bShadowHero = FALSE;																// �׸��� ǥ�� - �ڽ��� ����.
			m_GameOption.bShadowMonster = FALSE;															// �׸��� ǥ�� - ����/NPC�� ����.
			m_GameOption.bShadowOthers = FALSE;																// �׸��� ǥ�� - �ٸ� ĳ���͸� ����.

			m_dwShdowOption = eOO_SHADOW_NONE;																// �׸��� �ɼ��� ����.
			
			m_GameOption.nEffectMode = FALSE;																//����Ʈ�� �Ҵ�.
			
			ApplyGraphicSetting();																			// �׷��� ������ �����Ѵ�.
			
			m_dwCheck = LOW;																				// m_dwCheck�� LOW�� �����Ѵ�.	
		}
		break;

	case MID:																								// ������ ���̶��, 
		{
			if( m_dwCheck == MID ) return;																	// m_dwCheck�� LOW�� ������ ���� ó���� �Ѵ�.
	
			m_GameOption.nSightDistance = m_SigntGBMinValue + ( m_SigntGBMaxValue - m_SigntGBMinValue ) / 2;// �þ߰Ÿ��� �����Ѵ�.
			
			m_GameOption.nLODMode = TRUE;																	//LOD	���߱�.
			
			m_GameOption.bShadowHero = TRUE;																// �׸��� ǥ�� - �ڽ��� �Ҵ�.
			m_GameOption.bShadowMonster = FALSE;															// �׸��� ǥ�� - ����/NPC�� ����.
			m_GameOption.bShadowOthers = FALSE;																// �׸��� ǥ�� - �ٸ� ĳ���͸� ����.

			m_dwShdowOption = eOO_SHADOW_ONLYHERO;															// �׸��� �ɼ��� �ڽŸ� �Ҵ�.
			
			m_GameOption.nEffectMode = FALSE;																// ����Ʈ�� �Ҵ�.
			
			ApplyGraphicSetting();																			// �׷��� ������ �����Ѵ�.
			
			m_dwCheck = MID;																				// m_dwCheck�� MID�� �����Ѵ�.	
		}
		break;

	case HIGH:																								// �������̶��,
		{
			if( m_dwCheck == HIGH ) return;																	// m_dwCheck�� HIGH�� ������ ���� ó���� �Ѵ�.
		
			m_GameOption.nSightDistance = m_SigntGBMaxValue;												// �þ߰Ÿ��� �ְ�� �����Ѵ�.
		
			m_GameOption.nLODMode = FALSE;																	//LOD �����·� �Ѵ�.
			
			m_GameOption.bShadowHero = TRUE;																// �׸��� ǥ�� - �ڽ��� �Ҵ�.
			m_GameOption.bShadowMonster = TRUE;																// �׸��� ǥ�� - ����/NPC�� �Ҵ�.
			m_GameOption.bShadowOthers = TRUE;																// �׸��� ǥ�� - �ٸ� ĳ���͸� �Ҵ�.

			m_dwShdowOption = (eOO_SHADOW_ONLYHERO|eOO_SHADOW_MONSTER|eOO_SHADOW_OTHERCHARACTER);			// �׸��� �ɼ��� ��� �Ҵ�.

			m_GameOption.nEffectMode = FALSE;																// ����Ʈ�� �Ҵ�.
			
			ApplyGraphicSetting();																			// �׷��� ������ �����Ѵ�.
			
			m_dwCheck = HIGH;																				// m_dwCheck�� HIGH�� �����Ѵ�.	
		}
		break;

	default:
		return;
	}
}

void COptionManager::ApplyGraphicSetting()
{
	CAMERA->SetSightDistanceRate( (float)m_GameOption.nSightDistance );										// ī�޶��� �þ߰Ÿ��� �����Ѵ�.
	g_pExecutive->SetModelLODUsingMode( m_GameOption.nLODMode, 2 );											// LOD����带 �����Ѵ�.
	OBJECTMGR->SetShadowOption( m_dwShdowOption );															// �׸��� �ɼ��� �����Ѵ�.
	OBJECTMGR->SetEffectOption( m_GameOption.nEffectMode );													// ����Ʈ �ɼ��� �����Ѵ�.
}

void COptionManager::InitForGameIn()
{
	m_pOptionDlg = (COptionDialog*)WINDOWMGR->GetWindowForID( OTI_TABDLG );									// �ɼ� ���̾�α� ������ �޴´�.
	cDialog* pDlg = (cDialog*)m_pOptionDlg->GetTabSheet(2);													// ����° ��Ʈ�� �޴´�.
	m_pCammaGB = (cGuageBar*)(pDlg->GetWindowForID( OTI_GB_GAMMA ));										// ��� ���� �������� ������ �޴´�.
	m_pSightGB = (cGuageBar*)(pDlg->GetWindowForID( OTI_GB_SIGHT ));										// �þ߰Ÿ� ���� �������� ������ �޴´�.
	m_SigntGBMinValue = m_pSightGB->GetMinValue();															// �þ߰Ÿ� �ּҰ��� �޴´�.
	m_SigntGBMaxValue = m_pSightGB->GetMaxValue();															// �þ߰Ÿ� �ִ밪�� �޴´�.
	m_dwShdowOption = eOO_SHADOW_NONE;																		// �׸��� �ɼ��� �ƹ��͵� ���°����� �����Ѵ�.
	m_dwLastCheckTime = m_dwCheck = m_dwFrameAverage = m_dwCountFrame = 0;									// �ð����� �������� 0���� �����Ѵ�.
	m_nCurOptionState = HIGH;																				// ���� �ɼ� ���¸� HIGH�� �����Ѵ�.
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
//	// desc_hseos_�ɼ�01
//	// S �ɼ� �߰� added by hseos 2007.06.19
//	m_GameOption.nChatLineNum = 0;
//	m_GameOption.nCurChatMode = 0;
//	// E �ɼ� �߰� added by hseos 2007.06.19
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
////���峪 ����, �þ߰Ÿ����� ��ġ�� ��ȿ���������� üũ�ϴ� ���� �߰��ؾ��Ѵ�.
////��ȿ������ �ƴϸ� ��� �ʱ�ȭ!	//CONFIRM
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
////����
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
//	if( m_GameOption.bSoundBGM )	//bgm�� play���� �ƴҶ� ���������ϸ� ������.
//		AUDIOMGR->SetBGMVolume( (float)m_GameOption.nVolumnBGM / 100.0f );
//	
//	AUDIOMGR->SetEffectSoundOn( m_GameOption.bSoundEnvironment );
//	if( m_GameOption.bSoundEnvironment )
//		AUDIOMGR->SetSoundVolume( (float)m_GameOption.nVolumnEnvironment / 100.0f );
//	else
//		AUDIOMGR->StopAll();
//	
//////////
////�׷���
//
////-���� ����
////-�̸�ǥ�� �ɼ� 
////-�׷���Ŀ��
//	CURSOR->SetActive( !m_GameOption.bGraphicCursor );
////	MOUSE->SetGraphicCursor( m_GameOption.bGraphicCursor );
////	CURSOR->SetActive( TRUE );//MouseTest��
//	//taiyo test
//	//WINDOWMGR->GetMouseWindow()->SetActive( m_GameOption.bGraphicCursor );
//
////�þ߰Ÿ�
//	//CAMERA->SetSightDistanceRate( (float)m_GameOption.nSightDistance );
//	CAMERA->SetSightDistanceRate( (float)100 );
//
////�׸��� ��¿���
//	DWORD dwShadowOption = eOO_SHADOW_NONE;
//	if( m_GameOption.bShadowHero )		dwShadowOption |= eOO_SHADOW_ONLYHERO;
//	if( m_GameOption.bShadowMonster )	dwShadowOption |= eOO_SHADOW_MONSTER;
//	if( m_GameOption.bShadowOthers )	dwShadowOption |= eOO_SHADOW_OTHERCHARACTER;
//	OBJECTMGR->SetShadowOption( dwShadowOption );
//
////�̸�	
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
////ä��
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
//	g_pExecutive->SetModelLODUsingMode( m_GameOption.nLODMode, 2 );	//SW ��ȹ, �׷��� ������ 0������ �ּ� ���ؽ����� ���⼭�� 2 ������ �ּ��̴�.
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
////����Ű ���� ����
//	// 061220 LYW --- Delete this code.
//	/*
//	if( GAMEIN->GetMainInterfaceDialog() )
//		GAMEIN->GetMainInterfaceDialog()->Refresh();
//		*/
//
////////
////
//
////�޽���������
////	if( bMsg )
////		SendOptionMsg();
//}

//void COptionManager::CancelSettings()
//{
//	//���� ���� �־��� �͵� ����� ��������
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
//	m_GameOption.nEffectSnow        = 0;    // Snow_EFFECT_DEFAULT (2005.12.28�߰�)
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
//	// �⼧���� 2005.12.30 �߰� 
//	WEATHERMGR->SetWeather(m_GameOption.nEffectSnow);
//	if( !m_GameOption.bAutoCtrl ) return;
//
//	return;
//
//	DWORD Frame = 0;
//	if(gTickTime)
//	{
//		Frame = 1000 / gTickTime;
//		//10�ʰ��� ��� �������� ���� ���� �����Ӱ� ���Ͽ� ó���Ѵ�.
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
//			//�þ߰Ÿ� ����
//			m_GameOption.nSightDistance = m_SigntGBMinValue;
//			//LOD ���߱�
//			m_GameOption.nLODMode = TRUE;
//			//�׸��� ����
//			m_GameOption.bShadowHero = FALSE;
//			m_GameOption.bShadowMonster = FALSE;
//			m_GameOption.bShadowOthers = FALSE;
//			m_dwShdowOption = eOO_SHADOW_NONE;
//			//����Ʈ ����
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
//			//�þ߰Ÿ� ����
//			m_GameOption.nSightDistance = m_SigntGBMinValue + ( m_SigntGBMaxValue - m_SigntGBMinValue ) / 4;
//			//LOD ���߱�
//			m_GameOption.nLODMode = TRUE;
//			//�׸��� ���߱�
//			m_GameOption.bShadowHero = FALSE;
//			m_GameOption.bShadowMonster = FALSE;
//			m_GameOption.bShadowOthers = FALSE;
//			m_dwShdowOption = eOO_SHADOW_NONE;
//			//����Ʈ �ѱ�
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
//			//�þ߰Ÿ� ����
//			m_GameOption.nSightDistance = m_SigntGBMinValue + ( m_SigntGBMaxValue - m_SigntGBMinValue ) / 2;
//			//LOD	���߱�
//			m_GameOption.nLODMode = TRUE;
//			//�׸��� �ѱ�
//			m_GameOption.bShadowHero = TRUE;
//			m_GameOption.bShadowMonster = FALSE;
//			m_GameOption.bShadowOthers = FALSE;
//			m_dwShdowOption = eOO_SHADOW_ONLYHERO;
//			//����Ʈ �ѱ�
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
//			//�þ߰Ÿ� �ְ�
//			m_GameOption.nSightDistance = m_SigntGBMaxValue;
//			//LOD ������
//			m_GameOption.nLODMode = FALSE;
//			//�׸��� �ѱ�
//			m_GameOption.bShadowHero = TRUE;
//			m_GameOption.bShadowMonster = TRUE;
//			m_GameOption.bShadowOthers = TRUE;
//			m_dwShdowOption = (eOO_SHADOW_ONLYHERO|eOO_SHADOW_MONSTER|eOO_SHADOW_OTHERCHARACTER);
//			//����Ʈ �ѱ�
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
//	//SW050825 �׷��ȿɼ�Tab: GraphicAutoCtrlProcess()
//	m_pOptionDlg = (COptionDialog*)WINDOWMGR->GetWindowForID( OTI_TABDLG );
//	cDialog* pDlg = (cDialog*)m_pOptionDlg->GetTabSheet(2);
//	m_pCammaGB = (cGuageBar*)(pDlg->GetWindowForID( OTI_GB_GAMMA ));		//���� ������ �ȵ�
//	m_pSightGB = (cGuageBar*)(pDlg->GetWindowForID( OTI_GB_SIGHT ));
//	m_SigntGBMinValue = m_pSightGB->GetMinValue();
//	m_SigntGBMaxValue = m_pSightGB->GetMaxValue();
//	m_dwShdowOption = eOO_SHADOW_NONE;
//	m_dwLastCheckTime = m_dwCheck = m_dwFrameAverage = m_dwCountFrame = 0;
//	m_nCurOptionState = HIGH;
//}

