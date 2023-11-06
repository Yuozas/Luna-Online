// PetFrm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "DelCharFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDelCharFrm

IMPLEMENT_DYNCREATE(CDelCharFrm, CMDIChildWnd)

CDelCharFrm::CDelCharFrm()
{
}

CDelCharFrm::~CDelCharFrm()
{
}


BEGIN_MESSAGE_MAP(CDelCharFrm, CMDIChildWnd)
END_MESSAGE_MAP()


// CDelCharFrm 메시지 처리기입니다.
BOOL CDelCharFrm::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	cs.cx = 1024;
	cs.cy = 700;
	cs.lpszName = "Delete Character List";

	return TRUE;
}
