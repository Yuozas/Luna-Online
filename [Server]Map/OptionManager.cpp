// OptionManager.cpp: implementation of the OptionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OptionManager.h"

#include "UserTable.h"
#include "Player.h"
#include "PackedData.h"
#include "MapDBMsgParser.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COptionManager::COptionManager()
{

}

COptionManager::~COptionManager()
{

}


void COptionManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
	case MP_OPTION_SET_SYN:
		{
			MSG_WORD* msg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(msg->dwObjectID);

			if( pPlayer == NULL ) break;	//errorÃ³¸®

			sGAMEOPTION GameOption;
			MakeGameOption( msg->wData, &GameOption );
			pPlayer->SetGameOption( &GameOption );
		}
		break;

	case MP_OPTION_SET_HIDEPARTS_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pPlayer == NULL ) break;	//errorÃ³¸®
			
			SendShowParts(pmsg->dwData, pPlayer);
		}
	}
}

void COptionManager::MakeGameOption( WORD wOption, sGAMEOPTION* pOutGameOption )
{
	//ÃÊ±âÈ­»ý·«(pOutGameOption)
	if( wOption & eOPT_NODEAL )		pOutGameOption->bNoDeal = TRUE;
	if( wOption & eOPT_NOPARTY )	pOutGameOption->bNoParty = TRUE;
//	if( wOption & eOPT_NOFRIEND )	pOutGameOption->bNoFriend = TRUE;
	if( wOption & eOPT_NOSHOWDOWN)		pOutGameOption->bNoShowdown = TRUE;
//	if( wOption & eOPT_NOWHISPER )	pOutGameOption->bNoWhisper = TRUE;
}

void COptionManager::SendShowParts(DWORD dwHidePartsFlag, CPlayer* pPlayer)
{
	if(!pPlayer)	return;
	
	MSG_OPTION_HIDEPARTS msg;
	msg.Category = MP_OPTION;
	msg.Protocol = MP_OPTION_SET_HIDEPARTS_ACK;
	msg.dwObjectID = pPlayer->GetID();
	memset(msg.bHideParts, 0, sizeof(msg.bHideParts));

	if(dwHidePartsFlag != eOPT_HIDEPARTS_NONE)
	{
		if(dwHidePartsFlag & eOPT_HIDEPARTS_EQUIP_BAND)			msg.bHideParts[eWearedItem_Band]			= 1;
		if(dwHidePartsFlag & eOPT_HIDEPARTS_EQUIP_GLASSES)		msg.bHideParts[eWearedItem_Glasses]			= 1;
		if(dwHidePartsFlag & eOPT_HIDEPARTS_EQUIP_WING)			msg.bHideParts[eWearedItem_Wing]			= 1;
		if(dwHidePartsFlag & eOPT_HIDEPARTS_COSTUME_HEAD)		msg.bHideParts[eWearedItem_Costume_Head]	= 1;
		if(dwHidePartsFlag & eOPT_HIDEPARTS_COSTUME_DRESS)		msg.bHideParts[eWearedItem_Costume_Dress]	= 1;
		if(dwHidePartsFlag & eOPT_HIDEPARTS_COSTUME_GLOVE)		msg.bHideParts[eWearedItem_Costume_Glove]	= 1;
		if(dwHidePartsFlag & eOPT_HIDEPARTS_COSTUME_SHOES)		msg.bHideParts[eWearedItem_Costume_Shoes]	= 1;
	}
	pPlayer->SetHideParts(msg.bHideParts);

	PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof( msg ) );
	
	Option_Update(pPlayer);
}
