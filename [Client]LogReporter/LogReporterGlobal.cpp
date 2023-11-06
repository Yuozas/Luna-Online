#include "StdAfx.h"
#include "resource.h"
#include ".\zbugziller.h"
#include "HardwareInfo.h"
#include "LogReporterGlobal.h"


#ifdef _FOR_CHINA_
	#include "SkinStatic.h"
	#include "LogReporterChinaDlg.h"
#else
	#include "LogReporterDlg.h"
#endif




/// Global variables

INT g_iTimer = 0;


BOOL IsSupportSSE()
{
	BOOL	bResult = TRUE;
	__try 
	{
		// 	xorps	xmm0,xmm0
        __asm _emit 0x0f 
		__asm _emit 0x57 
		__asm _emit 0xc0

    }
	__except ( EXCEPTION_EXECUTE_HANDLER) 
	{
		bResult = FALSE;
		
	}
	return bResult;
}



BOOL SelectGFunc()
{
	if (IsSupportSSE())
		CopyFile("SS3DGFuncSSE.dll","SS3DGFunc.dll",FALSE);
	else
		CopyFile("SS3DGFuncN.dll","SS3DGFunc.dll",FALSE);
	return TRUE;
}



void ConvSpecFormatFile(char * szKey, char * szLocalFile, char * szRemoteFile)
{
	CFileStatus aStatus;
	if ( CFile::GetStatus(szLocalFile, aStatus) == TRUE )
	{
		CTime creationTime = aStatus.m_ctime;
		CString strTime = "[";
		strTime += szKey;
		strTime += "].txt";
		strcpy(szRemoteFile, (LPTSTR)(LPCTSTR)strTime);
	}
}

void ConvBugFormatFile(char * szKey, char * szLocalFile, char * szRemoteFile)
{
	CFileStatus aStatus;
	if ( CFile::GetStatus(szLocalFile, aStatus) == TRUE )
	{
		CTime creationTime = aStatus.m_ctime;
		CString strTime = "[";
		strTime += szKey;
		strTime += "]";
		strTime += creationTime.Format("(%Y-%m-%d)%H-%M-%S");
		strTime += szLocalFile;
		strcpy(szRemoteFile, (LPTSTR)(LPCTSTR)strTime);
	}
}



void OnStartTimer(HWND hWnd) 
{
	//g_iTimer = SetTimer(1, 1000, 0);
	g_iTimer = SetTimer(hWnd, 1, 1000, 0);	
}

void OnStopTimer(HWND hWnd) 
{
	KillTimer(hWnd, g_iTimer);  
}


void StartLog(VOID* pParameter)
{
	//CLogZiller logZ;
	//logZ.FtpConnect();
	//logZ.Upload("aa",CLIENTBUGFILE);
	//logZ.FtpDisconnect();
	DWORD id = 0;
	//HANDLE h = CreateThread(0, 0, ftpThread, this, 0, &id);
	HANDLE h = CreateThread(0, 0, ftpThread, pParameter, 0, &id);
	CloseHandle(h);
}


