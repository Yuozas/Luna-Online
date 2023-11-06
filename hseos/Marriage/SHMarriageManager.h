/*********************************************************************

	 ����		: SHMarriageManager.h
	 �ۼ���		: hseos
	 �ۼ���		: 2007/12/10

	 ���ϼ���	: ��ȥ Ŭ������ ���

 *********************************************************************/

#pragma once

class CPlayer;

#if !defined(_AGENTSERVER)
struct USERINFO;
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
class CSHMarriageManager
{
public:
	//----------------------------------------------------------------------------------------------------------------
	static UINT							DIVORCE_PENALTY_REMARRIAGE_TIME;		// ��ȥ �� ��ȥ�� �ʿ��� �ð�
	static UINT							TELEPORT_TO_PARTNER_FREQ;				// ����� ��ġ�� �ڷ���Ʈ ���� ȸ��
	static UINT							NICKNAME_CHARACTER_NUM;					// ȣĪ ����(����Ʈ) ��

	struct MARRIAGE_ITEM
	{
		DWORD							nItemID;
		UINT							nItemNum;
	};

	struct MARRIAGE_NEED_ITEM
	{
		DWORD*							pstWearItemID;
		UINT							nWearItemNum;
	};

	struct MARRIAGE_TAKE_ITEM
	{
		MARRIAGE_ITEM*					pstItem;
		UINT							nItemNum;
	};

	struct PACKET_MARRIAGE_INFO : public MSGBASE
	{
		DWORD			nPartnerID;													// ..����� ID
		BYTE			nRace;														// ..����
		BYTE			nJobFirst;													// ..����(ó��)
		BYTE			nJobCur;													// ..����(����)
		BYTE			nJobGrade;													// ..�������
		WORD			nLv;														// ..����
		WORD			nTeleportFreq;												// ..�ڷ���Ʈ Ƚ��
	};

private:
	//----------------------------------------------------------------------------------------------------------------
	MARRIAGE_NEED_ITEM					m_stMarriageNeedItem;
	MARRIAGE_TAKE_ITEM					m_stMarriageTakeItem;

public:
	//----------------------------------------------------------------------------------------------------------------
	CSHMarriageManager();
	~CSHMarriageManager();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����
	//
	//----------------------------------------------------------------------------------------------------------------
	//									������ �б�
	BOOL								LoadMarriageInfo();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����					
	//
	//----------------------------------------------------------------------------------------------------------------
	// ������Ʈ
	//									Ŭ���̾�Ʈ ��û �м�/ó��
	VOID								ASRV_ParseRequestFromClient(DWORD dwConnectionID, char* pMsg, DWORD dwLength);
	VOID								ASRV_ParseRequestFromServer(DWORD dwConnectionID, char* pMsg, DWORD dwLength);

	//									����� ��ġ�� �ڷ���Ʈ
	VOID								ASRV_TeleportToPartner(USERINFO* pUserInfo);

	//----------------------------------------------------------------------------------------------------------------
	// ��
	//									Ŭ���̾�Ʈ ��û �м�/ó��
	VOID								SRV_ParseRequestFromClient(DWORD dwConnectionID, char* pMsg, DWORD dwLength);

	//									��ȥ
	VOID								SRV_Marriage(CPlayer* pPlayer, DWORD nPartnerID);
	//									��ȥ ���
	VOID								SRV_MarriageResult(CPlayer* pPlayer, BOOL bAccept);
	//									���� ��ȥ
	VOID								SRV_AgreeDivorce(CPlayer* pPlayer, DWORD nPartnerID);
	//									���� ��ȥ ���
	VOID								SRV_AgreeDivorceResult(CPlayer* pPlayer, BOOL bAccept);
	//									���� ��ȥ
	VOID								SRV_ForceDivorce(CPlayer* pPlayer);
	//									���� ��ȥ ����
	VOID								SRV_ForceDivorced(CPlayer* pPlayer);
	//									����� ��ġ�� �ڷ���Ʈ
	VOID								SRV_TeleportToPartner(CPlayer* pPlayer);
	//									ȣĪ ����
	VOID								SRV_ChangeNickname(CPlayer* pPlayer, char* pszName);
	//									��ȥ ���� ������
	VOID								SRV_SendMarriageInfo(CPlayer* pPlayer);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Ŭ���̾�Ʈ
	//
	//----------------------------------------------------------------------------------------------------------------
	//									��ȥ ��û
	VOID								CLI_RequestMarriage(BOOL bWin);
	//									��ȥ ����
	VOID								CLI_RequestAcceptMarriage(BOOL bAccept);
	//									���� ��ȥ ��û
	VOID								CLI_RequestAgreeDivorce();
	//									���� ��ȥ ����
	VOID								CLI_RequestAcceptAgreeDivorce(BOOL bAccept);
	//									���� ��ȥ ��û
	VOID								CLI_RequestForceDivorce();
	//									����� ��ġ�� �ڷ���Ʈ ��û
	VOID								CLI_RequestTeleportToPartner();
	//									ȣĪ ���� ��û
	VOID								CLI_RequestNickname(char* pszNickname);

	//									�̸� �տ� ȣĪ ���̱�
	VOID								CLI_SetNickname(CPlayer* pPlayer, char* pszPartnerName, char* pszPartnerNickname = NULL);

	//----------------------------------------------------------------------------------------------------------------
	//									�����κ����� ���� �м�/ó��
	VOID								CLI_ParseAnswerFromSrv(void* pMsg);
};

extern CSHMarriageManager g_csMarriageManager;
