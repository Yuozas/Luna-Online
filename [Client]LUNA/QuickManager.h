// QuickManager.h: interface for the CQuickManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUICKMANAGER_H__B3C070DF_4FEC_4F04_BD3C_8F534A23A442__INCLUDED_)
#define AFX_QUICKMANAGER_H__B3C070DF_4FEC_4F04_BD3C_8F534A23A442__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define QUICKMGR USINGTON(CQuickManager)

// 080702 LYW --- QuickManager : 확장 슬롯 추가에 따른 슬롯 최대 페이지를 4로 변경한다.
#define TOTAL_SLOTPAGE	8
#define MAX_SLOTPAGE	4
#define MAX_SLOTNUM		10

class cIcon;
class CItem;
class cImage;
class cQuickItem;
class CBaseItem;

// 080702 LYW --- QuickManager : 확장 슬롯 추가 처리.
enum SLOT_KIND
{
	e_Slot_First = 0 ,			// 첫번째 슬롯
	e_Slot_Second,				// 확장 슬롯
} ;

class CQuickManager  
{
	// 080702 LYW --- QuickManager : 확장 슬롯 추가 처리.
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

	// 071215 LYW --- QuickManager : 퀵슬롯에 등록 된 아이템이 인벤에서 삭제될 때, 퀵 아이템을 삭제하는 함수 추가.
	//
	// 퀵 슬롯에 등록 된 아이템이 인벤토리에서 삭제될 때, 퀵슬롯에 등록된 퀵 아이템도 삭제해야 한다. 
	// 봉인관련 아이템이나, 인챈트, 강화에 의해 기본 아이템정보에서 변경이 된 아이템들의 툴팁이 뿌려지고 있는상황에서, 
	// 인벤에서만 아이템이 삭제될 경우, 이전에 진행하고 있던 처리과정의 정보들을 얻지 못해, 에러가 발생하기 때문이다.
	//
	void DeleteLinkdedQuickItem(DWORD dwDBIdx) ;
	// 071215 LYW --- QuickManager : 봉인된 아이템이 링크가 걸려있을 경우,
	// 봉인이 풀렸을 경우 봉인을 풀어주기 위한 함수 추가.
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

	// 071206 LYW --- QuickManager : 퀵슬롯을 모두 비우는 함수 추가.
	void Quick_Remove_AllSkill() ;

	void Quick_Change_Ack( void* pMsg ) ;

	void AddToolTip( cQuickItem* pQuickItem );

	// 070702 웅주, 스킬 슬롯을 보관하고, 업데이트
public:
	void RefreshSkillItem( DWORD skillIndex );

	// 071126 LYW --- QuickManager : 퀵 아이템 반환 함수.
	cQuickItem* GetQuickItem(WORD tab, WORD pos) ;
};

EXTERNGLOBALTON(CQuickManager);
#endif // !defined(AFX_QUICKMANAGER_H__B3C070DF_4FEC_4F04_BD3C_8F534A23A442__INCLUDED_)
