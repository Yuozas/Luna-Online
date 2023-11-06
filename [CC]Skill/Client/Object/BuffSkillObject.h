#pragma once
#include "skillobject.h"
#include "../status/status.h"


class cBuffSkillObject :
	public cSkillObject
{
	/// ����ȿ�� ����Ʈ
	cPtrList	mStatusList;

	cStatus		mStatus;

public:
	cBuffSkillObject( cSkillInfo* pSkillInfo );
	virtual ~cBuffSkillObject(void);

	virtual void	Init( SKILLOBJECT_INFO* pInfo );
	virtual void	ReleaseSkillObject();
	virtual void	Release();

	virtual void	OnReceiveSkillResult( MSG_SKILL_RESULT* pmsg );

	/// ���� ȿ�� ��ü �߰�
	void			AddStatusUnit( cStatus* pStatus );
	void			ResetDelay();
	void			SetCount( WORD count );

	// 080318 LUJ,	��ų�� ������ �ð� ����.	
private:
	DWORD			mBeginTime;
public:
	inline DWORD GetBeginTime() const { return mBeginTime; }
};
