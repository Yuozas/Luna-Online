// HardwareInfo.cpp: implementation of the CHardwareInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogReporter.h"
#include "HardwareInfo.h"
#include "./info/GetDxVer.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHardwareInfo::CHardwareInfo()
{


}

CHardwareInfo::~CHardwareInfo()
{

}
CString &	CHardwareInfo::GetLocalIP()
{
	m_strLocalIP = "";
	WSADATA wsaData;
	if (WSAStartup(0x0002,&wsaData) == SOCKET_ERROR)
	{
		WSACleanup();
		return m_strLocalIP;
	}

	char szHostName[256];
	char ipAddr[256];
	HOSTENT *pHostInfo;
	IN_ADDR inAddr;

	gethostname(szHostName, 256);
	pHostInfo = gethostbyname(szHostName);

	for(int i = 0 ; pHostInfo->h_addr_list[i] != NULL ; i++)
	{
	   memcpy(&inAddr, pHostInfo->h_addr_list[i], sizeof(IN_ADDR));
	   strcpy(ipAddr, inet_ntoa(inAddr));
	   m_strLocalIP = ipAddr;
	}
	WSACleanup();
	return m_strLocalIP;
}

BOOL CHardwareInfo::GetMacAddressFromSock(LPBYTE lpMacAddress)
{
    int          iAdapters ;
    int          iOpt  = sizeof(iAdapters) ;
    int          iSize = sizeof(SOCKADDR_IPX) ;
    BOOL         bRet  = FALSE ;
    SOCKET       sock ;
    SOCKADDR_IPX adrIPX ;
    WSADATA      wsaData ;
	
    if(WSAStartup(WINSOCK_11,&wsaData)==0)
    {
        if((sock=socket(AF_IPX,SOCK_DGRAM,NSPROTO_IPX)) !=INVALID_SOCKET)
        {
            memset(&adrIPX,0x00,sizeof(SOCKADDR_IPX)) ;
            adrIPX.sa_family = AF_IPX ;
			
            if(bind(sock,(SOCKADDR*)&adrIPX,iSize)!=SOCKET_ERROR)
            {
                if(getsockopt(sock,NSPROTO_IPX,IPX_MAX_ADAPTER_NUM, (char*)&iAdapters,&iOpt)!=SOCKET_ERROR)
                {
                    while(iAdapters)
                    {
                        IPX_ADDRESS_DATA ipxAdrData ;
                        memset( &ipxAdrData,0x00, sizeof(IPX_ADDRESS_DATA)) ;
                        ipxAdrData.adapternum = --iAdapters ;
                        iOpt = sizeof(IPX_ADDRESS_DATA) ;
                        if(getsockopt(sock,NSPROTO_IPX,IPX_ADDRESS, (char*)&ipxAdrData,&iOpt)!=SOCKET_ERROR)
                        {
                            if(ipxAdrData.status==TRUE && ipxAdrData.wan==FALSE)
                            {
                                memcpy(lpMacAddress, &ipxAdrData.nodenum, MAX_MAC_ADDRESS_IN_BYTE) ;
                                bRet = TRUE ;
                                break ;
                            }
                        }
                    }
                }
            }
            closesocket(sock) ;
        }
        WSACleanup() ;
    }
    return(bRet) ;
}

