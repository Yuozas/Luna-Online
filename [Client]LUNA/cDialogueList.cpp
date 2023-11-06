
#include "stdafx.h"																	// 표준 시스템과, 프로젝트가 지정한, 자주쓰는 해더들을 모은 해더파일을 불러온다.

#include "cDialogueList.h"															// 다이얼로그 리스트 클래스 헤더를 불러온다.


cDialogueList::cDialogueList()														// 생성자 함수.
{
	m_dwDefaultColor = NORMAL_COLOR;												// 일반 색상을 세팅한다.
	m_dwStressColor = STRESS_COLOR;													// 스트레스 색상을 세팅한다.
}

cDialogueList::~cDialogueList()														// 소멸자 함수.
{
	for(int i=0;i<MAX_DIALOGUE_COUNT;++i)											// 최대 다이얼로그 카운트 만큼, for문을 돌린다.
	{
		PTRLISTSEARCHSTART(m_Dialogue[i],DIALOGUE*,Msg)								// 다이얼로그 배열에서, 카운트에 해당하는 다이얼로그 정보를 찾는다.
			delete Msg;																// 다이얼로그 정보를 삭제한다.
		PTRLISTSEARCHEND															// 찾기 종료.
			
		m_Dialogue[i].RemoveAll();													// 카운트에 해당하는 다이얼로그 리스트를 모두 비운다.
	}
}

void cDialogueList::LoadDialogueListFile(char* filePath, char* mode)				// 파일로 부터 다이얼로그 정보를 로딩하는 함수.
{
	CMHFile fp;																		// 묵향 파일을 선언한다.

	if(!fp.Init(filePath, mode))													// 들어온 경로와 모드로 파일을 열기가 실패하면,
	{
		return;																		// 리턴 처리를 한다.
	}

	char buff[128]={0,};															// 임시 버퍼를 선언한다.

	while(1)																		// while문을 돌린다.
	{
		fp.GetString(buff);															// 임시버퍼로 스트링을 읽어들인다.

		if(fp.IsEOF())																// 파일포인터가 파일의 끝을 가리키면,
		{
			break;																	// while문을 탈출한다.
		}

		if(buff[0] == '@')															// 임시버퍼의 첫 글자가 @와 같으면,
		{
			fp.GetLineX(buff, 128);													// 임시 버퍼로 라인을 읽어들인다.
			continue;																// 계속 while문 진행.
		}

		CMD_ST(buff)																// 임시 버퍼를 확인한다.

		CMD_CS("#Msg")																// 버퍼가 #Msg와 같다면,
			DWORD dwMsgId = fp.GetDword();											// 메시지 아이디를 받는다.

			if((fp.GetString())[0] == '{')											// 다음 스트링을 읽어 첫 글자가 {와 같으면,
			{
				LoadDialogueList(dwMsgId, &fp);										// 다이얼로그 리스트 정보를 로딩한다.
			}
			else																	// 그렇지 않을 경우,
				__asm int 3;
		CMD_CS("#TEXTCOLOR")														// 임시 버퍼가 #TEXTCOLOR와 같으면,
			m_dwDefaultColor = RGB( fp.GetDword(), fp.GetDword(), fp.GetDword() );	// 기본 색상을 읽어들인다.
			m_dwStressColor = RGB( fp.GetDword(), fp.GetDword(), fp.GetDword() );	// 스트레스 색상을 읽어들인다.
		CMD_EN
	}
}

void cDialogueList::LoadDialogueList(DWORD dwId, CMHFile* fp)						// 다이얼로그 리스트 정보를 로딩하는 함수.
{
	char buff[1024];																// 임시 버퍼를 선언한다.

	while(1)																		// while문을 돌린다.
	{
		fp->GetLine( buff, 1024 );													// 임시 버퍼로 라인을 읽어들인다.

		if( *buff == '}' || fp->IsEOF() )											// 버퍼가 } 거나 파일의 끝이면, 
		{
			break;																	// while문을 탈출한다.
		}

		ParsingLine( dwId, buff );													// 임시 버퍼의 내용을 파싱한다.
	}
}

#define NPCDIALOG_TEXTLEN	50														// NPC 다이얼로그의 텍스트 길이를 36으로 디파인한다.

