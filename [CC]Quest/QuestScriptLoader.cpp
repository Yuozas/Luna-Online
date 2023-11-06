// QuestScriptLoader.cpp: implementation of the CQuestScriptLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"																	// ǥ�� �ý��۰�, ������Ʈ�� ������, ���־��� �ش����� ���� �ش������� �ҷ��´�.
#include "QuestScriptLoader.h"														// ����Ʈ ��ũ��Ʈ �δ� ����� �ҷ��´�.

#include "MHFile.h"																	// ���� ���� ����� �ҷ��´�.
#include "QuestInfo.h"																// ����Ʈ ���� ����� �ҷ��´�.
#include "SubQuestInfo.h"															// ���� ����Ʈ ���� ����� �ҷ��´�.
#include "QuestString.h"															// ����Ʈ ��Ʈ�� ����� �ҷ��´�.
#include "QuestEvent.h"																// ����Ʈ �̺�Ʈ ����� �ҷ��´�.

#include "QuestLimit.h"																// ����Ʈ ����Ʈ ����� �ҷ��´�.
#include "QuestLimitKind.h"															// ����Ʈ ����Ʈ ���� ����� �ҷ��´�.
#include "QuestLimitKind_Level.h"													// ����Ʈ ����Ʈ ���� �� ���� ����� �ҷ��´�.
#include "QuestLimitKind_Quest.h"													// ����Ʈ ����Ʈ ���� �� ����Ʈ ����� �ҷ��´�.

#include "QuestLimitKind_Stage.h"													// ����Ʈ ������ ���� �� �������� ����� �ҷ��´�.

#include "QuestTrigger.h"															// ����Ʈ Ʈ���� ����� �ҷ��´�.
#include "QuestCondition.h"															// ����Ʈ ���� ����� �ҷ��´�.
#include "QuestExecute.h"															// ����Ʈ ���� ����� �ҷ��´�.
#include "QuestExecute_Quest.h"														// ����Ʈ ���� ����Ʈ ����� �ҷ��´�.
#include "QuestExecute_Count.h"														// ����Ʈ ���� ī��Ʈ ����� �ҷ��´�.
#include "QuestExecute_Item.h"														// ����Ʈ ���� ������ ����� �ҷ��´�.
#include "QuestExecute_RandomItem.h"												// ����Ʈ ���� ���� ������ ����� �ҷ��´�.
#include "QuestExecute_Time.h"														// ����Ʈ ���� �ð� ����� �ҷ��´�.

#include "QuestNpcScript.h"															// ����Ʈ NPC ��ũ��Ʈ ����� �ҷ��´�.
#include "QuestNpcScriptData.h"														// ����Ʈ NPC ��ũ��Ʈ ������ ����� �ҷ��´�.
#include "QuestNpc.h"																// ����Ʈ NPC ����� �ҷ��´�.
#include "QuestNpcData.h"															// ����Ʈ NPC ������ ����� �ҷ��´�.

//#include "interface/cWindowDef.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define LOADUNIT(ekind,strkind,classname,questidx,subquestidx)	\
		if( strcmp(strKind,strkind) == 0 )						\
			return new classname(ekind,pTokens,questidx,subquestidx);

CQuestScriptLoader::CQuestScriptLoader()											// ������ �Լ�.
{
}

CQuestScriptLoader::~CQuestScriptLoader()											// �Ҹ��� �Լ�.
{
}

