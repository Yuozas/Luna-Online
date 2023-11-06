#pragma once
#include "afxwin.h"
#include "afxcmn.h"


class CclientApp;


// CFamilyDialog 대화 상자입니다.

class CFamilyDialog : public CDialog
{
	DECLARE_DYNAMIC(CFamilyDialog)

public:
	CFamilyDialog( CclientApp&, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFamilyDialog();

	virtual void OnFinalRelease();
	void Parse( DWORD serverIndex, const MSGROOT* );

	void Request( DWORD serverIndex, const CString& playerName, DWORD playerIndex );


protected:
	CclientApp&	mApplication;
	DWORD		mServerIndex;
	DWORD		mPlayerIndex;
	DWORD		mFamilyIndex;
	DWORD		mFarmIndex;
	CString		mServerName;
	CString		mPlayerName;
	//CGridCtrl	mDataGrid;
	//CListCtrl	mMemberListCtrl;

	// 키: 콤보 박스의 아이템 순서, 값: 마스터 인덱스
	typedef std::map< DWORD, DWORD >	FamilyMasterMap;
	FamilyMasterMap						mFamilyMasterMap;


// 대화 상자 데이터입니다.
	enum { IDD = IDD_FAMILYDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	virtual BOOL OnInitDialog();
	
protected:
	CEdit mFarmStateEdit;
	CEdit mGardenGradeEdit;
	CEdit mHouseGradeEdit;
	CEdit mWarehouseGradeEdit;
	CEdit mCageGradeEdit;
	CEdit mFenceGradeEdit;
	CListCtrl mCropListCtrl;

	CEdit mNameEdit;
	CEdit mHonorEdit;
	CEdit mEnableNickEdit;
	CEdit mBuildDateEdit;
public:
	afx_msg void OnBnClickedFamilyMemberButton();
	CComboBox mServerCombo;
	CEdit mKeywordEdit;
	CButton mFindButton;
	CComboBox mResultCombo;
	afx_msg void OnBnClickedFamilyFindButton();
	afx_msg void OnBnClickedFamilySpecificDataButton();
	CStatic mResultSizeStatic;
	CButton mDeletedCheck;
	afx_msg void OnBnClickedFamilyPointLogButton();
	afx_msg void OnBnClickedFamilyFarmLogButton();
	CListCtrl mMemberListCtrl;
	afx_msg void OnBnClickedFamilyPointUpdateButton();
};
