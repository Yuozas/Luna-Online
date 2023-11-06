#include "stdafx.h"
#include "cListDialog.h"
#include "cButton.h"
#include "cGuageBar.h"
#include "cFont.h"
#include "../Input/Mouse.h"
#include "cWindowManager.h"
#include "cScriptManager.h"
#include "cIcon.h"
#include "./Interface/GameNotifyManager.h"
#include "WindowIDEnum.h"

cListDialog::cListDialog() :
mToolTipIcon( 0 )
{
	m_lLineHeight		= LINE_HEIGHT;		//default
	m_lLineNum			= 0;
	m_lTopListItemIdx	= 0;
	memset(&m_textRelRect, 0, sizeof(RECT));
	m_minMiddleNum		= 0;
	m_maxMiddleNum		= 0;
	m_wMaxLine			= 50;
	m_middleNum			= 0;
	m_topHeight			= m_downHeight = m_middleHeight = 0;

	m_type				= WT_LISTDIALOG;

	m_pLengthLongBtn	= m_pLengthShortBtn = NULL;
	m_pUpwardBtn		= m_pDownwardBtn = NULL;
	m_pGuageBar			= NULL;

	mSelectedRowIdx	= -1;
	//mPressedRowIndex	= -1;
	m_ClickSelected		= -1;
	m_fGap				= 0.0f;

	m_bAutoScroll		= TRUE;
	m_bShowSelect		= FALSE;

	// 061209 LYW --- Add boolean variable wether show scroll part or not.
	m_bShowScroll		= FALSE ;

	// 061209 LYW --- Add new style for list dialog.
	m_bStyle			= LDS_NORMAL ;
	m_nAlign			= TXT_LEFT;

	// 070620 LYW --- cListDialog : Add columncolor for list dialog.
	m_dwColumnColor			= 0xffffffff ;
	m_dwSelectColumnColor	= 0xffffffff ;

	// 071017 LYW --- cListDialog : Initialize font index to render shadow.
	m_wShadowFont = 0 ;

	// 071018 LYW --- cListDialog : Initialize shadow distance to use render shadowfont.
	m_nShadowDistance = 1 ;
}

cListDialog::~cListDialog()
{
	RemoveAll();

	for(
		PTRLISTPOS pos = m_NoticeList.GetHeadPosition();
		pos;
	)
	{
		sGAMENOTIFY_MSG* pGameNotifyMsg = ( sGAMENOTIFY_MSG* )m_NoticeList.GetNext( pos );

		SAFE_DELETE( pGameNotifyMsg );
	}
}

void cListDialog::Add(cWindow * btn)
{
	if(btn->GetType() == WT_BUTTON)
	{
		if(!m_pUpwardBtn)
		{
			m_pUpwardBtn = (cButton *)btn;
		}
		else if(!m_pDownwardBtn)
		{
			m_pDownwardBtn = (cButton *)btn;
		}
		else if(!m_pLengthLongBtn)
		{
			m_pLengthLongBtn = (cButton *)btn;
		}
		else if(!m_pLengthShortBtn)
		{
			m_pLengthShortBtn = (cButton *)btn;
		}
	}
	else if(btn->GetType() == WT_GUAGEBAR)
	{
		m_pGuageBar = (cGuageBar *)btn;
	}

	cDialog::Add(btn);
}

void cListDialog::Init(LONG x, LONG y, WORD wid, cImage * topImage, WORD topHei, cImage * middleImage, WORD middleHei, cImage * downImage, WORD downHei, cImage * overImage, BYTE num, LONG ID)
{
	// #POINT���� �Է��� height�� ������� �ʴ´�
	m_TopImage		= *topImage; 
	m_MiddleImage	= *middleImage; 
	m_DownImage		= *downImage;
	m_OverImage		= *overImage;

	m_topHeight		= topHei; m_middleHeight = middleHei; m_downHeight = downHei;
	m_middleNum		= num;
	int hei			= topHei + downHei + ( num * middleHei );

	cDialog::Init(x,y,wid,hei,NULL,ID);
}

void cListDialog::InitList(WORD wMaxLine, RECT * textClippingRect)
{
	m_wMaxLine = wMaxLine;
	CopyRect(&m_textRelRect, textClippingRect);

	// m_middleNum�� Init()���� ���õ� ��
	// InitList�տ� m_minMiddleNum, m_maxMiddleNum�� �����Ǿ� �־�� �Ѵ�
	Resize( m_middleNum );
}


BOOL cListDialog::Resize(BYTE middleNum)
{
	if(middleNum > m_maxMiddleNum) return FALSE;
	if(middleNum < m_minMiddleNum) return FALSE;

	int var_len = (middleNum - m_middleNum)*m_middleHeight;
	m_middleNum = middleNum;
	m_textRelRect.bottom += var_len;
	m_height = m_topHeight+m_downHeight+(m_middleNum*m_middleHeight);
	//	m_lLineHeight = LINE_HEIGHT;		// �ӽ� �ؽ�Ʈ ����
	m_lLineNum = (m_textRelRect.bottom-m_textRelRect.top)/m_lLineHeight;
	//	m_lLineNum = m_middleNum;
	return TRUE;
}

BOOL cListDialog::IsMaxLineOver()
{
	if(m_lLineNum > GetItemCount())
		return FALSE;
	else
		return TRUE;
}