CQuestInfo* CQuestScriptLoader::LoadQuestInfo( CMHFile* pFile, DWORD dwQuestIdx )	// ����Ʈ ������ �ε��ϴ� �Լ�.
{
	CQuestInfo* pQuestInfo = new CQuestInfo( dwQuestIdx );							// ����Ʈ ������ ���� �����͸� �����ϰ�, ����Ʈ ���� Ŭ���� ��ŭ �޸𸮸� �Ҵ��� �޴´�.

	CSubQuestInfo* pSub;															// ���� ����Ʈ ������ ���� �����͸� �����Ѵ�.

	char tok[256];																	// �ӽ� ���۸� �����Ѵ�.

	DWORD dwSubQuestIdx = 0;														// ���� ����Ʈ �ε����� ���� ������ �����ϰ� 0���� �����Ѵ�.

	while( !pFile->IsEOF() )														// ���� �����Ͱ� ������ ���� �������� ���� ����	while���� ������.
	{
		pFile->GetString( tok );													// �ӽ� ���ۿ� ��Ʈ���� �о���δ�.

		if( tok[0] == '}' )															// }�� ������,
		{
			break;																	// while���� Ż���Ѵ�.
		}
		
		if( strcmp( tok, "$REPEAT" ) == 0 )										
		{
			pQuestInfo->SetRepeat();
		}

		if( strcmp( tok, "$SUBQUEST" ) == 0 )										// $SUBQUEST�� ������,
		{
			dwSubQuestIdx = pFile->GetDword();										// ���� ����Ʈ �ε����� �о���δ�.

			pSub = LoadSubQuestInfo( pFile, dwQuestIdx, dwSubQuestIdx, pQuestInfo );// ���� ����Ʈ ������ �ε��Ͽ�, �����ͷ� �޴´�.

			ASSERT(pSub);															// ASSERT üũ�� �Ѵ�.

			pQuestInfo->AddSubQuestInfo( dwSubQuestIdx, pSub );						// ����Ʈ ������ ���� ����Ʈ ������ �߰��Ѵ�.
		}
	}

	return pQuestInfo;																// ����Ʈ ������ �����Ѵ�.
}

CSubQuestInfo* CQuestScriptLoader::LoadSubQuestInfo( 
	CMHFile* pFile, DWORD dwQuestIdx, DWORD dwSubQuestIdx, CQuestInfo* pQuestInfo ) // ���� ����Ʈ�� �ε��ϴ� �Լ�.
{
	CSubQuestInfo* pSub = new CSubQuestInfo;										// ���� ����Ʈ ��ŭ �޸𸮸� �Ҵ��ϰ� �����ͷ� �޴´�.
	
	char buf[1024];																	// �ӽ� ���۸� �����Ѵ�.

	char Token[1024];																// ��ū ���۸� �����Ѵ�.

	while( !pFile->IsEOF() )														// ���� �����Ͱ� ������ ���� �������� ���� ���� while���� ������.
	{
		pFile->GetString( Token );													// ��ū ���ۿ� ��Ʈ���� �о���δ�.

		if( Token[0] == '}' )														// }�� ������,
		{
			break;																	// while���� Ż���Ѵ�.
		}

		if( strcmp(Token,"#LIMIT") == 0 )											// #LIMIT�� ������,
		{
			pFile->GetLine( buf, 1024 );											// �ӽù��۷� ������ �о���δ�.

			CStrTokenizer tok( buf, " \t\n" );										// �ǰ� ���������� ��ū �����Ͽ� �����Ѵ�.

			pSub->AddQuestLimit(new CQuestLimit(&tok,dwQuestIdx,dwSubQuestIdx));	// ���� ����Ʈ�� ����Ʈ ����Ʈ�� �߰��Ѵ�.
		}

//#ifdef _MAPSERVER_																	// �ʼ����� ����Ǿ��ٸ�,

		else if( strcmp(Token,"#TRIGGER") == 0 )									// �о���� ��ū�� #TRIGGER�� ������,
		{
			pFile->GetLine( buf, 1024 );											// �ӽ� ���۷� ������ �о���δ�.

			CStrTokenizer tok( buf, " \t\n" );										// �ǰ� ���������� ��ū �����Ͽ� �����Ѵ�.
			// �ݺ�����Ʈ�� Ȯ���� ���ؼ� �߰� RaMa - 04.10.26
			CQuestTrigger* pQuestTrigger = new CQuestTrigger( &tok, dwQuestIdx, dwSubQuestIdx );	// Ʈ���� �����ͷ� �޸𸮸� �Ҵ��� �޴´�.

			if(pQuestTrigger)														// Ʈ���� ������ ��ȿ���� üũ�Ѵ�.
			{
				pQuestInfo->SetEndParam( pQuestTrigger->GetEndParam() );			// ���� �Ķ���͸� �����Ѵ�.
			}

			pSub->AddQuestTrigger( pQuestTrigger );									// ���� ����Ʈ�� Ʈ���ŷ� �߰��Ѵ�.
		}		
//#else																				// �� ������ ������� �ʾҴٸ�,

		else if( strcmp(Token,"#NPCSCRIPT") == 0 )									// #NPCSCRIPT �� ���ٸ�,
		{
			pFile->GetLine( buf, 1024 );											// �ӽù��۷� ������ �о���δ�.

			CStrTokenizer tok( buf, " \t\n" );										// �ǰ� ���������� ��ū �����Ͽ� �����Ѵ�.

			pSub->AddQuestNpcScipt( new CQuestNpcScript( &tok, dwQuestIdx, dwSubQuestIdx ) );	// ���� ����Ʈ�� NPC ��ũ��Ʈ�� �߰��Ѵ�.
		}
		else if( strcmp(Token,"#NPCADD") == 0 )										// #NPCADD�� ���ٸ�,
		{
			pFile->GetLine( buf, 1024 );											// �ӽù��۷� ������ �о���δ�.

			CStrTokenizer tok( buf, " \t\n" );										// �ǰ� ���������� ��ū �����Ͽ� �����Ѵ�.

			pSub->AddQuestNpc( new CQuestNpc( &tok, dwQuestIdx, dwSubQuestIdx ) );	// ���� ����Ʈ NPC�� �߰��Ѵ�.
		}		
		else if( strcmp(Token, "#MAXCOUNT") == 0 )									// #MAXCOUNT�� ���ٸ�,
		{
			pSub->SetMaxCount( pFile->GetDword() );									// ���� ����Ʈ�� �ְ� ī��Ʈ�� �����Ѵ�.
		}
//#endif
	}

	return pSub;																	// ���� ����Ʈ ������ �����Ѵ�.
}

