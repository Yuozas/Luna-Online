#pragma once
#include "skillobject.h"

class cStatus;
// 080616 LUJ, ���� �� �̺�Ʈ ó���ϱ� ���� ����
class CEvent;

class cBuffSkillObject :
	public cSkillObject
{
	/// ���ӽð�
	cDelay*		mpDelay;
	cDelay*		mpInterval;
	cDelay*		mpUpdate;
	cDelay*		mpConsumption;

	/// ����ȿ�� ����Ʈ
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

	/// �ʱ�ȭ/����
	virtual void	Init( sSKILL_CREATE_INFO* pInfo );
	virtual void	Release();

	/// ���μ���
	virtual SKILL_STATE	Update();
	virtual void	Excute();

	void ProcessStart();
	void ProcessState();
	virtual void SetEndState();
	virtual void EndState();

	// 071206 KTH -- �����̰� �ִ°�?
	BOOL IsDelay() { return mDelayCheck; }

	void SetDelayCheck( BOOL check ) { mDelayCheck = check; }
	void SetActive( BOOL val );

	void SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin);

	void ResetDelay();

	BOOL DecreaseCount( BYTE kind );

	// 080616 LUJ, ���� �߿� �̺�Ʈ�� ó���� �� �ֵ��� �Ѵ�
	virtual void Execute( const CEvent& );
};
