//=================================================================================================
//	FILE		: CChattingDlg.cpp
//	PURPOSE		: Implemention part of chatting interface.
//	PROGRAMMER	: Yongs Lee
//	DATE		: January 22, 2008
//	ADDITION	:
//	ATTENTION	: 
//=================================================================================================





//=========================================================================
//		�ʿ��� ��������� �ҷ��´�.
//=========================================================================
#include "stdafx.h"
#include "WindowIdEnum.h"

#include "..\[CC]Header\CommonGameFunc.h"

#include ".\ChattingDlg.h"

#include "./Input/Mouse.h"
#include "./Input/Keyboard.h"
#include "./input/UserInput.h"

#include "./Interface/cButton.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cListDialog.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cStatic.h"
#include "../ConductInfo.h"

#include "cMsgBox.h"

#include "FilteringTable.h"

#include "ChatManager.h"
#include "CheatMsgParser.h"
#include "OptionManager.h"
#include "ChatManager.h"
#include "PartyManager.h"
#include "GameResourceManager.h"
#include "ExchangeManager.h"
#include "StreetStallManager.h"
#include "ShowdownManager.h"
#include "MoveManager.h"
#include "ConductManager.h"
#include "ObjectManager.h"
#include "cWindowManager.h"

#include "ObjectStateManager.h"
#include "AppearanceManager.h"
#include "FriendManager.h"
#include "GameIn.h"

#include "MiniFriendDialog.h"

#include "ShoutDialog.h"

// desc_hseos_�󱼱�ü01
// S �󱼱�ü �߰� added by hseos 2007.06.08
#include "FacialManager.h"
// E �󱼱�ü �߰� added by hseos 2007.06.08

// desc_hseos_����ý���_01
// S ����ý��� �߰� added by hseos 2007.09.20
#include "../hseos/Farm/SHFarmManager.h"
// E ����ý��� �߰� added by hseos 2007.09.20

// desc_hseos_��ȥ_01
// S ��ȥ �߰� added by hseos 2007.12.13
#include "../hseos/Marriage/SHMarriageManager.h"
// E ��ȥ �߰� added by hseos 2007.12.13


//---KES �����˻� 2008.3.11
#include "StreetStallManager.h"
//-------------------------

// 080421 LYW --- ChattingDlg : ä�ù� �Ŵ��� ȣ��.
#include "./ChatRoomMgr.h"
#include "MHMap.h" // 080402 NYJ


//=========================================================================
//	NAME : CChattingDlg
//	DESC : The function construct.
//=========================================================================
CChattingDlg::CChattingDlg(void)
{
	m_pChatList		= NULL ;
	m_pAssistList	= NULL ;
	m_pShoutList	= NULL ;

	m_pInputBox		= NULL ;

	m_pExtendBtn	= NULL ;
	m_pReductionBtn	= NULL ;

	for( BYTE count = 0 ; count < e_CONVERSATION_MAX; ++count )
	{
		m_pTabButton[count] = NULL ;
	}

	m_type = WT_CHATTING ;

	m_byCurConversation = e_CONVERSATION_ALL ;
	m_byCurAssistance	= e_CONVERSATION_ALL ;

	m_byCurCommand		= e_SMT_ALL ;

	ZeroMemory(&m_SenderName, sizeof(sWHISPER_NAME)) ;
	ZeroMemory(&m_ReceiverName, sizeof(sWHISPER_NAME)) ;

	m_nNameIndex		= 2 ;
	memset(m_szWhisperName, 0, sizeof(char)*128) ;
	memset(m_szSendMsg, 0, sizeof(char)*128) ;

	// 080421 LYW --- ChattingDlg : ä�ù� ����â ȣ�� ��Ʈ�� �߰�.
	m_pToggleChatRoom = NULL ;
}





//=========================================================================
//	NAME : ~CChattingDlg
//	DESC : The function destruct.
//=========================================================================
CChattingDlg::~CChattingDlg(void)
{
}





//=========================================================================
//	NAME : Linking
//	DESC : The function to link child controls.
//=========================================================================
void CChattingDlg::Linking()
{
 	m_pChatList		= (cListDialog*)GetWindowForID(CHAT_LIST) ;
	m_pAssistList	= (cListDialog*)GetWindowForID(CHAT_ASSIST_LIST) ;
	m_pAssistList->SetActive(FALSE) ;
	m_pShoutList	= (cListDialog*)GetWindowForID(CHAT_SHOUT_LIST) ;
	CallShoutDlg();

	m_pInputBox		= (cEditBox*)GetWindowForID(CHAT_EDITBOX) ;

	m_pExtendBtn	= (cButton*)GetWindowForID(CHAT_EXTEND_BTN) ;
	m_pReductionBtn	= (cButton*)GetWindowForID(CHAT_REDUCTION_BTN) ;
	m_pCloseBtn		= (cButton*)GetWindowForID(CHAT_CLOSE_BTN) ;
	m_pCloseBtn->SetActive(FALSE) ;

	for( BYTE count = 0 ; count < e_CONVERSATION_MAX ; ++count )
	{
		m_pTabButton[count] = (cPushupButton*)GetWindowForID(CHAT_BTN_ALL+count) ;
	}
	m_pTabButton[e_CONVERSATION_ALL]->SetPush(TRUE) ;
	m_pTabButton[e_CONVERSATION_ASSISTANCE]->SetActive(FALSE) ;

	// desc_hseos_�ɼ�01
	// S �ɼ� �߰� added by hseos 2007.06.19
	// ..ä��â ���� ���� ä�� ��带 �ҷ���.(���̵� ��)
	BOOL nAssistanceDlg = OPTIONMGR->GetGameOption()->nCurChatMode & 0x80;
	UINT btnIdx = OPTIONMGR->GetGameOption()->nCurChatMode & 0x7F;
	if (btnIdx < e_CONVERSATION_MAX)
	{
		for(int count = 0 ; count < e_CONVERSATION_MAX-1 ; ++count )
		{
			m_pTabButton[count]->SetPush(FALSE) ;
		}

		m_pTabButton[btnIdx]->SetPush(TRUE) ;

		SetCommandToInputBox() ;
		m_pInputBox->SetFocusEdit(FALSE) ;

		if (nAssistanceDlg)
		{
			CallAssistanceDlg( btnIdx ) ;
		}
	}

 	int nNum = OPTIONMGR->GetGameOption()->nChatLineNum-m_pChatList->GetMiddleImageNum();
	if (nNum > 0)
	{
		for(int i=0; i<nNum; i++)
		{
			ExtendDlg();;
		}
	}
	// E �ɼ� �߰� added by hseos 2007.06.19

	// 080421 LYW --- ChattingDlg : ä�ù� ����â ȣ�� ��Ʈ�� �߰�.
	m_pToggleChatRoom = (cButton*)GetWindowForID(CHAT_BTN_CHATROOM) ;

#ifdef _GMTOOL_
	ASSERT(m_pToggleChatRoom) ;

	if(!m_pToggleChatRoom)
	{
		MessageBox( NULL, "Failed to load toggle key of chatroom!!", "ChattingDlg", MB_OK) ;
		return ;
	}
#endif //_GMTOOL_
}





//=========================================================================
//	NAME : SetCommandToInputBox
//	DESC : The function to setting command to input edit box.
//=========================================================================
void CChattingDlg::SetCommandToInputBox()
{
	char strCommand[12] = {0, } ;

	int nCommandNum = GetCommand() ;

	switch(nCommandNum)
	{
	case 0 : m_pInputBox->SetEditText("") ;		break ;
	case 1 : m_pInputBox->SetEditText("#") ;	break ;
	case 2 : m_pInputBox->SetEditText("@") ;	break ;
	case 3 : m_pInputBox->SetEditText("$") ;	break ;
	case 4 : m_pInputBox->SetEditText("%") ;	break ;
	case 5 : m_pInputBox->SetEditText("!") ;	break ;
	case 6 : m_pInputBox->SetEditText("") ;	break ;
	}
}





