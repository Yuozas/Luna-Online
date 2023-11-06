// StatusIconDlg.cpp: implementation of the CStatusIconDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StatusIconDlg.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"
#include "./Input/UserInput.h"
#include "./Interface/cFont.h"
#include "MHFile.h"
#include "ItemManager.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "Quest.h"
#include "QuestManager.h"
//#include "GuildManager.h"
#include "../[cc]skill/client/info/skillinfo.h"
#include "../[cc]skill/client/manager/skillmanager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(CStatusIconDlg)

// 080318 LUJ,	최대 아이콘 표시 개수 초기화 추가
//				경과 시간 초기화 추가
CStatusIconDlg::CStatusIconDlg() :
mMaxIconQuantity( UINT_MAX ),
mElaspedTime( gCurTime )
{
	m_pDescriptionArray = NULL;
	m_pObject = NULL;
	memset(m_IconCount,0,sizeof(WORD)*eStatusIcon_Max);
	memset(m_StatusIcon,0,sizeof(cImage*)*eStatusIcon_Max);
	memset(m_IconInfo,0,sizeof(ICONRENDERINFO)*eStatusIcon_Max);

	m_CurIconNum = 0;

	memset(m_dwRemainTime,0,sizeof(DWORD)*eStatusIcon_Max);
	memset(m_dwStartTime,0,sizeof(DWORD)*eStatusIcon_Max);

	m_nQuestIconCount = 0;

	mStatusInfoTable.Initialize(10);

	// 080318 LUJ, 아이콘 크기 초기화
	{
		const float buffIconSize = 25;

		SetIconSize( buffIconSize, buffIconSize );
	}
}

// 080318 LUJ, 중복 코드를 Release()를 사용하여 제거
CStatusIconDlg::~CStatusIconDlg()
{
	if(m_pDescriptionArray)
	{
		delete [] m_pDescriptionArray;
	}

	Release();
}


void CStatusIconDlg::AddIcon(CObject* pObject,WORD StatusIconNum, DWORD ItemIdx, DWORD dwRemainTime )
{
	if(pObject != m_pObject)
		return;

	if(StatusIconNum >= eStatusIcon_Max)
		__asm int 3;
	
	++m_IconCount[StatusIconNum];

	if( m_IconCount[StatusIconNum] == 1 )
		++m_CurIconNum;

	m_dwRemainTime[StatusIconNum] = dwRemainTime;
	m_dwStartTime[StatusIconNum] = gCurTime;
	m_IconInfo[StatusIconNum].bAlpha = FALSE;

	if( ItemIdx )
	{
		m_IconInfo[StatusIconNum].ItemIndex = ItemIdx;
		m_IconInfo[StatusIconNum].bPlus = TRUE;
		m_IconInfo[StatusIconNum].bAlpha = FALSE;
		m_IconInfo[StatusIconNum].Alpha = 0;
	}
}

void CStatusIconDlg::AddQuestTimeIcon( CObject* pObject,WORD StatusIconNum )
{
	if( pObject != m_pObject )	return;
	if(StatusIconNum >= eStatusIcon_Max)
		__asm int 3;
	
	++m_IconCount[StatusIconNum];

	if( m_IconCount[StatusIconNum] == 1 )
		++m_CurIconNum;

//	m_IconCount[StatusIconNum] = 1;

//	m_dwRemainTime[StatusIconNum] = dwRemainTime;
//	m_dwStartTime[StatusIconNum] = dwQuestIdx;
	m_IconInfo[StatusIconNum].bAlpha = FALSE;
}

void CStatusIconDlg::RemoveIcon(CObject* pObject,WORD StatusIconNum, DWORD ItemIdx)
{
	if(pObject != m_pObject)
		return;

	if(StatusIconNum >= eStatusIcon_Max)
		__asm int 3;

	if(m_IconCount[StatusIconNum] == 0)
		return;
	
	--m_IconCount[StatusIconNum];

	if( m_IconCount[StatusIconNum] == 0 )
	{
		--m_CurIconNum;

		m_dwRemainTime[StatusIconNum] = 0;
		m_dwStartTime[StatusIconNum] = 0;
		m_IconInfo[StatusIconNum].bAlpha = FALSE;
	}

	if( ItemIdx )
	{
		m_IconInfo[StatusIconNum].ItemIndex = 0;
		m_IconInfo[StatusIconNum].bPlus = FALSE;
		m_IconInfo[StatusIconNum].bAlpha = FALSE;
		m_IconInfo[StatusIconNum].Alpha = 0;
	}
}

