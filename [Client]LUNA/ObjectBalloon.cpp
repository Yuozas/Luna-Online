#include "stdafx.h"
#include "ObjectBalloon.h"
#include "OBalloonName.h"
#include "OBalloonChat.h"
#include "StreetStallTitleTip.h"
//#include "Object.h"
#include "GameResourceManager.h"
#include "GuildMarkManager.h"
#include "GuildMark.h"
#include "Player.h"
#include "ChatManager.h"
#include "GuildUnionMarkMgr.h"
//#include "Interface/cScriptManager.h"
#include "Interface/cWindowManager.h"
//#include "Npc.h"

#include "FamilyMarkManager.h"
#include "FamilyMark.h"

// desc_hseos_����ý���_01
// S ����ý��� �߰� added by hseos 2007.09.20	2007.10.01
#include "../hseos/Common/SHProgressBar.h"
#include "../hseos/Monstermeter/SHMonstermeterDlg.h"
#include "ObjectManager.h"
#include "GameIn.h"
// E ����ý��� �߰� added by hseos 2007.09.20	2007.10.01

extern int g_nServerSetNum;

/*
WORD NPCIMAGE[] = {
	24,16,14,15,17,19,24,24,20,24,
	18,33,23,21,23,22,24,24,24,24,
	24,24,24,24,24,24,24,24,24,24,
	24,24,24,24,24,24,24,24,24,24,
	24,24,24
};
*/

CObjectBalloon::CObjectBalloon()
{
	m_pName			= NULL;
	m_pChat			= NULL;
	m_pSSTitle		= NULL;
	m_pSBSTitle		= NULL;
	m_pObject		= NULL;
	m_pGuildMark	= NULL;
//	m_pNpcMark		= NULL;

	/*
	m_pBack				= NULL ;
	m_pFront			= NULL ;

	m_bBackReady		= FALSE ;

	m_BackCurSpriteIdx	= 0 ;
	m_BackLastTick		= 0 ;
	m_BackCurTick		= 0 ;

	m_bFrontReady		= FALSE ;

	m_FrontCurSpriteIdx	= 0 ;
	m_FrontLastTick		= 0 ;
	m_FrontCurTick		= 0 ;
	*/
}
 
CObjectBalloon::~CObjectBalloon()
{
	Release();
}

void CObjectBalloon::InitBalloon(CObject * pParent, LONG tall, BYTE flag)
{
	cWindow::Init(0,0,0,0,NULL,pParent->GetID());

	m_pObject = pParent;

	if( pParent->GetObjectKind() == eObjectKind_Player )
	{
		tall = (LONG)(tall * ((CPlayer*)pParent)->GetCharacterTotalInfo()->Height);
	}
/*
	else if( pParent->GetObjectKind() == eObjectKind_Npc )
	{
		m_pNpcMark = new cImage;
		SCRIPTMGR->GetImage(NPCIMAGE[((CNpc*)pParent)->GetNpcKind()],m_pNpcMark,PFT_MINIMAPPATH);
	}
*/	
	m_lTall = tall;

	if(flag & ObjectBalloon_Name)
	{
		ASSERT(!m_pName);
		m_pName = new COBalloonName;
	}

	if(flag & ObjectBalloon_Chat)
	{
		ASSERT(!m_pChat);
		m_pChat = new COBalloonChat;
	}
	
	if(flag & ObjectBalloon_Title)
	{
		ASSERT(!m_pSSTitle);
		ASSERT(!m_pSBSTitle);
		m_pSSTitle = new CStreetStallTitleTip;
		m_pSBSTitle = new CStreetStallTitleTip;
	}

	m_bActive	= TRUE;

	WINDOWMGR->AddChatTooltip( this );
}

