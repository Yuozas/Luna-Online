// HardwareInfo.h: interface for the CHardwareInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HARDWAREINFO_H__30E3259F_CD97_43A9_98C8_6FDCDD49763A__INCLUDED_)
#define AFX_HARDWAREINFO_H__30E3259F_CD97_43A9_98C8_6FDCDD49763A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <dxdiag.h>




#include ".\info\cpu_info.h"


#define BUFSIZE 255
#define MEGA 1048576
#define KILO 1024


enum eGRAPHIC_CARD_OPTION
{
	NORMAL				,	// nvidia / ÀÌ»ó¾ø´Â ±×·¡ÇÈ Ä«µå 
	RADEON_7500_UNDER	,	// RADEON 7500 UNDER CHIP SERIES	
	RADEON_7500_ABOVE	,	// RADEON 7500 »óÀ§ CHIP SERIES 
	MATROX				,	// MATROX CHIP
	VOODOO              ,	// VOODOO CHIP
	POOR_CARD			,	// ±×·¡ÇÈ Ä«µåÀÇ ¸Þ¸ð¸®°¡ 32 ¸Þ°¡ ¹Ì¸¸ ÀÌ´Ù.
	OPTION_NUM				// ¿É¼Ç °³¼ö / ¿À·ùÀÏ¶§µµ »ç¿ëÇÑ´Ù. 
};

#define NVIDIA_VENTDER_ID	4318
#define ATI_VENTDER_ID		4098
#define MATROX_VENTDER_ID	4139
#define VOODOO_VENDER_ID	4634

#define NVIDIA_DRIVER_ENOUGH_VER	4109
#define	ATI_DRIVER_ENOUGH_VER		6343  


#define MAX_MAC_ADDRESS_IN_BYTE         6
#define MAX_MAC_ADDRESS_IN_STRING      12
#define WINSOCK_11 0x0101



struct GRAPHICINFO
{
	char szChipName[128];
	char szVersion[128];
	char szDisplayMode[128];
	int dwDriverVer;
	eGRAPHIC_CARD_OPTION	eCardKind;
};





class CHardwareInfo  
{
public:
	CHardwareInfo();
	virtual ~CHardwareInfo();

	CString &	GetCPUInfo();
	BOOL		GetOSInfo(CString & strOSInfoOut);
	DWORD		GetTotalMemorySize();	//unit MEGA
	BOOL		GetHardDiskFreeSize(ULONG & FreeDiskSizeOut);	//unit MEGA
	BOOL		GetSoundCardInfo(CString & strSoundCardInfoOut);	//unit MEGA
	BOOL		GetGraphicsCardInfo(GRAPHICINFO * pGraphicInfoOut);
	BOOL		IsCompressedTextureFormatSupport();
	DWORD		GetDXVerInfo(char * pszDxVer);
	DWORD		SpecVidMem(char * pszVidMem, char * pszAGPMem);
	CString &	GetInfoToString();

	CString &	GetLocalIP();
	BOOL		GetMacAddressFromSock(LPBYTE lpMacAddress);
	BOOL		GetMacAddressFromNetBIOS(LPBYTE lpMacAddress);
private:
	eGRAPHIC_CARD_OPTION IdentityGraphicCard();
	void	GetDllVersion(char * pszInOutName);
	int		GetGraphicDrvVerFromString( char * pszString );
	
	CString m_strCPUInfo;
	CString m_strOSInfo;
	CString m_strSoundCardInfoOut;
	CString m_strLocalIP;

	CString m_strTotalInfo;

	CPUInfo	m_CPUInfo;
};
INT_PTR CALLBACK DSoundEnumCallback( GUID* pGUID, LPSTR strDesc, LPSTR strDrvName, VOID* pContext );
#endif // !defined(AFX_HARDWAREINFO_H__30E3259F_CD97_43A9_98C8_6FDCDD49763A__INCLUDED_)
