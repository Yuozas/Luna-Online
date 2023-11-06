/*
	2007/03/05 RnD팀 이진영 처음 만듬.
	데이트 매칭 시스템 메인 다이얼로그 겸, 일단 연습코드 일부 포함.
*/


#include "./Interface/cTabDialog.h"
#include "./Interface/cIcon.h"

#include "indexgenerator.h"

#include "../hseos/ResidentRegist/SHResidentRegistManager.h"

//class CMyIcon : public cIcon
//{
//	virtual BOOL CanMoveToDialog( cDialog* targetdlg )	{	return FALSE;	}
//	virtual BOOL CanDelete()							{	return FALSE;	}	
//	virtual DWORD ActionEvent(CMouse * mouseInfo)
//	{
//		return cIcon::ActionEvent( mouseInfo);
//	}
//
//};

// 윗쪽 다이얼로그 박스.
// DateMatchingUpperDlg.bin에 연결.
//class CDateMatchingUpperDlg : public cTabDialog
//{
//	int m_iTabIndex;
//public:
//	CDateMatchingUpperDlg();
//	virtual ~CDateMatchingUpperDlg();
//
//	virtual void Add(cWindow * window);
//
//	void	InitMyDlg(void);
//};
//
//
//// 아래쪽 다이얼로그 박스.
//// DateMatchingLowerDlg.bin에 연결.
//class CDateMatchingLowerDlg : public cTabDialog
//{
//	int	m_iTabIndex;
//public:
//	CDateMatchingLowerDlg();
//	virtual ~CDateMatchingLowerDlg();
//
//	virtual void Add(cWindow * window);
//
//	void	InitMyDlg(void);
//
//};

#define FILE_FAVOR_ITEM_LIST	"./Data/Interface/Windows/FavorItemList.bin"

#define MAX_FAVOR_ITEM_COUNT	20

#define MAX_GOOD_ICON_COUNT		3
#define MAX_BAD_ICON_COUNT		2

#define MAX_MATCH_CHAT_MSG		46

// PARTNER LIST PART
#define MAX_LIST_LINE			5
#define MIN_LIST_PAGE			1
#define MAX_LIST_PAGE			80

enum ENUM_MATCH_RACK
{
	e_MATCH_HUMAN = 0,
	e_MATCH__ELF,
} ;

enum ENUM_MATCH_GENDER
{
	e_MATCH_MAN = 0,
	e_MATCH_WOMAN,
} ;

enum ENUM_MATCH_FACE
{
	e_MATCH_HUMAN_MAN = 92,
	e_MATCH_HUMAN_WOMAN,
	e_MATCH_ELF_MAN,
	e_MATCH_ELF_WOMAN,
} ;

class cComboBox ;
class cCheckBox ;
class cButton ;
class cListCtrl ;
class cListDialog ;
class cEditBox ;
class cStatic ;
class cIconGridDialog ;
class cTextArea ;

class CFavorItem : public cIcon
{
	DWORD m_dwItemIdx ;

	char m_ItemName[256] ;

	DWORD m_dwToolTipNum ;

	WORD m_w2DImageNum ;
	WORD m_wPos ;

public :
	CFavorItem() ;
	virtual ~CFavorItem() ;

	void SetItemIdx(DWORD idx)  { m_dwItemIdx = idx ; }
	DWORD GetItemIdx()			{ return m_dwItemIdx ; }

	void SetItemName(char* name) { strcpy(m_ItemName, name) ; }
	char* GetItemName()			{ return m_ItemName ; }

	void SetToolTipNum(DWORD tipNum)	{ m_dwToolTipNum = tipNum ; }
	DWORD GetToolTipNum()				{ return m_dwToolTipNum ; }

	void SetImageNum(WORD wImgNum)		{ m_w2DImageNum = wImgNum ; }
	WORD GetImageNum()					{ return m_w2DImageNum ; }

	void SetPos(WORD wPos)				{ m_wPos = wPos ; }
	WORD GetPos()						{ return m_wPos ; }
} ;

class CIdentification : public cDialog
{
	cImage		m_FacePicture ;

	cStatic*	m_pNickName ;
	cStatic*	m_pGender ;
	cEditBox*	m_pAge ;
	cComboBox*	m_pRegion ;
	cStatic*	m_pFavor ;

	CIndexGenerator m_IconIndexCreator;

	cIconGridDialog*	m_pGoodIcon ;
	cIconGridDialog*	m_pBadIcon ;