void CObjectBalloon::Release()
{
	WINDOWMGR->RemoveChatTooltipWindow( this );

	SAFE_DELETE(m_pName);
	SAFE_DELETE(m_pChat);
	SAFE_DELETE(m_pSSTitle);
	SAFE_DELETE(m_pSBSTitle);
	
	m_pObject		= NULL;
	m_pGuildMark	= NULL;
}

	
void CObjectBalloon::Render()
{
	if( !	m_pObject									||
		!	m_pObject->IsInited()						||
		!	m_pObject->GetEngineObject()				||
		!	m_pObject->GetEngineObject()->IsVisible()	||
		!	m_bActive )
	{
		return;
	}
	
#ifndef _GMTOOL_
	if( m_pObject->GetObjectKind() == eObjectKind_Player )
	{
		// �����ΰ��� ä�ó��� �Ⱥ���
		//GM�� ��
		if(((CPlayer*)m_pObject)->GetCharacterTotalInfo()->bVisible == FALSE)
			return;
	}
#endif

	BOOL bRender = FALSE;
	
	if( ( m_pName && m_pName->IsShow() ) ||
		( m_pChat && m_pChat->IsShow() ) )
	{
		bRender = TRUE;
	}

	LONG lTall = m_lTall + 20;	//�Ӹ������� 20
	
	if( m_pSSTitle	&& m_pSSTitle->IsShow() )
	{
		bRender = TRUE;
		
		lTall	= m_lTall - 50;
//		lX		= 100;
	}
	if( m_pSBSTitle && m_pSBSTitle->IsShow() )
	{
		bRender = TRUE;
		
		lTall	= m_lTall - 50;
		//		lX		= 100;
	}
		
	if( m_pObject->GetState() == eObjectState_Ungijosik )
		lTall = m_lTall - 50;
	else if( m_pObject->GetState() == eObjectState_Die )
		lTall = m_lTall - 100;

	if( !bRender ) return;
	
	VECTOR3 OutPos3, inPos3;
	m_pObject->GetPosition(&inPos3);
	BYTE state = m_pObject->GetState();
	
//	inPos3.x += lX;
	inPos3.y += lTall;

	GetScreenXYFromXYZ(g_pExecutive->GetGeometry(), 0, &inPos3, &OutPos3);
	if(OutPos3.x < 0 || OutPos3.x > 1 || OutPos3.y < 0 || OutPos3.y > 1) return;
	m_absPos.x = (GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth*OutPos3.x) + 0.25f;
	m_absPos.y = (GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight*OutPos3.y);

	if( m_pSSTitle &&
		m_pSSTitle->Render((long)m_absPos.x, (long)(m_absPos.y), 0 ) )
	{
		m_absPos.y -= 25.0f + 15.0f*m_pSSTitle->GetLineCount();
	}

	if( m_pSBSTitle &&
		m_pSBSTitle->Render((long)m_absPos.x, (long)(m_absPos.y), 1 ) )
	{
		m_absPos.y -= 25.0f + 15.0f*m_pSBSTitle->GetLineCount();
	}
	
	if( m_pChat &&
		m_pChat->Render((long)m_absPos.x,(long)(m_absPos.y) ) )
	{
		m_absPos.y -= m_pChat->GetBalloonHeight();
	}
	
	if( m_pName	&&
		m_pName->Render((long)m_absPos.x,(long)m_absPos.y) )
	{
		switch( m_pObject->GetObjectKind() )
 		{
		case eObjectKind_Player:
			{
 				CPlayer* player = ( CPlayer* )m_pObject;
				
				const float lineSize		= 15.0f;
				const float markSpace		= 2.0f;
				const float markSize		= GUILDMARKWIDTH + markSpace;
				cImageSelf*	familyMark		= player->GetFamilyEmblem();
				bool		isFamilyName	= false;

				{
					const char* name	= player->GetFamilyNickName();

					// �йи��� ���� ��쿡�� �̸��� ""����, ó�� ������ ��쿡�� " " ���·� ���õȴ�. �Ѵ� ȣĪ�� ���� ���̸�, ���� �� ���¸� üũ����� �Ѵ�.
					// �йи� ��ũ�� ���� ��쿡�� ������Ű��.
					if( *name && strcmp( " ", name ) )
					{
						isFamilyName = true;
					}
				}

				// �÷��̾� �̸��� �׻� ǥ�õǹǷ� �⺻���� 1
				const float line		= 1.0f + ( isFamilyName ? 1.0f : 0 );

				// desc_hseos_�йи�01
				// S �йи� �߰� added by hseos 2007.07.17
				if( familyMark )
				{
					VECTOR2 position;

					if( isFamilyName )
					{
						position.x = m_absPos.x + m_pName->GetFamilyNickPosX() - markSize;
						position.y = m_absPos.y + m_pName->GetTall() - lineSize - markSpace * 2;
					}
					else
					{
						//position.x = m_absPos.x + m_pName->GetPosX() - markSize;
						position.x = m_absPos.x + m_pName->GetPosX() + m_pName->GetNameLength();
						position.y = m_absPos.y + m_pName->GetTall() - markSpace * 2;
					}
					
					VECTOR2 scale;
					scale.x = 1;
					scale.y = 1;

					familyMark->RenderSprite( &scale, NULL, 0 , &position, 0xffffffff );
				}				
				// E �йи� �߰� added by hseos 2007.07.17

				// ��� ���õ� ��ũ�� ǥ���Ѵ�.
				{
					const COBalloonName::Data&	nickData	= m_pName->GetNickData();
					CGuildUnionMark*			unionMark	= GUILDUNIONMARKMGR->GetGuildUnionMark( 
						g_nServerSetNum,
						player->GetGuildUnionIdx(),
						player->GetGuildUnionMarkIdx() );

					float guildMarkX;
					float unionMarkX;

					{
						guildMarkX	= m_absPos.x + m_pName->GetPosX() - markSpace - markSize;

						if( m_pGuildMark && unionMark )
						{
							unionMarkX	= guildMarkX - markSpace - markSize;
						}
						else
						{
							unionMarkX	= guildMarkX;
						}
					}

					//// ��� ȣĪ�� ���� ���, ��ũ�� �¿����� ǥ�õȴ�.
					//if( nickData.mLength )
					//{
					//	guildMarkX	= m_absPos.x + nickData.mX - markSize;
					//	unionMarkX	= m_absPos.x + nickData.mX + nickData.mLength + markSpace;
					//}
					//// ��ũ�� �ΰ� ��� ������, ����->��� ��ũ ������ ǥ��
					//if( m_pGuildMark && unionMark )
					//{
					//	guildMarkX	= m_absPos.x + m_pName->GetPosX() - markSpace - markSize;
					//	unionMarkX	= guildMarkX - markSpace - markSpace;
					//}
					//// ��ũ�� �ϳ��� ���� ��� �̸� �տ� ǥ�õȴ�.
					//else
					//{
					//	guildMarkX	= m_absPos.x + m_pName->GetPosX() - markSpace - markSize;
					//	unionMarkX	= guildMarkX;
					//}

					VECTOR2 position;
					//position.y = m_absPos.y + m_pName->GetTall() - lineSize - markSpace * 2;
					position.y = m_absPos.y - markSpace * 2;

					// ��� ��ũ�� ��´�. ��� ȣĪ ������.
					if( m_pGuildMark )
					{
						position.x = guildMarkX;
						
						m_pGuildMark->Render( &position );
					}
					
					// ���� ��ũ�� ��´�. ��� ȣĪ ������
					if( unionMark )
					{
						position.x = unionMarkX;
						
						unionMark->Render( &position );
					}
				}
				
				// desc_hseos_����ý���_01
				// S ����ý��� �߰� added by hseos 2007.09.20	2007.10.01
				// ..�̸� �Ʒ� ���α׷�����. ����� ���忡���� ĳ���� ��� ���α׷����ٷ� ����
 				if (m_csProgressBar.CLI_IsStarted() && player->GetID() == HERO->GetID())
				{
					VECTOR2 stPos;
 					stPos.x = m_absPos.x;
					stPos.y = m_absPos.y + 15;

					m_csProgressBar.CLI_Render(&stPos);
				}
				// E ����ý��� �߰� added by hseos 2007.09.20	2007.10.01

				break;
			}
		/*
		case eObjectKind_Npc:
			{
				LONG nposx = m_pName->GetNickPosX() >= m_pName->GetPosX() ? m_pName->GetPosX() : m_pName->GetNickPosX(); 
				if(m_pNpcMark)
				{
					VECTOR2 pos2;
					pos2.x = m_absPos.x + nposx - 30.0f;
					pos2.y = m_absPos.y + m_pName->GetTall() - 10.0f;

					VECTOR2 scale;
					scale.x = 1.0f;
					scale.y = 1.0f;

					m_pNpcMark->RenderSprite( &scale, NULL, 0, &pos2, 0xffffffff );
				}

				break;
			}
		*/
		}
	}

	// 070312 LYW --- ObjectBalloon : Add emoticon part.

	/*
	VECTOR2 vPos = { 0, } ;
	BYTE byWid = 0 ;
	BYTE byHei = 0 ;

	if( m_pBack )
	{
		if( m_bBackReady )
		{
			byWid  = m_pBack->GetWid() ;
			byHei  = m_pBack->GetHei() ;

			vPos.x = m_absPos.x - (byWid/2) ;
			vPos.y = m_absPos.y - (byHei/2) ;

			m_pBack->SetPostition(&vPos) ;
		}

		BackProcess() ;
		m_pBack->Render( m_bBackReady, m_BackCurSpriteIdx ) ;
	}

	if( m_pFront )
	{
		if( m_bFrontReady )
		{
			byWid = m_pFront->GetWid() ;
			byHei = m_pFront->GetHei() ;

			vPos.x = m_absPos.x - (byWid/2) ;
			vPos.y = m_absPos.y - (byHei/2) ;

			m_pFront->SetPostition(&vPos) ;
		}

		FrontProcess() ;
		m_pFront->Render( m_bFrontReady, m_FrontCurSpriteIdx ) ;
	}
	*/
}

