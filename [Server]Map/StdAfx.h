#pragma once


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _WIN32_WINNT	0x0500 
#define _CRYPTCHECK_

//#define TAIWAN_LOCAL //pjslocal

//#define	_FILE_BIN_
#pragma warning(disable : 4786)

#include <windows.h>
#include <winsock2.h>
#include <ole2.h>
#include <initguid.h>
#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <crtdbg.h>
#include <time.h>
#include <tchar.h>

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
#include <stack>
#include <conio.h>
 
#include "..\[lib]yhlibrary\yhlibrary.h"
#include "..\[lib]yhlibrary\HashTable.h"
#include "..\[lib]yhlibrary\PtrList.h"
#include "..\[lib]yhlibrary\cLooseLinkedList.h"
#include "..\[lib]yhlibrary\cLinkedList.h"
#include "..\[lib]yhlibrary\IndexGenerator.h"
#include "..\[lib]yhlibrary\cConstLinkedList.h"
#include "..\[CC]Header\vector.h"
#include "..\[CC]Header\protocol.h"
#include "..\[CC]Header\CommonDefine.h"
#include "..\[CC]Header\CommonGameDefine.h"
#include "..\[CC]Header\ServerGameDefine.h"
#include "..\[CC]Header\CommonGameStruct.h"
#include "..\[CC]Header\CommonStruct.h"
#include "..\[CC]Header\ServerGameStruct.h"
#include "..\[CC]Header\CommonGameFunc.h"
#include "..\[CC]Header\CommonCalcFunc.h"

#include "DataBase.h"
#include "Console.h"
#include "ServerSystem.h"