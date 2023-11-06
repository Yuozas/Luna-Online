#pragma once
#include ".\interface\cdialog.h"

class cListDialog;
class cStatic;

struct SkillData
{
	DWORD	index;
	BYTE	level;
};

class cSkillTrainingDlg :
	public cDialog
{
	cListDialog*	m_SkillListDlg;
	cListDialog*	m_SkillTipListDlg;
	
	cStatic*		m_SkillPoint;
	cStatic*		m_Gold;

	cStatic*		m_Icon;
	cStatic*		m_level;

	cStatic*		m_NeedSp;
	cStatic*		m_NeedGold;
	cStatic*		m_NeedMana;

	SkillData		m_SkillList[ 100 ];

	int				m_Index;

	// 080417 LUJ, ��� ��ų ǥ�� �������� ��Ÿ���� �÷���
	BOOL			mGuildMode;

public:
	cSkillTrainingDlg(void);
	virtual ~cSkillTrainingDlg(void);

	void Linking();

	// 080618 LYW : ��Ȳ�� �°� �μ� â�� ǥ���ϴ� ����� �ϴ� �Լ� �߰�.
	void DisplayGuildSkill() ;

	// 080618 LYW : HERO�� ���� ���� �� �߿� �ش��ϴ� ��ų�� �ִ��� Ȯ���ϰ�, �ִٸ�,
	//			  : �� ��ų�� ���� ������ �ִ� ������ ��ȯ�ϴ� �Լ� �߰�.
	int GetSkillLevelFromJobTree(DWORD dwSkillIdx, WORD wCurJobLevel) ;

	// 080618 LYW : ������ ���� ��ų���� / ��ų�� / ���� / �������� �����ϴ� �Լ� �߰�.
	void AddSkillToSkillListDlg(BYTE byJobType, WORD wJobLevel, WORD* pPos) ;

	// ��ų ���� â�� ���� �Լ�.
	void OpenDialog() ;

	// 080417 LUJ, ��� ��ų ǥ��
	void OpenGuildSkill();
	// 080417 LUJ, ��� ��ų �н� â�̸� ���� ��ȯ�Ѵ�
	BOOL IsGuildMode() const { return mGuildMode; }

	void OnActionEvent( LONG lId, void* p, DWORD we );

	void SetSkillInfo();
	void SetGold();
	void SetSkillPoint();

	void Training();
};
