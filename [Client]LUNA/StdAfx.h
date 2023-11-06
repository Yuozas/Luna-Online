#pragma once

#define _WIN32_WINNT  0x0500
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _FOR_MUKHYANG
#define _CLIENTVERSIONCHK_
//pjslocal
//#define TAIWAN_LOCAL

#define _FILE_BIN_

#define _CLIENT_
#define _CRYPTCHECK_

#ifndef _SEMIRELEASE
#define _CHEATENABLE_
#endif
#define _ASSERTENABLE_



#pragma warning(disable:4786)

// Windows Header Files:
#include <windows.h>
#include <ole2.h>
#include <initguid.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <crtdbg.h>
#include <vfw.h>
#include <dbghelp.h>
#include <time.h>
#include <shellapi.h.>
#include <direct.h>
#include "resource.h"
#include <wingdi.h>
// 080610 LUJ, GM툴만 콘솔 출력하도록 함
#ifdef _GMTOOL_
#include <conio.h>
#endif

// data structure
#include "..\[lib]yhlibrary\yhlibrary.h"
#include "..\[lib]yhlibrary\cLinkedList.h"
#include "..\[lib]yhlibrary\PtrList.h"
#include "..\[lib]yhlibrary\IndexGenerator.h"
#include <map>
#include <set>
#include <hash_map>
#include <hash_set>
#include <list>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <functional>
#include <queue>

// TODO: reference additional headers your program requires here


#include "..\4DyuchiGXGFunc\global.h"
#include "..\[CC]Header\protocol.h"
#include "..\[CC]Header\CommonDefine.h"
#include "..\[CC]Header\CommonGameDefine.h"
#include "..\[CC]Header\ClientGameDefine.h"
#include "..\[CC]Header\CommonGameStruct.h"
#include "..\[CC]Header\CommonStruct.h"
#include "..\[CC]Header\ClientGameStruct.h"
#include "..\[CC]Header\CommonGameFunc.h"

#include "Motion.h"
#include "MHNetwork.h"
#include "ErrorMsg.h"
#include "DirectoryManager.h"


extern BOOL g_bUsingEnginePack;