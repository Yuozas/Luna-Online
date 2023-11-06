#pragma once
#include "afxwin.h"
#include "afxcmn.h"


class CclientApp;


// CQuestDialog ��ȭ �����Դϴ�.

class CQuestDialog : public CDialog
{
	DECLARE_DYNAMIC(CQuestDialog)

public:
	CQuestDialog( CclientApp&, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CQuestDialog();

	void Parse( DWORD serverIndex, const MSGROOT* );
	virtual void OnFinalRelease();

	void Request( DWORD serverIndex, const CString& playerName, DWORD playerIndex );

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_QUESTDIALOG };

protected:
	void RefreshSubQuest();
	void RefreshItem();

protected:
	CString GetTimeText( DWORD value ) const;
	BOOL	IsEndSubQuest( DWORD subQuestIndex, DWORD state ) const;


protected:
	CclientApp&	mApplication;
	DWORD		mServerIndex;
	DWORD		mPlayerIndex;
	CString		mPlayerName;

	//CListCtrl	mMainListCtrl;
	CListCtrl	mSubListCtrl;
	CListCtrl	mItemListCtrl;
	CListCtrl	mMonsterListCtrl;
	//CGridCtrl	mDataGrid;
	CStatic		mDescrptionStatic;

	// ��������Ʈ ����. Ű: ���� ����Ʈ �ε���
	typedef std::map< DWORD, QMBASE >	MainQuestMap;
	MainQuestMap						mMainQuestMap;

	// Ű: ��������Ʈ �ε���
	typedef std::map< DWORD, QSUBASE >	SubQuest;

	// ���� ����Ʈ ����, Ű: ���� ����Ʈ �ε���
	typedef std::map< DWORD, SubQuest >	SubQuestMap;
	SubQuestMap							mSubQuestMap;

	// ����Ʈ ������ ����, Ű: ����Ʈ ������ �ε���
	typedef std::map< DWORD, QITEMBASE >	ItemMap;
	ItemMap									mItemMap;

	// �ߺ� ����Ʈ üũ. Ű: ����Ʈ �ε���, ��: ����
	typedef std::map< DWORD, DWORD >	CheckMap;
	CheckMap							mCheckMap;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedQuestMainList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedQuestSubList(NMHDR *pNMHDR, LRESULT *pResult);
	CButton mFinishSubQuestButton;
	afx_msg void OnBnClickedQuestSelectedOneFinishButton();
	afx_msg void OnBnClickedQuestMainDeleteButton();
	CListCtrl mMainListCtrl;
	CListCtrl mTotalItemListCtrl;
	afx_msg void OnLvnItemchangedQuestItemTotalList(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic mItemDescriptionStatic;
	//CComboBox mItemSizeCombo;
	CButton mItemUpdateButton;
	afx_msg void OnBnClickedQuestItemUpdateButton();
	//afx_msg void OnBnClickedQuestItemUpdateButton2();
	CEdit mItemSizeEdit;
	afx_msg void OnBnClickedQuestItemDeleteButton();
	afx_msg void OnBnClickedQuestLogButton();
	CStatic mReportStatic;
	afx_msg void OnBnClickedQuestItemAddButton();
};
