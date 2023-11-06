// PartyDialog.h: interface for the CPartyDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTYDIALOG_H__B103D700_3CF5_4F20_8828_16E4212A76C2__INCLUDED_)
#define AFX_PARTYDIALOG_H__B103D700_3CF5_4F20_8828_16E4212A76C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPartyMemberDlg;
class CPartyBtnDlg;


class CPartyDialog 
{
	CPartyMemberDlg * m_MemberDlg[MAX_PARTY_LISTNUM-1];
	CPartyBtnDlg * m_PartyBtnDlg;
	BOOL m_bActive;
	
	DWORD m_ClickedMemberID;
	
	BOOL	m_bShowMember;
	BOOL	m_bOption;

public:
	CPartyDialog();
	virtual ~CPartyDialog();

	void RegistMemberDlg(int i, CPartyMemberDlg* pDlg);
	void RegistBtnDlg(CPartyBtnDlg* pDlg);

	BOOL IsActive();
	void SetActive(BOOL bState);
	void RefreshDlg();
		
	void MemberBtnPushUp(DWORD id);

	void SetClickedMemberID(DWORD id);
	DWORD GetClickedMemberID();
	
	void Init();
	
	void	ShowMember();
	void	ShowOption();

	void	SetOptionActive( BOOL bFlag );
	void	SetMemberActive( BOOL bFlag );
	BOOL	IsOption()	{ return m_bOption; }
	BOOL	IsMember()	{ return m_bShowMember; }

	// 070420 LYW --- PartyDialog : Add function to return member dialog.
	CPartyMemberDlg* GetMemberDlg( BYTE index ) ; 

	// 080318 LUJ, ��Ƽ�� ���� ��ų ǥ�� �߰�/����
	void AddSkill( const SKILLOBJECT_INFO& );
	void RemoveSkill( const SKILLOBJECT_INFO& );
	void RemoveAllSkill( DWORD playerIndex );
	// 080613 LUJ, ��Ƽ ���� ǥ�� ��ġ�� �ٽ� �����Ѵ�. ���� �������� ���� ���� ó�� �ð��� �����ǹǷ�, ���μ��� �ܿ����� ȣ������ �ʵ��� ����.
	void RelocateSkill();

	// 080318 LUJ, ��Ƽ���� ���� Ŭ���̾�Ʈ�� ���� ������ �ʱ�ȭ�Ѵ�
	void InitializeSkill( const PARTY_MEMBER& );

	// 080318 LUJ, ��Ƽ���� ���� ��ų ǥ�ø� ����Ѵ�
	virtual void OnActionEvent( LONG lId, void* p, DWORD we );
};

#endif // !defined(AFX_PARTYDIALOG_H__B103D700_3CF5_4F20_8828_16E4212A76C2__INCLUDED_)