DWORD cListDialog::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;

	if( IsMaxLineOver() )
	{	
		if(m_pUpwardBtn)
		{
			m_pUpwardBtn->SetActive(TRUE);
		}
		if(m_pDownwardBtn)
		{
			m_pDownwardBtn->SetActive(TRUE);
		}

		if(m_pGuageBar)
		{
			m_pGuageBar->SetActive(TRUE);
			m_pGuageBar->ActionEvent(mouseInfo);
		}

	}
	else if( m_bAutoScroll )
	{
		if(m_pUpwardBtn)
		{
			m_pUpwardBtn->SetActive(FALSE);
		}
		if(m_pDownwardBtn)
		{
			m_pDownwardBtn->SetActive(FALSE);
		}
		if(m_pGuageBar)
		{
			m_pGuageBar->SetActive(FALSE);
		}
	}

	DWORD we2 = WE_NULL;

	if(m_pLengthLongBtn)
	{
		we2 = m_pLengthLongBtn->ActionEvent(mouseInfo);
		if(we2 & WE_BTNCLICK)
			onSize(TRUE);
	}
	if(m_pLengthShortBtn)
	{
		we2 = m_pLengthShortBtn->ActionEvent(mouseInfo);
		if(we2 & WE_BTNCLICK)
			onSize(FALSE);
	}

	if(m_pUpwardBtn)
	{
		we2 = m_pUpwardBtn->ActionEvent(mouseInfo);
		if(we2 & WE_BTNCLICK)
			OnUpwardItem();
	}

	if(m_pDownwardBtn)
	{
		we2 = m_pDownwardBtn->ActionEvent(mouseInfo);
		if(we2 & WE_BTNCLICK)
			OnDownwardItem();
	}

	LONG cnt = GetItemCount();
	int itemCnt = cnt - m_lLineNum;
	if(itemCnt > 0)
	{
		if(m_pGuageBar)
			if(m_pGuageBar->IsDrag() )
			{
				m_lTopListItemIdx = (LONG)(((float)itemCnt)*m_pGuageBar->GetCurRate());
			}
	}
	else
		m_lTopListItemIdx = 0;

	// LBS 03.10.20  Mouse�� ���� üũ�� �Ѵ�. (�� Line�� ���� check)
	we |= cDialog::ActionEvent(mouseInfo);

	if( PtInWindow((LONG)mouseInfo->GetMouseX(), (LONG)mouseInfo->GetMouseY()) )
	{
		int Wheel = mouseInfo->GetWheel();

		if(Wheel)
		{
			if( Wheel > 0 )
			{
				OnUpwardItem();
			}
			else
			{
				OnDownwardItem();
			}
		}
	}

	//	LONG x = mouseInfo->GetMouseX();
	//	LONG y = mouseInfo->GetMouseY();

	//	if( mouseInfo->LButtonDown() )
	//	if( PtInWindow( x, y ) /*&& !WINDOWMGR->IsMouseDownUsed()*/ )
	//		we |= WE_LBTNCLICK;
	ListMouseCheck( mouseInfo );

	return we;
}

BOOL cListDialog::PtInWindow(LONG x, LONG y)
{
	if( !m_bActive ) return FALSE;

	WORD height = m_topHeight + m_middleNum * m_middleHeight; 
	if(m_absPos.x > x || m_absPos.y > y || m_absPos.x+m_width < x || m_absPos.y+height < y)
		return FALSE;
	else
		return TRUE;
}

void cListDialog::ListMouseCheck( CMouse * mouseInfo )
{
	const int selIdx = PtIdxInRow( mouseInfo->GetMouseX(), mouseInfo->GetMouseY() );

	mToolTipIcon = 0;

	if( selIdx < m_lLineNum && 0 <= selIdx )
	{
		mSelectedRowIdx = selIdx;

		PTRLISTPOS pos = m_ListItem.FindIndex( selIdx + m_lTopListItemIdx );
		if( pos == NULL ) return;

		const ITEM* item = ( ITEM* )m_ListItem.GetAt(pos);

		if( item )
		{
			// 080417 LUJ, Ȱ�� ������ ��쿡�� ������ ������ �ǵ��� ��
			if(		mouseInfo->LButtonDown() &&
				!	IsDisable() )
			{
				m_ClickSelected  = selIdx + m_lTopListItemIdx;

				cbWindowFunc(m_ID, m_pParent, WE_ROWCLICK);	//PPP
			}

			// 080225 LUJ, �ش� �ε����� �ش��ϴ� ������ ������ �������� �� �ְ� �����͸� ���ε��Ѵ�
			for(
				ToolTipList::iterator it = mToolTipList.begin();
				mToolTipList.end() != it;
			++it )
			{
				ToolTip& toolTip = *it;

				if( toolTip.mText == item->string )
				{
					mToolTipIcon = toolTip.mIcon;
					mToolTipIcon->SetAbsXY( mouseInfo->GetMouseX(), mouseInfo->GetMouseY() );
					break;
				}
			}
		}	
	}
	else
	{
		mSelectedRowIdx = -1;
	}
}


const char* cListDialog::GetClickedItem()
{
	PTRLISTPOS pos = m_ListItem.FindIndex( m_ClickSelected );

	if( ! pos )
	{
		return "";
	}

	const ITEM* item = ( ITEM* )m_ListItem.GetAt(pos);

	return item ? item->string : "";
}


void cListDialog::Render()																// ����Ʈ ���̾�α׸� �����ϴ� �Լ�.
{
	if(!m_bActive) return;																// Ȱ��ȭ ���°� �ƴ϶��, ���� ó���� �Ѵ�.
	cDialog::RenderWindow();															// ���̾�α׸� �����Ѵ�.

	switch(m_bStyle)																	// ����Ʈ ���̾�α� ��Ÿ���� Ȯ���Ѵ�.
	{
	case LDS_NORMAL :		RenderNormalList() ;	break ;								// �⺻ ����Ʈ ���̾�α׶��, �⺻ ���� ������� �����Ѵ�.
	case LDS_SERVERLIST :	RenderServerList() ;	break ;								// ���� ����Ʈ ������� �����Ѵ�.
	case LDS_FONTSHADOW :	RenderShadowList() ;	break ;								// ����Ʈ ���ڰ� �׸��ڰ� ����� ������� �����Ѵ�.
	case LDS_NOTICE_QUEST :	RenderQuestNotice() ;	break ;								// ���� ������� �����Ѵ�.
	}

	if( m_bShowScroll )																	// ��ũ���� �����ִ� �����,
	{
		cDialog::RenderComponent();			//component render							// ��ũ�� ���� ���� ��Ʈ�ѵ��� �����Ѵ�.
	}

	// 080225 LUJ, ������ �������Ѵ�.
	if( mToolTipIcon )
	{
		mToolTipIcon->Render();
		mToolTipIcon->ToolTipForcedRender();
	}
}

