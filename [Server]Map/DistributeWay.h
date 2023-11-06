	// DistributeWay.h: interface for the CDistributeWay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISTRIBUTEWAY_H__7C8B1F62_199B_4707_B7C9_2BAFC4A54A20__INCLUDED_)
#define AFX_DISTRIBUTEWAY_H__7C8B1F62_199B_4707_B7C9_2BAFC4A54A20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 070314 LYW --- DistributeWay : Include header file.
#include "stdafx.h"

class MONSTEREX_LIST;

//060822 - 3만 이상의 난수를 뽑아내기 위한 랜덤 클래스 by wonju
//----------------------------------------------------------------
class cRand_Big
{
private:
	int mRangeVal;
public:

	DWORD RandomVal();

	cRand_Big(){mRangeVal = 0;}
	~cRand_Big(){}
};
//-----------------------------------------------------------------

class CDistributeWay  // responsible for how to share compensation
{
protected:

	cRand_Big mBigRandObj;

	BOOL ChkGetItemLevel(LEVELTYPE PlayerLvl, LEVELTYPE MonsterLvl);
	
public:
	void ItemChangeAtLv(DWORD& DropItemId);        //060705 Add by wonju
	

public:
	CDistributeWay();
	virtual ~CDistributeWay();

	/*어빌리티 삭제 - virtual void CalcAbilandSend(LEVELTYPE MonsterLevel, PARTY_RECEIVE_MEMBER* pMemberInfo, LEVELTYPE MaxLevel);*/
	virtual void SendItem(PARTY_RECEIVE_MEMBER* pRealMember, DWORD DropItemId, DWORD DropItemRatio, MONSTEREX_LIST * pMonInfo, WORD MonsterKind, LEVELTYPE MaxLevel) {};

	// 080530 LYW --- DistributeWay : Modified data type DWORD to DWORDEX(EXPTYPE).
	//void SendToPersonalExp(CPlayer* pReceivePlayer, DWORD Exp);
	void SendToPersonalExp(CPlayer* pReceivePlayer, EXPTYPE Exp) ;
	/*어빌리티 삭제 - void SendToPersonalAbil(CPlayer* pReceivePlayer, LEVELTYPE MonsterLevel);*/
	void SendToPersonalMoney(CPlayer* pPlayer, MONEYTYPE Money, WORD MonsterKind);
	void SendToPersonalItem(CPlayer* pPlayer, DWORD DropItemId, DWORD DropItemRatio, MONSTEREX_LIST * pMonInfo, WORD MonsterKind);	
	// 070419 LYW --- DistributeWay : Add funtion to send acquisition to player.
	void SendAcquisition(CPlayer* pPlayer, DWORD DropItemId, DWORD DropItemRatio, MONSTEREX_LIST * pMonInfo, WORD MonsterKind, VECTOR3* pKilledPosition) ;
	
	/*어빌리티 삭제 - DWORD CalcObtainAbilityExp(LEVELTYPE KillerLevel,LEVELTYPE MonsterLevel);*/
};

#endif // !defined(AFX_DISTRIBUTEWAY_H__7C8B1F62_199B_4707_B7C9_2BAFC4A54A20__INCLUDED_)