CQuestString* CQuestScriptLoader::LoadQuestString(CMHFile* pFile)					// ����Ʈ ��Ʈ���� �ε��Ͽ� ��ȯ�ϴ� �Լ�.
{
	CQuestString* pQStr = new CQuestString;											// ����Ʈ ��Ʈ�� ��ŭ �޸𸮸� �Ҵ��Ͽ� �����ͷ� �޴´�.

	char* pChekString = NULL;														// üũ ��Ʈ�� �����͸� �����ϰ� NULL ó���� �Ѵ�.

	char buf[1024];																	// �ӽ� ���۸� �����Ѵ�.
	char Token[1024];																// ��ū ���۸� �����Ѵ�.

	int	tline=0;																	// Ÿ��Ʋ ���α��̵� ���� ������ �����ϰ� 0���� �����Ѵ�.
	int dline=0;																	// ���� ���̸� ���� ������ �����ϰ� 0���� �����Ѵ�.

	while( !pFile->IsEOF() )														// ���� �����Ͱ� ���� ���� �ƴҵ��� while���� ������.
	{
		pFile->GetString(Token);													// ��ū ���ۿ� ��Ʈ���� �о���δ�.

		if( pChekString = strstr( Token, "}" ) )									// ��ū�� }�� ������,
		{
			break;																	// while���� Ż���Ѵ�.
		}
		
		if(strcmp(Token,"#TITLE") == 0)												// ��ū�� #TITLE �� ���ٸ�,
		{
			pFile->GetLine(buf, 1024);												// �ӽ� ���ۿ� ������ �о���δ�.

			pQStr->AddLine(buf, tline, TRUE);										// ����Ʈ ��Ʈ���� ������ �߰��Ѵ�.
		}
		if(strcmp(Token,"#DESC") == 0)												// ��ū�� #DESC�� ���ٸ�,
		{
			while( !pFile->IsEOF() )												// ���� �����Ͱ� ���� ���� �ƴҵ��� while���� ������.
			{
				pFile->GetLine(buf, 1024);											// �ӽ� ���ۿ� ������ �о���δ�.

				char * p = buf;														// �ӽ� ���۸� �����ͷ� �޴´�.

				int nRt = 0;														// ��� ���� ���� ������ �����ϰ� 0���� �����Ѵ�.

				while( *p )															// �����Ͱ� ��ȿ�� ���� while���� ������.
				{
					if( IsDBCSLeadByte( *p ) )										// ���� ����Ʈ ���,
					{
						++p;														// �����͸� ������Ų��.
					}
					else															// ���� ����Ʈ�� �ƴ϶��,
					{
						if( *p == '{' )												// {�� ���ٸ�,
						{
							nRt = 1;												// ��� ���� 1�� �����Ѵ�.
							break;													// while���� Ż���Ѵ�.
						}
						else if(  *p == '}' )										// }�� ���ٸ�,
						{
							nRt = 2;												// ��� ���� 2�� �����Ѵ�.
							break;													// while���� Ż���Ѵ�.
						}
					}

					++p;															// �����͸� ������Ų��.
				}

				if( nRt == 1 )														// ��� ���� 1�� ���ٸ�, 
				{
					continue;														// ��� �����Ѵ�.
				}
				else if( nRt == 2 )													// ��� ���� 2�� ���ٸ�,
				{
					break;															// while���� Ż���Ѵ�.
				}

				pQStr->AddLine(buf, dline);											// ��Ʈ�� �����Ϳ� ������ �߰��Ѵ�.

				++dline;															// ���� ���� ������Ų��.
			}
		}
	}

	return pQStr;																	// ��Ʈ�� �����͸� �����Ѵ�.
}