void cListDialog::RenderNormalList()													// �⺻ ������� �����ϴ� �Լ�.
{
	static VECTOR2 m_tmpPos;
	m_TopImage.RenderSprite(NULL,NULL,0,&m_absPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));

	m_tmpPos.x = m_absPos.x;
	m_tmpPos.y = m_absPos.y+m_topHeight;
	// 061211 LYW --- Add serverlist part.
	if( m_bStyle != LDS_SERVERLIST )
		for(int j = 0 ; j < m_middleNum ; j++)
		{
			m_MiddleImage.RenderSprite(NULL,NULL,0,&m_tmpPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
			m_tmpPos.y += m_middleHeight;
		}

		m_DownImage.RenderSprite(NULL,NULL,0,&m_tmpPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));

		// Over Image Display 
		if( mSelectedRowIdx != -1 )
		{
			// 061227 LYW --- Delete this code.
			/*
			m_tmpPos.x = m_absPos.x + 2;
			m_tmpPos.y = m_absPos.y + m_textRelRect.top + (((float)mSelectedRowIdx)*m_lLineHeight) - m_fGap;
			m_OverImage.RenderSprite(&m_OverImageScale,NULL,0,&m_tmpPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
			*/
			// 061211 LYW --- Add serverlist part.
			if( m_bStyle == LDS_SERVERLIST )
			{
				m_tmpPos.x = m_absPos.x - 2;
				// 061227 LYW --- Change this code.
				//m_tmpPos.y = m_absPos.y + m_textRelRect.top + (((float)mSelectedRowIdx)*m_lLineHeight) - m_fGap - 12;
				m_tmpPos.y = m_absPos.y + m_textRelRect.top + (((float)mSelectedRowIdx)*18) - m_fGap - 12 + 4;
				m_MiddleImage.RenderSprite(NULL,NULL,0,&m_tmpPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
			}
			else	// 061227 LYW --- Move this code from up line to here.
			{
				m_tmpPos.x = m_absPos.x + 2;
				m_tmpPos.y = m_absPos.y + m_textRelRect.top + (((float)mSelectedRowIdx)*m_lLineHeight) - m_fGap;
				m_OverImage.RenderSprite(&m_OverImageScale,NULL,0,&m_tmpPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
			}
		}
		//

		// List Text Display //////////////////////////////////////////////////////////////

		PTRLISTPOS pos = m_ListItem.FindIndex( m_lTopListItemIdx );
		ITEM* item = NULL;
		LONG i = 0;

		// ���� �߰��� ���̶���Ʈ ���� �־��� ���(Item�� Line�� üũ)
		// RaMa 04.8.3
		int bline = -1;
		int blinelen = 0;
		int line = -1;
		int linelen = 0;
		DWORD dwFontColor = 0xffffffff;

		while(pos)
		{
			item = (ITEM*)m_ListItem.GetNext(pos);
			if(line < m_lLineNum-1)
				//		if(i < m_lLineNum)
			{
				if(CFONT_OBJ->IsCreateFont(m_wFontIdx))
				{
					// ������ ���� �����̸� ������ ���̸� ���Ѵ�.
					if(item->line != -1 && item->line == bline)
					{					
						linelen += blinelen;
						blinelen = strlen(item->string)*CFONT_OBJ->GetTextWidth(m_wFontIdx);
					}
					// ���ο� �����̸�
					else if(item->line != -1 && item->line != bline)
					{
						linelen = 0;
						++line;
						bline = item->line;
						blinelen = strlen(item->string)*CFONT_OBJ->GetTextWidth(m_wFontIdx);
					}
					// �Ϲ����� ���
					else
					{					
						linelen = 0;
						++line;
					}

					// 061209 LYW --- Modified render part.
					/*
					RECT rect={(LONG)m_absPos.x+m_textRelRect.left+linelen, (LONG)m_absPos.y+m_textRelRect.top + (m_lLineHeight*line), 1,1};

					if( (m_ClickSelected-m_lTopListItemIdx) == line && (m_bShowSelect == TRUE))
					CFONT_OBJ->RenderFont(m_wFontIdx,item->string,lstrlen(item->string),&rect,RGBA_MERGE(0xffffff00, m_alpha * m_dwOptionAlpha / 100));
					else
					CFONT_OBJ->RenderFont(m_wFontIdx,item->string,lstrlen(item->string),&rect,RGBA_MERGE(item->rgb, m_alpha * m_dwOptionAlpha / 100));
					*/
					if( m_bStyle == LDS_NORMAL )
					{
						RECT rect={(LONG)m_absPos.x+m_textRelRect.left+linelen, (LONG)m_absPos.y+m_textRelRect.top + (m_lLineHeight*line), 1,1};

						if( (m_ClickSelected-m_lTopListItemIdx) == line && (m_bShowSelect == TRUE))
							CFONT_OBJ->RenderFont(m_wFontIdx,item->string,lstrlen(item->string),&rect,RGBA_MERGE(0xffffff00, m_alpha * m_dwOptionAlpha / 100));
						else
							CFONT_OBJ->RenderFont(m_wFontIdx,item->string,lstrlen(item->string),&rect,RGBA_MERGE(item->rgb, m_alpha * m_dwOptionAlpha / 100));
					}
					else
					{
						// 061209 LYW --- Added align for text part.
						LONG TxtPosY = (LONG)m_absPos.y + linelen;
						LONG TxtPosX = 0 ;
						RECT rect ;

						if( m_nAlign & TXT_LEFT )
						{
							TxtPosX = (LONG)m_absPos.x + linelen;
						}
						else if( m_nAlign & TXT_RIGHT )
						{
							TxtPosX = (LONG)m_absPos.x + m_width 
								- CFONT_OBJ->GetTextExtentEx( m_wFontIdx, item->string, lstrlen(item->string) ) - linelen;

						}
						else	//TXT_CENTER
						{
							TxtPosX = (LONG)m_absPos.x + ( m_width 
								- CFONT_OBJ->GetTextExtentEx( m_wFontIdx, item->string, lstrlen(item->string) ) ) / 2;
						}

						SetRect( &rect, TxtPosX,
							// 061227 LYW --- Change this code.
							//TxtPosY + m_lLineHeight * i, 1, 1 );
							TxtPosY + 18 * i, 1, 1 );

						if( (m_ClickSelected-m_lTopListItemIdx) == line && (m_bShowSelect == TRUE))
						{
							CFONT_OBJ->RenderFont( m_wFontIdx, item->string, lstrlen(item->string), &rect, 
								// 061227 LYW --- Change this code.
								//RGBA_MERGE( 0xffffff00, m_alpha * m_dwOptionAlpha / 100 ) );
								RGBA_MERGE( m_dwSelectColumnColor, m_alpha * m_dwOptionAlpha / 100 ) );
						}
						else
						{
							CFONT_OBJ->RenderFont( m_wFontIdx, item->string, lstrlen(item->string), &rect, 
								// 061227 LYW --- Change this code.
								//RGBA_MERGE( item->rgb, m_alpha * m_dwOptionAlpha / 100 ) );
								RGBA_MERGE( m_dwColumnColor, m_alpha * m_dwOptionAlpha / 100 ) );
						}
					}


					/*				// �ٸ� �����̸� ���� �ٲ��ش�.				
					RECT rect={(LONG)m_absPos.x+m_textRelRect.left, (LONG)m_absPos.y+m_textRelRect.top + (m_lLineHeight*i), 1,1};
					//rect.right = rect.left + 1;
					//rect.bottom = rect.top + 1;

					CFONT_OBJ->RenderFont(m_wFontIdx,item->string,lstrlen(item->string),&rect,RGBA_MERGE(item->rgb, m_alpha * m_dwOptionAlpha / 100));				
					*/
				}
				i++;
			}
			else
				break;
		}

		//static VECTOR2 m_tmpPos;
		//DWORD dwImageColor = RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100) ;

		//m_TopImage.RenderSprite(NULL,NULL,0,&m_absPos,dwImageColor);

		//m_tmpPos.x = m_absPos.x;
		//m_tmpPos.y = m_absPos.y+m_topHeight;

		//for(int j = 0 ; j < m_middleNum ; j++)
		//{
		//	m_MiddleImage.RenderSprite(NULL,NULL,0,&m_tmpPos,dwImageColor);
		//	m_tmpPos.y += m_middleHeight; 
		//}

		//m_DownImage.RenderSprite(NULL,NULL,0,&m_tmpPos,dwImageColor);

		//// Over Image Display 
		//if( mSelectedRowIdx != -1 )
		//{
		//	if( m_bStyle == LDS_SERVERLIST )
		//	{
		//		m_tmpPos.x = m_absPos.x - 2;
		//		m_tmpPos.y = m_absPos.y + m_textRelRect.top + (((float)mSelectedRowIdx)*18) - m_fGap - 12 + 4;
		//		m_MiddleImage.RenderSprite(NULL,NULL,0,&m_tmpPos,dwImageColor);
		//	}
		//	else	
		//	{
		//		m_tmpPos.x = m_absPos.x + 2;
		//		m_tmpPos.y = m_absPos.y + m_textRelRect.top + (CFONT_OBJ->GetTextHeight(0)+2) * mSelectedRowIdx ;
		//		m_OverImage.RenderSprite(&m_OverImageScale,NULL,0,&m_tmpPos,dwImageColor);
		//	}
		//}

		//// List Text Display //////////////////////////////////////////////////////////////
		//PTRLISTPOS pos = m_ListItem.FindIndex( m_lTopListItemIdx );
		//ITEM* item = NULL;
		//LONG i = 0;

		//// ���� �߰��� ���̶���Ʈ ���� �־��� ���(Item�� Line�� üũ)
		//// RaMa 04.8.3
		//int bline = -1;
		//int blinelen = 0;
		//int line = -1;
		//int linelen = 0;
		//DWORD dwFontColor = 0xffffffff;

		//WORD wFontIdx = 0 ;

		//while(pos)
		//{
		//	item = (ITEM*)m_ListItem.GetNext(pos);
		//	if(line < m_lLineNum-1)
		//	{
		//		if(CFONT_OBJ->IsCreateFont(m_wFontIdx))
		//		{
		//			wFontIdx = m_wFontIdx ;

		//			// ������ ���� �����̸� ������ ���̸� ���Ѵ�.
		//			if(item->line != -1 && item->line == bline)
		//			{					
		//				linelen += blinelen;
		//				blinelen = strlen(item->string)*CFONT_OBJ->GetTextWidth(m_wFontIdx);
		//			}
		//			// ���ο� �����̸�
		//			else if(item->line != -1 && item->line != bline)
		//			{
		//				linelen = 0;
		//				++line;
		//				bline = item->line;
		//				blinelen = strlen(item->string)*CFONT_OBJ->GetTextWidth(m_wFontIdx);
		//			}
		//			// �Ϲ����� ���
		//			else
		//			{					
		//				linelen = 0;
		//				++line;
		//			}

		//			RECT rect={(LONG)m_absPos.x+m_textRelRect.left+linelen, (LONG)m_absPos.y+m_textRelRect.top + (m_lLineHeight*line), 1,1};

		//			if( (m_ClickSelected-m_lTopListItemIdx) == line && (m_bShowSelect == TRUE))
		//			{
		//				CFONT_OBJ->RenderFont(wFontIdx,item->string,lstrlen(item->string),&rect,RGBA_MERGE(0xffffff00, m_alpha * m_dwOptionAlpha / 100));
		//			}
		//			else
		//			{
		//				CFONT_OBJ->RenderFont(wFontIdx,item->string,lstrlen(item->string),&rect,RGBA_MERGE(item->rgb, m_alpha * m_dwOptionAlpha / 100));
		//			}

		//			i++;
		//		}
		//	}
		//	else
		//		break;
		//}
}

void cListDialog::RenderServerList() 
{
	static VECTOR2 m_tmpPos;
	DWORD dwColor = RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100) ;			// ���� ������ �����ϰ� ������ �����Ѵ�.

	m_TopImage.RenderSprite(NULL,NULL,0,&m_absPos,dwColor);

	m_tmpPos.x = m_absPos.x;
	m_tmpPos.y = m_absPos.y+m_topHeight;

	m_DownImage.RenderSprite(NULL,NULL,0,&m_tmpPos,dwColor);

	// Over Image Display 
	if( mSelectedRowIdx != -1 )
	{
		m_tmpPos.x = m_absPos.x - 2;
		m_tmpPos.y = m_absPos.y + m_textRelRect.top + (((float)mSelectedRowIdx)*18) - m_fGap - 12 + 4;
		m_MiddleImage.RenderSprite(NULL,NULL,0,&m_tmpPos,dwColor);
	}
	if( m_ClickSelected != -1 )
	{
		m_tmpPos.x = m_absPos.x - 2;
		m_tmpPos.y = m_absPos.y + m_textRelRect.top + (((float)m_ClickSelected)*18) - m_fGap - 12 + 4;
		m_MiddleImage.RenderSprite(NULL,NULL,0,&m_tmpPos,dwColor);
	}
	//

	// List Text Display //////////////////////////////////////////////////////////////
	PTRLISTPOS pos = m_ListItem.FindIndex( m_lTopListItemIdx );
	ITEM* item = NULL;
	LONG i = 0;

	// ���� �߰��� ���̶���Ʈ ���� �־��� ���(Item�� Line�� üũ)
	// RaMa 04.8.3
	int bline = -1;
	int blinelen = 0;
	int line = -1;
	int linelen = 0;
	DWORD dwFontColor = 0xffffffff;

	while(pos)
	{
		item = (ITEM*)m_ListItem.GetNext(pos);
		if(line < m_lLineNum-1)
		{
			if(CFONT_OBJ->IsCreateFont(m_wFontIdx))
			{
				if(item->line != -1 && item->line == bline)
				{					
					linelen += blinelen;
					blinelen = strlen(item->string)*CFONT_OBJ->GetTextWidth(m_wFontIdx);
				}
				else if(item->line != -1 && item->line != bline)
				{
					linelen = 0;
					++line;
					bline = item->line;
					blinelen = strlen(item->string)*CFONT_OBJ->GetTextWidth(m_wFontIdx);
				}
				else
				{					
					linelen = 0;
					++line;
				}

				LONG TxtPosY = (LONG)m_absPos.y + linelen;
				LONG TxtPosX = 0 ;

				RECT rect={(LONG)m_absPos.x+m_textRelRect.left+linelen, (LONG)m_absPos.y+m_textRelRect.top + (m_lLineHeight*line), 1,1};

				if( m_nAlign & TXT_LEFT )
				{
					TxtPosX = (LONG)m_absPos.x + linelen;
				}
				else if( m_nAlign & TXT_RIGHT )
				{
					TxtPosX = (LONG)m_absPos.x + m_width 
						- CFONT_OBJ->GetTextExtentEx( m_wFontIdx, item->string, lstrlen(item->string) ) - linelen;

				}
				else	//TXT_CENTER
				{
					TxtPosX = (LONG)m_absPos.x + ( m_width 
						- CFONT_OBJ->GetTextExtentEx( m_wFontIdx, item->string, lstrlen(item->string) ) ) / 2;
				}

				SetRect( &rect, TxtPosX, TxtPosY + 18 * i, 1, 1 );

				if( (m_ClickSelected-m_lTopListItemIdx) == line && (m_bShowSelect == TRUE))
				{
					CFONT_OBJ->RenderFont( m_wFontIdx, item->string, lstrlen(item->string), &rect, 
						RGBA_MERGE(m_dwSelectColumnColor, m_alpha * m_dwOptionAlpha / 100));
				}
				else
				{
					CFONT_OBJ->RenderFont( m_wFontIdx, item->string, lstrlen(item->string), &rect,
						RGBA_MERGE(m_dwColumnColor, m_alpha * m_dwOptionAlpha / 100));
				}
			}
			i++;
		}
		else
			break;
	}
}

