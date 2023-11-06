#include "stdafx.h"
#include "Hero.h"
#include "CharMakeManager.h"
#include "GameResourceManager.h"
#include "CharMake.h"
#include "windowidenum.h"
#include "cComboBoxEx.h"
#include "ObjectManager.h"
#include "MoveManager.h"
#include "HelperManager.h"
#include "AppearanceManager.h"
#include "interface/cGuageBar.h"
#include "HelperSpeechDlg.h"

#include "interface/cStatic.h"

// 061215 LYW --- Include dialog to create character.
#include "CharMakeNewDlg.h"

// 061215 LYW --- Include window manager.
#include "./interface/cWindowManager.h"

#include "ObjectStateManager.h"


GLOBALTON(cCharMakeManager)
cCharMakeManager::cCharMakeManager()
{
	m_bInit = FALSE;
}

cCharMakeManager::~cCharMakeManager()
{
}

void cCharMakeManager::Init()
{
	m_pNewPlayer = NULL;
	// desc_hseos_성별선택01
	// S 성별선택 추가 added by hseos 2007.06.16
	// ..여기 오기전에 DB에서 성별의 정보를 읽어와서 설정했기 때문에
	// ..초기화 하기 전과 후에 처리해줘야 함.
	int nTmpSexType = m_CharMakeInfo.SexType;
	memset(&m_CharMakeInfo, 0, sizeof(CHARACTERMAKEINFO));
	// E 성별선택 추가 added by hseos 2007.06.16
	m_CharMakeInfo.SexType = nTmpSexType;
	memset(&m_dwCurIndex, 0, sizeof(DWORD)*CE_MAX);	// 각 옵션별로 현재 선택된 인덱스만 가지고 있는다.

	// 061215 LYW -- Delete this code for a moment.
	//LoadUserSelectOption();
//	SetUserSelectOption();
	// 061215 LYW --- Delete this code for a moment.
	//SetUserSelections();

	// 061215 LYW --- Load option list.
	//LoadOptionList() ;
	// 061216 LYW --- Setting static's text.
	//SetOptionList(0, 0) ;
	//SetOptionList(1, 0) ;

	// 061215 LYW --- Initialize CharMakeNewDlg
	m_pCharMakeNewDlg = ( CCharMakeNewDlg* )WINDOWMGR->GetWindowForID( CM_MAKEDLG ) ;

	// 061217 LYW --- Load and setting option list.
	LoadOptionList() ;
	SetOptionList( e_MFACE, 0 ) ;
	SetOptionList( e_MHAIR, 0 ) ;
	SetOptionList( e_WMFACE, 0 ) ;
	SetOptionList( e_WMHAIR, 0 ) ;

	SetOptionList( e_EMFACE, 0 ) ;
	SetOptionList( e_EMHAIR, 0 ) ;
	SetOptionList( e_EWMFACE, 0 ) ;
	SetOptionList( e_EWMHAIR, 0 ) ;

	m_fAngle = 70;
	m_fOldX = 0.0f;

	m_bInit = TRUE;
	
	// 061215 --- Delete this code for a moment.
	/*
	m_pHeight = (cGuageBar*)CHARMAKE->GetCharMakeDialog()->GetWindowForID(CMID_Height);
	m_pWidth = (cGuageBar*)CHARMAKE->GetCharMakeDialog()->GetWindowForID(CMID_Width);
	*/
}

void cCharMakeManager::Release()
{
	for(int i=0;i<CE_MAX;++i)
	{
		if( !m_UserSelectOption[i].IsEmpty() )
		{
			PTRLISTSEARCHSTART(m_UserSelectOption[i],CM_OPTION*,Value)
				delete Value;
			PTRLISTSEARCHEND

			m_UserSelectOption[i].RemoveAll();
		}
	}
}

void cCharMakeManager::ListBoxResetAll()
{
	cStatic* pStatic = NULL;

	for(int i=0;i<CE_MAX;++i)
	{
		if( !m_UserSelectOption[i].IsEmpty() && i != CE_SEX  && i != CE_AREA && i != CE_WEAPON )
		{
			PTRLISTPOS pos = m_UserSelectOption[i].GetHeadPosition();
			CM_OPTION* Value = (CM_OPTION*)m_UserSelectOption[i].GetAt(pos);
			pStatic = (cStatic*)Value->pDescStatic;
			pStatic->SetStaticText(Value->strName);
		}
	}	
}



