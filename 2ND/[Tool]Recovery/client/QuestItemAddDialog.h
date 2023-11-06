#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// QuestItemAddDialog ��ȭ �����Դϴ�.

class CQuestItemAddDialog : public CDialog
{
	DECLARE_DYNAMIC(CQuestItemAddDialog)

public:
	CQuestItemAddDialog( CclientApp&, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CQuestItemAddDialog();

	virtual void OnFinalRelease();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_QUESTITEMADDDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg void OnEnChangeQuestItemAddEdit();
	afx_msg void OnLvnItemchangedQuestItemFindedQuestList(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl mQuestListCtrl;
	CListCtrl mitemListCtrl;
	
protected:
	virtual void OnOK();

	CclientApp& mApplication;
	
	DWORD	mQuestItemIndex;

public:
	virtual BOOL OnInitDialog();

	DWORD GetQuestItemIndex() const;
	CEdit mKeywordEdit;
	CStatic mQuestDescriptionStatic;
	CStatic mItemDescriptionStatic;
	afx_msg void OnLvnItemchangedQuestItemFindedItemList(NMHDR *pNMHDR, LRESULT *pResult);
	CButton mQuestRadio;
	CButton mItemRadio;
};
