/**********************************************************************

�̸�: NewUserDialog
�ۼ�: 2007/10/25 12:14:16, �̿���

����: ���ο� ����ڸ� �˻��� �� ����

***********************************************************************/
#pragma once
#include "afxwin.h"


class CclientApp;


// CNewUserDialog ��ȭ �����Դϴ�.

class CNewUserDialog : public CDialog
{
	DECLARE_DYNAMIC(CNewUserDialog)

public:
	CNewUserDialog( CclientApp&, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CNewUserDialog();

//	virtual void OnFinalRelease();

	void Parse( DWORD serverIndex, const MSGROOT* );

	void FindPlayer( DWORD serverIndex, DWORD playerIndex );

protected:	
	CclientApp&	mApplication;
	CComboBox	mServerComboBox;
	CComboBox	mPlayerComboBox;
	CString		mKeyword;
	CButton		mIdRadioButton;
	CButton		mNameRadioButton;
	CButton		mIndexRadioButton;

	DWORD		mServerIndex;

	// Ű: �÷��̾� �ε���
	struct Player 
	{
		DWORD	mPlayerIndex;
		DWORD	mServerIndex;	// �ش� �÷��̾ ��ȸ�� ����
		CString	mName;
		DWORD	mUserIndex;
		DWORD	mStandIndex;	// 5�� ������ ĳ��
	};

	friend bool SortByStandIndex( const Player&, const Player& );
	
	// Ű: ����� �޺� �ڽ������� ����. ����Ʈ�� �÷��̾� �޺� �ڽ��� �� ��������̴�
	typedef std::list< Player >						PlayerList;
	typedef	stdext::hash_map< DWORD, PlayerList >	UserMap;
	UserMap											mUserMap;

	struct User
	{
		CString	mName;
		DWORD	mUserIndex;
	};

	// ���� �̸��� ��ȣ�� ����Ʈ�� ��� �ִ�
	typedef std::list< User >	UserList;
	UserList					mUserList;


protected:
	//CString			GetPlayerName() const;
	DWORD			GetServerIndex() const;
	//DWORD			GetPlayerIndex() const;
	const Player&	GetPlayer() const;

	enum { IDD = IDD_NEWUSERDIALOG };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	virtual BOOL	OnInitDialog();
	afx_msg void OnBnClickedNewuserSearchButton();
	afx_msg void OnMenuNewuserDialog();
	afx_msg void OnBnClickedNewuserPlayerButton();
	afx_msg void OnBnClickedNewuserItemButton();
	afx_msg void OnBnClickedNewuserSkillButton();
	afx_msg void OnBnClickedNewuserFamilyButton();
	afx_msg void OnBnClickedNewuserButtonQuest();
protected:
	//	virtual void OnOK();
//	virtual void OnCancel();
public:
	//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//afx_msg void OnBnClickedNewuserButtonQuest2();
	CStatic mPlayerSizeStatic;
	CButton mPlayerRadioButton;
	CComboBox mUserComboBox;
	CStatic mUserSizeStatic;
	afx_msg void OnCbnSelchangeNewuserUserCombo();
	afx_msg void OnBnClickedNewuserUserDataButton();
	CEdit mKeywordEdit;
};