BOOL CHardwareInfo::GetMacAddressFromNetBIOS(LPBYTE lpMacAddress)
{
    NCB ncb;
    LANA_ENUM le;
    
    struct tagADAPTER
    {
        ADAPTER_STATUS adapt;
        NAME_BUFFER    NameBuff[256] ;
    } adapter;
	
	memset(&le,0,sizeof(LANA_ENUM)) ;             


    memset(&ncb,0,sizeof(NCB)) ;
    ncb.ncb_command = NCBENUM ;
    ncb.ncb_buffer = (UCHAR*)&le ;
    ncb.ncb_length = sizeof(LANA_ENUM) ;
	
    if(Netbios(&ncb)==NRC_GOODRET)
    {
        memset(&ncb,0x00,sizeof(NCB)) ;
        ncb.ncb_command = NCBRESET ;
		//          ncb.ncb_callname[0] = 20 ;                   
        ncb.ncb_lana_num = le.lana[0] ;              
		
        if(Netbios(&ncb)==NRC_GOODRET)
        {
            memset(&ncb,0x00,sizeof(NCB)) ;
            memset(&ncb.ncb_callname,' ',NCBNAMSZ) ;
            ncb.ncb_callname[0] = '*' ;
			//ncb.ncb_callname[NCBNAMSZ-1] = NULL ;    
            ncb.ncb_command = NCBASTAT ;
            ncb.ncb_lana_num = le.lana[0] ;          
            ncb.ncb_buffer = (UCHAR*)&adapter ;
            ncb.ncb_length = sizeof(tagADAPTER) ;
			
            if(Netbios(&ncb)==NRC_GOODRET)
            {
                memcpy(lpMacAddress,&adapter.adapt.adapter_address, MAX_MAC_ADDRESS_IN_BYTE) ;
                return(TRUE) ;
            }
        }
    }
    return(FALSE) ;
}