void CStatusIconDlg::RemoveQuestTimeIcon( CObject* pObject,WORD StatusIconNum )
{
	if(pObject != m_pObject)	return;

	if(StatusIconNum >= eStatusIcon_Max)
		__asm int 3;

	if(m_IconCount[StatusIconNum] == 0)	return;
	
	--m_IconCount[StatusIconNum];
	if( m_IconCount[StatusIconNum] == 0 )
	{
		--m_CurIconNum;
		m_dwRemainTime[StatusIconNum] = 0;
		m_dwStartTime[StatusIconNum] = 0;
		m_IconInfo[StatusIconNum].bAlpha = FALSE;
	}
}

void CStatusIconDlg::RemoveAllQuestTimeIcon()
{
	if( m_IconCount[eStatusIcon_QuestTime] > 0 )
	{
		--m_CurIconNum;
	}

	m_IconCount[eStatusIcon_QuestTime] = 0;
	m_dwRemainTime[eStatusIcon_QuestTime] = 0;
	m_dwStartTime[eStatusIcon_QuestTime] = 0;
	m_IconInfo[eStatusIcon_QuestTime].bAlpha = FALSE;
}

void CStatusIconDlg::LoadDescription()
{
	CMHFile file;
	if(file.Init("Data/Interface/Windows/buf_msg.bin","rb",MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR) == FALSE)
		return;
	
	char buf[256];
	m_MaxDesc = file.GetInt();
	file.GetLine(buf,256);	// read enter
	m_pDescriptionArray = new StaticString[m_MaxDesc+1];
	for(int n=1;n<=m_MaxDesc;++n)	// 아이콘 번호가 1부터 시작한다.
	{
		file.GetLine(buf,256);
		m_pDescriptionArray[n] = buf;
	}

	file.Release();
}

void CStatusIconDlg::Init(CObject* pObject,VECTOR2* pDrawPosition,int MaxIconPerLine)
{
	memset(m_IconCount,0,sizeof(WORD)*eStatusIcon_Max);
	memset(m_StatusIcon,0,sizeof(cImage*)*eStatusIcon_Max);

	m_pObject = pObject;
	m_DrawPosition = *pDrawPosition;
	m_MaxIconPerLine = MaxIconPerLine;

	// image load
	for(int n=1;n<eStatusIcon_Max;++n)
	{
		SCRIPTMGR->GetImage( n-1, &m_StatusIcon[n] , PFT_BUFFPATH );
	}
	
	//Description Load
	if(m_pDescriptionArray == NULL)
	{
		LoadDescription();
	}

	cImage ToolTipImg;
	SCRIPTMGR->GetImage( 0, &ToolTipImg, PFT_HARDPATH );
	m_toolTip.Init( 0, TTTC_DEFAULT, &ToolTipImg, TTCLR_ITEM_CANEQUIP );
	m_toolTip.SetImageAlpha( TOOLTIP_ALPHA );

	m_CurIconNum = 0;

	if( m_nQuestIconCount > 0 )
	{
		m_IconCount[eStatusIcon_QuestTime] = m_nQuestIconCount;
		++m_CurIconNum;
		m_nQuestIconCount = 0;
	}
}

void CStatusIconDlg::Release()
{
	m_pObject = NULL;
	m_CurIconNum = 0;
	memset(m_IconCount,0,sizeof(WORD)*eStatusIcon_Max);
	memset(m_StatusIcon,0,sizeof(cImage*)*eStatusIcon_Max);

	m_nQuestIconCount = 0;

	mStatusInfoTable.SetPositionHead();
	StatusInfo* pStatusInfo = NULL;

	while( pStatusInfo = mStatusInfoTable.GetData() )
	{
		delete pStatusInfo->Image;
		delete pStatusInfo;
	}

	mStatusInfoTable.RemoveAll();

	// 080318 LUJ, 아이콘 보관 컨테이너가 가진 메모리 해제
	{
		for(
			ReservedIconList::iterator it = mReservedIconList.begin();
			mReservedIconList.end() != it;
			++it )
		{
			SAFE_DELETE( *it );
		}

		mReservedIconList.clear();
	}
}