	//cPtrList			m_pIconList ;

	cPtrList			m_FavorIconList ;

	cButton*			m_pSaveBtn ;
	cButton*			m_pCancelBtn ;

	ITEM*				m_pRegionItem ;

	CSHResidentRegistManager::stRegistInfo m_RegistInfo ;

public :
	// CONSTRUCT & DESTRUCT
	CIdentification() ;
	virtual ~CIdentification() ;

    // EVENT PART
	virtual DWORD ActionEvent(CMouse* mouseInfo) ;
	void OnActionEvent(LONG lId, void * p, DWORD we);

	// SETTING CONTROLS PART.
	void Linking() ;
	void SettingControls() ;

	void ResetAllData() ;

	// CREATE ICON PART.
	void LoadFavorItem() ;

	virtual void Render() ;

	cPtrList* GetFavorIconList() { return &m_FavorIconList ; }

	// ICON MOVE PART.
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon) ;
	BOOL FakeMoveItem(LONG mouseX, LONG mouseY, cIcon * icon) ;
	BOOL GetPositionForXYRef(LONG x, LONG y, WORD& pos) ;
	void SetGoodIcon(int nPos, CFavorItem* pItem) ;
	void SetBadIcon(int nPos, CFavorItem* pItem) ;
	BOOL CheckSameItem(CFavorItem* pItem) ;
	//void DeleteItem(CMouse* mouseInfo) ;
	void DeleteItem(CFavorItem* pItem) ;


	//void SetFacePicture(cStatic* pPicture)	{ m_pFacePicture = pPicture ; }
	cImage* GetFacePicture()				{ return &m_FacePicture ; }

	void SetNickName(cStatic* pNickName)	{ m_pNickName = pNickName ; }
	cStatic* GetNickName()					{ return m_pNickName ; }

	void SetGender(cStatic* pGender)		{ m_pGender = pGender ; }
	cStatic* GetGender()					{ return m_pGender ; }

	void SetAge(cEditBox* pAge)				{ m_pAge = pAge ; }
	cEditBox* GetAge()						{ return m_pAge ; }

	void SetRegion(cComboBox* pRegion)		{ m_pRegion = pRegion ; }
	cComboBox* GetRegion()					{ return m_pRegion ; }

	void SetFavor(cStatic* pFavor)			{ m_pFavor = pFavor ; }
	cStatic* GetFavor()						{ return m_pFavor ; }

	void SetGoodIcon(cIconGridDialog* pGoodIcon)	{ m_pGoodIcon = pGoodIcon ; }
	cIconGridDialog* GetGoodIcon()					{ return m_pGoodIcon ; }

	void SetBadIcon(cIconGridDialog* pBadIcon)		{ m_pBadIcon = pBadIcon ; }
	cIconGridDialog* GetBadIcon()					{ return m_pBadIcon ; }

	void SetSaveBtn(cButton* pSave)			{ m_pSaveBtn = pSave ; }
	cButton* GetSaveBtn()					{ return m_pSaveBtn ; }

	void SetCancelBtn(cButton* pCancel)		{ m_pCancelBtn = pCancel ; }
	cButton* GetCancelBtn()					{ return m_pCancelBtn ; }

	CSHResidentRegistManager::stRegistInfo* GetRegistInfo() { return &m_RegistInfo ; }

	void CancelRegist() ;

} ;

class CFavorIcon : public cDialog
{
	CIndexGenerator m_IconIndexCreator;
	cIconGridDialog*	m_pFavorIcon ;

	cPtrList			m_FavorItemList ;

	DWORD				m_CurSelectedItemIdx;
	LONG				m_lCurSelItemPos;

public :
	CFavorIcon() ;
	virtual ~CFavorIcon() ;

	virtual DWORD ActionEvent(CMouse* mouseInfo) ;
	void OnActionEvent(LONG lId, void * p, DWORD we);

	void Linking() ;

	void LoadFavorItem() ;
	void ShowFavorItem() ;

	// ICON MOVE PART.
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon) ;


	virtual void Render() ;

	cImage* GetIconImage(WORD ImgIdx, cImage * pImageGet) ;

	cPtrList* GetFavorItemList() { return &m_FavorItemList ; }
} ;

class CDateMatchingPartnerList : public cDialog
{
	cComboBox*	m_pAge ;
	cComboBox*	m_pRegion ;

	cCheckBox*	m_pGenderMan ;
	cCheckBox*	m_pGenderWoman ;