CString &	CHardwareInfo::GetInfoToString()
{
	CString tmpInfo;
	m_strTotalInfo = "";


	m_strTotalInfo += GetCPUInfo();
	m_strTotalInfo += "\n";

	if(!GetOSInfo(tmpInfo))
		return m_strTotalInfo;
	m_strTotalInfo += tmpInfo;
	m_strTotalInfo += "\n";

	tmpInfo.Format("Memory : %u M", GetTotalMemorySize());
	m_strTotalInfo += tmpInfo;
	m_strTotalInfo += "\n";

	ULONG size = 0;
	if(!GetHardDiskFreeSize(size))
		return m_strTotalInfo;
	tmpInfo.Format("HardDiskFreeSize : %u M", size);
	m_strTotalInfo += tmpInfo;
	m_strTotalInfo += "\n";

	if(!GetSoundCardInfo(tmpInfo))
		return m_strTotalInfo;
	m_strTotalInfo += tmpInfo;
	m_strTotalInfo += "\n";
	

	char tmpBuf1[256];
	char tmpBuf2[256];
	int ret = GetDXVerInfo(tmpBuf1);
	m_strTotalInfo += tmpBuf1;
	m_strTotalInfo += "\n";
	
	
	DWORD dwGraphicVidMemoryToKilo = SpecVidMem(tmpBuf1, tmpBuf2);
	tmpInfo.Format("Video Memory : %s KB | AGP Memory : %s KB", tmpBuf1, tmpBuf2);
	m_strTotalInfo += tmpInfo;
	m_strTotalInfo += "\n";
	
	GRAPHICINFO tmpGInfo;
	if(!GetGraphicsCardInfo(&tmpGInfo))
		return m_strTotalInfo;

	tmpInfo.Format("Chip : %s | Ver : %s | DisplayMode : %s ", tmpGInfo.szChipName, tmpGInfo.szVersion, tmpGInfo.szDisplayMode);
	m_strTotalInfo += tmpInfo;
	m_strTotalInfo += "\n";

	if(IsCompressedTextureFormatSupport())
	{
		m_strTotalInfo += "압축텍스쳐 지원";
		m_strTotalInfo += "\n";
	}
	else
	{
		m_strTotalInfo += "압축텍스쳐 지원 안함";
		m_strTotalInfo += "\n";
	}

	return m_strTotalInfo;
}
CString & CHardwareInfo::GetCPUInfo()
{
	m_strCPUInfo = m_CPUInfo.GetExtendedProcessorName();

	if ( m_CPUInfo.DoesCPUSupportFeature(SSE_FEATURE) )
	{
		m_strCPUInfo += "| Support SSE";
	}
	else
	{
		m_strCPUInfo += "| Not Support SSE";
	}

	if (m_strCPUInfo.Find("Hr", 0) == 0)
	{ 
		// cpu 속도 체크를 하지못하였을때
		LONG result;
		HKEY hKey;
		DWORD data;
		DWORD dataSize;

		// Get the processor speed info.
		result = ::RegOpenKeyEx (HKEY_LOCAL_MACHINE,
			"Hardware\\Description\\System\\CentralProcessor\\0", 0, KEY_QUERY_VALUE, &hKey);

		// Check if the function has succeeded.
		if (result == ERROR_SUCCESS) 
		{
			result = ::RegQueryValueEx (hKey, _T("~MHz"), NULL, NULL,
				(LPBYTE)&data, &dataSize);
			
			CString strCpuSpeed;
			strCpuSpeed.Format ("%d MHz", data);
			m_strCPUInfo += strCpuSpeed;
		}
	}
	else
	{
		DWORD dwSpeedMhz = m_CPUInfo.GetCPUSpeedMhz();
		CString strCpuSpeed;
		strCpuSpeed.Format ("%d MHz", dwSpeedMhz);
		m_strCPUInfo += strCpuSpeed;
	}
	
	return m_strCPUInfo;
}
BOOL CHardwareInfo::GetOSInfo(CString & strOSInfoOut)
{
	char szBuf[255] = {0,};
	char szCatBuf[64] = {0,};

	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;

	// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	// If that fails, try using the OSVERSIONINFO structure.

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		// If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return FALSE;
	}

	switch (osvi.dwPlatformId)
	{
		// Tests for Windows NT product family.
		case VER_PLATFORM_WIN32_NT:

		// Test for the product.
		if ( osvi.dwMajorVersion <= 4 )
			sprintf(szBuf, "%s", "Microsoft Windows NT ");

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
			sprintf(szBuf, "%s", "Microsoft Windows 2000 ");

		if( bOsVersionInfoEx )  // Use information from GetVersionEx.
		{ 
			// Test for the workstation type.
			if ( osvi.wProductType == VER_NT_WORKSTATION )
			{
			   if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
				  strcat(szBuf, "Microsoft Windows XP ");

			   if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
				  strcat(szBuf, "Home Edition " );
			   else
				  strcat(szBuf, "Professional " );
			}

			// Test for the server type.
			else if ( osvi.wProductType == VER_NT_SERVER )
			{
			   if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
				  strcat(szBuf,"Microsoft Windows .NET ");

			   if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
				  strcat(szBuf, "DataCenter Server " );
			   else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
				  if( osvi.dwMajorVersion == 4 )
					 strcat(szBuf, "Advanced Server " );
				  else
					 strcat(szBuf, "Enterprise Server " );
			   else if ( osvi.wSuiteMask == VER_SUITE_BLADE )
				  strcat(szBuf, "Web Server " );
			   else
				  strcat(szBuf, "Server " );
			}
		}
		else   // Use the registry on early versions of Windows NT.
		{
			HKEY hKey;
			char szProductType[BUFSIZE];
			DWORD dwBufLen=BUFSIZE;
			LONG lRet;

			lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
			   "SYSTEM\\CurrentControlSet\\Control\\ProductOptions",
			   0, KEY_QUERY_VALUE, &hKey );
			if( lRet != ERROR_SUCCESS )
				return FALSE;

			lRet = RegQueryValueEx( hKey, "ProductType", NULL, NULL,
			   (LPBYTE) szProductType, &dwBufLen);
			if( (lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE) )
				return FALSE;

			RegCloseKey( hKey );

			if ( lstrcmpi( "WINNT", szProductType) == 0 )
			   strcat(szBuf, "Professional " );
			if ( lstrcmpi( "LANMANNT", szProductType) == 0 )
			   strcat(szBuf, "Server " );
			if ( lstrcmpi( "SERVERNT", szProductType) == 0 )
			   strcat(szBuf, "Advanced Server " );
		}

		// Display version, service pack (if any), and build number.

		if ( osvi.dwMajorVersion <= 4 )
		{
			sprintf (szCatBuf, "\r\nversion %d.%d %s (Build %d)",
			   osvi.dwMajorVersion,
			   osvi.dwMinorVersion,
			   osvi.szCSDVersion,
			   osvi.dwBuildNumber & 0xFFFF);
		}
		else
		{ 
			sprintf (szCatBuf, "\r\n%s (Build %d)",
			   osvi.szCSDVersion,
			   osvi.dwBuildNumber & 0xFFFF);
		}
		break;

		// Test for the Windows 95 product family.
		case VER_PLATFORM_WIN32_WINDOWS:

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
		{
			 strcpy(szBuf, "Microsoft Windows 95 ");
			 if ( osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B' )
				strcat(szBuf, "OSR2 " );
		} 

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
		{
			 strcpy(szBuf, "Microsoft Windows 98 ");
			 if ( osvi.szCSDVersion[1] == 'A' )
				strcat(szBuf, "SE " );
		} 

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
		{
			 strcpy (szBuf, "Microsoft Windows Millennium Edition ");
		} 
		break;
	}

	if ( strcmp(szCatBuf, "\0") != 0 )
	{ // 추가사항이 있다.
		strcat (szBuf, szCatBuf);
	}
	
	strOSInfoOut = m_strOSInfo = szBuf;
	return TRUE;
}