//=========================================================================
//	NAME : InspectMsg
//	DESC : The function to inspect message.
//=========================================================================
void CChattingDlg::InspectMsg(char* str)
{
	char cheatStr[128] = "" ;
	char* ptr ;

	if( strlen(str) == 0 )
	{
		return ;
	}

	strcpy( cheatStr, str ) ;
	ptr = cheatStr ;
	if( strlen(ptr) > 1 )
	{
		ptr++ ;

#ifdef _CHEATENABLE_
	if( CheatFunc( ptr ) )
	{
		return ;
	}
#endif

	}

	// 070620 LYW --- ChattingDlg : Check die.
	if( !HERO->IsDied() )
	{
		if( strlen(str) > 1 )
		{
			if( Check_FaceCommand( str ) )
			{
			}

			if( Check_LifeAction( str ) )
			{
				return ;
			}

			if( Check_EmotionCommand( str ) )
			{
				return ;
			}

			if( Check_EmotionWord( str ) )
			{
				return ;
			}
		}
	}

	FilteringMsg(str) ;
	if( strlen(m_szSendMsg) == 0 )
	{
		return ;
	}

	SendMsg() ;
}





//=========================================================================
//	NAME : FilteringMsg
//	DESC : The function to filter message.
//=========================================================================
void CChattingDlg::FilteringMsg(char* str)
{
	char desc[128] = {0, } ;
	char* tempStr1 ;

	int length1 = 0 ;
	int length2 = 0 ;
	int result  = 0 ;

	length1		= strlen( str ) ;
	tempStr1	= strchr( str, 32 ) ;
	if( tempStr1 != NULL )
	{
		length2 = strlen( tempStr1 ) ;

		result = tempStr1 - str + 1 ;

		strncpy( desc, str, length1 - length2 ) ;

		InspectCommand( desc ) ;

		if( m_byCurCommand == e_SMT_WHISPER )
		{
			tempStr1++ ;
			char* whisperStr = strchr( tempStr1, 32 ) ;
			if( whisperStr == NULL )
			{
				char* pName = str ;
				++pName ;

				strcpy( m_szSendMsg, tempStr1 ) ;

				memset( m_szWhisperName, 0, strlen(m_szWhisperName) ) ;

				char* namePoint = strchr(pName, 32) ;
				strncpy( m_szWhisperName, pName, strlen(pName) - strlen(namePoint)) ;
				return ;
			}
			whisperStr++ ;
			strcpy( m_szSendMsg, whisperStr ) ;

			char* nameStr = strchr( tempStr1, 32 ) ;
			memset( m_szWhisperName, 0, strlen(m_szWhisperName) ) ;
			strncpy( m_szWhisperName, tempStr1, strlen(tempStr1) - strlen(nameStr)) ;				
		}
		else if( m_byCurCommand == e_SMT_WHISPER2 )
		{
			++tempStr1 ;
			strcpy( m_szSendMsg, tempStr1 ) ;
		}
		else if( m_byCurCommand != e_SMT_ALL )
		{
			if(strlen(desc)!=0)
			{
				strcat(desc, tempStr1) ;
				strcpy( m_szSendMsg, desc ) ;
			}
			else
			{
				tempStr1++ ;
				strcpy( m_szSendMsg, tempStr1 ) ;
			}
		}
		else
		{
			strcpy( m_szSendMsg, str ) ;
		}
	}
	else
	{
		InspectCommand( str ) ;

		strcpy( m_szSendMsg, str ) ;
	}
}





//=========================================================================
//	NAME : InspectCommand
//	DESC : The function to inspect command.
//=========================================================================
void CChattingDlg::InspectCommand( char* str )
{
	char buf[12] = {0, } ;
	char* tempStr = str ;

	char token1 = 0 ;
	char token2 = 0 ;

	m_byCurCommand = e_CONVERSATION_ALL ;

	// command kind = 5;  party, family, guild, alliance, shout.
	int commandKind  = 5 ;
	// command count = 4 ; /p /P /�� #
	int commandCount = 4 ;

	// check other command.
	for( int count = 0 ; count < commandKind ; ++count )
	{
		for( int count2 = 0 ; count2 < commandCount ; ++count2 )
		{
			int msgNum = 261+(((count)*commandCount)+(count2)) ;

			strcpy(buf, CHATMGR->GetChatMsg(msgNum)) ;

			if( count2 == commandCount-1 )
			{
				token1 = *str ;

				token2 = *buf ;

				if( token1 == token2 )
				{
					++tempStr ;
					strcpy(str, tempStr) ;
					m_byCurCommand = ++count ;
					return ;
				}
			}
			else
			{
				if( strcmp( tempStr, buf ) == 0 )
				{
					memset(str, 0, strlen(str)) ;
					m_byCurCommand = ++count ;
					return ;
				}
			}
		}
	}

	// check whisper command.
	for( int count = 0 ; count < 3 ; ++count )
	{
		if( strcmp( str, CHATMGR->GetChatMsg(WHISPER_COMMAND+count) ) == 0 )
		{
			m_byCurCommand = e_SMT_WHISPER ;
			return ;
		}
	}

	// check whisper command to user id.
	if( token1 == '/' )
	{
		char* tempWhisper = str ;
		++tempWhisper ;
		memset( m_szWhisperName, 0, strlen(m_szWhisperName) ) ;
		strncpy( m_szWhisperName, tempWhisper, strlen(tempWhisper) ) ;

		m_byCurCommand = e_SMT_WHISPER2 ;
		return ;
	}

}





//=========================================================================
//	NAME : FindUserName
//	DESC : The function to find user name.
//=========================================================================
void CChattingDlg::FindUserName( char* name, char* msg )
{
	char tempStr1[256] = { 0, } ;
	char tempStr2[256] = { 0, } ;
	char* UserName = tempStr1 ;
	char* Msg = tempStr2 ;

	memcpy( tempStr1, name, strlen(name) ) ;
	memcpy( tempStr2, msg, strlen(msg) ) ;

    ++UserName ;
	++Msg ;

	MSG_CHAT data ;

	data.Category	= MP_CHAT ;
	data.Protocol	= MP_CHAT_FINDUSER ;
	data.dwObjectID = HEROID ;
	SafeStrCpy( data.Name, UserName, MAX_NAME_LENGTH+1 ) ;
	SafeStrCpy( data.Msg, Msg, MAX_CHAT_LENGTH+1 ) ;

	NETWORK->Send(&data,data.GetMsgLength()) ;
}