#define ALPHASETTING(Alpha, bPlus)			{ \
	if(bPlus) Alpha+=(int)(gTickTime*0.3f);	else Alpha-=(int)(gTickTime*0.3f); \
if(Alpha>255)	{ Alpha=255; bPlus=0;	} else if(Alpha<70) { Alpha=70; bPlus=1;	} }


// 080318 LUJ, 활성 정보 체크 추가
void CStatusIconDlg::Render()
{
	if(m_pObject == NULL)
		return;

#ifdef _GMTOOL_
	if( WINDOWMGR->IsInterfaceHided() ) return;
#endif

	int mx = g_UserInput.GetMouse()->GetMouseX();
	int my = g_UserInput.GetMouse()->GetMouseY();
	
	int curx = 0;
	int cury = 0;
	VECTOR2 curpos;
	int IconNum = 0;

	mStatusInfoTable.SetPositionHead();
	StatusInfo* pStatusInfo = NULL;

	while( pStatusInfo = mStatusInfoTable.GetData() )
	{
		// 080318 LUJ, 아이콘 크기를 변수에서 가져오도록 함
		curpos.x = m_DrawPosition.x + curx * mIconSize.x;
		curpos.y = m_DrawPosition.y + cury * mIconSize.y;

		//---KES 깜빡임과 남은시간표시

		DWORD dwRemain = pStatusInfo->Time - gCurTime;
		if( dwRemain < 60*1000 )
		{
			float fAlphaDiff = (gCurTime - mElaspedTime)/*gTickTime*/*0.2f;
            
			if( pStatusInfo->fAlpha > fAlphaDiff )
				pStatusInfo->fAlpha -= fAlphaDiff;
			else
				pStatusInfo->fAlpha = 255.f;

			pStatusInfo->Image->RenderSprite(&Scale,NULL,0,&curpos, RGBA_MAKE(255, 255, 255, (DWORD)(pStatusInfo->fAlpha) ) );

			char second[16];
			wsprintf( second, "%2d", dwRemain / 1000 );

			// 080318 LUJ, 파티창에 표시되는 50% 이하 크기의 아이콘에 숫자가 표시되도 알아볼 수가 없음
			if( Scale.x > 0.7f )
			{
				RECT rc;
				SetRect( &rc, curpos.x + 12, curpos.y + 14, 1, 1 );
				CFONT_OBJ->RenderFontAlpha( 0, second, 2, &rc, RGBA_MAKE(10, 10, 10, (DWORD)(pStatusInfo->fAlpha) ) );

				SetRect( &rc, curpos.x + 11, curpos.y + 13, 1, 1 );
				//CFONT_OBJ->RenderFontShadowAlpha( 0, 1, second, 2, &rc, RGBA_MAKE(255, 255, 255, 0 ), RGBA_MAKE(0, 0, 0, 0 ), (DWORD)(pStatusInfo->fAlpha) );
				CFONT_OBJ->RenderFontAlpha( 0, second, 2, &rc, RGBA_MAKE(255, 255, 0, (DWORD)(pStatusInfo->fAlpha) ) );
			}			
		}
		else
		{
			pStatusInfo->Image->RenderSprite(&Scale,NULL,0,&curpos, 0xFFFFFFFF );
		}

		if( pStatusInfo->Count )
		{
			char count[16];
			wsprintf( count, "%d", pStatusInfo->Count );

			RECT rc;
			SetRect( &rc, curpos.x + 2, curpos.y + 2, 1, 1 );
			CFONT_OBJ->RenderFont( 0, count, strlen( count ), &rc, RGB_HALF(255, 255, 255) );
		}

		//-----------------------------

		if(++curx >= m_MaxIconPerLine)
		{
			curx = 0;
			++cury;
		}

		if( !WINDOWMGR->IsMouseOverUsed() )
		{
			int dx = int(mx - curpos.x);
			int dy = int(my - curpos.y);

			if(0 < dx && dx < mIconSize.x && 0 < dy && dy < mIconSize.y )
			{
				m_toolTip.Release();

				cSkillInfo* pInfo = SKILLMGR->GetSkillInfo( pStatusInfo->Index );

				char line[128];

				wsprintf(line, "[%s]", pInfo->GetName() );

				m_toolTip.AddLine( line, TTTC_SKILLNAME );
				m_toolTip.AddLine( "" );

				TOOLTIP_TEXT* pTooltip = ITEMMGR->GetItemToolTipInfo( pInfo->GetTooltip() );

				while( pTooltip )
				{
					m_toolTip.AddLine( pTooltip->strToolTip, TTTC_DESCRIPTION );
					pTooltip = pTooltip->pNext;
				}

				if( pStatusInfo->Time > 0 )
				{
					if( pStatusInfo->Time >= gCurTime )
					{
						DWORD RemainTime = pStatusInfo->Time - gCurTime;
					
						if( RemainTime >= 60000 )
						{
							DWORD sec = (RemainTime % 60000) / 1000 + 1;
							DWORD min = RemainTime / 60000;

							sprintf( line, "%s%d%s %d%s", CHATMGR->GetChatMsg(793), min, CHATMGR->GetChatMsg(794), sec, CHATMGR->GetChatMsg(795) );
						}
						else
							sprintf( line, "%s%d%s", CHATMGR->GetChatMsg(793), RemainTime / 1000 + 1, CHATMGR->GetChatMsg(795) );

						m_toolTip.AddLine(line, TTTC_QUESTITEM);
					}
				}

				m_toolTip.SetXY( mx+TOOLTIP_MOUSESIZE_X, my+TOOLTIP_MOUSESIZE_Y );
				WINDOWMGR->SetToolTipWindow( &m_toolTip );
			}
		}
	}

	mElaspedTime = gCurTime; //

	/*
	for(int n=1;n<eStatusIcon_Max;++n)
	{
		if(m_IconCount[n] == 0)
			continue;
		if(m_StatusIcon[n].IsNull())
			continue;

		curpos.x = m_DrawPosition.x + curx*35;
		curpos.y = m_DrawPosition.y + cury*35;

		if(m_IconInfo[n].bAlpha)
		{
			ALPHASETTING(m_IconInfo[n].Alpha, m_IconInfo[n].bPlus);
			m_StatusIcon[n].RenderSprite(NULL,NULL,0,&curpos, RGBA_MAKE(255, 255, 255, m_IconInfo[n].Alpha));
		}
		else
			m_StatusIcon[n].RenderSprite(NULL,NULL,0,&curpos, 0xffffffff);

		
		if(++curx >= m_MaxIconPerLine)
		{
			curx = 0;
			++cury;
		}

		static int preselected = 0;
		char buf[128] = { 0, };
		if( !WINDOWMGR->IsMouseOverUsed() )
		{
			int dx = int(mx - curpos.x);
			int dy = int(my - curpos.y);
			if(0 < dx && dx < 35 && 0 < dy && dy < 35)
			{
				// 사용중인 ShopItem들..
				if(m_IconInfo[n].ItemIndex)
				{
					m_toolTip.Release();				
					
					ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo((WORD)m_IconInfo[n].ItemIndex);
					if(!pItemInfo)		continue;
					char itemname[64] = { 0, };
					wsprintf( itemname, "[%s]", pItemInfo->ItemName );
					m_toolTip.AddLine(itemname, TTTC_DEFAULT);
					m_toolTip.AddLine("");

					TOOLTIP_TEXT* pTooltip = ITEMMGR->GetItemToolTipInfo( pItemInfo->ItemTooltipIdx );
					while( pTooltip )
					{
						m_toolTip.AddLine( pTooltip->strToolTip, TTTC_DESCRIPTION );
						pTooltip = pTooltip->pNext;
					}
					m_toolTip.AddLine("");
					
					SHOPITEMBASE* pInfo = ITEMMGR->GetUsedItemInfo(m_IconInfo[n].ItemIndex);
					if(pInfo)
					{
						int day, hour, minute;
						if( pItemInfo->SellPrice == eShopItemUseParam_Realtime )
						{
							stTIME EndTime;
							EndTime.SetTime(pInfo->Remaintime);

							m_toolTip.AddLine(CHATMGR->GetChatMsg(766), TTTC_DEFAULT);
							sprintf(buf, CHATMGR->GetChatMsg(767),
								EndTime.GetYear(), EndTime.GetMonth(), EndTime.GetDay(), EndTime.GetHour(), EndTime.GetMinute());
							m_toolTip.AddLine(buf, TTTC_QUESTITEM);
							
							// 수호자주문서 남은 횟수
/*							if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION  )
							{
								if( pItemInfo->Vit )
								{
									m_toolTip.AddLine("");
									sprintf( buf, CHATMGR->GetChatMsg(730), HERO->GetShopItemStats()->ProtectCount );
									m_toolTip.AddLine( buf, TTTC_QUESTITEM );
								}
							}
*//*						}
						else if( pItemInfo->SellPrice == eShopItemUseParam_Playtime )
						{
							int temp = pInfo->Remaintime;
							day = pInfo->Remaintime/(DAYTOSECOND*1000);

							temp -= day*(DAYTOSECOND*1000);
							hour = temp/(HOURTOSECOND*1000);
							temp -= hour*(HOURTOSECOND*1000);
							minute = temp/(MINUTETOSECOND*1000);
							temp -= minute*(MINUTETOSECOND*1000);
				
							m_toolTip.AddLine(CHATMGR->GetChatMsg(768), TTTC_DEFAULT);
							if(day)
								sprintf(buf, CHATMGR->GetChatMsg(769), day, hour, minute+1);
							else if(hour)
								sprintf(buf, CHATMGR->GetChatMsg(770), hour, minute+1);
							else if(minute)
								sprintf(buf, CHATMGR->GetChatMsg(771), minute+1);
							else
								sprintf(buf, CHATMGR->GetChatMsg(772));
							
							m_toolTip.AddLine(buf, TTTC_QUESTITEM);
						}
						else if( pItemInfo->SellPrice == eShopItemUseParam_Forever )
						{
/*							if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
							{
								// 수호자주문서 남은 횟수
								if( pItemInfo->Vit )
								{
									m_toolTip.AddLine( CHATMGR->GetChatMsg(749), TTTC_QUESTITEM );
									m_toolTip.AddLine("");
									sprintf( buf, CHATMGR->GetChatMsg(730), HERO->GetShopItemStats()->ProtectCount );
									m_toolTip.AddLine( buf, TTTC_QUESTITEM );
								}
							}
*//*						}
					}
				}
				else
				{
					if( n == eStatusIcon_QuestTime )
					{
						m_toolTip.Release();

						int count = 0;
						stTIME* pTime = NULL;
						char* str = NULL;
						CQuest* pQuest = NULL;
						CYHHashTable<CQuest>* pTable = QUESTMGR->GetQuestTable();
						pTable->SetPositionHead();
						while( pQuest = pTable->GetData() )
						{
							if( pQuest->IsTimeCheck() )
							{
								pTime = pQuest->GetCheckTime();
								str = QUESTMGR->GetQuestTitle( pQuest->GetQuestIdx() );
								if( pQuest->GetCheckType() == 1 || pQuest->GetCheckType() == 2 )
								{
									if( str )
										sprintf( buf, CHATMGR->GetChatMsg(1154), str );
									else
										sprintf( buf, CHATMGR->GetChatMsg(1154), "" );
								}
								else if( pQuest->GetCheckType() == 3 )
								{
									if( str )
										sprintf( buf, CHATMGR->GetChatMsg(1152), str );
									else
										sprintf( buf, CHATMGR->GetChatMsg(1152), "" );
								}
								m_toolTip.AddLine( buf,TTTC_DEFAULT );
								sprintf( buf, CHATMGR->GetChatMsg(767), pTime->GetYear(), pTime->GetMonth(), pTime->GetDay(), pTime->GetHour(), pTime->GetMinute() );
								m_toolTip.AddLine( buf, TTTC_QUESTITEM );

								++count;
							}
						}

						if( count == 0 )
							RemoveAllQuestTimeIcon();

						count = 0;
					}
					else
					{
						if( n <= m_MaxDesc )
						{
							m_toolTip = m_pDescriptionArray[n];
						
							//남은시간을 보여주자!	
							DWORD elapsed = gCurTime - m_dwStartTime[n];
							if( elapsed < m_dwRemainTime[n] )
							{
								if( m_dwRemainTime[n] - elapsed >= 60000 )
								{
									DWORD sec = ((m_dwRemainTime[n] - elapsed) % 60000) / 1000 + 1;
									DWORD min = (m_dwRemainTime[n] - elapsed) / 60000;

									sprintf( buf, CHATMGR->GetChatMsg(1363), min, sec);
								}
								else
									sprintf( buf, CHATMGR->GetChatMsg(581), ( m_dwRemainTime[n] - elapsed ) / 1000 + 1);

								m_toolTip.AddLine(buf, TTTC_QUESTITEM);
							
								if( m_dwRemainTime[n] - elapsed <= 5000 )
									m_IconInfo[n].bAlpha = TRUE;
							}
						}
						else if( n == eStatusIcon_SkipEffect || n == eStatusIcon_Hide || n == eStatusIcon_Detect )
						{
							//1356 "은신상태"
							//1357 "혜안상태"
							//1358 "무초상태"

							m_toolTip.Release();

							switch(n)
							{
							case eStatusIcon_SkipEffect:
								m_toolTip.AddLine( CHATMGR->GetChatMsg(1358),TTTC_DEFAULT );
								break;
							case eStatusIcon_Hide:
								m_toolTip.AddLine( CHATMGR->GetChatMsg(1356),TTTC_DEFAULT );
								break;
							case eStatusIcon_Detect:
								m_toolTip.AddLine( CHATMGR->GetChatMsg(1357),TTTC_DEFAULT );
								break;
							}
							
							//남은시간을 보여주자!	
							DWORD elapsed = gCurTime - m_dwStartTime[n];
							if( elapsed < m_dwRemainTime[n] )
							{
								if( m_dwRemainTime[n] - elapsed >= 60000 )
								{
									DWORD sec = ((m_dwRemainTime[n] - elapsed) % 60000) / 1000 + 1;
									DWORD min = (m_dwRemainTime[n] - elapsed) / 60000;

									sprintf( buf, CHATMGR->GetChatMsg(1363), min, sec);
								}
								else
									sprintf( buf, CHATMGR->GetChatMsg(581), ( m_dwRemainTime[n] - elapsed ) / 1000 + 1);
								
								m_toolTip.AddLine(buf, TTTC_QUESTITEM);

								if( m_dwRemainTime[n] - elapsed <= 5000 )
									m_IconInfo[n].bAlpha = TRUE;
							}
						}
						//SW060719 문파포인트
						else if( n == eStatusIcon_GuildPlustime_SuRyun || n == eStatusIcon_GuildPlustime_Skill
							|| n == eStatusIcon_GuildPlustime_Exp || n == eStatusIcon_GuildPlustime_DamageUp )
						{
							m_toolTip.Release();

							GUILDPLUSTIME_INFO* pGPlustimeInfo = GUILDMGR->GetGuildPointPlustimeList();

							DWORD GPlustimeKind = 0;
							char* GPlustimeName = NULL;
							switch( n )
							{
							case eStatusIcon_GuildPlustime_SuRyun:
								{
									GPlustimeKind = eGPT_SuRyun;
								}
								break;
							case eStatusIcon_GuildPlustime_Skill:
								{
									GPlustimeKind = eGPT_Skill;
								}
								break;
							case eStatusIcon_GuildPlustime_Exp:
								{
									GPlustimeKind = eGPT_Exp;
								}
								break;
							case eStatusIcon_GuildPlustime_DamageUp:
								{
									GPlustimeKind = eGPT_DamageUp;
								}
								break;
							default:
								{
									ASSERT(0);
								}
								break;
							}

							char Buf[128] = {0,};
							GUILDMGR->GetGuildPlustimeNameFromKind( GPlustimeKind, Buf );
							sprintf( buf, CHATMGR->GetChatMsg(1381), Buf ); 
							m_toolTip.AddLine(buf, TTTC_QUESTITEM);

							DWORD dwEndTime = GUILDMGR->GetGuildPlustimeEndTimeFromKind(GPlustimeKind);

							stTIME EndTime;
							EndTime.SetTime(dwEndTime);
							m_toolTip.AddLine(CHATMGR->GetChatMsg(766), TTTC_DEFAULT);
							sprintf(buf, CHATMGR->GetChatMsg(767),
								EndTime.GetYear(), EndTime.GetMonth(), EndTime.GetDay(), EndTime.GetHour(), EndTime.GetMinute());
							m_toolTip.AddLine(buf, TTTC_QUESTITEM);
						}
					}

					// m_IconCount[n] = 0;
					//m_toolTip.Render();
					//CFONT_OBJ->RenderFont( 0, m_pDescriptionArray[n], strlen(m_pDescriptionArray[n]), &rect, 0xffffffff );
				}

				m_toolTip.SetXY( mx+TOOLTIP_MOUSESIZE_X, my+TOOLTIP_MOUSESIZE_Y );
				WINDOWMGR->SetToolTipWindow( &m_toolTip );
			}
		}

		if( ++IconNum >= m_CurIconNum )
			break;
		
	}*/
}






