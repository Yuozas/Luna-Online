#pragma once

enum eBLIT_IMAGE_FLAG
{
	BIF_NONE		= 0,
	BIF_RANDOMCOLOR	= 1,
};

class CAutoNoteRoom;

#define AUTONOTEMGR CAutoNoteManager::GetInstance()

class CAutoNoteManager
{
protected:
	CYHHashTable< CAutoNoteRoom >			m_htAutoNoteRoom;
	CMemoryPoolTempl< CAutoNoteRoom >*		m_pmpAutoNoteRoom;

	BOOL	m_bInited;

	BYTE*	m_pOriRaster[4];
	BYTE*	m_pBGRaster;
	BYTE*	m_pNoiseRaster;

	BYTE*	m_pSendRaster;

public:

	MAKESINGLETON( CAutoNoteManager );

	CAutoNoteManager(void);
	~CAutoNoteManager(void);

	void Init();
	void Release();
	void Process();
	void NetworkMsgParse( BYTE Protocol, void* pMsg, DWORD dwLength );

	void AutoPlayerLogOut( CPlayer* pAutoPlayer );

	BOOL CanUseAutoNote() { return m_bInited; }

	void MakeSendRaster( DWORD* pData );

	BYTE* GetSendRaster() { return m_pSendRaster; }

	BOOL BMP_RasterLoad( char* filename, BYTE* pRaster );
	void BlitImage( BYTE* pDest, BYTE* pSrc, int dw, int dh, int sw, int sh, int x, int y, float fszw = 1.0f, float fszh = 1.0f, float fRot = 0.0f, int cx = 0, int cy = 0, int Flag = BIF_NONE );
};