//=========================================================================
//	NAME : Check_LifeAction
//	DESC : The function to check life action.
//=========================================================================
BOOL CChattingDlg::Check_LifeAction( char* str )
{
	char* pName = NULL ;
	char checkBuf[128] = {0, } ;

	pName = strchr(str, 32) ;

	int stringLength = 0 ;

	int nameLength = 0 ;

	stringLength = strlen(str) ;

	if( pName )
	{
		nameLength = strlen(pName) ;
	}

	memcpy(checkBuf, str, stringLength - nameLength) ;

	int nIndex = -1 ;

	cPtrList* pList = NULL ;
	pList = CHATMGR->GetCommandList() ;

	if( !pList ) 
	{
		ASSERTMSG(0, "Fail to getting command list info.") ;
		return FALSE ;
	}

	sACTION_COMMAND* pCommand ;

	PTRLISTPOS pos = NULL ;
	pos = pList->GetHeadPosition() ;

	while(pos)
	{
		pCommand = NULL ;
		pCommand = (sACTION_COMMAND*)pList->GetNext(pos) ;

		if( !pCommand ) continue ;

		if( strcmp(checkBuf, pCommand->string) != 0 ) continue ;

		nIndex = pCommand->index ;
		break ;
	}

	switch(nIndex)
	{
	case 0 :	// ����
		{
			g_UserInput.GetHeroMove()->AttackSelectedObject() ;
			return TRUE ;
		}
		break ;

	case 1 :	// Ÿ��
	case 2 :	// target
	case 3 :	// ���
		{
			if( !pName )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(11)) ;
				return TRUE ;
			}

			++pName ;

			DWORD dwObjectID = 0 ;
			dwObjectID = OBJECTMGR->GetObjectByChatName( pName ) ;

			CObject* pObject = NULL ;
			pObject = OBJECTMGR->GetObject( dwObjectID );

			if( !pObject ) 
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1037)) ;
				return TRUE ;
			}

			BYTE objectKind = pObject->GetObjectKind() ;

			if( objectKind > 2 )	// ���� ���� ó��.
			{	
				dwObjectID = 0 ;
				dwObjectID = OBJECTMGR->GetNearMonsterByName(pName) ;

				pObject = OBJECTMGR->GetObject( dwObjectID );

				if( !pObject ) 
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1037)) ;
					return TRUE ;
				}
			}

			float fDistance = 0.0f ;																// HERO���� �Ÿ��� ���� ������ �����Ѵ�.

			VECTOR3 vHeroPos;																		// HERO�� ��ġ�� ���� ���͸� �����Ѵ�.
			HERO->GetPosition(&vHeroPos) ;															// HERO�� ��ġ�� �޴´�.
			fDistance = CalcDistanceXZ(&vHeroPos, &pObject->GetBaseMoveInfo()->CurPosition) ;				// HERO�� ������ �Ÿ��� ���Ѵ�.

			if( fDistance > 2000.0f ) 
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1037)) ;
				return TRUE ;
			}

			OBJECTMGR->SetSelectedObject(pObject) ;

			return TRUE ;
		}
		break ;

	case 4 :	// �ŷ�
		{
			CObject* pTarget = NULL ;

			if( !pName )
			{
				pTarget = OBJECTMGR->GetSelectedObject();
			}
			else
			{
				++pName ;
				pTarget = OBJECTMGR->GetObjectByName( pName );
			}

			if( !pTarget )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1037)) ;
				return TRUE ;
			}

            OBJECTMGR->SetSelectedObject(pTarget) ;

			EXCHANGEMGR->ApplyExchange() ;

			return TRUE ;
		}
		break ;

	case 5 :	// ����
		{
			STREETSTALLMGR->ChangeDialogState();

			return TRUE ;
		}
		break ;

	case 6 :	// ��Ƽ
	case 7 :	// ��Ƽ����
	case 8 :	// ��Ƽ�����
		{
			DWORD dwPartyIdx = HERO->GetPartyIdx() ;

			if( dwPartyIdx != 0 )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1038)) ;
				return TRUE ;
			}

			GAMEIN->GetPartyDialog()->SetActive(TRUE);

			return TRUE ;
		}
		break ;

	case 9 :	// ��Ƽ �ʴ�
		{
			DWORD dwPartyIdx = HERO->GetPartyIdx() ;

			if( dwPartyIdx == 0 )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(370)) ;
				return TRUE ;
			}

			if( HEROID != PARTYMGR->GetMasterID() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1039)) ;
				return TRUE ;
			}

			CObject* pTarget = NULL ;

			if( !pName )
			{
				pTarget = OBJECTMGR->GetSelectedObject();
			}
			else
			{
				++pName ;
				pTarget = OBJECTMGR->GetObjectByName( pName );
			}

			if( !pTarget ) 
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1037)) ;
				return TRUE ;
			}

			if( PARTYMGR->IsPartyMember(pTarget->GetID()) )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1038)) ;
				return TRUE ;
			}

			OBJECTMGR->SetSelectedObject(pTarget) ;

			PARTYMGR->AddPartyMemberSyn(pTarget->GetID()) ;

			return TRUE ;
		}
		break ;

	case 10 :	// ��Ƽ �߹�
		{
			DWORD dwPartyIdx = HERO->GetPartyIdx() ;

			if( dwPartyIdx == 0 )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(370)) ;
				return TRUE ;
			}

			if( HEROID != PARTYMGR->GetMasterID() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1039)) ;
				return TRUE ;
			}

			CObject* pTarget = NULL ;

			if( !pName )
			{
				pTarget = OBJECTMGR->GetSelectedObject();

				if( !pTarget ) 
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1037)) ;
					return TRUE ;
				}
				else
				{
					PARTYMGR->BanPartyMemberSyn(pTarget->GetID()) ;
				}
			}
			else
			{
				++pName ;

				CParty* pParty = PARTYMGR->GetParty();

                if( !pParty )
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(370)) ;
					return TRUE ;
				}

				if(pParty->IsPartyMember(pName))
				{
					DWORD dwMemberID = pParty->GetMemberIdForName(pName) ;

					PARTYMGR->BanPartyMemberSyn(dwMemberID) ;
				}
				else
				{
					//CHATMGR->AddMsg(CTC_SYSMSG, "�Է��Ͻ� �̸��� ���� ��Ƽ ����� �ƴմϴ�.") ;
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 410 ) );
					return TRUE ;
				}
			}

			return TRUE ;
		}
		break ;

	case 11 :	// ���� ����
		{
			DWORD dwPartyIdx = HERO->GetPartyIdx() ;

			if( dwPartyIdx == 0 )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(370)) ;
				return TRUE ;
			}

			if( HEROID != PARTYMGR->GetMasterID() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1039)) ;
				return TRUE ;
			}

			CObject* pTarget = NULL ;

			if( !pName )
			{
				pTarget = OBJECTMGR->GetSelectedObject();
			}
			else
			{
				++pName ;
				pTarget = OBJECTMGR->GetObjectByName( pName );
			}
			

			if( !pTarget ) 
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1037)) ;
				return TRUE ;
			}

			if( !PARTYMGR->IsPartyMember(pTarget->GetID()) )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1041)) ;
				return TRUE ;
			}

			OBJECTMGR->SetSelectedObject(pTarget) ;

			PARTYMGR->ChangeMasterPartySyn(HEROID, pTarget->GetID()) ;

			return TRUE ;
		}
		break ;

	case 12 :	// ��Ƽ ����
		{
			DWORD dwPartyIdx = HERO->GetPartyIdx() ;

			if( dwPartyIdx == 0 )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(370)) ;
				return TRUE ;
			}

			if( HEROID != PARTYMGR->GetMasterID() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1039)) ;
				return TRUE ;
			}

			if(PARTYMGR->IsProcessing() == TRUE)
				return TRUE ;
			WINDOWMGR->MsgBox(MBI_PARTYBREAKUP, MBT_YESNO, CHATMGR->GetChatMsg(651));

			return TRUE ;
		}
		break ;

	case 14 :	// ��Ƽ Ż��
		{
			DWORD dwPartyIdx = HERO->GetPartyIdx() ;

			if( dwPartyIdx == 0 )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(370)) ;
				return TRUE ;
			}

			if( HEROID == PARTYMGR->GetMasterID() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1042)) ;
				return TRUE ;
			}

			PARTYMGR->DelPartyMemberSyn() ;

			return TRUE ;
		}
		break ;

	case 16 :	// ��� ��û
		{
			CObject* pTarget = NULL ;

			if( !pName )
			{
				pTarget = OBJECTMGR->GetSelectedObject();
			}
			else
			{
				++pName ;
				pTarget = OBJECTMGR->GetObjectByName( pName );
			}

			if( !pTarget ) 
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1037)) ;
				return TRUE ;
			}

			BYTE objectKind = pTarget->GetObjectKind() ;

			if( objectKind != 1 )	// ���� üũ
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1043)) ;
				return TRUE ;
			}

            OBJECTMGR->SetSelectedObject(pTarget) ;

			SHOWDOWNMGR->ApplyShowdown() ;

			return TRUE ;
		}
		break ;

	case 17 :	// ģ�� �ʴ�
	case 18 :	// ģ�� �߰�
		{
			CObject* pTarget = NULL ;

			if( !pName )
			{
				pTarget = OBJECTMGR->GetSelectedObject();
			}
			else
			{
				++pName ;
				pTarget = OBJECTMGR->GetObjectByName( pName );
			}

			if( !pTarget ) 
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1037)) ;
				return TRUE ;
			}

			strupr(pName);

			if(strcmp("",pName) == 0) return FALSE ;

			char heroname[MAX_NAME_LENGTH+1] = {0,};

			sscanf(HERO->GetObjectName(), "%s", heroname);

			strupr(heroname);

			if(strcmp(heroname, pName) == 0)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(676) );
				return TRUE ;
			}				
			FRIENDMGR->AddDelFriendSyn(pName, MP_FRIEND_ADD_SYN);
			GAMEIN->GetMiniFriendDialog()->SetActive(FALSE);

			return TRUE ;
		}
		break ;

	case 19 :	// �ɱ�
	case 20 :	// ����
		{
			// desc_hseos_����ý���_01
			// S ����ý��� �߰� added by hseos 2007.09.20
			if (g_csFarmManager.CLI_GetIngCharMotion()) return FALSE;
			// E ����ý��� �߰� added by hseos 2007.09.20

			CHero* hero = OBJECTMGR->GetHero();
			ASSERT( hero );

			BYTE IsRestMode = OBJECTSTATEMGR->GetObjectState(hero) ;

			BOOL bReady = FALSE ;

			if( IsRestMode == eObjectState_None || IsRestMode == eObjectState_Move || IsRestMode == eObjectState_Rest )
			{
				bReady = TRUE ;
			}

			if( !bReady )
			{
				return FALSE ;
			}

			if( IsRestMode == eObjectState_Rest )
			{
				MOVEMGR->HeroMoveStop();
				OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_None);

				IsRestMode = eObjectState_None;
			}
			else
			{
				MOVEMGR->HeroMoveStop();
				OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Rest);

				IsRestMode = eObjectState_Rest;
			}

			MSG_BYTE msg ;

			msg.Category	= MP_EMOTION ;
			msg.Protocol	= MP_EMOTION_SITSTAND_SYN ;
			msg.dwObjectID	= HERO->GetID() ;
			msg.bData		= IsRestMode ;

			NETWORK->Send(&msg, sizeof(MSG_BYTE));

			return TRUE ;
		}
		break ;

	case 21 :	// ����
		{
			cDialog* dialog = WINDOWMGR->GetWindowForID( ITMD_MIXDLG );
			ASSERT( dialog );

			dialog->SetActive( ! dialog->IsActive() );

			return TRUE ;
		}
		break ;

	case 22 :	// ��ȭ
		{
			cDialog* dialog = WINDOWMGR->GetWindowForID( ITR_REINFORCEDLG );
			ASSERT( dialog );

			dialog->SetActive( ! dialog->IsActive() );

			return TRUE ;
		}
		break ;	

	case 23 :	// ����
		{
			cDialog* dialog = WINDOWMGR->GetWindowForID( DIS_DISSOLUTIONDLG );
			ASSERT( dialog );

			dialog->SetActive( ! dialog->IsActive() );

			return TRUE ;
		}
		break ;

	case 24 :	// ��æƮ
		{
			cDialog* dialog = WINDOWMGR->GetWindowForID( ITD_UPGRADEDLG	 );
			ASSERT( dialog );

			dialog->SetActive( ! dialog->IsActive() );

			return TRUE ;
		}
		break ;

	// desc_hseos_��ȥ_01
	// S ��ȥ �߰� added by hseos 2007.12.12
	case 25:	// ��ȥ ����� ȣĪ
		{
			if( !pName )
			{
			}
			else
			{
				++pName;
//				g_csMarriageManager.CLI_RequestNickname(pName);
			}
		}
		break;
	// E ��ȥ �߰� added by hseos 2007.12.12