// 071121 LYW - Modified.
void CStatusIconDlg::SetOneMinuteToShopItem(DWORD ItemIdx)
{
//	ITEM_INFO* pItemInfo = NULL ;
//	pItemInfo = ITEMMGR->GetItemInfo( (WORD)ItemIdx ) ;
//
//	if(!pItemInfo) return ;
//	
//	if( pItemInfo->SellPrice == eShopItemUseParam_Playtime )
//	{
//		ITEMBASE* pInfo = ITEMMGR->GetUsedItemInfo(m_IconInfo[pItemInfo->BuyPrice+1].ItemIndex) ;
//		if(!pInfo) return ; 	
//		pInfo->dwRemainMinute = MINUTETOSECOND*1000 ;
//	}
///*
//	if( pItemInfo->ItemKind & eSHOP_ITEM )
//	{
//		m_IconInfo[pItemInfo->BuyPrice+1].bAlpha = TRUE;
//		m_IconInfo[pItemInfo->BuyPrice+1].bPlus = TRUE;
//		m_IconInfo[pItemInfo->BuyPrice+1].Alpha = 0;
//	}
//*/
}



//void CStatusIconDlg::SetOneMinuteToShopItem(DWORD ItemIdx)
//{
//	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( (WORD)ItemIdx );
//	if(!pItemInfo)		return;
//	
//	if( pItemInfo->SellPrice == eShopItemUseParam_Playtime )
//	{
//		SHOPITEMBASE* pInfo = ITEMMGR->GetUsedItemInfo(m_IconInfo[pItemInfo->BuyPrice+1].ItemIndex);
//		if(!pInfo)			return;	
//		pInfo->Remaintime = MINUTETOSECOND*1000;
//	}
///*
//	if( pItemInfo->ItemKind & eSHOP_ITEM )
//	{
//		m_IconInfo[pItemInfo->BuyPrice+1].bAlpha = TRUE;
//		m_IconInfo[pItemInfo->BuyPrice+1].bPlus = TRUE;
//		m_IconInfo[pItemInfo->BuyPrice+1].Alpha = 0;
//	}
//*/
//}


