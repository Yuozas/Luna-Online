#pragma once
//=================================================================================================
//	FILE		: CChatRoomMainDlg.h
//	DESC		: Dialog interface to show chatting room list.
//	PROGRAMMER	: Yongs Lee
//	DATE		: APRIL 3, 2008
//=================================================================================================





//-------------------------------------------------------------------------------------------------
//		Define some macroinstruction.
//-------------------------------------------------------------------------------------------------
//#define MAX_ROW_GUESTLIST		10

enum ENUM_CHATROOM_MAINDLG_SEARCH_MODE		// �˻���� �з� ����.
{
	e_Search_Title = 0,						// ���� �˻�.
	e_Search_Name,							// �̸� �˻�.

	e_Search_Max,
} ;

enum ENUM_CHATROOM_JOIN_DLG_TYPE			// ä�ù� ���� ���̾�α� �з� ����.
{
	e_JoinDlg_Type_RoonIndex = 0,			// ���ȣ �Է� ���.
	e_JoinDlg_Type_SecretCode,				// ��й�ȣ �Է� ���.

	e_JoinDlg_Type_Max,
} ;





//-------------------------------------------------------------------------------------------------
//		Include header files.
//-------------------------------------------------------------------------------------------------
#include "./interface/cDialog.h"
#include "./interface/cComboBox.h"

class cButton ;
class cComboBox ;
class cEditBox ;
class cListDialog ;
class cPushupButton ;
class cStatic ;





//-------------------------------------------------------------------------------------------------
//		The class CChatRoomMainDlg.
//-------------------------------------------------------------------------------------------------
class CChatRoomMainDlg : public cDialog
{
	// ä�ù� ������ ���� Map.
	typedef std::list<ST_CR_ROOM_CLT> LIST_ROOM ;
	LIST_ROOM			m_Room ;

	// ��Ȳ�� ���� �ӽ÷� ä�ù� ������ ���� Map.
	typedef std::list<ST_CR_ROOM_CLT> LIST_TEMPROOM ;
	LIST_TEMPROOM			m_TempRoom ;

	// ���� �� ä�ù� ����.
	ST_CR_ROOM_CLT	m_SelectRoom ;

//	cPtrList			m_RoomList ;
//
//public :
//	void ReleaseRoom() ;													// ä�ù� ���ҽ� ���� �Լ�.
//
//	BYTE IsInRoomList(BYTE byRoomIdx) ;										// �� ����Ʈ��, �� ������ �ִ��� üũ�ϴ� �Լ�.
//
//	BYTE AddRoom(ST_CR_ROOM_CLT* pInfo) ;							// ä�ù� ������ �߰��ϴ� �Լ�.
//	BYTE DeleteRoom(BYTE byRoomIdx) ;										// ä�ù� ������ �����ϴ� �Լ�.
//
//	ST_CR_ROOM_CLT* GetRoomInfo(BYTE byRoomIdx) ;					// ä�ù� ������ ��ȯ�ϴ� �Լ�.
//
//	void UpdateRoom(ST_CR_ROOM_CLT* pInfo) ;

private :	
	cListDialog*		m_pRoomList ;					// ä�ù渦 ������ ����Ʈ ���̾�α�.

	cButton*			m_pSearchBtn ;					// �˻� ��ư.
	cEditBox*			m_pSearchBox ;					// �˻� �Է� ����Ʈ�ڽ�.
	cPushupButton*		m_pTitle ;						// ���� �˻� ��ư.
	cPushupButton*		m_pName ;						// �̸� �˻� ��ư.

	cComboBox*			m_pRoomType ;					// ä�ù� �з� �޺��ڽ�.

	cButton*			m_pCreateBtn ;					// ä�ù� ���� ��ư.
	cButton*			m_pJoinBtn ;					// ä�ù� ���� ��ư.
	cButton*			m_pNumJoinBtn ;					// ä�ù� ��ȣ�Է� ���� ��ư.
	cButton*			m_pRefreshBtn ;					// ���ΰ�ħ ��ư.

	cButton*			m_pPrevBtn ;					// ���� ������ ��ư.
	cButton*			m_pNextBtn ;					// ���� ������ ��ư.

