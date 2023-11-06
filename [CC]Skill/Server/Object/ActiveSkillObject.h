#pragma once
#include "skillobject.h"

class cBuffSkillObject;
class cDelay;
class cBuffSkillInfo;

class cActiveSkillObject :
	public cSkillObject
{
protected:
	/// ���� ��ų ���̺�
	CYHHashTable<cBuffSkillObject>	m_BuffSkillTable;

	/// ������
	cDelay*				mpDelay[ DELAY_KIND_MAX ];

	/// ��ų ȿ�� ��ü
	cSkillUnit*			mpSkillUnit;

	sSKILL_CREATE_INFO	mBuffSkillObjectInfo[MAX_BUFF_COUNT];
	cBuffSkillInfo*		mpBuffSkillInfo[MAX_BUFF_COUNT];
	WORD				mBuffRate[MAX_BUFF_COUNT];

	float				mAttackSpeedRate;

public:
	/// ������ / �Ҹ���
	cActiveSkillObject( cSkillInfo* pSkillInfo, cTarget* pTarget );
	virtual ~cActiveSkillObject( void );

	/// �ʱ�ȭ/����
	virtual void	Init( sSKILL_CREATE_INFO* pInfo );
	virtual void	Release();

	/// ��ų ȿ�� ��ü �߰�
	void			AddSkillUnit( cSkillUnit* pSkillUnit );

	/// ���μ���
	virtual SKILL_STATE	Update();
	virtual void	Excute();

	void CastingStart();
	void CastingState();
	void ProcessStart();
	void ProcessState();

	/// ������ų ����
	virtual void AddBuffSkillObject( cBuffSkillObject* pBuff );
	virtual void RemoveBuffSkillObject ( cBuffSkillObject* pBuff );

	// 080602 LUJ, �ش� ������Ʈ�� �ߵ��� ���� ��ų�� ��� �����Ѵ�
	virtual void RemoveAllBuffSkillObject();

	/// Grid �ý��ۿ� �߰�/����
	virtual void		SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin);
	virtual void		SetRemoveMsg(char* pRemoveMsg,WORD* pMsgLen,DWORD dwReceiverID);

};