// 080318 LUJ, 이미 표시 중이거나 저장 중인 아이콘인 경우 할당하지 않고 대체한다.
void CStatusIconDlg::AddSkillIcon( DWORD ID, DWORD Index, DWORD time, WORD count )
{
	// 080318 LUJ,  이미 표시 중인 아이콘이라면 정보를 대체하고 끝낸다
	{
		StatusInfo* info = mStatusInfoTable.GetData( Index );

		if( info )
		{
			info->Time	= time;
			info->Count	= count;
			info->ID	= ID;

			return;
		}
	}

	// 080318 LUJ, 이미 저장 중인 아이콘이면 정보를 대체하고 끝낸다
	for(
		ReservedIconList::iterator it = mReservedIconList.begin();
		mReservedIconList.end() != it;
		++it )
	{
		StatusInfo* info = *it;

		if( info->Index == Index )
		{
			info->Time	= time;
			info->Count	= count;
			info->ID	= ID;

			return;
		}
	}

	cSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo( Index );

	cImage* pImage = new cImage;
	SCRIPTMGR->GetImage( pSkillInfo->GetImage(), pImage, PFT_SKILLPATH );

	StatusInfo* pStatusInfo = new StatusInfo;

	pStatusInfo->Image = pImage;
	pStatusInfo->Index = Index;
	pStatusInfo->Time = time;
	pStatusInfo->ID = ID;
	pStatusInfo->Count = count;
	
	// 080318 LUJ, 최대 아이콘 개수가 초과된 경우 저장하고 끝낸다
	if( mStatusInfoTable.GetDataNum() >= mMaxIconQuantity )
	{
		mReservedIconList.push_back( pStatusInfo );
	}
	else
	{
		mStatusInfoTable.Add( pStatusInfo, pStatusInfo->Index );
	}
}

