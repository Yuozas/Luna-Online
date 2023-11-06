#pragma once


class CMSSystem  
{
public:
	CMSSystem();
	virtual ~CMSSystem();


	// 080813 LUJ, 수정된 inetwork.dll으로 호출 형식 변경
	static void __stdcall OnAcceptServer(DWORD dwConnectionIndex);
	static void __stdcall OnDisconnectServer(DWORD dwConnectionIndex);
	static void __stdcall ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);

	void Start();
	void End();

	// 080813 LUJ, 수정된 inetwork.dll으로 호출 형식 변경
	static void __stdcall Process( DWORD eventIndex );

	// 080813 LUJ, 수정된 inetwork.dll으로 호출 형식 변경
	static void __stdcall OnAcceptUser(DWORD dwConnectionIndex);
	static void __stdcall OnDisconnectUser(DWORD dwConnectionIndex);

	// 080813 LUJ, 수정된 inetwork.dll으로 호출 형식 변경
	static void	__stdcall ReceivedMsgFromRMTool(DWORD dwConnectionIndex,char* pMsg, DWORD );
	static void __stdcall ConnentMsgParse(DWORD dwConnectionIndex,char* pMsg, size_t );
	
	// 메시지 파서
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
	// 더 이상의 쿼리를 취소 요청했는지 알린다
	bool IsStopQuery( DWORD connectionIndex ) const;

	// 쿼리가 취소되었음을 알린다. BYTE는 프로토콜
	void StopQuery( DWORD connectionIndex, RecoveryCategory, BYTE );

private:
	// 키: 접속 인덱스. 연속되는 요청은 이 자료구조를 검색하여 취소하도록 한다
	// 
	// 예를 들어 로그같은 경우 많은 레코드 수를 가지고 있어 전체 결과를
	// 받는데 상당한 시간이 걸릴 수 있다. 잘못 쿼리를 날려 클라이언트를 닫았을 경우에도
	// 서버는 계속 프로세스가 돌게 된다. 서버의 프로세스를 취소시키고 쓸데없는 패킷을
	// 전송하지 않기 위해 이 자료구조를 검색하여 쿼리를 계속할지 여부를 판단하게 한다.
	typedef stdext::hash_set< DWORD >	StopQuerySet;
	StopQuerySet						mStopQuerySet;


private:
	static bool mIsRunning;

public:
	static bool IsRunning();
};