	cButton*	m_pSearch ;
	cButton*	m_pRefresh ;
	cButton*	m_pChatting ;

	cListCtrl*	m_pPartnerList ;

	cButton*	m_pList_Gender ;
	cButton*	m_pList_Age ;
	cButton*	m_pList_NickName ;
	cButton*	m_pList_Favor ;
	cButton*	m_pList_Point ;

	cPtrList	m_PartnerList ;

	/*cStatic*	m_pOverImage ;
	cStatic*	m_pSelectImage ;*/

	int			m_nPage ;
	int			m_nTotalPage ;
	// desc_hseos_데이트매칭01
	// S 데이트매칭 추가 added by hseos 2008.01.07
	int			m_nSerchGroupIndex;
	int			m_nSerchGroupNum;
	// E 데이트매칭 추가 added by hseos 2008.01.07

	cButton*	m_pBtnPrev ;
	cButton*	m_pBtnNext ;

	cStatic*	m_pPageNum ;
	cStatic*	m_pSlash ;
	cStatic*	m_pTotalPage ;

	// desc_hseos_데이트매칭01
	// S 데이트매칭 추가 added by hseos 2008.01.07
	cStatic*	m_pGroupNum ;
	cStatic*	m_pGroupSlash ;
	cStatic*	m_pGroupTotalPage ;
	// E 데이트매칭 추가 added by hseos 2008.01.07

	int			m_nGender ;

public :
	CDateMatchingPartnerList() ;
	virtual ~CDateMatchingPartnerList() ;

	virtual DWORD ActionEvent(CMouse* mouseInfo) ;
	void OnActionEvent(LONG lId, void * p, DWORD we) ;

	void Linking() ;

	void SettingControls() ; 

	void UpdateInfo(CSHResidentRegistManager::stSerchResult* pResult) ;
	//void UpdateInfo(int nCount) ;
	void ResetPartnerList() ;

	void StartChatting() ;

	// link controls part.
	void SetAge(cComboBox* pAge)				{ m_pAge = pAge ; }
	cComboBox* GetAge()							{ return m_pAge ; }

	void SetRegion(cComboBox* pRegion)			{ m_pRegion = pRegion ; }			
	cComboBox* GetRegion()						{ return m_pRegion ; }		

	void SetGenderMan(cCheckBox* pGender)		{ m_pGenderMan = pGender ; }
	cCheckBox* GetGenderMan()					{ return m_pGenderMan ; }

	void SetGenderWoman(cCheckBox* pGender)	{ m_pGenderWoman = pGender ; }
	cCheckBox* GetGenderWoman()					{ return m_pGenderWoman ; }

	void SetSearchBtn(cButton* pBtn)			{ m_pSearch = pBtn ; }
	cButton* GetSearchBtn()						{ return m_pSearch ; }

	void SetRefreshBtn(cButton* pBtn)			{ m_pRefresh = pBtn ; }
	cButton* GetRefreshBtn()					{ return m_pRefresh ; }

	void SetChattingBtn(cButton* pBtn )		{ m_pChatting = pBtn ; }
	cButton* GetChattingBtn()					{ return m_pChatting ; }

	void SetPartnerList(cListCtrl* pList)	{ m_pPartnerList = pList ; }
	cListCtrl* GetPartnerList()				{ return m_pPartnerList ; }

	void SetCheckGender(int byGender )	;
	int GetSearchGender() { return m_nGender ; }

	void SettingPage(LONG lId) ;

	// desc_hseos_데이트매칭01
	// S 데이트매칭 추가 added by hseos 2008.01.07
	void	SetSerchGroupIndex(int nIndex)			{ m_nSerchGroupIndex = nIndex; }
	void	SetSerchGroupNum(int nNum)				{ m_nSerchGroupNum = nNum; }
	int		GetSerchGroupIndex()					{ return m_nSerchGroupIndex; }
	int		GetSerchGroupNum()						{ return m_nSerchGroupNum; }

	void	UpdateGroupInfo();
	void	ClearPartnerList();
	// E 데이트매칭 추가 added by hseos 2008.01.07
} ;

class CDateMatchingChatDlg : public cDialog
{
	cListDialog*	m_pChatList ;

	cEditBox*		m_pInputBox ;

	BOOL			m_bChatOnOff ;

public :
	CDateMatchingChatDlg() ;
	virtual ~CDateMatchingChatDlg() ;

	virtual DWORD ActionEvent(CMouse* mouseInfo) ;
	virtual DWORD ActionKeyboardEvent(CKeyboard* keyInfo) ;
	void OnActionEvent(LONG lId, void * p, DWORD we) ;

