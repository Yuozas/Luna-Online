/*
080414 LUJ, ĳ���� �ܾ� �ٲٴ� â
*/
#pragma once

#include "interface/cDialog.h"
#include "MHCamera.h"

class	CItem;

class CBodyChangeDialog : public  cDialog
{
public:
	CBodyChangeDialog();
	virtual ~CBodyChangeDialog();

	void Linking();
	
	virtual void OnActionEvent( LONG id, void* p, DWORD we );
	
	virtual void SetActive( BOOL isActive, CItem& );
	virtual void SetActive( BOOL isActive );
	virtual void Render();
	
private:	
	class CMenu
	{
	private:		
		// 080414 LUJ, Ű: �޴� ��
		typedef std::map< BYTE, std::string >	ItemList;

		ItemList					mItemList;
		ItemList::const_iterator	mIterator;

		DWORD	mDefaultValue;

	public:
		CMenu() :
		mIterator( mItemList.begin() )
		{};

		void Add( BYTE value, const char* text )
		{
			mItemList.insert( std::make_pair( value, text ) );
		}

		void Increase()
		{
			++mIterator;

			if( mIterator == mItemList.end() )
			{
				mIterator = mItemList.begin();
			}
		}

		void Decrease()
		{
			if( mIterator == mItemList.begin() )
			{
				mIterator = mItemList.end();
			}

			--mIterator;	
		}

		const char* GetText() const
		{
			return mIterator == mItemList.end() ? "?" : mIterator->second.c_str();
		}

		BYTE GetValue() const
		{
			return mIterator == mItemList.end() ? 0 : mIterator->first;
		}

		DWORD GetSize() const
		{
			return mItemList.size();
		}

		void SetDefault( BYTE value )
		{
			mDefaultValue = value;

			ItemList::const_iterator it = mItemList.begin();
			
			for(
				;
				mItemList.end() != it;
				++it )
			{
				if( value == it->first )
				{
					mIterator = it;
					break;
				}
			}
		}
	};

	CMenu mHairMenu;
	CMenu mFaceMenu;

	CHARACTERCHANGE_INFO mCharacterChangeInfo;

	// 080414 LUJ, �ܾ� ���濡 ����� ������
	ITEMBASE mUsedItem;

	// 080414 LUJ, ī�޶� �ڵ����� �۵����� �ʴ� �ð�
	DWORD	mStopTime;

	// 080414 LUJ,	���ڿ�. ĳ���� ���� ������ ���ڿ��� CharMake_List.bin�� ����Ǿ� �ִ�. ���� ���⸦ �켱������ ��ȸ�ϵ�
	//				���� ��� ���ҽ� �Ŵ����� �����ϵ��� �Ѵ�
	struct MenuText
	{
		RaceType	mRace;
		DWORD		mGender;
		DWORD		mValue;
		PART_TYPE	mPart;
		std::string	mText;
	};

	typedef std::list< MenuText >	MenuTextList;
	MenuTextList					mMenuTextList;

	// 080414 LUJ, MenuTextList���� ���ǿ� �ش��ϴ� ���ڿ��� ��ȯ�Ѵ�
	const char* GetText( RaceType, PART_TYPE, DWORD gender, DWORD value ) const;

	// 080414 LUJ, ī�޶� ������ �����ϱ� ���� ���
	MHCAMERADESC mStoredCameraDescription;

	// 080414 LUJ, �ڵ� ȸ�� ���� �ð�
	DWORD	mAutoRotataionDelayTime;
};