//void CQuestScriptLoader::LoadQuestString(CMHFile* pFile, CQuestString* pQStr)		// ����Ʈ ��Ʈ���� �ε��Ͽ� ��ȯ�ϴ� �Լ�.
//{
//	char* pChekString = NULL;														// üũ ��Ʈ�� �����͸� �����ϰ� NULL ó���� �Ѵ�.
//
//	char buf[1024];																	// �ӽ� ���۸� �����Ѵ�.
//	char Token[1024];																// ��ū ���۸� �����Ѵ�.
//
//	int	tline=0;																	// Ÿ��Ʋ ���α��̵� ���� ������ �����ϰ� 0���� �����Ѵ�.
//	int dline=0;																	// ���� ���̸� ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//	while( !pFile->IsEOF() )														// ���� �����Ͱ� ���� ���� �ƴҵ��� while���� ������.
//	{
//		pFile->GetString(Token);													// ��ū ���ۿ� ��Ʈ���� �о���δ�.
//
//		if( pChekString = strstr( Token, "}" ) )									// ��ū�� }�� ������,
//		{
//			break;																	// while���� Ż���Ѵ�.
//		}
//		
//		if(strcmp(Token,"#TITLE") == 0)												// ��ū�� #TITLE �� ���ٸ�,
//		{
//			pFile->GetLine(buf, 1024);												// �ӽ� ���ۿ� ������ �о���δ�.
//
//			//pQStr->AddLine(buf, tline, TRUE);										// ����Ʈ ��Ʈ���� ������ �߰��Ѵ�.
//
//			ITEM* pItem = new ITEM ;
//
//			strcpy(pItem->string, buf) ;
//			pItem->line = tline ;
//			pItem->rgb = RGB(0, 0, 64) ;
//
//			pQStr->GetTitle()->AddTail(pItem) ;
//		}
//		if(strcmp(Token,"#DESC") == 0)												// ��ū�� #DESC�� ���ٸ�,
//		{
//			while( !pFile->IsEOF() )												// ���� �����Ͱ� ���� ���� �ƴҵ��� while���� ������.
//			{
//				pFile->GetLine(buf, 1024);											// �ӽ� ���ۿ� ������ �о���δ�.
//
//				char * p = buf;														// �ӽ� ���۸� �����ͷ� �޴´�.
//
//				int nRt = 0;														// ��� ���� ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//				while( *p )															// �����Ͱ� ��ȿ�� ���� while���� ������.
//				{
//					if( IsDBCSLeadByte( *p ) )										// ���� ����Ʈ ���,
//					{
//						++p;														// �����͸� ������Ų��.
//					}
//					else															// ���� ����Ʈ�� �ƴ϶��,
//					{
//						if( *p == '{' )												// {�� ���ٸ�,
//						{
//							nRt = 1;												// ��� ���� 1�� �����Ѵ�.
//							break;													// while���� Ż���Ѵ�.
//						}
//						else if(  *p == '}' )										// }�� ���ٸ�,
//						{
//							nRt = 2;												// ��� ���� 2�� �����Ѵ�.
//							break;													// while���� Ż���Ѵ�.
//						}
//					}
//
//					++p;															// �����͸� ������Ų��.
//				}
//
//				if( nRt == 1 )														// ��� ���� 1�� ���ٸ�, 
//				{
//					continue;														// ��� �����Ѵ�.
//				}
//				else if( nRt == 2 )													// ��� ���� 2�� ���ٸ�,
//				{
//					break;															// while���� Ż���Ѵ�.
//				}
//
//				pQStr->AddLine(buf, dline);											// ��Ʈ�� �����Ϳ� ������ �߰��Ѵ�.
//
//				++dline;															// ���� ���� ������Ų��.
//			}
//		}
//	}
//}

