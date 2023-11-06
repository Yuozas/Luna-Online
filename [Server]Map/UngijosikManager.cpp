// UngijosikManager.cpp: implementation of the CUngijosikManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UngijosikManager.h"
#include "PackedData.h"
#include "ObjectStateManager.h"
#include "Player.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUngijosikManager::CUngijosikManager()
{

}

CUngijosikManager::~CUngijosikManager()
{

}

void CUngijosikManager::UngijosikMode(CPlayer* pPlayer, BOOL bUngi)
{
	MSGBASE msg;
	msg.Category = MP_UNGIJOSIK;
	msg.dwObjectID = pPlayer->GetID();

	if(bUngi ==  TRUE)
	{
		if( OBJECTSTATEMGR_OBJ->StartObjectState(pPlayer, eObjectState_Ungijosik, 0) )
		{
			msg.Protocol = MP_UNGIJOSIK_START;		
			PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer,&msg,sizeof(msg));

		}
		else
		{
			char temp[256];
			sprintf(temp, "[%d] Á×Àº»óÅÂ¿¡¼­ ¿î±â¸¦ ½ÃÀÛÇÏ·Á°íÇÑ´Ù.", pPlayer->GetID());
			ASSERTMSG( 0, temp ); 
		}
	}
	else
	{
		OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer, eObjectState_Ungijosik, 0);

		msg.Protocol = MP_UNGIJOSIK_END;
		PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer,&msg,sizeof(msg));
	}
}