//---KES �����˻� 2008.3.11
	case 26:
		{
			//pName�� �̸��� �ƴϰ� �˻��� �ܾ��̴�.

			if( pName )
			{
				STREETSTALLMGR->SetSearchWord( pName+1 );
			}
			else
			{
				STREETSTALLMGR->SetSearchWord( "" );
			}

			STREETSTALLMGR->SearchWordInArea();
		}
		return TRUE;
//---------------

//---KES ���󰡱�
	case 27:
		{
			CObject* pObject = OBJECTMGR->GetSelectedObject();

			if( pObject )
			if( pObject->GetObjectKind() == eObjectKind_Player )
			{
				if( HERO )
					HERO->SetFollowPlayer( pObject->GetID() );
			}
		}
		return TRUE;
//---------------

	default : break ;
	}

	return FALSE ;
}





//=========================================================================
//	NAME : Check_EmotionCommand
//	DESC : The function to check command for emotion.
//=========================================================================
BOOL CChattingDlg::Check_EmotionCommand( char* str )
{
	// get a conduct list.
	CYHHashTable<cConductInfo>* pTable = CONDUCTMGR->GetConductTable() ;

	cConductInfo* pCInfo = NULL ;

	pTable->SetPositionHead() ;

	while( pCInfo = pTable->GetData() )
	{
		if( pCInfo )
		{
			// check conduct kind.
			if( pCInfo->GetConductKind() == 4 )
			{
				// if conduct kind is same to emotion then check command.
				if( strcmp( str, pCInfo->GetConductName() ) == 0 )
				{
					// setting emotioni number.
					MOVEMGR->HeroMoveStop() ;
					CONDUCTMGR->SendToServer( pCInfo->GetMotionIdx() ) ;

					return TRUE ;
				}
			}
		}
	}

	return FALSE ;
}





//=========================================================================
//	NAME : Check_EmotionWord
//	DESC : The function to check words for emotion.
//=========================================================================
BOOL CChattingDlg::Check_EmotionWord( char* str )
{
	int emoticonNum = GetEmoticonNum( str ) ;
	if( emoticonNum > -1 )
	{
		HERO->PlayEmoticon_Syn(emoticonNum) ;
	}
	else
	{
		return FALSE ;
	}

	return TRUE ;
}






//=========================================================================
//	NAME : Check_FaceCommand
//	DESC : The function to check command for face.
//=========================================================================
BOOL CChattingDlg::Check_FaceCommand( char* str )
{
	int faceNum = - 1 ;

	faceNum = CHATMGR->GetFaceNum( str ) ;

	if( faceNum > -1 )
	{
		// change face.
//		HERO->ChangeFace(faceNum) ;
		// desc_hseos_�󱼱�ü01
		// S �󱼱�ü �߰� added by hseos 2007.06.08
		FACIALMGR->SendToServerFace(faceNum);
		// E �󱼱�ü �߰� added by hseos 2007.06.08
		return TRUE ;
	}

	return FALSE ;
}





//=========================================================================
//	NAME : SendMsg
//	DESC : The function to send chatting message.
//=========================================================================
void CChattingDlg::SendMsg()
{
	switch(m_byCurCommand)
	{
		case e_SMT_ALL :		Msg_All() ;			break ;
		case e_SMT_PARTY :		Msg_Party() ;		break ;
		case e_SMT_FAMILY :		Msg_Family() ;		break ;
		case e_SMT_GUILD :		Msg_Guild() ;		break ;
		case e_SMT_ALLIANCE :	Msg_Alliance() ;	break ;
		case e_SMT_TRADE :		Msg_Trade() ;		break ;
		case e_SMT_WHOLE :		Msg_Whole() ;		break ;
		case e_SMT_WHISPER :	Msg_Whisper() ;		break ;
		case e_SMT_WHISPER2 :	Msg_Whisper() ;		break ;
	}
}





//=========================================================================
//	NAME : Msg_All
//	DESC : The function to send normal message.
//=========================================================================
void CChattingDlg::Msg_All()
{
	TESTMSG data;
			data.Category	= MP_CHAT;
			data.Protocol	= MP_CHAT_ALL;
			data.dwObjectID	= HEROID;
	#ifdef _TESTCLIENT_

			char buf[256];
			sprintf( buf, "[%s]: %s", HERO->GetObjectName(), m_szSendMsg );
			CHATMGR->AddMsg( CTC_GENERALCHAT, buf, RGBA_MAKE(94, 94, 94, 255) );
	#endif
			SafeStrCpy(data.Msg, m_szSendMsg, MAX_CHAT_LENGTH+1 );
			NETWORK->Send(&data,data.GetMsgLength());
}





//=========================================================================
//	NAME : Msg_Party
//	DESC : The function to send message to party.
//=========================================================================
void CChattingDlg::Msg_Party()
{
	#ifdef _TESTCLIENT_
  			CHATMGR->AddMsg( CTC_GENERALCHAT, CHATMGR->GetChatMsg(370), RGBA_MAKE(94, 94, 94, 255) );
#else
			if( !PARTYMGR->PartyChat( m_szSendMsg, HERO->GetObjectName() ) )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(370), RGBA_MAKE(94, 94, 94, 255) );
			}
#endif
}





//=========================================================================
//	NAME : Msg_Family
//	DESC : The function to send message to family.
//=========================================================================
void CChattingDlg::Msg_Family()
{
	if (HERO->GetFamily()->Get()->nID == 0)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(369), RGBA_MAKE(94, 94, 94, 255) );
		return ;
	}

	MSG_CHAT stPacket;

	stPacket.Category	= MP_CHAT;
	stPacket.Protocol	= MP_CHAT_FAMILY;
	stPacket.dwObjectID	= HEROID;
	SafeStrCpy(stPacket.Name, HERO->GetObjectName(), MAX_NAME_LENGTH+1);
	SafeStrCpy(stPacket.Msg, m_szSendMsg, MAX_CHAT_LENGTH+1);

	NETWORK->Send(&stPacket, stPacket.GetMsgLength());
	// E �йи� �߰� added by hseos 2007.07.13
}





