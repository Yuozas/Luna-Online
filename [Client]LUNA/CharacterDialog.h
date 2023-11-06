// CharacterDialog.h: interface for the CCharacterDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARACTERDIALOG_H__EF27ABB3_EA18_4466_88E9_4035E3807912__INCLUDED_)
#define AFX_CHARACTERDIALOG_H__EF27ABB3_EA18_4466_88E9_4035E3807912__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"
#include "./Interface/cStatic.h"
#include "./Interface/cGuagen.h"

class CGuildMarkDialog;
class cStatic;

// 070111 LYW --- Define max count for sub item of tree dialog.
#define MAX_SUBITEM			5
//#define MAX_CLASS_STATIC	9

// 070112 LYW --- Define max count for character image.
#define MAX_CHARIMAGE		4

// 070113 LYW --- Define max count for static of background image.
#define MAX_BGSTATIC			6
#define MAX_CIRCLE_IMAGE		9
#define	MAX_CHAR_IMAGE			4
#define MAX_RESISTANCE_IMAGE	6
#define MAX_LOCKED_TEXT			33
#define MAX_CHANGE_TEXT			29
#define MAX_STATUS_BTN			5
#define MAX_SUB_ITEMS			5
#define MAX_CLASS_STATIC		9

struct CHARSTATICCTRL
{
	cStatic* munpa;
	cStatic* jikwe;
	cStatic* fame;
	cStatic* badfame;
	cStatic* name;
//	cStatic* stage;		// ����
//	cStatic* job;		// ����
//	cStatic* ideology;	// ��ġ��

	cStatic* Str;
	cStatic* Wis;
	cStatic* Dex;
	cStatic* Vit;

	// 070111 LYW --- Add static control for intelligence of character.
	cStatic* Int ;

	cStatic* level;
	cStatic* expPercent;		// �������� ǥ��
	cStatic* point;		// �ܿ�����Ʈ

	cStatic* meleeattack;	// ���ݷ�
	cStatic* rangeattack;	// ���ݷ�
	cStatic* defense;
//	cStatic* kyeongkong;
//	cStatic* move;
	cStatic* life;
	cStatic* Mana;
//	cStatic* dok;

	cStatic* spname;
	cStatic* critical;
	cStatic* attackdistance;	
};

struct ATTRDEFENCE
{
	ATTRIBUTE_VAL<cStatic*> pStatic;
	ATTRIBUTE_VAL<cGuagen*> pGuage;

	void SetValue(WORD attrib,WORD value,DWORD color=0)
	{
		char temp[32];
		sprintf(temp,"%d%%",value);
		pStatic.GetElement_Val(attrib)->SetStaticText(temp);
		pGuage.GetElement_Val(attrib)->SetValue(value*0.01f);
		if( color )
			pStatic.GetElement_Val(attrib)->SetFGColor(color);
	}
};

class cGuagen;

class CCharacterDialog  : public cDialog
{
	CHARSTATICCTRL		m_ppStatic;			// ���� ��µ� ������
	cButton *			m_pPointBtn[MAX_BTN_POINT];

	// 070111 LYW --- Add button control for call class tree dialog.
	//cButton*			m_pClassTreeBtn ;

	// 070111 LYW --- Add new controls for class tree.
	/*
	cStatic*			m_pTreeBgImg ;
	cStatic*			m_pTreeMark ;
	cStatic*			m_pTreeTitle ;
	cStatic*			m_pSubItem[ MAX_SUBITEM ] ;
	cStatic*			m_pClassStatic[ MAX_CLASS_STATIC ] ;
	*/

	// 070112 LYW --- Add static controls for character image.
	//cStatic*			m_pCharImage[ MAX_CHARIMAGE ] ;

	// 070111 LYW --- Delete this line.
	//cButton*			m_pPointMinusBtn[MAX_BTN_POINT];
	int					m_nocoriPoint;
	CGuildMarkDialog*	m_pGuildMarkDlg;

	ATTRDEFENCE			m_AttrDefComponent;

	int					m_MinusPoint;

	// 070111 LYW --- Add variable for show tree dialogl.
	BOOL				m_bShowTreeDlg ;
	BOOL				m_bCheckedTreeInfo ;

	// 070112 LYW --- Add variable to store up character image number.
	BYTE				m_byCharImgNum ;