//---

void CObjectBalloon::SetOverInfoOption( DWORD dwOption )
{
	if( dwOption )	//���� �ɼǿ� �°� ����									// �ɼǰ��� 1�̸�,
	{
		if( m_pName )		m_pName->SetActive( TRUE );						// �̸� ������ ��ȿ�ϸ�, Ȱ��ȭ ��Ų��.
		if( m_pChat )		m_pChat->SetActive( TRUE );						// ä�� ������ ��ȿ�ϸ�, Ȱ��ȭ ��Ų��.
		if( m_pSSTitle )	m_pSSTitle->SetActive( TRUE );					// ���� ������ ��ȿ�ϸ�, Ȱ��ȭ ��Ų��.
		if( m_pSBSTitle )	m_pSBSTitle->SetActive( TRUE );					// ���� ������ ��ȿ�ϸ�, Ȱ��ȭ ��Ų��.

	}
	else																	// �ɼǰ��� 1�� �ƴϸ�,
	{
		if( m_pName )		m_pName->SetActive( FALSE );					// �̸� ������ ��ȿ�ϸ�, ��Ȱ��ȭ ��Ų��.
		if( m_pChat )		m_pChat->SetActive( FALSE );					// ä�� ������ ��ȿ�ϸ�, ��Ȱ��ȭ ��Ų��.
		if( m_pSSTitle )	m_pSSTitle->SetActive( FALSE );					// ���� ������ ��ȿ�ϸ�, ��Ȱ��ȭ ��Ų��.
		if( m_pSBSTitle )	m_pSBSTitle->SetActive( FALSE );				// ���� ������ ��ȿ�ϸ�, ��Ȱ��ȭ ��Ų��.
	}
}