void cListDialog::RenderShadowList() 
{
	static VECTOR2 m_tmpPos;

	DWORD dwImageColor = RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100) ;

	PTRLISTPOS pos = m_ListItem.FindIndex( m_lTopListItemIdx );
	ITEM* item = NULL;
	LONG i = 0;

	// ���� �߰��� ���̶���Ʈ ���� �־��� ���(Item�� Line�� üũ)
	// RaMa 04.8.3
	int bline = -1;
	int blinelen = 0;
	int line = -1;
	int linelen = 0;
	DWORD dwFontColor = 0xffffffff;

	WORD wFontIdx = m_wShadowFont ;

	while(pos)
	{
		item = (ITEM*)m_ListItem.GetNext(pos);
		//if(line < m_lLineNum-1)
		//{
		if(CFONT_OBJ->IsCreateFont(m_wFontIdx))
		{
			if(item->line != -1 && item->line == bline)													// ������ ���� �����̸� ������ ���̸� ���Ѵ�.
			{					
				linelen += blinelen;
				blinelen = strlen(item->string)*CFONT_OBJ->GetTextWidth(m_wFontIdx);
			}
			else if(item->line != -1 && item->line != bline)											// ���ο� �����̸�
			{
				linelen = 0;
				++line;
				bline = item->line;
				blinelen = strlen(item->string)*CFONT_OBJ->GetTextWidth(m_wFontIdx);
			}
			else																						// �Ϲ����� ���
			{					
				linelen = 0;
				++line;
			}


			LONG TxtPosY = (LONG)m_absPos.y + linelen;
			LONG TxtPosX = 0 ;
			RECT rect ;

			if( m_nAlign & TXT_LEFT )
			{
				TxtPosX = (LONG)m_absPos.x + linelen;
			}
			else if( m_nAlign & TXT_RIGHT )
			{
				TxtPosX = (LONG)m_absPos.x + m_width - CFONT_OBJ->GetTextExtentEx( wFontIdx, item->string, lstrlen(item->string) ) - linelen;

			}
			else	//TXT_CENTER
			{
				TxtPosX = (LONG)m_absPos.x + ( m_width - CFONT_OBJ->GetTextExtentEx( wFontIdx, item->string, lstrlen(item->string) ) ) / 2;
			}				

			SetRect( &rect, TxtPosX, TxtPosY + (CFONT_OBJ->GetTextHeight(wFontIdx)+2) * i, 1, 1 );

			if( (m_ClickSelected-m_lTopListItemIdx) == line && (m_bShowSelect == TRUE))
			{
				m_tmpPos.x = (float)rect.left;
				m_tmpPos.y = (float)rect.top-2 ;
				m_MiddleImage.RenderSprite(NULL,NULL,0,&m_tmpPos,dwImageColor);

				m_tmpPos.y = (float)rect.top ;
				CFONT_OBJ->RenderFontShadow( wFontIdx, m_nShadowDistance, item->string, lstrlen(item->string), &rect, RGBA_MERGE( item->rgb, m_alpha * m_dwOptionAlpha / 100 ) );
			}
			else
			{
				m_tmpPos.x = (float)rect.left;
				m_tmpPos.y = (float)rect.top-2 ;
				m_MiddleImage.RenderSprite(NULL,NULL,0,&m_tmpPos,dwImageColor);

				m_tmpPos.y = (float)rect.top ;
				CFONT_OBJ->RenderFontShadow( wFontIdx, m_nShadowDistance, item->string, lstrlen(item->string), &rect, item->rgb );
			}
		}

		i++;
		//}
		//else
		//	break;
	}
}

