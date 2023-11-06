// FamilyDialog.h: interface for the CFamilyDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FAMILYDIALOG_H__944612E3_C4A8_4FDD_AC8F_62047C1B202D__INCLUDED_)
#define AFX_FAMILYDIALOG_H__944612E3_C4A8_4FDD_AC8F_62047C1B202D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_MEMBER_COUNT	5																		// �ִ� �йи� �������� ���� 5������ �Ѵ�.

#include "../hseos/family/shfamilymanager.h"														// �йи� �Ŵ��� �ش� ������ �ҷ��´�.

#include "INTERFACE\cDialog.h"																		// ���̾�α� �ش� ������ �ҷ��´�.

class cButton ;																						// ��ư �������̽��� �����Ѵ�.
class cComboBox;																					// �޺��ڽ� �������̽��� �����Ѵ�.
class cListDialog;																					// ����Ʈ ���̾�α� �������̽��� �����Ѵ�.
class cStatic;																						// ����ƽ �������̽��� �����Ѵ�.
class cImage ;																						// �̹��� �������̽��� �����Ѵ�.

class CFamily;																						// �йи� Ŭ������ �����Ѵ�.

//struct IDINDEX
//{
//	int nIdx ;
//	int nClassIdx ;
//} ;

class CFamilyDialog : public cDialog  
{
	cStatic*	m_pFamilyName;																		// �йи� �̸�.
	cStatic*	m_pMasterName;																		// ������ �̸�.
	cStatic*	m_pHonorPoint ;																		// �� ����Ʈ.

	cListDialog*	m_pListDlg;																		// ��� ����Ʈ ���̾�α�.

	cStatic*	m_pMemberID ;																		// ��� ���̵�.
	cStatic*	m_pMemberLevel ;																	// ��� ����.
	cStatic*	m_pMemberClass ;																	// ��� Ŭ����.
	cStatic*	m_pMemberLogin ;																	// ��� �α���.
	cStatic*	m_pMemberNickName ;																	// ��� ȣĪ.
	cStatic*	m_pMemberGuildName ;																// ��� ����.

	cButton*	m_pChangeNickName ;																	// ȣĪ���� ��ư.
	cButton*	m_pInvite ;																			// �ʴ��û ��ư.
	cButton*	m_pRegistMark ;																		// ������ ��ư.

	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.10.22
	cButton*	m_pLeaveBtn;
	cButton*	m_pExpelBtn;
	cButton*	m_pFarmManageBtn;
	// E �йи� �߰� added by hseos 2007.10.22

	cImage		m_pClassImg[5] ;																	// Ŭ���� �̹���.

	cImage		m_pLongInImg[5] ;																	// �α��� �̹���.
	cImage		m_pLogOutImg[5] ;																	// �α׾ƿ� �̹���.

	int			m_nCurSelectIdx ;																	// ���� �� ��� �ε����� ��� ����.

	//cPtrList	m_ClassIconList ;																	// Ŭ������ ���� ������ �ε����� ��� ����Ʈ.

public:
	CFamilyDialog();																				// ������ �Լ�.
	virtual ~CFamilyDialog();																		// �Ҹ��� �Լ�.

	void Linking();																					// ��Ʈ�� ���� ��ũ�ϴ� �Լ�.

	void UpdateAllInfo() ;																			// �йи� â�� ���̴� ��� ������ ������Ʈ �Ѵ�.

	void SetMemberList(CSHFamily* pFamily) ;														// ��� ����Ʈ�� �����Ѵ�.
	void SetMemberInfo( CSHFamilyMember* pMember) ;													// �йи� â �ϴܺ�, ����� �� ������ ��Ÿ���� �Լ�.

	virtual void Render() ;																			// Ŭ���� �̹����� �α��� ���θ� ����ϴ� �����Լ�.

	virtual DWORD ActionEvent(CMouse* mouseInfo);													// �̺�Ʈ�� ó���ϴ� �Լ�.
	void OnActionEvent( LONG lId, void* p, DWORD we ) ;												// ��ư�� �̺�Ʈ�� ó���ϴ� �Լ�.

	int GetSelectedMemberIdx() { return m_nCurSelectIdx ; }											// ���� ���� �� ����� �ε����� �����ϴ� �Լ�.

	void SetHonorPoint(DWORD dwPoint) ;																// �� ����Ʈ�� �����ϴ� �Լ�.

	//void LoadClassIcon() ;																			// Ŭ���� �������� �ε��ϴ� �Լ�.
	//void ReleaseIconList() ;																		// Ŭ���� ������ ����Ʈ�� �����Ѵ�.

	int GetClassIconNum(int nClass) ;																// Ŭ���� �������� �޴´�.

	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.07.02
	//void OnActionEvent(LONG lId, void * p, DWORD we);
	// E �йи� �߰� added by hseos 2007.07.02
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
//	//���ù������ �÷���
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