	// link controls part.
	void SetChatList(cListDialog* pList)	{ m_pChatList = pList ; }
	cListDialog* GetChatList()				{ return m_pChatList ; }

	void SetInputBox(cEditBox* pEditBox)	{ m_pInputBox = pEditBox ; }
	cEditBox* GetEditBox()					{ return m_pInputBox ; }

	void AddMsg(char* msg, DWORD dwColor) ;

	BOOL IsOnChatMode() { return m_bChatOnOff ; }
	void SetOnOffChatMode(BOOL bOnOff) { m_bChatOnOff = bOnOff ; }

} ;

class CDateMatchingInfoDlg : public cDialog
{
	cImage		m_FacePicture ;

	cStatic*	m_pNickName ;
	cStatic*	m_pGender ;
	cStatic*	m_pAge ;
	cStatic*	m_pRegion ;
	cStatic*	m_pFavor ;

	cIconGridDialog*	m_pGoodIcon ;
	cIconGridDialog*	m_pBadIcon ;

	cTextArea*			m_pIntroduce ;

	cButton*			m_pLeftBtn ;
	cButton*			m_pRightBtn ;

	cButton*			m_pIntroduce_up ;
	cButton*			m_pIntroduce_down ;
	cButton*			m_pIntroduce_guage ;

	cPtrList			m_FavorIconList ;

	CIndexGenerator m_IconIndexCreator;

	CFavorItem*			m_pGoodItem[MAX_GOOD_ICON_COUNT] ;
	CFavorItem*			m_pBadItem[MAX_BAD_ICON_COUNT] ;

public :
	CDateMatchingInfoDlg() ;
	virtual ~CDateMatchingInfoDlg() ;

	virtual void Render() ;

	virtual DWORD ActionEvent(CMouse* mouseInfo) ;
	void OnActionEvent(LONG lId, void * p, DWORD we) ;

	// ICON MOVE PART.
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon) ;

	// link controls part.
	//void SetFacePicture(cStatic* pFace)			{ m_FacePicture = pFace ; }
	cImage* GetFacePicture()					{ return &m_FacePicture ; }

	void SetNickName(cStatic* pNickName)		{ m_pNickName = pNickName ; }
	cStatic* GetNickName()						{ return m_pNickName ; }

	void SetGender(cStatic* pGender)			{ m_pGender = pGender ; }
	cStatic* GetGender()						{ return m_pGender ; }

	void SetAge(cStatic* pAge)					{ m_pAge = pAge ; }
	cStatic* GetAge()							{ return m_pAge ; }

	void SetRegion(cStatic* pRegion)			{ m_pRegion = pRegion ; }
	cStatic* GetRegion()						{ return m_pRegion ; }

	void SetFavor(cStatic* pFavor)				{ m_pFavor = pFavor ; }
	cStatic* GetFavor()							{ return m_pFavor ; }			

	void SetGoodIcon(cIconGridDialog* pIcon)	{ m_pGoodIcon = pIcon ; }
	cIconGridDialog* GetGooIcon()				{ return m_pGoodIcon ; }

	void SetBadIcon(cIconGridDialog* pIcon)		{ m_pBadIcon = pIcon ; }
	cIconGridDialog* GetBadIcon()				{ return m_pBadIcon ; }

	void SetIntroduce(cTextArea* pTextArea)		{ m_pIntroduce = pTextArea ; }
	cTextArea* GetIntroduce()					{ return m_pIntroduce ; }

	void SetLeftBtn(cButton* pBtn)				{ m_pLeftBtn = pBtn ; }
	cButton* GetLeftBtn()						{ return m_pLeftBtn ; }

	void SetRightBtn(cButton* pBtn)				{ m_pRightBtn = pBtn ; }
	cButton* GetRightBtn()						{ return m_pRightBtn ; }

	void SetIntroduceUp(cButton* pBtn)			{ m_pIntroduce_up = pBtn ; }
	cButton* GetIntroduceUp()					{ return m_pIntroduce_up ; }

	void SetIntroduceDown(cButton* pBtn)		{ m_pIntroduce_down = pBtn ; }
	cButton* GetIntroduceDown()					{ return m_pIntroduce_down ; }

	void SetIntroduceGuage(cButton* pBtn)		{ m_pIntroduce_guage = pBtn ; }
	cButton* GetIntroduceGuage()				{ return m_pIntroduce_guage ; }

	void SetGoodIcon(int nPos, DWORD dwIconNum) ;
	void SetBadIcon(int nPos, DWORD dwIconNum) ;

	BOOL UpdateInfo(CSHResidentRegistManager::stSerchResult* pInfo) ;

	CIndexGenerator* GetIndexCreator() { return &m_IconIndexCreator ;}

	void LoadFavorItem() ;
} ;

