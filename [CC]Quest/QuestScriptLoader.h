// QuestScriptLoader.h: interface for the CQuestScriptLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTSCRIPTLOADER_H__19DDC2D1_0947_4501_B70B_C636D9250099__INCLUDED_)
#define AFX_QUESTSCRIPTLOADER_H__19DDC2D1_0947_4501_B70B_C636D9250099__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StrTokenizer.h"						// ��Ʈ�� ��ũ ������ ����� �ҷ��´�.

class CMHFile;									// ���� ���� Ŭ������ �����Ѵ�.
class CQuestInfo;								// ����Ʈ ���� Ŭ������ �����Ѵ�.
class CSubQuestInfo;							// ���� ����Ʈ ���� Ŭ������ �����Ѵ�.
class CQuestString;								// ����Ʈ ��Ʈ�� Ŭ������ �����Ѵ�.

class CQuestLimitKind;							// ����Ʈ ���� ���� Ŭ������ �����Ѵ�.
class CQuestCondition;							// ����Ʈ ���� Ŭ������ �����Ѵ�.
class CQuestEvent;								// ����Ʈ �̺�Ʈ Ŭ������ �����Ѵ�.
class CQuestExecute;							// ����Ʈ ���� Ŭ������ �����Ѵ�.

class CQuestNpcScriptData;						// ����Ʈ NPC ��ũ��Ʈ ������ Ŭ������ �����Ѵ�.
class CQuestNpcData;							// ����Ʈ NPC ������ Ŭ������ �����Ѵ�.

class CQuestScriptLoader						// ����Ʈ ��ũ��Ʈ �δ� Ŭ����.
{
public:
	CQuestScriptLoader();						// ������ �Լ�.
	virtual ~CQuestScriptLoader();				// �Ҹ��� �Լ�.

	// ����Ʈ ������ �����ϴ� �Լ�.
	static CQuestInfo*		LoadQuestInfo( CMHFile* pFile, DWORD dwQuestIdx );
	// ���� ����Ʈ ������ �����ϴ� �Լ�.
	static CSubQuestInfo*	LoadSubQuestInfo( CMHFile* pFile, DWORD dwQuestIdx, DWORD dwSubQuestIdx, CQuestInfo* pQuestInfo  );
	// ����Ʈ ��Ʈ�� ������ �����ϴ� �Լ�.
	static CQuestString*	LoadQuestString(CMHFile* pFile);
	//static void LoadQuestString(CMHFile* pFile, CQuestString* pQStr) ;

	// ����Ʈ ���� ���� ������ �����ϴ� �Լ�.
	static CQuestLimitKind*	LoadQuestLimitKind( char* strKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	// ����Ʈ ���� ������ �����ϴ� �Լ�.
	static CQuestCondition*	LoadQuestCondition( char* strKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	// ����Ʈ �̺�Ʈ ������ �����ϴ� �Լ�.
	static CQuestEvent*		LoadQuestEvent( char* strKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );	
	// ����Ʈ ���� ������ �����ϴ� �Լ�.
	static CQuestExecute*	LoadQuestExecute( char* strKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	
	// ����Ʈ NPC ��ũ��Ʈ ������ ������ �����ϴ� �Լ�.
	static CQuestNpcScriptData*	LoadQuestNpcScriptData( char* strKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	// ����Ʈ NPC ������ ������ �����ϴ� �Լ�.
	static CQuestNpcData*		LoadQuestNpcData( char* strKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
};

// ������ �ƿ��� �о���� ��ū ���� ��ȯ�� �����ϴ� �Լ�.
template<class T> void GetScriptParam( T& dataOut, CStrTokenizer* pTokens )	
{
	char* pOneToken = pTokens->GetNextTokenUpper();		// ��ū�� GetNextTokenUpper() �Լ��� ȣ���Ѵ�.
	dataOut = (T)atol(pOneToken);						// ��ū ���� ���ڷ� ��ȯ�� ������ �ƿ��� ��´�.
}

#endif // !defined(AFX_QUESTSCRIPTLOADER_H__19DDC2D1_0947_4501_B70B_C636D9250099__INCLUDED_)
