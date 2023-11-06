// cMonsterSpeechManager.cpp: implementation of the cMonsterSpeechManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cMonsterSpeechManager.h"
#include "MHTimeManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(cMonsterSpeechManager)

cMonsterSpeechManager::cMonsterSpeechManager()
{
//	srand((unsigned)time(NULL));
	Init();
}

cMonsterSpeechManager::~cMonsterSpeechManager()
{
	Release();
}

void cMonsterSpeechManager::Init()
{

	m_MonSpeechInfoTable.Initialize(100);
//	m_MonSpeechTable.Initialize(100);
}

void cMonsterSpeechManager::Release()
{
	ReleaseSpeechInfoList();
//	ReleaseSpeechList();
}
	
bool cMonsterSpeechManager::LoadMonSpeechInfoList()
{
	CMHFile fp;
	char szBuf[256] = {0,};

	char filename[256];

	sprintf(filename,"System/Resource/MonsterSpeechInfoList.bin");
	if( !fp.Init( filename, "rb" ) )
		return FALSE;

	int num = 0;
	int eSpeechKind = -1;
	MonSpeech* pMonSpeech = NULL;
	
	while(TRUE)
	{
		//��ũ���� ���� or �ִ뷮 ����
		if( fp.IsEOF() ) //||num == MAX_MONSTER_NUM )
			break;

		if( strcmp(szBuf, "#KIND") != 0 )
		{
			fp.GetString(szBuf);
		}
		//�ּ� ó��
		if(szBuf[0] == '@')
		{
			fp.GetLineX(szBuf, 256);
			continue;
		}

		//�빮��ȭ
//		strcpy(szBuf,strupr(szBuf));
		strupr(szBuf);

		//���� & ����
		if( strcmp(szBuf, "#KIND") == 0 )
		{
			pMonSpeech = new MonSpeech;
			//memset(pMonSpeech, 0, sizeof(MonSpeech));	//�ʱ�ȭ ����.

			pMonSpeech->MonsterIdx = fp.GetDword();
			
			pMonSpeech->MonsterType = fp.GetDword();

			while(TRUE)
			{
				fp.GetString(szBuf);

				strupr(szBuf);

				if( strcmp(szBuf, "#FOREATK" ) == 0 )
				{
					eSpeechKind = eMon_Speech_ForeAtk;
				}
				else if( strcmp(szBuf, "#FORGIVE" ) == 0 )
				{
					eSpeechKind = eMon_Speech_ForgivePursuit;
				}
				else if( strcmp(szBuf, "#HELP" ) == 0 )
				{
					eSpeechKind = eMon_Speech_Help;
				}
				else if( strcmp(szBuf, "#FORHELP" ) == 0 )
				{
					eSpeechKind = eMon_Speech_AboutHelp;
				}
				else if( strcmp(szBuf, "#DEATH" ) == 0 )
				{
					eSpeechKind = eMon_Speech_Death;
				}
				else if( strcmp(szBuf, "#STAND" ) == 0 )
				{
					eSpeechKind = eMon_Speech_Stand;
				}
				else if( strcmp(szBuf, "#WALK" ) == 0 )
				{
					eSpeechKind = eMon_Speech_WalkAround;
				}
				else if( strcmp(szBuf, "#KSTAND" ) == 0 )
				{
					eSpeechKind = eMon_Speech_KeepStand;
				}
				else if( strcmp(szBuf, "#KWALK" ) == 0 )
				{
					eSpeechKind = eMon_Speech_KeepWalkAround;
				}
				else if( strcmp(szBuf, "#KIND" ) == 0 || strcmp(szBuf, "#END") == 0 )
				{
					break;
				}

				ASSERT(eSpeechKind > -1);
				SetSpeechInfo(pMonSpeech, eSpeechKind, &fp);
			}
			ASSERT(!m_MonSpeechInfoTable.GetData(pMonSpeech->MonsterIdx));
			m_MonSpeechInfoTable.Add(pMonSpeech, pMonSpeech->MonsterIdx);
			++num;
		}		
	}//while

	fp.Release();
	return TRUE;
}