DWORD CHardwareInfo::GetTotalMemorySize()
{
	MEMORYSTATUS	state;

	memset(&state, 0, sizeof(state));
	state.dwLength = sizeof (state);
	GlobalMemoryStatus(&state);

	DWORD	dwTotalMemory;
	dwTotalMemory = DWORD(state.dwTotalPhys / MEGA)+1;

	return dwTotalMemory;
}

BOOL CHardwareInfo::GetHardDiskFreeSize(ULONG & FreeDiskSizeOut)
{
	FreeDiskSizeOut = 0;
	char  szBuf[255];
	GetCurrentDirectory(255, szBuf); 

    ULARGE_INTEGER FreeBytesAvailableToCaller;	// receives the number of bytes on // disk available to the caller
    ULARGE_INTEGER TotalNumberOfBytes;			// receives the number of bytes on disk
    ULARGE_INTEGER TotalNumberOfFreeBytes;		// receives the free bytes on disk

	int iRet;
	iRet = GetDiskFreeSpaceEx(szBuf,					// current directory information	
		&FreeBytesAvailableToCaller, 
		&TotalNumberOfBytes,
		&TotalNumberOfFreeBytes);
	if (0 == iRet)
	{
		return FALSE;
	}

	ULONG	ulFreeMegaBytes = ULONG(TotalNumberOfFreeBytes.QuadPart / MEGA);	// 1048576 == 1024^2
	//_ultoa(ulFreeMegaBytes, szBuf, 10);

	FreeDiskSizeOut = ulFreeMegaBytes;
	return TRUE;
}

BOOL CHardwareInfo::GetSoundCardInfo(CString & strSoundCardInfoOut)
{
	
	HRESULT				hr;
	char				szBuf[255] = {'\0', };


	LPDIRECTSOUND	lpDS = NULL;

    if( FAILED( hr = DirectSoundCreate(0, &lpDS, NULL ) ) )
	{
		strSoundCardInfoOut = "No Driver";
        return FALSE;
	}
    if( FAILED( hr = DirectSoundEnumerate( (LPDSENUMCALLBACK)DSoundEnumCallback,
                                                  (VOID*)szBuf ) ) )
	{
		strSoundCardInfoOut = "No Driver";
		MessageBox(NULL,"Error : DirectSoundCaptureEnumerate!!", "묵향온라인", MB_OK);
        return FALSE;
	}

    if(lpDS)
	{
		lpDS->Release();
		lpDS = NULL;
	}

	strSoundCardInfoOut = szBuf;

	return TRUE;
}

DWORD CHardwareInfo::GetDXVerInfo(char * pszDxVer)
{
	HRESULT hr;
    DWORD	dwDXVersion;
	char	strResult[128];
	
	hr = GetDXVersion(&dwDXVersion, pszDxVer, 10);
    if( SUCCEEDED(hr) )
    {
        if( dwDXVersion > 0 )
            _sntprintf( strResult, 128, TEXT("DirectX %s installed"), pszDxVer );
        else
            _tcsncpy( strResult, TEXT("DirectX not installed"), 128 );
        strResult[127] = 0;
    }
    else
    {
        _sntprintf( strResult, 128, TEXT("Unknown version of DirectX installed"), hr );
        strResult[127] = 0;
    }

	strcpy(pszDxVer, strResult);

	return dwDXVersion;
}