class CDateMatchingRecordDlg : public cDialog
{
	cListCtrl*		m_pDateRecord ;

	cButton*		m_pPaperBtn ;

	cButton*		m_pList_NickName ;
	cButton*		m_pList_LastDate ;
	cButton*		m_pList_Point ;

public :
	CDateMatchingRecordDlg() ;
	virtual ~CDateMatchingRecordDlg() ;

	virtual DWORD ActionEvent(CMouse* mouseInfo) ;
	void OnActionEvent(LONG lId, void * p, DWORD we) ;

	void Linking() ;

	// link controls part.
	void SetDateRecord(cListCtrl* pList) { m_pDateRecord = pList ; }
	cListCtrl* GetRecord()				{ return m_pDateRecord ; }

	void SetPaperBtn(cButton* pBtn)		{ m_pPaperBtn = pBtn ; }
	cButton* GetPaperBtn()					{ return m_pPaperBtn ; }
} ;

// DateMatchingDlg.bin에 연결.
// 위 두 탭의 프레임으로 쓰인다.

// 070605 LYW --- DateMatchingDlg : Add enum for controls.
enum ENUM_UPPERDLG
{
	e_PartnerListDlg = 0, 
	e_ChatDlg,
} ;

enum ENUM_LOWERDLG
{
	e_MyInfoDlg = 0,
	e_PartnerInfoDlg, 
	e_DateRecordDlg,
} ;

class CDateMatchingDlg : public cDialog
{
//	CDateMatchingUpperDlg*				m_pUpperDlg;				// 윗쪽 탭 다이얼로그.
//	CDateMatchingLowerDlg*				m_pLowerDlg;				// 아래쪽 탭 다이얼로그.
//
//	BOOL								m_bActiveSearchlist;		// 서버측의 SearchList에 내가 들어가있다 flag.
//
//	DATE_MATCHING_SEARCH_OUTPUT*	m_pSearchResult;			// 서치 결과의 복사본.
//	DWORD								m_dwSearchResultCount;		// 서치 결과 갯수.
//
//
//public:
//	CDateMatchingDlg(void);
//	virtual ~CDateMatchingDlg(void);
//
//	virtual void Render();						// 디버그용.
//	//virtual void Add(cWindow * window);
//
//	void	SetUpperDlg( CDateMatchingUpperDlg* pUpper){ m_pUpperDlg = pUpper;}
//	void	SetLowerDlg( CDateMatchingLowerDlg* pLower){ m_pLowerDlg = pLower;}
//
//	void	InitMyDlg(void);
//
//	void	SetMatchingInfo( DATE_MATCHING_INFO* pInfo, DWORD dwTabIndex);
//
//
//	// 네트웍 이벤트.
//	void	NetworkMsgParse( BYTE protocol,MSGBASE* pMsg);		// 네트웍 이벤트 파서.
//	void	SetMyInfo( SEND_HERO_TOTALINFO* pTotalInfo);		// 로그인 후 나의 데이트 매칭 정보를 받았을때 세팅 한다.
//	void	SetTargetInfo( BASEOBJECT_INFO* pInfo);
//	void	OnNetDatematchingSearchAck( MSG_DATEMATCHING_SEARCH_RESULT* pPacket);		// Search list를 서버로부터 받는다.
//	void	OnNetGetTargetInfo( DATE_MATCHING_INFO* pInfo);
//
//	// UI 이벤트들.
//	void	OnProcedure( LONG lId, void* p, DWORD we );			// UI 이벤트 파서.
//	void	OnCloseBtn(void);			// 윈도 창을 닫을때.
//	void	OnSearchBtn(void);			// 상대를 '검색하기' 버튼 눌렀을 때.
//	void	OnClickChatTarget(void);	// 대화상대 검색결과에서 상대를 클릭 했을 때.
//	void	OnModifyIntroduceBtn(void);	// 자기소개 변경.
//	void	OnSaveIntroduceBtn(void);	// 자기소개 서버에 저장..
//	void	OnRegistPartnerBtn(void);		// '친구 등록'
//	void	OnRecommendPartnerBtn(void);	// '추천하기'

