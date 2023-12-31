// HackCheck.h: interface for the CHackCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HACKCHECK_H__49EB8762_D978_4265_99FC_BD6B45DDF9B9__INCLUDED_)
#define AFX_HACKCHECK_H__49EB8762_D978_4265_99FC_BD6B45DDF9B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define HACKCHECK USINGTON(CHackCheck)


struct sCHECKTIME
{
	DWORD dwArrivedTime;	//클라이언트 도착 시간
	DWORD dwServerSendTime;		//서버에서 보낸 시간
};

class CHackCheck  
{

protected:

	cPtrList	m_ListSpeedHackCheck;
	DWORD		m_dwLastTime;
	
	BOOL		m_bIsHackUser;

	struct	ScriptHackCheck 
	{
		// 090109 LUJ, 스크립트를 체크한 고유값
		ScriptCheckValue	mCheckValue;
		// 090109 LUJ, 스크립트 해킹 체크하는 시간 간격
		DWORD				mCheckStepTick;
		// 090115 LUJ, 마지막 체크한 시간
		DWORD				mNextCheckTick;
	}
	mScriptHackCheck;

public:
	CHackCheck();
	virtual ~CHackCheck();
	
	void Release();
	void Process();
	void NetworkMsgParse( BYTE Protocol, void* pMsg );

	BOOL IsHackUser() { return m_bIsHackUser; }
};

EXTERNGLOBALTON(CHackCheck)

#endif // !defined(AFX_HACKCHECK_H__49EB8762_D978_4265_99FC_BD6B45DDF9B9__INCLUDED_)