// ===================================
// Graphic Card Spec
// CASE :
//  - 그래픽카드 명
//  - 그래픽카드 드라이버 버전
//  - 디스플레이 모드  
//  ...
// return : 버전 뒷자리 4자리를 리턴한다.(숫자)
// ===================================
BOOL CHardwareInfo::GetGraphicsCardInfo(GRAPHICINFO * pGraphicInfoOut)
{
	char			szBuf[255];
	D3DDEVTYPE		d3dType = D3DDEVTYPE_HAL;
	D3DCAPS8		d3dCaps;
	LPDIRECT3D8		lpD3D;
	HRESULT			hr;
	int				nRetVer;	// 뒷자리 4자리를 숫자로 내보낸다.

	// D3D 객체 얻음 
	if( NULL == ( lpD3D = Direct3DCreate8( D3D_SDK_VERSION ) ) )
	{
		AfxMessageBox("Error : Create Direct3D ");
		return FALSE;
	}

	hr = lpD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, d3dType, &d3dCaps);
	if ( FAILED(hr) )
	{
		AfxMessageBox("GetDeviceCaps Error");
        return FALSE;		
	}

	// 어뎁터(그래픽 카드)의 정보 얻기 
	D3DADAPTER_IDENTIFIER8  Adapter;
    if( FAILED( lpD3D->GetAdapterIdentifier(0, D3DENUM_NO_WHQL_LEVEL, &Adapter) ) )
	{
		AfxMessageBox("AdapterIdentifer Error");
        return FALSE;
	}

	strcpy(pGraphicInfoOut->szChipName, Adapter.Description);

	// driver 버전 얻기
	CString StrVersionInfo;
	strcpy(szBuf, Adapter.Driver);	// 버전 파일 명 얻기
	GetDllVersion(szBuf);			// szBuf에 버전정보를 담아 온다.

	nRetVer = GetGraphicDrvVerFromString(szBuf);

	StrVersionInfo.Format("%s (%s)", Adapter.Driver, szBuf);

	strcpy(pGraphicInfoOut->szVersion, (LPCTSTR)StrVersionInfo);
	
	// display mode 정보 얻기 
	D3DDISPLAYMODE d3ddm;
    if( FAILED( lpD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, & d3ddm ) ) )
	{
		AfxMessageBox("GetAdapterDisplayMode Error");
        return FALSE;
	}

	sprintf(szBuf, "%dX%d %d Hz", d3ddm.Width, d3ddm.Height, d3ddm.RefreshRate);
	strcpy(pGraphicInfoOut->szDisplayMode, szBuf);

	// 객체 릴리즈 
	lpD3D->Release();

	pGraphicInfoOut->eCardKind = IdentityGraphicCard();
	pGraphicInfoOut->dwDriverVer = nRetVer;
	return TRUE;
}

// ===================================
// Version 정보 뒷자리를 알아온다.
// ===================================
int	CHardwareInfo::GetGraphicDrvVerFromString( char * pszString )
{
	int		i;
	char *	pStrPos;
	int		iVersion;

	pStrPos = strstr(pszString, ".");
	for (i = 0; i < 2; i++)
	{
		++pStrPos;
		pStrPos = strstr(pStrPos, ".");
	}
	
	++pStrPos;
	iVersion = atoi((LPCTSTR)pStrPos);

	return iVersion;
}

