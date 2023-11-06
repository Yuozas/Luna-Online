


#include "stdafx.h"																			// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.

#include "cListDialogEx.h"																	// ����Ʈ ���̾�α� Ȯ�� Ŭ���� ����� �ҷ��´�.

#include "./Interface/cButton.h"															// ��ư Ŭ���� ����� �ҷ��´�.
#include "./Interface/cGuageBar.h"															// ������ �� Ŭ���� ����� �ҷ��´�.
#include "./Interface/cFont.h"																// ��Ʈ Ŭ���� ����� �ҷ��´�.


cListDialogEx::cListDialogEx()																// ������ �Լ�.
{
}

cListDialogEx::~cListDialogEx()																// �Ҹ��� �Լ�.
{
}


void cListDialogEx::ListMouseCheck( LONG x, LONG y, DWORD we )								// ����Ʈ ���콺 üũ �Լ�.
{
	int selIdx = PtIdxInRow(x, y);															// ���콺�� üũ�ؼ� ���� �� ������ �޴´�.

	if( selIdx < m_lLineNum )																// ���� �� ������ ���� �� ���� ������,
	{
		mSelectedRowIdx = selIdx;															// ���� �� ���� �ε����� �����Ѵ�.

		if( we & WE_LBTNCLICK )																// ���콺 ���� Ŭ���� �ߴٸ�,
		{
			PTRLISTPOS pos = m_ListItem.FindIndex(m_lTopListItemIdx+mSelectedRowIdx);		// ������ ����Ʈ���� ���� �� ��ġ ������ �޴´�.

			if( pos == NULL ) return;														// ��ġ ������ ��ȿ���� ������, ���� ó���� �Ѵ�.

			LINKITEM* item = NULL;															// ������ ������ ���� ������ ������ �����ϰ� NULL ó���� �Ѵ�.

			item = (LINKITEM*)m_ListItem.GetAt(pos);										// ��ġ ������ ���� ������ ������ ����Ʈ�� ���� �޴´�.

			if( item && item->dwType  > emLink_Null )										// ������ ������ ��ȿ�ϰ�, ��ũ Ÿ���� NULL �� �ƴϸ�,
			{
				cbWindowFunc(m_ID, m_pParent, WE_ROWCLICK);									// Ŭ�� �̺�Ʈ�� ������ �Լ��� ���� ��Ų��.

				we |= WE_ROWCLICK;															// ������ �̺�Ʈ�� ���� Ŭ������ �����Ѵ�.
			}
		}
	}
	else																					// ���� �� ������ ���� ���� ���� �ߴٸ�,
	{
		mSelectedRowIdx = -1;																// ���� �� ������ -1�� �����Ѵ�.
	}
}