//CQuestString* CQuestScriptLoader::LoadQuestString(CMHFile* pFile)					// ����Ʈ ��Ʈ���� �ε��Ͽ� ��ȯ�ϴ� �Լ�.
//{
//	CQuestString* pQStr = new CQuestString;											// ����Ʈ ��Ʈ�� ��ŭ �޸𸮸� �Ҵ��Ͽ� �����ͷ� �޴´�.
//
//	char* pChekString = NULL;														// üũ ��Ʈ�� �����͸� �����ϰ� NULL ó���� �Ѵ�.
//
//	char buf[1024];																	// �ӽ� ���۸� �����Ѵ�.
//	char Token[1024];																// ��ū ���۸� �����Ѵ�.
//
//	int	tline=0;																	// Ÿ��Ʋ ���α��̵� ���� ������ �����ϰ� 0���� �����Ѵ�.
//	int dline=0;																	// ���� ���̸� ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//	while( !pFile->IsEOF() )														// ���� �����Ͱ� ���� ���� �ƴҵ��� while���� ������.
//	{
//		pFile->GetString(Token);													// ��ū ���ۿ� ��Ʈ���� �о���δ�.
//
//		if( pChekString = strstr( Token, "}" ) )									// ��ū�� }�� ������,
//		{
//			break;																	// while���� Ż���Ѵ�.
//		}
//		
//		if(strcmp(Token,"#TITLE") == 0)												// ��ū�� #TITLE �� ���ٸ�,
//		{
//			pFile->GetLine(buf, 1024);												// �ӽ� ���ۿ� ������ �о���δ�.
//
//			pQStr->AddLine(buf, tline, TRUE);										// ����Ʈ ��Ʈ���� ������ �߰��Ѵ�.
//		}
//		if(strcmp(Token,"#DESC") == 0)												// ��ū�� #DESC�� ���ٸ�,
//		{
//			while( !pFile->IsEOF() )												// ���� �����Ͱ� ���� ���� �ƴҵ��� while���� ������.
//			{
//				pFile->GetLine(buf, 1024);											// �ӽ� ���ۿ� ������ �о���δ�.
//
//				char * p = buf;														// �ӽ� ���۸� �����ͷ� �޴´�.
//
//				int nRt = 0;														// ��� ���� ���� ������ �����ϰ� 0���� �����Ѵ�.
//
//				while( *p )															// �����Ͱ� ��ȿ�� ���� while���� ������.
//				{
//					if( IsDBCSLeadByte( *p ) )										// ���� ����Ʈ ���,
//					{
//						++p;														// �����͸� ������Ų��.
//					}
//					else															// ���� ����Ʈ�� �ƴ϶��,
//					{
//						if( *p == '{' )												// {�� ���ٸ�,
//						{
//							nRt = 1;												// ��� ���� 1�� �����Ѵ�.
//							break;													// while���� Ż���Ѵ�.
//						}
//						else if(  *p == '}' )										// }�� ���ٸ�,
//						{
//							nRt = 2;												// ��� ���� 2�� �����Ѵ�.
//							break;													// while���� Ż���Ѵ�.
//						}
//					}
//
//					++p;															// �����͸� ������Ų��.
//				}
//
//				if( nRt == 1 )														// ��� ���� 1�� ���ٸ�, 
//				{
//					continue;														// ��� �����Ѵ�.
//				}
//				else if( nRt == 2 )													// ��� ���� 2�� ���ٸ�,
//				{
//					break;															// while���� Ż���Ѵ�.
//				}
//
//				pQStr->AddLine(buf, dline);											// ��Ʈ�� �����Ϳ� ������ �߰��Ѵ�.
//
//				++dline;															// ���� ���� ������Ų��.
//			}
//		}
//	}
//
//	return pQStr;																	// ��Ʈ�� �����͸� �����Ѵ�.
//}