void cCharMakeManager::LoadUserSelectOption()
{
	int idx = 0;
	int num = 0;
///	char ComboId[64];
	char StaticId[64];

	CMHFile file;
	if(!file.Init(PATH_UOPTION, "rb")) return;

	char temp[256];
	file.GetLine(temp, 256);

	while(1)
	{
		if( idx >= CE_MAX || file.IsEOF()) break;

		num = file.GetInt(); 
///		strcpy(ComboId, file.GetString());
		strcpy(StaticId, file.GetString());

		for( int i=0;i<num;++i)
		{
			CM_OPTION*	option = new CM_OPTION;
			strcpy(option->strName, file.GetString());
			option->dwData = file.GetDword();
			option->dwHelperData = file.GetDword();
///			option->pDescCombo = CHARMAKE->GetCharMakeDialog()->GetWindowForID( IDSEARCH(ComboId) );
			option->pDescStatic = (cStatic*)CHARMAKE->GetCharMakeNewDlg()->GetWindowForID( IDSEARCH(StaticId) );
			if(option->pDescStatic != NULL)
			{
				option->pDescStatic->SetAlign(TXT_MIDDLE);
			}

			m_UserSelectOption[idx].AddTail(option);

		}

		++idx;
	}
}


//=================================================================================================
// NAME			: LoadOptionList()
// PURPOSE		: // 061214 LYW ---- Add function to load option list.
// ATTENTION	:
//=================================================================================================
void cCharMakeManager::LoadOptionList()
{
	int idx = 0;
	int num = 0;
	char StaticId[64];

	CMHFile file;
	if(!file.Init(PATH_NOPTION, "rb")) return;

	char temp[256];
	file.GetLine(temp, 256);

	while(1)
	{
		if( idx >= CM_MAX || file.IsEOF()) break;

		num = file.GetInt(); 
		strcpy(StaticId, file.GetString());

		for( int i=0;i<num;++i)
		{
			CM_OPTION*	option = new CM_OPTION;
			strcpy(option->strName, file.GetString());
			option->dwData = file.GetDword();
			option->dwHelperData = file.GetDword();
			//option->pDescStatic = (cStatic*)CHARMAKE->GetCharMakeNewDlg()->GetWindowForID( IDSEARCH(StaticId) );
			CCharMakeNewDlg* pDlg = ( CCharMakeNewDlg* )WINDOWMGR->GetWindowForID( CM_MAKEDLG ) ;
			option->pDescStatic = pDlg->GetStatic(idx) ;
			if(option->pDescStatic != NULL)
			{
				option->pDescStatic->SetAlign(TXT_MIDDLE);
			}

			m_UserSelectOption[idx].AddTail(option);
		}
		++idx;
	}
}

// 061214 LYW ---- End add.
//=================================================================================================
// NAME			: SetOptionList()
// PURPOSE		: Att fucntion to setting selection value.
// ATTENTION	: num ==> 0 = mface, 1 = mhair, 2 = wmface, 3 = wmhair
//=================================================================================================
void cCharMakeManager::SetOptionList( int curStatic, int curIdx )
{
	PTRLISTPOS pos = m_UserSelectOption[curStatic].FindIndex(curIdx) ;

	if(pos)
	{
		CM_OPTION* Value = (CM_OPTION*)m_UserSelectOption[curStatic].GetAt(pos) ;
		cStatic* pStatic = ( cStatic* )WINDOWMGR->GetWindowForIDEx(CM_ST_MFACE + curStatic) ;
		if(pStatic)
		{
			pStatic->SetStaticText(Value->strName) ;	
			pStatic->SetFontIdx(0) ;
			// 070122 LYW --- Setting color of font.
			pStatic->SetFGColor( RGBA_MAKE( 10, 10, 10, 255 ) ) ;
		}
	}
}




