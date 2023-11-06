#include "StdAfx.h"
#include "HackShieldManager.h"
#include "Network.h"

#ifdef _HACK_SHIELD_
GLOBALTON(CHackShieldManager);
#endif

CHackShieldManager::CHackShieldManager(void)
{
}

CHackShieldManager::~CHackShieldManager(void)
{
}

void CHackShieldManager::NetworkMsgParse(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
#ifdef _HACK_SHIELD_	
	USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if( pUserInfo == NULL ) return;

	DWORD dwRet = ERROR_SUCCESS;

	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_HACKSHIELD_GUID_ACK:
		{
			MSG_GUID_ACK* pmsg = (MSG_GUID_ACK*)pMsg;
			
			dwRet = _AntiCpSvr_AnalyzeGuidAckMsg( pmsg->msg, pUserInfo->m_GUIDInfo, &(pUserInfo->m_pCrcInfo));
			pUserInfo->m_bHSCheck = FALSE;

			if(dwRet != ERROR_SUCCESS )
			{
				MSGBASE msg;
				msg.Category = MP_HACKSHIELD;
				msg.Protocol = MP_HACKSHIELD_DISCONNECT;
				g_Network.Send2User(pUserInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
				DisconnectUser(dwConnectionIndex);
			}
			else
			{
				MSG_REQ msg;
				memset(&(msg.msg), 0, SIZEOF_REQMSG);

				msg.Category = MP_HACKSHIELD;
				msg.Protocol = MP_HACKSHIELD_REQ;

				DWORD dwRet = ERROR_SUCCESS;

				dwRet = _AntiCpSvr_MakeReqMsg( pUserInfo->m_pCrcInfo, msg.msg, pUserInfo->m_Info, ANTICPSVR_CHECK_ALL);

				if ( dwRet != ERROR_SUCCESS )
				{

				}
				else
				{
					g_Network.Send2User(dwConnectionIndex,(char*)&msg,sizeof(msg));
					pUserInfo->m_bHSCheck = 2;
				}
			}
		}
		break;
	case MP_HACKSHIELD_ACK:
		{
			MSG_ACK* pmsg = (MSG_ACK*)pMsg;

			dwRet = _AntiCpSvr_AnalyzeAckMsg( pUserInfo->m_pCrcInfo, pmsg->msg, pUserInfo->m_Info);
			pUserInfo->m_bHSCheck = FALSE;

			if(dwRet != ERROR_SUCCESS )
			{
				MSGBASE msg;
				msg.Category = MP_HACKSHIELD;
				msg.Protocol = MP_HACKSHIELD_DISCONNECT;
				g_Network.Send2User(pUserInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
				DisconnectUser(dwConnectionIndex);
			}
		}
		break;
	default:
		{
		}
		break;
	}
#endif
}

// Ŭ���̾�Ʈ�� GUID�� ���´�
void CHackShieldManager::SendGUIDReq(USERINFO* pUserInfo)
{
#ifdef _HACK_SHIELD_
	if( pUserInfo->UserLevel < eUSERLEVEL_SUPERUSER )
		return;

	MSG_GUID_REQ msg;

	// ���� �ʱ�ȭ
	memset(&(msg.msg), 0, SIZEOF_GUIDREQMSG);

	msg.Category = MP_HACKSHIELD;
	msg.Protocol = MP_HACKSHIELD_GUID_REQ;
	
	DWORD dwRet = ERROR_SUCCESS;
	
	// _AntiCpSvr_MakeGuidReqMsg��
	// ù��° ���ڴ� Ŭ���̾�Ʈ�� ���� �޼��� ����
	// �ι�° ���ڴ� �޼��� ����
	dwRet = _AntiCpSvr_MakeGuidReqMsg ( msg.msg, pUserInfo->m_GUIDInfo );
	
	if ( dwRet != ERROR_SUCCESS )
	{
		// ����ó��
	}
	else
	{
		g_Network.Send2User(pUserInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
		pUserInfo->m_bHSCheck = 2;
	}
#endif
}

void CHackShieldManager::SendReq(USERINFO* pUserInfo)
{
#ifdef _HACK_SHIELD_
	if( pUserInfo->UserLevel < eUSERLEVEL_SUPERUSER)
		return;

	if(pUserInfo->m_bHSCheck == 2)
	{
		// �������� ��û �޼����� ������ ������.
		// �� GUID�޼����� ���� ���� �䱸 �޼�����
		// ������ ������ �޼����� �����Ƿ�
		// �䱸�޼��� �����ڸ��� �̸��� ���ü��� �����Ƿ� �ѹ����� ���ش�.
		// ���� ���������� ��������� �߶������.
		pUserInfo->m_bHSCheck = 1;
		return;
	}
	if(pUserInfo->m_bHSCheck == 1)
	{
		// ���� ���� �޼����� ���� ������ ������...
		MSGBASE msg;
		msg.Category = MP_HACKSHIELD;
		msg.Protocol = MP_HACKSHIELD_DISCONNECT;
		g_Network.Send2User(pUserInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
		DisconnectUser(pUserInfo->dwConnectionIndex);
		return;
	}

	MSG_REQ msg;
	memset(&(msg.msg), 0, SIZEOF_REQMSG);

	msg.Category = MP_HACKSHIELD;
	msg.Protocol = MP_HACKSHIELD_REQ;

	DWORD dwRet = ERROR_SUCCESS;

	// _AntiCpSvr_MakeReqMsg��
	// ù��° ���ڴ� Ŭ���̾�Ʈ�� ����� CRC ������ ���� ������
	// �ι�° ���ڴ� Ŭ���̾�Ʈ�� ���� �޼���
	// ����° ���ڴ� �޼����� ���� ����
	// �׹�° ���ڴ� �ɼ�
	dwRet = _AntiCpSvr_MakeReqMsg( pUserInfo->m_pCrcInfo, msg.msg, pUserInfo->m_Info, ANTICPSVR_CHECK_GAME_MEMORY);

	if ( dwRet != ERROR_SUCCESS )
	{

	}
	else
	{
		g_Network.Send2User(pUserInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
		pUserInfo->m_bHSCheck = 1;
	}
#endif
}
