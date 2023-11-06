// PartyMemberDlg.h: interface for the CPartyMemberDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTYMEMBERDLG_H__7426A94E_644C_44BE_80FE_877AE4A137B7__INCLUDED_)
#define AFX_PARTYMEMBERDLG_H__7426A94E_644C_44BE_80FE_877AE4A137B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"

#define PARTY_MEMBER_RACE_HUMAN			0
#define PARTY_MEMBER_RACE_ELF			1

#define PARTY_MEMBER_GENDER_MAN			0
#define PARTY_MEMBER_GENDER_WOMAN		1

#define PARTY_MEMBER_FACE_HUMAN_MAN		98
#define PARTY_MEMBER_FACE_HUMAN_WOMAN	99

#define PARTY_MEMBER_FACE_ELF_MAN		100
#define PARTY_MEMBER_FACE_ELF_WOMAN		101

// 071004 LYW --- GuageDialog : Define master image num
#define MASTER_MARK_NUM		118

class CObjectGuagen;
class cPushupButton;
class cStatic;
class CPartyBtnDlg;

// 080318 LUJ, ��Ƽ�� ���� ��ų ǥ������ Ŭ���� ����
class CStatusIconDlg;
class cCheckBox;

class CPartyMemberDlg  : public cDialog
{
	cPushupButton * m_pName;
	CObjectGuagen * m_pLife;
	CObjectGuagen * m_pMana;
	cStatic		  * m_pLevel;

	// 070419 LYW --- PartyMember : Add member static for character image.
	//cStatic		  * m_pFace[4] ;

	CPartyBtnDlg*	m_pPartyBtnDlg;

	BOOL			m_bRealActive;
	int				m_nIndex;
	BOOL			m_bOption;
	BOOL			m_bMember;

	// 070622 LYW --- PartyMemberDlg : Modified face part.
	cImage			m_pFace ;
	BOOL			m_bShowFace ;

	// 071003 LYW --- PartyMemberDlg : Add master mark image part.
	BOOL			m_bRenderMasterMark ;
	cImage			m_MasterMark ;

	//// 071003 LYW --- PartyMemberDlg : Add class mark image part.
	//BOOL			m_bRenderClassMark ;
	//cImage			m_ClassMark ;

public:

	DWORD m_MemberID;
	
	CPartyMemberDlg();
	virtual ~CPartyMemberDlg();
	
	void SetActive(BOOL val);
	void Linking(int i);
	void SetMemberData(PARTY_MEMBER* pInfo);

	void SetNameBtnPushUp(BOOL val);
	void SetPartyBtnDlg( CPartyBtnDlg* pDlg )	{ m_pPartyBtnDlg = pDlg; }
	
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual void Render();

	void	ShowOption( BOOL bOption )			{ m_bOption = bOption; }	
	void	ShowMember( BOOL bMember )			{ m_bMember = bMember; }

	// 070420 LYW --- PartyMemberDlg : Add function to setting member face.
	void SetFace(BYTE racial, BYTE gender) ;

	// 071003 LYW --- PartyMemberDlg : Add function to setting render master mark or not.
	void SetRenderMasterMark(BOOL bRender)	{ m_bRenderMasterMark = bRender ; }

	// 071003 LYW --- PartyMemberDlg : Add function to setting render class mark or not.
	//void SetRenderClassMark(BOOL bRender) { m_bRenderClassMark  = bRender ; }	

	// 080318 LUJ, ��Ƽâ�� ǥ���ϰ� �ִ� �÷��̾� ��ȣ�� ��ȯ�Ѵ�
	inline DWORD GetPlayerIndex() const { return m_MemberID; }

	// 080318 LUJ,	���� ������ ǥ�� ��ġ ���� ���� �������̵�.
	//				����: SetAbsX(), SetAbsY() ���� �Լ��� �ִµ� ������� �ʾ� �������̵����� �ʾҴ�
	virtual void SetAbsXY( LONG x, LONG y);

	// 080318 LUJ, ���� ��ų�� ǥ�� ����
	void SetVisibleSkill( BOOL );
	BOOL IsVisibleSkill() const;

	// 080318 LUJ, ��Ƽ�� ���� ��ų ǥ�� ����
private:	
	CStatusIconDlg* mStatusIconDialog;
public:
	inline CStatusIconDlg* GetStatusIconDialog() const { return mStatusIconDialog; }

	// 080318 LUJ, ���� ��ų ��� ���� ��Ʈ��
private:	
	cCheckBox* mCheckBox;
};

#endif // !defined(AFX_PARTYMEMBERDLG_H__7426A94E_644C_44BE_80FE_877AE4A137B7__INCLUDED_)