	cStatic*			m_pPage ;						// ������ ��� ����ƽ.

	BYTE				m_bySearchMode ;				// �˻� ��带 ���� ����.

	BYTE				m_byRoomType ;					// ä�ù� �з��� ���� ����.

	BYTE				m_byCurPage ;					// ���� �������� ���� ����.
	BYTE				m_byMaxPage ;					// �ִ� �������� ���� ����.

	//cStatic*			m_pLoad_Chatroom ;

	//char				m_SearchBuff[128] ;				// 

	//BYTE				m_byPrintMode ;		// ���� ��µǰ� �ִ� ����Ʈ�� � ������� ��� ����.

public:
	CChatRoomMainDlg(void) ;							// ������ �Լ�.
	virtual ~CChatRoomMainDlg(void) ;					// �Ҹ��� �Լ�.

	void Linking() ;									// ���� ��Ʈ�� ��ũ �Լ�.

	// ���콺 �̺�Ʈ ó�� �Լ�.
	virtual DWORD ActionEvent( CMouse* mouseInfo ) ;
	// EventFunc�� ���� �Ѿ�� Event�� ó���ϴ� �Լ�.
	virtual void OnActionEvent(LONG id, void* p, DWORD event) ;

	//virtual void SetActive(BOOL val) ;

	//virtual DWORD ActionKeyboardEvent( CKeyboard * keyInfo ) ;

public :
	void Search_Syn() ;									// �˻� ó���� �ϴ� �Լ�.

	void Check_Title() ;								// �˻���带 ���� �˻����� �����ϴ� �Լ�.
	void Check_Name() ;									// �˻���带 �̸� �˻����� �����ϴ� �Լ�.
	
	void Create_Syn() ;									// ä�ù� ���� ó���� �ϴ� �Լ�.
	void Join_Syn() ;									// ä�ù� ���� ó���� �ϴ� �Լ�.
	void NumJoin_Syn() ;								// ���ȣ �Է� ���� ó���� �ϴ� �Լ�.
	void Refresh_Syn() ;								// ���ΰ�ħ ó���� �ϴ� �Լ�.

	void Btn_Prev() ;									// ���� ������ ó���� �ϴ� �Լ�.
	void Btn_Next() ;									// ���� ������ ó���� �ϴ� �Լ�.

	//void SortRoomByType() ;	

	// ����â/��й�ȣâ���� OK��ư�� ������ �� ó���ϴ� �Լ�.
	void Check_Room_To_Join(BYTE inputMode, char* pCode) ;

	// ä�ù� ������ ��� �ִ� ����Ʈ�� ��ȯ�ϴ� �Լ�.
	cListDialog* GetListDlg() { return m_pRoomList ; }

public :
	void Init_RoomList(void* pMsg) ;					// ä�ù� ����Ʈ�� �����ϴ� �Լ�.
	//void Init_TypeRoomList(void* pMsg) ;
	void Refresh_RoomList() ;							// ä�ù� ����Ʈ�� ���ΰ�ħ�ϴ� �Լ�.
	void Clear_RoomList() ;								// ä�ù� ����Ʈ�� ���� �Լ�.

	BOOL Add_Room(ST_CR_ROOM_CLT* pInfo) ;				// ä�ù� �߰��Լ�.
	void Update_RoomInfo(ST_CR_ROOM_CLT* pInfo) ;		// ä�ù� ������ ������Ʈ �ϴ� �Լ�.

	void DeleteRoom(BYTE byRoomIdx) ;					// ä�ù��� �����ϴ� �Լ�.
	void SetPage(BYTE num1, BYTE num2) ;				// �������� �����ϴ� �Լ�.

	//BYTE GetCurRoomType() { return (BYTE)m_pRoomType->GetCurSelectedIdx() ; }

	// ä�ù� ������ ��ȯ�ϴ� �Լ�.
	ST_CR_ROOM_CLT* GetRoomListItem(BYTE byRoomIdx) ;

public : 
	void Err_EmptyRoom() ;								// ä�ù��� ���� ��, ���� �� ���� ���ٴ� ������ ���� �Լ�.
};
