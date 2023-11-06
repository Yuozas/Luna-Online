/*********************************************************************

	 ����		: SHFamilyManager.h
	 �ۼ���		: hseos
	 �ۼ���		: 2007/07/03

	 ���ϼ���	: CSHFamilyManager Ŭ������ ���

 *********************************************************************/

#pragma once

class CPlayer;

#if !defined(_AGENTSERVER)
struct USERINFO;
#endif

#include "SHFamily.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
class CSHFamilyManager
{
public:
	//----------------------------------------------------------------------------------------------------------------
	enum FAMILY_CREATE_STATE																// �йи� ���� ���� ����
	{	
		FCS_NAME_CHECK1 = 0,																// ..DB���� �̸� üũ
		FCS_NAME_ERR,																		// ..DB�� ���� �̸� ����
		FCS_NAME_CHECK2,																	// ..���� ���̺��� �̸� üũ
		FCS_MONEY_ERR,																		// ..�ʿ� �� ����
		FCS_CREATE,																			// ..�йи� ����
		FCS_COMPLETE,																		// ..�йи� ���� �Ϸ�
	};

	enum FAMILY_INVITE_RESULT																// �йи� �ʴ� ���
	{
		FIR_ACCEPT_MASTER = 0,																// ..�ʴ� �¶� (�����Ϳ���)
		FIR_ACCEPT_PLAYER,																	// ..�ʴ� �¶� (�÷��̾��)
		FIR_DENY_MASTER,																	// ..�ʴ� ���� (�����Ϳ���)
		FIR_DENY_PLAYER,																	// ..�ʴ� ���� (�÷��̾��)
	};

	enum FAMILY_LEAVE_KIND																	// �йи� Ż�� ����
	{
		FLK_LEAVE = 0,																		// ..Ż��
		FLK_EXPEL,																			// ..�߹�
		FLK_BREAKUP,																		// ..��ü
		FLK_MAX,
	};

	//----------------------------------------------------------------------------------------------------------------
	static const int			MAX_FAMILY_UNIT_NUM				=	100;					// �йи� ������ �ִ� ����
	static const int			MAX_FAMILY_1U_NUM				=	100;					// �йи�(1���ִ�) �ִ� ����

	static const int			EMBLEM_WIDTH					=	16;						// ���� �̹��� ����
	static const int			EMBLEM_HEIGHT					=	16;						// ���� �̹��� ����
	static const int			EMBLEM_BPP						=	2;						// ���� �̹��� BPP
	static const int			EMBLEM_BUFSIZE					=	EMBLEM_WIDTH*			// ���� �̹��� ������
																	EMBLEM_HEIGHT*
																	EMBLEM_BPP;

	static DWORD				HONOR_POINT_CHECK_TIME;										// �� ����Ʈ ���� üũ �ð�
	static int					HONOR_POINT_CHECK_MEMBER_NUM;								// �� ����Ʈ ���� üũ ��� ��
	static int					HONOR_POINT_CHECK_POINT;									// �� ����Ʈ ���� ����Ʈ
	static float				MONKILL_HONOR_POINT_EXP_RATE;								// �� ����Ʈ�� ���޵Ǵ� ���� ų ����ġ ����

	static float				LEAVE_PENALTY_EXP_DOWN_RATE[FLK_MAX];						// �йи� Ż�� ���Ƽ ����ġ �ٿ� ����
	static int					LEAVE_PENALTY_REJOIN_TIME[FLK_MAX];							// �йи� Ż�� �� ����/â���� �ʿ��� �ð�


	//----------------------------------------------------------------------------------------------------------------
	struct PACKET_ADD_MEMBER: public MSGBASE												// ��� �߰� ��Ŷ ����
	{
		DWORD		nMasterID;																// ..������ID
		int			nRace;																	// ..����
		int			nSex;																	// ..����
		int			nJobFirst;																// ..����(ó��)
		int			nJobCur;																// ..����(����)
		int			nJobGrade;																// ..�������
		int			nLV;																	// ..����
		char		szName[MAX_NAME_LENGTH+1];												// ..�̸�
		BOOL		nResult;																// ..���� ���
	};

