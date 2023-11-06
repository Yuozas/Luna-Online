#include "stdafx.h"
#include ".\cskillbase.h"
#include "./Interface/cFont.h"
#include "./Input/Mouse.h"
#include "interface/cScriptManager.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "cSkillTreeManager.h"
#include "../[cc]skill/client/info/activeskillinfo.h"
#include "../[cc]skill/client/manager/skillmanager.h"

cSkillBase::cSkillBase(void)
{
	SCRIPTMGR->GetImage( 2, &m_DelayImage  );	
	m_bRemainTime	= FALSE;
	m_dwLastSecond	= 0;
}

cSkillBase::~cSkillBase(void)
{
}

void cSkillBase::SetSkillBase( SKILL_BASE* pSkill )
{
	cImage skillImage;
	DWORD skillidx = pSkill->wSkillIdx - 1 + pSkill->Level;
	cSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(skillidx);
	SCRIPTMGR->GetImage( pSkillInfo->GetImage(), &skillImage, PFT_SKILLPATH );
	m_pHighLayerImage = skillImage;
	memcpy( &m_SkillBaseInfo, pSkill, sizeof( SKILL_BASE ) );

	m_pSkillInfo = pSkillInfo;
}
void cSkillBase::Init( LONG x, LONG y, WORD wid, WORD hei, cImage * lowImage, cImage * highImage, LONG ID )
{
	cIcon::Init(x,y,wid,hei,lowImage,ID);
	m_type = WT_SKILL;
	if(highImage)
		m_pHighLayerImage = *highImage;
	
	cImage bimg,gimg;
	cImageRect rect;
	SetRect(&rect,1010,216,1011,226);
	SCRIPTMGR->GetImage(0,&bimg,&rect);	//basicimage
	SetRect(&rect,1006,216,1007,226);
	SCRIPTMGR->GetImage(0,&gimg,&rect); //guageimage

	m_dwFontColor = 0xffffffff;

	if( m_SkillBaseInfo.wSkillIdx != 0 )
	{
		if(m_SkillBaseInfo.Level == 0)
			SetAlpha(120);
		else
			SetAlpha(255);
	}
	else
	{
		SetAlpha(255);
	}
}
void cSkillBase::Render()
{
	cIcon::Render();
	if(!m_pHighLayerImage.IsNull())
		m_pHighLayerImage.RenderSprite( NULL, NULL, 0, &m_absPos, RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
	
	if(m_SkillBaseInfo.Level > 0)
	{
		static char nums[3];
		wsprintf(nums,"%d", m_SkillBaseInfo.Level);
		if( IsMovedWnd() )
		{
			m_rect.left		= (LONG)m_absPos.x+1 ;
			m_rect.top		= (LONG)m_absPos.y+22 ;
			m_rect.right	= 1 ;
			m_rect.bottom	= 1 ;

			SetMovedWnd( FALSE ) ;
		}
		CFONT_OBJ->RenderFont( 0, nums, strlen(nums), &m_rect, m_dwFontColor );
	}

	if( ( ( cActiveSkillInfo* )m_pSkillInfo )->GetCoolTime()->IsStart() )
	{
		( ( cActiveSkillInfo* )m_pSkillInfo )->GetCoolTime()->Check();
		float delayRate = ( ( cActiveSkillInfo* )m_pSkillInfo )->GetCoolTime()->GetRemainRate();

		if( delayRate > 0.0f )
		{
			VECTOR2 vScale = { 1.0f, m_height * delayRate };
			VECTOR2 pos = { m_absPos.x, m_absPos.y + m_height*(1 - delayRate) };
			m_DelayImage.RenderSprite( &vScale, NULL, 0.0f, &pos, 0xff000000  );
		}
	}
}