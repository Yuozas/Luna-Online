#pragma once


#include "MunpaWareSlot.h"
#include "Purse.h"

class CGuildWarehouse : public CMunpaWareSlot  
{
	cPtrList m_WarehousePlayerList[TAB_GUILDWAREHOUSE_NUM]; //â�� ���� �ִ� �����
	ITEMBASE	m_TotalItemBase[SLOT_GUILDWAREHOUSE_NUM];
	SLOTINFO	m_TotalSlotInfo[SLOT_GUILDWAREHOUSE_NUM];
	void AddWarehousePlayer(DWORD PlayerID, BYTE TabNum);
	void RemoveWarehousePlayer(DWORD PlayerID, BYTE TabNum);
	void SendtoWarehousePlayer(BYTE TabNum, MSGBASE* msg, int size, DWORD ExceptID=0);
	
	friend class CMunpaWarePurse;
	
	CMunpaWarePurse m_GuildWarePurse;
	
public:
	CGuildWarehouse();
	virtual ~CGuildWarehouse();

	virtual ERROR_ITEM InsertItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItem, WORD state=SS_NONE);
	virtual ERROR_ITEM DeleteItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItemOut, WORD state=SS_NONE);
	virtual BOOL IsEmpty(POSTYPE absPos);
	virtual ITEMBASE const * GetItemInfoAbs(POSTYPE absPos);
	//SW050920 ���� Rare
	virtual ERROR_ITEM UpdateItemAbs(CPlayer * pPlayer, POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD flag=UB_ALL, WORD state=SS_NONE, DWORD RareDBIdx=0);

	void Init(MONEYTYPE GuildMoney);
	
	//void EnterWareHouse(CPlayer* pPlayer,BYTE TabNum);
	//void LeaveWareHouse(DWORD PlayerID,BYTE TabNum);

	BOOL InitGuildItem(ITEMBASE* pInfo);
	
//	BOOL AddGuildItem(CPlayer* pPlayer,ITEMBASE* pInfo);
//	BOOL DeleteGuildItem(CPlayer* pPlayer,POSTYPE absPos,ITEMBASE* pRtInfo);


	//void SendtoWarehousePlayerAll(MSGBASE* msg, int size);
	MONEYTYPE SetMoney( MONEYTYPE ChangeValue, BYTE bOper, BYTE MsgFlag );
	MONEYTYPE GetMaxMoney();
//	BYTE GetTabNum(POSTYPE absPos);
	
	BOOL	IsEmptyAndMoney();

	//SW060526 ����â�� ������ �������� ���� ����
	void AddItemInfoWaitingMember(CPlayer* pPlayer, BYTE TabNum);
	//void SendItemInfoToAllWaitingMembers();
};