#ifndef _LOG_REPORTER_GLOBAL_
#define _LOG_REPORTER_GLOBAL_



/// Constants 

#define LAUNCHER_RELEASE		"LunaLauncher.exe"
#define LAUNCHER_DEBUG			"LunaLauncher_d.exe"
#define CAPTION					"Luna Online"

#define FTP_IP					"211.233.41.139"
#define FTP_PORT				3100
#define FTP_USER_ID				"bugreporter"
#define FTP_USER_PASSWORD		"bug0!!##"
#define SPECFILE				"SpecInfo.txt"
#define CLIENTBUGFILE			"assert.txt"
#define MAPCHANGEERRORFILE		"debug.txt"
#define RECVERRORFILE			"recv.txt"



/// Types

struct FAILPAIR
{
	char oldfilename[MAX_PATH];
	char newfilename[MAX_PATH];
	void Set(char* oldf,char* newf)
	{
		strcpy(oldfilename,oldf);
		strcpy(newfilename,newf);
	}
};



/// Global variables

extern INT g_iTimer;


/// Function prototypes

BOOL IsSupportSSE();
BOOL SelectGFunc();
void ConvBugFormatFile(char * szKey, char * szLocalFile, char * szRemoteFile);
void ConvSpecFormatFile(char * szKey, char * szLocalFile, char * szRemoteFile);

void OnStartTimer(HWND hWnd);
void OnStopTimer(HWND hWnd);
void StartLog(VOID* pParameter);

void WriteFile(const char *fn, int type, char * buffer);
//void ExecuteMHAutoPatchEXE();	
void ExecuteMHAutoPatchEXE(char* pszExecuteFileName, char* pszArg);

static DWORD WINAPI ftpThread(LPVOID p);


#endif