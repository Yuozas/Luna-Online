#pragma once
#include "afxwin.h"
#include "afxcmn.h"


class CclientApp;
class CSkillReportDialog;


// CSkillDialog ��ȭ �����Դϴ�.

class CSkillDialog : public CDialog
{
	friend class CSkillReportDialog;

	DECLARE_DYNAMIC(CSkillDialog)

public:
	CSkillDialog( CclientApp&, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSkillDialog();

	void Parse( DWORD serverIndex, const MSGROOT* );

	virtual void OnFinalRelease();

	void Request( DWORD serverIndex, const CString& playerName, DWORD playerIndex );


protected:
	void PutSkill();
	
protected:
	DWORD		mServerIndex;
	DWORD		mPlayerIndex;
	CString		mPlayerName;
	//CListCtrl	mMyListCtrl;
	CclientApp&	mApplication;
	CComboBox mLevelCombo;
	CButton mUpdateButton;
	CStatic mEnableSkillStatic;
	CStatic mMySkillStatic;
	CListCtrl mEnableListCtrl;

	// Ű: ��ų �ε���
	typedef	stdext::hash_map< DWORD, SKILL_BASE >	SkillMap;
	SkillMap										mSkillMap;


// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SKILLDIALOG };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSkillUpdateButton();
	afx_msg void OnLvnItemchangedSkillDialogList(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnBnClickedSkillRemoveButton();
	afx_msg void OnBnClickedSkillAddButton();
	afx_msg void OnLvnItemchangedSkillEnableList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedSkillMyList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSkillLogButton();
	CListCtrl mMyListCtrl;
	CButton mReportButton;
	afx_msg void OnBnClickedSkillReportButton();
};