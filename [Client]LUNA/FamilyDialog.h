// FamilyDialog.h: interface for the CFamilyDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FAMILYDIALOG_H__944612E3_C4A8_4FDD_AC8F_62047C1B202D__INCLUDED_)
#define AFX_FAMILYDIALOG_H__944612E3_C4A8_4FDD_AC8F_62047C1B202D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_MEMBER_COUNT	5																		// 최대 패밀리 구성원의 수를 5명으로 한다.

#include "../hseos/family/shfamilymanager.h"														// 패밀리 매니져 해더 파일을 불러온다.

#include "INTERFACE\cDialog.h"																		// 다이얼로그 해더 파일을 불러온다.

class cButton ;																						// 버튼 인터페이스를 선언한다.
class cComboBox;																					// 콤보박스 인터페이스를 선언한다.
class cListDialog;																					// 리스트 다이얼로그 인터페이스를 선언한다.
class cStatic;																						// 스테틱 인터페이스를 선언한다.
class cImage ;																						// 이미지 인터페이스를 선언한다.

class CFamily;																						// 패밀리 클래스를 선언한다.

//struct IDINDEX
//{
//	int nIdx ;
//	int nClassIdx ;
//} ;

class CFamilyDialog : public cDialog  
{
	cStatic*	m_pFamilyName;																		// 패밀리 이름.
	cStatic*	m_pMasterName;																		// 마스터 이름.
	cStatic*	m_pHonorPoint ;																		// 명예 포인트.

	cListDialog*	m_pListDlg;																		// 멤버 리스트 다이얼로그.

	cStatic*	m_pMemberID ;																		// 멤버 아이디.
	cStatic*	m_pMemberLevel ;																	// 멤버 레벨.
	cStatic*	m_pMemberClass ;																	// 멤버 클래스.
	cStatic*	m_pMemberLogin ;																	// 멤버 로그인.
	cStatic*	m_pMemberNickName ;																	// 멤버 호칭.
	cStatic*	m_pMemberGuildName ;																// 멤버 길드명.

	cButton*	m_pChangeNickName ;																	// 호칭변경 버튼.
	cButton*	m_pInvite ;																			// 초대신청 버튼.
	cButton*	m_pRegistMark ;																		// 문장등록 버튼.

	// desc_hseos_패밀리01
	// S 패밀리 추가 added by hseos 2007.10.22
	cButton*	m_pLeaveBtn;
	cButton*	m_pExpelBtn;
	cButton*	m_pFarmManageBtn;
	// E 패밀리 추가 added by hseos 2007.10.22

	cImage		m_pClassImg[5] ;																	// 클래스 이미지.

	cImage		m_pLongInImg[5] ;																	// 로그인 이미지.
	cImage		m_pLogOutImg[5] ;																	// 로그아웃 이미지.

	int			m_nCurSelectIdx ;																	// 선택 된 멤버 인덱스를 담는 변수.

	//cPtrList	m_ClassIconList ;																	// 클래스에 따른 아이콘 인덱스를 담는 리스트.

public:
	CFamilyDialog();																				// 생성자 함수.
	virtual ~CFamilyDialog();																		// 소멸자 함수.

	void Linking();																					// 컨트롤 들을 링크하는 함수.

	void UpdateAllInfo() ;																			// 패밀리 창에 보이는 모든 정보를 업데이트 한다.

	void SetMemberList(CSHFamily* pFamily) ;														// 멤버 리스트를 세팅한다.
	void SetMemberInfo( CSHFamilyMember* pMember) ;													// 패밀리 창 하단부, 멤버의 상세 정보를 나타내는 함수.

	virtual void Render() ;																			// 클래스 이미지와 로그인 여부를 출력하는 레던함수.

	virtual DWORD ActionEvent(CMouse* mouseInfo);													// 이벤트를 처리하는 함수.
	void OnActionEvent( LONG lId, void* p, DWORD we ) ;												// 버튼의 이벤트를 처리하는 함수.

	int GetSelectedMemberIdx() { return m_nCurSelectIdx ; }											// 현재 선택 된 멤버의 인덱스를 리턴하는 함수.

	void SetHonorPoint(DWORD dwPoint) ;																// 명예 포인트를 설정하는 함수.

	//void LoadClassIcon() ;																			// 클래스 아이콘을 로드하는 함수.
	//void ReleaseIconList() ;																		// 클래스 아이콘 리스트를 해제한다.