	// 070113 LYW --- Add static controls for background image.
	// 070118 LYW --- Delete this control.
	//cStatic*			m_pBGStatic[ MAX_BGSTATIC ] ;
	// 070118 LYW --- Delete this control.
	//cStatic*			m_pCircleImage[ MAX_CIRCLE_IMAGE ] ;
	cStatic*			m_pCharImage[ MAX_CHAR_IMAGE ] ;
	// 070118 LYW --- Delete this control.
	//cStatic*			m_pResistanceImage[ MAX_RESISTANCE_IMAGE ] ;
	// 070118 LYW --- Delete this control.
	//cStatic*			m_pLockedText[ MAX_LOCKED_TEXT ] ;
	cStatic*			m_pChangeText[ MAX_CHANGE_TEXT ] ;

	cButton*			m_pStatusBtn[ MAX_STATUS_BTN ] ;
	cButton*			m_pCloseBtn ;
	cButton*			m_pClassTreeBtn ;

	cStatic*			m_pTreeBgImg ;
	cStatic*			m_pTreeMark ;
	cStatic*			m_pTreeTitle ;
	cStatic*			m_pSubItem[ MAX_SUB_ITEMS ] ;
	cStatic*			m_pClassStatic[ MAX_CLASS_STATIC ] ;

	// 070416 LYW --- CharacterDialog : Add static to print racial.
	cStatic*			m_pRacial ;

	// 070503 LYW --- CharacterDialog : Add static to setting family name.
	cStatic*			m_pFamilyName ;
public:
	CCharacterDialog();
	virtual ~CCharacterDialog();
	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);
	void Linking();
	virtual void SetActive( BOOL val );	

	void UpdateData();				// ���� ������ �ʴ°�
	CHARSTATICCTRL * GetCharacterData(){return &m_ppStatic;}
	void RefreshGuildInfo();

	// 070503 LYW --- CharacterDialog : Add function to setting family name
	void RefreshFamilyInfo() ;

	void SetLevel(LEVELTYPE level);
	void SetLife(DWORD life);
	void SetMana(DWORD mana);
	void SetStrength();
	void SetDexterity();
	void SetVitality();
	void SetWisdom();

	// 070411 LYW --- CharacterDialog : Add function to print values of physical part.
	void PrintAttackRate() ;
	void PrintDefenseRate() ;
	void PrintMagAttackRate() ;
	void PrintMagDefenseRate() ;
	void PrintAccuracyRate() ;
	void PrintEvasionRate() ;
	void PrintCriticalRate() ;

	// 070113 LYW --- Add function to setting intelligence.
	void SetIntelligence() ;

	void SetExpPointPercent(float perc);
	void SetAttackRate();
	void SetDefenseRate();
	
	void SetCritical();
	void SetAttackRange();
	void SetFame(FAMETYPE fame);
	void SetBadFame(FAMETYPE badfame);

	void SetPointLeveling(BOOL val, WORD point=0);	
	void OnAddPoint(BYTE whatsPoint);
	void SetMinusPoint(BOOL val, WORD point=0);
	void OnMinusPoint(BYTE whatsPoint);
	
	// RaMa
	void SetMinusPointValue( WORD point )		{	m_MinusPoint = point;	}
	void SetPointLevelingHide();
	void RefreshInfo();

	// 070111 LYW --- Add function to process events.
	virtual DWORD ActionEvent( CMouse* mouseInfo ) ;
	void OnActionEvent( LONG lId, void* p, DWORD we ) ;

	// 070111 LYW --- Add function to check and show tree information.
	void CheckTreeInfo() ;
	void ShowTreeDlg() ;

	// 070112 LYW --- Add function to check character image.
	void CheckHeroImage() ;

	// 070115 LYW --- Add function to setting view tree dialog.
	void SetViewTreeDlg( BOOL val ) { m_bShowTreeDlg = val ; ShowTreeDlg() ; }

	// 070416 LYW --- CharacterDialog : Add function to change class name.
	void SetClassName( char* className ) { m_pChangeText[ 5]->SetStaticText( className ) ; }
};

#endif // !defined(AFX_CHARACTERDIALOG_H__EF27ABB3_EA18_4466_88E9_4035E3807912__INCLUDED_)
