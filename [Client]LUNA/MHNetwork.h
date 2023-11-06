// MHNetwork.h: interface for the CMHNetwork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MHNETWORK_H__E93C75BD_70FA_4330_B63C_2B5A6A57132A__INCLUDED_)
#define AFX_MHNETWORK_H__E93C75BD_70FA_4330_B63C_2B5A6A57132A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "basenetworkinterface.h"
#include "Crypt.h"
class CGameState;

#define NETWORK USINGTON(CMHNetwork)

class CMHNetwork  
{
public:
	BOOL m_bDisconnecting;
private:
	HWND m_hMainWnd;
	CGameState* m_pCurState;

	ISC_BaseNetwork* m_pBaseNetwork;
	DWORD m_ConnectionIndex;
	
	BYTE m_CheckSum;
	CCrypt m_crypt;

	DWORD m_RecvedDataSize;		// �޼����� �󸶳� �޴��� �½�Ʈ �ϱ� ���ؼ�
	DWORD m_RecvCount;

#ifdef _DEBUG
	DWORD m_MsgIn[MP_MAX];
	DWORD m_MsgOut[MP_MAX];
	void AddMsgIn(BYTE Category,DWORD length)	{	m_MsgIn[Category] += length;	}
	void AddMsgOut(BYTE Category,DWORD length)	{	m_MsgOut[Category] += length;	}
public:
	DWORD GetMsgIn(BYTE Category)	{	return m_MsgIn[Category];	}
	DWORD GetMsgOut(BYTE Category)	{	return m_MsgOut[Category];	}
	DWORD GetTotalMsgIn()
	{
		DWORD total = 0;
		for(int n=0;n<MP_MAX;++n)
			total += m_MsgIn[n];
		return total;
	}
	DWORD GetTotalMsgOut()
	{
		DWORD total = 0;
		for(int n=0;n<MP_MAX;++n)
			total += m_MsgIn[n];
		return total;
	}
#endif

public:
//	//MAKESINGLETON(CMHNetwork);

	CMHNetwork();
	virtual ~CMHNetwork();
	
	BOOL Init(HWND hWnd);
	void Release();

	BOOL ConnectToServer(char* ip,WORD port);
	void Disconnect();

	void SetCurState(CGameState* pState);
	
	void Send(MSGROOT* pMsg,int MsgLen);

	BOOL IsConnected()	{	return m_ConnectionIndex ? TRUE : FALSE;	}
	
	void SetCheckSum(BYTE CS)	{	m_CheckSum = CS;	}
	void SetKey( HselInit eninit, HselInit deinit ) {	m_crypt.Init(eninit, deinit); }
	void ReleaseKey()	{ m_crypt.Release(); }


	ISC_BaseNetwork* GetBaseNetwork()							{ return m_pBaseNetwork; }
	friend void OnCon(DWORD dwConIndex);
	friend void OnDisCon(DWORD dwConIndex);
	friend void OnRecv(DWORD dwConIndex,char* pMsg,DWORD msglen);
};

EXTERNGLOBALTON(CMHNetwork)
#endif // !defined(AFX_MHNETWORK_H__E93C75BD_70FA_4330_B63C_2B5A6A57132A__INCLUDED_)
