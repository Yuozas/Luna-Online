// QuickManager.h: interface for the CQuickManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUICKMANAGER_H__B3C070DF_4FEC_4F04_BD3C_8F534A23A442__INCLUDED_)
#define AFX_QUICKMANAGER_H__B3C070DF_4FEC_4F04_BD3C_8F534A23A442__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define QUICKMGR USINGTON(CQuickManager)

// 080702 LYW --- QuickManager : Ȯ�� ���� �߰��� ���� ���� �ִ� �������� 4�� �����Ѵ�.
#define TOTAL_SLOTPAGE	8
#define MAX_SLOTPAGE	4
#define MAX_SLOTNUM		10

class cIcon;
class CItem;
class cImage;
class cQuickItem;
class CBaseItem;

// 080702 LYW --- QuickManager : Ȯ�� ���� �߰� ó��.
enum SLOT_KIND
{
	e_Slot_First = 0 ,			// ù��° ����
	e_Slot_Second,				// Ȯ�� ����
} ;

class CQuickManager  
{
	// 080702 LYW --- QuickManager : Ȯ�� ���� �߰� ó��.
	//cQuickItem* mQuickItemTable[ 8 ][ 10 ];
	cQuickItem* mQuickItemTable[ MAX_SLOTPAGE ][ MAX_SLOTNUM ];
	cQuickItem* mExQuickItemTable[ MAX_SLOTPAGE ][ MAX_SLOTNUM ];

	CIndexGenerator m_IconIndexCreator;

public:
	CQuickManager();
	virtual ~CQuickManager();

	cQuickItem* NewQuickItem( WORD tab, WORD pos );

	BOOL CanEquip( cIcon * pIcon );
	void Release();
	
	void UseQuickItem( WORD tab, WORD pos );

	void NetworkMsgParse( BYTE Protocol, void* pMsg );

	void RefreshQickItem();

	BOOL CheckQPosForItemIdx( DWORD ItemIdx );

	// 071215 LYW --- QuickManager : �����Կ� ��� �� �������� �κ����� ������ ��, �� �������� �����ϴ� �Լ� �߰�.
	//
	// �� ���Կ� ��� �� �������� �κ��丮���� ������ ��, �����Կ� ��ϵ� �� �����۵� �����ؾ� �Ѵ�. 
	// ���ΰ��� �������̳�, ��æƮ, ��ȭ�� ���� �⺻ �������������� ������ �� �����۵��� ������ �ѷ����� �ִ»�Ȳ����, 
	// �κ������� �������� ������ ���, ������ �����ϰ� �ִ� ó�������� �������� ���� ����, ������ �߻��ϱ� �����̴�.
	//
	void DeleteLinkdedQuickItem(DWORD dwDBIdx) ;
	// 071215 LYW --- QuickManager : ���ε� �������� ��ũ�� �ɷ����� ���,
	// ������ Ǯ���� ��� ������ Ǯ���ֱ� ���� �Լ� �߰�.
	void UnsealQuickItem(DWORD dwDBIdx) ;

	void ReleaseQuickItem( cQuickItem * quick );


private:
	void GetImage( cQuickItem* pQuickItem );

	void AddQuickItem( WORD tab, WORD pos, SLOT_INFO* pSlot );
	void RemoveQuickItem( WORD tab, WORD pos );
	void ChangeQuickItem( WORD tab1, WORD pos1, WORD tab2, WORD pos2 );

	// 070205 LYW --- Add functions to process network msg.
public :
	void Quick_Info( void* pMsg ) ;
	void Quick_Add_Ack( void* pMsg ) ;
	void Quick_Remove_Ack( void* pMsg ) ;

	// 071206 LYW --- QuickManager : �������� ��� ���� �Լ� �߰�.
	void Quick_Remove_AllSkill() ;

	void Quick_Change_Ack( void* pMsg ) ;

	void AddToolTip( cQuickItem* pQuickItem );

	// 070702 ����, ��ų ������ �����ϰ�, ������Ʈ
public:
	void RefreshSkillItem( DWORD skillIndex );

	// 071126 LYW --- QuickManager : �� ������ ��ȯ �Լ�.
	cQuickItem* GetQuickItem(WORD tab, WORD pos) ;
};

EXTERNGLOBALTON(CQuickManager);
#endif // !defined(AFX_QUICKMANAGER_H__B3C070DF_4FEC_4F04_BD3C_8F534A23A442__INCLUDED_)