void cDialogueList::ParsingLine( DWORD dwId, char* buf )							// 라인 정보를 파싱하는 함수.
{
	DWORD dwColor	= m_dwDefaultColor;												// 일반 컬러를 세팅한다.
	WORD wLine		= m_Dialogue[dwId].GetCount();									// 라인 카운트를 세팅한다.

	char wBuff[512] = {0,};															// 임시 버퍼를 선언한다.

	int	nCut = 0;																	// 커팅 정보를 담을 변수를 선언하고 0으로 세팅한다.
	int nStrLen = 0;																// 문자열 길이을 담을 변수를 선언하고 0으로 세팅한다.

	while( *buf )																	// 버퍼의 정보가 유효하면, while문을 돌린다.
	{
		if( *buf == '$' )															// 버퍼가 '$'와 같으면,
		{
			if( nStrLen != 0 )														// 문자열 길이가 0과 같지 않으면,
			{
				wBuff[nStrLen] = 0;													// 임시 버퍼의 길이 위치의 값을 0으로 세팅한다.
				AddLine( dwId, wBuff, dwColor, wLine, emLink_Null );				// 라인을 추가한다.
				memset( wBuff, 0, 512 );											// 임시 버퍼를 메모리 셋 한다.
				nStrLen = 0;														// 문자열 길이를 0으로 세팅한다.
			}

			buf += 2;																// 버퍼 포인터를 2증가한다.

			if( *buf == 's' || *buf == 'S' )										// 버퍼의 글자가 s또는 S이면,
			{
				dwColor = m_dwStressColor;											// 색상을 스트레스 색상으로 세팅한다.
			}
			else																	// 그렇지 않을 경우,
			{
				dwColor = m_dwDefaultColor;											// 일반 색상으로 세팅한다.
			}

			++buf;																	// 버퍼 포인터를 증가한다.

			continue;																// while 계속 진행.
		}

		wBuff[nStrLen] = *buf;														// 임시 버퍼의 문자열 길이의 위치에 버퍼의 값을 세팅한다.

		if( IsDBCSLeadByte( *buf ) )												// 버퍼의 값이 한글 바이트의 첫 바이트라면,
		{
			++buf;																	// 버퍼 포인터를 증가한다.
			++nStrLen;																// 문자열 길이를 증가한다.
			++nCut;																	// 컷팅 길이를 증가한다.
			wBuff[nStrLen] = *buf;													// 임시 버퍼의 문자열 길이에 버퍼의 값을 세팅한다.
		}

		++nCut;																		// 컷팅 정보를 증가한다.
		++buf;																		// 버퍼 포인터를 증가한다.
		++nStrLen;																	// 문자열 길이를 증가한다.

		if( nCut >= NPCDIALOG_TEXTLEN )												// 컷트 정보가 npc 다이얼로그 텍스트 길이보가 이상이면,
		{
			if( nStrLen != 0 )														// 문자열 길이가 0이 아이념,
			{	
				wBuff[nStrLen] = 0;													// 임시 버퍼의 문자열 길이 위치에 값을 0으로 세팅한다.
				AddLine( dwId, wBuff, dwColor, wLine, emLink_Null );				// 라인을 추가한다.
				nStrLen = 0;														// 문자열 길이를 0으로 세팅한다.
			}

			nCut = 0;																// 컷트 정보를 0으로 세팅한다.

			++wLine;																// 라인 카운트를 증가한다.

			if( *buf == ' ' ) ++buf;												// 버퍼의 값이 빈칸과 같으면 버퍼 포인터를 증가한다.
		}
	}

	if( nStrLen != 0 )																// 문자열 길이가 0과 같지 않으면,
	{
		wBuff[nStrLen] = 0;															// 임시 버퍼의 문자열 길이의 위치 값을 0으로 세팅한다.
		AddLine( dwId, wBuff, dwColor, wLine, emLink_Null );						// 라인을 추가한다.
	}
}

DIALOGUE* cDialogueList::GetDialogue( DWORD dwMsgId, WORD wLine )					// 다이얼로그 정보를 반환하는 함수.
{
	if( m_Dialogue[dwMsgId].IsEmpty() ) return NULL;								// 메시지 아이디에 해당하는 정보가 없으면 NULL 리턴 처리를 한다.

	PTRLISTPOS p = m_Dialogue[dwMsgId].FindIndex(wLine);							// 다이얼로그 리스트에서 라인의 위치 정보를 받는다.

	if( p == NULL ) return NULL;													// 위치 정보가 유효하지 않으면 NULL 리턴 처리를 한다.

	return (DIALOGUE*)m_Dialogue[dwMsgId].GetAt(p);									// 다이얼로그 정보를 리턴한다.
}


void cDialogueList::AddLine( DWORD dwId, char* str, DWORD color, WORD Line, WORD type)	// 라인 추가 함수.
{
	DIALOGUE* p = new DIALOGUE;														// 다이얼로그 메모리를 할당한다.

	p->Init();																		// 다이얼로그 정보를 초기화 한다.

	p->dwColor = color;																// 색상을 세팅한다.

	strcpy( p->str, str );															// 문자열을 세팅한다.

	p->wLine = Line;																// 라인을 세팅한다.
	p->wType = type;																// 타입을 세팅한다.

	m_Dialogue[dwId].AddTail( p );													// 다이얼로그 리스트에 추가한다.
}

/*
void cDialogueList::ParsingLine( DWORD dwId, char* buf )
{
	DWORD dwColor = m_dwDefaultColor;
	WORD wLine = m_Dialogue[dwId].GetCount();
	unsigned int nLen = 0;

	char wBuff[256]={0,};

	int i = 0;

	// °ËÃâÇØ¼­ ¸¸µç´Ù.
	while(1)
	{
		if( (*buf) == '\0' )
		{
			// ÀúÀå ¸ñ·ÏÀÌ ÀÖÀ¸¸é ÀúÀå
			if( nLen = strlen(wBuff) )
			{
				AddLine( dwId, wBuff, dwColor, wLine, emLink_Null );
//				memset(wBuff, 0, 256);
//				i = 0;
			}
			break;
		}

		if((*buf) == '$')
		{
			// ÀúÀå ¸ñ·ÏÀÌ ÀÖÀ¸¸é ÀúÀå
			if( nLen = strlen(wBuff) )
			{
				AddLine( dwId, wBuff, dwColor, wLine, emLink_Null );
				memset(wBuff, 0, 256);
				i = 0;
			}

			char flag[4]={0,};
			memcpy(flag, buf, 3);
			buf += 3;
			
			if( strcmp(flag, "$cs") == 0 )
			{
//				dwColor = STRESS_COLOR;
				dwColor = m_dwStressColor;
			}
			if( strcmp(flag, "$ce") == 0 )
			{
//				dwColor = NORMAL_COLOR;
				dwColor = m_dwDefaultColor;
			}
		}
		wBuff[i++] = *buf;

		++buf;
	}
}
*/