// ====================================================
// SpecGraphicCard()
// - 어떤 그래픽카드인지 알아서 정보를 리턴함.
// ====================================================
eGRAPHIC_CARD_OPTION CHardwareInfo::IdentityGraphicCard()
{
	char					szBuf[256];
	D3DDEVTYPE				d3dType = D3DDEVTYPE_HAL;
	LPDIRECT3D8				lpD3D;
	eGRAPHIC_CARD_OPTION	eFlag = NORMAL;

	// D3D 객체 얻음 
	if( NULL == ( lpD3D = Direct3DCreate8( D3D_SDK_VERSION ) ) )
	{
		MessageBox(NULL, "Error : Create Direct3D object", NULL, MB_OK);
		return OPTION_NUM;	
	}

	// 어뎁터(그래픽 카드)의 정보 얻기 
	D3DADAPTER_IDENTIFIER8  Adapter;
    if( FAILED( lpD3D->GetAdapterIdentifier(0, D3DENUM_NO_WHQL_LEVEL, &Adapter) ) )
	{	
		MessageBox(NULL, "AdapterIdentifer Error", NULL, MB_OK);
		
		lpD3D->Release();		
        return OPTION_NUM;
	}	

	int		i;
	char *	pString;
	bool	bRadeon8500MoreThan = FALSE;
	DWORD	dwMemKilo;
	
	dwMemKilo = SpecVidMem(NULL, NULL);
	
	switch(Adapter.VendorId)
	{
	case NVIDIA_VENTDER_ID:
		if (dwMemKilo < 30000)
			eFlag = POOR_CARD;
		else
			eFlag = NORMAL;

		goto lb_Exit;

	case ATI_VENTDER_ID:
		for (i = 0; i < 10; i++)
		{ // 숫자가 나오면 무조건 검사한다. 
			itoa(i, szBuf, 10);
			pString = strstr(Adapter.Description, szBuf);

			if (!pString)
				continue;

			if ( strstr(Adapter.Description, "7000") || 
				 strstr(Adapter.Description, "7200") ||
				 strstr(Adapter.Description, "7500") )
			{
				 bRadeon8500MoreThan = FALSE;
				 break;
			}
			else if ( strstr(Adapter.Description, "8500") )
			{
				bRadeon8500MoreThan = TRUE;
				break;
			}
			else if ( strstr(Adapter.Description, "9000") )
			{
				bRadeon8500MoreThan = TRUE;
				break;
			}
			else if ( strstr(Adapter.Description, "9100") )		
			{
				bRadeon8500MoreThan = TRUE;
				break;
			}
			else if ( strstr(Adapter.Description, "9200") )		
			{
				bRadeon8500MoreThan = TRUE;
				break;
			}
			else if ( strstr(Adapter.Description, "9500") )		
			{
				bRadeon8500MoreThan = TRUE;
				break;
			}
			else if ( strstr(Adapter.Description, "9600") )		
			{
				bRadeon8500MoreThan = TRUE;
				break;
			}
			else if ( strstr(Adapter.Description, "9700") )		
			{
				bRadeon8500MoreThan = TRUE;
				break;
			}
			else if ( strstr(Adapter.Description, "9800") )		
			{
				bRadeon8500MoreThan = TRUE;
				break;
			}
			else if ( strstr(Adapter.Description, "VE") )		
			{
				bRadeon8500MoreThan = TRUE;
			}
			else if (dwMemKilo > 120000)
			{ // vid mem 128 이상 
				bRadeon8500MoreThan = TRUE;
			}
			else
			{
				bRadeon8500MoreThan = FALSE;
			}
		}

		if (bRadeon8500MoreThan)
		{
			eFlag = RADEON_7500_ABOVE;
			goto lb_Exit;
		}
		else
		{
			eFlag = RADEON_7500_UNDER;
			goto lb_Exit;
		}

		break;
	case MATROX_VENTDER_ID:
		if (dwMemKilo < 30000)
			eFlag = POOR_CARD;
		else
			eFlag = MATROX;

		goto lb_Exit;

	case VOODOO_VENDER_ID:
		eFlag = VOODOO;
		goto lb_Exit;

	default:
		if (dwMemKilo < 30000)
			eFlag = POOR_CARD;
		else
			eFlag = NORMAL;	

		goto lb_Exit;
	}

lb_Exit:
	// release object
	lpD3D->Release();	
	
	return eFlag;
}


