// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers


// Windows Header Files:
#include <windows.h>
#include <winsock2.h>
#include <ole2.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>

struct VECTOR3	{float x,y,z;};
extern DWORD gCurTime;

#define _CRYPTCHECK_

// Local Header Files
#include "../[Lib]YhLibrary/YHLibrary.h"
#include "Protocol.h"
#include "CommonDefine.h"
#include "MSDefine.h"
#include "CommonGameDefine.h"
#include "CommonGameStruct.h"
#include "CommonStruct.h"
#include "ServerGameStruct.h"
#include "CommonStructMS.h"
#include "..\4DYUCHINET_COMMON\INetwork_GUID.h"

extern void LogConsole(char * buff,...);
extern void WriteAssertMsg(char* pStrFileName,int Line,char* pMsg);
extern void LOG( char* msg, ... );

// TODO: reference additional headers your program requires here
/// 중국
#ifdef _CHINA_LOCAL_
	#define		SERVERTOOLVERSION	"OPEN20050601"
	#define		RMTOOLVERSION		"CHINA20051216"
/// 일본
#elif defined _JAPAN_LOCAL_
	#define		SERVERTOOLVERSION	"JAPAN20051001"
	#define		RMTOOLVERSION		"JAPAN20051001"	
/// 태국
#elif defined _TL_LOCAL_
	#define		SERVERTOOLVERSION	"TL2006033001"
	#define		RMTOOLVERSION		"TL2006033001"	
#elif defined _HK_LOCAL_
/// 대만
	#ifdef _TW_LOCAL_
//		#define		SERVERTOOLVERSION	"HK2006021701"
//		#define		RMTOOLVERSION		"HK2006060101"	
//		#define		SERVERTOOLVERSION	"TW2006091101"
		#define		RMTOOLVERSION		"TW2006091101"
		#define		SERVERTOOLVERSION	"TW2006091901"
/// 홍콩
	#else
//		#define		SERVERTOOLVERSION	"HK2006021701"
//		#define		RMTOOLVERSION		"HK2006060101"
//		#define		SERVERTOOLVERSION	"HK2006091101"
		#define		RMTOOLVERSION		"HK2006091101"
		#define		SERVERTOOLVERSION	"HK2006091901"
	#endif
/// 한국
#else
	#define		SERVERTOOLVERSION	"OPEN06041801"
	#define		RMTOOLVERSION		"OPEN06080801" // 펫 관련 로그 추가
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