//=========================================================================
//	NAME : Msg_Guild
//	DESC : The function to send guild message to guild.
//=========================================================================
void CChattingDlg::Msg_Guild()
{
	if( !HERO->GetGuildIdx() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(325), RGBA_MAKE(94, 94, 94, 255) );
		return;
	}

	TESTMSGID data;
	data.Category = MP_CHAT;
	data.Protocol = MP_CHAT_GUILD;
	data.dwObjectID = HERO->GetGuildIdx();
	data.dwSenderID = HEROID;
	SafeStrCpy(data.Msg, m_szSendMsg, MAX_CHAT_LENGTH+1 );
	NETWORK->Send(&data,data.GetMsgLength());
}





//=========================================================================
//	NAME : Msg_Alliance
//	DESC : The function to send message to alliance.
//=========================================================================
void CChattingDlg::Msg_Alliance()
{
	CHero* hero = OBJECTMGR->GetHero();
	ASSERT( hero );
	
	if( ! hero )
	{
		return;
	}
	else if( ! hero->GetGuildIdx() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(325), RGBA_MAKE(94, 94, 94, 255) );
		return;
	}
	else if( !hero->GetGuildUnionIdx() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(371), RGBA_MAKE(94, 94, 94, 255) );
		return;
	}

	{
		MSG_CHAT_WITH_SENDERID message;
		message.Category	= MP_CHAT;
		message.Protocol	= MP_CHAT_GUILDUNION;
		message.dwObjectID	= hero->GetGuildUnionIdx();
		message.dwSenderID	= hero->GetID();
		SafeStrCpy( message.Name, hero->GetObjectName(), sizeof( message.Name ) );
		SafeStrCpy( message.Msg, m_szSendMsg, sizeof( message.Msg ) );
		NETWORK->Send( &message, message.GetMsgLength() );
	}	
}





//=========================================================================
//	NAME : Msg_Trade
//	DESC : The function to send message for trade.
//=========================================================================
void CChattingDlg::Msg_Trade()
{
	MSG_CHAT_WITH_SENDERID msgChat;
	msgChat.Category = MP_CHAT;
	msgChat.Protocol = MP_CHAT_TRADE;
	msgChat.dwObjectID = HEROID;
	msgChat.dwSenderID = HEROID;
	SafeStrCpy( msgChat.Msg, m_szSendMsg, MAX_CHAT_LENGTH+1 );
	SafeStrCpy( msgChat.Name, HERO->GetObjectName(), MAX_NAME_LENGTH+1 );
	NETWORK->Send( &msgChat, msgChat.GetMsgLength() );
}





//=========================================================================
//	NAME : Msg_Whole
//	DESC : The function to send message to whole.
//=========================================================================
void CChattingDlg::Msg_Whole()
{
	MSG_CHAT_WITH_SENDERID msgChat;
	msgChat.Category = MP_CHAT;
	msgChat.Protocol = MP_CHAT_WHOLE;
	msgChat.dwObjectID = HEROID;
	msgChat.dwSenderID = HEROID;
	SafeStrCpy( msgChat.Msg, m_szSendMsg, MAX_CHAT_LENGTH+1 );
	SafeStrCpy( msgChat.Name, HERO->GetObjectName(), MAX_NAME_LENGTH+1 );
	NETWORK->Send( &msgChat, msgChat.GetMsgLength() );
}





//=========================================================================
//	NAME : Msg_Whisper
//	DESC : The function to send message to whisper.
//=========================================================================
void CChattingDlg::Msg_Whisper() 
{
	if( strcmp( m_szWhisperName, HERO->GetObjectName() ) == 0 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(367), RGBA_MAKE(94, 94, 94, 255) ) ;
		return ;
	}

	// desc_hseos_�������͸�01
	// S �������͸� �߰� added by hseos 2007.06.25
	// ..���Ϳ��� ������ ���� ó���� ���� Ÿ���� �����Ѵ�.
	FILTERTABLE->SetExceptionInvalidCharIncludeType(FET_WHISPER_CHARNAME);
	// E �������͸� �߰� added by hseos 2007.06.25
	if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)m_szWhisperName)) == TRUE )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(368), RGBA_MAKE(94, 94, 94, 255));
		return;
	}

	MSG_CHAT data;
	data.Category	= MP_CHAT;
	data.Protocol	= MP_CHAT_WHISPER_SYN;
	data.dwObjectID	= HEROID;
	SafeStrCpy( data.Name, m_szWhisperName, MAX_NAME_LENGTH+1 );
	SafeStrCpy(data.Msg, m_szSendMsg, MAX_CHAT_LENGTH+1 );
	NETWORK->Send(&data,data.GetMsgLength());
}





//=========================================================================
//	NAME : UpdateSenderName
//	DESC : The function to update sender name.
//=========================================================================
void CChattingDlg::UpdateSenderName( char* name ) 
{
	ZeroMemory( m_SenderName.name1, strlen(m_SenderName.name1) ) ;
	memcpy( m_SenderName.name1, m_SenderName.name2, strlen(m_SenderName.name2) ) ;

	ZeroMemory( m_SenderName.name2, strlen(m_SenderName.name2) ) ;
	memcpy( m_SenderName.name2, m_SenderName.name3, strlen(m_SenderName.name3) ) ;

	ZeroMemory( m_SenderName.name3, strlen(m_SenderName.name3) ) ;
	memcpy( m_SenderName.name3, name, strlen(name) ) ;
}





//=========================================================================
//	NAME : UpdateReceiverName
//	DESC : The function to update receiver name.
//=========================================================================
void CChattingDlg::UpdateReceiverName( char* name )
{
	ZeroMemory( m_ReceiverName.name1, strlen(m_ReceiverName.name1) ) ;
	memcpy( m_ReceiverName.name1, m_ReceiverName.name2, strlen(m_ReceiverName.name2) ) ;

	ZeroMemory( m_ReceiverName.name2, strlen(m_ReceiverName.name2) ) ;
	memcpy( m_ReceiverName.name2, m_ReceiverName.name3, strlen(m_ReceiverName.name3) ) ;

	ZeroMemory( m_ReceiverName.name3, strlen(m_ReceiverName.name3) ) ;
	memcpy( m_ReceiverName.name3, name, strlen(name) ) ;
}





//=========================================================================
//	NAME : GetEmoticonNum
//	DESC : The function to receive number of emotion.
//=========================================================================
int CChattingDlg::GetEmoticonNum(char* str)
{
	int returnNum = -1 ;

	cPtrList* pList = CONDUCTMGR->GetEmoticonList() ;

	BYTE listCount = pList->GetCount() ;
	BYTE count = 0 ;

	char* pResult = NULL ;
	EMOTICON* pEmoticon = NULL ;

	PTRLISTPOS pos = pList->GetHeadPosition() ;
	while(count < listCount)
	{
		pEmoticon = (EMOTICON*)pList->GetNext(pos) ;
		if( pEmoticon )
		{
			pResult = strstr(str, pEmoticon->command) ;

			if( pResult != NULL ) return pEmoticon->effectNum ;

			++count ;
		}
	}

	return returnNum ;
}





//=========================================================================
//	NAME : GetCommand
//	DESC : The function to receive command of chatting type.
//=========================================================================
int CChattingDlg::GetCommand()
{
	for( BYTE count = 0 ; count < e_CONVERSATION_MAX-1 ; ++count )
	{
		if( m_pTabButton[count]->IsPushed() ) return count ;
	}

	return 0 ;
}





//=========================================================================
//	NAME : AddMsg
//	DESC : The function to add message to each part.
//=========================================================================
void CChattingDlg::AddMsg( BYTE byClass, char* msg, DWORD dwColor )
{
	char buf[ MAX_TEMPBUFF ] = {0,} ;

	int nLen	= strlen( msg ) ;
	int nCpyNum = 0 ;
	int count = 0 ;

	while( nLen > MAX_TEXTLEN )
	{
		if( (msg + MAX_TEXTLEN ) != CharNext( CharPrev( msg, msg + MAX_TEXTLEN ) ) )
		{
			nCpyNum = MAX_TEXTLEN - 1 ;
		}
		else
		{
			nCpyNum = MAX_TEXTLEN ;
		}

		strncpy( buf, msg, nCpyNum ) ;
		buf[ nCpyNum ] = 0 ;

		m_pChatList->AddItem( buf, dwColor ) ;

		nLen -= nCpyNum ;
		msg  += nCpyNum ;

		if( *msg == ' ' ) ++msg ;
	}

	if( nLen > 0 )
	{
		m_pChatList->AddItem( msg, dwColor ) ;
	}

	if( byClass == m_byCurAssistance )
	{
		// ������ �ش�.
		if( m_pAssistList->IsActive() )
		{
			if(strlen(buf) > 0)
			{
				m_pAssistList->AddItem( buf, dwColor ) ;
			}
			if( strlen(msg) > 0 )
			{
				m_pAssistList->AddItem( msg, dwColor ) ;
			}
		}
	}
}

