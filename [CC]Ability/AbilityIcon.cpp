/**************±æ◊ ‘¥¿¥◊‘ª•¡™Õ¯ ø™∑¢’ﬂ÷––ƒÃ· æΩ˚…Ã“µ Ωˆ—ßœ∞*******************/


/*************’æ≥§ –«∆⁄∞À ∏––ªπ„¥Û”√ªß∂‘ø™∑¢’ﬂ÷––ƒ”Œœ∑◊ ‘¥…Á«¯µƒ¥Û¡¶÷ß≥÷*****************/
/**************±æ◊ ‘¥¿¥◊‘ª•¡™Õ¯ ø™∑¢’ﬂ÷––ƒÃ· æΩ˚…Ã“µ Ωˆ—ßœ∞*******************/


/*************’æ≥§ –«∆⁄∞À ∏––ªπ„¥Û”√ªß∂‘ø™∑¢’ﬂ÷––ƒ”Œœ∑◊ ‘¥…Á«¯µƒ¥Û¡¶÷ß≥÷*****************/
// AbilityIcon.cpp: implementation of the CAbilityIcon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AbilityIcon.h"
#include "Ability.h"
#include "AbilityInfo.h"
#include "./interface/cFont.h"
#include "AbilityCommonHeader.h"
#include "ObjectManager.h"
#include "AbilityManager.h"
#include "ChatManager.h"

#include "./interface/cScriptManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CAbilityIcon::CAbilityIcon()
{
	m_type = WT_ABILITYICON;
	m_bAbilityActive = FALSE;
}

CAbilityIcon::~CAbilityIcon()
{	

}

void CAbilityIcon::UpdateState(int State,int Level)
{
	m_Level = Level;
	m_State = State;
}

void CAbilityIcon::Render()
{
	switch(m_State)
	{
	// 06. 01. æÓ∫Ù∏Æ∆º√¢ ¿Œ≈Õ∆‰¿ÃΩ∫ ºˆ¡§ - ¿Ãøµ¡ÿ
	// æ»∫∏¿Ã¥¬ ¡∂∞« - « ø‰∑π∫ß 100 or »≠∞Ê±ÿ∏∂ ¡∂∞« πÃ« 
	case eAIS_NotUsable:
		{
			return;
		}
	// æÓµ”∞‘ ∫∏¿Ã±‚ - ∑π∫ß ¡¶«—
	case eAIS_NotAvailable:
		{

			SetMovable(FALSE);
			SetAlpha(125);
			cIcon::Render();

			return;
		}
		break;
	// 06. 01. æÓ∫Ù∏Æ∆º√¢ ¿Œ≈Õ∆‰¿ÃΩ∫ ºˆ¡§ - ¿Ãøµ¡ÿ

	// ¬Ô¿ªºˆ ¿÷¥¬ Ω∫≈≥¿∫ ∏µŒ ¡§ªÛ¿˚¿∏∑Œ ∫∏¿Œ¥Ÿ
	// ¥Ÿ∏∏ ºˆ∑√ æ»«— Ω∫≈≥¿∫ 0¿ª ∞∞¿Ã ¬ÔæÓ¡ÿ¥Ÿ
	case eAIS_OnlyVisible:
		{
			SetMovable(FALSE);
//			SetAlpha(125);
			SetAlpha(254);
			cIcon::Render();


			static char num[3];
			DWORD color = RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100);

			wsprintf(num,"%2d", 0);
			RECT rect={(LONG)m_absPos.x+27, (LONG)m_absPos.y+29, 1,1};
			CFONT_OBJ->RenderFont(0,num,strlen(num),&rect,color);
		}
		break;
	case eAIS_Usable:

		{
			if( m_pAbilityInfo->GetUseType() == eAUT_Passive && m_pAbilityInfo->GetKind() != eAGK_Job )
				SetMovable(FALSE);
			else
				SetMovable(TRUE);
			SetAlpha(254);
			cIcon::Render();

			RenderInfo(&m_absPos);
			
			//m_pAbilityInfo->m_AbilityInfo.Ability_Grade_exp
			/*


			if( m_Level != 1 || (int)m_pAbilityInfo->GetAbilityGradeExp( m_Level + 1 ) != -1 )
			{
				static char num[3];
				WORD level = m_Level;
				DWORD color = RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100);

				if( m_pAbilityInfo->GetKind() == eAGK_Battle &&					
					HERO->GetShopItemStats()->AddSung )
				{
					level += HERO->GetShopItemStats()->AddSung;
					color = TTTC_SHOPITEMABILITY;
				}

				if(m_MaxLevel > level)
					wsprintf(num,"%2d", level);
				// 06. 01. æÓ∫Ù∏Æ∆º√¢ ¿Œ≈Õ∆‰¿ÃΩ∫ ºˆ¡§ - ¿Ãøµ¡ÿ
				// √÷¡æ∑π∫ß µµ¥ﬁΩ√ Ë«¿⁄ √‚∑¬
				else


					wsprintf(num,"%s", CHATMGR->GetChatMsg(179));

				RECT rect={(LONG)m_absPos.x+27, (LONG)m_absPos.y+29, 1,1};
				CFONT_OBJ->RenderFont(0,num,strlen(num),&rect,color);
			}


			CAbility* pAblilty = HERO->GetAbilityGroup()->GetAbility(m_pAbilityInfo->GetIdx());
			if(pAblilty == NULL)
				return;
			


			if( HERO )
			if( HERO->GetKyungGongMode() )
			{
				WORD KyungGongIdx = pAblilty->GetAbilityInfo()->GetInfo()->Ability_effect_Param1;
				if( KyungGongIdx == HERO->GetKyungGongGrade() )
				{
					static int TICK_PER_IMAGE = 500;
					m_CurScrollImage+=gTickTime;
					int curImage = m_CurScrollImage % (TICK_PER_IMAGE*3);
					curImage /= TICK_PER_IMAGE;
					m_ScrollImg[curImage].RenderSprite(NULL,NULL,0,&m_absPos,0xffffffff);
				}
			}
			*/
/*	
			if(pAblilty->IsActivating() == TRUE)
			{
				static int TICK_PER_IMAGE = 500;
				m_CurScrollImage+=gTickTime;
				int curImage = m_CurScrollImage % (TICK_PER_IMAGE*3);
				curImage /= TICK_PER_IMAGE;
				m_ScrollImg[curImage].RenderSprite(NULL,NULL,0,&m_absPos,0xffffffff);
			}
*/

/*			//KES ¿Ã∞≈ ∏”«œ¥¬∞≈¡ˆ?
			if(m_bAbilityActive)
			{
				// Active √≥∏Æ
				static char num[3];			//KES confirm ???
				wsprintf(num,"Active");
				RECT rect={(LONG)m_absPos.x+2, (LONG)m_absPos.y+29, 1,1};
				CFONT_OBJ->RenderFont(0,num,strlen(num),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
			}

*/
		}
		break;
	}
}