	struct PACKET_MEMBER_INFO: public MSGBASE												// ��� ���� ��Ŷ ����
	{
		int							nIndex;													// ..��� �ε���
		CSHFamilyMember::stINFO		stInfo;													// ..�⺻ ����
		CSHFamilyMember::stINFO_EX	stInfoEx;												// ..�߰� ����
	};

	struct PACKET_FAMILY: public MSGBASE													// �йи� ��Ŷ ����
	{
		CSHFamily::stINFO		stInfo;														// ..�⺻ ����
		CSHFamily::stINFO_EX	stInfoEx;													// ..�߰� ����
	};

	struct stFAMILY																			// �йи� ����
	{
		CSHFamily::stINFO		stInfo;														// ..�йи� �⺻ ����
		CSHFamily::stINFO_EX	stInfoEx;													// ..�йи� �߰� ����
	};

	struct stFAMILYMEMBER																	// ��� ����
	{
		int							nIndex;													// ..��� �ε���
		CSHFamilyMember::stINFO		stInfo;													// ..��� �⺻ ����
		CSHFamilyMember::stINFO_EX	stInfoEx;												// ..��� �߰� ����
	};

	struct PACKET_FAMILY_TOTAL: public MSGBASE												// �йи� ��� ���� ��Ŷ ����
	{
		stFAMILY		stFamily;															// ..�йи� ����
		stFAMILYMEMBER	stMember[CSHFamily::MAX_MEMBER_NUM];								// ..��� ����
	};

	struct PACKET_FAMILY_EMBLEM : public MSGBASE											// ���� ��Ŷ ����
	{
		DWORD nFamilyID;																	// ..�йи� ID
		DWORD nTargetPlayerID;																// ..��� �÷��̾� ID
		DWORD nChangedFreq;																	// ..���� ȸ��
		char cImgData[EMBLEM_BUFSIZE];														// ..�̹��� ������
	};

private:
	//----------------------------------------------------------------------------------------------------------------
	CSHFamily*					m_pcsFamily[MAX_FAMILY_UNIT_NUM];							// �йи� ���̺�
	int							m_nFamilyNum;												// �йи� ����

	DWORD						m_nInviteMasterID;											// �ʴ��� �йи� ������ID

public:
	//----------------------------------------------------------------------------------------------------------------
	CSHFamilyManager();
	~CSHFamilyManager();


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����
	//
	//----------------------------------------------------------------------------------------------------------------
	//							���̺� �йи� �߰�
	CSHFamily*					AddFamilyToTbl(CSHFamily* pcsFamily);
	//							���̺��� �йи� ����
	BOOL						DelFamilyToTbl(DWORD nPlayerID, CSHFamily* pcsFamily);
	BOOL						DelFamilyToTbl(CSHFamily* pcsFamily);
	//							�йи� ����
	VOID						SetFamily(CSHFamily* pcsFamily);
	//							�йи� ���
	CSHFamily*					GetFamily(DWORD nFamilyID);
	CSHFamily*					GetFamily(char* pszName);
	CSHFamily*					GetFamilyByMasterID(DWORD nMasterID);
	
	VOID						Process();