void CChattingDlg::AddShoutMsg(char* msg, DWORD dwColor)
{
	char buf[ MAX_TEMPBUFF ] = {0,} ;


#ifdef _TL_LOCAL_

	int nLen = strlen( msg );

	BYTE brk[512];
	int nBrkNum = g_TBreak.FindThaiWordBreak( msg, nLen, brk, 512, FTWB_SEPARATESYMBOL );
	int nCutPos = 0;

	int nCount = 0;

	for( int i = 0 ; i < nBrkNum ; ++i )
	{
		++nCount;	//��ܾ�質
		nCutPos += brk[i];
		if( CFONT_OBJ->GetTextExtentEx(0, msg, nCutPos ) > CHATLIST_TEXTEXTENT )
		{
			if( nCount == 1 )	//�Ѵܾ ��µ� �Ѿ������ ������ ���´�.
			{
				for( int k = 1 ; k <= nCutPos ; ++k )
				{
					if( CFONT_OBJ->GetTextExtentEx(0, msg, k ) > CHATLIST_TEXTEXTENT )
					{
						nCutPos = nCutPos - brk[i] + k-1; //�� ���ڱ�����
						strncpy( buf, msg, nCutPos );	//���ܰ����
						buf[nCutPos] = 0;
						m_pChatList->AddItem( buf, dwColor );
						msg += nCutPos;
						brk[i] -= nCutPos;	//�������κ� ���
						nCutPos = 0;
						nCount = 0; //�ʱ�ȭ
						--i;
						break;
					}
				}
			}
			else
			{
				nCutPos -= brk[i];
				strncpy( buf, msg, nCutPos );	//���ܰ����
				buf[nCutPos] = 0;
				//m_pChatList->AddItem( buf, dwColor );
				--i;
				msg += nCutPos;
				nCutPos = 0;
				nCount = 0; //�ʱ�ȭ
			}

			if( *msg == ' ' )
			{
				++msg;	//���� ù���ڰ� �����̽��� �ƴϵ���...
				++i;
			}
		}
	}

#else
	int nLen	= strlen( msg ) ;
	int nCpyNum = 0 ;
	int count = 0 ;

	while( nLen > MAX_TEXTLEN )
	{
		if( (msg + MAX_TEXTLEN ) != CharNext( CharPrev( msg, msg + MAX_TEXTLEN ) ) )
		{
			nCpyNum = MAX_TEXTLEN - 1 ;
		}
		else
		{
			nCpyNum = MAX_TEXTLEN ;
		}

		strncpy( buf, msg, nCpyNum ) ;
		buf[ nCpyNum ] = 0 ;

		//m_pChatList->AddItem( buf, dwColor ) ;

		nLen -= nCpyNum ;
		msg  += nCpyNum ;

		if( *msg == ' ' ) ++msg ;
	}
#endif

	if(strlen(buf) > 0)
	{
		m_pShoutList->AddItem(buf, dwColor);
	}

	if( nLen > 0 )
	{
		m_pShoutList->AddItem(msg, dwColor);
	}
}





//=========================================================================
//	NAME : ActionEvent
//	DESC : The function to process mouse events.
//=========================================================================
DWORD CChattingDlg::ActionEvent( CMouse* mouseInfo )
{
	DWORD we = WE_NULL ;

	if( !IsActive() ) return we ;

	if( !mouseInfo ) return we ;

	// ä�ù� ȣ�� ��ư ó��.
	we |= cDialog::ActionEvent( mouseInfo ) ;
	LONG xPos = (LONG)mouseInfo->GetMouseX() ;
	LONG yPos = (LONG)mouseInfo->GetMouseY() ;
	if(m_pToggleChatRoom->PtInWindow( xPos, yPos ) )
	{
		if( we & WE_LBTNCLICK )
		{
			CHATROOMMGR->ToggleChatRoomMainDlg() ;
			return we ;
		}
	}

	we = WE_NULL ;

	if( !IsPtInControls(mouseInfo) )
	{
		m_pInputBox->ActionEvent(mouseInfo) ;

		return we ;
	}

	we |= cDialog::ActionEvent( mouseInfo ) ;

	if( m_pExtendBtn->IsClickInside() )
	{
		ExtendDlg() ;
		return we ;
	}
	else if( m_pReductionBtn->IsClickInside() )
	{
		ReductionDlg() ;
		return we ;
	}

	if( we & WE_RBTNCLICK )
	{
		Event_RButtonClick(mouseInfo) ;
	}
	else if( we & WE_LBTNCLICK )
	{
		if(!Event_LButtonClick(mouseInfo))
		{
			Event_PushDown(mouseInfo) ;
		}
	}
	
	return we ;
}





//=========================================================================
//	NAME : Event_LButtonClick
//	DESC : The function to process left click event of button.
//=========================================================================
BOOL CChattingDlg::Event_LButtonClick(CMouse* mouseInfo)
{
	LONG xPos = (LONG)mouseInfo->GetMouseEventX() ;
	LONG yPos = (LONG)mouseInfo->GetMouseEventY() ;

	if( m_pExtendBtn->PtInWindow( xPos, yPos ) )
	{
		ExtendDlg() ;
		return TRUE ;
	}
	else if( m_pReductionBtn->PtInWindow( xPos, yPos ) )
	{
		ReductionDlg() ;
		return TRUE ;
	}
	else if( m_pCloseBtn->PtInWindow( xPos, yPos ) )
	{
		ResetAssist() ;

		// 080312 NYJ --- ä�ú���â(=�˾�â) ������ �ɼ� �����ϴ� �ڵ� �߰�.
		sGAMEOPTION stOption = *OPTIONMGR->GetGameOption();
		stOption.nCurChatMode = m_byCurAssistance & 0x80;
		OPTIONMGR->SetGameOption(&stOption);

		return TRUE ;
	}
	/*else if(m_pToggleChatRoom->PtInWindow( xPos, yPos ) )
	{
		CHATROOMMGR->ToggleChatRoomMainDlg() ;
	}*/

	return FALSE ;
}





//=========================================================================
//	NAME : Event_RButtonClick
//	DESC : The function to process right click event of button.
//=========================================================================
void CChattingDlg::Event_RButtonClick(CMouse* mouseInfo)
{
	int btnIdx = -1 ;

	LONG xPos = (LONG)mouseInfo->GetMouseEventX() ;
	LONG yPos = (LONG)mouseInfo->GetMouseEventY() ;

	int count = 0 ;

	for( count = 0 ; count < e_CONVERSATION_MAX-1 ; ++count )
	{
		if( m_pTabButton[count]->PtInWindow(xPos, yPos) )
		{
			btnIdx = count ;
		}
	}

	if( btnIdx == -1 ) return ;

	for( count = 0 ; count < e_CONVERSATION_MAX-1 ; ++count )
	{
		m_pTabButton[count]->SetPush(FALSE) ;
	}

	m_pTabButton[btnIdx]->SetPush(TRUE) ;

	SetCommandToInputBox() ;
	m_pInputBox->SetFocusEdit(FALSE) ;

	CallAssistanceDlg( btnIdx ) ;

	// desc_hseos_�ɼ�01
	// S �ɼ� �߰� added by hseos 2007.06.19
	// ..ä�� ��带 ����. ���̵��� �ҷ���
 	sGAMEOPTION stOption = *OPTIONMGR->GetGameOption();
	stOption.nCurChatMode = btnIdx | 0x80;
	OPTIONMGR->SetGameOption(&stOption);
	// E �ɼ� �߰� added by hseos 2007.06.19
}