void CAbilityIcon::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{

	cIcon::Init(x,y,wid,hei,basicImage,ID);
	
	m_CurScrollImage = 0;
	SCRIPTMGR->GetImage(95, &m_ScrollImg[0], PFT_HARDPATH);
	SCRIPTMGR->GetImage(96, &m_ScrollImg[1], PFT_HARDPATH);
	SCRIPTMGR->GetImage(97, &m_ScrollImg[2], PFT_HARDPATH);	
}

void CAbilityIcon::InitAbilityIcon(CAbilityInfo* pInfo)


{
	m_State = eAIS_NotAvailable;
	m_pAbilityInfo = pInfo;
	m_bAbilityActive = FALSE;

	// 06. 01. æÓ∫Ù∏Æ∆º√¢ ¿Œ≈Õ∆‰¿ÃΩ∫ ºˆ¡§ - ¿Ãøµ¡ÿ
	// √÷¡æ∑π∫ß ∞ËªÍ
	for(m_MaxLevel = 0; m_MaxLevel < MAX_ABILITY_LEVEL; m_MaxLevel++)
	{
		if(m_pAbilityInfo->GetInfo()->Ability_Grade_exp[m_MaxLevel] == -1)
			break;
	}
}

POSTYPE	CAbilityIcon::GetQuickPosition()
{
	CAbility* pAbility = HERO->GetAbilityGroup()->GetAbility( m_pAbilityInfo->GetIdx() );
	if( !pAbility )
		return 0;
	return pAbility->GetQuickPosition();
}

POSTYPE	CAbilityIcon::GetPosition()

{
	return m_pAbilityInfo->GetAbsPosition();
}

WORD CAbilityIcon::GetItemIdx()	//quick¿ª ¿ß«— ¿˝¥Î index∞™¿Ã¥Ÿ.
{
	return m_pAbilityInfo->GetIdx() + MIN_ABILITY_INDEX;
}

void CAbilityIcon::SetQuickPosition(POSTYPE pos)
{
	ABILITYMGR->SetAbilityQuickPosition( m_pAbilityInfo->GetIdx(), pos, HERO->GetAbilityGroup() );
}

void CAbilityIcon::RenderInfo(VECTOR2* pPos)
{
	if( m_Level != 1 || (int)m_pAbilityInfo->GetAbilityGradeExp( m_Level + 1 ) != -1 )
	{
		static char num[3];
		WORD level = m_Level;
		DWORD color = RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100);


		if( m_pAbilityInfo->GetKind() == eAGK_Battle &&					
			HERO->GetShopItemStats()->AddSung )
		{
			level += HERO->GetShopItemStats()->AddSung;
			color = TTTC_SHOPITEMABILITY;
		}

		if(m_MaxLevel > level)
			wsprintf(num,"%2d", level);
		// 06. 01. æÓ∫Ù∏Æ∆º√¢ ¿Œ≈Õ∆‰¿ÃΩ∫ ºˆ¡§ - ¿Ãøµ¡ÿ
		// √÷¡æ∑π∫ß µµ¥ﬁΩ√ Ë«¿⁄ √‚∑¬
		else
			wsprintf(num,"%s", CHATMGR->GetChatMsg(179));

		RECT rect={(LONG)(pPos->x)+27, (LONG)(pPos->y)+29, 1,1};
		CFONT_OBJ->RenderFont(0,num,strlen(num),&rect,color);
	}

	CAbility* pAblilty = HERO->GetAbilityGroup()->GetAbility(m_pAbilityInfo->GetIdx());
	if(pAblilty == NULL)
		return;


	if( HERO )

		if( HERO->GetKyungGongMode() )
		{
			WORD KyungGongIdx = pAblilty->GetAbilityInfo()->GetInfo()->Ability_effect_Param1;
			if( KyungGongIdx == HERO->GetKyungGongGrade() )
			{
				static int TICK_PER_IMAGE = 500;
				m_CurScrollImage+=gTickTime;
				int curImage = m_CurScrollImage % (TICK_PER_IMAGE*3);
				curImage /= TICK_PER_IMAGE;

				m_ScrollImg[curImage].RenderSprite(NULL,NULL,0,pPos,0xffffffff);
			}
		}


}