	BOOL						LoadFamilyInfo();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����					
	//
	//----------------------------------------------------------------------------------------------------------------
	//	������Ʈ
	//							Ŭ���̾�Ʈ ��û �м�/ó��
	VOID						ASRV_ParseRequestFromClient(DWORD dwConnectionID, char* pMsg, DWORD dwLength);
	VOID						ASRV_ParseRequestFromServer(DWORD dwConnectionID, char* pMsg, DWORD dwLength);
	//							�йи� ����
	VOID						ASRV_CreateFamily(USERINFO* pUserInfo, char* szFamilyName, int nCheckState, CSHFamily* pcsFamily = NULL);
	//							�йи��� �ʴ�
	VOID						ASRV_Invite(USERINFO* pUserInfo, DWORD nTargetPlayerID);
	//							�йи��� �ʴ� ���
	VOID						ASRV_InviteResult(USERINFO* pUserInfo, PACKET_ADD_MEMBER* pstInfo);
	//							ȣĪ ����
	VOID						ASRV_ChangeNickname(USERINFO* pUserInfo, DWORD nTargetPlayerID, char* pszNickname);
	//							�� ����Ʈ ó�� (���� ��ɽ� ����ġ�� ����)
	VOID						ASRV_UpdateHonorPoint(USERINFO* pUserInfo, DWORD nHonorPoint);
	//							���� �÷��̾� ���� ������Ʈ
	VOID						ASRV_UpdateMapPlayerInfo(USERINFO* pUserInfo, DWORD nLevel, char* pszGuildName);
	//							ȣĪ�� �ʼ����� ������
	VOID						ASRV_SendNicknameToMap(USERINFO* pUserInfo, char* pszNickname);
	//							���� ���
	VOID						ASRV_RegistEmblem(USERINFO* pUserInfo, DWORD nFamilyID, char* pImgData, int nState = 0);
	//							Ŭ���̾�Ʈ�� �йи� ���� ������
	VOID						ASRV_SendInfoToClient(CSHFamily* pcsFamily, int nType = 0);
	//							�ٸ� ������Ʈ�� ��� ���� �˸���
	VOID						ASRV_SendMemberConToOtherAgent(CSHFamily* pcsFamily, DWORD nMemberID, int nConState, int nType = 0);
	//							Ż��
	VOID						ASRV_Leave(USERINFO* pUserInfo);
	//							�߹�
	VOID						ASRV_Expel(USERINFO* pUserInfo, DWORD nMemberID);
	//							��ü
	VOID						ASRV_BreakUp(USERINFO* pUserInfo);

	//							�ٸ� ������Ʈ�κ����� ��� ���� ó��
	VOID						ASRV_MemberConFromOtherAgent(DWORD nFamilyID, DWORD nMemberID, int nConState, int nType = 0);
	//							�ٸ� ������Ʈ�κ����� ȣĪ ���� ó��
	VOID						ASRV_ChangeNicknameFromOtherAgent(DWORD nFamilyID, DWORD nBehavePlayerID, DWORD nTargetPlayerID, char* pszNickname);
	//							�ٸ� ������Ʈ�κ����� ���� �÷��̾� ���� ������Ʈ
	VOID						ASRV_UpdateMapPlayerInfoFromOtherAgent(DWORD nFamilyID, DWORD nPlayerID, DWORD nLevel, char* pszGuildName);
	//							�ٸ� ������Ʈ�κ����� �� ����Ʈ ó��
	VOID						ASRV_UpdateHonorPointFromOtherAgent(DWORD nFamilyID, DWORD nHonorPoint, int nType);
	//							�ٸ� ������Ʈ�κ����� �йи��� �ʴ�
	VOID						ASRV_InviteFromOtherAgent(USERINFO* pUserInfo, DWORD nMasterID, char* pszFamilyName);
	//							�ٸ� ������Ʈ�κ����� �йи��� �ʴ� ���
	VOID						ASRV_InviteResultFromOtherAgent(PACKET_ADD_MEMBER* pstInfo);
	//							�ٸ� ������Ʈ�κ����� �йи��� �ʴ� ���
	VOID						ASRV_InviteResultFromOtherAgent2(PACKET_FAMILY_TOTAL* pstInfo);
	//							�ٸ� ������Ʈ�κ����� ���� ���
	VOID						ASRV_RegistEmblemFromOtherAgent(DWORD nFamilyID, char* pImgData);
	//							�ٸ� ������Ʈ�κ����� Ż��
	VOID						ASRV_LeaveFromOtherAgent(DWORD nFamilyID, DWORD nMemberID);
	//							�ٸ� ������Ʈ�κ����� �߹�
	VOID						ASRV_ExpelFromOtherAgent(DWORD nFamilyID, DWORD nMemberID);
	//							�ٸ� ������Ʈ�κ����� ��ü
	VOID						ASRV_BreakUpFromOtherAgent(DWORD nFamilyID);