//=========================================================================
//	NAME : Event_PushDown
//	DESC : The function to process push down event of tab button.
//=========================================================================
void CChattingDlg::Event_PushDown(CMouse* mouseInfo)
{
	int btnIdx = -1 ;

	LONG xPos = mouseInfo->GetMouseEventX() ;
	LONG yPos = mouseInfo->GetMouseEventY() ;

	int count = 0 ;

	for( count = 0 ; count < e_CONVERSATION_MAX-1 ; ++count )
	{
		if( m_pTabButton[count]->PtInWindow( xPos, yPos ) )
		{
			btnIdx = count ;
		}
	}

	if( btnIdx == -1 ) return ;

	for( count = 0 ; count < e_CONVERSATION_MAX-1 ; ++count )
	{
		m_pTabButton[count]->SetPush(FALSE) ;
	}

	m_pTabButton[btnIdx]->SetPush(TRUE) ;

	SetCommandToInputBox() ;
	m_pInputBox->SetFocusEdit(TRUE) ;

	// desc_hseos_�ɼ�01
	// S �ɼ� �߰� added by hseos 2007.06.19
	// ..ä�� ��带 ����. ���̵��� �ҷ���
	sGAMEOPTION stOption = *OPTIONMGR->GetGameOption();
	stOption.nCurChatMode = btnIdx;
	OPTIONMGR->SetGameOption(&stOption);
	// E �ɼ� �߰� added by hseos 2007.06.19
}





//=========================================================================
//	NAME : ExtendDlg
//	DESC : The function to extend dialog.
//=========================================================================
void CChattingDlg::ExtendDlg()
{
 	int middleCount  = m_pChatList->GetMiddleImageNum() ;

	if( middleCount >= m_pChatList->GetMaxMiddleNum() ) return ;
	
	int middleHeight = m_pChatList->GetHeight(LD_MIDDLE) ;

	int interval = middleCount * middleHeight + 2 ;

	if(m_pShoutList->IsActive())
	{
		m_pShoutList->SetAbsXY( (LONG)m_pShoutList->GetAbsX(), (LONG)m_pShoutList->GetAbsY()-middleHeight ) ;
		//middleHeight -= m_pShoutList->GetHeight(LD_MIDDLE);
	}

	m_pAssistList->SetAbsXY( (LONG)m_pAssistList->GetAbsX(), (LONG)m_pAssistList->GetAbsY()-middleHeight ) ;
	m_pTabButton[e_CONVERSATION_ASSISTANCE]->SetAbsXY((LONG)m_pTabButton[e_CONVERSATION_ASSISTANCE]->GetAbsX(), (LONG)m_pAssistList->GetAbsY() - m_pTabButton[e_CONVERSATION_ASSISTANCE]->GetHeight()) ;
	m_pCloseBtn->SetAbsXY((LONG)m_pCloseBtn->GetAbsX(), (LONG)m_pAssistList->GetAbsY()- m_pTabButton[e_CONVERSATION_ASSISTANCE]->GetHeight() + 5) ;

	m_pChatList->onSize(TRUE) ;

	// desc_hseos_�ɼ�01
	// S �ɼ� �߰� added by hseos 2007.06.20
	// ..ä��â ���� ���� ����. ���̵��� �ҷ���
	sGAMEOPTION stOption = *OPTIONMGR->GetGameOption();
	stOption.nChatLineNum = middleCount;
	OPTIONMGR->SetGameOption(&stOption);
	// E �ɼ� �߰� added by hseos 2007.06.20
}



//=========================================================================
//	NAME : ReductionDlg
//	DESC : The function to reduction dialog.
//=========================================================================
void CChattingDlg::ReductionDlg()
{	
	int middleCount  = m_pChatList->GetMiddleImageNum() ;

	if( middleCount <= m_pChatList->GetMinMiddleNum() ) return ;

	int middleHeight = m_pChatList->GetHeight(LD_MIDDLE) ;

	int interval = middleCount * middleHeight + 2 ;

	if(m_pShoutList->IsActive())
	{
		m_pShoutList->SetAbsXY( (LONG)m_pShoutList->GetAbsX(), (LONG)m_pShoutList->GetAbsY()+middleHeight ) ;
		//middleHeight += m_pShoutList->GetHeight(LD_MIDDLE);
	}

	m_pAssistList->SetAbsXY( (LONG)m_pAssistList->GetAbsX(), (LONG)m_pAssistList->GetAbsY()+middleHeight ) ;
	m_pTabButton[e_CONVERSATION_ASSISTANCE]->SetAbsXY((LONG)m_pTabButton[e_CONVERSATION_ASSISTANCE]->GetAbsX(), (LONG)m_pAssistList->GetAbsY() - m_pTabButton[e_CONVERSATION_ASSISTANCE]->GetHeight()) ;
	m_pCloseBtn->SetAbsXY((LONG)m_pCloseBtn->GetAbsX(), (LONG)m_pAssistList->GetAbsY()- m_pTabButton[e_CONVERSATION_ASSISTANCE]->GetHeight() + 5) ;

	// setting position of extend and reduction button.
	/*m_pExtendBtn->SetAbsXY((LONG)m_pExtendBtn->GetAbsX(), (LONG)m_pExtendBtn->GetAbsY()+middleHeight) ;
	m_pReductionBtn->SetAbsXY((LONG)m_pReductionBtn->GetAbsX(), (LONG)m_pReductionBtn->GetAbsY()+middleHeight) ;*/

	m_pChatList->onSize(FALSE) ;

	// desc_hseos_�ɼ�01
	// S �ɼ� �߰� added by hseos 2007.06.20
	// ..ä��â ���� ���� ����. ���̵��� �ҷ���
	sGAMEOPTION stOption = *OPTIONMGR->GetGameOption();
	stOption.nChatLineNum = middleCount;
	OPTIONMGR->SetGameOption(&stOption);
	// E �ɼ� �߰� added by hseos 2007.06.20
}




//=========================================================================
//	NAME : ResetAssist
//	DESC : The function to resetting to assist chatting dialog.
//=========================================================================
void CChattingDlg::ResetAssist()
{
	//setting backbround.
	if(m_pAssistList->IsActive())	m_pAssistList->SetActive(FALSE) ;
	if(m_pCloseBtn->IsActive())		m_pCloseBtn->SetActive(FALSE) ;
	if(m_pTabButton[e_CONVERSATION_ASSISTANCE]->IsActive())	m_pTabButton[e_CONVERSATION_ASSISTANCE]->SetActive(FALSE) ;
}





//=========================================================================
//	NAME : GetCurMsgColor
//	DESC : The function to receive current message color.
//=========================================================================
DWORD CChattingDlg::GetCurMsgColor(BYTE msgType)
{
	BYTE colorIdx = 0 ;

	switch(msgType)
	{
		case e_CONVERSATION_ALL :		colorIdx = CTC_GENERALCHAT ;	break ;
		case e_CONVERSATION_PARTY :		colorIdx = CTC_PARTYCHAT ;		break ;
		case e_CONVERSATION_FAMILY :	colorIdx = CTC_FAMILY ;			break ;
		case e_CONVERSATION_GUILD :		colorIdx = CTC_GUILDCHAT ;		break ;
		case e_CONVERSATION_ALLIANCE :	colorIdx = CTC_ALLIANCE ;		break ;
		case e_CONVERSATION_TRADE :		colorIdx = CTC_TRADE ;			break ;
		case e_CONVERSATION_SYSTEM : 	colorIdx = CTC_SYSMSG ;			break ;
	}

	return CHATMGR->GetMsgColor(colorIdx) ;
}





