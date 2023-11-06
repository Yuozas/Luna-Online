#pragma once


#include "interface/cTabDialog.h"


class cStatic;
class cListDialog;
class CGuild;

struct GUILDMEMBERINFO;


class CGuildDialog : public cTabDialog  
{
public:
	CGuildDialog();
	virtual ~CGuildDialog();
	virtual void SetActive( BOOL );
	virtual void OnActionEvent( LONG id, void* p, DWORD event );
	virtual void Render();
	
	void Linking();
	
	void DeleteMemberAll();
	
	void SetScore( DWORD score );
	void SetPosition( const char* );
	void SetUnionName( const char* );
	const char* GetUnionName() const;
	
	void Add( cWindow* );

	const GUILDMEMBERINFO* GetSelectedMember() const;

	void SetData			( const MSG_GUILD_DATA& );
	
	void LoginMember		( DWORD playerIndex );
	void LogoutMember		( DWORD playerIndex );

	void SetLevel			( DWORD level );
	DWORD GetLevel			() const;
	void AddMember			( const GUILDMEMBERINFO& );
	void RemoveMember		( DWORD playerIndex );
	void SetMemberLevel		( DWORD playerIndex, DWORD level );
	void SetRank			( DWORD playerIndex, BYTE rank );
	
	// 080225 LUJ, 특정 플레이어에 대한 길드 회원 정보를 반환한다
	GUILDMEMBERINFO* GetMember( DWORD playerIndex );

	// 080225 LUJ, 정보를 설정한다
	void SetMember( const GUILDMEMBERINFO& );

	const char* GetGuildName() const;
	const char* GetMasterName() const;
	DWORD		GetMasterIndex() const;
	DWORD GetScore() const;
	DWORD GetMemberSize() const;
	DWORD GetStudentSize() const;

	DWORD GetKickMemberIndex() const;
	
private:
	void SortByRank();
	void SortByLevel();
	void FormatText( const GUILDMEMBERINFO&, char*, SIZE_T ) const;
	void RefreshMemeber();
	
private:
	cStatic*	mGuildName;
	cStatic*	mGuildLevel;
	cStatic*	mGuildScore;
	cStatic*	mMasterName;
	cStatic*	mMemberSize;
	cStatic*	mPosition;
	cStatic*	mUnionName;
	
	cListDialog*	mMemberDialog;
	
	cWindow* mRetireButton;
	cWindow* mKickMemberButton;
	cWindow* mNameMemberButton;
	cWindow* mRankMemberButton;
	cWindow* mInviteMemberButton;
	cWindow* mLeaveMemberButton;
	cWindow* mNoticeButton;
	cWindow* mWarehouseRankButton;
	cWindow* mUnionInviteButton;
	cWindow* mLeaveUnionButton;
	cWindow* mKickUnionButton;
	cWindow* mDeclareWarButton;
	cWindow* mSuggestStopWarButton;
	cWindow* mSurrenderButton;
	cWindow* mAddTimeButton;
	cWindow* mManageApprenticeButton;
	cWindow* mUnionCreateButton;
	cWindow* mUnionDisbandButton;

	DWORD	mKickMemberIndex;

	int	m_CurGuildMode;

	//소팅방향관련 플래그
	BOOL mSortRankFlag;
	BOOL mSortLevelFlag;
	
	typedef std::list< GUILDMEMBERINFO >	MemberList;
	MemberList								mMemberList;

	typedef std::set< DWORD >	LoginPlayerSet;
	LoginPlayerSet				mLoginPlayerSet;

	DWORD mMasterIndex;
};