void cMonsterSpeechManager::ReleaseSpeechInfoList()
{
	MonSpeech* pList = NULL;
	m_MonSpeechInfoTable.SetPositionHead();
	while( pList = m_MonSpeechInfoTable.GetData() )
	{
		for( int i = 0; i < eMon_Speech_MAX; ++i )
		{
			if(pList->pSpchList[i])
			{
				delete [] pList->pSpchList[i];
				pList->pSpchList[i] = NULL;
			}//SAFE_DELETE
		}
		delete pList;
		pList = NULL;
	}
	m_MonSpeechInfoTable.RemoveAll();
}

MonSpeechInfo* cMonsterSpeechManager::GetCurStateSpeechIndex(DWORD MonKind, DWORD StateKind)
{
	MonSpeech* pMonSpeech = m_MonSpeechInfoTable.GetData(MonKind);

	if( !pMonSpeech ) return NULL; //��ȿ�Ѱ�?

	if( pMonSpeech->dwProbNoSpeech[StateKind] == 100 ) return NULL;
	ASSERT( pMonSpeech->dwSpeechTotalRate[StateKind] >= 0 );
	
	if( pMonSpeech->dwSpchListCount[StateKind] == 0 ) //�ش� ��� ī��Ʈ�� 0 �̸�
		return NULL;
	
	DWORD Rnd = rand()%100;
	
	if( Rnd < pMonSpeech->dwProbNoSpeech[StateKind] )	//��� ���� Ȯ��
		return NULL;

	DWORD SpeechRate = random(0, pMonSpeech->dwSpeechTotalRate[StateKind]);
	if( !SpeechRate )	return NULL;

	DWORD MinRate = 0;
	DWORD MaxRate = 0;
	for( DWORD i = 0; i < pMonSpeech->dwSpchListCount[StateKind]; ++i )
	{
		MinRate = MaxRate;
		MaxRate += pMonSpeech->pSpchList[StateKind][i].Prob;

		if( MinRate < SpeechRate && SpeechRate <= MaxRate )		//��� Ȯ�� ��
		{
			//���� Ÿ��(�Ϲ�, �߰�����, ����)�� ���� ����ü �ȿ� �����Ƿ�..ó���� �ָ��ϴ�.
			//���� Ÿ���̶�� ���� ä�� Ÿ������ ����!
			m_SpeechInfo.SpeechType = pMonSpeech->MonsterType;
			m_SpeechInfo.SpeechIndex = pMonSpeech->pSpchList[StateKind][i].SpeechIdx;
			if(m_SpeechInfo.SpeechIndex == 0)	return NULL;	//��ũ���� �Ǽ��� ���� ����ó��.
				
			return &m_SpeechInfo;
		}
	}
/*
#ifdef _DEBUG
	if(SpeechRate)
	{
		char buf[128];
		sprintf(buf, "!MonsterSpeechError_NonSpeech!	MonsterIdx: %d\t StateKind: %d", MonKind, StateKind);
		ASSERTMSG(0, buf);	
	}
#endif*/
	return NULL;
}


void cMonsterSpeechManager::SetSpeechInfo(MonSpeech* pMonSpeech, DWORD StateKind, CMHFile* fp)
{
	DWORD	dwSpeechNum = fp->GetDword();	//��� ����
	pMonSpeech->dwSpchListCount[StateKind] = dwSpeechNum;

	pMonSpeech->dwProbNoSpeech[StateKind] = fp->GetDword();	//��� ���� Ȯ��

	char szBuf[512];
	if( !dwSpeechNum  || pMonSpeech->dwProbNoSpeech[StateKind] >= 100 )
	{

		fp->GetLineX(szBuf, 512);		
		return;
	}

	pMonSpeech->pSpchList[StateKind] = new MonSpeechList[dwSpeechNum];

	for( DWORD i = 0; i < dwSpeechNum; ++i )
	{
		pMonSpeech->pSpchList[StateKind][i].Prob = fp->GetDword();
		pMonSpeech->pSpchList[StateKind][i].SpeechIdx = fp->GetDword();
		pMonSpeech->dwSpeechTotalRate[StateKind] += pMonSpeech->pSpchList[StateKind][i].Prob;
	}
	
	fp->GetLineX(szBuf,512);

}