//=========================================================================
//	NAME : CallAssistanceDlg
//	DESC : The function to call assistance dialog.
//=========================================================================
void CChattingDlg::CallAssistanceDlg( BYTE byClass )
{
	m_byCurAssistance = byClass ;

	ResetAssist() ;

	LONG itemCount = m_pChatList->GetItemCount() ;

	BYTE count = 0 ;

	m_pAssistList->RemoveAll() ;

	DWORD curColor  = GetCurMsgColor(byClass) ;
	DWORD itemColor = 0 ;

	while( itemCount > count )
	{
		ITEM* pItem = m_pChatList->GetItem( count ) ;
		if( pItem )
		{
			itemColor = pItem->rgb ;

			if( curColor == itemColor )
			{
				m_pAssistList->AddItem(pItem->string, pItem->rgb) ;
			}
		}

		++count ;
	}

	DWORD txtColor = RGBA_MAKE(255, 255, 255, 255) ;

	int middleCount  = m_pChatList->GetMiddleImageNum() ;
	int middleHeight = m_pChatList->GetHeight(LD_MIDDLE) ;

	int interval = (middleCount-2) * middleHeight + m_pChatList->GetHeight(LD_TOP) + m_pChatList->GetHeight(LD_DOWN) + 2 ;

	if(m_pShoutList->IsActive())
	{
		middleCount = m_pShoutList->GetMiddleImageNum();
		middleHeight = m_pShoutList->GetHeight(LD_MIDDLE);
		interval += middleCount * middleHeight + m_pShoutList->GetHeight(LD_TOP) + m_pShoutList->GetHeight(LD_DOWN) + 2 ;
	}

	m_pAssistList->SetAbsXY( (LONG)m_pAssistList->GetAbsX(), (LONG)GetAbsY()-interval ) ;
	m_pAssistList->SetActive( TRUE ) ;

	m_pTabButton[e_CONVERSATION_ASSISTANCE]->SetText( m_pTabButton[byClass]->GetButtonText(), txtColor, txtColor, txtColor ) ;
	m_pTabButton[e_CONVERSATION_ASSISTANCE]->SetAbsXY((LONG)m_pAssistList->GetAbsX()+6, (LONG)m_pAssistList->GetAbsY() - m_pTabButton[e_CONVERSATION_ASSISTANCE]->GetHeight()) ;
	m_pTabButton[e_CONVERSATION_ASSISTANCE]->SetActive(TRUE) ;

	m_pCloseBtn->SetAbsXY((LONG)m_pAssistList->GetAbsX()+52, (LONG)m_pAssistList->GetAbsY() - m_pTabButton[e_CONVERSATION_ASSISTANCE]->GetHeight() + 5) ;
	m_pCloseBtn->SetActive(TRUE) ;
}

void CChattingDlg::CallShoutDlg()
{
	int middleCount  = m_pChatList->GetMiddleImageNum() ;
	int middleHeight = m_pChatList->GetHeight(LD_MIDDLE) ;

	int interval = (middleCount-2) * middleHeight + m_pShoutList->GetHeight(LD_TOP) + m_pShoutList->GetHeight(LD_DOWN) + 2 ;

	m_pShoutList->SetAbsXY( (LONG)m_pShoutList->GetAbsX(), (LONG)m_pShoutList->GetAbsY() -interval ) ;
	m_pShoutList->SetActive(TRUE);
}





//=========================================================================
//	NAME : ActionKeyboardEvent
//	DESC : The function to process keyboard event.
//=========================================================================
DWORD CChattingDlg::ActionKeyboardEvent( CKeyboard* keyInfo )
{
	DWORD we = WE_NULL ;

	we |= cDialog::ActionKeyboardEvent( keyInfo ) ;

	BOOL bFocused = m_pInputBox->IsFocus() ;
	if( !bFocused ) return we ;

	char nameStr[ MAX_NAME_LENGTH+1 ] = {0, } ;

	char* whisperName = NULL ;
	int nCurWNameIndex = 0 ;

	BOOL bKey_UpAndDown = FALSE ;

	BOOL bPressedUp = FALSE ;
	BOOL bPressedDown = FALSE ;

	if( keyInfo->GetKeyPressed(KEY_UP) && keyInfo->GetKeyPressed(KEY_CONTROL) )
	{
		bPressedUp = TRUE ;

		nCurWNameIndex = m_nNameIndex -1 ;

		if( nCurWNameIndex < 0 ) nCurWNameIndex = MAX_WNAME_COUNT-1 ;

		whisperName = CHATMGR->GetWhisperName(nCurWNameIndex) ;
	}
	else if( keyInfo->GetKeyPressed(KEY_DOWN) && keyInfo->GetKeyPressed(KEY_CONTROL) )
	{
		bPressedDown = TRUE ;

		nCurWNameIndex = m_nNameIndex +1 ;

		if( nCurWNameIndex > MAX_WNAME_COUNT-1 ) nCurWNameIndex = 0 ;

		whisperName = CHATMGR->GetWhisperName(nCurWNameIndex) ;
	}

	if( !bPressedUp && !bPressedDown ) return we ;

	if( !whisperName || strlen(whisperName) < 4 )
	{
		whisperName = NULL ;
		whisperName = CHATMGR->GetWhisperName(m_nNameIndex) ;

		if( !whisperName || strlen(whisperName) < 4 ) return we ;
	}

	if( bPressedUp ) 
	{
		--m_nNameIndex ;

		if( m_nNameIndex < 0 ) m_nNameIndex = MAX_WNAME_COUNT-1 ;
	}
	else if( bPressedDown )
	{
		++m_nNameIndex ;

		if( m_nNameIndex > MAX_WNAME_COUNT-1 ) m_nNameIndex = 0 ;
	}

	char slash[12] = {0, } ;
	memcpy(slash, "/", strlen("/")) ;

	char* pCurPos = NULL ;
	char space[1] = {0, } ;
	space[0] = 32 ;

	strcat(nameStr, slash) ;
	strcat(nameStr, whisperName) ;
	pCurPos = nameStr ;
	pCurPos += strlen(nameStr) ;

	memcpy(pCurPos, space, sizeof(char)) ;

	if(strlen(nameStr) != 0)
	{
		m_pInputBox->SetEditText(nameStr) ;
	}

	return we ;
}





//=========================================================================
//	NAME : IsPtInControls
//	DESC : The function to check mouse in to the controls. 080122
//=========================================================================
BOOL CChattingDlg::IsPtInControls(CMouse* mouseInfo)
{
	BOOL bPtinControls = FALSE ;											// ��� ���� ���� ������ �����ϰ� false ����.

	if( !mouseInfo ) return bPtinControls ;									// ���콺 ������ üũ.

	LONG lXpos = mouseInfo->GetMouseX() ;
	LONG lYpos = mouseInfo->GetMouseY() ;									// ���콺 ��ġ�� �޴´�.

	if(m_pInputBox->PtInWindow(lXpos, lYpos))								// ä�� �Է� â üũ.
	{
		bPtinControls = TRUE ;
	}

	if(m_pExtendBtn->PtInWindow(lXpos, lYpos))								// Ȯ�� ��ư üũ.
	{
		bPtinControls = TRUE ;
	}

	if(m_pReductionBtn->PtInWindow(lXpos, lYpos))							// ��� ��ư üũ.
	{
		bPtinControls = TRUE ;
	}

	for( int count = 0 ; count < e_CONVERSATION_MAX ; ++count )				// ä�� �� ��ư �� üũ.
	{
		if(m_pTabButton[count]->PtInWindow(lXpos, lYpos))
		{
			bPtinControls = TRUE ;
			break ;
		}
	}

	if( m_pChatList->GetUpwardBtn()->PtInWindow(lXpos, lYpos))				// ä�� â �� ��ư üũ.
	{
		bPtinControls = TRUE ;
	}

	if( m_pChatList->GetDownwardBtn()->PtInWindow(lXpos, lYpos))			// ä�� â �ٿ� ��ư üũ.
	{
		bPtinControls = TRUE ;
	}

	if( m_pChatList->GetGuageBar()->PtInWindow(lXpos, lYpos))				// ä�� â ������ �� üũ.
	{
		bPtinControls = TRUE ;
	}

	if( m_pAssistList->GetUpwardBtn()->PtInWindow(lXpos, lYpos))			// ����ä�� â �� ��ư üũ.
	{
		bPtinControls = TRUE ;
	}

	if( m_pAssistList->GetDownwardBtn()->PtInWindow(lXpos, lYpos))			// ����ä�� â �ٿ� ��ư üũ.
	{
		bPtinControls = TRUE ;
	}

	if( m_pAssistList->GetGuageBar()->PtInWindow(lXpos, lYpos))				// ����ä�� â ������ �� üũ.
	{
		bPtinControls = TRUE ;
	}

	if(m_pShoutList->GetUpwardBtn()->PtInWindow(lXpos, lYpos))				// ��ġ�� â �� ��ư üũ.
	{
		bPtinControls = TRUE;
	}

	if(m_pShoutList->GetDownwardBtn()->PtInWindow(lXpos, lYpos))			// ����ä�� â �ٿ� ��ư üũ.
	{
		bPtinControls = TRUE ;
	}

	if(m_pShoutList->GetGuageBar()->PtInWindow(lXpos, lYpos))				// ����ä�� â ������ �� üũ.
	{
		bPtinControls = TRUE ;
	}

	//// 080421 LYW --- ChattingDlg : ä�ù� ȣ�� üũ.
	//if(m_pToggleChatRoom->PtInWindow(lXpos, lYpos))
	//{
	//	bPtinControls = TRUE ;
	//}

	return bPtinControls ;
}