// ����Ʈ ����Ʈ ������ �ε��ϴ� �Լ�.
CQuestLimitKind* CQuestScriptLoader::LoadQuestLimitKind( char* strKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	LOADUNIT( eQuestLimitKind_Level,	"&LEVEL",		CQuestLimitKind_Level, dwQuestIdx, dwSubQuestIdx );	// ���� ���� ������ �ε��Ͽ� �����Ѵ�.
	LOADUNIT( eQuestLimitKind_Money,	"&MONEY",		CQuestLimitKind_Level, dwQuestIdx, dwSubQuestIdx );	// �Ӵ� ���� ������ �ε��Ͽ� �����Ѵ�.
	LOADUNIT( eQuestLimitKind_Quest,	"&QUEST",		CQuestLimitKind_Quest, dwQuestIdx, dwSubQuestIdx );	// ����Ʈ ���� ������ �ε��Ͽ� �����Ѵ�.
	LOADUNIT( eQuestLimitKind_SubQuest, "&SUBQUEST",	CQuestLimitKind_Quest, dwQuestIdx, dwSubQuestIdx );	// ���� ����Ʈ ���� ������ �ε��Ͽ� �����Ѵ�.
	LOADUNIT( eQuestLimitKind_Stage,	"&STAGE",		CQuestLimitKind_Stage, dwQuestIdx, dwSubQuestIdx );	// �������� ���� ������ �ε��Ͽ� �����Ѵ�.
	// 071011 LYW --- QuestScriptLoader : Add quest limit to check running quest.
	LOADUNIT( eQuestLimitKind_RunningQuest, "&RUNNING_QUEST", CQuestLimitKind_Quest, dwQuestIdx, dwSubQuestIdx ) ; // �������� ����Ʈ ���� ������ �ε��Ͽ� �����Ѵ�.

	return NULL;																							// NULL�� �����Ѵ�.
}

// ����Ʈ ���¸� �ε��Ͽ� �����ϴ� �Լ�.
CQuestCondition* CQuestScriptLoader::LoadQuestCondition( char* strKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	CQuestEvent* pEvent = LoadQuestEvent( strKind, pTokens, dwQuestIdx, dwSubQuestIdx );					// ����Ʈ �̺�Ʈ ������ �ε��Ͽ� �޴´�.

	ASSERT(pEvent);																							// ASSERT üũ�� �߰��Ͽ� �ִ´�.

	CQuestCondition* pCondition = new CQuestCondition( pEvent, dwQuestIdx, dwSubQuestIdx );					// ����Ʈ ���� �޸𸮸� �Ҵ��Ͽ� �����ͷ� �޴´�.

	return pCondition;																						// �����͸� �����Ѵ�.
}

