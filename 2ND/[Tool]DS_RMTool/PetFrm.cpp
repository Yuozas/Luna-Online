// PetFrm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "PetFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPetFrm

IMPLEMENT_DYNCREATE(CPetFrm, CMDIChildWnd)

CPetFrm::CPetFrm()
{
}

CPetFrm::~CPetFrm()
{
}


BEGIN_MESSAGE_MAP(CPetFrm, CMDIChildWnd)
END_MESSAGE_MAP()


// CPetFrm 메시지 처리기입니다.
BOOL CPetFrm::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	cs.cx = 830;
	cs.cy = 700;
	cs.lpszName = "Pet Info";

	return TRUE;
}
