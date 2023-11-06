// MHFile.h: interface for the CMHFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MHFILE_H__CF584ED8_B10C_4E1B_A1CF_4E098D34E802__INCLUDED_)
#define AFX_MHFILE_H__CF584ED8_B10C_4E1B_A1CF_4E098D34E802__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR 0x00000001

struct MHFILE_HEADER
{
	DWORD	dwVersion;	// version
	DWORD	dwType;		// file 횁쩐쨌첫
	DWORD	dwFileSize;	// data size
};

enum
{
	MHFILE_NORMALMODE,
	MHFILE_PACKEDFILE,
	MHFILE_ENGINEPACKEDFILE,
};

class CMHFile  
{
	FILE* fp;
public:
	CMHFile();
	virtual ~CMHFile();

	BOOL Init(char* filename,char* mode, DWORD dwFlag = 0);
	void Release();

	char* GetStringInQuotation();
	void GetStringInQuotation(char* pBuf);
	char* GetString();
	void GetString(char* pBuf);
	void GetLine(char * pBuf, int len);
	void GetLineX(char * pBuf, int len);	// 횁횜쩌짰횄쨀쨍짰 쨉횊 째첨 ?횖?쨩쨋짠 쨩챌쩔챘
	int GetInt();
	LONG GetLong();
	float GetFloat();
	DWORD GetDword();
	WORD GetWord();
	BYTE GetByte();
	BOOL GetBool();
	BOOL GetExpPoint();
	BOOL GetLevel();
	BOOL Seek(int n);	

	void GetFloat(float* pArray,int count);
	void GetWord(WORD* pArray,int count);
	void GetDword(DWORD* pArray,int count);

	BOOL IsEOF();


	BOOL IsInited();

	void SetString(char * str);

protected:	// JSD - binary file
	DWORD			m_bReadMode;
	int				m_Dfp;
	MHFILE_HEADER	m_Header;				// 횈횆?횕 횉챙쨈천
	char			m_crc1;					// 쩐횕횊짙횊짯 째횏쨩챌 쨉짜?횑횇횒
	char			m_crc2;					// 쩐횕횊짙횊짯 째횏쨩챌 쨉짜?횑횇횒
	char*			m_pData;				// 쩍횉횁짝 쨉짜?횑횇횒
	BOOL			m_bInit;

public:
	BOOL	OpenBin( char* filename );
	void	Clear();
	BOOL	CheckHeader();
	BOOL	CheckCRC();
	char*	GetData()		{ return m_pData; }
	DWORD	GetDataSize()	{ return m_Header.dwFileSize; }

};


#endif // !defined(AFX_MHFILE_H__CF584ED8_B10C_4E1B_A1CF_4E098D34E802__INCLUDED_)
