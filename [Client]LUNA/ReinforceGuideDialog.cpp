#include "stdafx.h"
#include "ReinforceGuideDialog.h"
#include "WindowIDEnum.h"
#include "Interface/cPushupButton.h"
#include "interface/cStatic.h"
#include "interface/cIconDialog.h"
#include "ItemManager.h"
#include "ItemShow.h"
#include "cScriptManager.h"
#include "MHFile.h"


CReinforceGuideDialog::CReinforceGuideDialog(void)
{}


CReinforceGuideDialog::~CReinforceGuideDialog(void)
{
	Release();
}


void CReinforceGuideDialog::Release()
{
	for( IconList::iterator it = mIconList.begin();
		mIconList.end() != it;
		++it )
	{
		SAFE_DELETE( *it );
	}
}


void CReinforceGuideDialog::Add( cWindow* window )
{
	switch( window->GetType() )
	{
	case WT_PUSHUPBUTTON:
		{
			AddTabBtn( curIdx1++, ( cPushupButton* )window );
			break;
		}
	case WT_ICONDIALOG:
		{
			AddTabSheet( curIdx2++, window );
			break;
		}
	default:
		{
			cDialog::Add(window);
			break;
		}
	}
}


void CReinforceGuideDialog::Linking()
{	
	Release();

	typedef std::map< DWORD, std::string >				ReinforceDescription;
	ReinforceDescription								mReinforceDescription;

	// ��ȭ �����۸��� ª�� ������ ���ε��س��´�.
	{
		CMHFile file;

		if( ! file.Init( "data/interface/windows/ReinforceGuideDescription.bin", "rb" ) )
		{
			ASSERT( 0 );
			return;
		}

		while( ! file.IsEOF() )
		{
			const DWORD itemIndex	= file.GetDword();
			const char* text		= file.GetString();

			mReinforceDescription[ itemIndex ] = std::string( text );
		}
	}

	typedef CItemManager::ReinforceScriptMap			ReinforceScriptMap;
	typedef std::pair< DWORD, const ReinforceScript* >	Script;
	typedef std::multimap< eEquipSlot, Script >			ScriptAsSlot;
	ScriptAsSlot										mScriptAsSlot;

	// ��ȭ ��ũ��Ʈ�� �о� �������� �з��س��´�.
	{
		const ReinforceScriptMap& scriptContainer = ITEMMGR->GetReinforceScript();

		for(	ReinforceScriptMap::const_iterator it = scriptContainer.begin();
				scriptContainer.end() != it;
				++it )
		{
			const DWORD				itemIndex		= it->first;
			const ReinforceScript&	reinforceScript	= it->second;

			// ���� ��ȭ�� �������� ǥ������ �ʴ´�.
			if( reinforceScript.mForRare )
			{
				continue;
			}
            
			for(	ReinforceScript::Slot::const_iterator inner = reinforceScript.mSlot.begin();
					reinforceScript.mSlot.end() != inner;
					++inner )
			{
				const eEquipSlot slot = *inner;

				// ����2�� �Ͱ���2�� ���õ� �������� ����1�� �Ͱ���1�� ���õǾ� �����Ƿ� �� �� �ϳ��� ó���ؾ��Ѵ�.
				if( eEquipSlot_Ring2	!= slot &&
					eEquipSlot_Earring2	!= slot )
				{
					mScriptAsSlot.insert( std::pair< eEquipSlot, Script >( slot, Script( itemIndex, &reinforceScript ) ) );
				}
			}
		}
	}

	cImage image;
	SCRIPTMGR->GetImage( 0, &image, PFT_HARDPATH );

	// �з��� ���� �������� ǥ���Ѵ�
	for(	ScriptAsSlot::const_iterator it = mScriptAsSlot.begin();
			mScriptAsSlot.end() != it;
			++it )
	{
		const eEquipSlot		slot	= it->first;
		cIconDialog*			dialog	= GetTabDialog( slot );

		if( !	dialog ||
				dialog->GetType() != WT_ICONDIALOG )
		{
			//ASSERT( 0 && "�ش� ������ ǥ���� â�� ���ǵ��� �ʾҰų� ������ â�� �ƴ�" );
			continue;
		}

		CItemShow*	icon		= new CItemShow;
		WORD		iconIndex	= 0;

		for( ; dialog->GetCellNum() > iconIndex; ++iconIndex )
		{
			if( dialog->IsAddable( iconIndex ) )
			{
				dialog->AddIcon( iconIndex, icon );
				break;
			}
		}

		if( dialog->GetCellNum() <= iconIndex )
		{
			ASSERT( 0 && "������ â�� ���ο� �������� �߰��� ������ ����" );
			SAFE_DELETE( icon );
			continue;
		}

		// ���߿� �ϰ� �����ϱ� ���� ����Ʈ�� ��������
		mIconList.push_back( icon );

		const DWORD				itemIndex	= it->second.first;
		const ReinforceScript*	script		= it->second.second;

		ASSERT( itemIndex && script );

		// ������ ����
		{
			icon->SetMovable( FALSE );
			icon->SetVisibleDurability( FALSE );
			icon->SetData( itemIndex );
			icon->Init( itemIndex, 1 );

			ITEMMGR->AddToolTip( icon );
		}
		
		// �ؽ�Ʈ ����
		{
			cStatic*			titleText		= GetTitleStatic( dialog, iconIndex );
			cStatic*			descriptionText	= GetDescriptionStatic( dialog, iconIndex );
			const ITEM_INFO*	info			= ITEMMGR->GetItemInfo( itemIndex );

			ASSERT( titleText && descriptionText && info );
			
			const std::string& text = mReinforceDescription[ itemIndex ];

			descriptionText->SetStaticText( text.c_str() );
			titleText->SetStaticText( info->ItemName );
		}
	}
}


