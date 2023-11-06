#pragma once
#include "skillobject.h"
#include "../status/status.h"


class cBuffSkillObject :
	public cSkillObject
{
	/// 상태효과 리스트
	cPtrList	mStatusList;

	cStatus		mStatus;

public:
	cBuffSkillObject( cSkillInfo* pSkillInfo );
	virtual ~cBuffSkillObject(void);

	virtual void	Init( SKILLOBJECT_INFO* pInfo );
	virtual void	ReleaseSkillObject();
	virtual void	Release();

	virtual void	OnReceiveSkillResult( MSG_SKILL_RESULT* pmsg );

	/// 상태 효과 객체 추가
	void			AddStatusUnit( cStatus* pStatus );
	void			ResetDelay();
	void			SetCount( WORD count );

	// 080318 LUJ,	스킬이 설정된 시각 보관.	
private:
	DWORD			mBeginTime;
public:
	inline DWORD GetBeginTime() const { return mBeginTime; }
};