void cCharMakeManager::ReplaceCharMakeInfo()
{
	if( m_pNewPlayer ) 
	{
		OBJECTMGR->AddGarbageObject( (CObject*)m_pNewPlayer );
		m_pNewPlayer = NULL;
	}
}

void cCharMakeManager::CreateNewCharacter(VECTOR3* pv3Pos)
{
	if (NULL == pv3Pos)
	{
		return;
	}

	
	VECTOR3	vScale ;

	BASEOBJECT_INFO sBaseInfo;
	CHARACTER_TOTALINFO sTotalInfo;

	memset(&sBaseInfo, 0, sizeof(BASEOBJECT_INFO));
	memset(&sTotalInfo, 0, sizeof(CHARACTER_TOTALINFO));

	sBaseInfo.dwObjectID = 1;
	strcpy( sBaseInfo.ObjectName, "NewCharacter" );
//	sBaseInfo.ObjectState = 1;
	sBaseInfo.ObjectState = eObjectState_Enter;

//	sTotalInfo.Job = m_CharMakeInfo.JobType;
	sTotalInfo.HairType = m_CharMakeInfo.HairType;
	sTotalInfo.FaceType = m_CharMakeInfo.FaceType;
	sTotalInfo.Race = m_CharMakeInfo.RaceType;
	sTotalInfo.Gender = m_CharMakeInfo.SexType;
	/*sTotalInfo.WearedItemIdx[eWearedItem_Dress] = 0;
	sTotalInfo.WearedItemIdx[eWearedItem_Shoes] = 0;
	sTotalInfo.WearedItemIdx[eWearedItem_Weapon] = 0;*/
	sTotalInfo.Height = 1;
	sTotalInfo.Width = 1;
	sTotalInfo.bVisible = TRUE;
	sTotalInfo.WearedItemIdx[ eWearedItem_Weapon ] = GetWeaponIndex( ENUM_CM_CLASS(	m_CharMakeInfo.JobType ) );
	sTotalInfo.WearedItemIdx[ eWearedItem_Shield ] = GetShieldIndex( ENUM_CM_CLASS(	m_CharMakeInfo.JobType ) );
	sTotalInfo.WearedItemIdx[ eWearedItem_Dress	] =	GetDressIndex( ENUM_CM_CLASS( m_CharMakeInfo.JobType ) );
	sTotalInfo.WearedItemIdx[ eWearedItem_Costume_Dress	] =	GetCDressIndex(	ENUM_CM_CLASS( m_CharMakeInfo.JobType )	);
	sTotalInfo.WearedItemIdx[ eWearedItem_Costume_Glove	] =	GetCGloveIndex(	ENUM_CM_CLASS( m_CharMakeInfo.JobType )	);
	sTotalInfo.WearedItemIdx[ eWearedItem_Costume_Shoes	] =	GetCShoesIndex(	ENUM_CM_CLASS( m_CharMakeInfo.JobType )	);
	sTotalInfo.WearedItemIdx[ eWearedItem_Wing ] = GetWingIndex( ENUM_CM_CLASS(	m_CharMakeInfo.JobType ) );

//	m_pNewPlayer = OBJECTMGR->AddPlayer(&sBaseInfo,0,&sTotalInfo);
	m_pNewPlayer = OBJECTMGR->AddPlayer(&sBaseInfo, 0, &sTotalInfo, 0);
	m_pNewPlayer->GetEngineObject()->ApplyHeightField(FALSE);
	m_pNewPlayer->SetOverInfoOption(0);
	m_pNewPlayer->SetPosition( pv3Pos );
	
	// 061226 LYW --- Modified this code.
    //MOVEMGR->SetPosition( m_pNewPlayer, &pos );
	//MOVEMGR->SetAngle( m_pNewPlayer, m_fAngle, 0 );
	MOVEMGR->SetPosition( m_pNewPlayer, pv3Pos );
	MOVEMGR->SetAngle( m_pNewPlayer, 1.7f, 0 );
	
	// 061226 LYW --- Change motion of character.
	/*
	m_pNewPlayer->ChangeMotion(eMotion_Battle_Gum_Standard + m_dwCurIndex[CE_WEAPON],1);
	m_pNewPlayer->ChangeBaseMotion(eMotion_Battle_Gum_Standard + m_dwCurIndex[CE_WEAPON]);	
	*/
	//m_pNewPlayer->ChangeMotion( CHARACTER_MOTION[ eCharacterMotion_Standard ][ WP_ONEHANDED ] ,1);
	m_pNewPlayer->ChangeMotion( CHARACTER_MOTION[ eCharacterMotion_Battle ][ WP_NONE ] ,1);
	m_pNewPlayer->ChangeBaseMotion( CHARACTER_MOTION[ eCharacterMotion_Standard ][ WP_NONE ] );	

	// 061226 LYW --- Change scale of character.
	vScale.x = 0.3f ;
	vScale.y = 0.3f ;
	vScale.z = 0.3f ;

	m_pNewPlayer->GetEngineObject()->SetScale( &vScale ) ;
	m_pNewPlayer->SetAngle( 1.7f ) ;
}

