#pragma once


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers


// Windows Header Files:
#include <windows.h>
#include <winsock2.h>
#include <ole2.h>
#include <assert.h>

// C RunTime Header Files
#include <cstdlib>
#include <cstdio>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
#include <hash_map>
#include <hash_set>
#include <iostream>
#include <conio.h>
#include <fstream>


struct VECTOR3	{float x,y,z;};
extern DWORD gCurTime;
struct USERINFO;


#define _CRYPTCHECK_


#include "..\..\[lib]yhlibrary\HSEL.h"
#include "..\..\[CC]Header\Protocol.h"
#include "..\..\[CC]Header\CommonDefine.h"
#include "..\..\[CC]ServerModule\DataBase.h"
#include "..\..\[CC]Header\CommonGameDefine.h"
#include "..\..\[CC]Header\CommonGameStruct.h"
#include "..\..\[CC]Header\CommonStruct.h"
#include "..\..\[CC]Header\ServerGameStruct.h"
#include "..\common\common.h"
#include "MHFile.h"


extern void WriteAssertMsg(TCHAR* pStrFileName,int Line, TCHAR* pMsg);

void PutLog( const TCHAR*, ... );