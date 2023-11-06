#ifndef __MHFILEEX_H__
#define __MHFILEEX_H__

#include <stdio.h>
#ifdef _AES_FILE_
#include "AESFile.h"
#endif

// desc_hseos_에디트박스_01
// S 에디트박스 용량늘림 추가 added by hseos 2007.05.30
// ..bin의 save 및 open 을 MEGA 만큼만 NEW 를 하게 되어 있어서 MEGA 를 초과하는 bin 을 처리할 수 없다.
// ..그래서 넉넉하게 20메가로 늘림..
// ..2007/05/30 현재 ItemList.bin의 용량이 3메가 조금 안 됨. 나중에 더 늘려야 할 지도 모름..
// #define		MEGA		1024*1024
#define		MEGA		1024*1024*20
// E 에디트박스 용량늘림 추가 added by hseos 2007.05.30
#define		DOFHEADER	20041120
#define		DOFTAIL		20040921

struct MHFILE_HEADER
{
	DWORD	dwVersion;	// version
	DWORD	dwType;		// file Á¾·ù
	DWORD	dwDataSize;	// data size
};

struct DOF_HEADER
{
	DWORD	dwVersion;
	DWORD	dwDataSize;
	DWORD	dwSeed;
};

struct DOF_TAIL
{
	DWORD	dwVersion;
	DWORD	dwSeed;
};

class CMHFileEx
{
protected:
	FILE*			m_fp;
	char			m_szFullFileName[256];
	char			m_szFileName[256];
	char			m_szExt[256];

	MHFILE_HEADER	m_Header;
	char			m_crc1;
	char			m_crc2;
	char*			m_pData;
	char*			m_pBinData;
	
public:
	CMHFileEx();
	virtual ~CMHFileEx();

	void	Clear();
	BOOL	New( const char* fullfilename );
	BOOL	OpenTxt( const char* fullfilename );
	BOOL	OpenBin( const char* fullfilename );
	BOOL	SaveToTxt( const char* filename );
	BOOL	SaveToTxt();
	BOOL	SaveToBin( const char* filename );
	BOOL	SaveToBin();
	BOOL	CheckHeader();
	BOOL	CheckCRC();
	BOOL	ConvertBin();

	BOOL	InitFileName( const char* fullfilename );				
	char*	GetFullFileName()								{ return m_szFullFileName; }
	char*	GetFileName()									{ return m_szFileName; }
	char*	GetExt()										{ return m_szExt; }
	void	SetFullFileName( const char* fullfilename )		{ strcpy( m_szFullFileName, fullfilename ); }
	void	SetFileName( const char* filename )				{ strcpy( m_szFileName, filename ); }
	void	SetExt( const char* ext )						{ strcpy( m_szExt, ext ); }
	void	MakeFullFileName();
	char*	GetData()										{ return m_pData; }
	void	SetData( char* str );
	MHFILE_HEADER	GetHeader()								{ return m_Header; }

protected:
	DOF_HEADER		m_DOFHeader;
	DOF_TAIL		m_DOFTail;
	char			m_szDOF[256];
public:
	BOOL	SaveToDOF( const char* filename );
	BOOL	SaveToDOF();
	BOOL	ConvertDOF();	
	BOOL	MakeDOFName();

	BOOL	OpenDOF( const char* fullfilename );
	BOOL	CheckDOF();
#ifdef _AES_FILE_
	cAESFile	m_AESFile;
#endif
};

#endif