	CDateMatchingPartnerList*		m_pPartnerList ;
	CDateMatchingChatDlg*			m_pChatDlg ;

	CDateMatchingInfoDlg*			m_pMyInfo ;
	CDateMatchingInfoDlg*			m_pPartnerInfo ;

	CDateMatchingRecordDlg*			m_pDateRecord ;

	cPushupButton*					m_pBtnPartnerList ;
	cPushupButton*					m_pBtnChatDlg ;

	cPushupButton*					m_pBtnMyInfo ;
	cPushupButton*					m_pBtnPartnerInfo ;
	cPushupButton*					m_pBtnDateRecord ;

	// desc_hseos_데이트존_01
	// S 데이트존 추가 added by hseos 2008.01.08
	cStatic*						m_pTitle;
	char							m_szTitle[256];
	// E 데이트존 추가 added by hseos 2008.01.08

	int			m_nSelectedPatner ;

public:
	CDateMatchingDlg(void);
	virtual ~CDateMatchingDlg(void);

	void Linking() ;

	virtual DWORD ActionEvent(CMouse* mouseInfo) ;
	void OnActionEvent(LONG lId, void * p, DWORD we) ;

	// setting active dialog.
	void SetUpperDlg(int nUpperDlg) ;
	void SetLowerDlg(int nLowerDlg) ;

	// linking dialog.
	void LinkPartnerList() ;
	void LinkChatDlg() ;

	void LinkMyInfoDlg() ;
	void LinkPartnerInfo() ;
	void LinkRecordDlg() ;

	void UpdateMyInfo(DATE_MATCHING_INFO* pInfo) ;  
	//void UpdatePartnerInfo(DATE_MATCHING_INFO* pInfo) ;

	void SetPartnerListDlg(CDateMatchingPartnerList* pDlg)	{ m_pPartnerList = pDlg ; }
	CDateMatchingPartnerList* GetPartnerListDlg()			{ return m_pPartnerList ; }

	void SetChatingDlg(CDateMatchingChatDlg* pDlg) { m_pChatDlg = pDlg ; }
	CDateMatchingChatDlg* GetChatingDlg()			{ return m_pChatDlg ; }


	void SetMyInfoDlg(CDateMatchingInfoDlg* pDlg) { m_pMyInfo = pDlg ; }
	CDateMatchingInfoDlg* GetMyInfoDlg()			{ return m_pMyInfo ; }

	void SetPartnerInfoDlg(CDateMatchingInfoDlg* pDlg) { m_pPartnerInfo = pDlg ; }
	CDateMatchingInfoDlg* GetPartnerInfoDlg()			{ return m_pPartnerInfo ; }


	void SetRecordDlg(CDateMatchingRecordDlg* pDlg) { m_pDateRecord = pDlg ; }
	CDateMatchingRecordDlg* GetRecordDlg()			{ return m_pDateRecord ; }


	void SetPartnerTabBtn(cPushupButton* pBtn) { m_pBtnPartnerList = pBtn ; }
	cPushupButton* GetPartnerTabBtn()			{ return m_pBtnPartnerList ; }

	void SetChattingTabBtn(cPushupButton* pBtn) { m_pBtnChatDlg = pBtn ; }
	cPushupButton* GetChattingTabBtn()			{ return m_pBtnChatDlg ; }


	void SetMyInfoTabBtn(cPushupButton* pBtn) { m_pBtnMyInfo = pBtn ; }
	cPushupButton* GetMyInfoTabBtn()			{ return m_pBtnMyInfo ; }

	void SetPartnerInfoTabBtn(cPushupButton* pBtn) { m_pBtnPartnerInfo = pBtn ; }
	cPushupButton* GetPartnerInfoTabBtn()			{ return m_pBtnPartnerInfo ; }

	void SetRecordTabBtn(cPushupButton* pBtn) { m_pBtnDateRecord = pBtn ; }
	cPushupButton* GetRecordTabBtn()			{ return m_pBtnDateRecord ; }

	// desc_hseos_데이트존_01
	// S 데이트존 추가 added by hseos 2007.11.13 2007.11.15	2008.01.08
	void ActivateBtnEnterDateZone(BOOL bActive);
	void ActivateBtnEnterChallengeZone(BOOL bActive);
	void SetEnterChallengeZoneFreq(int nFreq);
	// E 데이트존 추가 added by hseos 2007.11.13 2007.11.15	2008.01.08
};
