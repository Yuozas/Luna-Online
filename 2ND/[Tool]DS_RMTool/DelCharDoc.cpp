// DelCharDoc.cpp : implementation file
//

#include "stdafx.h"

#include "DS_RMTool.h"
#include "DelCharDoc.h"
#include "DelChar.h"
#include "RMNetwork.h"
#include "DelCharView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DelCharDoc.cpp : 구현 파일입니다.
//

IMPLEMENT_DYNCREATE(CDelCharDoc, CBaseDoc)

CDelCharDoc::CDelCharDoc()
{
}

BOOL CDelCharDoc::OnNewDocument()
{
	if (!CBaseDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

CDelCharDoc::~CDelCharDoc()
{
}

BEGIN_MESSAGE_MAP(CDelCharDoc, CBaseDoc)
END_MESSAGE_MAP()

// CDelCharDoc 진단입니다.

#ifdef _DEBUG
void CDelCharDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDelCharDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDelCharDoc serialization입니다.

void CDelCharDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CDelCharDoc 명령입니다.

void CDelCharDoc::OnCloseDocument()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CBaseDoc::OnCloseDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CAdminDoc commands

void CDelCharDoc::NetworkMsgParse(BYTE Protocol, void* pMsg)
{

	CDelCharView* pView = (CDelCharView*)GetView(0);
	if( !pView )	return;

	switch(Protocol)
	{
	case MP_RMTOOL_DELCHARLIST_ACT:	// 삭제된 캐릭터 조회결과
		{
			TMSG_DELCHARLISRESULT* pmsg = (TMSG_DELCHARLISRESULT*)pMsg;
			pView->SetDelCharList( pmsg->Count, pmsg );
		}
		break;
	case MP_RMTOOL_DELCHARLIST_NACK:// 삭제된 캐릭터 조회 결과 오류
		{
			pView->MessageBox("No Data!!", "Error");
		}
		break;
	}
}

void CDelCharDoc::OpenData()
{
	// 컨트롤 초기화 지시..
	CDelCharView* pView = (CDelCharView*)GetView(0);
}
