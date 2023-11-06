#include "stdafx.h"
#include ".\cquickitem.h"
#include "./Interface/cScriptManager.h"
#include "./interface/cFont.h"
#include "./Interface/cWindowManager.h"
#include "./input/UserInput.h"
#include "../[cc]skill/client/manager/skillmanager.h"
#include "../[cc]skill/client/info/activeskillinfo.h"
#include "../[cc]skill/client/info/skillinfo.h"
#include ".\cskillbase.h"
#include "chatmanager.h"
#include "ItemManager.h"

cQuickItem::cQuickItem( WORD tab, WORD pos )
: mTab( tab ), mPos( pos )
{
	SCRIPTMGR->GetImage( 2, &m_DelayImage  );	
	m_type = WT_QUICKITEM;
	m_bRemainTime	= FALSE;
	m_dwLastSecond	= 0;
	m_pItemLink = NULL;
	memset( &mInfo, 0, sizeof( SLOT_INFO ) );
	mData[0] = 0;
}

cQuickItem::~cQuickItem(void)
{
}

void cQuickItem::SetLinkItem( CBaseItem * pItem )
{
	m_pItemLink = pItem;
	//m_pItemLink->AddQuick( this );
}

void cQuickItem::SetSlotData( WORD dur )
{
	mInfo.data = dur;

	if( dur <= 0 )
	{
		SetZeroCount(TRUE) ;
	}
	
	if( mInfo.kind == QuickKind_MultiItem )
	{
		wsprintf(mData,"%2d", mInfo.data);
	}
}

void cQuickItem::ClearLink()
{
	if( m_pItemLink )
	{
		m_pItemLink->RemoveQuick( this );
		m_pItemLink = NULL;
	}

	memset( &mInfo, 0, sizeof( SLOT_INFO ) );
	mData[0] = 0;
	SetCurImage( NULL );
	m_HighLayerImage.SetSpriteObject( NULL );

	SetZeroCount(FALSE) ;
}

void cQuickItem::SetImage2(cImage * low, cImage * high)
{
	SetBasicImage(low);
	if(high)
	m_HighLayerImage = *high;
	
	SCRIPTMGR->GetImage( 2, &m_DelayImage  );	
}

void cQuickItem::Render()
{
	DWORD colorBackup = m_dwImageRGB;
	if( !m_bActive ) return;

	cIcon::Render();

	if(!m_HighLayerImage.IsNull())
		m_HighLayerImage.RenderSprite( NULL, NULL, 0, &m_absPos, RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));

	if(!m_LinkImage.IsNull())
	{
		VECTOR2 pos = { m_absPos.x+1, m_absPos.y+29 };
		m_LinkImage.RenderSprite(NULL, NULL, NULL, &pos, RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
	}

	if( m_bZeroCount )
	{
		m_ZeroImage.RenderSprite( NULL, NULL, 0, &m_absPos, 
									RGBA_MERGE(0xffffff, 200 * m_dwOptionAlpha / 100));
	}
	
	if(mData[0])		
	{
		//RECT rect={(LONG)m_absPos.x+23, (LONG)m_absPos.y+25, 1,1};
		//CFONT_OBJ->RenderFont(0,mData,strlen(mData),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));		// color hard coding : taiyo 
		int len = strlen( mData );

		RECT rect={(LONG)m_absPos.x+38-(len*8), (LONG)m_absPos.y+24, 1,1};
		CFONT_OBJ->RenderFontShadow(0, 1, mData,strlen(mData),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));		// color hard coding : taiyo 
	}

	if( m_pItemLink && mInfo.kind == QuickKind_Skill )
	{
		SKILL_BASE* pBase = ( ( cSkillBase* )m_pItemLink )->GetSkillBase();
		cSkillInfo* pInfo = SKILLMGR->GetSkillInfo( pBase->wSkillIdx - 1 + pBase->Level );

		if( pInfo &&
			( pInfo->GetKind() == SKILLKIND_PHYSIC || pInfo->GetKind() == SKILLKIND_MAGIC || pInfo->GetKind() == SKILLKIND_ONOFF ) )
		{
			cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )pInfo;

			DWORD dwRemainTime = 0;

			if( pSkillInfo->GetCoolTime()->IsStart() )
			{
				pSkillInfo->GetCoolTime()->Check();
				float delayRate = pSkillInfo->GetCoolTime()->GetRemainRate();
				dwRemainTime = pSkillInfo->GetCoolTime()->GetRemainTime();
				if( delayRate > 0.0f )
				{
					VECTOR2 vScale = { 1.0f, m_height * delayRate };
					VECTOR2 pos = { m_absPos.x, m_absPos.y + m_height*(1 - delayRate) };
					m_DelayImage.RenderSprite( &vScale, NULL, 0.0f, &pos, 0xff000000  );
				}
			}

			if( WINDOWMGR->IsMouseOverUsed() )
			{
				int mx = g_UserInput.GetMouse()->GetMouseX();
				int my = g_UserInput.GetMouse()->GetMouseY();

				if( PtInWindow( mx, my ) == TRUE )
				{
					if( dwRemainTime )
					{
						if( m_dwLastSecond != dwRemainTime/1000+1 )
						{
							m_dwLastSecond = dwRemainTime/1000+1;
							SetSkillIconToolTip( dwRemainTime );
							m_bRemainTime = TRUE;
						}
					}
					else if( m_bRemainTime == TRUE )
					{
						SetSkillIconToolTip( dwRemainTime );
						m_bRemainTime = FALSE;
						m_dwLastSecond = 0;
					}
					else
					{
						SetSkillIconToolTip( dwRemainTime );	//마우스가 있을땐 항상 갱신해줄 필요가 있다.
					}
				}
			}
		}
	}
	
	m_dwImageRGB = colorBackup;
}


