#pragma once
#include "afxwin.h"
#include "afxcmn.h"


class CclientApp;


// CFamilyDialog ��ȭ �����Դϴ�.

class CFamilyDialog : public CDialog
{
	DECLARE_DYNAMIC(CFamilyDialog)

public:
	CFamilyDialog( CclientApp&, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
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

	// Ű: �޺� �ڽ��� ������ ����, ��: ������ �ε���
	typedef std::map< DWORD, DWORD >	FamilyMasterMap;
	FamilyMasterMap						mFamilyMasterMap;


// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_FAMILYDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