void cListDialogEx::Render()																// ���� �Լ�.
{
	if(!m_bActive) return;																	// Ȱ��ȭ ���°� �ƴ϶��, ���� ó�� �Ѵ�.

	cDialog::RenderWindow();																// �����츦 �����Ѵ�.

	static VECTOR2 m_tmpPos;																// �ӽ� ��ġ ���͸� �����Ѵ�.

	DWORD dwMergeColor = RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100) ;		// ���� ������ �����Ѵ�.


	m_TopImage.RenderSprite(NULL,NULL,0,&m_absPos,dwMergeColor);							// ž �̹����� �����Ѵ�.

	m_tmpPos.x = m_absPos.x;																// �ӽ� ��ġ ���͸� �����Ѵ�.
	m_tmpPos.y = m_absPos.y+m_topHeight;

	for(int j = 0 ; j < m_middleNum ; j++)													// �̵� ���� ��ŭ for���� ������.
	{
		m_MiddleImage.RenderSprite(NULL,NULL,0,&m_tmpPos,dwMergeColor);						// �̵� �̹����� �����Ѵ�.
		m_tmpPos.y += m_middleHeight;														// �ӽ� ��ġ ���� ���� �����Ѵ�.
	}

	m_DownImage.RenderSprite(NULL,NULL,0,&m_tmpPos,dwMergeColor);							// �ٿ� �̹����� �����Ѵ�.


	LINKITEM* item = NULL;																	// ������ ������ ���� ������ ������ ������ NULL�� �����Ѵ�.

	LONG i = 0;																				// ī��Ʈ �ε����� �����ϰ� 0���� �����Ѵ�.

	char ReadBuff[256] = {0,};																// ���� ���۸� �����Ѵ�.
	char WriteBuff[256] = {0,};																// ����Ʈ ���۸� �����Ѵ�.

	char temp[256] = {0,};																	// �ӽ� ���۸� �����Ѵ�.

	int ReadPos = 0;																		// ���� ������ ������ �����ϰ� 0���� �����Ѵ�.
	int WritePos = 0;																		// ����Ʈ ������ ������ �����ϰ� 0���� �����Ѵ�.

	RECT rect ;																				// ��¿� ��Ʈ�� �����Ѵ�.

	rect.left   = (LONG)m_absPos.x+m_textRelRect.left ;										// ����Ʈ�� �ؽ�Ʈ ��Ʈ�� ����Ʈ�� �����Ѵ�.
	rect.top    = (LONG)m_absPos.y+m_textRelRect.top + (m_lLineHeight*i) ;					// ž�� �ؽ�Ʈ ��Ʈ�� ž + ���γ���*ī��Ʈ �� �����Ѵ�.
	rect.right  = 1 ;																		// ����Ʈ�� 1�� �����Ѵ�.
	rect.bottom = 1 ;																		// ������ 1�� �����Ѵ�.

	PTRLISTPOS pos = m_ListItem.FindIndex( m_lTopListItemIdx );								// ž ����Ʈ ������ �ε����� ������ ����Ʈ���� ��ġ ������ �޴´�.

	while(pos)																				// ��ġ ������ ��ȿ�� ���� while���� ������.
	{
		item = (LINKITEM*)m_ListItem.GetNext(pos);											// ��ġ ������ �ش��ϴ� ������ ������ �޴´�.

		if(i < m_lLineNum)																	// ī��Ʈ�� ���� �� ���� �۴ٸ�,
		{
			if(CFONT_OBJ->IsCreateFont(m_wFontIdx))											// �־��� �ε����� ��Ʈ�� �����Ǿ����� Ȯ���ϰ�,
			{
				rect.top    = (LONG)m_absPos.y+m_textRelRect.top + (m_lLineHeight*i) ;		// ��Ʈ�� ž�� �����Ѵ�.

				if( item->dwType  > emLink_Null )											// ������ Ÿ���� ��ũ NULL ��Ÿ ũ�ٸ�,
				{
					if( mSelectedRowIdx == i )												// ���� �� ���� �ε����� ī��Ʈ�� ���ٸ�,
					{
						m_tmpPos.x = m_absPos.x + 2;										// �ӽ� ��ġ X��ǥ�� �����Ѵ�.
						m_tmpPos.y = m_absPos.y + m_textRelRect.top + 
							(((float)mSelectedRowIdx)*m_lLineHeight) - m_fGap;				// �ӽ� ��ġ Y��ǥ�� �����Ѵ�.

						m_OverImage.RenderSprite(&m_OverImageScale,NULL,0,&m_tmpPos,dwMergeColor);	// ���� �̹����� ���� �Ѵ�.
					}

					if(CFONT_OBJ->IsCreateFont(4))											// 4�� ��Ʈ�� ���� �Ǿ��ٸ�,
					{
						if( mSelectedRowIdx == i )											// ���콺 ������ ���,
						{
							// 4�� ��Ʈ, ������ ���� �������� ���ڸ� ����Ѵ�.
							CFONT_OBJ->RenderFont(4,item->string,lstrlen(item->string),&rect,RGBA_MERGE(item->dwOverRGB, m_alpha * m_dwOptionAlpha / 100));
						}
						else																// �Ϲ����� ���
						{
							// 4�� ��Ʈ, ������ �������� ���ڸ� ����Ѵ�.
							CFONT_OBJ->RenderFont(4,item->string,lstrlen(item->string),&rect,RGBA_MERGE(item->rgb, m_alpha * m_dwOptionAlpha / 100));
						}
					}
					else																	// 4�� ��Ʈ�� ���� ���� �ʾҴٸ�,
					{
						if( mSelectedRowIdx == i )											// ���콺 ������ ���,
						{
							// ���� ��Ʈ, ������ ���� �������� ���� ����� �Ѵ�.
							CFONT_OBJ->RenderFont(m_wFontIdx,item->string,lstrlen(item->string),&rect,RGBA_MERGE(item->dwOverRGB, m_alpha * m_dwOptionAlpha / 100));
						}
						else																// �Ϲ����� ���
						{	// ���� ��Ʈ, ������ �������� ���ڸ� ����Ѵ�.
							CFONT_OBJ->RenderFont(m_wFontIdx,item->string,lstrlen(item->string),&rect,RGBA_MERGE(item->rgb, m_alpha * m_dwOptionAlpha / 100));
						}
					}
				}
				else																		// ������ Ÿ���� ��ũ NULL �� ���ų� ������,
				{
					// ���� ��Ʈ, ������ �������� ���ڸ� ����Ѵ�.
					CFONT_OBJ->RenderFont(m_wFontIdx,item->string,lstrlen(item->string),&rect,RGBA_MERGE(item->rgb, m_alpha * m_dwOptionAlpha / 100));

					// ���� ��Ʈ, ��Ʈ���� ���� ������ �޴´�.
					LONG lWid = CFONT_OBJ->GetTextExtentEx( m_wFontIdx, item->string, lstrlen(item->string) );

					// ���� ������ ������ �޴´�.
					LINKITEM* pNext = (LINKITEM*)item->NextItem;						

					// ���� ���ο� ���� �������� �����ϸ�..

					int nTempLeft = rect.left ;
					while(pNext)
					{
						// ��Ʈ�� ����Ʈ�� �ٽ� �����Ѵ�.
						rect.left += lWid;	

						// ���� ��Ʈ, ������ �������� ���� ������ ��Ʈ���� ����Ѵ�.
						CFONT_OBJ->RenderFont(m_wFontIdx,pNext->string,lstrlen(pNext->string),&rect,RGBA_MERGE(pNext->rgb, m_alpha * m_dwOptionAlpha / 100));

						// ���� ������ ��Ʈ���� ���� ���̸� �޴´�.
						lWid = CFONT_OBJ->GetTextExtent( m_wFontIdx, pNext->string, lstrlen(pNext->string) );

						// ���� ������ ������ �޴´�.
						pNext = (LINKITEM*)pNext->NextItem;
					}

					rect.left = nTempLeft ;
				}
			}

			i++;																			// ī��Ʈ ������ ���� ��Ų��.
		}
		else		
			break;																			// ī��Ʈ�� ���μ��� ���ų� ũ��,
	}


	if(IsMaxLineOver())																		// �ִ� ������ ���� ���� ���,
	{	
		if(m_pUpwardBtn)																	// �� ��ư ������ ��ȿ�� ���,
		{
			m_pUpwardBtn->Render();															// �� ��ư�� ���� �Ѵ�.
		}

		if(m_pDownwardBtn)																	// �ٿ� ��ư ������ ��ȿ�� ���,
		{
			m_pDownwardBtn->Render();														// �ٿ� ��ư�� ���� �Ѵ�.
		}

		if(m_pGuageBar)																		// �������� ������ ��ȿ�� ���,
		{
			m_pGuageBar->Render();															// �������ٸ� �����Ѵ�.
		}
	}	

	cDialog::RenderComponent();																// ������ ������Ʈ���� ���� �Ѵ�.
}

