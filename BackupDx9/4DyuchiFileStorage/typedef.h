#pragma once

#include "../4DyuchiGRX_Common/IFileStorage.h"
#define MAX_KEY_LENGTH		512
#define MAX_CHARKEY_LENGTH		2048
#define CURRENT_VERSION		0x00000001
class CPackFile;


struct FSFILE_DESC
{
	DWORD			dwTotalSize;
	DWORD			dwPositionInPackFile;

	DWORD			dwRealFileSize;
			
	BOOL			bSourceFromPackFile;	// TRUE이면 팩파일로부터 생성한 디스크립터, FALSE이면 낱개의 파일로부터 생성한 디스크립터.
	FSFILE_DESC*	pPrvInPackFile;
	FSFILE_DESC*	pNextInPackFile;

	char*			pEntryInMemory;
	DWORD			dwOpenCount;


	void*			pHashHandle;
	CPackFile*		pOwnerPackFile;

	char*			pszFileName;				// full path 파일명
	DWORD			dwFileNameLen;			// full path 파일명 길이
};
struct FSFILE_POINTER
{
	BOOL			bFromPack;
	FILE*			fp;
	DWORD			dwSize;
	DWORD			dwOffset;
	FSFILE_DESC*	pFSDesc;
	
};

struct FSFILE_HEADER
{
	DWORD	dwTotalSize;
	DWORD	dwRealFileSize;
	DWORD	dwFileNameLen;
	DWORD	dwFileDataOffset;
	DWORD	dwFlag1;
	DWORD	dwFlag2;
	DWORD	dwFlag3;
	DWORD	dwFlag4;

	char	szFileName[4];
};

#define FSFILE_HEADER_SIZE	32

struct PACK_FILE_HEADER
{
	//Additional Keys
	DWORD	Key1;
	DWORD	Key2;
	DWORD	Key3;
	DWORD	Key4;
	DWORD	Key5;
	DWORD	Key6;
	DWORD	Key7;
	DWORD	Key8;
	DWORD	Key9;
	DWORD	Key10;
	DWORD	dwVersion;
	DWORD	dwFileItemNum;
	DWORD	dwFlag;
	DWORD	dwCRC[4];
	DWORD	dwReserved[16];

	DWORD	dwType;		// fileType 
	DWORD	dwDataSize;	// data size
};

const DWORD dwVersion[] = 
{
	20040308,
	20090506,
	20101117,
};

const unsigned char aKey[][MAX_KEY_LENGTH] = 
{
	"",
	"E76B2413958B00E193A1",
	"A15H3825287T19G082K8",
};