	//----------------------------------------------------------------------------------------------------------------
	//	��
	//							Ŭ���̾�Ʈ ��û �м�/ó��
	VOID						SRV_ParseRequestFromClient(DWORD dwConnectionID, char* pMsg, DWORD dwLength);
	//							�йи� ����
	VOID						SRV_CreateFamily(CPlayer* pPlayer, char* szFamilyName);
	//							�йи��� �ʴ� ���
	VOID						SRV_InviteResult(CPlayer* pPlayer, DWORD nMasterID, DWORD nResult);
	//							�� ����Ʈ ó�� (���� ��ɽ� ����ġ�� ����)
	VOID						SRV_ProcessHonorPoint(CPlayer* pPlayer, DWORD nExp);
	//							���� �÷��̾� ���� ������Ʈ
	VOID						SRV_UpdateMapPlayerInfo(CPlayer* pPlayer, char* pszGuildName = NULL);
	//							ȣĪ�� Ŭ���̾�Ʈ�� ������
	VOID						SRV_SendNicknameToClient(CPlayer* pPlayer, char* pszNickname);
	//							���� ���
	VOID						SRV_RegistEmblem(CPlayer* pPlayer, DWORD nFamilyID, char* pImgData, DWORD nChangedNum);
	//							���� ǥ��
	VOID						SRV_ShowEmblem(CPlayer* pPlayer, DWORD nFamilyID, char* pImgData, DWORD nChangedNum);
	//							���� ǥ��2
	VOID						SRV_ShowEmblem2(CPlayer* pPlayer, DWORD nTargetPlayerID);
	//							Ż��
	VOID						SRV_Leave(CPlayer* pPlayer, DWORD nPlayerID, int nKInd = 0);
	//							�߹�
	VOID						SRV_Expel(CPlayer* pPlayer, DWORD nMemberID, int nKInd = 0);
	//							��ü
	VOID						SRV_BreakUp(CPlayer* pPlayer, DWORD nPlayerID, int nKInd = 0);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Ŭ���̾�Ʈ
	//
	//----------------------------------------------------------------------------------------------------------------
	//							�йи� ���� ��û
	VOID						CLI_RequestCreateFamily(char* szFamilyName);
	//							�йи������� �ʴ� ��û
	VOID						CLI_RequestInvite(CPlayer* pPlayer);
	//							�йи��� �ʴ뿡 ���� ��û
	VOID						CLI_RequestAnswerToInvite(FAMILY_INVITE_RESULT eResult);
	//							���� ��� ��û
	BOOL						CLI_RequestRegistEmblem(char* pszBmpName);
	//							���� ǥ�� ��û (�׸������ �ٸ� �÷��̾�鿡��)
	VOID						CLI_RequestShowEmblem(DWORD nPlayerID, DWORD nTargetPlayerID);
	//							���� ǥ��
	VOID						CLI_ShowEmblem(CPlayer* pPlayer, DWORD nFamilyID, DWORD nChangedFreq);

	//							ȣĪ���� ��û
	VOID						CLI_RequestChangeNickname(DWORD nTargetPlayerID, char* szNickname);
	//							ȣĪON ��û
	VOID						CLI_RequestChangeNicknameON();

	//							Ż�� ��û
	VOID						CLI_RequestLeave();
	//							�߹� ��û
	VOID						CLI_RequestExpel(int nMemberID);
	//							��ü ��û
	VOID						CLI_RequestBreakUp();

	//----------------------------------------------------------------------------------------------------------------
	//							�����κ����� ���� �м�/ó��
	VOID						CLI_ParseAnswerFromSrv(void* pMsg);
};

extern CSHFamilyManager g_csFamilyManager;