// ===================================
// IsCompressedTextureFormatOk
//
// CASE : 압축 텍스쳐 지원하는지 안하는지를 알아옴 
// RETURN :
//			TRUE, FALSE ;	지원한다, 안한다,
// ===================================
BOOL CHardwareInfo::IsCompressedTextureFormatSupport()
{
		BOOL					bResult = FALSE;
	LPDIRECT3D8				lpD3D;
	HRESULT					hr;

    if( NULL == ( lpD3D = Direct3DCreate8( D3D_SDK_VERSION ) ) )
	{
		MessageBox(NULL, "Can't Create Direct3D object!", "Error", MB_OK);
        return FALSE;
	}

	D3DDISPLAYMODE			d3ddm;
	if( FAILED( lpD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
	{
		lpD3D->Release();
		return E_FAIL;
	}

	hr = lpD3D->CheckDeviceFormat( D3DADAPTER_DEFAULT,
                                      D3DDEVTYPE_HAL,
                                      d3ddm.Format,
                                      0,
                                      D3DRTYPE_TEXTURE,
                                      D3DFMT_DXT1);

	if (hr == D3D_OK)
	{
		bResult = TRUE;
	}

	lpD3D->Release();

	return bResult;
}

// ==============================================
// SpecVidMem
// 그래픽카드의 비디오 메모리량을 알아 KB 단위로 리턴 
// ==============================================
DWORD CHardwareInfo::SpecVidMem(char * pszVidMem, char * pszAGPMem)
{
	LPDIRECTDRAW	lpDD; 
	LPDIRECTDRAW2	lpDD2; 
	DDSCAPS2		ddsCaps; 
	DWORD			dwTotal; 
	DWORD			dwFree; 

	char			szBuf[255];

	HRESULT			hr;

    hr = DirectDrawCreate( NULL, &lpDD, NULL );
    if( FAILED(hr) )
    {
		strcpy(pszVidMem, "Can't create directdraw object!");
		return -1;
    }

    // Let's see if IID_IDirectDraw2 exists.
    hr = lpDD->QueryInterface( IID_IDirectDraw2, (VOID**)&lpDD2 );
    if( FAILED(hr) )
    {
        // No IDirectDraw2 exists... must be DX1
        lpDD->Release();

		strcpy(pszVidMem, "Can't Query IID_IDirectDraw2");
        return -1;
    }

//	DXSAMPLEFORMATENUM dsPixelFormatEnum;
	//lpDD->get

	// GET VIDEO MEMORY	
	CString	strBuf;
	DWORD dwVidMemory;
	ddsCaps.dwCaps = // 현재 구할 캡스 스테이트 
		DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY|DDSCAPS_LOCALVIDMEM; 
	hr = lpDD2->GetAvailableVidMem((DDSCAPS*)&ddsCaps, &dwTotal, &dwFree); 
	if (FAILED(hr)) 
	{
		
		lpDD2->Release();
		lpDD->Release();

		MessageBox(NULL, "GetAvailableVidMem", "ERROR", MB_OK);
		return -1;
	}
	
	// 전체 비디오 메모리 / 사용가능 비디오 메모리 
	if (pszVidMem)
	{
		dwTotal = dwTotal / KILO;
		ultoa(dwTotal, szBuf, 10);
		strcpy(pszVidMem, szBuf);
		dwFree = dwFree / KILO;
		ultoa(dwFree, szBuf, 10);
		strcat(pszVidMem, " / ");
		strcat(pszVidMem, szBuf);	
	}

	dwVidMemory = dwTotal;

	// GET AGP MEMORY
	ddsCaps.dwCaps = // 현재 구할 캡스 스테이트 
				DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY|DDSCAPS_NONLOCALVIDMEM; 

	hr = lpDD2->GetAvailableVidMem((DDSCAPS*)&ddsCaps, &dwTotal, &dwFree); 
	if (FAILED(hr)) 
	{
		lpDD2->Release();
		lpDD->Release();

		MessageBox(NULL, "GetAvailableVidMem", "ERROR", MB_OK);
		return -1;
	}

	dwTotal = dwTotal / KILO;
	if (pszAGPMem)
	{
		ultoa(dwTotal, szBuf, 10);
		strcpy(pszAGPMem, szBuf);
	}

    // IDirectDraw2 exists. We must be at least DX2
    lpDD2->Release();
	lpDD->Release();

	return dwVidMemory;
}




void	CHardwareInfo::GetDllVersion(char * pszInOutName)
{
	//	TCHAR execFilename[_MAX_PATH];
	//	VERIFY(::GetModuleFileName(NULL, execFilename, _MAX_PATH));

	HWND hWindow = NULL;

	BYTE *block;
	DWORD FAR *translation;
	DWORD FAR *buffer;
	DWORD handle;
	UINT bytes;
	TCHAR *lpszFileName = pszInOutName;
	TCHAR name[512];
	CString szString;

	// 버전 정보의 실제 크기를 가져온다.
	bytes = (UINT)::GetFileVersionInfoSize( lpszFileName, &handle);

	if(bytes)
	{
		block = new BYTE[bytes];

		// 버전 정보에 대한 실제 블록을 가져온다.
		if( ::GetFileVersionInfo( lpszFileName, handle, bytes, block) ) 
		{
			if( ::VerQueryValue( block, "\\VarFileInfo\\Translation", (LPVOID *)&translation,(UINT FAR *)&bytes) )
			{

				// 버전 정보
				wsprintf(name,"\\StringFileInfo\\%04x%04x\\FileVersion", 
				LOWORD(*translation), HIWORD(*translation) );

				if( ::VerQueryValue(block, name, (LPVOID *)&buffer, (UINT FAR *)&bytes) ) 
				{
					szString.Format("%s", (LPCTSTR)buffer );
				}
				else
				{
					MessageBox(hWindow,"Can't call Version Info","Version Error",MB_OK);
					PostQuitMessage(0);
				}
			
			}
			else 
			{
				MessageBox(hWindow,"Can't call Translation Type","Version Error",MB_OK);
				PostQuitMessage(0);
			}
				
		}
		else 
		{
			MessageBox(hWindow,"Can't call Version Info","Version Error",MB_OK);
			PostQuitMessage(0);
		}

		delete [] block;
	}
	else 
	{
		char temp[256];
		sprintf(temp, "Can't call FileVersionInfo Size(%s)", lpszFileName);
		MessageBox(hWindow,temp,"Version Error",MB_OK);
		//PostQuitMessage(0);
	}

	strcpy(pszInOutName, (LPCTSTR)szString );
}
//-----------------------------------------------------------------------------
// Name: DSoundEnumCallback()
// Desc: Enumeration callback called by DirectSoundEnumerate
//-----------------------------------------------------------------------------
INT_PTR CALLBACK DSoundEnumCallback( GUID* pGUID, LPSTR strDesc, LPSTR strDrvName,
                                  VOID* pContext )
{
    // Set aside static storage space for 20 audio drivers
    static GUID  AudioDriverGUIDs[20];
    static DWORD dwAudioDriverIndex = 0;

	char* pBuf;

    GUID* pTemp  = NULL;

	pBuf = (char*)pContext;


	memcpy( pBuf, strDesc, strlen(strDesc) );
/*
    if( pGUID )
    {
        if( dwAudioDriverIndex >= 20 )
            return TRUE;

        pTemp = &AudioDriverGUIDs[dwAudioDriverIndex++];
        memcpy( pTemp, pGUID, sizeof(GUID) );
    }

    HWND hSoundDeviceCombo = (HWND)pContext;

    // Add the string to the combo box
    SendMessage( hSoundDeviceCombo, CB_ADDSTRING, 
                 0, (LPARAM) (LPCTSTR) strDesc );

    // Get the index of the string in the combo box
    INT nIndex = (INT)SendMessage( hSoundDeviceCombo, CB_FINDSTRING, 
                                   0, (LPARAM) (LPCTSTR) strDesc );

    // Set the item data to a pointer to the static guid stored in AudioDriverGUIDs
    SendMessage( hSoundDeviceCombo, CB_SETITEMDATA, 
                 nIndex, (LPARAM) pTemp );
*/
    return TRUE;
}