void cListDialog::RenderQuestNotice() 
{
	/*PTRLISTPOS pos = m_NoticeList.GetHeadPosition() ;

	sGAMENOTIFY_MSG* pGameNotifyMsg ;

	char tempBuf[128] = {0, } ;

	while(pos)
	{
		pGameNotifyMsg = NULL ;
		pGameNotifyMsg = (sGAMENOTIFY_MSG*)m_NoticeList.GetNext(pos) ;

		if( pGameNotifyMsg )
		{
			cPtrList* pList = pGameNotifyMsg->itemList ;


		}
	}*/

		//static VECTOR2 m_tmpPos;

		//PTRLISTPOS pos = m_ListItem.FindIndex( m_lTopListItemIdx );
		//ITEM* item = NULL;
		//LONG i = 0;

		//// ���� �߰��� ���̶���Ʈ ���� �־��� ���(Item�� Line�� üũ)
		//// RaMa 04.8.3
		//int bline = -1;
		//int blinelen = 0;
		//int line = -1;
		//int linelen = 0;
		//DWORD dwFontColor = 0xffffffff;

		//WORD wFontIdx = 0 ;

		//while(pos)
		//{
		//	item = (ITEM*)m_ListItem.GetNext(pos);
		//	if(line < m_lLineNum-1)
		//	{
		//		if(CFONT_OBJ->IsCreateFont(6))
		//		{
		//			wFontIdx = 6 ;

		//			if(item->line != -1 && item->line == bline)
		//			{					
		//				linelen += blinelen;
		//				blinelen = strlen(item->string)*CFONT_OBJ->GetTextWidth(m_wFontIdx);
		//			}
		//			else if(item->line != -1 && item->line != bline)
		//			{
		//				linelen = 0;
		//				++line;
		//				bline = item->line;
		//				blinelen = strlen(item->string)*CFONT_OBJ->GetTextWidth(m_wFontIdx);
		//			}
		//			else
		//			{					
		//				linelen = 0;
		//				++line;
		//			}				

		//			LONG TxtPosY = (LONG)m_absPos.y + linelen;
		//			LONG TxtPosX = 0 ;
		//			RECT rect ;

		//			TxtPosX = (LONG)m_absPos.x + ( m_width 
		//						- CFONT_OBJ->GetTextExtentEx( wFontIdx, item->string, lstrlen(item->string) ) ) / 2;
		//			
		//			SetRect( &rect, TxtPosX, TxtPosY + 18 * i, 1, 1 );

		//			CFONT_OBJ->RenderOutLineFont( wFontIdx, item->string, lstrlen(item->string), &rect, RGBA_MERGE( item->rgb, m_alpha * m_dwOptionAlpha / 100 ) );
		//		}
		//		i++;
		//	}
		//	else
		//		break;
		//}
}


