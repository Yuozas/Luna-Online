// MapObject.cpp: implementation of the CMapObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MapObject.h"
#include "SiegeWarMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapObject::CMapObject()
{

}

CMapObject::~CMapObject()
{

}


void CMapObject::InitMapObject( MAPOBJECT_INFO* pInitInfo )
{
	memcpy( &m_MapObjectInfo, pInitInfo, sizeof(MAPOBJECT_INFO) );
	m_Level = 0;
}


DWORD CMapObject::GetLife()
{
	return m_MapObjectInfo.Life;
}

void CMapObject::SetLife(DWORD Life,BOOL bSendMsg)
{
	m_MapObjectInfo.Life = Life;
}

DWORD CMapObject::DoGetMaxLife()
{ 
	return m_MapObjectInfo.MaxLife; 
}

DWORD CMapObject::DoGetPhyDefense()
{ 
	return m_MapObjectInfo.PhyDefence;
}

float CMapObject::DoGetAttDefense(WORD Attrib)
{ 
	return m_MapObjectInfo.AttrRegist.GetElement_Val( Attrib ); 
}	


void CMapObject::DoDie( CObject* pAttacker )
{
	switch( GetObjectKind() )
	{
	case eObjectKind_CastleGate:
		SIEGEWARMGR->DeleteCastleGate( GetID() );
		break;
	}
}


void CMapObject::SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin)
{
	switch( GetObjectKind() )
	{
	case eObjectKind_CastleGate:
		SetCastlegateAddMsg( pAddMsg, pMsgLen, dwReceiverID, bLogin );
		break;
	}
}


void CMapObject::SetRemoveMsg(char* pRemoveMsg,WORD* pMsgLen,DWORD dwReceiverID)
{
}


void CMapObject::SetCastlegateAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin)
{
}