void CObjectBalloon::ShowObjectName( BOOL bShow, DWORD dwColor )
{
	if( !m_pName ) 
	{
		return;
	}
	m_pName->SetFGColor( dwColor );
	m_pName->Show( bShow );
}

void CObjectBalloon::ShowChatBalloon( BOOL bShow, char* chatMsg, DWORD dwColor, DWORD dwAliveTime )
{
	if( !m_pChat ) return;
	m_pChat->SetFGColor( dwColor );
	m_pChat->Show( bShow );

	if( bShow )
	{
		m_pChat->SetChat( chatMsg );
		m_pChat->SetDisappearTime( dwAliveTime );
	}
}

//---KES �����˻� 2008.3.11
void CObjectBalloon::ShowStreetStallTitle( BOOL bShow, char* strTitle, DWORD dwFontColor, DWORD dwBGColor )
{
	if( !m_pSSTitle ) return;
	m_pSSTitle->SetFGColor( dwFontColor );
	m_pSSTitle->SetBGColor( dwBGColor );
	m_pSSTitle->Show( bShow );

	if( bShow )
	{
		if( strTitle )
			m_pSSTitle->SetTitle( strTitle );
		else
			m_pSSTitle->SetTitle( CHATMGR->GetChatMsg(366) );
	}
}

