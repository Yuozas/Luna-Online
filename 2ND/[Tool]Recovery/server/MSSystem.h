#pragma once


class CMSSystem  
{
public:
	CMSSystem();
	virtual ~CMSSystem();


	// 080813 LUJ, ������ inetwork.dll���� ȣ�� ���� ����
	static void __stdcall OnAcceptServer(DWORD dwConnectionIndex);
	static void __stdcall OnDisconnectServer(DWORD dwConnectionIndex);
	static void __stdcall ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);

	void Start();
	void End();

	// 080813 LUJ, ������ inetwork.dll���� ȣ�� ���� ����
	static void __stdcall Process( DWORD eventIndex );

	// 080813 LUJ, ������ inetwork.dll���� ȣ�� ���� ����
	static void __stdcall OnAcceptUser(DWORD dwConnectionIndex);
	static void __stdcall OnDisconnectUser(DWORD dwConnectionIndex);

	// 080813 LUJ, ������ inetwork.dll���� ȣ�� ���� ����
	static void	__stdcall ReceivedMsgFromRMTool(DWORD dwConnectionIndex,char* pMsg, DWORD );
	static void __stdcall ConnentMsgParse(DWORD dwConnectionIndex,char* pMsg, size_t );
	
	// �޽��� �ļ�
	static void ParseSearch				( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParsePlayer				( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParseItem				( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParseItemFind			( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParseItemLog			( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParseItemOptionLog		( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParseSkill				( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParseSkillLog			( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParseFamily				( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParseQuest				( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParseQuestLog			( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParseGuild				( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParseGuildLog			( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParseGuildWarehouseLog	( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParseExperienceLog				( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParseStatLog			( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParseUser				( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParseJobLog				( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParseGuildScoreLog		( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParseFamilyCropLog		( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParseFamilyPointLog		( DWORD connectionIndex, const MSGROOT*, size_t );
	//static void ParseUserCreate			( DWORD connectionIndex, const MSGROOT*, size_t );
	//static void ParseRestoreUser		( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParsePermission			( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParseOperator			( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParseOperatorLog		( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParsePassword			( DWORD connectionIndex, const MSGROOT*, size_t );
	static void ParseNameLog			( DWORD connectionIndex, const MSGROOT*, size_t );
	

public:
	// �� �̻��� ������ ��� ��û�ߴ��� �˸���
	bool IsStopQuery( DWORD connectionIndex ) const;

	// ������ ��ҵǾ����� �˸���. BYTE�� ��������
	void StopQuery( DWORD connectionIndex, RecoveryCategory, BYTE );

private:
	// Ű: ���� �ε���. ���ӵǴ� ��û�� �� �ڷᱸ���� �˻��Ͽ� ����ϵ��� �Ѵ�
	// 
	// ���� ��� �αװ��� ��� ���� ���ڵ� ���� ������ �־� ��ü �����
	// �޴µ� ����� �ð��� �ɸ� �� �ִ�. �߸� ������ ���� Ŭ���̾�Ʈ�� �ݾ��� ��쿡��
	// ������ ��� ���μ����� ���� �ȴ�. ������ ���μ����� ��ҽ�Ű�� �������� ��Ŷ��
	// �������� �ʱ� ���� �� �ڷᱸ���� �˻��Ͽ� ������ ������� ���θ� �Ǵ��ϰ� �Ѵ�.
	typedef stdext::hash_set< DWORD >	StopQuerySet;
	StopQuerySet						mStopQuerySet;


private:
	static bool mIsRunning;

public:
	static bool IsRunning();
};