void cQuickItem::SetSkillIconToolTip( DWORD dwRemainTime )
{	
	cImage imgToolTip;
	SCRIPTMGR->GetImage( 0, &imgToolTip, PFT_HARDPATH );

	char line[64];
	cSkillBase* pSkill = ( cSkillBase* )( GetItem() );

	cSkillInfo* pCommSkill = pSkill->m_pSkillInfo;
	if(!pCommSkill)	return;

	SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP );

	wsprintf(line, "[%s]", pCommSkill->GetName() );

	AddToolTipLine( line, TTTC_SKILLNAME );
	AddToolTipLine( "" );
	wsprintf(line, "%s%d", CHATMGR->GetChatMsg( 769 ), pSkill->GetSkillBase()->Level );

	AddToolTipLine( line, TTTC_SKILLLEVEL );

	if( pCommSkill->GetKind() != SKILLKIND_PASSIVE )
	{
		const ACTIVE_SKILL_INFO* info = ( ( cActiveSkillInfo* )pCommSkill )->GetSkillInfo();

		if( ! info )
		{
			return;
		}
		// 080616 LUJ, 상대값으로 소모되는 마나 양 표시
		else if( info->mMana.mPercent )
		{
			sprintf(
				line,
				"%s%0.0f%%",
				CHATMGR->GetChatMsg( 768 ),
				info->mMana.mPercent * 100.0f );
		}
		// 080616 LUJ, 절대값으로 소모되는 마나 양 표시
		else if( info->mMana.mPlus )
		{
			sprintf(
				line,
				"%s%0.0f",
				CHATMGR->GetChatMsg( 768 ),
				info->mMana.mPlus );
		}

		AddToolTipLine( line, TTTC_SKILLLEVEL );

		float coolTime = ( float )( ( ( cActiveSkillInfo* )pCommSkill )->GetSkillInfo()->CoolTime ) / 1000.0f;	//KES 070530 : 1000 --> 1000.f ??	//ncf

		sprintf(line, CHATMGR->GetChatMsg( 776 ), coolTime);

		AddToolTipLine( line, TTTC_SKILLLEVEL );
	}

	ITEMMGR->AddToolTip( this, pCommSkill->GetTooltip() );	

	if( dwRemainTime )
	{
		AddToolTipLine( "" );
		wsprintf( line, "%s%d",CHATMGR->GetChatMsg(174), dwRemainTime/1000+1);
		AddToolTipLine( line, TTTC_QUESTITEM );
	}
	//	ITEMMGR->AddDescriptionToolTip( this, pCommSkill->GetTooltip() );
	/*	cSkillInfo* pCommSkill	= SKILLMGR->GetSkillInfo( GetSrcIdx() );

	if( pCommSkill )
	{
	char ToolTipText[256] = { 0, };

	//				cImage imgToolTip;
	//				SCRIPTMGR->GetImage( 0, &imgToolTip, PFT_HARDPATH );
	//				SetToolTip( "", RGB_HALF( 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP );
	SetToolTip( "", RGB_HALF( 255, 255, 255), NULL, TTCLR_ITEM_CANEQUIP );

	wsprintf(ToolTipText, "[%s]", pCommSkill->GetSkillName() );
	AddToolTipLine( ToolTipText, TTTC_DEFAULT );
		
		CSkillBase* pSkillBase = GAMEIN->GetSkillDialog()->GetSkillAbs( GetSrcPosition() );
		
		if( pSkillBase )
		{
			//무공의 경우 성 / 필요내력
			int a = pCommSkill->GetNeedExp( pSkillBase->GetLevel() );
			
			if( a > 0 )
			{				
				if( pSkillBase->GetLevel() < 12 )
				{
//					sprintf( ToolTipText, CHATMGR->GetChatMsg(471), pSkillBase->GetLevel(),
//						pSkillBase->GetExpPoint() * 100 / (float)pCommSkill->GetNeedExp(pSkillBase->GetLevel()) );
					float percent = pSkillBase->GetExpPoint() * 100.f / (float)pCommSkill->GetNeedExp(pSkillBase->GetLevel());
					if( pSkillBase->GetExpPoint() != pCommSkill->GetNeedExp(pSkillBase->GetLevel()) -1 )
					{
						if(percent > 99.99f)
							percent = 99.99f;
					}

					sprintf( ToolTipText, CHATMGR->GetChatMsg(471), pSkillBase->GetLevel(), percent);

				}
				else
				{
					if(SKILLMGR->IsDeadlySkill(pSkillBase->GetItemIdx()) == FALSE)
						sprintf( ToolTipText, CHATMGR->GetChatMsg(405), pSkillBase->GetLevel() );
				}
				AddToolTipLine( ToolTipText, TTTC_SKILLEXP );
			}
			
			if( pSkillBase->GetLevel() > 0 )
			{
				wsprintf( ToolTipText, CHATMGR->GetChatMsg(472), pCommSkill->GetNeedMana( pSkillBase->GetLevel() ) );
				AddToolTipLine( ToolTipText, TTTC_SKILLNEEDMANA );
			}

			//제한레벨 체크해서 이미지 rgb 바꾸기
			if( pCommSkill->GetSkillInfo()->RestrictLevel > HERO->GetLevel() )
			{
				wsprintf( ToolTipText, CHATMGR->GetChatMsg(460), pCommSkill->GetSkillInfo()->RestrictLevel );
				AddToolTipLine( ToolTipText, TTTC_LIMIT );
				m_dwImageRGB = RGBA_MAKE( 255, 100, 100, 255 );
			}
			else
			{
				m_dwImageRGB = RGBA_MAKE( 255, 255, 255, 255 );
			}
		}
		
		if( /*pCommSkill->GetSkillKind() == SKILLKIND_OUTERSKILL && */
/*			pCommSkill->GetWeaponType() != HERO->GetWeaponEquipType() )
		{
			switch(pCommSkill->GetWeaponType())
			{
			case WP_GUM:
				AddToolTipLine( CHATMGR->GetChatMsg(465), TTTC_LIMIT );
				break;
			case WP_GWUN:	
				AddToolTipLine( CHATMGR->GetChatMsg(466), TTTC_LIMIT );
				break;
			case WP_DO:	
				AddToolTipLine( CHATMGR->GetChatMsg(467), TTTC_LIMIT );
				break;
			case WP_CHANG:	
				AddToolTipLine( CHATMGR->GetChatMsg(468), TTTC_LIMIT );
				break;
			case WP_GUNG:	
				AddToolTipLine( CHATMGR->GetChatMsg(469), TTTC_LIMIT );
				break;
			case WP_AMGI:	
				AddToolTipLine( CHATMGR->GetChatMsg(470), TTTC_LIMIT );
				break;
			}
		}
		
		if( dwRemainTime )
		{
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(582), dwRemainTime/1000+1 );
			AddToolTipLine( ToolTipText, TTTC_QUESTITEM );
		}
	}		*/
}