void CObjectBalloon::ShowStreetBuyStallTitle( BOOL bShow, char* strTitle, DWORD dwFontColor, DWORD dwBGColor )
{
	if( !m_pSBSTitle ) return;
	m_pSBSTitle->SetFGColor( dwFontColor );
	m_pSBSTitle->SetBGColor( dwBGColor );
	m_pSBSTitle->Show( bShow );

	if( bShow )
	{
		if( strTitle )
			m_pSBSTitle->SetTitle( strTitle );
		else
			m_pSBSTitle->SetTitle( CHATMGR->GetChatMsg(366) );
	}
}
//-------------------

BOOL CObjectBalloon::SetGuildMark( DWORD GuildIdx )
{
	m_pGuildMark = GUILDMARKMGR->GetGuildMark( GuildIdx );
	if( m_pGuildMark )	return TRUE;
	else				return FALSE;
}

void CObjectBalloon::SetNickName(char* NickName)
{
	m_pName->SetNickName(NickName);	
}

BOOL CObjectBalloon::SetFamilyMark( DWORD FamilyIdx )
{
	m_pFamilyMark = FAMILYMARKMGR->GetFamilyMark( FamilyIdx );
	if( m_pFamilyMark )	return TRUE;
	else				return FALSE;
}

void CObjectBalloon::SetFamilyNickName(char* NickName)
{
	m_pName->SetFamilyNickName(NickName);	
}

void CObjectBalloon::SetSiegeName( DWORD NameType, DWORD dwLength )
{
	if( m_pName )
		m_pName->SetSiegeName( NameType, dwLength );
}

/*
void CObjectBalloon::InitializeBackProcess()
{
	m_bBackReady = TRUE ;

	m_BackCurSpriteIdx	= 0 ;
	m_BackLastTick		= (GetTickCount()) ;
	m_BackCurTick		= 0 ;
}

void CObjectBalloon::BackProcess() 
{
	if( m_pBack )
	{
		if( m_bBackReady )
		{
			m_BackCurTick = (GetTickCount()) ;

			if( m_BackCurTick - m_BackLastTick > m_pBack->GetDelay(m_BackCurSpriteIdx) )
			{
				m_BackLastTick = m_BackCurTick ;

				if(++m_BackCurSpriteIdx >= m_pBack->GetFrame())
				{
					m_bBackReady = FALSE ;
					m_pBack = NULL ;
				}
			}
		}
	}
}

void CObjectBalloon::InitializeFrontProcess()
{
	m_bFrontReady = TRUE ;

	m_FrontCurSpriteIdx	= 0 ;
	m_FrontLastTick		= (GetTickCount()) ;
	m_FrontCurTick		= 0 ;
}

void CObjectBalloon::FrontProcess() 
{
	if( m_pFront )
	{
		if( m_bFrontReady )
		{
			m_FrontCurTick = (GetTickCount()) ;

			if( m_FrontCurTick - m_FrontLastTick > m_pFront->GetDelay(m_FrontCurSpriteIdx) )
			{
				m_FrontLastTick = m_FrontCurTick ;

				if(++m_FrontCurSpriteIdx >= m_pFront->GetFrame())
				{
					m_bFrontReady = FALSE ;
					m_pFront = NULL ;
				}
			}
		}
	}
}

void CObjectBalloon::InitializeEmoticon( CEmoticon* pBack, CEmoticon* pFront ) 
{
	m_pBack  = pBack ;
	m_pFront = pFront ;

	InitializeBackProcess() ;
	InitializeFrontProcess() ;
}
*/

void CObjectBalloon::Add( cWindow* )
{
	ASSERT( 0 && "It never contain any child control" );
}


void CObjectBalloon::SetObjectName( const char* name )
{
	m_pName->SetName( name );
}