void CReinforceGuideDialog::OnActionEvent(LONG id, void * p, DWORD we)
{
	if( we & WE_BTNCLICK && id == RFGUIDE_OKBTN )
	{
		SetActive( FALSE );
	}
}


void CReinforceGuideDialog::SetActive( BOOL isActive )
{	
	if( isActive )
	{
		cWindow* window = GetTabSheet( 0 );
		ASSERT( window );

		window->SetActive( TRUE );
	}

	cTabDialog::SetActive( isActive );
}


void CReinforceGuideDialog::Render()
{
	cDialog::RenderWindow();
	cDialog::RenderComponent();	
	cTabDialog::RenderTabComponent();
}


cIconDialog* CReinforceGuideDialog::GetTabDialog( eEquipSlot slot )
{
	// �������� ���� �����´�. �̰� �ε��������� �ϵ��ڵ��� �� �ۿ� ����.
	// ���� ���� 0��...

	cWindow* window = 0;

	switch( slot )
	{
	case eEquipSlot_hand:
		{
			window = GetWindowForID( RFGUIDE_SHEET1 );
			break;
		}
	case eEquipSlot_Shield:
		{
			window = GetWindowForID( RFGUIDE_SHEET2 );
			break;
		}
	case eEquipSlot_Armor:
		{
			window = GetWindowForID( RFGUIDE_SHEET3 );
			break;
		}
	case eEquipSlot_Helmet:
		{
			window = GetWindowForID( RFGUIDE_SHEET4 );
			break;
		}
	case eEquipSlot_Glove:
		{
			window = GetWindowForID( RFGUIDE_SHEET5 );
			break;
		}
	case eEquipSlot_Boots:
		{
			window = GetWindowForID( RFGUIDE_SHEET6 );
			break;
		}
	case eEquipSlot_Ring1:
	case eEquipSlot_Ring2:
		{
			window = GetWindowForID( RFGUIDE_SHEET7 );
			break;
		}
	case eEquipSlot_Necklace:
		{
			window = GetWindowForID( RFGUIDE_SHEET8 );
			break;
		}
	case eEquipSlot_Earring1:
	case eEquipSlot_Earring2:
		{
			window = GetWindowForID( RFGUIDE_SHEET9 );
			break;
		}
	case eEquipSlot_Belt:
		{
			window = GetWindowForID( RFGUIDE_SHEET10 );
			break;
		}
	case eEquipSlot_Band:
		{
			// ����� ���� ����... ��ũ��Ʈ�� �߰��Ǿ�� ��
			//window = GetTabSheet( 10 );
			ASSERT( 0 );
			break;
		}
	}

	return ( cIconDialog* )window;
}