void cListDialog::onSize(BOOL fSize)
{
	if(fSize)
	{
		// long
		if(!Resize(m_middleNum+1)) return;
		//1. ������ �̵�
		SetAbsY((LONG)(GetAbsY()-m_middleHeight));
		SetHeight(cWindow::GetHeight()+m_middleHeight);
		//2. guagebar��ȭ
		m_pGuageBar->SetAbsY((LONG)(m_pGuageBar->GetAbsY()-m_middleHeight));
		m_pGuageBar->SetHeight(m_pGuageBar->GetHeight()+m_middleHeight);
		m_pGuageBar->SetInterval(m_pGuageBar->GetInterval()+m_middleHeight);
		//3. upward ��ġ ��ȭ
		m_pUpwardBtn->SetAbsY((LONG)(m_pUpwardBtn->GetAbsY()-m_middleHeight));

		OnUpwardItem();	//KES 040309
	}
	else
	{
		// short
		if(!Resize(m_middleNum-1)) return;
		//1. ������ �̵�
		SetAbsY((LONG)((GetAbsY()+m_middleHeight)));
		SetHeight(cWindow::GetHeight()-m_middleHeight);
		//2. guagebar��ȭ
		m_pGuageBar->SetAbsY((LONG)(m_pGuageBar->GetAbsY()+m_middleHeight));
		m_pGuageBar->SetHeight(m_pGuageBar->GetHeight()-m_middleHeight);
		m_pGuageBar->SetInterval(m_pGuageBar->GetInterval()-m_middleHeight);
		//3. upward ��ġ ��ȭ
		m_pUpwardBtn->SetAbsY((LONG)(m_pUpwardBtn->GetAbsY()+m_middleHeight));

		OnDownwardItem();	//KES 040309
	}
}