// 080318 LUJ, 저장 중인 아이콘도 삭제할 수 있도록 함
void CStatusIconDlg::RemoveSkillIcon( DWORD ID, DWORD Index )
{
	// 080318 LUJ, 저장 중인 아이콘이면 삭제
	for(
		ReservedIconList::iterator it = mReservedIconList.begin();
		mReservedIconList.end() != it;
		++it )
	{
		StatusInfo* info = *it;

		if( info->ID == ID )
		{
			SAFE_DELETE( info );
			mReservedIconList.erase( it );

			break;
		}
	}

	StatusInfo* pStatusInfo = mStatusInfoTable.GetData( Index );

	if( pStatusInfo && ID == pStatusInfo->ID )
	{
		delete pStatusInfo->Image;
		delete pStatusInfo;
	
		mStatusInfoTable.Remove( Index );
	}

	// 080318 LUJ, 저장된 아이콘이 있을 경우 표시 정보 컨테이너로 이동시킨다
	if( !	mReservedIconList.empty() &&
			mStatusInfoTable.GetDataNum() < mMaxIconQuantity )
	{
		StatusInfo* info = mReservedIconList.front();
		mReservedIconList.pop_front();

		if( info )
		{
			mStatusInfoTable.Add( info, info->Index );
		}
	}
}