cStatic* CReinforceGuideDialog::GetTitleStatic( cDialog* dialog, DWORD iconIndex )
{
	ASSERT( dialog );

	cWindow* window = 0;

	switch( dialog->GetID() )
	{
	case RFGUIDE_SHEET1:
		{
			window = dialog->GetWindowForID( RFGUIDE_SHEET1_TITLE1 + iconIndex );
			break;
		}
	case RFGUIDE_SHEET2:
		{
			window = dialog->GetWindowForID( RFGUIDE_SHEET2_TITLE1 + iconIndex );
			break;
		}
	case RFGUIDE_SHEET3:
		{
			window = dialog->GetWindowForID( RFGUIDE_SHEET3_TITLE1 + iconIndex );
			break;
		}
	case RFGUIDE_SHEET4:
		{
			window = dialog->GetWindowForID( RFGUIDE_SHEET4_TITLE1 + iconIndex );
			break;
		}
	case RFGUIDE_SHEET5:
		{
			window = dialog->GetWindowForID( RFGUIDE_SHEET5_TITLE1 + iconIndex );
			break;
		}
	case RFGUIDE_SHEET6:
		{
			window = dialog->GetWindowForID( RFGUIDE_SHEET6_TITLE1 + iconIndex );
			break;
		}
	case RFGUIDE_SHEET7:
		{
			window = dialog->GetWindowForID( RFGUIDE_SHEET7_TITLE1 + iconIndex );
			break;
		}
	case RFGUIDE_SHEET8:
		{
			window = dialog->GetWindowForID( RFGUIDE_SHEET8_TITLE1 + iconIndex );
			break;
		}
	case RFGUIDE_SHEET9:
		{
			window = dialog->GetWindowForID( RFGUIDE_SHEET9_TITLE1 + iconIndex );
			break;
		}
	case RFGUIDE_SHEET10:
		{
			window = dialog->GetWindowForID( RFGUIDE_SHEET10_TITLE1 + iconIndex );
			break;
		}
	}

	return ( cStatic* )window;
}


cStatic* CReinforceGuideDialog::GetDescriptionStatic( cDialog* dialog, DWORD iconIndex )
{
	ASSERT( dialog );

	cWindow* window = 0;

	switch( dialog->GetID() )
	{
	case RFGUIDE_SHEET1:
		{
			window = dialog->GetWindowForID( RFGUIDE_SHEET1_DESC1 + iconIndex );
			break;
		}
	case RFGUIDE_SHEET2:
		{
			window = dialog->GetWindowForID( RFGUIDE_SHEET2_DESC1 + iconIndex );
			break;
		}
	case RFGUIDE_SHEET3:
		{
			window = dialog->GetWindowForID( RFGUIDE_SHEET3_DESC1 + iconIndex );
			break;
		}
	case RFGUIDE_SHEET4:
		{
			window = dialog->GetWindowForID( RFGUIDE_SHEET4_DESC1 + iconIndex );
			break;
		}
	case RFGUIDE_SHEET5:
		{
			window = dialog->GetWindowForID( RFGUIDE_SHEET5_DESC1 + iconIndex );
			break;
		}
	case RFGUIDE_SHEET6:
		{
			window = dialog->GetWindowForID( RFGUIDE_SHEET6_DESC1 + iconIndex );
			break;
		}
	case RFGUIDE_SHEET7:
		{
			window = dialog->GetWindowForID( RFGUIDE_SHEET7_DESC1 + iconIndex );
			break;
		}
	case RFGUIDE_SHEET8:
		{
			window = dialog->GetWindowForID( RFGUIDE_SHEET8_DESC1 + iconIndex );
			break;
		}
	case RFGUIDE_SHEET9:
		{
			window = dialog->GetWindowForID( RFGUIDE_SHEET9_DESC1 + iconIndex );
			break;
		}
	case RFGUIDE_SHEET10:
		{
			window = dialog->GetWindowForID( RFGUIDE_SHEET10_DESC1 + iconIndex );
			break;
		}
	}

	return ( cStatic* )window;
}