	int GetClassIconNum(int nClass) ;																// 클래스 아이콘을 받는다.

	// desc_hseos_패밀리01
	// S 패밀리 추가 added by hseos 2007.07.02
	//void OnActionEvent(LONG lId, void * p, DWORD we);
	// E 패밀리 추가 added by hseos 2007.07.02
};

#endif // !defined(AFX_FAMILYDIALOG_H__944612E3_C4A8_4FDD_AC8F_62047C1B202D__INCLUDED_)




//// FamilyDialog.h: interface for the CFamilyDialog class.
////
////////////////////////////////////////////////////////////////////////
//
//#if !defined(AFX_FAMILYDIALOG_H__944612E3_C4A8_4FDD_AC8F_62047C1B202D__INCLUDED_)
//#define AFX_FAMILYDIALOG_H__944612E3_C4A8_4FDD_AC8F_62047C1B202D__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
//
//#include "INTERFACE\cDialog.h"
//class cEditBox;
//class cStatic;
//class cComboBox;
//class cListDialog;
//class cPushupButton;
//class CFamily;
//class cPtrList;
//
//enum Familyfunclevel
//{
//	eFamilyMemberFunc,
//	eFamilySeniorFunc,
//	eFamilyViceMasterFunc,
//	eFamilyMasterFunc,
//	eFamilyInvailedFunc,
//	eFamilyMaxFunc,
//};
//
//class CFamilyDialog : public cDialog  
//{
//	cStatic*	m_pFamilyName;
//	cStatic*	m_pFamilyLvl;
//	cStatic*	m_pMasterName;
//	cStatic*	m_pMemberNum;
//	cStatic*	m_pLocation;
//	cStatic*	m_pUnionName;
//	cListDialog*	m_pListDlg;
//	std::vector<FAMILYMEMBERINFO>       m_MemberList;//Add 060803 by wonju
//	
//	cStatic*	m_pFamilyStatic[12];
//	cButton*	m_pFamilyBtn[13];
///*	cButton * m_pBanMemberBtn;
//	cButton * m_pGiveMemberNickBtn;
//	cButton * m_pGiveMemberRankBtn;
//	cButton * m_pInviteMemberBtn;
//	cButton * m_pSecedeBtn;
//	cButton * m_pInviteUnionBtn;
//	cButton * m_pSecedeUnionBtn;
//	cButton * m_pRemoveUnionBtn;
//	cButton * m_pDeclareBtn;
//	cButton * m_pSuggestBtn;
//	cButton * m_pSurrenderBtn;
//*/
////	cPtrList m_FamilyFuncCtrlArray[eMaxFunc];
////	cPtrList m_FamilyModeArray[eFamilyMode_Max];
//
//	int	m_CurFamilyMode;
//
//	cPushupButton*	m_pFamilyPushupBtn[2];
//	cButton*		m_pRelationBtn;
//
//	//소팅방향관련 플래그
//	int m_nPositionFlag;//Add 060803 by wonju
//	int m_nLevelFlag;//Add 060803 by wonju
//
//public:
//	CFamilyDialog();
//	virtual ~CFamilyDialog();
//	virtual void SetActive(BOOL val);
//	virtual DWORD ActionEvent(CMouse* mouseInfo);
//	void Linking();
//	void SetInfo(char* guildname, BYTE guildlvl, char* mastername, BYTE membernum, BYTE location);
//	void SetFamilyInfo(char* FamilyName, char* MasterName, char* MapName, BYTE FamilyLevel, BYTE MemberNum, char* pUnionName );
//	void SetFamilyUnionName();
//	void ResetMemberInfo(FAMILYMEMBERINFO * pInfo);
//	void DeleteMemberAll();
//	void RefreshFamilyInfo(CFamily* pFamily);
//	void ShowFamilyMode(BYTE ShowMode);
//	void SetDisableFuncBtn(BYTE Rank);
//	void SetActiveFamilyMode(int showmode,BOOL bActive);
//	void ClearDisableBtn();
//	void SetFamilyPushupBtn(BYTE ShowMode);
//	void SetFamilyPosition( char* MapName );
//
//	void SortMemberListbyPosition();//Add 060803 by wonju
//	void SortMemberListbyLevel();//Add 060803 by wonju
//};
//
//#endif // !defined(AFX_FAMILYDIALOG_H__944612E3_C4A8_4FDD_AC8F_62047C1B202D__INCLUDED_)
