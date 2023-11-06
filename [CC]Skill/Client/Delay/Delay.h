#pragma once

enum DELAY_TYPE
{
	DELAY_NONE,
	DELAY_TIME,
	DELAY_SCOPE,
	DELAY_MAX,
};

class cDelay
{
protected:
	BOOL			mbStart;

public:
	cDelay(void);
	virtual ~cDelay(void);

	virtual void	Start() = 0;
	virtual DWORD	Check() = 0;
	virtual void	End() = 0;

	BOOL		IsStart()
	{
		return mbStart;
	}

};
