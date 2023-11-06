#pragma once
#include "delay.h"

class cTimeDelay :
	public cDelay
{
	DWORD			mStart;
	DWORD			mDelay;
    DWORD			mRemain;
	float			mRate;

public:
	cTimeDelay(void);
	virtual ~cTimeDelay(void);

	void			Init( DWORD time );
	virtual void	Start();
	virtual DWORD	Check();
	virtual void	End();

	DWORD GetStartTime()
	{
		return mStart;
	}

	DWORD GetDelayTime()
	{
		return mDelay;
	}

	DWORD GetRemainTime()
	{
		return mRemain;
	}

	float GetRemainRate()
	{
		return mRate;
	}
};
