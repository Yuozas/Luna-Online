// cListItem.h: interface for the cListItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLISTITEM_H__EFECF4A3_00CC_4F48_A2CA_775C28D96531__INCLUDED_)
#define AFX_CLISTITEM_H__EFECF4A3_00CC_4F48_A2CA_775C28D96531__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "cWindowDef.h"

class cListItem
{
public:
	cListItem():m_wMaxLine(0)
	{
	}
	virtual ~cListItem()
	{
		RemoveAll();
	}
	// 070409 LYW --- cListItem : Add function to return item.
	ITEM* GetItem( LONG idx )
	{
		PTRLISTPOS p = m_ListItem.FindIndex( idx ) ;

		if( p )
		{
			return (ITEM*)m_ListItem.GetAt( p ) ;
		}
		else
		{
			return NULL ;
		}
	}

	LONG GetItemCount()
	{
		return m_ListItem.GetCount();
	}
	virtual void AddItem(ITEM* pItem)			// pItem�� �޸� ������ �ۿ��� �Ҵ��ؼ� �ְ�, cListItem���� �����Ѵ�.
	{
		if(m_wMaxLine < 1)	return;
		if(m_wMaxLine <= GetItemCount())		// �� á�� ���, ��� �������� ������ ���� Ȯ��.
		{
			ITEM* item = (ITEM*)m_ListItem.GetHead();
			SAFE_DELETE(item);					// ���ʿ��� �����. ���� ���� ������. -_-;
			m_ListItem.RemoveHead();
		}
		m_ListItem.AddTail( pItem );
	}
	virtual void AddItem(ITEM* pItem, LONG idx)		// �ε����� ������ ���, �ε����� �ƽ����θ�ŭ ��� �ǰ� �ִ°��� ������ �ϹǷ� �����ϸ� ���� ����.
	{
		_asm int 3;
		if(m_wMaxLine < 1) return;
		// ���� ����� ������ ����. �׸��� index �����ؼ� ���� ���� �� ���� �ش� �ε����� ��ü�ϴ°� �ƴ϶� ��带 ���������Ƿ� ������. - ������.
		if(m_wMaxLine <= GetItemCount())
		{
			_asm int 3;
			ITEM* item = (ITEM*)m_ListItem.GetHead();
			SAFE_DELETE(item);
			m_ListItem.RemoveHead();
		}
	
		PTRLISTPOS p = m_ListItem.FindIndex(idx);
		if( p == NULL ) return;								// �ε����� �ƽ����θ�ŭ ������ ���� ��� �̶��ο��� ������ �ȴ�. �׳� ���������� ��¼���. -_-;

		m_ListItem.InsertAfter(p, pItem);
	}
	
	virtual void RemoveAll()
	{
		PTRLISTSEARCHSTART(m_ListItem,ITEM*,item)				// ������ ����Ʈ�� �����۵��� �����ϰ�, (���� ������. -_-;)
			delete item;
		PTRLISTSEARCHEND

		m_ListItem.RemoveAll();									// ������ ����Ʈ ��ü�� ����.
	}
	virtual void RemoveItem(LONG idx)
	{
		PTRLISTPOS p = m_ListItem.FindIndex( idx );

		if( p )
		{
			ITEM * item = (ITEM*)m_ListItem.GetAt( p );
			SAFE_DELETE(item);
			m_ListItem.RemoveAt(p);
		}
	}
	
	virtual void RemoveItem(char * str)
	{
		PTRLISTPOS p  = m_ListItem.GetHeadPosition();
		while( p )
		{
//			ITEM* item = (ITEM*)m_ListItem.GetNext( p );
			ITEM* item = (ITEM*)m_ListItem.GetAt( p );

			if( item )
			{
				if(strcmp(item->string, str) == 0)
				{
					SAFE_DELETE(item);
					m_ListItem.RemoveAt(p);
					break;
				}
			}

			m_ListItem.GetNext( p );
		}		
	}

	void SetMaxLine(WORD maxLine)	{ m_wMaxLine = maxLine; }
	WORD GetMaxLine()				{ return m_wMaxLine; }

	// 061129 LYW --- Add Function that Return cPtrList.
	cPtrList* GetListItem() { return &m_ListItem ; }

protected:

	WORD		m_wMaxLine;
	cPtrList	m_ListItem;
};

/*
class cListItem
{
public:
	cListItem():m_wMaxLine(0)
	{
		m_pItemList = new cLinkedList<ITEM>;
	}
	~cListItem()
	{
		RemoveAll();
		SAFE_DELETE(m_pItemList);
	}
	LONG GetItemCount()
	{
		return m_pItemList->GetCount();
	}
	void AddItem(char * str)
	{
		ITEM * item = new ITEM;
		strcpy(item->string, str);
		item->rgb = RGB_HALF(255,255,255);
		m_pItemList->AddTail(item);
		item->wType = 0;
	}
	
	void AddItem(char * str, LONG idx)
	{
		if(m_wMaxLine < 1) return;
		if(m_wMaxLine < GetItemCount())
			m_pItemList->DeleteHead();
		
		ITEM * item = new ITEM;
		strcpy(item->string, str);
		item->rgb = RGB_HALF(255,255,255);
		item->wType = 0;
		m_pItemList->InsertpAfter(item, idx);
	}
	
	void AddItem(char * str, DWORD color)
	{
		if(m_wMaxLine < 1) return;
		if(m_wMaxLine < GetItemCount())
			m_pItemList->DeleteHead();
		ITEM * item = new ITEM;
		strcpy(item->string, str);
		item->rgb = color;
		item->wType = 0;
		
		m_pItemList->AddTail(item);
	}

	void AddItem( WORD wType, char * str)
	{
		ITEM * item = new ITEM;
		strcpy(item->string, str);
		item->rgb = RGB_HALF(255,255,255);
		m_pItemList->AddTail(item);
		item->wType = wType;
	}

	void RemoveAll()
	{
		m_pItemList->DeleteAll();
	}
	
	void RemoveItem(LONG idx)
	{
		ITEM * item = m_pItemList->DeleteAt(idx);			// return �� �̻���-_-;
		SAFE_DELETE(item);
	}
	
	void RemoveItem(char * str)
	{
		POS pos = m_pItemList->GetFirstPos();
		ITEM * item = NULL;
		while (pos)
		{
			item = m_pItemList->GetNextPos(pos);			
			if(strcmp(item->string, str) == 0)
			{
				m_pItemList->GetPrevPos(pos);
				m_pItemList->DeletePos(pos);			// return �� �̻���-_-;
				SAFE_DELETE(item);
			}
		}
	}
	void SetMaxLine(WORD maxLine) { m_wMaxLine = maxLine; }
protected:

	WORD m_wMaxLine;								// ��ӹ��� Ŭ�������� �ʱ�ȭ�Ѵ�.
	cLinkedList<ITEM> * m_pItemList;
};
*/
#endif // !defined(AFX_CLISTITEM_H__EFECF4A3_00CC_4F48_A2CA_775C28D96531__INCLUDED_)
