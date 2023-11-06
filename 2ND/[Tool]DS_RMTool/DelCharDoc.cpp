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
// DelCharDoc.cpp : ���� �����Դϴ�.
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

// CDelCharDoc �����Դϴ�.

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


// CDelCharDoc serialization�Դϴ�.

void CDelCharDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}


// CDelCharDoc ����Դϴ�.

void CDelCharDoc::OnCloseDocument()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

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
	case MP_RMTOOL_DELCHARLIST_ACT:	// ������ ĳ���� ��ȸ���
		{
			TMSG_DELCHARLISRESULT* pmsg = (TMSG_DELCHARLISRESULT*)pMsg;
			pView->SetDelCharList( pmsg->Count, pmsg );
		}
		break;
	case MP_RMTOOL_DELCHARLIST_NACK:// ������ ĳ���� ��ȸ ��� ����
		{
			pView->MessageBox("No Data!!", "Error");
		}
		break;
	}
}

void CDelCharDoc::OpenData()
{
	// ��Ʈ�� �ʱ�ȭ ����..
	CDelCharView* pView = (CDelCharView*)GetView(0);
}