DWORD WINAPI ftpThread(LPVOID p)
{
	//CString commandLine = AfxGetApp()->m_lpCmdLine;
	//if ( commandLine == "" ) commandLine = "_NO_ID_";

	
	CHardwareInfo HWInfo;

	//BYTE MacAddress[64];
	//HWInfo.GetMacAddressFromNetBIOS(MacAddress);
	//HWInfo.GetMacAddressFromSock(MacAddress);

	CString commandLine = HWInfo.GetLocalIP();

	// 071101 웅주, 64비트 CPU에서 GetDllVersion() 안의 GetFileVersionInfoSize()가 정상적으로
	// 실행되지 않는다. 이에 따라 주석처리

	//CString str = HWInfo.GetInfoToString();
	//((CLogReporterDlg*)p)->WriteFile(SPECFILE, 0, (LPTSTR)(LPCTSTR)str);
	//WriteFile(SPECFILE, 0, (LPTSTR)(LPCTSTR)str);

	ZBugZiller aBug;
	aBug.SetFTPInfo(FTP_IP, FTP_PORT);
	aBug.SetAccount(FTP_USER_ID, FTP_USER_PASSWORD);
//	aBug.ConnectFTP();

	
	int cac = 0;//_access(CLIENTBUGFILE,0);
	int mac = _access(MAPCHANGEERRORFILE,0);
	int rac = _access(RECVERRORFILE,0);

	if(cac != -1 || mac != -1 || rac != -1)
	{
		char szConvFile[256];
		ConvSpecFormatFile((LPTSTR)(LPCTSTR)commandLine, SPECFILE, szConvFile);
		aBug.Upload(SPECFILE, szConvFile);
		//ConvBugFormatFile((LPTSTR)(LPCTSTR)commandLine, CLIENTBUGFILE, szConvFile);
		//aBug.Upload(CLIENTBUGFILE, szConvFile);
		ConvBugFormatFile((LPTSTR)(LPCTSTR)commandLine, MAPCHANGEERRORFILE, szConvFile);
		aBug.Upload(MAPCHANGEERRORFILE, szConvFile);
		ConvBugFormatFile((LPTSTR)(LPCTSTR)commandLine, RECVERRORFILE, szConvFile);
		aBug.Upload(RECVERRORFILE, szConvFile);
	}
	
	DeleteFile(CLIENTBUGFILE);
	//((CLogReporterDlg*)p)->OnStopTimer();
	//((CLogReporterDlg*)p)->ExecuteMHAutoPatchEXE();
	//((CLogReporterDlg*)p)->PostMessage(WM_CLOSE);

#ifdef _FOR_CHINA_
	//((CLogReporterChinaDlg*)p)->m_bEnd = TRUE;

#else
	((CLogReporterDlg*)p)->m_bEnd = TRUE;

#endif	

	return 0;
}


void WriteFile(const char *fn, int type, char * buffer)
{
	FILE	*fp;
	if(type == 0)
	{
		fp = fopen(fn, "wt");
		fwrite(buffer, strlen(buffer), 1, fp);
		fclose(fp);
	}
	else if(type == 1)
	{
		fp = fopen(fn,"a+t");
		fprintf(fp,"\n%s", buffer);
		fclose(fp);
	}
}


//void ExecuteMHAutoPatchEXE()
void ExecuteMHAutoPatchEXE(char* pszExecuteFileName, char* pszArg)
{
	//////////////////////////////////////////////////////////////////////////
	// 바꿀파일의 갯수
	const int arraySize = 2;
	//////////////////////////////////////////////////////////////////////////
	
	FAILPAIR files[ arraySize ];
#ifdef _DEBUG
	//*.new 파일은 릴리즈 파일이므로 디버그로 바꾸는 것은 오히려 문제를 발생시킬 수 있다.
	//files[temp++].Set( LAUNCHER_DEBUG,"LunaLauncher.new");
#else
	files[ 0 ].Set( LAUNCHER_RELEASE,"LunaLauncher.new");
#endif
	files[ 1 ].Set("SS3DGFunc.dll","SS3DGFunc.new");	

	for(int n=0;n< arraySize;++n)
	{
		// TODO: Place code here.
		FILE* fp = fopen(files[n].newfilename,"rb");
		if(fp != NULL)
		{
			fclose(fp);
			while(1)
			{
				if(DeleteFile(files[n].oldfilename) == TRUE)
					break;
				Sleep(500);
			}
			BOOL rt = MoveFile(files[n].newfilename,files[n].oldfilename);
			assert(rt);
		}
	}

	char szDir[256];
	GetCurrentDirectory(256, szDir);
	CString strFullPath;
	//strFullPath.Format("%s\\%s", szDir,LAUNCHER_RELEASE);
	strFullPath.Format("%s\\%s", szDir, pszExecuteFileName);
	//int rt = _execl((LPTSTR)(LPCTSTR)strFullPath,"a",NULL);
	int rt = _execl((LPTSTR)(LPCTSTR)strFullPath, pszArg, pszArg, NULL);
}