DWORD cCharMakeManager::GetWeaponIndex( ENUM_CM_CLASS jobType ) const
{
	switch( jobType )
	{
	case e_FIGHTER:
		{
			//return 11000001;
			return 60000548;
		}
	case e_ROGUE:
		{
			return 60000549;
		}
	case e_MAGE:
		{
			return 11007827;
		}
	}

	return 0;
}

DWORD cCharMakeManager::GetShieldIndex( ENUM_CM_CLASS jobType ) const
{
	switch( jobType )
	{
	case e_FIGHTER:
		{
			//return 11000001;
			return 60000551;
		}
	case e_ROGUE:
		{
			return 60000551;
		}
	case e_MAGE:
		{
			return 60000551;
		}
	}

	return 0;
}

DWORD cCharMakeManager::GetDressIndex( ENUM_CM_CLASS jobType ) const
{
	switch( jobType )
	{
	case e_FIGHTER:
		{
			return 12000032;
		}
	case e_ROGUE:
		{
			return 12000032;
		}
	case e_MAGE:
		{
			return 12000001;
		}
	}

	return 0;
}

DWORD cCharMakeManager::GetCDressIndex( ENUM_CM_CLASS jobType ) const
{
	switch( jobType )
	{
	case e_FIGHTER:
		{
			//return 11000001;
			return 13000850;
		}
	case e_ROGUE:
		{
			return 13000858;
		}
	case e_MAGE:
		{
			return 13000846;
		}
	}

	return 0;
}
DWORD cCharMakeManager::GetCGloveIndex( ENUM_CM_CLASS jobType ) const
{
	switch( jobType )
	{
	case e_FIGHTER:
		{
			//return 11000001;
			return 13000851;
		}
	case e_ROGUE:
		{
			return 13000859;
		}
	case e_MAGE:
		{
			return 13000847;
		}
	}

	return 0;
}
DWORD cCharMakeManager::GetCShoesIndex( ENUM_CM_CLASS jobType ) const
{
	switch( jobType )
	{
	case e_FIGHTER:
		{
			//return 11000001;
			return 13000852;
		}
	case e_ROGUE:
		{
			return 13000860;
		}
	case e_MAGE:
		{
			return 13000848;
		}
	}

	return 0;
}
DWORD cCharMakeManager::GetWingIndex( ENUM_CM_CLASS jobType ) const
{
	switch( jobType )
	{
	case e_FIGHTER:
		{
			//return 11000001;
			return 13000689;
		}
	case e_ROGUE:
		{
			return 13000689;
		}
	case e_MAGE:
		{
			return 13000689;
		}
	}

	return 0;
}

void cCharMakeManager::NewCharacterRotate( int x )
{
	if( !m_pNewPlayer ) return;

	m_fAngle -= x*CHARROTATE_RATE*30;

	
	if( m_fAngle >= 360 )
		m_fAngle = 0;

//	m_pNewPlayer->SetAngle( m_fAngle );
	MOVEMGR->SetAngle( m_pNewPlayer, m_fAngle, 0 );
}


void cCharMakeManager::ApplyScale()
{
}


cPtrList* cCharMakeManager::GetOptionList(int idx)
{
	if( idx < 0 || CE_MAX <= idx ) return NULL ; 
	
	return &m_UserSelectOption[idx] ; 
}