// ����Ʈ �̺�Ʈ�� �ε��Ͽ� �����Ѵ�.
CQuestEvent* CQuestScriptLoader::LoadQuestEvent( char* strKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	LOADUNIT( eQuestEvent_NpcTalk,		"@TALKTONPC",	CQuestEvent, dwQuestIdx, dwSubQuestIdx );			// NPC ��ȭ �̺�Ʈ�� �ε��Ͽ� �����Ѵ�.
	LOADUNIT( eQuestEvent_Hunt,			"@HUNT",		CQuestEvent, dwQuestIdx, dwSubQuestIdx );			// ��� �̺�Ʈ�� �ε��Ͽ� �����Ѵ�.
	LOADUNIT( eQuestEvent_Count,		"@COUNT",		CQuestEvent, dwQuestIdx, dwSubQuestIdx );			// ī��Ʈ �̺�Ʈ�� �ε��Ͽ� �����Ѵ�.
	LOADUNIT( eQuestEvent_GameEnter,	"@GAMEENTER",	CQuestEvent, dwQuestIdx, dwSubQuestIdx );			// ���� ����? �̺�Ʈ�� �ε��Ͽ� �����Ѵ�.
	LOADUNIT( eQuestEvent_Level,		"@LEVEL",		CQuestEvent, dwQuestIdx, dwSubQuestIdx );			// ���� �̺�Ʈ�� �ε��Ͽ� �����Ѵ�.
	LOADUNIT( eQuestEvent_UseItem,		"@USEITEM",		CQuestEvent, dwQuestIdx, dwSubQuestIdx );			// ������ ��� �̺�Ʈ�� �ε��Ͽ� �����Ѵ�.
	LOADUNIT( eQuestEvent_MapChange,	"@MAPCHANGE",	CQuestEvent, dwQuestIdx, dwSubQuestIdx );			// �� ü���� �̺�Ʈ�� �ε��Ͽ� �����Ѵ�.
	LOADUNIT( eQuestEvent_Die,			"@DIE",			CQuestEvent, dwQuestIdx, dwSubQuestIdx );			// ���� �̺�Ʈ�� �ε��Ͽ� �����Ѵ�.
	LOADUNIT( eQuestEvent_HuntAll,		"@HUNTALL",		CQuestEvent, dwQuestIdx, dwSubQuestIdx );			// ��� ��~ �̺�Ʈ�� �ε��Ͽ� �����Ѵ�.

	return NULL;																							// NULL�� �����Ѵ�.
}