void cListDialog::SetExtendReduction(int nSetMode)										// ����Ʈ ���̾�α׸� Ȯ��/��� �ϴ� �Լ�.(����Ʈ �˸��� ���̾�α׿��� ���)
{
	switch(nSetMode)																	// ���� ��带 Ȯ���Ѵ�.
	{
	case LD_EXTEND :																	// Ȯ�� �����,
		{
			if( !Resize( m_middleNum + 1 ) )											// ����Ʈ ���̾�α� �������� �۾��� �����ϸ�,
			{
				return ;																// ���� ó���� �Ѵ�.
			}

			/*m_textRelRect.bottom += m_middleHeight ;
			m_height += m_middleHeight ;
			++m_middleNum ;*/
		}
		break ;

	case LD_REDUCTION :																	// ��� �����,
		{
			if( !Resize( m_middleNum - 1 ) )											// ����Ʈ ���̾�α� �������� �۾��� �����ϸ�,
			{
				return ;																// ���� ó���� �Ѵ�.
			}

			/*m_textRelRect.bottom -= m_middleHeight ;
			m_height -= m_middleHeight ;
			--m_middleNum ;*/
		}
		break ;
	}

	if( m_pGuageBar )																	// ������ �ٰ� �ִٸ�,
	{
		m_pGuageBar->SetAbsY((LONG)(m_pGuageBar->GetAbsY()+m_middleHeight));			// ������ ���� ��ġ�� �����Ѵ�.
		m_pGuageBar->SetHeight(m_pGuageBar->GetHeight()-m_middleHeight);				// ������ ���� ���̸� �����Ѵ�.
		m_pGuageBar->SetInterval(m_pGuageBar->GetInterval()-m_middleHeight);			// ������ ���� ���͹��� �����Ѵ�.
	}

	if( m_pDownwardBtn )																// �ٿ� ��ư�� �ִٸ�,
	{
		m_pDownwardBtn->SetAbsY((LONG)(m_pDownwardBtn->GetAbsY()+m_middleHeight)) ;		// �ٿ� ��ư�� ��ġ�� �ٽ� ��´�.
	}

	switch(nSetMode)																	// ���� ��带 Ȯ���Ѵ�.
	{
	case LD_EXTEND :		OnUpwardItem() ;	break ;									// Ȯ�� �����, OnUpwardItem() �Լ��� ȣ���Ѵ�.

	case LD_REDUCTION :		OnDownwardItem() ;	break ;									// ��� �����, OnDownwardItem() �Լ��� ȣ���Ѵ�.
	}
}

void cListDialog::OnUpwardItem() 
{ 
	LONG cnt = GetItemCount();
	if(cnt < m_lLineNum) return;

	if(m_lTopListItemIdx < 1) return;
	m_lTopListItemIdx--;

	if( m_pGuageBar )
		m_pGuageBar->SetCurRate( (float)m_lTopListItemIdx/(float)(cnt-m_lLineNum) );

}

void cListDialog::OnDownwardItem() 
{ 
	LONG cnt = GetItemCount();
	if(cnt < m_lLineNum) return;

	if((cnt-m_lTopListItemIdx-1) < m_lLineNum) return;
	m_lTopListItemIdx++;

	if( m_pGuageBar )
		m_pGuageBar->SetCurRate( (float)m_lTopListItemIdx/(float)(cnt-m_lLineNum) );
}

void cListDialog::AddItem( const char * str, DWORD color, int line)
{
	ITEM* pItem = new ITEM;
	strcpy( pItem->string, str);
	pItem->rgb = color;
	pItem->line = line;
	pItem->nFontIdx = 0 ;
	//	pItem->line = -1;
	cListItem::AddItem(pItem);	

	//�ǾƷ��϶���
	/*	if( m_pGuageBar )
	if( m_lTopListItemIdx == GetItemCount() - m_lLineNum -1 )
	{
	m_pGuageBar->SetCurValue(m_pGuageBar->GetMaxValue());
	m_lTopListItemIdx = GetItemCount() - m_lLineNum;
	}
	*/
	if( m_pGuageBar )
		if( m_lTopListItemIdx == GetItemCount() - m_lLineNum -1 )
		{
			m_pGuageBar->SetCurValue(m_pGuageBar->GetMaxValue());
			m_lTopListItemIdx = GetItemCount() - m_lLineNum;
		}	
}


// 080225 LUJ, ������ �Բ� ����Ʈ �������� �߰��Ѵ�
void cListDialog::AddItem( const char* text, DWORD color, const ToolTipTextList& tipTextList )
{
	AddItem( text, color );

	// 080225 LUJ, ������ �ִٸ� �߰��Ѵ�
	if( ! tipTextList.empty() )
	{
		ToolTip toolTip;		
		toolTip.mText = text;

		// 080225 LUJ, ������ ����
		toolTip.mIcon = new cIcon;
		{
			cImage image;			
			SCRIPTMGR->GetImage( 0, &image, PFT_HARDPATH );
			// 080924 LUJ, �޽��� �ڽ��� ���̵� ���� 0���� ���� �ִ�. ���� ������ ���� �� �ݵ�� ���̵� ���� ������ �Ѵ�. 
			//			���� ������ �������� �߰��� ���� WINDOWID�� �����ؼ� �������ֵ��� ����.
			toolTip.mIcon->SetID( GD_MEMBER_JOB_TOOLTIP );
			toolTip.mIcon->SetToolTip( "", RGB_HALF( 255, 255, 255), &image );

			for(
				ToolTipTextList::const_iterator it = tipTextList.begin();
				tipTextList.end() != it;
			++it )
			{
				toolTip.mIcon->AddToolTipLine( it->c_str() );
			}
		}

		WINDOWMGR->AddWindow( toolTip.mIcon );

		mToolTipList.push_back( toolTip );
	}
}


void cListDialog::AddItemWithFont(char* str, DWORD color, int fontIdx, int line)
{
	ITEM* pItem = new ITEM;
	strcpy( pItem->string, str);
	pItem->rgb = color;
	pItem->line = line;
	pItem->nFontIdx = fontIdx ;
	cListItem::AddItem(pItem);	

	if( m_pGuageBar )
		if( m_lTopListItemIdx == GetItemCount() - m_lLineNum -1 )
		{
			m_pGuageBar->SetCurValue(m_pGuageBar->GetMaxValue());
			m_lTopListItemIdx = GetItemCount() - m_lLineNum;
		}	
}

void cListDialog::AddNoticeItem(sGAMENOTIFY_MSG* pGameNotifyMsg)
{
	sGAMENOTIFY_MSG* pNewGameNotifyMsg = new sGAMENOTIFY_MSG ;

	memcpy(pNewGameNotifyMsg, pGameNotifyMsg, sizeof(sGAMENOTIFY_MSG)) ;

	m_NoticeList.AddTail(pNewGameNotifyMsg) ;
}


void cListDialog::SetTopListItemIdx(int Idx)
{
	m_lTopListItemIdx=Idx;
	LONG cnt = GetItemCount();

	if( m_pGuageBar )
	{
		float rate = (float)m_lTopListItemIdx/(float)(cnt-m_lLineNum);
		if(rate > 1.f)		rate = 1.f;
		else if(rate< 0.f)	rate = 0.f;
		m_pGuageBar->SetCurRate( rate );		
	}	
}

//void cListDialog::RemoveItem( char * str )
//{
//	
//}

