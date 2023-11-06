#pragma once
#include "skillobject.h"

class cStatus;
// 080616 LUJ, 버프 중 이벤트 처리하기 위해 참조
class CEvent;

class cBuffSkillObject :
	public cSkillObject
{
	/// 지속시간
	cDelay*		mpDelay;
	cDelay*		mpInterval;
	cDelay*		mpUpdate;
	cDelay*		mpConsumption;

	/// 상태효과 리스트
	cPtrList	mStatusList;
/*
	eStatusKind mStatusKind;
	StatusData	mStatusData;
*/
	WORD		mStatusKind;
	float		mStatusData;
	WORD		mStatusDataType;

	Status*		mpTargetStatus;
	
	BOOL		mDelayCheck;
	BOOL		mActive;
	BOOL		mDestroy;

	BOOL		mIsUpdate;

	WORD		mCount;
	BYTE		mCountType;

	void		Consumption();

public:
	cBuffSkillObject( cSkillInfo* pSkillInfo, cTarget* pTarget );
	virtual ~cBuffSkillObject(void);

	/// 초기화/해제
	virtual void	Init( sSKILL_CREATE_INFO* pInfo );
	virtual void	Release();

	/// 프로세스
	virtual SKILL_STATE	Update();
	virtual void	Excute();

	void ProcessStart();
	void ProcessState();
	virtual void SetEndState();
	virtual void EndState();

	// 071206 KTH -- 딜레이가 있는가?
	BOOL IsDelay() { return mDelayCheck; }

	void SetDelayCheck( BOOL check ) { mDelayCheck = check; }
	void SetActive( BOOL val );

	void SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin);

	void ResetDelay();

	BOOL DecreaseCount( BYTE kind );

	// 080616 LUJ, 버프 중에 이벤트를 처리할 수 있도록 한다
	virtual void Execute( const CEvent& );
};