// 080318 LUJ, 저장된 아이콘의 정보도 갱신한다
void CStatusIconDlg::UpdateSkillTime( DWORD ID, DWORD Index, DWORD time )
{
	// 080318 LUJ, 저장 중인 아이콘 정보 갱신
	for(
		ReservedIconList::iterator it = mReservedIconList.begin();
		mReservedIconList.end() != it;
		++it )
	{
		StatusInfo* info = *it;

		if( info->ID == ID )
		{
			info->Time = time;

			break;
		}
	}

	StatusInfo* pStatusInfo = mStatusInfoTable.GetData( Index );

	if( pStatusInfo && ID == pStatusInfo->ID )
	{
		pStatusInfo->Time = time;
	}
}


// 080318 LUJ, 저장된 아이콘의 정보도 갱신한다
void CStatusIconDlg::UpdateSkillCount( DWORD ID, DWORD Index, WORD count )
{
	// 080318 LUJ, 저장 중인 아이콘 정보 갱신
	for(
		ReservedIconList::iterator it = mReservedIconList.begin();
		mReservedIconList.end() != it;
		++it )
	{
		StatusInfo* info = *it;

		if( info->ID == ID )
		{
			info->Count = count;

			break;
		}
	}

	StatusInfo* pStatusInfo = mStatusInfoTable.GetData( Index );

	if( pStatusInfo && ID == pStatusInfo->ID )
	{
		pStatusInfo->Count = count;
	}
}