// ����Ʈ ������ �ε��Ͽ� �����Ѵ�.
CQuestExecute* CQuestScriptLoader::LoadQuestExecute( char* strKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	LOADUNIT( eQuestExecute_EndQuest,			"*ENDQUEST",			CQuestExecute_Quest, dwQuestIdx, dwSubQuestIdx );	// ����Ʈ ����.
	LOADUNIT( eQuestExecute_StartSub,			"*STARTSUB",			CQuestExecute_Quest, dwQuestIdx, dwSubQuestIdx );	// ���� ����Ʈ ����.
	LOADUNIT( eQuestExecute_EndSub,				"*ENDSUB",				CQuestExecute_Quest, dwQuestIdx, dwSubQuestIdx );	// ���� ����Ʈ ����.
	LOADUNIT( eQuestExecute_EndOtherSub,		"*ENDOTHERSUB",			CQuestExecute_Quest, dwQuestIdx, dwSubQuestIdx );	// �ٸ� ���� ����Ʈ ����.
	
	LOADUNIT( eQuestExecute_AddCount,			"*ADDCOUNT",			CQuestExecute_Count, dwQuestIdx, dwSubQuestIdx );	// ī��Ʈ �߰�.
	LOADUNIT( eQuestExecute_AddCountFQW,		"*ADDCOUNTFQW",			CQuestExecute_Count, dwQuestIdx, dwSubQuestIdx );	// ���� ī��Ʈ �߰�.
	LOADUNIT( eQuestExecute_AddCountFW,			"*ADDCOUNTFW",			CQuestExecute_Count, dwQuestIdx, dwSubQuestIdx );	// ���� ī��Ʈ �߰�.
	LOADUNIT( eQuestExecute_LevelGap,			"*ADDCOUNTLEVELGAP",	CQuestExecute_Count, dwQuestIdx, dwSubQuestIdx );	// ���� ���� ī��Ʈ �߰�.
	LOADUNIT( eQuestExecute_MonLevel,			"*ADDCOUNTMONLEVEL",	CQuestExecute_Count, dwQuestIdx, dwSubQuestIdx );	// ���� ���� ī��Ʈ �߰�.

	LOADUNIT( eQuestExecute_GiveQuestItem,		"*GIVEQUESTITEM",		CQuestExecute_Item, dwQuestIdx, dwSubQuestIdx );	// ����Ʈ ������ �ݳ�.
	LOADUNIT( eQuestExecute_TakeQuestItem,		"*TAKEQUESTITEM",		CQuestExecute_Item, dwQuestIdx, dwSubQuestIdx );	// ����Ʈ ������ ȹ��.
	LOADUNIT( eQuestExecute_GiveItem,			"*GIVEITEM",			CQuestExecute_Item, dwQuestIdx, dwSubQuestIdx );	// ������ �ݳ�.
	LOADUNIT( eQuestExecute_GiveMoney,			"*GIVEMONEY",			CQuestExecute_Item, dwQuestIdx, dwSubQuestIdx );	// �Ӵ� �ݳ�.
	LOADUNIT( eQuestExecute_TakeItem,			"*TAKEITEM",			CQuestExecute_Item, dwQuestIdx, dwSubQuestIdx );	// ������ ȹ��.
	LOADUNIT( eQuestExecute_TakeMoney,			"*TAKEMONEY",			CQuestExecute_Item, dwQuestIdx, dwSubQuestIdx );	// �Ӵ� ȹ��.
	LOADUNIT( eQuestExecute_TakeExp,			"*TAKEEXP",				CQuestExecute_Item, dwQuestIdx, dwSubQuestIdx );	// ����ġ ȹ��.
	LOADUNIT( eQuestExecute_TakeSExp,			"*TAKESEXP",			CQuestExecute_Item, dwQuestIdx, dwSubQuestIdx );	// SP ȹ��.
	LOADUNIT( eQuestExecute_TakeQuestItemFQW,	"*TAKEQUESTITEMFQW",	CQuestExecute_Item, dwQuestIdx, dwSubQuestIdx );	// ����Ʈ ������ ���� ȹ��.
	LOADUNIT( eQuestExecute_TakeQuestItemFW,	"*TAKEQUESTITEMFW",		CQuestExecute_Item, dwQuestIdx, dwSubQuestIdx );	// ����Ʈ ������ ���� ȹ��.
	LOADUNIT( eQuestExecute_TakeMoneyPerCount,	"*TAKEMONEYPERCOUNT",	CQuestExecute_Item, dwQuestIdx, dwSubQuestIdx );	// ī��Ʈ �� �Ӵ� ȹ��.
	
	LOADUNIT( eQuestExecute_RandomTakeItem,		"*RANDOMTAKEITEM",		CQuestExecute_RandomItem, dwQuestIdx, dwSubQuestIdx );	// ���� ȹ�� ������.

	LOADUNIT( eQuestExecute_RegenMonster,		"*REGENMONSTER",		CQuestExecute_Quest, dwQuestIdx, dwSubQuestIdx );		// ���� ����.
	LOADUNIT( eQuestExecute_MapChange,			"*MAPCHANGE",			CQuestExecute_Quest, dwQuestIdx, dwSubQuestIdx );		// �� ü����.
	
	LOADUNIT( eQuestExecute_ChangeStage,		"*CHANGESTAGE",			CQuestExecute_Quest, dwQuestIdx, dwSubQuestIdx );		// �������� ü����.
	
	LOADUNIT( eQuestExecute_RegistTime,			"*REGISTTIME",			CQuestExecute_Time, dwQuestIdx, dwSubQuestIdx );		// �ð� ���.

	return NULL;	
}

// ����Ʈ NPC ��ũ��Ʈ �����͸� �ε��Ͽ� �����ϴ� �Լ�.
CQuestNpcScriptData* CQuestScriptLoader::LoadQuestNpcScriptData( char* strKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	LOADUNIT( 0, "@NPC", CQuestNpcScriptData, dwQuestIdx, dwSubQuestIdx );	// ����Ʈ NPC ��ũ��Ʈ �����͸� �ε��Ͽ� �����Ѵ�.
	return NULL;															// NULL�� �����Ѵ�.
}

// ����Ʈ NPC �����͸� �ε��Ͽ� �����ϴ� �Լ�.
CQuestNpcData* CQuestScriptLoader::LoadQuestNpcData( char* strKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	LOADUNIT( 0, "@NPC", CQuestNpcData, dwQuestIdx, dwSubQuestIdx );		// NPC �����͸� �ε��Ͽ� �����Ѵ�.
	return NULL;															// NULL�� �����Ѵ�.
}
