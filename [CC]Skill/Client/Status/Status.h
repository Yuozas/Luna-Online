#pragma once

enum STATUSKIND
{
	STATUSKIND_POISON = 19,
	STATUSKIND_STUN = 25,
	STATUSKIND_TIEUP = 41,
};

class cStatus
{
	WORD	mKind;
	
	DWORD	mData1;
	DWORD	mData2;
	DWORD	mData3;

public:
	cStatus(void);
	virtual ~cStatus(void);

	void	Init( WORD Kind, DWORD Data1, DWORD Data2, DWORD Data3 )
	{
		mKind = Kind;
		mData1 = Data1;
		mData2 = Data2;
		mData3 = Data3;
	}

	WORD	GetKind() { return mKind; }

	DWORD	GetData1() { return mData1; }
	DWORD	GetData2() { return mData2; }
	DWORD	GetData3() { return mData3; }
};
