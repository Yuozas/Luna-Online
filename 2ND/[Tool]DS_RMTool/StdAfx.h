// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__E6037B9D_5BE5_449E_AF68_5BBA36E5ABCD__INCLUDED_)
#define AFX_STDAFX_H__E6037B9D_5BE5_449E_AF68_5BBA36E5ABCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#ifndef _RMTOOL_ 
#define _RMTOOL_ 
#endif

#ifndef _FILE_BIN_
#define _FILE_BIN_
#endif

#define _CRYPTCHECK_

#include "mmsystem.h"

#include <winsock2.h>
#include "yhlibrary.h"

struct VECTOR3	{float x,y,z;};
extern DWORD gCurTime;

#include "..\[CC]Header\protocol.h"
#include "..\[CC]Header\CommonDefine.h"
#include "..\[CC]Header\CommonGameDefine.h"
#include "..\[CC]Header\CommonGameStruct.h"
#include "..\[CC]Header\CommonStruct.h"

#include "..\[CC]Header\ServerGameDefine.h"

#undef	_SERVER_RESOURCE_FIELD_

#include "..\[CC]Ability\AbilityCommonHeader.h"
#include "..\[CC]Header\ServerGameStruct.h"
#include "..\[CC]Header\CommonStructMS.h"
#include "..\[CC]Header\GameResourceStruct.h"
#include "..\[CC]Header\CommonCalcFunc.h"

#include "ResourceManager.h"
#include "CommonToolDefine.h"



#include "basedoc.h"

void SafeStrCpy( char* pDest, const char* pSrc, int nDestBufSize );
void NetworkMsgParse(BYTE Category, BYTE Protocol, void *pMsg);

// 버전
#ifdef _CHINA_LOCAL_
//	#define		RMTOOLVERSION		"CHINA20051125"
	#define		RMTOOLVERSION		"CHINA20051216"
#elif defined _JAPAN_LOCAL_
	#define		RMTOOLVERSION		"JAPAN20051001"	
#else
//	#define		RMTOOLVERSION		"OPEN20051101"	
//	#define		RMTOOLVERSION		"OPEN06032101"
//	#define		RMTOOLVERSION		"OPEN06041801"
//	#define		RMTOOLVERSION		"OPEN06070401"  // 펫 신규 추가
//	#define		RMTOOLVERSION		"OPEN06071901" // 펫 관련 수정
	#define		RMTOOLVERSION		"OPEN06080801" // 펫 관련 로그 추가
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E6037B9D_5BE5_449E_AF68_5BBA36E5ABCD__INCLUDED_)
