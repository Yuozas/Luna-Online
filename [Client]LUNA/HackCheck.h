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
	DWORD dwArrivedTime;	//Ŭ���̾�Ʈ ���� �ð�
	DWORD dwServerSendTime;		//�������� ���� �ð�
};

class CHackCheck  
{

protected:

	cPtrList	m_ListSpeedHackCheck;
	DWORD		m_dwLastTime;
	
	BOOL		m_bIsHackUser;

	struct	ScriptHackCheck 
	{
		// 090109 LUJ, ��ũ��Ʈ�� üũ�� ������
		ScriptCheckValue	mCheckValue;
		// 090109 LUJ, ��ũ��Ʈ ��ŷ üũ�ϴ� �ð� ����
		DWORD				mCheckStepTick;
		// 090115 LUJ, ������ üũ�� �ð�
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