int cListDialog::PtIdxInRow(LONG x, LONG y)
{
	LONG cnt = GetItemCount();

	for(int i = 0 ; i < cnt ; i++)
	{
		if( m_bStyle != LDS_SERVERLIST )
		{
			if( m_absPos.x < x 
				&& m_absPos.y < y
				&& x < m_absPos.x+m_width 
				&& y < m_absPos.y+m_textRelRect.top+(i+1)*m_lLineHeight )
				return i;
		}
		else
		{
			if( m_absPos.x < x 
				&& m_absPos.y < y
				&& x < m_absPos.x+m_width 
				&& y < m_absPos.y+m_textRelRect.top+i*18 + CFONT_OBJ->GetTextHeight(m_wFontIdx) )
				return i;
		}
	}

	return -1;
}

int cListDialog::GetCurSelectedRowIdx()
{
	if( mSelectedRowIdx == -1 ) 
		return -1;

	return mSelectedRowIdx+m_lTopListItemIdx;
}

void cListDialog::SetCurSelectedRowIdx(int Idx)
{
	if(Idx < 0)
	{
		SetTopListItemIdx(0);

		mSelectedRowIdx = -1;
		m_ClickSelected = -1;
	}
	else
	{
		if(Idx <= GetItemCount() - m_lLineNum)
		{
			SetTopListItemIdx(Idx);

			mSelectedRowIdx = 0;
			m_ClickSelected = Idx;
		}
		else
		{
			SetTopListItemIdx(GetItemCount() - m_lLineNum);

			mSelectedRowIdx = Idx - GetItemCount();
			m_ClickSelected = Idx;
		}
	}
}

void cListDialog::ResetGuageBarPos()
{ 
	if( m_pGuageBar )
	{
		m_pGuageBar->SetCurValue(0); 

		int itemCnt = GetItemCount() - m_lLineNum;

		if(itemCnt > 0)
			m_lTopListItemIdx = (LONG)(((float)itemCnt)*m_pGuageBar->GetCurRate());
		else
			m_lTopListItemIdx = 0;
	}

	mSelectedRowIdx = -1;
	m_ClickSelected = -1;
}

void cListDialog::SetLineHeight( LONG height )
{ 
	m_lLineHeight = height; 

	if( height > 10 )
		m_fGap = ((float)height - 10.0f)/2.0f; // �ӽ÷� (���߿� ������ ���̸� �˾ƿͼ� ����Ѵ�.)
	else
		m_fGap = 0.0f;
}

void cListDialog::SetShowSelect(BOOL val)
{
	m_bShowSelect = val;
}


//=================================================================================================
// NAME			: SetDisableScroll()
// PURPOSE		: Setting to disable scroll part.
// ATTENTION	:
//=================================================================================================
void cListDialog::SetDisableScroll()
{
	m_pUpwardBtn->SetActive( FALSE ) ;
	m_pDownwardBtn->SetActive( FALSE ) ;
	m_pGuageBar->SetActive( FALSE ) ;
}


void cListDialog::SetPosition( float ratio )
{
	ASSERT( 0 <= ratio && ratio <= 1.0f);

	const DWORD row = DWORD( ratio * m_ListItem.GetCount() );

	mSelectedRowIdx	= row;
	m_lTopListItemIdx	= row;

	if( m_pGuageBar )
	{
		m_pGuageBar->SetCurRate( ratio );
	}
}


void cListDialog::SetTextRect( const RECT& r )
{
	mTextRect = r;
}


void cListDialog::AddItemAutoLine( const char* text, DWORD color )
{
	const DWORD fontSize	= 6;
	const DWORD width		= ( abs( mTextRect.right - mTextRect.left ) ) / fontSize;

	BYTE line[ MAX_TEXT_SIZE ];
	int length = _mbslen( ( BYTE* )text );

	for(	const BYTE* ch = ( BYTE* )text;
		0 < length; )
	{
		_mbsnbcpy( line, ch, width );

		line[ width ] = 0;

		AddItem( ( char* )line, color );

		const DWORD lineLength = _mbslen( line );

		for( DWORD i = 0; i < lineLength; ++i )
		{
			ch = _mbsinc( ch );
		}

		length -= lineLength;
	}	
}


void cListDialog::SetActive( BOOL isActive )
{
	mSelectedRowIdx		= -1;
	//mPressedRowIndex	= -1;

	cDialog::SetActive( isActive );
}


void cListDialog::RemoveAll()
{
	cListItem::RemoveAll();

	m_ClickSelected	= -1;
	mSelectedRowIdx	= -1;

	mToolTipList.clear();

	// 080225 LUJ, �������� �����ϰ� �ʱ�ȭ�Ѵ�
	{
		for(
			ToolTipList::iterator it = mToolTipList.begin();
			mToolTipList.end() != it;
		++it )
		{
			const ToolTip& toolTip = *it;

			WINDOWMGR->DeleteWindow( toolTip.mIcon );
		}

		mToolTipIcon = 0;
	}
}


// 080225 LUJ, ����Ʈ�� �������� �����Ѵ�
void cListDialog::RemoveItem( DWORD index )
{
	const ITEM* item = GetItem( index );

	for(
		ToolTipList::iterator it = mToolTipList.begin();
		mToolTipList.end() != it;
	++it )
	{
		const ToolTip& toolTip = *it;

		if( toolTip.mText == item->string )
		{
			if( mToolTipIcon == toolTip.mIcon )
			{
				mToolTipIcon = 0;
			}

			WINDOWMGR->DeleteWindow( toolTip.mIcon );

			mToolTipList.erase( it );
			break;
		}
	}

	cListItem::RemoveItem( index );
}


// 080225 LUJ, ������ �����ϸ鼭 ������ �����Ѵ�
void cListDialog::RemoveItem( const char* text )
{
	for(
		ToolTipList::iterator it = mToolTipList.begin();
		mToolTipList.end() != it;
	++it )
	{
		const ToolTip& toolTip = *it;

		if( toolTip.mText == text )
		{
			if( mToolTipIcon == toolTip.mIcon )
			{
				mToolTipIcon = 0;
			}

			WINDOWMGR->DeleteWindow( toolTip.mIcon );

			mToolTipList.erase( it );
			break;
		}
	}

	cListItem::RemoveItem( ( char* )text );
}