// 080318 LUJ, 표시 아이콘 크기 설정
void CStatusIconDlg::SetIconSize( float width, float height )
{
	mIconSize.x	= width;
	mIconSize.y	= height;

	Scale.x	= width / DEFAULT_ICONSIZE;
	Scale.y	= height / DEFAULT_ICONSIZE;
}


// 080318 LUJ, 현재 보유한 스킬을 자료구조에 복사한다
void CStatusIconDlg::Copy( std::list< StatusInfo >& skillList )
{
	// 080318 LUJ, 표시 중인 스킬 복사
	{
		mStatusInfoTable.SetPositionHead();

		for(
			StatusInfo* source = 0;
			source = mStatusInfoTable.GetData(); )
		{
			if( source )
			{
				StatusInfo dest = { 0 };
				dest.ID		= source->ID;
				dest.Index	= source->Index;
				dest.Time	= source->Time;
				dest.Count	= source->Count;

				skillList.push_back( dest );
			}
		}
	}	

	// 080318 LUJ, 저장 중인 스킬 복사
	for(
		ReservedIconList::iterator it = mReservedIconList.begin();
		mReservedIconList.end() != it;
		++it )
	{
		StatusInfo* source = *it;
		
		if( source )
		{
			StatusInfo dest = { 0 };
			dest.ID		= source->ID;
			dest.Index	= source->Index;
			dest.Time	= source->Time;
			dest.Count	= source->Count;

			skillList.push_back( dest );
		}		
	}
}
