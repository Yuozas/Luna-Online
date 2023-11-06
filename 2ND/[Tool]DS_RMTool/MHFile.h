// MHFile.h: interface for the CMHFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MHFILE_H__0AE77EF3_1035_4F4E_B26A_853B53D1F0A5__INCLUDED_)
#define AFX_MHFILE_H__0AE77EF3_1035_4F4E_B26A_853B53D1F0A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR 0x00000001

struct MHFILE_HEADER
{
	DWORD	dwVersion;	// version
	DWORD	dwType;		// file ����
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
	void GetLineX(char * pBuf, int len);	// �ּ�ó�� �� �� ������ ���
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
	MHFILE_HEADER	m_Header;				// ���� ���
	char			m_crc1;					// ��ȣȭ �˻� ������
	char			m_crc2;					// ��ȣȭ �˻� ������
	char*			m_pData;				// ���� ������
	BOOL			m_bInit;

public:
	BOOL	OpenBin( char* filename );
	void	Clear();
	BOOL	CheckHeader();
	BOOL	CheckCRC();
	char*	GetData()		{ return m_pData; }
	DWORD	GetDataSize()	{ return m_Header.dwFileSize; }

};

#endif // !defined(AFX_MHFILE_H__0AE77EF3_1035_4F4E_B26A_853B53D1F0A5__INCLUDED_)
